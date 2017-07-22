#include "PanelBluetoothWrapper.h"
#include "../Media/MediaCommon.h"
#include "../../CarPlay/CarplayAdapter.h"
#if CVTE_EN_KEY_PROXY
#include "../Key/CKeyHandler.h"
#endif

#define OPPS_FILE_RECEIVE_UNKOWN           0
#define OPPS_FILE_RECEIVE_REMIND           1
#define OPPS_FILE_RECEIVE_ALWAYS_ACCEPT    2
#define OPPS_FILE_RECEIVE_ALWAYS_REFUSE    3

#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO || CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
#define BT_INIT_MODULE_TICK     15000
#else
#define BT_INIT_MODULE_TICK     20000
#endif

CPanelBluetoothWrapper *g_pBtWrapper=NULL;
HANDLE gh_BTDAD=NULL;
extern BOOL	g_bBTModuleLoaded;
extern const wchar_t* g_strInstallPath;

CPanelBluetoothWrapper::CPanelBluetoothWrapper(void)
{
	m_ppMain = NULL;
	m_ppTalking = NULL;
	m_ppA2dp = NULL;

	m_bBTPowerOn = FALSE;//TRUE;
	m_bBTPowerOnBeforeCarplay = FALSE;//TRUE;
	m_bSpeechOn = FALSE;
	m_bCarPlayConnected = FALSE;
	m_bBTPowerOnTarget = FALSE;
	m_bBTManualPowerOn = FALSE;

	//
	bisInit = false;
	bisneedplay = false;
	bMute = false;
	CallStatus[0].callSetup = HF_STATUS_CALL_IDLE;
	CallStatus[1].callSetup = HF_STATUS_CALL_IDLE;
	ConnectStatus = HF_STATUS_DISCONNECTED;
	AudioStatus = HF_AUDIO_NONE;
	a2dpStatus = BT_AV_DISCONNECTED;
	playStatus = PLAYER_STATE_STOP;
	m_phonebook = 0;
	m_enumOppAcceptMethod = OPPS_FILE_RECEIVE_REMIND;
	m_micState = MICPHONE_MUTE_STATE_UNMUTED;

	//
	m_pvInquire = NULL;
	m_pvPaired = NULL;
	m_pvContacts = NULL;
	m_pvAllCalls = NULL;
	m_pvDialCalls = NULL;
	m_pvMissCalls = NULL;
	m_pvReceivCalls = NULL;
	memset(&m_strRequestDeviceInfo,0,sizeof(strRequestDeviceInfo));

	m_dwSignal = -1;
	m_dwBattery = -1;

	m_bCurSrcEnableA2dp = FALSE;
	m_bAutoDownloadPhonebookFlag = FALSE;
	m_bA2dpVolumeOn = FALSE;
	m_dwA2dpVolumeOnTick = 0;
	m_idSRCBeforeA2dpOn = 0;
	m_bInitializing = FALSE;
	m_dwInitializeTick = 0;
	m_bA2dpMonitoring = FALSE;
	m_bMediaOnMute = FALSE;

	m_bPreparePlay = FALSE;
	m_bPreparePause = FALSE;

	m_bAutoUnmuteMic = TRUE;

	m_bHfpConnected = FALSE;
	m_bA2dpConnected = FALSE;
	m_dwA2dpAutoConnectCnt = 0;

	m_strCurDeviceID[0] =0;
	m_bSetModuleName = FALSE;
	m_bGetModuleInfo = FALSE;
	m_bGetIniSettingValue = FALSE;
	m_bGetLocalAdress = FALSE;

	m_dwCallStatus=HF_STATE_IDLE;
	m_bManualPlay=FALSE;

	m_strConnectedDevAddr[0]=0;
	m_strConnectedDevID[0]=0;
	m_strConnectedDevName[0]=0;
#if BT_PHONEBOOK_AUTOSTORE
	m_bNeedLoadStoredContact = FALSE;
#endif
	m_bRefrehPhonebookFlag = FALSE;
	m_bPhonebookDownloading = FALSE;
	m_dwConnectTick = 0;
	m_pairRequestCallback = NULL;
	m_paraPairRequestCallback = 0;
	m_bAndroidAutoConnect = FALSE;
	m_strAndroidAutoDevAddr[0] = 0;
}

CPanelBluetoothWrapper::~CPanelBluetoothWrapper(void)
{

}
void CPanelBluetoothWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_BT_NOTIFYBOX_PIN_REQUEST:
		if(wMsg == NB_OK)
		{
			BT_PairDevice(m_strRequestDeviceInfo.ID, PAIR_ACTION_ACCEPT_REMOTE);
		}
		else
		{
			BT_PairDevice(m_strRequestDeviceInfo.ID, PAIR_ACTION_REFUSE_REMOTE);
		}
		PlaySoundEffect(0);
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
void CPanelBluetoothWrapper::OnBluetoothCommand(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnBluetoothCommand]: %x,%x\r\n"),wParam,lParam));

	switch (wParam)
	{
	case BT_CMD_POWER:
		{
			m_bBTManualPowerOn = lParam;
			m_bBTPowerOnBeforeCarplay = lParam;
			SetBTPower(lParam);
		}
		break;
	case BT_CMD_A2DP_VOLUME:
		{

		}
		break;
	case BT_CMD_DISCONNECT_DEVICE:
		{
			if( m_strConnectedDevID[0])
			{
				RETAILMSG(1,(_T("MSG:BT_CMD_DISCONNECT_DEVICE: %s\r\n"),m_strConnectedDevAddr));
				BT_ConnectPairedDevice(m_strConnectedDevID, DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
				m_strConnectedDevID[0] = 0;
			}
		}
		break;
	case BT_CMD_DOWNLOAD_PHONEBOOK:
		{
			if(!(m_bAutoDownloadPhonebookFlag && GetTickCount()-m_dwConnectTick < 6000))
			{
				RETAILMSG(1,(_T("==BT_CMD_DOWNLOAD_PHONEBOOK==\r\n")));
				BT_DownloadPB(DOWNLOAD_PB_ACTION_YES);
				SetTimer(m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_PB,5000,BTTimerProc);
			}
		}
		break;
	case BT_CMD_DOWNLOAD_CALLLOG:
		{
			if(!(m_bAutoDownloadPhonebookFlag && GetTickCount()-m_dwConnectTick < 6000))
			{
				RETAILMSG(1,(_T("==BT_CMD_DOWNLOAD_CALLLOG==\r\n")));
				BT_GetCallLog(CALLLOG_TYPE_ALL);
				SetTimer(m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_CALLLOG,5000,BTTimerProc);
			}
		}
		break;
	case BT_CMD_GET_A2DP_STREAM:
		{
			*((BOOL*)lParam) = (a2dpStatus==BT_AV_STREAMING) || (playStatus==PLAYER_STATE_PLAY);
		}
		break;
	case BT_CMD_MUTE:
		{

		}
		break;
	case BT_CMD_SET_AUTO_CONNECT:
		{
			if(bisInit)
			{
				BT_SetIniSettingValue(SETUP_MENU_TYPE_AUTOCONNECT, lParam?AUTO_CONNECT_MODE_ENABLED:AUTO_CONNECT_MODE_DISABLED);
			}
		}
		break;
	case BT_CMD_SET_AUTO_ANSWER:
		{
			if(bisInit)
			{
				BT_SetIniSettingValue(SETUP_MENU_TYPE_AUTOANSWER, lParam?AUTO_ANSWER_MODE_ENABLED:AUTO_ANSWER_MODE_DISABLED);
			}
		}
		break;
	case BT_CMD_SET_AUTO_SYNCPB:
		{
			if(bisInit)
			{
				BT_SetIniSettingValue(SETUP_MENU_TYPE_AUTOSYNCPB, lParam?AUTO_SYNC_PB_MODE_ENABLED:AUTO_SYNC_PB_MODE_DISABLED);
			}
		}
		break;
	case BT_CMD_SET_VCARD_NAME_ORDER:
		{
// 			if(bisInit)
// 			{
// 				BT_SetIniSettingValue(SETUP_MENU_TYPE_VCARDNAMEORDER, lParam?VCARD_NAME_ORDER_FAMILLY_LAST:VCARD_NAME_ORDER_FAMILLY_FIRST);
// 			}
			//reset contact and call history:
			RETAILMSG(1,(_T(">>BT_CMD_SET_VCARD_NAME_ORDER: %d\r\n"),GetTickCount()));

			if(m_ppMain)
			{
				if(m_pvContacts)
				{
					m_ppMain->ClearContactList();
					for(int i=0; i<m_pvContacts->size(); i++)
					{
						m_ppMain->AddContact(&m_pvContacts->at(i));
					}
				}
				//if current phone is ios, reset call log.
				//some andriod phone's call log name order is correct and do not need adjust.
				//the way by looking for the string  "iPhone"
				//is not effective to determine a phone is ios or not.
				//FIX ME if a better way is found. by wyp
				if (m_strConnectedDevName[0])
				{
					if(wcsstr(m_strConnectedDevName,_T("iPhone")))
					{
						if(m_pvAllCalls)
						{
							m_ppMain->ClearHistoryList();
							for(int i=0; i<m_pvAllCalls->size(); i++)
							{
								m_ppMain->AddCallLog(&m_pvAllCalls->at(i),TRUE);
							}
						}
					}
				}
			}
			RETAILMSG(1,(_T(">>BT_CMD_SET_VCARD_NAME_ORDER: %d\r\n"),GetTickCount()));

		}
		break;	
	case BT_CMD_SET_24H_TIME_MODE:
		{
			if(m_ppMain)
			{
				if(m_pvAllCalls)
				{
					m_ppMain->ClearHistoryList();
					for(int i=0; i<m_pvAllCalls->size(); i++)
					{
						m_ppMain->AddCallLog(&m_pvAllCalls->at(i),TRUE);
					}
				}
			}		
		}		
		break;
	case BT_CMD_SET_DEVICE_NAME:
		{
			if(bisInit)
			{
				BT_SetModuleName((const wchar_t*)lParam);
			}

			CReg reg;
			reg.Open(HKEY_LOCAL_MACHINE,_T("Ident\\"));
			reg.SetSZ(_T("Name"),(const wchar_t*)lParam);
		}
		break;
	case BT_CMD_SET_PIN:
		{

		}
		break;
	case BT_CMD_HFP_CONTROL:
		{
			if(bisInit && m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn == TRUE)
			{
				switch (lParam)
				{
				case HFP_CONTROL_ANSWER1:
					BT_Answer();
					break;
				case HFP_CONTROL_ANSWER2:
					BT_Answer();
					break;
				case HFP_CONTROL_HANGUP1:
					BT_Hangup(0);
					break;
				case HFP_CONTROL_HANGUP2:
					BT_Hangup(1);
					break;
				case HFP_CONTROL_ENDINCOMINGCALL:
					BT_MultiCallsControl(MULTICALLS_CONTROL_TYPE_ENDINCOMINGCALL);
					break;
				case HFP_CONTROL_HOLD1STCALL:
					BT_MultiCallsControl(MULTICALLS_CONTROL_TYPE_HOLD1STCALL);
					break;
				case HFP_CONTROL_END1STCALL:
					BT_MultiCallsControl(MULTICALLS_CONTROL_TYPE_END1STCALL);
					break;
				case HFP_CONTROL_SWITCHCALLS:
					BT_MultiCallsControl(MULTICALLS_CONTROL_TYPE_SWITCHCALLS);
					break;
				case HFP_CONTROL_ACH:
					BT_AudioTransfer(AudioStatus == HF_AUDIO_CONNECTED?AUDIO_TRANSFER_MODE_PHONE:AUDIO_TRANSFER_MODE_HANDFREE);
					break;
				case HFP_CONTROL_MIC_MUTE:
					BT_MicControl(m_micState==MICPHONE_MUTE_STATE_MUTED?MICPHONE_MUTE_STATE_UNMUTED:MICPHONE_MUTE_STATE_MUTED);
					break;
				}
			}
		}
		break;
	case BT_CMD_A2DP_CONTROL:
		{
			if(bisInit && m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn == TRUE)
			{
				switch (lParam)
				{
				case A2DP_CONTROL_ENABLE:
					EnableA2DP(true,__FUNCTION__);
					break;
				case A2DP_CONTROL_DISABLE:
					EnableA2DP(false,__FUNCTION__);
					break;
				case A2DP_CONTROL_PLAY:
					//Play();
					RETAILMSG(1, (TEXT("MSG:[CPanelBluetoothWrapper::OnBluetoothCommand] BT_CMD_A2DP_CONTROL->A2DP_CONTROL_PLAY\r\n")));
					PreparePlay();
					m_bManualPlay = TRUE;
					break;
				case A2DP_CONTROL_PAUSE:
					//Pause();
					PreparePause();
					m_bManualPlay = FALSE;
					break;
				case A2DP_CONTROL_NEXT:
					PlayNext();
					break;
				case A2DP_CONTROL_PREVIOUS:
					PlayPrevious();
					break;
				case A2DP_CONTROL_PLAYMODE_REPEATALL:
					break;
				case A2DP_CONTROL_PLAYMODE_REPEATONE:
					break;
				case A2DP_CONTROL_PLAYMODE_SHUFFLE:
					break;
				case A2DP_CONTROL_FF:
					PlayFF();
					break;
				case A2DP_CONTROL_RW:
					PlayRW();
					break;
				}
			}
		}
		break;
	}
}
void CPanelBluetoothWrapper::PlayNext()
{
	BT_MusicControl(PLAY_CTL_TYPE_NEXT);
	m_ppA2dp->PlayInfoFlyOut(1);

}
void CPanelBluetoothWrapper::PlayPrevious()
{
	BT_MusicControl(PLAY_CTL_TYPE_PREVIOUS);
	m_ppA2dp->PlayInfoFlyOut(0);

}
void CPanelBluetoothWrapper::PlayFF()
{
	BT_MusicControl(PLAY_CTL_TYPE_FAST_F);

}
void CPanelBluetoothWrapper::PlayRW()
{
	BT_MusicControl(PLAY_CTL_TYPE_FAST_R);

}

