#include "PanelSettingBluetooth.h"
#include "../../resource.h"

extern BOOL	g_bBTModuleLoaded;

CPanelSettingBluetooth::CPanelSettingBluetooth(void)
{
	m_iBTPowerState = BT_POWER_OFF;
	m_hfConnectStatus = CONNECTION_STATUS_IDLE;
	m_bCarPlayConnected = FALSE;
	m_bShowDetails = FALSE;
	m_bOperatePowerOn = FALSE;
	m_bAutoSwitchDeviceMgr = FALSE;
	m_bPhoneCallActive = FALSE;
}
CPanelSettingBluetooth::~CPanelSettingBluetooth(void)
{

}

BOOL CPanelSettingBluetooth::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingBluetooth::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingBluetooth::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingBluetooth::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif
	//power:
	m_tagTitlePower.SetAlpha(m_fCurAlpha);
	m_tagTitlePower.Render();
	m_sliPower.SetAlpha(m_fCurAlpha);
	m_sliPower.Render();

// 	//connect:
// 	m_btnConnect.SetAlpha(m_fCurAlpha);
// 	m_btnConnect.Render();
// 	m_tagValueConnect.SetAlpha(m_fCurAlpha);
// 	m_tagValueConnect.Render();

	//name:
	m_btnDeviceName.SetAlpha(m_fCurAlpha);
	m_btnDeviceName.Render();
	m_tagValueDeviceName.SetAlpha(m_fCurAlpha);
	m_tagValueDeviceName.Render();

	/*
	//pin:
	m_btnPairPin.SetAlpha(m_fCurAlpha);
	m_btnPairPin.Render();
	m_tagValuePairPin.SetAlpha(m_fCurAlpha);
	m_tagValuePairPin.Render();
	*/

	//auto connect:
	m_tagTitleAutoConnect.SetAlpha(m_fCurAlpha);
	m_tagTitleAutoConnect.Render();
	m_sliAutoConnect.SetAlpha(m_fCurAlpha);
	m_sliAutoConnect.Render();

	//auto answer:
	m_tagTitleAutoAnswer.SetAlpha(m_fCurAlpha);
	m_tagTitleAutoAnswer.Render();
	m_sliAutoAnswer.SetAlpha(m_fCurAlpha);
	m_sliAutoAnswer.Render();

	//auto download:
	m_tagTitleAutoDownloadPB.SetAlpha(m_fCurAlpha);
	m_tagTitleAutoDownloadPB.Render();
	m_sliAutoDownloadPB.SetAlpha(m_fCurAlpha);
	m_sliAutoDownloadPB.Render();

#if CVTE_EN_BT_VCARD_NAME_ORDER
	m_tagTitleVcardNameOrder.SetAlpha(m_fCurAlpha);
	m_tagTitleVcardNameOrder.Render();
	m_sliVcardNameOrder.SetAlpha(m_fCurAlpha);
	m_sliVcardNameOrder.Render();
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagBTNote.SetAlpha(m_fCurAlpha);
	m_tagBTNote.Render();
#endif

/*
	//ringtone:
	m_tagTitleRingtone.SetAlpha(m_fCurAlpha);
	m_tagTitleRingtone.Render();
	m_sliRingtone.SetAlpha(m_fCurAlpha);
	m_sliRingtone.Render();
*/

	//version:
	m_btnBluetoothVersion.SetAlpha(m_fCurAlpha);
	m_btnBluetoothVersion.Render();
	m_tagValueBluetoothVersion.SetAlpha(m_fCurAlpha);
	m_tagValueBluetoothVersion.Render();

	if(m_bOperatePowerOn && !m_bShowDetails)
	{
		SetFrameTime(30);

		VIEW_STATE vs=m_iconSearching.GetCurViewState();
		vs.fRotateZ-=5;
		if(vs.fRotateZ<-360)
			vs.fRotateZ=0;
		m_iconSearching.SetCurViewState(&vs);
		m_iconSearching.SetAlpha(m_fCurAlpha);
		m_iconSearching.Render();
	}
	else
	{
		SetFrameTime(500);
	}

	EndRender();
}

