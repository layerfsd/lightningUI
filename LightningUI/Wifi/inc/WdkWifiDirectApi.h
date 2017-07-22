/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkWifiDirectApi.h#1 $ */
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
/// \brief      WDK Wi-Fi Direct APIs
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKWIFIDIRECTAPI_H_
#define _WDKWIFIDIRECTAPI_H_
#pragma once

//sdk 
#include <WdkVersion.h>
#include <WdkWfdTypes.h>
#include <WdkWps.h>
#include <WifiDKdef.h>
#include <WdkWifiStaApi.h>
#include <WdkEhm.h>

#define WDK_WFD_DEFAULT_CHANNEL     1

//
// WiFi Direct Definitions
//
#define WFD_API_VERSION_1_0    0x00000001

#define WFD_API_SUPPORTED
#define WFD_API_VERSION WFD_API_VERSION_1_0

/// \brief Returns the Wi-Fi Direct state enum value to a string
__inline LPCWSTR WifiDirectStateToString(WDK_WFD_STATE state)
{
    LPCWSTR wzCase = NULL;

    switch(state)
    {
        CASE_TO_STRING(WDK_WFD_STATE_Disabled);
        CASE_TO_STRING(WDK_WFD_STATE_Idle);
        CASE_TO_STRING(WDK_WFD_STATE_Discovery);
        CASE_TO_STRING(WDK_WFD_STATE_GroupFormation);
        CASE_TO_STRING(WDK_WFD_STATE_Connect);
        default:
        {
            static WCHAR szBuf[64];
            VBR(FALSE);
            szBuf[0] = 0;
            StringCchPrintf(SZARGS(szBuf), L"Unknown state(%d)", state);
            wzCase = szBuf;
        }
    }
    return wzCase;
}

/// \brief Returns the Wi-Fi Direct group formation phase enum value to a string
__inline LPCWSTR WifiDirectGroupFormationSubstateToString(WDK_WFD_GROUP_FORMATION_PHASE phase)
{
    LPCWSTR wzCase = NULL;

    switch(phase)
    {
        CASE_TO_STRING(WDK_WFD_GROUP_FORMATION_PHASE_Unknown);
        CASE_TO_STRING(WDK_WFD_GROUP_FORMATION_PHASE_Waiting);
        CASE_TO_STRING(WDK_WFD_GROUP_FORMATION_PHASE_RemoteRequest);
        CASE_TO_STRING(WDK_WFD_GROUP_FORMATION_PHASE_LocalRequest);
        CASE_TO_STRING(WDK_WFD_GROUP_FORMATION_PHASE_StartClient);
        CASE_TO_STRING(WDK_WFD_GROUP_FORMATION_PHASE_StartGO);
        default:
        {
            static WCHAR szBuf[64];
            VBR(FALSE);
            szBuf[0] = 0;
            StringCchPrintf(SZARGS(szBuf), L"Unknown group formation phase(%d)", phase);
            wzCase = szBuf;
        }
    }
    return wzCase;
}

__inline LPCWSTR WifiDirectConnectSubstateToString(WDK_WFD_CONNECT_PHASE phase)
{
    LPCWSTR wzCase = NULL;

    switch(phase)
    {
        CASE_TO_STRING(WDK_WFD_CONNECT_PHASE_Unknown);
        CASE_TO_STRING(WDK_WFD_CONNECT_PHASE_ClientWPS);
        CASE_TO_STRING(WDK_WFD_CONNECT_PHASE_ClientProfile);
        CASE_TO_STRING(WDK_WFD_CONNECT_PHASE_ClientConnecting);
        CASE_TO_STRING(WDK_WFD_CONNECT_PHASE_ClientConnected);
        CASE_TO_STRING(WDK_WFD_CONNECT_PHASE_StartGO);
        default:
        {
            static WCHAR szBuf[64];
            VBR(FALSE);
            szBuf[0] = 0;
            StringCchPrintf(SZARGS(szBuf), L"Unknown connect phase(%d)", phase);
            wzCase = szBuf;
        }
    }
    return wzCase;
}

