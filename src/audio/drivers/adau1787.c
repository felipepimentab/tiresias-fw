#include "adau1787.h"
#include "SigmaStudioFW.h"
#include "adau_1787_IC_1_FAST.h"
#include "adau_1787_IC_1_FAST_PARAM.h"
#include "adau_1787_IC_1_FAST_REG.h"
#include "adau_1787_IC_1_SIGMA.h"
#include "adau_1787_IC_1_SIGMA_PARAM.h"
#include "adau_1787_IC_1_SIGMA_REG.h"
#include "macros_common.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(adau1787_driver, LOG_LEVEL_INF);

/** @brief Device Tree Specification for ADAU1787 */
#define ADAU1787_NODE DT_NODELABEL(adau_1787)

/** @brief I2C device configuration structure for ADAU1787 */
const struct i2c_dt_spec adau1787_i2c = I2C_DT_SPEC_GET(ADAU1787_NODE);
/** @brief Codec !PD pin (Power Down - active low) */
static const struct gpio_dt_spec codec_powerdown = GPIO_DT_SPEC_GET(ADAU1787_NODE, powerdown_gpios);
/** @brief Codec MP3 pin (Multi Purpose pin 3) */
static const struct gpio_dt_spec codec_mp3 = GPIO_DT_SPEC_GET(ADAU1787_NODE, mp3_gpios);
/** @brief Codec MP4 pin (Multi Purpose pin 4) */
static const struct gpio_dt_spec codec_mp4 = GPIO_DT_SPEC_GET(ADAU1787_NODE, mp4_gpios);
/** @brief Codec MP5 pin (Multi Purpose pin 5) */
static const struct gpio_dt_spec codec_mp5 = GPIO_DT_SPEC_GET(ADAU1787_NODE, mp5_gpios);
/** @brief Codec MP6 pin (Multi Purpose pin 6) */
static const struct gpio_dt_spec codec_mp6 = GPIO_DT_SPEC_GET(ADAU1787_NODE, mp6_gpios);

static int adau_init_error = 0;

#define ADAU1787_FIELD_GET(value, mask, shift) (((value) & (mask)) >> (shift))

static void adau1787_log_status_2(void)
{
  reg_word_t status2 = 0;
  int ret = adau1787_read_register(REG_STATUS2_IC_1_Sigma_ADDR, &status2);

  if (ret != 0) {
    LOG_ERR("Failed to read ADAU1787 STATUS2: %d", ret);
    return;
  }

  const uint8_t power_up_complete
      = ADAU1787_FIELD_GET(status2, R148_POWER_UP_COMPLETE_IC_1_Sigma_MASK, R148_POWER_UP_COMPLETE_IC_1_Sigma_SHIFT);
  const uint8_t sync_lock
      = ADAU1787_FIELD_GET(status2, R148_SYNC_LOCK_IC_1_Sigma_MASK, R148_SYNC_LOCK_IC_1_Sigma_SHIFT);
  const uint8_t spt1_lock
      = ADAU1787_FIELD_GET(status2, R148_SPT1_LOCK_IC_1_Sigma_MASK, R148_SPT1_LOCK_IC_1_Sigma_SHIFT);
  const uint8_t spt0_lock
      = ADAU1787_FIELD_GET(status2, R148_SPT0_LOCK_IC_1_Sigma_MASK, R148_SPT0_LOCK_IC_1_Sigma_SHIFT);
  const uint8_t asrco_lock
      = ADAU1787_FIELD_GET(status2, R148_ASRCO_LOCK_IC_1_Sigma_MASK, R148_ASRCO_LOCK_IC_1_Sigma_SHIFT);
  const uint8_t asrci_lock
      = ADAU1787_FIELD_GET(status2, R148_ASRCI_LOCK_IC_1_Sigma_MASK, R148_ASRCI_LOCK_IC_1_Sigma_SHIFT);
  const uint8_t avdd_uvw = ADAU1787_FIELD_GET(status2, R148_AVDD_UVW_IC_1_Sigma_MASK, R148_AVDD_UVW_IC_1_Sigma_SHIFT);
  const uint8_t pll_lock = ADAU1787_FIELD_GET(status2, R148_PLL_LOCK_IC_1_Sigma_MASK, R148_PLL_LOCK_IC_1_Sigma_SHIFT);

  LOG_INF("ADAU1787 STATUS2=0x%02x", status2);
  LOG_INF("POWER_UP_COMPLETE = %u (%s)", power_up_complete,
      power_up_complete ? "power domains powered up after POWER_EN=1" : "power domains are not fully powered up");
  LOG_INF("SYNC_LOCK = %u (%s)", sync_lock,
      sync_lock ? "multichip synchronization is locked" : "multichip synchronization is not locked");
  LOG_INF("SPT1_LOCK = %u (%s)", spt1_lock, spt1_lock ? "Serial Port 1 is locked" : "Serial Port 1 is not locked");
  LOG_INF("SPT0_LOCK = %u (%s)", spt0_lock, spt0_lock ? "Serial Port 0 is locked" : "Serial Port 0 is not locked");
  LOG_INF(
      "ASRCO_LOCK = %u (%s)", asrco_lock, asrco_lock ? "output ASRC is locked" : "output ASRC is currently unlocked");
  LOG_INF("ASRCI_LOCK = %u (%s)", asrci_lock, asrci_lock ? "input ASRC is locked" : "input ASRC is currently unlocked");
  LOG_INF("AVDD_UVW = %u (%s)", avdd_uvw, avdd_uvw ? "undervoltage on AVDD detected" : "AVDD is in normal operation");
  LOG_INF("PLL_LOCK = %u (%s)", pll_lock, pll_lock ? "PLL is locked" : "PLL is not locked");
}

