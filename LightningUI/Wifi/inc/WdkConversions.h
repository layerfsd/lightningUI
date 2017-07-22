/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkConversions.h#1 $ */
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
/// \brief      Inline functions for converting strings to numbers and visa versa.
/// \remarks 
///             - Module Name     :  Common
///             - Original Author :  Multiple
///             - Original Date   :  Nov 2010
//-----------------------------------------------------------------------------
/// \file
#ifndef _WDK_CONVERSIONS_H_
#define _WDK_CONVERSIONS_H_

#include <WdkVersion.h>
#include <WdkMacros.h>
#include <WdkHresult.h>
#include <WdkEhm.h>

#include <tchar.h>
#include <strsafe.h>

/// \brief Wrapper for the TCHAR version of atoi
///
/// \param pInt pointer to the int to receive the conversion
/// \param tzArg const tstring to be converted
///
/// \return HRESULT
///     - S_OK if the conversion was successful.
///     - E_INVALID_DATA if the string is invalid and could not be converted
///     - E_INVALIDARG one of the arguements is invalid
_inline HRESULT tcs2int(int *pInt, const TCHAR *tzArg)
{
    HRESULT hr = S_OK;
    
    int iValue;
    int iLen;
    TCHAR szBuf[32];
    BOOL fSuccess;

    CPP(tzArg);

    iLen = _tcslen(tzArg);

    CBREx(0<iLen, E_INVALID_DATA);

    //
    // quick out for zero
    //
    if(1==iLen && tzArg[0]==TEXT('0'))
    {
        *pInt = 0;
        goto Done;
    }

    iValue = _ttoi(tzArg);

    //
    // atoi returns 0 if the conversion failed
    //
    CBREx(0 != iValue, E_INVALID_DATA);

    //
    // check the conversion - atoi will return a value
    // even if int value is out of the range of an int
    //
    hr = StringCchPrintfW(szBuf, _countof(szBuf), TEXT("%d"), iValue);
    CHR(hr);

    fSuccess = IS_TSTR_EQUAL(tzArg, szBuf);
    CBREx(fSuccess, E_INVALID_DATA);

Done:
Error:
    return hr;
}


/// \brief Wrapper for the CHAR version of atoi
///
/// \param pInt pointer to the int to receive the conversion
/// \param szArg const string to be converted
///
/// \return HRESULT
/// 	- S_OK if the conversion was successful.
///     - E_INVALID_DATA if the string is invalid and could not be converted
/// 	- E_INVALIDARG one of the arguments is invalid
_inline HRESULT sz2int(int *pInt, const CHAR *szArg)
{
    HRESULT hr = S_OK;
    
    int iValue;
    int iLen;
    CHAR szBuf[32];
    BOOL fSuccess;

    CPP(szArg);

    iLen = strlen(szArg);

    CBREx(0<iLen, E_INVALID_DATA);

    //
    // quick out for zero
    //
    if(1==iLen && szArg[0]=='0')
    {
        *pInt = 0;
        goto Done;
    }

    iValue = atoi(szArg);

    //
    // atoi returns 0 if the conversion failed
    //
    CBREx(0 != iValue, E_INVALID_DATA);

    //
    // check the conversion - atoi will return a value
    // even if int value is out of the range of an int
    //
    hr = StringCchPrintfA(szBuf, _countof(szBuf), "%d", iValue);
    CHR(hr);

    fSuccess = IS_CSTR_EQUAL(szArg, szBuf);
    CBREx(fSuccess, E_INVALID_DATA);

Done:
Error:
    return hr;
}

