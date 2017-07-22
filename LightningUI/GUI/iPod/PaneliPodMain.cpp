#include "PaneliPodMain.h"
#include "../Media/MediaCommon.h"
#include "../../resource.h"


//main:
CPaneliPodMain::CPaneliPodMain(void)
{
	m_iElapsedTime=0;
	m_iRemainTime=0;

	m_bPlayingFlag=FALSE;

	m_pBuffAlbumArt = NULL;
	m_dwAlbumArtSize = 0;
	m_bNeedUpdateAlbumArt = FALSE;
	m_bHasAlbumArt = FALSE;

	m_bAutoResumeFlag = TRUE; 	//20160428

	m_pSmartBarMedia = NULL;

	m_iRemainTime=0;
	m_iElapsedTime=0;

	m_dwEnterTick = 0;

	m_bPlayInfoFlyOut = FALSE;
	m_idPlayInfoFlyOutType=0;

	m_idPlayMode = 0;
	m_bConnected = FALSE;
}
CPaneliPodMain::~CPaneliPodMain(void)
{

}
BOOL CPaneliPodMain::IsReady()
{
    BOOL bRet = CPanelGL::IsReady() && m_tagAlbumBk.IsReady() && m_iconAlbum.IsReady();
    return bRet;
}
void CPaneliPodMain::OnGesture(GESTUREINFO *pgi)
{
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
				case ARG_SCROLL_LEFT:
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
					SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PREVIOUS);				
#else
					SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_NEXT);			
#endif
					PlaySoundEffect(0);
					break;
				case ARG_SCROLL_RIGHT:
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
					SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_NEXT);			
#else
					SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PREVIOUS);
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

