/**
 * @file peripheral.c
 * @brief Module to handle GPIO-based buttons and LEDs using a work queue.
 *
 * This module provides a thread-safe interface for controlling LEDs and handling button
 * presses on the nRF5340 DK. It implements the following features:
 *
 * - Thread-safe interface for LED control and button event handling
 * - Dedicated work queue for asynchronous event processing
 * - Software debouncing for buttons to prevent spurious events
 * - Non-blocking LED control functions with configurable parameters
 * - Atomic operations for resource management and thread safety
 * - Comprehensive error handling and logging
 *
 * The module is designed for the nRF5340 Development Kit and uses Zephyr RTOS
 * features such as GPIO drivers, work queues, and atomic operations.
 */

#include "peripheral.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(peripheral_module, CONFIG_LOG_DEFAULT_LEVEL);

/* ZBUS channel for button events */
ZBUS_CHAN_DEFINE(btn_event_chan, btn_event_msg_t, NULL, NULL, ZBUS_OBSERVERS(controller_sub),
    ZBUS_MSG_INIT(.event = BUTTON_1_PRESSED));

/* === Device Tree Aliases === */
/**
 * @brief Device Tree node aliases for buttons and LEDs
 *
 * These macros define the Device Tree nodes for the buttons and LEDs on the nRF5340 DK.
 * Using aliases from the Device Tree provides several advantages:
 * - Hardware independence: The code doesn't need to know the specific GPIO pins
 * - Portability: The same code can work on different boards with different pin assignments
 * - Maintainability: Pin assignments can be changed in the Device Tree without modifying code
 */
#define BTN1_NODE DT_ALIAS(sw0) /* Button 1 node from Device Tree */
#define LED1_NODE DT_ALIAS(led0) /* LED 1 node from Device Tree */
#define LED2_NODE DT_ALIAS(led1) /* LED 2 node from Device Tree */
#define LED3_NODE DT_ALIAS(led2) /* LED 3 node from Device Tree */

/* === GPIO Configuration Flags === */
/**
 * @brief GPIO configuration flags for buttons and LEDs
 *
 * These flags define how the GPIO pins should be configured:
 * - BTN_FLAGS: Configure pins as inputs with pull-up resistors and edge-triggered interrupts
 * - LED_FLAGS: Configure pins as active-low outputs (writing 0 turns LED on, writing 1 turns it off)
 *
 * The button configuration uses pull-up resistors because the buttons on the nRF5340 DK
 * connect the pin to ground when pressed. The interrupt is configured to trigger on the
 * falling edge (when the button is pressed).
 */
#define BTN_FLAGS (GPIO_INPUT | GPIO_PULL_UP | GPIO_INT_EDGE_TO_ACTIVE)

/* === Button Debounce Configuration === */
/**
 * @brief Configuration for button debounce and LED behavior
 *
 * DEBOUNCE_DELAY_MS: The time in milliseconds to wait after a button press before
 * processing it. This helps filter out spurious signals caused by mechanical button bounce.
 * 50ms is a typical value that works well for most physical buttons.
 */
#define DEBOUNCE_DELAY_MS 50

/**
 * LED_FLAGS: Configuration for LED GPIO pins. The LEDs on the nRF5340 DK are active-low,
 * meaning they turn on when the GPIO pin is set to 0 and turn off when set to 1.
 */
#define LED_FLAGS (GPIO_OUTPUT_ACTIVE | GPIO_ACTIVE_LOW)

/* === LED GPIO Specs === */
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, { 0 });
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET_OR(LED2_NODE, gpios, { 0 });
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET_OR(LED3_NODE, gpios, { 0 });

/* === Button GPIO Specs === */
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(BTN1_NODE, gpios, { 0 });

/* === Callback Structs === */
static struct gpio_callback btn1_cb;

/* === Work Queue === */
struct peripheral_work_item {
  struct k_work work;
  enum peripheral_event_t event;
};

/* === LED Blink Work Items === */
struct led_blink_work {
  struct k_work_delayable dwork;
  enum led_t led;
  uint8_t blink_count;
  uint16_t on_time_ms;
  uint16_t off_time_ms;
  led_state_t end_state;
};

/* === Button Debounce Work Items === */
struct btn_debounce_work {
  struct k_work_delayable dwork;
  enum peripheral_event_t event;
};

