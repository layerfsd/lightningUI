#include "PanelSettingGeneral.h"
#include "../../resource.h"
#include "../Idle/PanelIdle.h"
#include "CarplayAPI.h"
#include "../../Include/oemsector.h"
#include "../Bluetooth FLC/BluetoothCommon.h"
#include "../../Include/CVTE_GpioProxy.h"
extern const wchar_t* g_strInstallPath;
//	extern BOOL						g_bKeyBeep;
extern DWORD					g_dwTimeSyncedCnt;
extern const wchar_t* g_strDefaultBackground;

const wchar_t* g_strRearViewDelayValue[5] =
{
	_T("0 Sec"),
	_T("0.1 Sec"),
	_T("0.5 Sec"),
	_T("1 Sec"),
	_T("2 Sec"),
};

const wchar_t* g_strLanguage[16] =
{
	_T("English"),			//	0
	_T(""),					//	1
	_T("繁體中文"),			//	2
	_T("简体中文"),			//	3
	_T("???"),			//	4
	_T("Deutsch"),			//	5
	_T("Espa?ol"),			//	6
	_T("Português"),		//	7
	_T("Italiano"),			//	8
	_T("Fran?ais"),			//	9
	_T("日本語"),			//	10
	_T("Русский"),	//	11
	_T("?????"),				//	12
	_T("ελληνικ?"),	//  13
	_T(""),
	_T("")
};

static BOOL IsFileExists(LPCTSTR   lpszFileName)
{
	WIN32_FIND_DATA   wfd;
	BOOL   bRet;
	HANDLE   hFind;

	if(!lpszFileName)
		return FALSE;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   hFind   !=   INVALID_HANDLE_VALUE;
	FindClose(hFind);
	return   bRet;
}

CPanelSettingGeneral::CPanelSettingGeneral(void)
{

}
CPanelSettingGeneral::~CPanelSettingGeneral(void)
{

}

BOOL CPanelSettingGeneral::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingGeneral::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	TIME_ZONE_INFORMATION tziCurrent;
	DWORD tzid = GetTimeZoneInformation(&tziCurrent);

	m_sliTimeAutoAdjustDLST.Enable(ZoneHasDaylightTime(tziCurrent));

	CReg reg(HKEY_LOCAL_MACHINE, RK_CLOCK);

	m_sliTimeAutoAdjustDLST.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDSTAutoAdjust?10:0);
}

void CPanelSettingGeneral::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingGeneral::Render()
{
	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_606):NULL;

	static DWORD s_dwlastUpdateTimeTick=0;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

// 	//
// 	glEnable(GL_SCISSOR_TEST);
// 	glScissor(240,0,g_iClientWidth-240,g_iClientHeight);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_BOSSAUDIOUI)
	//brake test:
	m_tagTitleBrakeTest.SetAlpha(m_fCurAlpha);
	m_tagTitleBrakeTest.Render();
	m_sliBrakeTest.SetAlpha(m_fCurAlpha);
	m_sliBrakeTest.Render();

	//lamp control:
	m_tagTitleLampControl.SetAlpha(m_fCurAlpha);
	m_tagTitleLampControl.Render();
	m_sliLampControl.SetAlpha(m_fCurAlpha);
	m_sliLampControl.Render();

	//drive type:
	m_btnDriveType.SetAlpha(m_fCurAlpha);
	m_btnDriveType.Render();
	m_tagValueDriveType.SetAlpha(m_fCurAlpha);
	m_tagValueDriveType.Render();
#endif
#endif
	//key beep:
//	m_tagTitleKeyBeep.SetAlpha(m_fCurAlpha);
//	m_tagTitleKeyBeep.Render();

//	m_sliKeyBeep.SetAlpha(m_fCurAlpha);
//	m_sliKeyBeep.Render();


	//idle screen:
//	    m_btnIdleScreen.SetAlpha(m_fCurAlpha);
//	    m_btnIdleScreen.Render();
//	    m_tagValueIdleScreen.SetAlpha(m_fCurAlpha);
//	    m_tagValueIdleScreen.Render();

//		//drive type:
//		m_btnDriveType.SetAlpha(m_fCurAlpha);
//		m_btnDriveType.Render();
//		m_tagValueDriveType.SetAlpha(m_fCurAlpha);
//		m_tagValueDriveType.Render();

#if 0//(CUSTOMER_ID != CUSTOMER_ENTERFRONT)
    //Rear color signal type:
    m_btnRearColorSignalType.SetAlpha(m_fCurAlpha);
	m_btnRearColorSignalType.Render();
	m_tagRearColorSignalType.SetAlpha(m_fCurAlpha);
	m_tagRearColorSignalType.Render();
#endif

// 	//camera type:
// 	m_btnCameraType.SetAlpha(m_fCurAlpha);
// 	m_btnCameraType.Render();
// 	m_tagValueCameraType.SetAlpha(m_fCurAlpha);
// 	m_tagValueCameraType.Render();

	//Time and date:
	m_tagTitleTimeAndDate.SetAlpha(m_fCurAlpha);
	m_tagTitleTimeAndDate.Render();
	m_btnDate.SetAlpha(m_fCurAlpha);
	m_btnDate.Render();
	m_btnTime.SetAlpha(m_fCurAlpha);
	m_btnTime.Render();
	m_btnTimeZone.SetAlpha(m_fCurAlpha);
	m_btnTimeZone.Render();

	//update time:
	if(GetTickCount()-s_dwlastUpdateTimeTick>1000)
	{
		s_dwlastUpdateTimeTick = GetTickCount();

		SYSTEMTIME sys_time;
		wchar_t str[32];
		GetLocalTime(&sys_time);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		swprintf_s(str,32,_T("%02d/%02d/%02d"),sys_time.wMonth,sys_time.wDay,sys_time.wYear);
#else
		swprintf_s(str,32,_T("%d-%02d-%02d"),sys_time.wYear,sys_time.wMonth,sys_time.wDay);
#endif
		m_tagValueDate.SetCaption(str);
		if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H)
		{

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
			if(sys_time.wHour>=12)
				swprintf_s(str,32,_T("%d:%02d PM"),(sys_time.wHour-12)?(sys_time.wHour-12):12,sys_time.wMinute);
			else
				swprintf_s(str,32,_T("%d:%02d AM"),sys_time.wHour?sys_time.wHour:12,sys_time.wMinute);
#else
			if(sys_time.wHour>=12)
				swprintf_s(str,32,_T("PM %d:%02d"),(sys_time.wHour-12)?(sys_time.wHour-12):12,sys_time.wMinute);
			else
				swprintf_s(str,32,_T("AM %d:%02d"),sys_time.wHour?sys_time.wHour:12,sys_time.wMinute);
#endif	

		}
		else
		{
			swprintf_s(str,32,_T("%d:%02d"),sys_time.wHour,sys_time.wMinute);
		}

		m_tagValueTime.SetCaption(str);
	}
	m_tagValueDate.SetAlpha(m_fCurAlpha);
	m_tagValueDate.Render();
	m_tagValueTime.SetAlpha(m_fCurAlpha);
	m_tagValueTime.Render();

	m_tagTitleTime24HMode.SetAlpha(m_fCurAlpha);
	m_tagTitleTime24HMode.Render();

	m_sliTime24HMode.SetAlpha(m_fCurAlpha);
	m_sliTime24HMode.Render();

#if CVTE_EN_NAVI
	m_tagTitleTimeAutoAdjust.SetAlpha(m_fCurAlpha);
	m_tagTitleTimeAutoAdjust.Render();
	m_sliTimeAutoAdjust.SetAlpha(m_fCurAlpha);
	m_sliTimeAutoAdjust.Render();
#endif

#if (!CVTE_EN_STEER_WHEEL_OLD_UI_STYLE || (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI))
	//Time auto adjust
	m_tagTitleTimeAutoAdjustDLST.SetAlpha(m_fCurAlpha);
	m_tagTitleTimeAutoAdjustDLST.Render();
	m_sliTimeAutoAdjustDLST.SetAlpha(m_fCurAlpha);
	m_sliTimeAutoAdjustDLST.Render();
#endif

#if (CVTE_EN_ANDROID_AUTO)
#if CVTE_EN_COMBINE_ANDROID_LINK
	if (m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType == DEVICE_TYPE_ANDROID_AUTO)
#endif
	{
	m_tagTitleAAP.SetAlpha(m_fCurAlpha);
	m_tagTitleAAP.Render();
	m_tagAAPAutoLaunch.SetAlpha(m_fCurAlpha);
	m_tagAAPAutoLaunch.Render();
	m_sliAAPAutoLaunch.SetAlpha(m_fCurAlpha);
	m_sliAAPAutoLaunch.Render();
	}
#endif

	//navigation:
	m_tagTitleNavi.SetAlpha(m_fCurAlpha);
	m_tagTitleNavi.Render();
	m_btnNaviPath.SetAlpha(m_fCurAlpha);
	m_btnNaviPath.Render();
	m_btnNaviSpeech.SetAlpha(m_fCurAlpha);
	m_btnNaviSpeech.Render();

	m_tagValueNaviPath.SetAlpha(m_fCurAlpha);
	m_tagValueNaviPath.Render();
	m_tagValueNaviSpeech.SetAlpha(m_fCurAlpha);
	m_tagValueNaviSpeech.Render();

	//language:
	m_tagTitleLanguage.SetAlpha(m_fCurAlpha);
	m_tagTitleLanguage.Render();
	m_btnLanguageSet.SetAlpha(m_fCurAlpha);
	m_btnLanguageSet.Render();

#if CVTE_EN_COMBINE_ANDROID_LINK
	//Android Type:
	m_tagTitleAndroidType.SetAlpha(m_fCurAlpha);
	m_tagTitleAndroidType.Render();
	m_btnAndroidTypeSet.SetAlpha(m_fCurAlpha);
	m_btnAndroidTypeSet.Render();
#endif

#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	//skin:
	m_tagTitleSkin.SetAlpha(m_fCurAlpha);
	m_tagTitleSkin.Render();
#if 0
	m_btnSkinPath.SetAlpha(m_fCurAlpha);
	m_btnSkinPath.Render();
	m_tagValueSkinPath.SetAlpha(m_fCurAlpha);
	m_tagValueSkinPath.Render();
#endif

	//background
	m_btnBackground.SetAlpha(m_fCurAlpha);
	m_btnBackground.Render();
	m_tagValueBackground.SetAlpha(m_fCurAlpha);
	m_tagValueBackground.Render();

	m_tagTitleBkLumin.SetAlpha(m_fCurAlpha);
	m_tagTitleBkLumin.Render();
	m_sliBkLumin.SetAlpha(m_fCurAlpha);
	m_sliBkLumin.Render();

#endif

//		m_tagTitleBlurEffect.SetAlpha(m_fCurAlpha);
//		m_tagTitleBlurEffect.Render();
//		m_sliBlurEffect.SetAlpha(m_fCurAlpha);
//		m_sliBlurEffect.Render();

#if !CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	//Advance settings
	m_tagTitleAdvance.SetAlpha(m_fCurAlpha);
	m_tagTitleAdvance.Render();
	m_btnAdvanceSettings.SetAlpha(m_fCurAlpha);
	m_btnAdvanceSettings.Render();

#if CVTE_EN_CDP_DCP_CONTROL
	m_tagTitleDCPControl.SetAlpha(m_fCurAlpha);
	m_tagTitleDCPControl.Render();
	m_sliDCPControl.SetAlpha(m_fCurAlpha);
	m_sliDCPControl.Render();
#endif

#endif

#if (CVTE_EN_GESTURE_AIR==1)
	//Gesture settings
	m_tagTitleGesture.SetAlpha(m_fCurAlpha);
	m_tagTitleGesture.Render();
	m_btnGestureSettings.SetAlpha(m_fCurAlpha);
	m_btnGestureSettings.Render();
#endif

#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO) || (CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
	m_tagTitleRearViewDelay.SetAlpha(m_fCurAlpha);
	m_tagTitleRearViewDelay.Render();
	m_btnRearViewDelay.SetAlpha(m_fCurAlpha);
	m_btnRearViewDelay.Render();
	m_tagValueRearViewDelay.SetAlpha(m_fCurAlpha);
	m_tagValueRearViewDelay.Render();

	m_tagTitlePowerOffDelay.SetAlpha(m_fCurAlpha);
	m_tagTitlePowerOffDelay.Render();
	m_tagPowerOffDelay.SetAlpha(m_fCurAlpha);
	m_tagPowerOffDelay.Render();
	m_sliPowerOffDelay.SetAlpha(m_fCurAlpha);
	m_sliPowerOffDelay.Render();
#endif

	//factory settings
	m_tagTitleFactory.SetAlpha(m_fCurAlpha);
	m_tagTitleFactory.Render();
	m_btnRestoreFactorySettings.SetAlpha(m_fCurAlpha);
	m_btnRestoreFactorySettings.Render();


// 	glDisable(GL_SCISSOR_TEST);

	EndRender();
}

