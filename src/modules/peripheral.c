/**
 * @file peripheral.c
 * @brief Module to handle GPIO-based buttons and LEDs using a work queue.
 */

#include "peripheral.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/atomic.h>

LOG_MODULE_REGISTER(peripheral_module, CONFIG_LOG_DEFAULT_LEVEL);

/* === Device Tree Aliases === */
#define BTN1_NODE DT_ALIAS(sw0)
#define BTN2_NODE DT_ALIAS(sw1)
#define BTN3_NODE DT_ALIAS(sw2)
#define BTN4_NODE DT_ALIAS(sw3)

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

/* === GPIO Configuration Flags === */
#define BTN_FLAGS (GPIO_INPUT | GPIO_PULL_UP | GPIO_INT_EDGE_TO_ACTIVE)

/* === Button Debounce Configuration === */
#define DEBOUNCE_DELAY_MS 50
#define LED_FLAGS (GPIO_OUTPUT_ACTIVE | GPIO_ACTIVE_LOW)

/* === LED GPIO Specs === */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, { 0 });
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, { 0 });
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET_OR(LED2_NODE, gpios, { 0 });
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET_OR(LED3_NODE, gpios, { 0 });

/* === Button GPIO Specs === */
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(BTN1_NODE, gpios, { 0 });
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET_OR(BTN2_NODE, gpios, { 0 });
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET_OR(BTN3_NODE, gpios, { 0 });
static const struct gpio_dt_spec button4 = GPIO_DT_SPEC_GET_OR(BTN4_NODE, gpios, { 0 });

/* === Callback Structs === */
static struct gpio_callback btn1_cb, btn2_cb, btn3_cb, btn4_cb;
static btn_ext_handler_t btn_ext_handler;

/* === Work Queue === */
struct peripheral_work_item {
  struct k_work work;
  enum peripheral_event event;
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
  enum peripheral_event event;
};

/* === Work Queue Configuration === */
#define WORK_QUEUE_STACK_SIZE 1024
#define WORK_QUEUE_PRIORITY K_PRIO_PREEMPT(8)

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
int peripheral_set_led_blink_async(enum led_t led, uint8_t count, uint16_t on_time_ms, 
                                  uint16_t off_time_ms, led_state_t end_state);

static void handle_event_work(struct k_work* work)
{
  struct peripheral_work_item* item = CONTAINER_OF(work, struct peripheral_work_item, work);
  enum peripheral_event evt = item->event;

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
  case LED_4_ON:
    peripheral_set_led(LED_4, LED_ON);
    break;
  case LED_4_OFF:
    peripheral_set_led(LED_4, LED_OFF);
    break;
  case LED_4_BLINK:
    peripheral_set_led_blink_async(LED_4, 1, 500, 500, LED_OFF);
    break;
  case BTN_1:
    LOG_INF("Button 1 pressed! (internal)");
    if (btn_ext_handler)
      btn_ext_handler(BUTTON_1_PRESSED);
    break;
  case BTN_2:
    LOG_INF("Button 2 pressed! (internal)");
    if (btn_ext_handler)
      btn_ext_handler(BUTTON_2_PRESSED);
    break;
  case BTN_3:
    LOG_INF("Button 3 pressed! (internal)");
    if (btn_ext_handler)
      btn_ext_handler(BUTTON_3_PRESSED);
    break;
  case BTN_4:
    LOG_INF("Button 4 pressed! (internal)");
    if (btn_ext_handler)
      btn_ext_handler(BUTTON_4_PRESSED);
    break;
  default:
    LOG_WRN("Unknown peripheral event: %d", evt);
    break;
  }
}

static void enqueue_event(enum peripheral_event evt)
{
  uint32_t idx = atomic_inc(&work_item_idx) % MAX_WORK_ITEMS;
  struct peripheral_work_item* item = &work_items[idx];
  item->event = evt;
  k_work_init(&item->work, handle_event_work);
  k_work_submit_to_queue(&peripheral_work_q, &item->work);
}