void CPaneliPodMain::OnGestureMsg(UINT idGesture)
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
void CPaneliPodMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	m_bAutoResumeFlag = (m_bPlayingFlag);	//20160428

}
void CPaneliPodMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(DEBUG_IPOD, (TEXT(" @@@@ CPaneliPodMain::OnEnterPanel iParam:%d\r\n"),iParam));

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

	//Switch audio SRC:
	if(GetAudioSRC() != SRC_AUDIO_ARM)
	    SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);

	//notify src:
	::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_IPOD,0x40);

	//20160428
	if(iParam==-1)
	{
		m_bAutoResumeFlag = TRUE;
	}
	m_dwEnterTick = GetTickCount();

	//update widget:
	if(m_bConnected)
	{
	if(m_pSmartBarMedia)
	{
		m_pSmartBarMedia->SetType(SB_MEDIATYPE_IPOD);
		m_pSmartBarMedia->SetNowPlayingTitle(_T(""));
		m_pSmartBarMedia->SetTrackName(GetResourceString(IDS_DEVICE_IPOD));
	}

	//

	if(m_pSkinManager)
	{
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,LPVOID(MEDIA_TYPE_IPOD));
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,(LPVOID)m_tagTitle.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)m_tagArtist.GetCaption());
	}
	}
}
void CPaneliPodMain::Render()
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

	//
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


	m_iconiPod.SetAlpha(m_fCurAlpha);
	m_iconiPod.Render();

	m_tagNowPlaying.SetAlpha(m_fCurAlpha);
	m_tagNowPlaying.Render();


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

	m_btnPlayModeRepeatAll.SetLuminance(m_fCurLuminance*(m_idPlayMode?1.0f:0.6f));
	m_btnPlayModeRepeatAll.SetAlpha(m_fCurAlpha*(m_idPlayMode?1.0f:0.6f));
	m_btnPlayModeRepeatAll.Render();

	m_btnPlayModeRepeatOne.SetAlpha(m_fCurAlpha);
	m_btnPlayModeRepeatOne.Render();

	m_btnPlayModeShuffle.SetAlpha(m_fCurAlpha);
	m_btnPlayModeShuffle.Render();



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
	if(IsAniReady() && GetTickCount()-m_dwEnterTick>1000)
	{
		if(m_bAutoResumeFlag)
		{
			if(!m_bPlayingFlag && m_bConnected)
			{
				SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PLAY);
			}
			m_bAutoResumeFlag = FALSE;
		}
		if(!m_bConnected)
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
		}
	}

	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}

	EndRender();

}
BOOL CPaneliPodMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

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
	m_tagVinyl.SetPos(-g_iClientWidth/2+268,g_iClientHeight/2-186);
	m_tagVinyl.SetShowScale(173.0f/221.0f);

	m_tagAlbumBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_201));
	m_tagAlbumBk.SetShowScale(184.0f/228.0f);
	m_tagAlbumBk.SetPos(-g_iClientWidth/2+189,g_iClientHeight/2-186);

	m_tagAlbumArt.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_003),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbumArt.SetPos(-g_iClientWidth/2+189-5,g_iClientHeight/2-186);
	m_tagAlbumArt.SetShowScale(182.0f/320.0f);
	m_tagAlbumArt.Show(FALSE);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=-180;
	m_tagAlbumArt.SetIniViewState(&vs);

	//m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	//m_iconLoading.SetShowScale(1.2f);
	//m_iconLoading.SetPos(0,20);

	SIZE siInfo2={g_iScreenWidth/2,40};

	m_iconiPod.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_266));
	//m_iconiPod.SetShowScale(0.4f);
	m_iconiPod.SetPos(65,g_iClientHeight/2-97-16);
	//m_iconiPod.Show(FALSE);

	m_iconTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_206));
	m_iconArtist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_207));
	m_iconAlbum.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_214));
	m_iconTrack.SetPos(65,g_iClientHeight/2-147-16);
	m_iconArtist.SetPos(65,g_iClientHeight/2-197-16);
	m_iconAlbum.SetPos(65,g_iClientHeight/2-247-16);


	COLORGL crb={1.0,1.0,1.0,1.0};
	SIZE siInfo={216,40};
	//m_tagMediaStatus.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	//m_tagMediaStatus.SetSize(&siInfo);
	//m_tagMediaStatus.SetCaption(GetResourceString(IDS_BROWSING_ACTIVE));
	//m_tagMediaStatus.LinkTextGL(pTextGL);
	//m_tagMediaStatus.SetPos(0,g_iClientHeight/2-80);
	//m_tagMediaStatus.SetCaptionHeight(22);
	//m_tagMediaStatus.EnableDrawBack(FALSE);
	//m_tagMediaStatus.SetCaptionPos(BTN_CAPTION_CENTER);
	//m_tagMediaStatus.SetCaptionColor(&crb);

	SIZE siTagTitle = {300,40};
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

	//
	m_btnPlayModeRepeatAll.Initialize(IDC_MEDIA_BTN_REPEATALL,this,
		m_pSkinManager->GetTexture(TEXID_223),NULL,
		m_pSkinManager->GetTexture(TEXID_222),NULL,
		m_pSkinManager->GetTexture(TEXID_224),NULL,
		NULL,NULL);
	m_btnPlayModeRepeatAll.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPlayModeRepeatAll.SetPos(g_iClientWidth/2-98,-g_iClientHeight/2+48);
	m_btnPlayModeRepeatAll.SetTouchRate(2.0,2.0);

	m_btnPlayModeRepeatOne.Initialize(IDC_MEDIA_BTN_REPEATONE,this,
		m_pSkinManager->GetTexture(TEXID_212),NULL,
		m_pSkinManager->GetTexture(TEXID_211),NULL,
		m_pSkinManager->GetTexture(TEXID_213),NULL,
		NULL,NULL);
	m_btnPlayModeRepeatOne.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPlayModeRepeatOne.SetPos(g_iClientWidth/2-98,-g_iClientHeight/2+48);
	m_btnPlayModeRepeatOne.SetTouchRate(2.0,2.0);
	m_btnPlayModeRepeatOne.Show(FALSE);

	m_btnPlayModeShuffle.Initialize(IDC_MEDIA_BTN_SHUFFLE,this,
		m_pSkinManager->GetTexture(TEXID_220),NULL,
		m_pSkinManager->GetTexture(TEXID_219),NULL,
		m_pSkinManager->GetTexture(TEXID_221),NULL,
		NULL,NULL);
	m_btnPlayModeShuffle.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPlayModeShuffle.SetPos(g_iClientWidth/2-98,-g_iClientHeight/2+48);
	m_btnPlayModeShuffle.SetTouchRate(2.0,2.0);
	m_btnPlayModeShuffle.Show(FALSE);

	//home
	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_205),NULL,
		m_pSkinManager->GetTexture(TEXID_204),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+65,-g_iClientHeight/2+48);
	m_btnHome.SetTouchRate(2.0,2.0);

	//play control
	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,
		m_pSkinManager->GetTexture(TEXID_209),NULL,
		m_pSkinManager->GetTexture(TEXID_208),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPlay.SetPos(0,-g_iClientHeight/2+48);
	m_btnPlay.SetTouchRate(2.0,2.0);

	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,
		m_pSkinManager->GetTexture(TEXID_228),NULL,
		m_pSkinManager->GetTexture(TEXID_227),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPause.SetPos(0,-g_iClientHeight/2+48);
	m_btnPause.Show(FALSE);
	m_btnPause.SetTouchRate(2.0,2.0);

	m_btnPrev.Initialize(IDC_MEDIA_BTN_PREV,this,
		m_pSkinManager->GetTexture(TEXID_218),NULL,
		m_pSkinManager->GetTexture(TEXID_217),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPrev.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPrev.SetPos(-104,-g_iClientHeight/2+48);
	m_btnPrev.SetTouchRate(2.0,2.0);

	m_btnNext.Initialize(IDC_MEDIA_BTN_NEXT,this,
		m_pSkinManager->GetTexture(TEXID_226),NULL,
		m_pSkinManager->GetTexture(TEXID_225),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnNext.SetPos(104,-g_iClientHeight/2+48);
	m_btnNext.SetTouchRate(2.0,2.0);

	m_btnLibrary.Initialize(IDC_MEDIA_BTN_LIBRARY,this,
		m_pSkinManager->GetTexture(TEXID_216),NULL,
		m_pSkinManager->GetTexture(TEXID_215),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
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
	m_sliProgress.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_sliProgress.SetRange(0,MEDIA_SILDER_LENGTH);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+140);
	m_sliProgress.SetAutoBackFill(TRUE);
#else
// 	if(m_pSkinManager->GetTexture(TEXID_USER1) == NULL)
// 	{
// 		if(!m_pSkinManager->AddTexture(TEXID_USER1,320,320))
// 		{
// 			RETAILMSG(DEBUG_IPOD, (TEXT("CPanelIPod: Album art texture FAILED!\r\n")));
// 		}
// 	}

	/*
	// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_200,_T("\\Windows\\media_bk.jpg"),g_iClientWidth,g_iClientHeight,FALSE,GL_NEAREST))
	if(!m_pSkinManager->AddTextureFromFile(TEXID_200,_T("\\Windows\\media_bk.jpg"),g_iClientWidth,500,FALSE,GL_NEAREST))
	{
		RETAILMSG(DEBUG_IPOD, (TEXT(" @@@@ TEXID_200 already exist\r\n")));
	}
	if(!m_pSkinManager->AddTextureFromFile(TEXID_299,_T("\\Windows\\media_bar.jpg"),g_iClientWidth,100,FALSE,GL_NEAREST))
	{
		RETAILMSG(DEBUG_IPOD, (TEXT(" @@@@ TEXID_299 already exist\r\n")));
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

	m_tagAlbumArt.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_003),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbumArt.SetPos(-245.0f-4.9f,30);
// 	m_tagAlbumArt.SetScale(1.4,0.8);
	m_tagAlbumArt.SetShowScale(0.49f);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=-180;
// 	vs.fRotateY=-60;
// 	vs.fRotateZ=0;
	m_tagAlbumArt.SetIniViewState(&vs);
	//m_tagAlbumArt.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
// 	m_tagAlbumArt.Show(FALSE);


	//test
// 	m_tagAlbumArtRound.Initialize(IDC_MEDIA_TAG_ALBUMART,this,0,m_pSkinManager->GetTexture(TEXID_100),m_pSkinManager->GetTexture(TEXID_506));
// 	m_tagAlbumArtRound.SetPos(/*-250*/0,52);
// 	m_tagAlbumArtRound.SetShowScale(0.6875f);
// 	m_tagAlbumArtRound.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
// // 	m_tagAlbumArtRound.SetNotifyFunc(NULL,NULL);
// 	vs=m_tagAlbumArtRound.GetCurViewState();
// 	vs.fRotateX=-180;
// 	vs.fRotateZ=0;
// 	m_tagAlbumArtRound.SetIniViewState(&vs);

	SIZE siInfo={360,40};
	SIZE siInfo2={g_iScreenWidth/2,40};

	m_iconiPod.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_253));
    m_iconiPod.SetShowScale(0.8f);
	m_iconiPod.SetPos(-60,g_iClientHeight/2-80);

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
	m_iconTrack.SetShowScale(0.8f);
	m_iconArtist.SetShowScale(0.8f);
	m_iconAlbum.SetShowScale(0.8f);
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
	m_tagNowPlaying.SetCaption(_T(""));
	m_tagNowPlaying.LinkTextGL(pTextGL);
	m_tagNowPlaying.SetCaptionHeight(25);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionColor(&crb);
