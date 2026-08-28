#ifndef ZBUS_COMMON_H_
#define ZBUS_COMMON_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>
#include <stdint.h>

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
  BT_STATE_OFF,
  BT_STATE_INITIALIZING,
  BT_STATE_NOT_CONNECTED,
  BT_STATE_INIT_ERROR,
  BT_STATE_ADVERTISING,
  BT_STATE_CONNECTING,
  BT_STATE_CONNECTED,
  BT_STATE_CONFIG,
  BT_STATE_STREAMING,
  BT_STATE_FOTA,
  BT_STATE_CONNECTION_ERROR,
  BT_STATE_DISCONNECTING
} bt_state;
typedef enum bt_cmd {
  BT_CMD_INIT,
  BT_CMD_ADVERTISE,
  BT_CMD_CONNECT,
  BT_CMD_DISCONNECT,
  BT_CMD_LOW_POWER,

} bt_cmd;
typedef struct bt_state_chan_msg {
  bt_state state;
} bt_state_chan_msg;
typedef struct bt_cmd_chan_msg {
  bt_cmd cmd;
} bt_cmd_chan_msg;

/* === AUDIO CODEC === */
typedef enum codec_state {
  CODEC_STATE_OFF,
  CODEC_STATE_INITIALIZING,
  CODEC_STATE_IDLE,
  CODEC_STATE_ERROR,
  CODEC_STATE_STANDARD,
  CODEC_STATE_STREAMING,
} codec_state;
typedef enum codec_cmd {
  CODEC_CMD_INIT,
  CODEC_CMD_START_STANDARD,
  CODEC_CMD_START_STREAMING,
  CODEC_CMD_STOP_STREAMING,
} codec_cmd;
typedef struct codec_state_chan_msg {
  codec_state state;
} codec_state_chan_msg;
typedef struct codec_cmd_chan_msg {
  codec_cmd cmd;
} codec_cmd_chan_msg;

/* === IMU === */

typedef enum imu_state {
  IMU_STATE_OFF,
  IMU_STATE_INITIALIZING,
  IMU_STATE_IDLE,
  IMU_STATE_STREAMING,
  IMU_STATE_ERROR,
} imu_state;

typedef enum imu_cmd {
  IMU_CMD_INIT,
  IMU_CMD_START_STREAMING,
  IMU_CMD_STOP_STREAMING,
} imu_cmd;

typedef struct imu_state_chan_msg {
  imu_state state;
} imu_state_chan_msg;

typedef struct imu_cmd_chan_msg {
  imu_cmd cmd;
} imu_cmd_chan_msg;

typedef struct imu_data_chan_msg {
  uint32_t seq;
  uint32_t device_time_ms;
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
} imu_data_chan_msg;

#define ORIENTATION_TELEMETRY_VERSION 1
#define ORIENTATION_TELEMETRY_PAYLOAD_SIZE 64

typedef enum orientation_calibration_state {
  ORIENTATION_CAL_NONE = 0,
  ORIENTATION_CAL_TARED = 1,
  ORIENTATION_CAL_GYRO_BIAS_ESTIMATED = 2,
} orientation_calibration_state;

typedef struct __packed orientation_chan_msg {
  uint8_t version;
  uint8_t flags;
  uint16_t payload_size;
  uint32_t seq;
  uint32_t device_time_ms;
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
  float qw;
  float qx;
  float qy;
  float qz;
  float yaw_deg;
  uint8_t calibration_state;
  uint8_t reserved0;
  uint16_t reserved1;
  uint32_t reserved2;
} orientation_chan_msg;

BUILD_ASSERT(sizeof(orientation_chan_msg) == ORIENTATION_TELEMETRY_PAYLOAD_SIZE,
             "orientation telemetry packet must remain 64 bytes");

// struct orientation_chan_msg {
//     float roll;
//     float pitch;
//     float yaw;
// };

#endif /* ZBUS_COMMON_H_ */
