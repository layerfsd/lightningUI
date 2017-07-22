/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkWifiApi.h#1 $ */
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
/// \brief      WDK Wifi ACS and Status APIs
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKWIFI_API_H_
#define _WDKWIFI_API_H_
#pragma once

#include <WdkVersion.h>
#include <WifiDKDef.h>
#include <WdkDualModeWifiApi.h>
#include <WdkMacros.h>
#include <WdkConversions.h>

/// \brief ACS Settings  Auto Channel Selection
typedef struct _WDK_ACS_SETTINGS_
{
    DWORD dwSize;                   ///< size of this structure

    /*channel scan options.*/
    BOOL    fBootScanEnabled;       ///< initial scan is forced
    DWORD   dwPeriodicScanDelay;    ///< ticks or 0 if disabled
    DWORD   dwChannelDwell;         ///< time to spend on each channel
    DWORD   dwChannelDelay;         ///< time to wait between individual channels

    /*channel scoring*/
    BOOL    fPreferNonOverlapped;   ///< does channel scoring prefer chan 1,6,11 etc

} WDK_ACS_SETTINGS, *PWDK_ACS_SETTINGS;

/// \brief returns the ACS settings
///
/// \param pSettings pointer to a WDK_ACS_SETTINGS struct
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_ACS_GetSettings(__out PWDK_ACS_SETTINGS pSettings);

/// \brief sets the ACS settings
///
/// \param pSettings pointer to a WDK_ACS_SETTINGS struct
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_ACS_SetSettings(__in PWDK_ACS_SETTINGS pSettings);

/// \brief Wifi monitor escalation state enumeration
typedef enum _WDK_WIFIMON_ACTION
{
    WDK_WIFIMON_Unknown             = -1,   ///< Monitor state is unknown 
    WDK_WIFIMON_Monitoring          = 0,    ///< Wifi driver is currently monitored 
    WDK_WIFIMON_NotMonitored        = 1,    ///< Monitor service is disabled, or starting up
    WDK_WIFIMON_WifiSoftBoot        = 2,    ///< Perform a softboot
    WDK_WIFIMON_WifiPowerToggle     = 3,    ///< Driver is powered down and then powered up (suspend/resume)
    WDK_WIFIMON_WifiReload          = 4,    ///< Driver is powered down, unloaded, reloaded, then powered up
    WDK_WIFIMON_RequestReboot       = 5,    ///< Reboot request event has been set 
    WDK_WIFIMON_FactoryReset        = 6,    ///< Factory reset request event has been set
    WDK_WIFIMON_MonitorWarning1     = 7,    ///< Continue monitoring, warning level 1
    WDK_WIFIMON_MonitorWarning2     = 8,    ///< Continue monitoring, warning level 2
} WDK_WIFIMON_ACTION;

/// \brief Wifi driver status and monitor state
typedef struct _WDK_WIFIMON_INFO
{
    time_t                  tTime;      ///< time of last state change
    WDK_WIFISTATUS          status;     ///< status of the wifi driver
    WDK_WIFIMON_ACTION      action;     ///< last/current action of the wifi monitor
} WDK_WIFIMON_INFO, *PWDK_WIFIMON_INFO;

/// \brief Named event indicating the wifi monitor status has changed
#define WDK_WIFIMON_STATUS_CHANGE_EVENT_NAME   L"WdkWifiStatus"
/// \brief Named event indicating the wifi monitor action has changed
#define WDK_WIFIMON_ACTION_CHANGE_EVENT_NAME   L"WdkWifiAction"

/// \brief Returns the current wifi driver status
///
/// \param pInfo pointer to a WDK_WIFIMON_INFO struct
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_WifiMonitor_GetStatus(__out PWDK_WIFIMON_INFO pInfo);