// 	m_tagNowPlaying.SetPos(g_iScreenWidth/4-20,200);
// 	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT);
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
	//m_tagElapsedTime.Enable(FALSE);

	m_tagRemainTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRemainTime.SetSize(&siTime);
	m_tagRemainTime.SetCaption(_T("00:00"));
	m_tagRemainTime.LinkTextGL(pTextGL);
	m_tagRemainTime.SetPos(g_iClientWidth/2-70,-g_iClientHeight/2+120);
	m_tagRemainTime.SetCaptionHeight(22);
	m_tagRemainTime.EnableDrawBack(FALSE);
	m_tagRemainTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagRemainTime.SetCaptionColor(&crb);
	//m_tagRemainTime.Enable(FALSE);



	//
	m_btnPlayModeRepeatAll.Initialize(IDC_MEDIA_BTN_REPEATALL,this,m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_243),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeRepeatAll.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPlayModeRepeatAll.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+40);
	m_btnPlayModeRepeatAll.SetTouchRate(2.0,2.0);
    m_btnPlayModeRepeatAll.SetShowScale(0.8f);
	m_btnPlayModeRepeatAll.EnableClickAni(TRUE);
	m_btnPlayModeRepeatAll.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnPlayModeRepeatOne.Initialize(IDC_MEDIA_BTN_REPEATONE,this,m_pSkinManager->GetTexture(TEXID_244),NULL,
		m_pSkinManager->GetTexture(TEXID_245),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayModeRepeatOne.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
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
	m_btnPlayModeShuffle.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
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
	m_btnHome.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
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
	m_btnPrev.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPrev.SetPos(-(g_iClientWidth/2-80)/2,-g_iClientHeight/2+40);
	m_btnPrev.SetTouchRate(2.0,2.0);
    m_btnPrev.SetShowScale(0.8f);
	m_btnPrev.EnableClickAni(TRUE);
	m_btnPrev.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnNext.Initialize(IDC_MEDIA_BTN_NEXT,this,m_pSkinManager->GetTexture(TEXID_240),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnNext.SetPos((g_iClientWidth/2-80)/2,-g_iClientHeight/2+40);
	m_btnNext.SetTouchRate(2.0,2.0);
    m_btnNext.SetShowScale(0.8f);
	m_btnNext.EnableClickAni(TRUE);
	m_btnNext.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnLibrary.Initialize(IDC_MEDIA_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_231),NULL,
		m_pSkinManager->GetTexture(TEXID_232),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnLibrary.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-50);
	m_btnLibrary.SetTouchRate(2.0,2.0);
    m_btnLibrary.SetShowScale(0.8f);
	m_btnLibrary.EnableClickAni(TRUE);
	m_btnLibrary.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_233),NULL,
		m_pSkinManager->GetTexture(TEXID_234),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_btnPlay.SetPos(0,-g_iClientHeight/2+40);
	m_btnPlay.SetTouchRate(2.0,2.0);
    m_btnPlay.SetShowScale(0.8f);
	m_btnPlay.EnableClickAni(TRUE);
	m_btnPlay.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,m_pSkinManager->GetTexture(TEXID_238),NULL,
		m_pSkinManager->GetTexture(TEXID_239),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
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
// 	m_sliProgress.SetNotifyFunc(&CPaneliPodMain::OnNotify,this);
	m_sliProgress.SetRange(0,512);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+120);
	m_sliProgress.SetAutoBackFill(TRUE);
	//m_sliProgress.Enable(FALSE);