void CPanelBluetoothWrapper::PreparePlay()
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::PreparePlay]: playStatus:%d\r\n"),playStatus));

	m_bPreparePlay = TRUE;
	m_bPreparePause = FALSE;

	//DuckAudio(TRUE,0,FALSE);
	EnableA2DP(true,__FUNCTION__);

	if((playStatus!=PLAYER_STATE_PLAY))
	{
		BT_MusicControl(PLAY_CTL_TYPE_PLAY);
	}

	SetTimer(m_hMainWnd,TIMER_MEDIA_DELAY_OPERATION,/*1000*/10,BTTimerProc);
}
void CPanelBluetoothWrapper::PreparePause()
{
	m_bPreparePlay = FALSE;
	m_bPreparePause = TRUE;

// 	if((playStatus==PLAYER_STATE_PLAY))
// 	{
// 		DuckAudio(TRUE,0,TRUE);
// 	}
	SetTimer(m_hMainWnd,TIMER_MEDIA_DELAY_OPERATION,/*1000*/10,BTTimerProc);
}
void CPanelBluetoothWrapper::Play()
{
	if((playStatus!=PLAYER_STATE_PLAY))
	{
		//MuteAudio(TRUE,FALSE,_ARM_AUDIO_MUTE_ALL,TRUE,1000);
		BT_MusicControl(PLAY_CTL_TYPE_PLAY);
	}
	EnableA2DP(true,__FUNCTION__);
}
void CPanelBluetoothWrapper::Pause()
{
	if((playStatus==PLAYER_STATE_PLAY))
	{
		BT_MusicControl(PLAY_CTL_TYPE_PAUSE);
	// 	EnableA2DP(false,__FUNCTION__);
	//	MuteAudio(TRUE,TRUE,_ARM_AUDIO_MUTE_ALL,TRUE,2000);
	}
}
void CPanelBluetoothWrapper::PlayPause()
{
	BT_MusicControl(PLAY_CTL_TYPE_PAUSE_PLAY);

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
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnSpeechStart] \r\n")));

	m_bSpeechOn = TRUE;
#if !USE_WAV_ONCHIP
	/*
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
			//SetA2dpVolume(dwVol);
			BT_A2DPSoundControl(FALSE);
		}
		else
		{
			//SetA2dpVolume(0,FALSE);
		}
	}
	*/

	UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

	if(GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_BT
		|| GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_PHONELINK
		|| GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_EASYCONNECTED)
	{
		OnBT_A2DPSoundControl(FALSE,-1);
		MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,FALSE,2000);
	}
#endif
}
void CPanelBluetoothWrapper::OnSpeechTerminate()
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnSpeechTerminate]\r\n")));

	m_bSpeechOn = FALSE;
#if !USE_WAV_ONCHIP
	/*
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
	{
		//pull up volume
		SetA2dpVolume(A2DP_MAX_VOL);
	}
	*/

	UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

	if(GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_BT
		|| GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_PHONELINK
		|| GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_EASYCONNECTED)
	{
		if(m_bCurSrcEnableA2dp && !m_bMediaOnMute)
		{
			OnBT_A2DPSoundControl(TRUE,-1);
		}
	}
#endif
}

void CPanelBluetoothWrapper::Render()
{
	if(!BeginRender())return;

	EndRender();
}
BOOL CPanelBluetoothWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	g_pBtWrapper = this;

	m_bBTManualPowerOn = m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn;
	m_bBTPowerOnBeforeCarplay = m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn;

	//bt power:
	if(m_bCarPlayConnected || !m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
	{
		m_bBTPowerOn = FALSE;
		OnNotifyPowerStatus(FALSE);
	}
	else
	{
// 		IniBTModule();

// 		//delay load bt:
// 		SetTimer(m_hMainWnd,TIMER_BT_AUTO_LOAD_MODULE,5000,BTTimerProc);

// 		m_bBTPowerOn = TRUE;
// 		m_bBTPowerOnBeforeCarplay = TRUE;

		DWORD dwTick=GetTickCount();
		if(dwTick>=BT_INIT_MODULE_TICK)		//Wuhaoheng change from 25000 to 15000
		{
			IniBTModule();
		}
		else
		{
			RETAILMSG(1/*DEBUG_BT*/, (TEXT("MSG:[CPanelBluetoothWrapper::Initialize]: delay load bt after %d ms \r\n"), BT_INIT_MODULE_TICK-dwTick));
			//delay load:
			SetTimer(m_hMainWnd,TIMER_BT_AUTO_LOAD_MODULE,BT_INIT_MODULE_TICK-dwTick,BTTimerProc);
		}
	}


	return TRUE;
}
void CPanelBluetoothWrapper::LinkPages(CPanelBluetoothMain *pMain,CPanelBluetoothTalking *pTalking,CPanelBluetoothA2dp *pA2dp)
{
	m_ppMain = pMain;
	m_ppTalking = pTalking;
	m_ppA2dp = pA2dp;
}
BOOL CPanelBluetoothWrapper::IniBTModule()
{
	if(m_bInitializing)
		return TRUE;

	if(!m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
	{
		SetTimer(m_hMainWnd,TIMER_BT_NOTIFY_POWER_OFF,1000,BTTimerProc);
		return FALSE;
	}

	RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::IniBTModule]: %d\r\n"),GetTickCount()));

	m_bInitializing = TRUE;

	if(!bisInit)
	{
		BT_Init(CoCallBack
#if 0
			, NULL, NULL
#endif
			);

		CallStatus[0].callSetup = HF_STATUS_CALL_IDLE;
		CallStatus[1].callSetup = HF_STATUS_CALL_IDLE;
		ConnectStatus = HF_STATUS_DISCONNECTED;
		AudioStatus = HF_AUDIO_NONE;
		m_phonebook = 0;
		a2dpStatus = BT_AV_DISCONNECTED;
		m_micState = MICPHONE_MUTE_STATE_UNMUTED;
	}

	SetTimer(m_hMainWnd,TIMER_BT_NOTIFY_POWER_ON,5000,BTTimerProc);
	SetTimer(m_hMainWnd,TIMER_BT_MONITOR_INI_STATUS,10000,BTTimerProc);

	return TRUE;
}
BOOL CPanelBluetoothWrapper::IsBTModuleIni()
{
	return bisInit;
}

BOOL CPanelBluetoothWrapper::IsPlaying()
{
	return (playStatus != PLAYER_STATE_STOP && playStatus != PLAYER_STATE_PAUSE);
}

void CPanelBluetoothWrapper::OnMonitorA2dpStatus()
{
#if 0
	//RETAILMSG(1/*DEBUG_BT*/, (TEXT("\r\n #### OnMonitorA2dpStatus ####\r\n \r\n")));
	UINT idPanel=m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
	UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

	if(a2dpStatus==BT_AV_STREAMING && playStatus!=PLAYER_STATE_PLAY
		&&!m_bSpeechOn)
	{
		if(idPanel==MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER) ||
			idPanel==MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER))
		{
			RETAILMSG(1/*DEBUG_BT*/, (TEXT("MSG:[CPanelBluetoothWrapper::OnMonitorA2dpStatus]: start play\r\n")));
			//Play();
			PreparePlay();
		}
	}
#endif
}
BOOL CPanelBluetoothWrapper::SetBTPower(BOOL bOn)
{
	RETAILMSG(1/*DEBUG_BT*/, (TEXT("MSG:[CPanelBluetoothWrapper::SetBTPower]: %d\r\n"), bOn));
	/*
	if ((m_bBTPowerOn == FALSE) && (bOn == FALSE))
	{
		return FALSE;
	}
	*/

	m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn = bOn;

	if(bOn)
	{
		if(!bisInit)
		{
			if(!m_bInitializing)
			{
				DWORD dwTick=GetTickCount();
				if(dwTick>=BT_INIT_MODULE_TICK)
				{
					IniBTModule();
				}
				else
				{
					//delay load:
					RETAILMSG(1/*DEBUG_BT*/, (TEXT("MSG:[CPanelBluetoothWrapper::SetBTPower]: delay load bt after %d ms\r\n \r\n"), BT_INIT_MODULE_TICK-dwTick));
					SetTimer(m_hMainWnd,TIMER_BT_AUTO_LOAD_MODULE,BT_INIT_MODULE_TICK-dwTick,BTTimerProc);
				}
			}
		}
		else
		{
			OnNotifyPowerStatus(TRUE);
		}
// 		else if(bisInit)
// 		{
// 			BT_Standby_Off();
// 		}
	}
	else
	{
		if(bisInit)
		{
			//BT_Standby_On();
			BT_DeInit();
			//蓝牙版本1109之后都会发送FLC_BT_EVENT_DEINIT_INDICATOR，因此取消该赋值
			//bisInit = false;
		}
		SetTimer(m_hMainWnd,TIMER_BT_NOTIFY_POWER_OFF,5000/*3000*/,BTTimerProc);

		//quit current page:
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_BT)
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
		}
	}

	return TRUE;
}
void CPanelBluetoothWrapper::OnSRCChanged(UINT idSRC)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: idSRC:0x%x\r\n"), TEXT(__FUNCTION__),idSRC));

	m_bCurSrcEnableA2dp = (GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_BT
		|| GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_PHONELINK
		|| GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_EASYCONNECTED
		|| GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_WEBLINK);


	if(m_bCurSrcEnableA2dp && !m_bSpeechOn && !m_bMediaOnMute)
	{
		RETAILMSG(1,(_T("MSG:[%s]: EnableA2DP \r\n"), TEXT(__FUNCTION__)));
		EnableA2DP(true,__FUNCTION__);
	}
	else
	{
		RETAILMSG(1,(_T("MSG:[%s]: DisableA2DP \r\n"), TEXT(__FUNCTION__)));
		m_bA2dpVolumeOn = FALSE;
		Pause();
		EnableA2DP(false,__FUNCTION__);
	}
}
void CPanelBluetoothWrapper::EnableA2DP(bool bEnable, const char* iReason)
{
	if(bisInit)
	{
		RETAILMSG(1,(_T("#### EnableA2DP:%d ,reason:%S\r\n"),bEnable,iReason));
		BT_A2DPSoundControl(bEnable?true:false,-1);
	}
}

