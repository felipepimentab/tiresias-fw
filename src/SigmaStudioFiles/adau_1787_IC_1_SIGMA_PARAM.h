/*
 * File:           C:\users\crossover\Desktop\exports\adau_1787_IC_1_SIGMA_PARAM.h
 *
 * Created:        Wednesday, June 10, 2026 3:09:05 PM
 * Description:    adau_1787:IC 1-Sigma parameter RAM definitions.
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
 * Copyright ©2026 Analog Devices, Inc. All rights reserved.
 */
#ifndef __ADAU_1787_IC_1_SIGMA_PARAM_H__
#define __ADAU_1787_IC_1_SIGMA_PARAM_H__


/* Module Non Modulo Location Start - Non Modulo Location Start*/
#define MOD_NONMODULOLOCATIONSTART_COUNT               1
#define MOD_NONMODULOLOCATIONSTART_DEVICE              "IC1_Sigma"
#define MOD_NONMODULOLOCATIONSTART_NON_MODULO_LOCATION_START_ADDR 8192
#define MOD_NONMODULOLOCATIONSTART_NON_MODULO_LOCATION_START_FIXPT 0x00000800
#define MOD_NONMODULOLOCATIONSTART_NON_MODULO_LOCATION_START_VALUE SIGMASTUDIOTYPE_INTEGER_CONVERT(2048)
#define MOD_NONMODULOLOCATIONSTART_NON_MODULO_LOCATION_START_TYPE SIGMASTUDIOTYPE_INTEGER

/* Module SafeLoadModule - SafeLoadModule*/
#define MOD_SAFELOADMODULE_COUNT                       7
#define MOD_SAFELOADMODULE_DEVICE                      "IC1_Sigma"
#define MOD_SAFELOADMODULE_DATALOADSTART_SAFELOAD_ADDR 8196
#define MOD_SAFELOADMODULE_DATALOAD1_SAFELOAD_ADDR     8200
#define MOD_SAFELOADMODULE_DATALOAD2_SAFELOAD_ADDR     8204
#define MOD_SAFELOADMODULE_DATALOAD3_SAFELOAD_ADDR     8208
#define MOD_SAFELOADMODULE_DATALOADEND_SAFELOAD_ADDR   8212
#define MOD_SAFELOADMODULE_ADDRESSLOAD_SAFELOAD_ADDR   8216
#define MOD_SAFELOADMODULE_NUMLOAD_SAFELOAD_ADDR       8220

/* Module Coefficient Offset Location Start - Coefficient Offset Location Start*/
#define MOD_COEFFICIENTOFFSETLOCATIONSTART_COUNT       1
#define MOD_COEFFICIENTOFFSETLOCATIONSTART_DEVICE      "IC1_Sigma"
#define MOD_COEFFICIENTOFFSETLOCATIONSTART_COEFFICIENT_OFFSET_LOCATION_START_ADDR 8224
#define MOD_COEFFICIENTOFFSETLOCATIONSTART_COEFFICIENT_OFFSET_LOCATION_START_FIXPT 0x00002004
#define MOD_COEFFICIENTOFFSETLOCATIONSTART_COEFFICIENT_OFFSET_LOCATION_START_VALUE SIGMASTUDIOTYPE_INTEGER_CONVERT(8196)
#define MOD_COEFFICIENTOFFSETLOCATIONSTART_COEFFICIENT_OFFSET_LOCATION_START_TYPE SIGMASTUDIOTYPE_INTEGER

/* Module DC1 - DC Input Entry*/
#define MOD_DC1_COUNT                                  1
#define MOD_DC1_DEVICE                                 "IC1_Sigma"
#define MOD_DC1_DCINPALG1_ADDR                         8228
#define MOD_DC1_DCINPALG1_FIXPT                        0x00800000
#define MOD_DC1_DCINPALG1_VALUE                        SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_DC1_DCINPALG1_TYPE                         SIGMASTUDIOTYPE_FIXPOINT

/* Module Mute1 - Mute*/
#define MOD_MUTE1_COUNT                                1
#define MOD_MUTE1_DEVICE                               "IC1_Sigma"
#define MOD_MUTE1_MUTENOSLEWALG1MUTE_ADDR              8232
#define MOD_MUTE1_MUTENOSLEWALG1MUTE_FIXPT             0x00800000
#define MOD_MUTE1_MUTENOSLEWALG1MUTE_VALUE             SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_MUTE1_MUTENOSLEWALG1MUTE_TYPE              SIGMASTUDIOTYPE_FIXPOINT

/* Module ReadBack1 - DSP Readback*/
#define MOD_READBACK1_COUNT                            1
#define MOD_READBACK1_DEVICE                           "IC1_Sigma"
#define MOD_READBACK1_READBACKALGSIGMA2001_ADDR        8236
#define MOD_READBACK1_READBACKALGSIGMA2001_FIXPT       0x00000000
#define MOD_READBACK1_READBACKALGSIGMA2001_VALUE       SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0)
#define MOD_READBACK1_READBACKALGSIGMA2001_TYPE        SIGMASTUDIOTYPE_FIXPOINT

/* Module Nx2-1 - Stereo Switch Nx2*/
#define MOD_NX2_1_COUNT                                1
#define MOD_NX2_1_DEVICE                               "IC1_Sigma"
#define MOD_NX2_1_STEREOSWSLEW_ADDR                    8240
#define MOD_NX2_1_STEREOSWSLEW_FIXPT                   0x00000000
#define MOD_NX2_1_STEREOSWSLEW_VALUE                   SIGMASTUDIOTYPE_INTEGER_CONVERT(0)
#define MOD_NX2_1_STEREOSWSLEW_TYPE                    SIGMASTUDIOTYPE_INTEGER

#endif