/* === ZBUS Configuration === */

/* Message type for LED tasks */
typedef struct led_task_msg {
  enum peripheral_event_t event;
} led_task_msg_t;

/* ZBUS channel for LED tasks */
ZBUS_CHAN_DEFINE(led_task_chan, struct led_task_msg, NULL, NULL, ZBUS_OBSERVERS(led_task_listener),
    ZBUS_MSG_INIT(.event = LED_1_BLINK));

/* === Work Queue Configuration === */
/**
 * @brief Work queue configuration for asynchronous event handling
 *
 * The peripheral module uses a dedicated work queue to handle all events asynchronously.
 * This design provides several benefits:
 * - Button callbacks (which run in interrupt context) return quickly, improving system responsiveness
 * - LED operations don't block the calling thread, allowing for concurrent operations
 * - Multiple events can be queued and processed in sequence
 * - Work items can be scheduled with delays for timing-sensitive operations (like LED blinking)
 *
 * The work queue thread has a medium-high priority to ensure responsive handling of
 * peripheral events without interfering with critical system tasks.
 */
#define WORK_QUEUE_STACK_SIZE 1024 /* Stack size for the work queue thread */
#define WORK_QUEUE_PRIORITY K_PRIO_PREEMPT(8) /* Medium-high priority for responsive handling */

K_THREAD_STACK_DEFINE(peripheral_work_q_stack, WORK_QUEUE_STACK_SIZE);
static struct k_work_q peripheral_work_q;

/* === Work Item Pools === */
#define MAX_WORK_ITEMS 16
static struct peripheral_work_item work_items[MAX_WORK_ITEMS];
static atomic_t work_item_idx;

#define MAX_BLINK_WORKS 4
static struct led_blink_work blink_works[MAX_BLINK_WORKS];
static atomic_t blink_work_idx;

#define MAX_DEBOUNCE_WORKS 4
static struct btn_debounce_work debounce_works[MAX_DEBOUNCE_WORKS];
static atomic_t debounce_work_idx;

/* === Button State Tracking === */
static atomic_t btn_pressed[4];

/* === Function Declarations === */
int peripheral_set_led(enum led_t led, led_state_t state);
int peripheral_set_led_blink_async(
    enum led_t led, uint8_t count, uint16_t on_time_ms, uint16_t off_time_ms, led_state_t end_state);
int peripheral_publish_led_task(enum peripheral_event_t event);
static void enqueue_event(enum peripheral_event_t evt);

/**
 * @brief Listener callback function for LED task messages
 *
 * This function is called whenever a message is published to the led_task_chan.
 * It creates a work item for the peripheral work queue to handle the LED operation
 * asynchronously.
 *
 * @param chan Pointer to the channel that triggered the callback
 */
static void led_task_listener_callback(const struct zbus_channel* chan)
{
  const struct led_task_msg* msg = zbus_chan_const_msg(chan);

  LOG_DBG("Received LED task message, event: %d", msg->event);

  /* Create a work item for the peripheral work queue */
  enqueue_event(msg->event);
}

/* ZBUS listener for LED tasks */
ZBUS_LISTENER_DEFINE(led_task_listener, led_task_listener_callback);

/**
 * @brief Handles peripheral events from the work queue
 *
 * This function processes peripheral events that have been queued for asynchronous handling.
 * It extracts the event type from the work item and performs the appropriate action based on
 * the event type. The function handles the following types of events:
 * - LED control events (turn on, turn off, blink)
 * - Button press events (with external callback notification)
 *
 * For LED events, it calls the appropriate LED control function.
 * For button events, it logs the event and calls the external handler if registered.
 *
 * @param work Pointer to the work item structure containing the event to process
 */