#if CVTE_EN_KEY_PROXY
BOOL CPanelBluetoothWrapper::OnKey(UINT idKey,UINT idStatus)
{
	RETAILMSG(1, (TEXT("MSG:[CPanelBluetoothWrapper::OnKey] idKey:%X\r\n"),idKey));
	if(!bisInit)
	{
		return FALSE;
	}

	switch (idKey)
	{
	case KEY_HOOK_SWITCH:
		{
			if ( idStatus == EV_KEY_UP )
			{
				BT_Answer();
			}
		}
		break;
	case KEY_DROP:
		{
			if ( idStatus == EV_KEY_UP )
			{
				BT_Hangup(0);
			}
		}
		break;
	case KEY_FLASH:
		{
			if ( idStatus == EV_KEY_UP )
			{
				if(m_dwCallStatus == HF_STATE_INCOMING_CALL)
					BT_Answer();
				else if(m_dwCallStatus == HF_STATE_OUTGOING_CALL)
					BT_Hangup(0);
				else if(m_dwCallStatus == HF_STATE_TALKING)
					BT_Hangup(0);
			}
		}
		break;
	case KEY_ACH:
		{
			if ( idStatus == EV_KEY_UP )
			{
				BT_AudioTransfer(AudioStatus == HF_AUDIO_CONNECTED?AUDIO_TRANSFER_MODE_PHONE:AUDIO_TRANSFER_MODE_HANDFREE);
			}
		}
		break;
	case KEY_NEXT:
		{
			if ( idStatus == EV_KEY_SHORT_PRESS )
			{
				PlayNext();
			}
			else if ( idStatus == EV_KEY_LONG_PRESS )
			{
				PlayFF();
			}
		}
		break;
	case KEY_PREVIOUS:
		{
			if ( idStatus == EV_KEY_SHORT_PRESS )
			{
				PlayPrevious();
			}
			else if ( idStatus == EV_KEY_LONG_PRESS )
			{
				PlayRW();
			}
		}
		break;
	case KEY_RESUME:
		{
			if ( idStatus == EV_KEY_UP )
			{
				if(m_bManualPlay)
				{
					SwitchAudioSRC(SRC_AUDIO_ARM);
					//Play();
					PreparePlay();
				}
			}
		}
	case KEY_PLAY:
		{
			if ( idStatus == EV_KEY_UP )
			{
				SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				//Play();
				PreparePlay();
			}
		}
		break;
	case KEY_PAUSE:
		{
			if ( idStatus == EV_KEY_UP )
			{
				RETAILMSG(1, (TEXT("MSG:[%s]: =>PreparePause\r\n"), TEXT(__FUNCTION__)));
				//Pause();
				PreparePause();
			}
		}
		break;
	case KEY_PLAYPAUSE:
		{
			if ( idStatus == EV_KEY_UP )
			{
				PlayPause();
			}
		}
		break;
	case KEY_STOP:
		{
			if ( idStatus == EV_KEY_UP )
			{
				Pause();
			}
		}
		break;
	case KEY_MUTE:
		{
			if ( idStatus == EV_KEY_UP )
			{
				m_bMediaOnMute = !m_bMediaOnMute;
				EnableA2DP(m_bMediaOnMute?false:true,__FUNCTION__);
			}
		}
		break;
	}

	return TRUE;
}
#else
BOOL CPanelBluetoothWrapper::OnKey(UINT idKey)
{
	RETAILMSG(1, (TEXT("MSG:[CPanelBluetoothWrapper::OnKey] idKey:%X\r\n"),idKey));
	if(!bisInit)
	{
		return FALSE;
	}
	switch (idKey)
	{
	case KEY_PHONE_ANSWER:
		{
			BT_Answer();
		}
		break;
	case KEY_PHONE_HANG:
		{
			BT_Hangup(0);
		}
		break;
	case KEY_PHONE_ACH:
		{
			BT_AudioTransfer(AudioStatus == HF_AUDIO_CONNECTED?AUDIO_TRANSFER_MODE_PHONE:AUDIO_TRANSFER_MODE_HANDFREE);
		}
		break;
	case KEY_MEDIA_NEXT:
	case KEY_MCU_TRACK_NEXT:
		{
            RETAILMSG(1, (TEXT("MSG:[CPanelBluetoothWrapper::OnKey] Next\r\n")));
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
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
			//Play();
			PreparePlay();
		}
		break;
	case KEY_MEDIA_PAUSE:
		{
			//Pause();
			PreparePause();
		}
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		{
			PlayPause();
		}
		break;
	case KEY_MEDIA_STOP:
		{
			Pause();
		}
		break;
	case KEY_MEDIA_MUTE:
		{
			m_bMediaOnMute = TRUE;
			EnableA2DP(false,__FUNCTION__);
		}
		break;
	case KEY_MEDIA_UNMUTE:
		{
			m_bMediaOnMute = FALSE;
			EnableA2DP(true,__FUNCTION__);
		}
		break;
	}

	return TRUE;
}
#endif
void CPanelBluetoothWrapper::OnIniStatus(BOOL bSucc)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnIniStatus]: bSucc:%d, %d\r\n"),bSucc,GetTickCount()));

	m_bInitializing = FALSE;
	m_dwInitializeTick = GetTickCount();

	if(bSucc)
	{
		bisInit = TRUE;

// 		OnNotifyPowerStatus(TRUE);

// 		BT_GetModuleInfo();
// 
// 		BT_GetIniSettingValue();
// 
// 		BT_GetLocalAdress();

// 		//reset name:
// 		CReg reg;
// 		WCHAR name_reg[MAX_PATH];
// 		reg.Open(HKEY_LOCAL_MACHINE,_T("Ident\\"));
// 		reg.ValueSZ(_T("Name"),name_reg,MAX_PATH);
// 		BT_SetModuleName(name_reg);

		//notify power on:
		m_bBTPowerOn = TRUE;
		m_bBTPowerOnBeforeCarplay = TRUE;

// 		::PostMessage(m_hMainWnd,WM_BLUETOOTH_SETUP_STATUS,BT_STATUS_POWER,BT_POWER_ON);
// 		m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn = TRUE;

// 		OnNotifyPowerStatus(TRUE);

		g_bBTModuleLoaded=TRUE;

		m_ppMain->OnIniStatus(bSucc);

		//reset BT status:
		bisneedplay = false;
		bMute = false;
		CallStatus[0].callSetup = HF_STATUS_CALL_IDLE;
		CallStatus[1].callSetup = HF_STATUS_CALL_IDLE;
		ConnectStatus = HF_STATUS_DISCONNECTED;
		AudioStatus = HF_AUDIO_NONE;
		a2dpStatus = BT_AV_DISCONNECTED;
		playStatus = PLAYER_STATE_STOP;
		m_phonebook = 0;
		m_enumOppAcceptMethod = OPPS_FILE_RECEIVE_REMIND;
		m_micState = MICPHONE_MUTE_STATE_UNMUTED;
		m_pvInquire = NULL;
		m_pvPaired = NULL;
		m_pvContacts = NULL;
		m_pvAllCalls = NULL;
		m_pvDialCalls = NULL;
		m_pvMissCalls = NULL;
		m_pvReceivCalls = NULL;
		memset(&m_strRequestDeviceInfo,0,sizeof(strRequestDeviceInfo));
		m_dwSignal = -1;
		m_dwBattery = -1;
		m_bCurSrcEnableA2dp = FALSE;
		m_bAutoDownloadPhonebookFlag = FALSE;
		m_bA2dpVolumeOn = FALSE;
		m_dwA2dpVolumeOnTick = 0;
		m_idSRCBeforeA2dpOn = 0;
		m_bInitializing = FALSE;
		m_bA2dpMonitoring = FALSE;
		m_bMediaOnMute = FALSE;
		m_bPreparePlay = FALSE;
		m_bPreparePause = FALSE;
		m_bAutoUnmuteMic = TRUE;
		m_bHfpConnected = FALSE;
		m_bA2dpConnected = FALSE;
		m_dwA2dpAutoConnectCnt = 0;
		m_strCurDeviceID[0] =0;

		m_bSetModuleName = FALSE;
		m_bGetModuleInfo = FALSE;
		m_bGetIniSettingValue = FALSE;
		m_bGetLocalAdress = FALSE;

		//start timer to auto get module info:
		SetTimer(m_hMainWnd,TIMER_BT_AUTO_GET_MODULE_INFO,300,BTTimerProc);
	}
}
void CPanelBluetoothWrapper::OnDeiniStatus(BOOL bSucc)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnDeiniStatus]: bSucc:%d\r\n"),bSucc));

	if(bSucc)
	{
		bisInit = FALSE;
	}
}

//信号强度通知
void CPanelBluetoothWrapper::OnSignalChange(DWORD dwSignal)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnSignalChange]: dwSignal:%d\r\n"),dwSignal));
#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO || CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
	if(dwSignal <= 1)
		dwSignal = 0;
#endif
	m_dwSignal = dwSignal;
	if(m_pStatusBar/* && m_bBTPowerOn*/)
	{
		//主页显示信号强度
		m_pStatusBar->SetBTStatus(ConnectStatus,m_dwSignal,-1);
	}
}

//电池电量通知
void CPanelBluetoothWrapper::OnBatteryChange(DWORD dwBattery)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnBatteryChange] dwBattery:%d\r\n"),dwBattery));
	m_dwBattery = dwBattery;

	if(m_pStatusBar/* && m_bBTPowerOn*/)
	{
		//主页显示电池电量
		m_pStatusBar->SetBTStatus(ConnectStatus,-1,m_dwBattery);
	}
}

//蓝牙连接
void CPanelBluetoothWrapper::OnNotifyHfpConnected(BOOL bConnected)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyHfpConnected]: bConnected:%d\r\n"),bConnected));

	m_bHfpConnected = bConnected;
	if(bConnected)
	{
		m_dwConnectTick = GetTickCount();
	}

#if BT_PHONEBOOK_AUTOSTORE
	m_bNeedLoadStoredContact = bConnected;
#endif

	ConnectStatus = bConnected?HF_STATUS_CONNECTED:HF_STATUS_DISCONNECTED;

	//update widget:
	BOOL benable=bConnected;
	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_PHONE,0,UIOPRT_SHOWHIDE,&benable);

	m_ppMain->OnNotifyHfpConnected(bConnected);

	//update status bar:
	if(m_pStatusBar/* && m_bBTPowerOn*/)
	{
		m_pStatusBar->SetBTStatus(ConnectStatus,m_dwSignal,m_dwBattery);
	}

	//start a timer to monitor a2dp connection if hfp is connected:
	if(bConnected)
	{
		m_dwA2dpAutoConnectCnt = 5;
		SetTimer(m_hMainWnd,TIMER_BT_MONITOR_A2DP_CONNECTION,5000,BTTimerProc);
	}

	//reset phonebook downloading status:
	if(!bConnected && m_bPhonebookDownloading)
	{
		m_bPhonebookDownloading = FALSE;
		m_ppMain->OnUpdatePBFinish(0);
		m_ppMain->OnUpdateCallHistoryFinish(0);
	}
}
void CPanelBluetoothWrapper::OnNotifyA2dpConnected(BOOL bConnected)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyA2dpConnected]: bConnected:%d, bAndroidAutoConnect:%d\r\n"),bConnected,m_bAndroidAutoConnect));

	m_bA2dpConnected = bConnected;

	//if android auto is connected, do not update a2dp status
	//bluetooth audio will played via android auto.
	if(m_bAndroidAutoConnect && bConnected)
		return;

	::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_A2DP,bConnected);

	UINT idPanel=m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
	UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

	if(bConnected &&
		( GET_PANEL_CLASS_ID(idSRC)==UI_CLASS_BT || GET_PANEL_CLASS_ID(idSRC)==UI_CLASS_NULL ||
			idPanel==MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER) ||
			idPanel==MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER) ||
			idPanel==MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER) ||
			idPanel==MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP) ) )
	{
		SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);
		EnableA2DP(true,__FUNCTION__);
	}
	else
	{
		SetA2dpVolume(0);
		EnableA2DP(false,__FUNCTION__);
		m_bA2dpVolumeOn = FALSE;
		m_idSRCBeforeA2dpOn = 0;
	}

	if(m_ppA2dp)
	{
		m_ppA2dp->OnA2dpConnected(bConnected);
		m_ppA2dp->OnAvrcpConnected(bConnected);
	}

	if(m_ppMain)
	{
		m_ppMain->OnNotifyA2dpConnected(bConnected);
	}

	//
