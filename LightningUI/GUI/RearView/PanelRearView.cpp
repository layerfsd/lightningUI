#include "PanelRearView.h"
#include "../../resource.h"
#include "../../Backlight/Backlight.h"

#if CVTE_EN_CAMERA
CPanelRearView::CPanelRearView(void)
{
	m_pVideoWindow = NULL;

	m_dwEnterTick = 0;
	m_bHasRadar = FALSE;//TRUE;

	m_bCAMVideoStarted = FALSE;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_hRearMonitorStartEvt = NULL;
	m_hRearMonitorEndEvt = NULL;
	m_hRearMonitorThread = NULL;
	m_bHasVideoSignal = TRUE;
	m_bRearMonitorWorking = FALSE;
#endif
}

CPanelRearView::~CPanelRearView(void)
{
}
BOOL CPanelRearView::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelRearView::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	StopRearMonitor();
#endif

	if(m_pVideoWindow && m_pVideoWindow->IsRunning())
	{
		m_pVideoWindow->SetGestureFunc(NULL,NULL);
		m_pVideoWindow->StopVideo();
	}
	if(m_pSmartBarVol)
	{
		m_pSmartBarVol->EnableShow(TRUE);
	}

#if (CVTE_DEF_CAMERA_WIDGET_TYPE != CVTE_DEF_CAMERA_WIDGET_TYPE_360)
	if(m_pRearOverlay)
	{
		m_pRearOverlay->Show(FALSE);
	}
