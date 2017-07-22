#include "PanelEasyConnected.h"
#include "../../resource.h"
#include <dbt.h>
extern HWND					g_hMainWnd;
extern const wchar_t* g_strInstallPath;

#define EC_WND_CLASSNAME 			TEXT("EasyConnected")
#define EC_WND_NAME		  		TEXT("EasyConnect")
#define EC_HIDDEN_WND_CLASSNAME 	TEXT("HiddenEasyConnected")
#define EC_HIDDEN_WND_NAME		  	TEXT("HiddenEasyConnect")
#define DEVICE_HKEY_LOCATION 		TEXT("Drivers\\USB\\ClientDrivers\\ADB_Driver")

#define DRIVER_DEFAULT_DEVICE		TEXT("ADB1:")
#define IOCTL_ADB_GET_DEVICE 0x0001
#define TIMER_SWITCH_LANGUAGE		100
#define TIMER_AUTO_RESUME			101

BOOL g_bEasyConnectedInitSrv = FALSE;
BOOL g_bEasyConnectedRunApp = FALSE;


#if 0
//to register and get user message
#define GET_USER_MESSAGE_ID(name) ::RegisterWindowMessage(name)

//Ev_AutoSearchEnd
#define UWM_PHONELINK_EXIT   _T("UWM_PHONELINK_EXIT-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_DISCONNECT _T("UWM_PHONELINK_DISCONNECT-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_CONNECTING _T("UWM_PHONELINK_CONNECTING-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_CONNECTED _T("UWM_PHONELINK_CONNECTED-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_CONNECT_FAIL _T("UWM_PHONELINK_CONNECT_FAIL-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_START _T("UWM_PHONELINK_START-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_STOP _T("UWM_PHONELINK_STOP-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_HAND_CLIENT   _T("UWM_PHONELINK_HAND_CLIENT-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_HAND_MAIN   _T("UWM_PHONELINK_HAND_MAIN-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_CLIENT_TRANS   _T("UWM_PHONELINK_CLIENT_TRANS-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_MAIN_TRANS   _T("UWM_PHONELINK_MAIN_TRANS-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_INSTALL   _T("UWM_PHONELINK_INSTALL-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_INSTALL_OK   _T("UWM_PHONELINK_INSTALL_OK-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_RESET   _T("UWM_PHONELINK_RESET-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_RESET_OK   _T("UWM_PHONELINK_RESET_OK-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_KEY_HOME   _T("UWM_PHONELINK_KEY_HOME-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_KEY_MENU   _T("UWM_PHONELINK_KEY_MENU-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_KEY_BACK   _T("UWM_PHONELINK_KEY_BACK-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_NO_LICENSE   _T("UWM_PHONELINK_NO_LICENSE-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
#define UWM_PHONELINK_NO_USB_DEBUG   _T("UWM_PHONELINK_NO_USB_DEBUG-{7049499B-0480-44f9-BF71-4AE91FA0443C}")


const UINT ID_UWM_PHONELINK_EXIT = GET_USER_MESSAGE_ID(UWM_PHONELINK_EXIT);
const UINT ID_UWM_PHONELINK_DISCONNECT = GET_USER_MESSAGE_ID(UWM_PHONELINK_DISCONNECT);
const UINT ID_UWM_PHONELINK_CONNECTING = GET_USER_MESSAGE_ID(UWM_PHONELINK_CONNECTING);
const UINT ID_UWM_PHONELINK_CONNECTED = GET_USER_MESSAGE_ID(UWM_PHONELINK_CONNECTED);
const UINT ID_UWM_PHONELINK_CONNECT_FAIL = GET_USER_MESSAGE_ID(UWM_PHONELINK_CONNECT_FAIL);
const UINT ID_UWM_PHONELINK_START = GET_USER_MESSAGE_ID(UWM_PHONELINK_START);
const UINT ID_UWM_PHONELINK_STOP = GET_USER_MESSAGE_ID(UWM_PHONELINK_STOP);
const UINT ID_UWM_PHONELINK_HAND_CLIENT = GET_USER_MESSAGE_ID(UWM_PHONELINK_HAND_CLIENT);
const UINT ID_UWM_PHONELINK_HAND_MAIN = GET_USER_MESSAGE_ID(UWM_PHONELINK_HAND_MAIN);
const UINT ID_UWM_PHONELINK_CLIENT_TRANS = GET_USER_MESSAGE_ID(UWM_PHONELINK_CLIENT_TRANS);
const UINT ID_UWM_PHONELINK_MAIN_TRANS = GET_USER_MESSAGE_ID(UWM_PHONELINK_MAIN_TRANS);
const UINT ID_UWM_PHONELINK_INSTALL = GET_USER_MESSAGE_ID(UWM_PHONELINK_INSTALL);
const UINT ID_UWM_PHONELINK_INSTALL_OK = GET_USER_MESSAGE_ID(UWM_PHONELINK_INSTALL_OK);
const UINT ID_UWM_PHONELINK_RESET = GET_USER_MESSAGE_ID(UWM_PHONELINK_RESET);
const UINT ID_UWM_PHONELINK_RESET_OK = GET_USER_MESSAGE_ID(UWM_PHONELINK_RESET_OK);
const UINT ID_UWM_PHONELINK_KEY_HOME = GET_USER_MESSAGE_ID(UWM_PHONELINK_KEY_HOME);
const UINT ID_UWM_PHONELINK_KEY_MENU = GET_USER_MESSAGE_ID(UWM_PHONELINK_KEY_MENU);
const UINT ID_UWM_PHONELINK_KEY_BACK = GET_USER_MESSAGE_ID(UWM_PHONELINK_KEY_BACK);
const UINT ID_UWM_PHONELINK_NO_LICENSE = GET_USER_MESSAGE_ID(UWM_PHONELINK_NO_LICENSE);
const UINT ID_UWM_PHONELINK_NO_USB_DEBUG = GET_USER_MESSAGE_ID(UWM_PHONELINK_NO_USB_DEBUG);
#else
//#define GET_USER_MESSAGE_ID(name) ::RegisterWindowMessage(name)
//#define UWM_EASYCONNECTED_HAND_CLIENT   _T("UWM_EASYCONNECTED_HAND_CLIENT-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
//#define UWM_EASYCONNECTED_HAND_MAIN   _T("UWM_EASYCONNECTED_HAND_MAIN-{7049499B-0480-44f9-BF71-4AE91FA0443C}")
//const UINT ID_UWM_EASYCONNECTED_HAND_CLIENT = GET_USER_MESSAGE_ID(UWM_EASYCONNECTED_HAND_CLIENT);
//const UINT ID_UWM_EASYCONNECTED_HAND_MAIN = GET_USER_MESSAGE_ID(UWM_EASYCONNECTED_HAND_MAIN);


