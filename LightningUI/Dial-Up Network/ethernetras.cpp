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


#include "ethernetras.h" 
#include "winsock2.h"
#include "ws2tcpip.h"
#include <ehm.h>


#define MAX_NETBIOS_NAME    15
#define DEFAULTLDAILINGPATTERNS TEXT("Default\000G\000G\000G\0000\000\000")
#define DEFAULTLDAILINGPATTERNS_STRLENGTH 18
static const TCHAR  szRegistryKey1[]=TEXT("ControlPanel\\Dial");
static const TCHAR  szRegistryKey2[]=TEXT("ControlPanel\\Dial\\Locations");
static const TCHAR  szCurrentLoc[] = TEXT("CurrentLoc");
static const TCHAR  szHighLoc[] = TEXT("HighLocID");


const TCHAR g_wszGoodNetBIOSSymbols[] = L"!@#$%^&\')(.-_{}~";
const TCHAR g_wszConnTCPIPParmsRegistryPath[] = L"Comm\\TCPIP\\Parms";
const TCHAR g_wszVPNStrictDNS[] = L"VPNStrictDNSNamesOnly";
const TCHAR g_szBadRasCharList[] = { _T('\\'), _T('/'), _T(':'),
                                                _T('?'), _T('\"'), _T('<'),
                                                _T('>'), _T('|') };

#define net_long(x) (((((ulong)(x))&0xffL)<<24) | \
                     ((((ulong)(x))&0xff00L)<<8) | \
                     ((((ulong)(x))&0xff0000L)>>8) | \
                     ((((ulong)(x))&0xff000000L)>>24))


DWORD AllocRasEnumDevices(
    __out LPRASDEVINFO *ppRasDevInfo,
    __out DWORD        *pcbRasDevInfo,
    __out DWORD        *pcRasDevices)
//
//  This function allocates space for a buffer to
//  contain an array of RASDEVINFO structure and fills
//  it with info on all the RAS devices in the system.
//
//  If successful, it returns ERROR NO_ERROR and the caller
//  is responsible for deleting the buffer. Otherwise, a
//  non-zero error code is returned.
//
{
    LPRASDEVINFO pRasDevInfo            = NULL;
    DWORD        cbRasDevInfo           = sizeof(RASDEVINFO);
    DWORD        cRasDevices            = 0;
    DWORD        dwResult               = NO_ERROR;

    //
    //  Keep iterating as long as RasEnumDevices returns ERROR_BUFFER_TOO_SMALL.
    //  On each iteration, free the old buffer and get a new buffer that hopefully
    //  will be large enough.
    //
    do
    {
		if(pRasDevInfo)
			delete [] pRasDevInfo;
        pRasDevInfo = reinterpret_cast<LPRASDEVINFO>(new BYTE[cbRasDevInfo]);
        if (NULL == pRasDevInfo)
        {
            dwResult = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }
        pRasDevInfo->dwSize = sizeof(RASDEVINFO);
        dwResult = ::RasEnumDevices(pRasDevInfo, &cbRasDevInfo, &cRasDevices);

    } while (ERROR_BUFFER_TOO_SMALL == dwResult);

    if (NO_ERROR != dwResult)
    {
        delete [] pRasDevInfo;
        pRasDevInfo = NULL;
        cbRasDevInfo = 0;
        cRasDevices = 0;
    }
    *ppRasDevInfo = pRasDevInfo;
    *pcbRasDevInfo = cbRasDevInfo;
    *pcRasDevices = cRasDevices;

    return dwResult;
}


DWORD GetDevices(
    __out LPRASDEVINFO *ppRasDevInfo)
