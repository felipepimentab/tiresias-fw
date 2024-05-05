/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _CS47L63_REG_CONF_H_
#define _CS47L63_REG_CONF_H_

#include "cs47l63_spec.h"

/* Magic value to signal a sleep instead of register address.
 * This can be used e.g. after resets where time is needed before
 * a device is ready.
 * Note that this is a busy wait, and should only be used sparingly where fast
 * execution is not critical.
 *
 * 0001 is used as the reg addr. In case of a fault, this reg is read only.
 */
#define SPI_BUSY_WAIT 0x0001
#define SPI_BUSY_WAIT_US_1000 1000
#define SPI_BUSY_WAIT_US_3000 3000

#define MAX_VOLUME_REG_VAL 0x8D
#define MAX_VOLUME_DB 64
#define OUT_VOLUME_DEFAULT 0x62
#define VOLUME_UPDATE_BIT (1 << 9)

#define CS47L63_SOFT_RESET_VAL 0x5A000000

/* clang-format off */
/* Set up clocks */
const uint32_t clock_configuration[][2] = {
	{ CS47L63_SAMPLE_RATE3, 0x0012 },
	{ CS47L63_SAMPLE_RATE2, 0x0002 },
	{ CS47L63_SAMPLE_RATE1, 0x0003 },
	{ CS47L63_SYSTEM_CLOCK1, 0x034C },
	{ CS47L63_ASYNC_CLOCK1, 0x034C },
	{ CS47L63_FLL1_CONTROL2, 0x88200008 },
	{ CS47L63_FLL1_CONTROL3, 0x10000 },
	{ CS47L63_FLL1_GPIO_CLOCK, 0x0005 },
	{ CS47L63_FLL1_CONTROL1, 0x0001 },
};
/* clang-format on */

/* Set up GPIOs */
const uint32_t GPIO_configuration[][2] = {
	{ CS47L63_GPIO6_CTRL1, 0x61000001 },
	{ CS47L63_GPIO7_CTRL1, 0x61000001 },
	{ CS47L63_GPIO8_CTRL1, 0x61000001 },

	/* Enable CODEC LED */
	{ CS47L63_GPIO10_CTRL1, 0x41008001 },
};

/* Set up input Vesper mic */
const uint32_t vesper_mic_enable_configure[][2] = {
	/* Set MICBIASes */
	{ CS47L63_LDO2_CTRL1, 0x0005 },		// LDO2 Enable Vout = 2.4V
	{ CS47L63_MICBIAS_CTRL1, 0x00EC },	// Vdd_io disable
	{ CS47L63_MICBIAS_CTRL5, 0x0272 },  // Micb1b source - VddA = 1V8

	/* Enable IN1L */
	{ CS47L63_INPUT_CONTROL, 0x000F },

	/* Enable PDM mic as digital input */
	{ CS47L63_INPUT1_CONTROL1, 0x50021 },

	/* Un-mute and set gain to 0dB */
	{ CS47L63_IN1L_CONTROL2, 0x800080 },
	{ CS47L63_IN1R_CONTROL2, 0x800080 },

	/* Volume Update */
	{ CS47L63_INPUT_CONTROL3, 0x20000000 },

	/* Send PDM MIC to I2S Tx */
	// { CS47L63_ASP1TX1_INPUT1, 0x800010 },
	// { CS47L63_ASP1TX2_INPUT1, 0x800011 },

	/* Send PDM MIC to Output */
	{ CS47L63_OUT1L_INPUT1, 0x800010 },
	{ CS47L63_OUT1L_INPUT2, 0x800011 },
	{ CS47L63_OUTPUT_ENABLE_1, 0x0002 },
	{ CS47L63_OUT1L_CONTROL_1, 0x10001 },
};

