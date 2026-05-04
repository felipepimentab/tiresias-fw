#include "audio_codec.h"
#include "drivers/adau1787.h"
#include "macros_common.h"
#include "modules/i2s/i2s_control.h"
#include "zbus_common.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(audio_codec_module, LOG_LEVEL_INF);

#define CODEC_THREAD_STACK_SIZE 2048
#define CODEC_THREAD_PRIORITY 3

ZBUS_SUBSCRIBER_DEFINE(codec_cmd_sub, 4);

ZBUS_CHAN_DEFINE(codec_state_chan, struct codec_state_chan_msg, NULL, NULL, ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(0));

ZBUS_CHAN_DEFINE(
    codec_cmd_chan, struct codec_cmd_chan_msg, NULL, NULL, ZBUS_OBSERVERS(codec_cmd_sub), ZBUS_MSG_INIT(0));

static codec_state current_state = CODEC_STATE_OFF;

static void set_codec_state(codec_state state)
{
  struct codec_state_chan_msg msg;

  if (current_state == state) {
    return;
  }

  current_state = state;
  msg.state = state;

  int err = zbus_chan_pub(&codec_state_chan, &msg, ZBUS_READ_TIMEOUT_MS);
  if (err != 0) {
    LOG_ERR("Failed to publish codec state message: %d", err);
  }
}

/* === State Handlers === */

static void handle_state_off(codec_cmd cmd)
{
  if (cmd != CODEC_CMD_INIT) {
    return;
  }

  set_codec_state(CODEC_STATE_INITIALIZING);

#if CONFIG_AUDIO_CODEC_ADAU1787
  int ret = adau1787_init();
  ERR_CHK(ret);
  ret = audio_i2s_init();
  ERR_CHK(ret);
#endif

  set_codec_state(CODEC_STATE_IDLE);
};

static void handle_state_initializing(codec_cmd cmd)
{
  LOG_DBG("Handling codec command %d in state %d", cmd, current_state);
};

static void handle_state_idle(codec_cmd cmd)
{
  LOG_DBG("Handling codec command %d in state %d", cmd, current_state);
};

static void handle_state_error(codec_cmd cmd)
{
  LOG_DBG("Handling codec command %d in state %d", cmd, current_state);
};

static void handle_state_standard(codec_cmd cmd)
{
  LOG_DBG("Handling codec command %d in state %d", cmd, current_state);
};

static void handle_state_streaming(codec_cmd cmd)
{
  LOG_DBG("Handling codec command %d in state %d", cmd, current_state);
};

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

static void audio_codec_thread(void)
{
  LOG_DBG("Audio codec thread started");
  int ret = 0;
  struct codec_cmd_chan_msg msg;

  while (1) {
    const struct zbus_channel* chan;

    ret = zbus_sub_wait(&codec_cmd_sub, &chan, K_FOREVER);
    if (ret != 0) {
      LOG_ERR("Failed to wait for codec command: %d", ret);
      continue;
    }

    ret = zbus_chan_read(chan, &msg, K_MSEC(500));
    if (ret != 0) {
      LOG_ERR("Failed to read codec command message: %d", ret);
      continue;
    }

    codec_state_machine(msg.cmd);
  }
}

K_THREAD_DEFINE(
    audio_codec_thread_id, CODEC_THREAD_STACK_SIZE, audio_codec_thread, NULL, NULL, NULL, CODEC_THREAD_PRIORITY, 0, 0);