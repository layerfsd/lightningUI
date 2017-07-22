// #include "../StdAfx.h"
#include "PanelMHL.h"
#include "../../resource.h"

CPanelMHL::CPanelMHL(void)
{
	m_dwLastFullScreenTick=0;

	m_bFullScreenMode=FALSE;

	m_bIniMHL = FALSE;
	m_hDevMHL = INVALID_HANDLE_VALUE;

	m_bSpeechOn = FALSE;

	m_pVideoWindow = NULL;
}

CPanelMHL::~CPanelMHL(void)
{
	DeiniMHL();
}
BOOL CPanelMHL::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelMHL::OnSRCChanged(UINT idSRC)
{
	RETAILMSG(DEBUG_MHL,(_T("CPanelMHL::OnSRCChanged %d\r\n"),idSRC));

// 	if(idSRC != UI_MHL)
// 	{
// 		
// 	}
}


void CPanelMHL::StartMHL()
{
	//test: start
	if(m_hDevMHL!=NULL && m_hDevMHL!=INVALID_HANDLE_VALUE)
	{
		if (!DeviceIoControl(m_hDevMHL, IOCTL_MHL_START, NULL, 0, NULL, 0, NULL, NULL))
		{
			RETAILMSG(DEBUG_MHL, (TEXT("Failed DeviceIoControl IOCTL_MHL_INIT -- error = 0x%x\r\n"), GetLastError()));
		}
	}
}
void CPanelMHL::StopMHL()
{
	//stop
	if(m_hDevMHL!=NULL && m_hDevMHL!=INVALID_HANDLE_VALUE)
	{
		if (!DeviceIoControl(m_hDevMHL, IOCTL_MHL_STOP, NULL, 0, NULL, 0, NULL, NULL))
		{
			RETAILMSG(DEBUG_MHL, (TEXT("Failed DeviceIoControl IOCTL_MHL_INIT -- error = 0x%x\r\n"), GetLastError()));
		}
	}
}
void CPanelMHL::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
 	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

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


	//MCU command:
	SwitchAudioSRC(SRC_AUDIO_MHL);

	StartMHL();

	//MHL can bus:
	::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_OTHER,0x10);
// 	::PostMessage(m_hMainWnd,WM_NOTIFY_MEDIA_PLAYBACK_INFO,0x0,(0x0<<16)+0x10);

	if(m_pVideoWindow)
	{
// 		m_pVideoWindow->SetWindowMode(VIP_WND_PIP_NORMAL);
		m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
	}

	m_dwEnterTick = GetTickCount();

}
void CPanelMHL::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	StopMHL();

	if(m_pVideoWindow && m_pVideoWindow->IsRunning())
	{
		m_pVideoWindow->StopVideo();
	}

}
void CPanelMHL::Render()
{
	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;


	if(IsAniReady() && !m_bIniMHL)
	{
		RETAILMSG(DEBUG_MHL, (TEXT("^^^^^^^^^^^IniMHL \r\n")));

		if(IniMHL())
		{
			StartMHL();
		}
	}

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};
	COLORGL cr_b={0.0,0.0,0.0,0.6};

	m_btnCancel.Show(m_videoState != VSTATE_VideoOn);

	{
		int x=168;
		int y=72;
		int cx=464;
		int cy=256;

		if(m_pVideoWindow)
		{
// 			m_pVideoWindow->SetWindowMode(VIP_WND_PIP_NORMAL);
// 			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
			m_pVideoWindow->GetWindowPos(&x,&y,&cx,&cy);
		}

// 		DrawRect(pTexVideoBack,-(400.0f-x-cx/2),(240.0f-y-cy/2),cx-4,cy-56,50,&cr,TRUE);
// 		DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr_b);

		/*
		if(m_bShowAlert)
		{
			COLORGL cr2={1,0,0,1};
			m_iconAlert.Render();

			m_pTextGL->SetString((const wchar_t*)::LoadString( m_hInstance, IDS_WARNING+g_idLanguageOffset, NULL, 0 ));
			m_pTextGL->SetCharScale(1.0f,1.0f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),30);
			m_pTextGL->SetColor(&cr2);
			m_pTextGL->SetAlpha(1.0f);
			m_pTextGL->SetLuminance(1.0f);
			m_pTextGL->Render();

			m_pTextGL->SetString((const wchar_t*)::LoadString( m_hInstance, IDS_CANNOT_WATCH_VIDEO+g_idLanguageOffset, NULL, 0 ));
			m_pTextGL->SetCharScale(0.7f,0.7f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),-20);
			//m_pTextGL->SetColor(&cr2);
			m_pTextGL->Render();

		}
		else
		*/
		{
			if(/*m_bMHLVideoOn*/m_videoState == VSTATE_VideoOn)
			{
				if(IsAniReady() && m_bPanelEntered)
				{
					if(m_pVideoWindow && !m_pVideoWindow->IsRunning())
					{
						m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_MHL);
// 						m_pVideoWindow->SetColorSystem(NTSM_MJ);
// 						m_pVideoWindow->SetClickFunc(&CPanelMHL::OnVideoWndDown,&CPanelMHL::OnVideoWndUp,
// 							&CPanelMHL::OnVideoWndMove,&CPanelMHL::OnVideoWndDblClick,this);

						m_pVideoWindow->SetGestureFunc(&CPanelMHL::OnVideoWndGesture,this);

						m_pVideoWindow->StartVideo();
					}
				}
			}
			else
			{

				if(m_pVideoWindow && m_pVideoWindow->IsRunning())
				{
					m_pVideoWindow->StopVideo();
				}


				m_tagTitle.SetAlpha(m_fCurAlpha);
				m_tagTitle.Render();

				m_iconMHL.SetAlpha(m_fCurAlpha);
				m_iconMHL.Render();

				m_btnCancel.SetAlpha(m_fCurAlpha);
				m_btnCancel.Render();
			}

		}
	}


	EndRender();
}
BOOL CPanelMHL:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bShowStatusBar = FALSE;

