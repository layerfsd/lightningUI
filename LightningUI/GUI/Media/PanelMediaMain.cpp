#include "PanelMediaMain.h"
#include "../../resource.h"
#if CVTE_EN_ACCON_WARNING
#include "../Warning/WarningProxy.h"
#endif

extern BOOL				g_bMediaModuleLoaded;
extern BOOL				g_bBTModuleLoaded;
extern DWORD           g_dUIScrollBeginTime;//UI is scrolling;

#if CVTE_EN_ACCON_WARNING
static DWORD g_dwFirstEnterTick = 0;
#endif

//main:
CPanelMediaMain::CPanelMediaMain(void)
{
	m_iElapsedTime=0;
	m_iRemainTime=0;

	m_bShowAlert=FALSE;
	m_bVideoMode=TRUE;//FALSE;
	m_bPlayingFlag=FALSE;

	m_bBrowsing = FALSE;

	m_pBuffAlbumArt = NULL;
	m_dwAlbumArtSize = 0;
	m_bNeedUpdateAlbumArt = FALSE;
	m_bHasAlbumArt = FALSE;

	m_bAutoResumeFlag = TRUE;
	m_idLoadingStatus = LOAD_STATUS_NULL;
	m_idCurMediaType=0;

	m_bVideoFullScreen=FALSE;
	m_dwEnterTick = 0;

	m_bUpdateParamFlag = FALSE;

	m_pSmartBarMedia = NULL;

	//
	m_bEnableOperate = TRUE;

	m_bPlayInfoFlyOut = FALSE;
	m_idPlayInfoFlyOutType=0;

	m_bFileLoaded = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_bWarningPageShowed = FALSE;
	m_dwNoValidFileTick = 0;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_bBtnVinylClicked = FALSE;
#endif
	m_title = GetResourceString(IDS_LIBRARY_MENU_TRACKS);
	m_artist = GetResourceString(IDS_LIBRARY_MENU_ARTISTS);
	m_album = GetResourceString(IDS_LIBRARY_MENU_ALBUMS);
}
CPanelMediaMain::~CPanelMediaMain(void)
{

}
BOOL CPanelMediaMain::IsReady()
{
    BOOL bRet = CPanelGL::IsReady() && (m_bVideoMode?TRUE:(m_tagAlbumBk.IsReady() && m_iconAlbum.IsReady()));

    return bRet;
}
void CPanelMediaMain::OnGesture(GESTUREINFO *pgi)
{
	RETAILMSG(1, (TEXT("########## CPanelMediaMain::OnGesture pgi->dwID[%d]##########\r\n"), pgi->dwID));
	if(!pgi)
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
				//向左换，切换到下一首
				case ARG_SCROLL_LEFT:
					{
						g_dUIScrollBeginTime = timeGetTime();
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
						SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PREVIOUS);
#else
						SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_NEXT);
#endif
						PlaySoundEffect(0);
					}
					break;
					//向右滑，切换到上一首
				case ARG_SCROLL_RIGHT:
					{
						g_dUIScrollBeginTime = timeGetTime();
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
						SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_NEXT);
#else
						SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PREVIOUS);
#endif
						PlaySoundEffect(0);
					}
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
void CPanelMediaMain::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);
    RETAILMSG(1, (TEXT("########## CPanelMediaMain::OnGestureMsg ##########\r\n")));
	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY,PAGE_SWITCH_ANI_LEFT);
		break;
	}
}
void CPanelMediaMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);


	if(m_bVideoMode)
	{
		SendMediaCommand(MEDIA_CMD_VIDEO_CONTROL,MEDIA_VIDEO_HIDE);
		//SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PAUSE);
	}

	m_bAutoResumeFlag = (m_bPlayingFlag);
}
void CPanelMediaMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{

	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(DEBUG_MEDIA, (TEXT(" @@@@ CPanelMediaMain::OnEnterPanel iParam:%d\r\n"),iParam));

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


// 	if(!m_bPlayingFlag)
// 	{
// 		SendMediaCommand(MEDIA_CMD_MUTE,1);
// 	}

	if(iParam != -1 && iParam)
	{
		m_bUpdateParamFlag = TRUE;
// 		SendMediaCommand(MEDIA_CMD_CATEGORY,HIWORD(iParam));
// 		SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,LOWORD(iParam));
	}
	else if(iParam == -1)
	{
		m_bAutoResumeFlag = TRUE;
	}
	else if(!m_bFileLoaded)
	{
		m_bAutoResumeFlag = TRUE;
	}

// 	SendMediaCommand(MEDIA_CMD_VIDEO_CONTROL,MEDIA_VIDEO_WINDOW);

	//Switch audio SRC:
	if(GetAudioSRC() != SRC_AUDIO_ARM)
	    SendMediaCommand(MEDIA_CMD_SWITCH_AUDIO_SRC,0);

	//notify src:
	if(m_idCurMediaType)
	{
		::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,m_idCurMediaType,m_bVideoMode?0x20:0x11);
	}


	//
	if(m_pSmartBarMedia)
	{
		if(m_idCurMediaType)
		{
			if(m_idCurMediaType == MEDIA_TYPE_USB1 || m_idCurMediaType == MEDIA_TYPE_USB2)
				m_pSmartBarMedia->SetType(SB_MEDIATYPE_USB);
#ifdef USE_GPS_CARD_MEDIA_PLAY
			else if(m_idCurMediaType == MEDIA_TYPE_SD1 || m_idCurMediaType == MEDIA_TYPE_SD2)
#else
			else if(m_idCurMediaType == MEDIA_TYPE_SD1)
#endif
				m_pSmartBarMedia->SetType(SB_MEDIATYPE_SD);
		}
		m_pSmartBarMedia->SetTrackName(m_tagTitle.GetCaption());
		m_pSmartBarMedia->SetNowPlayingTitle(m_tagNowPlaying.GetCaption());
	}

	if(m_pSkinManager)
	{
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,LPVOID(m_idCurMediaType));
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,(LPVOID)m_tagTitle.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)m_tagArtist.GetCaption());
	}
	//
	m_dwEnterTick = GetTickCount();

#if CVTE_EN_ACCON_WARNING
	if(g_dwFirstEnterTick == 0)
	{
		g_dwFirstEnterTick = m_dwEnterTick;
	}
#endif
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_dwNoValidFileTick = m_dwEnterTick;
#endif
}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

void CPanelMediaMain::Render()
{
// 	static wchar_t str[16];
// 	static COLORGL cr2={0.9,0.9,0.9,1};
	RETAILMSG(DEBUG_MEDIA, (TEXT("CPanelMediaMain::Render enter\r\n")));
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_065):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	static BOOL bRendered=FALSE;
	static BOOL bNoValidFileCanPlay = FALSE; 

	if(!BeginRender())return;

	COLORGL cr_w={1.0,1.0,1.0,m_fCurAlpha};
	COLORGL crb={0.03125,0.03125,0.03125,m_fCurAlpha};

	SIZE si;

	GLfloat fbackLumin=m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance;

#if (CVTE_EN_ACCON_WARNING)
	if(m_bVideoMode && !CWarningProxy::GetInstance()->IsShowWarningSmartBar() && !m_bWarningPageShowed)
	{
		if( GetTickCount() - g_dwFirstEnterTick > 1500)
		{
			m_bWarningPageShowed = TRUE;
			CWarningProxy::GetInstance()->EnterWarningPage(INFINITE);
		}
		//else
		//{
		//	RETAILMSG(1, (TEXT("MSG:[%s]: GetTickCount()-m_dwEnterTick:%d\r\n"),TEXT(__FUNCTION__),GetTickCount()-m_dwEnterTick));
		//}
	}
#endif

	//
	if(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect)
	{
		m_tagBkBlur.SetAlpha(m_fCurAlpha);
		m_tagBkBlur.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkBlur.Render();
	}
	else
	{
		m_tagBkOrigin.SetAlpha(m_fCurAlpha);
		m_tagBkOrigin.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkOrigin.Render();
	}

