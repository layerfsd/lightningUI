#include "PanelCarPlay.h"
#include "../../resource.h"
#include "../../CarPlay/CarplayAdapter.h"

#if CVTE_EN_CARPLAY
extern const wchar_t* g_strDefaultBackground;

CPanelCarPlay::CPanelCarPlay(void)
{
	m_bSpeechOn = FALSE;
	m_bCarPlayRunning = FALSE;
	m_dwEnterTick=0;
	m_bHasValidBackImage = FALSE;
	m_bConnected = FALSE;
    m_bControlConnected = FALSE;
	m_bCarPlaySpeechOn = FALSE;
	m_bCarPlayPhoneCallOn = FALSE;
	m_bCarPlayTurnbyTurnOn = FALSE;

	m_bCarPlayTakeMainAudio = 0;
	m_idSRCBeforeCarplayTakeAudio = 0;
	m_bCarPlayAltAudioOn = 0;
	m_bCarPlayMainAudioOn = 0;
	m_bCarPlayAudioOnDucking = 0;
	m_bNeedReturnHome = FALSE;

	m_idRequestURL = -1;
	m_bVideoConnected = FALSE;
	m_bManualEnter = FALSE;

	m_bPlaying = 0;
	m_bPlayingBeforeIdle = 0;
	m_dwDisableReloadScreenTick = 0;

	m_bPhoneCallActive = FALSE;
	m_bNeedTakeScreen = TRUE;
	m_bNeedUnTakeScreen = TRUE;

	m_bRdsTAon = FALSE;
	m_dwAutoSwitchCarplayRef=0;
	m_bRefreshScreen=TRUE;

	m_idUIBeforePhoneCallOn = 0;
	m_idUIBeforeSiri = 0;
	m_idUIBeforeReverse = 0;

	m_bOnNightMode = FALSE;
	memset(&m_NativeRequestUIPara,0,sizeof(SWITCH_PAGE_PARAM));

	m_bIdleTakeResource = FALSE;
	m_bRequestingCarPlayUI = FALSE;
	m_dwVideoConnectTick = 0;

#if CVTE_EN_KEY_PROXY
	m_buttonkey.ucReport = 0;
	m_phonekey.uiReport = 0;
#endif	
}

CPanelCarPlay::~CPanelCarPlay(void)
{
}
BOOL CPanelCarPlay::IsReady()
{
	return CPanelGL::IsReady() || m_bVideoConnected;

}
void CPanelCarPlay::OnSRCChanged(UINT idSRC)
{
	RETAILMSG(1,(_T("CPanelCarPlay::OnSRCChanged 0x%x, TA:%d\r\n"),idSRC,m_bRdsTAon));

	if(GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_CARPLAY)
	{
		if(!m_bRdsTAon)
		{
			if(m_idSRCBeforeCarplayTakeAudio != 0)
			{
				m_idSRCBeforeCarplayTakeAudio = idSRC;
			}
		}	
		//change [main audio] to [Accessory]:
		if (m_bControlConnected && !(m_bRdsTAon&&GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_RADIO))
		{
			CarplayAPITakeMainAudio(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_UserInitiated,kAirPlayConstraint_Anytime,NULL);
		}
	}
}

void CPanelCarPlay::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	RETAILMSG(1,(_T("CPanelCarPlay::OnQuitPanel iParam:%d, idAni %d\r\n"),iParam,idAni));

#if 1
	//get thumbnail:
	if(m_bVideoConnected)
	{
		if((GetTickCount()-m_dwDisableReloadScreenTick>2000 || !m_bHasValidBackImage) && !m_bCarPlayPhoneCallOn)
		{
			CTexture *pTex=m_bk.GetTexture();
			if(pTex)
			{
				//RETAILMSG(1,(_T("==take snapshot start==\r\n")));
				m_bHasValidBackImage = TRUE;
				pTex->ReloadFormScreen();
				//RETAILMSG(1,(_T("==take snapshot end==\r\n")));
			}
		}
	}
	else
	{
		//m_bHasValidBackImage = FALSE;
	}
#endif

	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	//
	StopCarPlay();

	if(m_pSmartBarVol)
	{
		m_pSmartBarVol->EnableShow(TRUE);
	}
	if(m_pSmartBarMedia)
	{
		m_pSmartBarMedia->EnableShow(TRUE);
	}

	if(m_ID == MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER))
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
		{
			EnableNaviSpeech(FALSE);
		}
	}

}
void CPanelCarPlay::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(DEBUG_CARPLAY,(_T("CPanelCarPlay::OnEnterPanel iParam:%d, idAni %d\r\n"),iParam,idAni));

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
// 	SwitchAudioSRC(SRC_AUDIO_ARM,GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) != UI_CLASS_CARPLAY);

	//reset volume:
	if(m_bSpeechOn)
	{
		DWORD dwGPS=m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS;
		DWORD dwMedia=m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia;

		DWORD dwVol=0;

		//pull down volume

	}
	else
	{
		//pull up volume
	}


	//can bus:
//	::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_OTHER,0x30);
// 	::PostMessage(m_hMainWnd,WM_NOTIFY_MEDIA_PLAYBACK_INFO,0x0,(0x0<<16)+0x30);

	m_dwEnterTick = GetTickCount();

	m_idRequestURL = LOWORD(iParam);
	m_bManualEnter = (HIWORD(iParam) == 0);

	RETAILMSG(1,(_T("CPanelCarPlay::OnEnterPanel m_bManualEnter: %d \r\n"),m_bManualEnter));

	m_bNeedTakeScreen = TRUE;

	m_bRefreshScreen=TRUE;

	if(m_pSmartBarVol)
	{
		m_pSmartBarVol->Show(FALSE);
		m_pSmartBarVol->EnableShow(m_bVideoConnected);
	}
	if(m_pSmartBarMedia)
	{
		m_pSmartBarMedia->Show(FALSE);
		m_pSmartBarMedia->EnableShow(FALSE);
	}

	//m_pSkinManager->GetGUI()->EnableSwap(!m_bVideoConnected);

	if(m_ID == MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER))
	{
		if(!m_bPhoneCallActive && m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
		{
			RETAILMSG(1, (TEXT("MSG:[%s]: =>EnableNaviSpeech TRUE\r\n"),TEXT(__FUNCTION__)));
			EnableNaviSpeech(TRUE);
		}
	}
}

