/**
 * @file connection.c
 * @brief Simple BLE connectivity interface providing direct API functions.
 *
 * This file implements the BLE connectivity functionality defined in connection.h,
 * providing Bluetooth Low Energy capabilities including device initialization,
 * advertising control, and connection management.
 *
 * Module Architecture:
 * --------------------
 * The connection module follows a lightweight, synchronous design pattern:
 *
 * 1. Direct Operations:
 *    - All functions are synchronous and execute immediately
 *    - No work queues or deferred processing
 *    - Functions return only after operation completes or fails
 *    - Error handling follows Zephyr's negative error code convention
 *
 * 2. Event-Driven Callbacks:
 *    - Registers callbacks for connection events (connected, disconnected)
 *    - Callbacks are executed in the Bluetooth host work queue context
 *    - Events are logged for debugging and monitoring
 *    - No state tracking - callbacks only handle logging
 *
 * 3. Static Configuration:
 *    - Uses static configuration for advertising parameters and data
 *    - Configures a custom service UUID for device identification
 *    - Fixed advertising interval optimized for power/discovery balance
 *    - Device name from Kconfig for easy customization
 *
 * 4. Dependency Management:
 *    - Minimal dependencies on Zephyr Bluetooth APIs
 *    - No dependencies on application-specific modules
 *    - Clean separation between connectivity and business logic
 *    - Self-contained with no external state dependencies
 *
 * Implementation Notes:
 * --------------------
 * - The module uses a static random address (FF:EE:DD:CC:BB:AA) for the Bluetooth identity
 * - Advertising interval is set to approximately 500ms (800 * 0.625ms)
 * - Logging is performed using Zephyr's logging subsystem
 * - Custom service UUID (0x00001523-1212-efde-1523-785feabcd123) for device identification
 * - Thread-safe implementation for all public API functions
 */

#include "connection.h"
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(connection, CONFIG_LOG_DEFAULT_LEVEL);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/**
 * @brief Storage for external callback functions
 *
 * These static variables store the callback functions provided during initialization.
 * They are used by the internal callback functions to delegate event handling
 * to the external modules.
 */
static ble_connected_cb_t external_connected_cb = NULL;
static ble_disconnected_cb_t external_disconnected_cb = NULL;

/**
 * @brief Advertising parameters configuration
 *
 * Configures the BLE advertising with carefully selected parameters optimized for
 * both power efficiency and reliable discovery:
 *
 * - Connectable advertising (allows connections from central devices)
 * - Uses identity address (uses the device's static random address)
 * - Advertising interval of approximately 500ms (800 * 0.625ms)
 *   - This interval balances power consumption and discovery latency
 *   - Shorter intervals improve discovery speed but consume more power
 *   - Longer intervals save power but increase discovery latency
 * - Undirected advertising (broadcasts to any listening device)
 * - No scan request notifications (reduces processing overhead)
 *
 * @note These parameters can be adjusted based on specific application requirements
 * for power consumption, discovery speed, or connection reliability.
 */
static const struct bt_le_adv_param* adv_param = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONN | BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
    800, /* Min Advertising Interval 500ms (800*0.625ms) */
    801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
    NULL); /* Set to NULL for undirected advertising */

/**
 * @brief Primary advertising data
 *
 * Contains the essential advertising data fields to ensure compatibility and
 * proper device discovery:
 *
 * - Flags: General discoverable mode and BR/EDR not supported
 *   - BT_LE_AD_GENERAL: Device is in general discoverable mode
 *   - BT_LE_AD_NO_BREDR: Device does not support BR/EDR (Classic Bluetooth)
 * - Complete device name as configured in Kconfig (CONFIG_BT_DEVICE_NAME)
 *   - Using complete name ensures the full name is visible during scanning
 *   - Name is included in advertising data for immediate identification
 *
 * @note The advertising payload is limited to 31 bytes total, so fields
 * should be kept minimal to ensure all data fits within this limit.
 */
static const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

/**
 * @brief Scan response data
 *
 * Contains additional advertising data sent in response to active scans.
 * This data complements the primary advertising data with:
 *
 * - 128-bit service UUID that identifies the device's primary service
 *   - Custom UUID: 0x00001523-1212-efde-1523-785feabcd123
 *   - Used to identify this specific device/application
 *   - Allows central devices to filter for compatible devices
 *   - Placed in scan response to conserve primary advertising payload space
 *
 * @note Using a custom UUID helps prevent connections from incompatible devices
 * and enables targeted discovery by scanning specifically for this UUID.
 */
static const struct bt_data sd[] = {
  BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)),
};

/**
 * @brief Internal callback for BLE connection events
 *
 * This function is called when a BLE connection is established with a central device.
 * It logs the connection status and delegates to the external callback if provided.
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
static void connected_cb(struct bt_conn* conn, uint8_t err)
{
  if (err) {
    LOG_ERR("Connection failed (err %u)", err);
  } else {
    LOG_INF("Connected");
  }

  /* Call external callback if provided */
  if (external_connected_cb) {
    external_connected_cb(conn, err);
  }
}

