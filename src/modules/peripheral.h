#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <zephyr/kernel.h>

/* Button event types */
enum button_event_type { BUTTON_1_PRESSED, BUTTON_2_PRESSED, BUTTON_3_PRESSED, BUTTON_4_PRESSED };
enum led_t { LED_1, LED_2, LED_3, LED_4 };
/* LED state type */
typedef enum { LED_OFF, LED_ON } led_state_t;
/* Set the state of a LED */
int peripheral_set_led(enum led_t led, led_state_t state);
/* LED state type */
typedef enum { LED_BLINK, LED_BLINK_SLOW, LED_BLINK_FAST } led_blink_t;
/* Set the state of a LED */
int peripheral_set_led_blink(enum led_t led);
enum peripheral_event { PERIPH_EVENT_BTN1, PERIPH_EVENT_BTN2, PERIPH_EVENT_BTN3, PERIPH_EVENT_BTN4 };
/* Button event callback type */
typedef void (*button_callback_t)(enum button_event_type event);

/* Initialize the peripheral module */
int peripheral_init(button_callback_t callback);

#endif /* PERIPHERAL_H */