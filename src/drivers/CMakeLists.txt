#
# Copyright (c) 2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

target_sources(app PRIVATE
	      #  ${CMAKE_CURRENT_SOURCE_DIR}/adau1701.c
	      #  ${CMAKE_CURRENT_SOURCE_DIR}/adau1701.h
				 ${CMAKE_CURRENT_SOURCE_DIR}/adau1787.c
	       ${CMAKE_CURRENT_SOURCE_DIR}/adau1787.h
	       ${CMAKE_CURRENT_SOURCE_DIR}/SigmaStudioFW.h
)
