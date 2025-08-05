/**
 * @file ble.h
 * @brief Public API for the BLE module.
 *
 * This module provides a simple interface for BLE functionality including
 * initialization, advertising control, and connection state management.
 * 
 * The BLE module handles the following responsibilities:
 * - Bluetooth subsystem initialization
 * - Device identity creation and management
 * - Advertising control (start/stop)
 * - Connection state tracking
 * 
 * Usage example:
 * ```c
 * // Initialize the BLE module
 * ble_init();
 * 
 * // Start advertising
 * ble_start_advertising();
 * 
 * // Check the current state
 * ble_state_t state = ble_get_state();
 * if (state == BLE_ADVERTISING) {
 *   // Device is currently advertising
 * }
 * 
 * // Stop advertising when needed
 * ble_stop_advertising();
 * ```
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
 * The state transitions typically occur in the following order:
 * 1. BLE_OFF (initial state before initialization)
 * 2. BLE_NOT_CONNECTED (after initialization)
 * 3. BLE_ADVERTISING (when advertising starts)
 * 4. BLE_CONNECTED (when a connection is established)
 * 5. BLE_DISCONNECTED (when a connection is terminated)
 * 
 * After disconnection, the module may return to BLE_NOT_CONNECTED or
 * automatically restart advertising depending on the implementation.
 */
typedef enum {
  BLE_OFF,          /**< Bluetooth is not initialized */
  BLE_NOT_CONNECTED, /**< Bluetooth is initialized but not advertising or connected */
  BLE_ADVERTISING,   /**< Bluetooth is advertising and waiting for connections */
  BLE_CONNECTED,     /**< Bluetooth is connected to a central device */
  BLE_DISCONNECTED   /**< Bluetooth was connected but the connection was lost */
} ble_state_t;

/**
 * @brief Initialize the BLE module
 *
 * This function initializes the Bluetooth subsystem and prepares it for use.
 * It must be called before any other BLE functions. The initialization process includes:
 * 
 * - Creating a Bluetooth identity with a static random address
 * - Enabling the Bluetooth subsystem
 * - Initializing internal work items for advertising
 * - Setting up connection callbacks
 * 
 * After successful initialization, the module state will be set to BLE_NOT_CONNECTED.
 *
 * @return 0 on success, negative error code on failure
 *         Possible error codes:
 *         - -EIO: Bluetooth controller initialization failed
 *         - -ENOBUFS: Not enough memory for initialization
 */
int ble_init(void);

/**
 * @brief Start BLE advertising
 *
 * This function starts BLE advertising to allow connections from central devices.
 * The advertising is configured with the following parameters:
 * 
 * - Connectable advertising (allows connections from central devices)
 * - Uses identity address (uses the device's public or static random address)
 * - Advertising interval of approximately 500ms
 * - Includes device name and service UUID in the advertising data
 * 
 * The advertising operation is performed asynchronously through a work item.
 * After advertising successfully starts, the module state will be set to BLE_ADVERTISING.
 *
 * @return 0 on success (work item submitted successfully)
 */
int ble_start_advertising(void);

/**
 * @brief Stop BLE advertising
 *
 * This function stops BLE advertising if it is currently active.
 * It should be called when advertising is no longer needed or before
 * changing advertising parameters.
 * 
 * After advertising is successfully stopped, the module state will be
 * set to BLE_NOT_CONNECTED.
 *
 * @return 0 on success, negative error code on failure
 *         Possible error codes:
 *         - -EALREADY: Advertising is not active
 *         - Other negative values: Implementation-specific errors
 */
int ble_stop_advertising(void);

/**
 * @brief Get the current BLE state
 *
 * This function returns the current operational state of the BLE module.
 * It can be used to determine if the module is advertising, connected,
 * or in another state before performing operations.
 * 
 * The state is updated automatically by internal callbacks when events
 * such as connection establishment or disconnection occur.
 *
 * @return The current state of the BLE module (one of the ble_state_t enum values)
 *         - BLE_OFF: Bluetooth is not initialized
 *         - BLE_NOT_CONNECTED: Bluetooth is initialized but not advertising or connected
 *         - BLE_ADVERTISING: Bluetooth is advertising and waiting for connections
 *         - BLE_CONNECTED: Bluetooth is connected to a central device
 *         - BLE_DISCONNECTED: Bluetooth was connected but the connection was lost
 */
ble_state_t ble_get_state(void);

#ifdef __cplusplus
}
#endif

#endif /* BLE_H_ */
