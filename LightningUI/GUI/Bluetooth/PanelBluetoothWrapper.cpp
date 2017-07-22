#include "PanelBluetoothWrapper.h"
#include "../Media/MediaCommon.h"

#define HFP_MAX_COMMAND_LENGTH						64

// BOOL	g_bBTPowerON = TRUE;

CPanelBluetoothWrapper::CPanelBluetoothWrapper(void)
{
	m_hfpStatus=SNM_CONNECTION_STATE_IDLE;
	m_bTalking=FALSE;
	m_dwCurDeviceID=1;

	m_dwCallStatus=HF_STATE_IDLE;
	m_dwRingTick=0;
	m_bAutoAnswerFlag=FALSE;
	m_dwConnectedTick=0;
	m_bAutoDownloadPhonebookFlag=FALSE;


	m_hBTWndHidden = NULL;
	m_pbtdPairRequest = NULL;
	m_pbtdAutoConnect = NULL;

	m_PlaySate = apStopped;

	m_pcr1 = NULL;
	m_pcr2 = NULL;

	m_dwEnterTick =0;

	m_bSpeechOn=FALSE;
	m_bPlayingBeforeSpeech=FALSE;

	m_pSmartBarPhone = NULL;

	m_bCheckA2dpFlag = FALSE;

	m_ppMain = NULL;
	m_ppTalking = NULL;
	m_ppA2dp = NULL;

	m_bBTPowerOn = TRUE;
	m_bBTPowerOnBeforeCarplay = TRUE;

	m_bOperatingPower = FALSE;
	m_bChangingAudioDir = FALSE;
	m_idAudioDir = HFM_AD_IN_DEVICE;
	m_idAudioDirManualSet = HFM_AD_IN_DEVICE;
	m_bPhoneCallActive = FALSE;

	m_bManualPause = FALSE;
	m_bNeedPause = FALSE;
	m_bNeedUnMute = TRUE;
	m_dwLastUnMuteTick = 0;

	//
	m_iVolumeCur = 0;
	m_iVolumeSet = -1;
	m_bVolumeFadeReady=TRUE;
	m_dwTalkingTick = 0;
	m_bA2dpStreamingOn = FALSE;
	m_idSRCBeforeA2dpStreamingOn = 0;
	m_bMonitorA2dpStatus = FALSE;
	m_bCarPlayConnected = FALSE;
}