#define UM_NET_EASYCONN		0x7001  //消息定义
#define EC_PHONE_PLUGIN		0x01    //手机插入
#define EC_PHONE_PLUGOUT    	0x02    //手机拔出
#define EC_PROCESS_START		0x03    //程序启动
#define EC_PROCESS_EXIT		0x04    //程序退出
#define EC_INTERACTIVE_START 	0x05    //手机互连开始
#define EC_INTERACTIVE_EXIT 	0x06    //手机互连退出
#define ECST_GUI_MIN			0x08    //程序最小化窗口消息
#define ECST_FOCUS_CHANGE		0x09    //亿连程序窗口得到和失去焦点 LPARAM:0 失去焦点 1 得到焦点
#define ECST_NEED_ACTIVE		0x0A    //需要激活

#define UM_EASYCONNET_LANG		0x7006
#define EC_LANG_ARABIC			1025
#define EC_LANG_CHINESE_SIM	2052
#define EC_LANG_CHINESE_TRAD	1028
#define EC_LANG_ENGLISH		1033
#define EC_LANG_PORTUGUESE		2070
#define EC_LANG_RUSSIAN		1049
#define EC_LANG_KOREAN			1042

#define UM_EASYCONNET_IN 		0x7008  //按键消息
#define EC_KEY_VOLUME_UP      	0x01    //手机音量加
#define EC_KEY_VOLUME_DOWN		0x02    //手机音量减
#define EC_KEY_HOME			0x03
#define EC_KEY_MENU			0x04
#define EC_KEY_BACK			0x05
#define OUTER_PLAY				0x06    //手机音乐播放
#define OUTER_PAUSE			0x07    //手机音乐暂停
#define OUTER_PLAY_PREV		0x08    //手机音乐上一曲
#define OUTER_PLAY_NEXT		0x09    //手机音乐下一曲

#define INER_QUIT				0xFF01  //外部退出亿连
#define BLUETOOTH_STATUS		0xFF02  //蓝牙状态通知 参数 0：蓝牙断开 1：蓝牙连接
#define DRIVE_MODE				0xFF03  //行车模式通知 参数 0：停车 1：行车
#endif

CPanelEasyConnected::CPanelEasyConnected(void)
{
	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::CPanelEasyConnected] \r\n")));

	m_hwndEasyConnectedNotify = NULL;
	//m_hwndClient = NULL;

	m_hmCoreDll = NULL;
	m_fnSendNotifyMessage = NULL;

	m_bEasyConnectedVisible = FALSE;
	m_bSpeechOn = FALSE;
	m_bExitEasyConnected = FALSE;
	m_bIsReadyToScreen = FALSE;
    m_bLanguageSwitched = FALSE;
	m_nConnectedStatus = EC_PHONE_PLUGOUT;
	m_bAutoSwitchToEasyConnected = FALSE;
    m_bInteractive = FALSE;
	m_bAutoResumeFlag = FALSE;
	m_bFocusGet = FALSE;
    m_dwSetLanguageTick = 0;

	m_iPowerStatus = SYS_POWER_ON;

	m_bA2dpOn = FALSE;
	m_dwEnterTick = 0;
	m_bPlayingFlag = FALSE;
	m_bAutoResumeTimer = FALSE;
}

CPanelEasyConnected::~CPanelEasyConnected(void)
{
	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::~CPanelEasyConnected] \r\n")));

	EasyConnectedDeinitSrv();
}
BOOL CPanelEasyConnected::IsReady()
{

	if(m_nConnectedStatus==EC_INTERACTIVE_START&& m_bIsReadyToScreen && m_bEasyConnectedVisible && m_bLanguageSwitched)
		return TRUE;
	else
	{
	    //RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::IsReady m_nConnectedStatus[%d] m_bEasyConnectedVisible[%d]\r\n"),m_nConnectedStatus,m_bEasyConnectedVisible));
		return FALSE;
	}
	//return CPanelGL::IsReady();
}
void CPanelEasyConnected::OnSRCChanged(UINT idSRC)
{
#if 0
	//Mark20160628 whh
	//Do not operate bt cmd here, we do in bluetoothwrapper panel OnSRChanged.
	RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::OnSRCChanged %x\r\n"),idSRC));
	
	if(	GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_EASYCONNECTED
		&& GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_PHONELINK
		&& GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_BT)
	{
		RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnSRCChanged]: Stop BT play\r\n")));
		SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_DISABLE);
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+0);
	}
