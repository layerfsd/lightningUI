#include "PanelImageMain.h"
#include "../../resource.h"
#include <new>
#if CVTE_EN_ACCON_WARNING
#include "../Warning/WarningProxy.h"
#endif

CPanelImageMain* g_pCPanelImage = NULL;

CPanelImageMain::CPanelImageMain(void)
{
	m_ptagImageDisplay = NULL;
//		m_ptagImageDisplayPrev = NULL;
//		m_ptagImageDisplayNext = NULL;
	m_fZoomRate = 1.0f;

	m_dwShowControlTick = 0;
	m_bShowControl = TRUE;
	m_bEnableFreeDrag = FALSE;
	m_bImageLoadReady = FALSE;

//    m_bGetFirstDown = FALSE;
	m_bFirstMove = FALSE;
	m_bDoubleClick = FALSE;
	m_bEnableGestureSwitch = TRUE;
	m_bImagePlaying = FALSE;
	m_px = 0;
	m_py = 0;
	m_dwTouchDownTick = 0;
	m_pCS = new CCriticalSection;
}
CPanelImageMain::~CPanelImageMain(void)
{
    if(m_pCS != NULL)
    {
        delete m_pCS;
    }
}

BOOL CPanelImageMain::IsReady()
{
	//BOOL bRet;
	//Lock();
	//bRet =(m_ptagImageDisplay?m_ptagImageDisplay->IsReady():TRUE);
	//Unlock();
//			(m_ptagImageDisplayPrev?m_ptagImageDisplayPrev->IsReady():TRUE) &&
//			(m_ptagImageDisplayNext?m_ptagImageDisplayNext->IsReady():TRUE);
	return (CPanelGL::IsReady() && m_bImageLoadReady);
}

void CPanelImageMain::OnGesture(GESTUREINFO *pgi)
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
		if(pt.y<250)
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
					//RETAILMSG(1, (TEXT("MSG:SCROLL Next\r\n")));
//					if(m_bEnableGestureSwitch)
//					{
//							SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_NEXT);
//						m_bEnableFreeDrag = FALSE;
//						PlaySoundEffect(0);
//					}
					break;
				case ARG_SCROLL_RIGHT:
					//RETAILMSG(1, (TEXT("MSG:SCROLL Prev\r\n")));
//					if(m_bEnableGestureSwitch)
//					{
//						SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_PREVIOUS);
//						m_bEnableFreeDrag = FALSE;
//						PlaySoundEffect(0);
//					}
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

void CPanelImageMain::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_LIBRARY,PAGE_SWITCH_ANI_LEFT);
		break;
	case GEST_DUAL_TOUCH_ZOOM_IN:
		{
			ZoomIn();
		}
		break;
	case GEST_DUAL_TOUCH_ZOOM_OUT:
		{
			ZoomOut();
		}
		break;
	}
}
void CPanelImageMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	RETAILMSG(1, (TEXT("CPanelImageMain::OnEnterPanel\r\n")));
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
	//PointUpdate(IMAGE_ACTION_ALL);
	//SetTimer(m_hMainWnd,TIMER_IMAGE_HD_SHOW,500,OnTimerProc);
#if CVTE_EN_ACCON_WARNING
	static BOOL bShowWarningPage = FALSE;
	if(m_ptagImageDisplay && m_bImageLoadReady && !bShowWarningPage)
	{
		bShowWarningPage = TRUE;
		CWarningProxy::GetInstance()->EnterWarningPage(INFINITE);
	}
#endif
}

void CPanelImageMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);
	KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
	//KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_HD_SHOW);
}
void CPanelImageMain::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);

//	    if(!m_bGetFirstDown)
//	    {
//	        m_bGetFirstDown = TRUE;
//	        m_ptPenDown.x = pt->x;
//	        m_ptPenDown.y = pt->y;
//	    }
//
//		m_bShowControl = TRUE;
//		m_dwShowControlTick = GetTickCount();
//
//		if(m_bEnableFreeDrag)
//		{
//			m_px=m_ptagImageDisplay->GetPosX();
//			m_py=m_ptagImageDisplay->GetPosY();
//		}
}
void CPanelImageMain::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
//	    m_bGetFirstDown = FALSE;
}
void CPanelImageMain::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

//		m_bEnableFreeDrag = ((abs(m_ptPenDown.x-pt->x)>20 || abs(m_ptPenDown.y-pt->y)>20) && m_ptPenDown.y>80 && m_ptPenDown.y<380); //pt->y>80 && pt->y<380 ;
//
//		if(m_ptagImageDisplay && m_bEnableFreeDrag)
//		{
//			m_ptagImageDisplay->SetPos(m_px+pt->x-m_ptLastPress.x,m_py-pt->y+m_ptLastPress.y);
//		}
}

void CPanelImageMain::Render()
{
	if(!BeginRender())
		return;

	//RETAILMSG(1, (TEXT("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));

#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO || CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
	if(m_ptagImageDisplay && m_bImageLoadReady && m_bOnBrake)
	{
		m_ptagImageDisplay->SetAlpha(m_fCurAlpha);
		m_ptagImageDisplay->Render();
	}
	//show alert:
	if(!m_bOnBrake)
	{
		SIZE si;
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

		m_pTextGL->SetString(_T("Can't Watch Pictures While Driving!"));
		m_pTextGL->SetCharScale(0.65f,0.65f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(GLfloat(-si.cx/2),-40);
		//m_pTextGL->SetColor(&cr2);
		m_pTextGL->Render();
	}
#else
	if(m_ptagImageDisplay && m_bImageLoadReady)
	{
		m_ptagImageDisplay->SetAlpha(m_fCurAlpha);
		m_ptagImageDisplay->Render();
	}
#endif
//		else
//		{
//			printf("Render m_bImageLoadReady[%d]\r\n",m_bImageLoadReady);
//			VIEW_STATE vs = m_iconLoading.GetCurViewState();
//			vs.fRotateZ -= 6;
//			if(vs.fRotateZ < -360)
//				vs.fRotateZ = 0;
//			m_iconLoading.SetCurViewState(&vs);
//			m_iconLoading.SetAlpha(m_fCurAlpha);
//			m_iconLoading.Render();
//		}

	if(m_bShowControl)
	{
		if(GetTickCount()-m_dwShowControlTick>5000 && m_bImageLoadReady)
		{
			m_bShowControl = FALSE;
			m_bRotateSliShow = FALSE;
		}
	}

//		m_btnBrowse.Show(m_bShowControl);
//		m_btnBrowse.SetAlpha(m_fCurAlpha*0.6f);
//		m_btnBrowse.Render();
#if (CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS && CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS_BOA)
	m_tagButtonBG.Show(m_bShowControl);
	m_tagButtonBG.SetAlpha(m_fCurAlpha);
	m_tagButtonBG.Render();
#endif
	m_btnLibraryImage.Show(m_bShowControl);
	m_btnLibraryImage.SetAlpha(m_fCurAlpha);
	m_btnLibraryImage.Render();

#if (CUSTOMER_ID == CUSTOMER_SOUNDEXPRESS || CUSTOMER_ID == CUSTOMER_SOUNDEXPRESS_BOA)
    m_btnZoomIn.Show(m_bShowControl);
	m_btnZoomIn.SetAlpha(m_fCurAlpha);
	m_btnZoomIn.Render();

    m_btnZoomOut.Show(m_bShowControl);
	m_btnZoomOut.SetAlpha(m_fCurAlpha);
	m_btnZoomOut.Render();
#else
	m_btnZoom.Show(m_bShowControl);
	m_btnZoom.SetAlpha(m_fCurAlpha);
	m_btnZoom.Render();

	m_btnRotate.Show(m_bShowControl);
	m_btnRotate.SetAlpha(m_fCurAlpha);
	m_btnRotate.Render();

	if(m_bRotateSliShow)
	{
		m_sliRotate.Show(m_bShowControl);
		m_sliRotate.SetAlpha(m_fCurAlpha);
		m_sliRotate.Render();
	}
#endif
	m_btnPrev.Show(m_bShowControl);
	m_btnPrev.SetAlpha(m_fCurAlpha);
	m_btnPrev.Render();

	m_btnNext.Show(m_bShowControl);
	m_btnNext.SetAlpha(m_fCurAlpha);
	m_btnNext.Render();

	m_tagTitle.Show(m_bShowControl);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	if(m_ptagImageDisplay && m_bImageLoadReady)
	{
		m_btnSetWallPaper.Show(m_bShowControl);
		m_btnSetWallPaper.SetAlpha(m_fCurAlpha);
		m_btnSetWallPaper.Render();
	}
#endif

	m_btnPlayPause.Show(m_bShowControl);
	m_btnPlayPause.SetAlpha(m_fCurAlpha);
	m_btnPlayPause.Render();

	EndRender();
}

BOOL CPanelImageMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	g_pCPanelImage = this;

	//We should not show status bar in image panel.
	//ShowStatusBar(TRUE);

	//define ani:
	SetAniMoveIn(ANIM_LINE_LEFT_IN);
	SetAniPrePanelMoveOut(ANIM_LINE_RIGHT_OUT);

	m_iconAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_084));
	m_iconAlert.SetShowScale(1.44f);
	m_iconAlert.SetPos(0,90);


	//
	m_iconLibraryImage.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_401),NULL);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_iconPrev.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_290),NULL);
	m_iconNext.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_291),NULL);
	m_iconZoom.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_292),NULL);
	m_iconRotate.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_293),NULL);
	m_iconWallPaper.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_294),NULL);
	m_iconPlay.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_295),NULL);
	m_iconPause.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_296),NULL);
    m_iconWallPaper.SetShowScale(0.8f);