// 	IniMHL();

	m_iconMHL.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_127));
// 	m_iconMHL.SetShowScale(1.4);
// 	m_iconMHL.SetPos(0,140);
	m_iconMHL.SetShowScale(2.0f);
	m_iconMHL.SetPos(0,80);

	COLORGL cr={1,1,1,1.0};
	COLORGL cr_down={0.465,0.824,0.965,1.0};
	SIZE si0={800,120};

	//cancel
	m_btnCancel.Initialize(IDC_MHL_BTN_CANCEL,this,m_pSkinManager->GetTexture(TEXID_047),NULL,
		m_pSkinManager->GetTexture(TEXID_046),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr,&cr_down);
	m_btnCancel.SetCaptionHeight(32);
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetSize(&si0);
	m_btnCancel.SetNotifyFunc(&CPanelMHL::OnNotify,this);
	m_btnCancel.SetPos(0,-180);


	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(GetResourceString(IDS_MHL_MAIN_PROMPT));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-70);
	m_tagTitle.SetCaptionHeight(24);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);

	//
	SetVideoState(VSTATE_Off);

	return TRUE;
}

void CPanelMHL::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_MHL_BTN_HOME:
		break;
	case IDC_MHL_BTN_CANCEL:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		break;
	}
}

void CPanelMHL::OnBack()
{
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
}
void CPanelMHL::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}

BOOL CPanelMHL::IniMHL()
{
	m_hDevMHL = CreateFile(TEXT("MHL1:"), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, 0);

	if (m_hDevMHL != INVALID_HANDLE_VALUE)
	{
		RETAILMSG(DEBUG_MHL, (TEXT("CPanelMHL::open MHL succeed\r\n")));

		if (!DeviceIoControl(m_hDevMHL, IOCTL_MHL_INIT, NULL, 0,	NULL, 0, NULL, NULL))
		{

			RETAILMSG(DEBUG_MHL, (TEXT("Failed DeviceIoControl IOCTL_MHL_INIT -- error = 0x%x\r\n"), GetLastError()));
		}
// 		//test: start
// 		if(m_hDevMHL!=NULL && m_hDevMHL!=INVALID_HANDLE_VALUE)
// 		{
// 			if (!DeviceIoControl(m_hDevMHL, IOCTL_MHL_START, NULL, 0, NULL, 0, NULL, NULL))
// 			{
// 				RETAILMSG(DEBUG_MHL, (TEXT("Failed DeviceIoControl IOCTL_MHL_INIT -- error = 0x%x\r\n"), GetLastError()));
// 			}
// 		}

	}
	else
	{
		RETAILMSG(DEBUG_MHL, (TEXT("CPanelMHL::open MHL failed:%x\r\n"),GetLastError()));
	}

	m_bIniMHL = (m_hDevMHL != INVALID_HANDLE_VALUE);

	return m_hDevMHL != INVALID_HANDLE_VALUE;
}
BOOL CPanelMHL::DeiniMHL()
{
	if(m_hDevMHL!= INVALID_HANDLE_VALUE && m_hDevMHL!=NULL)
	{
		CloseHandle(m_hDevMHL);
	}

	return TRUE;
}

void CPanelMHL::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelMHL::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);
}
void CPanelMHL::SetVideoState(Video_State_Type state)
{
	m_videoState = state;

	switch (state)
	{
	case VSTATE_Off:
	case VSTATE_SWReset:
		m_tagTitle.SetCaption(GetResourceString(IDS_MHL_MAIN_PROMPT));
		break;
	case VSTATE_SyncWait:
		m_tagTitle.SetCaption(GetResourceString(IDS_MHL_WAITING));
		break;
	case VSTATE_VideoOn:
		m_tagTitle.SetCaption(GetResourceString(IDS_MHL_CONNECTED));
		break;
	default:
		m_tagTitle.SetCaption(GetResourceString(IDS_MHL_CONNECTING));

		break;
	}

	if(state != VSTATE_VideoOn)
	{
		if(m_pVideoWindow && m_pVideoWindow->IsRunning())
		{
			m_pVideoWindow->StopVideo();
		}
	}
}


