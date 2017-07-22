#include "PanelBluetoothA2dp.h"
#include "../Media/MediaCommon.h"
#include "../../resource.h"

CPanelBluetoothA2dp *g_pBtA2DP=NULL;

//main:
CPanelBluetoothA2dp::CPanelBluetoothA2dp(void)
{
	m_iElapsedTime=0;
	m_iRemainTime=0;

	m_bPlayingFlag=FALSE;

	m_pBuffAlbumArt = NULL;
	m_dwAlbumArtSize = 0;
	m_bNeedUpdateAlbumArt = FALSE;
	m_bHasAlbumArt = FALSE;

	m_bAutoResumeFlag = TRUE;


	m_pSmartBarMedia = NULL;

	m_bA2dpConnected = FALSE;
	m_bAvrcpConnected = FALSE;

	m_iRemainTime=0;
	m_iElapsedTime=0;

	m_dwEnterTick = 0;

	m_bPlayInfoFlyOut = FALSE;
	m_idPlayInfoFlyOutType=0;
	m_bFirstTimeConnect = TRUE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_bShowVolMaxNote = TRUE;
	m_bVolMaxNoteShowed = FALSE;
#endif
}
CPanelBluetoothA2dp::~CPanelBluetoothA2dp(void)
{

}
BOOL CPanelBluetoothA2dp::IsReady()
{
	return CPanelGL::IsReady() && m_tagAlbumBk.IsReady();
}
void CPanelBluetoothA2dp::OnGesture(GESTUREINFO *pgi)
{
	if(!pgi)
		return;

	POINT pt={pgi->ptsLocation.x,pgi->ptsLocation.y};

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_bShowVolMaxNote = FALSE;
	m_bVolMaxNoteShowed = TRUE;
#endif

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
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PREVIOUS);
#else
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_NEXT);
#endif
					PlaySoundEffect(0);
					break;
				case ARG_SCROLL_RIGHT:
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_NEXT);
#else
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PREVIOUS);
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

void CPanelBluetoothA2dp::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY,PAGE_SWITCH_ANI_LEFT,MEDIA_TYPE_A2DP);
		break;
	}
}
void CPanelBluetoothA2dp::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	m_bAutoResumeFlag = (m_bPlayingFlag);
	RETAILMSG(DEBUG_BT, (TEXT("MSG:[%s]: m_bAutoResumeFlag:%d\r\n"),TEXT(__FUNCTION__),m_bAutoResumeFlag));

}
void CPanelBluetoothA2dp::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(DEBUG_BT, (TEXT("MSG:[%s]: idPreSRC:%X idCurSRC:%X\r\n"),TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->idPreSRC,m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC));

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

// 	MakeDefaultAni(/*ANIM_ROTATE_TOP_CLOCK_IN*/ANIM_LINE_RIGHT_IN);

#if 1
	//BUG 0002745 蓝牙页面Home切换或从Easyconnect切换卡顿一下
#if 0
	if (m_bFirstTimeConnect && m_bA2dpConnected)
	{
		SetShowType(m_bFirstTimeConnect);
	}
#endif


	if(GetAudioSRC() != SRC_AUDIO_ARM)
	{
		RETAILMSG(1,(_T("MSG:[%s]: SwitchMediaClassSRC & SwitchAudioSRC \r\n"),TEXT(__FUNCTION__)));
		SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);

		BOOL bA2dpOn=FALSE;
		SendBluetoothCommand(BT_CMD_GET_A2DP_STREAM,LPARAM(&bA2dpOn));

		//if a2dp is already playing,show a2dp sound directly:
		if(bA2dpOn)
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_ENABLE);
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
			SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
		}
	}
	else
	{ 
		if (m_bPlayingFlag && GetCurrentMuteStatus())
		{
			RETAILMSG(1,(_T("MSG:[%s]: =>MuteAudio\r\n"),TEXT(__FUNCTION__)));

			MuteAudio(FALSE,TRUE,_ARM_MUTE_ALL,TRUE,100);
		}
	}
	//Bug 0003483 播放蓝牙音乐，关ACC，开ACC，蓝牙开始播放后返回主页，再点击蓝牙音乐icon，蓝牙音乐没有声音，点击一下音量区，声音恢复
	SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,MEDIA_TYPE_A2DP);
	SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP));
#else

		BOOL bA2dpOn=FALSE;
		SendBluetoothCommand(BT_CMD_GET_A2DP_STREAM,LPARAM(&bA2dpOn));

		if(bA2dpOn)
		{
			SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP));
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
		}
