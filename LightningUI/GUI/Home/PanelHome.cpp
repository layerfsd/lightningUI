
#include "../Bluetooth FLC/BluetoothCommon.h"
#include "PanelHome.h"
#include "../../resource.h"
#include "../Media/MediaCommon.h"
#include "../Radio/RadioCommon.h"

#define WIDGET_WIDTH			190
#define WIDGET_HEIGHT			280
#define WIDGET_CX					190
#define WIDGET_PX					-285
#define WIDGET_PY					15

// extern BOOL	g_bBTPowerON;
extern const wchar_t* g_strDefaultBackground;

CPanelHomePage::CPanelHomePage(void)
{
	m_idCurSubpage = PAGE_HOME_WIDGET;
	m_bPhoneCallActive = FALSE;
	m_bHasCanbus = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	m_iPageSelectNum = 1;
	m_ibtnIDPageSelect1 = BS_UP;
	m_ibtnIDPageSelect2 = BS_UP;
	m_ibtnIDStatusTime = BS_UP;
#endif
}

CPanelHomePage::~CPanelHomePage(void)
{

}
void CPanelHomePage::Render()
{
	if(!BeginRender())return;

	GLfloat flumin=m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance*1.2f;
	if(flumin>1.0f)
		flumin=1.0f;
	m_tagBkOrigin.SetAlpha(m_fCurAlpha);
	m_tagBkOrigin.SetLuminance(flumin);
	m_tagBkOrigin.Render();

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->SetAlpha(m_fCurAlpha);
		panel->Render();
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	m_tagButtonStatusBar.SetAlpha(m_fCurAlpha);
	m_tagButtonStatusBar.SetLuminance(flumin);
	m_tagButtonStatusBar.Render();

	m_btnPageSelect1.SetAlpha(m_fCurAlpha);
	m_btnPageSelect1.SetLuminance(flumin);
	m_btnPageSelect1.Render();

	m_btnPageSelect2.SetAlpha(m_fCurAlpha);
	m_btnPageSelect2.SetLuminance(flumin);
	m_btnPageSelect2.Render();

	UpdateTime();
	m_tagTime.SetAlpha(m_fCurAlpha);
	m_tagTime.Render();
	if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H)
	{
		m_tagAM.SetAlpha(m_fCurAlpha);
		m_tagAM.Render();
	}

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}
#else
	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}
#endif

	EndRender();
}
void CPanelHomePage::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

// 	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
// 	if(panel)
// 	{
// 		panel->OnEnterPanel(iParam,idAni,lpAniPara);
// 	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	if(m_pStatusBar)
	{
		//RETAILMSG(1,(TEXT("MSG:[%s]: Hide StatusBar\r\n"), TEXT(__FUNCTION__)));
		m_pStatusBar->Show(FALSE);
	}
#endif
}
void CPanelHomePage::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0,TRUE);

// 	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
// 	if(panel)
// 	{
// 		panel->OnQuitPanel(iParam,idAni,lpAniPara);
// 	}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	if(m_pStatusBar)
	{
		//RETAILMSG(1,(TEXT("MSG:[%s]: Show StatusBar\r\n"), TEXT(__FUNCTION__)));
		m_pStatusBar->Show(TRUE);
	}
