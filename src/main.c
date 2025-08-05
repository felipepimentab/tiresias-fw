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

#include "application/controller.h"
#include "modules/ble.h"
#include "modules/peripheral.h"
#include "modules/storage.h"
#include "services/audio_codec.h"
#include "version.h"

LOG_MODULE_REGISTER(Main_app, LOG_LEVEL_INF);

static void button_event_handler(enum button_event_t event)
{
  switch (event) {
  case BUTTON_1_PRESSED:
    LOG_INF("Button 1 pressed! Starting BLE advertising");
    break;
  case BUTTON_2_PRESSED:
    LOG_INF("Button 2 pressed! Stopping BLE advertising");
    break;
  case BUTTON_3_PRESSED:
    LOG_INF("Button 3 pressed! (external)");
    break;
  case BUTTON_4_PRESSED:
    LOG_INF("Button 4 pressed! (external)");
    break;
  default:
    return;
  }
}

int main(void)
{
  LOG_INF("Tiresias Firmware v%s starting.", TIRESIAS_VERSION_STRING);
  LOG_INF("Main application started.");
  int ret = 0;

  ret = controller_init();
  if (ret != 0) {
    LOG_ERR("Failed to initialize controller module");
    return ret;
  }

  ret = audio_codec_init();
  if (ret != 0) {
    LOG_ERR("Failed to initialize audio codec module");
    return ret;
  }

  ret = storage_init();
  if (ret != 0) {
    LOG_ERR("Failed to initialize storage module");
    return ret;
  }

  ret = peripheral_init(button_event_handler);
  if (ret != 0) {
    LOG_ERR("Failed to initialize peripheral module");
    return ret;
  }

  ret = ble_init();
  if (ret != 0) {
    LOG_ERR("Failed to initialize BLE module");
    return ret;
  }

  LOG_INF("Press Button 1 to start BLE advertising");
  LOG_INF("Press Button 2 to stop BLE advertising");

  return 0;
}