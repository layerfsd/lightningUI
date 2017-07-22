/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkNotificationApi.h#1 $ */
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
/// \brief      Notification API 
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-04-19
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_NOTIFICATION_API_H_
#define _WDK_NOTIFICATION_API_H_

#include <WdkVersion.h>
#include <WdkCoreDef.h>
#include <WdkMacros.h>

/// \brief Named event indicating the notification API is ready.
#define WDK_NOTIFIY_READY_EVENT         L"WdkNotify.Ready"

/// \brief Notification data types
typedef enum 
{
    NOTIFY_DATATYPE_UNKNOWN = 0,    ///< Unknown data type
    NOTIFY_DWORD,                   ///< DWORD
    NOTIFY_SZ,                      ///< Multibyte zero terminated string
    NOTIFY_WZ,                      ///< Unicode zero terminated string
    NOTIFY_BINARY,                  ///< Binary data
} WDK_NOTIFY_DATATYPE;

/// \brief Notification categories
typedef enum _WDK_NOTIFYCAT
{
    WDK_NOTIFYCAT_UNKNOWN               = 0x0000,   ///< Unknown/No category
    WDK_NOTIFYCAT_WIFI_AP               = 0x0001,   ///< WifiDK: WiFi AP notifications
    WDK_NOTIFYCAT_WIFI_CLIENT           = 0x0002,   ///< WifiDK: WiFi STA notifications
    WDK_NOTIFYCAT_MODEM                 = 0x0004,   ///< WanDK: Modem notifications
    WDK_NOTIFYCAT_WAN                   = 0x0008,   ///< WanDK: WAN notifications
    WDK_NOTIFYCAT_BT                    = 0x0010,   ///< WanDK: Bluetooth notification
    WDK_NOTIFYCAT_CONNECTIONS           = 0x0020,   ///< WanDK: Connection type notification
    WDK_NOTIFYCAT_SETTINGS_AUTODIAL     = 0x0040,   ///< WanDK: Autodial settings notification (CE6 Only)
    WDK_NOTIFYCAT_CORE                  = 0x0080,   ///< Common: Core notifications
    WDK_NOTIFYCAT_SERVICES              = 0x0100,   ///< Common: Service state notification
    WDK_NOTIFYCAT_PARENTAL_CONTROLS     = 0x0200,   ///< WanDK: Parental controls notification
    WDK_NOTIFYCAT_USB                   = 0x0400,   ///< WanDK: USB modem notification
    WDK_NOTIFYCAT_SMS                   = 0x1000,   ///< WanDK: SMS notification
    WDK_NOTIFYCAT_WIFI_P2P              = 0x2000,   ///< WifiDK: Wi-Fi Direct
    WDK_NOTIFYCAT_WIFI_WPS              = 0x4000,   ///< WifiDK: Wi-Fi Protected Setup
} WDK_NOTIFYCAT;

/// \brief Enable all notification categories
#define WDK_NOTIFYCAT_ALL               0xffff
/// \brief Enable all WIFI only notification categories
#define WDK_NOTIFYCAT_ALL_WIFI          ( WDK_NOTIFYCAT_WIFI_AP \
                                        | WDK_NOTIFYCAT_WIFI_CLIENT \
                                        | WDK_NOTIFYCAT_WIFI_P2P \
                                        | WDK_NOTIFYCAT_WIFI_WPS)

/// \brief Enable all WAN only notification categories
#define WDK_NOTIFYCAT_ALL_WAN           ( WDK_NOTIFYCAT_MODEM \
                                        | WDK_NOTIFYCAT_WAN \
                                        | WDK_NOTIFYCAT_BT \
                                        | WDK_NOTIFYCAT_CONNECTIONS \
                                        | WDK_NOTIFYCAT_SETTINGS_AUTODIAL \
                                        | WDK_NOTIFYCAT_PARENTAL_CONTROLS \
                                        | WDK_NOTIFYCAT_USB \
                                        | WDK_NOTIFYCAT_SMS )
/// \brief Enable all WEB only notification categories
#define WDK_NOTIFYCAT_ALL_WEB           0

#define WDK_NOTIFY_ITEM_FIRST                   1