#endif


	//notify src:
	::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_A2DP,0x40);

	//

	m_bAutoResumeFlag = TRUE;

	m_dwEnterTick = GetTickCount();

	//
	if(m_pSmartBarMedia)
	{
		m_pSmartBarMedia->SetType(SB_MEDIATYPE_A2DP);
		m_pSmartBarMedia->SetNowPlayingTitle(_T(""));
		m_pSmartBarMedia->SetTrackName(GetResourceString(IDS_DEVICE_BT));
	}

	//

	if(m_pSkinManager)
	{
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,LPVOID(MEDIA_TYPE_A2DP));
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,(LPVOID)m_tagTitle.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)m_tagArtist.GetCaption());
	}


	m_btnLibrary.EnableNotify(FALSE);

	//
// 	if(m_bPlayingFlag && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)!=UI_CLASS_BT)
// 	{
// 		SendBluetoothCommand(BT_CMD_MUTE,0);
// 		SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
// 	}
}
void CPanelBluetoothA2dp::Render()
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_229):NULL;
#else
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
#endif
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

	COLORGL cr_w={1.0,1.0,1.0,m_fCurAlpha};
	COLORGL crb={0.03125,0.03125,0.03125,m_fCurAlpha};


// 	//update playtime:
// 	if(/*m_bHavePlayTimeInfo &&*/ m_PlaySate==apPlaying)
// 	{
// 		static DWORD dwLastTick=0;
// 		if(GetTickCount() - dwLastTick >1000)
// 		{
// 			dwLastTick = GetTickCount();
// 			g_BTModule.pApi->apiAvrcp.GetPlayTime();
// 		}
// 	}

	GLfloat fbackLumin=m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance;

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

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	//bar:
	DrawRect(pTexBar,0,(-g_iClientHeight/2+48),800,96,2,&cr,TRUE);
#else
	DrawRect(pTexBar,0,(-g_iClientHeight/2+40),g_iClientWidth-4,80,2,&cr,TRUE);
#endif

	if(m_bPlayInfoFlyOut)
	{
		if(IsReady())
		{
			PlayInfoFlyIn();
		}
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBackEq.SetAlpha(m_fCurAlpha);
	m_tagBackEq.Render();
#else
	//
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
#endif

	//UpdateAlbumArt();

	m_tagVinyl.SetAlpha(m_fCurAlpha);
	m_tagVinyl.Render();

	m_tagAlbumBk.SetAlpha(m_fCurAlpha);
	m_tagAlbumBk.Render();

	m_tagAlbumArt.Show(m_bHasAlbumArt /*&& IsAniReady()*/);
	m_tagAlbumArt.SetAlpha(m_fCurAlpha);
	m_tagAlbumArt.Render();


	if(!m_bA2dpConnected)
	{
		m_tagNowPlaying.SetAlpha(m_fCurAlpha);
		m_tagNowPlaying.Render();
	}
	else
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_tagNowPlaying.SetAlpha(m_fCurAlpha);
		m_tagNowPlaying.Render();
#endif
		m_iconBT.SetAlpha(m_fCurAlpha);
		m_iconBT.Render();
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

	//common controls
	/*
	m_btnPlayModeRepeatAll.SetAlpha(m_fCurAlpha);
	m_btnPlayModeRepeatAll.Render();

	m_btnPlayModeRepeatOne.SetAlpha(m_fCurAlpha);
	m_btnPlayModeRepeatOne.Render();

	m_btnPlayModeShuffle.SetAlpha(m_fCurAlpha);
	m_btnPlayModeShuffle.Render();
	*/


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

	m_btnLibrary.SetAlpha(m_fCurAlpha);
	m_btnLibrary.Render();


	//auto resume:
	if(IsAniReady() && GetTickCount()-m_dwEnterTick>500)
	{
		if(m_bAutoResumeFlag)
		{
			RETAILMSG(1, (TEXT("MSG:[%s]: auto resume m_bPlayingFlag:%d\r\n"), TEXT(__FUNCTION__),m_bPlayingFlag));

			if(m_bA2dpConnected)
			{
				if(!m_bPlayingFlag)
				{
					//whh 自动播放时强制enable a2dp及打开a2dp音量
					RETAILMSG(1, (TEXT("MSG:[%s]: A2DP_CONTROL_PLAY \r\n"), TEXT(__FUNCTION__)));
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_ENABLE);
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
					SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
				}
			}
			else
			{
				//20160804
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			}
			m_bAutoResumeFlag = FALSE;
		}
		m_btnLibrary.EnableNotify(TRUE);
	}

	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	//show max volume note one time.
	if(GetTickCount()-m_dwEnterTick < 10000 && GetTickCount()-m_dwEnterTick > 1000)
	{
		if(m_bShowVolMaxNote)
		{
			m_bVolMaxNoteShowed = TRUE;
			m_tagVolMaxNote.SetAlpha(m_fCurAlpha);
			m_tagVolMaxNote.Render();
		}
	}
	else if(m_bVolMaxNoteShowed)
	{
		m_bShowVolMaxNote = FALSE;
	}
