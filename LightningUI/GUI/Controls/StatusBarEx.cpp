#include "StatusBarEx.h"
#include "../../resource.h"

CStatusBarEx::CStatusBarEx(void)
{
	m_idBTStatus=0;
	m_idAFStatus=0;
	m_idTAStatus=0;
	m_idEQStatus=0;
	m_idLoudStatus=0;
	m_iVolume=0;

	m_idCurDisplayPanel=-1;
	m_idUIClass=-1;

	m_bBTConnecting=FALSE;
	m_bOnMute=FALSE;
	m_dwVolume=0;
	m_iTimeMode=TIME_MODE_24H;
// 	m_bSimpleMode=FALSE;
// 	m_idShowType=SB_NORMAL;
// 	m_dwLastShowSettingTick=0;
// 	m_idShowTypeBeforeSetting=SB_NORMAL;
// 	m_bEnableButtons=TRUE;

	m_wifiStatus=WIFI_OFF;
	m_bWifiPowerON=TRUE;
	m_dialupStatus=DIALUP_OFF;
	m_statusAF=RDS_OFF;
	m_statusTA=RDS_OFF;

	m_bBtnTimeClicked = FALSE;
}

CStatusBarEx::~CStatusBarEx(void)
{

}
void CStatusBarEx::Render(void)
{

	if(!BeginRender())return;

	if(m_bEnableRender && IsVisible())
	{

		/*
		VIEW_STATE vs;

		vs=m_iconBT.GetCurViewState();
		if(m_bBTConnecting)
		{
			static DWORD last=0;
			if(GetTickCount()-last>=40)
			{
				last = GetTickCount();

				vs.fRotateZ+=5;
				if(vs.fRotateZ>360)
					vs.fRotateZ=0;
				m_iconBT.SetCurViewState(&vs);
			}
		}
		else
		{
			vs.fRotateZ=0;
			m_iconBT.SetCurViewState(&vs);
		}
		*/

		static GLfloat bt_angle=0.0f;
		static GLfloat bt_alpha=1.0f;

		if(m_bBTConnecting)
		{
			static DWORD last=0;
			if(GetTickCount()-last>=40)
			{
				last = GetTickCount();

				bt_angle+=15;
				if(bt_angle>360)
					bt_angle=0;
			}
			bt_alpha=sin(bt_angle*PI/180.0f);
		}
		else
		{
			bt_angle=0;
			bt_alpha=1.0f;
		}
#if CVTE_CUSTOMER_STATUS_BAR_LOGO
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_HOME)
		{
			m_iconCustomerLogo.SetAlpha(m_fCurAlpha);
			m_iconCustomerLogo.Render();
		}
#endif

#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
		m_iconNavi.SetAlpha(m_fCurAlpha);
		m_iconNavi.Render();

		m_btnPage1.SetAlpha(m_fCurAlpha);
		m_btnPage1.Render();

		m_btnPage2.SetAlpha(m_fCurAlpha);
		m_btnPage2.Render();

		m_btnPage3.SetAlpha(m_fCurAlpha);
		m_btnPage3.Render();

		m_btnArrowLeft.SetAlpha(m_fCurAlpha);
		m_btnArrowLeft.Render();

		m_btnArrowRight.SetAlpha(m_fCurAlpha);
		m_btnArrowRight.Render();

		m_btnTopDropMenu.SetAlpha(m_fCurAlpha);
		m_btnTopDropMenu.Render();
#endif

		m_iconBT.SetAlpha(bt_alpha);
		m_iconBT.Render();
/*
		if(m_bOnMute)
		{
			m_iconMute.SetLuminance(m_fCurLuminance);
			m_iconMute.SetAlpha(m_fCurAlpha);
			m_iconMute.Render();
		}
		else
		{
			m_iconVolume.SetLuminance(m_fCurLuminance);
			m_iconVolume.SetAlpha(m_fCurAlpha);
			m_iconVolume.Render();
		}

		m_btnPopup.SetLuminance(m_fCurLuminance);
		m_btnPopup.SetAlpha(m_fCurAlpha);
		m_btnPopup.Render();
*/
		m_iconPhonePower.SetLuminance(m_fCurLuminance);
		m_iconPhonePower.SetAlpha(m_fCurAlpha);
		m_iconPhonePower.Render();

		m_sliPhonePower.SetLuminance(m_fCurLuminance);
		m_sliPhonePower.SetAlpha(m_fCurAlpha);
		m_sliPhonePower.Render();

		m_sliPhoneSignal.SetLuminance(m_fCurLuminance);
		m_sliPhoneSignal.SetAlpha(m_fCurAlpha);
		m_sliPhoneSignal.Render();

// 		m_tagAC.SetAlpha(m_fCurAlpha);
// 		m_tagAC.Render();

		//time
		UpdateTime();
		m_tagTime.SetAlpha(m_fCurAlpha);
		m_tagTime.Render();
		if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H)
		{
			m_tagAM.SetAlpha(m_fCurAlpha);
			m_tagAM.Render();
		}

