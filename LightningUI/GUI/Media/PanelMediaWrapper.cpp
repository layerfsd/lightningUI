#include "PanelMediaWrapper.h"
#include "../../resource.h"
//#include "PanelAux.h"
#include "../../Backlight/Backlight.h"
#include "../Settings/SettingCommon.h"
#if CVTE_EN_CARPLAY
#include "CarplayAPI.h"
#endif
#include "../../CarPlay/CarplayAdapter.h"

#if CVTE_EN_ACCON_WARNING
#include "../Warning/WarningProxy.h"
#endif

#define FANT_VIDEO_WND_CLASS		_T("FANT_VIDEO_WINDOW")
#define FANT_VIDEO_WND_TITLE		_T("Player Video Window")
#if CVTE_EN_DYNAMIC_LOGO
#define DYNAMIC_LOGO_PLAY_FILE    _T("ResidentFlash\\LightningUI\\Car logo\\logo.mp4")
#endif

#define PLAYER_PLAYPOS_UPDATE_TIME			500

// extern LPCTSTR 		g_strInstallPathUI;
extern HWND			g_hMainWnd;

BOOL				g_bMediaModuleLoaded = FALSE;
extern BOOL				g_bBTModuleLoaded;
DWORD        g_dUIScrollBeginTime = 0;//UI is scrolling;

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
                    wprintf(L"  %s = <uint32>%d\r\n", wszName, *(UINT32*)pValue);
                    break;
                case CM_VALUE_TYPE_UINT64:
                    wprintf(L"  %s = <uint64>%I64d\r\n", wszName, *(UINT64*)pValue);
                    break;
                case CM_VALUE_TYPE_DOUBLE:
                    wprintf(L"  %s = <double>%g\r\n", wszName, *(double*)pValue);
                    break;
                case CM_VALUE_TYPE_STRING:
                    wprintf(L"  %s = <string>%s\r\n", wszName, (LPCWSTR)pValue);
                    break;
                case CM_VALUE_TYPE_BINARY:
                    wprintf(L"  %s = <binary>%u bytes data\r\n", wszName, cbValue);
                    break;
            }

            free(pValue);
        }
    }

#endif

}

static BOOL IsDecoderSupport(CM_META_TYPE nMetaType, ICSRMetaData* pMetaData)
{
	BOOL bSupport = FALSE;
	static LPCWSTR s_Types[4] = { L"File", L"Audio", L"Video", L"Tag" };

	LPCWSTR pwszType = s_Types[nMetaType];

	UINT nCount = pMetaData->GetFieldCount();
	RETAILMSG(1, (TEXT("MSG:[%s]: >>>>>%s: %u fields\r\n"), TEXT(__FUNCTION__), pwszType, nCount));

	for (UINT i = 0; i < nCount && !bSupport; i++)
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
				//wprintf(L"MSG:[%s]: >>>>>%s = %d\r\n", TEXT(__FUNCTION__), wszName, *(UINT32*)pValue);
				break;
			case CM_VALUE_TYPE_UINT64:
				//wprintf(L"MSG:[%s]: >>>>>%s = %I64d\r\n", TEXT(__FUNCTION__), wszName, *(UINT64*)pValue);
				break;
			case CM_VALUE_TYPE_DOUBLE:
				//wprintf(L"MSG:[%s]: >>>>>%s = %g\r\n", TEXT(__FUNCTION__), wszName, *(double*)pValue);
				break;
			case CM_VALUE_TYPE_STRING:
				wprintf(L"MSG:[%s]: >>>>>%s = %s\r\n", TEXT(__FUNCTION__), wszName, (LPCWSTR)pValue);
				
				if(wcsstr((LPCWSTR)pValue,_T("divx")))
				{
					bSupport = CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX;
					break;
				}
				else if(wcsstr((LPCWSTR)pValue,_T("xvid")))
				{
					bSupport = CVTE_EN_VIDEO_DECODE_SUPPORT_XVID;
					break;
				}
				else if(wcsstr((LPCWSTR)pValue,_T("h264")))
				{
					bSupport = CVTE_EN_VIDEO_DECODE_SUPPORT_H264;
					break;
				}
				else
				{
					bSupport = TRUE;
				}
				break;
			case CM_VALUE_TYPE_BINARY:
				wprintf(L"MSG:[%s]: >>>>>%s = <binary>%u bytes data\r\n", TEXT(__FUNCTION__), wszName, cbValue);
				break;
			}

			free(pValue);
		}
	}

	return bSupport;
}

static void GetVideoSize(ICSRMetaData* pMetaData,int *cx,int *cy)
{
#if 1

    if (!pMetaData || !cx || !cy)
        return;

    UINT nCount = pMetaData->GetFieldCount();

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

            if (lstrcmpi(wszName, _T("width")) == 0)
            {
                *cx=*(UINT32*)pValue;
            }
            else if (lstrcmpi(wszName, _T("height")) == 0)
            {
                *cy=*(UINT32*)pValue;
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

    m_pRingTonePlayer = NULL;
    m_pRingToneEvent = NULL;
    m_pRingToneAudio = NULL;

    m_bRingTonePlaying=FALSE;

    m_bFullScreenMode=FALSE;
    m_nLastState = CM_STATE_NULL;

    m_bHasMediaStream=FALSE;
    m_bCurPlayingAudioOnly=FALSE;

    m_iVolumeCur = 0x0;
    m_iVolumeSet = -1;
    m_bVolumeFadeReady=TRUE;

    m_bRepeat=FALSE;
    m_bShuffle=FALSE;
    m_dwPlayPos=0;
    m_bNeedSetIniPos=FALSE;
    m_dwIniPos=0;
    m_dwPlayTick=0;

    // 	m_bBeginPlayIndex=FALSE;
    // 	m_bBeginPlayFile=FALSE;
    // 	m_dwBeginPlayIndex=0;
    // 	m_dwBeginPlayPos=0;

    m_category = MEDIA_AUDIO;
    m_categoryToSet = MEDIA_AUDIO;

    m_bSpeechOn = FALSE;
    m_bPhoneCallActive = FALSE;

    m_bMediaModuleLoaded = FALSE;

    m_pSmartBarVol = NULL;

    m_idPlayMode=0;
    m_bBrowsing=FALSE;

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
    m_hOpenFileReadyEvent = INVALID_HANDLE_VALUE;
    m_dwOpenFileDelay = 1000;

    m_hLoadMediaModuleEvent = INVALID_HANDLE_VALUE;

    m_idCurMediaType = MEDIA_TYPE_INVALID;
    m_idSelectMediaType = MEDIA_TYPE_INVALID;

    m_dwLoadFileErrorCode = 0;

    m_bMonitorPlayinfo = FALSE;
    m_bNeedProcessCompleteNotify = FALSE;

    m_bManualPause = FALSE;
    m_bManualSetPosition = FALSE;
    m_dwManualSetPosition = 0;
    m_bManualPlayFile = FALSE;
    m_bManualChangeSrcType = FALSE;
    m_bManualChangeCategory = FALSE;
    m_bManualStop = FALSE;
    m_bManualMute = FALSE;

    m_iAudioStreamCount = 0;
    m_iVideoStreamCount = 0;

    m_iAutoSelectCategory = 0;
    m_iAutoDetectDeviceRetryCnt = 3;

    m_bVideoWndVisible = FALSE;

    m_bEnableVideoPlayer = TRUE;

    m_bCarPlayConnected = FALSE;
    m_bAutoPlayAfterBrowsingFinish = TRUE;
    m_pSmartBarMedia = NULL;

    m_iFactoryTestType = FACTORY_CMD_MEDIA_TEST_INVALID;
    m_iFactoryTestCmd = FACTORY_CMD_INVALID;
	m_bPreparePlay = FALSE;
	m_bRingToneLoop = FALSE;
	m_dwRingToneVol = 0xFFFFFFFF;
	m_dwRingToneWavID = 0;

	m_bEnableAutoPlay = FALSE;
	m_bAbortLoadFile = FALSE;

	m_bResumeVideoPlayAfterCarplay = FALSE;

	m_hGstReadyEvt = INVALID_HANDLE_VALUE;
	m_bMediaPlayerTakeGst = FALSE;

	//m_bUsb1Inserted = TRUE;
	m_bSROn = FALSE;      //add param init

	#if CVTE_EN_DYNAMIC_LOGO
	m_dwDynamicLogoVol = 0xFFFFFFFF;
	m_bDynamicLogoPlaying = FALSE;
	m_pDynamicLogoPlayer = NULL;
    m_pDynamicLogoSourceEvent = NULL;
    m_pDynamicLogoVideo = NULL;
    m_pDynamicLogoAudio = NULL;
	#endif

}

CPanelMediaWrapper::~CPanelMediaWrapper(void)
{

    if (m_pCSRMedia)
    {
        m_pCSRMedia->Release();
        m_pCSRMedia=NULL;
    }

    if (m_hVideoWnd)
    {
        DestroyWindow(m_hVideoWnd);
    }

    if (m_DataCS)
    {
        delete m_DataCS;
    }

    if (m_hOpenFileThread != INVALID_HANDLE_VALUE)
        CloseHandle(m_hOpenFileThread);

    if (m_hOpenFileEvent != INVALID_HANDLE_VALUE)
        CloseHandle(m_hOpenFileEvent);
    if (m_hOpenFileReadyEvent != INVALID_HANDLE_VALUE)
        CloseHandle(m_hOpenFileReadyEvent);

    if (m_hLoadMediaModuleEvent != INVALID_HANDLE_VALUE)
        CloseHandle(m_hLoadMediaModuleEvent);

	if(m_hGstReadyEvt != INVALID_HANDLE_VALUE)
		CloseHandle(m_hGstReadyEvt);

}
void CPanelMediaWrapper::OnReverseModeChange(BOOL bReverse)
{
    RETAILMSG(1,(_T("MSG:[CPanelMediaWrapper::OnReverseModeChange]: %d\r\n"),bReverse));

    CPanelGL::OnReverseModeChange(bReverse);
            
    if (m_bHasMediaStream && !m_bCurPlayingAudioOnly)
    {
        ShowVideoWindow(FALSE);
    }

}

void CPanelMediaWrapper::OnUIChanged(UINT idUI)
{
#if 1

	if(	GET_PANEL_CLASS_ID(idUI) == UI_CLASS_CARPLAY || 
		GET_PANEL_CLASS_ID(idUI) == UI_CLASS_ANDROID_AUTO || 
		GET_PANEL_CLASS_ID(idUI) == UI_CLASS_WEBLINK ||
		GET_PANEL_CLASS_ID(idUI) == UI_CLASS_TV ||
		GET_PANEL_CLASS_ID(idUI) == UI_CLASS_DISK ||
		GET_PANEL_CLASS_ID(idUI) == UI_CLASS_MHL)
	{
		//terminate current media playback:
		TerminateVideoPlayback();

		//do nothing here, please do not select other source.
		/*
		//change source type while current is bluetooth:
		if(m_idCurMediaType == MEDIA_TYPE_A2DP)
		{
			UINT idType = m_ppanelDevice->GetFirstAvailableDevice();
			if(idType)
			{
				SelectMediaType(idType,FALSE);
			}
		}
		*/

		//disable video:
		//EnableVideoPlayer(FALSE);
	}
	else if(GET_PANEL_CLASS_ID(idUI) == UI_CLASS_MEDIA)
	{
		if(m_category==MEDIA_VIDEO)
		{
			TakeGstResource(TRUE);
		}
#if 0
#if CVTE_EN_CARPLAY
		if(m_bCarPlayConnected)
		{
			if(m_category==MEDIA_VIDEO)
			{
				RETAILMSG(1/*DEBUG_MEDIA*/,(_T("++++CPanelMediaWrapper::OnUIChanged video mode, take screen forver ++++\r\n")));
				CarplayAPITakeScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,kAirPlayConstraint_Never,NULL);
			}
			else
			{
				RETAILMSG(1/*DEBUG_MEDIA*/,(_T("++++CPanelMediaWrapper::OnUIChanged music mode, take screen UserInitiated ++++\r\n")));
				CarplayAPITakeScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_UserInitiated,kAirPlayConstraint_Anytime,NULL);
			}
		}
#endif
#endif
	}
	else
	{
		//enable video:
		//EnableVideoPlayer(TRUE);
	}
#endif

}

void CPanelMediaWrapper::OnSRCChanged(UINT idSRC)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG: [CPanelMediaWrapper::OnSRCChanged]: 0x%x\r\n"),idSRC));

    if (GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_MEDIA/* && m_bPlaying*/)
    {
        if (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_CARPLAY /*&& m_category==MEDIA_VIDEO*/)
        {
            if (m_category==MEDIA_VIDEO)
            {
				AbortLoadFile();
				WaitForLoadFileReady();
                Stop();
                CleanUp();
            }
            else
            {
                Pause();
                //PreparePause();
            }
        }
        else
        {
            Pause();
        }
    }
    else
    {
        if (m_bPlaying)
        {
            EnableMonitorPlayinfo(TRUE);
        }
    }

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI || CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	//update widget status:
	if(m_pSkinManager)
	{
		BOOL bFocus=1;
		BOOL bNoFocus=0;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_USB,0,UIOPRT_SETSTATUS,&bNoFocus);
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_A2DP,0,UIOPRT_SETSTATUS,&bNoFocus);
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_AUX,0,UIOPRT_SETSTATUS,&bNoFocus);
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_IPOD,0,UIOPRT_SETSTATUS,&bNoFocus);
		#if CVTE_EN_CARPLAY
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_CARPLAY,0,UIOPRT_SETSTATUS,&bNoFocus);
		#endif
		#if CVTE_EN_ANDROID_AUTO
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_ANDROID_AUTO,0,UIOPRT_SETSTATUS,&bNoFocus);
		#endif
		if (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_AUX)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_AUX,0,UIOPRT_SETSTATUS,&bFocus);
		}
		else if (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_MEDIA)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_USB,0,UIOPRT_SETSTATUS,&bFocus);
		}
		else if (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_BT )
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_A2DP,0,UIOPRT_SETSTATUS,&bFocus);
		}
		else if (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_IPOD )
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_IPOD,0,UIOPRT_SETSTATUS,&bFocus);
		}
		#if CVTE_EN_CARPLAY
		else if (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_CARPLAY)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_CARPLAY,0,UIOPRT_SETSTATUS,&bFocus);
		}
		#endif
		#if CVTE_EN_ANDROID_AUTO
		else if (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_ANDROID_AUTO)
		{
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_ANDROID_AUTO,0,UIOPRT_SETSTATUS,&bFocus);
		}
		#endif
	}
#endif

}
BOOL CPanelMediaWrapper::IsMassStorageMediaType(UINT idType)
{
#ifdef USE_GPS_CARD_MEDIA_PLAY
    return ( idType == MEDIA_TYPE_USB1) || ( idType == MEDIA_TYPE_USB2) || ( idType == MEDIA_TYPE_SD1) || ( idType == MEDIA_TYPE_SD2);
#else
    return ( idType == MEDIA_TYPE_USB1) || ( idType == MEDIA_TYPE_USB2) || ( idType == MEDIA_TYPE_SD1);
#endif
}
void CPanelMediaWrapper::LinkPages(CPanelMediaMain *pMain,CPanelMediaLibrary *pLibrary,CPanelMediaDevice *pDevice)
{
    if (pMain)
    {
        m_ppanelMain = pMain;
        m_ppanelMain->SetPlayMode(m_idPlayMode);
    }
    if (pLibrary)
    {
        m_ppanelLibrary = pLibrary;
        m_ppanelLibrary->SetNotifyWnd(m_hVideoWnd);
    }
    if (pDevice)
    {
        m_ppanelDevice = pDevice;
    }
}
void CPanelMediaWrapper::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
    //static BOOL bplaying_before=m_bPlaying;
    m_bPhoneCallActive = wParam;

    if (GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
    {
        if (wParam) //active
        {
            //bplaying_before = m_bPlaying;
            if (m_bPlaying)
            {
				//20160514 
                //PreparePause();
				Pause();
            }
        }
        else
        {
            SwitchAudioSRC(SRC_AUDIO_ARM);
            //if (bplaying_before /*&& !m_bSpeechOn*/)
			if(m_bEnableAutoPlay)
            {
				//20160514
                //Play();
				PreparePlay();
            }
        }
    }
}
void CPanelMediaWrapper::OnMediaVolumeFade(WPARAM wParam, LPARAM lParam)
{
    if (AutoVolumeStepProc())
    {
        KillTimer(m_hVideoWnd,TIMER_MEDIA_VOLUME_FADE);

        RETAILMSG(DEBUG_MEDIA,(_T("APP:MSG:GUI:MEDIA: [CPanelMediaWrapper::OnMediaVolumeFade]: --\r\n")));
    }
}
void CPanelMediaWrapper::OnVideoWndGesture(WPARAM wParam, LPARAM lParam)
{
    if (m_pSmartBarVol && m_pSmartBarVol->IsVisible())
    {
        return;
    }


    GESTUREINFO gi;
    memset(&gi, 0, sizeof(gi));
    gi.cbSize = sizeof(GESTUREINFO);

    if (GetGestureInfo((HGESTUREINFO)lParam, &gi))
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnVideoWndGesture]: gi.dwID:%d\r\n"),gi.dwID));
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
                if (m_bFullScreenMode)
                {
                    LONG velocity = GID_SCROLL_VELOCITY (gi.ullArguments );
                    LONG angle =   GID_SCROLL_ANGLE (gi.ullArguments );
                    LONG direction = GID_SCROLL_DIRECTION (gi.ullArguments );


                    if (velocity>1000)
                    {
                        switch (direction )
                        {
                            case ARG_SCROLL_DOWN:
                                break;
                            case ARG_SCROLL_UP:
                                break;
                            case ARG_SCROLL_LEFT:
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
								OnMediaPlayerCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PREVIOUS);
#else
								OnMediaPlayerCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_NEXT);
#endif
                                break;
                            case ARG_SCROLL_RIGHT:
#if CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
								OnMediaPlayerCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_NEXT);
#else
								OnMediaPlayerCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PREVIOUS);
#endif
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
                //RETAILMSG(DEBUG_MEDIA,(_T("video windows Clicked on WM_LBUTTONDOWN,x:%d,y:%d\r\n"),pt.x,pt.y));
                if (GetTickCount() - lastTick >500)
                {
                    OnMediaPlayerCommand(MEDIA_CMD_VIDEO_CONTROL,MEDIA_VIDEO_TOGGLE);
                    lastTick = GetTickCount();
                }

                //test
                /*if(m_bHasMediaStream && m_pStreamInfo && m_iAudioStreamCount>1)
                {
                	int i=m_pStreamInfo->GetCurrentStream(CM_STREAM_AUDIO);

                	RETAILMSG(DEBUG_MEDIA,(_T("vGetCurrentStream:%d\r\n"),i));
                	m_pStreamInfo->SetCurrentStream(CM_STREAM_AUDIO,i==0?1:0);
                }
                */
            }
            break;
        }
    }

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnVideoWndGesture]: GetGestureInfo failed.\r\n")));
}
void CPanelMediaWrapper::OnUpdatePlaybackInfo(WPARAM wParam, LPARAM lParam)
{
	//更新进度条，播放时间
	UpdatePlayPostion();

    //
    m_bEnableQuit = !(m_bHasMediaStream && !m_bCurPlayingAudioOnly && GetTickCount()-m_dwPlayTick<3000);

    //auto show video window:
    if (!m_bManualPlayFile
        && !m_bBrowsing
        && m_bPlayFileReady
        && m_pPlayer
        && m_bPlaying
        && !m_bCurPlayingAudioOnly
        &&  m_bHasMediaStream
        && !m_pSkinManager->GetGUI()->IsAnimating() )
    {

        if (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN)
            && m_bOnBrake && !m_bOnReverseMode)
        {
            if (!m_bVideoWndVisible)
            {
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnUpdatePlaybackInfo] auto show video windowr\n")));
                if (m_pSmartBarVol && m_pSmartBarVol->IsVisible() )
                {
                    m_pSmartBarVol->Show(FALSE);
                }
                ShowVideoWindow(TRUE);
            }

            //auto full screen:
            if (!m_bFullScreenMode)
            {
                if (GetTickCount()-m_dwLastFullScreenTick>8000)
                {
                    SwitchFullscreen(1);
                }
            }
        }
//        else
//        {
//            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnUpdatePlaybackInfo] idCurUI[%X] m_bOnBrake[%d] m_bOnReverseMode[%d] \r\n"),
//                m_pSkinManager->GetConfig()->GetCurParam()->idCurUI,m_bOnBrake,m_bOnReverseMode));
//        }
    }
