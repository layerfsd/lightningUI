#include "DABAdapter.h"
#include "CommonServices.h"

#if CVTE_EN_DAB

const CDABAdapter* CDABAdapter::m_pInstance = new CDABAdapter();
CnComm* CDABAdapter::m_pCnComm=NULL;
byte *CDABAdapter::m_pdata=NULL;
int CDABAdapter::m_iDataLen=0;
BOOL CDABAdapter::m_bHasData=FALSE;
link_RDS_FM_info_t *CDABAdapter::m_pServiceLinkInfo=NULL;
BOOL CDABAdapter::m_bNeedServiceLink=FALSE;

extern const wchar_t* g_strInstallPath;
extern HWND g_hUIWnd;

CDABAdapter::CDABAdapter()
:m_bInitialized(FALSE),m_hThreadHandle(NULL)
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));
}

CDABAdapter::~CDABAdapter()
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));

    Deinitialize();
}

CDABAdapter* CDABAdapter::GetInstance()
{
    return (CDABAdapter*)m_pInstance;
}

void CDABAdapter::Initialize(DWORD PortNum, DWORD dwBaudRate)
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: COM%d\n"), TEXT(__FUNCTION__),PortNum));

	m_PortNum = PortNum;
	m_dwStartTick = GetTickCount();

	//Init COM
	if(m_pCnComm != NULL)
	{
		m_pCnComm->Close();
		delete m_pCnComm;
		m_pCnComm = NULL;
	}

	m_pCnComm = new CnComm;
	m_pCnComm->SetOption(m_pCnComm->GetOption() | CnComm::EN_THREAD | CnComm::EN_RX_BUFFER  | CnComm::EN_TX_BUFFER );
	//m_pCnComm->SetNotifyNum(1);

	m_hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start event
    if(m_hStartEvent == NULL)
    {
        RETAILMSG(1, (TEXT("ERR:[%s]: create start event failed,errno:%d\n"),TEXT(__FUNCTION__),::GetLastError()));
    }

	StartCOM(dwBaudRate);

	if(m_pCnComm->IsOpen())
	{
		//Register DAB Callback
		RegisterDABCallback();
		DABApi_Service(TRUE);
		m_bInitialized = TRUE;
	}

}

void CDABAdapter::Deinitialize()
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));

    if (m_bInitialized)
    {
		DABApi_Service(FALSE);

		//StopCOM
		StopCOM();

		//Clear COM
		if(m_pCnComm)
		{
			m_pCnComm->Close();
			delete m_pCnComm;
			m_pCnComm=NULL;
		}

		//close event:
		if(m_hStartEvent != NULL)
		{
			CloseHandle(m_hStartEvent);
			m_hStartEvent = NULL;
		}

		//Clear Param
        m_bInitialized = FALSE;
    }
}

void CDABAdapter::StartCOM(DWORD dwBaudRate)
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));

	if(m_pCnComm == NULL)
	{
		RETAILMSG(DABADAPTER_DEBUG_ERR, (TEXT("ERR:[%s]: new CnComm failed.\n"), TEXT(__FUNCTION__)));
		return;
	}

	if (m_PortNum <= 0)
	{
		return;
	}
	m_hThreadHandle = ::CreateThread( NULL,
                                      0,
                                      CDABAdapter::ThreadProc,
                                      ( LPVOID )this,
                                      0,
                                      &m_dwThreadId );
	if (m_hThreadHandle != NULL)
	{
		CeSetThreadPriority(m_hThreadHandle, CE_THREAD_PRIO_256_HIGHEST);
		m_pCnComm->SetNotifyThreadId(m_dwThreadId);
		::WaitForSingleObject(m_hStartEvent,INFINITE);
		if (!m_pCnComm->IsOpen())
		{
			if (!m_pCnComm->Open(m_PortNum, dwBaudRate/*38400 230400*/,  NOPARITY, 8, ONESTOPBIT))
			{
				RETAILMSG(DABADAPTER_DEBUG_ERR, (TEXT("ERR:[%s]: COM%d open failed, error:%d\n"), TEXT(__FUNCTION__), m_PortNum, GetLastError()));
				if ( m_hThreadHandle )
				{
					PostThreadMessage(m_dwThreadId, WM_USER+600, WPARAM(0), LPARAM(0));
					WaitForSingleObject( m_hThreadHandle, INFINITE);
					::CloseHandle( m_hThreadHandle );
					m_hThreadHandle = NULL;
					m_dwThreadId = 0;
				}
			}
			else
			{
				RETAILMSG(DABADAPTER_DEBUG_MSG, (TEXT("MSG:[%s]: COM%d open success %d.\n"), TEXT(__FUNCTION__), m_PortNum,dwBaudRate));
			}
		}
	}
}

void CDABAdapter::StopCOM()
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));
	PostThreadMessage(m_dwThreadId, WM_USER+600, WPARAM(0), LPARAM(0));
	if (m_hThreadHandle)
	{
		WaitForSingleObject( m_hThreadHandle, INFINITE);
		::CloseHandle( m_hThreadHandle );
		m_hThreadHandle = NULL;
		m_dwThreadId = 0;
	}
	m_pCnComm->Close();
}

VOID CDABAdapter::RegisterDABCallback()
{
	FuncDABCallBack cbfun;
	cbfun.pCBSendDABData = OnComSend;
	cbfun.pCBSrvLink     = OnSrvLink;
	cbfun.pCBLoadData    = OnLoadData;
	cbfun.pCBSaveData    = OnSaveData;
	//cbfun.pPrintPic		 = NULL;

	RegisterDABCBFunc(cbfun);
}