/* === Button Debounce Handler === */
static void btn_debounce_handler(struct k_work *work)
{
  struct btn_debounce_work *debounce_work = CONTAINER_OF(work, struct btn_debounce_work, dwork.work);
  enum peripheral_event evt = debounce_work->event;
  
  // Determine which button this is for
  int btn_idx = -1;
  switch (evt) {
    case BTN_1: btn_idx = 0; break;
    case BTN_2: btn_idx = 1; break;
    case BTN_3: btn_idx = 2; break;
    case BTN_4: btn_idx = 3; break;
    default: return; // Not a button event
  }
  
  // Reset the button state
  atomic_set(&btn_pressed[btn_idx], 0);
  
  // Process the button event
  enqueue_event(evt);
}

static void schedule_debounce(enum peripheral_event evt)
{
  uint32_t idx = atomic_inc(&debounce_work_idx) % MAX_DEBOUNCE_WORKS;
  struct btn_debounce_work *debounce_work = &debounce_works[idx];
  
  debounce_work->event = evt;
  k_work_init_delayable(&debounce_work->dwork, btn_debounce_handler);
  k_work_schedule_for_queue(&peripheral_work_q, &debounce_work->dwork, K_MSEC(DEBOUNCE_DELAY_MS));
}

/* === Button Interrupt Handlers === */
static void button1_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) 
{
  // Check if button is already being processed (debounce)
  if (atomic_cas(&btn_pressed[0], 0, 1)) {
    schedule_debounce(BTN_1);
  }
}

static void button2_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) 
{
  if (atomic_cas(&btn_pressed[1], 0, 1)) {
    schedule_debounce(BTN_2);
  }
}

static void button3_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) 
{
  if (atomic_cas(&btn_pressed[2], 0, 1)) {
    schedule_debounce(BTN_3);
  }
}

static void button4_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) 
{
  if (atomic_cas(&btn_pressed[3], 0, 1)) {
    schedule_debounce(BTN_4);
  }
}

/* === LED Blink Handler === */
static void led_blink_handler(struct k_work *work)
{
  struct led_blink_work *blink_work = CONTAINER_OF(work, struct led_blink_work, dwork.work);
  static led_state_t current_state[4] = {LED_OFF, LED_OFF, LED_OFF, LED_OFF};
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
      uint16_t delay = (current_state[led] == LED_ON) ? 
                        blink_work->on_time_ms : blink_work->off_time_ms;
      k_work_schedule_for_queue(&peripheral_work_q, &blink_work->dwork, K_MSEC(delay));
    } else {
      // Set final state if we're done blinking
      peripheral_set_led(led, blink_work->end_state);
    }
  }
}

/* === LED Control API === */
int peripheral_set_led(enum led_t led, led_state_t state)
{
  int ret = -EINVAL;
  
  switch (led) {
  case LED_1:
    ret = gpio_pin_set_dt(&led0, state);
    break;
  case LED_2:
    ret = gpio_pin_set_dt(&led1, state);
    break;
  case LED_3:
    ret = gpio_pin_set_dt(&led2, state);
    break;
  case LED_4:
    ret = gpio_pin_set_dt(&led3, state);
    break;
  }
  
  if (ret < 0) {
    LOG_ERR("Failed to set LED %d to state %d: %d", led, state, ret);
  }
  
  return ret;
}

int peripheral_set_led_blink(enum led_t led)
{
  // For backward compatibility, use the async version with default parameters
  return peripheral_set_led_blink_async(led, 1, 500, 500, LED_OFF);
}