/// \brief Wrapper for the CHAR version of atoi
///
/// \param pUint pointer to the UINT to receive the conversion
/// \param tzArg const tstring to be converted
///
/// \return HRESULT
/// 	- S_OK if the conversion was successful.
///     - E_INVALID_DATA if the string is invalid and could not be converted
/// 	- E_INVALIDARG one of the arguements is invalid
_inline HRESULT tcs2uint(UINT *pUint, const TCHAR *tzArg)
{
    HRESULT hr = S_OK;
    
    UINT uiValue;
    int iLen;
    TCHAR szBuf[32];
    BOOL fSuccess;
    
    CPP(tzArg);
    iLen = _tcslen(tzArg);

    CBREx(0<iLen, E_INVALID_DATA);

    //
    // quick out for zero
    //
    if(1==iLen && tzArg[0]==TEXT('0'))
    {
        *pUint = 0;
        goto Done;
    }

    uiValue = _ttol(tzArg);

    //
    // _ttol returns 0 if the conversion failed
    //
    CBREx(0 != uiValue, E_INVALID_DATA);

    //
    // check the conversion - _ttol will return a value
    // even if int value is out of the range of an UINT
    //
    hr = StringCchPrintfW(szBuf, _countof(szBuf), TEXT("%u"), uiValue);
    CHR(hr);

    fSuccess = IS_TSTR_EQUAL(tzArg, szBuf);
    CBREx(fSuccess, E_INVALID_DATA);

    *pUint = uiValue;
    
Done:
Error:
    return hr;
}

/// \brief Wrapper for the LONG version of atoi
///
/// \param pVal pointer to the LONG to receive the conversion
/// \param tzArg const tstring to be converted
///
/// \return HRESULT
///     - S_OK if the conversion was successful.
///     - E_INVALID_DATA if the string is invalid and could not be converted
///     - E_INVALIDARG one of the arguements is invalid
_inline HRESULT tcs2long(LONG *pVal, const TCHAR *tzArg)
{
    HRESULT hr = S_OK;
    
    LONG lValue;
    int iLen;
    
    CPP(tzArg);
    iLen = _tcslen(tzArg);

    CBREx(0<iLen, E_INVALID_DATA);

    //
    // quick out for zero
    //
    if(1==iLen && tzArg[0]==TEXT('0'))
    {
        *pVal = 0;
        goto Done;
    }

    lValue = _ttol(tzArg);

    //
    // _ttol returns 0 if the conversion failed
    //
    CBREx(0 != lValue, E_INVALID_DATA);

    *pVal = lValue;
    
Done:
Error:
    return hr;
}

/// \brief Wrapper for the TCHAR version of _wtoi
///
/// \param pdwValue pointer to the UINT to receive the conversion
/// \param tzArg const tstring to be converted
///
/// \return HRESULT
/// 	- S_OK if the conversion was successful.
///     - E_INVALID_DATA if the string is invalid and could not be converted
/// 	- E_INVALIDARG one of the arguements is invalid 
_inline HRESULT tcs2dw(DWORD *pdwValue, const TCHAR *tzArg)
{
    HRESULT hr = S_OK;
    
    DWORD dwValue;
    int iLen;
    TCHAR szBuf[32];
    BOOL fSuccess;
    
    CPP(tzArg);
    iLen = _tcslen(tzArg);

    CBREx(0<iLen, E_INVALID_DATA);

    //
    // quick out for zero
    //
    if(1==iLen && tzArg[0]==TEXT('0'))
    {
        *pdwValue = 0;
        goto Done;
    }

    //chop our i64 to a u32
    dwValue = (DWORD)_wtoi64(tzArg);

    //
    // _wtoi64 returns several things if the conversion failed
    // check for zero we should never see it since we trap 0
    CBREx(0 != dwValue, E_INVALID_DATA);

    //
    // check the conversion - _ttol will return a value
    // even if int value is out of the range of an UINT
    hr = StringCchPrintfW(szBuf, _countof(szBuf), TEXT("%u"), dwValue);
    CHR(hr);

    fSuccess = IS_TSTR_EQUAL(tzArg, szBuf);
    CBREx(fSuccess, E_INVALID_DATA);

    *pdwValue = dwValue;
    
Done:
Error:
    return hr;
}