#endif

	EndRender();

}
BOOL CPanelBluetoothA2dp::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	g_pBtA2DP = this;

	ShowBackground(FALSE);
	ShowStatusBar(TRUE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
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

	//m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	//m_iconLoading.SetShowScale(1.2f);
	//m_iconLoading.SetPos(0,20);

	SIZE siInfo2={g_iScreenWidth/2,40};

	m_iconBT.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_298));
	m_iconBT.SetPos(49+16,g_iClientHeight/2-97-16);

	m_iconTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_206));
	m_iconArtist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_207));
	m_iconAlbum.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_214));
	m_iconTrack.SetPos(49+16,g_iClientHeight/2-147-16);
	m_iconArtist.SetPos(49+16,g_iClientHeight/2-197-16);
	m_iconAlbum.SetPos(49+16,g_iClientHeight/2-247-16);


	COLORGL crb={1.0,1.0,1.0,1.0};
	SIZE siTagTitle = {300,40};
	//m_tagMediaStatus.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	//m_tagMediaStatus.SetSize(&siInfo);
	//m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
	//m_tagMediaStatus.LinkTextGL(pTextGL);
	//m_tagMediaStatus.SetPos(0,g_iClientHeight/2-80);
	//m_tagMediaStatus.SetCaptionHeight(22);
	//m_tagMediaStatus.EnableDrawBack(FALSE);
	//m_tagMediaStatus.SetCaptionPos(BTN_CAPTION_CENTER);
	//m_tagMediaStatus.SetCaptionColor(&crb);

	m_tagNowPlaying.Initialize(IDC_MEDIA_TAG_NOWPLAYINGTITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.LinkTextGL(pTextGL);
	m_tagNowPlaying.SetSize(&siTagTitle);
	m_tagNowPlaying.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-97-16);
	m_tagNowPlaying.SetCaptionHeight(22);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagNowPlaying.SetCaptionColor(&crb);
	m_tagNowPlaying.SetCaption(GetResourceString(IDS_TITLE_BLUETOOTH));

	m_tagTitle.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetSize(&siTagTitle);
	m_tagTitle.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-147-16);
	m_tagTitle.SetCaptionHeight(22);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_tagTitle.SetCaption(GetResourceString(IDS_LIBRARY_MENU_TRACKS));

	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetSize(&siTagTitle);
	m_tagArtist.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-197-16);
	m_tagArtist.SetCaptionHeight(22);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crw);
	m_tagArtist.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));

	m_tagAlbum.Initialize(IDC_MEDIA_TAG_ALBUM,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbum.LinkTextGL(pTextGL);
	m_tagAlbum.SetSize(&siTagTitle);
	m_tagAlbum.SetPos(90+siTagTitle.cx/2,g_iClientHeight/2-247-16);
	m_tagAlbum.SetCaptionHeight(22);
	m_tagAlbum.EnableDrawBack(FALSE);
	m_tagAlbum.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlbum.SetCaptionColor(&crw);
	m_tagAlbum.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));

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

	//home
	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_205),NULL,
		m_pSkinManager->GetTexture(TEXID_204),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+65,-g_iClientHeight/2+48);
	m_btnHome.SetTouchRate(2.0,2.0);

	//play control
	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,
		m_pSkinManager->GetTexture(TEXID_209),NULL,
		m_pSkinManager->GetTexture(TEXID_208),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPlay.SetPos(0,-g_iClientHeight/2+48);
	m_btnPlay.SetTouchRate(2.0,2.0);

	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,
		m_pSkinManager->GetTexture(TEXID_228),NULL,
		m_pSkinManager->GetTexture(TEXID_227),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPause.SetPos(0,-g_iClientHeight/2+48);
	m_btnPause.Show(FALSE);
	m_btnPause.SetTouchRate(2.0,2.0);

	m_btnPrev.Initialize(IDC_MEDIA_BTN_PREV,this,
		m_pSkinManager->GetTexture(TEXID_218),NULL,
		m_pSkinManager->GetTexture(TEXID_217),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPrev.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPrev.SetPos(-104,-g_iClientHeight/2+48);
	m_btnPrev.SetTouchRate(2.0,2.0);

	m_btnNext.Initialize(IDC_MEDIA_BTN_NEXT,this,
		m_pSkinManager->GetTexture(TEXID_226),NULL,
		m_pSkinManager->GetTexture(TEXID_225),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnNext.SetPos(104,-g_iClientHeight/2+48);
	m_btnNext.SetTouchRate(2.0,2.0);

	m_btnLibrary.Initialize(IDC_MEDIA_BTN_LIBRARY,this,
		m_pSkinManager->GetTexture(TEXID_216),NULL,
		m_pSkinManager->GetTexture(TEXID_215),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
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
	//m_sliProgress.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_sliProgress.SetRange(0,MEDIA_SILDER_LENGTH);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+140);
	m_sliProgress.SetAutoBackFill(TRUE);

#else

	/*
	// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_200,_T("\\Windows\\media_bk.jpg"),g_iClientWidth,g_iClientHeight,FALSE,GL_NEAREST))
	if(!m_pSkinManager->AddTextureFromFile(TEXID_200,_T("\\Windows\\media_bk.jpg"),g_iClientWidth,500,FALSE,GL_NEAREST))
	{
		RETAILMSG(DEBUG_BT, (TEXT(" @@@@ TEXID_200 already exist\r\n")));
	}
	if(!m_pSkinManager->AddTextureFromFile(TEXID_299,_T("\\Windows\\media_bar.jpg"),g_iClientWidth,100,FALSE,GL_NEAREST))
	{
		RETAILMSG(DEBUG_BT, (TEXT(" @@@@ TEXID_299 already exist\r\n")));
	}
	*/

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

// 	m_tagBar.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_299));
// 	m_tagBar.SetPos(0,-(g_iClientHeight-100)/2);

	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	m_tagVinyl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_202));
	m_tagVinyl.SetPos(-180,30);
	m_tagVinyl.SetShowScale(0.7f);

	m_tagAlbumBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_203));
	m_tagAlbumBk.SetShowScale(0.7f);
	m_tagAlbumBk.SetPos(-245,30);

	m_tagAlbumArt.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_001),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbumArt.SetPos(-245.0f-4.9f,30);