static void handle_event_work(struct k_work* work)
{
  struct peripheral_work_item* item = CONTAINER_OF(work, struct peripheral_work_item, work);
  enum peripheral_event_t evt = item->event;

  switch (evt) {
  case LED_1_ON:
    peripheral_set_led(LED_1, LED_ON);
    break;
  case LED_1_OFF:
    peripheral_set_led(LED_1, LED_OFF);
    break;
  case LED_1_BLINK:
    peripheral_set_led_blink_async(LED_1, 1, 500, 500, LED_OFF);
    break;
  case LED_2_ON:
    peripheral_set_led(LED_2, LED_ON);
    break;
  case LED_2_OFF:
    peripheral_set_led(LED_2, LED_OFF);
    break;
  case LED_2_BLINK:
    peripheral_set_led_blink_async(LED_2, 1, 500, 500, LED_OFF);
    break;
  case LED_3_ON:
    peripheral_set_led(LED_3, LED_ON);
    break;
  case LED_3_OFF:
    peripheral_set_led(LED_3, LED_OFF);
    break;
  case LED_3_BLINK:
    peripheral_set_led_blink_async(LED_3, 1, 500, 500, LED_OFF);
    break;
  case BTN_1:
    LOG_DBG("Button 1 pressed! (internal)");
    {
      // Publish to button event channel on ZBUS
      btn_event_msg_t msg = { .event = BUTTON_1_PRESSED };
      zbus_chan_pub(&btn_event_chan, &msg, K_NO_WAIT);
    }
    break;
  default:
    LOG_WRN("Unknown peripheral event: %d", evt);
    break;
  }
}

/**
 * @brief Enqueues a peripheral event for asynchronous processing
 *
 * This function safely adds a peripheral event to the work queue for asynchronous processing.
 * It uses atomic operations to allocate a work item from the pool in a thread-safe manner,
 * initializes it with the event data, and submits it to the peripheral work queue.
 *
 * The function implements a circular buffer pattern for work item allocation:
 * - Uses atomic increment to get the next available index
 * - Uses modulo operation to wrap around when reaching the end of the array
 * - Initializes the work item with the event and handler function
 * - Submits the work item to the dedicated peripheral work queue
 *
 * This approach ensures that events can be safely enqueued from any context,
 * including interrupt handlers and other threads.
 *
 * @param evt The peripheral event to enqueue for processing
 */
static void enqueue_event(enum peripheral_event_t evt)
{
  uint32_t idx = atomic_inc(&work_item_idx) % MAX_WORK_ITEMS;
  struct peripheral_work_item* item = &work_items[idx];
  item->event = evt;
  k_work_init(&item->work, handle_event_work);
  k_work_submit_to_queue(&peripheral_work_q, &item->work);
}

/* === Button Debounce Handler === */
/**
 * @brief Handler for button debounce work items
 *
 * This function is called after the debounce delay has elapsed. It processes the button
 * event by resetting the button state and enqueueing the event for further processing.
 *
 * The debounce mechanism solves several common issues with physical buttons:
 * - Prevents multiple events from being generated when a button bounces during press or release
 * - Ensures consistent behavior across different button types and conditions
 * - Reduces CPU load by filtering out spurious button events
 * - Improves user experience by providing predictable button response
 *
 * Implementation details:
 * - Uses atomic operations to track button state for thread safety
 * - Resets button state only after the debounce period to prevent re-triggering
 * - Enqueues the event to the work queue for asynchronous processing
 * - Maps internal button events to the appropriate button index
 *
 * @param work Pointer to the work item structure containing the delayed work
 */
static void btn_debounce_handler(struct k_work* work)
{
  struct btn_debounce_work* debounce_work = CONTAINER_OF(work, struct btn_debounce_work, dwork.work);
  enum peripheral_event_t evt = debounce_work->event;

  // Determine which button this is for
  int btn_idx = -1;
  switch (evt) {
  case BTN_1:
    btn_idx = 0;
    break;
  default:
    return; // Not a button event
  }

  // Reset the button state to allow future button presses to be detected
  atomic_set(&btn_pressed[btn_idx], 0);

  // Process the button event
  enqueue_event(evt);
}

/**
 * @brief Schedules a button event for debounce processing
 *
 * This function creates a delayed work item to process a button event after
 * the debounce delay has elapsed. It uses atomic operations to allocate a
 * debounce work item from the pool in a thread-safe manner.
 *
 * The debounce mechanism works as follows:
 * 1. When a button interrupt occurs, this function is called
 * 2. A work item is scheduled to execute after DEBOUNCE_DELAY_MS milliseconds
 * 3. After the delay, btn_debounce_handler is called to process the event
 * 4. The button state is reset to allow future button presses
 *
 * This approach ensures that rapid button presses or bouncing contacts
 * don't generate multiple events, improving the reliability of button input.
 *
 * @param evt The button event to schedule for debounce processing
 */
