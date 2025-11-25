/**
 * @file controller.h
 * @brief Main application controller interface
 *
 * This module provides the main application controller that manages the overall
 * system state and coordinates between different services (Bluetooth, audio codec)
 * and system modules (peripherals). The controller implements a state machine
 * that handles system initialization, operation modes, and error conditions.
 *
 * @author Tiresias Firmware Team
 * @version 1.0
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>
#include <zephyr/zbus/zbus.h>

typedef enum controller_event {
  /** @brief Request to transition to a new state */
  CONTROLLER_EVENT_INIT = 0,
} controller_event;

typedef struct controller_event_chan_msg {
  controller_event event;
} controller_event_chan_msg;

/* === Controller State Definitions === */

/**
 * @brief Controller state enumeration
 *
 * Defines all possible states of the main application controller.
 * The controller follows a specific state transition flow:
 * OFF -> INITIALIZING -> IDLE -> (STD_OP | STREAMING | LOW_POWER)
 * Any state can transition to ERROR state on failure.
 */
typedef enum controller_state {
  /** @brief System is powered off or not initialized */
  CONTROLLER_STATE_OFF,

  /** @brief System is initializing services and modules */
  CONTROLLER_STATE_INITIALIZING,

  /** @brief System is idle and ready for operation */
  CONTROLLER_STATE_IDLE,

  /** @brief System is in low power mode */
  CONTROLLER_STATE_LOW_POWER,

  /** @brief System is in standard operation mode */
  CONTROLLER_STATE_STD_OP,

  /** @brief System encountered an error */
  CONTROLLER_STATE_ERROR,

  /** @brief System is actively streaming audio */
  CONTROLLER_STATE_STREAMING
} controller_state;

/* === Public Function Declarations === */

/**
 * @brief Get the current controller state
 *
 * @return controller_state Current state of the controller
 */
controller_state controller_get_state(void);

/**
 * @brief Check if the controller is in an operational state
 *
 * @return true if controller is ready for operation, false otherwise
 */
bool controller_is_operational(void);

int controller_init(void);

#endif /* CONTROLLER_H */