#include "PanelRadioWrapper.h"
#include "../../DAB/Adapter/DABAdapter.h"
#if CVTE_EN_KEY_PROXY
#include "../Key/CKeyHandler.h"
#endif
#include "../../GFX/pwingdi.h"
#include "../../GFX/Escape.h"

extern DWORD				g_UserClickTick;
extern const wchar_t* g_strInstallPath;

#define RDS_AF_SEARCH_INTERVAL_MIN		3000
#define RDS_AF_SEARCH_INTERVAL_MID		4000
#define RDS_AF_SEARCH_INTERVAL_MAX		10000
#define RDS_AF_SEARCH_INTERVAL_STEP		1000

#define RDS_SIGNAL_LEVEL_THRESHOLD		25

#define GET_RDS_DATA_TIME		70

#if (CUSTOMER_ID == CUSTOMER_ALDINET)
#define RDS_ENABLE_PI_SEARCHING		1
#define RDS_AF_JUMP_ALWAYS_PI_CHECK		0
#else
#define RDS_ENABLE_PI_SEARCHING		0
#define RDS_AF_JUMP_ALWAYS_PI_CHECK		1
#endif


#define RDS_OUTPUT_DEBUG_FILE		0

#if RDS_OUTPUT_DEBUG_FILE
CFileLog g_rdsFileLog(_T("\\USB Disk\\rdsLog.txt"));
#endif

enum RDS_TASK
{
	RDS_TASK_NULL,
	RDS_TASK_TA,
	RDS_TASK_AF,
	RDS_TASK_COMMON,
	RDS_TASK_DAB_LINK,
};
//radio core
CPanelRadioWrapper::CPanelRadioWrapper(void)
{
	m_RegionId = RADIO_REGION_Europe;
	m_BandId = RADIO_BAND_ID_FM;

	m_ppanelMain = NULL;
	m_ppanelLibrary = NULL;

	m_bQuickTAMode = FALSE;
	m_bSpeechOn = FALSE;
	m_bSROn = FALSE;
	m_bPhoneCallActive = FALSE;
	m_bNotifyingRadioInfo = FALSE;
	m_bPlaying = TRUE;
	m_bTASignaled = 0;
	m_idAudioSrcBeforeTA = 0;
	m_idMediaClassBeforeTA = 0;
	m_idUIBeforeTA = 0;
	m_bSrcPlayingBeforeTA = FALSE;
	m_bTAMonitor = FALSE;
	m_bOnTAMode = FALSE;
	m_bTADetected = FALSE;
	m_dwTADetected = 0;

// 	m_iPSBufIndex = 0;
// 	for (int i=0;i<MAX_PS_FRAME;i++)
// 	{
// 		memset(m_bufPS[i],0,8);
// 	}
	memset(m_bufPSOut,0,8);

	memset(&m_rdsData,0,sizeof(RdsData));
	memset(&m_rdsInfo,0,sizeof(RdsInfo));
	memset(&m_rdsInfo.AF,0,sizeof(AFData));
	memset(&m_AFData,0,sizeof(AFData));
	for (int i=0;i<256;i++)
	{
		memset(&m_radioSignalData[i],0,sizeof(SIGNAL_DATA));
	}
	m_dwFreqRequestSingal = 0;
	m_curPIForAF=0xFFFF;
	m_bPISearching=FALSE;
	m_bAFSearching=FALSE;
	m_bAbortCurrentRDSProcess=FALSE;
	m_dwFreqPISearching=0;
	m_dwFreqPISearchStart=0;
	m_rdsStatus = 0;
	m_dwCurFreq = 0;
	m_hRdsThread = NULL;
	m_hEvtRdsData = NULL;
	m_hEvtFreq = NULL;
	m_hEvtRdsThread = NULL;
	m_dwQuality = 0;
	m_bTASearching = FALSE;
	m_dwFreqBeforeTASearching = 0;
	m_dwFreqBeforeDABLink = 0;
	m_dwAFSearchTick = 0;
	m_bMCUScanning = FALSE;
	m_dwRadioCmdTick = 0;
	m_dwSRCChangeTick = 0;
	m_bNeedScanAllAFList = TRUE;

	m_idRdsTask=0; //0-NULL,1-TA,2-AF,3-COMMON

	m_dwSignalLevel = 0;
	m_dwQualityCntNG = 0;
	m_dwQualityCntGood = 0;
	m_idCurrentIdSrc = -1;
	m_dwLastProcessRDSTick = 0;
	m_dwRDSProcessTime = GET_RDS_DATA_TIME;
	m_dwGetSignalQualityTick = 0;
	m_dwRdsDataLossCnt = 0;
	m_dwRdsDataErrorCnt = 0;
	m_dwRdsDataRequestCnt = 0;

	m_bDabLinkProgramFound = FALSE;
	m_bDabServiceLinked = FALSE;
	m_bEnableRDSProcess = TRUE;//FALSE;
	m_bManualPlay = TRUE;
	m_curuiPixelClock = 30000000;
	m_dwTACancelTick = 0;
	m_dwAFSearchInterval = RDS_AF_SEARCH_INTERVAL_MIN; 
	m_bRdsDatabaseChanged = FALSE;
	m_iAFSearchIndex = 0;

	m_idRadioCmdType = 0;
	m_idRadioCmdPara = 0;
	m_dwRdsStartTick = 0;
	m_bUserSetRDSON = FALSE;
	m_dwTAQuitTick = 0;
	m_dwTAEnterTick = 0;
	m_dwRdsTaskStartTick = 0;
	m_userSetPI = 0;
	m_userSetPIFreq = 0;
	m_bAutoUpdateAFinfo = FALSE;
	m_bNeedDoPISearching = TRUE;
	m_bJumpAFFlag = FALSE;
	m_dwLastPISearchTick = 0;
	m_searchingPI = 0;
	m_dwFreqSet = 0;
#if ENABLE_RADIO_DEBUG_SCREEN
	m_bEnbaleDebugScreen = FALSE;
#endif
}

CPanelRadioWrapper::~CPanelRadioWrapper(void)
{

}

void CPanelRadioWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_RADIO_BTN_HOME:
		break;
	}
}
void CALLBACK CPanelRadioWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_RADIO,(_T("CPanelRadioWrapper OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelRadioWrapper *panel=(CPanelRadioWrapper*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);

}
void CPanelRadioWrapper::OnSRStart()
{
	RETAILMSG(DEBUG_RADIO,(_T("............CPanelRadioWrapper OnSRStart......................\r\n")));
	m_bSROn = TRUE;
}
void CPanelRadioWrapper::OnSRTerminate()
{
	RETAILMSG(DEBUG_RADIO,(_T("............CPanelRadioWrapper OnSRTerminate......................\r\n")));
	m_bSROn = FALSE;
}
void CPanelRadioWrapper::OnSpeechStart()
{
	RETAILMSG(DEBUG_RADIO,(_T("............CPanelRadioWrapper OnSpeechStart......................\r\n")));
	m_bSpeechOn = TRUE;
}
void CPanelRadioWrapper::OnSpeechTerminate()
{
	RETAILMSG(DEBUG_RADIO,(_T("............CPanelRadioWrapper OnSpeechTerminate......................\r\n")));
	m_bSpeechOn = FALSE;
}
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
void CPanelRadioWrapper::AbortCurrentRdsProcess()
{
	if(m_idRdsTask != RDS_TASK_NULL && !m_bAbortCurrentRDSProcess)
	{
		RETAILMSG(1/*DEBUG_RDS*/,(_T("====quit current rds process====\r\n")));
		m_bAbortCurrentRDSProcess = TRUE;
		SetEvent(m_hEvtRdsThread);

		SetEventData(m_hEvtRdsData,0);
		SetEvent(m_hEvtRdsData);

		SetEventData(m_hEvtFreq,0);
		SetEvent(m_hEvtFreq);

		SetEventData(m_hEvtRadioSignal,0);
		SetEvent(m_hEvtRadioSignal);

		ResetEvent(m_hEvtRdsAbort);

		BOOL bRet =(WaitForSingleObject(m_hEvtRdsAbort,2000) == WAIT_OBJECT_0);
		RETAILMSG(1/*DEBUG_RDS*/,(_T("====quit current rds process Ret=%d====\r\n"),bRet));
	}
}
#endif
void CPanelRadioWrapper::Play()
{
	RETAILMSG(1/*DEBUG_RADIO*/,(_T("CPanelRadioWrapper Play\r\n")));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_RADIO_PLAY_STATE,0);
	}

	if(!m_bOnTAMode)
	{
		BOOL bTASearching=m_bTASearching;
		if(bTASearching)
		{
			m_dwRadioCmdTick = GetTickCount();
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			//abort rds process first:
			AbortCurrentRdsProcess();
#endif
			//recover previous frequency:
			if(m_dwFreqBeforeTASearching)
			{
				RETAILMSG(1/*DEBUG_RDS*/,(_T("====recover frequecy from TA: form:%d to %d\r\n"),m_dwCurFreq,m_dwFreqBeforeTASearching));
				SetFrequency(m_dwFreqBeforeTASearching,FALSE);
				m_dwFreqBeforeTASearching = 0;
			}
		}
	}
}
void CPanelRadioWrapper::Pause()
{
	RETAILMSG(1/*DEBUG_RADIO*/,(_T("CPanelRadioWrapper Pause\r\n")));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_RADIO_PLAY_STATE,1);
	}
}
void CPanelRadioWrapper::PlayNext()
{
	DWORD freq;

	if(m_pCommander && m_ppanelLibrary)
	{
		if(m_BandId == RADIO_BAND_ID_FM)
		{
			freq = m_ppanelLibrary->GetNextFM();
			//如果当前的频率在FM列表里面，则能获取到列表里的下一个频率
			if(freq!=-1)
			{
				RETAILMSG(1, (TEXT("PlayNext freq!=-1\r\n")));
				SetFrequency(freq,FALSE);
			}
			//如果当期频率不在FM列表里面，则不能获取到，发送_ARM_RADIO_SEARCH_KEY搜索命令
			else
			{
				RETAILMSG(1, (TEXT("PlayNext freq ==-1\r\n")));
				m_pCommander->SendCmd(_ARM_RADIO_SEARCH_KEY,ARM_ADD_OPERATION);
			}
		}
		else if(m_BandId == RADIO_BAND_ID_AM)
		{
			freq = m_ppanelLibrary->GetNextAM();
			if(freq!=-1)
			{
				SetFrequency(freq,FALSE);
			}
			else
			{
				m_pCommander->SendCmd(_ARM_RADIO_SEARCH_KEY,ARM_ADD_OPERATION);
			}
		}

// 		//
// 		if(m_ppanelMain)
// 		{
// 			m_ppanelMain->SetNowPlayingTitle(_T(""));
// 		}
	}

}
void CPanelRadioWrapper::PlayPrevious()
{
	DWORD freq;

	if(m_pCommander && m_ppanelLibrary)
	{
		if(m_BandId == RADIO_BAND_ID_FM)
		{
			freq = m_ppanelLibrary->GetPrevFM();
			if(freq!=-1)
			{
				SetFrequency(freq,FALSE);
			}
			else
			{
				m_pCommander->SendCmd(_ARM_RADIO_SEARCH_KEY,ARM_SUB_OPERATION);
			}
		}
		else if(m_BandId == RADIO_BAND_ID_AM)
		{
			freq = m_ppanelLibrary->GetPrevAM();
			if(freq!=-1)
			{
				SetFrequency(freq,FALSE);
			}
			else
			{
				m_pCommander->SendCmd(_ARM_RADIO_SEARCH_KEY,ARM_SUB_OPERATION);
			}
		}

// 		//
// 		if(m_ppanelMain)
// 		{
// 			m_ppanelMain->SetNowPlayingTitle(_T(""));
// 		}
	}
}
void CPanelRadioWrapper::LinkSmartBarMedia(CSmartBarMedia *pSmartBar)
{
	if(m_ppanelMain)
	{
		m_ppanelMain->LinkSmartBarMedia(pSmartBar);
	}
}
void CPanelRadioWrapper::OnUpdateRadioPlayInfo()
{
	if(m_ppanelMain)
	{
		if(m_ppanelMain->OnUpdateRadioPlayInfo())
		{
			if (m_ppanelLibrary)
			{
				m_ppanelLibrary->OnUpdateRadioPlayInfo();
			}

// 			//clear ps info:
// 			RETAILMSG(1,(_T("#### clear ps info\r\n")));
// 			AnalyzePS(NULL,0);
		}
	}
}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
void CPanelRadioWrapper::OnTAStatus(BOOL bSignaled)
{
	static BOOL bOnMuteBeforeTA = FALSE;
	RETAILMSG(1,(_T("............OnTAStatus: %d..............\r\n"),bSignaled));
	if(bSignaled)
	{
		if(!m_bOnTAMode && !m_bPhoneCallActive /*&& GetTickCount()-g_UserClickTick>3000*/
			&& !m_bSROn && !m_bSpeechOn && m_idSystemPowerState==SYS_POWER_ON
			&& (GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI)!=UI_CLASS_WARNING))
		{
			m_idAudioSrcBeforeTA = 0;
			m_idUIBeforeTA = 0;
			m_idMediaClassBeforeTA = 0;

			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

			if(GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_RADIO)
			{
				m_bOnTAMode = TRUE;
				m_dwTAEnterTick = GetTickCount();
				m_idUIBeforeTA = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
// 				if(GetCurrentSrcPlayingStatus())
// 				{
// 					m_idSrcBeforeTA = idSRC;
// 				}
				bOnMuteBeforeTA = GetCurrentMuteStatus();
				m_bSrcPlayingBeforeTA = GetCurrentSrcPlayingStatus();
				m_idMediaClassBeforeTA = idSRC;
				m_idAudioSrcBeforeTA = GetAudioSRC();

				//signal UI:
				::SendMessage(m_hMainWnd,WM_RDS_TA_STATUS,1,NULL);

				SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE,(1<<16)+0);

				//
				m_ppanelMain->SetOnTAMode(TRUE);
			}
		}
	}
	else
	{
		if(m_bOnTAMode)
		{
			m_bOnTAMode = FALSE;

			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

			m_dwTAQuitTick = GetTickCount();

			//signal UI:
			m_ppanelMain->SetOnTAMode(FALSE);
			::SendMessage(m_hMainWnd,WM_RDS_TA_STATUS,0,NULL);

			if(GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_RADIO && m_idUIBeforeTA && m_bQuickTAMode)
			{
				SwitchPage(GET_PANEL_CLASS_ID(m_idUIBeforeTA),GET_PANEL_PAGE_ID(m_idUIBeforeTA),PAGE_SWITCH_ANI_NONE);
				if(m_idMediaClassBeforeTA)
				{
					SwitchMediaClassSRC(m_idMediaClassBeforeTA);
					//if phone-call is active, do not switch audio source.
					if(!m_bPhoneCallActive)
					{
						SwitchAudioSRC(m_idAudioSrcBeforeTA,TRUE);
						if(m_bSrcPlayingBeforeTA || bOnMuteBeforeTA)
						{
#if CVTE_EN_KEY_PROXY
							CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_RESUME);
							CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_RESUME);
#else
							::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PLAY,NULL);						
#endif
						}
					}
					//reset:
					m_idMediaClassBeforeTA = 0;
				}
			}
		}
	}
}
void CPanelRadioWrapper::OnRadioMonitorTA()
{
	RETAILMSG(1,(_T("............OnRadioMonitorTA: %d..............\r\n"),m_bTASignaled));

	if(m_bTASignaled)
	{
		if(!m_bOnTAMode && !m_bPhoneCallActive /*&& GetTickCount()-g_UserClickTick>3000*/
			&& !m_bSROn && !m_bSpeechOn && m_idSystemPowerState==SYS_POWER_ON)
		{
			m_idAudioSrcBeforeTA = 0;
			m_idUIBeforeTA = 0;
			m_idMediaClassBeforeTA = 0;

			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

			if(GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_RADIO)
			{
				m_bOnTAMode = TRUE;

				m_idUIBeforeTA = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
// 				if(GetCurrentSrcPlayingStatus())
// 				{
// 					m_idSrcBeforeTA = idSRC;
// 				}
				m_bSrcPlayingBeforeTA = GetCurrentSrcPlayingStatus();
				m_idMediaClassBeforeTA = idSRC;
				m_idAudioSrcBeforeTA = GetAudioSRC();

				//signal UI:
				::SendMessage(m_hMainWnd,WM_RDS_TA_STATUS,1,NULL);

				SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE,(1<<16)+0);
			}
		}
	}
	else
	{
		if(m_bOnTAMode)
		{
			m_bOnTAMode = FALSE;

			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

			if(GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_RADIO && m_idUIBeforeTA && m_bQuickTAMode)
			{
				//signal UI:
				::SendMessage(m_hMainWnd,WM_RDS_TA_STATUS,0,NULL);

				SwitchPage(GET_PANEL_CLASS_ID(m_idUIBeforeTA),GET_PANEL_PAGE_ID(m_idUIBeforeTA),PAGE_SWITCH_ANI_NONE);
				if(m_idMediaClassBeforeTA)
				{
					SwitchAudioSRC(m_idAudioSrcBeforeTA);
					SwitchMediaClassSRC(m_idMediaClassBeforeTA);
					
					#if CVTE_EN_KEY_PROXY
					CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
					CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
					#else
					::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PLAY,NULL);
					#endif
					//reset:
					m_idMediaClassBeforeTA = 0;
				}
			}
		}

		//end monitor:
		if(m_bTAMonitor)
		{
			RETAILMSG(1,(_T("----end OnRadioMonitorTA----\r\n")));

			m_bTAMonitor = FALSE;
			KillTimer(m_hMainWnd,TIMER_RADIO_MONITOR_TA);
		}
	}
}
#endif

