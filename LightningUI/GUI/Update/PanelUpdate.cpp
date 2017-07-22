#include "PanelUpdate.h"
#include "../../resource.h"

CPanelUpdate::CPanelUpdate(void)
{
	m_hACKEvent = INVALID_HANDLE_VALUE;
	m_hMCUReadyEvent = INVALID_HANDLE_VALUE;

	m_bUpdating = FALSE;

	m_pCmdSender = NULL;

	m_bMCUReadyToUpdate = FALSE;
}

CPanelUpdate::~CPanelUpdate(void)
{
	if(m_pCmdSender)
	{
		delete m_pCmdSender;
		m_pCmdSender = NULL;
	}

	if(m_hACKEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hACKEvent);
		m_hACKEvent = INVALID_HANDLE_VALUE;
	}

	if(m_hMCUReadyEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hMCUReadyEvent);
		m_hMCUReadyEvent = INVALID_HANDLE_VALUE;
	}
}
BOOL CPanelUpdate::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelUpdate::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelUpdate::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
	RETAILMSG(1,(_T("--->>CPanelUpdate OnEnterPanel :%d,%d,%d\n"),iParam,idAni,lpAniPara));

	//make animation:
	if(idAni != ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}

	//
	m_bEnableQuit = FALSE;

	if(!m_bUpdating)
	{
		StartMcuUpdate();
	}

	if(iParam && m_bMCUReadyToUpdate!=iParam)
	{
		m_bMCUReadyToUpdate = iParam;

		RETAILMSG(DEBUG_UPDATE,(_T("--->>CPanelUpdate OnEnterPanel :%d\n"),iParam));

		if(iParam)
		{
			SetEvent(m_hMCUReadyEvent);
		}
	}
}

void CPanelUpdate::Render()
{
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;

	if(!BeginRender())return;

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

	//device status
	SIZE si;
	wchar_t temp[16];

	if(m_pTextGL)
	{
		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);

		if(m_strTitle.String())
		{
			m_pTextGL->SetColor(&cr);
			m_pTextGL->SetString(m_strTitle.String());
			m_pTextGL->SetCharScale(1.4f,1.4f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),160);
			m_pTextGL->Render();
		}

		if(m_strNotification.String())
		{
			m_pTextGL->SetString(m_strNotification.String());
			m_pTextGL->SetCharScale(1.2f,1.2f);
			m_pTextGL->GetExtension(&si);
	// 		m_pTextGL->SetPos(-310,m_strTitle.String()?-20:26);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),80);
	// 		m_pTextGL->EnableAutoChangeLine(TRUE,620);
			m_pTextGL->Render();
		}

		//draw percent:
		swprintf_s(temp,16,_T("%d%%"),m_sliProgress.GetValue()*100/512);
		m_pTextGL->SetString(temp);
		m_pTextGL->SetCharScale(1.4f,1.4f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(GLfloat(-si.cx/2),-30);
		m_pTextGL->Render();
	}

	m_sliProgress.SetAlpha(m_fCurAlpha);
	m_sliProgress.Render();

	EndRender();

}
BOOL CPanelUpdate::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableQuit = FALSE;

	m_hACKEvent = CreateEvent(NULL,FALSE,FALSE,_T("MCU_UPDATE_ACK_EVENT"));
	m_hMCUReadyEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	m_strTitle = GetResourceString(IDS_MCU_UPDATING);
	m_strNotification = GetResourceString(IDS_MCU_UPDATING_ALERT);


	//slider
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=g_iClientWidth-280;//520/*776*/;
	sli.size.cy=32;
	sli.bHasBtns=FALSE;
	sli.bFilled=TRUE;
	sli.bFilledBoth=FALSE;

	m_sliProgress.Initialize(0,NULL,&sli,NULL);
	m_sliProgress.SetPos(0.0f,-130.0f);
	m_sliProgress.SetRange(0,512);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoBackFill(TRUE);
	m_sliProgress.ShowThumb(FALSE);

	return TRUE;
}


void CPanelUpdate::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelUpdate::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelUpdate::OnTouchMove(POINT *pt)
{

}

