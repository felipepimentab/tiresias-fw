#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/zbus/zbus.h>

// #include "drivers/adau1701.h"
#include "drivers/adau1787.h"

LOG_MODULE_REGISTER(Main_app, LOG_LEVEL_INF);

// int main(void) { adau1701_init(); }
int main(void) { adau1787_init(); }