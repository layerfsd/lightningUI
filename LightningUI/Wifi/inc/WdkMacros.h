/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkMacros.h#1 $ */
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
/// \brief      WDK common macros.  
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2010-10-08
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_MACROS_H_
#define _WDK_MACROS_H_
#pragma once

#include <WdkVersion.h>

#ifdef DEBUG
#define IS_DEBUG    TRUE;    
#else
#define IS_DEBUG    FALSE;    
#endif

#define BOOLIFY(_v) !!(_v)

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#define MAX(x,y)    ((x) > (y) ? (x) : (y))
#define MIN(x,y)    ((x) < (y) ? (x) : (y))
#define ABS(a) ( ((a)>0) ? (a) : -(a) )

#define MAKE_ULONGLONG(ldw, hdw)    ((ULONGLONG(hdw) << 32) | ((ldw) & 0xffffffff))
#define MAKE_LONG(l, h)             (((DWORD) (l)) | (((DWORD) (h)) << 16L))
#define HI_DWORD(ull)   (DWORD)(ull >> 32)
#define LO_DWORD(ull)   (DWORD)(ull & 0xffffffff)

#define LO_BYTE(w)      ((BYTE)((w) & 0xff))
#define HI_BYTE(w)      ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define MAKE_WORD(l, h)         (((WORD) (l)) | (((WORD) (h)) << 8))

#ifndef countof
#define countof(x)  _countof(x)
#endif

#define MAX_WORD   ((WORD)0xFFFF)

#define MAX_DEVICE_NAME     16

#define ZERO(_var)    ZeroMemory(_var, sizeof(_var))
#define ZERO_SEC(_var) SecureZeroMemory(_var, (SIZE_T)sizeof(_var))

#define SWAP(_type, x,y) {_type t;t=x;x=y;y=t;}

// networking

#define MAC_LEN                     6       ///< Size of a hardware address.
#define IPV4_LEN                    4       ///< Size of an IPv4 address.

#define MAX_MAC_SZ_LEN              18      ///< Maximum allowed length of a zero terminated MAC string  (12:45:78:01:34:67\0).
#define MAX_IPADDR_STRING_LEN       16      ///< Maximum allowed length of a zero terminated IPv4 string  (123.567.901.345\0).
#define MAX_ADAPTER_NAME_LEN        128     ///< Maximum allowed length of an zero terminated ndis interface.
#define MAX_HOSTNAME_LEN            128     ///< Maximum allowed length of a zero terminated hostname.
#define MAX_SSID_LEN                32      ///< Maximum allowed length of a ssid, not including a null terminator.
#define MAX_SSID_SZ_LEN             (MAX_SSID_LEN+1)  ///< Zero termiated max length of an SSID.

// --------------------------------------------------------------------------------------------
// LocalFree, etc. and set to NULL
//
#define DELETEOBJ(_obj_) if(_obj_) { delete (_obj_); (_obj_) = NULL; }
#define DELETEARRAY(_array_) if(_array_) { delete [] (_array_); (_array_) = NULL; }
#define LOCALFREE(_lpmem_) if((_lpmem_)) { (void)LocalFree((_lpmem_)); (_lpmem_) = NULL; }
#define CLOSEHANDLE(_hndl_) if((_hndl_)) { (void)CloseHandle((_hndl_)); (_hndl_) = NULL; }
#define CLOSE_INET_HANDLE(_hndl_) if((_hndl_)) { (void)InternetCloseHandle((_hndl_)); (_hndl_) = NULL; }
#define CLOSEMSGQUEUE(_que_) if((_que_)) { (void)CloseMsgQueue((_que_)); (_que_) = NULL; }
#define DEACTIVATEDEVICE(_hd_) if((_hd_) != INVALID_HANDLE_VALUE && (_hd_)!= NULL) { DeactivateDevice((_hd_)); (_hd_) = NULL; }
#define ALLOCBSTR(_wz) SysAllocString(_wz)
#define REALLOCBSTR(_bstr, _wz) FREEBSTR(_bstr); _bstr = SysAllocString(_wz)
#define FREELIBRARY(_hlib) if((_hlib)) { (void)FreeLibrary(_hlib); (_hlib) = NULL; }
#define FREEBSTR(_bs) if(_bs) { SysFreeString(_bs); _bs = NULL; }
#define RELEASE(_comObj)  if(_comObj) { _comObj->Release(); _comObj=NULL; }
#define DESTROYWINDOW(_hwnd) if(_hwnd) { DestroyWindow(_hwnd); _hwnd=NULL; }
#define REGISTRYCLOSENOTIFICATION(_h) if(_h) { RegistryCloseNotification(_h); _h = NULL; }
#define REGCLOSEKEY(_k) if(_k) { RegCloseKey(_k); _k=NULL; }
#define SYSFREESTRING(_bstr) if(_bstr) {SysFreeString(_bstr); _bstr = NULL; }

