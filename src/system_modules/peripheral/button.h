#ifndef _BUTTON_H_
#define _BUTTON_H_

/**
 * @file
 * @brief Button module public API.
 *
 * Provides a single initialization entry point for the button peripheral.
 * The module configures the GPIO, interrupt, basic debouncing, and a Zbus
 * channel used to publish button events to other parts of the system.
 */

#include <stdint.h>
#include <zephyr/drivers/gpio.h>

/**
 * @brief Initialize the button peripheral module.
 *
 * Configures the button GPIO, sets up the interrupt and debounce timer,
 * and prepares the Zbus channel for publishing button events.
 *
 * @return 0 on success, negative errno-style value on failure.
 */
int init_button();

#endif /* _BUTTON_H_ */
