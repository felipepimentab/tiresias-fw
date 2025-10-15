/**
 * @file connection.h
 * @brief Public API for the BLE connectivity module.
 *
 * This module provides a simplified interface for BLE (Bluetooth Low Energy)
 * operations, focusing on advertising and basic connection handling. It abstracts
 * the complexity of the Zephyr Bluetooth API and provides a clean interface for
 * applications that need basic BLE functionality.
 *
 * The module handles:
 * - BLE initialization and configuration
 * - Advertising management (start/stop)
 * - Connection event delegation to external callbacks
 * - Device naming and identification
 *
 * Key features:
 * - Callback-based initialization with ble_init()
 * - Easy advertising control with start/stop functions
 * - Flexible connection event handling through external callbacks
 * - Configurable device name through Kconfig
 * - Built-in error handling and logging
 *
 * Integration with other modules:
 * - Connection events are delegated to external callbacks provided during initialization
 * - State management is handled by the calling module through the callback functions
 * - The module operates as a bridge between Zephyr BLE stack and application logic
 *
 * Usage example:
 * ```c
 * // Define callback functions
 * void my_connected_cb(struct bt_conn *conn, uint8_t err) {
 *   if (err) {
 *     // Handle connection error
 *     return;
 *   }
 *   // Handle successful connection
 * }
 *
 * void my_disconnected_cb(struct bt_conn *conn, uint8_t reason) {
 *   // Handle disconnection
 * }
 *
 * // Initialize the BLE module with callbacks
 * int err = ble_init(my_connected_cb, my_disconnected_cb);
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

#include <zephyr/bluetooth/conn.h>
#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback function type for BLE connection events
 *
 * This function type defines the signature for callbacks that will be invoked
 * when a BLE connection is established with a central device.
 *
 * @param conn Pointer to the connection object representing the new connection
 * @param err Error code (0 for success, non-zero for failure)
 *            Common error codes:
 *            - 0: Connection successful
 *            - BT_HCI_ERR_CONN_TIMEOUT: Connection timeout
 *            - BT_HCI_ERR_CONN_FAIL: Connection failed to be established
 *
 * @note This callback executes in the context of the Bluetooth host work queue
 * @warning Keep processing in this callback minimal to avoid blocking the BT stack
 */
typedef void (*ble_connected_cb_t)(struct bt_conn* conn, uint8_t err);

/**
 * @brief Callback function type for BLE disconnection events
 *
 * This function type defines the signature for callbacks that will be invoked
 * when a BLE connection is terminated for any reason.
 *
 * @param conn Pointer to the connection object that was disconnected
 * @param reason Reason code for the disconnection
 *               Common reason codes:
 *               - BT_HCI_ERR_REMOTE_USER_TERM_CONN: Remote device terminated connection
 *               - BT_HCI_ERR_LOCAL_HOST_TERM_CONN: Local device terminated connection
 *               - BT_HCI_ERR_CONN_TIMEOUT: Connection supervision timeout
 *               - BT_HCI_ERR_CONN_FAIL_TO_ESTABLISH: Connection failed to establish
 *
 * @note This callback executes in the context of the Bluetooth host work queue
 * @warning Keep processing in this callback minimal to avoid blocking the BT stack
 */
typedef void (*ble_disconnected_cb_t)(struct bt_conn* conn, uint8_t reason);

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
 * - Setting up connection callbacks for connection events using provided callbacks
 *
 * @param connected_cb Callback function to be called when a connection is established.
 *                     Can be NULL if connection events are not needed.
 * @param disconnected_cb Callback function to be called when a connection is terminated.
 *                        Can be NULL if disconnection events are not needed.
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
int ble_init(ble_connected_cb_t connected_cb, ble_disconnected_cb_t disconnected_cb);

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
