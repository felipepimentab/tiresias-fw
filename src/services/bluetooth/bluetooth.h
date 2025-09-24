#ifndef BLUETOOTH_H
#define BLUETOOTH_H

/**
 * @file bluetooth.h
 * @brief Bluetooth Low Energy (Bluetooth) service module
 *
 * @details This module provides a state-machine based Bluetooth service that manages
 * the Bluetooth connectivity lifecycle. It handles initialization, advertising,
 * connection management, and various operational states through a command-based
 * interface using Zephyr's ZBUS messaging system.
 *
 * @section design Design Philosophy
 * The Bluetooth service follows these design principles:
 * - State-driven architecture: All Bluetooth operations are managed through well-defined states
 * - Message-based control: Commands are sent via ZBUS to trigger state transitions
 * - Event notification: State changes are published to subscribers via ZBUS
 * - Separation of concerns: The Bluetooth service handles connectivity while application
 *   logic remains in separate modules
 *
 * @section integration Integration with Other Modules
 * This service is designed to be controlled by application modules (like controller.c)
 * and provides state updates that can be consumed by any module that needs to react
 * to Bluetooth connectivity changes.
 *
 * @section usage Usage Example
 * @code
 * // Subscribe to Bluetooth state changes
 * ZBUS_LISTENER_DEFINE(bluetooth_state_listener, 4);
 * zbus_chan_add_obs(&bluetooth_state_chan, &bluetooth_state_listener, NULL);
 *
 * // Send command to initialize Bluetooth
 * struct bluetooth_cmd_chan_msg cmd_msg = {
 *   .cmd = BLUETOOTH_CMD_INIT
 * };
 * zbus_chan_pub(&bluetooth_cmd_chan, &cmd_msg, K_MSEC(100));
 *
 * // Later, receive state updates
 * struct bluetooth_state_chan_msg state_msg;
 * const struct zbus_channel *chan;
 * zbus_sub_wait(&bluetooth_state_listener, &chan, K_MSEC(SYS_FOREVER_MS));
 * zbus_chan_read(chan, &state_msg, K_MSEC(0));
 * if (state_msg.state == BLUETOOTH_STATE_CONNECTED) {
 *   // Handle connected state
 * }
 * @endcode
 */

#include <stdint.h>
#include <zephyr/zbus/zbus.h>

/**
 * @brief Bluetooth states
 *
 * Defines all possibluetooth states of the Bluetooth service state machine.
 */
typedef enum bluetooth_state {
  BLUETOOTH_STATE_OFF, /**< Bluetooth stack is powered off */
  BLUETOOTH_STATE_INITIALIZING, /**< Bluetooth stack is initializing */
  BLUETOOTH_STATE_NOT_CONNECTED, /**< Bluetooth stack is initialized but not connected or advertising */
  BLUETOOTH_STATE_INIT_ERROR, /**< Bluetooth initialization failed */
  BLUETOOTH_STATE_ADVERTISING, /**< Device is advertising and waiting for connections */
  BLUETOOTH_STATE_CONNECTING, /**< Connection is in progress */
  BLUETOOTH_STATE_CONNECTED, /**< Device is connected to a peer */
  BLUETOOTH_STATE_CONFIG, /**< Connected and in configuration mode */
  BLUETOOTH_STATE_STREAMING, /**< Connected and actively streaming data */
  BLUETOOTH_STATE_FOTA, /**< Firmware Over The Air update in progress */
  BLUETOOTH_STATE_CONNECTION_ERROR, /**< Error occurred during connection */
  BLUETOOTH_STATE_DISCONNECTING /**< Disconnection in progress */
} bluetooth_state;

/**
 * @brief Controller commands to Bluetooth service
 *
 * Commands that can be sent to the Bluetooth service to trigger state transitions
 * or actions. These commands are sent via the bluetooth_cmd_chan ZBUS channel.
 */
typedef enum bluetooth_cmd {
  BLUETOOTH_CMD_INIT, /**< Initialize the Bluetooth stack */
  BLUETOOTH_CMD_ADVERTISE, /**< Start advertising */
  BLUETOOTH_CMD_CONNECT, /**< Initiate connection (if applicabluetooth) */
  BLUETOOTH_CMD_DISCONNECT, /**< Disconnect or stop advertising */
  BLUETOOTH_CMD_LOW_POWER, /**< Enter low power mode */
  BLUETOOTH_CMD_OFF /**< Turn off Bluetooth stack */
} bluetooth_cmd;

/**
 * @brief Bluetooth state channel message structure
 *
 * Message structure for the bluetooth_state_chan ZBUS channel.
 * This structure is published whenever the Bluetooth state changes.
 */
typedef struct bluetooth_state_chan_msg {
  bluetooth_state state; /**< Current Bluetooth state */
} bluetooth_state_chan_msg;

/**
 * @brief Bluetooth command channel message structure
 *
 * Message structure for the bluetooth_cmd_chan ZBUS channel.
 * This structure is used to send commands to the Bluetooth service.
 */
typedef struct bluetooth_cmd_chan_msg {
  bluetooth_cmd cmd; /**< Command to execute */
} bluetooth_cmd_chan_msg;

/**
 * @brief Bluetooth state channel declaration
 *
 * This channel broadcasts Bluetooth state changes to all subscribers.
 * Modules interested in Bluetooth state changes should subscribe to this channel.
 */
ZBUS_CHAN_DECLARE(bluetooth_state_chan);

/**
 * @brief Bluetooth command channel declaration
 *
 * This channel receives commands for the Bluetooth service.
 * Modules that need to control Bluetooth should publish to this channel.
 */
ZBUS_CHAN_DECLARE(bluetooth_cmd_chan);

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
int bluetooth_send_command(bluetooth_cmd cmd, uint32_t timeout_ms);

#endif /* BLUETOOTH_H */