#endif

	//
	return TRUE;
}
void CPaneliPodMain::SetRemainTime(int iSet)
{
	if(iSet<0)
		iSet=0;

	m_iRemainTime = iSet;

	wchar_t str[16];
	int h,m,s;
	h=m_iRemainTime/3600;
	m=m_iRemainTime/60;//-h*60;
	s=m_iRemainTime-m*60;//-h*3600;
	swprintf_s(str,16,_T("%02d:%02d"),m,s);

	m_tagRemainTime.SetCaption(str);
}
void CPaneliPodMain::SetTitle(const wchar_t *pTitle)
{
	if(pTitle && wcslen(pTitle) != 0)
		m_tagTitle.SetCaption(pTitle,TRUE);
	else
		m_tagTitle.SetCaption(GetResourceString(IDS_UNKNOWN),TRUE);

	if(m_pSmartBarMedia)
	{
		if(m_pSmartBarMedia->GetType() == SB_MEDIATYPE_NULL || m_pSmartBarMedia->GetType() == SB_MEDIATYPE_IPOD)
		{
			m_pSmartBarMedia->SetTrackName(pTitle);
		}
	}
}
void CPaneliPodMain::SetArtist(const wchar_t *pArtist)
{
	if(pArtist && wcslen(pArtist) != 0)
		m_tagArtist.SetCaption(pArtist,TRUE);
	else
		m_tagArtist.SetCaption(GetResourceString(IDS_UNKNOWN),TRUE);
}
void CPaneliPodMain::SetAlbum(const wchar_t *pAlbum)
{
	if(pAlbum && wcslen(pAlbum) != 0)
		m_tagAlbum.SetCaption(pAlbum,TRUE);
	else
		m_tagAlbum.SetCaption(GetResourceString(IDS_UNKNOWN),TRUE);
}
void CPaneliPodMain::SetNowPlayingTitle(const wchar_t *pNowPlayingTitle)
{
	m_tagNowPlaying.SetCaption(pNowPlayingTitle,TRUE);

	if(m_pSmartBarMedia)
	{
		if(m_pSmartBarMedia->GetType() == SB_MEDIATYPE_NULL || m_pSmartBarMedia->GetType() == SB_MEDIATYPE_IPOD)
		{
			m_pSmartBarMedia->SetNowPlayingTitle(pNowPlayingTitle);
		}
	}
}

