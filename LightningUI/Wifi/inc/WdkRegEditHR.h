/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkRegEditHR.h#1 $ */
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
/// \brief      Registry access helper class (HRESULT version).  
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2010-10-11
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_REGEDITHR_H_
#define _WDK_REGEDITHR_H_

#include <windef.h>

#include <WdkVersion.h>
#include <WdkCoreDef.h>
#include <WdkRegEdit.h>
#include <WdkHresult.h>

class WdkRegEditHR
{
public:
    /// \brief Creates a RegEditHR Object
    WdkRegEditHR( void )
    {
    }
    
    /// \brief Creates a RegEditHR Object using the specified HKEY.
    ///
    /// \param hRoot Root HKEY
    WdkRegEditHR(HKEY hRoot)
    {
        m_reg.Create(hRoot, NULL);
    }

    /// \brief Creates a RegEditHR Object using the specified HKEY.
    ///
    /// \param hRoot Root HKEY
    /// \param pwzSubKey Subkey to open or create
    WdkRegEditHR(HKEY hRoot, __in LPCWSTR pwzSubKey)
    {
        m_reg.Create(hRoot, pwzSubKey);
    }

    /// \brief Destructor.
    ~WdkRegEditHR(void)
    {
    }

    /// Returns the name of the registry override DLL.
    /// Returns E_DLL_NOT_FOUND if override DLL not used.
    HRESULT GetOverrideDll(__out_opt LPWSTR pwzDllName, DWORD cchDllName, __out_opt PDWORD pdwFlags) 
    { 
        return m_reg.GetOverrideDll(pwzDllName, cchDllName, pdwFlags); 
    }
    
