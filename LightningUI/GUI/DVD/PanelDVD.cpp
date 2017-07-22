#include "PanelDVD.h"
#include "../../resource.h"

#if CVTE_EN_DVD
CPanelDVD::CPanelDVD(void)
{
	m_pVideoWindow = NULL;

	m_dwEnterTick = 0;
    m_dwVideoStartTick = 0;

    m_bShowAlert = TRUE;
    m_bPhoneCallActive = FALSE;
	m_bSpeechOn = FALSE;
}

CPanelDVD::~CPanelDVD(void)
{
}
BOOL CPanelDVD::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelDVD::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);


	if(m_pVideoWindow && m_pVideoWindow->IsRunning())
	{
		RETAILMSG(DEBUG_DVD,(_T("MSG:[CPanelDVD::OnQuitPanel]: StopVideo\r\n")));

		m_pVideoWindow->SetGestureFunc(NULL,NULL);
		m_pVideoWindow->StopVideo();
	}

}
void CPanelDVD::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

    RETAILMSG(1,(_T("++DVD: OnEnterPanel\r\n")));

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

	m_dwEnterTick = GetTickCount();

	//switch video:
	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_VIDEO_SW_KEY,SRC_VIDEO_DVD);
// 		m_pCommander->SendCmd(_ARM_VIDEO_SW_KEY,SRC_VIDEO_CAMERA);
	}

	//switch audio:
	if(GetAudioSRC() != SRC_AUDIO_DVD /*&& !m_bSpeechOn */&& !m_bPhoneCallActive)
        SwitchAudioSRC(SRC_AUDIO_DVD,TRUE);

//	    MuteAudio(FALSE,FALSE,_ARM_MUTE_ALL,TRUE,100);
}
void CPanelDVD::Render()
{
// 	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_078):NULL;
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_043):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;

	BOOL bHasSignal=FALSE;
    SIZE si;

	//start video:
	if(!m_bShowAlert)
	{
		if(!m_bOnReverseMode)
		{
	    	m_iconDVD.SetAlpha(m_fCurAlpha);
	    	m_iconDVD.Render();

			m_tagDVDTitle.SetAlpha(m_fCurAlpha);
			m_tagDVDTitle.Render();
		}

    	if(!m_bOnReverseMode && IsReady() && GetTickCount()-m_dwEnterTick>1800)
    	{
    		if(m_pVideoWindow && !m_pVideoWindow->IsRunning())
    		{
    			RETAILMSG(DEBUG_DVD,(_T("++DVD: StartVideo++ iDVDStandard[%d]\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iDVDStandard));

    			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_DVD);
    			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iDVDStandard);	// NTSC
    			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
    			m_pVideoWindow->SetGestureFunc(&CPanelDVD::OnVideoWndGesture,this);

    			m_pVideoWindow->StartVideo();

                m_dwVideoStartTick = GetTickCount();
    		}
			else
			{
    			//RETAILMSG(DEBUG_DVD,(_T("++DVD: Render: m_pVideoWindow->IsRunning():%d\r\n"),m_pVideoWindow->IsRunning()));
			}
    	}
		else
		{
    		//RETAILMSG(DEBUG_DVD,(_T("++DVD: Render: m_bOnReverseMode:%d IsReady:%d time:%d\r\n"),m_bOnReverseMode,IsReady(),GetTickCount()-m_dwEnterTick));
		}

    	if(m_pVideoWindow)
    	{
    		bHasSignal=m_pVideoWindow->HasVideoSignal();
    	}

    	if(!bHasSignal && m_pVideoWindow->IsRunning() && GetTickCount()-m_dwVideoStartTick>2000)
    	{
    		m_tagTitle.SetAlpha(m_fCurAlpha);
    		m_tagTitle.Render();
    	}
	}
	else
    {
		COLORGL cr2={1,0,0,1};
		m_iconAlert.Render();

		m_pTextGL->SetString(GetResourceString(IDS_WARNING));
		m_pTextGL->SetCharScale(1.0f,1.0f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(GLfloat(-si.cx/2),10);
		m_pTextGL->SetColor(&cr2);
		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);
		m_pTextGL->Render();

		m_pTextGL->SetString(GetResourceString(IDS_CANNOT_WATCH_VIDEO));
		m_pTextGL->SetCharScale(0.65f,0.65f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(GLfloat(-si.cx/2),-40);
		//m_pTextGL->SetColor(&cr2);
		m_pTextGL->Render();
    }
	EndRender();

}
BOOL CPanelDVD::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);


	m_bShowStatusBar = FALSE;
	m_bEnableSmartBar = TRUE;

    m_iconAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_084));
	m_iconAlert.SetShowScale(1.44f);
	m_iconAlert.SetPos(0,90);

	m_iconDVD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_120));
	m_iconDVD.SetShowScale(1.2f);

	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(GetResourceString(IDS_NO_SIGNAL));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-160);
	m_tagTitle.SetCaptionHeight(28);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);

	m_tagDVDTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDVDTitle.SetSize(&si1);
	m_tagDVDTitle.SetCaption(_T(" "));
	m_tagDVDTitle.LinkTextGL(pTextGL);
	m_tagDVDTitle.SetPos(0,-80);
	m_tagDVDTitle.SetCaptionHeight(20);
	m_tagDVDTitle.EnableDrawBack(FALSE);
	m_tagDVDTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	/*
	//toolbar:
	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_151));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_152));

	m_iconSettingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_155));
	m_iconSettingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_156));

	SIZE si1={140,80};
	m_btnHome.Initialize(IDC_REARVIEW_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
		m_pSkinManager->GetTexture(TEXID_056),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelDVD::OnNotify,this);
	m_btnHome.SetSize(&si1);
	m_btnHome.SetPos(-330,-200);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);

	m_btnSetting.Initialize(IDC_REARVIEW_BTN_SETTING,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
		m_pSkinManager->GetTexture(TEXID_056),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSetting.SetNotifyFunc(&CPanelDVD::OnNotify,this);
	m_btnSetting.SetSize(&si1);
	m_btnSetting.SetPos(330,-200);
	m_btnSetting.SetIcon(&m_iconSettingUp,&m_iconSettingDown);
*/



	return TRUE;
}


