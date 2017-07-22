//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#include "ethernetlan.h"
#include <nuiouser.h>
#include <ras.h>
#include <raserror.h>
#include <Guts.h>

#define MAX_DATA_SIZE 1024
#define LANNAME_LENGTH  512
#define MAX_FRIENDLYNAME_LENGTH 128
#define SAFE_REGCLOSEKEY(x){if(x){RegCloseKey(x);x=NULL;}}

const TCHAR g_szBadNameCharList[] = { _T('\\'), _T('/'),
                                              _T('*'), _T('?'), _T('\"'),
                                              _T('<'), _T('>'), _T('|') };


//void ValidateName(TCHAR * pszNewName, UINT * pError);

////////////////////////
// EthernetLan
////////////////////////

const TCHAR EthernetLan::m_cszNameMappingRegistryPath[] = _T("Comm\\AdapterNameMappings");
const TCHAR EthernetLan::m_cszWirelessCacheRegistryPath[] = _T("Comm\\WirelessAdapterCache");

TCHAR EthernetLan::m_szAdapterNames[] = {0};
TCHAR * EthernetLan::m_pCurrentAdapter = NULL;

EthernetLan::EthernetLan(TCHAR * pszName) :
   m_pszLanName(NULL),
   m_dwAddress(0),
   m_dwGatewayAddress(0),
   m_fEnabled(FALSE)
{
   if (!pszName)
      return;

   // Determine if we are a valid lan type
   TCHAR szAdapterBindings[LANNAME_LENGTH] = {0};
   DWORD cbAdapterBindings = sizeof(szAdapterBindings);
   if (DoNdisIOControl(IOCTL_NDIS_GET_ADAPTER_BINDINGS,
                       pszName, (_tcslen(pszName)+1)*sizeof(TCHAR),
                       szAdapterBindings, &cbAdapterBindings))
   {
      // Loop through all of the bindings for this adapter looking for tcp/ip
      TCHAR * pCurrentBinding = szAdapterBindings;
      while (_T('\0') != *pCurrentBinding)
      {
         if (!::_tcsicmp(pCurrentBinding, _T("TCPIP")) ||
             !::_tcsicmp(pCurrentBinding, _T("MSTCP")))
         {
            m_fEnabled = TRUE;
            break;
         }
         pCurrentBinding += ::_tcslen(pCurrentBinding)+1;
      }
   }

   // Set the internal name
   m_pszAdapter = new TCHAR[LANNAME_LENGTH];
   if (m_pszAdapter)
      _tcscpy_s(m_pszAdapter, LANNAME_LENGTH, pszName);
   else
      return;
  
   // Get the friendly name from the registry
   if (!readFriendlyName())
   {
      m_pszLanName = new TCHAR[LANNAME_LENGTH];
      if (m_pszLanName)
      {
         _tcscpy_s(m_pszLanName, LANNAME_LENGTH, m_pszAdapter);

         // Replace all '\' in the name
         TCHAR * pch = ::_tcschr(m_pszLanName, _T('\\'));
         while (pch)
         {
            *pch = _T('-'); // Replace the '\'
            pch = ::_tcsrchr(pch, _T('\\'));
         }
      }
   }

   // Determine if we support power managment  
   CEDEVICE_POWER_STATE Dx = PwrDeviceUnspecified;
   TCHAR                szName[MAX_PATH];
   int                  nChars;

//    nChars = _sntprintf_s(
   nChars = swprintf_s(
                szName, 
                MAX_PATH-1, 
                _T("%s\\%s"), 
                PMCLASS_NDIS_MINIPORT, 
                m_pszAdapter);
   
    szName[MAX_PATH-1]=0;
    
    if(nChars != (-1)) 
       ::GetDevicePower(szName, POWER_NAME, &Dx);

   m_fSupportsPowerManagment = IsValidDx(Dx);

   // Grab the ip address info
   if (m_fEnabled)
   {
      PIP_ADAPTER_INFO pAdapterInfo = NULL;
      PIP_ADAPTER_INFO pCurrentAdapterInfo = NULL;
      DWORD cbAdapterInfo = 0;
      if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(pAdapterInfo, &cbAdapterInfo))
      {
         pAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(new BYTE[cbAdapterInfo]);
         ASSERT(pAdapterInfo);
         if (pAdapterInfo)
         {
            if (ERROR_SUCCESS == GetAdaptersInfo(pAdapterInfo, &cbAdapterInfo) && cbAdapterInfo != 0)
            {
               CHAR szAdapter[256];
#ifdef UNICODE
               size_t returnValue = 0;
               wcstombs_s(&returnValue, szAdapter, 256, m_pszAdapter, _tcslen(m_pszAdapter)+1);
#else
               _tcsncpy_s(szAdapter, 256, m_pszAdapter, _tcslen(m_pszAdapter)+1);
#endif
               pCurrentAdapterInfo = pAdapterInfo;
               while (pCurrentAdapterInfo)
               {
                  if (!::strcmp(szAdapter, pCurrentAdapterInfo->AdapterName))
                  {
                     m_dwAddress = ::inet_addr(pCurrentAdapterInfo->IpAddressList.IpAddress.String);
                     m_dwGatewayAddress = ::inet_addr(pCurrentAdapterInfo->GatewayList.IpAddress.String);
                     break;
                  }
                  pCurrentAdapterInfo = pCurrentAdapterInfo->Next;
               }
            }
            delete [] pAdapterInfo;
            pAdapterInfo = NULL;
         }
      }
   }

   // Determine wireless attribute
   m_fWireless = isWireless();
}