// 	m_tagBar.SetAlpha(m_fCurAlpha);
// 	m_tagBar.Render();

	//
	if(!m_bVideoMode)
	{
		VIEW_STATE vs=m_tagBackEq.GetCurViewState();
		vs.fRotateX=0;
		vs.fTransY=75;
		vs.fTransX=-253;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.3f);
		m_tagBackEq.Render();
		vs.fTransX=254;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.Render();
		vs.fRotateX=-180;
		vs.fTransY=-35;
		vs.fTransX=-253;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.2f);
		m_tagBackEq.Render();
		vs.fTransX=254;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.Render();
	}

	if(!g_bMediaModuleLoaded)
	{
		if(!m_bVideoMode)
		{
			m_tagVinyl.SetAlpha(m_fCurAlpha);
			m_tagVinyl.Render();

			m_tagAlbumBk.SetAlpha(m_fCurAlpha);
			m_tagAlbumBk.Render();

			if(m_idLoadingStatus != LOAD_STATUS_LOADING)
			{
				m_tagAlbumArt.Show(m_bHasAlbumArt /*&& IsAniReady()*/);
				m_tagAlbumArt.SetAlpha(m_fCurAlpha);
				m_tagAlbumArt.Render();
			}
			else
			{
				m_tagAlbumArt.Animate();
			}

			//title:
			m_tagTitle.SetAlpha(m_fCurAlpha);
			m_tagTitle.Render();
			m_iconTrack.SetAlpha(m_fCurAlpha);
			m_iconTrack.Render();

			//artist:
			m_tagArtist.SetAlpha(m_fCurAlpha);
			m_tagArtist.Render();
			m_iconArtist.SetAlpha(m_fCurAlpha);
			m_iconArtist.Render();

			//album:
			m_tagAlbum.SetAlpha(m_fCurAlpha);
			m_tagAlbum.Render();
			m_iconAlbum.SetAlpha(m_fCurAlpha);
			m_iconAlbum.Render();
		}
		else
		{
// 			m_tagVideoBk.SetAlpha(m_fCurAlpha);
// 			m_tagVideoBk.Render();
			DrawRect(pTexVideoBack,0,50.0f,g_iScreenWidth-16,g_iScreenHeight-80,8,&cr,TRUE);
		}

		//status:
		m_tagMediaStatus.SetAlpha(m_fCurAlpha);
		m_tagMediaStatus.Render();

		//
// 		if(bRendered && IsReady() && GetTickCount()-m_dwEnterTick>500 /*&& g_bBTModuleLoaded*/)
// 		{
// 			//begin to load media module:
// 			SendMediaCommand(MEDIA_CMD_INITIALIZE,NULL);
// 		}
	}
	else //if(m_bBrowsing)
	{
		if(!m_bVideoMode)
		{
			UpdateAlbumArt();

			m_tagVinyl.SetAlpha(m_fCurAlpha);
			m_tagVinyl.Render();

			m_tagAlbumBk.SetAlpha(m_fCurAlpha);
			m_tagAlbumBk.Render();

			if(m_idLoadingStatus != LOAD_STATUS_LOADING)
			{
				m_tagAlbumArt.Show(m_bHasAlbumArt /*&& IsAniReady()*/);
				m_tagAlbumArt.SetAlpha(m_fCurAlpha);
				m_tagAlbumArt.Render();
			}
			else
			{
				m_tagAlbumArt.Animate();
			}

			//title:
			m_tagTitle.SetAlpha(m_fCurAlpha);
			m_tagTitle.Render();
			m_iconTrack.SetAlpha(m_fCurAlpha);
			m_iconTrack.Render();

			//artist:
			m_tagArtist.SetAlpha(m_fCurAlpha);
			m_tagArtist.Render();
			m_iconArtist.SetAlpha(m_fCurAlpha);
			m_iconArtist.Render();

			//album:
			m_tagAlbum.SetAlpha(m_fCurAlpha);
			m_tagAlbum.Render();
			m_iconAlbum.SetAlpha(m_fCurAlpha);
			m_iconAlbum.Render();
		}
		else
		{
// 			m_tagVideoBk.SetAlpha(m_fCurAlpha);
// 			m_tagVideoBk.Render();
			if(m_bVideoFullScreen && !m_bShowAlert)
			{
				DrawRect(pTexVideoBack,0,0,g_iScreenWidth-16,g_iScreenHeight,8,&cr,TRUE);
			}
			else
			{
				DrawRect(pTexVideoBack,0,50.0f,g_iScreenWidth-16,g_iScreenHeight-80,8,&cr,TRUE);
			}

			if(m_idLoadingStatus!=LOAD_STATUS_LOADING/*m_bPlayingFlag*/)
			{
				SetFrameTime(500);

				if(m_bShowAlert)
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
			}
			else
			{
				SetFrameTime(50);	//Modified to make it the samse as FVW8010

				VIEW_STATE vs=m_iconLoading.GetCurViewState();
				vs.fRotateZ-=5;
				if(vs.fRotateZ<-360)
					vs.fRotateZ=0;
				m_iconLoading.SetCurViewState(&vs);
				m_iconLoading.SetAlpha(m_fCurAlpha);
				m_iconLoading.Render();
			}
		}

		if(m_bBrowsing ||
			!m_idCurMediaType ||
			m_idLoadingStatus==LOAD_STATUS_FAILED_INVALID_FILE  ||
			(m_bVideoMode?m_idLoadingStatus==LOAD_STATUS_LOADING:FALSE)  ||
			m_idLoadingStatus==LOAD_STATUS_FAILED_NO_FILE)
		{
			m_tagMediaStatus.SetAlpha(m_fCurAlpha);
			m_tagMediaStatus.Render();
		}
		else if(!m_bVideoMode)
		{
			m_iconSD.SetAlpha(m_fCurAlpha);
			m_iconSD.Render();

			m_iconUSB.SetAlpha(m_fCurAlpha);
			m_iconUSB.Render();

			m_tagNowPlaying.SetAlpha(m_fCurAlpha);
			m_tagNowPlaying.Render();
		}

		//auto resume:
		if(IsAniReady() && GetTickCount()-m_dwEnterTick>/*1500*/500)
		{
//	            //
//	            if(1)
//	            {
//	                if(TickCount()-m_dwEnterTick>1000)
//	                {
//						SendMediaCommand(MEDIA_CMD_LOAD_FILE,_T(""));
//	                    SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAYMODE_REPEAT);
//	                }
//	            }
//	            else
            if(m_bUpdateParamFlag)
			{
				SendMediaCommand(MEDIA_CMD_CATEGORY,HIWORD(m_iParam));
				SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,LOWORD(m_iParam));
				m_bUpdateParamFlag = FALSE;
			}
			else if(m_bAutoResumeFlag)
			{
				if(!m_bPlayingFlag)
				{
					SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAY);
				}
				m_bAutoResumeFlag = FALSE;
			}
		}
	}

	//common controls
	if(!(m_bVideoMode && m_bVideoFullScreen && !m_bShowAlert))
	{
		//bar:
		DrawRect(pTexBar,0,(-g_iClientHeight/2+40),g_iClientWidth-4,80,2,&cr,TRUE);

		m_btnPlayModeShuffle.SetAlpha(m_fCurAlpha);
		m_btnPlayModeShuffle.Render();

		m_btnPlayModeRepeatAll.SetAlpha(m_fCurAlpha);
		m_btnPlayModeRepeatAll.Render();

		m_btnPlayModeRepeatOne.SetAlpha(m_fCurAlpha);
		m_btnPlayModeRepeatOne.Render();



		m_sliProgress.SetAlpha(m_fCurAlpha);
		m_sliProgress.Render();

		m_tagElapsedTime.SetAlpha(m_fCurAlpha);
		m_tagElapsedTime.Render();

		m_tagRemainTime.SetAlpha(m_fCurAlpha);
		m_tagRemainTime.Render();

		m_btnHome.SetAlpha(m_fCurAlpha);
		m_btnHome.Render();

		m_btnPlay.SetAlpha(m_fCurAlpha);
		m_btnPlay.Render();

		m_btnPause.SetAlpha(m_fCurAlpha);
		m_btnPause.Render();

		m_btnPrev.SetAlpha(m_fCurAlpha);
		m_btnPrev.Render();

		m_btnNext.SetAlpha(m_fCurAlpha);
		m_btnNext.Render();

		m_btnLibraryMusic.SetAlpha(m_fCurAlpha);
		m_btnLibraryMusic.Render();

		m_btnLibraryVideo.SetAlpha(m_fCurAlpha);
		m_btnLibraryVideo.Render();

		if(m_pStatusBar && !m_bVideoMode && IsStatusBarVisible())
		{
			m_pStatusBar->SetAlpha(m_fCurAlpha);
			m_pStatusBar->Render();
		}
	}

	bRendered = TRUE;

	if(m_idLoadingStatus == LOAD_STATUS_FAILED_NO_FILE)
	{
		if(bNoValidFileCanPlay)
		{
			DWORD dwTick = GetTickCount();
			if(dwTick - m_dwNoValidFileTick > 3000 && dwTick - m_dwEnterTick > 5000)
			{
				//Auto return to Radio
				bNoValidFileCanPlay = FALSE;
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_DEFAULT);
			}
		}
		else
		{
			bNoValidFileCanPlay = TRUE;
			m_dwNoValidFileTick = GetTickCount();
		}
	}
	else
	{
		bNoValidFileCanPlay = FALSE;
	}

	EndRender();

}
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
void CPanelMediaMain::Render()
{
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_229):NULL;
	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_065):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	static BOOL bRendered=FALSE;
	if(!BeginRender())return;

	COLORGL cr_w={1.0,1.0,1.0,m_fCurAlpha};
	COLORGL crb={0.03125,0.03125,0.03125,m_fCurAlpha};

	SIZE si;
	GLfloat fbackLumin=m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance;

#if (CVTE_EN_ACCON_WARNING)
	if(m_bVideoMode && !CWarningProxy::GetInstance()->IsShowWarningSmartBar() && !m_bWarningPageShowed)
	{
		if( GetTickCount() - g_dwFirstEnterTick > 1500)
		{
			m_bWarningPageShowed = TRUE;
			CWarningProxy::GetInstance()->EnterWarningPage(INFINITE);
		}
	}