void CALLBACK CPanelDVD::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_DVD,(_T("APP:MSG:CPanelDVD OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelDVD *panel=(CPanelDVD*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelDVD::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_TV_BTN_QUIT:
		break;

	}
}
void CPanelDVD::OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lpContext)
{
	CPanelDVD *panel=(CPanelDVD*)lpContext;

	GESTUREINFO gi;
	memset(&gi, 0, sizeof(gi));
	gi.cbSize = sizeof(GESTUREINFO);
	if (GetGestureInfo((HGESTUREINFO)lParam, &gi))
	{

		// Handle gesture indicated by wParam or gi.dwID
		switch (gi.dwID)
		{
		case GID_BEGIN:
			break;
		case GID_END:
			break;
		case GID_PAN:
			break;
		case GID_ROTATE:
			break;
		case GID_SCROLL:
			break;
		case GID_HOLD:
			break;
		case GID_DOUBLESELECT:
			break;
		case GID_SELECT:
			{
				POINT pt={gi.ptsLocation.x,gi.ptsLocation.y};
				panel->SendPointCmd(&pt);
				PlaySoundEffect(0);
			}
			break;
		}
	}

}
void CPanelDVD::SendPointCmd(POINT *pt)
{
	if(m_pCommander)
	{
		RETAILMSG(DEBUG_DVD,(_T("APP:MSG:CPanelDVD GID_SELECT: ptx:%d,pty:%d\r\n"),pt->x,pt->y));
		BYTE x=(BYTE)(pt->x*255/g_iScreenWidth);
		BYTE y=(BYTE)(pt->y*255/g_iScreenHeight);

		RETAILMSG(DEBUG_DVD,(_T("APP:MSG:CPanelDVD GID_SELECT: x:%d,y:%d\r\n"),x,y));

//	        if(x==0 && y==0)
//	        {
//	            RETAILMSG(DEBUG_DVD,(_T("APP:MSG:CPanelDVD SwitchPage HOME\r\n"),x,y));
//	            SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
//	        }

		m_pCommander->SendCmd(_ARM_TV_X_KEY,x);
		m_pCommander->SendCmd(_ARM_TV_Y_KEY,y);
	}
}
void CPanelDVD::OnReverseModeChange(BOOL bReverse)
{
	RETAILMSG(DEBUG_DVD,(_T("MSG:[CPanelDVD::OnReverseModeChange]: %d\r\n"),bReverse));

	CPanelGL::OnReverseModeChange(bReverse);

	if(bReverse && m_pVideoWindow && m_pVideoWindow->IsRunning())
	{
		RETAILMSG(DEBUG_DVD,(_T("MSG:[CPanelDVD::OnReverseModeChange]: StopVideo\r\n")));
		m_pVideoWindow->SetGestureFunc(NULL,NULL);
		m_pVideoWindow->StopVideo();
	}
	else if(!bReverse && !m_bShowAlert && m_pVideoWindow && (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_DISK,PAGE_DISK_WRAPPER)))
	{
		if(m_pVideoWindow->IsRunning())
		{
			RETAILMSG(DEBUG_DVD,(_T("MSG:[CPanelDVD::OnReverseModeChange]: Stop and Restart video again\r\n")));
    		m_pVideoWindow->SetGestureFunc(NULL,NULL);
    		m_pVideoWindow->StopVideo();

			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_DVD);
			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iDVDStandard);	// NTSC
			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
			m_pVideoWindow->SetGestureFunc(&CPanelDVD::OnVideoWndGesture,this);

			m_pVideoWindow->StartVideo();

	        m_dwVideoStartTick = GetTickCount();
		}
		else
		{
			RETAILMSG(DEBUG_DVD,(_T("MSG:[CPanelDVD::OnReverseModeChange]: Restart video again\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iDVDStandard));

			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_DVD);
			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iDVDStandard);	// NTSC
			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
			m_pVideoWindow->SetGestureFunc(&CPanelDVD::OnVideoWndGesture,this);

			m_pVideoWindow->StartVideo();

	        m_dwVideoStartTick = GetTickCount();
		}
	}
	else
	{
		RETAILMSG(DEBUG_DVD,(_T("MSG:[CPanelDVD::OnReverseModeChange]: bReverse[%d] IsRunning[%d] idCurSRC[0x%x]\r\n"),bReverse,m_pVideoWindow->IsRunning(),m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC));
	}
}

