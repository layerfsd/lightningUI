#include "PanelDABWrapper.h"
#include "DABCommon.h"
#include "../../resource.h"

#if CVTE_EN_DAB

CPanelDABWrapper::CPanelDABWrapper(void)
{
	m_ppLibrary = NULL;
	m_ppMain = NULL;
	m_hDABThread = NULL;

	m_bDABScaning = FALSE;

	m_dwDABCmdTick = 0;
	m_dwDABLibUpdateTick = 0;
	m_dwDABScaningTick = 0;
	m_bNeedUpdateAfterScaning = FALSE;
	m_bIniStatus = FALSE;
	m_bDABServiceStart = FALSE;//TRUE;
	m_bPhoneCallActive = FALSE;
}
CPanelDABWrapper::~CPanelDABWrapper(void)
{

}
void CPanelDABWrapper::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_DAB)
	{
		if(wParam)//active
		{

		}
		else
		{
			//if(!m_bSpeechOn)
			{
				SwitchAudioSRC(SRC_AUDIO_DAB,TRUE);
			}
		}
	}
}

void CPanelDABWrapper::Render()
{
	if(!BeginRender())return;


	EndRender();
}

BOOL CPanelDABWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_hEvtDABThread = CreateEvent(NULL,FALSE,FALSE,NULL);

	//if(m_pSkinManager->GetConfig()->GetCurParam()->bDABEnable)
	{
		if(!CDABAdapter::GetInstance()->GetInitialize())
		{
			RETAILMSG(DEBUG_DAB,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: Initialize DABAdapter \r\n")));
			CDABAdapter::GetInstance()->Initialize(5,/*230400*/38400);
		}
		EnableDAB(TRUE);
	}

	//ini-settings:
	//SetLBandSetting(TRUE);

	return TRUE;
}

void CPanelDABWrapper::EnableDAB(BOOL Enable)
{
	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s] :%d\r\n"),TEXT(__FUNCTION__),Enable));

	m_pSkinManager->GetConfig()->GetCurParam()->bDABEnable = Enable;
	if(Enable)
	{
		//begin thread for rds:
		if(m_hDABThread == NULL)
		{
			m_hDABThread=CreateThread(NULL,NULL,DABThreadPro,this,NULL,NULL);
			//CeSetThreadPriority(m_hDABThread, CE_THREAD_PRIO_256_IDLE);
		}
	}
	else
	{
		//terminate DAB thread:
		if(m_hDABThread != NULL)
		{
			SetEvent(m_hEvtDABThread);
			//wait quit:
			WaitForSingleObject(m_hDABThread,3000);
			CloseHandle(m_hDABThread);
			m_hDABThread = NULL;
		}
	}
}

#if CVTE_EN_KEY_PROXY
BOOL CPanelDABWrapper::OnKey(UINT idKey,UINT idStatus)
{
	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s] :%d, idStatus: %d\r\n"),TEXT(__FUNCTION__),idKey,idStatus));

	switch (idKey)
	{
	case KEY_NEXT:
		if ( idStatus == EV_KEY_SHORT_PRESS )
		{
			PlayNext();
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SEEK_NEXT);
		}
		break;
	case KEY_PREVIOUS:
		if ( idStatus == EV_KEY_SHORT_PRESS )
		{
			PlayPrevious();
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			SendDABCommand(DAB_CMD_PLAY_CONTROL,DAB_CONTROL_SEEK_PREVIOUS);
		}
		break;
	case KEY_PLAY:
	case KEY_PLAYPAUSE:
	case KEY_RESUME:
		if ( idStatus == EV_KEY_UP )
		{
			if(!m_bPhoneCallActive)
			{
				SwitchAudioSRC(SRC_AUDIO_DAB,TRUE);
			}
		}
		break;
	default:
		break;
	}	
	return TRUE;
}
#else
BOOL CPanelDABWrapper::OnKey(UINT idKey)
{
	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s] :%d\r\n"),TEXT(__FUNCTION__),idKey));
	switch (idKey)
	{
	case KEY_MCU_TRACK_NEXT:
	case KEY_MEDIA_NEXT:
		PlayNext();
		break;
	case KEY_MCU_TRACK_PREV:
	case KEY_MEDIA_PREVIOUS:
		PlayPrevious();
		break;
	//case KEY_MEDIA_RESUME:
	case KEY_MEDIA_PLAY:
	case KEY_PLAYPAUSE:
	case KEY_RESUME:
		{
			SwitchAudioSRC(SRC_AUDIO_DAB,TRUE);
		}
		break;
	case KEY_MEDIA_STOP:
		{
		}
		break;
	case KEY_MEDIA_PAUSE:
		{
		}
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		{
		}
		break;
	}

	return TRUE;
}
#endif