EthernetLan::~EthernetLan()
{
   if (m_pszAdapter)
      delete [] m_pszAdapter;

   if (m_pszLanName)
    {
        delete [] m_pszLanName;
        m_pszLanName = NULL;
    }
}

EthernetLan * EthernetLan::FindFirst()
{
   DWORD cbAdapterNames = sizeof(m_szAdapterNames);
   if (DoNdisIOControl(IOCTL_NDIS_GET_ADAPTER_NAMES, NULL, 0,
                       &m_szAdapterNames, &cbAdapterNames))
   {
      m_pCurrentAdapter = m_szAdapterNames;
      return FindNext();
   }
   return NULL;
}

EthernetLan * EthernetLan::FindNext()
{
   EthernetLan * pEthernetLan = NULL;
   BOOL Filter = FALSE;

   // Need to call FindFirst first
   if (!m_pCurrentAdapter)
      return NULL;

   // Find the next LAN adapter
   while ((_T('\0') != *m_pCurrentAdapter) && !pEthernetLan)
   {
      pEthernetLan = new EthernetLan(m_pCurrentAdapter);
      if (pEthernetLan)
      {
          if (pEthernetLan->isValid(NULL))
          {
              // Filter out RAS connections
              IsRasVEMAdapter(pEthernetLan->m_pszAdapter, &Filter);
          }else{
              Filter = TRUE;
          }

          if (Filter)
          {
              Filter = FALSE;
              delete pEthernetLan;
              pEthernetLan = NULL;
          }
      }
     
      m_pCurrentAdapter += ::_tcslen(m_pCurrentAdapter)+1;
   }

   // Reset pCurrentAdapter on a failure or end of list
   if (!pEthernetLan)
      m_pCurrentAdapter = NULL;

   return pEthernetLan;
}

TCHAR * EthernetLan::GetLanName()
{
    return m_pszLanName;
}

BOOL EthernetLan::IsValidLanName(TCHAR * pszValidate)
{
   if (!pszValidate)
      return FALSE;

   for (int i = 0; i < (_countof(g_szBadNameCharList)); i++)
   {
      if (::_tcschr(pszValidate, g_szBadNameCharList[i]))
         return FALSE;
   }

   return TRUE;
}

BOOL EthernetLan::isValid(UINT * pError)
{
    if (!m_pszAdapter)
    {
        return FALSE;
    }
    
    
    // Do error checking here
    ASSERT(IsValidLanName(m_pszLanName));
    if (!IsValidLanName(m_pszLanName))
    {
        return FALSE;
    }
    
    
    // If the adapter is not enabled and it is bound that means it is
    // not a valid LAN connectoid
    TCHAR szAdapterBindings[512] = {0};
    DWORD cbAdapterBindings = sizeof(szAdapterBindings);
    if (DoNdisIOControl(IOCTL_NDIS_GET_ADAPTER_BINDINGS,
        m_pszLanName, (_tcslen(m_pszAdapter)+1)*sizeof(TCHAR),
        szAdapterBindings, &cbAdapterBindings))
    {
        if (!m_fEnabled && *szAdapterBindings)
        {
            return FALSE;
        }
    }
    
    if (m_fSupportsPowerManagment)
    {     
        CEDEVICE_POWER_STATE Dx = PwrDeviceUnspecified;     
        TCHAR                szName[MAX_PATH];
        int                  nChars;
        
//         nChars = _sntprintf_s(
		nChars = swprintf_s(
            szName, 
            MAX_PATH-1, 
            _T("%s\\%s"), 
            PMCLASS_NDIS_MINIPORT, 
            m_pszAdapter);
        
        szName[MAX_PATH-1]=0;
        
        if(nChars != (-1)) 
            ::GetDevicePower(szName, POWER_NAME, &Dx);
        
        
        if (!IsValidDx(Dx))
        {
            return FALSE;
        }
        
        // if we are in power state D0 (Full Power) and not enabled then we 
        // have been unbound by something other than CONNMC and so we shouldn't
        // display the icon.  If Disabled by CONNMC we should be in state D4 (No power)
        if ((D0 == Dx) && !m_fEnabled)
        {
            return FALSE;
        }                
    }   
    return TRUE;
}