void CPanelDVD::OnBrakeStatusChanged(BOOL bOn)
{
	CPanelGL::OnBrakeStatusChanged(bOn);

	ShowAlert(!bOn);

	RETAILMSG(1,(_T("MSG:[CPanelDVD::OnBrakeStatusChanged] m_bShowAlert[%d]\r\n"),m_bShowAlert));

	if(m_bShowAlert)
	{
        if(m_pVideoWindow && m_pVideoWindow->IsRunning())
        {
			RETAILMSG(DEBUG_DVD,(_T("MSG:[CPanelDVD::OnBrakeStatusChanged]: StopVideo\r\n")));

    		m_pVideoWindow->SetGestureFunc(NULL,NULL);
    		m_pVideoWindow->StopVideo();
        }
    }
}

void CPanelDVD::OnSpeechStart()
{
	m_bSpeechOn = TRUE;
}
void CPanelDVD::OnSpeechTerminate()
{
	m_bSpeechOn = FALSE;
}

void CPanelDVD::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;
    RETAILMSG(1, (TEXT("MSG:[CPanelDVD::OnPhoneCallState]: m_bPhoneCallActive:%d\r\n"),m_bPhoneCallActive));

    if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_DISK)
	{
		if(wParam)//active
		{
		}
		else
		{
            //if(!m_bSpeechOn)
            {
			    SwitchAudioSRC(SRC_AUDIO_DVD,TRUE);
            }
		}
	}
}//

#if CVTE_EN_KEY_PROXY
BOOL CPanelDVD::OnKey(UINT idKey,UINT idStatus)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelDVD::OnKey]: key=0x%x, Status=0x%x\r\n"),idKey,idStatus));
	switch (idKey)
	{
	case KEY_PREVIOUS:
		if ( idStatus == EV_KEY_UP )
		{
            if(!m_bSpeechOn && !m_bPhoneCallActive)
            {
                RETAILMSG(1, (TEXT("MSG:[CPanelDVD::OnKey]: Prev\r\n")));
	            if(m_pCommander)
	            {
		            m_pCommander->SendCmd(_ARM_TV_PREVNEXT_KEY,1);
                }
            }
		}
		break;
	case KEY_NEXT:
		if ( idStatus == EV_KEY_UP )
		{
			if(!m_bSpeechOn && !m_bPhoneCallActive)
			{
				RETAILMSG(1, (TEXT("MSG:[CPanelDVD::OnKey]: Next\r\n")));
				if(m_pCommander)
				{
					m_pCommander->SendCmd(_ARM_TV_PREVNEXT_KEY,2);
				}
			}
		}
		break;
	case KEY_RESUME:
	case KEY_PLAY:
		if ( idStatus == EV_KEY_UP )
		{
			if(/*!m_bSpeechOn && */!m_bPhoneCallActive /*&& GetAudioSRC() != SRC_AUDIO_DVD*/)
			{
				SwitchAudioSRC(SRC_AUDIO_DVD,TRUE);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}
#else
BOOL CPanelDVD::OnKey(UINT idKey)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelDVD::OnKey]: idKey:%d\r\n"),idKey));
	switch (idKey)
	{
	case KEY_MCU_ENTER:
		break;
	case KEY_MCU_SEEK_NEXT:
		break;
	case KEY_MCU_SEEK_PREV:
		break;
	case KEY_MCU_TRACK_NEXT:
		break;
	case KEY_MCU_TRACK_PREV:
        break;
	case KEY_MEDIA_PREVIOUS:
            if(!m_bSpeechOn && !m_bPhoneCallActive)
            {
                RETAILMSG(1, (TEXT("MSG:[CPanelDVD::OnKey]: Prev\r\n")));
	            if(m_pCommander)
	            {
		            m_pCommander->SendCmd(_ARM_TV_PREVNEXT_KEY,1);
                }
            }
		break;
	case KEY_MEDIA_NEXT:
            if(!m_bSpeechOn && !m_bPhoneCallActive)
            {
                RETAILMSG(1, (TEXT("MSG:[CPanelDVD::OnKey]: Next\r\n")));
	            if(m_pCommander)
	            {
		            m_pCommander->SendCmd(_ARM_TV_PREVNEXT_KEY,2);
                }
            }
		break;
	case KEY_MEDIA_PLAY:
		{
			if(/*!m_bSpeechOn && */!m_bPhoneCallActive /*&& GetAudioSRC() != SRC_AUDIO_DVD*/)
			{
			    SwitchAudioSRC(SRC_AUDIO_DVD,TRUE);
			}
		}
		break;
	case KEY_MEDIA_PAUSE:
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		break;
	}

	return TRUE;
}

#endif

#endif