/**
 * @brief Configures GPIO pins for the ADAU1787
 */
int adau1787_config_gpios(void)
{
  int ret = 0;

  if (!gpio_is_ready_dt(&codec_powerdown)) {
    LOG_ERR("ADAU1787 power-down GPIO controller is not ready");
    return -ENODEV;
  }
  // Start with codec powered down to ensure it resets whenever the nordic resets
  ret = gpio_pin_configure_dt(&codec_powerdown, GPIO_OUTPUT_ACTIVE);
  if (ret != 0) {
    LOG_ERR("Failed to configure ADAU1787 power-down GPIO: %d", ret);
    return ret;
  }

  if (!gpio_is_ready_dt(&codec_mp3)) {
    LOG_ERR("ADAU1787 MP3 controller is not ready");
    return -ENODEV;
  }
  ret = gpio_pin_configure_dt(&codec_mp3, GPIO_OUTPUT_INACTIVE);
  if (ret != 0) {
    LOG_ERR("Failed to configure ADAU1787 MP3 GPIO: %d", ret);
    return ret;
  }

  if (!gpio_is_ready_dt(&codec_mp4)) {
    LOG_ERR("ADAU1787 MP4 controller is not ready");
    return -ENODEV;
  }
  ret = gpio_pin_configure_dt(&codec_mp4, GPIO_OUTPUT_INACTIVE);
  if (ret != 0) {
    LOG_ERR("Failed to configure ADAU1787 MP4 GPIO: %d", ret);
    return ret;
  }

  if (!gpio_is_ready_dt(&codec_mp5)) {
    LOG_ERR("ADAU1787 MP5 controller is not ready");
    return -ENODEV;
  }
  ret = gpio_pin_configure_dt(&codec_mp5, GPIO_OUTPUT_INACTIVE);
  if (ret != 0) {
    LOG_ERR("Failed to configure ADAU1787 MP5 GPIO: %d", ret);
    return ret;
  }

  if (!gpio_is_ready_dt(&codec_mp6)) {
    LOG_ERR("ADAU1787 MP6 controller is not ready");
    return -ENODEV;
  }
  ret = gpio_pin_configure_dt(&codec_mp6, GPIO_OUTPUT_INACTIVE);
  if (ret != 0) {
    LOG_ERR("Failed to configure ADAU1787 MP6 GPIO: %d", ret);
    return ret;
  }

  return ret;
}

int adau1787_power_up(void)
{
  int ret = gpio_pin_set_dt(&codec_powerdown, 0);
  if (ret != 0) {
    LOG_ERR("Failed to release ADAU1787 power-down GPIO: %d", ret);
  }
  return ret;
}

int adau1787_power_down(void)
{
  int ret = gpio_pin_set_dt(&codec_powerdown, 1);
  if (ret != 0) {
    LOG_ERR("Failed to assert ADAU1787 power-down GPIO: %d", ret);
  }
  return ret;
}

/**
 * @brief Configures the I2C bus for the ADAU1787
 */