#endif

	if(m_pSmartBarTopBanner)
	{
		m_pSmartBarTopBanner->EnableShow(TRUE);
	}

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_CAMERA_POWER_KEY,CAMERA_POWER_OFF);
	}
}
void CPanelRearView::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

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

	if(m_pSmartBarVol)
	{
		m_pSmartBarVol->EnableShow(FALSE);
	}

	if(m_pSmartBarTopBanner)
	{
		m_pSmartBarTopBanner->EnableShow(FALSE);
	}

	//MCU command:
	if(m_pCommander)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]: SendCmd[%d][%d]\r\n"), TEXT(__FUNCTION__),_ARM_VIDEO_SW_KEY,SRC_VIDEO_CAMERA));
		m_pCommander->SendCmd(_ARM_VIDEO_SW_KEY,SRC_VIDEO_CAMERA);
		m_pCommander->SendCmd(_ARM_CAMERA_POWER_KEY,CAMERA_POWER_ON);
	}
}
void CPanelRearView::Render()
{
	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_078):NULL;
	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_043):NULL;
 	static COLORGL crBK={0.375,0.363,0.359,1.0};

	if(!BeginRender())return;

	COLORGL cr={1.0,1.0,1.0,/*m_fCurAlpha*/1.0};

	BOOL bHasVideoSignal=FALSE;

	if(m_bHasRadar)
	{

		int x=168;
		int y=72;
		int cx=464;
		int cy=256;

		if(m_pVideoWindow)
		{
			m_pVideoWindow->SetWindowMode(VIP_WND_PIP_REVERSE);
			m_pVideoWindow->GetWindowPos(&x,&y,&cx,&cy);
			bHasVideoSignal=m_pVideoWindow->HasVideoSignal();
		}


		int cx2=(g_iClientWidth-cx-20);

		DrawRect(pTexVideoBack,g_iClientWidth/2-cx2/2,((g_iClientHeight/2)-y-cy/2),cx2-20,cy,10,&cr,TRUE);

		DrawRect(pTexVideoBack,-((g_iClientWidth/2)-x-cx/2),((g_iClientHeight/2)-y-cy/2),cx,cy,10,&cr,TRUE);


	//		m_barRadar.SetAlpha(m_fCurAlpha);
	//		m_barRadar.Render();

		//toolbar:
		DrawRect(pTexToolbar,0,-g_iClientHeight/2+50,g_iClientWidth-16,100-16,8,&cr,TRUE);

		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);

		const wchar_t *pStrNotice = GetResourceString(IDS_PARKING_NOTICE);
		if(pStrNotice)
		{
			SIZE si;
			COLORGL crw={1.0,1.0,1.0,1.0};
			m_pTextGL->SetColor(&crw,FALSE);
			m_pTextGL->SetString(pStrNotice);
			m_pTextGL->SetCharScale(1.0f,1.0f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetAlpha(m_fCurAlpha);
			m_pTextGL->SetLuminance(m_fCurLuminance);
			m_pTextGL->SetPos(-GLfloat(si.cx/2),-g_iClientHeight/2+50+16);
			m_pTextGL->Render();
		}


		const wchar_t *pStrNoSignal = GetResourceString(IDS_NO_SIGNAL);
		if(!bHasVideoSignal && pStrNoSignal && GetTickCount()-m_dwEnterTick>4000)
		{
			SIZE si;
			COLORGL crw={1.0,1.0,1.0,1.0};
			m_pTextGL->SetColor(&crw,FALSE);
			m_pTextGL->SetString(pStrNoSignal);
			m_pTextGL->SetCharScale(1.0f,1.0f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetAlpha(m_fCurAlpha);
			m_pTextGL->SetLuminance(m_fCurLuminance);
			m_pTextGL->SetPos((-((g_iClientWidth/2)-x-cx/2))-GLfloat(si.cx/2),(((g_iClientHeight/2)-y-cy/2))+16);
			m_pTextGL->Render();
		}
		else
		{
	// 		m_iconCamera.SetPos(-((g_iClientWidth/2)-x-cx/2),((g_iClientHeight/2)-y-cy/2));
	// 		m_iconCamera.Render();
		}

	}
	else
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		if(!m_bOnReverseMode && IsReady() && m_pVideoWindow/* && GetTickCount()-m_dwEnterTick>1800*/)
		{
			if(!m_pVideoWindow->IsRunning())
			{
				RETAILMSG(1,(_T("Camera: StartVideo++ iRearStandard[%d]\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard));

				m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_CVBS);
				m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard);	// NTSC
				m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);

				m_pVideoWindow->StartVideo();

				//if(m_pRearOverlay)
				//{
				//	if(!m_pRearOverlay->IsVisible())
				//	{
				//		//RETAILMSG(1, (TEXT("MSG:[%s]: Overlay Show\r\n"), TEXT(__FUNCTION__)));
				//		m_pRearOverlay->Show(TRUE,INFINITE);
				//	}
				//}
				m_dwVideoStartTick = GetTickCount();

				StartRearMonitor();
			}
		}

		if(!m_bHasVideoSignal)
		{
			if(m_pVideoWindow?m_pVideoWindow->IsRunning():TRUE)
			{
				static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
				static CTexture *pTexBar2=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_056):NULL;
				DrawRect(0,0,g_iClientWidth,g_iClientHeight,&crBK);
				DrawRect(pTexBar2,0,(g_iClientHeight/2-30),g_iClientWidth-4,60-4,2,&cr,TRUE);
				DrawRect(pTexBar,0,(-g_iClientHeight/2+30),g_iClientWidth-4,60-4,2,&cr,TRUE);

				m_tagCamera.SetAlpha(m_fCurAlpha);
				m_tagCamera.Render();

				m_tagCameraBox.SetAlpha(m_fCurAlpha);
				m_tagCameraBox.Render();

				m_tagCameraTitle.SetAlpha(m_fCurAlpha);
				m_tagCameraTitle.Render();

				m_btnHome.SetAlpha(m_fCurAlpha);
				m_btnHome.Render();
			}
		}

