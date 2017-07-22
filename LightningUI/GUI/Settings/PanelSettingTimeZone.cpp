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
	_T("(GMT-12:00) §­§Ú§ß§Ú§ñ §ã§Þ§Ö§ß§í §Õ§Ñ§ä §ß§Ñ §©§Ñ§á§Ñ§Õ§Ö"),
	_T("(GMT-11:00) §¡§Þ§Ö§â§Ú§Ü§Ñ§ß§ã§Ü§à§Ö §³§Ñ§Þ§à§Ñ"),
	_T("(GMT-11:00) UTC-11"),
	_T("(GMT-10:00) §¤§Ñ§Ó§Ñ§Û§Ú"),
	_T("(GMT-09:00) §¡§Ý§ñ§ã§Ü§Ñ"),
	_T("(GMT-08:00) §´§Ú§ç§à§à§Ü§Ö§Ñ§ß§ã§Ü§à§Ö (§³§º§¡ §Ú §¬§Ñ§ß§Ñ§Õ§Ñ)"),
	_T("(GMT-08:00) §¬§Ñ§Ý§Ú§æ§à§â§ß§Ú§ñ"),
	_T("(GMT-07:00) §¤§à§â§ß§à§Ö (§³§º§¡ §Ú §¬§Ñ§ß§Ñ§Õ§Ñ)"),
	_T("(GMT-07:00) §¡§â§Ú§Ù§à§ß§Ñ"),
	_T("(GMT-07:00) §¹§Ú§å§Ñ§å§Ñ, §­§Ñ-§±§Ñ§ã, §®§Ñ§Ù§Ñ§ä§Ý§Ñ§ß"),
	_T("(GMT-06:00) §¸§Ö§ß§ä§â§Ñ§Ý§î§ß§à§Ö (§³§º§¡ §Ú §¬§Ñ§ß§Ñ§Õ§Ñ)"),
	_T("(GMT-06:00) §³§Ñ§ã§Ü§Ñ§é§Ö§Ó§Ñ§ß"),
	_T("(GMT-06:00) §¸§Ö§ß§ä§â§Ñ§Ý§î§ß§Ñ§ñ §¡§Þ§Ö§â§Ú§Ü§Ñ"),
	_T("(GMT-06:00) §¤§Ó§Ñ§Õ§Ñ§Ý§Ñ§ç§Ñ§â§Ñ, §®§Ö§ç§Ú§Ü§à"),
	_T("(GMT-05:00) §£§à§ã§ä§à§é§ß§à§Ö (§³§º§¡ §Ú §¬§Ñ§ß§Ñ§Õ§Ñ)"),
	_T("(GMT-05:00) §£§à§ã§ä. §ª§ß§Õ§Ú§Ñ§ß§Ñ"),
	_T("(GMT-05:00) §¢§à§Ô§à§ä§Ñ, §­§Ú§Þ§Ñ, §¬§å§Ú§ä§à"),
	_T("(GMT-04:30) §¬§Ñ§â§Ñ§Ü§Ñ§ã"),
	_T("(GMT-04:00) §¡§ä§Ý§Ñ§ß§ä§Ú§é§Ö§ã§Ü§à§Ö (§¬§Ñ§ß§Ñ§Õ§Ñ)"),
	_T("(GMT-04:00) §¥§Ø§à§â§Õ§Ø§ä§Ñ§å§ß, §­§Ñ-§±§Ñ§ã"),
	_T("(GMT-04:00) §³§Ñ§ß§ä§î§ñ§Ô§à"),
	_T("(GMT-04:00) §¬§å§ñ§Ò§Ñ"),
	_T("(GMT-04:00) §¡§ã§å§ß§ã§î§à§ß"),
	_T("(GMT-03:30) §¯§î§ð§æ§Ñ§å§ß§Õ§Ý§Ö§ß§Õ"),
	_T("(GMT-03:00) §¢§â§Ñ§Ù§Ú§Ý§Ú§Ñ"),
	_T("(GMT-03:00) §¬§Ñ§Û§Ö§ß§ß§Ñ, §¶§à§â§ä§Ñ§Ý§Ö§Ù§Ñ"),
	_T("(GMT-03:00) §¤§â§Ö§ß§Ý§Ñ§ß§Õ§Ú§ñ"),
	_T("(GMT-03:00) §¢§å§ï§ß§à§ã-§¡§Û§â§Ö§ã"),
	_T("(GMT-03:00) §®§à§ß§ä§Ö§Ó§Ú§Õ§Ö§à"),
	_T("(GMT-03:00) §³§Ñ§Ý§î§Ó§Ñ§Õ§à§â"),
	_T("(GMT-02:00) §³§â§Ö§Õ§ß§Ö§Ñ§ä§Ý§Ñ§ß§ä§Ú§é§Ö§ã§Ü§à§Ö"),
	_T("(GMT-02:00) UTC-2"),
	_T("(GMT-01:00) §¡§Ù§à§â§ã§Ü§Ú§Ö §à§ã§ä§â§à§Ó§Ñ"),
	_T("(GMT-01:00) §¬§Ñ§Ò§à-§£§Ö§â§Õ§Ö"),
	_T("(GMT) §¥§å§Ò§Ý§Ú§ß, §¿§Õ§Ú§ß§Ò§å§â§Ô, §­§Ú§ã§ã§Ñ§Ò§à§ß, §­§à§ß§Õ§à§ß"),
	_T("(GMT) §®§à§ß§â§à§Ó§Ú§ñ, §²§Ö§Û§Ü§î§ñ§Ó§Ú§Ü"),
	_T("(GMT) §¬§Ñ§ã§Ñ§Ò§Ý§Ñ§ß§Ü§Ñ"),
	_T("(GMT) §£§â§Ö§Þ§ñ §á§à §¤§â§Ú§ß§Ó§Ú§é§å"),
	_T("(GMT+01:00) §¢§Ö§Ý§Ô§â§Ñ§Õ, §¢§â§Ñ§ä§Ú§ã§Ý§Ñ§Ó§Ñ, §¢§å§Õ§Ñ§á§Ö§ê§ä"),
	_T("(GMT+01:00) §³§Ñ§â§Ñ§Ö§Ó§à, §³§Ü§à§á§î§Ö, §£§Ñ§â§ê§Ñ§Ó§Ñ"),
	_T("(GMT+01:00) §¢§â§ð§ã§ã§Ö§Ý§î, §¬§à§á§Ö§ß§Ô§Ñ§Ô§Ö§ß"),
	_T("(GMT+01:00) §¡§Þ§ã§ä§Ö§â§Õ§Ñ§Þ, §¢§Ö§â§Ý§Ú§ß, §¢§Ö§â§ß"),
	_T("(GMT+01:00) §©§Ñ§á§Ñ§Õ§ß§à-§è§Ö§ß§ä§â§Ñ§Ý§î§ß§Ñ§ñ §¡§æ§â§Ú§Ü§Ñ"),
	_T("(GMT+01:00) §£§Ú§ß§Õ§ç§å§Ü"),
	_T("(GMT+02:00) §¯§Ú§Ü§à§ã§Ú§ñ"),
	_T("(GMT+02:00) §¬§Ñ§Ú§â"),
	_T("(GMT+02:00) §·§Ö§Ý§î§ã§Ú§ß§Ü§Ú, §¬§Ú§Ö§Ó, §²§Ú§Ô§Ñ, §³§à§æ§Ú§ñ"),
	_T("(GMT+02:00) §¡§æ§Ú§ß§í, §¢§å§ç§Ñ§â§Ö§ã§ä"),
	_T("(GMT+02:00) §·§Ñ§â§Ñ§â§Ö, §±§â§Ö§ä§à§â§Ú§ñ"),
	_T("(GMT+02:00) §ª§Ö§â§å§ã§Ñ§Ý§Ú§Þ"),
	_T("(GMT+02:00) §¡§Þ§Þ§Ñ§ß"),
	_T("(GMT+02:00) §¢§Ö§Û§â§å§ä"),
	_T("(GMT+02:00) §¥§Ñ§Þ§Ñ§ã§Ü"),
	_T("(GMT+02:00) §³§ä§Ñ§Þ§Ò§å§Ý"),
	_T("(GMT+03:00) §®§à§ã§Ü§Ó§Ñ, §³§Ñ§ß§Ü§ä-§±§Ö§ä§Ö§â§Ò§å§â§Ô"),
	_T("(GMT+03:00) §¬§å§Ó§Ö§Û§ä, §¿§â-§²§Ú§ñ§Õ"),
	_T("(GMT+03:00) §¯§Ñ§Û§â§à§Ò§Ú"),
	_T("(GMT+03:00) §¢§Ñ§Ô§Õ§Ñ§Õ"),
	_T("(GMT+03:00) §¬§Ñ§Ý§Ú§ß§Ú§ß§Ô§â§Ñ§Õ, §®§Ú§ß§ã§Ü"),
	_T("(GMT+03:30) §´§Ö§Ô§Ö§â§Ñ§ß"),
	_T("(GMT+04:00) §´§Ò§Ú§Ý§Ú§ã§Ú"),
	_T("(GMT+04:00) §¡§Ò§å-§¥§Ñ§Ò§Ú, §®§å§ã§Ü§Ñ§ä"),
	_T("(GMT+04:00) §¦§â§Ö§Ó§Ñ§ß"),
	_T("(GMT+04:00) §¢§Ñ§Ü§å"),
	_T("(GMT+04:00) §±§à§â§ä-§­§å§Ú"),
	_T("(GMT+04:30) §¬§Ñ§Ò§å§Ý"),
	_T("(GMT+05:00) §¦§Ü§Ñ§ä§Ö§â§Ú§ß§Ò§å§â§Ô"),
	_T("(GMT+05:00) §´§Ñ§ê§Ü§Ö§ß§ä"),
	_T("(GMT+05:00) §ª§ã§Ý§Ñ§Þ§Ñ§Ò§Ñ§Õ, §¬§Ñ§â§Ñ§é§Ú"),
	_T("(GMT+05:30) §¹§Ö§ß§ß§Ñ§Ú, §¬§Ñ§Ý§î§Ü§å§ä§ä§Ñ, §®§å§Þ§Ò§Ñ§Ú"),
	_T("(GMT+05:30) §º§â§Ú-§¥§Ø§Ñ§ñ§Ó§Ñ§â§Õ§Ö§ß§Ö§á§å§â§Ñ"),
	_T("(GMT+05:45) §¬§Ñ§ä§Þ§Ñ§ß§Õ§å"),
	_T("(GMT+06:00) §¡§ã§ä§Ñ§ß§Ñ"),
	_T("(GMT+06:00) §¯§à§Ó§à§ã§Ú§Ò§Ú§â§ã§Ü"),
	_T("(GMT+06:00) §¥§Ñ§Ü§Ü§Ñ"),
	_T("(GMT+06:30) §²§Ñ§ß§Ô§å§ß"),
	_T("(GMT+07:00) §¢§Ñ§ß§Ô§Ü§à§Ü, §·§Ñ§ß§à§Û, §¥§Ø§Ñ§Ü§Ñ§â§ä§Ñ"),
	_T("(GMT+07:00) §¬§â§Ñ§ã§ß§à§ñ§â§ã§Ü"),
	_T("(GMT+08:00) §±§Ö§Ü§Ú§ß, §¹§å§ß§è§Ú§ß, §¤§à§ß§Ü§à§ß§Ô"),
	_T("(GMT+08:00) §´§Ñ§Û§Ò§ï§Û"),
	_T("(GMT+08:00) §±§Ö§â§ä"),
	_T("(GMT+08:00) §ª§â§Ü§å§ä§ã§Ü"),
	_T("(GMT+08:00) §¬§å§Ñ§Ý§Ñ-§­§å§Þ§á§å§â, §³§Ú§ß§Ô§Ñ§á§å§â"),
	_T("(GMT+08:00) §µ§Ý§Ñ§ß-§¢§Ñ§ä§à§â"),
	_T("(GMT+09:00) §³§Ö§å§Ý"),
	_T("(GMT+09:00) §°§ã§Ñ§Ü§Ñ, §³§Ñ§á§á§à§â§à, §´§à§Ü§Ú§à"),
	_T("(GMT+09:00) §Á§Ü§å§ä§ã§Ü"),
	_T("(GMT+09:30) §¥§Ñ§â§Ó§Ú§ß"),
	_T("(GMT+09:30) §¡§Õ§Ö§Ý§Ñ§Ú§Õ§Ñ"),
	_T("(GMT+10:00) §¬§Ñ§ß§Ò§Ö§â§â§Ñ, §®§Ö§Ý§î§Ò§å§â§ß, §³§Ú§Õ§ß§Ö§Û"),
	_T("(GMT+10:00) §¢§â§Ú§ã§Ò§Ö§ß"),
	_T("(GMT+10:00) §·§à§Ò§Ñ§â§ä"),
	_T("(GMT+10:00) §£§Ý§Ñ§Õ§Ú§Ó§à§ã§ä§à§Ü "),
	_T("(GMT+10:00) §¤§å§Ñ§Þ, §±§à§â§ä-§®§à§â§ã§Ò§Ú"),
	_T("(GMT+11:00) §³§à§Ý§à§Þ§à§ß§à§Ó§í §à§ã§ä§â§à§Ó§Ñ"),
	_T("(GMT+11:00) §®§Ñ§Ô§Ñ§Õ§Ñ§ß"),
	_T("(GMT+12:00) §¶§Ú§Õ§Ø§Ú"),
	_T("(GMT+12:00) §°§Ü§Ý§Ö§ß§Õ, §£§Ö§Ý§Ý§Ú§ß§Ô§ä§à§ß"),
	_T("(GMT+12:00) UTC+12"),
	_T("(GMT+13:00) §¯§å§Ü§å§Ñ§Ý§à§æ§Ñ"),

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

