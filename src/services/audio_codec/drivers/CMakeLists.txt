# SPDX-License-Identifier: Apache-2.0

target_sources(app PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/adau1787.c
)

# Make header files available to the rest of the project
zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})