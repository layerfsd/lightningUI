#include "PanelPhoneLink.h"
#include "../../resource.h"

extern HWND					g_hMainWnd;
extern const wchar_t* g_strInstallPath;

#define PL_HIDDEN_WND_CLASSNAME TEXT("PL:NOTIFY_WND")

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

CPanelPhoneLink::CPanelPhoneLink(void)
{
	m_hwndPhoneLinkNotify = NULL;
	m_hwndClient = NULL;

	m_hPLDevice = NULL;

	memset(&m_data_in,0,sizeof(TRANS_DATA));
	memset(&m_data_out,0,sizeof(TRANS_DATA));

	m_bPhoneLinkVisible = FALSE;

	m_bSpeechOn = FALSE;
	m_bPhoneLinkSrvIni = FALSE;

	m_bConnected = FALSE;
	m_dwEnterTick = 0;

	m_bAutoSwitchToPhoelink = FALSE;
	m_bDeviceInserted = FALSE;

	m_bA2dpOn = FALSE;
}

CPanelPhoneLink::~CPanelPhoneLink(void)
{
	DeiniPhoneLinkSrv();
}
BOOL CPanelPhoneLink::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelPhoneLink::OnSRCChanged(UINT idSRC)
{
	RETAILMSG(DEBUG_PHONELINK,(_T("CPanelPhoneLink::OnSRCChanged %d\r\n"),idSRC));

	if(	GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_EASYCONNECTED
		&& GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_PHONELINK
		&& GET_PANEL_CLASS_ID(idSRC) != UI_CLASS_BT)
	{
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+0);
	}
}

void CPanelPhoneLink::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);


	if(m_bConnected && m_bPhoneLinkVisible)
	{
		PostMsgToClient(ID_UWM_PHONELINK_STOP,0,0);
		m_bPhoneLinkVisible = FALSE;
	}
}
void CPanelPhoneLink::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
 	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

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


// 	//MCU command:
// 	SwitchAudioSRC(SRC_AUDIO_ARM);

	//
	SendBluetoothCommand(BT_CMD_GET_A2DP_STREAM,LPARAM(&m_bA2dpOn));

	RETAILMSG(1/*DEBUG_PHONELINK*/,(_T("............CPanelPhoneLink bA2dpOn :%d ......................\r\n"),m_bA2dpOn));
	if(m_bA2dpOn)
	{
		//SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_ENABLE);
		//SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
		if(SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER)))
		{
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
		}
	}


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

		RETAILMSG(DEBUG_PHONELINK,(_T("............CPanelPhoneLink apiAvrcp.put_Volume :%d ......................\r\n"),dwVol));

		//pull down volume
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+dwVol);
	}
	else
	{
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
	}


	//can bus:
	::PostMessage(m_hMainWnd,WM_NOTIFY_SOURCE_INFO,MEDIA_TYPE_OTHER,0x30);
// 	::PostMessage(m_hMainWnd,WM_NOTIFY_MEDIA_PLAYBACK_INFO,0x0,(0x0<<16)+0x30);

	if(m_bDeviceInserted)
		SetMsg(GetResourceString(IDS_PHONELINK_CONNECTING));
	else if(!m_bConnected)
		SetMsg(GetResourceString(IDS_PHONELINK_MAIN_PROMPT));
	else
		SetMsg(_T(""));


	m_dwEnterTick = GetTickCount();
}

void CPanelPhoneLink::Render()
{
	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;

// 	COLORGL cr_b={0.0,0.0,0.0,m_fCurAlpha};

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

// 	DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr_b);

	m_btnCancel.Show(!m_bConnected);

	if(!m_bConnected)
	{
		m_tagTitle.SetAlpha(m_fCurAlpha);
		m_tagTitle.Render();

		m_iconLink.SetAlpha(m_fCurAlpha);
		m_iconLink.Render();

// 		m_btnCancel.SetAlpha(m_fCurAlpha);
// 		m_btnCancel.Render();
	}

	if(IsAniReady() && GetTickCount()-m_dwEnterTick>500)
	{
		if(!m_bPhoneLinkSrvIni)
		{
			//launch service:
			IniPhoneLinkSrv();
			Sleep(200);
			m_bPhoneLinkSrvIni = TRUE;
		}
		else if(m_bConnected && !m_bPhoneLinkVisible)
		{
			RETAILMSG(DEBUG_PHONELINK,(_T("CPanelPhoneLink show phone link window\r\n")));
			PostMsgToClient(ID_UWM_PHONELINK_START,0,0);
			m_bPhoneLinkVisible = TRUE;
		}
		else if(!m_bConnected && m_bDeviceInserted && GetTickCount()-m_dwEnterTick>30000)
		{
			//connect error!
			//SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			SetMsg(GetResourceString(IDS_PHONELINK_CONNECT_FAILED));
		}
	}

	EndRender();
}
void CPanelPhoneLink::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
BOOL CPanelPhoneLink:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bShowStatusBar = FALSE;

