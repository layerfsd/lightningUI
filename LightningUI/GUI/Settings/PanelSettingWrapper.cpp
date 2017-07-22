#include "PanelSettingWrapper.h"
#include "../../resource.h"

#ifndef DEBUG_TEST_MSG
#define DEBUG_TEST_MSG 1
#endif

CPanelSettingWrapper::CPanelSettingWrapper(void)
{
	m_idCurSubpage =PAGE_SETTINGS_GENERAL;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	int strMargin = 34;

	m_pGeneralItem = new CListBoxItemGL(GetResourceString(IDS_SETTING_GENERAL), NULL, NULL, strMargin, 0);
	m_pAudioItem  = new CListBoxItemGL(GetResourceString(IDS_SETTING_AUDIO), NULL, NULL, strMargin, 0);
	m_pDisplayItem = new CListBoxItemGL(GetResourceString(IDS_SETTING_DISPLAY), NULL, NULL, strMargin, 0);
	m_pBluetoothItem = new CListBoxItemGL(GetResourceString(IDS_SETTING_BLUETOOTH), NULL, NULL, strMargin, 0);
	m_pRadioItem = new CListBoxItemGL(GetResourceString(IDS_SETTING_RADIO), NULL, NULL, strMargin, 0);
	m_pAboutItem = new CListBoxItemGL(GetResourceString(IDS_SETTING_ABOUT), NULL, NULL, strMargin, 0);
#endif
}
CPanelSettingWrapper::~CPanelSettingWrapper(void)
{

}

BOOL CPanelSettingWrapper::IsReady()
{
	CPanelGL *panel=GetChildPanel(m_idCurSubpage);

	return CPanelGL::IsReady() && (panel?panel->IsReady():TRUE);
}

void CPanelSettingWrapper::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	if(idAni!=ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}


// 	MakeDefaultAni(ANIM_ROTATE_TOP_CLOCK_IN);
	if(iParam)
	{
		m_idCurSubpage =iParam;
		m_boxMenu.MoveToIndex(m_boxMenu.SearchID(m_idCurSubpage));
	}

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnEnterPanel();
	}
}

void CPanelSettingWrapper::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	if(idAni!=ANIM_NONE)
	{
		SetAlpha(1.0f);
		SetAlpha(0.0f,TRUE);
	}

	switch (m_idCurSubpage)
	{
	case PAGE_SETTINGS_TIME:
	case PAGE_SETTINGS_DATE:
	case PAGE_SETTINGS_TIMEZONE:
	case PAGE_SETTINGS_LANGUAGE:
	case PAGE_SETTINGS_BACKGROUND:
#if CVTE_EN_COMBINE_ANDROID_LINK
	case PAGE_SETTINGS_ANDROIDTYPE:
#endif
	//case PAGE_SETTINGS_ADVANCED:
		m_idCurSubpage = PAGE_SETTINGS_GENERAL;
		break;
	case PAGE_SETTINGS_RDS_PTY:
		m_idCurSubpage = PAGE_SETTINGS_RADIO;
		break;
    case PAGE_SETTINGS_FACTORY:
		//m_idCurSubpage = PAGE_SETTINGS_ABOUT;
		break;
    default:
        break;
    }

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnQuitPanel();
	}
}

void CPanelSettingWrapper::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);


	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchDown(pt);
	}
}
void CPanelSettingWrapper::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchUp(pt);
	}
}
void CPanelSettingWrapper::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchMove(pt);
	}
}
void CPanelSettingWrapper::OnGesture(GESTUREINFO *pgi)
{
	CPanelGL::OnGesture(pgi);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnGesture(pgi);
	}
}
void CPanelSettingWrapper::OnGestureMsg(UINT idGesture)
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
void CPanelSettingWrapper::Render()
{
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_229):NULL;
	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_604):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
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

#else
	DrawRect(pTexBack,0,0,g_iClientWidth-4,g_iClientHeight-4,2,&cr,TRUE);
#endif
	

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	m_boxMenu.SetAlpha(m_fCurAlpha);
	m_boxMenu.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_btnMenuCover.SetAlpha(m_fCurAlpha);
	m_btnMenuCover.Render();
