#include "PanelSettingTimeZone.h"
#include "../../resource.h"

extern DWORD					g_dwTimeSyncedCnt;

//typedef struct tagTZREG {
//    LONG    Bias;
//    LONG    StandardBias;
//    LONG    DaylightBias;
//    SYSTEMTIME StandardDate;
//    SYSTEMTIME DaylightDate;
//} TZREG;
static const wchar_t g_RussianTimeZoneName[MAX_TIME_ZONE_CNT][128] = {
	_T("(GMT-12:00) ���ڧߧڧ� ��ާ֧ߧ� �էѧ� �ߧ� ���ѧ�ѧէ�"),
	_T("(GMT-11:00) ���ާ֧�ڧܧѧߧ�ܧ�� ���ѧާ��"),
	_T("(GMT-11:00) UTC-11"),
	_T("(GMT-10:00) ���ѧӧѧۧ�"),
	_T("(GMT-09:00) ���ݧ��ܧ�"),
	_T("(GMT-08:00) ���ڧ���ܧ֧ѧߧ�ܧ�� (������ �� ���ѧߧѧէ�)"),
	_T("(GMT-08:00) ���ѧݧڧ���ߧڧ�"),
	_T("(GMT-07:00) �����ߧ�� (������ �� ���ѧߧѧէ�)"),
	_T("(GMT-07:00) ����ڧ٧�ߧ�"),
	_T("(GMT-07:00) ���ڧ�ѧ��, ����-���ѧ�, ���ѧ٧ѧ�ݧѧ�"),
	_T("(GMT-06:00) ���֧ߧ��ѧݧ�ߧ�� (������ �� ���ѧߧѧէ�)"),
	_T("(GMT-06:00) ���ѧ�ܧѧ�֧ӧѧ�"),
	_T("(GMT-06:00) ���֧ߧ��ѧݧ�ߧѧ� ���ާ֧�ڧܧ�"),
	_T("(GMT-06:00) ���ӧѧէѧݧѧ�ѧ��, ���֧�ڧܧ�"),
	_T("(GMT-05:00) ��������ߧ�� (������ �� ���ѧߧѧէ�)"),
	_T("(GMT-05:00) ������. ���ߧէڧѧߧ�"),
	_T("(GMT-05:00) ����ԧ���, ���ڧާ�, ����ڧ��"),
	_T("(GMT-04:30) ���ѧ�ѧܧѧ�"),
	_T("(GMT-04:00) ����ݧѧߧ�ڧ�֧�ܧ�� (���ѧߧѧէ�)"),
	_T("(GMT-04:00) ���ا��էا�ѧ��, ����-���ѧ�"),
	_T("(GMT-04:00) ���ѧߧ���ԧ�"),
	_T("(GMT-04:00) �����ҧ�"),
	_T("(GMT-04:00) �����ߧ����"),
	_T("(GMT-03:30) ������ѧ�ߧէݧ֧ߧ�"),
	_T("(GMT-03:00) ����ѧ٧ڧݧڧ�"),
	_T("(GMT-03:00) ���ѧ֧ۧߧߧ�, ������ѧݧ֧٧�"),
	_T("(GMT-03:00) ����֧ߧݧѧߧէڧ�"),
	_T("(GMT-03:00) �����ߧ��-���ۧ�֧�"),
	_T("(GMT-03:00) ����ߧ�֧ӧڧէ֧�"),
	_T("(GMT-03:00) ���ѧݧ�ӧѧէ��"),
	_T("(GMT-02:00) ����֧էߧ֧ѧ�ݧѧߧ�ڧ�֧�ܧ��"),
	_T("(GMT-02:00) UTC-2"),
	_T("(GMT-01:00) ���٧���ܧڧ� ������ӧ�"),
	_T("(GMT-01:00) ���ѧҧ�-���֧�է�"),
	_T("(GMT) ����ҧݧڧ�, ���էڧߧҧ���, ���ڧ��ѧҧ��, ����ߧէ��"),
	_T("(GMT) ����ߧ��ӧڧ�, ���֧ۧܧ��ӧڧ�"),
	_T("(GMT) ���ѧ�ѧҧݧѧߧܧ�"),
	_T("(GMT) ����֧ާ� ��� ����ڧߧӧڧ��"),
	_T("(GMT+01:00) ���֧ݧԧ�ѧ�, ����ѧ�ڧ�ݧѧӧ�, ����էѧ�֧��"),
	_T("(GMT+01:00) ���ѧ�ѧ֧ӧ�, ���ܧ����, ���ѧ��ѧӧ�"),
	_T("(GMT+01:00) �������֧ݧ�, �����֧ߧԧѧԧ֧�"),
	_T("(GMT+01:00) ���ާ��֧�էѧ�, ���֧�ݧڧ�, ���֧��"),
	_T("(GMT+01:00) ���ѧ�ѧէߧ�-��֧ߧ��ѧݧ�ߧѧ� �����ڧܧ�"),
	_T("(GMT+01:00) ���ڧߧէ���"),
	_T("(GMT+02:00) ���ڧܧ��ڧ�"),
	_T("(GMT+02:00) ���ѧڧ�"),
	_T("(GMT+02:00) ���֧ݧ��ڧߧܧ�, ���ڧ֧�, ���ڧԧ�, �����ڧ�"),
	_T("(GMT+02:00) ����ڧߧ�, �����ѧ�֧��"),
	_T("(GMT+02:00) ���ѧ�ѧ��, ����֧���ڧ�"),
	_T("(GMT+02:00) ���֧���ѧݧڧ�"),
	_T("(GMT+02:00) ���ާާѧ�"),
	_T("(GMT+02:00) ���֧ۧ���"),
	_T("(GMT+02:00) ���ѧާѧ��"),
	_T("(GMT+02:00) ����ѧާҧ��"),
	_T("(GMT+03:00) �����ܧӧ�, ���ѧߧܧ�-���֧�֧�ҧ���"),
	_T("(GMT+03:00) ����ӧ֧ۧ�, ����-���ڧ��"),
	_T("(GMT+03:00) ���ѧۧ��ҧ�"),
	_T("(GMT+03:00) ���ѧԧէѧ�"),
	_T("(GMT+03:00) ���ѧݧڧߧڧߧԧ�ѧ�, ���ڧߧ��"),
	_T("(GMT+03:30) ���֧ԧ֧�ѧ�"),
	_T("(GMT+04:00) ���ҧڧݧڧ��"),
	_T("(GMT+04:00) ���ҧ�-���ѧҧ�, �����ܧѧ�"),
	_T("(GMT+04:00) ����֧ӧѧ�"),
	_T("(GMT+04:00) ���ѧܧ�"),
	_T("(GMT+04:00) ������-�����"),
	_T("(GMT+04:30) ���ѧҧ��"),
	_T("(GMT+05:00) ���ܧѧ�֧�ڧߧҧ���"),
	_T("(GMT+05:00) ���ѧ�ܧ֧ߧ�"),
	_T("(GMT+05:00) ����ݧѧާѧҧѧ�, ���ѧ�ѧ��"),
	_T("(GMT+05:30) ���֧ߧߧѧ�, ���ѧݧ�ܧ����, ����ާҧѧ�"),
	_T("(GMT+05:30) �����-���اѧ�ӧѧ�է֧ߧ֧����"),
	_T("(GMT+05:45) ���ѧ�ާѧߧէ�"),
	_T("(GMT+06:00) �����ѧߧ�"),
	_T("(GMT+06:00) ����ӧ��ڧҧڧ���"),
	_T("(GMT+06:00) ���ѧܧܧ�"),
	_T("(GMT+06:30) ���ѧߧԧ��"),
	_T("(GMT+07:00) ���ѧߧԧܧ��, ���ѧߧ��, ���اѧܧѧ���"),
	_T("(GMT+07:00) ����ѧ�ߧ�����"),
	_T("(GMT+08:00) ���֧ܧڧ�, ����ߧ�ڧ�, ����ߧܧ�ߧ�"),
	_T("(GMT+08:00) ���ѧۧҧ��"),
	_T("(GMT+08:00) ���֧��"),
	_T("(GMT+08:00) ����ܧ����"),
	_T("(GMT+08:00) ����ѧݧ�-����ާ���, ���ڧߧԧѧ���"),
	_T("(GMT+08:00) ���ݧѧ�-���ѧ���"),
	_T("(GMT+09:00) ���֧��"),
	_T("(GMT+09:00) ����ѧܧ�, ���ѧ�����, ����ܧڧ�"),
	_T("(GMT+09:00) ���ܧ����"),
	_T("(GMT+09:30) ���ѧ�ӧڧ�"),
	_T("(GMT+09:30) ���է֧ݧѧڧէ�"),
	_T("(GMT+10:00) ���ѧߧҧ֧���, ���֧ݧ�ҧ���, ���ڧէߧ֧�"),
	_T("(GMT+10:00) ����ڧ�ҧ֧�"),
	_T("(GMT+10:00) ����ҧѧ��"),
	_T("(GMT+10:00) ���ݧѧէڧӧ����� "),
	_T("(GMT+10:00) ����ѧ�, ������-������ҧ�"),
	_T("(GMT+11:00) ����ݧ�ާ�ߧ�ӧ� ������ӧ�"),
	_T("(GMT+11:00) ���ѧԧѧէѧ�"),
	_T("(GMT+12:00) ���ڧէا�"),
	_T("(GMT+12:00) ���ܧݧ֧ߧ�, ���֧ݧݧڧߧԧ���"),
	_T("(GMT+12:00) UTC+12"),
	_T("(GMT+13:00) ����ܧ�ѧݧ���"),

};