BOOL CPanelSettingGeneral::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	const int box_size_x=LIST_BOX_ITEM_WIDTH;
	const int tag_h = LIST_BOX_TAG_HEIGHT;

	//large size:
	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	//
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};


	int margin_x=12;
	int margin_y = 6;
	int cx = g_iClientWidth-LIST_BOX_ITEM_WIDTH;
	int caption_h = 22;
	int caption2_h = 22;
	int value_Font = 20;
	int gridHeight = 80;
	int margin_left = 40;
	int space = 80;

	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
	{
        	caption_h = 20;
        	caption2_h = 16;
			value_Font = 14;
	}

	SLIDERSTRUCTURE sli;
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

	GLfloat py = g_iClientHeight/2-40;

	int captionMargin = 22;

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(cx, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);

	//Date&Time
	m_tagTitleTimeAndDate.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleTimeAndDate.SetCaption(GetResourceString(IDS_SETTING_DATE_AND_TIME));
	m_tagTitleTimeAndDate.SetSize(cx, gridHeight);
	m_tagTitleTimeAndDate.SetPos(0,py);
	m_tagTitleTimeAndDate.LinkTextGL(pTextGL);
	m_tagTitleTimeAndDate.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleTimeAndDate.SetCaptionColor(&cr_gray,&cr_gray);


	py-=space;
	//TimeZone
	m_btnTimeZone.Initialize(IDC_SETTING_BTN_SET_TIMEZONE,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTimeZone.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_btnTimeZone.SetSize(cx - margin_left, gridHeight);
	m_btnTimeZone.SetCaption(GetResourceString(IDS_SETTING_TIMEZONE));
	m_btnTimeZone.LinkTextGL(pTextGL);
	m_btnTimeZone.SetPos(margin_left/2, py);
	m_btnTimeZone.SetCaptionHeight(caption_h);
	m_btnTimeZone.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnTimeZone.SetCaptionColor(&cr_gray, &cr_gray);


	py-=space;
	//Date
	m_btnDate.Initialize(IDC_SETTING_BTN_SET_DATE,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDate.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_btnDate.SetSize(cx - margin_left, gridHeight);
	m_btnDate.SetCaption(GetResourceString(IDS_SETTING_DATE));
	m_btnDate.LinkTextGL(pTextGL);
	m_btnDate.SetPos(margin_left/2,py);
	m_btnDate.SetCaptionHeight(caption_h);
	m_btnDate.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnDate.SetCaptionColor(&cr_gray,&cr_gray);

	m_tagValueDate.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueDate.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_tagValueDate.SetSize(cx-margin_left, gridHeight);
	m_tagValueDate.SetCaption(_T("2014-11-03"));
	m_tagValueDate.LinkTextGL(pTextGL);
	m_tagValueDate.SetPos(margin_left/2, py);
	m_tagValueDate.SetCaptionHeight(value_Font);
	m_tagValueDate.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin+400);
	m_tagValueDate.SetCaptionColor(&cr_gray,&cr_gray);


	py-=space;
	//Time
	m_btnTime.Initialize(IDC_SETTING_BTN_SET_TIME,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTime.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_btnTime.SetSize(cx-margin_left, gridHeight);
	m_btnTime.SetCaption(GetResourceString(IDS_SETTING_TIME));
	m_btnTime.LinkTextGL(pTextGL);
	m_btnTime.SetPos(margin_left/2, py);
	m_btnTime.SetCaptionHeight(caption_h);
	m_btnTime.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnTime.SetCaptionColor(&cr_gray, &cr_gray);

	m_tagValueTime.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueTime.SetSize(cx - margin_left,gridHeight);
	m_tagValueTime.SetCaption(_T("12:09"));
	m_tagValueTime.LinkTextGL(pTextGL);
	m_tagValueTime.SetPos(margin_left/2,py);
	m_tagValueTime.SetCaptionHeight(value_Font);
	m_tagValueTime.SetCaptionPos(BTN_CAPTION_LEFT, margin_left/2 + 430);
	m_tagValueTime.SetCaptionColor(&cr_gray, &cr_gray);


	py-=space;
	//24Hr mode:
	m_tagTitleTime24HMode.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueTime.SetNotifyFunc(&CPanelSettingGeneral::OnNotify, this);
	m_tagTitleTime24HMode.SetSize(cx-margin_left, gridHeight);
	m_tagTitleTime24HMode.SetCaption(GetResourceString(IDS_SETTING_24H_MODE));
	m_tagTitleTime24HMode.LinkTextGL(pTextGL);
	m_tagTitleTime24HMode.SetPos(margin_left/2,py);
	m_tagTitleTime24HMode.SetCaptionHeight(caption_h);
	m_tagTitleTime24HMode.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleTime24HMode.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliTime24HMode.Initialize(IDC_SETTING_SLI_TIME_24HMODE,this,&sli,pSkinManager);
	m_sliTime24HMode.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_sliTime24HMode.SetRange(0,10);
	m_sliTime24HMode.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H?0:10);
	m_sliTime24HMode.SetPos(m_siCurSize.cx/2-70, py);


	py-=space;
	//Auto adjust:
	m_tagTitleTimeAutoAdjust.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleTimeAutoAdjust.SetSize(cx-margin_left, gridHeight);
	m_tagTitleTimeAutoAdjust.SetCaption(GetResourceString(IDS_SETTING_TIME_AUTO_ADJUST));
	m_tagTitleTimeAutoAdjust.LinkTextGL(pTextGL);
	m_tagTitleTimeAutoAdjust.SetPos(margin_left/2,py);
	m_tagTitleTimeAutoAdjust.SetCaptionHeight(caption_h);
	m_tagTitleTimeAutoAdjust.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleTimeAutoAdjust.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliTimeAutoAdjust.Initialize(IDC_SETTING_SLI_TIME_AUTOADJUST,this,&sli,pSkinManager);
	m_sliTimeAutoAdjust.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_sliTimeAutoAdjust.SetRange(0,10);
	m_sliTimeAutoAdjust.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bTimeAutoAdjust?10:0);
	m_sliTimeAutoAdjust.SetPos(m_siCurSize.cx/2-70, py);

	py-=tag_h;

	//Auto adjust DLST:
	m_tagTitleTimeAutoAdjustDLST.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleTimeAutoAdjustDLST.SetSize(cx-margin_left, gridHeight);
	m_tagTitleTimeAutoAdjustDLST.SetCaption(GetResourceString(IDS_SETTING_TIME_AUTO_ADJUST_DLST));
	m_tagTitleTimeAutoAdjustDLST.LinkTextGL(pTextGL);
	m_tagTitleTimeAutoAdjustDLST.SetPos(margin_left/2,py);

	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_SPANISH:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(caption_h-5);
		break;
	case LANGUAGE_RUSSIAN:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(17);
		break;
	case LANGUAGE_ITALIAN:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(caption_h-4);
		break;
	default:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(caption_h);
		break;
	}

	m_tagTitleTimeAutoAdjustDLST.SetCaptionPos(BTN_CAPTION_LEFT,captionMargin);
	m_tagTitleTimeAutoAdjustDLST.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliTimeAutoAdjustDLST.Initialize(IDC_SETTING_SLI_TIME_AUTOADJUST_DLST,this,&sli,pSkinManager);
	m_sliTimeAutoAdjustDLST.SetRange(0,10);
	m_sliTimeAutoAdjustDLST.SetValue(0);
	m_sliTimeAutoAdjustDLST.SetPos(m_siCurSize.cx/2-70,py);
	m_sliTimeAutoAdjustDLST.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

#if (CVTE_EN_ANDROID_AUTO)
#if CVTE_EN_COMBINE_ANDROID_LINK
	if (m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType == DEVICE_TYPE_ANDROID_AUTO)
#endif
	{
	py-=space;

	//AAP:
	m_tagTitleAAP.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleAAP.SetSize(cx, gridHeight);
	m_tagTitleAAP.SetCaption(_T("Android Auto"));
	m_tagTitleAAP.LinkTextGL(pTextGL);
	m_tagTitleAAP.SetPos(0,py);
	m_tagTitleAAP.SetCaptionHeight(caption_h);
	m_tagTitleAAP.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleAAP.SetCaptionColor(&cr_gray, &cr_gray);

	py-=space;

	m_tagAAPAutoLaunch.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagAAPAutoLaunch.SetSize(cx-margin_left, gridHeight);
	m_tagAAPAutoLaunch.SetCaption(_T("Auto Launch"));
	m_tagAAPAutoLaunch.LinkTextGL(pTextGL);
	m_tagAAPAutoLaunch.SetPos(margin_left/2,py);
	m_tagAAPAutoLaunch.SetCaptionHeight(caption_h);
	m_tagAAPAutoLaunch.SetCaptionPos(BTN_CAPTION_LEFT,captionMargin);
	m_tagAAPAutoLaunch.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliAAPAutoLaunch.Initialize(IDC_SETTING_SLI_AAP_AUTOLAUNCH,this,&sli,pSkinManager);
	m_sliAAPAutoLaunch.SetRange(0,10);
	m_sliAAPAutoLaunch.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bAAPAutoLaunch?10:0);
	m_sliAAPAutoLaunch.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAAPAutoLaunch.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	}
