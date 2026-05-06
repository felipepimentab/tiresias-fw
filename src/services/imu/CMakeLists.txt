set(SRCS
  # Main IMU files
  ${CMAKE_CURRENT_SOURCE_DIR}/imu.c
)

target_sources(app PRIVATE ${SRCS})