    /// Create a registry key
    HRESULT Create(HKEY hRoot, __in LPCWSTR pwzKey = NULL )
    {
        BOOL fSuccess = m_reg.Create(hRoot, pwzKey);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Open a registry key
    HRESULT Open(HKEY hRoot, __in_opt LPCWSTR pwzKey = NULL )
    {
        BOOL fSuccess = m_reg.Open(hRoot, pwzKey);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }
    
    /// Closes the currently open key
    VOID    Close( void ) { m_reg.Close(); }

    /// Returns the current key
    HKEY    GetKey( void ) { return m_reg.GetKey(); }

    /// Opens/Creates a subkey
    HRESULT SetKey( __in LPCWSTR pwzSubKey, BOOL fCreate, BOOL fSetAsRoot )
    {
        BOOL fSuccess = m_reg.SetKey(pwzSubKey, fCreate, fSetAsRoot);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Creates a subkey, keeps the current key as the parent
    HRESULT SetKey( __in LPCWSTR pwzSubKey )
    {
        BOOL fSuccess = m_reg.SetKey(pwzSubKey);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns to the parent, or previous subkey
    HRESULT Up( void )
    {
        BOOL fSuccess = m_reg.Up();
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns to the root key
    VOID    Root( void ) { m_reg.Root(); }
    
    /// Flushes the currently open key
    VOID    Flush( void )  { m_reg.Flush(); }

    /// Returns TRUE if a key is currently open
    BOOL    IsOpen( void ) { return m_reg.IsOpen(); }

    /// Deletes value in the current key
    HRESULT Delete(__in LPCWSTR pwzValue)
    {
        BOOL fSuccess = m_reg.Delete(pwzValue);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Clears all subkeys and values from the currently open key.
    HRESULT Clear( void )
    {
        BOOL fSuccess = m_reg.Clear();
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns the value type for the requested value.
    HRESULT GetValueType(__in LPCWSTR pwzValue, __in_opt WdkRegValueType *pType)
    {
        BOOL fSuccess = m_reg.GetValueType(pwzValue, pType);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns the BOOL data for the requested value
    HRESULT GetBool(__in LPCWSTR pwzValue, __out PBOOL pfBool)
    {
        BOOL fSuccess = m_reg.GetBool(pwzValue, pfBool);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns the DWORD data for the requested value
    HRESULT GetDword(__in LPCWSTR pwzValue, __out PDWORD pdwData)
    {
        BOOL fSuccess = m_reg.GetDword(pwzValue, pdwData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns the string data for the requested value
    /// Both the string and multistring values are returned using
    /// this function.
    HRESULT GetValue(__in LPCWSTR pwzValue, __out LPWSTR pwzData, DWORD cch)
    {
        BOOL fSuccess = m_reg.GetValue(pwzValue, pwzData, cch);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns the string data for the requested value, the number of bytes used,
    /// and if the returned value is a multi-string.
    /// Both the string and multistring values are returned using
    /// this function. 
    HRESULT GetValue(__in LPCWSTR pwzValue, __out LPWSTR pwzData, DWORD cchData, __out_opt DWORD *pcchUsed, __out_opt BOOL *pfMultiString)
    {
        BOOL fSuccess = m_reg.GetValue(pwzValue, pwzData, cchData, pcchUsed, pfMultiString);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Returns binary values
    HRESULT GetBinary(__in LPCWSTR pwzValue, __out BYTE* pData, DWORD cbData)
    {
        BOOL fSuccess = m_reg.GetBinary(pwzValue, pData, cbData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Sets the BOOL data for the requested value
    HRESULT SetBool(__in LPCWSTR pwzValue, BOOL fBool)
    {
        BOOL fSuccess = m_reg.SetBool(pwzValue, fBool);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Sets the DWORD data for the requested value
    HRESULT SetDword(__in LPCWSTR pwzValue, DWORD dwData)
    {
        BOOL fSuccess = m_reg.SetDword(pwzValue, dwData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Sets the string data for the requested value
    HRESULT SetValue(__in LPCWSTR pwzValue, __in LPCWSTR pwzData)
    {
        BOOL fSuccess = m_reg.SetValue(pwzValue, pwzData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }
    
    /// Sets the multi-string data for the requested value
    HRESULT SetValue(__in LPCWSTR pwzValue, __in LPCWSTR * pwzData, DWORD cStrings)
    {
        BOOL fSuccess = m_reg.SetValue(pwzValue, pwzData, cStrings);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Sets the (multi)string data for the requested value
    /// cch is the character count, not the byte count
    /// the cch count should include the null terminator if needed.
    HRESULT SetValue(__in LPCWSTR pwzValue, __in LPCWSTR wzData, DWORD cch, BOOL fMultiString)
    {
        BOOL fSuccess = m_reg.SetValue(pwzValue, wzData, cch, fMultiString);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }
    
    /// Sets binary data
    HRESULT SetBinary(__in LPCWSTR pwzValue, __in BYTE *pbBinary, DWORD cBytes)
    {
        BOOL fSuccess = m_reg.SetBinary(pwzValue, pbBinary, cBytes);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }
    
    /// Queries for the number of sub keys of the current open key
    HRESULT NumSubKeys(__out PDWORD pdwCount, __in_opt PDWORD pdwMaxKeyLen = NULL )
    {
        BOOL fSuccess = m_reg.NumSubKeys(pdwCount, pdwMaxKeyLen);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Enumerate the subkeys of the current open key
    HRESULT EnumKeyEx( DWORD dwIndex, __out LPWSTR pwzSubkey, __inout DWORD *pcchSubkey = NULL )
    {
        BOOL fSuccess = m_reg.EnumKeyEx( dwIndex, pwzSubkey, pcchSubkey );
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Enumerate the values of the current open key
    HRESULT EnumValue( DWORD dwIndex, __out LPWSTR pwzValueName, __inout DWORD *pcchValueName, __out_opt PDWORD pdwType, __out_opt PBYTE pbData, __out_opt PDWORD pcbData )
    {
        BOOL fSuccess = m_reg.EnumValue( dwIndex, pwzValueName, pcchValueName, pdwType, pbData, pcbData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Sets a single BOOL
    static HRESULT SetBool(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, BOOL fBool)
    {
        BOOL fSuccess = WdkRegEdit::SetBool(hRoot, pwzKey, pwzValue, fBool);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Sets a single DWORD
    static HRESULT SetDword(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, DWORD dwData)
    {
        BOOL fSuccess = WdkRegEdit::SetDword(hRoot, pwzKey, pwzValue, dwData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Sets a single string
    static HRESULT SetValue(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, __out LPCWSTR wzData)
    {
        BOOL fSuccess = WdkRegEdit::SetValue(hRoot, pwzKey, pwzValue, wzData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    
    /// Sets binary data
    static HRESULT SetBinary(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, __in BYTE *pbBinary, DWORD cBytes)
    {
       BOOL fSuccess = WdkRegEdit::SetBinary(hRoot, pwzKey, pwzValue, pbBinary, cBytes);
       return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Gets a single BOOL
    static HRESULT GetBool(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, PBOOL pfBool)
    {
        BOOL fSuccess = WdkRegEdit::GetBool(hRoot, pwzKey, pwzValue, pfBool);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Gets a single DWORD
    static HRESULT GetDword(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, __out PDWORD pdwData)
    {
        BOOL fSuccess = WdkRegEdit::GetDword(hRoot, pwzKey, pwzValue, pdwData);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Gets a single string
    static HRESULT GetValue(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, __out LPWSTR pwzData, DWORD cch)
    {
        BOOL fSuccess = WdkRegEdit::GetValue(hRoot, pwzKey, pwzValue, pwzData, cch);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Deletes the registry key
    static HRESULT DeleteKey(HKEY hRoot, __in LPCWSTR pwzKey)
    {
        BOOL fSuccess = WdkRegEdit::DeleteKey(hRoot, pwzKey);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

    /// Deletes the registry key
    static HRESULT DeleteValue(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue)
    {
        BOOL fSuccess = WdkRegEdit::DeleteValue(hRoot, pwzKey, pwzValue);
        return fSuccess ? S_OK : E_LASTWIN32ERROR;
    }

private:
    
    WdkRegEdit m_reg;
    
};

#endif // _WDK_REGEDITHR_H_