BOOL CPanelSettingBluetooth::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

    const int box_size_x=LIST_BOX_ITEM_WIDTH;

	//large size:
	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};


	int margin_x=12;
	int margin_y = 6;
	int cx = g_iScreenWidth-LIST_BOX_ITEM_WIDTH;
	int caption_h = 22;
	int caption2_h = 22;
	int value_Font = 20;
	int gridHeight = 80;
	int margin_left = 40;
	int space = 80;
	int tag_h = space;
	int captionMargin = 22;

	SLIDERSTRUCTURE sli;
	GLfloat py=g_iClientHeight/2-40;

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(cx, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);

	//power:
	m_tagTitlePower.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitlePower.SetSize(cx, tag_h);
	m_tagTitlePower.SetCaption(GetResourceString(IDS_SETTING_BT_POWER));
	m_tagTitlePower.LinkTextGL(pTextGL);
	m_tagTitlePower.SetPos(0,py);
	m_tagTitlePower.SetCaptionHeight(caption_h);
	m_tagTitlePower.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitlePower.SetCaptionColor(&cr_gray, &cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_BOOL_BUTTON_WIDTH;
	sli.size.cy=SLI_BOOL_BUTTON_HEIGHT;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_642;
	sli.idChannelTex=TEXID_641;
	sli.idThumbTex=TEXID_640;
	sli.bSwitchMode=TRUE;

	m_sliPower.Initialize(IDC_BT_SLI_POWER,this,&sli,pSkinManager);
	m_sliPower.SetRange(0,10);
	m_sliPower.SetValue(0);
	m_sliPower.SetPos(m_siCurSize.cx/2-70,py);
	m_sliPower.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	m_iBTPowerState=m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn?BT_POWER_ON:BT_POWER_OFF;


	m_iconSearching.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconSearching.SetShowScale(1.2f);
	m_iconSearching.SetPos(0,-40);

	//
	py-=tag_h;

	//auto connect:
	m_tagTitleAutoConnect.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleAutoConnect.SetSize(cx, tag_h);
	m_tagTitleAutoConnect.SetCaption(GetResourceString(IDS_SETTING_BT_AUTO_CONNECT));
	m_tagTitleAutoConnect.LinkTextGL(pTextGL);
	m_tagTitleAutoConnect.SetPos(0,py);
	m_tagTitleAutoConnect.SetCaptionHeight(caption_h);
	m_tagTitleAutoConnect.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleAutoConnect.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliAutoConnect.Initialize(IDC_BT_SLI_AUTO_CONNECT,this,&sli,pSkinManager);
	m_sliAutoConnect.SetRange(0,10);
	m_sliAutoConnect.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothAutoConnect?10:0);
	m_sliAutoConnect.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAutoConnect.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	py-=tag_h;

	//auto answer:
	m_tagTitleAutoAnswer.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleAutoAnswer.SetSize(cx, tag_h);
	m_tagTitleAutoAnswer.SetCaption(GetResourceString(IDS_SETTING_BT_AUTO_ANSWER));
	m_tagTitleAutoAnswer.LinkTextGL(pTextGL);
	m_tagTitleAutoAnswer.SetPos(0,py);
	m_tagTitleAutoAnswer.SetCaptionHeight(caption_h);
	m_tagTitleAutoAnswer.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleAutoAnswer.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliAutoAnswer.Initialize(IDC_BT_SLI_AUTO_ANSWER,this,&sli,pSkinManager);
	m_sliAutoAnswer.SetRange(0,10);
	m_sliAutoAnswer.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bCallAutoAnswer?10:0);
	m_sliAutoAnswer.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAutoAnswer.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	py-=tag_h;

	//auto download:
	m_tagTitleAutoDownloadPB.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleAutoDownloadPB.SetSize(cx, tag_h);
	m_tagTitleAutoDownloadPB.SetCaption(GetResourceString(IDS_SETTING_BT_AUTO_DOWNLOAD_PB));
	m_tagTitleAutoDownloadPB.LinkTextGL(pTextGL);
	m_tagTitleAutoDownloadPB.SetPos(0,py);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
		//case LANGUAGE_ITALIAN:
	case LANGUAGE_FRANCH:
		m_tagTitleAutoDownloadPB.SetCaptionHeight(caption_h-4);
		break;
	case LANGUAGE_GREEK:
		m_tagTitleAutoDownloadPB.SetCaptionHeight(caption2_h);
		break;
	default:
		m_tagTitleAutoDownloadPB.SetCaptionHeight(caption_h);
	}
	m_tagTitleAutoDownloadPB.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleAutoDownloadPB.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliAutoDownloadPB.Initialize(IDC_BT_SLI_AUTO_DOWNLOAD,this,&sli,pSkinManager);
	m_sliAutoDownloadPB.SetRange(0,10);
	m_sliAutoDownloadPB.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bContactAutoDownload?10:0);
	m_sliAutoDownloadPB.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAutoDownloadPB.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	py-=tag_h;

	//device name
	m_btnDeviceName.Initialize(IDC_BT_BTN_SET_DEVICE_NAME,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDeviceName.SetSize(cx, tag_h);
	m_btnDeviceName.SetCaption(GetResourceString(IDS_SETTING_BT_DEVICE_NAME));
	m_btnDeviceName.LinkTextGL(pTextGL);
	m_btnDeviceName.SetPos(0,py);
	m_btnDeviceName.SetCaptionHeight(caption_h);
	m_btnDeviceName.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnDeviceName.SetCaptionColor(&cr_gray, & cr_gray);
	m_btnDeviceName.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	m_tagValueDeviceName.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueDeviceName.SetSize(cx,gridHeight);
	m_tagValueDeviceName.SetCaption(_T(""));
	m_tagValueDeviceName.LinkTextGL(pTextGL);
	m_tagValueDeviceName.SetPos(0, py);
	m_tagValueDeviceName.SetCaptionHeight(value_Font);
	m_tagValueDeviceName.EnableDrawBack(FALSE);
	m_tagValueDeviceName.SetCaptionPos(BTN_CAPTION_RIGHT, -20);
	m_tagValueDeviceName.SetCaptionColor(&cr_gray);

	//
	py-=tag_h;

	//bluetooth version
	m_btnBluetoothVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBluetoothVersion.SetSize(cx, tag_h);
	m_btnBluetoothVersion.SetCaption(GetResourceString(IDS_SETTING_SOFTWARE_VERSION));
	m_btnBluetoothVersion.LinkTextGL(pTextGL);
	m_btnBluetoothVersion.SetPos(0,py);
	m_btnBluetoothVersion.SetCaptionHeight(caption_h);
	m_btnBluetoothVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnBluetoothVersion.SetCaptionColor(&cr_gray, &cr_gray);

	m_tagValueBluetoothVersion.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueBluetoothVersion.SetSize(cx, gridHeight);
	m_tagValueBluetoothVersion.SetCaption(_T(""));
	m_tagValueBluetoothVersion.LinkTextGL(pTextGL);
	m_tagValueBluetoothVersion.SetPos(0, py);
	m_tagValueBluetoothVersion.SetCaptionHeight(value_Font);
	m_tagValueBluetoothVersion.EnableDrawBack(FALSE);
	m_tagValueBluetoothVersion.SetCaptionPos(BTN_CAPTION_RIGHT, -20);
	m_tagValueBluetoothVersion.SetCaptionColor(&cr_gray);

#else

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};

	SLIDERSTRUCTURE sli;

	int margin_x=12;
	int cx=(g_iClientWidth-box_size_x-margin_x*2)/2;
	//int cy=cx;
	int tag_h = LIST_BOX_TAG_HEIGHT;
	int caption2_h = 20;
	int caption_h = 22;
	int caption_little = 18;

	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN /*||
																				  m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_SPANISH*/)
	{
		caption_h = 20;
	}

	GLfloat py=g_iClientHeight/2-50;

	//power:
	m_tagTitlePower.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitlePower.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagTitlePower.SetCaption(GetResourceString(IDS_TITLE_BLUETOOTH));
