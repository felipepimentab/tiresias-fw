#include "bluetooth.h"
#include "modules/connection/connection.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

/**
 * @file bluetooth.c
 * @brief Implementation of the Bluetooth service module
 *
 * This file implements a state-machine based Bluetooth Low Energy (Bluetooth) service
 * that manages the complete lifecycle of Bluetooth connectivity. The implementation
 * follows a message-driven architecture using Zephyr's ZBUS for communication.
 *
 * @section architecture Architecture Overview
 *
 * The Bluetooth service is implemented as a dedicated thread that:
 * 1. Processes commands received through the bluetooth_cmd_chan
 * 2. Manages state transitions based on commands and Bluetooth events
 * 3. Publishes state changes through the bluetooth_state_chan
 *
 * The state machine handles the following key states:
 * - OFF: Bluetooth stack is powered down
 * - INITIALIZING: Bluetooth stack is starting up
 * - NOT_CONNECTED: Ready but not connected or advertising
 * - ADVERTISING: Actively advertising for connections
 * - CONNECTED: Connected to a peer device
 * - Various operational states (CONFIG, STREAMING, FOTA)
 *
 * @section threading Threading Model
 *
 * The Bluetooth service runs in its own dedicated thread with configurabluetooth stack size
 * and priority. This ensures that Bluetooth operations don't block other system functions
 * and that the Bluetooth stack receives appropriate CPU time based on its priority.
 *
 * @section messaging Message Flow
 *
 * 1. Commands -> bluetooth_cmd_chan -> Bluetooth thread -> state machine -> state transition
 * 2. State changes -> bluetooth_state_chan -> subscribers -> application logic
 *
 * This decoupled approach allows for clean separation between the Bluetooth service
 * and application modules that use Bluetooth functionality.
 */

LOG_MODULE_REGISTER(bluetooth, LOG_LEVEL_INF);

/**
 * @brief Configuration Constants
 *
 * These constants define the resource allocation and execution parameters
 * for the Bluetooth service thread.
 */
#define BLUETOOTH_STACK_SIZE 4096 /**< Stack size for Bluetooth thread (bytes) */
#define BLUETOOTH_PRIORITY 4 /**< Thread priority (0-15, lower is higher priority) */
#define BLUETOOTH_QUEUE_SIZE 4 /**< Size of the message queue for Bluetooth commands */

/**
 * @brief ZBUS Subscriber for Bluetooth commands
 *
 * Defines a subscriber for the Bluetooth command channel with a queue size of 4.
 * This subscriber is used by the Bluetooth thread to receive commands.
 */
ZBUS_SUBSCRIBER_DEFINE(bluetooth_cmd_sub, 4);

/**
 * @brief ZBUS Channel Definitions
 *
 * These channels provide the communication infrastructure for the Bluetooth service.
 */

/**
 * @brief Bluetooth state channel definition
 *
 * This channel broadcasts the current Bluetooth state to all subscribers.
 * It is initialized with BLUETOOTH_STATE_OFF as the default state.
 * Subscribers are added dynamically by modules that need to monitor Bluetooth state.
 */
ZBUS_CHAN_DEFINE(bluetooth_state_chan, /* Name */
    bluetooth_state_chan_msg, /* Message type */
    NULL, /* Validator */
    NULL, /* User data */
    ZBUS_OBSERVERS_EMPTY, /* No static observers */
    ZBUS_MSG_INIT(.state = BLUETOOTH_STATE_OFF) /* Initial value */
);

/**
 * @brief Bluetooth command channel definition
 *
 * This channel receives commands for the Bluetooth service.
 * The Bluetooth thread subscribes to this channel programmatically during initialization.
 * It is initialized with BLUETOOTH_CMD_INIT as the default command.
 */
ZBUS_CHAN_DEFINE(bluetooth_cmd_chan, /* Name */
    bluetooth_cmd_chan_msg, /* Message type */
    NULL, /* Validator */
    NULL, /* User data */
    ZBUS_OBSERVERS(bluetooth_cmd_sub), /* Subscriber for Bluetooth commands */
    ZBUS_MSG_INIT(.cmd = BLUETOOTH_CMD_INIT) /* Initial value */
);

/**
 * @brief Send a command to the Bluetooth service
 *
 * This function publishes a command to the Bluetooth command channel,
 * which will be processed by the Bluetooth thread. It provides a simple
 * interface for controlling the Bluetooth service from other modules.
 *
 * @param cmd The command to send to the Bluetooth service
 * @param timeout_ms Timeout in milliseconds for the publish operation
 *
 * @return 0 on success, negative error code on failure
 */
int bluetooth_send_command(bluetooth_cmd cmd)
{
  struct bluetooth_cmd_chan_msg msg;
  msg.cmd = cmd;

  return zbus_chan_pub(&bluetooth_cmd_chan, &msg, K_MSEC(100));
}

