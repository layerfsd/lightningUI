#include "PanelAndroidAuto.h"
#include "../../resource.h"
#if CVTE_EN_KEY_PROXY
#include "../Key/CKeyHandler.h"
#endif
#include "../Settings/SettingCommon.h"
#include "Asplib.h"
#include "ModulVer.h"

#if CVTE_EN_ANDROID_AUTO
extern HWND					g_hMainWnd;
extern const wchar_t* g_strInstallPath;

const wchar_t * CPanelAndroidAuto::m_DestCAPath1 = _T("\\StaticStore");
const wchar_t * CPanelAndroidAuto::m_DestCAPath2 = _T("\\ResidentFlash");

CPanelAndroidAuto::CPanelAndroidAuto(void)
:CMiniThread( 0, TRUE )
{
	m_pAndroidAuto = NULL;
	m_pBTPanel = NULL;
	m_bAndroidAutoConnected = FALSE;
	m_idMediaClassBeforeAA = -1;
	m_bSrcPlayingBeforeAA = FALSE;
	m_audioRequest = AUDIO_FOCUS_RELEASE;
	m_audioRequestBeforeIdle = AUDIO_FOCUS_RELEASE;
	m_audioRequestBeforePhonecall = AUDIO_FOCUS_RELEASE;
	m_bVideoRequested = false;
	m_timerEnterVoiceMode = 0;
	m_timerQuitVoiceMode = 0;
	m_timerEnterAASourceMode = 0;
	m_timerQuitAASourceMode = 0;
	m_timerKillAAPVideoFocus = 0;
	m_timerFirstConnection = 0;
	m_timerAutoShowAAPTel = 0;
	m_bRdsTAon = FALSE;
	m_bAndroidAutoVoiceOn = FALSE;
	m_strPhoneBluetoothAddr[0] = 0;
	m_bAudioStreamGuidance = FALSE;
	m_bAudioStreamMedia = FALSE;
	m_bVoiceOn = FALSE;
	m_bAABorrowAudio = FALSE;
	m_bPhoneCallActive = FALSE;
	m_bOnNightMode = FALSE;
	m_bScreenVideoOn = FALSE;
	m_bNeedShowPrompt = TRUE;
	m_dwEnterTick = 0;
	memset(m_strDeviceName, 0, sizeof(m_strDeviceName));
}

CPanelAndroidAuto::~CPanelAndroidAuto(void)
{
	if (m_pAndroidAuto != NULL)
	{
		delete m_pAndroidAuto;
	}
}
BOOL CPanelAndroidAuto::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelAndroidAuto::OnSRCChanged(UINT idSRC)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("CPanelAndroidAuto::OnSRCChanged 0x%x\r\n"),idSRC));

	if(GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_ANDROID_AUTO)
	{
		m_bAABorrowAudio = TRUE;
	}
	else
	{
		//reset para:
		m_bAABorrowAudio = FALSE;
		m_bAudioStreamMedia = FALSE;
	}

	if(m_pAndroidAuto && m_bAndroidAutoConnected)
	{
		if(GET_PANEL_CLASS_ID(idSRC) == UI_CLASS_ANDROID_AUTO)
		{
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN, true);
		}
		else
		{
			if(m_bRdsTAon)
			{
				m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_LOSS_TRANSIENT, true);
			}
			else
			{
				m_audioRequest = AUDIO_FOCUS_RELEASE;
				//update previous media class:
				if(m_idMediaClassBeforeAA != -1)
				{
					m_idMediaClassBeforeAA = idSRC;
				}
				m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_LOSS, true);
			}
		}	
	}
}

void CPanelAndroidAuto::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	if(m_pAndroidAuto && m_bAndroidAutoConnected)
	{
		m_pAndroidAuto->setVideoFocus(VIDEO_FOCUS_NATIVE,false);
	}
}
void CPanelAndroidAuto::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

	//MCU command:
	//SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);

	m_dwEnterTick = GetTickCount();
	m_bVideoRequested = false;
	m_bNeedShowPrompt = TRUE;

// 	if(m_pAndroidAuto && m_bAndroidAutoConnected)
// 	{
// 		m_pAndroidAuto->setVideoFocus(VIDEO_FOCUS_PROJECTED,false);
// 	}
}

