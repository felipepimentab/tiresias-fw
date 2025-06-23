#ifndef AUDIO_CODEC_H
#define AUDIO_CODEC_H

#include <stdint.h>
#include <zephyr/kernel.h>

/**
 * @file audio_codec.h
 * @brief Audio codec control module interface
 *
 * This module provides a threaded interface for controlling an audio codec
 * (e.g., ADAU1787) through asynchronous task submission. Tasks are sent
 * via FIFO and processed by a dedicated control thread.
 */

/**
 * @brief Available task types for the audio codec control thread.
 */
enum audio_codec_task_type {
  AUDIO_CODEC_INIT, /**< Initialize the audio codec hardware */
  AUDIO_CODEC_VOLUME_SET, /**< Set output volume (expects payload: int32_t*) */
  AUDIO_CODEC_VOLUME_GET, /**< Get current volume (expects payload: int32_t*) */
  AUDIO_CODEC_PARAM_SET, /**< Set filter/compressor parameters (payload type varies) */
  AUDIO_CODEC_MUTE, /**< Mute output */
  AUDIO_CODEC_UNMUTE /**< Unmute output */
};

/**
 * @brief Structure representing a task to be executed by the codec thread.
 *
 * The task type determines the expected contents of the payload.
 *
 * - For `AUDIO_CODEC_VOLUME_SET`: `payload` should point to an `int32_t` value.
 * - For `AUDIO_CODEC_PARAM_SET`: `payload` should point to a user-defined parameter struct.
 * - For other types: `payload` can be `NULL` or used as needed.
 */
struct audio_codec_task {
  enum audio_codec_task_type type; /**< Task type to execute */
  void* payload; /**< Pointer to task-specific data */
};

/**
 * @brief Initializes the audio codec control thread and hardware.
 *
 * This function starts the internal thread and sends an initialization task.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int audio_codec_init(void);

/**
 * @brief Sends a task to the audio codec control thread.
 *
 * This function copies the given task into internal memory and places it into
 * the task FIFO for execution. The original task structure can be discarded
 * by the caller after the function returns.
 *
 * @param task Pointer to the task to send.
 *
 * @return 0 on success, or -ENOMEM if memory allocation fails.
 */
int audio_codec_task_send(struct audio_codec_task* task);

#endif /* AUDIO_CODEC_H */