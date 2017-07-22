#include "PanelWebLink.h"
#include "../../resource.h"
#include <dbt.h>
#include "../../Weblink/IPCApp.h"
#include <mmsystem.h>

extern HWND					g_hMainWnd;
extern const wchar_t* g_strInstallPath;

CPanelWebLink::CPanelWebLink(void)
{
	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::CPanelWebLink] \r\n")));

	m_bWeblinkVisible = FALSE;
	m_bSpeechOn = FALSE;
	m_bExitWeblink = FALSE;
	m_bRunApp = FALSE;
	m_bConnect = FALSE;
	m_bWindowEnable = FALSE;

	m_dwEnterTick = 0;

	m_bAutoResumeTimer = FALSE;
	m_bAutoResumeFlag = FALSE;
	m_AutoResumeTimer = 0;

	m_bA2dpConnected = FALSE;
	m_bPlayingFlag = FALSE;
}

CPanelWebLink::~CPanelWebLink(void)
{
	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::~CPanelWebLink] \r\n")));
}
BOOL CPanelWebLink::IsReady()
{
//		if(m_bWeblinkVisible)
//		{
//			return TRUE;
//		}
//		else
//		{
//		    return FALSE;
//		}
	return CPanelGL::IsReady();
}
void CPanelWebLink::OnSRCChanged(UINT idSRC)
{

}

void CPanelWebLink::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
//		RETAILMSG(DEBUG_WEBLINK,(_T("CPanelWebLink::OnQuitPanel iParam[%d] idAni[%d] m_bWeblinkVisible[%d]\r\n"),
//			iParam,
//			idAni,
//			m_bWeblinkVisible));

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	m_bExitWeblink = TRUE;
	if(m_bAutoResumeTimer)
	{
		RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnQuitPanel] KillTimer TIMER_AUTO_RESUME\r\n")));
		m_bAutoResumeTimer = FALSE;
		if ( m_AutoResumeTimer != 0 )
		{
			timeKillEvent(m_AutoResumeTimer);
			m_AutoResumeTimer = 0;
		}
	}

	//if(m_bWeblinkVisible)
	{
		//RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnQuitPanel] hidewindow\r\n")));
		WeblinkShowWindow(FALSE);
	}
}
void CPanelWebLink::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
 	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
//		RETAILMSG(DEBUG_WEBLINK,(_T("CPanelWebLink::OnEnterPanel iParam[%d] idAni[%d]\r\n"),iParam,idAni));

	//make animation:
	if(idAni != ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}

	m_bExitWeblink = FALSE;

	if(GetAudioSRC() != SRC_AUDIO_ARM)
	{
		RETAILMSG(1,(_T("MSG:[%s]: SwitchMediaClassSRC & SwitchAudioSRC \r\n"),TEXT(__FUNCTION__)));
		SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);

		BOOL bA2dpOn=FALSE;
		SendBluetoothCommand(BT_CMD_GET_A2DP_STREAM,LPARAM(&bA2dpOn));

		if(bA2dpOn)
		{
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_ENABLE);
			SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
		}
	}
	//Mark 20160628
	//切换到UI_CLASS_WEBLINK会使得BT与Easyconnect互切时，蓝牙音乐卡顿一下
	SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP));
	//SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_EASYCONNECTED_WRAPPER));

		//SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);

#if !USE_WAV_ONCHIP
	//reset volume:
	if(m_bSpeechOn)
	{
		DWORD dwGPS=m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS;
		DWORD dwMedia=m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia;

		DWORD dwVol=0;

		if(dwGPS>dwMedia)
			dwVol = (dwGPS>0?A2DP_LOW_VOL*dwMedia/dwGPS:A2DP_LOW_VOL);
		else
			dwVol = (dwMedia>0?A2DP_LOW_VOL*dwGPS/dwMedia:A2DP_LOW_VOL);

		RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[%s]: dwVol:%d ......................\r\n"),TEXT(__FUNCTION__),dwVol));

		//pull down volume
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+dwVol);
	}
	else
	{
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
	}
#endif

	//can bus:
//	::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_OTHER,0x30);
// 	::PostMessage(m_hMainWnd,WM_NOTIFY_MEDIA_PLAYBACK_INFO,0x0,(0x0<<16)+0x30);

