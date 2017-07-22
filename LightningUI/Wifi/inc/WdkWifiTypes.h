/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkWifiTypes.h#1 $ */
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
/// \brief      Wifi types 
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2012-06-14
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKWIFITYPES_H_
#define _WDKWIFITYPES_H_

#include <WdkVersion.h>
#include <WifiDKDef.h>
#include <WdkMacros.h>

#define WIFI_STA_MAX_USERNAME       256
#define WIFI_STA_MAX_PASSWORD       254
#define WIFI_STA_MAX_DOMAIN         15
#define WIFI_STA_MAX_KEYMATERIAL    256

/// \brief Available Wi-Fi AP network management type
typedef enum _WDK_AVAIL_AP_NETWORK_MGMT
{
    WDK_AVAIL_AP_NETWORK_MGMT_Unknown           = 0,    ///< Unknown Wi-Fi network type
    WDK_AVAIL_AP_NETWORK_MGMT_AdHoc             = 1,    ///< AdHock Wi-Fi network
    WDK_AVAIL_AP_NETWORK_MGMT_Infrastructure    = 2,    ///< Infrastructure Wi-Fi network
} WDK_AVAIL_AP_NETWORK_MGMT;

/// \brief Available Wi-Fi AP encryption type
typedef enum _WDK_AVAIL_AP_ENCRYPTION
{
    WDK_AVAIL_AP_ENCRYPTION_Unknown = -1,       ///< Unknown/Unsupported encryption
    WDK_AVAIL_AP_ENCRYPTION_None    = 0,        ///< No encryption
    WDK_AVAIL_AP_ENCRYPTION_Wep     = 1,        ///< WEP, key length not specified
    WDK_AVAIL_AP_ENCRYPTION_Tkip    = 2,        ///< TKIP
    WDK_AVAIL_AP_ENCRYPTION_Aes     = 3,        ///< AES
    WDK_AVAIL_AP_ENCRYPTION_Wapi    = 4,        ///< WAPI China, reserved for future use
} WDK_AVAIL_AP_ENCRYPTION;

/// \brief Available Wi-Fi AP authentication type
typedef enum _WDK_AVAIL_AP_AUTHENTICATION
{
    WDK_AVAIL_AP_AUTHENTICATION_Unknown     = -1,   ///< Unknown/Unsupported authentication
    WDK_AVAIL_AP_AUTHENTICATION_Open        = 0,    ///< Open - no authentication
    WDK_AVAIL_AP_AUTHENTICATION_Shared      = 1,    ///< Shared - pre-shared key
    WDK_AVAIL_AP_AUTHENTICATION_AutoSwitch  = 2,    ///< Open/Shared
    WDK_AVAIL_AP_AUTHENTICATION_Wpa         = 3,    ///< WPA Enterprise
    WDK_AVAIL_AP_AUTHENTICATION_WpaPsk      = 4,    ///< WPA Pre-shared key, infrastructure networks
    WDK_AVAIL_AP_AUTHENTICATION_WpaAdHoc    = 5,    ///< WPA pre-shared key, ad-hoc networks
    WDK_AVAIL_AP_AUTHENTICATION_Wpa2        = 6,    ///< WPA2 Enterprise
    WDK_AVAIL_AP_AUTHENTICATION_Wpa2Psk     = 7,    ///< WPA2 Pre-shared key, infrastructure networks
    WDK_AVAIL_AP_AUTHENTICATION_Wapi        = 8,    ///< WAPI Enterprise, reserved for future use
    WDK_AVAIL_AP_AUTHENTICATION_WapiPsk     = 9,    ///< WAPI Pre-shared key, reserved for future use
} WDK_AVAIL_AP_AUTHENTICATION;

/// \brief Wi-Fi AP information structure
typedef struct _WDK_STA_AP_INFO
{
    DWORD                           dwSize;         ///< Size of this structure in bytes
    WCHAR                           wzSsid[34];     ///< SSID, 32 characters plus terminator
    WDK_AVAIL_AP_NETWORK_MGMT       Mgmt;           ///< network management
    WDK_AVAIL_AP_ENCRYPTION         Encryption;     ///< Encryption type
    WDK_AVAIL_AP_AUTHENTICATION     Authentication; ///< Authentication type
} WDK_STA_AP_INFO, *PWDK_STA_AP_INFO;

/// \brief Zeros the structure and sets the size
_inline void ResetApInfo( PWDK_STA_AP_INFO pInfo )
{
    pInfo->dwSize = sizeof(WDK_STA_AP_INFO);
    pInfo->wzSsid[0] = 0;
    pInfo->Authentication = WDK_AVAIL_AP_AUTHENTICATION_Unknown;
    pInfo->Encryption = WDK_AVAIL_AP_ENCRYPTION_Unknown;
    pInfo->Mgmt = WDK_AVAIL_AP_NETWORK_MGMT_Unknown;
}

