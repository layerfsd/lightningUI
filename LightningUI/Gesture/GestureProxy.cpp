#include "../Config/customer.h"
#include "GestureProxy.h"
#include "CVTE_IoCtl.h"

#define PAC_MSG_QUEUE_NAME  					TEXT("PAC_MSG_QUEUE_NAME")

#if (CVTE_EN_GESTURE_AIR == 1)

const CGestureProxy* CGestureProxy::m_pInstance = new CGestureProxy();
CGestureProxy * g_pCGestureProxy = NULL;


CGestureProxy::CGestureProxy(void)
{
	m_hGestProxy = NULL;
	m_nGesContinueCircleCount = 0;
	m_dwGesDetectTick = 0;
	m_dwGesIDTick = 0;
	m_hGestProxy = GestureProxyOpen();

}
CGestureProxy::~CGestureProxy(void)
{
	if(m_hGestProxy)
	{
		GestureProxyClose(m_hGestProxy);
		m_hGestProxy = NULL;
	}
}

CGestureProxy* CGestureProxy::GetInstance()
{
    return (CGestureProxy*)m_pInstance;
}

void CGestureProxy::Initialize(CSkinManagerGL* pSkinManager, HWND hWnd)
{
	g_pCGestureProxy = this;
	m_hMainWnd = hWnd;
	m_nGesRotateThd = 8;
	m_bGesWaveFirstDetect = TRUE;
	m_bGestureIDDoubleCheck = FALSE;
	GestureIDEnable(TRUE);
	GestureCircleEnable(TRUE);
	m_dwGesInitTick = GetTickCount();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GestureIDThread, this, 0, NULL);
}

#if CVTE_EN_LED
void CGestureProxy::LinkLED(CLED *phLED)
{
	m_pLED = phLED;
}
#endif

DWORD WINAPI CGestureProxy::GestureIDThread(LPVOID lpvParam)
{
	HANDLE hMsgQueue;
	DWORD   dwBytesRead, dwFlags;
	CGestureProxy *pDevice = (CGestureProxy*)lpvParam;
	if(pDevice == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: pDevice is NULL!!!\r\n"), TEXT(__FUNCTION__)));
		return -1;
	}


	MSGQUEUEOPTIONS MsgQueueOpt;
	MsgQueueOpt.dwSize = sizeof(MsgQueueOpt);
	MsgQueueOpt.dwFlags = MSGQUEUE_NOPRECOMMIT | MSGQUEUE_ALLOW_BROKEN;
	MsgQueueOpt.dwMaxMessages = 0;
	MsgQueueOpt.cbMaxMessage = sizeof(PACGestureData_t);
	MsgQueueOpt.bReadAccess = TRUE;

	hMsgQueue = CreateMsgQueue(PAC_MSG_QUEUE_NAME, &MsgQueueOpt);
	if(hMsgQueue == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateMsgQueue failed!!!\r\n"), TEXT(__FUNCTION__)));
		return -1;
	}

    while(!pDevice->m_bGestureIDThreadEnd)
	{
		memset(&pDevice->m_sGesData, 0, sizeof(pDevice->m_sGesData));
		if(!ReadMsgQueue(hMsgQueue, &(pDevice->m_sGesData), sizeof(pDevice->m_sGesData), &dwBytesRead, INFINITE, &dwFlags))
		{
			RETAILMSG(1, (TEXT("ERR:[%s]: ReadMsgQueue failed!!!\r\n"), TEXT(__FUNCTION__)));
			continue;
		}
		RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

		if(pDevice->m_hMainWnd == NULL)
			continue;

		if(pDevice->m_sGesData.nWave)
		{
			if(pDevice->m_bGestureDetectIsRunning)
			{
				pDevice->GestureIDEnable(TRUE);
				//pDevice->PauseGestureDetect();
			}
			else
			{
				if(pDevice->m_bGesWaveFirstDetect || GetTickCount()-pDevice->m_dwGesInitTick<=3000)
				{
					pDevice->m_bGesWaveFirstDetect = FALSE;
					continue;
				}
				pDevice->PlayGestureDetect();
				////Play sound effect
				//::SendMessage(pDevice->m_hMainWnd,WM_MSG_GESTURE_ID,sizeof(PACGestureData_t),(LPARAM)(&pDevice->m_sGesData));
			}
		}

		if(pDevice->m_bGestureIDEnable && pDevice->m_bGestureDetectIsRunning)
		{
			pDevice->GestureIDProcess();
		}
		else
		{
			RETAILMSG(1, (TEXT("MSG:[%s]: m_bGestureIDEnable[%d] m_bGestureDetectIsRunning[%d]\r\n"), TEXT(__FUNCTION__),pDevice->m_bGestureIDEnable,pDevice->m_bGestureDetectIsRunning));
		}
	}

	if(hMsgQueue)
	{
		CloseMsgQueue(hMsgQueue)
;	}
}

