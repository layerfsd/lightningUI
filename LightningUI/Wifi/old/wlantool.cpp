//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
// ----------------------------------------------------------------------------
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// ----------------------------------------------------------------------------
//
// Module Name:
//
//     wlantool.cpp
//
// Abstract:
//
//     Program to control ACM (Auto Config Module) service to configure WiFi.
//
// ----------------------------------------------------------------------------

// BUGBUG - Remove unnecessary Headers
// #include <windows.h>
// #include <stdio.h>
// #include <stdarg.h>
// 
// #include <Eaphostpeerconfigapis.h>
// #include <wlanapi.h>
// 
// #include <rpcdce.h>
// #include <rpc.h>
// #include <atlbase.h>
// #include <msxml.h>

#include "wlantool.h"

// ========================================================================== //
// Macros:
// ========================================================================== //

// Retrieves the number elements in the specified array:
//
#ifndef COUNTOF
#define COUNTOF(arr)  (sizeof(arr) / sizeof(arr[0]))
#endif

// ========================================================================== //
// Logging:
// ========================================================================== //

// Determines whether we can write output to the console:
//
static BOOL g_bOutputToConsoleCapable = TRUE;

// ========================================================================== //
// Mac Address Formatting: 
// ========================================================================== //

// -----------------------------------------------------------------------------
BOOL
ValidateMacAddress
// Validates the specified raw MAC address.
(
    IN  const PWLAN_RAW_DATA prdMAC
)
{
    if (prdMAC == NULL           || 
        prdMAC->DataBlob == NULL || 
        prdMAC->dwDataSize != 6  || 
       (prdMAC->DataBlob[0] == 0    && 
        prdMAC->DataBlob[1] == 0    && 
        prdMAC->DataBlob[2] == 0    && 
        prdMAC->DataBlob[3] == 0    && 
        prdMAC->DataBlob[4] == 0    && 
        prdMAC->DataBlob[5] == 0))
        return FALSE;
    else
        return TRUE;
} // ValidateMacAddress()

// -----------------------------------------------------------------------------
LPWSTR
FormatMacAddress
// Formats the specified raw MAC address.
(
                            IN  const PWLAN_RAW_DATA prdMAC,
  __out_ecount(BufferChars) OUT LPWSTR               pBuffer,
                            IN  int                  BufferChars
)
{
    if (!ValidateMacAddress(prdMAC))
    {
        StringCchCopyW(pBuffer, BufferChars, L"<NULL>");
    }
    else
    {
        StringCchPrintfW(pBuffer, BufferChars,
                         L"%02X:%02X:%02X:%02X:%02X:%02X",
                         (UINT)prdMAC->DataBlob[0],
                         (UINT)prdMAC->DataBlob[1],
                         (UINT)prdMAC->DataBlob[2],
                         (UINT)prdMAC->DataBlob[3],
                         (UINT)prdMAC->DataBlob[4],
                         (UINT)prdMAC->DataBlob[5]);
    }

    free(prdMAC);
    return pBuffer;
} // FormatMacAddress()

// -----------------------------------------------------------------------------
LPWSTR
FormatMacAddress
// Formats the specified raw MAC address.
(
                            IN  const DOT11_MAC_ADDRESS        &MAC,
  __out_ecount(BufferChars) OUT LPWSTR                         pBuffer,
                            IN  int                            BufferChars
)
{
    size_t size = FIELD_OFFSET(WLAN_RAW_DATA,DataBlob) + sizeof(MAC);
    PWLAN_RAW_DATA prdBuffer = (PWLAN_RAW_DATA)malloc(size);
    prdBuffer->dwDataSize = sizeof(MAC);
    memcpy(prdBuffer->DataBlob,&MAC,prdBuffer->dwDataSize);
   
    // This call frees prdBuffer memory
    return FormatMacAddress(prdBuffer, pBuffer, BufferChars);
} // FormatMacAddress()
// -----------------------------------------------------------------------------
LPWSTR
FormatSsid
// Formats the specified raw SSID.
(
                            IN  const PWLAN_RAW_DATA prdSsid,
  __out_ecount(BufferChars) OUT LPWSTR               pBuffer,
                            IN  int                  BufferChars
)
{
    WCHAR outBuf[DOT11_SSID_MAX_LENGTH+1];
    if (prdSsid == NULL
     || prdSsid->DataBlob == NULL
     || prdSsid->dwDataSize == 0
     || prdSsid->DataBlob[0] == (BYTE)0)
    {
        StringCchCopyW(pBuffer, BufferChars, L"<NULL>");
    }
    else
    {
        int ssidLen = static_cast<int>(prdSsid->dwDataSize);
        if (ssidLen > DOT11_SSID_MAX_LENGTH)
            ssidLen = DOT11_SSID_MAX_LENGTH;
        int wchars = MultiByteToWideChar(CP_ACP, 0,
                                        (LPCSTR)(prdSsid->DataBlob), ssidLen,
                                         outBuf, DOT11_SSID_MAX_LENGTH+1);
        if (0 >= wchars || wchars > DOT11_SSID_MAX_LENGTH)
            StringCchCopyW(pBuffer, BufferChars, L"<INVALID SSID>");
        else
        {
            outBuf[wchars] = L'\0';
            StringCchCopyW(pBuffer, BufferChars, outBuf);
        }
    }
    free(prdSsid);
    return pBuffer;
} // FormatSsid()

// -----------------------------------------------------------------------------
LPWSTR
FormatSsid
// Formats the specified SSID.
(
                            IN  const DOT11_SSID &Ssid,
  __out_ecount(BufferChars) OUT LPWSTR           pBuffer,
                            IN  int              BufferChars
)
{
    size_t size = FIELD_OFFSET(WLAN_RAW_DATA,DataBlob) + Ssid.uSSIDLength * sizeof(Ssid.ucSSID[0]);
    PWLAN_RAW_DATA prdBuffer = (PWLAN_RAW_DATA)malloc(size);
    prdBuffer->dwDataSize = Ssid.uSSIDLength * sizeof(Ssid.ucSSID[0]);
    memcpy(prdBuffer->DataBlob,Ssid.ucSSID,prdBuffer->dwDataSize);

    // This call free's prdBuffer memory
    return FormatSsid(prdBuffer, pBuffer, BufferChars);
} // FormatSsid()

// -----------------------------------------------------------------------------

// Provides a generic mechanism for translating between enumerations
// and string values:
//
// -----------------------------------------------------------------------------

static int UNKNOWN_ENUM = 0xFFFFFFFF;

// A Map to store Enums and Corresponding String Maps

CONST LPCWSTR BOOLToString(CONST PBOOL pBool)
{
    return *pBool?TEXT("TRUE"):TEXT("FALSE");
}

struct EnumMap
{
    CONST int   code;
    CONST TCHAR *name;
    CONST TCHAR *alias;
};


template <class T> const TCHAR *
EnumMap2String(
    IN CONST EnumMap      Map[], 
    IN CONST int          MapSize,
    IN CONST T            Code, 
    IN CONST TCHAR *Unknown)
{
    int comp = (int)Code;
    for (int mx = 0 ; mx < MapSize ; ++mx)
    {
        if (Map[mx].code == comp)
            return Map[mx].name;
    }
    return Unknown;
}

template <class T> T
String2EnumMap(
    IN CONST EnumMap      Map[], 
    IN CONST int          MapSize, 
    IN CONST TCHAR        *Name, 
    IN CONST T            Unknown)
{
    for (int mx = 0 ; mx < MapSize ; ++mx)
    {
        if (_tcsicmp(Map[mx].name, Name) == 0 ||
            _tcsicmp(Map[mx].alias,Name) == 0)
            return (T)Map[mx].code;
    }
    return Unknown;
}   

// -----------------------------------------------------------------------------

// Class for Converting between String and Enum

class EnumStringMapper
{
    public:
                     EnumStringMapper(
                         IN CONST EnumMap emap[],
                         IN  CONST DWORD   mapsz,
                         IN  CONST DWORD   enm,
                         OUT CONST TCHAR*  str);
        const int    ToEnum(IN CONST TCHAR* str);
        const TCHAR* ToString(IN CONST int enm);
    private:
        CONST EnumMap  *map;
        CONST DWORD    mapsize;
        CONST int      unknownEnum;
        CONST TCHAR*   unknownStr;        
};

EnumStringMapper::
EnumStringMapper(
    IN CONST EnumMap emap[],
    IN CONST DWORD   mapsz,
    IN CONST DWORD   enm, 
    IN CONST TCHAR*  str):
    map(emap),
    mapsize(mapsz),
    unknownEnum(enm),
    unknownStr(str)
{
}

const int
EnumStringMapper::
ToEnum(IN CONST TCHAR* str)
{
    return String2EnumMap(map, 
                          mapsize,
                          str,
                          unknownEnum);        
}

const TCHAR*
EnumStringMapper::
ToString(IN CONST int enm)
{
    return EnumMap2String(map, 
                          mapsize,
                          enm,
                          TEXT("unknown"));    
}

// Profile Format
static TCHAR szProfileFmt[] = 
    
    L"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
    L"<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">"
    L"<name>%s</name>"
    L"<SSIDConfig>"
    L"<SSID>"
    L"<name>%s</name>"
    L"</SSID>"
    L"<nonBroadcast>%s</nonBroadcast>"
    L"</SSIDConfig>"
    L"<connectionType>%s</connectionType>"
    L"<connectionMode>%s</connectionMode>"
    L"<MSM>"
    L"<security>"                                                     
    L"<authEncryption>"                                                
    L"<authentication>%s</authentication>"
    L"<encryption>%s</encryption>"
    L"<useOneX>%s</useOneX>"
    L"</authEncryption>"
    L"<sharedKey>"
    L"<keyType>%s</keyType>"
    L"<protected>false</protected>"
    L"<keyMaterial>%s</keyMaterial>"
    L"</sharedKey>"    
    L"<keyIndex>%s</keyIndex>"
    L"<OneX xmlns=\"http://www.microsoft.com/networking/OneX/v1\">"
    L"<EAPConfig>"
    L"<EapHostConfig xmlns=\"http://www.microsoft.com/provisioning/EapHostConfig\" "
    L"xmlns:eapCommon=\"http://www.microsoft.com/provisioning/EapCommon\" " 
    L"xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapMethodConfig\">"
    L"<EapMethod>"
    L"<eapCommon:Type>%d</eapCommon:Type>"
    L"<eapCommon:AuthorId>%d</eapCommon:AuthorId>"
    L"</EapMethod>"
    L"<ConfigBlob/>"
    L"</EapHostConfig>"
    L"</EAPConfig>"
    L"</OneX>"
    L"</security>"
    L"</MSM>"
    L"</WLANProfile>";


// Enum String Maps

static EnumMap 
s_InterfaceStates[] =
{
    { wlan_interface_state_not_ready,              TEXT("wlan_interface_state_not_ready")},
    { wlan_interface_state_connected,              TEXT("wlan_interface_state_connected")},
    { wlan_interface_state_ad_hoc_network_formed,  TEXT("wlan_interface_state_ad_hoc_network_formed")},
    { wlan_interface_state_disconnecting,          TEXT("wlan_interface_state_disconnecting")},
    { wlan_interface_state_disconnected,           TEXT("wlan_interface_state_disconnected")},   
    { wlan_interface_state_associating,            TEXT("wlan_interface_state_associating")},    
    { wlan_interface_state_discovering,            TEXT("wlan_interface_state_discovering")},      
    { wlan_interface_state_authenticating,         TEXT("wlan_interface_state_authenticating")},            
    { UNKNOWN_ENUM,                                TEXT("wlan_interface_state_unknown")},            
};

static EnumMap 
s_OpcodeValueTypes[] =
{
    { wlan_opcode_value_type_query_only,          TEXT("wlan_opcode_value_type_query_only")},
    { wlan_opcode_value_type_set_by_group_policy, TEXT("wlan_opcode_value_type_set_by_group_policy")},
    { wlan_opcode_value_type_set_by_user,         TEXT("wlan_opcode_value_type_set_by_user")},
    { wlan_opcode_value_type_invalid,             TEXT("wlan_opcode_value_type_invalid")}
};

static EnumMap 
s_InterfaceTypes[] =
{
    { wlan_interface_type_emulated_802_11 ,       TEXT("wlan_interface_type_emulated_802_11")},
    { wlan_interface_type_native_802_11,          TEXT("wlan_interface_type_native_802_11")},
    { wlan_interface_type_invalid,                TEXT("wlan_interface_type_invalid")},
};

static EnumMap 
s_OpcodeValues[] =
{
    { wlan_intf_opcode_autoconf_start,                            TEXT("wlan_intf_opcode_autoconf_start")},
    { wlan_intf_opcode_autoconf_enabled,                          TEXT("wlan_intf_opcode_autoconf_enabled")},
    { wlan_intf_opcode_background_scan_enabled,                   TEXT("wlan_intf_opcode_background_scan_enabled")},
    { wlan_intf_opcode_media_streaming_mode,                      TEXT("wlan_intf_opcode_media_streaming_mode")},
    { wlan_intf_opcode_radio_state,                               TEXT("wlan_intf_opcode_radio_state")},
    { wlan_intf_opcode_bss_type,                                  TEXT("wlan_intf_opcode_bss_type")},
    { wlan_intf_opcode_interface_state,                           TEXT("wlan_intf_opcode_interface_state")},
    { wlan_intf_opcode_current_connection,                        TEXT("wlan_intf_opcode_current_connection")},
    { wlan_intf_opcode_channel_number,                            TEXT("wlan_intf_opcode_channel_number")},
    { wlan_intf_opcode_supported_infrastructure_auth_cipher_pairs,TEXT("wlan_intf_opcode_supported_infrastructure_auth_cipher_pairs")},
    { wlan_intf_opcode_supported_adhoc_auth_cipher_pairs,         TEXT("wlan_intf_opcode_supported_adhoc_auth_cipher_pairs")},
    { wlan_intf_opcode_supported_country_or_region_string_list,   TEXT("wlan_intf_opcode_supported_country_or_region_string_list")},
    { wlan_intf_opcode_current_operation_mode,                    TEXT("wlan_intf_opcode_current_operation_mode")},
    { wlan_intf_opcode_supported_safe_mode,                       TEXT("wlan_intf_opcode_supported_safe_mode")},
    { wlan_intf_opcode_certified_safe_mode,                       TEXT("wlan_intf_opcode_certified_safe_mode")},
    { wlan_intf_opcode_autoconf_end,                              TEXT("wlan_intf_opcode_autoconf_end")},
    { wlan_intf_opcode_msm_start,                                 TEXT("wlan_intf_opcode_msm_start")},
    { wlan_intf_opcode_statistics,                                TEXT("wlan_intf_opcode_statistics")},
    { wlan_intf_opcode_rssi,                                      TEXT("wlan_intf_opcode_rssi")},
    { wlan_intf_opcode_msm_end,                                   TEXT("wlan_intf_opcode_msm_end")},
    { wlan_intf_opcode_security_start,                            TEXT("wlan_intf_opcode_security_start")},
    { wlan_intf_opcode_security_end,                              TEXT("wlan_intf_opcode_security_end")},
    { wlan_intf_opcode_ihv_start,                                 TEXT("wlan_intf_opcode_ihv_start")},
    { wlan_intf_opcode_ihv_end,                                   TEXT("wlan_intf_opcode_ihv_end")}    
};

static EnumMap 
s_RadioStates[] =
{
    { dot11_radio_state_unknown, TEXT("dot11_radio_state_unknown")},
    { dot11_radio_state_on,      TEXT("dot11_radio_state_on")},
    { dot11_radio_state_off,     TEXT("dot11_radio_state_off")}
};

static EnumMap 
s_BssTypes[] =
{
    { dot11_BSS_type_infrastructure,TEXT("dot11_BSS_type_infrastructure") , TEXT("ESS")},
    { dot11_BSS_type_independent,   TEXT("dot11_BSS_type_independent")    , TEXT("IBSS")},
    { dot11_BSS_type_any,           TEXT("dot11_BSS_type_any")            , TEXT("")},
};

static EnumMap 
s_ConnectionModes[] =
{
    { wlan_connection_mode_profile,           TEXT("wlan_connection_mode_profile")},
    { wlan_connection_mode_temporary_profile, TEXT("wlan_connection_mode_temporary_profile")},
    { wlan_connection_mode_discovery_secure,  TEXT("wlan_connection_mode_discovery_secure")},
    { wlan_connection_mode_discovery_unsecure,TEXT("wlan_connection_mode_discovery_unsecure")},
    { wlan_connection_mode_auto,              TEXT("wlan_connection_mode_auto")},
    { wlan_connection_mode_invalid,           TEXT("wlan_connection_mode_invalid")}    
};

static EnumMap 
s_PhyTypes[] =
{
    { dot11_phy_type_unknown,    TEXT("dot11_phy_type_unknown")},
    { dot11_phy_type_any,        TEXT("dot11_phy_type_any")},
    { dot11_phy_type_fhss,       TEXT("dot11_phy_type_fhss")},
    { dot11_phy_type_dsss,       TEXT("dot11_phy_type_dsss")},
    { dot11_phy_type_irbaseband, TEXT("dot11_phy_type_irbaseband")},
    { dot11_phy_type_ofdm,       TEXT("dot11_phy_type_ofdm")} ,   
    { dot11_phy_type_hrdsss,     TEXT("dot11_phy_type_hrdsss")},
    { dot11_phy_type_erp,        TEXT("dot11_phy_type_erp")},
    { dot11_phy_type_IHV_start,  TEXT("dot11_phy_type_IHV_start")},
    { dot11_phy_type_IHV_end,    TEXT("dot11_phy_type_IHV_end")}    
};

