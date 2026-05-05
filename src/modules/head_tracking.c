#include "head_tracking.h"
#include "zbus_common.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <math.h>

LOG_MODULE_REGISTER(head_tracking, LOG_LEVEL_INF);

/* === ZBUS === */

ZBUS_SUBSCRIBER_DEFINE(ht_sub, 4);

ZBUS_CHAN_DECLARE(imu_data_chan);

/* output */
ZBUS_CHAN_DEFINE(orientation_chan, struct orientation_chan_msg, NULL, NULL, ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(0));

/* === Madgwick params === */
static float beta = 0.1f; // tuning

/* === Degree to Radians Macro === */
#define DEG2RAD (3.14159265359f / 180.0f)

/* quaternion */
static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

/* sample freq */
#define SAMPLE_FREQ 100.0f

/* === Madgwick update (gyro + accel) === */

static void madgwick_update(float gx, float gy, float gz,
                            float ax, float ay, float az)
{
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;

  /* normalize accel */
  float norm = sqrtf(ax*ax + ay*ay + az*az);
  if (norm == 0.0f) return;
  recipNorm = 1.0f / norm;
  ax *= recipNorm;
  ay *= recipNorm;
  az *= recipNorm;

  /* gradient descent (simplificado) */
  s0 = 4*q0*q2*q2 + 2*q2*ax + 4*q0*q1*q1 - 2*q1*ay;
  s1 = 4*q1*q3*q3 - 2*q3*ax + 4*q0*q0*q1 - 2*q0*ay;
  s2 = 4*q0*q0*q2 + 2*q0*ax + 4*q2*q3*q3 - 2*q3*ay;
  s3 = 4*q1*q1*q3 - 2*q1*ax + 4*q2*q2*q3 - 2*q2*ay;

  norm = sqrtf(s0*s0 + s1*s1 + s2*s2 + s3*s3);
  recipNorm = 1.0f / norm;
  s0 *= recipNorm;
  s1 *= recipNorm;
  s2 *= recipNorm;
  s3 *= recipNorm;

  /* gyro (rad/s assumed) */
  qDot1 = 0.5f * (-q1*gx - q2*gy - q3*gz) - beta*s0;
  qDot2 = 0.5f * ( q0*gx + q2*gz - q3*gy) - beta*s1;
  qDot3 = 0.5f * ( q0*gy - q1*gz + q3*gx) - beta*s2;
  qDot4 = 0.5f * ( q0*gz + q1*gy - q2*gx) - beta*s3;

  q0 += qDot1 * (1.0f / SAMPLE_FREQ);
  q1 += qDot2 * (1.0f / SAMPLE_FREQ);
  q2 += qDot3 * (1.0f / SAMPLE_FREQ);
  q3 += qDot4 * (1.0f / SAMPLE_FREQ);

  norm = sqrtf(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  recipNorm = 1.0f / norm;
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
}

/* === Thread === */
static void head_tracking_thread(void)
{
  LOG_INF("Head tracking thread started");

  const struct zbus_channel *chan;
  struct imu_data_chan_msg imu_msg;

  /* 🔥 REGISTRO DINÂMICO */
  int ret = zbus_chan_add_obs(&imu_data_chan, &ht_sub, K_MSEC(100));
  if (ret != 0) {
    LOG_ERR("Failed to subscribe to imu_data_chan: %d", ret);
  }

  while (1) {

    int ret = zbus_sub_wait(&ht_sub, &chan, K_FOREVER);
    if (ret != 0) {
      LOG_ERR("zbus wait error: %d", ret);
      continue;
    }

    if (chan == &imu_data_chan) {

        ret = zbus_chan_read(chan, &imu_msg, K_MSEC(10));
        if (ret != 0) {
          LOG_ERR("Failed to read IMU data: %d", ret);
          continue;
        }

        /* === AGORA SIM: variáveis definidas === */
        float ax = imu_msg.ax;
        float ay = imu_msg.ay;
        float az = imu_msg.az;

        float gx = imu_msg.gx;
        float gy = imu_msg.gy;
        float gz = imu_msg.gz;

        /* === conversão para rad/s === */
        gx *= DEG2RAD;
        gy *= DEG2RAD;
        gz *= DEG2RAD;

        /* === aqui entra o Madgwick === */
        madgwick_update(gx, gy, gz, ax, ay, az);

        /* opcional: log */
        LOG_INF("HT update gx=%f gy=%f gz=%f", gx, gy, gz);
    }
  }
}

K_THREAD_DEFINE(ht_thread_id, 2048, head_tracking_thread, NULL, NULL, NULL, 3, 0, 0);