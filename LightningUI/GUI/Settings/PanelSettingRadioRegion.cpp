#include "PanelSettingRadioRegion.h"
#include "../../resource.h"

CPanelSettingRadioRegion::CPanelSettingRadioRegion(void)
{
	m_idRadioRegionToSet = 0;
}
CPanelSettingRadioRegion::~CPanelSettingRadioRegion(void)
{

}

BOOL CPanelSettingRadioRegion::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingRadioRegion::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	DWORD idRegion;
	
	if (m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_EastEuro_Euro)
		idRegion = RADIO_REGION_Euro_EastEuro;
	else
		idRegion = m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion;

	m_boxRadioRegion.MoveToIndex(m_boxRadioRegion.SearchID(idRegion));
}

void CPanelSettingRadioRegion::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingRadioRegion::Render()
{
	if(!BeginRender())return;

	m_boxRadioRegion.SetAlpha(m_fCurAlpha);
	m_boxRadioRegion.Render();


	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	m_btnCancel.SetAlpha(m_fCurAlpha);
	m_btnCancel.Render();

	EndRender();
}

BOOL CPanelSettingRadioRegion::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
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
	int caption_h = 24;
	int caption2_h = 24;
	int caption3_h = 20;
	int gridHeight = 80;
	int cx = g_iClientWidth - LIST_BOX_ITEM_WIDTH;//g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x*2;
	int cy = LIST_BOX_ITEM_HEIGHT;

	m_iconSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1507));
	m_iconSettingFocus.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_1508));

	GLfloat py=g_iClientHeight/2-40;

	LISTBOXSTRUCTURE box;
	memset(&box,0,sizeof(box));

	//find param:
	box.iSpacing=(g_iClientHeight - cy)/5;
	box.siClient.cx = cx;	
	box.siClient.cy = g_iClientHeight - cy;
	box.siBorder.cx = cx;
	box.siBorder.cy = g_iClientHeight - cy;

	box.idTexIndexBase=TEXID_066;
	box.idTexIndexFocus=TEXID_240;
	box.idTexBase=TEXID_065;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxRadioRegion.Initialize(IDC_SETTING_BOX_RADIO_REGION,this,&box,pSkinManager);
	m_boxRadioRegion.SetNotifyFunc(&CPanelSettingRadioRegion::OnNotify,this);
	m_boxRadioRegion.SetPos(0,40);
	m_boxRadioRegion.LinkTextGL(pTextGL);
	m_boxRadioRegion.Show(TRUE,FALSE);
	m_boxRadioRegion.SetAlpha(1.0f);
	m_boxRadioRegion.SetCaptionColor(&cr_gray,&cr_w);
	m_boxRadioRegion.SetCaptioniHeight(caption_h);

	int iconMarginR = 24;
	int strMarginR = 34;
	CListBoxItemGL* pItem;
	//Europe
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_EUROPE )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_EUROPE), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_EUROPE);
#endif

	//USA1
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_USA )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_USA), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_USA);
#endif

	//East Europe
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_EAST_ERNEUROPE )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_EASTERN_EUROPE), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_EAST_ERNEUROPE);
#endif

	//Japan
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_JAPAN )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_JAPAN), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_JAPAN);
#endif

	//East Europe & Europe
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_EUROPE_EASTERNEUROPE )
	{
		wchar_t temp[256];
		swprintf_s(temp,256,_T("%s & %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
		pItem=new CListBoxItemGL(temp, NULL, NULL, strMarginR, 0);
		pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
		m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_EUROPE_EASTERNEUROPE);
	}
#endif

	//Korea
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_KOREAN )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_KOREA), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_KOREAN);
#endif

	//Taiwan
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_TAIWANG )
	pItem=new CListBoxItemGL(GetResourceString(IDS_TAIWAN), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_TAIWANG);
#endif

	//USA2
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_USA2 )
	{
		wchar_t temp[256];
		swprintf_s(temp,256,_T("%s 2"),GetResourceString(IDS_SETTING_USA));
		pItem=new CListBoxItemGL(temp, NULL, NULL, strMarginR, 0);
		pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
		m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_USA2);
	}
