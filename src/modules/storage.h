#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

/**
 * @brief Initializes the storage module and starts its thread.
 *
 * This function must be called at system startup.
 *
 * @return 0 on success, negative error code otherwise.
 */
int storage_init(void);

/**
 * @brief Request the storage module to persist the volume level.
 *
 * This function is non-blocking and sends the value to the storage thread.
 *
 * @param vol_db Volume level in centi-decibels (e.g., -6000 for -60.00 dB).
 *
 * @return 0 on successful queueing, negative error code otherwise.
 */
int storage_save_volume(int32_t vol_db);

/**
 * @brief Synchronously reads the currently stored volume level.
 *
 * This function blocks until the value is returned by the storage thread.
 *
 * @param[out] vol_db Pointer to where the read volume will be stored.
 *
 * @return 0 on success, negative error code if no value is found or read fails.
 */
int storage_read_volume(int32_t* vol_db);

#endif /* STORAGE_H */