void CPanelAndroidAuto::Render()
{
	if(!BeginRender())return;

	if(GetTickCount()-m_dwEnterTick<1500)
	{
		m_iconLink.SetAlpha(m_fCurAlpha);
		m_iconLink.Render();
	}

	if(m_idAni==ANIM_NONE && GetTickCount()-m_dwEnterTick>100)
	{
		if(m_pAndroidAuto && m_bAndroidAutoConnected && !m_bVideoRequested)
		{
			RETAILMSG(DEBUG_ANDROIDAUTO,(_T("setVideoFocus: VIDEO_FOCUS_PROJECTED 1\r\n")));
			m_bVideoRequested = true;
			m_pAndroidAuto->setVideoFocus(VIDEO_FOCUS_PROJECTED,false);
		}
	}
	if( m_idAni!=ANIM_NONE && (/*IsReady()||*/GetTickCount()-m_dwEnterTick>400) && m_bPanelEntered && !m_bVideoRequested)
	{
		m_bVideoRequested = true;
		if(m_pAndroidAuto && m_bAndroidAutoConnected)
		{
			RETAILMSG(DEBUG_ANDROIDAUTO,(_T("setVideoFocus: VIDEO_FOCUS_PROJECTED 2\r\n")));
			m_pAndroidAuto->setVideoFocus(VIDEO_FOCUS_PROJECTED,false);
		}
	}
	//show prompt:
	if(m_bNeedShowPrompt && m_pTextGL && GetTickCount()-m_dwEnterTick>1500)
	{
		COLORGL cr={1,1,1,1};
		SIZE si;

		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);
		m_pTextGL->SetColor(&cr,FALSE);
		m_pTextGL->SetCharScale(1.0f,1.0f);

		m_pTextGL->SetString(GetResourceString(IDS_AA_NOTE1));
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(-GLfloat(si.cx/2),100);
		m_pTextGL->Render();

		m_pTextGL->SetString(GetResourceString(IDS_AA_NOTE2));
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(-GLfloat(si.cx/2),54);
		m_pTextGL->Render();

		m_pTextGL->SetString(GetResourceString(IDS_AA_NOTE3));
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(-GLfloat(si.cx/2),8);
		m_pTextGL->Render();
	}
	EndRender();
}
void CPanelAndroidAuto::OnGestureMsg(UINT idGesture)
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
void CPanelAndroidAuto::OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam)
{
	m_bRdsTAon = wParam;
}
void CPanelAndroidAuto::OnNativeNaviMsg(UINT iNaviCode, LPARAM lParam)
{
	if(!m_pAndroidAuto)
		return;
	if(!m_bAndroidAutoConnected)
		return;
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("CPanelAndroidAuto::OnNativeNaviMsg 0x%x, 0x%x\r\n"),iNaviCode,lParam));
	if(iNaviCode == NAVI_START_MSG)
	{
		switch (iNaviCode)
		{
		case 0://closed or launch failed:
			break;
		case 1://map on screen:
			m_pAndroidAuto->setNaviFocus(NAV_FOCUS_NATIVE);
			break;
		case 2://hide map:
			break;
		default:
			break;
		}
	}
	else if(iNaviCode == NAVI_GUIDANCE_MSG)
	{
		BOOL bDuck = HIWORD(lParam);
		BOOL bActive = LOWORD(lParam);
		if(bActive)
		{
			if(bDuck)
				m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN_MEDIA_ONLY,false);
			else
				m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_LOSS_TRANSIENT,false);
		}
		else
		{
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN,false);
		}
	}
}
BOOL CPanelAndroidAuto::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry,DeviceActivateCallback_f callback, PVOID context)
{
	BOOL bRet = FALSE;
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bShowStatusBar = FALSE;

	m_DeviceActivateCallback = callback;
	m_DeviceActivateCallbackContext = context;

	m_iconLink.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_186));
	m_iconLink.SetShowScale(1.25f);
	m_iconLink.SetPos(0,0);

	CModuleVersion ver;

	if (ver.GetFileVersionInfo(_T("AndroidAutoAPI.dll"))) {
		// display file version from VS_FIXEDFILEINFO struct
		if ( (HIWORD(ver.dwProductVersionMS) != AndroidAutoAPIMajorVersion)
			||	 (LOWORD(ver.dwProductVersionMS) != AndroidAutoAPIMinorVersion)
			||	 (HIWORD(ver.dwProductVersionLS) != AndroidAutoAPIRevisionVersion)
			||	 (LOWORD(ver.dwProductVersionLS) != AndroidAutoAPIBuildVersion) )
		{
			//---------------0----0----1----1----2----2----3----3----4----4----5----5----6----6---
			//---------------0----5----0----5----0----5----0----5----0----5----0----5----0----5---
			RETAILMSG(1, (_T("\r\n")));
			RETAILMSG(1, (_T("############################################################\r\n")));
			RETAILMSG(1, (_T("##### Android Auto API Dll version isn't match!        #####\r\n")));
			RETAILMSG(1, (_T("############################################################\r\n")));
			RETAILMSG(1, (_T("\r\n")));
			return FALSE;
		}
		else
		{

			//---------------0----0----1----1----2----2----3----3----4----4----5----5----6----6---
			//---------------0----5----0----5----0----5----0----5----0----5----0----5----0----5---
			RETAILMSG(1, (_T("\r\n")));
			RETAILMSG(1, (_T("############################################################\r\n")));
			RETAILMSG(1, (_T("##### Android Auto API DLL Version: %-19s#####\r\n"), ver.GetValue(_T("ProductVersion")).c_str()));
			RETAILMSG(1, (_T("############################################################\r\n")));
			RETAILMSG(1, (_T("\r\n")));
		}
	}

	if (m_pAndroidAuto == NULL)
	{
		m_pAndroidAuto = new CAndroidAutoAPI();

		m_pAndroidAuto->GetDeviceID(m_strDeviceName, MAX_PATH);

		bRet = ThreadStart();
	}

	return bRet;
}

wchar_t* CPanelAndroidAuto::GetDeviceName()
{
    return m_strDeviceName;
}