static void schedule_debounce(enum peripheral_event_t evt)
{
  uint32_t idx = atomic_inc(&debounce_work_idx) % MAX_DEBOUNCE_WORKS;
  struct btn_debounce_work* debounce_work = &debounce_works[idx];

  debounce_work->event = evt;
  k_work_init_delayable(&debounce_work->dwork, btn_debounce_handler);
  k_work_schedule_for_queue(&peripheral_work_q, &debounce_work->dwork, K_MSEC(DEBOUNCE_DELAY_MS));
}

/* === Button Interrupt Handlers === */
/**
 * @brief Interrupt handlers for button press events
 *
 * These functions are called directly from the GPIO interrupt context when a button
 * is pressed. They use atomic compare-and-swap operations to check if the button
 * is already being processed (for debounce purposes) and schedule the event for
 * processing if it's a new press.
 *
 * Important characteristics of these handlers:
 * - They run in interrupt context, so they must be as brief as possible
 * - They use atomic operations to safely check and update button state
 * - They don't process the button event directly, but schedule it for later processing
 * - They implement the first stage of the debounce mechanism
 *
 * The atomic_cas() function performs an atomic Compare-And-Swap operation:
 * - It checks if the current value is 0 (button not being processed)
 * - If it is, it sets it to 1 (button being processed) and returns true
 * - If not, it leaves the value unchanged and returns false
 * This ensures that even if multiple interrupts occur, the button is only processed once.
 */
static void button1_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  // Check if button is already being processed (debounce)
  if (atomic_cas(&btn_pressed[0], 0, 1)) {
    schedule_debounce(BTN_1);
  }
}

/* === LED Blink Handler === */
/**
 * @brief Handler for LED blink work items
 *
 * This function implements non-blocking LED blinking by toggling the LED state and
 * rescheduling itself until the requested number of blink cycles is complete.
 *
 * Key features of the LED blink implementation:
 * - Non-blocking operation using Zephyr's delayable work items
 * - Support for configurable on/off durations for precise timing control
 * - Ability to specify the number of blink cycles (including infinite blinking)
 * - Configurable final LED state when blinking completes
 * - Independent control of multiple LEDs simultaneously
 *
 * Implementation details:
 * - Uses a static array to track the current state of each LED
 * - Toggles LED state between ON and OFF at specified intervals
 * - Decrements blink counter only after a complete on-off cycle
 * - Schedules the next state change with appropriate delay
 * - Sets the final LED state when blinking is complete
 * - Handles edge cases like invalid LED identifiers
 *
 * @param work Pointer to the work item structure containing the delayed work
 */
static void led_blink_handler(struct k_work* work)
{
  struct led_blink_work* blink_work = CONTAINER_OF(work, struct led_blink_work, dwork.work);
  static led_state_t current_state[4] = { LED_OFF, LED_OFF, LED_OFF, LED_OFF };
  enum led_t led = blink_work->led;

  if (led >= LED_1 && led <= LED_4) {
    // Toggle the LED state
    current_state[led] = (current_state[led] == LED_ON) ? LED_OFF : LED_ON;
    peripheral_set_led(led, current_state[led]);

    // Decrement blink count if we just completed a full on-off cycle
    if (current_state[led] == LED_OFF && blink_work->blink_count > 0) {
      blink_work->blink_count--;
    }

    // Schedule next toggle or finish
    if (blink_work->blink_count > 0 || current_state[led] == LED_ON) {
      // If we still have blinks remaining or we're in the ON state of a cycle,
      // schedule the next state change
      uint16_t delay = (current_state[led] == LED_ON) ? blink_work->on_time_ms : blink_work->off_time_ms;
      k_work_schedule_for_queue(&peripheral_work_q, &blink_work->dwork, K_MSEC(delay));
    } else {
      // Set final state if we're done blinking
      peripheral_set_led(led, blink_work->end_state);
    }
  }
}

