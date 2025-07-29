/**
 * @file peripheral.h
 * @brief Public API for the Peripheral module (LEDs and Buttons).
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include <zephyr/kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief LED identifiers */
enum led_t {
  LED_1,
  LED_2,
  LED_3,
  LED_4,
};

/** @brief LED state values */
typedef enum {
  LED_OFF = 0,
  LED_ON = 1,
} led_state_t;

/** @brief Button press events (external) */
enum button_event_t {
  BUTTON_1_PRESSED,
  BUTTON_2_PRESSED,
  BUTTON_3_PRESSED,
  BUTTON_4_PRESSED,
};

/**
 * @brief Callback signature for button event handler.
 *
 * @param event The button event that occurred.
 */
typedef void (*btn_ext_handler_t)(enum button_event_t event);

/** @brief Internal peripheral event types (used by the work queue) */
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

/**
 * @brief Initializes the peripheral module, including LEDs and buttons.
 *
 * @param handler Optional callback function for external button press events.
 * @return 0 on success, negative error code otherwise.
 */
int peripheral_init(btn_ext_handler_t handler);

/**
 * @brief Sets the state of a specific LED.
 *
 * @param led   The LED to control.
 * @param state Desired state (ON or OFF).
 * @return 0 on success, negative error code otherwise.
 */
int peripheral_set_led(enum led_t led, led_state_t state);

/**
 * @brief Blinks an LED once with default timing (500ms on, 500ms off).
 * This is a backward-compatible wrapper around peripheral_set_led_blink_async.
 *
 * @param led The LED to blink.
 * @return 0 on success, negative error code otherwise.
 */
int peripheral_set_led_blink(enum led_t led);

/**
 * @brief Blinks an LED asynchronously with configurable parameters.
 * This function is non-blocking and uses the work queue for timing.
 *
 * @param led The LED to blink.
 * @param count Number of complete on-off cycles (0 for indefinite blinking).
 * @param on_time_ms Duration in milliseconds for the ON state.
 * @param off_time_ms Duration in milliseconds for the OFF state.
 * @param end_state Final state of the LED after blinking completes.
 * @return 0 on success, negative error code otherwise.
 */
int peripheral_set_led_blink_async(enum led_t led, uint8_t count, uint16_t on_time_ms, 
                                  uint16_t off_time_ms, led_state_t end_state);

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_H_ */