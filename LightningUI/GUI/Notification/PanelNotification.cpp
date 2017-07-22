#include "PanelNotification.h"
#include "../../resource.h"

CPanelNotification::CPanelNotification(void)
{
	m_idCaller=0;
	m_pCallbackFunc=NULL;
	m_lpCallbackParam=NULL;
	m_type=0;

	m_dwLastNotificationTick=0;
	m_dwNotificationShowTime=5000;
	m_idNotificationIcon=0;
	m_bShowingNotification=FALSE;

	m_bAutoHide=FALSE;
// 	m_idLastUIClass=UI_MAINMENU;
}

CPanelNotification::~CPanelNotification(void)
{
}
BOOL CPanelNotification::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelNotification::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelNotification::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(0.1f);
	SetAlpha(1.0f,TRUE);


	m_dwLastNotificationTick=GetTickCount();

}

void CPanelNotification::Render()
{
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#else
	m_tagBkBlur.Render();
	m_tagBkBlur.SetAlpha(m_fCurAlpha);
#endif

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};
	//device status
	SIZE si;

	if(m_pTextGL)
	{
		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);

		if(m_strTitle.String())
		{
			m_pTextGL->SetString(m_strTitle.String());
			m_pTextGL->EnableAutoEllipsis(TRUE,g_iScreenWidth-60);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)			
			m_pTextGL->SetCharScale(1.0f,1.0f);
#else
			m_pTextGL->SetCharScale(0.9f,0.9f);
#endif
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),160);
			m_pTextGL->Render();
			m_pTextGL->EnableAutoEllipsis(FALSE);
		}

		if(m_strNotification.String())
		{
			m_pTextGL->SetString(m_strNotification.String());
			m_pTextGL->EnableAutoEllipsis(TRUE,g_iScreenWidth-60);
			m_pTextGL->SetCharScale(0.8f,0.8f);
			m_pTextGL->GetExtension(&si);
	// 		m_pTextGL->SetPos(-310,m_strTitle.String()?-20:26);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),80);
	// 		m_pTextGL->EnableAutoChangeLine(TRUE,620);
			m_pTextGL->Render();
			m_pTextGL->EnableAutoEllipsis(FALSE);
	// 		m_pTextGL->EnableAutoChangeLine(FALSE);
		}
	}

	if(GetTickCount() - m_dwLastNotificationTick > m_dwNotificationShowTime)
	{
		if(m_bShowingNotification)
		{
			m_bShowingNotification = FALSE;
			//quit
			SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
		}
	}

	m_btOK.SetAlpha(m_fCurAlpha);
	m_btCancel.SetAlpha(m_fCurAlpha);

	m_btOK.Render();
	m_btCancel.Render();


	EndRender();

}
BOOL CPanelNotification::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
	VIEW_STATE vs;
	SIZE siBk;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	
	ShowBackground(FALSE);

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(g_iClientWidth, g_iClientHeight);
	m_tagBk.SetPos(0, 0);

	SIZE si={g_iClientWidth/2-10, 100};

	m_btOK.Initialize(IDC_NB_BTN_OK,this,
		pSkinManager->GetTexture(TEXID_1101),NULL,
		pSkinManager->GetTexture(TEXID_1102),NULL,
		pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btOK.LinkTextGL(pTextGL);
	m_btOK.SetCaption(GetResourceString(IDS_YES));
	m_btOK.ShowCaption(TRUE);
	m_btOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btOK.SetCaptionHeight(28);
	m_btOK.SetSize(&si);
	m_btOK.SetPos(-g_iClientWidth/4,-g_iClientHeight/2+100);
	m_btOK.SetNotifyFunc(&CPanelNotification::OnNotify,this);


	m_btCancel.Initialize(IDC_NB_BTN_CANCEL,this,
		pSkinManager->GetTexture(TEXID_1101),NULL,
		pSkinManager->GetTexture(TEXID_1102),NULL,
		pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btCancel.LinkTextGL(pTextGL);
	m_btCancel.SetCaption(GetResourceString(IDS_NO));
	m_btCancel.ShowCaption(TRUE);
	m_btCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btCancel.SetCaptionHeight(28);
	m_btCancel.SetSize(&si);
	m_btCancel.SetPos(g_iClientWidth/4,-g_iClientHeight/2+100);
	m_btCancel.SetNotifyFunc(&CPanelNotification::OnNotify,this);

#else
	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2000)));
	siBk=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/siBk.cx,GLfloat(g_iClientHeight)/siBk.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	//notification icons:
	// 	m_iconInfo.Initialize(IDC_STATUSBAR_ICON_INFO,this,m_pSkinManager->GetTexture(TEXID_504));
	// 	m_iconInfo.SetPos(0,90);
	// 	m_iconInfo.SetShowScale(1.25);
	//
	// 	m_iconError.Initialize(IDC_STATUSBAR_ICON_ERROR,this,m_pSkinManager->GetTexture(TEXID_503));
	// 	m_iconError.SetPos(0,90);
	// 	m_iconError.SetShowScale(1.25);
	//
	// 	m_iconSpeech.Initialize(IDC_STATUSBAR_ICON_SPEECH,this,m_pSkinManager->GetTexture(TEXID_194));
	// 	m_iconSpeech.SetPos(0,90);
	// 	m_iconSpeech.SetShowScale(1.25);
	//
	// 	m_iconQuest.Initialize(IDC_STATUSBAR_ICON_QUEST,this,m_pSkinManager->GetTexture(TEXID_505));
	// 	m_iconQuest.SetPos(0,90);
	// 	m_iconQuest.SetShowScale(1.25);

	// 	COLORGL cr={0.0,1.0,0.0,1.0};
	// 	COLORGL cr2={1.0,0.0,0.0,1.0};
	// 	wchar_t caption[8];

	SIZE si={g_iClientWidth/2-10,160-10};

	m_btOK.Initialize(IDC_NB_BTN_OK,this,pSkinManager->GetTexture(TEXID_516),NULL,
		pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	// 	swprintf_s(caption,8,L"%c",CHAR_OK);
	m_btOK.LinkTextGL(pTextGL);
	m_btOK.SetCaption(GetResourceString(IDS_YES));
	m_btOK.ShowCaption(TRUE);
	m_btOK.SetCaptionPos(BTN_CAPTION_CENTER);
	// 	m_btOK.SetCaptionColor(&cr);
	m_btOK.SetCaptionHeight(28);
	//m_btOK.SetSize(&si);
	m_btOK.SetPos(-g_iClientWidth/4,-g_iClientHeight/2+100);
	m_btOK.SetNotifyFunc(&CPanelNotification::OnNotify,this);


	m_btCancel.Initialize(IDC_NB_BTN_CANCEL,this,pSkinManager->GetTexture(TEXID_533),NULL,
		pSkinManager->GetTexture(TEXID_534),NULL,
		NULL,NULL,
		NULL,NULL);
	//swprintf_s(caption,8,L"%c",CHAR_CANCEL);
	m_btCancel.LinkTextGL(pTextGL);
	m_btCancel.SetCaption(GetResourceString(IDS_NO));
	m_btCancel.ShowCaption(TRUE);
	m_btCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	// 	m_btCancel.SetCaptionColor(&cr2);
	m_btCancel.SetCaptionHeight(28);
	//m_btCancel.SetSize(&si);
	m_btCancel.SetPos(g_iClientWidth/4,-g_iClientHeight/2+100);
	m_btCancel.SetNotifyFunc(&CPanelNotification::OnNotify,this);

	/*
	//toolbar:
	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_151));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_152));

	m_iconReturnUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_153));
	m_iconReturnDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_154));

	SIZE si1={140,80};
	m_btnHome.Initialize(IDC_NB_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
	m_pSkinManager->GetTexture(TEXID_056),NULL,
	NULL,NULL,
	NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelNotification::OnNotify,this);
	m_btnHome.SetSize(&si1);
	m_btnHome.SetPos(-330,-200);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);

	m_btnReturn.Initialize(IDC_NB_BTN_RETURN,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
	m_pSkinManager->GetTexture(TEXID_056),NULL,
	NULL,NULL,
	NULL,NULL);
	m_btnReturn.SetNotifyFunc(&CPanelNotification::OnNotify,this);
	m_btnReturn.SetSize(&si1);
	m_btnReturn.SetPos(330,-200);
	m_btnReturn.SetIcon(&m_iconReturnUp,&m_iconReturnDown);
	*/
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	return TRUE;
}