//
//  Allocate a buffer containing an array
//  of RAS device info. The number of devices is returned.
//  The caller is responsible for freeing the buffer.
//
{
    DWORD cb;
    DWORD dwDeviceCount = 0;

    if (ppRasDevInfo)
    {
        AllocRasEnumDevices(ppRasDevInfo, &cb, &dwDeviceCount);

#ifdef DEBUG
        DEBUGMSG (1, (TEXT("GetRasDevices: Found %d devices\r\n"), dwDeviceCount));
        for (DWORD i = 0; i < dwDeviceCount; i++)
        {
          DEBUGMSG (1, (TEXT("\tDev[%d] : Name='%s' Type='%s'\r\n"),
                    i, (*ppRasDevInfo)[i].szDeviceName,
                    (*ppRasDevInfo)[i].szDeviceType));
        }
#endif
    }
    return dwDeviceCount;
}

HRESULT EthernetGetRasDialParams(LPCTSTR szEntryName, RASDIALPARAMS *pRasDialParams, BOOL *pbGotPassword)
{
    HRESULT dwResult = S_OK;
    BOOL bGotPassword = FALSE;

    // Get RasDialParams
    memset(pRasDialParams,0,sizeof(*pRasDialParams));
    pRasDialParams->dwSize = sizeof(*pRasDialParams);
    if (szEntryName[0]!=TEXT('\0'))
    {
        StringCchCopy(pRasDialParams->szEntryName, RAS_MaxEntryName + 1, szEntryName);
        dwResult = ::RasGetEntryDialParams(NULL, pRasDialParams, &bGotPassword);
        if (NO_ERROR!=dwResult && bGotPassword)
        {
            dwResult = E_FAIL;
        }
    }

    if (pbGotPassword)
    {
        *pbGotPassword = bGotPassword;
    }  

    return dwResult;
}

// Get the device specific structure from out of the phonebook. 
// This is of varying size and content according to the kind of RAS connection.
// For L2TP-IPSec VPNs this will be a L2TP_CONFIG_DATA. 
HRESULT EthernetGetRasEntryProperties(LPCTSTR szEntryName,
                     RASENTRY* pRasEntry,
                     PBYTE& pDevConfig,
                     DWORD& cbDevConfig)
{
    DWORD cbRasEntry=0;
    DWORD dwRasResult;

    // Get the size of the RasEntry structure
    dwRasResult = RasGetEntryProperties(NULL,
                                   szEntryName,
                                   NULL,
                                   &cbRasEntry,
                                   NULL,
                                   NULL);

    if (dwRasResult!=ERROR_BUFFER_TOO_SMALL)
    {
        goto Exit;
    }

    pRasEntry->dwSize = cbRasEntry;

    // Now get the size of the Devconfig structure
    // RAS doesn't give me the size of both buffers at once!
    dwRasResult = RasGetEntryProperties(NULL,
                                   szEntryName,
                                   pRasEntry,
                                   &cbRasEntry,
                                   NULL,
                                   &cbDevConfig);

    if (cbDevConfig>0)
    {
        if(pDevConfig)
        {
            free(pDevConfig);
            pDevConfig = NULL;
        }
        pDevConfig = (PBYTE)malloc(cbDevConfig);
        if (!pDevConfig)
        {
            dwRasResult = ERROR_NOT_ENOUGH_MEMORY;
            goto Exit;
        }

        dwRasResult = RasGetEntryProperties(NULL,
                                       szEntryName,
                                       pRasEntry,
                                       &cbRasEntry,
                                       pDevConfig,
                                       &cbDevConfig);
    }

    Exit:
    if (dwRasResult!=ERROR_SUCCESS)
    {
        free(pDevConfig);
        pDevConfig=NULL;
        cbDevConfig=0;
    }
    return (dwRasResult==ERROR_SUCCESS) ? S_OK : E_FAIL;
}