#endif
}
BOOL CPanelHomePage::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowStatusBar(TRUE);

	//define ani:
	SetAniMoveIn(ANIM_ZOOMIN_IN);
	SetAniPrePanelMoveOut(ANIM_ZOOMIN_OUT);

	SetAniMoveOut(ANIM_ZOOMOUT_OUT);
	SetAniNextPanelMoveIn(ANIM_ZOOMOUT_IN);

	//
	WIN32_FIND_DATA   wfd;  
	BOOL   bGetBackFile;  
	HANDLE   hFind;  

	hFind   =   FindFirstFile(m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName.String(),   &wfd);  
	bGetBackFile   =   (hFind   !=   INVALID_HANDLE_VALUE);  
	FindClose(hFind); 

	int cx=256;//128;//g_iClientWidth;//
	int cy=128;//64;//512;

	if(bGetBackFile)
	{
		m_pSkinManager->AddTextureFromFile(TEXID(2000),m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName.String(),cx,cy,FALSE,GL_LINEAR,TRUE,5);
		m_pSkinManager->AddTextureFromFile(TEXID(2001),m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName.String(),g_iClientWidth,512);
	}
	else
	{
		m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName=g_strDefaultBackground;
		m_pSkinManager->AddTextureFromFile(TEXID(2000),g_strDefaultBackground,cx,cy,FALSE,GL_LINEAR,TRUE,5);
		m_pSkinManager->AddTextureFromFile(TEXID(2001),g_strDefaultBackground,g_iClientWidth,512);
	}



	SIZE si;
	m_tagBkOrigin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2001)));
	si=m_tagBkOrigin.GetTexture()->GetImageSize();
	m_tagBkOrigin.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	VIEW_STATE vs=m_tagBkOrigin.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkOrigin.SetIniViewState(&vs);

	//panels:
	m_panelWidget.Initialize(PAGE_HOME_WIDGET,this,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	//Init callback
	InitWidgetCallback();

	//button status
	SIZE siButtonStatus={g_iClientWidth,80};
	m_tagButtonStatusBar.Initialize(NULL,this,m_pSkinManager->GetTexture(TEXID_159),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagButtonStatusBar.SetSize(&siButtonStatus);
	m_tagButtonStatusBar.SetPos(0,-g_iClientHeight/2+40);

	m_iconPage.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_133));
	m_iconPageFocus.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_134));

	m_btnPageSelect1.Initialize(NULL,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnPageSelect1.SetTouchRate(1.2f,1.2f);
	m_btnPageSelect1.EnableDrawBack(FALSE);
	m_btnPageSelect1.SetIcon(&m_iconPageFocus, &m_iconPage);
	m_btnPageSelect1.SetPos(-48,-g_iClientHeight/2+40);
	//m_btnPageSelect1.SetNotifyFunc(&CPanelHomePage::OnNotify,this);

	m_btnPageSelect2.Initialize(NULL,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnPageSelect2.SetTouchRate(1.2f,1.2f);
	m_btnPageSelect2.EnableDrawBack(FALSE);
	m_btnPageSelect2.SetIcon(&m_iconPage, &m_iconPageFocus);
	m_btnPageSelect2.SetPos(48,-g_iClientHeight/2+40);
	//m_btnPageSelect2.SetNotifyFunc(&CPanelHomePage::OnNotify,this);

	SIZE si60_40={170,40};
	COLORGL crw={1.0f,1.0f,1.0f,1.0};
	m_tagAM.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAM.SetSize(&si60_40);
	// 	m_tagAM.SetCaption(str);
	m_tagAM.LinkTextGL(pTextGL);
	// 	m_tagAM.SetPos(-276,220);
	// 	m_tagAM.SetPos(g_iClientWidth/2-140,g_iClientHeight/2-50-2);
	m_tagAM.SetPos(-g_iClientWidth/2+180,-g_iClientHeight/2+34);
	m_tagAM.SetCaptionHeight(25);
	m_tagAM.EnableDrawBack(FALSE);
	m_tagAM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagAM.SetCaptionColor(&crw);


	m_tagTime.Initialize(IDC_HOME_PAGE_STATUS_TIME,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTime.SetSize(&si60_40);
	m_tagTime.SetCaption(_T("00:00"));
	m_tagTime.LinkTextGL(pTextGL);
	m_tagTime.SetPos(-g_iClientWidth/2+70,-g_iClientHeight/2+40);
	m_tagTime.SetCaptionHeight(40);
	m_tagTime.EnableDrawBack(FALSE);
	m_tagTime.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagTime.SetCaptionColor(&crw);
	m_tagTime.SetNotifyFunc(&CPanelHomePage::OnNotify,this);
#endif

	return TRUE;
}
void CPanelHomePage::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
	m_panelWidget.OnAvailableSourceChanged(wParam,lParam);
}
CWidgetGL*	CPanelHomePage::GetWidget(UINT idWidget)
{
	return m_panelWidget.GetWidget(idWidget);
}
void CPanelHomePage::OnCanbusStatus(WPARAM wParam, LPARAM lParam)
{
	m_bHasCanbus = wParam;

	m_panelWidget.OnCanbusStatus(wParam,lParam);
}
void CPanelHomePage::OnWidgetChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_MAINMENU,(_T("------CPanelHomePage OnWidgetChanged:%d,%d\r\n"),wParam,lParam));

	m_panelWidget.OnWidgetChanged(wParam,lParam);

}

