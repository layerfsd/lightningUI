#include "PanelBluetoothA2dp.h"
#include "../../resource.h"


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

	m_bAutoResumeFlag = FALSE;


	m_pSmartBarMedia = NULL;

	m_bA2dpConnected = FALSE;
	m_bAvrcpConnected = FALSE;

	m_PlaySate = apStopped;
	m_bHavePlayTimeInfo = FALSE;
	m_iRemainTime=0;
	m_iElapsedTime=0;

	m_repeatAttrVal = AVCM_PAS_REPEAT_VAL_OFF;
	m_shuffleAttrVal = AVCM_PAS_SHUFFLE_VAL_OFF;

	m_dwEnterTick = 0;

	m_bPlayInfoFlyOut = FALSE;
	m_idPlayInfoFlyOutType=0;
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
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_NEXT);
					PlaySoundEffect(0);
					break;
				case ARG_SCROLL_RIGHT:
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PREVIOUS);
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

}
void CPanelBluetoothA2dp::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(DEBUG_BT, (TEXT(" @@@@ CPanelBluetoothA2dp::OnEnterPanel iParam:%d\r\n"),iParam));

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
	SwitchAudioSRC(SRC_AUDIO_ARM);

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
	/*
	if(m_pSkinManager)
	{
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,LPVOID(MEDIA_TYPE_A2DP));
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_TITLE,UIOPRT_SETSTRING,(LPVOID)m_tagTitle.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)m_tagArtist.GetCaption());
	}
	*/

	//
// 	if(m_bPlayingFlag && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)!=UI_CLASS_BT)
// 	{
// 		SendBluetoothCommand(BT_CMD_MUTE,0);
// 		SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
// 	}
}
void CPanelBluetoothA2dp::Render()
{
// 	static wchar_t str[16];
// 	static COLORGL cr2={0.9,0.9,0.9,1};

	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_250):NULL;
// 	static CTexture *pTexFrame=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_033):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};


	if(!BeginRender())return;

	COLORGL cr_w={1.0,1.0,1.0,m_fCurAlpha};
	COLORGL crb={0.03125,0.03125,0.03125,m_fCurAlpha};

	SIZE si;

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

// 	m_tagBar.SetAlpha(m_fCurAlpha);
// 	m_tagBar.Render();

	DrawRect(pTexBar,0,(-g_iClientHeight/2+52),g_iClientWidth-4,104-4,2,&cr,TRUE);

	//
	if(m_bPlayInfoFlyOut)
	{
		if(IsReady())
		{
			PlayInfoFlyIn();
		}
	}

	//
	VIEW_STATE vs=m_tagBackEq.GetCurViewState();
	vs.fRotateX=0;
	vs.fTransY=75;
	vs.fTransX=-253;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.5f);
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
	m_btnPlayModeRepeatAll.SetAlpha(m_fCurAlpha);
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
	if(m_bAutoResumeFlag && IsAniReady() && GetTickCount()-m_dwEnterTick>1500)
	{
		if(m_bA2dpConnected)
		{
// 			if(!m_bPlayingFlag)
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
			}
		}
		else
		{
			SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_NONE);
		}

		m_bAutoResumeFlag = FALSE;
	}

	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}

	EndRender();

}
BOOL CPanelBluetoothA2dp::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);
	ShowStatusBar(TRUE);

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

	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_100));
	si=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	m_tagBkOrigin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_101));
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
	m_tagAlbumArt.SetShowScale(0.62f);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=-180;
	m_tagAlbumArt.SetIniViewState(&vs);



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
	m_iconBT.SetPos(/*-60*/0,/*200*/g_iClientHeight/2-80);

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
	m_tagNowPlaying.SetCaptionHeight(32);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionColor(&crb);