static EnumMap 
s_AuthAlgorithm[] =
{
    { DOT11_AUTH_ALGO_80211_OPEN,       TEXT("DOT11_AUTH_ALGO_80211_OPEN")      , TEXT("open")},
    { DOT11_AUTH_ALGO_80211_SHARED_KEY, TEXT("DOT11_AUTH_ALGO_80211_SHARED_KEY"), TEXT("shared")},
    { DOT11_AUTH_ALGO_WPA,              TEXT("DOT11_AUTH_ALGO_WPA")             , TEXT("WPA")},
    { DOT11_AUTH_ALGO_WPA_PSK,          TEXT("DOT11_AUTH_ALGO_WPA_PSK")         , TEXT("WPAPSK")},
    { DOT11_AUTH_ALGO_WPA_NONE,         TEXT("DOT11_AUTH_ALGO_WPA_NONE")        , TEXT("")},
    { DOT11_AUTH_ALGO_RSNA,             TEXT("DOT11_AUTH_ALGO_RSNA")            , TEXT("WPA2")} ,   
    { DOT11_AUTH_ALGO_RSNA_PSK,         TEXT("DOT11_AUTH_ALGO_RSNA_PSK")        , TEXT("WPA2PSK")},
    { DOT11_AUTH_ALGO_IHV_START,        TEXT("DOT11_AUTH_ALGO_IHV_START")       , TEXT("")},
    { DOT11_AUTH_ALGO_IHV_END,          TEXT("DOT11_AUTH_ALGO_IHV_END")         , TEXT("")}
};

static EnumMap 
s_CipherAlgorithm[] =
{
    { DOT11_CIPHER_ALGO_NONE,          TEXT("DOT11_CIPHER_ALGO_NONE")           , TEXT("none")},
    { DOT11_CIPHER_ALGO_WEP40,         TEXT("DOT11_CIPHER_ALGO_WEP40")          , TEXT("")},
    { DOT11_CIPHER_ALGO_TKIP,          TEXT("DOT11_CIPHER_ALGO_TKIP")           , TEXT("TKIP")},
    { DOT11_CIPHER_ALGO_CCMP,          TEXT("DOT11_CIPHER_ALGO_CCMP")           , TEXT("AES")},
    { DOT11_CIPHER_ALGO_WEP104,        TEXT("DOT11_CIPHER_ALGO_WEP104")         , TEXT("")},
    { DOT11_CIPHER_ALGO_WPA_USE_GROUP, TEXT("DOT11_CIPHER_ALGO_WPA_USE_GROUP")  , TEXT("")},
    { DOT11_CIPHER_ALGO_RSN_USE_GROUP, TEXT("DOT11_CIPHER_ALGO_RSN_USE_GROUP")  , TEXT("")},
    { DOT11_CIPHER_ALGO_WEP,           TEXT("DOT11_CIPHER_ALGO_WEP")            , TEXT("WEP")},
    { DOT11_CIPHER_ALGO_IHV_START,     TEXT("DOT11_CIPHER_ALGO_IHV_START")      , TEXT("")},
    { DOT11_CIPHER_ALGO_IHV_END,       TEXT("DOT11_CIPHER_ALGO_IHV_END")        , TEXT("")}
};

static EnumMap 
s_OperationModes[] =
{
    { DOT11_OPERATION_MODE_UNKNOWN,           TEXT("DOT11_OPERATION_MODE_UNKNOWN")},
    { DOT11_OPERATION_MODE_STATION,           TEXT("DOT11_OPERATION_MODE_STATION")},
    { DOT11_OPERATION_MODE_AP,                TEXT("DOT11_OPERATION_MODE_AP")},
    { DOT11_OPERATION_MODE_EXTENSIBLE_STATION,TEXT("DOT11_OPERATION_MODE_EXTENSIBLE_STATION")},
    { DOT11_OPERATION_MODE_NETWORK_MONITOR,   TEXT("DOT11_OPERATION_MODE_NETWORK_MONITOR")}
};

// -----------------------------------------------------------------------------

#ifdef UNDER_CE
typedef int (__cdecl *PFN_wprintf)(const wchar_t *, ...);
static PFN_wprintf v_pfn_wprintf = NULL;
static HMODULE     v_hCoreDLL = NULL;
#endif

// -----------------------------------------------------------------------------

void
PrintMsg
// Logs the specified message to both the console and debug.
// Always adds a newline to the end of the message.
(
    LPCWSTR pFormat,
    ...)
{
    HRESULT hr;
    va_list argList;
    WCHAR    buffer[2048];
    wcscpy_s(buffer,2048,L"[WLANTOOL] ");
    int offset = wcslen(buffer);

    va_start(argList, pFormat);
  __try
    {
        hr = StringCchVPrintfExW(buffer + offset, COUNTOF(buffer) - offset, NULL, NULL, STRSAFE_IGNORE_NULLS, pFormat, argList);
    }
  __except(1)
    {
        hr = StringCchPrintfW(buffer, COUNTOF(buffer), L"Error formatting \"%.128s\"", pFormat);
    }
    va_end(argList);

    if (SUCCEEDED(hr) || hr == STRSAFE_E_INSUFFICIENT_BUFFER)
	{
   /*       // Check to see if Output to Console is Capable
        if (g_bOutputToConsoleCapable && NULL == v_pfn_wprintf)
        {
#ifdef UNDER_CE
            if (NULL == v_pfn_wprintf)
            {
                // Since not all configs contain the wprintf function we'll
                // try to find it.  If it's not there we'll default to using
                // OutputDebugString.
                v_hCoreDLL = LoadLibraryW(L"coredll.dll");
                if (NULL != v_hCoreDLL)
                {
                    v_pfn_wprintf = (PFN_wprintf)GetProcAddressW(v_hCoreDLL, L"wprintf");
                }
                
            }
            if (NULL == v_pfn_wprintf)
            {
                // Couldn't find the entry point for console output
                g_bOutputToConsoleCapable = FALSE;
            }
#endif            
        }
        if(v_pfn_wprintf)
        {
            v_pfn_wprintf(L"%s\n", buffer);
#ifndef UNDER_CE
            _putts(buffer);
            _putts(TEXT("\n"));
#endif
        }*/
       // OutputDebugStringW(buffer);
		wcscat_s(buffer,COUNTOF(buffer),L"\r\n");

		RETAILMSG( 1, ( buffer) );
    }
} // PrintfMsg()

// -----------------------------------------------------------------------------
void
PrintMsgList
// Logs the specified list of messages
(
    IN LPCWSTR *szMsgBlock
)
{
    for (LPCWSTR *sz1 = szMsgBlock ; *sz1 ; sz1++)
    {
        PrintMsg(L"%s", *sz1);
    }
} // PrintMsgList()


DWORD
WlanInterfaces::
PrintMacFrameStatistics(IN PWLAN_MAC_FRAME_STATISTICS pStats)
{
    if(!pStats)
        return ERROR_INVALID_PARAMETER;
    PrintMsg(L"ullTransmittedFrameCount : %I64u", pStats->ullTransmittedFrameCount);
    PrintMsg(L"ullReceivedFrameCount    : %I64u", pStats->ullReceivedFrameCount);
    PrintMsg(L"ullWEPExcludedCount      : %I64u", pStats->ullWEPExcludedCount);
    PrintMsg(L"ullTKIPLocalMICFailures  : %I64u", pStats->ullTKIPLocalMICFailures);
    PrintMsg(L"ullTKIPReplays           : %I64u", pStats->ullTKIPReplays);
    PrintMsg(L"ullTKIPICVErrorCount     : %I64u", pStats->ullTKIPICVErrorCount);
    PrintMsg(L"ullCCMPReplays           : %I64u", pStats->ullCCMPReplays);
    PrintMsg(L"ullCCMPDecryptErrors     : %I64u", pStats->ullCCMPDecryptErrors);
    PrintMsg(L"ullWEPUndecryptableCount : %I64u", pStats->ullWEPUndecryptableCount);
    PrintMsg(L"ullWEPICVErrorCount      : %I64u", pStats->ullWEPICVErrorCount);
    PrintMsg(L"ullDecryptSuccessCount   : %I64u", pStats->ullDecryptSuccessCount);
    PrintMsg(L"ullDecryptFailureCount   : %I64u",pStats->ullDecryptFailureCount);    

    return ERROR_SUCCESS;
}

// -----------------------------------------------------------------------------

void 
WlanInterfaces::
QueryBoolFromInterface(CONST HANDLE           hClientHandle, 
                                CONST GUID*            pGuid, 
                                CONST WLAN_INTF_OPCODE opcode, 
                                PDWORD           pdwDataSize, 
                                PVOID*           ppData, 
                                PWLAN_OPCODE_VALUE_TYPE pOpcodeType)
{
    DWORD dwError;    
    dwError = WlanQueryInterface(hClientHandle, 
                                 pGuid, 
                                 opcode, 
                                 NULL, 
                                 pdwDataSize, 
                                 ppData, 
                                 pOpcodeType);
    EnumStringMapper opcodemapper(s_OpcodeValues,COUNTOF(s_OpcodeValues),UNKNOWN_ENUM,TEXT("unknown"));
    EnumStringMapper opcodetypemapper(s_OpcodeValueTypes,COUNTOF(s_OpcodeValueTypes),UNKNOWN_ENUM,TEXT("unknown"));
    if(ERROR_SUCCESS != dwError)
    {
        PrintMsg(L"\t%-40s %-6s Error : %u %s", 
                 opcodemapper.ToString(opcode), 
                 TEXT("FAILED"),
                 dwError,
                 opcodetypemapper.ToString(*pOpcodeType));
    }
    else
    {
        PrintMsg(L"\t%-40s %-6s %s",opcodemapper.ToString(opcode), BOOLToString((PBOOL)*ppData),opcodetypemapper.ToString((WLAN_OPCODE_VALUE_TYPE)(*pOpcodeType)) );
    }

}
BOOL 
WlanInterfaces::
SetKey(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strSsidName,IN CONST LPCWSTR strKey)
{
	BOOL bResult = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	WCHAR buff[1024];
	DWORD buffChars = COUNTOF(buff);
	WCHAR* strPrf = NULL;

	PrintMsg(L">>>>SetKey strAdapter:%s,SSID: %s,key:%s",strAdapter,strSsidName,strKey);

	do
	{
		EAP_METHOD_INFO_ARRAY eapMethodInfoArray = {0};
		// Get Eap Methods
		EAP_ERROR* pEapErr = NULL;
		DWORD dwError = EapHostPeerGetMethods(&eapMethodInfoArray,&pEapErr);
		if(ERROR_SUCCESS != dwError)
		{
			PrintMsg(
				L"EapHostPeerGetMethods FAILED Err: %ld Reason: %s Fix: %s",
				dwError,
				pEapErr->pRootCauseString,
				pEapErr->pRepairString);
			EapHostPeerFreeErrorMemory(pEapErr);
			bResult = FALSE;
			break;
		}     

		//get network:
		WLAN_AVAILABLE_NETWORK *pNetwork=NULL;
		PWLAN_AVAILABLE_NETWORK_LIST pList=NULL;
		if(!GetNetworksList(strAdapter,&pList))
			break;

		for(DWORD i=0;i<pList->dwNumberOfItems;i++)
		{
			FormatSsid(pList->Network[i].dot11Ssid,buff,buffChars);

			if(0 == wcscmp(buff,strSsidName))
			{
				PrintMsg(L"Get Interface Matches the given SSID: %s,index:%d",strSsidName,i);
				pNetwork=&pList->Network[i];
				break;
			}
		}

		if(pNetwork==NULL)
		{
			PrintMsg(L"No Interface Matches the given SSID: %s",strSsidName);
			break;
		}

		//set profile here:
		EnumStringMapper bssTypes(s_BssTypes,COUNTOF(s_BssTypes),UNKNOWN_ENUM,TEXT("UNKNOWN"));
		EnumStringMapper phyTypes(s_PhyTypes,COUNTOF(s_PhyTypes),UNKNOWN_ENUM,TEXT("UNKNOWN"));
		EnumStringMapper authTypes(s_AuthAlgorithm,COUNTOF(s_AuthAlgorithm),UNKNOWN_ENUM,TEXT("UNKNOWN"));
		EnumStringMapper cipherTypes(s_CipherAlgorithm,COUNTOF(s_CipherAlgorithm),UNKNOWN_ENUM,TEXT("UNKNOWN"));

		//SSID:
		LPCWSTR wszSsidToConnect = NULL;
		wszSsidToConnect=strSsidName;

		// adhoc? or infrastructure net?
		LPCWSTR wszConnMode = NULL;
		LPCWSTR wszConnType = NULL;
		if(pNetwork->dot11BssType == dot11_BSS_type_independent)
		{
			wszConnType = L"IBSS";
			wszConnMode = L"manual";
		}
		else
		{
			wszConnType = L"ESS";
			wszConnMode = L"auto";
		}

		// Default to connecting to only APs that are beaconing.
		LPCWSTR wszNonBroadcast = L"false";

		// Authentication
		LPCWSTR wszAuthMode = NULL;
		switch(pNetwork->dot11DefaultAuthAlgorithm)
		{
		case DOT11_AUTH_ALGO_80211_OPEN:
			wszAuthMode=TEXT("open");
			break;
		case DOT11_AUTH_ALGO_80211_SHARED_KEY:
			wszAuthMode=TEXT("shared");
			break;
		case DOT11_AUTH_ALGO_WPA:
			wszAuthMode=TEXT("WPA");
			break;
		case DOT11_AUTH_ALGO_WPA_PSK:
			wszAuthMode=TEXT("WPAPSK");
			break;
		case DOT11_AUTH_ALGO_WPA_NONE:
			wszAuthMode=TEXT("");
			break;
		case DOT11_AUTH_ALGO_RSNA:
			wszAuthMode=TEXT("WPA2");
			break;
		case DOT11_AUTH_ALGO_RSNA_PSK:
			wszAuthMode=TEXT("WPA2PSK");
			break;
		case DOT11_AUTH_ALGO_IHV_START:
			wszAuthMode=TEXT("");
			break;
		case DOT11_AUTH_ALGO_IHV_END:
			wszAuthMode=TEXT("");
			break;
		}

		// Encryption
		LPCWSTR wszKeyType = NULL;
		LPCWSTR wszCipherMode = NULL;
		switch(pNetwork->dot11DefaultCipherAlgorithm)
		{
		case DOT11_CIPHER_ALGO_NONE:
			wszCipherMode=TEXT("none");
			break;
		case DOT11_CIPHER_ALGO_TKIP:
			wszCipherMode=TEXT("TKIP");
			break;
		case DOT11_CIPHER_ALGO_CCMP:
			wszCipherMode=TEXT("AES");
			break;
		case DOT11_CIPHER_ALGO_WEP:
			wszCipherMode=TEXT("WEP");
			break;
		default:
			wszCipherMode=TEXT("");
		}

		if(wcscmp(wszCipherMode,L"WEP") == 0)
			wszKeyType = L"networkKey";
		else
			wszKeyType = L"passPhrase";

		// Key
		LPCWSTR wszEncryptionKey = NULL;
		WCHAR wszKeyString[500];
		ZeroMemory(wszKeyString,sizeof(WCHAR)*COUNTOF(wszKeyString));
		LPCWSTR wszKey = NULL;
		LPCWSTR wszKeyIndex = NULL;

 		if (!strKey)
			wszEncryptionKey = L"auto";
		else
			wszEncryptionKey=strKey;

		wcscpy_s(wszKeyString,500,wszEncryptionKey);
		BOOL bNeed8021X = FALSE;
		if (!InterpretEncryptionKeyValue(
			wszKeyString, 
			wszAuthMode,
			wszCipherMode,
			&wszKey,
			&wszKeyIndex, 
			bNeed8021X))
			break;

		// Fill with dummy values so that Profile Formatting doesnt choke
		if(!wszKey)
			wszKey = L"0";
		if(!wszKeyIndex)
			wszKeyIndex = L"0";

		// One X
		LPCWSTR wszUseOneX = NULL;

		if (bNeed8021X)
		{
			wszKey = L"0";
			wszUseOneX = L"true";
		}
		else
			wszUseOneX = L"false";            

		// OneX
		LPCWSTR szEapAuthMode = NULL;
		BYTE EapType = 0;
		DWORD dwAuthorID = 0;

		if (FALSE/*GetOption(argc, argv, L"eap", &szEapAuthMode) > 0*/)
		{
			if(!bNeed8021X)
			{
				PrintMsg(
					L"-eap option is not valid for -auth %s -encr %s connnection Configuration",
					wszAuthMode,
					wszCipherMode);
				break;
			}
			if(0 == eapMethodInfoArray.dwNumberOfMethods)
			{
				PrintMsg(L"There are no EAP methods Available in the System");
				PrintMsg(L" -eap option is invalid");
				break;
			}
			BOOL bEapMatched = FALSE;
			for (DWORD ex = 0 ; ex < eapMethodInfoArray.dwNumberOfMethods ; ++ex)
			{
				if(wcscmp(eapMethodInfoArray.pEapMethods[ex].pwszFriendlyName,szEapAuthMode) ==0)
				{
					EapType    = eapMethodInfoArray.pEapMethods[ex].eaptype.eapType.type;
					dwAuthorID = eapMethodInfoArray.pEapMethods[ex].eaptype.dwAuthorId;
					bEapMatched = TRUE;
					break;
				}            
			}
			if(!bEapMatched)
			{
				PrintMsg(L"%s EAP method is not available in the System",szEapAuthMode);
				PrintMsg(L"Please use -c -? to look for available Eap Methods(Options are case sensitive TLS is not equal to tls)");
				break;
			}
		}

		DWORD dwPrfBufSize = 0;
		HRESULT hr;

		// Calculate Profile String Buffer Size
		dwPrfBufSize = wcslen(szProfileFmt)    + 
			wcslen(wszSsidToConnect)+ 
			wcslen(wszSsidToConnect)+ 
			wcslen(wszNonBroadcast) +
			wcslen(wszConnType)     +
			wcslen(wszConnMode)     +
			wcslen(wszAuthMode)     +
			wcslen(wszCipherMode)   +
			wcslen(wszUseOneX)      +
			wcslen(wszKeyType)      +
			wcslen(wszKey)          +
			wcslen(wszKeyIndex)     +
			sizeof(EapType)         +
			sizeof(dwAuthorID)      +
			1; // For Null entry
		strPrf = (WCHAR*)LocalAlloc(LPTR,dwPrfBufSize * sizeof(WCHAR));

		hr = StringCchPrintf(
			strPrf,
			dwPrfBufSize,
			szProfileFmt,
			wszSsidToConnect,
			wszSsidToConnect,
			wszNonBroadcast,
			wszConnType,
			wszConnMode,
			wszAuthMode,
			wszCipherMode,
			wszUseOneX,
			wszKeyType,
			wszKey,
			wszKeyIndex,
			EapType,
			dwAuthorID);
		if(S_OK != hr)
		{
			PrintMsg(L"Insufficient Buffer while calling StringCchPrintf hr : ld",hr);
			break;
		}

		// Set Profile First
		// If connecting to Infrastructure networks auto connection will kick in
		// If connecting to adhoc we have to set profile first and then do a manual connect
		if(SetProfile(strAdapter,NULL,strPrf))
			break;

		if(pNetwork->dot11BssType == dot11_BSS_type_independent)
		{
			// Use manual connect for adhoc networks
			if(Connect(strAdapter,wszSsidToConnect))
				break;
		}

		bResult=TRUE;
	}
	while(FALSE);

	return bResult;
}
BOOL
WlanInterfaces::
SetAutoconfigParam(LPCWSTR strAdapter,WLAN_AUTOCONF_OPCODE opCode,BOOL bFlag)
{

    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanSetAutoConfigParameter(*phClientHandle,
                                             opCode,
                                             sizeof(BOOL),
                                             &bFlag,
                                             NULL);
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanSetAutoConfigParameter() Failed Error : %d",dwError);
            break;
        }

        bResult = TRUE;
    }while(FALSE);
    
    return bResult;
}
BOOL
WlanInterfaces::
QueryAutoconfigParam(LPCWSTR strAdapter)
{

    BOOL bResult = FALSE;
    PVOID pData = NULL;
    DWORD dwError = ERROR_SUCCESS;
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        EnumStringMapper opTypes(s_OpcodeValueTypes,COUNTOF(s_OpcodeValueTypes),UNKNOWN_ENUM,TEXT("unknown"));
        DWORD dwDataSize = 0;
        WLAN_OPCODE_VALUE_TYPE opType;
        WLAN_AUTOCONF_OPCODE opCode = wlan_autoconf_opcode_show_denied_networks;
        dwError = WlanQueryAutoConfigParameter(*phClientHandle,
                                               opCode,
                                               NULL,
                                               &dwDataSize,
                                               &pData,
                                               &opType);
            
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanQueryAutoConfigParameter() Failed Error : %d",dwError);
            break;
        }
        PrintMsg(L"wlan_autoconf_opcode_show_denied_networks = %d OpcodeType %s",
                 *((BOOL*)pData),
                 opTypes.ToString(opType));

        opCode = wlan_autoconf_opcode_power_setting;
        dwError = WlanQueryAutoConfigParameter(*phClientHandle,
                                               opCode,
                                               NULL,
                                               &dwDataSize,
                                               &pData,
                                               &opType);
            
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanQueryAutoConfigParameter() wlan_autoconf_opcode_power_settingFailed Error : %d",dwError);
            break;
        }
        PrintMsg(L"wlan_autoconf_opcode_power_setting = %d OpcodeType %s",
                 *((BOOL*)pData),
                 opTypes.ToString(opType));

        opCode = wlan_autoconf_opcode_only_use_gp_profiles_for_allowed_networks;
        dwError = WlanQueryAutoConfigParameter(*phClientHandle,
                                               opCode,
                                               NULL,
                                               &dwDataSize,
                                               &pData,
                                               &opType);
            
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanQueryAutoConfigParameter() wlan_autoconf_opcode_only_use_gp_profiles_for_allowed_networks Failed Error : %d",dwError);
            break;
        }
        PrintMsg(L"wlan_autoconf_opcode_only_use_gp_profiles_for_allowed_networks = %d OpcodeType %s",
                 *((BOOL*)pData),
                 opTypes.ToString(opType));


        bResult = TRUE;
    }while(FALSE);
    if(pData)
        WlanFreeMemory(pData);
    return bResult;
}

