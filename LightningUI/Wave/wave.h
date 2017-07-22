/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/

#if !defined( _WAVE_H )
#define _WAVE_H

#include <windows.h>
#include "queue.hpp"

//****************************************************************
// Constants
//****************************************************************

#define AUDIO_FRAME_BUFF_SIZE_MAX		(2048)
#define AUDIO_FRAME_BUFF_SIZE_MIN		(256)

// typedef BYTE    WAVE_FRAME[ AUDIO_FRAME_BUFF_SIZE ]; // 16bit, 8KHz,

typedef struct _WAVEFRAME
{
	BYTE				*pData;
	DWORD       cbData;
}WAVEFRAME;

// extern  LPCTSTR mmGetErrMsg( MMRESULT mr );

__inline void
INIT_WAVEHDR(
    LPWAVEHDR   pwh,
    LPBYTE      pData,
    DWORD       cbData )
{
    pwh->dwLoops = 0;
    pwh->dwFlags = 0;
    pwh->lpData = ( LPSTR )pData;
    pwh->dwBufferLength = cbData;
}

__inline VOID ReportError( LPCTSTR szFuncName, MMRESULT mr )
{
    RETAILMSG( 1, ( TEXT( "%s() FAILED, %x\r\n" ), szFuncName, /*mmGetErrMsg*/( mr ) ) );
}

#endif  /* !defined( _WAVE_H ) */
