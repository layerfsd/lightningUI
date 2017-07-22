/***
*process.h - definition and declarations for process control functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the modeflag values for spawnxx calls.
*       Also contains the function argument declarations for all
*       process control related routines.
*
*       [Public]
*
****/

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_PROCESS
#define _INC_PROCESS

#include <crtdefs.h>

#ifndef _POSIX_

#ifdef __cplusplus
extern "C" {
#endif

/* modeflag values for _spawnxx routines */

#define _P_WAIT         0
#define _P_NOWAIT       1
#define _OLD_P_OVERLAY  2
#define _P_NOWAITO      3
#define _P_DETACH       4

#ifdef  _MT
#define _P_OVERLAY      2
#else
extern int _p_overlay;
#define _P_OVERLAY      _p_overlay
#endif  /* _MT */

/* Action codes for _cwait(). The action code argument to _cwait is ignored
   on Win32 though it is accepted for compatibilty with old MS CRT libs */
#define _WAIT_CHILD      0
#define _WAIT_GRANDCHILD 1


/* function prototypes */

#ifdef  _MT
_CRTIMP uintptr_t __cdecl _beginthread (__in void (__cdecl * _StartAddress) (void *),
        __in unsigned _StackSize, __in_opt void * _ArgList);
_CRTIMP void __cdecl _endthread(void);
_CRTIMP uintptr_t __cdecl _beginthreadex(__in_opt void * _Security, __in unsigned _StackSize,
        __in unsigned (__stdcall * _StartAddress) (void *), __in_opt void * _ArgList, 
        __in unsigned _InitFlag, __in_opt unsigned * _ThrdAddr);
_CRTIMP void __cdecl _endthreadex(__in unsigned _Retval);

#if defined(__cplusplus) && defined(_M_CEE)
extern "C++"
{
_MCRTIMP uintptr_t __cdecl _beginthread (__in void (__clrcall * _StartAddress) (void *),
         __in unsigned _StackSize, __in_opt void * _ArgList);
_MCRTIMP uintptr_t __cdecl _beginthreadex(__in_opt void * _Security, __in unsigned _StackSize,
        __in unsigned (__clrcall * _StartAddress) (void *), __in_opt void * _ArgList, 
        __in unsigned _InitFlag, __out_opt unsigned * _ThrdAddr);
}
#endif

#endif

#ifndef _CRT_TERMINATE_DEFINED
#define _CRT_TERMINATE_DEFINED
_CRTIMP __declspec(noreturn) void __cdecl exit(__in int _Code);
_CRTIMP __declspec(noreturn) void __cdecl _exit(__in int _Code);
_CRTIMP void __cdecl abort(void);
#endif

_CRTIMP void __cdecl _cexit(void);
_CRTIMP void __cdecl _c_exit(void);
_CRTIMP int __cdecl _getpid(void);

_CRTIMP intptr_t __cdecl _cwait(__out_opt int * _TermStat, __in intptr_t _ProcHandle, __in int _Action);
_CRTIMP intptr_t __cdecl _execl(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execle(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execlp(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execlpe(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execv(__in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _execve(__in_z const char * _Filename, __in_z const char * const * _ArgList, __in_z_opt const char * const * _Env);
_CRTIMP intptr_t __cdecl _execvp(__in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _execvpe(__in_z const char * _Filename, __in_z const char * const * _ArgList, __in_z_opt const char * const * _Env);
_CRTIMP intptr_t __cdecl _spawnl(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnle(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnlp(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnlpe(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnv(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _spawnve(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList,
        __in_z_opt const char * const * _Env);
_CRTIMP intptr_t __cdecl _spawnvp(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _spawnvpe(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList,
        __in_z_opt const char * const * _Env);

#ifndef _CRT_SYSTEM_DEFINED
#define _CRT_SYSTEM_DEFINED
_CRTIMP int __cdecl system(__in_z_opt const char * _Command);
#endif

#ifndef _WPROCESS_DEFINED
/* wide function prototypes, also declared in wchar.h  */
_CRTIMP intptr_t __cdecl _wexecl(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexecle(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexeclp(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexeclpe(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexecv(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wexecve(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList,
        __in_z_opt const wchar_t * const * _Env);
_CRTIMP intptr_t __cdecl _wexecvp(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wexecvpe(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList, 
        __in_z_opt const wchar_t * const * _Env);
_CRTIMP intptr_t __cdecl _wspawnl(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnle(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnlp(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnlpe(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnv(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wspawnve(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList,
        __in_z_opt const wchar_t * const * _Env);
_CRTIMP intptr_t __cdecl _wspawnvp(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wspawnvpe(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList,
        __in_z_opt const wchar_t * const * _Env);
#ifndef _CRT_WSYSTEM_DEFINED
#define _CRT_WSYSTEM_DEFINED
_CRTIMP int __cdecl _wsystem(__in_z_opt const wchar_t * _Command);
#endif

#define _WPROCESS_DEFINED
#endif

/*
 * Security check initialization and failure reporting used by /GS security
 * checks.
 */
#if !defined(_M_CEE)
void __cdecl __security_init_cookie(void);
#ifdef  _M_IX86
void __fastcall __security_check_cookie(__in uintptr_t _StackCookie);
__declspec(noreturn) void __cdecl __report_gsfailure(void);
#else
void __cdecl __security_check_cookie(__in uintptr_t _StackCookie);
__declspec(noreturn) void __cdecl __report_gsfailure(__in uintptr_t _StackCookie);
#endif
#endif
extern uintptr_t __security_cookie;

/* --------- The preceding functions are OBSOLETE --------- */


#ifdef  _DECL_DLLMAIN
/*
 * Declare DLL notification (initialization/termination) routines
 *      The preferred method is for the user to provide DllMain() which will
 *      be called automatically by the DLL entry point defined by the C run-
 *      time library code.  If the user wants to define the DLL entry point
 *      routine, the user's entry point must call _CRT_INIT on all types of
 *      notifications, as the very first thing on attach notifications and
 *      as the very last thing on detach notifications.
 */
#ifdef  _WINDOWS_       /* Use types from WINDOWS.H */
BOOL WINAPI DllMain(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
BOOL WINAPI _CRT_INIT(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
BOOL WINAPI _wCRT_INIT(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
extern BOOL (WINAPI * const _pRawDllMain)(HANDLE, DWORD, LPVOID);
#else
int __stdcall DllMain(__in void * _HDllHandle, __in unsigned _Reason, __in_opt void * _Reserved);
int __stdcall _CRT_INIT(__in void * _HDllHandle, __in unsigned _Reason, __in_opt void * _Reserved);
int __stdcall _wCRT_INIT(__in void * _HDllHandle, __in unsigned _Reason, __in_opt void * _Reserved);
extern int (__stdcall * const _pRawDllMain)(void *, unsigned, void *);
#endif  /* _WINDOWS_ */
#endif

#if     !__STDC__

/* Non-ANSI names for compatibility */

#define P_WAIT          _P_WAIT
#define P_NOWAIT        _P_NOWAIT
#define P_OVERLAY       _P_OVERLAY
#define OLD_P_OVERLAY   _OLD_P_OVERLAY
#define P_NOWAITO       _P_NOWAITO
#define P_DETACH        _P_DETACH
#define WAIT_CHILD      _WAIT_CHILD
#define WAIT_GRANDCHILD _WAIT_GRANDCHILD

#endif  /* __STDC__ */

#ifdef  __cplusplus
}
#endif

#endif  /* _POSIX_ */

#endif  /* _INC_PROCESS */
