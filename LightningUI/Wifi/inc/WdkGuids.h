/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkGuids.h#1 $ */
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
/// \brief      WDK specific guid strings
/// \remarks 
///             - Original Author :  Lester Meeks
///             - Original Date   :  2011-03-31
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_GUIDS_H_
#define _WDK_GUIDS_H_

#pragma once

#include <windef.h>

#include <WdkVersion.h>

/// \brief constant wstring guid for the health service watch dog name
static LPCWSTR WZGUID_QLX_WATCHDOG_HEALTHSVC = L"{C814E435-4A2E-403f-ABC2-906BAA123E71}";

/// \brief constant wstring guid to identify the inbound local ip spoof blocking rule
static LPCWSTR WZGUID_QLX_BLOCK_INBOUND_RULE = L"{CEEBCD94-C3B0-4686-AB9A-51E48182D664}";
/// \brief constant wstring guid to identify the allow local subnet traffic rule
static LPCWSTR WZGUID_QLX_ALLOW_CLIENT_LOCAL_RULE = L"{2941A1F9-59D4-4a74-BB56-25683610A41A}";
/// \brief constant wstring guid to identify firewall rules that are specifically created for ipsec control
static LPCWSTR WZGUID_QLX_IPSEC_RULE         = L"{83C4A869-8856-4d54-8E0B-07CAB43934A9}";


#endif // _WDK_GUIDS_H_