/// \brief Wrapper for the CHAR version of _atoi
///
/// \param pdwValue pointer to the DWORD to receive the conversion
/// \param szArg const string to be converted
///
/// \return HRESULT
/// 	- S_OK if the conversion was successful.
///     - E_INVALID_DATA the string is invalid and could not be converted
/// 	- E_INVALIDARG one of the arguements is invalid 
_inline HRESULT cs2dw(DWORD *pdwValue, const CHAR *szArg)
{
    HRESULT hr = S_OK;
    
    DWORD dwValue;
    int iLen;
    CHAR szBuf[32];
    BOOL fSuccess;
    
    CPP(szArg);
    iLen = strlen(szArg);

    CBREx(0<iLen, E_INVALID_DATA);

    //
    // quick out for zero
    //
    if(1==iLen && szArg[0]== '0')
    {
        *pdwValue = 0;
        goto Done;
    }

    //chop our i64 to a u32
    dwValue = (DWORD)_atoi64(szArg);

    //
    // _wtoi64 returns several things if the conversion failed
    // check for zero we should never see it since we trap 0
    CBREx(0 != dwValue, E_INVALID_DATA);

    //
    // check the conversion - _ttol will return a value
    // even if int value is out of the range of an UINT
    hr = StringCchPrintfA(szBuf, _countof(szBuf), "%u", dwValue);
    CHR(hr);

    fSuccess = strcmp(szArg, szBuf) == 0;
    CBREx(fSuccess, E_INVALID_DATA);

    *pdwValue = dwValue;
    
Done:
Error:
    return hr;
}

/// \brief Wrapper for the LONG version of atoi
///
/// \param pVal pointer to the LONG to receive the conversion
/// \param szArg const tstring to be converted
///
/// \return HRESULT
///     - S_OK if the conversion was successful.
///     - E_INVALID_DATA if the string is invalid and could not be converted
///     - E_INVALIDARG one of the arguements is invalid
_inline HRESULT cs2long(LONG *pVal, const CHAR *szArg)
{
    HRESULT hr = S_OK;
    
    LONG lValue;
    size_t iLen = 0;
    
    CPP(szArg);
    hr = StringCchLengthA(szArg, 64, &iLen);
    CHR(hr);
    
    CBREx(0<iLen, E_INVALID_DATA);

    //
    // quick out for zero
    //
    if(1==iLen && szArg[0]==TEXT('0'))
    {
        *pVal = 0;
        goto Done;
    }

    lValue = atol(szArg);

    //
    // _ttol returns 0 if the conversion failed
    //
    CBREx(0 != lValue, E_INVALID_DATA);

    *pVal = lValue;
    
Done:
Error:
    return hr;
}

/// \brief converts a string hex pair to a char value
///
/// \param pchData pointer to a char to receive the value
/// \param szValue pointer to a char array containing atleast 2 hex digits to convert.
///
/// \return HRESULT
///     - S_OK: success
///     - E_*: Other error codes
_inline HRESULT hex2char(CHAR *pchData, LPCSTR szValue)
{
    HRESULT hr = S_OK;
    
    CHAR chVal = 0;
    CHAR chTemp;
    int iLen = 2, chIndex = 0;
    

    CPP(szValue);
    CBREx((int)strlen(szValue) >= iLen, E_INVALID_DATA);

    while(chIndex < iLen && SUCCEEDED(hr))
    {
        chTemp = szValue[iLen - chIndex - 1];
        
        if(TEXT('0') <= chTemp && chTemp <= TEXT('9'))
        {
            chTemp = chTemp - TEXT('0');
        }
        else if(TEXT('A') <= chTemp && chTemp <= TEXT('F'))
        {
            chTemp = chTemp - TEXT('A') + 10;
        }
        else if(TEXT('a') <= chTemp && chTemp <= TEXT('f'))
        {
            chTemp = chTemp - TEXT('a') + 10;
        }
        else
        {
            CHR(E_INVALID_DATA);
        }

        chVal += (chTemp << (CHAR)(chIndex*4));
        chIndex++;

    }

    *pchData = chVal;

Error:
    return hr;

}