// 		m_tagEQ.SetAlpha(m_fCurAlpha);
// 		m_tagEQ.Render();

	}

	EndRender();
}
BOOL CStatusBarEx::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CControlGL::Initialize(id,NULL,pSkinManager,pTextGL,hWnd,entry);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL crb={0.75f,0.75f,0.75f,1.0};
	COLORGL crw={1.0f,1.0f,1.0f,1.0};
	COLORGL crGray={0.1,0.1,0.1,1.0};
	GLfloat sx=10.0;
	GLfloat sy=g_iClientHeight/2-35;

	SIZE si60_40={170,40};
	m_tagAM.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAM.SetSize(&si60_40);
	m_tagAM.LinkTextGL(pTextGL);
	m_tagAM.SetPos(g_iClientWidth/2-22,sy-5);
	m_tagAM.SetCaptionHeight(16);
	m_tagAM.EnableDrawBack(FALSE);
	m_tagAM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagAM.SetCaptionColor(&crw);


	SIZE siTime={70,33};
	m_tagTime.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTime.SetSize(&siTime);
	m_tagTime.SetCaption(_T("00:00"));
	m_tagTime.LinkTextGL(pTextGL);
	m_tagTime.SetPos(g_iClientWidth/2-45,sy);
	m_tagTime.SetCaptionHeight(26);
	m_tagTime.EnableDrawBack(FALSE);
	m_tagTime.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTime.SetCaptionColor(&crw);

	int cx=44;
#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
	m_iconNavi.Initialize(IDC_STATUSBAR_TAG_BTSTATUS,this,m_pSkinManager->GetTexture(TEXID_134));
	m_iconNavi.SetPos(-g_iClientWidth/2+118,sy);
	m_iconNavi.Show(FALSE);
#endif

	m_iconBT.Initialize(IDC_STATUSBAR_TAG_BTSTATUS,this,m_pSkinManager->GetTexture(TEXID_142));
	m_iconBT.SetPos(-g_iClientWidth/2+84+sx,sy);
	m_iconBT.Show(FALSE);

	m_iconPhonePower.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_035));
	m_iconPhonePower.SetPos(-g_iClientWidth/2+51+sx,sy);
	m_iconPhonePower.Show(FALSE);

	//slider:
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=34;
	sli.size.cy=32;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_037;
	sli.idChannelTex=TEXID_038;
	sli.idThumbTex=0;

	m_sliPhonePower.Initialize(IDC_STATUSBAR_SLI_PHONEPOWER,this,&sli,pSkinManager);
	m_sliPhonePower.ShowThumb(FALSE);
	// 	m_sliPhonePower.SetRange(0,5);
	m_sliPhonePower.SetRange(0,5);
	m_sliPhonePower.SetValue(3);
	m_sliPhonePower.SetScale(0.6875,1.0);
	m_sliPhonePower.SetPos(-g_iClientWidth/2+50-1+sx,sy);
	m_sliPhonePower.Show(FALSE);

	sli.idBackTex=TEXID_141;	//TEXID_135;
	sli.idChannelTex=TEXID_135;
	m_sliPhoneSignal.Initialize(IDC_STATUSBAR_SLI_PHONESIGNAL,this,&sli,pSkinManager);
	m_sliPhoneSignal.ShowThumb(FALSE);
	m_sliPhoneSignal.SetShowScale(0.9f);
	m_sliPhoneSignal.SetRange(0,5);
	m_sliPhoneSignal.SetValue(3);
	m_sliPhoneSignal.SetPos(-g_iClientWidth/2+16+sx,sy);
	m_sliPhoneSignal.Show(FALSE);

#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
	//Page
	m_iconSwitch.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_105));
	m_iconSwitchSelect.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_106));

	m_btnPage1.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnPage1.SetIcon(&m_iconSwitch);
	m_btnPage1.EnableDrawBack(FALSE);
	m_btnPage1.SetPos(-13, g_iClientHeight/2-52);

	m_btnPage2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnPage2.SetIcon(&m_iconSwitchSelect);
	m_btnPage2.EnableDrawBack(FALSE);
	m_btnPage2.SetPos(0, g_iClientHeight/2-52);

	m_btnPage3.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnPage3.SetIcon(&m_iconSwitch);
	m_btnPage3.EnableDrawBack(FALSE);
	m_btnPage3.SetPos(13, g_iClientHeight/2-52);

	//Arrow
	m_btnArrowLeft.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_140),NULL,
		m_pSkinManager->GetTexture(TEXID_140),NULL,
		NULL,NULL,
		m_pSkinManager->GetTexture(TEXID_139),NULL);
	m_btnArrowLeft.SetPos(-g_iClientWidth/2+16, g_iClientHeight/2-50);

	m_btnArrowRight.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_138),NULL,
		m_pSkinManager->GetTexture(TEXID_138),NULL,
		NULL,NULL,
		m_pSkinManager->GetTexture(TEXID_137),NULL);
	m_btnArrowRight.SetPos(g_iClientWidth/2-16, g_iClientHeight/2-50);

	//Drop Menu
	GLfloat texCoordDropMenu[]={
		231.0/512, 44.0/512.0,
		231.0/512, 78.0/512.0,
		507.0/512, 44.0/512.0,
		507.0/512.0, 78.0/512.0};

	GLfloat texCoordDropMenuSelect[]={
		231.0/512, 5.0/512.0,
		231.0/512, 39.0/512.0,
		507.0/512, 5.0/512.0,
		507.0/512.0, 39.0/512.0};

	m_iconDropMenu.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_100),texCoordDropMenu);
	m_iconDropMenuSelect.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_100),texCoordDropMenuSelect);
	m_btnTopDropMenu.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnTopDropMenu.SetIcon(&m_iconDropMenu,&m_iconDropMenuSelect);
	m_btnTopDropMenu.EnableDrawBack(FALSE);
	//m_btnTopDropMenu.SetNotifyFunc(&CStatusBarEx::OnNotify,this);
	m_btnTopDropMenu.SetPos(0, g_iClientHeight/2-17);

