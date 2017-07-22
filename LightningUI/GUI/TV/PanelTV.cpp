#include "PanelTV.h"
#include "../../resource.h"


#if CVTE_EN_DTV

#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
#define CVTE_ANDROID_TOUCH_FIRST_UP		0x00
#define CVTE_ANDROID_TOUCH_FIRST_DOWN	0x01
#define CVTE_ANDROID_TOUCH_SECOND_UP	0x10
#define CVTE_ANDROID_TOUCH_SECOND_DOWN	0x11
#endif

CPanelTV::CPanelTV(void)
{
	m_pVideoWindow = NULL;

	m_dwEnterTick = 0;
	m_dwReverseTick = 0;
	m_dwVideoStartTick = 0;

	m_bSpeechOn=FALSE;

    m_bVideoWindowShow=FALSE;

    m_bShowAlert=TRUE;
    m_bPhoneCallActive = FALSE;

#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	m_bShowAlert=FALSE;
	m_pTouchCmd = NULL;
	m_pTouchSender = NULL;
#endif
}

CPanelTV::~CPanelTV(void)
{
#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	if (m_pTouchSender != NULL)
	{
		delete m_pTouchSender;
		m_pTouchSender = NULL;
	}
	if (m_pTouchCmd != NULL)
	{
		delete m_pTouchCmd;
		m_pTouchCmd = NULL;
	}
#endif
}
BOOL CPanelTV::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelTV::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

    RETAILMSG(DEBUG_TV,(_T("++DTV: OnQuitPanel\r\n")));

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);


	if(m_pVideoWindow && m_pVideoWindow->IsRunning())
	{
		m_pVideoWindow->SetGestureFunc(NULL,NULL);
		m_pVideoWindow->SetClickFunc(NULL, NULL, NULL, NULL, NULL);
		m_pVideoWindow->StopVideo();
	}

}
void CPanelTV::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

    RETAILMSG(DEBUG_TV,(_T("++DTV: OnEnterPanel\r\n")));

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
		m_pCommander->SendCmd(_ARM_VIDEO_SW_KEY,SRC_VIDEO_TV);
// 		m_pCommander->SendCmd(_ARM_VIDEO_SW_KEY,SRC_VIDEO_CAMERA);
	}

	//switch audio:
	if(/*!m_bSpeechOn && */!m_bPhoneCallActive && (GetAudioSRC() != SRC_AUDIO_TV))
	{
		SwitchAudioSRC(SRC_AUDIO_TV,TRUE);
	}
}
void CPanelTV::Render()
{
// 	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_078):NULL;
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_043):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;

	BOOL bHasSignal=FALSE;
    SIZE si;

    RETAILMSG(DEBUG_TV,(_T("++DTV: Render\r\n")));

	//start video:
	if(!m_bShowAlert)
	{
    	if(!m_bOnReverseMode && IsReady() && GetTickCount()-m_dwEnterTick>600)
    	{
    		if(m_pVideoWindow && !m_pVideoWindow->IsRunning())
    		{
    			RETAILMSG(DEBUG_TV,(_T("++DTV: StartVideo++ iDTVStandard[%d]\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iDTVStandard));

    			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_DTV);
    			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iDTVStandard);	// NTSC
    			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
    			//m_pVideoWindow->SetGestureFunc(&CPanelTV::OnVideoWndGesture,this);
				//m_pVideoWindow->SetClickFunc(&CPanelTV::OnVideoWndTouchDown, &CPanelTV::OnVideoWndTouchUp, &CPanelTV::OnVideoWndTouchMove, &CPanelTV::OnVideoWndTouchDBClick, this);

    			m_pVideoWindow->StartVideo();

    			m_dwVideoStartTick = GetTickCount();
    		}
    	}

    	if(m_pVideoWindow)
    	{
    		bHasSignal=m_pVideoWindow->HasVideoSignal();
    	}

		if(!m_bOnReverseMode)
		{
    		m_iconTV.SetAlpha(m_fCurAlpha);
    		m_iconTV.Render();

			m_tagDTVTitle.SetAlpha(m_fCurAlpha);
			m_tagDTVTitle.Render();
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
BOOL CPanelTV::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);


	m_bShowStatusBar = FALSE;
	//m_bEnableSmartBar = FALSE;
	m_iconAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_084));
	m_iconAlert.SetShowScale(1.44f);
	m_iconAlert.SetPos(0,90);

#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ISDB)
	m_iconTV.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_190));
	m_iconTV.SetShowScale(1.5f);