void CPanelCarPlay::Render()
{
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;

// 	COLORGL cr_b={0.0,0.0,0.0,m_fCurAlpha};

// 	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

// 	DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr_b);

	if(m_bHasValidBackImage && m_bManualEnter)
	{
		m_bk.SetAlpha(m_fCurAlpha);
		m_bk.Render();
	}
	else
	{
 		COLORGL cr_b={0.1,0.1,0.1,1.0};
 		DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr_b);

// 		m_iconLink.SetAlpha(m_fCurAlpha);
// 		m_iconLink.Render();

// 		m_tagTitle.SetAlpha(m_fCurAlpha);
// 		m_tagTitle.Render();

		//if(!m_bVideoConnected)
		if(!m_bControlConnected)
		{
			m_iconLink.SetAlpha(m_fCurAlpha);
			m_iconLink.Render();

			VIEW_STATE vs=m_iconLoading.GetCurViewState();
			vs.fRotateZ-=5;
			if(vs.fRotateZ<-360)
				vs.fRotateZ=0;
			m_iconLoading.SetCurViewState(&vs);
			m_iconLoading.SetAlpha(m_fCurAlpha);
			m_iconLoading.Render();

		}
	}

	if(!m_bControlConnected || !m_bCarPlayRunning)
	{
		SetFrameTime(50);
	}
	else
	{
		SetFrameTime(500);
	}

	if(m_bVideoConnected /*&& !m_bRefreshScreen*/ && m_bPanelEntered)
	{
		if(m_pSkinManager->GetGUI()->isActivated())
		{
			m_pSkinManager->GetGUI()->EnableSwap(FALSE);
		}
	}

	if( (/*IsReady()||*/GetTickCount()-m_dwEnterTick>400) && m_bPanelEntered && !m_bCarPlayRunning)
	{
		StartCarPlay();
	}

#if 0
	if(IsReady() /*&& m_bVideoConnected*/ && !m_bCarPlayRunning /*&& GetTickCount()-m_dwEnterTick>10*/)
	{
		if(!m_bRefreshScreen)
		{
			//if(GetTickCount()-m_dwEnterTick>500)
			{
				StartCarPlay();
			}
		}
		else
		{
			m_bRefreshScreen=FALSE;
		}
	}
#endif


	EndRender();
}
void CPanelCarPlay::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
BOOL CPanelCarPlay:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bShowStatusBar = FALSE;

	m_iconLink.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_152));
	m_iconLink.SetShowScale(1.4f);
	m_iconLink.SetPos(0,/*80*/0);

	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.8f);
	m_iconLoading.SetPos(0,0);


	//background:
	int cx=g_iClientWidth;
	int cy=g_iClientHeight;//512;//
	if(m_pSkinManager->GetTexture(TEXID_USER3) == NULL)
	{
		if(!m_pSkinManager->AddTextureFromFile(TEXID_USER3,g_strDefaultBackground,cx,cy))
		{
			RETAILMSG(DEBUG_CARPLAY, (TEXT("APP: background texture FAILED!\r\n")));
		}
	}
	m_bk.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_USER3),NULL,TRUE);
	m_bk.SetScale((GLfloat)g_iScreenWidth/(GLfloat)cx,(GLfloat)g_iScreenHeight/(GLfloat)cy);
	VIEW_STATE vs=m_bk.GetCurViewState();
	vs.fRotateX=-180;
	m_bk.SetIniViewState(&vs);


	//
	COLORGL cr={1,1,1,1.0};
	COLORGL cr_down={0.465,0.824,0.965,1.0};
	SIZE si0={800,120};


	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(GetResourceString(IDS_CONNECTING));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-160);
	m_tagTitle.SetCaptionHeight(28);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);

	return TRUE;
}


void CPanelCarPlay::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	if(!lpDeviceName)
		return;

	RETAILMSG(DEBUG_CARPLAY, (TEXT("CPanelCarPlay::OnDeviceArrivedReady :%s\r\n"), lpDeviceName));

}
void CPanelCarPlay::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_CARPLAY, (TEXT("CPanelCarPlay::OnDeviceRemovedReady :%s\r\n"), lpDeviceName));

}
BOOL CPanelCarPlay::IsEnableQuit()
{
	if(GetTickCount()-m_dwEnterTick < 500)
	{
		m_bEnableQuit = FALSE;
	}
	else
	{
		if(!m_bControlConnected)
		{
			m_bEnableQuit = TRUE;
		}
		else if(m_bRequestingCarPlayUI)
		{
			if(m_bVideoConnected)
			{
				m_bEnableQuit = GetTickCount()-m_dwVideoConnectTick > 400;
			}
			else
			{
				//[bug]PanelCarPlay can't quit if screen video can't connect
				m_bEnableQuit = (GetTickCount()-m_dwEnterTick > 500);
			}
		}
		else
		{
			m_bEnableQuit = TRUE;
		}

	}

	return m_bEnableQuit;
}
void CPanelCarPlay::OnBack()
{
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
}
void CPanelCarPlay::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);

}


void CPanelCarPlay::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

}
void CPanelCarPlay::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);
}
void CPanelCarPlay::OnRequestSiri()
{
    if (m_bControlConnected)
    {
	    CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonDown);
		Sleep(10);
		CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonUp);
   }
}
void CPanelCarPlay::OnRequestUI(const char* inURL)
{
    if (m_bControlConnected)
    {
        CarplayAPIRequestUI(inURL);
    }
}
void CPanelCarPlay::PlayNext()
{
	DWORD err = 0;
    if (m_bControlConnected && m_bCarPlayTakeMainAudio)
    {
    	ButtonHIDReport_t key;
    	key.ucReport=0;
    	key.unReprot.ScanNextTrack=1;
    	err = CarplayAPIButtonKey(key);
		if (err == 0)
		{
			Sleep(10);
			key.ucReport=0;
			CarplayAPIButtonKey(key);
		}
    }
}
void CPanelCarPlay::PlayPrevious()
{
	DWORD err = 0;
    if (m_bControlConnected && m_bCarPlayTakeMainAudio)
    {
    	ButtonHIDReport_t key;
    	key.ucReport=0;
    	key.unReprot.ScanPreviousTrack=1;
        err = CarplayAPIButtonKey(key);
		if (err == 0)
		{
			Sleep(10);
			key.ucReport=0;
			CarplayAPIButtonKey(key);
		}
    }
}
void CPanelCarPlay::Play()
{
    DWORD err = 0;
    if (m_bControlConnected && m_bCarPlayTakeMainAudio)
    {
    	ButtonHIDReport_t key;
    	key.ucReport=0;
    	key.unReprot.Play=1;
        err = CarplayAPIButtonKey(key);
		if (err == 0)
		{
			Sleep(10);
			key.ucReport=0;
			err = CarplayAPIButtonKey(key);
		}

		if((err == 0) && m_pSmartBarMedia && m_pSmartBarMedia->GetType()==SB_MEDIATYPE_CARPLAY)
    	{
    		m_pSmartBarMedia->SetPlayingFlag(TRUE);
    	}
    }
}
void CPanelCarPlay::Pause()
{
    if (m_bControlConnected && m_bCarPlayTakeMainAudio)
    {
    	DWORD err = 0;
    	ButtonHIDReport_t key;
    	key.ucReport=0;
    	key.unReprot.Pause=1;
    	err = CarplayAPIButtonKey(key);
		if (err == 0)
		{
			Sleep(10);
			key.ucReport=0;
			err = CarplayAPIButtonKey(key);
		}

		if((err == 0) && m_pSmartBarMedia && m_pSmartBarMedia->GetType()==SB_MEDIATYPE_CARPLAY)
    	{
    		m_pSmartBarMedia->SetPlayingFlag(FALSE);
    	}
    }
}
void CPanelCarPlay::PlayPause()
{
    if (m_bControlConnected && m_bCarPlayTakeMainAudio)
    {
    	ButtonHIDReport_t key;
    	key.ucReport=0;
    	key.unReprot.PlayPause=1;
    	CarplayAPIButtonKey(key);
		Sleep(10);
		key.ucReport=0;
		CarplayAPIButtonKey(key);
    }
}

