/**
 * @file i2s_control.c
 * @brief Modular I2S interface for RX and TX initialization, control, and future expansion.
 */

#include "i2s_control.h"
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2s.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(i2s_control_module, CONFIG_LOG_DEFAULT_LEVEL);

#define I2S_SAMPLE_FREQUENCY 44100 // or 48000
#define I2S_SAMPLE_BIT_WIDTH 16
#define I2S_CHANNELS 2
#define I2S_TIMEOUT_MS 1000
#define I2S_BLOCK_SIZE (sizeof(int16_t) * I2S_CHANNELS * I2S_SAMPLE_FREQUENCY / 100)
#define I2S_NUM_BLOCKS 4

static const struct device* i2s_rx_dev = DEVICE_DT_GET(DT_NODELABEL(i2s0));
static const struct device* i2s_tx_dev = DEVICE_DT_GET(DT_NODELABEL(i2s0));

K_MEM_SLAB_DEFINE_STATIC(i2s_block_slab, I2S_BLOCK_SIZE, I2S_NUM_BLOCKS, 4);

static struct i2s_config i2s_cfg = {
  .word_size = I2S_SAMPLE_BIT_WIDTH,
  .channels = I2S_CHANNELS,
  .format = I2S_FMT_DATA_FORMAT_I2S,
  .options = I2S_OPT_BIT_CLK_MASTER | I2S_OPT_FRAME_CLK_MASTER,
  .frame_clk_freq = I2S_SAMPLE_FREQUENCY,
  .mem_slab = &i2s_block_slab,
  .block_size = I2S_BLOCK_SIZE,
  .timeout = I2S_TIMEOUT_MS,
};

int i2s_control_init(void)
{
  if (!device_is_ready(i2s_rx_dev) || !device_is_ready(i2s_tx_dev)) {
    LOG_ERR("I2S device not ready");
    return -ENODEV;
  }

  int ret = i2s_configure(i2s_rx_dev, I2S_DIR_RX, &i2s_cfg);
  if (ret < 0) {
    LOG_ERR("RX configure failed: %d", ret);
    return ret;
  }

  ret = i2s_configure(i2s_tx_dev, I2S_DIR_TX, &i2s_cfg);
  if (ret < 0) {
    LOG_ERR("TX configure failed: %d", ret);
    return ret;
  }

  LOG_INF("I2S module initialized");
  return 0;
}

int i2s_control_start(void)
{
  void* block;
  for (int i = 0; i < 2; ++i) {
    if (k_mem_slab_alloc(&i2s_block_slab, &block, K_NO_WAIT) != 0) {
      LOG_ERR("Memory slab alloc failed");
      return -ENOMEM;
    }
    memset(block, 0, I2S_BLOCK_SIZE);
    if (i2s_write(i2s_tx_dev, block, I2S_BLOCK_SIZE) < 0) {
      LOG_ERR("TX write failed");
      return -EIO;
    }
  }

  int ret;
  ret = i2s_trigger(i2s_rx_dev, I2S_DIR_RX, I2S_TRIGGER_START);
  if (ret < 0) {
    LOG_ERR("RX start failed: %d", ret);
    return ret;
  }

  ret = i2s_trigger(i2s_tx_dev, I2S_DIR_TX, I2S_TRIGGER_START);
  if (ret < 0) {
    LOG_ERR("TX start failed: %d", ret);
    return ret;
  }

  LOG_INF("I2S transfer started");
  return 0;
}

int i2s_control_stop(void)
{
  int ret;
  ret = i2s_trigger(i2s_rx_dev, I2S_DIR_RX, I2S_TRIGGER_DROP);
  if (ret < 0) {
    LOG_ERR("RX stop failed: %d", ret);
    return ret;
  }

  ret = i2s_trigger(i2s_tx_dev, I2S_DIR_TX, I2S_TRIGGER_DROP);
  if (ret < 0) {
    LOG_ERR("TX stop failed: %d", ret);
    return ret;
  }

  LOG_INF("I2S transfer stopped");
  return 0;
}

// Future extension: implement i2s_control_read() and i2s_control_write() here
// to handle block-level RX/TX operations

// Future extension: implement callback or thread loop for continuous streaming