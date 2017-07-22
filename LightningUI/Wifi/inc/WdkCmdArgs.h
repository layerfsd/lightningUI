/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkCmdArgs.h#1 $ */
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
/// \brief      Command line parser.
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2006-11-30
//-----------------------------------------------------------------------------
/// \file

#ifndef _CMDARGS_H_
#define _CMDARGS_H_

#include <WdkVersion.h>
#include <WdkMacros.h>
#include <WdkConversions.h>

#include <strsafe.h>

#define CMDARG_NOTFOUND -1

/// \brief Returns a the position of the switch in a list of arguments
///
///     Returns a valid result for for switches preceeded by either "/" or a "-"
///
/// \param tzSwitch The switch to look for
/// \param argc Number of arguments in the list
/// \param argv Argument list
///
/// \return -1 if the switch is not present, otherwise the switch position is returned.
_inline int CmdArg_GetSwitchPos(const TCHAR *tzSwitch, const int argc, TCHAR *argv[])
{
    if(!tzSwitch || !argv || argc < 1)
    {
        return CMDARG_NOTFOUND;
    }
    
    for( int i=0; i<argc; ++i)
    {
        if(argv[i][0] == TEXT('-') 
            || argv[i][0] == TEXT('/'))
        {
            if(IS_TSTR_EQUAL(tzSwitch, &argv[i][1]))
            {
                return i;
            }
        }
    }
    return CMDARG_NOTFOUND;
}


/// \ Checks a list of arguments for a given switch 
///
/// \param tzSwitch The switch to look for
/// \param argc Number of arguments in the list
/// \param argv Argument list
///
/// \return  BOOL
///     - TRUE if the switch is present.
///     - FALSE if the switch is not present 
_inline BOOL CmdArg_HasSwitch(const TCHAR *tzSwitch, const int argc, TCHAR *argv[])
{
    return (CmdArg_GetSwitchPos(tzSwitch, argc, argv) != CMDARG_NOTFOUND);
}


/// \ Returns the int value for an argument following a given switch 
///
/// \param pInt pointer to an int to hold the value 
/// \param tzSwitch The switch to look for
/// \param argc Number of arguments in the list
/// \param argv Argument list
///
/// \return  HRESULT 
///     - S_OK  The conversion was successful
///     - E_INVALIDARG One of the parameters is invalid
///     - E_*   Other HRESULT
_inline HRESULT CmdArg_GetInt(int * pInt, const TCHAR *tzSwitch, const int argc, TCHAR *argv[])
{
    HRESULT hr = S_OK;
    int iValue;
    int iArg;

    CBREx(pInt && tzSwitch && argv, E_INVALIDARG);

    iArg = CmdArg_GetSwitchPos(tzSwitch, argc, argv);
    CBREx(iArg != CMDARG_NOTFOUND, E_PATH_NOT_FOUND);
    CBREx(++iArg < argc, E_INVALID_DATA);

    hr = tcs2int(&iValue, argv[iArg]);
    CHR(hr);

    *pInt = iValue;

Error:    
    return hr;
}

/// \ Returns the DWORD value for an argument following a given switch 
///
/// \param pDword pointer to a DWORD to hold the value 
/// \param tzSwitch The switch to look for
/// \param argc Number of arguments in the list
/// \param argv Argument list
///
/// \return  HRESULT 
///     - S_OK  The conversion was successful
///     - E_INVALIDARG One of the parameters is invalid
///     - E_*   Other HRESULT
_inline HRESULT CmdArg_GetDword(DWORD * pDword, const TCHAR *tzSwitch, const int argc, TCHAR *argv[])
{
    HRESULT hr = S_OK;
    DWORD dwValue;
    int iArg;
    
    CBREx(pDword && tzSwitch && argv, E_INVALIDARG);

    iArg = CmdArg_GetSwitchPos(tzSwitch, argc, argv);
    CBREx(iArg != CMDARG_NOTFOUND, E_PATH_NOT_FOUND);
    CBREx(++iArg < argc, E_INVALID_DATA);

    hr = tcs2dword(&dwValue, argv[iArg]);
    CHR(hr);

    *pDword = dwValue;

Error:
    return hr;    
}

/// \ Returns the value of an argument following a given switch 
///
/// \param szBuf pointer to hold the value 
/// \param cch maximum length of the buffer in characters
/// \param tzSwitch The switch to look for
/// \param argc Number of arguments in the list
/// \param argv Argument list
///
/// \return  HRESULT 
///     - S_OK  The conversion was successful
///     - E_INVALIDARG One of the parameters is invalid
///     - E_*   Other HRESULT
_inline HRESULT CmdArg_GetString(TCHAR * szBuf, const DWORD cch, const TCHAR *tzSwitch, const int argc, TCHAR *argv[])
{
    HRESULT hr = S_OK;
    int iArg;
    
    CBREx(szBuf && tzSwitch && argv, E_INVALIDARG);

    iArg = CmdArg_GetSwitchPos(tzSwitch, argc, argv);
    CBREx(iArg != CMDARG_NOTFOUND, E_PATH_NOT_FOUND);
    CBREx(++iArg < argc, E_INVALID_DATA);

    hr = StringCchCopy(szBuf, cch, argv[iArg]);
    CHR(hr);

Error:
    return hr;
}

/// \ Returns the boolean value for an argument following a given switch 
///
/// \param pBool pointer to a BOOL to hold the value 
/// \param tzSwitch The switch to look for
/// \param argc Number of arguments in the list
/// \param argv Argument list
///
/// \return  HRESULT 
///     - S_OK  The conversion was successful
///     - E_INVALIDARG One of the parameters is invalid
///     - E_*   Other HRESULT
_inline HRESULT CmdArg_GetBool(BOOL * pBool, const TCHAR *tzSwitch, const int argc, TCHAR *argv[])
{
    HRESULT hr = S_OK;
    DWORD dwValue = 0;
    TCHAR szBool[32];    
    BOOL fValue = FALSE;
    
    hr = CmdArg_GetDword(&dwValue, tzSwitch, argc, argv);
    if(SUCCEEDED(hr))
    {
        if(dwValue < 2)
        {
            *pBool = BOOLIFY(dwValue);
            goto Done;
        }
        CHR(E_INVALIDARG);
    }
    
    hr = CmdArg_GetString(szBool, countof(szBool), tzSwitch, argc, argv);
    if(SUCCEEDED(hr))
    {
        if(0 == szBool[1])
        {
            if(szBool[0] == TEXT('t')
                || szBool[0] == TEXT('T'))
            {
                fValue = TRUE;
                goto Done;
            }

            if(szBool[0] == TEXT('f')
                || szBool[0] == TEXT('F'))
            {
                goto Done;
            }
        }
        else
        {
            if(IS_TSTR_EQUAL(szBool, TEXT("TRUE")))
            {
                fValue = TRUE;
                goto Done;
            }

            if(IS_TSTR_EQUAL(szBool, TEXT("FALSE")))
            {
                goto Done;
            }
        }
    }

    CHR(E_INVALIDARG);

Done:
    *pBool = fValue;
    
Error:
    return hr;
}

#endif _CMDARGS_H_
