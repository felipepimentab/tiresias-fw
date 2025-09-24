/**
 * @file storage.c
 * @brief Implementation of the non-volatile storage module
 *
 * This file implements a thread-safe storage system using Zephyr's NVS (Non-Volatile Storage)
 * subsystem. It creates a dedicated thread that processes storage operations asynchronously
 * through a message queue, preventing blocking of the calling thread during flash operations.
 *
 * The implementation uses a producer-consumer pattern where client threads send messages
 * to the storage thread, which processes them in order. For synchronous operations,
 * semaphores are used to block the calling thread until the operation completes.
 *
 * The storage is configured to use a dedicated flash partition defined in the device tree
 * as 'storage_partition'.
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(storage_module, LOG_LEVEL_INF);

/**
 * @brief ID used for storing volume settings in NVS
 *
 * Each data item in NVS needs a unique ID. This ID is used for the volume setting.
 * IDs 0x00-0x7F are reserved for system use, 0x80-0xFF are available for application use.
 */
#define STORAGE_VOLUME_ID 0x01

/**
 * @brief Name of the flash partition used for storage
 *
 * This must match a partition defined in the device tree with this exact name.
 */
#define STORAGE_PARTITION storage_partition

/**
 * @brief Device pointer for the flash partition
 *
 * Uses Zephyr's FIXED_PARTITION_DEVICE macro to get the device pointer for the named partition.
 */
#define STORAGE_PARTITION_DEVICE FIXED_PARTITION_DEVICE(STORAGE_PARTITION)

/**
 * @brief Offset of the storage partition in flash memory
 *
 * Uses Zephyr's FIXED_PARTITION_OFFSET macro to get the starting offset of the named partition.
 */
#define STORAGE_PARTITION_OFFSET FIXED_PARTITION_OFFSET(STORAGE_PARTITION)

/**
 * @brief Message types for storage operations
 *
 * Defines the types of operations that can be requested from the storage thread.
 * This enum can be extended with additional operation types as needed.
 */
typedef enum {
  STORAGE_SAVE_VOLUME,  /**< Request to save volume setting to NVS */
  STORAGE_READ_VOLUME   /**< Request to read volume setting from NVS */
} storage_msg_type_t;

/**
 * @brief Message structure for communication with the storage thread
 *
 * This structure contains all the information needed for a storage operation:
 * - The operation type
 * - The data to be stored or retrieved
 * - Pointers for returning results and status codes
 * - A semaphore for synchronous operations
 */
typedef struct {
  storage_msg_type_t type;      /**< Type of storage operation */
  union {
    int32_t volume_db;          /**< Volume data for save operations */
  } data;                        /**< Union of possible data payloads */
  struct k_sem* response_sem;    /**< Optional semaphore to signal completion */
  int32_t* response_value;       /**< Optional pointer to store read values */
  int* response_code;            /**< Optional pointer to store result code */
} storage_msg_t;

/**
 * @brief Message queue for storage operations
 *
 * This queue can hold up to 10 messages, each of size storage_msg_t.
 * The alignment is set to 4 bytes to ensure proper alignment on 32-bit systems.
 */
K_MSGQ_DEFINE(storage_msgq, sizeof(storage_msg_t), 10, 4);

/**
 * @brief Storage thread stack size in bytes
 *
 * The stack size is set to 1024 bytes, which should be sufficient for the
 * operations performed by the storage thread, including NVS operations.
 */
#define STORAGE_STACK_SIZE 1024

/**
 * @brief Storage thread priority
 *
 * The priority is set to 4, which is a medium priority in Zephyr.
 * This allows the thread to be preempted by higher priority tasks
 * while still ensuring timely processing of storage operations.
 */
#define STORAGE_PRIORITY 4

/**
 * @brief Stack area for the storage thread
 *
 * Allocates a stack of STORAGE_STACK_SIZE bytes for the storage thread.
 */
K_THREAD_STACK_DEFINE(storage_stack, STORAGE_STACK_SIZE);

/**
 * @brief Thread control structure for the storage thread
 */
static struct k_thread storage_thread_data;

/**
 * @brief NVS file system structure
 *
 * This structure holds the configuration for the NVS file system,
 * including the flash device, offset, sector size, and sector count.
 */
static struct nvs_fs fs;

/**
 * @brief Storage thread function
 *
 * This function implements the main loop of the storage thread. It waits for
 * messages on the storage message queue and processes them according to their type.
 * For each operation, it performs the requested NVS operation, logs the result,
 * updates any response values or codes, and signals completion via semaphore if provided.
 *
 * The thread never exits and processes messages indefinitely.
 *
 * @param a Unused thread parameter (required by Zephyr thread API)
 * @param b Unused thread parameter (required by Zephyr thread API)
 * @param c Unused thread parameter (required by Zephyr thread API)
 */