BOOL
WlanInterfaces::
SetInterface(LPCWSTR strAdapter, WLAN_INTF_OPCODE opCode,DWORD dwSize,PVOID pData)
{

    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanSetInterface(*phClientHandle, 
                                   &pInterface->InterfaceGuid,
                                   opCode,
                                   dwSize,
                                   pData,
                                   NULL);

        if(ERROR_SUCCESS != dwError)
        {
            EnumStringMapper opcodeMapper(s_OpcodeValues,COUNTOF(s_OpcodeValues),UNKNOWN_ENUM,TEXT("UNKNOWN"));
            PrintMsg(L"WlantSetInterface() %s Failed Error : %d",opcodeMapper.ToString(opCode),dwError);
            break;
        }

        bResult = TRUE;
    }while(FALSE);
    
    return bResult;
}
BOOL
WlanInterfaces::
Connect(LPCWSTR strAdapter, LPCWSTR strProfile)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        WLAN_CONNECTION_PARAMETERS wlanConnParam;
        wlanConnParam.dot11BssType = dot11_BSS_type_any;
        wlanConnParam.dwFlags = 0;
        wlanConnParam.pDesiredBssidList = NULL;
        wlanConnParam.pDot11Ssid = NULL;
        wlanConnParam.strProfile = strProfile;
        wlanConnParam.wlanConnectionMode = wlan_connection_mode_profile;
        dwError = WlanConnect(*phClientHandle,
                              &pInterface->InterfaceGuid,
                              &wlanConnParam,
                              NULL);

        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanConnect() Failed Error : %d",dwError);
            break;
        }

        bResult = TRUE;
    }while(FALSE);
    
    return bResult;
}

BOOL
WlanInterfaces::
GetInterface(LPCWSTR pAdapter, PWLAN_INTERFACE_INFO* ppInt)
{
    BOOL bResult = FALSE;

    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface =NULL;
        for(unsigned int i=0;i < pInterfaceList->dwNumberOfItems;i++)
        {
            if(0 == wcscmp(pAdapter,pInterfaceList->InterfaceInfo[i].strInterfaceDescription))
            {
                pInterface = &pInterfaceList->InterfaceInfo[i];
                break;
            }
        }

        if(NULL == pInterface)
        {
            PrintMsg(L"No Interface Matches the given Adapter Name: %s",pAdapter);
            break;
        }

        *ppInt = pInterface;

        bResult = TRUE;    

    }while(FALSE);

    return bResult;        
}

BOOL
WlanInterfaces::
GetFilterList(LPCWSTR strAdapter,WLAN_FILTER_LIST_TYPE fType)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PDOT11_NETWORK_LIST pNetworkList = NULL;
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        DWORD dwError = WlanGetFilterList(*phClientHandle,
                                          fType,
                                          NULL,
                                          &pNetworkList);
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlantGetFilterList() Failed Error : %d",dwError);
            break;
        }
        
        if(NULL == pNetworkList)
        {
            PrintMsg(L"WlanGetFilterList() returned NULL Network List");
            break;
        }

        PrintNetworkList(pNetworkList);
        bResult = TRUE;
    }while(FALSE);

    if(pNetworkList)
        WlanFreeMemory(pNetworkList);
    return bResult;
}

BOOL
WlanInterfaces::
SetProfileList(CONST LPCWSTR strAdapter,LPCWSTR* strList,CONST DWORD dwNumOfProfiles)
{

    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanSetProfileList(*phClientHandle,
                                     &pInterface->InterfaceGuid,
                                     dwNumOfProfiles,
                                     strList,
                                     NULL);
        if(ERROR_SUCCESS != dwError)
        {
            if(ERROR_NOT_FOUND == dwError)
            {
                PrintMsg(L"Profile Names contain the name of a profile that is not present in the profile store");
            }
            PrintMsg(L"WlantSetProfileList() Failed Error : %d",dwError);
            break;
        }

        bResult = TRUE;
    }while(FALSE);
    
    return bResult;
}

BOOL
WlanInterfaces::
ListProfileList(LPCWSTR strAdapter)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PWLAN_PROFILE_INFO_LIST pProfileList = NULL;    
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanGetProfileList(*phClientHandle,
                                     &pInterface->InterfaceGuid,
                                     NULL,
                                     &pProfileList);

        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanGetProfileList() Failed Error : %d",dwError);
            break;
        }

        if(NULL == pProfileList)
        {
            PrintMsg(L"WlanGetProfileList() returned NULL ProfileList");
            break;
        }

        for(DWORD i =0;i<pProfileList->dwNumberOfItems;i++)
        {
            PrintMsg(L"");
            PrintMsg(L"Index           : %lu",i);
            PrintMsg(L"Flags           : %lu",pProfileList->ProfileInfo[i].dwFlags);
            PrintMsg(L"ProfileName     : %s",pProfileList->ProfileInfo[i].strProfileName);
            ListProfile(strAdapter,pProfileList->ProfileInfo[i].strProfileName);
        }
        
        bResult = TRUE;
        }while(FALSE);

    if(pProfileList)
        WlanFreeMemory(pProfileList);    
    
    return bResult;
}

BOOL
WlanInterfaces::
ListProfile(LPCWSTR strAdapter,LPCWSTR strProfileName)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    LPWSTR strXml = NULL;    
    
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        DWORD dwFlags = 0;
        DWORD dwGrantedAccess = 0;
        dwError = WlanGetProfile(*phClientHandle,
                                 &pInterface->InterfaceGuid,
                                 strProfileName,
                                 NULL,
                                 &strXml,
                                 &dwFlags,
                                 &dwGrantedAccess);
        if(ERROR_SUCCESS != dwError)
        {
            if(ERROR_NOT_FOUND == dwError)
            {
                PrintMsg(L"Profile : %s Not Found",strProfileName);
            }
            PrintMsg(L"WlanGetProfile() Failed Error : %d",dwError);
            break;
        }

        if(NULL == strXml)
        {
            PrintMsg(L"WlanGetProfile returned NULL ProfileXmlString");
            break;
        }

        PrintMsg(L"Profile Name      : %s",strProfileName);
        PrintMsg(L"Flags             : %lu",dwFlags);
        PrintMsg(L"Granted Access    : %lu",dwGrantedAccess);
        PrintMsg(L"-------------------");
        PrintMsg(L"\n%s",strXml);
       
        bResult = TRUE;
    }while(FALSE);

    if(strXml)
        WlanFreeMemory(strXml);
    return bResult;
}

BOOL 
WlanInterfaces::    
SetProfilePos(LPCWSTR strAdapter, LPCWSTR strProfileName, DWORD dwOrder)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanSetProfilePosition(*phClientHandle,
                                         &pInterface->InterfaceGuid,
                                         strProfileName,
                                         dwOrder,
                                         NULL);

        if( ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanSetProfilePos Failed Error : %d",dwError);
            break;
        }
        bResult = TRUE;
    }while(FALSE);

    return bResult;
}
BOOL 
WlanInterfaces::    
SetFilterList(LPCWSTR strAdapter,WLAN_FILTER_LIST_TYPE lType, PDOT11_NETWORK_LIST pList)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanSetFilterList(*phClientHandle,
                                    lType,
                                    pList,
                                    NULL);

        if( ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanSetFilterList Failed Error : %d",dwError);
            break;
        }
        bResult = TRUE;
    }while(FALSE);

    return bResult;
}

BOOL   
WlanInterfaces::
RenameProfile(LPCWSTR strAdapter,LPCWSTR strProfileName,LPCWSTR strNewName)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;

    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanRenameProfile(*phClientHandle,
                                    &pInterface->InterfaceGuid,
                                    strProfileName,
                                    strNewName,
                                    NULL);

        if( ERROR_SUCCESS != dwError)
        {
            if(ERROR_NOT_FOUND == dwError)
            {
                PrintMsg(L"Profile : %s Not Found",strProfileName);
            }       
            PrintMsg(L"WlanRenameProfile() Failed Error : %d",dwError);
            break;
        }

        bResult = TRUE;
    }while(FALSE);
    
    return bResult;
}

BOOL
WlanInterfaces::
SaveTempProfile(LPCWSTR strAdapter,LPCWSTR strProfileName)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanDeleteProfile(*phClientHandle,
                                    &pInterface->InterfaceGuid,
                                    strProfileName,
                                    NULL);
        BOOL bOverWrite = TRUE;
        dwError = WlanSaveTemporaryProfile(*phClientHandle,
                                           &pInterface->InterfaceGuid,
                                           strProfileName,
                                           NULL,
                                           0,
                                           bOverWrite,
                                           NULL);

        if( ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanSaveTemporaryProfile() Failed Error : %d",dwError);
            break;
        }
        bResult = TRUE;
    }while(FALSE);

    return bResult;
}

BOOL
WlanInterfaces::
DeleteProfile(LPCWSTR strAdapter,LPCWSTR strProfileName)
{
    BOOL bResult = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        dwError = WlanDeleteProfile(*phClientHandle,
                                    &pInterface->InterfaceGuid,
                                    strProfileName,
                                    NULL);

        if( ERROR_SUCCESS != dwError)
        {
            if(ERROR_NOT_FOUND == dwError)
            {
                PrintMsg(L"Profile : %s Not Found",strProfileName);
            }       
            PrintMsg(L"WlanDeleteProfile Failed Error : %d",dwError);
            break;
        }
        bResult = TRUE;
    }while(FALSE);

    return bResult;
}

BOOL
WlanInterfaces::
SetProfile(CONST LPCWSTR strAdapter,CONST LPCWSTR strFileName,LPCWSTR strPrfXml)
{
    DWORD dwError = NO_ERROR;
    BOOL bResult = FALSE;
    HRESULT hr = E_FAIL;
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;


#ifndef UNDER_CE
        CComPtr<IXMLDOMDocument2> pXmlDoc;
#else
        CComPtr<IXMLDOMDocument> pXmlDoc;
#endif

// 		//initialize COM/OLE:
// 		if(FAILED(CoInitializeEx(NULL,COINIT_MULTITHREADED)))
// 		{
// 			RETAILMSG(1,(_T("ERROR: CoInitialize\n")));
// 			break;;
// 		}


#ifndef UNDER_CE
        hr = CoCreateInstance(
            CLSID_DOMDocument60,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IXMLDOMDocument2,
            (void**)&pXmlDoc);
    #else
        hr = CoCreateInstance(
            CLSID_DOMDocument,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IXMLDOMDocument,
            (void**)&pXmlDoc);
    #endif

        if (hr != S_OK)
        {
            PrintMsg(L"CoCreateInstance Failed  hr : %ld",hr);
            break;
        } 

        VARIANT_BOOL vbSuccess = VARIANT_FALSE;        
        if(!strPrfXml)
        {
            CComVariant vstrFile = strFileName;
            hr = pXmlDoc->load(vstrFile, &vbSuccess);
        }
        else
        {
            BSTR bstrXml = SysAllocString(strPrfXml);
            hr = pXmlDoc->loadXML(bstrXml,&vbSuccess);
            SysFreeString(bstrXml);
        }
        if(hr != S_OK || VARIANT_TRUE != vbSuccess)
        {
            PrintMsg(L"pXmlDoc->load Failed hr : %ld",hr);
            break;
        }

        
        CComBSTR bstrXml = NULL;

        PWSTR strGUID = (PWSTR)strAdapter;
        hr = pXmlDoc->get_xml(&bstrXml);
        if (hr != S_OK)
        {
            PrintMsg(L"pxmlDoc->get_xml Failed hr : %ld",hr);
            break;
        }

		VARIANT path;
// 		path.vt=VT_LPWSTR;
// 		path.bstrVal=L"\\SDMemory\\xml1.xml";
		path=CComVariant(L"\\SDMemory\\xml1.xml");
		if(SUCCEEDED(pXmlDoc->save(path)))
		{
			PrintMsg(L"pXmlDoc->save %s: SUCCEEDED",L"\\SDMemory\\xml1.xml");
		}
		else
		{
			PrintMsg(L"pXmlDoc->save %s: FAILED",L"\\SDMemory\\xml1.xml");
		}


		//ERROR_BAD_PROFILE
		//ERROR_ACCESS_DENIED

        DWORD dwFlags = 0;
        BOOL bOverwrite = TRUE;
        DWORD dwReasonCode = WLAN_REASON_CODE_UNKNOWN;
        dwError = WlanSetProfile(*phClientHandle, 
                                 &pInterface->InterfaceGuid, 
                                 0,
                                 bstrXml,
                                 NULL,
                                 bOverwrite, 
                                 NULL, 
                                 &dwReasonCode);
        if(ERROR_SUCCESS != dwError)
        {
			PrintMsg(L"WlanSetProfile() FAILED Error: %lu , dwReasonCode : %lu",dwError,dwReasonCode);

            WCHAR strMeaning[MAX_PATH];
            DWORD dwEr = ERROR_SUCCESS;
            dwEr = WlanReasonCodeToString(dwReasonCode,
                                          MAX_PATH,
                                          strMeaning,
                                          NULL);
            if(ERROR_SUCCESS != dwEr )
            {
                PrintMsg(L"WlanReasonCodeToString() FAILED error: %lu",dwEr);
                break;
            }
            
            PrintMsg(L"WlanSetProfile() FAILED Error: %lu dwReasonCode : %lu Meaning : %s,",
                     dwError,
                     dwReasonCode,
                     strMeaning);
            break;
        }

        bResult = TRUE;
    }while(FALSE);

    return bResult;
}

