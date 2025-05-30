#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <zephyr/kernel.h>

/**
 * @file peripheral.h
 * @brief Public interface for the peripheral module handling LEDs and buttons.
 *
 * This module provides initialization and control routines for GPIO-based peripherals,
 * such as buttons and LEDs. It also defines the message types exchanged with the internal
 * peripheral thread via a message queue.
 */

/* === Button Event Types === */

/**
 * @brief Enumerates external button events that can be passed to a user-defined callback.
 */
enum button_event_type { BUTTON_1_PRESSED, BUTTON_2_PRESSED, BUTTON_3_PRESSED, BUTTON_4_PRESSED };

/* === LED Identifiers === */

/**
 * @brief Identifiers for available LEDs.
 */
enum led_t { LED_1, LED_2, LED_3, LED_4 };

/* === LED States === */

/**
 * @brief Represents the logical ON/OFF state of an LED.
 */
typedef enum { LED_OFF, LED_ON } led_state_t;

/* === Peripheral Events === */

/**
 * @brief Events handled internally by the peripheral module.
 *
 * These events represent actions triggered by buttons or requests to change LED state.
 * They are processed asynchronously by the peripheral thread.
 */
enum peripheral_event {
  LED_1_ON,
  LED_1_OFF,
  LED_1_BLINK,
  LED_2_ON,
  LED_2_OFF,
  LED_2_BLINK,
  LED_3_ON,
  LED_3_OFF,
  LED_3_BLINK,
  LED_4_ON,
  LED_4_OFF,
  LED_4_BLINK,
  BTN_1,
  BTN_2,
  BTN_3,
  BTN_4,
};

/* === External Button Handler === */

/**
 * @brief Callback type for external button event notifications.
 *
 * This callback is invoked from the peripheral thread when a button is pressed.
 */
typedef void (*btn_ext_handler_t)(enum button_event_type event);

/* === Public API === */

/**
 * @brief Initializes the peripheral module.
 *
 * Sets up GPIOs for LEDs and buttons, installs interrupt handlers,
 * and starts the peripheral control thread.
 *
 * @param callback User-defined function to be called on button presses.
 * @return 0 on success, negative error code otherwise.
 */
int peripheral_init(btn_ext_handler_t callback);

#endif /* PERIPHERAL_H */