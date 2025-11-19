#include "button.h"
#include "zbus_common.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(button, LOG_LEVEL_INF);

#define BTN1_NODE DT_ALIAS(sw0)
#define BTN_FLAGS (GPIO_INPUT | GPIO_PULL_UP | GPIO_INT_EDGE_TO_ACTIVE)

static const struct gpio_dt_spec btn1 = GPIO_DT_SPEC_GET_OR(BTN1_NODE, gpios, { 0 });
static struct gpio_callback btn1_cb;

static void btn1_cb_handler(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  //
}

ZBUS_CHAN_DEFINE(button_chan, btn_event_msg_t, NULL, NULL, ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(0));

int init_button()
{
  int ret = 0;

  if (!device_is_ready(btn1.port)) {
    return -1;
  }

  ret = gpio_pin_configure_dt(&btn1, BTN_FLAGS);
  ret = gpio_pin_interrupt_configure_dt(&btn1, GPIO_INT_EDGE_TO_ACTIVE);

  gpio_init_callback(&btn1_cb, btn1_cb_handler, BIT(btn1.pin));
  ret = gpio_add_callback(btn1.port, &btn1_cb);

  return ret;
}