void CPanelMHL::OnRCPKey(BYTE ucKey)
{
	if(m_hDevMHL!=NULL && m_hDevMHL!=INVALID_HANDLE_VALUE)
	{
		BYTE key=ucKey;
		if (!DeviceIoControl(m_hDevMHL, IOCTL_MHL_RCP, &key, 1, NULL, 0, NULL, NULL))
		{
			RETAILMSG(DEBUG_MHL, (TEXT("Failed DeviceIoControl IOCTL_MHL_INIT -- error = 0x%x\r\n"), GetLastError()));
		}
	}
}
void CPanelMHL::OnMHLMsg(WPARAM wParam, LPARAM lParam)
{
	UINT msg = HIWORD(wParam);
	UINT val = LOWORD(wParam);

	switch (msg)
	{
	case MHL_VIDEO_MSG:
		{
			BOOL bVideoOn=(val==VSTATE_VideoOn);
			SetVideoState((Video_State_Type)val);
			if(bVideoOn)
			{
				//::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_MHL,NULL);
			}
			else
			{
// 				if(m_pSkinManager)
// 				{
// 					UINT idPresrc=m_pSkinManager->GetConfig()->GetCurParam()->idPreSRC;
// 					UINT idCurUI=m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
// 					if(idCurUI == UI_MHL)
// 					{
// // 						::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,(idPresrc != UI_MHL)?idPresrc:UI_MAINMENU,NULL);
// // 						::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_MAINMENU,NULL);
// 					}
// 				}
			}
		}
		break;
	}
}
void CPanelMHL::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
void CPanelMHL::LinkVideoWindow(CVideoWindow *pVideoWindow)
{
	m_pVideoWindow = pVideoWindow;
}
BOOL CPanelMHL::OnKey(UINT idKey)
{
	switch (idKey)
	{
	case KEY_MCU_SEEK_PREV:
		OnRCPKey(RCP_PAUSE);
		break;
	case KEY_MCU_SEEK_NEXT:
		OnRCPKey(RCP_PLAY);
		break;
	case KEY_MCU_ENTER:
		OnRCPKey(RCP_SELECT);
		break;
	}

	return TRUE;

}

void CPanelMHL::OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lparam)
{

}

/*
void CPanelMHL::OnVideoWndDown(POINT *pt,LPVOID lparam)
{
	CPanelMHL *panel = (CPanelMHL*)lparam;

	RETAILMSG(DEBUG_MHL,(_T("CPanelMHL OnVideoWndDown %d,%d\r\n"),pt->x,pt->y));

// 
// 	PCHAR reportBuf = NULL; 
// 	ULONG reportSize = 0;
// 	HidHostInfo* pHostInfo = g_BTModule.pApi->apiHID.GetHostInfo();
// 
// // 	if (!m_bHidConnected)
// // 		return;
// // 
// // 	m_bPressed = TRUE;
// 
// 	if (IS_MOUSE(g_BTModule.pApi->apiHID.GetFeatureMask()))
// 	{
// 		// Move mouse firstly when touch is down. 
// 		g_BTModule.pApi->apiHID.HandleTouchMessage(WM_MOUSEMOVE, 0, pt->x,pt->y);
// 
// 		Sleep(pHostInfo->idleTime);
// 	}
// 
// 	g_BTModule.pApi->apiHID.HandleTouchMessage(WM_LBUTTONDOWN, MK_LBUTTON, pt->x,pt->y);
// 
	
}
void CPanelMHL::OnVideoWndUp(POINT *pt,LPVOID lparam)
{
	RETAILMSG(DEBUG_MHL,(_T("CPanelMHL OnVideoWndUp %d,%d\r\n"),pt->x,pt->y));

	return;

	
// 	if(m_pVideoWindow)
// 	{
// 		UINT idLastMode=m_pVideoWindow->GetWindowMode();
// 
// 		if(idLastMode==VIP_WND_FULLSCREEN)
// 		{
// 			if(pt->x>700 && pt->y<100)
// 			{
// 				m_pVideoWindow->SetWindowMode(VIP_WND_PIP_DISK);
// 			}
// 			else
// 			{
// 				//send command:
// 			}
// 		}
// 		else
// 		{
// 			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
// 		}
// 	}
	
}
void CPanelMHL::OnVideoWndMove(POINT *pt,LPVOID lparam)
{
	// TODO: Add your message handler code here and/or call default
// 	if (!m_bHidConnected)
// 		return;

	BTDBG(ZONE_DEBUGONLY,(TEXT("<%s> point(%03d, %03d)\r\n"), _T(__FUNCTION__), pt->x,pt->y));


}
void CPanelMHL::OnVideoWndDblClick(POINT *pt,LPVOID lparam)
{

}
*/

void CALLBACK CPanelMHL::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_MHL,(_T("CPanelMHL OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelMHL *panel=(CPanelMHL*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelMHL::OnSpeechStart()
{
	RETAILMSG(DEBUG_MHL,(_T("............CPanelMHL OnSpeechStart......................\r\n")));

	m_bSpeechOn = TRUE;

}
void CPanelMHL::OnSpeechTerminate()
{
	RETAILMSG(DEBUG_MHL,(_T("............CPanelMHL OnSpeechTerminate......................\r\n")));
	m_bSpeechOn = FALSE;
}
