#include "SndPlayer.h"


CSndPlayer::CSndPlayer(HINSTANCE hIns, ICSRMedia* media, const wchar_t *filename, BOOL bLoop, DWORD dwWaveId, DWORD dwVol)
{
	m_pCSRMedia = media;
	m_pPlayer = NULL ;
	m_pEvent = NULL ;
	m_pAudio = NULL ;

	m_strFile = filename;
	m_bLoop = bLoop;
	m_dwWavID = dwWaveId;
	m_dwVol = dwVol;
	m_bPlaying = FALSE;

	//create video window:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WNDCLASS wc = {0};
	memset (&wc, 0, sizeof(wc));

	// Register  window class
	wc.style            =  0;//CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc      = notifyWndProc;
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = hIns;
	wc.hIcon            = NULL;
	wc.hCursor          = NULL;
	wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wc.lpszMenuName     = NULL;
	wc.lpszClassName    = _T("SNDPLAYER_WND_CLASS");

	RegisterClass(&wc) ;

	// Create Main Window.placeholder. Not visible by default
	m_hNotifyWnd  = ::CreateWindowEx(
		WS_EX_NOANIMATION,
		_T("SNDPLAYER_WND_CLASS"),
		_T("SNDPLAYER_WND"),
		WS_POPUP,// | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//WS_VISIBLE,//
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		NULL,
		NULL,
		hIns,
		this);

	::ShowWindow(m_hNotifyWnd,SW_HIDE);
}

CSndPlayer::~CSndPlayer(void)
{

	if(m_pAudio)
	{
		m_pAudio->Release();
		m_pAudio = NULL;
	}
	if(m_pEvent)
	{
		m_pEvent->Release();
		m_pEvent = NULL;
	}
	if(m_pPlayer)
	{
		m_pPlayer->Release();
		m_pPlayer = NULL;
	}
}
void CSndPlayer::start()
{
	m_bPlaying = TRUE;

	HANDLE hThread=CreateThread(NULL,NULL,playThreadPro,this,NULL,NULL);

	if (hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
		hThread = INVALID_HANDLE_VALUE;
	}
}
void CSndPlayer::stop()
{
	m_bPlaying = FALSE;
	if(m_pPlayer)
	{
		m_pPlayer->SetPosition(0);
		m_pPlayer->Pause();
	}
}
DWORD CSndPlayer::playThreadPro(LPVOID lpParam)
{
	CSndPlayer *panel=(CSndPlayer*)lpParam;
	RETAILMSG(1, (TEXT("MSG:[CSndPlayer::playThreadPro]: ++\r\n")));
	//Sleep(1500);

	if (panel->m_pPlayer)
	{
		if (panel->m_bPlaying)
		{
			panel->m_pAudio->SetVolume(panel->m_dwVol);
			panel->m_pPlayer->Play();
		}
	}
	else
	{

		if (!panel->m_pCSRMedia)
			return 0;

		//create player for ringtone:
		HRESULT hr;
		hr = panel->m_pCSRMedia->CreatePlayer(&panel->m_pPlayer);
		if (FAILED(hr))
			return 0;

		hr = panel->m_pPlayer->QueryInterface(IID_ICSREventSource, (void**)&panel->m_pEvent);
		if (FAILED(hr))
			return 0;

		hr = panel->m_pEvent->SetNotifyWindow(panel->m_hNotifyWnd, WM_MEDIAPLAYER_MSG, 0);
		if (FAILED(hr))
			return 0;

		hr = panel->m_pPlayer->QueryInterface(IID_ICSRWaveoutAudio, (void**)&panel->m_pAudio);
		if (FAILED(hr))
			return 0;

		panel->m_pAudio->SetDeviceID(panel->m_dwWavID);   //ARM
		panel->m_pAudio->SetVolume(panel->m_dwVol);

		hr = panel->m_pPlayer->OpenFile(panel->m_strFile.String());
		if (FAILED(hr))
			return 0;


		if (panel->m_bPlaying)
		{
			hr = panel->m_pPlayer->Play();
			if (FAILED(hr))
				return 0;
		}
	}
	RETAILMSG(1, (TEXT("MSG:[CSndPlayer::playThreadPro]: --\r\n")));

	return 0;
}
LRESULT CALLBACK CSndPlayer::notifyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static CSndPlayer *pSndPlayer=NULL;


    PAINTSTRUCT ps;
    HDC hdc;
    //	POINT pt;

    switch (message)
    {
        case WM_COMMAND:
            break;
        case WM_CREATE:
            pSndPlayer =(CSndPlayer *)(((CREATESTRUCT*)lParam)->lpCreateParams);
            break;
        case WM_SETCURSOR:
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            // TODO: Add any drawing code here...

            EndPaint(hWnd, &ps);
            break;

        case WM_MEDIAPLAYER_MSG:
            if (pSndPlayer)
            {
                pSndPlayer->OnPlayerMsg(wParam,lParam);
            }
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
HRESULT CSndPlayer::OnPlayerMsg(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr=S_OK;

	CM_EVENT_BASE* pEvent = (CM_EVENT_BASE*)wParam;

	if (pEvent == NULL)
		return 0;

	CM_EVENT_TYPE type=pEvent->nType;

	switch (type)
	{
	case CM_EVENT_STATE_CHANGED:
		{
			CM_STATE nNewState = (CM_STATE)pEvent->dwParam;
			RETAILMSG(1,(_T("MSG:[%s]: ----CM_EVENT_STATE_CHANGED:%d----\r\n"),TEXT(__FUNCTION__),nNewState));
			//m_bRingTonePlaying = (CM_STATE_PLAYING==nNewState);
		}
		break;

	case CM_EVENT_COMPLETE:
		{
			RETAILMSG(1,(_T("MSG:[%s]: ----CM_EVENT_COMPLETE----\r\n"),TEXT(__FUNCTION__)));

			if(m_bLoop)
			{
				if(m_pPlayer)
				{
					m_pPlayer->SetPosition(0);
				}
			}
			else
			{
				stop();
			}

		}
		break;

	case CM_EVENT_STRUCTURE_CHANGE:
		break;

	case CM_EVENT_DURATION:
		break;

	case CM_EVENT_TAG:
		{
		}
		break;

	case CM_EVENT_ERROR:	//	low word of dwParam: error code, high word of dwParam: error domain
		break;

	case CM_EVENT_WARNING:	//	low word of dwParam: error code, high word of dwParam: error domain
		break;

	case CM_EVENT_INFO:		//	low word of dwParam: error code, high word of dwParam: error domain
		break;
	default:
		break;

	}

	if (m_pCSRMedia)
	{
		m_pCSRMedia->FreeEvent(pEvent);
	}

	return hr;
}