BOOL CPanelAndroidAuto::ActiveOnce(LPCTSTR strDestCA)
{
	BOOL bRet = FALSE;
	unsigned char *ca_input;
	FILE* fp;
	DWORD err = 0;
	int iRet;

	if (IsFileExists(strDestCA))
	{
		if(0 == _wfopen_s(&fp,strDestCA, L"rb"))
		{
			size_t fileSize=0;
			fseek (fp , 0 , SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			fileSize = min(2560,fileSize);
			ca_input =new unsigned char[fileSize];
			size_t readsize = fread(ca_input,fileSize,1,fp);
			fclose(fp);
			bRet = m_pAndroidAuto->InstallCA(ca_input, fileSize);
			if (!bRet)
			{
				RETAILMSG(1,(_T("\n####ActiveOnce Install CA file Failed! %s##### \n"),strDestCA));
			}
			else
			{
				bRet = m_pAndroidAuto->IsActivated();
				RETAILMSG(1,(_T("\n####CA Is Activated = %d!##### \n"),bRet));
			}
			delete ca_input;
		}
	}
	else
	{
		RETAILMSG(1,(_T("\n####%s CA File isn't exists!##### \n"),strDestCA));
	}
	return bRet;
}

BOOL CPanelAndroidAuto::StartClient()
{
	BOOL bRet = FALSE;
	int32_t pix_ratio=10000*CVTE_DEF_SCREEN_PHYSICALWIDTH/CVTE_DEF_SCREEN_PHYSICALHEIGHT*g_iScreenHeight/g_iScreenWidth;

#if (CUSTOMER_ID == CUSTOMER_CVTE_CERTIFICATION)
	m_pAndroidAuto->setIdentityInfo("CVTE", "U702", "2016", getId().c_str());
	m_pAndroidAuto->setHeadUnitInfo("CVTE", "U702", CVTE_DEF_APP_VERSION_STRING, CVTE_DEF_APP_VERSION_STRING);
#else
	m_pAndroidAuto->setIdentityInfo("CAR", "U702", "2016", getId().c_str());
	m_pAndroidAuto->setHeadUnitInfo("CAR", "U702", CVTE_DEF_APP_VERSION_STRING, CVTE_DEF_APP_VERSION_STRING);
#endif
	m_pAndroidAuto->setDriverPosition(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType?DRIVER_POSITION_RIGHT:DRIVER_POSITION_LEFT);
	m_pAndroidAuto->setVideoParameters(CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH,CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT,160,pix_ratio,VIDEO_FPS_30);

	m_pAndroidAuto->setGPSPort(CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT);

	KeyCode keys[] = {
		KEYCODE_SEARCH,
		KEYCODE_MEDIA_PLAY,KEYCODE_MEDIA_PAUSE,
		KEYCODE_MEDIA_NEXT,KEYCODE_MEDIA_PREVIOUS,
		KEYCODE_CALL,KEYCODE_ENDCALL/*,KEYCODE_TEL*/ };
	m_pAndroidAuto->addKeycode(keys, _countof(keys));

	m_pAndroidAuto->addAudioIDs(AUDIO_STREAM_GUIDANCE,CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT);
	m_pAndroidAuto->addAudioIDs(AUDIO_STREAM_SYSTEM_AUDIO,CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT);
	m_pAndroidAuto->addAudioIDs(AUDIO_STREAM_MEDIA,CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA);
	m_pAndroidAuto->addAudioIDs(AUDIO_STREAM_TELEPHONY,CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA);

	m_pAndroidAuto->setAudioSourceID(CVTE_DEF_AUDIO_DEVICE_ID_INPUT);

	CReg reg;
	string str_mac;
	reg.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Bluetooth\\hci\\"));
	LPCTSTR wmac=reg.ValueSZ(_T("mac"));
	if(wmac)
	{
		char nmac[20]={0};
		int cret = WideCharToMultiByte( CP_ACP, 0, wmac, -1, nmac, _countof(nmac), NULL, NULL );
		if ( cret > 0 )
		{
			printf("++Get device bluetooth mac: %s",nmac);
			str_mac=nmac;
		}
	}
	if (str_mac.size() != 0)
	{
		m_pAndroidAuto->setBluetoothAddress(str_mac);
	}

	bRet = m_pAndroidAuto->init(this, _T("\\Windows"));
// 	bRet = m_pAndroidAuto->init(this, _T("\\ResidentFlash\\AndroidAuto"));

	return bRet;
}

BOOL CPanelAndroidAuto::IsPlaying()
{
	return m_bAudioStreamMedia;
}

BOOL CPanelAndroidAuto::IsFileExists(LPCTSTR   lpszFileName)  
{  
	WIN32_FIND_DATA   wfd;  
	BOOL   bRet;  
	HANDLE   hFind;  

	if(!lpszFileName)
		return FALSE;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);  
	bRet   =   hFind   !=   INVALID_HANDLE_VALUE;  
	FindClose(hFind);
	return   bRet;
}

DWORD CPanelAndroidAuto::ThreadRun( )
{
	wchar_t strDestCA[MAX_PATH];
	wchar_t strDestCA2[MAX_PATH];
	wchar_t strSrcCA[MAX_PATH];
	wchar_t strSrcCA2[MAX_PATH];
	int iRet;
	DWORD err = 0;

	iRet = swprintf_s(strDestCA,MAX_PATH,_T("%s\\%s.ca"), m_DestCAPath1, m_strDeviceName);
	if (iRet == -1)
	{
		printf("Format destination CA path failed!\n");
		err = GetLastError();
		goto exit;
	}
	iRet = swprintf_s(strDestCA2,MAX_PATH,_T("%s\\%s.ca"), m_DestCAPath2, m_strDeviceName);
	if (iRet == -1)
	{
		printf("Format destination CA path failed!\n");
		err = GetLastError();
		goto exit;
	}
	
	iRet = swprintf_s(strSrcCA,MAX_PATH,_T("\\USB Disk\\%s.ca"),m_strDeviceName);
	if (iRet == -1)
	{
		printf("Format srouce CA path failed!\n");
		err = GetLastError();
		goto exit;
	}
	iRet = swprintf_s(strSrcCA2,MAX_PATH,_T("\\GPS CARD\\%s.ca"),m_strDeviceName);
	if (iRet == -1)
	{
		printf("Format srouce CA2 path failed!\n");
		err = GetLastError();
		goto exit;
	}
	
	while ( 1 )
	{
		if (ActiveOnce(strDestCA))
		{
			if (!IsFileExists(strDestCA2))
			{
				CopyFile(strDestCA, strDestCA2, FALSE);
				SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_HIDDEN);
			}
			break;
		}
		if (ActiveOnce(strDestCA2))
		{
			CopyFile(strDestCA2, strDestCA, FALSE);
			SetFileAttributes(strDestCA,FILE_ATTRIBUTE_HIDDEN);
			break;
		}
		if (m_DeviceActivateCallback)
		{
			m_DeviceActivateCallback(m_DeviceActivateCallbackContext, FALSE);
		}
		if (IsFileExists(strSrcCA))
		{
			RETAILMSG(1, (_T("%s is Exists!\n"), strSrcCA));
			if (IsFileExists(strDestCA))
			{
				SetFileAttributes(strDestCA,FILE_ATTRIBUTE_NORMAL);
			}
			if (!CopyFile(strSrcCA, strDestCA, FALSE))
			{
				RETAILMSG(1, (_T("Copy %s to %s Failed! err = %d\n"), strSrcCA, strDestCA, GetLastError()));
			}
			else
			{
				SetFileAttributes(strDestCA,FILE_ATTRIBUTE_HIDDEN);
			}
			if (IsFileExists(strDestCA2))
			{
				SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_NORMAL);
			}
			if (!CopyFile(strSrcCA, strDestCA2, FALSE))
			{
				RETAILMSG(1, (_T("Copy %s to %s Failed! err = %d\n"), strSrcCA, strDestCA2, GetLastError()));
			}
			else
			{
				SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_HIDDEN);
			}
		}
		if (IsFileExists(strSrcCA2))
		{
			RETAILMSG(1, (_T("%s is Exists!\n"), strSrcCA2));
			if (IsFileExists(strDestCA))
			{
				SetFileAttributes(strDestCA,FILE_ATTRIBUTE_NORMAL);
			}
			if (!CopyFile(strSrcCA2, strDestCA, FALSE))
			{
				RETAILMSG(1, (_T("Copy %s to %s Failed! err = %d\n"), strSrcCA2, strDestCA, GetLastError()));
			}
			else
			{
				SetFileAttributes(strDestCA,FILE_ATTRIBUTE_HIDDEN);
			}
			if (IsFileExists(strDestCA2))
			{
				SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_NORMAL);
			}
			if (!CopyFile(strSrcCA2, strDestCA2, FALSE))
			{
				RETAILMSG(1, (_T("Copy %s to %s Failed! err = %d\n"), strSrcCA2, strDestCA2, GetLastError()));
			}
			else
			{
				SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_HIDDEN);
			}
		}
		Sleep(1000);
	}
	
	if (m_DeviceActivateCallback)
	{
		m_DeviceActivateCallback(m_DeviceActivateCallbackContext, TRUE);
	}
	StartClient();