BOOL EthernetLan::DoNdisIOControl(DWORD dwCommand, LPVOID pInBuffer,
                                  DWORD cbInBuffer, LPVOID pOutBuffer,
                                  DWORD * pcbOutBuffer)
{
   HANDLE hNdis;
   BOOL fResult = FALSE;

   hNdis = ::CreateFile(DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
                        0, NULL);

   if (INVALID_HANDLE_VALUE != hNdis)
   {
      fResult = ::DeviceIoControl(hNdis, dwCommand, pInBuffer, cbInBuffer,
                                  pOutBuffer, (pcbOutBuffer ? *pcbOutBuffer : 0),
                                  pcbOutBuffer, NULL);
      ::CloseHandle(hNdis);
   }

   return fResult;
}

BOOL EthernetLan::isWireless() const
{
   BOOL fSuccess = FALSE;

   if (!m_pszAdapter)
      return FALSE;

   if (m_fEnabled)
   {
       typedef DWORD (WINAPI *PFN_WlanOpenHandle)(DWORD dwClientVersion,PVOID pReserved,PDWORD pdwNegotiatedVersion,PHANDLE phClientHandle);
       typedef DWORD (WINAPI *PFN_WlanCloseHandle)(HANDLE hClientHandle,PVOID pReserved);
       typedef DWORD (WINAPI *PFN_WlanEnumInterfaces)(HANDLE hClientHandle,PVOID pReserved,PWLAN_INTERFACE_INFO_LIST *ppInterfaceList);
       typedef VOID (WINAPI *PFN_WlanFreeMemory)(PVOID pMemory);

       HINSTANCE hNWLib = NULL;
       HANDLE    hSession = NULL;
       DWORD     dwError = ERROR_SUCCESS;
       DWORD     ActualVersion = 0;
       PWLAN_INTERFACE_INFO_LIST pInterfaces = NULL;
       PFN_WlanOpenHandle pfnWlanOpenHandle = NULL;
       PFN_WlanCloseHandle pfnWlanCloseHandle = NULL;
       PFN_WlanEnumInterfaces pfnWlanEnumInterfaces = NULL;
       PFN_WlanFreeMemory pfnWlanFreeMemory = NULL;

       // See if Native Wifi API present in system
       if ((hNWLib = LoadLibrary(L"wlanapi.dll")) == NULL) {
           return FALSE;
       }

       pfnWlanOpenHandle = (PFN_WlanOpenHandle)GetProcAddress(hNWLib, L"WlanOpenHandle");
       pfnWlanCloseHandle = (PFN_WlanCloseHandle)GetProcAddress(hNWLib, L"WlanCloseHandle");
       pfnWlanEnumInterfaces = (PFN_WlanEnumInterfaces)GetProcAddress(hNWLib, L"WlanEnumInterfaces");
       pfnWlanFreeMemory = (PFN_WlanFreeMemory)GetProcAddress(hNWLib, L"WlanFreeMemory");

       if (!pfnWlanOpenHandle || !pfnWlanCloseHandle || !pfnWlanEnumInterfaces || !pfnWlanFreeMemory)
       {
           fSuccess = FALSE;
           goto CleanupWlan;
       }

       dwError = pfnWlanOpenHandle(2/*ExpectedVersion*/, NULL/*reserved*/, &ActualVersion, &hSession);
       if (dwError != ERROR_SUCCESS)
       {
           fSuccess = FALSE;
           goto CleanupWlan;
       }

       dwError = pfnWlanEnumInterfaces(hSession, NULL/*reserved*/, &pInterfaces);
       if (dwError != ERROR_SUCCESS)
       {
           fSuccess = FALSE;
           goto CleanupWlan;
       }

       dwError = ERROR_NOT_FOUND;
       for (;pInterfaces->dwIndex < pInterfaces->dwNumberOfItems; pInterfaces->dwIndex++)
       {
           PWLAN_INTERFACE_INFO pInt = &pInterfaces->InterfaceInfo[pInterfaces->dwIndex];
           if (wcscmp(pInt->strInterfaceDescription, m_pszAdapter) == 0)
           {
               dwError = ERROR_SUCCESS;
               break;
           }
       }

       if (dwError != ERROR_SUCCESS)
       {
           fSuccess = FALSE;
           goto CleanupWlan;
       }

       fSuccess = TRUE;

       // Write out our wireless property
       HKEY hKey = NULL;
       if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_CURRENT_USER,
           m_cszWirelessCacheRegistryPath,
           NULL, NULL, 0, KEY_WRITE,
           NULL, &hKey, NULL))
       {
           DWORD dwSet = 1;
           ::RegSetValueEx(hKey, m_pszAdapter, NULL, REG_DWORD,
               reinterpret_cast<LPBYTE>(&dwSet), sizeof(dwSet));
           ::RegCloseKey(hKey);
       }