void CPanelNotification::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelNotification::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelNotification::OnTouchMove(POINT *pt)
{

}
void CPanelNotification::HideNotification()
{
	m_bShowingNotification=FALSE;
	//quit
// 	::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,m_idLastPanel,NULL);
// 	::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,m_idLastPanel,m_iLastPanelParam);
	OnBack();
}

void CALLBACK CPanelNotification::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelNotification *panel=(CPanelNotification*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelNotification::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_NB_BTN_CANCEL://cancel
		if(m_bAutoHide)
		{
			HideNotification();
		}
		if(m_pCallbackFunc)
		{
			m_pCallbackFunc(m_idCaller,NB_CANCEL,0,NULL,m_lpCallbackParam);
		}
		break;
	case IDC_NB_BTN_OK://ok
		if(m_bAutoHide)
		{
			HideNotification();
		}
		if(m_pCallbackFunc)
		{
			m_pCallbackFunc(m_idCaller,NB_OK,0,NULL,m_lpCallbackParam);
		}
		break;
	case IDC_NB_BTN_HOME:
		//::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_MAINMENU,NULL);
		break;
	case IDC_NB_BTN_RETURN:
		OnBack();
		PlaySoundEffect(0);
		break;
	}
}
void CPanelNotification::SetNotification(NBMSG *pMsg)
{
	if(!pMsg)
		return;

	m_idCaller=pMsg->idCaller;
	m_pCallbackFunc=pMsg->pCallbackFunc;
	m_lpCallbackParam=pMsg->lpCallbackParam;
	m_type=pMsg->type;

	m_dwNotificationShowTime=pMsg->dwShowTime;
	m_idNotificationIcon=pMsg->idIcon;
	m_strNotification = pMsg->pstrNotification;
	m_strTitle=pMsg->pstrTitle;

	m_bAutoHide=pMsg->bAutoHide;

	m_dwLastNotificationTick=GetTickCount();
	m_bShowingNotification = TRUE;

	m_btOK.Show(m_type!=-1);
	m_btCancel.Show(m_type!=-1);
}
void CPanelNotification::OnBack()
{
	m_bShowingNotification = FALSE;

	SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
}
