#include "audio_codec.h"
#include "drivers/adau1787.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(audio_codec_module, LOG_LEVEL_INF);

/* === Configuration Constants === */
#define AUDIO_CODEC_STACK_SIZE 2048 /* Stack size for codec thread */
#define AUDIO_CODEC_PRIORITY 3 /* Thread priority */

/* === Zbus Configuration === */
/* Subscriber for Audio Codec commands channel */
ZBUS_SUBSCRIBER_DEFINE(codec_cmd_sub, 4);

/* Channel for Audio Codec state updates */
ZBUS_CHAN_DEFINE(codec_state_chan, struct codec_state_chan_msg, NULL, NULL, ZBUS_OBSERVERS_EMPTY,
    ZBUS_MSG_INIT(.state = CODEC_STATE_OFF));

/* Channel for Audio Codec commands */
ZBUS_CHAN_DEFINE(codec_cmd_chan, struct codec_cmd_chan_msg, NULL, NULL, ZBUS_OBSERVERS(codec_cmd_sub),
    ZBUS_MSG_INIT(.cmd = CODEC_CMD_INIT));

/**
 * @brief Public API function to send commands to the Audio Codec service.
 *
 * @param cmd The command to send.
 * @return int 0 on success, negative error code on failure.
 */
int codec_send_command(codec_cmd cmd)
{
  struct codec_cmd_chan_msg msg;
  msg.cmd = cmd;

  int err = zbus_chan_pub(&codec_cmd_chan, &msg, K_MSEC(100));
  if (err != 0) {
    LOG_ERR("Failed to publish codec command message: %d", err);
    return err;
  }

  return 0;
}

/* === State Management === */

/**
 * @brief Current codec state.
 */
static codec_state current_state = CODEC_STATE_OFF;

/**
 * @brief Set the codec state.
 *
 * @param state The new codec state.
 */
static void set_codec_state(codec_state state)
{
  struct codec_state_chan_msg msg;

  if (current_state == state) {
    return;
  }

  current_state = state;
  msg.state = state;

  int err = zbus_chan_pub(&codec_state_chan, &msg, K_MSEC(100));
  if (err != 0) {
    LOG_ERR("Failed to publish codec state message: %d", err);
  }
}

/* === State Handlers === */

/**
 * @brief Handle codec command in state CODEC_STATE_OFF.
 *
 * @param cmd The command to handle.
 */
static void handle_state_off(codec_cmd cmd)
{
  if (cmd != CODEC_CMD_INIT) {
    return;
  }

  set_codec_state(CODEC_STATE_INITIALIZING);
  int ret = adau1787_init();
  if (ret != 0) {
    LOG_ERR("Failed to initialize ADAU1787 codec: %d", ret);
    set_codec_state(CODEC_STATE_ERROR);
    return;
  }

  set_codec_state(CODEC_STATE_IDLE);
};

static void handle_state_initializing(codec_cmd cmd)
{
  LOG_INF("Handling codec command %d in state %d", cmd, current_state);
};

static void handle_state_idle(codec_cmd cmd) { LOG_INF("Handling codec command %d in state %d", cmd, current_state); };

static void handle_state_error(codec_cmd cmd) { LOG_INF("Handling codec command %d in state %d", cmd, current_state); };

static void handle_state_standard(codec_cmd cmd)
{
  LOG_INF("Handling codec command %d in state %d", cmd, current_state);
};

static void handle_state_streaming(codec_cmd cmd)
{
  LOG_INF("Handling codec command %d in state %d", cmd, current_state);
};

/* === State Machine === */

/**
 * @brief Audio codec state machine.
 *
 * @param cmd The command to handle.
 */
static void codec_state_machine(codec_cmd cmd)
{
  switch (current_state) {
  case CODEC_STATE_OFF:
    handle_state_off(cmd);
    break;
  case CODEC_STATE_INITIALIZING:
    handle_state_initializing(cmd);
    break;
  case CODEC_STATE_IDLE:
    handle_state_idle(cmd);
    break;
  case CODEC_STATE_ERROR:
    handle_state_error(cmd);
    break;
  case CODEC_STATE_STANDARD:
    handle_state_standard(cmd);
    break;
  case CODEC_STATE_STREAMING:
    handle_state_streaming(cmd);
    break;
  default:
    LOG_ERR("Unhandled Audio Codec state: %d", current_state);
    break;
  }
}

/* === Codec Thread Function === */

/**
 * @brief Audio codec control thread
 *
 * Handles audio codec operations directly
 */
static void audio_codec_thread_fn(void)
{
  LOG_INF("Audio codec thread started");
  int err;
  struct codec_cmd_chan_msg msg;

  while (1) {
    const struct zbus_channel* chan;

    err = zbus_sub_wait(&codec_cmd_sub, &chan, K_FOREVER);
    if (err != 0) {
      LOG_ERR("Failed to wait for codec command: %d", err);
      continue;
    }

    err = zbus_chan_read(chan, &msg, K_MSEC(500));
    if (err != 0) {
      LOG_ERR("Failed to read codec command message: %d", err);
      continue;
    }

    codec_state_machine(msg.cmd);
  }
}

/* Define and automatically start the thread at boot time */
K_THREAD_DEFINE(
    audio_codec_thread, AUDIO_CODEC_STACK_SIZE, audio_codec_thread_fn, NULL, NULL, NULL, AUDIO_CODEC_PRIORITY, 0, 0);
