#ifndef _I2S_CONTROL_H_
#define _I2S_CONTROL_H_

#include <stdint.h>
#include <zephyr/kernel.h>

#define HFCLKAUDIO_12_288_MHZ 0x9BA6
#define HFCLKAUDIO_12_165_MHZ 0x8FD8
#define HFCLKAUDIO_12_411_MHZ 0xA774

int audio_i2s_init(void);

#endif