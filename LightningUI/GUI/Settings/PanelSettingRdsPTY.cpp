#include "PanelSettingRdsPTY.h"
#include "../../resource.h"

CPanelSettingRdsPTY::CPanelSettingRdsPTY(void)
{
	
}
CPanelSettingRdsPTY::~CPanelSettingRdsPTY(void)
{

}

BOOL CPanelSettingRdsPTY::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingRdsPTY::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingRdsPTY::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingRdsPTY::Render()
{
	if(!BeginRender())return;

	m_boxPTY.SetAlpha(m_fCurAlpha);
	m_boxPTY.Render();


	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	m_btnCancel.SetAlpha(m_fCurAlpha);
	m_btnCancel.Render();

	EndRender();
}

BOOL CPanelSettingRdsPTY::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int cx=240;
	int cy=100;
	int margin_x = 20;
	int margin_y = 20;


	m_iconSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_623));

	LISTBOXSTRUCTURE box;
/*
	int count = (g_iClientHeight-110)/80;
	if(count<=0)
		count=1;
*/

	memset(&box,0,sizeof(box));

	box.iSpacing= 78; //(g_iClientHeight)/count;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=box.iSpacing*5; //(box.iSpacing * count)-100;
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=390;  //g_iClientHeight-100;	//(box.iSpacing * count);

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

	m_boxPTY.Initialize(IDC_RADIO_BOX_SETTING_PTY,this,&box,pSkinManager);
	m_boxPTY.SetNotifyFunc(&CPanelSettingRdsPTY::OnNotify,this);
	m_boxPTY.SetMarginHor(30);
	m_boxPTY.SetPos(0,40);
	m_boxPTY.LinkTextGL(pTextGL);
	m_boxPTY.Show(TRUE,FALSE);
	m_boxPTY.SetAlpha(1.0f);
	m_boxPTY.SetCaptionColor(&cr_gray,&cr_w);

	//
	CListBoxItemGL* pItem;

	for (int i=0;i<PT_Count;i++)
	{
		pItem=new CListBoxItemGL(ProgramTypeToString(ProgramType(i)));
		pItem->SetIcon(&m_iconSetting);
		m_boxPTY.AddItem(pItem,i);
	}

	//pItem=new CListBoxItemGL(ProgramTypeToString(ProgramType(PT_Emergency)));
	//pItem->SetIcon(&m_iconSetting);
	//m_boxPTY.AddItem(pItem,PT_Emergency);


	//Set
	m_btnOK.Initialize(IDC_OK,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnOK.SetSize((g_iClientWidth-240-margin_x)/2,80);
	m_btnOK.SetPos(-(g_iClientWidth-240-margin_x)/4,-g_iClientHeight/2+45);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_gray);
	m_btnOK.SetCaptionHeight(32);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingRdsPTY::OnNotify,this);

	//Cancel
	m_btnCancel.Initialize(IDC_CANCEL,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancel.SetSize((g_iClientWidth-240-margin_x)/2,80);
	m_btnCancel.SetPos((g_iClientWidth-240-margin_x)/4,-g_iClientHeight/2+45);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_gray);
	m_btnCancel.SetCaptionHeight(32);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingRdsPTY::OnNotify,this);

	return TRUE;
}


void CALLBACK CPanelSettingRdsPTY::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingRdsPTY *panel=(CPanelSettingRdsPTY*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingRdsPTY::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_RADIO_BOX_SETTING_PTY:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;

			RETAILMSG(1,(_T("==PTY,index: %d, id_item: %d\r\n"),index,id_item));

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
					SendRadioCommand(RADIO_CMD_SELECT_PTY,id_item);
					SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_RADIO);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_OK:
		{
			SendRadioCommand(RADIO_CMD_SELECT_PTY,m_boxPTY.GetCurFocusItemID());
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

