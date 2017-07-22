/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkCoreApi.h#1 $ */
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
/// \brief      WDK Core API's
/// \remarks
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-10-17
//-----------------------------------------------------------------------------
/// \file 

#ifndef _WDK_COREAPI_H_
#define _WDK_COREAPI_H_
#pragma once

#include <WdkVersion.h>
#include <WdkCoreDef.h>

/// \brief Flags indicating which WDK features are present
typedef enum _WDK_FEATURE_FLAG
{
    WDK_FEATURE_WIFI = 0x0001,  ///< WDK Wifi DK included in the image
    WDK_FEATURE_WEB  = 0x0002,  ///< WDK Web DK included in the image
    WDK_FEATURE_WAN  = 0x0004,  ///< WDK Wan DK included in the image
} WDK_FEATURE_FLAG;
#define WDK_FEATURE_MASK        0x0007
    
/// \brief Returns the feature set.
/// 
/// If *pdwFeatures is non-zero, it is treated as a mask of the desired feature.
/// If the desired features are not available the function returns E_NOT_FOUND.
WdkCore_API HRESULT WDK_Core_GetFeatures( __inout PDWORD pdwFeatures );

/// \brief WDK debug zones
typedef struct _WDK_DEBUG_ZONES
{
    unsigned error:1;       ///< error
    unsigned warning:1;     ///< warning
    unsigned dll:1;         ///< dll 
    unsigned init:1;        ///< initialization
    unsigned io:1;          ///< I/O
    unsigned read:1;        ///< Read
    unsigned write:1;       ///< Write
    unsigned power:1;       ///< Power
    unsigned ioctl:1;       ///< IOCTL
    unsigned func:1;        ///< Functions
    unsigned info:1;        ///< Informational
    unsigned details:1;     ///< Detailed information
    unsigned status:1;      ///< Status
    unsigned trace:1;       ///< Trace
    unsigned thread:1;      ///< Thread
    unsigned cmd:1;         ///< Command
} WDK_DEBUG_ZONES, *PWDK_DEBUG_ZONES;

/// \brief WDK debug info structure
typedef union _WDK_DEBUG_INFO
{
    WDK_DEBUG_ZONES zone;   ///< Debug zones
    DWORD dw;               ///< DWORD equivalent
} WDK_DEBUG_INFO, *PWDK_DEBUG_INFO;

/// \brief Sets the debug zones for the specified feature.
/// 
/// The WDK_FEATURE_WIFI feature flag also enables the Common/Core debug zones.
/// Depending on the module, a reboot may be required for the new values to take effect.
///
/// \param dwFeatures Or-ed set of WDK_FEATURE_FLAG values indicating which feature debug zones to set.
/// \param pDebugZones pointer to a WDK_DEBUG_INFO struct indicating which zones to enable.
WdkCore_API HRESULT WDK_Core_SetDebugZones( DWORD dwFeatures, __in PWDK_DEBUG_INFO pDebugZones);

/// \brief Returns the debug zones for the specified feature.
/// 
/// The WDK_FEATURE_WIFI also enables the Common/Core debug zones.
///
/// \param dwFeature WDK_FEATURE_FLAG value indicating which feature debug zones to retrieve.
/// \param pDebugZones pointer to a WDK_DEBUG_INFO struct to hold the debug zones.
WdkCore_API HRESULT WDK_Core_GetDebugZones( DWORD dwFeature, __out PWDK_DEBUG_INFO pDebugZones);

/// \brief Returns TRUE if test support is enabled for WDK.
WdkCore_API BOOL WDK_Core_IsTestSupportEnabled( void );

#endif // _WDK_COREAPI_H_
