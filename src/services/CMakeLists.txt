set(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/audio_codec.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ble.c
)

target_sources(app PRIVATE ${SRCS})