#else
	m_tagTitlePower.SetCaption(GetResourceString(IDS_SETTING_BT_POWER));
#endif
	m_tagTitlePower.LinkTextGL(pTextGL);
	m_tagTitlePower.SetPos(0,py);
	m_tagTitlePower.SetCaptionHeight(caption_h);
	m_tagTitlePower.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitlePower.SetCaptionColor(&cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_BOOL_BUTTON_WIDTH;
	sli.size.cy=SLI_BOOL_BUTTON_HEIGHT;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_642;
	sli.idChannelTex=TEXID_641;
	sli.idThumbTex=TEXID_640;
	sli.bSwitchMode=TRUE;

	m_sliPower.Initialize(IDC_BT_SLI_POWER,this,&sli,pSkinManager);
	m_sliPower.SetRange(0,10);
	m_sliPower.SetValue(0/*m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn?10:0*/);
	m_sliPower.SetPos(m_siCurSize.cx/2-70,py);
	m_sliPower.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	m_iBTPowerState=m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn?BT_POWER_ON:BT_POWER_OFF;


	m_iconSearching.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconSearching.SetShowScale(1.2f);
	m_iconSearching.SetPos(0,-40);

	/*
	//connect setting:
	py-=83;

	m_btnConnect.Initialize(IDC_BT_BTN_CONNECT_SETTING,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
	m_pSkinManager->GetTexture(TEXID_607),NULL,
	NULL,NULL,
	NULL,NULL);
	m_btnConnect.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnConnect.SetCaption(GetResourceString(IDS_BT_CONNECT_SETTING));
	m_btnConnect.LinkTextGL(pTextGL);
	m_btnConnect.SetPos(0,py);
	m_btnConnect.SetCaptionHeight(caption_h);
	m_btnConnect.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnConnect.SetCaptionColor(&cr_gray);
	m_btnConnect.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	m_tagValueConnect.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueConnect.SetSize(cx,40);
	m_tagValueConnect.SetCaption(GetResourceString(IDS_NOT_CONNECTED));
	m_tagValueConnect.LinkTextGL(pTextGL);
	m_tagValueConnect.SetPos(cx/2+10 -20,py);
	m_tagValueConnect.SetCaptionHeight(caption_h);
	m_tagValueConnect.EnableDrawBack(FALSE);
	m_tagValueConnect.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueConnect.SetCaptionColor(&cr_gray);
	*/

	//
	py-=tag_h;

	/*
	//pair pin
	m_btnPairPin.Initialize(IDC_BT_BTN_SET_PAIR_PIN,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
	m_pSkinManager->GetTexture(TEXID_607),NULL,
	NULL,NULL,
	NULL,NULL);
	m_btnPairPin.SetSize((g_iClientWidth-box_size_x-margin_x*2),80);
	m_btnPairPin.SetCaption(GetResourceString(IDS_SETTING_BT_PAIR_PIN));
	m_btnPairPin.LinkTextGL(pTextGL);
	m_btnPairPin.SetPos(0,py);
	m_btnPairPin.SetCaptionHeight(caption_h);
	m_btnPairPin.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnPairPin.SetCaptionColor(&cr_gray);
	m_btnPairPin.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	m_tagValuePairPin.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValuePairPin.SetSize(cx,40);
	m_tagValuePairPin.SetCaption(szPIN);
	m_tagValuePairPin.LinkTextGL(pTextGL);
	m_tagValuePairPin.SetPos(cx/2+10 -20,py);
	m_tagValuePairPin.SetCaptionHeight(caption_h);
	m_tagValuePairPin.EnableDrawBack(FALSE);
	m_tagValuePairPin.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValuePairPin.SetCaptionColor(&cr_gray);

	py-=83;
	*/

	//auto connect:
	m_tagTitleAutoConnect.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAutoConnect.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleAutoConnect.SetCaption(GetResourceString(IDS_SETTING_BT_AUTO_CONNECT));
	m_tagTitleAutoConnect.LinkTextGL(pTextGL);
	m_tagTitleAutoConnect.SetPos(0,py);
	m_tagTitleAutoConnect.SetCaptionHeight(caption_h);
	m_tagTitleAutoConnect.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleAutoConnect.SetCaptionColor(&cr_gray);

	m_sliAutoConnect.Initialize(IDC_BT_SLI_AUTO_CONNECT,this,&sli,pSkinManager);
	m_sliAutoConnect.SetRange(0,10);
	m_sliAutoConnect.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothAutoConnect?10:0);
	m_sliAutoConnect.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAutoConnect.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	py-=tag_h;

	//auto answer:
	m_tagTitleAutoAnswer.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAutoAnswer.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleAutoAnswer.SetCaption(GetResourceString(IDS_SETTING_BT_AUTO_ANSWER));
	m_tagTitleAutoAnswer.LinkTextGL(pTextGL);
	m_tagTitleAutoAnswer.SetPos(0,py);
	m_tagTitleAutoAnswer.SetCaptionHeight(caption_h);
	m_tagTitleAutoAnswer.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleAutoAnswer.SetCaptionColor(&cr_gray);

	m_sliAutoAnswer.Initialize(IDC_BT_SLI_AUTO_ANSWER,this,&sli,pSkinManager);
	m_sliAutoAnswer.SetRange(0,10);
	m_sliAutoAnswer.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bCallAutoAnswer?10:0);
	m_sliAutoAnswer.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAutoAnswer.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	py-=tag_h;

	//auto download:
	m_tagTitleAutoDownloadPB.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAutoDownloadPB.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleAutoDownloadPB.SetCaption(GetResourceString(IDS_SETTING_BT_AUTO_DOWNLOAD_PB));
	m_tagTitleAutoDownloadPB.LinkTextGL(pTextGL);
	m_tagTitleAutoDownloadPB.SetPos(0,py);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
		//case LANGUAGE_ITALIAN:
	case LANGUAGE_FRANCH:
		m_tagTitleAutoDownloadPB.SetCaptionHeight(caption_h-4);
		break;
	case LANGUAGE_GREEK:
		m_tagTitleAutoDownloadPB.SetCaptionHeight(caption2_h);
		break;
	default:
		m_tagTitleAutoDownloadPB.SetCaptionHeight(caption_h);
	}
	m_tagTitleAutoDownloadPB.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleAutoDownloadPB.SetCaptionColor(&cr_gray);

	m_sliAutoDownloadPB.Initialize(IDC_BT_SLI_AUTO_DOWNLOAD,this,&sli,pSkinManager);
	m_sliAutoDownloadPB.SetRange(0,10);
	m_sliAutoDownloadPB.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bContactAutoDownload?10:0);
	m_sliAutoDownloadPB.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAutoDownloadPB.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

