# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(tiresias-fw)

target_sources(app PRIVATE src/main.c)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

zephyr_library_include_directories(
  include
  src/application
  src/system_modules
  src/bluetooth
  src/audio
  src/audio/drivers
  src/SigmaStudioFiles
  src/utils
)

add_subdirectory(src/application)
add_subdirectory(src/system_modules)
add_subdirectory(src/bluetooth)
add_subdirectory(src/audio)
add_subdirectory(src/SigmaStudioFiles)
add_subdirectory(src/utils)