#elif (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	m_iconTV.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_189));
	m_iconTV.SetShowScale(1.5f);
#else
	m_iconTV.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_129));
	m_iconTV.SetShowScale(1.6f);
#endif

	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(GetResourceString(IDS_NO_SIGNAL));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-160);
	m_tagTitle.SetCaptionHeight(28);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);

	m_tagDTVTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDTVTitle.SetSize(&si1);
	m_tagDTVTitle.SetCaption(_T(" "));
	m_tagDTVTitle.LinkTextGL(pTextGL);
	m_tagDTVTitle.SetPos(0,-80);
	m_tagDTVTitle.SetCaptionHeight(20);
	m_tagDTVTitle.EnableDrawBack(FALSE);
	m_tagDTVTitle.SetCaptionPos(BTN_CAPTION_CENTER);
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
	m_btnHome.SetNotifyFunc(&CPanelTV::OnNotify,this);
	m_btnHome.SetSize(&si1);
	m_btnHome.SetPos(-330,-200);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);

	m_btnSetting.Initialize(IDC_REARVIEW_BTN_SETTING,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
		m_pSkinManager->GetTexture(TEXID_056),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSetting.SetNotifyFunc(&CPanelTV::OnNotify,this);
	m_btnSetting.SetSize(&si1);
	m_btnSetting.SetPos(330,-200);
	m_btnSetting.SetIcon(&m_iconSettingUp,&m_iconSettingDown);
*/
#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	if( m_pTouchCmd == NULL)
	{
		m_pTouchCmd = new CCommanderEx();
		m_pTouchCmd->Initialize(CommandProc,this,_T("COM5:"),CBR_115200,hWnd);
	}

	if( m_pTouchSender == NULL)
	{
		m_pTouchSender = new CmdSender_AndroidTouch();
		m_pTouchSender->LinkCommander(m_pTouchCmd);
	}
#endif
	return TRUE;
}

#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
void CALLBACK CPanelTV::CommandProc(LPBYTE pCmd,DWORD dwCmdLen,LPVOID lparam)
{
	CPanelTV *pGUI=(CPanelTV*)lparam;
}
#endif