#if CVTE_EN_BT_VCARD_NAME_ORDER
	py-=tag_h;

	//First name first:
	m_tagTitleVcardNameOrder.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVcardNameOrder.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVcardNameOrder.SetCaption(GetResourceString(IDS_BT_PHONEBOOK_FIRST_NAME_FIRST));
	m_tagTitleVcardNameOrder.LinkTextGL(pTextGL);
	m_tagTitleVcardNameOrder.SetPos(0,py);
	m_tagTitleVcardNameOrder.SetCaptionHeight(caption_h);
	m_tagTitleVcardNameOrder.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleVcardNameOrder.SetCaptionColor(&cr_gray);

	m_sliVcardNameOrder.Initialize(IDC_BT_SLI_VCARD_NAME_ORDER,this,&sli,pSkinManager);
	m_sliVcardNameOrder.SetRange(0,10);
	m_sliVcardNameOrder.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bVcardFirstNameFirst?10:0);
	m_sliVcardNameOrder.SetPos(m_siCurSize.cx/2-70,py);
	m_sliVcardNameOrder.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	m_tagBTNote.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBTNote.SetSize(cx,40);
	m_tagBTNote.SetCaption(_T("Turn OFF CarPlay to Enable Bluetooth"));
	m_tagBTNote.LinkTextGL(pTextGL);
	m_tagBTNote.SetPos(0,0);
	m_tagBTNote.SetCaptionHeight(24);
	m_tagBTNote.EnableDrawBack(FALSE);
	m_tagBTNote.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagBTNote.SetCaptionColor(&cr_dark);
	m_tagBTNote.Show(FALSE);