#endif

	// 	m_tagTitle.SetSize(&siInfo);
	// // 	m_tagTitle.SetCaption(str);
	// 	m_tagTitle.LinkTextGL(pTextGL);
	// 	m_tagTitle.SetPos(-210,215);
	// 	m_tagTitle.SetCaptionHeight(30);
	// 	m_tagTitle.EnableDrawBack(FALSE);
	// 	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);

	SetTimeMode(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode);
#else
// 	m_tagBKBottom.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_5));
// 	m_tagBKBottom.SetPos(0,-208);

// 	m_tagBKTop.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_3));
// 	m_tagBKTop.SetPos(0,208);

// 	m_tagLight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_94));
// 	m_tagLight.SetPos(-250,0);

/*
	m_btnPopup.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_021),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPopup.SetNotifyFunc(NULL,NULL);
	m_btnPopup.SetPos(0,215);
	m_btnPopup.SetTouchRate(1.5,2.0);
	m_btnPopup.EnableClickAni(TRUE);
*/

// 	//settings:
// 	memset(&sli,0,sizeof(sli));
// 	sli.size.cx=300;
// 	sli.size.cy=16;
// 	sli.bHasBtns=FALSE;
// 	sli.bFilled=FALSE;
// 	sli.bFilledBoth=FALSE;
// 	sli.idBackTex=TEXID_052;
// 	sli.idChannelTex=TEXID_053;
// 	sli.idThumbTex=TEXID_051;
//
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
// 	m_sliSettings.Initialize(0,this,&sli,m_pSkinManager);
// 	m_sliSettings.SetSliderType(ST_HOR_NORMAL);
// 	m_sliSettings.SetRange(0,64);
// 	m_sliSettings.SetValue(0);
// 	m_sliSettings.SetPos(60,-214);
// 	m_sliSettings.ShowValue(TRUE);
// 	m_sliSettings.LinkTextGL(m_pTextGL);
// 	m_sliSettings.Show(FALSE);
// 	m_sliSettings.SetCaptionColor(&cr_gray);


// 	wchar_t str[MAX_STRING];
// 	::LoadString( m_hInstance, IDS_MAIN_MENU+g_idLanguageOffset, str, MAX_STRING );

// 	SIZE siInfo={360,40};
// 	m_tagTitle.Initialize(IDC_STATUSBAR_TAG_TITLE,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagTitle.SetSize(&siInfo);
// // 	m_tagTitle.SetCaption(str);
// 	m_tagTitle.LinkTextGL(pTextGL);
// 	m_tagTitle.SetPos(-210,215);
// 	m_tagTitle.SetCaptionHeight(30);
// 	m_tagTitle.EnableDrawBack(FALSE);
// 	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);

	COLORGL crb={0.75f,0.75f,0.75f,1.0};
	COLORGL crw={1.0f,1.0f,1.0f,1.0};
	COLORGL crGray={0.1,0.1,0.1,1.0};
	int sx=140;//215;
	int sy=g_iClientHeight/2-35;

	SIZE si60_40={170,40};
	m_tagAM.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAM.SetSize(&si60_40);
	// 	m_tagAM.SetCaption(str);
	m_tagAM.LinkTextGL(pTextGL);
// 	m_tagAM.SetPos(-276,220);
// 	m_tagAM.SetPos(g_iClientWidth/2-140,g_iClientHeight/2-50-2);
	m_tagAM.SetPos(g_iClientWidth/2-26,sy-3);
	m_tagAM.SetCaptionHeight(16);
	m_tagAM.EnableDrawBack(FALSE);
	m_tagAM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagAM.SetCaptionColor(&crw);


	m_tagTime.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTime.SetSize(&si60_40);
	m_tagTime.SetCaption(_T("00:00"));
	m_tagTime.LinkTextGL(pTextGL);
// 	m_tagTime.SetPos(-348,215);
	m_tagTime.SetPos(g_iClientWidth/2-100,sy);
	m_tagTime.SetCaptionHeight(28);
	m_tagTime.EnableDrawBack(FALSE);
	m_tagTime.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagTime.SetCaptionColor(&crw);

// 	m_tagAC.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagAC.SetSize(&si60_40);
// 	m_tagAC.SetCaption(_T("26C/34C"));
// 	m_tagAC.LinkTextGL(pTextGL);
// 	m_tagAC.SetPos(-g_iClientWidth/2+90+3,g_iClientHeight/2-50+6);
// 	m_tagAC.SetCaptionHeight(40);
// 	m_tagAC.EnableDrawBack(FALSE);
// 	m_tagAC.SetCaptionPos(BTN_CAPTION_LEFT);
// 	m_tagAC.SetCaptionColor(&crb);

// 	m_tagVolume.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagVolume.SetSize(&si60_40);
// 	m_tagVolume.SetCaption(_T("00"));
// 	m_tagVolume.LinkTextGL(pTextGL);
// 	m_tagVolume.SetPos(363,215);
// 	m_tagVolume.SetCaptionHeight(30);
// 	m_tagVolume.EnableDrawBack(FALSE);
// 	m_tagVolume.SetCaptionPos(BTN_CAPTION_CENTER);