void CALLBACK CPanelTV::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_TV,(_T("APP:MSG:CPanelTV OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelTV *panel=(CPanelTV*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelTV::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_TV_BTN_QUIT:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	}
}
void CPanelTV::OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lpContext)
{
	CPanelTV *panel=(CPanelTV*)lpContext;

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
				panel->SendPointCmd(&pt,TRUE);
				Sleep(10);
				panel->SendPointCmd(&pt,FALSE);
				PlaySoundEffect(0);
			}
			break;
		}
	}

}
#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
void CPanelTV::SendPointCmd(POINT *pt, BYTE bDown)
{
	if (m_pTouchSender)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:x = %d, y = %d, bDown = 0x%x\r\n"),TEXT(__FUNCTION__), pt->x, pt->y, bDown));
		BYTE touchData[5] = {0};
		touchData[0] = bDown;
		touchData[1] = LOBYTE(LOWORD(pt->x));
		touchData[2] = HIBYTE(LOWORD(pt->x));
		touchData[3] = LOBYTE(LOWORD(pt->y));
		touchData[4] = HIBYTE(LOWORD(pt->y));
		m_pTouchSender->QueueCmdBuff(0,touchData,sizeof(touchData));
	}
}
#else
void CPanelTV::SendPointCmd(POINT *pt, BOOL bDown)
{
	if(m_pCommander)
	{
		BYTE x=(BYTE)(pt->x*255/g_iScreenWidth);
		BYTE y=(BYTE)(pt->y*255/g_iScreenHeight);

		//if(x==0 && y==0)
		//{
		//	RETAILMSG(DEBUG_TV,(_T("APP:MSG:CPanelTV SwitchPage HOME\r\n"),x,y));
		//	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		//}
		if (bDown)
		{
			//RETAILMSG(1,(_T("MSG:[%s]: Down x:%d,y:%d\r\n"),TEXT(__FUNCTION__), x, y));
			m_pCommander->SendCmd(_ARM_TV_X_KEY,x);
			m_pCommander->SendCmd(_ARM_TV_Y_KEY,y);
		}
		else
		{
			//RETAILMSG(1,(_T("MSG:[%s]: Up x:%d,y:%d\r\n"),TEXT(__FUNCTION__), x, y));
			m_pCommander->SendCmd(_ARM_TV_X_KEY_UP,x);
			m_pCommander->SendCmd(_ARM_TV_Y_KEY_UP,y);
		}
	}
}
#endif
void CPanelTV::OnReverseModeChange(BOOL bReverse)
{
	RETAILMSG(DEBUG_TV,(_T("MSG:[CPanelTV::OnReverseModeChange]: %d\r\n"),bReverse));

	CPanelGL::OnReverseModeChange(bReverse);

	//m_dwReverseTick = GetTickCount();

	if(bReverse && m_pVideoWindow && m_pVideoWindow->IsRunning())
	{
		RETAILMSG(DEBUG_TV,(_T("MSG:[CPanelTV::OnReverseModeChange]: StopVideo\r\n")));
		m_pVideoWindow->SetGestureFunc(NULL,NULL);
		m_pVideoWindow->SetClickFunc(NULL, NULL, NULL, NULL, NULL);
		m_pVideoWindow->StopVideo();
	}
	else if(!bReverse && !m_bShowAlert && m_pVideoWindow && (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER)))
	{
		if(m_pVideoWindow->IsRunning())
		{
			RETAILMSG(DEBUG_TV,(_T("MSG:[CPanelTV::OnReverseModeChange]: Stop and Restart video again\r\n")));
    		m_pVideoWindow->SetGestureFunc(NULL,NULL);
			m_pVideoWindow->SetClickFunc(NULL, NULL, NULL, NULL, NULL);
    		m_pVideoWindow->StopVideo();

			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_DTV);
			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iDTVStandard);	// NTSC
			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
			//m_pVideoWindow->SetGestureFunc(&CPanelTV::OnVideoWndGesture,this);
			//m_pVideoWindow->SetClickFunc(&CPanelTV::OnVideoWndTouchDown, &CPanelTV::OnVideoWndTouchUp, &CPanelTV::OnVideoWndTouchMove, &CPanelTV::OnVideoWndTouchDBClick, this);

			m_pVideoWindow->StartVideo();

	        m_dwVideoStartTick = GetTickCount();
		}
		else
		{
			RETAILMSG(DEBUG_TV,(_T("MSG:[CPanelTV::OnReverseModeChange]: Restart video again\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iDTVStandard));

			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_DTV);
			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iDTVStandard);	// NTSC
			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
			//m_pVideoWindow->SetGestureFunc(&CPanelTV::OnVideoWndGesture,this);
			//m_pVideoWindow->SetClickFunc(&CPanelTV::OnVideoWndTouchDown, &CPanelTV::OnVideoWndTouchUp, &CPanelTV::OnVideoWndTouchMove, &CPanelTV::OnVideoWndTouchDBClick, this);

			m_pVideoWindow->StartVideo();

	        m_dwVideoStartTick = GetTickCount();
		}
	}
	else
	{
		RETAILMSG(DEBUG_TV,(_T("MSG:[CPanelTV::OnReverseModeChange]: bReverse[%d] IsRunning[%d] idCurUI[0x%x]\r\n"),bReverse,m_pVideoWindow->IsRunning(),m_pSkinManager->GetConfig()->GetCurParam()->idCurUI));
	}
}

void CPanelTV::OnSpeechStart()
{
	m_bSpeechOn = TRUE;
}
void CPanelTV::OnSpeechTerminate()
{
	m_bSpeechOn = FALSE;

}

void CPanelTV::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;
    RETAILMSG(1, (TEXT("MSG:[CPanelTV::OnPhoneCallState]: m_bPhoneCallActive:%d\r\n"),m_bPhoneCallActive));

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_TV)
	{
		if(wParam)//active
		{

		}
		else
		{
			//if(!m_bSpeechOn)
			{
				SwitchAudioSRC(SRC_AUDIO_TV,TRUE);
			}
		}
	}
}