#endif

	py-=space;
	//navigation
	m_tagTitleNavi.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleNavi.SetSize(cx, gridHeight);
	m_tagTitleNavi.SetCaption(GetResourceString(IDS_SETTING_NAVIGATION));
	m_tagTitleNavi.LinkTextGL(pTextGL);
	m_tagTitleNavi.SetPos(0,py);
	m_tagTitleNavi.SetCaptionHeight(caption_h);
	m_tagTitleNavi.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleNavi.SetCaptionColor(&cr_gray, &cr_gray);


	py-=space;
	//Lanuch Location
	m_btnNaviPath.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNaviPath.SetSize(cx-margin_left,gridHeight);
	m_btnNaviPath.SetCaption(GetResourceString(IDS_SETTING_NAVI_LAUNCH_LOCATION));
	m_btnNaviPath.LinkTextGL(pTextGL);
	m_btnNaviPath.SetPos(margin_left/2,py);
	m_btnNaviPath.SetCaptionHeight(caption_h);
	m_btnNaviPath.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnNaviPath.SetCaptionColor(&cr_gray, &cr_gray);
	
	py-=space;
	m_tagValueNaviPath.Initialize(IDC_SETTING_BTN_NAVI_PATH,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueNaviPath.LinkTextGL(pTextGL);
	m_tagValueNaviPath.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_tagValueNaviPath.SetSize(cx-margin_left*1.5,gridHeight);
	m_tagValueNaviPath.SetCaption(m_pSkinManager->GetConfig()->GetCurParam()->strNaviPath.String(),TRUE);
	m_tagValueNaviPath.SetPos(margin_left*1.5/2,py);
	m_tagValueNaviPath.SetCaptionHeight(value_Font);
	m_tagValueNaviPath.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagValueNaviPath.SetCaptionColor(&cr_gray, &cr_gray);


	py-=space;
	//NaviSpeech
	EnableNaviSpeech(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS);
	m_btnNaviSpeech.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNaviSpeech.SetSize(cx-margin_left, gridHeight);
	m_btnNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH));
	m_btnNaviSpeech.LinkTextGL(pTextGL);
	m_btnNaviSpeech.SetPos(margin_left/2, py);
	m_btnNaviSpeech.SetCaptionHeight(caption_h);
	m_btnNaviSpeech.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnNaviSpeech.SetCaptionColor(&cr_gray, &cr_gray);
	
	py-=space;

	m_tagValueNaviSpeech.Initialize(IDC_SETTING_BTN_NAVI_SPEECH,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueNaviSpeech.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_tagValueNaviSpeech.SetSize(cx - margin_left*1.5, gridHeight);
	m_tagValueNaviSpeech.LinkTextGL(pTextGL);
	m_tagValueNaviSpeech.SetPos(margin_left*1.5/2, py);
	m_tagValueNaviSpeech.SetCaptionHeight(value_Font);
	switch (m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig)
	{
	case NAVI_SPEECH_ALWAYS:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_ALWAYS));
		break;
	case NAVI_SPEECH_OFF:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_CLOSED));
		break;
	case NAVI_SPEECH_WHILEONTOP:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_MAP));
		break;
	}
	m_tagValueNaviSpeech.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagValueNaviSpeech.SetCaptionColor(&cr_gray, &cr_gray);


	py-=space;
	//language:
	m_tagTitleLanguage.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleLanguage.SetNotifyFunc(&CPanelSettingGeneral::OnNotify, this);
	m_tagTitleLanguage.SetSize(cx,gridHeight);
	m_tagTitleLanguage.SetCaption(GetResourceString(IDS_SETTING_LANGUAGE));
	m_tagTitleLanguage.LinkTextGL(pTextGL);
	m_tagTitleLanguage.SetPos(0, py);
	m_tagTitleLanguage.SetCaptionHeight(caption_h);
	m_tagTitleLanguage.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleLanguage.SetCaptionColor(&cr_gray, &cr_gray);

	py-=space;

	m_btnLanguageSet.Initialize(IDC_SETTING_BTN_LANGUAGE_SEL,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLanguageSet.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_btnLanguageSet.SetSize(cx - margin_left,gridHeight);
	m_btnLanguageSet.SetCaption(g_strLanguage[m_pSkinManager->GetConfig()->GetCurParam()->idLanguage]);
	m_btnLanguageSet.LinkTextGL(pTextGL);
	m_btnLanguageSet.SetPos(margin_left/2,py);
	m_btnLanguageSet.SetCaptionHeight(caption_h);
	m_btnLanguageSet.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnLanguageSet.SetCaptionColor(&cr_gray, &cr_gray);

	py-=space;
	//Advance settings
	m_tagTitleAdvance.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleAdvance.SetNotifyFunc(&CPanelSettingGeneral::OnNotify, this);
	m_tagTitleAdvance.SetSize(cx,gridHeight);
	m_tagTitleAdvance.SetCaption(GetResourceString(IDS_SETTING_ADVANCE));
	m_tagTitleAdvance.LinkTextGL(pTextGL);
	m_tagTitleAdvance.SetPos(0,py);
	m_tagTitleAdvance.SetCaptionHeight(caption_h);
	m_tagTitleAdvance.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleAdvance.SetCaptionColor(&cr_gray, &cr_gray);

	py-=space;

	m_btnAdvanceSettings.Initialize(IDC_SETTING_BTN_ADVANCED_SETTINGS,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAdvanceSettings.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_btnAdvanceSettings.SetSize(cx - margin_left,gridHeight);
	m_btnAdvanceSettings.SetCaption(GetResourceString(IDS_SETTING_ADVANCED));
	m_btnAdvanceSettings.LinkTextGL(pTextGL);
	m_btnAdvanceSettings.SetPos(margin_left/2,py);
	m_btnAdvanceSettings.SetCaptionHeight(caption_h);
	m_btnAdvanceSettings.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnAdvanceSettings.SetCaptionColor(&cr_gray, &cr_gray);


	py-=space;
	//Factory settings
	m_tagTitleFactory.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleFactory.SetNotifyFunc(&CPanelSettingGeneral::OnNotify, this);
	m_tagTitleFactory.SetSize(cx,gridHeight);
	m_tagTitleFactory.SetCaption(GetResourceString(IDS_SETTING_UI_FACTORY));
	m_tagTitleFactory.LinkTextGL(pTextGL);
	m_tagTitleFactory.SetPos(0, py);
	m_tagTitleFactory.SetCaptionHeight(caption_h);
	m_tagTitleFactory.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleFactory.SetCaptionColor(&cr_gray, &cr_gray);
	
	py-=space;

	m_btnRestoreFactorySettings.Initialize(IDC_SETTING_BTN_FACTORY,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRestoreFactorySettings.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_btnRestoreFactorySettings.SetSize(cx - margin_left,gridHeight);
	m_btnRestoreFactorySettings.SetCaption(GetResourceString(IDS_SETTING_UI_RESTORE_FACTORY_SETTINGS));
	m_btnRestoreFactorySettings.LinkTextGL(pTextGL);
	m_btnRestoreFactorySettings.SetPos(margin_left/2,py);
	
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_GREEK:
		m_btnRestoreFactorySettings.SetCaptionHeight(caption2_h);
		break;
	default:
		m_btnRestoreFactorySettings.SetCaptionHeight(caption_h);
		break;
	}

	m_btnRestoreFactorySettings.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnRestoreFactorySettings.SetCaptionColor(&cr_gray, &cr_gray);

	if(m_pStatusBar)
	{
		m_pStatusBar->SetTimeMode(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode);
	}

		
	int iPanelHeight = g_iClientHeight/2 - py + tag_h;

	if(iPanelHeight < g_iClientHeight)
		iPanelHeight = g_iClientHeight;

	SetSize(g_iClientWidth-box_size_x,iPanelHeight);

#else //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	const int box_size_x=LIST_BOX_ITEM_WIDTH;
	const int tag_h = LIST_BOX_TAG_HEIGHT;

	//large size:
	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	//
	COLORGL cr_w={1.0,1.0,1.0,1.0};
 	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};


	int margin_x=12;
	int cx=(g_iClientWidth-box_size_x-margin_x*2)/2;
	int caption_h = 22;
	int caption2_h = 20;
	int caption3_h = 16;
	//int cy=cx;
	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
	{
        	caption_h = 20;
        	caption2_h = 16;
		caption3_h = 14;
	}

	SLIDERSTRUCTURE sli;
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

	GLfloat py=g_iClientHeight/2-40;

//		//key beep:
//		m_tagTitleKeyBeep.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagTitleKeyBeep.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
//		m_tagTitleKeyBeep.SetCaption(GetResourceString(IDS_SETTING_KEY_BEEP));
//		m_tagTitleKeyBeep.LinkTextGL(pTextGL);
//		m_tagTitleKeyBeep.SetPos(0,py);
//		m_tagTitleKeyBeep.SetCaptionHeight(caption_h);
//		m_tagTitleKeyBeep.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
//		m_tagTitleKeyBeep.SetCaptionColor(&cr_gray);
//
//		m_sliKeyBeep.Initialize(IDC_SETTING_SLI_KEY_BEEP,this,&sli,pSkinManager);
//		m_sliKeyBeep.SetRange(0,10);
//		m_sliKeyBeep.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep?10:0);
//		m_sliKeyBeep.SetPos(m_siCurSize.cx/2-70,py);
//		m_sliKeyBeep.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
//
//		py-=tag_h;

//		//brake test:
//		m_tagTitleBrakeTest.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagTitleBrakeTest.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
//		m_tagTitleBrakeTest.SetCaption(GetResourceString(IDS_SETTING_HANDBRAKE_TEST));
//		m_tagTitleBrakeTest.LinkTextGL(pTextGL);
//		m_tagTitleBrakeTest.SetPos(0,py);
//		m_tagTitleBrakeTest.SetCaptionHeight(caption_h);
//		m_tagTitleBrakeTest.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
//		m_tagTitleBrakeTest.SetCaptionColor(&cr_gray);
//
//		m_sliBrakeTest.Initialize(IDC_SETTING_SLI_BREAK_TEST,this,&sli,pSkinManager);
//		m_sliBrakeTest.SetRange(0,10);
//		m_sliBrakeTest.SetValue(0);
//		m_sliBrakeTest.SetPos(m_siCurSize.cx/2-70,py);
//		m_sliBrakeTest.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
//
//		py-=tag_h;
/*
	//reverse polar:
	m_tagTitleReversePolar.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleReversePolar.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleReversePolar.SetCaption(GetResourceString(IDS_SETTING_REVERSE_POLAR));
	m_tagTitleReversePolar.LinkTextGL(pTextGL);
	m_tagTitleReversePolar.SetPos(0,py);
	m_tagTitleReversePolar.SetCaptionHeight(caption_h);
	m_tagTitleReversePolar.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleReversePolar.SetCaptionColor(&cr_gray);

	m_sliReversePolar.Initialize(IDC_SETTING_SLI_REVERSE_LEVEL,this,&sli,pSkinManager);
	m_sliReversePolar.SetRange(0,10);
	m_sliReversePolar.SetValue(0);
	m_sliReversePolar.SetPos(m_siCurSize.cx/2-70,py);
	m_sliReversePolar.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	py-=tag_h;
*/

#if (CVTE_EN_STEER_WHEEL_OLD_UI_STYLE && CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_BOSSAUDIOUI)
	py-=10;

	//brake test:
	m_tagTitleBrakeTest.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleBrakeTest.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleBrakeTest.SetCaption(GetResourceString(IDS_SETTING_HANDBRAKE_TEST));
	m_tagTitleBrakeTest.LinkTextGL(pTextGL);
	m_tagTitleBrakeTest.SetPos(0,py);
	m_tagTitleBrakeTest.SetCaptionHeight(caption_h);
	m_tagTitleBrakeTest.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleBrakeTest.SetCaptionColor(&cr_gray);

	m_sliBrakeTest.Initialize(IDC_SETTING_SLI_BREAK_TEST,this,&sli,pSkinManager);
	m_sliBrakeTest.SetRange(0,10);
	m_sliBrakeTest.SetValue(0);
	m_sliBrakeTest.SetPos(m_siCurSize.cx/2-70,py);
	m_sliBrakeTest.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	py-=tag_h;
	//lamp control:
	m_tagTitleLampControl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLampControl.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLampControl.SetCaption(GetResourceString(IDS_SETTING_LAMP_CONTROL));
	m_tagTitleLampControl.LinkTextGL(pTextGL);
	m_tagTitleLampControl.SetPos(0,py);
	m_tagTitleLampControl.SetCaptionHeight(caption_h);
	m_tagTitleLampControl.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleLampControl.SetCaptionColor(&cr_gray);

	m_sliLampControl.Initialize(IDC_SETTING_SLI_LAMP_CONTROL,this,&sli,pSkinManager);
	m_sliLampControl.SetRange(0,10);
	m_sliLampControl.SetValue(0);
	m_sliLampControl.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLampControl.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	py-=tag_h;

	//drive type
	m_btnDriveType.Initialize(IDC_SETTING_BTN_SET_DRIVE_TYPE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDriveType.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_TYPE));
	m_btnDriveType.LinkTextGL(pTextGL);
	m_btnDriveType.SetPos(0,py);
	m_btnDriveType.SetCaptionHeight(caption_h);
	m_btnDriveType.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnDriveType.SetCaptionColor(&cr_gray);
	m_btnDriveType.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValueDriveType.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueDriveType.SetSize(cx,40);
	switch (m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType)
	{
	case 0://left
		m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_LEFT));
		break;
	case 1://right
		m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_RIGHT));
		break;
	}
	m_tagValueDriveType.LinkTextGL(pTextGL);
	m_tagValueDriveType.SetPos(cx/2-10,py);
	m_tagValueDriveType.SetCaptionHeight(caption_h);
	m_tagValueDriveType.EnableDrawBack(FALSE);
	m_tagValueDriveType.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueDriveType.SetCaptionColor(&cr_gray);

#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
	//Update Driver Type Info to MCU
	if (m_pCommander)
	{
		BYTE buf = m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType;
		m_pCommander->SendExtenCmd(_ARM_EXTEN_TAPE_SIDE,&buf,1);
	}	
#endif
	py-=70;
#endif

#if 0	//(CUSTOMER_ID != CUSTOMER_ENTERFRONT)
	py-=tag_h;

    m_btnRearColorSignalType.Initialize(IDC_SETTING_BTN_COLORSIGNAL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRearColorSignalType.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnRearColorSignalType.SetCaption(GetResourceString(IDS_SETTING_CAMERA_TYPE));
	m_btnRearColorSignalType.LinkTextGL(pTextGL);
	m_btnRearColorSignalType.SetPos(0,py);
	m_btnRearColorSignalType.SetCaptionHeight(caption_h);
	m_btnRearColorSignalType.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnRearColorSignalType.SetCaptionColor(&cr_gray);
	m_btnRearColorSignalType.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagRearColorSignalType.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRearColorSignalType.SetSize(cx,40);
	switch (m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard)
	{
	case OEMCONFIG_COLORSIGNAL_NTSM_MJ:
    case OEMCONFIG_COLORSIGNAL_NTSC_443:
		m_tagRearColorSignalType.SetCaption(_T("NTSC"));
		break;
	case OEMCONFIG_COLORSIGNAL_PAL_M:
    case OEMCONFIG_COLORSIGNAL_PAL_60:
    case OEMCONFIG_COLORSIGNAL_PAL_BGHID:
    case OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N:
		m_tagRearColorSignalType.SetCaption(_T("PAL"));
		break;
    default:
		m_tagRearColorSignalType.SetCaption(_T("NTSC"));
		break;
    }

	m_tagRearColorSignalType.LinkTextGL(pTextGL);
	m_tagRearColorSignalType.SetPos(cx/2-10,py);
	m_tagRearColorSignalType.SetCaptionHeight(caption_h);
	m_tagRearColorSignalType.EnableDrawBack(FALSE);
	m_tagRearColorSignalType.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagRearColorSignalType.SetCaptionColor(&cr_gray);
#endif

//		//
//		py-=70;

	//time and date:
	m_tagTitleTimeAndDate.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleTimeAndDate.SetSize(cx,40);
	m_tagTitleTimeAndDate.SetCaption(GetResourceString(IDS_SETTING_DATE_AND_TIME));
	m_tagTitleTimeAndDate.LinkTextGL(pTextGL);
	m_tagTitleTimeAndDate.SetPos(-cx/2-10 +12,py);
	m_tagTitleTimeAndDate.SetCaptionHeight(caption2_h);
	m_tagTitleTimeAndDate.EnableDrawBack(FALSE);
	m_tagTitleTimeAndDate.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleTimeAndDate.SetCaptionColor(&cr_dark);

	py-=60;

	m_btnTimeZone.Initialize(IDC_SETTING_BTN_SET_TIMEZONE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTimeZone.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnTimeZone.SetCaption(GetResourceString(IDS_SETTING_TIMEZONE));
	m_btnTimeZone.LinkTextGL(pTextGL);
	m_btnTimeZone.SetPos(0,py);
	m_btnTimeZone.SetCaptionHeight(caption_h);
	m_btnTimeZone.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnTimeZone.SetCaptionColor(&cr_gray);
	m_btnTimeZone.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	py-=tag_h;

	m_btnDate.Initialize(IDC_SETTING_BTN_SET_DATE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDate.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnDate.SetCaption(GetResourceString(IDS_SETTING_DATE));
	m_btnDate.LinkTextGL(pTextGL);
	m_btnDate.SetPos(0,py);
	m_btnDate.SetCaptionHeight(caption_h);
	m_btnDate.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnDate.SetCaptionColor(&cr_gray);
	m_btnDate.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValueDate.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueDate.SetSize(cx,40);
	m_tagValueDate.SetCaption(_T("2014-11-03"));
	m_tagValueDate.LinkTextGL(pTextGL);
	m_tagValueDate.SetPos(cx/2+10 -20,py);
	m_tagValueDate.SetCaptionHeight(caption_h);
	m_tagValueDate.EnableDrawBack(FALSE);
	m_tagValueDate.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueDate.SetCaptionColor(&cr_gray);

	py-=tag_h;

	m_btnTime.Initialize(IDC_SETTING_BTN_SET_TIME,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTime.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnTime.SetCaption(GetResourceString(IDS_SETTING_TIME));
	m_btnTime.LinkTextGL(pTextGL);
	m_btnTime.SetPos(0,py);
	m_btnTime.SetCaptionHeight(caption_h);
	m_btnTime.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnTime.SetCaptionColor(&cr_gray);
	m_btnTime.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValueTime.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueTime.SetSize(cx,40);
	m_tagValueTime.SetCaption(_T("12:09"));
	m_tagValueTime.LinkTextGL(pTextGL);
	m_tagValueTime.SetPos(cx/2-10,py);
	m_tagValueTime.SetCaptionHeight(caption_h);
	m_tagValueTime.EnableDrawBack(FALSE);
	m_tagValueTime.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueTime.SetCaptionColor(&cr_gray);

	py-=tag_h;

	//24h mode:
	m_tagTitleTime24HMode.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleTime24HMode.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleTime24HMode.SetCaption(GetResourceString(IDS_SETTING_24H_MODE));
	m_tagTitleTime24HMode.LinkTextGL(pTextGL);
	m_tagTitleTime24HMode.SetPos(0,py);
	m_tagTitleTime24HMode.SetCaptionHeight(caption_h);
	m_tagTitleTime24HMode.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleTime24HMode.SetCaptionColor(&cr_gray);

	m_sliTime24HMode.Initialize(IDC_SETTING_SLI_TIME_24HMODE,this,&sli,pSkinManager);
	m_sliTime24HMode.SetRange(0,10);
	m_sliTime24HMode.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H?0:10);
	m_sliTime24HMode.SetPos(m_siCurSize.cx/2-70,py);
	m_sliTime24HMode.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

#if CVTE_EN_NAVI
	py-=tag_h;

	//Auto adjust:
	m_tagTitleTimeAutoAdjust.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleTimeAutoAdjust.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleTimeAutoAdjust.SetCaption(GetResourceString(IDS_SETTING_TIME_AUTO_ADJUST));
	m_tagTitleTimeAutoAdjust.LinkTextGL(pTextGL);
	m_tagTitleTimeAutoAdjust.SetPos(0,py);
	m_tagTitleTimeAutoAdjust.SetCaptionHeight(caption_h);
	m_tagTitleTimeAutoAdjust.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleTimeAutoAdjust.SetCaptionColor(&cr_gray);

	m_sliTimeAutoAdjust.Initialize(IDC_SETTING_SLI_TIME_AUTOADJUST,this,&sli,pSkinManager);
	m_sliTimeAutoAdjust.SetRange(0,10);
	m_sliTimeAutoAdjust.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bTimeAutoAdjust?10:0);
	m_sliTimeAutoAdjust.SetPos(m_siCurSize.cx/2-70,py);
	m_sliTimeAutoAdjust.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
#endif

#if (!CVTE_EN_STEER_WHEEL_OLD_UI_STYLE || (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI))
	py-=tag_h;

	//Auto adjust DLST:
	m_tagTitleTimeAutoAdjustDLST.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleTimeAutoAdjustDLST.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleTimeAutoAdjustDLST.SetCaption(GetResourceString(IDS_SETTING_TIME_AUTO_ADJUST_DLST));
	m_tagTitleTimeAutoAdjustDLST.LinkTextGL(pTextGL);
	m_tagTitleTimeAutoAdjustDLST.SetPos(0,py);

	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_SPANISH:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(caption2_h-3);
		break;
	case LANGUAGE_RUSSIAN:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(17);
		break;
	case LANGUAGE_ITALIAN:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(caption2_h-2);
		break;
	default:
		m_tagTitleTimeAutoAdjustDLST.SetCaptionHeight(caption_h);
		break;
	}

	m_tagTitleTimeAutoAdjustDLST.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleTimeAutoAdjustDLST.SetCaptionColor(&cr_gray);

	m_sliTimeAutoAdjustDLST.Initialize(IDC_SETTING_SLI_TIME_AUTOADJUST_DLST,this,&sli,pSkinManager);
	m_sliTimeAutoAdjustDLST.SetRange(0,10);
	m_sliTimeAutoAdjustDLST.SetValue(0);
	m_sliTimeAutoAdjustDLST.SetPos(m_siCurSize.cx/2-70,py);
	m_sliTimeAutoAdjustDLST.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
#endif

#if (CVTE_EN_ANDROID_AUTO)
#if CVTE_EN_COMBINE_ANDROID_LINK
	if (m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType == DEVICE_TYPE_ANDROID_AUTO)
#endif
	{
	py-=70;

	//AAP:
	m_tagTitleAAP.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAAP.SetSize(cx,40);
	m_tagTitleAAP.SetCaption(_T("Android Auto"));
	m_tagTitleAAP.LinkTextGL(pTextGL);
	m_tagTitleAAP.SetPos(-cx/2-10 +12,py);
	m_tagTitleAAP.SetCaptionHeight(caption2_h);
	m_tagTitleAAP.EnableDrawBack(FALSE);
	m_tagTitleAAP.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleAAP.SetCaptionColor(&cr_dark);

	py-=60;

	m_tagAAPAutoLaunch.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAAPAutoLaunch.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagAAPAutoLaunch.SetCaption(GetResourceString(IDS_AUTO_LAUNCH));
	m_tagAAPAutoLaunch.LinkTextGL(pTextGL);
	m_tagAAPAutoLaunch.SetPos(0,py);
	m_tagAAPAutoLaunch.SetCaptionHeight(caption_h);
	m_tagAAPAutoLaunch.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagAAPAutoLaunch.SetCaptionColor(&cr_gray);

	m_sliAAPAutoLaunch.Initialize(IDC_SETTING_SLI_AAP_AUTOLAUNCH,this,&sli,pSkinManager);
	m_sliAAPAutoLaunch.SetRange(0,10);
	m_sliAAPAutoLaunch.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bAAPAutoLaunch?10:0);
	m_sliAAPAutoLaunch.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAAPAutoLaunch.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	}
