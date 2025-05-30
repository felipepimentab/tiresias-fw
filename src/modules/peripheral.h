#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <zephyr/kernel.h>

/* Button event types */
enum button_event_type { BUTTON_1_PRESSED, BUTTON_2_PRESSED, BUTTON_3_PRESSED, BUTTON_4_PRESSED };
enum led_t { LED_1, LED_2, LED_3, LED_4 };
/* LED state type */
typedef enum { LED_OFF, LED_ON } led_state_t;
/* LED state type */
typedef enum { LED_BLINK, LED_BLINK_SLOW, LED_BLINK_FAST } led_blink_t;
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
/* Button event callback type */
typedef void (*btn_ext_handler_t)(enum button_event_type event);

/* Initialize the peripheral module */
int peripheral_init(btn_ext_handler_t callback);

#endif /* PERIPHERAL_H */