//		//notify src:
//		::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_WEBLINK,0x40);

	SetMsg(GetResourceString(IDS_PHONELINK_MAIN_PROMPT));

	//WeblinkShowWindow(TRUE);
	m_bWeblinkVisible = FALSE;

	m_bAutoResumeFlag = TRUE;
	if(m_AutoResumeTimer)
	{
		m_bAutoResumeTimer = TRUE;
		m_AutoResumeTimer = timeSetEvent( 500, 1, AutoResumeTimerProc, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
	}

	m_dwEnterTick = GetTickCount();
}

void CPanelWebLink::Render()
{
	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;

// 	COLORGL cr_b={0.0,0.0,0.0,m_fCurAlpha};

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

	if(IsAniReady() && GetTickCount()-m_dwEnterTick>200)
	{
		if(!m_bRunApp)
		{
			//launch app:
			RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::Render] WeblinkRunApp\r\n")));
			if(WeblinkRunApp(TRUE))
				m_bRunApp = TRUE;
		}

		else if(!m_bWeblinkVisible && m_bWindowEnable)
		{
			WeblinkShowWindow(TRUE);
		}

	}

	//auto resume:
	EndRender();

	//RETAILMSG(DEBUG_WEBLINK,(_T("CPanelWebLink::Render -\r\n")));

}
void CPanelWebLink::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);
	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnGestureMsg] idGesture[%d]\r\n"),idGesture));

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
BOOL CPanelWebLink:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	RETAILMSG(1, (TEXT("MSG:[CPanelWebLink::Initialize]: ++ \r\n")));
	m_bShowStatusBar = FALSE;

	if (!CIPCApp::GetInstance().Init(this))
	{
		RETAILMSG(1, (TEXT("MSG:[CPanelWebLink::Initialize]: CIPCApp Init failed!\r\n")));
		return FALSE;
	}

	if(!m_bRunApp)
	{
		//launch app:
		if(WeblinkRunApp(FALSE))
			m_bRunApp = TRUE;
	}

	BOOL bshow = FALSE;
	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_WEBLINK,0,UIOPRT_SHOWHIDE,&bshow);

	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.8f);
	m_iconLoading.SetPos(0,0);

	m_iconLink.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_128));
	m_iconLink.SetShowScale(1.4f);
	m_iconLink.SetPos(0,80);

	COLORGL cr={1,1,1,1.0};
	COLORGL cr_down={0.465,0.824,0.965,1.0};
	SIZE si0={800,120};

	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(GetResourceString(IDS_MHL_MAIN_PROMPT));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-70);
	m_tagTitle.SetCaptionHeight(28);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);

	return TRUE;
}


void CPanelWebLink::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnDeviceArrivedReady]:%s \r\n"), lpDeviceName));
}

void CPanelWebLink::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnDeviceRemovedReady]:%s \r\n"), lpDeviceName));
	if (wcsstr(lpDeviceName,_T("AAC")))
	{
		m_bConnect = FALSE;
		m_bWindowEnable = FALSE;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_WEBLINK,0,UIOPRT_SHOWHIDE,&m_bWindowEnable);
		::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_WEBLINK,m_bWindowEnable);
		if(m_idSystemPowerState == SYS_POWER_ON &&
			UI_CLASS_WEBLINK == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI))
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
	}
}

BOOL CPanelWebLink::WeblinkRunApp(BOOL bShow)
{
	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("WebLink\\WebLinkClientApp.exe"));
    LPCTSTR lpCmd = NULL;
	//start APP
	SHELLEXECUTEINFO ShExeInfo={0};
	ShExeInfo.cbSize=sizeof(SHELLEXECUTEINFO);
	ShExeInfo.fMask=SEE_MASK_NOCLOSEPROCESS;
	ShExeInfo.hwnd=NULL;
	ShExeInfo.lpVerb=NULL;
	ShExeInfo.lpFile = strFile.String();
	ShExeInfo.lpParameters=lpCmd;
	ShExeInfo.lpDirectory=NULL;
	ShExeInfo.nShow=SW_SHOW;
	ShExeInfo.hInstApp=NULL;

	if(!ShellExecuteEx(&ShExeInfo))
	{
		RETAILMSG(1, (TEXT("ERR:[CPanelWebLink::WeblinkRunApp] ShellExecuteEx %s ERROR\r\n"),ShExeInfo.lpFile));
		return FALSE;
	}
	else
	{
		RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::WeblinkRunApp]: ShellExecuteEx %s SUCCEED\r\n"),ShExeInfo.lpFile));
		return TRUE;
	}
	return TRUE;
}