CPanelBluetoothWrapper::~CPanelBluetoothWrapper(void)
{

}
void CPanelBluetoothWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_BT_BTN_HOME:
		break;
	}
}
void CALLBACK CPanelBluetoothWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{

	CPanelBluetoothWrapper *panel=(CPanelBluetoothWrapper*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);

}
void CPanelBluetoothWrapper::OnVolumeFadeReday(BOOL bMute)
{
	//auto pause:
	if(m_bNeedPause)
	{
		if(m_bManualPause && bMute && (m_PlaySate == apPlaying))
		{
			m_bNeedPause= FALSE;
			RETAILMSG(DEBUG_BT,(_T("#######CPanelBluetoothWrapper OnVolumeFadeReday Ready to bManualPause!!!!!\r\n")));
			Pause();
		}
	}
}
void CPanelBluetoothWrapper::OnDelayOperation()
{
	//auto pause:
	if(m_bNeedPause)
	{
		m_bNeedPause= FALSE;

		if(m_bManualPause && (m_PlaySate == apPlaying))
		{
			RETAILMSG(DEBUG_BT,(_T("#######CPanelBluetoothWrapper OnDelayOperation Ready to bManualPause!!!!!\r\n")));
			Pause();
		}
	}
}
void CPanelBluetoothWrapper::OnBluetoothCommand(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_BT,(_T("@@@@@@ OnBluetoothCommand :%d,%d\r\n"),wParam,lParam));

	if(!g_BTModule.IsInitialized())
		return;

	switch (wParam)
	{
	case BT_CMD_POWER:
		{
			SetBTPower(lParam);
			m_bBTPowerOnBeforeCarplay = lParam;
		}
		break;
	case BT_CMD_A2DP_VOLUME:
		{
			SetA2dpVolume(LOWORD(lParam),HIWORD(lParam));
		}
		break;
	case BT_CMD_GET_A2DP_STREAM:
		{
			*((BOOL*)lParam) = m_bA2dpStreamingOn;
		}
		break;
	case BT_CMD_MUTE:
		{
			if(lParam)
				MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000);
			else
				MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,1000);
		}
		break;
	case BT_CMD_SET_DEVICE_NAME:
		{
			SetDeviceName(LPTSTR(lParam));
		}
		break;
	case BT_CMD_SET_PIN:
		{
			SetPIN(LPTSTR(lParam));
		}
		break;
	case BT_CMD_HFP_CONTROL:
		{
			switch (lParam)
			{
			case HFP_CONTROL_ANSWER1:
				PhoneAnswer1();
				break;
			case HFP_CONTROL_ANSWER2:
				PhoneAnswer2();
				break;
			case HFP_CONTROL_HANGUP1:
				PhoneHangup1();
				break;
			case HFP_CONTROL_HANGUP2:
				PhoneHangup2();
				break;
			case HFP_CONTROL_ACH:
				PhoneTransferAudio();
				break;
			}
		}
		break;
	case BT_CMD_A2DP_CONTROL:
		{
			switch (lParam)
			{
			case A2DP_CONTROL_PLAY:
				Play();
				break;
			case A2DP_CONTROL_PAUSE:
				//Pause();
				PreparePause();
				break;
			case A2DP_CONTROL_NEXT:
				PlayNext();
				break;
			case A2DP_CONTROL_PREVIOUS:
				PlayPrevious();
				break;
			case A2DP_CONTROL_PLAYMODE_REPEATALL:
				ShuffleOff();
				RepeatAll();
				break;
			case A2DP_CONTROL_PLAYMODE_REPEATONE:
				ShuffleOff();
				RepeatOne();
				break;
			case A2DP_CONTROL_PLAYMODE_SHUFFLE:
				RepeatAll();
				ShuffleOn();
				break;
			}
		}
		break;
	}
}
void CPanelBluetoothWrapper::LinkSmartBarMedia(CSmartBarMedia *pSmartBar)
{
	if(m_ppA2dp)
	{
		m_ppA2dp->LinkSmartBarMedia(pSmartBar);
	}
}
void CPanelBluetoothWrapper::OnSpeechStart()
{
	RETAILMSG(DEBUG_BT,(_T("............CPanelBT OnSpeechStart......................\r\n")));

	m_bSpeechOn = TRUE;

#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
	{
		DWORD dwGPS=m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS;
		DWORD dwMedia=m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia;

		if(dwMedia)
		{
			DWORD dwVol=0;

			if(dwGPS>dwMedia)
				dwVol = (dwGPS>0?A2DP_LOW_VOL*dwMedia/dwGPS:A2DP_LOW_VOL);
			else
				dwVol = (dwMedia>0?A2DP_LOW_VOL*dwGPS/dwMedia:A2DP_LOW_VOL);

			RETAILMSG(DEBUG_BT,(_T("............CPanelBT apiAvrcp.put_Volume :%d ......................\r\n"),dwVol));

			//pull down volume
			SetA2dpVolume(dwVol);
		}
		else
		{
			SetA2dpVolume(0,FALSE);
		}
	}
#endif
}
void CPanelBluetoothWrapper::OnSpeechTerminate()
{
	RETAILMSG(DEBUG_BT,(_T("............CPanelBT OnSpeechTerminate......................\r\n")));

	m_bSpeechOn = FALSE;
#if !USE_WAV_ONCHIP

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
	{
		//pull up volume
		SetA2dpVolume(A2DP_MAX_VOL);
	}
#endif
}
void CPanelBluetoothWrapper::OnPlaySoundEffect(BOOL bPlay)
{
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
	{
		SetA2dpVolume(bPlay?0:A2DP_MAX_VOL,FALSE);
	}
}
void CPanelBluetoothWrapper::Render()
{
	if(!BeginRender())return;

	EndRender();
}
BOOL CPanelBluetoothWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);


	IniBTModule();

	//bt power:
	if(!m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
	{
		SetBTPower(FALSE);

		m_bBTPowerOn = FALSE;
		m_bBTPowerOnBeforeCarplay = FALSE;
	}

	return TRUE;
}
void CPanelBluetoothWrapper::LinkPages(CPanelBluetoothMain *pMain,CPanelBluetoothTalking *pTalking,CPanelBluetoothA2dp *pA2dp)
{
	m_ppMain = pMain;
	m_ppTalking = pTalking;
	m_ppA2dp = pA2dp;

	//
	wchar_t name[BTENUM_MAX_NAME];
	GetDeviceName(name);

	if(m_ppMain)
	{
		m_ppMain->SetDeviceName(name);
	}
}
BOOL CPanelBluetoothWrapper::IniBTModule()
{
	DWORD timeInMilliSec = GetTickCount();

	//initialize BT module:
	WNDCLASS    wc;
	TCHAR szWndClassName[] = HIDDEN_WND_CLASSNAME;
	memset(&wc, 0, sizeof(wc));

	wc.lpfnWndProc      = CPanelBluetoothWrapper::BTWndProc;
	wc.hInstance        =  m_moduleEntry.hInstance;
	wc.lpszClassName    = szWndClassName;
	wc.hIcon			= NULL;//LoadIcon(IDR_MAINFRAME);
	//wc.hIconSm			= ::LoadIcon(wc.hInstance , MAKEINTRESOURCE(IDR_MAINFRAME));

	//UnregisterClass(wc.lpszClassName, wc.hInstance);
	if (! RegisterClass(&wc))
	{
		BTDBG(ZONE_ERROR, (TEXT("!!!!!CPanelBT::Initialize  RegisterClass ERROR:%d\r\n"), GetLastError()));
		return FALSE;
	}

	m_hBTWndHidden = CreateWindow(szWndClassName, szWndClassName, WS_DISABLED | WS_POPUP,
		//CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, 1, 1,
		NULL/*g_hMainWnd*/, NULL,  wc.hInstance, this);

	ShowWindow(m_hBTWndHidden,SW_HIDE);

	//
	BTDBG(ZONE_ERROR, (TEXT("!!!!!g_BTModule.Initialize begin!!!!!\r\n")));

	if(!g_BTModule.Initialize(m_hBTWndHidden))
	{
		return FALSE;
	}
	BTDBG(ZONE_ERROR, (TEXT("!!!!!g_BTModule.Initialize end!!!!!\r\n")));

	//get and initialize data
	g_BTModule.pDataPool->GetData(CDataPool::BTState, (const void **)&m_pdwBTState);
	g_BTModule.pDataPool->GetData(CDataPool::HFState, (const void **)&m_szHFState);
	g_BTModule.pDataPool->GetData(CDataPool::CallState, (const void **)&m_szCallState);
	g_BTModule.pDataPool->GetData(CDataPool::DUNState, (const void **)&m_szDUNState);
	g_BTModule.pDataPool->GetData(CDataPool::CallRecordsList, (const void **)&m_pcrlist);
	g_BTModule.pDataPool->GetData(CDataPool::DUNNetworkList, (const void **)&m_pdnlist);
	g_BTModule.pDataPool->GetData(CDataPool::CallNumber, (const void **)&m_szCallNumber);
	g_BTModule.pDataPool->GetData(CDataPool::CallInfo1, (const void **)&m_ciCallInfo1);
	g_BTModule.pDataPool->GetData(CDataPool::CallInfo2, (const void **)&m_ciCallInfo2);
	g_BTModule.pDataPool->GetData(CDataPool::HeldState, (const void **)&m_szHeldState);
	g_BTModule.pDataPool->GetData(CDataPool::WaitingNumber, (const void **)&m_szWaitingNumber);
	g_BTModule.pDataPool->GetData(CDataPool::TotalNumberOfCalls, (const void **)&m_pdwTotalNumberOfCalls);
	/*
	g_BTModule.pDataPool->GetData(CDataPool::TireL1, (const void **)&m_tdTireL1);		//TPMS
	g_BTModule.pDataPool->GetData(CDataPool::TireL2, (const void **)&m_tdTireL2);		//TPMS
	g_BTModule.pDataPool->GetData(CDataPool::TireR1, (const void **)&m_tdTireR1);		//TPMS
	g_BTModule.pDataPool->GetData(CDataPool::TireR2, (const void **)&m_tdTireR2);		//TPMS
	*/
	g_BTModule.pDataPool->GetData(CDataPool::PBAPState, (const void **)&m_szPBAPState);	//PBAP
	g_BTModule.pDataPool->GetData(CDataPool::ContactsList, (const void **)&m_pclist); //PBAP
	g_BTModule.pDataPool->GetData(CDataPool::MAPState, (const void **)&m_szMAPState);	//MAP
	g_BTModule.pDataPool->GetData(CDataPool::TPMSState, (const void **)&m_szTPMSState);	//TPMS
	g_BTModule.pDataPool->GetData(CDataPool::MessagesList, (const void **)&m_pmlist); //PBAP
	g_BTModule.pDataPool->GetData(CDataPool::A2DPState, (const void **)&m_szA2DPState);	//A2DP
	g_BTModule.pDataPool->GetData(CDataPool::AVRCPState, (const void **)&m_szAVRCPState);	//AVRCP
	g_BTModule.pDataPool->GetData(CDataPool::AVRCPPlayState, (const void **)&m_tcAVRCPPlayState);	//AVRCP Play State
	g_BTModule.pDataPool->GetData(CDataPool::SPPState, (const void **)&m_szSPPState);	//MAP


	if (m_pdwBTState == NULL
		|| m_szCallState == NULL
		|| m_szHFState == NULL
		|| m_szPBAPState == NULL
		|| m_szDUNState == NULL
		|| m_szMAPState == NULL
		|| m_szTPMSState == NULL
		|| m_szSPPState == NULL
		|| m_pcrlist == NULL
		|| m_pclist == NULL
		|| m_pmlist == NULL
		|| m_pdnlist == NULL
		|| m_szCallNumber == NULL
		|| m_ciCallInfo1 == NULL
		|| m_szWaitingNumber == NULL
		|| m_pdwTotalNumberOfCalls == NULL
		/*
		|| m_tdTireL1 == NULL
		|| m_tdTireL2 == NULL
		|| m_tdTireR1 == NULL
		|| m_tdTireR2 == NULL
		*/
		|| m_szA2DPState == NULL
		|| m_szAVRCPState == NULL
		|| m_tcAVRCPPlayState == NULL)
	{
		BTDBG(ZONE_ERROR, (TEXT("###### Initialize GetData ERROR: \r\n")));
		return FALSE;
	}

	DWORD dwBtOff = FALSE;

	if (ERROR_SUCCESS != g_BTModule.pApi->apiSearchPair.IsBTPowerOff(&dwBtOff))
	{
		BTDBG(ZONE_LOGIC, (TEXT("CFSM_User::Initialize, Read BT Power Failed\r\n")));
		return FALSE;
	}

	DWORD dwBTState = (dwBtOff ? BT_STATE_OFF : BT_STATE_UNCONNECTED);	//need bt on/off function
	g_BTModule.pDataPool->SetData(CDataPool::BTState, (void *)&dwBTState, sizeof(DWORD));

	//set call state
	TCHAR szCallState[STATE_STRING_MAXLEN];
	_tcscpy(szCallState, BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::CallState, szCallState, sizeof(szCallState));

	//set hf state
	TCHAR szHFState[STATE_STRING_MAXLEN];
	_tcscpy(szHFState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::HFState, szHFState, sizeof(szHFState));

	//set dun state
	TCHAR szDUNState[STATE_STRING_MAXLEN];
	_tcscpy(szDUNState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::DUNState, szDUNState, sizeof(szDUNState));

	//set map state
	TCHAR szMAPState[STATE_STRING_MAXLEN];
	_tcscpy(szMAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::MAPState, szMAPState, sizeof(szMAPState));

	//set pbap state
	TCHAR szPBAPState[STATE_STRING_MAXLEN];
	_tcscpy(szPBAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::PBAPState, szPBAPState, sizeof(szPBAPState));

	//set a2dp state
	TCHAR szA2DPState[STATE_STRING_MAXLEN];
	_tcscpy(szA2DPState, A2dpStatus2String(AvsmNull));
	g_BTModule.pDataPool->SetData(CDataPool::A2DPState, szA2DPState, sizeof(szA2DPState));

	//set avrcp state
	TCHAR szAVRCPState[STATE_STRING_MAXLEN];
	_tcscpy(szAVRCPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::AVRCPState, szAVRCPState, sizeof(szAVRCPState));

	//set avrcp play state
	TCHAR tcAVRCPPlayState=0; //0 = apStopped, see bluetooth_avrcp.h
	g_BTModule.pDataPool->SetData(CDataPool::AVRCPPlayState, (void *)&tcAVRCPPlayState, sizeof(TCHAR));

	//set spp state
	TCHAR szSPPState[STATE_STRING_MAXLEN];
	_tcscpy(szSPPState, SPP_STATE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::SPPState, szSPPState, sizeof(szSPPState));

	//init HID state in DataPool
	TCHAR szHIDState[STATE_STRING_MAXLEN];
	_tcscpy(szHIDState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
	g_BTModule.pDataPool->SetData(CDataPool::HIDState, szHIDState, sizeof(szHIDState));

	//init HIDDiscoverable in DataPool
	BOOL bHidDiscoverable = TRUE;//init HIDDiscoverable to true,because, it's real true when bt app start
	g_BTModule.pDataPool->SetData(CDataPool::HIDDiscoverable,&bHidDiscoverable,sizeof(bHidDiscoverable));

	//set held state
	TCHAR szHeldState[STATE_STRING_MAXLEN];
	_tcscpy(szHeldState, BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS_VALUE_NOHELDCALL);
	g_BTModule.pDataPool->SetData(CDataPool::HeldState, szHeldState, sizeof(szHeldState));

	//set TotalNumbers
	DWORD dwNum = 0;
	g_BTModule.pDataPool->SetData(CDataPool::TotalNumberOfCalls, (void *)&dwNum, sizeof(DWORD));


	DWORD timeElapse = GetTickCount() - timeInMilliSec;
	RETAILMSG(DEBUG_BT, (TEXT("\r\n ####CBTModule::Initialize -  takes total time: %ums####\r\n \r\n"), timeElapse));

	return TRUE;
}
LRESULT CALLBACK CPanelBluetoothWrapper::BTWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CPanelBluetoothWrapper *panel=NULL;

	DWORD	result;
	TCHAR	*str;
	DWORD	dw;

	if(message == WM_CREATE)
	{
		RETAILMSG(DEBUG_BT, (TEXT("##@@CPanelBT::WndProc get panel point \r\n")));

		SetCursor(NULL);
		ShowCursor(FALSE);
		panel =(CPanelBluetoothWrapper *)(((CREATESTRUCT*)lParam)->lpCreateParams);
	}
	//hfp
	else if (message == g_uiSignalNotifyMessage && wParam & BLUETOOTH_MODULE_HANDSFREE)
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));

		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE:	//call state
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE - State: %s \r\n"), str));

						/*
						Possible value:
						1. INCOMING_CALL_STATE:
						incoming call or waiting call
						A call waiting in connected state is ONLY when a held call is present. Because there is no active call the call waiting is considered as a incoming call
						2. OUTGOING_CALL_STATE
						outgoing call
						3. TALKING_STATE
						having active call
						4. TERMINATING_STATE
						after calling HfmTerminateCall() at call waiting/calling/call pending state
						5. WAITING_CALL_STATE
						incoming call while there is an active call (+ccwa reponse)
						*/

					TCHAR szCallState[STATE_STRING_MAXLEN] = {0};
					StringCchCopy(STRING_AND_COUNTOF(szCallState),str);
					g_BTModule.pDataPool->SetData(CDataPool::CallState, szCallState, sizeof(TCHAR)*STATE_STRING_MAXLEN);

					//Set NumberOfCalls
					g_BTModule.pApi->apiHF.CallInfo();


					if (!_tcsicmp(str, BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_IDLE))
					{
						panel->OnCallStateChange(HF_STATE_IDLE);
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_TALKING))
					{
						panel->OnCallStateChange(HF_STATE_TALKING);
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_OUTGOINGCALL))
					{
						panel->OnCallStateChange(HF_STATE_OUTGOING_CALL);
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_INCOMINGCALL))
					{
						panel->OnCallStateChange(HF_STATE_INCOMING_CALL);
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_WAITING))
					{
						panel->OnCallStateChange(HF_STATE_WAITING_CALL);
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_TERMINATING))
					{
						panel->OnCallStateChange(HF_STATE_TERMINATING);
					}

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS:
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS - State: %s \r\n"), str));

					TCHAR szHeldState[STATE_STRING_MAXLEN] = {0};
					StringCchCopy(STRING_AND_COUNTOF(szHeldState),str);
					g_BTModule.pDataPool->SetData(CDataPool::HeldState, szHeldState, sizeof(TCHAR)*STATE_STRING_MAXLEN);

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_SPEAKER_VOLUME:
			{
				if ((result = g_BluetoothNotificationResolveValue(wParam, &dw)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_SPEAKER_VOLUME - State: %d \r\n"), dw));
					g_BTModule.pDataPool->SetData(CDataPool::HFSpeakerVolume, &dw, sizeof(dw));
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_SPEAKER_VOLUME - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_MIC_VOLUME:
			{
				if ((result = g_BluetoothNotificationResolveValue(wParam, &dw)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_MIC_VOLUME - State: %d \r\n"), dw));
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_MIC_VOLUME - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_BATTERY_LEVEL:
			{
				if ((result = g_BluetoothNotificationResolveValue(wParam, &dw)) == ERROR_SUCCESS)
				{
					BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_BATTERY_LEVEL - State: %d \r\n"), dw));
					panel->OnBatteryChange(dw);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_BATTERY_LEVEL - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_PHONE_SIGNAL_STRENGTH:
			{
				if ((result = g_BluetoothNotificationResolveValue(wParam, &dw)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_PHONE_SIGNAL_STRENGTH - State: %d \r\n"), dw));
					panel->OnSignalChange(dw);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_PHONE_SIGNAL_STRENGTH - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_SERVICE:
			{
				if ((result = g_BluetoothNotificationResolveValue(wParam, &dw)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_SERVICE - State: %d \r\n"), dw));
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_SERVICE - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_CALL_LINE_IDENTIFICATION:
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CALL_LINE_IDENTIFICATION - Phone number: %s \r\n"), str));

					TCHAR szCallNumber[STATE_STRING_MAXLEN] = {0};
					StringCchCopy(STRING_AND_COUNTOF(szCallNumber),str);
					g_BTModule.pDataPool->SetData(CDataPool::CallNumber, szCallNumber, sizeof(TCHAR)*STATE_STRING_MAXLEN);
					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CALL_LINE_IDENTIFICATION - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_CONNECTION_TYPE:
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CONNECTION_TYPE - State: %s \r\n"), str));
					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CONNECTION_TYPE - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID:
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID - : %s \r\n"), str));

					//save hf address id;
					if(!GetBA(str,&g_bHFConnecting))
					{
						g_bHFConnecting = 0;
					}

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_HANDSFREE_CALL_WAITING_NOTIFICATION:
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CALL_WAITING_NOTIFICATION - '%s' \r\n"), str));

					//init phone number of call waiting.

					TCHAR szWaitingNumber[MAX_PHONE_NUMBER] = {0};
					StringCchCopy(STRING_AND_COUNTOF(szWaitingNumber),str);
					g_BTModule.pDataPool->SetData(CDataPool::WaitingNumber, szWaitingNumber, sizeof(TCHAR) * MAX_PHONE_NUMBER);

					//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_SUCCESS_WAITINGCALL);

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_HANDSFREE_CALL_WAITING_NOTIFICATION - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_STATE_ID:	//HF state
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID - State: %s \r\n"), str));

					if (g_BTModule.pApi->apiHF.IsSameConnectionStatus((LPTSTR) str))
					{
						//not handle due to same HF state.
						BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID - Not handle due to same state \r\n")));
						free(str);
						break;
					}

					g_BTModule.pApi->apiHF.SetConnectionStatus((LPTSTR) str);

					if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
					{
						g_BTModule.pApi->apiHF.NotifyConnected();

#ifdef ENABLE_HFP_QUERY_OPERATOR
						//[TestOnly] Query operator selection.
						g_BTModule.pApi->apiHF.QueryOperator();
#endif
						panel->OnHFStateChange(SNM_CONNECTION_STATE_CONNECTED);

						//g_BTModule.pLogic->OfferTriggerToFSM(HF_DEVICE_EVT_STATE_CONNECTED, NULL);

						panel->OnActiveBTService(BTENUM_DEVICE_HANDSFREE_AG);

						//connect all service:

					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
					{
						g_BTModule.pApi->apiHF.NotifyConnected(FALSE);

						panel->OnHFStateChange(SNM_CONNECTION_STATE_IDLE);
						//g_BTModule.pLogic->OfferTriggerToFSM(HF_DEVICE_EVT_STATE_IDLE, NULL);

						panel->OnDeactiveBTService(BTENUM_DEVICE_HANDSFREE_AG);

					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTING))	//connecting, don't search now
					{
						//TODO
						panel->OnHFStateChange(SNM_CONNECTION_STATE_CONNECTING);

					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_DISCONNECTING))
					{
						//TODO
					}
					else
					{
						BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID - ERROR \r\n")));
						break;
					}

					//always set hf state after state is triggerred.
					TCHAR szHFState[STATE_STRING_MAXLEN] = {0};
					StringCchCopy(STRING_AND_COUNTOF(szHFState), str);
					g_BTModule.pDataPool->SetData(CDataPool::HFState, szHFState, sizeof(szHFState));

					//

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID - ERROR: %u \r\n"), result));
				}
			}
			break;


		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}
	//hfp api call back
	if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_HANDSFREE)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_OUTGOING:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_SUCCESS_OUTGOING);
			}
			break;

		case BLUETOOTH_NOTIFY_HF_API_CALLBACK_FAIL_OUTGOING:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_FAIL_OUTGOING);
			}
			break;

		case BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_INCOMINGCALL:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_SUCCESS_INCOMINGCALL, (void*)lParam);
			}
			break;

		case BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_ANSWERCALL:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_SUCCESS_ANSWERCALL);
			}
			break;

		case BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_REJECTCALL:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_SUCCESS_REJECTCALL);
			}
			break;

		case BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_TERMINATE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_SUCCESS_TERMINATE);
			}
			break;

		case BLUETOOTH_NOTIFY_HF_DEVICE_EVT_CALLNUM_ZERO:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_DEVICE_EVT_CALLNUM_ZERO);
				panel->OnCallInfoChange();
			}
			break;
		case BLUETOOTH_NOTIFY_HF_DEVICE_EVT_CALLNUM_VALID:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(BLUETOOTH_NOTIFY_HF_DEVICE_EVT_CALLNUM_VALID);
				panel->OnCallInfoChange();
			}
			break;

		case BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_WAITINGCALL:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HF_API_CALLBACK_SUCCESS_WAITINGCALL);
			}
			break;
		case BLUETOOTH_NOTIFY_HF_AUDIO_DIRECTION:
			{
				panel->OnAudioDirChange(lParam);
			}
			break;

		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HF SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}


	//dun
	else if (message == g_uiSignalNotifyMessage && wParam & BLUETOOTH_MODULE_DUN_CLIENT)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID:
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc DUN SN BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID - : %s \r\n"), str));
					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc DUN SN BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_STATE_ID:	//DUN state
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc DUN SN BLUETOOTH_NOTIFY_GENERAL_STATE_ID - State: %s \r\n"), str));

					if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
					{
						//don't use this trigger, use rasdial result instead
						////g_BTModule.pLogic->OfferTriggerToFSM(DUN_DEVICE_EVT_STATE_CONNECTED, NULL);
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
					{
						//g_BTModule.pLogic->OfferTriggerToFSM(DUN_DEVICE_EVT_STATE_IDLE, NULL);

						panel->OnDeactiveBTService(BTENUM_DEVICE_MODEM);

					}

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc DUN SN BLUETOOTH_NOTIFY_GENERAL_STATE_ID - ERROR: %u \r\n"), result));
				}
			}
			break;

		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc DUN SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}
	//phonebook
	else if (message == g_uiSignalNotifyMessage && wParam & BLUETOOTH_MODULE_PHONEBOOK_CLIENT)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_STATE_ID:	//PBAP state
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc PBAP SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID - State: %s \r\n"), str));


					g_BTModule.pApi->apiPB.SetConnectionStatus((LPTSTR) str);

					if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
					{
						//g_BTModule.pLogic->OfferTriggerToFSM(PBAP_DEVICE_EVT_STATE_CONNECTED, NULL);

						panel->OnActiveBTService(BTENUM_DEVICE_PHONEBOOK_PSE);
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
					{
						//g_BTModule.pLogic->OfferTriggerToFSM(PBAP_DEVICE_EVT_STATE_IDLE, NULL);

						panel->OnDeactiveBTService(BTENUM_DEVICE_PHONEBOOK_PSE);

					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTING))	//connecting, don't search now
					{
						LPTSTR szOldPBAPState = NULL;

						g_BTModule.pDataPool->GetData(CDataPool::PBAPState, (const void **)&szOldPBAPState);

						//[FIXME] Work-around for B-143028, because bt app may well not receive
						//PBAP "idle" status when bluetooth address to connect is changed. Then
						//it's necessary to update PBAP "idle" status firstly.
						if (_tcsicmp(szOldPBAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
						{
							BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc PBAP SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID  Update 'Idle' firstly, new state: 'Connecting', old state: '%s' \r\n"), szOldPBAPState));

							//g_BTModule.pLogic->OfferTriggerToFSM(PBAP_DEVICE_EVT_STATE_IDLE, NULL);
							panel->OnDeactiveBTService(BTENUM_DEVICE_PHONEBOOK_PSE);
						}
					}
					else if (!_tcsicmp(str, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_DISCONNECTING))
					{
					}
					else
					{
						BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc PBAP SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID - ERROR \r\n")));
						break;
					}

					//set pb state:
					TCHAR szPBState[STATE_STRING_MAXLEN] = {0};
					StringCchCopy(STRING_AND_COUNTOF(szPBState),str);
					g_BTModule.pDataPool->SetData(CDataPool::PBAPState, szPBState, sizeof(TCHAR)*STATE_STRING_MAXLEN);
					RETAILMSG(DEBUG_BT, (TEXT("##@@3PBAPState :%s \r\n"),szPBState));

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc PBAP SN :BLUETOOTH_NOTIFY_GENERAL_STATE_ID - ERROR: %u \r\n"), result));
				}
			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID:
			{
				if ((result = g_BluetoothNotificationResolveString(wParam, &str)) == ERROR_SUCCESS)
				{
					BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc PBAP SN :BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID - : %s \r\n"), str));

					//save hf address id;
					if(!GetBA(str,&g_bPBAPConnecting))
					{
						g_bPBAPConnecting = 0;
					}

					free(str);
				}
				else
				{
					BTDBG(ZONE_ERROR, (TEXT("CPanelBT::WndProc HF SN :BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID - ERROR: %u \r\n"), result));
				}
			}
			break;

		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc PBAP SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}

	//phonebook api call back
	else if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_PHONEBOOK_CLIENT)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_DOWNLOADED_COUNT:
			{
				if(panel->m_ppMain)
					panel->m_ppMain->SetContactDownloadedCount((DWORD)lParam);
			}
			break;
		case BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_DOWNLOAD_FINISH:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(PBAP_DEVICE_EVT_DOWNLOAD_FINISH, (void*)lParam );

				panel->OnPhoneBookDownloadFinish();
			}
			break;

		case BLUETOOTH_NOTIFY_PBAP_API_CALL_FAIL_DOWNLOAD:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(PBAP_API_CALL_FAIL_DOWNLOAD, (void*)lParam);
				panel->OnPhoneBookDownloadFailed();
			}
			break;

		case BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_SEARCH_FINISH:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(PBAP_DEVICE_EVT_SEARCH_FINISH, (void*)lParam);
			}
			break;

		case BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_GETENTRY_FINISH:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(PBAP_DEVICE_EVT_GETENTRY_FINISH);
			}
			break;

		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc PBAP SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}


	//a2dp
	else if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_A2DP_SINK)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTED:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(A2DP_DEVICE_EVT_STATE_CONNECTED);
				g_BTModule.pApi->apiA2dp.NotifyConnected();
				panel->OnNotifyA2dpConnected(TRUE);

				panel->OnActiveBTService(BTENUM_DEVICE_A2DP);
			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_STATE_IDLE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(A2DP_DEVICE_EVT_STATE_IDLE);
				g_BTModule.pApi->apiA2dp.NotifyConnected(FALSE);
				panel->OnNotifyA2dpConnected(FALSE);

				panel->OnDeactiveBTService(BTENUM_DEVICE_A2DP);

			}
			break;

		case BLUETOOTH_NOTIFY_A2DP_API_CALL_FAIL_CONNECT:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(A2DP_API_CALL_FAIL_CONNECT);
			}
			break;


		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc A2DP SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}

	//avrcp
	else if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_AVRCP_CT)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTED:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(AVRCP_DEVICE_EVT_STATE_CONNECTED,(void *)lParam);
				g_BTModule.pApi->apiAvrcp.NotifyConnected();
				panel->OnNotifyAvrcpConnected(TRUE);

				panel->OnActiveBTService(BTENUM_DEVICE_AVRCP);

			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_STATE_IDLE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(AVRCP_DEVICE_EVT_STATE_IDLE,(void *)lParam);
				g_BTModule.pApi->apiAvrcp.NotifyConnected(FALSE);
				panel->OnNotifyAvrcpConnected(FALSE);

				panel->OnDeactiveBTService(BTENUM_DEVICE_AVRCP);

			}
			break;

		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_PLAYER:
			{
				g_BTModule.pDataPool->AddData(CDataPool::AVRCPMediaPlayerList,(void*)lParam,sizeof(AvrcpMediaPlayerContentData));
			}
			break;

		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_FOLDER_ITEM:
			{
				g_BTModule.pDataPool->AddData(CDataPool::AVRCPFolderItemList,(void*)lParam,sizeof(AvrcpFolderItemContentData));
			}
			break;

		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_MEDIA_ITEM:
			{
				g_BTModule.pDataPool->AddData(CDataPool::AVRCPMediaItemList,(void*)lParam,sizeof(AvrcpMediaItemContentData));
			}
			break;

		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_MEDIA_ATTR:
			{
				//Update the media's attribute
				BOOL bUpdateSuc = FALSE;
				AvcmMetaDataInfoT * localInfo = (AvcmMetaDataInfoT *)lParam;
				if(!(g_AVRCPMusicListSelectedItemAttrQueue.empty()))
				{
					AvrcpMediaAttrQueueMemberT * p = (g_AVRCPMusicListSelectedItemAttrQueue.front());
					if(p != NULL && p->bMediaItem)
					{
						AvrcpMediaItemContentData *pMediaItemData = (AvrcpMediaItemContentData *)(p->p);
						if(pMediaItemData != NULL)
						{
							if(localInfo != NULL)
							{
								pMediaItemData->artistName = localInfo->artist;
								pMediaItemData->albumName = localInfo->album;
								g_BTModule.pDataPool->UpdateData(CDataPool::AVRCPMediaItemList,pMediaItemData,sizeof(AvrcpMediaItemContentData));
								bUpdateSuc = TRUE;
							}
							else
							{
								BTDBG(ZONE_ERROR,(TEXT("<%s>,ERROR! lParam is NULL!\r\n"),_T(__FUNCTION__)));
							}
						}

					}
				}
				//free lParam
				if(localInfo != NULL)
				{
					//if update media attribute failed, we need free localInfo's album & artist
					//if succeed, the free work will be done when ClearData(CDataPool::AVRCPMediaItemList)
					if(!bUpdateSuc)
					{
						if(localInfo->artist != NULL)
						{
							free(localInfo->artist);
							localInfo->artist = NULL;
						}
						if(localInfo->album != NULL)
						{
							free(localInfo->album);
							localInfo->album = NULL;
						}
					}
					//free the localInfo which is malloced in GetMediaItemAttribute callback function
					free(localInfo);
					localInfo = NULL;
				}
			}
			break;

		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_MEDIA_ATTR_WRONG:
			{
				//POP THE FIRST MEMBER IN THE QUEUE
				if(!(g_AVRCPMusicListSelectedItemAttrQueue.empty()))
				{
					delete(g_AVRCPMusicListSelectedItemAttrQueue.front());
					g_AVRCPMusicListSelectedItemAttrQueue.pop();
				}
			}
			break;

		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_NOW_PLAYING_CONTENT_CHANGED:
			{
				//Update now playing list
				//g_BTModule.pLogic->OfferTriggerToFSM(AVRCP_DEVICE_EVT_UPDATE_NOWPLAYING_CONTENT_AUTO);
			}
			break;

		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_UID_CHANGED:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(AVRCP_DEVICE_EVT_UPDATE_UID_AUTO);
			}
			break;
		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_PLAYSTATE_CHANGED:
			{
				panel->OnAvrcpPlayStateChange(AvcmPlayStateT(lParam));
			}
			break;
		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_METADATA:
			{
				panel->OnAvrcpMetaDataChange();
			}
			break;
		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_PLAYTIME:
			{
				panel->OnAvrcpPlayTimeChange();
			}
			break;
		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_REPEATMODE:
			{
				panel->OnAvrcpRepeatModeChange(lParam);
			}
			break;
		case BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_SHUFFLEMODE:
			{
				panel->OnAvrcpShuffleModeChange(lParam);
			}
			break;
		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc AVRCP SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}

	//tpms
	else if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_TPMS_CLIENT)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTED:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(TPMS_DEVICE_EVT_STATE_CONNECTED,(void *)lParam);

				panel->OnActiveBTService(BTENUM_DEVICE_TPMS);

			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_STATE_IDLE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(TPMS_DEVICE_EVT_STATE_IDLE,(void *)lParam);

				panel->OnDeactiveBTService(BTENUM_DEVICE_TPMS);

			}
			break;

		case BLUETOOTH_NOTIFY_TPMS_ALERT_TEMP:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(TPMS_DEVICE_EVT_ALERT_TEMP,(void *)lParam);
			}
			break;

		case BLUETOOTH_NOTIFY_TPMS_ALERT_PRESSUREHIGH:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(TPMS_DEVICE_EVT_ALERT_PRESSUREHIGH,(void *)lParam);
			}
			break;

		case BLUETOOTH_NOTIFY_TPMS_ALERT_PRESSURELOW:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(TPMS_DEVICE_EVT_ALERT_PRESSURELOW,(void *)lParam);
			}
			break;

		case BLUETOOTH_NOTIFY_TPMS_ALERT_RAPID_LEAK:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(TPMS_DEVICE_EVT_ALERT_RAPID_LEAK,(void *)lParam);
			}
			break;

		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc TPMS SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}
	// spp
	else if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_SPP)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTED:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(SPP_DEVICE_EVT_STATE_CONNECTED);
				g_BTModule.pApi->apiSPP.NotifyConnected();

				panel->OnActiveBTService(BTENUM_DEVICE_SPP);

			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_STATE_IDLE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(SPP_DEVICE_EVT_STATE_IDLE);
				g_BTModule.pApi->apiSPP.NotifyConnected(FALSE);

				panel->OnDeactiveBTService(BTENUM_DEVICE_SPP);

			}
			break;

		case BLUETOOTH_NOTIFY_SPP_API_CALL_FAIL_CONNECT:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(SPP_API_CALL_FAIL_CONNECT);
			}
			break;

		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc SPP SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}

	//hid
	else if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_HID_DEVICE)
	{
		switch(wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_STATE_IDLE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HID_DEVICE_EVT_STATE_IDLE);

				panel->OnDeactiveBTService(BTENUM_DEVICE_HID);

			}
			break;
		case BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTED:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(HID_DEVICE_EVT_STATE_CONNECTED,(void *)lParam);

				panel->OnActiveBTService(BTENUM_DEVICE_HID);

			}
			break;
		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc HID SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
		}
	}
	// map
	else if (message == UM_SINGAL_MSG && wParam & BLUETOOTH_MODULE_MAP_CLIENT)
	{
		switch (wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK)
		{
		case BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTED:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_DEVICE_EVT_STATE_CONNECTED,(void *)lParam);
				g_BTModule.pApi->apiMAP.NotifyConnected();

				panel->OnActiveBTService(BTENUM_DEVICE_MAP_MSE);

			}
			break;

		case BLUETOOTH_NOTIFY_GENERAL_STATE_IDLE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_DEVICE_EVT_STATE_IDLE,(void *)lParam);
				g_BTModule.pApi->apiMAP.NotifyConnected(FALSE);

				panel->OnDeactiveBTService(BTENUM_DEVICE_MAP_MSE);

			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_GETFOLDERLIST:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_FAIL_GETFOLDERLIST);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_GETMESSAGELIST:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_FAIL_GETMESSAGELIST);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_GETMESSAGEDETAIL:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_FAIL_GETMESSAGEDETAIL);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_CONNECT:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_FAIL_CONNECT);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_PUSHMESSAGE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_FAIL_PUSHMESSAGE);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_GETFOLDERLIST:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_SUCCESS_GETFOLDERLIST);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_GETMESSAGELIST:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_SUCCESS_GETMESSAGELIST);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_GETMESSAGE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_SUCCESS_GETMESSAGE);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_PUSHMESSAGE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_API_CALLBACK_SUCCESS_PUSHMESSAGE);
			}
			break;

		case BLUETOOTH_NOTIFY_MAP_DEVICE_EVT_NEW_MESSAGE:
			{
				//g_BTModule.pLogic->OfferTriggerToFSM(MAP_DEVICE_EVT_NEW_MESSAGE);
			}
			break;

		default:
			{
				BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc MAP SN :%d \r\n"), wParam & BLUETOOTH_NOTIFY_NAME_ID_MASK));
			}
			break;
		}
	}

	else if (message == UM_PIN_REQUEST)	//handle pin request
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_PIN_REQUEST - pbtd: 0x%x \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_DEVICE_EVT_PIN_REQUEST, (BTDEV *)wParam);
		if(panel)
		{
			panel->OnPinRequest((BTDEV *)wParam);
		}
	}
	else if (message == UM_SEARCH_SUCCESS)	//
	{
		BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc UM_SEARCH_SUCCESS - %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_API_CALLBACK_SUCCESS_SEARCH, (void *)wParam);
		panel->OnSearchFinished();
	}
	else if (message == UM_SEARCH_FAIL)	//
	{
		BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc UM_SEARCH_FAIL - : %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_API_CALLBACK_FAIL_SEARCH, (void *)wParam);
		panel->OnSearchFinished();
	}
	else if (message == UM_PAIR_SUCCESS)	//
	{
		BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc UM_PAIR_SUCCESS - %d,%x \r\n"), wParam,lParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_API_CALLBACK_SUCCESS_PAIR, (void *)wParam);

		//

		//connect:
		if(lParam)
		{
			g_BTModule.pApi->apiSearchPair.AddPairedDevice((BTDEV*)lParam);
// 			Sleep(5000);
// 			g_BTModule.pApi->apiSearchPair.ConnectDeviceAll((BTDEV*)lParam);
// 			g_BTModule.pApi->apiSearchPair.StartAutoConnect();

			panel->m_pbtdAutoConnect=(BTDEV*)lParam;

			SetTimer(hWnd,TIMER_BT_AUTO_CONNECT_AFTER_PAIRED,5000,NULL);
		}
	}
	else if (message == UM_PAIR_FAIL)	//
	{
		BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc UM_PAIR_FAIL - : %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_API_CALLBACK_FAIL_PAIR, (void *)wParam);
		if(panel)
		{
			panel->OnPairFailed();
		}
	}
	else if (message == UM_SYS_PN_SLEEP)	//
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_SYS_PN_SLEEP - : %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(SYS_DEVICE_EVT_PN_SLEEP, (void *)wParam);
	}
	else if (message == UM_SYS_PN_WAKEUP)	//
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_SYS_PN_WAKEUP - : %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(SYS_DEVICE_EVT_PN_WAKEUP, (void *)wParam);
	}
	else if (message == UM_SHOW_BTAPP)	//
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_SHOW_BTAPP - : %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(IPC_DEVICE_EVT_SHOW_BTAPP, (void *)wParam);
	}
	else if (message == WM_COPYDATA)	//
	{
		PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc WM_COPYDATA - : %x \r\n"), lParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(IPC_DEVICE_EVT_CALL_DIALER, (void *)lParam);
	}
	else if (message == UM_DUN_CONNECT_SUCCESS)	//
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_DUN_CONNECT_SUCCESS - %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(DUN_API_CALLBACK_SUCCESS_CONNECT, (void *)wParam);

		panel->OnActiveBTService(BTENUM_DEVICE_MODEM);
	}
	else if (message == UM_DUN_CONNECT_FAIL)	//
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_DUN_CONNECT_FAIL - %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(DUN_API_CALLBACK_FAIL_CONNECT, (void *)wParam);
	}
	else if (message == UM_ANSWERPAIR_SUCCESS)	//
	{
		BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc UM_ANSWERPAIR_SUCCESS - wParam %d ,lParam %x,panel->m_pbtdPairRequest:%x\r\n"), wParam,lParam,panel->m_pbtdPairRequest));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_API_CALLBACK_SUCCESS_ANSWERPAIR, (void *)wParam);

		if(panel->m_pbtdPairRequest)
		{
			g_BTModule.pApi->apiSearchPair.AddPairedDevice(panel->m_pbtdPairRequest);

// 			Sleep(5000);
// 			g_BTModule.pApi->apiSearchPair.ConnectDeviceAll(panel->m_pbtdPairRequest);

// 			g_BTModule.pApi->apiSearchPair.StartAutoConnect();

			panel->m_pbtdAutoConnect=panel->m_pbtdPairRequest;
			SetTimer(hWnd,TIMER_BT_AUTO_CONNECT_AFTER_PAIRED,5000,NULL);
		}
// 		if(lParam)
// 		{
// 			g_BTModule.pApi->apiSearchPair.AddPairedDevice((BTDEV*)lParam);
// 			g_BTModule.pApi->apiSearchPair.ConnectDeviceAll((BTDEV*)lParam);
// 		}
	}
	else if (message == UM_ANSWERPAIR_FAIL)	//
	{
		BTDBG(1/*ZONE_THREAD*/, (TEXT("CPanelBT::WndProc UM_ANSWERPAIR_FAIL - %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_API_CALLBACK_FAIL_ANSWERPAIR, (void *)wParam);
		if(panel)
		{
			panel->OnPairFailed();
		}
	}
	else if (message == UM_ANSWERPAIR_TIMEOUT)	//
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_ANSWERPAIR_TIMEOUT - %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_DEVICE_EVT_PIN_REQUEST_TIMEOUT, (void *)wParam);
		if(panel)
		{
			panel->OnPairFailed();
		}
	}
	else if (message == UM_CONNECT_ALL_FINISH)
	{
		BTDBG(1/*ZONE_THREAD*/, (TEXT("###CPanelBT::WndProc UM_CONNECT_ALL_FINISH - %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_API_CALLBACK_CONNECT_ALL_FINISH, (void *)wParam);
		panel->OnDeviceConnectFinish(wParam);
	}
	else if (message == UM_OPP_REQUEST)
	{
		BTDBG(ZONE_THREAD, (TEXT("CPanelBT::WndProc UM_OPP_REQUEST - %d \r\n"), wParam));
		//g_BTModule.pLogic->OfferTriggerToFSM(BT_DEVICE_EVT_OPP_REQUEST, (void *)wParam);
	}
	else if (message == UM_AUTO_CONNECT_STATUS)
	{
		RETAILMSG(1,(_T(">>>>BTWndProc:: UM_AUTO_CONNECT_STATUS: %d .........\r\n"),wParam));
		panel->m_ppMain->SetAutoConnectingFlag(wParam);
	}
	else if (message == UM_A2DP_STREAM_STATUS)
	{
		RETAILMSG(1,(_T(">>>>BTWndProc:: UM_A2DP_STREAM_STATUS: %d .........\r\n"),wParam));
		panel->OnA2dpStreamingStatus(wParam);
	}
	else if(message == WM_TIMER)
	{
		RETAILMSG(DEBUG_BT,(_T(">>>>BTWndProc:: WM_TIMER: %d .........\r\n"),wParam));

		if(panel)
		{
			if (wParam == TIMER_BT_PBDOWNLOAD_NOTIFY)
			{
				panel->AutoDownloadPhonebook();
			}
			else if (wParam == TIMER_BT_AUTO_ANSWER)
			{
				panel->AutoAnswer();
				KillTimer(panel->m_hBTWndHidden,TIMER_BT_AUTO_ANSWER);
			}
			else if(wParam == TIMER_BT_INCOMINGCALL_NOTIFY)
			{
				if(panel->m_pSkinManager->GetConfig()->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING) && panel->m_bPhoneCallActive)
				{
					RETAILMSG(DEBUG_BT,(_T("####TIMER_INCOMINGCALL_NOTIFY, switch to BT#####\r\n")));
					SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING,PAGE_SWITCH_ANI_NONE,0);
				}
				else
				{
					RETAILMSG(DEBUG_BT,(_T("####KillTimer TIMER_INCOMINGCALL_NOTIFY#####\r\n")));
					KillTimer(hWnd,TIMER_BT_INCOMINGCALL_NOTIFY);
				}
			}
	 		else if (wParam == TIMER_BT_UPDATE_PHONEBOOK)
			{
	 			panel->UpdatePhonebook();
			}
			else if (wParam == TIMER_BT_BEGIN_CONNECT)
			{
				if(panel->m_ppMain)
					panel->m_ppMain->OnBeginConnect();
				KillTimer(hWnd,TIMER_BT_BEGIN_CONNECT);
			}
			else if (wParam == TIMER_BT_BEGIN_PAIR)
			{
				if(panel->m_ppMain)
					panel->m_ppMain->OnBeginPair();
				KillTimer(hWnd,TIMER_BT_BEGIN_PAIR);
			}
			else if(wParam == TIMER_BT_DISCONNECT_ALL_FINISH)
			{
				if(panel->m_ppMain)
					panel->m_ppMain->OnDisconnectAllFinish();
				KillTimer(hWnd,TIMER_BT_DISCONNECT_ALL_FINISH);
			}
			else if (wParam == TIMER_BT_A2DP_NOTIFY)
			{
				KillTimer(hWnd,TIMER_BT_A2DP_NOTIFY);
				panel->AutoSwitchToA2DP();
			}
			else if (wParam == TIMER_BT_CHANGE_AUDIO_DIR)
			{
				KillTimer(hWnd,TIMER_BT_CHANGE_AUDIO_DIR);
				g_BTModule.pApi->apiHF.TransferHFPAudio();
				panel->m_bChangingAudioDir=FALSE;
			}
			else if (wParam == TIMER_BT_AUTO_CONNECT_AFTER_PAIRED)
			{
				KillTimer(hWnd,TIMER_BT_AUTO_CONNECT_AFTER_PAIRED);

				RETAILMSG(DEBUG_BT,(_T("####CPanelBT:: auto connect after paired dev:%x\r\n"),panel->m_pbtdAutoConnect));

				if(panel->m_pbtdAutoConnect)
				{
					g_BTModule.pApi->apiSearchPair.ConnectDeviceAll(panel->m_pbtdAutoConnect);
				}
			}
			else if (wParam == TIMER_BT_START_RINGTONE)
			{
				KillTimer(hWnd,TIMER_BT_START_RINGTONE);
				RETAILMSG(DEBUG_BT,(_T("####CPanelBT::TIMER_BT_START_RINGTONE\r\n")));

				if(panel->m_dwCallStatus == HF_STATE_INCOMING_CALL)
				{
					::PostMessage(panel->m_hMainWnd,WM_CMD_RINGTONE,1,NULL);
				}
			}
			else if (wParam == TIMER_BT_AUTO_CHANGE_AUDIO_DIR)
			{
				KillTimer(hWnd,TIMER_BT_AUTO_CHANGE_AUDIO_DIR);
				RETAILMSG(DEBUG_BT,(_T("####CPanelBT::TIMER_BT_AUTO_CHANGE_AUDIO_DIR\r\n")));

				if(panel->m_bPhoneCallActive)
				{
					g_BTModule.pApi->apiHF.TransferHFPAudio();
					if(panel->m_idAudioDirManualSet == HFM_AD_IN_DEVICE && panel->m_bTalking)
					{
						panel->MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,100);
					}
				}
			}
			else if(wParam == TIMER_BT_DELAY_OPERATION)
			{
				KillTimer(hWnd,TIMER_BT_DELAY_OPERATION);
				panel->OnDelayOperation();
			}
			else if(wParam == TIMER_BT_VOLUME_FADE)
			{
				panel->OnA2dpVolumeFade(0,0);
			}
			else if(wParam == TIMER_BT_AUTO_TERMINATE_SPEECH)
			{
				KillTimer(hWnd,TIMER_BT_AUTO_TERMINATE_SPEECH);
				if(panel->m_dwCallStatus!=HF_STATE_IDLE)
				{
					RETAILMSG(DEBUG_BT,(_T("####CPanelBT::TIMER_BT_AUTO_TERMINATE_SPEECH\r\n")));
					panel->OnCallStateChange(HF_STATE_IDLE);
				}
			}
			else if(wParam == TIMER_BT_MONITOR_PLAYINFO)
			{
				panel->OnMonitorPlayInfo();
			}
			else if(wParam == TIMER_BT_AUTO_POWER_ON)
			{
				if(panel->OnAutoPowerOn())
				{
					KillTimer(hWnd,TIMER_BT_AUTO_POWER_ON);
				}
			}
			else if(wParam == TIMER_BT_AUTO_POWER_OFF)
			{
				if(panel->OnAutoPowerOff())
				{
					KillTimer(hWnd,TIMER_BT_AUTO_POWER_OFF);
				}
			}
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}

void CPanelBluetoothWrapper::AutoSwitchToA2DP()
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper::AutoSwitchToA2DP  :%d\r\n"),m_PlaySate));

	if(m_PlaySate == apPlaying)
	{
		UINT idUI=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);
		UINT idClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);
		if(idUI != UI_CLASS_NAVI && idClass != UI_CLASS_BT && idClass != UI_CLASS_PHONELINK && m_idSystemPowerState == SYS_POWER_ON)
		{
			//SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,MEDIA_TYPE_A2DP);
		}
	}

	m_bCheckA2dpFlag = FALSE;
}