// 	m_tagEQ.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagEQ.SetSize(&si60_40);
// 	m_tagEQ.SetCaption(_T(""));
// 	m_tagEQ.LinkTextGL(pTextGL);
// 	m_tagEQ.SetPos(-200,215);
// 	m_tagEQ.SetCaptionHeight(30);
// 	m_tagEQ.EnableDrawBack(FALSE);
// 	m_tagEQ.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_tagEQ.SetCaptionColor(&crb);

// 	m_tagAF.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagAF.SetSize(&si60_40);
// 	m_tagAF.SetCaption(_T("AF"));
// 	m_tagAF.LinkTextGL(pTextGL);
// 	m_tagAF.SetPos(60,215);
// 	m_tagAF.SetCaptionHeight(28);
// 	m_tagAF.EnableDrawBack(FALSE);
// 	m_tagAF.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_tagAF.SetCaptionColor(&cr_gray);
//
// 	m_tagTA.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagTA.SetSize(&si60_40);
// 	m_tagTA.SetCaption(_T("TA"));
// 	m_tagTA.LinkTextGL(pTextGL);
// 	m_tagTA.SetPos(10,215);
// 	m_tagTA.SetCaptionHeight(28);
// 	m_tagTA.EnableDrawBack(FALSE);
// 	m_tagTA.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_tagTA.SetCaptionColor(&cr_gray);

	/*
	SIZE siInfo2={240,40};

	m_tagSettingItemName.Initialize(IDC_STATUSBAR_TAG_SETTING_NAME,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagSettingItemName.SetSize(&siInfo2);
	m_tagSettingItemName.SetCaption((const wchar_t*)	::LoadString( m_hInstance, IDS_SETTINGS_VOLUME+g_idLanguageOffset, NULL, 0 ));
	m_tagSettingItemName.LinkTextGL(m_pTextGL);
	m_tagSettingItemName.SetPos(-160,-214);
	m_tagSettingItemName.SetCaptionHeight(26);
	m_tagSettingItemName.EnableDrawBack(FALSE);
	m_tagSettingItemName.SetCaptionPos(BTN_CAPTION_CENTER);

	m_tagPhoneName.Initialize(IDC_STATUSBAR_TAG_PHONE_NAME,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPhoneName.SetSize(&siInfo2);
	m_tagPhoneName.SetCaption(_T("NAME"));
	m_tagPhoneName.LinkTextGL(m_pTextGL);
	m_tagPhoneName.SetPos(-140,-214);
	m_tagPhoneName.SetCaptionHeight(26);
	m_tagPhoneName.EnableDrawBack(FALSE);
	m_tagPhoneName.SetCaptionPos(BTN_CAPTION_CENTER);

	m_tagPhoneNumber.Initialize(IDC_STATUSBAR_TAG_PHONE_NUMBER,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPhoneNumber.SetSize(&siInfo2);
	m_tagPhoneNumber.SetCaption(_T("13570204516"));
	m_tagPhoneNumber.LinkTextGL(m_pTextGL);
	m_tagPhoneNumber.SetPos(80,-214);
	m_tagPhoneNumber.SetCaptionHeight(26);
	m_tagPhoneNumber.EnableDrawBack(FALSE);
	m_tagPhoneNumber.SetCaptionPos(BTN_CAPTION_CENTER);

	*/

// 	if(g_iClientWidth>=1024)
// 	{
// 		sx=200;
// 		sy=g_iClientHeight/2-52;
// 	}
// 	else
// 	{
// 		sx=36;
// 		sy=g_iClientHeight/2-14;
// 	}

	int cx=44;

// 	m_iconVolume.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_010));
// 	m_iconVolume.SetPos(sx-cx*1,215);
//
// 	m_iconMute.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_011));
// 	m_iconMute.SetPos(sx-cx*1,215);

// 	m_iconWifi.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_16));
// 	m_iconWifi.SetPos(sx-cx*1,215);
// 	m_iconWifi.EnableLight(TRUE);
// 	m_iconWifi.SetLightColor(&crGray);
// 	m_iconWifi.SetAniSpeed(14);
//
// 	m_icon3G.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_2));
// 	m_icon3G.SetPos(sx-cx*2,215);
// 	m_icon3G.EnableLight(TRUE);
// 	m_icon3G.SetLightColor(&crGray);
// 	m_icon3G.SetAniSpeed(14);

// 	m_iconRDS.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_019));
// 	m_iconRDS.SetPos(sx-cx*2,215);
// 	m_iconRDS.EnableLight(TRUE);
// 	m_iconRDS.SetLightColor(&crGray);

#if CVTE_CUSTOMER_STATUS_BAR_LOGO
	//RETAILMSG(1, (TEXT("MSG:[%s]: Initialize [statusbarlogo.tif]\r\n"), TEXT(__FUNCTION__)));
	m_iconCustomerLogo.Initialize(0,this,/*_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\statusbarlogo.tif")*/m_pSkinManager->GetTexture(TEXID_901));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_iconCustomerLogo.SetScale(0.8f/pix_ratio,0.8f);
	m_iconCustomerLogo.SetPos(-g_iClientWidth/2+16+m_iconCustomerLogo.GetScaledSize().cx/2,sy+4);
#else
	m_iconCustomerLogo.SetShowScale(0.9f);
	m_iconCustomerLogo.SetPos(-g_iClientWidth/2+100,sy+5);
#endif
#endif

	m_iconBT.Initialize(IDC_STATUSBAR_TAG_BTSTATUS,this,m_pSkinManager->GetTexture(TEXID_023));
	m_iconBT.SetPos(g_iClientWidth/2-sx,sy);
	//m_iconBT.EnableLight(TRUE);
	//m_iconBT.SetLightColor(&crGray);