#endif

	py-=70;

#if CVTE_EN_NAVI
	//navigation:
	m_tagTitleNavi.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleNavi.SetSize(cx,40);
	m_tagTitleNavi.SetCaption(GetResourceString(IDS_SETTING_NAVIGATION));
	m_tagTitleNavi.LinkTextGL(pTextGL);
	m_tagTitleNavi.SetPos(-cx/2-10 +12,py);
	m_tagTitleNavi.SetCaptionHeight(caption2_h);
	m_tagTitleNavi.EnableDrawBack(FALSE);
	m_tagTitleNavi.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleNavi.SetCaptionColor(&cr_dark);

	py-=(60+10);

	m_btnNaviPath.Initialize(IDC_SETTING_BTN_NAVI_PATH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNaviPath.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h+caption2_h);
	m_btnNaviPath.SetCaption(GetResourceString(IDS_SETTING_NAVI_LAUNCH_LOCATION));
	m_btnNaviPath.LinkTextGL(pTextGL);
	m_btnNaviPath.SetPos(0,py);
	m_btnNaviPath.SetCaptionHeight(caption_h);
	m_btnNaviPath.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h,caption2_h);
	m_btnNaviPath.SetCaptionColor(&cr_gray);
	m_btnNaviPath.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValueNaviPath.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueNaviPath.LinkTextGL(pTextGL);
// 	m_tagValueNaviPath.SetSize(cx,40);
// 	m_tagValueNaviPath.SetPos(cx/2+10 -20,py);
	m_tagValueNaviPath.SetSize((g_iClientWidth-box_size_x-margin_x*2),40);
	m_tagValueNaviPath.SetPos(0,py-caption2_h);
	m_tagValueNaviPath.SetCaptionHeight(caption3_h);
	m_tagValueNaviPath.SetCaption(m_pSkinManager->GetConfig()->GetCurParam()->strNaviPath.String(),TRUE);
	m_tagValueNaviPath.EnableDrawBack(FALSE);
	m_tagValueNaviPath.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagValueNaviPath.SetCaptionColor(&cr_gray);

	py-=(tag_h+caption2_h);

	EnableNaviSpeech(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS);

	m_btnNaviSpeech.Initialize(IDC_SETTING_BTN_NAVI_SPEECH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNaviSpeech.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h+caption2_h);
	m_btnNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH));
	m_btnNaviSpeech.LinkTextGL(pTextGL);
	m_btnNaviSpeech.SetPos(0,py);
	m_btnNaviSpeech.SetCaptionHeight(caption_h);
	m_btnNaviSpeech.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h,caption2_h);
	m_btnNaviSpeech.SetCaptionColor(&cr_gray);
	m_btnNaviSpeech.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValueNaviSpeech.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueNaviSpeech.SetSize((g_iClientWidth-box_size_x-margin_x*2),40);
	m_tagValueNaviSpeech.LinkTextGL(pTextGL);
	m_tagValueNaviSpeech.SetPos(0,py-caption2_h);
	m_tagValueNaviSpeech.SetCaptionHeight(caption2_h);
	switch (m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig)
	{
	case NAVI_SPEECH_ALWAYS:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_ALWAYS));
		break;
	case NAVI_SPEECH_OFF:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_CLOSED));
		break;
	case NAVI_SPEECH_WHILEONTOP:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_MAP));
		break;
	}
	m_tagValueNaviSpeech.EnableDrawBack(FALSE);
	m_tagValueNaviSpeech.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagValueNaviSpeech.SetCaptionColor(&cr_gray);


	py-=(70+10);
#else
	EnableNaviSpeech(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS);
