#include "PanelSettingLanguage.h"
#include "../../resource.h"

CPanelSettingLanguage::CPanelSettingLanguage(void)
: m_pTextGL(NULL)
{
	m_idLanguageToSet = 0;
}
CPanelSettingLanguage::~CPanelSettingLanguage(void)
{

}

BOOL CPanelSettingLanguage::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingLanguage::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	if (m_pTextGL)
	{
		m_pTextGL->SetReadingOrder(FALSE);
	}
	m_boxLanguage.MoveToIndex(m_boxLanguage.SearchID(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage));
}

void CPanelSettingLanguage::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	if (m_pTextGL)
	{
		m_pTextGL->SetReadingOrder(TRUE);
	}
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingLanguage::Render()
{
	if(!BeginRender())return;

	m_boxLanguage.SetAlpha(m_fCurAlpha);
	m_boxLanguage.Render();


	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	m_btnCancel.SetAlpha(m_fCurAlpha);
	m_btnCancel.Render();

	EndRender();
}

BOOL CPanelSettingLanguage::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x = 12;
	int margin_y = 12;
	int caption_h = 22;
	int caption2_h = 22;
	int caption3_h = 22;
	int gridHeight = 80;
	int cx = g_iClientWidth - LIST_BOX_ITEM_WIDTH;//g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x*2;
	int cy = LIST_BOX_ITEM_HEIGHT;

	m_iconSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1507));
	m_iconSettingFocus.Initialize(0, NULL, m_pSkinManager->GetTexture(TEXID_1508));

	GLfloat py=g_iClientHeight/2-40;

	LISTBOXSTRUCTURE box;
	memset(&box,0,sizeof(box));

	box.iSpacing = (g_iClientHeight - cy)/5;
	box.siClient.cx = 610;
	box.siClient.cy = g_iClientHeight - cy;	
	box.siBorder.cx = 610;	
	box.siBorder.cy = g_iClientHeight - cy;
	box.idTexIndexBase=TEXID_066;
	box.idTexIndexFocus=TEXID_242;
	box.idTexBase=TEXID_065;
	box.idTexBtnUp=TEXID_066;
	box.idTexBtnDown=TEXID_241;
	box.idTexSliBase=TEXID_060;
	box.idTexSliChannel=TEXID_060;
	box.idTexSliThumb=TEXID_276;
	box.bNoSlider=FALSE;
	box.bNoBorder=FALSE;	
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=TRUE;
	box.pxSlider=-8;
	box.fIndexBaseOffset = 0;

	m_boxLanguage.Initialize(IDC_SETTING_BOX_LANGUAGE,this,&box,pSkinManager);
	m_boxLanguage.SetNotifyFunc(&CPanelSettingLanguage::OnNotify,this);
	m_boxLanguage.SetPos(0, cy/2);
	m_boxLanguage.LinkTextGL(pTextGL);
	m_boxLanguage.Show(TRUE,FALSE);
	m_boxLanguage.SetAlpha(1.0f);
	m_boxLanguage.SetCaptionColor(&cr_gray,&cr_w);
	m_boxLanguage.SetCaptioniHeight(caption_h);
	//
	CListBoxItemGL* pItem;
	int iconMarginR = 24;
	int strMarginR = 34;