#endif
	/*
	py-=85;

	//ringtone:
	m_tagTitleRingtone.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRingtone.SetSize((g_iClientWidth-box_size_x-margin_x*2),80);
	m_tagTitleRingtone.SetCaption(GetResourceString(IDS_SETTING_BT_RINGTONE));
	m_tagTitleRingtone.LinkTextGL(pTextGL);
	m_tagTitleRingtone.SetPos(0,py);
	m_tagTitleRingtone.SetCaptionHeight(caption_h);
	m_tagTitleRingtone.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleRingtone.SetCaptionColor(&cr_gray);

	m_sliRingtone.Initialize(IDC_BT_SLI_RINGTONE,this,&sli,pSkinManager);
	m_sliRingtone.SetRange(0,10);
	m_sliRingtone.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bIncomingRing?10:0);
	m_sliRingtone.SetPos(m_siCurSize.cx/2-70,py);
	m_sliRingtone.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);
	*/

	//
	py-=tag_h;

	//device name
	m_btnDeviceName.Initialize(IDC_BT_BTN_SET_DEVICE_NAME,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDeviceName.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnDeviceName.SetCaption(GetResourceString(IDS_SETTING_BT_DEVICE_NAME));
	m_btnDeviceName.LinkTextGL(pTextGL);
	m_btnDeviceName.SetPos(0,py);
	m_btnDeviceName.SetCaptionHeight(caption_h);
	m_btnDeviceName.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnDeviceName.SetCaptionColor(&cr_gray);
	m_btnDeviceName.SetNotifyFunc(&CPanelSettingBluetooth::OnNotify,this);

	m_tagValueDeviceName.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueDeviceName.SetSize(cx,40);
	m_tagValueDeviceName.SetCaption(_T(""));
	m_tagValueDeviceName.LinkTextGL(pTextGL);
	m_tagValueDeviceName.SetPos(cx/2+10 -20,py);
	m_tagValueDeviceName.SetCaptionHeight(caption_little);
	m_tagValueDeviceName.EnableDrawBack(FALSE);
	m_tagValueDeviceName.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueDeviceName.SetCaptionColor(&cr_gray);

	//
	py-=tag_h;

	//bluetooth version
	m_btnBluetoothVersion.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBluetoothVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnBluetoothVersion.SetCaption(GetResourceString(IDS_SETTING_SOFTWARE_VERSION));
	m_btnBluetoothVersion.LinkTextGL(pTextGL);
	m_btnBluetoothVersion.SetPos(0,py);
	m_btnBluetoothVersion.SetCaptionHeight(caption_h);
	m_btnBluetoothVersion.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnBluetoothVersion.SetCaptionColor(&cr_gray);

	m_tagValueBluetoothVersion.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueBluetoothVersion.SetSize(cx,40);
	m_tagValueBluetoothVersion.SetCaption(_T(""));
	m_tagValueBluetoothVersion.LinkTextGL(pTextGL);
	m_tagValueBluetoothVersion.SetPos(cx/2+10 -20,py);
	m_tagValueBluetoothVersion.SetCaptionHeight(caption_little);
	m_tagValueBluetoothVersion.EnableDrawBack(FALSE);
	m_tagValueBluetoothVersion.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueBluetoothVersion.SetCaptionColor(&cr_gray);

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	//
	RefreshSettingItem(FALSE/*m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn*/);

	//auto calculate size:
	SetSize(g_iClientWidth-box_size_x,max(g_iClientHeight,g_iClientHeight/2+50-py));

	return TRUE;
}