/**
 * @brief Internal callback for BLE disconnection events
 *
 * This function is called when a BLE connection is terminated for any reason.
 * It logs the disconnection reason and delegates to the external callback if provided.
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
static void disconnected_cb(struct bt_conn* conn, uint8_t reason)
{
  LOG_INF("Disconnected (reason %u)", reason);

  /* Call external callback if provided */
  if (external_disconnected_cb) {
    external_disconnected_cb(conn, reason);
  }
}

/**
 * @brief Connection callbacks definition
 *
 * Defines the callbacks that will be invoked for various connection events.
 * The BT_CONN_CB_DEFINE macro registers these callbacks with the Bluetooth stack.
 *
 * Registered callbacks:
 * - connected: Called when a connection is established
 * - disconnected: Called when a connection is terminated
 *
 * @note Additional callbacks could be added here for security events,
 * parameter updates, or other connection-related events if needed.
 */
BT_CONN_CB_DEFINE(conn_callbacks) = {
  .connected = connected_cb,
  .disconnected = disconnected_cb,
};

/**
 * @brief Initialize the BLE connection module
 *
 * This function initializes the Bluetooth Low Energy (BLE) subsystem and prepares
 * the device for advertising and accepting connections. It must be called before
 * any other BLE operations.
 *
 * The function performs the following operations:
 * 1. Stores the provided callback functions for connection events
 * 2. Creates a Bluetooth identity with a static random address (FF:EE:DD:CC:BB:AA)
 * 3. Enables the Bluetooth controller and host stack
 * 4. Prepares the device for advertising
 *
 * @param connected_cb Callback function to be called when a connection is established.
 *                     Can be NULL if no external handling is needed.
 * @param disconnected_cb Callback function to be called when a connection is terminated.
 *                        Can be NULL if no external handling is needed.
 *
 * @return 0 on success, negative error code on failure
 *         Common error codes:
 *         - -EALREADY: Bluetooth is already enabled
 *         - -EIO: Hardware initialization failed
 *         - -ENOMEM: Insufficient memory for initialization
 *         - -ENODEV: Bluetooth controller not found
 *
 * @note This function should be called once during system initialization
 * @warning Calling this function multiple times may result in undefined behavior
 *
 * @see ble_start_advertising() to begin advertising after initialization
 * @see ble_stop_advertising() to stop advertising
 */
int ble_init(ble_connected_cb_t connected_cb, ble_disconnected_cb_t disconnected_cb)
{
  int err;

  /* Store external callback functions */
  external_connected_cb = connected_cb;
  external_disconnected_cb = disconnected_cb;

  /* Create a static random address for the device */
  bt_addr_le_t addr;
  err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
  if (err) {
    LOG_ERR("Invalid BT address (err %d)", err);
  }

  /* Create a new Bluetooth identity with the specified address */
  err = bt_id_create(&addr, NULL);
  if (err < 0) {
    LOG_ERR("Creating new ID failed (err %d)", err);
  }

  /* Enable the Bluetooth subsystem */
  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth enable failed (err %d)", err);
    return -1;
  }

  LOG_INF("Bluetooth initialized");

  return 0;
}

/**
 * @brief Start BLE advertising
 *
 * This function starts BLE advertising immediately with the configured parameters.
 * It configures the advertising data and scan response data, then activates the
 * radio to begin broadcasting.
 *
 * The advertising configuration includes:
 * - Device name in the primary advertising data
 * - Service UUID in the scan response data
 * - General discoverable mode flags
 * - 500ms advertising interval
 *
 * Once started, advertising will continue until explicitly stopped or until
 * a connection is established (if connectable advertising is used).
 *
 * @return 0 on success, negative error code on failure
 *
 * @pre ble_init() must be called successfully before this function
 * @see ble_stop_advertising() to stop the advertising process
 * @see bt_le_adv_start() in Zephyr Bluetooth API
 */
int ble_start_advertising(void)
{
  int err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

  if (err) {
    LOG_ERR("Advertising failed to start (err %d)", err);
    return err;
  }

  LOG_INF("Advertising successfully started");
  return 0;
}

/**
 * @brief Stop BLE advertising
 *
 * This function stops any ongoing BLE advertising activity, turning off
 * the radio's advertising transmission. It should be called when advertising
 * is no longer needed, before changing advertising parameters, or to conserve
 * power.
 *
 * If advertising is not currently active, this function will return -EALREADY.
 *
 * @return 0 on success, negative error code on failure
 *         Common error codes:
 *         - -EALREADY: Advertising is not active
 *         - -EINVAL: Invalid state or parameters
 *
 * @pre ble_init() must be called successfully before this function
 * @see ble_start_advertising() to restart advertising
 * @see bt_le_adv_stop() in Zephyr Bluetooth API
 */
int ble_stop_advertising(void)
{
  int err = bt_le_adv_stop();
  if (err) {
    LOG_ERR("Advertising failed to stop (err %d)", err);
    return err;
  }

  LOG_INF("Advertising successfully stopped");
  return 0;
}