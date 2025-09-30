#include "controller.h"
#include "../services/audio_codec/audio_codec.h"
#include "../services/bluetooth/bluetooth.h"
#include "../system_modules/peripheral/peripheral.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(controller_module, CONFIG_LOG_DEFAULT_LEVEL);

/* === Configuration Constants === */
#define CONTROLLER_THREAD_STACK_SIZE 1024
#define CONTROLLER_THREAD_PRIORITY 3

/* ZBUS multi-channel observer */
ZBUS_SUBSCRIBER_DEFINE(controller_sub, 8);

ZBUS_CHAN_DECLARE(btn_event_chan, bluetooth_state_chan);

/**
 * @brief Current controller state.
 */
static controller_state current_state = CONTROLLER_STATE_OFF;

/**
 * @brief Set the controller state.
 *
 * @param state The new controller state.
 */
static void set_controller_state(controller_state state)
{
  if (current_state == state) {
    return;
  }

  current_state = state;
}

static void handle_state_off(struct zbus_channel* chan)
{
  LOG_WRN("******** Initializing ********");
  int ret = bluetooth_send_command(BLUETOOTH_CMD_INIT, 100);
  if (ret != 0) {
    LOG_ERR("Failed to send Bluetooth init command: %d", ret);
  }
  zbus_chan_add_obs(&bluetooth_state_chan, &controller_sub, K_MSEC(100));
  set_controller_state(CONTROLLER_STATE_INITIALIZING);
};

static void handle_state_initializing(struct zbus_channel* chan)
{
  bluetooth_state bt_state;
  int err = zbus_chan_read(&bluetooth_state_chan, &bt_state, K_MSEC(100));
  if (err != 0) {
    LOG_ERR("Failed to read Bluetooth bt_state: %d", err);
    return;
  }

  if (bt_state == BLUETOOTH_STATE_INITIALIZING) {
    LOG_WRN("Bluetooth is initializing");
    return;
  }

  if (bt_state == BLUETOOTH_STATE_INIT_ERROR) {
    LOG_ERR("Bluetooth initialization failed");
    set_controller_state(CONTROLLER_STATE_ERROR);
    zbus_chan_rm_obs(&bluetooth_state_chan, &controller_sub, K_MSEC(100));
    return;
  }

  if (bt_state == BLUETOOTH_STATE_NOT_CONNECTED) {
    set_controller_state(CONTROLLER_STATE_IDLE);
    zbus_chan_rm_obs(&bluetooth_state_chan, &controller_sub, K_MSEC(100));
    return;
  }
};

static void handle_state_idle(struct zbus_channel* chan) { };
static void handle_state_low_power(struct zbus_channel* chan) { };
static void handle_state_std_op(struct zbus_channel* chan) { };
static void handle_state_error(struct zbus_channel* chan) { };
static void handle_state_streaming(struct zbus_channel* chan) { };

static void controller_state_machine(struct zbus_channel* chan)
{
  switch (current_state) {
  case CONTROLLER_STATE_OFF:
    handle_state_off(chan);
    break;
  case CONTROLLER_STATE_INITIALIZING:
    handle_state_initializing(chan);
    break;
  case CONTROLLER_STATE_IDLE:
    handle_state_idle(chan);
    break;
  case CONTROLLER_STATE_LOW_POWER:
    handle_state_low_power(chan);
    break;
  case CONTROLLER_STATE_STD_OP:
    handle_state_std_op(chan);
    break;
  case CONTROLLER_STATE_ERROR:
    handle_state_error(chan);
    break;
  case CONTROLLER_STATE_STREAMING:
    handle_state_streaming(chan);
    break;
  default:
    LOG_ERR("Unknown controller state: %d", current_state);
    break;
  }
}

/* === Controller Thread Function === */

/**
 * @brief Controller thread function.
 */
static void controller_thread(void)
{
  LOG_INF("Controller thread started");
  struct zbus_channel* chan;
  /* Main thread loop */
  while (1) {
    int err = zbus_sub_wait(&controller_sub, &chan, K_FOREVER);
    if (err != 0) {
      LOG_ERR("Failed to wait for controller event: %d", err);
      continue;
    }

    controller_state_machine(chan);
  }
}

/* === Internal Resources === */
/* Static thread definition */
K_THREAD_DEFINE(controller_thread_id, CONTROLLER_THREAD_STACK_SIZE, controller_thread, NULL, NULL, NULL,
    CONTROLLER_THREAD_PRIORITY, 0, 0);