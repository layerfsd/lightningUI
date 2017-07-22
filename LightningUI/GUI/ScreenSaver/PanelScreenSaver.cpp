#include "PanelScreenSaver.h"

CPanelScreenSaver::CPanelScreenSaver(void)
{

}

CPanelScreenSaver::~CPanelScreenSaver(void)
{
}
BOOL CPanelScreenSaver::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelScreenSaver::OnEnterPanel(UINT iParam,UINT idAni)
{
	CPanelGL::OnEnterPanel(iParam,idAni);

// 	VIEW_STATE vs=GetCurViewState();
// 	vs.fRotateX=180;
// 	vs.fScaleX=0.2;
// 	vs.fScaleY=0.2;
// 	SetCurViewState(&vs);
// 	vs.fRotateX=0;
// 	vs.fScaleX=1.0;
// 	vs.fScaleY=1.0;
// 	SetAimViewState(&vs);

// 	//re-animate:
// 	SetAnimateFrameIndex(0);

	SetAlpha(0.2f);
	SetAlpha(1,TRUE);


	//update status bar:
	if(m_pStatusBar)
	{
		//update status bar UI class id:
		m_pStatusBar->SetCurUIClass(m_ID);

		m_pStatusBar->SetCurDisplayPanel(m_ID);

// 		m_pStatusBar->SetTitle((const wchar_t*)::LoadString( m_hInstance, IDS_MAINMENU_TV+g_idLanguageOffset, NULL, 0 ));
	}

}

void CPanelScreenSaver::Render()
{
	static COLORGL crb={0.03125,0.03125,0.03125,1.0};

	if(!BeginRender())return;


	DrawRect(0,0,800,480,&crb);

	m_barClock.SetAlpha(m_fCurAlpha);
	m_barClock.Render();

	EndRender();

}
BOOL CPanelScreenSaver::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	CLOCKBARSTRUCTURE stru;
	memset(&stru,sizeof(stru),0);
	stru.idTexBase=200;
	stru.idTexHour=201;
	stru.idTexMinute=202;
	stru.idTexSecond=203;
	stru.bDashboardOnly=TRUE;

	m_barClock.Initialize(0,this,&stru,pSkinManager,pTextGL);
	m_barClock.SetShowScale(1.30f);
	m_barClock.SetPos(0,0);

	m_bShowStatusBar = FALSE;


	return TRUE;
}


void CPanelScreenSaver::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelScreenSaver::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

	OnBack();
}

void CPanelScreenSaver::OnTouchMove(POINT *pt)
{

}


void CALLBACK CPanelScreenSaver::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(1,(_T("CPanelScreenSaver OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelScreenSaver *panel=(CPanelScreenSaver*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelScreenSaver::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{

}
void CPanelScreenSaver::OnBack()
{
	//quit
	SwitchPage(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
		GET_PANEL_PAGE_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
		PAGE_SWITCH_ANI_NONE,0);//quit
}
