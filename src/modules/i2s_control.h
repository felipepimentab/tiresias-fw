#ifndef I2S_CONTROL_H
#define I2S_CONTROL_H

#include <stddef.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2s.h>
#include <zephyr/kernel.h>

/**
 * @brief I2S audio parameters
 */
#define I2S_SAMPLE_FREQUENCY 44100 /**< Audio sampling frequency in Hz */
#define I2S_SAMPLE_BIT_WIDTH 16 /**< Bits per sample */
#define I2S_CHANNEL_COUNT 2 /**< Stereo audio */
#define I2S_BYTES_PER_SAMPLE sizeof(int16_t)
#define I2S_BLOCK_DURATION_MS 10 /**< Each block represents 10ms of audio */
#define I2S_BLOCK_SIZE_BYTES                                                                                           \
  ((I2S_SAMPLE_FREQUENCY / (1000 / I2S_BLOCK_DURATION_MS)) * I2S_CHANNEL_COUNT * I2S_BYTES_PER_SAMPLE)
#define I2S_BLOCK_COUNT 4 /**< Total blocks in memory slab */
#define I2S_TRANSFER_TIMEOUT_MS 1000 /**< Timeout for transfers */

/**
 * @brief Initialize the I2S interface (RX and TX)
 *
 * @return 0 on success, negative errno on failure.
 */
int i2s_control_init(void);

/**
 * @brief Start the I2S transfer
 *
 * This function allocates and sends a few zeroed blocks to initiate TX and starts both RX and TX.
 *
 * @return 0 on success, negative errno on failure.
 */
int i2s_control_start(void);

/**
 * @brief Stop the I2S transfer
 *
 * This function stops and resets RX and TX directions.
 *
 * @return 0 on success, negative errno on failure.
 */
int i2s_control_stop(void);

/**
 * @brief Allocate an audio TX block for writing
 *
 * @param[out] block Pointer to the allocated buffer
 * @return 0 on success, negative errno on failure
 */
int i2s_control_alloc_tx_block(void** block);

/**
 * @brief Queue a written TX block for transmission
 *
 * @param block Pointer to a block of audio samples
 * @return 0 on success, negative errno on failure
 */
int i2s_control_submit_tx_block(void* block);

/**
 * @brief Read a received RX block from the I2S interface
 *
 * @param[out] block Pointer to the buffer to be filled with data
 * @param[out] size  Size of the received block in bytes
 * @return 0 on success, negative errno on failure
 */
int i2s_control_read_rx_block(void** block, size_t* size);

/**
 * @brief Release a previously read RX block
 *
 * @param block Pointer to the block to release
 * @return 0 on success, negative errno on failure
 */
int i2s_control_release_rx_block(void* block);

#endif /* I2S_CONTROL_H */