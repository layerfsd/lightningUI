#include "PanelMediaWrapper.h"
#include "../../resource.h"
//#include "PanelAux.h"
#include "../../Backlight/Backlight.h"

#define FANT_VIDEO_WND_CLASS		_T("FANT_VIDEO_WINDOW")
#define FANT_VIDEO_WND_TITLE		_T("Player Video Window")


#define PLAYER_PLAYPOS_UPDATE_TIME			500

#define TIMER_UPDATE_PLAYINFO									124
#define TIMER_MEDIA_VOLUME_FADE								125
#define TIMER_MEDIA_LOADFILE_READY							126

// extern LPCTSTR 		g_strInstallPathUI;
extern HWND			g_hMainWnd;
extern BOOL				g_bBTModuleLoaded;

BOOL				g_bMediaModuleLoaded = FALSE;

static void DumpMetaData(CM_META_TYPE nMetaType, ICSRMetaData* pMetaData)
{
#if 0

	static LPCWSTR s_Types[4] = { L"File", L"Audio", L"Video", L"Tag" };

	LPCWSTR pwszType = s_Types[nMetaType];

	UINT nCount = pMetaData->GetFieldCount();
	wprintf(L">>>>>%s: %u fields\r\n", pwszType, nCount);

	for (UINT i = 0; i < nCount; i++)
	{
		CM_VALUE_TYPE nType = CM_VALUE_TYPE_UNKNOWN;
		WCHAR wszName[256];
		DWORD cchName = 256, cbValue = 0;
		//	1: query name and type
		HRESULT hr = pMetaData->GetFieldByIndex(i, wszName, &cchName, &nType, NULL, &cbValue);
		if (SUCCEEDED(hr))
		{
			//	2: alloc data and query value
			BYTE* pValue = (BYTE*)malloc(cbValue);
			hr = pMetaData->GetFieldByIndex(i, wszName, &cchName, &nType, pValue, &cbValue);

			//	process value data here
			switch (nType)
			{
			case CM_VALUE_TYPE_UINT32:
				wprintf(L"  %s = %d\r\n", wszName, *(UINT32*)pValue);
				break;
			case CM_VALUE_TYPE_UINT64:
				wprintf(L"  %s = %I64d\r\n", wszName, *(UINT64*)pValue);
				break;
			case CM_VALUE_TYPE_DOUBLE:
				wprintf(L"  %s = %g\r\n", wszName, *(double*)pValue);
				break;
			case CM_VALUE_TYPE_STRING:
				wprintf(L"  %s = %s\r\n", wszName, (LPCWSTR)pValue);
				break;
			case CM_VALUE_TYPE_BINARY:
				wprintf(L"  %s = %u bytes data\r\n", wszName, cbValue);
				break;
			}

			free(pValue);
		}
	}

#endif

}



CPanelMediaWrapper::CPanelMediaWrapper(void)
{
// 	m_pVideoCore=NULL;

// 	m_bBeginToPlay=FALSE;
// 	m_bContinueToPlay=FALSE;

	m_ppanelMain = NULL;
	m_ppanelLibrary = NULL;
	m_ppanelDevice = NULL;

	m_bAutoPlay = FALSE;
	m_bAutoResume = FALSE;

	m_dwLastFullScreenTick=0;

	m_bPlaying=FALSE;
	m_iVideoStartPos=0;
	m_bPlayingBeforeReverse=FALSE;
	m_bPlayingBeforeBreak=FALSE;
	m_dwLastSeekTick=0;

	m_hVideoWnd=NULL;
	m_pCSRMedia = NULL ;
	m_pPlayer = NULL ;
	m_pEvent = NULL ;
	m_pVideo = NULL ;
	m_pAudio = NULL ;
	m_pStreamInfo = NULL ;
	m_pInspector = NULL ;
	m_pMediaInfo = NULL ;

	m_bFullScreenMode=FALSE;
	m_nLastState = CM_STATE_NULL;

	m_bHasMediaStream=FALSE;
	m_bCurPlayingAudioOnly=FALSE;

	m_iVolumeCur = 0x0;
	m_iVolumeSet = 0xFFFF;
	m_bVolumeFadeReady=TRUE;

	m_bRepeat=FALSE;
	m_bShuffle=FALSE;
	m_dwPlayPos=0;
	m_bNeedSetIniPos=FALSE;
	m_dwIniPos=0;
	m_dwPlayTick=0;

	m_bBeginPlayIndex=FALSE;
	m_bBeginPlayFile=FALSE;
	m_dwBeginPlayIndex=0;
	m_dwBeginPlayPos=0;

	m_category = MEDIA_AUDIO;

	m_bPlayingBeforeSpeech = FALSE;
	m_bSpeechOn = FALSE;
	m_bPhoneCallActive = FALSE;

	m_bMediaModuleLoaded = FALSE;

	m_pSmartBarWindow = NULL;
	m_idPlayMode=0;
	m_bScanning=FALSE;
	m_bScanningUpdate=TRUE;

	m_bA2DPAvailable = FALSE;
	m_bLoadFileReady = FALSE;
	m_bPlayFileReady = FALSE;

	m_dwEnterTick = 0;

	m_dwDeviceRemoveTick = 0;
	m_bAutoSelectDevice = FALSE;

	m_DataCS = new CCriticalSection;

	m_hOpenFileThread = INVALID_HANDLE_VALUE;
	m_bOpeningFile = FALSE;
	m_bAbortOpeningFile = FALSE;
	m_bPlayCompleteDetection = FALSE;
}