/* === LED Control API === */
/**
 * @brief Sets the state of a specific LED
 *
 * This function provides direct control over an LED's state (on or off).
 * It maps the logical LED identifier to the corresponding GPIO pin and sets
 * its state accordingly.
 *
 * The function handles the following:
 * - Mapping from logical LED identifiers (LED_1 to LED_4) to physical GPIO pins
 * - Setting the GPIO pin state based on the requested LED state
 * - Error handling and logging for failed operations
 *
 * Note: Due to the active-low configuration of the LEDs on the nRF5340 DK,
 * LED_ON (1) actually sets the GPIO pin to logical 0, and LED_OFF (0) sets it to logical 1.
 * This is handled internally by the gpio_pin_set_dt function using the GPIO_ACTIVE_LOW flag.
 *
 * @param led   The LED identifier (LED_1 through LED_4)
 * @param state The desired state: LED_ON to turn on, LED_OFF to turn off
 *
 * @return 0 on success, negative error code on failure:
 *         -EINVAL if the LED identifier is invalid
 *         Other negative values from gpio_pin_set_dt() if the GPIO operation fails
 */
int peripheral_set_led(enum led_t led, led_state_t state)
{
  int ret = -EINVAL;

  switch (led) {
  case LED_1:
    ret = gpio_pin_set_dt(&led1, state);
    break;
  case LED_2:
    ret = gpio_pin_set_dt(&led2, state);
    break;
  case LED_3:
    ret = gpio_pin_set_dt(&led3, state);
    break;
  }

  if (ret < 0) {
    LOG_ERR("Failed to set LED %d to state %d: %d", led, state, ret);
  }

  return ret;
}

/**
 * @brief Makes an LED blink once with default parameters
 *
 * This is a simplified version of the LED blinking functionality that uses
 * default parameters for a single blink cycle. It's provided for backward
 * compatibility and convenience when simple blinking is needed.
 *
 * Default behavior:
 * - Blinks exactly once (one ON-OFF cycle)
 * - ON state lasts for 500ms
 * - OFF state lasts for 500ms
 * - LED ends in the OFF state
 *
 * This function is non-blocking and returns immediately after scheduling the blink.
 *
 * @param led The LED to blink (LED_1 through LED_4)
 *
 * @return 0 on success, negative error code on failure (see peripheral_set_led_blink_async)
 */
int peripheral_set_led_blink(enum led_t led)
{
  // For backward compatibility, use the async version with default parameters
  return peripheral_set_led_blink_async(led, 1, 500, 500, LED_OFF);
}

/**
 * @brief Makes an LED blink asynchronously with configurable parameters
 *
 * This function provides a non-blocking way to blink an LED with precise control over:
 * - Number of blink cycles
 * - Duration of ON and OFF states
 * - Final state after blinking completes
 *
 * The function works by immediately turning the LED on and scheduling a delayed work
 * item to handle the subsequent state changes. The work item reschedules itself until
 * the requested number of blink cycles is complete.
 *
 * Use cases:
 * - Status indication (e.g., connection status, error conditions)
 * - User notifications (e.g., operation complete, input required)
 * - Visual feedback for user interactions
 * - Timing indicators (e.g., countdown visualization)
 *
 * Implementation details:
 * - Thread-safe using atomic operations for work item allocation
 * - Non-blocking operation that returns immediately
 * - Efficient resource usage through work item pooling
 * - Comprehensive error handling and validation
 *
 * @param led The LED to blink (LED_1 through LED_4)
 * @param count Number of complete on-off cycles (0 for indefinite blinking)
 * @param on_time_ms Duration in milliseconds for the ON state
 * @param off_time_ms Duration in milliseconds for the OFF state
 * @param end_state Final state of the LED after blinking completes (LED_ON or LED_OFF)
 *
 * @return 0 on success, -EINVAL for invalid LED, -EBUSY if work queue is full,
 *         or other negative error code if LED control fails
 */
int peripheral_set_led_blink_async(
    enum led_t led, uint8_t count, uint16_t on_time_ms, uint16_t off_time_ms, led_state_t end_state)
{
  if (led < LED_1 || led > LED_4) {
    return -EINVAL;
  }

  // Get the next available blink work item from the pool
  uint32_t idx = atomic_inc(&blink_work_idx) % MAX_BLINK_WORKS;
  struct led_blink_work* blink_work = &blink_works[idx];