void CPanelWebLink::WeblinkShowWindow(BOOL bShow)
{
	if(m_bConnect)
	{
		RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::WeblinkShowWindow] bShow = %d\r\n"), bShow));
		if(bShow)
		{
			m_pSkinManager->GetGUI()->EnableSwap(FALSE);
			if (CIPCApp::GetInstance().ShowWindow(true))
			{
				m_bWeblinkVisible = TRUE;
			}
			else
			{
				m_bWeblinkVisible = FALSE;
				m_pSkinManager->GetGUI()->EnableSwap(TRUE);
			}
		}
		else
		{
			if (CIPCApp::GetInstance().ShowWindow(false))
			{
				m_bWeblinkVisible = FALSE;
			}
			else
			{
				m_bWeblinkVisible = TRUE;
			}
			m_pSkinManager->GetGUI()->EnableSwap(TRUE);
		}
	}
}

void CPanelWebLink::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	//RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::HandleControlMSG] idControl[%d] wMsg[%d] wPara[%d]\r\n"),idControl,wMsg,wPara));
	switch (idControl)
	{
	case IDC_MHL_BTN_HOME:
		break;
	case IDC_EASYCONNECTED_BTN_CANCEL:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	}
}

void CPanelWebLink::OnBack()
{
	RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnBack]\r\n")));
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
}
void CPanelWebLink::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
	//RETAILMSG(DEBUG_WEBLINK, (TEXT("CPanelWebLink::OnTouchDown \r\n")));

}


void CPanelWebLink::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);


}
void CPanelWebLink::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);
	//RETAILMSG(DEBUG_WEBLINK, (TEXT("CPanelWebLink::OnTouchMove \r\n")));

}

#if CVTE_EN_KEY_PROXY
BOOL CPanelWebLink::OnKey(UINT idKey,UINT idStatus)
{
	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnKey] idKey[%d]\r\n"),idKey));

	switch (idKey)
	{
	case KEY_PREVIOUS:
		{
        	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnKey] Prev\r\n")));
			if(GetAudioSRC() != SRC_AUDIO_ARM)
			{
				SwitchAudioSRC(SRC_AUDIO_ARM);
			}
			if ( idStatus == EV_KEY_SHORT_PRESS )
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PREVIOUS);
			}
			else if ( idStatus == EV_KEY_LONG_PRESS )
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_RW);
			}
		}
        break;
	case KEY_NEXT:
		{
        	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnKey] Next\r\n")));
			if(GetAudioSRC() != SRC_AUDIO_ARM)
			{
				SwitchAudioSRC(SRC_AUDIO_ARM);
			}
			if ( idStatus == EV_KEY_SHORT_PRESS )
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_NEXT);
			}
			else if ( idStatus == EV_KEY_LONG_PRESS )
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_FF);
			}
		}
        break;
	case KEY_RESUME:
	case KEY_PLAY:
		{
			if ( idStatus == EV_KEY_UP )
			{
				if(GetAudioSRC() != SRC_AUDIO_ARM)
				{
					SwitchAudioSRC(SRC_AUDIO_ARM);
				}
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
			}
		}
		break;
	case KEY_PAUSE:
		{
			if ( idStatus == EV_KEY_UP )
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PAUSE);
			}
		}
		break;
	case KEY_STOP:
		{
			if ( idStatus == EV_KEY_UP )
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_STOP);
			}
		}
		break;
	case KEY_HOME_POWER:
		{
			if ( idStatus == EV_KEY_SHORT_PRESS )
			{
				CIPCApp::GetInstance().OnHome();
			}
			else if ( idStatus == EV_KEY_DOUBLE_PRESS )
			{
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			}
		}
	default:
		break;
	}

	return TRUE;
}
#else
BOOL CPanelWebLink::OnKey(UINT idKey)
{
	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnKey] idKey[%d]\r\n"),idKey));

	switch (idKey)
	{
    case KEY_MEDIA_PREVIOUS:
	case KEY_MCU_SEEK_PREV:
		{
        	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnKey] Prev\r\n")));
			SwitchAudioSRC(SRC_AUDIO_ARM);
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PREVIOUS);
		}
        break;
    case KEY_MEDIA_NEXT:
	case KEY_MCU_SEEK_NEXT:
		{
        	RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnKey] Next\r\n")));
			SwitchAudioSRC(SRC_AUDIO_ARM);
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_NEXT);
		}
        break;
	case KEY_MCU_ENTER:
		break;

	case KEY_MEDIA_PLAY:
		{
			if(GetAudioSRC() != SRC_AUDIO_ARM)
			{
				SwitchAudioSRC(SRC_AUDIO_ARM);
			}
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
		}
		break;
	}

	return TRUE;

}
#endif