// --------------------------------------------------------------------------------------------
/// \brief Returns TRUE if the strings are exactly equal (Case insensitive)
#define IS_TSTR_EQUAL(str1,conststr2) ((_tcsnicmp(str1, conststr2, _tcslen(conststr2)+1)    == 0))
/// \brief Returns TRUE if the strings are exactly equal (Case insensitive)
#define IS_CSTR_EQUAL(str1,conststr2) ((_strnicmp(str1, conststr2, strlen(conststr2)+1)     == 0))
/// \brief Returns TRUE if the strings are exactly equal (Case insensitive)
#define IS_WSTR_EQUAL(str1,conststr2) ((_wcsnicmp(str1, conststr2, wcslen(conststr2)+1)     == 0))

/// \brief Returns TRUE if the strings are exactly equal (Case sensitive)
#define IS_TSTR_EQUAL_CASESENS(str1,conststr2) ((_tcsncmp(str1, conststr2, _tcslen(conststr2)+1)     == 0))
/// \brief Returns TRUE if the strings are exactly equal (Case sensitive)
#define IS_CSTR_EQUAL_CASESENS(str1,conststr2) ((strncmp(str1, conststr2, strlen(conststr2)+1)     == 0))
/// \brief Returns TRUE if the strings are exactly equal (Case sensitive)
#define IS_WSTR_EQUAL_CASESENS(str1,conststr2) ((wcsncmp(str1, conststr2, wcslen(conststr2)+1)     == 0))

// --------------------------------------------------------------------------------------------
/// \brief Returns TRUE if a string starts with the given substring  (Case insensitive)
#define IS_TSTR_STARTS_WITH(str1,conststr2) (_tcsnicmp(str1, conststr2, _tcslen(conststr2)) == 0)
/// \brief Returns TRUE if a string starts with the given substring  (Case insensitive)
#define IS_CSTR_STARTS_WITH(str1,conststr2) (_strnicmp(str1, conststr2, strlen(conststr2))  == 0)
/// \brief Returns TRUE if a string starts with the given substring  (Case insensitive)
#define IS_WSTR_STARTS_WITH(str1,conststr2) (_wcsnicmp(str1, conststr2, wcslen(conststr2))  == 0)

/// \brief Returns TRUE if a string starts with the given substring  (Case sensitive)
#define IS_TSTR_STARTS_WITH_CASESENS(str1,conststr2) (_tcsncmp(str1, conststr2, _tcslen(conststr2)) == 0)
/// \brief Returns TRUE if a string starts with the given substring  (Case sensitive)
#define IS_CSTR_STARTS_WITH_CASESENS(str1,conststr2) (strncmp(str1, conststr2, strlen(conststr2))  == 0)
/// \brief Returns TRUE if a string starts with the given substring  (Case sensitive)
#define IS_WSTR_STARTS_WITH_CASESENS(str1,conststr2) (wcsncmp(str1, conststr2, wcslen(conststr2))  == 0)

/// \brief Returns TRUE if a wide character is a hex character
#define ISHEX(h) ( (L'0' <= h && h <= L'9') || (L'A' <= h && h <= L'F') || (L'a' <= h && h <= L'f') )

