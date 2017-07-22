/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkEhm.h#1 $ */
//-----------------------------------------------------------------------------
//
//  Copyright (c) Qualnetics Corporation. All rights reserved.
//  Contact Qualnetics Corporation for a copy of the license agreement.
//
//-----------------------------------------------------------------------------
//
/// $Author: xg01 $  
/// $Revision: #1 $  
/// $Date: 2013/07/11 $  
//
//-----------------------------------------------------------------------------
/// \brief      Enhanced error handling macros
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2010-10-29
//-----------------------------------------------------------------------------
/// \file
#ifndef _WDK_EHM_H_
#define _WDK_EHM_H_
#pragma once

#include <WdkVersion.h>
#include <WdkHresult.h>

#ifndef ZONE_EHM_PRINT_ON_FAIL
#define ZONE_EHM_PRINT_ON_FAIL   1
#endif

#ifdef VERBOSE_EHM
#define QLX_EHM_MSG DEBUGMSG
#else
#define QLX_EHM_MSG(a,b) __noop
#endif

#ifndef _EHM_H_
#define _ehmOnFail(eType, dwCode, pszFile, ulLine, pszMessage) QLX_EHM_MSG(ZONE_EHM_PRINT_ON_FAIL, \
            (TEXT("Error in File: eType=%d, err=0x%08x (%d),  %s(%d)\r\n"), \
                    eType, dwCode, dwCode, pszFile, ulLine));
#include <ehm.h>
#endif

#pragma warning (push)
#pragma warning(disable : 4127) // allow constant conditionals

#define CHK_PTR_PARAM(_p) if(!(_p)) { SetLastError(ERROR_INVALID_PARAMETER); goto Error; }
#define CHK_PTR_RESULT(_p) if(!(_p)) { SetLastError(ERROR_OUTOFMEMORY); goto Error; }

#define CHK_BOOL_EX(_f, _e) do {                    \
        BOOL _fFail = !(_f);                        \
        if(_fFail) {                                \
            SetLastError(_e);                       \
            goto Error;                             \
        } }  while (0,0)
    
#define _CHK_BOOL(_f, _assert) do {                 \
        BOOL _fFail = !(_f);                        \
        if(_fFail) {                                \
            VBR(!_assert);                       \
            if(ERROR_SUCCESS == GetLastError())     \
                SetLastError(ERROR_GEN_FAILURE);    \
            goto Error;                             \
        } }  while (0,0)

#define CHK_BOOL(_f)   _CHK_BOOL(_f, FALSE)
#define CHKA_BOOL(_f)  _CHK_BOOL(_f, TRUE) 

#define CHK_BOOL_PARAM(_f) if(!(_f)) { SetLastError(ERROR_INVALID_PARAMETER); goto Error; }

#define CHK_ERR(_e)         do { DWORD __e = (_e);          \
                                if(ERROR_SUCCESS != __e) {  \
                                    SetLastError(__e);      \
                                    goto Error;             \
                            } }  while (0,0)

#define CHK_HANDLE(_h)      do { HANDLE __h = _h; \
                                if(!(__h) || (__h) == INVALID_HANDLE_VALUE) { SetLastError(ERROR_INVALID_HANDLE); goto Error; } \
                                } while(0,0)
#define CHK_WINERR(_e)      do { DWORD __e = _e; \
                                if(ERROR_SUCCESS != (__e)) { SetLastError(__e); goto Error; } \
                            } while(0,0)
#define CHK_HRESULT(_hr)    do { HRESULT __hr = _hr; \
                              if(FAILED(__hr)) { SetLastError(WIN32ERR_FROM_HRESULT(__hr)); goto Error; } \
                            } while(0,0)

#define CHK_STRSAFE(_hr)    do { HRESULT __hr = _hr; \
                                if(FAILED(__hr)) { \
                                    switch(__hr) { \
                                        case STRSAFE_E_INVALID_PARAMETER:   SetLastError(ERROR_INVALID_PARAMETER); break; \
                                        case STRSAFE_E_INSUFFICIENT_BUFFER: SetLastError(ERROR_INSUFFICIENT_BUFFER); break; \
                                        case STRSAFE_E_END_OF_FILE:         SetLastError(ERROR_HANDLE_EOF); break; \
                                        default:                            SetLastError(ERROR_INTERNAL_ERROR); break; \
                                    } \
                                goto Error; } \
                            } while(0,0)

// -------------------------------------------
// TAPI Errors
//
#define CHK_TAPI_BOOL_EX(_f, _e) do {        \
        BOOL _fFail = !(_f);                 \
        if(_fFail) {                         \
            lRet = _e;                       \
            goto Error;                      \
        } }  while (0,0)
        
#define CHK_TAPI_BOOL_PARAM(_p) if(!(_p)) { lRet = LINEERR_INVALPARAM; goto Error; }
#define CHK_TAPI_PTR_PARAM(_p) if(!(_p)) { lRet = LINEERR_INVALPOINTER; goto Error; }
#define CHK_TAPI_PTR_RESULT(_p) if(!(_p)) { lRet = LINEERR_NOMEM; goto Error; }

#pragma warning( pop )

#endif // _WDK_EHM_H_
