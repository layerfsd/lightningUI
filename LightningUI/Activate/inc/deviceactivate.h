
#ifndef	__deviceactivate_h__
#define	__deviceactivate_h__

// Windows Header Files:
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_ACTIVATE_NO_CA				(-1)

//FUNCTIONS:
DWORD CryptoOpen();
DWORD CryptoClose();
DWORD CryptoGetDeviceName(wchar_t* inBuff,DWORD inLen);
DWORD CryptoVerifyValue(PBYTE inBuff,DWORD inLen);
DWORD CryptoInstallCA(LPCTSTR pDestCAFile,LPCTSTR pSrcCAFile);
DWORD CryptoActivateUsingCA(LPCTSTR pStrCAFile,PBYTE inResult,DWORD inLen);
DWORD CryptoResolveResult(PBYTE inResult,DWORD inLen,PBYTE outResult,DWORD outLen);

#ifdef __cplusplus
}
#endif

#endif	// __deviceactivate_h__