exit:
	return err;
}

void CPanelAndroidAuto::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_PHONELINK_BTN_CANCEL:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	}
}
BOOL CPanelAndroidAuto::IsEnableQuit()
{
	if(m_bAndroidAutoVoiceOn)
		m_bEnableQuit = m_bAndroidAutoConnected ? FALSE : TRUE;
	else
		m_bEnableQuit = (m_bAndroidAutoConnected ? GetTickCount()-m_dwEnterTick > 2000 : TRUE);
	return m_bEnableQuit;
}

void CPanelAndroidAuto::OnBack()
{
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
}
void CPanelAndroidAuto::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);

}


void CPanelAndroidAuto::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

}
void CPanelAndroidAuto::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);
}

BOOL CPanelAndroidAuto::OnKey(UINT idKey,UINT idStatus)
{
	if(!m_pAndroidAuto)
		return FALSE;
	if(!m_bAndroidAutoConnected)
		return FALSE;
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("==CPanelAndroidAuto OnKey idKey=%d, idStatus=%d\r\n"),idKey,idStatus));

	switch (idKey)
	{
	case KEY_SPEECH:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_SEARCH,true);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_SEARCH,false);
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_SEARCH,false);
		}
		break;
	case KEY_PLAY:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_PLAY,true);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_PLAY,false);
		}
		break;
	case KEY_PAUSE:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_PAUSE,true);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_PAUSE,false);
		}
		break;
	case KEY_NEXT:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_NEXT,true);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_NEXT,false);
		}
		break;
	case KEY_PREVIOUS:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_PREVIOUS,true);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_MEDIA_PREVIOUS,false);
		}
		break;
	case KEY_DROP:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_ENDCALL,true);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_ENDCALL,false);
		}
		break;
	case KEY_HOOK_SWITCH:
		if ( idStatus == EV_KEY_DOWN )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_CALL,true);
		}
		else if ( idStatus == EV_KEY_UP )
		{
			m_pAndroidAuto->onKeyEvent(KEYCODE_CALL,false);
		}
		break;
	}

	return TRUE;

}
void CALLBACK CPanelAndroidAuto::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("CPanelAndroidAuto OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelAndroidAuto *panel=(CPanelAndroidAuto*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelAndroidAuto::OnSpeechStart()
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("...CPanelAndroidAuto OnSpeechStart...\r\n")));

	m_bSpeechOn = TRUE;

}
void CPanelAndroidAuto::OnSpeechTerminate()
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("...CPanelAndroidAuto OnSpeechTerminate...\r\n")));

	m_bSpeechOn = FALSE;

}
void CPanelAndroidAuto::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	RETAILMSG(DEBUG_ANDROIDAUTO, (TEXT("CPanelAndroidAuto::OnSystemPowerStatePrepare :%d\r\n"), idState));

	switch (idState)
	{
	case SYS_POWER_OFF:
		{

		}
		break;
	case SYS_POWER_IDLE:
		{
			m_audioRequestBeforeIdle = m_audioRequest;
			if(m_bAndroidAutoConnected && m_pAndroidAuto && m_audioRequest == AUDIO_FOCUS_GAIN)
			{
				m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_LOSS_TRANSIENT, false);
			}
		}
		break;
	case SYS_POWER_ON:
		{
			if(m_bAndroidAutoConnected && m_pAndroidAuto && m_audioRequestBeforeIdle == AUDIO_FOCUS_GAIN)
			{
				RETAILMSG(DEBUG_ANDROIDAUTO, (TEXT("CPanelAndroidAuto::OnSystemPowerStatePrepare :%d,AUDIO_FOCUS_STATE_GAIN\r\n"), idState));

				m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN, true);
			}
		}
		break;
	}

}

