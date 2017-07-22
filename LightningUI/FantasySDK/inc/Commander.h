#pragma once

#include "Util.h"
#include "CriticalSection.h"

typedef void (CALLBACK* COMMANDPROC)(LPBYTE,DWORD,LPVOID);

class CCommander
{
public:
	CCommander(void);
	~CCommander(void);

public:
	BOOL Initialize(COMMANDPROC lpfnCmdProc,LPVOID lparam,LPCTSTR lpPortName,UINT iBaudRate,HWND hWnd);
	BOOL SendExtenCmd(BYTE byKey, BYTE *pData, BYTE Len);
	BOOL SendCmd(BYTE byKey, BYTE byValue,BOOL bSkipSameCmd=FALSE);
	BOOL WriteSerialPort(PBYTE pBuff, DWORD dwLen);
	virtual void PreProcess(LPBYTE lpData,DWORD dwLen)=0;

	virtual BYTE GetItemData(INT ID)=0;

	void Lock();
	void UnLock();

private:
	BOOL OpenComMCU(LPCTSTR lpPortName,UINT iBaudRate=CBR_115200);
	static DWORD ComProcessMCU(LPVOID lparam);

protected:
	COMMANDPROC		m_lpfnCmdProc;
	LPVOID					m_lpProcPara;
	CCriticalSection* m_DataCS;
	HANDLE			m_hComMCU;
	HANDLE			m_hComThreadMCU;
	HWND				m_hWnd;

	DWORD			 m_dwRevLen;
	unsigned char		m_cRevBuff[4096];
	unsigned char		m_cSendBuff[4096];
};
