#include "PanelRadioMain.h"

#include "../../resource.h"

#define ENABLE_SAVE_FREQUENCY		0
#define ENABLE_HOME_BTN_CLOSE_RADIO		0

extern const wchar_t* g_strInstallPath;

static RadioRegionParam g_RadioRegionParam[] =
{
	//iRegionID
	//dwFMFrepMin	//dwFMFrepMax	//iFMStep
	//dwAMFrepMin	//dwAMFrepMax	//iAMStep
	{
		//欧洲
		CVTE_DEF_RADIO_REGION_EUROPE,
		CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN,		CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX,		CVTE_DEF_RADIO_EUROPE_FM_STEP,
		CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN,		CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX,		CVTE_DEF_RADIO_EUROPE_AM_STEP
	},

	{
		//U.S.A 1
		CVTE_DEF_RADIO_REGION_USA,
		CVTE_DEF_RADIO_USA_FM_FREQ_MIN,		CVTE_DEF_RADIO_USA_FM_FREQ_MAX,		CVTE_DEF_RADIO_USA_FM_STEP,
		CVTE_DEF_RADIO_USA_AM_FREQ_MIN,		CVTE_DEF_RADIO_USA_AM_FREQ_MAX,		CVTE_DEF_RADIO_USA_AM_STEP
	},

	{
		//东欧
		CVTE_DEF_RADIO_REGION_EAST_ERNEUROPE,
		CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MIN,		CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MAX,		CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_STEP,
		CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MIN,		CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MAX,		CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_STEP
	},

	{
		//日本
		CVTE_DEF_RADIO_REGION_JAPAN,
		CVTE_DEF_RADIO_JAPAN_FM_FREQ_MIN,		CVTE_DEF_RADIO_JAPAN_FM_FREQ_MAX,		CVTE_DEF_RADIO_JAPAN_FM_STEP,
		CVTE_DEF_RADIO_JAPAN_AM_FREQ_MIN,		CVTE_DEF_RADIO_JAPAN_AM_FREQ_MAX,		CVTE_DEF_RADIO_JAPAN_AM_STEP
	},

	{
		//欧洲、东欧 OIRT
		CVTE_DEF_RADIO_REGION_EUROPE_EASTERNEUROPE,
		CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN,		CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX,		CVTE_DEF_RADIO_EUROPE_FM_STEP,
		CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN,		CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX,		CVTE_DEF_RADIO_EUROPE_AM_STEP
	},

	{
		//韩国
		CVTE_DEF_RADIO_REGION_KOREAN,
		CVTE_DEF_RADIO_KOREAN_FM_FREQ_MIN,		CVTE_DEF_RADIO_KOREAN_FM_FREQ_MAX,		CVTE_DEF_RADIO_KOREAN_FM_STEP,
		CVTE_DEF_RADIO_KOREAN_AM_FREQ_MIN,		CVTE_DEF_RADIO_KOREAN_AM_FREQ_MAX,		CVTE_DEF_RADIO_KOREAN_AM_STEP
	},

	{
		//台湾
		CVTE_DEF_RADIO_REGION_TAIWANG,
		CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MIN,		CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MAX,		CVTE_DEF_RADIO_TAIWANG_FM_STEP,
		CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MIN,		CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MAX,		CVTE_DEF_RADIO_TAIWANG_AM_STEP
	},

	{
		//U.S.A 2
		CVTE_DEF_RADIO_REGION_USA2,
		CVTE_DEF_RADIO_USA2_FM_FREQ_MIN,		CVTE_DEF_RADIO_USA2_FM_FREQ_MAX,		CVTE_DEF_RADIO_USA2_FM_STEP,
		CVTE_DEF_RADIO_USA2_AM_FREQ_MIN,		CVTE_DEF_RADIO_USA2_AM_FREQ_MAX,		CVTE_DEF_RADIO_USA2_AM_STEP
	},

	{
		//南美
		CVTE_DEF_RADIO_REGION_SOUTHAMERICA,
		CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MIN,		CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MAX,		CVTE_DEF_RADIO_SOUTHAMERICA_FM_STEP,
		CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MIN,		CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MAX,		CVTE_DEF_RADIO_SOUTHAMERICA_AM_STEP
	},

	{
		//ARABIA AUSTRALIA
		CVTE_DEF_RADIO_REGION_ARABIA_AUSTRALIA,
		CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MIN,		CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX,		CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_STEP,
		CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MIN,		CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MAX,		CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_STEP
	},

	{
		//LATIN AMERICA
		CVTE_DEF_RADIO_REGION_LATIN_AMERICA,
		CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MIN,		CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX,		CVTE_DEF_RADIO_LATIN_AMERICA_FM_STEP,
		CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MIN,		CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MAX,		CVTE_DEF_RADIO_LATIN_AMERICA_AM_STEP
	},

	{
		//中东
		CVTE_DEF_RADIO_REGION_MIDDLE_EAST,
		CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MIN,		CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX,		CVTE_DEF_RADIO_MIDDLE_EAST_FM_STEP,
		CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MIN,		CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MAX,		CVTE_DEF_RADIO_MIDDLE_EAST_AM_STEP
	},

	{
		//AUST
		CVTE_DEF_RADIO_REGION_AUST,
		CVTE_DEF_RADIO_AUST_FM_FREQ_MIN,		CVTE_DEF_RADIO_AUST_FM_FREQ_MAX,		CVTE_DEF_RADIO_AUST_FM_STEP,
		CVTE_DEF_RADIO_AUST_AM_FREQ_MIN,		CVTE_DEF_RADIO_AUST_AM_FREQ_MAX,		CVTE_DEF_RADIO_AUST_AM_STEP
	},

	{
		//巴西
		CVTE_DEF_RADIO_REGION_BRAZIL,
		CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MIN,		CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX,		CVTE_DEF_RADIO_BRAZIL_FM_STEP,
		CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MIN,		CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MAX,		CVTE_DEF_RADIO_BRAZIL_AM_STEP
	},

	//{
	//	//俄罗斯
	//	CVTE_DEF_RADIO_REGION_RUSSIA,
	//	CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MIN,		CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MAX,		CVTE_DEF_RADIO_RUSSIA_FM_STEP,
	//	CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MIN,		CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MAX,		CVTE_DEF_RADIO_RUSSIA_AM_STEP
	//},
};

//0:Europe:				fm 87.5-108.0MHz, am 522-1620KHz
//1:USA:					fm 87.5-107.9MHz, am 530-1710KHz
//2:EasternEurope:	fm 65.00-74.0MHz, am 522-1620KHz
//3:Japan:					fm 76.0-90.0MHz, am 522-1629KHz
//4:Euro&EastEuro
//5:Korea:					fm 87.5-108.0MHz, am 531-1629KHz
//6:Row:					fm 87.5-108.0MHz, am 522-1620KHz
//7:South America           fm 87.5-108.0MHz, am 520-1600KHz
//radio main
CPanelRadioMain::CPanelRadioMain(void)
{
	m_RegionId = (RadioRegionId)CVTE_DEF_RADIO_REGION_EUROPE;
	m_BandId = RADIO_BAND_ID_INVALID;

	m_dwFreqFM = CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN;	//87500000;
	m_dwFreqAM = CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN;	//522000;

	m_dwFreqMinFM =CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN;		// 87500000;
	m_dwFreqMaxFM = CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX;	//108000000;
	m_dwFreqMinAM = CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN;	//522000;
	m_dwFreqMaxAM = CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX;	//1620000;

	m_fScaleOffsetCur = 0;
	m_fScaleOffsetSet = 0;
	m_fScaleOffsetBeforeDraging = 0;
	m_bScaleMoving = FALSE;
	m_bScaleDraging = FALSE;
	m_bEnableScaleDraging = FALSE;

	m_dwLastPressTick=0;
	m_dwLastScaleDragTick = 0;
	m_fScaleOffsetLimitHigh = -50;
	m_fScaleOffsetLimitLow = -50*42;

	m_iScaleCount = 60;
	m_iScaleMargin = 8;

	m_dwCurFreq = 0;
	m_dwFreqChangeTick = 0;

	m_bCheckFavorite = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_iStepFM = 20;
	m_iStepAM = 10;
#else
	m_iStepFM = 5;
	m_iStepAM = 9;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_hidearea = 60;
#else
	m_hidearea = 0;
#endif

	m_bSearching = FALSE;

	m_pSmartBarMedia = NULL;
	m_bShowQuickFav = FALSE;
	m_bFavListChanged = FALSE;

	for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
	{
		memset(&m_FMFavList[i],0,sizeof(RadioStationInfo));
		memset(&m_AMFavList[i],0,sizeof(RadioStationInfo));

		m_FMFavList[i].dwBand=RADIO_BAND_ID_FM;
		m_AMFavList[i].dwBand=RADIO_BAND_ID_AM;
	}

	m_bAutoShowList = FALSE;
	m_dwLastSearchTick = 0;

	m_dwShowQuickFavTick = 0;
	//m_bRDSOn = FALSE;
	m_bEnableUpdateWidget = TRUE;
	m_bOnTAMode = FALSE;
	m_bShowPSForCurrentFreq = FALSE;
	m_bRdsListChanged = FALSE;
	m_bAFSearching = FALSE;

	m_dwRadioTextUpdateTick = 0;
	m_iRadioTextShowOffset = 0;
	m_bShowingLongRadioText = FALSE;
	m_iRadioTextUpdateInterval = 200;

	m_strRadioText=_T("");
	m_dwCurPICode = 0;
	m_dwCurPICodeFreq = 0;
	m_idPTY = PT_Invalid;
	m_dwAFSearchingTick = 0;

#if ENABLE_RADIO_DEBUG_SCREEN
	m_iDebugClickRef = 0;
	m_bEnbaleDebugScreen = FALSE;
	memset(&m_DebugAFData,0,sizeof(AFData));
	m_DebugPIData = 0;
	for (int i=0;i<256;i++)
	{
		memset(&m_DebugRadioSignalData[i],0,sizeof(SIGNAL_DATA));
	}
#endif
}
CPanelRadioMain::~CPanelRadioMain(void)
{

}
void CPanelRadioMain::OnTouchDown(POINT *pt)
{
	if(m_bOnTAMode)
	{
		m_btnCancelTA.OnTouchDown(pt);
		return;
	}

#if ENABLE_RADIO_DEBUG_SCREEN
	//enable debug mode:
	if(pt->x<60 && pt->y<60)
	{
		m_iDebugClickRef++;
		RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain:DebugClickRef: %d\r\n"),m_iDebugClickRef));
		if(m_iDebugClickRef >= 10)
		{
			m_bEnbaleDebugScreen = TRUE;
			SendRadioCommand(RADIO_CMD_ENABLE_DEBUG_SCREEN, TRUE);
		}
	}
	else
	{
		m_iDebugClickRef=0;
	}
#endif

	CPanelGL::OnTouchDown(pt);

	m_dwLastPressTick=GetTickCount();

	if(m_bShowQuickFav)
		return;

	m_bEnableScaleDraging = FALSE;
	m_bScaleDraging = FALSE;

	m_ptLastPress = *pt;
	m_fScaleOffsetBeforeDraging = m_fScaleOffsetCur;

	if(pt->y>g_iScreenHeight-120 && pt->x>(160+m_hidearea) && pt->x<(g_iScreenWidth-160-m_hidearea))
	{
		m_bEnableScaleDraging = TRUE;
	}
}
void CPanelRadioMain::OnTouchUp(POINT *pt)
{
	if(m_bOnTAMode)
	{
		m_btnCancelTA.OnTouchUp(pt);
		if(m_btnCancelTA.IsPtnOnObject(pt))
		{
			SendRadioCommand(RADIO_CMD_CANCEL_TA,0);
		}
		return;
	}

	CPanelGL::OnTouchUp(pt);

	if(m_bShowQuickFav)
		return;

	if(m_bEnableScaleDraging)
	{
		if(!m_bScaleDraging)
		{
			if(GetTickCount() - m_dwLastScaleDragTick >500)
			{
				GLfloat offset = m_fScaleOffsetBeforeDraging - ( pt->x - g_iClientWidth/2);
				//reset:
				if(offset>m_fScaleOffsetLimitHigh)
				{
					offset=m_fScaleOffsetLimitLow;//m_fScaleOffsetLimitHigh;
				}
				if(offset<m_fScaleOffsetLimitLow)
				{
					offset=m_fScaleOffsetLimitHigh;//m_fScaleOffsetLimitLow;
				}
                //RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain:SetScaleOffset 1\r\n")));
				SetScaleOffset(offset);
			}
		}
		else
		{
			for (int i=0;i<m_iScaleCount;i++)
			{
				m_tagScale[i].SetStatus(BS_UP);
			}

			//reset:
			if(m_fScaleOffsetSet>m_fScaleOffsetLimitHigh)
			{
				m_fScaleOffsetSet=m_fScaleOffsetLimitHigh;
			}
			if(m_fScaleOffsetSet<m_fScaleOffsetLimitLow)
			{
				m_fScaleOffsetSet=m_fScaleOffsetLimitLow;
			}
            //RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain:SetScaleOffset 2\r\n")));
			SetScaleOffset(m_fScaleOffsetSet);
		}

		//update freq:
		ApplyCurrentScaleFreq();
	}


	m_bScaleDraging = FALSE;
	m_bEnableScaleDraging = FALSE;

}
void CPanelRadioMain::OnTouchMove(POINT *pt)
{
	if(m_bOnTAMode)
	{
		m_btnCancelTA.OnTouchMove(pt);
		return;
	}

	CPanelGL::OnTouchMove(pt);

	if(m_bShowQuickFav)
		return;

	if(m_bEnableScaleDraging)
	{
		BOOL bDraging=FALSE;
		if((GetTickCount()-m_dwLastPressTick>200) || abs(pt->x - m_ptLastPress.x)>30)
		{
			bDraging = TRUE;
		}
		if(bDraging && !m_bScaleDraging)
		{
			m_bScaleDraging = TRUE;
			for (int i=0;i<m_iScaleCount;i++)
			{
				m_tagScale[i].SetStatus(BS_FOCUS);
			}
		}
	}

	if(m_bScaleDraging)
	{
		m_dwLastScaleDragTick = GetTickCount();
        //RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain:SetScaleOffset 3\r\n")));
		SetScaleOffset(m_fScaleOffsetBeforeDraging + pt->x - m_ptLastPress.x);
	}
}
void CPanelRadioMain::OnGesture(GESTUREINFO *pgi)
{
	if(!pgi)
		return;

	if(m_bOnTAMode)
		return;

	POINT pt={pgi->ptsLocation.x,pgi->ptsLocation.y};

	// Handle gesture indicated by wParam or gi.dwID
	switch (pgi->dwID)
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
		if(pt.y<400)
		{
			LONG velocity = GID_SCROLL_VELOCITY (pgi->ullArguments );
			LONG angle =   GID_SCROLL_ANGLE (pgi->ullArguments );
			LONG direction = GID_SCROLL_DIRECTION (pgi->ullArguments );
			if(velocity>1000)
			{
				switch (direction )
				{
				case ARG_SCROLL_DOWN:
					break;
				case ARG_SCROLL_UP:
					break;
				case ARG_SCROLL_LEFT:
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
					SendRadioCommand(RADIO_CMD_PLAY_PREVIOUS,0);
#else
					SendRadioCommand(RADIO_CMD_PLAY_NEXT,0);
#endif
					PlaySoundEffect(0);
					break;
				case ARG_SCROLL_RIGHT:
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
					SendRadioCommand(RADIO_CMD_PLAY_NEXT,0);
#else
					SendRadioCommand(RADIO_CMD_PLAY_PREVIOUS,0);
#endif
					PlaySoundEffect(0);
					break;
				}
			}
		}
		break;
	case GID_HOLD:
		break;
	case GID_DOUBLESELECT:
		break;
	case GID_SELECT:
		break;
	}
}