/// \brief converts a hex string to a DWORD value
///
/// \param pDword pointer to a DWORD to receive the value
/// \param tzHex pointer to a TCHAR array containing the hex string
/// \param fIgnoreTrailingChars bool value indicating how to handle trailing space
///
/// \return HRESULT
///     - S_OK: success
///     - E_*: Other error codes
_inline HRESULT hex2dword(DWORD *pDword, const TCHAR *tzHex, BOOL fIgnoreTrailingChars)
{
    HRESULT hr = S_OK;
    
    DWORD dwVal = 0;
    DWORD dwTmp = 0;
    int iLen;
    int nChars = 0;
    const TCHAR *sz;
    const TCHAR *szStart;
    int i;
    
    CPP(tzHex);

    iLen = _tcslen(tzHex);
    CBREx(0<iLen, E_INVALID_DATA);

    szStart = tzHex;
    sz = &tzHex[iLen-1];

    //
    // remove "0x" if prepended
    //
    if(iLen > 2)
    {
        if(tzHex[0]==TEXT('0') && tzHex[1]==TEXT('x'))
        {
            iLen -= 2;
            szStart += 2;
        }
    }

    if(fIgnoreTrailingChars)
    {
        // find the end (starting point)
        for(i=0; i<iLen; ++i)
        {
            if(!ISHEX(szStart[i]))
            {
                iLen = i;
                break;
            }
        }
        CBREx(iLen>0, E_INVALID_DATA);
        sz = &tzHex[iLen-1];
    }
    else
    {
        CBREx(iLen<9, E_INVALID_DATA);
    }

    for(i=0; i<iLen; ++i, --sz)
    {
        if(TEXT('0') <= *sz && *sz <= TEXT('9'))
        {
            dwTmp = *sz - TEXT('0');
        }
        else if(TEXT('A') <= *sz && *sz <= TEXT('F'))
        {
            dwTmp = *sz - TEXT('A') + 10;
        }
        else if(TEXT('a') <= *sz && *sz <= TEXT('f'))
        {
            dwTmp = *sz - TEXT('a') + 10;
        }
        else
        {
            if(fIgnoreTrailingChars && (nChars > 0))
            {
                goto Done;
            }
            CHR(E_INVALID_DATA);
        }

        dwVal += dwTmp * (1<<(i*4));
        
        if(++nChars>8)
        {
            goto Done;
        }
    }

    *pDword = dwVal;

Done:
Error:
    return hr;
}

/// \brief Converts a decimal or hexidecimal value to a DWORD value
///
/// \param pDword desc of pDword
/// \param tzArg desc of tzArg
///
/// \return HRESULT
///     - S_OK: Success
///     - E_INVALIDARG: Invalid parameter was passed
///     - E_INVALID_DATA: The value could not be converted to an int value

_inline HRESULT tcs2dword(DWORD *pDword, const TCHAR *tzArg)
{
    HRESULT hr = S_OK;
    
    DWORD dwVal;
    int iLen;
    TCHAR szBuf[32];
    BOOL fSuccess;
    
    CPP(tzArg);
    iLen = _tcslen(tzArg);

    CBREx(0<iLen, E_INVALIDARG);

    //
    // quick out for decimal zero
    //
    if(1==iLen && *tzArg==TEXT('0'))
    {
        *pDword = 0;
        goto Done;
    }

    //
    // Hexidecimal ?
    //
    if(iLen>2 && tzArg[0]==TEXT('0') && tzArg[1]==TEXT('x'))
    {
        hr = hex2dword(pDword, tzArg, TRUE);
        CHR(hr);
        goto Done;
    }


    dwVal = _ttol(tzArg);

    //
    // atoi returns 0 if the conversion failed
    //
    CBREx(0<dwVal, E_INVALID_DATA);

    //
    // check the conversion - atol can return a valid value
    // even if number value is out of range
    //
    hr = StringCchPrintfW(szBuf, _countof(szBuf), TEXT("%u"), dwVal);
    CHR(hr);
    
    fSuccess = IS_TSTR_EQUAL(tzArg, szBuf);
    CBREx(fSuccess, E_INVALID_DATA);

    *pDword= dwVal;

Done:
Error:
    return hr;
}
/// \brief converts a single hex WCHAR to an int
///
/// \param c WCHAR to be converted.
///
/// \return int
///     - 0-15: successful hex conversion.
///     - -1: c was not a valid hex char
_inline int hex2byte(WCHAR c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    
    return -1;
}

