#include "WarningProxy.h"
#include "../Common/message_map.h"
#include "mmsystem.h"

static BOOL bACCOnWarnShow = FALSE;
const CWarningProxy* CWarningProxy::m_pInstance = new CWarningProxy();

CWarningProxy::CWarningProxy(void)
: m_SmartBarWarnTimer(0), m_SmartBarWarnShowTick(0)
, m_bShowWarningSmartBar(FALSE), m_bShowWarningPage(FALSE)
, m_pSmartBarWarn(NULL), m_nShowPageCount(1), m_nShowBarTime(0)
{
#if CVTE_EN_CARPLAY
	m_bCarPlaySetAccOnWarning = FALSE;
#endif
}

CWarningProxy::~CWarningProxy(void)
{
	if (m_pSmartBarWarn)
		delete m_pSmartBarWarn;
}

CWarningProxy* CWarningProxy::GetInstance()
{
	return (CWarningProxy*)m_pInstance;
}


BOOL CWarningProxy::Initialize(CSmartBarWarn* pSmartBarWarn, HWND hWnd)
{
	m_hWnd = hWnd;
	m_pSmartBarWarn = pSmartBarWarn;

	return TRUE;
}

BOOL CWarningProxy::IsShowWarningSmartBar()
{
	return m_bShowWarningSmartBar;
}

BOOL CWarningProxy::IsShowWarningPage()
{
	return m_bShowWarningPage;
}

void CWarningProxy::StartShowWarningBar(UINT nShowTime)
{
	if (m_pSmartBarWarn && m_SmartBarWarnTimer == 0 && !bACCOnWarnShow)
	{
		bACCOnWarnShow = TRUE;
		m_nShowBarTime = nShowTime;
		RETAILMSG(1, (TEXT("MSG:[%s]: %d ms \r\n"), TEXT(__FUNCTION__),m_nShowBarTime));
		m_SmartBarWarnTimer = timeSetEvent( m_nShowBarTime, 1, WarningBarTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
		m_SmartBarWarnShowTick = GetTickCount();
		m_bShowWarningSmartBar = TRUE;
		m_pSmartBarWarn->Show(TRUE);

	}
}

void CWarningProxy::StopAndHideWarningBar()
{
	if (m_bShowWarningSmartBar)
	{
		DoWarningBarTimerEnd();
	}
}

void CWarningProxy::PauseShowWarningBar()
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

	if (m_pSmartBarWarn && m_SmartBarWarnTimer)
	{
		timeKillEvent(m_SmartBarWarnTimer);
		m_SmartBarWarnTimer = 0;

		if (m_pSmartBarWarn)
		{
			m_pSmartBarWarn->Show(FALSE);
		}

	}
}

void CWarningProxy::ResumeShowWarningBar()
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: m_bShowWarningSmartBar:%d\r\n"), TEXT(__FUNCTION__),m_bShowWarningSmartBar));
	if (m_pSmartBarWarn && m_bShowWarningSmartBar)
	{
		DWORD dwCurTick = GetTickCount();
		UINT  nTime = (dwCurTick-m_SmartBarWarnShowTick<m_nShowBarTime+10)?(m_nShowBarTime-(dwCurTick-m_SmartBarWarnShowTick)):0;
		
		RETAILMSG(1, (TEXT("MSG:[%s]: nTime:%d\r\n"), TEXT(__FUNCTION__),nTime));
	
		if (nTime)
		{
			m_SmartBarWarnTimer = timeSetEvent( (nTime), 1, WarningBarTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
			m_pSmartBarWarn->Show(TRUE);

		}
	}
}

void CWarningProxy::WarningBarTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CWarningProxy* handler = (CWarningProxy*)dwUser;
	if ( handler != NULL )
	{
		handler->DoWarningBarTimerEnd();
	}
}

void CWarningProxy::DoWarningBarTimerEnd()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

	if ( m_SmartBarWarnTimer != 0 )
	{
		timeKillEvent(m_SmartBarWarnTimer);
		m_SmartBarWarnTimer = 0;
	}

	if (m_pSmartBarWarn)
	{
		m_pSmartBarWarn->Show(FALSE);
	}

	m_bShowWarningSmartBar = FALSE;
	m_SmartBarWarnShowTick = 0;
	m_nShowBarTime = 0;

}

void  CWarningProxy::OnTouchDown(POINT *pt)
{
	if (m_pSmartBarWarn && m_pSmartBarWarn->IsVisible())
	{
		m_pSmartBarWarn->OnTouchDown(pt);
	}
}

void  CWarningProxy::OnTouchUp(POINT *pt)
{
	if (m_pSmartBarWarn && m_pSmartBarWarn->IsVisible())
	{
		m_pSmartBarWarn->OnTouchUp(pt);
	}
}

void  CWarningProxy::OnTouchMove(POINT *pt)
{
	if (m_pSmartBarWarn && m_pSmartBarWarn->IsVisible())
	{
		m_pSmartBarWarn->OnTouchMove(pt);
	}
}

void CWarningProxy::EnterWarningPage(UINT nShowTime)
{
	if(m_hWnd && m_nShowPageCount)
	{
		m_bShowWarningPage = TRUE;
		m_nShowPageCount--;
		::SendMessage(m_hWnd,WM_SHOW_WARNING_PAGE,nShowTime,NULL);
	}
}

void CWarningProxy::LeaveWarningPage()
{
	if(m_hWnd && m_bShowWarningPage)
	{
		::SendMessage(m_hWnd,WM_HIDE_WARNING_PAGE,NULL,NULL);
		m_bShowWarningPage = FALSE;
	}
}