// 	//launch service:
// 	IniPhoneLinkSrv();
// 	Sleep(200);


	m_iconLink.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_128));
	m_iconLink.SetShowScale(1.6f);
	m_iconLink.SetPos(0,0);

	COLORGL cr={1,1,1,1.0};
	COLORGL cr_down={0.465,0.824,0.965,1.0};
	SIZE si0={800,120};

	//cancel
// 	m_btnCancel.Initialize(IDC_PHONELINK_BTN_CANCEL,this,m_pSkinManager->GetTexture(TEXID_043),NULL,
// 		m_pSkinManager->GetTexture(TEXID_044),NULL,
// 		NULL,NULL,
// 		NULL,NULL);
// 	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
// 	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_btnCancel.SetCaptionColor(&cr,&cr_down);
// 	m_btnCancel.SetCaptionHeight(32);
// 	m_btnCancel.LinkTextGL(pTextGL);
// 	m_btnCancel.SetSize(&si0);
// 	m_btnCancel.SetNotifyFunc(& CPanelPhoneLink::OnNotify,this);
// 	m_btnCancel.SetPos(0,-180);

	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
// 	m_tagTitle.SetCaption(GetResourceString(IDS_MHL_MAIN_PROMPT));
	m_tagTitle.SetCaption(GetResourceString(IDS_PHONELINK_CONNECTING));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-160);
	m_tagTitle.SetCaptionHeight(28);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);


	return TRUE;
}
void CPanelPhoneLink::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	if(!lpDeviceName)
		return;

	RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink::OnDeviceArrivedReady :%s\r\n"), lpDeviceName));

	if(wcsstr(lpDeviceName,_T("PLA"))/* || wcsstr(lpDeviceName,_T("LIN"))*/)//
	{
// 		if(UI_PHONELINK != m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)
// 		{
// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_IPODCORE,NULL);
// 		}

		m_bAutoSwitchToPhoelink = FALSE;

		m_bDeviceInserted = TRUE;
	}
}
void CPanelPhoneLink::NotifyConnected(BOOL bConnected)
{
	RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink::NotifyConnected :%d\r\n"), bConnected));

	m_bConnected = bConnected;

	if(bConnected)
	{
		if(m_bAutoSwitchToPhoelink)
		{
			SwitchPage(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER);
			m_bAutoSwitchToPhoelink = FALSE;
		}
	}
	else
	{
		PostMsgToClient(ID_UWM_PHONELINK_STOP,0,0);
		m_bPhoneLinkVisible = FALSE;
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_PHONELINK)
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
	}

	//update widget:
	BOOL bshow=(bConnected);
	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_PHONELINK,0,UIOPRT_SHOWHIDE,&bshow);

}
void CPanelPhoneLink::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink::OnDeviceRemovedReady :%s\r\n"), lpDeviceName));

	if(wcsstr(lpDeviceName,_T("PLA"))/* || wcsstr(lpDeviceName,_T("LIN"))*/)//
	{
		if(m_idSystemPowerState == SYS_POWER_ON &&
			UI_CLASS_PHONELINK == GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC))
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}

		m_bDeviceInserted = FALSE;
	}

}

