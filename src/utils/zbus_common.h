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

#endif /* ZBUS_COMMON_H_ */