/// \brief Returns the Wifi monitor action enum value to a string
__inline LPCWSTR WifiMonActionToString(WDK_WIFIMON_ACTION action)
{
    static WCHAR szBuf[64];
    LPCWSTR wzCase = NULL;

    switch(action)
    {
        CASE_TO_STRING(WDK_WIFIMON_Unknown);
        CASE_TO_STRING(WDK_WIFIMON_Monitoring);
        CASE_TO_STRING(WDK_WIFIMON_NotMonitored);
        CASE_TO_STRING(WDK_WIFIMON_WifiSoftBoot);
        CASE_TO_STRING(WDK_WIFIMON_WifiPowerToggle);
        CASE_TO_STRING(WDK_WIFIMON_WifiReload);
        CASE_TO_STRING(WDK_WIFIMON_RequestReboot);
        CASE_TO_STRING(WDK_WIFIMON_FactoryReset);
        CASE_TO_STRING(WDK_WIFIMON_MonitorWarning1);
        CASE_TO_STRING(WDK_WIFIMON_MonitorWarning2);
        default:
        {
            StringCchPrintf(SZARGS(szBuf), L"Unknown action(%d)", action);
            wzCase = szBuf;
        }
    }
    return wzCase;
}

/// \brief Returns the Wifi driver status enum value to a string
__inline LPCWSTR WifiStatusToString(WDK_WIFISTATUS status)
{
    static WCHAR szBuf[64];
    LPCWSTR wzCase = NULL;

    switch(status)
    {
        CASE_TO_STRING(WDK_WIFISTATUS_Unknown);
        CASE_TO_STRING(WDK_WIFISTATUS_Okay);
        CASE_TO_STRING(WDK_WIFISTATUS_LoaderDisabled);
        CASE_TO_STRING(WDK_WIFISTATUS_DriverUnloaded);
        CASE_TO_STRING(WDK_WIFISTATUS_DriverLoading);
        CASE_TO_STRING(WDK_WIFISTATUS_DriverDisabled);
        CASE_TO_STRING(WDK_WIFISTATUS_DriverNoResp);
        CASE_TO_STRING(WDK_WIFISTATUS_DriverInternalFailure);
        CASE_TO_STRING(WDK_WIFISTATUS_DriverFirmwareFailure);
        CASE_TO_STRING(WDK_WIFISTATUS_DriverLicenseFailure);
        CASE_TO_STRING(WDK_WIFISTATUS_NdisuioNotLoaded);
        CASE_TO_STRING(WDK_WIFISTATUS_OutOfMemory);
        CASE_TO_STRING(WDK_WIFISTATUS_BadUserInput);
        CASE_TO_STRING(WDK_WIFISTATUS_PowerOff);
        default:
        {
            StringCchPrintf(SZARGS(szBuf), L"Unknown status(%d)", status);
            wzCase = szBuf;
        }
    }
    return wzCase;
}

typedef enum
{
    WDK_WIFI_SIGNAL_STRENGTH_0             = 0, ///< Signal strength 0/5 bars
    WDK_WIFI_SIGNAL_STRENGTH_1             = 1, ///< Signal strength 1/5 bars
    WDK_WIFI_SIGNAL_STRENGTH_2             = 2, ///< Signal strength 2/5 bars
    WDK_WIFI_SIGNAL_STRENGTH_3             = 3, ///< Signal strength 3/5 bars
    WDK_WIFI_SIGNAL_STRENGTH_4             = 4, ///< Signal strength 4/5 bars
    WDK_WIFI_SIGNAL_STRENGTH_5             = 5, ///< Signal strength 5/5 bars
    WDK_WIFI_SIGNAL_STRENGTH_MAX           = WDK_WIFI_SIGNAL_STRENGTH_5, ///< Number of signal strength bars
} WDK_WIFI_SIGNAL_STRENGTH;

#define WDK_WIFI_RSSI_VERY_LOW       -97    ///< Very low signal strength threshold
#define WDK_WIFI_RSSI_LOW            -88    ///< Low signal strength threshold
#define WDK_WIFI_RSSI_ACCEPTABLE     -80    ///< Acceptable signal strength threshold
#define WDK_WIFI_RSSI_GOOD           -70    ///< Good signal strength threshold
#define WDK_WIFI_RSSI_VERY_GOOD      -62    ///< Very good signal strength threshold