BOOL
WlanInterfaces::
ListBss(LPCWSTR strAdapter,PDOT11_SSID pDot11Ssid)
{
    BOOL bResult = FALSE;
    PWLAN_BSS_LIST pWlanBssList = NULL;
    do
    {
        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        DWORD dwError = WlanGetNetworkBssList(*phClientHandle,
                                               &pInterface->InterfaceGuid,
                                               pDot11Ssid,
                                               dot11_BSS_type_any,
                                               TRUE,
                                               NULL,
                                               &pWlanBssList);
                
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanGetNetworkBssList Failed Error : %d",dwError);
            break;
        }

        if(NULL == pWlanBssList)
        {
            PrintMsg(L"WlanGetNetworkBssList returned NULL Bss List");
            break;
        }

        EnumStringMapper bssTypes(s_BssTypes,COUNTOF(s_BssTypes),UNKNOWN_ENUM,TEXT("UNKNOWN"));
        EnumStringMapper phyTypes(s_PhyTypes,COUNTOF(s_PhyTypes),UNKNOWN_ENUM,TEXT("UNKNOWN"));        
        for(DWORD i=0;i<pWlanBssList->dwNumberOfItems;i++)
        {
            WCHAR buff[1024];
            DWORD buffChars = COUNTOF(buff);
            
            PrintMsg(L"%3d  dot11Ssid                : %s",
                     i,
                     FormatSsid(pWlanBssList->wlanBssEntries[i].dot11Ssid,
                                buff,
                                buffChars));
            PrintMsg(L"     uPhyId                   : %lu",pWlanBssList->wlanBssEntries[i].uPhyId);
            PrintMsg(L"     dot11Bssid               : %s",
                     FormatMacAddress(pWlanBssList->wlanBssEntries[i].dot11Bssid,
                                      buff,
                                      buffChars));
            PrintMsg(L"     dot11BssType             : %s",bssTypes.ToString(pWlanBssList->wlanBssEntries[i].dot11BssType));
            PrintMsg(L"     dot11BssPhyType          : %s",phyTypes.ToString(pWlanBssList->wlanBssEntries[i].dot11BssPhyType));
            PrintMsg(L"     lRssi                    : %ld",pWlanBssList->wlanBssEntries[i].lRssi);
            PrintMsg(L"     uLinkQuality             : %lu",pWlanBssList->wlanBssEntries[i].uLinkQuality);            
            PrintMsg(L"     bInRegDomain             : %d",pWlanBssList->wlanBssEntries[i].bInRegDomain ? 1:0);                        
            PrintMsg(L"     usBeaconPeriod           : %hu",pWlanBssList->wlanBssEntries[i].usBeaconPeriod);
            PrintMsg(L"     ullTimeStamp             : %I64u",pWlanBssList->wlanBssEntries[i].ullTimestamp);            
            PrintMsg(L"     ullHostTimeStamp         : %I64u",pWlanBssList->wlanBssEntries[i].ullHostTimestamp);          
            PrintMsg(L"     ullCapabilityInformation : %hu",pWlanBssList->wlanBssEntries[i].usCapabilityInformation);
            PrintMsg(L"     ullChCenterFrequency     : %lu",pWlanBssList->wlanBssEntries[i].ulChCenterFrequency);           
            PrintMsg(L"     wlanRateSet uRateSetLen  : %lu",pWlanBssList->wlanBssEntries[i].wlanRateSet.uRateSetLength);
            for(ULONG j=0;j<pWlanBssList->wlanBssEntries[i].wlanRateSet.uRateSetLength;j++)
                PrintMsg(L"                       mbps     %f",(pWlanBssList->wlanBssEntries[i].wlanRateSet.usRateSet[j] & 0x7FFF)*0.5);
            PrintMsg(L"     ulIeOffset               : %lu",pWlanBssList->wlanBssEntries[i].ulIeOffset);
            PrintMsg(L"     ulIeSize                 : %lu",pWlanBssList->wlanBssEntries[i].ulIeSize);            
            PrintMsg(L"");
        }

        bResult = TRUE;
    }while(FALSE);

    if(pWlanBssList)
        WlanFreeMemory(pWlanBssList);
    return bResult;
}

BOOL
WlanInterfaces::
Disconnect(LPCWSTR strAdapter)
{
    BOOL bResult = FALSE;
    
    do
    {
        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        DWORD dwError = WlanDisconnect(*phClientHandle,
                                       &pInterface->InterfaceGuid,
                                       NULL);

        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanDisconnect Failed Error : %d",dwError);
            break;
        }
    
        bResult = TRUE;
    }while(FALSE);

    return bResult;
}


BOOL
WlanInterfaces::
Scan(LPCWSTR strAdapter,PDOT11_SSID pDot11Ssid)
{
    BOOL bResult = FALSE;
    
    do
    {
        if(FALSE == Init())
            break;

        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(strAdapter,&pInterface))
            break;

        DWORD dwError = WlanScan(*phClientHandle,
                                 &pInterface->InterfaceGuid,
                                 pDot11Ssid,
                                 NULL,
                                 NULL);
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanScan Failed Error : %lu",dwError);
            break;
        }

// 		Sleep(1000);

//         if(!ListNetworks(strAdapter))
//             break;                
    
        bResult = TRUE;
    }while(FALSE);

    return bResult;
}

VOID
WlanInterfaces::
PrintAvailableNetworkList(PWLAN_AVAILABLE_NETWORK_LIST pList)
{
    if(NULL == pList)
        return;
    PrintMsg(L"Available Network List");

    WCHAR buff[1024];
    DWORD buffChars = COUNTOF(buff);
    EnumStringMapper bssTypes(s_BssTypes,COUNTOF(s_BssTypes),UNKNOWN_ENUM,TEXT("UNKNOWN"));
    EnumStringMapper phyTypes(s_PhyTypes,COUNTOF(s_PhyTypes),UNKNOWN_ENUM,TEXT("UNKNOWN"));
    EnumStringMapper authTypes(s_AuthAlgorithm,COUNTOF(s_AuthAlgorithm),UNKNOWN_ENUM,TEXT("UNKNOWN"));
    EnumStringMapper cipherTypes(s_CipherAlgorithm,COUNTOF(s_CipherAlgorithm),UNKNOWN_ENUM,TEXT("UNKNOWN"));

    for(DWORD i=0;i<pList->dwNumberOfItems;i++)
    {
        FormatSsid(pList->Network[i].dot11Ssid,buff,buffChars);
        PrintMsg(L"%3d  dot11Ssid                : %s",i,buff);
        PrintMsg(L"     profileName              : %s",pList->Network[i].strProfileName);
        PrintMsg(L"     dot11BssType             : %s",bssTypes.ToString(pList->Network[i].dot11BssType));            
        PrintMsg(L"     unNumOfBssids            : %lu",pList->Network[i].uNumberOfBssids);            
        PrintMsg(L"     bNetworkConnectable      : %d",pList->Network[i].bNetworkConnectable);                        
        if(!pList->Network[i].bNetworkConnectable)
        {
            WlanReasonCodeToString(
                pList->Network[i].wlanNotConnectableReason,
                buffChars,
                buff,
                NULL);
            PrintMsg(L"     wlanNotConnectableReason : %d",pList->Network[i].wlanNotConnectableReason);
            PrintMsg(L"     NotConnectableReason     : %s",buff);
        }
        PrintMsg(L"     uNumOfPhyTypes           : %lu",pList->Network[i].uNumberOfPhyTypes);
        for(DWORD j =0 ;j < pList->Network[i].uNumberOfPhyTypes;j++)
            PrintMsg(L"                                %s",
                 phyTypes.ToString(pList->Network[i].dot11PhyTypes[j]));
        PrintMsg(L"     bMorePhyTypes            : %d",pList->Network[i].bMorePhyTypes);
        PrintMsg(L"     WlanSignalQuality        : %lu",pList->Network[i].wlanSignalQuality);
        PrintMsg(L"     bSecurityEnabled         : %d",pList->Network[i].bSecurityEnabled);            
        PrintMsg(L"     dot11DefaultAuthAlgo     : %s",authTypes.ToString(pList->Network[i].dot11DefaultAuthAlgorithm));            
        PrintMsg(L"     dot11DefaultCipherAlgo   : %s",cipherTypes.ToString(pList->Network[i].dot11DefaultCipherAlgorithm));            
        PrintMsg(L"     dwFlags                  : %lu",pList->Network[i].dwFlags);            
        PrintMsg(L"     dwReserved               : %lu",pList->Network[i].dwReserved);            
        PrintMsg(L"");
    }
    
}
VOID
WlanInterfaces::
PrintNetworkList(PDOT11_NETWORK_LIST pList)
{
    if(NULL == pList)
        return;
    PrintMsg(L"Network List");

    WCHAR buff[1024];
    DWORD buffChars = COUNTOF(buff);
    EnumStringMapper bssTypes(s_BssTypes,COUNTOF(s_BssTypes),UNKNOWN_ENUM,TEXT("UNKNOWN"));

    for(DWORD i=0;i<pList->dwNumberOfItems;i++)
    {
        FormatSsid(pList->Network[i].dot11Ssid,buff,buffChars);
        PrintMsg(L"%3d  dot11Ssid                : %s",i,buff);
        PrintMsg(L"     dot11BssType                 : %s",bssTypes.ToString(pList->Network[i].dot11BssType));            
        PrintMsg(L"");        
    }
    
}

BOOL
WlanInterfaces::
ListNetworks(LPCWSTR pAdapter,BOOL bPrint)
{
    BOOL bResult = FALSE;
//     PWLAN_AVAILABLE_NETWORK_LIST pList = NULL;
	if(pNetworksList)
		WlanFreeMemory(pNetworksList);
	pNetworksList = NULL;

    do
    {
        if(FALSE == Init())
            break;
        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(pAdapter,&pInterface))
            break;

        DWORD dwError = WlanGetAvailableNetworkList(*phClientHandle,
                                                    &pInterface->InterfaceGuid,
                                                    WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES |
                                                    WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_MANUAL_HIDDEN_PROFILES,
                                                    NULL,
                                                    &pNetworksList);
        if(ERROR_SUCCESS != dwError)
        {
        /*
            if(ERROR_NDIS_DOT11_POWER_STATE_INVALID == dwError)
                PrintMsg(L"The radio for the interface is turned off");
                */
            if(ERROR_NOT_ENOUGH_MEMORY == dwError)
                PrintMsg(L"Failed to allocate memory for the query results");
            PrintMsg(L"WLanGetAvailableNetworkList() Failed Error : %d",dwError);
            break;
        }
        if(NULL == pNetworksList)
        {
            PrintMsg(L"WlanGetAvailableNetworkList returned NULL Network List");
            break;
        }
        
		if(bPrint)
		{
		  PrintAvailableNetworkList(pNetworksList);
		}

        bResult = TRUE;
    }while(FALSE);
//     if(pList)
//         WlanFreeMemory(pList);
//     pList = NULL;
    return bResult;
}

BOOL
WlanInterfaces::
EnumInfo(LPCWSTR pAdapter)
{
    BOOL bResult = FALSE;
    WCHAR buff[1024];
    DWORD buffChars = COUNTOF(buff);
    DWORD dwError = ERROR_SUCCESS;
    DWORD dwDataSize = 0;
    VOID* pData = NULL;
    WLAN_OPCODE_VALUE_TYPE opcodeType = wlan_opcode_value_type_invalid;
    PWLAN_INTERFACE_CAPABILITY pIntCap = NULL;
    EnumStringMapper phytypemapper(s_PhyTypes,COUNTOF(s_PhyTypes),UNKNOWN_ENUM,TEXT("unknown"));
        
    do
    {
        if(FALSE == Init())
            break;
        WLAN_INTERFACE_INFO* pInterface = NULL;
        if(!GetInterface(pAdapter,&pInterface))
            break;
        
        PrintMsg(L"WlanGetInterfaceCapability for %s",pAdapter);
        dwError = WlanGetInterfaceCapability(*phClientHandle,
                                             &pInterface->InterfaceGuid,
                                             NULL,
                                             &pIntCap);
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanGetInterfaceCapability Failed Error : %ul",dwError);   
            break;
        }

        if(NULL == pIntCap)
        {
            PrintMsg(L"WlanGetInterfaceCapability returned NULL WLAN_INTERFACE_CAPABILITY");            
        }
        EnumStringMapper intTypeMapper(s_InterfaceTypes,
                                       COUNTOF(s_InterfaceTypes),
                                       wlan_interface_type_invalid,
                                       TEXT("wlan_interface_type_invalid"));
        PrintMsg(L"\tinterfaceType             : %s",intTypeMapper.ToString(pIntCap->interfaceType));
        PrintMsg(L"\tbDot11DSupported          : %d",pIntCap->bDot11DSupported);
        PrintMsg(L"\tdwMaxDesiredSsidListSize  : %lu",pIntCap->dwMaxDesiredSsidListSize);        
        PrintMsg(L"\tdwMaxDesiredBssidListSize : %lu",pIntCap->dwMaxDesiredBssidListSize);        
        PrintMsg(L"\tdwNumberOfSupportedPhys   : %lu",pIntCap->dwNumberOfSupportedPhys);
        for(DWORD i =0;i<pIntCap->dwNumberOfSupportedPhys && i<WLAN_MAX_PHY_INDEX;i++)
        {
            PrintMsg(L"\t                            %s",phytypemapper.ToString(pIntCap->dot11PhyTypes[i]));        
        }

        PrintMsg(L"");        
        PrintMsg(L"WlanQueryInterface() for %s", pAdapter);
        PrintMsg(L"\t%-40s %-6s %s",TEXT("Opcode"),TEXT("Value"),TEXT("OpcodeType"));        
        QueryBoolFromInterface(*phClientHandle, 
                               &pInterface->InterfaceGuid, 
                               wlan_intf_opcode_autoconf_enabled, 
                               &dwDataSize, 
                               &pData, 
                               &opcodeType);

        QueryBoolFromInterface(*phClientHandle, 
                               &pInterface->InterfaceGuid, 
                               wlan_intf_opcode_background_scan_enabled, 
                               &dwDataSize, 
                               &pData, 
                               &opcodeType);
        QueryBoolFromInterface(*phClientHandle, 
                               &pInterface->InterfaceGuid, 
                               wlan_intf_opcode_media_streaming_mode, 
                               &dwDataSize, 
                               &pData, 
                               &opcodeType);
        QueryBoolFromInterface(*phClientHandle, 
                               &pInterface->InterfaceGuid, 
                               wlan_intf_opcode_supported_safe_mode, 
                               &dwDataSize, 
                               &pData, 
                               &opcodeType);
        QueryBoolFromInterface(*phClientHandle, 
                               &pInterface->InterfaceGuid, 
                               wlan_intf_opcode_certified_safe_mode, 
                               &dwDataSize, 
                               &pData, 
                               &opcodeType);
        
        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_radio_state, 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"WLAN_RADIO_STATE\n");
        if(ERROR_SUCCESS == dwError)
        {
            PWLAN_RADIO_STATE pRadioState = (PWLAN_RADIO_STATE)pData;
            PrintMsg(L"\t%-8s %-50s %-50s",L"PhyIndex",L"Software",L"Hardware");
            EnumStringMapper radstatemapper(s_RadioStates,COUNTOF(s_RadioStates),UNKNOWN_ENUM,TEXT("unknown"));
            for(unsigned int i=0;i < pRadioState->dwNumberOfPhys;i++)
            {
                PrintMsg(L"\t%8d %-50s %-50s",
                         pRadioState->PhyRadioState[i].dwPhyIndex,
                         radstatemapper.ToString(pRadioState->PhyRadioState[i].dot11SoftwareRadioState),
                         radstatemapper.ToString(pRadioState->PhyRadioState[i].dot11HardwareRadioState));                
            }

        }
        else
        {
            PrintMsg(L"WLAN_RADIO_STATE FAILED");
        }

        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_bss_type, 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"\tBSS TYPE   : \n");
        EnumStringMapper bsstypemapper(s_BssTypes,COUNTOF(s_BssTypes),UNKNOWN_ENUM,TEXT("unknown"));
        if(ERROR_SUCCESS == dwError)
        {
            PDOT11_BSS_TYPE pBssType = (PDOT11_BSS_TYPE)pData;


            PrintMsg(L"\t%s %s",TEXT("wlan_intf_opcode_bss_type"),bsstypemapper.ToString(*pBssType));
        }
        else
        {
            PrintMsg(L"\twlan_intf_opcode_bss_type  FAILED");
        }


        dwError = WlanQueryInterface(*phClientHandle, 
                                 &pInterface->InterfaceGuid, 
                                 wlan_intf_opcode_interface_state , 
                                 NULL, 
                                 &dwDataSize, 
                                 &pData, 
                                 &opcodeType);
        PrintMsg(L"Interface State\n");
        EnumStringMapper intfstatemapper(s_InterfaceStates,COUNTOF(s_InterfaceStates),UNKNOWN_ENUM,TEXT("unknown"));        
        if(ERROR_SUCCESS == dwError)
        {
            PWLAN_INTERFACE_STATE pIntfState = (PWLAN_INTERFACE_STATE)pData;

            PrintMsg(L"%s %s",TEXT("wlan_intf_opcode_interface_state"),intfstatemapper.ToString(*pIntfState));
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_interface_state  FAILED");
        }


        dwError = WlanQueryInterface(*phClientHandle, 
                                 &pInterface->InterfaceGuid, 
                                 wlan_intf_opcode_current_connection  , 
                                 NULL, 
                                 &dwDataSize, 
                                 &pData, 
                                 &opcodeType);
        PrintMsg(L"wlan_intf_opcode_current_connection \n");
        EnumStringMapper connmodemapper(s_ConnectionModes,COUNTOF(s_ConnectionModes),UNKNOWN_ENUM,TEXT("unknown"));
        EnumStringMapper authmapper(s_AuthAlgorithm,COUNTOF(s_AuthAlgorithm),UNKNOWN_ENUM,TEXT("unknown"));
        EnumStringMapper ciphermapper(s_CipherAlgorithm,COUNTOF(s_CipherAlgorithm),UNKNOWN_ENUM,TEXT("unknown"));
        if(ERROR_SUCCESS == dwError)
        {
            PWLAN_CONNECTION_ATTRIBUTES pConAttr = (PWLAN_CONNECTION_ATTRIBUTES)pData;
        
            PrintMsg(L"\tInterface State     : %s",intfstatemapper.ToString(pConAttr->isState));
            PrintMsg(L"\tConnection Mode     : %s",connmodemapper.ToString(pConAttr->wlanConnectionMode));
            PrintMsg(L"\tProfile Name        : %s",pConAttr->strProfileName);
            PrintMsg(L"\n");
            PrintMsg(L"Association Attributes");
            PrintMsg(L"\tSSID                : %s",FormatSsid(pConAttr->wlanAssociationAttributes.dot11Ssid,buff,buffChars));
            PrintMsg(L"\tBSS Type            : %s",bsstypemapper.ToString(pConAttr->wlanAssociationAttributes.dot11BssType));
            PrintMsg(L"\tMAC Address         : %s ",FormatMacAddress(pConAttr->wlanAssociationAttributes.dot11Bssid,buff,buffChars));
            PrintMsg(L"\tPhy Types           : %s",phytypemapper.ToString(pConAttr->wlanAssociationAttributes.dot11PhyType));
            PrintMsg(L"\tPhy Index           : %lu",pConAttr->wlanAssociationAttributes.uDot11PhyIndex);
            PrintMsg(L"\tWLAN Signal Quality : %lu", pConAttr->wlanAssociationAttributes.wlanSignalQuality);
            PrintMsg(L"\tRxRate              : %lu",pConAttr->wlanAssociationAttributes.ulRxRate);
            PrintMsg(L"\tTxRate              : %lu",pConAttr->wlanAssociationAttributes.ulTxRate);
            PrintMsg(L"\n");
            PrintMsg(L"Security Attributes");
            PrintMsg(L"\tSecurity Enabled    : %d", pConAttr->wlanSecurityAttributes.bSecurityEnabled ? 1:0);
            PrintMsg(L"\tOneX Enabled        : %d", pConAttr->wlanSecurityAttributes.bOneXEnabled ? 1:0);         
            PrintMsg(L"\tAuth Algorithm      : %s",authmapper.ToString(pConAttr->wlanSecurityAttributes.dot11AuthAlgorithm));
            PrintMsg(L"\tCipher Algorithm    : %s",ciphermapper.ToString(pConAttr->wlanSecurityAttributes.dot11CipherAlgorithm));            
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_current_connection   FAILED");
        }


        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_channel_number, 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"CHANNEL NUMBER\n");
        if(ERROR_SUCCESS == dwError)
        {
            PULONG puChannelNum = (PULONG)pData;


            PrintMsg(L"%s %lu",TEXT("wlan_intf_opcode_channel_number "),*puChannelNum);
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_channel_number   FAILED");
        }

        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_supported_infrastructure_auth_cipher_pairs , 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"Infrastructure supported Auth Cipher Pair List\n");
        if(ERROR_SUCCESS == dwError)
        {
            PWLAN_AUTH_CIPHER_PAIR_LIST pAuthCipherList = (PWLAN_AUTH_CIPHER_PAIR_LIST)pData;
            for(unsigned int i = 0;i < pAuthCipherList->dwNumberOfItems ; i++)
                PrintMsg(L"%-50s %-50s",
                         authmapper.ToString(pAuthCipherList->pAuthCipherPairList[i].AuthAlgoId),
                         ciphermapper.ToString(pAuthCipherList->pAuthCipherPairList[i].CipherAlgoId));
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_supported_infrastructure_auth_cipher_pairs    FAILED");
        }

        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_supported_adhoc_auth_cipher_pairs, 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"Adhoc supported Auth Cipher Pair List\n");
        if(ERROR_SUCCESS == dwError)
        {
            PWLAN_AUTH_CIPHER_PAIR_LIST pAuthCipherList = (PWLAN_AUTH_CIPHER_PAIR_LIST)pData;
            for(unsigned int i = 0;i < pAuthCipherList->dwNumberOfItems ; i++)
                PrintMsg(L"%-50s %-50s",
                         authmapper.ToString(pAuthCipherList->pAuthCipherPairList[i].AuthAlgoId),
                         ciphermapper.ToString(pAuthCipherList->pAuthCipherPairList[i].CipherAlgoId));
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_supported_adhoc_auth_cipher_pairs    FAILED");
        }

        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_supported_country_or_region_string_list, 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"Supported Country Or Region String List\n");
        if(ERROR_SUCCESS == dwError)
        {
            PWLAN_COUNTRY_OR_REGION_STRING_LIST pCountryList = (PWLAN_COUNTRY_OR_REGION_STRING_LIST)pData;
            for(unsigned int i = 0;i < pCountryList->dwNumberOfItems ; i++)
                PrintMsg(L"%s ",pCountryList->pCountryOrRegionStringList[i]);
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_supported_country_or_region_string_list     FAILED");
        }

        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_rssi, 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"RSSI\n");
        if(ERROR_SUCCESS == dwError)
        {
            PLONG plRssi = (PLONG)pData;
            PrintMsg(L"RSSI %ld ",*plRssi);
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_rssi     FAILED");
        }

        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_current_operation_mode, 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"Operation Mode\n");
        EnumStringMapper opmodemapper(s_OperationModes,COUNTOF(s_OperationModes),UNKNOWN_ENUM,TEXT("UNKNOWN"));
        if(ERROR_SUCCESS == dwError)
        {
            PULONG pulOpMode = (PULONG)pData;
            PrintMsg(L"Operation Mode %s ",opmodemapper.ToString((int)*pulOpMode));
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_current_operation_mode     FAILED");
        }


        dwError = WlanQueryInterface(*phClientHandle, 
                                     &pInterface->InterfaceGuid, 
                                     wlan_intf_opcode_statistics , 
                                     NULL, 
                                     &dwDataSize, 
                                     &pData, 
                                     &opcodeType);
        PrintMsg(L"Statistics\n");
        if(ERROR_SUCCESS == dwError)
        {
            PWLAN_STATISTICS pStatistics = (PWLAN_STATISTICS)pData;
            PrintMsg(L"4WayHandShakeFailures        : %I64u",pStatistics->ullFourWayHandshakeFailures);
            PrintMsg(L"TKIP CounterMeasures Invoked : %I64u",pStatistics->ullTKIPCounterMeasuresInvoked);
            PrintMsg(L"Reserved                     : %I64u",pStatistics->ullReserved);         
            PrintMsg(L"Number of Phys               : %lu",pStatistics->dwNumberOfPhys);   
            PrintMsg(L"MacUcastCounters");
            PrintMacFrameStatistics(&pStatistics->MacUcastCounters);
            PrintMsg(L"MacMcastCounters");
            PrintMacFrameStatistics(&pStatistics->MacMcastCounters);           
        }
        else
        {
            PrintMsg(L"wlan_intf_opcode_statistics     FAILED");
        }


        bResult = TRUE;
    }while(FALSE);

    if(pIntCap)
        WlanFreeMemory(pIntCap);
    pIntCap = NULL;

    if(pData)
        WlanFreeMemory(pData);
    pData=NULL;
    return bResult;
}

