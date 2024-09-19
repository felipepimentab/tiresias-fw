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
 * @note This driver assumes the ADAU1787 is configured for I2C communication.
 * Ensure the correct I2C address is set according to the hardware configuration.
 */

#include "adau1787.h"
#include "SigmaStudioFW.h"
#include "adau_1787_IC_1_FAST.h"
#include "adau_1787_IC_1_FAST_PARAM.h"
#include "adau_1787_IC_1_FAST_REG.h"
#include "adau_1787_IC_1_SIGMA.h"
#include "adau_1787_IC_1_SIGMA_PARAM.h"
#include "adau_1787_IC_1_SIGMA_REG.h"
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

/** @brief I2C Device Tree Specification for ADAU1787 */
#define I2C_NODE DT_NODELABEL(adau_1787)

/** @brief Enable passthrough mode for debug */
#define DEBUG_PASSTHROUGH true

/** @brief I2C device configuration structure for ADAU1787 */
const struct i2c_dt_spec adau1787_i2c = I2C_DT_SPEC_GET(I2C_NODE);

int adau1787_init(void)
{
  k_msleep(200); // Wait for power-up

  if (!device_is_ready(adau1787_i2c.bus)) {
    printk("I2C bus %s is not ready!\n\r", adau1787_i2c.bus->name);
    return -1;
  }

#if DEBUG_PASSTHROUGH
  adau1787_mute(false);
#else
  // Use the default SigmaStudio program for initialization
  default_download_IC_1();
#endif

  return 0;
}

// Write operations

int adau1787_write(sub_addr_t sub_addr, uint8_t* data, size_t data_len)
{
  int ret;
  uint8_t buf[2 + data_len]; // Buffer to hold the address (2 bytes) and the data

  // Split the 16-bit address into two bytes
  split_addr(sub_addr, &buf);

  // Copy data to the buffer after the address
  for (size_t i = 0; i < data_len; i++) {
    buf[2 + i] = data[i];
  }

  // Use the Zephyr I2C API to send the buffer over I2C
  ret = i2c_write_dt(&adau1787_i2c, buf, sizeof(buf));
  if (ret != 0) {
    printk("Failed to write to I2C device address %x at reg. %x \n\r", adau1787_i2c.addr, buf[0]);
    return ret;
  }
  return ret;
}

int adau1787_block_write(sub_addr_t start_addr, uint8_t* data, size_t len)
{
  int ret;
  uint8_t addr_buf[2];
  struct i2c_msg msg[2];
  // Split the 16-bit start address into two bytes
  split_addr(start_addr, &addr_buf);
  // First message: send the 16-bit address
  msg[0].buf = addr_buf;
  msg[0].len = 2; // 16-bit address means 2 bytes
  msg[0].flags = I2C_MSG_WRITE;
  // Second message: send the data block
  msg[1].buf = data;
  msg[1].len = len;
  msg[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;
  // Perform the I2C transfer: write the address and data in two messages
  ret = i2c_transfer_dt(&adau1787_i2c, msg, 2);
  if (ret < 0) {
    printk("Failed to write block to ADAU1787 starting at register 0x%02x\n\r", start_addr);
  }
  return ret;
}

int adau1787_write_register(sub_addr_t reg_addr, reg_word_t data)
{
  return adau1787_write(reg_addr, data, ADAU1787_CTRL_REG_WIDTH_BYTES);
}

int adau1787_safeload_write(sub_addr_t* param_addrs, prog_word_t* data, uint8_t num_registers)
{
  int ret;
  if (num_registers > 5) {
    return -1; // Error: Too many registers
  }
  for (uint8_t i = 0; i < num_registers; i++) {
    reg_word_t addr_data;
    // Split the 16-bit address into two bytes
    split_addr(param_addrs[i], &addr_data);
    // Write the parameter RAM address to the safeload address register
    ret = adau1787_write_register(SAFELOAD_ADDRESS_REGISTER_0 + i, addr_data);
    if (ret < 0) {
      return ret;
    }
    // Write the 28-bit data to the corresponding safeload data register
    ret = adau1787_write(SAFELOAD_DATA_REGISTER_0 + i, data[i], ADAU1787_PROG_RAM_WIDTH_BYTES);
    if (ret < 0) {
      return ret;
    }
  }
  // Set the initiate safeload transfer bit in the core control register
  reg_word_t core_control_value;
  ret = adau1787_read_register(CORE_CONTROL_REGISTER_ADDRESS, &core_control_value, ADAU1787_CTRL_REG_WIDTH_BYTES);
  if (ret < 0) {
    return ret; // Error reading core control register
  }
  core_control_value[1] |= IST_BIT;
  ret = adau1787_write_register(CORE_CONTROL_REGISTER_ADDRESS, core_control_value);
  if (ret < 0) {
    return ret; // Error writing to core control register
  }
  return ret;
}

// Read operations

int adau1787_read_register(sub_addr_t reg_addr, reg_word_t* value, size_t len)
{
  int ret;
  uint8_t addr_buf[2];
  split_addr(reg_addr, &addr_buf);
  // First, write the 16-bit address, then read the data from that address
  ret = i2c_write_read_dt(&adau1787_i2c, addr_buf, sizeof(addr_buf), value, len); // Read the data into the value buffer
  if (ret < 0) {
    printk("Failed to read from ADAU1787 register 0x%02x\n\r", reg_addr);
  }
  return ret;
}

// Other

int adau1787_mute(bool mute)
{
  int ret;
  reg_word_t core_control_value;
  ret = adau1787_read_register(CORE_CONTROL_REGISTER_ADDRESS, &core_control_value, ADAU1787_CTRL_REG_WIDTH_BYTES);
  if (ret < 0) {
    return ret; // Error reading core control register
  }
  if (mute) {
    core_control_value[1] &= ~(ADM_BIT | DAM_BIT | CR_BIT);
  } else {
    core_control_value[1] |= (ADM_BIT | DAM_BIT | CR_BIT);
  }
  ret = adau1787_write_register(CORE_CONTROL_REGISTER_ADDRESS, core_control_value);
  if (ret < 0) {
    return ret; // Error writing to core control register
  }
  return ret;
}

// Conversions
void split_addr(uint16_t word, reg_word_t* byte)
{
  *byte[0] = (word >> 8) & 0xFF; // High byte
  *byte[1] = word & 0xFF; // Low byte
  return;
}