// 	if(!bConnected && !m_bCarPlayConnected)
// 	{
// 		if(UI_CLASS_BT == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
// 		{
// 			SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_NONE);
// 		}
// 	}

	//
	if(!bConnected)
	{
		a2dpStatus = BT_AV_DISCONNECTED;
		playStatus = PLAYER_STATE_STOP;

		//stop monitoring...
		if(m_bA2dpMonitoring)
		{
			m_bA2dpMonitoring = FALSE;
			KillTimer(m_hMainWnd,TIMER_BT_MONITOR_A2DP_STATUS);
		}
	}
}
void CPanelBluetoothWrapper::OnNotifyAvrcpConnected(BOOL bConnected)
{

}
void CPanelBluetoothWrapper::OnNotifyDunServiceSupport(BOOL bSupport)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyAvrcpConnected]: bSupport:%d\r\n"),bSupport));

}
void CPanelBluetoothWrapper::OnNotifyPanServiceSupport(BOOL bSupport)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyPanServiceSupport]: bSupport:%d\r\n"),bSupport));

}
void CPanelBluetoothWrapper::OnNotifyPairResult(PAIR_RESULT result)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyPairResult]: %d\r\n"),result));

	m_ppMain->OnNotifyPairResult(result);

	if(PAIR_RESULT_OK != result)
	{
		OnNotifyDeviceListChanged();
	}

#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	HideNotificationBox(m_hMainWnd);
#endif

}
void CPanelBluetoothWrapper::OnNotifyDeviceDelete(LPCTSTR name)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyDeviceDelete]: %s\r\n"),name));

}
void CPanelBluetoothWrapper::OnNotifyCallCount(DWORD dwCount)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyCallCount]: dwCount:%d\r\n"),dwCount));
	m_ppTalking->OnNotifyCallCount(dwCount);

	if(dwCount>0)
	{
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
			if(!m_bAndroidAutoConnect && !m_bCarPlayConnected && 
				m_pSkinManager->GetConfig()->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
			{
				SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING,PAGE_SWITCH_ANI_NONE,0);
			}
			else if(m_bAndroidAutoConnect)
			{
				if (m_pSkinManager->GetConfig()->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER))
					SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
			}
		}

// 		::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,TRUE,HF_STATE_TALKING);

		EnableA2DP(false,__FUNCTION__);
	}
	else
	{
		m_dwCallStatus=HF_STATE_IDLE;
		::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,FALSE,HF_STATE_IDLE);
		if(m_pSmartBarPhone)
		{
			m_pSmartBarPhone->Show(FALSE);
		}

		if(m_bCurSrcEnableA2dp && !m_bSpeechOn && !m_bMediaOnMute)
		{
			EnableA2DP(true,__FUNCTION__);
		}

		m_bAutoUnmuteMic = TRUE;
	}
}
void CPanelBluetoothWrapper::OnNotifyCallStatus(strCallStatus* callStatus)
{
	if(!(callStatus->iCallIndex>=0 && callStatus->iCallIndex<=6))
	{
		RETAILMSG(1/*DEBUG_BT*/,(_T("ERR:[CPanelBluetoothWrapper::OnNotifyCallStatus]: !!! index:%d ERR !!! return !!!!\r\n"),callStatus->iCallIndex));
		return;
	}
	
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyCallStatus]: index:%d, iCallState:%d\r\n"),callStatus->iCallIndex,callStatus->iCallState));

	//
	if(m_pSmartBarPhone && callStatus->iCallIndex==0)
	{
		m_pSmartBarPhone->SetPhoneNumber(callStatus->wcCallerNum);
		switch (callStatus->iCallState)
		{
		case DIAL_STATE_CALLOUT:
			m_pSmartBarPhone->SetType(SB_OUTGOING);
			break;
		case DIAL_STATE_INCOMING:
			m_pSmartBarPhone->SetType(SB_INCOMING);
			break;
		case DIAL_STATE_TALKING:
		default:
			m_pSmartBarPhone->SetType(SB_TALKING);
			break;
		}
	}

#if BT_PHONEBOOK_AUTOSTORE
	//auto adjust call name:
	if (callStatus->wcCallerName[0]==0 || wcscmp(callStatus->wcCallerName,callStatus->wcCallerNum) == 0)
	{
		//invalid caller name, try to get caller name from stored phonebook:
		int search_cnt=0;
		wchar_t *ret=0;
		wchar_t number_tmp[20]={0};
		DWORD dwStart=GetTickCount();
		m_listStoredContacts.Head();
		while (!m_listStoredContacts.End())
		{
			ContactInfo *p=m_listStoredContacts.Get();
			if(p)
			{
				for (int i=0;i<p->PhoneNumber.size();i++)
				{
					wchar_t *cursor=p->PhoneNumber.at(i).number;
					wchar_t *cursor_t=number_tmp;
					int len=wcslen(p->PhoneNumber.at(i).number);
					for (int n=0;n<len;n++)
					{
						if (isdigit(*cursor))
						{
							*cursor_t = *cursor;
							cursor_t++;
						}
						cursor++;
					}
					*cursor_t = 0;
					//RETAILMSG(1/*DEBUG_BT*/,(_T("==wcCallerNum:%s, number: %s\r\n"),callStatus->wcCallerNum,number_tmp));
					if(wcsstr(callStatus->wcCallerNum,number_tmp) != 0)
					{
						ret = p->Name;
						wcscpy_s(callStatus->wcCallerName,32,p->Name);
						break;
					}
				}
			}
			search_cnt++;
			m_listStoredContacts.Next();
			if(ret)
			{
				break;
			}
		}
		RETAILMSG(1/*DEBUG_BT*/,(_T("####Search name for number=%s, ret=%s,search_cnt=%d, take %dms\r\n"),callStatus->wcCallerNum,ret,search_cnt,GetTickCount()-dwStart));
	}
#endif

	m_ppTalking->OnNotifyCallStatus(callStatus);

	switch (callStatus->iCallState)
	{
	case DIAL_STATE_TALKING:
		m_dwCallStatus=HF_STATE_TALKING;
		::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,TRUE,HF_STATE_TALKING);
		//unmute mic
		if(m_bAutoUnmuteMic)
		{
			RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyCallStatus]: auto unmute mic \r\n")));

			m_bAutoUnmuteMic = FALSE;
			BT_MicControl(MICPHONE_MUTE_STATE_UNMUTED);
		}
		break;
	case DIAL_STATE_INCOMING:
		m_dwCallStatus=HF_STATE_INCOMING_CALL;
		::SendMessage(m_hMainWnd,WM_PHONE_CALL_STATE,TRUE,HF_STATE_INCOMING_CALL);
		break;
	case DIAL_STATE_CALLOUT:
		m_dwCallStatus=HF_STATE_OUTGOING_CALL;
		::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,TRUE,HF_STATE_OUTGOING_CALL);
		break;
	}
}
void CPanelBluetoothWrapper::OnNotifyMicState(MICPHONE_MUTE_STATE state)
{
	RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyMicState]: %d\r\n"),state));

	m_micState = state;
	m_ppTalking->SetMicMute(state==MICPHONE_MUTE_STATE_MUTED);
}
void CPanelBluetoothWrapper::OnNotifyAudioTransferMode(AUDIO_TRANSFER_MODE mode)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyAudioTransferMode]: %d\r\n"),mode));
	if(AUDIO_TRANSFER_MODE_HANDFREE == mode)
	{
		AudioStatus = HF_AUDIO_CONNECTED;
	}
	else
	{
		AudioStatus = HF_AUDIO_NONE;
	}
	m_ppTalking->SetAudioDir(AudioStatus == HF_AUDIO_CONNECTED);

	if(m_pSmartBarPhone)
	{
		m_pSmartBarPhone->SetAudioDir(AudioStatus == HF_AUDIO_CONNECTED);
	}

	//auto change to bluetooth if android auto connected:
	if(m_bAndroidAutoConnect && AUDIO_TRANSFER_MODE_HANDFREE != mode)
	{
		RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyAudioTransferMode]: %d, auto switch to handfree because AAP connected\r\n"),mode));
		//BT_AudioTransfer(AUDIO_TRANSFER_MODE_HANDFREE);
		//SetTimer(m_hMainWnd,TIMER_BT_AUTO_CHANGE_AUDIO_DIR,2000,BTTimerProc);
		HANDLE hdl=CreateThread(NULL,NULL,AutoChangeAudioDirThreadPro,this,NULL,NULL);
		CeSetThreadPriority(hdl, CE_THREAD_PRIO_256_ABOVE_NORMAL);
		CloseHandle(hdl);
	}
}
void CPanelBluetoothWrapper::OnNotifyPBSyncState(PB_SYNC_STATE state)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyPBSyncState]: %d\r\n"),state));

	if(state == PB_SYNC_STATE_START)
	{
		m_bPhonebookDownloading = TRUE;
		m_ppMain->OnNotifyStartSyncContact();
	}
	else
	{
		if(m_bPhonebookDownloading)
		{
			m_bPhonebookDownloading = FALSE;
			m_ppMain->OnUpdatePBFinish(0);
			m_ppMain->OnUpdateCallHistoryFinish(0);
		}
	}
}
void CPanelBluetoothWrapper::OnNotifyAudioSourceState(AUDIO_SOURCE src,AUDIO_STATE state)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyAudioSourceState]: src:%d,state:%d,m_bA2dpVolumeOn:%d\r\n"),src,state,m_bA2dpVolumeOn));

	if(src==AUDIO_SOURCE_STREAMING)
	{
		UINT idPanel=m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
		UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
		BOOL bA2dpStreamingOn=(state==AUDIO_STATE_ACTIVE);
		a2dpStatus = bA2dpStreamingOn?BT_AV_STREAMING:BT_AV_DISCONNECTED;

		if(bA2dpStreamingOn)
		{
			if(!m_bA2dpMonitoring)
			{
				m_bA2dpMonitoring = TRUE;
				SetTimer(m_hMainWnd,TIMER_BT_MONITOR_A2DP_STATUS,5000,BTTimerProc);
			}
		}
		else
		{
			if(m_bA2dpMonitoring)
			{
				m_bA2dpMonitoring = FALSE;
				KillTimer(m_hMainWnd,TIMER_BT_MONITOR_A2DP_STATUS);
			}
		}
		//auto a2dp volume out:
		if(bA2dpStreamingOn && !m_bA2dpVolumeOn)
		{
			if(idPanel==MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER) ||
				idPanel==MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER) ||
				idPanel==MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER) ||
				idPanel==MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP))
			{
				m_bA2dpVolumeOn = TRUE;
				m_dwA2dpVolumeOnTick = GetTickCount();

				RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyAudioSourceState]: auto a2dp volume out:\r\n")));

				m_idSRCBeforeA2dpOn = 0;
				if(GetCurrentSrcPlayingStatus() &&
					GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_BT &&
					GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_PHONELINK &&
					GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_EASYCONNECTED &&
					GET_PANEL_CLASS_ID(idSRC)!=UI_CLASS_WEBLINK
					)
				{
					m_idSRCBeforeA2dpOn = idSRC;
				}

				SwitchMediaClassSRC(idPanel);
				SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