/// \brief The WDK_WFD_OpenHandle function opens a handle to the Wi-Fi Direct service and negotiates a version of the Wi-FI Direct API to use.
///
/// In order to use Wi-Fi Direct, an application must first obtain a handle to the Wi-Fi Direct service by calling the WDK_WFD_OpenHandle function. 
/// The Wi-Fi Direct (WFD) handle returned by the WDK_WFD_OpenHandle function is used for subsequent calls made to the Wi-Fi Direct service. 
/// Once an application is done using the Wi-Fi Direct service, the application should call the WDK_WFD_CloseHandle function to signal to the 
/// Wi-Fi Direct service that the application is done using the service. This allows the Wi-Fi Direct service to release resources used by the application.
///
/// \param dwClientVersion [in] The highest version of the Wi-Fi Direct API the client supports. This parameter should be set to WFD_API_VERSION. 
/// \param pdwNegotiatedVersion [out] A pointer to a DWORD to received the negotiated version.
///         If the WDK_WFD_OpenHandle function is successful, the version negotiated with the Wi-Fi Direct Service to be used by this session is returned. 
///         This value is usually the highest version supported by both the client and Wi-Fi Direct service.
/// \param phClientHandle [out] A pointer to a HANDLE to receive the handle to the Wi-Fi Direct service for this session.
///         If the WDK_WFD_OpenHandle function is successful, a handle to the Wi-Fi Direct service to use in this session is returned. 
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALID_PARAMETER (E_INVALIDARG)
///     - E_OUTOFMEMORY
///     - Other HRESULTs
///
WifiDKCore_API HRESULT WDK_WFD_OpenHandle( DWORD dwClientVersion, __out PDWORD pdwNegotiatedVersion, __out PHANDLE phClientHandle );

/// \brief The WDK_WFD_CloseHandle function closes a handle to the Wi-Fi Direct service.
///
/// \param hClientHandle [in] A client handle to the Wi-Fi Direct service. This handle was obtained by a previous call to the WDK_WFD_OpenHandle function.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALID_PARAMETER (E_INVALIDARG)
///
WifiDKCore_API HRESULT WDK_WFD_CloseHandle( HANDLE hClientHandle );

/// \brief Gets the state of the WFD service
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param pState Pointer to a WDK_WFD_STATE indicating the state of the discovery service
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_GetState( HANDLE hWFD, __out PWDK_WFD_STATE pState );

/// \brief Sets callbacks used by the WiFi Direct service. 
///
/// Callbacks are made when certain events occur. Each callback is made on a separate thread.
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param pCallbacks Pointer to a WDK_WFD_CALLBACKS struture.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALID_PARAMETER (E_INVALIDARG)
///
WifiDKCore_API HRESULT WDK_WFD_SetCallbacks( HANDLE hWFD, __in_opt PWDK_WFD_CALLBACKS pCallbacks );

/// \brief The WFD_OPEN_SESSION_COMPLETE_CALLBACK function defines the callback function that is called by the WFDStartOpenSession function when the WFDStartOpenSession operation completes. 
typedef VOID (WINAPI *WFD_OPEN_SESSION_COMPLETE_CALLBACK) (
    HANDLE         hSessionHandle,      ///< A session handle to a Wi-Fi Direct session. This is a session handle previously returned by the WFDStartOpenSession function.
    PVOID          pvContext,           ///< User data
    GUID           guidSessionInterface,///< GUID specifing the session interface
    DWORD          dwError,             ///< A value that specifies whether there was an error encountered during the call to the WFDStartOpenSession function. If this value is ERROR_SUCCESS, then no error occurred and the operation to open the session completed successfully.
    DWORD          dwReasonCode         ///< Not used
    );

/// \brief The WFDStartOpenSession function starts an on-demand connection to a specific Wi-Fi Direct device.
///
/// The WFDStartOpenSession function starts an asynchronous operation to start an on-demand connection to a specific Wi-Fi Direct device. 
/// The target Wi-Fi device must previously have been paired through the WDK_WFD APIs. When the asynchronous operation completes, the 
/// callback function specified in the pfnCallback parameter is called. 
///
/// If the application attempts to close the handle to the Wi-Fi Direct service by calling the WDK_WFD_CloseHandle function before the 
/// WFDStartOpenSession function completes asynchronously, the WDK_WFD_CloseHandle function will wait until the WFDStartOpenSession call is completed.
///
/// \param hClientHandle [in] A client handle to the Wi-Fi Direct service. This handle was obtained by a previous call to the WDK_WFD_OpenHandle function.
/// \param pDeviceAddress [in] A pointer to the target device’s Wi-Fi Direct device address. This is the MAC address of the target Wi-Fi device. 
/// \param pvContext [in, optional] An optional context pointer which is passed to the callback function specified in the pfnCallback parameter.
/// \param pfnCallback [in] A pointer to the callback function to be called once the WFDStartOpenSession request has completed.
/// \param phSessionHandle [out] A handle to this specific Wi-Fi Direct session.
///
/// \return HRESULT
///     - ERROR_SUCCESS: Success
///     - ERROR_INVALID_PARAMETER
///     - Other Error Conditions
///
WifiDKCore_API
DWORD
WFDStartOpenSession(
    HANDLE                  hClientHandle,
    __in PDOT11_MAC_ADDRESS pDeviceAddress,
    __in_opt PVOID           pvContext,
    __in WFD_OPEN_SESSION_COMPLETE_CALLBACK pfnCallback,
    __out PHANDLE           phSessionHandle
    );

