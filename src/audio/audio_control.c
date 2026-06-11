#include "audio_control.h"
#include "adau_1787_IC_1_SIGMA_PARAM.h"
#include "drivers/adau1787.h"
#include "macros_common.h"
#include "modules/audio_datapath.h"
#include "modules/audio_i2s.h"
#include "zbus_common.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(audio_control_module, LOG_LEVEL_INF);

#define CODEC_THREAD_STACK_SIZE 8192
#define CODEC_THREAD_PRIORITY 3

ZBUS_SUBSCRIBER_DEFINE(codec_cmd_sub, 4);

ZBUS_CHAN_DEFINE(codec_state_chan, struct codec_state_chan_msg, NULL, NULL, ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(0));

ZBUS_CHAN_DEFINE(
    codec_cmd_chan, struct codec_cmd_chan_msg, NULL, NULL, ZBUS_OBSERVERS(codec_cmd_sub), ZBUS_MSG_INIT(0));

static codec_state current_state = CODEC_STATE_OFF;

static uint32_t param_word_to_u32(const param_word_t param_word)
{
  return ((uint32_t)param_word[0] << 24) | ((uint32_t)param_word[1] << 16) | ((uint32_t)param_word[2] << 8) |
         param_word[3];
}

static void u32_to_param_word(uint32_t value, param_word_t param_word)
{
  param_word[0] = (value >> 24) & 0xFF;
  param_word[1] = (value >> 16) & 0xFF;
  param_word[2] = (value >> 8) & 0xFF;
  param_word[3] = value & 0xFF;
}

static const char* stereo_switch_value_label(uint32_t value)
{
  switch (value) {
  case 0:
    return "False";
  case 1:
    return "True";
  default:
    return "Unknown";
  }
}

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
  int ret;
  if (cmd != CODEC_CMD_INIT) {
    return;
  }

  set_codec_state(CODEC_STATE_INITIALIZING);

  ret = audio_datapath_init();
  ERR_CHK(ret);
  ret = audio_datapath_start(NULL);
  ERR_CHK(ret);
  ret = adau1787_init();
  ERR_CHK(ret);

  set_codec_state(CODEC_STATE_IDLE);
};

static void handle_state_initializing(codec_cmd cmd)
{
  LOG_DBG("Handling codec command %d in state %d", cmd, current_state);
};

static void handle_state_idle(codec_cmd cmd)
{
  int ret;
  param_word_t codec_param;
  uint32_t codec_param_value;
  uint32_t toggled_codec_param_value;

  LOG_DBG("Handling codec command %d in state %d", cmd, current_state);

  if (cmd != CODEC_CMD_SWITCH) {
    return;
  }

  ret = adau1787_read(MOD_NX2_1_STEREOSWSLEW_ADDR, codec_param, sizeof(codec_param));
  if (ret != 0) {
    LOG_ERR("Failed to read codec parameter at 0x%04X: %d", MOD_NX2_1_STEREOSWSLEW_ADDR, ret);
    return;
  }

  codec_param_value = param_word_to_u32(codec_param);
  toggled_codec_param_value = (codec_param_value == 0U) ? 1U : 0U;
  u32_to_param_word(toggled_codec_param_value, codec_param);

  ret = adau1787_write(MOD_NX2_1_STEREOSWSLEW_ADDR, codec_param, sizeof(codec_param));
  if (ret != 0) {
    LOG_ERR("Failed to write codec parameter at 0x%04X: %d", MOD_NX2_1_STEREOSWSLEW_ADDR, ret);
    return;
  }

  ret = adau1787_read(MOD_NX2_1_STEREOSWSLEW_ADDR, codec_param, sizeof(codec_param));
  if (ret != 0) {
    LOG_ERR("Failed to read toggled codec parameter at 0x%04X: %d", MOD_NX2_1_STEREOSWSLEW_ADDR, ret);
    return;
  }

  codec_param_value = param_word_to_u32(codec_param);

  LOG_INF("Stereo Switch Nx2 at 0x%04X toggled to %u (%s), raw=0x%08X", MOD_NX2_1_STEREOSWSLEW_ADDR,
      codec_param_value, stereo_switch_value_label(codec_param_value), codec_param_value);
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

static void audio_control_thread(void)
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

K_THREAD_DEFINE(audio_control_thread_id, CODEC_THREAD_STACK_SIZE, audio_control_thread, NULL, NULL, NULL,
    CODEC_THREAD_PRIORITY, 0, 0);