void CPanelAndroidAuto::connectCallback(bool connect)
{
	BOOL bNeedShowTopBanner = FALSE;
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]+++, connect = %d\r\n"),_T(__FUNCTION__), connect));
	m_bAndroidAutoConnected = connect;
	::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_ANDROID_AUTO,connect);
	if(m_pBTPanel)
		m_pBTPanel->OnAndriodAutoConnectStatus(connect);
	if(connect)
	{
		//set microphone gain:
#if (CVTE_DEF_AUDIO_DEVICE_ID_INPUT == 1)
		m_pAndroidAuto->setMicGain(6);
#else
		m_pAndroidAuto->setMicGain(10);
#endif
		m_pAndroidAuto->setDriverPosition(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType?DRIVER_POSITION_RIGHT:DRIVER_POSITION_LEFT);
		m_pAndroidAuto->reportParkingBrakeData(m_bOnBrake);
		m_pAndroidAuto->reportNightModeData(m_bOnNightMode);
		if (m_bOnBrake)
		{
			m_pAndroidAuto->reportDrivingStatusData(DRIVE_STATUS_UNRESTRICTED);
		}
		else
		{
			m_pAndroidAuto->reportDrivingStatusData(DRIVE_STATUS_NO_VIDEO|DRIVE_STATUS_NO_KEYBOARD_INPUT|DRIVE_STATUS_NO_VOICE_INPUT|DRIVE_STATUS_NO_CONFIG|DRIVE_STATUS_LIMIT_MESSAGE_LEN);
		}
		if(m_pSkinManager->GetConfig()->GetCurParam()->bAAPAutoLaunch)
		{
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_ANDROID_AUTO)
			{
				SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_NONE);
			}
		}
		else
		{
			bNeedShowTopBanner = TRUE;
		}

		//start a timer to determine if the connect is a first connection:
		if(m_timerFirstConnection != 0)
		{
			timeKillEvent(m_timerFirstConnection);
			m_timerFirstConnection = 0;
		}
		m_timerFirstConnection = timeSetEvent( 1500, 1, FirstConnectionTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
	}
	else
	{
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_ANDROID_AUTO)
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
		//reset status:
		if(m_bVoiceOn)
		{
			m_bAudioStreamGuidance = FALSE;
			m_bAndroidAutoVoiceOn = FALSE;
			QuitVoiceMode();
		}
		m_bScreenVideoOn = FALSE;

		//reset source if current is AAP:
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_ANDROID_AUTO)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC = 0;
		}
	}

	//notify gui:
	::PostMessage(m_hMainWnd,WM_ANDROID_AUTO_MSG,ANDROIDAUTO_CONNECT_STATUS, (bNeedShowTopBanner<<16) + connect);
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::audioFocusRequestCallback(AudioFocusRequestType request)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]+++, request = %d\r\n"),_T(__FUNCTION__), request));
	m_audioRequest = request;
	switch (request) 
	{
		case AUDIO_FOCUS_GAIN:
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN, false);
			SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE,20,FALSE);
			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_ANDROID_AUTO)
			{
				SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_NONE);
			}
			break;
		case AUDIO_FOCUS_GAIN_TRANSIENT:
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN_TRANSIENT, false);
			PrepareEnterAASourceMode();
			break;
		case AUDIO_FOCUS_GAIN_TRANSIENT_MAY_DUCK:
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN_TRANSIENT_GUIDANCE_ONLY, false);
			PrepareEnterAASourceMode();
			break;
		case AUDIO_FOCUS_RELEASE:
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_LOSS, false);
			PrepareQuitAASourceMode();
			break;
		default:
			printf("Unknown audio focus request type. Ignoring.\n");
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], wParam=%d,  lParam=%d, audioRequest=%d\r\n"),_T(__FUNCTION__), wParam,lParam,m_audioRequest));
	m_bPhoneCallActive = wParam;

	if(!m_pAndroidAuto || !m_bAndroidAutoConnected)
		return ;

	if(wParam)
	{
		m_audioRequestBeforePhonecall = m_audioRequest;
		if(m_audioRequestBeforePhonecall == AUDIO_FOCUS_GAIN)
		{
			RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], loss audio \r\n"),_T(__FUNCTION__)));
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_LOSS_TRANSIENT, false);
		}

		//start a timer to force APP show TEL page:
		if(m_timerAutoShowAAPTel != 0)
		{
			timeKillEvent(m_timerAutoShowAAPTel);
			m_timerAutoShowAAPTel = 0;
		}
		if(!m_bScreenVideoOn)
		{
			m_timerAutoShowAAPTel = timeSetEvent( 2000, 1, AutoShowAAPTelTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
		}
	}
	else
	{
		if(m_audioRequestBeforePhonecall == AUDIO_FOCUS_GAIN && m_idSystemPowerState==SYS_POWER_ON)
		{
			RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], gain audio \r\n"),_T(__FUNCTION__)));
			m_pAndroidAuto->setAudioFocus(AUDIO_FOCUS_STATE_GAIN, false);
		}
	}
}

