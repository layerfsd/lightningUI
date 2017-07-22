#include "PanelApp.h"
#include "../Settings/SettingCommon.h"
#if CVTE_EN_CARPLAY
#include "CarplayAPI.h"
#endif
#include "../../CarPlay/CarplayAdapter.h"

extern HWND			g_hMainWnd;
extern HWND			g_hUIWnd;
extern BOOL				g_bNaviSpeechEnabled;
extern DWORD					g_dwNaviProcessID;
extern DWORD					g_dwTimeSyncedCnt;
extern const wchar_t* g_strDefaultBackground;

BOOL   CALLBACK   EnumWinProc(HWND   hwnd,   LPARAM   lParam)
{
	DWORD   dwID;

	EnumParam*   pep   =   (EnumParam*)lParam;
	GetWindowThreadProcessId(hwnd,   &dwID);

// 	wchar_t temp[MAX_STRING];
// 	GetWindowText(hwnd,temp,MAX_STRING);
// 	RETAILMSG(1, (TEXT(">>EnumWinProc,dwID:0x%x, wnd: 0x%x\r\n"),dwID,hwnd));

	if   (dwID   ==   pep->dwProcessID && pep->dwWndNum<15)
	{
		//check exist:
		BOOL bexist=FALSE;
		if(pep->dwWndNum)
		{
			for (int i=0;i<pep->dwWndNum;i++)
			{
				if(hwnd == pep->hWnd[i])
				{
					bexist=TRUE;
					break;
				}
			}
		}

		if(!bexist && IsWindowVisible(hwnd))
		{
			pep->hWnd[pep->dwWndNum]   =   hwnd;
			pep->dwWndNum++;
		}
		return   TRUE;
	}
	return   TRUE;
}

CPanelApp::CPanelApp(UINT idIcon,UINT idBackTex)
{
	m_hAppWnd=NULL;
	m_hAppProcess=NULL;
	m_bAppLaunched=FALSE;
	m_bAppStartedSuceess=FALSE;

	m_idBackTex = idBackTex;
	m_idIcon = idIcon;

	m_hWindowDetectThread = INVALID_HANDLE_VALUE;
	m_bEnableWindowDetect = FALSE;
	m_hWindowDetectEvent = INVALID_HANDLE_VALUE;

	m_dwEnterTick = 0;
	m_bHasValidBackImage = FALSE;

	m_bPhoneCallActive = FALSE;
	m_bCarPlayConnected = FALSE;

	m_idAudioSRC = -1;
	m_pSmartBarDrop = NULL;
	m_pSmartBarMedia = NULL;

	m_bAutoShowDropBar = FALSE;
	m_bEnableShowDropBar = FALSE;
	m_bRdsTAon = FALSE;
	m_bSRon = FALSE;

	m_hAppLaunchThread = NULL;

	m_iGPSGroundSpeed = 0;
	m_bNeedRenderSplash = TRUE;

    m_idPowerState = SYS_POWER_ON;
    m_bPowerStateDone = TRUE;
}