CPanelSettingTimeZone::CPanelSettingTimeZone(void)
{
	m_iTimeZoneCurIndex = -1;
}
CPanelSettingTimeZone::~CPanelSettingTimeZone(void)
{

}

BOOL CPanelSettingTimeZone::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingTimeZone::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	GetTimeZoneInformation(&m_tziCurrent);
	INT num = m_boxTimeZone.GetCount();
	TCHAR   szTimeZone[64] = {0};
    CReg    Reg1(HKEY_LOCAL_MACHINE, RV_TIMEZONES), Reg2;
    while(Reg1.EnumKey(szTimeZone, ARRAYSIZEOF(szTimeZone)))
    {
		Reg2.Reset();
        if(Reg2.Open(Reg1,szTimeZone))
        {
			if(lstrcmp(m_tziCurrent.StandardName, Reg2.ValueSZ(RV_STD)) == 0)
            {
				Reg2.ValueSZ(RV_DISPLAY, szTimeZone, ARRAYSIZEOF(szTimeZone));
				break;
			}
        }
    }
	for ( INT i = 0; i < num; i++ )
	{
		if(lstrcmp(m_boxTimeZone.GetIndexValue(i),szTimeZone) == 0)
		{
			m_iTimeZoneCurIndex = i;
			break;
		}
	}
	m_tziSelected = m_tziCurrent;

	m_boxTimeZone.MoveToIndex(m_iTimeZoneCurIndex);

}

