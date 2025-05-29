#ifndef AUDIO_CODEC_H
#define AUDIO_CODEC_H

#include <stdint.h>
#include <zephyr/kernel.h>

/* Task types that can be sent to the codec thread */
enum audio_codec_task_type {
  AUDIO_CODEC_INIT,
  AUDIO_CODEC_START,
  AUDIO_CODEC_STOP,
  AUDIO_CODEC_VOLUME_SET,
  AUDIO_CODEC_MUTE,
  AUDIO_CODEC_UNMUTE
};

/* Task structure for the FIFO */
struct audio_codec_task {
  enum audio_codec_task_type type;
  union {
    struct {
      int32_t volume_db; /* Volume in dB * 100 */
    } volume;
  } data;
};

/* Initialize the audio codec module */
int audio_codec_init(void);

/* Send a task to the codec thread */
int audio_codec_task_send(struct audio_codec_task* task);

#endif /* AUDIO_CODEC_H */