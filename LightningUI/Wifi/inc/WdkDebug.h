/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkDebug.h#1 $ */
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
/// \brief      Debug and logging macros
/// \remarks 
///             - Original Author :  Les Meeks
///             - Original Date   :  2011-02-24
//-----------------------------------------------------------------------------
/// \file

#ifndef __QLX_DEBUG_H_
#define __QLX_DEBUG_H_
#pragma once

#define WDK_DEBUG_MODULE_CORE   "WdkCore"
#define WDK_DEBUG_MODULE_WEBDK  "WdkWanDK"
#define WDK_DEBUG_MODULE_WANDK  "WdkWebDK"

#ifdef UNDER_CE
#include <dbgapi.h>
#else
#ifdef QLX_DEBUG_NO_EXTERN
#undef QLX_DEBUG_NO_EXTERN
#endif
#define QLX_DEBUG_NO_EXTERN 1
#endif

#include <WdkVersion.h>
#include <WdkMacros.h>

#define WDK_LOGMSG(_zone, _fmt)    RETAILMSG(_zone, _fmt)

#ifdef UNDER_CE
#define WDK_LOGERROR(_fail, _fmt)    ((_fail) ? WDK_LOGMSG(QLX_ZONE_ERROR, _fmt) : 0)
#else
#include <wchar.h>
#define WDK_LOGERROR(_fail, _fmt)    ((_fail) ? wprintf(_fmt) : 0)
#endif 

#define WDK_LOGWARN(_fail, _fmt)    ((_fail) ? WDK_LOGMSG(QLX_ZONE_WARN, _fmt) : 0)

#ifndef QLX_DEBUG_MASK
#define QLX_DEBUG_MASK  QLX_ZONE_MASKS_BASIC
#endif 

// zone masks
#define QLX_ZONE_MASK_ERROR     0x0001
#define QLX_ZONE_MASK_WARN      0x0002
#define QLX_ZONE_MASK_DLL       0x0004
#define QLX_ZONE_MASK_INIT      0x0008
#define QLX_ZONE_MASK_IO        0x0010
#define QLX_ZONE_MASK_READ      0x0020
#define QLX_ZONE_MASK_WRITE     0x0040
#define QLX_ZONE_MASK_POWER     0x0080
#define QLX_ZONE_MASK_IOCTL     0x0100
#define QLX_ZONE_MASK_FUNC      0x0200
#define QLX_ZONE_MASK_INFO      0x0400
#define QLX_ZONE_MASK_DETAILS   0x0800
#define QLX_ZONE_MASK_STATUS    0x1000
#define QLX_ZONE_MASK_TRACE     0x2000
#define QLX_ZONE_MASK_THREAD    0x4000
#define QLX_ZONE_MASK_CMD       0x8000

// some useful zone groups
#define QLX_ZONE_MASKS_ALL      0xFFFF
#define QLX_ZONE_MASKS_NONE     0x0000

#define QLX_ZONE_MASKS_BASIC    0x008F  // error, warn, dll, init, power
#define QLX_ZONE_MASKS_IO       0x0070  // io, read, write
#define QLX_ZONE_MASKS_FUNC     0x4300  // ioctl, func, thread
#define QLX_ZONE_MASKS_DETAIL   0x9c00  // info, status, cmd, details

#ifdef QLX_DEBUG_NO_EXTERN

#pragma warning( disable : 6316 ) // const tested expression

#define QLX_ZONE_ERROR          BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_ERROR  )
#define QLX_ZONE_WARN           BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_WARN   )
#define QLX_ZONE_DLL            BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_DLL    )
#define QLX_ZONE_INIT           BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_INIT   )
#define QLX_ZONE_IO             BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_IO     )
#define QLX_ZONE_READ           BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_READ   )
#define QLX_ZONE_WRITE          BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_WRITE  )
#define QLX_ZONE_POWER          BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_POWER )
#define QLX_ZONE_IOCTL          BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_IOCTL  )
#define QLX_ZONE_FUNC           BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_FUNC   )
#define QLX_ZONE_INFO           BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_INFO   )
#define QLX_ZONE_DETAILS        BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_DETAILS )
#define QLX_ZONE_STATUS         BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_STATUS )
#define QLX_ZONE_TRACE          BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_TRACE  )
#define QLX_ZONE_THREAD         BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_THREAD )
#define QLX_ZONE_CMD            BOOLIFY( QLX_DEBUG_MASK | QLX_ZONE_MASK_CMD    )

#else // !QLX_DEBUG_NO_EXTERN

//extern DBGPARAM dpCurSettings;
#define QLX_ZONE_ERROR          DEBUGZONE(0)
#define QLX_ZONE_WARN           DEBUGZONE(1)
#define QLX_ZONE_DLL            DEBUGZONE(2)
#define QLX_ZONE_INIT           DEBUGZONE(3)
#define QLX_ZONE_IO             DEBUGZONE(4)
#define QLX_ZONE_READ           DEBUGZONE(5)
#define QLX_ZONE_WRITE          DEBUGZONE(6)
#define QLX_ZONE_POWER          DEBUGZONE(7)
#define QLX_ZONE_IOCTL          DEBUGZONE(8)
#define QLX_ZONE_FUNC           DEBUGZONE(9)
#define QLX_ZONE_INFO           DEBUGZONE(10)
#define QLX_ZONE_DETAILS        DEBUGZONE(11)
#define QLX_ZONE_STATUS         DEBUGZONE(12)
#define QLX_ZONE_TRACE          DEBUGZONE(13)
#define QLX_ZONE_THREAD         DEBUGZONE(14)
#define QLX_ZONE_CMD            DEBUGZONE(15)

//if you define a module name before you include this file
//it will initialize you modules debug settings.
#ifdef QLX_DEBUG_MODULE_NAME

    extern DBGPARAM dpCurSettings =
    {
        TEXT(QLX_DEBUG_MODULE_NAME),
        {
            TEXT("Error"),  TEXT("Warn"),   TEXT("Dll"),    TEXT("Init"),
            TEXT("I/O"),    TEXT("Read"),   TEXT("Write"),  TEXT("Power"),
            TEXT("Ioctl"),  TEXT("Func"),   TEXT("Info"),   TEXT("Details"),
            TEXT("Status"), TEXT("Trace"),  TEXT("Thread"), TEXT("Cmd")
        },
        QLX_DEBUG_MASK,
    };

#else // !QLX_DEBUG_MODULE_NAME

#ifdef UNDER_CE
    extern DBGPARAM dpCurSettings;
#endif
#endif // QLX_DEBUG_MODULE_NAME

#endif // QLX_DEBUG_NO_EXTERN

#endif
