#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "peripheral.h"
#include <stdint.h>
#include <zephyr/zbus/zbus.h>

/**
 * @brief Initializes the controller thread and associated resources.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int controller_init(void);

#endif /* CONTROLLER_H */