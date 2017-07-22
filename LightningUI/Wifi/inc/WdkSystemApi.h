
/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkSystemApi.h#1 $ */
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
/// \brief      Common CE system API's
/// \remarks
///             - Original Author :  Lester Meeks
///             - Original Date   :  2010-10-11
//-----------------------------------------------------------------------------
/// \file 

#ifndef _WDK_SYSTEMUTILAPI_H_
#define _WDK_SYSTEMUTILAPI_H_
#pragma once

#include <WdkVersion.h>
#include <WdkCoreDef.h>
#include <WdkMacros.h>

static const FILETIME DHCP_FT_PERM = {0xFFFFFFFF, 0xFFFFFFFF}; ///< The value used for permenant dhcp expire times.

/// \brief structure that contains the basic info of a dhcp record.
typedef struct _WDK_DHCP_INFO_
{
    DWORD   dwSize;                             ///< set to sizeof(WDK_DHCP_INFO) before calling get/set
    WCHAR   wzIpAddress[MAX_IPADDR_STRING_LEN];     ///< ip address of the dhcp client
    WCHAR   wzDeviceName[MAX_HOSTNAME_LEN];     ///< device name of the dhcp client
    BYTE    bMacAddress[MAC_LEN];      ///< hardware address of the dhcp client.
    FILETIME ftExpires;                         ///< time this client expires
} WDK_DHCP_INFO, *PWDK_DHCP_INFO; ///< pointer to a _WDK_DHCP_INFO_

/// \brief structure that contains the summary information of the ICS settings.
typedef struct _WDK_ICS_CONFIG_
{
    DWORD dwSize;                                   ///< set to sizeof(WdkIcsConfig) before calling get/set
    
    WCHAR wzPrivateIntfName[MAX_ADAPTER_NAME_LEN];   ///< wstring containing the private adapter name
    WCHAR wzPrivateIp[MAX_IPADDR_STRING_LEN];           ///< wstring containing the ip address of the private adapter
    WCHAR wzPrivateMask[MAX_IPADDR_STRING_LEN];         ///< wstring containing the subnet mask of the private adapter

    WCHAR wzPublicIntfName[MAX_ADAPTER_NAME_LEN];    ///< wstring containing the public adapter name (REG_MULTI_SZ)
    DWORD cchPublicIntfName;                        ///< DWORD length of the public interface multi string (including double zero terminator)
    WCHAR wzPublicIp[MAX_IPADDR_STRING_LEN];            ///< wstring containing the configured public ip address (not live)
    WCHAR wzPublicMask[MAX_IPADDR_STRING_LEN];          ///< wstring containing the configured public subnet mask (not live)

    WCHAR wzExposedHost[MAX_IPADDR_STRING_LEN];         ///< wstring containing the ip address of the dmz
    CHAR  szHostname[MAX_HOSTNAME_LEN];             ///< string containing the current hostname

    BOOL bDhcpEnabled;                              ///< BOOL containing DHCP allocator enabled state
    DWORD dwDhcpMin;                                ///< DWORD with the minimum allowed dhcp octet
    DWORD dwDhcpMax;                                ///< DWORD with the maximum allowed dhcp octet
    DWORD dwDhcpLeaseTime;                          ///< DWORD with the DHCP lease time in minutes

} WdkIcsConfig, *PWdkIcsConfig; ///< pointer to a _WDK_ICS_CONFIG_

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Reads the current configuration of the Internet Connection Sharing
///
/// Reads the ICS settings from the registry and compiles them into a single
/// structure for ease of use.  You must init the dwSize member of the struct
/// pointer that is passed in to this api.
///
/// \param pInfo pointer to a WdkIcsConfig struct to receive the settings
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Ics_GetInfo( __out PWdkIcsConfig pInfo );

/// \brief Writes the specified Internet Connection Sharing sharing to the registry
///
/// Writes the ICS settings given to the registry.  You must provide valid settings
/// for all members of the pInfo struct including the dwSize member.
///
/// \param pInfo pointer to a WdkIcsConfig struct containing the new ICS settings.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Ics_SetInfo( __in PWdkIcsConfig pInfo );

/// \brief Reads the system dhcp expire time out of the registry.
///
/// \param pFileTime pointer to a FILETIME structure to receive the value
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Dhcp_GetSystemExpires( __out FILETIME *pFileTime );

/// \brief Deletes an address mapping (DHCP record) from the system
///
/// \param wzAddress wstring specifying the ip address of the mapping to remove
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Dhcp_RemoveAddressMap( __in LPCWSTR wzAddress );

/// \brief Reads an address mapping (DHCP record)
///
/// \param wzAddress wstring specifying the ip address of the mapping to lookup
/// \param pInfo pointer to a WDK_DHCP_INFO structure to receive the mapping info
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Dhcp_GetAddressMap( __in LPCWSTR wzAddress, __out PWDK_DHCP_INFO pInfo );

/// \brief Writes an address mapping (DHCP record) to the registry
///
/// \param pDhcpList pointer to a WDK_DHCP_INFO structure to receive the mapping info
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Dhcp_SetAddressMap( __in PWDK_DHCP_INFO pDhcpList );

/// \brief  Reads all the address mappings from the registry
///
/// \param  ppDhcpList pointer to an array of WDK_DHCP_INFO structures.  This will get
///         created for you.  You should delete[] this memory when you are finished with it.
/// \param  pdwDhcpCount pointer to a DWORD to receive the count of structures created
///         in the pDhcpList parameter.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Dhcp_GetAddressList( __out PWDK_DHCP_INFO *ppDhcpList, __out PDWORD pdwDhcpCount );

/// \brief Removes all the address mappings from the registry
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_Dhcp_ClearAddressList( void );

/// \brief Rebinds an ndis adapter.
///
/// Immediately rebinds the specified ndis adapter.
///
/// \param pwzAdapter wstring specifying the name of the adapter to be rebound.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_System_RebindAdapter( __in LPCWSTR pwzAdapter );

/// \brief Unbinds an ndis adapter.
///
/// Immediately unbinds the specified ndis adapter.
///
/// \param pwzAdapter pwstring specifying the name of the adapter to be unbound.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_System_UnbindAdapter( __in LPCWSTR pwzAdapter );

/// \brief Binds an ndis adapter.
///
/// Immediately binds the specified ndis adapter.
///
/// \param pwzAdapter wstring specifying the name of the adapter to be bound.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_System_BindAdapter( __in LPCWSTR pwzAdapter );

/// \brief Retrieves the current bindings for an NDIS adapter.
///
/// \param pwzAdapter wstring specifying the name of the adapter to be bound.
/// \param pwzBindings multi-string list of bindings.
/// \param cchBindings length of the buffer in characters.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
HRESULT WDK_System_GetAdapterBindings(__in LPCWSTR pwzAdapter, __out LPWSTR pwzBindings, DWORD cchBindings );

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif // _WDK_SYSTEMUTILAPI_H_
