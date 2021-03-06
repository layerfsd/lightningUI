#include "PanelSettingAndroidType.h"
#include "../../resource.h"

CPanelSettingAndroidType::CPanelSettingAndroidType(void)
{
	m_idAndroidTypeToSet = 0;
}
CPanelSettingAndroidType::~CPanelSettingAndroidType(void)
{

}

BOOL CPanelSettingAndroidType::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingAndroidType::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\AndroidAccessory"));
	m_boxAndroidType.MoveToIndex(m_boxAndroidType.SearchID(reg.ValueDW(_T("SwitchDeviceType"))));
}

void CPanelSettingAndroidType::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingAndroidType::Render()
{
	if(!BeginRender())return;

	m_boxAndroidType.SetAlpha(m_fCurAlpha);
	m_boxAndroidType.Render();


	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	m_btnCancel.SetAlpha(m_fCurAlpha);
	m_btnCancel.Render();

	EndRender();
}

BOOL CPanelSettingAndroidType::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

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

	m_boxAndroidType.Initialize(IDC_SETTING_BOX_ANDORID_TYPE,this,&box,pSkinManager);
	m_boxAndroidType.SetNotifyFunc(&CPanelSettingAndroidType::OnNotify,this);
	m_boxAndroidType.SetMarginHor(caption_h);
	m_boxAndroidType.SetPos(0,40);
	m_boxAndroidType.LinkTextGL(pTextGL);
	m_boxAndroidType.Show(TRUE,FALSE);
	m_boxAndroidType.SetAlpha(1.0f);
	m_boxAndroidType.SetCaptionColor(&cr_gray,&cr_w);
    	m_boxAndroidType.SetCaptioniHeight(caption_h);
	//
	CListBoxItemGL* pItem;

	pItem=new CListBoxItemGL(_T("AndroidAuto"));
	pItem->SetIcon(&m_iconSetting);
	m_boxAndroidType.AddItem(pItem,DEVICE_TYPE_ANDROID_AUTO);

	pItem=new CListBoxItemGL(_T("WebLink"));
	pItem->SetIcon(&m_iconSetting);
	m_boxAndroidType.AddItem(pItem,DEVICE_TYPE_WEBLINK);

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
	m_btnOK.SetNotifyFunc(CPanelSettingAndroidType::OnNotify,this);

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
	m_btnCancel.SetNotifyFunc(CPanelSettingAndroidType::OnNotify,this);

	return TRUE;
}


void CALLBACK CPanelSettingAndroidType::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingAndroidType *panel=(CPanelSettingAndroidType*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingAndroidType::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BOX_ANDORID_TYPE:
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
					CReg reg;
					reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\AndroidAccessory"));
					if(reg.ValueDW(_T("SwitchDeviceType")) != id_item)
					{
						m_idAndroidTypeToSet = id_item;

						ShowNotificationBox(m_hMainWnd,IDC_SETTING_SET_ANDORID_TYPE_RESET_CONFIRM,
							&CPanelSettingAndroidType::OnNotify,
							this,
							0,
							GetResourceString(IDS_SETTING_ANDROID_TYPE),
							GetResourceString(IDS_RESET_TO_TAKE_EFFECT),
							0);
					}

					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_SETTING_SET_ANDORID_TYPE_RESET_CONFIRM:
		{
			if(wMsg == NB_OK)//
			{
				CReg reg;
				reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\AndroidAccessory"));
				reg.SetDW(_T("SwitchDeviceType"),m_idAndroidTypeToSet);
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
			CReg reg;
			reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\AndroidAccessory"));
			if(reg.ValueDW(_T("SwitchDeviceType")) != m_boxAndroidType.GetCurFocusItemID())
			{
				m_idAndroidTypeToSet = m_boxAndroidType.GetCurFocusItemID();

				ShowNotificationBox(m_hMainWnd,IDC_SETTING_SET_ANDORID_TYPE_RESET_CONFIRM,
					&CPanelSettingAndroidType::OnNotify,
					this,
					0,
					GetResourceString(IDS_SETTING_ANDROID_TYPE),
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