void CPanelSettingTimeZone::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingTimeZone::Render()
{
	if(!BeginRender())return;

	m_boxTimeZone.SetAlpha(m_fCurAlpha);
	m_boxTimeZone.Render();


	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	m_btnCancel.SetAlpha(m_fCurAlpha);
	m_btnCancel.Render();

	EndRender();
}

BOOL CPanelSettingTimeZone::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
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

	m_boxTimeZone.Initialize(IDC_SETTING_BOX_LANGUAGE,this,&box,pSkinManager);
	m_boxTimeZone.SetNotifyFunc(&CPanelSettingTimeZone::OnNotify,this);
	m_boxTimeZone.SetPos(0, cy/2);
	m_boxTimeZone.LinkTextGL(pTextGL);
	m_boxTimeZone.Show(TRUE,FALSE);
	m_boxTimeZone.SetAlpha(1.0f);
	m_boxTimeZone.SetCaptionColor(&cr_gray,&cr_w);
	m_boxTimeZone.SetCaptioniHeight(caption3_h);

	//
	CListBoxItemGL* pItem;

	CReg   Reg1(HKEY_LOCAL_MACHINE, RV_TIMEZONES), Reg2;
	TCHAR  szTimeZone[64];
	int index_cursor=0;

	while (Reg1.EnumKey(szTimeZone, ARRAYSIZEOF(szTimeZone)) && index_cursor<MAX_TIME_ZONE_CNT)
	{
		LPTSTR szTemp;
		Reg2.Reset();

		if(Reg2.Open(Reg1,szTimeZone))
		{
			// use the display name rather than the standard name
			szTemp = (LPTSTR)Reg2.ValueSZ(RV_DISPLAY);
			Reg2.ValueSZ(RV_STD, szTimeZone, ARRAYSIZEOF(szTimeZone));
			pItem=new CListBoxItemGL(szTemp, szTemp, NULL, 34, 0);
			pItem->SetIcon(&m_iconSetting, &m_iconSettingFocus, 24, 0);
			pItem->ShowValue(FALSE);
			m_boxTimeZone.AddItem(pItem);
			pItem->EnableAutoEllipsis(TRUE);
		}
		index_cursor++;
	}

	int gap = 5;
	//Set
	m_btnOK.Initialize(IDC_OK,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnOK.SetSize((cx-margin_x + gap)/2, gridHeight);
	m_btnOK.SetPos(-(cx-margin_x)/4 - gap, -g_iClientHeight/2+cy/2);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_w, &cr_w);
	m_btnOK.SetCaptionHeight(caption_h);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingTimeZone::OnNotify,this);

	//Cancel
	m_btnCancel.Initialize(IDC_CANCEL,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnCancel.SetSize((cx-margin_x + gap)/2, gridHeight);
	m_btnCancel.SetPos((cx-margin_x)/4 + gap,-g_iClientHeight/2+cy/2);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_w, &cr_w);
	m_btnCancel.SetCaptionHeight(caption_h);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingTimeZone::OnNotify,this);