void CPanelCarPlay::AnswerOrHang()
{
    if (m_bControlConnected)
    {
        PhoneKeyHIDReport_t key;
        key.uiReport=0;
    	key.unReprot.Flash=1;
        CarplayAPIPhoneKey(key);
		Sleep(10);
		key.uiReport=0;
		CarplayAPIPhoneKey(key);
    }
}

void CPanelCarPlay::Answer()
{
    if (m_bControlConnected)
    {
        PhoneKeyHIDReport_t key;
        key.uiReport=0;
    	key.unReprot.HookSwitch=1;
        CarplayAPIPhoneKey(key);
		Sleep(10);
		key.uiReport=0;
		CarplayAPIPhoneKey(key);
    }
}

void CPanelCarPlay::Hang()
{
    if (m_bControlConnected)
    {
        PhoneKeyHIDReport_t key;
        key.uiReport=0;
    	key.unReprot.Drop=1;
        CarplayAPIPhoneKey(key);
		Sleep(10);
		key.uiReport=0;
		CarplayAPIPhoneKey(key);
    }
}
void CPanelCarPlay::RequestNativeUI(SWITCH_PAGE_PARAM *para)
{
	if(para)
	{
		RETAILMSG(1,(_T("###CPanelCarPlay::RequestNativeUI ......\r\n")));

		if(m_NativeRequestUIPara.idClass != para->idClass
			||m_NativeRequestUIPara.idPage != para->idPage
			||m_NativeRequestUIPara.pageParam != para->pageParam)
		{
			RETAILMSG(1,(_T("###CPanelCarPlay::RequestNativeUI: idClass=0x%x, idPage=0x%x, pageParam=0x%x ......\r\n"),para->idClass,para->idPage,para->pageParam));
			memcpy(&m_NativeRequestUIPara,para,sizeof(SWITCH_PAGE_PARAM));
			m_bNeedTakeScreen = FALSE;
			CarplayAPIBorrowScreen(kAirPlayTransferPriority_NiceToHave,kAirPlayConstraint_Anytime,NULL);
		}
	}
}
void CPanelCarPlay::ReleaseNativeUI()
{
	RETAILMSG(1,(_T("###CPanelCarPlay::ReleaseNativeUI ......\r\n")));
	CarplayAPIUnborrowScreen(NULL);
	m_bNeedUnTakeScreen = FALSE;
}

#if CVTE_EN_KEY_PROXY
BOOL CPanelCarPlay::OnKey(UINT idKey,UINT idStatus)
{
	DWORD err = 0;
	RETAILMSG(DEBUG_CARPLAY,(_T("........CPanelCarPlay::OnKey: key=0x%x, Status=0x%x ......\r\n"),idKey,idStatus));
	switch (idKey)
	{
	case KEY_HOOK_SWITCH:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_phonekey.unReprot.HookSwitch = 1;
			CarplayAPIPhoneKey(m_phonekey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_phonekey.unReprot.HookSwitch = 0;
			CarplayAPIPhoneKey(m_phonekey);
		}
		break;
	case KEY_DROP:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_phonekey.unReprot.Drop = 1;
			CarplayAPIPhoneKey(m_phonekey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_phonekey.unReprot.Drop = 0;
			CarplayAPIPhoneKey(m_phonekey);
		}
		break;
	case KEY_FLASH:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_phonekey.unReprot.Flash = 1;
			CarplayAPIPhoneKey(m_phonekey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_phonekey.unReprot.Flash = 0;
			CarplayAPIPhoneKey(m_phonekey);
		}
		break;
	case KEY_NEXT:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_buttonkey.unReprot.ScanNextTrack = 1;
			CarplayAPIButtonKey(m_buttonkey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_buttonkey.unReprot.ScanNextTrack = 0;
			CarplayAPIButtonKey(m_buttonkey);
		}
		break;
	case KEY_PREVIOUS:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_buttonkey.unReprot.ScanPreviousTrack = 1;
			CarplayAPIButtonKey(m_buttonkey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_buttonkey.unReprot.ScanPreviousTrack = 0;
			CarplayAPIButtonKey(m_buttonkey);
		}
		break;
	case KEY_PLAY:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_buttonkey.unReprot.Play = 1;
			CarplayAPIButtonKey(m_buttonkey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_buttonkey.unReprot.Play = 0;
			CarplayAPIButtonKey(m_buttonkey);
		}
		break;
	case KEY_STOP:
	case KEY_PAUSE:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_buttonkey.unReprot.Pause = 1;
			CarplayAPIButtonKey(m_buttonkey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_buttonkey.unReprot.Pause = 0;
			CarplayAPIButtonKey(m_buttonkey);
		}
		break;
	case KEY_PLAYPAUSE:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_buttonkey.unReprot.PlayPause = 1;
			CarplayAPIButtonKey(m_buttonkey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_buttonkey.unReprot.PlayPause = 0;
			CarplayAPIButtonKey(m_buttonkey);
		}
		break;
	case KEY_MUTE:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_phonekey.unReprot.PhoneMute = 1;
			CarplayAPIPhoneKey(m_phonekey);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_phonekey.unReprot.PhoneMute = 0;
			CarplayAPIPhoneKey(m_phonekey);
		}
		break;
	case KEY_SPEECH:
		if ( idStatus == EV_KEY_DOWN )
		{
			CarplayAPIRequestSiri(kAirPlaySiriAction_Prewarm);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonUp);
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonDown);
		}
		break;
	}

	return TRUE;
}
#else
BOOL CPanelCarPlay::OnKey(UINT idKey)
{
	RETAILMSG(DEBUG_CARPLAY,(_T("........CPanelCarPlay::OnKey: 0x%x ......\r\n"),idKey));
	switch (idKey)
	{
	case KEY_PHONE_ANSWER:
        //AnswerOrHang();
		Answer();
		break;
	case KEY_PHONE_HANG:
		Hang();
		break;
	case KEY_PHONE_ACH:
		{
		}
		break;
	case KEY_MEDIA_NEXT:
		PlayNext();
		break;
	case KEY_MEDIA_PREVIOUS:
		PlayPrevious();
		break;
	case KEY_MEDIA_PLAY:
		SwitchAudioSRC(SRC_AUDIO_ARM);
		Play();
		break;
	case KEY_MEDIA_PAUSE:
		Pause();
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		PlayPause();
		break;
	}

	return TRUE;

}
#endif