HRASCONN GetRasConnectionHandle(__in TCHAR * pszName)
{
   HRASCONN hRasConn = NULL;
   RASCONN rasConn = {0};
   LPRASCONN lpRasConn = &rasConn;
   DWORD cb = sizeof(RASCONN);
   DWORD dwConnections = 1;
   DWORD dwRetVal = 0;

   lpRasConn->dwSize = sizeof(RASCONN);
   dwRetVal = ::RasEnumConnections(lpRasConn, &cb, &dwConnections);

   if (0 != dwRetVal)
   {
      if (ERROR_BUFFER_TOO_SMALL == dwRetVal)
      {
         // This means there is more than one active connection
         lpRasConn = reinterpret_cast<LPRASCONN>(new BYTE[cb]);
         if (!lpRasConn)
            return NULL;

         lpRasConn->dwSize = sizeof(RASCONN);
         if (0 != ::RasEnumConnections(lpRasConn, &cb, &dwConnections))
         {
            // An unknown error occured, bail now
            delete [] lpRasConn;
            return NULL;
         }
      }
      else
      {
         // An unknown error occured, bail now
         return NULL;
      }
   }

   for (DWORD i = 0; i < dwConnections; i++)
   {
      if (!::_tcscmp(pszName, lpRasConn[i].szEntryName))
      {
         hRasConn = lpRasConn[i].hrasconn;
         break;
      }
   }

   // If we had to dynamicly allocate memory, free it here
   if (lpRasConn != &rasConn)
      delete [] lpRasConn;

   return hRasConn;
}

BOOL IsValidPhoneNumberString(TCHAR * pszValidate)
{
   if (!pszValidate)
      return FALSE;

   for (int i = 0; i < (_countof(g_szBadRasCharList)); i++)
   {
      if (::_tcschr(pszValidate, g_szBadRasCharList[i]))
         return FALSE;
   }

   return TRUE;
}


BOOL IsValidVPNHostString(TCHAR * wszValidate)
{
    BOOL bValid = FALSE;
    
    if (!wszValidate)
        return FALSE;

    // First, check for a valid numeric format.
    // (The really annoying thing about this is that the first thing
    // ::getaddrinfo does is MultiByteToWideChar(CP_ACP,...).)

    char szTargetString[NI_MAXHOST];
    if (0 != ::WideCharToMultiByte(CP_ACP, 0, wszValidate, -1,
                                   szTargetString, sizeof(szTargetString),
                                   NULL, NULL))
    {
        ADDRINFO hints, *ai;
        ::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_flags = AI_NUMERICHOST;
        bValid = NO_ERROR == ::getaddrinfo(szTargetString, "", &hints, &ai);
        ::freeaddrinfo(ai);
    }

    // Unfortunately can't use getaddrinfo to check for a valid name
    // because it does a DNS lookup which might fail for various
    // reasons even if there is a valid name.  It is too bad, because
    // presumably the stack knows how to validate DNS names &
    // NetBIOS names.

    // So see the MSDN article "Complying with Name Restrictions for
    // Hosts and Domains" for the rules for valid names.

    // If the name is 15 chars or less, then check against the valid
    // NetBIOS characters:  characters, numbers, white space, and the
    // symbols !@#$%^&')(.-_{}~

    if (!bValid && ::wcslen(wszValidate) <= MAX_NETBIOS_NAME)
    {
        TCHAR * wsz = wszValidate;
        TCHAR wc;
        while (0 != (wc = *wsz++))
        {
            if (::iswctype(wc, _ALPHA|_DIGIT|_SPACE)) continue;
            if (::wcschr(g_wszGoodNetBIOSSymbols, wc))  continue;
            // invalid NetBIOS character
            break;
        }
        bValid = 0 == wc;
    }
    
    // Strict DNS:  A-Za-z0-9-.
    // (except not checking maximum 63 bytes/label)
    // (and not checking that '_' only at first character in a label)

    if (!bValid)
    {
        TCHAR * wsz = wszValidate;
        TCHAR wc;
        while (0 != (wc = *wsz++))
        {
            if (L'a' <= wc && wc <= L'z') continue;
            if (L'A' <= wc && wc <= L'Z') continue;
            if (L'0' <= wc && wc <= L'9') continue;
            if (L'-' == wc)               continue;
            if (L'.' == wc)               continue;
            if (L'_' == wc)               continue;
            // invalid strict DNS character
            break;
        }
        bValid = 0 == wc;
    }

    // Slack DNS rules: Windows can be configured so that it is
    // possible to use any UTF-8 character in a name.  Maximum length
    // is 255 bytes in UTF-8.

    // Determine if non-standard slackDNS check should be done by
    // checking the registry.  Default (registry key missing) is to do
    // the slack check.  If the value is non-zero skip the slack check.

    if (!bValid)
    {
        HKEY  hKey = NULL;
        DWORD dwVPNStrictDNS = 0;
        DWORD cbVPNStrictDNS = sizeof(dwVPNStrictDNS);
        if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, g_wszConnTCPIPParmsRegistryPath, NULL, KEY_READ, &hKey))
        {
            (void)::RegQueryValueEx(hKey, g_wszVPNStrictDNS, NULL, NULL, (LPBYTE)&dwVPNStrictDNS, &cbVPNStrictDNS);
        }
        if (NULL != hKey)
        {            
            ::RegCloseKey(hKey);
        }
        if (0 == dwVPNStrictDNS)
        {
            // (But ::WideCharToMultiByte will never return an error on
            // conversion due to invalid characters (it will even eat misused
            // surrogates).  So there's no point in this check.)

            int cb = ::WideCharToMultiByte(CP_UTF8, 0, wszValidate, -1, NULL, 0, NULL, NULL);
                // (CE: Doesn't support default char on UTF8 conversion)
            bValid = 0 < cb && cb <= 255;
        }
    }

    return bValid;
}

