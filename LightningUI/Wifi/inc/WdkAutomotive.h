/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkAutomotive.h#1 $ */
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
/// \brief      Automotive utility functions
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-01-20
//-----------------------------------------------------------------------------
/// \file

#ifndef _QLX_AUTOMOTIVE_UTIL_H_
#define _QLX_AUTOMOTIVE_UTIL_H_
#pragma once

#include <WdkCoreDef.h>
#include <WdkVersion.h>

/// \brief Returns TRUE if the OS is Windows Embedded Automotive
WdkCore_API BOOL IsAutomotiveOs(void);

/// \brief Writes a message to the WEA time log
WdkCore_API BOOL AutoComp_WriteTimeLog( LPCWSTR pszMsg);

#endif // _QLX_AUTOMOTIVE_UTIL_H_
