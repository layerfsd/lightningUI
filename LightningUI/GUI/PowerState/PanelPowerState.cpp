#include "PanelPowerState.h"


CPanelPowerState::CPanelPowerState(void)
{
	m_dwEnterTick = 0;

	m_bNotifyPrePowerMsg=FALSE;
	m_bNotifyPowerMsg=FALSE;

    m_hPowerManagerEndEvt = NULL;
    m_hPowerManagerThread = NULL;
    m_bEndPowerManager = TRUE;
}

CPanelPowerState::~CPanelPowerState(void)
{
}
BOOL CPanelPowerState::IsReady()
{
	return CPanelGL::IsReady() ;//&& m_iconLogo.IsReady();

}
void CPanelPowerState::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	RETAILMSG(1, (TEXT("MSG:%s: ++++++++++++++++++++++\r\n"), TEXT(__FUNCTION__)));
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//
	if(m_iParam == SYS_POWER_ON)
	{
		SetAlpha(0.0f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
		SetAlpha(0.0f,TRUE);
	}


	m_dwEnterTick = GetTickCount();
	m_bEnableQuit = FALSE;

	m_bNotifyPrePowerMsg=TRUE;
	m_bNotifyPowerMsg=TRUE;

    StartPowerManager();
}
void CPanelPowerState::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	RETAILMSG(1, (TEXT("MSG:%s: -----------------\r\n"), TEXT(__FUNCTION__)));
    //StopPowerManager();
// 	if(m_bNotifyPowerMsg)
// 	{
// 		m_bNotifyPowerMsg=FALSE;
// 		m_bEnableQuit = TRUE;
// 		::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_OPERATING_ABORT,NULL);
// 	}
}
void CPanelPowerState::Render()
{
	static COLORGL crb={0.0,0.0,0.0,1.0};
	static COLORGL crw={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;


	DrawRect(0,0,g_iClientWidth,g_iClientHeight,&crb);

#if 0
	//m_iconLogo.SetAlpha(m_fCurAlpha);
#if !CVTE_EN_IDLE_LOGO
	if(m_iParam == SYS_POWER_ON)
	{
	    m_iconLogo.Render();
		m_iconLogo.SetAlpha(m_fCurAlpha);
	}
    else
    {
		//RETAILMSG(1, (TEXT("MSG:%s: 5555555555555555555\r\n"), TEXT(__FUNCTION__)));
	    m_iconLogo2.Render();
		m_iconLogo2.SetAlpha(m_fCurAlpha);
    }
#endif
#endif

        //The power state switch operation do not do here.
        //Because this part is based on the Panel render.
        //Whh 20160522
#if 0
//		if(m_bNotifyPrePowerMsg && GetTickCount()-m_dwEnterTick>50)
//		{
//			RETAILMSG(1, (TEXT("MSG:%s: 1111111111111111111\r\n"), TEXT(__FUNCTION__)));
//			m_bNotifyPrePowerMsg=FALSE;
//			::PostMessage(m_hMainWnd,WM_SYSTEM_PRE_POWER_STATE,m_iParam/*SYS_POWER_OFF*/,NULL);
//		}
//		
//		if(IsReady())
//		{
//			if(m_bNotifyPowerMsg && GetTickCount()-m_dwEnterTick>1200)
//			{
//				RETAILMSG(1, (TEXT("MSG:%s: 22222222222222222222\r\n"), TEXT(__FUNCTION__)));
//				m_bNotifyPowerMsg=FALSE;
//				m_bEnableQuit = TRUE;
//				::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,m_iParam/*SYS_POWER_OFF*/,NULL);
//			}
//	        else
//	        {
//	            if(GetTickCount()-m_dwEnterTick>1200)
//	            {
//	    			RETAILMSG(1, (TEXT("ERR:%s: 3333333333333333333 m_bNotifyPowerMsg:%d\r\n"), TEXT(__FUNCTION__),m_bNotifyPowerMsg));
//	            }            
//	        }
//		}
//	    else
//	    {
//	        if(GetTickCount()-m_dwEnterTick>1200)
//	        {
//				RETAILMSG(1, (TEXT("ERR:%s: 44444444444444444444444\r\n"), TEXT(__FUNCTION__)));
//	        }
//	    }
//	
//	    RETAILMSG(1, (TEXT("MSG:%s:\r\n"), TEXT(__FUNCTION__)));
#endif

	EndRender();

}
BOOL CPanelPowerState::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	SetAniSpeed(100);

	m_iconLogo.Initialize(0,NULL,m_pSkinManager->GetConfig()->GetCurParam()->strLogoName.String());
	m_iconLogo2.Initialize(0,NULL,m_pSkinManager->GetConfig()->GetCurParam()->strLogoName2.String());
	//m_iconLogo.SetScale(1.2f,1.2f,1.0f,FALSE);
	//m_iconLogo2.SetScale(1.2f,1.2f,1.0f,FALSE);
	m_bShowStatusBar = FALSE;
	m_bEnableQuit = FALSE;


	return TRUE;
}


