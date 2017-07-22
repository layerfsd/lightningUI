#include "PanelIdle.h"
#include "../../resource.h"

CPanelIdle::CPanelIdle(void)
{
	m_idType = IDLE_SCREEN_DIGITAL_CLOCK;
}

CPanelIdle::~CPanelIdle(void)
{
}
BOOL CPanelIdle::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelIdle::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	SetAlpha(0.1f);
	SetAlpha(1.0f,TRUE);

	RETAILMSG(1, (TEXT("MSG:[%s] \r\n"),TEXT(__FUNCTION__)));
	if(iParam)
	{
		m_idType = iParam;
	}
}
void CPanelIdle::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
// 	SetAlpha(1.0f);
	SetAlpha(0.0f,FALSE);

}
void CPanelIdle::Render()
{
	//static COLORGL crb={0.03125,0.03125,0.03125,1.0};

	static DWORD s_dwlastUpdateTimeTick=0;
	static SYSTEMTIME sys_time;
	static COLORGL cr_w={1,1,1,1};
	static COLORGL cr_gray={0.7,0.7,0.7,1};
	static COLORGL cr_b={0.38,0.74,0.89,1};

	if(!BeginRender())return;


	//DrawRect(0,0,800,480,&crb);

	switch (m_idType)
	{
	case IDLE_SCREEN_ANALOG_CLOCK://analog clock
		{
			m_barClock.SetAlpha(m_fCurAlpha);
			m_barClock.Render();
		}
		break;
	case IDLE_SCREEN_DIGITAL_CLOCK://digital clock
		{
			//update time:
			if(GetTickCount()-s_dwlastUpdateTimeTick>1000)
			{
				s_dwlastUpdateTimeTick = GetTickCount();
				GetLocalTime(&sys_time);
			}

			if(m_pTextGL)
			{
				SIZE si;
				wchar_t str[32];

				m_pTextGL->SetCharHeight(90);
				m_pTextGL->SetColor(&cr_w);
				m_pTextGL->SetAlpha(m_fCurAlpha);
				m_pTextGL->SetLuminance(m_fCurLuminance);

				//time
				if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H)
				{
					if(sys_time.wHour>=12)
						swprintf_s(str,32,_T("%d:%02d"),(sys_time.wHour-12)?(sys_time.wHour-12):12,sys_time.wMinute);
					else
						swprintf_s(str,32,_T("%d:%02d"),sys_time.wHour?sys_time.wHour:12,sys_time.wMinute);
				}
				else
				{
					swprintf_s(str,32,_T("%d:%02d"),sys_time.wHour,sys_time.wMinute);
				}

				//m_pTextGL->SetStyle(STYLE_ART);
				m_pTextGL->SetString(str);
				m_pTextGL->GetExtension(&si/*,STYLE_ART*/);
				m_pTextGL->SetPos(GLfloat(-si.cx/2),100);
				m_pTextGL->SetColor(&cr_b);
				m_pTextGL->Render();
				m_pTextGL->SetStyle(STYLE_NORMAL);//reset

				//
				m_pTextGL->SetCharScale(0.8f,0.8f);

				//am/pm
				if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode==TIME_MODE_12H)
				{
					if(sys_time.wHour>=12)
					{
						m_pTextGL->SetString(_T("PM"));
						m_pTextGL->SetPos(150,18);
					}
					else
					{
						m_pTextGL->SetString(_T("AM"));
						m_pTextGL->SetPos(150,50+18);
					}

					//m_pTextGL->SetPos(150,18);
					m_pTextGL->SetColor(&cr_b);
					m_pTextGL->Render();
				}

				//
				m_pTextGL->SetCharHeight(34);

				//
				m_pTextGL->SetColor(&cr_gray);

				//date:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				swprintf_s(str,32,_T("%02d/%02d/%02d"),sys_time.wMonth,sys_time.wDay,sys_time.wYear);
#else
				swprintf_s(str,32,_T("%d-%02d-%02d"),sys_time.wYear,sys_time.wMonth,sys_time.wDay);
#endif

				m_pTextGL->SetString(str);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(GLfloat(-si.cx/2),-120);
				m_pTextGL->SetColor(&cr_b);
				m_pTextGL->Render();

				//week:
				m_pTextGL->SetString(GetResourceString(IDS_SUNDAY+sys_time.wDayOfWeek));
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(GLfloat(-si.cx/2),-60);
				m_pTextGL->SetColor(&cr_b);
				m_pTextGL->Render();

			}
		}
		break;
	default:
		break;
	}

	EndRender();

}
BOOL CPanelIdle::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

// 	CLOCKBARSTRUCTURE stru;
// 	memset(&stru,sizeof(stru),0);
// 	stru.idTexBase=TEXID_091;
// 	stru.idTexHour=TEXID_092;
// 	stru.idTexMinute=TEXID_093;
// 	stru.idTexSecond=TEXID_094;
// 	stru.bDashboardOnly=TRUE;
//
// 	m_barClock.Initialize(0,this,&stru,pSkinManager,pTextGL);
// 	m_barClock.SetShowScale(1.40f);
// 	m_barClock.SetPos(0,0);

	CLOCKBARSTRUCTURE stru;
	memset(&stru,sizeof(stru),0);
	stru.idTexBase=TEXID_091;
	stru.idTexHour=TEXID_092;
	stru.idTexMinute=TEXID_093;
	stru.idTexSecond=TEXID_094;
	stru.bDashboardOnly=TRUE;
	stru.fDashboardScale=1.4f;
	stru.siBorder.cx=g_iClientWidth-240-20;
	stru.siBorder.cy=g_iClientHeight-20;
// 	stru.idTexBtnUp=TEXID_606;
// 	stru.idTexBtnDown=TEXID_607;
// 	stru.idTexIconAdd=TEXID_083;
// 	stru.idTexIconSub=TEXID_082;
// 	stru.crCaptionUp=cr_gray;
// 	stru.crCaptionDown=cr_gray;

	m_barClock.Initialize(0,this,&stru,pSkinManager,pTextGL);

	m_bShowStatusBar = FALSE;


	return TRUE;
}


void CPanelIdle::OnTouchDown(POINT *pt)
{
	//CPanelGL::OnTouchDown(pt);
}
void CPanelIdle::OnTouchUp(POINT *pt)
{
	//CPanelGL::OnTouchUp(pt);

}

void CPanelIdle::OnTouchMove(POINT *pt)
{
	//CPanelGL::OnTouchMove(pt);

}


void CALLBACK CPanelIdle::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelIdle *panel=(CPanelIdle*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelIdle::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{

}
void CPanelIdle::OnBack()
{
	//quit
// 	SwitchPage(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
// 		GET_PANEL_PAGE_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
// 		PAGE_SWITCH_ANI_NONE,0);//quit

	SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);

}