  // Configure the blink parameters
  blink_work->led = led;
  blink_work->blink_count = count;
  blink_work->on_time_ms = on_time_ms;
  blink_work->off_time_ms = off_time_ms;
  blink_work->end_state = end_state;

  // Initialize the delayable work item
  k_work_init_delayable(&blink_work->dwork, led_blink_handler);

  // Start with LED on
  int ret = peripheral_set_led(led, LED_ON);
  if (ret < 0) {
    return ret;
  }

  // Schedule the first state change
  return k_work_schedule_for_queue(&peripheral_work_q, &blink_work->dwork, K_MSEC(on_time_ms)) ? 0 : -EBUSY;
}

/* === Module Initialization === */
/**
 * @brief Configures an LED GPIO pin
 *
 * This helper function configures a GPIO pin for use as an LED output.
 * It performs the following steps:
 * 1. Verifies that the GPIO device is ready for operation
 * 2. Configures the pin as an output with active-low logic
 *
 * The function uses the LED_FLAGS macro which sets the pin as an active-low output,
 * meaning that writing a logical '1' to the pin will turn the LED off, and writing
 * a logical '0' will turn it on. This is typical for LEDs on development boards
 * where one terminal is connected to VCC and the GPIO pin controls the ground path.
 *
 * @param led Pointer to the GPIO device tree specification for the LED,
 *            obtained from the device tree using GPIO_DT_SPEC_GET_OR macro
 *
 * @return 0 on success, -ENODEV if the device is not ready,
 *         or other negative error code from gpio_pin_configure_dt
 */
static int setup_led(const struct gpio_dt_spec* led)
{
  // Check if the GPIO device is ready
  if (!device_is_ready(led->port)) {
    LOG_ERR("LED device not ready");
    return -ENODEV;
  }

  // Configure the GPIO pin as an output with active-low logic
  int ret = gpio_pin_configure_dt(led, LED_FLAGS);
  if (ret < 0) {
    LOG_ERR("Failed to configure LED: %d", ret);
  }
  return ret;
}

/**
 * @brief Configures a button GPIO pin with interrupt
 *
 * This helper function configures a GPIO pin for use as a button input with
 * interrupt capability. It performs the following steps:
 * 1. Checks if the GPIO device is ready
 * 2. Configures the pin as an input with pull-up resistor
 * 3. Configures the pin to generate interrupts on active edge
 * 4. Initializes and adds the callback function for the interrupt
 *
 * The function uses the BTN_FLAGS macro which configures the pin as an input with
 * a pull-up resistor and edge-triggered interrupts. With this configuration:
 * - The pin normally reads as '1' (high) when the button is not pressed
 * - When the button is pressed, it connects the pin to ground, causing it to read as '0' (low)
 * - An interrupt is generated on the falling edge (when the button is pressed)
 *
 * Error handling is comprehensive, with each step checked for errors and appropriate
 * error codes returned to the caller. This allows the caller to identify exactly
 * which part of the configuration failed.
 *
 * @param btn Pointer to the GPIO device tree specification for the button,
 *            obtained from the device tree using GPIO_DT_SPEC_GET_OR macro
 * @param cb Pointer to the GPIO callback structure to initialize,
 *           this structure must persist as long as the callback is active
 * @param handler Function to call when the button interrupt occurs,
 *               this function will run in interrupt context and should be brief
 *
 * @return 0 on success, -ENODEV if the device is not ready,
 *         or other negative error code from GPIO configuration functions:
 *         - gpio_pin_configure_dt(): if pin configuration fails
 *         - gpio_pin_interrupt_configure_dt(): if interrupt configuration fails
 *         - gpio_add_callback(): if adding the callback fails
 */
static int setup_button(const struct gpio_dt_spec* btn, struct gpio_callback* cb, gpio_callback_handler_t handler)
{
  // Check if the GPIO device is ready
  if (!device_is_ready(btn->port)) {
    LOG_ERR("Button device not ready");
    return -ENODEV;
  }

  // Configure the GPIO pin as an input with pull-up resistor
  int ret = gpio_pin_configure_dt(btn, BTN_FLAGS);
  if (ret < 0) {
    LOG_ERR("Failed to configure button: %d", ret);
    return ret;
  }

  // Configure the pin to generate interrupts on active edge
  ret = gpio_pin_interrupt_configure_dt(btn, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret < 0) {
    LOG_ERR("Failed to configure button interrupt: %d", ret);
    return ret;
  }

  // Initialize the callback structure with the handler function
  gpio_init_callback(cb, handler, BIT(btn->pin));

  // Add the callback to the GPIO device
  ret = gpio_add_callback(btn->port, cb);
  if (ret < 0) {
    LOG_ERR("Failed to add button callback: %d", ret);
    return ret;
  }

  return 0;
}

