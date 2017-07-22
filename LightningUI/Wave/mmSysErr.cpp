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
#include <windows.h>

static LPCTSTR mmSysErr[] =
{
    TEXT( "MMSYSERR_NOERROR(0)" ),
    TEXT( "MMSYSERR_ERROR(1)" ),
    TEXT( "MMSYSERR_BADDEVICEID(2)" ),
    TEXT( "MMSYSERR_NOTENABLED(3)" ),
    TEXT( "MMSYSERR_ALLOCATED(4)" ),
    TEXT( "MMSYSERR_INVALHANDLE(5)" ),
    TEXT( "MMSYSERR_NODRIVER(6)" ),
    TEXT( "MMSYSERR_NOMEM(7)" ),
    TEXT( "MMSYSERR_NOTSUPPORTED(8)" ),
    TEXT( "MMSYSERR_BADERRNUM(9)" ),
    TEXT( "MMSYSERR_INVALFLAG(10)" ),
    TEXT( "MMSYSERR_INVALPARAM(11)" ),
    TEXT( "MMSYSERR_HANDLEBUSY(12)" ),
    TEXT( "MMSYSERR_INVALIDALIAS(13)" ),
    TEXT( "MMSYSERR_BADDB(14)" ),
    TEXT( "MMSYSERR_KEYNOTFOUND(15)" ),
    TEXT( "MMSYSERR_READERROR(16)" ),
    TEXT( "MMSYSERR_WRITEERROR(17)" ),
    TEXT( "MMSYSERR_DELETEERROR(18)" ),
    TEXT( "MMSYSERR_VALNOTFOUND(19)" ),
    TEXT( "MMSYSERR_NODRIVERCB(20)" ),
    TEXT( "" ), TEXT( "" ), TEXT( "" ), TEXT( "" ), TEXT( "" ), TEXT( "" ),
    TEXT( "" ), TEXT( "" ), TEXT( "" ), TEXT( "" ), TEXT( "" ),
    TEXT( "WAVERR_BADFORMAT(32)" ),
    TEXT( "WAVERR_STILLPLAYING(33)" ),
    TEXT( "WAVERR_UNPREPARED(34)" ),
    TEXT( "WAVERR_SYNC(35)" ),
};

LPCTSTR mmGetErrMsg( MMRESULT res )
{
    return mmSysErr[ ( DWORD )res ];
}