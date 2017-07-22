/***************************************************************************
 *                                                                         *
 *       Shanghai Zongmu Technology Co. Ltd.                               *
 *                                                                         *
 *    Copyright (c) 2013 by Shanghai Zongmu Technology Co. Ltd.            *
 *    All rights reserved.                                                 *
 *                                                                         *
 *    This Software is protected by People's Republic of China copyright   *
 *    laws and international treaties.  You may not reverse engineer,      *
 *    decompile or disassemble this Software.                              *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains Shanghai Zongmu Technology Co. Ltd.'s         *
 *    confidential and proprietary information. UNAUTHORIZED COPYING,      *
 *    USE, DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE *
 *    IS PROHIBITED AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not  *
 *    copy this Software without Shanghai Zongmu Technology Co. Ltd.'s     *
 *    express written permission.   Use of any portion of the contents of  *
 *    this Software is subject to and restricted by your written agreement *
 *    with Shanghai Zongmu Technology Co. Ltd.                             *
 *                                                                         *
 ***************************************************************************/
#ifndef _ZONGMU_DATA_TYPE_H
#define _ZONGMU_DATA_TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined ZADAS || defined WIN32 || defined ZMOS || defined UNDER_CE

typedef signed char zm_s8;
typedef unsigned char zm_u8;
typedef signed short zm_s16;
typedef unsigned short zm_u16;
typedef signed int zm_s32;
typedef unsigned int zm_u32;
typedef signed long long zm_s64;
typedef unsigned long long zm_u64;

#elif defined CSR101X
#include <types.h>
typedef int8 zm_s8;
typedef uint8 zm_u8;
typedef int16 zm_s16;
typedef uint16 zm_u16;
typedef int32 zm_s32;
typedef uint32 zm_u32;
typedef signed long long zm_s64;
typedef unsigned long long zm_u64;

#endif

#ifdef __cplusplus
}
#endif

#endif
