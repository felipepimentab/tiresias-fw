/**
 * @file adau1701.c
 * @brief ADAU1701 DSP Driver Implementation
 *
 * This file contains the implementation of functions for controlling the ADAU1701
 * digital signal processor (DSP) using I2C communication via the Zephyr RTOS.
 * The functions provided include initialization, register read/write operations,
 * safeload parameter updates, soft reset, power down, and volume control.
 *
 * The driver is intended to work with the SigmaStudio framework and can be
 * customized based on the specific needs of the application.
 *
 * @note This driver assumes the ADAU1701 is configured for I2C communication.
 * Ensure the correct I2C address is set according to the hardware configuration.
 */

#include "adau1701.h"
#include "SigmaStudioFW.h"
#include "adau_1701_IC_1.h"
#include "adau_1701_IC_1_PARAM.h"
#include "adau_1701_IC_1_REG.h"
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

/** @brief I2C Device Tree Specification for ADAU1701 */
#define I2C_NODE DT_NODELABEL(adau_1701)

/** @brief Enable passthrough mode for debug */
#define DEBUG_PASSTHROUGH true

/** @brief I2C device configuration structure */
const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

int adau1701_init(void)
{
  if (!device_is_ready(dev_i2c.bus)) {
    printk("I2C bus %s is not ready!\n\r", dev_i2c.bus->name);
    return -1;
  }

#if DEBUG_PASSTHROUGH
  //
#else
  // Use the default SigmaStudio program for initialization
  default_download_IC_1();
#endif

  return 0;
}

/*
 * Write operations
 */

int adau1701_direct_write_byte(uint16_t addr, uint8_t value)
{
  int ret = i2c_reg_write_byte_dt(&dev_i2c, addr, value);
  if (ret < 0) {
    printk("Failed to write to ADAU1701 register 0x%02x\n\r", addr);
  }
  return ret;
}

int adau1701_direct_write_block(uint16_t start_addr, const uint8_t* data, size_t len)
{
  int ret = i2c_burst_write_dt(&dev_i2c, start_addr, data, len);
  if (ret < 0) {
    printk("Failed to write block to ADAU1701 starting at register 0x%02x\n\r", start_addr);
  }
  return ret;
}

int adau1701_safeload_write(uint16_t param_addr, const uint8_t value)
{
  if (param_addr > 1023) {
    printk("Address %02x is outside of range for Parameter RAM (0x0000 to 0x03FF)\n\r", param_addr);
    return -1;
  }
  // Safeload logic
  int ret = adau1701_direct_write_byte(param_addr, value);
  // Safeload logic
  return ret;
}

/*
 * Read operations
 */

int adau1701_read_byte(uint16_t addr, uint8_t* value)
{
  int ret = i2c_reg_read_byte_dt(&dev_i2c, addr, value);
  if (ret < 0) {
    printk("Failed to read from ADAU1701 register 0x%02x\n\r", addr);
  }
  return ret;
}

int adau1701_read_block(uint16_t start_addr, uint8_t* data, size_t len)
{
  int ret = i2c_burst_read_dt(&dev_i2c, start_addr, data, len);
  if (ret < 0) {
    printk("Failed to read block from ADAU1701 starting at register 0x%02x\n\r", start_addr);
  }
  return ret;
}

/*
 * Other
 */

int adau1701_soft_reset(void)
{
  // Using the CORE register to trigger a soft reset
  uint8_t reset_value = R13_MUTE_DAC_IC_1_MASK | R13_MUTE_ADC_IC_1_MASK;
  int ret = adau1701_direct_write_byte(REG_COREREGISTER_IC_1_ADDR, reset_value);
  if (ret < 0) {
    printk("Failed to reset ADAU1701\n\r");
  }
  return ret;
}

int adau1701_power_down(void)
{
  uint8_t power_down_value = R19_DAC3_POWERDOWN_IC_1_MASK | R19_DAC2_POWERDOWN_IC_1_MASK | R19_DAC1_POWERDOWN_IC_1_MASK
      | R19_DAC0_POWERDOWN_IC_1_MASK | R19_ADCS_POWERDOWN_IC_1_MASK;
  int ret = adau1701_direct_write_byte(REG_ANALOGPOWERDOWNREGISTER_IC_1_ADDR, power_down_value);
  if (ret < 0) {
    printk("Failed to power down ADAU1701\n\r");
  }
  return ret;
}

void adau1701_set_volume(uint8_t volume)
{
  uint16_t volume_register_addr = REG_ANALOGINTERFACEREGISTER1_IC_1_ADDR;
  adau1701_direct_write_byte(volume_register_addr, volume);
}

void adau1701_mute(bool mute)
{
  uint8_t mute_value = mute ? (R13_MUTE_DAC_IC_1_MASK | R13_MUTE_ADC_IC_1_MASK) : 0x00;
  uint16_t mute_register_addr = REG_COREREGISTER_IC_1_ADDR;
  adau1701_direct_write_byte(mute_register_addr, mute_value);
}

void adau1701_set_volume_fade(uint8_t target_volume, uint16_t fade_steps)
{
  uint8_t current_volume;
  adau1701_read_register(REG_ANALOGINTERFACEREGISTER1_IC_1_ADDR, &current_volume);
  int step = (target_volume > current_volume) ? 1 : -1;

  for (uint16_t i = 0; i < fade_steps; i++) {
    current_volume += step;
    adau1701_set_volume(current_volume);
    k_msleep(10); // Delay between steps for fading effect
  }

  adau1701_set_volume(target_volume);
}

void adau1701_configure_i2s_tdm(uint8_t mode) { adau1701_direct_write_byte(REG_SERIALOUTREGISTER1_IC_1_ADDR, mode); }