void CPanelBluetoothWrapper::OnSRCChanged(UINT idSRC)
{
	if(	GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_BT
		&& GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_PHONELINK
		&& GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_EASYCONNECTED)
	{
		Pause();
	}

}
BOOL CPanelBluetoothWrapper::OnKey(UINT idKey)
{

	switch (idKey)
	{
	case KEY_PHONE_ANSWER:
		{
			PhoneAnswer1();
		}
		break;
	case KEY_PHONE_HANG:
		{
			PhoneHangup1();
		}
		break;
	case KEY_PHONE_ACH:
		{
			PhoneTransferAudio();
		}
		break;
	case KEY_MEDIA_NEXT:
	case KEY_MCU_TRACK_NEXT:
		{
			PlayNext();
		}
		break;
	case KEY_MEDIA_PREVIOUS:
	case KEY_MCU_TRACK_PREV:
		{
			PlayPrevious();
		}
		break;
	case KEY_MEDIA_PLAY:
		{
			SwitchAudioSRC(SRC_AUDIO_ARM);
			Play();
		}
		break;
	case KEY_MEDIA_PAUSE:
		{
			PreparePause();
		}
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		{
			if(m_PlaySate == apPlaying)
			{
				PreparePause();
			}
			else
			{
				Play();
			}
		}
		break;
	}

	return TRUE;
}
void CPanelBluetoothWrapper::OnPinRequest(BTDEV * pbtd)
{
	if(pbtd)
	{
		RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnPinRequest  :%s,dev:0x%x\r\n"),pbtd->szDeviceName,pbtd));

		m_pbtdPairRequest = pbtd;

		//answer directly:
		Sleep(1000);
		g_BTModule.pApi->apiSearchPair.AnswerPair(&pbtd->b);

	}
}