#endif

	//
	if(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect)
	{
		m_tagBkBlur.SetAlpha(m_fCurAlpha);
		m_tagBkBlur.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkBlur.Render();
	}
	else
	{
		m_tagBkOrigin.SetAlpha(m_fCurAlpha);
		m_tagBkOrigin.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkOrigin.Render();
	}

	if(!m_bVideoMode)
	{
		m_tagBackEq.SetAlpha(m_fCurAlpha);
		m_tagBackEq.Render();
	}

	if(!g_bMediaModuleLoaded)
	{
		if(!m_bVideoMode)
		{
			m_tagVinyl.SetAlpha(m_fCurAlpha);
			m_tagVinyl.Render();

			m_tagAlbumBk.SetAlpha(m_fCurAlpha);
			m_tagAlbumBk.Render();

			if(m_idLoadingStatus != LOAD_STATUS_LOADING)
			{
				m_tagAlbumArt.Show(m_bHasAlbumArt /*&& IsAniReady()*/);
				m_tagAlbumArt.SetAlpha(m_fCurAlpha);
				m_tagAlbumArt.Render();
			}
			else
			{
				m_tagAlbumArt.Animate();
			}

			//title:
			m_tagTitle.SetAlpha(m_fCurAlpha*0.8f);
			m_tagTitle.Render();
			m_iconTrack.SetAlpha(m_fCurAlpha);
			m_iconTrack.Render();

			//artist:
			m_tagArtist.SetAlpha(m_fCurAlpha*0.8f);
			m_tagArtist.Render();
			m_iconArtist.SetAlpha(m_fCurAlpha);
			m_iconArtist.Render();

			//album:
			m_tagAlbum.SetAlpha(m_fCurAlpha*0.8f);
			m_tagAlbum.Render();
			m_iconAlbum.SetAlpha(m_fCurAlpha);
			m_iconAlbum.Render();
		}

		//status:
		m_tagMediaStatus.SetAlpha(m_fCurAlpha);
		m_tagMediaStatus.Render();

	}
	else //if(m_bBrowsing)
	{
		if(!m_bVideoMode)
		{
			UpdateAlbumArt();

			//if (m_bPlayingFlag && !m_bBtnVinylClicked)
			//{
			//	SetFrameTime(50);
			//	VIEW_STATE vs=m_tagVinyl.GetCurViewState();
			//	vs.fRotateZ-=2;
			//	if(vs.fRotateZ<-360)
			//		vs.fRotateZ=0;
			//	m_tagVinyl.SetCurViewState(&vs);
			//}
			m_tagVinyl.SetAlpha(m_fCurAlpha);
			m_tagVinyl.Render();

			m_tagAlbumBk.SetAlpha(m_fCurAlpha);
			m_tagAlbumBk.Render();

			if(m_idLoadingStatus != LOAD_STATUS_LOADING)
			{
				m_tagAlbumArt.Show(m_bHasAlbumArt /*&& IsAniReady()*/);
				m_tagAlbumArt.SetAlpha(m_fCurAlpha);
				m_tagAlbumArt.Render();
			}
			else
			{
				m_tagAlbumArt.Animate();
			}

			//title:
			m_tagTitle.SetAlpha(m_fCurAlpha*0.8f);
			m_tagTitle.Render();
			m_iconTrack.SetAlpha(m_fCurAlpha);
			m_iconTrack.Render();

			//artist:
			m_tagArtist.SetAlpha(m_fCurAlpha*0.8f);
			m_tagArtist.Render();
			m_iconArtist.SetAlpha(m_fCurAlpha);
			m_iconArtist.Render();

			//album:
			m_tagAlbum.SetAlpha(m_fCurAlpha*0.8f);
			m_tagAlbum.Render();
			m_iconAlbum.SetAlpha(m_fCurAlpha);
			m_iconAlbum.Render();
		}
		else
		{
			if(m_idLoadingStatus!=LOAD_STATUS_LOADING/*m_bPlayingFlag*/)
			{
				SetFrameTime(500);

				if(m_bShowAlert)
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
			}
			else
			{
				SetFrameTime(50);	//Modified to make it the samse as FVW8010

				VIEW_STATE vs=m_iconLoading.GetCurViewState();
				vs.fRotateZ-=5;
				if(vs.fRotateZ<-360)
					vs.fRotateZ=0;
				m_iconLoading.SetCurViewState(&vs);
				m_iconLoading.SetAlpha(m_fCurAlpha);
				m_iconLoading.Render();
			}
		}

		if(m_bBrowsing ||
			!m_idCurMediaType ||
			m_idLoadingStatus==LOAD_STATUS_FAILED_INVALID_FILE  ||
			(m_bVideoMode?m_idLoadingStatus==LOAD_STATUS_LOADING:FALSE)  ||
			m_idLoadingStatus==LOAD_STATUS_FAILED_NO_FILE)
		{
			m_tagMediaStatus.SetAlpha(m_fCurAlpha);
			m_tagMediaStatus.Render();
		}
		else if(!m_bVideoMode)
		{
			m_iconSD.SetAlpha(m_fCurAlpha);
			m_iconSD.Render();

			m_iconUSB.SetAlpha(m_fCurAlpha);
			m_iconUSB.Render();

			m_tagNowPlaying.SetAlpha(m_fCurAlpha);
			m_tagNowPlaying.Render();
		}

		//auto resume:
		if(IsAniReady() && GetTickCount()-m_dwEnterTick>/*1500*/500)
		{
			if(m_bUpdateParamFlag)
			{
				SendMediaCommand(MEDIA_CMD_CATEGORY,HIWORD(m_iParam));
				SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,LOWORD(m_iParam));
				m_bUpdateParamFlag = FALSE;
			}
			else if(m_bAutoResumeFlag)
			{
				if(!m_bPlayingFlag)
				{
					SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAY);
				}
				m_bAutoResumeFlag = FALSE;
			}
		}
	}

	//common controls
	if(!(m_bVideoMode && m_bVideoFullScreen && !m_bShowAlert))
	{
		//bar:
		DrawRect(pTexBar,0,(-g_iClientHeight/2+48),800,96,2,&cr,TRUE);

		m_btnPlayModeShuffle.SetAlpha(m_fCurAlpha);
		m_btnPlayModeShuffle.Render();

		m_btnPlayModeRepeatAll.SetAlpha(m_fCurAlpha);
		m_btnPlayModeRepeatAll.Render();

		m_btnPlayModeRepeatOne.SetAlpha(m_fCurAlpha);
		m_btnPlayModeRepeatOne.Render();



		m_sliProgress.SetAlpha(m_fCurAlpha);
		m_sliProgress.Render();

		m_tagElapsedTime.SetAlpha(m_fCurAlpha*0.8f);
		m_tagElapsedTime.Render();

		m_tagRemainTime.SetAlpha(m_fCurAlpha*0.8f);
		m_tagRemainTime.Render();

		m_btnHome.SetAlpha(m_fCurAlpha);
		m_btnHome.Render();

		m_btnPlay.SetAlpha(m_fCurAlpha);
		m_btnPlay.Render();

		m_btnPause.SetAlpha(m_fCurAlpha);
		m_btnPause.Render();

		m_btnPrev.SetAlpha(m_fCurAlpha);
		m_btnPrev.Render();

		m_btnNext.SetAlpha(m_fCurAlpha);
		m_btnNext.Render();

		m_btnLibrary.SetAlpha(m_fCurAlpha);
		m_btnLibrary.Render();

		if(m_pStatusBar && !m_bVideoMode && IsStatusBarVisible())
		{
			m_pStatusBar->SetAlpha(m_fCurAlpha);
			m_pStatusBar->Render();
		}
	}

	bRendered = TRUE;
	EndRender();

}
#else
void CPanelMediaMain::Render()
{
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_065):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	static BOOL bRendered=FALSE;

	if(!BeginRender())return;

	COLORGL cr_w={1.0,1.0,1.0,m_fCurAlpha};
	COLORGL crb={0.03125,0.03125,0.03125,m_fCurAlpha};

	SIZE si;

	GLfloat fbackLumin=m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance;

#if (CVTE_EN_ACCON_WARNING)
	if(m_bVideoMode && !CWarningProxy::GetInstance()->IsShowWarningSmartBar() && !m_bWarningPageShowed)
	{
		if( GetTickCount() - g_dwFirstEnterTick > 1500)
		{
			m_bWarningPageShowed = TRUE;
			CWarningProxy::GetInstance()->EnterWarningPage(INFINITE);
		}
	}