#else
	m_iconPrev.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_289),NULL);
	m_iconNext.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_290),NULL);
	m_iconZoom.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_291),NULL);
	//m_iconBrowse.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_292),NULL);
	m_iconRotate.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_242),NULL);
//		m_iconLoading.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713),NULL);
//		m_iconLoading.SetShowScale(0.6f);
//		m_iconLoading.SetPos(0,0);

	m_iconWallPaper.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_090),NULL);
	m_iconPlay.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_233),NULL);
	m_iconPause.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_238),NULL);
    m_iconWallPaper.SetShowScale(0.8f);
#endif

//		m_btnBrowse.Initialize(IDC_IAMGE_BTN_BROWSE,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
//			m_pSkinManager->GetTexture(TEXID_522),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnBrowse.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
//		m_btnBrowse.SetPos(0,-g_iClientHeight/2+70);
//		m_btnBrowse.SetIcon(&m_iconBrowse);
//		m_btnBrowse.SetIconScale(0.6f,0.6f);
//		m_btnBrowse.SetTouchRate(1.0,1.0);
//	    m_btnBrowse.SetShowScale(0.7f);
//		m_btnBrowse.EnableClickAni(TRUE);

	m_btnLibraryImage.Initialize(IDC_IMAGE_BTN_LIBRARY,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLibraryImage.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnLibraryImage.SetPos(64,-g_iClientHeight/2+50);
	m_btnLibraryImage.SetIcon(&m_iconLibraryImage);
	//m_btnLibraryImage.SetIconScale(0.8f,0.8f);
	m_btnLibraryImage.SetTouchRate(1.0,1.0);
    m_btnLibraryImage.SetShowScale(0.7f);

	//
	COLORGL cr={1,1,1,1};

#if (CUSTOMER_ID == CUSTOMER_SOUNDEXPRESS || CUSTOMER_ID == CUSTOMER_SOUNDEXPRESS_BOA)
	m_btnZoomIn.Initialize(IDC_IMAGE_BTN_ZOOMIN,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnZoomIn.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnZoomIn.SetPos(-320,-g_iClientHeight/2+50);
	m_btnZoomIn.LinkTextGL(pTextGL);
	m_btnZoomIn.SetCaptionColor(&cr);
	m_btnZoomIn.SetCaption(_T("-"));
	m_btnZoomIn.SetCaptionHeight(50);
	m_btnZoomIn.SetCaptionPos(BTN_CAPTION_CENTER);
//		m_btnZoomIn.EnableClickAni(TRUE);
	m_btnZoomIn.SetShowScale(0.7f);

	m_btnZoomOut.Initialize(IDC_IMAGE_BTN_ZOOMOUT,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnZoomOut.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnZoomOut.SetPos(320,-g_iClientHeight/2+50);
	m_btnZoomOut.LinkTextGL(pTextGL);
	m_btnZoomOut.SetCaptionColor(&cr);
	m_btnZoomOut.SetCaption(_T("+"));
	m_btnZoomOut.SetCaptionHeight(50);
	m_btnZoomOut.SetCaptionPos(BTN_CAPTION_CENTER);
//		m_btnZoomOut.EnableClickAni(TRUE);
	m_btnZoomOut.SetShowScale(0.7f);
#else
	m_btnZoom.Initialize(IDC_IMAGE_BTN_ZOOM,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnZoom.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnZoom.SetPos(-320,-g_iClientHeight/2+50);
	m_btnZoom.SetIcon(&m_iconZoom);
	m_btnZoom.LinkTextGL(pTextGL);
	m_btnZoom.SetCaptionPos(BTN_CAPTION_CENTER);
//		m_btnZoomIn.EnableClickAni(TRUE);
	m_btnZoom.SetShowScale(0.7f);

	//slider
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_THICKNESS;
	sli.size.cy=200;//cy-tag_h;//256;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_662;
	sli.idChannelTex=TEXID_662;
	sli.idThumbTex=TEXID_613;
	sli.dwType=ST_VER_NORMAL;

	m_sliRotate.Initialize(IDC_IMAGE_SLI_ZOOM,this,&sli,pSkinManager);
	m_sliRotate.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_sliRotate.SetRange(0,20);
	m_sliRotate.SetValue(0);
	m_sliRotate.SetPos(-320,-g_iClientHeight/2+140+100);
	m_sliRotate.SetAutoBackFill(TRUE);
	m_sliRotate.EnableMoveNotify(TRUE);
	m_sliRotate.EnbaleClickOnlyThumb(TRUE);

	m_btnRotate.Initialize(IDC_IAMGE_BTN_ROTATE,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRotate.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnRotate.SetPos(320,-g_iClientHeight/2+50);
	m_btnRotate.SetIcon(&m_iconRotate);
	//m_btnRotate.SetIconScale(0.8f,0.8f);
	m_btnRotate.SetTouchRate(1.0,1.0);
    m_btnRotate.SetShowScale(0.7f);
#endif
  	m_btnPrev.Initialize(IDC_IMAGE_BTN_PREV,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPrev.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnPrev.SetPos(-192,-g_iClientHeight/2+50);
	m_btnPrev.SetIcon(&m_iconPrev);
	//m_btnPrev.SetIconScale(0.8f,0.8f);
	m_btnPrev.SetTouchRate(1.0,1.0);
    m_btnPrev.SetShowScale(0.7f);

	m_btnNext.Initialize(IDC_IMAGE_BTN_NEXT,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNext.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnNext.SetPos(192,-g_iClientHeight/2+50);
	m_btnNext.SetIcon(&m_iconNext);
	//m_btnNext.SetIconScale(0.8f,0.8f);
	m_btnNext.SetTouchRate(1.0,1.0);
    m_btnNext.SetShowScale(0.7f);


	m_btnPlayPause.Initialize(IDC_IMAGE_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayPause.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnPlayPause.SetPos(-64,-g_iClientHeight/2+50);
	m_btnPlayPause.SetIcon(&m_iconPlay);
	//m_btnPlayPause.SetIconScale(0.8f,0.8f);
	m_btnPlayPause.SetTouchRate(1.0,1.0);
	m_btnPlayPause.SetShowScale(0.7f);


	SIZE siInfo2={g_iScreenWidth,84};

	m_tagTitle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_056),NULL,
		NULL,NULL,
		NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(g_iScreenWidth+10,84);
	//m_tagTitle.SetCaption(_T("13870204516888888"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,g_iClientHeight/2-42);
	m_tagTitle.SetCaptionHeight(18);
	//m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaption(_T(""));
	m_tagTitle.SetSubCaption(_T(""));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT,10,10);
	m_tagTitle.SetSubCaptionPos(BTN_CAPTION_LEFT,10,0);
	m_tagTitle.SetCaptionColor(&cr);

#if (CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS && CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS_BOA)
	m_tagButtonBG.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
		NULL,NULL,
		NULL,NULL,NULL,NULL);
	m_tagButtonBG.SetSize(g_iScreenWidth+10,100);
	m_tagButtonBG.SetPos(0,-g_iClientHeight/2+50);
#endif
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	SIZE sibtn3={100,80};
	m_btnSetWallPaper.Initialize(IDC_IMAGE_BTN_SETWALLPAPER,this,pSkinManager->GetTexture(TEXID_609),NULL,
		pSkinManager->GetTexture(TEXID_608),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSetWallPaper.SetSize(&sibtn3);
	m_btnSetWallPaper.SetIcon(&m_iconWallPaper);
	m_btnSetWallPaper.SetNotifyFunc(&CPanelImageMain::OnNotify,this);
	m_btnSetWallPaper.SetPos(g_iClientWidth/2-70,g_iClientHeight/2-42);
#endif

	//PointUpdate(IMAGE_ACTION_ALL);
	return TRUE;
}


void CALLBACK CPanelImageMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelImageMain *panel=(CPanelImageMain*)lpUserPara;
	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelImageMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: idControl[%d], wMsg[%d], wPara[%d]\r\n"),TEXT(__FUNCTION__),idControl, wMsg, wPara));
	switch (idControl)
	{
	case IDC_IMAGE_BTN_LIBRARY:
		{
			SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_LIBRARY);
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			PlaySoundEffect(0);

		}
		break;
	case IDC_IMAGE_BTN_PREV:
		{
			m_dwShowControlTick = GetTickCount();
			SetTimer(m_hMainWnd,TIMER_IMAGE_LOAD_PREV,10,OnTimerProc);
			m_bEnableFreeDrag = FALSE;
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_IMAGE_BTN_NEXT:
		{
			m_dwShowControlTick = GetTickCount();
			SetTimer(m_hMainWnd,TIMER_IMAGE_LOAD_NEXT,10,OnTimerProc);
			m_bEnableFreeDrag = FALSE;
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			PlaySoundEffect(0);
		}
		break;
    case IDC_IMAGE_BTN_ZOOMIN:
        {
			m_dwShowControlTick = GetTickCount();
            ZoomIn();
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			PlaySoundEffect(0);
        }
        break;
    case IDC_IMAGE_BTN_ZOOMOUT:
        {
			m_dwShowControlTick = GetTickCount();
            ZoomOut();
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			PlaySoundEffect(0);
        }
        break;
    case IDC_IMAGE_BTN_SETWALLPAPER:
		{
			::SendMessage(m_hMainWnd,WM_BACKGROUND_CHANGE,0, (LPARAM)m_strCurPlayFile.String());
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_IAMGE_BTN_BROWSE:
		{

		}
		break;
	case IDC_IMAGE_BTN_ZOOM:
		{
			m_dwShowControlTick = GetTickCount();
			if(m_bRotateSliShow)
			{
				m_bRotateSliShow = FALSE;
			}
			else
			{
				//在render刷新的时候， 放大条会显示
				m_bRotateSliShow = TRUE;
				//m_sliRotate.SetValue(m_nRotateSliValue);
			}

			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
		}
		break;
	case IDC_IMAGE_SLI_ZOOM:
		{
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			Lock();

			if(m_ptagImageDisplay)
			{
				m_bRotateSliShow = TRUE;
				m_bEnableFreeDrag = FALSE;
				m_dwShowControlTick = GetTickCount();
				VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
				m_nRotateSliValue = (UINT)wPara;
				//计算出放大的值，在move的时候这个值会不断更新
				m_fZoomRate = 1.0f+(m_nRotateSliValue * 0.1);

				if(m_fZoomRate == 1.0f)
				{
					SIZE sii=m_ptagImageDisplay->GetTexture()->GetImageSize();
					float fx=(float)g_iScreenWidth/sii.cx;
					float fy=(float)g_iClientHeight/sii.cy;
					float rate=min(fx,fy);
					rate = rate;//min(1.0f,rate);
					VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
					vs.fScaleX=rate;
					vs.fScaleY=rate;
					vs.fRotateX=-180;
					m_ptagImageDisplay->SetCurViewState(&vs);
					m_ptagImageDisplay->SetPos(0,0);
					m_px=m_ptagImageDisplay->GetPosX();
					m_py=m_ptagImageDisplay->GetPosY();
					m_bEnableFreeDrag = FALSE;
				}
				else
				{
					//更新放大后的图片
					vs.fScaleX=m_fZoomRate;
					vs.fScaleY=m_fZoomRate;
					m_ptagImageDisplay->SetCurViewState(&vs);
				}
				//printf("MSG:m_fZoomRate[%.2f]\r\n",m_fZoomRate);
				m_bEnableFreeDrag = (m_fZoomRate==1.0f)?FALSE:TRUE;
			}
			Unlock();
		}
		break;
	case IDC_IAMGE_BTN_ROTATE:
		{
			if (TRUE == m_bImagePlaying)
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			Lock();
			m_dwShowControlTick = GetTickCount();
			if(m_ptagImageDisplay)
			{
				VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
				vs.fRotateZ += 90;
				if(vs.fRotateZ>360)
					vs.fRotateZ = 0;
				m_ptagImageDisplay->SetCurViewState(&vs);
			}
			Unlock();
		}
		break;
	case IDC_IMAGE_BTN_PLAY:
		{
			if (m_bImagePlaying == FALSE)
			{
				m_bImagePlaying = TRUE;
				m_btnPlayPause.SetIcon(&m_iconPause);
				//设置定时器，4秒播放一次
				SetTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY,4000,ImageAutoPlayTimerProc);

			}
			else
			{
				m_bImagePlaying = FALSE;
				m_btnPlayPause.SetIcon(&m_iconPlay);
				KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
			}
			PlaySoundEffect(0);
		}
		break;
	default:
		break;
	}
}

#if 0

BOOL CPanelImageMain::LoadFile(const wchar_t *pFileName, CLabelGL * ptagImage, int cx, int cy)
{
	if(!pFileName || !ptagImage)
		return FALSE;

	//File
	do
	{
//	        try
//			{
//				ptagImage = new CLabelGL;
//			}
//			catch (const std::bad_alloc &)
//			{
//				RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: No memory for new CLabelGL!\n"),TEXT(__FUNCTION__)));
//				ptagImage = NULL;
//				return FALSE;
//			}
//			// 	VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
//			// 	vs.fRotateX=-180;
//			// 	m_ptagImageDisplay->SetCurViewState(&vs);
//
//			// 	m_ptagImageDisplay->SetAniSpeed(15);

		SIZE si={0,0};
		//wchar_t str[32];

		if(!ptagImage->Initialize(0,this,pFileName,cx,cy,&si))
		{
			RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: Initialize [%s] failed!\n"),TEXT(__FUNCTION__),pFileName));
			return FALSE;
		}

		//set rate:
		SIZE sii=ptagImage->GetTexture()->GetImageSize();
		float fx=(float)g_iScreenWidth/sii.cx;
		float fy=(float)g_iClientHeight/sii.cy;
		float rate=min(fx,fy);
		rate = min(1.0f,rate);
		//m_sliImageScale.SetValue(rate*100-50);
	}while(0);

	return TRUE;
}

BOOL CPanelImageMain::LoadPrevImage(const wchar_t *pFileName)
{
	BOOL bRet;
	if(!pFileName)
	{
		m_bImagePrevIsEmpty = TRUE;
		return FALSE;
	}

	RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Prev[%X] pFileName[%s]\n"),TEXT(__FUNCTION__),m_ptagImageDisplayPrev,pFileName));
	Lock();
	if(m_ptagImageDisplayPrev == NULL)
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: m_ptagImageDisplayPrev is NULL !!!!!!!!!!!!!!!!!!!!!!!!!!\n"),TEXT(__FUNCTION__)));
		//PointUpdate(IMAGE_ACTION_RIGHT_MOVE);
        try
		{
			m_ptagImageDisplayPrev = new CLabelGL;
		}
		catch (const std::bad_alloc &)
		{
			RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: No memory for new CLabelGL!\n"),TEXT(__FUNCTION__)));
			m_ptagImageDisplayPrev = NULL;
			Unlock();
			return FALSE;
		}
	}

	m_bImagePrevIsEmpty = FALSE;
	//Prev File
	bRet = LoadFile(pFileName, m_ptagImageDisplayPrev, 128,80);
	if(!bRet)
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: LoadFile[%s] failed!!!\n"),TEXT(__FUNCTION__),pFileName));
		m_bImage1Valid = FALSE;
		return FALSE;
	}
	m_ptagImageDisplayPrev->SetAlpha(1.0f,TRUE);
	m_ptagImageDisplayPrev->SetPosIni(-810,0);

	//memcpy(m_pImageNode[0].strFilePath,pFileName,sizeof(m_pImageNode[0].strFilePath));
	if(GetFileNameFromPath(pFileName,m_pImageNode[0].strFileName,MAX_PATH))
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Prev strFileName[%s]\n"),TEXT(__FUNCTION__),m_pImageNode[0].strFileName));
	}
	//RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: m_pImageNode[0].strFilePath[%s]\n"),TEXT(__FUNCTION__),m_pImageNode[0].strFilePath));

	Unlock();

	m_bImage1Valid = TRUE;

	return bRet;
}

BOOL CPanelImageMain::ReleaseImageHD()
{
	m_bImageDisplayHDReady = FALSE;
	m_bImageDisplayHDShow = FALSE;
	if(m_ptagImageDisplayHD)
	{
		delete m_ptagImageDisplayHD;
		m_ptagImageDisplayHD = NULL;
	}
	return TRUE;
}

BOOL CPanelImageMain::LoadImageHD(const wchar_t *pFileName)
{
	BOOL bRet;
	if(!m_pImageNode[1].strFileName)
	{
		return FALSE;
	}

	RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: LoadFile[%s]\n"),TEXT(__FUNCTION__),pFileName));

	Lock();

	if(m_ptagImageDisplayHD == NULL)
	{
        try
		{
			m_ptagImageDisplayHD = new CLabelGL;
		}
		catch (const std::bad_alloc &)
		{
			RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: No memory for new CLabelGL!!!!!!!!!!!!!!!!!!!!!!!!!!\n"),TEXT(__FUNCTION__)));
			m_ptagImageDisplayHD = NULL;
			Unlock();
			return FALSE;
		}
	}

	bRet =  LoadFile(pFileName, m_ptagImageDisplayHD,0 ,0);
	if(!bRet)
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: LoadFile[%s] failed!!!\n"),TEXT(__FUNCTION__),pFileName));
		return FALSE;
		//m_ptagImageDisplay = NULL;
	}
	m_ptagImageDisplayHD->SetAlpha(1.0f,TRUE);
	m_ptagImageDisplayHD->SetPosIni(0,0);
	m_bImageDisplayHDReady = TRUE;

	Unlock();
	return TRUE;

}