#endif
}

void CPanelEasyConnected::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
//		RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::OnQuitPanel iParam[%d] idAni[%d] m_nConnectedStatus[%d] m_bEasyConnectedVisible[%d]\r\n"),
//			iParam,
//			idAni,
//			m_nConnectedStatus,
//			m_bEasyConnectedVisible));

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	m_bExitEasyConnected = TRUE;
	if(m_bAutoResumeTimer)
	{
		RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnQuitPanel] KillTimer TIMER_AUTO_RESUME\r\n")));
		m_bAutoResumeTimer = FALSE;
		KillTimer(m_hwndEasyConnectedNotify,TIMER_AUTO_RESUME);
	}

	//if(m_bEasyConnectedVisible)
	{
		//RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnQuitPanel] hidewindow\r\n")));
		EasyConnectedShowWindow(FALSE);
	}
}
void CPanelEasyConnected::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
 	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
//		RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::OnEnterPanel iParam[%d] idAni[%d]\r\n"),iParam,idAni));

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

	m_bExitEasyConnected = FALSE;
	//
	//SendBluetoothCommand(BT_CMD_GET_A2DP_STREAM,LPARAM(&m_bA2dpOn));

	//RETAILMSG(1,(_T("MSG:[%s]: m_bA2dpOn :%d \r\n"),TEXT(__FUNCTION__),m_bA2dpOn));
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
	//切换到UI_CLASS_EASYCONNECTED会使得BT与Easyconnect互切时，蓝牙音乐卡顿一下
	SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP));
	//SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER));

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

		RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[%s]: dwVol:%d ......................\r\n"),TEXT(__FUNCTION__),dwVol));

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
//		::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_EASYCONNECT,0x40);

	SetMsg(GetResourceString(IDS_PHONELINK_MAIN_PROMPT));

	m_bAutoResumeFlag = TRUE;
	if(m_hwndEasyConnectedNotify)
	{
		m_bAutoResumeTimer = TRUE;
		SetTimer(m_hwndEasyConnectedNotify,TIMER_AUTO_RESUME,500,NULL);
	}
	m_dwEnterTick = GetTickCount();
}

void CPanelEasyConnected::Render()
{
	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;

// 	COLORGL cr_b={0.0,0.0,0.0,m_fCurAlpha};

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

// 	DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr_b);

//	m_btnCancel.Show(!m_nConnectedStatus);

/*
	if(!m_nConnectedStatus)
	{
		//RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::Render m_nConnectedStatus[%d]\r\n"),m_nConnectedStatus));

		m_tagTitle.SetAlpha(m_fCurAlpha);
		m_tagTitle.Render();

		m_iconLink.SetAlpha(m_fCurAlpha);
		m_iconLink.Render();

		m_btnCancel.SetAlpha(m_fCurAlpha);
		m_btnCancel.Render();
	}
*/

	if(((!m_bIsReadyToScreen || !m_bEasyConnectedVisible || !g_bEasyConnectedRunApp || !m_bLanguageSwitched) && !m_bExitEasyConnected && !m_bInteractive))
	{
		VIEW_STATE vs = m_iconLoading.GetCurViewState();
		vs.fRotateZ -= 6;
		if(vs.fRotateZ < -360)
			vs.fRotateZ = 0;
		m_iconLoading.SetCurViewState(&vs);
		m_iconLoading.SetAlpha(m_fCurAlpha);
		m_iconLoading.Render();

		//RETAILMSG(DEBUG_EASYCONNECTED,(_T("Render ### 1 ### IsReadyToScreen[%d] EasyConnectedVisible[%d] EasyConnectedRunApp[%d] ExitEasyConnected[%d]\r\n"),
		//	m_bIsReadyToScreen,
		//	m_bEasyConnectedVisible,
		//	g_bEasyConnectedRunApp,
		//	m_bExitEasyConnected));
		//printf("CPanelEasyConnected::Render ### 1 ### %4.0f\r\n",vs.fRotateZ);
	}

	if(IsAniReady() && GetTickCount()-m_dwEnterTick>500)
	{
		if(!g_bEasyConnectedInitSrv)
		{
			//launch service:
			RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::Render] EasyConnectedInitSrv\r\n")));
			if(EasyConnectedInitSrv())
			{
				g_bEasyConnectedInitSrv = TRUE;
			}
		}
		if(!g_bEasyConnectedRunApp)
		{
			//launch app:
			RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::Render] EasyConnectedRunApp\r\n")));
			if(EasyConnectedRunApp(TRUE))
				g_bEasyConnectedRunApp = TRUE;
		}

		else if(!m_bEasyConnectedVisible)
		{
			RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::Render] EasyConnectedShowWindow\r\n")));
			EasyConnectedShowWindow(TRUE);
		}

	}

	//auto resume:
//		if(m_bInteractive && m_bAutoResumeFlag && m_bEasyConnectedVisible && IsAniReady() && GetTickCount()-m_dwEnterTick>1000)
//		{
//			RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::Render Auto resume BT A2DP\r\n")));
//			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
//			m_bAutoResumeFlag = FALSE;
//		}
	EndRender();

	//RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::Render -\r\n")));

}
void CPanelEasyConnected::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);
	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnGestureMsg] idGesture[%d]\r\n"),idGesture));

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
BOOL CPanelEasyConnected:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::Initialize]: ++ \r\n")));
	m_bShowStatusBar = FALSE;

	if(!g_bEasyConnectedInitSrv)
	{
		//launch service:
		if(EasyConnectedInitSrv())
		{
			g_bEasyConnectedInitSrv = TRUE;
		}
	}
	if(!g_bEasyConnectedRunApp)
	{
		//launch app:
		if(EasyConnectedRunApp(FALSE))
			g_bEasyConnectedRunApp = TRUE;
	}

	if(EasyConnectedDeviceStatusCheck()>=1)
	{
		BOOL bshow = TRUE;
		m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_EASYCONNECTED,0,UIOPRT_SHOWHIDE,&bshow);
	}

	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.8f);
	m_iconLoading.SetPos(0,0);

	m_iconLink.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_128));
	m_iconLink.SetShowScale(1.4f);
	m_iconLink.SetPos(0,80);

	COLORGL cr={1,1,1,1.0};
	COLORGL cr_down={0.465,0.824,0.965,1.0};
	SIZE si0={800,120};

//		//cancel
//		m_btnCancel.Initialize(IDC_EASYCONNECTED_BTN_CANCEL,this,m_pSkinManager->GetTexture(TEXID_043),NULL,
//			m_pSkinManager->GetTexture(TEXID_044),NULL,
//			NULL,NULL,
//			NULL,NULL);
//		m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
//		m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
//		m_btnCancel.SetCaptionColor(&cr,&cr_down);
//		m_btnCancel.SetCaptionHeight(32);
//		m_btnCancel.LinkTextGL(pTextGL);
//		m_btnCancel.SetSize(&si0);
//		m_btnCancel.SetNotifyFunc(& CPanelEasyConnected::OnNotify,this);
//		m_btnCancel.SetPos(0,-180);

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


void CPanelEasyConnected::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::OnDeviceArrivedReady]:%s \r\n"), lpDeviceName));

	if(!lpDeviceName)
		return;

	if(wcsstr(lpDeviceName,_T("ECD1")))//
	{
		if(UI_CLASS_EASYCONNECTED != GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
 		{
			SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_PHONELINK_WRAPPER);
			EasyConnectedShowWindow(TRUE);
		}
		//m_bAutoSwitchToEasyConnected = FALSE;
	}
}

void CPanelEasyConnected::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::OnDeviceRemovedReady]:%s \r\n"), lpDeviceName));

	if(wcsstr(lpDeviceName,_T("ECD1:")))
	{
		if(m_idSystemPowerState == SYS_POWER_ON &&
			UI_CLASS_EASYCONNECTED == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
	}

}

#if 0
void CPanelEasyConnected::NotifyConnected(BYTE nConnectedStatus)
{
	m_nConnectedStatus = nConnectedStatus;

	switch(nConnectedStatus)
	{
        case EC_PHONE_PLUGIN:
		if(m_bAutoSwitchToEasyConnected)
		{
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_EASYCONNECTED)
			{
				SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_PHONELINK_WRAPPER);
				//m_bAutoSwitchToEasyConnected = FALSE;
			}
		}
        break;
        case EC_PHONE_PLUGOUT:
        {
    		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_EASYCONNECTED)
    		{
    			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
    		}
    		m_bEasyConnectedVisible = FALSE;
        }
        break;
        case EC_INTERACTIVE_START:
        {
//            ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_EASYCONNECT,1);
        }
        break;
        case EC_INTERACTIVE_EXIT:
        {
//            ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_EASYCONNECT,0);
        }
        break;
	}

}
#endif