#if CVTE_EN_KEY_PROXY
BOOL CPanelTV::OnKey(UINT idKey,UINT idStatus)
{
	switch (idKey)
	{
	case KEY_PREVIOUS:
		if ( idStatus == EV_KEY_UP )
		{
			if(!m_bSpeechOn && !m_bPhoneCallActive)
			{
				RETAILMSG(1, (TEXT("MSG:[CPanelTV::OnKey]: Prev\r\n")));
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
				RETAILMSG(1, (TEXT("MSG:[CPanelTV::OnKey]: Next\r\n")));
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
			if(/*!m_bSpeechOn && */!m_bPhoneCallActive && GetAudioSRC() != SRC_AUDIO_TV)
			{
				SwitchAudioSRC(SRC_AUDIO_TV,TRUE);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}
#else
BOOL CPanelTV::OnKey(UINT idKey)
{
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
                RETAILMSG(1, (TEXT("MSG:[CPanelTV::OnKey]: Prev\r\n")));
	            if(m_pCommander)
	            {
		            m_pCommander->SendCmd(_ARM_TV_PREVNEXT_KEY,1);
                }
            }
		break;
	case KEY_MEDIA_NEXT:
            if(!m_bSpeechOn && !m_bPhoneCallActive)
            {
                RETAILMSG(1, (TEXT("MSG:[CPanelTV::OnKey]: Next\r\n")));
	            if(m_pCommander)
	            {
		            m_pCommander->SendCmd(_ARM_TV_PREVNEXT_KEY,2);
                }
            }
		break;
	case KEY_MEDIA_PLAY:
		{
			if(/*!m_bSpeechOn && */!m_bPhoneCallActive && GetAudioSRC() != SRC_AUDIO_TV)
			{
			    SwitchAudioSRC(SRC_AUDIO_TV,TRUE);
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


void CPanelTV::OnBrakeStatusChanged(BOOL bOn)
{
	CPanelGL::OnBrakeStatusChanged(bOn);
#if (CVTE_DEF_TV_WIDGET_TYPE != CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	ShowAlert(!bOn);

	RETAILMSG(1,(_T("MSG:[CPanelTV::OnBrakeStatusChanged] m_bShowAlert[%d]\r\n"),m_bShowAlert));

	if(m_bShowAlert)
	{
        if(m_pVideoWindow && m_pVideoWindow->IsRunning())
        {
    		m_pVideoWindow->SetGestureFunc(NULL,NULL);
			m_pVideoWindow->SetClickFunc(NULL, NULL, NULL, NULL, NULL);
    		m_pVideoWindow->StopVideo();
        }
    }
#endif
}

#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
void CPanelTV::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	static BOOL bPointTouchDown = FALSE;
	static BOOL bPoint2TouchDown = FALSE;
	int nPointStatus = pCyMultiPoint->sPointStatus.pointstatus;
	int nPointTouchArea = pCyMultiPoint->sPointStatus.toucharea;
	//RETAILMSG(1, (TEXT("MSG:[%s]:nPointStatus[%X] nPointTouchArea[%X]\r\n"),TEXT(__FUNCTION__),nPointStatus,nPointTouchArea));

	if((nPointTouchArea != TOUCH_AREA_GFX_WINDOW && (nPointStatus != TOUCH_STATUS_UP  && nPointStatus != GEST_SINGLE_TOUCH_END && nPointStatus != GEST_SINGLE_TOUCH_DOUBLE_CLICK)))
	{
		//RETAILMSG(1, (TEXT("ERR:[%s]:nPointTouchArea[%d]\r\n"),TEXT(__FUNCTION__),nPointTouchArea));
		return;
	}

	if(nPointStatus == TOUCH_STATUS_UP || nPointStatus == GEST_SINGLE_TOUCH_END || nPointStatus == GEST_SINGLE_TOUCH_DOUBLE_CLICK)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:Up\r\n"),TEXT(__FUNCTION__)));
		//UP
		bPointTouchDown = FALSE;
		
		SendPointCmd(&m_ptPoint1, CVTE_ANDROID_TOUCH_FIRST_UP);

		if (bPoint2TouchDown)
		{
			SendPointCmd(&m_ptPoint2, CVTE_ANDROID_TOUCH_SECOND_UP);
		}
		bPoint2TouchDown = FALSE;
		return;
	}
	else if(!bPointTouchDown && (nPointStatus == TOUCH_STATUS_DOWN_START || nPointStatus == TOUCH_STATUS_UNKNOW))
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:Down\r\n"),TEXT(__FUNCTION__)));
		//DOWN
		bPointTouchDown = TRUE;
		bPoint2TouchDown = FALSE;
		m_ptPoint1.x = pCyMultiPoint->sPointCoordinate[0].PointX;
		m_ptPoint1.y = pCyMultiPoint->sPointCoordinate[0].PointY;
		
		SendPointCmd(&m_ptPoint1, CVTE_ANDROID_TOUCH_FIRST_DOWN);
	}
	else
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:Move\r\n"),TEXT(__FUNCTION__)));
		//MOVE
		if (pCyMultiPoint->sPointStatus.pointnum == 1)
		{
			m_ptPoint1.x = pCyMultiPoint->sPointCoordinate[0].PointX;
			m_ptPoint1.y = pCyMultiPoint->sPointCoordinate[0].PointY;
			
			SendPointCmd(&m_ptPoint1, CVTE_ANDROID_TOUCH_FIRST_DOWN);
			if (bPoint2TouchDown)
			{
				bPoint2TouchDown = FALSE;
				SendPointCmd(&m_ptPoint2, CVTE_ANDROID_TOUCH_SECOND_UP);
			}
		}
		else
		{
			bPoint2TouchDown = TRUE;
			m_ptPoint1.x = pCyMultiPoint->sPointCoordinate[0].PointX;
			m_ptPoint1.y = pCyMultiPoint->sPointCoordinate[0].PointY;
			m_ptPoint2.x = pCyMultiPoint->sPointCoordinate[1].PointX;
			m_ptPoint2.y = pCyMultiPoint->sPointCoordinate[1].PointY;
			
			SendPointCmd(&m_ptPoint1, CVTE_ANDROID_TOUCH_FIRST_DOWN);
			SendPointCmd(&m_ptPoint2, CVTE_ANDROID_TOUCH_SECOND_DOWN);
		}
	}
}
#else
void CPanelTV::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	static BOOL bPointTouchDown = FALSE;
	int nPointStatus = pCyMultiPoint->sPointStatus.pointstatus;
	int nPointTouchArea = pCyMultiPoint->sPointStatus.toucharea;
	//RETAILMSG(1, (TEXT("MSG:[%s]:nPointStatus[%X] nPointTouchArea[%X]\r\n"),TEXT(__FUNCTION__),nPointStatus,nPointTouchArea));

	if((nPointTouchArea != TOUCH_AREA_GFX_WINDOW && (nPointStatus != TOUCH_STATUS_UP  && nPointStatus != GEST_SINGLE_TOUCH_END && nPointStatus != GEST_SINGLE_TOUCH_DOUBLE_CLICK)))
	{
		//RETAILMSG(1, (TEXT("ERR:[%s]:nPointTouchArea[%d]\r\n"),TEXT(__FUNCTION__),nPointTouchArea));
		return;
	}

	if(nPointStatus == TOUCH_STATUS_UP || nPointStatus == GEST_SINGLE_TOUCH_END || nPointStatus == GEST_SINGLE_TOUCH_DOUBLE_CLICK)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:Up\r\n"),TEXT(__FUNCTION__)));
		//UP
		bPointTouchDown = FALSE;
		return;
	}
	else if(!bPointTouchDown && (nPointStatus == TOUCH_STATUS_DOWN_START || nPointStatus == TOUCH_STATUS_UNKNOW))
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:Down\r\n"),TEXT(__FUNCTION__)));
		//DOWN
		bPointTouchDown = TRUE;
		m_ptPoint1.x = pCyMultiPoint->sPointCoordinate[0].PointX;
		m_ptPoint1.y = pCyMultiPoint->sPointCoordinate[0].PointY;

		SendPointCmd(&m_ptPoint1, TRUE);
	}
	else
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:Move\r\n"),TEXT(__FUNCTION__)));
		//MOVE
	}
}
#endif

