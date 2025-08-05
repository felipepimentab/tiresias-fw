/**
 * @file ble.c
 * @brief Modular BLE interface for initialization, control, and future expansion.
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

/* Current state of the BLE module */
static ble_state_t current_ble_state = BLE_OFF;

static const struct bt_le_adv_param* adv_param = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONN | BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
    800, /* Min Advertising Interval 500ms (800*0.625ms) */
    801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
    NULL); /* Set to NULL for undirected advertising */

static struct k_work adv_work;

static const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
  BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)),
};

static void adv_work_handler(struct k_work* work)
{
  int err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

  if (err) {
    printk("Advertising failed to start (err %d)\n", err);
    return;
  }

  current_ble_state = BLE_ADVERTISING;
  printk("Advertising successfully started\n");
}
static void advertising_start(void) { k_work_submit(&adv_work); }
static void connected_cb(struct bt_conn *conn, uint8_t err)
{
  if (err) {
    LOG_ERR("Connection failed (err %u)", err);
    return;
  }

  LOG_INF("Connected");
  current_ble_state = BLE_CONNECTED;
}

static void disconnected_cb(struct bt_conn *conn, uint8_t reason)
{
  LOG_INF("Disconnected (reason %u)", reason);
  current_ble_state = BLE_DISCONNECTED;
}

static void recycled_cb(void)
{
  printk("Connection object available from previous conn. Disconnect is complete!\n");
  advertising_start();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
  .connected = connected_cb,
  .disconnected = disconnected_cb,
  .recycled = recycled_cb,
};

int ble_init(void)
{
  int err;

  bt_addr_le_t addr;
  err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
  if (err) {
    printk("Invalid BT address (err %d)\n", err);
  }

  err = bt_id_create(&addr, NULL);
  if (err < 0) {
    printk("Creating new ID failed (err %d)\n", err);
  }

  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth enable failed (err %d)", err);
    return -1;
  }

  LOG_INF("Bluetooth initialized\n");
  k_work_init(&adv_work, adv_work_handler);
  
  current_ble_state = BLE_NOT_CONNECTED;

  return 0;
}

int ble_start_advertising(void)
{
  advertising_start();

  LOG_INF("Advertising successfully started\n");
  return 0;
}

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

ble_state_t ble_get_state(void)
{
  return current_ble_state;
}