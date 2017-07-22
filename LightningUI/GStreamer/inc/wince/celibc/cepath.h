#ifndef __CE_PATH_H__
#define __CE_PATH_H__

char*		CE_FixPathA(const char* pszIn, char* pszOut);
wchar_t*	CE_FixPathW(const wchar_t* pwszIn, wchar_t* pwszOut);
wchar_t*	CE_CanonicalizePathW(wchar_t* pwszTarget);
DWORD		CE_GetCurrentDirectoryA(DWORD dwSize, char* pszDir);
DWORD		CE_GetCurrentDirectoryW(DWORD dwSize, wchar_t* pwszDir);
BOOL		CE_SetCurrentDirectoryA(const char* pszDir);
BOOL		CE_SetCurrentDirectoryW(const wchar_t* pwszDir);
DWORD		CE_GetFileAttributesA(const char* pszFileName);
DWORD		CE_GetFileAttributesW(const wchar_t* pwszFileName);
BOOL		CE_SetFileAttributesA(const char* pszFileName, DWORD dwAttrib);
BOOL		CE_SetFileAttributesW(const wchar_t* pwszFileName, DWORD dwAttrib);
HANDLE		CE_FindFirstFileA(const char* pszPath, LPWIN32_FIND_DATAA pfd);
HANDLE		CE_FindFirstFileW(const wchar_t* pwszPath, LPWIN32_FIND_DATAW pfd);
HANDLE		CE_CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
HANDLE		CE_CreateFileW(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

#ifdef UNICODE
#define CE_FixPath				CE_FixPathW
#define CE_CanonicalizePath		CE_CanonicalizePathW
#define CE_GetCurrentDirectory	CE_GetCurrentDirectoryW
#define CE_SetCurrentDirectory	CE_SetCurrentDirectoryW
#define CE_GetFileAttributes	CE_GetFileAttributesW
#define CE_SetFileAttributes	CE_SetFileAttributesW
#define CE_FindFirstFile		CE_FindFirstFileW
#define CE_CreateFile			CE_CreateFileW
#else
#define CE_FixPath				CE_FixPathA
#define CE_CanonicalizePath		CE_CanonicalizePathA
#define CE_GetCurrentDirectory	CE_GetCurrentDirectoryA
#define CE_SetCurrentDirectory	CE_SetCurrentDirectoryA
#define CE_GetFileAttributes	CE_GetFileAttributesA
#define CE_SetFileAttributes	CE_SetFileAttributesA
#define CE_FindFirstFile		CE_FindFirstFileA
#define CE_CreateFile			CE_CreateFileA
#endif

#endif	/* __CE_PATH_H__ */
