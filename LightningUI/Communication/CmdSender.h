#pragma once

#include "Util.h"
#include "keydef.h"
#include "Renesas_com.h"
#include "../GUI/Common/message_map.h"
#include "Commander.h"
#include <pkfuncs.h>
#include "CriticalSection.h"

typedef struct _TAG_CMD_BUFF
{
	INT index;
	INT iBuffLen;
	BYTE BuffData[320];
	INT iSendDelay;
	BOOL bSkip;
	DWORD dwUserData;
	_TAG_CMD_BUFF *pNext;
}CMD_BUFF,*LPCMD_BUFF;

class CmdSender 
{
public:
	CmdSender(void);
	~CmdSender(void);

public:
	void	LinkCommander(CCommander *pCommander);
	BOOL QueueCmdBuff(DWORD dwUserData,BYTE *pBuff,INT  iLen,INT iSendDelay=0);
	BOOL ClearCmdBuff();

	BOOL WaitSendFinish();
	BOOL IsCmdListEmpty();
	BOOL SetPriority(INT iPriority);

	void SkipCmd(DWORD dwUserData);

	virtual BOOL DoSendCmd(CMD_BUFF *pbuff)=0;

protected:
	static DWORD SendCommandThreadPro(LPVOID lpParam);
	void SendCmdPro();
	void Lock();
	void UnLock();

protected:
	CCommander	*m_pCommander;
	CMD_BUFF		*m_pCmdList;
	CMD_BUFF		*m_pCmdListTail;
	INT					m_iPriority;

	HANDLE			m_hSendCommandThread;
	HANDLE			m_hCmdBuffReadyEvent;
	BOOL				m_bQuitSendThread;
	INT					m_iCmdSerialNumber;
	CCriticalSection* m_DataCS;
	HANDLE			m_hCmdSendOutEvent;
	int					m_iCmdCnt;

};
