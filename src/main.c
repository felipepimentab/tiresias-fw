#include "adau_1787_IC_1_FAST_REG.h"
#include "drivers/adau1787.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(Main_app, LOG_LEVEL_INF);
#define SLEEP_TIME_MS 10 * 60 * 100

#define ADAU_THREAD_STACKSIZE 1024
#define ADAU_THREAD_PRIORITY 2

// Button configuration
#define VOL_DOWN DT_ALIAS(sw0)
#define VOL_UP DT_ALIAS(sw1)
static const struct gpio_dt_spec vol_down = GPIO_DT_SPEC_GET(VOL_DOWN, gpios);
static const struct gpio_dt_spec vol_up = GPIO_DT_SPEC_GET(VOL_UP, gpios);
static struct gpio_callback vol_down_cb_data;
static struct gpio_callback vol_up_cb_data;
// Callback configuration
void volume_down(const struct device* dev, struct gpio_callback* cb, uint32_t pins) { LOG_INF("Volume down!"); }
void volume_up(const struct device* dev, struct gpio_callback* cb, uint32_t pins) { LOG_INF("Volume up!"); }

int main(void)
{
  int ret;

  ret = adau1787_init();
  if (ret < 0) {
    LOG_ERR("Failed to initiate ADAU1787.");
    return ret;
  }

  // Configuring buttons and interruptions
  if (!device_is_ready(vol_down.port)) {
    LOG_ERR("Button 0 (volume down) is not ready.");
    return -1;
  }
  if (!device_is_ready(vol_up.port)) {
    LOG_ERR("Button 1 (volume up) is not ready.");
    return -1;
  }
  ret = gpio_pin_configure_dt(&vol_down, GPIO_INPUT);
  if (ret < 0) {
    LOG_ERR("Failed to configure Button 0 (volume down).");
    return -1;
  }
  ret = gpio_pin_configure_dt(&vol_up, GPIO_INPUT);
  if (ret < 0) {
    LOG_ERR("Failed to configure Button 1 (volume up).");
    return -1;
  }
  ret = gpio_pin_interrupt_configure_dt(&vol_down, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret < 0) {
    LOG_ERR("Failed to configure interruption Button 0 (volume down).");
    return -1;
  }
  gpio_init_callback(&vol_down_cb_data, volume_down, BIT(vol_down.pin));
  gpio_add_callback(vol_down.port, &vol_down_cb_data);
  ret = gpio_pin_interrupt_configure_dt(&vol_up, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret < 0) {
    LOG_ERR("Failed to configure interruption Button 1 (volume up).");
    return -1;
  }
  gpio_init_callback(&vol_up_cb_data, volume_up, BIT(vol_up.pin));
  gpio_add_callback(vol_up.port, &vol_up_cb_data);

  while (1) {
    k_msleep(SLEEP_TIME_MS);
  }
}
