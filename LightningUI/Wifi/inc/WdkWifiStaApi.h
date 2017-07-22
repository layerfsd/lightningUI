/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkWifiStaApi.h#1 $ */
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
/// \brief      Wifi Station Functions.
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2012-05-05
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_WIFISTA_H_
#define _WDK_WIFISTA_H_
#pragma once

#include <lmcons.h> // UNLEN

#include <WdkVersion.h>
#include <WdkBldVer.h>
#include <WdkMacros.h>
#include <WifiDKDef.h>
#include <WdkWifiTypes.h>

/// \brief Enables/disables the auto connect capability
///
/// CE6: The WZC service is enabled/disabled for the adapter.
/// WEC7: The Native Wi-Fi service is enabled/disabled for the adapter.
///
/// The STA adapter instance must be enabled for this function to succeed.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_SetEnableAutoConnect( __in LPCWSTR pwzAdapterName, BOOL fEnable );

/// \brief Retrieve the auto connect enabled state
///
/// In CE6: The WZC service state for the adapter is retrieved.
/// In WEC7: The Native Wi-Fi service state for the adapter is retrieved.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_GetEnableAutoConnect( __in LPCWSTR pwzAdapterName, __out PBOOL pfEnable );

/// \brief Enables or disables the AP scanner.
///
/// The WDK Scan service must be enabled for this function to enable scanning.
/// This setting is persistent.
///
/// \param fEnable Desired enabled state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WIFI_STA_SetScannerEnabled( BOOL fEnable );

/// \brief Returns the enabled state for the AP scanner.
///
/// The WDK Scan service must be enabled for this function to enable scanning.
///
/// \param pfEnable pointer to a BOOL value to contain the enabled state.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WIFI_STA_GetScannerEnabled( __out BOOL *pfEnable );


/// \brief Returns information about the currently attached AP
///
/// If the STA instance is active and currently connected to an AP, the returned AP list may 
/// need to be refreshed by calling this API again in approx 3 seconds to allow a rescan an
/// a refresh of the maintained AP list.
///
/// This API will fail if the Wi-Fi STA Scan Service is not running. 
///
/// \param pwzAdapterName NDIS adapter
/// \param pAdapterState pointer to a WDK_STA_STATUS value to receive the adapter state
/// \param pAp pointer to a WDK_CURR_AP_INFO struct to receive the results of the query
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_GetCurrentAp( __in LPCWSTR pwzAdapterName, __out PWDK_STA_STATUS pStatus, __out PWDK_CURR_AP_INFO pAp);

/// \brief Returns a list of preferred APs
///
/// If the list count indicates the buffer is too small, this function will return E_INSUFFICIENT_BUFFER.
/// Upon success, the list count will be set to the actual number of valid entries in the list.
///
/// \param pwzAdapterName NDIS adapter
/// \param pList Buffer to hold the results of the list query
/// \param pcList Count of the buffer. 
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_GetPreferredApList( __in LPCWSTR pwzAdapterName, __out_opt PWDK_STA_AP_INFO pList, __inout PDWORD pcList);

/// \brief Removes an AP from the preferred list given its SSID
///
/// \param pwzAdapterName NDIS adapter
/// \param pwzSsid Name of the AP to remove
///
/// \return HRESULT
///     - S_OK: Success
///     - S_FALSE: AP was not in the list
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_RemovePreferredApByName( __in LPCWSTR pwzAdapterName, __in LPCWSTR pwzSsid );

/// \brief Adds an AP to the preferred list
///
/// In WEC7 fVolatile is ignored if Connection Manager is not included in the OS image.
///
/// \param pwzAdapterName NDIS adapter
/// \param pAp AP information
/// \param pSecurity encryption and authentication settings
/// \param fVolatile If TRUE the connection will not be persisted.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_AddPreferredAp( __in LPCWSTR pwzAdapterName, __in PWDK_STA_AP_INFO pAp, __in PWIFI_SECURITY_PARAMS pSecurity, BOOL fVolatile );

/// \brief Enables or disables a preferred AP connection.
///
/// This API is not supported on CE6. 
/// This API is supported on WEA7/WEC7 if connection manager is included in the OS design.
///
/// \param pwzAdapterName NDIS adapter
/// \param pwzSsid SSID of the AP to enable
/// \param fEnable BOOL value indicating the enable state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_EnablePreferredApByName( __in LPCWSTR pwzAdapterName, __in LPCWSTR pwzSsid, BOOL fEnable );

/// \brief Connects to an AP on the preferred list given its SSID
///
/// \param pwzAdapterName NDIS adapter
/// \param pwzSsid Name of the AP to connect
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_FOUND: AP was not in the list
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_ConnectToPreferredApByName( __in LPCWSTR pwzAdapterName, __in LPCWSTR pwzSsid );

