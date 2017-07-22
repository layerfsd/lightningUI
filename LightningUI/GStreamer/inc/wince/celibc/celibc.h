#ifndef __CE_LIBC_H__
#define __CE_LIBC_H__

#pragma warning(disable:4996)  /* declared deprecated */

#define _WINSOCKAPI_
#include <windef.h>
#include <types.h>
#include <winbase.h>
#include <cmnintrin.h>
#include <tchar.h>
#include "cepath.h"

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.

#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)

#define FILE_MAP_COPY		SECTION_QUERY

#define MOVEFILE_REPLACE_EXISTING	0x00000001

#define MWMO_ALERTABLE		0x0002
#define WAIT_IO_COMPLETION	STATUS_USER_APC

#define FILENAME_MAX	260

#define ISSLASH(a)		((a) == _T('\\') || (a) == _T('/'))

#define _IOFBF			0x0000
#define _IOLBF			0x0040
#define _IONBF			0x0004

#define errno			(*_errno())
#define _doserrno		(*__doserrno())
#define _close			close
#define	_tfullpath		_wfullpath
#define	_tfindfirst		_wfindfirst
#define	_tfindnext		_wfindnext
#define strncasecmp		_strnicmp

#ifdef _USE_32BIT_TIME_T
#define gmtime			_gmtime32
#define localtime		_localtime32
#define mktime			_mktime32
#define time			_time32
#else
#define gmtime			_gmtime64
#define localtime		_localtime64
#define mktime			_mktime64
#define time			_time64
#endif

int *			_errno(void);
unsigned long * __doserrno(void);
void			abort(void);
int				getpid(void);
char *			getcwd(char *buffer, int size);
char *			getenv(const char * name);
int				putenv(const char * string);
char *			setlocale(int category, const char * locale);
struct lconv *	localeconv(void);
size_t			strxfrm(char *strDest, const char *strSource, size_t count);
int				strcoll(const char * str1, const char * str2);
void			perror(const char * str);
const char *	strerror(int errnum);
size_t			strftime(char * string, size_t maxsize, const char * format, const struct tm * timeptr);
void *			bsearch(const void * key, const void * base, size_t num, size_t width, int (* compare)(const void *, const void *));
wchar_t *		_wfullpath(wchar_t * absPath, const wchar_t * relPath, size_t maxLength);
int				_wremove(const wchar_t * filename);
struct tm *		_gmtime64(const __time64_t * ptime);
struct tm *		_localtime64(const __time64_t * ptime);
__time64_t		_mktime64(struct tm * ptm);
__time64_t		_time64(__time64_t * ptime);
errno_t			_get_daylight(__out int * _Daylight);
errno_t			_get_dstbias(__out long * _Daylight_savings_bias);
errno_t			_get_timezone(__out long * _Timezone);
errno_t			_gmtime32_s(struct tm * ptm, const __time32_t * ptime);
errno_t			_gmtime64_s(struct tm * ptm, const __time64_t * ptime);
errno_t			_localtime64_s(struct tm * ptm, const __time64_t * ptime);
uintptr_t		_beginthreadex(void * security, unsigned stacksize, unsigned (__stdcall * initialcode) (void *), void * argument, unsigned createflag, unsigned * thrdaddr);
void			_endthreadex(unsigned retcode);

DWORD WINAPI	GetVersion();
LCID WINAPI		GetThreadLocale(void);
BOOL WINAPI		GetUserNameW(LPWSTR lpBuffer, LPDWORD pcbBuffer);
UINT WINAPI		GetSystemDirectoryW(LPWSTR lpBuffer, UINT uSize);
UINT WINAPI		GetWindowsDirectoryW(LPWSTR lpBuffer, UINT uSize);
DWORD WINAPI	GetShortPathNameW(LPCWSTR lpszLongPath, LPWSTR  lpszShortPath, DWORD cchBuffer);
BOOL WINAPI		MoveFileExW(LPCWSTR lpExistingFileName,LPCWSTR lpNewFileName, DWORD dwFlags);
DWORD WINAPI	SleepEx(DWORD dwMilliseconds, BOOL bAlertable);
DWORD WINAPI	WaitForMultipleObjectsEx(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMilliseconds, BOOL bAlertable);
HANDLE WINAPI	GetStdHandle(DWORD nStdHandle);
BOOL WINAPI		SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes);

#endif	/* __CE_LIBC_H__ */