//	    else
//	    {
//	        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnUpdatePlaybackInfo] m_bManualPlayFile[%d] m_bBrowsing[%d] m_bPlayFileReady[%d] m_bCurPlayingAudioOnly[%d] m_bHasMediaStream[%d] IsAnimating[%d]\r\n")
//	            ,m_bManualPlayFile,m_bBrowsing,m_bPlayFileReady,m_bCurPlayingAudioOnly,m_bHasMediaStream,m_pSkinManager->GetGUI()->IsAnimating()));
//	    }

    //auto pause:
    UINT idClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);

    if ( m_bManualPause
         || (idClass != UI_CLASS_MEDIA && idClass != UI_CLASS_BT)
#ifdef USE_GPS_CARD_MEDIA_PLAY
         || (m_idCurMediaType != MEDIA_TYPE_USB1 && m_idCurMediaType != MEDIA_TYPE_USB2 && m_idCurMediaType != MEDIA_TYPE_SD1 && m_idCurMediaType != MEDIA_TYPE_SD2)
#else
         || (m_idCurMediaType != MEDIA_TYPE_USB1 && m_idCurMediaType != MEDIA_TYPE_USB2 && m_idCurMediaType != MEDIA_TYPE_SD1/* && m_idCurMediaType != MEDIA_TYPE_NAND*/)
#endif
         || m_bPhoneCallActive
		 || m_bSROn
         || (m_idSystemPowerState != SYS_POWER_ON))
    {
        if (m_bPlaying /*&& GetTickCount()-m_dwPlayTick>2000*/)
        {
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnUpdatePlaybackInfo] media pause, current src is not media,idClass:%d,m_idCurMediaType:%d\r\n"),idClass,m_idCurMediaType));
            //modified by whh 20160514 
			//merge FVW8010
			//Pause();
			PreparePause(FALSE);
        }
    }
}
void CPanelMediaWrapper::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnAvailableSourceChanged] :src id:%d, bavail: %d\r\n"),wParam,lParam));

    if (m_ppanelLibrary)
        m_ppanelLibrary->OnAvailableSourceChanged(wParam,lParam);
    if (m_ppanelMain)
        m_ppanelMain->OnAvailableSourceChanged(wParam,lParam);

    //
    if (wParam == MEDIA_TYPE_A2DP)
    {
        m_bA2DPAvailable=lParam;
    }
}
void CPanelMediaWrapper::OnBrakeStatusChanged(BOOL bOn)
{
    CPanelGL::OnBrakeStatusChanged(bOn);

    if (m_ppanelMain)
        m_ppanelMain->ShowAlert(!m_bOnBrake);

    if (!m_bOnBrake)
    {
        if (IsWindowVisible(m_hVideoWnd))
        {
#if CVTE_EN_DYNAMIC_LOGO
			if(!m_bDynamicLogoPlaying)
			{
				ShowVideoWindow(FALSE);
			}
#else
			ShowVideoWindow(FALSE);
#endif     
        }
    }
    else
    {
        if (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN)
            && !m_bCurPlayingAudioOnly
            &&  m_bHasMediaStream
            && !m_pSkinManager->GetGUI()->IsAnimating()
            && !IsWindowVisible(m_hVideoWnd)
           )
        {
            ShowVideoWindow(TRUE);
        }
    }
    // 	if((!m_bOnBrake||m_bCurPlayingAudioOnly) && IsWindowVisible(m_hVideoWnd))
    // 	{
    // 		ShowVideoWindow(FALSE);
    // 	}
}

void CPanelMediaWrapper::Render()
{
    if (!BeginRender())return;

    EndRender();
}
BOOL CPanelMediaWrapper:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
    CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_hGstReadyEvt = CreateEvent( NULL, FALSE, FALSE, _T("EVT_GSTREAMER_READY") );
    //create video window:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WNDCLASS wc = {0};
    memset (&wc, 0, sizeof(wc));

    // Register  window class
    wc.style            =  0;//CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = VideoWndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = entry.hInstance;
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

    m_hOpenFileEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
    m_hOpenFileReadyEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
    m_hLoadMediaModuleEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

    m_category = MEDIA_CATEGORY(m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory);

    SetPlayMode(m_pSkinManager->GetConfig()->GetCurParam()->idMediaPlayMode);

    LoadMediaModule();

	TakeGstResource(TRUE);	//Modified to make it the same as FVW8010


	return TRUE;
}
void CPanelMediaWrapper::LoadMediaModule()
{
    HANDLE hThread=CreateThread(NULL,NULL,LoadMediaModuleThreadPro,this,NULL,NULL);

    //SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL);
    if (hThread != INVALID_HANDLE_VALUE)
    {
#if CVTE_EN_DYNAMIC_LOGO
		CeSetThreadPriority(hThread, CE_THREAD_PRIO_256_ABOVE_NORMAL);
#endif
        CloseHandle(hThread);
        hThread = INVALID_HANDLE_VALUE;
    }
}
DWORD CPanelMediaWrapper::LoadMediaModuleThreadPro(LPVOID lpParam)
{
    CPanelMediaWrapper *panel=(CPanelMediaWrapper*)lpParam;

    if (panel)
    {
        panel->LoadMediaModuleProc();
    }

    return 0;
}
BOOL CPanelMediaWrapper::WaitForMediaModuleReady()
{
	//Modified to make it the same as FVW8010
	return WaitForSingleObject(m_hLoadMediaModuleEvent,30000) == WAIT_OBJECT_0;
}

BOOL CPanelMediaWrapper::LoadMediaModuleProc()
{
    if (!m_bMediaModuleLoaded)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::LoadMediaModuleProc]....\r\n")));
        //media:
        if (FAILED( ::CSRCreateMedia(&m_pCSRMedia) ))
        {
            RETAILMSG(DEBUG_MEDIA,(_T("ERROR:[CPanelMediaWrapper::LoadMediaModuleProc]: CSRCreateMedia FAILED!!\r\n")));

            return FALSE;
        }
        else
        {
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::LoadMediaModuleProc]: CSRCreateMedia SUCCEED!!\r\n")));
        }

        m_bMediaModuleLoaded = TRUE;

        g_bMediaModuleLoaded = TRUE;

        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::LoadMediaModuleProc]: SUCCEED!!!!....\r\n")));

        Sleep(100);

        SetEvent(m_hLoadMediaModuleEvent);

        return TRUE;
    }
    else
    {
        SetEvent(m_hLoadMediaModuleEvent);

        return TRUE;
    }

}

void CPanelMediaWrapper::SetVideoColorKey(DWORD dwColorKey)
{
	CReg reg;
	reg.Open(HKEY_CURRENT_USER,_T("SOFTWARE\\Gstreamer\\"));
	DWORD col=reg.ValueDW(_T("VideoColorKey"),RGB(8,8,8));
	if(col!=dwColorKey)
	{
		RETAILMSG(1,(_T("==SetVideoColorKey: 0x%x \r\n"),dwColorKey));
		reg.SetDW(_T("VideoColorKey"),dwColorKey);
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
    //	POINT pt;

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
            // 			RETAILMSG(DEBUG_MEDIA,(_T("+++++++CPanelMediaWrapper::VideoWndProc WM_ERASEBKGND!!!!....\r\n")));

			CReg reg;
			reg.Open(HKEY_CURRENT_USER,_T("SOFTWARE\\Gstreamer\\"));
			DWORD col=reg.ValueDW(_T("VideoColorKey"),RGB(8,8,8));

			RETAILMSG(1,(_T("MSG:[CPanelMediaWrapper::VideoWndProc] VideoWndProc WM_ERASEBKGND GET VideoColorKey: 0x%x \r\n"),col));
            RECT rc;
			static HBRUSH br=CreateSolidBrush(/*RGB(8,8,8)*/col);
            GetClientRect(hWnd,&rc);
			rc.bottom-=1;
            ::FillRect(GetDC(hWnd), &rc, br);

			// 			RECT rc;
			// 			static HBRUSH br=CreateSolidBrush(RGB(8,8,8));
			// 			GetClientRect(hWnd,&rc);
			// 			::FillRect(GetDC(hWnd), &rc, br);
            // 			//
            // 			if(pMediaPanel->m_pVideo)
            // 			{
            // 				HRESULT hr=pMediaPanel->m_pVideo->UpdateVideo();
            // 				RETAILMSG(DEBUG_MEDIA,(_T("*******ShowVideoWindow m_pVideo->UpdateVideo:%d\r\n"),hr));
            // 			}
            return 0;
        }
        break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // 		RETAILMSG(DEBUG_MEDIA,(_T("+++++++CPanelMediaWrapper::VideoWndProc WM_PAINT!!!!....\r\n")));

            // TODO: Add any drawing code here...

            EndPaint(hWnd, &ps);
            break;
        case WM_TIMER:
            if (pMediaPanel)
            {
                if (wParam == TIMER_UPDATE_PLAYINFO)
                {
                    pMediaPanel->OnUpdatePlaybackInfo(0,NULL);
                }
                else if (wParam == TIMER_MEDIA_VOLUME_FADE)
                {
                    pMediaPanel->OnMediaVolumeFade(0,NULL);
                }
                else if (wParam == TIMER_MEDIA_LOADFILE_READY)
                {
                    KillTimer(hWnd,TIMER_MEDIA_LOADFILE_READY);
                    pMediaPanel->OnPlayerLoadFileReady();
                }
                else if (wParam == TIMER_MEDIA_LOADFILE_FAILED)
                {
                    KillTimer(hWnd,TIMER_MEDIA_LOADFILE_FAILED);
                    pMediaPanel->OnPlayerLoadFileFailed();
                }
                else if (wParam == TIMER_MEDIA_PLAY_COMPLETE)
                {
                    KillTimer(hWnd,TIMER_MEDIA_PLAY_COMPLETE);
                    pMediaPanel->OnPlayerComplete();
                }
                else if (wParam == TIMER_MEDIA_AUTO_SELECT_DEVICE)
                {
                    KillTimer(hWnd,TIMER_MEDIA_AUTO_SELECT_DEVICE);
                    pMediaPanel->AutoSelectDevice();
                }
                else if (wParam == TIMER_MEDIA_DELAY_OPERATION)
                {
                    KillTimer(hWnd,TIMER_MEDIA_DELAY_OPERATION);
                    pMediaPanel->OnDelayOperation();
                }
				//merge from FVW8010
				//whh 20160514
				else if(wParam == TIMER_MEDIA_AUTO_RESUME)
				{
					KillTimer(hWnd,TIMER_MEDIA_AUTO_RESUME);
					pMediaPanel->OnAutoResume();
				}
            }
            break;
        case WM_MEDIAPLAYER_MSG:
            if (pMediaPanel)
            {
                pMediaPanel->OnPlayerMsg(wParam,lParam);
            }
            break;
        case WM_MEDIAPLAYER_MSG_EX:
            if (pMediaPanel)
            {
                pMediaPanel->OnPlayerMsgEx(wParam,lParam);
            }
            break;
        case WM_RINGTONE_MSG:
            if (pMediaPanel)
            {
                pMediaPanel->OnRingToneMsg(wParam,lParam);
            }
            break;
		#if CVTE_EN_DYNAMIC_LOGO
        case WM_DYNAMIC_LOGO_MSG:
            if (pMediaPanel)
            {
                pMediaPanel->OnDynamicLogoMsg(wParam,lParam);
            }
            break;
		#endif
        case WM_DESTROY:
            break;
        case WM_GESTURE:
        {
            if (pMediaPanel)
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
BOOL CPanelMediaWrapper::PreparePlayFile(LPCTSTR lpFileName,DWORD dwPos)
{

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::PreparePlayFile] %s \r\n"),lpFileName));

    MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000);

    //SetMediaVolume(0,TRUE);

    EnablePlayUI(FALSE,__FUNCTION__);

    m_bManualPlayFile = TRUE;

    m_strFileToPlay = lpFileName;
    m_dwPosToPlay = dwPos;

	//和之前的播放方式相同
    SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL);

    return TRUE;
}
BOOL CPanelMediaWrapper::PlayFile(LPCTSTR lpFileName,DWORD dwPos)
{
    // 	if(!lpFileName)
    // 	{
    // 		PlayFailed();
    // 		return FALSE;
    // 	}

    /*
    if(m_hOpenFileThread != INVALID_HANDLE_VALUE)
    {
    	//m_bAbortOpeningFile = TRUE;

    	SetEvent(m_hOpenFileEvent);

    	DWORD icount=0;
    	DWORD dwExitCode;
    	//do
    	{
    		//Sleep(5);
    		if (GetExitCodeThread(m_hOpenFileThread, &dwExitCode))
    		{
    			//break;
    		if(dwExitCode == STILL_ACTIVE)
    		{
    			RETAILMSG(DEBUG_MEDIA,(_T("++++PlayFile -----Already playing..., wait...  icount=%d --\r\n"),icount));

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
    */

    if (m_bOpeningFile)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::PlayFile] Already opening..., try to abort --\r\n")));
        SetEvent(m_hOpenFileEvent);

        /*
        int itry=0;
        do
        {
        	itry++;
        	Sleep(10);
        }
        while(m_bOpeningFile && itry<5);

        if(m_bOpeningFile)
        {
        	RETAILMSG(DEBUG_MEDIA,(_T("++++PlayFile -----Already opening..., try to abort failed, return--\r\n")));
        	return FALSE;
        }
        */

		AbortLoadFile();
		if(!WaitForLoadFileReady())
		{
			RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::PlayFile] Already opening...,WaitForLoadFileReady failed! --\r\n")));
		}
	}

    //
    if (m_bHasMediaStream)
    {
        ShowVideoWindow(FALSE);
        EnableMonitorPlayinfo(FALSE);

        if (m_pPlayer && m_bPlaying)
        {
            m_pPlayer->Pause();
        }

        m_bHasMediaStream = FALSE;
        m_bPlaying = FALSE;
        m_bNeedSetIniPos = FALSE;
        m_bEnableQuit = TRUE;
        m_bNeedProcessCompleteNotify = FALSE;

        //MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,2000);
    }
    else
    {
        //MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,2000);
    }

	if(!m_bSROn && !m_bPhoneCallActive)
	{
		MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,2000);
	}

    // 	MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,2000);


    //clear play info:
    if (m_ppanelMain)
    {
    	//设置loading状态，loading图标会显示
        m_ppanelMain->SetLoadingStatus(LOAD_STATUS_LOADING);

        m_ppanelMain->SetProgressValue(0);
        m_ppanelMain->SetElapsedTime(0);
        m_ppanelMain->SetRemainTime(0);

	 //清空播放信息，此时音乐，艺术家，专辑都是用默认
        m_ppanelMain->SetTitle(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
        m_ppanelMain->SetArtist(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
        m_ppanelMain->SetAlbum(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));

        m_ppanelMain->PlayInfoFlyIn();
    }
    if (m_ppanelLibrary)
    {
        m_ppanelLibrary->ShowProgressSlider(FALSE);
        m_ppanelLibrary->SetProgressValue(0);

        m_ppanelLibrary->SetLoadingStatus(LOAD_STATUS_LOADING);
	//设置当前播放的文件,m_boxLibrary移到当前文件位置
        m_ppanelLibrary->SetCurPlayFile(lpFileName);
    }

    m_bOpeningFile = TRUE;

    m_bLoadFileReady = FALSE;
    m_bPlayFileReady = FALSE;

    m_strFileToPlay = lpFileName;
    m_dwPosToPlay = dwPos;

    m_bManualPause = FALSE;
	m_bAbortLoadFile = FALSE;

    //begin thread to open file:
    ResetEvent(m_hOpenFileEvent);
    ResetEvent(m_hOpenFileReadyEvent);
    m_bAbortOpeningFile = FALSE;
#if CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON
    m_dwOpenFileDelay=m_bHasMediaStream?200:2000;
#else
    m_dwOpenFileDelay=m_bHasMediaStream?200:1000;
#endif
	//创建打开文件线程
    m_hOpenFileThread=CreateThread(NULL,NULL,OpenFileThreadPro,this,NULL,NULL);

	CeSetThreadPriority(m_hOpenFileThread, CE_THREAD_PRIO_256_ABOVE_NORMAL);
	if(m_hOpenFileThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hOpenFileThread);
		m_hOpenFileThread = INVALID_HANDLE_VALUE;
	}

    return TRUE;
}
DWORD CPanelMediaWrapper::OpenFileThreadPro(LPVOID lpParam)
{
    CPanelMediaWrapper *panel=(CPanelMediaWrapper*)lpParam;

    if (panel)
    {
		RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] ++ m_dwOpenFileDelay[%d]\r\n"),panel->m_dwOpenFileDelay));
        if (WaitForSingleObject(panel->m_hOpenFileEvent,panel->m_dwOpenFileDelay) == WAIT_OBJECT_0)
        {
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] PlayFile AbortOpeningFile \r\n")));
			panel->OnPlayerLoadFileAbort(__FUNCTION__,__LINE__);
        }
        else
        {
            BOOL bPlay=TRUE;
            /*while*/
            if (panel->m_pSkinManager->GetGUI()->IsAnimating()/* || panel->m_bPhoneCallActive*/)
            {
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] =>PlayFile IsAnimating, wait 1000s\r\n")));
                Sleep(1000);
            }
            if (panel->m_idSystemPowerState != SYS_POWER_ON || WaitForSingleObject(panel->m_hOpenFileEvent,0) == WAIT_OBJECT_0)
            {
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] =>PlayFile power off or manual break\r\n")));
                bPlay=FALSE;
            }
			if(panel->m_bAbortLoadFile)
			{
				RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] =>bPlay=FALSE;\r\n")));
				bPlay=FALSE;
			}
            if (bPlay)
            {
				RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] =>PlayFilePro\r\n")));
				//播放文件
                		panel->PlayFilePro(panel->m_strFileToPlay.String(),panel->m_dwPosToPlay);
            }
			else
			{
				RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] =>OnPlayerLoadFileAbort\r\n")));
				panel->OnPlayerLoadFileAbort(__FUNCTION__,__LINE__);
			}
		}

        //开启ExploreThreadPro线程，保存文件绝对路径，用于文件夹模式，在RefreshLibrary函数时候创建
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OpenFileThreadPro] SetEvent\r\n")));
        SetEvent(panel->m_hOpenFileReadyEvent);
        panel->m_bOpeningFile = FALSE;

    }

    return 0;
}

BOOL CPanelMediaWrapper::WaitForLoadFileReady(INT iTimeOut)
{
    BOOL bRet = FALSE;
    if (m_bOpeningFile)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::WaitForLoadFileReady] is opening, wait\r\n")));
		while (m_bOpeningFile)
		{
			if(WaitForSingleObject(m_hOpenFileReadyEvent,5000) == WAIT_OBJECT_0)
			{
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::WaitForLoadFileReady] successfully\r\n")));
                bRet = TRUE;
                break;
            }
            else
            {
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::WaitForLoadFileReady] failed\r\n")));
                bRet = FALSE;
                break;
            }
		}
		return bRet;
		//return WaitForSingleObject(m_hOpenFileReadyEvent,iTimeOut)==WAIT_OBJECT_0;
	}
	else
	{
		//RETAILMSG(DEBUG_MEDIA,(_T("#### WaitForLoadFileReady , not opening--\r\n")));
		return TRUE;
	}
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

    RETAILMSG(DEBUG_MEDIA,(_T("[%x]****begin to play file :%s..........\r\n"),GetCurrentThreadId(),lpFileName));

    if (!m_bMediaModuleLoaded)
    {
        WaitForMediaModuleReady();
    }

    if (!m_pCSRMedia /*|| !lpFileName*/)
    {
        goto ERR;
    }

	if(m_bAbortLoadFile)
	{
		goto ABORT;
	}

	//自动播放没有传入歌名
	if(!lpFileName)
	{
		//auto play:
		AutoPlayDevice();
		return S_FALSE;
	}

    if (!IsMediaFileExists(lpFileName))
    {
        RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper:: %s does not exist---\r\n"),lpFileName));
        PlayFailed();
        return S_FALSE;
        /*goto ERR;*/
    }

    //cleanup first:
    CleanUp();

    Sleep(100);

	if(m_category == MEDIA_VIDEO && !m_bMediaPlayerTakeGst)
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("------CPanelMediaWrapper:: gst is taken by other app, abort playing\r\n")));
		goto ABORT;
	}

	//BOOL bNeedMute = m_pCommander? !m_pCommander->GetItemData(MCU_MUTE_MODE) : FALSE;

    m_bLoadFileReady = FALSE;
    m_bPlayFileReady = FALSE;

    m_bEnableSmartBar = FALSE;

	//
	if(m_pSmartBarVol)
	{
		if(m_pSmartBarVol->IsVisible())
		{
			m_pSmartBarVol->Show(FALSE);
		}
		m_pSmartBarVol->EnableShow(FALSE);
	}
	if(m_pSmartBarMedia && m_category==MEDIA_VIDEO)
	{
		if(m_pSmartBarMedia->IsVisible())
		{
			m_pSmartBarMedia->Show(FALSE);
		}
		m_pSmartBarMedia->EnableShow(FALSE);
	}

	if(m_bAbortLoadFile)
	{
		goto ABORT;
	}
    hr = m_pCSRMedia->CreatePlayer(&m_pPlayer);
    if (FAILED(hr))
        goto ERR;

    hr = m_pPlayer->QueryInterface(IID_ICSREventSource, (void**)&m_pEvent);
    if (FAILED(hr))
        goto ERR;

    hr = m_pEvent->SetNotifyWindow(NULL/*m_hVideoWnd*//*m_hMainWnd*/, 0, 0);
    if (FAILED(hr))
        goto ERR;

    hr = m_pPlayer->QueryInterface(IID_ICSRVideo, (void**)&m_pVideo);
    if (FAILED(hr))
        goto ERR;


    // 	hr = m_pVideo->SetOverlayLayer(2);	//	Set video overlay layer here
    // 	if (FAILED(hr))
    // 	{
    // 		RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper::m_pVideo->SetOverlayLayer(2) FAILED!!!---\r\n")));
    // 		//goto ERR;
    // 	}
    // 	else
    // 	{
    // 		RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper::m_pVideo->SetOverlayLayer(2) SUCCEED!!!---\r\n")));
    // 	}

    hr = m_pVideo->SetVideoWindow(m_hVideoWnd);
    if (FAILED(hr))
        goto ERR;

    //
    m_pVideo->KeepAspectRatio(TRUE);

    hr = m_pPlayer->QueryInterface(IID_ICSRWaveoutAudio, (void**)&m_pAudio);
    if (FAILED(hr))
        goto ERR;