// EAP related functions

DWORD EthernetEapEnumExtensions(PEAP_EXTENSION_INFO& pEapExtInfo, DWORD& dwNumExtensions)
//
//  Build an array containining information about all the EAP extensions
//  available in the system.
//
{
    DWORD   dwResult = NO_ERROR;
    DWORD   cbEapExtInfo = 0;

    // Don't reenumerate if we have already enumerated the info
    if (pEapExtInfo == NULL)
    {
        dwNumExtensions = 0;

        while (TRUE)
        {
            dwResult = ::EapEnumExtensions(cbEapExtInfo, pEapExtInfo, &dwNumExtensions, &cbEapExtInfo);
            if (dwResult != ERROR_INSUFFICIENT_BUFFER)
                break;

            // Free old buffer
            LocalFree(pEapExtInfo);

            // Allocate new bigger buffer
            pEapExtInfo = (PEAP_EXTENSION_INFO)LocalAlloc(LPTR, cbEapExtInfo);
            if (pEapExtInfo == NULL)
            {
                dwNumExtensions = 0;
                dwResult = ERROR_OUTOFMEMORY;
                break;
            }
        }
    }
    return dwResult;
}


void CALLBACK NullLineCallbackFunc(DWORD dwDevice, DWORD dwMsg,
                                                DWORD dwCallbackInstance,
                                                DWORD dwParam1, DWORD dwParam2,
                                                DWORD dwParam3)

{
   // NULL function.  I can't do a lineInitialize without this function.
   // Since I never dial I don't actually care about any of the state changes
   return;
}

