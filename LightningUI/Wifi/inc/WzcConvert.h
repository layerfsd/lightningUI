/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WzcConvert.h#1 $ */
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
/// \brief      Convert WZC parameters to corresponding WDK parameters
//-----------------------------------------------------------------------------
/// \file

#ifndef _WZC_CONVERT_H_
#define _WZC_CONVERT_H_
#pragma once

#include <WdkVersion.h>
#include <WdkBldVer.h>

#ifdef WZC_SUPPORT

#include <ntddndis.h>
#include <eapol.h>
#include <wzcsapi.h>
#include <wzcmsq.h>

#include <WdkStringUtil.h>

/// \brief Convert a WDK_AVAIL_AP_NETWORK_MGMT value to a NDIS_802_11_NETWORK_INFRASTRUCTURE value.
_inline NDIS_802_11_NETWORK_INFRASTRUCTURE ToNdisInfraType( WDK_AVAIL_AP_NETWORK_MGMT mgmt )
{
    NDIS_802_11_NETWORK_INFRASTRUCTURE infra;

    switch(mgmt)
    {
        case WDK_AVAIL_AP_NETWORK_MGMT_AdHoc:           infra = Ndis802_11IBSS;             break;
        case WDK_AVAIL_AP_NETWORK_MGMT_Infrastructure:  infra = Ndis802_11Infrastructure;   break;
        default:                                        infra = Ndis802_11AutoUnknown;      break;
    }

    return infra;
}

/// \brief Convert a WDK_AVAIL_AP_ENCRYPTION value to a NDIS_802_11_ENCRYPTION_STATUS value.
_inline NDIS_802_11_ENCRYPTION_STATUS ToNdisEncryptionStatus( WDK_AVAIL_AP_ENCRYPTION encryption )
{
    NDIS_802_11_WEP_STATUS status = Ndis802_11EncryptionDisabled;

    switch(encryption)
    {
        case WDK_AVAIL_AP_ENCRYPTION_Wep:   status = Ndis802_11Encryption1Enabled;  break;
        case WDK_AVAIL_AP_ENCRYPTION_Tkip:  status = Ndis802_11Encryption2Enabled;  break;
        case WDK_AVAIL_AP_ENCRYPTION_Aes:   status = Ndis802_11Encryption3Enabled;  break;
    }

    return status;
}

/// \brief Convert a NDIS_802_11_ENCRYPTION_STATUS value to a WDK_AVAIL_AP_ENCRYPTION value.
_inline WDK_AVAIL_AP_ENCRYPTION ToEncryptionType( NDIS_802_11_ENCRYPTION_STATUS status )
{
    WDK_AVAIL_AP_ENCRYPTION encryption = WDK_AVAIL_AP_ENCRYPTION_None;

    switch(status)
    {
        case Ndis802_11Encryption1Enabled:
        case Ndis802_11Encryption1KeyAbsent:
            encryption = WDK_AVAIL_AP_ENCRYPTION_Wep;
            break;
        case Ndis802_11Encryption2Enabled:
        case Ndis802_11Encryption2KeyAbsent:
            encryption = WDK_AVAIL_AP_ENCRYPTION_Tkip;
            break;
        case Ndis802_11Encryption3Enabled:
        case Ndis802_11Encryption3KeyAbsent:
            encryption = WDK_AVAIL_AP_ENCRYPTION_Aes;
            break;
    }

    return encryption;
}

/// \brief Convert a WDK_AVAIL_AP_AUTHENTICATION value to a NDIS_802_11_AUTHENTICATION_MODE value.
_inline NDIS_802_11_AUTHENTICATION_MODE ToNdisAuthMode( WDK_AVAIL_AP_AUTHENTICATION authentication )
{
    NDIS_802_11_AUTHENTICATION_MODE mode = Ndis802_11AuthModeOpen;

    switch(authentication)
    {
        case WDK_AVAIL_AP_AUTHENTICATION_Shared:        mode = Ndis802_11AuthModeShared;    break;
        case WDK_AVAIL_AP_AUTHENTICATION_AutoSwitch:    mode = Ndis802_11AuthModeAutoSwitch;  break;
        case WDK_AVAIL_AP_AUTHENTICATION_Wpa:           mode = Ndis802_11AuthModeWPA;       break;
        case WDK_AVAIL_AP_AUTHENTICATION_WpaPsk:        mode = Ndis802_11AuthModeWPAPSK;    break;
        case WDK_AVAIL_AP_AUTHENTICATION_WpaAdHoc:      mode = Ndis802_11AuthModeWPANone;   break;
        case WDK_AVAIL_AP_AUTHENTICATION_Wpa2:          mode = Ndis802_11AuthModeWPA2;      break;
        case WDK_AVAIL_AP_AUTHENTICATION_Wpa2Psk:       mode = Ndis802_11AuthModeWPA2PSK;   break;
    }

    return mode;
}

