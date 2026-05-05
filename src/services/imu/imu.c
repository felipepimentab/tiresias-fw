#include "imu.h"
#include "macros_common.h"
#include "zbus_common.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(imu_module, LOG_LEVEL_INF);

/** @brief Device Tree node for BMI270 */
#define IMU_NODE DT_NODELABEL(bmi270)

/** @brief Device pointer */
static const struct device *imu_dev = DEVICE_DT_GET(IMU_NODE);

/* === Thread Config === */

#define IMU_THREAD_STACK_SIZE 2048
#define IMU_THREAD_PRIORITY 4
#define IMU_SAMPLE_PERIOD_MS 10   // 100 Hz

/* === ZBUS === */

ZBUS_SUBSCRIBER_DEFINE(imu_cmd_sub, 4);

ZBUS_CHAN_DEFINE(imu_state_chan, struct imu_state_chan_msg, NULL, NULL,
                 ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(0));

ZBUS_CHAN_DEFINE(imu_data_chan, struct imu_data_chan_msg, NULL, NULL,
                 ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(0));

ZBUS_CHAN_DEFINE(imu_cmd_chan, struct imu_cmd_chan_msg, NULL, NULL,
                 ZBUS_OBSERVERS(imu_cmd_sub), ZBUS_MSG_INIT(0));

/* === Internal State === */

static imu_state current_state = IMU_STATE_OFF;

/* === State Update === */

static void set_imu_state(imu_state state)
{
  struct imu_state_chan_msg msg;

  if (current_state == state) {
    return;
  }

  current_state = state;
  msg.state = state;

  int err = zbus_chan_pub(&imu_state_chan, &msg, ZBUS_READ_TIMEOUT_MS);
  if (err != 0) {
    LOG_ERR("Failed to publish IMU state: %d", err);
  }
}

/* === Device Bind === */

static int imu_bind_device(void)
{
  LOG_INF("Initializing IMU...");

  if (!device_is_ready(imu_dev)) {
      LOG_ERR("BMI270 not ready");
      return -ENODEV;
  }

  LOG_INF("IMU device bound successfully");
  return 0;
}

/* === State Handlers === */

static void handle_state_initializing(void)
{
  int ret;

  LOG_DBG("Configuring IMU");

  ret = imu_bind_device();
  if (ret != 0) {
    set_imu_state(IMU_STATE_ERROR);
    return;
  }

  struct sensor_value odr = {
    .val1 = 100,
    .val2 = 0
  };

  ret = sensor_attr_set(imu_dev,
                        SENSOR_CHAN_ACCEL_XYZ,
                        SENSOR_ATTR_SAMPLING_FREQUENCY,
                        &odr);
  if (ret != 0) {
    LOG_ERR("Failed to set accel ODR: %d", ret);
    set_imu_state(IMU_STATE_ERROR);
  return;
  }

  ret = sensor_attr_set(imu_dev,
                        SENSOR_CHAN_GYRO_XYZ,
                        SENSOR_ATTR_SAMPLING_FREQUENCY,
                        &odr);
  if (ret != 0) {
    LOG_ERR("Failed to set gyro ODR: %d", ret);
    set_imu_state(IMU_STATE_ERROR);
    return;
  }

  LOG_INF("IMU configured (ODR = 100 Hz)");
  set_imu_state(IMU_STATE_IDLE);
}

/* === State Machine === */

static void imu_state_machine(imu_cmd cmd)
{
  switch (current_state) {

  case IMU_STATE_OFF:
    if (cmd == IMU_CMD_INIT) {
        set_imu_state(IMU_STATE_INITIALIZING);
        handle_state_initializing();
    }
    break;

  case IMU_STATE_IDLE:
    if (cmd == IMU_CMD_START_STREAMING) {
        set_imu_state(IMU_STATE_STREAMING);
    }
    break;

  case IMU_STATE_STREAMING:
    if (cmd == IMU_CMD_STOP_STREAMING) {
        set_imu_state(IMU_STATE_IDLE);
    }
    break;

  case IMU_STATE_ERROR:
    /* Optional recovery */
    break;

  default:
    break;
  }
}

/* === Thread === */

static void imu_thread(void)
{
  LOG_INF("IMU thread started");

  static int cnt = 0; // This is a counter for the log inf

  int ret;
  struct imu_cmd_chan_msg cmd_msg;

  while (1) {

    /* === Handle commands === */
    const struct zbus_channel *chan;

    ret = zbus_sub_wait(&imu_cmd_sub, &chan, K_NO_WAIT);
    if (ret == 0) {
      ret = zbus_chan_read(chan, &cmd_msg, K_MSEC(10));
      if (ret == 0) {
        imu_state_machine(cmd_msg.cmd);
      }
    }

    /* === Sampling === */
    if (current_state == IMU_STATE_STREAMING) {

      struct sensor_value accel[3];
      struct sensor_value gyro[3];

      ret = sensor_sample_fetch(imu_dev);
      if (ret != 0) {
        LOG_ERR("Sample fetch error: %d", ret);
        set_imu_state(IMU_STATE_ERROR);
        continue;
      }

      ret = sensor_channel_get(imu_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
      if (ret != 0) {
        LOG_ERR("Accel read error: %d", ret);
        continue;
      }

      ret = sensor_channel_get(imu_dev, SENSOR_CHAN_GYRO_XYZ, gyro);
      if (ret != 0) {
        LOG_ERR("Gyro read error: %d", ret);
        continue;
      }

      struct imu_data_chan_msg data_msg = {
        .ax = sensor_value_to_double(&accel[0]),
        .ay = sensor_value_to_double(&accel[1]),
        .az = sensor_value_to_double(&accel[2]),
        .gx = sensor_value_to_double(&gyro[0]),
        .gy = sensor_value_to_double(&gyro[1]),
        .gz = sensor_value_to_double(&gyro[2]),
      };

      if (cnt++ % 20 == 0) {
        LOG_INF("IMU sample: ax=%.2f ay=%.2f az=%.2f, gx=%.2f gy=%.2f gz=%.2f", 
          data_msg.ax, data_msg.ay, data_msg.az, 
          data_msg.gx, data_msg.gy, data_msg.gz);
      }  

      ret = zbus_chan_pub(&imu_data_chan, &data_msg, K_NO_WAIT);
      if (ret != 0) {
        LOG_ERR("Failed to publish IMU data: %d", ret);
      }
    }

    k_sleep(K_MSEC(IMU_SAMPLE_PERIOD_MS));
  }
}

/* === IMU Thread Definition === */

K_THREAD_DEFINE(imu_thread_id, IMU_THREAD_STACK_SIZE, imu_thread, NULL, NULL, NULL, IMU_THREAD_PRIORITY, 0, 0);