/* Set up input Digital Stereo MEMS mic (external) */
const uint32_t external_mic_enable_configure[][2] = {
	/* Set MICBIASes */
	{ CS47L63_LDO2_CTRL1, 0x0005 },		// LDO2 Enable Vout = 2.4V
	{ CS47L63_MICBIAS_CTRL1, 0x00EC },  // Vdd_io disable
	{ CS47L63_MICBIAS_CTRL5, 0x0722 },  // Micb1c source - VddA = 1V8

	/* Enable IN1L */
	{ CS47L63_INPUT_CONTROL, 0x000F },
	// 0x000C turns ch1 off and keeps 2 on

	/* Enable PDM mic as digital input for both chs */
	{ CS47L63_INPUT2_CONTROL1, 0x50021 },

	/* Un-mute and set gain to 0dB */
	{ CS47L63_IN2L_CONTROL2, 0x800080 },
	{ CS47L63_IN2R_CONTROL2, 0x8000BE },

	/* Volume Update */
	{ CS47L63_INPUT_CONTROL3, 0x20000000 },

	/* Send PDM MIC to Output */
	{ CS47L63_OUT1L_INPUT1, 0x800012 },
	{ CS47L63_OUT1L_INPUT2, 0x800013 },
	{ CS47L63_OUTPUT_ENABLE_1, 0x0002 },
	{ CS47L63_OUT1L_CONTROL_1, 0x10001 },
};

/* Analog MEMS enable */
const uint32_t mems_analog_mic_enable_configure[][2] = {
	/* Set MICBIASes */
	{ CS47L63_LDO2_CTRL1, 0x0005 },		// LDO2 Enable Vout = 2.4V
	{ CS47L63_MICBIAS_CTRL1, 0x00EC },	// Vdd_io disable
	{ CS47L63_MICBIAS_CTRL5, 0x0722 },  // Micb1c source - VddA = 1V8
	/* OBS: use micbias pin from external DMIC */
	/* Here we configue IN1RP and IN1RN as diff
	   inputs while configuring PWR pin from 
	   header 15 as external pwr for analog mic*/

	/* Select IN1_mode as analog input */
	{ CS47L63_INPUT1_CONTROL1, 0x50020 },

	/* Set IN1RP and IN1RN to single ended */
	{ CS47L63_IN1R_CONTROL1, 0x10000000 },

	/* Un-mute and set gain to 0dB */
	/* If more gain is needed this is the register you might 
		update the value of. In WISCE it is the IN_Rate dile. */
	{ CS47L63_IN1L_CONTROL2, 0x800080 },
	{ CS47L63_IN1R_CONTROL2, 0x8000BE },

	/* Enable IN1R */
	{ CS47L63_INPUT_CONTROL, 0x0001 },

	/* Volume Update */
	{ CS47L63_INPUT_CONTROL3, 0x20000000 },

	/* Send Analog MIC to Output */
	{ CS47L63_OUT1L_INPUT1, 0x800010 },
	{ CS47L63_OUT1L_INPUT2, 0x800011 },
	{ CS47L63_OUTPUT_ENABLE_1, 0x0002 },
	{ CS47L63_OUT1L_CONTROL_1, 0x10001 },
};

/* Analog Electret enable */
const uint32_t electret_analog_mic_enable_configure[][2] = {
	{ CS47L63_LDO2_CTRL1, 0x0005 },		// LDO2 Enable Vout = 2.4V
	{ CS47L63_MICBIAS_CTRL1, 0x00EC },	// Vdd_io disable
	{ CS47L63_MICBIAS_CTRL5, 0x0722 },  // Micb1c source - VddA = 1V8

	/* OBS: use external battery and circuitry to power mics up */

	/* Select IN1_mode as analog input */
	{ CS47L63_INPUT1_CONTROL1, 0x50020 },

	/* Set IN1RP and IN1RN to differential */
	{ CS47L63_IN1R_CONTROL1, 0x00000000 },

	/* Un-mute and set gain to 0dB */
	/* If more gain is needed this is the register you might 
		update the value of. In WISCE it is the IN_Rate dile. */
	{ CS47L63_IN1L_CONTROL2, 0x800080 },
	{ CS47L63_IN1R_CONTROL2, 0x8000A0 },

	/* Enable IN1R */
	{ CS47L63_INPUT_CONTROL, 0x0001 },

	/* Volume Update */
	{ CS47L63_INPUT_CONTROL3, 0x20000000 },

	/* Send Analog MIC to Output */
	{ CS47L63_OUT1L_INPUT1, 0x800010 },
	{ CS47L63_OUT1L_INPUT2, 0x800011 },
	{ CS47L63_OUTPUT_ENABLE_1, 0x0002 },
	{ CS47L63_OUT1L_CONTROL_1, 0x10001 },
};