/**
 * @brief Current Bluetooth State
 *
 * Tracks the current state of the Bluetooth service state machine.
 * This variabluetooth is updated by the set_bluetooth_state() function,
 * which also publishes state changes to subscribers.
 */
static bluetooth_state current_state = BLUETOOTH_STATE_OFF;

/**
 * @brief State Machine Handler Functions
 *
 * Each function handles commands for a specific Bluetooth state.
 * The state machine routes commands to the appropriate handler
 * based on the current_state variabluetooth.
 */
static void handle_state_off(bluetooth_cmd cmd); /**< Handles commands when Bluetooth is off */
static void handle_state_initializing(bluetooth_cmd cmd); /**< Handles commands during initialization */
static void handle_state_not_connected(bluetooth_cmd cmd); /**< Handles commands when initialized but not connected */
static void handle_state_advertising(bluetooth_cmd cmd); /**< Handles commands during advertising */
static void handle_state_connecting(bluetooth_cmd cmd); /**< Handles commands during connection establishment */
static void handle_state_connected(bluetooth_cmd cmd); /**< Handles commands when connected */
static void handle_state_config(bluetooth_cmd cmd); /**< Handles commands in configuration mode */
static void handle_state_streaming(bluetooth_cmd cmd); /**< Handles commands during data streaming */
static void handle_state_fota(bluetooth_cmd cmd); /**< Handles commands during firmware updates */
static void handle_state_disconnecting(bluetooth_cmd cmd); /**< Handles commands during disconnection */

/**
 * @brief State Change Function
 *
 * Updates the current Bluetooth state and publishes the change to all subscribers.
 * This function is called by state handlers when a state transition occurs.
 *
 * @param new_state The new Bluetooth state to transition to
 */
static void set_bluetooth_state(bluetooth_state new_state)
{
  struct bluetooth_state_chan_msg msg;

  /* Avoid unnecessary state changes and publications */
  if (current_state == new_state) {
    return;
  }

  /* Update internal state */
  current_state = new_state;
  msg.state = new_state;

  /* Publish state change with timeout to prevent blocking */
  int err = zbus_chan_pub(&bluetooth_state_chan, &msg, K_MSEC(100));
  if (err) {
    LOG_ERR("Failed to publish Bluetooth state change: %d", err);
  }
}

/**
 * @brief Command Handler
 *
 * Central function that processes incoming Bluetooth commands.
 * Routes commands to the appropriate state handler based on current state.
 *
 * This function implements the core state machine logic by:
 * 1. Logging the received command and current state
 * 2. Dispatching the command to the appropriate state handler
 * 3. Handling unexpected states with a warning
 *
 * @param cmd The Bluetooth command to process
 */
static void bluetooth_state_machine(bluetooth_cmd cmd)
{
  /* State machine implementation - routes commands to appropriate handler */
  switch (current_state) {
  case BLUETOOTH_STATE_OFF:
    handle_state_off(cmd);
    break;
  case BLUETOOTH_STATE_INITIALIZING:
    handle_state_initializing(cmd);
    break;
  case BLUETOOTH_STATE_NOT_CONNECTED:
    handle_state_not_connected(cmd);
    break;
  case BLUETOOTH_STATE_ADVERTISING:
    handle_state_advertising(cmd);
    break;
  case BLUETOOTH_STATE_CONNECTING:
    handle_state_connecting(cmd);
    break;
  case BLUETOOTH_STATE_CONNECTED:
    handle_state_connected(cmd);
    break;
  case BLUETOOTH_STATE_CONFIG:
    handle_state_config(cmd);
    break;
  case BLUETOOTH_STATE_STREAMING:
    handle_state_streaming(cmd);
    break;
  case BLUETOOTH_STATE_FOTA:
    handle_state_fota(cmd);
    break;
  case BLUETOOTH_STATE_DISCONNECTING:
    handle_state_disconnecting(cmd);
    break;
  default:
    LOG_WRN("Unhandled Bluetooth state: %d", current_state);
    break;
  }
}

/**
 * @brief State Handler Implementations
 *
 * Each function below handles commands for a specific Bluetooth state.
 * They implement the state transition logic and perform the necessary
 * operations for each command in the context of their specific state.
 */

/**
 * @brief Handler for BLUETOOTH_STATE_OFF state
 *
 * In the OFF state, the only valid command is BLUETOOTH_CMD_INIT,
 * which starts the initialization process.
 *
 * @param cmd The command to process
 */
static void handle_state_off(bluetooth_cmd cmd)
{
  if (cmd != BLUETOOTH_CMD_INIT) {
    LOG_WRN("Command %d not valid in OFF state", cmd);
    return;
  }

  LOG_INF("Initializing Bluetooth");
  set_bluetooth_state(BLUETOOTH_STATE_INITIALIZING);
  int ret = ble_init();
  if (ret != 0) {
    LOG_ERR("Failed to initialize Bluetooth: %d", ret);
    set_bluetooth_state(BLUETOOTH_STATE_INIT_ERROR);
    return;
  }
  set_bluetooth_state(BLUETOOTH_STATE_NOT_CONNECTED);
}