/// \brief The WFDCancelOpenSession function indicates that the application wants to cancel a pending WFDStartOpenSession function that has not completed.
///
/// A call to the WFDCancelOpenSession function notifies the Wi-Fi Direct service that the client requests a cancellation of this session. 
/// The WFDCancelOpenSession function does not modify the expected WFDStartOpenSession behavior. The callback function specified to the 
/// WFDStartOpenSession function will still be called, and the WFDStartOpenSession function may not be completed immediately.
/// 
/// It is the responsibility of the caller to pass the WFDCancelOpenSession function a handle in the hSessionHandle parameter that was 
/// returned from call to the WFDStartOpenSession function.
/// 
/// \param hSessionHandle [in] A session handle to a Wi-Fi Direct session to cancel. This is a session handle previously returned by the WFDStartOpenSession function.
///
/// \return HRESULT
///     - ERROR_SUCCESS: Success
///     - ERROR_INVALID_PARAMETER: Invalid parameter
///     - ERROR_ALREADY_EXISTS: An open WFD session already exists
///     - Other Error Conditions
///
WifiDKCore_API
DWORD
WFDCancelOpenSession(
    HANDLE                  hSessionHandle
    );

/// \brief The WFDOpenLegacySession function retrieves and applies a stored profile for a Wi-Fi Direct legacy device.
///
/// The WFDOpenLegacySession function retrieves the stored legacy profile for device from the profile store for the 
/// specified legacy device address. This device address must be obtained from a Device Node created as a result of 
/// the Inbox pairing experience (Legacy WPS Pairing).
///
/// Once an application is done using the Wi-Fi Direct service, the application should call the WFDCloseSession function 
/// to close the session and call the WDK_WFD_CloseHandle function to signal to the Wi-Fi Direct service that the application 
/// is done using the service. This allows the Wi-Fi Direct service to release resources used by the application.
/// 
/// \param hClientHandle A HANDLE to the Wi-Fi Direct service for this session. This parameter is retrieved using the WDK_WFD_OpenHandle function.
/// \param pLegacyMacAddress Not used, set to NULL.
/// \param phSessionHandle A pointer to a HANDLE to receive the handle to the Wi-Fi Direct service for this session.
///         If the WFDOpenLegacySession function is successful, a handle to the Wi-Fi Direct service to use in this session is returned. 
/// \param pGuidSessionInterface GUID specifing the session interface.
///
/// \return HRESULT
///     - ERROR_SUCCESS: Success
///     - ERROR_INVALID_PARAMETER
///     - Other Error Conditions
///
WifiDKCore_API
DWORD
WFDOpenLegacySession(
    HANDLE                  hClientHandle,
    __in_opt PDOT11_MAC_ADDRESS pLegacyMacAddress,
    __out HANDLE*           phSessionHandle,
    __out GUID*             pGuidSessionInterface
    );

/// \brief The WFDCloseSession function closes a session after a previously successful call to the WFDStartOpenSession or WFDOpenLegacySession functions
///
/// The WFDCloseSession function queues a future work item to close the session, so disconnection may not be immediate.
/// Calling the WFDCloseSession function while a WFDStartOpenSession call is pending will not close the session.
/// It is the responsibility of the caller to pass the WFDCloseSession function a handle in the hSessionHandle parameter 
/// that was returned from a successful asynchronous call to the WFDStartOpenSession function.
///
/// Calling the WFDCloseSession function with a handle that was valid and has become invalid will yield undefined results.
/// 
/// \param hSessionHandle [in] A session handle to a Wi-Fi Direct session. This is a session handle previously returned by the WFDStartOpenSession function.
///
/// \return HRESULT
///     - ERROR_SUCCESS: Success
///     - ERROR_INVALID_PARAMETER
///     - Other Error Conditions
///
WifiDKCore_API
DWORD
WFDCloseSession(
    HANDLE                      hSessionHandle
    );

