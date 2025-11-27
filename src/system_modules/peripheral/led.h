#ifndef _LED_H_
#define _LED_H_

/**
 * @file
 * @brief LED module public API.
 *
 * Exposes a single initialization function for the LED peripheral. The
 * implementation subscribes to a Zbus channel to react to LED commands
 * (on/off/toggle/blink) and manages GPIO configuration.
 */

#include "macros_common.h"
#include "zbus_common.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

/**
 * @brief Initialize LED GPIOs and prepare the module.
 *
 * Configures all LED pins as outputs and sets them inactive. Must be called
 * before any LED commands are processed.
 *
 * @return 0 on success, negative errno-style value on failure.
 */
int init_led();

#endif /* _LED_H_ */
