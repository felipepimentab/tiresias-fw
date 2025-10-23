# SPDX-License-Identifier: Apache-2.0

target_sources_ifdef(CONFIG_AUDIO_CODEC_ADAU1787
  app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/adau1787.c
)
target_sources_ifdef(CONFIG_AUDIO_CODEC_CS47L63
  app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cs47l63_comm.c
)

# Make header files available to the rest of the project
zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})