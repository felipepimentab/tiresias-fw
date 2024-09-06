/*
 * File:            SigmaStudioFW.h
 *
 * Description:     SigmaStudio System Framework macro definitions. These
 *                  macros should be implemented for your system's software.
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
 * Copyright © 2008 Analog Devices, Inc. All rights reserved.
 */

#ifndef __SIGMASTUDIOFW_H__
#define __SIGMASTUDIOFW_H__

#include "adau1701.h"
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

/*
 * TODO: Update for your system's data type
 */
typedef unsigned short ADI_DATA_U16;
typedef unsigned char ADI_REG_TYPE;

extern const struct i2c_dt_spec dev_i2c;

/*
 * Parameter data format
 */
#define SIGMASTUDIOTYPE_FIXPOINT 0
#define SIGMASTUDIOTYPE_INTEGER 1

/*
 * Write to a single Device register
 */
#define SIGMA_WRITE_REGISTER(devAddress, address, dataLength, data)

/*
 * Write to multiple Device registers
 */
#define SIGMA_WRITE_REGISTER_BLOCK(devAddress, address, length, pData)

/*
 * Writes delay (in ms)
 */
#define SIGMA_WRITE_DELAY(devAddress, length, pData)                                                                   \
  {                                                                                                                    \
    k_msleep(*(pData));                                                                                                \
  }

/*
 * Read device registers
 */
#define SIGMA_READ_REGISTER(devAddress, address, length, pData)

/*
 * Set a register field's value
 */
#define SIGMA_SET_REGISTER_FIELD(regVal, fieldVal, fieldMask, fieldShift)                                              \
  {                                                                                                                    \
    (regVal) = (((regVal) & (~(fieldMask))) | (((fieldVal) << (fieldShift)) & (fieldMask)));                           \
  }

/*
 * Get the value of a register field
 */
#define SIGMA_GET_REGISTER_FIELD(regVal, fieldMask, fieldShift) (((regVal) & (fieldMask)) >> (fieldShift))

/*
 * Convert a floating-point value to SigmaDSP (5.23) fixed point format
 *    This optional macro is intended for systems having special implementation
 *    requirements (for example: limited memory size or endianness)
 */
#define SIGMASTUDIOTYPE_FIXPOINT_CONVERT(_value) ((int32_t)((_value) * (1 << 23)))

/*
 * Convert integer data to system compatible format
 *    This optional macro is intended for systems having special implementation
 *    requirements (for example: limited memory size or endianness)
 */
#define SIGMASTUDIOTYPE_INTEGER_CONVERT(_value) ((int32_t)(_value))

#endif /* __SIGMASTUDIOFW_H__ */