/*
 *  Copyright (c) 2021, PACKETCRAFT, INC.
 *
 *  SPDX-License-Identifier: LicenseRef-PCFT
 */

#include "audio_datapath.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "audio_i2s.h"
#include "drivers/adau1787.h"

LOG_MODULE_REGISTER(audio_datapath, LOG_LEVEL_INF);

/*
 * Debug I2S transparency datapath.
 *
 * The normal datapath buffers decoded Bluetooth audio, forwards I2S RX data to
 * the encoder, and runs presentation/drift compensation. For hardware bring-up
 * this module is intentionally reduced to copy each I2S RX block directly to
 * the I2S TX path.
 */

static struct {
  bool datapath_initialized;
  bool stream_started;
  uint32_t pres_delay_us;
} ctrl_blk;

static uint32_t __aligned(sizeof(uint32_t)) i2s_buf_0[I2S_SAMPLES_NUM];
static uint32_t __aligned(sizeof(uint32_t)) i2s_buf_1[I2S_SAMPLES_NUM];
static uint32_t __aligned(sizeof(uint32_t)) i2s_buf_2[I2S_SAMPLES_NUM];
static uint32_t __aligned(sizeof(uint32_t)) i2s_buf_3[I2S_SAMPLES_NUM];

static void spt0_lock_log_work_handler(struct k_work* work)
{
  ARG_UNUSED(work);

  reg_word_t status2 = 0;
  int ret = adau1787_read_register(REG_STATUS2_IC_1_Sigma_ADDR, &status2);

  if (ret != 0) {
    LOG_ERR("Failed to read ADAU1787 STATUS2 for SPT0 lock: %d", ret);
    return;
  }

  LOG_INF("ADAU1787 STATUS2=0x%02x SPT0_LOCK=%u", status2,
      (status2 & R148_SPT0_LOCK_IC_1_Sigma_MASK) >> R148_SPT0_LOCK_IC_1_Sigma_SHIFT);
}

K_WORK_DEFINE(spt0_lock_log_work, spt0_lock_log_work_handler);

static void audio_datapath_i2s_blk_complete(
    uint32_t frame_start_ts_us, uint32_t* rx_buf_released, uint32_t const* tx_buf_released)
{
  ARG_UNUSED(frame_start_ts_us);

  if (!ctrl_blk.stream_started) {
    return;
  }

  if ((rx_buf_released == NULL) || (tx_buf_released == NULL)) {
    LOG_WRN("I2S transparency callback missing released buffer");
    return;
  }

  static uint32_t rx_log_counter;
  bool log_this_block = ((rx_log_counter++ % 1000) == 0);

  if (log_this_block) {
    uint32_t nonzero = 0;

    for (uint32_t i = 0; i < I2S_SAMPLES_NUM; i++) {
      if (rx_buf_released[i] != 0) {
        nonzero++;
      }
    }

    LOG_INF("I2S RX first=0x%08x nonzero=%u/%u", rx_buf_released[0], nonzero, I2S_SAMPLES_NUM);
    LOG_INF("I2S RX raw: %08x %08x %08x %08x %08x %08x %08x %08x", rx_buf_released[0], rx_buf_released[1],
        rx_buf_released[2], rx_buf_released[3], rx_buf_released[4], rx_buf_released[5], rx_buf_released[6],
        rx_buf_released[7]);
#if CONFIG_AUDIO_BIT_DEPTH_16
    LOG_INF("I2S RX dec: %d %d %d %d %d %d %d %d", (int16_t)(rx_buf_released[0] & 0xffff),
        (int16_t)(rx_buf_released[1] & 0xffff), (int16_t)(rx_buf_released[2] & 0xffff),
        (int16_t)(rx_buf_released[3] & 0xffff), (int16_t)(rx_buf_released[4] & 0xffff),
        (int16_t)(rx_buf_released[5] & 0xffff), (int16_t)(rx_buf_released[6] & 0xffff),
        (int16_t)(rx_buf_released[7] & 0xffff));
#elif CONFIG_AUDIO_BIT_DEPTH_32
    LOG_INF("I2S RX dec: %d %d %d %d", (int32_t)rx_buf_released[0], (int32_t)rx_buf_released[2],
        (int32_t)rx_buf_released[4], (int32_t)rx_buf_released[6]);
#endif
  }

  memcpy((uint32_t*)tx_buf_released, rx_buf_released, sizeof(i2s_buf_0));

  if (log_this_block) {
    const uint32_t* tx_buf = tx_buf_released;
    uint32_t nonzero = 0;

    for (uint32_t i = 0; i < I2S_SAMPLES_NUM; i++) {
      if (tx_buf[i] != 0) {
        nonzero++;
      }
    }

    LOG_INF("I2S TX first=0x%08x nonzero=%u/%u", tx_buf[0], nonzero, I2S_SAMPLES_NUM);
    LOG_INF("I2S TX raw: %08x %08x %08x %08x %08x %08x %08x %08x", tx_buf[0], tx_buf[1], tx_buf[2], tx_buf[3],
        tx_buf[4], tx_buf[5], tx_buf[6], tx_buf[7]);
#if CONFIG_AUDIO_BIT_DEPTH_16
    LOG_INF("I2S TX dec: %d %d %d %d %d %d %d %d", (int16_t)(tx_buf[0] & 0xffff), (int16_t)(tx_buf[1] & 0xffff),
        (int16_t)(tx_buf[2] & 0xffff), (int16_t)(tx_buf[3] & 0xffff), (int16_t)(tx_buf[4] & 0xffff),
        (int16_t)(tx_buf[5] & 0xffff), (int16_t)(tx_buf[6] & 0xffff), (int16_t)(tx_buf[7] & 0xffff));
#elif CONFIG_AUDIO_BIT_DEPTH_32
    LOG_INF("I2S TX dec: %d %d %d %d", (int32_t)tx_buf[0], (int32_t)tx_buf[2], (int32_t)tx_buf[4], (int32_t)tx_buf[6]);
#endif

    // k_work_submit(&spt0_lock_log_work);
  }

  audio_i2s_set_next_buf((uint8_t*)tx_buf_released, rx_buf_released);
}