void CPanelPowerState::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelPowerState::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

	OnBack();

}
void CPanelPowerState::OnTouchMove(POINT *pt)
{

}


void CALLBACK CPanelPowerState::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelPowerState *panel=(CPanelPowerState*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelPowerState::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{

}
void CPanelPowerState::OnBack()
{
	//quit

}

BOOL CPanelPowerState::StartPowerManager()
{
    m_bEndPowerManager = FALSE;

	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

    if(m_hPowerManagerEndEvt == NULL)
    {
        m_hPowerManagerEndEvt = CreateEvent(0, FALSE, FALSE, NULL);
        if(m_hPowerManagerEndEvt == NULL)
        {
            RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent [m_hPowerManagerEndEvt] failed!!!\r\n"), TEXT(__FUNCTION__)));
            return FALSE;
        }
        //RETAILMSG(1, (TEXT("MSG:[%s]: CreateEvent [m_hPowerManagerEndEvt] \r\n"), TEXT(__FUNCTION__)));
    }

    if(m_hPowerManagerThread)
    {
        RETAILMSG(1, (TEXT("MSG:[%s]: CloseHandle [m_hPowerManagerThread] \r\n"), TEXT(__FUNCTION__)));
        CloseHandle(m_hPowerManagerThread);
        m_hPowerManagerThread = NULL;
    }
    
    if(m_hPowerManagerThread == NULL)
    {
    	m_hPowerManagerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PowerManagerThreadPro, this, 0, NULL);
        if(m_hPowerManagerThread == NULL)
        {
            RETAILMSG(1, (TEXT("ERR:[%s]: CreateThread [m_hPowerManagerThread] failed!!!\r\n"), TEXT(__FUNCTION__)));
            return FALSE;
        }
        CeSetThreadPriority(m_hPowerManagerThread, 230);
        //RETAILMSG(1, (TEXT("MSG:[%s]: CreateThread [m_hPowerManagerThread] \r\n"), TEXT(__FUNCTION__)));
		return TRUE;
    }
	return FALSE;
}

void CPanelPowerState::StopPowerManager()
{
    m_bEndPowerManager = TRUE;
    if(m_hPowerManagerEndEvt)
    {
        SetEvent(m_hPowerManagerEndEvt);
        WaitForSingleObject(m_hPowerManagerThread,INFINITE);
        CloseHandle(m_hPowerManagerEndEvt);
        m_hPowerManagerEndEvt = NULL;
    }

    if(m_hPowerManagerThread)
    {
        CloseHandle(m_hPowerManagerThread);
        m_hPowerManagerThread = NULL;
    }
    
    RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
    
}


DWORD CPanelPowerState::PowerManagerThreadPro(LPVOID lpParam)
{
	CPanelPowerState *pDevice = (CPanelPowerState*)lpParam;
	DWORD			dwRet;
    
	if(pDevice == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Param check failed.\r\n"), TEXT(__FUNCTION__)));
		return -1;
	}

	RETAILMSG(1, (TEXT("MSG:[%s]: ++\r\n"), TEXT(__FUNCTION__)));
    
   
    while(!pDevice->m_bEndPowerManager)
    {
        dwRet = WaitForSingleObject(pDevice->m_hPowerManagerEndEvt, 50);
        if(dwRet == WAIT_OBJECT_0)
        {
            pDevice->m_bEndPowerManager = TRUE;
            RETAILMSG(1, (TEXT("MSG:[%s]: END \r\n"), TEXT(__FUNCTION__)));
            break;
        }
        else    
        {
            if(pDevice->PowerManagerRun())
			{
				break;
			}
        }

    }

	RETAILMSG(1, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
	return 0;
}

BOOL CPanelPowerState::PowerManagerRun() 
{
	if(m_bNotifyPrePowerMsg && GetTickCount()-m_dwEnterTick>50)
	{
		RETAILMSG(1, (TEXT("MSG:%s: =>OnSystemPrePowerState\r\n"), TEXT(__FUNCTION__)));
		m_bNotifyPrePowerMsg=FALSE;
		::PostMessage(m_hMainWnd,WM_SYSTEM_PRE_POWER_STATE,m_iParam/*SYS_POWER_OFF*/,NULL);
	}

	if(m_bNotifyPowerMsg && GetTickCount()-m_dwEnterTick>1200)
	{
		RETAILMSG(1, (TEXT("MSG:%s: =>OnSystemPowerState\r\n"), TEXT(__FUNCTION__)));
		m_bNotifyPowerMsg=FALSE;
		m_bEnableQuit = TRUE;
		::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,m_iParam/*SYS_POWER_OFF*/,NULL);
		m_bEndPowerManager = TRUE;
		return TRUE;
	}
	return FALSE;
}