void CPanelCarPlay::OnSpeechStart()
{
//     DWORD err;
// 	RETAILMSG(DEBUG_CARPLAY,(_T("............CPanelCarPlay OnSpeechStart......................\r\n")));

	m_bSpeechOn = TRUE;

// 	if(!m_bCarPlaySpeechOn)
// 	{
//         if (m_bControlConnected)
//         {
//             err = CarplayAPIDuckAudio(0.5, 0.2);
//             if (err == kNoErr)
//             {
//                 m_bCarPlayAudioOnDucking = TRUE;
//             }
//         }
// 	}
}
void CPanelCarPlay::OnSpeechTerminate()
{
//     DWORD err;
	RETAILMSG(DEBUG_CARPLAY,(_T("............CPanelCarPlay OnSpeechTerminate......................\r\n")));

	m_bSpeechOn = FALSE;

// 	if(m_bCarPlayAudioOnDucking)
// 	{
//         if (m_bControlConnected)
//         {
//             err = CarplayAPIUnduckAudio(0.5);
//             if (err == kNoErr)
//             {
//                 m_bCarPlayAudioOnDucking = FALSE;
//             }
//         }
//         else
//         {
//             m_bCarPlayAudioOnDucking = FALSE;
//         }
// 	}

}
void CPanelCarPlay::OnPlaySoundEffect(BOOL bPlay)
{

}
void CPanelCarPlay::OnBrakeStatusChanged(BOOL bOn)
{
	CPanelGL::OnBrakeStatusChanged(bOn);

	RETAILMSG(DEBUG_CARPLAY, (TEXT("++++OnBrakeStatusChanged:: %d \r\n"),bOn));

#if CVTE_EN_CARPLAY_LIMIT_UI
	CCarplayAdapter::GetInstance()->LimitedUI(!bOn);
#endif

}
void CPanelCarPlay::OnSystemPowerStatePrepare(UINT idState)
{
	if(idState==SYS_POWER_ON && m_idSystemPowerState!=SYS_POWER_ON)
	{
		m_bNeedReturnHome = TRUE;
	}

	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(DEBUG_CARPLAY, (TEXT("CPanelCarPlay::OnSystemPowerStatePrepare :%d\r\n"), idState));

	switch (idState)
	{
	case SYS_POWER_OFF:
	case SYS_POWER_IDLE:
		{
			//StopCarPlay();

			/*
			m_bPlayingBeforeIdle = m_bCarPlayMainAudioOn;
			if(m_bPlayingBeforeIdle)
			{
				Pause();
			}
			*/
		}
		break;
	case SYS_POWER_ON:
		{

		}
		break;
	}
}
void CPanelCarPlay::OnSystemPowerStateDone(UINT idState)
{
	CPanelGL::OnSystemPowerStateDone(idState);

	RETAILMSG(1/*DEBUG_CARPLAY*/, (TEXT("MSG:[CPanelCarPlay::OnSystemPowerStateDone]: idState:%d\r\n"), idState));

	switch (idState)
	{
	case SYS_POWER_OFF:
		{
			if (m_bControlConnected && m_bIdleTakeResource)
			{
				RETAILMSG(1/*DEBUG_CARPLAY*/, (TEXT("MSG:[CPanelCarPlay::OnSystemPowerStateDone]:  SYS_POWER_OFF UnBorrow Screen&MainAudio\r\n")));
				m_bIdleTakeResource = FALSE;
				m_bNeedUnTakeScreen = FALSE;
				CarplayAPIUnborrowScreen(NULL);
				CarplayAPIUnborrowMainAudio(NULL);
			}
		}
		break;
	case SYS_POWER_IDLE:
		{
			//Do not operate any more during the idle state.
			if (m_bControlConnected /*&& !m_bIdleTakeResource*/)
			{
				//RETAILMSG(1/*DEBUG_CARPLAY*/, (TEXT("MSG:[CPanelCarPlay::OnSystemPowerStateDone]:  Borrow Screen&MainAudio\r\n")));
				m_bIdleTakeResource = TRUE;
				CarplayAPIBorrowScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,NULL);
				CarplayAPIBorrowMainAudio(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,NULL);
			}
		}
		break;
	case SYS_POWER_ON:
		{
			//Do not operate any more during the idle state.
			if (m_bControlConnected /*&& m_bIdleTakeResource*/)
			{
				//RETAILMSG(1/*DEBUG_CARPLAY*/, (TEXT("MSG:[CPanelCarPlay::OnSystemPowerStateDone]:  UnBorrow Screen&MainAudio\r\n")));
				m_bIdleTakeResource = FALSE;
				m_bNeedUnTakeScreen = FALSE;
				CarplayAPIUnborrowScreen(NULL);
				CarplayAPIUnborrowMainAudio(NULL);
			}
		}
		break;
	}
}

