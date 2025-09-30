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
#include "services/audio_codec/audio_codec.h"
#include "services/bluetooth/bluetooth.h"
#include "system_modules/peripheral/peripheral.h"
#include "system_modules/storage/storage.h"
#include "version.h"

LOG_MODULE_REGISTER(Main_app, LOG_LEVEL_INF);

int main(void)
{
  LOG_INF("Tiresias Firmware v%s starting.", TIRESIAS_VERSION_STRING);
  int ret = 0;

  ret = peripheral_init();
  if (ret != 0) {
    LOG_ERR("Failed to initialize peripheral module");
    return ret;
  }
  return 0;
}