#endif

	//
	if(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect)
	{
		m_tagBkBlur.SetAlpha(m_fCurAlpha);
		m_tagBkBlur.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkBlur.Render();
	}
	else
	{
		m_tagBkOrigin.SetAlpha(m_fCurAlpha);
		m_tagBkOrigin.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkOrigin.Render();
	}

	//
	if(!m_bVideoMode)
	{
		VIEW_STATE vs=m_tagBackEq.GetCurViewState();
		vs.fRotateX=0;
		vs.fTransY=75;
		vs.fTransX=-253;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.3f);
		m_tagBackEq.Render();
		vs.fTransX=254;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.Render();
		vs.fRotateX=-180;
		vs.fTransY=-35;
		vs.fTransX=-253;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.2f);
		m_tagBackEq.Render();
		vs.fTransX=254;
		m_tagBackEq.SetCurViewState(&vs);
		m_tagBackEq.Render();
	}

	if(!g_bMediaModuleLoaded)
	{
		if(!m_bVideoMode)
		{
			m_tagVinyl.SetAlpha(m_fCurAlpha);
			m_tagVinyl.Render();

			m_tagAlbumBk.SetAlpha(m_fCurAlpha);
			m_tagAlbumBk.Render();

			if(m_idLoadingStatus != LOAD_STATUS_LOADING)
			{
				m_tagAlbumArt.Show(m_bHasAlbumArt /*&& IsAniReady()*/);
				m_tagAlbumArt.SetAlpha(m_fCurAlpha);
				m_tagAlbumArt.Render();
			}
			else
			{
				m_tagAlbumArt.Animate();
			}

			//title:
			m_tagTitle.SetAlpha(m_fCurAlpha);
			m_tagTitle.Render();
			m_iconTrack.SetAlpha(m_fCurAlpha);
			m_iconTrack.Render();

			//artist:
			m_tagArtist.SetAlpha(m_fCurAlpha);
			m_tagArtist.Render();
			m_iconArtist.SetAlpha(m_fCurAlpha);
			m_iconArtist.Render();

			//album:
			m_tagAlbum.SetAlpha(m_fCurAlpha);
			m_tagAlbum.Render();
			m_iconAlbum.SetAlpha(m_fCurAlpha);
			m_iconAlbum.Render();
		}
		else
		{
			DrawRect(pTexVideoBack,0,50.0f,g_iScreenWidth-16,g_iScreenHeight-80,8,&cr,TRUE);
		}

		//status:
		m_tagMediaStatus.SetAlpha(m_fCurAlpha);
		m_tagMediaStatus.Render();
	}
	else //if(m_bBrowsing)
	{
		if(!m_bVideoMode)
		{
			UpdateAlbumArt();

			m_tagVinyl.SetAlpha(m_fCurAlpha);
			m_tagVinyl.Render();

			m_tagAlbumBk.SetAlpha(m_fCurAlpha);
			m_tagAlbumBk.Render();

			if(m_idLoadingStatus != LOAD_STATUS_LOADING)
			{
				m_tagAlbumArt.Show(m_bHasAlbumArt /*&& IsAniReady()*/);
				m_tagAlbumArt.SetAlpha(m_fCurAlpha);
				m_tagAlbumArt.Render();
			}
			else
			{
				m_tagAlbumArt.Animate();
			}

			//title:
			m_tagTitle.SetAlpha(m_fCurAlpha);
			m_tagTitle.Render();
			m_iconTrack.SetAlpha(m_fCurAlpha);
			m_iconTrack.Render();

			//artist:
			m_tagArtist.SetAlpha(m_fCurAlpha);
			m_tagArtist.Render();
			m_iconArtist.SetAlpha(m_fCurAlpha);
			m_iconArtist.Render();

			//album:
			m_tagAlbum.SetAlpha(m_fCurAlpha);
			m_tagAlbum.Render();
			m_iconAlbum.SetAlpha(m_fCurAlpha);
			m_iconAlbum.Render();
		}
		else
		{
			if(m_bVideoFullScreen && !m_bShowAlert)
			{
				DrawRect(pTexVideoBack,0,0,g_iScreenWidth-16,g_iScreenHeight,8,&cr,TRUE);
			}
			else
			{
				DrawRect(pTexVideoBack,0,50.0f,g_iScreenWidth-16,g_iScreenHeight-80,8,&cr,TRUE);
			}

			if(m_idLoadingStatus!=LOAD_STATUS_LOADING/*m_bPlayingFlag*/)
			{
				SetFrameTime(500);

				if(m_bShowAlert)
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
			}
			else
			{
				SetFrameTime(50);	//Modified to make it the samse as FVW8010

				VIEW_STATE vs=m_iconLoading.GetCurViewState();
				vs.fRotateZ-=5;
				if(vs.fRotateZ<-360)
					vs.fRotateZ=0;
				m_iconLoading.SetCurViewState(&vs);
				m_iconLoading.SetAlpha(m_fCurAlpha);
				m_iconLoading.Render();
			}
		}

		if(m_bBrowsing ||
			!m_idCurMediaType ||
			m_idLoadingStatus==LOAD_STATUS_FAILED_INVALID_FILE  ||
			(m_bVideoMode?m_idLoadingStatus==LOAD_STATUS_LOADING:FALSE)  ||
			m_idLoadingStatus==LOAD_STATUS_FAILED_NO_FILE)
		{
			m_tagMediaStatus.SetAlpha(m_fCurAlpha);
			m_tagMediaStatus.Render();
		}
		else if(!m_bVideoMode)
		{
			m_iconSD.SetAlpha(m_fCurAlpha);
			m_iconSD.Render();

			m_iconUSB.SetAlpha(m_fCurAlpha);
			m_iconUSB.Render();

			m_tagNowPlaying.SetAlpha(m_fCurAlpha);
			m_tagNowPlaying.Render();
		}

		//auto resume:
		if(IsAniReady() && GetTickCount()-m_dwEnterTick>/*1500*/500)
		{
			if(m_bUpdateParamFlag)
			{
				SendMediaCommand(MEDIA_CMD_CATEGORY,HIWORD(m_iParam));
				SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,LOWORD(m_iParam));
				m_bUpdateParamFlag = FALSE;
			}
			else if(m_bAutoResumeFlag)
			{
				if(!m_bPlayingFlag)
				{
					SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAY);
				}
				m_bAutoResumeFlag = FALSE;
			}
		}
	}

	//common controls
	if(!(m_bVideoMode && m_bVideoFullScreen && !m_bShowAlert))
	{
		//bar:
		DrawRect(pTexBar,0,(-g_iClientHeight/2+40),g_iClientWidth-4,80,2,&cr,TRUE);

		m_btnPlayModeShuffle.SetAlpha(m_fCurAlpha);
		m_btnPlayModeShuffle.Render();

		m_btnPlayModeRepeatAll.SetAlpha(m_fCurAlpha);
		m_btnPlayModeRepeatAll.Render();

		m_btnPlayModeRepeatOne.SetAlpha(m_fCurAlpha);
		m_btnPlayModeRepeatOne.Render();



		m_sliProgress.SetAlpha(m_fCurAlpha);
		m_sliProgress.Render();

		m_tagElapsedTime.SetAlpha(m_fCurAlpha);
		m_tagElapsedTime.Render();

		m_tagRemainTime.SetAlpha(m_fCurAlpha);
		m_tagRemainTime.Render();

		m_btnHome.SetAlpha(m_fCurAlpha);
		m_btnHome.Render();

		m_btnPlay.SetAlpha(m_fCurAlpha);
		m_btnPlay.Render();

		m_btnPause.SetAlpha(m_fCurAlpha);
		m_btnPause.Render();

		m_btnPrev.SetAlpha(m_fCurAlpha);
		m_btnPrev.Render();

		m_btnNext.SetAlpha(m_fCurAlpha);
		m_btnNext.Render();

		m_btnLibraryMusic.SetAlpha(m_fCurAlpha);
		m_btnLibraryMusic.Render();

		m_btnLibraryVideo.SetAlpha(m_fCurAlpha);
		m_btnLibraryVideo.Render();

		if(m_pStatusBar && !m_bVideoMode && IsStatusBarVisible())
		{
			m_pStatusBar->SetAlpha(m_fCurAlpha);
			m_pStatusBar->Render();
		}
	}

	bRendered = TRUE;

	EndRender();

}
#endif