void CPanelCarPlay::StartCarPlay()
{
	if(m_bControlConnected && !m_bCarPlayRunning)
	{
		RETAILMSG(1, (TEXT("++++CPanelCarPlay:: StartCarPlay, requestui=%d \r\n"),m_bManualEnter));

		//m_pSkinManager->GetGUI()->EnableSwap(FALSE);
		m_bCarPlayRunning = TRUE;

  /*
	   const char * urllist[] =
       {
            NULL,
            "maps:"
            "mobilephone:"
            "music:"
            "nowplaying:"
            "tel:xxx-xxx"
        };

		DWORD idURL=-1;

		if(m_idRequestURL != 0)
		{
			idURL=m_idRequestURL;
		}
		else
		{
			if(m_bCarPlayPhoneCallOn)
				idURL=2;
// 				else if(m_bCarPlayTurnbyTurnOn)
// 					idURL=1;
// 				else if(m_bCarPlayTakeMainAudio)
// 					idURL=4;
			else if(m_bNeedReturnHome && m_bManualEnter)
			{
				m_bNeedReturnHome = FALSE;
				idURL=0;
			}
		}
		*/

		if(m_bManualEnter)
		{
			m_bRequestingCarPlayUI = TRUE;
			if(m_bNeedUnTakeScreen)
				CarplayAPIUntakeScreen(NULL);
			CarplayAPIRequestUI("");
			m_bNeedUnTakeScreen = TRUE;
		}
        //CarplayAPIShowWindow(TRUE);

//         if((idURL >= 0) && (idURL <= 5))
//         {
//             if (m_bControlConnected)
//             {
//                 CarplayAPIRequestUI(urllist[idURL]);
//             }
//         }

		//auto play:
// 		if(idURL==-1 && !m_bCarPlayTakeMainAudio)
// 		{
// 			//Play();
// 		}
	}
}
void CPanelCarPlay::StopCarPlay()
{
	m_bCarPlayRunning = FALSE;
	m_bRequestingCarPlayUI = FALSE;

	if(m_bControlConnected)
	{
        CarplayAPIShowWindow(FALSE);
		const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();

		if(/*m_bNeedTakeScreen*/mode->screen==kAirPlayEntity_Controller /*&& !m_bRdsTAon*/ && m_idSystemPowerState == SYS_POWER_ON)
		{
			RETAILMSG(1, (TEXT("++++CPanelCarPlay:: StopCarPlay , need take screen\r\n")));
			CarplayAPITakeScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_UserInitiated,kAirPlayConstraint_Anytime,NULL);
		}
	}
	else
	{
		//m_bHasValidBackImage = FALSE;
	}
}
void CPanelCarPlay::OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1,(_T("CPanelCarPlay::OnNotifyRdsTAStatus: %d\r\n"),wParam));

	m_bRdsTAon = wParam;
	if (m_bControlConnected)
	{
		if(m_bRdsTAon)
		{
// 			CarplayAPIBorrowMainAudio(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,NULL);
// 			CarplayAPIBorrowScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,NULL);
			CarplayAPIBorrowMainAudio(kAirPlayTransferPriority_NiceToHave,kAirPlayConstraint_Anytime,NULL);
			CarplayAPIBorrowScreen(kAirPlayTransferPriority_NiceToHave,kAirPlayConstraint_Anytime,NULL);
		}
		else
		{
			CarplayAPIUnborrowMainAudio(NULL);
			CarplayAPIUnborrowScreen(NULL);
			m_bNeedUnTakeScreen = FALSE;
		}
	}
}
void CPanelCarPlay::OnReverseModeChange(BOOL bReverse)
{
	RETAILMSG(DEBUG_CARPLAY,(_T("MSG:[CPanelCarPlay::OnReverseModeChange]: %d\r\n"),bReverse));

	CPanelGL::OnReverseModeChange(bReverse);

	UINT idCurUI = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
	if(m_bControlConnected)
	{
		if(bReverse)
		{
// 			m_idUIBeforeReverse = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
			m_bNeedTakeScreen = FALSE;
			CarplayAPIBorrowScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,NULL);
			RETAILMSG(1/*DEBUG_CARPLAY*/,(_T("===CarplayAPIBorrowScreen===\r\n")));
		}
		else if(GET_PANEL_CLASS_ID(idCurUI) != UI_CLASS_WARNING)
		{
			RETAILMSG(1/*DEBUG_CARPLAY*/,(_T("===CarplayAPIUnborrowScreen===\r\n")));
			m_bNeedTakeScreen = TRUE;
			m_bNeedUnTakeScreen = FALSE;
			CarplayAPIUnborrowScreen(NULL);
// 			if(m_idUIBeforeReverse == MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER))
// 			{
// 				m_idUIBeforeReverse = 0;
// 				SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE);
// 			}
			if(m_bCarPlayPhoneCallOn)
			{
				m_idUIBeforePhoneCallOn = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
				SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE);
			}
			/*
			else if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI) == UI_CLASS_CARPLAY)
			{
				SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE);
			}
			*/
		}
	}
}
void CPanelCarPlay::OnNightModeChanged(BOOL bNightMode)
{
	m_bOnNightMode = bNightMode;

#if CVTE_EN_CARPLAY_NIGHTMODE
	CarplayAPISetNightMode(m_bOnNightMode);
#endif

}
void CPanelCarPlay::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
	{
	case CARPLAY_CONNECT_STATUS:
		{
            m_bConnected = (BOOL)lParam;
            if (m_bConnected)
            {
            }
            else
            {
                m_bEnableQuit = TRUE;
        		StopCarPlay();
                m_bControlConnected = FALSE;
        		m_bVideoConnected = FALSE;
        		m_bHasValidBackImage = FALSE;

        		//reset
        		if(m_bCarPlayPhoneCallOn)
        		{
        			m_bCarPlayPhoneCallOn = FALSE;
        			::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,FALSE,(1<<16)+HF_STATE_IDLE);
        		}
        		if(m_bCarPlaySpeechOn)
        		{
        			m_bCarPlaySpeechOn = FALSE;
        			::PostMessage(m_hMainWnd,WM_MSG_SR,0,0);
        		}
        		if(m_bCarPlayAltAudioOn)
        		{
        			m_bCarPlayAltAudioOn = FALSE;
        			//::PostMessage(m_hMainWnd,WM_MSG_SPEECH,0,0);
        		}
        		if(m_bCarPlayMainAudioOn)
        		{
        			m_bCarPlayMainAudioOn = FALSE;
        		}

        		if(m_bCarPlayTurnbyTurnOn)
        		{
        			m_bCarPlayTurnbyTurnOn = FALSE;
        		}

        		if(m_bCarPlayTakeMainAudio)
        		{
        			m_bCarPlayTakeMainAudio = FALSE;
        			//return to radio:
        			//SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_MAIN));
        			//::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PLAY,NULL);
        		}

				//switch to last page if current audio source is not carplay,
				//or switch to homepage:
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_CARPLAY)
				{
					if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_CARPLAY)
					{
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
					}
					else
					{
						//if previous panel has valid source, but doesn't match current source,
						//don't switch the previous panel, just switch back to home page.
						UINT idUIClass=GET_PANEL_CLASS_ID(m_idPreviousPanel);
						UINT idSRCClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);
						UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

						if( (idUIClass > UI_CLASS_HOME && idUIClass < UI_CLASS_MAX_VALID && idUIClass != UI_CLASS_IDLE) )
						{
							if( IsPanelHasValidAudioSrc(m_idPreviousPanel) ? (idUIClass==idSRCClass) : TRUE )
								SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
							else
								SwitchPage(GET_PANEL_CLASS_ID(idSRC),GET_PANEL_PAGE_ID(idSRC),PAGE_SWITCH_ANI_NONE);
						}
						else
						{
							SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
						}
					}
				}
            }
		}
		break;
    case CARPLAY_CONTROL_STATUS:
        {
			RETAILMSG(1,(_T("MSG:[%s]: CARPLAY_CONTROL_STATUS: %d\r\n"),TEXT(__FUNCTION__),lParam));

            m_bControlConnected = (BOOL)lParam;

			if(m_pSmartBarMedia && !m_bControlConnected && m_pSmartBarMedia->GetType()==SB_MEDIATYPE_CARPLAY)
			{
				m_pSmartBarMedia->SetType(SB_MEDIATYPE_NULL);
			}

			//reset current source:
			if(!m_bControlConnected && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_CARPLAY)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC = 0;
				//media bar:
				if(m_pSmartBarMedia)
				{
					m_pSmartBarMedia->SetType(SB_MEDIATYPE_NULL);
					m_pSmartBarMedia->SetPlayingFlag(FALSE);
				}
			}

			//reset para:
			if(!m_bControlConnected)
			{
				m_bCarPlayPhoneCallOn = FALSE;
				m_bCarPlaySpeechOn = FALSE;
				m_bCarPlayAltAudioOn = FALSE;
				m_bCarPlayMainAudioOn = FALSE;
				m_bCarPlayTurnbyTurnOn = FALSE;
				m_bCarPlayTakeMainAudio = FALSE;
			}
        }
        break;
    case CARPLAY_MAIN_AUDIO_STATUS:
        {
            if((BOOL)lParam)
    		{
    			if(!m_bCarPlayMainAudioOn)
    			{
    				m_bCarPlayMainAudioOn = TRUE;
    			}
    		}
    		else
    		{
    			if(m_bCarPlayMainAudioOn)
    			{
    				m_bCarPlayMainAudioOn = FALSE;
    			}
    		}
        }
        break;
    case CARPLAY_ALT_AUDIO_STATUS:
        {
            if((BOOL)lParam)
    		{
    			if(!m_bCarPlayAltAudioOn)
    			{
    				m_bCarPlayAltAudioOn = TRUE;
    			}
    		}
    		else
    		{
    			if(m_bCarPlayAltAudioOn)
    			{
    				m_bCarPlayAltAudioOn = FALSE;
    			}
    		}
        }
        break;
    case CARPLAY_SCREEN_STATUS:
        {
			RETAILMSG(1,(_T("##CarPlay Video connect: %d\r\n"),lParam));

			m_dwVideoConnectTick = GetTickCount();
            m_bVideoConnected = (BOOL)lParam;
            if (m_bVideoConnected)
            {
				//m_bRequestingCarPlayUI = FALSE;
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_CARPLAY)
				{
					SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,(1<<16)+0);
					m_pSkinManager->GetGUI()->EnableSwap(FALSE);
				}
				if(m_pSmartBarVol)
					m_pSmartBarVol->EnableShow(TRUE);
				if(m_pSmartBarMedia)
					m_pSmartBarMedia->EnableShow(TRUE);
            }
            else
            {
				//20160429 It may be changed the srceen status by OnPanelKey.
				/*
				m_pSkinManager->GetGUI()->EnableSwap(TRUE);
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_CARPLAY)
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
				}
				*/
        	}
        }
        break;
    case CARPLAY_MODESCHANGED_STATUS:
        {
            const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();

			RETAILMSG(1,( (L"##Mode: s=%d, a=%d, p=%d, sp=%d, sp.m=%d, t=%d\r\n"),
				mode->screen, mode->mainAudio, mode->phoneCall, mode->speech.entity,mode->speech.mode, mode->turnByTurn  ));

			//phone:
            if(mode->phoneCall == kAirPlayEntity_Controller)
    		{
				//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: phoneCall = Controller\r\n")));
				//no need auto change to carplay UI while phone-call or siri active,
				//unless current mode is video player, because video player will
				//take screen forever, if phone-call or siri want to borrow screen,
				//native UI should un-take screen and request carplay ui.
				//auto change to carplay UI while modechange screen=1, line 1082
				/*
    			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_CARPLAY)
    			{
					m_idUIBeforePhoneCallOn = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
					SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
    			}
				*/

    			//notify
    			if(!m_bCarPlayPhoneCallOn)
    			{
    				m_bCarPlayPhoneCallOn = TRUE;
    				//::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,TRUE,(1<<16)+HF_STATE_TALKING);
					//assume incoming-call, 
					//phone-call connected message will be posted while CARPLAY_PHONE_AUDIO_FORMAT received, @GUI_Fantasy.cpp
					::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,TRUE,(1<<16)+/*HF_STATE_TALKING*/HF_STATE_INCOMING_CALL);

					if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_CARPLAY)
					{
						SetTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_REQUEST_SCREEN,2000/*500*/,NULL);
					}
    			}
    		}
    		else
    		{
				//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: phoneCall != Controller\r\n")));
    			//notify
    			if(m_bCarPlayPhoneCallOn)
    			{
    				m_bCarPlayPhoneCallOn = FALSE;
    				m_dwDisableReloadScreenTick = GetTickCount();
    				::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,FALSE,(1<<16)+HF_STATE_IDLE);

					//return previous page:
					if(m_idUIBeforePhoneCallOn != 0)
					{
						m_bEnableQuit = TRUE;
						if(!m_bCarPlaySpeechOn &&GET_PANEL_CLASS_ID(m_idUIBeforePhoneCallOn) < UI_CLASS_MAX_VALID && GET_PANEL_CLASS_ID(m_idUIBeforePhoneCallOn) != UI_CLASS_IDLE)
							SwitchPage(GET_PANEL_CLASS_ID(m_idUIBeforePhoneCallOn),GET_PANEL_PAGE_ID(m_idUIBeforePhoneCallOn),PAGE_SWITCH_ANI_NONE);
						m_idUIBeforePhoneCallOn = 0;
					}

				}
    		}

    		//speech:
    		if(mode->speech.entity == kAirPlayEntity_Controller)
    		{
				//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: speech.entity = Controller\r\n")));
				//no need auto change to carplay UI while phone-call or siri active,
				//unless current mode is video player, because video player will
				//take screen forever, if phone-call or siri want to borrow screen,
				//native UI should un-take screen and request carplay ui.
				//auto change to carplay UI while modechange screen=1, line 1082
				/*
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_CARPLAY)
				{
					m_idUIBeforeSiri = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
    				SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
    			}
				*/

    			//notify
    			if(!m_bCarPlaySpeechOn && !m_bCarPlayPhoneCallOn)
    			{
    				m_bCarPlaySpeechOn = TRUE;
    				::SendMessage(m_hMainWnd,WM_MSG_SR,(/*m_bCarPlayAltAudioOn||*/m_bCarPlaySpeechOn),NULL);
    			}
    		}
    		else
    		{
				//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: speech.entity != Controller m_bCarPlaySpeechOn:%d\r\n"),m_bCarPlaySpeechOn));
    			//notify
    			if(m_bCarPlaySpeechOn)
    			{
    				m_bCarPlaySpeechOn = FALSE;
    				::SendMessage(m_hMainWnd,WM_MSG_SR,(/*m_bCarPlayAltAudioOn||*/m_bCarPlaySpeechOn),NULL);
					/*
					//return previous page:
					if(m_idUIBeforeSiri != 0)
					{
						m_bEnableQuit = TRUE;
						if(!m_bCarPlayPhoneCallOn && GET_PANEL_CLASS_ID(m_idUIBeforeSiri) < UI_CLASS_MAX_VALID && GET_PANEL_CLASS_ID(m_idUIBeforeSiri) != UI_CLASS_IDLE)
							SwitchPage(GET_PANEL_CLASS_ID(m_idUIBeforeSiri),GET_PANEL_PAGE_ID(m_idUIBeforeSiri),PAGE_SWITCH_ANI_NONE);
						m_idUIBeforeSiri = 0;
					}
					*/
				}
    		}

    		//mainAudio:
    		if(mode->mainAudio == kAirPlayEntity_Controller)
    		{
				//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: mainAudio = Controller\r\n")));
    			if(!m_bCarPlayTakeMainAudio && !m_bCarPlaySpeechOn && !m_bCarPlayPhoneCallOn)
    			{
    				m_idSRCBeforeCarplayTakeAudio=0;
					m_dwAutoSwitchCarplayRef=0;
    				//MuteAudio(TRUE,FALSE,_ARM_AUDIO_MUTE_ALL,FALSE,2000);
					//DuckAudio(TRUE,0,FALSE);

    				//KillTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_MEDIA);
    				SetTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_MEDIA,/*500*/100,NULL);

					m_bCarPlayTakeMainAudio = TRUE;
					if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) != UI_CLASS_CARPLAY)
					{
						m_idSRCBeforeCarplayTakeAudio = m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
					}
