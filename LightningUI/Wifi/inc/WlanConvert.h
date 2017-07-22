/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WlanConvert.h#1 $ */
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
/// \brief      Convert Wlan parameters to corresponding WDK parameters
//-----------------------------------------------------------------------------
/// \file

#ifndef _WLAN_CONVERT_H_
#define _WLAN_CONVERT_H_
#pragma once

#include <WdkVersion.h>
#include <WdkBldVer.h>

#include <windot11_wfd.h>

#include <WdkWifiStaApi.h>
#include <WdkMacros.h>

#ifdef WLAN_SUPPORT

#include <wlanapi.h>
#include <wlannotify.h>

/// \brief Authentication mode conversion translation structure
typedef struct _AUTH_TRANSLATE_WLAN
{
    DOT11_AUTH_ALGORITHM        alg;    ///< authentication algorithum
    WDK_AVAIL_AP_AUTHENTICATION auth;   ///< authentication mode
} AUTH_TRANSLATE_WLAN, *PAUTH_TRANSLATE_WLAN;

const AUTH_TRANSLATE_WLAN c_rgAuthTrans[] = 
    {
        { DOT11_AUTH_ALGO_80211_OPEN,       WDK_AVAIL_AP_AUTHENTICATION_Open },
        { DOT11_AUTH_ALGO_80211_SHARED_KEY, WDK_AVAIL_AP_AUTHENTICATION_Shared },
        { DOT11_AUTH_ALGO_WPA,              WDK_AVAIL_AP_AUTHENTICATION_Wpa },
        { DOT11_AUTH_ALGO_WPA_PSK,          WDK_AVAIL_AP_AUTHENTICATION_WpaPsk },
        { DOT11_AUTH_ALGO_WPA_NONE,         WDK_AVAIL_AP_AUTHENTICATION_WpaAdHoc },
        { DOT11_AUTH_ALGO_RSNA,             WDK_AVAIL_AP_AUTHENTICATION_Wpa2 },
        { DOT11_AUTH_ALGO_RSNA_PSK,         WDK_AVAIL_AP_AUTHENTICATION_Wpa2Psk },
    };

/// \brief Convert a DOT11_AUTH_ALGORITHM value to a WDK_AVAIL_AP_AUTHENTICATION value
_inline WDK_AVAIL_AP_AUTHENTICATION ToAuthenticationType( DOT11_AUTH_ALGORITHM alg )
{
    WDK_AVAIL_AP_AUTHENTICATION auth = WDK_AVAIL_AP_AUTHENTICATION_Unknown;

    for(int i=0; i<countof(c_rgAuthTrans); ++i)
    {
        if(alg == c_rgAuthTrans[i].alg)
        {
            auth = c_rgAuthTrans[i].auth;
            break;
        }
    }

    return auth;
}

/// \brief Encryption mode conversion translation structure
typedef struct _ENCRYPT_TRANSLATE_WLAN
{
    DOT11_CIPHER_ALGORITHM      cipher;     ///< Cipher algorithum
    WDK_AVAIL_AP_ENCRYPTION     encrypt;    ///< entryption mode
} ENCRYPT_TRANSLATE_WLAN, *PENCRYPT_TRANSLATE_WLAN;

const ENCRYPT_TRANSLATE_WLAN c_rgEncryptTrans[] =
    {
        { DOT11_CIPHER_ALGO_NONE,           WDK_AVAIL_AP_ENCRYPTION_None },
        { DOT11_CIPHER_ALGO_WEP40,          WDK_AVAIL_AP_ENCRYPTION_Wep },
        { DOT11_CIPHER_ALGO_TKIP,           WDK_AVAIL_AP_ENCRYPTION_Tkip },
        { DOT11_CIPHER_ALGO_CCMP,           WDK_AVAIL_AP_ENCRYPTION_Aes },
        { DOT11_CIPHER_ALGO_WEP104,         WDK_AVAIL_AP_ENCRYPTION_Wep },
        { DOT11_CIPHER_ALGO_WPA_USE_GROUP,  WDK_AVAIL_AP_ENCRYPTION_Unknown },
        { DOT11_CIPHER_ALGO_RSN_USE_GROUP,  WDK_AVAIL_AP_ENCRYPTION_Unknown },
        { DOT11_CIPHER_ALGO_WEP,            WDK_AVAIL_AP_ENCRYPTION_Wep },
        { DOT11_CIPHER_ALGO_WEP40,          WDK_AVAIL_AP_ENCRYPTION_Wep },
        { DOT11_CIPHER_ALGO_WEP104,         WDK_AVAIL_AP_ENCRYPTION_Wep },
    };

/// \brief Convert a DOT11_CIPHER_ALGORITHM value to a WDK_AVAIL_AP_ENCRYPTION value
_inline WDK_AVAIL_AP_ENCRYPTION ToEncryptionType( DOT11_CIPHER_ALGORITHM cipher )
{
    WDK_AVAIL_AP_ENCRYPTION encrypt = WDK_AVAIL_AP_ENCRYPTION_Unknown;

    for(int i=0; i<countof(c_rgEncryptTrans); ++i)
    {
        if(cipher == c_rgEncryptTrans[i].cipher)
        {
            encrypt = c_rgEncryptTrans[i].encrypt;
            break;
        }
    }

    return encrypt;
}

inline BOOL Wlan2StaStatus( WLAN_NOTIFICATION_ACM acm, __inout PWDK_STA_STATUS pStatus )
{
    typedef struct _STA_STATUS_MAP
    {
        WLAN_NOTIFICATION_ACM acm;
        WDK_STA_STATUS status;
    } STA_STATUS_MAP, *PSTA_STATUS_MAP;

    int i;
    const STA_STATUS_MAP c_rgStaStatusMap[] = 
    {
        { wlan_notification_acm_connection_attempt_fail,    WDK_STA_STATUS_Connect_Failed },
        { wlan_notification_acm_disconnected,               WDK_STA_STATUS_Disconnected },
        { wlan_notification_acm_connection_start,           WDK_STA_STATUS_Connecting },
        { wlan_notification_acm_connection_complete,        WDK_STA_STATUS_Connected },
        { wlan_notification_acm_disconnecting,              WDK_STA_STATUS_Disconnecting },
    };

    for(i=0; i<countof(c_rgStaStatusMap); ++i)
    {
        if(c_rgStaStatusMap[i].acm == acm)
        {
            *pStatus = c_rgStaStatusMap[i].status;
            return TRUE;
        }
    }

    return FALSE;
}

#endif // WLAN_SUPPORT

/// \brief Convert a DOT11_BSS_TYPE value to a WDK_AVAIL_AP_NETWORK_MGMT value
_inline WDK_AVAIL_AP_NETWORK_MGMT ToMgmtType( DOT11_BSS_TYPE type )
{
    WDK_AVAIL_AP_NETWORK_MGMT mgmt = WDK_AVAIL_AP_NETWORK_MGMT_Unknown;

    switch(type)
    {
        case dot11_BSS_type_infrastructure:
            mgmt = WDK_AVAIL_AP_NETWORK_MGMT_Infrastructure;
            break;
        case dot11_BSS_type_independent:
            mgmt = WDK_AVAIL_AP_NETWORK_MGMT_AdHoc;
            break;
        case dot11_BSS_type_any:
            mgmt = WDK_AVAIL_AP_NETWORK_MGMT_Unknown;
    }

    return mgmt;
}

#endif // _WLAN_CONVERT_H_

