#include "PanelSettingDate.h"
#include "../../resource.h"

CPanelSettingDate::CPanelSettingDate(void)
{

}
CPanelSettingDate::~CPanelSettingDate(void)
{

}

BOOL CPanelSettingDate::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingDate::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//
	m_barCalendar.ResetTime();
}

void CPanelSettingDate::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingDate::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

	m_barCalendar.SetAlpha(m_fCurAlpha);
	m_barCalendar.Render();

	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	m_btnCancel.SetAlpha(m_fCurAlpha);
	m_btnCancel.Render();

	EndRender();
}

BOOL CPanelSettingDate::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

// 	//large size:
// 	SetSize(g_iClientWidth-240,g_iClientHeight+171);
//
// 	//enable scrolling vertical:
// 	EnableScrollingY(TRUE);



#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};

	int margin_x = 12;
	int margin_y = 12;
	int caption_h = 28;
	int caption2_h = 18;
	int caption3_h = 16;
	int cx=LIST_BOX_ITEM_WIDTH;//g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x*2;
	int cy=LIST_BOX_ITEM_HEIGHT;
	int gap = 5;

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(g_iClientWidth - cx, g_iClientHeight*2);
	m_tagBk.SetPos(0, 0);

	m_btnOK.Initialize(IDC_SETTING_BTN_SET_DATE,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnOK.SetSize((g_iClientWidth-cx-margin_x + gap)/2,cy);
	m_btnOK.SetPos(-(g_iClientWidth-cx-margin_x)/4 - gap, -g_iClientHeight/2+45);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_w);
	m_btnOK.SetCaptionHeight(caption_h);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingDate::OnNotify,this);

	m_btnCancel.Initialize(IDC_SETTING_BTN_CANCEL_DATE,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnCancel.SetSize((g_iClientWidth-cx-margin_x + gap)/2,cy);
	m_btnCancel.SetPos((g_iClientWidth-cx-margin_x)/4 + gap, -g_iClientHeight/2+45);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_w);
	m_btnCancel.SetCaptionHeight(caption_h);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingDate::OnNotify,this);

#else
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};

	int margin_x = 12;
	int margin_y = 12;
	int caption_h = 28;
	int caption2_h = 18;
	int caption3_h = 16;
	int cx=LIST_BOX_ITEM_WIDTH;//g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x*2;
	int cy=LIST_BOX_ITEM_HEIGHT;

	m_btnOK.Initialize(IDC_SETTING_BTN_SET_DATE,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnOK.SetSize((g_iClientWidth-cx-margin_x)/2,cy);
	m_btnOK.SetPos(-(g_iClientWidth-cx-margin_x)/4,-g_iClientHeight/2+45);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_gray);
	m_btnOK.SetCaptionHeight(caption_h);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingDate::OnNotify,this);

	m_btnCancel.Initialize(IDC_SETTING_BTN_CANCEL_DATE,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancel.SetSize((g_iClientWidth-cx-margin_x)/2,cy);
	m_btnCancel.SetPos((g_iClientWidth-cx-margin_x)/4,-g_iClientHeight/2+45);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_gray);
	m_btnCancel.SetCaptionHeight(caption_h);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingDate::OnNotify,this);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET


	CALENDARBARSTRUCTURE stru;
	memset(&stru,sizeof(stru),0);
	stru.siBorder.cx=g_iClientWidth-cx-20;
	stru.siBorder.cy=g_iClientHeight-cy;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	stru.idTexBtnUp=TEXID_065;
	stru.idTexBtnDown=TEXID_240;
#else
	stru.idTexBtnUp=TEXID_606;
	stru.idTexBtnDown=TEXID_607;
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	stru.idTexIconAdd=TEXID_083;
	stru.idTexIconSub=TEXID_082;
	stru.crCaptionUp=cr_gray;
	stru.crCaptionDown=cr_gray;

	m_barCalendar.Initialize(0,this,&stru,pSkinManager,pTextGL);
	m_barCalendar.SetNotifyFunc(NULL,NULL);
	m_barCalendar.SetPos(0,0);



	return TRUE;
}


void CALLBACK CPanelSettingDate::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingDate *panel=(CPanelSettingDate*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingDate::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BTN_SET_DATE:
		{
			RETAILMSG(DEBUG_SETTING, (_T("APP:TEST:GUI:Settings: [CPanelSettingDate::HandleControlMSG]: IDC_SETTING_BTN_SET_DATE \r\n")));
			m_barCalendar.ApplySettings();
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);

			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_CANCEL_DATE:
		{
			RETAILMSG(DEBUG_SETTING, (_T("APP:TEST:GUI:Settings: [CPanelSettingDate::HandleControlMSG]: IDC_SETTING_BTN_CANCEL_DATE \r\n")));
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);

			PlaySoundEffect(0);
		}
		break;
	}
}

