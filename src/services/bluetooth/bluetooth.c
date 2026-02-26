#include "bluetooth.h"
#include "modules/connection/connection.h"
#include "zbus_common.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(bluetooth, LOG_LEVEL_INF);

#define BT_THREAD_STACK_SIZE 4096
#define BT_THREAD_PRIORITY 4
#define BLUETOOTH_QUEUE_SIZE 4

ZBUS_SUBSCRIBER_DEFINE(bt_cmd_sub, 4);

ZBUS_CHAN_DEFINE(
    bt_state_chan, bt_state_chan_msg, NULL, NULL, ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(.state = BT_STATE_OFF));

ZBUS_CHAN_DEFINE(
    bt_cmd_chan, bt_cmd_chan_msg, NULL, NULL, ZBUS_OBSERVERS(bt_cmd_sub), ZBUS_MSG_INIT(.cmd = BT_CMD_INIT));

ZBUS_CHAN_DECLARE(led_chan);

static bt_state current_state = BT_STATE_OFF;

static void set_bt_state(bt_state new_state)
{
  struct bt_state_chan_msg msg;

  if (current_state == new_state) {
    return;
  }

  current_state = new_state;
  msg.state = new_state;

  struct led_chan_msg_t led_msg;

  led_msg.led = LED_3;
  if (new_state == BT_STATE_ADVERTISING) {
    led_msg.cmd = BLINK;
  } else if (new_state == BT_STATE_CONNECTED) {
    led_msg.cmd = TURN_ON;
  } else {
    led_msg.cmd = TURN_OFF;
  }

  int err = zbus_chan_pub(&bt_state_chan, &msg, K_MSEC(100));
  if (err) {
    LOG_ERR("Failed to publish Bluetooth state change: %d", err);
  }

  err = zbus_chan_pub(&led_chan, &led_msg, K_MSEC(100));
  if (err) {
    LOG_ERR("Failed to send LED command: %d", err);
  }
}

void ble_connected_cb(void)
{
  set_bt_state(BT_STATE_CONNECTED);
}

void ble_disconnected_cb(void)
{
  set_bt_state(BT_STATE_ADVERTISING);
}

static void handle_state_off(bt_cmd cmd)
{
  if (cmd != BT_CMD_INIT) {
    LOG_WRN("Command %d not valid in OFF state", cmd);
    return;
  }

  LOG_DBG("Initializing Bluetooth");
  set_bt_state(BT_STATE_INITIALIZING);
  int ret = ble_init(ble_connected_cb, ble_disconnected_cb);
  if (ret != 0) {
    LOG_ERR("Failed to initialize Bluetooth: %d", ret);
    set_bt_state(BT_STATE_INIT_ERROR);
    return;
  }
  set_bt_state(BT_STATE_NOT_CONNECTED);
}

static void handle_state_initializing(bt_cmd cmd)
{
  LOG_WRN("Command %d ignored in INITIALIZING state", cmd);
}

static void handle_state_not_connected(bt_cmd cmd)
{
  if (cmd != BT_CMD_ADVERTISE) {
    LOG_WRN("Command %d not valid in NOT_CONNECTED state", cmd);
    return;
  }

  LOG_DBG("Starting advertising");
  int ret = ble_start_advertising();
  if (ret != 0) {
    LOG_ERR("Failed to start advertising: %d", ret);
    return;
  }
  set_bt_state(BT_STATE_ADVERTISING);
}

static void handle_state_advertising(bt_cmd cmd)
{
  if (cmd != BT_CMD_ADVERTISE) {
    LOG_WRN("Command %d not valid in ADVERTISING state", cmd);
  }

  int ret = ble_stop_advertising();
  if (ret != 0) {
    LOG_ERR("Failed to stop advertising: %d", ret);
    return;
  }
  set_bt_state(BT_STATE_NOT_CONNECTED);
}

static void handle_state_connecting(bt_cmd cmd)
{
  switch (cmd) {
  case BT_CMD_DISCONNECT:
    LOG_DBG("Cancelling connection attempt");
    set_bt_state(BT_STATE_DISCONNECTING);
    set_bt_state(BT_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in CONNECTING state", cmd);
    break;
  }
  /* In a real implementation, connection success/failure would trigger state changes */
}

static void handle_state_connected(bt_cmd cmd)
{
  switch (cmd) {
  case BT_CMD_DISCONNECT:
    LOG_DBG("Disconnecting from connected state");
    set_bt_state(BT_STATE_DISCONNECTING);
    /* Add actual disconnection code here */
    set_bt_state(BT_STATE_NOT_CONNECTED);
    break;
  default:
    LOG_WRN("Command %d not valid in CONNECTED state", cmd);
    break;
  }
}

static void handle_state_config(bt_cmd cmd)
{
  /* Placeholder for CONFIG state handling */
  LOG_WRN("Command %d not implemented in CONFIG state", cmd);
}

static void handle_state_streaming(bt_cmd cmd)
{
  /* Placeholder for STREAMING state handling */
  LOG_WRN("Command %d not implemented in STREAMING state", cmd);
}

static void handle_state_fota(bt_cmd cmd)
{
  /* Placeholder for FOTA state handling */
  LOG_WRN("Command %d not implemented in FOTA state", cmd);
}

static void handle_state_disconnecting(bt_cmd cmd)
{
  /* Most commands would be ignored during disconnection */
  LOG_WRN("Command %d ignored in DISCONNECTING state", cmd);
}

static void bt_state_machine(bt_cmd cmd)
{
  /* State machine implementation - routes commands to appropriate handler */
  switch (current_state) {
  case BT_STATE_OFF:
    handle_state_off(cmd);
    break;
  case BT_STATE_INITIALIZING:
    handle_state_initializing(cmd);
    break;
  case BT_STATE_NOT_CONNECTED:
    handle_state_not_connected(cmd);
    break;
  case BT_STATE_ADVERTISING:
    handle_state_advertising(cmd);
    break;
  case BT_STATE_CONNECTING:
    handle_state_connecting(cmd);
    break;
  case BT_STATE_CONNECTED:
    handle_state_connected(cmd);
    break;
  case BT_STATE_CONFIG:
    handle_state_config(cmd);
    break;
  case BT_STATE_STREAMING:
    handle_state_streaming(cmd);
    break;
  case BT_STATE_FOTA:
    handle_state_fota(cmd);
    break;
  case BT_STATE_DISCONNECTING:
    handle_state_disconnecting(cmd);
    break;
  default:
    LOG_WRN("Unhandled Bluetooth state: %d", current_state);
    break;
  }
}

static void bt_thread(void* arg1, void* arg2, void* arg3)
{
  ARG_UNUSED(arg1);
  ARG_UNUSED(arg2);
  ARG_UNUSED(arg3);

  LOG_DBG("Bluetooth thread started.");
  int ret = 0;
  struct bt_cmd_chan_msg msg;

  while (1) {
    const struct zbus_channel* chan;

    ret = zbus_sub_wait(&bt_cmd_sub, &chan, K_FOREVER);
    if (ret != 0) {
      LOG_ERR("Error waiting for Bluetooth command: %d", ret);
      continue;
    }

    ret = zbus_chan_read(chan, &msg, K_MSEC(500));
    if (ret != 0) {
      LOG_ERR("Error reading Bluetooth command: %d", ret);
      continue;
    }

    bt_state_machine(msg.cmd);
  }
}

K_THREAD_DEFINE(bt_thread_id, BT_THREAD_STACK_SIZE, bt_thread, NULL, NULL, NULL, BT_THREAD_PRIORITY, 0, 0);