CPanelMediaWrapper::~CPanelMediaWrapper(void)
{
	CleanUp();

	if(m_pCSRMedia)
	{
		m_pCSRMedia->Release();
		m_pCSRMedia=NULL;
	}

	if(m_hVideoWnd)
	{
		DestroyWindow(m_hVideoWnd);
	}

	if(m_DataCS)
	{
		delete m_DataCS;
	}

	if(m_hOpenFileThread != INVALID_HANDLE_VALUE)
		CloseHandle(m_hOpenFileThread);

	if(m_hOpenFileEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hOpenFileEvent);

}
void CPanelMediaWrapper::OnSRCChanged(UINT idSRC)
{
	RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnSRCChanged] %d\r\n"),idSRC));

	if(GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_MEDIA && m_bPlaying)
	{
		m_bPlayingBeforeSpeech=FALSE;
		Pause();
	}
}
void CPanelMediaWrapper::LinkPages(CPanelMediaMain *pMain,CPanelMediaLibrary *pLibrary,CPanelMediaDevice *pDevice)
{
	if(pMain)
	{
		m_ppanelMain = pMain;
		m_ppanelMain->SetParent(this);
	}
	if(pLibrary)
	{
		m_ppanelLibrary = pLibrary;
		m_ppanelLibrary->SetParent(this);
	}
	if(pDevice)
	{
		m_ppanelDevice = pDevice;
		m_ppanelDevice->SetParent(this);
	}
}
void CPanelMediaWrapper::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	static BOOL bplaying_before=m_bPlaying;
	m_bPhoneCallActive = wParam;

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
	{
		if(wParam)//active
		{
			bplaying_before = (m_bSpeechOn?m_bPlayingBeforeSpeech:m_bPlaying);
			if(bplaying_before)
			{
				Pause();
			}
		}
		else
		{
			if(bplaying_before && !m_bSpeechOn)
			{
				SetVolume(0,FALSE);
				SetVolume(MEDIA_MAX_VOLMUE,TRUE);
				Play();
			}
		}
	}
}
void CPanelMediaWrapper::OnMediaVolumeFade(WPARAM wParam, LPARAM lParam)
{
	if(AutoVolumeStepProc())
	{
		KillTimer(m_hVideoWnd,TIMER_MEDIA_VOLUME_FADE);

		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnMediaVolumeFade]: -- \r\n")));
	}
}
void CPanelMediaWrapper::OnVideoWndGesture(WPARAM wParam, LPARAM lParam)
{
	GESTUREINFO gi;
	memset(&gi, 0, sizeof(gi));
	gi.cbSize = sizeof(GESTUREINFO);
	if (GetGestureInfo((HGESTUREINFO)lParam, &gi))
	{
		// Handle gesture indicated by wParam or gi.dwID
		switch (gi.dwID)
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
			if(m_bFullScreenMode)
			{
				LONG velocity = GID_SCROLL_VELOCITY (gi.ullArguments );
				LONG angle =   GID_SCROLL_ANGLE (gi.ullArguments );
				LONG direction = GID_SCROLL_DIRECTION (gi.ullArguments );
				if(velocity>1000)
				{
					switch (direction )
					{
					case ARG_SCROLL_DOWN:
						break;
					case ARG_SCROLL_UP:
						break;
					case ARG_SCROLL_LEFT:
						//PlayPrevious();
						::PostMessage(m_hMainWnd,WM_PANEL_KEY,KEY_MEDIA_PREVIOUS,NULL);
						break;
					case ARG_SCROLL_RIGHT:
						//PlayNext();
						::PostMessage(m_hMainWnd,WM_PANEL_KEY,KEY_MEDIA_NEXT,NULL);
						break;
					}
				}
			}
			break;

		case GID_HOLD:
			break;
		case GID_DOUBLESELECT:
			break;
		case GID_SELECT:
			{
				static DWORD lastTick=0;
				RETAILMSG(DEBUG_MEDIA,(_T("APP:TEST:MediaPlayer: [CPanelMediaWrapper::OnVideoWndGesture]: video windows Clicked on WM_LBUTTONDOWN,x:%d,y:%d\r\n"),pt.x,pt.y));
				if(GetTickCount() - lastTick >500)
				{
					//SwitchFullscreen(2);
					::PostMessage(m_hMainWnd,WM_PANEL_KEY,KEY_MEDIA_FULLSCREEN,NULL);
					lastTick = GetTickCount();
				}
			}
			break;
		}
	}
}
void CPanelMediaWrapper::OnUpdatePlaybackInfo(WPARAM wParam, LPARAM lParam)
{
	UpdatePlayPostion();

	//
	m_bEnableQuit = !(m_bHasMediaStream && !m_bCurPlayingAudioOnly && GetTickCount()-m_dwPlayTick<3000);

}
void CPanelMediaWrapper::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
	if(m_ppanelMain)
		m_ppanelMain->OnAvailableSourceChanged(wParam,lParam);

	//
	if(wParam == MEDIA_TYPE_A2DP)
	{
		m_bA2DPAvailable=lParam;
	}
}
void CPanelMediaWrapper::SetBrakeStatus(BOOL bOn)
{
	CPanelGL::SetBrakeStatus(bOn);

	if(m_ppanelMain)
		m_ppanelMain->ShowAlert(!m_bOnBrake);

	if((!m_bOnBrake||m_bCurPlayingAudioOnly) && IsWindowVisible(m_hVideoWnd))
	{
		ShowVideoWindow(FALSE);
	}
}
void CPanelMediaWrapper::OnReverseModeChange(BOOL bReverse)
{
	CPanelGL::SetReverseStatus(bReverse);

}
void CPanelMediaWrapper::Render()
{
	if(!BeginRender())return;

	EndRender();
}
BOOL CPanelMediaWrapper:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);


	m_hOpenFileEvent = CreateEvent(NULL,FALSE,FALSE,NULL);



	return TRUE;
}
BOOL CPanelMediaWrapper::LoadMediaModule()
{
	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::LoadMediaModule] ++\r\n")));

	if(!m_bMediaModuleLoaded)
	{

		//media:
		if (FAILED( ::CSRCreateMedia(&m_pCSRMedia) ))
		{
			RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::LoadMediaModule]: CSRCreateMedia FAILED!!\r\n")));

			return FALSE;
		}
		else
		{
			RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::LoadMediaModule]: CSRCreateMedia SUCCEED!!\r\n")));
		}

		//create video window:
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		WNDCLASS wc = {0};
		memset (&wc, 0, sizeof(wc));

		// Register  window class
		wc.style            =  0;//CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc      = VideoWndProc;
		wc.cbClsExtra       = 0;
		wc.cbWndExtra       = 0;
		wc.hInstance        = m_moduleEntry.hInstance;
		wc.hIcon            = NULL;
		wc.hCursor          = NULL;
		wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
		wc.lpszMenuName     = NULL;
		wc.lpszClassName    = FANT_VIDEO_WND_CLASS;

		RegisterClass(&wc) ;

		// Create Main Window.placeholder. Not visible by default
		m_hVideoWnd  = ::CreateWindowEx(
			WS_EX_NOANIMATION,
			FANT_VIDEO_WND_CLASS,
			FANT_VIDEO_WND_TITLE,
			WS_CHILD,// | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//WS_VISIBLE,//
			VIDEO_WINDOW_PX,//CW_USEDEFAULT,
			VIDEO_WINDOW_PY,//CW_USEDEFAULT,
			VIDEO_WINDOW_CX,//CW_USEDEFAULT,
			VIDEO_WINDOW_CY,//CW_USEDEFAULT, 2
			g_hMainWnd,//NULL,
			NULL,
			m_moduleEntry.hInstance,
			this);

		::ShowWindow(m_hVideoWnd,SW_HIDE);

		m_bMediaModuleLoaded = TRUE;

		g_bMediaModuleLoaded = TRUE;

		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::LoadMediaModule]: SUCCEED --\r\n")));

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}
LRESULT CALLBACK CPanelMediaWrapper::VideoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CPanelMediaWrapper *pMediaPanel=NULL;
	static int iLastPosX=0;
	static int iLastPosY=0;
	static int iLastPtX=0;
	static int iLastPtY=0;
	static BOOL bDraging=FALSE;
	static BOOL bPlaying=FALSE;

	PAINTSTRUCT ps;
	HDC hdc;
	POINT pt;

	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		pMediaPanel =(CPanelMediaWrapper *)(((CREATESTRUCT*)lParam)->lpCreateParams);
		break;
	case WM_SETCURSOR:
		break;
	case 	WM_ERASEBKGND:
		{
// 			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("+++++++CPanelMediaWrapper::VideoWndProc WM_ERASEBKGND!!!!....\r\n")));

			RECT rc;
			static HBRUSH br=CreateSolidBrush(RGB(8,8,8));
			GetClientRect(hWnd,&rc);
			::FillRect(GetDC(hWnd), &rc, br);

// 			//
// 			if(pMediaPanel->m_pVideo)
// 			{
// 				HRESULT hr=pMediaPanel->m_pVideo->UpdateVideo();
// 				RETAILMSG(1/*DEBUG_MEDIA*/,(_T("*******ShowVideoWindow m_pVideo->UpdateVideo:%d\r\n"),hr));
// 			}
			return 0;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
// 		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("+++++++CPanelMediaWrapper::VideoWndProc WM_PAINT!!!!....\r\n")));

		// TODO: Add any drawing code here...

		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if(pMediaPanel)
		{
			if(wParam == TIMER_UPDATE_PLAYINFO)
			{
				//pMediaPanel->UpdatePlayPostion();
				::PostMessage(pMediaPanel->m_hMainWnd,WM_MEDIA_UPDATE_PLAYBACK_INFO,0,NULL);
			}
			else if(wParam == TIMER_MEDIA_VOLUME_FADE)
			{
				::PostMessage(pMediaPanel->m_hMainWnd,WM_MEDIA_VOLUME_FADE,0,NULL);
			}
			else if(wParam == TIMER_MEDIA_LOADFILE_READY)
			{
				KillTimer(hWnd,TIMER_MEDIA_LOADFILE_READY);
				::PostMessage(pMediaPanel->m_hMainWnd,WM_MEDIA_LOADFILE_READY,0,NULL);
			}
		}
		break;
	case WM_DESTROY:
		break;
	case WM_GESTURE:
		{
			if(pMediaPanel)
			{
				pMediaPanel->OnVideoWndGesture(wParam,lParam);
			}
		}
		break;
		/*
	case WM_LBUTTONDBLCLK:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		if(pMediaPanel)
		{
			static DWORD lastTick=0;
			RETAILMSG(DEBUG_MEDIA,(_T("video windows Clicked on WM_LBUTTONDOWN,x:%d,y:%d\r\n"),pt.x,pt.y));
			if(GetTickCount() - lastTick >500)
			{
				pMediaPanel->SwitchFullscreen(2);
				lastTick = GetTickCount();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
// 		if(pMediaPanel)
// 		{
// 			static DWORD lastTick=0;
// 			RETAILMSG(DEBUG_MEDIA,(_T("video windows Clicked on WM_LBUTTONDOWN,x:%d,y:%d\r\n"),pt.x,pt.y));
// 			if(GetTickCount() - lastTick >500)
// 			{
// 				pMediaPanel->SwitchFullscreen(2);
// 				lastTick = GetTickCount();
// 			}
// 		}
		break;
	case WM_LBUTTONUP:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);

		//RETAILMSG(DEBUG_MEDIA,(_T("Clicked on WM_LBUTTONUP,x:%d,y:%d\r\n"),pt.x,pt.y));
		if(pMediaPanel )
		{

		}

		bDraging=FALSE;

		break;
	case WM_MOUSEMOVE:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		//RETAILMSG(DEBUG_MEDIA,(_T("Clicked on WM_MOUSEMOVE,x:%d,y:%d\r\n"),pt.x,pt.y));
		if(pMediaPanel)
		{

		}
		break;
*/
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
void CPanelMediaWrapper::PlayFile(LPCTSTR lpFileName,DWORD dwPos)
{
	if(!lpFileName)
		return;


	if(m_hOpenFileThread != INVALID_HANDLE_VALUE)
	{
		//m_bAbortOpeningFile = TRUE;

		SetEvent(m_hOpenFileEvent);

		DWORD icount=0;
		DWORD dwExitCode;
		//do
		{
			Sleep(50);
			if (GetExitCodeThread(m_hOpenFileThread, &dwExitCode))
			{
				//break;
			if(dwExitCode == STILL_ACTIVE)
			{
				RETAILMSG(1,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::PlayFile]: Already playing..., wait...  icount=%d --\r\n"),icount));

// 				if(icount>2)
// 				{
// 					TerminateThread(m_hOpenFileThread,-1);
// 					Sleep(100);
// 					break;
// 				}
// 				else
// 				{
// 					Sleep(500);
// 				}
				return ;
			}
			icount++;
			}
		} //while (dwExitCode == STILL_ACTIVE);


		CloseHandle(m_hOpenFileThread);
		m_hOpenFileThread = INVALID_HANDLE_VALUE;
	}

// 	m_panelMain.SetScanningStatus(TRUE);

	m_bLoadFileReady = FALSE;
	m_bPlayFileReady = FALSE;

	m_strFileToPlay = lpFileName;
	m_dwPosToPlay = dwPos;

	m_bOpeningFile = TRUE;

	//begin thread to open file:
	ResetEvent(m_hOpenFileEvent);
	m_bAbortOpeningFile = FALSE;
	m_hOpenFileThread=CreateThread(NULL,NULL,OpenFileThreadPro,this,NULL,NULL);

}
DWORD CPanelMediaWrapper::OpenFileThreadPro(LPVOID lpParam)
{
	CPanelMediaWrapper *panel=(CPanelMediaWrapper*)lpParam;

	if(panel)
	{
		//Sleep(200);
		if(WaitForSingleObject(panel->m_hOpenFileEvent,500) == WAIT_OBJECT_0)
		{
			RETAILMSG(1,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OpenFileThreadPro] Wait OpenFileEvent successfully.\r\n")));
		}
		else
		{
			panel->PlayFilePro(panel->m_strFileToPlay.String(),panel->m_dwPosToPlay);
		}
	}

	panel->m_bOpeningFile = FALSE;

	return 0;
}


BOOL CPanelMediaWrapper::IsMediaFileExists(LPCTSTR   lpszFileName)
{
	WIN32_FIND_DATA   wfd;
	BOOL   bRet;
	HANDLE   hFind;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   hFind   !=   INVALID_HANDLE_VALUE;
	FindClose(hFind);
	return   bRet;
}

HRESULT CPanelMediaWrapper::PlayFilePro(LPCTSTR lpFileName,DWORD dwPos)
{
	HRESULT hr=S_FALSE;

	RETAILMSG(1,(_T("APP:MSG:MdeiaPlayer: [CPanelMediaWrapper::PlayFilePro]: ThreadID[%x] begin to play file :%s..........\r\n"),GetCurrentThreadId(),lpFileName));

	if(!m_bMediaModuleLoaded)
		return hr;


	if(!m_pCSRMedia || !lpFileName)
		goto ERR;

	//BOOL bNeedMute = m_pCommander? !m_pCommander->GetItemData(MCU_MUTE_MODE) : FALSE;

	m_bLoadFileReady = FALSE;
	m_bPlayFileReady = FALSE;

	m_bEnableSmartBar = FALSE;

	//if(bNeedMute)
// 	{
// 		//Mute(TRUE);
// 		::SendMessage(m_hMainWnd,WM_MUTE_AUDIO,(1<<16)+_TFT_MUTE_ALL,(1<<16)+(5000));
// 	}

	//Sleep(100);

	if(m_pSmartBarWindow && m_pSmartBarWindow->IsVisible() )
	{
		m_pSmartBarWindow->Show(FALSE);
	}

// 	ShowVideoWindow(FALSE);

	CleanUp();

	//clear play info:
	if(m_ppanelMain)
	{
		m_ppanelMain->SetProgressValue(0);
		m_ppanelMain->SetElapsedTime(0);
		m_ppanelMain->SetRemainTime(0);
		m_ppanelMain->SetTitle(_T(""));
		m_ppanelMain->SetArtist(_T(""));
		m_ppanelMain->SetAlbum(_T(""));
	}

	//
	hr = m_pCSRMedia->CreatePlayer(&m_pPlayer);
	if (FAILED(hr))
		goto ERR;

	hr = m_pPlayer->QueryInterface(IID_ICSREventSource, (void**)&m_pEvent);
	if (FAILED(hr))
		goto ERR;

	hr = m_pEvent->SetNotifyWindow(/*m_hVideoWnd*/m_hMainWnd, WM_PLAYER_MESSAGE, 0);
	if (FAILED(hr))
		goto ERR;

	hr = m_pPlayer->QueryInterface(IID_ICSRVideo, (void**)&m_pVideo);
	if (FAILED(hr))
		goto ERR;


	hr = m_pVideo->SetOverlayLayer(3);	//	Set video overlay layer here
	if (FAILED(hr))
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MdeiaPlayer: [CPanelMediaWrapper::PlayFilePro]: m_pVideo->SetOverlayLayer(3) FAILED!!!\r\n")));
		//goto ERR;
	}
	else
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MdeiaPlayer: [CPanelMediaWrapper::PlayFilePro]: m_pVideo->SetOverlayLayer(3) SUCCEED!!!\r\n")));
	}

	hr = m_pVideo->SetVideoWindow(m_hVideoWnd);
	if (FAILED(hr))
		goto ERR;

	//
	m_pVideo->KeepAspectRatio(TRUE);

	hr = m_pPlayer->QueryInterface(IID_ICSRWaveoutAudio, (void**)&m_pAudio);
	if (FAILED(hr))
		goto ERR;


	hr = m_pPlayer->QueryInterface(IID_ICSRStreamInfo, (void**)&m_pStreamInfo);
	if (FAILED(hr))
		goto ERR;

	//inspect metadata:
	hr = m_pCSRMedia->CreateInspector(&m_pInspector);
	if (FAILED(hr))
		goto ERR;

	hr = m_pInspector->QueryInterface(IID_ICSRMediaInfo, (void**)&m_pMediaInfo);
	if (FAILED(hr))
		goto ERR;

	if(!IsMediaFileExists(lpFileName))
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:ERROR:MdeiaPlayer: [CPanelMediaWrapper::PlayFilePro]: [%s] does not exist!!!\r\n"),lpFileName));
		goto ERR;
	}

	hr = m_pInspector->InspectFile(lpFileName,TRUE);
	if (FAILED(hr))
		goto ERR;

	hr = m_pInspector->WaitForCompletion(5000, NULL);