WlanInterfaces::
WlanInterfaces(WLAN_NOTIFICATION_CALLBACK lpfnCallback,LPVOID lpfnContext)
{
    pdwNegotiatedVersion = NULL;
    phClientHandle = NULL;
    pInterfaceList = NULL;
	pNetworksList = NULL;
	m_lpfnCallback=lpfnCallback;
	m_lpfnContext=lpfnContext;
	m_bIni=FALSE;

	Init();
}
BOOL 
WlanInterfaces::
GetNetworksList( LPCWSTR pAdapter, PWLAN_AVAILABLE_NETWORK_LIST* ppNetworksList)
{
	BOOL bResult = FALSE;
	do
	{
		if(ListNetworks(pAdapter))
		{
			*ppNetworksList = pNetworksList;
			bResult = TRUE;
		}
	}while(FALSE);

	return bResult;
}
BOOL
WlanInterfaces::
GetInterfaceList(PWLAN_INTERFACE_INFO_LIST* ppInterfaceList)
{
    BOOL bResult = FALSE;
    do
    {
        if(FALSE == Init())
            break;
        
        *ppInterfaceList = pInterfaceList;
        
        bResult = TRUE;
    }while(FALSE);
    
    return bResult;
}

BOOL
WlanInterfaces::
EnumWirelessNetworkCards()
{
    BOOL bResult = FALSE;
    unsigned short * strGuid = NULL;    
    RPC_STATUS rpcStatus = RPC_S_OK;

    do
    {
        if(FALSE == Init())
            break;
        
        if(0 == pInterfaceList->dwNumberOfItems)
        {
            PrintMsg(L"System has no Wireless Cards.");
            bResult = TRUE;
            break;
        }

        EnumStringMapper mapper(s_InterfaceStates,COUNTOF(s_InterfaceStates),UNKNOWN_ENUM,TEXT("unknown"));        
        for (unsigned int i=0; i < pInterfaceList->dwNumberOfItems; i++)         
        {

            rpcStatus = UuidToStringW(&pInterfaceList->InterfaceInfo[i].InterfaceGuid,&strGuid);
            if(RPC_S_OK != rpcStatus)
            {
                PrintMsg(L"UuidToStringW() Error: %u",rpcStatus);
                break;
            }
                           
            PrintMsg(L"wifi-card [%2d]",i);
            PrintMsg(L"\t Description: %s",
                     pInterfaceList->InterfaceInfo[i].strInterfaceDescription);
            PrintMsg(L"\t GUID       : %s",(LPWSTR)strGuid);
            PrintMsg(L"\t State      : %s",
                     mapper.ToString(pInterfaceList->InterfaceInfo[i].isState));
            PrintMsg(L"\n");
            
        }
        bResult = TRUE;
    }while(FALSE);
    
    return bResult;
}

BOOL
WlanInterfaces::
Init()
{

	BOOL bResult = FALSE;
	DWORD dwError = ERROR_SUCCESS;

	if(m_bIni)
	{
// 		RETAILMSG(1,(_T("***********WlanInterfaces already ini.******************\n")));

		//clear:
		if(pInterfaceList)
			WlanFreeMemory(pInterfaceList);
		pInterfaceList = NULL;

		if(pNetworksList)
			WlanFreeMemory(pNetworksList);
		pNetworksList = NULL;

		dwError = WlanEnumInterfaces(*phClientHandle, NULL, &pInterfaceList);
		if(ERROR_SUCCESS != dwError)
		{
			PrintMsg(L"WlanEnumInterfaces() error %u  ...",dwError);
		}

		if(NULL == pInterfaceList)
		{
			PrintMsg(L"WlanEnumInterfaces returned a NULL InterfaceList.");
		}
		else
		{
			bResult = TRUE;
		}

		m_bIni = bResult;

		return bResult;
	}

    //Clean up Before Reinitializing
    Cleanup();

    do
    {
        pdwNegotiatedVersion = (PDWORD)WlanAllocateMemory(sizeof(DWORD));
        if(NULL == pdwNegotiatedVersion)
        {
            PrintMsg(L"WlanAllocateMemory() error %u",GetLastError());
            break;
        }
        *pdwNegotiatedVersion = 0;
        
        phClientHandle = (PHANDLE)WlanAllocateMemory(sizeof(HANDLE));
        if(NULL == phClientHandle)
        {
            PrintMsg(L"WlanAllocateMemory() error %u",GetLastError());
            break;
        }       
        *phClientHandle = 0;

        dwError = WlanOpenHandle(dwClientVersion,NULL, pdwNegotiatedVersion, phClientHandle);        
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanOpenHandle() error %u",dwError);
            break;
        }

		if(0 == *phClientHandle)
		{
			PrintMsg(L"WlanOpenHandle returned NULL hClientHandle");
			break;
		}

		//registry notification
		if(m_lpfnCallback)
		{
			DWORD dwPrevNotif = 0;
			dwError = WlanRegisterNotification(*phClientHandle,WLAN_NOTIFICATION_SOURCE_ALL,TRUE,m_lpfnCallback,m_lpfnContext,NULL,&dwPrevNotif);
			if(ERROR_SUCCESS != dwError)
			{
				PrintMsg(L"WlanRegisterNotification() error %u",dwError);
				break;
			}  
		}

        if(dwClientVersion != *pdwNegotiatedVersion)
        {
            PrintMsg(L"ExpectedClientVersion=%d and NegotiatedClientVersion=%d Does not Match...",
                     dwClientVersion,
                     *pdwNegotiatedVersion);
            break;
        }

        dwError = WlanEnumInterfaces(*phClientHandle, NULL, &pInterfaceList);
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(L"WlanEnumInterfaces() error %u  ...",dwError);
            break;
        }

        if(NULL == pInterfaceList)
        {
            PrintMsg(L"WlanEnumInterfaces returned a NULL InterfaceList.");
            break;
        }

		bResult=TRUE;

    }while(FALSE);

	m_bIni=bResult;

    return bResult;
}

void
WlanInterfaces::
Cleanup()
{
    if(pdwNegotiatedVersion)
        WlanFreeMemory(pdwNegotiatedVersion);
    pdwNegotiatedVersion = NULL;
    if(phClientHandle)
    {
        if(*phClientHandle)
        {
            DWORD dwError = WlanCloseHandle(*phClientHandle,NULL);
            if(ERROR_SUCCESS != dwError)
                PrintMsg(L"WlanCloseHandle() error %u",dwError);
            *phClientHandle = NULL;
        }

        WlanFreeMemory(phClientHandle);
        phClientHandle = NULL;
    }

    if(pInterfaceList)
        WlanFreeMemory(pInterfaceList);
    pInterfaceList = NULL;
    
	if(pNetworksList)
		WlanFreeMemory(pNetworksList);
	pNetworksList = NULL;

}

WlanInterfaces::
~WlanInterfaces()
{
    Cleanup();
}

// ========================================================================== //
// Run-time Configuration:
// ========================================================================== //

// -----------------------------------------------------------------------------
int
WasOption
// look for argument like '-t' or '\t'.
// returns option index
// returns index of argv[] found, -1 if not found
(
    IN int     argc,    // number ars
    IN LPCWSTR argv[],  // arg array
    IN LPCWSTR szOption // to find ('t')
)
{
    for (int i=0; i<argc; i++)
    {
        if (((*argv[i] == L'-') || (*argv[i] == L'/'))
         && !wcscmp(argv[i]+1, szOption))
            return i;
    }
    return -1;
} // WasOption()

// -----------------------------------------------------------------------------
int
GetOption
// look for argument like '-t 100' or '\t 100'.
// returns index of '100' if option ('t') is found
// returns -1 if not found
(
    IN  int      argc,        // number ars
    IN  LPCWSTR  argv[],      // arg array
    IN  LPCWSTR  pszOption,   // to find ('n')
    OUT LPCWSTR *ppszArgument // option value ('100')
)
{
    if (!ppszArgument)
        return -1;

    int i = WasOption(argc, argv, pszOption);
    if ((i < 0) || ((i+1) >= argc))
    {
        *ppszArgument = NULL;
        return -1;
    }

    *ppszArgument = argv[i+1];
    return i+1;
} // GetOption()

// ========================================================================== //
// WZC Utilities:
// ========================================================================== //

// First wireless card found by WZC query:
WCHAR g_WirelessCardName[MAX_PATH] = L"";

// -----------------------------------------------------------------------------
BOOL
EnumWirelessNetworkCard
// enumerate wireless network cards detected by WlanSvc
(
   // arg none
)
{
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {
        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->EnumWirelessNetworkCards())
            break;

        bResult = TRUE;
    }
    while(FALSE);
    if(pWlanInterfaces)
        delete pWlanInterfaces;
    return bResult;
} // EnumWirelessNetworkCard()


// -----------------------------------------------------------------------------
BOOL
GetFirstWirelessNetworkCard
// find the first wireless network cards
// found card name goes to g_WirelessCardName
(
   // arg none
)
{
    
    g_WirelessCardName[0] = L'\0';

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    PWLAN_INTERFACE_INFO_LIST pInterfaceList = NULL;
    
    do
    {
        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->GetInterfaceList(&pInterfaceList))
            break;
            
        if(0 == pInterfaceList->dwNumberOfItems)
        {
            PrintMsg(L"system has no wireless cards.");
            break;
        }

        wcsncpy_s(g_WirelessCardName, MAX_PATH, pInterfaceList->InterfaceInfo[0].strInterfaceDescription, MAX_PATH-1);
        PrintMsg(L"wireless card found: %s", g_WirelessCardName);

        bResult = TRUE;
    }
    while(FALSE);
    if(pWlanInterfaces)
        delete pWlanInterfaces;
    return bResult;
    
} // GetFirstWirelessNetworkCard()