#else
		if(!m_bOnReverseMode)
		{
	    	m_iconCamera.SetAlpha(m_fCurAlpha);
	    	m_iconCamera.Render();
		}

    	if(!m_bOnReverseMode && IsReady() && m_pVideoWindow/* && GetTickCount()-m_dwEnterTick>1800*/)
    	{
    		if(m_pVideoWindow->IsRunning())
    		{
	 			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
	 			bHasVideoSignal=m_pVideoWindow->HasVideoSignal();

				COLORGL cr2={0.0,0.0,0.0,m_fCurAlpha};
				DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr2);

				const wchar_t *pStrNoSignal = GetResourceString(IDS_NO_SIGNAL);

				if(!bHasVideoSignal && pStrNoSignal && GetTickCount()-m_dwVideoStartTick>4000)
				{
					SIZE si;
					COLORGL crw={1.0,1.0,1.0,1.0};
					m_pTextGL->SetColor(&crw,FALSE);
					m_pTextGL->SetString(pStrNoSignal);
					m_pTextGL->SetCharScale(1.0f,1.0f);
					m_pTextGL->GetExtension(&si);
					m_pTextGL->SetAlpha(m_fCurAlpha);
					m_pTextGL->SetLuminance(m_fCurLuminance);
					m_pTextGL->SetPos(-GLfloat(si.cx/2),16);
					m_pTextGL->Render();
				}
			}
			else
    		{
    			RETAILMSG(1,(_T("Camera: StartVideo++ iRearStandard[%d]\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard));

    			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_CVBS);
    			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard);	// NTSC
    			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);

    			m_pVideoWindow->StartVideo();

#if (CVTE_DEF_CAMERA_WIDGET_TYPE != CVTE_DEF_CAMERA_WIDGET_TYPE_360)
				if(m_pRearOverlay)
				{
					m_pRearOverlay->Show(TRUE,INFINITE);
				}
#endif
                m_dwVideoStartTick = GetTickCount();
    		}
    	}
#endif
	}


// 	m_btnHome.SetAlpha(m_fCurAlpha);
// 	m_btnSetting.SetAlpha(m_fCurAlpha);
//
// 	m_btnHome.Render();
// 	m_btnSetting.Render();

	EndRender();

}
BOOL CPanelRearView::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

// 	COLORGL cr={0.0,1.0,0.0,1.0};
// 	COLORGL cr_r={1.0,0.0,0.0,1.0};
// 	wchar_t caption[8];
// 	SIZE si={400,160};

	m_bShowStatusBar = FALSE;
	m_bEnableSmartBar = TRUE;

#if (CVTE_DEF_UI_STYLE ==  CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagCamera.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_298));
	m_tagCamera.SetShowScale(0.94f);
	m_tagCamera.SetPos(-215,0);

	m_tagCameraBox.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_299));
	m_tagCameraBox.SetShowScale(0.94f);
	m_tagCameraBox.SetPos(115,0);

	m_tagCameraTitle.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_904));
	m_tagCameraTitle.SetShowScale(0.91f);
	m_tagCameraTitle.SetPos(-GLfloat(g_iClientWidth/2)+138,(g_iClientHeight/2-30));

	m_btnHome.Initialize(IDC_REARVIEW_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelRearView::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+90,-g_iClientHeight/2+30);
	m_btnHome.SetTouchRate(1.6f,1.6f);
	m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	//m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
#else
	m_iconCamera.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_368));
	m_iconCamera.SetShowScale(2.0f);
#endif
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
	m_btnHome.SetNotifyFunc(&CPanelRearView::OnNotify,this);
	m_btnHome.SetSize(&si1);
	m_btnHome.SetPos(-330,-200);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);

	m_btnSetting.Initialize(IDC_REARVIEW_BTN_SETTING,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
		m_pSkinManager->GetTexture(TEXID_056),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSetting.SetNotifyFunc(&CPanelRearView::OnNotify,this);
	m_btnSetting.SetSize(&si1);
	m_btnSetting.SetPos(330,-200);
	m_btnSetting.SetIcon(&m_iconSettingUp,&m_iconSettingDown);
*/

//		m_barRadar.Initialize(0,this,pSkinManager,pTextGL,hWnd,entry);
//		m_barRadar.SetPos(410,50);
//		m_barRadar.SetShowScale(0.6f);

	//test:
// 	for (int i=0;i<8;i++)
// 	{
// 		m_barRadar.SetRadarSingal(i,i);
// 	}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	InitRearMonitor();
#endif
	return TRUE;
}