// 	m_tagNowPlaying.SetPos(g_iScreenWidth/4-20,200);
// 	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagNowPlaying.SetPos(0,/*200*/g_iClientHeight/2-80);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_CENTER);

	m_tagTitle.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(250,80);
	m_tagTitle.SetCaptionHeight(28);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crb);

	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.SetSize(&siInfo);
	m_tagArtist.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetPos(250,30);
	m_tagArtist.SetCaptionHeight(28);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crb);

	m_tagAlbum.Initialize(IDC_MEDIA_TAG_ALBUM,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlbum.SetSize(&siInfo);
	m_tagAlbum.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
	m_tagAlbum.LinkTextGL(pTextGL);
	m_tagAlbum.SetPos(250,-20);
	m_tagAlbum.SetCaptionHeight(28);
	m_tagAlbum.EnableDrawBack(FALSE);
	m_tagAlbum.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlbum.SetCaptionColor(&crb);

	SIZE siTime={140,40};

	m_tagElapsedTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagElapsedTime.SetSize(&siTime);
	m_tagElapsedTime.SetCaption(_T("00:00"));
	m_tagElapsedTime.LinkTextGL(pTextGL);
	m_tagElapsedTime.SetPos(-g_iClientWidth/2+70,-g_iClientHeight/2+130);
	m_tagElapsedTime.SetCaptionHeight(22);
	m_tagElapsedTime.EnableDrawBack(FALSE);
	m_tagElapsedTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagElapsedTime.SetCaptionColor(&crb);

	m_tagRemainTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRemainTime.SetSize(&siTime);
	m_tagRemainTime.SetCaption(_T("00:00"));
	m_tagRemainTime.LinkTextGL(pTextGL);
	m_tagRemainTime.SetPos(g_iClientWidth/2-70,-g_iClientHeight/2+130);
	m_tagRemainTime.SetCaptionHeight(22);
	m_tagRemainTime.EnableDrawBack(FALSE);
	m_tagRemainTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagRemainTime.SetCaptionColor(&crb);



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

	//home
	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+50);
	m_btnHome.SetTouchRate(2.0,2.0);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//play control
	m_btnPrev.Initialize(IDC_MEDIA_BTN_PREV,this,m_pSkinManager->GetTexture(TEXID_236),NULL,
		m_pSkinManager->GetTexture(TEXID_237),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPrev.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPrev.SetPos(-(g_iClientWidth/2-80)/2,-g_iClientHeight/2+50);
	m_btnPrev.SetTouchRate(2.0,2.0);
	m_btnPrev.EnableClickAni(TRUE);
	m_btnPrev.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	m_btnNext.Initialize(IDC_MEDIA_BTN_NEXT,this,m_pSkinManager->GetTexture(TEXID_240),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnNext.SetPos((g_iClientWidth/2-80)/2,-g_iClientHeight/2+50);
	m_btnNext.SetTouchRate(2.0,2.0);
	m_btnNext.EnableClickAni(TRUE);
	m_btnNext.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnLibrary.Initialize(IDC_MEDIA_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_231),NULL,
		m_pSkinManager->GetTexture(TEXID_232),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnLibrary.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-50);
	m_btnLibrary.SetTouchRate(2.0,2.0);
	m_btnLibrary.EnableClickAni(TRUE);
	m_btnLibrary.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_233),NULL,
		m_pSkinManager->GetTexture(TEXID_234),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPlay.SetPos(0,-g_iClientHeight/2+50);
	m_btnPlay.SetTouchRate(2.0,2.0);
	m_btnPlay.EnableClickAni(TRUE);
	m_btnPlay.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));


	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,m_pSkinManager->GetTexture(TEXID_238),NULL,
		m_pSkinManager->GetTexture(TEXID_239),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_btnPause.SetPos(0,-g_iClientHeight/2+50);
	m_btnPause.Show(FALSE);
	m_btnPause.SetTouchRate(2.0,2.0);
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
// 	m_sliProgress.SetNotifyFunc(&CPanelBluetoothA2dp::OnNotify,this);
	m_sliProgress.SetRange(0,512);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(0.0f,-g_iClientHeight/2+120);
	m_sliProgress.SetAutoBackFill(TRUE);


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
// 		m_btnPause.Enable(TRUE);
// 		m_btnPlay.Enable(TRUE);

		m_tagNowPlaying.SetCaption(GetResourceString(IDS_DISCONNECTED));

		m_tagTitle.SetCaption(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
		m_tagArtist.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
		m_tagAlbum.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));

		m_tagElapsedTime.SetCaption(_T("00:00"));
		m_tagRemainTime.SetCaption(_T("00:00"));

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
		}
	}

}
void CPanelBluetoothA2dp::OnAvrcpPlayStateChange(AvcmPlayStateT state)
{

	if(m_PlaySate!=state)
	{
		m_PlaySate = state;

// 		m_btnPause.Enable(TRUE);
// 		m_btnPlay.Enable(TRUE);

		m_btnPause.Show(state == apPlaying);
		m_btnPlay.Show(state != apPlaying);

		m_bPlayingFlag = (state == apPlaying);

		UINT media_type=MEDIA_TYPE_INVALID;

		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);

		if(media_type == MEDIA_TYPE_A2DP || media_type == MEDIA_TYPE_INVALID)
		{
			//update widget:
			BOOL bShow=(state != apPlaying);
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PLAY,UIOPRT_SHOWHIDE,&bShow);
			bShow=!bShow;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PAUSE,UIOPRT_SHOWHIDE,&bShow);
		}

		//query repeat and shuffle mode
		AvcmPlayAppSettingAttIdT repeatAttIdT = AVCM_PAS_REPEAT_ATT_ID;
		AvcmPlayAppSettingAttValIdT shuffleAttrVal=AVCM_PAS_SHUFFLE_VAL_ALL;
		g_BTModule.pApi->apiAvrcp.GetRepeat(repeatAttIdT);
		g_BTModule.pApi->apiAvrcp.GetShuffle(shuffleAttrVal);

		//
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
		{
			if(m_pSmartBarMedia)
			{
				m_pSmartBarMedia->SetPlayingFlag(state == apPlaying);
			}
		}
	}
}
void CPanelBluetoothA2dp::OnAvrcpRepeatModeChange(AvcmPlayAppSettingAttValIdT repeatMode)
{
	m_repeatAttrVal = repeatMode;

	m_btnPlayModeShuffle.Show(m_shuffleAttrVal==AVCM_PAS_SHUFFLE_VAL_ALL);
	m_btnPlayModeRepeatOne.Show(m_shuffleAttrVal!=AVCM_PAS_SHUFFLE_VAL_ALL && m_repeatAttrVal==AVCM_PAS_REPEAT_VAL_SINGLE);
	m_btnPlayModeRepeatAll.Show(m_shuffleAttrVal!=AVCM_PAS_SHUFFLE_VAL_ALL && m_repeatAttrVal!=AVCM_PAS_REPEAT_VAL_SINGLE);

}
void CPanelBluetoothA2dp::OnAvrcpShuffleModeChange(AvcmPlayAppSettingAttValIdT shuffleMode)
{
	m_shuffleAttrVal = shuffleMode;

	m_btnPlayModeShuffle.Show(m_shuffleAttrVal==AVCM_PAS_SHUFFLE_VAL_ALL);
	m_btnPlayModeRepeatOne.Show(m_shuffleAttrVal!=AVCM_PAS_SHUFFLE_VAL_ALL && m_repeatAttrVal==AVCM_PAS_REPEAT_VAL_SINGLE);
	m_btnPlayModeRepeatAll.Show(m_shuffleAttrVal!=AVCM_PAS_SHUFFLE_VAL_ALL && m_repeatAttrVal!=AVCM_PAS_REPEAT_VAL_SINGLE);

}
void CPanelBluetoothA2dp::OnAvrcpPlayTimeChange()
{
	const DWORD *tempSongPlayTime;
	g_BTModule.pDataPool->GetData(CDataPool::AVRCPSongPlayTime,(const void **)&tempSongPlayTime);
	if(tempSongPlayTime)
	{
		DWORD dwSongLen =*tempSongPlayTime;
		DWORD dwSongPos =*(tempSongPlayTime+1);		//TODO?

		if(dwSongLen>0 && dwSongPos<=dwSongLen)
		{
			m_iElapsedTime = dwSongPos/1000;
			m_iRemainTime = (dwSongLen - dwSongPos)/1000;
			m_sliProgress.SetValue( m_iElapsedTime*512/(m_iElapsedTime+m_iRemainTime));

			//
			wchar_t str[16];
			int h,m,s;

			h=m_iElapsedTime/3600;
			m=m_iElapsedTime/60;//-h*60;
			s=m_iElapsedTime-m*60;//-h*3600;
			swprintf_s(str,16,_T("%02d:%02d"),m,s);

			m_tagElapsedTime.SetCaption(str);

			h=m_iRemainTime/3600;
			m=m_iRemainTime/60;//-h*60;
			s=m_iRemainTime-m*60;//-h*3600;
			swprintf_s(str,16,_T("%02d:%02d"),m,s);

			m_tagRemainTime.SetCaption(str);
		}
	}
}
void CPanelBluetoothA2dp::OnAvrcpMetaDataChange()
{
	AvcmMetaDataInfoT *pinfo =NULL ;

	g_BTModule.pDataPool->GetData(CDataPool::AVRCPMediaInfo,(const void **)&pinfo);

	if(pinfo)
	{
		RETAILMSG(DEBUG_BT,(_T("####metadata mask:%x  \r\n"),pinfo->metaDataBitMask));

		m_bHavePlayTimeInfo = (pinfo->metaDataBitMask & AVCM_ITEM_ATT_MASK_TIME);

		if(!m_bHavePlayTimeInfo)
		{
			m_iElapsedTime = 0;
			m_iRemainTime = 0;
			m_sliProgress.SetValue(0);
		}
		else
		{

		}

		//now playing title:
		if( (pinfo->metaDataBitMask & AVCM_ITEM_ATT_MASK_TRACK_NUMBER) && (pinfo->metaDataBitMask & AVCM_ITEM_ATT_MASK_TOTAL_TRACK_NUMBER))
		{
			WCHAR str[MAX_STRING];
			swprintf_s(str,MAX_STRING,GetResourceString(IDS_NOWPLAYING_PLAYING),pinfo->trackNumber,pinfo->totalTrackNumber);
			m_tagNowPlaying.SetCaption(str);
		}
		else
		{
			m_tagNowPlaying.SetCaption(_T(""));
		}

		//title
		if(pinfo->metaDataBitMask & AVCM_ITEM_ATT_MASK_TITLE)
		{
			RETAILMSG(DEBUG_BT,(_T("####title:%s  \r\n"),pinfo->title));
			m_tagTitle.SetCaption(pinfo->title,TRUE);
		}
		else
		{
			WCHAR name[MAX_STRING];
#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_TRACKS),GetResourceString(IDS_UNKNOWN));
#else
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
				GetResourceString(IDS_LIBRARY_MENU_TRACKS));
#endif
			m_tagTitle.SetCaption(name,TRUE);
		}

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

			if(m_pSmartBarMedia)
			{
				if(m_pSmartBarMedia->GetType() == SB_MEDIATYPE_NULL || m_pSmartBarMedia->GetType() == SB_MEDIATYPE_A2DP)
				{
					m_pSmartBarMedia->SetTrackName(m_tagTitle.GetCaption());
				}
			}
		}

		//artist
		if(pinfo->metaDataBitMask & AVCM_ITEM_ATT_MASK_ARTIST)
		{
			RETAILMSG(DEBUG_BT,(_T("####artist:%s  \r\n"),pinfo->artist));
			m_tagArtist.SetCaption(pinfo->artist,TRUE);
		}
		else
		{
			WCHAR name[MAX_STRING];
#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_ARTISTS),GetResourceString(IDS_UNKNOWN));
#else
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
				GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
#endif
			m_tagArtist.SetCaption(name,TRUE);
		}
		//update widget:
		if(media_type == MEDIA_TYPE_A2DP || media_type == MEDIA_TYPE_INVALID)
		{
			if(media_type == MEDIA_TYPE_INVALID)
			{
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_SETVALUE,(LPVOID)MEDIA_TYPE_A2DP);
			}
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_ARTIST,UIOPRT_SETSTRING,(LPVOID)m_tagArtist.GetCaption());
		}

		//album
		if(pinfo->metaDataBitMask & AVCM_ITEM_ATT_MASK_ALBUM)
		{
			RETAILMSG(DEBUG_BT,(_T("####album:%s  \r\n"),pinfo->album));
			m_tagAlbum.SetCaption(pinfo->album,TRUE);
		}
		else
		{
			WCHAR name[MAX_STRING];
#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_ALBUMS),GetResourceString(IDS_UNKNOWN));
#else
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
				GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
#endif
			m_tagAlbum.SetCaption(name,TRUE);
		}
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
		m_tagNowPlaying.SetCaption(_T(""));
		WCHAR name[MAX_STRING];

		m_iRemainTime=0;
		m_iElapsedTime=0;

