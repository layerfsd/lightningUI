/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkRegEdit.h#1 $ */
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
/// \brief      Registry access helper class.  
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2010-10-11
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKREGEDIT_H_
#define _WDKREGEDIT_H_

#include "windef.h"

#include <WdkVersion.h>
#include <WdkCoreDef.h>

/// \brief Supported registry value types
typedef enum _WdkRegValueType
{
    WdkRegValueNotSupported    = -1,            ///< Unsupported registry value type
    WdkRegValueString          = REG_SZ,        ///< String registry value type
    WdkRegValueDword           = REG_DWORD,     ///< DWORD registry value type
    WdkRegValueMultiString     = REG_MULTI_SZ,  ///< Multi-String registry value type
    WdkRegValueBinary          = REG_BINARY,    ///< Binary registry value type
} WdkRegValueType;

class WdkCore_API WdkRegEdit
{
public:
    
    /// \brief Creates a RegEdit Object
    WdkRegEdit( void );

    /// \brief Creates a RegEdit Object using the specified HKEY.
    ///
    /// \param hRoot Root HKEY
    /// \param fCreate If TRUE, attempt to create the key if it does not exist, open otherwise.
    WdkRegEdit(HKEY hRoot, BOOL fCreate=TRUE);

    /// \brief Creates a RegEdit Object using the specified HKEY.
    ///
    /// \param hRoot Root HKEY
    /// \param pwzSubKey Subkey to open or create
    /// \param fCreate If TRUE, attempt to create the HKEY if it does not exist, open otherwise.
    WdkRegEdit(HKEY hRoot, __in_opt LPCWSTR pwzSubKey, BOOL fCreate=TRUE);

    /// \brief Destructor.
    ~WdkRegEdit(void);

    /// c++ exception caught and NULL returned
    static WdkRegEdit* TryCreate(HKEY hRoot, __in LPCWSTR wzKey, BOOL fCreate=TRUE);

    /// Returns the name of the registry override DLL.
    /// Returns E_DLL_NOT_FOUND if override DLL not used.
    HRESULT GetOverrideDll(__out_opt LPWSTR pwzDllName, DWORD cchDllName, __out_opt PDWORD pdwFlags);

    /// Create a registry key
    BOOL Create(HKEY hRoot, __in_opt LPCWSTR pwzKey = NULL);

    /// Open a registry key
    BOOL Open(HKEY hRoot, __in_opt LPCWSTR pwzKey = NULL);

    /// Closes the currently open key
    VOID Close( void );

    /// Returns the current open key
    HKEY GetKey( void ) { return m_hKey; }

    /// Opens/Creates a registry sub key
    BOOL SetKey( __in LPCWSTR pwzSubKey, BOOL fCreate, BOOL fSetAsRoot );

    /// Creates a registry sub key, using the current key as the parent
    BOOL SetKey( __in LPCWSTR pwzSubKey ) { return SetKey(pwzSubKey, TRUE, FALSE); };

    /// Goes up one level. Returns FALSE if at the rott key
    BOOL Up( void );

    /// Goes to the base key
    VOID Root( void );

    /// Flushes the currently open key
    VOID Flush( void );

    /// Returns TRUE if a key is currently open
    BOOL IsOpen( void );

    /// Deletes value in the current key
    BOOL Delete( __in LPCWSTR wzValue );

    /// Clears all subkeys and values from the currently open key.
    BOOL Clear( void );

    /// Returns the value type for the requested value.
    BOOL GetValueType( __in LPCWSTR wzValue, __in_opt WdkRegValueType *pType);

    /// Returns a BOOL for the requested value
    BOOL GetBool( __in LPCWSTR pwzValue, __out PBOOL pdwData);

    /// Returns a BOOL for the requested value
    BOOL GetDword( __in LPCWSTR pwzValue, __out PDWORD pdwData);

    /// Returns the string data for the requested value
    /// Both the string and multistring values are returned using
    /// this function.
    BOOL GetValue(__in LPCWSTR pwzValue, __out LPWSTR pwzData, DWORD cchData);

