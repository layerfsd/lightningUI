/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkFileUtil.h#1 $ */
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
/// \brief      File utilities  
/// \remarks 
///             - Module Name     :  WdkCore.dll
///             - Original Author :  Rory McLeod
///             - Original Date   :  2011-02-15
//-----------------------------------------------------------------------------
/// \file
#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_
#pragma once

#include <WdkVersion.h>
#include <WdkCoreDef.h>
#include <WdkMacros.h>
#include <WdkDebug.h>

/// \brief      File version values 
typedef struct _VERSION
{
    WORD major;     ///< Major version
    WORD minor;     ///< Minor version
    WORD release;   ///< Release version
    WORD revision;  ///< Revision (build) version
} VERSION;

/// \brief Returns the file version for the given module
WdkCore_API HRESULT GetFileVersion(HMODULE hModule, VERSION *pVer);
/// \brief Returns the file version for the given file 
WdkCore_API HRESULT GetFileVersion(__in LPCTSTR lptstrFilename, VERSION *pVer);

/// \brief Returns TRUE if the file or directory exists
WdkCore_API BOOL Exists(__in LPCTSTR lptstrFilename);

/// \brief Returns the file name and path(home) for a given module
WdkCore_API HRESULT GetModuleBaseName(HMODULE hModule, __out_opt LPTSTR szNameBuf, size_t cchNameLen, __out_opt LPTSTR szHomeBuf, size_t cchHomeLen);
/// \brief Returns the directory for the current process
WdkCore_API HRESULT GetHomeDir(__out LPTSTR szHome, int cch);
WdkCore_API BOOL IsDirectoryA(const char* dirName);
/// \brief Returns TRUE if the directory exists
WdkCore_API HRESULT DirectoryExistsW(LPCWSTR wzPath);
/// \brief Returns TRUE if the file exists
WdkCore_API HRESULT FileExistsW(LPCWSTR wzFilename);

/// \brief Creates a new directory
///
/// Similar to CreateDirectory, but also creates intermediate directorise if needed.
///
/// \param szPath:                  The directory path to create
/// \param lpSecurityAttributes:    Ignored, set to 0
/// \return BOOL
///     - TRUE: Success
///     - FALSE: Failure. Use GetLastError() to determine the cause.
///
WdkCore_API BOOL CreateDirectoryEx(__in LPCTSTR szPath, __in SECURITY_ATTRIBUTES * lpSecurityAttributes);

/// \brief Prints the module name and version to the debug log
_inline void PrintModuleVersion(LPCWSTR wzName, HMODULE hModule)
{
    VERSION v = {0};
    GetFileVersion(hModule, &v);
    LPCWSTR pwzFmt = 
#ifdef DEBUG
                        L"%s: v %d.%d.%d.%d <=============== DEBUG BUILD\r\n";
#else
                        L"%s: v %d.%d.%d.%d\r\n";
#endif
    WDK_LOGMSG(QLX_ZONE_ERROR, (pwzFmt, wzName, v.major, v.minor, v.release, v.revision));
}

#endif // _FILE_UTIL_H_
