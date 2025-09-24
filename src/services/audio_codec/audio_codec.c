#include "audio_codec.h"
#include "drivers/adau1787.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(audio_codec_module, LOG_LEVEL_INF);

/* === Configuration Constants === */
#define AUDIO_CODEC_STACK_SIZE 2048 /* Stack size for codec thread */
#define AUDIO_CODEC_PRIORITY 3 /* Thread priority */

/* === Codec Thread Function === */

/**
 * @brief Audio codec control thread
 *
 * Handles audio codec operations directly
 */
static void audio_codec_thread_fn(void)
{
  LOG_INF("Audio codec thread started");
  int ret;

  /* Initialize codec hardware */
  // ret = adau1787_init();
  // if (ret == 0) {
  //   LOG_INF("ADAU1787 codec initialized.");
  // } else {
  //   LOG_ERR("ADAU1787 init failed: %d", ret);
  // }

  /* Main thread loop */
  while (1) {
    /* Sleep until explicitly woken up by a direct function call */
    k_sleep(K_FOREVER);
  }
}

/* Define and automatically start the thread at boot time */
K_THREAD_DEFINE(
    audio_codec_thread, AUDIO_CODEC_STACK_SIZE, audio_codec_thread_fn, NULL, NULL, NULL, AUDIO_CODEC_PRIORITY, 0, 0);
