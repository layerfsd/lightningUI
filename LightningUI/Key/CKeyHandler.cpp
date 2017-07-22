#include "CKeyHandler.h"
#include "mmsystem.h"

CKeyHandler::CKeyHandler(KeyHandlerParam& param, HWND hWnd, UINT Msg)
:m_param(param),m_hWnd(hWnd),m_Msg(Msg),m_Down(FALSE),
m_PressHoldTimer(0),m_LongPressTimer(0),m_DoublePressTimer(0),
m_SendDoubleEvent(FALSE)
{

}

CKeyHandler::~CKeyHandler()
{

}

void CKeyHandler::KeyDown()
{
	if ( m_Down )
	{
		return;
	}
	m_Down = TRUE;
	PostMsg(EV_KEY_DOWN);
	if ( (m_param.EventMask & EV_KEY_PRESS_HOLD) != 0 )
	{
		if ( m_param.LongPressIntervalTime != 0 )
		{
			if (m_LongPressTimer == 0)
			{
				m_LongPressTimer = timeSetEvent( m_param.LongPressIntervalTime, 1, LongPressTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
			}
		}
		else
		{
			if (m_PressHoldTimer == 0)
			{
				m_PressHoldTimer = timeSetEvent( m_param.PressHoldIntervalTime, 1, PressHoldTimerProc, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
			}
		}
	}
	if ( (m_param.EventMask & (EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS) ) != 0 )
	{
		if (m_LongPressTimer == 0)
		{
			m_LongPressTimer = timeSetEvent( m_param.LongPressIntervalTime, 1, LongPressTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
		}
	}
	if ( (m_param.EventMask & EV_KEY_DOUBLE_PRESS ) != 0 )
	{
		if ( m_DoublePressTimer == 0 )
		{
			m_SendDoubleEvent = FALSE;
			m_DoublePressTimer = timeSetEvent( m_param.DoublePressIntervalTime, 1, DoublePressTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
		}
		else
		{
			timeKillEvent(m_DoublePressTimer);
			m_DoublePressTimer = 0;
			m_SendDoubleEvent = TRUE;
			PostMsg(EV_KEY_DOUBLE_PRESS);
		}
	}
}

void CKeyHandler::KeyUp(BOOL bCancle)
{
	if ( !bCancle )
	{
		PostMsg(EV_KEY_UP);
	}
	if ( m_PressHoldTimer != 0 )
	{
		timeKillEvent(m_PressHoldTimer);
		m_PressHoldTimer = 0;
	}
	if ( m_LongPressTimer != 0 )
	{
		timeKillEvent(m_LongPressTimer);
		m_LongPressTimer = 0;
		if ( ((m_param.EventMask & EV_KEY_SHORT_PRESS ) != 0) && !m_SendDoubleEvent )
		{
			if ( !bCancle )
			{
				PostMsg(EV_KEY_SHORT_PRESS);
			}
		}
	}
	m_Down = FALSE;
}

void CKeyHandler::PressHoldTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CKeyHandler* handler = (CKeyHandler*)dwUser;
	if ( handler != NULL )
	{
		handler->PressHoldTimer();
	}
}

void CKeyHandler::LongPressTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CKeyHandler* handler = (CKeyHandler*)dwUser;
	if ( handler != NULL )
	{
		handler->LongPressTimer();
	}
}

void CKeyHandler::DoublePressTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CKeyHandler* handler = (CKeyHandler*)dwUser;
	if ( handler != NULL )
	{
		handler->DoublePressTimer();
	}
}

void CKeyHandler::PressHoldTimer()
{
	PostMsg(EV_KEY_PRESS_HOLD);
}

void CKeyHandler::LongPressTimer()
{
	if ( m_LongPressTimer != 0 )
	{
		PostMsg(EV_KEY_LONG_PRESS);
		timeKillEvent(m_LongPressTimer);
		m_LongPressTimer = 0;
	}
	if ( (m_param.EventMask & EV_KEY_PRESS_HOLD) != 0 )
	{
		PostMsg(EV_KEY_PRESS_HOLD);
		if (m_PressHoldTimer == 0)
		{
			m_PressHoldTimer = timeSetEvent( m_param.PressHoldIntervalTime, 1, PressHoldTimerProc, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
		}
	}
	else
	{
		if ( m_DoublePressTimer != 0 )
		{
			timeKillEvent(m_DoublePressTimer);
			m_DoublePressTimer = 0;
		}
	}
}

void CKeyHandler::DoublePressTimer()
{
	if ( m_DoublePressTimer != 0 )
	{
		timeKillEvent(m_DoublePressTimer);
		m_DoublePressTimer = 0;
		m_SendDoubleEvent = FALSE;
	}
}

void CKeyHandler::PostMsg( unsigned int event )
{
	if ( ( m_param.EventMask & event ) != 0 )
	{
		if ( ( m_hWnd != NULL ) && ( m_Msg != 0 ) )
		{
			::PostMessage( m_hWnd, m_Msg, m_param.v_Key, event );
		}
		if ( m_param.FuncCallback != NULL )
		{
			m_param.FuncCallback( m_param.v_Key, event, m_param.FuncContext );
		}
	}
}

const CKeysHandler* CKeysHandler::m_pInstance = new CKeysHandler();

CKeysHandler::CKeysHandler()
:m_hWnd(NULL),m_Msg(0)
{

}

CKeysHandler::~CKeysHandler()
{

}

CKeysHandler* CKeysHandler::GetInstance()
{
	return (CKeysHandler*)m_pInstance;
}

void CKeysHandler::Initialize( HWND hWnd, UINT Msg )
{
	m_hWnd = hWnd;
	m_Msg = Msg;
}

void CKeysHandler::AddKeys(KeyHandlerParam* param, size_t count)
{
	for( size_t i = 0; i < count; i++ )
	{
		AddorReplaceKey( param[i] );
	}
}

void CKeysHandler::AddorReplaceKey(KeyHandlerParam &param)
{
	RemoveKey( param.Key );
	m_KeyMap.insert( CKeyHandler_Pair( param.Key, CKeyHandler(param, m_hWnd, m_Msg) ) );
}

void CKeysHandler::RemoveKey(unsigned int key)
{
	CKeyHandler_Map::iterator Para;
	Para = m_KeyMap.find( key );
	if ( Para != m_KeyMap.end() )
	{
		m_KeyMap.erase(Para);
	}
}

void CKeysHandler::KeyDown(unsigned int key)
{
	CKeyHandler_Map::iterator Para;
	Para = m_KeyMap.find( key );
	if ( Para != m_KeyMap.end() )
	{
		Para->second.KeyDown();
	}
}

void CKeysHandler::KeyUp(unsigned int key, BOOL bCancle)
{
	CKeyHandler_Map::iterator Para;
	Para = m_KeyMap.find( key );
	if ( Para != m_KeyMap.end() )
	{
		Para->second.KeyUp(bCancle);
	}
}

