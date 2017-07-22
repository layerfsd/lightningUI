#pragma once
#include "Util.h"
#include "CriticalSection.h"

typedef void (CALLBACK* COMPROC)(LPBYTE,DWORD,LPVOID);

class CCOM
{
public:
	CCOM(COMPROC lpfnComProc,LPVOID lparam,LPCTSTR lpPortName,UINT iBaudRate,HWND hWnd);
	~CCOM(void);
	BOOL OpenCom(LPCTSTR lpPortName,UINT iBaudRate);
	void Lock(){m_DataCS->Enter();}
	void UnLock(){m_DataCS->Leave();}
	DWORD Write(char *pString);
	DWORD Write(PBYTE pBuff, DWORD dwLen);
	DWORD Read(PBYTE pBuff, DWORD dwLen,DWORD dwTimeOut=1000);
	void Reset();
private:
	static DWORD ComProcessThread(LPVOID lparam);

private:
	COMPROC			m_lpfnComProc;
	LPVOID					m_lpProcPara;
	HWND					m_hWnd;
	CCriticalSection* m_DataCS;
	HANDLE			m_hCom;
	HANDLE			m_hComThread;
	HANDLE			m_hRevEvent;
	unsigned char		m_cRevBuff[4096];
	DWORD			 m_dwRevLen;
	CM_String			m_strPortName;
};