#if USE_WAV_ONCHIP
    hr = m_pAudio->SetDeviceID(1);
#endif
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

    if (!IsMediaFileExists(lpFileName))
    {
        RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper:: %s does not exist---\r\n"),lpFileName));
        goto ERR;
    }

	if(m_bAbortLoadFile)
	{
		goto ABORT;
	}
    hr = m_pInspector->InspectFile(lpFileName,TRUE);
    if (FAILED(hr))
        goto ERR;

    hr = m_pInspector->WaitForCompletion(10000, NULL);
    // 	for (int nMetaType = CM_META_DATA_FILE; nMetaType <= CM_META_DATA_TAG; nMetaType++)
    // 	{
    // 		ICSRMetaData* pMetaData = NULL;
    // 		hr = m_pMediaInfo->GetMetaData((CM_META_TYPE)nMetaType, &pMetaData);
    // 		if (SUCCEEDED(hr))
    // 			DumpMetaData((CM_META_TYPE)nMetaType, pMetaData);
    // 	}

	if(m_bAbortLoadFile)
	{
		goto ABORT;
	}

    BOOL bValidFile=FALSE;

    //File:
    ICSRMetaData* pFileMetaData = NULL;
    hr = m_pMediaInfo->GetMetaData(CM_META_DATA_FILE, &pFileMetaData);
    if (SUCCEEDED(hr))
    {
        DumpMetaData(CM_META_DATA_FILE, pFileMetaData);
    }
    else
    {
        RETAILMSG(DEBUG_MEDIA,(_T("----GetMetaData CM_META_DATA_FILE FAILED!\r\n")));
    }

    //Audio:
    ICSRMetaData* pAudioMetaData = NULL;
    hr = m_pMediaInfo->GetMetaData(CM_META_DATA_AUDIO, &pAudioMetaData);
    if (SUCCEEDED(hr))
    {
        DumpMetaData(CM_META_DATA_AUDIO, pAudioMetaData);
        bValidFile = TRUE;
    }
    else
    {
        RETAILMSG(DEBUG_MEDIA,(_T("----GetMetaData CM_META_DATA_AUDIO FAILED!\r\n")));
    }

    //Video:
    int video_cx=0;
    int video_cy=0;
    ICSRMetaData* pVideoMetaData = NULL;
    hr = m_pMediaInfo->GetMetaData(CM_META_DATA_VIDEO, &pVideoMetaData);
    if (SUCCEEDED(hr))
    {
        DumpMetaData(CM_META_DATA_VIDEO, pVideoMetaData);

		if(!IsDecoderSupport(CM_META_DATA_VIDEO, pVideoMetaData))
		{
			RETAILMSG(1,(_T("ERR:[%s]: =>Not Support File\r\n"),TEXT(__FUNCTION__)));

			goto ERR;			
		}
        bValidFile = TRUE;

        GetVideoSize(pVideoMetaData,&video_cx,&video_cy);
        if (video_cy>1080)
        {
            RETAILMSG(DEBUG_MEDIA,(_T("----video_cx:%d,video_cy:%d---video too large!!! skip this file\r\n"),video_cx,video_cy));

            goto ERR;
        }
        else
        {
            RETAILMSG(DEBUG_MEDIA,(_T("----video_cx:%d,video_cy:%d---\r\n"),video_cx,video_cy));
        }
    }
    else
    {
        RETAILMSG(DEBUG_MEDIA,(_T("----GetMetaData CM_META_DATA_VIDEO FAILED!\r\n")));
    }

    if (!bValidFile)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("----GetMetaData bValidFile FALSE!\r\n")));
        goto ERR;
    }

    //Tag:
    ICSRMetaData* pTagMetaData = NULL;
    if (!pVideoMetaData)
    {
		BOOL bGotTitle = FALSE;
		BOOL bGotArtist = FALSE;
		BOOL bGotAlbum = FALSE;
		BOOL bGotAlbumArt = FALSE;

        hr = m_pMediaInfo->GetMetaData(CM_META_DATA_TAG, &pTagMetaData);
        if (SUCCEEDED(hr))
        {
			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("----GetMetaData CM_META_DATA_TAG SUCCEED\r\n")));

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
                    RETAILMSG(DEBUG_MEDIA,(_T("retrieve title SUCCEED,size:%d,%s\r\n"),dwSize,(LPCWSTR)pValue));
					bGotTitle = TRUE;
                    if (m_ppanelMain)
                        m_ppanelMain->SetTitle((LPCWSTR)pValue);
                }
                free(pValue);
            }
            else
            {
                WCHAR name[MAX_STRING];
                if (GetFileNameFromPath(lpFileName,name,MAX_STRING))
                {
                    if (m_ppanelMain)
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
                    if (m_ppanelMain)
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
                    RETAILMSG(DEBUG_MEDIA,(_T("retrieve artist SUCCEED,size:%d,%s\r\n"),dwSize,(LPCWSTR)pValue));
					bGotArtist = TRUE;
                    if (m_ppanelMain)
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
                if (m_ppanelMain)
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
                    RETAILMSG(DEBUG_MEDIA,(_T("retrieve album SUCCEED,size:%d,%s\r\n"),dwSize,(LPCWSTR)pValue));
					bGotAlbum = TRUE;
                    if (m_ppanelMain)
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
                if (m_ppanelMain)
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
                        RETAILMSG(DEBUG_MEDIA,(_T("##### retrieve album art SUCCEED,size:%d\r\n"),dwSize));
						bGotAlbumArt = TRUE;
                        if (m_ppanelMain)
                            m_ppanelMain->OnPrepareUpdateAlbumArt(pbImage,dwSize);
                    }
                    delete[] pbImage;
                }
            }
            else
            {
                RETAILMSG(DEBUG_MEDIA,(_T("---NO album art,set default---\r\n")));
                if (m_ppanelMain)
                    m_ppanelMain->OnPrepareUpdateAlbumArt(NULL,0);
            }
        }
        //else //get ID3v1
		if(!bGotTitle || !bGotArtist || !bGotAlbum)
        {
            ID3V1INFO id3v1info;
			memset(&id3v1info, 0, sizeof(ID3V1INFO));
            if (GetID3v1Info(lpFileName,&id3v1info))
            {
                RETAILMSG(1/*DEBUG_MEDIA*/,(_T("----GetMetaData ID3v2 FAILED, try to get ID3v1 SUCCEED\r\n")));

                wchar_t title[32]= {0};
                wchar_t album[32]= {0};
                wchar_t artist[32]= {0};

                // 				MultiByteToWideChar(CP_ACP, 0, id3v1info.Title, -1, title, 32);
                // 				MultiByteToWideChar(CP_ACP, 0, id3v1info.Album, -1, album, 32);
                // 				MultiByteToWideChar(CP_ACP, 0, id3v1info.Artist, -1, artist, 32);


                if (m_ppanelMain)
                {
                    //title://设置播放标题
					if(!bGotTitle)
					{
						//GBKTOUnicode(id3v1info.Title,30,title,30);
						MultiCharTOUnicode(id3v1info.Title,30,title,30,GetLanguageCodePage(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage));
						if (title[0])
						{
							m_ppanelMain->SetTitle(title);
						}
						else
						{
							WCHAR name[MAX_STRING];
							if (GetFileNameFromPath(lpFileName,name,MAX_STRING))
							{
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
								m_ppanelMain->SetTitle(name);
							}
						}
					}

                    //artist://设置艺术家
					if(!bGotArtist)
					{
						//GBKTOUnicode(id3v1info.Artist,30,artist,30);
						MultiCharTOUnicode(id3v1info.Artist,30,artist,30,GetLanguageCodePage(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage));
						if (artist[0])
						{
							m_ppanelMain->SetArtist(artist);
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
							m_ppanelMain->SetArtist(name);
						}
					}

                    //album://设置专辑
					if(!bGotAlbum)
					{
						//GBKTOUnicode(id3v1info.Album,30,album,30);
						MultiCharTOUnicode(id3v1info.Album,30,album,30,GetLanguageCodePage(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage));
						if (album[0])
						{
							m_ppanelMain->SetAlbum(album);
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
							m_ppanelMain->SetAlbum(name);
						}
					}

                    //clear album art:
					if(!bGotAlbumArt)
					{
						m_ppanelMain->OnPrepareUpdateAlbumArt(NULL,0);
					}
                }
            }
            else
            {
                RETAILMSG(1/*DEBUG_MEDIA*/,(_T("----GetMetaData ID3v2 FAILED!, try to get ID3v1 FAILED\r\n")));

                if (m_ppanelMain)
                {
                    //set default:
                    WCHAR name[MAX_STRING];
                    if (GetFileNameFromPath(lpFileName,name,MAX_STRING))
                    {
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
                        m_ppanelMain->SetTitle(name);
                    }

#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
                    swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_ARTISTS),GetResourceString(IDS_UNKNOWN));
#else
                    swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
                               GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
#endif
                    m_ppanelMain->SetArtist(name);

#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
                    swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_LIBRARY_MENU_ALBUMS),GetResourceString(IDS_UNKNOWN));
#else
                    swprintf_s(name,MAX_STRING,_T("%s %s"),GetResourceString(IDS_UNKNOWN),
                               GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
#endif
                    m_ppanelMain->SetAlbum(name);

                    //clear album art:
                    m_ppanelMain->OnPrepareUpdateAlbumArt(NULL,0);
                }
            }
        }
    }
    else
    {
        //set video name
        WCHAR name[MAX_STRING];
        if (GetFileNameFromPath(lpFileName,name,MAX_STRING))
        {
            if (m_ppanelMain)
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
            if (m_ppanelMain)
                m_ppanelMain->SetTitle(name);
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

	if(m_bAbortLoadFile)
	{
		goto ABORT;
	}
    hr = m_pPlayer->OpenFile(lpFileName);
    if (FAILED(hr))
        goto ERR;


    //
    SetMediaAudioVolume(0,FALSE);

    // 	//
    // 	hr = m_pEvent->SetNotifyWindow(m_hVideoWnd/*m_hMainWnd*/, WM_MEDIAPLAYER_MSG, 0);
    // 	if (FAILED(hr))
    // 		goto ERR;
    //
    // 	//start to play:
    // 	hr = m_pPlayer->Play();
    // 	if (FAILED(hr))
    // 		goto ERR;


    m_strCurrentPlayFile = lpFileName;

    //has video:
    m_bCurPlayingAudioOnly = !pVideoMetaData;

    //ui:
    if (m_ppanelMain)
	{
		m_ppanelMain->SetVideoMode(!m_bCurPlayingAudioOnly);
		m_ppanelMain->SetFileLoadFlag(TRUE);
	}

    //update now playing title:
    if (m_ppanelLibrary)
    {
        WCHAR str[MAX_STRING];
        swprintf_s(str,MAX_STRING,GetResourceString(IDS_NOWPLAYING_PLAYING),
                   m_ppanelLibrary->GetCurIndex()+1,
                   m_ppanelLibrary->GetTrackCount());

        if (m_ppanelMain)
        {
            m_ppanelMain->SetNowPlayingTitle(str);
        }

    }

    m_bNeedSetIniPos = (dwPos != 0);
    m_dwIniPos=dwPos;

    m_dwLastFullScreenTick=m_dwPlayTick;

    m_bEnableQuit = FALSE;

    m_bPlayCompleteDetection = TRUE;


    //
    hr = m_pEvent->SetNotifyWindow(m_hVideoWnd/*m_hMainWnd*/, WM_MEDIAPLAYER_MSG, 0);
    if (FAILED(hr))
        goto ERR;

    if (!m_bCurPlayingAudioOnly)
    {
        //delay:
        Sleep(1000);

        //auto select stream:
        if (m_pStreamInfo)
        {
            m_iAudioStreamCount = m_pStreamInfo->GetStreamCount(CM_STREAM_AUDIO);
            RETAILMSG(DEBUG_MEDIA,(_T("------GetStreamCount CM_STREAM_AUDIO :%d---\r\n"),m_iAudioStreamCount));

            if (m_iAudioStreamCount>1)
            {
                m_pStreamInfo->SetCurrentStream(CM_STREAM_AUDIO,0);
            }

            m_iVideoStreamCount = m_pStreamInfo->GetStreamCount(CM_STREAM_VIDEO);
            RETAILMSG(DEBUG_MEDIA,(_T("------GetStreamCount CM_STREAM_VIDEO :%d---\r\n"),m_iVideoStreamCount));

        }
    }

    //start to play:
    if (m_idSystemPowerState == SYS_POWER_ON /*&& !m_bSpeechOn && !m_bPhoneCallActive*/ && !m_bManualMute)
    {
        if (m_bPhoneCallActive)
        {
            m_bLoadFileReady = TRUE;
            SetTimer(m_hVideoWnd,TIMER_MEDIA_LOADFILE_READY,100,NULL);
        }
        else
        {
			if(!m_bSROn && !m_bPhoneCallActive)
			{
				hr = m_pPlayer->Play();
				if (FAILED(hr))
					goto ERR;
				EnableMonitorPlayinfo(TRUE);
			}

            //EnableMonitorPlayinfo(TRUE);
        }
    }
	else
	{
		RETAILMSG(1, (TEXT("ERR:PlayFilePro: m_bManualMute[%d] !!!!!!!!!!!!!!!!!!\r\n"),m_bManualMute));
	}

    m_dwPlayTick=GetTickCount();

    m_bHasMediaStream=TRUE;


    RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper::PlayFile SUCCEED!!:%s---\r\n"),lpFileName));

    return hr;

ERR:
    RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper::PlayFile FAILED!!:%s---\r\n"),lpFileName));
    m_strCurrentPlayFile = lpFileName;

    CleanUp();
    PlayFailed(lpFileName,LOAD_STATUS_FAILED_INVALID_FILE);
	m_bEnableSmartBar = TRUE;

	return hr;

ABORT:
	RETAILMSG(DEBUG_MEDIA,(_T("------CPanelMediaWrapper::PlayFile ABORT!!:%s---\r\n"),lpFileName));
	CleanUp();
	OnPlayerLoadFileAbort(__FUNCTION__,__LINE__);
	m_bEnableSmartBar = TRUE;

    return hr;
}
void CPanelMediaWrapper::TerminateRingtonePlayer()
{
	RETAILMSG(1,(_T("------CPanelMediaWrapper::TerminateRingtonePlayer---\r\n")));

	if(m_pRingToneAudio)
	{
		m_pRingToneAudio->Release();
		m_pRingToneAudio = NULL;
	}
	if(m_pRingToneEvent)
	{
		m_pRingToneEvent->Release();
		m_pRingToneEvent = NULL;
	}
	if(m_pRingTonePlayer)
	{
		m_pRingTonePlayer->Release();
		m_pRingTonePlayer = NULL;
	}
}
DWORD CPanelMediaWrapper::RingtoneThreadPro(LPVOID lpParam)
{
    CPanelMediaWrapper *panel=(CPanelMediaWrapper*)lpParam;
    RETAILMSG(1, (TEXT("MSG:[CPanelMediaWrapper::RingtoneThreadPro]: ++\r\n")));
    //Sleep(1500);

    if (panel->m_pRingTonePlayer)
    {
        if (panel->m_bRingTonePlaying)
        {
			panel->m_pRingToneAudio->SetVolume(panel->m_dwRingToneVol);
            panel->m_pRingTonePlayer->Play();
        }
    }
    else
    {
        if (!panel->m_bMediaModuleLoaded)
        {
            panel->WaitForMediaModuleReady();
        }

        if (!panel->m_pCSRMedia)
            return 0;

        //create player for ringtone:
        HRESULT hr;
        hr = panel->m_pCSRMedia->CreatePlayer(&panel->m_pRingTonePlayer);
        if (FAILED(hr))
            return 0;

        hr = panel->m_pRingTonePlayer->QueryInterface(IID_ICSREventSource, (void**)&panel->m_pRingToneEvent);
        if (FAILED(hr))
            return 0;

        hr = panel->m_pRingToneEvent->SetNotifyWindow(panel->m_hVideoWnd, WM_RINGTONE_MSG, 0);
        if (FAILED(hr))
            return 0;

        hr = panel->m_pRingTonePlayer->QueryInterface(IID_ICSRWaveoutAudio, (void**)&panel->m_pRingToneAudio);
        if (FAILED(hr))
            return 0;

        panel->m_pRingToneAudio->SetDeviceID(panel->m_dwRingToneWavID);   //ARM
		panel->m_pRingToneAudio->SetVolume(panel->m_dwRingToneVol);

        //hr = panel->m_pRingTonePlayer->OpenFile(_T("\\Windows\\Ring.wav"));
        //hr = panel->m_pRingTonePlayer->OpenFile(_T("\\ResidentFlash\\LightningUI\\Ring\\Ring.wav"));
		hr = panel->m_pRingTonePlayer->OpenFile(panel->m_strRingtoneFile.String());
        if (FAILED(hr))
            return 0;


        if (panel->m_bRingTonePlaying)
        {
            hr = panel->m_pRingTonePlayer->Play();
            if (FAILED(hr))
                return 0;
        }
    }
    RETAILMSG(1, (TEXT("MSG:[CPanelMediaWrapper::RingtoneThreadPro]: --\r\n")));

    return 0;
}
void CPanelMediaWrapper::StartRingTone(DWORD dwWavID,DWORD dwVol,BOOL bLoop)
{
    if (m_bRingTonePlaying)
        return;

	RETAILMSG(1,(_T("++++StartRingTone vol:0x%x, loop:%d++++\r\n"),dwVol,bLoop));

	m_bRingToneLoop = bLoop;
	m_bRingTonePlaying = TRUE;
	m_dwRingToneVol = (dwVol<<16) + dwVol;
	m_dwRingToneWavID = dwWavID;

    HANDLE hThread=CreateThread(NULL,NULL,RingtoneThreadPro,this,NULL,NULL);

    if (hThread != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hThread);
        hThread = INVALID_HANDLE_VALUE;
    }
}
void CPanelMediaWrapper::EndRingTone()
{
    if (!m_bRingTonePlaying)
        return;

    m_bRingTonePlaying = FALSE;

    if (m_pRingTonePlayer)
    {
        m_pRingTonePlayer->SetPosition(0);
        m_pRingTonePlayer->Pause();
    }
}
void CPanelMediaWrapper::TakeGstResource(BOOL bTake)
{
	if(m_bMediaPlayerTakeGst != bTake)
	{
		RETAILMSG(1,(_T("++++TakeGstResource :%d++++\r\n"),bTake));

		m_bMediaPlayerTakeGst = bTake;

		if(bTake)
		{
			ResetEvent(m_hGstReadyEvt);
			SetEventData(m_hGstReadyEvt,1);
		}
		else
		{
			SetEventData(m_hGstReadyEvt,0);
			SetEvent(m_hGstReadyEvt);
		}
	}
}
void CPanelMediaWrapper::TerminateVideoPlayback()
{
	//terminate current media playback:
	//if(m_category==MEDIA_VIDEO)
	if(!m_bCurPlayingAudioOnly && m_pPlayer)
	{
		RETAILMSG(1,(_T("++++TerminateVideoPlayback++++\r\n")));

		AbortLoadFile();
		WaitForLoadFileReady();
		RETAILMSG(DEBUG_MEDIA,(_T("++++CPanelMediaWrapper::carplay on, cleanup ++++\r\n")));
		Stop();
		CleanUp();
	}	

	//un-take gst resource
	TakeGstResource(FALSE);
}
void CPanelMediaWrapper::OnAutoResume()
{
	m_bResumeVideoPlayAfterCarplay = FALSE;
	SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_NONE);
}
void CPanelMediaWrapper::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:%d, %d ++++\r\n"),TEXT(__FUNCTION__),wParam,lParam));

    switch (wParam)
    {
        case CARPLAY_CONNECT_STATUS:
        {
			m_bCarPlayConnected = (lParam); // modified it same to FVW8010

			if(m_bCarPlayConnected)//connected
			{
				/*
				TerminateVideoPlayback();

				//change source type while current is bluetooth:
				if(m_idCurMediaType == MEDIA_TYPE_A2DP)
				{
					UINT idType = m_ppanelDevice->GetFirstAvailableDevice();
					if(idType)
					{
						SelectMediaType(idType,FALSE);
					}
				}
				*/

				/*
				//if current mode is video player UI,  then switch to Carplay UI automatically
				//without care about mode-change massage
				if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN)
					&& m_category==MEDIA_VIDEO)
				{
					SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE);
				}
				*/

// 				//disable video:
// 				EnableVideoPlayer(FALSE);
//
// 				//
// 				SetVideoColorKey(RGB(255,0,255));
            }
            else
            {
// 				//enable video:
// 				EnableVideoPlayer(TRUE);
//
// 				//
// 				SetVideoColorKey(RGB(8,8,8));
            }
        }
        break;
	case CARPLAY_SCREEN_STATUS:
		{

        }
        break;
     case CARPLAY_MODESCHANGED_STATUS:
		{
			#if CVTE_EN_CARPLAY
			const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
			UINT idUIClass = GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);
			UINT idSRCClass = GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);

			//RETAILMSG(1,(_T("++++modechange, idUIClass: %d, idSRCClass: %d++++\r\n"),idUIClass,idSRCClass));

			if(mode->screen == kAirPlayEntity_Controller)
			{
				TerminateVideoPlayback();
			}
			/*
			//phone or siri active:
			if(mode->phoneCall == kAirPlayEntity_Controller || mode->speech.entity == kAirPlayEntity_Controller)
			{
				//if current mode is video player,
				//you need terminate video player and un-take screen,
				//so carplay can borrow back screen
				if(m_category==MEDIA_VIDEO && idUIClass!=UI_CLASS_CARPLAY)
				{
					TerminateVideoPlayback();
					CarplayAPIUntakeScreen(NULL);
					CarplayAPIRequestUI("");

				}	
				if(idSRCClass == UI_CLASS_MEDIA && idUIClass == UI_CLASS_MEDIA)
				{
					m_bResumeVideoPlayAfterCarplay = TRUE;
				}
			}
			else
			{
				if(m_bResumeVideoPlayAfterCarplay)
				{
					SetTimer(m_hVideoWnd,TIMER_MEDIA_AUTO_RESUME,2000,NULL);
				}
			}
			*/
			#endif
		}
		break;
	 default:
		 break;
	}
}
void CPanelMediaWrapper::MuteAudio(BOOL bMute,BOOL bFade,UINT type,BOOL bAutoRelease,UINT time_out,BOOL bSync, BOOL bNoSendMuteCmd)
{
#if USE_WAV_ONCHIP
    if (/*!m_bSpeechOn && */!m_bPhoneCallActive)
#else
    if (!m_bSpeechOn && !m_bPhoneCallActive)
#endif
    {
        if (time_out<100)
            time_out=100;

        if (bSync)
            ::SendMessage(m_hMainWnd,WM_MUTE_AUDIO_CMD,(bNoSendMuteCmd<<28)+(bMute<<24)+(bFade<<16)+type,(bAutoRelease<<16)+(time_out));
        else
            ::PostMessage(m_hMainWnd,WM_MUTE_AUDIO_CMD,(bNoSendMuteCmd<<28)+(bMute<<24)+(bFade<<16)+type,(bAutoRelease<<16)+(time_out));
    }
#if !USE_WAV_ONCHIP
    else
    {
        //cancel manual mute
        if ((bMute && bAutoRelease) || !bMute)
        {
            ::PostMessage(m_hMainWnd,WM_MANUAL_MUTE,0+type,0);
        }
    }
#endif
}
void CPanelMediaWrapper::StartPlayback()
{
    RETAILMSG(DEBUG_MEDIA,(_T("[%x]>>>>>StartPlayback<<<<<\r\n"),GetCurrentThreadId()));

    int index=-1;
    DWORD dwPos=0;
    const wchar_t *pFileToPlay=NULL;
	m_bEnableAutoPlay = TRUE;

    if (m_ppanelLibrary)
    {
        if (m_idCurMediaType == MEDIA_TYPE_SD1)
        {
            if (m_category==MEDIA_VIDEO)
            {
                pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD1.String();
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosSD1;
            }
            else
            {
                pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileSD1.String();
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosSD1;
            }
            //notify SD SRC:
            ::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_SD1,m_category == MEDIA_VIDEO?0x20:0x11);
        }
#ifdef USE_GPS_CARD_MEDIA_PLAY
        else if (m_idCurMediaType == MEDIA_TYPE_SD2)
        {
            if (m_category==MEDIA_VIDEO)
            {
                pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD2.String();
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosSD2;
            }
            else
            {
                pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileSD2.String();
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosSD2;
            }
            //notify SD SRC:
            ::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_SD2,m_category == MEDIA_VIDEO?0x20:0x11);
        }