// 				if(!SwitchAudioSRC(SRC_AUDIO_ARM,FALSE/*TRUE*/))
// 				{
// 					MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,100);
// 				}
				SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);
				if(!m_bPreparePlay)
				{
					m_bPreparePlay = TRUE;
					//DuckAudio(TRUE,0,FALSE);
					SetTimer(m_hMainWnd,TIMER_MEDIA_DELAY_OPERATION,1000,BTTimerProc);
				}
			}
		}
		else if(!bA2dpStreamingOn && m_bA2dpVolumeOn)
		{
			if(idPanel==MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER) ||
				idPanel==MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER) ||
				idPanel==MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER))
			{
				if(m_idSRCBeforeA2dpOn && GetTickCount()-m_dwA2dpVolumeOnTick<20000)//if >20s, music mode, else, navi mode
				{
					EnableA2DP(false,__FUNCTION__);

					SwitchMediaClassSRC(m_idSRCBeforeA2dpOn);
					#if CVTE_EN_KEY_PROXY
					CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
					CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);					
					#else
					::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PLAY,NULL);
					#endif
					//reset:
					m_idSRCBeforeA2dpOn = 0;
				}
			}
		}
	}
	else if(src==AUDIO_SOURCE_COMM)
	{

	}
}
void CPanelBluetoothWrapper::OnNotifyA2dpPlayState(PLAYER_STATE state)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyA2dpPlayState] :%d ,m_bA2dpVolumeOn:%d\r\n"),state,m_bA2dpVolumeOn));

	if (m_bAndroidAutoConnect)
	{
		return;
	}

	playStatus = state;
	m_ppA2dp->SetPlayState(state);

	//auto a2dp volume out:
	if(state == PLAYER_STATE_PLAY && !m_bA2dpVolumeOn)
	{
		UINT idPanel=m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;
		switch (idPanel)
		{
		case MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER):
		case MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER):
		case MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER):
		case MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP):
			{
				RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyA2dpPlayState]: a2dp volume out:\r\n")));

				m_bA2dpVolumeOn = TRUE;
				SwitchMediaClassSRC(idPanel);
				SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				if(!m_bPreparePlay)
				{
					m_bPreparePlay = TRUE;
					//DuckAudio(TRUE,0,FALSE);
					SetTimer(m_hMainWnd,TIMER_MEDIA_DELAY_OPERATION,1000,BTTimerProc);
				}
				SetA2dpVolume(m_bSpeechOn?A2DP_LOW_VOL:A2DP_MAX_VOL);
			}
			break;
		}
	}

}
void CPanelBluetoothWrapper::OnNotifyDeviceInquiryFinish()
{
	m_ppMain->OnNotifyDeviceInquiryFinish();

}
void CPanelBluetoothWrapper::OnNotifyID3(strMusicID3 *id3info)
{
	if(!m_bAndroidAutoConnect)
	{
		m_ppA2dp->OnNotifyID3(id3info);
	}
}
void CPanelBluetoothWrapper::OnNotifyAvPlayTime(strAvPlayStatus *info)
{
	if(!info || m_bAndroidAutoConnect)
		return;

	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyAvPlayTime]: position:%d, length:%d, status:%d, \r\n"),info->position,info->length,info->status));

	int secPos=info->position/1000;
	int secLength=info->length/1000;

	if(m_ppA2dp)
	{
		if(secPos<=secLength && secLength>0)
		{
			m_ppA2dp->SetProgressValue(secPos*512/secLength);
			m_ppA2dp->SetElapsedTime(secPos);
			m_ppA2dp->SetRemainTime(secLength-secPos);
		}
		else
		{
			m_ppA2dp->SetProgressValue(0);
			m_ppA2dp->SetElapsedTime(0);
			m_ppA2dp->SetRemainTime(0);
		}
	}
}
void CPanelBluetoothWrapper::OnNotifySetupInI(strSetupInI *ptIni)
{
	DWORD dwStatus=0;

	dwStatus|=(ptIni->bAutoConnect == AUTO_CONNECT_MODE_ENABLED)?0x01:0x00;
	dwStatus|=(ptIni->bAutoAnswer == AUTO_ANSWER_MODE_ENABLED)?0x02:0x00;
	dwStatus|=(ptIni->bAutoSyncPB == AUTO_SYNC_PB_MODE_ENABLED)?0x04:0x00;
	dwStatus|=(ptIni->CallLogType == CALLLOG_SYNC_ALL)?0x08:0x00;
	dwStatus|=(ptIni->bVcardFamilyNameFirst == VCARD_NAME_ORDER_FAMILLY_LAST)?0x10:0x00;

	//force to family first:
	if(ptIni->bVcardFamilyNameFirst != VCARD_NAME_ORDER_FAMILLY_FIRST)
	{
		BT_SetIniSettingValue(SETUP_MENU_TYPE_VCARDNAMEORDER, VCARD_NAME_ORDER_FAMILLY_FIRST);
	}

	::PostMessage(m_hMainWnd,WM_BLUETOOTH_SETUP_STATUS,BT_STATUS_SETUP_INI,dwStatus);

	m_bAutoDownloadPhonebookFlag = (ptIni->bAutoSyncPB == AUTO_SYNC_PB_MODE_ENABLED);

	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifySetupInI]: bAutoSyncPB:%d\r\n"),m_bAutoDownloadPhonebookFlag));

	m_ppMain->OnNotifySetupInI(ptIni);
}
void CPanelBluetoothWrapper::OnNotifyPowerStatus(BOOL bOn)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyPowerStatus]: bOn:%d\r\n"),bOn));
	//m_bInitializing = FALSE;
	m_bBTPowerOn = bOn;
	::PostMessage(m_hMainWnd,WM_BLUETOOTH_SETUP_STATUS,BT_STATUS_POWER,bOn?BT_POWER_ON:BT_POWER_OFF);
	m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn = bOn;

	if(!bOn && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_BT)
	{
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
	}
}
void CPanelBluetoothWrapper::OnNotifyModuleName(LPCTSTR name)
{
	m_strBTName = name;
	::PostMessage(m_hMainWnd,WM_BLUETOOTH_SETUP_STATUS,BT_STATUS_NAME,(LPARAM)(LPCTSTR)m_strBTName.String());

}
void CPanelBluetoothWrapper::OnNotifyModuleVersion(LPCTSTR version)
{
	m_strBTVersion = version;
	::PostMessage(m_hMainWnd,WM_BLUETOOTH_SETUP_STATUS,BT_STATUS_VERSION,(LPARAM)(LPCTSTR)m_strBTVersion.String());
}

void CPanelBluetoothWrapper::OnNotifyPhoneBookChanged()
{
	int cnt=-1;
	m_bPhonebookDownloading = FALSE;
	KillTimer(m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_PB);
	if(m_bRefrehPhonebookFlag || m_bAutoDownloadPhonebookFlag || m_ppMain->GetContactStatus() == CONTACT_DOWNLOADING)
	{
		m_bRefrehPhonebookFlag=FALSE;
		cnt=0;
		//do not update phone-book if cnt==0
		if(m_pvContacts && m_pvContacts->size()>0)
		{
			m_ppMain->ClearContactList();
			RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyPhoneBookChanged]: all cnt:%d\r\n"),m_pvContacts->size()));

#if BT_PHONEBOOK_AUTOSTORE
			FILE	*file = NULL;
			DWORD dwStart=GetTickCount();
			if(m_strConnectedDevAddr[0])
			{
				WCHAR strTmp[MAX_PATH];
				swprintf_s(strTmp,MAX_PATH,_T("%sPhonebook\\"),g_strInstallPath);
				CreateDirectory(strTmp,NULL);
				wcscat_s(strTmp,MAX_PATH,m_strConnectedDevAddr);
				wcscat_s(strTmp,MAX_PATH,_T(".pb"));

				RETAILMSG(1,(_T("\n*************Begin SavePhonebook.%s*************\n"),strTmp));

				if ( 0!=_wfopen_s(&file, strTmp, L"wb") )
				{
					RETAILMSG(1,(_T("\n*************Open file FAILED!!!.%s*************\n"),strTmp));
				}
			}
#endif
			cnt=m_pvContacts->size();
			for(int i=0; i<m_pvContacts->size(); i++)
			{
				m_ppMain->AddContact(&m_pvContacts->at(i));
#if BT_PHONEBOOK_AUTOSTORE
				if(file)
				{
					SaveContactToFile(&m_pvContacts->at(i), file);
				}
#endif
			}

#if BT_PHONEBOOK_AUTOSTORE
			if(file)
			{
				fclose(file);
				file=NULL;
				RETAILMSG(1,(_T("\n*************end SavePhonebook.cnt=%d, take %dms*************\n"),cnt,GetTickCount()-dwStart));
			}
#endif
		}

		//m_ppMain->OnUpdatePBFinish(cnt);

		if(cnt>0)
		{
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
			if(m_ppMain->IsUserDownloadCallLogPermit())
			{
				SetTimer(m_hMainWnd,TIMER_BT_AUTO_DOWNLOAD_CALLLOG,1000,BTTimerProc);
			}
			else
			{
				m_ppMain->OnUpdateCallHistoryFinish(0);
			}
#else
			SetTimer(m_hMainWnd,TIMER_BT_AUTO_DOWNLOAD_CALLLOG,1000,BTTimerProc);
#endif
		}
		else
		{
			if(m_ppMain)
			{
				m_ppMain->OnUpdateCallHistoryFinish(0);
			}
		}
	}
	else
	{
		if(m_ppMain)
		{
			m_ppMain->OnUpdateCallHistoryFinish(0);
		}
	}

	if(m_ppMain)
	{
		m_ppMain->OnUpdatePBFinish(cnt);
	}
}

#if BT_PHONEBOOK_AUTOSTORE
void CPanelBluetoothWrapper::SaveContactToFile(ContactInfo *contact, FILE *fp)
{
	if(contact && fp)
	{
		RETAILMSG(0,(_T("==SaveContactToFile, idx=%d, size=%d, num=%d\n"),contact->Index,sizeof(ContactInfo),contact->PhoneNumber.size()));

		contact->PhoneCount = contact->PhoneNumber.size();
		fwrite(contact,sizeof(ContactInfo),1,fp);
		for (int i=0;i<contact->PhoneNumber.size();i++)
		{
			fwrite(&contact->PhoneNumber.at(i),sizeof(strPhoneNum),1,fp);
		}
	}
}
int CPanelBluetoothWrapper::LoadStoredPhonebook(const wchar_t *strDevAddr)
{
	int cnt = 0;

	FILE					*file = NULL;
	int					fileSize = 0;
	BYTE				*localBuffer;
	BYTE				*cursor;

	DWORD dwStart=GetTickCount();

	WCHAR filename[MAX_PATH];
	swprintf_s(filename,MAX_PATH,_T("%sPhonebook\\"),g_strInstallPath);
	wcscat_s(filename,MAX_PATH,strDevAddr);
	wcscat_s(filename,MAX_PATH,_T(".pb"));

	RETAILMSG(1,(_T("\n*************Begin LoadStoredPhonebook.%s*************\n"),filename));

	if ( 0!=_wfopen_s(&file, filename, L"rb") )
	{
		RETAILMSG(1,(_T("ERROR: phonbook data file not found,%s\n"), filename));
		return  cnt;
	}	

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	localBuffer = new BYTE[fileSize];
	if(!localBuffer)
		return cnt;

	fread(localBuffer, fileSize, 1, file);
	cursor = localBuffer;

	fclose(file);

	//clear first:
	m_listStoredContacts.Head();
	while (!m_listStoredContacts.End())
	{
		delete m_listStoredContacts.Get();
		m_listStoredContacts.Next();
	}
	m_listStoredContacts.Clear();

	m_ppMain->ClearContactList();

	int contact_size = sizeof(ContactInfo);
	int number_size = sizeof(strPhoneNum);

	while (cursor-localBuffer < fileSize-contact_size)
	{
		ContactInfo *pcontact = new ContactInfo;
		memcpy(pcontact,cursor,/*contact_size*/140);
		cursor+=contact_size;
		//RETAILMSG(1,(_T("==LoadContact, idx=%d, num=%d,contact_size=%d\n"),pcontact->Index,pcontact->PhoneCount,contact_size));

		for (int i=0;i<pcontact->PhoneCount;i++)
		{
			strPhoneNum number;
			memcpy(&number,cursor,number_size);
			pcontact->PhoneNumber.push_back(number);
			cursor+=number_size;
		}
		if(m_ppMain)
		{
			m_ppMain->AddContact(pcontact);
		}

		m_listStoredContacts.Add(pcontact,pcontact->Index);
		cnt++;
	}

	if(m_ppMain)
	{
		m_ppMain->SetStoredContactStatus(cnt>0);
		m_ppMain->OnUpdatePBFinish(cnt);
	}

	delete[] localBuffer;

	RETAILMSG(1,(_T("\n*************End LoadStoredPhonebook, total count:%d,  take %d ms*************\n"),cnt,GetTickCount()-dwStart));

	return cnt;
}
#endif