// 	for (int nMetaType = CM_META_DATA_FILE; nMetaType <= CM_META_DATA_TAG; nMetaType++)
// 	{
// 		ICSRMetaData* pMetaData = NULL;
// 		hr = m_pMediaInfo->GetMetaData((CM_META_TYPE)nMetaType, &pMetaData);
// 		if (SUCCEEDED(hr))
// 			DumpMetaData((CM_META_TYPE)nMetaType, pMetaData);
// 	}

	//File:
	ICSRMetaData* pFileMetaData = NULL;
	hr = m_pMediaInfo->GetMetaData(CM_META_DATA_FILE, &pFileMetaData);
	if (SUCCEEDED(hr))
		DumpMetaData(CM_META_DATA_FILE, pFileMetaData);

	//Audio:
	ICSRMetaData* pAudioMetaData = NULL;
	hr = m_pMediaInfo->GetMetaData(CM_META_DATA_AUDIO, &pAudioMetaData);
	if (SUCCEEDED(hr))
		DumpMetaData(CM_META_DATA_AUDIO, pAudioMetaData);

	//Video:
	ICSRMetaData* pVideoMetaData = NULL;
	hr = m_pMediaInfo->GetMetaData(CM_META_DATA_VIDEO, &pVideoMetaData);
	if (SUCCEEDED(hr))
		DumpMetaData(CM_META_DATA_VIDEO, pVideoMetaData);

	//Tag:
	ICSRMetaData* pTagMetaData = NULL;
	if(!pVideoMetaData)
	{
	hr = m_pMediaInfo->GetMetaData(CM_META_DATA_TAG, &pTagMetaData);
	if (SUCCEEDED(hr))
	{
		DumpMetaData(CM_META_DATA_TAG, pTagMetaData);
		CM_VALUE_TYPE nType;
		DWORD dwSize = 0;

		//retrieve title:
		LPCOLESTR pszName = L"title";
		hr = pTagMetaData->GetFieldByName(pszName, &nType, NULL, &dwSize);
		if (SUCCEEDED(hr))
		{
			BYTE* pValue = (BYTE*)malloc(dwSize);
			hr = pTagMetaData->GetFieldByName(pszName, &nType, pValue, &dwSize);
			if (SUCCEEDED(hr))
			{
				RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: Retrieve title SUCCEED,size:%d,%s\r\n"),dwSize,(LPCWSTR)pValue));
				if(m_ppanelMain)
					m_ppanelMain->SetTitle((LPCWSTR)pValue);
			}
			free(pValue);
		}
		else
		{
			WCHAR name[MAX_STRING];
			if(GetFileNameFromPath(lpFileName,name,MAX_STRING))
			{
				if(m_ppanelMain)
					m_ppanelMain->SetTitle(name);
			}
			else
			{
#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
				swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_TRACKS),GetResourceString(IDS_UNKNOWN));
#else
				swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
					GetResourceString(IDS_LIBRARY_MENU_TRACKS));
#endif
				if(m_ppanelMain)
					m_ppanelMain->SetTitle(name);
			}
		}
		//retrieve artist:
		pszName = L"artist";
		hr = pTagMetaData->GetFieldByName(pszName, &nType, NULL, &dwSize);
		if (SUCCEEDED(hr))
		{
			BYTE* pValue = (BYTE*)malloc(dwSize);
			hr = pTagMetaData->GetFieldByName(pszName, &nType, pValue, &dwSize);
			if (SUCCEEDED(hr))
			{
				RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: Retrieve artist SUCCEED,size:%d,%s\r\n"),dwSize,(LPCWSTR)pValue));

				if(m_ppanelMain)
					m_ppanelMain->SetArtist((LPCWSTR)pValue);
			}
			free(pValue);
		}
		else
		{
			WCHAR name[MAX_STRING];
#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_ARTISTS),GetResourceString(IDS_UNKNOWN));
#else
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
				GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
#endif
			if(m_ppanelMain)
				m_ppanelMain->SetArtist(name);
		}

		//retrieve album:
		pszName = L"album";
		HRESULT hr = pTagMetaData->GetFieldByName(pszName, &nType, NULL, &dwSize);
		if (SUCCEEDED(hr))
		{
			BYTE* pValue = (BYTE*)malloc(dwSize);
			hr = pTagMetaData->GetFieldByName(pszName, &nType, pValue, &dwSize);
			if (SUCCEEDED(hr))
			{
				RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: Retrieve album SUCCEED,size:%d,%s\r\n"),dwSize,(LPCWSTR)pValue));

				if(m_ppanelMain)
					m_ppanelMain->SetAlbum((LPCWSTR)pValue);
			}
			free(pValue);
		}
		else
		{
			WCHAR name[MAX_STRING];
#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_ALBUMS),GetResourceString(IDS_UNKNOWN));
#else
			swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
				GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
#endif
			if(m_ppanelMain)
				m_ppanelMain->SetAlbum(name);
		}
		//retrieve album art if have:
		pszName = L"image";
		hr = pTagMetaData->GetFieldByName(pszName, &nType, NULL, &dwSize);
		if (FAILED(hr))
		{
			pszName = L"preview-image";
			hr = pTagMetaData->GetFieldByName(pszName, &nType, NULL, &dwSize);
		}
		if (SUCCEEDED(hr))
		{
			BYTE* pbImage = new BYTE[dwSize];
			if (pbImage)
			{
				hr = pTagMetaData->GetFieldByName(pszName, &nType, pbImage, &dwSize);
				if (SUCCEEDED(hr))
				{
					RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: Retrieve album art SUCCEED,size:%d\r\n"),dwSize));
// 					Lock();
					if(m_ppanelMain)
						m_ppanelMain->OnUpdateAlbumArt(pbImage,dwSize);
// 					UnLock();
				}
				delete[] pbImage;
			}
		}
		else
		{
			RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: NO album art,set default---\r\n")));
// 			Lock();
			if(m_ppanelMain)
				m_ppanelMain->OnUpdateAlbumArt(NULL,0);
// 			UnLock();
		}
	}
	}