void CPanelRadioWrapper::OnRadioCommand(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_RADIO,(_T("CPanelRadioWrapper::OnRadioCommand %d,%d\r\n"),wParam,lParam));

#if 0
	//if PI or AF is searching, cancel it.
	if(m_bPISearching || m_bAFSearching)
	{
		m_dwRadioCmdTick = GetTickCount();
		SetEvent(m_hEvtRdsThread);
		m_idRadioCmdType = wParam;
		m_idRadioCmdPara = lParam;
		return;
	}
#endif

	switch (wParam)
	{
	case RADIO_CMD_SET_LOCAL_ONOFF:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_LOC_SEL_KEY,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_SWITCH_AUDIO_SRC:
		if(/*!m_bSpeechOn && */!m_bPhoneCallActive)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_bEnableRDSProcess = TRUE;
			SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
		}
		break;
	case RADIO_CMD_SET_PLAYSTATE:
		{
			m_dwRadioCmdTick = GetTickCount();
			m_bManualPlay = !lParam;
			if(lParam)
			{
				Pause();
			}
			else
			{
				Play();
			}
		}
		break;
	case RADIO_CMD_ENTER_MODE:
		{
			m_bQuickTAMode = (lParam || m_bOnTAMode);
		}
		break;
	case RADIO_CMD_PLAY_NEXT:
		m_dwRadioCmdTick = GetTickCount();
		m_userSetPI = 0;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		//abort rds process first:
		AbortCurrentRdsProcess();
#endif
		//两种不同搜索模式
		PlayNext();
		break;
	case RADIO_CMD_PLAY_PREVIOUS:
		m_dwRadioCmdTick = GetTickCount();
		m_userSetPI = 0;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		//abort rds process first:
		AbortCurrentRdsProcess();
#endif
		PlayPrevious();
		break;
	case RADIO_CMD_SET_FM_STOP_LEVEL:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_RADIO_FM_STOP_LEVEL,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_SET_AM_STOP_LEVEL:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_RADIO_AM_STOP_LEVEL,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_RESET_STOP_LEVEL:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_RADIO_RESET_STOP_LEVEL,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_SELECT_CHANNEL:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_RADIO_CHANNEL_SEL_KEY,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_SELECT_BAND:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_RADIO_BAND_SEL_KEY,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_AMS:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_userSetPI = 0;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			//abort rds process first:
			AbortCurrentRdsProcess();
#endif
			m_pCommander->SendCmd(_ARM_RADIO_AMS_KEY,(BYTE)lParam);

			//clear list:
			if(m_ppanelLibrary)
			{
				m_ppanelLibrary->ClearList(m_BandId);
			}

			if(m_ppanelMain)
			{
				m_ppanelMain->SetNowPlayingTitle(_T(""));
			}
		}
		break;
	case RADIO_CMD_SEARCH:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_userSetPI = 0;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			//abort rds process first:
			AbortCurrentRdsProcess();
#endif
			m_pCommander->SendCmd(_ARM_RADIO_SEARCH_KEY,(BYTE)lParam);

			//每次搜索的时候会将标题清空，在搜索完mcu发低字节频率的时候更新标题
			if(m_ppanelMain)
			{
				m_ppanelMain->SetNowPlayingTitle(_T(""));
			}
		}
		break;
	case RADIO_CMD_SEEK:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_userSetPI = 0;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			//abort rds process first:
			AbortCurrentRdsProcess();
#endif
			m_pCommander->SendCmd(_ARM_RADIO_SEEK_KEY,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_SET_FREQ:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_userSetPI = 0;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			//abort rds process first:
			AbortCurrentRdsProcess();
#endif
			SetFrequency(lParam,FALSE);
		}
		break;
	case RADIO_CMD_RDS_SWITCH:
// 		if(m_pCommander)
// 		{
// 			m_pCommander->SendCmd(_ARM_RDS_DISP_KEY,(BYTE)lParam);
// 		}
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		{
			if(HIWORD(lParam))
			{
				m_bUserSetRDSON = LOWORD(lParam);
			}
			EnableRDS(LOWORD(lParam));
		}
#endif
		break;
	case RADIO_CMD_AF_SWITCH:
// 		if(m_pCommander)
// 		{
// 			m_pCommander->SendCmd(_ARM_RDS_AF_KEY,(BYTE)lParam);
// 		}
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		EnableAF(lParam);
#endif
		break;
	case RADIO_CMD_TA_SWITCH:
// 		if(m_pCommander)
// 		{
// 			m_pCommander->SendCmd(_ARM_RDS_TA_KEY,(BYTE)lParam);
// 		}
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		EnableTA(lParam);
#endif
		break;
	case RADIO_CMD_PTY_SWITCH:
// 		if(m_pCommander)
// 		{
// 			m_pCommander->SendCmd(_ARM_RDS_PTY_KEY,(BYTE)lParam);
// 		}
		break;
	case RADIO_CMD_REG_SWITCH:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_REG_ON_OFF_KEY,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_SELECT_PTY:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_PTY_SEL_KEY,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_QUERY_STORAGE_CNT:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_RADIO_QUERY_STORAGE_CNT,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_QUERY_FREQ:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_RADIO_QUERY_FREQ,(BYTE)lParam);
		}
		break;
	case RADIO_CMD_SET_REGION:
		if(m_pCommander)
		{
			m_dwRadioCmdTick = GetTickCount();
			m_pCommander->SendCmd(_ARM_REGION_SEL_KEY,(BYTE)(LOWORD(lParam)));
		}
		m_ppanelLibrary->SetRadioRegion(RadioRegionId(LOWORD(lParam)),HIWORD(lParam));
		break;

	case RADIO_CMD_ADD_FAVORITE:
		{
			m_dwRadioCmdTick = GetTickCount();
			if(m_ppanelLibrary)
			{
				if(!m_ppanelLibrary->IsFavorite(lParam))
				{
					m_ppanelLibrary->AddFavorite(lParam,m_BandId,m_RegionId);
				}
			}
		}
		break;
	case RADIO_CMD_DELETE_FAVORITE:
		{
			m_dwRadioCmdTick = GetTickCount();
			if(m_ppanelLibrary)
			{
				if(m_ppanelLibrary->IsFavorite(lParam))
				{
					m_ppanelLibrary->DeleteFavorite(lParam);
				}
			}
			if(m_ppanelMain)
			{
				m_ppanelMain->DeleteQuickFav(lParam);
			}
		}
		break;
	case RADIO_CMD_TOGGLE_FAVORITE:
		{
			m_dwRadioCmdTick = GetTickCount();
			if(m_ppanelLibrary)
			{
				if(m_ppanelLibrary->IsFavorite(lParam))
				{
					m_ppanelLibrary->DeleteFavorite(lParam);

					//
					if(m_ppanelMain)
					{
						m_ppanelMain->DeleteQuickFav(lParam);
					}
				}
				else
				{
					m_ppanelLibrary->AddFavorite(lParam,m_BandId,m_RegionId);

					//
					if(m_ppanelMain)
					{
						m_ppanelMain->AutoUpdateQuickFav(lParam);
					}
				}
			}

		}
		break;
	case RADIO_CMD_QUERY_FAVORITE_STATUS:
		{
			if(m_ppanelLibrary && m_ppanelMain)
			{
				m_ppanelMain->SetFavStatus(m_ppanelLibrary->IsFavorite(lParam));

				//update now playing title:
				wchar_t str[32]={0};
				if(m_BandId == RADIO_BAND_ID_FM)
				{
					//判断当期频率是否是喜爱频率，如果是显示标题
					int index=m_ppanelLibrary->GetFMStorageIndex(lParam);
					if(index!=-1)
					{
						swprintf_s(str,32,_T("%d/%d"),index+1,m_ppanelLibrary->GetFMListCnt());
					}
				}
				else if(m_BandId == RADIO_BAND_ID_AM)
				{
					int index=m_ppanelLibrary->GetAMStorageIndex(lParam);
					if(index!=-1)
					{
						swprintf_s(str,32,_T("%d/%d"),index+1,m_ppanelLibrary->GetAMListCnt());
					}
				}	
				m_ppanelMain->SetNowPlayingTitle(str);
			}
		}
		break;
	case RADIO_CMD_CANCEL_TA:
		{
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			m_dwTACancelTick = GetTickCount();
			OnTAStatus(FALSE);
#endif
		}
		break;
	case RADIO_CMD_SET_PI:
		{
			int freq=HIWORD(lParam);
			int picode=LOWORD(lParam);
			m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq = lParam;
			m_userSetPIFreq = freq*100000;
			RETAILMSG(1,(_T("@@@RADIO_CMD_SET_PI: freq=%d, PI=0x%x\r\n"),m_userSetPIFreq,picode));
			m_userSetPI = (WORD)picode;
		}
		break;
#if ENABLE_RADIO_DEBUG_SCREEN
	case RADIO_CMD_ENABLE_DEBUG_SCREEN:
		{
			m_bEnbaleDebugScreen = lParam;
		}
		break;
#endif
	default:
		break;
	}
}
void CPanelRadioWrapper::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_RADIO)
	{
		if(wParam)//active
		{

		}
		else
		{
			//if(!m_bSpeechOn)
			{
				SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
			}
		}
	}
}
void CPanelRadioWrapper::Render()
{
	if(!BeginRender())return;

	EndRender();
}
BOOL CPanelRadioWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

// 	m_panelMain.Initialize(PAGE_RADIOCORE_MAIN,this,pSkinManager,pTextGL,hWnd,entry);
//
// 	m_panelSetting.Initialize(PAGE_RADIOCORE_SETTING,this,pSkinManager,pTextGL,hWnd,entry);
//
// 	m_panelRegion.Initialize(PAGE_RADIOCORE_REGION,this,pSkinManager,pTextGL,hWnd,entry);
//
// 	m_panelRDSSetting.Initialize(PAGE_RADIOCORE_RDS,this,pSkinManager,pTextGL,hWnd,entry);
//
// 	m_panelPTY.Initialize(PAGE_RADIOCORE_PTY,this,pSkinManager,pTextGL,hWnd,entry);



	//
	m_bEnableScreenSaver = TRUE;

	m_hEvtRdsData = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEvtRdsThread = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEvtFreq = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEvtRadioSignal = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEvtRdsAbort = CreateEvent(NULL,FALSE,FALSE,NULL);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_bUserSetRDSON = m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS;
	if(m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS)
	{
		EnableRDS(TRUE);
	}
#endif

	{
		UINT uiPixelClock;
		int ret;
		ret = GetPixelClock(&uiPixelClock);
		RETAILMSG(TRUE, (_T("[CPanelRadioWrapper::Initialize] GetPixelClock ret = %u, uiPixelClock = %u\r\n"), ret, uiPixelClock));
		if (ret >= 0)
		{
			m_curuiPixelClock = uiPixelClock;
		}
	}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	LoadRDSDatabase();
