/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkServicesApi.h#1 $ */
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
/// \brief      API's for retrieving WKD service information.
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKSERVICES_API_H_
#define _WDKSERVICES_API_H_
#pragma once

#include <WdkVersion.h>
#include <WdkCoreDef.h>
#include <WdkFileUtil.h>    // VERSION

#define SVC_NAME_HEALTH     L"WdkHealthSvc"
#define SVC_NAME_AUTODIAL   L"WdkAutoDialSvc"
#define SVC_NAME_CONDIR     L"WdkConnDirSvc"
#define SVC_NAME_NOTIFY     L"WdkNotifySvc"

/// \brief Enumeration of WDK services.
typedef enum _WDK_SVC_TYPE
{
    WDK_SVC_UNKNOWN = -1,
    
    WDK_SVC_HEALTH  = 0,    ///< WDK Health Service
    WDK_SVC_CONNDIR,        ///< WDK Connection Director Service
    WDK_SVC_AUTODIAL,       ///< WDK AutoDial Service
    WDK_SVC_NOTIFY,         ///< WDK Notification Service
    
    NUM_WDK_SVC_TYPES,      ///< Count of service types.
} WDK_SVC_TYPE;

/// \brief Returns the current SERVICE_STATUS for the specified WDK service
///
/// \param type WDK service
/// \param pdwStatus pointer to a DWORD value indicating the requested service state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT WDK_SVC_GetStatus(WDK_SVC_TYPE type, __out DWORD *pdwStatus);

/// \brief Returns the version for the specified WDK service
///
/// \param type WDK service
/// \param pVersion pointer to a VERSION structure indicating the requested service version
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT WDK_SVC_GetVersion(WDK_SVC_TYPE type, __out VERSION *pVersion);

#define WDK_SERVICE_NAME_MAX_LEN            32
#define WDK_SERVICE_HANDLER_NAME_MAX_LEN    32

typedef enum _WDK_SERVICE_HANDLER_ID
{
    WDK_SERVICE_HANDLER_NONE            = 0,    ///< Reserved
    
    WDK_SERVICE_HANDLER_WIFIACS         = 1,    ///< Wifi AP ACS
    WDK_SERVICE_HANDLER_FIREWALL        = 2,    ///< Firewall
    WDK_SERVICE_HANDLER_IPSEC           = 3,    ///< IPSec handle
    WDK_SERVICE_HANDLER_PC              = 4,    ///< Parental Controls
    WDK_SERVICE_HANDLER_HOSTNAME        = 5,    ///< Hostname validator
    WDK_SERVICE_HANDLER_CPU             = 6,    ///< Hostname validator
    WDK_SERVICE_HANDLER_FW_CLIENTLOCAL  = 7,    ///< Allow Local Traffic on the specified client adapter validator
    WDK_SERVICE_HANDLER_WZCREFRESH      = 8,    ///< periodically kick the wzc refresh api, needed if the wzc ctl applet is missing
    WDK_SERVICE_HANDLER_WIFIMONITOR     = 9,    ///< monitor wifi drivers and recover from error states
    WDK_SERVICE_HANDLER_WIFIDIRECT      = 10,   ///< Wi-Fi Direct service
    
    WDK_SERVICE_HANDLER_CONNMGR         = 101,   ///< Connection Manager
    WDK_SERVICE_HANDLER_BLUETOOTH       = 102,   ///< Bluetooth
    WDK_SERVICE_HANDLER_USBMODEM        = 103,   ///< USB Modem
    
    WDK_SERVICE_HANDLER_NOTIFY          = 200,  ///< Notifications 
        
} WDK_SERVICE_HANDLER_ID, *PWDK_SERVICE_HANDLER_ID;

/// \brief Service handler state
typedef enum _SVCHANDLER_STATE
{
    SVCHANDLER_STATE_UNKNOWN,           ///< Unknown state
    SVCHANDLER_STATE_DEINITIALIZED,     ///< Service handler is not initialized
    SVCHANDLER_STATE_STOPPED,           ///< Service handler is stopped
    SVCHANDLER_STATE_STARTED,           ///< Service handler is running
    SVCHANDLER_STATE_INIT_FAILED,       ///< Initializaiton failed
    SVCHANDLER_STATE_LOAD_SETTINGS_FAILED,  ///< Initializaiton failed
    SVCHANDLER_STATE_START_FAILED,          ///< Initializaiton failed
} SVCHANDLER_STATE, *PSVCHANDLER_STATE;

/// \brief Service handler information
typedef struct _WDK_SERVICE_HANDLE_INFO
{
    WDK_SERVICE_HANDLER_ID id;      ///< Handler ID
    WDK_SVC_TYPE type;              ///< Service type
    BOOL fEnabled;                  ///< Handler enabled state
    WCHAR wzName[WDK_SERVICE_HANDLER_NAME_MAX_LEN]; ///< Handler name
    SVCHANDLER_STATE state;         ///< State
} WDK_SERVICE_HANDLE_INFO, *PWDK_SERVICE_HANDLE_INFO;

/// \brief Returns a list of service handlers for each WDK service
///
/// \param type WDK service
/// \param pHndlr pointer to a list of WDK_SERVICE_HANDLE_INFO structures
/// \param pcHndr pointer to a DWORD indicating the number of items in the list
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INSUFFICIENT_BUFFER
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT WDK_SVC_GetHandlerInfoList( WDK_SVC_TYPE type, __out_ecount_opt(*pcHandlers) PWDK_SERVICE_HANDLE_INFO pHndlr, __inout PDWORD pcHndr );

/// \brief Returns information about a specific service handler.
///
/// \param id identifier for the service handler
/// \param pHndlr pointer to a WDK_SERVICE_HANDLE_INFO structure
///
/// \return HRESULT
///     - S_OK: Success
///     - E_SERVICE_NOT_ACTIVE: Service handler is not available
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT WDK_SVC_GetHandlerInfo( WDK_SERVICE_HANDLER_ID id, __out PWDK_SERVICE_HANDLE_INFO pHndlr);

/// \brief Returns a name of each WDK service
///
/// \param type WDK service
/// \param pwzName pointer to WCHAR buffer
/// \param cchName character count of the buffer
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INSUFFICIENT_BUFFER
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT WDK_SVC_GetName(WDK_SVC_TYPE type, __out LPWSTR pwzName, DWORD cchName );

#endif // _WDKSERVICES_API_H_