BOOL CPanelImageMain::LoadImage(const wchar_t *pFileName)
{
	BOOL bRet;
	if(!pFileName)
	{
		return FALSE;
	}

	Lock();
	RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Cur[%X] pFileName[%s]\n"),TEXT(__FUNCTION__),m_ptagImageDisplay,pFileName));

	if(m_ptagImageDisplay == NULL)
	{
        try
		{
			m_ptagImageDisplay = new CLabelGL;
		}
		catch (const std::bad_alloc &)
		{
			RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: No memory for new CLabelGL!!!!!!!!!!!!!!!!!!!!!!!!!!\n"),TEXT(__FUNCTION__)));
			m_ptagImageDisplay = NULL;
			Unlock();
			return FALSE;
		}
	}

	bRet =  LoadFile(pFileName, m_ptagImageDisplay, 128, 80);
	if(!bRet)
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: LoadFile[%s] failed!!!\n"),TEXT(__FUNCTION__),pFileName));
		m_bImage2Valid = FALSE;
		return FALSE;
	}
	m_ptagImageDisplay->SetAlpha(1.0f,TRUE);

	m_ptagImageDisplay->SetPosIni(0,0);

	//memcpy(m_pImageNode[1].strFilePath,pFileName,sizeof(m_pImageNode[1].strFilePath));
	//memcpy(m_strImageDisplayPath, pFileName, sizeof(m_strImageDisplayPath));

	if(GetFileNameFromPath(pFileName,m_pImageNode[1].strFileName,MAX_PATH))
	{
		//RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Cur strFileName[%s]\n"),TEXT(__FUNCTION__),m_pImageNode[1].strFileName));
		m_tagTitle.SetCaption(m_pImageNode[1].strFileName);
	}
	RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]:m_strImageDisplayPath[%s]\n"),TEXT(__FUNCTION__),m_strImageDisplayPath));
	m_bImage2Valid = TRUE;

	Unlock();

	SetTimer(m_hMainWnd,TIMER_IMAGE_LOAD_CUR_HD,800,OnTimerProc);
