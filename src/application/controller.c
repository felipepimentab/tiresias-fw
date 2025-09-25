#include "controller.h"
#include "../system_modules/peripheral/peripheral.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(controller_module, CONFIG_LOG_DEFAULT_LEVEL);

/* === Configuration Constants === */
#define CONTROLLER_THREAD_STACK_SIZE 1024
#define CONTROLLER_THREAD_PRIORITY 3

/* ZBUS multi-channel observer */
ZBUS_SUBSCRIBER_DEFINE(controller_sub, 4);

/* Button event handler */
static void btn_event_handler()
{
  btn_event_msg_t msg;

  int err = zbus_chan_read(&btn_event_chan, &msg, K_MSEC(50));
  if (err != 0) {
    LOG_ERR("Failed to receive button event: %d", err);
    return;
  }

  LOG_INF("Received button event: %d", msg.event);
}

/* === Controller Thread Function === */

/**
 * @brief Controller thread function.
 */
static void controller_thread(void)
{
  LOG_INF("Controller thread started");
  const struct zbus_channel* chan;
  /* Main thread loop */
  while (1) {
    int err = zbus_sub_wait(&controller_sub, &chan, K_FOREVER);
    if (err != 0) {
      LOG_ERR("Failed to wait for controller event: %d", err);
      continue;
    }

    if (chan == &btn_event_chan) {
      btn_event_handler();
      continue;
    }
  }
}

/* === Internal Resources === */
/* Static thread definition */
K_THREAD_DEFINE(controller_thread_id, CONTROLLER_THREAD_STACK_SIZE, controller_thread, NULL, NULL, NULL,
    CONTROLLER_THREAD_PRIORITY, 0, 0);