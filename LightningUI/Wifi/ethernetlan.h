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
#include <iphlpapi.h>
#include <pm.h>
#include <winsock.h>
#include <wlanapi.h>
#include <ndispwr.h>

typedef struct _NETWORK_ADPT_INFO {
    LPTSTR    szAdapterName;
    BOOL    fUseDHCP;
    BOOL    fSaved;
    DWORD    IPAddr;
    DWORD    SubnetMask;
    DWORD    Gateway;
    DWORD    DNSAddr;
    DWORD    DNSAltAddr;
    DWORD    WINSAddr;
    DWORD    WINSAltAddr;
    TCHAR    szDisplayName[256];
} NETWORK_ADPT_INFO, *PNETWORK_ADPT_INFO;


class EthernetLan
{
protected:
    static const TCHAR m_cszNameMappingRegistryPath[];
    static const TCHAR m_cszWirelessCacheRegistryPath[];

    // These are used to enum the adapters in the system
    // pAdapterInfo is refreshed by a call to FindFirst
    static TCHAR m_szAdapterNames[512];
    static TCHAR * m_pCurrentAdapter;

    TCHAR * m_pszLanName;
    TCHAR * m_pszAdapter;
    BOOL m_fEnabled;
    BOOL m_fSupportsPowerManagment;
    BOOL m_fWireless;

    DWORD m_dwAddress;
    DWORD m_dwGatewayAddress;

    NETWORK_ADPT_INFO m_OldAdptInfo;

    static BOOL DoNdisIOControl(DWORD dwCommand, LPVOID pInBuffer,
                               DWORD cbInBuffer, LPVOID pOutBuffer,
                               DWORD * pcbOutBuffer);

    static BOOL IsValidDx(CEDEVICE_POWER_STATE Dx) { return (PwrDeviceUnspecified < Dx &&
                                                            PwrDeviceMaximum > Dx); }

    BOOL isWireless() const;
    BOOL readFriendlyName();

    static DWORD GetIfAddressFromIfIndex(DWORD dwIndex);
    BOOL GetDefaultGatewayRoute(MIB_IPFORWARDROW & ipRoute) const;

public:
    EthernetLan(TCHAR * pszName);
    virtual ~EthernetLan();

    TCHAR * GetLanName();
    BOOL IsValidLanName(TCHAR * pszValidate);
    virtual BOOL isType(TCHAR * pszType) const { return !::_tcscmp(pszType, _T("EthernetLan")); }
    virtual BOOL isValid(UINT * pError);

    static DWORD IsRasVEMAdapter(IN const WCHAR *wszAdapterName, OUT BOOL *bIsVEMAdapter);

    // Allows enuming of the adapters
    static EthernetLan * FindFirst();
    static EthernetLan * FindNext();

    // Used by AdapterOrder to order the LAN connections
    DWORD GetDefaultGatewayRouteMetric() const;
    BOOL SetDefaultGatewayRouteMetric(DWORD dwRoute);

    BOOL GetAdapterIPProperties (PNETWORK_ADPT_INFO pAdptInfo);
    BOOL SetAdapterIPProperties (PNETWORK_ADPT_INFO pAdptInfo);

    void DoNdisMultiSzIOControl( DWORD dwCommand, ...);
    void RestartAdapter(TCHAR *szAdapterName);
};

#define net_long(x) (((((ulong)(x))&0xffL)<<24) | \
                     ((((ulong)(x))&0xff00L)<<8) | \
                     ((((ulong)(x))&0xff0000L)>>8) | \
                     ((((ulong)(x))&0xff000000L)>>24))

void IPAddrToStr (LPTSTR szStr, DWORD IPAddr);
BOOL StringToAddr(TCHAR *AddressString, DWORD *AddressValue);
VOID GetMultiSZAddr (HKEY hKey, LPTSTR szValueName, LPDWORD pAddr1, LPDWORD pAddr2);