static void handle_state_initializing(bluetooth_cmd cmd) { LOG_WRN("Command %d ignored in INITIALIZING state", cmd); }

static void handle_state_not_connected(bluetooth_cmd cmd)
{
  if (cmd != BLUETOOTH_CMD_ADVERTISE) {
    LOG_WRN("Command %d not valid in NOT_CONNECTED state", cmd);
    return;
  }

  LOG_INF("Starting advertising");
  int ret = ble_start_advertising();
  if (ret != 0) {
    LOG_ERR("Failed to start advertising: %d", ret);
    return;
  }
  set_bluetooth_state(BLUETOOTH_STATE_ADVERTISING);
}

static void handle_state_advertising(bluetooth_cmd cmd)
{
  switch (cmd) {
  case BLUETOOTH_CMD_CONNECT:
    LOG_INF("Connection requested while advertising");
    set_bluetooth_state(BLUETOOTH_STATE_CONNECTING);
    break;
  case BLUETOOTH_CMD_DISCONNECT:
  case BLUETOOTH_CMD_OFF:
    LOG_INF("Stopping advertising");
    set_bluetooth_state(BLUETOOTH_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in ADVERTISING state", cmd);
    break;
  }
}

static void handle_state_connecting(bluetooth_cmd cmd)
{
  switch (cmd) {
  case BLUETOOTH_CMD_DISCONNECT:
    LOG_INF("Cancelling connection attempt");
    set_bluetooth_state(BLUETOOTH_STATE_DISCONNECTING);
    set_bluetooth_state(BLUETOOTH_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in CONNECTING state", cmd);
    break;
  }
  /* In a real implementation, connection success/failure would trigger state changes */
}

static void handle_state_connected(bluetooth_cmd cmd)
{
  switch (cmd) {
  case BLUETOOTH_CMD_DISCONNECT:
    LOG_INF("Disconnecting from connected state");
    set_bluetooth_state(BLUETOOTH_STATE_DISCONNECTING);
    /* Add actual disconnection code here */
    set_bluetooth_state(BLUETOOTH_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in CONNECTED state", cmd);
    break;
  }
}

static void handle_state_config(bluetooth_cmd cmd)
{
  /* Placeholder for CONFIG state handling */
  LOG_WRN("Command %d not implemented in CONFIG state", cmd);
}

static void handle_state_streaming(bluetooth_cmd cmd)
{
  /* Placeholder for STREAMING state handling */
  LOG_WRN("Command %d not implemented in STREAMING state", cmd);
}

static void handle_state_fota(bluetooth_cmd cmd)
{
  /* Placeholder for FOTA state handling */
  LOG_WRN("Command %d not implemented in FOTA state", cmd);
}

static void handle_state_disconnecting(bluetooth_cmd cmd)
{
  /* Most commands would be ignored during disconnection */
  LOG_WRN("Command %d ignored in DISCONNECTING state", cmd);
}

/**
 * @brief Bluetooth Thread Function
 *
 * Main execution function for the Bluetooth service thread.
 * This function:
 * 1. Initializes the Bluetooth state to OFF
 * 2. Subscribes to the Bluetooth command channel
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
static void bluetooth_thread(void* arg1, void* arg2, void* arg3)
{
  ARG_UNUSED(arg1);
  ARG_UNUSED(arg2);
  ARG_UNUSED(arg3);

  LOG_INF("Bluetooth thread started.");
  int ret = 0;
  struct bluetooth_cmd_chan_msg msg;

  while (1) {
    const struct zbus_channel* chan;

    ret = zbus_sub_wait(&bluetooth_cmd_sub, &chan, K_FOREVER);
    if (ret != 0) {
      LOG_ERR("Error waiting for Bluetooth command: %d", ret);
      continue;
    }

    ret = zbus_chan_read(chan, &msg, K_MSEC(500));
    if (ret != 0) {
      LOG_ERR("Error reading Bluetooth command: %d", ret);
      continue;
    }

    bluetooth_state_machine(msg.cmd);
  }
}

/**
 * @brief Bluetooth Thread Definition
 *
 * Creates and starts the Bluetooth service thread with:
 * - Stack size defined by BLUETOOTH_STACK_SIZE
 * - Priority defined by BLUETOOTH_PRIORITY
 * - Delayed start (500ms) to ensure system initialization is complete
 *
 * This thread runs independently of other system threads and
 * manages the entire Bluetooth state machine.
 */
K_THREAD_DEFINE(
    bluetooth_thread_id, BLUETOOTH_STACK_SIZE, bluetooth_thread, NULL, NULL, NULL, BLUETOOTH_PRIORITY, 0, 0);