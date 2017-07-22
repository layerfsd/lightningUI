/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WifiDKDef.h#1 $ */
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
/// \brief      WifiDK Core Definitions 
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-04-19
//-----------------------------------------------------------------------------
/// \file
#ifndef _WIFIDKCOREDEF_H_
#define _WIFIDKCOREDEF_H_
#pragma once

#include <WdkVersion.h>

#ifdef __cplusplus
#define WifiDKCore_API extern "C"
#else
#define WifiDKCore_API
#endif

#ifdef WifiDKCore_EXPORTS
#define WifiDKCore_CPP_API __declspec(dllexport)
#else
#define WifiDKCore_CPP_API __declspec(dllimport)
#endif

#endif // _WIFIDKCOREDEF_H_