int peripheral_set_led_blink_async(enum led_t led, uint8_t count, uint16_t on_time_ms, 
                                  uint16_t off_time_ms, led_state_t end_state)
{
  if (led < LED_1 || led > LED_4) {
    return -EINVAL;
  }
  
  uint32_t idx = atomic_inc(&blink_work_idx) % MAX_BLINK_WORKS;
  struct led_blink_work *blink_work = &blink_works[idx];
  
  blink_work->led = led;
  blink_work->blink_count = count;
  blink_work->on_time_ms = on_time_ms;
  blink_work->off_time_ms = off_time_ms;
  blink_work->end_state = end_state;
  
  k_work_init_delayable(&blink_work->dwork, led_blink_handler);
  
  // Start with LED on
  int ret = peripheral_set_led(led, LED_ON);
  if (ret < 0) {
    return ret;
  }
  
  // Schedule the first state change
  return k_work_schedule_for_queue(&peripheral_work_q, &blink_work->dwork, 
                                  K_MSEC(on_time_ms)) ? 0 : -EBUSY;
}

/* === Module Initialization === */
static int setup_led(const struct gpio_dt_spec* led)
{
  if (!device_is_ready(led->port)) {
    LOG_ERR("LED device not ready");
    return -ENODEV;
  }
  int ret = gpio_pin_configure_dt(led, LED_FLAGS);
  if (ret < 0) {
    LOG_ERR("Failed to configure LED: %d", ret);
  }
  return ret;
}

static int setup_button(const struct gpio_dt_spec* btn, struct gpio_callback* cb, gpio_callback_handler_t handler)
{
  if (!device_is_ready(btn->port)) {
    LOG_ERR("Button device not ready");
    return -ENODEV;
  }
  
  int ret = gpio_pin_configure_dt(btn, BTN_FLAGS);
  if (ret < 0) {
    LOG_ERR("Failed to configure button: %d", ret);
    return ret;
  }
  
  ret = gpio_pin_interrupt_configure_dt(btn, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret < 0) {
    LOG_ERR("Failed to configure button interrupt: %d", ret);
    return ret;
  }
  
  gpio_init_callback(cb, handler, BIT(btn->pin));
  ret = gpio_add_callback(btn->port, cb);
  if (ret < 0) {
    LOG_ERR("Failed to add button callback: %d", ret);
    return ret;
  }
  
  return 0;
}

int peripheral_init(btn_ext_handler_t handler)
{
  int ret;
  
  // Initialize work queue
  k_work_queue_start(&peripheral_work_q, peripheral_work_q_stack,
                     K_THREAD_STACK_SIZEOF(peripheral_work_q_stack),
                     WORK_QUEUE_PRIORITY, NULL);
  k_thread_name_set(&peripheral_work_q.thread, "peripheral_work_q");
  
  // Initialize atomic counters
  atomic_set(&work_item_idx, 0);
  atomic_set(&blink_work_idx, 0);
  atomic_set(&debounce_work_idx, 0);
  
  // Initialize button state tracking
  for (int i = 0; i < 4; i++) {
    atomic_set(&btn_pressed[i], 0);
  }
  
  // Setup LEDs
  ret = setup_led(&led0);
  if (ret < 0) {
    LOG_ERR("Failed to setup LED 1");
    return ret;
  }
  peripheral_set_led(LED_1, LED_OFF);
  
  ret = setup_led(&led1);
  if (ret < 0) {
    LOG_ERR("Failed to setup LED 2");
    return ret;
  }
  peripheral_set_led(LED_2, LED_OFF);
  
  ret = setup_led(&led2);
  if (ret < 0) {
    LOG_ERR("Failed to setup LED 3");
    return ret;
  }
  peripheral_set_led(LED_3, LED_OFF);
  
  ret = setup_led(&led3);
  if (ret < 0) {
    LOG_ERR("Failed to setup LED 4");
    return ret;
  }
  peripheral_set_led(LED_4, LED_OFF);

  btn_ext_handler = handler;

  setup_button(&button1, &btn1_cb, button1_pressed);
  setup_button(&button2, &btn2_cb, button2_pressed);
  setup_button(&button3, &btn3_cb, button3_pressed);
  setup_button(&button4, &btn4_cb, button4_pressed);

  LOG_INF("Peripheral module initialized with work queue");
  return 0;
}