/// \brief Sets the IP address configuration for the specified apdapter
///
/// If the default gateway is NULL, any existing value will be deleted. The IP address and subnet mask value will not be
/// deleted if null. Settings will not take effect until the next time the adapter is connected.
///
/// \param pwzAdapterName NDIS adapter
/// \param fDhcp Enable or disable DHCP
/// \param pwzIpAddress Static IP address
/// \param pwzSubnetMask Static subnet mask
/// \param pwzDefaultGateway default gateway
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_FOUND: AP was not in the list
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_SetIpConfig( 
    __in LPCWSTR pwzAdapterName, 
    BOOL fDhcp, 
    __in LPCWSTR pwzIpAddress, 
    __in LPCWSTR pwzSubnetMask, 
    __in_opt LPCWSTR pwzDefaultGateway
);

/// \brief Gets the IP address configuration for the specified apdapter
///
/// \param pwzAdapterName NDIS adapter
/// \param pfDhcp DHCP enabled state
/// \param pwzIpAddress Static IP address buffer
/// \param cchIpAddress length in characters of the IP address buffer
/// \param pwzSubnetMask subnet mask buffer
/// \param cchSubnetMask length in characters of the subnet mask buffer
/// \param pwzDefaultGateway default gateway buffer
/// \param cchDefaultGateway length in characters of the default gateway buffer
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_FOUND: Adapter was not found
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WIFI_STA_GetIpConfig( 
    __in LPCWSTR pwzAdapterName, 
    __in PBOOL pfDhcp, 
    __in LPWSTR pwzIpAddress, DWORD cchIpAddress,
    __in LPWSTR pwzSubnetMask, DWORD cchSubnetMask, 
    __in LPWSTR pwzDefaultGateway, DWORD cchDefaultGateway
);

/// \brief Returns a list of available APs
///
/// If the list count indicates the buffer is too small, this function will return E_INSUFFICIENT_BUFFER.
/// Upon success, the list count will be set to the actual number of valid entries in the list.
///
/// The scan service periodically querys for available APs and maintains a list of available APs. If
/// the STA wifi driver instance is attached to an AP the scan period is 30 seconds. If not attached it is
/// 3 seconds.
///
/// If the wzc scan service is active, calling this function will initiate a scan and activate the fast scan mode for
/// 15 seconds. 
///
/// \param pList Buffer to hold the results of the list query
/// \param pcList Count of the buffer. 
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_GetAvailableApList( __out_opt PWDK_AVAIL_AP_INFO pList, __inout PDWORD pcList );

/// \brief Returns information about the currently attached AP
///
/// If the STA instance is active and currently connected to an AP, the returned AP list may 
/// need to be refreshed by calling this API again in approx 3 seconds to allow a rescan an
/// a refresh of the maintained AP list.
///
/// If the WDK AP scanning service is not included in the image, the pStatus parameter will be set to WDK_STA_STATUS_Unknown,
///
/// \param pStatus pointer to a WDK_STA_STATUS value to receive the adapter state
/// \param pAp pointer to a WDK_CURR_AP_INFO struct to receive the results of the query
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_GetCurrentAp( __out PWDK_STA_STATUS pStatus, __out PWDK_CURR_AP_INFO pAp );

/// \brief Returns a list of preferred APs
///
/// If the list count indicates the buffer is too small, this function will return E_INSUFFICIENT_BUFFER.
/// Upon success, the list count will be set to the actual number of valid entries in the list.
///
/// \param pList Buffer to hold the results of the list query
/// \param pcList Count of the buffer. 
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_GetPreferredApList( __out_opt PWDK_STA_AP_INFO pList, __inout PDWORD pcList );

/// \brief Removes an AP from the preferred list given its SSID
///
/// \param pwzSsid Name of the AP to remove
///
/// \return HRESULT
///     - S_OK: Success
///     - S_FALSE: AP was not in the list
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_RemovePreferredApByName( __in LPCWSTR pwzSsid );

/// \brief Adds an AP to the preferred list
///
/// \param pAp AP information
/// \param pSecurity encryption key data
/// \param fVolatile If TRUE the connection will not be persisted.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_AddPreferredAp( __in PWDK_STA_AP_INFO pAp, __in PWIFI_SECURITY_PARAMS pSecurity, BOOL fVolatile );

/// \brief Enables or disables an the preferred AP connection.
///
/// This API is not supported on CE6.
///
/// \param pwzSsid SSID of the preferred AP to enable
/// \param fEnable BOOL value indicating the enable state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_EnablePreferredApByName( __in LPCWSTR pwzSsid, BOOL fEnable );

/// \brief Connects to an AP on the preferred list given its SSID
///
/// \param pwzSsid Name of the AP to connect
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_FOUND: AP was not in the list
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_ConnectToPreferredApByName( __in LPCWSTR pwzSsid );

/// \brief Renews the IP Address of the adapter
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_STA_RenewIPAddress( VOID );

#endif // _WDK_WIFISTA_H_

