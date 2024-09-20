/*
 * File:           E:\adau-1787-dsp-program\adau_1787_IC_1_SIGMA.h
 *
 * Created:        Friday, September 20, 2024 2:49:24 PM
 * Description:    adau_1787:IC 1-Sigma program data.
 *
 * This software is distributed in the hope that it will be useful,
 * but is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This software may only be used to program products purchased from
 * Analog Devices for incorporation by you into audio products that
 * are intended for resale to audio product end users. This software
 * may not be distributed whole or in any part to third parties.
 *
 * Copyright ©2024 Analog Devices, Inc. All rights reserved.
 */
#ifndef __ADAU_1787_IC_1_SIGMA_H__
#define __ADAU_1787_IC_1_SIGMA_H__

#include "SigmaStudioFW.h"
#include "adau_1787_IC_1_SIGMA_REG.h"

#define DEVICE_ARCHITECTURE_IC_1_SIGMA            "ADAU1787S"
#define DEVICE_ADDR_IC_1_SIGMA                    0x50

/* DSP Ram Data */
#define NON_MODULO_RAM__SIZE_IC_1_Sigma 4
#define NON_MODULO_RAM__ADDR_IC_1_Sigma 32756
ADI_REG_TYPE NON_MODULO_RAM__Data_IC_1_Sigma[NON_MODULO_RAM__SIZE_IC_1_Sigma] = {
0x00, 0x00, 0x00, 0x00, 
};

/* DSP Program Data */
#define PROGRAM_SIZE_IC_1_Sigma 425
#define PROGRAM_ADDR_IC_1_Sigma 20480
ADI_REG_TYPE Program_Data_IC_1_Sigma[PROGRAM_SIZE_IC_1_Sigma] = {
0x00, 0x00, 0x00, 0x00, 0x00, 
0x91, 0xC0, 0x00, 0x00, 0x00, 
0x92, 0x68, 0x00, 0x00, 0x00, 
0x92, 0x58, 0x00, 0x00, 0x00, 
0x92, 0xA8, 0x00, 0x00, 0x00, 
0x92, 0xB8, 0x00, 0x00, 0x00, 
0xFF, 0xE9, 0x08, 0x20, 0x00, 
0x92, 0x70, 0x00, 0x00, 0x00, 
0x93, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x91, 0xD0, 0x0C, 0x00, 0x00, 
0x90, 0x60, 0x00, 0xE2, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0xFF, 0xD0, 0x07, 0x20, 0x08, 
0xFF, 0xD9, 0x0A, 0x20, 0x00, 
0x00, 0x00, 0x00, 0xE2, 0x00, 
0x00, 0x00, 0x06, 0xA0, 0x00, 
0xFF, 0xC0, 0x00, 0xC0, 0x00, 
0x00, 0x0B, 0xFF, 0x40, 0x00, 
0x91, 0x80, 0x25, 0x00, 0x27, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x08, 0x44, 0x40, 
0x93, 0x00, 0x07, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xE4, 0x00, 
0x00, 0x00, 0x00, 0xC0, 0x00, 
0x92, 0x00, 0x00, 0x00, 0x00, 
0x91, 0xD0, 0x21, 0x00, 0x00, 
0xFF, 0xD0, 0x01, 0x20, 0x00, 
0xFF, 0xB0, 0x01, 0x03, 0x00, 
0x00, 0x09, 0xDE, 0x00, 0x00, 
0x92, 0x10, 0x00, 0x00, 0x00, 
0xFF, 0xE8, 0x00, 0x20, 0x00, 
0xFF, 0xB0, 0x07, 0x02, 0x00, 
0x80, 0x29, 0x08, 0x20, 0x00, 
0x00, 0x10, 0x00, 0xE2, 0x00, 
0x80, 0x39, 0x08, 0x20, 0x00, 
0x00, 0x20, 0x00, 0xE2, 0x00, 
0x00, 0x10, 0x00, 0xC0, 0x00, 
0x00, 0x19, 0x08, 0x20, 0x00, 
0x00, 0x90, 0x00, 0xE2, 0x00, 
0xFF, 0xB0, 0x0E, 0x02, 0x00, 
0xFF, 0xB0, 0x0F, 0x12, 0x00, 
0x00, 0x1B, 0xFF, 0x20, 0x00, 
0x00, 0x70, 0x00, 0xE2, 0x00, 
0x00, 0x80, 0x00, 0xF2, 0x00, 
0x00, 0x59, 0x08, 0x20, 0x00, 
0x00, 0x50, 0x09, 0x22, 0x40, 
0x00, 0x80, 0x09, 0x22, 0x00, 
0x00, 0x39, 0x08, 0x34, 0x00, 
0x00, 0x30, 0x09, 0x22, 0x40, 
0x00, 0x70, 0x09, 0x22, 0x00, 
0x00, 0x40, 0x00, 0xE2, 0x00, 
0x00, 0x60, 0x00, 0xF2, 0x00, 
0xFF, 0xB0, 0x0C, 0x02, 0x00, 
0xFF, 0xB0, 0x0D, 0x12, 0x00, 
0xFF, 0xD0, 0x10, 0x20, 0x00, 
0xFF, 0xD0, 0x12, 0x22, 0x48, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x7F, 0xF0, 0x00, 0xF0, 0x00, 
0xFF, 0xD0, 0x12, 0x20, 0x00, 
0xFF, 0xD0, 0x11, 0x22, 0x37, 
0xFF, 0xD0, 0x11, 0x22, 0x75, 
0xFF, 0xB0, 0x12, 0x02, 0x00, 
0x7F, 0xF9, 0x08, 0x20, 0x00, 
0xFF, 0xD0, 0x11, 0x40, 0x08, 
0xFF, 0xD0, 0x10, 0x60, 0x00, 
0xFF, 0xB0, 0x12, 0x06, 0x23, 
0x00, 0x90, 0x12, 0x20, 0x00, 
0x00, 0xA0, 0x00, 0xE2, 0x00, 
0x00, 0x20, 0x12, 0x20, 0x00, 
0x00, 0xB0, 0x00, 0xE2, 0x00, 
0x00, 0xB9, 0x08, 0x20, 0x00, 
0x84, 0x90, 0x00, 0xE2, 0x00, 
0x00, 0xA9, 0x08, 0x20, 0x00, 
0x84, 0x80, 0x00, 0xE2, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x90, 0x60, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x91, 0x80, 0x0E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
};

/* DSP Parameter (Coefficient) Data */
#define PARAM_SIZE_IC_1_Sigma 76
#define PARAM_ADDR_IC_1_Sigma 8192
ADI_REG_TYPE Param_Data_IC_1_Sigma[PARAM_SIZE_IC_1_Sigma] = {
0x00, 0x00, 0x07, 0xFF, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x20, 0x04, 
0x00, 0x00, 0x4E, 0xB0, 
0x00, 0x00, 0x01, 0xE0, 
0x00, 0x00, 0x07, 0x1C, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x54, 
0x00, 0x00, 0x00, 0x00, 
};