// --------------------------------------------------------------------------------------------
/// \brief Converts a hex charater into its value
#define HEX(c)  ((c)<='9'?(c)-'0':(c)<='F'?(c)-'A'+0xA:(c)-'a'+0xA)

// --------------------------------------------------------------------------------------------
// Flag macros
//
/// \brief TRUE if a flag is set in a DWORD
#define IS_FLAG_SET(_dw, _flag)     (((_dw) & (_flag)) == (_flag))
/// \brief Set a flag in a DWORD
#define SET_FLAG(_dw, _flag)        (_dw) |= (_flag)
/// \brief Unset a flag in a DWORD
#define UNSET_FLAG(_dw, _flag)      (_dw) &= ~(_flag)

/// \brief Set a flag in a DWORD (interlocked)
#define SET_FLAG_SAFE(_lFlags,_flag)    { LONG __lFlags = _lFlags | _flag; \
                                            InterlockedExchange(&_lFlags, __lFlags); }
/// \brief Unset a flag in a DWORD (interlocked)
#define UNSET_FLAG_SAFE(_lFlags, _flag) { LONG __lFlags = _lFlags & ~_flag; \
                                            InterlockedExchange(&_lFlags, __lFlags); }
// --------------------------------------------------------------------------------------------
// String/IO arg macros
//
/// \brief Buffer pointer and count
#define SZARGS(_sz) _sz, countof(_sz)
/// \brief Buffer pointer and size
#define IOARGS(_sz) &_sz, sizeof(_sz)

// --------------------------------------------------------------------------------------------
// Test & Debug macros
//
#define CASE_TO_STRING(_case) case _case: wzCase = TEXT(#_case); break;
#define CASE_TO_STRINGA(_case) case _case: szCase = #_case; break;

/// \brief Convert a BOOL value to a text value (wchar)
#define BOOLTEXT(_f)        ((_f) ? L"TRUE" : L"FALSE")
/// \brief Convert a BOOL value to a text value (char)
#define BOOLTEXTA(_f)       ((_f) ? "TRUE" : "FALSE")
/// \brief Convert a BOOL suceess/fail value to a text value
#define SUCCESS_TEXT(_f)  ((_f) ? L"Success" : L"Fail")
/// \brief Convert a text value to a BOOL
#define IS_SZ_TRUE(_sz)     (_sz ? IS_CSTR_EQUAL(_sz, "TRUE") : FALSE)  // TODO: add "1" and "yes"
#define ENABLE_TEXT(_f) ((_f)?"Enable":"Disable")

#define CRUMB(_s,_i)     WDK_LOGMSG(1,(L"Crumb %s:%d\r\n", _s, _i));

// --------------------------------------------------------------------------------------------
// class macros
//

// Try to create a class, catch alloc exception
// REQUIRES #include <new>
#define TRY_CREATE( _pObj, _type_ ) \
            try { \
                _pObj = new _type_(); \
            } catch(std::bad_alloc) { \
                _pObj = NULL;  \
                SetLastError(ERROR_OUTOFMEMORY); } \

#define TRY_CREATE_ARG( _pObj, _type_, _args_ ) \
            try { \
                _pObj = new _type_ _args_; \
            } catch(std::bad_alloc) { \
                _pObj = NULL; \
                SetLastError(ERROR_OUTOFMEMORY); } \

#define TRY_CREATE_ARRAY( _pArray_, _type_, _size_ ) \
            try { \
                _pArray_ = new _type_[_size_]; \
            } catch(std::bad_alloc) { \
                _pArray_ = NULL;  \
                SetLastError(ERROR_OUTOFMEMORY); } \

// --------------------------------------------------------------------------------------------
// conversion macros
//
/// \brief Convert time in milliseconds to a FILETIME value
#define MILLISECONDS_TO_FILETIME     (DWORD)10000

// --------------------------------------------------------------------------------------------
// Library function macros
//
/// \brief Load a function pointer from a library
#define LOAD_API(_lib, _name) (_lib ? (FUNC_##_name) GetProcAddress(_lib, FN_##_name) : NULL)


#endif // _WDK_MACROS_H_
