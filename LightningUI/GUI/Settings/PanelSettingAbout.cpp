#include "PanelSettingAbout.h"
#include "../../resource.h"
#include "CVTE_AppProxy.h"
#include "../../CarPlay/CarplayAdapter.h"

extern CM_String				g_strDeviceName;

CPanelSettingAbout::CPanelSettingAbout(void)
{
	m_dwMCUVersion = 0;
	m_dwUpdateMcuRequestFlag = 0;
	m_bQRGenerated = FALSE;
    m_dwClickUpCount = 0;
	m_bOnRequestMcuUpdate = FALSE;
    m_FactoryActivate = FALSE;
    m_dwLastTouchTick = 0;
}
CPanelSettingAbout::~CPanelSettingAbout(void)
{

}

BOOL CPanelSettingAbout::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingAbout::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingAbout::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
    m_FactoryActivate = FALSE;
	//m_btnFactoryMode.Show(FALSE);
}

void CPanelSettingAbout::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

	m_tagTitleOSVersion.SetAlpha(m_fCurAlpha);
	m_tagTitleOSVersion.Render();
	m_tagValueOSVersion.SetAlpha(m_fCurAlpha);
	m_tagValueOSVersion.Render();

	m_tagTitleAppVersion.SetAlpha(m_fCurAlpha);
	m_tagTitleAppVersion.Render();
	m_tagValueAppVersion.SetAlpha(m_fCurAlpha);
	m_tagValueAppVersion.Render();
	m_btnUpdateNK.SetAlpha(m_fCurAlpha);
	m_btnUpdateNK.Render();

	m_tagTitleMCUVersion.SetAlpha(m_fCurAlpha);
	m_tagTitleMCUVersion.Render();
	m_tagValueMCUVersion.SetAlpha(m_fCurAlpha);
	m_tagValueMCUVersion.Render();
	m_btnUpdateMCU.SetAlpha(m_fCurAlpha);
	m_btnUpdateMCU.Render();

	m_tagTitleHardwareVersion.SetAlpha(m_fCurAlpha);
	m_tagTitleHardwareVersion.Render();
	m_tagValueHardwareVersion.SetAlpha(m_fCurAlpha);
	m_tagValueHardwareVersion.Render();

	m_tagTitleHardwareID.SetAlpha(m_fCurAlpha);
	m_tagTitleHardwareID.Render();
	m_tagValueHardwareID.SetAlpha(m_fCurAlpha);
	m_tagValueHardwareID.Render();

#if CVTE_EN_CPMAC
	m_tagTitleCarPlayMAC.SetAlpha(m_fCurAlpha);
	m_tagTitleCarPlayMAC.Render();
	m_tagValueCarPlayMAC.SetAlpha(m_fCurAlpha);
	m_tagValueCarPlayMAC.Render();
#endif

	//
	m_tagQRBarBk.SetAlpha(m_fCurAlpha);
	m_tagQRBarBk.Render();
	m_QRBar.SetAlpha(m_fCurAlpha);
	m_QRBar.Render();

    m_btnFactoryMode.SetAlpha(m_fCurAlpha);
    m_btnFactoryMode.Render();

    m_btnFactoryActivate.Render();
    
	EndRender();
}

BOOL CPanelSettingAbout::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

    const int box_size_x=LIST_BOX_ITEM_WIDTH;

	//large size:

//	    if(m_pSkinManager->GetConfig()->GetCurParam()->bActiveFactory)
//	    {
//		    SetSize(g_iClientWidth-box_size_x,g_iClientHeight+505+140);
//	    }
//	    else
//	    {
	int Height_offset = 0;
#if CVTE_EN_CPMAC
	Height_offset += 130;
#endif
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		SetSize(g_iClientWidth-box_size_x,g_iClientHeight+800+Height_offset);
#else
		SetSize(g_iClientWidth-box_size_x,g_iClientHeight+635+Height_offset);
#endif
//	    }

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)


	HANDLE hOem = AppProxyOpen();
	DWORD dwUUID[4] = {0};
	wchar_t wcUUID[MAX_PATH] = {0};
	wchar_t wcSWID[MAX_PATH] = {0};

	if(hOem == INVALID_HANDLE_VALUE || hOem == NULL)
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: hOem is invalid\r\n"),TEXT(__FUNCTION__)));
		m_strOSVersion = _T("WCE6.0-R3-131231");
	}
	else
	{
		//
		//SWID
		//
		if(AppProxyGetSWID(hOem,wcSWID,sizeof(wcSWID)))
			m_strOSVersion = wcSWID;
		else
			m_strOSVersion = _T("WCE6.0-R3-131231");

		RETAILMSG(1,(TEXT("MSG:[%s] OSVersion[%s]\r\n"),TEXT(__FUNCTION__), m_strOSVersion.String()));

		//
		//UUID
		//
		RETAILMSG(1,(TEXT("MSG:[%s] AppProxyGetUUID ++ \r\n"),TEXT(__FUNCTION__)));
		AppProxyGetUUID(hOem,dwUUID,16);
		AppProxyClose(hOem);
		RETAILMSG(1,(TEXT("MSG:[%s] AppProxyClose \r\n"),TEXT(__FUNCTION__)));
	}
	swprintf_s(wcUUID, MAX_PATH, L"%04X-%04X-%04X-%04X-%04X-%04X-%04X-%04X", HIWORD(dwUUID[0]),LOWORD(dwUUID[0]),HIWORD(dwUUID[1]),LOWORD(dwUUID[1]),HIWORD(dwUUID[2]),LOWORD(dwUUID[2]),HIWORD(dwUUID[3]),LOWORD(dwUUID[3]));

	RETAILMSG(1,(TEXT("MSG:[%s] wcUUID[%s]\r\n"),TEXT(__FUNCTION__),wcUUID));
	m_strHardwareID = wcUUID;
	RETAILMSG(1,(TEXT("MSG:[%s] m_strHardwareID[%s]\r\n"),TEXT(__FUNCTION__),m_strHardwareID.String()));

	//WCHAR strVID[MAX_PATH];
	//WCHAR strHID[MAX_PATH];
	//swprintf_s(strVID,MAX_PATH,_T("C%d_%s.%d"),CUSTOMER_ID,_T(CVTE_DEF_APP_VERSION),CVTE_DEF_RELEASE_VERSION_NUM);
	//swprintf_s(strHID,MAX_PATH,_T("H%d_%s"),CVTE_MODEL_TYPE,_T(CVTE_DEF_HARDWARE_VERSION));
	m_strAPPVersion = CVTE_DEF_APP_VERSION_WIDE_STRING;
	m_strMCUVersion = _T("V1.0.0");
	m_strHardwareVersion = CVTE_DEF_HARDWARE_VERSION_WIDE_STRING;
