set(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_i2s.c
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_sync_timer.c
  # ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_system.c
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/audio_datapath.c
  # ${CMAKE_CURRENT_SOURCE_DIR}/modules/sw_codec_select.c
  # ${CMAKE_CURRENT_SOURCE_DIR}/modules/le_audio_rx.c
  # Main audio codec files
  ${CMAKE_CURRENT_SOURCE_DIR}/audio_control.c
)
target_sources(app PRIVATE
)
target_sources(app PRIVATE ${SRCS})

# Include the drivers subdirectory
add_subdirectory(drivers)