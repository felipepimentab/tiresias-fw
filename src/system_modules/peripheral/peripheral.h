/**
 * @file peripheral.h
 * @brief Public API for the Peripheral module (LEDs and Buttons).
 *
 * This module provides a thread-safe interface for controlling LEDs and handling button
 * presses on the nRF5340 DK. It implements the following features:
 *
 * - Thread-safe interface for LED control and button event handling
 * - Callback-based button event notification system
 * - Simple API for LED control (on/off/blink)
 * - Advanced LED blinking with configurable parameters
 * - Comprehensive error handling with descriptive return codes
 *
 * The module is designed for the nRF5340 Development Kit and internally uses
 * Zephyr RTOS features such as work queues, atomic operations, and GPIO drivers
 * to provide a robust and non-blocking interface.
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief LED identifiers for the nRF5340 DK
 *
 * These enumeration values identify the four LEDs available on the nRF5340 DK.
 * They are used as parameters to the LED control functions to specify which
 * LED to manipulate.
 *
 * The LEDs are mapped as follows:
 * - LED_1: The first LED (typically labeled LED1 on the board)
 * - LED_2: The second LED (typically labeled LED2 on the board)
 * - LED_3: The third LED (typically labeled LED3 on the board)
 * - LED_4: The fourth LED (typically labeled LED4 on the board)
 */
typedef enum led_t {
  LED_1, /**< First LED on the board */
  LED_2, /**< Second LED on the board */
  LED_3, /**< Third LED on the board */
  LED_4, /**< Fourth LED on the board */
} led_t;

/**
 * @brief LED state values
 *
 * These values define the possible states for an LED. They are used as parameters
 * to the LED control functions to specify the desired LED state.
 *
 * Note: Although the LEDs on the nRF5340 DK are physically active-low (meaning they
 * turn on when the GPIO pin is set to 0), these values provide a logical abstraction
 * where LED_ON means the LED is illuminated and LED_OFF means it is not, regardless
 * of the underlying GPIO logic.
 */
typedef enum led_state_t {
  LED_OFF = 1, /**< LED is turned off (not illuminated) */
  LED_ON = 0, /**< LED is turned on (illuminated) */
} led_state_t;

/**
 * @brief Button press events
 *
 * These enumeration values represent button press events that can be reported
 * to the application through the button event handler callback.
 *
 * When a button press is detected and debounced, the corresponding event is
 * passed to the application's button event handler function that was registered
 * during initialization.
 *
 * The buttons are mapped as follows:
 * - BUTTON_1_PRESSED: Event when the first button (typically labeled SW1) is pressed
 * - BUTTON_2_PRESSED: Event when the second button (typically labeled SW2) is pressed
 * - BUTTON_3_PRESSED: Event when the third button (typically labeled SW3) is pressed
 * - BUTTON_4_PRESSED: Event when the fourth button (typically labeled SW4) is pressed
 */
typedef enum button_event_t {
  BUTTON_1_PRESSED, /**< First button press event */
  BUTTON_2_PRESSED, /**< Second button press event */
  BUTTON_3_PRESSED, /**< Third button press event */
  BUTTON_4_PRESSED, /**< Fourth button press event */
} button_event_t;

/* Message type for button events */
typedef struct btn_event_msg_t {
  enum button_event_t event;
} btn_event_msg_t;

/**
 * @brief Callback signature for button event handler
 *
 * This defines the function signature for the callback that will be invoked when
 * a button press event occurs. The application must provide a function matching
 * this signature during initialization if it wants to receive button events.
 *
 * The callback is invoked from the context of the peripheral module's work queue,
 * not directly from the interrupt handler. This means:
 * - It's safe to perform longer operations in the callback
 * - The callback doesn't need to be ISR-safe
 * - Multiple button events are properly serialized
 *
 * Important: The callback should still return reasonably quickly to avoid blocking
 * other peripheral operations that use the same work queue.
 *
 * @param event The button event that occurred (BUTTON_1_PRESSED through BUTTON_4_PRESSED)
 */
typedef void (*btn_ext_handler_t)(enum button_event_t event);

/**
 * @brief Internal peripheral event types (used by the work queue)
 *
 * This enumeration defines all possible events that can be processed by the
 * peripheral module's work queue. It includes both LED control events and
 * button press events.
 *
 * The events are processed asynchronously by the handle_event_work function,
 * which is scheduled on the work queue. This approach provides several benefits:
 * - Non-blocking operation for the caller
 * - Serialized processing of events
 * - Consistent timing for LED blinking
 * - Proper debouncing for button presses
 *
 * LED events:
 * - LED_x_ON: Turn on LED x
 * - LED_x_OFF: Turn off LED x
 * - LED_x_BLINK: Blink LED x according to configured parameters
 *
 * Button events:
 * - BTN_x: Button x has been pressed (after debounce)
 *
 * Note: This enum is for internal use only and should not be used directly
 * by applications. Instead, use the public API functions and the button
 * event handler callback.
 */
typedef enum peripheral_event_t {
  LED_1_ON, /**< Turn on LED 1 */
  LED_1_OFF, /**< Turn off LED 1 */
  LED_1_BLINK, /**< Blink LED 1 */
  LED_2_ON, /**< Turn on LED 2 */
  LED_2_OFF, /**< Turn off LED 2 */
  LED_2_BLINK, /**< Blink LED 2 */
  LED_3_ON, /**< Turn on LED 3 */
  LED_3_OFF, /**< Turn off LED 3 */
  LED_3_BLINK, /**< Blink LED 3 */
  LED_4_ON, /**< Turn on LED 4 */
  LED_4_OFF, /**< Turn off LED 4 */
  LED_4_BLINK, /**< Blink LED 4 */
  BTN_1, /**< Button 1 pressed event */
  BTN_2, /**< Button 2 pressed event */
  BTN_3, /**< Button 3 pressed event */
  BTN_4, /**< Button 4 pressed event */
} peripheral_event_t;