BOOL CPanelPhoneLink::IniPhoneLinkSrv()
{
	//PL device:
	m_hPLDevice = CreateFile(TEXT("PLN1:"), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, 0);

	if (m_hPLDevice != INVALID_HANDLE_VALUE)
	{
		RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink::open PLN1 succeed\r\n")));
	}
	else
	{
		RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink::open PLN1 FAILED\r\n")));
	}

	/////////////////////////////////
	WNDCLASS    wc;
	TCHAR szWndClassName[] = PL_HIDDEN_WND_CLASSNAME;
	memset(&wc, 0, sizeof(wc));
	wc.lpfnWndProc      = CPanelPhoneLink::PhoneLinkWndProc;
	wc.hInstance        =  m_moduleEntry.hInstance;
	wc.lpszClassName    = szWndClassName;
	wc.hIcon			= NULL;//LoadIcon(IDR_MAINFRAME);
	//wc.hIconSm			= ::LoadIcon(wc.hInstance , MAKEINTRESOURCE(IDR_MAINFRAME));

	//UnregisterClass(wc.lpszClassName, wc.hInstance);
	if (! RegisterClass(&wc))
	{
		RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink::Initialize  RegisterClass ERROR:%d\r\n"), GetLastError()));
		return 0;
	}

	m_hwndPhoneLinkNotify = CreateWindow(szWndClassName, szWndClassName, WS_DISABLED | WS_POPUP,
		//CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, 1, 1,
		NULL/*g_hMainWnd*/, NULL,  wc.hInstance, this);

	ShowWindow(m_hwndPhoneLinkNotify,SW_HIDE);

	//

	LPCTSTR pstrCmdLine =  _T("SHIYUAN374SKG92S");

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("PhoneLink\\HMI\\PhoneLinkClient.exe"));

	//start APP
	SHELLEXECUTEINFO ShExeInfo={0};
	ShExeInfo.cbSize=sizeof(SHELLEXECUTEINFO);
	ShExeInfo.fMask=SEE_MASK_NOCLOSEPROCESS;
	ShExeInfo.hwnd=NULL;
	ShExeInfo.lpVerb=NULL;
// 	ShExeInfo.lpFile = _T("\\Windows\\PhoneLinkClient.exe");
	ShExeInfo.lpFile = strFile.String();
	ShExeInfo.lpParameters=pstrCmdLine;
	ShExeInfo.lpDirectory=NULL;
	ShExeInfo.nShow=SW_SHOW;
	ShExeInfo.hInstApp=NULL;

	if(!ShellExecuteEx(&ShExeInfo))
	{
		RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink:: ShellExecuteEx ERROR:%s\r\n"),ShExeInfo.lpFile));
	}
	else
	{
		RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink:: ShellExecuteEx SUCCEED:%s\r\n"),ShExeInfo.lpFile));
	}

	return TRUE;
}
void CPanelPhoneLink::DeiniPhoneLinkSrv()
{
	if(m_hwndPhoneLinkNotify != NULL)
	{
		DestroyWindow(m_hwndPhoneLinkNotify);
		UnregisterClass(PL_HIDDEN_WND_CLASSNAME, m_moduleEntry.hInstance);
		m_hwndPhoneLinkNotify = NULL;
	}
}

void CPanelPhoneLink::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_MHL_BTN_HOME:
		break;
	case IDC_PHONELINK_BTN_CANCEL:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	}
}

void CPanelPhoneLink::OnBack()
{
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
}
void CPanelPhoneLink::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);

}