// 	m_iconBT.SetAlpha(0.6f);
	m_iconBT.Show(FALSE);

	m_iconPhonePower.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_035));
	m_iconPhonePower.SetPos(g_iClientWidth/2-sx-cx*1,sy);
	m_iconPhonePower.Show(FALSE);

	//slider:
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=32;
	sli.size.cy=32;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_037;
	sli.idChannelTex=TEXID_038;
	sli.idThumbTex=0;

	m_sliPhonePower.Initialize(IDC_STATUSBAR_SLI_PHONEPOWER,this,&sli,pSkinManager);
	m_sliPhonePower.ShowThumb(FALSE);
	// 	m_sliPhonePower.SetRange(0,5);
	m_sliPhonePower.SetRange(0,5);
	m_sliPhonePower.SetValue(3);
	m_sliPhonePower.SetScale(0.6875,1.0);
	m_sliPhonePower.SetPos(g_iClientWidth/2-sx-cx*1-2,sy);
	m_sliPhonePower.Show(FALSE);

	sli.idBackTex=TEXID_024;
	sli.idChannelTex=TEXID_029;
	m_sliPhoneSignal.Initialize(IDC_STATUSBAR_SLI_PHONESIGNAL,this,&sli,pSkinManager);
	m_sliPhoneSignal.ShowThumb(FALSE);
	m_sliPhoneSignal.SetRange(0,5);
	m_sliPhoneSignal.SetValue(0);
	m_sliPhoneSignal.SetPos(g_iClientWidth/2-sx-cx*2,sy);
	m_sliPhoneSignal.Show(FALSE);

// 	m_iconPower.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_7));
// 	m_iconPower.SetPos(-260,215);
// 	m_iconPower.EnableLight(TRUE);
// 	m_iconPower.SetLightColor(&crLight);

// 	m_iconCall.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_11));
// 	m_iconCall.SetPos(-210,215);
// 	m_iconCall.EnableLight(TRUE);
// 	m_iconCall.SetLightColor(&crLight);
//
// 	m_iconSMS.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_12));
// 	m_iconSMS.SetPos(-160,215);
// 	m_iconSMS.EnableLight(TRUE);
// 	m_iconSMS.SetLightColor(&crLight);


	/*
	m_iconAdd.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_75));
	m_iconSub.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_76));
	m_iconDial.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_060));
	m_iconHang.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_61));

	m_iconHome.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_14));
	m_btHome.Initialize(IDC_STATUSBAR_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_18),NULL,
		m_pSkinManager->GetTexture(TEXID_19),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btHome.SetPos(-330,-208);
	m_btHome.SetIcon(&m_iconHome);
	m_btHome.SetIconPos(-6.0f,0.0f);
	m_btHome.SetScale(1.1,1.0);


	m_iconBack.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_13));
	VIEW_STATE vs2=m_iconBack.GetCurViewState();
	vs2.fRotateY=-180;
	m_iconBack.SetCurViewState(&vs2);
	m_iconAdd.SetCurViewState(&vs2);
	m_iconHang.SetCurViewState(&vs2);

	m_btBack.Initialize(IDC_STATUSBAR_BTN_BACK,this,m_pSkinManager->GetTexture(TEXID_18),NULL,
		m_pSkinManager->GetTexture(TEXID_19),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btBack.SetPos(330,-208);
	m_btBack.SetIcon(&m_iconBack);
	m_btBack.SetIconPos(-6.0f,0.0f);
	m_btBack.SetScale(1.1,1.0);

	VIEW_STATE vs=m_btBack.GetCurViewState();
	vs.fRotateY=-180;
	m_btBack.SetCurViewState(&vs);


	//time:
	COLORGL crb={0,0,0,1};
	m_tagHour1.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_22),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagHour1.SetCaption(_T("0"));
	m_tagHour1.LinkTextGL(pTextGL);
	m_tagHour1.SetPos(-50,214);
	m_tagHour1.SetCaptionHeight(26);
// 	m_tagHour1.SetCaptionCharStyle(STYLE_ART);
	m_tagHour1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagHour1.SetCaptionColor(&crb);

	m_tagHour2.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_22),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagHour2.SetCaption(_T("0"));
	m_tagHour2.LinkTextGL(pTextGL);
	m_tagHour2.SetPos(-20,214);
	m_tagHour2.SetCaptionHeight(26);
// 	m_tagHour2.SetCaptionCharStyle(STYLE_ART);
	m_tagHour2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagHour2.SetCaptionColor(&crb);

	m_tagMin1.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_22),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagMin1.SetCaption(_T("0"));
	m_tagMin1.LinkTextGL(pTextGL);
	m_tagMin1.SetPos(20,214);
	m_tagMin1.SetCaptionHeight(26);
// 	m_tagMin1.SetCaptionCharStyle(STYLE_ART);
	m_tagMin1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagMin1.SetCaptionColor(&crb);

	m_tagMin2.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_22),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagMin2.SetCaption(_T("0"));
	m_tagMin2.LinkTextGL(pTextGL);
	m_tagMin2.SetPos(50,214);
	m_tagMin2.SetCaptionHeight(26);
// 	m_tagMin2.SetCaptionCharStyle(STYLE_ART);
	m_tagMin2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagMin2.SetCaptionColor(&crb);

	m_tagTimeSep.Initialize(0,this,NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTimeSep.SetCaption(_T(":"));
	m_tagTimeSep.LinkTextGL(pTextGL);
	m_tagTimeSep.SetPos(0,214);
	m_tagTimeSep.SetCaptionHeight(26);
	m_tagTimeSep.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTimeSep.EnableDrawBack(FALSE);

	*/

	SetTimeMode(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode);