#if USE_WAV_ONCHIP
 	hr = m_pAudio->SetDeviceID(1);
 	if (FAILED(hr))
 	{
 		//RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper::m_pAudio->SetDeviceID(1) FAILED!!!---\r\n")));
 		//goto ERR;
 	}
 	Sleep(100);
#endif
	//begin to play:

	if(!IsMediaFileExists(lpFileName))
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:ERROR:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: %s does not exist---\r\n"),lpFileName));
		goto ERR;
	}

	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: begin to OpenFile---\r\n")));
	hr = m_pPlayer->OpenFile(lpFileName);
	if (FAILED(hr))
		goto ERR;

	m_strCurrentPlayFile = lpFileName;

	//has video:
	m_bCurPlayingAudioOnly = !pVideoMetaData;

	// 	m_panelMain.SetVideoMode(!m_bCurPlayingAudioOnly);

	m_bHasMediaStream=TRUE;

	//update now playing title:
	if(m_ppanelLibrary)
	{
		WCHAR str[MAX_STRING];
		swprintf_s(str,MAX_STRING,GetResourceString(IDS_NOWPLAYING_PLAYING),
			m_ppanelLibrary->GetCurIndex()+1,
			m_ppanelLibrary->GetTrackCount());

		if(m_ppanelMain)
			m_ppanelMain->SetNowPlayingTitle(str);
	}


	//mute:
	{
		//Mute(TRUE);
		::SendMessage(m_hMainWnd,WM_MUTE_AUDIO,(1<<16)+_TFT_MUTE_ALL,(1<<16)+((dwPos != 0)?2000:1000));
	}


	SetVolume(0,FALSE);

	//start to play:
	hr = m_pPlayer->Play();
	if (FAILED(hr))
		goto ERR;

	//m_bPlaying = TRUE;

	m_bNeedSetIniPos = (dwPos != 0);
	m_dwIniPos=dwPos;

	SetVolume(0,FALSE);

	//Sleep(300);

	//set fade volume:
	if(!m_bNeedSetIniPos)
	{
		SetVolume(m_bSpeechOn?MEDIA_LOW_VOLMUE:MEDIA_MAX_VOLMUE,TRUE);
	}

	m_dwPlayTick=GetTickCount();

	m_bEnableQuit = FALSE;

	m_bPlayCompleteDetection = TRUE;

// 	SetTimer(m_hVideoWnd,TIMER_MEDIA_LOADFILE_READY,2000,NULL);
	SetTimer(m_hVideoWnd,TIMER_UPDATE_PLAYINFO,PLAYER_PLAYPOS_UPDATE_TIME,NULL);

	return hr;

ERR:
	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:ERROR:MediaPlayer: [CPanelMediaWrapper::PlayFilePro]: PlayFile FAILED!!:%s---\r\n"),lpFileName));
	PlayFailed(lpFileName,PLAY_FAILED_INVALID_FILE);

	m_bEnableSmartBar = TRUE;

	return hr;
}
void CPanelMediaWrapper::StartPlayback()
{
	if(!m_bSpeechOn)
	{
		if(m_bHasMediaStream)
		{
			Play();
			//fade volume:
			SetVolume(0,FALSE);
			SetVolume(m_bSpeechOn?MEDIA_LOW_VOLMUE:MEDIA_MAX_VOLMUE,TRUE);
		}
		else
		{
			if(m_ppanelDevice)
				m_ppanelDevice->RefreshDeviceList();
// 			SwitchToPage(PAGE_MEDIA_DEVICE);
		}
	}
}
void CPanelMediaWrapper::AutoPlay()
{
	LPCTSTR pDevice=NULL;

	if(m_ppanelDevice)
	{
		m_ppanelDevice->RefreshDeviceList();

		if(-1 != m_ppanelDevice->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strMediaDevice.String()))
		{
			pDevice=m_pSkinManager->GetConfig()->GetCurParam()->strMediaDevice.String();
		}
		else
		{
			pDevice=m_ppanelDevice->GetFirstAvailableDevice();
		}

		if(pDevice)
		{
			RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::AutoPlay]: ThreadID[%x] Auto select device:%s\r\n"),GetCurrentThreadId(),pDevice));
			SelectSourceDevice(pDevice);
		}
		else if(m_bA2DPAvailable)
		{
			SelectSourceDevice(DEV_NAME_BT);
		}
		else
		{
			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::AutoPlay]: ThreadID[%x] Auto select device FAILED!!!\r\n"),GetCurrentThreadId()));

			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN);
		}
	}

}
void CPanelMediaWrapper::Play()
{
	if(!m_bSpeechOn && !m_bPhoneCallActive)
	{
		if(m_pPlayer)
		{
			//BOOL bNeedMute = m_pCommander? !m_pCommander->GetItemData(MCU_MUTE_MODE) : FALSE;

			//if(bNeedMute)
			{
				::SendMessage(m_hMainWnd,WM_MUTE_AUDIO,(1<<16)+_TFT_MUTE_ALL,(1<<16)+(300));
			}

			if(FAILED(m_pPlayer->Play()))
			{
				RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::Play]: FAILED!\r\n")));
			}
			else
			{
				RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::Play]: SUCCEED!\r\n")));
			}
		//	m_bPlaying = TRUE;
		}
	}
}
void CPanelMediaWrapper::Pause()
{
	if(m_pPlayer)
	{
		//BOOL bNeedMute = m_pCommander? !m_pCommander->GetItemData(MCU_MUTE_MODE) : FALSE;

		//if(bNeedMute)
		{
			::SendMessage(m_hMainWnd,WM_MUTE_AUDIO,(1<<16)+_TFT_MUTE_ALL,(1<<16)+(300));
		}
		m_pPlayer->Pause();
	//	m_bPlaying = FALSE;
	}
}
void CPanelMediaWrapper::Stop()
{
	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::Stop] \r\n")));

	CleanUp();

	if(m_ppanelMain)
	{
		m_ppanelMain->SetProgressValue(0);
		m_ppanelMain->SetElapsedTime(0);
		m_ppanelMain->SetRemainTime(0);

		m_ppanelMain->SetTitle(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
		m_ppanelMain->SetArtist(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
		m_ppanelMain->SetAlbum(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
		//m_ppanelMain->SetNowPlayingTitle(GetResourceString(IDS_UNKNOWN));
		m_ppanelMain->OnUpdateAlbumArt(NULL,0);
	}

	//widget:
	if(MEDIA_AUDIO == m_category)
	{
		DWORD dwSet=0;
		//m_pSkinManager->GetGUI()->UpdateWidget(UI_MEDIACORE,IDC_MEDIA_SLI_PROGRESS,UIOPRT_SETVALUE,&dwSet);
	}

	SwitchFullscreen(0);
}
const wchar_t* CPanelMediaWrapper::GetDeviceFriendlyName(LPCTSTR lpDeviceName)
{
	if (lstrcmpi(lpDeviceName, DEV_NAME_NAND) == 0)
	{
		return GetResourceString(IDS_DEVICE_RESIDENTFLASH);
	}
	else if (lstrcmpi(lpDeviceName, DEV_NAME_BT) == 0)//A2DP
	{
		return GetResourceString(IDS_DEVICE_BT);
	}
	else if (lstrcmpi(lpDeviceName, DEV_NAME_MEDIACARD) == 0)
	{
		return GetResourceString(IDS_DEVICE_SD);
	}
	else if (lstrcmpi(lpDeviceName, DEV_NAME_GPSCARD) == 0)
	{
		return GetResourceString(IDS_DEVICE_SD);
	}
	else if (lstrcmpi(lpDeviceName, DEV_NAME_USB1) == 0)
	{
		return GetResourceString(IDS_DEVICE_USB);
	}
	else if (lstrcmpi(lpDeviceName, DEV_NAME_USB2) == 0)
	{
		return GetResourceString(IDS_DEVICE_USB);
	}
	else if (lstrcmpi(lpDeviceName, DEV_NAME_IPOD) == 0)
	{
		return GetResourceString(IDS_DEVICE_IPOD);
	}
	else if (lstrcmpi(lpDeviceName, DEV_NAME_LINEIN_FRONT) == 0)
	{
		return GetResourceString(IDS_DEVICE_LINEIN_FRONT);
	}
	else
	{
		return NULL;
	}
}
void CPanelMediaWrapper::SelectSourceDevice(LPCTSTR lpDeviceName)
{

	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::SelectSourceDevice] %s,current:%s\r\n"),lpDeviceName,m_strCurrentDevice.String()));

	if(lpDeviceName)
	{
		if (lstrcmpi(lpDeviceName, DEV_NAME_BT) == 0)//A2DP
		{
// 			SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP);
		}
		else if (lstrcmpi(lpDeviceName, DEV_NAME_IPOD) == 0)//IPOD
		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_IPODCORE,NULL);
// 			SwitchPage(UI_CLASS_ipod,PAGE_BT_A2DP);
		}
		else if (lstrcmpi(lpDeviceName, DEV_NAME_LINEIN_FRONT) == 0)//
		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_AUXCORE,AUX_INPUT_FRONT);
		}
		else if (lstrcmpi(lpDeviceName, DEV_NAME_LINEIN_REAR) == 0)//
		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_AUXCORE,AUX_INPUT_REAR);
		}
		else if (lstrcmpi(lpDeviceName, DEV_NAME_DVD) == 0)//
		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_DISKCORE,NULL);
		}
		else//other storage device
		{
			if(m_strCurrentDevice.String() && lstrcmpi(m_strCurrentDevice.String(), lpDeviceName) == 0)
			{

			}
			else
			{
				if(m_bHasMediaStream)
				{
					Stop();
					Sleep(100);
				}
				m_strCurrentDevice=lpDeviceName;

				m_bBeginPlayFile = FALSE;
				m_bBeginPlayIndex = FALSE;

				if(m_ppanelLibrary)
					m_ppanelLibrary->SetDevice(lpDeviceName);

				if(m_ppanelMain)
					m_ppanelMain->SetDeviceName(lpDeviceName);//(GetDeviceFriendlyName(lpDeviceName));
			}


			//
			//SwitchToPage(PAGE_MEDIA_MAIN);
		}
	}
}
void CPanelMediaWrapper::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	if(!lpDeviceName)
		return;

	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnDeviceArrivedReady]: %s\r\n"),lpDeviceName));

	//need FIXED!  place this block into a thread:

// 	Sleep(3000);

// 	SwitchToPage(PAGE_MEDIA_DEVICE);

	//test:
	if(lstrcmpi(_T("DSK9:"), lpDeviceName) == 0)//Media Card
	{
		if(m_ppanelDevice)
			m_ppanelDevice->RefreshDeviceList();
		SelectSourceDevice(DEV_NAME_MEDIACARD);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
	}
#ifdef USE_GPS_CARD_MEDIA_PLAY
 	else if(lstrcmpi(_T("DSK8:"), lpDeviceName) == 0)//GPS card
 	{
		if(m_ppanelDevice)
			m_ppanelDevice->RefreshDeviceList();
		SelectSourceDevice(DEV_NAME_GPSCARD);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
 	}
#endif
	else if(lstrcmpi(_T("DSK2:"), lpDeviceName) == 0)//USB disk
	{
		if(m_ppanelDevice)
			m_ppanelDevice->RefreshDeviceList();
		SelectSourceDevice(DEV_NAME_USB1);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
	}
	else if(lstrcmpi(_T("DSK3:"), lpDeviceName) == 0)//USB disk2
	{
		if(m_ppanelDevice)
			m_ppanelDevice->RefreshDeviceList();
		SelectSourceDevice(DEV_NAME_USB2);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
	}
}
void CPanelMediaWrapper::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnDeviceRemovedReady]: %s\r\n"),lpDeviceName));

	if(!lpDeviceName)
		return;

	//notify:
	if(lstrcmpi(_T("DSK2:"), lpDeviceName) == 0)
		::PostMessage(m_hMainWnd,WM_AVAILABLE_SOURCE_CAHNGED,MEDIA_TYPE_USB1,0);
	else if(lstrcmpi(_T("DSK3:"), lpDeviceName) == 0)
		::PostMessage(m_hMainWnd,WM_AVAILABLE_SOURCE_CAHNGED,MEDIA_TYPE_USB2,0);
	else if(lstrcmpi(_T("DSK9:"), lpDeviceName) == 0)
		::PostMessage(m_hMainWnd,WM_AVAILABLE_SOURCE_CAHNGED,MEDIA_TYPE_SD1,0);
#ifdef USE_GPS_CARD_MEDIA_PLAY
	else if(lstrcmpi(_T("DSK8:"), lpDeviceName) == 0)
		::PostMessage(m_hMainWnd,WM_AVAILABLE_SOURCE_CAHNGED,MEDIA_TYPE_SD2,0);
#endif
	if( (m_strCurrentDevice.String() && lstrcmpi(_T("DSK2:"), lpDeviceName) == 0 && lstrcmpi(DEV_NAME_USB1, m_strCurrentDevice.String()) == 0) ||
		(m_strCurrentDevice.String() && lstrcmpi(_T("DSK3:"), lpDeviceName) == 0 && lstrcmpi(DEV_NAME_USB2, m_strCurrentDevice.String()) == 0) ||
#ifdef USE_GPS_CARD_MEDIA_PLAY
		(m_strCurrentDevice.String() && lstrcmpi(_T("DSK8:"), lpDeviceName) == 0 && lstrcmpi(DEV_NAME_GPSCARD, m_strCurrentDevice.String()) == 0 ) ||
#endif
		(m_strCurrentDevice.String() && lstrcmpi(_T("DSK9:"), lpDeviceName) == 0 && lstrcmpi(DEV_NAME_MEDIACARD, m_strCurrentDevice.String()) == 0 )
		)
		{

			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnDeviceRemovedReady]: reset current device\r\n")));

			m_strCurrentDevice = NULL;

			if(m_bHasMediaStream)
			{
				Stop();
			}

			if(m_ppanelLibrary)
			{
				if(m_ppanelLibrary->IsRefreshing())
				{
					m_ppanelLibrary->AbortRefresh();
				}
			}

			m_bBeginPlayFile = FALSE;
			m_bBeginPlayIndex = FALSE;

			m_dwDeviceRemoveTick = GetTickCount();
			m_bAutoSelectDevice = TRUE;
			/*
			if(UI_MEDIACORE == m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)
			{
				//auto change to other device:
				Sleep(1000);
				m_panelDevice.RefreshDeviceList();

				LPCTSTR pDevice=m_panelDevice.GetFirstAvailableDevice();
				if(pDevice)
				{
					SelectSourceDevice(pDevice);
				}
				else if(m_bA2DPAvailable)
				{
					SelectSourceDevice(DEV_NAME_BT);
				}
				else
				{
					m_panelLibrary.ClearList();

					RETAILMSG(1,(_T(">>>>>Auto select device FAILED!!! no available device\r\n")));
					::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_RADIOCORE,NULL);
				}
			}
			*/
		}
}
BOOL CPanelMediaWrapper::AutoSelectDevice()
{
	BOOL bRet=FALSE;
	//auto change to other device:
	if(m_ppanelDevice)
	{
		m_ppanelDevice->RefreshDeviceList();

		LPCTSTR pDevice=m_ppanelDevice->GetFirstAvailableDevice();
		if(pDevice)
		{
			SelectSourceDevice(pDevice);
			bRet=TRUE;
		}
		else if(m_bA2DPAvailable)
		{
			SelectSourceDevice(DEV_NAME_BT);
			bRet=TRUE;
		}
		else
		{
			bRet=FALSE;
			if(m_ppanelLibrary)
				m_ppanelLibrary->ClearList();

			RETAILMSG(1,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::AutoSelectDevice]: Auto select device FAILED!!! no available device\r\n")));
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN);
		}
	}

	return bRet;
}
void CPanelMediaWrapper::SetVolume(INT iVolume,BOOL bFade)
{
	if(m_pAudio)
	{
		if(iVolume<0)
			iVolume=0;
		if(iVolume>0xFFFF)
			iVolume=0xFFFF;
		m_iVolumeSet = iVolume;
		if(!bFade)
		{
			m_iVolumeCur=m_iVolumeSet;
			m_pAudio->SetVolume((iVolume<<16)+iVolume);
			m_bVolumeFadeReady=TRUE;
		}
		else
		{
			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::SetVolume]: set aim volume:%x\r\n"),m_iVolumeSet));
			if(m_bVolumeFadeReady)
			{
				m_bVolumeFadeReady=FALSE;
// 				HANDLE handle=CreateThread(NULL,NULL,AutoVolumeThreadPro,this,NULL,NULL);
// 				if(handle != INVALID_HANDLE_VALUE)
// 				{
// 					CloseHandle(handle);
// 				}
				SetTimer(m_hVideoWnd,TIMER_MEDIA_VOLUME_FADE,25,NULL);
			}
		}
	}

}

void CPanelMediaWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_MEDIA_BTN_PLAY:
		{
			StartPlayback();
		}
		break;
	}
}
void CPanelMediaWrapper::ShowVideoWindow(BOOL bShow)
{
// 	if( (bShow && IsWindowVisible(m_hVideoWnd)) || (!bShow && !IsWindowVisible(m_hVideoWnd)) )
// 		return;

	if(m_hVideoWnd /*&& m_pVideo*/)
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::ShowVideoWindow] %d\r\n"),bShow));

