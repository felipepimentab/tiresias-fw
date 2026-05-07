#include "orientation.h"
#include "zbus_common.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>

LOG_MODULE_REGISTER(orientation_ble, LOG_LEVEL_INF);

/* === ZBUS === */

ZBUS_SUBSCRIBER_DEFINE(orientation_ble_sub, 4);
ZBUS_CHAN_DECLARE(orientation_chan);

/* === BLE UUIDs === */

#define BT_UUID_ORIENTATION_SERVICE_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)

#define BT_UUID_ORIENTATION_CHAR_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)

static struct bt_uuid_128 orientation_service_uuid =
    BT_UUID_INIT_128(BT_UUID_ORIENTATION_SERVICE_VAL);

static struct bt_uuid_128 orientation_char_uuid =
    BT_UUID_INIT_128(BT_UUID_ORIENTATION_CHAR_VAL);

/* === BLE === */

static struct orientation_chan_msg orientation_value;

/* forward */
static void notify_handler(struct k_work *work);

/* work item */
static struct k_work notify_work;

/* last message buffer (thread-safe via copy) */
static struct orientation_chan_msg last_msg;

/* CCC callback */

static void ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);

    if (value == BT_GATT_CCC_NOTIFY) {
        LOG_INF("Orientation notifications ENABLED");
    } else {
        LOG_INF("Orientation notifications DISABLED");
    }
}

/* === GATT Service === */

BT_GATT_SERVICE_DEFINE(orientation_svc,
    BT_GATT_PRIMARY_SERVICE(&orientation_service_uuid),

    BT_GATT_CHARACTERISTIC(&orientation_char_uuid.uuid,
        BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_NONE,
        NULL, NULL, &orientation_value),

    BT_GATT_CCC(ccc_cfg_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

/* characteristic pointer */
static struct bt_gatt_attr *orientation_attr = &orientation_svc.attrs[1];

/* === WORK HANDLER === */

static void notify_handler(struct k_work *work)
{
    ARG_UNUSED(work);

    static int cnt = 0;

    orientation_value = last_msg;

    /* reduz taxa (~5 Hz se base ~100 Hz) */
    if (cnt++ % 20 != 0) {
        return;
    }

    int ret = bt_gatt_notify(NULL,
                             orientation_attr,
                             &orientation_value,
                             sizeof(orientation_value));

    if (ret != 0) {
        LOG_DBG("Notify failed: %d", ret);
    } else {
        LOG_DBG("Sent q: %.2f %.2f %.2f %.2f",
                orientation_value.q0,
                orientation_value.q1,
                orientation_value.q2,
                orientation_value.q3);
    }
}

/* === THREAD === */

static void orientation_thread(void)
{
    LOG_INF("Orientation BLE thread started");

    const struct zbus_channel *chan;
    struct orientation_chan_msg msg;

    while (1) {

        int ret = zbus_sub_wait(&orientation_ble_sub, &chan, K_FOREVER);
        if (ret != 0) {
            LOG_ERR("zbus wait error: %d", ret);
            continue;
        }

        if (chan == &orientation_chan) {

            ret = zbus_chan_read(chan, &msg, K_NO_WAIT);
            if (ret != 0) {
                LOG_ERR("Failed to read orientation: %d", ret);
                continue;
            }

            /* copia segura */
            last_msg = msg;

            /* agenda envio BLE */
            k_work_submit(&notify_work);
        }
    }
}

/* === INIT === */

int orientation_init(void)
{
    k_work_init(&notify_work, notify_handler);
    return 0;
}

/* === THREAD DEFINE === */

K_THREAD_DEFINE(orientation_thread_id,
                2048,
                orientation_thread,
                NULL, NULL, NULL,
                3, 0, 0);