// 	m_tagAlbumArt.SetScale(1.4,0.8);
	m_tagAlbumArt.SetShowScale(0.62f);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=-180;
// 	vs.fRotateY=-60;
// 	vs.fRotateZ=0;
	m_tagAlbumArt.SetIniViewState(&vs);
	//m_tagAlbumArt.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
// 	m_tagAlbumArt.Show(FALSE);


	//test
// 	m_tagAlbumArtRound.Initialize(IDC_MEDIA_TAG_ALBUMART,this,0,m_pSkinManager->GetTexture(TEXID_100),m_pSkinManager->GetTexture(TEXID_506));
// 	m_tagAlbumArtRound.SetPos(/*-250*/0,52);
// 	m_tagAlbumArtRound.SetShowScale(0.6875f);
// 	m_tagAlbumArtRound.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
// // 	m_tagAlbumArtRound.SetNotifyFunc(NULL,NULL);
// 	vs=m_tagAlbumArtRound.GetCurViewState();
// 	vs.fRotateX=-180;
// 	vs.fRotateZ=0;
// 	m_tagAlbumArtRound.SetIniViewState(&vs);

	SIZE siInfo={360,40};
	SIZE siInfo2={g_iScreenWidth/2,40};

	m_iconBT.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_208));
	m_iconBT.SetPos(/*-60*/0,g_iClientHeight/2-80);

/*
	m_iconVideo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_108));
	m_iconAudio.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_116));
	m_iconVideo.SetShowScale(2.0f);
	m_iconAudio.SetShowScale(2.0f);

	m_iconVideo.SetPos(0,52);
	m_iconAudio.SetPos(0,52);
*/