// 		SwitchFullscreen(m_bFullScreenMode);

		if(bShow)
		{
			if(!m_bCurPlayingAudioOnly)
			{
				::SetWindowPos(m_hVideoWnd, HWND_TOPMOST , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_SHOWWINDOW) );
			}
		}
		else
		{
			::SetWindowPos(m_hVideoWnd, HWND_BOTTOM , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_HIDEWINDOW) );
		}

// 		Sleep(100);

		if(m_pVideo)
		{
			m_pVideo->UpdateVideo();
		}
// 		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("*******ShowVideoWindow m_pVideo->UpdateVideo:%d\r\n"),hr));

// 		Sleep(100);
	}
}
void CPanelMediaWrapper::SwitchPlayMode()
{
	m_idPlayMode++;

	if(m_idPlayMode>2)
		m_idPlayMode=0;

	switch (m_idPlayMode)
	{
	case 0:
		m_bRepeat = FALSE;
		m_bShuffle=FALSE;
		break;
	case 1:
		m_bRepeat = TRUE;
		m_bShuffle=FALSE;
		break;
	case 2:
		m_bRepeat = FALSE;
		m_bShuffle=TRUE;
		break;
	}

	if(m_ppanelMain)
		m_ppanelMain->SetPlayMode(m_idPlayMode);
}
void CPanelMediaWrapper::SwitchRepeat(UINT idRepeat)
{
	switch (idRepeat)
	{
	case 0:
		m_bRepeat = FALSE;
		break;
	case 1:
		m_bRepeat = TRUE;
		break;
	case 2:
		m_bRepeat = !m_bRepeat;
		break;
	}

	if(m_bRepeat)
		m_bShuffle=FALSE;

	if(m_ppanelMain)
	{
		m_ppanelMain->SetRepeatCheck(m_bRepeat);
		m_ppanelMain->SetShuffleCheck(m_bShuffle);
	}

}
void CPanelMediaWrapper::SwitchShuffle(UINT idShuffle)
{
	switch (idShuffle)
	{
	case 0:
		m_bShuffle = FALSE;
		break;
	case 1:
		m_bShuffle = TRUE;
		break;
	case 2:
		m_bShuffle = !m_bShuffle;
		break;
	}

	if(m_bShuffle)
		m_bRepeat=FALSE;

	if(m_ppanelMain)
	{
		m_ppanelMain->SetRepeatCheck(m_bRepeat);
		m_ppanelMain->SetShuffleCheck(m_bShuffle);
	}
}
void CPanelMediaWrapper::SwitchFullscreen(UINT idMode)
{
	if(/*m_pVideo &&*/ !m_bCurPlayingAudioOnly)
	{

		int x=0,y=0,cx=800,cy=480;

		switch (idMode)
		{
		case 0:
			{
				x=VIDEO_WINDOW_PX;
				y=VIDEO_WINDOW_PY;
				cx=VIDEO_WINDOW_CX;
				cy=VIDEO_WINDOW_CY;
				m_bFullScreenMode=FALSE;
				break;
			}
		case 1:
			{
				x=0;
				y=0;
				cx=g_iScreenWidth;
				cy=g_iScreenHeight;
				m_bFullScreenMode=TRUE;
				break;
			}
		case 2:
			if(m_bFullScreenMode)
			{
				x=VIDEO_WINDOW_PX;
				y=VIDEO_WINDOW_PY;
				cx=VIDEO_WINDOW_CX;
				cy=VIDEO_WINDOW_CY;
				m_bFullScreenMode=FALSE;
			}
			else
			{
				x=0;
				y=0;
				cx=g_iScreenWidth;
				cy=g_iScreenHeight;
				m_bFullScreenMode=TRUE;
			}
			break;
		}

		::MoveWindow(m_hVideoWnd,x,y,cx,cy,TRUE);

		m_dwLastFullScreenTick = GetTickCount();

		if(m_pVideo)
		{
			m_pVideo->UpdateVideo();
		}
	}
}

void	CPanelMediaWrapper::CleanUp()
{
	if(m_pStreamInfo)
	{
		//RETAILMSG(1/*DEBUG_MEDIA*/,(_T("#####CPanelMediaWrapper::m_pStreamInfo->Release()\r\n")));

		m_pStreamInfo->Release();
		m_pStreamInfo=NULL;
	}

	if(m_bHasMediaStream)
	{
		KillTimer(m_hVideoWnd,TIMER_UPDATE_PLAYINFO);
	}

	if(m_pPlayer)
	{
		m_pPlayer->Pause();
		//Sleep(100);
	}

	m_bHasMediaStream = FALSE;
	m_bPlaying = FALSE;
	m_bNeedSetIniPos = FALSE;

	m_bEnableQuit = TRUE;

//	SwitchFullscreen(0);
// 	ShowVideoWindow(FALSE);

	if(m_pAudio)
	{
		m_pAudio->SetVolume(0);
	}




	if(m_pMediaInfo)
	{
		m_pMediaInfo->Release();
		m_pMediaInfo=NULL;
	}

	if(m_pInspector)
	{
		m_pInspector->Release();
		m_pInspector=NULL;
	}



	if(m_pAudio)
	{
		m_pAudio->Release();
		m_pAudio=NULL;
	}

	if(m_pEvent)
	{
		m_pEvent->Release();
		m_pEvent=NULL;
	}

	if(m_pVideo)
	{
		m_pVideo->Release();
		m_pVideo=NULL;
	}

	if(m_pPlayer)
	{
		//RETAILMSG(1/*DEBUG_MEDIA*/,(_T("#####CPanelMediaWrapper::m_pPlayer->Release()\r\n")));

		m_pPlayer->Release();
		m_pPlayer=NULL;
	}


	ShowVideoWindow(FALSE);

	//Sleep(100);
}
void CPanelMediaWrapper::OnSpeechStart()
{
	RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnSpeechStart]  ........... enter ...........\r\n")));

	m_bSpeechOn = TRUE;
#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
	{
		m_bPlayingBeforeSpeech = m_bPlaying;

// 		if(m_bPlayingBeforeSpeech)
// 		{
// 			Pause();
// 		}
		if(m_bPlayingBeforeSpeech)
		{
			SetVolume(MEDIA_LOW_VOLMUE,TRUE);
		}
	}
#endif
}
void CPanelMediaWrapper::OnSpeechTerminate()
{
// 	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("............CPanelMediaWrapper OnSpeechTerminate......................\r\n")));

	m_bSpeechOn = FALSE;
#if !USE_WAV_ONCHIP
	//switch back audio source:
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
	{
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_TFT_SOURCE_SW_KEY,SRC_AUDIO_ARM);
		}

		//

// 		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("............CPanelMediaWrapper m_bHasMediaStream :%d m_bPlayingBeforeSpeech :%d m_bPlaying :%d m_bPhoneCallActive :%d......................\r\n"),
// 			m_bHasMediaStream,m_bPlayingBeforeSpeech,m_bPlaying,m_bPhoneCallActive));

		if(m_bHasMediaStream && m_bPlayingBeforeSpeech && m_bPlaying && !m_bPhoneCallActive)
		{
			//SetVolume(0,FALSE);
// 			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("............CPanelMediaWrapper OnSpeechTerminate reset max volume......................\r\n")));

			SetVolume(MEDIA_MAX_VOLMUE,TRUE);
// 			Play();
		}

	}
