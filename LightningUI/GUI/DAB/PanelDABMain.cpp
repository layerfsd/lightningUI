#include "PanelDABMain.h"
#include "DABCommon.h"
#include "../../resource.h"

#if CVTE_EN_DAB

extern const wchar_t* g_strInstallPath;
CPanelDABMain * g_pCPanelDABMain = NULL;

COLORGL g_colorBlack={0.0,0.0,0.0,1.0};
COLORGL g_colorGray={0.8,0.8,0.8,1.0};
COLORGL g_colorWhite={1.0,1.0,1.0,1.0};

int g_nStatusbarX = -20;
int g_nStatusbarY = -90;


#define MAX_FREQUENCY_NUM  61
#define DISP_CHAR_MAX   16
#define DISP_CHAR_MAX1  4

static const wchar_t g_strDABPresetFreq[6][16]=
{
	_T("174.928MHz"),
	_T("174.928MHz"),
	_T("239.200MHz"),
	_T("174.928MHz"),
	_T("174.928MHz"),
	_T("1490.624MHz"),
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
CPanelDABMain::CPanelDABMain(void)
//:CMiniThread( 0, TRUE )
{

	m_dwLastSearchTick = 0;

	m_dwDABInfoUpdateTick = 0;

	m_bDABListInfoSpread = FALSE;

	m_bScanning = FALSE;
	m_bDabServiceReady = FALSE;
}
CPanelDABMain::~CPanelDABMain(void)
{

}

void CPanelDABMain::OnGesture(GESTUREINFO *pgi)
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
					//SendRadioCommand(RADIO_CMD_PLAY_NEXT,0);
					PlaySoundEffect(0);
					break;
				case ARG_SCROLL_RIGHT:
					//SendRadioCommand(RADIO_CMD_PLAY_PREVIOUS,0);
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

void CPanelDABMain::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		//SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		//SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_LIBRARY,PAGE_SWITCH_ANI_LEFT);
		break;
	}
}
BOOL CPanelDABMain::IsReady()
{
	return CPanelGL::IsReady();//&& (!m_bScaleMoving) && (!m_bScanning);

}
void CPanelDABMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	//
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	m_dwEnterTick = GetTickCount();
	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelDABMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
	m_dwEnterTick = GetTickCount();

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

	//Switch audio SRC:
	if(GetAudioSRC() != SRC_AUDIO_DAB)
	{
	    SwitchAudioSRC(SRC_AUDIO_DAB,TRUE);
        RETAILMSG(DEBUG_DAB, (TEXT("MSG:[CPanelDABMain::OnEnterPanel]: Switch to DAB audio SRC \r\n")));
	}

	//update widget:
	if(m_pSkinManager)
	{
		UINT idWidgetType=0;
#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDM_RADIO_WIDGET_TYPE,UIOPRT_GETVALUE,(LPVOID)(&idWidgetType));
#else
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDM_RADIO_WIDGET_TYPE,UIOPRT_GETVALUE,(LPVOID)(&idWidgetType));
#endif


		RETAILMSG(1, (TEXT("MSG:[CPanelDABMain::OnEnterPanel]: idWidgetType=%d \r\n"),idWidgetType));

		if(idWidgetType != UI_CLASS_DAB)
		{
#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDM_RADIO_WIDGET_TYPE,UIOPRT_SETVALUE,(LPVOID)UI_CLASS_DAB);
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagService.GetCaption());
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,NULL);
#else
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDM_RADIO_WIDGET_TYPE,UIOPRT_SETVALUE,(LPVOID)UI_CLASS_DAB);
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagService.GetCaption());
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,NULL);
#endif

		}
#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
		if ( wcslen(m_tagService.GetCaption())> 15)
		{
			m_tagStrService = (wchar_t *)m_tagService.GetCaption();
			SetTimer(g_pCPanelDABMain->m_hMainWnd,TIMER_DAB_ELLIPSIS,1000,OnTimerProc);
		}
#endif

	}
}
void CPanelDABMain::Render()
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_229):NULL;
#else
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
#endif
	
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

	GLfloat fbackLumin=m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance;

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


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBackEq.SetAlpha(m_fCurAlpha);
	m_tagBackEq.Render();

	m_tagBackCircle.SetAlpha(m_fCurAlpha);
	m_tagBackCircle.Render();

	m_tagSearching.SetAlpha(m_fCurAlpha);
	m_tagSearching.Render();

	m_btnBandFM.SetAlpha(m_fCurAlpha);
	m_btnBandFM.Render();

	m_btnBandAM.SetAlpha(m_fCurAlpha);
	m_btnBandAM.Render();

	m_btnDAB.SetAlpha(m_fCurAlpha);
	m_btnDAB.Render();

	m_tagNowPlaying.SetAlpha(m_fCurAlpha);
	m_tagNowPlaying.Render();

	m_btnSeekBackward.SetAlpha(m_fCurAlpha);
	m_btnSeekBackward.Render();

	m_btnSeekForward.SetAlpha(m_fCurAlpha);
	m_btnSeekForward.Render();

	m_tagStationName.SetAlpha(m_fCurAlpha);
	m_tagStationName.Render();

	DrawRect(pTexBar,0,(-g_iClientHeight/2+48),800,80,2,&cr,TRUE);

#else
	DrawRect(pTexBar,0,(-g_iClientHeight/2+40),g_iClientWidth-4,80,2,&cr,TRUE);

	if(m_bScanning)
	{
		VIEW_STATE vs=m_iconLoading.GetCurViewState();
		vs.fRotateZ-=5;
		if(vs.fRotateZ<-360)
			vs.fRotateZ=0;
		m_iconLoading.SetCurViewState(&vs);
		m_iconLoading.SetAlpha(m_fCurAlpha);
		m_iconLoading.Render();

		m_tagFreqNum.SetAlpha(m_fCurAlpha);
		m_tagFreqNum.Render();

		m_tagSearch.SetAlpha(m_fCurAlpha);
		m_tagSearch.Render();

		SetFrameTime(30);
	}
	else
	{
		SetFrameTime(500);

		m_tagNowPlaying.SetAlpha(m_fCurAlpha);
		m_tagNowPlaying.Render();

		m_tagEnsemble.SetAlpha(m_fCurAlpha);
		m_tagEnsemble.Render();

		m_tagService.SetAlpha(m_fCurAlpha);
		m_tagService.Render();

		m_tagDLS.SetAlpha(m_fCurAlpha);
		m_tagDLS.Render();

		for (int i=0;i<6;i++)
		{
			m_btnPreset[i].SetAlpha(m_fCurAlpha*0.7f);
			m_btnPreset[i].Render();
		}
	}

	m_btnBandChange.SetAlpha(m_fCurAlpha);
	m_btnBandChange.Render();

	m_btnFmAm.SetAlpha(m_fCurAlpha);
	m_btnFmAm.Render();

	m_btnTuneForward.SetAlpha(m_fCurAlpha);
	m_btnTuneForward.Render();

	m_btnTuneBackward.SetAlpha(m_fCurAlpha);
	m_btnTuneBackward.Render();

