set(SRCS
  # Peripheral Module
  ${CMAKE_CURRENT_SOURCE_DIR}/peripheral/peripheral.c
  ${CMAKE_CURRENT_SOURCE_DIR}/peripheral/button.c
  ${CMAKE_CURRENT_SOURCE_DIR}/peripheral/led.c
  # Storage Module
  # ${CMAKE_CURRENT_SOURCE_DIR}/storage/storage.c
)

target_sources(app PRIVATE ${SRCS})