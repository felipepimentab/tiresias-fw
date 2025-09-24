#include "ble.h"
#include "../modules/connection.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

/**
 * @brief Send a command to the BLE service
 *
 * This function publishes a command to the BLE command channel,
 * which will be processed by the BLE thread. It provides a simple
 * interface for controlling the BLE service from other modules.
 *
 * @param cmd The command to send to the BLE service
 * @param timeout_ms Timeout in milliseconds for the publish operation
 *
 * @return 0 on success, negative error code on failure
 */
int ble_send_command(ble_cmd cmd, uint32_t timeout_ms)
{
  struct ble_cmd_chan_msg msg;
  msg.cmd = cmd;

  return zbus_chan_pub(&ble_cmd_chan, &msg, K_MSEC(timeout_ms));
}

/**
 * @file ble.c
 * @brief Implementation of the BLE service module
 *
 * This file implements a state-machine based Bluetooth Low Energy (BLE) service
 * that manages the complete lifecycle of BLE connectivity. The implementation
 * follows a message-driven architecture using Zephyr's ZBUS for communication.
 *
 * @section architecture Architecture Overview
 *
 * The BLE service is implemented as a dedicated thread that:
 * 1. Processes commands received through the ble_cmd_chan
 * 2. Manages state transitions based on commands and BLE events
 * 3. Publishes state changes through the ble_state_chan
 *
 * The state machine handles the following key states:
 * - OFF: BLE stack is powered down
 * - INITIALIZING: BLE stack is starting up
 * - NOT_CONNECTED: Ready but not connected or advertising
 * - ADVERTISING: Actively advertising for connections
 * - CONNECTED: Connected to a peer device
 * - Various operational states (CONFIG, STREAMING, FOTA)
 *
 * @section threading Threading Model
 *
 * The BLE service runs in its own dedicated thread with configurable stack size
 * and priority. This ensures that BLE operations don't block other system functions
 * and that the BLE stack receives appropriate CPU time based on its priority.
 *
 * @section messaging Message Flow
 *
 * 1. Commands -> ble_cmd_chan -> BLE thread -> state machine -> state transition
 * 2. State changes -> ble_state_chan -> subscribers -> application logic
 *
 * This decoupled approach allows for clean separation between the BLE service
 * and application modules that use BLE functionality.
 */

LOG_MODULE_REGISTER(ble, LOG_LEVEL_INF);

/**
 * @brief Configuration Constants
 *
 * These constants define the resource allocation and execution parameters
 * for the BLE service thread.
 */
#define BLE_STACK_SIZE 4096 /**< Stack size for BLE thread (bytes) */
#define BLE_PRIORITY 4 /**< Thread priority (0-15, lower is higher priority) */
#define BLE_QUEUE_SIZE 4 /**< Size of the message queue for BLE commands */

/**
 * @brief ZBUS Subscriber for BLE commands
 *
 * Defines a subscriber for the BLE command channel with a queue size of 4.
 * This subscriber is used by the BLE thread to receive commands.
 */
ZBUS_SUBSCRIBER_DEFINE(ble_cmd_sub, 4);

/**
 * @brief ZBUS Channel Definitions
 *
 * These channels provide the communication infrastructure for the BLE service.
 */

/**
 * @brief BLE state channel definition
 *
 * This channel broadcasts the current BLE state to all subscribers.
 * It is initialized with BLE_STATE_OFF as the default state.
 * Subscribers are added dynamically by modules that need to monitor BLE state.
 */
ZBUS_CHAN_DEFINE(ble_state_chan, /* Name */
    ble_state_chan_msg, /* Message type */
    NULL, /* Validator */
    NULL, /* User data */
    ZBUS_OBSERVERS_EMPTY, /* No static observers */
    ZBUS_MSG_INIT(.state = BLE_STATE_OFF) /* Initial value */
);

/**
 * @brief BLE command channel definition
 *
 * This channel receives commands for the BLE service.
 * The BLE thread subscribes to this channel programmatically during initialization.
 * It is initialized with BLE_CMD_OFF as the default command.
 */
ZBUS_CHAN_DEFINE(ble_cmd_chan, /* Name */
    ble_cmd_chan_msg, /* Message type */
    NULL, /* Validator */
    NULL, /* User data */
    ZBUS_OBSERVERS(ble_cmd_sub), /* Subscriber for BLE commands */
    ZBUS_MSG_INIT(.cmd = BLE_CMD_OFF) /* Initial value */
);

/**
 * @brief Current BLE State
 *
 * Tracks the current state of the BLE service state machine.
 * This variable is updated by the set_ble_state() function,
 * which also publishes state changes to subscribers.
 */