CPanelApp::~CPanelApp(void)
{
}
BOOL CPanelApp::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelApp::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[PanelApp::OnEnterPanel] ++\r\n")));

	//make animation:
	if(idAni != ANIM_NONE)
	{
        m_bNeedRenderSplash = TRUE;
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
        m_bNeedRenderSplash = FALSE;
		SetAlpha(1.0f);
	}

	m_bAppLaunched=FALSE;


	if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
	{
		if(!m_bPhoneCallActive && m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
		{
			EnableNaviSpeech(TRUE);
		}

		#if CVTE_EN_CARPLAY
		//
// 		OSStatus err = CarplayAPIChangeAppState(kAirPlaySpeechMode_NotApplicable,kAirPlayTriState_NotApplicable,kAirPlayTriState_True,"Native map open");
// 		RETAILMSG(1, (TEXT("$$$$$$$$$$$ take turn-by-turn, 0x%x...\r\n"),err));
		#endif
	}

	//
	if(m_idAudioSRC != -1)
	{
		SwitchAudioSRC(m_idAudioSRC,TRUE);
	}

	m_dwEnterTick = GetTickCount();

	UINT idSrc=GET_PANEL_CLASS_ID( m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);
	if(idSrc==UI_CLASS_BT || idSrc==UI_CLASS_RADIO || idSrc==UI_CLASS_MEDIA || idSrc==UI_CLASS_IPOD || idSrc==UI_CLASS_CARPLAY)
	{
		m_bAutoShowDropBar = CVTE_EN_NAVI_DROP_BAR;
		m_bEnableShowDropBar = CVTE_EN_NAVI_DROP_BAR;
	}
	else
	{
		m_bAutoShowDropBar = FALSE;
		m_bEnableShowDropBar = FALSE;
	}
	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[PanelApp::OnEnterPanel] --\r\n")));

}
void CPanelApp::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[PanelApp::OnQuitPanel] ++\r\n")));

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	//stop windows detect:
	if(m_bEnableWindowDetect)
	{
		SetEvent(m_hWindowDetectEvent);
		m_bEnableWindowDetect=FALSE;
	}

	//get thumbnail:
	if(m_idBackTex != 0)
	{
		if(GetTickCount()-m_dwEnterTick>2000 && m_hAppProcess!=NULL && IsWindowVisible(m_hAppWnd) /*&& IsAniReady()*/)
		{
			RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[PanelApp::OnQuitPanel]: ReloadFormScreen..,%x\r\n"),m_hAppWnd));

			CTexture *pTex=m_bk.GetTexture();
			if(pTex)
			{
				pTex->ReloadFormScreen();
			}
		}
	}

 	if(IsWindow(m_hAppWnd) && IsWindowVisible(m_hAppWnd))
 	{
		ShowWindow(m_hAppWnd,SW_HIDE);
 	}

	//RETAILMSG(DEBUG_PANELAPP, (TEXT("$$$$$$$$$$$CPanelApp::OnQuitPanel wnd:0x%x...\r\n"),GetForegroundWindow()));

	if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
		{
			EnableNaviSpeech(FALSE);
		}
	}

	if(m_pSmartBarDrop)
	{
		m_pSmartBarDrop->Show(FALSE,INFINITE);
	}
	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[PanelApp::OnQuitPanel] --\r\n")));

}
DWORD CPanelApp::WindowDetectThreadPro(LPVOID lpParam)
{
	CPanelApp *panel = (CPanelApp*)lpParam;

	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::WindowDetectThreadPro] ++\r\n")));

	while (TRUE)
	{
		WaitForSingleObject(panel->m_hWindowDetectEvent,1000);

		if(panel->m_bEnableWindowDetect && panel->m_idPowerState == SYS_POWER_ON)
		{
			if(panel)
			{
				panel->OnAppWindowDetect();
			}
		}
		else
		{
			break;
		}

	}

	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::WindowDetectThreadPro] --\r\n")));


	return 0;
}
void CPanelApp::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnSystemPowerStatePrepare] idState:%d\r\n"),idState));
    m_idPowerState = idState;
    m_bPowerStateDone = FALSE;
	switch (idState)
	{
	case SYS_POWER_OFF:
		{
			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
				if(g_bNaviSpeechEnabled)
				{
					EnableNaviSpeech(FALSE);
				}
			}
		}
		break;
	case SYS_POWER_IDLE:
		{
			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
				if(g_bNaviSpeechEnabled)
				{
					EnableNaviSpeech(FALSE);
				}
			}
		}
		break;
	case SYS_POWER_ON:
		{
			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
				if(!g_bNaviSpeechEnabled && !m_bPhoneCallActive)
				{
					if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)//
					{
						EnableNaviSpeech(TRUE);
					}
					else if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_NAVI,0) &&
						m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
					{
						EnableNaviSpeech(TRUE);
					}
				}
			}
		}
		break;
	}
}
void CPanelApp::OnSystemPowerStateDone(UINT idState)
{
	CPanelGL::OnSystemPowerStateDone(idState);
    m_bPowerStateDone = TRUE;
}