#endif
        else if (m_idCurMediaType == MEDIA_TYPE_USB1)
        {
            if (m_category==MEDIA_VIDEO)
            {
                pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB1.String();
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosUSB1;
            }
            else
            {
                pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileUSB1.String();
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosUSB1;
            }
            //notify SD SRC:
            ::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_USB1,m_category == MEDIA_VIDEO?0x20:0x11);
        }
        else if (m_idCurMediaType == MEDIA_TYPE_USB2)
        {
            if (m_category==MEDIA_VIDEO)
            {
                pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB2.String();
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosUSB2;
            }
            else
            {
		//当前播放的音乐名
		pFileToPlay=m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileUSB2.String();
		//在第几首
                index=m_ppanelLibrary->SearchByValue(pFileToPlay);
		//播放进度
                dwPos=m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosUSB2;
            }
            //notify SD SRC:
            //通知SRC当前多媒体设备，当前是播放音乐还是视频
            ::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_USB2,m_category == MEDIA_VIDEO?0x20:0x11);
        }

        //begin to play
        RETAILMSG(DEBUG_MEDIA,(_T(">>>>>Get MeidaType[%d] media file: %s, index:%d,dwPos:%d\r\n"),m_idCurMediaType,pFileToPlay,index,dwPos));
        if (index != -1)
        {
            SwitchFullscreen(0);

            //PlayIndex(index,dwPos*1000);
            //开始播放音乐
            PlayFile(pFileToPlay,dwPos*1000);
        }
        else
        {
            SwitchFullscreen(0);

            //PlayIndex(0);
            if (m_ppanelLibrary->GetTotalTrackCount()>0)
                PlayFile(m_ppanelLibrary->GetLibraryValue(0));
            else
                PlayFailed();
        }
    }
}
void CPanelMediaWrapper::AutoPlay()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::AutoPlay]\r\n")));

    PlayFile(NULL,0);

}
BOOL CPanelMediaWrapper::AutoPlayDevice()
{
    BOOL bRet=FALSE;
    UINT idMediaType=0;

	RETAILMSG(1,(_T("[%s] Start, ThreadId=0x%x\r\n"),_T(__FUNCTION__),GetCurrentThreadId()));

    if (m_ppanelDevice)
    {
        //
        if (!m_bEnableVideoPlayer && m_category==MEDIA_VIDEO)
        {
            m_category=MEDIA_AUDIO;
        }

        m_bAutoPlayAfterBrowsingFinish = TRUE;

        m_ppanelDevice->RefreshDeviceList();

        m_idCurMediaType = MEDIA_TYPE_INVALID;

        /*
        int index=m_ppanelDevice->SearchByID(m_pSkinManager->GetConfig()->GetCurParam()->iMediaType);
        if(-1 != index)
        {
        	idMediaType=m_ppanelDevice->GetIndexID(index);
        }
        else
        {
        	idMediaType=m_ppanelDevice->GetFirstAvailableDevice();
        }
        */

        UINT idTypeStored=m_pSkinManager->GetConfig()->GetCurParam()->iMediaType;

        if (idTypeStored
            && IsMassStorageMediaType(idTypeStored))
        {
			//check stored source is valid:
			BOOL bStoredSrcValid = FALSE;
			int iRtyCnt=0;
			while (!bStoredSrcValid && iRtyCnt<5 && !m_bAbortLoadFile)
			{
				//从m_boxDevice中找出合适的播放设备
				bStoredSrcValid = m_ppanelDevice->IsMediaDeviceAvailable(idTypeStored,0);
				if(!bStoredSrcValid)
				{
					if(WaitForSingleObject(m_hOpenFileEvent,1000) == WAIT_OBJECT_0 || (m_iFactoryTestType == FACTORY_CMD_MEDIA_TEST_VIDEO && m_iFactoryTestCmd == FACTORY_CMD_TEST_START))
					{
						break;
					}
				}
				iRtyCnt++;
			}

			if(bStoredSrcValid)
			{
				//确定播放设备
				RETAILMSG(1,(_T("[%s], bStoredSrcValid=1, iRtyCnt=%d\r\n"),_T(__FUNCTION__),iRtyCnt));
				idMediaType = idTypeStored;
			}
			else
			{
				idMediaType=m_ppanelDevice->GetFirstAvailableDevice();
				RETAILMSG(1,(_T("[%s], bStoredSrcValid=0, GetFirstAvailableDevice=%d\r\n"),_T(__FUNCTION__),idMediaType));
			}
        }
        else
        {
            idMediaType=m_ppanelDevice->GetFirstAvailableDevice();
        }

		//check abort
		if(m_bAbortLoadFile)
		{
			RETAILMSG(DEBUG_ERR,(_T("[%s], abort\r\n"),_T(__FUNCTION__)));
			return FALSE;
		}
		else
		{
			//sleep 500ms for device ready:
			Sleep(500);
		}

        //reset retry cnt:
        m_iAutoDetectDeviceRetryCnt = min(3,m_iAutoDetectDeviceRetryCnt);


        if (idMediaType)
        {
			RETAILMSG(1,(_T("[%s] SUCCEED, idMediaType=%d\r\n"),_T(__FUNCTION__),idMediaType));
            SelectMediaType(idMediaType);
            bRet = TRUE;
        }
#if 0
		else if (m_bA2DPAvailable)
		{
			SelectMediaType(MEDIA_TYPE_A2DP);
			bRet = TRUE;
		}
#endif
		//Factory mode
#if 1
        else if (m_pSkinManager->GetConfig()->GetCurParam()->bActiveFactory || m_iFactoryTestCmd == FACTORY_CMD_TEST_START)
        {
			RETAILMSG(1,(_T("[%s] begin to Factory test, idMediaType=%d\r\n"),_T(__FUNCTION__),MEDIA_TYPE_FLASH));
            SelectMediaType(MEDIA_TYPE_FLASH);
            bRet = TRUE;
        }
#endif
        else
        {
			RETAILMSG(1,(_T("[%s] FAILED!!!\r\n"),_T(__FUNCTION__)));

            //SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN);
            if (m_ppanelMain)
            {
                m_ppanelMain->SetMediaSrcType(MEDIA_TYPE_INVALID);
                m_ppanelMain->SetLoadingStatus(LOAD_STATUS_FAILED_NO_FILE);
            }

            EnablePlayUI(TRUE,__FUNCTION__);

            bRet = FALSE;
        }
    }

    return bRet;
}
void CPanelMediaWrapper::Play()
{
    if (/*!m_bSpeechOn &&*/ !m_bPhoneCallActive)
    {
        if (m_pPlayer)
        {
            m_bManualPause = FALSE;

            if (!m_bPlaying)
            {
                //fade volume:
                //MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,2000);
                MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,2000,TRUE);
#if USE_WAV_ONCHIP
                SetMediaAudioVolume(MEDIA_MAX_VOLMUE,/*FALSE*/TRUE);
#else
                //adjust media volume:
                if (m_bSpeechOn)
                {
                    DWORD dwGPS=m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS;
                    DWORD dwMedia=m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia;

                    if (dwGPS>dwMedia)
                    {
                        RETAILMSG(DEBUG_MEDIA,(_T("####### dwGPS>dwMedia\r\n")));
                        SetMediaAudioVolume(dwGPS>0?MEDIA_LOW_VOLMUE*dwMedia/dwGPS:MEDIA_LOW_VOLMUE,FALSE/*TRUE*/);
                    }
                    else
                    {
                        RETAILMSG(DEBUG_MEDIA,(_T("####### dwGPS<=dwMedia\r\n")));
                        SetMediaAudioVolume(dwMedia>0?MEDIA_LOW_VOLMUE*dwGPS/dwMedia:MEDIA_LOW_VOLMUE,FALSE/*TRUE*/);
                    }
                }
                else
                {
                    SetMediaAudioVolume(MEDIA_MAX_VOLMUE,/*FALSE*/TRUE);
                }
#endif
                if (FAILED(m_pPlayer->Play()))
                {
                    RETAILMSG(DEBUG_MEDIA,(_T("#######m_pPlayer->Play() FAILED!\r\n")));
                }
                else
                {
                    RETAILMSG(DEBUG_MEDIA,(_T("#######m_pPlayer->Play() SUCCEED!\r\n")));

                    //SetMediaVolume(m_bSpeechOn?MEDIA_LOW_VOLMUE:MEDIA_MAX_VOLMUE,TRUE);
		     //设置定时器0.5秒更新一次进度条，播放时间，剩余时间等
                    EnableMonitorPlayinfo(TRUE);
                }
            }
        }
    }
}
void CPanelMediaWrapper::PrepareStop()
{
    m_bManualStop = TRUE;

    MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000);

    //fade volume:
    //SetMediaVolume(0,TRUE);

    EnableMonitorPlayinfo(FALSE);

	SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL);

}
void CPanelMediaWrapper::Resume()
{
    m_bManualPause = FALSE;

    if (m_pPlayer && !m_bPlaying)
    {
        if (SUCCEEDED(m_pPlayer->Play()))
        {
            EnableMonitorPlayinfo(TRUE);
        }
    }
}
void CPanelMediaWrapper::Pause()
{
    m_bManualPause = TRUE;

    if (m_pPlayer && m_bPlaying)
    {
        m_pPlayer->Pause();
    }
    EnableMonitorPlayinfo(FALSE);
}

void CPanelMediaWrapper::PreparePlay()
{
	m_bPreparePlay = TRUE;
	m_bManualPause = FALSE;

	if(m_pPlayer && !m_bPlaying)
	{
		MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);
		//DuckAudio(TRUE,0,TRUE);
		SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL);
	}
}

void CPanelMediaWrapper::PreparePause(BOOL bDuck)	//modifiy to make it the same as FVW8010
{
    m_bManualPause = TRUE;
	m_bPreparePlay = FALSE;

    if (m_pPlayer && m_bPlaying)
    {
		if(m_bManualMute)
		{
	        if (m_pPlayer && m_bPlaying)
	        {
	            //发送给SCR暂停播放
	            m_pPlayer->Pause();
	            m_bPlaying = FALSE;
	        }

	        EnableMonitorPlayinfo(FALSE);
		}
		else
		{
			if(bDuck)
			{
	        	MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000);
			}
	        //fade volume:
	        //SetMediaVolume(0,TRUE);

	        //m_pPlayer->Pause();
		//关闭定时器
	        EnableMonitorPlayinfo(FALSE);

	        SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL);    //1500
		}

    }
}
void CPanelMediaWrapper::Stop()
{
    RETAILMSG(DEBUG_MEDIA,(_T("#####CPanelMediaWrapper::Stop\r\n")));

    if (m_bOpeningFile)
    {
        SetEvent(m_hOpenFileEvent);
    }

    SwitchFullscreen(0);
    ShowVideoWindow(FALSE);
	if(m_bPlaying)
	{
		Pause();
		Sleep(10);	//Modified to make it the same as FVW8010
	}

    m_bManualPause = TRUE;
    m_bHasMediaStream = FALSE;
    m_bPlaying = FALSE;
    m_bNeedSetIniPos = FALSE;

    m_bEnableQuit = TRUE;
    m_bNeedProcessCompleteNotify = FALSE;

    m_bLoadFileReady = FALSE;
    m_bPlayFileReady = FALSE;

    m_iAudioStreamCount = 0;
    m_iVideoStreamCount = 0;

    if (m_ppanelMain)
    {
        m_ppanelMain->SetProgressValue(0);
        m_ppanelMain->SetElapsedTime(0);
        m_ppanelMain->SetRemainTime(0);

        m_ppanelMain->SetPlayingStatus(FALSE);
        m_ppanelMain->SetNowPlayingTitle(_T("--/--"));

        m_ppanelMain->SetTitle(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
        m_ppanelMain->SetArtist(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
        m_ppanelMain->SetAlbum(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
        //m_ppanelMain->SetNowPlayingTitle(GetResourceString(IDS_UNKNOWN));
        m_ppanelMain->OnPrepareUpdateAlbumArt(NULL,0);
		//merge from FVW8010
		//whh 20160514
		m_ppanelMain->SetFileLoadFlag(FALSE);
    }
    if (m_ppanelLibrary)
    {
        m_ppanelLibrary->ShowProgressSlider(FALSE);
        m_ppanelLibrary->SetProgressValue(0);
        m_ppanelLibrary->SetPlayingStatus(FALSE);
    }
}
// const wchar_t* CPanelMediaWrapper::GetDeviceFriendlyName(LPCTSTR lpDeviceName)
// {
// 	if (lstrcmpi(lpDeviceName, DEV_NAME_NAND) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_RESIDENTFLASH);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_BT) == 0)//A2DP
// 	{
// 		return GetResourceString(IDS_DEVICE_BT);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_MEDIACARD) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_SD);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_GPSCARD) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_SD);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_USB1) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_USB);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_USB2) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_USB);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_IPOD) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_IPOD);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_LINEIN_FRONT) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_LINEIN_FRONT);
// 	}
// 	else
// 	{
// 		return NULL;
// 	}
// }
void CPanelMediaWrapper::PrepareSelectCategory(MEDIA_CATEGORY idCategory)
{
    if (m_category != idCategory)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG: [CPanelMediaWrapper::PrepareSelectCategory]: %d,m_bEnableVideoPlayer:%d\r\n"),idCategory,m_bEnableVideoPlayer));

        if (!m_bEnableVideoPlayer && idCategory==MEDIA_VIDEO)
        {
            return;
        }

	//更新UI界面
        EnablePlayUI(FALSE,__FUNCTION__);

        MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000);
        //SetMediaVolume(0,TRUE);

        m_bManualChangeCategory = TRUE;

        m_categoryToSet = idCategory;

        SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,/*1200*/600,NULL); //modifiy to make it the same as FVW8010

    }
    else if (m_category!=MEDIA_NULL)
    {
        if (IsMassStorageMediaType(m_idCurMediaType))
        {
            if (m_bHasMediaStream)
                Play();
            else
                StartPlayback();
        }
    }
}
void CPanelMediaWrapper::EnablePlayUI(BOOL bEnable,const char* iReason)
{
    RETAILMSG(1,(_T("MSG: [CPanelMediaWrapper::EnablePlayUI]: %d ,reason:%S\r\n"),bEnable,iReason));

    if (m_ppanelMain)
    {
        m_ppanelMain->EnableOperate(bEnable);
    }

    if (m_ppanelLibrary)
    {
        m_ppanelLibrary->EnableOperate(bEnable);
    }

    //widget:
    EnableSrcWidget(bEnable);

	//
	if(m_pSmartBarVol &&  GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_REARVIEW)
	{
		m_pSmartBarVol->EnableShow(bEnable);
	}
	if(m_pSmartBarMedia &&  GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_REARVIEW)
	{
		m_pSmartBarMedia->EnableShow(bEnable);
	}
}
void CPanelMediaWrapper::EnableSrcWidget(BOOL bEnable)
{
    BOOL benable=bEnable;
    m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_USB,0,UIOPRT_ENABLE,&benable);
    m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_SD,0,UIOPRT_ENABLE,&benable);
    m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_A2DP,0,UIOPRT_ENABLE,&benable);

}
void CPanelMediaWrapper::PrepareSelectMediaType(UINT idType)
{
// 	if ((MEDIA_TYPE_USB1 == idType) && (FALSE == m_bUsb1Inserted))
// 	{
// 		return;
// 	}
    if (m_idCurMediaType != idType)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG: [CPanelMediaWrapper::PrepareSelectMediaType]: %d\r\n"),idType));

        if (m_ppanelMain)
        {
            m_ppanelMain->SetLoadingStatus(LOAD_STATUS_LOADING);
        }
        if (m_ppanelLibrary)
        {
            m_ppanelLibrary->SetLoadingStatus(LOAD_STATUS_LOADING);
        }

        EnablePlayUI(FALSE,__FUNCTION__);

        MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000);
        //SetMediaVolume(0,TRUE);

        m_bManualChangeSrcType = TRUE;

        m_idSelectMediaType = idType;

        m_iAutoSelectCategory = 2;

        SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL);

    }
    else if (m_idCurMediaType!=MEDIA_TYPE_INVALID)
    {
        if (IsMassStorageMediaType(m_idCurMediaType))
        {
            if (m_bHasMediaStream)
                Play();
            else
                StartPlayback();
        }
    }
}
void CPanelMediaWrapper::SelectMediaType(UINT idType,BOOL bAutoPlay)
{

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::SelectMediaType]: %d,current:%d\r\n"),idType,m_idCurMediaType));

    if (m_idCurMediaType != idType || idType == MEDIA_TYPE_A2DP || idType == MEDIA_TYPE_IPOD)
    {

        m_bAutoPlayAfterBrowsingFinish = bAutoPlay;

        //enable UI:
        if (!IsMassStorageMediaType(idType))
        {
            EnablePlayUI(TRUE,__FUNCTION__);
        }


        if (m_idCurMediaType == MEDIA_TYPE_A2DP && idType != MEDIA_TYPE_A2DP)
        {
            //pause a2dp:
            SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PAUSE);
        }

		//20160924
		if(idType == MEDIA_TYPE_FLASH)
		{
			m_idCurMediaType = MEDIA_TYPE_USB1;
		}
		else
		{
			m_idCurMediaType = idType;
		}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI || CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		//update widget status:
		if(m_pSkinManager)
		{
			BOOL bFocus=1;
			BOOL bNoFocus=0;

			if (m_idCurMediaType == MEDIA_TYPE_A2DP)
			{
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_USB,0,UIOPRT_SETSTATUS,&bNoFocus);
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_A2DP,0,UIOPRT_SETSTATUS,&bFocus);
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_AUX,0,UIOPRT_SETSTATUS,&bNoFocus);
			}
			else if (m_idCurMediaType == MEDIA_TYPE_USB1 || m_idCurMediaType == MEDIA_TYPE_USB2)
			{
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_USB,0,UIOPRT_SETSTATUS,&bFocus);
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_A2DP,0,UIOPRT_SETSTATUS,&bNoFocus);
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_AUX,0,UIOPRT_SETSTATUS,&bNoFocus);
			}
		}