LPLINETRANSLATECAPS GetTranslateCaps(HINSTANCE hInstance)
{
   long lReturn = 0;
   HLINEAPP hLineApp = NULL;
   DWORD dwNumDevs = 0;
   DWORD dwNeededSize = sizeof(LINETRANSLATECAPS);
   LPLINETRANSLATECAPS pLineTranCaps = NULL;
   DWORD dwVersion = TAPI_CURRENT_VERSION;

   lReturn = ::lineInitialize(&hLineApp, hInstance,
                              NullLineCallbackFunc,
                              NULL, &dwNumDevs);

   if (lReturn)
   {
      return NULL;
   }

   while (TRUE)
   {
      pLineTranCaps = (LPLINETRANSLATECAPS) LocalAlloc(LPTR, dwNeededSize);
      if (NULL == pLineTranCaps)
      {
         break;
      }
      pLineTranCaps->dwTotalSize = dwNeededSize;
      lReturn = ::lineGetTranslateCaps(hLineApp, dwVersion, pLineTranCaps);

      if (lReturn)
      {
         LocalFree(pLineTranCaps);
         pLineTranCaps = NULL;
         break;
      }
      if (pLineTranCaps->dwNeededSize > pLineTranCaps->dwTotalSize)
      {
         dwNeededSize = pLineTranCaps->dwNeededSize;
         ::LocalFree(pLineTranCaps);
         pLineTranCaps = NULL;
         continue;
      }
      break;
   }

   ::lineShutdown(hLineApp);
   
   return pLineTranCaps;
}

DWORD GetDefaultCntryRgnCode(LPLINETRANSLATECAPS pCaps)
{
   DWORD dwCntryRgnCode = 0;

   if (pCaps && pCaps->dwLocationListSize )
   {
      LPLINELOCATIONENTRY pLocEntry = (LPLINELOCATIONENTRY) ((LPBYTE)pCaps + pCaps->dwLocationListOffset);

      for (int i=0; i < (int) pCaps->dwNumLocations; i++)
      {
         if (pLocEntry[i].dwPermanentLocationID == pCaps->dwCurrentLocationID)
         {
            dwCntryRgnCode = pLocEntry[i].dwCountryCode;
         }
      }
   }

   return dwCntryRgnCode;
}

void GetDefaultAreaCode(LPLINETRANSLATECAPS pCaps, TCHAR * pszAreaCode, DWORD dwMaxLen)
{
   if (!pszAreaCode)
      return;

   *pszAreaCode = _T('\0');

   if (pCaps && pCaps->dwLocationListSize)
   {
      LPLINELOCATIONENTRY pLocEntry = (LPLINELOCATIONENTRY) ((LPBYTE)pCaps + pCaps->dwLocationListOffset);

      for(int i = 0; i < (int) pCaps->dwNumLocations; i++)
      {
         if(pLocEntry[i].dwPermanentLocationID == pCaps->dwCurrentLocationID)
         {
            _tcsncpy_s(pszAreaCode, RAS_MaxAreaCode,
                     (LPTSTR)((LPBYTE)pCaps + pLocEntry[i].dwCityCodeOffset),
                     dwMaxLen);
         }
      }
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
        while (*pStr && TEXT('.') != *pStr) 
        {
            if ((*pStr < TEXT('0')) || (*pStr > TEXT('9')))
                return FALSE;
            
            Value *= 10;
            Value += *pStr - TEXT('0');
            pStr++;
        }
        if (Value > 255)
            return FALSE;
        
        AddressPtr[i] = Value;
        if (TEXT('.') == *pStr) 
            pStr++;        
    }

    // Did we get all of the pieces?
    if (i != 4) 
        return FALSE;

    *AddressValue = net_long (*AddressValue);

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
        ((dwType == REG_SZ) || (dwType == REG_MULTI_SZ))) 
    {
        if (pAddr1 && szTemp[0]) 
        {
            StringToAddr (szTemp, pAddr1);
        }
        if (pAddr2 && szTemp[0] && (dwType == REG_MULTI_SZ)) 
        {
            szPtr = szTemp;
            while (*szPtr) 
            {
                szPtr++;
            }
            // Move past the null.
            szPtr++;
            if (*szPtr) 
            {
                StringToAddr (szPtr, pAddr2);
            }
        }
    }
}