DWORD CDABAdapter::ThreadProc( LPVOID dParam )
{
    CDABAdapter* pThreadPtr = ( CDABAdapter* ) dParam;
    pThreadPtr->m_dwExitCode = pThreadPtr->COMThreadRun();
    ::ExitThread( pThreadPtr->m_dwExitCode );
    return pThreadPtr->m_dwExitCode;
};

DWORD CDABAdapter::COMThreadRun()
{
	MSG msg;

	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));

    if(!SetEvent(m_hStartEvent)) //set thread start event
    {
		RETAILMSG(DABADAPTER_DEBUG_ERR, (TEXT("ERR:[%s]: set start event failed,errno:%d\n"), TEXT(__FUNCTION__),::GetLastError()));
        return 1;
    }

	while(1)
	{
	    if(GetMessage(&msg,NULL,0,0))
	    {
	        if(msg.message == ON_COM_RECEIVE)
	        {
				OnComRecv(msg.wParam, msg.lParam);
	        }
			else if (msg.message == (WM_USER+600))
			{
				break;
			}
	    }
	}
    return 0;
}

VOID CDABAdapter::OnComRecv(WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));

	do
	{
		char buffer[STRING_LEN];
		int len = m_pCnComm->Read(buffer, STRING_LEN);
		if (len > 0)
		{
				//RETAILMSG(1, (TEXT("MSG:[%s]: len[%d], %x \n"), TEXT(__FUNCTION__),len, buffer[0]));
			DABApi_ParseData((byte *)buffer, len);
		}

		//! 接收缓冲模式下，要确保把已经在Comm_.Input()缓冲区的数据处理完
		//! 否则如果没有新的数据再来，不会继续通知你
	} while(m_pCnComm->IsRxBufferMode() && m_pCnComm->Input().SafeSize());

}

BOOL CDABAdapter::GetInitialize()
{
	return m_bInitialized;
}
BOOL CDABAdapter::HasData()
{
	return m_bHasData;
}
bool CDABAdapter::SaveData()
{
	if(!m_iDataLen || !m_pdata)
		return false;


	FILE	*file = NULL;
	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("DAB\\"));
	CreateDirectory(strPath.String(),NULL);
	CM_String strFile=strPath+CM_String(_T("dab_config.dat"));

	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: nSize[%d]\n"), TEXT(__FUNCTION__),m_iDataLen));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"wb") )
	{
		return false;
	}

	int len=fwrite(m_pdata,1,m_iDataLen,file);

	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: len[%d],%s\n"), TEXT(__FUNCTION__),len,strFile.String()));

	fclose(file);

	return true;
}
bool CDABAdapter::OnSaveData( int nSize, void *pPtr )
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: nSize[%d]\n"), TEXT(__FUNCTION__),nSize));

	if(!pPtr || !nSize)
		return false;

	//clear first:
	if(nSize!=m_iDataLen && m_pdata)
	{
		delete[] m_pdata;
		m_pdata = NULL;
	}

	if(!m_pdata)
	{
		m_pdata = new byte[nSize];
	}

	m_iDataLen = nSize;
	memcpy(m_pdata,pPtr,m_iDataLen);

	return true;
}
bool CDABAdapter::OnLoadData( int nSize, void *pPtr )
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: nSize[%d]\n"), TEXT(__FUNCTION__),nSize));

	if(!pPtr || !nSize)
		return false;

	FILE	*file = NULL;
	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("DAB\\"));
	CreateDirectory(strPath.String(),NULL);
	CM_String strFile=strPath+CM_String(_T("dab_config.dat"));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"rb") )
	{
		return false;
	}

	int len=fread(pPtr,1,nSize,file);

	fclose(file);

	m_bHasData = (len==nSize);

	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: len[%d],%s\n"), TEXT(__FUNCTION__),len,strFile.String()));

	return true;
}
bool CDABAdapter::OnComSend( WPARAM wParam, LPARAM lParam )
{
	BYTE *pData;
    UINT32 len;

    pData = (BYTE*)wParam;
	len   = (UINT32)lParam;

	//RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: len[%d]\n"), TEXT(__FUNCTION__),len));

	if(CDABAdapter::m_pCnComm)
	{
		CDABAdapter::m_pCnComm->WritePort(pData,len);
		return TRUE;
	}
	return FALSE;
}

bool CDABAdapter::OnSrvLink(byte Link, WORD *pPtr )
{
	//RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: Link=%d\n"), TEXT(__FUNCTION__),Link));
	link_RDS_FM_info_t *pinfo = (link_RDS_FM_info_t*)pPtr;

	m_pServiceLinkInfo = pinfo;
	/*
	for (int i=0;i<MAX_PI_CODE_SEND;i++)
	{
		RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: PI=0x%x\n"), TEXT(__FUNCTION__),pinfo->PICode[i]));
	}
	for (int i=0;i<MAX_FREQS_SEND;i++)
	{
		RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: Freq=0x%x\n"), TEXT(__FUNCTION__),pinfo->Freqs[i]));
	}
	*/

	if(g_hUIWnd)
	{
		if(Link)
		{
			if(m_bNeedServiceLink)
			{
				//reset and notify GUI:
				m_bNeedServiceLink = FALSE;
				::PostMessage(g_hUIWnd,WM_DAB_SERVICE_LINK_MSG,Link,(LPARAM)pPtr);
			}
		}
		else
		{
			::PostMessage(g_hUIWnd,WM_DAB_SERVICE_LINK_MSG,Link,(LPARAM)pPtr);
		}
	}

	return TRUE;
}

bool CDABAdapter::OnPrintPic(unsigned char * filename)
{
	RETAILMSG(DABADAPTER_DEBUG_TEST, (TEXT("MSG:[%s]: \n"), TEXT(__FUNCTION__)));
	return TRUE;
}
#endif
