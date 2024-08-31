#ifndef ADAU1701_H
#define ADAU1701_H

#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the ADAU1701 DSP.
 *
 * This function initializes the ADAU1701 DSP by configuring the I2C bus
 * and downloading the default SigmaStudio program generated for the DSP.
 *
 * @return 0 on success, negative error code otherwise.
 */
int adau1701_init(void);

/**
 * @brief Write to a specific register on the ADAU1701.
 *
 * This function writes a value to a specific register on the ADAU1701 using I2C.
 *
 * @param reg_addr The register address to write to.
 * @param value The value to write to the register.
 * @return 0 on success, negative error code otherwise.
 */
int adau1701_write_register(uint16_t reg_addr, uint8_t value);

/**
 * @brief Read a value from a specific register on the ADAU1701.
 *
 * This function reads a value from a specific register on the ADAU1701 using I2C.
 *
 * @param reg_addr The register address to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, negative error code otherwise.
 */
int adau1701_read_register(uint16_t reg_addr, uint8_t* value);

/**
 * @brief Write a block of data to the ADAU1701.
 *
 * This function writes a block of data starting from a specific register address on the ADAU1701.
 *
 * @param start_addr The starting register address for the write operation.
 * @param data Pointer to the data buffer to write.
 * @param len Number of bytes to write.
 * @return 0 on success, negative error code otherwise.
 */
int adau1701_write_block(uint16_t start_addr, const uint8_t* data, size_t len);

/**
 * @brief Read a block of data from the ADAU1701.
 *
 * This function reads a block of data starting from a specific register address on the ADAU1701.
 *
 * @param start_addr The starting register address for the read operation.
 * @param data Pointer to the buffer to store the read data.
 * @param len Number of bytes to read.
 * @return 0 on success, negative error code otherwise.
 */
int adau1701_read_block(uint16_t start_addr, uint8_t* data, size_t len);

/**
 * @brief Perform a soft reset of the ADAU1701 DSP.
 *
 * This function performs a soft reset by writing to the appropriate control register on the ADAU1701.
 *
 * @return 0 on success, negative error code otherwise.
 */
int adau1701_soft_reset(void);

/**
 * @brief Power down the ADAU1701 DSP.
 *
 * This function powers down the ADAU1701 by setting the appropriate bit in the power control register.
 *
 * @return 0 on success, negative error code otherwise.
 */
int adau1701_power_down(void);

/**
 * @brief Sets the volume on the ADAU1701.
 *
 * @param volume Volume level to be set.
 */
void adau1701_set_volume(uint8_t volume);

/**
 * @brief Mutes or unmutes the ADAU1701.
 *
 * @param mute If true, mutes the ADAU1701; if false, unmutes it.
 */
void adau1701_mute(bool mute);

#ifdef __cplusplus
}
#endif

#endif /* ADAU1701_H */