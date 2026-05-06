set(SRCS
  # Main orientation files
  ${CMAKE_CURRENT_SOURCE_DIR}/orientation.c
)

target_sources(app PRIVATE ${SRCS})