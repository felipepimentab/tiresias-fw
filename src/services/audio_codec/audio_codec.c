#include "audio_codec.h"
#include "drivers/adau1787.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(audio_codec_module, LOG_LEVEL_INF);

/* === Configuration Constants === */
#define AUDIO_CODEC_STACK_SIZE 2048 /* Stack size for codec thread */
#define AUDIO_CODEC_PRIORITY 3 /* Thread priority */

// ZBUS
ZBUS_SUBSCRIBER_DEFINE(codec_cmd_sub, 4);

ZBUS_CHAN_DEFINE(codec_state_chan, struct codec_state_chan_msg, NULL, NULL, ZBUS_OBSERVERS_EMPTY,
    ZBUS_MSG_INIT(.state = CODEC_STATE_OFF));

ZBUS_CHAN_DEFINE(codec_cmd_chan, struct codec_cmd_chan_msg, NULL, NULL, ZBUS_OBSERVERS(codec_cmd_sub),
    ZBUS_MSG_INIT(.cmd = CODEC_CMD_INIT));

// State
static codec_state current_state = CODEC_STATE_OFF;

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

// State Handlers
static void handle_state_off(codec_cmd cmd) { LOG_INF("Handling codec command %d in state %d", cmd, current_state); };

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

// State Machine
static void codec_state_machine(codec_cmd cmd)
{
  switch (current_state) {
  case CODEC_STATE_OFF:
    handle_state_off(cmd);
    break;
  case CODEC_STATE_INIT:
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

  /* Main thread loop */
  while (1) {
    const struct zbus_channel* chan;

    err = zbus_sub_wait(&codec_cmd_sub, &chan, K_FOREVER);
    if (err != 0) {
      LOG_ERR("Failed to wait for codec command: %d", err);
      continue;
    }

    const struct codec_cmd_chan_msg* msg = zbus_chan_msg(chan);
    if (msg == NULL) {
      LOG_ERR("Failed to get codec command message");
      continue;
    }

    codec_state_machine(msg->cmd);
  }
}

/* Define and automatically start the thread at boot time */
K_THREAD_DEFINE(
    audio_codec_thread, AUDIO_CODEC_STACK_SIZE, audio_codec_thread_fn, NULL, NULL, NULL, AUDIO_CODEC_PRIORITY, 0, 0);
