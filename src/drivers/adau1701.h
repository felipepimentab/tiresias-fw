#ifndef ADAU1701_DRIVER_H
#define ADAU1701_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "nrf_drv_twi.h"

#define ADAU1701_I2C_ADDRESS 0x34 // I2C address for ADAU1701

// TWI instance for ADAU1701
extern nrf_drv_twi_t m_twi;

// Function prototypes

/**
 * @brief Initializes the I2C interface.
 */
void i2c_init(void);

/**
 * @brief Writes data to an ADAU1701 register.
 *
 * @param reg_address The register address to write to.
 * @param data Pointer to the data to be written.
 * @param length Length of the data to be written.
 * @return ret_code_t The result of the I2C write operation.
 */
ret_code_t adau1701_write_register(uint8_t reg_address, uint8_t* data, size_t length);

/**
 * @brief Reads data from an ADAU1701 register.
 *
 * @param reg_address The register address to read from.
 * @param data Pointer to the buffer where the read data will be stored.
 * @param length Length of the data to be read.
 * @return ret_code_t The result of the I2C read operation.
 */
ret_code_t adau1701_read_register(uint8_t reg_address, uint8_t* data, size_t length);

/**
 * @brief Initializes the ADAU1701 codec.
 */
void adau1701_init(void);

/**
 * @brief Loads a DSP program into the ADAU1701.
 *
 * @param program_data Pointer to the program data.
 * @param program_size Size of the program data.
 */
void adau1701_load_program(uint8_t* program_data, size_t program_size);

/**
 * @brief Loads DSP parameters into the ADAU1701.
 *
 * @param param_data Pointer to the parameter data.
 * @param param_size Size of the parameter data.
 */
void adau1701_load_parameters(uint8_t* param_data, size_t param_size);

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

#endif // ADAU1701_DRIVER_H