void CPanelRadioMain::OnGestureMsg(UINT idGesture)
{
	if(m_bOnTAMode)
		return;

	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_LIBRARY,PAGE_SWITCH_ANI_LEFT);
		break;
	}
}
BOOL CPanelRadioMain::IsReady()
{
	return CPanelGL::IsReady() && m_btnQuickFav[0].IsReady() && m_btnQuickFav[MAX_QUICK_FAV_CNT-1].IsReady();//&& (!m_bScaleMoving) && (!m_bSearching);

}
void CPanelRadioMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	//
	StopScaleAni();

	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	if(m_bShowQuickFav)
	{
		ShowQuickFav(FALSE,FALSE);
	}
}
void CPanelRadioMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

	EnableUpdateWidget(TRUE);

	//Switch audio SRC:
	if(GetAudioSRC() != SRC_AUDIO_RADIO)
	{
	    SendRadioCommand(RADIO_CMD_SWITCH_AUDIO_SRC,0);
        RETAILMSG(1, (TEXT("MSG:[CPanelRadioMain::OnEnterPanel]: Switch to Radio audio SRC \r\n")));
	}
	//play:
	SendRadioCommand(RADIO_CMD_SET_PLAYSTATE,0);

	SendRadioCommand(RADIO_CMD_ENTER_MODE,HIWORD(iParam));

	SendRadioCommand(RADIO_CMD_QUERY_FAVORITE_STATUS,m_dwCurFreq);//[BUG] 0002296 fix bug that current radio index invisible
	//notify src
	::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_RADIO,0x01);

	//
	if(m_pSmartBarMedia && HIWORD(iParam)==0)//if quick mode,do not update smart bar
	{
		m_pSmartBarMedia->SetType(SB_MEDIATYPE_RADIO);
		m_pSmartBarMedia->SetTrackName(m_tagCurFrenquency.GetCaption());
		m_pSmartBarMedia->SetNowPlayingTitle(m_tagNowPlaying.GetCaption());
		m_pSmartBarMedia->SetPlayingFlag(TRUE);
	}

	//update widget:
	//update ps:
	if(m_pSkinManager)
	{
		UINT idWidgetType=0;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDM_RADIO_WIDGET_TYPE,UIOPRT_GETVALUE,(LPVOID)(&idWidgetType));

		RETAILMSG(1, (TEXT("MSG:[CPanelRadioMain::OnEnterPanel]: idWidgetType=%d \r\n"),idWidgetType));

		if(idWidgetType != UI_CLASS_RADIO)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDM_RADIO_WIDGET_TYPE,UIOPRT_SETVALUE,(LPVOID)UI_CLASS_RADIO);
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagCurFrenquency.GetCaption());
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,m_BandId==RADIO_BAND_ID_FM?(LPVOID)GetResourceString(IDS_MHZ):(LPVOID)GetResourceString(IDS_KHZ));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI) || (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_PS_INFO,UIOPRT_SETSTRING,(LPVOID)m_btnCurPS.GetCaption());
#endif
		}
	}

	//update play info:
	SetTimer(m_hMainWnd,TIMER_RADIO_UPDATE_PLAYINFO,500,NULL);
	m_bCheckFavorite = TRUE;
}
void CPanelRadioMain::Render()
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_229):NULL;
#else
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
#endif
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	int iFrameTime = 500;

	if(!BeginRender())return;

	GLfloat fbackLumin=m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance;

	//
	if(m_bAutoShowList)
	{
		if(GetTickCount() - m_dwLastSearchTick >200)
		{
			UINT type=0;
			if(m_BandId==RADIO_BAND_ID_FM)
				type=RADIO_LIBRARY_FM;
			else if(m_BandId==RADIO_BAND_ID_AM)
				type=RADIO_LIBRARY_AM;

			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_LIBRARY,PAGE_SWITCH_ANI_DEFAULT,type);

			m_bAutoShowList = FALSE;
		}
	}

	//
	if(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect)
	{
		m_tagBkBlur.SetAlpha(m_fCurAlpha);
		m_tagBkBlur.SetLuminance(m_fCurLuminance*fbackLumin);
		m_tagBkBlur.Render();
	}
	else
	{
		m_tagBkOrigin.SetAlpha(m_fCurAlpha);
		m_tagBkOrigin.SetLuminance(m_fCurLuminance*fbackLumin);
		m_tagBkOrigin.Render();
	}

// 	m_tagBar.SetAlpha(m_fCurAlpha);
// 	m_tagBar.Render();

	//
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBackEq.SetAlpha(m_fCurAlpha);
	m_tagBackEq.Render();
#else
	VIEW_STATE vs=m_tagBackEq.GetCurViewState();
	vs.fRotateX=0;
	vs.fTransY=55;
	vs.fTransX=-253;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.3f);
	m_tagBackEq.Render();
	vs.fTransX=254;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.Render();
	vs.fRotateX=-180;
	vs.fTransY=-55;
	vs.fTransX=-253;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.2f);
	m_tagBackEq.Render();
	vs.fTransX=254;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.Render();
#endif


	m_btnRadioRegion.Show(!m_bShowQuickFav && m_BandId==RADIO_BAND_ID_FM
		&& (m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_Euro_EastEuro || m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_EastEuro_Euro));

	if(!m_bShowQuickFav)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)		
		DrawRect(pTexBar,0,(-g_iClientHeight/2+48),800,80,2,&cr,TRUE);
#else
		DrawRect(pTexBar,0,(-g_iClientHeight/2+40),g_iClientWidth-4,80,2,&cr,TRUE);
#endif
		//
		m_tagSTMono.SetAlpha(m_fCurAlpha);
		m_tagSTMono.Render();
		m_tagLOC.SetAlpha(m_fCurAlpha);
		m_tagLOC.Render();
	// 	m_tagTitle.SetAlpha(m_fCurAlpha);
	// 	m_tagTitle.Render();
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		if(m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS && m_BandId == RADIO_BAND_ID_FM)
		{
			if(m_pSkinManager->GetConfig()->GetCurParam()->bRadioTA)
			{
				m_tagTA.SetAlpha(m_fCurAlpha);
				m_tagTA.Render();
			}
			if(m_pSkinManager->GetConfig()->GetCurParam()->bRadioAF)
			{
				m_tagAF.SetAlpha(m_fCurAlpha);
				m_tagAF.Render();
			}

			if(m_tagRT.IsVisible() && !m_bAFSearching)
			{
				if(m_bShowingLongRadioText)
				{
					iFrameTime = 200;
					if(GetTickCount()-m_dwRadioTextUpdateTick>m_iRadioTextUpdateInterval)
					{
						BOOL bEllipsis=FALSE;
						m_dwRadioTextUpdateTick = GetTickCount();
						m_tagRT.SetCaption(m_strRadioText.String()+m_iRadioTextShowOffset,TRUE,&bEllipsis);
						if(bEllipsis)
						{
							m_iRadioTextShowOffset+=1;
						}
						else
						{
							m_iRadioTextShowOffset=0;
						}
						if(m_iRadioTextShowOffset == 0 || m_iRadioTextShowOffset == 1)
						{
							m_iRadioTextUpdateInterval = 3000;
						}
						else
						{
							m_iRadioTextUpdateInterval = 200;
						}
					}
				}
				m_tagRT.SetAlpha(m_fCurAlpha);
				m_tagRT.Render();
			}
		}
#endif
#if 0
		if(m_BandId == RADIO_BAND_ID_FM)
		{
			m_tagPS.SetAlpha(m_fCurAlpha);
			m_tagPS.Render();
			m_tagPTY.SetAlpha(m_fCurAlpha);
			m_tagPTY.Render();
			m_tagRT.SetAlpha(m_fCurAlpha);
			m_tagRT.Render();
		}
#endif

// 		m_tagSignal.SetAlpha(m_fCurAlpha);
// 		m_tagSignal.Render();

		m_btnTuneForward.SetAlpha(m_fCurAlpha);
		m_btnTuneForward.Render();

		m_btnTuneBackward.SetAlpha(m_fCurAlpha);
		m_btnTuneBackward.Render();

		m_btnRadioRegion.SetAlpha(m_fCurAlpha);
		m_btnRadioRegion.Render();

		m_btnSeekBackward.SetAlpha(m_fCurAlpha);
		m_btnSeekBackward.Render();

		m_btnSeekForward.SetAlpha(m_fCurAlpha);
		m_btnSeekForward.Render();

		m_btnSearch.SetAlpha(m_fCurAlpha);
		m_btnSearch.Render();

		m_btnBandFM.SetAlpha(m_fCurAlpha);
		m_btnBandFM.Render();

		m_btnBandAM.SetAlpha(m_fCurAlpha);
		m_btnBandAM.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		m_btnAudioSetting.SetAlpha(m_fCurAlpha);
		m_btnAudioSetting.Render();
#endif

		if(!m_bSearching)
		{
			m_tagNowPlaying.SetAlpha(m_fCurAlpha);
			m_tagNowPlaying.Render();
		}
	}
	else
	{
		if(GetTickCount()-m_dwShowQuickFavTick>30000)
		{
			ShowQuickFav(FALSE);
		}
	}


	//
	//show fav btns:
	if(m_bShowQuickFav || !m_btnQuickFav[0].IsReady() || !m_btnQuickFav[MAX_QUICK_FAV_CNT-1].IsReady())
	{
		for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
		{
			//m_btnQuickFav[i].SetAlpha(m_fCurAlpha);
			m_btnQuickFav[i].Render();
		}
	}

	//
	m_tagBackCircle.SetAlpha(m_fCurAlpha);
	m_tagBackCircle.Render();


	if(m_BandId == RADIO_BAND_ID_FM && m_bShowPSForCurrentFreq)
	{
		m_tagCurFrenquency.Show(FALSE);
		m_btnCurPS.Show(TRUE);

		m_btnCurPS.SetAlpha(m_fCurAlpha);
		m_btnCurPS.Render();
	}
	else
	{
		m_tagCurFrenquency.Show(TRUE);
		m_btnCurPS.Show(FALSE);

		m_tagCurFrenquency.SetAlpha(m_fCurAlpha);
		m_tagCurFrenquency.Render();

		m_tagFreqUnit.SetAlpha(m_fCurAlpha);
		m_tagFreqUnit.Render();
	}

	m_btnFavorite.SetAlpha(m_fCurAlpha);
	m_btnFavorite.Render();

	//
	if(!m_bShowQuickFav)
	{
		//scale:
		if(fabs(m_fScaleOffsetCur-m_fScaleOffsetSet)>0.5f)
		{
			float dev=6.0f;

			float step=(m_fScaleOffsetSet-m_fScaleOffsetCur)/dev;

			if(fabs(step)>40.0f)
				step=step>0?40.0f:-40.0f;
			else if(fabs(step)<0.5f)
				step=step>0?0.5f:-0.5f;

			m_fScaleOffsetCur += step;//(m_fScaleOffsetSet-m_fScaleOffsetCur)/20.0f;
		}
		else
		{
			m_fScaleOffsetCur = m_fScaleOffsetSet;
			m_bScaleMoving = FALSE;
		}

		for (int i=0;i<m_iScaleCount;i++)
		{
			//m_tagScale[i].EnableAnimate(!m_bScaleMoving);
			m_tagScale[i].SetPos(-50*m_iScaleMargin+i*50+m_fScaleOffsetCur,-g_iClientHeight/2+54.0f);

			GLfloat delt=fabs(m_tagScale[i].GetPosX());
			GLfloat ap=1.0-1.1*(delt)/(g_iClientWidth/2-120-m_hidearea);
			if(ap<0)
				ap=0;
			m_tagScale[i].Show(delt<(g_iClientWidth/2-120-m_hidearea));
			m_tagScale[i].SetAlpha(min(ap,m_fCurAlpha));
			m_tagScale[i].Render();
		}

		m_tagFrePointer.SetAlpha(m_fCurAlpha);
		m_tagFrePointer.Render();

	}


	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	m_btnLibrary.SetAlpha(m_fCurAlpha);
	m_btnLibrary.Render();

// 	m_btnSearch.SetAlpha(m_fCurAlpha);
// 	m_btnSearch.Render();
//
// 	m_btnBandFM.SetAlpha(m_fCurAlpha);
// 	m_btnBandFM.Render();
//
// 	m_btnBandAM.SetAlpha(m_fCurAlpha);
// 	m_btnBandAM.Render();

	#if CVTE_EN_DAB
	m_btnDAB.SetAlpha(m_fCurAlpha);
	m_btnDAB.Render();
	#endif

	/*
	m_btnPower.SetAlpha(m_fCurAlpha);
	m_btnPower.Render();
	*/

#if ENABLE_RADIO_DEBUG_SCREEN
	if(m_bEnbaleDebugScreen)
	{
		m_tagDebugPI.SetAlpha(m_fCurAlpha);
		m_tagDebugPI.Render();
		m_tagDebugAFlist.SetAlpha(m_fCurAlpha);
		m_tagDebugAFlist.Render();
	}
#endif

	BOOL bSearchingFlag  = (m_bSearching || (m_bAFSearching&&GetTickCount()-m_dwAFSearchingTick>3000));
	if(bSearchingFlag)
	{
		VIEW_STATE vs=m_tagSearching.GetCurViewState();
		vs.fRotateZ-=5;
		if(vs.fRotateZ<-360)
			vs.fRotateZ=0;
		m_tagSearching.SetCurViewState(&vs);
		m_tagSearching.SetAlpha(m_fCurAlpha);
		m_tagSearching.Render();

		//show PI searching label
		if(m_bAFSearching)
		{
			m_tagPISearching.SetAlpha(m_fCurAlpha);
			m_tagPISearching.Render();
		}
	}

	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	//draw TA notification:
	if(m_bOnTAMode)
	{
		//COLORGL crb={0,0,0,0.8f};
		//DrawRect(0,0,g_iScreenWidth,g_iScreenHeight,&crb);
		m_btnCancelTA.SetAlpha(m_fCurAlpha);
		m_btnCancelTA.Render();
	}
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (!m_bShowQuickFav)
	{
		COLORGL cr_gray={180.0f/256.0f,180.0f/256.0f,180.0f/256.0f,1.0};
#if CVTE_EN_DAB
		DrawRect(-40, g_iClientHeight/2 - 60, 1, 24, &cr_gray);
		DrawRect(40, g_iClientHeight/2 - 60, 1, 24, &cr_gray);
#else
		DrawRect(0, g_iClientHeight/2 - 60, 1, 24, &cr_gray);
#endif
	}
#endif
	//
	SetFrameTime((m_bScaleMoving || bSearchingFlag)?30:iFrameTime);

	EndRender();
}
BOOL CPanelRadioMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

// 	ShowBackground(FALSE);
	ShowStatusBar(TRUE);

	/*
// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_400,_T("\\Windows\\radio_bk.jpg"),g_iClientWidth,g_iClientHeight,FALSE,GL_NEAREST))
	if(!m_pSkinManager->AddTextureFromFile(TEXID_400,_T("\\Windows\\radio_bk.jpg"),g_iClientWidth,500,FALSE,GL_NEAREST))
	{

	}
	if(!m_pSkinManager->AddTextureFromFile(TEXID_499,_T("\\Windows\\radio_bar.jpg"),g_iClientWidth,100,FALSE,GL_NEAREST))
	{

	}
	*/

	COLORGL cr_gray={0.8,0.8,0.8,1.0};
	COLORGL cr_blue={4.0/256.0,190.0/256.0,254.0/256.0,1.0};
	COLORGL cr_gray2={0.9,0.9,0.9,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};

	SIZE si;
	VIEW_STATE vs;

	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2000)));
	si=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	m_tagBkOrigin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2001)));
	si=m_tagBkOrigin.GetTexture()->GetImageSize();
	m_tagBkOrigin.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkOrigin.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkOrigin.SetIniViewState(&vs);

// 	m_tagBar.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_452));
// 	si=m_tagBar.GetTexture()->GetImageSize();
// 	m_tagBar.SetScale(GLfloat(g_iClientWidth)/si.cx,100.0f/si.cy);
// 	m_tagBar.SetPos(0,-(g_iClientHeight-100)/2);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	{
		m_tagBackCircle.SetShowScale(1.0f);
	}
#else
	{
		m_tagBackCircle.SetShowScale(0.85f); //m_tagBackCircle.SetShowScale(1.25);
	}
#endif
	m_tagBackCircle.SetPos(0,0/*20*/);


	//
	SIZE siInfo={360,40};
	SIZE sibtn={200,80};


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	m_tagBackCircle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_1303));

	m_btnRadioRegion.Initialize(IDC_RADIO_BTN_REGION_SEL,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnRadioRegion.SetSize(&sibtn);
	m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_EUROPE));
	m_btnRadioRegion.LinkTextGL(pTextGL);
	m_btnRadioRegion.SetPos(20,g_iClientHeight/2-35);
	m_btnRadioRegion.SetCaptionHeight(28);
	m_btnRadioRegion.EnableDrawBack(FALSE);
	m_btnRadioRegion.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnRadioRegion.SetCaptionColor(&cr_w);
	m_btnRadioRegion.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnRadioRegion.EnableClickAni(TRUE);
	m_btnRadioRegion.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),400,128);


	m_tagNowPlaying.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.SetSize(&siInfo);
	m_tagNowPlaying.SetCaption(_T(""));
	m_tagNowPlaying.LinkTextGL(pTextGL);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagNowPlaying.SetPos(0,g_iClientHeight/2-120-56);
	m_tagNowPlaying.SetCaptionHeight(25);
#else
	m_tagNowPlaying.SetPos(0,g_iClientHeight/2-105);
	m_tagNowPlaying.SetCaptionHeight(25);
#endif
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNowPlaying.SetCaptionColor(&cr_w);

	m_tagSearching.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713));
	m_tagSearching.SetPos(0,0);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	{
		m_tagSearching.SetShowScale(2.15f * 1.0f);
	}
#else
	{
		m_tagSearching.SetShowScale(2.15f * 0.85f);
	}
