/*
***********************Copyright (C) 2013 Shinwa***************************
** File Name   : type_def.h
** Author      : lzihiping
** Date        : 2013/06/19
** Description : Common  include header files, typedef and define
***************************************************************************
*/
#ifndef __TYPE_DEF_H
#define __TYPE_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
//#include <string.h>

//typedef signed char        s8;
typedef __int8				int8_t;

//typedef signed short       s16;
typedef __int16				int16_t;

//typedef signed int         s32;
typedef __int32				int32_t;

//typedef unsigned char      u8;
typedef unsigned __int8     uint8_t;

//typedef unsigned short     u16;
typedef unsigned __int16    uint16_t;

//typedef unsigned int      u32;
typedef unsigned __int32    uint32_t;

//typedef unsigned int      s64;
typedef __int64				int64_t;

//typedef unsigned int      u64;
typedef unsigned __int64	uint64_t;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//#define MAX_MESSAGE_LENGTH   (512)//64
//
//#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#ifdef __cplusplus
}
#endif

#endif