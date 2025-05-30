#include "audio_codec.h"
#include "../drivers/adau1787.h"

#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(audio_codec_module, LOG_LEVEL_INF);

/* === Configuration Constants === */
#define AUDIO_CODEC_STACK_SIZE 2048 /* Stack size for codec thread */
#define AUDIO_CODEC_PRIORITY 3 /* Thread priority */
#define AUDIO_CODEC_FIFO_LENGTH 8 /* Max number of pending tasks */
#define AUDIO_CODEC_ALIGN_BYTES 4 /* Memory alignment for slab */

/* === Internal Resources === */

/* Thread stack and metadata */
K_THREAD_STACK_DEFINE(audio_codec_stack, AUDIO_CODEC_STACK_SIZE);
static struct k_thread audio_codec_thread_data;

/* FIFO queue for task messaging */
K_FIFO_DEFINE(audio_codec_fifo);

/* Memory slab for task allocation */
K_MEM_SLAB_DEFINE_STATIC(
    audio_codec_task_slab, sizeof(struct audio_codec_task), AUDIO_CODEC_FIFO_LENGTH, AUDIO_CODEC_ALIGN_BYTES);

/* === Codec Thread Function === */

/**
 * @brief Audio codec control thread
 *
 * Waits for tasks from the FIFO queue and executes them accordingly.
 * Frees memory after task completion.
 */
static void audio_codec_thread_fn(void* arg1, void* arg2, void* arg3)
{
  ARG_UNUSED(arg1);
  ARG_UNUSED(arg2);
  ARG_UNUSED(arg3);

  struct audio_codec_task* task;
  int ret;

  while (1) {
    /* Wait indefinitely for a task */
    task = k_fifo_get(&audio_codec_fifo, K_FOREVER);
    if (!task) {
      continue;
    }

    switch (task->type) {
    case AUDIO_CODEC_INIT:
      ret = adau1787_init();
      if (ret == 0) {
        LOG_INF("ADAU1787 codec initialized.");
      } else {
        LOG_ERR("ADAU1787 init failed: %d", ret);
      }
      break;

    case AUDIO_CODEC_VOLUME_SET:
      /* TODO: Implement volume set via I2C */
      LOG_DBG("Volume set request received.");
      break;

    case AUDIO_CODEC_VOLUME_GET:
      /* TODO: Implement volume get via I2C */
      LOG_DBG("Volume get request received.");
      break;

    case AUDIO_CODEC_MUTE:
      /* TODO: Implement mute functionality */
      LOG_DBG("Mute request received.");
      break;

    case AUDIO_CODEC_UNMUTE:
      /* TODO: Implement unmute functionality */
      LOG_DBG("Unmute request received.");
      break;

    default:
      LOG_WRN("Received unknown codec task type: %d", task->type);
      break;
    }

    /* Release memory after task execution */
    k_mem_slab_free(&audio_codec_task_slab, task);
  }
}

/* === Public API Functions === */

/**
 * @brief Initializes the audio codec thread and enqueues the init task
 *
 * @return 0 on success, negative error code otherwise
 */
int audio_codec_init(void)
{
  /* Launch codec thread */
  k_thread_create(&audio_codec_thread_data, audio_codec_stack, K_THREAD_STACK_SIZEOF(audio_codec_stack),
      audio_codec_thread_fn, NULL, NULL, NULL, AUDIO_CODEC_PRIORITY, 0, K_NO_WAIT);

  /* Allocate and send initialization task */
  struct audio_codec_task* task;
  if (k_mem_slab_alloc(&audio_codec_task_slab, (void**)&task, K_NO_WAIT) != 0) {
    LOG_ERR("Failed to allocate memory for init task");
    return -ENOMEM;
  }

  task->type = AUDIO_CODEC_INIT;
  k_fifo_put(&audio_codec_fifo, task);

  LOG_INF("Audio codec thread started and init task enqueued.");
  return 0;
}

/**
 * @brief Enqueues a new task to be handled by the audio codec thread
 *
 * @param task Pointer to a pre-filled task (copied internally)
 * @return 0 on success, -ENOMEM if memory allocation fails
 */
int audio_codec_task_send(struct audio_codec_task* task)
{
  struct audio_codec_task* new_task;

  if (k_mem_slab_alloc(&audio_codec_task_slab, (void**)&new_task, K_NO_WAIT) != 0) {
    LOG_ERR("Failed to allocate memory for audio codec task");
    return -ENOMEM;
  }

  memcpy(new_task, task, sizeof(struct audio_codec_task));
  k_fifo_put(&audio_codec_fifo, new_task);

  LOG_DBG("Audio codec task enqueued (type: %d).", task->type);
  return 0;
}