BOOL CPanelBluetoothWrapper::OnActiveBTService(DWORD idServiceActive)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnActiveBTService  :%d ####\r\n"),idServiceActive));

	BT_ADDR b = 0;
	BTDEV* pbtd = NULL;

	CBTDevList *pbtdPairedDeviceList;
	g_BTModule.pDataPool->GetData(CDataPool::PairedDeviceList, (const void **)&pbtdPairedDeviceList);

	if(pbtdPairedDeviceList == NULL)
	{
		BTDBG(ZONE_ERROR, (TEXT("<%s> ERROR pbtdPairedDeviceList is NULL \r\n"), _T(__FUNCTION__)));
		//g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
		return FALSE;
	}

	//HF
    if (idServiceActive == BTENUM_DEVICE_HANDSFREE_AG)
    {
#ifdef ENABLE_HFP
		if(g_bHFConnecting != 0)
		{
			b = g_bHFConnecting;
			g_bHFConnecting = 0;
		}
		else if(g_pbtdCurrent != NULL)
		{
			 b = g_pbtdCurrent->b;
		}
		else
		{
			int iRet = g_BTModule.pApi->apiHF.GetSelectedDevice(&b);

			if(iRet != ERROR_SUCCESS)
			{
				BTDBG(ZONE_ERROR, (TEXT("<%s> apiHF->GetSelectedDevice FATAL ERROR"), _T(__FUNCTION__)));
				return FALSE;
			}
		}

        //not paired!!! fatal error!! disconnect it
        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            DWORD dwRet = g_BTModule.pApi->apiHF.Disconnect();
			//g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
			//[Coverity] Check the return val by apiHF.Disconnect()
			if (dwRet != ERROR_SUCCESS)
			{
				BTDBG(ZONE_ERROR,(TEXT("<%s> ERROR! Strange! Can't disconnect the phone!"),_T(__FUNCTION__)));
			}
            return FALSE;
        }

        //update fActive
        BTSERVICE *pbts;
        pbtd->Lock();
        if ((pbts = pbtd->FindService(BTENUM_DEVICE_HANDSFREE_AG)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();

            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_HANDSFREE_AG) ERROR \r\n"), _T(__FUNCTION__)));
			//g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
			return FALSE;
        }
        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));

        //update hf state into connected.
        if (_tcsicmp(m_szHFState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
        {
            TCHAR szHFState[STATE_STRING_MAXLEN];
            _tcscpy(szHFState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::HFState, szHFState, sizeof(szHFState));
        }

        //set bt state
//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }
#endif
    }
	//DUN
    else if (idServiceActive == BTENUM_DEVICE_MODEM)
    {
#ifdef ENABLE_DUN
        // read bt address in registry
        if (g_BTModule.pApi->apiDUN.GetConnectedDev(&b) != ERROR_SUCCESS)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> apiDUN.GetConnectedDev FATAL ERROR"), _T(__FUNCTION__)));
			return FALSE;
        }

        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

        //update fActive
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_MODEM)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_MODEM) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

        pbtd->Unlock();


        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));

        //set dun state, bt state
        if (_tcsicmp(m_szDUNState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
        {
            TCHAR szDUNState[STATE_STRING_MAXLEN];
            _tcscpy(szDUNState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::DUNState, szDUNState, sizeof(szDUNState));
        }

//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }
#endif
    }

	//PBAP
    else if (idServiceActive == BTENUM_DEVICE_PHONEBOOK_PSE)
    {
#ifdef ENABLE_PBAP
		b = g_bPBAPConnecting;

        //not paired!!! fatal error!! disconnect it
        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            g_BTModule.pApi->apiPB.Disconnect();
           // g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
            return FALSE;
        }

        //update fActive
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_PHONEBOOK_PSE)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_HANDSFREE_AG) ERROR \r\n"), _T(__FUNCTION__)));
			//g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
            return FALSE;
        }

        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));

        //set pbap state, bt state
        if (_tcsicmp(m_szPBAPState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
        {
            TCHAR szPBAPState[STATE_STRING_MAXLEN];
            _tcscpy(szPBAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::PBAPState, szPBAPState, sizeof(szPBAPState));
			RETAILMSG(DEBUG_BT, (TEXT("##@@1 PBAPState :%s \r\n"),szPBAPState));
        }

//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }
#endif
    }

	//TPMS
    else if (idServiceActive == BTENUM_DEVICE_TPMS)	//TPMS
    {
#ifdef ENABLE_TPMS
		g_BTModule.pApi->apiTPMS.m_bAutoConnectTPMS = FALSE;

		if(g_bTPMSConnecting)
			b = *g_bTPMSConnecting;
		else
			b=0;

// 		if (triggerData)
// 		{
// 			b = *(BT_ADDR *)triggerData;
// 			free(triggerData);
// 		}
// 		else
// 		{
// 			BTDBG(ZONE_ERROR, (TEXT("<%s> triggerData is null\r\n"),_T(__FUNCTION__)));
// 		}

		BTDBG(ZONE_ERROR, (TEXT("<%s> tpms addr=%04x%08x\r\n"),_T(__FUNCTION__), GET_NAP(b), GET_SAP(b)));

       //not paired!!! fatal error!! disconnect it
        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR\r\n"), _T(__FUNCTION__)));
            //g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
            return FALSE;
        }

        //update fActive
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_TPMS)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_HANDSFREE_AG) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));

        //set tpms state, bt state
        if (_tcsicmp(m_szTPMSState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
        {
            TCHAR szTPMSState[STATE_STRING_MAXLEN];
            _tcscpy(szTPMSState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::TPMSState, szTPMSState, sizeof(szTPMSState));
        }


//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }
#endif
    }

	//MAP
    else if (idServiceActive == BTENUM_DEVICE_MAP_MSE)	//MAP
    {
#ifdef ENABLE_MAP
		b = g_bMAPConnecting;

        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            //g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
            return FALSE;
        }

        //update fActive
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_MAP_MSE)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_HANDSFREE_AG) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));


        //Set MAP state.
        if (_tcsicmp(m_szMAPState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
        {
            TCHAR szMAPState[STATE_STRING_MAXLEN];
            _tcscpy(szMAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::MAPState, szMAPState, sizeof(szMAPState));
        }

//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }
#endif
    }

	//A2DP
    else if (idServiceActive == BTENUM_DEVICE_A2DP)	//a2dp
    {
#ifdef ENABLE_A2DP

		b = g_bA2DPConnecting;

        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            //g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
            return FALSE;
        }

        //update fActive
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_A2DP)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_A2DP) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));

        //Set A2DP state.
       if (_tcsicmp(m_szA2DPState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
        {
            TCHAR szA2DPState[STATE_STRING_MAXLEN];
            _tcscpy(szA2DPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::A2DPState, szA2DPState, sizeof(szA2DPState));

//         if ((AvrcpIdle == g_BTModule.pApi->apiAvrcp.GetConnectionStatus())/* && g_BTModule.pApi->apiA2dp.InitAvrcpConnect*/)
            if ((AvrcpConnected != g_BTModule.pApi->apiAvrcp.GetConnectionStatus())/* && g_BTModule.pApi->apiA2dp.InitAvrcpConnect*/)
            {
                g_BTModule.pApi->apiAvrcp.Connect(pbtd->b);
            }
        }

//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }

#endif
    }

	//AVRCP
	else if (idServiceActive == BTENUM_DEVICE_AVRCP)
    {
#ifdef ENABLE_AVRCP
		b = g_bAVRCPConnecting;

        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            //g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
            return FALSE;
        }

        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_AVRCP)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_AVRCP) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));


        //Set AVRCP state.
		if (_tcsicmp(m_szAVRCPState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED))
        {
            TCHAR szAVRCPState[STATE_STRING_MAXLEN];
            _tcscpy(szAVRCPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::AVRCPState, szAVRCPState, sizeof(szAVRCPState));
        }

//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }
#endif
    }
	//SPP
	else if (idServiceActive == BTENUM_DEVICE_SPP)	//SPP
    {
#ifdef ENABLE_SPP
		b = g_bSPPConnecting;

        if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            //g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);
            return FALSE;
        }

        //update fActive
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_SPP)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
        else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_SPP) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));

        //Set SPP state.
        if (_tcsicmp(m_szSPPState , SPP_STATE_CONNECTED))
        {
            TCHAR szSPPState[STATE_STRING_MAXLEN];
            _tcscpy(szSPPState, SPP_STATE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::SPPState, szSPPState, sizeof(szSPPState));
        }