BOOL CPanelMediaMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	ShowBackground(FALSE);
	ShowStatusBar(TRUE);

	SIZE si;
	VIEW_STATE vs;
	COLORGL crw={1.0,1.0,1.0,1.0};

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

	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	m_tagVinyl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_200));
	m_tagVinyl.SetPos(-g_iClientWidth/2+268,g_iClientHeight/2-99-87);
	m_tagVinyl.SetShowScale(173.0f/221.0f);

	m_tagAlbumBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_201));
	m_tagAlbumBk.SetShowScale(184.0f/228.0f);
	m_tagAlbumBk.SetPos(-g_iClientWidth/2+189,g_iClientHeight/2-94-92);

	m_tagAlbumArt.Initialize(IDC_MEDIA_TAG_ALBUMART,this,m_pSkinManager->GetTexture(TEXID_001),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbumArt.SetPos(-g_iClientWidth/2+189-5,g_iClientHeight/2-94-92);
	m_tagAlbumArt.SetShowScale(182.0f/256.0f);
	m_tagAlbumArt.Show(FALSE);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=-180;
	m_tagAlbumArt.SetIniViewState(&vs);

	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.2f);
	m_iconLoading.SetPos(0,20);

	SIZE siInfo2={g_iScreenWidth/2,40};

	m_iconUSB.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_251));
	m_iconUSB.SetPos(49+16,g_iClientHeight/2-97-16);
	m_iconUSB.Show(FALSE);

	m_iconSD.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_248));
	m_iconSD.SetPos(49+16,g_iClientHeight/2-97-16);
	m_iconSD.Show(FALSE);
	
	m_iconTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_206));
	m_iconTrack.SetPos(49+16,g_iClientHeight/2-147-16);

	m_iconArtist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_207));
	m_iconArtist.SetPos(49+16,g_iClientHeight/2-197-16);

	m_iconAlbum.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_214));	
	m_iconAlbum.SetPos(49+16,g_iClientHeight/2-247-16);


	COLORGL crb={1.0,1.0,1.0,1.0};
	SIZE siTagTitle = {300,40};
	SIZE siInfo={360,40};

	m_tagMediaStatus.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagMediaStatus.SetSize(&siInfo);
	m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
	m_tagMediaStatus.LinkTextGL(pTextGL);
	m_tagMediaStatus.SetPos(siTagTitle.cx/2,g_iClientHeight/2-97-16);
	m_tagMediaStatus.SetCaptionHeight(22);
	m_tagMediaStatus.EnableDrawBack(FALSE);
	m_tagMediaStatus.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagMediaStatus.SetCaptionColor(&crb);

	m_tagNowPlaying.Initialize(IDC_MEDIA_TAG_NOWPLAYINGTITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.LinkTextGL(pTextGL);
	m_tagNowPlaying.SetSize(&siTagTitle);
	m_tagNowPlaying.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-97-16);
	m_tagNowPlaying.SetCaptionHeight(22);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagNowPlaying.SetCaptionColor(&crb);
	m_tagNowPlaying.SetCaption(_T("0/0"));

	m_tagTitle.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetSize(&siTagTitle);
	m_tagTitle.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-147-16);
	m_tagTitle.SetCaptionHeight(22);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_tagTitle.SetCaption(m_title.String(),TRUE);

	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetSize(&siTagTitle);
	m_tagArtist.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-197-16);
	m_tagArtist.SetCaptionHeight(22);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crw);
	m_tagArtist.SetCaption(m_artist.String(),TRUE);

	m_tagAlbum.Initialize(IDC_MEDIA_TAG_ALBUM,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbum.LinkTextGL(pTextGL);
	m_tagAlbum.SetSize(&siTagTitle);
	m_tagAlbum.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-247-16);
	m_tagAlbum.SetCaptionHeight(22);
	m_tagAlbum.EnableDrawBack(FALSE);
	m_tagAlbum.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlbum.SetCaptionColor(&crw);
	m_tagAlbum.SetCaption(m_album.String(),TRUE);

	SIZE siTime={50,24};

	m_tagElapsedTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagElapsedTime.SetSize(&siTime);
	m_tagElapsedTime.SetCaption(_T("00:00"));
	m_tagElapsedTime.LinkTextGL(pTextGL);
	m_tagElapsedTime.SetPos(-g_iClientWidth/2+109,-g_iClientHeight/2+140);
	m_tagElapsedTime.SetCaptionHeight(18);
	m_tagElapsedTime.EnableDrawBack(FALSE);
	m_tagElapsedTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagElapsedTime.SetCaptionColor(&crw);

	m_tagRemainTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRemainTime.SetSize(&siTime);
	m_tagRemainTime.SetCaption(_T("00:00"));
	m_tagRemainTime.LinkTextGL(pTextGL);
	m_tagRemainTime.SetPos(g_iClientWidth/2-109,-g_iClientHeight/2+140);
	m_tagRemainTime.SetCaptionHeight(18);
	m_tagRemainTime.EnableDrawBack(FALSE);
	m_tagRemainTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagRemainTime.SetCaptionColor(&crw);

	//
	m_iconAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_084));
	m_iconAlert.SetShowScale(1.44f);
	m_iconAlert.SetPos(0,90);

	//
	m_btnPlayModeShuffle.Initialize(IDC_MEDIA_BTN_SHUFFLE,this,
		m_pSkinManager->GetTexture(TEXID_220),NULL,
		m_pSkinManager->GetTexture(TEXID_219),NULL,
		m_pSkinManager->GetTexture(TEXID_221),NULL,
		NULL,NULL);
	m_btnPlayModeShuffle.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlayModeShuffle.SetPos(g_iClientWidth/2-36,-g_iClientHeight/2+48);
	m_btnPlayModeShuffle.SetTouchRate(2.0,2.0);

	m_btnPlayModeRepeatOne.Initialize(IDC_MEDIA_BTN_REPEATONE,this,
		m_pSkinManager->GetTexture(TEXID_212),NULL,
		m_pSkinManager->GetTexture(TEXID_211),NULL,
		m_pSkinManager->GetTexture(TEXID_213),NULL,
		NULL,NULL);
	m_btnPlayModeRepeatOne.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlayModeRepeatOne.SetPos(g_iClientWidth/2-98,-g_iClientHeight/2+48);
	m_btnPlayModeRepeatOne.SetTouchRate(2.0,2.0);

	m_btnPlayModeRepeatAll.Initialize(IDC_MEDIA_BTN_REPEATALL,this,
		m_pSkinManager->GetTexture(TEXID_223),NULL,
		m_pSkinManager->GetTexture(TEXID_222),NULL,
		m_pSkinManager->GetTexture(TEXID_224),NULL,
		NULL,NULL);
	m_btnPlayModeRepeatAll.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlayModeRepeatAll.SetPos(g_iClientWidth/2-160,-g_iClientHeight/2+48);
	m_btnPlayModeRepeatAll.SetTouchRate(2.0,2.0);

	//home
	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_205),NULL,
		m_pSkinManager->GetTexture(TEXID_204),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+65,-g_iClientHeight/2+48);
	m_btnHome.SetTouchRate(2.0,2.0);

	//play control
	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,
		m_pSkinManager->GetTexture(TEXID_209),NULL,
		m_pSkinManager->GetTexture(TEXID_208),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlay.SetPos(0,-g_iClientHeight/2+48);
	m_btnPlay.SetTouchRate(2.0,2.0);

	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,
		m_pSkinManager->GetTexture(TEXID_228),NULL,
		m_pSkinManager->GetTexture(TEXID_227),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPause.SetPos(0,-g_iClientHeight/2+48);
	m_btnPause.Show(FALSE);
	m_btnPause.SetTouchRate(2.0,2.0);

	m_btnPrev.Initialize(IDC_MEDIA_BTN_PREV,this,
		m_pSkinManager->GetTexture(TEXID_218),NULL,
		m_pSkinManager->GetTexture(TEXID_217),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPrev.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPrev.SetPos(-104,-g_iClientHeight/2+48);
	m_btnPrev.SetTouchRate(2.0,2.0);

	m_btnNext.Initialize(IDC_MEDIA_BTN_NEXT,this,
		m_pSkinManager->GetTexture(TEXID_226),NULL,
		m_pSkinManager->GetTexture(TEXID_225),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnNext.SetPos(104,-g_iClientHeight/2+48);
	m_btnNext.SetTouchRate(2.0,2.0);

	m_btnLibrary.Initialize(IDC_MEDIA_BTN_LIBRARY,this,
		m_pSkinManager->GetTexture(TEXID_216),NULL,
		m_pSkinManager->GetTexture(TEXID_215),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnLibrary.SetPos(-g_iClientWidth/2+145,-g_iClientHeight/2+48);
	m_btnLibrary.SetTouchRate(2.0,2.0);

	//slider
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=510;//520/*776*/;
	sli.size.cy=16;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_203;
	sli.idChannelTex=TEXID_202;
	sli.idThumbTex=TEXID_210;
	m_sliProgress.Initialize(IDC_MEDIA_SLI_PROGRESS,this,&sli,pSkinManager);
	m_sliProgress.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_sliProgress.SetRange(0,MEDIA_SILDER_LENGTH);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+140);
	m_sliProgress.SetAutoBackFill(TRUE);

	//
	SetVideoMode(m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory == MEDIA_VIDEO);
#else
	ShowBackground(FALSE);
	ShowStatusBar(TRUE);

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


	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	//
// 	m_tagBar.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_253));
// 	si=m_tagBar.GetTexture()->GetImageSize();
// 	m_tagBar.SetScale(GLfloat(g_iClientWidth)/si.cx,100.0f/si.cy);
// 	m_tagBar.SetPos(0,-(g_iClientHeight-100)/2);


	m_tagVinyl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_202));
	m_tagVinyl.SetPos(-180,30);
	m_tagVinyl.SetShowScale(0.7f);

	m_tagAlbumBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_203));
	m_tagAlbumBk.SetShowScale(0.7f);
	m_tagAlbumBk.SetPos(-245,30);

	m_tagAlbumArt.Initialize(IDC_MEDIA_TAG_ALBUMART,this,m_pSkinManager->GetTexture(TEXID_001),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbumArt.SetPos(-245.0f-4.9f,30);
	m_tagAlbumArt.SetShowScale(0.62f);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=-180;
	m_tagAlbumArt.SetIniViewState(&vs);


	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.2f);
	m_iconLoading.SetPos(0,20);

	//
	/*
	m_tagVideoBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_065),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagVideoBk.SetSize(VIDEO_WINDOW_CX+32,VIDEO_WINDOW_CY+32);
// 	m_tagVideoBk.SetPos(0,(g_iScreenHeight/2-VIDEO_WINDOW_PY-VIDEO_WINDOW_CY/2));
	m_tagVideoBk.SetSize(g_iScreenWidth,g_iScreenHeight-101);
	m_tagVideoBk.SetPos(0,50.5);
	m_tagVideoBk.Show(FALSE);
	*/

	//test
// 	m_tagAlbumArtRound.Initialize(IDC_MEDIA_TAG_ALBUMART,this,0,m_pSkinManager->GetTexture(TEXID_100),m_pSkinManager->GetTexture(TEXID_506));
// 	m_tagAlbumArtRound.SetPos(/*-250*/0,52);
// 	m_tagAlbumArtRound.SetShowScale(0.6875f);
// 	m_tagAlbumArtRound.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
// // 	m_tagAlbumArtRound.SetNotifyFunc(NULL,NULL);
// 	vs=m_tagAlbumArtRound.GetCurViewState();
// 	vs.fRotateX=-180;
// 	vs.fRotateZ=0;
// 	m_tagAlbumArtRound.SetIniViewState(&vs);

	SIZE siInfo={360,40};
	SIZE siInfo2={g_iScreenWidth/2,40};

	m_iconUSB.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_210));
	m_iconSD.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_212));

	m_iconUSB.SetPos(-60,g_iClientHeight/2-80);
	m_iconSD.SetPos(-60,g_iClientHeight/2-80);

	m_iconSD.Show(FALSE);
	m_iconUSB.Show(FALSE);

