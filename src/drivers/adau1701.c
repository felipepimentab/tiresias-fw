#include "adau1701.h"
#include "SigmaStudioFW.h"
#include "adau_1701_IC_1.h"
#include "adau_1701_IC_1_PARAM.h"
#include "adau_1701_IC_1_REG.h"
#include <zephyr/drivers/i2c.h>

#define I2C_NODE DT_NODELABEL(adau_1701)

static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

int adau1701_init(void)
{
  if (!device_is_ready(dev_i2c.bus)) {
    printk("I2C bus %s is not ready!\n\r", dev_i2c.bus->name);
    return -1;
  }

  default_download_IC_1();
  return 0;
}

int adau1701_write_register(uint16_t reg_addr, uint8_t value)
{
  int ret = i2c_reg_write_byte_dt(&dev_i2c, reg_addr, value);
  if (ret < 0) {
    printk("Failed to write to ADAU1701 register 0x%02x\n\r", reg_addr);
  }
  return ret;
}

int adau1701_read_register(uint16_t reg_addr, uint8_t* value)
{
  int ret = i2c_reg_read_byte_dt(&dev_i2c, reg_addr, value);
  if (ret < 0) {
    printk("Failed to read from ADAU1701 register 0x%02x\n\r", reg_addr);
  }
  return ret;
}

int adau1701_write_block(uint16_t start_addr, const uint8_t* data, size_t len)
{
  int ret = i2c_burst_write_dt(&dev_i2c, start_addr, data, len);
  if (ret < 0) {
    printk("Failed to write block to ADAU1701 starting at register 0x%02x\n\r", start_addr);
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

int adau1701_soft_reset(void)
{
  uint8_t reset_value = 0x01;
  int ret = adau1701_write_register(CONTROL_REGISTER_ADDR, reset_value);
  if (ret < 0) {
    printk("Failed to reset ADAU1701\n\r");
  }
  return ret;
}

int adau1701_power_down(void)
{
  uint8_t power_down_value = 0x00;
  int ret = adau1701_write_register(POWER_CONTROL_REGISTER_ADDR, power_down_value);
  if (ret < 0) {
    printk("Failed to power down ADAU1701\n\r");
  }
  return ret;
}

void adau1701_set_volume(uint8_t volume);

void adau1701_mute(bool mute);