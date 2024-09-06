/**
 * @file adau1701.h
 * @brief ADAU1701 DSP Driver Header
 *
 * This header file defines the ADAU1701 DSP driver interface using I2C communication. It contains
 * function prototypes for initialization, safeload updates, register read/write operations,
 * soft reset, power down, and volume/mute control of the ADAU1701 digital signal processor (DSP).
 *
 * @note This driver assumes the ADAU1701 is configured for I2C communication.
 */

#ifndef ADAU1701_H
#define ADAU1701_H

#include <stddef.h>
#include <stdint.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

/**
 * @brief ADAU1701 control and data width constants
 */
#define ADAU1701_CTRL_REG_WIDTH_BYTES 2 /**< Control Registers' width (16 bits) */
#define ADAU1701_PARAM_RAM_WIDTH_BYTES 4 /**< Parameter RAM width (32 bits) */
#define ADAU1701_DATA_RAM_WIDTH_BYTES 5 /**< Program RAM width (40 bits) */

/**
 * @brief ADAU1701 safeload and core control register addresses
 */
#define SAFELOAD_DATA_REGISTER_0 2064
#define SAFELOAD_DATA_REGISTER_1 2065
#define SAFELOAD_DATA_REGISTER_2 2066
#define SAFELOAD_DATA_REGISTER_3 2067
#define SAFELOAD_DATA_REGISTER_4 2068
#define SAFELOAD_ADDRESS_REGISTER_0 2069
#define SAFELOAD_ADDRESS_REGISTER_1 2070
#define SAFELOAD_ADDRESS_REGISTER_2 2071
#define SAFELOAD_ADDRESS_REGISTER_3 2072
#define SAFELOAD_ADDRESS_REGISTER_4 2073
#define CORE_CONTROL_REGISTER 2074

/**
 * @brief ADAU1701 control bits for core control register
 */
#define IST_BIT 0x0020 /**< Initiate Safeload Transfer (D5) */
#define ADM_BIT 0x0010 /**< Mute ADCs (D4) */
#define DAM_BIT 0x0008 /**< Mute DACs (D3) */
#define CR_BIT 0x0004 /**< Clear internal register to 0 (D2) */

/**
 * @brief Typedefs for register, parameter, and data words
 */
typedef uint8_t reg_word_t[ADAU1701_CTRL_REG_WIDTH_BYTES]; /**< Register word (16 bits) */
typedef uint8_t param_word_t[ADAU1701_PARAM_RAM_WIDTH_BYTES]; /**< Parameter word (32 bits) */
typedef uint8_t data_word_t[ADAU1701_DATA_RAM_WIDTH_BYTES]; /**< Program word (40 bits) */
typedef uint16_t sub_addr_t; /**< Internal address/sub address (16 bits) */

/**
 * @brief Initialize the ADAU1701 DSP.
 *
 * This function initializes the ADAU1701 DSP, ensures that the I2C device is ready,
 * and optionally loads the default SigmaStudio program if DEBUG_PASSTHROUGH is not enabled.
 *
 * @return 0 on success, negative value on failure.
 */
int adau1701_init(void);

/**
 * @brief Write data to the ADAU1701 DSP.
 *
 * This function writes a block of data to a specific subaddress in the ADAU1701 DSP.
 *
 * @param sub_addr The 16-bit subaddress where the data should be written.
 * @param data Pointer to the data to be written.
 * @param data_len Length of the data to be written in bytes.
 *
 * @return 0 on success, negative value on failure.
 */
int adau1701_write(sub_addr_t sub_addr, uint8_t* data, size_t data_len);

/**
 * @brief Write a block of data to the ADAU1701 DSP.
 *
 * This function writes a block of data to a specific subaddress in burst mode.
 *
 * @param start_addr The 16-bit subaddress to start writing.
 * @param data Pointer to the block of data to be written.
 * @param len Length of the data block in bytes.
 *
 * @return 0 on success, negative value on failure.
 */
int adau1701_block_write(sub_addr_t start_addr, uint8_t* data, size_t len);

/**
 * @brief Write to a register in the ADAU1701 DSP.
 *
 * This function writes a 16-bit value to a specific register in the ADAU1701 DSP.
 *
 * @param reg_addr The 16-bit register address.
 * @param data The 16-bit data to be written.
 *
 * @return 0 on success, negative value on failure.
 */
int adau1701_write_register(sub_addr_t reg_addr, reg_word_t data);

/**
 * @brief Perform a safeload write operation in the ADAU1701 DSP.
 *
 * This function writes multiple 40-bit program words to the parameter RAM using the safeload mechanism.
 *
 * @param param_addrs Array of 16-bit parameter addresses.
 * @param data Array of 40-bit program words to be written.
 * @param num_registers Number of registers to be written (maximum 5).
 *
 * @return 0 on success, negative value on failure.
 */
int adau1701_safeload_write(sub_addr_t* param_addrs, data_word_t* data, uint8_t num_registers);

/**
 * @brief Read data from an ADAU1701 DSP register.
 *
 * This function reads data from a specific register address in the ADAU1701 DSP.
 *
 * @param reg_addr The 16-bit register address to be read.
 * @param value Pointer to the buffer where the read value will be stored.
 * @param len Length of the data to be read in bytes.
 *
 * @return 0 on success, negative value on failure.
 */
int adau1701_read_register(sub_addr_t reg_addr, reg_word_t* value, size_t len);

/**
 * @brief Mute or unmute the ADAU1701 DSP.
 *
 * This function controls the mute functionality of the ADAU1701 by modifying the core control register.
 *
 * @param mute True to mute the DSP, false to unmute.
 *
 * @return 0 on success, negative value on failure.
 */
int adau1701_mute(bool mute);

#endif // ADAU1701_H