BOOL CPanelEasyConnected::EasyConnectedRunApp(BOOL bShow)
{
	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("easyconnected\\EasyConnected.exe"));

	if(bShow)
	{
        LPCTSTR lpCmd = NULL;
        switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
        {
		case LANGUAGE_ENGLISH:	// English
			lpCmd = TEXT("-lan 1033");
			break;
		case LANGUAGE_CHN_TRA:	//Chinese Traditional
			lpCmd = TEXT("-lan 1028");
			break;
		case LANGUAGE_CHN_SIM: //Chinese Simplified
			lpCmd = TEXT("-lan 2052");
			break;
		case LANGUAGE_KOREAN:
			lpCmd = TEXT("-lan 1042");
			break;
        case LANGUAGE_PORTUGUESE:
			lpCmd = TEXT("-lan 2070");
			break;
        case LANGUAGE_RUSSIAN:
			lpCmd = TEXT("-lan 1049");
			break;
		default:
			lpCmd = TEXT("-lan 1033");
			break;
        }
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
			RETAILMSG(1, (TEXT("ERR:[CPanelEasyConnected::EasyConnectedRunApp] ShellExecuteEx %s ERROR\r\n"),ShExeInfo.lpFile));
			return FALSE;
		}
		else
		{
			RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::EasyConnectedRunApp]: ShellExecuteEx %s SUCCEED\r\n"),ShExeInfo.lpFile));
            m_dwSetLanguageTick = GetTickCount();
            //RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::EasyConnectedRunApp  SetTimer(m_hwndEasyConnectedNotify)[%x]\r\n"), m_hwndEasyConnectedNotify));

			//SetTimer(m_hwndEasyConnectedNotify,TIMER_SWITCH_LANGUAGE,300,NULL);

			return TRUE;
		}
	}
	else
	{
        LPCTSTR lpCmd = NULL;
        switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
        {
		case LANGUAGE_ENGLISH:	// English
			lpCmd = TEXT("-lan 1033 -h");
			break;
		case LANGUAGE_CHN_TRA:	//Chinese Traditional
			lpCmd = TEXT("-lan 1028 -h");
			break;
		case LANGUAGE_CHN_SIM: //Chinese Simplified
			lpCmd = TEXT("-lan 2052 -h");
			break;
		case LANGUAGE_KOREAN:
			lpCmd = TEXT("-lan 1042 -h");
			break;
        case LANGUAGE_PORTUGUESE:
			lpCmd = TEXT("-lan 2070 -h");
			break;
        case LANGUAGE_RUSSIAN:
			lpCmd = TEXT("-lan 1049 -h");
			break;
		default:
			lpCmd = TEXT("-lan 1033 -h");
			break;
        }
		PROCESS_INFORMATION pi;
        /*
		STARTUPINFOW   si;
		ZeroMemory(&pi,sizeof(pi));
		si.cb = sizeof(STARTUPINFO);
		si.lpReserved = NULL;
		si.lpDesktop = NULL;
		si.lpTitle = NULL;
		si.dwFlags |= STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		si.cbReserved2 = NULL;
		si.lpReserved2 = NULL;
		*/

		if(CreateProcessW(strFile.String(),lpCmd,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))//set correct location:
		{
			RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::EasyConnectedRunApp] CreateProcess SUCCEED:%s\r\n"),strFile.String()));
            m_dwSetLanguageTick = GetTickCount();
            //RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::EasyConnectedRunApp  SetTimer(m_hwndEasyConnectedNotify)[%x]\r\n"), m_hwndEasyConnectedNotify));
            //Sleep(300);
			//SetTimer(m_hwndEasyConnectedNotify,TIMER_SWITCH_LANGUAGE,300,NULL);
		}
		else
		{
			RETAILMSG(1, (TEXT("ERR:[CPanelEasyConnected::EasyConnectedRunApp] CreateProcess ERROR:%s\r\n"),strFile.String()));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPanelEasyConnected::EasyConnectedInitSrv()
{
	RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::EasyConnectedInitSrv]\r\n")));
	/////////////////////////////////
	if(m_hmCoreDll == NULL || m_fnSendNotifyMessage == NULL)
	{
		m_hmCoreDll = (HMODULE)LoadLibrary(TEXT("COREDLL.DLL"));
		if(m_hmCoreDll != NULL)
		{
			m_fnSendNotifyMessage = (PFN_SENDNOTIFYMESSAGE)GetProcAddress(m_hmCoreDll, TEXT("SendNotifyMessageW"));
			FreeLibrary(m_hmCoreDll);
			//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::EasyConnectedInitSrv: Get m_fnSendNotifyMessage \r\n")));
		}
	}


	/////////////////////////////////
	WNDCLASS    wc;
	TCHAR szWndClassName[] = EC_HIDDEN_WND_CLASSNAME;
	memset(&wc, 0, sizeof(wc));
	wc.lpfnWndProc      = CPanelEasyConnected::EasyConnectedWndProc;
	wc.hInstance        = m_moduleEntry.hInstance;
	wc.lpszClassName    = szWndClassName;
	wc.hIcon			= NULL;//LoadIcon(IDR_MAINFRAME);
	//wc.hIconSm			= ::LoadIcon(wc.hInstance , MAKEINTRESOURCE(IDR_MAINFRAME));

	//UnregisterClass(wc.lpszClassName, wc.hInstance);
	if (! RegisterClass(&wc))
	{
		RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("ERR:[CPanelEasyConnected::EasyConnectedInitSrv] RegisterClass ERROR:%d\r\n"), GetLastError()));
		return FALSE;
	}

	m_hwndEasyConnectedNotify = CreateWindow(szWndClassName, szWndClassName, WS_DISABLED | WS_POPUP,
		//CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, 1, 1,
		NULL/*g_hMainWnd*/, NULL,  wc.hInstance, this);
	RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::EasyConnectedInitSrv] m_hwndEasyConnectedNotify[%x]\r\n"), m_hwndEasyConnectedNotify));

	ShowWindow(m_hwndEasyConnectedNotify,SW_HIDE);
	return TRUE;
}

void CPanelEasyConnected::EasyConnectedDeinitSrv()
{
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::EasyConnectedDeinitSrv]\r\n")));

	if(m_hwndEasyConnectedNotify != NULL)
	{
		DestroyWindow(m_hwndEasyConnectedNotify);
		UnregisterClass(EC_HIDDEN_WND_CLASSNAME, m_moduleEntry.hInstance);
		m_hwndEasyConnectedNotify = NULL;
	}
}