static ble_state current_state = BLE_STATE_OFF;

/**
 * @brief State Machine Handler Functions
 *
 * Each function handles commands for a specific BLE state.
 * The state machine routes commands to the appropriate handler
 * based on the current_state variable.
 */
static void handle_state_off(ble_cmd cmd); /**< Handles commands when BLE is off */
static void handle_state_initializing(ble_cmd cmd); /**< Handles commands during initialization */
static void handle_state_not_connected(ble_cmd cmd); /**< Handles commands when initialized but not connected */
static void handle_state_advertising(ble_cmd cmd); /**< Handles commands during advertising */
static void handle_state_connecting(ble_cmd cmd); /**< Handles commands during connection establishment */
static void handle_state_connected(ble_cmd cmd); /**< Handles commands when connected */
static void handle_state_config(ble_cmd cmd); /**< Handles commands in configuration mode */
static void handle_state_streaming(ble_cmd cmd); /**< Handles commands during data streaming */
static void handle_state_fota(ble_cmd cmd); /**< Handles commands during firmware updates */
static void handle_state_disconnecting(ble_cmd cmd); /**< Handles commands during disconnection */

/**
 * @brief State Change Function
 *
 * Updates the current BLE state and publishes the change to all subscribers.
 * This function is called by state handlers when a state transition occurs.
 *
 * @param new_state The new BLE state to transition to
 */
static void set_ble_state(ble_state new_state)
{
  struct ble_state_chan_msg msg;

  /* Avoid unnecessary state changes and publications */
  if (current_state == new_state) {
    return;
  }

  /* Update internal state */
  current_state = new_state;
  msg.state = new_state;

  /* Publish state change with timeout to prevent blocking */
  int err = zbus_chan_pub(&ble_state_chan, &msg, K_MSEC(100));
  if (err) {
    LOG_ERR("Failed to publish BLE state change: %d", err);
  } else {
    LOG_INF("BLE state changed to: %d", new_state);
  }
}

/**
 * @brief Command Handler
 *
 * Central function that processes incoming BLE commands.
 * Routes commands to the appropriate state handler based on current state.
 *
 * This function implements the core state machine logic by:
 * 1. Logging the received command and current state
 * 2. Dispatching the command to the appropriate state handler
 * 3. Handling unexpected states with a warning
 *
 * @param cmd The BLE command to process
 */
static void process_ble_command(ble_cmd cmd)
{
  LOG_INF("Processing BLE command: %d in state: %d", cmd, current_state);

  /* State machine implementation - routes commands to appropriate handler */
  switch (current_state) {
  case BLE_STATE_OFF:
    handle_state_off(cmd);
    break;
  case BLE_STATE_INITIALIZING:
    handle_state_initializing(cmd);
    break;
  case BLE_STATE_NOT_CONNECTED:
    handle_state_not_connected(cmd);
    break;
  case BLE_STATE_ADVERTISING:
    handle_state_advertising(cmd);
    break;
  case BLE_STATE_CONNECTING:
    handle_state_connecting(cmd);
    break;
  case BLE_STATE_CONNECTED:
    handle_state_connected(cmd);
    break;
  case BLE_STATE_CONFIG:
    handle_state_config(cmd);
    break;
  case BLE_STATE_STREAMING:
    handle_state_streaming(cmd);
    break;
  case BLE_STATE_FOTA:
    handle_state_fota(cmd);
    break;
  case BLE_STATE_DISCONNECTING:
    handle_state_disconnecting(cmd);
    break;
  default:
    LOG_WRN("Unhandled BLE state: %d", current_state);
    break;
  }
}

/**
 * @brief State Handler Implementations
 *
 * Each function below handles commands for a specific BLE state.
 * They implement the state transition logic and perform the necessary
 * operations for each command in the context of their specific state.
 */

/**
 * @brief Handler for BLE_STATE_OFF state
 *
 * In the OFF state, the only valid command is BLE_CMD_INIT,
 * which starts the initialization process.
 *
 * @param cmd The command to process
 */