#endif
}
void CPanelMediaWrapper::OnMediaScanFinish(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnMediaScanFinish] %s,bCancel:%d\r\n"),m_strCurrentDevice.String(),wParam));

	BOOL bCancel=wParam;

	if(m_ppanelMain)
		m_ppanelMain->SetScanningStatus(FALSE);

	m_bScanning=FALSE;

	m_bScanningUpdate = FALSE;


	if(!m_strCurrentDevice.String())
		return;

	if(m_bHasMediaStream)
		return;

	if(bCancel)
		return;

	int index=-1;
	DWORD dwPos=0;

	if(m_ppanelLibrary)
	{
		if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_MEDIACARD) == 0)
		{
			if(m_category == MEDIA_VIDEO)
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD1.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileSD1;

				//sd:
				::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_SD1,0x20);
			}
			else
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileSD1.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileSD1;

				//sd:
				::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_SD1,0x11);
			}
		}
		else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_GPSCARD) == 0)
		{
			if(m_category == MEDIA_VIDEO)
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD2.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileSD2;
			}
			else
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileSD2.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileSD2;
			}
		}
		else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_NAND) == 0)
		{
			if(m_category == MEDIA_VIDEO)
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileResident.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileResident;
			}
			else
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileResident.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileResident;
			}
		}
		else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_USB1) == 0)
		{
			if(m_category == MEDIA_VIDEO)
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB1.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileUSB1;

				//USB:
				::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_USB1,0x20);
			}
			else
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileUSB1.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileUSB1;

				//USB:
				::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_USB1,0x11);
			}
		}
		else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_USB2) == 0)
		{
			if(m_category == MEDIA_VIDEO)
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB2.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileUSB2;

				//USB:
				::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_USB1,0x20);
			}
			else
			{
				index=m_ppanelLibrary->SearchByValue(m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileUSB2.String());
				dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileUSB2;

				//USB:
				::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_USB1,0x11);
			}
		}
	}

	RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnMediaScanFinish]: Get %s media file:  index:%d,dwPos:%d\r\n"),m_strCurrentDevice.String(),index,dwPos));
	if(index != -1)
	{
		SwitchFullscreen(0);
		// 					PlayIndex(index,dwPos*1000);
		m_bBeginPlayIndex=TRUE;
		m_dwBeginPlayIndex=index;
		m_dwBeginPlayPos=dwPos*1000;
	}
	else
	{
		SwitchFullscreen(0);
		// 					PlayIndex(0);
		m_bBeginPlayIndex=TRUE;
		m_dwBeginPlayIndex=0;
		m_dwBeginPlayPos=0;
	}

	//
	if(m_ppanelMain)
	{
		m_ppanelMain->SetAlpha(0.9f);
		m_ppanelMain->SetAlpha(1.0f,TRUE);
	}
}
void CPanelMediaWrapper::OnMediaScanBegin(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnMediaScanBegin]  ........... enter ...........\r\n")));

	if(m_ppanelMain)
		m_ppanelMain->SetScanningStatus(TRUE);

	m_bScanning=TRUE;

}
HRESULT CPanelMediaWrapper::OnPlayerMsg(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr=S_OK;

	CM_EVENT_BASE* pEvent = (CM_EVENT_BASE*)wParam;

	if (pEvent == NULL)
		return 0;

	RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnPlayerMsg] Type:%d\r\n"),pEvent->nType));

	CM_EVENT_TYPE type=pEvent->nType;

	switch (type)
	{
	case CM_EVENT_STATE_CHANGED:
		{
			CM_STATE nNewState = (CM_STATE)pEvent->dwParam;

			RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnPlayerMsg]: CM_EVENT_STATE_CHANGED :%d\r\n"),nNewState));

			if(!m_bEnableSmartBar)
				m_bEnableSmartBar = (CM_STATE_PLAYING==nNewState);

			m_bPlaying = (CM_STATE_PLAYING==nNewState);

			if(m_ppanelMain)
				m_ppanelMain->SetPlaying(m_bPlaying);

			if(m_bPlaying && !m_bLoadFileReady)
			{
				m_bLoadFileReady = TRUE;
				SetTimer(m_hVideoWnd,TIMER_MEDIA_LOADFILE_READY,500,NULL);
// 				SetTimer(m_hVideoWnd,TIMER_UPDATE_PLAYINFO,PLAYER_PLAYPOS_UPDATE_TIME,NULL);
			}

// 			//update widget:
// 			BOOL bShow=(CM_STATE_PLAYING!=nNewState);
// 			m_pSkinManager->GetGUI()->UpdateWidget(UI_MEDIACORE,IDC_MEDIA_BTN_PLAY,UIOPRT_SHOWHIDE,&bShow);
// 			bShow=!bShow;
// 			m_pSkinManager->GetGUI()->UpdateWidget(UI_MEDIACORE,IDC_MEDIA_BTN_PAUSE,UIOPRT_SHOWHIDE,&bShow);

// 			if (nNewState >= CM_STATE_PAUSED)
// 				UpdatePosition();
//
			//	Only after state from READY to PAUSED, we can get file, audio and video info
// 			if (m_nLastState == CM_STATE_READY && nNewState == CM_STATE_PAUSED)
// 			{
// 				CComQIPtr<ICSRMediaInfo> spMediaInfo = m_pPlayer;
// 				if (spMediaInfo)
// 				{
// 					CMediaItemPtr pmi = g_MediaLibrary.Lookup(m_strFileName);
// 					if (pmi == NULL)
// 						pmi = g_MediaLibrary.CreateItem(m_strFileName);
// 					if (pmi && (pmi->GetFlags() & CMediaItem::FLAG_UPDATED) == 0)
// 						g_MediaLibrary.UpdateItem(pmi, spMediaInfo);
//
// 					::DumpMediaInfo(spMediaInfo);
// 				}
// 			}
			m_nLastState = nNewState;
		}
		break;

	case CM_EVENT_COMPLETE:
		{
			RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnPlayerMsg]: CM_EVENT_COMPLETE\r\n")));
// 			int nPosotion = m_spPlayer->GetPosition();
// 			CString strPosition = ::FormatTime((nPosotion + 500) / 1000);
// 			wprintf(L"Player complete: %s\r\n", (LPCTSTR)strPosition);
		}
		break;

	case CM_EVENT_STRUCTURE_CHANGE:
// 		InvalidateRect(m_hVideoWnd,NULL,TRUE);
		break;

	case CM_EVENT_DURATION:
// 		if (pEvent->dwParam != m_ToolBar.m_TrackBar.GetRangeMax())
// 			UpdatePosition();
// 		{
// 			CMediaItemPtr pmi = g_MediaLibrary.Lookup(m_strFileName);
// 			if (pmi != NULL)
// 				pmi->SetField(CMediaItem::FIELD_DURATION, ::FormatTime((pEvent->dwParam + 500) / 1000));
// 		}
		break;

	case CM_EVENT_TAG:
		{
			//	Only we get TAG event, we can get tag info
// 			CComQIPtr<ICSRMediaInfo> spMediaInfo = m_spPlayer;
// 			if (spMediaInfo)
// 			{
// 				CComPtr<ICSRMetaData> spMetaData;
// 				HRESULT hr = spMediaInfo->GetMetaData(CM_META_DATA_TAG, &spMetaData);
// 				if (SUCCEEDED(hr))
// 					DumpMetaData(CM_META_DATA_TAG, spMetaData);
// 			}
		}
		break;

	case CM_EVENT_ERROR:	//	low word of dwParam: error code, high word of dwParam: error domain
		//	wprintf(L"Error %d:%d: %s", HIWORD(pEvent->dwParam), LOWORD(pEvent->dwParam), pEvent->pszText);
		break;

	case CM_EVENT_WARNING:	//	low word of dwParam: error code, high word of dwParam: error domain
			RETAILMSG(DEBUG_MEDIA,(L"Warning %d:%d: %s\r\n", HIWORD(pEvent->dwParam), LOWORD(pEvent->dwParam), pEvent->pszText));
		break;

	case CM_EVENT_INFO:		//	low word of dwParam: error code, high word of dwParam: error domain
		//	wprintf(L"Info %d:%d: %s", HIWORD(pEvent->dwParam), LOWORD(pEvent->dwParam), pEvent->pszText);
		break;

	}

	if(m_pCSRMedia)
	{
		m_pCSRMedia->FreeEvent(pEvent);
	}

// 	//play end:
// 	if(type == CM_EVENT_COMPLETE)
// 	{
// 		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("--Play end 1\r\n")));
//
// 		Sleep(100);
// 		//SwitchFullscreen(0);
// 		ShowVideoWindow(FALSE);
// 		if(m_bRepeat)
// 		{
// 			PlayFile(m_strCurrentPlayFile.String());
// 		}
// 		else
// 		{
// 			PlayNext();
// 		}
// 	}

	return hr;
}



BOOL CPanelMediaWrapper::AutoVolumeStepProc()
{
	BOOL bRet=FALSE;

	//adjust volume:
	if(m_bHasMediaStream && m_pAudio && !m_bVolumeFadeReady)
	{
		if(abs(m_iVolumeCur-m_iVolumeSet)>100)
		{
			m_iVolumeCur += (m_iVolumeSet-m_iVolumeCur)/5;
			m_pAudio->SetVolume((m_iVolumeCur<<16)+m_iVolumeCur);
			bRet=FALSE;
		}
		else
		{
			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::AutoVolumeStepProc]: reach aim volume:%x\r\n"),(m_iVolumeSet<<16)+m_iVolumeSet));
			m_pAudio->SetVolume((m_iVolumeSet<<16)+m_iVolumeSet);
			m_bVolumeFadeReady = TRUE;
			bRet=TRUE;
		}
	}
	else
	{
		bRet=TRUE;
	}

	return bRet;
}