void CPanelPhoneLink::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

}
void CPanelPhoneLink::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);
}
void CPanelPhoneLink::OnPhoneLinkMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == ID_UWM_PHONELINK_CONNECTED)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_CONNECTED \r\n")));

		NotifyConnected(TRUE);
	}
	else if(message == ID_UWM_PHONELINK_DISCONNECT)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_DISCONNECT \r\n")));

		NotifyConnected(FALSE);
	}
	else if(message == ID_UWM_PHONELINK_EXIT)
	{
		PostMsgToClient(ID_UWM_PHONELINK_STOP,0,0);
		m_bPhoneLinkVisible = FALSE;
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
	}
	else if(message == ID_UWM_PHONELINK_HAND_CLIENT)
	{
		m_hwndClient = (HWND)wParam;
		::PostMessage(HWND_BROADCAST, ID_UWM_PHONELINK_HAND_MAIN,(WPARAM)m_hwndPhoneLinkNotify,0);
	}
	else if(message == WM_COPYDATA)
	{
		COPYDATASTRUCT *pCopyDataStruct=(COPYDATASTRUCT *)lParam;
		//RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -WM_COPYDATA dwData:0x%x,size:%d\r\n"),pCopyDataStruct->dwData,pCopyDataStruct->cbData));
		if(pCopyDataStruct->cbData == sizeof(TRANS_DATA))
		{
			memcpy(&m_data_in,pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		}
	}
	else if(message == ID_UWM_PHONELINK_CLIENT_TRANS)
	{
		//RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_CLIENT_TRANS \r\n")));
		if(TransDataProcess())
		{
			SendMsg();
			PostMsgToClient(ID_UWM_PHONELINK_MAIN_TRANS,0,0);
		}
	}
	else if(message == ID_UWM_PHONELINK_INSTALL)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_INSTALL \r\n")));
		//display install UI here
		SetMsg(GetResourceString(IDS_PHONELINK_INSTALLING));
	}
	else if(message == ID_UWM_PHONELINK_INSTALL_OK)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_INSTALL_OK \r\n")));
		//display install success UI here
		SetMsg(GetResourceString(IDS_PHONELINK_INSTALL_OK));
	}
	else if(message == ID_UWM_PHONELINK_CONNECTING)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_CONNECTING \r\n")));
		//display connecting UI here
		SetMsg(GetResourceString(IDS_PHONELINK_CONNECTING));
	}
	else if(message == ID_UWM_PHONELINK_RESET)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_RESET \r\n")));
		//let the IC power off here, power off  time 100ms
		Sleep(100);
		//after reset, post reset ok message
		PostMsgToClient(ID_UWM_PHONELINK_RESET_OK,0,0);
	}
	else if(message == ID_UWM_PHONELINK_NO_LICENSE)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_NO_LICENSE \r\n")));
	}
	else if(message == ID_UWM_PHONELINK_NO_USB_DEBUG)
	{
		RETAILMSG(DEBUG_PHONELINK, (_T("**CPanelPhoneLink::PhoneLinkWndProc -ID_UWM_PHONELINK_NO_USB_DEBUG \r\n")));

		SetMsg(GetResourceString(IDS_PHONELINK_NO_USB_DEBUG));
	}
}
void CPanelPhoneLink::PostMsgToClient(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_hwndClient != NULL)
	{
		::PostMessage(m_hwndClient, message,wParam,lParam);
	}
}
void CPanelPhoneLink::SendMsg()
{
	if(m_hwndClient != NULL)
	{
		COPYDATASTRUCT cpd;
		cpd.dwData=0;
		cpd.cbData = sizeof(TRANS_DATA);
		cpd.lpData = &m_data_out;

		//
		::SendMessage(m_hwndClient, WM_COPYDATA, NULL,(LPARAM)&cpd);
	}
}
BOOL CPanelPhoneLink::PLDeviceRead(TRANS_DATA *data)
{
	DWORD dwReturn=0;
	if(m_hPLDevice == NULL || m_hPLDevice == INVALID_HANDLE_VALUE)
		return FALSE;

	if(!data)
		return FALSE;

	//RETAILMSG(DEBUG_PHONELINK, (_T("**PLDeviceRead deviceAddr:%x, subAddr:%x \r\n"),data->device_addr,data->sub_addr));

	if (!DeviceIoControl(m_hPLDevice, IOCTL_PLN_READ, NULL, 0, data, sizeof(TRANS_DATA), &dwReturn, NULL))
	{
		RETAILMSG(DEBUG_PHONELINK, (TEXT("Failed DeviceIoControl IOCTL_PLN_READ -- error = 0x%x\r\n"), GetLastError()));
		return FALSE;
	}

	return TRUE;
}
BOOL CPanelPhoneLink::PLDeviceWrite(TRANS_DATA *data)
{
	DWORD dwReturn=0;

	if(m_hPLDevice == NULL || m_hPLDevice == INVALID_HANDLE_VALUE)
		return FALSE;

	if(!data)
		return FALSE;

// 	RETAILMSG(DEBUG_PHONELINK, (_T("**PLDeviceWrite deviceAddr:%x, subAddr:%x \r\n"),data->device_addr,data->sub_addr));

	if (!DeviceIoControl(m_hPLDevice, IOCTL_PLN_WRITE, NULL, 0, data, sizeof(TRANS_DATA), &dwReturn, NULL))
	{
		RETAILMSG(DEBUG_PHONELINK, (TEXT("Failed DeviceIoControl IOCTL_PLN_WRITE -- error = 0x%x\r\n"), GetLastError()));
		return FALSE;
	}

	return TRUE;
}
BOOL CPanelPhoneLink::TransDataProcess()
{
	BOOL ret = TRUE;

	if(m_data_in.type == TRANS_READ)
	{
// 		I2C.i2c_read(m_data_in.device_addr + 1, m_data_in.sub_addr, m_data_out.buf, m_data_in.count);
		ret = PLDeviceRead(&m_data_in);
		m_data_out.type = TRANS_READ_FEEDBACK;
		m_data_out.state = TRANS_STATE_SUCCESS;
		m_data_out.count = m_data_in.count;
		m_data_out.device_addr = m_data_in.device_addr;
		m_data_out.sub_addr = m_data_in.sub_addr;
		//copy data:
		memcpy(m_data_out.buf,m_data_in.buf,16);
	}
	else if(m_data_in.type == TRANS_WRITE)
	{
// 		I2C.i2c_write(m_data_in.device_addr, m_data_in.sub_addr, m_data_in.buf, m_data_in.count);
		ret = PLDeviceWrite(&m_data_in);
		m_data_out.type = TRANS_WRITE_FEEDBACK;
		m_data_out.state = TRANS_STATE_SUCCESS;
		m_data_out.count = m_data_in.count;
		m_data_out.device_addr = m_data_in.device_addr;
		m_data_out.sub_addr = m_data_in.sub_addr;
	}
	else
	{
		ret = FALSE;
	}

	return ret;
}


