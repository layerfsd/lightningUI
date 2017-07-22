/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkServices.h#1 $ */
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
/// \brief      WDK Services Info
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-07-22
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_SERVICES_H_
#define _WDK_SERVICES_H_
//pragma once

#include <WdkVersion.h>

#define WCD_INST        L"WCD1:"    ///< WDK Wan Connection Director service instance
#define WHE_INST        L"WHE1:"    ///< WDK Heath service instance
#define WNO_INST        L"WNO1:"    ///< WDK Notification service instance
#define WAD_INST        L"WAD1:"    ///< WDK Autodial service instance

#define WCD_READY_EVENT L"WCD1_READY"   ///< Connection director ready event

#endif // _WDK_SERVICES_H_