/*
	m_tagKeyBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_284));
// 	m_tagKeyBk.SetScale(g_iClientWidth/256.0f,1.0f);
	m_tagKeyBk.SetPos(0,-g_iClientHeight/2+64-27);
*/

	m_iconTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_283));
	m_iconArtist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_282));
	m_iconAlbum.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_281));
	m_iconTrack.SetShowScale(0.7f);
	m_iconArtist.SetShowScale(0.7f);
	m_iconAlbum.SetShowScale(0.7f);
	m_iconTrack.SetPos(-60,70);
	m_iconArtist.SetPos(-60,30);
	m_iconAlbum.SetPos(-60,-10);


	COLORGL crb={1.0,1.0,1.0,1.0};

// 	m_tagMediaStatus.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagMediaStatus.SetSize(&siInfo);
// 	m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
// 	m_tagMediaStatus.LinkTextGL(pTextGL);
// 	m_tagMediaStatus.SetPos(0,200);
// 	m_tagMediaStatus.SetCaptionHeight(36);
// 	m_tagMediaStatus.EnableDrawBack(FALSE);
// 	m_tagMediaStatus.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_tagMediaStatus.SetCaptionColor(&crb);

	m_tagNowPlaying.Initialize(IDC_MEDIA_TAG_NOWPLAYINGTITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.SetSize(&siInfo2);
	m_tagNowPlaying.SetCaption(GetResourceString(IDS_DISCONNECTED));
	m_tagNowPlaying.LinkTextGL(pTextGL);
	m_tagNowPlaying.SetCaptionHeight(25);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionColor(&crb);
	m_tagNowPlaying.SetPos(g_iScreenWidth/4-20,g_iClientHeight/2-80);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT);

	m_tagTitle.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(160,70);
	m_tagTitle.SetCaptionHeight(23);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crb);

	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.SetSize(&siInfo);
	m_tagArtist.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetPos(160,30);
	m_tagArtist.SetCaptionHeight(23);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crb);

	m_tagAlbum.Initialize(IDC_MEDIA_TAG_ALBUM,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbum.SetSize(&siInfo);
	m_tagAlbum.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
	m_tagAlbum.LinkTextGL(pTextGL);
	m_tagAlbum.SetPos(160,-10);
	m_tagAlbum.SetCaptionHeight(23);
	m_tagAlbum.EnableDrawBack(FALSE);
	m_tagAlbum.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlbum.SetCaptionColor(&crb);

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

	/*
	//
	m_btnPlayModeRepeatAll.Initialize(IDC_MEDIA_BTN_REPEATALL,this,m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_243),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeRepeatAll.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPlayModeRepeatAll.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+50);
	m_btnPlayModeRepeatAll.SetTouchRate(2.0,2.0);
	m_btnPlayModeRepeatAll.EnableClickAni(TRUE);
	m_btnPlayModeRepeatAll.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnPlayModeRepeatOne.Initialize(IDC_MEDIA_BTN_REPEATONE,this,m_pSkinManager->GetTexture(TEXID_244),NULL,
		m_pSkinManager->GetTexture(TEXID_245),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeRepeatOne.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPlayModeRepeatOne.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+50);
	m_btnPlayModeRepeatOne.SetTouchRate(2.0,2.0);
	m_btnPlayModeRepeatOne.Show(FALSE);
	m_btnPlayModeRepeatOne.EnableClickAni(TRUE);
	m_btnPlayModeRepeatOne.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnPlayModeShuffle.Initialize(IDC_MEDIA_BTN_SHUFFLE,this,m_pSkinManager->GetTexture(TEXID_246),NULL,
		m_pSkinManager->GetTexture(TEXID_247),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeShuffle.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPlayModeShuffle.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+50);
	m_btnPlayModeShuffle.SetTouchRate(2.0,2.0);
	m_btnPlayModeShuffle.Show(FALSE);
	m_btnPlayModeShuffle.EnableClickAni(TRUE);
	m_btnPlayModeShuffle.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	*/

	//home
	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
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
	m_btnPrev.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPrev.SetPos(-(g_iClientWidth/2-80)/2,-g_iClientHeight/2+40);
	m_btnPrev.SetTouchRate(2.0,2.0);
    m_btnPrev.SetShowScale(0.8f);
	m_btnPrev.EnableClickAni(TRUE);
	m_btnPrev.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnNext.Initialize(IDC_MEDIA_BTN_NEXT,this,m_pSkinManager->GetTexture(TEXID_240),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnNext.SetPos((g_iClientWidth/2-80)/2,-g_iClientHeight/2+40);
	m_btnNext.SetTouchRate(2.0,2.0);
    m_btnNext.SetShowScale(0.8f);
	m_btnNext.EnableClickAni(TRUE);
	m_btnNext.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnLibrary.Initialize(IDC_MEDIA_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_231),NULL,
		m_pSkinManager->GetTexture(TEXID_232),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnLibrary.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-50);
	m_btnLibrary.SetTouchRate(2.0,2.0);
    m_btnLibrary.SetShowScale(0.8f);
	m_btnLibrary.EnableClickAni(TRUE);
	m_btnLibrary.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_233),NULL,
		m_pSkinManager->GetTexture(TEXID_234),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPlay.SetPos(0,-g_iClientHeight/2+40);
	m_btnPlay.SetTouchRate(2.0,2.0);
    m_btnPlay.SetShowScale(0.8f);
	m_btnPlay.EnableClickAni(TRUE);
	m_btnPlay.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,m_pSkinManager->GetTexture(TEXID_238),NULL,
		m_pSkinManager->GetTexture(TEXID_239),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
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
	//m_sliProgress.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_sliProgress.SetRange(0,512);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+120);
	m_sliProgress.SetAutoBackFill(TRUE);


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	SIZE siNote={600,160};
	//COLORGL cr_blue={0.0f,0.58f,0.85f,1.0f};
	m_tagVolMaxNote.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_175),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagVolMaxNote.SetSize(&siNote);
	m_tagVolMaxNote.SetCaption(_T("MAKE SURE YOUR BLUETOOTH \nVOLUME IS TURNED TO MAX."));
	m_tagVolMaxNote.LinkTextGL(pTextGL);
	m_tagVolMaxNote.SetCaptionHeight(28);
	m_tagVolMaxNote.SetCaptionColor(&crb);
	m_tagVolMaxNote.SetPos(0,15);
	m_tagVolMaxNote.SetCaptionPos(BTN_CAPTION_CENTER,0);
