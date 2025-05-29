#include "peripheral.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(peripheral, CONFIG_LOG_DEFAULT_LEVEL);

#define BTN1_NODE DT_ALIAS(sw0)
#define BTN2_NODE DT_ALIAS(sw1)
#define BTN3_NODE DT_ALIAS(sw2)
#define BTN4_NODE DT_ALIAS(sw3)

#define BTN_FLAGS (GPIO_INPUT | GPIO_PULL_UP | GPIO_INT_EDGE_TO_ACTIVE)

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

#define LED_FLAGS (GPIO_OUTPUT_ACTIVE | GPIO_ACTIVE_LOW)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, { 0 });
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, { 0 });
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET_OR(LED2_NODE, gpios, { 0 });
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET_OR(LED3_NODE, gpios, { 0 });

static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(BTN1_NODE, gpios, { 0 });
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET_OR(BTN2_NODE, gpios, { 0 });
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET_OR(BTN3_NODE, gpios, { 0 });
static const struct gpio_dt_spec button4 = GPIO_DT_SPEC_GET_OR(BTN4_NODE, gpios, { 0 });

static struct gpio_callback btn1_cb, btn2_cb, btn3_cb, btn4_cb;
static button_callback_t button_callback;

#define PERIPHERAL_THREAD_STACK_SIZE 1024
#define PERIPHERAL_THREAD_PRIORITY 5

K_THREAD_STACK_DEFINE(peripheral_stack, PERIPHERAL_THREAD_STACK_SIZE);
static struct k_thread peripheral_thread_data;
K_MSGQ_DEFINE(peripheral_msgq, sizeof(enum peripheral_event), 10, 4);

static int setup_led(const struct gpio_dt_spec* led)
{
  if (!device_is_ready(led->port)) {
    LOG_ERR("LED device not ready");
    return -ENODEV;
  }
  return gpio_pin_configure_dt(led, LED_FLAGS);
}

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

/* GPIO callbacks */
static void enqueue_event(enum peripheral_event evt) { k_msgq_put(&peripheral_msgq, &evt, K_NO_WAIT); }

static void button1_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  enqueue_event(PERIPH_EVENT_BTN1);
}

static void button2_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  enqueue_event(PERIPH_EVENT_BTN2);
}

static void button3_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  enqueue_event(PERIPH_EVENT_BTN3);
}

static void button4_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  enqueue_event(PERIPH_EVENT_BTN4);
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

static void peripheral_thread(void* p1, void* p2, void* p3)
{
  enum peripheral_event evt;
  while (1) {
    if (k_msgq_get(&peripheral_msgq, &evt, K_FOREVER) == 0) {
      switch (evt) {
      case PERIPH_EVENT_BTN1:
        LOG_INF("Button 1 pressed.");
        peripheral_set_led(LED_1, LED_ON);
        if (button_callback)
          button_callback(BUTTON_1_PRESSED);
        break;
      case PERIPH_EVENT_BTN2:
        LOG_INF("Button 2 pressed.");
        peripheral_set_led(LED_1, LED_OFF);
        if (button_callback)
          button_callback(BUTTON_2_PRESSED);
        break;
      case PERIPH_EVENT_BTN3:
        LOG_INF("Button 3 pressed.");
        peripheral_set_led_blink(LED_1);
        if (button_callback)
          button_callback(BUTTON_3_PRESSED);
        break;
      case PERIPH_EVENT_BTN4:
        LOG_INF("Button 4 pressed.");
        if (button_callback)
          button_callback(BUTTON_4_PRESSED);
        break;
      }
    }
  }
}

int peripheral_init(button_callback_t callback)
{
  setup_led(&led0);
  peripheral_set_led(LED_1, LED_OFF);
  setup_led(&led1);
  peripheral_set_led(LED_2, LED_OFF);
  setup_led(&led2);
  peripheral_set_led(LED_3, LED_OFF);
  setup_led(&led3);
  peripheral_set_led(LED_4, LED_OFF);

  button_callback = callback;
  setup_button(&button1, &btn1_cb, button1_pressed);
  setup_button(&button2, &btn2_cb, button2_pressed);
  setup_button(&button3, &btn3_cb, button3_pressed);
  setup_button(&button4, &btn4_cb, button4_pressed);

  k_thread_create(&peripheral_thread_data, peripheral_stack, K_THREAD_STACK_SIZEOF(peripheral_stack), peripheral_thread,
      NULL, NULL, NULL, PERIPHERAL_THREAD_PRIORITY, 0, K_NO_WAIT);

  LOG_INF("Peripheral module initialized");
  return 0;
}