DWORD CPanelDABWrapper::DABThreadPro(LPVOID lpParam)
{
	CPanelDABWrapper *panel=(CPanelDABWrapper*)lpParam;

	//Sleep(3000);

	panel->DABProcess();

	return 0;
}

BOOL CPanelDABWrapper::DABProcess()
{
	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: ++\r\n"),TEXT(__FUNCTION__)));

	while (m_pSkinManager->GetConfig()->GetCurParam()->bDABEnable)
	{
//			BOOL bIniUIReady=m_pSkinManager->GetGUI()->IsIniUIReady();
//			BOOL bTA=m_pSkinManager->GetConfig()->GetCurParam()->bRadioTA;
//			BOOL bAF=m_pSkinManager->GetConfig()->GetCurParam()->bRadioAF;
//			BOOL bRDSMonitor;

		BOOL bStart = (CDABAdapter::GetInstance()->GetInitialize())?(GetTickCount()-CDABAdapter::GetInstance()->GetStartTick()>10000):FALSE;

		if(m_bDABServiceStart != bStart)
		{
			m_bDABServiceStart = bStart;
			RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: m_bDABServiceStart=%d\r\n"),TEXT(__FUNCTION__),m_bDABServiceStart));
			if(m_ppLibrary)
			{
				m_ppLibrary->NotifyDabServiceReady(m_bDABServiceStart);
			}
			if(m_ppMain)
			{
				m_ppMain->NotifyDabServiceReady(m_bDABServiceStart);
			}
			//notify GUI:
			::PostMessage(m_hMainWnd,WM_DAB_SERVICE_START_READY,bStart,0);
		}


		if(DABApi_GetFlag(DAB_ViewFlag_ScanStatus))
		{
			m_bDABScaning = TRUE;
			//Do something
			RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: Scaning...\r\n"), TEXT(__FUNCTION__)));
			UpdateScanStatus(TRUE);
			//Clear flag
			DABApi_SetFlag(DAB_ViewFlag_ScanStatus, 0);
		}
		else
		{
			if(m_bDABScaning)
			{
				m_dwDABScaningTick = GetTickCount();
				m_bNeedUpdateAfterScaning = TRUE;
				m_bDABScaning = FALSE;
				//UpdateScanStatus(FALSE);
			}

			//update after scanning
			if(m_bNeedUpdateAfterScaning && GetTickCount()-m_dwDABScaningTick > 3000)
			{
				m_bNeedUpdateAfterScaning = FALSE;
				UpdateScanStatus(FALSE);

				RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: UpdateAfterScaning...\r\n"), TEXT(__FUNCTION__)));

				if(m_ppLibrary)
				{
					m_ppLibrary->UpdateAllServiceList();
				}
			}
		}

		//play info:
		if(DABApi_GetFlag(DAB_ViewFlag_PlayInfoUpdate))
		{
			if(m_ppMain)
			{
				m_ppMain->UpdateCurChannelInfo();
			}

			if (IsPlayInfoValid())
			{
				//initial status:
				if(!m_bIniStatus)
				{
					m_bIniStatus = TRUE;
					if(CDABAdapter::GetInstance()->HasData())
					{
						if(m_ppLibrary)
						{
							m_ppLibrary->UpdateCurChannelInfo();
							m_ppLibrary->UpdateAllServiceList();
						}
						if(m_ppMain)
						{
							m_ppMain->UpdateBandlInfo();
							m_ppMain->UpdateCurChannelInfo();
							if(m_ppLibrary)
							{
								m_ppMain->SetNowPlayingIndex(m_ppLibrary->GetCurIndex(),m_ppLibrary->GetTotalServiceCnt());
							}
						}
					}
				}

				if(m_ppLibrary)
				{
					m_ppLibrary->UpdateCurChannelInfo();
				}
				if(m_ppMain && m_ppLibrary)
				{
					m_ppMain->SetNowPlayingIndex(m_ppLibrary->GetCurIndex(),m_ppLibrary->GetTotalServiceCnt());
				}
			}
			DABApi_SetFlag(DAB_ViewFlag_PlayInfoUpdate, 0);
		}

		if(!m_bDABScaning)
		{
			//Band update
			if(DABApi_GetFlag(DAB_ViewFlag_BandUpdate))
			{
				if(m_ppMain)
				{
					m_ppMain->UpdateBandlInfo();
				}
				DABApi_SetFlag(DAB_ViewFlag_BandUpdate, 0);
			}

			//TA update
			if(DABApi_GetFlag(DAB_ViewFlag_TAStatusUpdate))
			{
				//
				DABApi_SetFlag(DAB_ViewFlag_TAStatusUpdate, 0);
			}

			//SNR update
			if(DABApi_GetFlag(DAB_ViewFlag_SNRValueUpdate))
			{
				//
				if(m_ppMain)
				{
					m_ppMain->UpdateSignal();
				}
				DABApi_SetFlag(DAB_ViewFlag_SNRValueUpdate, 0);
			}
		}

		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hEvtDABThread,250))
		{
			//break;
		}
	}

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: -- \r\n"),TEXT(__FUNCTION__)));

	return TRUE;
}
BOOL CPanelDABWrapper::IsPlayInfoValid()
{
	wchar_t *name = DABApi_GetCurrChanInfo( ChnInfo_ServiceLabel );
	return (name && wcslen(name));
}
//	void CPanelDABWrapper::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
//	{
//	//		if(m_ppLibrary)
//	//		{
//	//			m_ppLibrary->OnAvailableSourceChanged(wParam,lParam);
//	//		}
//	}
BOOL CPanelDABWrapper::OnDABCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;
	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: %d,%d\r\n"),TEXT(__FUNCTION__), wParam,lParam));

	//Param check
	switch(wParam)
	{
		case DAB_CMD_PLAY_INDEX:
		case DAB_CMD_PLAY_CONTROL:
		case DAB_CMD_REFRESH_LIBRARY:
		case DAB_CMD_SWITCH:
		case DAB_CMD_ANTENNA_POWER:
			m_dwDABCmdTick = GetTickCount();
			break;
		default:
			RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: param check failed. wParam = %d \r\n"),TEXT(__FUNCTION__), wParam));
			return FALSE;
	}

	switch (wParam)
	{
	case DAB_CMD_ANTENNA_POWER:
		{
			if(m_pCommander)
			{
				RETAILMSG(1,(_T("===DAB set ANTENNA_POWER=%d\r\n"),lParam));
				m_pCommander->SendCmd(_ARM_DAB_ANT_POWER_KEY,lParam?1:0);
				m_pSkinManager->GetConfig()->GetCurParam()->bDABAntPowerOn = (lParam?1:0);
			}
		}
		break;
	case DAB_CMD_PLAY_INDEX:
		{
			bRet = PlayIndex(lParam);
		}
		break;
	case DAB_CMD_PLAY_CONTROL:
		{
			switch (lParam)
			{
				case DAB_CONTROL_NEXT:
					bRet = PlayNext();
					break;
				case DAB_CONTROL_PREVIOUS:
					bRet = PlayPrevious();
					break;
				case DAB_CONTROL_SEEK_NEXT:
					bRet = SeekNext();
					break;
				case DAB_CONTROL_SEEK_PREVIOUS:
					bRet = SeekPrevious();
					break;
				case DAB_CONTROL_SCAN_PLAY_START:
					bRet = ScanServicePlay(TRUE);
					break;
				case DAB_CONTROL_SCAN_PLAY_STOP:
					bRet = ScanServicePlay(FALSE);
					break;
				case DAB_CONTROL_CHANGE_BAND:
					bRet = ChangeBand();
					break;
				case DAB_CONTROL_CHANGE_SEARCH_TYPE:
					bRet = ChangeSearchType();
					break;
				case DAB_CONTROL_SCAN_AUTO_STORE:
					bRet = AutoScanStore();
					break;
				case DAB_CONTROL_SCAN_SERVICE:
					bRet = ScanService();
					break;
			}
		}
		break;
	case DAB_CMD_REFRESH_LIBRARY:
		{
			DAB_LIBRARY_TYPE nIndex = (DAB_LIBRARY_TYPE)lParam;
			RefreshLibrary(nIndex);
			bRet = TRUE;
		}
		break;
	case DAB_CMD_SWITCH:
		{
			EnableDAB((BOOL)lParam);
			bRet = TRUE;
		}
		break;
	default:
		break;
	}
	return bRet;
}

