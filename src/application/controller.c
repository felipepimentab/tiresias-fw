#include "controller.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(controller_module, CONFIG_LOG_DEFAULT_LEVEL);

/* === Configuration Constants === */
#define CONTROLLER_THREAD_STACK_SIZE 1024
#define CONTROLLER_THREAD_PRIORITY 3

/* === Controller Thread Function === */

/**
 * @brief Controller thread function.
 */
static void controller_thread(void)
{
  LOG_INF("Controller thread started");
  /* Main thread loop */
  while (1) {
    /* Sleep until explicitly woken up by a direct function call */
    k_sleep(K_FOREVER);
  }
}

/* === Internal Resources === */
/* Static thread definition */
K_THREAD_DEFINE(controller_thread_id, CONTROLLER_THREAD_STACK_SIZE, controller_thread, NULL, NULL, NULL,
    CONTROLLER_THREAD_PRIORITY, 0, 0);