/// \brief Notification items
typedef enum _WDK_NOTIFY_ITEM
{
    WDK_NOTIFY_NONE                             = 0,    ///< Not assigned to any notification

    WDK_NOTIFY_HEALTHSVC                        = WDK_NOTIFY_ITEM_FIRST,    ///< DWORD (SERVICE_STATE). Health service state change.
    WDK_NOTIFY_CONNDIRSVC                       = 2,    ///< DWORD (SERVICE_STATE). Connection Director service state change. WanDK Only
    WDK_NOTIFY_AUTODIALSVC                      = 3,    ///< DWORD (SERVICE_STATE). Autodial service state change. WanDK only.
    WDK_NOTIFY_NOTIFYSVC                        = 5,    ///< DWORD (SERVICE_STATE). Notification service state change.
    WDK_NOTIFY_TESTSVC                          = 6,    ///< DWORD (SERVICE_STATE). Internal use only.

    WDK_NOTIFY_CONNTYPES                        = 10,   ///< DWORD (CONNTYPE). WanDK only.

    WDK_NOTIFY_WIFI_AP_RADIO_ENABLED            = 100,  ///< DWORD (BOOL), WifiDK only. Wifi STA Radio enabled state. 
    WDK_NOTIFY_WIFI_AP_NETWORKADDRESS           = 101,  ///< WZ, WifiDK only. Wifi AP MAC address.
    WDK_NOTIFY_WIFI_AP_SSID                     = 102,  ///< SZ, WifiDK only. Wifi AP SSID.
    WDK_NOTIFY_WIFI_AP_CHANNEL                  = 103,  ///< DWORD, WifiDK only. Wifi AP active channel.
    WDK_NOTIFY_WIFI_AP_ENCRYPTIONCIPHER         = 104,  ///< DWORD, WifiDK only. Wifi AP encription cipher
    WDK_NOTIFY_WIFI_AP_WPAMODE                  = 105,  ///< DWORD, WifiDK only. Wifi AP authentication mode.
    WDK_NOTIFY_WIFI_AP_BEACONPERIOD             = 106,  ///< DWORD, WifiDK only. Wifi AP beacon period.
    WDK_NOTIFY_WIFI_AP_HIDESSID                 = 107,  ///< DWORD (BOOL), WifiDK only. Wifi AP hide SSID setting.
    WDK_NOTIFY_WIFI_AP_COUNTRYCODE              = 108,  ///< WZ, WifiDK only. Wifi AP country code.
    WDK_NOTIFY_WIFI_AP_ACS                      = 109,  ///< DWORD (HRESULT), WifiDK only. Wifi AP automatic channel scan result.

    WDK_NOTIFY_WIFI_AP_WPAKEY                   = 110,  ///< WZ, WifiDK only. WPA Passphrase. ( test builds only ).
    WDK_NOTIFY_WIFI_AP_DEFAULTKEYID             = 111,  ///< DWORD, WifiDK only. WEP key index. ( test builds only ).
    WDK_NOTIFY_WIFI_AP_DEFAULTKEYS              = 112,  ///< WZ, WifiDK only. WEP keys. ( test builds only ).

    WDK_NOTIFY_WIFI_CLIENT_RADIO_ENABLED        = 200,  ///< DWORD (BOOL), WifiDK only. Wifi STA radio enabled state.
    WDK_NOTIFY_WIFI_CLIENT_NETWORKADDRESS       = 201,  ///< WZ, WifiDK only. Wifi STA MAC address.
    WDK_NOTIFY_WIFI_CLIENT_COUNTRYCODE          = 208,  ///< WZ, WifiDK only. Wifi STA country code.

    WDK_NOTIFY_WIFI_MONITOR_STATUS              = 250,  ///< DWORD (WDK_WIFISTATUS), WifiDK only. Wifi driver monitor status.
    WDK_NOTIFY_WIFI_MONITOR_ACTION              = 251,  ///< DWORD (WDK_WIFIMON_ACTION), WifiDK only. wifi driver automatic recovery state.
    
    WDK_NOTIFY_MODEM_DIALRULES_ENABLED          = 300,  ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_MODEM_DIALRULES_ALLOW_ROAMING    = 301,  ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_MODEM_DIALRULES_ALWAYS_CONNECT   = 302,  ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_MODEM_DIALRULES_IDLE_TIMEOUT     = 303,  ///< DWORD WanDK only.
    WDK_NOTIFY_MODEM_DIALRULES_IDLE_TIMEOUT_RX  = 304,  ///< DWORD WanDK only.
    
    WDK_NOTIFY_MODEM_STATUS                     = 310,   ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_MODEM_PROVISIONED                = 311,   ///< BINARY (NOTIFY_PROVISION_INFO), WanDK only.
    
    WDK_NOTIFY_MODEM_ROAMING                    = 320,   ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_MODEM_TYPE                       = 321,   ///< DWORD (INT), WanDK only. MODEM_TYPE, 0 = removed
    WDK_NOTIFY_MODEM_NETWORK                    = 322,   ///< DWORD (INT), WanDK only. MODEM_NETWORK
    WDK_NOTIFY_MODEM_POWER                      = 323,   ///< DWORD (INT), WanDK only. MODEM_POWER
    WDK_NOTIFY_MODEM_RSSI                       = 324,   ///< DWORD (INT), WanDK only. Modem RSSI dBM
    WDK_NOTIFY_MODEM_SIGNAL_STRENGTH            = 325,   ///< DWORD (INT), WanDK only. 0-5 bars
    WDK_NOTIFY_MODEM_SIGNAL_QUALITY             = 326,   ///< DWORD (INT), WanDK only. 0-99 BER
    WDK_NOTIFY_MODEM_MODE                       = 327,   ///< DWORD (INT), WanDK only. MODEM_MODE
    WDK_NOTIFY_MODEM_CARRIER                    = 328,   ///< SZ, WanDK only. Modem carrier/operator.
    WDK_NOTIFY_MODEM_ISPHONECALL                = 329,   ///< DWORD (BOOL), WanDK only. Indicates connection is phone call.
    
    WDK_NOTIFY_MODEM_CDMA_SIMSTATE              = 330,   ///< DWORD (CDMA_SIM_STATE), WanDK only.
    WDK_NOTIFY_MODEM_CDMA_SIMTYPE               = 331,   ///< DWORD (CDMA_SIM_TYPE), WanDK only.
    WDK_NOTIFY_MODEM_CDMA_DOMAIN                = 332,   ///< DWORD (CDMA_DOMAIN_SERVICE_TYPE), WanDK only.
    WDK_NOTIFY_MODEM_CDMA_SERVICE               = 333,   ///< DWORD (CDMA_CARRIER_SERVICE_STATE), WanDK only.
    
    WDK_NOTIFY_MODEM_GSM_REGISTRATION           = 340,   ///< DWORD (GSM_REGISTRATION_STATUS), WanDK only.
    WDK_NOTIFY_MODEM_GSM_LOCATIONCODE           = 341,   ///< DWORD, WanDK only.
    WDK_NOTIFY_MODEM_GSM_CELLID                 = 342,   ///< DWORD, WanDK only.
    WDK_NOTIFY_MODEM_GSM_OPERATOR_MODE          = 343,   ///< DWORD (GSM_OPERATOR_MODE), WanDK only.
    WDK_NOTIFY_MODEM_GSM_OPERATOR_FORMAT        = 344,   ///< DWORD (GSM_OPERATOR_FORMAT), WanDK only.
    WDK_NOTIFY_MODEM_GSM_APN                    = 345,   ///< SZ, WanDK only.
    WDK_NOTIFY_MODEM_GSM_CME_ERROR              = 346,   ///< DWORD (GSM_CMS_ERROR), WanDK only.
    WDK_NOTIFY_MODEM_GSM_CMS_ERROR              = 347,   ///< DWORD (GSM_CME_ERROR), WanDK only.
    WDK_NOTIFY_MODEM_MILLIVOLTS                 = 348,  ///< DWORD, WanDK only. Modem supply voltage.
    
    WDK_NOTIFY_MODEM_ID_ESN                     = 360,  ///< SZ, WanDK only. Modem identification string.
    WDK_NOTIFY_MODEM_ID_MEID                    = 361,  ///< SZ, WanDK only. Modem identification string.
    
    WDK_NOTIFY_WAN_CONNSTATUS                   = 400,  ///< DWORD (CONNMGR_STATUS_*), WanDK only.
    WDK_NOTIFY_WAN_BILLBYTIME                   = 401,  ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_WAN_ADAPTERNAME                  = 402,  ///< WZ, WanDK only.
    WDK_NOTIFY_WAN_DESCRIPTION                  = 403,  ///< WZ, WanDK only.
    WDK_NOTIFY_WAN_CMTYPE                       = 404,  ///< DWORD (loword=CM_CONNTYPE, hiword=CM_CONNSUBTYPE), WanDK only.
    WDK_NOTIFY_WAN_CMSECURE                     = 405,  ///< DWORD, WanDK only. higher is more secure.
    WDK_NOTIFY_WAN_BYTES_RCVED                  = 406,  ///< DWORD, WanDK only.
    WDK_NOTIFY_WAN_BYTES_XMTED                  = 407,  ///< DWORD, WanDK only.
    WDK_NOTIFY_WAN_TIMEOUT                      = 408,  ///< DWORD (AUTODIAL_TIMEOUT), WanDK only.
    
    WDK_NOTIFY_BLUETOOTH_STATUS                 = 500,  ///< DWORD (BT_EVENT), WanDK only.
    WDK_NOTIFY_BLUETOOTH_DUN_PROVISIONED        = 501,  ///< BINARY (NOTIFY_PROVISION_INFO), WanDK only.
    WDK_NOTIFY_BLUETOOTH_PAN_PROVISIONED        = 502,  ///< BINARY (NOTIFY_PROVISION_INFO), WanDK only.

    WDK_NOTIFY_PC_RESP                          = 700,  ///< DWORD (DYNDNS_RESPONSE_CODE), WanDK only.

    WDK_NOTIFY_USB_MODESWITCH_VENDOR            = 800,  ///< DWORD, WanDK only.
    WDK_NOTIFY_USB_MODESWITCH_PRODUCT           = 801,  ///< DWORD, WanDK only.
    WDK_NOTIFY_USB_MFD_VENDOR                   = 802,  ///< DWORD, WanDK only.
    WDK_NOTIFY_USB_MFD_PRODUCT                  = 803,  ///< DWORD, WanDK only.

    WDK_NOTIFY_WZC_STATUS                       = 900,  ///< DWORD, WifiDK only. WZC status messages are forwarded only when the WZC Refresh service is enabled.

    WDK_NOTIFY_SMS_INITED                       = 1000, ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_SMS_UNREAD                       = 1001, ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_SMS_FULL                         = 1002, ///< DWORD (BOOL), WanDK only.

    WDK_NOTIFY_SIM_LOCKED                       = 1100, ///< DWORD (BOOL), WanDK only.
    WDK_NOTIFY_SIM_INSERTED                     = 1101, ///< DWORD (BOOL), WanDK only.
    
    WDK_NOTIFY_NDIS_BIND                        = 2000, ///< WZ, NDIS adapter name.
    WDK_NOTIFY_NDIS_UNBIND                      = 2001, ///< WZ, NDIS adapter name.
    WDK_NOTIFY_NDIS_RESET_START                 = 2002, ///< WZ, NDIS adapter name.
    WDK_NOTIFY_NDIS_RESET_END                   = 2003, ///< WZ, NDIS adapter name.
    WDK_NOTIFY_NDIS_MEDIA_CONNECT               = 2004, ///< WZ, NDIS adapter name.
    WDK_NOTIFY_NDIS_MEDIA_DISCONNECT            = 2005, ///< WZ, NDIS adapter name.
    WDK_NOTIFY_NDIS_DEVICE_POWER_UP             = 2006, ///< WZ, NDIS adapter name.
    WDK_NOTIFY_NDIS_DEVICE_POWER_DOWN           = 2007, ///< WZ, NDIS adapter name.

    WDK_NOTIFY_DEBUG_ZONE_WIFI                  = 10000,    ///< DWORD, Wifi/Core debug zones changed
    WDK_NOTIFY_DEBUG_ZONE_WEB                   = 10001,    ///< DWORD, WebDK debug zones changed
    WDK_NOTIFY_DEBUG_ZONE_WAN                   = 10002,    ///< DWORD, WanDK debug zones changed

    WDK_NOTIFY_P2P_GROUPCHANNEL                         = 11010,    ///< Binary, Wi-Fi Direct
    WDK_NOTIFY_P2P_DISCOVERY_COMPLETE                   = 11011,    ///< DWORD, Wi-Fi Direct, number of discovered devices
    WDK_NOTIFY_P2P_STATE                                = 11012,    ///< BINARY, Wi-Fi Direct

    WDK_NOTIFY_P2P_RECV_GO_NEGOTIATION_REQUEST          = 11017,    ///< BINARY, Wi-Fi Direct

    WDK_NOTIFY_WPS_STATUS                       = 11100,    ///< DWORD, Wi-Fi Protected Setup
    WDK_NOTIFY_WPS_KEYINFO                      = 11101,    ///< Binary, Wi-Fi Protected Setup
} WDK_NOTIFY_ITEM;