#endif

	//slider:
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=32;
	sli.size.cy=32;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idThumbTex=0;
	sli.idBackTex=TEXID_024;
	sli.idChannelTex=TEXID_029;

	m_sliRadioSignal.Initialize(1,this,&sli,pSkinManager);
	m_sliRadioSignal.ShowThumb(FALSE);
	m_sliRadioSignal.SetRange(0,5);
	m_sliRadioSignal.SetValue(3);
	m_sliRadioSignal.SetPos(0,90);


	SIZE si1={250,32};
	m_tagPS.Initialize(IDC_RADIO_TAG_STATION_NAME,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPS.SetSize(&si1);
	m_tagPS.SetCaption(_T("PS: Unknown"));
	m_tagPS.LinkTextGL(pTextGL);
	m_tagPS.SetPos(-265,136);
	m_tagPS.SetCaptionHeight(22);
	m_tagPS.EnableDrawBack(FALSE);
	m_tagPS.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPS.Show(FALSE);

	m_tagPTY.Initialize(IDC_RADIO_TAG_RADIO_PROGRAMTYPE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPTY.SetSize(&si1);
	m_tagPTY.SetCaption(_T("PTY: Unknown"));
	m_tagPTY.LinkTextGL(pTextGL);
	m_tagPTY.SetPos(240,136);
	m_tagPTY.SetCaptionHeight(22);
	m_tagPTY.EnableDrawBack(FALSE);
	m_tagPTY.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPTY.Show(FALSE);

	SIZE si2={/*g_iClientWidth*/600,40}; 	//SIZE si2={780,32};

	m_tagRT.Initialize(IDC_RADIO_TAG_RADIO_PROGRAMTYPE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRT.SetSize(&si2);
	m_tagRT.SetCaption(_T(""));
	m_tagRT.LinkTextGL(pTextGL);
	m_tagRT.SetPos(0,g_iClientHeight/2-94);
	m_tagRT.SetCaptionHeight(22);
	m_tagRT.EnableDrawBack(FALSE);
	m_tagRT.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagRT.SetCaptionColor(&cr_gray);
	m_tagRT.Show(FALSE);

	m_tagPISearching.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPISearching.SetSize(&si2);
	m_tagPISearching.SetCaption(_T("PI SEARCHING..."));
	m_tagPISearching.LinkTextGL(pTextGL);
	m_tagPISearching.SetPos(0,g_iClientHeight/2-88);
	m_tagPISearching.SetCaptionHeight(24);
	m_tagPISearching.EnableDrawBack(FALSE);
	m_tagPISearching.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPISearching.SetCaptionColor(&cr_w);


	SIZE siTitle2={38,21};

	m_tagFreqUnit.Initialize(IDC_RADIO_TAG_FREQ_UNIT,this,
		NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagFreqUnit.SetSize(&siTitle2);
	m_tagFreqUnit.SetCaption(GetResourceString(IDS_MHZ));
	m_tagFreqUnit.LinkTextGL(pTextGL);
	m_tagFreqUnit.SetShowScale(0.8f);
	m_tagFreqUnit.SetCaptionColor(&cr_blue);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagFreqUnit.SetPos(93,-13);
#else
	m_tagFreqUnit.SetPos(0,-15);
#endif
	
	m_tagFreqUnit.SetCaptionHeight(18);
	m_tagFreqUnit.EnableDrawBack(FALSE);
	m_tagFreqUnit.SetCaptionPos(BTN_CAPTION_CENTER);
	

	m_tagSTMono.Initialize(IDC_RADIO_TAG_ST_MONO,this,
		NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagSTMono.SetSize(&siTitle2);
	m_tagSTMono.SetCaption(_T("TA  Mono"));
	m_tagSTMono.LinkTextGL(pTextGL);
	m_tagSTMono.SetPos(-200, 90);
	m_tagSTMono.SetCaptionHeight(18);
	m_tagSTMono.EnableDrawBack(FALSE);
	m_tagSTMono.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagSTMono.SetCaptionColor(&cr_gray);

	m_tagLOC.Initialize(0,this,
		NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagLOC.SetSize(&siTitle2);
	m_tagLOC.SetCaption(GetResourceString(IDS_LOC));
	m_tagLOC.LinkTextGL(pTextGL);
	m_tagLOC.SetPos(-290,90);
	m_tagLOC.SetCaptionHeight(22);
	m_tagLOC.EnableDrawBack(FALSE);
	m_tagLOC.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagLOC.SetCaptionColor(&cr_gray);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagTA.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTA.SetSize(&siTitle2);
	m_tagTA.SetCaption(_T("TA : ON"));
	m_tagTA.LinkTextGL(pTextGL);
	m_tagTA.SetPos(180,90);
	m_tagTA.SetCaptionHeight(18);
	m_tagTA.EnableDrawBack(FALSE);
	m_tagTA.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTA.SetCaptionColor(&cr_gray2);

	m_tagAF.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAF.SetSize(&siTitle2);
	m_tagAF.SetCaption(_T("AF : ON"));
	m_tagAF.LinkTextGL(pTextGL);
	m_tagAF.SetPos(300,90);
	m_tagAF.SetCaptionHeight(18);
	m_tagAF.EnableDrawBack(FALSE);
	m_tagAF.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagAF.SetCaptionColor(&cr_gray2);
#endif

	SIZE siInfo2={250,80};

	m_tagCurFrenquency.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagCurFrenquency.SetSize(&siInfo2);
	m_tagCurFrenquency.SetCaption(_T("87.50"));
	m_tagCurFrenquency.LinkTextGL(pTextGL);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagCurFrenquency.SetPos(0,0);
	m_tagCurFrenquency.SetCaptionHeight(40);
#else
	m_tagCurFrenquency.SetPos(0,20);
	m_tagCurFrenquency.SetCaptionHeight(36);
#endif
	m_tagCurFrenquency.EnableDrawBack(FALSE);
	m_tagCurFrenquency.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagCurFrenquency.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_tagCurFrenquency.EnableClickAni(TRUE);


	RECT rc=m_tagCurFrenquency.GetScreenRect();
	rc.top-=80;
	m_tagCurFrenquency.SetTouchRect(&rc);

	//
	m_btnCurPS.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnCurPS.SetSize(280,40);   //&siInfo2
	m_btnCurPS.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnCurPS.SetPos(0,15);
	m_btnCurPS.EnableClickAni(TRUE);
	m_btnCurPS.SetCaption(_T(""));
	m_btnCurPS.LinkTextGL(pTextGL);
	m_btnCurPS.SetCaptionHeight(30);
	m_btnCurPS.EnableDrawBack(FALSE);
	m_btnCurPS.SetCaptionPos(BTN_CAPTION_CENTER);  //BTN_CAPTION_CENTER,0,16
	m_btnCurPS.SetSubCaptionPos(BTN_CAPTION_CENTER,0,-10);
	m_btnCurPS.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),340,170);

	//
	for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
	{
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		m_btnQuickFav[i].Initialize(IDC_RADIO_BTN_QUICK_FAV0+i,this,m_pSkinManager->GetTexture(TEXID_1313),NULL,
			m_pSkinManager->GetTexture(TEXID_1313),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btnQuickFav[i].SetShowScale(0.68f);
		m_btnQuickFav[i].SetCaptionHeight(32);
		//m_btnQuickFav[i].SetCaptionCharStyle(STYLE_ART);
		m_btnQuickFav[i].SetCaptionPos(BTN_CAPTION_CENTER,0,+10);
		m_btnQuickFav[i].SetSubCaption (_T(""));
		m_btnQuickFav[i].SetSubCaptionPos(BTN_CAPTION_CENTER,0,6);

#else
		m_btnQuickFav[i].Initialize(IDC_RADIO_BTN_QUICK_FAV0+i,this,m_pSkinManager->GetTexture(TEXID_1303),NULL,
			m_pSkinManager->GetTexture(TEXID_1303),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btnQuickFav[i].SetShowScale(0.45f);
		m_btnQuickFav[i].SetCaptionHeight(45);
		//m_btnQuickFav[i].SetCaptionCharStyle(STYLE_ART);
		m_btnQuickFav[i].SetCaptionPos(BTN_CAPTION_CENTER,0,0);
		//m_btnQuickFav[i].SetSubCaption (_T(""));
		//m_btnQuickFav[i].SetSubCaptionPos(10,-20);
#endif
		m_btnQuickFav[i].SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
		m_btnQuickFav[i].SetPos(0,0);
		m_btnQuickFav[i].EnableClickAni(TRUE);

		m_btnQuickFav[i].SetCaption(_T(""));
		m_btnQuickFav[i].LinkTextGL(pTextGL);
		m_btnQuickFav[i].EnableNotify(FALSE);
		m_btnQuickFav[i].EnableLongClickNotify(TRUE);
		m_btnQuickFav[i].EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),300,150);
	}

	//home button
	m_btnHome.Initialize(IDC_RADIO_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_205),NULL,
		m_pSkinManager->GetTexture(TEXID_204),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+65,-g_iClientHeight/2+48);
	m_btnHome.SetTouchRate(2.0,2.0);

	//library button 
	m_btnLibrary.Initialize(IDC_RADIO_BTN_LIBRARY,this,
		m_pSkinManager->GetTexture(TEXID_216),NULL,
		m_pSkinManager->GetTexture(TEXID_215),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnLibrary.SetPos(-g_iClientWidth/2+145,-g_iClientHeight/2+48);
	m_btnLibrary.SetTouchRate(2.0,2.0);
	
	//Search button
	m_btnSearch.Initialize(IDC_RADIO_BTN_AMS,this,
		m_pSkinManager->GetTexture(TEXID_272),NULL,
		m_pSkinManager->GetTexture(TEXID_273),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSearch.SetPos(g_iClientWidth/2-60,-g_iClientHeight/2+48);
	m_btnSearch.SetTouchRate(2.0,2.0);

	//主页面FM按钮
	m_btnBandFM.Initialize(IDC_RADIO_BTN_BAND_FM,this,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBandFM.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
#if CVTE_EN_DAB
	m_btnBandFM.SetPos(-80,g_iClientHeight/2-60);
#else
	m_btnBandFM.SetPos(-40,g_iClientHeight/2-60);
#endif
	m_btnBandFM.SetTouchRate(1.5f,1.5f);
	m_btnBandFM.EnableClickAni(TRUE);
	m_btnBandFM.SetCaption(_T("FM"));
	m_btnBandFM.LinkTextGL(pTextGL);
	m_btnBandFM.SetCaptionHeight(24);
	m_btnBandFM.EnableDrawBack(FALSE);
	m_btnBandFM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnBandFM.SetCaptionColor(&cr_gray, &cr_blue);

	//主页面AM按钮
	m_btnBandAM.Initialize(IDC_RADIO_BTN_BAND_AM,this,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBandAM.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
#if CVTE_EN_DAB
	m_btnBandAM.SetPos(0,g_iClientHeight/2-60);
#else
	m_btnBandAM.SetPos(40,g_iClientHeight/2-60);
#endif
	m_btnBandAM.SetTouchRate(1.5f,1.5f);
	m_btnBandAM.EnableClickAni(TRUE);
	m_btnBandAM.SetCaption(_T("AM"));
	m_btnBandAM.LinkTextGL(pTextGL);
	m_btnBandAM.SetCaptionHeight(24);
	m_btnBandAM.EnableDrawBack(FALSE);
	m_btnBandAM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnBandAM.SetCaptionColor(&cr_gray, &cr_blue);


#if CVTE_EN_DAB
	m_btnDAB.Initialize(IDC_RADIO_BTN_DAB,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDAB.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnDAB.SetPos(80,g_iClientHeight/2-60);
	m_btnDAB.SetTouchRate(1.5f,1.5f);
	m_btnDAB.EnableClickAni(TRUE);
	m_btnDAB.SetCaption(_T("DAB"));
	m_btnDAB.LinkTextGL(pTextGL);
	m_btnDAB.SetCaptionHeight(24);
	m_btnDAB.EnableDrawBack(FALSE);
	m_btnDAB.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDAB.SetCaptionColor(&cr_gray, &cr_blue);
#endif //CVTE_EN_DAB
	
	m_btnFavorite.Initialize(IDC_RADIO_BTN_FAVORITE,this,m_pSkinManager->GetTexture(TEXID_1403),NULL,
		m_pSkinManager->GetTexture(TEXID_1404),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnFavorite.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_btnFavorite.SetPos(0,-52-10);
#else
	m_btnFavorite.SetPos(0,-52-10);
#endif
	m_btnFavorite.SetShowScale(1.2f);
	m_btnFavorite.EnableClickAni(TRUE);
	m_btnFavorite.SetTouchRate(1.2f, 1.2f);

	GLfloat step=112.0f*g_iClientWidth/800.0f;

	//前进搜索按钮
	m_btnSeekBackward.Initialize(IDC_RADIO_BTN_SEEKBACKWARD,this,m_pSkinManager->GetTexture(TEXID_1304),NULL,
		m_pSkinManager->GetTexture(TEXID_1305),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeekBackward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSeekBackward.SetPos(-190-step,0);
	m_btnSeekBackward.SetShowScale(0.9f);
	m_btnSeekBackward.SetTouchRate(1.4,1.4);
	m_btnSeekBackward.EnableClickAni(FALSE);
	m_btnSeekBackward.EnableLongClickNotify(TRUE);

	//后退搜索按钮
	m_btnSeekForward.Initialize(IDC_RADIO_BTN_SEEKFORWARD,this,m_pSkinManager->GetTexture(TEXID_1306),NULL,
		m_pSkinManager->GetTexture(TEXID_1307),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeekForward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSeekForward.SetPos(190+step,0);
	m_btnSeekForward.SetShowScale(0.9f);
	m_btnSeekForward.SetTouchRate(1.4,1.4);
	m_btnSeekForward.EnableClickAni(FALSE);
//	m_btnSeekForward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
	m_btnSeekForward.EnableLongClickNotify(TRUE);

	//固定前进步进搜索按钮
	m_btnTuneBackward.Initialize(IDC_RADIO_BTN_TUNEBACKWARD,this,m_pSkinManager->GetTexture(TEXID_1308),NULL,
		m_pSkinManager->GetTexture(TEXID_1309),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTuneBackward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnTuneBackward.SetPos(-190,0);
	m_btnTuneBackward.SetShowScale(0.9f);
	m_btnTuneBackward.SetTouchRate(1.4,1.4);
	m_btnTuneBackward.EnableClickAni(FALSE);
//	m_btnTuneBackward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);

	//固定步进后退搜索按钮
	m_btnTuneForward.Initialize(IDC_RADIO_BTN_TUNEFORWARD,this,m_pSkinManager->GetTexture(TEXID_1310),NULL,
		m_pSkinManager->GetTexture(TEXID_1311),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTuneForward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnTuneForward.SetPos(190,0);
	m_btnTuneForward.SetShowScale(0.9f);
	m_btnTuneForward.SetTouchRate(1.4,1.4);
	m_btnTuneForward.EnableClickAni(FALSE);
	//m_btnTuneForward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);

	//scale:
	//FM
	m_tagFrePointer.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_1312));
	m_tagFrePointer.SetShowScale(1.2f);
	m_tagFrePointer.SetPos(0,-g_iClientHeight/2+40);

	//滑动条每个点坐标
	wchar_t str[16];
	for (int i=0;i<m_iScaleCount;i++)
	{
		m_tagScale[i].Initialize(0,this,m_pSkinManager->GetTexture(i%2?TEXID_426:TEXID_425),NULL,
			NULL,NULL,
			m_pSkinManager->GetTexture(i%2?TEXID_426:TEXID_425),NULL,
			NULL,NULL);
		m_tagScale[i].SetPos(-50*m_iScaleMargin+i*50,-g_iClientHeight/2+43.2f);
		m_tagScale[i].SetShowScale(0.9f);
		if(i%2 == 0 && i>7 && i<52)
		{
			m_tagScale[i].LinkTextGL(pTextGL);
			swprintf_s(str,16,_T("%d"),87+(i-7)/2);
			m_tagScale[i].SetCaption(str);
			m_tagScale[i].SetCaptionPos(BTN_CAPTION_CENTER,0,-36);
			m_tagScale[i].SetCaptionColor(&cr_w);
			m_tagScale[i].SetCaptionHeight(18);
		}
	}

#else
	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	m_tagBackCircle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_461));

	m_btnRadioRegion.Initialize(IDC_RADIO_BTN_REGION_SEL,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnRadioRegion.SetSize(&sibtn);
	m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_EUROPE));
	m_btnRadioRegion.LinkTextGL(pTextGL);
	// 	m_btnRadioRegion.SetPos(0,g_iClientHeight/2-50);
	m_btnRadioRegion.SetPos(20,g_iClientHeight/2-35);
	m_btnRadioRegion.SetCaptionHeight(28);
	m_btnRadioRegion.EnableDrawBack(FALSE);
	m_btnRadioRegion.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnRadioRegion.SetCaptionColor(&cr_w);
	m_btnRadioRegion.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnRadioRegion.EnableClickAni(TRUE);
	m_btnRadioRegion.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),400,128);

	//m_tagNowPlaying.Initialize(IDC_MEDIA_TAG_NOWPLAYINGTITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.SetSize(&siInfo);
	m_tagNowPlaying.SetCaption(_T(""));
	m_tagNowPlaying.LinkTextGL(pTextGL);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagNowPlaying.SetPos(0,g_iClientHeight/2-96-56);
	m_tagNowPlaying.SetCaptionHeight(20);
#else
	m_tagNowPlaying.SetPos(0,g_iClientHeight/2-96);
	m_tagNowPlaying.SetCaptionHeight(25);
#endif

	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNowPlaying.SetCaptionColor(&cr_w);

	m_tagSearching.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713));
	m_tagSearching.SetPos(0,0);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	{
		m_tagSearching.SetShowScale(2.15f * 1.0f);
	}
#else
	{
		m_tagSearching.SetShowScale(2.15f * 0.85f);
	}
#endif


	// 	m_tagSignal.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_452));
	// 	m_tagSignal.SetPos(0,90);
	// 	m_tagSignal.EnableLight(TRUE);
	// 	//m_tagSignal.SetShowScale(1.2);

	//slider:
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=32;
	sli.size.cy=32;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idThumbTex=0;
	sli.idBackTex=TEXID_024;
	sli.idChannelTex=TEXID_029;

	m_sliRadioSignal.Initialize(1,this,&sli,pSkinManager);
	m_sliRadioSignal.ShowThumb(FALSE);
	m_sliRadioSignal.SetRange(0,5);
	m_sliRadioSignal.SetValue(3);
	m_sliRadioSignal.SetPos(0,90);

	// 	m_sliSignalStrength.Initialize(0,this,&sli,pSkinManager);
	// 	m_sliSignalStrength.SetRange(0,240);
	// 	m_sliSignalStrength.SetValue(128);
	// 	m_sliSignalStrength.SetPos(0,6);
	// 	m_sliSignalStrength.ShowThumb(FALSE);

	SIZE si1={250,32};
	m_tagPS.Initialize(IDC_RADIO_TAG_STATION_NAME,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPS.SetSize(&si1);
	m_tagPS.SetCaption(_T("PS: Unknown"));
	m_tagPS.LinkTextGL(pTextGL);
	m_tagPS.SetPos(-265,136);
	m_tagPS.SetCaptionHeight(22);
	m_tagPS.EnableDrawBack(FALSE);
	m_tagPS.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPS.Show(FALSE);

	m_tagPTY.Initialize(IDC_RADIO_TAG_RADIO_PROGRAMTYPE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPTY.SetSize(&si1);
	m_tagPTY.SetCaption(_T("PTY: Unknown"));
	m_tagPTY.LinkTextGL(pTextGL);
	m_tagPTY.SetPos(240,136);
	m_tagPTY.SetCaptionHeight(22);
	m_tagPTY.EnableDrawBack(FALSE);
	m_tagPTY.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPTY.Show(FALSE);

	SIZE si2={/*g_iClientWidth*/600,40}; 	//SIZE si2={780,32};

	m_tagRT.Initialize(IDC_RADIO_TAG_RADIO_PROGRAMTYPE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRT.SetSize(&si2);
	m_tagRT.SetCaption(_T(""));
	m_tagRT.LinkTextGL(pTextGL);
	m_tagRT.SetPos(0,g_iClientHeight/2-88);
	m_tagRT.SetCaptionHeight(22);
	m_tagRT.EnableDrawBack(FALSE);
	m_tagRT.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagRT.SetCaptionColor(&cr_gray);
	m_tagRT.Show(FALSE);

	m_tagPISearching.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPISearching.SetSize(&si2);
	m_tagPISearching.SetCaption(_T("PI SEARCHING..."));
	m_tagPISearching.LinkTextGL(pTextGL);
	m_tagPISearching.SetPos(0,g_iClientHeight/2-88);
	m_tagPISearching.SetCaptionHeight(24);
	m_tagPISearching.EnableDrawBack(FALSE);
	m_tagPISearching.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPISearching.SetCaptionColor(&cr_w);

	//COLORGL cr_b={0,0,1,0};
	SIZE siTitle2={70,32};
	/*
	m_tagBandTitle.Initialize(IDC_RADIO_TAG_BAND_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBandTitle.SetSize(&siTitle2);
	m_tagBandTitle.SetCaption(_T("FM"));
	m_tagBandTitle.LinkTextGL(pTextGL);
	m_tagBandTitle.SetPos(-35,70);
	m_tagBandTitle.SetCaptionHeight(24);
	m_tagBandTitle.EnableDrawBack(FALSE);
	m_tagBandTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagBandTitle.SetCaptionColor(&cr_gray);

	m_tagChannelTitle.Initialize(IDC_RADIO_TAG_BAND_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagChannelTitle.SetSize(&siTitle2);
	m_tagChannelTitle.SetCaption(_T("CH1"));
	m_tagChannelTitle.LinkTextGL(pTextGL);
	m_tagChannelTitle.SetPos(-180,120);
	m_tagChannelTitle.SetCaptionHeight(28);
	m_tagChannelTitle.EnableDrawBack(FALSE);
	m_tagChannelTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	*/


	m_tagFreqUnit.Initialize(IDC_RADIO_TAG_FREQ_UNIT,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagFreqUnit.SetSize(&siTitle2);
	m_tagFreqUnit.SetCaption(GetResourceString(IDS_MHZ));
	m_tagFreqUnit.LinkTextGL(pTextGL);
	m_tagFreqUnit.SetShowScale(0.8f);
	// 	m_tagFreqUnit.SetPos(35,70);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagFreqUnit.SetPos(0,52);
	m_tagFreqUnit.SetCaptionHeight(26);
#else
	m_tagFreqUnit.SetPos(0,62);
	m_tagFreqUnit.SetCaptionHeight(28);
#endif
	m_tagFreqUnit.EnableDrawBack(FALSE);
	m_tagFreqUnit.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagFreqUnit.SetCaptionColor(&cr_gray);

	m_tagSTMono.Initialize(IDC_RADIO_TAG_ST_MONO,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagSTMono.SetSize(&siTitle2);
	m_tagSTMono.SetCaption(GetResourceString(IDS_STEREO));
	m_tagSTMono.LinkTextGL(pTextGL);
	//m_tagSTMono.SetShowScale(0.8f);
	m_tagSTMono.SetPos(-200,90);
	m_tagSTMono.SetCaptionHeight(22);
	m_tagSTMono.EnableDrawBack(FALSE);
	m_tagSTMono.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagSTMono.SetCaptionColor(&cr_gray2);

	m_tagLOC.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagLOC.SetSize(&siTitle2);
	m_tagLOC.SetCaption(GetResourceString(IDS_LOC));
	m_tagLOC.LinkTextGL(pTextGL);
	m_tagLOC.SetPos(-290,90);
	m_tagLOC.SetCaptionHeight(22);
	m_tagLOC.EnableDrawBack(FALSE);
	m_tagLOC.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagLOC.SetCaptionColor(&cr_gray2);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagTA.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTA.SetSize(&siTitle2);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagTA.SetCaption(_T(""));
#else
	wchar_t stmp[16];
	swprintf_s(stmp, 16, _T("TA : %s"), GetResourceString(IDS_ON));
	m_tagTA.SetCaption(stmp);
#endif
	m_tagTA.LinkTextGL(pTextGL);
	m_tagTA.SetPos(180,90);
	m_tagTA.SetCaptionHeight(22);
	m_tagTA.EnableDrawBack(FALSE);
	m_tagTA.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTA.SetCaptionColor(&cr_gray2);

	m_tagAF.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAF.SetSize(&siTitle2);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagAF.SetCaption(_T(""));
#else
	wchar_t stmp2[16];
	swprintf_s(stmp2, 16, _T("AF : %s"), GetResourceString(IDS_ON));
	m_tagAF.SetCaption(stmp2);
#endif
	m_tagAF.LinkTextGL(pTextGL);
	m_tagAF.SetPos(300,90);
	m_tagAF.SetCaptionHeight(22);
	m_tagAF.EnableDrawBack(FALSE);
	m_tagAF.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagAF.SetCaptionColor(&cr_gray2);
#endif
	// 	COLORGL cr_r={1,0,0,1};
	// 	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	// 	m_tagTitle.SetSize(&siTitle2);
	// 	m_tagTitle.SetCaption(_T(""));
	// 	m_tagTitle.LinkTextGL(pTextGL);
	// 	m_tagTitle.SetPos(0,180);
	// 	m_tagTitle.SetCaptionHeight(32);
	// 	m_tagTitle.EnableDrawBack(FALSE);
	// 	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	// 	m_tagTitle.SetCaptionColor(&cr_r);

	SIZE siInfo2={250,80};
	// 	int sy=126,dy=44;
	m_tagCurFrenquency.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagCurFrenquency.SetSize(&siInfo2);
	m_tagCurFrenquency.SetCaption(_T("87.50"));
	m_tagCurFrenquency.LinkTextGL(pTextGL);
	m_tagCurFrenquency.SetPos(0,0);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagCurFrenquency.SetCaptionHeight(48);
#else
	m_tagCurFrenquency.SetCaptionHeight(42);
#endif
	//m_tagCurFrenquency.SetCaptionCharStyle(STYLE_ART);
	m_tagCurFrenquency.EnableDrawBack(FALSE);
	m_tagCurFrenquency.SetCaptionPos(BTN_CAPTION_CENTER);
	//m_tagCurFrenquency.SetShowScale(0.72f);
	m_tagCurFrenquency.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_tagCurFrenquency.EnableClickAni(TRUE);
	m_tagCurFrenquency.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),340,170);

	RECT rc=m_tagCurFrenquency.GetScreenRect();
	rc.top-=80;
	m_tagCurFrenquency.SetTouchRect(&rc);

	//
	m_btnCurPS.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnCurPS.SetSize(280,40);   //&siInfo2
	m_btnCurPS.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnCurPS.SetPos(0,30);
	m_btnCurPS.EnableClickAni(TRUE);
	m_btnCurPS.SetCaption(_T(""));
	//		m_btnCurPS.SetSubCaption(_T("108.0"));
	//	    m_btnCurPS.SetSubCaptionPos(30, -8);
	m_btnCurPS.LinkTextGL(pTextGL);
	m_btnCurPS.SetCaptionHeight(30);
	m_btnCurPS.EnableDrawBack(FALSE);
	m_btnCurPS.SetCaptionPos(BTN_CAPTION_CENTER);  //BTN_CAPTION_CENTER,0,16
	m_btnCurPS.SetSubCaptionPos(BTN_CAPTION_CENTER,0,-10);
	m_btnCurPS.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),340,170);
	
	//
	for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
	{
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		m_btnQuickFav[i].Initialize(IDC_RADIO_BTN_QUICK_FAV0+i,this,m_pSkinManager->GetTexture(TEXID_462),NULL,
			m_pSkinManager->GetTexture(TEXID_462),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btnQuickFav[i].SetShowScale(0.68f);
		m_btnQuickFav[i].SetCaptionHeight(32);
		//m_btnQuickFav[i].SetCaptionCharStyle(STYLE_ART);
		m_btnQuickFav[i].SetCaptionPos(BTN_CAPTION_CENTER,0,+10);
		m_btnQuickFav[i].SetSubCaption (_T(""));
		m_btnQuickFav[i].SetSubCaptionPos(BTN_CAPTION_CENTER,0,6);

#else
		m_btnQuickFav[i].Initialize(IDC_RADIO_BTN_QUICK_FAV0+i,this,m_pSkinManager->GetTexture(TEXID_461),NULL,
			m_pSkinManager->GetTexture(TEXID_461),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btnQuickFav[i].SetShowScale(0.45f);
		m_btnQuickFav[i].SetCaptionHeight(45);
		//m_btnQuickFav[i].SetCaptionCharStyle(STYLE_ART);
		m_btnQuickFav[i].SetCaptionPos(BTN_CAPTION_CENTER,0,0);
		//m_btnQuickFav[i].SetSubCaption (_T(""));
		//m_btnQuickFav[i].SetSubCaptionPos(10,-20);
#endif

		m_btnQuickFav[i].SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
		m_btnQuickFav[i].SetPos(0,0);
		m_btnQuickFav[i].EnableClickAni(TRUE);

		m_btnQuickFav[i].SetCaption(_T(""));
		m_btnQuickFav[i].LinkTextGL(pTextGL);
		m_btnQuickFav[i].EnableNotify(FALSE);
		m_btnQuickFav[i].EnableLongClickNotify(TRUE);
		m_btnQuickFav[i].EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),300,150);
	}

	m_btnHome.Initialize(IDC_RADIO_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+40);
	m_btnHome.SetTouchRate(1.6f,1.6f);
    m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
#if ENABLE_HOME_BTN_CLOSE_RADIO
	m_btnHome.EnableLongClickNotify(TRUE);
	m_btnHome.EnableTouchUpNotify(TRUE);
#endif

	m_btnSearch.Initialize(IDC_RADIO_BTN_AMS,this,m_pSkinManager->GetTexture(TEXID_412),NULL,
		m_pSkinManager->GetTexture(TEXID_413),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSearch.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+40);
	m_btnSearch.SetTouchRate(1.6f,1.6f);
	m_btnSearch.SetShowScale(0.8f);
	m_btnSearch.EnableClickAni(TRUE);
	m_btnSearch.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnLibrary.Initialize(IDC_RADIO_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_401),NULL,
		m_pSkinManager->GetTexture(TEXID_402),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)	
	m_btnLibrary.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-55);
#else
	m_btnLibrary.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-35);
#endif
	m_btnLibrary.SetTouchRate(1.5f,1.5f);
	m_btnLibrary.EnableClickAni(TRUE);
	m_btnLibrary.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnBandFM.Initialize(IDC_RADIO_BTN_BAND_FM,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBandFM.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnBandFM.SetPos(-g_iClientWidth/2+80+80,g_iClientHeight/2-35);
	m_btnBandFM.SetTouchRate(1.5f,1.5f);
	m_btnBandFM.EnableClickAni(TRUE);
	m_btnBandFM.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	m_btnBandFM.SetCaption(_T("FM"));
	m_btnBandFM.LinkTextGL(pTextGL);
	m_btnBandFM.SetCaptionHeight(28);
	m_btnBandFM.EnableDrawBack(FALSE);
	m_btnBandFM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnBandFM.SetCaptionColor(&cr_w);

	m_btnBandAM.Initialize(IDC_RADIO_BTN_BAND_AM,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBandAM.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnBandAM.SetPos(-g_iClientWidth/2+80+80,g_iClientHeight/2-35);
	m_btnBandAM.SetTouchRate(1.5f,1.5f);
	m_btnBandAM.EnableClickAni(TRUE);
	m_btnBandAM.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	m_btnBandAM.SetCaption(_T("AM"));
	m_btnBandAM.LinkTextGL(pTextGL);
	m_btnBandAM.SetCaptionHeight(28);
	m_btnBandAM.EnableDrawBack(FALSE);
	m_btnBandAM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnBandAM.SetCaptionColor(&cr_w);

	#if CVTE_EN_DAB

	m_btnDAB.Initialize(IDC_RADIO_BTN_DAB,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDAB.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnDAB.SetPos(-g_iClientWidth/2+80+160+10,g_iClientHeight/2-35);
	m_btnDAB.SetTouchRate(1.5f,1.5f);
	m_btnDAB.EnableClickAni(TRUE);
	m_btnDAB.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	m_btnDAB.SetCaption(_T("DAB"));
	m_btnDAB.LinkTextGL(pTextGL);
	m_btnDAB.SetCaptionHeight(28);
	m_btnDAB.EnableDrawBack(FALSE);
	m_btnDAB.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDAB.SetCaptionColor(&cr_w);
	#endif

	m_btnFavorite.Initialize(IDC_RADIO_BTN_FAVORITE,this,m_pSkinManager->GetTexture(TEXID_435),NULL,
		m_pSkinManager->GetTexture(TEXID_436),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnFavorite.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnFavorite.SetPos(0,-52-10);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_btnFavorite.SetShowScale(1.0f);
#else
	m_btnFavorite.SetShowScale(0.9f);
#endif

	m_btnFavorite.EnableClickAni(TRUE);
	m_btnFavorite.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	GLfloat step=112.0f*g_iClientWidth/800.0f;

	m_btnSeekBackward.Initialize(IDC_RADIO_BTN_SEEKBACKWARD,this,m_pSkinManager->GetTexture(TEXID_414),NULL,
		m_pSkinManager->GetTexture(TEXID_415),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeekBackward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSeekBackward.SetPos(-190-step,0);
	m_btnSeekBackward.SetShowScale(0.9f);
	m_btnSeekBackward.SetTouchRate(1.4,1.4);
	m_btnSeekBackward.EnableClickAni(FALSE);
	m_btnSeekBackward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
	m_btnSeekBackward.EnableLongClickNotify(TRUE);

	m_btnSeekForward.Initialize(IDC_RADIO_BTN_SEEKFORWARD,this,m_pSkinManager->GetTexture(TEXID_416),NULL,
		m_pSkinManager->GetTexture(TEXID_417),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeekForward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSeekForward.SetPos(190+step,0);
	m_btnSeekForward.SetShowScale(0.9f);
	m_btnSeekForward.SetTouchRate(1.4,1.4);
	m_btnSeekForward.EnableClickAni(FALSE);
	m_btnSeekForward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
	m_btnSeekForward.EnableLongClickNotify(TRUE);

	m_btnTuneBackward.Initialize(IDC_RADIO_BTN_TUNEBACKWARD,this,m_pSkinManager->GetTexture(TEXID_418),NULL,
		m_pSkinManager->GetTexture(TEXID_419),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTuneBackward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnTuneBackward.SetPos(-190,0);
	m_btnTuneBackward.SetShowScale(0.9f);
	m_btnTuneBackward.SetTouchRate(1.4,1.4);
	m_btnTuneBackward.EnableClickAni(FALSE);
	m_btnTuneBackward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);

	m_btnTuneForward.Initialize(IDC_RADIO_BTN_TUNEFORWARD,this,m_pSkinManager->GetTexture(TEXID_420),NULL,
		m_pSkinManager->GetTexture(TEXID_421),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTuneForward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnTuneForward.SetPos(190,0);
	m_btnTuneForward.SetShowScale(0.9f);
	m_btnTuneForward.SetTouchRate(1.4,1.4);
	m_btnTuneForward.EnableClickAni(FALSE);
	m_btnTuneForward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);

	//scale:
	//FM
	m_tagFrePointer.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_424));
	m_tagFrePointer.SetShowScale(0.8f);
	m_tagFrePointer.SetPos(0,-g_iClientHeight/2+40);

	wchar_t str[16];
	for (int i=0;i<m_iScaleCount;i++)
	{
		m_tagScale[i].Initialize(0,this,m_pSkinManager->GetTexture(i%2?TEXID_426:TEXID_425),NULL,
			NULL,NULL,
			m_pSkinManager->GetTexture(i%2?TEXID_428:TEXID_427),NULL,
			NULL,NULL);
		m_tagScale[i].SetPos(-50*m_iScaleMargin+i*50,-g_iClientHeight/2+43.2f);
		m_tagScale[i].SetShowScale(0.9f);
		if(i%2 == 0 && i>7 && i<52)
		{
			m_tagScale[i].LinkTextGL(pTextGL);
			swprintf_s(str,16,_T("%d"),87+(i-7)/2);
			m_tagScale[i].SetCaption(str);
			m_tagScale[i].SetCaptionPos(BTN_CAPTION_CENTER,0,-36);
			m_tagScale[i].SetCaptionColor(&cr_w);
			m_tagScale[i].SetCaptionHeight(18);
		}
	}


#endif //(CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_btnAudioSetting.Initialize(IDC_RADIO_BTN_SETTING,this,m_pSkinManager->GetTexture(TEXID_454),NULL,
		m_pSkinManager->GetTexture(TEXID_455),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAudioSetting.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnAudioSetting.SetPos(-g_iClientWidth/2+240,g_iClientHeight/2-35);
	m_btnAudioSetting.SetTouchRate(1.5f,1.5f);
	//m_btnLibrary.SetShowScale(0.8f);
	m_btnAudioSetting.EnableClickAni(TRUE);
	m_btnAudioSetting.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
#endif




	SIZE si3={g_iScreenWidth,100};
	CM_String strCancelTA(GetResourceString(IDS_CANCEL));
	strCancelTA+=_T(" TA");
	m_btnCancelTA.Initialize(IDC_BT_BTN_CONTACT_CANCEL_DOWNLOAD,this,m_pSkinManager->GetTexture(TEXID_553),NULL,
		m_pSkinManager->GetTexture(TEXID_512),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancelTA.SetCaption(strCancelTA.String());
	m_btnCancelTA.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancelTA.SetCaptionHeight(32);
	m_btnCancelTA.LinkTextGL(pTextGL);
	m_btnCancelTA.SetSize(&si3);
	m_btnCancelTA.SetNotifyFunc(& CPanelRadioMain::OnNotify,this);
	m_btnCancelTA.SetPos(0,-g_iClientHeight/2+50-2);
	m_btnCancelTA.Show(FALSE);

	//读取喜爱电台
	GetQuickFavData();
	GetRdsListData();
    
#if ENABLE_RADIO_DEBUG_SCREEN
	COLORGL cr_red={1.0,0.0,0.0,1.0};
	SIZE siDebug={g_iScreenWidth-20,60};
	GLfloat py=g_iScreenHeight/2-64;

	m_tagDebugPI.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDebugPI.SetSize(&siDebug);
	m_tagDebugPI.SetCaption(_T(""));
	m_tagDebugPI.LinkTextGL(pTextGL);
	m_tagDebugPI.SetPos(0,py);
	m_tagDebugPI.SetCaptionHeight(26);
	m_tagDebugPI.EnableDrawBack(FALSE);
	m_tagDebugPI.SetCaptionPos(BTN_CAPTION_LEFT,300);
	m_tagDebugPI.SetCaptionColor(&cr_red);

	m_tagDebugAFlist.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDebugAFlist.SetSize(&siDebug);
	m_tagDebugAFlist.SetCaption(_T(""));
	m_tagDebugAFlist.LinkTextGL(pTextGL);
	m_tagDebugAFlist.SetPos(0,0);
	m_tagDebugAFlist.SetCaptionHeight(26);
	m_tagDebugAFlist.EnableDrawBack(FALSE);
	m_tagDebugAFlist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagDebugAFlist.SetCaptionColor(&cr_red);
#endif

	return TRUE;
}

void CPanelRadioMain::SetScaleOffset(GLfloat offset,BOOL bAni)
{
//    RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioMain:SetScaleOffset: offset[%d] bAni[%d]\r\n"),int(offset),bAni));
//    RETAILMSG(1/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain:SetScaleOffset: m_fScaleOffsetLimitHigh[%d] m_fScaleOffsetLimitLow[%d] m_BandId[%d]\r\n"),
//	        m_fScaleOffsetLimitHigh,m_fScaleOffsetLimitLow,m_BandId));
	GLfloat offset_scale=offset;
	GLfloat offset_freq=offset;

	if(offset_scale>m_fScaleOffsetLimitHigh+100)
	{
		offset_scale=m_fScaleOffsetLimitHigh+100;
	}
	else if(offset_scale<m_fScaleOffsetLimitLow-100)
	{
		offset_scale=m_fScaleOffsetLimitLow-100;
	}

	if(offset_freq>m_fScaleOffsetLimitHigh)
	{
		offset_freq=m_fScaleOffsetLimitHigh;
	}
	else if(offset_freq<m_fScaleOffsetLimitLow)
	{
		offset_freq=m_fScaleOffsetLimitLow;
	}


	int ioffset = 0;


	//test :
	wchar_t str[32];

	switch (m_BandId)
	{
	case RADIO_BAND_ID_FM:
		{
			const GLfloat start_offset = -(m_dwFreqMinFM/10000.0)+int(m_dwFreqMinFM/1000000)*100.0f;
			ioffset = int(start_offset) + int(offset_freq-start_offset)/m_iStepFM*m_iStepFM;
			offset_freq = GLfloat(ioffset);

			swprintf_s(str,32,_T("%.2f"),int(m_dwFreqMinFM/1000000)-offset_freq/50.0f*0.5);
			m_tagCurFrenquency.SetCaption(str);
 	        //RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioMain:SetScaleOffset: (FM)curFreq[%s]\r\n"),str));
		}
		break;
	case RADIO_BAND_ID_AM:
		{
			int is=m_dwFreqMinAM/100000*100;
			ioffset=int(offset_freq-is)/m_iStepAM*m_iStepAM;
			offset_freq = GLfloat(ioffset+is);

			swprintf_s(str,32,_T("%d"),int(is-offset_freq));
			m_tagCurFrenquency.SetCaption(str);
 	        //RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioMain:SetScaleOffset: (AM)curFreq[%s]\r\n"),str));
		}
		break;
	}

	if(bAni)
	{
		m_bScaleMoving = TRUE;
		m_fScaleOffsetSet = offset_scale;
	}
	else
	{
		m_fScaleOffsetSet = offset_scale;
		m_fScaleOffsetCur = offset_scale;
	}
//	 	RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain:SetScaleOffset: m_fScaleOffsetSet[%d]\r\n"),m_fScaleOffsetSet));
}

//0:Europe:				fm 87.5-108.0MHz, am 522-1620KHz
//1:USA:					fm 87.5-107.9MHz, am 530-1710KHz
//2:EasternEurope:	fm 65.00-74.0MHz, am 522-1620KHz
//3:Japan:					fm 76.0-90.0MHz, am 522-1629KHz
//4:Korea:					fm 87.5-108.0MHz, am 531-1719KHz
//5:Row:					fm 87.5-108.0MHz, am 520-1620KHz
void CPanelRadioMain::UpdateRegionDisplay(RadioRegionId id)
{
	if(id<CVTE_DEF_RADIO_REGION_MIN || id>CVTE_DEF_RADIO_REGION_MAX)
		return;

	m_RegionId = id;
//    RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain:UpdateRegionDisplay id[%d].\r\n"),id));
	for(int i=0;i<_countof(g_RadioRegionParam);i++)
	{
		if(id == g_RadioRegionParam[i].iRegionID)
		{
			m_dwFreqMinFM = g_RadioRegionParam[i].dwFMFrepMin;
			m_dwFreqMaxFM = g_RadioRegionParam[i].dwFMFrepMax;
			m_iStepFM = g_RadioRegionParam[i].iFMStep;
			m_dwFreqMinAM = g_RadioRegionParam[i].dwAMFrepMin;
			m_dwFreqMaxAM = g_RadioRegionParam[i].dwAMFrepMax;
			m_iStepAM = g_RadioRegionParam[i].iAMStep;
			break;
		}
	}

	if(id == RADIO_REGION_Europe)
	{
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_EUROPE));
	}
	else if(id == RADIO_REGION_EasternEurope)
	{
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_EASTERN_EUROPE));
	}
	else
	{

	}


}
void CPanelRadioMain::UpdateScaleForBand(RadioBandId id)
{
	wchar_t str[16];
//	RETAILMSG(1, (TEXT("MSG:RADIO:UpdateScaleForBand: id[%d] m_dwFreqMinFM[%d] m_dwFreqMaxFM[%d]\r\n"),id,m_dwFreqMinFM,m_dwFreqMaxFM));
	switch (id)
	{
	case RADIO_BAND_ID_FM:
		{
			int istart=m_dwFreqMinFM/1000000;
			int count=(m_dwFreqMaxFM-m_dwFreqMinFM)*2/1000000+1;

			int is=(m_dwFreqMinFM/10000-m_dwFreqMinFM/1000000*100);
			int ie=(m_dwFreqMaxFM/10000-m_dwFreqMinFM/10000);

			m_fScaleOffsetLimitHigh = -is;
			m_fScaleOffsetLimitLow = -is-ie;


			for (int i=0;i<m_iScaleCount;i++)
			{
				if(i%2 == 0 && i>7 && i<(7+count+4))
				{
					swprintf_s(str,16,_T("%d"),istart+(i-7)/2);
					m_tagScale[i].SetCaption(str);
				}
				else
				{
					m_tagScale[i].SetCaption(NULL);
				}
			}
		}
		break;
	case RADIO_BAND_ID_AM:
		{
			int istart=m_dwFreqMinAM/100000*100;
			int count=((m_dwFreqMaxAM-m_dwFreqMinAM)/100000+1)*2;

			int is0=m_dwFreqMinAM/100000*100;

			int is=(m_dwFreqMinAM/1000-is0);
			int ie=(m_dwFreqMaxAM-m_dwFreqMinAM)/1000;

			m_fScaleOffsetLimitHigh = -is;//-22;
			m_fScaleOffsetLimitLow = -is-ie;//-50*count-20;

			for (int i=0;i<m_iScaleCount;i++)
			{
				if(i%2 == 0 && i>7 && i<(7+count+4)/*33*/)
				{
					swprintf_s(str,16,_T("%d"),istart+(i-7)/2*100);
					m_tagScale[i].SetCaption(str);
				}
				else
				{
					m_tagScale[i].SetCaption(NULL);
				}
			}
		}
		break;
	default:
		break;
	}
}
void CPanelRadioMain::SetBand(RadioBandId id)
{
    //RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain: SetBand id[%d] ,m_BandId[%d]\r\n"),id,m_BandId));
	//if(m_BandId != id)		//20151022
	COLORGL cr_blue = {4.0/256.0,190.0/256.0,254.0/256.0,1.0};
	COLORGL cr_gray={0.8,0.8,0.8,1.0};
	
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_btnBandFM.Show(TRUE);
		m_btnBandAM.Show(TRUE);
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDM_RADIO_BAND_TYPE,UIOPRT_SETVALUE,(LPVOID)(&id));
#endif
		switch (id)
		{
		case RADIO_BAND_ID_FM:
			m_BandId=RADIO_BAND_ID_FM;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_btnBandFM.SetCaptionColor(&cr_blue);
			m_btnBandAM.SetCaptionColor(&cr_gray);
			#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			m_tagFreqUnit.SetPos(93,-13);
			#endif
#else
			m_btnBandFM.Show(TRUE);
			m_btnBandAM.Show(FALSE);
#endif
			m_tagBandTitle.SetCaption(_T("FM"));
			m_tagFreqUnit.SetCaption(GetResourceString(IDS_MHZ));
			UpdateScaleForBand(RADIO_BAND_ID_FM);
			SetFreq(m_dwCurFreq,TRUE);
			break;
		case RADIO_BAND_ID_AM:
			m_BandId=RADIO_BAND_ID_AM;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_btnBandFM.SetCaptionColor(&cr_gray);
			m_btnBandAM.SetCaptionColor(&cr_blue);
			#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			m_tagFreqUnit.SetPos(72,-13);
			#endif
#else
			m_btnBandFM.Show(FALSE);
			m_btnBandAM.Show(TRUE);
#endif
			m_tagBandTitle.SetCaption(_T("AM"));
			m_tagFreqUnit.SetCaption(GetResourceString(IDS_KHZ));
			UpdateScaleForBand(RADIO_BAND_ID_AM);
			SetFreq(m_dwCurFreq,TRUE);
			break;
		default:
			break;
		}

		//
		UpdateQuickFavList();
	}
}
void CPanelRadioMain::ApplyCurrentScaleFreq()
{
//	    RETAILMSG(DEBUG_RADIO, (TEXT("MSG:RADIO:RadioMain:ApplyCurrentScaleFreq m_BandId[%d]\r\n"),m_BandId));
//	    RETAILMSG(DEBUG_RADIO, (TEXT("MSG:RADIO:RadioMain:ApplyCurrentScaleFreq m_fScaleOffsetSet[%d] m_fScaleOffsetLimitHigh[%d] m_fScaleOffsetLimitLow[%d]\r\n"),
//	        m_fScaleOffsetSet,m_fScaleOffsetLimitHigh,m_fScaleOffsetLimitLow));

	GLfloat offset=m_fScaleOffsetSet;
	if(offset>m_fScaleOffsetLimitHigh)
	{
		offset=m_fScaleOffsetLimitHigh;
	}
	if(offset<m_fScaleOffsetLimitLow)
	{
		offset=m_fScaleOffsetLimitLow;
	}

	switch (m_BandId)
	{
	case RADIO_BAND_ID_FM:
		{
			m_dwFreqFM =DWORD( (int(m_dwFreqMinFM/1000000) - offset*0.01)*1000000 );
			RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain:ApplyCurrentScaleFreq m_dwFreqFM[%d]\r\n"),m_dwFreqFM));

			SendRadioCommand(RADIO_CMD_SET_FREQ,m_dwFreqFM);
		}
		break;
	case RADIO_BAND_ID_AM:
		{
			int is=m_dwFreqMinAM/100000*100;

			m_dwFreqAM =DWORD( (is - offset)*1000 );
			RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain:ApplyCurrentScaleFreq m_dwFreqAM[%d]\r\n"),m_dwFreqAM));

			SendRadioCommand(RADIO_CMD_SET_FREQ,m_dwFreqAM);
		}
		break;
	default:
		break;
	}
}
void CPanelRadioMain::StopScaleAni()
{
	m_fScaleOffsetCur = m_fScaleOffsetSet;

	for (int i=0;i<m_iScaleCount;i++)
	{
		m_tagScale[i].SetPos(-50*m_iScaleMargin+i*50+m_fScaleOffsetCur,-g_iClientHeight/2+66.0f);

		GLfloat delt=fabs(m_tagScale[i].GetPosX());
		GLfloat ap=1.0-1.3*(delt)/g_iClientWidth*2;
		if(ap<0)
			ap=0;
		m_tagScale[i].Show(delt<g_iClientWidth/2);
		m_tagScale[i].SetAlpha(min(ap,m_fCurAlpha));
	}
}
void CPanelRadioMain::SetNowPlayingTitle(const wchar_t *pNowPlayingTitle)
{
	m_tagNowPlaying.SetCaption(pNowPlayingTitle,TRUE);
	if(m_pSmartBarMedia && m_pSmartBarMedia->GetType()==SB_MEDIATYPE_RADIO)
	{
		m_pSmartBarMedia->SetNowPlayingTitle(pNowPlayingTitle);
	}
}
BOOL CPanelRadioMain::OnUpdateRadioPlayInfo()
{
// 	RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("#### CPanelRadioMain::OnUpdateRadioPlayInfo\r\n")));

	BOOL bRet= FALSE;

	//check fav:
	if(m_bCheckFavorite)
	{
		if(GetTickCount() - m_dwFreqChangeTick >= 500 || m_btnFavorite.GetStatus()==BS_FOCUS)
		{
			m_bCheckFavorite = FALSE;
			//在这里会更新标题2/3
			SendRadioCommand(RADIO_CMD_QUERY_FAVORITE_STATUS,m_dwCurFreq);

			::PostMessage(m_hMainWnd,WM_NOTIFY_RADIO_INFO,0,0);

			KillTimer(m_hMainWnd,TIMER_RADIO_UPDATE_PLAYINFO);

			bRet=TRUE;

			//update ps:
			const wchar_t *ps=GetFreqPS(m_dwCurFreq);
			if(ps)
			{
				m_btnCurPS.SetCaption(ps);
				m_btnCurPS.SetSubCaption(m_tagCurFrenquency.GetCaption());
			}
			ShowPSForCurrentFreq(ps!=NULL);
#if ENABLE_SAVE_FREQUENCY
			m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq = m_dwCurFreq;
			m_pSkinManager->GetConfig()->SaveConfig();
#endif
		}
	}

	return bRet;
}
void CPanelRadioMain::SetFavStatus(BOOL bFav)
{
	m_btnFavorite.SetStatus(bFav?BS_FOCUS:BS_UP);
}
void CPanelRadioMain::SetFreq(DWORD freq, BOOL bReset)
{
// 	RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("@@ CPanelRadioMain::SetFreq:%d\r\n"),freq));

	switch (m_BandId)
	{
	case RADIO_BAND_ID_FM:
		{
			if(freq>=m_dwFreqMinFM && freq<=m_dwFreqMaxFM && (m_dwFreqFM != freq || bReset))
			{
				m_dwFreqFM = freq;
				SetScaleOffset(-(freq/10000.0)+int(m_dwFreqMinFM/1000000)*100.0f);
			}
		}
		break;
	case RADIO_BAND_ID_AM:
		{
			if(freq>=m_dwFreqMinAM && freq<=m_dwFreqMaxAM && (m_dwFreqAM != freq || bReset))
			{
				m_dwFreqAM = freq;
				int is=m_dwFreqMinAM/100000*100;
				SetScaleOffset(-(freq/1000.0-is)*1.0f);
			}
		}
		break;
	default:
		break;
	}

}
void CPanelRadioMain::SetSignalStrength(UINT idb)
{
	/*
	//RETAILMSG(1,(_T("+++CPanelRadioMain::SetSignalStrength: %d \r\n"),idb));
	if(idb<=0)
	{
		m_sliRadioSignal.SetValue(0);
	}
	else if(idb>0 && idb<=10)
	{
		m_sliRadioSignal.SetValue(1);
	}
	else if(idb>10 && idb<=15)
	{
		m_sliRadioSignal.SetValue(2);
	}
	else if(idb>15 && idb<=20)
	{
		m_sliRadioSignal.SetValue(3);
	}
	else if(idb>20 && idb<=25)
	{
		m_sliRadioSignal.SetValue(4);
	}
	else
	{
		m_sliRadioSignal.SetValue(5);
	}
	*/

}
void CPanelRadioMain::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

    //RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain::OnMCUCmd:        type:%d,ID:%x,commander:%x\r\n"),dwType,dwID,lParam));

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

	wchar_t str[32];

	if(dwType == 0)//package
	{

		RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioMain: update data package:%x\r\n"),dwID));

		//region:
		UpdateRegionDisplay( RadioRegionId(pCommander->GetItemData(MCU_LOCAL_AREA)) );

		//band:
		int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
		//BYTE high=pCommander->GetItemData(MCU_CUR_FREQU_STORE_H);
		//BYTE low=pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);

        if(iband==0)//FM
		{
			//m_dwCurFreq = ((high*100+low)*10000);
            //RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain: [FM] Package msg m_dwCurFreq=%d\r\n"),m_dwCurFreq));

			SetBand(RADIO_BAND_ID_FM);
		}
		else//AM
		{
			//m_dwCurFreq = ((high*100+low)*1000);
            //RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain: [AM] Package msg m_dwCurFreq=%d\r\n"),m_dwCurFreq));
			SetBand(RADIO_BAND_ID_AM);
		}

		BYTE high=pCommander->GetItemData(MCU_CUR_FREQU_STORE_H);
		BYTE low=pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
		DWORD dwFreq=0;

		if(iband==0)
			dwFreq = ((high*100+low)*10000);
		else
			dwFreq = ((high*100+low)*1000);

		//Frequency store: update for current band

		//channel:
		int ichannel=pCommander->GetItemData(MCU_CUR_CHANNEL_STORE);
		if(ichannel>=0 && ichannel<=5)
		{
			swprintf_s(str,32,_T("CH%d"),ichannel+1);
			m_tagChannelTitle.Show(TRUE);
			m_tagChannelTitle.SetCaption(str);
		}
		else
		{
			m_tagChannelTitle.Show(FALSE);
		}
 	    RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain:OnMCUCmd: (package)channeltitle[%S]\r\n"),str));

		//cur frequency:
		swprintf_s(str,32,(iband==0?_T("%d.%02d"):_T("%d%02d")),high,low);
		m_tagCurFrenquency.SetCaption(str);
 	    RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain:OnMCUCmd: (package)curFreq[%s]\r\n"),str));

		//update ps:
		const wchar_t *ps=GetFreqPS(dwFreq);
		if(ps)
		{
			m_btnCurPS.SetCaption(ps);
			m_btnCurPS.SetSubCaption(m_tagCurFrenquency.GetCaption());
		}
		ShowPSForCurrentFreq(ps!=NULL,TRUE);


		//signal strength:
// 		m_sliSignalStrength.SetValue(pCommander->GetItemData(MCU_CUR_CHANNEL_LEVEL));
		SetSignalStrength(pCommander->GetItemData(MCU_CUR_CHANNEL_LEVEL));

		//title:
		BYTE info=pCommander->GetItemData(MCU_SYS_INFO_TITLE);
		m_bSearching=(info==3||info==4);
// 		switch (info)
// 		{
// 		case 2:
// 			m_tagTitle.Show(TRUE);
// 			m_tagTitle.SetCaption(_T("INTRO"));
// 			break;
// 		case 3:
// 			m_tagTitle.Show(TRUE);
// 			m_tagTitle.SetCaption(_T("AMS"));
// 			break;
// 		default:
// 			m_tagTitle.Show(FALSE);
// 			break;
// 		}

		//ST/MONO
		switch (pCommander->GetItemData(MCU_ST))
		{
		case 1:
			m_tagSTMono.Show(TRUE);
			m_tagSTMono.SetCaption(GetResourceString(IDS_STEREO));
			break;
		case 2:
			m_tagSTMono.Show(TRUE);
			m_tagSTMono.SetCaption(GetResourceString(IDS_MONO));
			break;
		default:
			m_tagSTMono.Show(FALSE);
			break;
		}

		//LOC
		m_tagLOC.Show(pCommander->GetItemData(MCU_LOC));

		UpdateScaleForBand(m_BandId);

		//play state
		BOOL bplay=!pCommander->GetItemData(MCU_RADIO_PLAY_STATE);

		/*
		m_btnPower.SetStatus(bplay?BS_UP:BS_FOCUS);
		*/

		BOOL bShow=bplay;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_BTN_POWERON,UIOPRT_SHOWHIDE,&bShow);
		bShow=!bShow;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_BTN_POWEROFF,UIOPRT_SHOWHIDE,&bShow);

		//RDS:
		/*
		m_bRDSOn = pCommander->GetItemData(MCU_RDS_STATE);
		BYTE id=pCommander->GetItemData(MCU_RDS_PTY_CODE_CUR);
		if(id && m_bRDSOn)
		{
			CM_String str=_T("PTY: ");
			str+=ProgramTypeToString((ProgramType)id);
			m_tagPTY.SetCaption(str.String());
			m_tagPTY.Show(TRUE);
		}
		else
		{
			m_tagPTY.Show(FALSE);
		}
		*/