#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
					SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
#endif
    			}
    		}
    		else if(mode->mainAudio == kAirPlayEntity_Accessory)
    		{
				//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: mainAudio = Accessory\r\n")));


    			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

    			if(m_bCarPlayTakeMainAudio)
    			{
    				m_bCarPlayTakeMainAudio = FALSE;
    				if(m_idSRCBeforeCarplayTakeAudio /*&& !m_bRdsTAon*/)
    				{
						if(m_bConnected && m_idSystemPowerState == SYS_POWER_ON)
						{
    						SwitchMediaClassSRC(m_idSRCBeforeCarplayTakeAudio);
							
							#if CVTE_EN_KEY_PROXY
    						CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_RESUME);
							CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_RESUME);
							#else
							::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PLAY,NULL);
							#endif
						}
    					//reset:
    					m_idSRCBeforeCarplayTakeAudio = 0;
    				}
    			}

//     			//
//     			if(GET_PANEL_CLASS_ID(idSRC)==UI_CLASS_CARPLAY && m_pSmartBarMedia)
//     			{
//     				m_pSmartBarMedia->SetPlayingFlag(FALSE);
//     			}
    		}

#if 0
			//page enable quit control:
			m_bEnableQuit = (!m_bVideoConnected) || (/*!m_bCarPlayPhoneCallOn && */!m_bCarPlaySpeechOn) || (mode->screen == kAirPlayEntity_Accessory);
