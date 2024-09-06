/**
 * @file adau1701.h
 * @brief ADAU1701 DSP Driver Interface
 *
 * This header file defines the interface for controlling the ADAU1701
 * digital signal processor (DSP) using I2C communication in the Zephyr RTOS.
 * It includes declarations for initialization, register read/write, safeload
 * parameter updates, soft reset, power down, and volume control.
 *
 * The ADAU1701 works with the SigmaStudio framework and can be customized
 * based on the application.
 *
 * @note The driver assumes ADAU1701 is configured for I2C communication.
 * Ensure the correct I2C address is set according to the hardware configuration.
 */

#ifndef ADAU1701_H
#define ADAU1701_H

#include "adau_1701_IC_1_REG.h"
#include <stddef.h>
#include <stdint.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

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

#define CORE_CONTROL_REGISTER_ADDRESS REG_COREREGISTER_IC_1_ADDR
#define IST_BIT 0x0020 /** @brief Initiate Safeload Transfer (D5) */
#define ADM_BIT 0x0010 /** @brief Mute ADCs (D4) */
#define DAM_BIT 0x0008 /** @brief Mute DACs (D3) */
#define CR_BIT 0x0004 /** @brief Clear internal register to 0 (D2) */

#define ADAU1701_CTRL_REG_WIDTH_BYTES 2 /** @brief Control Registers' width (16 bits) */
#define ADAU1701_PARAM_RAM_WIDTH_BYTES 4 /** @brief Parameter RAM width (32 bits) */
#define ADAU1701_DATA_RAM_WIDTH_BYTES 5 /** @brief Program RAM width (40 bits) */
typedef uint8_t reg_word_t[ADAU1701_CTRL_REG_WIDTH_BYTES]; /** @brief Register word (16 bits) */
typedef uint8_t param_word_t[ADAU1701_PARAM_RAM_WIDTH_BYTES]; /** @brief Parameter word (32 bits) */
typedef uint8_t data_word_t[ADAU1701_DATA_RAM_WIDTH_BYTES]; /** @brief Program word (40 bits) */
typedef uint16_t sub_addr_t; /** @brief Internal address/sub-address (16 bits) */
#define NULL_2_BYTES { 0x00, 0x00 }
#define NULL_4_BYTES { 0x00, 0x00, 0x00, 0x00 }
#define NULL_5_BYTES { 0x00, 0x00, 0x00, 0x00, 0x00 }

/**
 * @brief Initialize the ADAU1701 DSP.
 *
 * This function initializes the ADAU1701 DSP by powering it up and
 * setting it up for I2C communication.
 *
 * @return 0 if successful, negative error code otherwise.
 */
int adau1701_init(void);

/**
 * @brief Write data to a register in the ADAU1701.
 *
 * @param sub_addr The internal address to write to.
 * @param data Pointer to the data buffer.
 * @param data_len Length of the data buffer.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1701_write(sub_addr_t sub_addr, uint8_t* data, size_t data_len);

/**
 * @brief Write a block of data to the ADAU1701 starting at a given address.
 *
 * @param start_addr The starting address.
 * @param data Pointer to the data block.
 * @param len Length of the data block.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1701_block_write(sub_addr_t start_addr, uint8_t* data, size_t len);

/**
 * @brief Write to a specific register in the ADAU1701.
 *
 * @param reg_addr The register address.
 * @param data Data to write (2 bytes).
 * @return 0 if successful, negative error code otherwise.
 */
int adau1701_write_register(sub_addr_t reg_addr, reg_word_t data);

/**
 * @brief Perform a safeload write to the ADAU1701.
 *
 * Safeload allows for safely writing parameters to the ADAU1701 without
 * causing audio artifacts.
 *
 * @param param_addrs Array of parameter addresses to write to.
 * @param data Array of data words to write.
 * @param num_registers Number of registers to write.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1701_safeload_write(sub_addr_t* param_addrs, data_word_t* data, uint8_t num_registers);

/**
 * @brief Read a register from the ADAU1701.
 *
 * @param reg_addr The register address to read from.
 * @param value Pointer to buffer to store the read data.
 * @param len Length of the data to read.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1701_read_register(sub_addr_t reg_addr, reg_word_t* value, size_t len);

/**
 * @brief Mute or unmute the ADAU1701.
 *
 * This function mutes or unmutes both ADCs and DACs.
 *
 * @param mute True to mute, false to unmute.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1701_mute(bool mute);

/**
 * @brief Convert a 16-bit address into two bytes.
 *
 * @param word 16-bit word to split.
 * @param byte Pointer to the byte array to store the result.
 */
void split_addr(uint16_t word, reg_word_t* byte);

#endif // ADAU1701_H