#if CVTE_EN_CARPLAY
void CPanelApp::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CARPLAY_CONNECT_STATUS:
		{
			m_bCarPlayConnected = (BOOL)lParam;
		}
		break;
	case CARPLAY_MODESCHANGED_STATUS:
		{
			const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
					
				//do not use unsafe TerminateProcess function
#if 0			
				if((mode->turnByTurn == kAirPlayEntity_Controller))
				{

					if(m_hAppProcess != NULL)
					{
						RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnCarPlayMsg] iphone take map, terminate native map...\r\n")));
						if(TerminateProcess(m_hAppProcess,0))
						{
							RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnCarPlayMsg] TerminateProcess succeed...\r\n")));

							DWORD code=WaitForSingleObject(m_hAppProcess,0);
							if(code == WAIT_OBJECT_0 || code == WAIT_FAILED)
							{
								CloseHandle(m_hAppProcess);
								m_hAppWnd=NULL;
								m_hAppProcess=NULL;
								m_bAppStartedSuceess=FALSE;
							}

							//need to auto switch Carplay?
							//SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE);

							//
							CCarplayAdapter::GetInstance()->SetUserSelectNativeNavi(FALSE);                            
						}
						else
						{
							RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnCarPlayMsg] TerminateProcess failed err=0x%x...\r\n"),GetLastError()));

						}
					}
				}
#endif			

				BOOL bCarPlayNaviSpeechEnabled = (mode->turnByTurn == kAirPlayEntity_Controller);

				if(bCarPlayNaviSpeechEnabled)//carplay turn-by-turn active
				{
					
// 					if(g_bNaviSpeechEnabled)
// 					{
// 						EnableNaviSpeech(FALSE);
// 					}

					//if current page is navigation, switch to home page:
					if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
					{
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
					}
				}
				else
				{
					if(m_idSystemPowerState == SYS_POWER_ON && !g_bNaviSpeechEnabled /*&& !m_bPhoneCallActive*/ && !m_bRdsTAon && !m_bSRon)
					{
						if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)//
						{
							EnableNaviSpeech(TRUE);
						}
						else if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_NAVI,0) &&
							m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
						{
							EnableNaviSpeech(TRUE);
						}
					}
				}
			}
		}
		break;
	}
}
#endif
#if CVTE_EN_ANDROID_AUTO
void CPanelApp::OnAndroidAutoMsg(WPARAM wParam, LPARAM lParam)
{
	if(wParam == ANDROIDAUTO_NAVI_STATUS)
	{
		if(lParam)
		{
			//terminate native guidance:
			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
				if(m_hAppProcess != NULL)
				{
					RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnAndroidAutoMsg] phone take map, terminate native map...\r\n")));
					if(TerminateProcess(m_hAppProcess,0))
					{

						DWORD code=WaitForSingleObject(m_hAppProcess,0);
						if(code == WAIT_OBJECT_0 || code == WAIT_FAILED)
						{
							CloseHandle(m_hAppProcess);
							m_hAppWnd=NULL;
							m_hAppProcess=NULL;
							m_bAppStartedSuceess=FALSE;
						} 
						m_bHasValidBackImage = FALSE;
						if ( GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_NAVI )
						{
							SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
						}
						RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnAndroidAutoMsg] TerminateProcess succeed...\r\n")));
					}
					else
					{
						RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnAndroidAutoMsg] TerminateProcess failed err=0x%x...\r\n"),GetLastError()));
					}
				}
			}
		}
	}
}
#endif
void CPanelApp::OnSRMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_PANELAPP,(_T("MSG:[CPanelApp::OnSRMsg]: ++ wParam:%d,lParam:%d\r\n"),wParam,lParam));
	m_bSRon = wParam;

	if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
	{
		if(wParam)//active
		{
			if(g_bNaviSpeechEnabled)
			{
				EnableNaviSpeech(FALSE);
			}
		}
		else
		{
			if(!g_bNaviSpeechEnabled && !m_bPhoneCallActive && !m_bRdsTAon && !m_bSRon)
			{
				if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)//
				{
					EnableNaviSpeech(TRUE);
				}
				else if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_NAVI,0) &&
					m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
				{
					EnableNaviSpeech(TRUE);
				}
			}
		}
	}
	RETAILMSG(DEBUG_PANELAPP,(_T("MSG:[CPanelApp::OnSRMsg]: --\r\n")));
}
void CPanelApp::OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam)
{
	//20160428
	m_bRdsTAon = wParam;

	if(/*!m_bCarPlayConnected && */m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
	{
		if(wParam)//active
		{
			if(g_bNaviSpeechEnabled)
			{
				EnableNaviSpeech(FALSE);
			}
		}
		else
		{
			if(!g_bNaviSpeechEnabled && !m_bPhoneCallActive && !m_bRdsTAon && !m_bSRon)
			{
				if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)//
				{
					EnableNaviSpeech(TRUE);
				}
				else if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_NAVI,0) &&
					m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
				{
					EnableNaviSpeech(TRUE);
				}
			}
		}
	}
}
void CPanelApp::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;

	RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnPhoneCallState]: %d,m_bCarPlayConnected:%d...\r\n"),wParam,m_bCarPlayConnected));

	if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0) && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI)==UI_CLASS_NAVI)
	{
		if(wParam)
		{
			if(m_pSmartBarDrop)
			{
				m_pSmartBarDrop->Show(FALSE,INFINITE);
			}
		}
		else
		{
			if(m_pSmartBarDrop && m_bEnableShowDropBar && (m_pSmartBarMedia?(!m_pSmartBarMedia->IsVisible()):TRUE))
			{
				m_pSmartBarDrop->Show(TRUE,INFINITE);
			}
		}
	}


