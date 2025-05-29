# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tiresias-fw)

target_sources(app PRIVATE src/main.c)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/drivers)
add_subdirectory(src/modules)
add_subdirectory(src/SigmaStudioFiles)

zephyr_library_include_directories(
  include
  src/drivers
  src/modules
  src/SigmaStudioFiles
)