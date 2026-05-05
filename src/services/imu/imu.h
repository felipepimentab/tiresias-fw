#ifndef IMU_H_
#define IMU_H_

#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include "zbus_common.h"

/* === ZBUS Channels (extern) === */

extern const struct zbus_channel imu_state_chan;
extern const struct zbus_channel imu_cmd_chan;
extern const struct zbus_channel imu_data_chan;

/* === API pública mínima (opcional) === */

/*
 * Esses helpers são opcionais, mas úteis para padronizar
 * envio de comandos a partir de outros módulos.
 */

int imu_send_cmd(imu_cmd cmd);

#endif /* IMU_H_ */