__inline BOOL IsApInfoEqual(__in PWDK_STA_AP_INFO pAp1, PWDK_STA_AP_INFO pAp2)
{
    return pAp1->Mgmt == pAp2->Mgmt
        && pAp1->Encryption== pAp2->Encryption
        && pAp1->Authentication== pAp2->Authentication
        && IS_WSTR_EQUAL_CASESENS(pAp1->wzSsid, pAp2->wzSsid);
}


/// \brief Current Wi-Fi AP information structure 
typedef struct _WDK_CURR_AP_INFO
{
    DWORD                           dwSize;         ///< Size of this structure in bytes
    WDK_STA_AP_INFO                 Info;           ///< AP Info
    BYTE                            bBssid[MAC_LEN];      ///< BSSID
    ULONG                           ulFreq;         ///< frequency (kHz)
} WDK_CURR_AP_INFO, *PWDK_CURR_AP_INFO;

/// \brief Zeros the structure and sets the size
_inline void ResetCurrApInfo( PWDK_CURR_AP_INFO pCurr )
{
    pCurr->dwSize = sizeof(WDK_CURR_AP_INFO);
    ResetApInfo(&pCurr->Info);
    memset(pCurr->bBssid, 0, sizeof(pCurr->bBssid));
}

typedef enum _WDK_FLAGS
{
    WPS_FLAG_ENABLED        = 0x0001, ///< WPS is enabled
    WPS_FLAG_CONFIGURED     = 0x0002, ///< WPS is configured
    WPS_FLAG_SETUP_LOCKED   = 0x0004, ///< AP setup locked (e.g. too many authentication failures)
    WPS_FLAG_REGISTRAR      = 0x0008, ///< Registrar selected
    WPS_FLAG_PUSHBUTTON     = 0x0010, ///< Pushbutton configuration method
    WPS_FLAG_PIN            = 0x0020, ///< PIN configuration method (display, label, or keypad)
} WPS_FLAGS;

/// \brief Available Wi-Fi AP information structure
typedef struct _WDK_AVAIL_AP_INFO
{
    DWORD                           dwSize;         ///< Size of this structure in bytes
    WDK_STA_AP_INFO                 Info;           ///< AP Info
    BYTE                            bBssid[MAC_LEN];      ///< BSSID
    LONG                            lRssi;          ///< signal strength (dBm)
    ULONG                           ulFreq;         ///< frequency (kHz)
    BOOL                            fConnected;     ///< TRUE if connected (not supported on WEC6)
    DWORD                           dwCaps;         ///< AP_BEACON_CAP flags
    DWORD                           dwWps;          ///< Set of WPS_FLAGS flags (Wi-Fi Protected Setup)
} WDK_AVAIL_AP_INFO, *PWDK_AVAIL_AP_INFO;

/// \brief Zeros the structure and sets the size
_inline void ResetAvailApInfo( PWDK_AVAIL_AP_INFO pAvail )
{
    memset(pAvail, 0, sizeof(*pAvail));
    pAvail->dwSize = sizeof(WDK_CURR_AP_INFO);
    ResetApInfo(&pAvail->Info);
}


/// \brief WEP security related parameters 
typedef struct _WIFI_SHAREDKEY_PARMS
{
    WCHAR   wzKeyMaterial[WIFI_STA_MAX_KEYMATERIAL];       ///< WEP key, or WPA/WPA2 Pre-shared key
    DWORD   dwKeyIndex;               ///< WEP key index (WEP only, 0-4)
} WIFI_SHAREDKEY_PARMS, *PWIFI_SHAREDKEY_PARMS;

typedef enum _WIFI_EAP_TYPE
{

    WIFI_EAP_NONE       = 0,        ///< No EAP (WEP, WPAPSK, or WPA2PSK)
    WIFI_EAP_TLS        = 13,       ///< TLS
    WIFI_EAP_PEAP       = 25,       ///< PEAP
    WIFI_EAP_MSCHAPv2   = 26,       ///< MSCHAP v2
} WIFI_EAP_TYPE, *PWIFI_EAP_TYPE;

typedef enum _WIFI_EAP_PEAP_TYPE
{
 
    WIFI_EAP_PEAP_NONE      = 0,    ///< No PEAP
    WIFI_EAP_PEAP_MSCHAPv2  = 1,    ///< PEAP MSCHAPv2
    WIFI_EAP_PEAP_GTC       = 2,    ///< PEAP GTC
} WIFI_EAP_PEAP_TYPE, *PWIFI_EAP_PEAP_TYPE;