#endif

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		SetFrameTime(panel->GetFrameTime());
		panel->SetAlpha(m_fCurAlpha);
		panel->Render();
	}

	EndRender();
}

BOOL CPanelSettingWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);


	//panels:
	m_panelGeneral.Initialize(PAGE_SETTINGS_GENERAL,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelGeneral.SetPos(LIST_BOX_ITEM_WIDTH/2,0);
	m_panelGeneral.LinkPages(&m_panelSettingBackground);

	m_panelAudio.Initialize(PAGE_SETTINGS_AUDIO,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelAudio.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
    m_panelDisplay.LinkLED(m_pLED);
#endif
	m_panelDisplay.Initialize(PAGE_SETTINGS_DISPLAY,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelDisplay.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelDate.Initialize(PAGE_SETTINGS_DATE,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelDate.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelTime.Initialize(PAGE_SETTINGS_TIME,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelTime.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelRadio.Initialize(PAGE_SETTINGS_RADIO,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelRadio.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelBluetooth.Initialize(PAGE_SETTINGS_BLUETOOTH,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelBluetooth.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

    m_panelAbout.LinkAppConfig(m_pAppConfig);
	m_panelAbout.Initialize(PAGE_SETTINGS_ABOUT,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelAbout.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingBackground.Initialize(PAGE_SETTINGS_BACKGROUND,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingBackground.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingLanguage.Initialize(PAGE_SETTINGS_LANGUAGE,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingLanguage.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingAdvanced.Initialize(PAGE_SETTINGS_ADVANCED,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingAdvanced.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingTimeZone.Initialize(PAGE_SETTINGS_TIMEZONE,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingTimeZone.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingRdsPTY.Initialize(PAGE_SETTINGS_RDS_PTY,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingRdsPTY.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingFactory.Initialize(PAGE_SETTINGS_FACTORY,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingFactory.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelPassword.Initialize(PAGE_SETTINGS_PASSWORD,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelPassword.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingRadioRegion.Initialize(PAGE_SETTINGS_RADIO_REGION,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingRadioRegion.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

#if CVTE_EN_CARPLAY
	m_panelSettingIperfServer.Initialize(PAGE_SETTINGS_IPERF_SERVER,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingIperfServer.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingIperfClient.Initialize(PAGE_SETTINGS_IPERF_CLIENT,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingIperfClient.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingIperfTCPClient.Initialize(PAGE_SETTINGS_IPERF_TCP_CLIENT,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingIperfTCPClient.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingIperfUDPClient.Initialize(PAGE_SETTINGS_IPERF_UDP_CLIENT,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingIperfUDPClient.SetPos(LIST_BOX_ITEM_WIDTH/2,0);

	m_panelSettingConnectTest.Initialize(PAGE_SETTINGS_CONNECT_TEST,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingConnectTest.SetPos(LIST_BOX_ITEM_WIDTH/2,0);
#endif

#if (CVTE_EN_GESTURE_AIR == 1)
	m_panelGesture.Initialize(PAGE_SETTINGS_GESTURE,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelGesture.SetPos(LIST_BOX_ITEM_WIDTH/2,0);
#endif

#if CVTE_EN_COMBINE_ANDROID_LINK
	m_panelSettingAndroidType.Initialize(PAGE_SETTINGS_ANDROIDTYPE,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelSettingAndroidType.SetPos(LIST_BOX_ITEM_WIDTH/2,0);
#endif
	//
	COLORGL cr_gray={180.0f/256.0f,180.0f/256.0f,180.0f/256.0f,1.0};
	COLORGL cr_red={245.0f/256.0f,59.0f/256.0f,16.0f/256.0f,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	const int cx=LIST_BOX_ITEM_WIDTH;
	const int cy=LIST_BOX_ITEM_HEIGHT;

	SIZE sibtn={cx,cy};


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

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

	m_iconGeneral.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1507));
	m_iconGeneralFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1508));

	m_iconAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1501));
	m_iconAudioFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1502));

	m_iconDisplay.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1505));
	m_iconDisplayFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1506));

	m_iconRadio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1509));
	m_iconRadioFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1510));
	
	m_iconBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1503));
	m_iconBT.SetShowScale(1.2f);
	m_iconBTFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1504));
	m_iconBTFocus.SetShowScale(1.2f);
	
	m_iconAbout.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1513));
	m_iconAboutFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1514));

	m_iconAdvance.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1513));
	m_iconAdvanceFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1514));

	LISTBOXSTRUCTURE box;
	memset(&box,0,sizeof(box));

	//find param:
	int count= 5;
	box.iSpacing=(g_iClientHeight-cy)/count/2*2;
	box.siClient.cx = cx;	
	box.siClient.cy = g_iClientHeight - cy;
	box.siBorder.cx = cx;
	box.siBorder.cy = g_iClientHeight - cy;
	box.idTexIndexBase=TEXID_289;
	box.idTexIndexFocus=TEXID_242;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_285;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxMenu.Initialize(IDC_SETTING_BOX_MAIN,this,&box,pSkinManager);
	m_boxMenu.SetNotifyFunc(&CPanelSettingWrapper::OnNotify,this);
	m_boxMenu.SetPos(-g_iClientWidth/2+cx/2, cy/2);
	m_boxMenu.LinkTextGL(pTextGL);
	m_boxMenu.Show(TRUE,FALSE);
	m_boxMenu.SetAlpha(1.0f);
	m_boxMenu.SetCaptioniHeight(18);
	m_boxMenu.SetCaptionColor(&cr_gray,&cr_w);

	m_btnMenuCover.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_297));
	m_btnMenuCover.SetSize(cx,box.iSpacing);
	m_btnMenuCover.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy+(box.iSpacing/2));

	int strMargin = 26;
	int iconMarginLeft = 18;
	int iconMarginRight = -14;

	m_iconArrowOn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_083));
	m_iconArrowOn.SetShowScale(0.8f);
	m_iconArrowOff.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_068));
	m_iconArrowOff.SetShowScale(0.8f);

	//add Left listbox item
	m_pGeneralItem->SetIcon(&m_iconGeneral,&m_iconGeneralFocus, iconMarginLeft, 0);
	m_pGeneralItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
	m_boxMenu.AddItem(m_pGeneralItem, PAGE_SETTINGS_GENERAL);

	m_pAudioItem->SetIcon(&m_iconAudio, &m_iconAudioFocus, iconMarginLeft, 0);
	m_pAudioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
	m_boxMenu.AddItem(m_pAudioItem, PAGE_SETTINGS_AUDIO);

	m_pDisplayItem->SetIcon(&m_iconDisplay, &m_iconDisplayFocus, iconMarginLeft, 0);
	m_pDisplayItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
	m_boxMenu.AddItem(m_pDisplayItem, PAGE_SETTINGS_DISPLAY);

	m_pBluetoothItem->SetIcon(&m_iconBT, &m_iconBTFocus, iconMarginLeft, 0);
	m_pBluetoothItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
	m_boxMenu.AddItem(m_pBluetoothItem, PAGE_SETTINGS_BLUETOOTH);

	m_pRadioItem->SetIcon(&m_iconRadio, &m_iconRadioFocus, iconMarginLeft, 0);
	m_pRadioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
	m_boxMenu.AddItem(m_pRadioItem, PAGE_SETTINGS_RADIO);

	m_pAboutItem->SetIcon(&m_iconAbout, &m_iconAboutFocus, iconMarginLeft, 0);
	m_pAboutItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
	m_boxMenu.AddItem(m_pAboutItem, PAGE_SETTINGS_ABOUT);

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_204));
	m_iconHomeUp.SetShowScale(1.2f);
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));
	m_iconHomeDown.SetShowScale(1.2f);

	m_btnHome.Initialize(IDC_SETTING_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_288),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelSettingWrapper::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.SetIconPos(-cx/2+35,0);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,68);
	m_btnHome.SetCaptionColor(&cr_w,&cr_gray);
	m_btnHome.SetCaptionHeight(20);