void CPanelAndroidAuto::PrepareEnterAASourceMode()
{
	if(m_timerEnterAASourceMode == 0)
	{
		m_timerEnterAASourceMode = timeSetEvent( 1000, 1, EnterAASourceModeTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
	}
}
void CPanelAndroidAuto::PrepareQuitAASourceMode()
{
	if(m_timerQuitAASourceMode == 0)
	{
		m_timerQuitAASourceMode = timeSetEvent( 1000, 1, QuitAASourceModeTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
	}
}
BOOL CPanelAndroidAuto::EnterAASourceMode()
{
	if(!m_bPhoneCallActive && !m_bVoiceOn && !m_bAABorrowAudio && m_audioRequest != AUDIO_FOCUS_RELEASE)
	{
		if(m_idMediaClassBeforeAA == -1 && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)!=UI_CLASS_ANDROID_AUTO)
		{
			m_bSrcPlayingBeforeAA = GetCurrentSrcPlayingStatus();
			m_idMediaClassBeforeAA = m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
		}
		RETAILMSG(DEBUG_ANDROIDAUTO,(_T("##EnterAASourceMode: lastMediaClass=0x%x\r\n"),m_idMediaClassBeforeAA));
		SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
		SwitchAudioSRC(SRC_AUDIO_ARM,TRUE,20,FALSE);
		return TRUE;
	}
	return FALSE;
}
BOOL CPanelAndroidAuto::QuitAASourceMode()
{
	if(m_idMediaClassBeforeAA != -1 && m_audioRequest == AUDIO_FOCUS_RELEASE)
	{
		UINT idAudioSrc = GetPanelAudioSrc(m_idMediaClassBeforeAA);
		RETAILMSG(DEBUG_ANDROIDAUTO,(_T("##QuitAASourceMode: lastMediaClass=0x%x, lastAudioSrc=0x%x\r\n"),m_idMediaClassBeforeAA,idAudioSrc));
		SwitchMediaClassSRC(m_idMediaClassBeforeAA);
		SwitchAudioSRC(idAudioSrc,TRUE,1000);
		if(m_bSrcPlayingBeforeAA)
		{
#if CVTE_EN_KEY_PROXY
			CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_RESUME);
			CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_RESUME);
#else
			::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PLAY,NULL);						
#endif
		}
		m_idMediaClassBeforeAA = -1;
		return TRUE;
	}
	return FALSE;
}
void CPanelAndroidAuto::QuitVoiceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelAndroidAuto* panel = (CPanelAndroidAuto*)dwUser;
	if ( panel != NULL )
	{
		panel->QuitVoiceMode();
		timeKillEvent(panel->m_timerQuitVoiceMode);
		panel->m_timerQuitVoiceMode = 0;
	}
}
void CPanelAndroidAuto::EnterVoiceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelAndroidAuto* panel = (CPanelAndroidAuto*)dwUser;
	if ( panel != NULL )
	{
		panel->EnterVoiceMode();
		timeKillEvent(panel->m_timerEnterVoiceMode);
		panel->m_timerEnterVoiceMode = 0;
	}
}
void CPanelAndroidAuto::QuitAASourceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelAndroidAuto* panel = (CPanelAndroidAuto*)dwUser;
	if ( panel != NULL )
	{
		panel->QuitAASourceMode();
		timeKillEvent(panel->m_timerQuitAASourceMode);
		panel->m_timerQuitAASourceMode = 0;
	}
}
void CPanelAndroidAuto::EnterAASourceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelAndroidAuto* panel = (CPanelAndroidAuto*)dwUser;
	if ( panel != NULL )
	{
		panel->EnterAASourceMode();
		timeKillEvent(panel->m_timerEnterAASourceMode);
		panel->m_timerEnterAASourceMode = 0;
	}
}
void CPanelAndroidAuto::videoFocusCallback(VideoFocusMode focus, int reason)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s],focus=%d,reason=%d\r\n"),_T(__FUNCTION__), focus,reason));
	m_pAndroidAuto->setVideoFocus(focus, false);
	if(focus == VIDEO_FOCUS_PROJECTED)
	{
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_ANDROID_AUTO)
		{
			SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_NONE);
		}
	}
	else
	{
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_ANDROID_AUTO)
		{
			//return to previous panel:
			//if previous panel has valid source, but doesn't match current source,
			//don't switch the previous panel, just switch back to home page.
			UINT idUIClass=GET_PANEL_CLASS_ID(m_idPreviousPanel);
			UINT idSRCClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);
			UINT idSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

			if( (idUIClass > UI_CLASS_HOME && idUIClass < UI_CLASS_MAX_VALID && idUIClass != UI_CLASS_IDLE) )
			{
				RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s],focus=%d,reason=%d, switch pre page:0x%x\r\n"),_T(__FUNCTION__), focus,reason,m_idPreviousPanel));

				SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
			}
			else
			{
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			}
		}
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}

