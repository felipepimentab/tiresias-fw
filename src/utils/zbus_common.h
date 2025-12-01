#ifndef ZBUS_COMMON_H_
#define ZBUS_COMMON_H_

#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#define ZBUS_READ_TIMEOUT_MS K_MSEC(100)
#define ZBUS_ADD_OBS_TIMEOUT_MS K_MSEC(200)

/* === Button === */
typedef enum btn_event_t {
  BUTTON_1_PRESSED,
} btn_event_t;
typedef struct btn_chan_msg_t {
  enum btn_event_t event;
} btn_chan_msg_t;

/* === LED === */
typedef enum board_led_t {
  LED_1,
  LED_2,
  LED_3,
} board_led_t;
typedef enum led_cmd_t {
  TURN_ON,
  TURN_OFF,
  TOGGLE,
  BLINK,
} led_cmd_t;
typedef struct led_chan_msg_t {
  enum board_led_t led;
  enum led_cmd_t cmd;
} led_chan_msg_t;

/* === BLUETOOTH === */
typedef enum bt_state {
  BLUETOOTH_STATE_OFF,
  BLUETOOTH_STATE_INITIALIZING,
  BLUETOOTH_STATE_NOT_CONNECTED,
  BLUETOOTH_STATE_INIT_ERROR,
  BLUETOOTH_STATE_ADVERTISING,
  BLUETOOTH_STATE_CONNECTING,
  BLUETOOTH_STATE_CONNECTED,
  BLUETOOTH_STATE_CONFIG,
  BLUETOOTH_STATE_STREAMING,
  BLUETOOTH_STATE_FOTA,
  BLUETOOTH_STATE_CONNECTION_ERROR,
  BLUETOOTH_STATE_DISCONNECTING
} bt_state;
typedef enum bt_cmd {
  BLUETOOTH_CMD_INIT,
  BLUETOOTH_CMD_ADVERTISE,
  BLUETOOTH_CMD_CONNECT,
  BLUETOOTH_CMD_DISCONNECT,
  BLUETOOTH_CMD_LOW_POWER,

} bt_cmd;
typedef struct bt_state_chan_msg {
  bt_state state;
} bt_state_chan_msg;
typedef struct bt_cmd_chan_msg {
  bt_cmd cmd;
} bt_cmd_chan_msg;

#endif /* ZBUS_COMMON_H_ */