#endif
	
        if (idType == MEDIA_TYPE_A2DP)//A2DP
        {
            //test
			SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_RIGHT,-1);
			//do not send A2DP_CONTROL_PLAY, because a2dp will auto resume after panel entered
			//SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
			SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP));
			//BUG whh 媒体库从USB切换到BT，声音可能出不来的问题
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
		}
		else if (idType == MEDIA_TYPE_IPOD)//IPOD
		{
			SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_RIGHT);
		}
		else if (idType == MEDIA_TYPE_AUX)//
		{
		}
		else if (idType == MEDIA_TYPE_DISK)//
		{
		}
		//切换源
		else if(idType == MEDIA_TYPE_USB1 || idType == MEDIA_TYPE_USB2 || idType == MEDIA_TYPE_SD1)//other storage device
		{
			//SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);

            //
            // 			if(m_pSkinManager)
            // 			{
            // 				m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC = MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
            // 			}
            SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN));
        }
        //	        else if (idType == MEDIA_TYPE_NAND)
        //	        {
        //				SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN));
        //	        }

        //if(m_bHasMediaStream)
        //if(IsMassStorageMediaType(idType))
        //clear playing info:
        {
            if (m_bOpeningFile)
            {
                SetEvent(m_hOpenFileEvent);
            }

            SwitchFullscreen(0);
            ShowVideoWindow(FALSE);
            Pause();

            m_bManualPause = TRUE;
            m_bHasMediaStream = FALSE;
            m_bPlaying = FALSE;
            m_bNeedSetIniPos = FALSE;

            m_bEnableQuit = TRUE;
            m_bNeedProcessCompleteNotify = FALSE;

            m_bLoadFileReady = FALSE;
            m_bPlayFileReady = FALSE;

            m_iAudioStreamCount = 0;
            m_iVideoStreamCount = 0;

            if (m_ppanelMain)
            {
                m_ppanelMain->OnPrepareUpdateAlbumArt(NULL,0);
                m_ppanelMain->UpdateAlbumArt();
            }
        }

        if (m_ppanelLibrary)
            m_ppanelLibrary->SetMediaSrcType(idType);

        if (m_ppanelMain)
            m_ppanelMain->SetMediaSrcType(idType);
    }
}
void CPanelMediaWrapper::OnDeviceChanged()
{
    if (m_ppanelDevice)
    {
        m_ppanelDevice->RefreshDeviceList();
    }
}
void CPanelMediaWrapper::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
    if (!lpDeviceName)
        return;

#if CVTE_EN_ACCON_WARNING
	if (CWarningProxy::GetInstance()->IsShowWarningSmartBar() || CWarningProxy::GetInstance()->IsShowWarningPage())
	{
		return;
	}
#endif

    RETAILMSG(DEBUG_MEDIA,(_T("MSG: [CPanelMediaWrapper::OnDeviceArrivedReady]: %s\r\n"),lpDeviceName));

    //need FIXED!  place this block into a thread:

    // 	Sleep(3000);

    // 	SwitchToPage(PAGE_MEDIA_DEVICE);

    //test:
    if (lstrcmpi(_T("DSK9:"), lpDeviceName) == 0) //Media Card
    {
        if (m_ppanelDevice)
            m_ppanelDevice->RefreshDeviceList();
        PrepareSelectMediaType(MEDIA_TYPE_SD1);
        SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
    }
#ifdef USE_GPS_CARD_MEDIA_PLAY
    else if (lstrcmpi(_T("DSK8:"), lpDeviceName) == 0) //GPS card
    {
        if (m_ppanelDevice)
            m_ppanelDevice->RefreshDeviceList();
        PrepareSelectMediaType(MEDIA_TYPE_SD2);
        SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
    }
#endif
    else if (lstrcmpi(_T("DSK2:"), lpDeviceName) == 0) //USB disk
    {
        if (m_ppanelDevice)
            m_ppanelDevice->RefreshDeviceList();
		//m_bUsb1Inserted = TRUE;
        PrepareSelectMediaType(MEDIA_TYPE_USB1);
        SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
    }
    else if (lstrcmpi(_T("DSK3:"), lpDeviceName) == 0) //USB disk2
    {
        if (m_ppanelDevice)
            m_ppanelDevice->RefreshDeviceList();
        PrepareSelectMediaType(MEDIA_TYPE_USB2);
        SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
    }
}
void CPanelMediaWrapper::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnDeviceRemovedReady]: %s\r\n"),lpDeviceName));

    if (!lpDeviceName)
        return;

    //notify:
    if (lstrcmpi(_T("DSK2:"), lpDeviceName) == 0)
	{
		//m_bUsb1Inserted = FALSE;
        ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_USB1,0);
	}
	//更换设备源
    else if (lstrcmpi(_T("DSK3:"), lpDeviceName) == 0)
        ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_USB2,0);
    else if (lstrcmpi(_T("DSK9:"), lpDeviceName) == 0)
        ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_SD1,0);
#ifdef USE_GPS_CARD_MEDIA_PLAY
    else if (lstrcmpi(_T("DSK8:"), lpDeviceName) == 0)
        ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_SD2,0);
#endif
    if ( (m_idCurMediaType==MEDIA_TYPE_USB1 && lstrcmpi(_T("DSK2:"), lpDeviceName) == 0) ||
         (m_idCurMediaType==MEDIA_TYPE_USB2 && lstrcmpi(_T("DSK3:"), lpDeviceName) == 0 ) ||
#ifdef USE_GPS_CARD_MEDIA_PLAY
         (m_idCurMediaType==MEDIA_TYPE_SD2 && lstrcmpi(_T("DSK8:"), lpDeviceName) == 0 ) ||
#endif
         (m_idCurMediaType==MEDIA_TYPE_SD1 && lstrcmpi(_T("DSK9:"), lpDeviceName) == 0 )   )
    {

        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::OnDeviceRemovedReady]: reset current device\r\n")));

        // 			m_strCurrentDevice = NULL;

        // 			if(m_bHasMediaStream)
        {
            Stop();
        }

        if (m_ppanelLibrary)
        {
            if (m_ppanelLibrary->IsRefreshing())
            {
                m_ppanelLibrary->AbortRefresh();
            }

            //
            m_ppanelLibrary->ClearList();
        }

        // 			m_bBeginPlayFile = FALSE;
        // 			m_bBeginPlayIndex = FALSE;

        m_idCurMediaType = MEDIA_TYPE_INVALID;

        m_dwDeviceRemoveTick = GetTickCount();

        //
        if (m_idSystemPowerState == SYS_POWER_ON)
        {
            if (GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
            {
                if (m_bAutoSelectDevice)
                {
                    KillTimer(m_hVideoWnd,TIMER_MEDIA_AUTO_SELECT_DEVICE);
                }
		//重新自动选择设备
                SetTimer(m_hVideoWnd,TIMER_MEDIA_AUTO_SELECT_DEVICE,2500,NULL);
                m_bAutoSelectDevice = TRUE;
            }
        }
    }
}
void CPanelMediaWrapper::LinkSmartBarMedia(CSmartBarMedia *pSmartBar)
{
    m_pSmartBarMedia = pSmartBar;
    if (m_ppanelMain)
    {
        m_ppanelMain->LinkSmartBarMedia(pSmartBar);
    }
}
BOOL CPanelMediaWrapper::AutoSelectDevice()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::AutoSelectDevice]: Auto select device\r\n")));

    BOOL bRet=FALSE;

    m_bAutoSelectDevice = FALSE;

    //auto change to other device:
    if (m_ppanelDevice)
    {
    	//刷新设备列表
        m_ppanelDevice->RefreshDeviceList();
	//重新获取有效设备
        UINT idMediaType=m_ppanelDevice->GetFirstAvailableDevice();
        if (idMediaType)
        {
            SelectMediaType(idMediaType);
            bRet=TRUE;
        }
#if 0
		else if (m_bA2DPAvailable)
		{
			SelectMediaType(MEDIA_TYPE_A2DP);
			bRet=TRUE;
		}
#endif
        else
        {
            bRet=FALSE;
            // 			if(m_ppanelLibrary)
            // 				m_ppanelLibrary->ClearList();

            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::AutoSelectDevice]: Auto select device FAILED!!! no available device\r\n")));
            //SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN);
            if (m_ppanelMain)
                m_ppanelMain->SetMediaSrcType(MEDIA_TYPE_INVALID);

            EnablePlayUI(TRUE,__FUNCTION__);
        }
    }

    return bRet;
}
void CPanelMediaWrapper::SetMediaAudioVolume(INT iVolume,BOOL bFade)
{
    //RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::SetMediaAudioVolume]: Do nothing\r\n")));

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::SetMediaAudioVolume]: %x,bFade:%d\r\n"),iVolume,bFade));
    //if(m_pAudio)
    {
        if (iVolume<0)
            iVolume=0;
        if (iVolume>0xFFFF)
            iVolume=0xFFFF;

        m_iVolumeSet = iVolume;

        bFade = FALSE;  //cancel fade 20151205

        if (!bFade)
        {
            m_iVolumeCur=m_iVolumeSet;

            DWORD dwVol=(m_iVolumeCur<<16)+m_iVolumeCur;

            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::SetMediaAudioVolume]: 1 %x,bFade:%d\r\n"),dwVol,bFade));

            if (m_pAudio)
            {
                m_pAudio->SetVolume(dwVol);
            }
            m_bVolumeFadeReady=TRUE;
        }
        else
        {
            if (m_bVolumeFadeReady)
            {
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::SetMediaAudioVolume]: 2 %x,bFade:%d\r\n"),m_iVolumeSet,bFade));
                m_bVolumeFadeReady=FALSE;
                // 				HANDLE handle=CreateThread(NULL,NULL,AutoVolumeThreadPro,this,NULL,NULL);
                // 				if(handle != INVALID_HANDLE_VALUE)
                // 				{
                // 					CloseHandle(handle);
                // 				}
                SetTimer(m_hVideoWnd,TIMER_MEDIA_VOLUME_FADE,VOLUME_FADE_STEP,NULL);
            }
        }
    }

}


void CPanelMediaWrapper::ShowVideoWindow(BOOL bShow)
{
    // 	if( (bShow && IsWindowVisible(m_hVideoWnd)) || (!bShow && !IsWindowVisible(m_hVideoWnd)) )
    // 		return;

    if (m_hVideoWnd /*&& m_pVideo*/)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaWrapper::ShowVideoWindow]: %d\r\n"),bShow));

        m_bVideoWndVisible = bShow;

        if (bShow)
        {
            if (!m_bCurPlayingAudioOnly)
            {
                ::SetWindowPos(m_hVideoWnd, HWND_TOPMOST , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_SHOWWINDOW) );
            }
        }
        else
        {
            ::SetWindowPos(m_hVideoWnd, HWND_BOTTOM , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_HIDEWINDOW) );
        }

        if (m_pVideo)
        {
            m_pVideo->UpdateVideo();
        }
    }
}
void CPanelMediaWrapper::SetPlayMode(UINT idMode)
{
    m_idPlayMode=idMode;

    if (m_idPlayMode>2)
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

    if (m_ppanelMain)
        m_ppanelMain->SetPlayMode(m_idPlayMode);

    m_pSkinManager->GetConfig()->GetCurParam()->idMediaPlayMode = m_idPlayMode;
}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
void CPanelMediaWrapper::SwitchPlayMode(UINT mode)
{
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:*** MEDIA_CONTROL_PLAYMODE: %d\r\n"),TEXT(__FUNCTION__), mode));

	int play = 0;

	switch (mode)
	{
		case MEDIA_CONTROL_PLAYMODE_REPEAT_ALL:
			{
				m_bRepeat  = FALSE;
				m_bShuffle = FALSE;
				play = 0; 
			}
			break;
		case MEDIA_CONTROL_PLAYMODE_REPEAT_ONE:
			{
				m_bRepeat = TRUE;
				m_bShuffle= FALSE;
				play = 1;
			}
			break;
		case MEDIA_CONTROL_PLAYMODE_SHUFFLE:
			{
				m_bRepeat = FALSE;
				m_bShuffle=TRUE;
				play = 2;
			}
			break;
		default:
			break;
	}

	if (m_ppanelMain)
		m_ppanelMain->SetPlayMode(play);

	 m_pSkinManager->GetConfig()->GetCurParam()->idMediaPlayMode = play;
}
#endif
void CPanelMediaWrapper::SwitchPlayMode()
{
    m_idPlayMode++;

    if (m_idPlayMode>2)
        m_idPlayMode=0;

	//m_bShuffle在播放下一曲的时候会用到，根据m_bShuffle判断是调用随机播放还是下一曲播放
	//m_bRepeat在播放结束开始下一曲的时候会用到，如果是循环则继续播
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

	//设置按钮UI切换
    if (m_ppanelMain)
        m_ppanelMain->SetPlayMode(m_idPlayMode);

	//设置播放模式
    m_pSkinManager->GetConfig()->GetCurParam()->idMediaPlayMode = m_idPlayMode;

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

    if (m_bRepeat)
        m_bShuffle=FALSE;

    if (m_ppanelMain)
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

    if (m_bShuffle)
        m_bRepeat=FALSE;

    if (m_ppanelMain)
    {
        m_ppanelMain->SetRepeatCheck(m_bRepeat);
        m_ppanelMain->SetShuffleCheck(m_bShuffle);
    }
}
void CPanelMediaWrapper::SwitchFullscreen(UINT idMode)
{
    if (/*m_pVideo &&*/ !m_bCurPlayingAudioOnly)
    {
        if (m_pSmartBarVol && m_pSmartBarVol->IsVisible() )
        {
            m_pSmartBarVol->Show(FALSE);
        }

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
                if (m_bFullScreenMode)
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

        if (m_ppanelMain)
        {
            m_ppanelMain->SetVideoFullScreen(m_bFullScreenMode);
        }

        if (m_pVideo)
        {
            m_pVideo->UpdateVideo();
        }
    }
}
void CPanelMediaWrapper::EnableMonitorPlayinfo(BOOL bEnable)
{
    if (!m_bMonitorPlayinfo && bEnable)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: TRUE\r\n"),TEXT(__FUNCTION__)));

        m_bMonitorPlayinfo = TRUE;
	//设置定时器0.5秒更新一次进度条，播放时间，剩余时间等
        SetTimer(m_hVideoWnd,TIMER_UPDATE_PLAYINFO,PLAYER_PLAYPOS_UPDATE_TIME,NULL);
    }
    else if (m_bMonitorPlayinfo && !bEnable)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: FALSE\r\n"),TEXT(__FUNCTION__)));

        m_bMonitorPlayinfo = FALSE;

        KillTimer(m_hVideoWnd,TIMER_UPDATE_PLAYINFO);
    }
}
void	CPanelMediaWrapper::CleanUp()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: +\r\n"),TEXT(__FUNCTION__)));

    //WaitForLoadFileReady();

    EnableMonitorPlayinfo(FALSE);

    if (m_pPlayer && m_bPlaying)
    {
        m_pPlayer->Pause();
		Sleep(10); //Modified to make it the same as FVW8010
    }

    m_bHasMediaStream = FALSE;
    m_bPlaying = FALSE;
    m_bNeedSetIniPos = FALSE;

    m_bEnableQuit = TRUE;
    m_bNeedProcessCompleteNotify = FALSE;

    m_iAudioStreamCount = 0;
    m_iVideoStreamCount = 0;

    //release objects:
    if (m_pMediaInfo)
    {
        m_pMediaInfo->Release();
        m_pMediaInfo=NULL;
    }

    if (m_pInspector)
    {
        m_pInspector->Release();
        m_pInspector=NULL;
    }

    if (m_pStreamInfo)
    {
        m_pStreamInfo->Release();
        m_pStreamInfo=NULL;
    }

    if (m_pAudio)
    {
        m_pAudio->Release();
        m_pAudio=NULL;
    }

    if (m_pVideo)
    {
        m_pVideo->Release();
        m_pVideo=NULL;
    }

    if (m_pEvent)
    {
        m_pEvent->Release();
        m_pEvent=NULL;
    }

    if (m_pPlayer)
    {
        m_pPlayer->Release();
        m_pPlayer=NULL;
    }

	Sleep(10);

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: -\r\n"),TEXT(__FUNCTION__)));
}
void CPanelMediaWrapper::OnSpeechStart()
{
//    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));

    m_bSpeechOn = TRUE;
#if !USE_WAV_ONCHIP
    // 	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
    {
        if (m_bPlaying)
        {
            DWORD dwGPS=m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS;
            DWORD dwMedia=m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia;

            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: dwGPS:%d, dwMedia:%d \r\n"),TEXT(__FUNCTION__),dwGPS,dwMedia));

            if (dwMedia)
            {
                if (dwGPS>dwMedia)
                    SetMediaAudioVolume(dwGPS>0?MEDIA_LOW_VOLMUE*dwMedia/dwGPS:MEDIA_LOW_VOLMUE,FALSE/*TRUE*/);
                else
                    SetMediaAudioVolume(dwMedia>0?MEDIA_LOW_VOLMUE*dwGPS/dwMedia:MEDIA_LOW_VOLMUE,FALSE/*TRUE*/);
            }
            else
            {
                SetMediaAudioVolume(0,FALSE);
            }
        }
    }
#endif
}
void CPanelMediaWrapper::OnSpeechTerminate()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: CPanelMediaWrapper OnSpeechTerminate\r\n"),TEXT(__FUNCTION__)));

    m_bSpeechOn = FALSE;
#if !USE_WAV_ONCHIP
    // 	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
    {
        if (m_bPlaying)
        {
            if (m_bHasMediaStream && !m_bPhoneCallActive)
            {
                SetMediaAudioVolume(MEDIA_MAX_VOLMUE,TRUE);
            }
        }
    }
#endif
}

void CPanelMediaWrapper::OnSRStart()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: m_bPlaying:%d\r\n"),TEXT(__FUNCTION__),m_bPlaying));

    m_bSROn = TRUE;
    m_bPlayingBeforeSpeech = m_bPlaying;

#if USE_WAV_ONCHIP
    {
        if (m_bHasMediaStream)
        {
            if (m_bPlaying)
            {
                PreparePause();
            }
        }
    }
#endif
}
void CPanelMediaWrapper::OnSRTerminate()
{
    RETAILMSG(DEBUG_MEDIA,(_T("............CPanelMediaWrapper OnSRTerminate......................m_bPlaying:%d\r\n"),m_bPlaying));

    m_bSROn = FALSE;
#if USE_WAV_ONCHIP
    {
        if (m_bHasMediaStream)
        {
            if (!m_bPlaying && m_bPlayingBeforeSpeech)
            {
                Play();
            }
        }
    }
#endif
}
void CPanelMediaWrapper::OnPlaySoundEffect(BOOL bPlay)
{
    if (GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
    {
        if (m_bPlaying)
        {
            SetMediaAudioVolume(bPlay?0:MEDIA_MAX_VOLMUE,FALSE);
        }
    }
}
void CPanelMediaWrapper::OnMediaBrowsingFinish(BOOL bCancel)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: mediatype[%d],bCancel:%d ,m_category:%d\r\n"),TEXT(__FUNCTION__),m_idCurMediaType,bCancel,m_category));

    m_bBrowsing=FALSE;

    if (m_ppanelMain)
    {
    	//设置多媒体主页控件状态
        m_ppanelMain->SetBrowsingStatus(FALSE);
    }

    if (m_ppanelLibrary)
    {
        if (m_ppanelLibrary->GetTotalTrackCount() == 0)
        {
            EnablePlayUI(TRUE,__FUNCTION__);

            if (m_iAutoSelectCategory > 0)
            {
                m_iAutoSelectCategory--;

                if (!bCancel)
                {
                    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: auto select catalog:%d,cnt:%d\r\n"),TEXT(__FUNCTION__),m_category,m_iAutoSelectCategory));
                    SendMediaCommand(MEDIA_CMD_CATEGORY,(m_category==MEDIA_AUDIO)?MEDIA_VIDEO:MEDIA_AUDIO);
                    return;
                }
            }
        }
    }

    if (bCancel)
    {
        return;
    }

    if (!m_bAutoPlayAfterBrowsingFinish)
    {
        EnablePlayUI(TRUE,__FUNCTION__);

        return;
    }

    // 	if(m_bHasMediaStream)
    // 		return;
    //开始播放音乐
    StartPlayback();
}
void CPanelMediaWrapper::OnMediaBrowsingBegin()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));

    if (m_ppanelMain)
        m_ppanelMain->SetBrowsingStatus(TRUE);

    m_bBrowsing=TRUE;

}
void CPanelMediaWrapper::SetPlayPosition(DWORD dwNum,DWORD dwDen)
{
    if (m_pPlayer)
    {
        DWORD dwDuration=m_pPlayer->GetDuration();

        if (dwDuration != CM_INVALID_TIME)
        {
            // 			m_pPlayer->SetPosition(wPara*m_pPlayer->GetDuration()/512);
            //			BOOL bNeedMute = m_pCommander? !m_pCommander->GetItemData(MCU_MUTE_MODE) : FALSE;

            //if(bNeedMute)

            if (m_bPlaying)
            {
                MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);	//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000,TRUE);
                //SetMediaVolume(0,TRUE);
                m_bManualSetPosition = TRUE;
                m_dwManualSetPosition = dwNum*m_pPlayer->GetDuration()/dwDen;
		//如果正在播放，先关掉歌曲信息定时器
                EnableMonitorPlayinfo(FALSE);
                //重新更新歌曲信息，重新打开定时器
				SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL); //Modified to make it the same as FVW8010
            }
            else
            {
                //SetMediaVolume(0,FALSE);
                if (FAILED(m_pPlayer->SetPosition(dwNum*m_pPlayer->GetDuration()/dwDen)))
                {
                    RETAILMSG(DEBUG_MEDIA,(_T("ERR:[%s]: m_pPlayer->SetPosition FAILED!\r\n"),TEXT(__FUNCTION__)));
                }
                else
                {
                    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: m_pPlayer->SetPosition SUCCEED!\r\n"),TEXT(__FUNCTION__)));
                }
            }
        }
        else
        {
            if (m_ppanelMain)
            {
                m_ppanelMain->SetProgressValue(0);
            }
            if (m_ppanelLibrary)
            {
                m_ppanelLibrary->ShowProgressSlider(FALSE);
                m_ppanelLibrary->SetProgressValue(0);
            }
        }
    }
}

