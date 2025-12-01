/**
 * @file storage.c
 * @brief Implementation of the non-volatile storage module
 */

#include "storage.h"
#include "macros_common.h"
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/reboot.h>

LOG_MODULE_REGISTER(storage, LOG_LEVEL_INF);

static struct nvs_fs fs;

#define NVS_PARTITION storage_partition
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

int init_storage()
{
  int ret = 0;
  struct flash_pages_info info;

  fs.flash_device = NVS_PARTITION_DEVICE;
  if (!device_is_ready(fs.flash_device)) {
    printk("Flash device %s is not ready\n", fs.flash_device->name);
    return 0;
  }

  fs.offset = NVS_PARTITION_OFFSET;
  ret = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
  ERR_CHK(ret);
  fs.sector_size = info.size;
  fs.sector_count = 3U;

  ret = nvs_mount(&fs);
  ERR_CHK(ret);

  return ret;
}