void CPanelRearView::OnReverseModeChange(BOOL bReverse)
{
	RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnReverseModeChange]: %d\r\n"),bReverse));

	CPanelGL::OnReverseModeChange(bReverse);

	if(bReverse && m_pVideoWindow && m_pVideoWindow->IsRunning())
	{
		RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnReverseModeChange]: StopVideo\r\n")));
		m_pVideoWindow->SetGestureFunc(NULL,NULL);
		m_pVideoWindow->StopVideo();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		StopRearMonitor();
#endif
		
#if (CVTE_DEF_CAMERA_WIDGET_TYPE != CVTE_DEF_CAMERA_WIDGET_TYPE_360)
		if(m_pRearOverlay)
		{
			if(m_pRearOverlay->IsVisible())
			{
				//RETAILMSG(1, (TEXT("MSG:[%s]: Overlay Hide\r\n"), TEXT(__FUNCTION__)));
				m_pRearOverlay->Show(FALSE,INFINITE);
			}
		}
#endif
//			if(m_pCommander)
//			{
//				m_pCommander->SendCmd(_ARM_CAMERA_POWER_KEY,CAMERA_POWER_OFF);
//			}
	}
	else if(!bReverse && m_pVideoWindow && (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN)))
	{
		if(m_pVideoWindow->IsRunning())
		{
			RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnReverseModeChange]: Stop and Restart video again\r\n")));
    		m_pVideoWindow->SetGestureFunc(NULL,NULL);
    		m_pVideoWindow->StopVideo();
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)	
			StopRearMonitor();
#endif			

			if(m_pCommander)
			{
				m_pCommander->SendCmd(_ARM_CAMERA_POWER_KEY,CAMERA_POWER_ON);
			}

#if (CVTE_DEF_CAMERA_WIDGET_TYPE != CVTE_DEF_CAMERA_WIDGET_TYPE_360)
			if(m_pRearOverlay)
			{
				if(m_pRearOverlay->IsVisible())
				{
					//RETAILMSG(1, (TEXT("MSG:[%s]: Overlay Hide\r\n"), TEXT(__FUNCTION__)));
					m_pRearOverlay->Show(FALSE,INFINITE);
				}
			}
#endif

			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_CVBS);
			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard);	// NTSC
			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);

			m_pVideoWindow->StartVideo();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
			StartRearMonitor();
#else

#if (CVTE_DEF_CAMERA_WIDGET_TYPE != CVTE_DEF_CAMERA_WIDGET_TYPE_360)
			if(m_pRearOverlay)
			{
				if(!m_pRearOverlay->IsVisible())
				{
					//RETAILMSG(1, (TEXT("MSG:[%s]: Overlay Show\r\n"), TEXT(__FUNCTION__)));
					m_pRearOverlay->Show(TRUE,INFINITE);
				}
			}
#endif
#endif
			m_dwVideoStartTick = GetTickCount();
		}
		else
		{
			RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnReverseModeChange]: Restart video again\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iDVDStandard));
			if(m_pCommander)
			{
				m_pCommander->SendCmd(_ARM_CAMERA_POWER_KEY,CAMERA_POWER_ON);
			}

			m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_CVBS);
			m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard);	// NTSC
			m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
			//m_pVideoWindow->SetGestureFunc(&CPanelRearView::OnVideoWndGesture,this);

			m_pVideoWindow->StartVideo();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
			StartRearMonitor();
#else
#if (CVTE_DEF_CAMERA_WIDGET_TYPE != CVTE_DEF_CAMERA_WIDGET_TYPE_360)
			if(m_pRearOverlay)
			{
				if(!m_pRearOverlay->IsVisible())
				{
					//RETAILMSG(1, (TEXT("MSG:[%s]: Overlay Show\r\n"), TEXT(__FUNCTION__)));
					m_pRearOverlay->Show(TRUE,INFINITE);
				}
			}
#endif
#endif
			m_dwVideoStartTick = GetTickCount();
		}
	}
	else
	{
		RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnReverseModeChange]: bReverse[%d] IsRunning[%d] idCurSRC[0x%x]\r\n"),bReverse,m_pVideoWindow->IsRunning(),m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC));
	}
}
void CPanelRearView::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelRearView::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelRearView::OnTouchMove(POINT *pt)
{

}

