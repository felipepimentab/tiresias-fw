/**
 * @file adau1787.h
 * @brief ADAU1787 DSP Driver Interface
 *
 * This header file defines the interface for controlling the ADAU1787
 * digital signal processor (DSP) using I2C communication in the Zephyr RTOS.
 * It includes declarations for initialization, register read/write, safeload
 * parameter updates, soft reset, power down, and volume control.
 *
 * The ADAU1787 works with the SigmaStudio framework and can be customized
 * based on the application.
 *
 * @note The driver assumes ADAU1787 is configured for I2C communication.
 * Ensure the correct I2C address is set according to the hardware configuration.
 */

#ifndef ADAU1787_H
#define ADAU1787_H

#include "adau_1787_IC_1_SIGMA_REG.h"
#include <stddef.h>
#include <stdint.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

#define SAFELOAD_DATA_REGISTER_1 0x0001 /** @brief Register address for Safeload Data 1 */
#define SAFELOAD_DATA_REGISTER_2 0x0002 /** @brief Register address for Safeload Data 2 */
#define SAFELOAD_DATA_REGISTER_3 0x0003 /** @brief Register address for Safeload Data 3 */
#define SAFELOAD_DATA_REGISTER_4 0x0004 /** @brief Register address for Safeload Data 4 */
#define SAFELOAD_DATA_REGISTER_5 0x0005 /** @brief Register address for Safeload Data 5 */
#define SAFELOAD_TARGET_ADDRESS_REGISTER                                                                               \
  0x0006 /** @brief Register address for Safeload target address (offset of -1) */
#define SAFELOAD_NUMBER_OF_WORDS_REGISTER                                                                              \
  0x0007 /** @brief Register address for Number of words to write/safeload trigger */

#define CORE_CONTROL_REGISTER_ADDRESS REG_COREREGISTER_IC_1_ADDR
#define IST_BIT 0x0020 /** @brief Initiate Safeload Transfer (D5) */
#define ADM_BIT 0x0010 /** @brief Mute ADCs (D4) */
#define DAM_BIT 0x0008 /** @brief Mute DACs (D3) */
#define CR_BIT 0x0004 /** @brief Clear internal register to 0 (D2) */

#define ADAU1787_CTRL_REG_WIDTH_BYTES 2 /** @brief Control Registers' width (16 bits) */
#define ADAU1787_PARAM_RAM_WIDTH_BYTES 4 /** @brief Parameter RAM width (32 bits) */
#define ADAU1787_PROG_RAM_WIDTH_BYTES 5 /** @brief Program RAM width (40 bits) */
#define ADAU1787_DATA_RAM_WIDTH_BYTES 4
typedef uint8_t reg_word_t[ADAU1787_CTRL_REG_WIDTH_BYTES]; /** @brief Register word (16 bits) */
typedef uint8_t param_word_t[ADAU1787_PARAM_RAM_WIDTH_BYTES]; /** @brief Parameter word (32 bits) */
typedef uint8_t prog_word_t[ADAU1787_PROG_RAM_WIDTH_BYTES]; /** @brief Program word (40 bits) */
typedef uint16_t sub_addr_t; /** @brief Internal address/sub-address (16 bits) */
#define NULL_2_BYTES { 0x00, 0x00 }
#define NULL_4_BYTES { 0x00, 0x00, 0x00, 0x00 }
#define NULL_5_BYTES { 0x00, 0x00, 0x00, 0x00, 0x00 }

/**
 * @brief Initialize the ADAU1787 DSP.
 *
 * This function initializes the ADAU1787 DSP by powering it up and
 * setting it up for I2C communication.
 *
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_init(void);

/**
 * @brief Write data to a register in the ADAU1787.
 *
 * @param sub_addr The internal address to write to.
 * @param data Pointer to the data buffer.
 * @param data_len Length of the data buffer.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_write(sub_addr_t sub_addr, uint8_t* data, size_t data_len);

/**
 * @brief Write a block of data to the ADAU1787 starting at a given address.
 *
 * @param start_addr The starting address.
 * @param data Pointer to the data block.
 * @param len Length of the data block.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_block_write(sub_addr_t start_addr, uint8_t* data, size_t len);

/**
 * @brief Write to a specific register in the ADAU1787.
 *
 * @param reg_addr The register address.
 * @param data Data to write (2 bytes).
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_write_register(sub_addr_t reg_addr, reg_word_t data);

/**
 * @brief Perform a safeload write to the ADAU1787.
 *
 * Safeload allows for safely writing parameters to the ADAU1787 without
 * causing audio artifacts.
 *
 * @param param_addrs Array of parameter addresses to write to.
 * @param data Array of data words to write.
 * @param num_registers Number of registers to write.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_safeload_write(sub_addr_t* param_addrs, prog_word_t* data, uint8_t num_registers);

/**
 * @brief Read a register from the ADAU1787.
 *
 * @param reg_addr The register address to read from.
 * @param value Pointer to buffer to store the read data.
 * @param len Length of the data to read.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_read_register(sub_addr_t reg_addr, reg_word_t* value, size_t len);

/**
 * @brief Mute or unmute the ADAU1787.
 *
 * This function mutes or unmutes both ADCs and DACs.
 *
 * @param mute True to mute, false to unmute.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_mute(bool mute);

/**
 * @brief Convert a 16-bit address into two bytes.
 *
 * @param word 16-bit word to split.
 * @param byte Pointer to the byte array to store the result.
 */
void split_addr(uint16_t word, reg_word_t* byte);

#endif // ADAU1787_H