//		SetTimer(m_hMainWnd,TIMER_IMAGE_HD_SHOW,500,OnTimerProc);

	return TRUE;

}

BOOL CPanelImageMain::LoadNextImage(const wchar_t *pFileName)
{
	BOOL bRet;

	if(!pFileName)
	{
		m_bImageNextIsEmpty = TRUE;
		return FALSE;
	}

	RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Next[%X] pFileName[%s]\n"),TEXT(__FUNCTION__),m_ptagImageDisplayNext,pFileName));
	Lock();
	if(m_ptagImageDisplayNext == NULL)
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: m_ptagImageDisplayNext is NULL\n"),TEXT(__FUNCTION__)));
//			PointUpdate(IMAGE_ACTION_LEFT_MOVE);
        try
		{
			m_ptagImageDisplayNext = new CLabelGL;
		}
		catch (const std::bad_alloc &)
		{
			RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: No memory for new CLabelGL!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"),TEXT(__FUNCTION__)));
			m_ptagImageDisplayNext = NULL;
			Unlock();
			return FALSE;
		}
	}

	m_bImageNextIsEmpty = FALSE;

	bRet =  LoadFile(pFileName, m_ptagImageDisplayNext, 128, 80);
	if(!bRet)
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: LoadFile[%s] failed!!!\n"),TEXT(__FUNCTION__),pFileName));
		m_bImage3Valid = FALSE;
		return FALSE;
	}

	m_ptagImageDisplayNext->SetAlpha(1.0f,TRUE);
	m_ptagImageDisplayNext->SetPosIni(810,0);

	//memcpy(m_pImageNode[2].strFilePath,pFileName,sizeof(m_pImageNode[2].strFilePath));

	if(GetFileNameFromPath(pFileName,m_pImageNode[2].strFileName,MAX_PATH))
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Prev strFileName[%s]\n"),TEXT(__FUNCTION__),m_pImageNode[2].strFileName));
	}
	//RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: m_pImageNode[2].strFilePath[%s]\n"),TEXT(__FUNCTION__),m_pImageNode[2].strFilePath));

	m_bImage3Valid = TRUE;

	Unlock();

	return TRUE;
}
#endif

