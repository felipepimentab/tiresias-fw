#include "audio_codec.h"
#include "../drivers/adau1787.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define AUDIO_CODEC_STACK_SIZE 2048
#define AUDIO_CODEC_PRIORITY 5
#define AUDIO_CODEC_FIFO_LENGTH 8

LOG_MODULE_REGISTER(audio_codec_module, LOG_LEVEL_INF);

/* Thread stack area */
K_THREAD_STACK_DEFINE(audio_codec_stack, AUDIO_CODEC_STACK_SIZE);

/* Thread data */
static struct k_thread audio_codec_thread_data;

/* FIFO for codec tasks */
K_FIFO_DEFINE(audio_codec_fifo);

/* Task memory slab */
K_MEM_SLAB_DEFINE_STATIC(audio_codec_task_slab, sizeof(struct audio_codec_task), AUDIO_CODEC_FIFO_LENGTH, 4);

/* Thread function */
static void audio_codec_thread_fn(void* arg1, void* arg2, void* arg3)
{
  struct audio_codec_task* task;
  int ret;

  while (1) {
    /* Wait for task from FIFO */
    task = k_fifo_get(&audio_codec_fifo, K_FOREVER);

    switch (task->type) {
    case AUDIO_CODEC_INIT:
      ret = adau1787_init();
      break;

    case AUDIO_CODEC_START:
      /* Add codec start implementation */
      break;

    case AUDIO_CODEC_STOP:
      /* Add codec stop implementation */
      break;

    case AUDIO_CODEC_VOLUME_SET:
      /* Add volume control implementation */
      break;

    case AUDIO_CODEC_MUTE:
      /* Add mute implementation */
      break;

    case AUDIO_CODEC_UNMUTE:
      /* Add unmute implementation */
      break;

    default:
      break;
    }

    /* Free the task memory */
    k_mem_slab_free(&audio_codec_task_slab, (void**)&task);
  }
}

int audio_codec_init(void)
{
  /* Create codec thread */
  k_thread_create(&audio_codec_thread_data, audio_codec_stack, K_THREAD_STACK_SIZEOF(audio_codec_stack),
      audio_codec_thread_fn, NULL, NULL, NULL, AUDIO_CODEC_PRIORITY, 0, K_NO_WAIT);

  /* Send init task to the thread */
  struct audio_codec_task* task;
  if (k_mem_slab_alloc(&audio_codec_task_slab, (void**)&task, K_NO_WAIT) == 0) {
    task->type = AUDIO_CODEC_INIT;
    k_fifo_put(&audio_codec_fifo, task);
    LOG_INF("Audio codec thread started.");
    return 0;
  }

  return -ENOMEM;
}

int audio_codec_task_send(struct audio_codec_task* task)
{
  struct audio_codec_task* new_task;

  /* Allocate memory for the task */
  if (k_mem_slab_alloc(&audio_codec_task_slab, (void**)&new_task, K_NO_WAIT) != 0) {
    return -ENOMEM;
  }

  /* Copy task data */
  memcpy(new_task, task, sizeof(struct audio_codec_task));

  /* Send task to FIFO */
  k_fifo_put(&audio_codec_fifo, new_task);

  return 0;
}