#endif
	//language:

	m_tagTitleLanguage.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLanguage.SetSize(cx,40);
	m_tagTitleLanguage.SetCaption(GetResourceString(IDS_SETTING_LANGUAGE));
	m_tagTitleLanguage.LinkTextGL(pTextGL);
	m_tagTitleLanguage.SetPos(-cx/2-10 +12,py);
	m_tagTitleLanguage.SetCaptionHeight(caption2_h);
	m_tagTitleLanguage.EnableDrawBack(FALSE);
	m_tagTitleLanguage.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleLanguage.SetCaptionColor(&cr_dark);

	py-=60;

	m_btnLanguageSet.Initialize(IDC_SETTING_BTN_LANGUAGE_SEL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnLanguageSet.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnLanguageSet.SetCaption(g_strLanguage[m_pSkinManager->GetConfig()->GetCurParam()->idLanguage]);
	m_btnLanguageSet.LinkTextGL(pTextGL);
	m_btnLanguageSet.SetPos(0,py);
	m_btnLanguageSet.SetCaptionHeight(caption_h);
	m_btnLanguageSet.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnLanguageSet.SetCaptionColor(&cr_gray);
	m_btnLanguageSet.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

#if CVTE_EN_COMBINE_ANDROID_LINK
	py-=70;
	//Android Type:

	m_tagTitleAndroidType.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAndroidType.SetSize(cx,40);
	m_tagTitleAndroidType.SetCaption(GetResourceString(IDS_SETTING_ANDROID_TYPE));
	m_tagTitleAndroidType.LinkTextGL(pTextGL);
	m_tagTitleAndroidType.SetPos(-cx/2-10 +12,py);
	m_tagTitleAndroidType.SetCaptionHeight(caption2_h);
	m_tagTitleAndroidType.EnableDrawBack(FALSE);
	m_tagTitleAndroidType.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleAndroidType.SetCaptionColor(&cr_dark);

	py-=60;

	m_btnAndroidTypeSet.Initialize(IDC_SETTING_BTN_ANDROIDTYPE_SEL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAndroidTypeSet.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\AndroidAccessory"));
	m_btnAndroidTypeSet.SetCaption((reg.ValueDW(_T("SwitchDeviceType")) == DEVICE_TYPE_WEBLINK)?_T("WebLink"):_T("AndroidAuto"));
	m_btnAndroidTypeSet.LinkTextGL(pTextGL);
	m_btnAndroidTypeSet.SetPos(0,py);
	m_btnAndroidTypeSet.SetCaptionHeight(caption_h);
	m_btnAndroidTypeSet.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnAndroidTypeSet.SetCaptionColor(&cr_gray);
	m_btnAndroidTypeSet.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
#endif

	py-=70;

	//skin:
	m_tagTitleSkin.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleSkin.SetSize(cx,40);
	m_tagTitleSkin.SetCaption(GetResourceString(IDS_SETTING_UI));
	m_tagTitleSkin.LinkTextGL(pTextGL);
	m_tagTitleSkin.SetPos(-cx/2-10+12,py);
	m_tagTitleSkin.SetCaptionHeight(caption2_h);
	m_tagTitleSkin.EnableDrawBack(FALSE);
	m_tagTitleSkin.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleSkin.SetCaptionColor(&cr_dark);

#if 0//(CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_BOSSAUDIOUI)
	py-=60;
	{
	m_btnSkinPath.Initialize(IDC_SETTING_BTN_SKIN_PATH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	int w=g_iClientWidth-box_size_x-margin_x*2;
	int h=tag_h;
	m_btnSkinPath.SetSize(w,h);
	m_btnSkinPath.SetCaption(GetResourceString(IDS_SETTING_UI_SKIN));
	m_btnSkinPath.LinkTextGL(pTextGL);
	m_btnSkinPath.SetPos(-(g_iClientWidth-box_size_x-margin_x*2)/2+w/2,py);
	m_btnSkinPath.SetCaptionHeight(caption_h);
	m_btnSkinPath.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnSkinPath.SetCaptionColor(&cr_gray);
	m_btnSkinPath.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	}

	m_tagValueSkinPath.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueSkinPath.LinkTextGL(pTextGL);
	m_tagValueSkinPath.SetSize(cx,40);
	m_tagValueSkinPath.SetPos(cx/2+10-20,py);
	m_tagValueSkinPath.SetCaptionHeight(caption_h);
// 	m_tagValueSkinPath.SetCaption(m_pSkinManager->GetConfig()->GetCurParam()->strSkinFileName.String(),TRUE);
	m_tagValueSkinPath.EnableDrawBack(FALSE);
	m_tagValueSkinPath.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueSkinPath.SetCaptionColor(&cr_gray);

	CM_String strSkinName;
	m_pSkinManager->GetSkinName(m_pSkinManager->GetConfig()->GetCurParam()->strSkinFileName.String(),&strSkinName);
	m_tagValueSkinPath.SetCaption(strSkinName.String(),TRUE);
#endif


	py-=60;

	//Background
	{
	m_btnBackground.Initialize(IDC_SETTING_BTN_BACKGROUND_PATH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	int w=g_iClientWidth-box_size_x-margin_x*2;
	int h=tag_h;
	m_btnBackground.SetSize(w,h);
	m_btnBackground.SetCaption(GetResourceString(IDS_SETTING_UI_BACKGROUND));
	m_btnBackground.LinkTextGL(pTextGL);
	m_btnBackground.SetPos(-(g_iClientWidth-box_size_x-margin_x*2)/2+w/2,py);
	m_btnBackground.SetCaptionHeight(caption_h);
	m_btnBackground.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnBackground.SetCaptionColor(&cr_gray);
	m_btnBackground.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	}

	m_tagValueBackground.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueBackground.LinkTextGL(pTextGL);
	m_tagValueBackground.SetSize(cx,40);
	m_tagValueBackground.SetPos(cx/2+10-20,py);
	m_tagValueBackground.SetCaptionHeight(caption_h);
	m_tagValueBackground.EnableDrawBack(FALSE);
	m_tagValueBackground.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueBackground.SetCaptionColor(&cr_gray);

	wchar_t name[MAX_PATH];
	if(GetFileNameFromPath(m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName.String(),name,MAX_PATH))
	{
#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
		if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
		{
			if(wcsstr(name, _T("User")))
				m_tagValueBackground.SetCaption(_T("Мои"),TRUE);
			else
				m_tagValueBackground.SetCaption(_T("По умолчанию"),TRUE);
		}
		else
		{
			m_tagValueBackground.SetCaption(name,TRUE);
		}
#else
		m_tagValueBackground.SetCaption(name,TRUE);
#endif
	}

	py-=tag_h;

//		memset(&sli,0,sizeof(sli));
//		sli.size.cx=SLI_BOOL_BUTTON_WIDTH;
//		sli.size.cy=SLI_BOOL_BUTTON_HEIGHT;
//		sli.bHasBtns=FALSE;
//		sli.bFilled=FALSE;
//		sli.bFilledBoth=FALSE;
//		sli.idBackTex=TEXID_642;
//		sli.idChannelTex=TEXID_641;
//		sli.idThumbTex=TEXID_640;
//		sli.bSwitchMode=TRUE;
//
//		//
//		m_tagTitleBlurEffect.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagTitleBlurEffect.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
//		m_tagTitleBlurEffect.SetCaption(GetResourceString(IDS_SETTING_BLUR_EFFECT));
//		m_tagTitleBlurEffect.LinkTextGL(pTextGL);
//		m_tagTitleBlurEffect.SetPos(0,py);
//		m_tagTitleBlurEffect.SetCaptionHeight(caption_h);
//		m_tagTitleBlurEffect.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
//		m_tagTitleBlurEffect.SetCaptionColor(&cr_gray);
//
//		m_sliBlurEffect.Initialize(IDC_SETTING_SLI_BLUR_EFFECT,this,&sli,pSkinManager);
//		m_sliBlurEffect.SetRange(0,10);
//		m_sliBlurEffect.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect?10:0);
//		m_sliBlurEffect.SetPos(m_siCurSize.cx/2-70,py);
//		m_sliBlurEffect.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
//		m_sliBlurEffect.LinkTextGL(pTextGL);
//		m_sliBlurEffect.SetCaptionColor(&cr_gray);
//		m_sliBlurEffect.ShowValue(FALSE);
//
//		py-=tag_h;

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	//
	m_tagTitleBkLumin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleBkLumin.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleBkLumin.SetCaption(GetResourceString(IDS_SETTING_UI_BACK_LUMIN));
	m_tagTitleBkLumin.LinkTextGL(pTextGL);
	m_tagTitleBkLumin.SetPos(0,py);
	m_tagTitleBkLumin.SetCaptionHeight(caption_h);
	m_tagTitleBkLumin.SetCaptionPos(BTN_CAPTION_LEFT,caption_h);
	m_tagTitleBkLumin.SetCaptionColor(&cr_gray);

	m_sliBkLumin.Initialize(IDC_SETTING_SLI_BACK_LUMIN,this,&sli,pSkinManager);
	m_sliBkLumin.SetRange(0,10);
	m_sliBkLumin.SetValue(INT(m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance*10));
	m_sliBkLumin.SetPos(m_siCurSize.cx/2-144,py);
	m_sliBkLumin.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);
	m_sliBkLumin.LinkTextGL(pTextGL);
	m_sliBkLumin.SetCaptionColor(&cr_gray);
	m_sliBkLumin.ShowValue(TRUE);
	m_sliBkLumin.EnableMoveNotify(TRUE);
	m_sliBkLumin.EnbaleClickOnlyThumb(TRUE);

/*
	py-=tag_h;

	//idle screen
	m_btnIdleScreen.Initialize(IDC_SETTING_BTN_SET_IDLE_SCREEN,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnIdleScreen.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnIdleScreen.SetCaption(GetResourceString(IDS_SETTING_IDLE_SCREEN));
	m_btnIdleScreen.LinkTextGL(pTextGL);
	m_btnIdleScreen.SetPos(0,py);
	m_btnIdleScreen.SetCaptionHeight(caption_h);
	m_btnIdleScreen.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnIdleScreen.SetCaptionColor(&cr_gray);
	m_btnIdleScreen.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValueIdleScreen.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueIdleScreen.SetSize(cx,40);

    m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_NONE));
*/

//		switch (m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen)
//		{
//		case 0:
//			m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_NONE));
//			break;
//		case 1:
//			m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_CLOCK_ANALOG));
//			break;
//		case 2:
//			m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_CLOCK_DIGITAL));
//			break;
//		}
/*
	m_tagValueIdleScreen.LinkTextGL(pTextGL);
	m_tagValueIdleScreen.SetPos(cx/2+10 -20,py);
	m_tagValueIdleScreen.SetCaptionHeight(caption_h);
	m_tagValueIdleScreen.EnableDrawBack(FALSE);
	m_tagValueIdleScreen.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueIdleScreen.SetCaptionColor(&cr_gray);

*/
	py-=70;

#if !CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	//Advance settings
	m_tagTitleAdvance.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAdvance.SetSize(cx,40);
	m_tagTitleAdvance.SetCaption(GetResourceString(IDS_SETTING_ADVANCE));
	m_tagTitleAdvance.LinkTextGL(pTextGL);
	m_tagTitleAdvance.SetPos(-cx/2-10 +12,py);
	m_tagTitleAdvance.SetCaptionHeight(caption2_h);
	m_tagTitleAdvance.EnableDrawBack(FALSE);
	m_tagTitleAdvance.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleAdvance.SetCaptionColor(&cr_dark);

	py-=60;

	m_btnAdvanceSettings.Initialize(IDC_SETTING_BTN_ADVANCED_SETTINGS,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAdvanceSettings.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnAdvanceSettings.SetCaption(GetResourceString(IDS_SETTING_ADVANCED));
	m_btnAdvanceSettings.LinkTextGL(pTextGL);
	m_btnAdvanceSettings.SetPos(0,py);
	m_btnAdvanceSettings.SetCaptionHeight(caption_h);
	m_btnAdvanceSettings.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnAdvanceSettings.SetCaptionColor(&cr_gray);
	m_btnAdvanceSettings.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

#if CVTE_EN_CDP_DCP_CONTROL
	//DCP/CDP Charging control
	py-=tag_h;

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

	m_tagTitleDCPControl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleDCPControl.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleDCPControl.SetCaption(GetResourceString(IDS_SETTING_CHARGING_ONLY));
	m_tagTitleDCPControl.LinkTextGL(pTextGL);
	m_tagTitleDCPControl.SetPos(0,py);
	m_tagTitleDCPControl.SetCaptionHeight(caption_h);
	m_tagTitleDCPControl.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleDCPControl.SetCaptionColor(&cr_gray);

	m_sliDCPControl.Initialize(IDC_SETTING_SLI_DCP_CONTROL,this,&sli,pSkinManager);
	m_sliDCPControl.SetRange(0,10);
	m_sliDCPControl.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDCPControl?10:0);
	m_sliDCPControl.SetPos(m_siCurSize.cx/2-70,py);
	m_sliDCPControl.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	SwitchDCP(m_pSkinManager->GetConfig()->GetCurParam()->bDCPControl);
#endif

	py-=70;
#endif

#if (CVTE_EN_GESTURE_AIR==1)

	m_tagTitleGesture.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleGesture.SetSize(cx,40);
	m_tagTitleGesture.SetCaption(_T("Gesture"));
	m_tagTitleGesture.LinkTextGL(pTextGL);
	m_tagTitleGesture.SetPos(-cx/2-10 +12,py);
	m_tagTitleGesture.SetCaptionHeight(caption2_h);
	m_tagTitleGesture.EnableDrawBack(FALSE);
	m_tagTitleGesture.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleGesture.SetCaptionColor(&cr_dark);

	py-=60;

	m_btnGestureSettings.Initialize(IDC_SETTING_BTN_GESTURE_SETTINGS,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnGestureSettings.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnGestureSettings.SetCaption(_T("Gesture"));
	m_btnGestureSettings.LinkTextGL(pTextGL);
	m_btnGestureSettings.SetPos(0,py);
	m_btnGestureSettings.SetCaptionHeight(caption_h);
	m_btnGestureSettings.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnGestureSettings.SetCaptionColor(&cr_gray);
	m_btnGestureSettings.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	py-=70;
#endif

#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO) || (CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
	//rear view delay settings
	m_tagTitleRearViewDelay.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRearViewDelay.SetSize(cx,40);
	m_tagTitleRearViewDelay.SetCaption(_T("Rear View Settings"));
	m_tagTitleRearViewDelay.LinkTextGL(pTextGL);
	m_tagTitleRearViewDelay.SetPos(-cx/2-10 +12,py);
	m_tagTitleRearViewDelay.SetCaptionHeight(caption2_h);
	m_tagTitleRearViewDelay.EnableDrawBack(FALSE);
	m_tagTitleRearViewDelay.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleRearViewDelay.SetCaptionColor(&cr_dark);

	py-=60;

	m_btnRearViewDelay.Initialize(IDC_SETTING_BTN_SET_REARVIEW_DELAY,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRearViewDelay.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnRearViewDelay.SetCaption(_T("Rear View Delay"));
	m_btnRearViewDelay.LinkTextGL(pTextGL);
	m_btnRearViewDelay.SetPos(0,py);
	m_btnRearViewDelay.SetCaptionHeight(caption_h);
	m_btnRearViewDelay.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnRearViewDelay.SetCaptionColor(&cr_gray);
	m_btnRearViewDelay.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValueRearViewDelay.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueRearViewDelay.LinkTextGL(pTextGL);
	m_tagValueRearViewDelay.SetSize(cx,40);
	m_tagValueRearViewDelay.SetPos(cx/2+10-20,py);
	m_tagValueRearViewDelay.SetCaptionHeight(caption_h);
	m_tagValueRearViewDelay.EnableDrawBack(FALSE);
	m_tagValueRearViewDelay.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueRearViewDelay.SetCaptionColor(&cr_gray);
	m_tagValueRearViewDelay.SetCaption(g_strRearViewDelayValue[m_pSkinManager->GetConfig()->GetCurParam()->dwRearViewDelay]);

	py-=70;

	//power off delay settings
	m_tagTitlePowerOffDelay.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitlePowerOffDelay.SetSize(cx,40);
	m_tagTitlePowerOffDelay.SetCaption(_T("Power Off Settings"));
	m_tagTitlePowerOffDelay.LinkTextGL(pTextGL);
	m_tagTitlePowerOffDelay.SetPos(-cx/2-10 +12,py);
	m_tagTitlePowerOffDelay.SetCaptionHeight(caption2_h);
	m_tagTitlePowerOffDelay.EnableDrawBack(FALSE);
	m_tagTitlePowerOffDelay.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitlePowerOffDelay.SetCaptionColor(&cr_dark);

	py-=60;

	/*
	m_btnPowerOffDelay.Initialize(IDC_SETTING_BTN_SET_POWEROFF_DELAY,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPowerOffDelay.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnPowerOffDelay.SetCaption(_T("Power Off Delay"));
	m_btnPowerOffDelay.LinkTextGL(pTextGL);
	m_btnPowerOffDelay.SetPos(0,py);
	m_btnPowerOffDelay.SetCaptionHeight(caption_h);
	m_btnPowerOffDelay.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnPowerOffDelay.SetCaptionColor(&cr_gray);
	m_btnPowerOffDelay.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	m_tagValuePowerOffDelay.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValuePowerOffDelay.LinkTextGL(pTextGL);
	m_tagValuePowerOffDelay.SetSize(cx,40);
	m_tagValuePowerOffDelay.SetPos(cx/2+10-20,py);
	m_tagValuePowerOffDelay.SetCaptionHeight(caption_h);
	m_tagValuePowerOffDelay.EnableDrawBack(FALSE);
	m_tagValuePowerOffDelay.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValuePowerOffDelay.SetCaptionColor(&cr_gray);
	m_tagValuePowerOffDelay.SetCaption(m_pSkinManager->GetConfig()->GetCurParam()->dwPowerOffDelay ? _T("ON") : _T("OFF"));
	*/

	m_tagPowerOffDelay.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPowerOffDelay.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagPowerOffDelay.SetCaption(_T("Power Off Delay"));
	m_tagPowerOffDelay.LinkTextGL(pTextGL);
	m_tagPowerOffDelay.SetPos(0,py);
	m_tagPowerOffDelay.SetCaptionHeight(caption_h);
	m_tagPowerOffDelay.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagPowerOffDelay.SetCaptionColor(&cr_gray);

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
	m_sliPowerOffDelay.Initialize(IDC_SETTING_BTN_SET_POWEROFF_DELAY,this,&sli,pSkinManager);
	m_sliPowerOffDelay.SetRange(0,10);
	m_sliPowerOffDelay.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwPowerOffDelay?10:0);
	m_sliPowerOffDelay.SetPos(m_siCurSize.cx/2-70,py);
	m_sliPowerOffDelay.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	py-=70;
#endif

	//Factory settings
	m_tagTitleFactory.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleFactory.SetSize(cx,40);
	m_tagTitleFactory.SetCaption(GetResourceString(IDS_SETTING_UI_FACTORY));
	m_tagTitleFactory.LinkTextGL(pTextGL);
	m_tagTitleFactory.SetPos(-cx/2-10 +12,py);
	m_tagTitleFactory.SetCaptionHeight(caption2_h);
	m_tagTitleFactory.EnableDrawBack(FALSE);
	m_tagTitleFactory.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleFactory.SetCaptionColor(&cr_dark);



	py-=60;

	m_btnRestoreFactorySettings.Initialize(IDC_SETTING_BTN_FACTORY,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRestoreFactorySettings.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnRestoreFactorySettings.SetCaption(GetResourceString(IDS_SETTING_UI_RESTORE_FACTORY_SETTINGS));
	m_btnRestoreFactorySettings.LinkTextGL(pTextGL);
	m_btnRestoreFactorySettings.SetPos(0,py);
	
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_GREEK:
		m_btnRestoreFactorySettings.SetCaptionHeight(caption2_h);
		break;
	default:
		m_btnRestoreFactorySettings.SetCaptionHeight(caption_h);
		break;
	}

	m_btnRestoreFactorySettings.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnRestoreFactorySettings.SetCaptionColor(&cr_gray);
	m_btnRestoreFactorySettings.SetNotifyFunc(&CPanelSettingGeneral::OnNotify,this);

	if(m_pStatusBar)
	{
		m_pStatusBar->SetTimeMode(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode);
	}

		
	int iPanelHeight = g_iClientHeight/2 - py + tag_h;

	if(iPanelHeight < g_iClientHeight)
		iPanelHeight = g_iClientHeight;

	SetSize(g_iClientWidth-box_size_x,iPanelHeight);

#endif

	return TRUE;
}


void CALLBACK CPanelSettingGeneral::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingGeneral *panel=(CPanelSettingGeneral*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingGeneral::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}


	if(dwType == 0)//package
	{
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_BOSSAUDIOUI)
		//reverse polar:
		m_sliReversePolar.SetValue(pCommander->GetItemData(MCU_RESERVE_POLA)?10:0);

		//break test:
		m_sliBrakeTest.SetValue(pCommander->GetItemData(MCU_BREAKE_DETECT)?10:0);

		//lamp:
		m_sliLampControl.SetValue(pCommander->GetItemData(MCU_LIGHT_CONTROL)?10:0);
#endif
#endif
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
		case MCU_CUR_FREQU_STORE_L:
			{
			}
			break;
        case MCU_BACKLIGHT_CONTROL_KEY:
            {
                RETAILMSG(1, (TEXT("APP:MSG:Setting: [CPanelSettingGeneral::OnMCUCmd] %d\r\n"),pCommander->GetItemData(MCU_BACKLIGHT_CONTROL_KEY)));
            }
            break;
		}
	}
}
void CPanelSettingGeneral::ConfigBrakeTest(BOOL bOn)
{
	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_BREAKE_TEST_KEY,bOn?1:0);
	}
}
void CPanelSettingGeneral::ConfigReversePolar(BOOL bHigh)
{
	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_REVERSE_SEL_KEY,bHigh?1:0);
	}
}
void CPanelSettingGeneral::ConfigLampControl(BOOL bOn)
{
	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_LAMP_CONTROL_KEY,bOn?1:0);
	}
}