/*
	m_iconVideo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_108));
	m_iconAudio.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_116));
	m_iconVideo.SetShowScale(2.0f);
	m_iconAudio.SetShowScale(2.0f);

	m_iconVideo.SetPos(0,52);
	m_iconAudio.SetPos(0,52);
*/



	m_iconTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_283));
	m_iconArtist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_282));
	m_iconAlbum.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_281));
	m_iconTrack.SetShowScale(0.8f);
	m_iconArtist.SetShowScale(0.8f);
	m_iconAlbum.SetShowScale(0.8f);
	m_iconTrack.SetPos(-60,70);
	m_iconArtist.SetPos(-60,30);
	m_iconAlbum.SetPos(-60,-10);


	COLORGL crb={1.0,1.0,1.0,1.0};

	m_tagMediaStatus.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagMediaStatus.SetSize(&siInfo);
	m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
	m_tagMediaStatus.LinkTextGL(pTextGL);
	m_tagMediaStatus.SetPos(0,g_iClientHeight/2-80);
	m_tagMediaStatus.SetCaptionHeight(28);
	m_tagMediaStatus.EnableDrawBack(FALSE);
	m_tagMediaStatus.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagMediaStatus.SetCaptionColor(&crb);

	m_tagNowPlaying.Initialize(IDC_MEDIA_TAG_NOWPLAYINGTITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.SetSize(&siInfo2);
	m_tagNowPlaying.SetCaption(_T("0/0"));
	m_tagNowPlaying.LinkTextGL(pTextGL);
	m_tagNowPlaying.SetPos(g_iScreenWidth/4-20,g_iClientHeight/2-80);
	m_tagNowPlaying.SetCaptionHeight(25);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagNowPlaying.SetCaptionColor(&crb);

	m_tagTitle.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(160,70);
	m_tagTitle.SetCaptionHeight(23);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crb);
	m_tagTitle.SetCaption(m_title.String(),TRUE);

	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.SetSize(&siInfo);
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetPos(160,30);
	m_tagArtist.SetCaptionHeight(23);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crb);
	m_tagArtist.SetCaption(m_artist.String(),TRUE);

	m_tagAlbum.Initialize(IDC_MEDIA_TAG_ALBUM,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbum.SetSize(&siInfo);
	m_tagAlbum.LinkTextGL(pTextGL);
	m_tagAlbum.SetPos(160,-10);
	m_tagAlbum.SetCaptionHeight(23);
	m_tagAlbum.EnableDrawBack(FALSE);
	m_tagAlbum.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlbum.SetCaptionColor(&crb);
	m_tagAlbum.SetCaption(m_album.String(),TRUE);

	SIZE siTime={140,32};

	m_tagElapsedTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagElapsedTime.SetSize(&siTime);
	m_tagElapsedTime.SetCaption(_T("00:00"));
	m_tagElapsedTime.LinkTextGL(pTextGL);
	m_tagElapsedTime.SetPos(-g_iClientWidth/2+70,-g_iClientHeight/2+120);
	m_tagElapsedTime.SetCaptionHeight(22);
	m_tagElapsedTime.EnableDrawBack(FALSE);
	m_tagElapsedTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagElapsedTime.SetCaptionColor(&crb);

	m_tagRemainTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRemainTime.SetSize(&siTime);
	m_tagRemainTime.SetCaption(_T("00:00"));
	m_tagRemainTime.LinkTextGL(pTextGL);
	m_tagRemainTime.SetPos(g_iClientWidth/2-70,-g_iClientHeight/2+120);
	m_tagRemainTime.SetCaptionHeight(22);
	m_tagRemainTime.EnableDrawBack(FALSE);
	m_tagRemainTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagRemainTime.SetCaptionColor(&crb);


	//
	m_iconAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_084));
	m_iconAlert.SetShowScale(1.44f);
	m_iconAlert.SetPos(0,90);



	//
	m_btnPlayModeRepeatAll.Initialize(IDC_MEDIA_BTN_REPEATALL,this,m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_243),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeRepeatAll.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlayModeRepeatAll.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+40);
	m_btnPlayModeRepeatAll.SetTouchRate(2.0,2.0);
    m_btnPlayModeRepeatAll.SetShowScale(0.8f);
	m_btnPlayModeRepeatAll.EnableClickAni(TRUE);
	m_btnPlayModeRepeatAll.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnPlayModeRepeatOne.Initialize(IDC_MEDIA_BTN_REPEATONE,this,m_pSkinManager->GetTexture(TEXID_244),NULL,
		m_pSkinManager->GetTexture(TEXID_245),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeRepeatOne.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlayModeRepeatOne.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+40);
	m_btnPlayModeRepeatOne.SetTouchRate(2.0,2.0);
    m_btnPlayModeRepeatOne.SetShowScale(0.8f);
	m_btnPlayModeRepeatOne.Show(FALSE);
	m_btnPlayModeRepeatOne.EnableClickAni(TRUE);
	m_btnPlayModeRepeatOne.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnPlayModeShuffle.Initialize(IDC_MEDIA_BTN_SHUFFLE,this,m_pSkinManager->GetTexture(TEXID_246),NULL,
		m_pSkinManager->GetTexture(TEXID_247),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeShuffle.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlayModeShuffle.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+40);
	m_btnPlayModeShuffle.SetTouchRate(2.0,2.0);
    m_btnPlayModeShuffle.SetShowScale(0.8f);
	m_btnPlayModeShuffle.Show(FALSE);
	m_btnPlayModeShuffle.EnableClickAni(TRUE);
	m_btnPlayModeShuffle.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//home
	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+40);
	m_btnHome.SetTouchRate(2.0,2.0);
    m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//play control
	m_btnPrev.Initialize(IDC_MEDIA_BTN_PREV,this,m_pSkinManager->GetTexture(TEXID_236),NULL,
		m_pSkinManager->GetTexture(TEXID_237),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPrev.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPrev.SetPos(-(g_iClientWidth/2-80)/2,-g_iClientHeight/2+40);
	m_btnPrev.SetTouchRate(2.0,2.0);
    m_btnPrev.SetShowScale(0.8f);
	m_btnPrev.EnableClickAni(TRUE);
	m_btnPrev.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnNext.Initialize(IDC_MEDIA_BTN_NEXT,this,m_pSkinManager->GetTexture(TEXID_240),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnNext.SetPos((g_iClientWidth/2-80)/2,-g_iClientHeight/2+40);
	m_btnNext.SetTouchRate(2.0,2.0);
    m_btnNext.SetShowScale(0.8f);
	m_btnNext.EnableClickAni(TRUE);
	m_btnNext.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnLibraryMusic.Initialize(IDC_MEDIA_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_231),NULL,
		m_pSkinManager->GetTexture(TEXID_232),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibraryMusic.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnLibraryMusic.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-50);
	m_btnLibraryMusic.SetTouchRate(2.0,2.0);
    m_btnLibraryMusic.SetShowScale(0.8f);
	m_btnLibraryMusic.EnableClickAni(TRUE);
	m_btnLibraryMusic.Show(FALSE);
	m_btnLibraryMusic.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnLibraryVideo.Initialize(IDC_MEDIA_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_229),NULL,
		m_pSkinManager->GetTexture(TEXID_230),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibraryVideo.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnLibraryVideo.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-50);
	m_btnLibraryVideo.SetTouchRate(2.0,2.0);
    m_btnLibraryVideo.SetShowScale(0.8f);
	m_btnLibraryVideo.EnableClickAni(TRUE);
	m_btnLibraryVideo.Show(FALSE);
	m_btnLibraryVideo.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_233),NULL,
		m_pSkinManager->GetTexture(TEXID_234),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPlay.SetPos(0,-g_iClientHeight/2+40);
	m_btnPlay.SetTouchRate(2.0,2.0);
    m_btnPlay.SetShowScale(0.8f);
	m_btnPlay.EnableClickAni(TRUE);
	m_btnPlay.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,m_pSkinManager->GetTexture(TEXID_238),NULL,
		m_pSkinManager->GetTexture(TEXID_239),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_btnPause.SetPos(0,-g_iClientHeight/2+40);
	m_btnPause.Show(FALSE);
	m_btnPause.SetTouchRate(2.0,2.0);
    m_btnPause.SetShowScale(0.8f);
	m_btnPause.EnableClickAni(TRUE);
	m_btnPause.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));



	//slider
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=g_iClientWidth-280;//520/*776*/;
	sli.size.cy=16;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_052;
	sli.idChannelTex=TEXID_053;
	sli.idThumbTex=TEXID_051;
	m_sliProgress.Initialize(IDC_MEDIA_SLI_PROGRESS,this,&sli,pSkinManager);
	m_sliProgress.SetNotifyFunc(&CPanelMediaMain::OnNotify,this);
	m_sliProgress.SetRange(0,MEDIA_SILDER_LENGTH);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+120);
	m_sliProgress.SetAutoBackFill(TRUE);


	//
	SetVideoMode(m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory == MEDIA_VIDEO);
#endif
	return TRUE;
}

void CPanelMediaMain::SetRepeatCheck(BOOL bRepeat)
{
	//m_btnRepeat.SetStatus(bRepeat?BS_FOCUS:BS_UP);
}
void CPanelMediaMain::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{

}
void CPanelMediaMain::SetLoadingStatus(FILE_LOADING_STATUS status)
{
	RETAILMSG(DEBUG_MEDIA,(_T("**CPanelMediaMain SetLoadingStatus :%d\r\n"),status));

	//m_bEnableQuit = (status!=LOAD_STATUS_LOADING);

	//if(m_idLoadingStatus!=status)
	{
		m_idLoadingStatus = status;
		switch (status)
		{
		case LOAD_STATUS_FAILED_INVALID_FILE:
			m_tagMediaStatus.SetCaption(GetResourceString(IDS_NOWPLAYING_INVALIDMEDIA));
			break;
		case LOAD_STATUS_FAILED_NO_FILE:
			m_tagMediaStatus.SetCaption(GetResourceString(IDS_NO_MEDIA_FILES));
			break;
		case LOAD_STATUS_LOADING:
			m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
			m_bAutoResumeFlag = TRUE;
			break;
		default:
			m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
			break;
		}
	}
}
void CPanelMediaMain::SetBrowsingStatus(BOOL bBrowsing)
{
	m_bBrowsing = bBrowsing;

// 	m_tagAlbumArt.Show(!bBrowsing && !m_bVideoMode);
//
// 	m_tagTitle.Show(!bBrowsing && !m_bVideoMode);
// 	m_tagArtist.Show(!bBrowsing && !m_bVideoMode);
// 	m_tagNowPlaying.Show(!bBrowsing && !m_bVideoMode);

	m_sliProgress.EnableNotify(!bBrowsing);
	m_tagRemainTime.EnableNotify(!bBrowsing);
	m_tagElapsedTime.EnableNotify(!bBrowsing);

	m_btnPlayModeShuffle.EnableNotify(!bBrowsing);
	m_btnPlayModeRepeatAll.EnableNotify(!bBrowsing);
	m_btnPlayModeRepeatOne.EnableNotify(!bBrowsing);

	m_btnPlay.EnableNotify(!bBrowsing);
	m_btnPause.EnableNotify(!bBrowsing);
	m_btnNext.EnableNotify(!bBrowsing);
	m_btnPrev.EnableNotify(!bBrowsing);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_btnLibrary.EnableNotify(!bBrowsing);
#else
	m_btnLibraryMusic.Show(g_bMediaModuleLoaded&&!bBrowsing&&!m_bVideoMode);
	m_btnLibraryVideo.Show(g_bMediaModuleLoaded&&!bBrowsing&&m_bVideoMode);
#endif
// 	m_btnHome.EnableNotify(!bBrowsing);

// 	m_tagTitleBrowsing.Show(bBrowsing);

	if(bBrowsing)
	{
		m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
	}
	else
	{

	}
}
void CPanelMediaMain::EnableOperate(BOOL bEnable)
{
	//m_bEnableQuit = bEnable;

	m_bEnableOperate = bEnable;

	//m_btnHome.EnableNotify(bEnable);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_btnLibrary.EnableNotify(bEnable);
#else
	m_btnLibraryMusic.EnableNotify(bEnable);
	m_btnLibraryVideo.EnableNotify(bEnable);
#endif
	m_btnPlay.EnableNotify(bEnable);
	m_btnPause.EnableNotify(bEnable);
	m_btnPrev.EnableNotify(bEnable);
	m_btnNext.EnableNotify(bEnable);
	m_btnPlayModeShuffle.EnableNotify(bEnable);
	m_btnPlayModeRepeatAll.EnableNotify(bEnable);
	m_btnPlayModeRepeatOne.EnableNotify(bEnable);
	m_sliProgress.EnableNotify(bEnable);

	m_tagRemainTime.EnableNotify(bEnable);
	m_tagElapsedTime.EnableNotify(bEnable);
}

