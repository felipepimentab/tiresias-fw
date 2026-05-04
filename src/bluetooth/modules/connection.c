#include "connection.h"
#include "advertising.h"
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(connection, CONFIG_LOG_DEFAULT_LEVEL);

struct bt_conn* my_conn = NULL;

static ble_connected_cb_t external_connected_cb = NULL;
static ble_disconnected_cb_t external_disconnected_cb = NULL;

static void connected_cb(struct bt_conn* conn, uint8_t err)
{
  if (err) {
    LOG_ERR("Connection failed (err %u)", err);
  } else {
    LOG_INF("Connected");
  }
  my_conn = bt_conn_ref(conn);

  if (external_connected_cb) {
    external_connected_cb(conn, err);
  }
}

static void disconnected_cb(struct bt_conn* conn, uint8_t reason)
{
  LOG_INF("Disconnected (reason %u)", reason);
  bt_conn_unref(my_conn);

  if (external_disconnected_cb) {
    external_disconnected_cb(conn, reason);
  }
}

static void recycled_cb(void)
{
  ble_start_advertising();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
  .connected = connected_cb,
  .disconnected = disconnected_cb,
  .recycled = recycled_cb,
};

int ble_init(ble_connected_cb_t connected_cb, ble_disconnected_cb_t disconnected_cb)
{
  int err;

  external_connected_cb = connected_cb;
  external_disconnected_cb = disconnected_cb;

  bt_addr_le_t addr;
  err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
  if (err) {
    LOG_ERR("Invalid BT address (err %d)", err);
  }

  err = bt_id_create(&addr, NULL);
  if (err < 0) {
    LOG_ERR("Creating new ID failed (err %d)", err);
  }

  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth enable failed (err %d)", err);
    return -1;
  }

  LOG_INF("Bluetooth initialized");

  return 0;
}