void CPanelSettingGeneral::ConfigBacklightControl(BOOL bOn)
{
	if(m_pCommander)
	{
        RETAILMSG(1, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingGeneral::ConfigBacklightControl] :%d\r\n"),bOn));
		m_pCommander->SendCmd(_ARM_BACKLIGHT_CONTROL_KEY,bOn?1:0);
	}
}

//	void CPanelSettingGeneral::ConfigKeyBeep(BOOL bOn)
//	{
//		m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep = bOn;
//		g_bKeyBeep = bOn;
//	}
void CPanelSettingGeneral::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
		case IDC_SETTING_SLI_BREAK_TEST:
			ConfigBrakeTest(wPara);
			PlaySoundEffect(0);
			break;
		case IDC_SETTING_SLI_REVERSE_LEVEL:
			ConfigReversePolar(wPara);
			PlaySoundEffect(0);
			break;
		case IDC_SETTING_SLI_LAMP_CONTROL:
			ConfigLampControl(wPara);
			PlaySoundEffect(0);
			break;
		case IDC_SETTING_BTN_SET_DRIVE_TYPE:
			{
				if(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType == 1)//right
				{
					m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType = 0;
					m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_LEFT));
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType = 1;
					m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_RIGHT));
				}
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
				//Update Driver Type Info to MCU
				if (m_pCommander)
				{
					BYTE buf = m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType;
					m_pCommander->SendExtenCmd(_ARM_EXTEN_TAPE_SIDE,&buf,1);
				}	
#endif
				PlaySoundEffect(0);
			}
			break;
#endif
		//		case IDC_SETTING_SLI_KEY_BEEP:
		//			ConfigKeyBeep(wPara);
		//			PlaySoundEffect(0);
		//			break;
	case IDC_SETTING_BTN_COLORSIGNAL:
		{
			if(m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard == OEMCONFIG_COLORSIGNAL_NTSM_MJ ||
			m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard == OEMCONFIG_COLORSIGNAL_NTSC_443)//right
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard = OEMCONFIG_COLORSIGNAL_PAL_M;
				m_tagRearColorSignalType.SetCaption(_T("PAL"));
			}
			else
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard = OEMCONFIG_COLORSIGNAL_NTSM_MJ;
				m_tagRearColorSignalType.SetCaption(_T("NTSC"));
			}

			PlaySoundEffect(0);
		}
		break;
//		case IDC_SETTING_BTN_SET_IDLE_SCREEN:
//			{
//				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_SET_IDLE_SCREEN: \r\n")));
//				if(m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen == IDLE_SCREEN_NONE)//none
//				{
//					m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen = IDLE_SCREEN_ANALOG_CLOCK;
//					m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_CLOCK_ANALOG));
//	                ConfigBacklightControl(FALSE);
//				}
//				else if(m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen == IDLE_SCREEN_ANALOG_CLOCK)//analog clock
//				{
//					m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen = IDLE_SCREEN_DIGITAL_CLOCK;
//					m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_CLOCK_DIGITAL));
//	                ConfigBacklightControl(TRUE);
//				}
//				else if(m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen == IDLE_SCREEN_DIGITAL_CLOCK)//digital clock
//				{
//					m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen = IDLE_SCREEN_NONE;
//					m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_NONE));
//	                ConfigBacklightControl(TRUE);
//				}
//	    		m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen = IDLE_SCREEN_NONE;
//	    		m_tagValueIdleScreen.SetCaption(GetResourceString(IDS_NONE));
//	            //ConfigBacklightControl(TRUE);
//
//				PlaySoundEffect(0);
//			}
//			break;

		//time&date
	case IDC_SETTING_BTN_SET_TIMEZONE:
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_TIMEZONE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_SET_DATE:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_SET_DATE: \r\n")));
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_DATE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_SET_TIME:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_SET_TIME: \r\n")));
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_TIME);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_TIME_24HMODE:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_SLI_TIME_24HMODE: \r\n")));
			m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode = ((wPara==0)?TIME_MODE_12H:TIME_MODE_24H);

			if(m_pStatusBar)
			{
				m_pStatusBar->SetTimeMode(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode);
			}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)			
			SendBluetoothCommand(BT_CMD_SET_24H_TIME_MODE,(wPara?1:0));
#endif			
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_TIME_AUTOADJUST:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_SLI_TIME_AUTOADJUST: \r\n")));
			m_pSkinManager->GetConfig()->GetCurParam()->bTimeAutoAdjust = ((wPara==0)?FALSE:TRUE);

			if(wPara)
			{
				//need re-sync time:
				g_dwTimeSyncedCnt = 0;
			}

			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_TIME_AUTOADJUST_DLST:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_SLI_TIME_AUTOADJUST_DLST: \r\n")));
			m_pSkinManager->GetConfig()->GetCurParam()->bDSTAutoAdjust = (wPara==0)?FALSE:TRUE;

			OnDSTChanged();
			
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_AAP_AUTOLAUNCH:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: IDC_SETTING_SLI_AAP_AUTOLAUNCH: %d\r\n"),(wPara==0)?FALSE:TRUE));
			m_pSkinManager->GetConfig()->GetCurParam()->bAAPAutoLaunch = (wPara==0)?FALSE:TRUE;
			PlaySoundEffect(0);
		}
		break;
		//navigation:
	case IDC_SETTING_BTN_NAVI_SPEECH:
		{
			if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)
				ConfigNaviSpeech(NAVI_SPEECH_WHILEONTOP);
			else if(m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_WHILEONTOP)
				ConfigNaviSpeech(NAVI_SPEECH_OFF);
			else
				ConfigNaviSpeech(NAVI_SPEECH_ALWAYS);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_NAVI_PATH:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_NAVI_PATH: \r\n")));
			ShowExplorer(IDC_PANEL_EXPLORER+IDC_SETTING_BTN_NAVI_PATH,&CPanelSettingGeneral::OnNotify,this,
				GetResourceString(IDS_TITLE_NAVI),
				_T("\\"),_T("EXE"));
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_SKIN_PATH:
#if 0
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_SKIN_PATH: \r\n")));
			ShowExplorer(IDC_PANEL_EXPLORER+IDC_SETTING_BTN_SKIN_PATH,&CPanelSettingGeneral::OnNotify,this,
				GetResourceString(IDS_SETTING_UI_SKIN),
				_T("\\"),_T("SKN"));
			PlaySoundEffect(0);
		}
#endif
		break;
	case IDC_SETTING_BTN_BACKGROUND_PATH:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_BACKGROUND_PATH: \r\n")));
			//ShowExplorer(IDC_PANEL_EXPLORER+IDC_SETTING_BTN_BACKGROUND_PATH,&CPanelSettingGeneral::OnNotify,this,
			//	GetResourceString(IDS_SETTING_UI_BACKGROUND),
			//	_T("\\"),_T("JPG"));
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_BACKGROUND);
			PlaySoundEffect(0);
		}
		break;
	case IDC_PANEL_EXPLORER+IDC_SETTING_BTN_NAVI_PATH:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_PANEL_EXPLORER+IDC_SETTING_BTN_NAVI_PATH: lpPara=%s\r\n"),(const wchar_t*)lpPara));
			if(lpPara)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->strNaviPath=(const wchar_t*)lpPara;

				m_tagValueNaviPath.SetCaption((const wchar_t*)lpPara,TRUE);
				//update Navi app path:
				::SendMessage(m_hMainWnd,WM_NAVI_PATH_CAHNGED,0,(LPARAM)lpPara);
			}
			PlaySoundEffect(0);
		}
		break;
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	case IDC_PANEL_EXPLORER+IDC_SETTING_BTN_SKIN_PATH:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_PANEL_EXPLORER+IDC_SETTING_BTN_SKIN_PATH: lpPara=%s\r\n"),(const wchar_t*)lpPara));
			if(lpPara)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->strSkinFileName=(const wchar_t*)lpPara;

				CM_String strSkinName;
				m_pSkinManager->GetSkinName((const wchar_t*)lpPara,&strSkinName);
				m_tagValueSkinPath.SetCaption(strSkinName.String(),TRUE);

				m_pSkinManager->Reload((wchar_t*)lpPara);
			}
			PlaySoundEffect(0);
		}
		break;
#endif
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	case IDC_PANEL_EXPLORER+IDC_SETTING_BTN_BACKGROUND_PATH:
		{
			if(lpPara)
			{
				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_PANEL_EXPLORER+IDC_SETTING_BTN_BACKGROUND_PATH: %s\r\n"),(const wchar_t*)lpPara));

				//copy file:
				CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("background\\"));
				CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("background\\User.jpg"));
				CreateDirectory(strPath.String(),NULL);

				CopyFile((const wchar_t*)lpPara,strFile.String(),FALSE);

				m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName=strFile;

				CTexture *pTex=m_pSkinManager->GetTexture(TEXID(2000));
				if(pTex)
				{
					pTex->ReloadFromFileScaled(strFile.String(),0,NULL,GL_LINEAR,NULL,TRUE,5);
				}

				wchar_t name[MAX_PATH];
				if(GetFileNameFromPath(strFile.String(),name,MAX_PATH))
				{
#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
					if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
					{
						if(wcsstr(name, _T("User")))
							m_tagValueBackground.SetCaption(_T("Мои"),TRUE);
						else
							m_tagValueBackground.SetCaption(_T("По умолчанию"),TRUE);
					}
					else
					{
						m_tagValueBackground.SetCaption(name,TRUE);
					}
#else
					m_tagValueBackground.SetCaption(name,TRUE);
#endif
				}
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_BACK_LUMIN:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance = GLfloat(wPara)/10.0f;
			PlaySoundEffect(0);
		}
		break;
#endif
//		case IDC_SETTING_SLI_BLUR_EFFECT:
//			{
//				m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect = (wPara?1:0);
//				PlaySoundEffect(0);
//			}
//			break;
		//language:
	case IDC_SETTING_BTN_LANGUAGE_SEL:
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_LANGUAGE);

			/*
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_LANG_ENGLISH \r\n")));
			if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage!=0)
			{
				//m_pSkinManager->GetConfig()->GetCurParam()->idLanguage=0;

				ShowNotificationBox(m_hMainWnd,IDC_SETTING_SET_LANG_RESET_CONFIRM+IDC_SETTING_BTN_LANG_ENGLISH,
					&CPanelSettingGeneral::OnNotify,
					this,
					0,
					GetResourceString(IDS_SETTING_LANGUAGE),
					GetResourceString(IDS_RESET_TO_TAKE_EFFECT),
					0);
			}
			*/
			PlaySoundEffect(0);
		}
		break;


	case IDC_SETTING_SET_LANG_RESET_CONFIRM+IDC_SETTING_BTN_LANG_ENGLISH:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_SET_LANG_RESET_CONFIRM+IDC_SETTING_BTN_LANG_ENGLISH \r\n")));
			if(wMsg == NB_OK)//
			{
				m_pSkinManager->GetConfig()->GetCurParam()->idLanguage=0;
				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: ----confirm to reset-----\r\n")));
				//to do:
				::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
			}
			PlaySoundEffect(0);
		}
		break;