#endif

			//screen:
			if(mode->screen == kAirPlayEntity_Controller)
			{
				//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: screen = Controller\r\n")));
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_CARPLAY)
				{
					//RETAILMSG(1,( (L"MSG:[CPanelCarPlay::OnCarPlayMsg]: SwitchPage UI_CLASS_CARPLAY\r\n")));
					//SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,(1<<16)+0);
					SetTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_SCREEN,200,NULL);
					//m_dwAutoSwitchCarplayRef=0;
					//KillTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_MEDIA);
					//SetTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_MEDIA,200,NULL);
				}
			}
			else
			{
				SetTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_NATIVEUI,1000,NULL);
#if 0
				//if user request native ui, native UI should switch to
				//Corresponding page while carplay release UI screen resource
				if(m_NativeRequestUIPara.idClass != 0)
				{
					SwitchPage(m_NativeRequestUIPara.idClass,m_NativeRequestUIPara.idPage,PAGE_SWITCH_ANI_NONE,m_NativeRequestUIPara.pageParam);
					//reset:
					memset(&m_NativeRequestUIPara,0,sizeof(SWITCH_PAGE_PARAM));
				}
				else
				{
					//RETAILMSG(1, (TEXT("MSG:[PanelCarPlay::OnCarPlayMsg]: m_bVideoConnected:%d idCurUI:%X\r\n"),m_bVideoConnected,m_pSkinManager->GetConfig()->GetCurParam()->idCurUI));
					//Bug: Failed to quit carplay panel when m_bVideoConnected is false. whh
					if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_CARPLAY && !m_bRdsTAon/* && m_bVideoConnected*/)
					{
						//switch to last UI:
						if(m_bCarPlayTakeMainAudio && IsPanelHasValidAudioSrc(m_idPreviousPanel))
						{
							//if previous panel has valid audio source and
							//current audio is taken by carplay, then switch to
							//home page.
							SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
						}
						else
						{
							if(GET_PANEL_CLASS_ID(m_idPreviousPanel) < UI_CLASS_MAX_VALID && GET_PANEL_CLASS_ID(m_idPreviousPanel) != UI_CLASS_IDLE)
								SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
							else
								SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
						}
					}
				}