#else 

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_625));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_626));


	m_btnHome.Initialize(IDC_SETTING_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelSettingWrapper::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_b);
	m_btnHome.SetCaptionHeight(22);
	m_btnHome.SetIconPos(-cx/2+31,0);


	//
	m_iconGeneral.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_623));
    //m_iconGeneral.SetShowScale(0.8f);
	m_iconGeneralFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_624));
    //m_iconGeneralFocus.SetShowScale(0.8f);
	m_iconAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_665));
    //m_iconAudio.SetShowScale(0.8f);
	m_iconAudioFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_666));
    //m_iconAudioFocus.SetShowScale(0.8f);
	m_iconDisplay.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_627));
    //m_iconDisplay.SetShowScale(0.8f);
	m_iconDisplayFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_628));
    //m_iconDisplayFocus.SetShowScale(0.8f);
	m_iconRadio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_629));
    //m_iconRadio.SetShowScale(0.8f);
	m_iconRadioFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_630));
    //m_iconRadioFocus.SetShowScale(0.8f);
	m_iconBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_663));
    //m_iconBT.SetShowScale(0.8f);
	m_iconBTFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_664));
    //m_iconBTFocus.SetShowScale(0.8f);
	m_iconAbout.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_620));
    //m_iconAbout.SetShowScale(0.8f);
	m_iconAboutFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_621));
    //m_iconAboutFocus.SetShowScale(0.8f);
	m_iconAdvance.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_827));
    //m_iconAdvance.SetShowScale(0.8f);
	m_iconAdvanceFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_828));
    //m_iconAdvanceFocus.SetShowScale(0.8f);

	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));
	box.siClient.cx=cx;
	box.siClient.cy=g_iClientHeight-cy;
	box.siBorder.cx=cx;
	box.siBorder.cy=g_iClientHeight-cy;
	box.iSpacing=(g_iClientHeight-cy)/5;

	box.idTexIndexBase=TEXID_603;
	box.idTexIndexFocus=TEXID_068;//TEXID_604;
	box.idTexBase=TEXID_603;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;//FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxMenu.Initialize(IDC_SETTING_BOX_MAIN,this,&box,pSkinManager);
	m_boxMenu.SetNotifyFunc(&CPanelSettingWrapper::OnNotify,this);
	m_boxMenu.SetPos(-g_iClientWidth/2+cx/2,cy/2);
	m_boxMenu.LinkTextGL(pTextGL);
	m_boxMenu.SetAlpha(1.0f);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
		//case LANGUAGE_ITALIAN:
		case LANGUAGE_RUSSIAN:
	        m_boxMenu.SetCaptioniHeight(19);
			break;
		case LANGUAGE_PERSIAN:
			m_boxMenu.SetCaptioniHeight(20);
			break;
		default:
			m_boxMenu.SetCaptioniHeight(22);
    }

	m_boxMenu.SetCaptionColor(&cr_gray,&cr_b);

	CListBoxItemGL *pItem;
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_GENERAL));
	pItem->SetIcon(&m_iconGeneral,&m_iconGeneralFocus);
	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_GENERAL);
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_AUDIO));
	pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus);
	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_AUDIO);
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_DISPLAY));
	pItem->SetIcon(&m_iconDisplay,&m_iconDisplayFocus);
	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_DISPLAY);
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_BLUETOOTH));
	pItem->SetIcon(&m_iconBT,&m_iconBTFocus);
	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_BLUETOOTH);
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_RADIO));
	pItem->SetIcon(&m_iconRadio,&m_iconRadioFocus);
	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_RADIO);
