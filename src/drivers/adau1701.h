/**
 * @file adau1701.h
 * @brief Header file for ADAU1701 DSP Driver
 *
 * This header file defines the interface for controlling the ADAU1701
 * digital signal processor (DSP) using I2C communication via the Zephyr RTOS.
 * The provided functions allow for initialization, register read/write operations,
 * safeload parameter updates, soft reset, power down, and volume control.
 *
 * @note Ensure the ADAU1701 is configured correctly for I2C communication,
 *       and the appropriate I2C address is used.
 */

#ifndef ADAU1701_H
#define ADAU1701_H

#include <stdbool.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/types.h>

/** @brief Initializes the ADAU1701 DSP.
 *
 * This function initializes the ADAU1701 DSP by checking if the I2C bus is ready.
 * It optionally downloads the SigmaStudio program to configure the DSP.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_init(void);

/** @brief Writes a single byte to an ADAU1701 register.
 *
 * This function writes a single byte value to the specified register address of the ADAU1701 DSP.
 *
 * @param addr  The register address to write to.
 * @param value The byte value to write to the register.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_direct_write_byte(uint16_t addr, uint8_t value);

/** @brief Writes a block of data to consecutive ADAU1701 registers.
 *
 * This function writes a block of data starting from the specified register address
 * of the ADAU1701 DSP. The data is written using I2C burst mode.
 *
 * @param start_addr The starting register address.
 * @param data       A pointer to the data block to be written.
 * @param len        The length of the data block to be written.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_direct_write_block(uint16_t start_addr, const uint8_t* data, size_t len);

/** @brief Writes a single value to a safeload parameter register.
 *
 * This function writes a single value to a safeload parameter register on the ADAU1701 DSP.
 * Safeloading is used to safely update DSP parameters during runtime without causing
 * audio glitches.
 *
 * @param param_addr The safeload parameter register address.
 * @param value      The value to write to the safeload parameter register.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_safeload_write(uint16_t param_addr, const uint8_t value);

/** @brief Reads a single byte from an ADAU1701 register.
 *
 * This function reads a single byte value from the specified register address
 * of the ADAU1701 DSP.
 *
 * @param addr  The register address to read from.
 * @param value A pointer to the byte variable to store the read value.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_read_byte(uint16_t addr, uint8_t* value);

/** @brief Reads a block of data from consecutive ADAU1701 registers.
 *
 * This function reads a block of data starting from the specified register address
 * of the ADAU1701 DSP. The data is read using I2C burst mode.
 *
 * @param start_addr The starting register address.
 * @param data       A pointer to the buffer to store the read data.
 * @param len        The length of the data block to be read.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_read_block(uint16_t start_addr, uint8_t* data, size_t len);

/** @brief Performs a soft reset on the ADAU1701 DSP.
 *
 * This function performs a soft reset on the ADAU1701 DSP by writing to the
 * CORE register. The reset mutes the DAC and ADC channels.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_soft_reset(void);

/** @brief Powers down the ADAU1701 DSP.
 *
 * This function powers down the ADAU1701 DSP by writing to the analog power down register.
 * It turns off the DACs and ADCs, placing the DSP in a low-power state.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int adau1701_power_down(void);

/** @brief Sets the volume level on the ADAU1701 DSP.
 *
 * This function sets the volume level on the ADAU1701 DSP by writing to the
 * appropriate volume control register.
 *
 * @param volume The volume level to set (typically 0-255).
 */
void adau1701_set_volume(uint8_t volume);

/** @brief Mutes or unmutes the ADAU1701 DSP.
 *
 * This function mutes or unmutes the ADAU1701 DSP by writing to the appropriate
 * register. Muting is achieved by setting the MUTE bits in the CORE register.
 *
 * @param mute Set to `true` to mute, or `false` to unmute.
 */
void adau1701_mute(bool mute);

/** @brief Sets the volume with a fade effect on the ADAU1701 DSP.
 *
 * This function smoothly changes the volume level from the current level to the target
 * level over a specified number of steps, creating a fade effect.
 *
 * @param target_volume The target volume level to reach.
 * @param fade_steps    The number of steps to use for the fade effect.
 */
void adau1701_set_volume_fade(uint8_t target_volume, uint16_t fade_steps);

/** @brief Configures the ADAU1701 DSP for I2S or TDM mode.
 *
 * This function configures the ADAU1701 DSP for either I2S or TDM mode by writing
 * to the appropriate serial output register.
 *
 * @param mode The mode to set (e.g., I2S or TDM configuration value).
 */
void adau1701_configure_i2s_tdm(uint8_t mode);

#endif /* ADAU1701_H */