#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../system_modules/peripheral/peripheral.h"
#include <stdint.h>
#include <zephyr/zbus/zbus.h>

/* == State == */

typedef enum controller_state {
  CONTROLLER_STATE_OFF,
  CONTROLLER_STATE_INITIALIZING,
  CONTROLLER_STATE_IDLE,
  CONTROLLER_STATE_LOW_POWER,
  CONTROLLER_STATE_STD_OP,
  CONTROLLER_STATE_ERROR,
  CONTROLLER_STATE_STREAMING
} controller_state;

#endif /* CONTROLLER_H */