//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }
#endif
    }
	//HID
	else if (idServiceActive == BTENUM_DEVICE_HID)
	{
#ifdef ENABLE_HID

		b = g_bHIDDevice;

// 		if(triggerData != NULL)
// 		{
// 			BTDBG(ZONE_ERROR, (TEXT("<%s> triggerData = %#x\r\n"), _T(__FUNCTION__),triggerData));
// 			b = *((BT_ADDR*)triggerData);
// 		}
// 		else
// 		{
// 			BTDBG(ZONE_ERROR, (TEXT("<%s> I don't know why. TriggerData should not be NULL! Just return!\r\n"), _T(__FUNCTION__)));
// 			return FALSE;
// 		}

		if ((pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR \r\n"), _T(__FUNCTION__)));
            //g_pFSMUser->OfferTriggerToFSM(MB_BN_CLICKED_CANCEL);//[fixme_uu]In what situation,will this happen?
            return FALSE;
        }

		//update fActive
        BTSERVICE *pbts;
        pbtd->Lock();


        if ((pbts = pbtd->FindService(BTENUM_DEVICE_HID)) != NULL)
        {
            pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
        }
		else//Our CT's HID can only be found and connected by phone for the first time.
		{
            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_HID) ERROR. Will add manually!\r\n"), _T(__FUNCTION__)));

			pbts = new BTSERVICE(BTENUM_DEVICE_HID);
			pbtd->AddService(pbts,0);
			pbts->fActive = TRUE;
            pbtd->fActive = TRUE;
		}
        /*else
        {
			pbtd->Unlock();
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_HID) ERROR \r\n"), _T(__FUNCTION__)));
            return FALSE;
        }*/

        pbtd->Unlock();

        //update g_pbtdConnected[g_dwCntConnectedDev] and datapool.update  PairedDeviceList
        int i = 0;

        for (i = 0; i < (int)g_dwCntConnectedDev; i++)
        {
            if (g_pbtdConnected[i]->b == pbtd->b)
                break;
        }

        if (i == g_dwCntConnectedDev)	//new device connected
        {
            g_pbtdConnected[g_dwCntConnectedDev++] = pbtd;
        }

		//Set HID state. We must set HID state before updateData(CDataPool::PairedDeviceList...), because we use GetData(CDataPool::HIDState...)
		// to judge if HID is connected in void CDlgDevice::UpdateHidStatus()
		const TCHAR *szHIDState = NULL;
		g_BTModule.pDataPool->GetData(CDataPool::HIDState,(const void **)(&szHIDState));
        if (szHIDState != NULL && _tcsicmp(szHIDState , BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED) != 0)
        {
			TCHAR szHIDStateTemp[STATE_STRING_MAXLEN];
            _tcscpy(szHIDStateTemp, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED);
            g_BTModule.pDataPool->SetData(CDataPool::HIDState, szHIDStateTemp, sizeof(szHIDStateTemp));
        }

		//set HID UnDiscoverable; when HID is Connected, our CT's HID is unDiscoverable
		BOOL bHidDiscoverable = FALSE;
		g_BTModule.pDataPool->SetData(CDataPool::HIDDiscoverable,&bHidDiscoverable,sizeof(bHidDiscoverable));

        g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));


//         if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
        {
            DWORD dwBTState = BT_STATE_CONNECTED;//::Connected;
            g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
        }

#endif
	}

	OnPairedDeviceListChanged();

	RETAILMSG(DEBUG_BT, (TEXT("<%s> - UpdateBTServies success, bluetooth = %s(%012I64x), Svr ID:%d ,devnum:%d,%x\r\n"), _T(__FUNCTION__), pbtd->szDeviceName, b ,idServiceActive ,g_dwCntConnectedDev,g_pbtdConnected[0]));

	return TRUE;
}

BOOL CPanelBluetoothWrapper::OnDeactiveBTService(DWORD idServiceDeactive)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnDeactiveBTService  :%d ####\r\n"),idServiceDeactive));

    //maybe peer disconnect or not connected
//     if (*g_pFSMUser->m_pdwBTState != CFSM_BT::Connected)
//     {
// 		BTDBG(ZONE_LOGIC, (TEXT("<%s> - WARN: BT is disconnected\r\n"), _T(__FUNCTION__)));
//     }

    BTDEV *pbtd = NULL;

	//HF
    if (idServiceDeactive == BTENUM_DEVICE_HANDSFREE_AG)
    {
		if (!_tcsicmp(m_szHFState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_LOGIC, (TEXT("<%s> - HFState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		//always set HFState to idle first
        TCHAR szHFState[STATE_STRING_MAXLEN];
        _tcscpy(szHFState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::HFState, szHFState, sizeof(szHFState));

		//get pbtd
		int i = 0;

		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTSERVICE *pbts;
			pbtd = g_pbtdConnected[i];
			pbtd->Lock();

			if ((pbts = pbtd->FindService(BTENUM_DEVICE_HANDSFREE_AG)) != NULL && pbts->fActive)
			{
				pbtd->Unlock();
				break;
			}

			pbtd->Unlock();
		}

		if(i == g_dwCntConnectedDev)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected HF in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
			return FALSE;
		}

        //update pbtd and PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_HANDSFREE_AG)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> - FindService(BTENUM_DEVICE_HANDSFREE_AG) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);
        }

        pbtd->Unlock();


		//set other state about hf
		DWORD dwNumberofCalls = 0;
		g_BTModule.pDataPool->SetData(CDataPool::TotalNumberOfCalls, (void *)&dwNumberofCalls, sizeof(DWORD));

		g_dwTalkingTime1 = 0;
		g_dwTalkingTime2 = 0;

// 		if(g_pFSMUser->m_pcr)
// 		{
// 			delete g_pFSMUser->m_pcr;
// 			g_pFSMUser->m_pcr = NULL;
// 		}

		HfmAudioDirectionT direction = HFM_AD_IN_GATEWAY;
		g_BTModule.pDataPool->SetData(CDataPool::HFAudioDirection, &direction, sizeof(DWORD));
    }
	//DUN
    else if (idServiceDeactive == BTENUM_DEVICE_MODEM)
    {
        if (!_tcsicmp(m_szDUNState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - DUNState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		//always set DUNState to idle first
        TCHAR szDUNState[STATE_STRING_MAXLEN];
        _tcscpy(szDUNState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::DUNState, szDUNState, sizeof(szDUNState));

		if (g_BTModule.pApi->apiDUN.DUNDisconnect(NULL) != ERROR_SUCCESS)
		{
			BTDBG(ZONE_ERROR, (TEXT("CUserState_MB_ConnectOk::onEntry apiDUN.DUNDisconnect FATAL ERROR")));
		}

		//get ptd from saved status
		int i = 0;

		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTSERVICE *pbts;
			pbtd = g_pbtdConnected[i];
			pbtd->Lock();

			if ((pbts = pbtd->FindService(BTENUM_DEVICE_MODEM)) != NULL && pbts->fActive)
			{
				pbtd->Unlock();
				break;
			}

			pbtd->Unlock();
		}

		if(i == g_dwCntConnectedDev)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected DUN in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
			return FALSE;
		}

        //update fActive and datapool.update  PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_MODEM)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_MODEM) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);
        }

        pbtd->Unlock();
    }

	//PBAP
    else if (idServiceDeactive == BTENUM_DEVICE_PHONEBOOK_PSE)
    {
		if (!_tcsicmp(m_szPBAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - PBAPState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		//always set PBAPState to idle first
        TCHAR szPBAPState[STATE_STRING_MAXLEN];
        _tcscpy(szPBAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::PBAPState, szPBAPState, sizeof(szPBAPState));
		RETAILMSG(DEBUG_BT, (TEXT("##@@2PBAPState :%s \r\n"),szPBAPState));


		//get ptd from saved status
		int i = 0;

		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTSERVICE *pbts;
			pbtd = g_pbtdConnected[i];
			pbtd->Lock();

			if ((pbts = pbtd->FindService(BTENUM_DEVICE_PHONEBOOK_PSE)) != NULL && pbts->fActive)
			{
				pbtd->Unlock();
				break;
			}

			pbtd->Unlock();
		}

		if(i == g_dwCntConnectedDev)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected PBAP in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
			return FALSE;
		}

        //update fActive , datapool.update  PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_PHONEBOOK_PSE)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_PHONEBOOK_PSE) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);
        }

        pbtd->Unlock();

    }
	//MAP
    else if (idServiceDeactive == BTENUM_DEVICE_MAP_MSE)
    {
        if (!_tcsicmp(m_szMAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - MAPState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		//always set MAPState to idle first
        TCHAR szMAPState[STATE_STRING_MAXLEN];
        _tcscpy(szMAPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::MAPState, szMAPState, sizeof(szMAPState));

		//get ptd from saved status
		int i = 0;

		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTSERVICE *pbts;
			pbtd = g_pbtdConnected[i];
			pbtd->Lock();

			if ((pbts = pbtd->FindService(BTENUM_DEVICE_MAP_MSE)) != NULL && pbts->fActive)
			{
				pbtd->Unlock();
				break;
			}

			pbtd->Unlock();
		}

		if(i == g_dwCntConnectedDev)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected MAP in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
			return FALSE;
		}

        //update fActive , datapool.update  PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_MAP_MSE)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_MAP_MSE) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);
        }

        pbtd->Unlock();
    }

    //TPMS
	else if (idServiceDeactive == BTENUM_DEVICE_TPMS)
    {
		if (!_tcsicmp(m_szTPMSState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - TPMSState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		BT_ADDR b = 0;
		if(g_bTPMSConnecting)
			b = *g_bTPMSConnecting;
		else
			b=0;
// 		if (triggerData)
// 		{
// 			b = *(BT_ADDR *)triggerData;
// 			free(triggerData);
// 		}
// 		else
// 		{
// 			BTDBG(ZONE_ERROR, (TEXT("<%s> triggerData is null\r\n"),_T(__FUNCTION__)));
// 		}

		BTDBG(ZONE_ERROR, (TEXT("<%s> tpms addr=%04x%08x\r\n"),_T(__FUNCTION__), GET_NAP(b), GET_SAP(b)));

		CBTDevList *pbtdPairedDeviceList;
		g_BTModule.pDataPool->GetData(CDataPool::PairedDeviceList, (const void **)&pbtdPairedDeviceList);

		if (pbtdPairedDeviceList == NULL || (pbtd = pbtdPairedDeviceList->Find(b)) == NULL)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> GetData or pbtdPairedDeviceList->Find(b) ERROR\r\n"),_T(__FUNCTION__)));
			return FALSE;
		}


        BTSERVICE *pbts;

        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_TPMS)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s> FindService(BTENUM_DEVICE_TPMS) ERROR or not active\r\n"),_T(__FUNCTION__)));
			ASSERT(FALSE);
        }

        pbtd->Unlock();

		//Update TPMS state
		int i = 0;
		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTDEV*	pbtdTemp;
            BTSERVICE *pbtsTemp;
            pbtdTemp = g_pbtdConnected[i];

            if (pbtdTemp != pbtd && (pbtsTemp = pbtdTemp->FindService(BTENUM_DEVICE_TPMS)) != NULL && pbtsTemp->fActive)
            {
                break;
            }
        }

		if( i == (int)g_dwCntConnectedDev)
		{
			BTDBG(ZONE_LOGIC, (TEXT("<%s> All TPMS devices are disconnected, set tpms_state = idle\r\n"), _T(__FUNCTION__)));
			TCHAR szTPMSState[STATE_STRING_MAXLEN];
			_tcscpy(szTPMSState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
			g_BTModule.pDataPool->SetData(CDataPool::TPMSState, szTPMSState, sizeof(szTPMSState));
		}
	}

	//A2DP
	else if (idServiceDeactive == BTENUM_DEVICE_A2DP)
    {
        if (!_tcsicmp(m_szA2DPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - A2DPState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		//always set A2DPState to idle first
        TCHAR szA2DPState[STATE_STRING_MAXLEN];
        _tcscpy(szA2DPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::A2DPState, szA2DPState, sizeof(szA2DPState));

		//get ptd from saved status
		int i = 0;

		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTSERVICE *pbts;
			pbtd = g_pbtdConnected[i];
			pbtd->Lock();

			if ((pbts = pbtd->FindService(BTENUM_DEVICE_A2DP)) != NULL && pbts->fActive)
			{
				pbtd->Unlock();
				break;
			}

			pbtd->Unlock();
		}

		if(i == g_dwCntConnectedDev)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected A2DP in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
			return FALSE;
		}

        //update fActive , datapool.update  PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_A2DP)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_A2DP) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);
        }

        pbtd->Unlock();

    }

	//AVRCP
	else if (idServiceDeactive == BTENUM_DEVICE_AVRCP)
    {
        if (!_tcsicmp(m_szAVRCPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - AVRCPState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		//always set AVRCPState to idle first
        TCHAR szAVRCPState[STATE_STRING_MAXLEN];
        _tcscpy(szAVRCPState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::AVRCPState, szAVRCPState, sizeof(szAVRCPState));

		//get ptd from saved status
		int i = 0;

		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTSERVICE *pbts;
			pbtd = g_pbtdConnected[i];
			pbtd->Lock();

			if ((pbts = pbtd->FindService(BTENUM_DEVICE_AVRCP)) != NULL && pbts->fActive)
			{
				pbtd->Unlock();
				break;
			}

			pbtd->Unlock();
		}

		if(i == g_dwCntConnectedDev)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected AVRCP in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
			return FALSE;
		}

        //update fActive , datapool.update  PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_AVRCP)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_A2DP) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);
        }

        pbtd->Unlock();

    }

	//SPP
	else if (idServiceDeactive == BTENUM_DEVICE_SPP)
    {
        if (!_tcsicmp(m_szSPPState, SPP_STATE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - SPPState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }

		//set SPPState to Idle first
        TCHAR szSPPState[STATE_STRING_MAXLEN];
        _tcscpy(szSPPState, SPP_STATE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::SPPState, szSPPState, sizeof(szSPPState));

		//get ptd from saved status
		int i = 0;

		for (i = 0; i < (int)g_dwCntConnectedDev; i++)
		{
			BTSERVICE *pbts;
			pbtd = g_pbtdConnected[i];
			pbtd->Lock();

			if ((pbts = pbtd->FindService(BTENUM_DEVICE_SPP)) != NULL && pbts->fActive)
			{
				pbtd->Unlock();
				break;
			}

			pbtd->Unlock();
		}

		if(i == g_dwCntConnectedDev)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected SPP in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
			return FALSE;
		}

        //update fActive , datapool.update  PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_SPP)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_SPP) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);
        }

        pbtd->Unlock();
    }

	//HID
#ifdef ENABLE_HID
	else if (idServiceDeactive == BTENUM_DEVICE_HID)
	{
		TCHAR *szHIDState = NULL;
		g_BTModule.pDataPool->GetData(CDataPool::HIDState,(const void **)(&szHIDState));
		/*
		if ((szHIDState != NULL) && !_tcsicmp(szHIDState, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE))
        {
			BTDBG(ZONE_ERROR, (TEXT("<%s> - HIDState is idle already, will hot handle Idle trigger\r\n"), _T(__FUNCTION__)));
            return FALSE;
        }*/

		//set HIDState to Idle first
        TCHAR szHIDStateTmp[STATE_STRING_MAXLEN];
        _tcscpy(szHIDStateTmp, BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE);
        g_BTModule.pDataPool->SetData(CDataPool::HIDState, szHIDStateTmp, sizeof(szHIDStateTmp));

		//set HID UnDiscoverable; when HID is disconnected, our CT's HID is unDiscoverable
		BOOL bHidDiscoverable = FALSE;
		g_BTModule.pDataPool->SetData(CDataPool::HIDDiscoverable,&bHidDiscoverable,sizeof(bHidDiscoverable));

        //if (g_pFSMUser->m_pbtdDisconnecting == NULL)	//peer disconnect// we can't disconnect HID from our side.
        //{
            //get ptd from saved status
            int i = 0;

            for (i = 0; i < (int)g_dwCntConnectedDev; i++)
            {
                BTSERVICE *pbts;
                pbtd = g_pbtdConnected[i];

                pbtd->Lock();

                if ((pbts = pbtd->FindService(BTENUM_DEVICE_HID)) != NULL && pbts->fActive)
                {
                    pbtd->Unlock();
                    break;
                }

                pbtd->Unlock();
            }

            if(i == g_dwCntConnectedDev)
			{
				BTDBG(ZONE_ERROR, (TEXT("<%s> - Error: No device is connected HID in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
				return FALSE;
			}
        /*}
        else
        {
            pbtd = g_pFSMUser->m_pbtdDisconnecting;
        }*/

        //update fActive , datapool.update  PairedDeviceList
        BTSERVICE *pbts;
        pbtd->Lock();

        if ((pbts = pbtd->FindService(BTENUM_DEVICE_HID)) != NULL && pbts->fActive)
        {
            pbts->fActive = FALSE;
            g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));
        }
        else
        {
			if(pbts == NULL)
				BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_HID) ERROR \r\n"), _T(__FUNCTION__)));
			else
	            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_HID) not active\r\n"), _T(__FUNCTION__)));


            BTDBG(ZONE_ERROR, (TEXT("<%s>: FindService(BTENUM_DEVICE_HID) ERROR or not active\r\n"), _T(__FUNCTION__)));
            ASSERT(FALSE);//[fixme_uu]This code often cause problem.Need to fix!
        }

        pbtd->Unlock();
	}
