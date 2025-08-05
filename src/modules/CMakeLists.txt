#
# Copyright (c) 2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

set(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/ble.c
  ${CMAKE_CURRENT_SOURCE_DIR}/i2s_control.c
  ${CMAKE_CURRENT_SOURCE_DIR}/peripheral.c
  ${CMAKE_CURRENT_SOURCE_DIR}/storage.c
)

target_sources(app PRIVATE ${SRCS})