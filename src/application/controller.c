#include "controller.h"
#include "macros_common.h"
#include "zbus_common.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(controller, CONFIG_LOG_DEFAULT_LEVEL);

#define CONTROLLER_THREAD_STACK_SIZE 1024
#define CONTROLLER_THREAD_PRIORITY 3
#define ZBUS_TIMEOUT_MS 100

ZBUS_SUBSCRIBER_DEFINE(controller_sub, 8);
ZBUS_CHAN_DECLARE(button_chan, bt_cmd_chan, bt_state_chan, codec_cmd_chan, codec_state_chan, led_chan);

ZBUS_CHAN_DEFINE(controller_event_chan, controller_event_chan_msg, NULL, NULL, ZBUS_OBSERVERS(controller_sub),
    ZBUS_MSG_INIT(.event = CONTROLLER_EVENT_INIT));

static controller_state current_state = CONTROLLER_STATE_OFF;

static void set_controller_state(controller_state state)
{
  if (current_state == state) {
    LOG_DBG("State already set to %d, ignoring", state);
    return;
  }

  LOG_DBG("State transition: %d -> %d", current_state, state);
  current_state = state;
}

/* === State Handler Functions === */

static void handle_state_off(struct zbus_channel* chan)
{
  int ret;

  LOG_INF("Initializing system services");

  struct bt_cmd_chan_msg new_bt_msg = { BT_CMD_INIT };
  ret = zbus_chan_pub(&bt_cmd_chan, &new_bt_msg, ZBUS_READ_TIMEOUT_MS);
  ERR_CHK(ret);

  struct codec_cmd_chan_msg new_codec_msg = { CODEC_CMD_INIT };
  ret = zbus_chan_pub(&codec_cmd_chan, &new_codec_msg, ZBUS_READ_TIMEOUT_MS);
  ERR_CHK(ret);

  ret = zbus_chan_add_obs(&bt_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
  ERR_CHK(ret);
  ret = zbus_chan_add_obs(&button_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
  ERR_CHK(ret);
  ret = zbus_chan_add_obs(&codec_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
  ERR_CHK(ret);

  set_controller_state(CONTROLLER_STATE_INITIALIZING);
}

static void handle_state_initializing(struct zbus_channel* chan)
{
  bt_state bt_state;
  codec_state cd_state;
  int ret;

  ret = zbus_chan_read(&bt_state_chan, &bt_state, K_MSEC(ZBUS_TIMEOUT_MS));
  ERR_CHK(ret);

  if (bt_state == BT_STATE_INITIALIZING) {
    LOG_DBG("Bluetooth service still initializing");
    return;
  }

  if (bt_state == BT_STATE_INIT_ERROR) {
    LOG_ERR("Bluetooth initialization failed");
    set_controller_state(CONTROLLER_STATE_ERROR);
    (void)zbus_chan_rm_obs(&bt_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    (void)zbus_chan_rm_obs(&codec_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    return;
  }

  ret = zbus_chan_read(&codec_state_chan, &cd_state, K_MSEC(ZBUS_TIMEOUT_MS));
  ERR_CHK(ret);

  if (cd_state == CODEC_STATE_INITIALIZING) {
    LOG_DBG("Audio codec service still initializing");
    return;
  }

  if (cd_state == CODEC_STATE_ERROR) {
    LOG_ERR("Audio codec initialization failed");
    set_controller_state(CONTROLLER_STATE_ERROR);
    (void)zbus_chan_rm_obs(&bt_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    (void)zbus_chan_rm_obs(&codec_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    return;
  }

  LOG_DBG("Service states - Bluetooth: %d, Codec: %d", bt_state, cd_state);

  if (bt_state == BT_STATE_NOT_CONNECTED && cd_state == CODEC_STATE_IDLE) {
    LOG_INF("System initialization complete");
    set_controller_state(CONTROLLER_STATE_IDLE);

    struct bt_cmd_chan_msg new_bt_msg = { BT_CMD_ADVERTISE };
    ret = zbus_chan_pub(&bt_cmd_chan, &new_bt_msg, ZBUS_READ_TIMEOUT_MS);
    ERR_CHK(ret);

    (void)zbus_chan_rm_obs(&bt_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    (void)zbus_chan_rm_obs(&codec_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    return;
  }
}

static void handle_state_idle(struct zbus_channel* chan)
{
  int ret;
  struct btn_chan_msg_t button_msg;

  if (chan != &button_chan) {
    return;
  }

  ret = zbus_chan_read(&button_chan, &button_msg, K_MSEC(ZBUS_TIMEOUT_MS));
  ERR_CHK(ret);

  if (button_msg.event != BUTTON_1_PRESSED) {
    return;
  }

  LOG_INF("Button 1 pressed, switching codec");

  struct codec_cmd_chan_msg codec_msg = { CODEC_CMD_SWITCH };
  ret = zbus_chan_pub(&codec_cmd_chan, &codec_msg, ZBUS_READ_TIMEOUT_MS);
  ERR_CHK(ret);
}

static void handle_state_low_power(struct zbus_channel* chan)
{
  /* TODO: Implement low power mode handling */
  LOG_DBG("Low power state handler called");
}

static void handle_state_std_op(struct zbus_channel* chan)
{
  /* TODO: Implement standard operation handling */
  LOG_DBG("Standard operation state handler called");
}

static void handle_state_error(struct zbus_channel* chan)
{
  /* TODO: Implement error state handling and recovery */
  LOG_DBG("Error state handler called");
}

static void handle_state_streaming(struct zbus_channel* chan)
{
  /* TODO: Implement audio streaming handling */
  LOG_DBG("Streaming state handler called");
}

static void controller_state_machine(struct zbus_channel* chan)
{
  if (chan == NULL) {
    LOG_ERR("Invalid channel pointer");
    return;
  }

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
    set_controller_state(CONTROLLER_STATE_ERROR);
    break;
  }
}

static void controller_thread(void)
{
  struct zbus_channel* chan;
  int ret;

  LOG_INF("Controller thread started");

  while (1) {
    ret = zbus_sub_wait(&controller_sub, &chan, K_FOREVER);
    if (ret != 0) {
      LOG_ERR("Failed to wait for controller event: %d", ret);
      continue;
    }

    controller_state_machine(chan);
  }
}

int controller_init(void)
{
  int ret = zbus_chan_notify(&controller_event_chan, K_NO_WAIT);
  ERR_CHK(ret);
  return ret;
}

K_THREAD_DEFINE(controller_thread_id, CONTROLLER_THREAD_STACK_SIZE, controller_thread, NULL, NULL, NULL,
    CONTROLLER_THREAD_PRIORITY, 0, 0);