//	    if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_KOREAN)
//	    {
//	        pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_STEER_WHEEL));
//	    	pItem->SetIcon(&m_iconAdvance,&m_iconAdvanceFocus);
//	    	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_ADVANCED);
//	    }
//	    else
//	    {
//	        pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_ADVANCED));
//	    	pItem->SetIcon(&m_iconAdvance,&m_iconAdvanceFocus);
//	    	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_ADVANCED);
//	    }
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
		case LANGUAGE_ENGLISH:
			{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				pItem=new CListBoxItemGL(_T("SWC"));
#else
				pItem=new CListBoxItemGL(_T("Senior"));
#endif
			}
			break;
		case LANGUAGE_GERMAN:
			pItem=new CListBoxItemGL(_T("Erweiterte"));
			break;
		case LANGUAGE_FRANCH:
			pItem=new CListBoxItemGL(_T("Avanc¨¦"));
			break;
		case LANGUAGE_SPANISH:
			pItem=new CListBoxItemGL(_T("Avanzada"));
			break;
		case LANGUAGE_ITALIAN:
			pItem=new CListBoxItemGL(_T("Avanzato"));
			break;
		case LANGUAGE_RUSSIAN:
			pItem=new CListBoxItemGL(_T("§³§ä§Ñ§â§ê§Ú§Û"));
			break;
		case LANGUAGE_KOREAN:
		case LANGUAGE_PERSIAN:
			pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_STEER_WHEEL));
			break;
		default:
			pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_ADVANCED));
			break;
	}

	pItem->SetIcon(&m_iconAdvance,&m_iconAdvanceFocus);
	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_ADVANCED);