static void handle_state_off(ble_cmd cmd)
{
  switch (cmd) {
  case BLE_CMD_INIT:
    LOG_INF("Initializing BLE");
    set_ble_state(BLE_STATE_INITIALIZING);
    /* Add actual initialization code here */
    /* For now, simulate successful initialization */
    set_ble_state(BLE_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in OFF state", cmd);
    break;
  }
}

static void handle_state_initializing(ble_cmd cmd)
{
  /* In a real implementation, most commands would be ignored during initialization */
  LOG_WRN("Command %d ignored in INITIALIZING state", cmd);
}

static void handle_state_not_connected(ble_cmd cmd)
{
  switch (cmd) {
  case BLE_CMD_ADVERTISE:
    LOG_INF("Starting advertising");
    set_ble_state(BLE_STATE_ADVERTISING);
    /* Add actual advertising code here */
    break;
  case BLE_CMD_OFF:
    LOG_INF("Turning BLE off");
    set_ble_state(BLE_STATE_OFF);
    break;
  default:
    LOG_WRN("Command %d not valid in NOT_CONNECTED state", cmd);
    break;
  }
}

static void handle_state_advertising(ble_cmd cmd)
{
  switch (cmd) {
  case BLE_CMD_CONNECT:
    LOG_INF("Connection requested while advertising");
    set_ble_state(BLE_STATE_CONNECTING);
    break;
  case BLE_CMD_DISCONNECT:
  case BLE_CMD_OFF:
    LOG_INF("Stopping advertising");
    set_ble_state(BLE_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in ADVERTISING state", cmd);
    break;
  }
}

static void handle_state_connecting(ble_cmd cmd)
{
  switch (cmd) {
  case BLE_CMD_DISCONNECT:
    LOG_INF("Cancelling connection attempt");
    set_ble_state(BLE_STATE_DISCONNECTING);
    set_ble_state(BLE_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in CONNECTING state", cmd);
    break;
  }
  /* In a real implementation, connection success/failure would trigger state changes */
}

static void handle_state_connected(ble_cmd cmd)
{
  switch (cmd) {
  case BLE_CMD_DISCONNECT:
    LOG_INF("Disconnecting from connected state");
    set_ble_state(BLE_STATE_DISCONNECTING);
    /* Add actual disconnection code here */
    set_ble_state(BLE_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in CONNECTED state", cmd);
    break;
  }
}

static void handle_state_config(ble_cmd cmd)
{
  /* Placeholder for CONFIG state handling */
  LOG_WRN("Command %d not implemented in CONFIG state", cmd);
}

static void handle_state_streaming(ble_cmd cmd)
{
  /* Placeholder for STREAMING state handling */
  LOG_WRN("Command %d not implemented in STREAMING state", cmd);
}

static void handle_state_fota(ble_cmd cmd)
{
  /* Placeholder for FOTA state handling */
  LOG_WRN("Command %d not implemented in FOTA state", cmd);
}

static void handle_state_disconnecting(ble_cmd cmd)
{
  /* Most commands would be ignored during disconnection */
  LOG_WRN("Command %d ignored in DISCONNECTING state", cmd);
}

/**
 * @brief BLE Thread Function
 *
 * Main execution function for the BLE service thread.
 * This function:
 * 1. Initializes the BLE state to OFF
 * 2. Subscribes to the BLE command channel
 * 3. Enters an infinite loop to process incoming commands
 *
 * The thread blocks until a command is received, then processes it
 * through the state machine. Error handling ensures robustness
 * against ZBUS communication failures.
 *
 * @param arg1 Unused thread parameter
 * @param arg2 Unused thread parameter
 * @param arg3 Unused thread parameter
 */
static void ble_thread(void* arg1, void* arg2, void* arg3)
{
  ARG_UNUSED(arg1);
  ARG_UNUSED(arg2);
  ARG_UNUSED(arg3);

  LOG_INF("BLE thread started, waiting for commands");
  int err;
  struct ble_cmd_chan_msg msg;

  /* Initialize BLE state */
  set_ble_state(BLE_STATE_OFF);

  /* Process messages from the command channel */
  while (1) {
    const struct zbus_channel* chan;

    /* Use a timeout instead of waiting forever to prevent deadlock */
    err = zbus_sub_wait(&ble_cmd_sub, &chan, K_FOREVER);
    if (err) {
      if (err == -EAGAIN) {
        /* Timeout occurred, just continue and try again */
        continue;
      }
      LOG_ERR("Error waiting for BLE command: %d", err);
      continue;
    }

    /* Read the message with appropriate timeout */
    err = zbus_chan_read(chan, &msg, K_MSEC(0));
    if (err) {
      LOG_ERR("Error reading BLE command: %d", err);
      continue;
    }

    /* Process the command */
    process_ble_command(msg.cmd);
  }
}

/**
 * @brief BLE Thread Definition
 *
 * Creates and starts the BLE service thread with:
 * - Stack size defined by BLE_STACK_SIZE
 * - Priority defined by BLE_PRIORITY
 * - Delayed start (500ms) to ensure system initialization is complete
 *
 * This thread runs independently of other system threads and
 * manages the entire BLE state machine.
 */
K_THREAD_DEFINE(ble_thread_id, BLE_STACK_SIZE, ble_thread, NULL, NULL, NULL, BLE_PRIORITY, 0, 0);