#endif

	//South America
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_SOUTHAMERICA )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SOUTH_AMERICA), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_SOUTHAMERICA);
#endif

	//Arabia Australia
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_ARABIA_AUSTRALIA )
	pItem=new CListBoxItemGL(GetResourceString(IDS_ARABIA_AUSTRALIA), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_ARABIA_AUSTRALIA);
#endif

	//Latin America
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_LATIN_AMERICA )
	pItem=new CListBoxItemGL(GetResourceString(IDS_LATIN_AMERICA), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_LATIN_AMERICA);
#endif

	//Middle East
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_MIDDLE_EAST )
	pItem=new CListBoxItemGL(GetResourceString(IDS_MIDDLE_EAST), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_MIDDLE_EAST);
#endif

	//Aust
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_AUST )
	pItem=new CListBoxItemGL(GetResourceString(IDS_AUST), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_AUST);
#endif

	//Brazil
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_BRAZIL )
	pItem=new CListBoxItemGL(GetResourceString(IDS_BRAZIL), NULL, NULL, strMarginR, 0);
	pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, iconMarginR, 0);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_BRAZIL);
#endif

	int gap = 5;
	
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
	m_btnOK.SetNotifyFunc(CPanelSettingRadioRegion::OnNotify,this);
	
	m_btnCancel.Initialize(IDC_CANCEL,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnCancel.SetSize((cx-margin_x + gap)/2, gridHeight);
	m_btnCancel.SetPos((cx-margin_x)/4 + gap, -g_iClientHeight/2+cy/2);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_w);
	m_btnCancel.SetCaptionHeight(caption_h);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingRadioRegion::OnNotify,this);
		
#else
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

	m_boxRadioRegion.Initialize(IDC_SETTING_BOX_RADIO_REGION,this,&box,pSkinManager);
	m_boxRadioRegion.SetNotifyFunc(&CPanelSettingRadioRegion::OnNotify,this);
	m_boxRadioRegion.SetMarginHor(caption_h);
	m_boxRadioRegion.SetPos(0,40);
	m_boxRadioRegion.LinkTextGL(pTextGL);
	m_boxRadioRegion.Show(TRUE,FALSE);
	m_boxRadioRegion.SetAlpha(1.0f);
	m_boxRadioRegion.SetCaptionColor(&cr_gray,&cr_w);
    	m_boxRadioRegion.SetCaptioniHeight(caption_h);
	//
	CListBoxItemGL* pItem;
	

	//USA1
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_USA )
	{
		wchar_t temp[256];
		if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
			swprintf_s(temp,256,_T("%s1 (FM - §ê§Ñ§Ô 100K§¤§è)"),GetResourceString(IDS_SETTING_USA));
		else
			swprintf_s(temp,256,_T("%s1 (FM Step 100KHz)"),GetResourceString(IDS_SETTING_USA));
		pItem=new CListBoxItemGL(temp);
		pItem->SetIcon(&m_iconSetting);
		m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_USA);
	}
#endif

	//USA2
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_USA2 )
	{
		wchar_t temp[256];
		if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
			swprintf_s(temp,256,_T("%s2 (FM - §ê§Ñ§Ô 200K§¤§è)"),GetResourceString(IDS_SETTING_USA));
		else
			swprintf_s(temp,256,_T("%s2 (FM Step 200KHz)"),GetResourceString(IDS_SETTING_USA));
		pItem=new CListBoxItemGL(temp);
		pItem->SetIcon(&m_iconSetting);
		m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_USA2);
	}
#endif

	//Aust
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_AUST )
	pItem=new CListBoxItemGL(GetResourceString(IDS_AUST));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_AUST);
#endif

	//Arabia Australia
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_ARABIA_AUSTRALIA )
	pItem=new CListBoxItemGL(GetResourceString(IDS_ARABIA_AUSTRALIA));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_ARABIA_AUSTRALIA);
