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

int peripheral_set_led(enum led_t led, led_state_t state);
int peripheral_set_led_blink(enum led_t led);

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
    peripheral_set_led_blink(LED_1);
    break;
  case LED_2_ON:
    peripheral_set_led(LED_2, LED_ON);
    break;
  case LED_2_OFF:
    peripheral_set_led(LED_2, LED_OFF);
    break;
  case LED_2_BLINK:
    peripheral_set_led_blink(LED_2);
    break;
  case LED_3_ON:
    peripheral_set_led(LED_3, LED_ON);
    break;
  case LED_3_OFF:
    peripheral_set_led(LED_3, LED_OFF);
    break;
  case LED_3_BLINK:
    peripheral_set_led_blink(LED_3);
    break;
  case LED_4_ON:
    peripheral_set_led(LED_4, LED_ON);
    break;
  case LED_4_OFF:
    peripheral_set_led(LED_4, LED_OFF);
    break;
  case LED_4_BLINK:
    peripheral_set_led_blink(LED_4);
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
    break;
  }
}

static void enqueue_event(enum peripheral_event evt)
{
  static struct peripheral_work_item items[4];
  static int idx = 0;
  struct peripheral_work_item* item = &items[idx];
  idx = (idx + 1) % 4;
  item->event = evt;
  k_work_init(&item->work, handle_event_work);
  k_work_submit(&item->work);
}

/* === Button Interrupt Handlers === */
static void button1_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) { enqueue_event(BTN_1); }
static void button2_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) { enqueue_event(BTN_2); }
static void button3_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) { enqueue_event(BTN_3); }
static void button4_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins) { enqueue_event(BTN_4); }

/* === LED Control API === */
int peripheral_set_led(enum led_t led, led_state_t state)
{
  switch (led) {
  case LED_1:
    return gpio_pin_set_dt(&led0, state);
  case LED_2:
    return gpio_pin_set_dt(&led1, state);
  case LED_3:
    return gpio_pin_set_dt(&led2, state);
  case LED_4:
    return gpio_pin_set_dt(&led3, state);
  default:
    return -EINVAL;
  }
}

int peripheral_set_led_blink(enum led_t led)
{
  int ret = peripheral_set_led(led, LED_ON);
  if (ret < 0)
    return ret;
  k_msleep(500);
  return peripheral_set_led(led, LED_OFF);
}

/* === Module Initialization === */
static int setup_led(const struct gpio_dt_spec* led)
{
  if (!device_is_ready(led->port)) {
    LOG_ERR("LED device not ready");
    return -ENODEV;
  }
  return gpio_pin_configure_dt(led, LED_FLAGS);
}

static int setup_button(const struct gpio_dt_spec* btn, struct gpio_callback* cb, gpio_callback_handler_t handler)
{
  if (!device_is_ready(btn->port)) {
    LOG_ERR("Button device not ready");
    return -ENODEV;
  }
  gpio_pin_configure_dt(btn, BTN_FLAGS);
  gpio_pin_interrupt_configure_dt(btn, GPIO_INT_EDGE_TO_ACTIVE);
  gpio_init_callback(cb, handler, BIT(btn->pin));
  gpio_add_callback(btn->port, cb);
  return 0;
}

int peripheral_init(btn_ext_handler_t handler)
{
  setup_led(&led0);
  peripheral_set_led(LED_1, LED_OFF);
  setup_led(&led1);
  peripheral_set_led(LED_2, LED_OFF);
  setup_led(&led2);
  peripheral_set_led(LED_3, LED_OFF);
  setup_led(&led3);
  peripheral_set_led(LED_4, LED_OFF);

  btn_ext_handler = handler;

  setup_button(&button1, &btn1_cb, button1_pressed);
  setup_button(&button2, &btn2_cb, button2_pressed);
  setup_button(&button3, &btn3_cb, button3_pressed);
  setup_button(&button4, &btn4_cb, button4_pressed);

  LOG_INF("Peripheral module initialized with work queue");
  return 0;
}