void CPanelEasyConnected::EasyConnectedShowWindow(BOOL bShow)
{
	BOOL bRet = FALSE;
	HWND hWnd = ::FindWindow(EC_WND_CLASSNAME,EC_WND_NAME);

	if(IsWindow(hWnd))
	{

		//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::EasyConnectedShowWindow[%d] ### m_hwndEasyConnectedNotify[0x%x]\r\n"),bShow, m_hwndEasyConnectedNotify));

		if(bShow)
		{
			bRet = ::SetWindowPos(hWnd, HWND_TOPMOST , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
			if(bRet)
			{
				m_bEasyConnectedVisible = TRUE;
			}
			else
			{
				RETAILMSG(1, (TEXT("ERR:[CPanelEasyConnected::EasyConnectedShowWindow] SetWindowPos failed\r\n")));
				m_bEasyConnectedVisible = FALSE;
			}
			ShowWindow(hWnd,SW_SHOW);
			SetForegroundWindow(hWnd);
			//m_pSkinManager->GetGUI()->EnableSwap(FALSE);
		}
		else
		{
			bRet = ::SetWindowPos(hWnd, HWND_BOTTOM , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW );
			//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::EasyConnectedShowWindow[%d] SetWindowPos Return[%d]\r\n"),bShow, bRet));

			if(bRet)
			{
				m_bEasyConnectedVisible = FALSE;
			}
			else
			{
				RETAILMSG(1, (TEXT("ERR:[CPanelEasyConnected::EasyConnectedShowWindow] SetWindowPos failed\r\n")));
				m_bEasyConnectedVisible = TRUE;
			}

			//m_pSkinManager->GetGUI()->EnableSwap(TRUE);
		}
	}
	//else
	//	RETAILMSG(1, (TEXT("CPanelEasyConnected::EasyConnectedShowWindow: ####### ERROR ####### FindWindow failed.\r\n")));
}

void CPanelEasyConnected::EasyConnectedSetLanguage()
{
	//DWORD dwLang = GetUserDefaultLangID();
	UINT nLang = m_pSkinManager->GetConfig()->GetCurParam()->idLanguage;
	RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::Initialize] nLang[%d]\r\n"), nLang));

    BOOL bRet;

	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::EasyConnectedSetLanguage ++ nLang[%d]\r\n"), nLang));

    if(GetTickCount() - m_dwSetLanguageTick > 3000)
    {
	    KillTimer(m_hwndEasyConnectedNotify,TIMER_SWITCH_LANGUAGE);
        m_bLanguageSwitched = FALSE;
        RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("ERR:[CPanelEasyConnected::EasyConnectedSetLanguage] -- m_dwSetLanguageTick timeout!!! \r\n")));
        return;
    }

	switch(nLang)
	{
		case LANGUAGE_ENGLISH:	// English
			bRet = PostMsgToClient(UM_EASYCONNET_LANG, EC_LANG_ENGLISH, 0);
			break;
		case LANGUAGE_CHN_TRA:	//Chinese Traditional
			bRet = PostMsgToClient(UM_EASYCONNET_LANG, EC_LANG_CHINESE_TRAD, 0);
			break;
		case LANGUAGE_CHN_SIM: //Chinese Simplified
			bRet = PostMsgToClient(UM_EASYCONNET_LANG, EC_LANG_CHINESE_SIM, 0);
			break;
		case LANGUAGE_KOREAN:
			bRet = PostMsgToClient(UM_EASYCONNET_LANG, EC_LANG_KOREAN, 0);
			break;
		default:
			bRet = PostMsgToClient(UM_EASYCONNET_LANG, EC_LANG_ENGLISH, 0);
			break;
	}

    if(bRet)
    {
	    KillTimer(m_hwndEasyConnectedNotify,TIMER_SWITCH_LANGUAGE);
        m_bLanguageSwitched = TRUE;
        RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::EasyConnectedSetLanguage] --\r\n")));
    }
}

UINT CPanelEasyConnected::EasyConnectedDeviceStatusCheck()
{
	UINT nDeviceNum;
	HANDLE hDevice = CreateFile(DRIVER_DEFAULT_DEVICE,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,INVALID_HANDLE_VALUE);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		hDevice = ActivateDevice(DEVICE_HKEY_LOCATION, 0);
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::EasyConnectedDeviceStatusCheck]: failed to activate device: %d\r\n"), GetLastError()));
			return 0;
		}
		hDevice = CreateFile(DRIVER_DEFAULT_DEVICE,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,INVALID_HANDLE_VALUE);
	}
	DeviceIoControl(hDevice,
		IOCTL_ADB_GET_DEVICE,
		NULL, 0,
		&nDeviceNum, sizeof(nDeviceNum),
		NULL, NULL);
	CloseHandle(hDevice);
	return nDeviceNum;
}


void CPanelEasyConnected::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::HandleControlMSG] idControl[%d] wMsg[%d] wPara[%d]\r\n"),idControl,wMsg,wPara));
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

void CPanelEasyConnected::OnBack()
{
	RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::OnBack]\r\n")));
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
}
void CPanelEasyConnected::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::OnTouchDown \r\n")));

}


