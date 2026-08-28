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
ZBUS_CHAN_DEFINE(orientation_chan, struct orientation_chan_msg, NULL, NULL, ZBUS_OBSERVERS(orientation_ble_sub), ZBUS_MSG_INIT(0));

/* === Degree to Radians Macro === */
#define DEG2RAD (3.14159265359f / 180.0f)

/* quaternion */
static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

/* sample freq */
#define SAMPLE_FREQ 100.0f

/* === Madgwick update (gyro + accel) === */

#include <math.h>

// Ajuste o Beta: 
// Valores menores (0.01 - 0.05) = Movimento suave, mas demora a corrigir.
// Valores maiores (0.1 - 0.5) = Correção rápida, mas pode ficar "tremido".
#define beta 0.03f 

static float yaw_from_quaternion(float qw, float qx, float qy, float qz)
{
    float siny_cosp = 2.0f * (qw * qz + qx * qy);
    float cosy_cosp = 1.0f - 2.0f * (qy * qy + qz * qz);
    return atan2f(siny_cosp, cosy_cosp) / DEG2RAD;
}

static void madgwick_update(float gx, float gy, float gz, float ax, float ay, float az, float dt)
{
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

    // 1. Taxa de variação do quaternion baseada no giroscópio
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // 2. Só computa se o acelerômetro estiver enviando dados válidos
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normaliza o acelerômetro
        recipNorm = 1.0f / sqrtf(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Variáveis auxiliares para evitar repetir cálculos
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Função Gradiente Descendente (Onde a mágica acontece)
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;

        // Normaliza o gradiente
        recipNorm = 1.0f / sqrtf(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Aplica o feedback do acelerômetro na taxa de variação
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // 3. Integração (Regra de Euler)
    q0 += qDot1 * dt;
    q1 += qDot2 * dt;
    q2 += qDot3 * dt;
    q3 += qDot4 * dt;

    // 4. Normaliza o quaternion final
    recipNorm = 1.0f / sqrtf(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}

/* === Thread === */
static void head_tracking_thread(void)
{
  LOG_INF("Head tracking thread started");

  static int64_t last_ts = 0;
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
        // gx *= DEG2RAD;
        // gy *= DEG2RAD;
        // gz *= DEG2RAD;

        int64_t now = k_uptime_get();

        float dt;

        if (last_ts == 0) {
            dt = 0.01f; // fallback inicial
        } else {
            dt = (now - last_ts) / 1000.0f;
        }

        last_ts = now;
        /* === aqui entra o Madgwick === */
        madgwick_update(gx, gy, gz, ax, ay, az, dt);

        struct orientation_chan_msg msg = {
            .version = ORIENTATION_TELEMETRY_VERSION,
            .flags = 0,
            .payload_size = ORIENTATION_TELEMETRY_PAYLOAD_SIZE,
            .seq = imu_msg.seq,
            .device_time_ms = imu_msg.device_time_ms,
            .ax = ax,
            .ay = ay,
            .az = az,
            .gx = gx,
            .gy = gy,
            .gz = gz,
            .qw = q0,
            .qx = q1,
            .qy = q2,
            .qz = q3,
            .yaw_deg = yaw_from_quaternion(q0, q1, q2, q3),
            .calibration_state = ORIENTATION_CAL_NONE,
            .reserved0 = 0,
            .reserved1 = 0,
            .reserved2 = 0
        };

        ret = zbus_chan_pub(&orientation_chan, &msg, K_NO_WAIT);

        // ret = zbus_chan_pub(&orientation_chan, &msg, K_SECONDS(1));
        if (ret != 0) {
            LOG_ERR("Failed to publish orientation: %d", ret);
        } else {
            LOG_DBG("Orientation published");
        }

        /* opcional: log */
        // LOG_INF("HT update gx=%f gy=%f gz=%f", gx, gy, gz);
    }
  }
}

K_THREAD_DEFINE(ht_thread_id, 2048, head_tracking_thread, NULL, NULL, NULL, 3, 0, 0);
