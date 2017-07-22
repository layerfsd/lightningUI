/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkStringUtil.h#1 $ */
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
/// \brief      String utility functions
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-01-11
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_STRINGUTIL_H_
#define _WDK_STRINGUTIL_H_
#pragma once

#include <WdkVersion.h>
#include <WdkCoreDef.h>
#include <WdkMacros.h>
#include <WdkEhm.h>

#define MAX_UNICODE_CONVERSION_STRING_LEN   4096
#define DEFAULT_MAX_ALLOC_STRING_LEN        1024

/// \brief Converts a multibyte string to unicode
///
/// \param paszIn multibyte string to be converted
/// \param unCodePage codepage
/// \param pwzOut buffer to contain the converted string
/// \param cchOut character count of the buffer
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT MultibyteToUnicode(LPCSTR paszIn, UINT unCodePage, LPWSTR pwzOut, DWORD cchOut);

/// \brief Decodes a URL encoded ansi string inplace
///
/// \param szValue string to be decoded
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT UrlDecodeA(LPSTR szValue);

/// \brief allocate and copy the string into a new string
///
/// This function will take a wzSrc string as input and will cleanup
/// any existing memory at pwzNew and then allocate a new copy of
/// source for you.  Repeated calling of this function with the same
/// variables will not cause any memory leaks.
///
/// \param ppwzNew pointer to a wstring that will be allocated and
///         will receive the copy
/// \param pwzSrc wstring to be copied
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT QlxCopyStringW(LPWSTR *ppwzNew, LPCWSTR pwzSrc);

/// \brief allocate and copy the string into a new string
///
/// This function will take a szSrc string as input and will cleanup
/// any existing memory at pszNew and then allocate a new copy of
/// source for you.  Repeated calling of this function with the same
/// variables will not cause any memory leaks.
///
/// \param ppszNew pointer to a wstring that will be allocated and
///         will receive the copy
/// \param pszSrc wstring to be copied
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT QlxCopyStringA(LPSTR *ppszNew, LPCSTR pszSrc);

/// \brief unicode/ansi macro for QlxCopyStringW/QlxCopyStringA
#ifdef UNICODE
#define QlxCopyString QlxCopyStringW
#else
#define QlxCopyString QlxCopyStringA
#endif

/// \brief Convert a widestring into a utf8 encoded ansi string
///
/// \param wzIn wstring to be converted
/// \param cchIn DWORD character count of the string to be converted.
/// \param szOut string to receive the utf encoded result string
/// \param cchOut DWORD character count of the size of szOut
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_C_API HRESULT WideCharToUTF8Ex(LPCWSTR wzIn, DWORD cchIn, LPSTR szOut, DWORD cchOut);

/// \brief Convert a widestring into a utf8 encoded ansi string
///
/// \param paszIn zero terminated wstring to be converted
/// \param wzOut string to receive the utf encoded result string
/// \param cchOut DWORD character count of the size of szOut
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_C_API HRESULT WideCharToUTF8(LPCWSTR paszIn, LPSTR wzOut, DWORD cchOut);

/// \brief Convert a widestring into a utf8 encoded ansi string
///
/// \param wzIn zero terminated wstring to be converted
/// \param pszOut pointer to a string to be allocated and then 
///         receive the utf encoded result string
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT WideCharToUTF8_Alloc(LPCWSTR wzIn, LPSTR *pszOut);

/// \brief Convert a utf8 (or ascii) encoded string into wide string
///
/// \param szIn string to be converted
/// \param cchIn DWORD character count of the string to be converted.
/// \param wzOut wstring to receive the result
/// \param cchOut DWORD character count of the size of wzOut
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_C_API HRESULT UTF8ToWideCharEx(__in LPCSTR szIn, DWORD cchIn, __out LPWSTR wzOut, DWORD cchOut);

/// \brief Convert a utf8 (or ascii) encoded string into wide string
///
/// \param szIn string to be converted
/// \param wzOut wstring to receive the result
/// \param cchOut DWORD character count of the size of wzOut
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_C_API HRESULT UTF8ToWideChar(__in LPCSTR szIn, __out LPWSTR wzOut, DWORD cchOut);

/// \brief Convert a utf8 (or ascii) encoded string into wide string
///
/// \param szIn string to be converted
/// \param pwzOut pointer to a wstring to be allocated and then 
///         receive the result string
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WdkCore_API HRESULT UTF8ToWideChar_Alloc(__in LPCSTR szIn, __out LPWSTR *pwzOut);

/// \brief Convert an HRESULT to a string
///
/// \param hr value to be converted
///
/// \return pointer to a constant string
///
WdkCore_API LPCWSTR HR2String(HRESULT hr);

/// \brief Convert an HRESULT to an ASCII string
///
/// \param hr value to be converted
///
/// \return pointer to a constant string
///
WdkCore_API LPCSTR HR2StringA(HRESULT hr);

/// \brief Splits a string based on a char delimiter
///
/// Use delete[] on the split pointer to free memory.
WdkCore_API HRESULT SplitA(__inout CHAR * pszLine, __in LPCSTR pszDelim, __out LPSTR **pppSplit, __out DWORD *pcSplit, BOOL fTrim);

/// \brief Splits a string based on a string delimiter
///
/// Use delete[] on the split pointer to free memory.
WdkCore_API HRESULT SplitStrDelimA(__inout CHAR * pszLine, __in LPCSTR pszDelim, __out LPSTR **pppSplit, __out DWORD *pcSplit);

#define MAC_2_STRING_FMTA  "%02x%c%02x%c%02x%c%02x%c%02x%c%02x"
#define MAC_2_STRING_FMTW L"%02x%C%02x%C%02x%C%02x%C%02x%C%02x"
#define MAC_2_STRINGA(_sz, _cch, _mac, _sep) StringCchPrintfA(_sz, _cch, MAC_2_STRING_FMTA, _mac[0], _sep, _mac[1], _sep, _mac[2], _sep, _mac[3], _sep, _mac[4], _sep, _mac[5])
#define MAC_2_STRINGW(_sz, _cch, _mac, _sep) StringCchPrintfW(_sz, _cch, MAC_2_STRING_FMTW, _mac[0], _sep, _mac[1], _sep, _mac[2], _sep, _mac[3], _sep, _mac[4], _sep, _mac[5])

/// \brief Convert a SERVICE_STATE value to a string
///
/// \param dwServiceState value to be converted
///
/// \return pointer to a constant string
///
WdkCore_API LPCWSTR ServiceState2String(DWORD dwServiceState);

/// \brief Convert command line argumentsto a string
///
/// \param pwzCmdline buffer
/// \param cchCmdline Length of the buffer in characters
/// \param argc
/// \param argv
/// \param fPathname If TRUE, include the app pathname in the string
///
/// \return HRESULT
///
WdkCore_API HRESULT CmdLineToString(LPWSTR pwzCmdline, DWORD cchCmdline, int argc, WCHAR* argv[], BOOL fPathname );

#ifdef WZC_SUPPORT
/// \brief Convert a WZC notification state value to a string (Supported on WEC 6)
///
/// \param dwWzcState value to be converted
///
/// \return pointer to a constant string
///
WdkCore_API LPCWSTR WzcState2String(DWORD dwWzcState);
#endif 


#endif // _WDK_STRINGUTIL_H_