#endif

	return TRUE;
}
void CPanelRadioWrapper::LinkPages(CPanelRadioMain *pMain,CPanelRadioLibrary *pLibrary)
{
	m_ppanelMain = pMain;
	m_ppanelLibrary = pLibrary;

}
int CPanelRadioWrapper::SetPixelClock(UINT guiPixelClock)
{
	HDC hdc = NULL;
	int ret = 1;
	int nEscape = OEMESC_SET_PIXEL_CLOCK;

	hdc = CreateCompatibleDC(NULL);

	if(!hdc)
	{
		ret = -1;
		goto EXIT;
	}

    // query if token supported
	if(((ret = ExtEscape(hdc, QUERYESCSUPPORT, 0, (LPCSTR)&nEscape, 0, NULL)) <= 0))
	{
	    goto EXIT;
	}

	ret = ExtEscape(hdc, OEMESC_SET_PIXEL_CLOCK, sizeof(guiPixelClock),  (LPCSTR)&guiPixelClock, 0, NULL);

EXIT:
	if(hdc)
	{
		DeleteDC(hdc);
	}

	return ret;
}
int CPanelRadioWrapper::GetPixelClock(UINT* guiPixelClock)
{
	HDC hdc = NULL;
	int ret = 1;
	int nEscape = OEMESC_SET_PIXEL_CLOCK;

	hdc = CreateCompatibleDC(NULL);

	if(!hdc)
	{
		ret = -1;
		goto EXIT;
	}

    // query if token supported
	if(((ret = ExtEscape(hdc, QUERYESCSUPPORT, 0, (LPCSTR)&nEscape, 0, NULL)) <= 0))
	{
	    goto EXIT;
	}

	ret = ExtEscape(hdc, OEMESC_GET_PIXEL_CLOCK, 0, NULL, sizeof(UINT), (LPSTR)guiPixelClock);

EXIT:
	if(hdc)
	{
		DeleteDC(hdc);
	}

	return ret;
}
void CPanelRadioWrapper::SetLcdoscRatioOffset(DWORD dwCurFreq)
{
	static BOOL bChangeLcdFrequ = FALSE;
	DWORD ret = -1;
	
	if (!bChangeLcdFrequ && (abs(dwCurFreq - 90000000) <= 500000))
	{
		ret = SetPixelClock(m_curuiPixelClock-m_curuiPixelClock/10);
		RETAILMSG(TRUE, (_T("[CPanelRadioWrapper::SetLcdoscRatioOffset] ret = %u\r\n"), ret));
		if (ret>=0)
		{
			bChangeLcdFrequ = TRUE;
		}
	}
	else if (bChangeLcdFrequ && (abs(dwCurFreq - 90000000) > 500000))
	{
		ret = SetPixelClock(m_curuiPixelClock);
		RETAILMSG(TRUE, (_T("[CPanelRadioWrapper::SetLcdoscRatioOffset] ret = %u\r\n"), ret));
		if (ret>=0)
		{
			bChangeLcdFrequ = FALSE;
		}
	}
}
void CPanelRadioWrapper::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

 	//RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioWrapper::OnMCUCmd:     type:%d,ID:%x,commander:%x\r\n"),dwType,dwID,lParam));

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

	//common:
	if(dwType == 0)//package
	{
		RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioWrapper: update data package:%x\r\n"),dwID));
		//region:
		m_RegionId = ( RadioRegionId(pCommander->GetItemData(MCU_LOCAL_AREA)) );

		//band:
		int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
		BYTE high=pCommander->GetItemData(MCU_CUR_FREQU_STORE_H);
		BYTE low=pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
		if(iband==0)//FM
		{
			m_BandId = (RADIO_BAND_ID_FM);
			m_dwCurFreq = ((high*100+low)*10000);
			SetLcdoscRatioOffset(m_dwCurFreq);
		}
		else//AM
		{
			m_BandId = (RADIO_BAND_ID_AM);
			m_dwCurFreq = ((high*100+low)*1000);
		}
		
		m_bManualPlay = !pCommander->GetItemData(MCU_RADIO_PLAY_STATE);

#if 0
		//get saved use frequency and pi code:
		if(m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq)
		{
			int freq=HIWORD(m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq);
			int picode=LOWORD(m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq);
			m_userSetPIFreq = freq*100000;
			m_userSetPI = (WORD)picode;
		}
#endif

	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
// 		case MCU_CUR_FREQU_STORE_H:
		case MCU_CUR_FREQU_STORE_L:
			{
				int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
				BYTE high=pCommander->GetItemData(MCU_CUR_FREQU_STORE_H);
				BYTE low=pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
				DWORD dwFreq=0;

				if(iband==0)
				{
					dwFreq = ((high*100+low)*10000);
					SetLcdoscRatioOffset(dwFreq);
				}
				else
				{
					dwFreq = ((high*100+low)*1000);
				}

				RETAILMSG(1,(_T("+++MCU_CUR_FREQU: %d,%d\r\n"),high,low));

				if(m_dwCurFreq != dwFreq)
				{
					m_dwCurFreq = dwFreq;
					//clear rds data:
					memset(&m_rdsInfo.AF,0,sizeof(AFData));
					memset(&m_rdsInfo,0,sizeof(RdsInfo));
					m_dwQualityCntNG = 0;
					m_dwQualityCntGood = 0;
					m_dwQuality = 0;
					m_dwGetSignalQualityTick = 0;
					m_dwRdsDataLossCnt = 0;
					m_dwRdsDataErrorCnt = 0;
					m_dwRdsDataRequestCnt = 0;
					m_bAutoUpdateAFinfo = FALSE;
					if(m_userSetPI!=0 && m_dwCurFreq != m_userSetPIFreq && m_dwFreqSet != m_userSetPIFreq && !m_bPISearching && !m_bJumpAFFlag)
					{
						RETAILMSG(1,(_T("+++MCU_CUR_FREQU: reset user PI, freq=%d\r\n"),m_dwCurFreq));
						m_userSetPI = 0;
						m_pSkinManager->GetConfig()->GetCurParam()->dwCurFreq = 0;
					}
					//reset:
					if(m_dwCurFreq == m_dwFreqSet)
						m_dwFreqSet = 0;

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
					//reset UI: if not bJumpAFFlag
					if(!m_bJumpAFFlag)
					{
						AnalyzePS(NULL,0);
						AnalyzeRT(NULL,0);
						m_ppanelMain->SetPTY(PT_Invalid);
					}
					m_bJumpAFFlag = FALSE;
#endif
				}
				//notify event:
				SetEventData(m_hEvtFreq,1);
				SetEvent(m_hEvtFreq);
			}
			break;
		case MCU_RADIO_PLAY_STATE:
			{
				m_bPlaying=!pCommander->GetItemData(MCU_RADIO_PLAY_STATE);
			}
			break;
		case MCU_LOCAL_AREA:
			{
				m_RegionId = ( RadioRegionId(pCommander->GetItemData(MCU_LOCAL_AREA)) );
			}
			break;
		case MCU_CUR_BAND_STORE:
			{
				int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
				if(iband==0)//FM
				{
					m_BandId = (RADIO_BAND_ID_FM);
				}
				else//AM
				{
					m_BandId = (RADIO_BAND_ID_AM);
				}
			}
			break;
		case MCU_SYS_INFO_TITLE:
			{
				BYTE info=pCommander->GetItemData(MCU_SYS_INFO_TITLE);
				m_bMCUScanning=(info==3||info==4);
			}
			break;
		case MCU_CUR_CHANNEL_LEVEL:
			{
				m_dwSignalLevel = pCommander->GetItemData(MCU_CUR_CHANNEL_LEVEL);

				//RETAILMSG(1,(_T(">[%d,S=%d]\r\n"),m_dwCurFreq/100000,m_dwSignalLevel));
				SetEventData(m_hEvtRadioSignal,1);
				SetEvent(m_hEvtRadioSignal);
			}
			break;
		case MCU_RADIO_SIGNAL_STATUS:
			{
				//RETAILMSG(DEBUG_RDS,(_T(">MCU_RADIO_SIGNAL_STATUS[0x%x]\r\n"),pCommander->GetRadioSignalData()->signalStatus));
				m_radioSignalStatus = pCommander->GetRadioSignalData()->signalStatus;
				SetEventData(m_hEvtRadioSignal,1);
				SetEvent(m_hEvtRadioSignal);
			}
			break;
		case MCU_RADIO_SIGNAL_DATA:
			{
				const RADIO_SIGNAL_DATA *pData = (pCommander->GetRadioSignalData());
				int idx=m_dwFreqRequestSingal/100000 - 875;
				WORD picode=0;
				if((pData->signalStatus & 0x8000) && idx>=0 && idx<=205)
				{
					m_radioSignalData[idx].signalData = *pData;
					picode = m_radioSignalData[idx].PICode;
#if ENABLE_RADIO_DEBUG_SCREEN
					if(m_bEnbaleDebugScreen)
					{
						m_ppanelMain->SetDebugRadioSignalData(idx,&m_radioSignalData[idx]);
					}
#endif
				}
				SetEventData(m_hEvtRdsData,1);
				SetEvent(m_hEvtRdsData);
				RETAILMSG(0,(_T("[%d,%X][0x%x][%d][%d,%d,%d,%d,%d,%d]\r\n"),
					m_dwFreqRequestSingal/100000,
					picode,
					pData->signalStatus,
					pData->level,
					pData->usn,
					pData->wam,
					pData->offset,
					pData->bandwidth,
					pData->modulation,
					pData->stopQuality));
#if RDS_OUTPUT_DEBUG_FILE
				g_rdsFileLog.OutPut(_T("[%d][%d,%X][0x%x][%d][%d,%d,%d,%d,%d,%d]\r\n"),
					GetTickCount(),
					m_dwFreqRequestSingal/100000,
					picode,
					pData->signalStatus,
					pData->level,
					pData->usn,
					pData->wam,
					pData->offset,
					pData->bandwidth,
					pData->modulation,
					pData->stopQuality);
#endif
			}
			break;
		case MCU_RDS_STATUS:
			{
				m_rdsStatus=pCommander->GetRDSData()->rdsStatus;
				//RETAILMSG(DEBUG_RDS,(_T(">: MCU_RDS_STATUS [0x%x]\r\n"),m_rdsStatus));
				SetEventData(m_hEvtRdsData,1);
				SetEvent(m_hEvtRdsData);
			}
			break;
		case MCU_RDS_BLOCK_DATA:
			{
				BYTE *data=(BYTE*)pCommander->GetRDSData()->rdsBlockData;
				m_rdsData.blockA = (data[0*2]<<8) + data[0*2+1];
				m_rdsData.blockB = (data[1*2]<<8) + data[1*2+1];
				m_rdsData.blockC = (data[2*2]<<8) + data[2*2+1];
				m_rdsData.blockD = (data[3*2]<<8) + data[3*2+1];
				m_rdsData.decErr = (data[4*2]<<8) + data[4*2+1];
				//RETAILMSG(DEBUG_RDS,(_T(">: MCU_RDS_BLOCK_DATA [ 0x%x,0x%x,0x%x,0x%x, err: 0x%x ]\r\n"),m_rdsData.blockA,m_rdsData.blockB,m_rdsData.blockC,m_rdsData.blockD,m_rdsData.decErr));
				SetEventData(m_hEvtRdsData,1);
				SetEvent(m_hEvtRdsData);
			}
			break;
		case MCU_RADIO_QUALITY:
			{
				m_dwQuality = pCommander->GetItemData(MCU_RADIO_QUALITY);
				//RETAILMSG(1,(_T(">: MCU_RADIO_QUALITY [0x%x]\r\n"),m_dwQuality));
				SetEventData(m_hEvtRdsData,1);
				SetEvent(m_hEvtRdsData);
			}
			break;
		case MCU_RDS_STATUS_DATA:
			{

			}
			break;
		default:
			break;
		}
	}

}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
void CPanelRadioWrapper::AnalyzeRT(BYTE *bufRT, INT iLen)
{
	if(iLen && bufRT)
	{
		wchar_t strRT[128];
		int cnt=MultiByteToWideChar( CP_ACP, 0, (char*)bufRT, iLen, strRT, _countof(strRT));
		if(cnt > 0)
		{
			//cut tail space:
			strRT[cnt]=0;
			int ilen=wcslen(strRT);
			if(ilen>1)
			{
				//replace '\n' to ' '
				wchar_t *cursor = strRT;
				while (*cursor)
				{
					if(*cursor == '\n' || *cursor == '\r')
						*cursor = ' ';
					cursor++;
				}	
				//cut tail space:
				wchar_t *p=strRT+ilen-1;
				while (*p == ' ')
				{
					*p = 0;
					p--;
				}
			}

			if(m_ppanelMain)
				m_ppanelMain->SetRT(strRT);
		}
	}
	else
	{
		if(m_ppanelMain)
			m_ppanelMain->SetRT(NULL);
	}
}
void CPanelRadioWrapper::AnalyzePS(BYTE *bufPS, INT iLen)
{
	static BOOL bNeedRefreshPS=TRUE;

	if(iLen && bufPS)
	{

// 		if(m_iPSBufIndex>=MAX_PS_FRAME)
// 			m_iPSBufIndex=0;

// 		RETAILMSG(DEBUG_RDS,(_T("MSG:RADIO:RadioWrapper:RDS: %x %x %x %x %x %x %x %x [%d]\r\n"),bufPS[0],bufPS[1],bufPS[2],bufPS[3],bufPS[4],bufPS[5],bufPS[6],bufPS[7],m_iPSBufIndex));

		//memcpy(m_bufPS[m_iPSBufIndex],bufPS,8);

// 		m_iPSBufIndex++;

// 		for (int j=0;j<MAX_PS_FRAME;j++)
// 		{
// 			RETAILMSG(0,(_T("@: %x %x %x %x %x %x %x %x\r\n"),m_bufPS[j][0],m_bufPS[j][1],m_bufPS[j][2],m_bufPS[j][3],m_bufPS[j][4],m_bufPS[j][5],m_bufPS[j][6],m_bufPS[j][7]));
// 		}

		//
		BYTE output[9]={0};

		//20160218
		memcpy(output,bufPS,8);
		output[8]=0;

//			for (int i=0;i<8;i++)
//			{
//				BYTE temp[128]={0};
//				BYTE max=0;
//				BYTE t=0;
//
//				for (int m=0;m<MAX_PS_FRAME;m++)
//				{
//					t=m_bufPS[m][i];
//					if(t>0 && t<128 /*&& t!=0x20*/)
//						temp[t]++;
//				}
//
//				for (int n=0;n<128;n++)
//				{
//					if(max<temp[n])
//					{
//						max=temp[n];
//						output[i]=n;
//					}
//				}
//			}

		//get final result:
		//RETAILMSG(1,(_T("<: %x %x %x %x %x %x %x %x\r\n"),output[0],output[1],output[2],output[3],output[4],output[5],output[6],output[7]));

		if(output[0] /*&& memcmp(output,m_bufPSOut,8)*/)
		{
			//check valid ps, the lastest 2 segment are the same
			if(memcmp(output,m_bufPSOut,8) == 0)
			{
				if(bNeedRefreshPS)
				{
					wchar_t ps_output[16]=_T("");

					//GBKTOUnicode((char *)output,9,ps_output,16);
					MultiCharTOUnicode((char *)output,9,ps_output,16,GetLanguageCodePage(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage));

					//cut tail white space:
					for(int m=7;m>0;m--)
					{
						if(ps_output[m]==0x20)
						{
							ps_output[m]=0;
						}
						else
						{
							break;
						}
					}
					//cut head space:
					int ihead_space=0;
					for (int i=0;i<7;i++)
					{
						if(ps_output[i] == 0x20)
							ihead_space++;
						else
							break;
					}

					if(m_ppanelMain)
						m_ppanelMain->SetPS(ps_output+ihead_space);
					if(m_ppanelLibrary)
						m_ppanelLibrary->SetPS(ps_output+ihead_space);
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
					//Update PS infor to MCU
					if (m_pCommander)
					{
						m_pCommander->SendExtenCmd(_ARM_EXTEN_RDS_PS_INFOR,bufPS,8);
						BYTE Valid = 1;
						m_pCommander->SendExtenCmd(_ARM_EXTEN_RDS_INFOR_VALID,&Valid,1);
					}
#endif
				}

				bNeedRefreshPS = FALSE;
			}
			else
			{
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
				BYTE Valid = 0;
				m_pCommander->SendExtenCmd(_ARM_EXTEN_RDS_INFOR_VALID,&Valid,1);
#endif
				bNeedRefreshPS = TRUE;
			}

			memcpy(m_bufPSOut,output,8);
			m_bufPSOut[8]=0;
		}
	}
	else
	{
        //RETAILMSG(1,(_T("MSG:RADIO:RadioWrapper:RDS: clear! bufPS=%d,iLen=%d\r\n"),bufPS,iLen));

		//clear:
// 		m_iPSBufIndex = 0;
// 		for (int i=0;i<MAX_PS_FRAME;i++)
// 		{
// 			memset(m_bufPS[i],0,8);
// 		}

		memset(m_bufPSOut,0,8);

		//
		if(m_ppanelMain)
			m_ppanelMain->SetPS(NULL);
		if(m_ppanelLibrary)
			m_ppanelLibrary->SetPS(NULL);

#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
		//Update PS infor to MCU
		if (m_pCommander)
		{
			BYTE buftemp[8] = {0};
			m_pCommander->SendExtenCmd(_ARM_EXTEN_RDS_PS_INFOR,buftemp,8);
			BYTE Valid = 0;
			m_pCommander->SendExtenCmd(_ARM_EXTEN_RDS_INFOR_VALID,&Valid,1);
		}
#endif
	}

}
#endif