#if 1
	if(/*!m_bCarPlayConnected && */m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
	{
		if(wParam)//active
		{
			if(g_bNaviSpeechEnabled)
			{
				EnableNaviSpeech(FALSE);
			}
		}
		else
		{
			if(!g_bNaviSpeechEnabled && !m_bPhoneCallActive && !m_bRdsTAon && !m_bSRon)
			{
				if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)//
				{
					EnableNaviSpeech(TRUE);
				}
				else if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_NAVI,0) &&
							m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)//
				{
					EnableNaviSpeech(TRUE);
				}
			}
		}
	}
#endif

}
void CPanelApp::Render()
{
	static wchar_t str[16];
	static COLORGL cr2={1,1,1,1};

// 	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_100):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,0.8};
// 	cr.a=m_pSkinManager->GetConfig()->GetCurParam()->fUITransparency;

	if(!BeginRender())return;

// 	DrawRect(pTexBack,0,0,794-32,356-32,16,&cr,TRUE);

	// 	m_tagLoading.SetAlpha(m_fCurAlpha);

// 	if(!(m_bAppLaunched&&m_bAppStartedSuceess))
// 	{
// 		VIEW_STATE vs=m_tagLoading.GetCurViewState();
// 		vs.fRotateZ+=5;
// 		vs.fRotateZ=int(vs.fRotateZ)%360;
//
// 		m_tagLoading.SetCurViewState(&vs);
// 		m_tagLoading.SetAlpha(m_fCurAlpha);
// 		m_tagLoading.Render();
// 	}

	if(m_bHasValidBackImage)
	{
		if(m_idBackTex != 0)
		{
			m_bk.SetAlpha(m_fCurAlpha);
			m_bk.Render();
		}
	}
	else
	{
		COLORGL cr_b={0.0,0.0,0.0,0.0};
		DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr_b);

		if(m_idIcon != 0 && (m_bNeedRenderSplash || m_hAppProcess==NULL))
		{
			m_iconApp.SetAlpha(m_fCurAlpha);
			m_iconApp.Render();
		}
	}

	//launch app:
	if(IsAniReady() && GetTickCount()-m_dwEnterTick>500)
	{
#if 1
		if(m_hAppLaunchThread == NULL&& !m_bAppLaunched)
		{
			m_hAppLaunchThread=CreateThread(NULL,NULL,LaunchThreadPro,this,NULL,NULL);
		}
#else
		if(!m_bAppLaunched)
		{
			m_pSkinManager->GetGUI()->EnableSwap(FALSE);

			if(!LaunchApp())
			{
				m_bAppStartedSuceess=FALSE;

				EnableQuit(TRUE);

				if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
				{
					/*SwitchPage(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_NAVIGATION),0,FALSE);*/
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);//quit
				}
				else
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);//quit
				}
			}
			else
			{
				m_bAppStartedSuceess=TRUE;

				//window detect:
				if(!m_bEnableWindowDetect)
				{
					if(m_hWindowDetectThread!=INVALID_HANDLE_VALUE)
						CloseHandle(m_hWindowDetectThread);

					Sleep(10);
					BOOL bFocus = TRUE;
					m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_NAVI,0,UIOPRT_SETSTATUS,&bFocus);
					//begin thread to explore for video files:
					m_hWindowDetectThread=CreateThread(NULL,NULL,WindowDetectThreadPro,this,NULL,NULL);

					m_bEnableWindowDetect=TRUE;
				}

				//
				m_bHasValidBackImage = TRUE;
				if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
				{
					::PostMessage(m_hMainWnd,WM_NAVI_MSG,NAVI_START_MSG,1);
				}
			}

			m_bAppLaunched=TRUE;
		}