/// \brief Converts an RSSI dBM value to a signal strength value.
__inline WDK_WIFI_SIGNAL_STRENGTH WifiRssiToSignalStrength(LONG lRssidBm)
{
    if(lRssidBm == 0 || lRssidBm <= WDK_WIFI_RSSI_VERY_LOW)
        return WDK_WIFI_SIGNAL_STRENGTH_0;
    
    if(lRssidBm <= WDK_WIFI_RSSI_LOW)
        return WDK_WIFI_SIGNAL_STRENGTH_1;
    
    if(lRssidBm <= WDK_WIFI_RSSI_ACCEPTABLE)
        return WDK_WIFI_SIGNAL_STRENGTH_2;   
    
    if(lRssidBm <= WDK_WIFI_RSSI_GOOD)
        return WDK_WIFI_SIGNAL_STRENGTH_3;
    
    if(lRssidBm <= WDK_WIFI_RSSI_VERY_GOOD)
        return WDK_WIFI_SIGNAL_STRENGTH_4;

    return WDK_WIFI_SIGNAL_STRENGTH_5;
}

typedef struct _WIFI_FREQ_MAP
{
    DWORD dwChan;
    DWORD dwFreq;
} WIFI_FREQ_MAP, *PWIFI_FREQ_MAP;

/// \brief Converts an RSSI dBM value to a signal strength value.
__inline DWORD WifiFrequencyToChannel(ULONG ulFreq)
{
    const WIFI_FREQ_MAP c_map[] =
        {
            {  1, 2412000 },
            {  2, 2417000 },
            {  3, 2422000 },
            {  4, 2427000 },
            {  5, 2432000 },
            {  6, 2437000 },
            {  7, 2442000 },
            {  8, 2447000 },
            {  9, 2452000 },
            { 10, 2457000 },
            { 11, 2462000 },
            { 12, 2467000 },
            { 13, 2472000 },
            { 14, 2484000 },
        };

    DWORD dwChan = 0;
    int i;
    
    for(i=0; i<countof(c_map); ++i)
    {
        if(ulFreq == c_map[i].dwFreq)
        {
            dwChan = c_map[i].dwChan;
            break;
        }
    }

    return dwChan;
}

/// \brief Validate a string MAC or BSSID address, and convert to a byte array
///
/// if delim == '-' then the expected format is "00-c0-d9-00-00-00".
/// if delim == ':' then the expected format is "00:c0:d9:00:00:00".
/// if delim == 0, then the expected format is "00c0d0000000".
///
/// \param pszMac MAC address to valitate or convert 
/// \param cDelim The byte
/// \param bMac [out] Optional buffer that can accept the validated MAC value
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALIDARG: Validation failed
///     - E_*: Other Error Conditions
///
__inline HRESULT ValidateMacA(__in LPCSTR pszMac, CHAR cDelim, __out_opt BYTE bMac[MAC_LEN])
{
    HRESULT hr = S_OK;
    size_t len = 0;
    const size_t c_ADDRLEN = 17;
    CHAR szAddr[18];
    size_t i;
    int j=0;
    int inc = cDelim ? 3 : 2;

    CBREx(pszMac && pszMac[0], E_INVALIDARG);

    hr = StringCchCopyA(SZARGS(szAddr), pszMac);
    CHR(hr);

    hr = StringCchLengthA(szAddr, countof(szAddr), &len);
    CHR(hr);

    CBREx(len == c_ADDRLEN, E_INVALIDARG);

    for(i=0; i<len; i+=inc)
    {        
        BOOL fSuccess = isxdigit(szAddr[i]) && isxdigit(szAddr[i+1]);
        CBREx(fSuccess, E_INVALIDARG);

        if(i < 15 && cDelim)
        {
            CBREx((cDelim == szAddr[i+2]), E_INVALIDARG);
        }

        if(bMac)
        {
            bMac[j++] = (BYTE)hex2byteA(&szAddr[i]);
        }
    }

Error:
    return hr;
}

