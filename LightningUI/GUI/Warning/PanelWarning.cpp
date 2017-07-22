#include "PanelWarning.h"

#include "../../CarPlay/CarplayAdapter.h"
#if CVTE_EN_ACCON_WARNING
#include "../Warning/WarningProxy.h"
#endif

CPanelWarning::CPanelWarning(void)
{
	m_dwWarningShowTick = 0;
	m_nShowTime = INFINITE;
}

CPanelWarning::~CPanelWarning(void)
{
}
BOOL CPanelWarning::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelWarning::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

#if CVTE_EN_CARPLAY
	CCarplayAdapter::GetInstance()->SetAccOnWarning(FALSE);
	CarplayAPIUnborrowScreen(NULL);
	CarplayAPIUnborrowMainAudio(NULL);
#endif
}
void CPanelWarning::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(0.1f);
	SetAlpha(1.0f,TRUE);

	m_bEnableQuit = FALSE;
	SetShowTime(iParam);
	RETAILMSG(1, (TEXT("MSG:[%s]: iParam:0x%X, idAni:%d lpAniPara:%d\r\n"),TEXT(__FUNCTION__),iParam,idAni,lpAniPara));
#if CVTE_EN_CARPLAY
	CCarplayAdapter::GetInstance()->SetAccOnWarning(TRUE);
	//CarplayAPIUnborrowScreen(NULL);
	CarplayAPIBorrowScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,NULL);
	CarplayAPIBorrowMainAudio(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,NULL);
#endif

	m_dwWarningShowTick = GetTickCount();
}

void CPanelWarning::Render()
{
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;

	if(!BeginRender())return;

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};
    COLORGL cr_yellow ={0.7,0.8,0.12,1.0};
	COLORGL cr_blue ={0.4,0.77,0.85,1.0};

	//m_tagBkBlur.SetAlpha(m_fCurAlpha);
	m_tagBkBlur.Render();

#if 0
	//device status
	SIZE si;

	if(m_pTextGL)
	{
		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);

		if(m_strTitle.String())
		{
			m_pTextGL->SetString(m_strTitle.String());
			m_pTextGL->SetCharScale(1.0f,1.0f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),160);
			m_pTextGL->Render();
		}

		if(m_strNotification.String())
		{
			m_pTextGL->SetString(m_strNotification.String());
			m_pTextGL->SetCharScale(0.8f,0.8f);
			m_pTextGL->GetExtension(&si);
	// 		m_pTextGL->SetPos(-310,m_strTitle.String()?-20:26);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),80);
	// 		m_pTextGL->EnableAutoChangeLine(TRUE,620);
			m_pTextGL->Render();
	// 		m_pTextGL->EnableAutoChangeLine(FALSE);
		}
	}
#endif

	//m_btnAgree.SetAlpha(m_fCurAlpha);
	m_btnAgree.Render();

	if(m_nShowTime != INFINITE)
	{
		if( GetTickCount() - m_dwWarningShowTick > m_nShowTime)
		{
			m_bEnableQuit = TRUE;
#if CVTE_EN_ACCON_WARNING
			CWarningProxy::GetInstance()->LeaveWarningPage();
#endif
			RETAILMSG(1, (TEXT("MSG:[%s]: Auto EXIT warning panel.\r\n"), TEXT(__FUNCTION__)));
		}
	}

	EndRender();

}
BOOL CPanelWarning::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
	VIEW_STATE vs;
	SIZE siBk;

	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	 COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

	 SIZE si={g_iClientWidth/2-10,40};

	 m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_902));
	 siBk=m_tagBkBlur.GetTexture()->GetImageSize();
	 m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/siBk.cx,GLfloat(g_iClientHeight)/siBk.cy);
	 vs=m_tagBkBlur.GetCurViewState();
	 //vs.fRotateX=-180;
	 m_tagBkBlur.SetIniViewState(&vs);


// 	 m_tagTitleWarning.Initialize(0,this,NULL,NULL,
// 		 NULL,NULL,
// 		 NULL,NULL,
// 		 NULL,NULL);
// 	 m_tagTitleWarning.LinkTextGL(pTextGL);
// 	 m_tagTitleWarning.SetCaption(L"CAUTION");
// 	 m_tagTitleWarning.ShowCaption(TRUE);
// 	 m_tagTitleWarning.SetCaptionPos(BTN_CAPTION_CENTER);
// 	 m_tagTitleWarning.SetCaptionColor(&cr);
// 	 m_tagTitleWarning.SetCaptionHeight(40);
// 	 m_tagTitleWarning.SetSize(&si);
// 	 m_tagTitleWarning.SetPos(0,200);
// 	 m_tagTitleWarning.SetNotifyFunc(&CPanelWarning::OnNotify,this);
// 	 m_tagTitleWarning.EnableDrawBack(FALSE);


	m_btnAgree.Initialize(IDC_NB_BTN_OK,this,pSkinManager->GetTexture(TEXID_096),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);

	m_btnAgree.SetScale(1.0f/pix_ratio,1.0f);
	m_btnAgree.SetPos(0,-g_iClientHeight/2+37);
	m_btnAgree.SetTouchRate(2.0f,3.0f);
	m_btnAgree.SetNotifyFunc(&CPanelWarning::OnNotify,this);
	
	return TRUE;
}


void CPanelWarning::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelWarning::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelWarning::OnTouchMove(POINT *pt)
{

}

void CALLBACK CPanelWarning::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelWarning *panel=(CPanelWarning*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelWarning::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_NB_BTN_OK://ok
#if CVTE_EN_ACCON_WARNING
		m_bEnableQuit = TRUE;
		RETAILMSG(1, (TEXT("MSG:[%s]: Exit Warning page\r\n"),TEXT(__FUNCTION__)));
		CWarningProxy::GetInstance()->LeaveWarningPage();
#endif
		break;
	default:
		break;
	}
}

void CPanelWarning::OnReverseModeChange(BOOL bReverse)
{
	//RETAILMSG(1,(_T("MSG:[CPanelWarning::OnReverseModeChange]: %d\r\n"),bReverse));
	CPanelGL::OnReverseModeChange(bReverse);
#if CVTE_EN_ACCON_WARNING
	//BUG0003485:播放carplay音乐关ACC，开ACC，开ACC过程中进入倒车，直到听到音乐播放再退出倒车，闪现警告界面
	if(!bReverse && (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_WARNING,PAGE_ACCON_WARNING_MAIN)))
	{
		if( GetTickCount() - m_dwWarningShowTick > m_nShowTime)
		{
			m_bEnableQuit = TRUE;
			CWarningProxy::GetInstance()->LeaveWarningPage();
			RETAILMSG(1, (TEXT("MSG:[%s]: Auto EXIT warning panel.\r\n"), TEXT(__FUNCTION__)));
		}
	}
#endif
}