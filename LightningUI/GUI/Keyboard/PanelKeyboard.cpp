#include "PanelKeyboard.h"

CPanelKeyboard::CPanelKeyboard(void)
{
	m_idCaller=0;
	m_pCallbackFunc=NULL;
	m_lpCallbackParam=NULL;

	m_type = INPUT_LETTERS_CAP;
	m_bShowStatusBar=FALSE;
	m_bAutoReturn = TRUE;
}

CPanelKeyboard::~CPanelKeyboard(void)
{
}
BOOL CPanelKeyboard::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelKeyboard::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}

void CPanelKeyboard::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

void CPanelKeyboard::Render()
{
// 	static wchar_t str[16];
// 	static COLORGL cr2={1,1,1,1};
// 
// 	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_100):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,0.8};
// 	cr.a=m_pSkinManager->GetConfig()->fUITransparency;

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

	m_Keyboard.SetAlpha(m_fCurAlpha);
	m_Keyboard.Render();

	EndRender();

}
BOOL CPanelKeyboard::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
#else
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET


	KEYBOARDSTRUCTURE stru;
	memset(&stru,sizeof(stru),0);
	stru.siBorder.cx=g_iClientWidth;
	stru.siBorder.cy=g_iClientHeight;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	stru.idTexBtnUp=TEXID_079;
	stru.idTexBtnDown=TEXID_242;

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(g_iClientWidth, g_iClientHeight);
	m_tagBk.SetPos(0, 0);

#else
	stru.idTexBtnUp=TEXID_606;
	stru.idTexBtnDown=TEXID_607;
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	stru.crCaptionUp=cr_gray;
	stru.crCaptionDown=cr_gray;

	m_Keyboard.Initialize(0,this,&stru,m_pSkinManager,pTextGL);


	return TRUE;
}


void CPanelKeyboard::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelKeyboard::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

// 	m_Keyboard.MakeChildrenCtlAni(ANIM_FLY_IN);

	if(m_Keyboard.IsOnOK(pt))
	{
		if(m_pCallbackFunc)
		{
			m_pCallbackFunc(m_idCaller,0,0,LPVOID(m_Keyboard.GetInputString()),m_lpCallbackParam);
		}
		if(m_bAutoReturn)
		{
			SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
		}
	}
	else if(m_Keyboard.IsOnCancel(pt))
	{
		SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
	}

	//
	PlaySoundEffect(0);
}
void CPanelKeyboard::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

}


void CPanelKeyboard::SetNotification(KEYBOARDMSG *pMsg)
{
	if(!pMsg)
		return;

	RETAILMSG(1,(_T("CPanelKeyboard::SetNotification: title:%s\r\n"),pMsg->pStrTitle));

	m_idCaller=pMsg->idCaller;
	m_pCallbackFunc=pMsg->pCallbackFunc;
	m_lpCallbackParam=pMsg->lpCallbackParam;
	m_type=pMsg->type;
	m_bAutoReturn=pMsg->bAutoReturn;

	m_Keyboard.SetCaption(pMsg->pStrTitle);
	m_Keyboard.SetInputString(pMsg->pStrIni);
	m_Keyboard.SetInputType(pMsg->type);
	m_Keyboard.SetInputMaxLen(pMsg->iMaxInputLen);
	m_Keyboard.SetNotifyFunc(NULL,NULL);//just enable notify
}
void CPanelKeyboard::OnBack()
{
	//quit
	SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
}