/*--------------------------------------------------------------------------*\

    Function:   RegSetDefaultDialSettings
    
    Purpose:    
      Set default dialing settings of Network Connection to registry.  
\*--------------------------------------------------------------------------*/
HRESULT RegSetDefaultDialSettings(void)
{
    #define SZ_DUMMY    10
    TCHAR   achLocName[SZ_DUMMY] = {0}; /* buffer for location key name   */ 
    HKEY    hKey1 = NULL, hKey2 = NULL;
    HRESULT hr = S_OK;
    int i, iDialingRuleIndex;
    LPTSTR  lpszValue = NULL;
    DWORD dwCurrLocId, dwHighLocId;
    DWORD   dwDisp, dwSize, dwType, dwLocLen, dwDataLen;
    DWORD    cSubKeys=0;               /* number of subkeys       */ 
    DWORD    cbMaxSubKey=0;            /* longest subkey size     */ 
    DWORD    cchMaxClass=0;            /* longest class string    */ 
    DWORD    cValues=0;                /* number of values for key    */ 
    DWORD    cchMaxValue=0;            /* longest value name          */ 
    DWORD    cbMaxValueData=0;         /* longest value data          */ 

    hr = RegCreateKeyEx(HKEY_CURRENT_USER, szRegistryKey1, 0, NULL, 0, 0, NULL, &hKey1, &dwDisp);
    CBR(ERROR_SUCCESS == hr);
    
    if ( (REG_CREATED_NEW_KEY == dwDisp) ) 
    {
        dwCurrLocId = 0;
        dwHighLocId = 1;
    } 
    else 
    {
        // Read the Curr and High Location IDs stored under this key
        dwSize = sizeof(dwCurrLocId);
        hr = RegQueryValueEx(hKey1,
                               szCurrentLoc,
                               NULL,
                               &dwType,
                               (PUCHAR)&dwCurrLocId,
                               &dwSize);
        if (ERROR_SUCCESS != hr)
            dwCurrLocId = 0;

        dwSize = sizeof(dwHighLocId);
        hr = RegQueryValueEx(hKey1,
                               szHighLoc,
                               NULL,
                               &dwType,
                               (PUCHAR)&dwHighLocId,
                               &dwSize);
        if (ERROR_SUCCESS != hr)
            dwHighLocId = 0;  // Gets incremented below if needed.
    }
    
    // Second key contains list of SZSZ strings, each representing a locale
    hr = RegCreateKeyEx(HKEY_CURRENT_USER, szRegistryKey2, 0, NULL, 0, 0, NULL, &hKey2, &dwDisp);             
    CBR(ERROR_SUCCESS == hr);
    if ( REG_OPENED_EXISTING_KEY == dwDisp ) 
    {
        // See how many keys exist and how large they are
        hr = RegQueryInfoKey( hKey2, NULL, NULL, NULL,
                                     &cSubKeys,
                                     &cbMaxSubKey,
                                     &cchMaxClass,
                                     &cValues,
                                     &cchMaxValue,
                                     &cbMaxValueData,
                                     NULL, NULL );
        if( hKey2 && cchMaxValue )
        {
            // Allocate a temp string to store a single location from registry
            cbMaxValueData += sizeof(TCHAR);  // Leave room for final NULL

            lpszValue = (LPTSTR)VirtualAlloc(NULL, cbMaxValueData * sizeof(TCHAR), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
            CPR(lpszValue);

            // Enumerate through the available locations.
            for( i=0; i<(int)cValues; i++ )
            {
                dwLocLen = sizeof(achLocName)/sizeof(TCHAR);
                dwDataLen = cbMaxValueData;

                hr = RegEnumValue(hKey2,
                                    i,
                                    achLocName,
                                    &dwLocLen,
                                    NULL,
                                    &dwType,
                                    (LPBYTE)lpszValue,
                                    &dwDataLen);

                if( ERROR_SUCCESS != hr )
                    break;

                iDialingRuleIndex = _ttoi(achLocName);
                CBR(iDialingRuleIndex>=0 && iDialingRuleIndex<dwHighLocId);
                if(0 == _tcsncmp(DEFAULTLDAILINGPATTERNS, lpszValue, DEFAULTLDAILINGPATTERNS_STRLENGTH))
                {
                    if(iDialingRuleIndex!=dwCurrLocId)
                    {
                        dwCurrLocId = iDialingRuleIndex;
                        hr = RegSetValueEx(hKey1, (LPTSTR)szCurrentLoc, 0, REG_DWORD, (LPBYTE)&iDialingRuleIndex, sizeof(DWORD));
                        CBR(ERROR_SUCCESS == hr);
                    }
                    goto Error;
                }
            }
            dwCurrLocId = i;
            dwHighLocId = i + 1;            
        }
        else
        {
            dwCurrLocId = 0;
            dwHighLocId = 1;
        }
    }
    else
    {
        // No strings found in registry, use default.
        dwCurrLocId = 0;
        dwHighLocId = 1;
    }
    _itot_s(dwCurrLocId, achLocName, SZ_DUMMY, 10);
    hr = RegSetValueEx(hKey2, (LPTSTR)achLocName, 0, REG_MULTI_SZ, (LPBYTE)DEFAULTLDAILINGPATTERNS, DEFAULTLDAILINGPATTERNS_STRLENGTH*sizeof(TCHAR));
    CBR(ERROR_SUCCESS == hr);
    hr = RegSetValueEx(hKey1, (LPTSTR)szCurrentLoc, 0, REG_DWORD, (LPBYTE)&dwCurrLocId, sizeof(DWORD));
    CBR(ERROR_SUCCESS == hr);
    hr = RegSetValueEx(hKey1, (LPTSTR)szHighLoc, 0, REG_DWORD, (LPBYTE)&dwHighLocId, sizeof(DWORD)); 
    CBR(ERROR_SUCCESS == hr);
Error:
    if (lpszValue)
            VirtualFree(lpszValue, 0, MEM_RELEASE);
    if (hKey1) 
        RegCloseKey(hKey1);                       
    if (hKey2) 
        RegCloseKey(hKey2);                       
    return hr;
}

bool HasDevices(TCHAR * pszDeviceType, TCHAR *pszDeviceNamePrefix)
{
   ASSERT(pszDeviceType);
   if (!pszDeviceType)
      return false;

   bool fHasDevices = false;
   LPRASDEVINFO pRasDevInfo = NULL;
   DWORD dwRasDevices = GetDevices(&pRasDevInfo);

   if ((!dwRasDevices) || (!pRasDevInfo)) {
       // No devices
       return false;
   }

   for (DWORD i=0; i < dwRasDevices; i++)
   {
      if (!_tcscmp(pRasDevInfo[i].szDeviceType, pszDeviceType))
      {
          if (pszDeviceNamePrefix == NULL
            || !_tcsncmp(pRasDevInfo[i].szDeviceName, pszDeviceNamePrefix, _tcslen(pszDeviceNamePrefix)) )
          {
             fHasDevices = true;
             break;
          }
      }
   }
   if (pRasDevInfo)
      delete [] pRasDevInfo;

   return fHasDevices;
}

DWORD RegGetDwordValue(
    IN  HKEY        hKey,
    IN  PWSTR        wszSubKeyName,
    IN  PWSTR        wszValueName,
    OUT PDWORD      pdwValue)
{
    HKEY                hk;
    DWORD               dwSize;
    DWORD               dwType;
    DWORD               dwResult;
    DWORD               dwValue;

    dwResult = RegOpenKeyEx(hKey, wszSubKeyName, 0, KEY_ALL_ACCESS, &hk);
    if (ERROR_SUCCESS == dwResult)
    {
        dwSize = sizeof(DWORD);
        dwResult = RegQueryValueEx(hk, wszValueName, NULL, &dwType, (BYTE *)&dwValue, &dwSize);
        if (ERROR_SUCCESS == dwResult)
        {
            if (REG_DWORD == dwType)
                *pdwValue = dwValue;
            else
                dwResult = ERROR_BADDB;
        }
        RegCloseKey(hk);
    }

    return dwResult;
}