#endif
	}

	EndRender();

}
void CPanelApp::OnAppWindowDetect()
{
	static DWORD dwLastTestWindow = 0;
	//RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnAppWindowDetect]: m_hAppProcess[%d] m_bEnableShowDropBar[%d] m_bAutoShowDropBar[%d]\r\n"),m_hAppProcess,m_bEnableShowDropBar,m_bAutoShowDropBar));

	//window detect:
	//if( m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_NAVI,0) /*&& GetTickCount() - dwLastTestWindow >1000*/)
	{
		//dwLastTestWindow = GetTickCount();

// 		RETAILMSG(DEBUG_PANELAPP, (TEXT("****CPanelApp::OnAppWindowDetect enter...\r\n")));

		if(/*m_hAppWnd*/m_hAppProcess)
		{
			DWORD code=WaitForSingleObject(m_hAppProcess,0);
			if(code == WAIT_OBJECT_0 || code == WAIT_FAILED)
			{
				CloseHandle(m_hAppProcess);
				m_hAppWnd=NULL;
				m_hAppProcess=NULL;
				m_bAppStartedSuceess=FALSE;


				EnableQuit(TRUE);
				RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnAppWindowDetect]: quit..,%x\r\n"),m_hAppWnd));

				m_pSkinManager->GetGUI()->EnableSwap(TRUE);
				BOOL bFocus = FALSE;
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_NAVI,0,UIOPRT_SHOWHIDE,&bFocus);
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);//quit

				m_bEnableWindowDetect = FALSE;

				//
				m_bHasValidBackImage = FALSE;
				if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
				{
					::PostMessage(m_hMainWnd,WM_NAVI_MSG,NAVI_START_MSG,0);
				}


	#if CVTE_EN_CARPLAY
				//notify carplay ini-modes
				CCarplayAdapter::GetInstance()->SetUserSelectNativeNavi(FALSE);

				//notify carplay:
				CarplayAPIChangeAppState(kAirPlaySpeechMode_NotApplicable,kAirPlayTriState_NotApplicable,kAirPlayTriState_False,"Native map close");
	#endif
			}
			else if(/*GetForegroundWindow()==(/ *m_hMainWnd* /g_hMainWnd) &&*/ IsWindow(m_hAppWnd) && (!IsWindowVisible(m_hAppWnd)/*||GetForegroundWindow()!=m_hAppWnd*/))
			{
				//m_bAppStartedSuceess=FALSE;
				RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnAppWindowDetect]: window hide,%x,GetForegroundWindow wnd:%x\r\n"),m_hAppWnd,GetForegroundWindow()));

				m_pSkinManager->GetGUI()->EnableSwap(TRUE);

				EnableQuit(TRUE);
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);//quit


				m_bEnableWindowDetect = FALSE;

				//
				if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
				{
					PostMessage(m_hMainWnd,WM_NAVI_MSG,NAVI_START_MSG,2);
				}
			}
			else
			{
                //New carplay logic 20160514
				//SetWindowPos(m_hAppWnd,HWND_TOPMOST,0,0,g_iClientWidth,g_iClientHeight,SWP_SHOWWINDOW);
				if(GetTickCount()-m_dwEnterTick>2000 && m_bEnableShowDropBar && m_bAutoShowDropBar)
				{
					m_bAutoShowDropBar = FALSE;

					if(m_pSmartBarDrop && !m_bPhoneCallActive && (m_pSmartBarMedia?(!m_pSmartBarMedia->IsVisible()):TRUE))
					{
				        RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::OnAppWindowDetect]: Show\r\n")));
						m_pSmartBarDrop->Show(TRUE,INFINITE);
					}
				}
			}
		}
	}
}