void CALLBACK CPanelWebLink::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	//RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnNotify] idControl[%d],wMsg[%d],wPara[%d]\r\n"),idControl,wMsg,wPara));

	CPanelWebLink *panel=(CPanelWebLink*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelWebLink::OnSpeechStart()
{
	//RETAILMSG(DEBUG_WEBLINK,(_T("............CPanelWebLink OnSpeechStart......................\r\n")));

	m_bSpeechOn = TRUE;

#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_WEBLINK)
	{
		//pull down volume
		//if(g_BTModule.IsInitialized())
		{
			DWORD dwGPS=m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS;
			DWORD dwMedia=m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia;

			DWORD dwVol=0;

			if(dwMedia)
			{
				if(dwGPS>dwMedia)
					dwVol = (dwGPS>0?A2DP_LOW_VOL*dwMedia/dwGPS:A2DP_LOW_VOL);
				else
					dwVol = (dwMedia>0?A2DP_LOW_VOL*dwGPS/dwMedia:A2DP_LOW_VOL);
			}

			RETAILMSG(DEBUG_WEBLINK,(_T("MSG:[CPanelWebLink::OnSpeechStart]: dwVol:%d \r\n"),dwVol));

			//pull down volume
			SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+dwVol);
		}
	}
#endif
}
void CPanelWebLink::OnSpeechTerminate()
{
	//RETAILMSG(DEBUG_WEBLINK,(_T("............CPanelWebLink OnSpeechTerminate......................\r\n")));

	m_bSpeechOn = FALSE;

#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_WEBLINK)
	{
		//pull up volume
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
	}
#endif
}

void CPanelWebLink::OnBluetoothMsg(WPARAM wParam, LPARAM lParam)
{
	BT_MSG *msg = ((BT_MSG*)lParam);

	UINT eventID=msg->eventID;
	UINT _DataID=msg->_DataID;
	WCHAR* _data=msg->_data;
	UINT _dataLen=msg->_dataLen;

	switch (eventID)
	{
	case FLC_BT_EVENT_A2DPCONNECT_STATE:
		{
			m_bA2dpConnected = (A2DP_CONNECT_STATE_CONNECT == _DataID);
			RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnBluetoothMsg] m_bA2dpConnected[%d]\r\n"), m_bA2dpConnected));
		}
		break;

	case FLC_BT_EVENT_CONNECT_STATE:
		{

		}
		break;

	case FLC_BT_EVENT_A2DP_PLAYSTATE:
		{
			m_bPlayingFlag=(PLAYER_STATE_PLAY == _DataID);
			RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnBluetoothMsg] m_bPlayingFlag[%d]\r\n"), m_bPlayingFlag));
		}
		break;

	default:
		break;
	}
	//Do not delete
}

void CPanelWebLink::OnInit( bool init )
{
	RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnInit] init = %s\r\n"), init?_T("true"):_T("false")));
}

void CPanelWebLink::OnConnect( bool connect )
{
	RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnConnect] connect = %s\r\n"), connect?_T("true"):_T("false")));
	m_bConnect = connect?TRUE:FALSE;

	if (m_bConnect)
	{
		if(UI_CLASS_WEBLINK != GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI))
 		{
			SwitchPage(UI_CLASS_WEBLINK,PAGE_PHONELINK_WRAPPER);
		}
	}
	else
	{
		if(m_idSystemPowerState == SYS_POWER_ON &&
			UI_CLASS_WEBLINK == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI))
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
	}
}