void CPanelMediaWrapper::PlayFailed(LPCTSTR lpFileName,UINT idErrCode)
{
	if(m_ppanelMain)
	{
		m_ppanelMain->SetProgressValue(0);
		m_ppanelMain->SetElapsedTime(0);
		m_ppanelMain->SetRemainTime(0);
		m_ppanelMain->OnUpdateAlbumArt(NULL,0);
	}

	//widget:
	if(MEDIA_AUDIO == m_category)
	{
		DWORD dwSet=0;
		//m_pSkinManager->GetGUI()->UpdateWidget(UI_MEDIACORE,IDC_MEDIA_SLI_PROGRESS,UIOPRT_SETVALUE,&dwSet);
	}

	CleanUp();

	if(idErrCode == PLAY_FAILED_INVALID_FILE)
	{
		//SwitchToPage(PAGE_MEDIA_LIBRARY);
		//PlayNext();
		if(m_ppanelMain)
			m_ppanelMain->SetNowPlayingTitle(GetResourceString(IDS_NOWPLAYING_INVALIDMEDIA));

		if(lpFileName)
		{
			WCHAR name[MAX_STRING];
			if(GetFileNameFromPath(lpFileName,name,MAX_STRING))
			{
				if(m_ppanelMain)
					m_ppanelMain->SetTitle(name);
			}
		}
	}
	else if(idErrCode == PLAY_FAILED_NO_FILE)
	{
		if(m_ppanelMain)
		{
			m_ppanelMain->SetNowPlayingTitle(GetResourceString(IDS_NO_MEDIA_FILES));

			m_ppanelMain->SetTitle(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
			m_ppanelMain->SetArtist(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
			m_ppanelMain->SetAlbum(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
		}
	}


}
void CPanelMediaWrapper::UpdatePlayPostion()
{
	//update position:
	static DWORD dwLastGetPosTick=0;
	static DWORD dwLastPos=0;
	static DWORD dwPosNotChangedTimes=0;

	BOOL bEnd=FALSE;

	if(/*GetTickCount()-dwLastGetPosTick>PLAYER_PLAYPOS_UPDATE_TIME &&*/ m_pPlayer && m_bPlaying)
	{
		RETAILMSG(DEBUG_MEDIA,(_T("---UpdatePlayPostion....\r\n")));

		DWORD dwPos,dwDuration;
		dwLastGetPosTick=GetTickCount();

		dwDuration=m_pPlayer->GetDuration();
		dwPos=m_pPlayer->GetPosition();

// 		RETAILMSG(DEBUG_MEDIA,(_T("dwPos:%d,dwDuration:%d\r\n"),dwPos,dwDuration));


		if((dwPos != CM_INVALID_TIME) && (dwDuration != CM_INVALID_TIME) )
		{
			m_dwPlayPos=dwPos/1000;

			if((dwPos<=dwDuration) && (dwDuration >1000))
			{
				DWORD dwSet=(dwPos/1000)*512/(dwDuration/1000);

				if(m_ppanelMain)
				{
					m_ppanelMain->SetProgressValue(dwSet);
					m_ppanelMain->SetElapsedTime(dwPos/1000);
					m_ppanelMain->SetRemainTime((dwDuration-dwPos)/1000);
				}

				//
				SaveCurrentMediaInfo(m_dwPlayPos);

				//notify can bus:
				if(m_ppanelLibrary)
				{
					int index=m_ppanelLibrary->GetCurIndex()+1;
					int total=m_ppanelLibrary->GetTrackCount();

					DWORD dwData1=((BYTE(total))<<24)+((BYTE(total>>8))<<16)+((BYTE(index))<<8)+((BYTE(index>>8))<<0);
					DWORD dwData2=((dwPos/60000)<<24)+(((dwPos/1000)%60)<<16)+(m_category==MEDIA_VIDEO?0x20:0x11);
					::PostMessage(m_hMainWnd,WM_NOTIFY_MEDIA_PLAYBACK_INFO,dwData1,dwData2);
				}
			}

			if((dwPos+200)>=dwDuration)
			{
				RETAILMSG(1/*DEBUG_MEDIA*/,(_T("--Player reach end ,dwPos:%d--\r\n"),dwPos));
				bEnd = TRUE;
			}
			if(dwLastPos == dwPos && dwLastPos!=0)//pos not changed,reach end
			{
				dwPosNotChangedTimes++;
				RETAILMSG(1/*DEBUG_MEDIA*/,(_T("--pos not changed ,dwPos:%d,times:%d--\r\n"),dwPos,dwPosNotChangedTimes));
				if(dwPosNotChangedTimes>=6)
				{
					bEnd = TRUE;
				}
			}
			else
			{
				dwPosNotChangedTimes = 0;
			}

			if(bEnd)
			{
				dwLastPos = 0;
				dwPosNotChangedTimes = 0;
				//send notify:
				if(	m_bPlayCompleteDetection )
				{
					RETAILMSG(1/*DEBUG_MEDIA*/,(_T("--Send Player reach end notify, dwPos:%d--\r\n"),dwPos));
					::PostMessage(m_hMainWnd,WM_PLAYER_COMPLETE,0,NULL);
					m_bPlayCompleteDetection = FALSE;
				}
				return;
			}

			dwLastPos = dwPos;
		}
	}
}
void CPanelMediaWrapper::OnPlayerLoadFileReady()
{
	RETAILMSG(1/*DEBUG_MEDIA*/,(_T(".......CPanelMediaWrapper::OnPlayerLoadFileReady\r\n")));

	//set ini pos
	if(m_bNeedSetIniPos && m_pPlayer)
	{
		//if(GetTickCount() - m_dwPlayTick>3000)
		{
			//RETAILMSG(1/*DEBUG_MEDIA*/,(_T(".......set ini pos:%d\r\n"),m_dwIniPos));
			m_bNeedSetIniPos = FALSE;
			HRESULT hr;
			hr = m_pPlayer->SetPosition(m_dwIniPos);
			RETAILMSG(1/*DEBUG_MEDIA*/,(_T(".......set ini pos:%d,hr:%x\r\n"),m_dwIniPos,hr));

			SetVolume(m_bSpeechOn?MEDIA_LOW_VOLMUE:MEDIA_MAX_VOLMUE,TRUE);
		}
	}

	m_bPlayFileReady = TRUE;

// 	m_panelMain.SetScanningStatus(FALSE);

	//auto show video window:
// 	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == UI_MEDIACORE)
// 	{
// 		if(m_bOnBrake && !m_bCurPlayingAudioOnly && m_bHasMediaStream && !IsWindowVisible(m_hVideoWnd))
// 		{
// 			ShowVideoWindow(TRUE);
// 		}
// 	}
}
void CPanelMediaWrapper::OnPlayerComplete()
{
// 	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI != UI_MEDIACORE && MEDIA_VIDEO == m_category)
// 	{
// 		Stop();
// 	}


// 	Stop();

	//SwitchFullscreen(0);
	//ShowVideoWindow(FALSE);
	if(m_bRepeat)
	{
// 		PlayFile(m_strCurrentPlayFile.String());

// 		if(MEDIA_VIDEO == m_category)
// 		{
// 			m_bBeginPlayFile = TRUE;
// 			m_strBeginPlayFile = m_strCurrentPlayFile.String();
//
// 			SwitchToPage(PAGE_MEDIA_MAIN);
// 		}
// 		else
		{
			PlayFile(m_strCurrentPlayFile.String());
		}
	}
	else
	{
		PlayNext();
	}
}
void CPanelMediaWrapper::SaveCurrentMediaInfo(DWORD dwPos)
{
	if(m_bHasMediaStream)
	{
		if(m_pSkinManager && m_strCurrentDevice.String())
		{
			if(dwPos ==0 && m_pPlayer)
			{
				dwPos = m_pPlayer->GetPosition()/1000;
			}

			m_pSkinManager->GetConfig()->GetCurParam()->strMediaDevice=m_strCurrentDevice.String();
			m_pSkinManager->GetConfig()->GetCurParam()->strMediaFile = m_strCurrentPlayFile;
			m_pSkinManager->GetConfig()->GetCurParam()->iMediaPos=dwPos;

			//for device:
			if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_MEDIACARD) == 0)
			{
				if(m_category == MEDIA_VIDEO)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD1 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileSD1=dwPos;
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileSD1 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileSD1=dwPos;
				}
			}
			else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_GPSCARD) == 0)
			{
				if(m_category == MEDIA_VIDEO)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD2 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileSD2=dwPos;
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileSD2 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileSD2=dwPos;
				}
			}
			else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_USB1) == 0)
			{
				if(m_category == MEDIA_VIDEO)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB1 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileUSB1=dwPos;
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileUSB1 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileUSB1=dwPos;
				}
			}
			else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_USB2) == 0)
			{
				if(m_category == MEDIA_VIDEO)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB2 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileUSB2=dwPos;
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileUSB2 = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileUSB2=dwPos;
				}
			}
			else if (lstrcmpi(m_strCurrentDevice.String(), DEV_NAME_NAND) == 0)
			{
				if(m_category == MEDIA_VIDEO)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileResident = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iVideoFileResident=dwPos;
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->strMusicFileResident = m_strCurrentPlayFile;
					m_pSkinManager->GetConfig()->GetCurParam()->iMusicFileResident=dwPos;
				}
			}
		}
	}
}
void CPanelMediaWrapper::OnPowerOFF()
{
	//save media info:
	SaveCurrentMediaInfo();

	ShowVideoWindow(FALSE);
	Stop();

	CleanUp();
}
void CPanelMediaWrapper::PlayIndex(INT index,DWORD dwPos)
{
	if(m_ppanelLibrary)
	{
		LPCTSTR pNext=m_ppanelLibrary->GetIndexValue(index);
		if(pNext)
		{
			PlayFile(pNext,dwPos);
		}
		else
		{
			PlayFailed();
		}
	}
}
void CPanelMediaWrapper::PlayPrevious()
{
	if(m_ppanelLibrary)
	{

	// 	LPCTSTR pPrev=m_panelLibrary.GetPreviousValue();
		LPCTSTR pPrev=m_bShuffle?m_ppanelLibrary->GetRandomValue():m_ppanelLibrary->GetPreviousValue();

		if(pPrev)
		{
			PlayFile(pPrev);
		}
		else
		{
			//PlayFailed();
		}
	}
}
void CPanelMediaWrapper::PlayNext()
{
	if(m_ppanelLibrary)
	{
		LPCTSTR pNext=m_bShuffle?m_ppanelLibrary->GetRandomValue():m_ppanelLibrary->GetNextValue();
		if(pNext)
		{
			PlayFile(pNext);
		}
		else
		{
			//PlayFailed();
		}
	}

}

BOOL CPanelMediaWrapper::OnKey(UINT idKey)
{
	RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnKey]: %x--\r\n"),idKey));



	return TRUE;
}

void CALLBACK CPanelMediaWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:MediaPlayer: [CPanelMediaWrapper::OnNotify]: id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelMediaWrapper *panel=(CPanelMediaWrapper*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelMediaWrapper::Lock()
{
	if(m_DataCS)
	{
		m_DataCS->Enter();
	}
}
void CPanelMediaWrapper::UnLock()
{
	if(m_DataCS)
	{
		m_DataCS->Leave();
	}
}