#endif

    pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_ABOUT));
	pItem->SetIcon(&m_iconAbout,&m_iconAboutFocus);
	m_boxMenu.AddItem(pItem,PAGE_SETTINGS_ABOUT);

#endif	//CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	return TRUE;
}


void CALLBACK CPanelSettingWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingWrapper::OnNotify]: id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelSettingWrapper *panel=(CPanelSettingWrapper*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: idControl[%d], wMsg[%d], wPara[%d]\r\n"),TEXT(__FUNCTION__),idControl, wMsg, wPara));
	switch (idControl)
	{
	case IDC_SETTING_BTN_HOME:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BOX_MAIN:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_CONTROL:
				{
					m_boxMenu.SetFocus(index);
				}
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)				
					ArrowChange(index); 
#endif
					m_idCurSubpage=(PAGE_SETTINGS)id_item;
					SetPanelParam(m_idCurSubpage);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	}
}

void CPanelSettingWrapper::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	m_panelGeneral.OnMCUCmd(wParam,lParam);
	m_panelAudio.OnMCUCmd(wParam,lParam);
	m_panelRadio.OnMCUCmd(wParam,lParam);
	m_panelAbout.OnMCUCmd(wParam,lParam);
	m_panelDisplay.OnMCUCmd(wParam,lParam);
    m_panelSettingAdvanced.OnMCUCmd(wParam,lParam);
}
void CPanelSettingWrapper::OnDeviceChanged()
{
	m_panelAbout.OnDeviceChanged();
	m_panelUpdateARM.OnDeviceChanged();
}
#if CVTE_EN_CPMAC
void CPanelSettingWrapper::OnMACChanged()
{
	m_panelAbout.OnMACChanged();
}
#endif
void CPanelSettingWrapper::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	m_panelBluetooth.OnCarPlayMsg(wParam,lParam);
#if CVTE_EN_CARPLAY
	m_panelSettingIperfServer.OnCarPlayMsg(wParam,lParam);
	m_panelSettingIperfClient.OnCarPlayMsg(wParam,lParam);
	m_panelSettingIperfTCPClient.OnCarPlayMsg(wParam,lParam);
	m_panelSettingIperfUDPClient.OnCarPlayMsg(wParam,lParam);
#endif
}

#if (CVTE_EN_GESTURE_AIR == 1)
void CPanelSettingWrapper::OnGestureCircle(BOOL bCircleRight)
{
	m_panelGesture.OnGestureCircle(bCircleRight);
}

void CPanelSettingWrapper::OnGestureID(WPARAM wParam, LPARAM lParam)
{
	m_panelGesture.OnGestureID(wParam, lParam);
}
#endif

