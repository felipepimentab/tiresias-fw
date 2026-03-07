#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bas_service, CONFIG_LOG_DEFAULT_LEVEL);

static uint8_t battery_level_percent = 100;

static ssize_t read_battery_level(
    struct bt_conn* conn, const struct bt_gatt_attr* attr, void* buf, uint16_t len, uint16_t offset)
{
  const uint8_t* level = attr->user_data;

  return bt_gatt_attr_read(conn, attr, buf, len, offset, level, sizeof(*level));
}

void bas_service_set_battery_level(uint8_t level)
{
  battery_level_percent = level;
}

BT_GATT_SERVICE_DEFINE(bas_service, BT_GATT_PRIMARY_SERVICE(BT_UUID_BAS),
    BT_GATT_CHARACTERISTIC(BT_UUID_BAS_BATTERY_LEVEL, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_battery_level, NULL,
        &battery_level_percent));