#endif
#endif

	//
	return TRUE;
}

void CALLBACK CPanelBluetoothA2dp::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelBluetoothA2dp *panel=(CPanelBluetoothA2dp*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelBluetoothA2dp::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(DEBUG_BT,(_T("CPanelBluetoothA2dp::HandleControlMSG id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara));
	switch (idControl)
	{
	case IDC_MEDIA_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_LIBRARY:
		{
			SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY,PAGE_SWITCH_ANI_DEFAULT,MEDIA_TYPE_A2DP);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_REPEATALL:
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAYMODE_REPEATONE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_REPEATONE:
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAYMODE_SHUFFLE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_SHUFFLE:
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAYMODE_REPEATALL);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PLAY:
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
			//m_btnPlay.Enable(FALSE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PAUSE:
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PAUSE);
			//m_btnPause.Enable(FALSE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_NEXT:
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_NEXT);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PREV:
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PREVIOUS);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_SLI_PROGRESS:
		{

		}
		break;
	}
}
void CPanelBluetoothA2dp::OnA2dpConnected(BOOL bConnected)
{
	m_bA2dpConnected = bConnected;

	if(!bConnected)
	{
		//20160804
		m_sliProgress.SetValue(0);

		m_bFirstTimeConnect = TRUE;

		m_iRemainTime=0;
		m_iElapsedTime=0;

// 		m_btnPause.Enable(TRUE);
// 		m_btnPlay.Enable(TRUE);

		m_tagNowPlaying.SetCaption(GetResourceString(IDS_DISCONNECTED));

		m_tagTitle.SetCaption(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
		m_tagArtist.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
		m_tagAlbum.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));

		m_tagElapsedTime.SetCaption(_T("00:00"));
		m_tagRemainTime.SetCaption(_T("00:00"));

		//reset media widget:
		UINT media_type=MEDIA_TYPE_INVALID;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);
		if(media_type == MEDIA_TYPE_A2DP)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,(LPVOID)MEDIA_TYPE_INVALID);

			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,NULL);
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,NULL);

			//update widget:
			BOOL bShow=TRUE;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PLAY,UIOPRT_SHOWHIDE,&bShow);
			bShow=!bShow;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PAUSE,UIOPRT_SHOWHIDE,&bShow);
		}

		//quit to main menu:
		if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP))
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)== UI_CLASS_BT)
		{
			//m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC = 0;
			//media bar:
			if(m_pSmartBarMedia)
			{
				m_pSmartBarMedia->SetType(SB_MEDIATYPE_NULL);
				m_pSmartBarMedia->SetPlayingFlag(FALSE);
			}
		}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		m_bShowVolMaxNote = TRUE;
		m_bVolMaxNoteShowed = FALSE;