CleanupWlan:
       if (hSession && pfnWlanCloseHandle)
       {
           pfnWlanCloseHandle(hSession, NULL);
       }

       if (pInterfaces && pfnWlanFreeMemory)
       {
           pfnWlanFreeMemory(pInterfaces);
       }

       FreeLibrary(hNWLib);
   }
   else
   {
      HKEY hKey = NULL;
      if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CURRENT_USER,
                                          m_cszWirelessCacheRegistryPath,
                                          NULL, KEY_READ, &hKey))
      {
         DWORD dwSet = 0;
         DWORD cbSet = sizeof(dwSet);
         if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, m_pszAdapter, NULL, NULL,
                                                reinterpret_cast<LPBYTE>(&dwSet),
                                                &cbSet))
         {
            fSuccess = static_cast<BOOL>(dwSet);
         }
         ::RegCloseKey(hKey);
      }
   }

   return fSuccess;
}

BOOL EthernetLan::readFriendlyName()
{
    HKEY hKey = NULL;
    TCHAR szName[MAX_FRIENDLYNAME_LENGTH] = {0};
    DWORD dwName = _countof(szName);

    if (!m_pszAdapter)
          return FALSE;

    if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_CURRENT_USER, m_cszNameMappingRegistryPath,
                                       NULL, KEY_READ, &hKey))
    {
          return FALSE;
    }

    if (ERROR_SUCCESS != ::RegQueryValueEx(hKey, m_pszAdapter, NULL, NULL,
                                          reinterpret_cast<LPBYTE>(szName),
                                          &dwName))
    {
        ::RegCloseKey(hKey);
        return FALSE;
    }
    ::RegCloseKey(hKey);

    if (!IsValidLanName(szName))
        return FALSE;

    if (m_pszLanName)
    {
        delete [] m_pszLanName;
        m_pszLanName = NULL;
    }

    m_pszLanName = new TCHAR[::_tcslen(szName)+1];
    if (!m_pszLanName)
          return FALSE;

    _tcscpy_s(m_pszLanName, LANNAME_LENGTH, szName);
    return TRUE;
}

DWORD EthernetLan::GetIfAddressFromIfIndex(DWORD dwIndex)
{
   DWORD dwError = NO_ERROR;
   PMIB_IPADDRTABLE pTables = NULL;
   DWORD cbTables = 0;
   DWORD dwIfAddress = static_cast<DWORD>(-1);

   dwError = ::GetIpAddrTable(pTables, &cbTables, FALSE);
   if ((ERROR_INSUFFICIENT_BUFFER == dwError) || (ERROR_BUFFER_OVERFLOW == dwError))
   {
      pTables = reinterpret_cast<PMIB_IPADDRTABLE>(new BYTE[cbTables]);
      ASSERT(pTables);
      if (pTables)
      {
         if (NO_ERROR == ::GetIpAddrTable(pTables, &cbTables, FALSE))
         {
            for (DWORD i = 0; i < pTables->dwNumEntries; i++)
            {
               if (pTables->table[i].dwIndex == dwIndex)
               {
                  dwIfAddress = pTables->table[i].dwAddr;
                  break;
               }
            }
         }
         delete [] pTables;
      }
   }

   return dwIfAddress;
}