void CPanelBluetoothWrapper::OnNotifyCallLogChanged()
{
	KillTimer(m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_CALLLOG);

	//use permit only take affect on the button to click,
	//if the call history is downloaded and available, just update it
	//ignoring user permit. -by wyp
	//customer boss audio is a exception.
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	if(!m_ppMain->IsUserDownloadCallLogPermit())
	{
		RETAILMSG(1, (TEXT("WARN:[%s]: return\r\n"), TEXT(__FUNCTION__)));
		m_bPhonebookDownloading = FALSE;
		m_ppMain->OnUpdateCallHistoryFinish(0);
		return;
	}
#endif
#endif

	int cnt = 0;
	BOOL bAdjustName=FALSE;
	if (m_strConnectedDevName[0])
	{
		if(wcsstr(m_strConnectedDevName,_T("iPhone")))
		{
			bAdjustName = TRUE;
		}
	}
	m_bPhonebookDownloading = FALSE;
	m_ppMain->ClearHistoryList();
	if(m_pvAllCalls)
	{
		RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyCallLogChanged]: all cnt:%d\r\n"),m_pvAllCalls->size()));

		for(int i=0; i<m_pvAllCalls->size(); i++)
		{
			m_ppMain->AddCallLog(&m_pvAllCalls->at(i),bAdjustName);
			cnt++;
		}
	}
	m_ppMain->OnUpdateCallHistoryFinish(cnt);
}
BOOL CPanelBluetoothWrapper::IsDevicePaired(LPTSTR strDevAddr)
{
	BOOL bRet = FALSE;

	if(m_pvPaired)
	{
		for(int i=0; i<m_pvPaired->size(); i++)
		{
			if(lstrcmpi((m_pvPaired->at(i)).DevAdd,strDevAddr) == 0)
			{
				bRet = TRUE;
				break;
			}
		}
	}
	return bRet;
}
void CPanelBluetoothWrapper::RegisterPairRequestCallback(callbackBluetoothPairReques pairCallback, LPVOID para)
{
	m_pairRequestCallback = pairCallback;
	m_paraPairRequestCallback = para;
}
void CPanelBluetoothWrapper::OnNotifyDeviceListChanged()
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnNotifyDeviceListChanged]\r\n")));

	m_ppMain->ClearDeviceList();

	//reset:
	m_strConnectedDevAddr[0]=0;
	m_strConnectedDevID[0]=0;
	m_strConnectedDevName[0]=0;

	if(m_pvPaired)
	{
		for(int i=0; i<m_pvPaired->size(); i++)
		{
			m_ppMain->AddPairedDevice(&m_pvPaired->at(i));

			//get current connected device bluetooth id:
			if(m_pvPaired->at(i).HFPState == HFP_CONNECT_STATE_CONNECT)
			{
				if(m_pvPaired->at(i).ID[0])
				{
					wcscpy_s(m_strConnectedDevID,8,m_pvPaired->at(i).ID);
				}
				if(m_pvPaired->at(i).DeviceName[0])
				{
					wcscpy_s(m_strConnectedDevName,34,m_pvPaired->at(i).DeviceName);
				}
				if(m_pvPaired->at(i).DevAdd[0])
				{
					wcscpy_s(m_strConnectedDevAddr,20,m_pvPaired->at(i).DevAdd);
					int len=wcslen(m_strConnectedDevAddr);
					for (int i=0;i<len;i++)
					{
						if(m_strConnectedDevAddr[i] == ':')
						{
							m_strConnectedDevAddr[i]='-';
						}
					}
					RETAILMSG(1,(_T("####CPanelBT::OnNotifyDeviceListChanged, get connect device id=%s  \r\n"),m_strConnectedDevAddr));
#if BT_PHONEBOOK_AUTOSTORE
					if(m_bNeedLoadStoredContact)
					{
						if(LoadStoredPhonebook(m_strConnectedDevAddr))
						{
							//if stored phonebook is available, update at background after 10 sec.
							if(!m_bAutoDownloadPhonebookFlag)
							{
								SetTimer(m_hMainWnd,TIMER_BT_UPDATE_PHONEBOOK,10000,BTTimerProc);
							}
						}
						m_bNeedLoadStoredContact = FALSE;
					}
#endif
				}
			}
		}
	}

	if(m_pvInquire)
	{
		for(int i=0; i<m_pvInquire->size(); i++)
		{
			BOOL bInPairedList=FALSE;

			if(m_pvPaired)
			{
				for(int j=0; j<m_pvPaired->size(); j++)
				{
					bInPairedList = (lstrcmpi(m_pvPaired->at(j).DevAdd,m_pvInquire->at(i).DevAdd) == 0)?TRUE:FALSE ;
					if(bInPairedList)
					{
						break;
					}
				}
			}

			if(!bInPairedList)
			{
				m_ppMain->AddInquiredDevice(&m_pvInquire->at(i));
			}
		}
	}
}

void CPanelBluetoothWrapper::BTTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	RETAILMSG(1,(_T("++++BTTimerProc:: hwnd: %d, uMsg: %d,idEvent: %d , dwTime: %d++++\r\n"),hwnd,uMsg,idEvent,dwTime));

	switch (idEvent)
	{
	case TIMER_BT_MONITOR_INI_STATUS:
		{
			RETAILMSG(1,(_T("++++TIMER_BT_MONITOR_INI_STATUS,bisInit=%d \r\n"),g_pBtWrapper->bisInit));
#if 0
			if(!g_pBtWrapper->bisInit && g_pBtWrapper->m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
			{
				RETAILMSG(1,(_T("++++reset bluetooth+++++ \r\n")));
				
				BT_DeInit();
				g_pBtWrapper->bisInit=false;
				SetTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_LOAD_MODULE,5000,BTTimerProc);
			}
#endif
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_MONITOR_INI_STATUS);
		}
		break;
	case TIMER_BT_NOTIFY_POWER_ON:
		{
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_NOTIFY_POWER_ON);
			g_pBtWrapper->OnNotifyPowerStatus(TRUE);
		}
		break;
	case TIMER_BT_NOTIFY_POWER_OFF:
		{
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_NOTIFY_POWER_OFF);
			g_pBtWrapper->OnNotifyPowerStatus(FALSE);
		}
		break;
	case TIMER_BT_AUTO_GET_MODULE_INFO:
		{
			if(!g_pBtWrapper->m_bGetLocalAdress)
			{
				RETAILMSG(1,(_T("++++BT_GetLocalAdress ++++\r\n")));
				g_pBtWrapper->m_bGetLocalAdress = TRUE;
				BT_GetLocalAdress();
			}
			else if(!g_pBtWrapper->m_bSetModuleName)
			{
				g_pBtWrapper->m_bSetModuleName = TRUE;
#if (CUSTOMER_ID == CUSTOMER_CVTE_CERTIFICATION)
				BT_SetModuleName(_T("CVTE HeadUnit"));
#else
				//reset name:
				CReg reg;
				WCHAR name_reg[MAX_PATH];
				reg.Open(HKEY_LOCAL_MACHINE,_T("Ident\\"));
				reg.ValueSZ(_T("Name"),name_reg,MAX_PATH);
				BT_SetModuleName(name_reg);
				RETAILMSG(1,(_T("++++BT_SetModuleName %s++++\r\n"),name_reg));
#endif
			}
			else if(!g_pBtWrapper->m_bGetModuleInfo)
			{
				RETAILMSG(1,(_T("++++BT_GetModuleInfo ++++\r\n")));
				g_pBtWrapper->m_bGetModuleInfo = TRUE;
				BT_GetModuleInfo();
			}
			else if(!g_pBtWrapper->m_bGetIniSettingValue)
			{
				RETAILMSG(1,(_T("++++BT_GetIniSettingValue ++++\r\n")));
				g_pBtWrapper->m_bGetIniSettingValue = TRUE;
				BT_GetIniSettingValue();
			}
			else
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_GET_MODULE_INFO);
			}
		}
		break;
	case TIMER_BT_AUTO_LOAD_MODULE:
		if(g_pBtWrapper)
		{
			g_pBtWrapper->IniBTModule();
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_LOAD_MODULE);
		}
		break;
	case TIMER_BT_AUTO_POWER_ON:
		if(g_pBtWrapper)
		{
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_POWER_ON);
			if(!g_pBtWrapper->m_bCarPlayConnected && g_pBtWrapper->m_bBTPowerOnTarget)
				g_pBtWrapper->SetBTPower(TRUE);
		}
		break;
	case TIMER_BT_AUTO_POWER_OFF:
		if(g_pBtWrapper)
		{
#if 0
			if(g_pBtWrapper->ConnectStatus==HF_STATUS_DISCONNECTED && !g_pBtWrapper->m_bBTPowerOn)
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_POWER_OFF);
			}

			if(g_pBtWrapper->ConnectStatus==HF_STATUS_CONNECTED && !g_pBtWrapper->m_bBTPowerOn)
			{
				//g_pBtWrapper->SetBTPower(TRUE);
				g_pBtWrapper->SetBTPower(FALSE);
			}
			else
			{
				g_pBtWrapper->SetBTPower(FALSE);
			}