/// \brief Convert a NDIS_802_11_AUTHENTICATION_MODE value to a WDK_AVAIL_AP_AUTHENTICATION value.
_inline WDK_AVAIL_AP_AUTHENTICATION ToAuthenticationType( NDIS_802_11_AUTHENTICATION_MODE mode )
{
    WDK_AVAIL_AP_AUTHENTICATION authentication = WDK_AVAIL_AP_AUTHENTICATION_Open;

    switch(mode)
    {
        case Ndis802_11AuthModeOpen:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_Open;
            break;
        case Ndis802_11AuthModeShared:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_Shared;
            break;
        case Ndis802_11AuthModeAutoSwitch:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_AutoSwitch;
            break;
        case Ndis802_11AuthModeWPA:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_Wpa;
            break;
        case Ndis802_11AuthModeWPAPSK:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_WpaPsk;
            break;
        case Ndis802_11AuthModeWPANone:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_WpaAdHoc;
            break;
        case Ndis802_11AuthModeWPA2:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_Wpa2;
            break;
        case Ndis802_11AuthModeWPA2PSK:
            authentication = WDK_AVAIL_AP_AUTHENTICATION_Wpa2Psk;
            break;
    }

    return authentication;
}

/// \brief Convert a NDIS_802_11_NETWORK_INFRASTRUCTURE value to a WDK_AVAIL_AP_NETWORK_MGMT value.
_inline WDK_AVAIL_AP_NETWORK_MGMT ToMgmtType( NDIS_802_11_NETWORK_INFRASTRUCTURE infra )
{
    WDK_AVAIL_AP_NETWORK_MGMT mgmt = WDK_AVAIL_AP_NETWORK_MGMT_AdHoc;

    switch(infra)
    {
        case Ndis802_11IBSS:
            mgmt = WDK_AVAIL_AP_NETWORK_MGMT_AdHoc;
            break;
        case Ndis802_11Infrastructure:
            mgmt = WDK_AVAIL_AP_NETWORK_MGMT_Infrastructure;
            break;
    }

    return mgmt;
}

/// \brief Convert a WZC_WLAN_CONFIG structure to a WDK_STA_AP_INFO structure.
_inline VOID ConvertToApInfo( __in PWZC_WLAN_CONFIG pFromConfig, __out PWDK_STA_AP_INFO pToAp )
{
    if(pToAp && pFromConfig)
    {
        memset(pToAp, 0, sizeof(*pToAp));

        UTF8ToWideChar((LPCSTR)pFromConfig->Ssid.Ssid, SZARGS(pToAp->wzSsid));
        
        pToAp->Mgmt = ToMgmtType(pFromConfig->InfrastructureMode);
        pToAp->Authentication= ToAuthenticationType(pFromConfig->AuthenticationMode);
        pToAp->Encryption = ToEncryptionType((NDIS_802_11_ENCRYPTION_STATUS)pFromConfig->Privacy);
    }
}