void CGestureProxy::GestureIDProcess()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: [%X]\r\n"), TEXT(__FUNCTION__),m_sGesData.nGesture));
	switch(m_sGesData.nGesture)
	{
		case GES_RIGHT_FLAG:
		case GES_LEFT_FLAG:
		case GES_UP_FLAG:
		case GES_DOWN_FLAG:
		case GES_FORWARD_FLAG:
		case GES_BACKWARD_FLAG:
			{
				DWORD dwTick = GetTickCount();
				PACGestureData_t GesData;
				memcpy(&GesData, &m_sGesData, sizeof(m_sGesData));
				if(m_bGestureIDDoubleCheck)
				{
					if(m_sGesData.nGesture != m_nGestureIDPrev)
					{
						if(dwTick-m_dwGesIDTick>1500)
						{
							RETAILMSG(1, (TEXT("MSG:[%s]: dwTick-m_dwGesIDTick>1500\r\n"), TEXT(__FUNCTION__)));
							m_dwGesIDTick = dwTick;
							m_nGestureIDPrev = m_sGesData.nGesture;
							GestureCircleEnable(FALSE);
							SetTimer(m_hMainWnd,TIMER_GESTURE_CIRCLE_ENABLE, 1000, OnTimerProc);
							::SendMessage(m_hMainWnd,WM_MSG_GESTURE_ID,sizeof(PACGestureData_t),(LPARAM)(&GesData));
						}
						else
						{
							//m_dwGesIDTick = dwTick;
							RETAILMSG(1, (TEXT("MSG:[%s]: skip!!!\r\n"), TEXT(__FUNCTION__)));
						}

					}
					else
					{
						RETAILMSG(1, (TEXT("MSG:[%s]: m_sGesData.nGesture == m_nGestureIDPrev\r\n"), TEXT(__FUNCTION__)));
						m_dwGesIDTick = dwTick;
						m_nGestureIDPrev = m_sGesData.nGesture;
						GestureCircleEnable(FALSE);
						SetTimer(m_hMainWnd,TIMER_GESTURE_CIRCLE_ENABLE, 1000, OnTimerProc);
						::SendMessage(m_hMainWnd,WM_MSG_GESTURE_ID,sizeof(PACGestureData_t),(LPARAM)(&GesData));
					}
				}
				else
				{
					RETAILMSG(1, (TEXT("MSG:[%s]: !m_bGestureIDDoubleCheck\r\n"), TEXT(__FUNCTION__)));
					m_dwGesIDTick = dwTick;
					m_nGestureIDPrev = m_sGesData.nGesture;
					m_bGestureIDDoubleCheck = TRUE;
					GestureCircleEnable(FALSE);
					SetTimer(m_hMainWnd,TIMER_GESTURE_CIRCLE_ENABLE, 1000, OnTimerProc);
					::SendMessage(m_hMainWnd,WM_MSG_GESTURE_ID,sizeof(PACGestureData_t),(LPARAM)(&GesData));
				}
			}
			break;
		default:
			break;
	}
}

void CGestureProxy::GestureIDEnable(BOOL bEnable)
{
	m_bGestureIDEnable = bEnable;
}

void CGestureProxy::GestureCircleEnable(BOOL bEnable)
{
	m_bGestureCircleEnable = bEnable;
}

void CGestureProxy::StopGestureDetect()
{
    m_bGestureDetectThreadEnd = TRUE;
    m_bGestureDetectIsRunning = FALSE;
    if(m_hGestureDetectEvent)
    {
        SetEvent(m_hGestureDetectEvent);
        WaitForSingleObject(m_hGestureDetectThread,INFINITE);
        CloseHandle(m_hGestureDetectEvent);
        m_hGestureDetectEvent = NULL;
    }

    if(m_hGestureDetectThread)
    {
        CloseHandle(m_hGestureDetectThread);
        m_hGestureDetectThread = NULL;
    }
}