#endif

	//Brazil
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_BRAZIL )
	pItem=new CListBoxItemGL(GetResourceString(IDS_BRAZIL));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_BRAZIL);
#endif

	//Europe
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_EUROPE )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_EUROPE));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_EUROPE);
#endif

	//East Europe
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_EAST_ERNEUROPE )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_EASTERN_EUROPE));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_EAST_ERNEUROPE);
#endif


	//East Europe & Europe
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_EUROPE_EASTERNEUROPE )
	{
		wchar_t temp[256];
		if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
			swprintf_s(temp,256,_T("%s §Ú %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
		else
			swprintf_s(temp,256,_T("%s & %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
		pItem=new CListBoxItemGL(temp);
		pItem->SetIcon(&m_iconSetting);
		m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_EUROPE_EASTERNEUROPE);
	}
#endif

	//Japan
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_JAPAN )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_JAPAN));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_JAPAN);
#endif

	//Korea
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_KOREAN )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SETTING_KOREA));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_KOREAN);
#endif

	//Latin America
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_LATIN_AMERICA )
	pItem=new CListBoxItemGL(GetResourceString(IDS_LATIN_AMERICA));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_LATIN_AMERICA);
#endif


	//Middle East
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_MIDDLE_EAST )
	pItem=new CListBoxItemGL(GetResourceString(IDS_MIDDLE_EAST));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_MIDDLE_EAST);
#endif

	//South America
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_SOUTHAMERICA )
	pItem=new CListBoxItemGL(GetResourceString(IDS_SOUTH_AMERICA));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_SOUTHAMERICA);
#endif

	//Taiwan
#if ( CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_TAIWANG )
	pItem=new CListBoxItemGL(GetResourceString(IDS_TAIWAN));
	pItem->SetIcon(&m_iconSetting);
	m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_TAIWANG);
#endif


	////Russia
	//pItem=new CListBoxItemGL(GetResourceString(IDS_RUSSIA));
	//pItem->SetIcon(&m_iconSetting);
	//m_boxRadioRegion.AddItem(pItem,CVTE_DEF_RADIO_REGION_RUSSIA);

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
	m_btnOK.SetNotifyFunc(CPanelSettingRadioRegion::OnNotify,this);

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
	m_btnCancel.SetNotifyFunc(CPanelSettingRadioRegion::OnNotify,this);
	
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	SetRadioRegion(m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion);

	return TRUE;
}


void CALLBACK CPanelSettingRadioRegion::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingRadioRegion *panel=(CPanelSettingRadioRegion*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingRadioRegion::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BOX_RADIO_REGION:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			m_idRadioRegionToSet = id_item;
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
					//m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = id_item;
					SetRadioRegion(id_item);
					SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_RADIO);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_OK:
		{
			//m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = m_idRadioRegionToSet;
			SetRadioRegion(m_idRadioRegionToSet);
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_RADIO);
			PlaySoundEffect(0);
		}
		break;
	case IDC_CANCEL:
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_RADIO);
			PlaySoundEffect(0);
		}
		break;
	}
}

void CPanelSettingRadioRegion::SetRadioRegion(DWORD dwRegion)
{
	RETAILMSG(1, (TEXT("MSG:[CPanelSettingRadioRegion::SetRadioRegion]: dwRegion[%d]\r\n"),dwRegion));
	if (dwRegion == RADIO_REGION_Euro_EastEuro) //europ+east europ
	{
		m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = RADIO_REGION_Euro_EastEuro;
		SendRadioCommand(RADIO_CMD_SET_REGION,(1<<16)+RADIO_REGION_Europe);
	}
	else if(dwRegion == RADIO_REGION_EastEuro_Euro) //east europ + europ
	{
		m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = RADIO_REGION_EastEuro_Euro;
		SendRadioCommand(RADIO_CMD_SET_REGION,(1<<16)+RADIO_REGION_EasternEurope);
	}
	else
	{
		m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion = dwRegion;
		SendRadioCommand(RADIO_CMD_SET_REGION,(1<<16)+BYTE(dwRegion));
	}

}