void CALLBACK CPanelUpdate::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_UPDATE,(_T("CPanelUpdate OnNotify id:%d,msg:%d,para:%d\n"),idControl,wMsg,wPara));

	CPanelUpdate *panel=(CPanelUpdate*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelUpdate::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_NB_BTN_CANCEL://cancel
		break;
	case IDC_NB_BTN_OK://ok
		break;
	case IDC_NB_BTN_HOME:
		//::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_MAINMENU,NULL);
		break;
	case IDC_NB_BTN_RETURN:
		OnBack();
		break;
	}
}
void CPanelUpdate::OnBack()
{
	SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
}
void CPanelUpdate::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

	if(dwType == 0)//package
	{

	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
		case MCU_UPDATE_STATUS:
			{
				BOOL bUpdate=pCommander->GetItemData(MCU_UPDATE_STATUS);
				m_bMCUReadyToUpdate = bUpdate;

				RETAILMSG(DEBUG_UPDATE,(_T("--->>CPanelUpdate MCU_UPDATE_STATUS:%d\n"),bUpdate));

				if(bUpdate)
				{
					SetEvent(m_hMCUReadyEvent);
				}
			}
			break;
		}
	}
}
void CPanelUpdate::OnMcuUpdateMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case UPDATE_MCU_SUCCEED:
		{
		}
		break;
	case UPDATE_MCU_FAILED:
		{
		}
		break;
	case UPDATE_MCU_UPDATING:
		{
		}
		break;
	case UPDATE_MCU_REQUST_SUCCEED:
		{
		}
		break;
	case UPDATE_MCU_REQUST_FAILED:
		{
		}
		break;
	}
}
void CPanelUpdate::OnMcuUpdateACK(INT ack)
{
	//RETAILMSG(DEBUG_UPDATE,(_T("CPanelUpdate OnMcuUpdateACK :%d\n"),ack));

	SetEventData(m_hACKEvent,ack);
	SetEvent(m_hACKEvent);
}
BOOL CPanelUpdate::RequestUpdate()
{
	ResetEvent(m_hMCUReadyEvent);

	RETAILMSG(DEBUG_UPDATE,(_T("CPanelUpdate:: RequestUpdate \n")));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_UPDATE_STATUS,1);
	}

	return TRUE;
}
BOOL CPanelUpdate::CancelUpdate()
{
	RETAILMSG(DEBUG_UPDATE,(_T("CPanelUpdate:: CancelUpdate \n")));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_UPDATE_STATUS,0);
	}

	return TRUE;
}