#endif

	}
}
void CPanelBluetoothA2dp::OnAvrcpConnected(BOOL bConnected)
{
	m_bAvrcpConnected = bConnected;

	if(!bConnected)
	{
// 		m_btnPause.Enable(TRUE);
// 		m_btnPlay.Enable(TRUE);

		m_sliProgress.SetValue(0);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_tagNowPlaying.SetCaption(GetResourceString(IDS_TITLE_BLUETOOTH));
#else
		m_tagNowPlaying.SetCaption(_T(""));
#endif
		WCHAR name[MAX_STRING];

		m_iRemainTime=0;
		m_iElapsedTime=0;

#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_TRACKS),GetResourceString(IDS_UNKNOWN));
#else
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
			GetResourceString(IDS_LIBRARY_MENU_TRACKS));
#endif
		m_tagTitle.SetCaption(name);

#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_TRACKS),GetResourceString(IDS_UNKNOWN));
#else
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
			GetResourceString(IDS_LIBRARY_MENU_TRACKS));
#endif
		m_tagArtist.SetCaption(name);
	}
}

void CPanelBluetoothA2dp::PlayInfoFlyOut(UINT idType)
{
	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP))
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

			m_iconBT.SetPos(pos,g_iClientHeight/2-97-16,0,TRUE);
			m_tagNowPlaying.SetPos(pos,g_iClientHeight/2-97-16,0,TRUE);

			m_iconTrack.SetPos(pos,g_iClientHeight/2-147-16,0,TRUE);
			m_iconArtist.SetPos(pos,g_iClientHeight/2-197-16,0,TRUE);
			m_iconAlbum.SetPos(pos,g_iClientHeight/2-247-16,0,TRUE);

			m_tagTitle.SetPos(pos,g_iClientHeight/2-147-16,0,TRUE);
			m_tagArtist.SetPos(pos,g_iClientHeight/2-197-16,0,TRUE);
			m_tagAlbum.SetPos(pos,g_iClientHeight/2-247-16,0,TRUE);
#else
			m_tagVinyl.SetPos(pos,30,0,TRUE);
			m_tagAlbumBk.SetPos(pos,30,0,TRUE);
			m_tagAlbumArt.SetPos(pos,30,0,TRUE);

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
void CPanelBluetoothA2dp::PlayInfoFlyIn()
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

		m_iconBT.SetPos(pos,g_iClientHeight/2-97-16);
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

		m_iconBT.SetPos(65,g_iClientHeight/2-97-16,0,TRUE);
		m_tagNowPlaying.SetPos(240,g_iClientHeight/2-97-16,0,TRUE);

		m_iconTrack.SetPos(65,g_iClientHeight/2-147-16,0,TRUE);
		m_iconArtist.SetPos(65,g_iClientHeight/2-197-16,0,TRUE);
		m_iconAlbum.SetPos(65,g_iClientHeight/2-247-16,0,TRUE);


		m_tagTitle.SetPos(240,g_iClientHeight/2-147-16,0,TRUE);
		m_tagArtist.SetPos(240,g_iClientHeight/2-197-16,0,TRUE);
		m_tagAlbum.SetPos(240,g_iClientHeight/2-248-16,0,TRUE);
#else
		m_tagVinyl.SetPos(pos,30);
		m_tagAlbumBk.SetPos(pos,30);
		m_tagAlbumArt.SetPos(pos,30);

		m_iconTrack.SetPos(pos,70);
		m_iconArtist.SetPos(pos,30);
		m_iconAlbum.SetPos(pos,-10);

		m_tagTitle.SetPos(pos,70);
		m_tagArtist.SetPos(pos,30);
		m_tagAlbum.SetPos(pos,-10);

		//reset
		m_tagVinyl.SetPos(-180,30,0,TRUE);
		m_tagAlbumBk.SetPos(-245,30,0,TRUE);
		m_tagAlbumArt.SetPos(-245-4.9f,30,0,TRUE);

		m_iconTrack.SetPos(-60,70,0,TRUE);
		m_iconArtist.SetPos(-60,30,0,TRUE);
		m_iconAlbum.SetPos(-60,-10,0,TRUE);

		m_tagTitle.SetPos(160,70,0,TRUE);
		m_tagArtist.SetPos(160,30,0,TRUE);
		m_tagAlbum.SetPos(160,-10,0,TRUE);