static void storage_thread_fn(void* a, void* b, void* c)
{
  storage_msg_t msg;

  while (1) {
    /* Wait indefinitely for a message */
    if (k_msgq_get(&storage_msgq, &msg, K_FOREVER) == 0) {
      int rc = 0;

      switch (msg.type) {
      case STORAGE_SAVE_VOLUME:
        /* Write volume setting to NVS */
        rc = nvs_write(&fs, STORAGE_VOLUME_ID, &msg.data.volume_db, sizeof(msg.data.volume_db));
        if (rc >= 0) {
          LOG_INF("Saved volume: %d dB", msg.data.volume_db);
        } else {
          LOG_ERR("Failed to save volume, rc=%d", rc);
        }
        /* Update response code if pointer provided */
        if (msg.response_code)
          *msg.response_code = rc;
        break;

      case STORAGE_READ_VOLUME:
        /* Read volume setting from NVS if response pointer provided */
        if (msg.response_value) {
          rc = nvs_read(&fs, STORAGE_VOLUME_ID, msg.response_value, sizeof(*msg.response_value));
          if (rc > 0) {
            LOG_INF("Read volume: %d dB", *msg.response_value);
          } else {
            LOG_WRN("Volume not found or read error, rc=%d", rc);
          }
          /* Update response code if pointer provided */
          if (msg.response_code)
            *msg.response_code = rc;
        }
        break;
      }

      /* Signal completion if semaphore provided */
      if (msg.response_sem) {
        k_sem_give(msg.response_sem);
      }
    }
  }
}

/**
 * @brief Asynchronously save volume setting to non-volatile storage
 *
 * This function creates a message with the STORAGE_SAVE_VOLUME type and the
 * provided volume value, then sends it to the storage thread for processing.
 * The function returns immediately without waiting for the operation to complete.
 *
 * @param vol_db Volume level in centi-decibels to be stored
 *
 * @return 0 on successful queueing of the request
 *         -ENOMSG if the message queue is full
 */
int storage_save_volume(int32_t vol_db)
{
  /* Prepare message with volume data */
  storage_msg_t msg = {
    .type = STORAGE_SAVE_VOLUME,
    .data.volume_db = vol_db,
    .response_sem = NULL,     /* No semaphore for async operation */
    .response_code = NULL,    /* No response code needed */
  };
  
  /* Send message to storage thread without waiting */
  return k_msgq_put(&storage_msgq, &msg, K_NO_WAIT);
}

/**
 * @brief Synchronously read volume setting from non-volatile storage
 *
 * This function creates a semaphore and a message with the STORAGE_READ_VOLUME type,
 * then sends the message to the storage thread. It then blocks on the semaphore
 * until the storage thread processes the request and signals completion.
 *
 * @param vol_db Pointer to where the read volume will be stored
 *
 * @return Positive value: Number of bytes read (should be sizeof(int32_t))
 *         0: Item not found in NVS
 *         -EFAULT: Failed to queue the read request (queue full)
 *         Other negative values: NVS read errors
 */
int storage_read_volume(int32_t* vol_db)
{
  struct k_sem sem;
  int rc = 0;

  /* Initialize semaphore with count 0 (will block) and max count 1 */
  k_sem_init(&sem, 0, 1);

  /* Prepare message with response pointers and semaphore */
  storage_msg_t msg = {
    .type = STORAGE_READ_VOLUME,
    .response_value = vol_db,     /* Where to store the read value */
    .response_code = &rc,         /* Where to store the result code */
    .response_sem = &sem,         /* Semaphore to signal completion */
  };

  /* Send message to storage thread without waiting */
  if (k_msgq_put(&storage_msgq, &msg, K_NO_WAIT) != 0) {
    return -EFAULT;  /* Failed to queue the message */
  }

  /* Block until storage thread signals completion */
  k_sem_take(&sem, K_FOREVER);
  
  /* Return the result code set by the storage thread */
  return rc;
}

/**
 * @brief Initialize the storage module
 *
 * This function performs the following initialization steps:
 * 1. Sets up the flash device using the storage partition defined in the device tree
 * 2. Configures the NVS file system with appropriate parameters
 * 3. Mounts the NVS file system
 * 4. Creates and starts the storage handling thread
 *
 * @return 0 on success, or one of the following negative error codes:
 *         -ENODEV: Flash device is not ready
 *         Other error codes from flash_get_page_info_by_offs() or nvs_mount()
 */
int storage_init(void)
{
  struct flash_pages_info info;
  int rc;

  /* Get flash device from partition */
  fs.flash_device = STORAGE_PARTITION_DEVICE;
  if (!device_is_ready(fs.flash_device)) {
    LOG_ERR("Flash device not ready");
    return -ENODEV;
  }

  /* Configure NVS parameters */
  fs.offset = STORAGE_PARTITION_OFFSET;
  rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
  if (rc != 0) {
    LOG_ERR("flash_get_page_info_by_offs failed: %d", rc);
    return rc;
  }

  /* Set sector size based on flash page size */
  fs.sector_size = info.size;
  /* Use 3 sectors for wear leveling */
  fs.sector_count = 3;

  /* Mount the NVS file system */
  rc = nvs_mount(&fs);
  if (rc != 0) {
    LOG_ERR("nvs_mount failed: %d", rc);
    return rc;
  }

  /* Create and start the storage thread */
  k_thread_create(&storage_thread_data, storage_stack, STORAGE_STACK_SIZE, storage_thread_fn, NULL, NULL, NULL,
      STORAGE_PRIORITY, 0, K_NO_WAIT);

  LOG_INF("Storage module initialized");
  return 0;
}