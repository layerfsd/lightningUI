#include "PanelExplorer.h"


CPanelExplorer::CPanelExplorer(void)
{
	m_idCaller=0;
	m_pCallbackFunc=NULL;
	m_lpCallbackParam=NULL;

}

CPanelExplorer::~CPanelExplorer(void)
{
}
BOOL CPanelExplorer::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelExplorer::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);


// 	m_Explorer.MakeChildrenCtlAni(ANIM_FLY_OUT);

}

void CPanelExplorer::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	if(idAni != ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}

}

void CPanelExplorer::Render()
{

	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;

	if(!BeginRender())return;

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

// 	DrawRect(pTexBack,0,0,794-32,356-32,16,&cr,TRUE);

	m_Explorer.SetAlpha(m_fCurAlpha);
	m_Explorer.Render();

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	EndRender();

}
BOOL CPanelExplorer::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	RETAILMSG(1, (_T("APP:MSG:GUI:Explorer: [CPanelExplorer::Initialize] \r\n")));

	m_Explorer.Initialize(0,this,m_pSkinManager);
	m_Explorer.LinkTextGL(m_pTextGL);

	/*
	//toolbar:
	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_151));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_152));
	m_iconReturnUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_153));
	m_iconReturnDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_154));

	SIZE si1={140,80};
	m_btnHome.Initialize(IDC_EXPLORER_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
		m_pSkinManager->GetTexture(TEXID_056),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelExplorer::OnNotify,this);
	m_btnHome.SetSize(&si1);
	m_btnHome.SetPos(-330,-200);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);

	m_btnReturn.Initialize(IDC_EXPLORER_BTN_RETURN,this,m_pSkinManager->GetTexture(TEXID_055),NULL,
		m_pSkinManager->GetTexture(TEXID_056),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnReturn.SetNotifyFunc(&CPanelExplorer::OnNotify,this);
	m_btnReturn.SetSize(&si1);
	m_btnReturn.SetPos(330,-200);
	m_btnReturn.SetIcon(&m_iconReturnUp,&m_iconReturnDown);

*/
	m_btnHome.Initialize(IDC_EXPLORER_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_012),NULL,
		m_pSkinManager->GetTexture(TEXID_012),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelExplorer::OnNotify,this);
	//m_btnHome.SetSize(80,80);
	m_btnHome.SetPos(g_iClientWidth/2-50,g_iClientHeight/2-50);
	m_btnHome.EnableClickAni(TRUE);

	return TRUE;
}


void CPanelExplorer::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelExplorer::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

	if(m_Explorer.IsCurFocusFile() && m_Explorer.IsClickOnItem(pt))
	{
		if(m_pCallbackFunc)
		{
			m_pCallbackFunc(m_idCaller,0,0,LPVOID((CM_String(m_Explorer.GetPath())+CM_String(m_Explorer.GetFileName())).String()),m_lpCallbackParam);
		}
		//quit
		SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_DEFAULT,0/*m_iPreviousPanelParam*/);
	}

	//
	PlaySoundEffect(0);
}
void CPanelExplorer::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

}
void CPanelExplorer::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_EXPLORER_BTN_HOME:
		//quit
		SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_RIGHT,PAGE_SETTINGS_MAIN);
		break;
	case IDC_EXPLORER_BTN_RETURN:
		OnBack();
		break;
	}
}
void CALLBACK CPanelExplorer::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_EXPLORER,(_T("APP:MSG:GUI:Explorer: [CPanelExplorer::OnNotify] id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelExplorer *panel=(CPanelExplorer*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wMsg,lpPara);
}

#if CVTE_EN_KEY_PROXY
BOOL CPanelExplorer::OnKey(UINT idKey,UINT idStatus)
{
	switch (idKey)
	{
	case KEY_ENTER:
		{
			if ( idStatus == EV_KEY_UP )
			{
				m_Explorer.OnEnter();

				if(m_Explorer.IsCurFocusFile())
				{
					if(m_pCallbackFunc)
					{
						m_pCallbackFunc(m_idCaller,0,0,LPVOID((CM_String(m_Explorer.GetPath())+CM_String(m_Explorer.GetFileName())).String()),m_lpCallbackParam);
					}
				}
			}
		}
		break;
	case KEY_NEXT:
		{
			if ( (idStatus == EV_KEY_SHORT_PRESS) || (idStatus == EV_KEY_PRESS_HOLD) )
			{
				m_Explorer.MoveToNext();
			}
		}
		break;
	case KEY_PREVIOUS:
		{
			if ( (idStatus == EV_KEY_SHORT_PRESS) || (idStatus == EV_KEY_PRESS_HOLD) )
			{
				m_Explorer.MoveToPrevious();
			}
		}
		break;
	default:
		break;
	}

	return TRUE;

}
#else
BOOL CPanelExplorer::OnKey(UINT idKey)
{
	switch (idKey)
	{
	case KEY_MCU_ENTER:
		{
			m_Explorer.OnEnter();

			if(m_Explorer.IsCurFocusFile())
			{
				if(m_pCallbackFunc)
				{
					m_pCallbackFunc(m_idCaller,0,0,LPVOID((CM_String(m_Explorer.GetPath())+CM_String(m_Explorer.GetFileName())).String()),m_lpCallbackParam);
				}
			}
		}
		break;
	case KEY_MCU_SEEK_NEXT:
		{
			m_Explorer.MoveToNext();
		}
		break;
	case KEY_MCU_SEEK_PREV:
		{
			m_Explorer.MoveToPrevious();
		}
		break;
	}

	return TRUE;

}
#endif

void CPanelExplorer::SetNotification(EXPLORERMSG *pMsg)
{
	if(!pMsg)
		return;

	m_idCaller=pMsg->idCaller;
	m_pCallbackFunc=pMsg->pCallbackFunc;
	m_lpCallbackParam=pMsg->lpCallbackParam;
	m_strTitle=pMsg->lpTitle;
	m_strPath=pMsg->lpPath;
	m_strFilter=pMsg->lpFileFilter;

	m_Explorer.SetCurPath(pMsg->lpPath);
	m_Explorer.SetFilter(pMsg->lpFileFilter);
	m_Explorer.SetNotifyFunc(NULL,NULL);//just enable notify

}
void CPanelExplorer::OnBack()
{
	if(m_Explorer.IsRoot())
	{
		//quit
		SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_RIGHT,0/*m_iPreviousPanelParam*/);
	}
	else
	{
		m_Explorer.GotoParent();
	}
}