int adau1787_config_i2c(void)
{
  int ret = 0;

  const struct device* i2c_dev = device_get_binding("I2C_1");
  if (!i2c_dev) {
    LOG_ERR("I2C binding failed.");
    return -1;
  }

  i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_FAST_PLUS));

  LOG_DBG("SCL pin: %d", NRF_TWIM1->PSEL.SCL);
  LOG_DBG("SDA pin: %d", NRF_TWIM1->PSEL.SDA);
  LOG_DBG("I2C frequency: %d", NRF_TWIM1->FREQUENCY);

  if (!device_is_ready(adau1787_i2c.bus)) {
    LOG_ERR("I2C bus %s is not ready!", adau1787_i2c.bus->name);
    return -1;
  }

  return ret;
}

int adau1787_init(void)
{
  LOG_INF("Initializing audio codec...");
  int ret = 0;

  ret = adau1787_config_gpios();
  ERR_CHK_MSG(ret, "Failed to config ADAU1787 GPIOs");
  ret = adau1787_config_i2c();
  ERR_CHK_MSG(ret, "Failed to config ADAU1787 I2C");
  ret = adau1787_power_up();
  ERR_CHK_MSG(ret, "Failed to power up ADAU1787");
  k_msleep(100);

  default_download_IC_1_Sigma();
  default_download_IC_1_Fast();
  ERR_CHK_MSG(adau_init_error, "Failed to program ADAU1787 codec");

  adau1787_log_status_2();

  LOG_INF("Audio codec initialization done.");
  return 0;
}

// Write operations

int adau1787_write(sub_addr_t start_addr, uint8_t* data, size_t data_len)
{
  int ret;
  uint8_t buf[2 + data_len];
  split_addr(start_addr, buf);

  for (size_t i = 0; i < data_len; i++) {
    buf[2 + i] = data[i];
  }

  ret = i2c_write_dt(&adau1787_i2c, buf, sizeof(buf));
  if (ret != 0) {
    LOG_ERR("I2C write failed: addr=0x%X reg=0x%X", adau1787_i2c.addr, buf[0]);
    adau_init_error = ret;
    return ret;
  }
  return ret;
}

int adau1787_write_register(sub_addr_t reg_addr, reg_word_t* data)
{
  if (!IS_REG_ADDR(reg_addr)) {
    LOG_ERR("Invalid register address: 0x%04X", reg_addr);
    return -1;
  }
  return adau1787_write(reg_addr, data, ADAU1787_CTRL_REG_WIDTH_BYTES);
}

int adau1787_safeload_write(sub_addr_t target_addr, uint8_t* data, size_t num_words)
{
  int ret = 0;

  if (num_words > 5) {
    LOG_ERR("Too many words. Safeload supports a maximum of 5.");
    return -1;
  }

  ret = adau1787_write(SAFELOAD_DATA_1, data, sizeof(data));
  if (ret != 0) {
    LOG_ERR("Failed to write Safeload Data.");
    return ret;
  }

  uint8_t target_addr_buf[4] = { 0x00, 0x00, 0x00, 0x00 };
  split_addr(target_addr, target_addr_buf);

  ret = adau1787_write(SAFELOAD_TARGET_ADDR, target_addr_buf, sizeof(target_addr_buf));
  if (ret != 0) {
    LOG_ERR("Failed to write Safeload Target Address.");
    return ret;
  }

  uint8_t num_words_buf[4] = { num_words, 0x00, 0x00, 0x00 };

  k_usleep(21);

  ret = adau1787_write(SAFELOAD_NUM_WORDS, num_words_buf, sizeof(num_words_buf));
  if (ret != 0) {
    LOG_ERR("Failed to write Safeload Num Words.");
    return ret;
  }

  return ret;
}

// Read operations

int adau1787_read(sub_addr_t start_addr, uint8_t* value, size_t len)
{
  int ret = 0;
  uint8_t addr_buf[2];
  split_addr(start_addr, addr_buf);

  ret = i2c_write_read_dt(&adau1787_i2c, addr_buf, sizeof(addr_buf), value, len);
  if (ret < 0) {
    LOG_ERR("Failed to read from 0x%04X", start_addr);
  }
  return ret;
}

int adau1787_read_register(sub_addr_t reg_addr, reg_word_t* value)
{
  if (!IS_REG_ADDR(reg_addr)) {
    LOG_ERR("Invalid register read: 0x%04X", reg_addr);
    return -1;
  }
  return adau1787_read(reg_addr, value, ADAU1787_CTRL_REG_WIDTH_BYTES);
}

// Conversions
void split_addr(uint16_t word, uint8_t* byte)
{
  byte[0] = (word >> 8) & 0xFF; // High byte
  byte[1] = word & 0xFF; // Low byte
}