void CPaneliPodMain::SetElapsedTime(int iSet)
{
	if(iSet<0)
		iSet=0;

	m_iElapsedTime = iSet;

	wchar_t str[16];
	int h,m,s;
	h=m_iElapsedTime/3600;
	m=m_iElapsedTime/60;//-h*60;
	s=m_iElapsedTime-m*60;//-h*3600;
	swprintf_s(str,16,_T("%02d:%02d"),m,s);

	m_tagElapsedTime.SetCaption(str);
}

void CALLBACK CPaneliPodMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPaneliPodMain *panel=(CPaneliPodMain*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPaneliPodMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
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
			SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_LIBRARY);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_REPEATALL:
		{
			if(m_idPlayMode)
				SendiPodCommand(IPOD_CMD_SET_PLAYMODE,IPOD_PLAYMODE_REPEAT_ONE);
			else
				SendiPodCommand(IPOD_CMD_SET_PLAYMODE,IPOD_PLAYMODE_REPEAT_ALL);

			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_REPEATONE:
		{
			SendiPodCommand(IPOD_CMD_SET_PLAYMODE,IPOD_PLAYMODE_SHUFFLE);

			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_SHUFFLE:
		{
			SendiPodCommand(IPOD_CMD_SET_PLAYMODE,IPOD_PLAYMODE_OFF);

			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PLAY:
		{
			SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PLAY);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PAUSE:
		{
			SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PAUSE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_NEXT:
		{
			SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_NEXT);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_PREV:
		{
			SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PREVIOUS);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_SLI_PROGRESS:
		{

		}
		break;
	}
}
void CPaneliPodMain::PlayInfoFlyOut(UINT idType)
{
	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN))
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
			m_tagVinyl.SetPos(pos,g_iClientHeight/2-186,0,TRUE);
			m_tagAlbumBk.SetPos(pos,g_iClientHeight/2-186,0,TRUE);
			m_tagAlbumArt.SetPos(pos,g_iClientHeight/2-186,0,TRUE);

			m_iconiPod.SetPos(pos,g_iClientHeight/2-97-16,0,TRUE);
			m_iconTrack.SetPos(pos,g_iClientHeight/2-147-16,0,TRUE);
			m_iconArtist.SetPos(pos,g_iClientHeight/2-197-16,0,TRUE);
			m_iconAlbum.SetPos(pos,g_iClientHeight/2-247-16,0,TRUE);

			m_tagNowPlaying.SetPos(pos,g_iClientHeight/2-97-16,0,TRUE);
			m_tagTitle.SetPos(pos,g_iClientHeight/2-147-16,0,TRUE);
			m_tagArtist.SetPos(pos,g_iClientHeight/2-197-16,0,TRUE);
			m_tagAlbum.SetPos(pos,g_iClientHeight/2-247-16,0,TRUE);
#else
			m_tagVinyl.SetPos(pos,30,0,TRUE);
            m_tagVinyl.SetShowScale(0.7f);
			m_tagAlbumBk.SetPos(pos,30,0,TRUE);
            m_tagAlbumBk.SetShowScale(0.7f);
			m_tagAlbumArt.SetPos(pos,30,0,TRUE);
            m_tagAlbumArt.SetShowScale(0.49f);

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
void CPaneliPodMain::PlayInfoFlyIn()
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
		m_tagVinyl.SetPos(pos,g_iClientHeight/2-186);
		m_tagAlbumBk.SetPos(pos,g_iClientHeight/2-186);
		m_tagAlbumArt.SetPos(pos,g_iClientHeight/2-186);

		m_iconiPod.SetPos(pos,g_iClientHeight/2-97-16);
		m_iconTrack.SetPos(pos,g_iClientHeight/2-147-16);
		m_iconArtist.SetPos(pos,g_iClientHeight/2-197-16);
		m_iconAlbum.SetPos(pos,g_iClientHeight/2-247-16);

		m_tagNowPlaying.SetPos(pos,g_iClientHeight/2-147-16);
		m_tagTitle.SetPos(pos,g_iClientHeight/2-147-16);
		m_tagArtist.SetPos(pos,g_iClientHeight/2-197-16);
		m_tagAlbum.SetPos(pos,g_iClientHeight/2-247-16);

		//reset
		m_tagVinyl.SetPos(-g_iClientWidth/2+268,g_iClientHeight/2-186,0,TRUE);
		m_tagAlbumBk.SetPos(-g_iClientWidth/2+189,g_iClientHeight/2-186,0,TRUE);
		m_tagAlbumArt.SetPos(-g_iClientWidth/2+189-5,g_iClientHeight/2-186,0,TRUE);

		m_iconiPod.SetPos(65,g_iClientHeight/2-97-16,0,TRUE);
		m_iconTrack.SetPos(65,g_iClientHeight/2-147-16,0,TRUE);
		m_iconArtist.SetPos(65,g_iClientHeight/2-197-16,0,TRUE);
		m_iconAlbum.SetPos(65,g_iClientHeight/2-247-16,0,TRUE);

		m_tagNowPlaying.SetPos(240,g_iClientHeight/2-97-16,0,TRUE);
		m_tagTitle.SetPos(240,g_iClientHeight/2-147-16,0,TRUE);
		m_tagArtist.SetPos(240,g_iClientHeight/2-197-16,0,TRUE);
		m_tagAlbum.SetPos(240,g_iClientHeight/2-247-16,0,TRUE);
#else
		m_tagVinyl.SetPos(pos,30);
        m_tagVinyl.SetShowScale(0.7f);
		m_tagAlbumBk.SetPos(pos,30);
        m_tagAlbumBk.SetShowScale(0.7f);
		m_tagAlbumArt.SetPos(pos,30);
        m_tagAlbumArt.SetShowScale(0.49f);

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
void CPaneliPodMain::UpdateWidget()
{
	//update widget:
	UINT media_type=MEDIA_TYPE_INVALID;
	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);
	if(media_type == MEDIA_TYPE_IPOD || media_type == MEDIA_TYPE_INVALID)
	{
		if(media_type == MEDIA_TYPE_INVALID)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,(LPVOID)MEDIA_TYPE_IPOD);
		}
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,(LPVOID)m_tagTitle.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)m_tagArtist.GetCaption());

		if(m_pSmartBarMedia)
		{
			if(m_pSmartBarMedia->GetType() == SB_MEDIATYPE_NULL || m_pSmartBarMedia->GetType() == SB_MEDIATYPE_IPOD)
			{
				m_pSmartBarMedia->SetTrackName(m_tagTitle.GetCaption());
			}
		}
	}
}
void CPaneliPodMain::SetPlayMode(UINT idPlayMode)
{
	m_idPlayMode = idPlayMode;
	m_btnPlayModeRepeatAll.Show(idPlayMode == IPOD_PLAYMODE_OFF || idPlayMode == IPOD_PLAYMODE_REPEAT_ALL); //off or all
	m_btnPlayModeRepeatOne.Show(idPlayMode == IPOD_PLAYMODE_REPEAT_ONE);//one
	m_btnPlayModeShuffle.Show(idPlayMode == IPOD_PLAYMODE_SHUFFLE);//shuffle
}
void CPaneliPodMain::SetPlayState(BOOL bPlaying)
{
	m_btnPlay.Show(!bPlaying);
	m_btnPause.Show(bPlaying);

	m_bPlayingFlag=(bPlaying);

	UINT media_type=MEDIA_TYPE_INVALID;

	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);

	if(media_type == MEDIA_TYPE_IPOD || media_type == MEDIA_TYPE_INVALID)
	{
		//update widget:
		BOOL bShow=(!m_bPlayingFlag);
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PLAY,UIOPRT_SHOWHIDE,&bShow);
		bShow=!bShow;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PAUSE,UIOPRT_SHOWHIDE,&bShow);
	}

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_IPOD)
	{
		if(m_pSmartBarMedia)
		{
			m_pSmartBarMedia->SetPlayingFlag(m_bPlayingFlag);
		}
	}
}