/// \brief Received notification structure
typedef struct _WDK_NOTIFY_DATA
{
    WDK_NOTIFYCAT           category;   ///< Notification category
    WDK_NOTIFY_ITEM         item;       ///< Notification item
    LPCSTR                  pszItem;    ///< Stringified notification item
    WDK_NOTIFY_DATATYPE     type;       ///< Data type
    HKEY                    hKeyRoot;   ///< Registry root key if item is registry based, NULL otherwise.
    LPCWSTR                 pwzSubkey;  ///< Registry subkey if item is registry based, NULL otherwise.
    LPCWSTR                 pwzValue;   ///< Registry value name if item is registry based, NULL otherwise.
} WDK_NOTIFY_INFO, *PWDK_NOTIFY_DATA;

/// \brief Notification callback function pointer
///
/// Called by the notification system when a state change occurs.
///
/// \param lpvUser user defined parameter passed to WDK_NOTIFY_Open
/// \param pInfo pointer to a constant WDK_NOTIFY_INFO structure describing the notification
/// \param pData pointer to the notification data
/// \param cbData length of the notification data
///
typedef VOID ( *LPFN_WDKNOTIFY)( LPVOID lpvUser, const WDK_NOTIFY_INFO * pInfo, PVOID pData, DWORD cbData );