void CPanelWebLink::OnWindowStatus( bool enable )
{
	RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnWindowStatus] enable = %s\r\n"), enable?_T("true"):_T("false")));
	m_bWindowEnable = enable?TRUE:FALSE;
	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_WEBLINK,0,UIOPRT_SHOWHIDE,&m_bWindowEnable);
	::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_WEBLINK,m_bWindowEnable);
	if (m_bWindowEnable)
	{
		//m_pSkinManager->GetGUI()->EnableSwap(FALSE);
	}
	else
	{
		if(m_idSystemPowerState == SYS_POWER_ON &&
			UI_CLASS_WEBLINK == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI))
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
	}
}

void CPanelWebLink::OnPlaySoundEffect(BOOL bPlay)
{
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_WEBLINK)
	{
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(0<<16)+(bPlay?0:A2DP_MAX_VOL));
	}
}

void CPanelWebLink::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::OnSystemPowerStatePrepare] idState[%d]\r\n"), idState));

	switch (idState)
	{
	case SYS_POWER_OFF:
		{

		}
		break;
	case SYS_POWER_IDLE:
		{
			WeblinkShowWindow(FALSE);
		}
		break;
	case SYS_POWER_ON:
		{

		}
		break;
	}

}

void CPanelWebLink::OnDeviceChange(LPTSTR lpDevName, BOOL bNew)
{
	PDEV_BROADCAST_PORT pdbp;
	DWORD dwLen;
	LPTSTR lpStr;


	dwLen = sizeof(DEV_BROADCAST_HDR) + (_tcslen(lpDevName) + 1)*sizeof(TCHAR);

	pdbp = (PDEV_BROADCAST_PORT)LocalAlloc(LPTR, dwLen);
	if(pdbp == NULL)
	{
		RETAILMSG(1, (TEXT("%s LocalAlloc failed.\r\n"), TEXT(__FUNCTION__)));
		return;
	}

	//
	// Broadcast the arrival of the Weblink in the system
	//

	pdbp->dbcp_devicetype = DBT_DEVTYP_PORT;
	pdbp->dbcp_reserved = 0;
	lpStr = (LPTSTR)&(pdbp->dbcp_name[0]);
	_tcscpy(lpStr, lpDevName);
	pdbp->dbcp_size = dwLen;

	RETAILMSG(1, (TEXT("%s %s,%d size:%d.\r\n"), TEXT(__FUNCTION__),lpDevName, bNew, dwLen));

	LocalFree(pdbp);
}

void CPanelWebLink::AutoResumeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelWebLink* panel = (CPanelWebLink*)dwUser;
	if ( panel != NULL )
	{
		panel->AutoResumeTimer();
	}
}

void CPanelWebLink::AutoResumeTimer()
{
	static BYTE nRetry = 0;
	if(AutoResume() || nRetry > 5)
	{
		m_bAutoResumeTimer = FALSE;
		nRetry = 0;
		if ( m_AutoResumeTimer != 0 )
		{
			timeKillEvent(m_AutoResumeTimer);
			m_AutoResumeTimer = 0;
		}
	}
	else
	{
		nRetry++;
	}
}

BOOL CPanelWebLink::AutoResume()
{
	BOOL bDone = FALSE;
	if(!m_bAutoResumeFlag)
	{
		return TRUE;
	}

	//auto resume:
	if(GetTickCount()-m_dwEnterTick>500)
	{
		if(m_bA2dpConnected)
		{
			if(!m_bPlayingFlag)
			{
				RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::AutoResume]: A2DP_CONTROL_PLAY\r\n")));
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_ENABLE);
				SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
			}
			else
			{
				RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::AutoResume]: m_bPlayingFlag=TRUE\r\n")));
			}
		}
		else
		{
			RETAILMSG(DEBUG_WEBLINK, (TEXT("MSG:[CPanelWebLink::AutoResume]: Do nothing\r\n")));
			//SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_NONE);
		}
		m_bAutoResumeFlag = FALSE;
		bDone = TRUE;
	}
	else
	{
		RETAILMSG(DEBUG_WEBLINK, (TEXT("ERR:[CPanelWebLink::AutoResume]: Tick:%d\r\n"),
			GetTickCount()-m_dwEnterTick));
	}

	return bDone;
}