#endif

			if(g_pBtWrapper->m_bBTPowerOnTarget == FALSE)
			{
				RETAILMSG(1,(_T("++++TIMER_BT_AUTO_POWER_OFF , bInitializing=%d++++\r\n"),g_pBtWrapper->m_bInitializing));
				if(g_pBtWrapper->m_bInitializing ? FALSE : (g_pBtWrapper->bisInit ? GetTickCount()-g_pBtWrapper->m_dwInitializeTick > 5000 : TRUE))
				{
					KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_POWER_OFF);
					//if(g_pBtWrapper->bisInit)
						g_pBtWrapper->SetBTPower(FALSE);
				}
			}
			else
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_POWER_OFF);
			}
		}
		break;
	case TIMER_BT_MONITOR_A2DP_STATUS:
		if(g_pBtWrapper)
		{
			g_pBtWrapper->OnMonitorA2dpStatus();
		}
		break;
	case TIMER_MEDIA_DELAY_OPERATION:
		{
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_MEDIA_DELAY_OPERATION);

			if(g_pBtWrapper->m_bPreparePause)
			{
				RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::BTTimerProc]: delay pause\r\n")));

				g_pBtWrapper->m_bPreparePause = FALSE;
				g_pBtWrapper->Pause();
			}
			else if(g_pBtWrapper->m_bPreparePlay)
			{
				RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::BTTimerProc]: delay play\r\n")));

				//DuckAudio(TRUE,0,FALSE);
				g_pBtWrapper->m_bPreparePlay = FALSE;
				g_pBtWrapper->Play();
				MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,1100);
				//DuckAudio(FALSE,0,TRUE);
			}
		}
		break;
	case TIMER_BT_UPDATE_PHONEBOOK:
		{
			RETAILMSG(1,(_T("==TIMER_BT_UPDATE_PHONEBOOK Downloading=%d, AutoDownloadFlag=%d \r\n"),g_pBtWrapper->m_bPhonebookDownloading,g_pBtWrapper->m_bAutoDownloadPhonebookFlag));
			if(g_pBtWrapper->m_bAutoDownloadPhonebookFlag)
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_UPDATE_PHONEBOOK);
			}
			else if( !g_pBtWrapper->m_bPhonebookDownloading )
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_UPDATE_PHONEBOOK);

				if(	!g_pBtWrapper->m_bCarPlayConnected && 
					g_pBtWrapper->m_bHfpConnected )
				{
					RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::BTTimerProc]: BT_DownloadPB \r\n")));
					g_pBtWrapper->m_bRefrehPhonebookFlag = TRUE;
					BT_DownloadPB(DOWNLOAD_PB_ACTION_YES);
					g_pBtWrapper->OnNotifyPBSyncState(PB_SYNC_STATE_START);
					SetTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_PB,5000,BTTimerProc);
				}
			}
		}
		break;
	case TIMER_BT_AUTO_DOWNLOAD_CALLLOG:
		{
			RETAILMSG(1,(_T("==TIMER_BT_AUTO_DOWNLOAD_CALLLOG Downloading=%d, AutoDownloadFlag=%d \r\n"),g_pBtWrapper->m_bPhonebookDownloading,g_pBtWrapper->m_bAutoDownloadPhonebookFlag));
			if(g_pBtWrapper->m_bAutoDownloadPhonebookFlag)
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_DOWNLOAD_CALLLOG);
				g_pBtWrapper->m_ppMain->OnUpdateCallHistoryFinish(0);
			}
			else if( !g_pBtWrapper->m_bPhonebookDownloading )
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_DOWNLOAD_CALLLOG);

				if(	!g_pBtWrapper->m_bCarPlayConnected && 
					g_pBtWrapper->m_bHfpConnected )
				{
					RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::BTTimerProc]: BT_GetCallLog \r\n")));
					BT_GetCallLog(CALLLOG_TYPE_ALL);
					SetTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_CALLLOG,5000,BTTimerProc);
				}
				else
				{
					g_pBtWrapper->m_ppMain->OnUpdateCallHistoryFinish(0);
				}
			}
		}
		break;
	case TIMER_BT_AUTO_CANCLE_DOWNLOAD_CALLLOG:
		{
			RETAILMSG(1,(_T("==TIMER_BT_AUTO_CANCLE_DOWNLOAD_CALLLOG Downloading=%d \r\n"),g_pBtWrapper->m_bPhonebookDownloading));
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_CALLLOG);
			if(!g_pBtWrapper->m_bPhonebookDownloading)
			{
				g_pBtWrapper->m_ppMain->OnUpdateCallHistoryFinish(0);
			}
		}
		break;
	case TIMER_BT_AUTO_CANCLE_DOWNLOAD_PB:
		{
			RETAILMSG(1,(_T("==TIMER_BT_AUTO_CANCLE_DOWNLOAD_PB Downloading=%d \r\n"),g_pBtWrapper->m_bPhonebookDownloading));
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_CANCLE_DOWNLOAD_PB);
			if(!g_pBtWrapper->m_bPhonebookDownloading)
			{
				g_pBtWrapper->m_ppMain->OnUpdatePBFinish(0);
			}
		}
		break;
	case TIMER_BT_MONITOR_A2DP_CONNECTION:
		{
			RETAILMSG(1,(_T("++++CPanelBluetoothWrapper:: TIMER_BT_MONITOR_A2DP_CONNECTION = %d, %s ++++\r\n"),g_pBtWrapper->m_bA2dpConnected,g_pBtWrapper->m_strCurDeviceID));

			if(g_pBtWrapper->m_bBTPowerOn
				&& !g_pBtWrapper->m_bCarPlayConnected
				&& !g_pBtWrapper->m_bA2dpConnected 
				&& g_pBtWrapper->m_dwA2dpAutoConnectCnt > 0 
				&& g_pBtWrapper->m_bHfpConnected)
			{
				g_pBtWrapper->m_dwA2dpAutoConnectCnt--;
				BT_A2DPConnect(g_pBtWrapper->m_strCurDeviceID,DEVICE_CONNECT_ACTION_CONNECT);
			}
			else
			{
				KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_MONITOR_A2DP_CONNECTION);
			}
		}
		break;
	case TIMER_BT_AUTO_CHANGE_AUDIO_DIR:
		{
#if 0
			if(g_pBtWrapper->m_dwCallStatus == HF_STATE_TALKING && g_pBtWrapper->AudioStatus != HF_AUDIO_CONNECTED)
			{
				RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:TIMER_BT_AUTO_CHANGE_AUDIO_DIR: AUDIO_TRANSFER_MODE_HANDFREE\r\n")));
				BT_AudioTransfer(AUDIO_TRANSFER_MODE_HANDFREE);
			}
			else
			{
				RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:TIMER_BT_AUTO_CHANGE_AUDIO_DIR: skip, m_dwCallStatus=%d, AudioStatus=%d\r\n"),g_pBtWrapper->m_dwCallStatus,g_pBtWrapper->AudioStatus));
			}
#endif
			KillTimer(g_pBtWrapper->m_hMainWnd,TIMER_BT_AUTO_CHANGE_AUDIO_DIR);
		}
		break;
	default:
		break;
	}
}
DWORD CPanelBluetoothWrapper::AutoChangeAudioDirThreadPro(LPVOID lpParam)
{
	CPanelBluetoothWrapper *panel = (CPanelBluetoothWrapper*)lpParam;
	if(panel)
	{
		Sleep(500);
		if(panel->m_dwCallStatus == HF_STATE_TALKING && panel->AudioStatus != HF_AUDIO_CONNECTED)
		{
			RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:AutoChangeAudioDirThreadPro: AUDIO_TRANSFER_MODE_HANDFREE\r\n")));
			BT_AudioTransfer(AUDIO_TRANSFER_MODE_HANDFREE);
			RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:AutoChangeAudioDirThreadPro: AUDIO_TRANSFER_MODE_HANDFREE OK\r\n")));
		}
		else
		{
			RETAILMSG(1/*DEBUG_BT*/,(_T("MSG:AutoChangeAudioDirThreadPro: skip, m_dwCallStatus=%d, AudioStatus=%d\r\n"),panel->m_dwCallStatus,panel->AudioStatus));
		}
	}
	return 0;
}
void CPanelBluetoothWrapper::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnCarPlayMsg]: %d, %d\r\n"),wParam,lParam));

	switch (wParam)
	{
	case CARPLAY_DISABLE_BLUETOOTH:
		{
			if(g_pBtWrapper)
			{
				if(g_pBtWrapper->m_bInitializing ? FALSE : (g_pBtWrapper->bisInit ? GetTickCount()-g_pBtWrapper->m_dwInitializeTick > 5000 : TRUE))
				{
					RETAILMSG(1,(_T("++++carplay connect, disable bluetooth++++\r\n")));
					g_pBtWrapper->SetBTPower(FALSE);
				}
				else
				{
					RETAILMSG(1,(_T("++++carplay connect, will disable bluetooth after 1000ms ++++\r\n")));
					//use a timer to power off:
					m_bBTPowerOnTarget = FALSE;
					SetTimer(m_hMainWnd,TIMER_BT_AUTO_POWER_OFF,1000,BTTimerProc);
				}

				//quit current page:
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_BT)
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
				}
			}
		}
		break;
	case CARPLAY_CONNECT_STATUS:
		{
			if(lParam==1 || lParam==2)//connected or connecting...
			{
				m_bCarPlayConnected = TRUE;

				//do not turn off bluetooth here, 
				//turn off while CARPLAY_DISABLE_BLUETOOTH received.
				/*
				if(g_pBtWrapper)
				{
					//SetBTPower(FALSE);
					//SetTimer(m_hMainWnd,TIMER_BT_AUTO_POWER_OFF,5000,BTTimerProc);

					if(g_pBtWrapper->m_bInitializing ? FALSE : (g_pBtWrapper->bisInit ? GetTickCount()-g_pBtWrapper->m_dwInitializeTick > 5000 : TRUE))
					{
						RETAILMSG(1,(_T("++++carplay connect, disable bluetooth++++\r\n")));

						//if(g_pBtWrapper->bisInit)
							g_pBtWrapper->SetBTPower(FALSE);
					}
					else
					{
						RETAILMSG(1,(_T("++++carplay connect, will disable bluetooth after 1000ms ++++\r\n")));
						//use a timer to power off:
						m_bBTPowerOnTarget = FALSE;
						SetTimer(m_hMainWnd,TIMER_BT_AUTO_POWER_OFF,1000,BTTimerProc);
					}

					//quit current page:
					if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_BT)
					{
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
					}
				}
				*/
			}
			else if(lParam==0)//disconnected
			{
				m_bCarPlayConnected = FALSE;

				if(g_pBtWrapper)
				{
					if(m_bBTPowerOnBeforeCarplay && m_idSystemPowerState==SYS_POWER_ON)
					{
						//SetBTPower(TRUE);
						m_bBTPowerOnTarget = TRUE;
						SetTimer(m_hMainWnd,TIMER_BT_AUTO_POWER_ON,5000,BTTimerProc);
					}
				}
			}
 
 			if(m_ppTalking)
 			{
 				m_ppTalking->OnCarPlayConnected(m_bCarPlayConnected);
 			}

			if(m_bCarPlayConnected && m_pSkinManager)
			{
				/*
				//quit current page:
				if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_BT)
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
				}
				*/

				//update widget:
				BOOL benable=FALSE;
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_PHONE,0,UIOPRT_SHOWHIDE,&benable);
				m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_A2DP,0,UIOPRT_SHOWHIDE,&benable);
			}
		}
		break;
	default:
		break;
	}
}
BOOL CPanelBluetoothWrapper::SetA2dpVolume(DWORD dwVol,BOOL bFade)
{
	return TRUE;
}