/// \brief Convert a WZC_WLAN_CONFIG structure to a WDK_AVAIL_AP_INFO structure.
_inline VOID ConvertToApAvail( __in PWZC_WLAN_CONFIG pFromConfig, __out PWDK_AVAIL_AP_INFO pToAp )
{
    if(pToAp && pFromConfig)
    {
        memset(pToAp, 0, sizeof(*pToAp));
        
        UTF8ToWideChar((LPCSTR)pFromConfig->Ssid.Ssid, SZARGS(pToAp->Info.wzSsid));
        
        memcpy(pToAp->bBssid, pFromConfig->MacAddress, sizeof(pToAp->bBssid));
        
        pToAp->lRssi = pFromConfig->Rssi;
        pToAp->ulFreq = pFromConfig->Configuration.DSConfig;

        pToAp->Info.Mgmt = ToMgmtType(pFromConfig->InfrastructureMode);
        pToAp->Info.Authentication= ToAuthenticationType(pFromConfig->AuthenticationMode);
        pToAp->Info.Encryption = ToEncryptionType((NDIS_802_11_ENCRYPTION_STATUS)pFromConfig->Privacy);
    }
}

/// \brief Validate for compatible encryption and autentication modes.
_inline BOOL Validate(WDK_AVAIL_AP_ENCRYPTION encryption, WDK_AVAIL_AP_AUTHENTICATION authentication)
{
    HRESULT hr = S_OK;

    switch(authentication)
    {
        case WDK_AVAIL_AP_AUTHENTICATION_Open:
            //wep or none
            CBR( WDK_AVAIL_AP_ENCRYPTION_None == encryption 
                || WDK_AVAIL_AP_ENCRYPTION_Wep == encryption ) ;
            break;
        case WDK_AVAIL_AP_AUTHENTICATION_Shared:
        case WDK_AVAIL_AP_AUTHENTICATION_AutoSwitch:
            //wep
            CBR( WDK_AVAIL_AP_ENCRYPTION_Wep == encryption ) ;
            break;
        case WDK_AVAIL_AP_AUTHENTICATION_Wpa:
        case WDK_AVAIL_AP_AUTHENTICATION_WpaPsk:
        case WDK_AVAIL_AP_AUTHENTICATION_Wpa2:
        case WDK_AVAIL_AP_AUTHENTICATION_Wpa2Psk:
            //wep/tkip/aes
            CBR( WDK_AVAIL_AP_ENCRYPTION_Tkip == encryption 
                || WDK_AVAIL_AP_ENCRYPTION_Aes == encryption ) ;
            break;

        default:
            // not supported ?
            CBR(FALSE);
    }

Error:
    return SUCCEEDED(hr);
}

inline BOOL Wzc2StaStatus( DWORD dwWzcStatus, __inout PWDK_STA_STATUS pStatus )
{
    typedef struct _STA_STATUS_MAP
    {
        DWORD dwWzcStatus;
        WDK_STA_STATUS status;
    } STA_STATUS_MAP, *PSTA_STATUS_MAP;
    
    int i;
    const STA_STATUS_MAP c_rgStaStatusMap[] = 
    {
        { WZC_ASSOCIATED_NO_8021X,          WDK_STA_STATUS_Connected },
        { WZC_AUTHENTICATED,                WDK_STA_STATUS_Connected },
            
        { WZC_ASSOCIATING,                  WDK_STA_STATUS_Connecting},
        { WZC_AUTHENTICATING,               WDK_STA_STATUS_Connecting },
        { WZC_FAILED_AUTH_WILL_RETRY,       WDK_STA_STATUS_Connecting },
        { WZC_REAUTHENTICATING,             WDK_STA_STATUS_Connecting },
        { WZC_ASSOCIATED_REQUIRES_8021X,    WDK_STA_STATUS_Connecting },
            
        { WZC_CANCEL_AUTH,                  WDK_STA_STATUS_Disconnected },
        { WZC_PREFERED_LIST_EXHAUSTED,      WDK_STA_STATUS_Disconnected },
        
        { WZC_FAILED_ASSOCIATION,           WDK_STA_STATUS_Connect_Failed},
        { WZC_FAILED_AUTH_NO_RETRY,         WDK_STA_STATUS_Connect_Failed },
    };

    for(i=0; i<countof(c_rgStaStatusMap); ++i)
    {
        if(c_rgStaStatusMap[i].dwWzcStatus == dwWzcStatus)
        {
            *pStatus = c_rgStaStatusMap[i].status;
            return TRUE;
        }
    }

    return FALSE;
}
    
#endif // WZC_SUPPORT

#endif // _WZC_CONVERT_H_