BOOL CPanelImageMain::LoadImageFile(const wchar_t *pFileName)
{
	if(!pFileName)
	{
		m_bShowControl = TRUE;
		m_tagTitle.SetCaption(GetResourceString(IDS_NOWPLAYING_INVALIDMEDIA));
		m_tagTitle.SetSubCaption(_T(""));
		return FALSE;
	}

	m_bImageLoadReady = FALSE;

	Lock();
	RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Cur[%X] pFileName[%s]\n"),TEXT(__FUNCTION__),m_ptagImageDisplay,pFileName));

	//=== fix memory use no release,always increase begin====
	if(m_ptagImageDisplay)
	{
		delete m_ptagImageDisplay;
		m_ptagImageDisplay=NULL;
	}
	m_ptagImageDisplay = new CLabelGL;

	if(!m_ptagImageDisplay)
	{
        Unlock();
		return FALSE;
	}
	//=======fix memory use no release,always increase End=========

	SIZE si={0,0};
	//wchar_t str[32];

	if(!m_ptagImageDisplay->Initialize(0,this,pFileName,0,0,&si))
	{
		RETAILMSG(DEBUG_IMAGE,(TEXT("ERR:[%s]: Initialize [%s] failed!\n"),TEXT(__FUNCTION__),pFileName));
		m_bShowControl = TRUE;
		if(GetFileNameFromPath(pFileName,m_pImageNode[1].strFileName,MAX_PATH))
		{
			//RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Cur strFileName[%s]\n"),TEXT(__FUNCTION__),m_pImageNode[1].strFileName));
			m_tagTitle.SetCaption(m_pImageNode[1].strFileName);
		}
		m_tagTitle.SetSubCaption(GetResourceString(IDS_NOWPLAYING_INVALIDMEDIA));
        Unlock();
		return FALSE;
	}

	m_strCurPlayFile = pFileName;

	//set rate:
	SIZE sii=m_ptagImageDisplay->GetTexture()->GetImageSize();
	float fx=(float)g_iScreenWidth/sii.cx;
	float fy=(float)g_iClientHeight/sii.cy;
	float rate=min(fx,fy);
	rate = min(1.0f,rate);

	m_ptagImageDisplay->SetAlpha(1.0f,TRUE);

	m_ptagImageDisplay->SetPosIni(0,0);

	wchar_t strSI[MAX_PATH];
	swprintf_s(strSI,MAX_PATH,_T("%d x %d"),sii.cx,sii.cy);
	m_tagTitle.SetSubCaption(strSI);


	VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
	vs.fScaleX=rate;
	vs.fScaleY=rate;
	vs.fRotateX=-180;
	vs.fRotateZ = 0;
	m_ptagImageDisplay->SetCurViewState(&vs);

	m_fZoomRate = rate;
	//memcpy(m_pImageNode[1].strFilePath,pFileName,sizeof(m_pImageNode[1].strFilePath));
	//memcpy(m_strImageDisplayPath, pFileName, sizeof(m_strImageDisplayPath));

	if(GetFileNameFromPath(pFileName,m_pImageNode[1].strFileName,MAX_PATH))
	{
		//RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]: Cur strFileName[%s]\n"),TEXT(__FUNCTION__),m_pImageNode[1].strFileName));
		m_tagTitle.SetCaption(m_pImageNode[1].strFileName);
	}
	//RETAILMSG(DEBUG_IMAGE,(TEXT("MSG:[%s]:m_strImageDisplayPath[%s]\n"),TEXT(__FUNCTION__),m_strImageDisplayPath));

	Unlock();

	m_bImageLoadReady = TRUE;

	return TRUE;
}

void CPanelImageMain::ZoomIn()
{
	if(!m_bImageLoadReady || IsLock())
	{
		RETAILMSG(1, (TEXT("ERR:[%s] Not ready\r\n"), TEXT(__FUNCTION__)));
		return;
	}

	Lock();

	m_fZoomRate-=0.05;
	if(m_fZoomRate<1.0f)
		m_fZoomRate=1.0f;

	if(m_ptagImageDisplay)
	{
		if(m_fZoomRate == 1.0f)
		{
			SIZE sii=m_ptagImageDisplay->GetTexture()->GetImageSize();
			float fx=(float)g_iScreenWidth/sii.cx;
			float fy=(float)g_iClientHeight/sii.cy;
			float rate=min(fx,fy);
			rate = rate;//min(1.0f,rate);
			VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
			vs.fScaleX=rate;
			vs.fScaleY=rate;
			vs.fRotateX=-180;
			m_ptagImageDisplay->SetCurViewState(&vs);
			m_ptagImageDisplay->SetPos(0,0);
			m_px=m_ptagImageDisplay->GetPosX();
			m_py=m_ptagImageDisplay->GetPosY();
			m_bEnableFreeDrag = FALSE;
		}
		else
		{
			VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
			vs.fScaleX=m_fZoomRate;
			vs.fScaleY=m_fZoomRate;
			m_ptagImageDisplay->SetCurViewState(&vs);

	//	        if(vs.fScaleX == vs.fScaleY == 1.0)
	//	            m_bEnableFreeDrag = FALSE;
		}
	}
	Unlock();

}

#if 0
void CPanelImageMain::PointUpdate(UINT uAction)
{
	Lock();
	if(uAction == IMAGE_ACTION_ALL)
	{
	}
	else if(uAction == IMAGE_ACTION_LEFT_MOVE)
	{
		LeftMove(1);
	}
	else if(uAction == IMAGE_ACTION_RIGHT_MOVE)
	{
		RightMove(1);
	}
	m_ptagImageDisplayPrev =m_pImageNode[0].ptagImage;
	m_ptagImageDisplay = m_pImageNode[1].ptagImage;
	m_ptagImageDisplayNext= m_pImageNode[2].ptagImage;

	m_bImage2Valid = TRUE;
	m_bImageDisplayHDShow = FALSE;
	RETAILMSG(1,(TEXT("MSG:[%s]: uAction[%d] Prev[%X] Cur[%X] Next[%X]\r\n"),TEXT(__FUNCTION__),uAction,m_ptagImageDisplayPrev,m_ptagImageDisplay,m_ptagImageDisplayNext));
	Unlock();
}

