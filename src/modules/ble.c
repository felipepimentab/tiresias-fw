/**
 * @file ble.c
 * @brief Modular BLE interface for initialization, control, and future expansion.
 *
 * This file implements the BLE module functionality defined in ble.h, providing
 * Bluetooth Low Energy capabilities including device initialization, advertising
 * control, and connection state management.
 *
 * Module Architecture:
 * --------------------
 * The BLE module follows a state-based design pattern with the following components:
 *
 * 1. State Management:
 *    - Uses an enumeration (ble_state_t) to track the current operational state
 *    - State transitions occur based on initialization, advertising, and connection events
 *
 * 2. Asynchronous Operations:
 *    - Uses Zephyr's work queue system for non-blocking operations
 *    - Advertising is started asynchronously through a work item
 *
 * 3. Event Callbacks:
 *    - Registers callbacks for connection events (connected, disconnected, recycled)
 *    - Updates the module state based on these events
 *
 * 4. Static Configuration:
 *    - Uses static configuration for advertising parameters and data
 *    - Configures a custom service UUID for device identification
 *
 * Implementation Notes:
 * --------------------
 * - The module uses a static random address for the Bluetooth identity
 * - Advertising interval is set to approximately 500ms (800 * 0.625ms)
 * - Logging is performed using Zephyr's logging subsystem
 * - The module automatically restarts advertising when a connection is recycled
 */

#include "ble.h"
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ble_module, CONFIG_LOG_DEFAULT_LEVEL);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/**
 * @brief Current state of the BLE module
 *
 * This variable tracks the current operational state of the BLE module.
 * It is updated by various functions and callbacks throughout the module's lifecycle.
 */
static ble_state_t current_ble_state = BLE_OFF;

/**
 * @brief Advertising parameters configuration
 *
 * Configures the BLE advertising with the following parameters:
 * - Connectable advertising (allows connections from central devices)
 * - Uses identity address (uses the device's public or static random address)
 * - Advertising interval of approximately 500ms (800 * 0.625ms)
 * - Undirected advertising (broadcasts to any listening device)
 */
static const struct bt_le_adv_param* adv_param = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONN | BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
    800, /* Min Advertising Interval 500ms (800*0.625ms) */
    801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
    NULL); /* Set to NULL for undirected advertising */

/**
 * @brief Work item for handling advertising operations
 *
 * This work item is used to defer advertising operations to the system workqueue,
 * allowing advertising to be started asynchronously.
 */
static struct k_work adv_work;

/**
 * @brief Primary advertising data
 *
 * Contains the following advertising data:
 * - Flags: General discoverable mode and BR/EDR not supported
 * - Complete device name as configured in Kconfig
 */
static const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

/**
 * @brief Scan response data
 *
 * Contains the following scan response data:
 * - 128-bit service UUID that identifies the device's primary service
 *
 * The UUID 0x00001523-1212-efde-1523-785feabcd123 is a custom service UUID
 * used to identify this specific device/application. Central devices can scan
 * for this UUID to find compatible devices.
 */
static const struct bt_data sd[] = {
  BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)),
};

/**
 * @brief Handler for the advertising work item
 *
 * This function is called when the advertising work item is processed.
 * It starts the BLE advertising with the configured parameters and updates the module state.
 *
 * @param work Pointer to the work item being processed
 */
static void adv_work_handler(struct k_work* work)
{
  int err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

  if (err) {
    LOG_ERR("Advertising failed to start (err %d)", err);
    return;
  }

  current_ble_state = BLE_ADVERTISING;
  LOG_INF("Advertising successfully started");
}
/**
 * @brief Submit the advertising work item to the system workqueue
 *
 * This function submits the advertising work item to the system workqueue,
 * which will eventually call adv_work_handler to start advertising.
 */
static void advertising_start(void) { k_work_submit(&adv_work); }
/**
 * @brief Callback for BLE connection events
 *
 * This function is called when a BLE connection is established.
 * It updates the module state and logs the connection status.
 *
 * @param conn Pointer to the connection object
 * @param err Error code (0 for success, non-zero for failure)
 */
static void connected_cb(struct bt_conn *conn, uint8_t err)
{
  if (err) {
    LOG_ERR("Connection failed (err %u)", err);
    return;
  }

  LOG_INF("Connected");
  current_ble_state = BLE_CONNECTED;
}

/**
 * @brief Callback for BLE disconnection events
 *
 * This function is called when a BLE connection is terminated.
 * It updates the module state and logs the disconnection reason.
 *
 * @param conn Pointer to the connection object
 * @param reason Reason code for the disconnection
 */
static void disconnected_cb(struct bt_conn *conn, uint8_t reason)
{
  LOG_INF("Disconnected (reason %u)", reason);
  current_ble_state = BLE_DISCONNECTED;
}

/**
 * @brief Callback for BLE connection object recycling
 *
 * This function is called when a connection object is recycled and becomes available again.
 * It restarts advertising to allow new connections.
 */
static void recycled_cb(void)
{
  LOG_INF("Connection object recycled, restarting advertising");
  advertising_start();
}

/**
 * @brief Connection callbacks definition
 *
 * Defines the callbacks that will be invoked for various connection events:
 * - connected: Called when a connection is established
 * - disconnected: Called when a connection is terminated
 * - recycled: Called when a connection object is recycled
 */
BT_CONN_CB_DEFINE(conn_callbacks) = {
  .connected = connected_cb,
  .disconnected = disconnected_cb,
  .recycled = recycled_cb,
};

/**
 * @brief Initialize the BLE module
 *
 * This function performs the following initialization steps:
 * 1. Creates a Bluetooth identity with a static random address
 * 2. Enables the Bluetooth subsystem
 * 3. Initializes the advertising work item
 * 4. Sets the initial BLE state to NOT_CONNECTED
 *
 * @return 0 on success, negative error code on failure
 */
int ble_init(void)
{
  int err;

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
  
  /* Initialize the advertising work item */
  k_work_init(&adv_work, adv_work_handler);
  
  /* Set the initial state */
  current_ble_state = BLE_NOT_CONNECTED;

  return 0;
}

/**
 * @brief Start BLE advertising
 *
 * This function initiates BLE advertising by submitting the advertising work item
 * to the system workqueue. The actual advertising start happens asynchronously
 * when the work item is processed.
 *
 * @return 0 on success (work item submitted successfully)
 */
int ble_start_advertising(void)
{
  advertising_start();

  LOG_INF("Advertising work submitted");
  return 0;
}

/**
 * @brief Stop BLE advertising
 *
 * This function stops any ongoing BLE advertising and updates the module state.
 * It should be called when advertising is no longer needed or before changing
 * advertising parameters.
 *
 * @return 0 on success, negative error code on failure
 */
int ble_stop_advertising(void)
{
  int err = bt_le_adv_stop();
  if (err) {
    LOG_ERR("Advertising failed to stop (err %d)", err);
    return err;
  }

  current_ble_state = BLE_NOT_CONNECTED;
  LOG_INF("Advertising successfully stopped");
  return 0;
}

/**
 * @brief Get the current BLE state
 *
 * This function returns the current operational state of the BLE module.
 * It can be used to determine if the module is advertising, connected,
 * or in another state before performing operations.
 *
 * @return The current state of the BLE module (ble_state_t enum value)
 */
ble_state_t ble_get_state(void)
{
  return current_ble_state;
}