static void audio_datapath_i2s_start(void)
{
  memset(i2s_buf_0, 0, sizeof(i2s_buf_0));
  memset(i2s_buf_1, 0, sizeof(i2s_buf_1));
  memset(i2s_buf_2, 0, sizeof(i2s_buf_2));
  memset(i2s_buf_3, 0, sizeof(i2s_buf_3));

  audio_i2s_start((uint8_t*)i2s_buf_0, i2s_buf_1);
  audio_i2s_set_next_buf((uint8_t*)i2s_buf_2, i2s_buf_3);
}

static void audio_datapath_i2s_stop(void)
{
  audio_i2s_stop();
}

int audio_datapath_tone_play(uint16_t freq, uint16_t dur_ms, float amplitude)
{
  ARG_UNUSED(freq);
  ARG_UNUSED(dur_ms);
  ARG_UNUSED(amplitude);

  LOG_WRN("Shell tone playback is disabled in I2S transparency mode");
  return -ENOTSUP;
}

void audio_datapath_tone_stop(void)
{
}

int audio_datapath_pres_delay_us_set(uint32_t delay_us)
{
  ctrl_blk.pres_delay_us = delay_us;
  return 0;
}

void audio_datapath_pres_delay_us_get(uint32_t* delay_us)
{
  if (delay_us != NULL) {
    *delay_us = ctrl_blk.pres_delay_us;
  }
}

void audio_datapath_stream_out(
    const uint8_t* buf, size_t size, uint32_t sdu_ref_us, bool bad_frame, uint32_t recv_frame_ts_us)
{
  ARG_UNUSED(buf);
  ARG_UNUSED(size);
  ARG_UNUSED(sdu_ref_us);
  ARG_UNUSED(bad_frame);
  ARG_UNUSED(recv_frame_ts_us);
}

int audio_datapath_start(struct data_fifo* fifo_rx)
{
  ARG_UNUSED(fifo_rx);

  if (!ctrl_blk.datapath_initialized) {
    LOG_WRN("Audio datapath not initialized");
    return -ECANCELED;
  }

  if (ctrl_blk.stream_started) {
    return -EALREADY;
  }

  ctrl_blk.stream_started = true;
  audio_datapath_i2s_start();

  LOG_INF("Audio datapath started in I2S transparency mode");
  return 0;
}

int audio_datapath_stop(void)
{
  if (!ctrl_blk.stream_started) {
    return -EALREADY;
  }

  ctrl_blk.stream_started = false;
  audio_datapath_i2s_stop();

  LOG_INF("Audio datapath stopped");
  return 0;
}

int audio_datapath_init(void)
{
  memset(&ctrl_blk, 0, sizeof(ctrl_blk));

  audio_i2s_blk_comp_cb_register(audio_datapath_i2s_blk_complete);
  audio_i2s_init();

  ctrl_blk.datapath_initialized = true;
  ctrl_blk.pres_delay_us = CONFIG_BT_AUDIO_PRESENTATION_DELAY_US;

  LOG_INF("Audio datapath initialized in I2S transparency mode");
  return 0;
}
