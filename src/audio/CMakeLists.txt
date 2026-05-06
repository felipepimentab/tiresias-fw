set(SRCS
  # I2S module files
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_i2s.c
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_sync_timer.c
  # Main audio codec files
  ${CMAKE_CURRENT_SOURCE_DIR}/audio_codec.c
)

target_sources(app PRIVATE ${SRCS})

# Include the drivers subdirectory
add_subdirectory(drivers)