BOOL CPanelUpdate::PushPack(BYTE *pBuff,INT iLen,INT iSendDelay)
{
	if(!m_pCmdSender)
	{
		m_pCmdSender = new CmdSender_McuUpdate();
		if(m_pCmdSender)
		{
			m_pCmdSender->LinkCommander(m_pCommander);
		}
	}

	if(m_pCmdSender)
	{
		m_pCmdSender->QueueCmdBuff(0,pBuff,iLen,iSendDelay);
	}

	if(m_pCmdSender)
	{
		return m_pCmdSender->WaitSendFinish();
	}
	else
	{
		return FALSE;
	}
}
void CPanelUpdate::StartMcuUpdate()
{
	m_bUpdating = TRUE;
	::PostMessage(m_hMainWnd,WM_MCU_UPDATE_MSG,UPDATE_MCU_UPDATING,0);//updating

	HANDLE h = CreateThread(NULL,NULL,CPanelUpdate::UpdateMcuThreadPro,this,NULL,NULL);
	//CeSetThreadPriority(h, CE_THREAD_PRIO_256_BELOW_NORMAL);
	CloseHandle(h);
}
DWORD CPanelUpdate::UpdateMcuThreadPro(LPVOID lpParam)
{
	RETAILMSG(DEBUG_UPDATE,(_T("@@@@@@ UpdateMcuThreadPro enter\r\n")));

	CPanelUpdate *p= (CPanelUpdate*)lpParam;

	Sleep(1000);

	if(p)
	{
		p->UpdateMcuProc();
	}

	RETAILMSG(DEBUG_UPDATE,(_T("@@@@@@ UpdateMcuThreadPro quit\r\n")));

	return 0;
}
void CPanelUpdate::UpdateMcuProc()
{
	const int ipack_size=128;//240;

	const wchar_t* filename = UPDATE_FILE_MCU_USBDISK;
	const wchar_t* filechecksum = UPDATE_FILE_MCU_CKECKSUM_USBDISK;

	int try_cnt=10;

	RETAILMSG(DEBUG_UPDATE,(_T("\n*************wait for MCU update ready *************\n")));

	while(WAIT_TIMEOUT == WaitForSingleObject(m_hMCUReadyEvent,3000))
	{
		((CCommanderEx*)m_pCommander)->IniMCU();
	}

	RETAILMSG(DEBUG_UPDATE,(_T("\n************* MCU update is ready *************\n")));

	Sleep(1000);

	//
	BOOL bRequstSucc=FALSE;

	while(!bRequstSucc /*&& try_cnt>0*/)
	{
		if(!RequestUpdate())
		{
			bRequstSucc = FALSE;
		}
		else
		{
			//wait for mcu ready:
			bRequstSucc = (WaitForSingleObject(m_hMCUReadyEvent,5000)==WAIT_OBJECT_0);
		}

		try_cnt--;
	}

	if(!bRequstSucc)
	{
		RETAILMSG(DEBUG_UPDATE,(_T("UpdateMcuProc Failed!  MCU not respond\r\n")));

		m_bUpdating = FALSE;

		m_strTitle = GetResourceString(IDS_MCU_UPDATE_FAILED);
		m_strNotification = NULL;//GetResourceString(IDS_MCU_UPDATE_FAILED);

		Sleep(1000);

		m_bEnableQuit = TRUE;FALSE;//
		::PostMessage(m_hMainWnd,WM_MCU_UPDATE_MSG,UPDATE_MCU_FAILED,0);//update failed

		return;
	}

	//wait for update file ready
// 	while (!IsUpdateFileExists(filename))
	while (!IsValidMcuUpdateFile(filename,filechecksum))
	{
	    RETAILMSG(DEBUG_UPDATE,(_T("Can not found %s\n"),filename));

		m_strNotification = GetResourceString(IDS_MCU_UPDATING_INVALID_FILE);
		Sleep(2000);
	}

	m_strNotification = GetResourceString(IDS_MCU_UPDATING_ALERT);

	//read file:
	int iDataLen=0;
	int iPushed=0;
	FILE	*file = NULL;
	BYTE	 *localBuffer;

	RETAILMSG(DEBUG_UPDATE,(_T("\n*************Get MCU update file: %s*************\n"),filename));

	if ( 0!=_wfopen_s(&file, filename, L"rb") )
	{
		RETAILMSG(DEBUG_UPDATE,(_T("ERROR: MCU update file not found,%s\n"),filename));
		m_bUpdating = FALSE;
		m_bEnableQuit = TRUE;
		return  ;
	}

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	iDataLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	localBuffer = new BYTE[iDataLen];
	if(!localBuffer)
	{
		m_bUpdating = FALSE;
		m_bEnableQuit = TRUE;
		fclose(file);
		return ;
	}

	fread(localBuffer, iDataLen, 1, file);

	fclose(file);

	RETAILMSG(DEBUG_UPDATE,(_T("\n*************Get MCU update file SUCCEED, size:%d*************\n"),iDataLen));

	Sleep(1000);

	//push data to mcu:
	while (iPushed<iDataLen)
	{
		//test
		int ilen=min(ipack_size,iDataLen-iPushed);
		if(PushPack(localBuffer+iPushed,ilen))
		{
			//RETAILMSG(DEBUG_UPDATE,(_T("@@@@@@ UpdateMcuThreadPro PushPack:len:%d\r\n"),ilen));
			//Sleep(100);
			iPushed+=ilen;
		}
		else
		{
			Sleep(1000);
		}

		m_sliProgress.SetValue(iPushed*512/iDataLen);

		//Sleep(2000);
	}


	//clean:
	delete[] localBuffer ;

	m_strTitle = GetResourceString(IDS_MCU_UPDATE_SUCCEED);

	Sleep(1000);

	CancelUpdate();

	//
	m_bUpdating = FALSE;
	m_bEnableQuit = TRUE;
	::PostMessage(m_hMainWnd,WM_MCU_UPDATE_MSG,UPDATE_MCU_SUCCEED,0);//update successful

	Sleep(2000);
}
BOOL CPanelUpdate::IsUpdateFileExists(LPCTSTR   lpszFileName)
{
	WIN32_FIND_DATA   wfd;
	BOOL   bRet;
	HANDLE   hFind;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   hFind   !=   INVALID_HANDLE_VALUE;
	FindClose(hFind);
	return   bRet;
}
