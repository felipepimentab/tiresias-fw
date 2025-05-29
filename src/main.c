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

#include "modules/audio_codec.h"
#include "modules/peripheral.h"
#include "modules/storage.h"

LOG_MODULE_REGISTER(Main_app, LOG_LEVEL_INF);

static void button_event_handler(enum button_event_type event)
{
  switch (event) {
  case BUTTON_1_PRESSED:
    // btn_id = BTN_ID_VOL_UP;
    LOG_INF("Button 1 pressed!");
    break;
  case BUTTON_2_PRESSED:
    // btn_id = BTN_ID_VOL_DOWN;
    LOG_INF("Button 2 pressed!");
    break;
  case BUTTON_3_PRESSED:
    // btn_id = BTN_ID_VOL_LOG;
    LOG_INF("Button 3 pressed!");
    break;
  case BUTTON_4_PRESSED:
    // btn_id = BTN_ID_VOL_READ;
    LOG_INF("Button 4 pressed!");
    break;
  default:
    return;
  }
}

int main(void)
{
  LOG_INF("Main application started.");

  int ret = 0;
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

  return 0;
}