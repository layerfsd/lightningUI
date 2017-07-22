/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkVersion.h#1 $ */
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
/// \brief      WDK API Version 
//-----------------------------------------------------------------------------
/// \file
#ifndef _WDK_VERSION_H_
#define _WDK_VERSION_H_
#pragma once

#define WDK_API_VERSION     8

#ifdef __cplusplus
extern "C" {
#endif

BOOL WDK_API_VersionCheck(DWORD dwApiVersion);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif // _WDK_VERSION_H_

