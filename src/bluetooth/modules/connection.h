#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <zephyr/bluetooth/conn.h>
#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ble_connected_cb_t)(struct bt_conn* conn, uint8_t err);

typedef void (*ble_disconnected_cb_t)(struct bt_conn* conn, uint8_t reason);

int ble_init(ble_connected_cb_t connected_cb, ble_disconnected_cb_t disconnected_cb);

#ifdef __cplusplus
}
#endif

#endif /* CONNECTION_H_ */