BOOL EthernetLan::GetDefaultGatewayRoute(MIB_IPFORWARDROW & ipRoute) const
{
   DWORD dwError = NO_ERROR;
   PMIB_IPFORWARDTABLE pTables = NULL;
   DWORD cbTables = 0;
   BOOL fSuccess = FALSE;

   dwError = ::GetIpForwardTable(pTables, &cbTables, FALSE);
   if ((ERROR_INSUFFICIENT_BUFFER == dwError) || (ERROR_BUFFER_OVERFLOW == dwError))
   {
      pTables = reinterpret_cast<PMIB_IPFORWARDTABLE>(new BYTE[cbTables]);
      ASSERT(pTables);
      if (pTables)
      {
         if (NO_ERROR == ::GetIpForwardTable(pTables, &cbTables, FALSE))
         {
            for (DWORD i = 0; i < pTables->dwNumEntries; i++)
            {
               if ((GetIfAddressFromIfIndex(pTables->table[i].dwForwardIfIndex) == m_dwAddress) &&
                   (pTables->table[i].dwForwardNextHop == m_dwGatewayAddress))
               {
                  ipRoute = pTables->table[i];
                  fSuccess = TRUE;
                  break;
               }
            }
         }
         delete [] pTables;
      }
   }

   return fSuccess;
}

DWORD EthernetLan::GetDefaultGatewayRouteMetric() const
{
   MIB_IPFORWARDROW ipRoute = {0};
   DWORD dwMetric = static_cast<DWORD>(-1);

   if (GetDefaultGatewayRoute(ipRoute))
   {
      dwMetric = ipRoute.dwForwardMetric1;
   }
   else
   {
      HKEY hKey = NULL;
      TCHAR szMetricRegistryPath[MAX_PATH];
      StringCchPrintf(szMetricRegistryPath, MAX_PATH, _T("Comm\\%s\\Parms\\TCPIP"), m_pszAdapter);
      if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                          szMetricRegistryPath,
                                          NULL, KEY_READ, &hKey))
      {
         DWORD cbMetric = sizeof(dwMetric);
         if (ERROR_SUCCESS != ::RegQueryValueEx(hKey, _T("InterfaceMetric"), NULL, NULL,
                                                reinterpret_cast<LPBYTE>(&dwMetric),
                                                &cbMetric))
         {
            dwMetric = static_cast<DWORD>(-1);
         }
         ::RegCloseKey(hKey);
      }
   }

   return dwMetric;
}

BOOL EthernetLan::SetDefaultGatewayRouteMetric(DWORD dwMetric)
{
   MIB_IPFORWARDROW ipRoute = {0};
   DWORD dwSuccess = NO_ERROR;

   if (GetDefaultGatewayRoute(ipRoute))
   {
      ipRoute.dwForwardMetric1 = dwMetric;
      dwSuccess = ::SetIpForwardEntry(&ipRoute);
   }

   HKEY hKey = NULL;
   TCHAR szMetricRegistryPath[MAX_PATH];
   StringCchPrintf(szMetricRegistryPath, MAX_PATH, _T("Comm\\%s\\Parms\\TCPIP"), m_pszAdapter);
   if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, szMetricRegistryPath,
                                         NULL, NULL, 0, KEY_WRITE, NULL, &hKey, NULL))
   {
      ::RegSetValueEx(hKey, _T("InterfaceMetric"), NULL, REG_DWORD,
                      reinterpret_cast<LPBYTE>(&dwMetric),  sizeof(dwMetric));
      ::RegCloseKey(hKey);
   }

   return (NO_ERROR == dwSuccess);
}


DWORD
EthernetLan::IsRasVEMAdapter(
    IN const WCHAR *wszAdapterName,
    OUT BOOL       *bIsVEMAdapter)
