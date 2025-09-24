#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "connection.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

/**
 * @file ble.h
 * @brief Bluetooth Low Energy (BLE) service module
 *
 * @details This module provides a state-machine based BLE service that manages
 * the Bluetooth connectivity lifecycle. It handles initialization, advertising,
 * connection management, and various operational states through a command-based
 * interface using Zephyr's ZBUS messaging system.
 *
 * @section design Design Philosophy
 * The BLE service follows these design principles:
 * - State-driven architecture: All BLE operations are managed through well-defined states
 * - Message-based control: Commands are sent via ZBUS to trigger state transitions
 * - Event notification: State changes are published to subscribers via ZBUS
 * - Separation of concerns: The BLE service handles connectivity while application
 *   logic remains in separate modules
 *
 * @section integration Integration with Other Modules
 * This service is designed to be controlled by application modules (like controller.c)
 * and provides state updates that can be consumed by any module that needs to react
 * to BLE connectivity changes.
 *
 * @section usage Usage Example
 * @code
 * // Subscribe to BLE state changes
 * ZBUS_LISTENER_DEFINE(ble_state_listener, 4);
 * zbus_chan_add_obs(&ble_state_chan, &ble_state_listener, NULL);
 *
 * // Send command to initialize BLE
 * struct ble_cmd_chan_msg cmd_msg = {
 *   .cmd = BLE_CMD_INIT
 * };
 * zbus_chan_pub(&ble_cmd_chan, &cmd_msg, K_MSEC(100));
 *
 * // Later, receive state updates
 * struct ble_state_chan_msg state_msg;
 * const struct zbus_channel *chan;
 * zbus_sub_wait(&ble_state_listener, &chan, K_MSEC(SYS_FOREVER_MS));
 * zbus_chan_read(chan, &state_msg, K_MSEC(0));
 * if (state_msg.state == BLE_STATE_CONNECTED) {
 *   // Handle connected state
 * }
 * @endcode
 */

#include <stdint.h>
#include <zephyr/zbus/zbus.h>

/**
 * @brief BLE states
 *
 * Defines all possible states of the BLE service state machine.
 */
typedef enum ble_state {
  BLE_STATE_OFF, /**< BLE stack is powered off */
  BLE_STATE_INITIALIZING, /**< BLE stack is initializing */
  BLE_STATE_NOT_CONNECTED, /**< BLE stack is initialized but not connected or advertising */
  BLE_STATE_INIT_ERROR, /**< BLE initialization failed */
  BLE_STATE_ADVERTISING, /**< Device is advertising and waiting for connections */
  BLE_STATE_CONNECTING, /**< Connection is in progress */
  BLE_STATE_CONNECTED, /**< Device is connected to a peer */
  BLE_STATE_CONFIG, /**< Connected and in configuration mode */
  BLE_STATE_STREAMING, /**< Connected and actively streaming data */
  BLE_STATE_FOTA, /**< Firmware Over The Air update in progress */
  BLE_STATE_CONNECTION_ERROR, /**< Error occurred during connection */
  BLE_STATE_DISCONNECTING /**< Disconnection in progress */
} ble_state;

/**
 * @brief Controller commands to BLE service
 *
 * Commands that can be sent to the BLE service to trigger state transitions
 * or actions. These commands are sent via the ble_cmd_chan ZBUS channel.
 */
typedef enum ble_cmd {
  BLE_CMD_INIT, /**< Initialize the BLE stack */
  BLE_CMD_ADVERTISE, /**< Start advertising */
  BLE_CMD_CONNECT, /**< Initiate connection (if applicable) */
  BLE_CMD_DISCONNECT, /**< Disconnect or stop advertising */
  BLE_CMD_LOW_POWER, /**< Enter low power mode */
  BLE_CMD_OFF /**< Turn off BLE stack */
} ble_cmd;

/**
 * @brief BLE state channel message structure
 *
 * Message structure for the ble_state_chan ZBUS channel.
 * This structure is published whenever the BLE state changes.
 */
typedef struct ble_state_chan_msg {
  ble_state state; /**< Current BLE state */
} ble_state_chan_msg;

/**
 * @brief BLE command channel message structure
 *
 * Message structure for the ble_cmd_chan ZBUS channel.
 * This structure is used to send commands to the BLE service.
 */
typedef struct ble_cmd_chan_msg {
  ble_cmd cmd; /**< Command to execute */
} ble_cmd_chan_msg;

/**
 * @brief BLE state channel declaration
 *
 * This channel broadcasts BLE state changes to all subscribers.
 * Modules interested in BLE state changes should subscribe to this channel.
 */
ZBUS_CHAN_DECLARE(ble_state_chan);

/**
 * @brief BLE command channel declaration
 *
 * This channel receives commands for the BLE service.
 * Modules that need to control BLE should publish to this channel.
 */
ZBUS_CHAN_DECLARE(ble_cmd_chan);

#endif /* BLUETOOTH_H */