/// \brief converts 2 hex digits into an integer value
///
/// \param pwcHex pointer to a WCHAR array with atleast 2 hex characters
///
/// \return int
///     - 0-255: successful hex conversion.
///     - -1: pwcHex was not a valid hex pair
_inline int hex2byteW(__in const WCHAR * pwcHex)
{
    int a, b;
    
    a = hex2byte(*pwcHex++);
    if (a < 0)
        return -1;
    b = hex2byte(*pwcHex++);
    if (b < 0)
        return -1;
    
    return (a << 4) | b;
}

_inline int hex2byteA(__in const CHAR * pcHex)
{
    int a, b;
    
    a = hex2byte(*pcHex++);
    if (a < 0)
        return -1;
    b = hex2byte(*pcHex++);
    if (b < 0)
        return -1;
    
    return (a << 4) | b;
}

/// \brief converts 4 hex digits into a WORD value
///
/// \param pcHex pointer to a CHAR array with atleast 4 hex characters
///
/// \return WORD
///     - 0-65535: successful hex conversion.
_inline WORD hex2word(__in LPCSTR pcHex)
{
    WORD wRet = 0;
    int a, b, c, d;
    a = b = c = d = 0;
    
    a = hex2byte(*pcHex++);
    if (a < 0)
    {
        goto Done;
    }
    b = hex2byte(*pcHex++);
    if (b < 0)
    {
        wRet = (WORD) a;
        goto Done;
    }
    c = hex2byte(*pcHex++);
    if (c < 0)
    {
        wRet = (WORD)((a<<4) | b);
        goto Done;
    }
    d = hex2byte(*pcHex++);
    if (d < 0)
    {
        wRet = (WORD)((a<<8) | (b<<4) | c);
        goto Done;
    }
    wRet = (WORD)((a<<12) | (b<<8) | (c<<4) | d);

Done:
    return wRet;
}
/// \brief converts upto 8 hex chars into a DWORD
///
/// \param pcHex pointer to a CHAR array with at least 1 hex char
///
/// \return DWORD
///     - any DWORD value is possible
_inline DWORD hex2dword2(__in LPCSTR pcHex)
{
    DWORD dwRet = 0;
    int val, i;

    for(i=0; i<8; ++i)
    {
        if(!*pcHex)
            break;
        
        val =  hex2byte(*pcHex++);
        if(val < 0)
            break;

        dwRet = (dwRet << 4) + (BYTE)val;
    }
    return dwRet;
}

/// \brief converts a hex WCHAR array into an array of bytes
///
/// \param pwzHex WCHAR string to be converted
/// \param pbBuf desc of paramN
/// \param pcBuf desc of paramN
///
/// \return BOOL
///     - TRUE: the entire specified string was converted successfully
///     - FALSE: a null or an invalid hex character was encountered before pcBuf count
_inline BOOL hexstr2bin(__in const WCHAR *pwzHex, __in BYTE *pbBuf, __inout int *pcBuf)
{
    
    int i, a;
    const WCHAR *ipos = pwzHex;
    BYTE *opos = pbBuf;

    *pcBuf = 0;
    
    for (i = 0; *ipos; i++) 
    {
        a = hex2byteW(ipos);
        if (a < 0)
            return FALSE;
        *opos++ = (BYTE)a;
        ipos += 2;
        *pcBuf += 1;
    }

    return (*pcBuf > 0);
}

