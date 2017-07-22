/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkBldVer.h#1 $ */
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
/// \brief      WDK Build Version Definitions 
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-10-28
//-----------------------------------------------------------------------------
/// \file
#ifndef _WDKBLDVER_H_
#define _WDKBLDVER_H_
#pragma once

#include <bldver.h>
#include <WdkVersion.h>

#if (CE_MAJOR_VER < 7)
#define WZC_SUPPORT 1   ///< Indicates the WZC API set should be used
#define P2P_SUPPORT FALSE
#else
#define WLAN_SUPPORT 1  ///< Indicates the Native Wi-Fi (Wlan) API set should be used
#define P2P_SUPPORT  TRUE
#endif

#endif // _WDKBLDVER_H_

