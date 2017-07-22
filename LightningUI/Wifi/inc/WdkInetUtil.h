/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkInetUtil.h#1 $ */
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
/// \brief      WDK IP Utility
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2012-05-16
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKINETUTIL_H_
#define _WDKINETUTIL_H_
//pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <WdkBldVer.h>
#include <WdkVersion.h>
#include <WdkStringUtil.h>
#include <WdkRandom.hpp>
#include <WdkMacros.h>

#if (CE_MAJOR_VER < 7)
#ifdef __cplusplus
extern "C" {
#endif

int inet_pton(int af, const char *src, void *dst);
int inet_ntop(int af, const void *src, char *dst, size_t size);

#ifdef __cplusplus
}
#endif
#endif

#ifdef __cplusplus
class WdkInetUtil 
{
public:

    ///\brief Validate an IP address string
    ///
    /// \param fIPv6 BOOL value indicating the address is IPv6 (TRUE) or IPv4 (FALSE).
    /// \param [in] pszIpAddr The string representation of the IP address.
    ///
    /// \return HRESULT
    ///     - S_OK: Success
    ///     - E_*: Error Conditions
    static
    HRESULT ValidateIpAddress( BOOL fIPv6, __in LPCSTR pszIpAddr )
    {
        HRESULT hr = S_OK;
        int iRet;
        UCHAR ucBinary[16];
        
        CPP(pszIpAddr);

        iRet = inet_pton((fIPv6 ? AF_INET6 : AF_INET), pszIpAddr, &ucBinary);

        CBREx(1 == iRet, E_INVALIDARG);

    Error:
        return hr;
    }

    ///\brief Validate an IP address string
    ///
    /// \param fIPv6 BOOL value indicating the address is IPv6 (TRUE) or IPv4 (FALSE).
    /// \param [in] pwzIpAddr The unicode string representation of the IP address.
    ///
    /// \return HRESULT
    ///     - S_OK: Success
    ///     - E_*: Error Conditions
    static
    HRESULT ValidateIpAddress( BOOL fIPv6, __in LPCWSTR pwzIpAddr )
    {
        HRESULT hr = S_OK;
        CHAR szIpAddr[INET6_ADDRSTRLEN];
        
        CPP(pwzIpAddr);

        hr = WideCharToUTF8(pwzIpAddr, SZARGS(szIpAddr));
        CHR(hr);
        
        hr = ValidateIpAddress(fIPv6, szIpAddr);
        CHR(hr);

    Error:
        return hr;
    }

    ///\brief Generate a random link local IPv4 address.
    /// 
    /// The generated address is of the form 169.254.x.y, where x and y are a random values between 0-255.
    ///
    /// \param [out, opt] bIpAddr Buffer to hold the IP address bytes.
    /// \param [out, opt] pwzIpAddr String buffer to hold the string representation of the IP address.
    /// \param cchIpAddress Length of the string buffer in characters.
    ///
    /// \return HRESULT
    ///     - S_OK: Success
    ///     - E_QLX_NOT_RANDOM: The generated address is not random
    ///     - E_*: Other Error Conditions
    static
    HRESULT GetRandomLinkLocalAddressIpv4( __out_opt BYTE bIpAddr[IPV4_LEN], __out_opt LPWSTR pwzIpAddr, DWORD cchIpAddr )
    {
        HRESULT hr = S_OK;
        BOOL fRandom = FALSE;
        
        BYTE bRandIpAddr[IPV4_LEN] = {169, 254, 200, 1};
        WdkRandom rand;

        hr = rand.GetBytes( &bRandIpAddr[2], 2 );
        fRandom = SUCCEEDED(hr);

        if(bIpAddr)
        {
            memcpy(bIpAddr, bRandIpAddr, IPV4_LEN);
        }

        if(pwzIpAddr)
        {
            hr = StringCchPrintfW(pwzIpAddr, cchIpAddr, L"%u.%u.%u.%u", bRandIpAddr[0], bRandIpAddr[1], bRandIpAddr[2], bRandIpAddr[3]);
            CHR(hr);
        }

        CBREx(fRandom, E_QLX_NOT_RANDOM);

    Error:
        return hr;
    }
};

#endif // __cplusplus

#endif // _WDKINETUTIL_H_


