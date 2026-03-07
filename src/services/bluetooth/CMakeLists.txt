set(SRCS
  # Bluetooth module files
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/connection.c
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/advertising.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bt_services/bas_service.c
  # Main BLE files
  ${CMAKE_CURRENT_SOURCE_DIR}/bluetooth.c
)

target_sources(app PRIVATE ${SRCS})
