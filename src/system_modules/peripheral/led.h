#ifndef _LED_H_
#define _LED_H_

#include "macros_common.h"
#include "zbus_common.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

int init_led();

#endif /* _LED_H_ */