#endif


	m_btnSearch.SetAlpha(m_fCurAlpha);
	m_btnSearch.Render();

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	m_btnLibrary.SetAlpha(m_fCurAlpha);
	m_btnLibrary.Render();

	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}


	EndRender();
}
BOOL CPanelDABMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
	PRINTF_DEBUG_FUNC();

	g_pCPanelDABMain = this;
	ShowStatusBar(TRUE);

	SIZE si;
	VIEW_STATE vs;
	int cx=LIST_BOX_ITEM_WIDTH*1.5;
	int cy=LIST_BOX_TAG_HEIGHT;

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

	SIZE siInfo={360,40};
	SIZE sibtn={200,80};
	SIZE siTitle={200,32};
	SIZE siTitle2={100,32};
	SIZE siInfo2={760,80};

	int caption_max_h = 38;
    int caption1_h = 28;
    int caption2_h = 23;
    int caption3_h = 18;
    int caption4_h = 16;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.8,0.8,0.8,1.0};
	COLORGL cr_blue={4.0/256.0,190.0/256.0,254.0/256.0,1.0};

	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	m_tagBackCircle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_1303));
	m_tagBackCircle.SetShowScale(1.0f);

	m_iconLoading.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713),NULL);
	m_iconLoading.SetShowScale(2.15f * 0.88f);
	m_iconLoading.SetPos(0,40);

	SIZE szPlaying = {55,42};
	m_tagNowPlaying.Initialize(IDC_MEDIA_TAG_NOWPLAYINGTITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.SetSize(&szPlaying);
	m_tagNowPlaying.SetCaption(_T("0/0"));
	m_tagNowPlaying.LinkTextGL(pTextGL);
	m_tagNowPlaying.SetPos(0,0);
	m_tagNowPlaying.SetCaptionHeight(32);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagNowPlaying.SetCaptionColor(&cr_w);

	//m_tagSearching.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713));
	//m_tagSearching.SetPos(0,0);
	//m_tagSearching.SetShowScale(2.15f * 0.65f);


	//home button
	m_btnHome.Initialize(IDC_DAB_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_205),NULL,
		m_pSkinManager->GetTexture(TEXID_204),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+65,-g_iClientHeight/2+43);
	m_btnHome.SetTouchRate(2.0,2.0);

	//library button 
	m_btnLibrary.Initialize(IDC_DAB_BTN_LIBRARY,this,
		m_pSkinManager->GetTexture(TEXID_216),NULL,
		m_pSkinManager->GetTexture(TEXID_215),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnLibrary.SetPos(-g_iClientWidth/2+145, -g_iClientHeight/2+43);
	m_btnLibrary.SetTouchRate(2.0,2.0);

	//Search button
	m_btnSearch.Initialize(IDC_DAB_BTN_PTY_SEARCH,this,
		m_pSkinManager->GetTexture(TEXID_272),NULL,
		m_pSkinManager->GetTexture(TEXID_273),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnSearch.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+43);
	m_btnSearch.SetTouchRate(2.0,2.0);
	m_btnSearch.SetShowScale(0.8f);

	m_btnBandFM.Initialize(IDC_DAB_BTN_BAND_FM,this,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBandFM.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnBandFM.SetPos(-g_iClientWidth/2+308,g_iClientHeight/2-60);
	m_btnBandFM.SetTouchRate(1.5f,1.5f);
	m_btnBandFM.EnableClickAni(TRUE);
	//m_btnBandFM.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	m_btnBandFM.SetCaption(_T("FM"));
	m_btnBandFM.LinkTextGL(pTextGL);
	m_btnBandFM.SetCaptionHeight(24);
	m_btnBandFM.EnableDrawBack(FALSE);
	m_btnBandFM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnBandFM.SetCaptionColor(&cr_gray, &cr_blue);

	m_btnBandAM.Initialize(IDC_DAB_BTN_BAND_AM,this,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBandAM.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnBandAM.SetPos(-g_iClientWidth/2+375,g_iClientHeight/2-60);
	m_btnBandAM.SetTouchRate(1.5f,1.5f);
	m_btnBandAM.EnableClickAni(TRUE);
	m_btnBandAM.SetCaption(_T("AM"));
	m_btnBandAM.LinkTextGL(pTextGL);
	m_btnBandAM.SetCaptionHeight(24);
	m_btnBandAM.EnableDrawBack(FALSE);
	m_btnBandAM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnBandAM.SetCaptionColor(&cr_gray, &cr_blue);

	m_btnDAB.Initialize(IDC_DAB_BTN_DAB,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDAB.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnDAB.SetPos(-g_iClientWidth/2+442,g_iClientHeight/2-60);
	m_btnDAB.SetTouchRate(1.5f,1.5f);
	m_btnDAB.EnableClickAni(TRUE);
	m_btnDAB.SetCaption(_T("DAB"));
	m_btnDAB.LinkTextGL(pTextGL);
	m_btnDAB.SetCaptionHeight(24);
	m_btnDAB.EnableDrawBack(FALSE);
	m_btnDAB.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDAB.SetCaptionColor(&cr_blue, &cr_blue);

	SIZE szName = {334, 42};
 	m_tagStationName.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
 	m_tagStationName.SetSize(&siInfo2);
 	m_tagStationName.SetCaption(_T("DBA+ ......."));
 	m_tagStationName.LinkTextGL(pTextGL);
 	m_tagStationName.SetPos(0,-g_iClientHeight/2 + 130);
 	m_tagStationName.SetCaptionHeight(28);
 	m_tagStationName.EnableDrawBack(FALSE);
 	m_tagStationName.SetCaptionPos(BTN_CAPTION_CENTER);
 	m_tagStationName.SetCaptionColor(&g_colorWhite);

	m_btnSeekBackward.Initialize(IDC_RADIO_BTN_SEEKBACKWARD,this,m_pSkinManager->GetTexture(TEXID_1304),NULL,
		m_pSkinManager->GetTexture(TEXID_1305),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeekBackward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSeekBackward.SetPos(-g_iClientWidth/2 + 99,0);
	m_btnSeekBackward.SetShowScale(0.9f);
	m_btnSeekBackward.SetTouchRate(1.4,1.4);
	m_btnSeekBackward.EnableClickAni(FALSE);
	m_btnSeekBackward.EnableLongClickNotify(TRUE);

	m_btnSeekForward.Initialize(IDC_RADIO_BTN_SEEKFORWARD,this,m_pSkinManager->GetTexture(TEXID_1306),NULL,
		m_pSkinManager->GetTexture(TEXID_1307),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeekForward.SetNotifyFunc(&CPanelRadioMain::OnNotify,this);
	m_btnSeekForward.SetPos(g_iClientWidth/2 - 99,0);
	m_btnSeekForward.SetShowScale(0.9f);
	m_btnSeekForward.SetTouchRate(1.4,1.4);
	m_btnSeekForward.EnableClickAni(FALSE);
	m_btnSeekForward.EnableLongClickNotify(TRUE);

#else
	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	m_iconLoading.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713),NULL);
	m_iconLoading.SetShowScale(1.8f);
	m_iconLoading.SetPos(0,40);

//		//LBand OnOff
//		m_iconLBAND.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_466));
//		m_iconLBANDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_467));
//
//		m_btnLBand.Initialize(IDC_DAB_BTN_LBAND_ONOFF,this,NULL,NULL,
//			NULL,NULL,NULL,NULL,NULL,NULL);
//		m_btnLBand.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnLBand.SetPos(-60,g_iClientHeight/2-40);
//		m_btnLBand.SetTouchRate(1.2f,1.2f);
//		//m_btnLBand.SetShowScale(0.8f);
//		m_btnLBand.EnableDrawBack(FALSE);
//		m_btnLBand.SetIcon(&m_iconLBAND, &m_iconLBANDFocus);
//		m_btnLBand.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
//
//		//TA OnOff
//		m_iconTA.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_464));
//		m_iconTAFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_465));
//
//		m_btnTA.Initialize(IDC_DAB_BTN_TA_ONOFF,this,NULL,NULL,
//			NULL,NULL,NULL,NULL,NULL,NULL);
//		m_btnTA.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnTA.SetSize(128,64);
//		m_btnTA.SetPos(40,g_iClientHeight/2-40);
//		m_btnTA.SetTouchRate(1.2f,1.2f);
//	    m_btnTA.SetShowScale(0.8f);
//		m_btnTA.EnableClickAni(TRUE);
//		m_btnTA.EnableDrawBack(FALSE);
//		m_btnTA.SetIcon(&m_iconTA, &m_iconTAFocus);
//		m_btnTA.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
//
//
//		//SrvLink
//		m_iconSrvLinkDAB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_470));
//		m_iconSrvLinkRDS.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_471));
//		m_iconSrvLinkALL.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_472));
//		m_iconSrvLinkOff.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_473));
//
//		m_btnSrvLink.Initialize(IDC_DAB_BTN_SRVLINK,this,NULL,NULL,
//			NULL,NULL,NULL,NULL,NULL,NULL);
//		m_btnSrvLink.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnSrvLink.SetPos(140,g_iClientHeight/2-40);
//		m_btnSrvLink.SetTouchRate(1.6f,1.6f);
//		m_btnSrvLink.SetIcon(&m_iconSrvLinkOff);
//		m_btnSrvLink.EnableClickAni(TRUE);
//		m_btnSrvLink.EnableDrawBack(FALSE);
//		m_btnSrvLink.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
//
//		//Setting
//		m_btnSetting.Initialize(IDC_DAB_BTN_SETTING,this,m_pSkinManager->GetTexture(TEXID_623),NULL,
//			m_pSkinManager->GetTexture(TEXID_624),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnSetting.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnSetting.SetPos(g_iClientWidth/2-80,g_iClientHeight/2-40);
//		m_btnSetting.SetTouchRate(1.6f,1.6f);
//	    m_btnSetting.SetShowScale(0.8f);
//		m_btnSetting.EnableClickAni(TRUE);
//		m_btnSetting.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//DAB Album
// 	m_tagAlbumBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_463),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	//m_tagAlbumBk.SetSize(200,200);
// 	m_tagAlbumBk.SetShowScale(0.65f);
// 	m_tagAlbumBk.SetPos(-250,0);

//		m_tagBackCircle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_461));
//	    m_tagBackCircle.SetShowScale(0.8f); //m_tagBackCircle.SetShowScale(1.25);
//		m_tagBackCircle.SetPos(-280,0);

	//Favorite
// 	m_btnFavorite.Initialize(IDC_DAB_BTN_FAVORITE,this,m_pSkinManager->GetTexture(TEXID_435),NULL,
// 		m_pSkinManager->GetTexture(TEXID_436),NULL,
// 		NULL,NULL,
// 		NULL,NULL);
// 	m_btnFavorite.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
// 	m_btnFavorite.SetPos(-280,g_nStatusbarY-10);
//     m_btnFavorite.SetShowScale(0.8f);
// 	m_btnFavorite.SetTouchRate(1.5f,1.5f);
// 	m_btnFavorite.EnableClickAni(TRUE);
// 	m_btnFavorite.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

//		//slider:
//		SLIDERSTRUCTURE sli;
//		memset(&sli,0,sizeof(sli));
//		sli.size.cx=420;
//		sli.size.cy=26;
//		sli.bHasBtns=FALSE;
//		sli.bFilled=FALSE;
//		sli.bFilledBoth=FALSE;
//		sli.idBackTex=TEXID_052;
//		sli.idChannelTex=TEXID_053;
//		sli.idThumbTex=NULL;
//
//		m_sliSearchPercent.Initialize(0,this,&sli,pSkinManager);
//		m_sliSearchPercent.SetRange(0,100);
//		m_sliSearchPercent.SetValue(0);
//	    m_sliSearchPercent.SetPos(0.0f,-g_iClientHeight/2+40);
//		m_sliSearchPercent.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_sliSearchPercent.LinkTextGL(pTextGL);
//		m_sliSearchPercent.SetCaptionColor(&g_colorWhite);
//		m_sliSearchPercent.ShowValue(TRUE);
//		m_sliSearchPercent.EnableMoveNotify(TRUE);
//		m_sliSearchPercent.EnbaleClickOnlyThumb(TRUE);

//		m_tagSrvLink.Initialize(IDC_RADIO_TAG_ST_MONO,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagSrvLink.SetSize(&siTitle);
//		m_tagSrvLink.SetCaption(_T(""));
//		m_tagSrvLink.LinkTextGL(pTextGL);
//	    m_tagSrvLink.SetShowScale(0.8f);
//		m_tagSrvLink.SetPos(-200,90);
//		m_tagSrvLink.SetCaptionHeight(caption1_h);
//		m_tagSrvLink.EnableDrawBack(FALSE);
//		m_tagSrvLink.SetCaptionPos(BTN_CAPTION_CENTER);
//		m_tagSrvLink.SetCaptionColor(&g_colorGray);

//		//Freq
//		m_tagFreq.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagFreq.SetSize(&siTitle);
//		m_tagFreq.SetCaption(GetResourceString(IDS_MHZ));
//		m_tagFreq.LinkTextGL(pTextGL);
//		m_tagFreq.SetPos(370,55);
//		m_tagFreq.SetCaptionHeight(caption1_h);
//		m_tagFreq.EnableDrawBack(FALSE);
//		m_tagFreq.SetCaptionPos(BTN_CAPTION_LEFT);
//		m_tagFreq.SetCaptionColor(&g_colorWhite);
//

		m_tagFreqNum.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagFreqNum.SetSize(&siTitle);
		m_tagFreqNum.SetCaption(_T(""));
		m_tagFreqNum.LinkTextGL(pTextGL);
		m_tagFreqNum.SetPos(0,40);
		m_tagFreqNum.SetCaptionHeight(32);
		m_tagFreqNum.EnableDrawBack(FALSE);
		m_tagFreqNum.SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagFreqNum.SetCaptionColor(&g_colorWhite);


	//Searching
	m_tagSearch.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagSearch.SetSize(&siTitle);
	m_tagSearch.SetCaption(GetResourceString(IDS_SEARCHING));
	m_tagSearch.LinkTextGL(pTextGL);
	m_tagSearch.SetPos(0,-100);
	m_tagSearch.SetCaptionHeight(32);
	m_tagSearch.EnableDrawBack(FALSE);
	m_tagSearch.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagSearch.SetCaptionColor(&g_colorWhite);
	//m_tagSearch.Show(FALSE);

	m_tagNowPlaying.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNowPlaying.SetSize(&siInfo2);
	m_tagNowPlaying.SetCaption(_T(""));
	m_tagNowPlaying.LinkTextGL(pTextGL);
	m_tagNowPlaying.SetPos(0,g_iClientHeight/2-90);
	m_tagNowPlaying.SetCaptionHeight(28);
	m_tagNowPlaying.EnableDrawBack(FALSE);
	m_tagNowPlaying.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNowPlaying.SetCaptionColor(&g_colorGray);

	//Channel Service
	m_tagService.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagService.SetSize(&siInfo2);
	m_tagService.SetCaption(_T("No Signal"));
	m_tagService.LinkTextGL(pTextGL);
	m_tagService.SetPos(0,98);
	m_tagService.SetCaptionHeight(38);
	m_tagService.EnableDrawBack(FALSE);
	m_tagService.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagService.SetCaptionColor(&g_colorWhite);

	//Channel Ensemble
	m_tagEnsemble.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagEnsemble.SetSize(&siInfo2);
	m_tagEnsemble.SetCaption(_T(""));
	m_tagEnsemble.LinkTextGL(pTextGL);
	m_tagEnsemble.SetPos(0,50);
	m_tagEnsemble.SetCaptionHeight(26);
	m_tagEnsemble.EnableDrawBack(FALSE);
	m_tagEnsemble.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagEnsemble.SetCaptionColor(&g_colorGray);

	//Channel Station
// 	m_tagStationName.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagStationName.SetSize(&siInfo2);
// 	m_tagStationName.SetCaption(_T("StationName test test"));
// 	m_tagStationName.LinkTextGL(pTextGL);
// 	m_tagStationName.SetPos(0,96);
// 	m_tagStationName.SetCaptionHeight(32);
// 	m_tagStationName.EnableDrawBack(FALSE);
// 	m_tagStationName.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_tagStationName.SetCaptionColor(&g_colorWhite);

	//Channel DLS
	m_tagDLS.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagDLS.SetSize(500, 32);
	m_tagDLS.SetSize(&siInfo2);
	m_tagDLS.SetCaption(_T(""), TRUE);
	m_tagDLS.LinkTextGL(pTextGL);
	m_tagDLS.SetPos(0,16);
	m_tagDLS.SetCaptionHeight(20);
	m_tagDLS.EnableDrawBack(FALSE);
	m_tagDLS.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagDLS.SetCaptionColor(&g_colorGray);

	/*
	//TA
	m_tagTA.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTA.SetSize(&siInfo2);
	m_tagTA.SetPos(0,100-80);
	m_tagTA.SetCaption(_T("TA test test TA test test TA test testTA test test"));
	m_tagTA.LinkTextGL(pTextGL);
	m_tagTA.SetCaptionHeight(20);
	m_tagTA.EnableDrawBack(FALSE);
	m_tagTA.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTA.SetCaptionColor(&g_colorWhite);
	//m_tagTA.Show(m_bTAOn);
*/

	//Band Mode
// 	m_iconBandChange.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_475));
// 	m_iconBandChange.SetPos(g_nStatusbarX-90,g_nStatusbarY+1);
// 	m_iconBandChange.SetShowScale(0.50f);

	//Band: DAB1
// 	m_tagBandMode.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagBandMode.SetSize(&siTitle2);
// 	m_tagBandMode.SetPos(g_nStatusbarX-20,g_nStatusbarY);
// 	m_tagBandMode.SetCaption(_T("DAB1"));
// 	m_tagBandMode.LinkTextGL(pTextGL);
// 	m_tagBandMode.SetCaptionHeight(caption3_h);
// 	m_tagBandMode.EnableDrawBack(FALSE);
// 	m_tagBandMode.SetCaptionPos(BTN_CAPTION_LEFT);
// 	m_tagBandMode.SetCaptionColor(&g_colorWhite);

	//Search Type Mode
// 	m_tagSearchTypeMode.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagSearchTypeMode.SetSize(180,32);
// 	m_tagSearchTypeMode.SetPos(g_nStatusbarX+80,g_nStatusbarY);
// 	m_tagSearchTypeMode.SetCaption(_T("Current Service"));
// 	m_tagSearchTypeMode.LinkTextGL(pTextGL);
// 	m_tagSearchTypeMode.SetCaptionHeight(caption3_h);
// 	m_tagSearchTypeMode.EnableDrawBack(FALSE);
// 	m_tagSearchTypeMode.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_tagSearchTypeMode.SetCaptionColor(&g_colorWhite);

	/*
	//Signal
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

	m_sliDABSignal.Initialize(1,this,&sli,pSkinManager);
	m_sliDABSignal.ShowThumb(FALSE);
	m_sliDABSignal.SetRange(0,5);
	m_sliDABSignal.SetValue(4);
	m_sliDABSignal.SetPos(g_nStatusbarX+80+125,g_nStatusbarY);

	m_iconDABSignalBad.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_671));
	m_iconDABSignalPoor.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_673));
	m_iconDABSignalGood.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_675));

	m_tagDABSignalQuality.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
    m_tagDABSignalQuality.SetShowScale(0.25f);
	m_tagDABSignalQuality.EnableDrawBack(FALSE);
	m_tagDABSignalQuality.SetIcon(&m_iconDABSignalGood);
	m_tagDABSignalQuality.SetPos(g_nStatusbarX+80+105,g_nStatusbarY+5);
*/

//		//PTY type
//		m_tagPTYMode.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagPTYMode.SetSize(&siTitle2);
//		m_tagPTYMode.SetCaption(_T("PTY Type "));
//		m_tagPTYMode.LinkTextGL(pTextGL);
//		m_tagPTYMode.SetPos(g_nStatusbarX+80+160,g_nStatusbarY);
//		m_tagPTYMode.SetCaptionHeight(caption3_h);
//		m_tagPTYMode.EnableDrawBack(FALSE);
//		m_tagPTYMode.SetCaptionPos(BTN_CAPTION_CENTER);
//		m_tagPTYMode.SetCaptionColor(&g_colorWhite);

	//Home
	m_btnHome.Initialize(IDC_DAB_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+40);
	m_btnHome.SetTouchRate(1.6f,1.6f);
    m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//FM/AM
	m_btnFmAm.Initialize(IDC_DAB_BTN_SWITCH_FMAM,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnFmAm.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnFmAm.SetPos(-g_iClientWidth/2+80+120,g_iClientHeight/2-35);
	m_btnFmAm.SetTouchRate(1.6f,1.6f);
	//m_btnFmAm.SetShowScale(0.8f);
	m_btnFmAm.EnableClickAni(TRUE);
	m_btnFmAm.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),256,128);
	m_btnFmAm.SetCaption(_T("FM/AM"));
	m_btnFmAm.LinkTextGL(pTextGL);
	m_btnFmAm.SetCaptionHeight(28);
	m_btnFmAm.EnableDrawBack(FALSE);
	m_btnFmAm.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnFmAm.SetCaptionColor(&g_colorWhite);

	//Band Change
	m_btnBandChange.Initialize(IDC_DAB_BTN_BAND_CHANGE,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBandChange.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnBandChange.SetPos(-g_iClientWidth/2+80+254,g_iClientHeight/2-35);
	m_btnBandChange.SetTouchRate(1.6f,1.6f);
    //m_btnBandChange.SetShowScale(0.8f);
	m_btnBandChange.EnableClickAni(TRUE);
	m_btnBandChange.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	m_btnBandChange.SetCaption(_T("DAB1"));
	m_btnBandChange.LinkTextGL(pTextGL);
	m_btnBandChange.SetCaptionHeight(28);
	m_btnBandChange.EnableDrawBack(FALSE);
	m_btnBandChange.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnBandChange.SetCaptionColor(&g_colorWhite);

	/*
	//Search Type Change
	m_btnSearchTypeChange.Initialize(IDC_DAB_BTN_SEARCH_TYPE_CHANGE,this,m_pSkinManager->GetTexture(TEXID_477),NULL,
		m_pSkinManager->GetTexture(TEXID_478),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearchTypeChange.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnSearchTypeChange.SetPos(-g_iClientWidth/2+80+212,-g_iClientHeight/2+40);
	m_btnSearchTypeChange.SetTouchRate(1.6f,1.6f);
    m_btnSearchTypeChange.SetShowScale(0.8f);
	m_btnSearchTypeChange.EnableClickAni(TRUE);
	m_btnSearchTypeChange.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
*/

//		//PTY Type Change
//		m_btnPTYTypeChange.Initialize(IDC_DAB_BTN_PTY_TYPE_CHANGE,this,m_pSkinManager->GetTexture(TEXID_407),NULL,
//			m_pSkinManager->GetTexture(TEXID_408),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnPTYTypeChange.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnPTYTypeChange.SetPos(-g_iClientWidth/2+80+318,-g_iClientHeight/2+40);
//		m_btnPTYTypeChange.SetTouchRate(1.6f,1.6f);
//	    m_btnPTYTypeChange.SetShowScale(0.8f);
//		m_btnPTYTypeChange.EnableClickAni(TRUE);
//		m_btnPTYTypeChange.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
//
//		//PTY Search
//		m_btnPTYSearch.Initialize(IDC_DAB_BTN_PTY_SEARCH,this,m_pSkinManager->GetTexture(TEXID_409),NULL,
//			m_pSkinManager->GetTexture(TEXID_410),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnPTYSearch.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnPTYSearch.SetPos(g_iClientWidth/2-80-212,-g_iClientHeight/2+40);
//		m_btnPTYSearch.SetTouchRate(1.6f,1.6f);
//	    m_btnPTYSearch.SetShowScale(0.8f);
//		m_btnPTYSearch.EnableClickAni(TRUE);
//		m_btnPTYSearch.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

/*
	//Scan Play
	m_btnScanPlay.Initialize(IDC_DAB_BTN_SCAN_PLAY,this,m_pSkinManager->GetTexture(TEXID_468),NULL,
		m_pSkinManager->GetTexture(TEXID_469),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnScanPlay.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnScanPlay.SetPos(g_iClientWidth/2-80-106,-g_iClientHeight/2+40);
	m_btnScanPlay.SetTouchRate(1.6f,1.6f);
    m_btnScanPlay.SetShowScale(0.8f);
	m_btnScanPlay.EnableClickAni(TRUE);
	m_btnScanPlay.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
*/

	//Search
	m_btnSearch.Initialize(IDC_DAB_BTN_SEARCH,this,m_pSkinManager->GetTexture(TEXID_412),NULL,
		m_pSkinManager->GetTexture(TEXID_413),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnSearch.SetPos(g_iClientWidth/2-80,-g_iClientHeight/2+40);
	m_btnSearch.SetTouchRate(1.6f,1.6f);
    m_btnSearch.SetShowScale(0.8f);
	m_btnSearch.EnableClickAni(TRUE);
	m_btnSearch.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//Prev
	m_btnTuneBackward.Initialize(IDC_DAB_BTN_TUNE_PREVIOUS,this,m_pSkinManager->GetTexture(TEXID_414),NULL,
		m_pSkinManager->GetTexture(TEXID_415),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTuneBackward.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnTuneBackward.SetPos(-100,-g_iClientHeight/2+40);
	//m_btnTuneBackward.SetPos(170,0);
 	m_btnTuneBackward.SetShowScale(0.55f);
	m_btnTuneBackward.SetTouchRate(1.4,1.4);
	m_btnTuneBackward.EnableClickAni(FALSE);
	m_btnTuneBackward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
	m_btnTuneBackward.EnableLongClickNotify(TRUE);

	//Next
	m_btnTuneForward.Initialize(IDC_DAB_BTN_TUNE_NEXT,this,m_pSkinManager->GetTexture(TEXID_416),NULL,
		m_pSkinManager->GetTexture(TEXID_417),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTuneForward.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnTuneForward.SetPos(100,-g_iClientHeight/2+40);
	//m_btnTuneForward.SetPos(260,0);
 	m_btnTuneForward.SetShowScale(0.55f);
	m_btnTuneForward.SetTouchRate(1.4,1.4);
	m_btnTuneForward.EnableClickAni(FALSE);
	m_btnTuneForward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
	m_btnTuneForward.EnableLongClickNotify(TRUE);

//		//Seek-
//		m_btnSeekBackward.Initialize(IDC_DAB_BTN_SEEK_PREVIOUS,this,m_pSkinManager->GetTexture(TEXID_414),NULL,
//			m_pSkinManager->GetTexture(TEXID_415),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnSeekBackward.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnSeekBackward.SetPos(80,0);
//	 	m_btnSeekBackward.SetShowScale(0.7f);
//		m_btnSeekBackward.SetTouchRate(1.4,1.4);
//		m_btnSeekBackward.EnableClickAni(FALSE);
//		m_btnSeekBackward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
//		m_btnSeekBackward.EnableLongClickNotify(TRUE);
//
//		//Prev
//		m_btnTuneBackward.Initialize(IDC_DAB_BTN_TUNE_PREVIOUS,this,m_pSkinManager->GetTexture(TEXID_418),NULL,
//			m_pSkinManager->GetTexture(TEXID_419),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnTuneBackward.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		//m_btnTuneBackward.SetPos(-g_iClientWidth/2+80+318,-g_iClientHeight/2+40);
//		m_btnTuneBackward.SetPos(170,0);
//	 	m_btnTuneBackward.SetShowScale(0.7f);
//		m_btnTuneBackward.SetTouchRate(1.4,1.4);
//		m_btnTuneBackward.EnableClickAni(FALSE);
//		m_btnTuneBackward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
//
//		//Next
//		m_btnTuneForward.Initialize(IDC_DAB_BTN_TUNE_NEXT,this,m_pSkinManager->GetTexture(TEXID_420),NULL,
//			m_pSkinManager->GetTexture(TEXID_421),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnTuneForward.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		//m_btnTuneForward.SetPos(g_iClientWidth/2-80-212,-g_iClientHeight/2+40);
//		m_btnTuneForward.SetPos(260,0);
//	 	m_btnTuneForward.SetShowScale(0.7f);
//		m_btnTuneForward.SetTouchRate(1.4,1.4);
//		m_btnTuneForward.EnableClickAni(FALSE);
//		m_btnTuneForward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
//
//	//		//Seek+
//		m_btnSeekForward.Initialize(IDC_DAB_BTN_SEEK_NEXT,this,m_pSkinManager->GetTexture(TEXID_416),NULL,
//			m_pSkinManager->GetTexture(TEXID_417),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnSeekForward.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
//		m_btnSeekForward.SetPos(350,0);
//	 	m_btnSeekForward.SetShowScale(0.7f);
//		m_btnSeekForward.SetTouchRate(1.4,1.4);
//		m_btnSeekForward.EnableClickAni(FALSE);
//		m_btnSeekForward.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020),200,200);
//		m_btnSeekForward.EnableLongClickNotify(TRUE);

	//Library
	m_btnLibrary.Initialize(IDC_DAB_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_401),NULL,
		m_pSkinManager->GetTexture(TEXID_402),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibrary.SetNotifyFunc(&CPanelDABMain::OnNotify,this);
	m_btnLibrary.SetPos(-g_iClientWidth/2+80,g_iClientHeight/2-35);
	m_btnLibrary.SetTouchRate(1.6f,1.6f);
    //m_btnLibrary.SetShowScale(0.8f);
	m_btnLibrary.EnableClickAni(TRUE);
	m_btnLibrary.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

//		GLfloat step=112.0f*g_iClientWidth/800.0f;
//

	//
	SIZE siBtn={260,70};
	for (int i=0;i<6;i++)
	{
		m_btnPreset[i].Initialize(IDC_DAB_BTN_PRESET1+i,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
			m_pSkinManager->GetTexture(TEXID_607),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btnPreset[i].SetNotifyFunc(&CPanelDABMain::OnNotify,this);
		m_btnPreset[i].LinkTextGL(pTextGL);
		m_btnPreset[i].SetSize(&siBtn);
// 		m_btnPreset[i].SetShowScale(0.50f);
// 		m_btnPreset[i].SetScale(0.70f,0.50f);
		m_btnPreset[i].SetCaptionHeight(26);
// 		m_btnPreset[i].SetPos(-g_iClientWidth/2+10 + 65 + 130*i, -g_iClientHeight/2 + 140);
		m_btnPreset[i].SetPos(-g_iClientWidth/2+10 + 130 + 260*(i%3), -g_iClientHeight/2 + 194 - (i/3)*70);
// 		m_btnPreset[i].SetCaption(g_strDABPresetFreq[i]);
		m_btnPreset[i].SetCaption(GetResourceString(IDS_NONE));
		m_btnPreset[i].SetUserParam(0);
		m_btnPreset[i].SetCaptionPos(BTN_CAPTION_CENTER);
		//m_btnPreset[i].SetSubCaption (_T(""));
		//m_btnPreset[i].SetSubCaptionPos(BTN_CAPTION_CENTER,0,-1);
		m_btnPreset[i].SetCaptionColor(&g_colorBlack);

		//m_btnPreset[i].EnableClickAni(TRUE);
		m_btnPreset[i].EnableLongClickNotify(TRUE);

	}

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

//		//List Info
//		m_tagDABListInfoBK.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_451),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagDABListInfoBK.SetSize(cx,g_iClientHeight-cy*2);	//((g_iClientWidth-box_size_x-margin_x*2),tag_h*6+6);
//	    m_tagDABListInfoBK.SetPos(-400-cx/2,0);

	UpdateLBandSetting();

	BYTE nVersion;

	//it's better not to call any api here, this is only a ui interface,
	//you will call apis at XXXWrapper panels, and update information
	//to this ui panels
	DABApi_GetSoftWareVersion((byte*)&nVersion);
	RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: nVersion:%d\r\n"), TEXT(__FUNCTION__), nVersion));
	return TRUE;
}



void CPanelDABMain::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

//    RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioMain::OnMCUCmd:        type:%d,ID:%x,commander:%x\r\n"),dwType,dwID,lParam));

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}


	if(dwType == 0)//package
	{

	}
	else if(dwType == 1)//single command
	{
// 		RETAILMSG(DEBUG_RADIO,(_T("+++CPanelDABMain Parse command:%x"),dwID));


	}
}
void CPanelDABMain::NotifyDabServiceReady(BOOL bReady)
{
	m_bDabServiceReady = bReady;

	BOOL bEnable = m_bDabServiceReady && !m_bScanning;

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s] : bDabServiceReady=%d, bScanning=%d\r\n"),TEXT(__FUNCTION__),m_bDabServiceReady,m_bScanning));

	m_btnLibrary.EnableNotify(bEnable);
	m_btnSearch.EnableNotify(bEnable);
	m_btnTuneBackward.EnableNotify(bEnable);
	m_btnTuneForward.EnableNotify(bEnable);
	m_btnBandChange.EnableNotify(bEnable);

	for (int i=0;i<6;i++)
	{
		m_btnPreset[i].EnableNotify(bEnable);
	}
}
void CPanelDABMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(DEBUG_DAB, (TEXT("CPanelDABMain::HandleControlMSG idControl[%d].\r\n"),idControl));
	if(!m_bDabServiceReady && idControl!=IDC_DAB_BTN_HOME && idControl!=IDC_DAB_BTN_SWITCH_FMAM)
	{
		return;
	}
	switch (idControl)
	{
	case IDC_DAB_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_SWITCH_FMAM:
		{
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_BAND_CHANGE:
		{
			SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_CHANGE_BAND);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_SEARCH_TYPE_CHANGE:
		{
			SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_CHANGE_SEARCH_TYPE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_LIBRARY:
		{
			SwitchPage(UI_CLASS_DAB,PAGE_DAB_LIBRARY);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_SEEK_PREVIOUS:
		{
//				DABApi_GetCurrentSearchMode(&m_nSearchTypeMode);
//				DABApi_SeekUpProc(m_nSearchTypeMode);
			SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SEEK_PREVIOUS);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_SEEK_NEXT:
		{
//				DABApi_GetCurrentSearchMode(&m_nSearchTypeMode);
//				DABApi_SeekDownProc(m_nSearchTypeMode);
			SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SEEK_NEXT);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_TUNE_PREVIOUS:
		{
			if(wMsg==NOTIFY_SHORT_CLICK)
				SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_PREVIOUS);
			else if(wMsg==NOTIFY_LONG_CLICK)
				SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SEEK_PREVIOUS);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_TUNE_NEXT:
		{
			if(wMsg==NOTIFY_SHORT_CLICK)
				SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_NEXT);
			else if(wMsg==NOTIFY_LONG_CLICK)
				SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SEEK_NEXT);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_SCAN_PLAY:
		{
			static BOOL bScanPlay = FALSE;
//				DABApi_GetCurrentSearchMode(&m_nSearchTypeMode);
//				DABApi_ScanProc(m_nSearchTypeMode,TRUE);
			if(bScanPlay)
			{
				bScanPlay = FALSE;
				SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SCAN_PLAY_STOP);
			}
			else
			{
				bScanPlay = TRUE;
				SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SCAN_PLAY_START);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_SEARCH:
		{
			//SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SCAN_AUTO_STORE);
			SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SCAN_SERVICE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_LBAND_ONOFF:
		{
			DABApi_SetLBandSetting();
			UpdateLBandSetting();
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_TA_ONOFF:
		{
			DABApi_SetTASetting();
			UpdateTASetting();
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_SRVLINK:
		{
			DABApi_SetServiceLink();
			UpdateSrvLinkMode();
		}
		break;
	case IDC_DAB_BTN_PTY_TYPE_CHANGE:
		{

		}
		break;
	case IDC_DAB_BTN_PTY_SEARCH:
		{

		}
		break;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	case IDC_DAB_BTN_BAND_FM:
		{
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE);
			SendRadioCommand(RADIO_CMD_SELECT_BAND,1);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_BAND_AM:
		{
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE);
			SendRadioCommand(RADIO_CMD_SELECT_BAND,0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_BTN_DAB:
		{
			//SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_NONE);
			PlaySoundEffect(0);
		}
		break;
#else
	case IDC_DAB_BTN_PRESET1:
	case IDC_DAB_BTN_PRESET2:
	case IDC_DAB_BTN_PRESET3:
	case IDC_DAB_BTN_PRESET4:
	case IDC_DAB_BTN_PRESET5:
	case IDC_DAB_BTN_PRESET6:
		{
			int idx=idControl-IDC_DAB_BTN_PRESET1;
			if(wMsg==NOTIFY_SHORT_CLICK)
			{
				if(m_btnPreset[idx].GetUserParam())
				{
					SelectPreset(idx);
				}
				else
				{
					SavePreset(idx);
					m_btnPreset[idx].SetUserParam(1);
				}
			}
			else if(wMsg==NOTIFY_LONG_CLICK)
			{
				if(m_btnPreset[idx].GetUserParam())
				{
					DeletePreset(idx);
					m_btnPreset[idx].SetUserParam(0);
				}
				else
				{
					SavePreset(idx);
					m_btnPreset[idx].SetUserParam(1);
				}
			}
			PlaySoundEffect(0);
		}
		break;
#endif
	}
}
void CPanelDABMain::SelectPreset(int idx)
{
	byte band;
	DABApi_GetCurrentBand(&band);
	RETAILMSG(DEBUG_DAB,(_T("CPanelDABMain SelectPreset: %d, band=%d\r\n"),idx,band));
	DABApi_PresetLoadProc(band,idx);
	//UpdatePresetInfo();
}
void CPanelDABMain::DeletePreset(int idx)
{
	byte band;
	DABApi_GetCurrentBand(&band);
	RETAILMSG(DEBUG_DAB,(_T("CPanelDABMain DeletePreset: %d, band=%d\r\n"),idx,band));
	DABApi_PresetDeleteProc(band,idx);
	UpdatePresetInfo();
}
void CPanelDABMain::SavePreset(int idx)
{
	byte band;
	DABApi_GetCurrentBand(&band);
	RETAILMSG(DEBUG_DAB,(_T("CPanelDABMain SavePreset: %d, band=%d\r\n"),idx,band));
	DABApi_PresetMemeryProc(band,idx);
	UpdatePresetInfo();
}
void CALLBACK CPanelDABMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_DAB,(_T("CPanelDABMain OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelDABMain *panel=(CPanelDABMain*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);

}
/*
DWORD CCarplayAdapter::ThreadRun( )
{

}

void CPanelDABMain::StartStatusUpdate()
{
	ThreadStart();
}

void CPanelDABMain::StopStatusUpdate()
{
	ThreadStop();
}

void CPanelDABMain::StatusUpdate()
{

}
*/
void CPanelDABMain::SetNowPlayingIndex(int idxCur,int total)
{
	//RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: idxCur=%d,total=%d\r\n"), TEXT(__FUNCTION__),idxCur,total));

	if(idxCur >= 0)
	{
		wchar_t temp[16];
		swprintf_s(temp,16,_T("%d/%d"),idxCur+1,total);

		m_tagNowPlaying.SetCaption(temp);
	}
}
void CPanelDABMain::UpdateBandlInfo()
{
	DABApi_GetCurrentBand(&m_nBandMode);

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: %d.\r\n"), TEXT(__FUNCTION__),m_nBandMode));

	switch(m_nBandMode)
	{
	case DEF_DAB1:
		m_btnBandChange.SetCaption(_T("DAB1"));
		break;
	case DEF_DAB2:
		m_btnBandChange.SetCaption(_T("DAB2"));
		break;
	case DEF_DAB3:
		m_btnBandChange.SetCaption(_T("DAB3"));
		break;
	default:
		break;
	}

	UpdatePresetInfo();
}
BYTE CPanelDABMain::ChangeBandMode()
{
	DABApi_BandProc();

	Sleep(5);

	DABApi_GetCurrentBand(&m_nBandMode);

	switch(m_nBandMode)
	{
	case DEF_DAB1:
		m_btnBandChange.SetCaption(_T("DAB1"));
		break;
	case DEF_DAB2:
		m_btnBandChange.SetCaption(_T("DAB2"));
		break;
	case DEF_DAB3:
		m_btnBandChange.SetCaption(_T("DAB3"));
		break;
	default:
		break;
	}

	UpdatePresetInfo();

	return m_nBandMode;
}

BYTE CPanelDABMain::ChangeSearchType()
{
	DABApi_SearchModeProc();

	Sleep(5);

	DABApi_GetCurrentSearchMode(&m_nSearchTypeMode);

	switch(m_nSearchTypeMode)
	{
		case AllService_Search:
			m_tagSearchTypeMode.SetCaption(_T("All Service"));
		break;
		case PresetService_Search://在所有Preset 中操作
			m_tagSearchTypeMode.SetCaption(_T("Preset Service"));
			break;
		case CurrentService_Search: //在当前Service 中操作
			m_tagSearchTypeMode.SetCaption(_T("Current Service"));
			break;
		case Manual_Search:
		default: //Manual_Search, //在Station 中操作
			m_tagSearchTypeMode.SetCaption(_T("Manual Service"));
			break;
	}

	return m_nSearchTypeMode;
}

void CPanelDABMain::UpdateScanStatus(BOOL bScaning)
{
	m_bScanning = bScaning;

	BOOL bEnable = m_bDabServiceReady && !m_bScanning;

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s] : bDabServiceReady=%d, bScanning=%d\r\n"),TEXT(__FUNCTION__),m_bDabServiceReady,m_bScanning));

	m_btnLibrary.EnableNotify(bEnable);
	m_btnSearch.EnableNotify(bEnable);
	m_btnTuneBackward.EnableNotify(bEnable);
	m_btnTuneForward.EnableNotify(bEnable);
	m_btnBandChange.EnableNotify(bEnable);

	for (int i=0;i<6;i++)
	{
		m_btnPreset[i].EnableNotify(bEnable);
	}
}

void CPanelDABMain::UpdateSrvLinkMode()
{
	DABApi_GetSrvLinkSetting(&m_nLinkModel);

	switch(m_nLinkModel)
	{
		case  SrvFollowing_Off:
			m_btnSrvLink.SetIcon(&m_iconSrvLinkOff);
			break;
		case SrvFollowing_DABtoDAB:
			m_btnSrvLink.SetIcon(&m_iconSrvLinkDAB);
			break;
		case SrvFollowing_DABtoRDS:
			m_btnSrvLink.SetIcon(&m_iconSrvLinkRDS);
			break;
		case SrvFollowing_DABtoAll:
			m_btnSrvLink.SetIcon(&m_iconSrvLinkALL);
			break;
	}
}

void CPanelDABMain::UpdateLBandSetting()
{
	BYTE nLBand;
	DABApi_GetLBandSetting(&nLBand);

	if(nLBand)
	{
		m_bLBandOn = TRUE;
		m_btnLBand.SetIcon(&m_iconLBANDFocus,&m_iconLBAND);
		RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: LBandOn On.\r\n"), TEXT(__FUNCTION__)));
	}
	else
	{
		m_bLBandOn = FALSE;
		m_btnLBand.SetIcon(&m_iconLBAND,&m_iconLBANDFocus);
		RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: LBandOn Off.\r\n"), TEXT(__FUNCTION__)));
	}
}

void CPanelDABMain::UpdateTASetting()
{
	BYTE nTA;
	DABApi_GetTASetting(&nTA);

	if(nTA)
	{
		m_bTAOn = TRUE;
		m_btnTA.SetIcon(&m_iconTAFocus,&m_iconTA);
		RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: m_bTAOn On.\r\n"), TEXT(__FUNCTION__)));
	}
	else
	{
		m_bTAOn = FALSE;
		m_btnTA.SetIcon(&m_iconTA,&m_iconTAFocus);
		RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: m_bTAOn Off.\r\n"), TEXT(__FUNCTION__)));
	}
	m_tagTA.Show(m_bTAOn);
}

void CPanelDABMain::UpdatePTYSetting()
{
	//if(m_bPTYOn)
	//{
	//	m_bPTYOn = FALSE;
	//	m_btnPTY.SetIcon(&m_iconPTY,&m_iconPTYFocus);
	//}
	//else
	//{
	//	m_bPTYOn = TRUE;
	//	m_btnPTY.SetIcon(&m_iconPTYFocus,&m_iconPTY);
	//}
}
void CPanelDABMain::UpdatePresetInfo()
{
	int iTotal=0;
	DABApi_GetTotalOfListInfo(ListType_PresetShortService, 0, &iTotal);
	byte band;
	DABApi_GetCurrentBand(&band);

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: band=%d, iTotal=%d\r\n"), TEXT(__FUNCTION__),band,iTotal));

	if (iTotal>0)
	{
		int cnt=min(6,iTotal);
		for (int i=0;i<cnt;i++)
		{
			wchar_t *name=DABApi_GetNameOfListInfo(ListType_PresetShortService,i);
			if(name && wcslen(name))
			{
				m_btnPreset[i].SetUserParam(1);
				m_btnPreset[i].SetCaption(name);
				RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: index=%d, name=%s\r\n"), TEXT(__FUNCTION__),i,name));
			}
			else
			{
				m_btnPreset[i].SetUserParam(0);
				m_btnPreset[i].SetCaption(GetResourceString(IDS_NONE));
			}
		}
	}

}
void CPanelDABMain::UpdateCurChannelInfo()
{
//		TCHAR tSlsPic[MAX_PATH] = {};
//		DABApi_GetSlideShowFileName((byte*)tSlsPic);
//		RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: %S\r\n"), TEXT(__FUNCTION__),tSlsPic));

	BOOL bNoSignal=TRUE;

	m_strPTY = _T("PTY:");
	m_strPTY += DABApi_GetCurrChanInfo( ChnInfo_PTY );
	m_strFreq = DABApi_GetCurrChanInfo( ChnInfo_Freq );
	m_strStation = DABApi_GetCurrChanInfo( ChnInfo_StationName );
	m_strEnsemble = DABApi_GetCurrChanInfo( ChnInfo_EnsembleLabel );
	m_strService = DABApi_GetCurrChanInfo( ChnInfo_ServiceLabel );
	m_strDLS = DABApi_GetCurrChanInfo( ChnInfo_DLS );

	//cut tail space:
	int ispace = 0;
	const wchar_t *cursor=0;
	if(m_strService.Size())
	{
		cursor=m_strService.String() + m_strService.Size()-1;
		while (*cursor == 0x20)
		{
			cursor--;
			ispace++;
		}
		if(ispace)
			m_strService-=ispace;
	}

	bNoSignal = (m_strService.Size()==0);

	if(m_strEnsemble.Size())
	{
		ispace = 0;
		cursor=m_strEnsemble.String() + m_strEnsemble.Size()-1;
		while (*cursor == 0x20)
		{
			cursor--;
			ispace++;
		}
		if(ispace)
			m_strEnsemble-=ispace;
	}

	//for test: use m_strEnsemble to show PTY
	if(m_strPTY.Size())
	{
		ispace = 0;
		cursor=m_strPTY.String() + m_strPTY.Size()-1;
		while (*cursor == 0x20)
		{
			cursor--;
			ispace++;
		}
		if(ispace)
			m_strPTY-=ispace;
	}

	if(m_strDLS.Size())
	{
		ispace = 0;
		cursor=m_strDLS.String() + m_strDLS.Size()-1;
		while (*cursor == 0x20)
		{
			cursor--;
			ispace++;
		}
		if(ispace)
			m_strDLS-=ispace;
	}

	//RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: PTY=%s\r\n"), TEXT(__FUNCTION__),m_strPTY.String()));
	//RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: Freq=%s\r\n"), TEXT(__FUNCTION__),m_strFreq.String()));
	RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: StationName=%s\r\n"), TEXT(__FUNCTION__),m_strStation.String()));
	//RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: EnsembleLabel=%s\r\n"), TEXT(__FUNCTION__),m_strEnsemble.String()));
	//RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: ServiceLabel=%s\r\n"), TEXT(__FUNCTION__),m_strService.String()));
	//RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: DLS=%s\r\n"), TEXT(__FUNCTION__),m_strDLS.String()));


	if(m_strFreq.Size())
	{
		m_tagFreqNum.SetCaption(m_strFreq.String());
	}
	else
	{
		m_tagFreqNum.SetCaption(_T(""));
	}

	if(m_strStation.Size())
	{
		m_tagStationName.SetCaption(m_strStation.String());
	}
	else
	{
		m_tagStationName.SetCaption(_T("No Signal"));
	}

// 	if(m_strEnsemble.Size())
// 	{
// 		m_tagEnsemble.SetCaption(m_strEnsemble.String());
// 	}
// 	else
// 	{
// 		m_tagEnsemble.SetCaption(_T(""));
// 	}
	//for test: use m_strEnsemble to show PTY
	if(m_strPTY.Size() && !bNoSignal)
	{
		m_tagEnsemble.SetCaption(m_strPTY.String());
	}
	else
	{
		m_tagEnsemble.SetCaption(_T(""));
	}

	if(m_strService.Size())
	{
		m_tagService.SetCaption(m_strService.String());
		m_tagNowPlaying.Show(TRUE);
	}
	else
	{
		m_tagService.SetCaption(_T("No Signal"));
		m_tagNowPlaying.Show(FALSE);
	}

	if(m_pSkinManager && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_DAB)
	{
#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagService.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,NULL);
		if ( wcslen(m_tagService.GetCaption())> 15)
		{
			m_tagStrService = (wchar_t *)m_tagService.GetCaption();
			SetTimer(g_pCPanelDABMain->m_hMainWnd,TIMER_DAB_ELLIPSIS,1000,OnTimerProc);
		}
#else
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)m_tagService.GetCaption());
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_RADIO,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,NULL);
#endif

	}

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:m_strDLS.Size(%d)\r\n"),m_strDLS.Size()));
	if(m_strDLS.Size())
	{
		/*
		if(m_strDLS.Size() > 30 && 	m_nDLSLen != m_strDLS.Size())
		{
			//m_tagDLS.SetCaption(m_strDLS.String(),TRUE);
			m_nDLSLen = m_strDLS.Size();
			m_pStrDLS = (wchar_t *)m_strDLS.String();
			m_bDLSEffect = TRUE;
			m_tagDLS.SetCaption(m_strDLS.String(),TRUE);
			m_dwDLSTick = GetTickCount();
			SetTimer(m_hMainWnd,TIMER_DAB_DLS_EFFECT,300,OnTimerProc);
		}
		else
		{
			if(m_bDLSEffect)
			{
				KillTimer(m_hMainWnd, TIMER_DAB_DLS_EFFECT);
			}
			m_tagDLS.SetCaption(m_strDLS.String(),TRUE);

		}
		*/
		m_tagDLS.SetCaption(m_strDLS.String(),TRUE);
	}
	else
	{
		m_tagDLS.SetCaption(_T(""));
	}
	m_dwDABInfoUpdateTick = GetTickCount();
}

void CPanelDABMain::UpdateSignal()
{
	UINT8 nMscErr = DABApi_GetLinkSNRMscErr();
	BYTE nSignal = DABApi_GetLinkSNRValue();

	nSignal = GetNearInt((nSignal*5)/200.0f);
	if(m_nSignalSNR != nSignal)
	{
		m_nSignalSNR = nSignal;
		m_sliDABSignal.SetValue(m_nSignalSNR);
	}

	if(nMscErr <= 15)
	{
		m_tagDABSignalQuality.SetIcon(&m_iconDABSignalGood);

		//
		//enable service link:
		CDABAdapter::GetInstance()->EnableServiceLink(TRUE);
	}
	else if(nMscErr > 15 && nMscErr <= 30)
	{
		m_tagDABSignalQuality.SetIcon(&m_iconDABSignalPoor);
	}
	else
	{
		m_tagDABSignalQuality.SetIcon(&m_iconDABSignalBad);
	}
}

BOOL CPanelDABMain::ScanServicePlay(BOOL bScan)
{
	BYTE nSearchMode;
	if(bScan)
	{
		FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode((byte *)&nSearchMode));
		FUNC_CHECK_BOOL(DABApi_ScanProc(nSearchMode,TRUE));
	}
	else
	{
		FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode((byte *)&nSearchMode));
		FUNC_CHECK_BOOL(DABApi_ScanProc(nSearchMode,FALSE));
	}
	return TRUE;
}

void CPanelDABMain::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	switch(idEvent)
	{
	case TIMER_DAB_DLS_EFFECT:
		{
			static int stopcount = 0;
			if(g_pCPanelDABMain->m_pStrDLS - g_pCPanelDABMain->m_strDLS.String() > (g_pCPanelDABMain->m_nDLSLen - 36))
			{
				if(stopcount++ > 5)
				{
					stopcount = 0;
					g_pCPanelDABMain->m_pStrDLS = (wchar_t *)g_pCPanelDABMain->m_strDLS.String();
					g_pCPanelDABMain->m_dwDLSTick = GetTickCount();
				}
				return;
			}
			if(GetTickCount() - g_pCPanelDABMain->m_dwDLSTick < 1500)
			{
				return;
			}
			g_pCPanelDABMain->m_tagDLS.SetCaption(g_pCPanelDABMain->m_pStrDLS++,TRUE);
		}
		break;
#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
	case TIMER_DAB_ELLIPSIS:
		{
			if (UI_CLASS_DAB != GET_PANEL_CLASS_ID(g_pCPanelDABMain->m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
			{
				KillTimer(g_pCPanelDABMain->m_hMainWnd,TIMER_DAB_ELLIPSIS);
			}
			else
			{
				static int stopcount = 0;				

				if(stopcount++ > 10)
				{
					g_pCPanelDABMain->m_tagStrService = (wchar_t *)g_pCPanelDABMain->m_tagService.GetCaption();
					stopcount = 0;
				}
				else
				{
					g_pCPanelDABMain->m_tagStrService++;
				}				

				g_pCPanelDABMain->m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDC_RADIO_TAG_CURFREQUENCY,UIOPRT_SETSTRING,(LPVOID)g_pCPanelDABMain->m_tagStrService);
				g_pCPanelDABMain->m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_DAB,IDC_RADIO_TAG_FREQ_UNIT,UIOPRT_SETSTRING,NULL);
			}

		}		
		break;
#endif
	default:
		break;
	}
}

#endif