#endif
	else
	{
		return FALSE;
	}

	//Check if pbtd has other services active

	if(!pbtd)
	{
		BTDBG(ZONE_ERROR, (TEXT("<%s> - Error, pbtd is NULL  \r\n"), _T(__FUNCTION__)));
		return FALSE;
	}

	BOOL bHasOtherSrv=FALSE;
    BTSERVICE *pbts;
    pbtd->Lock();

    for (pbts = pbtd->pServiceList; pbts != NULL; pbts = pbts->pNext)
    {
        if (pbts->fActive)
		{
// 			BTDBG(ZONE_ERROR, (TEXT("## <%s> - pbtd has other services active :%d\r\n"), _T(__FUNCTION__),pbts->iDeviceClass));
			bHasOtherSrv=TRUE;
		}
    }

//     if (pbts != NULL)
	if (bHasOtherSrv)
    {
        pbtd->Unlock();

//         if (g_pFSMUser->m_pbtdDisconnecting == NULL)	//peer disconnect
//         {
// 			BTDBG(ZONE_LOGIC, (TEXT("<%s> - pbtd has other services active\r\n"), _T(__FUNCTION__)));
//             return TRUE;
//         }
//         else if (g_pFSMUser->m_dwServiceDisconnecting == BTENUM_CLASSES)	//disconnect device
//         {
// 			BTDBG(ZONE_ERROR, (TEXT("<%s> - pbtd has other services active\r\n"), _T(__FUNCTION__)));
// 			g_pFSMUser->m_pbtdDisconnecting = NULL;
//             return FALSE;
//         }
//         else
//         {
//             g_pFSMUser->m_pbtdDisconnecting = NULL;
//             return TRUE;
//         }

		//BTDBG(ZONE_ERROR, (TEXT("<%s> - pbtd has other services active\r\n"), _T(__FUNCTION__)));
		return FALSE;
    }

    //no service connected.update fActive
//     g_pFSMUser->m_pbtdDisconnecting = NULL;
    pbtd->fActive = FALSE;
    pbtd->Unlock();

    //remove from g_pbtdConnected[g_dwCntConnectedDev] and datapool, update  PairedDeviceList
    int i = 0;

    for (i = 0; i < (int)g_dwCntConnectedDev; i++)
    {
        if (g_pbtdConnected[i]->b == pbtd->b)
            break;
    }

    if(i != g_dwCntConnectedDev)
	{
		g_pbtdConnected[i] = NULL;

		for (; i < (int)g_dwCntConnectedDev - 1 ; i++)
		{
			g_pbtdConnected[i] = g_pbtdConnected[i + 1];
		}

		g_pbtdConnected[i] = NULL;
		g_dwCntConnectedDev--;
	}
	else
	{
		BTDBG(ZONE_ERROR,(TEXT("<%s> Error, i == g_dwCntConnectedDev, pbtd is not in g_pbtdConnected[]\r\n"), _T(__FUNCTION__)));
	}

    g_BTModule.pDataPool->UpdateData(CDataPool::PairedDeviceList, pbtd, sizeof(BTDEV));

    //Set bt state if all devices are disconnected
    if (g_dwCntConnectedDev == 0)
    {
        DWORD dwBTState = BT_STATE_UNCONNECTED;//::Unconnected;
        g_BTModule.pDataPool->SetData(CDataPool::BTState, &dwBTState, sizeof(DWORD));
    }

	OnPairedDeviceListChanged();


	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnDeactiveBTService  : no service connected  :: device count%d\r\n"),g_dwCntConnectedDev));

    return TRUE;
}



void CPanelBluetoothWrapper::OnPairFailed()
{
	if(m_ppMain)
	{
		m_ppMain->OnPairFailed();
	}
}
void CPanelBluetoothWrapper::OnSearchFinished()
{
	if(m_ppMain)
	{
		m_ppMain->OnDeviceSearchFinished();
	}
}
void CPanelBluetoothWrapper::OnPairedDeviceListChanged()
{
	if(m_ppMain)
	{
		m_ppMain->OnPairedDeviceListChanged();
	}
}

void CPanelBluetoothWrapper::OnHFStateChange(SnmConnectionState state)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnHFStateChange  state:%d\r\n"),state));

	//update connection status:
	if(m_ppMain)
		m_ppMain->OnHFStateChange(state);
	if(m_ppTalking)
		m_ppTalking->OnHFStateChange(state);

	m_hfpStatus = state;

	//Get device ID:
	if(SNM_CONNECTION_STATE_CONNECTED == m_hfpStatus)
	{
		//auto download phonebook:
		m_bAutoDownloadPhonebookFlag=TRUE;
		SetTimer(m_hBTWndHidden,TIMER_BT_PBDOWNLOAD_NOTIFY,10000,NULL);

	}
	else
	{

	}

	::PostMessage(m_hMainWnd,WM_PHONE_HF_STATE,state,NULL);

	//update status bar:
	if(m_pStatusBar && m_bBTPowerOn)
	{
		m_pStatusBar->SetBTStatus(m_hfpStatus,-1,-1);
	}

}


void CPanelBluetoothWrapper::OnCallStateChange(DWORD state)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnCallStateChange  :%d\r\n"),state));

	BOOL bActive=FALSE;

	m_dwCallStatus=state;

	switch (m_dwCallStatus)
	{
	case HF_STATE_IDLE:
		bActive=FALSE;
		break;

	default:
		// 	case HF_STATE_TERMINATING:
		// 	case HF_STATE_TALKING:
		// 	case HF_STATE_INCOMING_CALL:
		// 	case HF_STATE_OUTGOING_CALL:
		// 	case HF_STATE_WAITING_CALL:
		// 	case HF_STATE_NO_HELD_CALL:
		// 	case HF_STATE_HELD_CALL_NO_ACTIVE_CALL:

		bActive=TRUE;

		//
		if(m_dwCallStatus == HF_STATE_INCOMING_CALL )
		{
			if(!m_bAutoAnswerFlag)
			{
				m_dwRingTick = GetTickCount();
				m_bAutoAnswerFlag=TRUE;

				SetTimer(m_hBTWndHidden,TIMER_BT_AUTO_ANSWER,10000,NULL);
			}
		}
		break;
	}

	m_bEnableQuit=(!bActive);

	m_bPhoneCallActive = bActive;

	//
	if(m_ppTalking)
	{
		m_ppTalking->SetCallStatus(m_dwCallStatus);
	}

	{
		::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,bActive,m_dwCallStatus);

		if(bActive)
		{
			if(m_pSmartBarPhone)
			{
				if(m_dwCallStatus == HF_STATE_INCOMING_CALL)
					m_pSmartBarPhone->SetType(SB_INCOMING);
				else if(m_dwCallStatus == HF_STATE_OUTGOING_CALL)
					m_pSmartBarPhone->SetType(SB_OUTGOING);
				else if(m_dwCallStatus == HF_STATE_TALKING)
					m_pSmartBarPhone->SetType(SB_TALKING);
			}

			//enter talking panel:
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_NAVI)
			{
				if(m_pSmartBarPhone && !m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->Show(TRUE,INFINITE);
				}
			}
			else
			{
				if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
				{
					SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING,PAGE_SWITCH_ANI_NONE,0);

					//auto switch to talking panel:
					if(m_dwCallStatus == HF_STATE_INCOMING_CALL || m_dwCallStatus == HF_STATE_OUTGOING_CALL || m_dwCallStatus == HF_STATE_TALKING)
					{
						SetTimer(m_hBTWndHidden,TIMER_BT_INCOMINGCALL_NOTIFY,1000,NULL);
					}
				}
			}

			//pull down a2dp volume:
			SetA2dpVolume(0);
		}
		else
		{
			/////////////////////////
			//add call record:
			if(m_dwCallStatus == HF_STATE_IDLE)
			{
				BOOL bAddNew=FALSE;

				if(m_pcr1)
				{
					RETAILMSG(DEBUG_BT,(_T("####CPanelBT:: Add call record 1,type:%d,name:%s,number:%s\r\n"),m_pcr1->GetType(),m_pcr1->GetName(),m_pcr1->GetPhoneNumber()));
					g_BTModule.pDataPool->AddData(CDataPool::CallRecordsList, m_pcr1, sizeof(CALLREC));
// 					g_BTModule.pApi->apiHF.EnumUpdate(m_pcr1);
					m_pcr1 = NULL;
					bAddNew = TRUE;
				}
				if(m_pcr2)
				{
					RETAILMSG(DEBUG_BT,(_T("####CPanelBT:: Add call record 2,type:%d,name:%s,number:%s\r\n"),m_pcr2->GetType(),m_pcr2->GetName(),m_pcr2->GetPhoneNumber()));
					g_BTModule.pDataPool->AddData(CDataPool::CallRecordsList, m_pcr2, sizeof(CALLREC));
// 					g_BTModule.pApi->apiHF.EnumUpdate(m_pcr2);
					m_pcr2 = NULL;
					bAddNew = TRUE;
				}

				if(bAddNew)
				{
					if(m_ppMain)
					{
						m_ppMain->UpdateCallHistory();
					}
				}
			}


			//hide smart bar:
			if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
			{
				m_pSmartBarPhone->Show(FALSE);
			}
		}
	}

	m_bTalking = (m_dwCallStatus == HF_STATE_TALKING);
	if(m_dwCallStatus == HF_STATE_TALKING)
	{
		m_dwTalkingTick = GetTickCount();
	}

	//ring tone:
	if(m_pSkinManager->GetConfig()->GetCurParam()->bIncomingRing)
	{
		if(m_dwCallStatus == HF_STATE_INCOMING_CALL)
		{
			SetTimer(m_hBTWndHidden,TIMER_BT_START_RINGTONE,1000,NULL);
			//::PostMessage(m_hMainWnd,WM_CMD_RINGTONE,1,NULL);
		}
		else
		{
	// 		KillTimer(m_hBTWndHidden,TIMER_BT_START_RINGTONE);
			::PostMessage(m_hMainWnd,WM_CMD_RINGTONE,0,NULL);
		}
	}
}
void CPanelBluetoothWrapper::OnAudioDirChange(DWORD idDir)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnAudioDirChange  idDir:%d,m_idAudioDirManualSet:%d\r\n"),idDir,m_idAudioDirManualSet));

// 	if(idDir == HFM_AD_IN_DEVICE)
// 	{
// 		MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,1000);
// 	}

	m_idAudioDir = idDir;

	if((!m_bTalking&&m_bPhoneCallActive) || (m_bTalking && GetTickCount()-m_dwTalkingTick<2000))
	{
		if(m_ppTalking)
		{
			m_ppTalking->SetAudioDir(m_idAudioDirManualSet/*idDir*/);
		}

		if(m_pSmartBarPhone)
		{
			m_pSmartBarPhone->SetAudioDir(m_idAudioDirManualSet/*idDir*/);
		}

		//auto recover audio dir:
		if(m_idAudioDirManualSet!=idDir && m_idAudioDirManualSet!=0)
		{
			if(m_bPhoneCallActive)
			{
				RETAILMSG(DEBUG_BT,(_T("####CPanelBT::recover audio dir:  idDir:%d\r\n"),m_idAudioDirManualSet));
				SetTimer(m_hBTWndHidden,TIMER_BT_AUTO_CHANGE_AUDIO_DIR,1000,NULL);
			}
			else
			{
				//m_idAudioDirManualSet = idDir;
			}
		}
	}
	else if(/*m_bPhoneCallActive*/m_bTalking)
	{
		m_idAudioDirManualSet = idDir;

		if(m_ppTalking)
		{
			m_ppTalking->SetAudioDir(idDir);
		}
		if(m_pSmartBarPhone)
		{
			m_pSmartBarPhone->SetAudioDir(idDir);
		}
	}
	else
	{
		if(m_ppTalking)
		{
			m_ppTalking->SetAudioDir(m_idAudioDirManualSet);
		}
		if(m_pSmartBarPhone)
		{
			m_pSmartBarPhone->SetAudioDir(m_idAudioDirManualSet);
		}
	}
}

BOOL CPanelBluetoothWrapper::IsValidCallInfo(const CallInfoT *piCallInfo)
{
	BOOL bRet = FALSE;

	if(piCallInfo)
	{
		if(piCallInfo->phoneNumbers && wcslen(piCallInfo->phoneNumbers)>0)
		{
			bRet = TRUE;
		}
	}

	return bRet;
}
void CPanelBluetoothWrapper::OnCallInfoChange()
{
	DWORD dwTotalNumberOfCalls = *m_pdwTotalNumberOfCalls;
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnCallInfoChange  totalNumberOfCalls:%d\r\n"),dwTotalNumberOfCalls));

	if(dwTotalNumberOfCalls == 1)
	{
		if(m_ppTalking)
		{
			m_ppTalking->UpdateCallInfo(m_ciCallInfo1,0,1);
		}

		//test
		if(m_pSmartBarPhone)
		{
			m_pSmartBarPhone->SetPhoneNumber(m_ciCallInfo1->phoneNumbers);
		}

	}
	else if(dwTotalNumberOfCalls == 2)
	{
		if(m_ppTalking)
		{
			m_ppTalking->UpdateCallInfo(m_ciCallInfo1,0,2);
			m_ppTalking->UpdateCallInfo(m_ciCallInfo2,1,2);
		}
	}
	else if(dwTotalNumberOfCalls == 0)
	{
		if(m_ppTalking)
		{
			m_ppTalking->ClearCallInfo();
		}
	}


	//save record
	if(dwTotalNumberOfCalls > 0)
	{
		if(/*m_ciCallInfo1->status != HFM_CALL_ACTIVE &&*/ m_pcr1 == NULL )
		{
			SYSTEMTIME sysTime;
			FILETIME ft;
			GetLocalTime(&sysTime);
			if (!SystemTimeToFileTime(&sysTime, &ft) || (m_pcr1 = new CALLREC(ft, CALLREC::CRT_INVALID)) == NULL)
			{
				BTDBG(ZONE_ERROR, (TEXT("-----CPanelBT::OnCallInfoChange ERROR:SystemTimeToFileTime or new CALLREC\r\n")));
				return;
			}
			m_pcr1->SetName(m_ciCallInfo1->name);
			m_pcr1->SetPhoneNumber(m_ciCallInfo1->phoneNumbers);
		}

		RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnCallInfoChange  call status1 :%d\r\n"),m_ciCallInfo1->status));

		//update status:
		if(m_pcr1 && !_tcsicmp(m_pcr1->GetPhoneNumber(),m_ciCallInfo1->phoneNumbers))
		{
			if(m_ciCallInfo1->status == HFM_OUTGOING_CALL_ALERT || m_ciCallInfo1->status == HFM_OUTGOING_CALL)
			{
				m_pcr1->SetType(CALLREC::CRT_OUTGOING);
			}
			else if(m_ciCallInfo1->status == HFM_INCOMING_CALL)
			{
				m_pcr1->SetType(CALLREC::CRT_MISSED);
			}
			else if(m_ciCallInfo1->status == HFM_CALL_ACTIVE)
			{
				if(m_pcr1->GetType() == CALLREC::CRT_MISSED)
				{
					m_pcr1->SetType(CALLREC::CRT_INCOMING);
				}
			}
		}
	}

	if(dwTotalNumberOfCalls > 1)
	{
		if(/*m_ciCallInfo1->status != HFM_CALL_ACTIVE &&*/ m_pcr2 == NULL )
		{
			SYSTEMTIME sysTime;
			FILETIME ft;
			GetLocalTime(&sysTime);
			if (!SystemTimeToFileTime(&sysTime, &ft) || (m_pcr2 = new CALLREC(ft, CALLREC::CRT_INVALID)) == NULL)
			{
				BTDBG(ZONE_ERROR, (TEXT("-----CPanelBT::OnCallInfoChange ERROR:SystemTimeToFileTime or new CALLREC\r\n")));
				return;
			}
			m_pcr2->SetName(m_ciCallInfo2->name);
			m_pcr2->SetPhoneNumber(m_ciCallInfo2->phoneNumbers);
		}

		RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnCallInfoChange  call status2 :%d\r\n"),m_ciCallInfo2->status));

		//update status:
		if(m_pcr2 && !_tcsicmp(m_pcr2->GetPhoneNumber(),m_ciCallInfo2->phoneNumbers))
		{
			if(m_ciCallInfo2->status == HFM_OUTGOING_CALL_ALERT || m_ciCallInfo2->status == HFM_OUTGOING_CALL)
			{
				m_pcr2->SetType(CALLREC::CRT_OUTGOING);
			}
			else if(m_ciCallInfo2->status == HFM_CALL_WAITING)
			{
				m_pcr2->SetType(CALLREC::CRT_MISSED);
			}
			else if(m_ciCallInfo2->status == HFM_CALL_ACTIVE)
			{
				if(m_pcr2->GetType() == CALLREC::CRT_MISSED)
				{
					m_pcr2->SetType(CALLREC::CRT_INCOMING);
				}
			}
		}
	}
}