/* High Quality Analog Electret and Analog Electret enable */
const uint32_t knowles_analog_mic_enable_configure[][2] = {
	{ CS47L63_LDO2_CTRL1, 0x0005 },		// LDO2 Enable Vout = 2.4V
	{ CS47L63_MICBIAS_CTRL1, 0x00EC },	// Vdd_io disable
	{ CS47L63_MICBIAS_CTRL5, 0x0722 },  // Micb1c source - VddA = 1V8

	/* OBS: use external battery and circuitry to power mics up */

	/* Select IN1_mode as analog input */
	{ CS47L63_INPUT1_CONTROL1, 0x50020 },

	/* Set IN1RP and IN1RN to single end */
	{ CS47L63_IN1R_CONTROL1, 0x10000000 },

	/* Un-mute and set gain to 0dB */
	/* If more gain is needed this is the register you might 
		update the value of. In WISCE it is the IN_Rate dile. */
	{ CS47L63_IN1L_CONTROL2, 0x800080 },
	{ CS47L63_IN1R_CONTROL2, 0x8000A0 },

	/* Enable IN1R */
	{ CS47L63_INPUT_CONTROL, 0x0001 },

	/* Volume Update */
	{ CS47L63_INPUT_CONTROL3, 0x20000000 },

	/* Send Analog MIC to Output */
	{ CS47L63_OUT1L_INPUT1, 0x800010 },
	{ CS47L63_OUT1L_INPUT2, 0x800011 },
	{ CS47L63_OUTPUT_ENABLE_1, 0x0002 },
	{ CS47L63_OUT1L_CONTROL_1, 0x10001 },
};

/* line-in enable */
const uint32_t line_in_enable[][2] = {
	/* Select LINE-IN as analog input */
	{ CS47L63_INPUT2_CONTROL1, 0x50020 },

	/* Set IN2L and IN2R to single-ended */
	{ CS47L63_IN2L_CONTROL1, 0x10000000 },
	{ CS47L63_IN2R_CONTROL1, 0x10000000 },

	/* Un-mute and set gain to 0dB */
	{ CS47L63_IN2L_CONTROL2, 0x800080 },
	{ CS47L63_IN2R_CONTROL2, 0x800080 },

	/* Enable IN2L and IN2R */
	{ CS47L63_INPUT_CONTROL, 0x000F },

	/* Volume Update */
	{ CS47L63_INPUT_CONTROL3, 0x20000000 },

	/* Route IN2L and IN2R to I2S */
	{ CS47L63_ASP1TX1_INPUT1, 0x800012 },
	{ CS47L63_ASP1TX2_INPUT1, 0x800013 },
};

