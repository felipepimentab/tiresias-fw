set(SRCS
  # I2S module files
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/i2s/i2s_control.c
  # Main audio codec files
  ${CMAKE_CURRENT_SOURCE_DIR}/audio_codec.c
)

target_sources(app PRIVATE ${SRCS})

# Include the drivers subdirectory
add_subdirectory(drivers)