void CPanelTV::OnVideoWndTouchDown(POINT *pt,LPVOID lparam)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: Down:[%d,%d]\r\n"), TEXT(__FUNCTION__), pt->x, pt->y));
#if (CVTE_DEF_TV_WIDGET_TYPE != CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	CPanelTV *panel=(CPanelTV*)lparam;
	panel->SendPointCmd(pt, TRUE);
#endif
}

void CPanelTV::OnVideoWndTouchUp(POINT *pt,LPVOID lparam)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: Up:[%d,%d]\r\n"), TEXT(__FUNCTION__), pt->x, pt->y));
#if (CVTE_DEF_TV_WIDGET_TYPE != CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	CPanelTV *panel=(CPanelTV*)lparam;
	panel->SendPointCmd(pt, FALSE);
#endif
}

void CPanelTV::OnVideoWndTouchMove(POINT *pt,LPVOID lparam)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: Move:[%d,%d]\r\n"), TEXT(__FUNCTION__), pt->x, pt->y));
#if (CVTE_DEF_TV_WIDGET_TYPE != CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	CPanelTV *panel=(CPanelTV*)lparam;
	panel->SendPointCmd(pt, TRUE);
#endif
}

void CPanelTV::OnVideoWndTouchDBClick(POINT *pt,LPVOID lparam)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: DB Click:[%d,%d]\r\n"), TEXT(__FUNCTION__), pt->x, pt->y));
#if (CVTE_DEF_TV_WIDGET_TYPE != CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	CPanelTV *panel=(CPanelTV*)lparam;
	panel->SendPointCmd(pt, TRUE);
#endif
}

#endif