typedef struct _WIFI_EAP_PEAP_PARMS
{
    WIFI_EAP_PEAP_TYPE type;                    ///< PEAP type
    DWORD   dwVersion;                          ///< PEAP version
    WCHAR   wzUsername[WIFI_STA_MAX_USERNAME];  ///< Username
    WCHAR   wzPassphrase[WIFI_STA_MAX_PASSWORD];  ///< Password
    WCHAR   wzDomain[WIFI_STA_MAX_DOMAIN];      ///< Domain
    WCHAR   wzRootCA[MAX_PATH];                 ///< File path for the trusted root CA certificate
} WIFI_EAP_PEAP_PARMS, *PWIFI_EAP_PEAP_PARMS;

typedef struct _WIFI_EAP_TLS_PARMS
{
    WCHAR   wzCertificate[MAX_PATH];            ///< File path for the client (user) certificate
    WCHAR   wzRootCA[MAX_PATH];                 ///< File path for the trusted root CA certificate
} WIFI_EAP_TLS_PARMS, *PWIFI_EAP_TLS_PARMS;

/// \brief Security related parameters 
typedef struct _WIFI_SECURITY_PARAMS
{
    WIFI_EAP_TYPE eapType;                  ///< EAP type
    union _data
    {
        WIFI_SHAREDKEY_PARMS    psk;        ///< WPA/WPA2 PSK parameters
        WIFI_EAP_PEAP_PARMS     peap;       ///< PEAP parameters
        WIFI_EAP_TLS_PARMS      tls;        ///< TLS parameters
    } data;
} WIFI_SECURITY_PARAMS, *PWIFI_SECURITY_PARAMS;

typedef struct _WDK_WPS_DEVICE_TYPE
{
    WORD wCategory;
    WORD wSubcategory;
    BYTE oui[4];
} WDK_WPS_DEVICE_TYPE, *PWDK_WPS_DEVICE_TYPE;

typedef struct _WPS_IE_INFO
{
    DWORD dwFlags;
    WORD wConfigMethods;
} WPS_IE_INFO, *PWPS_IE_INFO;

/// \brief stuct to contain WPS key params
typedef struct _WDK_WPS_KEY_PARAMS
{
    DWORD   dwSize;                         ///< structure size
    WDK_AVAIL_AP_AUTHENTICATION auth;       ///< Authentication mode (Open/WPA/WPA2)
    WDK_AVAIL_AP_ENCRYPTION     encrypt;    ///< Encryption (None/AES)
    BYTE    keylen;                         ///< Encryption key length
    BYTE    reserved1;                      ///< Reserved
    CHAR    key[64];                        ///< Encryption key
    CHAR    ssid[MAX_SSID_SZ_LEN];          ///< Reserved
    CHAR    reserved2;                      ///< Padding
    UCHAR   mac_addr[MAC_LEN];              ///< MAC address (BSSID)
} WDK_WPS_KEY_PARAMS, *PWDK_WPS_KEY_PARAMS;

/// \brief enumeration of Wi-Fi STA status values
typedef enum _WDK_STA_STATUS
{
    WDK_STA_STATUS_Unknown          = -1,   ///< Status is unknown
    WDK_STA_STATUS_Connected        = 0,    ///< STA is connected
    WDK_STA_STATUS_Connecting       = 1,    ///< STA is connecting
    WDK_STA_STATUS_Connect_Failed   = 2,    ///< STA connect failed
    WDK_STA_STATUS_Disconnecting    = 3,    ///< STA is disconnecting
    WDK_STA_STATUS_Disconnected     = 4,    ///< STA is connecting
} WDK_STA_STATUS, *PWDK_STA_STATUS;

_inline LPCWSTR StaStatus2String(WDK_STA_STATUS status)
{
    static WCHAR szBuf[32] = {0};
    LPCWSTR wzCase = NULL;
    switch(status)
    {
        CASE_TO_STRING(WDK_STA_STATUS_Unknown);
        CASE_TO_STRING(WDK_STA_STATUS_Connected);
        CASE_TO_STRING(WDK_STA_STATUS_Connecting);
        CASE_TO_STRING(WDK_STA_STATUS_Connect_Failed);
        CASE_TO_STRING(WDK_STA_STATUS_Disconnected);
        CASE_TO_STRING(WDK_STA_STATUS_Disconnecting);

        default:
        {
            StringCchPrintf(szBuf, countof(szBuf), L"WDK_STA_STATUS_%d", status);
            wzCase = szBuf;
        }
    }
    return wzCase;
}


#endif //_WDKWIFITYPES_H_