void CPanelRadioWrapper::OnSRCChanged(UINT idSRC)
{
	//RETAILMSG(DEBUG_RADIO,(_T("CPanelRadioWrapper::OnSRCChanged %x\r\n"),idSRC));
	m_idCurrentIdSrc = idSRC;
	m_dwSRCChangeTick = GetTickCount();

	if(	GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_RADIO)
	{
		if(m_bNotifyingRadioInfo)
		{
			m_bNotifyingRadioInfo = FALSE;
		}

		/*
		if(m_bOnTAMode)
		{
			m_idSrcBeforeTA = 0;
			m_idUIBeforeTA = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
			if(GetCurrentSrcPlayingStatus() && GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_RADIO)
			{
				m_idSrcBeforeTA = idSRC;
			}
		}
		*/

		//
		if(m_ppanelMain)
		{
			m_ppanelMain->EnableUpdateWidget(FALSE);
		}
	}
	else
	{
		if(!m_bOnTAMode)
		{
			if(m_ppanelMain)
			{
				m_ppanelMain->EnableUpdateWidget(TRUE);
			}
		}
	}
}
void CPanelRadioWrapper::OnUIChanged(UINT idUI)
{
	if(	GET_PANEL_CLASS_ID(idUI) != UI_CLASS_RADIO)
	{
		if(m_bOnTAMode)
		{
			/*
			m_idMediaClassBeforeTA = 0;
			m_idUIBeforeTA = m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
			if(GetCurrentSrcPlayingStatus() && GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_RADIO)
			{
				m_idMediaClassBeforeTA = idSRC;
			}
			*/
			//RETAILMSG(1,(_T("##OnUIChanged:: TA is on, but user switch UI to 0x%x, cancel TA for a while\r\n"),idUI));
			//OnRadioCommand(RADIO_CMD_CANCEL_TA,0);
		}
	}
}

#if CVTE_EN_KEY_PROXY
BOOL CPanelRadioWrapper::OnKey(UINT idKey,UINT idStatus)
{
    RETAILMSG(1,(_T("MSG:CPanelRadioWrapper::OnKey: key=0x%x, Status=0x%x\r\n"),idKey,idStatus));

	switch (idKey)
	{
	case KEY_NEXT:
		if ( idStatus == EV_KEY_SHORT_PRESS )
		{
			SendRadioCommand(RADIO_CMD_PLAY_NEXT,0);
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			SendRadioCommand(RADIO_CMD_SEARCH,ARM_ADD_OPERATION);
		}
		break;
	case KEY_PREVIOUS:
		if ( idStatus == EV_KEY_SHORT_PRESS )
		{
			SendRadioCommand(RADIO_CMD_PLAY_PREVIOUS,0);
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			SendRadioCommand(RADIO_CMD_SEARCH,ARM_SUB_OPERATION);
		}
		break;
	case KEY_MCU_AMS:
		SendRadioCommand(RADIO_CMD_AMS,1);
		break;
	case KEY_MCU_AMS_LONG:
		SendRadioCommand(RADIO_CMD_AMS,1);
		break;
	case KEY_RESUME:
		if ( idStatus == EV_KEY_UP )
		{
			if(m_bManualPlay)
			{
				if(!m_bPhoneCallActive)
				{
					SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
				}
				Play();
			}
		}
		break;
	case KEY_PLAY:
		if ( idStatus == EV_KEY_UP )
		{
			if(!m_bPhoneCallActive)
			{
				SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
			}
			m_bManualPlay=TRUE;
			Play();
		}
		break;
	case KEY_STOP:
	case KEY_PAUSE:
		if ( idStatus == EV_KEY_UP )
		{
			Pause();
		}
		break;
	case KEY_PLAYPAUSE:
		if ( idStatus == EV_KEY_UP )
		{
			if(m_bPlaying)
			{
				Pause();
			}
			else
			{
				if(!m_bPhoneCallActive)
				{
					SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
				}
				Play();
			}
		}
		break;
	}

	return TRUE;
}
#else
BOOL CPanelRadioWrapper::OnKey(UINT idKey)
{
    RETAILMSG(1,(_T("MSG:CPanelRadioWrapper::OnKey: key=0x%x, Status=0x%x\r\n"),idKey,idStatus));

	switch (idKey)
	{
	case KEY_MCU_ENTER:
		{

		}
		break;
	case KEY_MCU_SEEK_NEXT:
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_RADIO_SEEK_KEY,ARM_ADD_OPERATION);
		}
		break;
	case KEY_MCU_SEEK_PREV:
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_RADIO_SEEK_KEY,ARM_SUB_OPERATION);
		}
		break;
	case KEY_MCU_AMS:
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_RADIO_AMS_KEY,1);
		}
		break;
	case KEY_MCU_AMS_LONG:
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_RADIO_AMS_KEY,1);
		}
		break;

	case KEY_MCU_TRACK_NEXT:
	case KEY_MEDIA_NEXT:
		{
			PlayNext();
		}
		break;
	case KEY_MCU_TRACK_PREV:
	case KEY_MEDIA_PREVIOUS:
		{
			PlayPrevious();
		}
		break;
	case KEY_MEDIA_PLAY:
		{
			if(!m_bPhoneCallActive)
			{
				SwitchAudioSRC(SRC_AUDIO_RADIO,TRUE);
			}
			m_bManualPlay=TRUE;
			Play();
		}
		break;
	case KEY_MEDIA_PAUSE:
		{
			m_bManualPlay=FALSE;
			Pause();
		}
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		{
			if(m_bPlaying)
			{
				m_bManualPlay=FALSE;
				Pause();
			}
			else
			{
				m_bManualPlay=TRUE;
				Play();
			}
		}
		break;
	}

	return TRUE;
}
#endif


void CPanelRadioWrapper::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(1,(_T("............CPanelRadioWrapper:: OnSystemPowerStatePrepare:%d, bUserSetRDSON:%d..........\r\n"),idState,m_bUserSetRDSON));

	switch (idState)
	{
	case SYS_POWER_OFF:
	//case SYS_POWER_SOFT_RESET:
		{
			if(m_bTASearching)
			{
				if(m_dwFreqBeforeTASearching)
				{
					RETAILMSG(1,(_T("=====power off, quit TA,recover freq:%d\r\n"),m_dwFreqBeforeTASearching));
					SetFrequency(m_dwFreqBeforeTASearching,FALSE);
					m_dwFreqBeforeTASearching = 0;
				}
			}
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
			m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS = m_bUserSetRDSON;
			SaveRDSDatabase();
#endif
		}
		break;
	case SYS_POWER_ON:
		{
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_RADIO)
			{
				//play
// 				if(m_pCommander)
// 				{
// 					m_pCommander->SendCmd(_ARM_RADIO_PLAY_STATE,0);
// 				}
			}
		}
		break;
	}

}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)