#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_TRACKS),GetResourceString(IDS_UNKNOWN);
#else
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
			GetResourceString(IDS_LIBRARY_MENU_TRACKS));
#endif
		m_tagTitle.SetCaption(name);

#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_ARTISTS),GetResourceString(IDS_UNKNOWN));
#else
		swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
			GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
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

			GLfloat pos=(idType==0)?-1000:1000;

			m_tagVinyl.SetPos(pos,30,0,TRUE);
			m_tagAlbumBk.SetPos(pos,30,0,TRUE);
			m_tagAlbumArt.SetPos(pos,30,0,TRUE);

			m_iconTrack.SetPos(pos,80,0,TRUE);
			m_iconArtist.SetPos(pos,30,0,TRUE);
			m_iconAlbum.SetPos(pos,-20,0,TRUE);

			m_tagTitle.SetPos(pos,80,0,TRUE);
			m_tagArtist.SetPos(pos,30,0,TRUE);
			m_tagAlbum.SetPos(pos,-20,0,TRUE);
		}
	}
}
void CPanelBluetoothA2dp::PlayInfoFlyIn()
{
	if(m_bPlayInfoFlyOut)
	{
		m_bPlayInfoFlyOut = FALSE;

		GLfloat pos=(m_idPlayInfoFlyOutType==0)?1000:-1000;

		m_tagVinyl.SetPos(pos,30);
		m_tagAlbumBk.SetPos(pos,30);
		m_tagAlbumArt.SetPos(pos,30);

		m_iconTrack.SetPos(pos,80);
		m_iconArtist.SetPos(pos,30);
		m_iconAlbum.SetPos(pos,-20);

		m_tagTitle.SetPos(pos,80);
		m_tagArtist.SetPos(pos,30);
		m_tagAlbum.SetPos(pos,-20);

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
	}
}