void CPanelSettingWrapper::OnPhoneHFState(WPARAM wParam, LPARAM lParam)
{
	m_panelBluetooth.OnPhoneHFState(wParam,lParam);
}
void CPanelSettingWrapper::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_panelBluetooth.OnPhoneCallState(wParam,lParam);
}
void CPanelSettingWrapper::OnBluetoothSetupStatus(WPARAM wParam, LPARAM lParam)
{
	m_panelBluetooth.OnBluetoothSetupStatus(wParam,lParam);
}

void CPanelSettingWrapper::OnCameraTypeChange(UINT uCameraType)
{
	m_panelSettingAdvanced.OnCameraTypeChange(uCameraType);
}

void CPanelSettingWrapper::OnRearColorSignalChange(UINT uColorSignal)
{
	m_panelSettingFactory.OnRearColorSignalChange(uColorSignal);
}

void CPanelSettingWrapper::OnBackgroundChanged(WPARAM wParam, LPARAM lParam)
{
	m_panelGeneral.OnBackgroundChanged(wParam,lParam);
}

void CPanelSettingWrapper::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	m_panelGeneral.OnDeviceArrivedReady(lpDeviceName);
}
void CPanelSettingWrapper::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_SETTING, (TEXT("APP:MSG:GUI:SETTINGS: [CPanelSettingWrapper::OnDeviceRemovedReady].\r\n")));
	m_panelGeneral.OnDeviceRemovedReady(lpDeviceName);
}

void CPanelSettingWrapper::OnVolumeConfigMsg(WPARAM wParam, LPARAM lParam)
{
	m_panelAudio.OnVolumeConfigMsg(wParam,lParam);
}

void CPanelSettingWrapper::OnPanelLightChange()
{
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
    m_panelDisplay.FreshLedSilder();
#endif
}

void CPanelSettingWrapper::OnHeadLightOn(BOOL bOn, UINT nBk)
{
	m_panelDisplay.OnHeadlightOn(bOn, nBk);
}

void CPanelSettingWrapper::OnLCDBrightnessChange()
{
    m_panelDisplay.FreshLCDBrightnessSilder();
}

void CPanelSettingWrapper::OnSetPasswordBox(NBMSG *pMsg)
{
	m_panelPassword.SetNotification(pMsg);
}

void CPanelSettingWrapper::OnPasswordBoxCmd(WPARAM wParam, LPARAM lParam)
{
	m_panelPassword.SetCmd(wParam,lParam);
}

void CPanelSettingWrapper::OnDabServiceStartReady(WPARAM wParam, LPARAM lParam)
{
	m_panelDAB.OnDabServiceStartReady(wParam,lParam);

}

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
void CPanelSettingWrapper::LinkAudioCodec(CAudioCodec *pAudioCodec)
{
	//m_panelGeneral.LinkAudioCodec(pAudioCodec);
	m_panelAudio.LinkAudioCodec(pAudioCodec);
}
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
void CPanelSettingWrapper::ArrowChange(UINT index)
{
	int iconMarginRight = -14;

	switch(index)
	{
	case GENERAL_SHOW_ARROW:
		{	
			m_pGeneralItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
			m_pAudioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pDisplayItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pBluetoothItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pRadioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAboutItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
		}
		break;
	case AUDIO_SHOW_ARROW:
		{
			m_pGeneralItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAudioItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
			m_pDisplayItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pBluetoothItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pRadioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAboutItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
		}
		break;
	case DISPLAY_SHOW_ARROW:
		{
			m_pGeneralItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAudioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pDisplayItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
			m_pBluetoothItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pRadioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAboutItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
		}
		break;
	case BLUETOOTH_SHOW_ARROW:
		{
			m_pGeneralItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAudioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pDisplayItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pBluetoothItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
			m_pRadioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAboutItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
		}
		break;
	case RADIO_SHOW_ARROW:
		{
			m_pGeneralItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAudioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pDisplayItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pBluetoothItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pRadioItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
			m_pAboutItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
		}
		break;
	case ABOUT_SHOW_ARROW:
		{
			m_pGeneralItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAudioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pDisplayItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pBluetoothItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pRadioItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
			m_pAboutItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
		}
		break;
	default:
		break;
	}
}
#endif