/**
 * @brief Initializes the peripheral module, including LEDs and buttons
 *
 * This function must be called before using any other peripheral module functions.
 * It performs the following initialization steps:
 * 1. Sets up the work queue for asynchronous event processing
 * 2. Initializes atomic counters for thread-safe operation
 * 3. Configures button state tracking for debounce handling
 * 4. Sets up all LEDs as outputs with initial OFF state
 * 5. Configures all buttons with pull-up resistors and interrupt handlers
 *
 * If any step fails (e.g., a device is not available), the function returns
 * immediately with an appropriate error code and logs the failure.
 *
 * @return 0 on success, or one of the following negative error codes:
 *         -ENODEV: If an LED or button device is not available
 *         -EIO: If GPIO configuration fails
 *         -EINVAL: If work queue initialization fails
 */
int peripheral_init(void);

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
 * This is handled internally by the GPIO driver using the GPIO_ACTIVE_LOW flag.
 *
 * @param led   The LED to control (LED_1 through LED_4)
 * @param state Desired state: LED_ON to turn on, LED_OFF to turn off
 *
 * @return 0 on success, or one of the following negative error codes:
 *         -EINVAL if the LED identifier is invalid
 *         Other negative values from the GPIO driver if the operation fails
 */
int peripheral_set_led(enum led_t led, led_state_t state);

/**
 * @brief Blinks an LED once with default timing (500ms on, 500ms off)
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
 * Internally, it calls peripheral_set_led_blink_async with the default parameters.
 *
 * @param led The LED to blink (LED_1 through LED_4)
 *
 * @return 0 on success, or one of the following negative error codes:
 *         -EINVAL if the LED identifier is invalid
 *         -EBUSY if the work queue is full
 *         Other negative values from the GPIO driver if the operation fails
 */
int peripheral_set_led_blink(enum led_t led);

/**
 * @brief Blinks an LED asynchronously with configurable parameters
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
 * @return 0 on success, or one of the following negative error codes:
 *         -EINVAL if the LED identifier is invalid
 *         -EBUSY if the work queue is full
 *         Other negative values from the GPIO driver if the operation fails
 */
int peripheral_set_led_blink_async(
    enum led_t led, uint8_t count, uint16_t on_time_ms, uint16_t off_time_ms, led_state_t end_state);

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
int peripheral_publish_led_task(enum peripheral_event_t event);

/* === For the nRF5340 Audio DK only === */
#define BOARD_RED()                                                                                                    \
  {                                                                                                                    \
    peripheral_publish_led_task(LED_1_ON);                                                                             \
    peripheral_publish_led_task(LED_2_OFF);                                                                            \
    peripheral_publish_led_task(LED_3_OFF);                                                                            \
    peripheral_publish_led_task(LED_4_OFF);                                                                            \
  }

#define BOARD_GREEN()                                                                                                  \
  {                                                                                                                    \
    peripheral_publish_led_task(LED_1_OFF);                                                                            \
    peripheral_publish_led_task(LED_2_ON);                                                                             \
    peripheral_publish_led_task(LED_3_OFF);                                                                            \
    peripheral_publish_led_task(LED_4_OFF);                                                                            \
  }

#define BOARD_BLUE()                                                                                                   \
  {                                                                                                                    \
    peripheral_publish_led_task(LED_1_OFF);                                                                            \
    peripheral_publish_led_task(LED_2_OFF);                                                                            \
    peripheral_publish_led_task(LED_3_ON);                                                                             \
    peripheral_publish_led_task(LED_4_OFF);                                                                            \
  }

#define BOARD_YELLOW()                                                                                                 \
  {                                                                                                                    \
    peripheral_publish_led_task(LED_1_OFF);                                                                            \
    peripheral_publish_led_task(LED_2_ON);                                                                             \
    peripheral_publish_led_task(LED_3_ON);                                                                             \
    peripheral_publish_led_task(LED_4_OFF);                                                                            \
  }

#define BOARD_PURPLE()                                                                                                 \
  {                                                                                                                    \
    peripheral_publish_led_task(LED_1_ON);                                                                             \
    peripheral_publish_led_task(LED_2_OFF);                                                                            \
    peripheral_publish_led_task(LED_3_ON);                                                                             \
    peripheral_publish_led_task(LED_4_OFF);                                                                            \
  }

#define BOARD_WHITE()                                                                                                  \
  {                                                                                                                    \
    peripheral_publish_led_task(LED_1_ON);                                                                             \
    peripheral_publish_led_task(LED_2_ON);                                                                             \
    peripheral_publish_led_task(LED_3_ON);                                                                             \
    peripheral_publish_led_task(LED_4_OFF);                                                                            \
  }

#define BOARD_OFF()                                                                                                    \
  {                                                                                                                    \
    peripheral_publish_led_task(LED_1_OFF);                                                                            \
    peripheral_publish_led_task(LED_2_OFF);                                                                            \
    peripheral_publish_led_task(LED_3_OFF);                                                                            \
    peripheral_publish_led_task(LED_4_OFF);                                                                            \
  }

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_H_ */