#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

/**
 * @file storage.h
 * @brief Non-volatile storage module for persistent system settings
 *
 * This module provides a thread-safe interface for storing and retrieving
 * persistent configuration data using Zephyr's NVS (Non-Volatile Storage)
 * subsystem. It implements a dedicated thread with a message queue to handle
 * storage operations asynchronously, preventing blocking of the calling thread
 * during flash write operations.
 *
 * The module currently supports storing and retrieving volume settings, with
 * an architecture that allows for easy extension to other configuration parameters
 * in the future.
 *
 * Key features:
 * - Thread-safe operations
 * - Asynchronous storage API with synchronous read option
 * - Flash wear leveling through Zephyr's NVS
 * - Error handling and reporting
 */

/**
 * @brief Initializes the storage module and starts its thread.
 *
 * This function performs the following initialization steps:
 * 1. Sets up the flash device using the storage partition defined in the device tree
 * 2. Configures the NVS (Non-Volatile Storage) file system with appropriate parameters
 * 3. Mounts the NVS file system
 * 4. Creates and starts the storage handling thread
 *
 * This function must be called at system startup before any other storage functions.
 *
 * @return 0 on success, or one of the following negative error codes:
 *         -ENODEV: Flash device is not ready
 *         Other error codes from flash_get_page_info_by_offs() or nvs_mount()
 */
int storage_init(void);

/**
 * @brief Request the storage module to persist the volume level.
 *
 * This function is non-blocking and sends the volume value to the storage thread
 * for asynchronous processing. The function returns immediately after queueing
 * the request, without waiting for the actual flash write operation to complete.
 *
 * The volume value is stored in the NVS with ID STORAGE_VOLUME_ID (0x01).
 *
 * @param vol_db Volume level in centi-decibels (e.g., -6000 for -60.00 dB).
 *              The value is stored as a raw int32_t without validation.
 *
 * @return 0 on successful queueing of the request
 *         -ENOMSG if the message queue is full
 */
int storage_save_volume(int32_t vol_db);

/**
 * @brief Synchronously reads the currently stored volume level.
 *
 * This function blocks until the value is returned by the storage thread.
 * It creates a semaphore and waits for the storage thread to signal completion
 * of the read operation. The function will block indefinitely until the storage
 * thread processes the request.
 *
 * The volume value is read from the NVS with ID STORAGE_VOLUME_ID (0x01).
 *
 * @param[out] vol_db Pointer to where the read volume will be stored.
 *                    Must not be NULL. The value is only updated if the read
 *                    operation succeeds.
 *
 * @return Positive value: Number of bytes read (should be sizeof(int32_t))
 *         0: Item not found in NVS
 *         -EFAULT: Failed to queue the read request (queue full)
 *         Other negative values: NVS read errors
 */
int storage_read_volume(int32_t* vol_db);

#endif /* STORAGE_H */