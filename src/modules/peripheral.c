#include "peripheral.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(peripheral, CONFIG_LOG_DEFAULT_LEVEL);

/* Button GPIOs */
#define BTN1_NODE DT_ALIAS(sw0)
#define BTN2_NODE DT_ALIAS(sw1)
#define BTN3_NODE DT_ALIAS(sw2)
#define BTN4_NODE DT_ALIAS(sw3)

#define BTN_FLAGS (GPIO_INPUT | GPIO_PULL_UP | GPIO_INT_EDGE_TO_ACTIVE)

static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(BTN1_NODE, gpios, { 0 });
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET_OR(BTN2_NODE, gpios, { 0 });
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET_OR(BTN3_NODE, gpios, { 0 });
static const struct gpio_dt_spec button4 = GPIO_DT_SPEC_GET_OR(BTN4_NODE, gpios, { 0 });
static struct gpio_callback btn1_cb, btn2_cb, btn3_cb, btn4_cb;

static button_callback_t button_callback;

/* GPIO callbacks */
static void button1_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  LOG_INF("Button 1 pressed.");
  if (button_callback) {
    button_callback(BUTTON_1_PRESSED);
  }
}

static void button2_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  LOG_INF("Button 2 pressed.");
  if (button_callback) {
    button_callback(BUTTON_2_PRESSED);
  }
}

static void button3_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  LOG_INF("Button 3 pressed.");
  if (button_callback) {
    button_callback(BUTTON_3_PRESSED);
  }
}

static void button4_pressed(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  LOG_INF("Button 4 pressed.");
  if (button_callback) {
    button_callback(BUTTON_4_PRESSED);
  }
}

/* GPIO config helper */
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

int peripheral_init(button_callback_t callback)
{
  int ret;

  button_callback = callback;

  ret = setup_button(&button1, &btn1_cb, button1_pressed);
  if (ret < 0) {
    return ret;
  }

  ret = setup_button(&button2, &btn2_cb, button2_pressed);
  if (ret < 0) {
    return ret;
  }

  ret = setup_button(&button3, &btn3_cb, button3_pressed);
  if (ret < 0) {
    return ret;
  }

  ret = setup_button(&button4, &btn4_cb, button4_pressed);
  if (ret < 0) {
    return ret;
  }

  LOG_INF("Peripheral module initialized");
  return 0;
}