/// \brief Validate a string MAC or BSSID address, and convert to a byte array
///
/// if delim == '-' then the expected format is "00-c0-d9-00-00-00".
/// if delim == ':' then the expected format is "00:c0:d9:00:00:00".
/// if delim == 0, then the expected format is "00c0d0000000".
///
/// \param pwzMac MAC address to valitate or convert 
/// \param cDelim The byte
/// \param bMac [out] Optional buffer that can accept the validated MAC value
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALIDARG: Validation failed
///     - E_*: Other Error Conditions
///
__inline HRESULT ValidateMacW(__in LPCWSTR pwzMac, WCHAR cDelim, __out_opt BYTE bMac[MAC_LEN])
{
    HRESULT hr = S_OK;
    size_t len = 0;
    const size_t c_ADDRLEN = 17;
    WCHAR szAddr[18];
    size_t i;
    int j=0;
    int inc = cDelim ? 3 : 2;

    CBREx(pwzMac && pwzMac[0], E_INVALIDARG);

    hr = StringCchCopyW(SZARGS(szAddr), pwzMac);
    CHR(hr);

    hr = StringCchLengthW(szAddr, countof(szAddr), &len);
    CHR(hr);

    CBREx(len == c_ADDRLEN, E_INVALIDARG);

    for(i=0; i<len; i+=inc)
    {        
        BOOL fSuccess = iswxdigit(szAddr[i]) && iswxdigit(szAddr[i+1]);
        CBREx(fSuccess, E_INVALIDARG);

        if(i < 15 && cDelim)
        {
            CBREx((cDelim == szAddr[i+2]), E_INVALIDARG);
        }

        if(bMac)
        {
            bMac[j++] = (BYTE)hex2byteW(&szAddr[i]);
        }
    }

Error:
    return hr;
}

/// \brief Validate a string MAC or BSSID address, and convert to a byte array
///
/// if delim == '-' then the expected format is "00-c0-d9-00-00-00".
/// if delim == ':' then the expected format is "00:c0:d9:00:00:00".
/// if delim == 0, then the expected format is "00c0d0000000".
///
/// \param pszMac MAC address to valitate or convert 
/// \param pszDelims Delimiters to test against
/// \param cDelims Count of delimiters to validate
/// \param bMac [out] Optional buffer that can accept the validated MAC value
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALIDARG: Validation failed
///     - E_*: Other Error Conditions
///
__inline HRESULT ValidateMacExA(__in LPCSTR pszMac, LPCSTR pszDelims, DWORD cDelims, __out_opt BYTE bMac[MAC_LEN])
{
    HRESULT hr = E_INVALIDARG;
    DWORD i;
    for(i=0; i<cDelims; ++i)
    {
        hr = ValidateMacA(pszMac, pszDelims[i], bMac);
        if(SUCCEEDED(hr))
        {
            break;
        }
    }

    return hr;
}

/// \brief Validate a string MAC or BSSID address, and convert to a byte array
///
/// if delim == '-' then the expected format is "00-c0-d9-00-00-00".
/// if delim == ':' then the expected format is "00:c0:d9:00:00:00".
/// if delim == 0, then the expected format is "00c0d0000000".
///
/// \param pwzMac MAC address to validate or convert 
/// \param pwzDelims Delimiters to test against
/// \param cDelims Count of delimiters to validate
/// \param bMac [out] Optional buffer that can accept the validated MAC value
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALIDARG: Validation failed
///     - E_*: Other Error Conditions
///
__inline HRESULT ValidateMacExW(__in LPCWSTR pwzMac, LPCWSTR pwzDelims, DWORD cDelims, __out_opt BYTE bMac[MAC_LEN])
{
    HRESULT hr = E_INVALIDARG;
    DWORD i;

    for(i=0; i<cDelims; ++i)
    {
        hr = ValidateMacW(pwzMac, pwzDelims[i], bMac);
        if(SUCCEEDED(hr))
        {
            break;
        }
    }

    return hr;
}

#endif // _WDKWIFI_API_H_