void CPanelMediaMain::ShowLibraryBtn(BOOL bShow)
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_btnLibrary.EnableNotify(bShow);
#else
	m_btnLibraryMusic.Show(bShow&&g_bMediaModuleLoaded&&!m_bBrowsing&&!m_bVideoMode);
	m_btnLibraryVideo.Show(bShow&&g_bMediaModuleLoaded&&!m_bBrowsing&&m_bVideoMode);
#endif
}
void CPanelMediaMain::SetPlayingStatus(BOOL bPlaying)
{
	if(m_bPlayingFlag != bPlaying)
	{
		m_bPlayingFlag=bPlaying;
		m_btnPlay.Show(!bPlaying);
		m_btnPause.Show(bPlaying);

		//update widget:
		BOOL bShow=(!bPlaying);
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PLAY,UIOPRT_SHOWHIDE,&bShow);
		bShow=!bShow;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PAUSE,UIOPRT_SHOWHIDE,&bShow);

		//
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
		{
			if(m_pSmartBarMedia)
			{
				m_pSmartBarMedia->SetPlayingFlag(bPlaying);
			}
		}
	}
}

void CPanelMediaMain::SetMediaSrcType(UINT idType)
{
	//最简单的改法
	if(idType == MEDIA_TYPE_FLASH)
		idType = MEDIA_TYPE_USB1;

	m_idCurMediaType = idType;

	if(m_pSmartBarMedia)
	{
		if(m_idCurMediaType == MEDIA_TYPE_USB1 || m_idCurMediaType == MEDIA_TYPE_USB2)
		{
			m_pSmartBarMedia->SetType(SB_MEDIATYPE_USB);
			m_pSmartBarMedia->SetTrackName(GetResourceString(IDS_TITLE_MEDIA));
		}
#ifdef USE_GPS_CARD_MEDIA_PLAY
		else if(m_idCurMediaType == MEDIA_TYPE_SD1 || m_idCurMediaType == MEDIA_TYPE_SD2)
#else
		else if(m_idCurMediaType == MEDIA_TYPE_SD1)
#endif
		{
			m_pSmartBarMedia->SetType(SB_MEDIATYPE_SD);
			m_pSmartBarMedia->SetTrackName(GetResourceString(IDS_TITLE_MEDIA));
		}
// 		else if(m_idCurMediaType == MEDIA_TYPE_A2DP)
// 		{
// 			m_pSmartBarMedia->SetType(SB_MEDIATYPE_A2DP);
// 			m_pSmartBarMedia->SetTrackName(GetResourceString(IDS_TITLE_BLUETOOTH));
// 		}
// 		else if(m_idCurMediaType == MEDIA_TYPE_IPOD)
// 		{
// 			m_pSmartBarMedia->SetType(SB_MEDIATYPE_IPOD);
// 			m_pSmartBarMedia->SetTrackName(GetResourceString(IDS_TITLE_IPOD));
// 		}
	}
	//更新艺术专辑
	UpdateAlbumArt();

	if(m_pSkinManager)
	{
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,(LPVOID)m_idCurMediaType);
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
#else
	m_btnLibraryMusic.Show(g_bMediaModuleLoaded&&!m_bBrowsing&&!m_bVideoMode);
	m_btnLibraryVideo.Show(g_bMediaModuleLoaded&&!m_bBrowsing&&m_bVideoMode);
#endif
	if(!idType)
	{
		m_tagMediaStatus.SetCaption(GetResourceString(IDS_NO_MEDIA_DEVICE));
		return;
	}

	//正在播放的歌曲
	if (idType == MEDIA_TYPE_USB1)
	{
		m_tagNowPlaying.SetCaption(_T("--/--"));
		m_iconUSB.Show(TRUE);
		m_iconSD.Show(FALSE);
	}
	else if (idType == MEDIA_TYPE_USB2)
	{
		m_tagNowPlaying.SetCaption(_T("--/--"));
		m_iconUSB.Show(TRUE);
		m_iconSD.Show(FALSE);
	}
	else if (idType == MEDIA_TYPE_SD1)
	{
		m_tagNowPlaying.SetCaption(_T("--/--"));
		m_iconSD.Show(TRUE);
		m_iconUSB.Show(FALSE);
	}
	else
	{
		m_tagNowPlaying.SetCaption(_T("--/--"));
		m_iconSD.Show(FALSE);
		m_iconUSB.Show(FALSE);
	}
}
void CPanelMediaMain::SetShuffleCheck(BOOL bShuffle)
{
// 	m_btnShuffle.SetStatus(bShuffle?BS_FOCUS:BS_UP);
}
void CPanelMediaMain::SetPlayMode(UINT idPlayMode)
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_btnPlayModeRepeatAll.SetStatus(idPlayMode == MEDIA_PLAY_MODE_REPEAT_ALL?BS_FOCUS:BS_UP);
	m_btnPlayModeRepeatOne.SetStatus(idPlayMode == MEDIA_PLAY_MODE_REPEAT_ONE?BS_FOCUS:BS_UP);
	m_btnPlayModeShuffle.SetStatus(idPlayMode == MEDIA_PLAY_MODE_SHUFFLE?BS_FOCUS:BS_UP);
#else
	m_btnPlayModeRepeatAll.Show(idPlayMode == 0);
	m_btnPlayModeRepeatOne.Show(idPlayMode == 1);
	m_btnPlayModeShuffle.Show(idPlayMode == 2);
#endif
}
void CPanelMediaMain::UpdateAlbumArt()
{
	static BOOL bUser=FALSE;

	if(m_bNeedUpdateAlbumArt)
	{
		m_bNeedUpdateAlbumArt = FALSE;

		if(m_pBuffAlbumArt && m_dwAlbumArtSize)
		{
			m_bHasAlbumArt = m_pSkinManager->ReloadFromImageBuff(TEXID_001,m_pBuffAlbumArt,m_dwAlbumArtSize,0);
			bUser=TRUE;

			//clear:
			delete [] m_pBuffAlbumArt;
			m_pBuffAlbumArt = NULL;
			m_dwAlbumArtSize = 0;
		}
		else if(bUser)
		{
			m_bHasAlbumArt = FALSE;
			bUser=FALSE;
	 		m_pSkinManager->ReloadTextureFromSkin(TEXID_001,m_pSkinManager->GetConfig()->GetCurParam()->strSkinFileName.String());
		}

		if(m_pSkinManager)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ALBUMART,UIOPRT_SETVALUE,&m_bHasAlbumArt);
		}
	}

}
void CPanelMediaMain::SetVideoFullScreen(BOOL bFullScreen)
{
	if(m_bVideoMode)
	{
		m_bVideoFullScreen = bFullScreen;

	}
}
void CPanelMediaMain::SetVideoMode(BOOL bVideo)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: m_bVideoMode = %d\r\n"), TEXT(__FUNCTION__), bVideo));
	m_bVideoMode = bVideo;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+(bVideo?120:140));
	m_tagElapsedTime.SetPos(-g_iClientWidth/2+109,-g_iClientHeight/2+(bVideo?120:140));
	m_tagRemainTime.SetPos(g_iClientWidth/2-109,-g_iClientHeight/2+(bVideo?120:140));
	m_tagMediaStatus.SetPos(bVideo?0:300/2,bVideo?g_iClientHeight/2-80:g_iClientHeight/2-97-16);
#else
	m_btnLibraryMusic.Show(g_bMediaModuleLoaded&&!m_bBrowsing&&!m_bVideoMode);
	m_btnLibraryVideo.Show(g_bMediaModuleLoaded&&!m_bBrowsing&&m_bVideoMode);
#endif 

	m_tagAlbumArt.Show(!m_bVideoMode);
	m_tagAlbumBk.Show(!m_bVideoMode);
	m_tagVinyl.Show(!m_bVideoMode);

	m_tagNowPlaying.SetCaption(_T("--/--"));
	m_tagNowPlaying.Show(!m_bVideoMode);

	m_tagTitle.Show(!m_bVideoMode);
	m_tagArtist.Show(!m_bVideoMode);
	m_tagAlbum.Show(!m_bVideoMode);

	m_iconTrack.Show(!m_bVideoMode);
	m_iconAlbum.Show(!m_bVideoMode);
	m_iconArtist.Show(!m_bVideoMode);

	//m_tagVideoBk.Show(m_bVideoMode);

	if(m_pSkinManager)
	{
		BOOL bshow=!bVideo;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ALBUMART,UIOPRT_SHOWHIDE,&bshow);
	}

	//Video mode should not show status bar
	ShowStatusBar(!m_bVideoMode);
}

void CPanelMediaMain::OnPrepareUpdateAlbumArt(const unsigned char *pBuff,DWORD dwSize)
{
	m_bNeedUpdateAlbumArt = TRUE;

	if(pBuff)
	{
		if(m_pBuffAlbumArt)
		{
			delete [] m_pBuffAlbumArt;
			m_pBuffAlbumArt = NULL;
		}

		m_pBuffAlbumArt = new unsigned char[dwSize];
		memcpy(m_pBuffAlbumArt,pBuff,dwSize);
		m_dwAlbumArtSize = dwSize;
	}
	else
	{
		if(m_pBuffAlbumArt)
		{
			delete [] m_pBuffAlbumArt;
			m_pBuffAlbumArt = NULL;
		}

		m_dwAlbumArtSize = 0;
	}
}

void CALLBACK CPanelMediaMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelMediaMain *panel=(CPanelMediaMain*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelMediaMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(1, (TEXT("CPanelMediaMain::HandleControlMSG idControl[%d], wMsg[%d], wPara[%d].\r\n"),idControl, wMsg, wPara));
	switch (idControl)
	{
	case IDC_MEDIA_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
		//切换到播放列表页面
	case IDC_MEDIA_BTN_LIBRARY:
		{
			SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY);
			PlaySoundEffect(0);
		}
		break;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	case IDC_MEDIA_BTN_REPEATALL:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAYMODE_REPEAT_ALL);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_REPEATONE:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAYMODE_REPEAT_ONE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_SHUFFLE:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAYMODE_SHUFFLE);
			PlaySoundEffect(0);
		}
		break;
