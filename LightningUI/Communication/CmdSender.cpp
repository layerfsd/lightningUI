#include "CmdSender.h"


CmdSender::CmdSender(void)
{
	m_hSendCommandThread = INVALID_HANDLE_VALUE;
	m_pCmdList = NULL;
	m_pCmdListTail = NULL;
	m_bQuitSendThread = FALSE;
	m_iCmdSerialNumber = 0;
	m_pCommander=NULL;
	m_iCmdCnt = 0;

	m_DataCS = new CCriticalSection;

	m_iPriority = CE_THREAD_PRIO_256_NORMAL;

	m_hCmdBuffReadyEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	m_hCmdSendOutEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	m_hSendCommandThread=CreateThread(NULL,NULL,SendCommandThreadPro,this,NULL,NULL);
	if(m_hSendCommandThread!=INVALID_HANDLE_VALUE)
	{
		CeSetThreadPriority(m_hSendCommandThread, m_iPriority);
	}
}

CmdSender::~CmdSender(void)
{
	m_bQuitSendThread = TRUE;
	SetEvent(m_hCmdBuffReadyEvent);
	Sleep(100);

	if(m_hCmdBuffReadyEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCmdBuffReadyEvent);
		m_hCmdBuffReadyEvent=INVALID_HANDLE_VALUE;
	}

	if(m_hCmdSendOutEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCmdSendOutEvent);
		m_hCmdSendOutEvent=INVALID_HANDLE_VALUE;
	}

	if(m_hSendCommandThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSendCommandThread);
		m_hSendCommandThread=INVALID_HANDLE_VALUE;
	}

	if(m_DataCS)
	{
		delete m_DataCS;
	}
}
void CmdSender::Lock()
{
	if(m_DataCS)
	{
		m_DataCS->Enter();
	}
}
void CmdSender::UnLock()
{
	if(m_DataCS)
	{
		m_DataCS->Leave();
	}
}
BOOL CmdSender::SetPriority(INT iPriority)
{
	m_iPriority = iPriority;

	if(m_hSendCommandThread!=INVALID_HANDLE_VALUE)
	{
		return CeSetThreadPriority(m_hSendCommandThread, m_iPriority);
	}

	return TRUE;
}
void	CmdSender::LinkCommander(CCommander *pCommander)
{
	m_pCommander = pCommander;
}
BOOL CmdSender::ClearCmdBuff()
{
	Lock();

	CMD_BUFF *p=m_pCmdList;

	while (p)
	{
		p->bSkip=TRUE;
		p=p->pNext;
	}

	UnLock();

	return m_pCmdList != NULL;
}
BOOL CmdSender::IsCmdListEmpty()
{
	return m_iCmdCnt==0;
}

BOOL CmdSender::WaitSendFinish()
{
	BOOL bRet = FALSE;

	if(m_pCmdList)
	{
		if(WaitForSingleObject(m_hCmdSendOutEvent,INFINITE)==WAIT_OBJECT_0)
		{
			bRet = GetEventData(m_hCmdSendOutEvent);
		}
	}
	else
	{
		bRet = TRUE;
	}

	return bRet;
}
void CmdSender::SkipCmd(DWORD dwUserData)
{
	Lock();

	CMD_BUFF *p=m_pCmdList;

	while (p)
	{
		if(dwUserData == p->dwUserData)
		{
			p->bSkip=TRUE;
		}
		p=p->pNext;
	}

	UnLock();
}
BOOL CmdSender::QueueCmdBuff(DWORD dwUserData,BYTE *pBuff,INT  iLen,INT iSendDelay)
{
	BOOL bRet = FALSE;

	if(!pBuff || !iLen || iLen>320)//max 320 BYTES
		return FALSE;

	//pending command buff:
	CMD_BUFF *pNewCmd=new CMD_BUFF;

	if(!pNewCmd)
		return FALSE;

	memcpy(pNewCmd->BuffData,pBuff,iLen);
	pNewCmd->iBuffLen=iLen;
	pNewCmd->pNext=NULL;
	pNewCmd->iSendDelay=iSendDelay;
	pNewCmd->index=m_iCmdSerialNumber;
	pNewCmd->bSkip=FALSE;
	pNewCmd->dwUserData = dwUserData;

	Lock();
	m_iCmdSerialNumber++;
	m_iCmdCnt++;

	if(m_pCmdListTail)
	{
		m_pCmdListTail->pNext = pNewCmd;
		m_pCmdListTail = pNewCmd;
	}
	else
	{
		m_pCmdList = pNewCmd;
		m_pCmdListTail = pNewCmd;
	}
	UnLock();
	/*
	Lock();
	CMD_BUFF *p=m_pCmdList;

	if(!p)
	{
		m_pCmdList = pNewCmd;
	}
	else
	{
		while (p)
		{
			if(!p->pNext)
			{
				p->pNext=pNewCmd;
				break;
			}
			else
			{
				p=p->pNext;
			}
		}
	}

	UnLock();
	*/

	SetEvent(m_hCmdBuffReadyEvent);

	ResetEvent(m_hCmdSendOutEvent);

	bRet = TRUE;

	return bRet;
}

DWORD CmdSender::SendCommandThreadPro(LPVOID lpParam)
{
	CmdSender *pSender=(CmdSender*)lpParam;

	if(pSender)
	{
		pSender->SendCmdPro();
	}

	return 0;
}
void CmdSender::SendCmdPro()
{
	while (!m_bQuitSendThread)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hCmdBuffReadyEvent,INFINITE))
		{
			CMD_BUFF *pCursor=m_pCmdList;
			BOOL bSendSucc=FALSE;
			while(pCursor)
			{
				if(!pCursor->bSkip)
				{
					if(pCursor->iSendDelay)
					{
						Sleep(pCursor->iSendDelay);
					}

					bSendSucc = DoSendCmd(pCursor);
				}
				else
				{
					//RETAILMSG(1,(_T("###skip: %d, 0x%x\r\n"),pCursor->index,pCursor->BuffData[0]));
				}

				Lock();
				if(!pCursor->pNext)//reach tail
				{
					m_pCmdListTail = NULL;
					m_pCmdList = NULL;

					SetEventData(m_hCmdSendOutEvent,bSendSucc);
					SetEvent(m_hCmdSendOutEvent);
				}
				else
				{
					m_pCmdList=pCursor->pNext;
				}
				delete pCursor;
				pCursor=m_pCmdList;
				m_iCmdCnt--;
				UnLock();

				Sleep(0);
			}
		}
		else
		{
			break;
		}
	}
}