void CPanelBluetoothWrapper::OnSignalChange(DWORD dwSignal)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnSignalChange  dwSignal:%d\r\n"),dwSignal));
	//update status bar:
	if(m_pStatusBar && m_bBTPowerOn)
	{
		m_pStatusBar->SetBTStatus(m_hfpStatus,dwSignal,-1);
	}
}
void CPanelBluetoothWrapper::OnBatteryChange(DWORD dwBattery)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBT::OnBatteryChange  dwBattery:%d\r\n"),dwBattery));
	//update status bar:
	if(m_pStatusBar && m_bBTPowerOn)
	{
		m_pStatusBar->SetBTStatus(m_hfpStatus,-1,dwBattery);
	}
}
void CPanelBluetoothWrapper::OnDeviceConnectFinish(WPARAM wParam)
{
	if(m_ppMain)
		m_ppMain->OnDeviceConnectFinish(wParam);
}
void CPanelBluetoothWrapper::OnPhoneBookDownloadFinish()
{
	SetTimer(m_hBTWndHidden,TIMER_BT_UPDATE_PHONEBOOK,2000,NULL);
}
void CPanelBluetoothWrapper::OnPhoneBookDownloadFailed()
{
	if(m_ppMain)
	{
		m_ppMain->OnPhoneBookDownloadFailed();
	}
}

void CPanelBluetoothWrapper::OnNotifyA2dpConnected(BOOL bConnected)
{
	RETAILMSG(1,(_T("####CPanelBT::OnNotifyA2dpConnected  :%d\r\n"),bConnected));

	::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_A2DP,bConnected);

	if(bConnected &&
		(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP)||
		m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_MAIN))
		)
	{
		SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);
	}
	else
	{
		SetA2dpVolume(0);
	}

	if(m_ppA2dp)
		m_ppA2dp->OnA2dpConnected(bConnected);

	if(m_ppMain)
		m_ppMain->OnA2dpConnected(bConnected);
}
void CPanelBluetoothWrapper::OnNotifyAvrcpConnected(BOOL bConnected)
{
	RETAILMSG(1,(_T("####CPanelBT::OnNotifyAvrcpConnected  :%d\r\n"),bConnected));

	if(m_ppA2dp)
		m_ppA2dp->OnAvrcpConnected(bConnected);

	if(bConnected && (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP))	)
	{
		//auto play:
		Play();
	}
}
void CPanelBluetoothWrapper::OnAvrcpPlayStateChange(AvcmPlayStateT state)
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper::OnAvrcpPlayStateChange  :%d\r\n"),state));

	if(m_ppA2dp)
		m_ppA2dp->OnAvrcpPlayStateChange(state);

	/*
	UINT idClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);

	//auto change to a2dp page:
	if(state == apPlaying && idClass!=UI_CLASS_BT)
	{
		if(m_bCheckA2dpFlag)
		{
			KillTimer(m_hBTWndHidden,TIMER_BT_A2DP_NOTIFY);
		}
		SetTimer(m_hBTWndHidden,TIMER_BT_A2DP_NOTIFY,3500,NULL);
		m_bCheckA2dpFlag = TRUE;
	}
	*/

	/*
	//volume:
	if(idClass == UI_CLASS_BT ||
		idClass == UI_CLASS_PHONELINK ||
		idClass == UI_CLASS_EASYCONNECTED)
	{
		if(state == apPlaying && ((m_PlaySate != state)||m_bNeedUnMute))
		{
			RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper:: unmute\r\n")));

			//recover volume:
			if(GetTickCount()-m_dwLastUnMuteTick>1500)
			{
				m_bNeedUnMute=FALSE;
				MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,100);
			}
			SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);
		}
	}
	*/

	//
	if(m_PlaySate != state)
	{
// 		if(state == apPlaying)
// 		{
// 			RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper:: monitor playinfo -------\r\n")));
// 			SetTimer(m_hBTWndHidden,TIMER_BT_MONITOR_PLAYINFO,1000,NULL);
// 		}
// 		else
// 		{
// 			RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper:: stop monitor playinfo -----\r\n")));
// 			KillTimer(m_hBTWndHidden,TIMER_BT_MONITOR_PLAYINFO);
// 		}
		m_PlaySate = state;
	}

}
void CPanelBluetoothWrapper::OnAvrcpMetaDataChange()
{
	RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper::OnAvrcpMetaDataChange  \r\n")));

	if(m_ppA2dp)
		m_ppA2dp->OnAvrcpMetaDataChange();

}
void CPanelBluetoothWrapper::OnAvrcpPlayTimeChange()
{
	//RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper::OnAvrcpPlayTimeChange  \r\n")));

	if(m_ppA2dp)
		m_ppA2dp->OnAvrcpPlayTimeChange();
}
void CPanelBluetoothWrapper::OnAvrcpRepeatModeChange(AvcmPlayAppSettingAttValIdT repeatMode)
{
	if(m_ppA2dp)
		m_ppA2dp->OnAvrcpRepeatModeChange(repeatMode);

}
void CPanelBluetoothWrapper::OnAvrcpShuffleModeChange(AvcmPlayAppSettingAttValIdT shuffleMode)
{
	if(m_ppA2dp)
		m_ppA2dp->OnAvrcpShuffleModeChange(shuffleMode);

}
void CPanelBluetoothWrapper::SetA2dpVolume(INT iVolume,BOOL bFade)
{
	if(g_BTModule.IsInitialized())
	{
		if(iVolume<0)
			iVolume=0;
		else if(iVolume>100)
			iVolume=100;

		RETAILMSG(1/*DEBUG_BT*/,(_T("+++SetA2dpVolume: %d , bFade:%d--\r\n"),iVolume,bFade));

		if(bFade)
		{
			m_iVolumeSet = iVolume;

			if(m_bVolumeFadeReady)
			{
				m_bVolumeFadeReady=FALSE;
				SetTimer(m_hBTWndHidden,TIMER_BT_VOLUME_FADE,VOLUME_FADE_STEP,NULL);
			}
		}
		else
		{
			m_iVolumeCur = iVolume;
			m_iVolumeSet = iVolume;
			g_BTModule.pApi->apiAvrcp.put_Volume(iVolume);
		}
	}
}
BOOL CPanelBluetoothWrapper::AutoVolumeStepProc()
{
	BOOL bRet=FALSE;

	//adjust volume:
	if(!m_bVolumeFadeReady)
	{
		if(abs(m_iVolumeCur-m_iVolumeSet)>1)
		{
			int step=(m_iVolumeSet-m_iVolumeCur)/10;

			if(step==0)
				step=(m_iVolumeSet-m_iVolumeCur)>0?1:-1;

			m_iVolumeCur += step;
		}
		else
		{
			m_iVolumeCur=m_iVolumeSet;
			m_bVolumeFadeReady = TRUE;
			bRet=TRUE;
		}

		g_BTModule.pApi->apiAvrcp.put_Volume(m_iVolumeCur);
	}
	else
	{
		m_bVolumeFadeReady=TRUE;
		bRet=TRUE;
	}

	return bRet;
}


void CPanelBluetoothWrapper::OnA2dpVolumeFade(WPARAM wParam, LPARAM lParam)
{
	if(AutoVolumeStepProc())
	{
		KillTimer(m_hBTWndHidden,TIMER_BT_VOLUME_FADE);

		RETAILMSG(DEBUG_BT,(_T("APP:MSG:GUI:BT: [CPanelBluetoothWrapper::OnA2dpVolumeFade end]: --\r\n")));
	}
}
void CPanelBluetoothWrapper::Play()
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("####CPanelBluetoothWrapper::Play  \r\n")));

	m_bManualPause = FALSE;

	MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,200);

	if(m_PlaySate != apPlaying)
	{
		DWORD dwRet = g_BTModule.pApi->apiAvrcp.Play();
		if (dwRet != ERROR_SUCCESS)
		{

		}
		else
		{
			SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);

			AvcmPlayAppSettingAttIdT repeatAttIdT = AVCM_PAS_REPEAT_ATT_ID;
			AvcmPlayAppSettingAttIdT shuffleAttIdT = AVCM_PAS_SHUFFLE_ATT_ID;
			g_BTModule.pApi->apiAvrcp.GetRepeat(repeatAttIdT);
			g_BTModule.pApi->apiAvrcp.GetShuffle(shuffleAttIdT);
		}
	}
	else
	{
		//recover volume:
		SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);
	}

}

void CPanelBluetoothWrapper::PreparePause()
{
	if(m_PlaySate == apPlaying)
	{
		RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper::PreparePause  \r\n")));

		m_bManualPause = TRUE;

		if(!m_bNeedPause)
		{
			m_bNeedUnMute = TRUE;
			m_dwLastUnMuteTick = GetTickCount();

			m_bNeedPause = TRUE;

			MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000);   //MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,FALSE,1000);
			SetTimer(m_hBTWndHidden,TIMER_BT_DELAY_OPERATION,1200,NULL);
		}
	}
}

void CPanelBluetoothWrapper::Pause()
{
	if(m_PlaySate == apPlaying)
	{
		RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper::Pause  \r\n")));
// 		MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,1000);

		SetA2dpVolume(0);
		g_BTModule.pApi->apiAvrcp.Pause();
	}

	m_bNeedUnMute = TRUE;
	m_dwLastUnMuteTick = GetTickCount();

}
void CPanelBluetoothWrapper::PlayNext()
{
	MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,600);

	DWORD dwRet = g_BTModule.pApi->apiAvrcp.Next();
	if (dwRet != ERROR_SUCCESS)
	{

	}
	else
	{
		//recover volume:
		SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);

		//
		if(m_ppA2dp)
		{
			m_ppA2dp->PlayInfoFlyOut(1);
		}
	}
}
void CPanelBluetoothWrapper::PlayPrevious()
{
	MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,600);

	DWORD dwRet = g_BTModule.pApi->apiAvrcp.Previous();
	if (dwRet != ERROR_SUCCESS)
	{

	}
	else
	{
		//recover volume:
		SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);

		//
		if(m_ppA2dp)
		{
			m_ppA2dp->PlayInfoFlyOut(0);
		}
	}
}


void CPanelBluetoothWrapper::RepeatOne()
{
	g_BTModule.pApi->apiAvrcp.Repeat(AVCM_PAS_REPEAT_VAL_SINGLE);
}
void CPanelBluetoothWrapper::RepeatAll()
{
	g_BTModule.pApi->apiAvrcp.Repeat(AVCM_PAS_REPEAT_VAL_ALL);
}
void CPanelBluetoothWrapper::RepeatOff()
{
	g_BTModule.pApi->apiAvrcp.Repeat(AVCM_PAS_REPEAT_VAL_OFF);
}
void CPanelBluetoothWrapper::ShuffleOn()
{
	AvcmPlayAppSettingAttValIdT shuffleAttrVal=AVCM_PAS_SHUFFLE_VAL_ALL;
	g_BTModule.pApi->apiAvrcp.Shuffle(shuffleAttrVal);
}
void CPanelBluetoothWrapper::ShuffleOff()
{
	AvcmPlayAppSettingAttValIdT shuffleAttrVal=AVCM_PAS_SHUFFLE_VAL_OFF;
	g_BTModule.pApi->apiAvrcp.Shuffle(shuffleAttrVal);
}
void CPanelBluetoothWrapper::AutoAnswer()
{
	m_bAutoAnswerFlag = FALSE;

	if(m_pSkinManager->GetConfig()->GetCurParam()->bCallAutoAnswer)
	{
		PhoneAnswer1();
	}
}
void CPanelBluetoothWrapper::UpdatePhonebook()
{
	if(!m_pSkinManager->GetGUI()->IsAnimating())
	{
		RETAILMSG(DEBUG_BT,(_T(">>>>CPanelBT:: UpdatePhonebook.........\r\n")));

		if(m_pclist && m_ppMain)
		{
			m_ppMain->OnPhoneBookDownloadFinish(m_pclist);
		}

		KillTimer(m_hBTWndHidden,TIMER_BT_UPDATE_PHONEBOOK);
	}
}
void CPanelBluetoothWrapper::AutoDownloadPhonebook()
{
	/*
	ShowNotificationBox(m_hMainWnd,IDC_SETTING_SET_LANG_RESET_CONFIRM+IDC_SETTING_BTN_LANG_ENGLISH,
		&CPanelBluetoothWrapper::OnNotify,
		this,
		0,
		GetResourceString(IDS_SETTING_LANGUAGE),
		GetResourceString(IDS_RESET_TO_TAKE_EFFECT),
		0);
	*/

	//auto download phonebook:
	if(m_bAutoDownloadPhonebookFlag && m_dwCallStatus==HF_STATE_IDLE && !m_pSkinManager->GetGUI()->IsAnimating())
	{
		RETAILMSG(DEBUG_BT,(_T(">>>>CPanelBT:: DownloadPhonebook.........\r\n")));

		//start download phonebook?
		if(m_ppMain && m_pSkinManager->GetConfig()->GetCurParam()->bContactAutoDownload && SNM_CONNECTION_STATE_CONNECTED == m_hfpStatus)
		{
			if(m_ppMain->GetPhonebookStatus() == CONTACT_NULL )
			{
				m_ppMain->DownloadPhonebook();
			}
		}


		m_bAutoDownloadPhonebookFlag = FALSE;

		KillTimer(m_hBTWndHidden,TIMER_BT_PBDOWNLOAD_NOTIFY);
	}
}
void CPanelBluetoothWrapper::PhoneAnswer1()
{
	if(m_ciCallInfo1)
	{
		if(m_ciCallInfo1->status == HFM_INCOMING_CALL)
		{
			DWORD dwRet = g_BTModule.pApi->apiHF.AnswerCall();

			if (dwRet != ERROR_SUCCESS)
			{
				return;
			}
		}
	}
}
void CPanelBluetoothWrapper::PhoneAnswer2()
{
	if(m_ciCallInfo2)
	{
		if(m_ciCallInfo2->status == HFM_CALL_WAITING)
		{
			DWORD dwRet = g_BTModule.pApi->apiHF.ThreewayCall(HFM_HOLD_ACTIVE_ACCEPT, HFM_NO_INDEX);

			if (dwRet != ERROR_SUCCESS)
			{
				return;
			}
		}
	}
}
void CPanelBluetoothWrapper::PhoneHangup1()
{
	if(m_ciCallInfo1 && m_ciCallInfo2 && m_pdwTotalNumberOfCalls)
	{
		if(*m_pdwTotalNumberOfCalls > 1) //has second call
		{
			if(m_ciCallInfo1->status == HFM_CALL_ACTIVE)
			{
				g_BTModule.pApi->apiHF.ThreewayCall(HFM_RELEASE_ACTIVE_ACCEPT, HFM_NO_INDEX);
			}
			else
			{
				g_BTModule.pApi->apiHF.ThreewayCall(HFM_RELEASE_ALL_HELD_CALL, HFM_NO_INDEX);
			}
		}
		else
		{
			SetTimer(m_hBTWndHidden,TIMER_BT_AUTO_TERMINATE_SPEECH,1500,NULL);

			if(m_ciCallInfo1->status == HFM_INCOMING_CALL)
			{
				DWORD dwRet = g_BTModule.pApi->apiHF.RejectCall();

				if (dwRet != ERROR_SUCCESS)
				{
					return;
				}
			}
			else
			{
				DWORD dwRet = g_BTModule.pApi->apiHF.TerminateCall();

				if (dwRet != ERROR_SUCCESS)
				{
					return;
				}
			}
		}
	}
}
void CPanelBluetoothWrapper::PhoneHangup2()
{
	if(m_ciCallInfo2 && m_ciCallInfo1)
	{
		if(m_ciCallInfo2->status == HFM_CALL_ACTIVE)
		{
			//DWORD dwRet = g_BTModule.pApi->apiHF.ThreewayCall(HFM_RELEASE_SPECIFIED_CALL, 1);
			DWORD dwRet = g_BTModule.pApi->apiHF.ThreewayCall(HFM_RELEASE_ACTIVE_ACCEPT, HFM_NO_INDEX);

			if (dwRet != ERROR_SUCCESS)
			{
				return;
			}
		}
		else if(m_ciCallInfo2->status == HFM_OUTGOING_CALL && m_ciCallInfo1->status == HFM_CALL_HELD)
		{
			DWORD dwRet = g_BTModule.pApi->apiHF.ThreewayCall(HFM_RELEASE_ACTIVE_ACCEPT, HFM_NO_INDEX);

			if (dwRet != ERROR_SUCCESS)
			{
				return;
			}
		}
		else if(m_ciCallInfo2->status == HFM_CALL_WAITING)
		{
			DWORD dwRet = g_BTModule.pApi->apiHF.ThreewayCall(HFM_RELEASE_ALL_HELD_CALL, HFM_NO_INDEX);

			if (dwRet != ERROR_SUCCESS)
			{
				return;
			}
		}
	}
}