#else
	case IDC_MEDIA_BTN_REPEATALL:
	case IDC_MEDIA_BTN_REPEATONE:
	case IDC_MEDIA_BTN_SHUFFLE:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAYMODE_TOGGLE);
			PlaySoundEffect(0);
		}
		break;
#endif
	case IDC_MEDIA_BTN_PLAY:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAY);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PAUSE:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PAUSE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_NEXT:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_NEXT);
			g_dUIScrollBeginTime = timeGetTime();
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PREV:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PREVIOUS);
			g_dUIScrollBeginTime = timeGetTime();
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_SLI_PROGRESS:
		{
			SendMediaCommand(MEDIA_CMD_SET_POSITION,wPara);
			PlaySoundEffect(0);
		}
		break;
	default:
		break;
	}
}
void CPanelMediaMain::SetRemainTime(int iSet)
{
	m_iRemainTime = iSet;

	wchar_t str[16];
	int h,m,s;
	h=m_iRemainTime/3600;
	m=m_iRemainTime/60;//-h*60;
	s=m_iRemainTime-m*60;//-h*3600;
	swprintf_s(str,16,_T("%02d:%02d"),m,s);

	m_tagRemainTime.SetCaption(str);
}
void CPanelMediaMain::SetElapsedTime(int iSet)
{
	m_iElapsedTime = iSet;

	wchar_t str[16];
	int h,m,s;
	h=m_iElapsedTime/3600;
	m=m_iElapsedTime/60;//-h*60;
	s=m_iElapsedTime-m*60;//-h*3600;
	swprintf_s(str,16,_T("%02d:%02d"),m,s);

	m_tagElapsedTime.SetCaption(str);
}
void CPanelMediaMain::SetTitle(const wchar_t *pTitle)
{
	m_title = pTitle;
	m_tagTitle.SetCaption(m_title.String(),TRUE);
	if(m_pSmartBarMedia && (m_pSmartBarMedia->GetType()==SB_MEDIATYPE_SD||m_pSmartBarMedia->GetType()==SB_MEDIATYPE_USB))
	{
		m_pSmartBarMedia->SetTrackName(pTitle);
	}
	if(m_pSkinManager)
	{
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,(LPVOID)pTitle);
	}
}
void CPanelMediaMain::SetArtist(const wchar_t *pArtist)
{
	m_artist = pArtist;
	m_tagArtist.SetCaption(m_artist.String(),TRUE);
	if(m_pSkinManager)
	{
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)pArtist);
	}
}

void CPanelMediaMain::SetAlbum(const wchar_t *pAlbum)
{
	m_album = pAlbum;
	m_tagAlbum.SetCaption(m_album.String(),TRUE);
}
void CPanelMediaMain::SetNowPlayingTitle(const wchar_t *pNowPlayingTitle)
{
	m_tagNowPlaying.SetCaption(pNowPlayingTitle,TRUE);
	if(m_pSmartBarMedia && (m_pSmartBarMedia->GetType()==SB_MEDIATYPE_SD||m_pSmartBarMedia->GetType()==SB_MEDIATYPE_USB))
	{
		m_pSmartBarMedia->SetNowPlayingTitle(pNowPlayingTitle);
	}
}

void CPanelMediaMain::PlayInfoFlyOut(UINT idType)
{
	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN))
	{
		if(!m_bPlayInfoFlyOut)
		{
			m_bPlayInfoFlyOut = TRUE;
			m_idPlayInfoFlyOutType = idType;

#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
			GLfloat pos=(idType==1)?-1000:1000;
#else
			GLfloat pos=(idType==0)?-1000:1000;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagVinyl.SetPos(pos,g_iClientHeight/2-99-87,0,TRUE);
			m_tagAlbumBk.SetPos(pos,g_iClientHeight/2-94-92,0,TRUE);
			m_tagAlbumArt.SetPos(pos,g_iClientHeight/2-94-92,0,TRUE);

			m_iconUSB.SetPos(pos,g_iClientHeight/2-97-16,0,TRUE);
			m_iconSD.SetPos(pos,g_iClientHeight/2-97-16,0,TRUE);
			m_tagNowPlaying.SetPos(pos,g_iClientHeight/2-97-16,0,TRUE);

			m_iconTrack.SetPos(pos,g_iClientHeight/2-147-16,0,TRUE);
			m_iconArtist.SetPos(pos,g_iClientHeight/2-197-16,0,TRUE);
			m_iconAlbum.SetPos(pos,g_iClientHeight/2-247-16,0,TRUE);

			m_tagTitle.SetPos(pos,g_iClientHeight/2-147-16,0,TRUE);
			m_tagArtist.SetPos(pos,g_iClientHeight/2-197-16,0,TRUE);
			m_tagAlbum.SetPos(pos,g_iClientHeight/2-247-16,0,TRUE);
#else

			m_tagVinyl.SetPos(pos,30,0,TRUE);
            m_tagVinyl.SetShowScale(0.7f);
			m_tagAlbumBk.SetPos(pos,30,0,TRUE);
            m_tagAlbumBk.SetShowScale(0.7f);
			m_tagAlbumArt.SetPos(pos,30,0,TRUE);
            m_tagAlbumArt.SetShowScale(0.62f);

			m_iconTrack.SetPos(pos,70,0,TRUE);
			m_iconArtist.SetPos(pos,30,0,TRUE);
			m_iconAlbum.SetPos(pos,-10,0,TRUE);

			m_tagTitle.SetPos(pos,70,0,TRUE);
			m_tagArtist.SetPos(pos,30,0,TRUE);
			m_tagAlbum.SetPos(pos,-10,0,TRUE);
#endif
		}
	}
}
void CPanelMediaMain::PlayInfoFlyIn()
{
	if(m_bPlayInfoFlyOut)
	{
		m_bPlayInfoFlyOut = FALSE;

#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
		GLfloat pos=(m_idPlayInfoFlyOutType==1)?1000:-1000;
#else
		GLfloat pos=(m_idPlayInfoFlyOutType==0)?1000:-1000;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_tagVinyl.SetPos(pos,g_iClientHeight/2-99-87);
		m_tagAlbumBk.SetPos(pos,g_iClientHeight/2-94-92);
		m_tagAlbumArt.SetPos(pos,g_iClientHeight/2-94-92);

		m_iconUSB.SetPos(pos,g_iClientHeight/2-97-16);
		m_iconSD.SetPos(pos,g_iClientHeight/2-97-16);
		m_tagNowPlaying.SetPos(pos,g_iClientHeight/2-97-16);

		m_iconTrack.SetPos(pos,g_iClientHeight/2-147-16);
		m_iconArtist.SetPos(pos,g_iClientHeight/2-197-16);
		m_iconAlbum.SetPos(pos,g_iClientHeight/2-247-16);

		m_tagTitle.SetPos(pos,g_iClientHeight/2-147-16);
		m_tagArtist.SetPos(pos,g_iClientHeight/2-197-16);
		m_tagAlbum.SetPos(pos,g_iClientHeight/2-247-16);

		//reset
		m_tagVinyl.SetPos(-g_iClientWidth/2+268,g_iClientHeight/2-99-87,0,TRUE);
		m_tagAlbumBk.SetPos(-g_iClientWidth/2+189,g_iClientHeight/2-94-92,0,TRUE);
		m_tagAlbumArt.SetPos(-g_iClientWidth/2+189-5,g_iClientHeight/2-94-92,0,TRUE);

		m_iconUSB.SetPos(65,g_iClientHeight/2-97-16,0,TRUE);
		m_iconSD.SetPos(65,g_iClientHeight/2-97-16,0,TRUE);
		m_tagNowPlaying.SetPos(240,g_iClientHeight/2-97-16,0,TRUE);

		m_iconTrack.SetPos(65,g_iClientHeight/2-147-16,0,TRUE);
		m_iconArtist.SetPos(65,g_iClientHeight/2-197-16,0,TRUE);
		m_iconAlbum.SetPos(65,g_iClientHeight/2-247-16,0,TRUE);


		m_tagTitle.SetPos(240,g_iClientHeight/2-147-16,0,TRUE);
		m_tagArtist.SetPos(240,g_iClientHeight/2-197-16,0,TRUE);
		m_tagAlbum.SetPos(240,g_iClientHeight/2-248-16,0,TRUE);
#else

		m_tagVinyl.SetPos(pos,30);
        m_tagVinyl.SetShowScale(0.7f);
		m_tagAlbumBk.SetPos(pos,30);
        m_tagAlbumBk.SetShowScale(0.7f);
		m_tagAlbumArt.SetPos(pos,30);
        m_tagAlbumArt.SetShowScale(0.62f);

		m_iconTrack.SetPos(pos,70);
		m_iconArtist.SetPos(pos,30);
		m_iconAlbum.SetPos(pos,-10);

		m_tagTitle.SetPos(pos,70);
		m_tagArtist.SetPos(pos,30);
		m_tagAlbum.SetPos(pos,-10);

		//reset
		m_tagVinyl.SetPos(-180,30,0,TRUE);
		m_tagAlbumBk.SetPos(-245,30,0,TRUE);
		m_tagAlbumArt.SetPos(-245.0f-4.9f,30,0,TRUE);

		m_iconTrack.SetPos(-60,70,0,TRUE);
		m_iconArtist.SetPos(-60,30,0,TRUE);
		m_iconAlbum.SetPos(-60,-10,0,TRUE);

		m_tagTitle.SetPos(160,70,0,TRUE);
		m_tagArtist.SetPos(160,30,0,TRUE);
		m_tagAlbum.SetPos(160,-10,0,TRUE);

#endif
	}
}

void CPanelMediaMain::SetFileLoadFlag(BOOL bLoaded)
{
	m_bFileLoaded = bLoaded;
}