void CPanelDABWrapper::LinkPages(CPanelDABMain *pMain, CPanelDABLibrary *pLibrary)
{
	m_ppMain = pMain;
	m_ppLibrary = pLibrary;
}

void CALLBACK CPanelDABWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelDABWrapper *panel=(CPanelDABWrapper*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelDABWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
}

BOOL CPanelDABWrapper::PlayIndex(INT index)
{
	if(m_ppLibrary)
	{
		return m_ppLibrary->PlayItemAllService(index);
	}
	return FALSE;
}

BOOL CPanelDABWrapper::SeekNext()
{
	FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchType));
	FUNC_CHECK_BOOL(DABApi_SeekDownProc(m_nSearchType/*Manual_Search*/));
	return TRUE;
}

BOOL CPanelDABWrapper::SeekPrevious()
{
	FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchType));
	FUNC_CHECK_BOOL(DABApi_SeekUpProc(m_nSearchType/*Manual_Search*/));
	return TRUE;
}

BOOL CPanelDABWrapper::PlayNext()
{
	if (IsPlayInfoValid())
	{
		FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchType));
		FUNC_CHECK_BOOL(DABApi_ChanDownProc(m_nSearchType));
	}
	else
	{
		FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchType));
		FUNC_CHECK_BOOL(DABApi_SeekDownProc(m_nSearchType));
	}

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: GetCurrentSearchMode=%d\r\n"), TEXT(__FUNCTION__),m_nSearchType));