void CPanelImageMain::InforUpdate()
{
	m_tagTitle.SetCaption(m_pImageNode[1].strFileName);
	//RETAILMSG(1,(TEXT("MSG:[%s]: Image name[%s]\r\n"),TEXT(__FUNCTION__),m_pImageNode[1].strFileName));
}

void CPanelImageMain::RightMoveRelease(int n)
{
	Lock();
	m_bImage1Valid = FALSE;
	for(int i = 0; i < n; i++)
	{
		if(m_pImageNode[i].ptagImage)
		{
			//m_pImageNode[i].bImageValid = FALSE;
			delete m_pImageNode[i].ptagImage;
			m_pImageNode[i].ptagImage = new CLabelGL;
			//m_pImageNode[i].bImageValid = TRUE;
			RETAILMSG(1,(TEXT("MSG:[%s]: delete m_pImageNode[%d]\r\n"),TEXT(__FUNCTION__),i));
		}
	}
	m_bImage1Valid = TRUE;
	Unlock();
}

void CPanelImageMain::LeftMoveRelease(int n)
{
	Lock();
	m_bImage3Valid = FALSE;
	for(int i = IMAGE_PRLOAD_NUM-n; i < IMAGE_PRLOAD_NUM; i++)
	{
		if(m_pImageNode[i].ptagImage)
		{
			//m_pImageNode[i].bImageValid = FALSE;
			delete m_pImageNode[i].ptagImage;
			m_pImageNode[i].ptagImage = new CLabelGL;
			//m_pImageNode[i].bImageValid = TRUE;
			RETAILMSG(1,(TEXT("MSG:[%s]: delete m_pImageNode[%d]\r\n"),TEXT(__FUNCTION__),i));
		}
	}
	m_bImage3Valid = TRUE;
	Unlock();
}