void CPanelEasyConnected::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);


}
void CPanelEasyConnected::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::OnTouchMove \r\n")));

}
void CPanelEasyConnected::OnEasyConnectedMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("CPanelEasyConnected::OnEasyConnectedMsg message[%d] wParam[%d]\r\n"),message,wParam));

	if(message == UM_NET_EASYCONN)
	{
        //NotifyConnected(wParam);
		switch(wParam)
		{
			case EC_PHONE_PLUGIN:
				{
					RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >EC_PHONE_PLUGIN\r\n")));
					OnDeviceChange(_T("ECD1:"),TRUE);
            		if(m_bAutoSwitchToEasyConnected)
            		{
            			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_EASYCONNECTED)
            			{
            				SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_PHONELINK_WRAPPER);
            				//m_bAutoSwitchToEasyConnected = FALSE;
            			}
            		}
                }
				break;
			case EC_PHONE_PLUGOUT:
				{
					RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >EC_PHONE_PLUGOUT\r\n")));
                    m_bInteractive = FALSE;
					OnDeviceChange(_T("ECD1:"),FALSE);
					BOOL bshow = FALSE;
                    //EasyConnectedShowWindow(FALSE);
					m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_EASYCONNECTED,0,UIOPRT_SHOWHIDE,&bshow);
                    ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_EASYCONNECT,0);

            		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_EASYCONNECTED)
            		{
            			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
            		}
				}
				break;
			case EC_PROCESS_START:
				RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >EC_PROCESS_START\r\n")));
                m_bIsReadyToScreen = TRUE;
				break;
			case EC_PROCESS_EXIT:
				RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >EC_PROCESS_EXIT\r\n")));
				g_bEasyConnectedRunApp = FALSE;
				m_bIsReadyToScreen = FALSE;
                m_bInteractive = FALSE;
				if(m_iPowerStatus == SYS_POWER_ON)
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
				break;
			case EC_INTERACTIVE_START:
				{
					RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >EC_INTERACTIVE_START\r\n")));
					BOOL bshow = TRUE;
                    ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_EASYCONNECT,1);

					m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_EASYCONNECTED,0,UIOPRT_SHOWHIDE,&bshow);
					/*
					Sleep(200);
					if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_EASYCONNECTED)
					{
						SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_PHONELINK_MAIN);
						EasyConnectedShowWindow(TRUE);
					}
					*/
					m_bInteractive = TRUE;
				}
				break;
			case EC_INTERACTIVE_EXIT:
				{
					RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >EC_INTERACTIVE_EXIT\r\n")));
					/*
					m_bIsReadyToScreen = FALSE;
					if(m_iPowerStatus == SYS_POWER_ON)
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
					*/
					m_bInteractive = FALSE;
                    ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_EASYCONNECT,0);

				}
				break;
			case ECST_FOCUS_CHANGE:
				{
					m_bFocusGet = (BOOL)lParam;
					RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >ECST_FOCUS_CHANGE [%d]\r\n"),m_bFocusGet));
				}
				break;
			case ECST_GUI_MIN:
				{
					RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >ECST_GUI_MIN\r\n")));
					if(m_iPowerStatus == SYS_POWER_ON)
						SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
				}
			default:
				RETAILMSG(1, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >Param Error [%X]!!!\r\n"),wParam));
				break;
		}
	}
	else if(message == UM_EASYCONNET_LANG)
	{
		switch(wParam)
		{
 			case EC_LANG_ARABIC:
			case EC_LANG_CHINESE_SIM:
			case EC_LANG_CHINESE_TRAD:
			case EC_LANG_ENGLISH:
			case EC_LANG_PORTUGUESE:
			case EC_LANG_RUSSIAN:
				break;
		}
	}
	else if(message == UM_EASYCONNET_IN)
	{
		switch(wParam)
		{
			case EC_KEY_VOLUME_UP:
			case EC_KEY_VOLUME_DOWN:
			case EC_KEY_HOME:
 			case EC_KEY_MENU:
  			case EC_KEY_BACK:
				break;
		}
	}
	else if(message == INER_QUIT)
	{
		RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::OnEasyConnectedMsg] >INER_QUIT\r\n")));
	}
}
BOOL CPanelEasyConnected::PostMsgToClient(UINT message, WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::PostMsgToClient] ++ message[%d] wParam[%d]\r\n"),message,wParam));

    HWND hWnd = NULL;
    BOOL bRet = FALSE;
	hWnd = ::FindWindow(EC_WND_CLASSNAME,EC_WND_NAME);
	if(hWnd)
	{
		bRet = ::PostMessage(hWnd,message,wParam,lParam);
	}
	else
	{
		RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("ERR:[CPanelEasyConnected::PostMsgToClient]: FindWindow failed.\r\n"),message,wParam));
	}
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::PostMsgToClient] -- bRet[%d]\r\n"),bRet));
    return bRet;
}
void CPanelEasyConnected::SendMsgToClient(UINT message, WPARAM wParam, LPARAM lParam)
{

}

#if CVTE_EN_KEY_PROXY
BOOL CPanelEasyConnected::OnKey(UINT idKey,UINT idStatus)
{
	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnKey] idKey[%d]\r\n"),idKey));

	switch (idKey)
	{
	case KEY_PREVIOUS:
		{
        	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnKey] Prev\r\n")));
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
        	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnKey] Next\r\n")));
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
	default:
		break;
	}

	return TRUE;
}
#else
BOOL CPanelEasyConnected::OnKey(UINT idKey)
{
	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnKey] idKey[%d]\r\n"),idKey));

	switch (idKey)
	{
    case KEY_MEDIA_PREVIOUS:
	case KEY_MCU_SEEK_PREV:
		{
        	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnKey] Prev\r\n")));
			SwitchAudioSRC(SRC_AUDIO_ARM);
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PREVIOUS);
		}
        break;
    case KEY_MEDIA_NEXT:
	case KEY_MCU_SEEK_NEXT:
		{
        	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnKey] Next\r\n")));
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

void CALLBACK CPanelEasyConnected::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	//RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnNotify] idControl[%d],wMsg[%d],wPara[%d]\r\n"),idControl,wMsg,wPara));

	CPanelEasyConnected *panel=(CPanelEasyConnected*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
LRESULT CALLBACK CPanelEasyConnected::EasyConnectedWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(DEBUG_EASYCONNECTED,(_T("CPanelEasyConnected::EasyConnectedWndProc message[%d],wParam[%d]\r\n"),message,wParam));

	static CPanelEasyConnected *panel=NULL;

	switch(message)
	{
		case WM_CREATE:
		{
			SetCursor(NULL);
			ShowCursor(FALSE);
			panel = (CPanelEasyConnected *)(((CREATESTRUCT*)lParam)->lpCreateParams);
		}
		break;
		case WM_TIMER:
		{
			if(panel)
			{
				if(wParam == TIMER_SWITCH_LANGUAGE)
				{
					panel->EasyConnectedSetLanguage();
				}
				else if(wParam == TIMER_AUTO_RESUME)
				{
					static BYTE nRetry = 0;
					if(panel->AutoResume() || nRetry > 5)
					{
						panel->m_bAutoResumeTimer = FALSE;
						nRetry = 0;
						KillTimer(panel->m_hwndEasyConnectedNotify,TIMER_AUTO_RESUME);
					}
					else
					{
						nRetry++;
					}
				}
			}
		}
		break;
		default:
		{
			if(panel)
			{
				panel->OnEasyConnectedMsg(message,wParam,lParam);
			}
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

}

void CPanelEasyConnected::OnSpeechStart()
{
	//RETAILMSG(DEBUG_EASYCONNECTED,(_T("............CPanelEasyConnected OnSpeechStart......................\r\n")));

	m_bSpeechOn = TRUE;

#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_EASYCONNECTED)
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

			RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnSpeechStart]: dwVol:%d \r\n"),dwVol));

			//pull down volume
			SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+dwVol);
		}
	}