#endif
	}
}
void CPanelBluetoothA2dp::SetRemainTime(int iSet)
{
	if(iSet<0)
		iSet = 0;

	m_iRemainTime = iSet;

	wchar_t str[16];
	int h,m,s;
	h=m_iRemainTime/3600;
	m=m_iRemainTime/60;//-h*60;
	s=m_iRemainTime-m*60;//-h*3600;
	swprintf_s(str,16,_T("%02d:%02d"),m,s);

	m_tagRemainTime.SetCaption(str);
}
void CPanelBluetoothA2dp::SetElapsedTime(int iSet)
{
	if(iSet<0)
		iSet = 0;

	m_iElapsedTime = iSet;

	wchar_t str[16];
	int h,m,s;
	h=m_iElapsedTime/3600;
	m=m_iElapsedTime/60;//-h*60;
	s=m_iElapsedTime-m*60;//-h*3600;
	swprintf_s(str,16,_T("%02d:%02d"),m,s);

	m_tagElapsedTime.SetCaption(str);
}

void CPanelBluetoothA2dp::OnNotifyID3(strMusicID3 *id3info)
{
	if(wcslen(id3info->Title) != 0)
		m_tagTitle.SetCaption(id3info->Title,TRUE);
	else
		m_tagTitle.SetCaption(GetResourceString(IDS_UNKNOWN),TRUE);

	if(wcslen(id3info->Artist) != 0)
		m_tagArtist.SetCaption(id3info->Artist,TRUE);
	else
		m_tagArtist.SetCaption(GetResourceString(IDS_UNKNOWN),TRUE);


	if(wcslen(id3info->Album) != 0)
		m_tagAlbum.SetCaption(id3info->Album,TRUE);
	else
		m_tagAlbum.SetCaption(GetResourceString(IDS_UNKNOWN),TRUE);

	//update widget:
	UINT media_type=MEDIA_TYPE_INVALID;
	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);
	if(media_type == MEDIA_TYPE_A2DP || media_type == MEDIA_TYPE_INVALID)
	{
		if(media_type == MEDIA_TYPE_INVALID)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,(LPVOID)MEDIA_TYPE_A2DP);
		}
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,(LPVOID)m_tagTitle.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)m_tagArtist.GetCaption());

		if(m_pSmartBarMedia)
		{
			if(m_pSmartBarMedia->GetType() == SB_MEDIATYPE_NULL || m_pSmartBarMedia->GetType() == SB_MEDIATYPE_A2DP)
			{
				m_pSmartBarMedia->SetTrackName(m_tagTitle.GetCaption());
			}
		}
	}
}

void CPanelBluetoothA2dp::SetPlayState(PLAYER_STATE state)
{
	m_btnPlay.Show(state!=PLAYER_STATE_PLAY);
	m_btnPause.Show(state==PLAYER_STATE_PLAY);

	m_bPlayingFlag=(state==PLAYER_STATE_PLAY);
	RETAILMSG(1, (TEXT("MSG:[%s]: m_bPlayingFlag:%d \r\n"), TEXT(__FUNCTION__),m_bPlayingFlag));
	UINT media_type=MEDIA_TYPE_INVALID;

	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);

	if(media_type == MEDIA_TYPE_A2DP || media_type == MEDIA_TYPE_INVALID)
	{
		//update widget:
		BOOL bShow=(!m_bPlayingFlag);
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PLAY,UIOPRT_SHOWHIDE,&bShow);
		bShow=!bShow;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PAUSE,UIOPRT_SHOWHIDE,&bShow);
	}

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
	{
		if(m_pSmartBarMedia)
		{
			m_pSmartBarMedia->SetPlayingFlag(m_bPlayingFlag);
		}
	}
}

void CPanelBluetoothA2dp::SetShowType(BOOL bFirstTimeConnect)
{
	if (bFirstTimeConnect)
	{
		m_bFirstTimeConnect = FALSE;
		//SwitchPage(UI_CLASS_BT,PAGE_BT_WARNING);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY,PAGE_SWITCH_ANI_DEFAULT,MEDIA_TYPE_A2DP);
		SetTimer(m_hMainWnd,TIMER_BLUETOOTH_A2DP_WARNING,3000,OnTimerProc);
	}
}

void CPanelBluetoothA2dp::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: idEvent[%d]\r\n"),TEXT(__FUNCTION__),idEvent));
	switch(idEvent)
	{
	case TIMER_BLUETOOTH_A2DP_WARNING:
		SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_DEFAULT);
		KillTimer(g_pBtA2DP->m_hMainWnd,TIMER_BLUETOOTH_A2DP_WARNING);
		break;
	default:
		break;
	}
}