void CPanelRearView::OnRearViewKey(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnRearViewKey]: CPanelRearView wParam:%d\r\n"),wParam));
	int cmd = (int)wParam;
	switch(cmd)
	{
		case KEY_REARVIEW_HOME:
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_ZOOMIN);
			break;
	}
}

void CALLBACK CPanelRearView::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnRearViewKey]: OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelRearView *panel=(CPanelRearView*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelRearView::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_REARVIEW_BTN_HOME:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	case IDC_REARVIEW_BTN_SETTING:
		break;
	}
}
void CPanelRearView::OnBack()
{
	//quit
	SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);

	RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::OnBack]: m_idPreviousPanel:0x%x\r\n"),m_idPreviousPanel));

}
void CPanelRearView::OnCanbusStatus(WPARAM wParam, LPARAM lParam)
{
// 	if(wParam)
// 	{
// 		m_bHasRadar = TRUE;
// 	}
// 	else
// 	{
// 		m_bHasRadar = FALSE;
// 	}
}
void CPanelRearView::OnRadarStatus(WPARAM wParam, LPARAM lParam)
{
	m_barRadar.SetRadarSingal(wParam,lParam);

	if(!m_bHasRadar)
	{
		m_bHasRadar = TRUE;

		if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_REARVIEW,0))
		{
			if(m_pVideoWindow && m_pVideoWindow->IsRunning())
			{
				m_pVideoWindow->SetGestureFunc(NULL,NULL);
				m_pVideoWindow->StopVideo();
			}
		}
	}
}
void CPanelRearView::StartCAMVideo()
{
	if(!m_bCAMVideoStarted)
	{
		RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::StartCAMVideo]\r\n")));

		m_bCAMVideoStarted = TRUE;

        /*
		//
		HANDLE hDevTW8836 = CreateFile(TEXT("TWD1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);

		if(hDevTW8836 != INVALID_HANDLE_VALUE)
		{
			TW8836INPUTCHANNEL input;

			switch(m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType)
			{
				case 0:	// CVBS NTSC
					input  = TW8836_INPUT_CHANNEL_CVBS_NTSC;
					break;
				case 1:	// CVBS PAL
					input  = TW8836_INPUT_CHANNEL_CVBS_PAL;
					break;
				case 2:	// ARGB
					input = TW8836_INPUT_CHANNEL_ARGB;
					break;
				default:
					input = TW8836_INPUT_CHANNEL_CVBS_NTSC;
					break;
			}

			if (!DeviceIoControl(hDevTW8836, TW8836_IOCTL_ACTIVATE_CHANNEL, &input, sizeof(input),	NULL, 0, NULL, NULL))
			{
				RETAILMSG(DEBUG_REARVIEW, (TEXT("APP:ERROR:GUI:RearView: Failed DeviceIoControl IOCTL_BACKLIGHT_SET -- error = 0x%x\r\n"), GetLastError()));
			}
			else
			{
			}

			CloseHandle(hDevTW8836);
		}
		*/
	}
}
void CPanelRearView::StopCAMVideo()
{
	if(m_bCAMVideoStarted)
	{
		RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::StopCAMVideo]\r\n")));

		m_bCAMVideoStarted = FALSE;
        /*
		HANDLE hDevTW8836 = CreateFile(TEXT("TWD1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);

		if(hDevTW8836 != INVALID_HANDLE_VALUE)
		{
			TW8836INPUTCHANNEL input = TW8836_INPUT_CHANNEL_RGB888;

			if (!DeviceIoControl(hDevTW8836, TW8836_IOCTL_ACTIVATE_CHANNEL, &input, sizeof(input),	NULL, 0, NULL, NULL))
			{
				RETAILMSG(DEBUG_REARVIEW, (TEXT("APP:ERROR:GUI:RearView: Failed DeviceIoControl TW8836_IOCTL_ACTIVATE_CHANNEL -- error = 0x%x\r\n"), GetLastError()));
			}
			else
			{
			}

			CloseHandle(hDevTW8836);
		}
		*/
	}
}