// 	if(m_ppLibrary)
// 	{
// 		return m_ppLibrary->PlayNext();
// 	}
	return FALSE;
}
BOOL CPanelDABWrapper::PlayPrevious()
{
	if (IsPlayInfoValid())
	{	
		FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchType));
		FUNC_CHECK_BOOL(DABApi_ChanUpProc(m_nSearchType));
	}
	else
	{
		FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchType));
		FUNC_CHECK_BOOL(DABApi_SeekUpProc(m_nSearchType));
	}


// 	if(m_ppLibrary)
// 	{
// 		return m_ppLibrary->PlayPrevious();
// 	}
	return FALSE;
}

void CPanelDABWrapper::RefreshLibrary(DAB_LIBRARY_TYPE nLibType)
{
	if(m_ppLibrary == NULL || GetTickCount() - m_dwDABLibUpdateTick < 250)
		return;

	switch(nLibType)
	{
		case DAB_LIBRARY_SERVICE:
			return m_ppLibrary->UpdateAllServiceList();
			break;
	}

	m_dwDABLibUpdateTick = GetTickCount();
}

BOOL CPanelDABWrapper::ScanServicePlay(BOOL bScan)
{
	if(m_ppMain == NULL)
		return FALSE;

	return m_ppMain->ScanServicePlay(bScan);
}

BOOL CPanelDABWrapper::AutoScanStore()
{
	FUNC_CHECK_BOOL(DABApi_AutoStoreProc());
	return TRUE;
}
BOOL CPanelDABWrapper::ScanService()
{
	FUNC_CHECK_BOOL(DABApi_ServiceScanProc());
	return TRUE;
}
void CPanelDABWrapper::SetTASetting(BOOL bOn)
{
	BYTE bCurrent;
	DABApi_GetTASetting(&bCurrent);

	if(bCurrent != bOn)
	{
		DABApi_SetTASetting();
	}

	DABApi_GetTASetting(&bCurrent);
	RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: %d.\r\n"), TEXT(__FUNCTION__),bCurrent));

}
void CPanelDABWrapper::SetLBandSetting(BOOL bOn)
{
	BYTE bCurrent;
	DABApi_GetLBandSetting(&bCurrent);

	if(bCurrent != bOn)
	{
		DABApi_SetLBandSetting();
	}

	DABApi_GetLBandSetting(&bCurrent);
	RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: %d.\r\n"), TEXT(__FUNCTION__),bCurrent));

}
BOOL CPanelDABWrapper::UpdateScanStatus(BOOL bScanning)
{
	RETAILMSG(DEBUG_DAB, (TEXT("MSG:DAB:[%s]: %d.\r\n"), TEXT(__FUNCTION__),bScanning));
	if(m_ppMain)
	{
		m_ppMain->UpdateScanStatus(bScanning);
	}
	return TRUE;
}

BOOL CPanelDABWrapper::ChangeBand()
{
	if(m_ppMain)
	{
		m_nBandMode = m_ppMain->ChangeBandMode();
		//RefreshLibrary(DAB_LIBRARY_SERVICE);
	}

	return TRUE;
}

BOOL CPanelDABWrapper::ChangeSearchType()
{
	if(m_ppMain)
	{
		m_nSearchType = m_ppMain->ChangeSearchType();
		//RefreshLibrary(DAB_LIBRARY_SERVICE);
	}
	return TRUE;
}

void CPanelDABWrapper::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(DEBUG_DAB,(_T("............CPanelDABWrapper:: OnSystemPowerStatePrepare:%d......................\r\n"),idState));

	switch (idState)
	{
	case SYS_POWER_OFF:
		{
			//save dab data:
			CDABAdapter::GetInstance()->SaveData();
		}
		break;
	case SYS_POWER_IDLE:
		{
			m_bIniStatus = FALSE;

			//save dab data:
			CDABAdapter::GetInstance()->SaveData();

			EnableDAB(FALSE);
			if(CDABAdapter::GetInstance()->GetInitialize())
			{
				RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: Deinitialize DABAdapter \r\n")));
				CDABAdapter::GetInstance()->Deinitialize();
			}
		}
		break;
	case SYS_POWER_ON:
		{
			if(!CDABAdapter::GetInstance()->GetInitialize())
			{
				RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: Initialize DABAdapter \r\n")));
				CDABAdapter::GetInstance()->Initialize(5,/*230400*/38400);
			}
			EnableDAB(TRUE);
		}
		break;
	}

}


#endif