// -----------------------------------------------------------------------------
BOOL
PrintWlanIntf
// Queries the WiFi card and prints out the info available for the interface.
// wlantool -q cisco1
//     query cisco1 adapter parameters.
// wlantool -q
//     find wireless card, and query for that adapter.
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {

        LPCWSTR pAdapter = NULL;
        if (GetOption(argc, argv, L"i", &pAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            pAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->EnumInfo(pAdapter))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}

// -----------------------------------------------------------------------------
BOOL
InterpretEncryptionKeyValue
// key is interpreted differently based on the Encryption
(
    IN OUT WCHAR*           wszEncryptionKey,
    IN     LPCWSTR          wszAuth,
    IN     LPCWSTR          wszEncr,
    OUT    LPCWSTR          *wszKey,
    OUT    LPCWSTR          *wszKeyIndex,
    IN     BOOL             &bNeed8021X // this becomes TRUE if wszEncryptionKey is "auto"
)
{
    if (0 == wcscmp(wszEncr,L"WEP"))
    {
        // WEP key : valid forms are
        // -key 1/1234567890 [index=1,40-bit(10-digit hexa)]
        // -key 0/zxcvb [index=0, 40-bit(5-char)
        // -key 3/12345678901234567890123456 [index=3, 104-bit(26-digit hexa)
        // -key 2/abcdefghij123 [index=2, 104-bit(13-char)
        // -key auto    [key comes from successful EAP]
        if (wcsicmp(wszEncryptionKey, L"auto") == 0)
            bNeed8021X = TRUE;
        else
        {
            if ((wszEncryptionKey[0] <  L'0')
             || (wszEncryptionKey[0] >  L'3')
             || (wszEncryptionKey[1] != L'/'))
            {
                PrintMsg(L"invalid key index");
                return FALSE;
            }
            wszEncryptionKey[1] = '\0';
            *wszKeyIndex = wszEncryptionKey;

            wszEncryptionKey += 2;
            UINT keyLength = wcslen(wszEncryptionKey);

            BOOL bAsciiKey;
            switch (keyLength)
            {
                case 5: case 13:
                    bAsciiKey = TRUE;
                    break;
                case 10: case 26:
                    bAsciiKey = FALSE;
                    break;
                default:
                    PrintMsg(L"invalid key length (s/b 5 or 13 (ascii), 10 or 26 (hex)");
                    return FALSE;
            }

            *wszKey = wszEncryptionKey;
            if (!bAsciiKey)
            {
                //Validate Hex Format of the Key
                for (UINT cx = 0, abyte = 0 ; cx < keyLength ; ++cx)
                {
                    UINT ch = static_cast<UINT>(wszEncryptionKey[cx]);
                    switch (ch)
                    {
                        case L'0': case L'1': case L'2': case L'3': case L'4':
                        case L'5': case L'6': case L'7': case L'8': case L'9':
                            break;
                        case L'a': case L'b': case L'c':
                        case L'd': case L'e': case L'f':
                            break;
                        case L'A': case L'B': case L'C':
                        case L'D': case L'E': case L'F':
                            break;
                        default:
                            PrintMsg(L"invalid HEX WEP key");
                            return FALSE;
                    }
                }
            }
        }
    }
    else
    if (wcscmp(wszEncr, L"TKIP") == 0 ||
        wcscmp(wszEncr, L"AES")  == 0)
    {
        // TKIP key
        // -key 12345678   [8-char]
        // -key HelloWorld [10-char]
        // -key abcdefghij1234567890abcdefghij1234567890abcdefghij1234567890abc [63-char]
        // -key auto    [key comes from successful EAP]
        if (wcsicmp(wszEncryptionKey, L"auto") == 0)
            bNeed8021X = TRUE;
        else
        {
            ULONG keyLength = wcslen(wszEncryptionKey);
            if ((keyLength<8) || (keyLength>63))
            {
                PrintMsg(L"WPA-PSK/TKIP key should be 8-63 char long string");
                return FALSE;
            }

            // WPA/TKIP pre-shared key takes 256 bit key.
            // Everything else is incorrect format.
            // Translates a user password (8 to 63 ascii chars) into a 256 bit network key.
            // We do this for WPA-PSK and WPA-None.
            *wszKey = wszEncryptionKey;

        }

    }

    return TRUE;
} // InterpretEncryptionKeyValue()


// ========================================================================== //
// WZC Connections:
// ========================================================================== //

// -----------------------------------------------------------------------------
BOOL
ListProfileList
// Set a Profile on a particular Interface 
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    if (WasOption(argc, argv, L"?")>0 )
    {
        PrintMsg(L"-lp options:");
        PrintMsg(L"   Usage: -lp -i <Interface Description>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"");
        return FALSE;
    }
  
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {

        LPCWSTR pAdapter = NULL;
        if (GetOption(argc, argv, L"i", &pAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            pAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->ListProfileList(pAdapter))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}//ListProfileList()
// -----------------------------------------------------------------------------
BOOL
SetProfilePosition
// Sets the Position of the Profile in the Preference List
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    LPCWSTR strProfileName = NULL;
    LPCWSTR strProfilePos = NULL;
 
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {
        if (WasOption(argc, argv, L"?")>0 || 
            GetOption(argc, argv, L"p", &strProfileName) < 0)
        {
            PrintMsg(L"spp options:");
            PrintMsg(L"   Sets the Profile -p <ProfileName> to the position specified in -n <Position>");
            PrintMsg(L"   Usage: -dp -i <Interface Description> -p <Profile Name>");
            PrintMsg(L"   -i  <Interface Description> Interface Description");
            PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
            PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
            PrintMsg(L"       Eg: -i ISLP21");     
            PrintMsg(L"   -p  <Profile Name> Name of the Profile in the profile store that needs to be deleted");
            PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\" ");        
            PrintMsg(L"       Eg: -p CaseSenPrfName ");                
            PrintMsg(L"   -n  <Order Number>");
            PrintMsg(L"       Order of the Profile in the Preference List");
            PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\" ");        
            PrintMsg(L"       Default value is 0 or the first position");                    
            PrintMsg(L"       Eg: -n 3 ");                            
            PrintMsg(L"");
            break;
        }
        
        LPCWSTR strAdapter = NULL;
        if(GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        
        // Default Position is 0 or First Position
        DWORD dwPosition = 0;
        GetOption(argc, argv, L"n", &strProfilePos);
        if(strProfilePos)
        {
            dwPosition = wcstoul(strProfileName,NULL,10);
        }

        if(!pWlanInterfaces->SetProfilePos(strAdapter,strProfileName,dwPosition))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}

// -----------------------------------------------------------------------------
BOOL
RenameProfile
// Deletes a specific Profile on a particular Interface 
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    LPCWSTR strProfileName = NULL;
    LPCWSTR strNewName = NULL;
 
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {
        if (WasOption(argc, argv, L"?")>0 || 
            GetOption(argc, argv, L"p", &strProfileName) < 0 ||
            GetOption(argc, argv, L"r", &strNewName) < 0)
        {
            PrintMsg(L"dp options:");
            PrintMsg(L"   Renames the Profile specified in -p <ProfileName>to the specified profile in -r <NewName>");
            PrintMsg(L"   Usage: -rp -i <Interface Description> -p <Existing Profile Name> -r <New Name of Profile>");
            PrintMsg(L"   -i  <Interface Description> Interface Description");
            PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
            PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
            PrintMsg(L"       Eg: -i ISLP21");     
            PrintMsg(L"   -p  <Profile Name> Name of the Profile in the profile store that needs to be deleted");
            PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\" ");        
            PrintMsg(L"       Eg: -p CaseSenPrfName ");                
            PrintMsg(L"   -r  <New Profile Name> New name for the profile specified in -p <argument>");
            PrintMsg(L"       Eg: -r CaseSenNewPrfName ");                            
            PrintMsg(L"");
            break;
        }
        
        LPCWSTR strAdapter = NULL;
        if(GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->RenameProfile(strAdapter,strProfileName,strNewName))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}
// -----------------------------------------------------------------------------
BOOL
SaveTempProfile
// Saves a Temporary Profile in the Profile Store
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    LPCWSTR strProfileName = NULL;
 
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {
        if (WasOption(argc, argv, L"?")>0 || GetOption(argc, argv, L"p", &strProfileName) < 0)
        {
            PrintMsg(L"dp options:");
            PrintMsg(L"   Usage: -dp -i <Interface Description> -p <Profile Name>");
            PrintMsg(L"   -i  <Interface Description> Interface Description");
            PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
            PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
            PrintMsg(L"       Eg: -i ISLP21");     
            PrintMsg(L"   -p  <Profile Name> Name of the Profile in the profile store that needs to be deleted");
            PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\" ");        
            PrintMsg(L"       Eg: -p CaseSenPrfName ");                
            PrintMsg(L"");
            break;
        }
        
        LPCWSTR strAdapter = NULL;
        if(GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->SaveTempProfile(strAdapter,strProfileName))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}
// -----------------------------------------------------------------------------
BOOL
DeleteProfile
// Deletes a specific Profile on a particular Interface 
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    LPCWSTR strProfileName = NULL;
 
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {
        if (WasOption(argc, argv, L"?")>0 || GetOption(argc, argv, L"p", &strProfileName) < 0)
        {
            PrintMsg(L"dp options:");
            PrintMsg(L"   Deletes the specified Profile");
            PrintMsg(L"   Usage: -dp -i <Interface Description> -p <Profile Name>");
            PrintMsg(L"   -i  <Interface Description> Interface Description");
            PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
            PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
            PrintMsg(L"       Eg: -i ISLP21");     
            PrintMsg(L"   -p  <Profile Name> Name of the Profile in the profile store that needs to be deleted");
            PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\" ");        
            PrintMsg(L"       Eg: -p CaseSenPrfName ");                
            PrintMsg(L"");
            break;
        }
        
        LPCWSTR strAdapter = NULL;
        if(GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->DeleteProfile(strAdapter,strProfileName))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}

// -----------------------------------------------------------------------------
BOOL
ListProfile
// Set a Profile on a particular Interface 
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    LPCWSTR strProfileName = NULL;
 
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {
        if (WasOption(argc, argv, L"?")>0 || GetOption(argc, argv, L"p", &strProfileName) < 0)
        {
            PrintMsg(L"-lp options:");
            PrintMsg(L"   Lists the specified profile");
            PrintMsg(L"   Usage: -lp -i <Interface Description> -p <Profile Name>");
            PrintMsg(L"   -i  <Interface Description> Interface Description");
            PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
            PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
            PrintMsg(L"       Eg: -i ISLP21");     
            PrintMsg(L"   -p  <Profile Name> Name of the Profile in the profile store");
            PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\" ");        
            PrintMsg(L"       Eg: -p CaseSenPrfName ");                
            PrintMsg(L"");
            break;
        }
        
        LPCWSTR strAdapter = NULL;
        if(GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->ListProfile(strAdapter,strProfileName))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}
// -----------------------------------------------------------------------------
VOID
GetSsidFromString
// Gets the Ssid from the command Line arguments using the arg Value
(
    IN  LPCWSTR      strSsid,   // Ssid String
    IN  PDOT11_SSID  pDot11Ssid // Pointer to Ssid object
)
{
    BYTE pbSsid[DOT11_SSID_MAX_LENGTH+1] = {0};
    pDot11Ssid->uSSIDLength = WideCharToMultiByte(
        CP_ACP,
        0,
        strSsid,
        -1,
        (LPSTR)pbSsid,
        sizeof(pbSsid),
        NULL,
        NULL);
    pDot11Ssid->uSSIDLength--;
    memcpy(pDot11Ssid->ucSSID,pbSsid,pDot11Ssid->uSSIDLength);

}

// -----------------------------------------------------------------------------
VOID
GetSsidFromString
// Gets the Ssid from the command Line arguments using the arg Value
(
    IN  LPCWSTR      strSsid,   // Ssid String
    IN  PDOT11_SSID* ppDot11Ssid // Pointer to Ssid object
)
{
    *ppDot11Ssid = (PDOT11_SSID)WlanAllocateMemory(sizeof(DOT11_SSID));
    ZeroMemory(*ppDot11Ssid,sizeof(DOT11_SSID));
    GetSsidFromString(strSsid,*ppDot11Ssid);
}
// -----------------------------------------------------------------------------
VOID
GetSsid
// Gets the Ssid from the command Line arguments using the arg Value
(
    IN  int          argc,       // number ars
    IN  LPCWSTR      argv[],     // arg array
    IN  WCHAR*       arg,        // arg value
    OUT PDOT11_SSID* ppDot11Ssid // DOT11_SSID
)
{
    LPCWSTR strSsid = NULL;        
    if (GetOption(argc, argv, arg, &strSsid) >= 0)
    {
        GetSsidFromString(strSsid,ppDot11Ssid);
    }

}
// -----------------------------------------------------------------------------
BOOL
ListBss
// Scans for Available Networks
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[] // arg array
)
{

    if (WasOption(argc, argv, L"?")>0)
    {
        PrintMsg(L"-lb options:");
        PrintMsg(L"   Usage: -ln -i <Interface Description> -id <SSID>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -id  <SSID> SSID of the network");
        PrintMsg(L"        Obtain the above value by running \"wlantool -ln\" ");        
        PrintMsg(L"        Eg: -id YOUR_netSsid ");        
        PrintMsg(L"");
        return FALSE;
    }

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;    
    PDOT11_SSID pDot11Ssid = NULL;    

    do
    {
        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        GetSsid(argc,argv,L"id",&pDot11Ssid);
        if(!pWlanInterfaces->ListBss(strAdapter,pDot11Ssid))
            break;
    
        bResult = TRUE;
    }while(FALSE);

    if(pDot11Ssid)
        WlanFreeMemory(pDot11Ssid);
    if(pWlanInterfaces)
        delete pWlanInterfaces;
    return bResult;
}
// -----------------------------------------------------------------------------
BOOL
Scan
// Scans for Available Networks
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    if (WasOption(argc, argv, L"?")>0)
    {
        PrintMsg(L"-sc options:");
        PrintMsg(L"   Usage: -ln -i <Interface Description> -id <SSID>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -id  <SSID> SSID of the network");
        PrintMsg(L"        Obtain the above value by running \"wlantool -ln\" ");        
        PrintMsg(L"        Eg: -id YOUR_netSsid ");        
        PrintMsg(L"");
        return FALSE;
    }

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;    
    PDOT11_SSID pDot11Ssid = NULL;    
    do
    {
        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        GetSsid(argc,argv,L"id",&pDot11Ssid);
        if(!pWlanInterfaces->Scan(strAdapter,pDot11Ssid))
            break;
        
    
        bResult = TRUE;
    }while(FALSE);

    if(pDot11Ssid)
        WlanFreeMemory(pDot11Ssid);
    if(pWlanInterfaces)
        delete pWlanInterfaces;
    
    return bResult;
}
// -----------------------------------------------------------------------------
WLAN_FILTER_LIST_TYPE GetFilterListType(int iValue)
{
    WLAN_FILTER_LIST_TYPE flType;
    switch(iValue)
    {
        case 0:
            flType = wlan_filter_list_type_gp_permit;
            break;
        case 1:
            flType = wlan_filter_list_type_gp_deny;                
            break;
        case 2:
            flType = wlan_filter_list_type_user_permit;                
            break;
        case 3:
            flType = wlan_filter_list_type_user_deny;                
            break;
    };

    return flType;
}
// -----------------------------------------------------------------------------
BOOL
AutoConfigParam
// Gets the filter List for the specified interface
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    if (WasOption(argc, argv, L"?")>0)
    {
        PrintMsg(L" -ac -i <Interface Description> -p <Parameter> -v <Enable/Disable>");
        PrintMsg(L"        Gets / Sets the AutoConfig Parameter");
        PrintMsg(L"        Default action is to query the AutoConfig Parameters");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -p  <AutoConfigParameter> SetAutoConfig Parameter");
        PrintMsg(L"        0 for wlan_autoconf_opcode_show_denied_networks");        
        PrintMsg(L"        1 for wlan_autoconf_opcode_power_setting");        
        PrintMsg(L"        2 for wlan_autoconf_opcode_only_use_gp_profiles_for_allowed_networks");        
        PrintMsg(L"        Eg: -p 1 ");        
        PrintMsg(L"   -v  <Enable/Disable> ");
        PrintMsg(L"        0 for Disable");        
        PrintMsg(L"        1 for Enable");        
        PrintMsg(L"        Eg: -v 1 ");        
        PrintMsg(L"");
        return FALSE;
    }

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;    
    do
    {
        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        LPCWSTR strArg=NULL;
        if(GetOption(argc,argv,L"p",&strArg) >= 0)
        {            
            int iValue = _wtoi(strArg);
            if(!(iValue > -1 && iValue < 3))
            {
                PrintMsg(L"-p value must be in the range of 0 through 2");
                break;
            }
            WLAN_AUTOCONF_OPCODE opCode; 
            switch(iValue)
            {
                case 0:
                    opCode = wlan_autoconf_opcode_show_denied_networks;
                    break;
                case 1:
                    opCode = wlan_autoconf_opcode_power_setting;
                    break;
                case 2:
                    opCode = wlan_autoconf_opcode_only_use_gp_profiles_for_allowed_networks;
                    break;
            };

            if(GetOption(argc,argv,L"v",&strArg) < 0)
            {
                PrintMsg(L"-v is missing");
                break;
            }

            int iflag = _wtoi(strArg);
            if(iflag !=0 && iflag !=1)
            {
                PrintMsg(L"-v can be only 0 or 1");
                break;
            }
            BOOL bFlag = iflag == 0?FALSE:TRUE;

            if(!pWlanInterfaces->SetAutoconfigParam(strAdapter,opCode,bFlag))
                break;

        }
        else
        {            
            if(!pWlanInterfaces->QueryAutoconfigParam(strAdapter))
                break;
        }
    
        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;
    
    return bResult;
}

// -----------------------------------------------------------------------------
BOOL
GetFilterList
// Gets the filter List for the specified interface
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    if (WasOption(argc, argv, L"?")>0)
    {
        PrintMsg(L"-gf options:");
        PrintMsg(L"   Usage: -gf -i <Interface Description> -ty <Filter Type>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -ty  <Filter Type> Specified the type of filter list");
        PrintMsg(L"        0 for wlan_filter_list_type_gp_permit");        
        PrintMsg(L"        1 for wlan_filter_list_type_gp_deny");        
        PrintMsg(L"        2 for wlan_filter_list_type_user_permit");        
        PrintMsg(L"        3 for wlan_filter_list_type_user_deny");                
        PrintMsg(L"        Eg: -ty 1 ");        
        PrintMsg(L"");
        return FALSE;
    }

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;    
    do
    {
        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        LPCWSTR strArg=NULL;
        if(GetOption(argc,argv,L"ty",&strArg) < 0)
        {            

            PrintMsg(L"Please Specify Filter Type");
        }

        int iValue = _wtoi(strArg);
        if(!(iValue > -1 && iValue < 4))
        {
            PrintMsg(L"-ty value mut be between 0 throuh 3");
            break;
        }

        WLAN_FILTER_LIST_TYPE lType = GetFilterListType(iValue);
        
        if(!pWlanInterfaces->GetFilterList(strAdapter,lType))
            break;
        
    
        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;
    
    return bResult;
}
// -----------------------------------------------------------------------------
BOOL
SetFilterList
// Gets the filter List for the specified interface
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    if (WasOption(argc, argv, L"?")>0)
    {
        PrintMsg(L"-sf options:");
        PrintMsg(L"   Usage: -gf -i <Interface Description> -id <SSID1> <SSID2> -ty <Filter Type>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -id  <SSID> Ssid of the network to filter");
        PrintMsg(L"        Obtain the ssid value by running \"wlantool -lnl \"");
        PrintMsg(L"        Eg: -id Ssid1 Ssid2");
        PrintMsg(L"   -ty  <Filter Type> Specified the type of filter list");
        PrintMsg(L"        0 for wlan_filter_list_type_gp_permit");        
        PrintMsg(L"        1 for wlan_filter_list_type_gp_deny");        
        PrintMsg(L"        2 for wlan_filter_list_type_user_permit");        
        PrintMsg(L"        3 for wlan_filter_list_type_user_deny");                
        PrintMsg(L"        Eg: -ty 1 ");        
        PrintMsg(L"");
        return FALSE;
    }

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;    
    PDOT11_NETWORK_LIST pList = NULL;
    do
    {
        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        LPCWSTR strArg=NULL;
        if(GetOption(argc,argv,L"ty",&strArg) < 0)
        {            
            PrintMsg(L"Please Specify Filter Type");
            break;
        }

        int iValue = _wtoi(strArg);
        if(!(iValue > -1 && iValue < 4))
        {
            PrintMsg(L"-ty value mut be between 0 throuh 3");
            break;
        }

        WLAN_FILTER_LIST_TYPE lType = GetFilterListType(iValue);


        if(WasOption(argc,argv,L"id") < 0)
        {
            PrintMsg(L"-id Option is not present");
            break;
        }

        DWORD dwNumOfSsids = (argc-1) - WasOption(argc,argv,L"id") ;
        if(dwNumOfSsids <= 0)
        {
            PrintMsg(L"-id option does not have enough Ssids");
            break;
        }

        size_t size = FIELD_OFFSET(DOT11_NETWORK_LIST,Network) + sizeof(DOT11_NETWORK) * dwNumOfSsids;
        pList = (PDOT11_NETWORK_LIST) WlanAllocateMemory(size);
        if(NULL == pList)
        {
            PrintMsg(L"Memory Allocation (WlanAllocateMemory) Failed Error : %u",GetLastError());
            break;
        }
        pList->dwNumberOfItems = dwNumOfSsids;
        for(DWORD i = 0;i<dwNumOfSsids;i++)
        {
            pList->Network[i].dot11BssType = dot11_BSS_type_any;
            GetSsidFromString(argv[i+WasOption(argc,argv,L"id")],&pList->Network[i].dot11Ssid);
        }
        if(!pWlanInterfaces->SetFilterList(strAdapter,lType,pList))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pList)
        WlanFreeMemory(pList);
    if(pWlanInterfaces)
        delete pWlanInterfaces;
    
    return bResult;
}

