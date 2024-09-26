#
# Copyright (c) 2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

target_sources(app PRIVATE
	       ${CMAKE_CURRENT_SOURCE_DIR}/adau_1787_IC_1_SIGMA.h
	       ${CMAKE_CURRENT_SOURCE_DIR}/adau_1787_IC_1_SIGMA_REG.h
	       ${CMAKE_CURRENT_SOURCE_DIR}/adau_1787_IC_1_SIGMA_PARAM.h
				 ${CMAKE_CURRENT_SOURCE_DIR}/adau_1787_IC_1_FAST.h
	       ${CMAKE_CURRENT_SOURCE_DIR}/adau_1787_IC_1_FAST_REG.h
	       ${CMAKE_CURRENT_SOURCE_DIR}/adau_1787_IC_1_FAST_PARAM.h
)