#endif
	return TRUE;
}

void CStatusBarEx::Show(BOOL bShow,BOOL bAni)
{
	CObjectGL::Show(bShow,FALSE);

}
BOOL CStatusBarEx::IsReady()
{
	return CControlGL::IsReady() && (m_wifiStatus!=WIFI_CONNECTING) && (m_dialupStatus!=DIALUP_CONNECTING)
		 && (!m_bBTConnecting);
}
void CStatusBarEx::OnTouchDown(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:[%s] [%d,%d]\r\n"),TEXT(__FUNCTION__),pt->x,pt->y));
	static RECT rcBtn_Time = {650, 0, 800, 40};
	m_bBtnTimeClicked = PtInRect(&rcBtn_Time,*pt);
#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
	static RECT rcTopDropMenu = {252,0,548,34};
	m_bBtnTopDropMenuClicked = PtInRect(&rcTopDropMenu, *pt);

	if (m_bBtnTopDropMenuClicked)
	{
		RETAILMSG(1, (TEXT("MSG:[%s] Down\r\n"),TEXT(__FUNCTION__)));
		m_btnTopDropMenu.SetStatus(BS_DOWN);
		PlaySoundEffect(0);
	}
#endif
}
void CStatusBarEx::OnTouchUp(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:[%s] [%d,%d]\r\n"),TEXT(__FUNCTION__),pt->x,pt->y));

	if(m_bBtnTimeClicked)
	{
		SwitchPage(UI_CLASS_IDLE,0,PAGE_SWITCH_ANI_NONE,2);
		PlaySoundEffect(0);
		return;
	}

	m_bBtnTimeClicked = FALSE;

#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
	if (m_bBtnTopDropMenuClicked)
	{
		RETAILMSG(1, (TEXT("MSG:[%s] Up\r\n"),TEXT(__FUNCTION__)));
		m_bBtnTopDropMenuClicked = FALSE;
		m_btnTopDropMenu.SetStatus(BS_UP);
	}
#endif

// 	m_btnPopup.OnTouchUp(pt);
//
// 	if(m_btnPopup.IsPtnOnObject(pt))
// 	{
// 		//::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_ALLAPP,NULL);
// 	}

// 	{
// 		//quick set time:
// 		if(pt->x>350 && pt->x<450 && pt->y<80 && m_ptLastPress.x>350 && m_ptLastPress.x<450 && m_ptLastPress.y<80)
// 		{
// // 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_SETTINGCORE,(1<<16)+(PAGE_SYSTEM_TIME<<8)+PAGE_SETTINGS_SYSTEM);
// 			return;
// 		}
// 		//quick audio setting
// 		else if(pt->x>0 && pt->x<50 && pt->y<80 && m_ptLastPress.x>0 && m_ptLastPress.x<50 && m_ptLastPress.y<80)
// 		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_SETTINGCORE,(1<<16)+(0<<8)+PAGE_SETTINGS_AUDIO);
// 			return;
// 		}
// 		//quick blue tooth setting
// 		else if(pt->x>50 && pt->x<100 && pt->y<80 && m_ptLastPress.x>50 && m_ptLastPress.x<100 && m_ptLastPress.y<80)
// 		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_BTCORE,NULL);
// 			return;
// 		}
// 		//quick wifi setting
// 		else if(pt->x>640 && pt->x<720 && pt->y<80 && m_ptLastPress.x>640 && m_ptLastPress.x<720 && m_ptLastPress.y<80)
// 		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_SETTINGCORE,(1<<16)+(0<<8)+PAGE_SETTINGS_WIRELESS);
// 			return;
// 		}
// 		//quick 3g setting
// 		else if(pt->x>720 && pt->x<800 && pt->y<80 && m_ptLastPress.x>720 && m_ptLastPress.x<800 && m_ptLastPress.y<80)
// 		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_SETTINGCORE,(1<<16)+(0<<8)+PAGE_SETTINGS_DIALUP);
// 			return;
// 		}
// 	}


}
void CStatusBarEx::OnTouchMove(POINT *pt)
{

// 	m_btnPopup.OnTouchMove(pt);
}
void CStatusBarEx::OnGesture(GESTUREINFO *pgi)
{

}


void CStatusBarEx::SetTimeMode(UINT iTimeMode)
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat sy=g_iClientHeight/2-35;
	m_iTimeMode = iTimeMode;

	if(m_iTimeMode==TIME_MODE_12H)
	{
		m_tagTime.SetPos(g_iClientWidth/2-45-32,sy);
	}
	else
	{
		m_tagTime.SetPos(g_iClientWidth/2-45,sy);
	}
