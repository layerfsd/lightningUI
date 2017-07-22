/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkWps.h#1 $ */
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
/// \brief      WPS Related Constants & functions 
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2012-06-14
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKWPS_H_
#define _WDKWPS_H_

#include <WdkVersion.h>
#include <WifiDKDef.h>
#include <WdkWifiTypes.h>
#include <WdkConversions.h>
#include <WdkStringUtil.h>

#define WPS_EVENT_NAME          L"WdkWpsEvent"      ///< Name of the WPS connection event
#define WPS_CONNECT_TIMEOUT     (2 * 60 * 1000)     ///< Timeout for the WPS connection
#define WPS_PIN_LENGTH          8;                  ///< WPS PIN length

/// \brief Parse wifi extended attributes (IEs) to retrieve WPS info.
///
/// \param pInfo [out] pointer to a WPS_IE_INFO structure to contain the results of the parsing.
/// \param pbIEs [in] pointer to a BYTE array containing the IEs.
/// \param cbIEs  length of the IE BYTE array.
/// \param fRaw set to TRUE if the IEs include the fixed IE elements.
///
/// \return HRESULT
///     - ERROR_SUCCESS: Success
///     - Other HRESULT values
///
WifiDKCore_API HRESULT WDK_WPS_ProcessIEs( __out PWPS_IE_INFO pInfo, __in PBYTE pbIEs, DWORD cbIEs, BOOL fRaw);

/// \brief Validate the checksum of an eight(8) digit WPS PIN value.
///
/// \param pin WPS pin to evaluate.
///
/// \return BOOL
///     - TRUE: The checksum is valid
///     - FALSE: The checksum is not invalid.
///
WifiDKCore_API BOOL WDK_WPS_ValidateWpsChecksum(LONG pin);

/// \brief Computes the checksum of an 7 digit PIN value.
///
/// \param pin WPS pin to evaluate.
///
/// \return INT
///     - TRUE: The checksum is valid
///     - FALSE: The checksum is not invalid.
///
WifiDKCore_API INT WDK_WPS_ComputeWpsChecksum(LONG pin);

/// \brief Validate the a WPS PIN value.
///
/// All PINs of length four (4) are considered valid.
/// Eight (8) digit pins are valid if the checksum digit is valid.
/// All other PIN lengths will fail validation.
///
/// \param pszPin String representation of a WPS pin.
///
/// \return HRESULT
///     - S_OK: The PIN is valid
///     - Other HRESULT: The PIN is not valid.
///
WifiDKCore_API HRESULT WDK_WPS_ValidateWpsPin( __in LPCSTR pszPin );

__inline HRESULT ConvertWpsKeyParams( __in PWDK_WPS_KEY_PARAMS pWpsKeys, __out PWDK_STA_AP_INFO pAp, __out PWIFI_SECURITY_PARAMS pSecurity )
{
    HRESULT hr = S_OK;
    
    LPWSTR pwzHex = NULL;
    
    memset(pSecurity, 0, sizeof(*pSecurity));
    ResetApInfo(pAp);

    pAp->Authentication = pWpsKeys->auth;
    pAp->Encryption = pWpsKeys->encrypt;
    pAp->Mgmt = WDK_AVAIL_AP_NETWORK_MGMT_Infrastructure;

    hr = UTF8ToWideCharEx(pWpsKeys->key, pWpsKeys->keylen, SZARGS(pSecurity->data.psk.wzKeyMaterial) );
    CHR(hr);

    hr = UTF8ToWideChar(pWpsKeys->ssid, SZARGS(pAp->wzSsid));
    CHR(hr);

Error:
    LOCALFREE(pwzHex);
    return hr;
}

#endif //_WDKWPS_H_