#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_CHINESE_SIM )
	pItem=new CListBoxItemGL(_T("简体中文"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0));
	m_boxLanguage.AddItem(pItem,LANGUAGE_CHN_SIM);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_CHINESE_TRA )
	pItem=new CListBoxItemGL(_T("繁體中文"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_CHN_TRA);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_ENGLISH )
	pItem=new CListBoxItemGL(_T("English"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_ENGLISH);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_KOREAN )
	pItem=new CListBoxItemGL(_T("한국어"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0));
	m_boxLanguage.AddItem(pItem,LANGUAGE_KOREAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_GERMAN )
	pItem=new CListBoxItemGL(_T("Deutsch"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0));
	m_boxLanguage.AddItem(pItem,LANGUAGE_GERMAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_FRENCH )
	pItem=new CListBoxItemGL(_T("Français"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_FRANCH);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_SPANISH )
	pItem=new CListBoxItemGL(_T("Español"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_SPANISH);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_PORTUGESE )
	pItem=new CListBoxItemGL(_T("Português"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_PORTUGUESE);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_ITALIAN )
	pItem=new CListBoxItemGL(_T("Italiano"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_ITALIAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_JAPANESS )
	pItem=new CListBoxItemGL(_T("日本語"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_JAPANESE);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_RUSSIAN )
	pItem=new CListBoxItemGL(_T("Русский"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_RUSSIAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_PERSIAN )
	pItem=new CListBoxItemGL(_T("فارسی"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_PERSIAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_GREEK )
	pItem=new CListBoxItemGL(_T("ελληνικά"), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxLanguage.AddItem(pItem,LANGUAGE_GREEK);
#endif
	int gap = 5;

	//Set
	m_btnOK.Initialize(IDC_OK,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnOK.SetSize((cx-margin_x + gap)/2, gridHeight);
	m_btnOK.SetPos(-(cx-margin_x)/4 - gap,-g_iClientHeight/2+cy/2);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_w);
	m_btnOK.SetCaptionHeight(caption_h);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingLanguage::OnNotify,this);

	//Cancel
	m_btnCancel.Initialize(IDC_CANCEL,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnCancel.SetSize((cx-margin_x + gap)/2, gridHeight);
	m_btnCancel.SetPos((cx-margin_x)/4 + gap,-g_iClientHeight/2+cy/2);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_w);
	m_btnCancel.SetCaptionHeight(caption_h);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingLanguage::OnNotify,this);

#else
	m_pTextGL = pTextGL;
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x = 12;
	int margin_y = 12;
	int caption_h = 22;
	int caption2_h = 18;
	int caption3_h = 16;
	int cx=LIST_BOX_ITEM_WIDTH;//g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x*2;
	int cy=LIST_BOX_ITEM_HEIGHT;


	m_iconSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_623));

	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));

	//find param:
	int count=(g_iClientHeight-cy-margin_y*2)/cy;
	if((g_iClientHeight-cy-margin_y)%(cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy-margin_y*2)/count;///2*2;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=(box.iSpacing * count);

	box.idTexIndexBase=TEXID_606;
	box.idTexIndexFocus=TEXID_607;			//TEXID_361;
	box.idTexBase=TEXID_066;
	box.idTexSliThumb=TEXID_661;
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;//FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=FALSE;	//TRUE;
	box.pxSlider=-15;	//-30;
	box.fIndexBaseOffset=10; //74;

	m_boxLanguage.Initialize(IDC_SETTING_BOX_LANGUAGE,this,&box,pSkinManager);
	m_boxLanguage.SetNotifyFunc(&CPanelSettingLanguage::OnNotify,this);
	m_boxLanguage.SetMarginHor(caption_h);
	m_boxLanguage.SetPos(0,40);
	m_boxLanguage.LinkTextGL(pTextGL);
	m_boxLanguage.Show(TRUE,FALSE);
	m_boxLanguage.SetAlpha(1.0f);
	m_boxLanguage.SetCaptionColor(&cr_gray,&cr_w);
	m_boxLanguage.SetCaptioniHeight(caption_h);
	//
	CListBoxItemGL* pItem;

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_CHINESE_SIM )
	pItem=new CListBoxItemGL(_T("简体中文"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_CHN_SIM);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_CHINESE_TRA )
	pItem=new CListBoxItemGL(_T("繁體中文"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_CHN_TRA);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_ENGLISH )
	pItem=new CListBoxItemGL(_T("English"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_ENGLISH);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_KOREAN )
	pItem=new CListBoxItemGL(_T("한국어"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_KOREAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_GERMAN )
	pItem=new CListBoxItemGL(_T("Deutsch"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_GERMAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_FRENCH )
	pItem=new CListBoxItemGL(_T("Français"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_FRANCH);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_SPANISH )
	pItem=new CListBoxItemGL(_T("Español"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_SPANISH);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_PORTUGESE )
	pItem=new CListBoxItemGL(_T("Português"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_PORTUGUESE);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_ITALIAN )
	pItem=new CListBoxItemGL(_T("Italiano"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_ITALIAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_JAPANESS )
	pItem=new CListBoxItemGL(_T("日本語"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_JAPANESE);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_RUSSIAN )
	pItem=new CListBoxItemGL(_T("Русский"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_RUSSIAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_PERSIAN )
	pItem=new CListBoxItemGL(_T("فارسی"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_PERSIAN);
#endif

#if ( CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_GREEK )
	pItem=new CListBoxItemGL(_T("ελληνικά"));
	pItem->SetIcon(&m_iconSetting);
	m_boxLanguage.AddItem(pItem,LANGUAGE_GREEK);
#endif

	//Set
	m_btnOK.Initialize(IDC_OK,this,pSkinManager->GetTexture(TEXID_606),NULL,
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
	m_btnOK.SetNotifyFunc(CPanelSettingLanguage::OnNotify,this);

	//Cancel
	m_btnCancel.Initialize(IDC_CANCEL,this,pSkinManager->GetTexture(TEXID_606),NULL,
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
	m_btnCancel.SetNotifyFunc(CPanelSettingLanguage::OnNotify,this);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET


	return TRUE;
}


void CALLBACK CPanelSettingLanguage::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingLanguage *panel=(CPanelSettingLanguage*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingLanguage::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BOX_LANGUAGE:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
				{

					//
					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_DOUBLE_CLICK:
				{
					if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage != id_item)
					{
						m_idLanguageToSet = id_item;

						ShowNotificationBox(m_hMainWnd,IDC_SETTING_SET_LANG_RESET_CONFIRM,
							&CPanelSettingLanguage::OnNotify,
							this,
							0,
							GetResourceString(IDS_SETTING_LANGUAGE),
							GetResourceString(IDS_RESET_TO_TAKE_EFFECT),
							0);
					}

					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_SETTING_SET_LANG_RESET_CONFIRM:
		{
			if(wMsg == NB_OK)//
			{
				m_pSkinManager->GetConfig()->GetCurParam()->idLanguage = m_idLanguageToSet;
				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingGeneral::HandleControlMSG]: ----confirm to reset-----\r\n")));
				//to do:
				::PostMessage(m_hMainWnd,WM_SYSTEM_PRE_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
				::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_OK:
		{
			if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage != m_boxLanguage.GetCurFocusItemID())
			{
				m_idLanguageToSet = m_boxLanguage.GetCurFocusItemID();

				ShowNotificationBox(m_hMainWnd,IDC_SETTING_SET_LANG_RESET_CONFIRM,
					&CPanelSettingLanguage::OnNotify,
					this,
					0,
					GetResourceString(IDS_SETTING_LANGUAGE),
					GetResourceString(IDS_RESET_TO_TAKE_EFFECT),
					0);
			}
			else
			{
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);
			}

			PlaySoundEffect(0);
		}
		break;
	case IDC_CANCEL:
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);
			PlaySoundEffect(0);
		}
		break;
	}
}

