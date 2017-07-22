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

#pragma once
#include <stdlib.h>
#include <string.hxx>
#include "winuserm.h"
// #include "miscsvcs.h"
#include "memory.h"
#include "raserror.h"
#include "l2tpcfg.h" 
#include "ras.h"
#include "eaputil.h"


DWORD AllocRasEnumDevices(
    __out LPRASDEVINFO *ppRasDevInfo,
    __out DWORD        *pcbRasDevInfo,
    __out DWORD        *pcRasDevices);

DWORD GetDevices(
    __out LPRASDEVINFO *ppRasDevInfo);

HRESULT EthernetGetRasDialParams(LPCTSTR szEntryName, 
    RASDIALPARAMS *pRasDialParams, 
    BOOL *pbGotPassword);


// Get the device specific structure from out of the phonebook. 
// This is of varying size and content according to the kind of RAS connection.
// For L2TP-IPSec VPNs this will be a L2TP_CONFIG_DATA. 
HRESULT EthernetGetRasEntryProperties(LPCTSTR szEntryName,
                     RASENTRY* pRasEntry,
                     PBYTE& pDevConfig,
                     DWORD& cbDevConfig);


HRASCONN GetRasConnectionHandle(__in WCHAR * pszName);

BOOL IsValidPhoneNumberString(WCHAR * pszValidate);

BOOL IsValidVPNHostString(WCHAR * wszValidate);

DWORD EthernetEapEnumExtensions(PEAP_EXTENSION_INFO& pEapExtInfo, DWORD& dwNumExtensions);

void CALLBACK NullLineCallbackFunc(DWORD dwDevice, DWORD dwMsg,
                                                DWORD dwCallbackInstance,
                                                DWORD dwParam1, DWORD dwParam2,
                                                DWORD dwParam3);

LPLINETRANSLATECAPS GetTranslateCaps(HINSTANCE hInstance);

DWORD GetDefaultCntryRgnCode(LPLINETRANSLATECAPS pCaps);

void GetDefaultAreaCode(LPLINETRANSLATECAPS pCaps, TCHAR * pszAreaCode, DWORD dwMaxLen);

void IPAddrToStr (LPTSTR szStr, DWORD IPAddr);
BOOL StringToAddr(TCHAR *AddressString, DWORD *AddressValue);
VOID GetMultiSZAddr (HKEY hKey, LPTSTR szValueName, LPDWORD pAddr1, LPDWORD pAddr2);
HRESULT RegSetDefaultDialSettings(void);

bool HasDevices(TCHAR * pszDeviceType, TCHAR *pszDeviceNamePrefix = NULL);

DWORD RegGetDwordValue(
    IN  HKEY        hKey,
    IN  PWSTR        wszSubKeyName,
    IN  PWSTR        wszValueName,
    OUT PDWORD      pdwValue);

