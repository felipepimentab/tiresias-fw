#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <zephyr/kernel.h>

/* Button event types */
enum button_event_type { BUTTON_1_PRESSED, BUTTON_2_PRESSED, BUTTON_3_PRESSED, BUTTON_4_PRESSED };

/* Button event callback type */
typedef void (*button_callback_t)(enum button_event_type event);

/* Initialize the peripheral module */
int peripheral_init(button_callback_t callback);

#endif /* PERIPHERAL_H */