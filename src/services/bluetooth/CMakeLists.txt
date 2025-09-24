set(SRCS
  # Bluetooth Connection module files
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/connection/connection.c
  # Main BLE files
  ${CMAKE_CURRENT_SOURCE_DIR}/ble.c
)

target_sources(app PRIVATE ${SRCS})