/// \brief FlipBytes
///
/// \param lpvData data to flip
/// \param dwDataSize length of data
///
_inline void FlipBytes( __inout LPVOID lpvData, DWORD dwDataSize )
{
    BYTE bTemp, *pbBuffer = (BYTE*)lpvData;
    DWORD dwIdx = 0;

    if(pbBuffer)
    {
        while(dwIdx < dwDataSize/2)
        {
            bTemp = pbBuffer[dwIdx];
            pbBuffer[dwIdx] = pbBuffer[dwDataSize-1-dwIdx];
            pbBuffer[dwDataSize-1-dwIdx] = bTemp;
            
            dwIdx++;
        }
    }
    return;

}

/// \brief IpStringToDword
///
/// \param szAddress string value
/// \param pdwValue dword value
///
/// \return HRESULT
///     - E_INVALIDARG: and other HRESULTS
_inline HRESULT IpStringToDword( __in LPCSTR szAddress, __out PDWORD pdwValue )
{
    HRESULT hr = S_OK;
    LPSTR szToken = NULL;
    LPSTR szNextToken = NULL;
    DWORD dwOctetCount = 0;
    DWORD dwCurrentOctet;
    DWORD dwRetval = 0;

    const DWORD dwMaxString = 32;
    CHAR szValue[32];

    CPP(szAddress);
    CPP(pdwValue);

    hr = StringCchCopyA(szValue, dwMaxString, szAddress);
    CHR(hr);

    szToken = strtok_s(szValue, ".", &szNextToken);

    while(szToken && dwOctetCount < 5)
    {
        hr = cs2dw(&dwCurrentOctet, szToken);
        CHR(hr);

        dwOctetCount++;

        dwRetval = dwRetval << 8;
        dwRetval += dwCurrentOctet;

        szToken = strtok_s(NULL, ".", &szNextToken);
    }

    //ipv4 address found.
    CBR(dwOctetCount == 4);

    //
    *pdwValue = dwRetval;

Error:

    return hr;
}

/// \brief byteArrayToStringA
///
///     If the return string is non-NULL then it must be LocalFree-ed when no
///     longer needed.
///
/// \param pData data to convert
/// \param cData length of pData
///
/// \return CHAR *
_inline CHAR * byteArrayToStringA(BYTE * pData, DWORD cData)
{
    BYTE *p = pData;
    DWORD i,j;
    CHAR * pStr = (char *)LocalAlloc(LMEM_FIXED, (2*cData + 1));

    if(!pStr)
    {
        return NULL;
    }

    for(i=0, j=0; i<cData; ++i, ++p)
    {
        // high nibble
        BYTE b = ((*p & 0xf0) >> 4);
        pStr[j++] = b + ((b < 10) ? '0' : ('a'-10));
        
        // low nibble
        b = (*p & 0x0f);
        pStr[j++] = b + ((b < 10) ? '0' : ('a'-10));
    }
    // zero terminate
    pStr[j] = '\0';
    
    return pStr;
}

/// \brief byteArrayToStringW
///
///     If the return string is non-NULL then it must be LocalFree-ed when no
///     longer needed.
///
/// \param pData data to convert
/// \param cData length of pData
///
/// \return WCHAR *
_inline WCHAR * byteArrayToStringW(BYTE * pData, DWORD cData)
{
    BYTE *p = pData;
    DWORD i,j;
    WCHAR * pStr = (WCHAR *)LocalAlloc(LMEM_FIXED, (2*cData + 1)*sizeof(WCHAR));

    if(!pStr)
    {
        return NULL;
    }

    for(i=0, j=0; i<cData; ++i, ++p)
    {
        // high nibble
        BYTE b = ((*p & 0xf0) >> 4);
        pStr[j++] = b + ((b < 10) ? L'0' : (L'a'-10));
        
        // low nibble
        b = (*p & 0x0f);
        pStr[j++] = b + ((b < 10) ? L'0' : (L'a'-10));
    }
    // zero terminate
    pStr[j] = L'\0';
    
    return pStr;
}

#endif _CONVERSIONS_H_