// ----------------------------------------------
//  This Device 

/// \brief Saves the device information in the registry
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param pInfo [in] A WDK_WFD_DEVICE_INFO structure.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_SaveDeviceInfo( HANDLE hWFD, __in PWDK_WFD_DEVICE_INFO pInfo );

/// \brief Gets the device information from the registry
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param pInfo [out] A WDK_WFD_DEVICE_INFO structure.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_GetSavedDeviceInfo( HANDLE hWFD, __out PWDK_WFD_DEVICE_INFO pInfo );

/// \brief Saves the listen channel to the registry
///
/// The new listen channel will take effect the next time WFD mode is enabled.
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param wChannelId The channel ID.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_SaveListenChannelId( HANDLE hWFD, WORD wChannelId );

/// \brief Gets the listen from the registry
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param pwChannelId Pointer to a WORD to receive the channel ID.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_GetSavedListenChannelId( HANDLE hWFD, __out PWORD pwChannelId );

WifiDKCore_API HRESULT WDK_WFD_SaveDeviceCaps( HANDLE hWFD, __in PWDK_WFD_DEVICE_CAPABILITIES pCaps );
WifiDKCore_API HRESULT WDK_WFD_GetSavedDeviceCaps( HANDLE hWFD, __out PWDK_WFD_DEVICE_CAPABILITIES pCaps );

WifiDKCore_API HRESULT WDK_WFD_SaveGroupOwnerCaps( HANDLE hWFD, __in PWDK_WFD_GROUP_OWNER_CAPABILITIES pCaps );
WifiDKCore_API HRESULT WDK_WFD_GetSavedGroupOwnerCaps( HANDLE hWFD, __out PWDK_WFD_GROUP_OWNER_CAPABILITIES pCaps );

WifiDKCore_API HRESULT WDK_WFD_SaveGroupId( HANDLE hWFD, __in PWDK_WFD_GROUP_ID pId );
WifiDKCore_API HRESULT WDK_WFD_GetSavedGroupId( HANDLE hWFD, __out PWDK_WFD_GROUP_ID pId );

WifiDKCore_API HRESULT WDK_WFD_SaveAutonomousChannelId( HANDLE hWFD, WORD wChannelId );
WifiDKCore_API HRESULT WDK_WFD_GetSavedAutonomousChannelId( HANDLE hWFD, __out PWORD pwChannelId );

// ----------------------------------------------
//  Device Discovery

/// \brief Gets a waitable event to indicate scan and discovery events
///
/// The caller can use
/// WFSO or WFMO to wait for the discovery completion. The caller is responsible
/// for closing this handle using CloseHandle. 
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param phWaitHandle pointer to a waitable handle.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_GetEvent( HANDLE hWFD, __out PHANDLE phWaitHandle );

/// \brief Start continuous WFD device discovery (scan, find, & listen)
///
/// The event obtained using WDK_WFD_GetEvent is set when
/// the discovery operation is complete or stopped.
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALID_STATE: GO mode is active
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_StartDiscovery( HANDLE hWFD );

/// \brief Stops WFD device discovery started by WDK_WFD_StartDiscovery
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_StopDiscovery( HANDLE hWFD );

/// \brief Returns a list of discovered WFD devices
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param pList buffer to receive a list of discovered devices.
/// \param pcList in: element count of the buffer. out: number of device entries copied to the buffer.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_GetDiscoveredDevices( HANDLE hWFD, __out_ecount_opt(*pcList) PWDK_WFD_DISCOVERED_DEVICE pList, __inout PDWORD pcList);

// ----------------------------------------------
//  Device Connections

/// \brief Starts a pairing to the specified Wi-Fi Direct device
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
/// \param pDevice Pointer to a PWDK_WFD_DEVICE_INFO structure.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_StartPairing( HANDLE hWFD, __in PWDK_WFD_DEVICE_INFO pDevice );

/// \brief Cancels a pairing action started by WDK_WFD_StartPairing
///
/// \param hWFD Handle returned by WDK_WFD_OpenHandle.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
WifiDKCore_API HRESULT WDK_WFD_CancelPairing( HANDLE hWFD );


#endif // _WDKWIFIDIRECTAPI_H_

