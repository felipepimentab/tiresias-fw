#include "led.h"
#include "macros_common.h"
#include "zbus_common.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(led, LOG_LEVEL_INF);

typedef enum led_state_t {
  OFF,
  ON,
  BLINKING,
} led_state_t;

#define LED1_NODE DT_ALIAS(led0)
#define LED2_NODE DT_ALIAS(led1)
#define LED3_NODE DT_ALIAS(led2)
static struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);
#define N_LEDS 3
#define BLINK_FREQ_MS 500
#define LED_SUB_Q_SIZE 3

static struct gpio_dt_spec* leds[N_LEDS] = { &led1, &led2, &led3 };
static led_state_t led_states[N_LEDS] = { OFF, OFF, OFF };

ZBUS_SUBSCRIBER_DEFINE(led_sub, LED_SUB_Q_SIZE);

ZBUS_CHAN_DEFINE(led_chan, led_chan_msg_t, NULL, NULL, ZBUS_OBSERVERS(led_sub), ZBUS_MSG_INIT(0));

#define LED_THREAD_STACK_SIZE 450
#define LED_THREAD_PRIORITY 6

int handle_led_msg(led_chan_msg_t msg)
{
  int ret = 0;
  board_led_t led_n = msg.led;

  if (led_n > N_LEDS) {
    return -1;
  }

  switch (msg.cmd) {
  case TURN_OFF:
    ret = gpio_pin_set_dt(leds[led_n], GPIO_OUTPUT_INACTIVE);
    ERR_CHK(ret);
    led_states[led_n] = OFF;
    break;

  case TURN_ON:
    ret = gpio_pin_set_dt(leds[led_n], GPIO_OUTPUT_ACTIVE);
    ERR_CHK(ret);
    led_states[led_n] = ON;
    break;

  case BLINK:
    led_states[led_n] = BLINKING;
    struct led_chan_msg_t new_msg = { led_n, TOGGLE };
    ret = zbus_chan_pub(&led_chan, &new_msg, ZBUS_READ_TIMEOUT_MS);
    ERR_CHK(ret);
    break;

  case TOGGLE:
    ret = gpio_pin_toggle_dt(leds[led_n]);
    ERR_CHK(ret);

    if (led_states[led_n] == BLINKING) {
      k_msleep(BLINK_FREQ_MS);
      struct led_chan_msg_t new_msg = { led_n, TOGGLE };
      ret = zbus_chan_pub(&led_chan, &new_msg, ZBUS_READ_TIMEOUT_MS);
      ERR_CHK(ret);
    }
    break;

  default:
    break;
  }
  return ret;
}

void led_thread(void)
{
  struct zbus_channel* chan;
  struct led_chan_msg_t msg;
  int ret;

  while (1) {
    ret = zbus_sub_wait(&led_sub, &chan, K_FOREVER);
    ERR_CHK(ret);

    ret = zbus_chan_read(chan, &msg, ZBUS_READ_TIMEOUT_MS);
    ERR_CHK(ret);

    ret = handle_led_msg(msg);
    ERR_CHK(ret);
  }
}

K_THREAD_DEFINE(led_thread_id, LED_THREAD_STACK_SIZE, led_thread, NULL, NULL, NULL, LED_THREAD_PRIORITY, 0, 0);

int init_led()
{
  int ret = 0;

  if (!gpio_is_ready_dt(&led1)) {
    return -1;
  }

  for (int i = 0; i < N_LEDS; i++) {
    ret = gpio_pin_configure_dt(leds[i], GPIO_OUTPUT_INACTIVE);
    ERR_CHK(ret);
  }

  return ret;
}