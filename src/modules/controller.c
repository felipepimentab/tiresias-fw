#include "controller.h"
#include "audio_codec.h"
#include "peripheral.h"
#include "storage.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(controller_module, CONFIG_LOG_DEFAULT_LEVEL);

/* === Configuration Constants === */
#define CONTROLLER_THREAD_STACK_SIZE 1024
#define CONTROLLER_THREAD_PRIORITY 4
#define CONTROLLER_FIFO_LENGTH 8
#define CONTROLLER_ALIGN_BYTES 4

/* === Internal Resources === */
/* Thread stack and metadata */
K_THREAD_STACK_DEFINE(controller_thread_stack, CONTROLLER_THREAD_STACK_SIZE);
static struct k_thread controller_thread_data;

/* FIFO queue for task messaging */
K_FIFO_DEFINE(controller_fifo);

/* Memory slab for task allocation */
K_MEM_SLAB_DEFINE(controller_task_slab, sizeof(struct controller_task), CONTROLLER_FIFO_LENGTH, CONTROLLER_ALIGN_BYTES);

/* === Controller Thread Function === */

/**
 * @brief Controller thread function.
 *
 * Waits for tasks from the FIFO queue and executes them accordingly.
 * Frees memory after task completion.
 */
static void controller_thread_fn(void* arg1, void* arg2, void* arg3)
{
  ARG_UNUSED(arg1);
  ARG_UNUSED(arg2);
  ARG_UNUSED(arg3);

  struct controller_task* task;

  while (1) {
    // K_FOREVER means the thread will wait until a task is available in the fifo
    task = k_fifo_get(&controller_fifo, K_FOREVER);
    if (!task) {
      continue;
    }

    switch (task->type) {
    default:
      LOG_WRN("Received unknown controller task type: %d", task->type);
      break;
    }

    /* Free memory after task execution */
    k_mem_slab_free(&controller_task_slab, (void**)&task);
  }
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
  k_thread_create(&controller_thread_data, controller_thread_stack, K_THREAD_STACK_SIZEOF(controller_thread_stack),
      controller_thread_fn, NULL, NULL, NULL, CONTROLLER_THREAD_PRIORITY, 0, K_NO_WAIT);
  return 0;
}

/**
 * @brief Enqueues a new task to be handled by the controller thread
 *
 * @param task Pointer to a pre-filled task (copied internally)
 * @return 0 on success, -ENOMEM if memory allocation fails
 */
int controller_task_send(struct controller_task* task)
{
  struct controller_task* new_task;

  if (k_mem_slab_alloc(&controller_task_slab, (void**)&new_task, K_NO_WAIT) != 0) {
    LOG_ERR("Failed to allocate memory for controller task");
    return -ENOMEM;
  }

  memcpy(new_task, task, sizeof(struct controller_task));
  k_fifo_put(&controller_fifo, new_task);

  LOG_DBG("Controller task enqueued (type: %d).", task->type);
  return 0;
}

// static void button_event_handler(enum button_event_type event)
// {
//   switch (event) {
//   case BUTTON_1_PRESSED:
//     LOG_INF("Button 1 pressed! (external)");
//     break;
//   case BUTTON_2_PRESSED:
//     LOG_INF("Button 2 pressed! (external)");
//     break;
//   case BUTTON_3_PRESSED:
//     LOG_INF("Button 3 pressed! (external)");
//     break;
//   case BUTTON_4_PRESSED:
//     LOG_INF("Button 4 pressed! (external)");
//     break;
//   default:
//     return;
//   }
// }