void CPanelMediaWrapper::SetFF(DWORD dwNum, BOOL bDown)
{
	if (m_pPlayer)
	{	
		static DWORD dwDuration;
		static DWORD dwSetPos = 0;	
		static DWORD dwProgressPos;
		static BOOL  bSetFF;

		if (dwSetPos == 0)
			dwSetPos = m_pPlayer->GetPosition();

		if (bDown)
		{
			bSetFF = TRUE;
			dwDuration = m_pPlayer->GetDuration();
			dwSetPos = min(dwSetPos+dwNum, dwDuration);
			dwProgressPos = dwSetPos*MEDIA_SILDER_LENGTH/dwDuration;

			RETAILMSG(1,(_T("MSG:[%s]: dwNum:%d GetPosition:%d GetDuration:%d dwProgressPos[%d]\r\n"),TEXT(__FUNCTION__),dwNum, m_pPlayer->GetPosition(), dwDuration, dwProgressPos));
			EnableMonitorPlayinfo(FALSE);
			if (m_ppanelMain)
			{
				m_ppanelMain->SetProgressValue(dwProgressPos);
			}
			if (m_ppanelLibrary)
			{
				m_ppanelLibrary->SetProgressValue(dwProgressPos);
				m_ppanelLibrary->ShowProgressSlider(TRUE);
			}
			
		}
		else
		{
			if (bSetFF)
			{
				if (dwDuration != CM_INVALID_TIME)
				{
					if (m_bPlaying)
					{
						MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);	//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000,TRUE);
						//SetMediaVolume(0,TRUE);
						m_bManualSetPosition = TRUE;
						m_dwManualSetPosition = dwSetPos;
						EnableMonitorPlayinfo(FALSE);
						//
						SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL); //Modified to make it the same as FVW8010
					}
					else
					{
						if (FAILED(m_pPlayer->SetPosition(dwSetPos)))
						{
							RETAILMSG(1,(_T("ERR:[%s]: m_pPlayer->SetPosition FAILED!\r\n"),TEXT(__FUNCTION__)));
						}
						else
						{
							RETAILMSG(1,(_T("MSG:[%s]: m_pPlayer->SetPosition SUCCEED!\r\n"),TEXT(__FUNCTION__)));
						}
					}
					RETAILMSG(1,(_T("MSG:[%s]: m_dwManualSetPosition[%d]\r\n"),TEXT(__FUNCTION__),m_dwManualSetPosition));
				}
				else
				{
					if (m_ppanelMain)
					{
						m_ppanelMain->SetProgressValue(0);
					}
					if (m_ppanelLibrary)
					{
						m_ppanelLibrary->ShowProgressSlider(FALSE);
						m_ppanelLibrary->SetProgressValue(0);
					}
				}

			}

			//Clear
			bSetFF = FALSE;
			dwSetPos = 0;
			dwDuration = CM_INVALID_TIME;
		}
	}
}


void CPanelMediaWrapper::SetFB(DWORD dwNum, BOOL bDown)
{
	if (m_pPlayer)
	{
		static DWORD dwDuration;
		static DWORD dwSetPos = 0;	
		static DWORD dwProgressPos;
		static BOOL  bSetFB = FALSE;

		if (dwSetPos == 0)
			dwSetPos = m_pPlayer->GetPosition();

		if (bDown)
		{
			bSetFB = TRUE;
			dwDuration = m_pPlayer->GetDuration();
			
			if (dwSetPos > dwNum)
				dwSetPos = dwSetPos-dwNum;
			else
				dwSetPos = 1;

			dwProgressPos = dwSetPos*MEDIA_SILDER_LENGTH/dwDuration;

			RETAILMSG(1,(_T("MSG:[%s]: dwNum:%d dwSetPos:%d dwDuration:%d dwProgressPos[%d]\r\n"),TEXT(__FUNCTION__),dwNum, dwSetPos, dwDuration, dwProgressPos));
			
			EnableMonitorPlayinfo(FALSE);

			if (m_ppanelMain)
			{
				m_ppanelMain->SetProgressValue(dwProgressPos);
			}
			if (m_ppanelLibrary)
			{
				m_ppanelLibrary->SetProgressValue(dwProgressPos);
				m_ppanelLibrary->ShowProgressSlider(TRUE);
			}			
		}
		else
		{
			if (bSetFB)
			{
				if (dwDuration != CM_INVALID_TIME)
				{
					if (m_bPlaying)
					{
						RETAILMSG(1,(_T("MSG:[%s]: dwSetPos[%d]\r\n"),TEXT(__FUNCTION__),dwSetPos));
						MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);	//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000,TRUE);
						//SetMediaVolume(0,TRUE);
						m_bManualSetPosition = TRUE;
						m_dwManualSetPosition = dwSetPos;//dwSetPos*dwDuration/512;
						EnableMonitorPlayinfo(FALSE);
						//
						SetTimer(m_hVideoWnd,TIMER_MEDIA_DELAY_OPERATION,600,NULL); //Modified to make it the same as FVW8010
					}
					else
					{
						if (FAILED(m_pPlayer->SetPosition(dwSetPos)))
						{
							RETAILMSG(1,(_T("ERR:[%s]: m_pPlayer->SetPosition FAILED!\r\n"),TEXT(__FUNCTION__)));
						}
						else
						{
							RETAILMSG(1,(_T("MSG:[%s]: m_pPlayer->SetPosition SUCCEED!\r\n"),TEXT(__FUNCTION__)));
						}
					}

					RETAILMSG(1,(_T("MSG:[%s]: m_dwManualSetPosition[%d]\r\n"),TEXT(__FUNCTION__),m_dwManualSetPosition));
				}
				else
				{
					if (m_ppanelMain)
					{
						m_ppanelMain->SetProgressValue(0);
					}
					if (m_ppanelLibrary)
					{
						m_ppanelLibrary->ShowProgressSlider(FALSE);
						m_ppanelLibrary->SetProgressValue(0);
					}
				}

			}

			//Clear
			bSetFB = FALSE;
			dwSetPos = 0;
			dwProgressPos = 0;
			dwDuration = CM_INVALID_TIME;
		}
	}
}


void CPanelMediaWrapper::SelectCategory(MEDIA_CATEGORY idCategory,BOOL bAutoPlay)
{
    if (!m_bEnableVideoPlayer && idCategory==MEDIA_VIDEO)
    {
        EnablePlayUI(TRUE,__FUNCTION__);
        return;
    }

    m_bAutoPlayAfterBrowsingFinish = bAutoPlay;
    m_category = idCategory;
    Stop();
    if (m_ppanelLibrary)
    {
        m_ppanelLibrary->SetMediaCategory(m_category);
    }
    if (m_ppanelMain)
    {
        m_ppanelMain->SetVideoMode(m_category==MEDIA_VIDEO);
    }
	if(m_category==MEDIA_VIDEO)
	{
		TakeGstResource(TRUE);
	}

#if 0
#if CVTE_EN_CARPLAY
	if(m_bCarPlayConnected)
	{
		if(m_category==MEDIA_VIDEO)
		{
			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("++++CPanelMediaWrapper::SelectCategory video mode, take screen forver ++++\r\n")));
			CarplayAPITakeScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_Never,kAirPlayConstraint_Never,NULL);
		}
		else
		{
			RETAILMSG(1/*DEBUG_MEDIA*/,(_T("++++CPanelMediaWrapper::SelectCategory music mode, take screen UserInitiated ++++\r\n")));
			CarplayAPITakeScreen(kAirPlayTransferPriority_UserInitiated,kAirPlayConstraint_UserInitiated,kAirPlayConstraint_Anytime,NULL);
		}
	}
#endif
#endif

}
void CPanelMediaWrapper::OnRingToneCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bRing=HIWORD(wParam);
	BOOL bLoop=LOWORD(wParam);
	DWORD dwVol=LOWORD(lParam);
	DWORD dwWavID=HIWORD(lParam);

	RETAILMSG(1,(_T("MSG:[%s]: bRing:%d, bLoop:%d, dwVol:%d, dwWavID:%d\r\n"),TEXT(__FUNCTION__),bRing,bLoop,dwVol,dwWavID));

	if(bRing)
	{
		StartRingTone(dwWavID,dwVol,bLoop);
	}
	else
	{
		EndRingTone();
	}
}

void CPanelMediaWrapper::EnableVideoPlayer(BOOL bEnable)
{
    if (m_bEnableVideoPlayer != bEnable)
    {
        m_bEnableVideoPlayer = bEnable;

        if (m_ppanelLibrary)
        {
            m_ppanelLibrary->EnableVideo(bEnable);
        }

        if (!m_bEnableVideoPlayer && m_category == MEDIA_VIDEO)
        {
            SelectCategory(MEDIA_AUDIO,FALSE);
        }
    }
}
void CPanelMediaWrapper::OnMediaPlayerCommand(WPARAM wParam, LPARAM lParam)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: wParam:%d,lParam:%d\r\n"),TEXT(__FUNCTION__),wParam,lParam));


    switch (wParam)
    {
        case MEDIA_CMD_LOAD_FILE:
        {
			PlayFile((const wchar_t*)lParam);
			m_bEnableAutoPlay = TRUE;
        }
        break;
        case MEDIA_CMD_INITIALIZE:
        {

        }
        break;
		case MEDIA_CMD_SET_RINGTONE_FILE:
		{
			//terminate current first:
			TerminateRingtonePlayer();
			m_strRingtoneFile = (const wchar_t*)lParam;
		}
		break;
        case MEDIA_CMD_ENABLE_VIDEO:
        {
            int reason = HIWORD(lParam);
            int enable = LOWORD(lParam);

            EnableVideoPlayer(enable);
        }
        break;
        case MEDIA_CMD_SWITCH_AUDIO_SRC:
            if (/*!m_bSpeechOn && */!m_bPhoneCallActive)
            {
                SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				if(!m_bPlaying)
				{
					MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000,TRUE);
					//DuckAudio(TRUE,0,FALSE);
				}
            }
            break;
        case MEDIA_CMD_PLAY_PAUSE_SWITCH:
        {
            if (m_bHasMediaStream && m_bLoadFileReady)
            {
                static DWORD s_tick=0;
                if (GetTickCount()-s_tick>1000)
                {
                    s_tick=GetTickCount();
                    if (m_bPlaying)
                    {
                        PreparePause();
						m_bEnableAutoPlay = FALSE;
                    }
                    else
                    {
                        //Play();
						PreparePlay();
						m_bEnableAutoPlay = TRUE;
                    }
                }
            }
        }
        break;
        case MEDIA_CMD_CATEGORY:
        {
            if (lParam)
            {
                if (!m_bOpeningFile && !m_bBrowsing)
                {
                   //在注册表里保存当前的播放源类别，视频，音频
                    m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory=MEDIA_CATEGORY(lParam);
                    PrepareSelectCategory(MEDIA_CATEGORY(lParam));
                }
            }
        }
        break;
        case MEDIA_CMD_SELECT_DEVICE:
        {
            if (lParam)
            {
                if (!m_bOpeningFile && !m_bBrowsing)
                {
                    //RETAILMSG(1, (TEXT("MSG:[%s]: PrepareSelectMediaType\r\n"), TEXT(__FUNCTION__)));
                    PrepareSelectMediaType(lParam);
		//更新注册表里的源，下次开机自动切到这个源
                    m_pSkinManager->GetConfig()->GetCurParam()->iMediaType=lParam;
                    //SelectMediaType(lParam);
                }
#if 0				
                else
                {
                    RETAILMSG(1, (TEXT("MSG:[%s]: m_bOpeningFile[%d] m_bBrowsing[%d]\r\n"), TEXT(__FUNCTION__),m_bOpeningFile,m_bBrowsing));
                    RETAILMSG(1, (TEXT("MSG:[%s]: OnPlayerMsgEx Abort and PrepareSelectMediaType\r\n"), TEXT(__FUNCTION__)));
                    OnPlayerMsgEx(MEDIA_MSG_SCAN_STATUS,MEDIA_SCAN_ABORT);
                    PrepareSelectMediaType(lParam);
                    m_pSkinManager->GetConfig()->GetCurParam()->iMediaType=lParam;
                }
#endif				
            }
        }
        break;
        case MEDIA_CMD_MUTE:
        {
#if 0		
            if (m_bHasMediaStream)
            {
                if (lParam)
                {
                    MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000);
                }
                else
                {
                    MuteAudio(FALSE,TRUE,_ARM_MUTE_ALL,FALSE,1000);
                }
            }