void CPanelBluetoothWrapper::PhoneTransferAudio(UINT idDirection)
{
	static UINT s_tick=0;

	if(GetTickCount()-s_tick>1000)
	{
		s_tick=GetTickCount();

		if(!m_bChangingAudioDir)
		{
			if(/*m_idAudioDir*/m_idAudioDirManualSet==HFM_AD_IN_DEVICE)
			{
				if(m_bTalking)
				{
					MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,800);//MuteAudio(TRUE,TRUE,_ARM_MUTE_ALL,TRUE,800);
				}

				SetTimer(m_hBTWndHidden,TIMER_BT_CHANGE_AUDIO_DIR,600,NULL);
				m_bChangingAudioDir = TRUE;

				//
				m_idAudioDirManualSet = HFM_AD_IN_GATEWAY;
			}
			else
			{
				if(m_bTalking)
				{
					MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,100);
				}

				g_BTModule.pApi->apiHF.TransferHFPAudio();

// 				if(m_idAudioDir!=0)
// 				{
					m_idAudioDirManualSet = HFM_AD_IN_DEVICE;
// 				}
			}

// 			RETAILMSG(DEBUG_BT,(_T(">>>>CPanelBT:: PhoneTransferAudio :%d.........\r\n"),m_idAudioDirManualSet));
		}
	}

}
void CPanelBluetoothWrapper::OnA2dpStreamingStatus(BOOL bOn)
{
	if(m_bA2dpStreamingOn == bOn)
	{
		return;
	}

	m_bA2dpStreamingOn = bOn;

	UINT idClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);

	switch (idClass)
	{
	case UI_CLASS_PHONELINK:
	case UI_CLASS_EASYCONNECTED:
		{
			if(bOn)
			{
				UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
				if(GetCurrentSrcPlayingStatus() && GET_PANEL_CLASS_ID(idSRC)!=idClass && GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_BT)
				{
					m_idSRCBeforeA2dpStreamingOn = idSRC;
				}
				else
				{
					m_idSRCBeforeA2dpStreamingOn = 0;
				}

				SwitchMediaClassSRC(MAKE_PANEL_ID(idClass,0));
				SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);

			}
			else
			{
				if(m_idSRCBeforeA2dpStreamingOn)
				{
					/*
					SwitchMediaClassSRC(m_idSRCBeforeA2dpStreamingOn);
					::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PLAY,NULL);
					*/
					//reset:
					m_idSRCBeforeA2dpStreamingOn = 0;
				}

			}
		}
		break;
	}

	//monitor a2dp:
	if(bOn)
	{
		if(!m_bMonitorA2dpStatus)
		{
			RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper:: monitor playinfo -------\r\n")));
			SetTimer(m_hBTWndHidden,TIMER_BT_MONITOR_PLAYINFO,1000,NULL);

			m_bMonitorA2dpStatus = TRUE;
		}
	}
	else
	{
		if(m_bMonitorA2dpStatus)
		{
			RETAILMSG(DEBUG_BT,(_T("####CPanelBluetoothWrapper:: stop monitor playinfo -----\r\n")));
			KillTimer(m_hBTWndHidden,TIMER_BT_MONITOR_PLAYINFO);

			m_bMonitorA2dpStatus = FALSE;
		}
	}

}
void CPanelBluetoothWrapper::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("++++CPanelBluetoothWrapper::OnCarPlayMsg %d, %d ++++\r\n"),wParam,lParam));

	switch (wParam)
	{
	case CARPLAY_CONNECT_STATUS:
		{
			if(lParam==1 || lParam==2)//connected or connecting...
			{
				m_bCarPlayConnected = TRUE;

				if(!SetBTPower(FALSE))
				{
					KillTimer(m_hBTWndHidden,TIMER_BT_AUTO_POWER_ON);
					SetTimer(m_hBTWndHidden,TIMER_BT_AUTO_POWER_OFF,1000,NULL);
				}
			}
			else if(lParam==0)//disconnected
			{
				m_bCarPlayConnected = FALSE;

				if(m_bBTPowerOnBeforeCarplay && m_idSystemPowerState==SYS_POWER_ON)
				{
					//SetBTPower(TRUE);
					SetTimer(m_hBTWndHidden,TIMER_BT_AUTO_POWER_ON,5000,NULL);
				}
			}
		}
		break;
	}
}
void CPanelBluetoothWrapper::SwitchPower()
{
	if(m_bBTPowerOn)
	{
		SetBTPower(FALSE);
	}
	else
	{
		SetBTPower(TRUE);
	}
}
BOOL CPanelBluetoothWrapper::SetBTPower(BOOL bOn)
{
	BOOL bRet=TRUE;

	RETAILMSG(1,(_T("####CPanelBluetoothWrapper:: SetBTPower %d ++++\r\n"),bOn));

	if(m_bOperatingPower)
	{
		bRet=FALSE;
		goto Exit;
	}

	if(m_bBTPowerOn==bOn)
	{
		bRet=TRUE;
		goto Exit;
	}

	m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn = bOn;

	if(bOn)
	{
		m_bOperatingPower = TRUE;

		::PostMessage(m_hMainWnd,WM_BLUETOOTH_POWER_STATUS,BT_POWER_OPERATING_ON,NULL);

		HANDLE h = CreateThread(NULL,NULL,CPanelBluetoothWrapper::PowerOnThreadPro,this,NULL,NULL);
		//CeSetThreadPriority(h, CE_THREAD_PRIO_256_BELOW_NORMAL);
		CloseHandle(h);
	}
	else
	{
		m_bOperatingPower = TRUE;

		::PostMessage(m_hMainWnd,WM_BLUETOOTH_POWER_STATUS,BT_POWER_OPERATING_OFF,NULL);

		HANDLE h = CreateThread(NULL,NULL,CPanelBluetoothWrapper::PowerOffThreadPro,this,NULL,NULL);
		//CeSetThreadPriority(h, CE_THREAD_PRIO_256_BELOW_NORMAL);
		CloseHandle(h);
	}

Exit:
	RETAILMSG(1,(_T("####CPanelBluetoothWrapper:: SetBTPower %d , bRet:%d++++\r\n"),bOn,bRet));

	return bRet;
}
void CPanelBluetoothWrapper::PowerOn()
{
	BOOL bRet=TRUE;

	if(!m_bBTPowerOn)
	{
		bRet = g_BTModule.pApi->apiSearchPair.SetBTOn();

		Sleep(1000);

		if(bRet)
		{
			g_BTModule.pApi->apiSearchPair.StartAutoConnect();
			m_bBTPowerOn = TRUE;
		}
	}

	m_bOperatingPower = FALSE;

	::PostMessage(m_hMainWnd,WM_BLUETOOTH_POWER_STATUS,bRet?BT_POWER_ON:BT_POWER_OFF,NULL);

}
BOOL CPanelBluetoothWrapper::OnAutoPowerOn()
{
	BOOL bRet=FALSE;

	if(!m_bCarPlayConnected && m_idSystemPowerState==SYS_POWER_ON)
	{
		bRet = SetBTPower(TRUE);
		RETAILMSG(1,(_T("####CPanelBluetoothWrapper:: OnAutoPowerOn : bRet:%d\r\n"),bRet));
	}
	else
	{
		bRet = TRUE;
	}

	return bRet;
}
BOOL CPanelBluetoothWrapper::OnAutoPowerOff()
{
	BOOL bRet=FALSE;

	 bRet = SetBTPower(FALSE);

	 RETAILMSG(1,(_T("####CPanelBluetoothWrapper:: OnAutoPowerOff : bRet:%d\r\n"),bRet));

	 return bRet;
}
void CPanelBluetoothWrapper::OnMonitorPlayInfo()
{
	UINT idClassSRC=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);
	UINT idClassUI=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);

	if(m_bA2dpStreamingOn)
	{
		if(idClassSRC == UI_CLASS_BT ||
			idClassSRC == UI_CLASS_PHONELINK ||
			idClassSRC == UI_CLASS_EASYCONNECTED ||
			idClassUI == UI_CLASS_PHONELINK ||
			idClassUI == UI_CLASS_EASYCONNECTED)
		{
			g_BTModule.pApi->apiAvrcp.GetPlayTime();

			if(m_iVolumeSet == 0)
			{
				RETAILMSG(1/*DEBUG_BT*/,(_T("@@@@@@ A2DP: OnMonitorPlayInfo recover volume\r\n")));
				MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,200);
				SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);
			}
		}
		else
		{
			Pause();
		}
	}
}
void CPanelBluetoothWrapper::PowerOff()
{
	BOOL bRet=TRUE;

	if(m_bBTPowerOn)
	{
// 		g_BTModule.pApi->apiSearchPair.SetAutoConnectFlag();
		g_BTModule.pApi->apiSearchPair.StopAutoConnect();

		if(m_ppMain)
		{
			m_ppMain->Disconnect();
		}

		Sleep(1000);

		bRet = g_BTModule.pApi->apiSearchPair.SetBTOff();
		if(bRet)
		{
			m_bBTPowerOn = FALSE;

			//
			if(m_pStatusBar)
			{
				m_pStatusBar->SetBTStatus(0,-1,-1);
			}
		}

	}

	m_bOperatingPower = FALSE;

	::PostMessage(m_hMainWnd,WM_BLUETOOTH_POWER_STATUS,bRet?BT_POWER_OFF:BT_POWER_ON,NULL);

}
DWORD CPanelBluetoothWrapper::PowerOnThreadPro(LPVOID lpParam)
{
	RETAILMSG(DEBUG_BT,(_T("@@@@@@ PowerOnThreadPro enter\r\n")));

	CPanelBluetoothWrapper *p= (CPanelBluetoothWrapper*)lpParam;

	if(p)
	{
		p->PowerOn();
	}

	RETAILMSG(DEBUG_BT,(_T("@@@@@@ PowerOnThreadPro quit\r\n")));

	return 0;
}
DWORD CPanelBluetoothWrapper::PowerOffThreadPro(LPVOID lpParam)
{
	RETAILMSG(DEBUG_BT,(_T("@@@@@@ PowerOffThreadPro enter\r\n")));

	CPanelBluetoothWrapper *p= (CPanelBluetoothWrapper*)lpParam;

	if(p)
	{
		p->PowerOff();
	}

	RETAILMSG(DEBUG_BT,(_T("@@@@@@ PowerOffThreadPro quit\r\n")));

	return 0;
}
void CPanelBluetoothWrapper::GetDeviceName(LPTSTR lpsz)
{
	HKEY hk = NULL;

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Ident"), 0, KEY_READ, &hk))
	{
		DWORD dwSize = BTENUM_MAX_NAME;
		DWORD dwType = 0;

		if ((ERROR_SUCCESS != RegQueryValueEx(hk, L"Name", NULL, &dwType, (BYTE *)lpsz, &dwSize)) ||
			(dwType != REG_SZ) || (dwSize > BTENUM_MAX_NAME))
		{
			SetDeviceName(DEFAULT_NAME);
			_tcscpy(lpsz, DEFAULT_NAME);
		}

		RegCloseKey(hk);
	}
	else
	{
		SetDeviceName(DEFAULT_NAME);
		_tcscpy(lpsz, DEFAULT_NAME);
	}
}

BOOL CPanelBluetoothWrapper::SetDeviceName(LPTSTR lpsz)
{
	HKEY hk = NULL;
	DWORD dwDisp;
	BOOL bRet=FALSE;

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Ident"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hk, &dwDisp))
	{
		RegSetValueEx(hk, L"Name", 0, REG_SZ, (BYTE *)lpsz, (_tcslen(lpsz) + 1) * sizeof(TCHAR));

		RegCloseKey(hk);
		//RegFlushKey(HKEY_LOCAL_MACHINE);

		// Make Bluetooth local name updated through "HCI_Write_Local_Name"(0x0c13: OGF = 0x03, OCF = 0x0013)
		bRet = g_BTModule.pApi->apiSearchPair.BthWriteLocalName(lpsz);

		if(m_ppMain)
		{
			m_ppMain->SetDeviceName(lpsz);
		}

		return bRet;
	}
	else
	{
		BTDBG(ZONE_ERROR, (TEXT("CDlgBtSettings::SetName ERROR \r\n")));

		return FALSE;
	}
}


void CPanelBluetoothWrapper::GetPIN(LPTSTR lpsz)
{
	HKEY hk = NULL;

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, BT_SETTING_REG_KEY, 0, KEY_READ, &hk))
	{
		DWORD dwSize = MAX_PIN_LEN + 1;
		DWORD dwType = 0;

		if ((ERROR_SUCCESS != RegQueryValueEx(hk, L"PIN", NULL, &dwType, (BYTE *)lpsz, &dwSize)) ||
			(dwType != REG_SZ) || (dwSize > MAX_PIN_LEN + 1))
		{
			SetPIN(DEFAULT_PIN);
			_tcscpy(lpsz, DEFAULT_PIN);
		}

		RegCloseKey(hk);
	}
	else
	{
		SetPIN(DEFAULT_PIN);
		_tcscpy(lpsz, DEFAULT_PIN);
	}
}

BOOL CPanelBluetoothWrapper::SetPIN(LPTSTR lpsz)
{
	HKEY hk = NULL;
	DWORD dwDisp;

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, BT_SETTING_REG_KEY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hk, &dwDisp))
	{
		RegSetValueEx(hk, L"PIN", 0, REG_SZ, (BYTE *)lpsz, (_tcslen(lpsz) + 1) * sizeof(TCHAR));

		RegCloseKey(hk);
		//RegFlushKey(HKEY_LOCAL_MACHINE);

		memset(g_byaDefaultPIN, 0, sizeof(g_byaDefaultPIN));
		WideCharToMultiByte(0, 0, lpsz, _tcslen(lpsz), (LPSTR)g_byaDefaultPIN, sizeof(g_byaDefaultPIN), NULL, NULL);
		return TRUE;
	}
	else
	{
		BTDBG(ZONE_ERROR, (TEXT("CDlgBtSettings::SetPIN ERROR \r\n")));
		return FALSE;
	}
}

void CPanelBluetoothWrapper::MuteAudio(BOOL bMute,BOOL bFade,UINT type,BOOL bAutoRelease,UINT time_out)
{
// 	RETAILMSG(DEBUG_BT,(_T("------CPanelBluetoothWrapper::MuteAudio:%d,bAutoRelease:%d---\r\n"),bMute,bAutoRelease));

	//if(!m_bSpeechOn)
	{
		if(time_out<100)
			time_out=100;

		::PostMessage(m_hMainWnd,WM_MUTE_AUDIO_CMD,(bMute<<24)+(bFade<<16)+type,(bAutoRelease<<16)+(time_out));
	}
}
void CPanelBluetoothWrapper::OnSystemPowerState(UINT idState)
{
	CPanelGL::OnSystemPowerState(idState);

	switch (idState)
	{
	case SYS_POWER_OFF:
	case SYS_POWER_SOFT_RESET:
		{
			Pause();
		}
		break;
	case SYS_POWER_IDLE:
		{
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
			{
				//pause
				Pause();
			}
		}
		break;
	case SYS_POWER_ON:
		{
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
			{
				//play
				//Play();
			}
		}
		break;
	}

}