void CPanelImageMain::RightMove(int n)
{
	//CLabelGL* pD[IMAGE_PRLOAD_NUM];
	IMAGE_NODE node[IMAGE_PRLOAD_NUM];

	Lock();
	for(int i = 0; i < IMAGE_PRLOAD_NUM; i ++)
	{
	    node[i].ptagImage = m_pImageNode[(i - n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM].ptagImage;
	    memcpy(node[i].strFileName,m_pImageNode[(i - n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM].strFileName,sizeof(m_pImageNode[(i - n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM].strFileName));
	}

	memcpy(&m_pImageNode,&node,sizeof(node));
	Unlock();
}

void CPanelImageMain::LeftMove(int n)
{
	//CLabelGL* pD[IMAGE_PRLOAD_NUM];
	IMAGE_NODE node[IMAGE_PRLOAD_NUM];

	Lock();

	for(int i = 0; i < IMAGE_PRLOAD_NUM; i ++)
	{
	    //pD[i] = m_pImage[(i + n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM];
	    //node[i].ptagImage = m_pImageNode[(i + n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM].ptagImage;
	    //memcpy(node[i].strFileName,m_pImageNode[(i + n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM].strFileName,sizeof(m_pImageNode[(i + n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM].strFileName));
		memcpy(&node[i],&m_pImageNode[(i + n + IMAGE_PRLOAD_NUM)%IMAGE_PRLOAD_NUM],sizeof(node[i]));
	}

	//memcpy(m_pImage,pD,sizeof(m_pImage));
	memcpy(&m_pImageNode,&node,sizeof(node));
	Unlock();
}
#endif

void CPanelImageMain::ZoomOut()
{
	if(!m_bImageLoadReady || IsLock())
	{
		RETAILMSG(1, (TEXT("ERR:[%s] Not ready\r\n"), TEXT(__FUNCTION__)));
		return;
	}

	Lock();
	m_fZoomRate+=0.05;
	if(m_fZoomRate>3.0f)
		m_fZoomRate=3.0f;

	if(m_ptagImageDisplay)
	{
		VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
		vs.fScaleX=m_fZoomRate;
		vs.fScaleY=m_fZoomRate;
		m_ptagImageDisplay->SetCurViewState(&vs);
		m_bEnableFreeDrag = TRUE;
	}
	Unlock();

}

void CPanelImageMain::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	static BOOL bPointTouchDown;
	static BOOL bPointTouchUp;
	static UINT nAbslOld;

	int nPointStatus = pCyMultiPoint->sPointStatus.pointstatus;
	int nPointTouchArea = pCyMultiPoint->sPointStatus.toucharea;
	RETAILMSG(1, (TEXT("MSG:[%s]:nPointStatus[%X] nPointTouchArea[%X]\r\n"),TEXT(__FUNCTION__),nPointStatus,nPointTouchArea));

	if((nPointTouchArea != TOUCH_AREA_GFX_WINDOW && (nPointStatus != TOUCH_STATUS_UP  && nPointStatus != GEST_SINGLE_TOUCH_END && nPointStatus != GEST_SINGLE_TOUCH_DOUBLE_CLICK)) || !m_bImageLoadReady)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]:nPointTouchArea[%d] m_bImageLoadReady[%d]\r\n"),TEXT(__FUNCTION__),nPointTouchArea,m_bImageLoadReady));
		return;
	}

	if(nPointStatus == TOUCH_STATUS_UP || nPointStatus == GEST_SINGLE_TOUCH_END || nPointStatus == GEST_SINGLE_TOUCH_DOUBLE_CLICK)
	{
		RETAILMSG(1, (TEXT("MSG:[%s]:Up\r\n"),TEXT(__FUNCTION__)));
		//UP
		bPointTouchDown = FALSE;

		DWORD dwTempTick = GetTickCount();
		if(dwTempTick-m_dwTouchDownTick < 250 && (nPointStatus == GEST_SINGLE_TOUCH_END || nPointStatus == TOUCH_STATUS_UP) && (!m_bShowControl || (m_bShowControl && m_ptPenDown.y <= 380)))
		{
			if(m_bShowControl)
			{
				//如果在显示smartbar，则不显示
				m_bShowControl = FALSE;
			}
			else
			{
				//每次抬起时m_bShowControl为真，表示显示smartbar
				m_bShowControl = TRUE;
				//记住当前的时间，在render不断刷新的时候会判断超过的时间
				//如果在5秒内没有按下，重新刷新m_dwShowControlTick的值，则会隐藏smartbar
				m_dwShowControlTick = dwTempTick;
			}
		}
		if(!m_bEnableFreeDrag && m_iMoveRange!=0)
		{
			if((m_iMoveRange>0 && m_iMoveRange <250))
			{
				RETAILMSG(1, (TEXT("MSG:[%s]:Right Restore \r\n"),TEXT(__FUNCTION__)));
				//m_iMoveRange = 0;
				Lock();
				if(m_ptagImageDisplay)
					m_ptagImageDisplay->SetPosIni(0,0);
				Unlock();
			}
			else if(m_iMoveRange>250)
			{
				RETAILMSG(1, (TEXT("MSG:[%s]:Right Change \r\n"),TEXT(__FUNCTION__)));
				Lock();
				if(m_ptagImageDisplay)
				{
					m_ptagImageDisplay->SetPosIni(1500,0);
				}
				Unlock();
				//抬起手发送TIMER_IMAGE_LOAD_PREV命令
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
				SetTimer(m_hMainWnd,TIMER_IMAGE_LOAD_PREV,200,OnTimerProc);
#else
				SetTimer(m_hMainWnd,TIMER_IMAGE_LOAD_NEXT,200,OnTimerProc);
#endif
				m_bEnableFreeDrag = FALSE;
				PlaySoundEffect(0);
			}
			else if((m_iMoveRange<0 && m_iMoveRange>-250))
			{
				RETAILMSG(1, (TEXT("MSG:[%s]:Left Restore \r\n"),TEXT(__FUNCTION__)));
				//m_iMoveRange = 0;
				Lock();
				if(m_ptagImageDisplay)
					m_ptagImageDisplay->SetPosIni(0,0);
				Unlock();
			}
			else if(m_iMoveRange<-250)
			{
				RETAILMSG(1, (TEXT("MSG:[%s]:Left Change \r\n"),TEXT(__FUNCTION__)));
				Lock();
				if(m_ptagImageDisplay)
					m_ptagImageDisplay->SetPosIni(-1500,0);
				Unlock();
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
				SetTimer(m_hMainWnd,TIMER_IMAGE_LOAD_NEXT,200,OnTimerProc);
#else
				SetTimer(m_hMainWnd,TIMER_IMAGE_LOAD_PREV,200,OnTimerProc);
#endif
				m_bEnableFreeDrag = FALSE;
				PlaySoundEffect(0);
			}
		}
//		m_bGetFirstDown = FALSE;
		m_bFirstMove = FALSE;
		memset(&m_ptPoint1,0,sizeof(m_ptPoint1));
		memset(&m_ptPoint2,0,sizeof(m_ptPoint2));
		m_iMoveRange = 0;
		return;
	}
	else if(!bPointTouchDown && (nPointStatus == TOUCH_STATUS_DOWN_START || nPointStatus == TOUCH_STATUS_UNKNOW))
	{
		//DOWN
		bPointTouchDown = TRUE;
		BOOL bInTouchDownRegion;
		UINT tempPx,tempPy,tempDx,tempDy;
		RETAILMSG(1, (TEXT("MSG:[%s]:Down\r\n"),TEXT(__FUNCTION__)));
		DWORD dwTempTick = GetTickCount();

		tempPx = pCyMultiPoint->sPointCoordinate[0].PointX;
		tempPy = pCyMultiPoint->sPointCoordinate[0].PointY;

		tempDx = abs(m_ptPenDown.x-tempPx);
		tempDy = abs(m_ptPenDown.y-tempPy);
		bInTouchDownRegion = (sqrt(tempDx*tempDx+tempDy*tempDy)<50);

        m_ptPenDown.x = tempPx;
        m_ptPenDown.y = tempPy;

		if(m_ptagImageDisplay)
		{
			m_px=m_ptagImageDisplay->GetPosX();
			m_py=m_ptagImageDisplay->GetPosY();
			RETAILMSG(1, (TEXT("MSG:Down m_px[%d] m_py[%d]\r\n"),m_px,m_py));
		}
#if 0
		if(m_ptagImageDisplayPrev)
		{
			m_Prevpx=m_ptagImageDisplayPrev->GetPosX();
			//RETAILMSG(1, (TEXT("MSG:Down m_px[%d] m_py[%d]\r\n"),m_px,m_py));
		}
		if(m_ptagImageDisplayNext)
		{
			m_Nextpx=m_ptagImageDisplayNext->GetPosX();
			//RETAILMSG(1, (TEXT("MSG:Down m_px[%d] m_py[%d]\r\n"),m_px,m_py));
		}
#endif
		//m_bShowControl = TRUE;
		//双击恢复原图大小
		if(!m_bDoubleClick && (dwTempTick-m_dwTouchDownTick<300) && bInTouchDownRegion && m_ptPenDown.y<380 && m_ptPenDown.y>84)
		{
			RETAILMSG(1, (TEXT("MSG:[%s]:Double Click!!! Tick[%d]\r\n"),TEXT(__FUNCTION__),dwTempTick-m_dwTouchDownTick));
			Lock();
			if(m_ptagImageDisplay)
			{
				SIZE sii=m_ptagImageDisplay->GetTexture()->GetImageSize();
				float fx=(float)g_iScreenWidth/sii.cx;
				float fy=(float)g_iClientHeight/sii.cy;
				float rate=min(fx,fy);
				rate = rate;//min(1.0f,rate);
				VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
				vs.fScaleX=rate;
				vs.fScaleY=rate;
				vs.fRotateX=-180;
				m_ptagImageDisplay->SetCurViewState(&vs);
				m_ptagImageDisplay->SetPosIni(0,0);
				m_px=m_ptagImageDisplay->GetPosX();
				m_py=m_ptagImageDisplay->GetPosY();
				m_bEnableGestureSwitch = TRUE;
				m_fZoomRate = 1.0f;
#if (CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS && CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS_BOA)
				m_sliRotate.SetValue(0);
#endif
				PlaySoundEffect(0);
			}
			Unlock();
			m_bDoubleClick = TRUE;
			m_bEnableFreeDrag = FALSE;
		}
		else
		{
			RETAILMSG(1, (TEXT("MSG:[%s]:m_bDoubleClick[%d] Tick[%d] bInTouchDownRegion[%d]\r\n"),TEXT(__FUNCTION__),m_bDoubleClick,dwTempTick-m_dwTouchDownTick,bInTouchDownRegion));
			m_bDoubleClick = FALSE;
		}

		//m_dwShowControlTick = dwTempTick;
		m_dwTouchDownTick = dwTempTick;
	}
	else
	{
		//MOVE
		RETAILMSG(1, (TEXT("MSG:[%s]:Move\r\n"),TEXT(__FUNCTION__)));
		if (TRUE == m_bImagePlaying)
		{
			m_bImagePlaying = FALSE;
			m_btnPlayPause.SetIcon(&m_iconPlay);
			KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
		}
		m_ptPoint1.x = pCyMultiPoint->sPointCoordinate[0].PointX;
		m_ptPoint1.y = pCyMultiPoint->sPointCoordinate[0].PointY;

		//双手滑动放大
		if(pCyMultiPoint->sPointStatus.pointnum > 1 && pCyMultiPoint->sPointCoordinate[1].Valid)
		{
			m_ptPoint2.x = pCyMultiPoint->sPointCoordinate[1].PointX;
			m_ptPoint2.y = pCyMultiPoint->sPointCoordinate[1].PointY;
			RETAILMSG(1, (TEXT("MSG:P1[%d,%d] P2[%d,%d]\r\n"),m_ptPoint1.x,m_ptPoint1.y,m_ptPoint2.x,m_ptPoint2.y));

			UINT nabsx = abs(m_ptPoint1.x-m_ptPoint2.x);
			UINT nabsy = abs(m_ptPoint1.y-m_ptPoint2.y);
			UINT nAbsNew = sqrt(nabsx*nabsx+nabsy*nabsy);

			if(!m_bFirstMove)
			{
				m_bFirstMove = TRUE;
				nAbslOld = nAbsNew;
			}

			UINT nabsL = abs(nAbsNew-nAbslOld);

			RETAILMSG(1, (TEXT("MSG:nabsx[%d] nabsy[%d] nAbsNew[%d] nAbslOld[%d]\r\n"),nabsx,nabsy,nAbsNew,nAbslOld));

			if(nabsL>20 && nAbsNew != nAbslOld)
			{
				if(nAbsNew > nAbslOld)
				{
					m_fZoomRate += ((float)0.05*(nabsL/10));
				}
				else
				{
					m_fZoomRate -= ((float)0.05*(nabsL/10));
				}

				RETAILMSG(1, (TEXT("MSG:nAbsNew[%d] nAbslOld[%d] m_fZoomRate[%0.2f]\r\n"),nAbsNew,nAbslOld,m_fZoomRate));
				//printf("MSG:nAbsNew[%d] nAbslOld[%d] m_fZoomRate[%0.2f]\r\n",nAbsNew,nAbslOld,m_fZoomRate);

				nAbslOld = nAbsNew;

				if(m_fZoomRate<1.0f)
					m_fZoomRate=1.0f;
				else if(m_fZoomRate>3.0f)
					m_fZoomRate=3.0f;

				Lock();
				if(m_ptagImageDisplay)
				{
					m_bEnableGestureSwitch = FALSE;

					if(m_fZoomRate == 1.0f)
					{
						SIZE sii=m_ptagImageDisplay->GetTexture()->GetImageSize();
						float fx=(float)g_iScreenWidth/sii.cx;
						float fy=(float)g_iClientHeight/sii.cy;
						float rate=min(fx,fy);
						rate = rate;//min(1.0f,rate);
						VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
						vs.fScaleX=rate;
						vs.fScaleY=rate;
						vs.fRotateX=-180;
						m_ptagImageDisplay->SetCurViewState(&vs);
						m_ptagImageDisplay->SetPosIni(0,0);
						m_px=m_ptagImageDisplay->GetPosX();
						m_py=m_ptagImageDisplay->GetPosY();
						m_bEnableFreeDrag = FALSE;
					}
					else
					{
						VIEW_STATE vs=m_ptagImageDisplay->GetCurViewState();
						vs.fScaleX=m_fZoomRate;
						vs.fScaleY=m_fZoomRate;
						//更新放大后的图片
						m_ptagImageDisplay->SetCurViewState(&vs);
						m_bEnableFreeDrag = TRUE;
					}
#if (CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS && CUSTOMER_ID != CUSTOMER_SOUNDEXPRESS_BOA)
					//更新放大条的值
					m_nRotateSliValue = (UINT)((m_fZoomRate-1.0f)/0.1f);
					m_sliRotate.SetValue(m_nRotateSliValue);
#endif
					//printf("MSG:SetValue[%d] m_fZoomRate[%.2f]\r\n",m_nRotateSliValue,m_fZoomRate);
				}
				Unlock();


			}
		}
		//拖到放大条放大，拖到图片
		else
		{
			m_ptPoint2.x = 0;
			m_ptPoint2.y = 0;
			m_bFirstMove = FALSE;
			//nAbslOld = 0;
			if(m_bEnableFreeDrag)
			{
				if(m_bShowControl)
				{
					if(m_ptPoint1.y>380 || m_ptPoint1.y<84 || IsLock())
					{
						return;
					}
				}
				Lock();
				if(m_ptagImageDisplay)
				{
					m_ptagImageDisplay->SetPosIni(m_px+m_ptPoint1.x-m_ptPenDown.x,m_py-m_ptPoint1.y+m_ptPenDown.y);
					RETAILMSG(1, (TEXT("MSG:Move:[%d,%d] m_pxy[%d,%d] m_ptPoint1.xy[%d,%d] m_ptPenDown.xy[%d,%d]\r\n"),m_px+m_ptPoint1.x-m_ptPenDown.x,m_py-m_ptPoint1.y+m_ptPenDown.y,m_px,m_py,m_ptPoint1.x,m_ptPoint1.y,m_ptPenDown.x,m_ptPenDown.y));
				}
				//在放大条控件回调函数里更新图片
				Unlock();
			}
			else
			{
				if(m_bShowControl)
				{
					if(m_ptPoint1.y>380 || m_ptPoint1.y<84 || IsLock() || abs(m_ptPoint1.x-m_ptPenDown.x)<50)
					{
						m_iMoveRange = 0;
						return;
					}
				}
				m_iMoveRange = m_ptPoint1.x-m_ptPenDown.x;
				Lock();
				if(m_ptagImageDisplay)
				{
					m_ptagImageDisplay->SetPosIni(m_px+m_iMoveRange,0);
				}
#if 0
				if(m_ptagImageDisplayPrev)
					m_ptagImageDisplayPrev->SetPos(m_Prevpx+m_iMoveRange,0);
				if(m_ptagImageDisplayNext)
					m_ptagImageDisplayNext->SetPos(m_Nextpx+m_iMoveRange,0);
#endif
				Unlock();
			}

		}
	}
}