#endif
        }
        break;
    }

    //
    if (!m_bMediaModuleLoaded /*|| !g_bBTModuleLoaded*/)
    {
        return;
    }

    switch (wParam)
    {
        case MEDIA_CMD_PLAY_INDEX:
        {
            PlayIndex(lParam);
			m_bEnableAutoPlay = TRUE;
        }
        break;
        case MEDIA_CMD_SET_POSITION:
        {
            static DWORD dwlast=0;
            if (GetTickCount() - dwlast >200)
            {
                dwlast=GetTickCount();
                SetPlayPosition(lParam,MEDIA_SILDER_LENGTH);
                //
                m_dwLastFullScreenTick=GetTickCount();
            }
        }
        break;
        case MEDIA_CMD_PLAY_CONTROL:
        {
            switch (lParam)
            {
			case MEDIA_CONTROL_TERMINATE:  //Merge from FVW8010
				{
					AbortLoadFile();
					WaitForLoadFileReady();
					Stop();
					CleanUp();
				}
				break;
                case MEDIA_CONTROL_STOP:
					//PrepareStop();
					Stop();			//Modified to make it the same as FVW8010
                    break;
                case MEDIA_CONTROL_PAUSE:
                    PreparePause();
				m_bEnableAutoPlay = FALSE;
                    break;
                case MEDIA_CONTROL_PLAY:
                {
					m_bEnableAutoPlay = TRUE;
                    if (m_bHasMediaStream && !m_bManualMute)
                    {
                        if (!m_bManualPlayFile)
                        {
				RETAILMSG(1, (TEXT("OnMediaPlayerCommand: PreparePlay()\r\n")));
				//Play();
				//正常点击播放，从主页点击进入会调用
				PreparePlay();
                        }
                    }
                    else if (!m_bOpeningFile && !m_bBrowsing && !m_bManualPlayFile && !m_bManualMute)
                    {
                    	//开机自动进入多媒体的时候会调用
			RETAILMSG(1, (TEXT("OnMediaPlayerCommand: AutoPlay()\r\n")));
			AutoPlay();
                    }
					else
					{
						RETAILMSG(1, (TEXT("ERR:OnMediaPlayerCommand: m_bManualMute[%d] m_bOpeningFile[%d] m_bHasMediaStream[%d]!!!!\r\n"),m_bManualMute,m_bOpeningFile,m_bHasMediaStream));
					}
                }
                break;
                case MEDIA_CONTROL_NEXT:
                    PlayNext();
				m_bEnableAutoPlay = TRUE;
                    break;
                case MEDIA_CONTROL_PREVIOUS:
                    PlayPrevious();
				m_bEnableAutoPlay = TRUE;
                    break;
                case MEDIA_CONTROL_PLAYMODE_TOGGLE:
                {
                    SwitchPlayMode();
                }
                break;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				case MEDIA_CONTROL_PLAYMODE_REPEAT_ALL:
					{
						SwitchPlayMode(MEDIA_CONTROL_PLAYMODE_REPEAT_ALL);
					}
					break;

				case MEDIA_CONTROL_PLAYMODE_REPEAT_ONE:
					{
						SwitchPlayMode(MEDIA_CONTROL_PLAYMODE_REPEAT_ONE);
					}
					break;

				case MEDIA_CONTROL_PLAYMODE_SHUFFLE:
					{
						SwitchPlayMode(MEDIA_CONTROL_PLAYMODE_SHUFFLE);
					}
					break;
#endif


#if 0				
                case MEDIA_CONTROL_PLAYMODE_REPEAT:
                {
                    m_bRepeat = TRUE;
                    m_bShuffle=FALSE;
                }
                break;
#endif
				default:
				break;				
            }

            //
            m_dwLastFullScreenTick=GetTickCount();

        }
        break;
        case MEDIA_CMD_VIDEO_CONTROL:
        {
            if (lParam==MEDIA_VIDEO_HIDE)
			{
                ShowVideoWindow(FALSE);
				//Bug 0003444: （BOSS AUDIO 1.4.0.0,1.3.3.0 E版本降噪）播放视频时，点击左上角图标进入目录时屏幕有麻点
				Sleep(100);
			}
            else if (lParam==MEDIA_VIDEO_SHOW)
                ShowVideoWindow(TRUE);
            else if (lParam==MEDIA_VIDEO_WINDOW)
                SwitchFullscreen(0);
            else if (lParam==MEDIA_VIDEO_FULLSCREEN)
                SwitchFullscreen(1);
            else if (lParam==MEDIA_VIDEO_TOGGLE)
                SwitchFullscreen(2);
        }
        break;
    }
}
HRESULT CPanelMediaWrapper::OnRingToneMsg(WPARAM wParam, LPARAM lParam)
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
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: ----CM_EVENT_STATE_CHANGED:%d----\r\n"),TEXT(__FUNCTION__),nNewState));
            //m_bRingTonePlaying = (CM_STATE_PLAYING==nNewState);
        }
        break;

        case CM_EVENT_COMPLETE:
        {
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: ----CM_EVENT_COMPLETE----\r\n"),TEXT(__FUNCTION__)));

			if(m_bRingToneLoop)
			{
				if(m_pRingTonePlayer)
				{
					m_pRingTonePlayer->SetPosition(0);
				}
			}
			else
			{
				EndRingTone();
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
HRESULT CPanelMediaWrapper::OnPlayerMsg(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr=S_OK;

    CM_EVENT_BASE* pEvent = (CM_EVENT_BASE*)wParam;

    if (pEvent == NULL || m_pPlayer == NULL)
        return 0;

     	RETAILMSG(DEBUG_MEDIA,(_T("............CPanelMediaWrapper OnPlayerMsg Type:%d\r\n"),pEvent->nType));

    CM_EVENT_TYPE type=pEvent->nType;

    switch (type)
    {
        case CM_EVENT_STATE_CHANGED:
        {
            CM_STATE nNewState = (CM_STATE)pEvent->dwParam;

            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: CM_EVENT_STATE_CHANGED :%d\r\n"),TEXT(__FUNCTION__),nNewState));

            if (nNewState==CM_STATE_PLAYING || nNewState==CM_STATE_PAUSED)
            {
                if (!m_bEnableSmartBar)
                {
                    m_bEnableSmartBar = (CM_STATE_PLAYING==nNewState);
                }

                m_bPlaying = (CM_STATE_PLAYING==nNewState);

                if (m_ppanelMain)
                {
                    m_ppanelMain->SetPlayingStatus(m_bPlaying);
                }
                if (m_ppanelLibrary)
                {
                    m_ppanelLibrary->SetPlayingStatus(m_bPlaying);
                }

                //
                if (m_bPlaying/* && !m_bManualPause*/)
                {
                    //notify play ready:
                    if (!m_bLoadFileReady)
                    {
                        m_bLoadFileReady = TRUE;
                        SetTimer(m_hVideoWnd,TIMER_MEDIA_LOADFILE_READY,100,NULL);
                    }

                    //set fade volume:
                    if (!m_bNeedSetIniPos && !m_bManualPause)
                    {
                        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: media volume come out #### \r\n"),TEXT(__FUNCTION__)));

                        //SetMediaVolume(m_bSpeechOn?MEDIA_LOW_VOLMUE:MEDIA_MAX_VOLMUE,TRUE);
                        MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,100,FALSE);
#if USE_WAV_ONCHIP
                        SetMediaAudioVolume(MEDIA_MAX_VOLMUE,/*FALSE*/TRUE);
#else
                        //
                        if (m_bSpeechOn)
                        {
                            DWORD dwGPS=m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS;
                            DWORD dwMedia=m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia;

                            if (dwGPS>dwMedia)
                            {
                                RETAILMSG(DEBUG_MEDIA,(_T("####### dwGPS>dwMedia\r\n")));
                                SetMediaAudioVolume(dwGPS>0?MEDIA_LOW_VOLMUE*dwMedia/dwGPS:MEDIA_LOW_VOLMUE,/*FALSE*/TRUE);
                            }
                            else
                            {
                                RETAILMSG(DEBUG_MEDIA,(_T("####### dwGPS<=dwMedia\r\n")));
                                SetMediaAudioVolume(dwMedia>0?MEDIA_LOW_VOLMUE*dwGPS/dwMedia:MEDIA_LOW_VOLMUE,/*FALSE*/TRUE);
                            }
                        }
                        else
                        {
                            SetMediaAudioVolume(MEDIA_MAX_VOLMUE,/*FALSE*/TRUE);
                        }
#endif
                    }
                }
            }

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
	 //播放完成，暂停1秒后再开始下一曲
	 //同时定时器触发OnUpdatePlaybackInfo会调用UpdatePlayPostion更新进度条和时间，回到起点
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: ----CM_EVENT_COMPLETE----\r\n"),TEXT(__FUNCTION__)));
			SetTimer(m_hVideoWnd,TIMER_MEDIA_PLAY_COMPLETE,1000,NULL);
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
            RETAILMSG(DEBUG_MEDIA,(L"WARNING:[%s]: %d:%d: %s\r\n", TEXT(__FUNCTION__),HIWORD(pEvent->dwParam), LOWORD(pEvent->dwParam), pEvent->pszText));
            break;

        case CM_EVENT_INFO:		//	low word of dwParam: error code, high word of dwParam: error domain
            //	wprintf(L"Info %d:%d: %s", HIWORD(pEvent->dwParam), LOWORD(pEvent->dwParam), pEvent->pszText);
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

HRESULT CPanelMediaWrapper::OnPlayerMsgEx(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr=S_OK;

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: wParam:%d,lParam:%d\r\n"),TEXT(__FUNCTION__),wParam,lParam));

    switch (wParam)
    {
        case MEDIA_MSG_SCAN_STATUS:
        {
            if (lParam == MEDIA_SCAN_BEGIN)
                OnMediaBrowsingBegin();
	    //多媒体音乐扫描完成
            else if (lParam == MEDIA_SCAN_FINISH)
                OnMediaBrowsingFinish(FALSE);
            else
                OnMediaBrowsingFinish(TRUE);
        }
        break;
		default:
			break;
    }

    return hr;

}

BOOL CPanelMediaWrapper::AutoVolumeStepProc()
{
    BOOL bRet=FALSE;

    //adjust volume:
    if (m_pPlayer && m_pAudio && !m_bVolumeFadeReady)
    {
        if (abs(m_iVolumeCur-m_iVolumeSet)>700)
        {
            // 			int step=(m_iVolumeSet-m_iVolumeCur)/20;
            // // 			if(abs(step)>3000)
            // // 				step=step>0?3000:-3000;
            // 			/*else*/ if(abs(step)<100)
            // 				step=step>0?100:-100;

            int step = (m_iVolumeSet-m_iVolumeCur)>0 ? 700:-700;
            m_iVolumeCur += step;//(m_iVolumeSet-m_iVolumeCur)/20; //

            // 			if(m_iVolumeCur<0x4000)
            // 				m_iVolumeCur=0x4000;

            DWORD dwVol=(m_iVolumeCur<<16)+m_iVolumeCur;
            dwVol=max(dwVol,0x40004000);
            m_pAudio->SetVolume(dwVol);
            bRet=FALSE;

            // 			RETAILMSG(DEBUG_MEDIA,(_T("--%x\r\n"),dwVol));
        }
        else
        {
            m_iVolumeCur=m_iVolumeSet;
            DWORD dwVol=(m_iVolumeSet<<16)+m_iVolumeSet;
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: media reach aim volume:%x\r\n"),TEXT(__FUNCTION__),dwVol));
            m_pAudio->SetVolume(dwVol);
            m_bVolumeFadeReady = TRUE;
            bRet=TRUE;
        }
    }
    else
    {
        m_bVolumeFadeReady=TRUE;
        bRet=TRUE;
    }


    return bRet;
}

void CPanelMediaWrapper::PlayFailed(LPCTSTR lpFileName,UINT idErrCode)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: idErrCode:%d\r\n"),TEXT(__FUNCTION__),idErrCode));

    if (m_ppanelMain)
    {
        m_ppanelMain->SetProgressValue(0);
        m_ppanelMain->SetElapsedTime(0);
        m_ppanelMain->SetRemainTime(0);
        m_ppanelMain->OnPrepareUpdateAlbumArt(NULL,0);
    }

    if (m_ppanelLibrary)
    {
        m_ppanelLibrary->ShowProgressSlider(FALSE);
        m_ppanelLibrary->SetProgressValue(0);
    }

    EnablePlayUI(TRUE,__FUNCTION__);

    //widget:
    if (MEDIA_AUDIO == m_category)
    {
        DWORD dwSet=0;
        //m_pSkinManager->GetGUI()->OperateWidget(UI_MEDIACORE,IDC_MEDIA_SLI_PROGRESS,UIOPRT_SETVALUE,&dwSet);
    }

    m_dwLoadFileErrorCode = idErrCode;


    if (idErrCode == LOAD_STATUS_FAILED_INVALID_FILE)
    {
        if (m_ppanelMain)
        {
            m_ppanelMain->SetLoadingStatus(LOAD_STATUS_FAILED_INVALID_FILE);
        }
        if (m_ppanelLibrary)
        {
            m_ppanelLibrary->SetLoadingStatus(LOAD_STATUS_FAILED_INVALID_FILE);

            //remove current invalid file:
            m_ppanelLibrary->RemoveInvalidFile(lpFileName);
        }

        if (lpFileName)
        {
            WCHAR name[MAX_STRING];
            if (GetFileNameFromPath(lpFileName,name,MAX_STRING))
            {
                if (m_ppanelMain)
                    m_ppanelMain->SetTitle(name);
            }
        }

        Stop();
        SetTimer(m_hVideoWnd,TIMER_MEDIA_LOADFILE_FAILED,5000,NULL);
    }
    else if (idErrCode == LOAD_STATUS_FAILED_NO_FILE)
    {
        if (m_ppanelMain)
        {
            m_ppanelMain->SetLoadingStatus(LOAD_STATUS_FAILED_NO_FILE);

            m_ppanelMain->SetTitle(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
            m_ppanelMain->SetArtist(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
            m_ppanelMain->SetAlbum(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
        }

        if (m_ppanelLibrary)
        {
            m_ppanelLibrary->SetLoadingStatus(LOAD_STATUS_FAILED_NO_FILE);
        }

        Stop();
        SetTimer(m_hVideoWnd,TIMER_MEDIA_LOADFILE_FAILED,1000,NULL);
    }

}
void CPanelMediaWrapper::UpdatePlayPostion()
{
    //update position:
    static DWORD dwLastGetPosTick=0;
    static DWORD dwLastPos=0;
    static DWORD dwPosNotChangedTimes=0;

    BOOL bEnd=FALSE;

    if (/*GetTickCount()-dwLastGetPosTick>PLAYER_PLAYPOS_UPDATE_TIME &&*/ m_pPlayer /*&& m_bPlaying*/)
    {
         		RETAILMSG(DEBUG_MEDIA,(_T("---UpdatePlayPostion....\r\n")));

        DWORD dwPos,dwDuration;
        dwLastGetPosTick=GetTickCount();

        dwDuration=m_pPlayer->GetDuration();
        dwPos=m_pPlayer->GetPosition();

         		RETAILMSG(DEBUG_MEDIA,(_T("dwPos:%d,dwDuration:%d\r\n"),dwPos,dwDuration));

        if (m_bPlaying)
        {
            if ((dwPos != CM_INVALID_TIME) && (dwDuration != CM_INVALID_TIME) )
            {
                m_dwPlayPos=dwPos/1000;

                if ((dwPos<=dwDuration) && (dwDuration >1000))
                {
                    DWORD dwSet=(dwPos/1000)*MEDIA_SILDER_LENGTH/(dwDuration/1000);
			//如果播放结束，则进度条清空
                    if (m_ppanelMain)
                    {
                        m_ppanelMain->SetProgressValue(dwSet);
                        m_ppanelMain->SetElapsedTime(dwPos/1000);
                        m_ppanelMain->SetRemainTime((dwDuration-dwPos)/1000);
                    }
                    if (m_ppanelLibrary)
                    {
                        m_ppanelLibrary->ShowProgressSlider(TRUE);
                        m_ppanelLibrary->SetProgressValue(dwSet);
                    }

                    //
                    SaveCurrentMediaInfo(m_dwPlayPos);

                    //notify can bus:
                    if (m_ppanelLibrary)
                    {
                        int index=m_ppanelLibrary->GetCurIndex()+1;
                        int total=m_ppanelLibrary->GetTrackCount();

                        DWORD dwData1=((BYTE(total))<<24)+((BYTE(total>>8))<<16)+((BYTE(index))<<8)+((BYTE(index>>8))<<0);
                        DWORD dwData2=((dwPos/60000)<<24)+(((dwPos/1000)%60)<<16)+(m_category==MEDIA_VIDEO?0x20:0x11);
                        ::PostMessage(m_hMainWnd,WM_NOTIFY_MEDIA_PLAYBACK_INFO,dwData1,dwData2);
                    }
                }

		//播放结束
                if ((dwPos+200)>=dwDuration)
                {
                    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Player reach end ,dwPos:%d--\r\n"),TEXT(__FUNCTION__),dwPos));
                    bEnd = TRUE;
                }

		//进度不变
                if (dwLastPos == dwPos && dwLastPos!=0) //pos not changed,reach end
                {
                    dwPosNotChangedTimes++;
                    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: pos not changed ,dwPos:%d,times:%d--\r\n"),TEXT(__FUNCTION__),dwPos,dwPosNotChangedTimes));
                    if (dwPosNotChangedTimes>=6)
                    {
                        bEnd = TRUE;
                    }
                }
                else
                {
                    dwPosNotChangedTimes = 0;
                }

		//播放结束
                if (bEnd)
                {
                    dwLastPos = 0;
                    dwPosNotChangedTimes = 0;
                    //send notify:
                    if (	m_bPlayCompleteDetection )
                    {
                        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Send Player reach end notify, dwPos:%d--\r\n"),TEXT(__FUNCTION__),dwPos));
                        m_bPlayCompleteDetection = FALSE;

                        m_bNeedProcessCompleteNotify = TRUE;

                        //关闭定时器
                        EnableMonitorPlayinfo(FALSE);
                        SetTimer(m_hVideoWnd,TIMER_MEDIA_PLAY_COMPLETE,1000,NULL);

                        //OnPlayerComplete();
                    }
                    return;
                }

                dwLastPos = dwPos;
            }

        }
        else if (!m_bLoadFileReady)
        {
            // 			RETAILMSG(DEBUG_MEDIA,(_T("--UpdatePlayPostion not load ready--\r\n")));

            if (0 == dwPos || CM_INVALID_TIME == dwPos || 0 == dwDuration || CM_INVALID_TIME == dwDuration) //pos not changed,reach end
            {
                dwPosNotChangedTimes++;
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: pos not changed 2 ,dwPos:%d,times:%d--\r\n"),TEXT(__FUNCTION__),dwPos,dwPosNotChangedTimes));
                if (dwPosNotChangedTimes>=6)
                {
                    bEnd = TRUE;
                }
            }
            else
            {
                dwPosNotChangedTimes = 0;
            }

            if (bEnd)
            {
                dwPosNotChangedTimes = 0;
                if (m_bPlayCompleteDetection )
                {
                    m_bPlayCompleteDetection = FALSE;
                    m_bNeedProcessCompleteNotify = TRUE;

                    EnableMonitorPlayinfo(FALSE);

                    PlayFailed(m_strCurrentPlayFile.String(),LOAD_STATUS_FAILED_INVALID_FILE);
                }
            }
        }
        else
        {
            dwPosNotChangedTimes = 0;
        }
    }
}
void CPanelMediaWrapper::OnPlayerLoadFileAbort(const char* iReason, int line)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:  %S, line:%d \r\n"),TEXT(__FUNCTION__),iReason,line));

    EnablePlayUI(TRUE,__FUNCTION__);

    Stop();
}
void CPanelMediaWrapper::OnPlayerLoadFileFailed()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));

    if (!m_bOpeningFile && !m_bBrowsing && !m_bManualPlayFile && !m_bHasMediaStream)
    {
        Stop();

        if (m_dwLoadFileErrorCode == LOAD_STATUS_FAILED_INVALID_FILE && m_idSystemPowerState==SYS_POWER_ON)
        {
            PlayNext(FALSE);
        }
    }
}
void CPanelMediaWrapper::OnPlayerLoadFileReady()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));

    if (m_ppanelMain)
    {
        m_ppanelMain->SetLoadingStatus(LOAD_STATUS_SUCCEED);
    }
    if (m_ppanelLibrary)
    {
        m_ppanelLibrary->SetLoadingStatus(LOAD_STATUS_SUCCEED);
    }

    //set ini pos
    if (m_bNeedSetIniPos && m_pPlayer)
    {
        //if(GetTickCount() - m_dwPlayTick>3000)
        {
            // 			MuteAudio(FALSE,TRUE,_ARM_MUTE_ALL,TRUE,2000);
            // 			//SetMediaVolume(0,FALSE);

            //RETAILMSG(DEBUG_MEDIA,(_T(".......set ini pos:%d\r\n"),m_dwIniPos));
            m_bNeedSetIniPos = FALSE;
            HRESULT hr;
            hr = m_pPlayer->SetPosition(m_dwIniPos);
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: set ini pos:%d,hr:%x\r\n"),TEXT(__FUNCTION__),m_dwIniPos,hr));

            ////SetMediaVolume(m_bSpeechOn?MEDIA_LOW_VOLMUE:MEDIA_MAX_VOLMUE,TRUE);
        }
    }

    m_bPlayFileReady = TRUE;

    if (m_ppanelMain)
    {
        m_ppanelMain->UpdateAlbumArt();
    }

    EnablePlayUI(TRUE,__FUNCTION__);

}
void CPanelMediaWrapper::OnPlayerComplete()
{
    // 	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI != UI_MEDIACORE && MEDIA_VIDEO == m_category)
    // 	{
    // 		Stop();
    // 	}

    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));
    if (m_bNeedProcessCompleteNotify /*&& !m_bPhoneCallActive*/ && !m_bManualPause)
    {
        // 		SwitchFullscreen(0);
        // 		ShowVideoWindow(FALSE);
	//如果是循环播放，则继续播放当前的歌
        if (m_bRepeat)
        {
            //PlayFile(m_strCurrentPlayFile.String());
            PreparePlayFile(m_strCurrentPlayFile.String());
        }
        else
        {
            PlayNext(FALSE);
        }
    }
}
void CPanelMediaWrapper::SaveCurrentMediaInfo(DWORD dwPos)
{
    if (m_bHasMediaStream)
    {
        if (m_pSkinManager && m_idCurMediaType)
        {
            if (dwPos ==0 && m_pPlayer)
            {
                dwPos = m_pPlayer->GetPosition()/1000;
            }

            if (m_idCurMediaType != MEDIA_TYPE_INVALID)
            {
                m_pSkinManager->GetConfig()->GetCurParam()->iMediaType=m_idCurMediaType;
            }

            //for device:
            if (m_idCurMediaType == MEDIA_TYPE_SD1)
            {
                if (m_category==MEDIA_VIDEO)
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD1 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosSD1=dwPos;
                }
                else
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileSD1 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosSD1=dwPos;
                }
            }
#ifdef USE_GPS_CARD_MEDIA_PLAY
            else if (m_idCurMediaType == MEDIA_TYPE_SD2)
            {
                if (m_category==MEDIA_VIDEO)
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileSD2 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosSD2=dwPos;
                }
                else
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileSD2 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosSD2=dwPos;
                }
            }
#endif
            else if (m_idCurMediaType == MEDIA_TYPE_USB1)
            {
                if (m_category==MEDIA_VIDEO)
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB1 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosUSB1=dwPos;
                }
                else
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileUSB1 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosUSB1=dwPos;
                }
            }
            else if (m_idCurMediaType == MEDIA_TYPE_USB2)
            {
                if (m_category==MEDIA_VIDEO)
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strVideoFileUSB2 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iVideoPosUSB2=dwPos;
                }
                else
                {
                    m_pSkinManager->GetConfig()->GetCurParam()->strAudioFileUSB2 = m_strCurrentPlayFile;
                    m_pSkinManager->GetConfig()->GetCurParam()->iAudioPosUSB2=dwPos;
                }
            }
        }
    }
}
void CPanelMediaWrapper::OnSystemPowerStatePrepare(UINT idState)
{
    CPanelGL::OnSystemPowerStatePrepare(idState);

    switch (idState)
    {
        case SYS_POWER_OFF:
        case SYS_POWER_SOFT_RESET:
        {
            //save media info:
            SaveCurrentMediaInfo();

            ShowVideoWindow(FALSE);

            PreparePause();
        }
        break;
        case SYS_POWER_IDLE:
        {
            //save media info:
            SaveCurrentMediaInfo();

            if (m_bOpeningFile)
            {
                SetEvent(m_hOpenFileEvent);
                WaitForLoadFileReady();
                Sleep(100);
            }

            if (GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
                PrepareStop();
            else
                Stop();

            //enable UI:
            EnablePlayUI(TRUE,__FUNCTION__);

            if (m_ppanelMain)
            {
                m_ppanelMain->ShowLibraryBtn(FALSE);
            }
        }
        break;
        case SYS_POWER_ON:
        {
            if (GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
            {
                if (m_ppanelMain)
                {
                    m_ppanelMain->SetLoadingStatus(LOAD_STATUS_LOADING);
                }
            }

            m_iAutoDetectDeviceRetryCnt = 10;
        }
        break;
    }

}
BOOL CPanelMediaWrapper::PlayIndex(INT index,DWORD dwPos)
{
    BOOL bRet = FALSE;
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: index:%d\r\n"),TEXT(__FUNCTION__),index));

    if (m_ppanelLibrary)
    {
        if (!m_bOpeningFile)
        {
            LPCTSTR pNext=m_ppanelLibrary->GetIndexValue(index);
            if (pNext)
            {
                //PlayFile(pNext,dwPos);
                PreparePlayFile(pNext,dwPos);
                bRet=TRUE;
            }
            else
            {
                PlayFailed();
            }
        }
    }

    return bRet;
}
void CPanelMediaWrapper::PlayPrevious(BOOL bAni)
{
    if (m_ppanelLibrary)
    {
        if (!m_bOpeningFile)
        {
            LPCTSTR pPrev=m_bShuffle?m_ppanelLibrary->GetRandomValue():m_ppanelLibrary->GetPreviousValue();
            if (pPrev)
            {
                //PlayFile(pPrev);
                PreparePlayFile(pPrev);

                if (bAni)
                {
                    if (m_category == MEDIA_AUDIO)
                    {
                        if (m_ppanelMain)
                        {
                            m_ppanelMain->PlayInfoFlyOut(0);
                        }
                    }
                }
            }
            else
            {
                PlayFailed();
            }
        }
    }
}
void CPanelMediaWrapper::PlayNext(BOOL bAni)
{
    if (m_ppanelLibrary)
    {
        if (!m_bOpeningFile)
        {
        	//下一个，如果是随机播放调用GetRandomValue，列表播放调用GetNextValue
            LPCTSTR pNext=m_bShuffle?m_ppanelLibrary->GetRandomValue():m_ppanelLibrary->GetNextValue();
            if (pNext)
            {
                //PlayFile(pNext);
                //播放下一首
                PreparePlayFile(pNext);

                if (bAni)
                {
                    if (m_category == MEDIA_AUDIO)
                    {
                        if (m_ppanelMain)
                        {
                        	//淡入淡出播放?
                            m_ppanelMain->PlayInfoFlyOut(1);
                        }
                    }
                }
            }
            else
            {
                PlayFailed();
            }
        }
    }
}

#if CVTE_EN_KEY_PROXY
BOOL CPanelMediaWrapper::OnKey(UINT idKey,UINT idStatus)
{
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: idKey: key=0x%x, Status=0x%x--\r\n"),TEXT(__FUNCTION__),idKey,idStatus));

	switch (idKey)
	{
	case KEY_NEXT:
		{
			static int iHoldNextCount = 1;
			if ( idStatus == EV_KEY_SHORT_PRESS )
			{
				if(m_bHasMediaStream)
				{
					PlayNext();
					g_dUIScrollBeginTime = timeGetTime();
					m_bEnableAutoPlay = TRUE;
				}
			}
			else if ( idStatus == EV_KEY_PRESS_HOLD )
			{			
				SetFF(500*iHoldNextCount,TRUE);
				iHoldNextCount++;
			}
			else if (idStatus == EV_KEY_UP)
			{
				SetFF(0,FALSE);
				iHoldNextCount = 0;
			}
		}
		break;

	case KEY_PREVIOUS:
		{
			static int iHoldPrevCount = 1;
			if ( idStatus == EV_KEY_SHORT_PRESS )
			{
				if(m_bHasMediaStream)
				{
					PlayPrevious();
					g_dUIScrollBeginTime = timeGetTime();
					m_bEnableAutoPlay = TRUE;
				}
			}
			else if ( idStatus == EV_KEY_PRESS_HOLD )
			{
				SetFB(500*iHoldPrevCount,TRUE);
				iHoldPrevCount++;
			}
			else if ( idStatus == EV_KEY_UP)
			{
				SetFB(0,FALSE);
				iHoldPrevCount = 0;
			}
		}
		break;

	case KEY_RESUME:
		{
			if(m_bEnableAutoPlay)
			{
				if(m_bHasMediaStream)
				{
					if(!m_bManualPlayFile)
					{
						SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
						//Play();
						PreparePlay();
					}
				}
			}
		}
		break;
	case KEY_PLAY:
		if ( idStatus == EV_KEY_UP )
		{
			m_bEnableAutoPlay = TRUE;
			RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: m_bHasMediaStream[%d], m_bOpeningFile[%d], m_bBrowsing[%d], m_bManualPlayFile[%d]\r\n"),
				TEXT(__FUNCTION__),m_bHasMediaStream,m_bOpeningFile,m_bBrowsing,m_bManualPlayFile));
			if(m_bHasMediaStream)
			{
				if(!m_bManualPlayFile)
				{
					SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
					//Play();
					PreparePlay();
				}
			}
			else if(!m_bOpeningFile && !m_bBrowsing && !m_bManualPlayFile)
			{
				SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				AutoPlay();
			}
		}
		break;
	case KEY_PAUSE:
		if ( idStatus == EV_KEY_UP )
		{
			m_bEnableAutoPlay = FALSE;
			if(m_bHasMediaStream)
			{
				PreparePause();
			}
		}
		break;
	case KEY_PLAYPAUSE:
		if ( idStatus == EV_KEY_UP )
		{
			if(m_bHasMediaStream)
			{
				if(m_bPlaying)
				{
					PreparePause();
					m_bEnableAutoPlay = FALSE;
				}
				else
				{
					//Play();
					PreparePlay();
					m_bEnableAutoPlay = TRUE;
				}
			}
		}
		break;
	case KEY_STOP:
		if ( idStatus == EV_KEY_UP )
		{
			if(m_bHasMediaStream)
			{
				Pause();
			}
			else
			{
				AbortLoadFile();
			}
		}
		break;
	case KEY_MUTE:
		{
			m_bManualMute = !m_bManualMute;
			if(m_bManualMute)
			{
				if (timeGetTime() - g_dUIScrollBeginTime < 1500)
				{
					PreparePause();
					return FALSE;
				}

				if(m_bHasMediaStream)
				{
					SetMediaAudioVolume(0,0);
				}
			}
			else
			{
				if(m_bHasMediaStream)
				{
					SetMediaAudioVolume(MEDIA_MAX_VOLMUE,0);
				}
			}

		}
		break;
	default:
		break;
	}

	return TRUE;
}
#else
BOOL CPanelMediaWrapper::OnKey(UINT idKey)
{
    RETAILMSG(1,(_T("MSG:[%s]: idKey:0x%x--\r\n"),TEXT(__FUNCTION__),idKey));


	switch (idKey)
	{
	case KEY_MCU_TRACK_NEXT:
	case KEY_MEDIA_NEXT:
		if (m_bHasMediaStream)
		{
			PlayNext();
			g_dUIScrollBeginTime = timeGetTime();
			m_bEnableAutoPlay = TRUE;
		}
		break;
	case KEY_MCU_TRACK_PREV:
	case KEY_MEDIA_PREVIOUS:
		if (m_bHasMediaStream)
		{
			PlayPrevious();
			g_dUIScrollBeginTime = timeGetTime();
			m_bEnableAutoPlay = TRUE;
		}
		break;
	case KEY_MEDIA_PLAY:
		{
			m_bEnableAutoPlay = TRUE;
			if (m_bHasMediaStream)
			{
				if (!m_bManualPlayFile)
				{
					SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
					//Play();
					PreparePlay();
				}
			}
			else if (!m_bOpeningFile && !m_bBrowsing && !m_bManualPlayFile)
			{
				SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				AutoPlay();
			}
		}
		break;
	case KEY_MEDIA_PAUSE:
		{
			if (m_bHasMediaStream)
			{
				PreparePause();
				m_bEnableAutoPlay = FALSE;
			}
		}
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		{
			if (m_bHasMediaStream)
			{
				if (m_bPlaying)
				{
					PreparePause();
					m_bEnableAutoPlay = FALSE;
				}
				else
				{
					//Play();
					PreparePlay();
					m_bEnableAutoPlay = TRUE;
				}
			}
		}
		break;
	case KEY_MEDIA_STOP:
		{
			if(m_bHasMediaStream)
			{
				Pause();
			}
		}
		break;
	case KEY_MEDIA_MUTE:
		{
			if (timeGetTime() - g_dUIScrollBeginTime < 1500)
			{
				PreparePause();
				return FALSE;
			}
			m_bManualMute = TRUE;
			if(m_bHasMediaStream)
			{
				SetMediaAudioVolume(0,0);
			}
		}
		break;
	case KEY_MEDIA_UNMUTE:
		{
			m_bManualMute = FALSE;
			if(m_bHasMediaStream)
			{
				SetMediaAudioVolume(MEDIA_MAX_VOLMUE,0);
			}
		}
		break;
	}

	return TRUE;
}
#endif