void CPanelAndroidAuto::onPairingRequest(const string& phoneAddr, BluetoothPairingMethod pairingMethod)
{
	int cret = MultiByteToWideChar( CP_ACP, 0, phoneAddr.c_str(), -1, m_strPhoneBluetoothAddr, 20);
	if(m_pBTPanel && m_pBTPanel->IsBTModuleIni())
	{
		BOOL bPaired = m_pBTPanel->IsDevicePaired(m_strPhoneBluetoothAddr);
		RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]: IsDevicePaired: %s, %d...\r\n"),_T(__FUNCTION__),m_strPhoneBluetoothAddr,bPaired));

		m_pBTPanel->OnPairRequestFromAndroidAuto(m_strPhoneBluetoothAddr);
		m_pAndroidAuto->onReadyForPairing(bPaired?true:false);
	}
	else
	{
		RETAILMSG(DEBUG_ANDROIDAUTO,(_T(".......requestDelayedPairing...\r\n")));
		m_pAndroidAuto->requestDelayedPairing();
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::microphoneRequestCallback(bool open, bool ancEnabled, bool ecEnabled, int maxUnacked)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s],open=%d,ancEnabled=%d,ecEnabled=%d,maxUnacked=%d\r\n"),_T(__FUNCTION__),open,ancEnabled,ecEnabled,maxUnacked));
}
void CPanelAndroidAuto::voiceSessionNotificationCallback(VoiceSessionStatus status)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], status=%d\r\n"),_T(__FUNCTION__),status));

	m_bAndroidAutoVoiceOn = (status == VOICE_SESSION_START);
	if(!m_bAndroidAutoVoiceOn)
	{
		PrepareQuitVoiceMode();
	}	
	
	//notify gui:
	if(m_bAndroidAutoVoiceOn)
	{
		::PostMessage(m_hMainWnd,WM_ANDROID_AUTO_MSG,ANDROIDAUTO_VOICE_STATUS,1);
	}
	else if(!m_bVoiceOn)
	{
		::PostMessage(m_hMainWnd,WM_ANDROID_AUTO_MSG,ANDROIDAUTO_VOICE_STATUS,0);
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::audioPlaybackStartCallback(AudioStreamType type)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], type=%d, audioRequest=%d\r\n"),_T(__FUNCTION__),type,m_audioRequest));

	/*
	if(type == AUDIO_STREAM_GUIDANCE && m_audioRequest==AUDIO_FOCUS_GAIN_TRANSIENT_MAY_DUCK)
	{
		//notify gui to terminate native map:
		::PostMessage(m_hMainWnd,WM_ANDROID_AUTO_MSG,ANDROIDAUTO_NAVI_STATUS,1);
	}
	*/
	if(type == AUDIO_STREAM_GUIDANCE || type == AUDIO_STREAM_SYSTEM_AUDIO)
	{
		m_bAudioStreamGuidance = TRUE;
		PrepareEnterVoiceMode();
	}
	else if(type == AUDIO_STREAM_MEDIA)
	{
		m_bAudioStreamMedia = TRUE;
		if(m_audioRequest != AUDIO_FOCUS_GAIN)
		{
			printf("==android auto borrow audio\n");
			EnterAASourceMode();
		}	
		if(m_bVoiceOn)
		{
			::PostMessage(m_hMainWnd,WM_MSG_SPEECH,(3<<30), (2<<16) + 1);
		}
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::audioPlaybackStopCallback(AudioStreamType type)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], type=%d\r\n"),_T(__FUNCTION__),type));

	if(type == AUDIO_STREAM_GUIDANCE || type == AUDIO_STREAM_SYSTEM_AUDIO)
	{
		m_bAudioStreamGuidance = FALSE;
		PrepareQuitVoiceMode();
	}
	else if(type == AUDIO_STREAM_MEDIA)
	{
		m_bAudioStreamMedia = FALSE;
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::FirstConnectionTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelAndroidAuto* panel = (CPanelAndroidAuto*)dwUser;

	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("AAP:FirstConnectionTimerProc....\r\n")));

	//first connection, we should switch to AAP page
	//to show user prompt:
	if(panel->m_bAndroidAutoConnected && GET_PANEL_CLASS_ID(panel->m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_ANDROID_AUTO)
	{
		SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_NONE);
	}

	timeKillEvent(panel->m_timerFirstConnection);
	panel->m_timerFirstConnection = 0;
}
void CPanelAndroidAuto::AutoShowAAPTelTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelAndroidAuto* panel = (CPanelAndroidAuto*)dwUser;

	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("AAP:AutoShowAAPTelTimerProc....\r\n")));

	if(panel->m_bPhoneCallActive && panel->m_bScreenVideoOn)
	{
		//panel->m_pAndroidAuto->onKeyEvent(KEYCODE_TEL,true);
		//Sleep(50);
		//panel->m_pAndroidAuto->onKeyEvent(KEYCODE_TEL,false);
	}

	timeKillEvent(panel->m_timerAutoShowAAPTel);
	panel->m_timerAutoShowAAPTel = 0;
}
void CPanelAndroidAuto::KillAAPVideoFocusTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelAndroidAuto* panel = (CPanelAndroidAuto*)dwUser;
	if(!panel->m_pSkinManager->GetConfig()->GetCurParam()->bAAPAutoLaunch &&
		GET_PANEL_CLASS_ID(panel->m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_ANDROID_AUTO)
	{
		RETAILMSG(DEBUG_ANDROIDAUTO,(_T("AAP not auto launch, KillAAPVideoFocus, change video focus to VIDEO_FOCUS_NATIVE\r\n")));
		panel->m_pAndroidAuto->setVideoFocus(VIDEO_FOCUS_NATIVE,false);
	}
	timeKillEvent(panel->m_timerKillAAPVideoFocus);
	panel->m_timerKillAAPVideoFocus = 0;
}
void CPanelAndroidAuto::vedioPlaybackStartCallback()
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]\r\n"),_T(__FUNCTION__)));
	m_bScreenVideoOn = TRUE;
	m_bNeedShowPrompt = FALSE;

	if(!m_pSkinManager->GetConfig()->GetCurParam()->bAAPAutoLaunch &&
		GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) != UI_CLASS_ANDROID_AUTO)
	{
		//if auto launch is disabled, kill AAP video focus after a while:
		m_timerKillAAPVideoFocus = timeSetEvent( 300, 1, KillAAPVideoFocusTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
	}
	else
	{
		if(m_pSkinManager->GetGUI()->isActivated())
			m_pSkinManager->GetGUI()->EnableSwap(FALSE);
	}

	//video connected, the connection is NOT a first connection,
	//terminate the timer:
	if(m_timerFirstConnection != 0)
	{
		timeKillEvent(m_timerFirstConnection);
		m_timerFirstConnection = 0;
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::vedioPlaybackStopCallback()
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]\r\n"),_T(__FUNCTION__)));
	m_bScreenVideoOn = FALSE;
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
BOOL CPanelAndroidAuto::EnterVoiceMode()
{
	if(/*!m_bVoiceOn &&*/ m_bAudioStreamGuidance)
	{
		DWORD duck = 0;
		if(m_bAudioStreamMedia)
			duck = 2;
		else if(m_audioRequest==AUDIO_FOCUS_GAIN_TRANSIENT_MAY_DUCK)
			duck = 1;
		RETAILMSG(DEBUG_ANDROIDAUTO,(_T("....EnterVoiceMode, duck=%d\r\n"),duck));
		m_bVoiceOn = TRUE;
		DuckNativeNaviSpeech(m_bAndroidAutoVoiceOn?DUCK_NO_VOLUME:DUCK_LOW_VOLUME);
		::PostMessage(m_hMainWnd,WM_ANDROID_AUTO_MSG,ANDROIDAUTO_VOICE_STATUS,1);
		::PostMessage(m_hMainWnd,WM_MSG_SPEECH,(3<<30), (duck<<16) + 1);
		return TRUE;
	}
	return FALSE;
}
BOOL CPanelAndroidAuto::QuitVoiceMode()
{
	if(!m_bAndroidAutoVoiceOn && !m_bAudioStreamGuidance && m_bVoiceOn)
	{
		RETAILMSG(DEBUG_ANDROIDAUTO,(_T("....QuitVoiceMode...\r\n")));
		m_bVoiceOn = FALSE;
		::PostMessage(m_hMainWnd,WM_MSG_SPEECH,(3<<30),0);
		DuckNativeNaviSpeech(DUCK_NORMAL_VOLUME);
		::PostMessage(m_hMainWnd,WM_ANDROID_AUTO_MSG,ANDROIDAUTO_VOICE_STATUS,0);
		return TRUE;
	}
	return FALSE;
}
void CPanelAndroidAuto::PrepareEnterVoiceMode()
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]\r\n"),_T(__FUNCTION__)));
	if(m_timerEnterVoiceMode == 0)
	{
		m_timerEnterVoiceMode = timeSetEvent( 200, 1, EnterVoiceModeTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::PrepareQuitVoiceMode()
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]\r\n"),_T(__FUNCTION__)));
	if(m_timerQuitVoiceMode == 0)
	{
		m_timerQuitVoiceMode = timeSetEvent( 1000, 1, QuitVoiceModeTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
	}
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}
void CPanelAndroidAuto::naviFocusCallback(NavFocusType type)
{
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], type=%d\r\n"),_T(__FUNCTION__),type));

	m_pAndroidAuto->setNaviFocus(type);

	//notify gui:
	::PostMessage(m_hMainWnd,WM_ANDROID_AUTO_MSG,ANDROIDAUTO_NAVI_STATUS,(type==NAV_FOCUS_PROJECTED));
	RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]---\r\n"),_T(__FUNCTION__)));
}

