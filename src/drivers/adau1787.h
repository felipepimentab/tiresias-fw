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
 * Ensure the correct I2C address is set according to the hardware configuration
 * on the device tree overlay file.
 */

#ifndef ADAU1787_H
#define ADAU1787_H

#include "adau_1787_IC_1_SIGMA_REG.h"
#include <stddef.h>
#include <stdint.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

/** @brief Parameter RAM address for Safeload Data 1 */
#define SAFELOAD_DATA_1 0x0001
/** @brief Parameter RAM address for Safeload Data 2 */
#define SAFELOAD_DATA_2 0x0002
/** @brief Parameter RAM address for Safeload Data 3 */
#define SAFELOAD_DATA_3 0x0003
/** @brief Parameter RAM address for Safeload Data 4 */
#define SAFELOAD_DATA_4 0x0004
/** @brief Parameter RAM address for Safeload Data 5 */
#define SAFELOAD_DATA_5 0x0005
/** @brief Parameter RAM address for Safeload target address (offset of -1) */
#define SAFELOAD_TARGET_ADDR 0x0006
/** @brief Parameter RAM address for Number of words to write/safeload trigger */
#define SAFELOAD_NUM_WORDS 0x0007

/** @brief ADC, DAC, Headphone Power Controls Register Address */
#define ADC_DAC_HP_PWR REG_ADC_DAC_HP_PWR_IC_1_Sigma_ADDR
/** @brief PLL, Microphone Bias, and PGA Power Controls Register Address */
#define PLL_MB_PGA_PWR REG_PLL_MB_PGA_PWR_IC_1_Sigma_ADDR
/** @brief Digital Microphone Power Controls Register Address */
#define DMIC_PWR REG_DMIC_PWR_IC_1_Sigma_ADDR
/** @brief Serial Port, PDM Output, and Digital Microphone CLK Power Controls Register Address */
#define SAI_CLK_PWR REG_SAI_CLK_PWR_IC_1_Sigma_ADDR
/** @brief DSP Power Controls Register Address */
#define DSP_PWR REG_DSP_PWR_IC_1_Sigma_ADDR

/** @brief Control Registers' width (8 bits) */
#define ADAU1787_CTRL_REG_WIDTH_BYTES 1
/** @brief Parameter RAM width (32 bits) */
#define ADAU1787_PARAM_RAM_WIDTH_BYTES 4
/** @brief Program RAM width (40 bits) */
#define ADAU1787_PROG_RAM_WIDTH_BYTES 5
/** @brief Data RAM width (40 bits) */
#define ADAU1787_DATA_RAM_WIDTH_BYTES 4

/** @brief Register word (8 bits) */
typedef uint8_t reg_word_t;
/** @brief Parameter word (32 bits) */
typedef uint8_t param_word_t[ADAU1787_PARAM_RAM_WIDTH_BYTES];
/** @brief Program word (40 bits) */
typedef uint8_t prog_word_t[ADAU1787_PROG_RAM_WIDTH_BYTES];
/** @brief Internal address/sub-address (16 bits) */
typedef uint16_t sub_addr_t;

/** @brief Checks if the given address is within the Parameter RAM range */
#define IS_PARAM_ADDR(addr) ((addr) >= 0x0000 && (addr) <= 0x03FF)
/** @brief Checks if the given address is within the Program RAM range */
#define IS_PROG_ADDR(addr) ((addr) >= 0x0C00 && (addr) <= 0x13FF)
/** @brief Checks if the given address is a register address */
#define IS_REG_ADDR(addr) ((addr) >= 0xC000 && (addr) <= 0xC0E1)

/**
 * @brief Initialize the ADAU1787 DSP.
 *
 * This function initializes the ADAU1787 DSP by starting the I2C bus then loading program, parameters and registers to
 * the DSP through the `default_download_IC_1_Sigma` function generated by SigmaStudio.
 *
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_init(void);

/**
 * @brief Write data to the ADAU1787.
 *
 * Write operations on the ADAU1787 utilize burst mode, where the master device sends an initial start address, and the
 * ADAU1787 automatically increments the internal address for subsequent data writes, eliminating the need for sending
 * individual addresses for each data word.
 *
 * @param start_addr The internal address to start writing to.
 * @param data Pointer to the data buffer.
 * @param data_len Length of the data buffer.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_write(sub_addr_t start_addr, uint8_t* data, size_t data_len);

/**
 * @brief Write to a single register in the ADAU1787.
 *
 * @param reg_addr The register address.
 * @param data Data to write.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_write_register(sub_addr_t reg_addr, reg_word_t* data);

/**
 * @brief Perform a Safeload Write Operation to the ADAU1787 DSP.
 *
 * This function performs a safeload write to the ADAU1787's parameter RAM.
 * The safeload operation consists of writing up to five 4-byte words (32-bit
 * words) into predefined safeload data registers, setting the target
 * address in the parameter RAM, and indicating the number of words to be
 * updated. Once the safeload write is triggered, the DSP core safely
 * transfers the data during the next available audio frame.
 *
 * @param target_addr The target address in the parameter RAM where data
 *                    should be written. This address is offset by -1 internally.
 * @param data Pointer to the buffer containing the 32-bit words to be written.
 * @param num_words Number of 32-bit words to be written (maximum 5).
 * @return 0 on success, or a negative error code on failure.
 */
int adau1787_safeload_write(sub_addr_t target_addr, uint8_t* data, size_t num_words);

/**
 * @brief Read from the ADAU1787.
 *
 * @param reg_addr The internal address to start reading from.
 * @param value Pointer to buffer to store the read data.
 * @param len Length of the data being read.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_read(sub_addr_t start_addr, uint8_t* value, size_t len);

/**
 * @brief Read a register from the ADAU1787.
 *
 * @param reg_addr The register address to read from.
 * @param value Pointer to buffer to store the read data.
 * @return 0 if successful, negative error code otherwise.
 */
int adau1787_read_register(sub_addr_t reg_addr, reg_word_t* value);

/**
 * @brief Mute or unmute the ADAU1787.
 *
 * @param mute True to mute, false to unmute.
 * @return 0 if successful, negative error code otherwise.
 *
 * @note Not yet implemented
 */
int adau1787_mute(bool mute);

/**
 * @brief Convert a 16-bit address into two bytes.
 *
 * @param word 16-bit word to split.
 * @param byte Pointer to the byte array to store the result.
 */
void split_addr(uint16_t word, uint8_t* byte);

#endif // ADAU1787_H