#else
	int sx;//215;
	int sy=g_iClientHeight/2-35;
	int cx;

	m_iTimeMode = iTimeMode;

	if(m_iTimeMode==TIME_MODE_12H)
	{
		cx = 38;
		sx = 140;
		m_tagTime.SetCaptionHeight(24);
		m_tagTime.SetPos(g_iClientWidth/2-126,sy);
		m_iconBT.SetPos(g_iClientWidth/2-sx,sy);
		m_iconPhonePower.SetPos(g_iClientWidth/2-sx-cx*1,sy);
		m_sliPhonePower.SetPos(g_iClientWidth/2-sx-cx*1-2,sy);
		m_sliPhoneSignal.SetPos(g_iClientWidth/2-sx-cx*2,sy);
	}
	else
	{
		cx = 44;
		sx = 130;
		m_tagTime.SetCaptionHeight(28);
		m_tagTime.SetPos(g_iClientWidth/2-100,sy);
		m_iconBT.SetPos(g_iClientWidth/2-sx,sy);
		m_iconPhonePower.SetPos(g_iClientWidth/2-sx-cx*1,sy);
		m_sliPhonePower.SetPos(g_iClientWidth/2-sx-cx*1-2,sy);
		m_sliPhoneSignal.SetPos(g_iClientWidth/2-sx-cx*2,sy);
	}
#endif
}


void CStatusBarEx::SetAFStatus(RDS_STATUS status)
{
	m_statusAF = status;
	switch (status)
	{
	case RDS_OFF:
		{
		}
		break;
	case RDS_ON:
		{
		}
		break;
	case RDS_SEARCHING:
		{
		}
		break;
	}
}
void CStatusBarEx::SetBTStatus(DWORD dwStatus,DWORD dwSignal,DWORD dwBattery)
{
	RETAILMSG(DEBUG_STATUSBAR,(_T("<%s>:: dwStatus:%d,dwSignal:%d,dwBattery:%d\r\n"),_T(__FUNCTION__),dwStatus,dwSignal,dwBattery));

	m_bBTConnecting=(dwStatus==1);

	if(dwBattery!=-1)
		m_sliPhonePower.SetValue(dwBattery);

	if(dwSignal!=-1)
		m_sliPhoneSignal.SetValue(dwSignal);

	m_iconBT.Show(dwStatus!=0);

	switch (dwStatus)
	{
	case 0://no phone
		{
// 			COLORGL crGray={0.1,0.1,0.1,1};
// 			m_iconBT.SetLightColor(&crGray);
			m_sliPhonePower.Show(FALSE);
			m_sliPhoneSignal.Show(FALSE);
			m_iconPhonePower.Show(FALSE);
// 			m_iconBT.SetAlpha(0.6f);
		}
		break;
	case 1://connecting
		{
// 			COLORGL crBlue={0,0,1,1};
// 			m_iconBT.SetLightColor(&crBlue);
			m_sliPhonePower.Show(FALSE);
			m_sliPhoneSignal.Show(FALSE);
			m_iconPhonePower.Show(FALSE);
// 			m_iconBT.SetAlpha(1.0f);
		}
		break;
	case 2://connected
		{
// 			COLORGL crGreen={0,1,0,1};
// 			m_iconBT.SetLightColor(&crGreen);
			m_sliPhonePower.Show(TRUE);
			m_sliPhoneSignal.Show(TRUE);
			m_iconPhonePower.Show(TRUE);
// 			m_iconBT.SetAlpha(1.0f);
		}
		break;
	case 3://disconnected
		{
// 			COLORGL crRed={1,0,0,1};
// 			m_iconBT.SetLightColor(&crRed);
			m_sliPhonePower.Show(FALSE);
			m_sliPhoneSignal.Show(FALSE);
			m_iconPhonePower.Show(FALSE);
// 			m_iconBT.SetAlpha(1.0f);
		}
		break;
	default:
		{
// 			COLORGL crGray={0.1,0.1,0.1,1};
// 			m_iconBT.SetLightColor(&crGray);
			m_sliPhonePower.Show(FALSE);
			m_sliPhoneSignal.Show(FALSE);
			m_iconPhonePower.Show(FALSE);
// 			m_iconBT.SetAlpha(0.6f);
		}
		break;
	}
}
void CStatusBarEx::SetTAStatus(RDS_STATUS status)
{
	m_statusTA = status;
	switch (status)
	{
	case RDS_OFF:
		{
			COLORGL crGray={0.1,0.1,0.1,1};
			m_iconRDS.SetLightColor(&crGray);
		}
		break;
	case RDS_ON:
		{
			COLORGL crWhite={1,1,1,1};
			m_iconRDS.SetLightColor(&crWhite);
		}
		break;
	case RDS_SEARCHING:
		{
			COLORGL crGreen={0,1,0,1};
			m_iconRDS.SetLightColor(&crGreen);
		}
		break;
	}
}

#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
void CStatusBarEx::SetNaviStatus(BOOL bEnable)
{
	m_iconNavi.Show(bEnable);
}
#endif