/* Set up input I2S mic */
const uint32_t i2s_mic_enable[][2] = {
	/* Enable ASP1 GPIOs */
	{ CS47L63_GPIO5_CTRL1, 0xE1000000 },
	{ CS47L63_GPIO6_CTRL1, 0xE1000000 },
	{ CS47L63_GPIO7_CTRL1, 0xE1000000 },
	{ CS47L63_GPIO8_CTRL1, 0xE1000000 },

	/* Set sample rate to 48kHz */
	{ CS47L63_SAMPLE_RATE1, 0x000000003 },

	/* Disable unused sample rates */
	{ CS47L63_SAMPLE_RATE2, 0 },
	{ CS47L63_SAMPLE_RATE3, 0 },
	{ CS47L63_SAMPLE_RATE4, 0 },

	/* Set ASP1 in slave mode and 16 bit per channel */
	{ CS47L63_ASP2_CONTROL2, 0x10100210 },
	{ CS47L63_ASP2_CONTROL3, 0x0000 },
	{ CS47L63_ASP2_DATA_CONTROL1, 0x0020 },
	{ CS47L63_ASP2_DATA_CONTROL5, 0x0020 },
	// Enables rx1, rx2, tx1 and tx2
	{ CS47L63_ASP2_ENABLES1, 0x30003 },

	/* Route ASP1 to Output */
	{ CS47L63_OUT1L_INPUT1, 0x800030 },
	{ CS47L63_OUT1L_INPUT2, 0x800031 },
	{ CS47L63_OUT1L_CONTROL_1, 0x10001 },
	{ CS47L63_OUTPUT_ENABLE_1, 0x0002 },
};

/* Set up output */
const uint32_t output_enable[][2] = {
	{ CS47L63_OUTPUT_ENABLE_1, 0x0002 },
	{ CS47L63_OUT1L_INPUT1, 0x800010 },
	{ CS47L63_OUT1L_INPUT2, 0x800011 },
};

const uint32_t output_disable[][2] = {
	{ CS47L63_OUTPUT_ENABLE_1, 0x00 },
};

/* Set up ASP1 (I2S) */
const uint32_t asp1_enable[][2] = {
	/* Enable ASP1 GPIOs */
	{ CS47L63_GPIO1_CTRL1, 0x61000000 },
	{ CS47L63_GPIO2_CTRL1, 0xE1000000 },
	{ CS47L63_GPIO3_CTRL1, 0xE1000000 },
	{ CS47L63_GPIO4_CTRL1, 0xE1000000 },
	{ CS47L63_GPIO5_CTRL1, 0x61000001 },

/* Set correct sample rate */
#if CONFIG_AUDIO_SAMPLE_RATE_16000_HZ
	{ CS47L63_SAMPLE_RATE1, 0x000000012 },
#elif CONFIG_AUDIO_SAMPLE_RATE_24000_HZ
	{ CS47L63_SAMPLE_RATE1, 0x000000002 },
#elif CONFIG_AUDIO_SAMPLE_RATE_48000_HZ
	{ CS47L63_SAMPLE_RATE1, 0x000000003 },
#endif
	/* Disable unused sample rates */
	{ CS47L63_SAMPLE_RATE2, 0 },
	{ CS47L63_SAMPLE_RATE3, 0 },
	{ CS47L63_SAMPLE_RATE4, 0 },

	/* Set ASP1 in slave mode and 16 bit per channel */
	{ CS47L63_ASP1_CONTROL2, 0x10100200 },
	{ CS47L63_ASP1_CONTROL3, 0x0000 },
	{ CS47L63_ASP1_DATA_CONTROL1, 0x0020 },
	{ CS47L63_ASP1_DATA_CONTROL5, 0x0020 },
	{ CS47L63_ASP1_ENABLES1, 0x30003 },
};

const uint32_t FLL_toggle[][2] = {
	{ CS47L63_FLL1_CONTROL1, 0x0000 },
	{ SPI_BUSY_WAIT, SPI_BUSY_WAIT_US_1000 },
	{ CS47L63_FLL1_CONTROL1, 0x0001 },
};

const uint32_t soft_reset[][2] = {
	{ CS47L63_SFT_RESET, CS47L63_SOFT_RESET_VAL },
	{ SPI_BUSY_WAIT, SPI_BUSY_WAIT_US_3000 },
};

#endif /* _CS47L63_REG_CONF_H_ */
