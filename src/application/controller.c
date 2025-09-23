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
 *
 * Waits for messages from the ZBUS channels and executes them accordingly.
 */
static void controller_thread_fn(void* arg1, void* arg2, void* arg3)
{
  ARG_UNUSED(arg1);
  ARG_UNUSED(arg2);
  ARG_UNUSED(arg3);
}

/* === Internal Resources === */
/* Thread definition using K_THREAD_DEFINE macro */
K_THREAD_DEFINE(controller_thread, CONTROLLER_THREAD_STACK_SIZE, controller_thread_fn, NULL, NULL, NULL,
    CONTROLLER_THREAD_PRIORITY, 0, 0);
