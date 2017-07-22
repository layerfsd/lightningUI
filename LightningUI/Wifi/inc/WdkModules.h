/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkModules.h#1 $ */
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
/// \brief      Module Names.  
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKMODULES_H_
#define _WDKMODULES_H_
#pragma once

#include <WdkVersion.h>

#define WDK_MODULE_COMMON_CORE                      "WdkCore.dll"
#define WDK_MODULE_COMMON_SERVICE_HEALTH            "WdkHealthSvc.dll"

#define WDK_MODULE_COMMON_SERVICE_HEALTH_CPU        "WdkHealthSvc_CPU.dll"
#define WDK_MODULE_COMMON_SERVICE_HEALTH_HOSTNAME   "WdkHealthSvc_Hostname.dll"
#define WDK_MODULE_COMMON_SERVICE_NOTIFY            "WdkNotifySvc.dll"
#define WDK_MODULE_COMMON_SERVICES                  "WdkServices.dll"

#define WDK_MODULE_WIFI_CORE                        "WifiDKCore.dll"
#define WDK_MODULE_WIFI_SERVICE_HEALTH_WIFIDIRECT   "WdkHealthSvc_WifiDirect.dll"
#define WDK_MODULE_WIFI_SERVICE_HEALTH_WIFIMONITOR  "WdkHealthSvc_WifiMonitor.dll"
#define WDK_MODULE_WIFI_SERVICE_HEALTH_WZCREFRESH   "WdkHealthSvc_WzcRefresh.dll"
#define WDK_MODULE_WIFI_NDIS_FILTER                 "WdkNdisFilter.dll"

#define WDK_MODULE_WEB_CORE                         "WebDKCore.dll"
#define WDK_MODULE_WEB_ISAPI                        "WDKWebExt.dll"

#define WDK_MODULE_WAN_CONNMGR                      "WdkConnMgr.dll"
#define WDK_MODULE_WAN_SERVICE_CONNDIR              "WdkConnDirSvc.dll"
#define WDK_MODULE_WAN_SERVICE_AUTODIAL             "WdkAutodialSvc.dll"


#endif // _WDKMODULES_H_