#endif
}
void CPanelEasyConnected::OnSpeechTerminate()
{
	//RETAILMSG(DEBUG_EASYCONNECTED,(_T("............CPanelEasyConnected OnSpeechTerminate......................\r\n")));

	m_bSpeechOn = FALSE;

#if !USE_WAV_ONCHIP
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_EASYCONNECTED)
	{
		//pull up volume
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
	}
#endif
}

void CPanelEasyConnected::OnBluetoothMsg(WPARAM wParam, LPARAM lParam)
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
			RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::OnBluetoothMsg] m_bA2dpConnected[%d]\r\n"), m_bA2dpConnected));
		}
		break;

	case FLC_BT_EVENT_CONNECT_STATE:
		{

		}
		break;

	case FLC_BT_EVENT_A2DP_PLAYSTATE:
		{
			m_bPlayingFlag=(PLAYER_STATE_PLAY == _DataID);
			RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::OnBluetoothMsg] m_bPlayingFlag[%d]\r\n"), m_bPlayingFlag));
		}
		break;

	default:
		break;
	}
	//Do not delete
}

void CPanelEasyConnected::OnPlaySoundEffect(BOOL bPlay)
{
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_EASYCONNECTED)
	{
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(0<<16)+(bPlay?0:A2DP_MAX_VOL));
	}
}

void CPanelEasyConnected::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::OnSystemPowerStatePrepare] idState[%d]\r\n"), idState));
	m_iPowerStatus = idState;

	switch (idState)
	{
	case SYS_POWER_OFF:
		{

		}
		break;
	case SYS_POWER_IDLE:
		{
			if(m_nConnectedStatus == EC_INTERACTIVE_START)
			{
				EasyConnectedShowWindow(FALSE);
			}
		}
		break;
	case SYS_POWER_ON:
		{

		}
		break;
	}

}

void CPanelEasyConnected::OnDeviceChange(LPTSTR lpDevName, BOOL bNew)
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
	// Broadcast the arrival of the Easyconnected in the system
	//

	pdbp->dbcp_devicetype = DBT_DEVTYP_PORT;
	pdbp->dbcp_reserved = 0;
	lpStr = (LPTSTR)&(pdbp->dbcp_name[0]);
	_tcscpy(lpStr, lpDevName);
	pdbp->dbcp_size = dwLen;

	RETAILMSG(1, (TEXT("%s %s,%d size:%d.\r\n"), TEXT(__FUNCTION__),lpDevName, bNew, dwLen));

	if(m_fnSendNotifyMessage)
	{
		m_fnSendNotifyMessage(g_hMainWnd,
				WM_DEVICECHANGE,
				(bNew) ? DBT_DEVICEARRIVAL : DBT_DEVICEREMOVECOMPLETE,
				(LPARAM)pdbp);
	}
	LocalFree(pdbp);
}

//	void CPanelBluetoothA2dp::OnA2dpConnected(BOOL bConnected)
//	{
//		m_bA2dpConnected = bConnected;
//	}

#if 0
void CPanelEasyConnected::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
    if (wParam == MEDIA_TYPE_A2DP)
    {
        m_bA2dpConnected=lParam;
    	RETAILMSG(DEBUG_EASYCONNECTED,(_T("MSG:[CPanelEasyConnected::OnAvailableSourceChanged]: m_bA2dpConnected:%d\r\n"),m_bA2dpConnected));
    }
}
#endif

BOOL CPanelEasyConnected::AutoResume()
{
	BOOL bDone = FALSE;
	if(!m_bAutoResumeFlag)
	{
		return TRUE;
	}

	//Test only
	//RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::AutoResume]: m_bPlayingFlag:%d\r\n"),m_bPlayingFlag));

	//auto resume:
	if(m_bInteractive && GetTickCount()-m_dwEnterTick>500)
	{
		if(m_bA2dpConnected)
		{
			if(!m_bPlayingFlag)
			{
				RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::AutoResume]: A2DP_CONTROL_PLAY\r\n")));
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_ENABLE);
				SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
			}
			else
			{
				RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::AutoResume]: m_bPlayingFlag=TRUE\r\n")));
			}
		}
		else
		{
			RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("MSG:[CPanelEasyConnected::AutoResume]: Do nothing\r\n")));
			//SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_NONE);
		}
		m_bAutoResumeFlag = FALSE;
		bDone = TRUE;
	}
	else
	{
		RETAILMSG(DEBUG_EASYCONNECTED, (TEXT("ERR:[CPanelEasyConnected::AutoResume]: m_bInteractive:%d Tick:%d\r\n"),
			m_bInteractive,GetTickCount()-m_dwEnterTick));
	}

	return bDone;
}
