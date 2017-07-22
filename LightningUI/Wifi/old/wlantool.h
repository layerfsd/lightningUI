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
//     wlantool.h
//
// Abstract:
//
//     Program to control ACM (Auto Config Module) service to configure WiFi.
//
// ----------------------------------------------------------------------------

#ifndef _WLAN_TOOL_H
#define _WLAN_TOOL_H

#pragma once

// BUGBUG - Remove unnecessary Headers
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include <Eaphostpeerconfigapis.h>
#include <wlanapi.h>

#include <rpcdce.h>
#include <rpc.h>
#include <atlbase.h>
#include <msxml.h>

// ========================================================================== //
// Macros:
// ========================================================================== //

// -----------------------------------------------------------------------------
LPWSTR
FormatSsid
// Formats the specified SSID.
(
 IN  const DOT11_SSID &Ssid,
 __out_ecount(BufferChars) OUT LPWSTR           pBuffer,
 IN  int              BufferChars
 );
LPWSTR
FormatSsid
// Formats the specified raw SSID.
(
 IN  const PWLAN_RAW_DATA prdSsid,
 __out_ecount(BufferChars) OUT LPWSTR               pBuffer,
 IN  int                  BufferChars
 );

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
 );

// -----------------------------------------------------------------------------
// Class for Managing Interfaces
class WlanInterfaces
{
public:
	WlanInterfaces(WLAN_NOTIFICATION_CALLBACK lpfnCallback=NULL,LPVOID lpfnContext=NULL);
	~WlanInterfaces();
	BOOL EnumWirelessNetworkCards();
	BOOL GetInterfaceList(OUT PWLAN_INTERFACE_INFO_LIST* ppInterfaceList);
	BOOL EnumInfo(IN CONST LPCWSTR strAdapter);
	BOOL ListNetworks(IN CONST LPCWSTR strAdapter,IN BOOL bPrint=FALSE); 
	BOOL GetNetworksList(IN LPCWSTR pAdapter, OUT PWLAN_AVAILABLE_NETWORK_LIST* ppNetworksList);
	BOOL Scan(IN CONST LPCWSTR strAdapter,IN CONST PDOT11_SSID pDot11Ssid);        
	BOOL Connect(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strProfile);
	BOOL Disconnect(IN CONST LPCWSTR strAdapter);                        
	BOOL ListBss(IN CONST LPCWSTR strAdapter,IN CONST PDOT11_SSID pDot11Ssid);  
	BOOL GetFilterList(IN CONST LPCWSTR strAdapter,IN CONST WLAN_FILTER_LIST_TYPE fType);  
	BOOL SetProfile(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strfName,IN LPCWSTR strPrfXml = NULL);
	BOOL ListProfile(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strProfileName);        
	BOOL DeleteProfile(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strProfileName);                
	BOOL SaveTempProfile(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strProfileName);                        
	BOOL RenameProfile(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strProfileName,IN CONST LPCWSTR strNewName);                        
	BOOL SetProfilePos(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strProfileName,IN CONST DWORD dwOrder);       
	BOOL SetInterface(IN CONST LPCWSTR strAdapter,IN CONST WLAN_INTF_OPCODE opCode,IN CONST DWORD dwSize,IN CONST PVOID pData);        
	BOOL SetFilterList(IN CONST LPCWSTR strAdapter,IN CONST WLAN_FILTER_LIST_TYPE lType,IN CONST PDOT11_NETWORK_LIST pList);
	BOOL ListProfileList(IN CONST LPCWSTR strAdapter);           
	BOOL SetProfileList(IN CONST LPCWSTR strAdapter,IN LPCWSTR* strList,IN CONST DWORD dwNumOfProfiles);
	BOOL QueryAutoconfigParam(IN CONST LPCWSTR strAdapter);
	BOOL SetAutoconfigParam(IN CONST LPCWSTR strAdapter,IN CONST WLAN_AUTOCONF_OPCODE opCode,IN CONST BOOL bFlag);
	BOOL SetKey(IN CONST LPCWSTR strAdapter,IN CONST LPCWSTR strSsidName,IN CONST LPCWSTR strKey);

private:
	BOOL Init();
	void Cleanup();
	void QueryBoolFromInterface(IN CONST HANDLE             hClientHandle, 
		IN CONST GUID*              pGuid, 
		IN CONST WLAN_INTF_OPCODE   opcode, 
		OUT PDWORD                  pdwDataSize, 
		OUT PVOID*                  ppData, 
		OUT PWLAN_OPCODE_VALUE_TYPE pOpcodeType);
	DWORD PrintMacFrameStatistics(IN CONST PWLAN_MAC_FRAME_STATISTICS pStats);
	void PrintNetworkList(IN CONST PDOT11_NETWORK_LIST pNetworkList);
	void PrintAvailableNetworkList(IN CONST PWLAN_AVAILABLE_NETWORK_LIST pNetworkList);        
	BOOL GetInterface(IN CONST LPCWSTR pAdapter,OUT PWLAN_INTERFACE_INFO* ppInt);
	static const DWORD dwClientVersion = 2;
	PDWORD pdwNegotiatedVersion;
	PHANDLE phClientHandle;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList;
	PWLAN_AVAILABLE_NETWORK_LIST pNetworksList;
	WLAN_NOTIFICATION_CALLBACK m_lpfnCallback;
	LPVOID		m_lpfnContext;
	BOOL				m_bIni;
};

//tools
VOID
GetSsidFromString
// Gets the Ssid from the command Line arguments using the arg Value
(
 IN  LPCWSTR      strSsid,   // Ssid String
 IN  PDOT11_SSID  pDot11Ssid // Pointer to Ssid object
 );

// -----------------------------------------------------------------------------

#endif  // _WLAN_TOOL_H