    /// Returns the string data for the requested value, the number of bytes used,
    /// and if the returned value is a multi-string.
    /// Both the string and multistring values are returned using
    /// this function. 
    BOOL GetValue(__in LPCWSTR pwzValue, __out LPWSTR pwzData, DWORD cchData, __out_opt DWORD *pcchUsed, __out_opt BOOL *pfMultiString);

    /// Returns binary values
    BOOL GetBinary(__in LPCWSTR pwzValue, __out BYTE* pData, DWORD cbData);

    /// Returns a BOOL for the requested value
    BOOL SetBool( __in LPCWSTR pwzValue, BOOL fBool);

    /// Returns a BOOL for the requested value
    BOOL SetDword( __in LPCWSTR pwzValue, DWORD dwData);

    /// Sets the string data for the requested value
    BOOL SetValue(__in LPCWSTR pwzValue, __in LPCWSTR wzData);
    
    /// Sets the multi-string data for the requested value
    BOOL SetValue(__in LPCWSTR pwzValue, __in LPCWSTR * pwzData, DWORD cStrings);

    /// Sets the (multi)string data for the requested value
    /// cch is the character count, not the byte count
    /// the cch count should include the null terminator if needed.
    BOOL SetValue(__in LPCWSTR pwzValue, __in LPCWSTR wzData, DWORD cch, BOOL fMultiString);
    
    /// Sets binary data
    BOOL SetBinary(__in LPCWSTR pwzValue, BYTE *pbBinary, DWORD cBytes);

    /// Queries for the number of sub keys of the current open key
    BOOL NumSubKeys(__out PDWORD pdwCount, __in_opt PDWORD pdwMaxKeyLen = NULL );

    /// Enumerate the subkeys of the current open key
    BOOL EnumKeyEx( DWORD dwIndex, __out LPWSTR pwzSubkey, __inout DWORD *pcchSubkey );

    /// Enumerate the values of the current open key
    BOOL EnumValue( DWORD dwIndex, __out LPWSTR pwzValueName, __inout DWORD *pcchValueName, __out_opt PDWORD pdwType, __out_opt PBYTE pbData, __out_opt PDWORD pcbData );

    /// Sets a single BOOL
    static BOOL SetBool(HKEY hRoot, __in LPCWSTR wzKey, __in LPCWSTR wzValue, BOOL fBool);

    /// Sets a single DWORD
    static BOOL SetDword(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, DWORD dwData);

    /// Sets a single string
    static BOOL SetValue(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, __in LPCWSTR pwzData);

    
    /// Sets binary data
    static BOOL SetBinary(HKEY hRoot, __in LPCWSTR wzKey, __in LPCWSTR pwzValue, BYTE *pbBinary, DWORD cBytes);

    /// Gets a single BOOL
    static BOOL GetBool(HKEY hRoot, __in LPCWSTR wzKey, __in LPCWSTR wzValue, __out PBOOL pfBool);

    /// Gets a string
    static BOOL GetDword(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, __out PDWORD pdwData);

    /// Gets a single string
    static BOOL GetValue(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue, __out LPWSTR pwzData, DWORD cch);

    /// Deletes the registry key
    static BOOL DeleteKey(HKEY hRoot, __in LPCWSTR pwzKey);

    /// Deletes a value
    static BOOL DeleteValue(HKEY hRoot, __in LPCWSTR pwzKey, __in LPCWSTR pwzValue);

    /// Returns true if the key exists
    static BOOL KeyExists(HKEY hRoot, __in LPCWSTR pwzKey);

private:
    
    VOID Swap(__inout PHKEY ph1, __inout PHKEY ph2)
    {
        HKEY hTemp = *ph1;
        *ph1 = *ph2;
        *ph2 = hTemp;
    }
    
    HKEY        m_hKey;
    WdkRegEdit  *m_pParent;
    
};

#endif // _QLX_REGEDIT_H_