#else
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x = 12;
	int margin_y = 12;
	int caption_h = 18;
	int caption2_h = 28;
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

	m_boxTimeZone.Initialize(IDC_SETTING_BOX_LANGUAGE,this,&box,pSkinManager);
	m_boxTimeZone.SetNotifyFunc(&CPanelSettingTimeZone::OnNotify,this);
	m_boxTimeZone.SetMarginHor(caption_h);
	m_boxTimeZone.SetPos(0,40);
	m_boxTimeZone.LinkTextGL(pTextGL);
	m_boxTimeZone.Show(TRUE,FALSE);
	m_boxTimeZone.SetAlpha(1.0f);
	m_boxTimeZone.SetCaptionColor(&cr_gray,&cr_w);
	m_boxTimeZone.SetCaptioniHeight(caption_h);

	//
	CListBoxItemGL* pItem;

	CReg   Reg1(HKEY_LOCAL_MACHINE, RV_TIMEZONES), Reg2;
	TCHAR  szTimeZone[64];
	int index_cursor=0;

	while (Reg1.EnumKey(szTimeZone, ARRAYSIZEOF(szTimeZone)) && index_cursor<MAX_TIME_ZONE_CNT)
	{
		LPTSTR szTemp;
		Reg2.Reset();

		if(Reg2.Open(Reg1,szTimeZone))
		{
			// use the display name rather than the standard name
			szTemp = (LPTSTR)Reg2.ValueSZ(RV_DISPLAY);
			Reg2.ValueSZ(RV_STD, szTimeZone, ARRAYSIZEOF(szTimeZone));
			if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
			{
				pItem=new CListBoxItemGL(g_RussianTimeZoneName[index_cursor], szTemp);
			}
			else
			{
				pItem=new CListBoxItemGL(szTemp, szTemp);
			}
			pItem->SetIcon(&m_iconSetting);
			pItem->ShowValue(FALSE);
			m_boxTimeZone.AddItem(pItem);
		}
		index_cursor++;
	}
	RETAILMSG(1, (_T("## TimeZone count = %d \r\n"), index_cursor));

	//Set
	m_btnOK.Initialize(IDC_OK,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnOK.SetSize((g_iClientWidth-cx-margin_x)/2,cy);
	m_btnOK.SetPos(-(g_iClientWidth-cx-margin_x)/4,-g_iClientHeight/2+45);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_gray);
	m_btnOK.SetCaptionHeight(caption2_h);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingTimeZone::OnNotify,this);

	//Cancel
	m_btnCancel.Initialize(IDC_CANCEL,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancel.SetSize((g_iClientWidth-cx-margin_x)/2,cy);
	m_btnCancel.SetPos((g_iClientWidth-cx-margin_x)/4,-g_iClientHeight/2+45);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_gray);
	m_btnCancel.SetCaptionHeight(caption2_h);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingTimeZone::OnNotify,this);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET


	return TRUE;
}


void CALLBACK CPanelSettingTimeZone::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingTimeZone *panel=(CPanelSettingTimeZone*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingTimeZone::TimeZoneChanged()
{
	m_iTimeZoneCurIndex=m_boxTimeZone.GetCurIndex();

	// Iterate over all timezones. Init new timezone to current just in case
    TCHAR   szTimeZone[64];
    CReg    Reg1(HKEY_LOCAL_MACHINE, RV_TIMEZONES), Reg2;
    while(Reg1.EnumKey(szTimeZone, ARRAYSIZEOF(szTimeZone)))
    {
        Reg2.Reset();
        if(Reg2.Open(Reg1,szTimeZone))
        {
            // if the display name matches the one in the combobox get the TZI data.
            if(!lstrcmp(m_boxTimeZone.GetIndexValue(m_iTimeZoneCurIndex), Reg2.ValueSZ(RV_DISPLAY)))
            {
                TZREG  *pTZR = (TZREG*)Reg2.ValueBinary(RV_TZI);
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

		            SystemTimeToFileTime(&pstOld, (FILETIME*)&ftOld);

		            ftNew = ftOld - ((Bias) * FILETIME_TO_MINUTES);
		            FileTimeToSystemTime((FILETIME*)&ftNew, &stNew);

		            // now update the clock
		            SetLocalTime(&stNew);

		            // update our notion of "current" timezone
		            SetTimeZoneInformation(&m_tziSelected);

		            m_tziCurrent = m_tziSelected;
                    break;
                }
            }
        }
    }
	//need re-sync time:
	g_dwTimeSyncedCnt = 0;
}

void CPanelSettingTimeZone::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
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

					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_DOUBLE_CLICK:
				{
					TimeZoneChanged();

					SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);

					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;

	case IDC_OK:
		{
			TimeZoneChanged();

			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);

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