#if CVTE_EN_COMBINE_ANDROID_LINK
	case IDC_SETTING_BTN_ANDROIDTYPE_SEL:
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_ANDROIDTYPE);
			PlaySoundEffect(0);
		}
		break;
#endif

	case IDC_SETTING_BTN_FACTORY:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_FACTORY \r\n")));
			ShowNotificationBox(m_hMainWnd,IDC_SETTING_RESTORE_FACTORY_SETTINGS,
				&CPanelSettingGeneral::OnNotify,
				this,
				0,
				GetResourceString(IDS_SETTING_UI_RESTORE_FACTORY_SETTINGS),
				GetResourceString(IDS_RESET_TO_TAKE_EFFECT),
				0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_RESTORE_FACTORY_SETTINGS:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_RESTORE_FACTORY_SETTINGS \r\n")));
			if(wMsg == NB_OK)//
			{
				//to do:
				if(m_pSkinManager)
				{
					RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: ----confirm to reset-----\r\n")));

					RestoreFactorySetting();

					::PostMessage(m_hMainWnd,WM_SYSTEM_PRE_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
					::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
				}
			}
			PlaySoundEffect(0);
		}
		break;

	case IDC_SETTING_BTN_ADVANCED_SETTINGS:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_ADVANCED_SETTINGS \r\n")));
			ShowPasswordBox(m_hMainWnd,IDC_SETTING_BTN_ADVANCE,
				&CPanelSettingGeneral::OnNotify,
				this,
				0,
				GetResourceString(IDS_SETTING_ADVANCE),
				GetResourceString(IDS_SETTING_ADVANCED),
				0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_ADVANCE:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_ADVANCE \r\n")));
			if(wMsg == NB_OK)//
			{
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_ADVANCED);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_GESTURE_SETTINGS:	
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: IDC_SETTING_BTN_GESTURE \r\n")));
			if(wMsg == NB_OK)//
			{
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GESTURE);
			}
			PlaySoundEffect(0);
		}
		break;	
	case IDC_SETTING_SLI_DCP_CONTROL:
		{
#if CVTE_EN_CDP_DCP_CONTROL
			SwitchDCP(wPara>=10?TRUE:FALSE);
			PlaySoundEffect(0);
#endif
		}
		break;
#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO) || (CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
	case IDC_SETTING_BTN_SET_REARVIEW_DELAY:
		{
			if(m_pCommander)
			{
				UINT setVal = 0;
				UINT setCurrent = m_pSkinManager->GetConfig()->GetCurParam()->dwRearViewDelay;
				setVal = (setCurrent+1);
				if(setVal > 4)
					setVal = 0;
				m_pSkinManager->GetConfig()->GetCurParam()->dwRearViewDelay = setVal;
				m_pCommander->SendCmd(_ARM_SET_REARVIEW_DELAY,BYTE(setVal));
				m_tagValueRearViewDelay.SetCaption(g_strRearViewDelayValue[setVal]);
				RETAILMSG(1,(_T("APP:MSG:GUI:Settings: _ARM_SET_REARVIEW_DELAY: %d \r\n"),setVal));
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_SET_POWEROFF_DELAY:
		{
			if(m_pCommander)
			{
				UINT setVal = ((wPara==0)?0:1);
				m_pSkinManager->GetConfig()->GetCurParam()->dwPowerOffDelay = setVal;
				m_pCommander->SendCmd(_ARM_SET_POWEROFF_DELAY,BYTE(setVal));
				RETAILMSG(1,(_T("APP:MSG:GUI:Settings: _ARM_SET_POWEROFF_DELAY: %d \r\n"),setVal));
			}
			PlaySoundEffect(0);
		}
		break;
#endif
	}
}
void CPanelSettingGeneral::RestoreFactorySetting()
{

	//general:
//		ConfigReversePolar(TRUE);
	ConfigBrakeTest(TRUE);
	ConfigLampControl(TRUE);

	Sleep(100);

	//audio:
	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_LOUDNESS_KEY,0);
		m_pCommander->SendCmd(_ARM_SUBWOOFER_KEY,7);

		m_pCommander->SendCmd(_ARM_BASS_KEY,7);
		m_pCommander->SendCmd(_ARM_MID_KEY,7);
		m_pCommander->SendCmd(_ARM_TREBLE_KEY,7);

		m_pCommander->SendCmd(_ARM_BALANCE_KEY,7);
		m_pCommander->SendCmd(_ARM_FAD_KEY,7);

		m_pCommander->SendCmd(_ARM_EQ_KEY,EQ_NONE);
	}

	Sleep(100);

	//radio:
	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_RADIO_RESET_STOP_LEVEL,0);
		//m_pCommander->SendCmd(_ARM_REGION_SEL_KEY,0);
	}

        //Test file
	//if(IsFileExists(_T("\\ResidentFlash\\Media\\TEST.mp4")))
	//{
	    RETAILMSG(1, (TEXT("Delete test file.\r\n")));
		DeleteFile(_T("\\ResidentFlash\\Media\\TEST.mp4"));
	//}

	//radio:
	//delete radio file:
	ClearFilesInFolder(_T("\\ResidentFlash\\Radio\\"));

	//bluetooth:
	//delete config file:
	SendBluetoothCommand(BT_CMD_SET_AUTO_CONNECT,1);
	SendBluetoothCommand(BT_CMD_SET_AUTO_ANSWER,0);
	SendBluetoothCommand(BT_CMD_SET_AUTO_SYNCPB,0);

	DeleteFile(_T("\\ResidentFlash\\dll\\config\\config.flc"));
	DeleteFile(_T("\\ResidentFlash\\dll\\ESIBT.ddb"));

	//delete bluetooth stored phonebook
	ClearFilesInFolder(_T("\\ResidentFlash\\Phonebook\\"));

	//DAB:
	DeleteFile(_T("\\ResidentFlash\\DAB\\dab_config.dat"));


	//reset factory settings:
	//m_pSkinManager->GetConfig()->RestoreFactorySetting();

	RestoreBTReg();

	RestoreTimeZone();

	RestoreRegSetting();

	Sleep(500);
}

void CPanelSettingGeneral::ClearFilesInFolder(LPCTSTR foder)
{
	WIN32_FIND_DATA wfd;
	TCHAR szwFind[MAX_PATH];
	TCHAR szwFile[MAX_PATH];
	swprintf_s(szwFind,MAX_PATH,_T("%s*.*"),foder);
	HANDLE hFind=FindFirstFile(szwFind,&wfd);
	if(hFind!=INVALID_HANDLE_VALUE) //
	{
		do
		{
			if(wfd.cFileName[0]=='.')
				continue;

			if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				swprintf_s(szwFile,MAX_PATH,_T("%s%s"),foder,wfd.cFileName);
				BOOL res=DeleteFile(szwFile);
				RETAILMSG(1,(_T(">>DeleteFile: %s, res=%d \r\n"),szwFile,res));
			}
		}
		while(FindNextFile(hFind,&wfd));
		FindClose(hFind); //
	}
}
BOOL CPanelSettingGeneral::RestoreTimeZone(void)
{
	TIME_ZONE_INFORMATION m_tziSelected;
	CReg Reg1(HKEY_LOCAL_MACHINE, RV_TIMEZONES);
	CReg Reg2;

	Reg2.Reset();

	CM_String strTimeZones = CVTE_REG_TIMEZONE_DEFAULT;
	Reg2.Open(HKEY_LOCAL_MACHINE,(LPCTSTR)(strTimeZones.String()));

	TZREG  *pTZR = (TZREG*)(Reg2.ValueBinary(RV_TZI));

	if(pTZR)
	{
		m_tziSelected.Bias = pTZR->Bias;
		m_tziSelected.StandardBias = pTZR->StandardBias;
		m_tziSelected.DaylightBias = pTZR->DaylightBias;
		m_tziSelected.StandardDate = pTZR->StandardDate;
		m_tziSelected.DaylightDate = pTZR->DaylightDate;

		Reg2.ValueSZ(RV_DLT, m_tziSelected.DaylightName, 32);
		Reg2.ValueSZ(RV_STD, m_tziSelected.StandardName, 32);

		CReg reg(HKEY_LOCAL_MACHINE, RK_CLOCK);
		SYSTEMTIME  stlocal;
		GetLocalTime(&stlocal);
		BOOL fUseDST = reg.ValueDW(RV_AUTODST,0) && IsUseDST(m_tziSelected, stlocal);
		__int64 Bias = m_tziSelected.Bias + (fUseDST ? m_tziSelected.DaylightBias : m_tziSelected.StandardBias);

		SYSTEMTIME  stNew;
		SYSTEMTIME	pstOld;

		__int64     ftOld = 0;
		__int64     ftNew = 0;

		GetSystemTime(&pstOld);

		//SystemTimeToFileTime(&pstOld, (FILETIME*)&ftOld);

		//ftNew = ftOld - ((Bias) * FILETIME_TO_MINUTES);
		//FileTimeToSystemTime((FILETIME*)&ftNew, &stNew);

		// now update the clock
		memset(&stNew, 0, sizeof(stNew));
		stNew.wYear = 2016;
		stNew.wMonth = 1;
		stNew.wDay = 1;
		stNew.wHour = 0;
		stNew.wMinute = 0;
		stNew.wSecond = 0;
		SetLocalTime(&stNew);

		// update our notion of "current" timezone
		SetTimeZoneInformation(&m_tziSelected);
	}

	RETAILMSG(1,(_T(">>>>>Save [Time Zones] :%s\n"),strTimeZones.String()));
	return TRUE;
}

BOOL CPanelSettingGeneral::RestoreBTReg(void)
{
	CReg reg;
	CM_String strBTDeviceName = CVTE_REG_BT_DEVICE_NAME_DEFAULT;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Ident\\"));
	reg.SetSZ(_T("Name"),(const wchar_t*)strBTDeviceName.String());

	RETAILMSG(1,(_T(">>>>>Save [HKEY_LOCAL_MACHINE\\Ident] name:%s\n"),strBTDeviceName.String()));
	return TRUE;
}

