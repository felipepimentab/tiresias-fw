#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

/**
 * @brief Task types that can be handled by the controller module.
 *
 * Extend this enum to define the different types of tasks that should be handled
 * in the controller thread.
 */
enum controller_task_type {
  CONTROLLER_TASK_NOOP = 0, /**< Default no-operation task (optional placeholder) */
  // Add additional task types below:
  // CONTROLLER_TASK_VOLUME_SYNC,
  // CONTROLLER_TASK_PARAM_UPDATE,
};

/**
 * @brief Structure representing a controller task.
 *
 * This structure is used to pass tasks to the controller thread via a FIFO.
 * The type field determines what the controller thread should do, and the
 * payload pointer can be used to pass custom data relevant to the task type.
 */
struct controller_task {
  enum controller_task_type type; /**< Type of task to be executed */
  void* payload; /**< Pointer to additional task-specific data (optional) */
};

/**
 * @brief Initializes the controller thread and associated resources.
 *
 * Creates the controller thread and starts listening for tasks on the FIFO queue.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int controller_init(void);

/**
 * @brief Enqueues a task for execution by the controller thread.
 *
 * Allocates memory for the task and copies its contents before sending it
 * to the internal FIFO. Memory is automatically freed after execution.
 *
 * @param task Pointer to a task structure to enqueue.
 * @return 0 on success, -ENOMEM if memory allocation fails, -EIO if the task couldn't be enqueued.
 */
int controller_task_send(struct controller_task* task);

#endif /* CONTROLLER_H */