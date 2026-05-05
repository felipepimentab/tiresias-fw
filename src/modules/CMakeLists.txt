set(SRCS
  # head tracking files
  ${CMAKE_CURRENT_SOURCE_DIR}/head_tracking.c
)

target_sources(app PRIVATE ${SRCS})