void CPanelImageMain::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: idEvent[%d]\r\n"),TEXT(__FUNCTION__),idEvent));
    switch(idEvent)
    {
    case TIMER_IMAGE_LOAD_PREV:
		SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_PREVIOUS);
		KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_LOAD_PREV);
		break;
	case TIMER_IMAGE_LOAD_NEXT:
		SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_NEXT);
		KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_LOAD_NEXT);
		break;
//	    case TIMER_IMAGE_LOAD_PREV:
//			g_pCPanelImage->RightMoveRelease(1);
//			SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_PRELOAD_PREVIOUS);
//			KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_LOAD_PREV);
//			break;
//		case TIMER_IMAGE_LOAD_NEXT:
//			g_pCPanelImage->LeftMoveRelease(1);
//			SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_PRELOAD_NEXT);
//			KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_LOAD_NEXT);
//			break;
//		case TIMER_IMAGE_LOAD_CUR_HD:
//			RETAILMSG(1, (TEXT("MSG:[%s]:TIMER_IMAGE_LOAD_CUR_HD\r\n"),TEXT(__FUNCTION__)));
//			g_pCPanelImage->m_dwImageDisplayHDLoadTick = GetTickCount();
//			g_pCPanelImage->ReleaseImageHD();
//			//g_pCPanelImage->LoadImageHD();
//			SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_PLAY);
//			KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_LOAD_CUR_HD);
//			break;
//		case TIMER_IMAGE_HD_SHOW:
//			//RETAILMSG(1, (TEXT("MSG:[%s]:TIMER_IMAGE_HD_SHOW\r\n"),TEXT(__FUNCTION__)));
//
//			if(g_pCPanelImage->m_bImageDisplayHDReady && g_pCPanelImage->IsReady())
//			{
//				g_pCPanelImage->m_bImageDisplayHDShow = TRUE;
//				//g_pCPanelImage->m_ptagImageDisplay->Show(FALSE);
//				g_pCPanelImage->m_bImage2Valid = FALSE;
//				g_pCPanelImage->m_ptagImageDisplay = g_pCPanelImage->m_ptagImageDisplayHD;
//				//KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_HD_SHOW);
//			}
//	//			if(GetTickCount() - g_pCPanelImage->m_dwImageDisplayHDLoadTick > 4000)
//	//			{
//	//				g_pCPanelImage->m_bImageDisplayHDShow = FALSE;
//	//				KillTimer(g_pCPanelImage->m_hMainWnd,TIMER_IMAGE_HD_SHOW);
//	//			}
//			break;
    }

}


void CPanelImageMain::ImageAutoPlayTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	switch(idEvent)
	{
	case TIMER_IMAME_AUTO_PLAY:
		{
			SendImageCommand(IMAGE_CMD_PLAY_CONTROL,IMAGE_CONTROL_NEXT);
		}
		break;
	default:
		break;
	}
}

void CPanelImageMain::ImagePlayAbort()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: m_bImagePlaying[%d]\r\n"),TEXT(__FUNCTION__),m_bImagePlaying));

	if (TRUE == m_bImagePlaying)
	{
		m_bImagePlaying = FALSE;
		m_btnPlayPause.SetIcon(&m_iconPlay);
		KillTimer(m_hMainWnd,TIMER_IMAME_AUTO_PLAY);
	}
	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN))
	{
		SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_LIBRARY);
	}

}