void CPanelRearView::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	static BOOL bPointTouchDown = FALSE;
	int nPointStatus = pCyMultiPoint->sPointStatus.pointstatus;
	int nPointTouchArea = pCyMultiPoint->sPointStatus.toucharea;
	//RETAILMSG(1, (TEXT("MSG:[%s]:nPointStatus[%X] nPointTouchArea[%X]\r\n"),TEXT(__FUNCTION__),nPointStatus,nPointTouchArea));

	if((nPointTouchArea != TOUCH_AREA_GFX_WINDOW && (nPointStatus != TOUCH_STATUS_UP  && nPointStatus != GEST_SINGLE_TOUCH_END && nPointStatus != GEST_SINGLE_TOUCH_DOUBLE_CLICK)))
	{
		return;
	}

	if(nPointStatus == TOUCH_STATUS_UP || nPointStatus == GEST_SINGLE_TOUCH_END || nPointStatus == GEST_SINGLE_TOUCH_DOUBLE_CLICK)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:Up\r\n"),TEXT(__FUNCTION__)));
		//UP
		bPointTouchDown = FALSE;
		m_ptPoint1.x = pCyMultiPoint->sPointCoordinate[0].PointX;
		m_ptPoint1.y = pCyMultiPoint->sPointCoordinate[0].PointY;
		SendPointCmd(&m_ptPoint1, FALSE);
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

void CPanelRearView::SendPointCmd(POINT *pt, BOOL bDown)
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
			RETAILMSG(1,(_T("MSG:[%s]: Down x:%d,y:%d\r\n"),TEXT(__FUNCTION__), x, y));
			m_pCommander->SendCmd(_ARM_TV_X_KEY,x);
			m_pCommander->SendCmd(_ARM_TV_Y_KEY,y);
		}
		else
		{
			RETAILMSG(1,(_T("MSG:[%s]: Up x:%d,y:%d\r\n"),TEXT(__FUNCTION__), x, y));
			m_pCommander->SendCmd(_ARM_TV_X_KEY_UP,x);
			m_pCommander->SendCmd(_ARM_TV_Y_KEY_UP,y);
		}
	}
}

void CPanelRearView::SwitchTW8836Channel(BYTE uChannel)
{
	RETAILMSG(DEBUG_REARVIEW,(_T("MSG:[CPanelRearView::SwitchTW8836Channel]: uChannel:%d \r\n"),uChannel));
/*
	HANDLE hDevTW8836 = CreateFile(TEXT("TWD1:"), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, 0, 0);

	if(hDevTW8836 != INVALID_HANDLE_VALUE)
	{
		TW8836INPUTCHANNEL input = (TW8836INPUTCHANNEL)uChannel;
		if (!DeviceIoControl(hDevTW8836, TW8836_IOCTL_ACTIVATE_CHANNEL, &input, sizeof(input),	NULL, 0, NULL, NULL))
		{
			RETAILMSG(DEBUG_REARVIEW, (TEXT("APP:ERROR:GUI:Rearview:  SwitchTW8836Channel IoCtl TW8836_IOCTL_ACTIVATE_CHANNEL uChannel[%d] -- error[0x%x]\r\n"),input, GetLastError()));
		}

		CloseHandle(hDevTW8836);
	}
*/
}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
BOOL CPanelRearView::InitRearMonitor()
{
	m_hRearMonitorStartEvt = CreateEvent(0, FALSE, FALSE, NULL);
	if(m_hRearMonitorStartEvt == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent [m_hRearMonitorStartEvt] failed!!!\r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}
	RETAILMSG(1, (TEXT("MSG:[%s]: CreateEvent [m_hRearMonitorStartEvt] \r\n"), TEXT(__FUNCTION__)));

	m_hRearMonitorEndEvt = CreateEvent(0, FALSE, FALSE, NULL);
	if(m_hRearMonitorEndEvt == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent [m_hRearMonitorEndEvt] failed!!!\r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}
	RETAILMSG(1, (TEXT("MSG:[%s]: CreateEvent [m_hRearMonitorEndEvt] \r\n"), TEXT(__FUNCTION__)));

	m_hRearMonitorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RearMonitorThreadPro, this, 0, NULL);
	if(m_hRearMonitorThread == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateThread [m_hRearMonitorThread] failed!!!\r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}
	RETAILMSG(1, (TEXT("MSG:[%s]: CreateThread [m_hRearMonitorThread] \r\n"), TEXT(__FUNCTION__)));

	return TRUE;
}