BOOL CPanelSettingGeneral::RestoreRegSetting()
{
	//0-English,2-Chinese tra,3-Chinese simple,4-Korean,5-German,
	//6-Spanish,7-Portuguese,8-Italian,9-French,10-Japaness,11-Russian

	//Radio region
	//0-Europe, 1-USA, 2-Eastern Europe, 3-Japan, 4-Europe & Eastern Europe 5-Korean.
	m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = CVTE_REG_RADIO_REGION_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF = CVTE_REG_RDS_AF_LEVEL_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->idLanguage = CVTE_REG_LANGUAGE_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode = CVTE_REG_TIME_MODE_DEFAULT;

	//rds:
	m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS = CVTE_REG_RDS_ENABLE_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bRadioAF = CVTE_REG_RDS_AF_ENABLE_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bRadioTA = CVTE_REG_RDS_TA_ENABLE_DEFAULT;

	//Gamma
	m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault = CVTE_REG_GAMMA_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent = CVTE_REG_GAMMA_DEFAULT;

	//LED
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDRedDefault = CVTE_REG_LED_R_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDGreenDefault = CVTE_REG_LED_G_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDBlueDefault = CVTE_REG_LED_B_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDRedCurrent = CVTE_REG_LED_R_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDGreenCurrent = CVTE_REG_LED_G_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDBlueCurrent = CVTE_REG_LED_B_DEFAULT;

	//DayMode & NightMode
	m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness = CVTE_REG_LCD_DAYMODE_BRIGHTNESS_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness = CVTE_REG_LCD_NIGHTMODE_BRIGHTNESS_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness = CVTE_REG_LED_DAYMODE_BRIGHTNESS_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness = CVTE_REG_LED_NIGHTMODE_BRIGHTNESS_DEFAULT;


	m_pSkinManager->GetConfig()->GetCurParam()->idFont = CVTE_REG_FONT_DEFAULT;
	// 	m_pSkinManager->GetConfig()->GetCurParam()->strInstallPath=_T("\\Windows\\");
	m_pSkinManager->GetConfig()->GetCurParam()->strLogoName=_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo.jpg");
	m_pSkinManager->GetConfig()->GetCurParam()->strLogoName2=_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo2.jpg");
	// 	m_pSkinManager->GetConfig()->GetCurParam()->strSkinPath=_T("\\Windows\\skin_shell.skn");

	m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName=CVTE_REG_BACKGROUND_PATH_DEFAULT;

	m_pSkinManager->GetConfig()->GetCurParam()->strNaviPath = CVTE_REG_NAVI_PATH_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->strSkinFileName=_T("\\ResidentFlash\\LightningUI\\Skins\\skin_lightning.skn");
	m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC=0;
	m_pSkinManager->GetConfig()->GetCurParam()->idPreSRC=0;
	m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI=0;
	m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC=0;
	m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance=0.8f;
	m_pSkinManager->GetConfig()->GetCurParam()->fUITransparency=0.8f;

	m_pSkinManager->GetConfig()->GetCurParam()->strOwnerName=_T("None");

	m_pSkinManager->GetConfig()->GetCurParam()->iMediaType=0;
	m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory=1;

	m_pSkinManager->GetConfig()->GetCurParam()->strCurMediaDevice=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->strCurMediaFile=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iCurMediaPos=0;
	m_pSkinManager->GetConfig()->GetCurParam()->idMediaPlayMode=0;
	m_pSkinManager->GetConfig()->GetCurParam()->idMediaLibraryType=0;

	m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileSD1=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosSD1=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileSD2=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosSD2=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileUSB1=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosUSB1=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileUSB2=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosUSB2=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileResident=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosResident=0;


	m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD1=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosSD1=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD2=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosSD2=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB1=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosUSB1=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB2=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosUSB2=0;
	m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileResident=_T("");
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosResident=0;


	m_pSkinManager->GetConfig()->GetCurParam()->iVideoContrast=32;
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoBrightness=32;
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoHUE=32;
	m_pSkinManager->GetConfig()->GetCurParam()->iVideoSaturation=32;

	m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS = CVTE_REG_AUDIO_VOLUME_SPEECH_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia = CVTE_REG_AUDIO_VOLUME_MEDIA_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->dwVolBluetooth = CVTE_REG_AUDIO_VOLUME_PHONE_DEFAULT;

	m_pSkinManager->GetConfig()->GetCurParam()->iScreenSaverTimeOut = 0;

	m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn=TRUE;
	m_pSkinManager->GetConfig()->GetCurParam()->bContactAutoDownload=FALSE;
	m_pSkinManager->GetConfig()->GetCurParam()->bCallAutoAnswer=FALSE;
	m_pSkinManager->GetConfig()->GetCurParam()->bIncomingRing=TRUE;
	m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothAutoConnect=TRUE;
	m_pSkinManager->GetConfig()->GetCurParam()->bVcardFirstNameFirst = 1;

	m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig = NAVI_SPEECH_ALWAYS;

	memset(m_pSkinManager->GetConfig()->GetCurParam()->dwKeyVolt,0,16);
	memset(m_pSkinManager->GetConfig()->GetCurParam()->idUIList,0,32);

	m_pSkinManager->GetConfig()->GetCurParam()->idUIList[0]=3;//radio
	m_pSkinManager->GetConfig()->GetCurParam()->idUIList[1]=6;//bt
	m_pSkinManager->GetConfig()->GetCurParam()->idUIList[2]=2;//media
	m_pSkinManager->GetConfig()->GetCurParam()->idUIList[3]=9;//navi
	m_pSkinManager->GetConfig()->GetCurParam()->idUIList[4]=8;//disk
	m_pSkinManager->GetConfig()->GetCurParam()->idUIList[5]=4;//video
	m_pSkinManager->GetConfig()->GetCurParam()->idUIList[6]=23;//vehicle

	m_pSkinManager->GetConfig()->GetCurParam()->bTimeAutoAdjust = TRUE;
	m_pSkinManager->GetConfig()->GetCurParam()->bDSTAutoAdjust = TRUE;
	OnDSTChanged();
	m_pSkinManager->GetConfig()->GetCurParam()->dwBacklight = CVTE_REG_BACKLIGHT_DEFAULT;

	m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen = 0;//none
	m_pSkinManager->GetConfig()->GetCurParam()->dwVolumeAdjustRate = 0;

	//m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType = 0;

	m_pSkinManager->GetConfig()->GetCurParam()->bMCUSoftwareUpdateFlag = 0;

	m_pSkinManager->GetConfig()->GetCurParam()->bAutoStartNavi = CVTE_REG_AUTO_START_NAVI_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep = CVTE_REG_KEEP_BEEP_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bRearVolControl = CVTE_REG_REAR_VOL_CONTROL_DEFAULT;

	m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect = CVTE_REG_BLUR_EFFECT_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bLedAnimEffect = CVTE_REG_LED_ANIM_EFFECT_DEFAULT;
	//m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType = 0;
	m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType = CVTE_REG_DRIVE_TYPE_DEFAULT;
	//m_pSkinManager->GetConfig()->GetCurParam()->iTimeZone = 8;

#if CVTE_EN_DTV
	m_pSkinManager->GetConfig()->GetCurParam()->iDTVStandard = CVTE_REG_DTV_COLOR_DEFAULT;    //NTSC
#endif
#if CVTE_EN_DVD
	m_pSkinManager->GetConfig()->GetCurParam()->iDVDStandard = CVTE_REG_DVD_COLOR_DEFAULT;    //NTSC
#endif
	m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard = CVTE_REG_REARVIEW_COLOR_DEFAULT;   //NTSC:0 PAL:3

	m_pSkinManager->GetConfig()->GetCurParam()->bActiveFactory = CVTE_REG_FACTORY_MODE_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bSnapshotEnable = CVTE_REG_SNAPSHOT_ENABLE_DEFAULT;
#if CVTE_EN_DAB
	m_pSkinManager->GetConfig()->GetCurParam()->bDABEnable = CVTE_REG_DAB_ENABLE_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bDABAntPowerOn = CVTE_REG_DAB_ANT_POWER_ON_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bDABServiceLink = CVTE_REG_DAB_SERVICE_LINK_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bDABLBAND = CVTE_REG_DAB_LBAND_DEFAULT;
	m_pSkinManager->GetConfig()->GetCurParam()->bDABTA = CVTE_REG_DAB_TA_DEFAULT;
#endif

	m_pSkinManager->GetConfig()->GetCurParam()->dwRearViewDelay = 0;
	m_pSkinManager->GetConfig()->GetCurParam()->dwPowerOffDelay = 0;
	m_pSkinManager->GetConfig()->GetCurParam()->bEQSwitch = 0;

	return TRUE;
}

BOOL CPanelSettingGeneral::ConfigNaviSpeech(NAVI_SPEECH_CONFIG config)
{
	if(m_pSkinManager)
	{
		m_pSkinManager->GetConfig()->GetCurParam()->idNaviSpeechConfig = config;
	}

	EnableNaviSpeech(config == NAVI_SPEECH_ALWAYS);

	switch (config)
	{
	case NAVI_SPEECH_ALWAYS:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_ALWAYS));
		break;
	case NAVI_SPEECH_OFF:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_CLOSED));
		break;
	case NAVI_SPEECH_WHILEONTOP:
		m_tagValueNaviSpeech.SetCaption(GetResourceString(IDS_SETTING_NAVI_SPEECH_MAP));
		break;
	}

	return TRUE;
}

void CPanelSettingGeneral::LinkPages(void *pLibrary)
{
	m_ppSettingBackground = (CPanelSettingBackground*)pLibrary;
}

void CPanelSettingGeneral::OnBackgroundChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SETTING, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingGeneral::OnBackgroundChanged].%s\r\n"),lParam));

	if(lParam /*&& lstrcmpi((const wchar_t*)lParam, m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName.String()) != 0*/)
	{
		/*
		int o=0;
		int i,m,n;
		RETAILMSG(1, (TEXT("o.%d\r\n"),o));
		for (i=0;i<500;i++)
		{
			for (m=0;m<500;m++)
			{
				for (n=0;n<4000;n++)
				{
					o++;
					o+=m+n+i;
					o=o/234;
				}
			}
		}
		RETAILMSG(1, (TEXT("o.%d\r\n"),o));
		return;
		*/


		if(lstrcmpi((const wchar_t*)lParam, g_strDefaultBackground) == 0)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName=g_strDefaultBackground;

			CTexture *pTex=m_pSkinManager->GetTexture(TEXID(2000));
			if(pTex)
			{
				pTex->ReloadFromFileScaled(g_strDefaultBackground,0,NULL,GL_LINEAR,NULL,TRUE,5);
			}
			CTexture *pTex2=m_pSkinManager->GetTexture(TEXID(2001));
			if(pTex2)
			{
				pTex2->ReloadFromFileScaled(g_strDefaultBackground);
			}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
			#if (CVTE_MODEL_TYPE == CVTE_MODEL_TYPE_U702P_ASP)
			m_tagValueBackground.SetCaption(_T("BOSS AUDIO"),TRUE);
			#else
			m_tagValueBackground.SetCaption(_T("PLANET AUDIO"),TRUE);
			#endif
#elif (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
				m_tagValueBackground.SetCaption(_T("По умолчанию"),TRUE);
	#else
				m_tagValueBackground.SetCaption(_T("LIGHTNING"),TRUE);
	#endif
#endif

		}
		else
		{
			//copy file:
			CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("background\\"));
			CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("background\\User.jpg"));
			CreateDirectory(strPath.String(),NULL);

			RETAILMSG(DEBUG_SETTING, (TEXT("copy file src:%s\r\n"),(const wchar_t*)lParam));
			RETAILMSG(DEBUG_SETTING, (TEXT("copy file new file: %s\r\n"),strFile.String()));

			if(CopyFile((const wchar_t*)lParam,strFile.String(),FALSE))
			{
				m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName=strFile;

				CTexture *pTex=m_pSkinManager->GetTexture(TEXID(2000));
				if(pTex)
				{
					pTex->ReloadFromFileScaled(strFile.String(),0,NULL,GL_LINEAR,NULL,TRUE,5);
				}
				CTexture *pTex2=m_pSkinManager->GetTexture(TEXID(2001));
				if(pTex2)
				{
					pTex2->ReloadFromFileScaled(strFile.String());
				}
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
				wchar_t name[MAX_PATH];
				if(GetFileNameFromPath(strFile.String(),name,MAX_PATH))
				{
#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
					if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
					{
						if(wcsstr(name, _T("User")))
							m_tagValueBackground.SetCaption(_T("Мои"),TRUE);
						else
							m_tagValueBackground.SetCaption(_T("По умолчанию"),TRUE);
					}
					else
					{
						m_tagValueBackground.SetCaption(name,TRUE);
					}
#else
					m_tagValueBackground.SetCaption(name,TRUE);
#endif
				}
#endif
			}
			else
			{
				RETAILMSG(DEBUG_SETTING, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingGeneral::OnBackgroundChanged].copy file failed! err:0x%x\r\n"),GetLastError()));
			}
		}
	}
}

void CPanelSettingGeneral::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_SETTING, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingGeneral::OnDeviceArrivedReady] m_ppSettingBackground[0x%x].\r\n"),m_ppSettingBackground));

	if(m_ppSettingBackground)
	{
		m_ppSettingBackground->EnableRefresh();
	}
}
void CPanelSettingGeneral::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_SETTING, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingGeneral::OnDeviceRemovedReady] m_ppSettingBackground[0x%x].\r\n"),m_ppSettingBackground));

	if(m_ppSettingBackground)
	{
		m_ppSettingBackground->EnableRefresh();
	}
}

void CPanelSettingGeneral::OnRearColorSignalChange(UINT uColorSignal)
{
	m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard = uColorSignal;

	switch (m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard)
	{
	case OEMCONFIG_COLORSIGNAL_NTSM_MJ:
    case OEMCONFIG_COLORSIGNAL_NTSC_443:
		m_tagRearColorSignalType.SetCaption(_T("NTSC"));
		break;
	case OEMCONFIG_COLORSIGNAL_PAL_M:
    case OEMCONFIG_COLORSIGNAL_PAL_60:
    case OEMCONFIG_COLORSIGNAL_PAL_BGHID:
    case OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N:
		m_tagRearColorSignalType.SetCaption(_T("PAL"));
		break;
    default:
		m_tagRearColorSignalType.SetCaption(_T("NTSC"));
		break;
    }
}

void CPanelSettingGeneral::OnDSTChanged()
{
	TIME_ZONE_INFORMATION tziCurrent;
	DWORD tzid = GetTimeZoneInformation(&tziCurrent);
	CReg reg(HKEY_LOCAL_MACHINE, RK_CLOCK);
	SYSTEMTIME  stlocal;
	GetLocalTime(&stlocal);
	BOOL fIsUseDST = IsUseDST(tziCurrent, stlocal);
	reg.SetDW(RV_AUTODST, m_pSkinManager->GetConfig()->GetCurParam()->bDSTAutoAdjust);
	BOOL fUseDST =  m_pSkinManager->GetConfig()->GetCurParam()->bDSTAutoAdjust && fIsUseDST;
	RETAILMSG(1, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingGeneral::OnDSTChanged] fUseDST[%d], fIsUseDST[%d].\r\n"),fUseDST, fIsUseDST));
	__int64 Bias = tziCurrent.Bias + ( fUseDST ? tziCurrent.DaylightBias : tziCurrent.StandardBias );
	RETAILMSG(1, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingGeneral::OnDSTChanged] Bias[%ld].\r\n"),Bias));
	SYSTEMTIME  stNew;
	SYSTEMTIME	pstOld;

	__int64     ftOld = 0;
	__int64     ftNew = 0;

	GetSystemTime(&pstOld);

	SystemTimeToFileTime(&pstOld, (FILETIME*)&ftOld);

	ftNew = ftOld - ((Bias) * FILETIME_TO_MINUTES);
	FileTimeToSystemTime((FILETIME*)&ftNew, &stNew);

	// now update the clock
	SetLocalTime(&stNew);
}

#if CVTE_EN_CDP_DCP_CONTROL
void CPanelSettingGeneral::SwitchDCP(BOOL bEnable)
{
	DWORD dwDCPGpioID = 4;	//0 * 32 + 4; 
	HANDLE hGPIO = GPIOOpen();
	if (hGPIO == NULL)
		return;
	GPIOPadEnable(hGPIO, dwDCPGpioID);
	if (bEnable)
		GPIOOutputZero(hGPIO, dwDCPGpioID);
	else
		GPIOOutputOne(hGPIO, dwDCPGpioID);
	GPIOOutputEnable(hGPIO, dwDCPGpioID);
	GPIOClose(hGPIO);

	RETAILMSG(1, (TEXT("MSG:[%s]: bEnable:%d\r\n"), TEXT(__FUNCTION__), bEnable));
	m_pSkinManager->GetConfig()->GetCurParam()->bDCPControl = bEnable;
}
#endif