#if ENABLE_SAVE_FREQUENCY
		if(m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq)
		{
			SendRadioCommand(RADIO_CMD_SET_FREQ,m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq);
		}
#endif
	}
	else if(dwType == 1)//single command
	{
// 		RETAILMSG(DEBUG_RADIO,(_T("+++CPanelRadioMain Parse command:%x"),dwID));

		switch (dwID)
		{
		case MCU_RADIO_PLAY_STATE:
			{
				BOOL bplay=!pCommander->GetItemData(MCU_RADIO_PLAY_STATE);

				//m_btnPower.SetStatus(bplay?BS_UP:BS_FOCUS);

				BOOL bShow=bplay;
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_BTN_POWERON,UIOPRT_SHOWHIDE,&bShow);
				bShow=!bShow;
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_BTN_POWEROFF,UIOPRT_SHOWHIDE,&bShow);

				//
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_RADIO)
				{
					if(m_pSmartBarMedia)
					{
						m_pSmartBarMedia->SetPlayingFlag(bplay);
					}
				}
			}
			break;
// 		case MCU_CUR_FREQU_STORE_H:
		case MCU_CUR_FREQU_STORE_L:
			if(!m_bAFSearching)
			{
				int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
				BYTE high=pCommander->GetItemData(MCU_CUR_FREQU_STORE_H);
				BYTE low=pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
				DWORD dwFreq=0;

				if(iband==0)
					dwFreq = ((high*100+low)*10000);
				else
					dwFreq = ((high*100+low)*1000);

				if(m_dwCurFreq != dwFreq)
				{
					m_dwCurFreq = dwFreq;

					//update freq:
					swprintf_s(str,32,(iband==0?_T("%d.%02d"):_T("%d%02d")),pCommander->GetItemData(MCU_CUR_FREQU_STORE_H),pCommander->GetItemData(MCU_CUR_FREQU_STORE_L));
					m_tagCurFrenquency.SetCaption(str);
					m_bShowPSForCurrentFreq = FALSE;

					//
					if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_RADIO)
					{
						if(m_pSmartBarMedia)
						{
							m_pSmartBarMedia->SetTrackName(str);
						}
					}

					if(iband==0)
						SetFreq((high*100+low)*10000);
					else
						SetFreq((high*100+low)*1000);

	// 				if(iband==0)
	// 					m_dwCurFreq = ((high*100+low)*10000);
	// 				else
	// 					m_dwCurFreq = ((high*100+low)*1000);

					m_dwFreqChangeTick = GetTickCount();

					if(!m_bCheckFavorite)
					{
						KillTimer(m_hMainWnd,TIMER_RADIO_UPDATE_PLAYINFO);
						//每次接受完频率会在RADIO_CMD_QUERY_FAVORITE_STATUS里面更新标题
						SetTimer(m_hMainWnd,TIMER_RADIO_UPDATE_PLAYINFO,500,NULL);
					}
					m_bCheckFavorite = TRUE;

#if ENABLE_RADIO_DEBUG_SCREEN
					ClearDebugData();
#endif
				}
			}
			break;
		case MCU_CUR_CHANNEL_LEVEL:
			{
				//signal strength:
// 				m_sliSignalStrength.SetValue(pCommander->GetItemData(MCU_CUR_CHANNEL_LEVEL));
				SetSignalStrength(pCommander->GetItemData(MCU_CUR_CHANNEL_LEVEL));
			}
			break;
		case MCU_CUR_BAND_STORE:
			{
				int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
                //RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain: #MCU_CUR_BAND_STORE# iband:%d.\r\n"),iband));
				if(iband==0)//FM
				{
					SetBand(RADIO_BAND_ID_FM);
				}
				else//AM
				{
					SetBand(RADIO_BAND_ID_AM);
				}

			}
			break;
		case MCU_CUR_CHANNEL_STORE:
			{
				int ichannel=pCommander->GetItemData(MCU_CUR_CHANNEL_STORE);

//	                RETAILMSG(1, (TEXT("MSG:RADIO:MCU_CUR_CHANNEL_STORE ichannel:%d.\r\n"),ichannel));
				if(ichannel>=0 && ichannel<=5)
				{
					swprintf_s(str,32,_T("CH%d"),ichannel+1);
					m_tagChannelTitle.Show(TRUE);
					m_tagChannelTitle.SetCaption(str);
				}
				else
				{
					m_tagChannelTitle.Show(FALSE);
				}
			}
			break;
			/*
		case MCU_FM1_FREQU_STORE_1_H:
		case MCU_FM1_FREQU_STORE_1_L:
			swprintf_s(str,32,_T("%d.%02d"),pCommander->GetItemData(MCU_FM1_FREQU_STORE_1_H),pCommander->GetItemData(MCU_FM1_FREQU_STORE_1_L));
			m_btnFreq[0].SetCaption(str);
			break;
		case MCU_FM1_FREQU_STORE_2_H:
		case MCU_FM1_FREQU_STORE_2_L:
			swprintf_s(str,32,_T("%d.%02d"),pCommander->GetItemData(MCU_FM1_FREQU_STORE_2_H),pCommander->GetItemData(MCU_FM1_FREQU_STORE_2_L));
			m_btnFreq[1].SetCaption(str);
			break;
		case MCU_FM1_FREQU_STORE_3_H:
		case MCU_FM1_FREQU_STORE_3_L:
			swprintf_s(str,32,_T("%d.%02d"),pCommander->GetItemData(MCU_FM1_FREQU_STORE_3_H),pCommander->GetItemData(MCU_FM1_FREQU_STORE_3_L));
			m_btnFreq[2].SetCaption(str);
			break;
		case MCU_FM1_FREQU_STORE_4_H:
		case MCU_FM1_FREQU_STORE_4_L:
			swprintf_s(str,32,_T("%d.%02d"),pCommander->GetItemData(MCU_FM1_FREQU_STORE_4_H),pCommander->GetItemData(MCU_FM1_FREQU_STORE_4_L));
			m_btnFreq[3].SetCaption(str);
			break;
		case MCU_FM1_FREQU_STORE_5_H:
		case MCU_FM1_FREQU_STORE_5_L:
			swprintf_s(str,32,_T("%d.%02d"),pCommander->GetItemData(MCU_FM1_FREQU_STORE_5_H),pCommander->GetItemData(MCU_FM1_FREQU_STORE_5_L));
			m_btnFreq[4].SetCaption(str);
			break;
		case MCU_FM1_FREQU_STORE_6_H:
		case MCU_FM1_FREQU_STORE_6_L:
			swprintf_s(str,32,_T("%d.%02d"),pCommander->GetItemData(MCU_FM1_FREQU_STORE_6_H),pCommander->GetItemData(MCU_FM1_FREQU_STORE_6_L));
			m_btnFreq[5].SetCaption(str);
			break;
			*/
		case MCU_SYS_INFO_TITLE:
			{
				static BYTE s_last_info=0;

				BYTE info=pCommander->GetItemData(MCU_SYS_INFO_TITLE);

				if(s_last_info==3 && info!=3)
				{
					if(GetTickCount()-m_dwLastSearchTick>3000)
					{
						m_bAutoShowList = TRUE;
					}
					else
					{
						m_bAutoShowList = FALSE;
					}
					m_dwLastSearchTick = GetTickCount();
				}
				else
				{
					m_bAutoShowList = FALSE;
				}
				m_bSearching=(info==3||info==4);
				s_last_info=info;
				//title:
// 				switch (info)
// 				{
// 				case 2:
// 					m_tagTitle.Show(TRUE);
// 					m_tagTitle.SetCaption(_T("INTRO"));
// 					break;
// 				case 3:
// 					m_tagTitle.Show(TRUE);
// 					m_tagTitle.SetCaption(_T("AMS"));
// 					break;
// 				default:
// 					m_tagTitle.Show(FALSE);
// 					break;
// 				}
			}
			break;

		case MCU_ST:
			//ST/MONO
			switch (pCommander->GetItemData(MCU_ST))
			{
			case 1:
				m_tagSTMono.Show(TRUE);
				m_tagSTMono.SetCaption(GetResourceString(IDS_STEREO));
				break;
			case 2:
				m_tagSTMono.Show(TRUE);
				m_tagSTMono.SetCaption(GetResourceString(IDS_MONO));
				break;
			default:
				m_tagSTMono.Show(FALSE);
				break;
			}
			break;
		case MCU_LOC:
			//LOC
			m_tagLOC.Show(pCommander->GetItemData(MCU_LOC));
			break;
		case MCU_LOCAL_AREA:
			UpdateRegionDisplay( RadioRegionId(pCommander->GetItemData(MCU_LOCAL_AREA)) );
			break;
		default:
			break;
		}
	}
}
void CPanelRadioMain::SetRT(const wchar_t* rt)
{
	if(rt)
	{
		BOOL bEllipsis=FALSE;
		if(wcscmp(rt,m_strRadioText.String()) != 0)
		{
			RETAILMSG(1,(_T("####: CPanelRadioMain::SetRT: %s\r\n"),rt));
			m_dwRadioTextUpdateTick = GetTickCount();
			m_iRadioTextShowOffset = 0;
			m_iRadioTextUpdateInterval = 3000;
			m_strRadioText = rt;
			m_tagRT.SetCaption(rt,TRUE,&bEllipsis);
			if(bEllipsis)
			{
				m_bShowingLongRadioText=TRUE;
				m_tagRT.SetCaptionPos(BTN_CAPTION_LEFT);
			}
			else
			{
				m_bShowingLongRadioText=FALSE;
				m_tagRT.SetCaptionPos(BTN_CAPTION_CENTER);
			}
			m_tagRT.Show(TRUE);
		}
	}
	else if(!m_bAFSearching)
	{
		if(wcslen(m_strRadioText.String()))
		{
			RETAILMSG(1,(_T("####: CPanelRadioMain::SetRT: NULL\r\n")));

			m_strRadioText=_T("");
			m_tagRT.SetCaption(_T(""));
			m_tagRT.Show(FALSE);
		}
	}
}
void CPanelRadioMain::SetPTY(ProgramType idPTY)
{
	if(idPTY>0 && idPTY<PT_Count)
	{
		if(m_idPTY != idPTY)
		{
			m_idPTY = idPTY;
			CM_String str=_T("PTY: ");
			str+=ProgramTypeToString((ProgramType)idPTY);
			m_tagPTY.SetCaption(str.String());
			m_tagPTY.Show(TRUE);
		}
	}
	else if(!m_bAFSearching)
	{
		m_tagPTY.Show(FALSE);
	}
}
void CPanelRadioMain::ShowPSForCurrentFreq(BOOL bShow,BOOL bPowerOn)
{
	RETAILMSG(1,(_T("####: ShowPSForCurrentFreq: %d\r\n"),bShow));

	m_bShowPSForCurrentFreq = bShow;
	if(bShow)
	{
		m_tagCurFrenquency.Show(FALSE);
		m_btnCurPS.Show(TRUE);
#if CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
		m_tagNowPlaying.SetPos(0,g_iClientHeight/2-120-48);
		m_btnFavorite.SetPos(0,-52-22);
#endif

		if (bPowerOn)
		{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI) || (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_PS_INFO,UIOPRT_SETSTRING,(LPVOID)m_btnCurPS.GetCaption());
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagCurFrenquency.GetCaption());
#else
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_btnCurPS.GetCaption());
#endif
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,m_BandId==RADIO_BAND_ID_FM?(LPVOID)GetResourceString(IDS_MHZ):(LPVOID)GetResourceString(IDS_KHZ));
#else
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,NULL);
#endif
		}
		else if(m_pSkinManager && m_bEnableUpdateWidget && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_RADIO)
		{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI) || (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_PS_INFO,UIOPRT_SETSTRING,(LPVOID)m_btnCurPS.GetCaption());
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagCurFrenquency.GetCaption());
#else
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_btnCurPS.GetCaption());
#endif
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,m_BandId==RADIO_BAND_ID_FM?(LPVOID)GetResourceString(IDS_MHZ):(LPVOID)GetResourceString(IDS_KHZ));
#else
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,NULL);
#endif
		}
	}
	else
	{
		m_tagCurFrenquency.Show(TRUE);
		m_btnCurPS.Show(FALSE);
#if CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
		m_tagNowPlaying.SetPos(0,g_iClientHeight/2-120-56);
		m_btnFavorite.SetPos(0,-52-10);
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI) || (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_PS_INFO,UIOPRT_SETSTRING,_T(""));
#endif

		if (bPowerOn)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagCurFrenquency.GetCaption());
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,m_BandId==RADIO_BAND_ID_FM?(LPVOID)GetResourceString(IDS_MHZ):(LPVOID)GetResourceString(IDS_KHZ));
		}
		else if(m_pSkinManager && m_bEnableUpdateWidget && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_RADIO)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagCurFrenquency.GetCaption());
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,m_BandId==RADIO_BAND_ID_FM?(LPVOID)GetResourceString(IDS_MHZ):(LPVOID)GetResourceString(IDS_KHZ));
		}
	}
}
void CPanelRadioMain::SetPI(DWORD dwFreq, DWORD dwPICode)
{
	if((m_dwCurPICode != dwPICode || m_dwCurPICodeFreq != dwFreq) && dwFreq && dwPICode)
	{
		m_dwCurPICode = dwPICode;
		m_dwCurPICodeFreq = dwFreq;
		UpdateRdsList(dwFreq,NULL,dwPICode);
	}
}
void CPanelRadioMain::SetPS(const wchar_t* ps)
{
	if(ps /*&& m_bRDSOn*/)
	{
		/*
		const wchar_t *ps_exist=GetFreqPS(m_dwCurFreq);

		if(ps_exist)
		{
			if(ps[0] == ps_exist[0] && wcsstr(ps_exist,ps))//exist
			{
				return;
			}
		}
		*/
		RETAILMSG(1,(_T("####: CPanelRadioMain::SetPS: %s\r\n"),ps));
		wchar_t str[32];
		swprintf_s(str,32,_T("PS: %s"),ps);
		m_tagPS.SetCaption(str);
		m_tagPS.Show(TRUE);

		m_strPS = ps;

		m_btnCurPS.SetCaption(ps);
		m_btnCurPS.SetSubCaption(m_tagCurFrenquency.GetCaption());

		UpdateRdsList(m_dwCurFreq,ps,0);

		//UI update
		ShowPSForCurrentFreq(TRUE);
	}
	else if(!m_bAFSearching)
	{
		if(m_strPS.String())
		{
			RETAILMSG(1,(_T("####: CPanelRadioMain::SetPS: NULL\r\n")));
			m_strPS=NULL;

			m_tagPS.Show(FALSE);

			//UI update
			ShowPSForCurrentFreq(FALSE);
		}
	}
}
void CPanelRadioMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
        RETAILMSG(1, (TEXT("CPanelRadioMain::HandleControlMSG idControl[0x%x].\r\n"),idControl));
	switch (idControl)
	{
	case IDC_RADIO_BTN_SETTING:
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_RADIO);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_HOME:
		{
#if ENABLE_HOME_BTN_CLOSE_RADIO
			if (wMsg == NOTIFY_LONG_CLICK)
			{
				SendRadioCommand(RADIO_CMD_SET_PLAYSTATE,1);
			}
			else if (wMsg == NOTIFY_TOUCH_UP || wMsg == NOTIFY_SHORT_CLICK)
			{
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			}
#else
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
#endif
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_LIBRARY:
		{
			if(!m_bSearching)
			{
				m_bAutoShowList=FALSE;
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_LIBRARY);
				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_RADIO_BTN_REGION_SEL:
		if(m_RegionId == RADIO_REGION_Europe)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = RADIO_REGION_EastEuro_Euro;
			SendRadioCommand(RADIO_CMD_SET_REGION,(1<<16)+RADIO_REGION_EasternEurope);
		}
		else if(m_RegionId == RADIO_REGION_EasternEurope)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = RADIO_REGION_Euro_EastEuro;
			SendRadioCommand(RADIO_CMD_SET_REGION,(1<<16)+RADIO_REGION_Europe);
		}
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_TAG_CURFREQUENCY:
		{
			ShowQuickFav(!m_bShowQuickFav);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_FAVORITE:
		{
			SendRadioCommand(RADIO_CMD_TOGGLE_FAVORITE,m_dwCurFreq);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_SEEKBACKWARD:
		{
			if (wMsg == NOTIFY_SHORT_CLICK)
			{
				SendRadioCommand(RADIO_CMD_PLAY_PREVIOUS,0);
			}
			else if (wMsg == NOTIFY_LONG_CLICK)
			{
				SendRadioCommand(RADIO_CMD_SEARCH,ARM_SUB_OPERATION);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_SEEKFORWARD:
		{
			if (wMsg == NOTIFY_SHORT_CLICK)
			{
				//搜索RADIO_CMD_PLAY_NEXT，有两种不同搜索模式
				SendRadioCommand(RADIO_CMD_PLAY_NEXT,0);
			}
			else if (wMsg == NOTIFY_LONG_CLICK)
			{
				SendRadioCommand(RADIO_CMD_SEARCH,ARM_ADD_OPERATION);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_TUNEBACKWARD:
		{
			SendRadioCommand(RADIO_CMD_SEEK,ARM_SUB_OPERATION);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_TUNEFORWARD:
		{
			SendRadioCommand(RADIO_CMD_SEEK,ARM_ADD_OPERATION);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_BAND_FM:
		{
# if(CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				SendRadioCommand(RADIO_CMD_SELECT_BAND,0);
#else
				SendRadioCommand(RADIO_CMD_SELECT_BAND,1);
#endif		
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_BAND_AM:
		{
# if(CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			SendRadioCommand(RADIO_CMD_SELECT_BAND,1);
#else
			SendRadioCommand(RADIO_CMD_SELECT_BAND,0);
#endif		
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_DAB:
		{
			SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_NONE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_AMS:
		{
			SendRadioCommand(RADIO_CMD_AMS,1);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_POWERON:
		{
			/*
			if(m_btnPower.IsOnFocus())//off
			{
				SendRadioCommand(RADIO_CMD_SET_PLAYSTATE,0);
			}
			else
			{
				SendRadioCommand(RADIO_CMD_SET_PLAYSTATE,1);
			}*/
		}
		break;
	case IDC_RADIO_BTN_QUICK_FAV0:
	case IDC_RADIO_BTN_QUICK_FAV1:
	case IDC_RADIO_BTN_QUICK_FAV2:
	case IDC_RADIO_BTN_QUICK_FAV3:
	case IDC_RADIO_BTN_QUICK_FAV4:
	case IDC_RADIO_BTN_QUICK_FAV5:
	case IDC_RADIO_BTN_QUICK_FAV6:
	case IDC_RADIO_BTN_QUICK_FAV7:
	case IDC_RADIO_BTN_QUICK_FAV8:
	case IDC_RADIO_BTN_QUICK_FAV9:
		{
			int index=idControl-IDC_RADIO_BTN_QUICK_FAV0;
			DWORD dwFreq=m_btnQuickFav[index].GetUserParam();
			if (wMsg == NOTIFY_SHORT_CLICK)
			{
				if(dwFreq)
				{
					switch (m_BandId)
					{
					case RADIO_BAND_ID_FM:
						{
							if(m_RegionId!=m_FMFavList[index].dwRegion)
							{
								SendRadioCommand(RADIO_CMD_SET_REGION,m_FMFavList[index].dwRegion);
							}

							SendRadioCommand(RADIO_CMD_SET_FREQ,dwFreq);
							SendRadioCommand(RADIO_CMD_SET_PI,((dwFreq/100000)<<16)+m_FMFavList[index].dwPICode);
						}
						break;
					case RADIO_BAND_ID_AM:
						{
							if(m_RegionId!=m_AMFavList[index].dwRegion)
							{
								SendRadioCommand(RADIO_CMD_SET_REGION,m_AMFavList[index].dwRegion);
							}

							SendRadioCommand(RADIO_CMD_SET_FREQ,dwFreq);
						}
						break;
					}

					//
					ShowQuickFav(FALSE);
				}
				else
				{
					SendRadioCommand(RADIO_CMD_ADD_FAVORITE,m_dwCurFreq);
					AutoUpdateQuickFav(m_dwCurFreq,idControl-IDC_RADIO_BTN_QUICK_FAV0);
				}
			}
			else if (wMsg == NOTIFY_LONG_CLICK)
			{
				if(m_dwCurFreq==dwFreq)
				{
					SendRadioCommand(RADIO_CMD_DELETE_FAVORITE,m_dwCurFreq);
				}
				else
				{
					SendRadioCommand(RADIO_CMD_ADD_FAVORITE,m_dwCurFreq);
					AutoUpdateQuickFav(m_dwCurFreq,idControl-IDC_RADIO_BTN_QUICK_FAV0);
				}
			}

			m_dwShowQuickFavTick = GetTickCount();
			PlaySoundEffect(0);
		}
		break;
	default:
		break;
	}
}
void CALLBACK CPanelRadioMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_RADIO,(_T("CPanelRadioMain OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelRadioMain *panel=(CPanelRadioMain*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);

}
void CPanelRadioMain::AddQuickFav(RadioBandId id,RadioStationInfo* info,int index)
{
	if(index>=0 && index<MAX_QUICK_FAV_CNT)//add new
	{
		switch (id)
		{
		case RADIO_BAND_ID_FM:
			{
				m_FMFavList[index] = *info;
			}
			break;
		case RADIO_BAND_ID_AM:
			{
				m_AMFavList[index] = *info;
			}
			break;
		default:
			break;
		}
	}
}
void CPanelRadioMain::UpdateQuickFavList()
{
	wchar_t str[32];
	for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
	{
		switch (m_BandId)
		{
		case RADIO_BAND_ID_FM:
			{
				if(m_FMFavList[i].dwFreq)
				{
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
					if(m_FMFavList[i].name[0])
					{
						m_btnQuickFav[i].SetCaption(m_FMFavList[i].name);
					}
					else
					{
						const wchar_t *name=GetFreqPS(m_FMFavList[i].dwFreq);
						m_btnQuickFav[i].SetCaption(name?name:GetResourceString(IDS_UNKNOWN));
						if(name)
						{
							wcscpy_s(m_FMFavList[i].name,32,name);
							m_bFavListChanged = TRUE;
						}
					}

					swprintf_s(str,32,_T("%.2f"),m_FMFavList[i].dwFreq/1000000.0);
					m_btnQuickFav[i].SetSubCaption(str);
#else
					swprintf_s(str,32,_T("%.2f"),m_FMFavList[i].dwFreq/1000000.0);
					m_btnQuickFav[i].SetCaption(str);
					//m_btnQuickFav[i].SetSubCaption(GetResourceString(IDS_MHZ));
#endif
				}
				else
				{
					m_btnQuickFav[i].SetCaption(_T(""));
					m_btnQuickFav[i].SetSubCaption(_T(""));
				}
				m_btnQuickFav[i].SetUserParam(m_FMFavList[i].dwFreq);
			}
			break;
		case RADIO_BAND_ID_AM:
			{
				if(m_AMFavList[i].dwFreq)
				{

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
					swprintf_s(str,32,_T("%d"),m_AMFavList[i].dwFreq/1000);
					m_btnQuickFav[i].SetCaption(GetResourceString(IDS_UNKNOWN));
					m_btnQuickFav[i].SetSubCaption(str);
#else
					swprintf_s(str,32,_T("%d"),m_AMFavList[i].dwFreq/1000);
					m_btnQuickFav[i].SetCaption(str);
					//m_btnQuickFav[i].SetSubCaption(GetResourceString(IDS_KHZ));
#endif


				}
				else
				{
					m_btnQuickFav[i].SetCaption(_T(""));
					m_btnQuickFav[i].SetSubCaption(_T(""));
				}
				m_btnQuickFav[i].SetUserParam(m_AMFavList[i].dwFreq);
			}
			break;
		default:
			break;
		}
	}
}
void CPanelRadioMain::DeleteQuickFav(DWORD dwFreq)
{
	RadioStationInfo *pList=(dwFreq>2000000)?m_FMFavList:m_AMFavList;
	BOOL bUpdateBtns=(dwFreq>2000000)?(m_BandId==RADIO_BAND_ID_FM):(m_BandId==RADIO_BAND_ID_AM);

	int index_exist=-1;

	for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
	{
		if(index_exist==-1 && pList[i].dwFreq==dwFreq)
		{
			index_exist=i;
		}
	}

	if(index_exist!=-1)
	{
		//remove
		if(bUpdateBtns)
		{
			m_btnQuickFav[index_exist].SetCaption(_T(""));
			m_btnQuickFav[index_exist].SetSubCaption(_T(""));
			m_btnQuickFav[index_exist].SetUserParam(0);
		}

		pList[index_exist].dwFreq=0;
		pList[index_exist].name[0]=0;
		pList[index_exist].dwRegion=RADIO_REGION_Europe;
		pList[index_exist].dwPICode=0;
		m_bFavListChanged = TRUE;
	}
}

//在点击收藏喜爱电台后，会调用，将喜爱电台m_FMFavList更新到m_btnQuickFav
void CPanelRadioMain::AutoUpdateQuickFav(DWORD dwFreq,int index)
{
	RadioStationInfo *pList=(dwFreq>2000000)?m_FMFavList:m_AMFavList;
	BOOL bUpdateBtns=(dwFreq>2000000)?(m_BandId==RADIO_BAND_ID_FM):(m_BandId==RADIO_BAND_ID_AM);

	if(index>=0 && index<MAX_QUICK_FAV_CNT)
	{
		int index_first_blank=-1;
		int index_exist=-1;

		for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
		{
			if(index_first_blank==-1 && pList[i].dwFreq==0)
			{
				index_first_blank=i;
			}
			if(index_exist==-1 && pList[i].dwFreq==dwFreq)
			{
				index_exist=i;
			}
		}

		if(index_exist!=-1)//remove old and add new
		{
			//remove
			if(bUpdateBtns)
			{
				m_btnQuickFav[index_exist].SetCaption(_T(""));
				m_btnQuickFav[index_exist].SetSubCaption(_T(""));
				m_btnQuickFav[index_exist].SetUserParam(0);
			}

			pList[index_exist].dwFreq=0;
			pList[index_exist].name[0]=0;
			pList[index_exist].dwPICode=0;
			m_bFavListChanged = TRUE;
		}

		if(index_exist!=index)
		{
			//add new:
			if(bUpdateBtns)
			{
				wchar_t str[32];
				switch (m_BandId)
				{
				case RADIO_BAND_ID_FM:
					{
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
						const wchar_t *name=GetFreqPS(dwFreq);
						m_btnQuickFav[index].SetCaption(name?name:GetResourceString(IDS_UNKNOWN));
						if(name)
						{
							wcscpy_s(pList[index].name,32,name);
						}
						else
						{
							pList[index].name[0]=0;
						}

						swprintf_s(str,32,_T("%.2f"),dwFreq/1000000.0);
						m_btnQuickFav[index].SetSubCaption(str);
						m_btnQuickFav[index].SetUserParam(dwFreq);
#else
						swprintf_s(str,32,_T("%.2f"),dwFreq/1000000.0);
						m_btnQuickFav[index].SetCaption(str);
						//m_btnQuickFav[index].SetSubCaption(GetResourceString(IDS_MHZ));
						m_btnQuickFav[index].SetUserParam(dwFreq);
#endif
					}
					break;
				case RADIO_BAND_ID_AM:
					{
// 						const wchar_t *name=GetFreqPS(dwFreq);
// 						m_btnQuickFav[index].SetCaption(name?name:GetResourceString(IDS_UNKNOWN));
// 						if(name)
// 						{
// 							wcscpy_s(pList[index].name,32,name);
// 						}
// 						else
// 						{
// 							pList[index].name[0]=0;
// 						}
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
						m_btnQuickFav[index].SetCaption(GetResourceString(IDS_UNKNOWN));
						swprintf_s(str,32,_T("%d"),dwFreq/1000);
						m_btnQuickFav[index].SetSubCaption(str);
						m_btnQuickFav[index].SetUserParam(dwFreq);
#else
						swprintf_s(str,32,_T("%d"),dwFreq/1000);
						m_btnQuickFav[index].SetCaption(str);
						//m_btnQuickFav[index].SetSubCaption(GetResourceString(IDS_KHZ));
						m_btnQuickFav[index].SetUserParam(dwFreq);
#endif
					}
					break;
				default:
					break;
				}
			}

			pList[index].dwFreq=dwFreq;
			pList[index].dwRegion=m_RegionId;
			if(m_dwCurPICode && m_dwCurPICodeFreq==dwFreq)
				pList[index].dwPICode=m_dwCurPICode;
			else
				pList[index].dwPICode=GetFreqPI(dwFreq);
			m_bFavListChanged = TRUE;
		}
	}
	else if(index == -1)
	{
		int index_first_blank=-1;
		int index_exist=-1;

		for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
		{
			if(index_first_blank==-1 && pList[i].dwFreq==0)
			{
				index_first_blank=i;
			}
			if(index_exist==-1 && pList[i].dwFreq==dwFreq)
			{
				index_exist=i;
			}
		}

		if(index_exist!=-1)//remove exist
		{
			if(bUpdateBtns)
			{
				m_btnQuickFav[index_exist].SetCaption(_T(""));
				m_btnQuickFav[index_exist].SetSubCaption(_T(""));
				m_btnQuickFav[index_exist].SetUserParam(0);
			}

			pList[index_exist].dwFreq=0;
			pList[index_exist].name[0]=0;
			pList[index_exist].dwPICode=0;
			m_bFavListChanged = TRUE;
		}
		else if(index_first_blank!=-1)//add new
		{
			if(bUpdateBtns)
			{
				wchar_t str[32];
				switch (m_BandId)
				{
				case RADIO_BAND_ID_FM:
					{
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
						const wchar_t *name=GetFreqPS(dwFreq);
						m_btnQuickFav[index_first_blank].SetCaption(name?name:GetResourceString(IDS_UNKNOWN));
						if(name)
						{
							wcscpy_s(pList[index_first_blank].name,32,name);
						}
						else
						{
							pList[index_first_blank].name[0]=0;
						}

						swprintf_s(str,32,_T("%.2f"),dwFreq/1000000.0);
						m_btnQuickFav[index_first_blank].SetSubCaption(str);
						m_btnQuickFav[index_first_blank].SetUserParam(dwFreq);
#else
						swprintf_s(str,32,_T("%.2f"),dwFreq/1000000.0);
						m_btnQuickFav[index_first_blank].SetCaption(str);
						//m_btnQuickFav[index_first_blank].SetSubCaption(GetResourceString(IDS_MHZ));
						m_btnQuickFav[index_first_blank].SetUserParam(dwFreq);
#endif
					}
					break;
				case RADIO_BAND_ID_AM:
					{
						/*
						const wchar_t *name=GetFreqPS(dwFreq);
						m_btnQuickFav[index_first_blank].SetCaption(name?name:GetResourceString(IDS_UNKNOWN));
						if(name)
						{
							wcscpy_s(pList[index_first_blank].name,32,name);
						}
						else
						{
							pList[index_first_blank].name[0]=0;
						}
						*/
#if CVTE_EN_RDS
						m_btnQuickFav[index_first_blank].SetCaption(GetResourceString(IDS_UNKNOWN));
						swprintf_s(str,32,_T("%d"),dwFreq/1000);
						m_btnQuickFav[index_first_blank].SetSubCaption(str);
						m_btnQuickFav[index_first_blank].SetUserParam(dwFreq);
#else
						swprintf_s(str,32,_T("%d"),dwFreq/1000);
						m_btnQuickFav[index_first_blank].SetCaption(str);
						//m_btnQuickFav[index_first_blank].SetSubCaption(GetResourceString(IDS_KHZ));
						m_btnQuickFav[index_first_blank].SetUserParam(dwFreq);
#endif
					}
					break;
				default:
					break;
				}
			}

			pList[index_first_blank].dwFreq=dwFreq;
			pList[index_first_blank].dwRegion=m_RegionId;
			if(m_dwCurPICode && m_dwCurPICodeFreq==dwFreq)
				pList[index_first_blank].dwPICode=m_dwCurPICode;
			else
				pList[index_first_blank].dwPICode=GetFreqPI(dwFreq);
			m_bFavListChanged = TRUE;
		}
	}

}
void CPanelRadioMain::ShowQuickFav(BOOL bShow,BOOL bAni)
{
	m_bShowQuickFav = bShow;
	if(bShow)
	{
		GLfloat step=360.0f/12;
		GLfloat offset_x=0;
		GLfloat offset_y=0;

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
		{
			int index=i;
			if(i>7)
				index=i+2;
			else if(i>2)
				index=i+1;

			if(i==2)
			{
				offset_x=-30;
				offset_y=20;
			}
			else if(i==3)
			{
				offset_x=30;
				offset_y=20;
			}
			else if(i==7)
			{
				offset_x=30;
				offset_y=-20;
			}
			else if(i==8)
			{
				offset_x=-30;
				offset_y=-20;
			}
			else
			{
				offset_x=0;
				offset_y=0;
			}

			m_btnQuickFav[i].EnableNotify(TRUE);
			m_btnQuickFav[i].SetPos(230.0f*cos(index*step*PI/180.0f)+offset_x,/*20+*/180.0f*sin(index*step*PI/180.0f)+offset_y,/*-10*/-1,/*TRUE*/bAni);
            m_btnQuickFav[i].SetAlpha(1.0f,bAni);
		}
#else
		for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
		{
			m_btnQuickFav[i].EnableNotify(TRUE);
			m_btnQuickFav[i].SetPos(190.0f*cos(i*36.0f*PI/180.0f),/*20+*/190.0f*sin(i*36.0f*PI/180.0f),/*-10*/-1,TRUE);
            m_btnQuickFav[i].SetAlpha(1.0f,bAni);
		}
#endif
		m_dwShowQuickFavTick = GetTickCount();
	}
	else
	{
		for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
		{
			m_btnQuickFav[i].EnableNotify(FALSE);
			m_btnQuickFav[i].SetPos(0,20,-1600,/*TRUE*/bAni);
			m_btnQuickFav[i].SetAlpha(0.0f,bAni);
		}
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_btnBandAM.Show(!bShow);
	m_btnBandFM.Show(!bShow);
	m_btnLibrary.Show(!bShow);
#else
	m_btnBandAM.Show(m_BandId==RADIO_BAND_ID_AM && !bShow);
	m_btnBandFM.Show(m_BandId==RADIO_BAND_ID_FM && !bShow);
#endif

	#if CVTE_EN_DAB
	m_btnDAB.Show(!bShow);
	#endif
	m_btnSeekBackward.Show(!bShow);
	m_btnSeekForward.Show(!bShow);
	m_btnTuneBackward.Show(!bShow);
	m_btnTuneForward.Show(!bShow);
	m_btnHome.Show(!bShow);
	m_btnSearch.Show(!bShow);
	m_btnRadioRegion.Show(!bShow && m_BandId==RADIO_BAND_ID_FM
		&& (m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_Euro_EastEuro || m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_EastEuro_Euro));

}
BOOL CPanelRadioMain::SaveQuickFavData()
{
	FILE	*file = NULL;

	if(!m_bFavListChanged)
		return FALSE;

	m_bFavListChanged = FALSE;

	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("Radio\\"));

	CreateDirectory(strPath.String(),NULL);

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\QuickRadioFav.dat"));

	DWORD dwStart=GetTickCount();

	RETAILMSG(DEBUG_RADIO,(_T("\n*************Begin SaveQuickFavData.%s*************\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"wb") )
	{
		return FALSE;
	}


	//FM:
	for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
	{
		fwrite(&m_FMFavList[i],sizeof(RadioStationInfo),1,file);
	}
	//AM:
	for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
	{
		fwrite(&m_AMFavList[i],sizeof(RadioStationInfo),1,file);
	}


	fclose(file);

	RETAILMSG(DEBUG_RADIO,(_T("\n*************End SaveQuickFavData,  take %d ms*************\n"),GetTickCount()-dwStart));

	return TRUE;
}

BOOL CPanelRadioMain::SaveRdsListData()
{
	FILE	*file = NULL;

	if(!m_bRdsListChanged)
		return FALSE;

	m_bRdsListChanged = FALSE;

	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("Radio\\"));

	CreateDirectory(strPath.String(),NULL);

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RdsList.dat"));

	DWORD dwStart=GetTickCount();

	RETAILMSG(1/*DEBUG_RADIO*/,(_T("\n*************Begin SaveRdsListData.%s*************\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"wb") )
	{
		return FALSE;
	}

	m_listRdsStations.Head();
	while (!m_listRdsStations.End())
	{
		fwrite(m_listRdsStations.Get(),sizeof(RadioStationInfo),1,file);
		m_listRdsStations.Next();
	}

	fclose(file);

	RETAILMSG(1/*DEBUG_RADIO*/,(_T("\n*************End SaveRdsListData,  take %d ms*************\n"),GetTickCount()-dwStart));

	return TRUE;
}
BOOL CPanelRadioMain::GetRdsListData()
{
	FILE					*file = NULL;
	int					fileSize = 0;
	BYTE				*localBuffer;
	BYTE				*cursor;

	DWORD dwStart=GetTickCount();

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RdsList.dat"));

	RETAILMSG(1/*DEBUG_RADIO*/,(_T("\n*************Begin GetRdsListData.%s*************\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"rb") )
	{
		RETAILMSG(DEBUG_RADIO,(_T("ERROR: radio data file not found,%s\n"), strFile.String()));
		return  FALSE;
	}

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	localBuffer = new BYTE[fileSize];
	if(!localBuffer)
		return FALSE;

	fread(localBuffer, fileSize, 1, file);
	cursor = localBuffer;

	fclose(file);

	//para data:
	int count=0;
	RadioStationInfo info;
	int siInfo=sizeof(RadioStationInfo);

	while ((cursor-localBuffer)<=fileSize-siInfo)
	{
		memcpy(&info,cursor,siInfo);

		//
		//if(info.name[0])
		{
			UpdateRdsList(info.dwFreq,info.name,info.dwPICode);
		}

		cursor+=siInfo;
		count++;
	}

	delete[] localBuffer;
	m_bRdsListChanged = FALSE;

	RETAILMSG(1/*DEBUG_RADIO*/,(_T("\n*************End GetRdsListData, total count:%d,  take %d ms*************\n"),count,GetTickCount()-dwStart));

	return TRUE;
}

const DWORD CPanelRadioMain::GetFreqPI(DWORD dwFreq)
{
	RadioStationInfo *station=m_listRdsStations.GetItemByID(dwFreq);
	if(station)
	{
		RETAILMSG(0,(_T("\n*************GetFreqPI %d 0x%x*************\n"),dwFreq,station->dwPICode));
		return station->dwPICode;
	}
	else
	{
		return 0;
	}
}
const wchar_t* CPanelRadioMain::GetFreqPS(DWORD dwFreq)
{
	RadioStationInfo *station=m_listRdsStations.GetItemByID(dwFreq);
	if(station)
	{
		if(station->name[0])
		{
			RETAILMSG(1,(_T("\n*************GetFreqPS %d %s *************\n"),dwFreq,station->name));
			return station->name;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;//GetResourceString(IDS_UNKNOWN);
	}
}
void CPanelRadioMain::UpdateRdsList(DWORD dwFreq,const wchar_t* name,DWORD dwPICode)
{
	RETAILMSG(0,(_T("### UpdateRdsList :%d ,0x%x, 0x%x###\r\n"),dwFreq,name,dwPICode));

	RadioStationInfo *station=m_listRdsStations.GetItemByID(dwFreq);
	if(station)
	{
		if(name)
		{
			if(wcscmp(name,station->name) != 0)
			{
				m_bRdsListChanged = TRUE;
				wcscpy_s(station->name,32,name);
			}
		}
		if(dwPICode && station->dwPICode != dwPICode)
		{
			m_bRdsListChanged = TRUE;
			station->dwPICode = dwPICode;
		}
	}
	else
	{
		m_bRdsListChanged = TRUE;

		//create new:
		RadioStationInfo *sta=new RadioStationInfo;
		memset(sta,0,sizeof(RadioStationInfo));
		sta->dwBand=RADIO_BAND_ID_FM;
		sta->dwFreq=dwFreq;
		if(dwPICode)
			sta->dwPICode=dwPICode;
		if(name)
			wcscpy_s(sta->name,32,name);

		m_listRdsStations.Add(sta,dwFreq);
	}

	//update buttons:
	if(name)
	{
		for (int i=0;i<MAX_QUICK_FAV_CNT;i++)
		{
			if(m_FMFavList[i].dwFreq == dwFreq && m_FMFavList[i].dwPICode == dwPICode)
			{
				wcscpy_s(m_FMFavList[i].name,32,name);
				if(m_BandId==RADIO_BAND_ID_FM)
					m_btnQuickFav[i].SetCaption(name);

				m_bFavListChanged = TRUE;

				break;
			}
			else if(m_AMFavList[i].dwFreq == dwFreq)
			{
				wcscpy_s(m_AMFavList[i].name,32,name);
				if(m_BandId==RADIO_BAND_ID_AM)
					m_btnQuickFav[i].SetCaption(name);

				m_bFavListChanged = TRUE;

				break;
			}
		}
	}
}
BOOL CPanelRadioMain::GetQuickFavData()
{
	FILE					*file = NULL;
	int					fileSize = 0;
	BYTE				*localBuffer;
	BYTE				*cursor;

	DWORD dwStart=GetTickCount();

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\QuickRadioFav.dat"));

	RETAILMSG(DEBUG_RADIO,(_T("\n*************Begin GetQuickFavData.%s*************\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"rb") )
	{
		RETAILMSG(DEBUG_RADIO,(_T("ERROR: radio data file not found,%s\n"), strFile.String()));
		return  FALSE;
	}

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	localBuffer = new BYTE[fileSize];
	if(!localBuffer)
		return FALSE;

	fread(localBuffer, fileSize, 1, file);
	cursor = localBuffer;

	fclose(file);

	//para data:
	int count=0;
	RadioStationInfo info;
	int siInfo=sizeof(RadioStationInfo);

	while ((cursor-localBuffer)<=fileSize-siInfo)
	{
		memcpy(&info,cursor,siInfo);
		//

		if(count<MAX_QUICK_FAV_CNT)
		{
			AddQuickFav(RADIO_BAND_ID_FM,&info,count);
		}
		else
		{
			AddQuickFav(RADIO_BAND_ID_AM,&info,count-MAX_QUICK_FAV_CNT);
		}

		//
		if(info.name[0])
		{
			UpdateRdsList(info.dwFreq,info.name,info.dwPICode);
		}

		cursor+=siInfo;
		count++;
	}


	delete[] localBuffer;

	RETAILMSG(DEBUG_RADIO,(_T("\n*************End GetQuickFavData, total count:%d,  take %d ms*************\n"),count,GetTickCount()-dwStart));

	//reset
	m_bFavListChanged = FALSE;

	return TRUE;
}
void CPanelRadioMain::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	switch (idState)
	{
	case SYS_POWER_OFF:
	//case SYS_POWER_SOFT_RESET:
		{
			SaveQuickFavData();
			SaveRdsListData();
		}
		break;
	default:
		break;
	}
}

#if ENABLE_RADIO_DEBUG_SCREEN
void CPanelRadioMain::SetDebugRadioSignalData(UINT idx, const SIGNAL_DATA* pData)
{
	if(idx>=0 && idx<256 && pData)
	{
		memcpy(&m_DebugRadioSignalData[idx],pData,sizeof(SIGNAL_DATA));

		//update af list debug info:
		RETAILMSG(1,(_T("***SetDebugRadioSignalData: %d, %X, s=%d\n"),idx+875,pData->PICode,pData->signalData.level));

		if(m_DebugAFData.AFFilledNum)
		{
			wchar_t temp[256]={0};
			wchar_t cat[32];
			for (int i=0;i<m_DebugAFData.AFFilledNum;i++)
			{
				int freq=m_DebugAFData.AFFreq[i];
				swprintf_s(cat,256,_T("AF%d:%d,%X,%d,%d\n"),i+1,freq+875,m_DebugRadioSignalData[freq].PICode,
					m_DebugRadioSignalData[freq].signalData.level,m_DebugRadioSignalData[freq].signalData.stopQuality);
				wcscat_s(temp,256,cat);
			}
			m_tagDebugAFlist.SetCaption(temp);
		}
	}
}
void CPanelRadioMain::SetDebugAFData(const AFData* pData)
{
	if(pData)
	{
		memcpy(&m_DebugAFData,pData,sizeof(AFData));
	}
}
void CPanelRadioMain::SetDebugPIData(WORD piCode, WORD piCodeUser)
{
	if(m_DebugPIData != piCode)
	{
		RETAILMSG(1,(_T("***SetDebugPIData: %X, userPI: %X\n"),piCode,piCodeUser));

		wchar_t temp[32];
		m_DebugPIData = piCode;
		if(piCode==0 || piCode==0xFFFF)
		{
			swprintf_s(temp,32,_T("INVALID PI, USER = %X"),piCodeUser);
			m_tagDebugPI.SetCaption(temp);
		}
		else
		{
			swprintf_s(temp,32,_T("PI = %X, USER = %X"),piCode,piCodeUser);
			m_tagDebugPI.SetCaption(temp);
		}
	}
}
void CPanelRadioMain::ClearDebugData()
{
	memset(&m_DebugAFData,0,sizeof(AFData));
	m_DebugPIData = 0;
	m_tagDebugPI.SetCaption(_T(""));
	m_tagDebugAFlist.SetCaption(_T(""));
}
#endif
