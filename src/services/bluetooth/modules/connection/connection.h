/**
 * @file connection.h
 * @brief Public API for the BLE connectivity module.
 *
 * This module provides a simple, lightweight interface for Bluetooth Low Energy (BLE) 
 * connectivity functionality, focusing on core operations including initialization, 
 * identity management, and advertising control.
 *
 * The connection module is designed with the following principles:
 * - Simplicity: Clean API with minimal dependencies
 * - Synchronous operation: All functions execute immediately without deferred processing
 * - Separation of concerns: Handles only connectivity, not application-specific logic
 *
 * The connection module handles the following responsibilities:
 * - Bluetooth subsystem initialization and configuration
 * - Device identity creation and management (using static random address)
 * - Advertising control (start/stop) with configurable parameters
 * - Connection event handling through registered callbacks
 *
 * Integration with other modules:
 * - Can be used alongside the BLE service module for GATT services
 * - Works with the peripheral module for hardware-specific configurations
 * - Can integrate with application-level state management
 *
 * Usage example:
 * ```c
 * // Initialize the BLE module
 * int err = ble_init();
 * if (err) {
 *   // Handle initialization error
 *   return err;
 * }
 *
 * // Start advertising to allow connections
 * err = ble_start_advertising();
 * if (err) {
 *   // Handle advertising error
 *   return err;
 * }
 *
 * // When advertising is no longer needed
 * ble_stop_advertising();
 * ```
 *
 * @note This module uses Zephyr's Bluetooth APIs internally and follows
 * Zephyr's error code conventions for return values.
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the BLE module
 *
 * This function initializes the Bluetooth subsystem and prepares it for use.
 * It must be called before any other BLE functions and is typically invoked
 * during system startup.
 * 
 * The initialization process includes:
 *
 * - Creating a Bluetooth identity with a static random address (FF:EE:DD:CC:BB:AA)
 * - Enabling the Bluetooth subsystem with default controller settings
 * - Setting up connection callbacks for connection events
 *
 * @return 0 on success, negative error code on failure
 *         Possible error codes:
 *         - -EIO: Bluetooth controller initialization failed
 *         - -ENOBUFS: Not enough memory for initialization
 *         - -EINVAL: Invalid parameters during identity creation
 *         - -EALREADY: Bluetooth is already initialized
 *
 * @note This function is thread-safe and can be called from any context.
 * @warning This function may block while the controller is being initialized.
 */
int ble_init(void);

/**
 * @brief Start BLE advertising
 *
 * This function starts BLE advertising to allow connections from central devices.
 * If advertising is already active, this function will return an error.
 * 
 * The advertising is configured with the following parameters:
 *
 * - Connectable advertising (allows connections from central devices)
 * - Uses identity address (uses the device's static random address)
 * - Advertising interval of approximately 500ms (800 * 0.625ms)
 * - Includes device name and service UUID in the advertising data
 * - Undirected advertising (broadcasts to any listening device)
 *
 * The advertising operation is performed synchronously and will not return
 * until advertising has started or an error occurs.
 *
 * @return 0 on success, negative error code on failure
 *         Possible error codes:
 *         - -EINVAL: Invalid advertising parameters
 *         - -EALREADY: Advertising is already active
 *         - -ENOMEM: Insufficient memory for advertising data
 *         - -EIO: Controller error during advertising setup
 *
 * @pre ble_init() must be called successfully before this function
 * @see ble_stop_advertising() to stop the advertising process
 */
int ble_start_advertising(void);

/**
 * @brief Stop BLE advertising
 *
 * This function stops BLE advertising if it is currently active.
 * It should be called when advertising is no longer needed, before
 * changing advertising parameters, or to conserve power.
 *
 * If advertising is not active, this function will return -EALREADY.
 * The operation is performed synchronously and will not return until
 * advertising has been stopped or an error occurs.
 *
 * @return 0 on success, negative error code on failure
 *         Possible error codes:
 *         - -EALREADY: Advertising is not active
 *         - -EINVAL: Invalid state or parameters
 *         - -EIO: Controller error during advertising stop
 *
 * @pre ble_init() must be called successfully before this function
 * @see ble_start_advertising() to restart advertising
 */
int ble_stop_advertising(void);

#ifdef __cplusplus
}
#endif

#endif /* CONNECTION_H_ */