void CPanelMediaWrapper::AbortSelectMediaType()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));

    EnablePlayUI(TRUE,__FUNCTION__);

    //recover volume:
    MuteAudio(FALSE,TRUE,_ARM_MUTE_ALL,FALSE,1000);
}
void CPanelMediaWrapper::AbortLoadFile()
{
	if(m_bOpeningFile)
	{
		SetEvent(m_hOpenFileEvent);
	}
	m_bAbortLoadFile = TRUE;
}
void CPanelMediaWrapper::AbortSelectCategory()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));

    EnablePlayUI(TRUE,__FUNCTION__);
}
void CPanelMediaWrapper::OnDelayOperation()
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));
    UINT idCurClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);

    //auto pause:
    if (m_bManualPause && m_bPlaying)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to bManualPause!!!!!\r\n"),TEXT(__FUNCTION__)));

        if (m_pPlayer && m_bPlaying)
        {
            m_pPlayer->Pause();
            //m_bPlaying = FALSE; 	//Do not need to modify param here.
        }
    }

	//auto play:
	if(m_bPreparePlay && !m_bPlaying)
	{
		m_bPreparePlay = FALSE;
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("#######OnDelayOperation Ready to bManualPlay!!!!!\r\n")));

		Play();
	}

	//auto stop
	if(m_bManualStop)
	{
		//
		m_bManualStop = FALSE;

        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to bManualStop!!!!!\r\n"),TEXT(__FUNCTION__)));

        Stop();
    }

    //auto set position:
    if (m_bManualSetPosition)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to bManualSetPosition!!!!!\r\n"),TEXT(__FUNCTION__)));

        m_bManualSetPosition=FALSE;

        if (idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT)
        {
            if (FAILED(m_pPlayer->SetPosition(m_dwManualSetPosition)))
            {
            }
            else
            {
            }
            EnableMonitorPlayinfo(TRUE);
        }
        else
        {
            //abort:
        }
    }

    if (m_bManualPlayFile && !m_bManualMute)
    {
        //
        m_bManualPlayFile = FALSE;

        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to ManualPlayFile!!!!!\r\n"),TEXT(__FUNCTION__)));

        if (idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT)
        {
            PlayFile(m_strFileToPlay.String(),m_dwPosToPlay);
        }
        else
        {
            //abort:
            Stop();
        }
    }

	//切换设备的时候进入
    if (m_bManualChangeSrcType)
    {
        //
        m_bManualChangeSrcType = FALSE;

        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to ManualChangeSrcType!!!!!\r\n"),TEXT(__FUNCTION__)));

        if (MEDIA_TYPE_A2DP == m_idSelectMediaType)
        {
            SelectMediaType(m_idSelectMediaType);
        }
        else
        {
            if (idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT)
            {
                SelectMediaType(m_idSelectMediaType);
            }
            else
            {
                AbortSelectMediaType();
            }
        }

    }

	//切换视频源的时候进入
    if (m_bManualChangeCategory)
    {
        //
        m_bManualChangeCategory = FALSE;

        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to ManualChangeCategory!!!!!\r\n"),TEXT(__FUNCTION__)));

        if (idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT)
        {
            SelectCategory(m_categoryToSet);
        }
        else
        {
            AbortSelectCategory();
        }
    }

}
void CPanelMediaWrapper::OnVolumeFadeReday(BOOL bMute)
{

    UINT idCurClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);

    //auto pause:
    if (m_bManualPause && bMute && m_bPlaying)
    {
        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to bManualPause!!!!!\r\n"),TEXT(__FUNCTION__)));

        if (m_pPlayer && m_bPlaying)
        {
            m_pPlayer->Pause();
        }
    }

    //auto stop
    if (m_bManualStop && bMute)
    {
        //
        m_bManualStop = FALSE;

        RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to bManualStop!!!!!\r\n"),TEXT(__FUNCTION__)));
        Stop();
    }

    //auto set position:
    if (m_bManualSetPosition && bMute)
    {
        if ((idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT))
        {
            m_bManualSetPosition=FALSE;
            RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to bManualSetPosition!!!!!\r\n"),TEXT(__FUNCTION__)));

            if (FAILED(m_pPlayer->SetPosition(m_dwManualSetPosition)))
            {
            }
            else
            {
            }
            EnableMonitorPlayinfo(TRUE);
        }
    }

    if (m_bManualPlayFile && bMute)
    {
        //
        if (!m_pSkinManager->GetGUI()->IsAnimating())
        {
            if ((idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT))
            {
                m_bManualPlayFile = FALSE;
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to ManualPlayFile!!!!!\r\n"),TEXT(__FUNCTION__)));

                PlayFile(m_strFileToPlay.String(),m_dwPosToPlay);
            }
        }
    }

    if (m_bManualChangeSrcType && bMute)
    {
        //
        if (!m_pSkinManager->GetGUI()->IsAnimating())
        {
            if ((idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT))
            {
                m_bManualChangeSrcType = FALSE;
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to ManualChangeSrcType!!!!!\r\n"),TEXT(__FUNCTION__)));

                SelectMediaType(m_idSelectMediaType);
            }
        }
    }

    if (m_bManualChangeCategory && bMute)
    {
        //
        if (!m_pSkinManager->GetGUI()->IsAnimating())
        {
            if ((idCurClass==UI_CLASS_MEDIA||idCurClass==UI_CLASS_BT))
            {
                m_bManualChangeCategory = FALSE;
                RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Ready to ManualChangeCategory!!!!!\r\n"),TEXT(__FUNCTION__)));

                SelectCategory(m_categoryToSet);
            }
        }
    }

}
void CPanelMediaWrapper::OnMute(BOOL bMute)
{
    if (GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_MEDIA)
    {
        if (bMute)
        {
            Pause();
        }
        else
        {
            Resume();
        }
    }
}
void CPanelMediaWrapper::Lock()
{
    if (m_DataCS)
    {
        m_DataCS->Enter();
    }
}
void CPanelMediaWrapper::UnLock()
{
    if (m_DataCS)
    {
        m_DataCS->Leave();
    }
}

void CPanelMediaWrapper::OnFactoryCmd(WPARAM wParam, LPARAM lParam)
{
    DWORD dwType = DWORD(wParam);
    DWORD dwCmd = DWORD(lParam);

    switch (dwType)
    {
        case FACTORY_CMD_MEDIA_TEST_VIDEO:
        case FACTORY_CMD_MEDIA_TEST_AUDIO:
        case FACTORY_CMD_MEDIA_TEST_IMAGE:
        case FACTORY_CMD_MEDIA_TEST_IPOD:
        case FACTORY_CMD_MEDIA_TEST_A2DP:
            m_iFactoryTestType = dwType;
            m_iFactoryTestCmd = dwCmd;
            break;
        default:
            break;
    }

    RETAILMSG(DEBUG_MEDIA, (TEXT("MSG:[CPanelMediaWrapper::OnFactoryCmd]: m_iFactoryTestType:%d m_iFactoryTestCmd:%d\r\n"),m_iFactoryTestType,m_iFactoryTestCmd));
}

BOOL CPanelMediaWrapper::IsPlaying()
{
	return m_bPlaying;
}

#if CVTE_EN_DYNAMIC_LOGO
void CPanelMediaWrapper::ClearUpDynamicLogo()
{
    RETAILMSG(1, (TEXT("MSG:[CPanelMediaWrapper::ClearUpDynamicLogo]: ++\r\n")));

    if (m_pDynamicLogoPlayer && m_bDynamicLogoPlaying)
    {
        m_pDynamicLogoPlayer->Pause();
		Sleep(10); //Modified to make it the same as FVW8010
    }

	m_bDynamicLogoPlaying = FALSE;

    //release objects:
    if (m_pDynamicLogoAudio)
    {
        m_pDynamicLogoAudio->Release();
        m_pDynamicLogoAudio=NULL;
    }

    if (m_pDynamicLogoVideo)
    {
        m_pDynamicLogoVideo->Release();
        m_pDynamicLogoVideo=NULL;
    }

    if (m_pDynamicLogoSourceEvent)
    {
        m_pDynamicLogoSourceEvent->Release();
        m_pDynamicLogoSourceEvent=NULL;
    }

    if (m_pDynamicLogoPlayer)
    {
        m_pDynamicLogoPlayer->Release();
        m_pDynamicLogoPlayer=NULL;
    }

	Sleep(10);

    RETAILMSG(1, (TEXT("MSG:[CPanelMediaWrapper::ClearUpDynamicLogo]: --\r\n")));
}

BOOL CPanelMediaWrapper::DynamicLogoFinish()
{
	return !m_bDynamicLogoPlaying;
}
void CPanelMediaWrapper::PlayDynamicLogo(DWORD dwVol)
{
    //if (m_bDynamicLogoPlaying)
    //    return;

	RETAILMSG(1,(_T("MSG:[PlayDynamicLogo]: ++ vol:0x%x\r\n"),dwVol));

	m_bDynamicLogoPlaying = TRUE;
	m_dwDynamicLogoVol = dwVol;

	if(!IsMediaFileExists(DYNAMIC_LOGO_PLAY_FILE))
	{
		RETAILMSG(1,(_T("ERR:[PlayDynamicLogo]: %s is not exit!!!\r\n"),DYNAMIC_LOGO_PLAY_FILE));
		return;
	}

    HANDLE hThread=CreateThread(NULL,NULL,DynamicLogoThreadPro,this,NULL,NULL);

    if (hThread != INVALID_HANDLE_VALUE)
    {
		CeSetThreadPriority(hThread, CE_THREAD_PRIO_256_HIGHEST);
        CloseHandle(hThread);
        hThread = INVALID_HANDLE_VALUE;
    }

	RETAILMSG(1,(_T("MSG:[PlayDynamicLogo]: --\r\n")));
}

void CPanelMediaWrapper::StopDynamicLogo()
{
    //if (!m_bDynamicLogoPlaying)
    //    return;

	RETAILMSG(1,(_T("MSG:[StopDynamicLogo]: \r\n")));
    if (m_pDynamicLogoPlayer && m_bDynamicLogoPlaying)
    {
        //m_pDynamicLogoPlayer->SetPosition(0);
		RETAILMSG(1,(_T("MSG:[StopDynamicLogo]: Pause\r\n")));
        m_pDynamicLogoPlayer->Pause();
		::SetWindowPos(m_hVideoWnd, HWND_BOTTOM , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_HIDEWINDOW) );
		m_pDynamicLogoVideo->UpdateVideo();
		ClearUpDynamicLogo();
//		ShowVideoWindow(FALSE);
    }
    m_bDynamicLogoPlaying = FALSE;

}

HRESULT CPanelMediaWrapper::OnDynamicLogoMsg(WPARAM wParam, LPARAM lParam)
{
    HRESULT hr=S_OK;

    CM_EVENT_BASE* pEvent = (CM_EVENT_BASE*)wParam;

    if (pEvent == NULL)
        return 0;

    CM_EVENT_TYPE type=pEvent->nType;

    switch (type)
    {
        RETAILMSG(1,(_T("MSG:[%s]: -- type:%d --\r\n"),TEXT(__FUNCTION__),type));
        case CM_EVENT_STATE_CHANGED:
        {
            CM_STATE nNewState = (CM_STATE)pEvent->dwParam;
        }
        break;

        case CM_EVENT_COMPLETE:
        {
            RETAILMSG(1,(_T("MSG:[%s]: -- CM_EVENT_COMPLETE --\r\n"),TEXT(__FUNCTION__)));
			StopDynamicLogo();
		}
		break;

    }

    if (m_pCSRMedia)
    {
        m_pCSRMedia->FreeEvent(pEvent);
    }

    return hr;
}

DWORD CPanelMediaWrapper::DynamicLogoThreadPro(LPVOID lpParam)
{
    CPanelMediaWrapper *panel=(CPanelMediaWrapper*)lpParam;
    RETAILMSG(1, (TEXT("MSG:[CPanelMediaWrapper::DynamicLogoThreadPro]: ++\r\n")));

	panel->DynamicLogoRun();

    RETAILMSG(1, (TEXT("MSG:[CPanelMediaWrapper::DynamicLogoThreadPro]: --\r\n")));

    return 0;
}

HRESULT CPanelMediaWrapper::DynamicLogoRun()
{
	HRESULT hr = S_FALSE;
	RETAILMSG(1,(_T("WARN:[%s]: ++\r\n"),TEXT(__FUNCTION__)));

    if(!m_bMediaModuleLoaded)
    {
		RETAILMSG(1,(_T("WARN:[%s]: WaitForMediaModuleReady ++\r\n"),TEXT(__FUNCTION__)));
        WaitForMediaModuleReady();
		RETAILMSG(1,(_T("WARN:[%s]: WaitForMediaModuleReady --\r\n"),TEXT(__FUNCTION__)));
    }

    if(!m_pCSRMedia)
    {
		RETAILMSG(1,(_T("ERR:[%s]: m_pCSRMedia is empty!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
    }

	if(!m_hVideoWnd)
	{
		RETAILMSG(1,(_T("ERR:[%s]: m_hVideoWnd is empty!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
	}

	if(m_bAbortLoadFile)
	{
		RETAILMSG(1,(_T("ERR:[%s]: m_bAbortLoadFile!!!\r\n"),TEXT(__FUNCTION__)));
		goto ABORT;
	}

    //cleanup first:
    //ClearUpDynamicLogo();
	//Sleep(100);

    hr = m_pCSRMedia->CreatePlayer(&m_pDynamicLogoPlayer);
    if (FAILED(hr))
    {
		RETAILMSG(1,(_T("ERR:[%s]: CreatePlayer(m_pDynamicLogoPlayer) failed!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
    }

    hr = m_pDynamicLogoPlayer->QueryInterface(IID_ICSREventSource, (void**)&m_pDynamicLogoSourceEvent);
    if (FAILED(hr))
    {
		RETAILMSG(1,(_T("ERR:[%s]: QueryInterface(m_pDynamicLogoSourceEvent) failed!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
    }

	//stop receiving event messages.
    hr = m_pDynamicLogoSourceEvent->SetNotifyWindow(m_hVideoWnd, WM_DYNAMIC_LOGO_MSG, 0);
    if (FAILED(hr))
    {
		RETAILMSG(1,(_T("ERR:[%s]: SetNotifyWindow(WM_DYNAMIC_LOGO_MSG) failed!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
    }

    hr = m_pDynamicLogoPlayer->QueryInterface(IID_ICSRVideo, (void**)&m_pDynamicLogoVideo);
    if (FAILED(hr))
    {
		RETAILMSG(1,(_T("ERR:[%s]: QueryInterface(m_pDynamicLogoVideo) failed!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
    }

    hr = m_pDynamicLogoVideo->SetVideoWindow(m_hVideoWnd);
    if (FAILED(hr))
    {
		RETAILMSG(1,(_T("ERR:[%s]: SetVideoWindow(m_hVideoWnd) failed!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
    }

    //Force keep the aspect ratio, if change it in playing time, call UpdateVideo() to update the video
    m_pDynamicLogoVideo->KeepAspectRatio(TRUE);

    //has video:
    m_bCurPlayingAudioOnly = FALSE;


    hr = m_pDynamicLogoPlayer->QueryInterface(IID_ICSRWaveoutAudio, (void**)&m_pDynamicLogoAudio);
    if (FAILED(hr))
    {
		RETAILMSG(1,(_T("ERR:[%s]: QueryInterface(m_pDynamicLogoAudio) failed!!!\r\n"),TEXT(__FUNCTION__)));
        goto ERR;
    }

    hr = m_pDynamicLogoAudio->SetDeviceID(1);
	hr = m_pDynamicLogoAudio->SetVolume(m_dwDynamicLogoVol);

    hr = m_pDynamicLogoPlayer->OpenFile(DYNAMIC_LOGO_PLAY_FILE);
    if (FAILED(hr))
    {
		RETAILMSG(1,(_T("ERR:[%s]: OpenFile(%s) failed!!!\r\n"),TEXT(__FUNCTION__),DYNAMIC_LOGO_PLAY_FILE));
        goto ERR;
    }

    if (m_bDynamicLogoPlaying)
    {
        hr = m_pDynamicLogoPlayer->Play();
        if (FAILED(hr))
        {
			RETAILMSG(1,(_T("ERR:[%s]: Play(%s) failed!!!\r\n"),TEXT(__FUNCTION__),DYNAMIC_LOGO_PLAY_FILE));
            return 0;
        }

		RETAILMSG(1,(_T("MSG:[%s]: Play\r\n"),TEXT(__FUNCTION__)));
		::SetWindowPos(m_hVideoWnd, HWND_TOPMOST , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_SHOWWINDOW) );
		::MoveWindow(m_hVideoWnd,0,0,g_iScreenWidth,g_iScreenHeight,TRUE);
		m_pDynamicLogoVideo->UpdateVideo();
		//ShowVideoWindow(TRUE);
    }
	else
	{
		RETAILMSG(1,(_T("ERR:[%s]: m_bDynamicLogoPlaying(%d) !!!\r\n"),TEXT(__FUNCTION__),m_bDynamicLogoPlaying));
	}

	RETAILMSG(1,(_T("WARN:[%s]: --\r\n"),TEXT(__FUNCTION__)));

	return hr;

ERR:
    RETAILMSG(1,(_T("ERR:[%s]: ERR!!!\r\n"),TEXT(__FUNCTION__)));
	return hr;

ABORT:
	RETAILMSG(1,(_T("ERR:[%s]: ABORT!!!\r\n"),TEXT(__FUNCTION__)));
    return hr;
}
#endif