void CALLBACK CPanelSettingBluetooth::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingBluetooth *panel=(CPanelSettingBluetooth*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingBluetooth::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MS:CPanelSettingBluetooth::HandleControlMSG: idControl[%d] wPara[%d]\r\n"),idControl,wPara));
	switch (idControl)
	{
	case IDC_BT_SLI_POWER:
		{
			BOOL bOn=wPara?1:0;
			if(m_iBTPowerState != bOn)
			{
				m_bOperatePowerOn = bOn;

				SendBluetoothCommand(BT_CMD_POWER,bOn);
				if(!bOn)
				{
					RefreshSettingItem(FALSE);
				}
				else
				{
					m_bAutoSwitchDeviceMgr = TRUE;
				}

				m_sliPower.Enable(FALSE);

				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_BT_SLI_AUTO_CONNECT:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothAutoConnect = (wPara?1:0);
			SendBluetoothCommand(BT_CMD_SET_AUTO_CONNECT,(wPara?1:0));

			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_SLI_AUTO_ANSWER:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bCallAutoAnswer = (wPara?1:0);
			SendBluetoothCommand(BT_CMD_SET_AUTO_ANSWER,(wPara?1:0));

			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_SLI_AUTO_DOWNLOAD:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bContactAutoDownload = (wPara?1:0);
			SendBluetoothCommand(BT_CMD_SET_AUTO_SYNCPB,(wPara?1:0));

			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_SLI_VCARD_NAME_ORDER:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bVcardFirstNameFirst = (wPara?1:0);
			SendBluetoothCommand(BT_CMD_SET_VCARD_NAME_ORDER,(wPara?1:0));
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_SLI_RINGTONE:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bIncomingRing = (wPara?1:0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_CONNECT_SETTING:
		{
			SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN,PAGE_SWITCH_ANI_DEFAULT,BT_HOMEPAGE_DEVICE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_SET_DEVICE_NAME:
		{
			ShowKeyboard(IDC_PANEL_KEYBOARD+IDC_BT_BTN_SET_DEVICE_NAME,&CPanelSettingBluetooth::OnNotify,this,INPUT_LETTERS_CAP,
				GetResourceString(IDS_INPUT_NEW_DEVICE_NAME),
				m_tagValueDeviceName.GetCaption(),14);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_SET_PAIR_PIN:
		{
			ShowKeyboard(IDC_PANEL_KEYBOARD+IDC_BT_BTN_SET_PAIR_PIN,&CPanelSettingBluetooth::OnNotify,this,INPUT_LETTERS_CAP,
				GetResourceString(IDS_INPUT_NEW_PAIR_PIN),
				NULL,8);
			PlaySoundEffect(0);
		}
		break;
	case IDC_PANEL_KEYBOARD+IDC_BT_BTN_SET_DEVICE_NAME:
		{
			if(lpPara)
			{
				RETAILMSG(DEBUG_SETTING,(_T("CPanelSettingBluetooth IDC_PANEL_KEYBOARD IDC_BT_BTN_SET_DEVICE_NAME:%s\r\n"),(const wchar_t*)lpPara));

				m_pSkinManager->GetConfig()->GetCurParam()->strOwnerName=(const wchar_t*)lpPara;

				m_tagValueDeviceName.SetCaption((const wchar_t*)lpPara);

				SendBluetoothCommand(BT_CMD_SET_DEVICE_NAME,LPARAM(lpPara));
			}
			PlaySoundEffect(0);
		}
		break;
		/*
	case IDC_PANEL_KEYBOARD+IDC_BT_BTN_SET_PAIR_PIN:
		{
			if(lpPara)
			{
				RETAILMSG(DEBUG_SETTING,(_T("CPanelSettingBluetooth IDC_PANEL_KEYBOARD IDC_BT_BTN_SET_PAIR_PIN:%s\r\n"),(const wchar_t*)lpPara));

				m_tagValuePairPin.SetCaption((const wchar_t*)lpPara);

				SendBluetoothCommand(BT_CMD_SET_PIN,LPARAM(lpPara));
			}
			PlaySoundEffect(0);
		}
		break;
		*/
	}
}
void CPanelSettingBluetooth::RefreshSettingItem(BOOL bShow)
{
	//RETAILMSG(1,(_T("CPanelSettingBluetooth::RefreshSettingItem: bShow[%d]\r\n"),bShow));
	m_bShowDetails = bShow;
// 	m_sliPower.SetValue(bShow?10:0);

// 	m_btnConnect.Show(bShow);
// 	m_tagValueConnect.Show(bShow);

	m_btnDeviceName.Show(bShow);
	m_tagValueDeviceName.Show(bShow);

// 	m_btnPairPin.Show(bShow);
// 	m_tagValuePairPin.Show(bShow);

	m_tagTitleAutoConnect.Show(bShow);
	m_sliAutoConnect.Show(bShow);

	m_tagTitleAutoAnswer.Show(bShow);
	m_sliAutoAnswer.Show(bShow);

	m_tagTitleAutoDownloadPB.Show(bShow);
	m_sliAutoDownloadPB.Show(bShow);

// 	m_tagTitleRingtone.Show(bShow);
// 	m_sliRingtone.Show(bShow);

#if CVTE_EN_BT_VCARD_NAME_ORDER
	m_tagTitleVcardNameOrder.Show(bShow);
	m_sliVcardNameOrder.Show(bShow);
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	if(m_bCarPlayConnected)
	{
		m_tagBTNote.Show(!bShow);
	}
	
#endif

	m_btnBluetoothVersion.Show(bShow);
	m_tagValueBluetoothVersion.Show(bShow);

}
//
void CPanelSettingBluetooth::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CARPLAY_CONNECT_STATUS:
		{
			m_bCarPlayConnected = (BOOL)lParam;

			/*
			m_sliPower.Enable(!m_bCarPlayConnected
				&& CONNECTION_STATUS_CONNECTING!=m_hfConnectStatus
				&& m_iBTPowerState!=BT_POWER_OPERATING_OFF
				&& m_iBTPowerState!=BT_POWER_OPERATING_ON);
			*/
			m_sliPower.Enable(!m_bCarPlayConnected && !m_bPhoneCallActive);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)			
			if(!m_bCarPlayConnected)
				m_tagBTNote.Show(FALSE);
#endif				
		}
		break;
	}
}
void CPanelSettingBluetooth::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;
	m_sliPower.Enable(!m_bCarPlayConnected && !m_bPhoneCallActive);
}
void CPanelSettingBluetooth::OnPhoneHFState(WPARAM wParam, LPARAM lParam)
{
	ConnectionStatus state=(ConnectionStatus)wParam;

	m_hfConnectStatus = state;

	/*
	switch (state)
	{
	case CONNECTION_STATUS_CONNECTED:
		m_tagValueConnect.SetCaption(GetResourceString(IDS_CONNECTED));
		break;
	case CONNECTION_STATUS_CONNECTING:
		m_tagValueConnect.SetCaption(GetResourceString(IDS_CONNECTING));
		break;
	case CONNECTION_STATUS_DISCONNECTING:
		m_tagValueConnect.SetCaption(GetResourceString(IDS_DISCONNECTING));
		break;
	case CONNECTION_STATUS_IDLE:
	default:
		m_tagValueConnect.SetCaption(GetResourceString(IDS_NOT_CONNECTED));
		break;
	}
	*/

	m_sliPower.Enable(!m_bCarPlayConnected && !m_bPhoneCallActive);
}
void CPanelSettingBluetooth::OnBluetoothSetupStatus(WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(DEBUG_SETTING,(_T("###CPanelSettingBluetooth::OnBluetoothSetupStatus: wParam[%d] lParam[%d]\r\n"),wParam,lParam));

	switch (wParam)
	{
	case BT_STATUS_POWER:
		{
			m_iBTPowerState = (UINT)lParam;

			m_sliPower.Enable(!m_bCarPlayConnected && !m_bPhoneCallActive);

			m_bOperatePowerOn = FALSE;

			if(m_iBTPowerState==BT_POWER_ON)
			{
                //m_bOperatePowerOn = TRUE;
				RefreshSettingItem(TRUE);
				m_sliPower.SetValue(10);

				//
				if(m_bAutoSwitchDeviceMgr)
				{
					m_bAutoSwitchDeviceMgr = FALSE;
					SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN,PAGE_SWITCH_ANI_DEFAULT,BT_HOMEPAGE_DEVICE);
				}
			}
			else if(m_iBTPowerState==BT_POWER_OFF)
			{
                //m_bOperatePowerOn = FALSE;
				RefreshSettingItem(FALSE);
				m_sliPower.SetValue(0);
			}
		}
		break;

	case BT_STATUS_SETUP_INI:
		{
			RETAILMSG(1/*DEBUG_SETTING*/,(_T("CPanelSettingBluetooth AutoConnect :%d, AutoAnswer:%d, AutoDownloadPB:%d,VcardNameOrder:%d\r\n"),
				((lParam&0x01)!=0),((lParam&0x02)!=0),((lParam&0x04)!=0),((lParam&0x10)!=0)));

			m_sliAutoConnect.SetValue((lParam&0x01)?10:0);
			m_sliAutoAnswer.SetValue((lParam&0x02)?10:0);
			m_sliAutoDownloadPB.SetValue((lParam&0x04)?10:0);
// #if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
// 			m_sliVcardNameOrder.SetValue((lParam&0x10)?10:0);
// #endif
			//m_sliAutoAnswer.SetValue((lParam&0x08)?10:0);
		}
		break;
	case BT_STATUS_VERSION:
		{
			m_tagValueBluetoothVersion.SetCaption((LPCTSTR)lParam);
		}
		break;
	case BT_STATUS_NAME:
		{
			m_tagValueDeviceName.SetCaption((LPCTSTR)lParam);
		}
		break;
	}

}