void CPanelRearView::StartRearMonitor()
{
	if(!m_bRearMonitorWorking && m_hRearMonitorStartEvt)
	{
		SetEvent(m_hRearMonitorStartEvt);
	}

	return;
}

void CPanelRearView::StopRearMonitor()
{
	if(m_bRearMonitorWorking && m_hRearMonitorEndEvt)
	{
		SetEvent(m_hRearMonitorEndEvt);
	}
}


DWORD CPanelRearView::RearMonitorThreadPro(LPVOID lpParam)
{
	CPanelRearView *pDevice = (CPanelRearView*)lpParam;
	DWORD			dwRet;

	if(pDevice == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Param check failed.\r\n"), TEXT(__FUNCTION__)));
		return -1;
	}

	RETAILMSG(1, (TEXT("MSG:[%s]: ++\r\n"), TEXT(__FUNCTION__)));

	do 
	{
		dwRet = WaitForSingleObject(pDevice->m_hRearMonitorStartEvt, INFINITE);
		if(dwRet != WAIT_OBJECT_0)
		{
			RETAILMSG(1, (TEXT("MSG:[%s]: WaitForSingleObject[StartEvt] failed!!! \r\n"), TEXT(__FUNCTION__)));
			continue;
		}

		pDevice->m_bRearMonitorWorking = TRUE;
		RETAILMSG(1, (TEXT("MSG:[%s]: START \r\n"), TEXT(__FUNCTION__)));

		while(pDevice->m_bRearMonitorWorking)
		{
			dwRet = WaitForSingleObject(pDevice->m_hRearMonitorEndEvt, 700);
			if(dwRet == WAIT_OBJECT_0)
			{
				pDevice->m_bRearMonitorWorking = FALSE;
				RETAILMSG(1, (TEXT("MSG:[%s]: STOP \r\n"), TEXT(__FUNCTION__)));
				break;
			}
			else    
			{
				pDevice->RearMonitorRun();
			}

		}
	} while (1);

	RETAILMSG(1, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
	return 0;
}

void CPanelRearView::RearMonitorRun() 
{
	if(!m_bOnReverseMode && IsReady() && m_pVideoWindow/* && GetTickCount()-m_dwEnterTick>1800*/)
	{
		if(m_pVideoWindow->IsRunning())
		{
			//m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
			m_bHasVideoSignal=m_pVideoWindow->HasVideoSignal();

			if(m_bHasVideoSignal)
			{
				if(m_pRearOverlay)
				{
					if(!m_pRearOverlay->IsVisible())
					{
						//RETAILMSG(1, (TEXT("MSG:[%s]: Overlay Show\r\n"), TEXT(__FUNCTION__)));
						m_pRearOverlay->Show(TRUE,INFINITE);
					}
				}
			}
			else
			{
				if(m_pRearOverlay)
				{
					if(m_pRearOverlay->IsVisible())
					{
						//RETAILMSG(1, (TEXT("MSG:[%s]: Overlay Hide\r\n"), TEXT(__FUNCTION__)));
						m_pRearOverlay->Show(FALSE,INFINITE);
					}
				}
			}

		}
		//else
		//{
		//	RETAILMSG(1,(_T("Camera: StartVideo++ iRearStandard[%d]\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard));

		//	m_pVideoWindow->SetVideoChannel(VIP_CHANNEL_CVBS);
		//	m_pVideoWindow->SetColorSystem((COLOR_SYSTEM)m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard);	// NTSC
		//	m_pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);

		//	m_pVideoWindow->StartVideo();

		//	//#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_BOSSAUDIOUI)
		//	//if(m_pRearOverlay)
		//	//{
		//	//	m_pRearOverlay->Show(TRUE,INFINITE);
		//	//}
		//	//#endif
		//	m_dwVideoStartTick = GetTickCount();
		//}
	}
}
#endif

#endif