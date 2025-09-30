#ifndef AUDIO_CODEC_H
#define AUDIO_CODEC_H

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

/**
 * @file audio_codec.h
 * @brief Audio codec control module interface
 *
 * This module provides a direct interface for controlling an audio codec
 * (e.g., ADAU1787). The codec is initialized automatically at boot time
 * through a dedicated thread created with K_THREAD_DEFINE.
 */

/* Audio codec state */
typedef enum codec_state {
  CODEC_STATE_OFF,
  CODEC_STATE_INITIALIZING,
  CODEC_STATE_IDLE,
  CODEC_STATE_ERROR,
  CODEC_STATE_STANDARD,
  CODEC_STATE_STREAMING,
} codec_state;

/* Controller commands to Audio Codec */
typedef enum codec_cmd {
  CODEC_CMD_INIT,
  CODEC_CMD_START_STANDARD,
  CODEC_CMD_START_STREAMING,
  CODEC_CMD_STOP_STREAMING,
} codec_cmd;

typedef struct codec_state_chan_msg {
  codec_state state;
} codec_state_chan_msg;

/* Audio codec command channel message */
typedef struct codec_cmd_chan_msg {
  codec_cmd cmd;
} codec_cmd_chan_msg;

ZBUS_CHAN_DECLARE(codec_state_chan);
ZBUS_CHAN_DECLARE(codec_cmd_chan);

#endif /* AUDIO_CODEC_H */