/**
 * @brief Initializes the peripheral module
 *
 * This function performs the following initialization steps:
 * 1. Creates and starts a dedicated work queue for peripheral events
 * 2. Initializes atomic counters for work item allocation
 * 3. Initializes button state tracking for debounce handling
 * 4. Configures all LEDs and sets them to OFF state
 * 5. Stores the button event handler callback
 * 6. Configures all buttons with interrupt handlers
 *
 * Initialization sequence details:
 * - Work queue: Creates a dedicated thread with specified stack size and priority
 * - Atomic counters: Ensures thread-safe allocation of work items from pools
 * - Button state: Initializes atomic variables for debounce state tracking
 * - LEDs: Configures GPIO pins as outputs with active-low logic
 * - Buttons: Configures GPIO pins as inputs with pull-up resistors and interrupts
 *
 * Error handling:
 * - Each initialization step is checked for errors
 * - Detailed error messages are logged to aid in debugging
 * - Function returns immediately if any step fails
 * - Error codes from lower-level functions are propagated to the caller
 *
 * @return 0 on success, or a negative error code if initialization fails:
 *         -ENODEV: if a GPIO device is not ready
 *         Other negative error codes from gpio_pin_configure_dt(), gpio_pin_interrupt_configure_dt(),
 *         or gpio_add_callback() functions
 */
int peripheral_init(void)
{
  int ret;

  // Initialize work queue for asynchronous event processing
  k_work_queue_start(&peripheral_work_q, peripheral_work_q_stack, K_THREAD_STACK_SIZEOF(peripheral_work_q_stack),
      WORK_QUEUE_PRIORITY, NULL);
  k_thread_name_set(&peripheral_work_q.thread, "peripheral_work_q");

  // Initialize atomic counters for work item allocation
  atomic_set(&work_item_idx, 0);
  atomic_set(&blink_work_idx, 0);
  atomic_set(&debounce_work_idx, 0);

  // Initialize button state tracking for debounce handling
  for (int i = 0; i < 4; i++) {
    atomic_set(&btn_pressed[i], 0);
  }

  // Setup LEDs with proper error handling
  ret = setup_led(&led1);
  if (ret < 0) {
    LOG_ERR("Failed to setup LED 2: %d", ret);
    return ret;
  }
  peripheral_set_led(LED_1, LED_OFF);

  ret = setup_led(&led2);
  if (ret < 0) {
    LOG_ERR("Failed to setup LED 3: %d", ret);
    return ret;
  }
  peripheral_set_led(LED_2, LED_OFF);

  ret = setup_led(&led3);
  if (ret < 0) {
    LOG_ERR("Failed to setup LED 4: %d", ret);
    return ret;
  }
  peripheral_set_led(LED_3, LED_OFF);

  // Setup buttons with interrupt handlers
  ret = setup_button(&button1, &btn1_cb, button1_pressed);
  if (ret < 0) {
    LOG_ERR("Failed to setup button 1: %d", ret);
    return ret;
  }

  LOG_DBG("Peripheral module initialized with work queue");
  return 0;
}

/* === Public API Function === */

/**
 * @brief Publish a message to the LED task channel
 *
 * This function publishes a message to the LED task channel, which will be processed
 * by the LED task listener callback. The message contains a peripheral event that
 * specifies which LED operation to perform.
 *
 * @param event The peripheral event to publish (should be an LED-related event)
 * @return 0 on success, negative errno on failure
 */
int peripheral_publish_led_task(enum peripheral_event_t event)
{
  struct led_task_msg msg = { .event = event };

  int ret = zbus_chan_pub(&led_task_chan, &msg, K_MSEC(100));
  if (ret) {
    LOG_ERR("Failed to publish LED task message: %d", ret);
  }

  return ret;
}