BOOL CPanelApp::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	COLORGL crw={1,0,0,1};

	m_bShowStatusBar = FALSE;

	m_hWindowDetectEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	if(m_idBackTex != 0)
	{
		if(m_pSkinManager->GetTexture(m_idBackTex) == NULL)
		{
			if(!m_pSkinManager->AddTextureFromFile(m_idBackTex,g_strDefaultBackground,g_iScreenWidth/*1024*/,g_iScreenHeight/*512*/))
	// 		if(!m_pSkinManager->AddTextureFromFile(m_idBackTex,g_strDefaultBackground,512,256))
			{
				RETAILMSG(DEBUG_PANELAPP, (TEXT("MSG:[CPanelApp::Initialize] background texture FAILED!\r\n")));
			}
		}

		//background:
		m_bk.Initialize(IDC_TAG_BACKGROUND,NULL,m_pSkinManager->GetTexture(m_idBackTex),NULL,TRUE);
	// 	VIEW_STATE vs=m_bk.GetCurViewState();
	// 	vs.fRotateX=-180;
	// 	m_bk.SetIniViewState(&vs);
	// 	m_bk.SetScale(g_iScreenWidth/1024.0f,g_iScreenHeight/512.0f);

		VIEW_STATE vs=m_bk.GetCurViewState();
		vs.fRotateX=-180;
		m_bk.SetIniViewState(&vs);
	}


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_iconApp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_118));
#else
	if(m_idIcon != 0)
	{
		m_iconApp.Initialize(0,NULL,m_pSkinManager->GetTexture(m_idIcon));
		m_iconApp.SetShowScale(1.2f);
	}
#endif

	return TRUE;
}

