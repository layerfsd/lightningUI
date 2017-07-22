/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkCoreDef.h#1 $ */
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
/// \brief      WDK Core Definitions 
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-04-19
//-----------------------------------------------------------------------------
/// \file
#ifndef _WDKCOREDEF_H_
#define _WDKCOREDEF_H_
#pragma once

#include <winnt.h>  // PVOID
#include <WdkVersion.h>

#ifdef WDKCORE_STATIC_LIB
#define WdkCore_API
#else
#ifdef WDKCORE_EXPORTS
#define WdkCore_API __declspec(dllexport)
#else
#define WdkCore_API __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
#define WdkCore_C_API extern "C"
#else
#define WdkCore_C_API
#endif

/// \brief HANDLE returned by WDK APIs
typedef PVOID WDK_HANDLE;

#endif // _WDKCOREDEF_H_
