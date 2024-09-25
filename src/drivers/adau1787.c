/**
 * @file adau1787.c
 * @brief ADAU1787 DSP Driver Implementation
 *
 * This file contains the implementation of functions for controlling the ADAU1787
 * digital signal processor (DSP) using I2C communication via the Zephyr RTOS.
 * The functions provided include initialization, register read/write operations,
 * safeload parameter updates, soft reset, power down, and volume control.
 *
 * The driver is intended to work with the SigmaStudio framework and can be
 * customized based on the specific needs of the application.
 *
 * @note The driver assumes ADAU1787 is configured for I2C communication.
 * Ensure the correct I2C address is set according to the hardware configuration
 * on the device tree overlay file.
 */

#include "adau1787.h"
#include "SigmaStudioFW.h"
#include "adau_1787_IC_1_FAST.h"
#include "adau_1787_IC_1_FAST_PARAM.h"
#include "adau_1787_IC_1_FAST_REG.h"
#include "adau_1787_IC_1_SIGMA.h"
#include "adau_1787_IC_1_SIGMA_PARAM.h"
#include "adau_1787_IC_1_SIGMA_REG.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

/** @brief I2C Device Tree Specification for ADAU1787 */
#define I2C_NODE DT_NODELABEL(adau_1787)

/** @brief Enable passthrough mode for debug */
#define SSTUDIO_DEFAULT_DOWNLOAD true

/** @brief Maximum size for I2C buffer */
#define I2C_MAX_BUF_SIZE 16

/** @brief I2C device configuration structure for ADAU1787 */
const struct i2c_dt_spec adau1787_i2c = I2C_DT_SPEC_GET(I2C_NODE);

int adau1787_init(void)
{
  printk("Starting ADAU1787...\n");
  const struct device* i2c_dev;
  i2c_dev = device_get_binding("I2C_1");
  if (!i2c_dev) {
    printk("I2C binding failed.");
    return -1;
  }

  i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_FAST_PLUS));
  printk("SCL pin: %d \n", NRF_TWIM1->PSEL.SCL);
  printk("SDA pin: %d \n", NRF_TWIM1->PSEL.SDA);
  printk("I2C frequency: %d \n", NRF_TWIM1->FREQUENCY);

  if (!device_is_ready(adau1787_i2c.bus)) {
    printk("I2C bus %s is not ready!\n\r", adau1787_i2c.bus->name);
    return -1;
  }

  default_download_IC_1_Sigma();

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
    printk("Failed to write to I2C device address %x at reg. %x \n\r", adau1787_i2c.addr, buf[0]);
    return ret;
  }
  return ret;
}

int adau1787_write_register(sub_addr_t reg_addr, reg_word_t* data)
{
  if (!IS_REG_ADDR(reg_addr)) {
    printk("Address 0x%02x is not a valid register address (0xC000 to 0xC0E1)\n\r", reg_addr);
    return -1;
  }
  return adau1787_write(reg_addr, data, ADAU1787_CTRL_REG_WIDTH_BYTES);
}

int adau1787_safeload_write(sub_addr_t* param_addrs, prog_word_t* data, uint8_t num_registers)
{
  int ret = 0;

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
    printk("Failed to read from ADAU1787 starting at address 0x%02x\n\r", start_addr);
  }
  return ret;
}

int adau1787_read_register(sub_addr_t reg_addr, reg_word_t* value)
{
  if (!IS_REG_ADDR(reg_addr)) {
    printk("Address 0x%02x is not a valid register address (0xC000 to 0xC0E1)\n\r", reg_addr);
    return -1;
  }
  return adau1787_read(reg_addr, value, ADAU1787_CTRL_REG_WIDTH_BYTES);
}

// Other

int adau1787_mute(bool mute)
{
  int ret = 0;
  return ret;
}

// Conversions
void split_addr(uint16_t word, uint8_t* byte)
{
  byte[0] = (word >> 8) & 0xFF; // High byte
  byte[1] = word & 0xFF; // Low byte
  return;
}