//
//  Set *bIsVEMAdapter to TRUE if the adapter name is that of a VEM interface created
//  to represent a RAS connection.
//
//  Return ERROR_SUCCESS if the function completes successfully, otherwise an error code.
{
    BOOL bFoundMatchingRasDeviceName    = FALSE;
    LPRASDEVINFO pRasDevInfo            = NULL;
    DWORD        cbRasDevInfo            = 0;
    DWORD        cRasDevices            = 0;
    DWORD        dwResult                = ERROR_SUCCESS;

    //
    // Get a list of RAS device names
    //
    cbRasDevInfo = sizeof(RASDEVINFO);
    do
    {
        LocalFree(pRasDevInfo);
        pRasDevInfo = (RASDEVINFO*) LocalAlloc(LPTR, cbRasDevInfo);
        if (NULL == pRasDevInfo)
        {
            dwResult = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }

        pRasDevInfo->dwSize = sizeof(RASDEVINFO);
        dwResult = RasEnumDevices(pRasDevInfo, &cbRasDevInfo, &cRasDevices);
    } while (ERROR_BUFFER_TOO_SMALL == dwResult);

    if (ERROR_SUCCESS == dwResult)
    {
        DWORD i;

        //
        // Search the list for a matching name.
        //
        for (i = 0; i < cRasDevices; i++)
        {
            if (0 == wcsicmp(pRasDevInfo[i].szDeviceName, wszAdapterName))
            {
                bFoundMatchingRasDeviceName = TRUE;
                break;
            }
        }
    }
    LocalFree(pRasDevInfo);

    // If the name of the NDIS adapter matches that of a RAS device, then it
    // is a VEM interface representing a RAS connection.
    *bIsVEMAdapter = bFoundMatchingRasDeviceName;

    return dwResult;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
BOOL EthernetLan::GetAdapterIPProperties (PNETWORK_ADPT_INFO pAdptInfo)
{
    BOOL    fRetVal = TRUE;
    TCHAR    szTemp[256];
    LONG    hRes;
    DWORD    dwSize, dwType;
    HKEY    hKey = NULL;
    PFIXED_INFO         pNetworkParams = NULL;
    PIP_ADDR_STRING pDnsServer = NULL;
    ULONG               ulBufferSize = 0;
    WCHAR   szDNSAddr[16];

    if ((NULL == m_pszLanName) || (NULL==pAdptInfo))
        return FALSE;

    pAdptInfo->szAdapterName = m_pszAdapter;
    // By default we enable DHCP.
    pAdptInfo->fUseDHCP = 1;
    
    // Get the current registry data.
    _tcscpy_s (szTemp, 256, TEXT("Comm\\"));
    StringCchCat (szTemp, _countof(szTemp), m_pszAdapter);
    StringCchCat (szTemp, _countof(szTemp), TEXT("\\Parms\\TcpIp"));
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes) 
    {
        // Read the saved data in.
        dwSize = sizeof(DWORD);
        hRes = RegQueryValueEx (hKey, TEXT("EnableDHCP"), 0, &dwType,
                                (LPBYTE)&pAdptInfo->fUseDHCP,
                                &dwSize);
        if ((ERROR_SUCCESS == hRes) && (!pAdptInfo->fUseDHCP)) 
        {
            GetMultiSZAddr (hKey, TEXT("IpAddress"),
                            &pAdptInfo->IPAddr, NULL);
            GetMultiSZAddr (hKey, TEXT("Subnetmask"),
                            &pAdptInfo->SubnetMask, NULL);
            GetMultiSZAddr (hKey, TEXT("DefaultGateway"),
                            &pAdptInfo->Gateway, NULL);
            GetMultiSZAddr (hKey, TEXT("DNS"), &pAdptInfo->DNSAddr,
                            &pAdptInfo->DNSAltAddr);
        }
        else
        {
            GetMultiSZAddr (hKey, TEXT("DhcpIpAddress"),
                            &pAdptInfo->IPAddr, NULL);
            GetMultiSZAddr (hKey, TEXT("DhcpSubnetmask"),
                            &pAdptInfo->SubnetMask, NULL);
            GetMultiSZAddr (hKey, TEXT("DhcpDefaultGateway"),
                            &pAdptInfo->Gateway, NULL);
            // Get Network params struct for DNS servers
            if (ERROR_BUFFER_OVERFLOW == GetNetworkParams(pNetworkParams, &ulBufferSize))
            {
                pNetworkParams = (PFIXED_INFO)malloc(ulBufferSize);
                if (pNetworkParams)
                {
                    if (ERROR_SUCCESS==GetNetworkParams(pNetworkParams, &ulBufferSize))
                    {
                        // Get DNS Servers
                        pDnsServer = &(pNetworkParams->DnsServerList);
                        if (pDnsServer->IpAddress.String[0] != 0)
                        {
                            StringCchPrintf(szDNSAddr, ARRAYSIZE(szDNSAddr), TEXT("%hs"), pDnsServer->IpAddress.String);
                            StringToAddr(szDNSAddr, &pAdptInfo->DNSAddr);
                        }
                    }
                }
            }
        }
    }

    // Save copy of Adptinfo to detect changes
    memcpy(&m_OldAdptInfo, pAdptInfo, sizeof (NETWORK_ADPT_INFO));
    SAFE_REGCLOSEKEY(hKey);
    if (pNetworkParams)
        free(pNetworkParams);
    return fRetVal;
}