void CPanelBluetoothWrapper::OnVolumeFadeReday(BOOL bMute)
{

}
void CPanelBluetoothWrapper::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	switch (idState)
	{
	case SYS_POWER_OFF:
	case SYS_POWER_SOFT_RESET:
	case SYS_POWER_IDLE:
		{
			//RETAILMSG(DEBUG_BT,(_T("MSG:[CPanelBluetoothWrapper::OnSystemPowerStatePrepare]: Pause\r\n")));
			Pause();
			m_bA2dpVolumeOn = FALSE;
			m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn = m_bBTManualPowerOn ;

			//disconnect device if any:
			if(m_strConnectedDevID[0] && (idState==SYS_POWER_OFF || idState==SYS_POWER_SOFT_RESET))
			{
				RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::OnSystemPowerStatePrepare]: %d, disconnect device\r\n"),idState));
				BT_ConnectPairedDevice(m_strConnectedDevID, DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
			}
		}
		break;
	}
}
void CPanelBluetoothWrapper::OnPairRequestFromAndroidAuto(LPTSTR strDevAddr)
{
	if(strDevAddr)
	{
		wcscpy_s(m_strAndroidAutoDevAddr,20,strDevAddr);
		int len=wcslen(m_strAndroidAutoDevAddr);
		for (int i=0;i<len;i++)
		{
			if(m_strAndroidAutoDevAddr[i] == ':')
			{
				m_strAndroidAutoDevAddr[i]='-';
			}
		}

		if( m_strConnectedDevID[0] && wcscmp(m_strAndroidAutoDevAddr,m_strConnectedDevAddr)!=0)
		{
			RETAILMSG(1,(_T("MSG:[OnPairRequestFromAndroidAuto]:  disconnect current device, %s\r\n"),m_strConnectedDevAddr));
			BT_ConnectPairedDevice(m_strConnectedDevID, DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
			m_strConnectedDevID[0] = 0;
		}
	}	
}
void CPanelBluetoothWrapper::OnAndriodAutoConnectStatus(BOOL bConnect)
{
	m_bAndroidAutoConnect = bConnect;
	if(bConnect)
	{
		if(m_bA2dpConnected)
		{
			OnNotifyA2dpConnected(FALSE);
			m_bA2dpConnected = TRUE;
		}
	}
	else
	{
		/*
		if( m_strConnectedDevID[0] && wcscmp(m_strAndroidAutoDevAddr,m_strConnectedDevAddr)==0)
		{
			RETAILMSG(1,(_T("MSG:[OnAndriodAutoConnectStatus]:  disconnect current device, %s\r\n"),m_strConnectedDevAddr));
			BT_ConnectPairedDevice(m_strConnectedDevID, DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
			m_strConnectedDevID[0] = 0;
		}
		*/
	}
}
void CPanelBluetoothWrapper::CoCallBack(UINT eventID, UINT _DataID, WCHAR* _data, UINT _dataLen)
{
	//RETAILMSG(1,(_T("__CoCallBack eventID = 0x%x _DataID:0x%X _data:0x%X _dataLen:%d \r\n"), eventID,_DataID,_data,_dataLen));

	if(!g_pBtWrapper)
	{
		return;
	}

	//copy message:
	BT_MSG *msg=new BT_MSG;
	if(msg)
	{
		memset(msg,0,sizeof(BT_MSG));
		msg->eventID=eventID;
		msg->_DataID=_DataID;
		msg->_dataLen=_dataLen;
		msg->dataCopy = FALSE;

		switch (eventID)
		{
		case FLC_BT_EVENT_LOCALADRESS:
		case FLC_BT_EVENT_INQUIRY_DEVICE_ADDRESS:
		case FLC_BT_EVENT_HFPCONNECT_STATE:
		case FLC_BT_EVENT_MODULE_VERSION:
		case FLC_BT_EVENT_MODULE_NAME:
		case FLC_BT_EVENT_PIN_CODE:
		case FLC_BT_EVENT_DELETE_PAIRED:
			{
				//_dataLen is calculated on WCHAR count
				msg->dataCopy = TRUE;
				msg->_data = new WCHAR[_dataLen+1];
				memcpy(msg->_data,_data,_dataLen*2);
				msg->_data[_dataLen]=0;
			}
			break;
		case FLC_BT_EVENT_PAIR_REQUEST:
		case FLC_BT_EVENT_CALL_STATE:
		case FLC_BT_EVENT_MUSIC_ID3:
		case FLC_BT_EVENT_SETUP_INI:
		case FLC_BT_EVENT_MUSIC_LENGTH_STATUS:
			{
				RETAILMSG(DEBUG_BT,(_T("__CoCallBack eventID = 0x%x Copy data\r\n"),eventID));
				msg->dataCopy = TRUE;
				msg->_data = (WCHAR*)(new BYTE[_dataLen]);
				memcpy(msg->_data,_data,_dataLen);
			}
			break;
		default:
			{
				//directly get the pointer:
				msg->_data = _data;
			}
			break;
		}

		//
		// Debug Print 
		//
		/*
		switch(eventID)
		{
		case FLC_BT_EVENT_LOCALADRESS:
			RETAILMSG(1,(_T("__CoCallBack FLC_BT_EVENT_LOCALADRESS = %s\r\n"),msg->_data));
			break;
		case FLC_BT_EVENT_HFPCONNECT_STATE:
			RETAILMSG(1,(_T("__CoCallBack FLC_BT_EVENT_HFPCONNECT_STATE = %d\r\n"),msg->_DataID));
			break;
		case FLC_BT_EVENT_A2DPCONNECT_STATE:
			RETAILMSG(1,(_T("__CoCallBack FLC_BT_EVENT_A2DPCONNECT_STATE = %d\r\n"),msg->_DataID));
			break;
		case FLC_BT_EVENT_CALL_STATE:
			strCallStatus *pStrCallStatus = (strCallStatus*)_data;
			RETAILMSG(1,(_T("__CoCallBack FLC_BT_EVENT_CALL_STATE iCallIndex:%d iCallState:%d\r\n"),pStrCallStatus->iCallIndex, pStrCallStatus->iCallState));
			break;
		}
		*/
		
		//notify:
		::PostMessage(g_pBtWrapper->m_hMainWnd,WM_BLUETOOTH_MESSAGE,0,(LPARAM)msg);

	}

	/*
	switch (eventID)
	{
	case FLC_BT_EVENT_INIT_INDICATOR:
	case FLC_BT_EVENT_MODULE_VERSION:
	case FLC_BT_EVENT_MODULE_NAME:
	case FLC_BT_EVENT_LOCALADRESS:
		::SendMessage(g_pBtWrapper->m_hMainWnd,WM_BLUETOOTH_MESSAGE,0,(LPARAM)msg);
		break;
	default:
		::PostMessage(g_pBtWrapper->m_hMainWnd,WM_BLUETOOTH_MESSAGE,0,(LPARAM)msg);
		break;
	}
	*/
}
void CPanelBluetoothWrapper::OnBluetoothMsg(WPARAM wParam, LPARAM lParam)
{
	BT_MSG *msg = ((BT_MSG*)lParam);
	if(!msg)
	{
		return;
	}

	UINT eventID=msg->eventID;
	UINT _DataID=msg->_DataID;
	WCHAR* _data=msg->_data;
	UINT _dataLen=msg->_dataLen;

	RETAILMSG(1,(_T("MSG:[CPanelBluetoothWrapper::OnBluetoothMsg] eventID:0x%x, _DataID:%d, _dataLen:%d \r\n"), eventID, _DataID, _dataLen));

	switch (eventID)
	{
	case FLC_BT_EVENT_WARNING:
		break;
	case FLC_BT_EVENT_LOCALADRESS:
		{
			RETAILMSG(1/*DEBUG_BT*/,(_T("###BT local Adress: %s\r\n"),(LPCTSTR)_data));
			//save mac address to registry:
			CReg reg;
			reg.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Bluetooth\\hci\\"));
			LPCTSTR mac=reg.ValueSZ(_T("mac"));
			if((mac && wcscmp(mac,(LPCTSTR)_data) != 0) || !mac)
			{
				RETAILMSG(1,(_T("==save bt mac: %s \r\n"),(LPCTSTR)_data));
				reg.SetSZ(_T("mac"),(LPCTSTR)_data);
			}
#if CVTE_EN_CARPLAY
			//if no mac address added to caplay, add it.
			if(CCarplayAdapter::GetInstance()->GetBluetoothIDCount()==0)
			{
				CCarplayAdapter::GetInstance()->AddBluetoothID((LPCTSTR)_data);
			}
#endif
		}
		break;
	case FLC_BT_EVENT_INQUIRY_DEVICE_ADDRESS:
		{
			OnNotifyDeviceListChanged();
		}
		break;
	case FLC_BT_EVENT_INIT_INDICATOR:
		if(_DataID == INIT_STATE_OK)
		{
			OnIniStatus(TRUE);
		}
		else
		{
			OnIniStatus(FALSE);
		}
		break;

	case FLC_BT_EVENT_DEINIT_INDICATOR:
		if(_DataID == DEINIT_STATE_OK)
		{
			OnDeiniStatus(TRUE);
		}
		else
		{
			OnDeiniStatus(FALSE);
		}
		break;

	case FLC_BT_EVENT_STANDBY_ON:
		{
			OnNotifyPowerStatus(FALSE);
		}
		break;
	case FLC_BT_EVENT_STANDBY_OFF:
		{
			OnNotifyPowerStatus(TRUE);
		}
		break;
	case FLC_BT_EVENT_SIGNAL_LEVEL:
		{
			OnSignalChange(_DataID);
		}
		break;

	case FLC_BT_EVENT_BATTERY_LEVEL:
		{
			OnBatteryChange(_DataID);
		}
		break;

	case FLC_BT_EVENT_HFPCONNECT_STATE:
		OnNotifyHfpConnected(HFP_CONNECT_STATE_CONNECT == (HFP_CONNECT_STATE)_DataID);
		RETAILMSG(1/*DEBUG_BT*/,(_T("###HFP_CONNECT_STATE_CONNECT = %d\r\n"),_DataID));
		if(_data)
		{
			wcscpy_s(m_strCurDeviceID,8,(LPCTSTR)_data);
		}
		break;
	case FLC_BT_EVENT_A2DPCONNECT_STATE:
		RETAILMSG(1/*DEBUG_BT*/,(_T("###FLC_BT_EVENT_A2DPCONNECT_STATE = %d\r\n"),_DataID));
		OnNotifyA2dpConnected(A2DP_CONNECT_STATE_CONNECT == _DataID);
		break;

	case FLC_BT_EVENT_CONNECT_STATE:
		break;

	case FLC_BT_EVENT_DUNSERVICE_SUPPORT:
		OnNotifyDunServiceSupport(DUN_SERVICE_SUPPORT_YES == _DataID);
		break;

	case FLC_BT_EVENT_PANSERVICE_SUPPORT:
		OnNotifyPanServiceSupport(PAN_SERVICE_SUPPORT_YES == _DataID);
		break;

	case FLC_BT_EVENT_INQUIRY_DEVICE:
		{
			RETAILMSG(1/*DEBUG_BT*/,(_T("###BT FLC_BT_EVENT_INQUIRY_DEVICE Finish.\r\n")));

			m_pvInquire = (std::vector<strDeviceInfo>*)_data;
			OnNotifyDeviceInquiryFinish();
			OnNotifyDeviceListChanged();
		}
		break;

	case FLC_BT_EVENT_PAIR_REQUEST:
		{
			strRequestDeviceInfo *pinfo = (strRequestDeviceInfo*)_data;
			memcpy(&m_strRequestDeviceInfo,pinfo,sizeof(strRequestDeviceInfo));

#if CVTE_EN_BT_PAIRED_CODE_PERMIT
			if(m_bAndroidAutoConnect)
			{
				//accept pair request directly
				BT_PairDevice(m_strRequestDeviceInfo.ID, PAIR_ACTION_ACCEPT_REMOTE);
			}
			else
			{
				//show pop massage to user to confirm pairing
				ShowNotificationBox(m_hMainWnd,IDC_BT_NOTIFYBOX_PIN_REQUEST,
					&CPanelBluetoothWrapper::OnNotify,
					this,
					0,
					m_strRequestDeviceInfo.DeviceName,
					m_strRequestDeviceInfo.PassKey,
					0);
			}
#else
			//accept pair request directly
			BT_PairDevice(m_strRequestDeviceInfo.ID, PAIR_ACTION_ACCEPT_REMOTE);
#endif

			if(m_pairRequestCallback)
			{
				m_pairRequestCallback(m_strRequestDeviceInfo.DevAdd,m_strRequestDeviceInfo.PassKey,m_paraPairRequestCallback);
			}
		}
		break;

	case FLC_BT_EVENT_PAIR_RESULT:
		OnNotifyPairResult((PAIR_RESULT)_DataID);
		break;

	case FLC_BT_EVENT_PAIRD_DEVICE_INFO:
		{
			m_pvPaired = (std::vector<strDeviceInfo>*)_data;
			OnNotifyDeviceListChanged();
			if(m_pairRequestCallback)
			{
				m_pairRequestCallback(NULL,NULL,m_paraPairRequestCallback);
			}
		}
		break;

	case FLC_BT_EVENT_DELETE_PAIRED:
		OnNotifyDeviceDelete((LPCTSTR)_data);
		break;

	case FLC_BT_EVENT_CALL_COUNT:
		{
			OnNotifyCallCount(_DataID);
		}
		break;

	case FLC_BT_EVENT_CALL_STATE:
		{
			strCallStatus* ptCallStatus = (strCallStatus*)_data;
			OnNotifyCallStatus(ptCallStatus);
		}
		break;

	case FLC_BT_EVENT_AUDIOTRANSFER_STATE:
		OnNotifyAudioTransferMode((AUDIO_TRANSFER_MODE)_DataID);
		break;

	case FLC_BT_EVENT_MIC_MUTE_STATE:
		OnNotifyMicState((MICPHONE_MUTE_STATE)_DataID);
		break;

	case FLC_BT_EVENT_PB_SYNC_INDICATOR:
		OnNotifyPBSyncState((PB_SYNC_STATE)_DataID);
		break;

	case FLC_BT_EVENT_PB_ENTRIES:
		m_pvContacts = (std::vector<ContactInfo>*)_data;
		OnNotifyPhoneBookChanged();
		break;

	case FLC_BT_EVENT_LETTER_POS:
		break;

	case FLC_BT_EVENT_CALLLOG_ENTRIES:
		{
			switch (_DataID)
			{
			case CALLLOG_TYPE_ALL:
				m_pvAllCalls = (std::vector<strCallLog>*)_data;
				OnNotifyCallLogChanged();
				break;
			case CALLLOG_TYPE_LASTDIAL:
				m_pvDialCalls = (std::vector<strCallLog>*)_data;
				break;
			case CALLLOG_TYPE_MISSEDCALL:
				m_pvMissCalls = (std::vector<strCallLog>*)_data;
				break;
			case CALLLOG_TYPE_RECEIVEDCALL:
				m_pvReceivCalls = (std::vector<strCallLog>*)_data;
				break;
			}

			//OnNotifyCallLogChanged();
		}
		break;

	case FLC_BT_EVENT_AUDIO_SOURCE_INDICATOR:
		OnNotifyAudioSourceState((AUDIO_SOURCE)_DataID,(AUDIO_STATE)_dataLen);
		break;

	case FLC_BT_EVENT_A2DP_PLAYSTATE:
		{
			OnNotifyA2dpPlayState((PLAYER_STATE)_DataID);
		}
		break;

	case FLC_BT_EVENT_MUSIC_ID3:
		{
			strMusicID3* tMusicID3 = (strMusicID3*)_data;
			OnNotifyID3(tMusicID3);
		}
		break;
	case FLC_BT_EVENT_MUSIC_LENGTH_STATUS:
		{
			OnNotifyAvPlayTime((strAvPlayStatus*)_data);
		}
		break;

	case FLC_BT_EVENT_MODULE_VERSION:
		OnNotifyModuleVersion((LPCTSTR)_data);
		break;

	case FLC_BT_EVENT_MODULE_NAME:
		OnNotifyModuleName((LPCTSTR)_data);
		break;

	case FLC_BT_EVENT_SETUP_INI:
		{
			strSetupInI* ptIni = (strSetupInI*)_data;
			OnNotifySetupInI(ptIni);
		}
		break;

	case FLC_BT_EVENT_PIN_CODE:
		RETAILMSG(DEBUG_BT,(_T("###FLC_BT_EVENT_PIN_CODE,_data = %s\r\n"),(LPCTSTR)_data));
		break;

	case FLC_BT_EVENT_MIC_MODE:
		break;

	case FLC_BT_EVENT_MIC_GAIN:
		break;

	case FLC_BT_EVENT_AUTO_CONNECT:
		if(_DataID == AUTO_CONNECT_MODE_ENABLED)
		{
		}
		else
		{
		}
		break;

	case FLC_BT_EVENT_AUTO_ANSWER:
		if(_DataID == AUTO_ANSWER_MODE_ENABLED)
		{
		}
		else
		{
		}
		break;

	case FLC_BT_EVENT_AUTO_SYNCPB:
		if(_DataID == AUTO_SYNC_PB_MODE_ENABLED)
		{
		}
		else
		{
		}
		break;

	case FLC_BT_EVENT_SPEAKER_GAIN:
		break;

	default:
		break;
	}

	//clear message:
	if(msg->dataCopy && msg->_data)
	{
		delete[] msg->_data;
		msg->_data = NULL;
	}
	delete msg;
	msg = NULL;

}