void CPanelRadioWrapper::EnableRDS(BOOL Enable)
{
	RETAILMSG(DEBUG_RDS,(_T("<%s> :%d\r\n"),_T(__FUNCTION__),Enable));

	m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS = Enable;
	if(Enable)
	{
		//begin thread for rds:
		if(m_hRdsThread == NULL)
		{
			m_dwRdsStartTick = GetTickCount();
			m_hRdsThread=CreateThread(NULL,NULL,RdsThreadPro,this,NULL,NULL);
			CeSetThreadPriority(m_hRdsThread, CE_THREAD_PRIO_256_ABOVE_NORMAL);
		}
	}
	else
	{
		//terminate rds thread:
		if(m_hRdsThread != NULL)
		{
			SetEvent(m_hEvtRdsThread);
			//wait quit:
			WaitForSingleObject(m_hRdsThread,3000);
			CloseHandle(m_hRdsThread);
			m_hRdsThread = NULL;
		}
	}
}
void CPanelRadioWrapper::EnableAF(BOOL Enable)
{
	RETAILMSG(DEBUG_RDS,(_T("<%s> :%d\r\n"),_T(__FUNCTION__),Enable));

	m_pSkinManager->GetConfig()->GetCurParam()->bRadioAF = Enable;

}
void CPanelRadioWrapper::EnableTA(BOOL Enable)
{
	RETAILMSG(DEBUG_RDS,(_T("<%s> :%d\r\n"),_T(__FUNCTION__),Enable));

	m_dwTACancelTick = 0;
	m_pSkinManager->GetConfig()->GetCurParam()->bRadioTA = Enable;

}
BOOL CPanelRadioWrapper::RdsCommonProcess()
{
	RETAILMSG(DEBUG_RDS,(_T("<%s> \r\n"),_T(__FUNCTION__)));

	//test:
// 	m_ppanelMain->SetPS(_T("JAMFMABB"));
// 	m_ppanelMain->SetRT(_T("SDF FG ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
// 	m_ppanelMain->SetPTY(PT_Easy_Music);
	BOOL bRet=FALSE;

	if(!GetSignalQuality(FALSE/*!m_dwQuality*/))
	{
		RETAILMSG(DEBUG_ERR,(_T("<%s> QueryQuality FAILED!\r\n"),_T(__FUNCTION__)));
		return FALSE;
	}

	if(m_dwQuality)
	{
		UINT type;
		UINT err;
		err = GetRDSData(&type);
		if(err == ERR_RDS_OK)
		{
			bRet=TRUE;
			//Get rds data and push to UI:
			//PS:
			if(type==RDS_GROUP_TYPE_A(0) || type==RDS_GROUP_TYPE_B(0)) //0A & 0B has PS information
			{
				AnalyzePS(m_rdsInfo.PS,8);
			}
			//PTY:
			if(m_ppanelMain)
			{
				m_ppanelMain->SetPTY(ProgramType(m_rdsInfo.PTY));
			}
			//RT:
			if(type==RDS_GROUP_TYPE_A(2) || type==RDS_GROUP_TYPE_B(2)) //2A & 2B has RT information
			{
				AnalyzeRT(m_rdsInfo.RT,64);
			}
		}
	}

	return bRet;
}
BOOL CPanelRadioWrapper::RdsAFProcess()
{
	//RETAILMSG(DEBUG_RDS,(_T("<%s> \r\n"),_T(__FUNCTION__)));
	static DWORD s_dwCnt=0;
	static DWORD s_dwLastScanAllListTick=0;
	static DWORD s_dwLastCalErrTick=0;
	static DWORD s_dwCheckAllListCnt=0;
	UINT type;
	UINT err;
	BOOL bRet=FALSE;
	BOOL bRdsLoss=FALSE;
	INT iErrPercent=-1;
	BOOL bPIChangedUnexpected=FALSE;

#if RDS_ENABLE_PI_SEARCHING
	if(m_bPISearching && m_searchingPI!=0 && m_dwFreqPISearching)
	{
		DWORD dwFreqMinFM = 87500000;
		DWORD dwFreqMaxFM = 108000000;
		DWORD dwStep=100000;//0.1M

		if(m_RegionId == RADIO_REGION_Europe)
			dwFreqMaxFM = 108000000;
		else if(m_RegionId == RADIO_REGION_USA)
			dwFreqMaxFM = 107900000;
		else
			return FALSE;
		m_dwFreqPISearching+=dwStep;
		if(m_dwFreqPISearching>dwFreqMaxFM)
			m_dwFreqPISearching=dwFreqMinFM;
		if(m_dwFreqPISearching<dwFreqMinFM)
			m_dwFreqPISearching=dwFreqMinFM;

		if(m_dwFreqPISearching == m_dwFreqPISearchStart)
		{
			if(m_ppanelMain)
				m_ppanelMain->SetAFSearching(FALSE);
			if(m_ppanelLibrary)
				m_ppanelLibrary->SetAFSearching(FALSE);

			SetFrequency(m_dwFreqPISearching);

			RETAILMSG(1,(_T("==>AF: finish PI searching, PI=0x%x, freq=%d\r\n"),m_searchingPI,m_dwFreqPISearchStart));

			//un-mute radio output:
			if(m_bPISearching)
			{
				if(m_pCommander)
				{
					m_pCommander->SendCmd(_ARM_RADIO_PLAY_STATE,0);
				}
			}
			m_bPISearching = FALSE;
		}
		else
		{
			if(SetFrequencyAndGetSignal(m_dwFreqPISearching))
			{
				if(m_dwSignalLevel < RDS_SIGNAL_LEVEL_THRESHOLD)
					return TRUE;
				if(GetSignalQuality(TRUE))
				{
					if(m_dwQuality)
					{
						int itry=2;
						err = ERR_RDS_NO_DATA;
						do 
						{
							//wait for rds data ready:
							Sleep(GET_RDS_DATA_TIME);
							err = GetRDSData(&type);
							itry--;
						} while (err != ERR_RDS_OK && itry>0);

						if(err == ERR_RDS_OK)
						{
							RETAILMSG(1,(_T("==>PI searched=0x%x\r\n"),m_rdsInfo.PICode));

							if(m_searchingPI == m_rdsInfo.PICode)
							{
								m_curPIForAF = m_searchingPI;
								RETAILMSG(1,(_T("==>PI searching, Got correct PI=0x%x, replace work PI\r\n"),m_searchingPI));

								if(m_ppanelMain)
									m_ppanelMain->SetAFSearching(FALSE);
								if(m_ppanelLibrary)
									m_ppanelLibrary->SetAFSearching(FALSE);

								//reset freq:
								SetFrequency(m_dwFreqPISearching);
								Sleep(100);
								//un-mute radio output:
								if(m_bPISearching)
								{
									if(m_pCommander)
									{
										m_pCommander->SendCmd(_ARM_RADIO_PLAY_STATE,0);
									}
								}
								m_bPISearching = FALSE;

								//check favorite:
								if(m_ppanelLibrary->IsFavorite(m_dwFreqPISearchStart) && !m_ppanelLibrary->IsFavorite(m_dwFreqPISearching))
								{
									SendRadioCommand(RADIO_CMD_ADD_FAVORITE,m_dwFreqPISearching);
								}
							}
						}
					}
					bRet=TRUE;
				}
			}
		}
		//m_dwAFSearchInterval = RDS_AF_SEARCH_INTERVAL_MIN;
		m_dwAFSearchTick = GetTickCount();
		return bRet;
	}
#endif

	if(!GetSignalQuality(TRUE))
	{
		RETAILMSG(DEBUG_ERR,(_T("<%s> QueryQuality FAILED!\r\n"),_T(__FUNCTION__)));
		return FALSE;
	}

	if(m_dwQuality)
	{
		err = GetRDSData(&type,&bRdsLoss);
		if(bRdsLoss)
		{
			m_dwRdsDataLossCnt++;
		}
		if(err == ERR_RDS_OK)
		{
			m_dwRdsDataRequestCnt++;
			bRet=TRUE;
			//Get rds data and push to UI:
			//PS:
			if(type==RDS_GROUP_TYPE_A(0) || type==RDS_GROUP_TYPE_B(0)) //0A & 0B has PS information
			{
				AnalyzePS(m_rdsInfo.PS,8);
			}
			//PTY:
			if(m_ppanelMain)
			{
				m_ppanelMain->SetPTY(ProgramType(m_rdsInfo.PTY));
			}
			//RT:	
			if(type==RDS_GROUP_TYPE_A(2) || type==RDS_GROUP_TYPE_B(2)) //2A & 2B has RT information
			{
				AnalyzeRT(m_rdsInfo.RT,64);
			}

			//AF list: 0A
			if(type==RDS_GROUP_TYPE_A(0)) //0A has AF information
			{
				//if AF list updated, scan all AF list
				if(m_AFData.AFFilledNum != m_rdsInfo.AF.AFFilledNum)
				{
					m_bNeedScanAllAFList = TRUE;
				}
				memcpy(&m_AFData,&m_rdsInfo.AF,sizeof(AFData));
#if ENABLE_RADIO_DEBUG_SCREEN
				if(m_bEnbaleDebugScreen)
				{
					m_ppanelMain->SetDebugAFData(&m_AFData);
				}
#endif
			}

			//PI
			if(m_curPIForAF == 0xFFFF)
			{
				m_curPIForAF = m_rdsInfo.PICode;
			}
			else
			{
				if(m_userSetPI && m_userSetPI==m_rdsInfo.PICode)
					m_curPIForAF = m_userSetPI;
				else
					bPIChangedUnexpected = (m_curPIForAF != m_rdsInfo.PICode);
			}
		}
		else if(err == ERR_RDS_DECODE_ERR)
		{
			m_dwRdsDataErrorCnt++;
		}
	}

#if RDS_ENABLE_PI_SEARCHING
	//check PI is user set, if not start PI searching:
	if( bPIChangedUnexpected || (m_userSetPI != 0 && m_curPIForAF != 0xFFFF && m_curPIForAF != m_userSetPI) )
	{
		if(!m_bPISearching)
		{
			RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: PI do not match user setting or changed unexpected!! PI=%X, user=%X, PI get=%X\r\n"),m_curPIForAF,m_userSetPI,m_rdsInfo.PICode));
			StartPISearching(m_dwCurFreq,(m_userSetPI?m_userSetPI:m_curPIForAF));
			return TRUE;
		}
	}
#endif

#if 0
	//for test:
	m_AFData.AFFreq[0]=1;
	m_AFData.AFFreq[1]=2;
	m_AFData.AFFreq[2]=45;
	m_AFData.AFFreq[3]=88;
	m_AFData.AFFreq[4]=123;
	m_AFData.AFFreq[5]=126;
	m_AFData.AFFreq[6]=143;
	m_AFData.AFFreq[7]=188;
	m_AFData.AFFilledNum=8;
	m_AFSignal[21]=80;
	m_AFSignal[123]=55;
	m_AFSignal[45]=45;
	m_AFSignal[0]=10;
	m_curPIForAF = 0xeeee;
#endif

	if(GetTickCount() - s_dwLastCalErrTick > 2000)
	{
		if(m_dwRdsDataErrorCnt+m_dwRdsDataRequestCnt)
		{
			iErrPercent = (m_dwRdsDataErrorCnt)*100/(m_dwRdsDataErrorCnt+m_dwRdsDataRequestCnt);
			RETAILMSG(DEBUG_RDS,(_T("==>AF: [%d][%d,%d,%d%%]\r\n"),
				m_dwCurFreq/100000,m_dwRdsDataLossCnt,m_dwRdsDataRequestCnt,iErrPercent));
#if RDS_OUTPUT_DEBUG_FILE
			g_rdsFileLog.OutPut(_T("[%d]==>AF: [%d][%d,%d,%d%%]\r\n"),
				GetTickCount(),m_dwCurFreq/100000,m_dwRdsDataLossCnt,m_dwRdsDataRequestCnt,iErrPercent);
#endif
			m_dwRdsDataErrorCnt = 0;
			m_dwRdsDataRequestCnt = 0;
			m_dwRdsDataLossCnt = 0;
		}
		s_dwLastCalErrTick = GetTickCount();
	}

	DWORD dwStopLevelAF = m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF;
#if (CVTE_EN_RDS || CVTE_EN_RBDS) && (CVTE_EN_RDS_AF_STOP_LEVEL)
	if(m_dwQuality == 0 || m_dwSignalLevel < dwStopLevelAF)
#else
	if(m_dwQuality == 0)
#endif
	{
		m_dwQualityCntNG+=((m_dwQuality == 0)?10:5);
		if(m_dwQualityCntNG > 100)
			m_dwQualityCntNG = 100;
		m_dwQualityCntGood = 0;
		m_dwAFSearchInterval = RDS_AF_SEARCH_INTERVAL_MIN; 
	}
	else
	{
		m_dwQualityCntGood++;
		if(m_dwQualityCntGood > 100)
			m_dwQualityCntGood = 100;
		if(m_dwQualityCntGood > 10)
		{
			if(m_dwSignalLevel < 60)
				m_dwAFSearchInterval = RDS_AF_SEARCH_INTERVAL_MID; 
			else
				m_dwAFSearchInterval = RDS_AF_SEARCH_INTERVAL_MAX; 
			m_dwQualityCntNG = 0;
		}
	}

#if ENABLE_RADIO_DEBUG_SCREEN
	if(m_bEnbaleDebugScreen)
	{
		m_ppanelMain->SetDebugPIData(m_curPIForAF,m_userSetPI);
	}
#endif

	//get AF list:
	//if current AF list is empty, try to get it from database
	if((m_curPIForAF == 0xFFFF || m_AFData.AFFilledNum == 0) && !m_bAutoUpdateAFinfo && GetTickCount()-m_dwRdsTaskStartTick > 2000)
	{
		RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: AutoUpdateAFinfo [%d]\r\n"),m_dwCurFreq/100000));
		m_bAutoUpdateAFinfo = TRUE;
		RdsInfo *pinfo=SearchDatabaseByAF(m_dwCurFreq);
		if(pinfo)
		{
			m_bNeedScanAllAFList = TRUE;
			memcpy(&m_AFData,&pinfo->AF,sizeof(AFData));
			if(m_userSetPI != 0)
			{
				m_curPIForAF = m_userSetPI;
			}
			RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: SearchDatabaseByAF %d, Num=%d\r\n"),m_dwCurFreq/100000,m_AFData.AFFilledNum));
#if 0
			m_curPIForAF = pinfo->PICode;
			RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: SearchDatabaseByAF [%d,%X],Num=%d, userSetPI=0x%X\r\n"),
				m_dwCurFreq/100000,m_curPIForAF,m_AFData.AFFilledNum,m_userSetPI));
			if(m_userSetPI != 0 && m_curPIForAF != m_userSetPI)
			{
				//invalid PI, use PI use set:
				m_curPIForAF = m_userSetPI;
			}
#endif
		}
#if RDS_ENABLE_PI_SEARCHING
		else if(m_curPIForAF == 0xFFFF)
		{
			if(m_userSetPI != 0)
			{
#if 0
				m_curPIForAF = m_userSetPI;
 				RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: user set PI [%d,%X]\r\n"),
 					m_dwCurFreq/100000,m_curPIForAF));
#endif
				if(!m_bPISearching)
				{
					RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: Current PI is not valid!!!! PI=%X, user=%X\r\n"),m_curPIForAF,m_userSetPI));
					StartPISearching(m_dwCurFreq,m_userSetPI);
					return TRUE;
				}
			}
#if 0
			else if(m_ppanelMain)//if user not set PI, try to get current PI from rds list:
			{
				DWORD picode=m_ppanelMain->GetFreqPI(m_dwCurFreq);
				if(picode !=0 )
				{
					m_curPIForAF = (WORD)picode;
					RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: GetFreqPI [%d,%X]\r\n"),
						m_dwCurFreq/100000,m_curPIForAF));
				}
			}
#endif
		}
#endif
	}

	if(GetTickCount() - s_dwCnt > 2000)
	{
		RETAILMSG(DEBUG_RDS,(_T("==>AF: [%d,%X][%d][%d,%d][%d][%d,%d][%d]\r\n"),
			m_dwCurFreq/100000,m_curPIForAF,m_dwQuality,m_dwSignalLevel,dwStopLevelAF,m_AFData.AFFilledNum,m_dwQualityCntNG,m_dwQualityCntGood,m_dwAFSearchInterval));
		s_dwCnt=GetTickCount();
#if RDS_OUTPUT_DEBUG_FILE
		GetSignalData(m_dwCurFreq);
		g_rdsFileLog.OutPut(_T("[%d]==>AF: [%d,%X][%d][%d,%d][%d][%d,%d][%d]\r\n"),
			GetTickCount(),m_dwCurFreq/100000,m_curPIForAF,m_dwQuality,m_dwSignalLevel,dwStopLevelAF,m_AFData.AFFilledNum,m_dwQualityCntNG,m_dwQualityCntGood,m_dwAFSearchInterval);
#endif
	}

	//process frequency change:
	if(/*m_dwQualityCntNG>50 && */m_curPIForAF != 0xFFFF && GetTickCount()-m_dwRdsTaskStartTick > 2000)
	{
		const DWORD dwAFSearchStart = m_dwCurFreq;
		BOOL bGetAFSucceed = FALSE;
		if(m_AFData.AFFilledNum>1)
		{
			if(GetTickCount()-m_dwAFSearchTick>m_dwAFSearchInterval)
			{
				m_dwAFSearchTick = GetTickCount();

				//search current index:
				const BYTE idx_freq_cur = m_dwCurFreq/100000 - 875; 

				//update AF signal:
				int index_for_check = 0;
				BOOL bCheckSingle = (m_dwAFSearchInterval >= RDS_AF_SEARCH_INTERVAL_MID);
				//update current first:
				DetectAFSignal(m_dwCurFreq);
				if(bCheckSingle)
				{
					s_dwCheckAllListCnt = 0;
					index_for_check = m_iAFSearchIndex;
					const BYTE idx_freq_check = m_AFData.AFFreq[index_for_check];
					m_iAFSearchIndex++;
					if(m_iAFSearchIndex>=m_AFData.AFFilledNum)
						m_iAFSearchIndex=0;
					if(!DetectAFSignal((m_AFData.AFFreq[index_for_check] + 875) * 100000))
						return FALSE;
					if(idx_freq_check != idx_freq_cur)
						bGetAFSucceed = ProcessAF(idx_freq_check,idx_freq_cur);
				}
				else
				{
					s_dwCheckAllListCnt++;
					int loop_cnt=0;
					index_for_check = 0;
					for (int i=0; i<m_AFData.AFFilledNum; i++)
					{
						if(m_AFData.AFFreq[i] == idx_freq_cur)
						{
							index_for_check = i;
						}
					}
					while (loop_cnt < m_AFData.AFFilledNum)
					{
						if(WAIT_OBJECT_0 == WaitForSingleObject(m_hEvtRdsThread,100))
							break;
						if(m_bAbortCurrentRDSProcess)
							return FALSE;
						if(!DetectAFSignal((m_AFData.AFFreq[index_for_check] + 875) * 100000))
							break;
						index_for_check++;
						if(index_for_check >= m_AFData.AFFilledNum)
							index_for_check = 0;
						loop_cnt++;
					}

					//search a highest quality frequency
					for (int i=0; i<m_AFData.AFFilledNum; i++)
					{
						const BYTE idx_freq_check = m_AFData.AFFreq[i];
						if(idx_freq_check == idx_freq_cur)
							continue;
						if(m_bAbortCurrentRDSProcess)
							return FALSE;
						if(bGetAFSucceed = ProcessAF(idx_freq_check,idx_freq_cur))
							break;
					}			
				}
			}
		}

#if RDS_ENABLE_PI_SEARCHING
		if((m_userSetPI||m_curPIForAF!=0xFFFF) &&
			!bGetAFSucceed &&
			(m_bNeedDoPISearching/*||GetTickCount()-m_dwLastPISearchTick>60000*/) && 
			!m_bAbortCurrentRDSProcess && 
			m_dwQualityCntNG>=100 && 
			(m_AFData.AFFilledNum>1 ? s_dwCheckAllListCnt>3 : TRUE) &&
			GetTickCount()-m_dwRdsTaskStartTick > 10000)
		{
			m_bNeedDoPISearching = FALSE;
			m_dwLastPISearchTick = GetTickCount();
			if(GetSignalQuality(TRUE))
			{
				if(!m_dwQuality)
				{
					StartPISearching(dwAFSearchStart,(m_userSetPI?m_userSetPI:m_curPIForAF));
				}
			}
		}
#endif

	}

	return bRet;
}

void CPanelRadioWrapper::StartPISearching(DWORD dwStartFreq,WORD piSearch)
{
	//then enter PI searching...
	//mute radio output:
	m_dwLastPISearchTick = GetTickCount();
	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_RADIO_PLAY_STATE,1);
	}
	m_bPISearching = TRUE;
	m_dwFreqPISearchStart = dwStartFreq;//m_dwCurFreq;
	m_dwFreqPISearching = m_dwFreqPISearchStart;
	m_searchingPI = piSearch;
	RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF: start PI searching, PI=0x%x, freq=%d\r\n"),m_searchingPI,m_dwFreqPISearchStart));
	if(m_ppanelMain)
		m_ppanelMain->SetAFSearching(TRUE);
	if(m_ppanelLibrary)
		m_ppanelLibrary->SetAFSearching(TRUE);
}