/// \brief Opens a new notification queue.
///
/// Creates a notification queue. Once open the returned handle may be passed to the
/// other WDK_NOTIFY_ APIs.
///
/// The notification service must be running for this API to succeed. 
/// See WDK_NOTIFIY_READY_EVENT on how to ensure the service is ready.
///
/// \param pwzName unique name for the notification queue
/// \param pfnNotify pointer to a LPFN_WDKNOTIFY callback function
/// \param lpvUser option user defined data passed to the notification function
///
/// \return WDK_HANDLE
///     - non-NULL: Success
///     - NULL: Failure, Use GetLastError to determine cause.
///
WdkCore_C_API WDK_HANDLE  WDK_NOTIFY_Open( __in LPCWSTR pwzName, __in LPFN_WDKNOTIFY pfnNotify, __in_opt LPVOID lpvUser );

/// \brief Closes a notification queue.
///
/// Once closed the queue may not be re-enabled.
///
/// \param wh handle returned by WDK_NOTIFY_Open
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_C_API HRESULT     WDK_NOTIFY_Close( WDK_HANDLE wh );

/// \brief Enables or disables a notification queue.
///
/// Once enabled, the callback function passed to WDK_NOTIFY_Open will be called when a 
/// state change occurs in one of the selected notification categories.
///
/// \param wh handle returned by WDK_NOTIFY_Open
/// \param dwCategoryMask bit-wise OR-ing of desired WDK_NOTIFYCAT values. If this value is zero (0),
///        notifications are disabled.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_C_API HRESULT     WDK_NOTIFY_Enable( WDK_HANDLE wh, DWORD dwCategoryMask );

/// \brief Stringifies a notification item.
///
/// This helper function is included to enhance debugging and tracing efforts.
///
/// \param wh handle returned by WDK_NOTIFY_Open
/// \param pszBuf buffer to contain the string
/// \param cchBuf character count of the buffer
/// \param item notification item to stringify
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_C_API HRESULT     WDK_NOTIFY_ToString(WDK_HANDLE wh, __in LPSTR pszBuf, DWORD cchBuf, WDK_NOTIFY_ITEM item);

#endif // _WDK_NOTIFICATION_API_H_