void CPanelAndroidAuto::LinkBluetoothPanel(CPanelBluetoothWrapper *pBTPanel)
{
	if(pBTPanel)
	{
		m_pBTPanel=pBTPanel;
		pBTPanel->RegisterPairRequestCallback(onBluetoothPairRequestCallback,this);
	}
}
void CPanelAndroidAuto::onBluetoothPairRequestCallback(LPCTSTR devAddr,LPCTSTR passkey,LPVOID para)
{
	CPanelAndroidAuto *panel = (CPanelAndroidAuto*)para;
	if(panel && panel->m_pAndroidAuto && panel->m_bAndroidAutoConnected)
	{
		if(devAddr && passkey)
		{
			char npasskey[20]={0};
			int cret = WideCharToMultiByte( CP_ACP, 0, passkey, -1, npasskey, _countof(npasskey), NULL, NULL );
			if ( cret > 0 )
			{
				RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s], sendAuthData=%s\r\n"),_T(__FUNCTION__),passkey));
				panel->m_pAndroidAuto->sendAuthData(string(npasskey));
			}
		}
		else
		{
			if(panel->m_pBTPanel && panel->m_strPhoneBluetoothAddr[0])
			{
				BOOL bPaired = panel->m_pBTPanel->IsDevicePaired(panel->m_strPhoneBluetoothAddr);
				RETAILMSG(DEBUG_ANDROIDAUTO,(_T("[%s]: IsDevicePaired: %s, %d...\r\n"),_T(__FUNCTION__),panel->m_strPhoneBluetoothAddr,bPaired));

				panel->m_pBTPanel->OnPairRequestFromAndroidAuto(panel->m_strPhoneBluetoothAddr);
				panel->m_pAndroidAuto->onReadyForPairing(bPaired?true:false);
				//reset:
				panel->m_strPhoneBluetoothAddr[0] = 0;
			}
		}
	}
}

string CPanelAndroidAuto::getId() {
	// TODO: Must generate a random number for vehicle ID during a factory reset.

	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Ident\\"));
	LPCTSTR id_wide=reg.ValueSZ(_T("VehicleID"));
	if(!id_wide)
	{
		GenerateId();
	}
	id_wide=reg.ValueSZ(_T("VehicleID"));
	if(id_wide)
	{
		char id_multi[40];
		int ret = WideCharToMultiByte( CP_ACP, 0, id_wide, -1, id_multi, _countof(id_multi), NULL, NULL );
		if(ret > 0)
		{
			RETAILMSG(1,(_T("==getId: succeed: %S \r\n"),id_multi));
			return id_multi;
		}
		else
		{
			return "8d11fb57-c7cb-44c6-8599-19d763969437";
		}
	}
	else
	{
		return "8d11fb57-c7cb-44c6-8599-19d763969437";
	}
}
bool CPanelAndroidAuto::GenerateId()
{
	RETAILMSG(1,(_T("==GenerateId==\r\n")));
	const wchar_t *s=_T("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	wchar_t data[37];
	LARGE_INTEGER		t;
	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Ident\\"));
	LPCTSTR id=reg.ValueSZ(_T("VehicleID"));
	if(!id)
	{
		srand(GetTickCount());
		for (int i=0;i<37;i++)
		{
			Sleep(abs(rand()%5));
			if( QueryPerformanceCounter( &t ) )
			{
				data[i] = s[t.QuadPart%61];
			}
			else
			{
				data[i] = s[rand()%61];
			}
		}
		data[8]='-';
		data[13]='-';
		data[18]='-';
		data[23]='-';
		data[36]=0;
		RETAILMSG(1,(_T("==GenerateId: succeed: %s \r\n"),data));
		reg.SetSZ(_T("VehicleID"),data);
	}
	else
	{
		RETAILMSG(1,(_T("==GenerateId: already exist: %s \r\n"),id));
	}

	return true;
}
void CPanelAndroidAuto::OnNightModeChanged(BOOL bNightMode)
{
	m_bOnNightMode = bNightMode;
	m_pAndroidAuto->reportNightModeData(m_bOnNightMode);
}

void CPanelAndroidAuto::OnBrakeStatusChanged(BOOL bOn)
{
	CPanelGL::OnBrakeStatusChanged(bOn);

	m_pAndroidAuto->reportParkingBrakeData(bOn);

	if (bOn)
	{
		m_pAndroidAuto->reportDrivingStatusData(DRIVE_STATUS_UNRESTRICTED);
	}
	else
	{
		m_pAndroidAuto->reportDrivingStatusData(DRIVE_STATUS_NO_VIDEO|DRIVE_STATUS_NO_KEYBOARD_INPUT|DRIVE_STATUS_NO_VOICE_INPUT|DRIVE_STATUS_NO_CONFIG|DRIVE_STATUS_LIMIT_MESSAGE_LEN);
	}

}

#endif
