#include "controller.h"
#include "audio_codec.h"
#include "peripheral.h"
#include "storage.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(controller_module, CONFIG_LOG_DEFAULT_LEVEL);

/* === Configuration Constants === */
#define CONTROLLER_THREAD_STACK_SIZE 1024
#define CONTROLLER_THREAD_PRIORITY 4

/* === Internal Resources === */
/* Thread stack and metadata */
K_THREAD_STACK_DEFINE(controller_thread_stack, CONTROLLER_THREAD_STACK_SIZE);
static struct k_thread controller_thread_data;

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

/* === Public API Functions === */

/**
 * @brief Initializes the controller thread
 *
 * @return 0 on success
 */
int controller_init(void)
{
  LOG_INF("Controller module started.");

  /* Create the controller thread */
  k_thread_create(&controller_thread_data, controller_thread_stack, K_THREAD_STACK_SIZEOF(controller_thread_stack),
      controller_thread_fn, NULL, NULL, NULL, CONTROLLER_THREAD_PRIORITY, 0, K_NO_WAIT);

  return 0;
}