void CStatusBarEx::SetDialupStatus(DIALUP_STATUS status)
{
	RETAILMSG(DEBUG_STATUSBAR,(_T("+++++SetDialupStatus %d\r\n"),status));

	m_dialupStatus = status;

	switch (status)
	{
	case DIALUP_OFF://no
		{
			COLORGL crGray={0.1,0.1,0.1,1};
			m_icon3G.EnableLight(TRUE);
			m_icon3G.SetLightColor(&crGray);
		}
		break;
	case DIALUP_CONNECTING://connecting
		{
			m_icon3G.EnableLight(FALSE);
		}
		break;
	case DIALUP_CONNECTED://connected
		{
			COLORGL crGreen={0,1,0,1};
			m_icon3G.EnableLight(TRUE);
			m_icon3G.SetLightColor(&crGreen);
		}
		break;
	case DIALUP_DISCONNECTED://disconnected
	case DIALUP_DISCONNECTING://disconnecting
		{
			COLORGL crGray={0.1,0.1,0.1,1};
			m_icon3G.EnableLight(TRUE);
			m_icon3G.SetLightColor(&crGray);
		}
		break;
	}
}
void CStatusBarEx::SetVolume(DWORD dwVol)
{
	wchar_t temp[16];
	m_dwVolume=dwVol;

	swprintf_s(temp,16,_T("%02d"),dwVol);
	m_tagVolume.SetCaption(temp);
}
void CStatusBarEx::SetEQ(UINT idEQ)
{
	switch (idEQ)
	{
	case 0:
		m_tagEQ.SetCaption(_T(""));
		break;
	case 1:
		//m_tagEQ.SetCaption((const wchar_t*)::LoadString( m_hInstance, IDS_SETTINGS_EQ_POP+g_idLanguageOffset, NULL, 0 ),NULL);
		break;
	case 2:
		//m_tagEQ.SetCaption((const wchar_t*)::LoadString( m_hInstance, IDS_SETTINGS_EQ_CLASSICAL+g_idLanguageOffset, NULL, 0 ),NULL);
		break;
	case 3:
		//m_tagEQ.SetCaption((const wchar_t*)::LoadString( m_hInstance, IDS_SETTINGS_EQ_ROCK+g_idLanguageOffset, NULL, 0 ),NULL);
		break;
	}
}
void CStatusBarEx::SetWiFiStatus(WIFI_STATUS status)
{
	if(!m_bWifiPowerON)
		return;

	RETAILMSG(DEBUG_STATUSBAR,(_T("+++++SetWiFiStatus %d\r\n"),status));

	m_wifiStatus = status;

	switch (status)
	{
	case WIFI_OFF://no wifi
		{
			COLORGL crGray={0.1,0.1,0.1,1};
			m_iconWifi.EnableLight(TRUE);
			m_iconWifi.SetLightColor(&crGray);
		}
		break;
	case WIFI_CONNECTING://connecting
		{
// 			COLORGL crBlue={0,0,1,1};
// 			m_iconWifi.SetLightColor(&crBlue);
			m_iconWifi.EnableLight(FALSE);
		}
		break;
	case WIFI_CONNECTED://connected
		{
			COLORGL crGreen={0,1,0,1};
			m_iconWifi.EnableLight(TRUE);
			m_iconWifi.SetLightColor(&crGreen);
		}
		break;
	case WIFI_DISCONNECTED://disconnected
	case WIFI_DISCONNECTING://disconnecting
		{
			COLORGL crGray={0.1,0.1,0.1,1};
			m_iconWifi.EnableLight(TRUE);
			m_iconWifi.SetLightColor(&crGray);
		}
		break;
	}
}
void CStatusBarEx::UpdateTime()
{
	static DWORD last_tick=0;
	DWORD this_tick=GetTickCount();

	if(this_tick-last_tick>1000)//update per second
	{
		SYSTEMTIME sys_time;
		int hour1,hour2,min1,min2;
		wchar_t str[16]={0};
// 		RETAILMSG(DEBUG_STATUSBAR,(_T("UpdateTime...\r\n")));
		last_tick=this_tick;
		GetLocalTime(&sys_time);

		if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H)
		{
			m_tagAM.SetCaption(sys_time.wHour>=12?_T("PM"):_T("AM"));
			#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
			m_tagAM.SetPos(g_iClientWidth/2-26,g_iClientHeight/2-(sys_time.wHour>=12 ? 35 : 29)-3);
			#endif
			sys_time.wHour=sys_time.wHour%12;
			if(sys_time.wHour==0)
				sys_time.wHour=12;
		}
		hour1=sys_time.wHour/10;
		hour2=sys_time.wHour%10;

		min1=sys_time.wMinute/10;
		min2=sys_time.wMinute%10;

		swprintf_s(str,16,_T("%02d:%02d"),sys_time.wHour,sys_time.wMinute);
		m_tagTime.SetCaption(str);

		/*
		if(hour1!=m_tagHour1.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),hour1);
			m_tagHour1.SetCaption(str);
			m_tagHour1.UpdateID(hour1);
			VIEW_STATE vs=m_tagHour1.GetCurViewState();
			vs.fRotateX=-180;
			m_tagHour1.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagHour1.SetAimViewState(&vs);
		}
		if(hour2!=m_tagHour2.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),hour2);
			m_tagHour2.SetCaption(str);
			m_tagHour2.UpdateID(hour2);
			VIEW_STATE vs=m_tagHour2.GetCurViewState();
			vs.fRotateX=-180;
			m_tagHour2.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagHour2.SetAimViewState(&vs);
		}
		if(min1!=m_tagMin1.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),min1);
			m_tagMin1.SetCaption(str);
			m_tagMin1.UpdateID(min1);
			VIEW_STATE vs=m_tagMin1.GetCurViewState();
			vs.fRotateX=-180;
			m_tagMin1.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagMin1.SetAimViewState(&vs);
		}
		if(min2!=m_tagMin2.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),min2);
			m_tagMin2.SetCaption(str);
			m_tagMin2.UpdateID(min2);
			VIEW_STATE vs=m_tagMin2.GetCurViewState();
			vs.fRotateX=-180;
			m_tagMin2.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagMin2.SetAimViewState(&vs);
		}
		*/
	}
}

#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
void CALLBACK CStatusBarEx::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(1,(_T("MSG:CStatusBarEx OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CStatusBarEx *panel=(CStatusBarEx*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CStatusBarEx::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{

	}
}
#endif