#if CVTE_EN_CPMAC
	m_strCarPlayMAC = CCarplayAdapter::GetInstance()->GetMAC();
	RETAILMSG(1,(TEXT("MSG:[%s] m_strCarPlayMAC[%s]\r\n"),TEXT(__FUNCTION__),m_strCarPlayMAC.String()));
#endif

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

	GLfloat py=g_iClientHeight/2-40;

	SIZE siTitle={cx,gridHeight};

	m_tagBk.Initialize(0,this,
	m_pSkinManager->GetTexture(TEXID_065),NULL,
	m_pSkinManager->GetTexture(TEXID_065),NULL,
	NULL,NULL,
	NULL,NULL);
	m_tagBk.SetSize(cx, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);

	//os:
	m_tagTitleOSVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleOSVersion.SetSize(cx,gridHeight);
	m_tagTitleOSVersion.SetCaption(GetResourceString(IDS_SETTING_OS_VERSION));
	m_tagTitleOSVersion.LinkTextGL(pTextGL);
	m_tagTitleOSVersion.SetPos(0,py);
	m_tagTitleOSVersion.SetCaptionHeight(caption_h);
	m_tagTitleOSVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleOSVersion.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	m_tagValueOSVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueOSVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueOSVersion.SetCaption(m_strOSVersion.String());
	m_tagValueOSVersion.LinkTextGL(pTextGL);
	m_tagValueOSVersion.SetPos(margin_left/2, py);
	m_tagValueOSVersion.SetCaptionHeight(caption_h);
	m_tagValueOSVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagValueOSVersion.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	//app:
	m_tagTitleAppVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleAppVersion.SetSize(cx,gridHeight);
	m_tagTitleAppVersion.SetCaption(GetResourceString(IDS_SETTING_SOFTWARE_VERSION));
	m_tagTitleAppVersion.LinkTextGL(pTextGL);
	m_tagTitleAppVersion.SetPos(0,py);
	m_tagTitleAppVersion.SetCaptionHeight(caption_h);
	m_tagTitleAppVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleAppVersion.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	m_tagValueAppVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueAppVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueAppVersion.SetCaption(m_strAPPVersion.String());
	m_tagValueAppVersion.LinkTextGL(pTextGL);
	m_tagValueAppVersion.SetPos(margin_left/2,py);
	m_tagValueAppVersion.SetCaptionHeight(caption_h);
	m_tagValueAppVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagValueAppVersion.SetCaptionColor(&cr_gray, &cr_gray);

	m_btnUpdateNK.Initialize(IDC_SETTING_BTN_UPDATE,this,
		m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUpdateNK.SetSize(150,60);
	m_btnUpdateNK.SetCaption(GetResourceString(IDS_UPDATE));
	m_btnUpdateNK.LinkTextGL(pTextGL);
	m_btnUpdateNK.SetPos(m_siCurSize.cx/2-100, py);
	m_btnUpdateNK.SetCaptionHeight(caption_h);
	m_btnUpdateNK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUpdateNK.SetCaptionColor(&cr_dark,&cr_gray);
	m_btnUpdateNK.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	m_btnUpdateNK.Show(IsFileExists(UPDATE_FILE_IMAGE_USBDISK));

	py-=tag_h;

	//mcu:
	m_tagTitleMCUVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleMCUVersion.SetSize(cx, gridHeight);
	m_tagTitleMCUVersion.SetCaption(GetResourceString(IDS_SETTING_MCU_VERSION));
	m_tagTitleMCUVersion.LinkTextGL(pTextGL);
	m_tagTitleMCUVersion.SetPos(0,py);
	m_tagTitleMCUVersion.SetCaptionHeight(caption_h);
	m_tagTitleMCUVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleMCUVersion.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	m_tagValueMCUVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueMCUVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueMCUVersion.SetCaption(m_strMCUVersion.String());
	m_tagValueMCUVersion.LinkTextGL(pTextGL);
	m_tagValueMCUVersion.SetPos(margin_left/2,py);
	m_tagValueMCUVersion.SetCaptionHeight(caption_h);
	m_tagValueMCUVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagValueMCUVersion.SetCaptionColor(&cr_gray, &cr_gray);

	m_btnUpdateMCU.Initialize(IDC_SETTING_BTN_UPDATE_MCU,this,
		m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUpdateMCU.SetSize(150,60);
	m_btnUpdateMCU.SetCaption(GetResourceString(IDS_UPDATE));
	m_btnUpdateMCU.LinkTextGL(pTextGL);
	m_btnUpdateMCU.SetPos(m_siCurSize.cx/2-100,py);
	m_btnUpdateMCU.SetCaptionHeight(caption_h);
	m_btnUpdateMCU.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUpdateMCU.SetCaptionColor(&cr_dark,&cr_gray);
	m_btnUpdateMCU.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	m_btnUpdateMCU.Show(IsFileExists(UPDATE_FILE_MCU_USBDISK));

	py-=tag_h;

	//hardware:
	m_tagTitleHardwareVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleHardwareVersion.SetSize(cx,gridHeight);
	m_tagTitleHardwareVersion.SetCaption(GetResourceString(IDS_SETTING_HARDWARE_VERSION));
	m_tagTitleHardwareVersion.LinkTextGL(pTextGL);
	m_tagTitleHardwareVersion.SetPos(0,py);
	m_tagTitleHardwareVersion.SetCaptionHeight(caption_h);
	m_tagTitleHardwareVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleHardwareVersion.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	m_tagValueHardwareVersion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueHardwareVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueHardwareVersion.SetCaption(m_strHardwareVersion.String());
	m_tagValueHardwareVersion.LinkTextGL(pTextGL);
	m_tagValueHardwareVersion.SetPos(margin_left/2,py);
	m_tagValueHardwareVersion.SetCaptionHeight(caption_h);
	m_tagValueHardwareVersion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagValueHardwareVersion.SetCaptionColor(&cr_gray, &cr_gray);

	m_btnFactoryMode.Initialize(IDC_SETTING_BTN_FACTORY_MODE,this,
		m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnFactoryMode.SetSize(150,60);
	m_btnFactoryMode.SetCaption(_T("工厂模式"));
	m_btnFactoryMode.LinkTextGL(pTextGL);
	m_btnFactoryMode.SetPos(m_siCurSize.cx/2-100,py);
	m_btnFactoryMode.SetCaptionHeight(caption_h);
	m_btnFactoryMode.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnFactoryMode.SetCaptionColor(&cr_dark,&cr_gray);
	m_btnFactoryMode.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	m_btnFactoryMode.Show(m_pSkinManager->GetConfig()->GetCurParam()->bActiveFactory || m_FactoryActivate);

	py-=tag_h;

	//hardware id:
	m_tagTitleHardwareID.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleHardwareID.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleHardwareID.SetCaption(_T("Hardware ID"));
	m_tagTitleHardwareID.LinkTextGL(pTextGL);
	m_tagTitleHardwareID.SetPos(0,py);
	m_tagTitleHardwareID.SetCaptionHeight(caption_h);
	m_tagTitleHardwareID.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleHardwareID.SetCaptionColor(&cr_gray);

	py-=tag_h;
	m_tagValueHardwareID.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagValueHardwareID.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueHardwareID.SetCaption(m_strHardwareID.String());
	m_tagValueHardwareID.LinkTextGL(pTextGL);
	m_tagValueHardwareID.SetPos(margin_left/2,py);
	m_tagValueHardwareID.SetCaptionHeight(caption_h*0.8);
	m_tagValueHardwareID.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagValueHardwareID.SetCaptionColor(&cr_gray, &cr_gray);

	py-=(220+45);

	//qr:
	m_tagQRBarBk.Initialize(0,this,
		m_pSkinManager->GetTexture(/*TEXID_289*/TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagQRBarBk.SetSize(/*((g_iClientWidth-box_size_x)-margin_x*2)*/400,400);
	m_tagQRBarBk.SetPos(0,py);

	m_btnFactoryActivate.Initialize(IDC_SETTING_BTN_FACTORY_ACTIVATE,this,NULL,NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnFactoryActivate.SetSize(300,300);
	m_btnFactoryActivate.SetPos(0,py);
	m_btnFactoryActivate.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	m_btnFactoryActivate.EnableDrawBack(FALSE);

#else
	//RETAILMSG(1,(TEXT("####################### 1 ########################\r\n")));
	HANDLE hOem = AppProxyOpen();
	DWORD dwUUID[4] = {0};
	wchar_t wcUUID[MAX_PATH] = {0};
	wchar_t wcSWID[MAX_PATH] = {0};

	if(hOem == INVALID_HANDLE_VALUE || hOem == NULL)
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: hOem is invalid\r\n"),TEXT(__FUNCTION__)));
		m_strOSVersion = _T("WCE6.0-R3-131231");
	}
	else
	{
		//
		//SWID
		//
		if(AppProxyGetSWID(hOem,wcSWID,sizeof(wcSWID)))
			m_strOSVersion = wcSWID;
		else
			m_strOSVersion = _T("WCE6.0-R3-131231");

		RETAILMSG(1,(TEXT("MSG:[%s] OSVersion[%s]\r\n"),TEXT(__FUNCTION__), m_strOSVersion.String()));

		//
		//UUID
		//
		RETAILMSG(1,(TEXT("MSG:[%s] AppProxyGetUUID ++ \r\n"),TEXT(__FUNCTION__)));
		AppProxyGetUUID(hOem,dwUUID,16);
		//RETAILMSG(1,(TEXT("MSG:[%s] AppProxyGetUUID -- [%08X-%08X-%08X-%08X]\r\n"),TEXT(__FUNCTION__),dwUUID[0],dwUUID[1],dwUUID[2],dwUUID[3]));
		AppProxyClose(hOem);
		RETAILMSG(1,(TEXT("MSG:[%s] AppProxyClose \r\n"),TEXT(__FUNCTION__)));
	}
	swprintf_s(wcUUID, MAX_PATH, L"%04X-%04X-%04X-%04X-%04X-%04X-%04X-%04X", HIWORD(dwUUID[0]),LOWORD(dwUUID[0]),HIWORD(dwUUID[1]),LOWORD(dwUUID[1]),HIWORD(dwUUID[2]),LOWORD(dwUUID[2]),HIWORD(dwUUID[3]),LOWORD(dwUUID[3]));

	RETAILMSG(1,(TEXT("MSG:[%s] wcUUID[%s]\r\n"),TEXT(__FUNCTION__),wcUUID));
	m_strHardwareID = wcUUID;
	RETAILMSG(1,(TEXT("MSG:[%s] m_strHardwareID[%s]\r\n"),TEXT(__FUNCTION__),m_strHardwareID.String()));

	//WCHAR strVID[MAX_PATH];
	//WCHAR strHID[MAX_PATH];
	//swprintf_s(strVID,MAX_PATH,_T("C%d_%s.%d"),CUSTOMER_ID,_T(CVTE_DEF_APP_VERSION),CVTE_DEF_RELEASE_VERSION_NUM);
	//swprintf_s(strHID,MAX_PATH,_T("H%d_%s"),CVTE_MODEL_TYPE,_T(CVTE_DEF_HARDWARE_VERSION));
	m_strAPPVersion = CVTE_DEF_APP_VERSION_WIDE_STRING;
	m_strMCUVersion = _T("V1.0.0");
	m_strHardwareVersion = CVTE_DEF_HARDWARE_VERSION_WIDE_STRING;
#if CVTE_EN_CPMAC
	m_strCarPlayMAC = CCarplayAdapter::GetInstance()->GetMAC();
	RETAILMSG(1,(TEXT("MSG:[%s] m_strCarPlayMAC[%s]\r\n"),TEXT(__FUNCTION__),m_strCarPlayMAC.String()));
#endif

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x=12;
	int cx=(g_iClientWidth-box_size_x-margin_x*2)/2;
	//int cy=cx;
	int tag_h = LIST_BOX_TAG_HEIGHT;
	int caption_h = 22;
	int caption2_h = 18;

	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
	{
		caption_h = 20;
	}

	GLfloat py=g_iClientHeight/2-40;

	SIZE siTitle={cx,40};

	//
	// 	m_QRBar.Initialize(0,this,pSkinManager,"Hello!nelSettingAbout::Initialize(UINT id,CPanelGL *pParen");
	// 	m_QRBar.SetShowScale(2.0f);
	// 	m_QRBar.SetPos((g_iClientWidth-box_size_x)/2-170,0);

	//		m_tagQRBarBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	//		m_tagQRBarBk.SetSize(100,200);  //(400,485)
	//		m_tagQRBarBk.SetPos((g_iClientWidth-box_size_x)/2-210,-2.5f);


	//os:
	m_tagTitleOSVersion.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleOSVersion.SetSize(cx,40);
	m_tagTitleOSVersion.SetCaption(GetResourceString(IDS_SETTING_OS_VERSION));
	m_tagTitleOSVersion.LinkTextGL(pTextGL);
	m_tagTitleOSVersion.SetPos(-cx/2-10 +12,py);
	m_tagTitleOSVersion.SetCaptionHeight(20);
	m_tagTitleOSVersion.EnableDrawBack(FALSE);
	m_tagTitleOSVersion.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleOSVersion.SetCaptionColor(&cr_dark);

	py-=60;

	m_tagValueOSVersion.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueOSVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueOSVersion.SetCaption(m_strOSVersion.String());
	m_tagValueOSVersion.LinkTextGL(pTextGL);
	m_tagValueOSVersion.SetPos(0,py);
	m_tagValueOSVersion.SetCaptionHeight(caption_h);
	m_tagValueOSVersion.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagValueOSVersion.SetCaptionColor(&cr_gray);

	py-=70;

	//app:
	m_tagTitleAppVersion.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAppVersion.SetSize(cx,40);
	m_tagTitleAppVersion.SetCaption(GetResourceString(IDS_SETTING_SOFTWARE_VERSION));
	m_tagTitleAppVersion.LinkTextGL(pTextGL);
	m_tagTitleAppVersion.SetPos(-cx/2-10 +12,py);
	m_tagTitleAppVersion.SetCaptionHeight(20);
	m_tagTitleAppVersion.EnableDrawBack(FALSE);
	m_tagTitleAppVersion.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleAppVersion.SetCaptionColor(&cr_dark);

	py-=60;


	m_tagValueAppVersion.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueAppVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueAppVersion.SetCaption(m_strAPPVersion.String());
	m_tagValueAppVersion.LinkTextGL(pTextGL);
	m_tagValueAppVersion.SetPos(0,py);
	m_tagValueAppVersion.SetCaptionHeight(caption_h);
	m_tagValueAppVersion.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagValueAppVersion.SetCaptionColor(&cr_gray);

	m_btnUpdateNK.Initialize(IDC_SETTING_BTN_UPDATE,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUpdateNK.SetSize(150,60);
	m_btnUpdateNK.SetCaption(GetResourceString(IDS_UPDATE));
	m_btnUpdateNK.LinkTextGL(pTextGL);
	m_btnUpdateNK.SetPos(cx-80,py);
	m_btnUpdateNK.SetCaptionHeight(caption2_h);
	m_btnUpdateNK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUpdateNK.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUpdateNK.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	//m_btnUpdateNK.SetShowScale(1.2f);
	m_btnUpdateNK.Show(IsFileExists(UPDATE_FILE_IMAGE_USBDISK));


	py-=70;

	//mcu:
	m_tagTitleMCUVersion.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleMCUVersion.SetSize(cx,40);
	m_tagTitleMCUVersion.SetCaption(GetResourceString(IDS_SETTING_MCU_VERSION));
	m_tagTitleMCUVersion.LinkTextGL(pTextGL);
	m_tagTitleMCUVersion.SetPos(-cx/2-10 +12,py);
	m_tagTitleMCUVersion.SetCaptionHeight(20);
	m_tagTitleMCUVersion.EnableDrawBack(FALSE);
	m_tagTitleMCUVersion.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleMCUVersion.SetCaptionColor(&cr_dark);

	py-=60;

	m_tagValueMCUVersion.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueMCUVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueMCUVersion.SetCaption(m_strMCUVersion.String());
	m_tagValueMCUVersion.LinkTextGL(pTextGL);
	m_tagValueMCUVersion.SetPos(0,py);
	m_tagValueMCUVersion.SetCaptionHeight(caption_h);
	m_tagValueMCUVersion.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagValueMCUVersion.SetCaptionColor(&cr_gray);

	m_btnUpdateMCU.Initialize(IDC_SETTING_BTN_UPDATE_MCU,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUpdateMCU.SetSize(150,60);
	m_btnUpdateMCU.SetCaption(GetResourceString(IDS_UPDATE));
	m_btnUpdateMCU.LinkTextGL(pTextGL);
	m_btnUpdateMCU.SetPos(cx-80,py);
	m_btnUpdateMCU.SetCaptionHeight(caption2_h);
	m_btnUpdateMCU.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUpdateMCU.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUpdateMCU.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	m_btnUpdateMCU.Show(IsFileExists(UPDATE_FILE_MCU_USBDISK));

	py-=70;

	//hardware:
	m_tagTitleHardwareVersion.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleHardwareVersion.SetSize(cx,40);
	m_tagTitleHardwareVersion.SetCaption(GetResourceString(IDS_SETTING_HARDWARE_VERSION));
	m_tagTitleHardwareVersion.LinkTextGL(pTextGL);
	m_tagTitleHardwareVersion.SetPos(-cx/2-10 +12,py);
	m_tagTitleHardwareVersion.SetCaptionHeight(20);
	m_tagTitleHardwareVersion.EnableDrawBack(FALSE);
	m_tagTitleHardwareVersion.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleHardwareVersion.SetCaptionColor(&cr_dark);

	py-=60;

	m_tagValueHardwareVersion.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueHardwareVersion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueHardwareVersion.SetCaption(m_strHardwareVersion.String());
	m_tagValueHardwareVersion.LinkTextGL(pTextGL);
	m_tagValueHardwareVersion.SetPos(0,py);
	m_tagValueHardwareVersion.SetCaptionHeight(caption_h);
	m_tagValueHardwareVersion.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagValueHardwareVersion.SetCaptionColor(&cr_gray);

	m_btnFactoryMode.Initialize(IDC_SETTING_BTN_FACTORY_MODE,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnFactoryMode.SetSize(150,60);
	m_btnFactoryMode.SetCaption(_T("工厂模式"));
	m_btnFactoryMode.LinkTextGL(pTextGL);
	m_btnFactoryMode.SetPos(cx-80,py);
	m_btnFactoryMode.SetCaptionHeight(caption2_h);
	m_btnFactoryMode.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnFactoryMode.SetCaptionColor(&cr_w,&cr_gray);
	m_btnFactoryMode.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	m_btnFactoryMode.Show(m_pSkinManager->GetConfig()->GetCurParam()->bActiveFactory || m_FactoryActivate);

	py-=70;

	//hardware id:
	m_tagTitleHardwareID.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleHardwareID.SetSize(cx,40);
	m_tagTitleHardwareID.SetCaption(GetResourceString(IDS_SETTING_HARDWARE_ID));
	m_tagTitleHardwareID.LinkTextGL(pTextGL);
	m_tagTitleHardwareID.SetPos(-cx/2-10 +12,py);
	m_tagTitleHardwareID.SetCaptionHeight(20);
	m_tagTitleHardwareID.EnableDrawBack(FALSE);
	m_tagTitleHardwareID.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleHardwareID.SetCaptionColor(&cr_dark);

	py-=60;
	m_tagValueHardwareID.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueHardwareID.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueHardwareID.SetCaption(m_strHardwareID.String());
	m_tagValueHardwareID.LinkTextGL(pTextGL);
	m_tagValueHardwareID.SetPos(0,py);
	m_tagValueHardwareID.SetCaptionHeight(caption_h-2);
	m_tagValueHardwareID.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagValueHardwareID.SetCaptionColor(&cr_gray);

#if CVTE_EN_CPMAC
	py-=70;

	//CarPlay MAC:
	m_tagTitleCarPlayMAC.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleCarPlayMAC.SetSize(cx,40);
	m_tagTitleCarPlayMAC.SetCaption(_T("CarPlay MAC"));
	m_tagTitleCarPlayMAC.LinkTextGL(pTextGL);
	m_tagTitleCarPlayMAC.SetPos(-cx/2-10 +12,py);
	m_tagTitleCarPlayMAC.SetCaptionHeight(20);
	m_tagTitleCarPlayMAC.EnableDrawBack(FALSE);
	m_tagTitleCarPlayMAC.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleCarPlayMAC.SetCaptionColor(&cr_dark);

	py-=60;
	m_tagValueCarPlayMAC.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueCarPlayMAC.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagValueCarPlayMAC.SetCaption(m_strCarPlayMAC.String());
	m_tagValueCarPlayMAC.LinkTextGL(pTextGL);
	m_tagValueCarPlayMAC.SetPos(0,py);
	m_tagValueCarPlayMAC.SetCaptionHeight(caption_h);
	m_tagValueCarPlayMAC.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagValueCarPlayMAC.SetCaptionColor(&cr_gray);
#endif

	py-=(220+45);

	//qr:

	m_tagQRBarBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagQRBarBk.SetSize(((g_iClientWidth-box_size_x)-margin_x*2),440);
	m_tagQRBarBk.SetPos(0,py);

	m_btnFactoryActivate.Initialize(IDC_SETTING_BTN_FACTORY_ACTIVATE,this,NULL,NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnFactoryActivate.SetSize(300,300);
	m_btnFactoryActivate.SetPos(0,py);
	m_btnFactoryActivate.SetNotifyFunc(&CPanelSettingAbout::OnNotify,this);
	m_btnFactoryActivate.EnableDrawBack(FALSE);

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET


	return TRUE;
}


void CALLBACK CPanelSettingAbout::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingAbout *panel=(CPanelSettingAbout*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingAbout::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BTN_UPDATE:
		{
			PrepareUpdateOS();
			SwitchPage(UI_CLASS_UPDATE_ARM,PAGE_UPDATE_MAIN,PAGE_SWITCH_ANI_NONE,1);
			PlaySoundEffect(0);
		}
		break;
#if 0
	case IDC_SETTING_BTN_UPDATE_MCU:
		{
			SwitchPage(UI_CLASS_UPDATE_ARM,PAGE_UPDATE_MAIN,PAGE_SWITCH_ANI_NONE,1);
			PlaySoundEffect(0);
		}
		break;
#else
  case IDC_SETTING_BTN_UPDATE_MCU:
		{
			//check update file is valid:
			BOOL bUpdateFileValid = FALSE;

			bUpdateFileValid = IsValidMcuUpdateFile(UPDATE_FILE_MCU_USBDISK,UPDATE_FILE_MCU_CKECKSUM_USBDISK);

			if(bUpdateFileValid)
			{
				//show confirm box:
				ShowNotificationBox(m_hMainWnd,IDC_SETTING_BOX_UPDATE_MCU_CONFIRM,
					&CPanelSettingAbout::OnNotify,
					this,
					0,
					GetResourceString(IDS_MCU_UPDATE_CONFIRM),
					NULL,//GetResourceString(IDS_MCU_UPDATE_PROMPT),
					0,
					FALSE);
			}
			else
			{
				ShowNotificationBox(m_hMainWnd,IDC_SETTING_BOX_UPDATE_MCU_CANCEL,
					&CPanelSettingAbout::OnNotify,
					this,
					0,
					GetResourceString(IDS_MCU_UPDATING_INVALID_FILE),
					//NULL,//GetResourceString(IDS_MCU_UPDATE_PROMPT),
					_T("File does not exist or the file is corrupt"),
					0,
					FALSE);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BOX_UPDATE_MCU_CANCEL:
		{
			HideNotificationBox(m_hMainWnd);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BOX_UPDATE_MCU_CONFIRM:
		{
			if(wMsg == NB_OK)//
			{
				//request update:
				if(m_pSkinManager->GetConfig()->GetCurParam()->iPowerStatusBeforeACCOff == 1)
				{
					HANDLE hOPD = AppProxyOpen();
					BYTE nPowerStatus = 0;
					if(hOPD)
					{
						RETAILMSG(1, (TEXT("MSG:[%s]: =>AppProxySetPowerStatus [0].\r\n"), TEXT(__FUNCTION__)));
						AppProxySetPowerStatus(hOPD,&nPowerStatus,sizeof(nPowerStatus));
						AppProxyClose(hOPD);
					}
				}

				RequestUpdateMCU(TRUE);
			}
			else if(wMsg == NB_CANCEL)//
			{
				//cancel update:
				//RequestUpdateMCU(FALSE);
				HideNotificationBox(m_hMainWnd);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BOX_UPDATE_MCU_CONFIRM+1:
		{
			if(wMsg == NB_OK)//
			{
				//reset:
				::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_HARD_RESET,NULL);
			}
			else if(wMsg == NB_CANCEL)//
			{
				RequestUpdateMCU(FALSE);
				HideNotificationBox(m_hMainWnd);
			}
			PlaySoundEffect(0);
		}
		break;
#endif
    case IDC_SETTING_BTN_FACTORY_ACTIVATE:
        {
            if(!m_FactoryActivate && (GetTickCount()-m_dwLastTouchTick)<3000)
            {
                m_dwClickUpCount++;
                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_SOFTWARE_VERSION [%d]\r\n"),m_dwClickUpCount));

                if(m_dwClickUpCount>=9)
                {
                    m_dwClickUpCount=0;
                    RETAILMSG(1, (TEXT("APP:MSG: Activate Engineering Mode\r\n")));
                    m_FactoryActivate = TRUE;
                    m_btnFactoryMode.Show(TRUE);
                    PlaySoundEffect(0);
                }
            }
            else
            {
                if(m_dwClickUpCount)
                    m_dwClickUpCount=0;
            }
            m_dwLastTouchTick = GetTickCount();
        }
        break;
    case IDC_SETTING_BTN_FACTORY_MODE:
        {
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_FACTORY);
            m_pSkinManager->GetConfig()->GetCurParam()->bActiveFactory = TRUE;
            PlaySoundEffect(0);
        }
        break;
    default:
        break;
	}

}

void CPanelSettingAbout::GenerateQR()
{
	if(!m_bQRGenerated)
	{
		m_bQRGenerated = TRUE;

		wchar_t buff_w[1024];
		char buff_s[1024];

		swprintf_s(buff_w,1024,_T("DeviceID: %s;\r\nOS Version: %s;\r\nSoftware Version: %s;\r\nMCU Version: %s;\r\nHardware Version: %s;\r\n"),
			m_strHardwareID.String(),m_strOSVersion.String(),m_strAPPVersion.String(),m_strMCUVersion.String(),m_strHardwareVersion.String());

		WideCharToMultiByte(0, 0, buff_w, _tcslen(buff_w), (LPSTR)buff_s, sizeof(buff_s), NULL, NULL);

		m_QRBar.Initialize(0,this,m_pSkinManager,buff_s);

		SIZE si=m_QRBar.GetQRSize();

		if(si.cx)
		{
			m_QRBar.SetShowScale(360.0f/si.cx);
		}
		m_QRBar.SetPos(0,m_tagQRBarBk.GetPosY());
	}

}

void CPanelSettingAbout::OnDeviceChanged()
{
	RETAILMSG(1,(_T("APP:MSG:GUI:Main: [CPanelSettingAbout::OnDeviceChanged] \r\n")));
	BOOL bNKShow;
	#if CVTE_EN_PACKAGE_UPDATE
	bNKShow = IsFileExists(UPDATE_FILE_PACKAGE_USBDISK);	
	#else
	bNKShow = (IsFileExists(UPDATE_FILE_UPDATE_TEXT) || IsFileExists(UPDATE_FILE_IMAGE_USBDISK) || IsFileExists(UPDATE_FILE_UI_USBDISK));
	#endif
	m_btnUpdateMCU.Show(IsFileExists(UPDATE_FILE_MCU_USBDISK));
	m_btnUpdateNK.Show(bNKShow);

}

#if CVTE_EN_CPMAC
void CPanelSettingAbout::OnMACChanged()
{
	m_strCarPlayMAC = CCarplayAdapter::GetInstance()->GetMAC();
	RETAILMSG(1,(TEXT("MSG:[%s] m_strCarPlayMAC[%s]\r\n"),TEXT(__FUNCTION__),m_strCarPlayMAC.String()));
	m_tagValueCarPlayMAC.SetCaption(m_strCarPlayMAC.String());
}
#endif

void CPanelSettingAbout::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

	// 	wchar_t str[32];

	if(dwType == 0)//package
	{
		BYTE ver=pCommander->GetItemData(MCU_MAIN_VERSION);

		m_dwMCUVersion = ver;

		wchar_t str[32];
		swprintf_s(str,32,_T("V%d.%d.%d"),ver/100,(ver-(ver/100)*100)/10,ver-(ver/10)*10);
		m_strMCUVersion = str;
		m_tagValueMCUVersion.SetCaption(str);

		GenerateQR();
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
		case MCU_UPDATE_STATUS:
			{
				BOOL bUpdate=pCommander->GetItemData(MCU_UPDATE_STATUS);

				RETAILMSG(1,(_T(">>>MCU_UPDATE_STATUS :%d\r\n"),bUpdate));

				if(bUpdate==0)//MCU refused update
				{
					if(	m_bOnRequestMcuUpdate )
					{
						m_bOnRequestMcuUpdate = FALSE;
						if(m_dwUpdateMcuRequestFlag != bUpdate)
						{
							//request failed:
							::PostMessage(m_hMainWnd,WM_MCU_UPDATE_MSG,UPDATE_MCU_REQUST_FAILED,0);
							OnRequestUpdateMCUFailed();
						}
						else
						{
							OnRequestUpdateMCUSucceed();
						}
					}
				}
				else if(bUpdate==1)//MCU agreed update
				{
					if(	m_bOnRequestMcuUpdate )
					{
						m_bOnRequestMcuUpdate = FALSE;
						if(m_dwUpdateMcuRequestFlag != bUpdate)
						{
							//request failed:
							::PostMessage(m_hMainWnd,WM_MCU_UPDATE_MSG,UPDATE_MCU_REQUST_FAILED,0);
							OnRequestUpdateMCUFailed();
						}
						else
						{
							OnRequestUpdateMCUSucceed();
						}
					}
				}
				else if(bUpdate==2) //MCU ready to get data
				{
					SwitchPage(UI_CLASS_UPDATE,0,PAGE_SWITCH_ANI_NONE,1);
				}
			}
			break;
		}
	}
}

void CPanelSettingAbout::RequestUpdateMCU(BOOL bUpdate)
{
	m_dwUpdateMcuRequestFlag = bUpdate;

	m_bOnRequestMcuUpdate = TRUE;

	RETAILMSG(DEBUG_SETTING,(_T("###### RequestUpdateMCU : %d\r\n"),bUpdate));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_UPDATE_STATUS,bUpdate);
	}
}
void CPanelSettingAbout::OnRequestUpdateMCUSucceed()
{
	RETAILMSG(DEBUG_SETTING,(_T("###### OnRequestUpdateMCUSucceed bUpdate:%d\r\n"),m_dwUpdateMcuRequestFlag));

	if(m_dwUpdateMcuRequestFlag == 1)
	{
		//reset:
		m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC=MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_MAIN);
		m_pSkinManager->GetConfig()->GetCurParam()->bMCUSoftwareUpdateFlag = TRUE;
		m_pSkinManager->GetConfig()->SaveConfig();

		//
		ShowNotificationBox(m_hMainWnd,IDC_SETTING_BOX_UPDATE_MCU_CONFIRM+1,
			&CPanelSettingAbout::OnNotify,
			this,
			-1,
			GetResourceString(IDS_MCU_UPDATE_CONFIRM),
			GetResourceString(IDS_MCU_UPDATE_PROMPT),
			0,
			FALSE);
	}
	else
	{
		//reset:
		//m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC=MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_MAIN);
		m_pSkinManager->GetConfig()->GetCurParam()->bMCUSoftwareUpdateFlag = FALSE;
		m_pSkinManager->GetConfig()->SaveConfig();

	}

}
void CPanelSettingAbout::OnRequestUpdateMCUFailed()
{
	RETAILMSG(DEBUG_SETTING,(_T("###### OnRequestUpdateMCUFailed \r\n")));

}

BOOL CPanelSettingAbout::IsFileExists(LPCTSTR   lpszFileName)
{
	//
	WIN32_FIND_DATA   wfd;
	HANDLE   hFind;
	BOOL   bRet;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   (hFind   !=   INVALID_HANDLE_VALUE);
	FindClose(hFind);
	return   bRet;
}
void CPanelSettingAbout::SetNcmMacAdress()
{
	//MAC: [74-72-B0-FF-D8-F0]---[74-72-B0-FF-FF-FF]
	CReg reg;
	BYTE buf[6]={0x74,0x72,0xB0,0xFF,0x00,0x00};
	LARGE_INTEGER		t;
	wchar_t strMac[32];
	reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\USB\\FunctionDrivers\\ncmfn\\"));
	LPCTSTR mac=reg.ValueSZ(_T("MacAddress"));
	if(!mac)
	{
		//generate random bytes:
		srand(GetTickCount());
		for (int i=0;i<2;i++)
		{
			Sleep(abs(rand()%10));
			if( QueryPerformanceCounter( &t ) )
			{
				buf[i+4] = (t.QuadPart%255);
			}
			else
			{
				buf[i+4] = (rand()%255);
			}
		}

		buf[4]=buf[4] % (0xFF-0xD8+0x01) + 0xD8;
		buf[5]=buf[5] % (0xFF-0xF0+0x01) + 0xF0;
		swprintf_s(strMac,32,_T("%02X%02X%02X%02X%02X%02X"),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
		RETAILMSG(1,(_T("==SetNcmMacAdress: %s \r\n"),strMac));
		reg.SetSZ(_T("MacAddress"),strMac);
	}
	else
	{
		RETAILMSG(1,(_T("==NcmMacAdress = %s \r\n"),mac));
	}
}
void CPanelSettingAbout::PrepareUpdateOS()
{
	RETAILMSG(1/*DEBUG_SETTING*/,(_T("###### PrepareUpdateOS \r\n")));
#if CVTE_EN_CARPLAY
	wchar_t *name=CCarplayAdapter::GetInstance()->GetDeviceName();
	if(name)
	{
		wchar_t filename_dest[MAX_PATH];
		wchar_t filename_src[MAX_PATH];
		swprintf_s(filename_dest,MAX_PATH,_T("\\USB Disk\\%s.ca"),name);
		swprintf_s(filename_src,MAX_PATH,_T("\\StaticStore\\%s.ca"),name);
		CopyFile(filename_src,filename_dest,0);
		RETAILMSG(1/*DEBUG_SETTING*/,(_T("###### backup CA =%s \r\n"),name));
	}
#endif
#if (CVTE_EN_CPCA || CVTE_EN_CPMAC)
	HANDLE hCpd = CreateFile(TEXT("CPD1:"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if ( hCpd == INVALID_HANDLE_VALUE )
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CPD Open Failed! err = %d\n"), TEXT(__FUNCTION__), GetLastError()));
		return;
	}

	//Backup CA
	unsigned char bufca[2560] = {0};
	size_t bufsizeca = sizeof(bufca);

	if (!DeviceIoControl(hCpd, IOCTL_CSECURE_GET_CA, NULL, 0, bufca, bufsizeca, NULL, NULL))
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Get CA Failed! err = %d\n"), TEXT(__FUNCTION__), GetLastError()));
		CloseHandle(hCpd);
		return;
	}
	
	wchar_t strSrcCA[MAX_PATH];
	if (swprintf_s(strSrcCA,MAX_PATH,_T("\\USB Disk\\%s.ca"),m_strHardwareID) == -1)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Format srouce CA path failed! err = %d\n"), TEXT(__FUNCTION__), GetLastError()));
		return;
	}

	if (!StoreFile(strSrcCA, bufca, bufsizeca))
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Store srouce CA path[%s] failed! err = %d\n"), TEXT(__FUNCTION__), strSrcCA, GetLastError()));
		return;
	}

	//Backup MAC
	const wchar_t strSrcMAC[] = _T("\\USB Disk\\MAC");
	unsigned char bufmac[6] = {0};
	size_t bufsizemac = sizeof(bufmac);

	if (!DeviceIoControl(hCpd, IOCTL_CSECURE_GET_MAC, NULL, 0, bufmac, bufsizemac, NULL, NULL))
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Get MAC Failed! err = %d\n"), TEXT(__FUNCTION__), GetLastError()));
		CloseHandle(hCpd);
		return;
	}

	if (!StoreFile(strSrcMAC, bufmac, bufsizemac))
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Store srouce CA path[%s] failed! err = %d\n"), TEXT(__FUNCTION__), strSrcMAC, GetLastError()));
		return;
	}

	if (hCpd)
	{
		CloseHandle(hCpd);
	}
#endif

}

BOOL CPanelSettingAbout::StoreFile( const wchar_t * path, const void * buf, size_t bufsize )
{
	FILE *file;
	BOOL bRet;

	if(0 != _wfopen_s(&file, path, L"wb"))
	{
		goto exit;
	}

	if (fwrite(buf,bufsize,1,file) != 1)
	{
		goto exit;
	}

	//bRet = SetFileAttributes(path,FILE_ATTRIBUTE_HIDDEN);
	//if (!bRet)
	//{
	//	bRet = GetLastError();
	//	goto exit;
	//}

	bRet = TRUE;

exit:
	if (file != NULL)
	{
		fclose(file);
	}
	return bRet;
}