#endif
			}

    		//turnbyturn:
    		m_bCarPlayTurnbyTurnOn = (mode->turnByTurn == kAirPlayEntity_Controller);

        }
        break;
    case CARPLAY_REQUESTUI_STATUS:
        {
            //if(!m_bCarPlaySpeechOn && !m_bCarPlayPhoneCallOn)
			if(GetTickCount()-m_dwEnterTick>1000)
    		{
				m_bNeedTakeScreen = TRUE;
    			//StopCarPlay();
    			m_bNeedReturnHome = TRUE;
    			//quit to main menu:
    			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_CARPLAY)
    			{
    				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
    			}
    		}
        }
        break;
    default:
        break;
	}
}
void CPanelCarPlay::AutoSwitchNativeUI()
{
	const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();

	RETAILMSG(1/*DEBUG_CARPLAY*/, (TEXT("#### AutoSwitchNativeUI screen=%d####\r\n"),mode->screen));

	if(mode->screen == kAirPlayEntity_Accessory)
	{
		//if user request native ui, native UI should switch to
		//Corresponding page while carplay release UI screen resource
		if(m_NativeRequestUIPara.idClass != 0)
		{
			SwitchPage(m_NativeRequestUIPara.idClass,m_NativeRequestUIPara.idPage,PAGE_SWITCH_ANI_NONE,m_NativeRequestUIPara.pageParam);
			//reset:
			memset(&m_NativeRequestUIPara,0,sizeof(SWITCH_PAGE_PARAM));
		}
		else
		{
			//RETAILMSG(1, (TEXT("MSG:[PanelCarPlay::OnCarPlayMsg]: m_bVideoConnected:%d idCurUI:%X\r\n"),m_bVideoConnected,m_pSkinManager->GetConfig()->GetCurParam()->idCurUI));
			//Bug: Failed to quit carplay panel when m_bVideoConnected is false. whh
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_CARPLAY /*&& !m_bRdsTAon*//* && m_bVideoConnected*/)
			{
				//switch to last UI:
				if(m_bCarPlayTakeMainAudio && IsPanelHasValidAudioSrc(m_idPreviousPanel))
				{
					//if previous panel has valid audio source and
					//current audio is taken by carplay, then switch to
					//home page.
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
				}
				else
				{
					/*
					if(GET_PANEL_CLASS_ID(m_idPreviousPanel) < UI_CLASS_MAX_VALID && GET_PANEL_CLASS_ID(m_idPreviousPanel) != UI_CLASS_IDLE)
						SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
					else
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
					*/

					//if previous panel has valid source, but doesn't match current source,
					//don't switch the previous panel, just switch back to home page.
					UINT idUIClass=GET_PANEL_CLASS_ID(m_idPreviousPanel);
					UINT idSRCClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);
					UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

					if( (idUIClass > UI_CLASS_HOME && idUIClass < UI_CLASS_MAX_VALID && idUIClass != UI_CLASS_IDLE) )
					{
						if( IsPanelHasValidAudioSrc(m_idPreviousPanel) ? (idUIClass==idSRCClass) : TRUE )
							SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
						else
							SwitchPage(GET_PANEL_CLASS_ID(idSRC),GET_PANEL_PAGE_ID(idSRC),PAGE_SWITCH_ANI_NONE);
					}
					else
					{
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
					}
				}
			}
		}
	}
}
void CPanelCarPlay::AutoSwitchCarPlayMedia()
{
    const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
	const int max_cnt = 10;
	RETAILMSG(DEBUG_CARPLAY, (TEXT("#### AutoSwitchCarPlayMedia screen=%d, audio=%d, cnt=%d####\r\n"),
		mode->screen,mode->mainAudio,m_dwAutoSwitchCarplayRef));

	m_dwAutoSwitchCarplayRef++;
	if(m_dwAutoSwitchCarplayRef>max_cnt)
	{
		KillTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_MEDIA);
	}

	if(m_idSystemPowerState != SYS_POWER_ON)
	{
		KillTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_MEDIA);
		return;
	}


// 	UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
	if(m_dwAutoSwitchCarplayRef == 1)
	{
		if(!m_bCarPlayPhoneCallOn && !m_bCarPlaySpeechOn && mode->mainAudio == kAirPlayEntity_Controller)
		{
// 			m_bCarPlayTakeMainAudio = TRUE;

// 			if(GetCurrentSrcPlayingStatus() && GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_CARPLAY)
// 			{
// 				m_idSRCBeforeCarplayTakeAudio = idSRC;
// 			}

			SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER),1);
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
			MuteAudio(FALSE,FALSE,_ARM_MUTE_ALL,FALSE,100);
			//
// 			if(GET_PANEL_CLASS_ID(idSRC)==UI_CLASS_CARPLAY && m_pSmartBarMedia)
// 			{
// 				m_pSmartBarMedia->SetPlayingFlag(TRUE);
// 			}
		}
	}
	else if(m_dwAutoSwitchCarplayRef == max_cnt)
	{
		if(mode->mainAudio == kAirPlayEntity_Controller || mode->screen == kAirPlayEntity_Controller)
		{
			UINT idclass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);
			BOOL bPanelHasSRC =
				(idclass==UI_CLASS_RADIO) ||
				(idclass==UI_CLASS_BT) ||
				(idclass==UI_CLASS_IPOD) ||
				(idclass==UI_CLASS_TV) ||
				(idclass==UI_CLASS_DISK) ||
				(idclass==UI_CLASS_AUX) ||
				(idclass==UI_CLASS_MEDIA);

			if(idclass != UI_CLASS_CARPLAY && bPanelHasSRC)
			{
				//switch to homepage instead
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
				//SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
			}
		}
	}
}

void CPanelCarPlay::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	UINT idPhoneCallType = HIWORD(lParam);//0-bluetooth, 1-carplay
	UINT idCallPara=LOWORD(lParam);
	m_bPhoneCallActive = wParam;

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_CARPLAY)
	{
		if(wParam)//active
		{

		}
		else
		{
			//SwitchAudioSRC(SRC_AUDIO_ARM);//20160428

		}
	}
}

#endif
