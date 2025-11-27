/**
 * @file
 * @brief Button peripheral module.
 *
 * Implements a simple button handler with interrupt-driven event capture,
 * software debouncing, and asynchronous publication of events via a Zbus
 * channel. The ISR pushes events to a message queue; a worker thread pops
 * messages and publishes them on `button_chan`.
 */

#include "button.h"
#include "macros_common.h"
#include "zbus_common.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(button, LOG_LEVEL_INF);

/* Button device-tree alias and GPIO configuration flags */
#define BTN1_NODE DT_ALIAS(sw0)
#define BTN_FLAGS (GPIO_INPUT | GPIO_PULL_UP | GPIO_INT_EDGE_TO_ACTIVE)

/* Zbus channel used to publish button events */
ZBUS_CHAN_DEFINE(button_chan, btn_chan_msg_t, NULL, NULL, ZBUS_OBSERVERS_EMPTY, ZBUS_MSG_INIT(0));

K_MSGQ_DEFINE(button_queue, sizeof(struct btn_chan_msg_t), 1, 4);
#define BUTTON_PUBLISH_THREAD_STACK_SIZE 1024
#define BUTTON_PUBLISH_THREAD_PRIORITY 5
#define BUTTON_DEBOUNCE_MS 50

static const struct gpio_dt_spec btn1 = GPIO_DT_SPEC_GET_OR(BTN1_NODE, gpios, { 0 });
static struct gpio_callback btn1_cb;
static bool debounce_is_ongoing;

/**
 * @brief Debounce timer callback.
 *
 * Marks the debounce period as completed, allowing the next button event.
 */
static void on_button_debounce_timeout(struct k_timer* timer)
{
  debounce_is_ongoing = false;
}
K_TIMER_DEFINE(button_debounce_timer, on_button_debounce_timeout, NULL);

/**
 * @brief Button interrupt service routine.
 *
 * Captures a button press event, applies software debouncing, and enqueues
 * the event for asynchronous publication.
 */
static void btn1_isr(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  int ret;
  struct btn_chan_msg_t msg;
  msg.event = BUTTON_1_PRESSED;

  if (debounce_is_ongoing) {
    LOG_DBG("Button debounce is ongoing");
    return;
  }

  ret = k_msgq_put(&button_queue, (void*)&msg, K_NO_WAIT);
  ERR_CHK(ret);

  debounce_is_ongoing = true;
  k_timer_start(&button_debounce_timer, K_MSEC(BUTTON_DEBOUNCE_MS), K_NO_WAIT);
}

/**
 * @brief Worker thread that publishes button events to Zbus.
 *
 * Dequeues events from `button_queue` and publishes them on `button_chan`.
 * This design keeps ISR execution minimal and moves I/O work to a thread.
 */
static void button_publish_thread(void)
{
  int ret;
  struct btn_chan_msg_t msg;

  while (1) {
    k_msgq_get(&button_queue, &msg, K_FOREVER);

    ret = zbus_chan_pub(&button_chan, &msg, K_NO_WAIT);
    ERR_CHK(ret);
  }
}
K_THREAD_DEFINE(button_publish, BUTTON_PUBLISH_THREAD_STACK_SIZE, button_publish_thread, NULL, NULL, NULL,
    BUTTON_PUBLISH_THREAD_PRIORITY, 0, 0);

/**
 * @brief Initialize the button module.
 *
 * Configures the button GPIO, interrupt, debounce timer, and logging.
 */
int init_button()
{
  int ret = 0;

  if (!device_is_ready(btn1.port)) {
    return -1;
  }

  ret = gpio_pin_configure_dt(&btn1, BTN_FLAGS);
  ERR_CHK(ret);
  ret = gpio_pin_interrupt_configure_dt(&btn1, GPIO_INT_EDGE_TO_ACTIVE);
  ERR_CHK(ret);

  gpio_init_callback(&btn1_cb, btn1_isr, BIT(btn1.pin));
  ret = gpio_add_callback(btn1.port, &btn1_cb);
  ERR_CHK(ret);

  LOG_DBG("Button module initialized");

  return ret;
}