BOOL EthernetLan::SetAdapterIPProperties (PNETWORK_ADPT_INFO pAdptInfo)
{
     BOOL    fRetVal = TRUE;
    TCHAR    szTemp[256];
    HKEY    hKey = NULL;
    LONG    hRes;
    DWORD    Len;
    
    if (pAdptInfo->szAdapterName == NULL)
        return FALSE;

    _tcscpy_s (szTemp, 256, TEXT("Comm\\"));
    StringCchCat (szTemp, _countof(szTemp), pAdptInfo->szAdapterName);
    StringCchCat (szTemp, _countof(szTemp), TEXT("\\Parms\\TcpIp"));
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
    
    m_OldAdptInfo.fSaved = 1;  // To allow memcmp below
    //only save AdptInfo when AdptInfo changed(differnet from m_OldAdptInfo)
     if (pAdptInfo->fSaved && hKey != NULL && memcmp(&m_OldAdptInfo,pAdptInfo,sizeof(NETWORK_ADPT_INFO))) 
    {
        
        fRetVal = TRUE;
        
        //When save to registry, use 1 to replace TRUE.
        if (pAdptInfo->fUseDHCP) 
            pAdptInfo->fUseDHCP = 1;
        
        RegSetValueEx (hKey, TEXT("EnableDHCP"), 0, REG_DWORD,
                       (byte *)&(pAdptInfo->fUseDHCP), sizeof(DWORD));

        IPAddrToStr (szTemp, pAdptInfo->IPAddr);
        Len = _tcslen(szTemp)+1;
        szTemp[Len++] = TEXT('\0');
        RegSetValueEx (hKey, TEXT("IpAddress"), 0, REG_MULTI_SZ,
                       (byte *)szTemp, sizeof(TCHAR)*Len);
        
        IPAddrToStr (szTemp, pAdptInfo->SubnetMask);
        Len = _tcslen(szTemp)+1;
        szTemp[Len++] = TEXT('\0');
        RegSetValueEx (hKey, TEXT("Subnetmask"), 0, REG_MULTI_SZ,
                       (byte *)szTemp, sizeof(TCHAR)*Len);
        
        IPAddrToStr (szTemp, pAdptInfo->Gateway);
        Len = _tcslen(szTemp)+1;
        szTemp[Len++] = TEXT('\0');
        RegSetValueEx (hKey, TEXT("DefaultGateway"), 0, REG_MULTI_SZ,
                       (byte *)szTemp, sizeof(TCHAR)*Len);

        Len = 0;
        if (pAdptInfo->DNSAddr) 
        {
            IPAddrToStr (szTemp, pAdptInfo->DNSAddr);
            Len = _tcslen(szTemp)+1;
            if (pAdptInfo->DNSAltAddr) 
            {
                IPAddrToStr (szTemp+Len, pAdptInfo->DNSAltAddr);
                Len += _tcslen(szTemp+Len)+1; // allow for NULL character added by IPAddrToStr
            }
            szTemp[Len++] = TEXT('\0');
        } 
        else 
            if (pAdptInfo->DNSAltAddr) 
            {
                IPAddrToStr (szTemp, pAdptInfo->DNSAltAddr);
                Len = _tcslen(szTemp)+1;
                szTemp[Len++] = TEXT('\0');
            }
        if (Len) 
        {
            RegSetValueEx (hKey, TEXT("DNS"), 0, REG_MULTI_SZ,
                           (byte *)szTemp, sizeof(TCHAR)*Len);
        } 
        else 
        {
            RegSetValueEx (hKey, TEXT("DNS"), 0, REG_MULTI_SZ,
                           (byte *)TEXT(""), sizeof(TCHAR));
        }        
        
        Len = 0;
        if (pAdptInfo->WINSAddr) 
        {
            IPAddrToStr (szTemp, pAdptInfo->WINSAddr);
            Len = _tcslen(szTemp)+1;
            if (pAdptInfo->WINSAltAddr) 
            {
                IPAddrToStr (szTemp+Len, pAdptInfo->WINSAltAddr);
                Len += _tcslen(szTemp+Len)+1; // allow for NULL character added by IPAddrToStr
            }
            szTemp[Len++] = TEXT('\0');
        } 
        else 
            if (pAdptInfo->WINSAltAddr)
            {
                IPAddrToStr (szTemp, pAdptInfo->WINSAltAddr);
                Len = _tcslen(szTemp)+1;
                szTemp[Len++] = TEXT('\0');
            }
            
        if (Len)
        {
            RegSetValueEx (hKey, TEXT("WINS"), 0, REG_MULTI_SZ,
                           (byte *)szTemp, sizeof(TCHAR)*Len);
        } 
        else 
        {
            RegSetValueEx (hKey, TEXT("WINS"), 0, REG_MULTI_SZ,
                           (byte *)TEXT(""), sizeof(TCHAR));
        }    

        RestartAdapter(pAdptInfo->szAdapterName);
    
    }
    SAFE_REGCLOSEKEY(hKey);
    
    return fRetVal;

}