BOOL CGestureProxy::StartGestureDetect()
{
    m_dwGestureDetectTimeout = 8;
    m_bGestureDetectThreadEnd = FALSE;
    m_bGestureDetectIsRunning = FALSE;

    if(m_hGestureDetectEvent == NULL)
    {
        m_hGestureDetectEvent = CreateEvent(0, FALSE, FALSE, NULL);
        if(m_hGestureDetectEvent == NULL)
        {
            RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent [m_hGestureDetectEvent] failed!!!\r\n"), TEXT(__FUNCTION__)));
            return FALSE;
        }
    }

    if(m_hGestureDetectThread == NULL)
    {
    	m_hGestureDetectThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GestureDetectThread, this, 0, NULL);
        if(m_hGestureDetectThread == NULL)
        {
            RETAILMSG(1, (TEXT("ERR:[%s]: CreateThread [m_hGestureDetectThread] failed!!!\r\n"), TEXT(__FUNCTION__)));
            return FALSE;
        }
        CeSetThreadPriority(m_hGestureDetectThread, 230);

    }
}

void CGestureProxy::PauseGestureDetect()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
    m_dwGestureDetectTimeout = INFINITE;
    m_bGestureDetectIsRunning = FALSE;

    if(m_hGestureDetectEvent)
    {
        //RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
        SetEvent(m_hGestureDetectEvent);
    }

	//::PostMessage(m_hMainWnd,WM_CMD_PLAY_SOUND_EFFECT,0,NULL);
	//::SendMessage(m_hMainWnd,WM_CMD_RINGTONE,0,NULL);
	#if CVTE_EN_LED
	if(m_pLED)
	{
		m_pLED->SetLedFlash(FALSE);
	}
	#endif
}

void CGestureProxy::PlayGestureDetect()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
    m_dwGestureDetectTimeout = 8;
    m_bGestureDetectIsRunning = TRUE;
	m_dwGesDetectTick = GetTickCount();
	m_bGestureIDDoubleCheck = FALSE;
    if(m_hGestureDetectEvent)
    {
        //RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
        SetEvent(m_hGestureDetectEvent);
    }

	//::PostMessage(m_hMainWnd,WM_CMD_PLAY_SOUND_EFFECT,0,NULL);

	//::SendMessage(m_hMainWnd,WM_CMD_RINGTONE,1,NULL);
	#if CVTE_EN_LED
	if(m_pLED)
	{
		m_pLED->SetLedFlash(TRUE);
	}
	#endif

	SetTimer(m_hMainWnd,TIMER_GESTURE_DETECT_CHECK,2000,OnTimerProc);
}

void CGestureProxy::GestureDetect()
{
	//GestureProxyGetObjectInfo(m_hGestProxy,  &m_GesObject);
	GestureProxyGetOperationRotate(m_hGestProxy, &m_nGesRotate);
//		if(abs(m_GesObjectPrev.dwPositionX-m_GesObject.dwPositionX)>40 || abs(m_GesObjectPrev.dwPositionY-m_GesObject.dwPositionY)>40)
//		{
//			memcpy(&m_GesObjectPrev, &m_GesObject, sizeof(m_GesObject));
//
//			m_tagObject.SetPos(g_ObjectPx+m_GesObject.dwPositionX, g_ObjectPy-m_GesObject.dwPositionY/2);
//			//RETAILMSG(1, (TEXT("MSG:XY[%d,%d] Size[%d] AvgY[%d]\r\n"),m_GesObject.dwPositionX,m_GesObject.dwPositionY,m_GesObject.nSize,m_GesObject.nAvgY));
//		}
	if(m_nGesRotate == m_nGesRotatePrev || m_nGesRotate == 0)
	{
		return;
	}

	//RETAILMSG(1, (TEXT("MSG:GestureDetect: m_nGesRotate[%d] m_nGesRotatePrev[%d]\r\n"),m_nGesRotate,m_nGesRotatePrev));
	if(m_nGesRotate<m_nGesRotatePrev)
	{
		if((m_nGesRotate-m_nGesRotatePrev)<(-m_nGesRotateThd))
		{
			RETAILMSG(1, (TEXT("MSG:GestureDetect: Circle right\r\n")));
			if(m_bGesClockCircle)
			{
				m_nGesContinueCircleCount++;
			}
			else
			{
				m_nGesContinueCircleCount = 0;
			}

			if(m_nGesContinueCircleCount)
			{
				///
				//Notify
				if(m_hMainWnd && m_bGestureCircleEnable)
				{
					RETAILMSG(1, (TEXT("MSG:GestureDetect: Circle right Send Notify\r\n")));
					GestureIDEnable(FALSE);
					m_dwGesDetectTick = GetTickCount();
					SetTimer(m_hMainWnd,TIMER_GESTURE_ID_ENABLE, 1000, OnTimerProc);
					::SendMessage(m_hMainWnd,WM_MSG_GESTURE_CIRCLE,TRUE,0);

				}
				m_nGesContinueCircleCount = 0;
			}
			m_nGesRotatePrev = m_nGesRotate;
			m_bGesClockCircle = TRUE;
		}
	}
	else
	{
		if((m_nGesRotate-m_nGesRotatePrev)>m_nGesRotateThd)
		{
			RETAILMSG(1, (TEXT("MSG:GestureDetect: Circle left\r\n")));
			if(!m_bGesClockCircle)
			{
				m_nGesContinueCircleCount++;
			}
			else
			{
				m_nGesContinueCircleCount = 0;
			}

			if(m_nGesContinueCircleCount)
			{
				////
				//Notify
				if(m_hMainWnd && m_bGestureCircleEnable)
				{
					RETAILMSG(1, (TEXT("MSG:GestureDetect: Circle left Send Notify\r\n")));

					GestureIDEnable(FALSE);
					m_dwGesDetectTick = GetTickCount();
					SetTimer(m_hMainWnd,TIMER_GESTURE_ID_ENABLE, 1000, OnTimerProc);
					::SendMessage(m_hMainWnd,WM_MSG_GESTURE_CIRCLE,FALSE,0);

				}
				m_nGesContinueCircleCount = 0;
			}

			m_nGesRotatePrev = m_nGesRotate;
			m_bGesClockCircle = FALSE;
		}
	}

}