void CPanelApp::OnBack()
{
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);//quit

}
void CPanelApp::OnGestureMsg(UINT idGesture)
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
DWORD CPanelApp::LaunchThreadPro(LPVOID lpParam)
{
	CPanelApp *panel = (CPanelApp*)lpParam;

	panel->EnableQuit(FALSE);

	RETAILMSG(1/*DEBUG_PANELAPP*/, (TEXT("MSG:[CPanelApp::LaunchThreadPro] enter...\r\n")));

	if(panel)
	{
		panel->LaunchAppProc();
	}

	//Sleep(100);

	RETAILMSG(1/*DEBUG_PANELAPP*/, (TEXT("MSG:[CPanelApp::LaunchThreadPro] quit...\r\n")));

	CloseHandle(panel->m_hAppLaunchThread);
	panel->m_hAppLaunchThread=NULL;

	panel->EnableQuit(TRUE);

	return 0;
}
BOOL CPanelApp::LaunchAppProc()
{
	BOOL bRet=TRUE;

	if(!m_bAppLaunched)
	{
		m_pSkinManager->GetGUI()->EnableSwap(FALSE);

		if(!LaunchApp())
		{
			m_bAppStartedSuceess=FALSE;

			EnableQuit(TRUE);

			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
				/*SwitchPage(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_NAVIGATION),0,FALSE);*/
                RETAILMSG(1, (TEXT("ERR:[CPanelApp::LaunchAppProc] SwitchPage UI_CLASS_HOME 1\r\n")));
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);//quit
			}
			else
			{
                RETAILMSG(1, (TEXT("ERR:[CPanelApp::LaunchAppProc] SwitchPage UI_CLASS_HOME 2\r\n")));
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);//quit
			}
			bRet=FALSE;
		}
		else
		{
			//notify carplay ini-modes
			#if CVTE_EN_CARPLAY
			CCarplayAdapter::GetInstance()->SetUserSelectNativeNavi(TRUE);

			OSStatus err = CarplayAPIChangeAppState(kAirPlaySpeechMode_NotApplicable,kAirPlayTriState_NotApplicable,kAirPlayTriState_True,"Native map open");
			RETAILMSG(1, (TEXT("$$$$$$$$$$$ take turn-by-turn, 0x%x...\r\n"),err));
			#endif
            
			m_bAppStartedSuceess=TRUE;

			//window detect:
			if(!m_bEnableWindowDetect)
			{
				if(m_hWindowDetectThread!=INVALID_HANDLE_VALUE)
					CloseHandle(m_hWindowDetectThread);

				Sleep(10);
				BOOL bFocus = TRUE;
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_NAVI,0,UIOPRT_SHOWHIDE,&bFocus);
				//begin thread to explore for video files:
				m_hWindowDetectThread=CreateThread(NULL,NULL,WindowDetectThreadPro,this,NULL,NULL);

				m_bEnableWindowDetect=TRUE;
			}

			//
			m_bHasValidBackImage = TRUE;
			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
				::PostMessage(m_hMainWnd,WM_NAVI_MSG,NAVI_START_MSG,1);
			}

			bRet=TRUE;
		}

		m_bAppLaunched=TRUE;
	}

	return bRet;
}
BOOL CPanelApp::LaunchApp()
{
	if(IsWindow(m_hAppWnd))
	{
		BOOL bRet=FALSE;
		RETAILMSG(1/*DEBUG_PANELAPP*/, (TEXT("MSG:[CPanelApp::LaunchApp] window exist,%x,show window\r\n"),m_hAppWnd));

		//
		ShowWindow(g_hMainWnd,SW_HIDE);

		Sleep(10);

		if(!IsWindowVisible(m_hAppWnd))
		{
			bRet=ShowWindow(m_hAppWnd,SW_SHOW);
	// 		RETAILMSG(DEBUG_PANELAPP, (TEXT("APP: ShowWindow SW_SHOW:%d\r\n"),bRet));

	 		bRet=SetForegroundWindow((HWND)(((ULONG) m_hAppWnd) | 0x01) );
	// 		RETAILMSG(DEBUG_PANELAPP, (TEXT("APP: SetForegroundWindow m_hAppWnd) | 0x01:%d\r\n"),bRet));

			bRet=SetForegroundWindow(m_hAppWnd);
	// 		RETAILMSG(DEBUG_PANELAPP, (TEXT("APP: SetForegroundWindow m_hAppWnd:%d\r\n"),bRet));

			Sleep(100);

			SetWindowPos(m_hAppWnd,HWND_TOPMOST,0,0,g_iClientWidth,g_iClientHeight,SWP_SHOWWINDOW);

			bRet = TRUE;
		}

		Sleep(10);

		return TRUE;
	}
	else
	{
		PROCESS_INFORMATION pi;
		if(CreateProcess(m_strAppPath.String(),NULL,NULL,NULL,0,0,NULL,NULL,NULL,&pi))//set correct location:
		{
			memset(&m_ep,0,sizeof(EnumParam));
			m_ep.dwProcessID   =   pi.dwProcessId;//GetProcessId(ShExeInfo.hProcess);//.dwProcessId;

			int count=0;
			m_hAppProcess=pi.hProcess;//ShExeInfo.hProcess;

			g_dwNaviProcessID = m_ep.dwProcessID;

			RETAILMSG(1/*DEBUG_PANELAPP*/,(_T("MSG:[CPanelApp::LaunchApp] CreateProcess succeed:%s.hProcess:%x,dwProcessId:%x,\r\n"),
				m_strAppPath.String(),m_hAppProcess,m_ep.dwProcessID));

/*
            do
            {
                Sleep(1000);
            }while(m_idPowerState != SYS_POWER_ON || !m_bPowerStateDone);
*/			
			Sleep(4000);
			RETAILMSG(DEBUG_PANELAPP,(_T("MSG:[%s]:Hide MainWnd \r\n"),TEXT(__FUNCTION__)));

			ShowWindow(g_hMainWnd,SW_HIDE);

			while(m_ep.dwWndNum==0 && count<5 /*|| m_ep.dwWndNum==0*/)
			{
				EnumWindows((WNDENUMPROC)EnumWinProc,   (long)&m_ep);
				count++;

				if(m_ep.dwWndNum)
				{
					for (int i=0;i<m_ep.dwWndNum;i++)
					{
						RETAILMSG(1/*DEBUG_PANELAPP*/,(_T("MSG:[%s]:Get wnd: 0x%x, total cnt:%d\r\n"),TEXT(__FUNCTION__),m_ep.hWnd[i],m_ep.dwWndNum));
					}
					break;
				}

				Sleep(2000);
				/*
				if(ep.hMainWnd==NULL)
				{
					DWORD code=WaitForSingleObject(m_hAppProcess,500);

					RETAILMSG(DEBUG_PANELAPP,(_T("WaitForSingleObject m_hAppProcess:%d\r\n"),code));

// 					if(code == WAIT_OBJECT_0 || code == WAIT_FAILED)
// 					{
// 						break;
// 					}
// 					else
					{
						Sleep(500);
					}

					RETAILMSG(DEBUG_PANELAPP,(_T("EnumWindows failed,next.\r\n")));
				}
				else
				{
					RETAILMSG(DEBUG_PANELAPP,(_T("EnumWindows SUCCEED!. 0x%x\r\n"),ep.hMainWnd));
					break;
				}
				*/
			}

			RETAILMSG(1/*DEBUG_PANELAPP*/,(_T("MSG:[%s]: End while\r\n"),TEXT(__FUNCTION__)));

			m_hAppWnd=m_ep.hWnd[/*m_ep.dwWndNum-1*/0];

            while(m_idPowerState != SYS_POWER_ON && (m_idPowerState == SYS_POWER_IDLE && !m_bPowerStateDone))
            {
                Sleep(1000);
            }

			RETAILMSG(DEBUG_PANELAPP,(_T("MSG:[%s]: ShowWindow \r\n"),TEXT(__FUNCTION__)));

			ShowWindow(m_hAppWnd,SW_SHOW);
			SetForegroundWindow((HWND)(((ULONG) m_hAppWnd) | 0x01) );
			SetForegroundWindow(m_hAppWnd);

			SetWindowPos(m_hAppWnd,HWND_TOPMOST,0,0,g_iClientWidth,g_iClientHeight,SWP_SHOWWINDOW);

			//
			if(m_ID == MAKE_PANEL_ID(UI_CLASS_NAVI,0))
			{
				g_dwTimeSyncedCnt = 0;
			}

			return TRUE;
		}
		else
		{
			RETAILMSG(DEBUG_PANELAPP,(_T("MSG:[CPanelApp::LaunchApp] CreateProcess failed:%s,next.\r\n"),m_strAppPath.String()));

			m_hAppWnd=NULL;
			m_hAppProcess=NULL;
			return FALSE;
		}
	}
	//Sleep(1000);

}
