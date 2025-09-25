#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../system_modules/peripheral/peripheral.h"
#include <stdint.h>
#include <zephyr/zbus/zbus.h>

/* External references to ZBUS channel */
extern const struct zbus_channel btn_event_chan;

#endif /* CONTROLLER_H */