#if CVTE_EN_KEY_PROXY
BOOL CPanelHomePage::OnKey(UINT idKey,UINT idStatus)
{
	switch (idKey)
	{
	case KEY_NEXT:

		break;
	case KEY_PREVIOUS:

		break;
	default:
		break;
	}

	return TRUE;
}
#else
BOOL CPanelHomePage::OnKey(UINT idKey)
{
	switch (idKey)
	{
	case KEY_MCU_SEEK_NEXT:

		break;
	case KEY_MCU_SEEK_PREV:

		break;
	}

	return TRUE;
}
#endif

void CPanelHomePage::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);


	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchDown(pt);
	}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	if(m_btnPageSelect1.IsPtnOnObject(pt))
	{
		m_ibtnIDPageSelect1 = BS_DOWN;
		return;
	}
	else if(m_btnPageSelect2.IsPtnOnObject(pt))
	{
		m_ibtnIDPageSelect2 = BS_DOWN;
		return;
	}
#endif
}
void CPanelHomePage::OperateWidget(UINT idUI,UINT idControl,UINT idOperation,LPVOID lpara)
{
	m_panelWidget.OperateWidget(idUI,idControl,idOperation,lpara);
}

void CPanelHomePage::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);


	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchUp(pt);
	}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	if(m_ibtnIDPageSelect1 == BS_DOWN)
	{
		if(m_btnPageSelect1.IsPtnOnObject(pt))
		{
			SetPageNum(1);
		}
		else if(m_btnPageSelect2.IsPtnOnObject(pt))
		{
			SetPageNum(2);
		}
	}
	else if(m_ibtnIDPageSelect2 == BS_DOWN)
	{
		if(m_btnPageSelect2.IsPtnOnObject(pt))
		{
			SetPageNum(2);
		}
		else if(m_btnPageSelect1.IsPtnOnObject(pt))
		{
			SetPageNum(1);
		}
	}

	m_ibtnIDPageSelect1 = BS_UP;
	m_ibtnIDPageSelect2 = BS_UP;

#endif
}
void CPanelHomePage::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchMove(pt);
	}
}


void CPanelHomePage::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
// 		SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
// 		SwitchPage(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
// 			GET_PANEL_PAGE_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
// 			PAGE_SWITCH_ANI_LEFT);
		break;
	}
}
void CPanelHomePage::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;

	m_panelWidget.OnPhoneCallState(wParam,lParam);
}
void CPanelHomePage::OnGesture(GESTUREINFO *pgi)
{
	if(!pgi)
		return;

	POINT pt={pgi->ptsLocation.x,pgi->ptsLocation.y};

	CPanelGL::OnGesture(pgi);

	// Handle gesture indicated by wParam or gi.dwID	
	
// 	if(pgi->dwID != GID_END && pgi->dwID != GID_BEGIN)
// 	{
// 		m_dwLastGestureTick = GetTickCount();
// 	}

	switch (pgi->dwID)
	{
	case GID_BEGIN:
		break;
	case GID_END:
		break;
	case GID_PAN:
		break;
	case GID_ROTATE:
		break;
	case GID_SCROLL:
		{
// 			m_bScrolling = TRUE;
// 			m_dwLastScrollTick = GetTickCount();

			LONG velocity = GID_SCROLL_VELOCITY (pgi->ullArguments );
			LONG angle =   GID_SCROLL_ANGLE (pgi->ullArguments );
			LONG direction = GID_SCROLL_DIRECTION (pgi->ullArguments );

			switch (direction )
			{
			case ARG_SCROLL_DOWN:
				break;
			case ARG_SCROLL_UP:
				break;
			case ARG_SCROLL_LEFT:
				break;
			case ARG_SCROLL_RIGHT:
				break;
			}
		}
		break;

	case GID_HOLD:
		break;
	case GID_SELECT:
		break;
	case GID_DOUBLESELECT:
		break;
	}

}

BOOL CPanelHomePage::IsReady()
{
	CPanelGL *panel=GetChildPanel(m_idCurSubpage);

	return CPanelGL::IsReady() && (panel?panel->IsReady():TRUE);
}
BOOL CPanelHomePage::IsAniReady(void)
{
	CPanelGL *panel=GetChildPanel(m_idCurSubpage);

	return CPanelGL::IsAniReady() && (panel?panel->IsAniReady():TRUE);
}