BOOL CPanelRadioWrapper::ProcessAF(const int idxProcess, const int idxCurrent)
{
	BOOL bRet = FALSE;
	if((idxCurrent>=0 && idxCurrent <= 205) && (idxProcess>=0 && idxProcess <= 205))
	{
		RETAILMSG(0,(_T("=====ProcessAF:  idxProcess=[%d,%d,%d], idxCurrent=[%d,%d,%d] \r\n"),
			idxProcess,m_radioSignalData[idxProcess].signalData.level,m_radioSignalData[idxProcess].signalData.stopQuality,
			idxCurrent,m_radioSignalData[idxCurrent].signalData.level,m_radioSignalData[idxCurrent].signalData.stopQuality));
		//if current signal is good enough, we need higher signal level(+20) to trigger AF switch.
		const WORD offset = (m_dwQualityCntGood >= 100) ? 20 : 0;
		if(	m_radioSignalData[idxProcess].signalData.stopQuality &&
			m_radioSignalData[idxProcess].signalData.level > (m_radioSignalData[idxCurrent].signalData.level+offset))
		{
			const DWORD dwFreqToCheck = (idxProcess + 875) * 100000;
			const DWORD dwFreqCurrent = (idxCurrent + 875) * 100000;
#if (!RDS_AF_JUMP_ALWAYS_PI_CHECK)
			//valid pi, jump directly, then check pi:
			if (m_radioSignalData[idxProcess].PICode == m_curPIForAF)
			{
				RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF Search: jump directly %d\r\n"),dwFreqToCheck));
				JumpAF(dwFreqToCheck);
				if( CheckPI(dwFreqToCheck,5) != m_curPIForAF )
				{
					RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF Search: wrong pi, return back %d\r\n"),dwFreqCurrent));
					JumpAF(dwFreqCurrent);
				}
				else
				{
					//check favorite:
					if(m_ppanelLibrary->IsFavorite(dwFreqCurrent) && !m_ppanelLibrary->IsFavorite(dwFreqToCheck))
					{
						SendRadioCommand(RADIO_CMD_ADD_FAVORITE,dwFreqToCheck);
					}
					bRet = TRUE;
				}
			}
			//no valid PI code(PICode==0), then check it, 
			//or the  current signal quality is bad, also try to check it.
			else if( (m_radioSignalData[idxProcess].PICode == 0) || (m_dwQualityCntNG >= 100 ))
#else
			if( (m_radioSignalData[idxProcess].PICode == m_curPIForAF) || 
				(m_radioSignalData[idxProcess].PICode == 0) || 
				(m_dwQualityCntNG >= 100))
#endif
			{
				const int check_cnt = (m_dwQuality) ? 2 : 5;
				CheckAF(dwFreqToCheck);
				if( CheckPI(dwFreqToCheck,check_cnt) == m_curPIForAF )
				{
					RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF Search: jump %d\r\n"),dwFreqToCheck));
					//SetFrequency(dwFreqToCheck);
					JumpAF(dwFreqToCheck);

					//check favorite:
					if(m_ppanelLibrary->IsFavorite(dwFreqCurrent) && !m_ppanelLibrary->IsFavorite(dwFreqToCheck))
					{
						SendRadioCommand(RADIO_CMD_ADD_FAVORITE,dwFreqToCheck);
					}
					bRet = TRUE;
				}
				else
				{
					RETAILMSG(1/*DEBUG_RDS*/,(_T("==>AF Search: return back %d\r\n"),dwFreqCurrent));
					JumpAF(dwFreqCurrent);
				}
			}
		}
	}
	RETAILMSG(0,(_T("=====ProcessAF: ret=%d\r\n"),bRet,idxProcess,idxCurrent));
	return bRet;
}

#endif
void CPanelRadioWrapper::OnDabServiceLinkMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		if(GET_PANEL_CLASS_ID(m_idCurrentIdSrc) == UI_CLASS_DAB)
		{
			if(m_bDabLinkProgramFound)
			{
				m_bDabServiceLinked = TRUE;
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE,(1<<16)+0);
			}
		}
	}
	else
	{
		if(GET_PANEL_CLASS_ID(m_idCurrentIdSrc) == UI_CLASS_RADIO)
		{
			if(m_bDabServiceLinked)
			{
				m_bDabServiceLinked = FALSE;
				SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_NONE);
			}
		}
	}
}


#if (CVTE_EN_RDS || CVTE_EN_RBDS)
#if CVTE_EN_DAB
BOOL CPanelRadioWrapper::RdsDABServiceLinkProcess()
{
	static DWORD s_dwLastTick = 0;
	link_RDS_FM_info_t *pInfo = CDABAdapter::GetInstance()->GetServiceLinkInfo();

	BOOL bGetLink=FALSE;

	if(pInfo && pInfo->PICode[0])
	{
		//RETAILMSG(1,(_T("<%s> link to PI=0x%x\r\n"),_T(__FUNCTION__),pInfo->PICode[0]));

		//check PI:
		if(!GetSignalQuality(/*FALSE*/TRUE))
		{
			RETAILMSG(DEBUG_ERR,(_T("<%s> QueryQuality FAILED!\r\n"),_T(__FUNCTION__)));
			return FALSE;
		}

		if(GetTickCount() - s_dwLastTick > 5000)
		{
			s_dwLastTick = GetTickCount();
			RETAILMSG(1,(_T("==>DABLink to PI=0x%x, freq=%d, Q=%d\r\n"),pInfo->PICode[0],m_dwCurFreq,m_dwQuality));
		}

		if(m_dwQuality)
		{
			UINT type;
			UINT err;
			err = GetRDSData(&type);
			if(err == ERR_RDS_OK)
			{
				for (int i=0;i<MAX_PI_CODE_SEND;i++)
				{
					if(pInfo->PICode[i] != 0 && m_rdsInfo.PICode == pInfo->PICode[i])
					{
						RETAILMSG(1,(_T("<%s> Get Link PI code=0x%x\r\n"),_T(__FUNCTION__),m_rdsInfo.PICode));
						bGetLink = TRUE;
						break;
					}
				}
			}
		}

		//step frequency:
		if(!bGetLink)
		{
			DWORD freq = m_dwCurFreq / 100000 * 100000;
			if(freq < 87500000)
				freq = 87500000;

			if(!m_bOnTAMode)
			{
				freq+=100000;//step 0.1M
				if(freq>108000000)
				{
					freq=87500000;
				}
				if(SetFrequency(freq))
				{
					Sleep(100);
				}
				else
				{
					RETAILMSG(DEBUG_ERR,(_T("<%s> SetFrequency FAILED!\r\n"),_T(__FUNCTION__)));
					return FALSE;
				}
			}
		}
	}
	return bGetLink;
}
#endif

BOOL CPanelRadioWrapper::RdsTAProcess()
{
	if(m_dwTACancelTick && GetTickCount() - m_dwTACancelTick < 15 * 60000)//user cancel TA, restart TA after 15 minutes
	{
		return FALSE;
	}

	static DWORD s_freq_TA=m_dwCurFreq / 100000 * 100000;
	static UINT s_iNoRdsCnt=0;
	//static DWORD s_dwLastTick=0;
	BOOL bGetTAInfo=FALSE;
	UINT iDetectTAStationCnt=0;

	BOOL bfreqChanged=FALSE;
	//DWORD freq = m_dwCurFreq / 100000 * 100000;
	//if(freq_TA < 87500000)
	//	freq_TA = 87500000;

	if(!m_bOnTAMode)
	{
		//don't change frequency within 1 minutes after last TA quit.
		if(m_dwTAQuitTick==0?TRUE:(GetTickCount()-m_dwTAQuitTick > 60000))
		{
			//step:
			DWORD dwFreqMinFM = 87500000;
			DWORD dwFreqMaxFM = 108000000;
			DWORD dwStep=100000;//0.1M

			switch (m_RegionId)
			{
			case RADIO_REGION_Europe:
				dwFreqMaxFM = CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX;
				break;
			case RADIO_REGION_USA:
				dwFreqMaxFM = CVTE_DEF_RADIO_USA_FM_FREQ_MAX;
				break;
			case RADIO_REGION_USA2:
				dwFreqMaxFM = CVTE_DEF_RADIO_USA2_FM_FREQ_MAX;
				dwStep = 200000;
				break;
			case RADIO_REGION_ArabiaAustralia:
				dwFreqMaxFM = CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX;
				break;
			case RADIO_REGION_LatinAmerica:
				dwFreqMaxFM = CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX;
				break;
			case RADIO_REGION_MiddleEast:
				dwFreqMaxFM = CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX;
				break;
			case RADIO_REGION_Aust:
				dwFreqMaxFM = CVTE_DEF_RADIO_AUST_FM_FREQ_MAX;
				break;
			case RADIO_REGION_Brazil:
				dwFreqMaxFM = CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX;
				break;
			default:
				return FALSE;
			}

			s_freq_TA+=dwStep;
			if(s_freq_TA>dwFreqMaxFM)
				s_freq_TA=dwFreqMinFM;
			if(s_freq_TA<dwFreqMinFM)
				s_freq_TA=dwFreqMinFM;
		}

		if(s_freq_TA != m_dwCurFreq)
		{
			if(SetFrequencyAndGetSignal(s_freq_TA))
			{
				m_rdsInfo.TA = 0;
				m_rdsInfo.TP = 0;
				//Sleep(100);
				bfreqChanged=TRUE;
				s_iNoRdsCnt = 0;
			}
			else
			{
				RETAILMSG(DEBUG_ERR,(_T("<%s> SetFrequency FAILED! %d\r\n"),_T(__FUNCTION__),s_freq_TA));
				return FALSE;
			}
		}
	}

	if(bfreqChanged)
	{
		if(m_dwSignalLevel < RDS_SIGNAL_LEVEL_THRESHOLD)
		{
			return TRUE;
		}
	}

	if(!GetSignalQuality(bfreqChanged))
	{
		RETAILMSG(DEBUG_ERR,(_T("<%s> QueryQuality FAILED!\r\n"),_T(__FUNCTION__)));
		return FALSE;
	}

	//if(GetTickCount() - s_dwLastTick > 1000)
	//{
	//	s_dwLastTick = GetTickCount();
	//	RETAILMSG(1,(_T("==>TA: F=%d, Q=%d, freqChanged=%d\r\n"),s_freq_TA,m_dwQuality,bfreqChanged));
	//}	

	if(m_dwQuality)
	{
		UINT type;
		UINT err;
		UINT iTryCnt=5;

		//wait for rds data ready:
		Sleep(100);
		while(/*!bGetTAInfo && */iTryCnt>0)//check several times if rds data is received
		{
			m_dwLastProcessRDSTick = GetTickCount();

			err = GetRDSData(&type);

			if(err == ERR_RDS_NO_DATA)
			{
				s_iNoRdsCnt++;
				break;
			}
			else if(err == ERR_RDS_OK)
			{
				s_iNoRdsCnt=0;
				//Get rds data and push to UI:
				//PS://TP&TA
				if(type==RDS_GROUP_TYPE_A(0) || type==RDS_GROUP_TYPE_B(0)) //0A & 0B has PS information
				{
					if(m_bOnTAMode)
					{
						AnalyzePS(m_rdsInfo.PS,8);
					}
					bGetTAInfo=TRUE;
					if(m_rdsInfo.TA && m_rdsInfo.TP)
					{
						iDetectTAStationCnt++;
					}
				}
				//PTY:
				if(m_ppanelMain)
				{
					m_ppanelMain->SetPTY(ProgramType(m_rdsInfo.PTY));
				}
				//RT:	
				if(type==RDS_GROUP_TYPE_A(2) || type==RDS_GROUP_TYPE_B(2)) //2A & 2B has RT information
				{
					if(m_bOnTAMode)
					{
						AnalyzeRT(m_rdsInfo.RT,64);
					}
				}

				if(	type==RDS_GROUP_TYPE_A(0)||
					type==RDS_GROUP_TYPE_B(0)/*||
					type==RDS_GROUP_TYPE_B(14)||
					type==RDS_GROUP_TYPE_B(15)*/)
				{
					iTryCnt--;
				}
			}
			else
			{
				break;
			}

			//Sleep(20);

			DWORD dwcost=GetTickCount()-m_dwLastProcessRDSTick;
			if(dwcost>=m_dwRDSProcessTime)
			{
				//RETAILMSG(1,(_T("=====RDS TA process over time:%d\r\n"),dwcost));
			}
			else
			{
				WaitForSingleObject(m_hEvtRdsThread,m_dwRDSProcessTime-dwcost);
			}

			if(m_bAbortCurrentRDSProcess || !m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS)
			{
				RETAILMSG(1,(_T("=====RDS TA break...\r\n")));
				break;
			}

#if 0
			//check TA:
			if(!m_rdsInfo.TA || !m_rdsInfo.TP)
			{
				do
				{
					Sleep(100);
					err = GetRDSData(&type);
					if(err == ERR_RDS_NO_DATA)
					{
						break;
					}
					RETAILMSG(DEBUG_RDS,(_T("<%s> check TA, cnt=%d,TA=%d, TP=%d \r\n"),_T(__FUNCTION__),iTryCnt,m_rdsInfo.TA,m_rdsInfo.TP));
					if(m_rdsInfo.TA && m_rdsInfo.TP)
					{
						break;
					}
					if(err==ERR_RDS_OK &&
						(type==RDS_GROUP_TYPE_A(0)||
						type==RDS_GROUP_TYPE_B(0)||
						type==RDS_GROUP_TYPE_B(14)||
						type==RDS_GROUP_TYPE_B(15)))
					{
						iTryCnt--;
					}
				} while (iTryCnt>0/* && err==ERR_RDS_DECODE_ERR*/);
			}
#endif

		}
	}

// 	if(m_dwTACancelTick && GetTickCount() - m_dwTACancelTick < 15 * 60000)//user cancel TA, restart TA after 15 minutes
// 	{
// 		return FALSE;
// 	}
#if 0
	//test:
	if(m_dwQuality == 1)
	{
		bGetTAInfo=1;
		m_dwQuality=1;
		m_rdsInfo.TA=1;
		m_rdsInfo.TP=1;
		iDetectTAStationCnt=2;
		s_iNoRdsCnt=0;
	}
#endif

	//check if a TA station:
	if(bGetTAInfo)
	{
		//RETAILMSG(1,(_T("<%s> freq=%d, TA=%d, TP=%d, iDetectTAStationCnt=%d \r\n"),_T(__FUNCTION__),freq,m_rdsInfo.TA,m_rdsInfo.TP,iDetectTAStationCnt));
	}
	if(!m_bOnTAMode)
	{
		if(bGetTAInfo && m_dwQuality && m_rdsInfo.TA && m_rdsInfo.TP && iDetectTAStationCnt>1)
		{
			if(m_dwTACancelTick && GetTickCount() - m_dwTACancelTick < 15 * 60000)//user cancel TA, restart TA after 15 minutes
			{
				return FALSE;
			}
			else
			{
				OnTAStatus(1);
			}
		}
	}
	else
	{
		if((m_dwQuality == 0 || m_rdsInfo.TA == 0 || m_rdsInfo.TP == 0 || s_iNoRdsCnt>10) && GetTickCount()-m_dwTAEnterTick > 5000)
		{
			OnTAStatus(0);
		}
	}

	return TRUE;
}

