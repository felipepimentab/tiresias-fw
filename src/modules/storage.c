#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(storage_module, LOG_LEVEL_INF);

#define STORAGE_VOLUME_ID 0x01
#define STORAGE_PARTITION storage_partition
#define STORAGE_PARTITION_DEVICE FIXED_PARTITION_DEVICE(STORAGE_PARTITION)
#define STORAGE_PARTITION_OFFSET FIXED_PARTITION_OFFSET(STORAGE_PARTITION)

/* Message types */
typedef enum { STORAGE_SAVE_VOLUME, STORAGE_READ_VOLUME } storage_msg_type_t;

/* Message struct */
typedef struct {
  storage_msg_type_t type;
  union {
    int32_t volume_db;
  } data;
  struct k_sem* response_sem;
  int32_t* response_value;
  int* response_code;
} storage_msg_t;

/* Queue */
K_MSGQ_DEFINE(storage_msgq, sizeof(storage_msg_t), 10, 4);

/* Thread definition */
#define STORAGE_STACK_SIZE 1024
#define STORAGE_PRIORITY 4
K_THREAD_STACK_DEFINE(storage_stack, STORAGE_STACK_SIZE);
static struct k_thread storage_thread_data;

static struct nvs_fs fs;

static void storage_thread_fn(void* a, void* b, void* c)
{
  storage_msg_t msg;

  while (1) {
    if (k_msgq_get(&storage_msgq, &msg, K_FOREVER) == 0) {
      int rc = 0;

      switch (msg.type) {
      case STORAGE_SAVE_VOLUME:
        rc = nvs_write(&fs, STORAGE_VOLUME_ID, &msg.data.volume_db, sizeof(msg.data.volume_db));
        if (rc >= 0) {
          LOG_INF("Saved volume: %d dB", msg.data.volume_db);
        } else {
          LOG_ERR("Failed to save volume, rc=%d", rc);
        }
        if (msg.response_code)
          *msg.response_code = rc;
        break;

      case STORAGE_READ_VOLUME:
        if (msg.response_value) {
          rc = nvs_read(&fs, STORAGE_VOLUME_ID, msg.response_value, sizeof(*msg.response_value));
          if (rc > 0) {
            LOG_INF("Read volume: %d dB", *msg.response_value);
          } else {
            LOG_WRN("Volume not found or read error, rc=%d", rc);
          }
          if (msg.response_code)
            *msg.response_code = rc;
        }
        break;
      }

      if (msg.response_sem) {
        k_sem_give(msg.response_sem);
      }
    }
  }
}

int storage_save_volume(int32_t vol_db)
{
  storage_msg_t msg = {
    .type = STORAGE_SAVE_VOLUME,
    .data.volume_db = vol_db,
    .response_sem = NULL,
    .response_code = NULL,
  };
  return k_msgq_put(&storage_msgq, &msg, K_NO_WAIT);
}

int storage_read_volume(int32_t* vol_db)
{
  struct k_sem sem;
  int rc = 0;

  k_sem_init(&sem, 0, 1);

  storage_msg_t msg = {
    .type = STORAGE_READ_VOLUME,
    .response_value = vol_db,
    .response_code = &rc,
    .response_sem = &sem,
  };

  if (k_msgq_put(&storage_msgq, &msg, K_NO_WAIT) != 0) {
    return -EFAULT;
  }

  k_sem_take(&sem, K_FOREVER);
  return rc;
}

int storage_init(void)
{
  struct flash_pages_info info;
  int rc;

  fs.flash_device = STORAGE_PARTITION_DEVICE;
  if (!device_is_ready(fs.flash_device)) {
    LOG_ERR("Flash device not ready");
    return -ENODEV;
  }

  fs.offset = STORAGE_PARTITION_OFFSET;
  rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
  if (rc != 0) {
    LOG_ERR("flash_get_page_info_by_offs failed: %d", rc);
    return rc;
  }

  fs.sector_size = info.size;
  fs.sector_count = 3;

  rc = nvs_mount(&fs);
  if (rc != 0) {
    LOG_ERR("nvs_mount failed: %d", rc);
    return rc;
  }

  k_thread_create(&storage_thread_data, storage_stack, STORAGE_STACK_SIZE, storage_thread_fn, NULL, NULL, NULL,
      STORAGE_PRIORITY, 0, K_NO_WAIT);

  LOG_INF("Storage module initialized");
  return 0;
}