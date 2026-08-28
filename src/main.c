#include "application/controller.h"
#include "print_version.h"
#include "services/audio_codec/audio_codec.h"
#include "services/bluetooth/bluetooth.h"
#include "system_modules/peripheral/button.h"
#include "system_modules/peripheral/led.h"
#include "system_modules/storage/storage.h"
#include "utils/macros_common.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(Main_app, LOG_LEVEL_INF);
ZBUS_CHAN_DECLARE(led_chan);

int main(void)
{
  int ret = 0;
  PRINT_TIRESIAS_BANNER();
  PRINT_FIRMWARE_VERSION();

  ret = init_button();
  ERR_CHK(ret);

  ret = init_led();
  ERR_CHK(ret);

  ret = orientation_init();
  ERR_CHK(ret);

  ret = init_storage();
  ERR_CHK(ret);

  struct led_chan_msg_t led_msg;

  led_msg.led = LED_3;
  led_msg.cmd = TURN_ON;
  
  int err = zbus_chan_pub(&led_chan, &led_msg, K_MSEC(100));
  if (err) {
    LOG_ERR("Failed to publish Bluetooth state change: %d", err);
  }

  ret = controller_init();
  ERR_CHK_MSG(ret, "Failed to initialize controller module");

  return ret;
}