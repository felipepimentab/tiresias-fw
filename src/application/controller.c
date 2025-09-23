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
static void controller_thread(void* arg1, void* arg2, void* arg3)
{
  ARG_UNUSED(arg1);
  ARG_UNUSED(arg2);
  ARG_UNUSED(arg3);
}

/* === Internal Resources === */
/* Static thread definition */
K_THREAD_DEFINE(controller_thread_id, CONTROLLER_THREAD_STACK_SIZE, controller_thread, NULL, NULL, NULL,
    CONTROLLER_THREAD_PRIORITY, 0, 0);
