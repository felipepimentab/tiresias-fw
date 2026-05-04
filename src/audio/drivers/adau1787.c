#include "adau1787.h"
#include "SigmaStudioFW.h"
#include "adau_1787_IC_1_FAST.h"
#include "adau_1787_IC_1_FAST_PARAM.h"
#include "adau_1787_IC_1_FAST_REG.h"
#include "adau_1787_IC_1_SIGMA.h"
#include "adau_1787_IC_1_SIGMA_PARAM.h"
#include "adau_1787_IC_1_SIGMA_REG.h"
#include "macros_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(adau1787_driver, LOG_LEVEL_INF);

/** @brief I2C Device Tree Specification for ADAU1787 */
#define ADAU_I2C_NODE DT_NODELABEL(adau_1787)

/** @brief I2C device configuration structure for ADAU1787 */
const struct i2c_dt_spec adau1787_i2c = I2C_DT_SPEC_GET(ADAU_I2C_NODE);

static int adau_init_error = 0;

int adau1787_init(void)
{
  LOG_INF("Initializing audio codec...");

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

  default_download_IC_1_Sigma();
  ERR_CHK_MSG(adau_init_error, "Failed to program ADAU1787 codec");

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