BOOL CPanelRadioWrapper::RdsProcess()
{
	RETAILMSG(1/*DEBUG_RDS*/,(_T("<%s> ENTER...threadID=0x%x\r\n"),_T(__FUNCTION__),GetCurrentThreadId()));

// 	GetSignalQuality(TRUE);
// 	Sleep(100);

	while (m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS)
	{
		//UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
		DWORD dwThisTick=GetTickCount();
		BOOL bIniUIReady=m_pSkinManager->GetGUI()->IsIniUIReady();
		BOOL bTA=m_pSkinManager->GetConfig()->GetCurParam()->bRadioTA;
		BOOL bAF=m_pSkinManager->GetConfig()->GetCurParam()->bRadioAF;
		BOOL bRDSMonitor = (m_bEnableRDSProcess) 
			&& (m_bManualPlay)
			&& (m_idCurrentIdSrc != -1)
			&& (bIniUIReady)
			&& (m_idSystemPowerState==SYS_POWER_ON)
			&& (dwThisTick-m_dwRdsStartTick>10000)
			&& (dwThisTick-m_dwRadioCmdTick>2000)
			&& (dwThisTick-m_dwSRCChangeTick>3000)
			&& (!m_bMCUScanning) 
			&& (!m_bAbortCurrentRDSProcess)
			/*&& (!m_bPhoneCallActive)*/
			/*&& (!m_bOnReverseMode)*/
			/*&& (!m_bSpeechOn)*/
			&& (m_BandId==RADIO_BAND_ID_FM);
			//&&(m_RegionId==RADIO_REGION_Europe||m_RegionId==RADIO_REGION_USA);

		bRDSMonitor = (bRDSMonitor || m_bOnTAMode);

		//TA:
		if(bRDSMonitor && bTA && (m_bOnTAMode?TRUE:(GET_PANEL_CLASS_ID(m_idCurrentIdSrc) != UI_CLASS_RADIO && GET_PANEL_CLASS_ID(m_idCurrentIdSrc) != UI_CLASS_DAB)))
		{
			if(m_idRdsTask!=RDS_TASK_TA)
			{
				//WaitForSingleObject(m_hEvtRdsThread,5000);
				m_dwRdsTaskStartTick = GetTickCount();
				RETAILMSG(1,(_T("=====RDS Task: TA searching start\r\n")));
			}

			m_dwLastProcessRDSTick = GetTickCount();
			m_idRdsTask=RDS_TASK_TA;
			if(m_bTASearching)
			{
				if(!m_bSpeechOn && !m_bSROn && !m_bPhoneCallActive && !m_bOnReverseMode)
				{
					if(RdsTAProcess())
					{
						m_dwRDSProcessTime = GET_RDS_DATA_TIME;
					}
					else
					{
						m_dwRDSProcessTime = 2000;
					}
				}
				else
				{
					m_dwRDSProcessTime = 2000;
				}
			}
			else
			{
				m_bTASearching = TRUE;
				m_dwFreqBeforeTASearching = m_dwCurFreq;
			}
			//WaitForSingleObject(m_hEvtRdsThread,/*m_dwRDSProcessTime*/200);
			DWORD dwcost=GetTickCount()-m_dwLastProcessRDSTick;
			if(dwcost>=m_dwRDSProcessTime)
			{
				//RETAILMSG(1,(_T("=====RDS TA process over time:%d\r\n"),dwcost));
			}
			else
			{
				WaitForSingleObject(m_hEvtRdsThread,m_dwRDSProcessTime-dwcost);
			}
		}

		//AF:
		else if(bRDSMonitor && bAF && GET_PANEL_CLASS_ID(m_idCurrentIdSrc) == UI_CLASS_RADIO)
		{
			if(m_idRdsTask!=RDS_TASK_AF)
			{
				//WaitForSingleObject(m_hEvtRdsThread,5000);
				m_dwRdsTaskStartTick = GetTickCount();
				RETAILMSG(1,(_T("=====RDS Task: AF searching start\r\n")));
			}

			m_idRdsTask=RDS_TASK_AF;

			if(m_bTASearching)
			{
				if(m_dwFreqBeforeTASearching)
				{
					RETAILMSG(1,(_T("=====quit TA,recover freq:%d\r\n"),m_dwFreqBeforeTASearching));
					SetFrequency(m_dwFreqBeforeTASearching,FALSE);
					//Sleep(100);
					m_dwFreqBeforeTASearching = 0;
				}
			}
			m_bTASearching = FALSE;

			m_dwLastProcessRDSTick = GetTickCount();

			if(!m_bSpeechOn && !m_bSROn && !m_bPhoneCallActive && !m_bOnReverseMode)
			{
				if(RdsAFProcess())
				{
					m_dwRDSProcessTime = GET_RDS_DATA_TIME;
				}
				else
				{
					m_dwRDSProcessTime = 200;
				}
			}
			else
			{
				m_dwRDSProcessTime = 500;
			}

			DWORD dwcost=GetTickCount()-m_dwLastProcessRDSTick;
			if(dwcost>=m_dwRDSProcessTime)
			{
				//RETAILMSG(1,(_T("=====RDS AF process over time:%d\r\n"),dwcost));
			}
			else
			{
				WaitForSingleObject(m_hEvtRdsThread,m_dwRDSProcessTime-dwcost);
			}
		}

		//rds info process:
		else if(bRDSMonitor && GET_PANEL_CLASS_ID(m_idCurrentIdSrc) == UI_CLASS_RADIO)
		{
			DWORD dwstart=GetTickCount();
			if(m_idRdsTask!=RDS_TASK_COMMON)
			{
				//WaitForSingleObject(m_hEvtRdsThread,5000);
				m_dwRdsTaskStartTick = GetTickCount();
				RETAILMSG(1,(_T("=====RDS Task: common process start\r\n")));
				m_rdsInfo.userData = 0;
			}

			m_idRdsTask=RDS_TASK_COMMON;

			if(m_bTASearching)
			{
				if(m_dwFreqBeforeTASearching)
				{
					RETAILMSG(1,(_T("=====quit TA,recover freq:%d\r\n"),m_dwFreqBeforeTASearching));
					SetFrequency(m_dwFreqBeforeTASearching,FALSE);
					//Sleep(100);
					m_dwFreqBeforeTASearching = 0;
				}
			}
			m_bTASearching = FALSE;

			m_dwLastProcessRDSTick = GetTickCount();

			if(RdsCommonProcess())
			{
				m_dwRDSProcessTime = GET_RDS_DATA_TIME;
			}
			else
			{
				m_dwRDSProcessTime = 500;
			}

			DWORD dwcost=GetTickCount()-m_dwLastProcessRDSTick;
			if(dwcost>=m_dwRDSProcessTime)
			{
				//RETAILMSG(1,(_T("=====RDS common process over time:%d\r\n"),dwcost));
			}
			else
			{
				WaitForSingleObject(m_hEvtRdsThread,m_dwRDSProcessTime-dwcost);
			}
			m_rdsInfo.userData++;
		}
		#if CVTE_EN_DAB
		//DAB service link
		else if(bRDSMonitor && GET_PANEL_CLASS_ID(m_idCurrentIdSrc) == UI_CLASS_DAB)
		{
			if(m_idRdsTask!=RDS_TASK_DAB_LINK)
			{
				//WaitForSingleObject(m_hEvtRdsThread,5000);
				m_dwRdsTaskStartTick = GetTickCount();
				RETAILMSG(1,(_T("=====RDS Task: DAB link service start, freq=%d\r\n"),m_dwCurFreq));
				m_dwFreqBeforeDABLink = m_dwCurFreq;
			}

			m_idRdsTask=RDS_TASK_DAB_LINK;

			m_bDabLinkProgramFound = RdsDABServiceLinkProcess();
			WaitForSingleObject(m_hEvtRdsThread,m_bDabLinkProgramFound?2000:100);
		}
		#endif
		//no process:
		else
		{
			if(m_idRdsTask!=RDS_TASK_NULL)
			{
				RETAILMSG(1,(_T("=====RDS Task: no task\r\n")));
				//clear rds data:
				memset(&m_rdsInfo.AF,0,sizeof(AFData));
				memset(&m_rdsInfo,0,sizeof(RdsInfo));
				memset(&m_AFData,0,sizeof(AFData));
				RETAILMSG(1,(_T("=====clear RDS data\r\n")));
				m_dwQualityCntNG = 0;
				m_dwQualityCntGood = 0;
				m_dwRdsDataLossCnt = 0;
				m_dwRdsDataErrorCnt = 0;
				m_dwRdsDataRequestCnt = 0;
				m_curPIForAF = 0xFFFF;
				m_bAutoUpdateAFinfo = FALSE;
				m_bNeedDoPISearching = TRUE;

#if RDS_ENABLE_PI_SEARCHING
				//un-mute radio output:
				if(m_bPISearching)
				{
					RETAILMSG(1,(_T("=====quit PI Searching,recover freq:%d\r\n"),m_dwFreqPISearchStart));

					if(m_ppanelMain)
						m_ppanelMain->SetAFSearching(FALSE);
					if(m_ppanelLibrary)
						m_ppanelLibrary->SetAFSearching(FALSE);

					SetFrequency(m_dwFreqPISearchStart);
					if(m_pCommander)
					{
						m_pCommander->SendCmd(_ARM_RADIO_PLAY_STATE,0);
					}
				}
				m_bPISearching = FALSE;
#endif

				m_bNeedScanAllAFList = TRUE;
				m_dwAFSearchInterval = RDS_AF_SEARCH_INTERVAL_MIN;
			}
			m_idRdsTask = RDS_TASK_NULL;
			if(m_bTASearching)
			{
				if(m_dwFreqBeforeTASearching)
				{
					RETAILMSG(1,(_T("=====quit TA,recover freq:%d\r\n"),m_dwFreqBeforeTASearching));
					SetFrequency(m_dwFreqBeforeTASearching,FALSE);
					//Sleep(100);
					m_dwFreqBeforeTASearching = 0;
				}
			}
			m_bTASearching = FALSE;
			//recover freq before DAB ink
			if(m_dwFreqBeforeDABLink)
			{
				RETAILMSG(1,(_T("=====quit DAB link,recover freq:%d\r\n"),m_dwFreqBeforeDABLink));
				SetFrequency(m_dwFreqBeforeDABLink,FALSE);
				m_dwFreqBeforeDABLink = 0;
			}
			//process delay command:
			if(m_idRadioCmdType)
			{
				OnRadioCommand(m_idRadioCmdType,m_idRadioCmdPara);
				m_idRadioCmdType = 0;
				m_idRadioCmdPara = 0;
			}
			if(m_bAbortCurrentRDSProcess)
			{
				SetEvent(m_hEvtRdsAbort);
				m_bAbortCurrentRDSProcess = FALSE;
			}
			WaitForSingleObject(m_hEvtRdsThread,1000);
		}
	}

	//reset parameters:
	m_bTASignaled = 0;
	m_bTAMonitor = FALSE;
	m_bOnTAMode = FALSE;
	m_bTADetected = FALSE;
	m_bTASearching = FALSE;
	m_bQuickTAMode = FALSE;

	RETAILMSG(1/*DEBUG_RDS*/,(_T("<%s> QUIT...\r\n"),_T(__FUNCTION__)));

	return TRUE;
}

DWORD CPanelRadioWrapper::RdsThreadPro(LPVOID lpParam)
{
	CPanelRadioWrapper *panel=(CPanelRadioWrapper*)lpParam;

	//Sleep(3000);

	panel->RdsProcess();

	return 0;
}