/* Register Default - IC 1.SDSP_CTRL2 */
ADI_REG_TYPE R0_SDSP_CTRL2_IC_1_Sigma_Default[REG_SDSP_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.DAC_VOL0 */
ADI_REG_TYPE R1_DAC_VOL0_IC_1_Sigma_Default[REG_DAC_VOL0_IC_1_Sigma_BYTE] = {
0xFD
};

/* Register Default - IC 1.DAC_VOL1 */
ADI_REG_TYPE R2_DAC_VOL1_IC_1_Sigma_Default[REG_DAC_VOL1_IC_1_Sigma_BYTE] = {
0xFD
};

/* Register Default - IC 1.ADC_DAC_HP_PWR */
ADI_REG_TYPE R3_ADC_DAC_HP_PWR_IC_1_Sigma_Default[REG_ADC_DAC_HP_PWR_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PLL_MB_PGA_PWR */
ADI_REG_TYPE R4_PLL_MB_PGA_PWR_IC_1_Sigma_Default[REG_PLL_MB_PGA_PWR_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.CHIP_PWR */
ADI_REG_TYPE R5_CHIP_PWR_IC_1_Sigma_Default[REG_CHIP_PWR_IC_1_Sigma_BYTE] = {
0x17
};

/* Register Default - IC 1.Power En Delay */
ADI_REG_TYPE R6_POWER_EN_DELAY_IC_1_Sigma_Default[REG_POWER_EN_DELAY_IC_1_Sigma_BYTE] = {
0x00, 0x23
};

/* Register Default - IC 1.CHIP_PWR */
ADI_REG_TYPE R7_CHIP_PWR_IC_1_Sigma_Default[REG_CHIP_PWR_IC_1_Sigma_BYTE] = {
0x17
};

/* Register Default - IC 1.CLK_CTRL1 */
ADI_REG_TYPE R8_CLK_CTRL1_IC_1_Sigma_Default[REG_CLK_CTRL1_IC_1_Sigma_BYTE] = {
0xC8
};

/* Register Default - IC 1.CLK_CTRL2 */
ADI_REG_TYPE R9_CLK_CTRL2_IC_1_Sigma_Default[REG_CLK_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.CLK_CTRL3 */
ADI_REG_TYPE R10_CLK_CTRL3_IC_1_Sigma_Default[REG_CLK_CTRL3_IC_1_Sigma_BYTE] = {
0x00, 0x02
};

/* Register Default - IC 1.CLK_CTRL5 */
ADI_REG_TYPE R11_CLK_CTRL5_IC_1_Sigma_Default[REG_CLK_CTRL5_IC_1_Sigma_BYTE] = {
0x00, 0x00
};

/* Register Default - IC 1.CLK_CTRL7 */
ADI_REG_TYPE R12_CLK_CTRL7_IC_1_Sigma_Default[REG_CLK_CTRL7_IC_1_Sigma_BYTE] = {
0x00, 0x00
};

/* Register Default - IC 1.CLK_CTRL9 */
ADI_REG_TYPE R13_CLK_CTRL9_IC_1_Sigma_Default[REG_CLK_CTRL9_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.PLL_MB_PGA_PWR */
ADI_REG_TYPE R14_PLL_MB_PGA_PWR_IC_1_Sigma_Default[REG_PLL_MB_PGA_PWR_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.CLK_CTRL9 */
ADI_REG_TYPE R15_CLK_CTRL9_IC_1_Sigma_Default[REG_CLK_CTRL9_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.DSP_PWR */
ADI_REG_TYPE R16_DSP_PWR_IC_1_Sigma_Default[REG_DSP_PWR_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.DSP_PWR */
ADI_REG_TYPE R17_DSP_PWR_IC_1_Sigma_Default[REG_DSP_PWR_IC_1_Sigma_BYTE] = {
0x11
};

/* Register Default - IC 1.DMIC_PWR Register */
ADI_REG_TYPE R18_DMIC_PWR_IC_1_Sigma_Default[REG_DMIC_PWR_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SAI_CLK_PWR Register */
ADI_REG_TYPE R19_SAI_CLK_PWR_IC_1_Sigma_Default[REG_SAI_CLK_PWR_IC_1_Sigma_BYTE] = {
0x03
};

/* Register Default - IC 1.ASRC_PWR Register */
ADI_REG_TYPE R20_ASRC_PWR_IC_1_Sigma_Default[REG_ASRC_PWR_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_PWR Register */
ADI_REG_TYPE R21_FINT_PWR_IC_1_Sigma_Default[REG_FINT_PWR_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_PWR Register */
ADI_REG_TYPE R22_FDEC_PWR_IC_1_Sigma_Default[REG_FDEC_PWR_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.KEEPS Register */
ADI_REG_TYPE R23_KEEPS_IC_1_Sigma_Default[REG_KEEPS_IC_1_Sigma_BYTE] = {
0x10
};

/* Register Default - IC 1.ADC_CTRL1 Register */
ADI_REG_TYPE R24_ADC_CTRL1_IC_1_Sigma_Default[REG_ADC_CTRL1_IC_1_Sigma_BYTE] = {
0x22
};

/* Register Default - IC 1.ADC_CTRL2 Register */
ADI_REG_TYPE R25_ADC_CTRL2_IC_1_Sigma_Default[REG_ADC_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ADC_CTRL3 Register */
ADI_REG_TYPE R26_ADC_CTRL3_IC_1_Sigma_Default[REG_ADC_CTRL3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ADC_CTRL4 Register */
ADI_REG_TYPE R27_ADC_CTRL4_IC_1_Sigma_Default[REG_ADC_CTRL4_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.ADC_CTRL5 Register */
ADI_REG_TYPE R28_ADC_CTRL5_IC_1_Sigma_Default[REG_ADC_CTRL5_IC_1_Sigma_BYTE] = {
0x26
};

/* Register Default - IC 1.ADC_MUTES Register */
ADI_REG_TYPE R29_ADC_MUTES_IC_1_Sigma_Default[REG_ADC_MUTES_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ADC0_VOL Register */
ADI_REG_TYPE R30_ADC0_VOL_IC_1_Sigma_Default[REG_ADC0_VOL_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.ADC1_VOL Register */
ADI_REG_TYPE R31_ADC1_VOL_IC_1_Sigma_Default[REG_ADC1_VOL_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.ADC2_VOL Register */
ADI_REG_TYPE R32_ADC2_VOL_IC_1_Sigma_Default[REG_ADC2_VOL_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.ADC3_VOL Register */
ADI_REG_TYPE R33_ADC3_VOL_IC_1_Sigma_Default[REG_ADC3_VOL_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.PGA0_CTRL1 Register */
ADI_REG_TYPE R34_PGA0_CTRL1_IC_1_Sigma_Default[REG_PGA0_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA0_CTRL2 Register */
ADI_REG_TYPE R35_PGA0_CTRL2_IC_1_Sigma_Default[REG_PGA0_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA1_CTRL1 Register */
ADI_REG_TYPE R36_PGA1_CTRL1_IC_1_Sigma_Default[REG_PGA1_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA1_CTRL2 Register */
ADI_REG_TYPE R37_PGA1_CTRL2_IC_1_Sigma_Default[REG_PGA1_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA2_CTRL1 Register */
ADI_REG_TYPE R38_PGA2_CTRL1_IC_1_Sigma_Default[REG_PGA2_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA2_CTRL2 Register */
ADI_REG_TYPE R39_PGA2_CTRL2_IC_1_Sigma_Default[REG_PGA2_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA3_CTRL1 Register */
ADI_REG_TYPE R40_PGA3_CTRL1_IC_1_Sigma_Default[REG_PGA3_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA3_CTRL2 Register */
ADI_REG_TYPE R41_PGA3_CTRL2_IC_1_Sigma_Default[REG_PGA3_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PGA_CTRL Register */
ADI_REG_TYPE R42_PGA_CTRL_IC_1_Sigma_Default[REG_PGA_CTRL_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MBIAS_CTRL Register */
ADI_REG_TYPE R43_MBIAS_CTRL_IC_1_Sigma_Default[REG_MBIAS_CTRL_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.DMIC_CTRL1 Register */
ADI_REG_TYPE R44_DMIC_CTRL1_IC_1_Sigma_Default[REG_DMIC_CTRL1_IC_1_Sigma_BYTE] = {
0x33
};

/* Register Default - IC 1.DMIC_CTRL2 Register */
ADI_REG_TYPE R45_DMIC_CTRL2_IC_1_Sigma_Default[REG_DMIC_CTRL2_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.DMIC_CTRL3 Register */
ADI_REG_TYPE R46_DMIC_CTRL3_IC_1_Sigma_Default[REG_DMIC_CTRL3_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.DMIC_CTRL4 Register */
ADI_REG_TYPE R47_DMIC_CTRL4_IC_1_Sigma_Default[REG_DMIC_CTRL4_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.DMIC_CTRL5 Register */
ADI_REG_TYPE R48_DMIC_CTRL5_IC_1_Sigma_Default[REG_DMIC_CTRL5_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.DMIC_CTRL6 Register */
ADI_REG_TYPE R49_DMIC_CTRL6_IC_1_Sigma_Default[REG_DMIC_CTRL6_IC_1_Sigma_BYTE] = {
0x04
};

/* Register Default - IC 1.DMIC_MUTES Register */
ADI_REG_TYPE R50_DMIC_MUTES_IC_1_Sigma_Default[REG_DMIC_MUTES_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.DMIC_VOL0 Register */
ADI_REG_TYPE R51_DMIC_VOL0_IC_1_Sigma_Default[REG_DMIC_VOL0_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DMIC_VOL1 Register */
ADI_REG_TYPE R52_DMIC_VOL1_IC_1_Sigma_Default[REG_DMIC_VOL1_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DMIC_VOL2 Register */
ADI_REG_TYPE R53_DMIC_VOL2_IC_1_Sigma_Default[REG_DMIC_VOL2_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DMIC_VOL3 Register */
ADI_REG_TYPE R54_DMIC_VOL3_IC_1_Sigma_Default[REG_DMIC_VOL3_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DMIC_VOL4 Register */
ADI_REG_TYPE R55_DMIC_VOL4_IC_1_Sigma_Default[REG_DMIC_VOL4_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DMIC_VOL5 Register */
ADI_REG_TYPE R56_DMIC_VOL5_IC_1_Sigma_Default[REG_DMIC_VOL5_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DMIC_VOL6 Register */
ADI_REG_TYPE R57_DMIC_VOL6_IC_1_Sigma_Default[REG_DMIC_VOL6_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DMIC_VOL7 Register */
ADI_REG_TYPE R58_DMIC_VOL7_IC_1_Sigma_Default[REG_DMIC_VOL7_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.DAC_CTRL1 Register */
ADI_REG_TYPE R59_DAC_CTRL1_IC_1_Sigma_Default[REG_DAC_CTRL1_IC_1_Sigma_BYTE] = {
0x02
};

/* Register Default - IC 1.DAC_CTRL2 Register */
ADI_REG_TYPE R60_DAC_CTRL2_IC_1_Sigma_Default[REG_DAC_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.DAC_ROUTE0 Register */
ADI_REG_TYPE R61_DAC_ROUTE0_IC_1_Sigma_Default[REG_DAC_ROUTE0_IC_1_Sigma_BYTE] = {
0x30
};

/* Register Default - IC 1.DAC_ROUTE1 Register */
ADI_REG_TYPE R62_DAC_ROUTE1_IC_1_Sigma_Default[REG_DAC_ROUTE1_IC_1_Sigma_BYTE] = {
0x31
};

/* Register Default - IC 1.HP_CTRL Register */
ADI_REG_TYPE R63_HP_CTRL_IC_1_Sigma_Default[REG_HP_CTRL_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_CTRL1 Register */
ADI_REG_TYPE R64_FDEC_CTRL1_IC_1_Sigma_Default[REG_FDEC_CTRL1_IC_1_Sigma_BYTE] = {
0x25
};

/* Register Default - IC 1.FDEC_CTRL2 Register */
ADI_REG_TYPE R65_FDEC_CTRL2_IC_1_Sigma_Default[REG_FDEC_CTRL2_IC_1_Sigma_BYTE] = {
0x25
};

/* Register Default - IC 1.FDEC_CTRL3 Register */
ADI_REG_TYPE R66_FDEC_CTRL3_IC_1_Sigma_Default[REG_FDEC_CTRL3_IC_1_Sigma_BYTE] = {
0x25
};

/* Register Default - IC 1.FDEC_CTRL4 Register */
ADI_REG_TYPE R67_FDEC_CTRL4_IC_1_Sigma_Default[REG_FDEC_CTRL4_IC_1_Sigma_BYTE] = {
0x25
};

/* Register Default - IC 1.FDEC_ROUTE0 Register */
ADI_REG_TYPE R68_FDEC_ROUTE0_IC_1_Sigma_Default[REG_FDEC_ROUTE0_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_ROUTE1 Register */
ADI_REG_TYPE R69_FDEC_ROUTE1_IC_1_Sigma_Default[REG_FDEC_ROUTE1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_ROUTE2 Register */
ADI_REG_TYPE R70_FDEC_ROUTE2_IC_1_Sigma_Default[REG_FDEC_ROUTE2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_ROUTE3 Register */
ADI_REG_TYPE R71_FDEC_ROUTE3_IC_1_Sigma_Default[REG_FDEC_ROUTE3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_ROUTE4 Register */
ADI_REG_TYPE R72_FDEC_ROUTE4_IC_1_Sigma_Default[REG_FDEC_ROUTE4_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_ROUTE5 Register */
ADI_REG_TYPE R73_FDEC_ROUTE5_IC_1_Sigma_Default[REG_FDEC_ROUTE5_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_ROUTE6 Register */
ADI_REG_TYPE R74_FDEC_ROUTE6_IC_1_Sigma_Default[REG_FDEC_ROUTE6_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDEC_ROUTE7 Register */
ADI_REG_TYPE R75_FDEC_ROUTE7_IC_1_Sigma_Default[REG_FDEC_ROUTE7_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_CTRL1 Register */
ADI_REG_TYPE R76_FINT_CTRL1_IC_1_Sigma_Default[REG_FINT_CTRL1_IC_1_Sigma_BYTE] = {
0x52
};

/* Register Default - IC 1.FINT_CTRL2 Register */
ADI_REG_TYPE R77_FINT_CTRL2_IC_1_Sigma_Default[REG_FINT_CTRL2_IC_1_Sigma_BYTE] = {
0x52
};

/* Register Default - IC 1.FINT_CTRL3 Register */
ADI_REG_TYPE R78_FINT_CTRL3_IC_1_Sigma_Default[REG_FINT_CTRL3_IC_1_Sigma_BYTE] = {
0x52
};

/* Register Default - IC 1.FINT_CTRL4 Register */
ADI_REG_TYPE R79_FINT_CTRL4_IC_1_Sigma_Default[REG_FINT_CTRL4_IC_1_Sigma_BYTE] = {
0x52
};

/* Register Default - IC 1.FINT_ROUTE0 Register */
ADI_REG_TYPE R80_FINT_ROUTE0_IC_1_Sigma_Default[REG_FINT_ROUTE0_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_ROUTE1 Register */
ADI_REG_TYPE R81_FINT_ROUTE1_IC_1_Sigma_Default[REG_FINT_ROUTE1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_ROUTE2 Register */
ADI_REG_TYPE R82_FINT_ROUTE2_IC_1_Sigma_Default[REG_FINT_ROUTE2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_ROUTE3 Register */
ADI_REG_TYPE R83_FINT_ROUTE3_IC_1_Sigma_Default[REG_FINT_ROUTE3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_ROUTE4 Register */
ADI_REG_TYPE R84_FINT_ROUTE4_IC_1_Sigma_Default[REG_FINT_ROUTE4_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_ROUTE5 Register */
ADI_REG_TYPE R85_FINT_ROUTE5_IC_1_Sigma_Default[REG_FINT_ROUTE5_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_ROUTE6 Register */
ADI_REG_TYPE R86_FINT_ROUTE6_IC_1_Sigma_Default[REG_FINT_ROUTE6_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FINT_ROUTE7 Register */
ADI_REG_TYPE R87_FINT_ROUTE7_IC_1_Sigma_Default[REG_FINT_ROUTE7_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ASRCI_CTRL Register */
ADI_REG_TYPE R88_ASRCI_CTRL_IC_1_Sigma_Default[REG_ASRCI_CTRL_IC_1_Sigma_BYTE] = {
0x02
};

/* Register Default - IC 1.ASRCI_ROUTE01 Register */
ADI_REG_TYPE R89_ASRCI_ROUTE01_IC_1_Sigma_Default[REG_ASRCI_ROUTE01_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ASRCI_ROUTE23 Register */
ADI_REG_TYPE R90_ASRCI_ROUTE23_IC_1_Sigma_Default[REG_ASRCI_ROUTE23_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ASRCO_CTRL Register */
ADI_REG_TYPE R91_ASRCO_CTRL_IC_1_Sigma_Default[REG_ASRCO_CTRL_IC_1_Sigma_BYTE] = {
0x02
};

/* Register Default - IC 1.ASRCO_ROUTE0 Register */
ADI_REG_TYPE R92_ASRCO_ROUTE0_IC_1_Sigma_Default[REG_ASRCO_ROUTE0_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ASRCO_ROUTE1 Register */
ADI_REG_TYPE R93_ASRCO_ROUTE1_IC_1_Sigma_Default[REG_ASRCO_ROUTE1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ASRCO_ROUTE2 Register */
ADI_REG_TYPE R94_ASRCO_ROUTE2_IC_1_Sigma_Default[REG_ASRCO_ROUTE2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.ASRCO_ROUTE3 Register */
ADI_REG_TYPE R95_ASRCO_ROUTE3_IC_1_Sigma_Default[REG_ASRCO_ROUTE3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDSP_CTRL1 Register */
ADI_REG_TYPE R96_FDSP_CTRL1_IC_1_Sigma_Default[REG_FDSP_CTRL1_IC_1_Sigma_BYTE] = {
0x70
};

/* Register Default - IC 1.FDSP_CTRL2 Register */
ADI_REG_TYPE R97_FDSP_CTRL2_IC_1_Sigma_Default[REG_FDSP_CTRL2_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.FDSP_CTRL3 Register */
ADI_REG_TYPE R98_FDSP_CTRL3_IC_1_Sigma_Default[REG_FDSP_CTRL3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDSP_CTRL4 Register */
ADI_REG_TYPE R99_FDSP_CTRL4_IC_1_Sigma_Default[REG_FDSP_CTRL4_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDSP_CTRL5 Register */
#define R100_FDSP_CTRL5_REGISTER_IC_1_Sigma_SIZE 2
ADI_REG_TYPE R100_FDSP_CTRL5_REGISTER_IC_1_Sigma_Default[R100_FDSP_CTRL5_REGISTER_IC_1_Sigma_SIZE] = {
0x00, 0x7F
};

/* Register Default - IC 1.FDSP_CTRL7 Register */
ADI_REG_TYPE R101_FDSP_CTRL7_IC_1_Sigma_Default[REG_FDSP_CTRL7_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDSP_CTRL8 Register */
ADI_REG_TYPE R102_FDSP_CTRL8_IC_1_Sigma_Default[REG_FDSP_CTRL8_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDSP_SL_ADDR Register */
ADI_REG_TYPE R103_FDSP_SL_ADDR_IC_1_Sigma_Default[REG_FDSP_SL_ADDR_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FDSP_SL_P0 Register */
#define R104_FDSP_SL_P0_REGISTER_IC_1_Sigma_SIZE 4
ADI_REG_TYPE R104_FDSP_SL_P0_REGISTER_IC_1_Sigma_Default[R104_FDSP_SL_P0_REGISTER_IC_1_Sigma_SIZE] = {
0x00, 0x00, 0x00, 0x00
};

/* Register Default - IC 1.FDSP_SL_P1 Register */
#define R105_FDSP_SL_P1_REGISTER_IC_1_Sigma_SIZE 4
ADI_REG_TYPE R105_FDSP_SL_P1_REGISTER_IC_1_Sigma_Default[R105_FDSP_SL_P1_REGISTER_IC_1_Sigma_SIZE] = {
0x00, 0x00, 0x00, 0x00
};

/* Register Default - IC 1.FDSP_SL_P2 Register */
#define R106_FDSP_SL_P2_REGISTER_IC_1_Sigma_SIZE 4
ADI_REG_TYPE R106_FDSP_SL_P2_REGISTER_IC_1_Sigma_Default[R106_FDSP_SL_P2_REGISTER_IC_1_Sigma_SIZE] = {
0x00, 0x00, 0x00, 0x00
};

/* Register Default - IC 1.FDSP_SL_P3 Register */
#define R107_FDSP_SL_P3_REGISTER_IC_1_Sigma_SIZE 4
ADI_REG_TYPE R107_FDSP_SL_P3_REGISTER_IC_1_Sigma_Default[R107_FDSP_SL_P3_REGISTER_IC_1_Sigma_SIZE] = {
0x00, 0x00, 0x00, 0x00
};

/* Register Default - IC 1.FDSP_SL_P4 Register */
#define R108_FDSP_SL_P4_REGISTER_IC_1_Sigma_SIZE 4
ADI_REG_TYPE R108_FDSP_SL_P4_REGISTER_IC_1_Sigma_Default[R108_FDSP_SL_P4_REGISTER_IC_1_Sigma_SIZE] = {
0x00, 0x00, 0x00, 0x00
};

/* Register Default - IC 1.FDSP_SL_UPDATE Register */
ADI_REG_TYPE R109_FDSP_SL_UPDATE_IC_1_Sigma_Default[REG_FDSP_SL_UPDATE_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SDSP_CTRL1 Register */
ADI_REG_TYPE R110_SDSP_CTRL1_IC_1_Sigma_Default[REG_SDSP_CTRL1_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.SDSP_CTRL3 Register */
ADI_REG_TYPE R111_SDSP_CTRL3_IC_1_Sigma_Default[REG_SDSP_CTRL3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SDSP_CTRL4 Register */
#define R112_SDSP_CTRL4_REGISTER_IC_1_Sigma_SIZE 3
ADI_REG_TYPE R112_SDSP_CTRL4_REGISTER_IC_1_Sigma_Default[R112_SDSP_CTRL4_REGISTER_IC_1_Sigma_SIZE] = {
0x00, 0x00, 0x00
};

/* Register Default - IC 1.SDSP_CTRL7 Register */
#define R113_SDSP_CTRL7_REGISTER_IC_1_Sigma_SIZE 2
ADI_REG_TYPE R113_SDSP_CTRL7_REGISTER_IC_1_Sigma_Default[R113_SDSP_CTRL7_REGISTER_IC_1_Sigma_SIZE] = {
0x07, 0xF4
};

/* Register Default - IC 1.SDSP_CTRL9 Register */
#define R114_SDSP_CTRL9_REGISTER_IC_1_Sigma_SIZE 2
ADI_REG_TYPE R114_SDSP_CTRL9_REGISTER_IC_1_Sigma_Default[R114_SDSP_CTRL9_REGISTER_IC_1_Sigma_SIZE] = {
0x07, 0xFF
};

/* Register Default - IC 1.SDSP_CTRL11 Register */
ADI_REG_TYPE R115_SDSP_CTRL11_IC_1_Sigma_Default[REG_SDSP_CTRL11_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL1 Register */
ADI_REG_TYPE R116_MP_CTRL1_IC_1_Sigma_Default[REG_MP_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL2 Register */
ADI_REG_TYPE R117_MP_CTRL2_IC_1_Sigma_Default[REG_MP_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL3 Register */
ADI_REG_TYPE R118_MP_CTRL3_IC_1_Sigma_Default[REG_MP_CTRL3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL4 Register */
ADI_REG_TYPE R119_MP_CTRL4_IC_1_Sigma_Default[REG_MP_CTRL4_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL5 Register */
ADI_REG_TYPE R120_MP_CTRL5_IC_1_Sigma_Default[REG_MP_CTRL5_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL6 Register */
ADI_REG_TYPE R121_MP_CTRL6_IC_1_Sigma_Default[REG_MP_CTRL6_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL7 Register */
ADI_REG_TYPE R122_MP_CTRL7_IC_1_Sigma_Default[REG_MP_CTRL7_IC_1_Sigma_BYTE] = {
0x10
};

/* Register Default - IC 1.MP_CTRL8 Register */
ADI_REG_TYPE R123_MP_CTRL8_IC_1_Sigma_Default[REG_MP_CTRL8_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.MP_CTRL9 Register */
ADI_REG_TYPE R124_MP_CTRL9_IC_1_Sigma_Default[REG_MP_CTRL9_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.FSYNC0_CTRL Register */
ADI_REG_TYPE R125_FSYNC0_CTRL_IC_1_Sigma_Default[REG_FSYNC0_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.BCLK0_CTRL Register */
ADI_REG_TYPE R126_BCLK0_CTRL_IC_1_Sigma_Default[REG_BCLK0_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.SDATAO0_CTRL Register */
ADI_REG_TYPE R127_SDATAO0_CTRL_IC_1_Sigma_Default[REG_SDATAO0_CTRL_IC_1_Sigma_BYTE] = {
0x04
};

/* Register Default - IC 1.SDATAI0_CTRL Register */
ADI_REG_TYPE R128_SDATAI0_CTRL_IC_1_Sigma_Default[REG_SDATAI0_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.FSYNC1_CTRL Register */
ADI_REG_TYPE R129_FSYNC1_CTRL_IC_1_Sigma_Default[REG_FSYNC1_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.BCLK1_CTRL Register */
ADI_REG_TYPE R130_BCLK1_CTRL_IC_1_Sigma_Default[REG_BCLK1_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.SDATAO1_CTRL Register */
ADI_REG_TYPE R131_SDATAO1_CTRL_IC_1_Sigma_Default[REG_SDATAO1_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.SDATAI1_CTRL Register */
ADI_REG_TYPE R132_SDATAI1_CTRL_IC_1_Sigma_Default[REG_SDATAI1_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.DMIC_CLK0_CTRL Register */
ADI_REG_TYPE R133_DMIC_CLK0_CTRL_IC_1_Sigma_Default[REG_DMIC_CLK0_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.DMIC_CLK1_CTRL Register */
ADI_REG_TYPE R134_DMIC_CLK1_CTRL_IC_1_Sigma_Default[REG_DMIC_CLK1_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.DMIC01_CTRL Register */
ADI_REG_TYPE R135_DMIC01_CTRL_IC_1_Sigma_Default[REG_DMIC01_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.DMIC23_CTRL Register */
ADI_REG_TYPE R136_DMIC23_CTRL_IC_1_Sigma_Default[REG_DMIC23_CTRL_IC_1_Sigma_BYTE] = {
0x05
};

/* Register Default - IC 1.I2C_SPI_CTRL Register */
ADI_REG_TYPE R137_I2C_SPI_CTRL_IC_1_Sigma_Default[REG_I2C_SPI_CTRL_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ_CTRL1 Register */
ADI_REG_TYPE R138_IRQ_CTRL1_IC_1_Sigma_Default[REG_IRQ_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ1_MASK1 Register */
ADI_REG_TYPE R139_IRQ1_MASK1_IC_1_Sigma_Default[REG_IRQ1_MASK1_IC_1_Sigma_BYTE] = {
0xF3
};

/* Register Default - IC 1.IRQ1_MASK2 Register */
ADI_REG_TYPE R140_IRQ1_MASK2_IC_1_Sigma_Default[REG_IRQ1_MASK2_IC_1_Sigma_BYTE] = {
0xFF
};

/* Register Default - IC 1.IRQ1_MASK3 Register */
ADI_REG_TYPE R141_IRQ1_MASK3_IC_1_Sigma_Default[REG_IRQ1_MASK3_IC_1_Sigma_BYTE] = {
0x1F
};

/* Register Default - IC 1.IRQ2_MASK1 Register */
ADI_REG_TYPE R142_IRQ2_MASK1_IC_1_Sigma_Default[REG_IRQ2_MASK1_IC_1_Sigma_BYTE] = {
0xF3
};

/* Register Default - IC 1.IRQ2_MASK2 Register */
ADI_REG_TYPE R143_IRQ2_MASK2_IC_1_Sigma_Default[REG_IRQ2_MASK2_IC_1_Sigma_BYTE] = {
0xFF
};

/* Register Default - IC 1.IRQ2_MASK3 Register */
ADI_REG_TYPE R144_IRQ2_MASK3_IC_1_Sigma_Default[REG_IRQ2_MASK3_IC_1_Sigma_BYTE] = {
0x1F
};

/* Register Default - IC 1.RESETS Register */
ADI_REG_TYPE R145_RESETS_IC_1_Sigma_Default[REG_RESETS_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.READ_LAMBDA Register */
ADI_REG_TYPE R146_READ_LAMBDA_IC_1_Sigma_Default[REG_READ_LAMBDA_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.STATUS1 Register */
ADI_REG_TYPE R147_STATUS1_IC_1_Sigma_Default[REG_STATUS1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.STATUS2 Register */
ADI_REG_TYPE R148_STATUS2_IC_1_Sigma_Default[REG_STATUS2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.GPI1 Register */
ADI_REG_TYPE R149_GPI1_IC_1_Sigma_Default[REG_GPI1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.GPI2 Register */
ADI_REG_TYPE R150_GPI2_IC_1_Sigma_Default[REG_GPI2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.DSP_STATUS Register */
ADI_REG_TYPE R151_DSP_STATUS_IC_1_Sigma_Default[REG_DSP_STATUS_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ1_STATUS1 Register */
ADI_REG_TYPE R152_IRQ1_STATUS1_IC_1_Sigma_Default[REG_IRQ1_STATUS1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ1_STATUS2 Register */
ADI_REG_TYPE R153_IRQ1_STATUS2_IC_1_Sigma_Default[REG_IRQ1_STATUS2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ1_STATUS3 Register */
ADI_REG_TYPE R154_IRQ1_STATUS3_IC_1_Sigma_Default[REG_IRQ1_STATUS3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ2_STATUS1 Register */
ADI_REG_TYPE R155_IRQ2_STATUS1_IC_1_Sigma_Default[REG_IRQ2_STATUS1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ2_STATUS2 Register */
ADI_REG_TYPE R156_IRQ2_STATUS2_IC_1_Sigma_Default[REG_IRQ2_STATUS2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.IRQ2_STATUS3 Register */
ADI_REG_TYPE R157_IRQ2_STATUS3_IC_1_Sigma_Default[REG_IRQ2_STATUS3_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SPT0_CTRL1 Register */
ADI_REG_TYPE R158_SPT0_CTRL1_IC_1_Sigma_Default[REG_SPT0_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SPT0_CTRL2 Register */
ADI_REG_TYPE R159_SPT0_CTRL2_IC_1_Sigma_Default[REG_SPT0_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SPT0_ROUTE0 Register */
ADI_REG_TYPE R160_SPT0_ROUTE0_IC_1_Sigma_Default[REG_SPT0_ROUTE0_IC_1_Sigma_BYTE] = {
0x10
};

/* Register Default - IC 1.SPT0_ROUTE1 Register */
ADI_REG_TYPE R161_SPT0_ROUTE1_IC_1_Sigma_Default[REG_SPT0_ROUTE1_IC_1_Sigma_BYTE] = {
0x11
};

/* Register Default - IC 1.SPT0_ROUTE2 Register */
ADI_REG_TYPE R162_SPT0_ROUTE2_IC_1_Sigma_Default[REG_SPT0_ROUTE2_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE3 Register */
ADI_REG_TYPE R163_SPT0_ROUTE3_IC_1_Sigma_Default[REG_SPT0_ROUTE3_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE4 Register */
ADI_REG_TYPE R164_SPT0_ROUTE4_IC_1_Sigma_Default[REG_SPT0_ROUTE4_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE5 Register */
ADI_REG_TYPE R165_SPT0_ROUTE5_IC_1_Sigma_Default[REG_SPT0_ROUTE5_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE6 Register */
ADI_REG_TYPE R166_SPT0_ROUTE6_IC_1_Sigma_Default[REG_SPT0_ROUTE6_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE7 Register */
ADI_REG_TYPE R167_SPT0_ROUTE7_IC_1_Sigma_Default[REG_SPT0_ROUTE7_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE8 Register */
ADI_REG_TYPE R168_SPT0_ROUTE8_IC_1_Sigma_Default[REG_SPT0_ROUTE8_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE9 Register */
ADI_REG_TYPE R169_SPT0_ROUTE9_IC_1_Sigma_Default[REG_SPT0_ROUTE9_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE10 Register */
ADI_REG_TYPE R170_SPT0_ROUTE10_IC_1_Sigma_Default[REG_SPT0_ROUTE10_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE11 Register */
ADI_REG_TYPE R171_SPT0_ROUTE11_IC_1_Sigma_Default[REG_SPT0_ROUTE11_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE12 Register */
ADI_REG_TYPE R172_SPT0_ROUTE12_IC_1_Sigma_Default[REG_SPT0_ROUTE12_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE13 Register */
ADI_REG_TYPE R173_SPT0_ROUTE13_IC_1_Sigma_Default[REG_SPT0_ROUTE13_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE14 Register */
ADI_REG_TYPE R174_SPT0_ROUTE14_IC_1_Sigma_Default[REG_SPT0_ROUTE14_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT0_ROUTE15 Register */
ADI_REG_TYPE R175_SPT0_ROUTE15_IC_1_Sigma_Default[REG_SPT0_ROUTE15_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_CTRL1 Register */
ADI_REG_TYPE R176_SPT1_CTRL1_IC_1_Sigma_Default[REG_SPT1_CTRL1_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SPT1_CTRL2 Register */
ADI_REG_TYPE R177_SPT1_CTRL2_IC_1_Sigma_Default[REG_SPT1_CTRL2_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SPT1_ROUTE0 Register */
ADI_REG_TYPE R178_SPT1_ROUTE0_IC_1_Sigma_Default[REG_SPT1_ROUTE0_IC_1_Sigma_BYTE] = {
0x10
};

/* Register Default - IC 1.SPT1_ROUTE1 Register */
ADI_REG_TYPE R179_SPT1_ROUTE1_IC_1_Sigma_Default[REG_SPT1_ROUTE1_IC_1_Sigma_BYTE] = {
0x11
};

/* Register Default - IC 1.SPT1_ROUTE2 Register */
ADI_REG_TYPE R180_SPT1_ROUTE2_IC_1_Sigma_Default[REG_SPT1_ROUTE2_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE3 Register */
ADI_REG_TYPE R181_SPT1_ROUTE3_IC_1_Sigma_Default[REG_SPT1_ROUTE3_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE4 Register */
ADI_REG_TYPE R182_SPT1_ROUTE4_IC_1_Sigma_Default[REG_SPT1_ROUTE4_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE5 Register */
ADI_REG_TYPE R183_SPT1_ROUTE5_IC_1_Sigma_Default[REG_SPT1_ROUTE5_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE6 Register */
ADI_REG_TYPE R184_SPT1_ROUTE6_IC_1_Sigma_Default[REG_SPT1_ROUTE6_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE7 Register */
ADI_REG_TYPE R185_SPT1_ROUTE7_IC_1_Sigma_Default[REG_SPT1_ROUTE7_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE8 Register */
ADI_REG_TYPE R186_SPT1_ROUTE8_IC_1_Sigma_Default[REG_SPT1_ROUTE8_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE9 Register */
ADI_REG_TYPE R187_SPT1_ROUTE9_IC_1_Sigma_Default[REG_SPT1_ROUTE9_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE10 Register */
ADI_REG_TYPE R188_SPT1_ROUTE10_IC_1_Sigma_Default[REG_SPT1_ROUTE10_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE11 Register */
ADI_REG_TYPE R189_SPT1_ROUTE11_IC_1_Sigma_Default[REG_SPT1_ROUTE11_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE12 Register */
ADI_REG_TYPE R190_SPT1_ROUTE12_IC_1_Sigma_Default[REG_SPT1_ROUTE12_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE13 Register */
ADI_REG_TYPE R191_SPT1_ROUTE13_IC_1_Sigma_Default[REG_SPT1_ROUTE13_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE14 Register */
ADI_REG_TYPE R192_SPT1_ROUTE14_IC_1_Sigma_Default[REG_SPT1_ROUTE14_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.SPT1_ROUTE15 Register */
ADI_REG_TYPE R193_SPT1_ROUTE15_IC_1_Sigma_Default[REG_SPT1_ROUTE15_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.MP_CTRL10 Register */
ADI_REG_TYPE R194_MP_CTRL10_IC_1_Sigma_Default[REG_MP_CTRL10_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.SELFBOOT_CTRL Register */
ADI_REG_TYPE R195_SELFBOOT_CTRL_IC_1_Sigma_Default[REG_SELFBOOT_CTRL_IC_1_Sigma_BYTE] = {
0x41
};

/* Register Default - IC 1.SW_EN_CTRL Register */
ADI_REG_TYPE R196_SW_EN_CTRL_IC_1_Sigma_Default[REG_SW_EN_CTRL_IC_1_Sigma_BYTE] = {
0x41
};

/* Register Default - IC 1.PDM_CTRL1 Register */
ADI_REG_TYPE R197_PDM_CTRL1_IC_1_Sigma_Default[REG_PDM_CTRL1_IC_1_Sigma_BYTE] = {
0x02
};

/* Register Default - IC 1.PDM_CTRL2 Register */
ADI_REG_TYPE R198_PDM_CTRL2_IC_1_Sigma_Default[REG_PDM_CTRL2_IC_1_Sigma_BYTE] = {
0xC4
};

/* Register Default - IC 1.PDM_VOL0 Register */
ADI_REG_TYPE R199_PDM_VOL0_IC_1_Sigma_Default[REG_PDM_VOL0_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.PDM_VOL1 Register */
ADI_REG_TYPE R200_PDM_VOL1_IC_1_Sigma_Default[REG_PDM_VOL1_IC_1_Sigma_BYTE] = {
0x40
};

/* Register Default - IC 1.PDM_ROUTE0 Register */
ADI_REG_TYPE R201_PDM_ROUTE0_IC_1_Sigma_Default[REG_PDM_ROUTE0_IC_1_Sigma_BYTE] = {
0x00
};

/* Register Default - IC 1.PDM_ROUTE1 Register */
ADI_REG_TYPE R202_PDM_ROUTE1_IC_1_Sigma_Default[REG_PDM_ROUTE1_IC_1_Sigma_BYTE] = {
0x01
};

/* Register Default - IC 1.ADC_DAC_HP_PWR */
ADI_REG_TYPE R206_ADC_DAC_HP_PWR_IC_1_Sigma_Default[REG_ADC_DAC_HP_PWR_IC_1_Sigma_BYTE] = {
0x3F
};

/* Register Default - IC 1.DAC_VOL0 */
ADI_REG_TYPE R207_DAC_VOL0_IC_1_Sigma_Default[REG_DAC_VOL0_IC_1_Sigma_BYTE] = {
0x50
};

/* Register Default - IC 1.DAC_VOL1 */
ADI_REG_TYPE R208_DAC_VOL1_IC_1_Sigma_Default[REG_DAC_VOL1_IC_1_Sigma_BYTE] = {
0x50
};

/* Register Default - IC 1.SDSP_CTRL2 */
ADI_REG_TYPE R209_SDSP_CTRL2_IC_1_Sigma_Default[REG_SDSP_CTRL2_IC_1_Sigma_BYTE] = {
0x01
};


/*
 * Default Download
 */
#define DEFAULT_DOWNLOAD_SIZE_IC_1_Sigma 210

void default_download_IC_1_Sigma() {
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL2_IC_1_Sigma_ADDR, REG_SDSP_CTRL2_IC_1_Sigma_BYTE, R0_SDSP_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_VOL0_IC_1_Sigma_ADDR, REG_DAC_VOL0_IC_1_Sigma_BYTE, R1_DAC_VOL0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_VOL1_IC_1_Sigma_ADDR, REG_DAC_VOL1_IC_1_Sigma_BYTE, R2_DAC_VOL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_DAC_HP_PWR_IC_1_Sigma_ADDR, REG_ADC_DAC_HP_PWR_IC_1_Sigma_BYTE, R3_ADC_DAC_HP_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PLL_MB_PGA_PWR_IC_1_Sigma_ADDR, REG_PLL_MB_PGA_PWR_IC_1_Sigma_BYTE, R4_PLL_MB_PGA_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CHIP_PWR_IC_1_Sigma_ADDR, REG_CHIP_PWR_IC_1_Sigma_BYTE, R5_CHIP_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_DELAY( DEVICE_ADDR_IC_1_SIGMA, REG_POWER_EN_DELAY_IC_1_Sigma_BYTE, R6_POWER_EN_DELAY_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CHIP_PWR_IC_1_Sigma_ADDR, REG_CHIP_PWR_IC_1_Sigma_BYTE, R7_CHIP_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CLK_CTRL1_IC_1_Sigma_ADDR, REG_CLK_CTRL1_IC_1_Sigma_BYTE, R8_CLK_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CLK_CTRL2_IC_1_Sigma_ADDR, REG_CLK_CTRL2_IC_1_Sigma_BYTE, R9_CLK_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CLK_CTRL3_IC_1_Sigma_ADDR, REG_CLK_CTRL3_IC_1_Sigma_BYTE, R10_CLK_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CLK_CTRL5_IC_1_Sigma_ADDR, REG_CLK_CTRL5_IC_1_Sigma_BYTE, R11_CLK_CTRL5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CLK_CTRL7_IC_1_Sigma_ADDR, REG_CLK_CTRL7_IC_1_Sigma_BYTE, R12_CLK_CTRL7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CLK_CTRL9_IC_1_Sigma_ADDR, REG_CLK_CTRL9_IC_1_Sigma_BYTE, R13_CLK_CTRL9_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PLL_MB_PGA_PWR_IC_1_Sigma_ADDR, REG_PLL_MB_PGA_PWR_IC_1_Sigma_BYTE, R14_PLL_MB_PGA_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_CLK_CTRL9_IC_1_Sigma_ADDR, REG_CLK_CTRL9_IC_1_Sigma_BYTE, R15_CLK_CTRL9_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DSP_PWR_IC_1_Sigma_ADDR, REG_DSP_PWR_IC_1_Sigma_BYTE, R16_DSP_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DSP_PWR_IC_1_Sigma_ADDR, REG_DSP_PWR_IC_1_Sigma_BYTE, R17_DSP_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_PWR_IC_1_Sigma_ADDR, REG_DMIC_PWR_IC_1_Sigma_BYTE, R18_DMIC_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SAI_CLK_PWR_IC_1_Sigma_ADDR, REG_SAI_CLK_PWR_IC_1_Sigma_BYTE, R19_SAI_CLK_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRC_PWR_IC_1_Sigma_ADDR, REG_ASRC_PWR_IC_1_Sigma_BYTE, R20_ASRC_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_PWR_IC_1_Sigma_ADDR, REG_FINT_PWR_IC_1_Sigma_BYTE, R21_FINT_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_PWR_IC_1_Sigma_ADDR, REG_FDEC_PWR_IC_1_Sigma_BYTE, R22_FDEC_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_KEEPS_IC_1_Sigma_ADDR, REG_KEEPS_IC_1_Sigma_BYTE, R23_KEEPS_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_CTRL1_IC_1_Sigma_ADDR, REG_ADC_CTRL1_IC_1_Sigma_BYTE, R24_ADC_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_CTRL2_IC_1_Sigma_ADDR, REG_ADC_CTRL2_IC_1_Sigma_BYTE, R25_ADC_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_CTRL3_IC_1_Sigma_ADDR, REG_ADC_CTRL3_IC_1_Sigma_BYTE, R26_ADC_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_CTRL4_IC_1_Sigma_ADDR, REG_ADC_CTRL4_IC_1_Sigma_BYTE, R27_ADC_CTRL4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_CTRL5_IC_1_Sigma_ADDR, REG_ADC_CTRL5_IC_1_Sigma_BYTE, R28_ADC_CTRL5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_MUTES_IC_1_Sigma_ADDR, REG_ADC_MUTES_IC_1_Sigma_BYTE, R29_ADC_MUTES_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC0_VOL_IC_1_Sigma_ADDR, REG_ADC0_VOL_IC_1_Sigma_BYTE, R30_ADC0_VOL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC1_VOL_IC_1_Sigma_ADDR, REG_ADC1_VOL_IC_1_Sigma_BYTE, R31_ADC1_VOL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC2_VOL_IC_1_Sigma_ADDR, REG_ADC2_VOL_IC_1_Sigma_BYTE, R32_ADC2_VOL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC3_VOL_IC_1_Sigma_ADDR, REG_ADC3_VOL_IC_1_Sigma_BYTE, R33_ADC3_VOL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA0_CTRL1_IC_1_Sigma_ADDR, REG_PGA0_CTRL1_IC_1_Sigma_BYTE, R34_PGA0_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA0_CTRL2_IC_1_Sigma_ADDR, REG_PGA0_CTRL2_IC_1_Sigma_BYTE, R35_PGA0_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA1_CTRL1_IC_1_Sigma_ADDR, REG_PGA1_CTRL1_IC_1_Sigma_BYTE, R36_PGA1_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA1_CTRL2_IC_1_Sigma_ADDR, REG_PGA1_CTRL2_IC_1_Sigma_BYTE, R37_PGA1_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA2_CTRL1_IC_1_Sigma_ADDR, REG_PGA2_CTRL1_IC_1_Sigma_BYTE, R38_PGA2_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA2_CTRL2_IC_1_Sigma_ADDR, REG_PGA2_CTRL2_IC_1_Sigma_BYTE, R39_PGA2_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA3_CTRL1_IC_1_Sigma_ADDR, REG_PGA3_CTRL1_IC_1_Sigma_BYTE, R40_PGA3_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA3_CTRL2_IC_1_Sigma_ADDR, REG_PGA3_CTRL2_IC_1_Sigma_BYTE, R41_PGA3_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PGA_CTRL_IC_1_Sigma_ADDR, REG_PGA_CTRL_IC_1_Sigma_BYTE, R42_PGA_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MBIAS_CTRL_IC_1_Sigma_ADDR, REG_MBIAS_CTRL_IC_1_Sigma_BYTE, R43_MBIAS_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CTRL1_IC_1_Sigma_ADDR, REG_DMIC_CTRL1_IC_1_Sigma_BYTE, R44_DMIC_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CTRL2_IC_1_Sigma_ADDR, REG_DMIC_CTRL2_IC_1_Sigma_BYTE, R45_DMIC_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CTRL3_IC_1_Sigma_ADDR, REG_DMIC_CTRL3_IC_1_Sigma_BYTE, R46_DMIC_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CTRL4_IC_1_Sigma_ADDR, REG_DMIC_CTRL4_IC_1_Sigma_BYTE, R47_DMIC_CTRL4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CTRL5_IC_1_Sigma_ADDR, REG_DMIC_CTRL5_IC_1_Sigma_BYTE, R48_DMIC_CTRL5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CTRL6_IC_1_Sigma_ADDR, REG_DMIC_CTRL6_IC_1_Sigma_BYTE, R49_DMIC_CTRL6_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_MUTES_IC_1_Sigma_ADDR, REG_DMIC_MUTES_IC_1_Sigma_BYTE, R50_DMIC_MUTES_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL0_IC_1_Sigma_ADDR, REG_DMIC_VOL0_IC_1_Sigma_BYTE, R51_DMIC_VOL0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL1_IC_1_Sigma_ADDR, REG_DMIC_VOL1_IC_1_Sigma_BYTE, R52_DMIC_VOL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL2_IC_1_Sigma_ADDR, REG_DMIC_VOL2_IC_1_Sigma_BYTE, R53_DMIC_VOL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL3_IC_1_Sigma_ADDR, REG_DMIC_VOL3_IC_1_Sigma_BYTE, R54_DMIC_VOL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL4_IC_1_Sigma_ADDR, REG_DMIC_VOL4_IC_1_Sigma_BYTE, R55_DMIC_VOL4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL5_IC_1_Sigma_ADDR, REG_DMIC_VOL5_IC_1_Sigma_BYTE, R56_DMIC_VOL5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL6_IC_1_Sigma_ADDR, REG_DMIC_VOL6_IC_1_Sigma_BYTE, R57_DMIC_VOL6_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_VOL7_IC_1_Sigma_ADDR, REG_DMIC_VOL7_IC_1_Sigma_BYTE, R58_DMIC_VOL7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_CTRL1_IC_1_Sigma_ADDR, REG_DAC_CTRL1_IC_1_Sigma_BYTE, R59_DAC_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_CTRL2_IC_1_Sigma_ADDR, REG_DAC_CTRL2_IC_1_Sigma_BYTE, R60_DAC_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_ROUTE0_IC_1_Sigma_ADDR, REG_DAC_ROUTE0_IC_1_Sigma_BYTE, R61_DAC_ROUTE0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_ROUTE1_IC_1_Sigma_ADDR, REG_DAC_ROUTE1_IC_1_Sigma_BYTE, R62_DAC_ROUTE1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_HP_CTRL_IC_1_Sigma_ADDR, REG_HP_CTRL_IC_1_Sigma_BYTE, R63_HP_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_CTRL1_IC_1_Sigma_ADDR, REG_FDEC_CTRL1_IC_1_Sigma_BYTE, R64_FDEC_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_CTRL2_IC_1_Sigma_ADDR, REG_FDEC_CTRL2_IC_1_Sigma_BYTE, R65_FDEC_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_CTRL3_IC_1_Sigma_ADDR, REG_FDEC_CTRL3_IC_1_Sigma_BYTE, R66_FDEC_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_CTRL4_IC_1_Sigma_ADDR, REG_FDEC_CTRL4_IC_1_Sigma_BYTE, R67_FDEC_CTRL4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE0_IC_1_Sigma_ADDR, REG_FDEC_ROUTE0_IC_1_Sigma_BYTE, R68_FDEC_ROUTE0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE1_IC_1_Sigma_ADDR, REG_FDEC_ROUTE1_IC_1_Sigma_BYTE, R69_FDEC_ROUTE1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE2_IC_1_Sigma_ADDR, REG_FDEC_ROUTE2_IC_1_Sigma_BYTE, R70_FDEC_ROUTE2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE3_IC_1_Sigma_ADDR, REG_FDEC_ROUTE3_IC_1_Sigma_BYTE, R71_FDEC_ROUTE3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE4_IC_1_Sigma_ADDR, REG_FDEC_ROUTE4_IC_1_Sigma_BYTE, R72_FDEC_ROUTE4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE5_IC_1_Sigma_ADDR, REG_FDEC_ROUTE5_IC_1_Sigma_BYTE, R73_FDEC_ROUTE5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE6_IC_1_Sigma_ADDR, REG_FDEC_ROUTE6_IC_1_Sigma_BYTE, R74_FDEC_ROUTE6_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDEC_ROUTE7_IC_1_Sigma_ADDR, REG_FDEC_ROUTE7_IC_1_Sigma_BYTE, R75_FDEC_ROUTE7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_CTRL1_IC_1_Sigma_ADDR, REG_FINT_CTRL1_IC_1_Sigma_BYTE, R76_FINT_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_CTRL2_IC_1_Sigma_ADDR, REG_FINT_CTRL2_IC_1_Sigma_BYTE, R77_FINT_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_CTRL3_IC_1_Sigma_ADDR, REG_FINT_CTRL3_IC_1_Sigma_BYTE, R78_FINT_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_CTRL4_IC_1_Sigma_ADDR, REG_FINT_CTRL4_IC_1_Sigma_BYTE, R79_FINT_CTRL4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE0_IC_1_Sigma_ADDR, REG_FINT_ROUTE0_IC_1_Sigma_BYTE, R80_FINT_ROUTE0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE1_IC_1_Sigma_ADDR, REG_FINT_ROUTE1_IC_1_Sigma_BYTE, R81_FINT_ROUTE1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE2_IC_1_Sigma_ADDR, REG_FINT_ROUTE2_IC_1_Sigma_BYTE, R82_FINT_ROUTE2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE3_IC_1_Sigma_ADDR, REG_FINT_ROUTE3_IC_1_Sigma_BYTE, R83_FINT_ROUTE3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE4_IC_1_Sigma_ADDR, REG_FINT_ROUTE4_IC_1_Sigma_BYTE, R84_FINT_ROUTE4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE5_IC_1_Sigma_ADDR, REG_FINT_ROUTE5_IC_1_Sigma_BYTE, R85_FINT_ROUTE5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE6_IC_1_Sigma_ADDR, REG_FINT_ROUTE6_IC_1_Sigma_BYTE, R86_FINT_ROUTE6_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FINT_ROUTE7_IC_1_Sigma_ADDR, REG_FINT_ROUTE7_IC_1_Sigma_BYTE, R87_FINT_ROUTE7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCI_CTRL_IC_1_Sigma_ADDR, REG_ASRCI_CTRL_IC_1_Sigma_BYTE, R88_ASRCI_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCI_ROUTE01_IC_1_Sigma_ADDR, REG_ASRCI_ROUTE01_IC_1_Sigma_BYTE, R89_ASRCI_ROUTE01_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCI_ROUTE23_IC_1_Sigma_ADDR, REG_ASRCI_ROUTE23_IC_1_Sigma_BYTE, R90_ASRCI_ROUTE23_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCO_CTRL_IC_1_Sigma_ADDR, REG_ASRCO_CTRL_IC_1_Sigma_BYTE, R91_ASRCO_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCO_ROUTE0_IC_1_Sigma_ADDR, REG_ASRCO_ROUTE0_IC_1_Sigma_BYTE, R92_ASRCO_ROUTE0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCO_ROUTE1_IC_1_Sigma_ADDR, REG_ASRCO_ROUTE1_IC_1_Sigma_BYTE, R93_ASRCO_ROUTE1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCO_ROUTE2_IC_1_Sigma_ADDR, REG_ASRCO_ROUTE2_IC_1_Sigma_BYTE, R94_ASRCO_ROUTE2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ASRCO_ROUTE3_IC_1_Sigma_ADDR, REG_ASRCO_ROUTE3_IC_1_Sigma_BYTE, R95_ASRCO_ROUTE3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_CTRL1_IC_1_Sigma_ADDR, REG_FDSP_CTRL1_IC_1_Sigma_BYTE, R96_FDSP_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_CTRL2_IC_1_Sigma_ADDR, REG_FDSP_CTRL2_IC_1_Sigma_BYTE, R97_FDSP_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_CTRL3_IC_1_Sigma_ADDR, REG_FDSP_CTRL3_IC_1_Sigma_BYTE, R98_FDSP_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_CTRL4_IC_1_Sigma_ADDR, REG_FDSP_CTRL4_IC_1_Sigma_BYTE, R99_FDSP_CTRL4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_CTRL5_IC_1_Sigma_ADDR , R100_FDSP_CTRL5_REGISTER_IC_1_Sigma_SIZE, R100_FDSP_CTRL5_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_CTRL7_IC_1_Sigma_ADDR, REG_FDSP_CTRL7_IC_1_Sigma_BYTE, R101_FDSP_CTRL7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_CTRL8_IC_1_Sigma_ADDR, REG_FDSP_CTRL8_IC_1_Sigma_BYTE, R102_FDSP_CTRL8_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_SL_ADDR_IC_1_Sigma_ADDR, REG_FDSP_SL_ADDR_IC_1_Sigma_BYTE, R103_FDSP_SL_ADDR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_SL_P0_IC_1_Sigma_ADDR , R104_FDSP_SL_P0_REGISTER_IC_1_Sigma_SIZE, R104_FDSP_SL_P0_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_SL_P1_IC_1_Sigma_ADDR , R105_FDSP_SL_P1_REGISTER_IC_1_Sigma_SIZE, R105_FDSP_SL_P1_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_SL_P2_IC_1_Sigma_ADDR , R106_FDSP_SL_P2_REGISTER_IC_1_Sigma_SIZE, R106_FDSP_SL_P2_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_SL_P3_IC_1_Sigma_ADDR , R107_FDSP_SL_P3_REGISTER_IC_1_Sigma_SIZE, R107_FDSP_SL_P3_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_SL_P4_IC_1_Sigma_ADDR , R108_FDSP_SL_P4_REGISTER_IC_1_Sigma_SIZE, R108_FDSP_SL_P4_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FDSP_SL_UPDATE_IC_1_Sigma_ADDR, REG_FDSP_SL_UPDATE_IC_1_Sigma_BYTE, R109_FDSP_SL_UPDATE_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL1_IC_1_Sigma_ADDR, REG_SDSP_CTRL1_IC_1_Sigma_BYTE, R110_SDSP_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL3_IC_1_Sigma_ADDR, REG_SDSP_CTRL3_IC_1_Sigma_BYTE, R111_SDSP_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL4_IC_1_Sigma_ADDR , R112_SDSP_CTRL4_REGISTER_IC_1_Sigma_SIZE, R112_SDSP_CTRL4_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL7_IC_1_Sigma_ADDR , R113_SDSP_CTRL7_REGISTER_IC_1_Sigma_SIZE, R113_SDSP_CTRL7_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL9_IC_1_Sigma_ADDR , R114_SDSP_CTRL9_REGISTER_IC_1_Sigma_SIZE, R114_SDSP_CTRL9_REGISTER_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL11_IC_1_Sigma_ADDR, REG_SDSP_CTRL11_IC_1_Sigma_BYTE, R115_SDSP_CTRL11_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL1_IC_1_Sigma_ADDR, REG_MP_CTRL1_IC_1_Sigma_BYTE, R116_MP_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL2_IC_1_Sigma_ADDR, REG_MP_CTRL2_IC_1_Sigma_BYTE, R117_MP_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL3_IC_1_Sigma_ADDR, REG_MP_CTRL3_IC_1_Sigma_BYTE, R118_MP_CTRL3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL4_IC_1_Sigma_ADDR, REG_MP_CTRL4_IC_1_Sigma_BYTE, R119_MP_CTRL4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL5_IC_1_Sigma_ADDR, REG_MP_CTRL5_IC_1_Sigma_BYTE, R120_MP_CTRL5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL6_IC_1_Sigma_ADDR, REG_MP_CTRL6_IC_1_Sigma_BYTE, R121_MP_CTRL6_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL7_IC_1_Sigma_ADDR, REG_MP_CTRL7_IC_1_Sigma_BYTE, R122_MP_CTRL7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL8_IC_1_Sigma_ADDR, REG_MP_CTRL8_IC_1_Sigma_BYTE, R123_MP_CTRL8_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL9_IC_1_Sigma_ADDR, REG_MP_CTRL9_IC_1_Sigma_BYTE, R124_MP_CTRL9_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FSYNC0_CTRL_IC_1_Sigma_ADDR, REG_FSYNC0_CTRL_IC_1_Sigma_BYTE, R125_FSYNC0_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_BCLK0_CTRL_IC_1_Sigma_ADDR, REG_BCLK0_CTRL_IC_1_Sigma_BYTE, R126_BCLK0_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDATAO0_CTRL_IC_1_Sigma_ADDR, REG_SDATAO0_CTRL_IC_1_Sigma_BYTE, R127_SDATAO0_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDATAI0_CTRL_IC_1_Sigma_ADDR, REG_SDATAI0_CTRL_IC_1_Sigma_BYTE, R128_SDATAI0_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_FSYNC1_CTRL_IC_1_Sigma_ADDR, REG_FSYNC1_CTRL_IC_1_Sigma_BYTE, R129_FSYNC1_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_BCLK1_CTRL_IC_1_Sigma_ADDR, REG_BCLK1_CTRL_IC_1_Sigma_BYTE, R130_BCLK1_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDATAO1_CTRL_IC_1_Sigma_ADDR, REG_SDATAO1_CTRL_IC_1_Sigma_BYTE, R131_SDATAO1_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDATAI1_CTRL_IC_1_Sigma_ADDR, REG_SDATAI1_CTRL_IC_1_Sigma_BYTE, R132_SDATAI1_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CLK0_CTRL_IC_1_Sigma_ADDR, REG_DMIC_CLK0_CTRL_IC_1_Sigma_BYTE, R133_DMIC_CLK0_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC_CLK1_CTRL_IC_1_Sigma_ADDR, REG_DMIC_CLK1_CTRL_IC_1_Sigma_BYTE, R134_DMIC_CLK1_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC01_CTRL_IC_1_Sigma_ADDR, REG_DMIC01_CTRL_IC_1_Sigma_BYTE, R135_DMIC01_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DMIC23_CTRL_IC_1_Sigma_ADDR, REG_DMIC23_CTRL_IC_1_Sigma_BYTE, R136_DMIC23_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_I2C_SPI_CTRL_IC_1_Sigma_ADDR, REG_I2C_SPI_CTRL_IC_1_Sigma_BYTE, R137_I2C_SPI_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ_CTRL1_IC_1_Sigma_ADDR, REG_IRQ_CTRL1_IC_1_Sigma_BYTE, R138_IRQ_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ1_MASK1_IC_1_Sigma_ADDR, REG_IRQ1_MASK1_IC_1_Sigma_BYTE, R139_IRQ1_MASK1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ1_MASK2_IC_1_Sigma_ADDR, REG_IRQ1_MASK2_IC_1_Sigma_BYTE, R140_IRQ1_MASK2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ1_MASK3_IC_1_Sigma_ADDR, REG_IRQ1_MASK3_IC_1_Sigma_BYTE, R141_IRQ1_MASK3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ2_MASK1_IC_1_Sigma_ADDR, REG_IRQ2_MASK1_IC_1_Sigma_BYTE, R142_IRQ2_MASK1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ2_MASK2_IC_1_Sigma_ADDR, REG_IRQ2_MASK2_IC_1_Sigma_BYTE, R143_IRQ2_MASK2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ2_MASK3_IC_1_Sigma_ADDR, REG_IRQ2_MASK3_IC_1_Sigma_BYTE, R144_IRQ2_MASK3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_RESETS_IC_1_Sigma_ADDR, REG_RESETS_IC_1_Sigma_BYTE, R145_RESETS_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_READ_LAMBDA_IC_1_Sigma_ADDR, REG_READ_LAMBDA_IC_1_Sigma_BYTE, R146_READ_LAMBDA_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_STATUS1_IC_1_Sigma_ADDR, REG_STATUS1_IC_1_Sigma_BYTE, R147_STATUS1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_STATUS2_IC_1_Sigma_ADDR, REG_STATUS2_IC_1_Sigma_BYTE, R148_STATUS2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_GPI1_IC_1_Sigma_ADDR, REG_GPI1_IC_1_Sigma_BYTE, R149_GPI1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_GPI2_IC_1_Sigma_ADDR, REG_GPI2_IC_1_Sigma_BYTE, R150_GPI2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DSP_STATUS_IC_1_Sigma_ADDR, REG_DSP_STATUS_IC_1_Sigma_BYTE, R151_DSP_STATUS_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ1_STATUS1_IC_1_Sigma_ADDR, REG_IRQ1_STATUS1_IC_1_Sigma_BYTE, R152_IRQ1_STATUS1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ1_STATUS2_IC_1_Sigma_ADDR, REG_IRQ1_STATUS2_IC_1_Sigma_BYTE, R153_IRQ1_STATUS2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ1_STATUS3_IC_1_Sigma_ADDR, REG_IRQ1_STATUS3_IC_1_Sigma_BYTE, R154_IRQ1_STATUS3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ2_STATUS1_IC_1_Sigma_ADDR, REG_IRQ2_STATUS1_IC_1_Sigma_BYTE, R155_IRQ2_STATUS1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ2_STATUS2_IC_1_Sigma_ADDR, REG_IRQ2_STATUS2_IC_1_Sigma_BYTE, R156_IRQ2_STATUS2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_IRQ2_STATUS3_IC_1_Sigma_ADDR, REG_IRQ2_STATUS3_IC_1_Sigma_BYTE, R157_IRQ2_STATUS3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_CTRL1_IC_1_Sigma_ADDR, REG_SPT0_CTRL1_IC_1_Sigma_BYTE, R158_SPT0_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_CTRL2_IC_1_Sigma_ADDR, REG_SPT0_CTRL2_IC_1_Sigma_BYTE, R159_SPT0_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE0_IC_1_Sigma_ADDR, REG_SPT0_ROUTE0_IC_1_Sigma_BYTE, R160_SPT0_ROUTE0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE1_IC_1_Sigma_ADDR, REG_SPT0_ROUTE1_IC_1_Sigma_BYTE, R161_SPT0_ROUTE1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE2_IC_1_Sigma_ADDR, REG_SPT0_ROUTE2_IC_1_Sigma_BYTE, R162_SPT0_ROUTE2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE3_IC_1_Sigma_ADDR, REG_SPT0_ROUTE3_IC_1_Sigma_BYTE, R163_SPT0_ROUTE3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE4_IC_1_Sigma_ADDR, REG_SPT0_ROUTE4_IC_1_Sigma_BYTE, R164_SPT0_ROUTE4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE5_IC_1_Sigma_ADDR, REG_SPT0_ROUTE5_IC_1_Sigma_BYTE, R165_SPT0_ROUTE5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE6_IC_1_Sigma_ADDR, REG_SPT0_ROUTE6_IC_1_Sigma_BYTE, R166_SPT0_ROUTE6_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE7_IC_1_Sigma_ADDR, REG_SPT0_ROUTE7_IC_1_Sigma_BYTE, R167_SPT0_ROUTE7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE8_IC_1_Sigma_ADDR, REG_SPT0_ROUTE8_IC_1_Sigma_BYTE, R168_SPT0_ROUTE8_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE9_IC_1_Sigma_ADDR, REG_SPT0_ROUTE9_IC_1_Sigma_BYTE, R169_SPT0_ROUTE9_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE10_IC_1_Sigma_ADDR, REG_SPT0_ROUTE10_IC_1_Sigma_BYTE, R170_SPT0_ROUTE10_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE11_IC_1_Sigma_ADDR, REG_SPT0_ROUTE11_IC_1_Sigma_BYTE, R171_SPT0_ROUTE11_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE12_IC_1_Sigma_ADDR, REG_SPT0_ROUTE12_IC_1_Sigma_BYTE, R172_SPT0_ROUTE12_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE13_IC_1_Sigma_ADDR, REG_SPT0_ROUTE13_IC_1_Sigma_BYTE, R173_SPT0_ROUTE13_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE14_IC_1_Sigma_ADDR, REG_SPT0_ROUTE14_IC_1_Sigma_BYTE, R174_SPT0_ROUTE14_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT0_ROUTE15_IC_1_Sigma_ADDR, REG_SPT0_ROUTE15_IC_1_Sigma_BYTE, R175_SPT0_ROUTE15_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_CTRL1_IC_1_Sigma_ADDR, REG_SPT1_CTRL1_IC_1_Sigma_BYTE, R176_SPT1_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_CTRL2_IC_1_Sigma_ADDR, REG_SPT1_CTRL2_IC_1_Sigma_BYTE, R177_SPT1_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE0_IC_1_Sigma_ADDR, REG_SPT1_ROUTE0_IC_1_Sigma_BYTE, R178_SPT1_ROUTE0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE1_IC_1_Sigma_ADDR, REG_SPT1_ROUTE1_IC_1_Sigma_BYTE, R179_SPT1_ROUTE1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE2_IC_1_Sigma_ADDR, REG_SPT1_ROUTE2_IC_1_Sigma_BYTE, R180_SPT1_ROUTE2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE3_IC_1_Sigma_ADDR, REG_SPT1_ROUTE3_IC_1_Sigma_BYTE, R181_SPT1_ROUTE3_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE4_IC_1_Sigma_ADDR, REG_SPT1_ROUTE4_IC_1_Sigma_BYTE, R182_SPT1_ROUTE4_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE5_IC_1_Sigma_ADDR, REG_SPT1_ROUTE5_IC_1_Sigma_BYTE, R183_SPT1_ROUTE5_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE6_IC_1_Sigma_ADDR, REG_SPT1_ROUTE6_IC_1_Sigma_BYTE, R184_SPT1_ROUTE6_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE7_IC_1_Sigma_ADDR, REG_SPT1_ROUTE7_IC_1_Sigma_BYTE, R185_SPT1_ROUTE7_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE8_IC_1_Sigma_ADDR, REG_SPT1_ROUTE8_IC_1_Sigma_BYTE, R186_SPT1_ROUTE8_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE9_IC_1_Sigma_ADDR, REG_SPT1_ROUTE9_IC_1_Sigma_BYTE, R187_SPT1_ROUTE9_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE10_IC_1_Sigma_ADDR, REG_SPT1_ROUTE10_IC_1_Sigma_BYTE, R188_SPT1_ROUTE10_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE11_IC_1_Sigma_ADDR, REG_SPT1_ROUTE11_IC_1_Sigma_BYTE, R189_SPT1_ROUTE11_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE12_IC_1_Sigma_ADDR, REG_SPT1_ROUTE12_IC_1_Sigma_BYTE, R190_SPT1_ROUTE12_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE13_IC_1_Sigma_ADDR, REG_SPT1_ROUTE13_IC_1_Sigma_BYTE, R191_SPT1_ROUTE13_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE14_IC_1_Sigma_ADDR, REG_SPT1_ROUTE14_IC_1_Sigma_BYTE, R192_SPT1_ROUTE14_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SPT1_ROUTE15_IC_1_Sigma_ADDR, REG_SPT1_ROUTE15_IC_1_Sigma_BYTE, R193_SPT1_ROUTE15_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_MP_CTRL10_IC_1_Sigma_ADDR, REG_MP_CTRL10_IC_1_Sigma_BYTE, R194_MP_CTRL10_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SELFBOOT_CTRL_IC_1_Sigma_ADDR, REG_SELFBOOT_CTRL_IC_1_Sigma_BYTE, R195_SELFBOOT_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SW_EN_CTRL_IC_1_Sigma_ADDR, REG_SW_EN_CTRL_IC_1_Sigma_BYTE, R196_SW_EN_CTRL_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PDM_CTRL1_IC_1_Sigma_ADDR, REG_PDM_CTRL1_IC_1_Sigma_BYTE, R197_PDM_CTRL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PDM_CTRL2_IC_1_Sigma_ADDR, REG_PDM_CTRL2_IC_1_Sigma_BYTE, R198_PDM_CTRL2_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PDM_VOL0_IC_1_Sigma_ADDR, REG_PDM_VOL0_IC_1_Sigma_BYTE, R199_PDM_VOL0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PDM_VOL1_IC_1_Sigma_ADDR, REG_PDM_VOL1_IC_1_Sigma_BYTE, R200_PDM_VOL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PDM_ROUTE0_IC_1_Sigma_ADDR, REG_PDM_ROUTE0_IC_1_Sigma_BYTE, R201_PDM_ROUTE0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_PDM_ROUTE1_IC_1_Sigma_ADDR, REG_PDM_ROUTE1_IC_1_Sigma_BYTE, R202_PDM_ROUTE1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, PROGRAM_ADDR_IC_1_Sigma, PROGRAM_SIZE_IC_1_Sigma, Program_Data_IC_1_Sigma );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, PARAM_ADDR_IC_1_Sigma, PARAM_SIZE_IC_1_Sigma, Param_Data_IC_1_Sigma );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, NON_MODULO_RAM__ADDR_IC_1_Sigma, NON_MODULO_RAM__SIZE_IC_1_Sigma, NON_MODULO_RAM__Data_IC_1_Sigma );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_ADC_DAC_HP_PWR_IC_1_Sigma_ADDR, REG_ADC_DAC_HP_PWR_IC_1_Sigma_BYTE, R206_ADC_DAC_HP_PWR_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_VOL0_IC_1_Sigma_ADDR, REG_DAC_VOL0_IC_1_Sigma_BYTE, R207_DAC_VOL0_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_DAC_VOL1_IC_1_Sigma_ADDR, REG_DAC_VOL1_IC_1_Sigma_BYTE, R208_DAC_VOL1_IC_1_Sigma_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1_SIGMA, REG_SDSP_CTRL2_IC_1_Sigma_ADDR, REG_SDSP_CTRL2_IC_1_Sigma_BYTE, R209_SDSP_CTRL2_IC_1_Sigma_Default );
}

#endif