void CPanelHomePage::OnBack()
{
	SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_DEFAULT,0/*m_iPreviousPanelParam*/);
}
void CPanelHomePage::OnHome()
{

}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
void CALLBACK CPanelHomePage::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(1,(_T("MSG:CPanelHomePage OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelHomePage *panel=(CPanelHomePage*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}

void CPanelHomePage::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_HOME_PAGE_STATUS_TIME:
		SwitchPage(UI_CLASS_IDLE,0,PAGE_SWITCH_ANI_NONE,2);
		break;

	}
}

void CPanelHomePage::InitWidgetCallback()
{
	m_panelWidget.SetCallback(&CPanelHomePage::CallbackProc, this);
}

void CPanelHomePage::CallbackProc(UINT nPageNum, PVOID pContext)
{
	CPanelHomePage * pPanelHome = (CPanelHomePage *)pContext;
	if(pPanelHome != NULL)
	{
		pPanelHome->ChangePageSelectIcon(nPageNum);
	}
}

void CPanelHomePage::ChangePageSelectIcon(UINT nPageNum)
{
	switch (nPageNum)
	{
	case 1:
		m_btnPageSelect1.SetIcon(&m_iconPageFocus, &m_iconPage);
		m_btnPageSelect2.SetIcon(&m_iconPage, &m_iconPageFocus);
		break;
	case 2:
		m_btnPageSelect1.SetIcon(&m_iconPage, &m_iconPageFocus);
		m_btnPageSelect2.SetIcon(&m_iconPageFocus, &m_iconPage);
		break;
	default:
		break;
	}
}

void CPanelHomePage::SetPageNum(UINT nPageNum)
{
	switch(nPageNum)
	{
	case 1:
	case 2:
		m_iPageSelectNum = nPageNum;
		break;
	default:
		return;
		break;
	}
		
	ChangePageSelectIcon(m_iPageSelectNum);
	m_panelWidget.SetPage(m_iPageSelectNum);
}
void CPanelHomePage::UpdateTime()
{
	static DWORD last_tick=0;
	DWORD this_tick=GetTickCount();

	if(this_tick-last_tick>1000)//update per second
	{
		SYSTEMTIME sys_time;
		int hour1,hour2,min1,min2;
		wchar_t str[16]={0};
// 		RETAILMSG(DEBUG_STATUSBAR,(_T("UpdateTime...\r\n")));
		last_tick=this_tick;
		GetLocalTime(&sys_time);

		if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H)
		{
			m_tagAM.SetCaption(sys_time.wHour>=12?_T("PM"):_T("AM"));
			sys_time.wHour=sys_time.wHour%12;
			if(sys_time.wHour==0)
				sys_time.wHour=12;
		}
		hour1=sys_time.wHour/10;
		hour2=sys_time.wHour%10;

		min1=sys_time.wMinute/10;
		min2=sys_time.wMinute%10;

		swprintf_s(str,16,_T("%02d:%02d"),sys_time.wHour,sys_time.wMinute);
		m_tagTime.SetCaption(str);

		/*
		if(hour1!=m_tagHour1.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),hour1);
			m_tagHour1.SetCaption(str);
			m_tagHour1.UpdateID(hour1);
			VIEW_STATE vs=m_tagHour1.GetCurViewState();
			vs.fRotateX=-180;
			m_tagHour1.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagHour1.SetAimViewState(&vs);
		}
		if(hour2!=m_tagHour2.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),hour2);
			m_tagHour2.SetCaption(str);
			m_tagHour2.UpdateID(hour2);
			VIEW_STATE vs=m_tagHour2.GetCurViewState();
			vs.fRotateX=-180;
			m_tagHour2.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagHour2.SetAimViewState(&vs);
		}
		if(min1!=m_tagMin1.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),min1);
			m_tagMin1.SetCaption(str);
			m_tagMin1.UpdateID(min1);
			VIEW_STATE vs=m_tagMin1.GetCurViewState();
			vs.fRotateX=-180;
			m_tagMin1.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagMin1.SetAimViewState(&vs);
		}
		if(min2!=m_tagMin2.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),min2);
			m_tagMin2.SetCaption(str);
			m_tagMin2.UpdateID(min2);
			VIEW_STATE vs=m_tagMin2.GetCurViewState();
			vs.fRotateX=-180;
			m_tagMin2.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagMin2.SetAimViewState(&vs);
		}
		*/
	}
}


#endif