BOOL CPanelRadioWrapper::SetFrequencyAndGetSignal(DWORD dwFreq)
{
	m_dwFreqSet = dwFreq;
	if(m_pCommander)
	{
		RETAILMSG(DEBUG_RDS,(_T("****<%s>\r\n"),_T(__FUNCTION__)));
		ResetEvent(m_hEvtRadioSignal);
		int high=dwFreq/1000000;
		int low=dwFreq/10000-dwFreq/1000000*100;	
		
		m_pCommander->SendCmd(_ARM_RADIO_TUNE_MODE,FREQ_MODE_PRESET);
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_H_KEY,(BYTE)(high));
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_L_KEY,(BYTE)(low));

		return (WaitForSingleObject(m_hEvtRadioSignal,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtRadioSignal);
	}
	else
	{
		return FALSE;
	}
}
BOOL CPanelRadioWrapper::GetSignalData(DWORD dwFreq)
{
	if(m_pCommander)
	{
		RETAILMSG(DEBUG_RDS,(_T("****<%s> %d\r\n"),_T(__FUNCTION__),dwFreq));
		m_dwFreqRequestSingal = dwFreq;
		ResetEvent(m_hEvtRdsData);
		m_pCommander->SendCmd(_ARM_RADIO_QUERY_SIGNAL_DATA,0);
		if(!(WaitForSingleObject(m_hEvtRdsData,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtRdsData))
		{
			RETAILMSG(DEBUG_ERR,(_T("****<%s> %d QUERY_SIGNAL_DATA FAILED!\r\n"),_T(__FUNCTION__),dwFreq));
			return FALSE;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CPanelRadioWrapper::DetectAFSignal(DWORD dwFreq)
{
	if(m_pCommander)
	{
		RETAILMSG(DEBUG_RDS,(_T("****<%s> %d\r\n"),_T(__FUNCTION__),dwFreq));
		
		int high=dwFreq/1000000;
		int low=dwFreq/10000-dwFreq/1000000*100;	

		m_dwFreqRequestSingal = dwFreq;

		m_pCommander->SendCmd(_ARM_RADIO_TUNE_MODE,FREQ_MODE_AF_UPDATE);
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_H_KEY,(BYTE)(high));
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_L_KEY,(BYTE)(low));

		//get signal status:
		ResetEvent(m_hEvtRadioSignal);
		Sleep(30);
		m_pCommander->SendCmd(_ARM_RADIO_QUERY_SIGNAL_STATUS,0);
		if(!(WaitForSingleObject(m_hEvtRadioSignal,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtRadioSignal))
		{
			RETAILMSG(DEBUG_ERR,(_T("****<%s> %d QUERY_SIGNAL_STATUS FAILED!\r\n"),_T(__FUNCTION__),dwFreq));
			return FALSE;
		}
		if(m_bAbortCurrentRDSProcess)
			return FALSE;
		//get signal data:
		ResetEvent(m_hEvtRdsData);
		m_pCommander->SendCmd(_ARM_RADIO_QUERY_SIGNAL_DATA,0);
		if(!(WaitForSingleObject(m_hEvtRdsData,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtRdsData))
		{
			RETAILMSG(DEBUG_ERR,(_T("****<%s> %d QUERY_SIGNAL_DATA FAILED!\r\n"),_T(__FUNCTION__),dwFreq));
			return FALSE;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CPanelRadioWrapper::JumpAF(DWORD dwFreq)
{
	if(m_pCommander)
	{
		RETAILMSG(1/*DEBUG_RDS*/,(_T("****<%s> %d\r\n"),_T(__FUNCTION__),dwFreq));

		int high=dwFreq/1000000;
		int low=dwFreq/10000-dwFreq/1000000*100;	

		m_bJumpAFFlag = TRUE;

		m_pCommander->SendCmd(_ARM_RADIO_TUNE_MODE,FREQ_MODE_AF_JUMP);
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_H_KEY,(BYTE)(high));
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_L_KEY,(BYTE)(low));

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CPanelRadioWrapper::CheckAF(DWORD dwFreq)
{
	if(m_pCommander)
	{
		RETAILMSG(1/*DEBUG_RDS*/,(_T("****<%s> %d\r\n"),_T(__FUNCTION__),dwFreq));

		int high=dwFreq/1000000;
		int low=dwFreq/10000-dwFreq/1000000*100;	

		m_pCommander->SendCmd(_ARM_RADIO_TUNE_MODE,FREQ_MODE_AF_CHECK);
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_H_KEY,(BYTE)(high));
		m_pCommander->SendCmd(_ARM_RADIO_FREQ_L_KEY,(BYTE)(low));

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
WORD CPanelRadioWrapper::CheckPI(DWORD dwFreq, int iRetry)
{
	UINT err,type;
	int cnt = 0;
	WORD ret = 0;
	int idx = dwFreq/100000 - 875;
	DWORD dwstart=GetTickCount();
	do 
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hEvtRdsThread,cnt?10:80))
		{
			//user abort.
			break;
		}

		if(m_bAbortCurrentRDSProcess)
			break;

		err = GetRDSData(&type);
		cnt++;
		if(err == ERR_RDS_OK || err == ERR_RDS_PI_ONLY)
		{
			ret = m_rdsInfo.PICode;
			break;
		}
		else if(err == ERR_RDS_ABORT)
		{
			break;
		}
	} while (cnt < iRetry);

	if(idx>=0 && idx<=205)
	{
		m_radioSignalData[idx].PICode = ret;
	}

	RETAILMSG(1/*DEBUG_RDS*/,(_T("<CheckPI> %d, err=%d, ret=0x%x, try=[%d,%d], t=%dms\r\n"),dwFreq,err,ret,cnt,iRetry,GetTickCount()-dwstart));

	return ret;
}
#endif
BOOL CPanelRadioWrapper::SetFrequency(DWORD dwFreq,BOOL bSync)
{
	RETAILMSG(DEBUG_RDS,(_T("<%s> %d\r\n"),_T(__FUNCTION__),dwFreq));
	m_dwFreqSet = dwFreq;
	if(m_pCommander)
	{
		if(m_BandId == RADIO_BAND_ID_FM)
		{
			ResetEvent(m_hEvtFreq);
			int high=dwFreq/1000000;
			int low=dwFreq/10000-dwFreq/1000000*100;
			m_pCommander->SendCmd(_ARM_RADIO_TUNE_MODE,FREQ_MODE_PRESET);
			m_pCommander->SendCmd(_ARM_RADIO_FREQ_H_KEY,(BYTE)(high));
			m_pCommander->SendCmd(_ARM_RADIO_FREQ_L_KEY,(BYTE)(low));
			if(bSync)
				return (WaitForSingleObject(m_hEvtFreq,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtFreq);
			else
				return TRUE;
		}
		else if(m_BandId == RADIO_BAND_ID_AM)
		{
			ResetEvent(m_hEvtFreq);
			int high=dwFreq/100000;
			int low=dwFreq/1000-dwFreq/100000*100;
			m_pCommander->SendCmd(_ARM_RADIO_TUNE_MODE,FREQ_MODE_PRESET);
			m_pCommander->SendCmd(_ARM_RADIO_FREQ_H_KEY,(BYTE)(high));
			m_pCommander->SendCmd(_ARM_RADIO_FREQ_L_KEY,(BYTE)(low));
			if(bSync)
				return (WaitForSingleObject(m_hEvtFreq,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtFreq);
			else
				return TRUE;
		}
	}

	return TRUE;
}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
BOOL CPanelRadioWrapper::LoadRDSDatabase()
{
#if 0
	//for test:
	//create new:
	RdsInfo *sta=new RdsInfo;
	memset(sta,0,sizeof(RdsInfo));
	sta->PICode = 0xeeee;
	sta->AF.AFFreq[0]=1;
	sta->AF.AFFreq[1]=2;
	sta->AF.AFFreq[2]=45;
	sta->AF.AFFreq[3]=88;
	sta->AF.AFFreq[4]=123;
	sta->AF.AFFreq[5]=126;
	sta->AF.AFFreq[6]=143;
	sta->AF.AFFreq[7]=188;
	sta->AF.AFFilledNum=8;
	sta->AF.AFTotalNum=8;

	m_listRdsDatabase.Add(sta,sta->PICode);
	m_bRdsDatabaseChanged=TRUE;
#endif

	FILE					*file = NULL;
	int					fileSize = 0;
	BYTE				*localBuffer;
	BYTE				*cursor;

	DWORD dwStart=GetTickCount();

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RdsDatabase.dat"));

	RETAILMSG(1/*DEBUG_RADIO*/,(_T("\r\n**Begin LoadRDSDatabase.%s \r\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"rb") )
	{
		RETAILMSG(DEBUG_RADIO,(_T("ERROR: radio data file not found,%s\n"), strFile.String()));
		return  FALSE;
	}

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	localBuffer = new BYTE[fileSize];
	if(!localBuffer)
	{
		fclose(file);
		return FALSE;
	}

	fread(localBuffer, fileSize, 1, file);
	cursor = localBuffer;

	fclose(file);

	//para data:
	int count=0;
	int siInfo=sizeof(RdsInfo);

	while ((cursor-localBuffer)<=fileSize-siInfo)
	{
		RdsInfo *pinfo = new RdsInfo;
		memcpy(pinfo,cursor,siInfo);

		m_listRdsDatabase.Add(pinfo,pinfo->PICode);

		cursor+=siInfo;
		count++;
	}

	delete[] localBuffer;

	RETAILMSG(1/*DEBUG_RADIO*/,(_T("\r\n**End LoadRDSDatabase, total count:%d,  take %d ms \r\n"),count,GetTickCount()-dwStart));

	return TRUE;
}
BOOL CPanelRadioWrapper::SaveRDSDatabase()
{
	if(m_bRdsDatabaseChanged)
	{
		m_bRdsDatabaseChanged = FALSE;

		RETAILMSG(1,(_T("**<%s> cnt=%d \r\n"),_T(__FUNCTION__),m_listRdsDatabase.GetCount()));

		DWORD dwStart=GetTickCount();
		FILE	*file = NULL;

		CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("Radio\\"));

		CreateDirectory(strPath.String(),NULL);

		CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RdsDatabase.dat"));


		if ( 0!=_wfopen_s(&file, strFile.String(), L"wb") )
		{
			return FALSE;
		}

		m_listRdsDatabase.Head();
		while (!m_listRdsDatabase.End())
		{
			fwrite(m_listRdsDatabase.Get(),sizeof(RdsInfo),1,file);
			m_listRdsDatabase.Next();
		}

		fclose(file);

		RETAILMSG(1/*DEBUG_RADIO*/,(_T("**<%s> finish,  take %d ms************* \r\n"),_T(__FUNCTION__),GetTickCount()-dwStart));
	}

	return TRUE;
}
RdsInfo* CPanelRadioWrapper::SearchDatabaseByAF(DWORD dwAFFreq)
{
	RdsInfo *sta = NULL;
	int idx=dwAFFreq/100000 - 875;

	if(idx>=0 && idx<=205)
	{
		m_listRdsDatabase.Head();
		while (!m_listRdsDatabase.End())
		{
			sta = m_listRdsDatabase.Get();
			for (int i=0;i<sta->AF.AFFilledNum;i++)
			{
				if(idx == sta->AF.AFFreq[i])
				{
					return sta;
					break;
				}
			}
			m_listRdsDatabase.Next();
		}
	}
	return NULL;
}
void CPanelRadioWrapper::UpdateRDSDatabase(RdsInfo *prdsInfo)
{
	static DWORD s_AFFilledNum = 0;

	if(!prdsInfo)
		return;

	RdsInfo *pSearched=m_listRdsDatabase.GetItemByID(prdsInfo->PICode);
	if(pSearched)
	{
		//already exist:
		//memcpy(pSearched,prdsInfo,sizeof(RdsInfo));
		pSearched->freq = prdsInfo->freq;
		pSearched->userData = prdsInfo->userData;

		pSearched->PIN = prdsInfo->PIN;
		pSearched->TP = prdsInfo->TP;
		pSearched->TA = prdsInfo->TA;
		pSearched->PTY = prdsInfo->PTY;
		pSearched->DI = prdsInfo->DI;
		pSearched->MS = prdsInfo->MS;

		//update AF list, make sure update correct data
		//check if the AF list is changed:
		BOOL bAFChanged=FALSE;
		if(prdsInfo->AF.AFFilledNum > pSearched->AF.AFFilledNum)
		{
			bAFChanged = TRUE;
		}
		else if(prdsInfo->AF.AFFilledNum && prdsInfo->AF.AFFilledNum != s_AFFilledNum)
		{
			s_AFFilledNum = prdsInfo->AF.AFFilledNum;
			for (int i=0;i<prdsInfo->AF.AFFilledNum;i++)
			{
				BOOL bExist=FALSE;
				for (int m=0;m<pSearched->AF.AFFilledNum;m++)
				{
					if(prdsInfo->AF.AFFreq[i] == pSearched->AF.AFFreq[m])
					{
						//RETAILMSG(1,(_T("update AF list, AFFreq[%d] exist.\r\n"),prdsInfo->AF.AFFreq[i]));
						bExist = TRUE;
					}
				}
				if(!bExist)
				{
					bAFChanged = TRUE;
					break;
				}
			}
		}
		if(bAFChanged)
		{
			RETAILMSG(1,(_T("<%s> update AF list, PI=0x%x, AFFilledNum=%d\r\n"),_T(__FUNCTION__),prdsInfo->PICode,prdsInfo->AF.AFFilledNum));
			memcpy(&pSearched->AF,&prdsInfo->AF,sizeof(AFData));
			m_bRdsDatabaseChanged = TRUE;
		}
	}
	else if(prdsInfo->PICode != 0)
	{
		//new
		RdsInfo *pNew = new RdsInfo;
		memcpy(pNew,prdsInfo,sizeof(RdsInfo));
		m_listRdsDatabase.Add(pNew,pNew->PICode);
		m_bRdsDatabaseChanged = TRUE;
	}

	//update other info:
	if(m_ppanelMain && prdsInfo->freq)
	{
		m_ppanelMain->SetPI(prdsInfo->freq,prdsInfo->PICode);
	}

	//update PI in signal data:
	if(prdsInfo->freq && prdsInfo->PICode)
	{
		int idx = prdsInfo->freq/100000 - 875;
		if(idx>=0 && idx<=205)
		{
			m_radioSignalData[idx].PICode = prdsInfo->PICode;
		}
	}

}

UINT	 CPanelRadioWrapper::GetRDSData(UINT *pRdsDataType,BOOL *pbDataLoss)
{
	UINT ret=ERR_RDS_NO_DATA;

	if(!GetRDSStatus())
	{
		RETAILMSG(DEBUG_ERR,(_T("<%s> GetRDSStatus FAILED!\r\n"),_T(__FUNCTION__)));
		ret= ERR_RDS_ABORT;
		return ret;
	}
	else
	{
		if(m_rdsStatus & 0x4000)
		{
			if(pbDataLoss)
				*pbDataLoss = TRUE;
			//RETAILMSG(1,(_T("!!!RDS LOSS: 0x%x\r\n"),m_rdsStatus));
		}
	}

	if((m_rdsStatus & 0x8000) && (m_rdsStatus & 0x0200))//rds data received
	{
		if(!GetRDSBlockData())
		{
			RETAILMSG(DEBUG_ERR,(_T("<%s> GetRDSBlockData FAILED!\r\n"),_T(__FUNCTION__)));
			ret= ERR_RDS_ABORT;
			return ret;
		}

		m_rdsInfo.PICode = 0xFFFF; //reset to invalid PI
		if(RdsParse(m_dwCurFreq,&m_rdsData,&m_rdsInfo))
		{
			if(pRdsDataType)
			{
				*pRdsDataType = RdsGetGroupType(&m_rdsData);
			}
			ret= ERR_RDS_OK;

			//update RDS data base:
			UpdateRDSDatabase(&m_rdsInfo);
		}
		else if(m_rdsInfo.PICode != 0xFFFF)
		{
			ret= ERR_RDS_PI_ONLY;
		}
		else
		{
			//RETAILMSG(1,(_T("!!!RDS DECODE_ERR\r\n")));
			ret= ERR_RDS_DECODE_ERR;
		}
	}

	return ret;
}
BOOL CPanelRadioWrapper::GetRDSStatus()
{
	//RETAILMSG(DEBUG_RDS,(_T("****<%s>\r\n"),_T(__FUNCTION__)));

	if(m_pCommander)
	{
		ResetEvent(m_hEvtRdsData);
		m_pCommander->SendCmd(_ARM_RADIO_QUERY_RDS_STATUS,0);
		return (WaitForSingleObject(m_hEvtRdsData,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtRdsData);
	}
	else
	{
		return FALSE;
	}
}
BOOL CPanelRadioWrapper::GetRDSBlockData()
{
	//RETAILMSG(DEBUG_RDS,(_T("****<%s>\r\n"),_T(__FUNCTION__)));

	if(m_pCommander)
	{
		ResetEvent(m_hEvtRdsData);
		m_pCommander->SendCmd(_ARM_RADIO_QUERY_RDS_BLOCK_DATA,0);
		return (WaitForSingleObject(m_hEvtRdsData,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtRdsData);
	}
	else
	{
		return FALSE;
	}
}

BOOL CPanelRadioWrapper::GetSignalQuality(BOOL bSync)
{
	//RETAILMSG(DEBUG_RDS,(_T("****<%s>\r\n"),_T(__FUNCTION__)));

	if(m_pCommander)
	{
		if(GetTickCount() - m_dwGetSignalQualityTick > 1000 || bSync)
		{
			RETAILMSG(DEBUG_RDS,(_T("****<%s>\r\n"),_T(__FUNCTION__)));
			ResetEvent(m_hEvtRdsData);
			m_pCommander->SendCmd(_ARM_RADIO_QUERY_QUALITY,0);
			m_dwGetSignalQualityTick = GetTickCount();
			//RETAILMSG(1,(_T("MSG:GetSignalQuality \r\n")));

			return (WaitForSingleObject(m_hEvtRdsData,200) == WAIT_OBJECT_0) && GetEventData(m_hEvtRdsData);
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}

#endif

void CPanelRadioWrapper::OnReverseModeChange(BOOL bReverse)
{
	//RETAILMSG(DEBUG_RADIO,(_T("CPanelRadioWrapper::OnReverseModeChange: %d\r\n"),bReverse));

	CPanelGL::OnReverseModeChange(bReverse);
}