// -----------------------------------------------------------------------------
BOOL
ListNetworks
// Lists Available Networks in the profile
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    if (WasOption(argc, argv, L"?")>0)
    {
        PrintMsg(L"-ln options:");
        PrintMsg(L"   Usage: -ln -i <Interface Description>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"");
        return FALSE;
    }

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;    
    
    do
    {
        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        if(!pWlanInterfaces->ListNetworks(strAdapter))
            break;
    
        bResult = TRUE;     
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;
    
    return bResult;
}
// -----------------------------------------------------------------------------
BOOL
SetProfile
// Set a Profile on a particular Interface 
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    LPCWSTR fName = NULL;
    if (WasOption(argc, argv, L"?")>0 || GetOption(argc, argv, L"f", &fName) < 0)
    {
        PrintMsg(L"-sp options:");
        PrintMsg(L"   Usage: -sp -i <Interface Description> -f <File Name>");
        PrintMsg(L"   If running on the device the file name should include release\a.prf ");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -f  <File Name> FileName of the Xml which describes the profile");
        PrintMsg(L"       Eg: -f Profile.xml ");        
        PrintMsg(L"");
        return FALSE;
    }
  
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {

        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->SetProfile(strAdapter,fName))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}
// -----------------------------------------------------------------------------
BOOL
Disconnect
// Disconnects the interface from the Network
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    LPCWSTR fName = NULL;
    if (WasOption(argc, argv, L"?")>0)
    {
        PrintMsg(L"-d options:");
        PrintMsg(L"   Usage: -d -i <Interface Description>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"");
        return FALSE;
    }
  
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {

        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
        if(!pWlanInterfaces->Disconnect(strAdapter))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}
// -----------------------------------------------------------------------------
BOOL
Connect
// configure WZC as given arguments.
// as a result of this configuration, WZC will atempt to connect given SSID.
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{
    EAP_METHOD_INFO_ARRAY eapMethodInfoArray = {0};

    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    WCHAR* strPrf = NULL;

    do
    {
        // Get Eap Methods
        EAP_ERROR* pEapErr = NULL;
        DWORD dwError = EapHostPeerGetMethods(&eapMethodInfoArray,&pEapErr);
        if(ERROR_SUCCESS != dwError)
        {
            PrintMsg(
                L"EapHostPeerGetMethods FAILED Err: %ld Reason: %s Fix: %s",
                dwError,
                pEapErr->pRootCauseString,
                pEapErr->pRepairString);
            EapHostPeerFreeErrorMemory(pEapErr);
            bResult = FALSE;
            break;
        }     

        if (WasOption(argc, argv, L"?")>0)
        {
            PrintMsg(L" -c -i <Interface Description> -ssid <SSID> -auth <Auth> -encr <Encr> -key <index>/<Key> -eap <EapType>");
            PrintMsg(L"       Adds the specified Network to the Profile Store and sets");
            PrintMsg(L"       the profile to the top of the preferred List");
            PrintMsg(L" -c options:");
            PrintMsg(L"   -i  <Interface Description> Interface Description");
            PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
            PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
            PrintMsg(L"       Eg: -i ISLP21");     
            PrintMsg(L"-ssid   SSID to connect. SSID is the name of wireless network");
            PrintMsg(L"");

            PrintMsg(L"-auth   authentication mode:");
            for (int ax = 0 ; ax < COUNTOF(s_AuthAlgorithm) ; ++ax)
                if(0 != wcslen(s_AuthAlgorithm[ax].alias))
                    PrintMsg(
                        L"            %-8s = %s", 
                        s_AuthAlgorithm[ax].alias,
                        s_AuthAlgorithm[ax].name);
            PrintMsg(L"        '-authentication' is same as -auth");
            PrintMsg(L"        default is open-authentication");
            PrintMsg(L"");

            PrintMsg(L"-encr   encription mode:");
            for (int cx = 0 ; cx < COUNTOF(s_CipherAlgorithm) ; ++cx)
                if(0 != wcslen(s_CipherAlgorithm[cx].alias))
                    PrintMsg(
                        L"            %-12s = %s", 
                        s_CipherAlgorithm[cx].alias,
                        s_CipherAlgorithm[cx].name);
            PrintMsg(L"        '-encryption' is same as -encr");
            PrintMsg(L"        WEP and TKIP uld have '-key' option");
            PrintMsg(L"        default is encryption-disabled");
            PrintMsg(L"");

            PrintMsg(L"-key    key value.");
            PrintMsg(L"        for WEP-key, use '#/<key-value>' form.");
            PrintMsg(L"        '#' is key-index (0-3), '<key-value>' is WEP key value (40-bit or 104-bit).");
            PrintMsg(L"        40-bit is either '10-digit hexa numbers' (ex: 1234567890) or '5-char ASCII string' (ex: zxcvb)");
            PrintMsg(L"        104-bit is either '26-digit hexa numbers' (ex: 12345678901234567890123)");
            PrintMsg(L"                       or '13-char ASCII string' (ex: abcdefghijklm)");
            PrintMsg(L"        for TKIP-key, use '<key-value>' form. (no key index)");
            PrintMsg(L"        TKIP-key can be 8-63 char ASCII string (ex: asdfghjk)");
            PrintMsg(L"");

            PrintMsg(L"-eap    EAP mode:");
            if(0 == eapMethodInfoArray.dwNumberOfMethods)
            {
                PrintMsg(L"        No Eap Methods Available\n");
            }
            else
            {
                for (DWORD ex = 0 ; ex < eapMethodInfoArray.dwNumberOfMethods ; ++ex)
                    PrintMsg(
                    L"            %s", 
                    eapMethodInfoArray.pEapMethods[ex].pwszFriendlyName);
            }
            PrintMsg(L"        this is for 802.1X (EAP). both AP and STA will get keys automatically after the successful EAP.");
            PrintMsg(L"        so, '-eap' should have '-key auto' option.");
            PrintMsg(L"        UI dialogs will popup and ask user credentials (like certificate or user-name/password).");
            PrintMsg(L"");

            PrintMsg(L"-adhoc  connects to an adhoc net.");
            PrintMsg(L"        if this is not given, by default connecting to an AP (infrastructure net).");
            PrintMsg(L"");

            PrintMsg(L"-hidden connects to an AP that is not broadcasting its beacon.");

            PrintMsg(L"-example  shows example usage.");

            PrintMsg(L"note");
            PrintMsg(L"1. options are case insensitive. (ie. '-auth'!='-AUTH', 'open'=='OPEN')");
            PrintMsg(L"2. options values are case sensitive (ie. '-auth open' != '-auth OPEN')");
            PrintMsg(L"3. but the SSID and key-values are case sensitive. (ie. 'TEST1'!='test1', 'abcdefgh'!='ABCDEFGH'");
            PrintMsg(L"4. giving WEP key in a command line could be a security concern.");
            PrintMsg(L"   somebody could watch keys over your shoulder.");
            PrintMsg(L"");

            bResult = TRUE;
            break;
        }
        else
        if ((WasOption(argc, argv, L"ex")>0) || (WasOption(argc, argv, L"example")>0))
        {
            LPCWSTR szOptionExamples[] = {
                L"examples:",
                L"wlantool -c -ssid CEOPEN -auth open -encr disabled",
                L"wlantool -c -ssid CEOPEN    (same as above)",
                L"wlantool -c -ssid CESHARED -auth open -encr WEP -key 1/1234567890",
                L"wlantool -c -ssid CESHARED2 -auth open -encr WEP -key 4/zxcvb",
                L"wlantool -c -ssid CESHARED3 -auth shared -encr WEP -key 1/1234567890",
                L"wlantool -c -ssid CE8021X -auth open -encr wep -key auto -eap tls",
                L"wlantool -c -ssid WPAPSK -auth wpa-psk -encr tkip -key qwertyuiop",
                L"wlantool -c -ssid WPA -auth wpa -encr tkip -key auto -eap peap",
                L"wlantool -c -ssid CEAD1 -adhoc",
                L"wlantool -c -ssid CEADWEP104 -adhoc -auth open -encr WEP -key 1/abcdefghijabc",
                NULL };
            PrintMsgList(szOptionExamples);

            bResult = TRUE;
            break;
        }

        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        // SSID
        LPCWSTR wszSsidToConnect = NULL;
        if (GetOption(argc, argv, L"ssid", &wszSsidToConnect) < 0 || L'\0' == wszSsidToConnect[0])
        {
            PrintMsg(L"no SSID is given");
            return FALSE;
        }

        // adhoc? or infrastructure net?
        LPCWSTR wszConnMode = NULL;
        LPCWSTR wszConnType = NULL;
        if(WasOption(argc, argv, L"adhoc") > 0)
        {
            wszConnType = L"IBSS";
            wszConnMode = L"manual";
        }
        else
        {
            wszConnType = L"ESS";
            wszConnMode = L"auto";
        }

        // Default to connecting to only APs that are beaconing.
        LPCWSTR wszNonBroadcast = 
            (WasOption(argc, argv, L"hidden") > 0) ? L"true" : L"false";

        // Authentication
        LPCWSTR wszAuthMode = NULL;
        if ((GetOption(argc, argv, L"auth",           &wszAuthMode) > 0)
         || (GetOption(argc, argv, L"authentication", &wszAuthMode) > 0))
        {
            EnumStringMapper authmapper(s_AuthAlgorithm,COUNTOF(s_AuthAlgorithm),UNKNOWN_ENUM,TEXT("unknown"));
            int authmode = authmapper.ToEnum(wszAuthMode);
            if(authmode == UNKNOWN_ENUM)
            {
                PrintMsg(L"unknown auth mode");
                break;
            }
        }
        else
        {
            PrintMsg(L"missing auth mode (-auth)");
            break;
        }

        // Encryption
        LPCWSTR wszKeyType = NULL;
        LPCWSTR wszCipherMode = NULL;
        if ((GetOption(argc, argv, L"encr",       &wszCipherMode) > 0)
         || (GetOption(argc, argv, L"encryption", &wszCipherMode) > 0))
        {
            EnumStringMapper ciphermapper(s_CipherAlgorithm,COUNTOF(s_CipherAlgorithm),UNKNOWN_ENUM,TEXT("unknown"));
            ULONG cipherMode = ciphermapper.ToEnum(wszCipherMode);
            if (UNKNOWN_ENUM == cipherMode)
            {
                PrintMsg(L"unknown encryption mode");
                break;
            }
            if(wcscmp(wszCipherMode,L"WEP") == 0)
                wszKeyType = L"networkKey";
            else
                wszKeyType = L"passPhrase";
        }
        else
        {
            PrintMsg(L"missing encryption mode (-encr)");
            break;
        }

        // Key
        LPCWSTR wszEncryptionKey = NULL;
        WCHAR wszKeyString[500];
        ZeroMemory(wszKeyString,sizeof(WCHAR)*COUNTOF(wszKeyString));
        LPCWSTR wszKey = NULL;
        LPCWSTR wszKeyIndex = NULL;
        if (GetOption(argc, argv, L"key", &wszEncryptionKey) < 0)
            wszEncryptionKey = L"auto";
        wcscpy_s(wszKeyString,500,wszEncryptionKey);
        BOOL bNeed8021X = FALSE;
        if (!InterpretEncryptionKeyValue(
                 wszKeyString, 
                 wszAuthMode,
                 wszCipherMode,
                 &wszKey,
                 &wszKeyIndex, 
                 bNeed8021X))
            break;

        // Fill with dummy values so that Profile Formatting doesnt choke
        if(!wszKey)
            wszKey = L"0";
        if(!wszKeyIndex)
            wszKeyIndex = L"0";

        // One X
        LPCWSTR wszUseOneX = NULL;

        if (bNeed8021X)
        {
            wszKey = L"0";
            wszUseOneX = L"true";
        }
        else
            wszUseOneX = L"false";            

        // OneX
        LPCWSTR szEapAuthMode = NULL;
        BYTE EapType = 0;
        DWORD dwAuthorID = 0;

        if (GetOption(argc, argv, L"eap", &szEapAuthMode) > 0)
        {
            if(!bNeed8021X)
            {
                PrintMsg(
                    L"-eap option is not valid for -auth %s -encr %s connnection Configuration",
                    wszAuthMode,
                    wszCipherMode);
                break;
            }
            if(0 == eapMethodInfoArray.dwNumberOfMethods)
            {
                PrintMsg(L"There are no EAP methods Available in the System");
                PrintMsg(L" -eap option is invalid");
                break;
            }
            BOOL bEapMatched = FALSE;
            for (DWORD ex = 0 ; ex < eapMethodInfoArray.dwNumberOfMethods ; ++ex)
            {
                if(wcscmp(eapMethodInfoArray.pEapMethods[ex].pwszFriendlyName,szEapAuthMode) ==0)
                {
                    EapType    = eapMethodInfoArray.pEapMethods[ex].eaptype.eapType.type;
                    dwAuthorID = eapMethodInfoArray.pEapMethods[ex].eaptype.dwAuthorId;
                    bEapMatched = TRUE;
                    break;
                }            
            }
            if(!bEapMatched)
            {
                PrintMsg(L"%s EAP method is not available in the System",szEapAuthMode);
                PrintMsg(L"Please use -c -? to look for available Eap Methods(Options are case sensitive TLS is not equal to tls)");
                break;
            }
        }

        DWORD dwPrfBufSize = 0;
        HRESULT hr;

        // Calculate Profile String Buffer Size
        dwPrfBufSize = wcslen(szProfileFmt)    + 
                       wcslen(wszSsidToConnect)+ 
                       wcslen(wszSsidToConnect)+ 
                       wcslen(wszNonBroadcast) +
                       wcslen(wszConnType)     +
                       wcslen(wszConnMode)     +
                       wcslen(wszAuthMode)     +
                       wcslen(wszCipherMode)   +
                       wcslen(wszUseOneX)      +
                       wcslen(wszKeyType)      +
                       wcslen(wszKey)          +
                       wcslen(wszKeyIndex)     +
                       sizeof(EapType)         +
                       sizeof(dwAuthorID)      +
                       1; // For Null entry
        strPrf = (WCHAR*)LocalAlloc(LPTR,dwPrfBufSize * sizeof(WCHAR));

        hr = StringCchPrintf(
            strPrf,
            dwPrfBufSize,
            szProfileFmt,
            wszSsidToConnect,
            wszSsidToConnect,
            wszNonBroadcast,
            wszConnType,
            wszConnMode,
            wszAuthMode,
            wszCipherMode,
            wszUseOneX,
            wszKeyType,
            wszKey,
            wszKeyIndex,
            EapType,
            dwAuthorID);
        if(S_OK != hr)
        {
            PrintMsg(L"Insufficient Buffer while calling StringCchPrintf hr : ld",hr);
            break;
        }

        // Set Profile First
        // If connecting to Infrastructure networks auto connection will kick in
        // If connecting to adhoc we have to set profile first and then do a manual connect
        if(!pWlanInterfaces->SetProfile(strAdapter,NULL,strPrf))
            break;

        if(WasOption(argc, argv, L"adhoc") > 0)
        {
            // Use manual connect for adhoc networks
            if(!pWlanInterfaces->Connect(strAdapter,wszSsidToConnect))
                break;
        }

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    if(strPrf)
        LocalFree(strPrf);

    if(eapMethodInfoArray.pEapMethods)
        EapHostPeerFreeMemory((BYTE*)eapMethodInfoArray.pEapMethods);
    eapMethodInfoArray.pEapMethods = NULL;

    return bResult;

} // Connect()

// -----------------------------------------------------------------------------
BOOL
ConnectProfile
// Set a Profile on a particular Interface 
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    // First profile Name used the check to see if atleast one profile is present
    LPCWSTR strProfileName = NULL;
    if (WasOption(argc, argv, L"?")>0 || GetOption(argc, argv, L"p", &strProfileName) < 0)
    {
        PrintMsg(L"-c options:");
        PrintMsg(L"   Usage: -c -i <Interface Description> -p <ProfileName1>");
        PrintMsg(L"   Usage: -c -i <Interface Description> -id <SSID> -auth <Auth> -encr <Encr> /key <Key>");        
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");             
        PrintMsg(L"   -p  <ProfileName> Name of the profile used to connect");
        PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\"");
        PrintMsg(L"       Eg: -p CaseSenPrfName");        
        PrintMsg(L"");
        return FALSE;
    }
  
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {

        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        LPCWSTR strProfile = NULL;
        if(GetOption(argc,argv,L"p",&strProfile) < 0)
        {
            PrintMsg(L"-p Option is not present");
            break;
        }

        if(!pWlanInterfaces->Connect(strAdapter,strProfile))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}

// -----------------------------------------------------------------------------
BOOL
SetProfileList
// Set a Profile on a particular Interface 
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    // First profile Name used the check to see if atleast one profile is present
    LPCWSTR strProfileName = NULL;
    if (WasOption(argc, argv, L"?")>0 || GetOption(argc, argv, L"p", &strProfileName) < 0)
    {
        PrintMsg(L"-spl options:");
        PrintMsg(L"   Usage: -spl -i <Interface Description> -p <ProfileName1> <ProfileName2>");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -p  <ProfileName1> List of ProfileNames separated by spaces");
        PrintMsg(L"       Obtain the above value by running \"wlantool -lpl\"");
        PrintMsg(L"       Eg: -p CaseSenPrfName1 CaseSenPrfName2 ");        
        PrintMsg(L"");
        return FALSE;
    }
  
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {

        LPCWSTR pAdapter = NULL;
        if (GetOption(argc, argv, L"i", &pAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            pAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }

        if(WasOption(argc,argv,L"p") < 0)
        {
            PrintMsg(L"-p Option is not present");
            break;
        }

        DWORD dwNumOfProfiles = (argc-1) - WasOption(argc,argv,L"p") ;
        if(dwNumOfProfiles <= 0)
        {
            PrintMsg(L"-p option does not have enough profiles");
            break;
        }
        if(!pWlanInterfaces->SetProfileList(pAdapter,&argv[WasOption(argc,argv,L"p")+1],dwNumOfProfiles))
            break;

        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}
// -----------------------------------------------------------------------------
BOOL SetInterface(int argc,LPCWSTR argv[],WlanInterfaces* pWlanInterfaces,LPCWSTR strAdapter,WCHAR* arg,WLAN_INTF_OPCODE opCode)
{
    BOOL bResult = FALSE;
    LPCWSTR strArg = NULL;
    int eval = UNKNOWN_ENUM;    
    int iEnum = UNKNOWN_ENUM;     
    if(GetOption(argc,argv,arg,&strArg) >= 0)
    {            
        int iValue = _wtoi(strArg);
        switch(opCode)
        {
            case wlan_intf_opcode_autoconf_enabled:
            case wlan_intf_opcode_background_scan_enabled:
            case wlan_intf_opcode_media_streaming_mode:
                if(0 == iValue || 1 == iValue)
                {
                    PBOOL pbFlag = (PBOOL)WlanAllocateMemory(sizeof(BOOL));
                    *pbFlag = iValue ? TRUE:FALSE;
                    bResult = pWlanInterfaces->SetInterface(strAdapter,opCode,sizeof(BOOL),pbFlag);
                    WlanFreeMemory(pbFlag);
                }
                else
                {
                    PrintMsg(L"-%s Argument is not Valid. Should be 0 or 1",arg);
                    return FALSE;
                }
                break;

            case wlan_intf_opcode_current_operation_mode:
                if(!(0 == iValue || 1 == iValue))
                {
                    PrintMsg(L"-%s Argument is not Valid. Should be 0 or 1",arg);
                    return FALSE;
                }  
                eval = iValue ? DOT11_OPERATION_MODE_NETWORK_MONITOR : DOT11_OPERATION_MODE_EXTENSIBLE_STATION;
                bResult = pWlanInterfaces->SetInterface(strAdapter,opCode,sizeof(eval),&eval);                
                break;

            case wlan_intf_opcode_bss_type:
                if(!(iValue  > 0 && iValue < 4))
                {
                    PrintMsg(L"-%s Argument is not Valid. Should be 1 or 2 or 3",arg);
                    return FALSE;
                }
                if( 1 == iValue)
                    iEnum = dot11_BSS_type_infrastructure;
                else
                if( 2 == iValue)
                    iEnum = dot11_BSS_type_independent;
                else
                if( 3 == iValue)
                    iEnum = dot11_BSS_type_any;
                    
                bResult = pWlanInterfaces->SetInterface(strAdapter,opCode,sizeof(int),&iEnum);                
                break;
             
            case wlan_intf_opcode_radio_state:
                int iRadio;
                int iPhIndex;
                int iState;


                // BUG BUG: Check Accepted Values
                if(GetOption(argc,argv,L"pi",&strArg) >= 0)
                {            
                    iPhIndex = _wtoi(strArg);
                }

                if(GetOption(argc,argv,L"rt",&strArg) >= 0)
                {            
                    iRadio = _wtoi(strArg);
                }
                if(!(iRadio == 0 || iRadio ==1))
                {
                    PrintMsg(L"iRadio can only be 0 or 1");
                    break;
                }
                if(GetOption(argc,argv,L"rs",&strArg) >= 0)
                {            
                    iState = _wtoi(strArg);
                }
                if(!(iState == 0 || iState ==1))
                {
                    PrintMsg(L"iState can only be 0 or 1");
                    break;
                }

                WLAN_PHY_RADIO_STATE wlanRadioState;
                wlanRadioState.dwPhyIndex = iPhIndex;
                DOT11_RADIO_STATE dot11RadioState = iState ? dot11_radio_state_off:dot11_radio_state_on;
                if(iRadio)
                    wlanRadioState.dot11HardwareRadioState = dot11RadioState;
                else
                    wlanRadioState.dot11SoftwareRadioState = dot11RadioState;
                bResult = pWlanInterfaces->SetInterface(strAdapter,opCode,sizeof(WLAN_PHY_RADIO_STATE),&wlanRadioState);                                
                break;
                
        };

    }

    return bResult;
    
}

// -----------------------------------------------------------------------------
BOOL
SetInterface
// Sets User Configurable parameters on the interface
(
    IN int     argc,   // number ars
    IN LPCWSTR argv[]  // arg array
)
{

    if (WasOption(argc, argv, L"?")>0 || 
        ((WasOption(argc, argv, L"ac") < 0) &&
         (WasOption(argc, argv, L"bs") < 0) &&        
         (WasOption(argc, argv, L"sm") < 0) &&        
         (WasOption(argc, argv, L"om") < 0) &&        
         (WasOption(argc, argv, L"sr") < 0) &&        
         (WasOption(argc, argv, L"ty") < 0)))
    {
        PrintMsg(L"-si options:");
        PrintMsg(L"   Usage: -si -i <Interface Description> -ac 0");
        PrintMsg(L"   -i  <Interface Description> Interface Description");
        PrintMsg(L"       Obtain the above value by running \"wlantool -e\" ");        
        PrintMsg(L"       Eg: -i {97D0D9DA-B813-1372-E499-FB3C44EDACE0} or");     
        PrintMsg(L"       Eg: -i ISLP21");     
        PrintMsg(L"   -ac  <0 for Disable and 1 for Enable> ");
        PrintMsg(L"        Enables or disables auto config for the indicated interface");
        PrintMsg(L"        Eg: -ac 1");
        PrintMsg(L"   -bs  <0 for Disable and 1 for Enable> ");
        PrintMsg(L"        Enables or disables background scan for the indicated interface");
        PrintMsg(L"        Eg: -bs 0");
        PrintMsg(L"   -sm  <0 for Disable and 1 for Enable> ");
        PrintMsg(L"        Sets Media Streaming mode for the driver");
        PrintMsg(L"        Eg: -sm 0");               
        PrintMsg(L"   -om  <Operation Mode of the interface");
        PrintMsg(L"        0 for DOT11_OPERATION_MODE_EXTENSIBLE_STATION");
        PrintMsg(L"        1 for DOT11_OPERATION_MODE_NETWORK_MONITOR");
        PrintMsg(L"        Sets the current operation mode of the interface");
        PrintMsg(L"        Eg: -om 1");        
        PrintMsg(L"   -sr  -pi <Phy Index> -rt <Radio Type> -rs <Radio State>");
        PrintMsg(L"        Sets the Radio State of the interface");
        PrintMsg(L"        Eg: -sr -pi 4 -rt 1 -rs 1");                
        PrintMsg(L"        -pi  <Phy Index");        
        PrintMsg(L"             Obtain Phy Indexes from \"wzctool -q \"");
        PrintMsg(L"             Phy Index");
        PrintMsg(L"             Eg: -pi 4");          
        PrintMsg(L"        -rt  <Radio Type");        
        PrintMsg(L"             0 for Software");
        PrintMsg(L"             1 for Hardware");
        PrintMsg(L"             Pick the Type of Radio");
        PrintMsg(L"             Eg: -rt 1");          
        PrintMsg(L"        -rs  <Radio State");        
        PrintMsg(L"             0 for OFF");
        PrintMsg(L"             1 for ON");
        PrintMsg(L"             Sets the Software Radio state for the interface");
        PrintMsg(L"             Eg: -rs 1");               
        PrintMsg(L"   -ty  <Bss Type");
        PrintMsg(L"        1 for Infrastructure");
        PrintMsg(L"        2 for Independent");
        PrintMsg(L"        3 for Any");        
        PrintMsg(L"        Sets the the current operation mode of the interface");
        PrintMsg(L"        Eg: -ty 1");                 
        PrintMsg(L"");
        return FALSE;
    }
  
    BOOL bResult = FALSE;
    WlanInterfaces* pWlanInterfaces = NULL;
    do
    {

        LPCWSTR strAdapter = NULL;
        if (GetOption(argc, argv, L"i", &strAdapter) < 0)
        {
            GetFirstWirelessNetworkCard();
            if (!*g_WirelessCardName)    // wifi card not found
                break;
            strAdapter = g_WirelessCardName;
        }

        pWlanInterfaces = new WlanInterfaces();
        if(NULL == pWlanInterfaces)
        {
            PrintMsg(L"Memory Allocation(new) Failed error: %u",GetLastError());    
            break;
        }
       
        WLAN_INTF_OPCODE opCode;
        if(WasOption(argc, argv, L"ac") > 0)
        {
            opCode = wlan_intf_opcode_autoconf_enabled;
            if(!SetInterface(argc,
                             argv,
                             pWlanInterfaces,
                             strAdapter,
                             L"ac",
                             opCode))
                break;
        }

        if(WasOption(argc, argv, L"bs") > 0)
        {
            opCode = wlan_intf_opcode_background_scan_enabled;
            if(!SetInterface(argc,
                              argv,
                              pWlanInterfaces,
                              strAdapter,
                              L"bs",
                              opCode))
                break;
        }

        if(WasOption(argc, argv, L"sm") > 0)
        {
            opCode = wlan_intf_opcode_media_streaming_mode;
            if(!SetInterface(argc,
                              argv,
                              pWlanInterfaces,
                              strAdapter,
                              L"sm",
                              opCode))
                break;
        }

        if(WasOption(argc, argv, L"om") > 0)
        {
            opCode = wlan_intf_opcode_current_operation_mode;
            if(!SetInterface(argc,
                              argv,
                              pWlanInterfaces,
                              strAdapter,
                              L"om",
                              opCode))
                break;
        }

        if(WasOption(argc, argv, L"ty") > 0)
        {
            opCode = wlan_intf_opcode_bss_type;
            if(!SetInterface(argc,
                              argv,
                              pWlanInterfaces,
                              strAdapter,
                              L"ty",
                              opCode))
                break;
        }
        if(WasOption(argc, argv, L"sr") > 0)
        {
            opCode = wlan_intf_opcode_radio_state;
            if(!SetInterface(argc,
                              argv,
                              pWlanInterfaces,
                              strAdapter,
                              L"sr",
                              opCode))
                break;
        }
        bResult = TRUE;
    }while(FALSE);

    if(pWlanInterfaces)
        delete pWlanInterfaces;

    return bResult;

}



// ========================================================================== //
// Main:
// ========================================================================== //

LPCWSTR g_szHelp[] =
{
    L"wlantool usage:",
    L"options:",
    L" <Interface Description> can be obtained from wlantool -e ",
    L"     is either of the form {97D0D9DA-B813-1372-E499-FB3C44EDACE0}",
    L"     or ISLP21. Look for Interface Description entry",
    L" -e  Enumerate wireless cards",
    L" -q <Interface Description>  Query wireless card",
    L" -ln -i <Interface Description>",
    L"     Lists Available Networks",
    L" -sc -i <Interface Description> -id <SSID>",
    L"     -id <SSID> SSID of the Network to Scan (Optional)",
    L"     Scans for Networks using the given interface",   
    L" -lb -i <Interface Description>",
    L"     -id <SSID> SSID of the Network to Scan (Optional)",
    L"     Lists Available Network Bss List",   
    L" -sp -i <Interface Description> -f <File Name> Sets the Profile on the interface",
    L"     Use -sp -? for details",
    L" -spl -i <Interface Description> -p <ProfileName1> <ProfileName2> ",
    L"     Sets the Preference order of the Profiles on the interface",
    L"     Use -sp -? for details",    
    L" -si -i <Interface Description> ...",
    L"     Sets the Configurable parameters on the interface",
    L"     Use -si -? for details",       
    L" -lpl -i <Interface Description> Lists Profile List stored for the interface",
    L"     Use -lpl -? for details",   
    L" -lp -i <Interface Description> -p <Profile Name> Lists the specified Profile stored for the interface",
    L"     Use -lp -? for details",       
    L" -dp -i <Interface Description> -p <Profile Name> ",
    L"     Deletes the Profile stored for the interface",
    L"     Use -dp -? for details",           
    L" -rp -i <Interface Description> -p <Profile Name> -r <New Profile Name>",
    L"     Renames an existing profile name to a new Profile Name",
    L"     Use -rp -? for details",               
    L" -spp -i <Interface Description> -p <Profile Name> -n <Order in the Preference List> ",
    L"     Sets the position of the Profile in the Preference List",
    L"     Use -dp -? for details",     
    L" -cp -i <Interface Description> -p <Profile Name>",
    L"     Calls Connect using the speicified Profile Name",
    L" -c -i <Interface Description> -ssid <SSID> -auth <Auth> -encr <Encr> -key <index>/<Key> -eap <EapType>",    
    L"     Adds the specified Network to the Profile Store and sets",
    L"     the profile to the top of the preferred List",
    L"     <Interface Description> get this info from wlantool -e",
    L"     Note <Interface Description> is of the form {97d0d9da-b813-1372-e499-fb3c44edace0} or just ISLP21",
    L"     Use -c -? for details",
    L"     Usage -c -i <Interface Description> -ssid AP-SSID -auth open -encr wep -key 0/1234567890",
    L"     KeyMaterial 1234567890 can be either Hex or Ascii based on Auth Cipher Combination and length of the key",
    L"     connect to AP-SSID with given parameters",
    L"     Use -c -? for details",
    L" -d -i <Interface Description>",
    L"     Disconnects the specified interface from the network",    
    L" -stp -i <Interface Description> -p <Profile Name> -n <Order in the Preference List> ",
    L"     Saves a temporary profile",
    L"     Use -stp -? for details",     
    L" -gf -i <Interface Description> -ty <Filter List Type>",
    L"     Gets the Filter List",        
    L"     Use -gf -? for details",    
    L" -sf -i <Interface Description> -ty <Filter List Type> -id <SSID1> <SSID2>",
    L"     -id <SSID> SSID of the Network to Filter",
    L"     Gets the Filter List",        
    L"     Use -sf -? for details",        
    L" -ac -i <Interface Description> -p <Parameter> -v <Enable/Disable>",
    L"     -p is Optional if not present it just queries the parameters"
    L"     Gets / Sets the AutoConfig Parameter",        
    L"     Use -ac -? for details",        
    L"if no <Card Name> is given, wzctool will use the first WiFi card it finds.",
    L" -?  shows this help message",
    NULL
};

int
wmain
// main function for WCHAR console application
(
                      IN int     argc,   // number args
    __in_ecount(argc) IN LPCWSTR argv[]  // arg array
)
{
    PrintMsg(L"Wireless Configuration Tool");

    BOOL succeeded = FALSE;
    int  minimumArgs = 1;
    if (WasOption(argc, argv, L"e") > 0)
        //Enumerate all wireless network cards
        succeeded = EnumWirelessNetworkCard();
    else
    if (WasOption(argc, argv, L"q") > 0)
        // Query a Wireless card using its Description String
        succeeded = PrintWlanIntf(argc, argv);
    else       
    if (WasOption(argc, argv, L"ln") > 0)
        // List Available Networks
        succeeded = ListNetworks(argc, argv);          
    else       
    if (WasOption(argc, argv, L"sc") > 0)
        // Scan for the available network
        succeeded = Scan(argc, argv);             
    else       
    if (WasOption(argc, argv, L"lb") > 0)
        // Lists Bss Entries
        succeeded = ListBss(argc, argv);    
    else
    if (WasOption(argc, argv, L"lpl") > 0)
        // Lists Profile List
        succeeded = ListProfileList(argc, argv);      
    else       
    if (WasOption(argc, argv, L"lp") > 0)
        // Lists a Particular Profile
        succeeded = ListProfile(argc, argv);   
    else       
    if (WasOption(argc, argv, L"dp") > 0)
        // Deletes the Specified Profile
        succeeded = DeleteProfile(argc, argv);       
    else       
    if (WasOption(argc, argv, L"rp") > 0)
        // Renames the Specified Profile
        succeeded = RenameProfile(argc, argv);           
    else       
        // Sets the profile position
    if (WasOption(argc, argv, L"spp") > 0)
        succeeded = SetProfilePosition(argc, argv);        
    else       
    if (WasOption(argc, argv, L"d") > 0)
        //Disconnects
        succeeded = Disconnect(argc, argv);             
    else
    if (WasOption(argc, argv, L"si") > 0)
        //Set interface attributes
        succeeded = SetInterface(argc, argv);
    else               
    if (WasOption(argc, argv, L"ac") > 0)
        // Set/Query AutoConfig Parameters
        succeeded = AutoConfigParam(argc, argv);                     
    else              
    if (WasOption(argc, argv, L"c") > 0)
        // Connects either to the Specified Profile or
        // creates a new Profile for connection
        succeeded = Connect(argc, argv);
    else
    if (WasOption(argc, argv, L"sp") > 0)
        // Sets the profile on a Particular interface
        succeeded = SetProfile(argc, argv);
    else       
    if (WasOption(argc, argv, L"spl") > 0)
        // Sets the Profile List
        succeeded = SetProfileList(argc, argv);    
    else       
    if (WasOption(argc, argv, L"gf") > 0)
        // Gets the Filter list
        succeeded = GetFilterList(argc, argv);                 
    else       
    if (WasOption(argc, argv, L"sf") > 0)
        // Sets the filter List
        succeeded = SetFilterList(argc, argv);                 
    else       
    if (WasOption(argc, argv, L"cp") > 0)
        succeeded = ConnectProfile(argc, argv);                 
    else                      
    if (WasOption(argc, argv, L"stp") > 0)
        // Saves the Temporary Profile
        succeeded = SaveTempProfile(argc, argv);          
    else
    {
        if (WasOption(argc, argv, L"?")    > 0
         || WasOption(argc, argv, L"help") > 0)
            succeeded = TRUE;
        else
        {
            succeeded = FALSE;
            PrintMsg(L"unknown command");
        }
        PrintMsgList(g_szHelp);
    }

    succeeded ? PrintMsg(L"SUCCESS"):PrintMsg(L"FAILED");
    return succeeded? 0 : 1;
} // wmain()

// -----------------------------------------------------------------------------