BOOL CPanelPhoneLink::OnKey(UINT idKey)
{
	switch (idKey)
	{
	case KEY_MCU_SEEK_PREV:
		break;
	case KEY_MCU_SEEK_NEXT:
		break;
	case KEY_MCU_ENTER:
		break;

	case KEY_MEDIA_PLAY:
		{
			SwitchAudioSRC(SRC_AUDIO_ARM);
			SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
		}
		break;
	}

	return TRUE;

}
void CALLBACK CPanelPhoneLink::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_PHONELINK,(_T("CPanelPhoneLink OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelPhoneLink *panel=(CPanelPhoneLink*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
LRESULT CALLBACK CPanelPhoneLink::PhoneLinkWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CPanelPhoneLink *panel=NULL;

	if(message == WM_CREATE)
	{
		SetCursor(NULL);
		ShowCursor(FALSE);
		panel = (CPanelPhoneLink *)(((CREATESTRUCT*)lParam)->lpCreateParams);
	}
	else
	{
		if(panel)
		{
			panel->OnPhoneLinkMsg(message,wParam,lParam);
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}

void CPanelPhoneLink::OnSpeechStart()
{
	RETAILMSG(DEBUG_PHONELINK,(_T("............CPanelPhoneLink OnSpeechStart......................\r\n")));

	m_bSpeechOn = TRUE;

//#if !USE_WAV_ONCHIP

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_PHONELINK)
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

			RETAILMSG(DEBUG_PHONELINK,(_T("............CPanelPhoneLink apiAvrcp.put_Volume :%d ......................\r\n"),dwVol));

			//pull down volume
			SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+dwVol);
		}
	}
//#endif
}
void CPanelPhoneLink::OnSpeechTerminate()
{
	RETAILMSG(DEBUG_PHONELINK,(_T("............CPanelPhoneLink OnSpeechTerminate......................\r\n")));

	m_bSpeechOn = FALSE;

//#if !USE_WAV_ONCHIP

	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_PHONELINK)
	{
		//pull up volume
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(1<<16)+A2DP_MAX_VOL);
	}
//#endif
}
void CPanelPhoneLink::OnPlaySoundEffect(BOOL bPlay)
{
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_PHONELINK)
	{
		SendBluetoothCommand(BT_CMD_A2DP_VOLUME,(0<<16)+(bPlay?0:A2DP_MAX_VOL));
	}
}

void CPanelPhoneLink::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(DEBUG_PHONELINK, (TEXT("CPanelPhoneLink::OnSystemPowerStatePrepare :%d\r\n"), idState));

	switch (idState)
	{
	case SYS_POWER_OFF:
		{

		}
		break;
	case SYS_POWER_IDLE:
		{
			if(m_bConnected)
			{
				m_bPhoneLinkVisible = FALSE;
				PostMsgToClient(ID_UWM_PHONELINK_STOP,0,0);
			}
		}
		break;
	case SYS_POWER_ON:
		{

		}
		break;
	}

}
