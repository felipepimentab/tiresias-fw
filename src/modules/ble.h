/**
 * @file ble.h
 * @brief Public API for the BLE module.
 *
 * This module provides a simple interface for BLE functionality including
 * initialization, advertising control, and connection handling.
 */

#ifndef BLE_H_
#define BLE_H_

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BLE module states
 *
 * These enumeration values represent the possible states of the BLE module.
 */
typedef enum {
  BLE_OFF, /**< Bluetooth is not initialized */
  BLE_NOT_CONNECTED, /**< Bluetooth is initialized but not advertising or connected */
  BLE_ADVERTISING, /**< Bluetooth is advertising and waiting for connections */
  BLE_CONNECTED, /**< Bluetooth is connected to a central device */
  BLE_DISCONNECTED /**< Bluetooth was connected but the connection was lost */
} ble_state_t;

/**
 * @brief Initialize the BLE module
 *
 * This function initializes the Bluetooth subsystem and prepares it for use.
 * It must be called before any other BLE functions.
 *
 * @return 0 on success, negative error code on failure
 */
int ble_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BLE_H_ */