DWORD WINAPI CGestureProxy::GestureDetectThread(LPVOID lpvParam)
{
    DWORD dwRet;
    CGestureProxy *pDevice = (CGestureProxy*)lpvParam;

    RETAILMSG(1, (TEXT("MSG:[%s]: ++\r\n"), TEXT(__FUNCTION__)));

	do
	{
    	WaitForSingleObject(pDevice->m_hGestureDetectEvent, INFINITE);
	}while(!pDevice->m_bGestureDetectIsRunning);

    RETAILMSG(1, (TEXT("MSG:[%s]: GestureProc START\r\n"), TEXT(__FUNCTION__)));

    while(!pDevice->m_bGestureDetectThreadEnd)
    {
        dwRet = WaitForSingleObject(pDevice->m_hGestureDetectEvent, pDevice->m_dwGestureDetectTimeout);
        if(dwRet == WAIT_OBJECT_0)
        {
            if(pDevice->m_bGestureDetectThreadEnd)
            {
                RETAILMSG(1, (TEXT("MSG:[%s]: GestureProc END \r\n"), TEXT(__FUNCTION__)));
                break;
            }
            if(pDevice->m_bGestureDetectIsRunning)
            {
                RETAILMSG(1, (TEXT("MSG:[%s]: GestureProc PLAY \r\n"), TEXT(__FUNCTION__)));
				continue;
            }
            else
            {
                RETAILMSG(1, (TEXT("MSG:[%s]: GestureProc PAUSE \r\n"), TEXT(__FUNCTION__)));
                continue;
            }
        }

        if(pDevice->m_bGestureDetectIsRunning)
        {
            pDevice->GestureDetect();
        }
		else
		{
        	RETAILMSG(1, (TEXT("ERR:[%s]: m_bGestureDetectIsRunning = FALSE \r\n"), TEXT(__FUNCTION__)));
		}
    }

    RETAILMSG(1, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));

	return 0;
}

void CGestureProxy::ChangeRotateThd(UINT16 nRotate)
{
	m_nGesRotateThd = nRotate;
    RETAILMSG(1, (TEXT("MSG:[%s]: m_nGesRotateThd[%d]\r\n"), TEXT(__FUNCTION__),m_nGesRotateThd));
}

void CGestureProxy::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	switch(idEvent)
	{
		case TIMER_GESTURE_ID_ENABLE:
			{
				g_pCGestureProxy->GestureIDEnable(TRUE);
				KillTimer(g_pCGestureProxy->m_hMainWnd,TIMER_GESTURE_ID_ENABLE);
			}
			break;
		case TIMER_GESTURE_CIRCLE_ENABLE:
			{
				g_pCGestureProxy->GestureCircleEnable(TRUE);
				KillTimer(g_pCGestureProxy->m_hMainWnd,TIMER_GESTURE_CIRCLE_ENABLE);
			}
			break;
		case TIMER_GESTURE_DETECT_CHECK:
			{
				DWORD dwTick = GetTickCount();
				if(dwTick-g_pCGestureProxy->m_dwGesDetectTick >= 10000 && dwTick-g_pCGestureProxy->m_dwGesIDTick >= 10000)
				{
					//Gesture circle finish
					g_pCGestureProxy->PauseGestureDetect();
					KillTimer(g_pCGestureProxy->m_hMainWnd,TIMER_GESTURE_DETECT_CHECK);
				}
			}
			break;
		default:
			break;
	}
}

#endif