//
//    Do an IOControl to NDIS, passing to NDIS a multi_sz string buffer.
//    The variable parameters to this function are a NULL terminated list of
//    strings from which to build the multi_sz.
//
void EthernetLan::DoNdisMultiSzIOControl( DWORD dwCommand, ...)
{
    TCHAR    multiSzBuffer[MAX_DATA_SIZE], *tszBuf, *tszArg;
    va_list ArgList;

    va_start (ArgList, dwCommand);

    tszBuf = &multiSzBuffer[0];

    // Build the multi_sz
    while (NULL != (tszArg = va_arg(ArgList, TCHAR *)))
    {
        _tcscpy_s(tszBuf, _countof(multiSzBuffer) - (tszBuf - multiSzBuffer)/sizeof(TCHAR), tszArg);
        tszBuf += _tcslen(tszArg) + 1;
    }

    // Add the extra terminator to denote end of the multi_sz
    *tszBuf++ = TEXT('\0');

    va_end(ArgList);

    (void)DoNdisIOControl(dwCommand, &multiSzBuffer[0], (tszBuf - &multiSzBuffer[0]) * sizeof(TCHAR), NULL, NULL);
}


void EthernetLan::RestartAdapter(TCHAR *szAdapterName)
{
    if(NULL != szAdapterName)
    {
        DoNdisMultiSzIOControl(IOCTL_NDIS_UNBIND_ADAPTER, szAdapterName, NULL, NULL);
        DoNdisMultiSzIOControl(IOCTL_NDIS_BIND_ADAPTER, szAdapterName, NULL, NULL);
    }
}

void
IPAddrToStr (LPTSTR szStr, DWORD IPAddr)
{
    StringCchPrintf(szStr, 256, TEXT("%d.%d.%d.%d"),
              (IPAddr >> 24) & 0xFF, (IPAddr >> 16) & 0xFF,
              (IPAddr >> 8) & 0xFF,  IPAddr & 0xFF);
}
BOOL StringToAddr(TCHAR *AddressString, DWORD *AddressValue) {
    TCHAR    *pStr = AddressString;
    PUCHAR    AddressPtr = (PUCHAR)AddressValue;
    int        i;
    int        Value;

    // Parse the four pieces of the address.
    for (i=0; *pStr && (i < 4); i++) {
        Value = 0;
        while (*pStr && TEXT('.') != *pStr) {
            if ((*pStr < TEXT('0')) || (*pStr > TEXT('9'))) {
                DEBUGMSG (1,
                          (TEXT("Unable to convert %s to address\r\n"),
                           AddressString));
                return FALSE;
            }
            Value *= 10;
            Value += *pStr - TEXT('0');
            pStr++;
        }
        if (Value > 255) {
            DEBUGMSG (1,
                      (TEXT("Unable to convert %s to address\r\n"),
                       AddressString));
            return FALSE;
        }
        AddressPtr[i] = Value;
        if (TEXT('.') == *pStr) {
            pStr++;
        }
    }

    // Did we get all of the pieces?
    if (i != 4) {
        DEBUGMSG (1,
                  (TEXT("Unable to convert %s to address\r\n"),
                   AddressString));
        return FALSE;
    }

    *AddressValue = net_long (*AddressValue);
    
    DEBUGMSG (1,
              (TEXT("Converted %s to address %X\r\n"),
               AddressString, *AddressValue));

    return TRUE;
}    // StringToAddr()

VOID
GetMultiSZAddr (HKEY hKey, LPTSTR szValueName, LPDWORD pAddr1, LPDWORD pAddr2)
{
    TCHAR    szTemp[256] = {0};
    LPTSTR    szPtr;
    LONG    hr;
    DWORD    dwSize, dwType;

    dwSize = sizeof(szTemp);
    hr = RegQueryValueEx (hKey, szValueName, 0, &dwType, (LPBYTE)szTemp,
                            &dwSize); 
    if ((hr == ERROR_SUCCESS) &&
        ((dwType == REG_SZ) || (dwType == REG_MULTI_SZ))) {
        if (pAddr1 && szTemp[0]) {
            StringToAddr (szTemp, pAddr1);
        }
        if (pAddr2 && szTemp[0] && (dwType == REG_MULTI_SZ)) {
            szPtr = szTemp;
            while (*szPtr) {
                szPtr++;
            }
            // Move past the null.
            szPtr++;
            if (*szPtr) {
                StringToAddr (szPtr, pAddr2);
            }
        }
    } else {
        DEBUGMSG (1,
                  (TEXT("GetMultiSZ: '%s' hr=%d dwType=%d\r\n"),
                   szValueName, hr, dwType));
    }
    
}
