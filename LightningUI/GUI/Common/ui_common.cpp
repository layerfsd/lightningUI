#include "../../StdAfx.h"
#include "ui_common.h"
#include <wavedev.h>
#include "../Communication/Renesas_com.h"

#define WODM_SPEECH_CONTROL				501

extern HINSTANCE			g_hInst;
extern HWND					g_hUIWnd;
extern UINT						g_idLanguageOffset;
extern BOOL						g_bKeyBeep;
extern DWORD					g_dwNaviProcessID;
extern BOOL						g_bNaviSpeechEnabled;
extern BOOL						g_bNaviSpeechSignaled;

HANDLE g_hWAV = NULL;

// SWITCH_PAGE_PARAM g_swPagePara;

BOOL SwitchPage(UINT idClass,UINT idPage,PAGE_ANI_ID idSwitchAniId,UINT pageParam,LPVOID lpAniPara,BOOL	bUserInteraction)
{
	if(g_hUIWnd)
	{
		//SWITCH_PAGE_PARAM para={idClass,idPage,idSwitchAniId,pageParam,lpAniPara};
		SWITCH_PAGE_PARAM *para = new SWITCH_PAGE_PARAM;
		memset(para, 0, sizeof(SWITCH_PAGE_PARAM));
		para->idClass=idClass;
		para->idPage=idPage;
		para->idSwitchAniId=idSwitchAniId;
		para->pageParam=pageParam;
		para->lpAniPara=lpAniPara;
		para->bUserInteraction=bUserInteraction;

		return ::PostMessage(g_hUIWnd,WM_SWITCH_PANEL,0,LPARAM(para));
	}

	return TRUE;
}
BOOL IsPanelAutoSwitchMediaSRC(UINT idPanel)
{
	UINT idclass=GET_PANEL_CLASS_ID(idPanel);
	UINT idpage=GET_PANEL_PAGE_ID(idPanel);

	return IsPanelHasValidAudioSrc(idPanel)
		&& idclass!=UI_CLASS_CARPLAY
		&& idclass!=UI_CLASS_PHONELINK
#if CVTE_EN_EASYCONNECT
		&& idclass!=UI_CLASS_EASYCONNECTED
#endif
#if CVTE_EN_WEBLINK
		&& idclass!=UI_CLASS_WEBLINK
#endif
#if CVTE_EN_ANDROID_AUTO
		&& idclass!=UI_CLASS_ANDROID_AUTO
#endif
		;
}

BOOL IsPanelAutoUnMute(UINT idPanel)
{
	UINT idclass=GET_PANEL_CLASS_ID(idPanel);
	UINT idpage=GET_PANEL_PAGE_ID(idPanel);

	if (IsPanelHasValidAudioSrc(idPanel))
	{
		return (((idclass != UI_CLASS_MEDIA) && (idclass != UI_CLASS_IPOD)
			&& (idclass != UI_CLASS_BT)) ? TRUE:FALSE);
	}
	else
	{
		return FALSE;
	}	

}

BOOL IsPanelHasValidAudioSrc(UINT idPanel)
{
	UINT idclass=GET_PANEL_CLASS_ID(idPanel);
	UINT idpage=GET_PANEL_PAGE_ID(idPanel);

	switch (idclass)
	{
	case UI_CLASS_MEDIA:
		return idpage == PAGE_MEDIA_MAIN;
		break;
	case UI_CLASS_RADIO:
		return idpage == PAGE_RADIO_MAIN;
		break;
	case UI_CLASS_BT:
		return idpage==PAGE_BT_A2DP;// || idpage == PAGE_BT_TALKING;
		break;
	case UI_CLASS_IPOD:
		return idpage == PAGE_IPOD_MAIN;
		break;
	case UI_CLASS_DAB:
		return idpage == PAGE_DAB_MAIN; //20160428
		break;
	case UI_CLASS_DISK:
	case UI_CLASS_MHL:
	case UI_CLASS_PHONELINK:
	case UI_CLASS_TV:
	case UI_CLASS_CARPLAY:
	case UI_CLASS_AUX:
	case UI_CLASS_EASYCONNECTED:
	case UI_CLASS_WEBLINK:
	case UI_CLASS_ANDROID_AUTO:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}
UINT GetPanelAudioSrc(UINT idPanel)
{
	UINT ret=SRC_AUDIO_INVALID;
	UINT idclass=GET_PANEL_CLASS_ID(idPanel);
	UINT idpage=GET_PANEL_PAGE_ID(idPanel);

	switch (idclass)
	{
	case UI_CLASS_MEDIA:
	case UI_CLASS_BT:
	case UI_CLASS_IPOD:
	case UI_CLASS_PHONELINK:
	case UI_CLASS_CARPLAY:
	case UI_CLASS_EASYCONNECTED:
	case UI_CLASS_WEBLINK:
	case UI_CLASS_ANDROID_AUTO:
		ret = SRC_AUDIO_ARM;
		break;
	case UI_CLASS_RADIO:
		ret = SRC_AUDIO_RADIO;
		break;
	case UI_CLASS_DAB:
		ret = SRC_AUDIO_DAB;
		break;
	case UI_CLASS_DISK:
		ret = SRC_AUDIO_DVD;
		break;
	case UI_CLASS_MHL:
		ret = SRC_AUDIO_MHL;
		break;
	case UI_CLASS_TV:
		ret = SRC_AUDIO_TV;
		break;
	case UI_CLASS_AUX:
		ret = SRC_AUDIO_AUX1;
		break;
	default:
		break;
	}
	return ret;
}
BOOL SetNativeNaviSpeechVolume(DWORD dwVolume)
{
	static DWORD s_vol = -1;
	if(dwVolume == s_vol)
		return FALSE;
	s_vol = dwVolume;
	MMDRV_MESSAGE_PARAMS Params;
	BOOL bRet = FALSE;

	/* Must reset the structure fully to avoid the un-initialized field of "uDeviceId" is referenced in WAV driver. */
	ZeroMemory(&Params, sizeof(MMDRV_MESSAGE_PARAMS));

	Params.uMsg = WODM_SPEECH_CONTROL;

	Params.dwParam1 = dwVolume;
	Params.dwParam2 = g_bNaviSpeechEnabled;

	if (g_hWAV == NULL)
	{
		g_hWAV = CreateFile(TEXT("WAV1:"),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);

		if (INVALID_HANDLE_VALUE == g_hWAV)
		{
			RETAILMSG(1, (TEXT("Fail to open wav driver\r\n")));
			bRet = FALSE;
		}
	}

	if(g_hWAV!=NULL && g_hWAV!=INVALID_HANDLE_VALUE)
	{
		bRet = DeviceIoControl(g_hWAV, IOCTL_WAV_MESSAGE, &Params, sizeof(MMDRV_MESSAGE_PARAMS), 0, 0, NULL, NULL);

		if (FALSE == bRet)
		{
			RETAILMSG(1, (TEXT(" fail IOCTL_WAV_MESSAGE\r\n")));
			bRet = FALSE;
		}
	}

	// 	if (hWAV)
	// 	{
	// 		CloseHandle(hWAV);
	// 		hWAV = NULL;
	// 	}

	RETAILMSG(1,(_T(">>>>SetNativeNaviSpeechVolume: 0x%x, bRet %d.....\r\n"),dwVolume,bRet));
	return bRet;
}
BOOL DuckNativeNaviSpeech(DUCK_STATUS duckStatus)
{
	if(!g_bNaviSpeechEnabled)
		return FALSE;
	if(duckStatus == DUCK_NO_VOLUME)
		return SetNativeNaviSpeechVolume(0);
	else if(duckStatus == DUCK_LOW_VOLUME)
		return SetNativeNaviSpeechVolume(0xCCCCCCCC);
	else
		return SetNativeNaviSpeechVolume(-1);
}
BOOL EnableNaviSpeech(BOOL bEnable)
{
	BOOL bRet = FALSE;
	g_bNaviSpeechEnabled = bEnable;
	//return EnableAudio(bEnable);

	if(bEnable)
		::SendMessage(g_hUIWnd,WM_SPEECH_ENABLE_MSG,bEnable,NULL);

	bRet = SetNativeNaviSpeechVolume(bEnable?-1:0);

	if(!bEnable)
		::SendMessage(g_hUIWnd,WM_SPEECH_ENABLE_MSG,bEnable,NULL);

	return bRet;
}
BOOL EnableAudio(BOOL bEnable)
{
	RETAILMSG(1,(_T("MSG:EnableAudio:  bEnable: %d.....\r\n"),bEnable));

	if(bEnable)
		::SendMessage(g_hUIWnd,WM_SPEECH_ENABLE_MSG,bEnable,NULL);

 	if(bEnable)
		waveOutSetVolume(0,-1);
 	else
		waveOutSetVolume(0,0);

	if(!bEnable)
		::SendMessage(g_hUIWnd,WM_SPEECH_ENABLE_MSG,bEnable,NULL);

#if 1
	MMDRV_MESSAGE_PARAMS Params;
	BOOL bRet = FALSE;

	/* Must reset the structure fully to avoid the un-initialized field of "uDeviceId" is referenced in WAV driver. */
	ZeroMemory(&Params, sizeof(MMDRV_MESSAGE_PARAMS));

	Params.uMsg = WODM_SPEECH_CONTROL;

	Params.dwParam1 = -1;
	Params.dwParam2 = bEnable?1:0;

	if (g_hWAV == NULL)
	{
		g_hWAV = CreateFile(TEXT("WAV1:"),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);

		if (INVALID_HANDLE_VALUE == g_hWAV)
		{
			RETAILMSG(1, (TEXT("Fail to open wav driver\r\n")));
			bRet = FALSE;
		}
	}

	if(g_hWAV!=NULL && g_hWAV!=INVALID_HANDLE_VALUE)
	{
		bRet = DeviceIoControl(g_hWAV, IOCTL_WAV_MESSAGE, &Params, sizeof(MMDRV_MESSAGE_PARAMS), 0, 0, NULL, NULL);

		if (FALSE == bRet)
		{
			RETAILMSG(1, (TEXT(" fail IOCTL_WAV_MESSAGE\r\n")));
			bRet = FALSE;
		}
	}

// 	if (hWAV)
// 	{
// 		CloseHandle(hWAV);
// 		hWAV = NULL;
// 	}

	RETAILMSG(1,(_T(">>>>EnableAudio,%d, bRet %d.....\r\n"),bEnable,bRet));
	return bRet;
#endif
}


const wchar_t* GetResourceString(UINT id,UINT idLang)
{
	return (const wchar_t*)::LoadString( g_hInst, id+((idLang!=-1)?(idLang*1000):g_idLanguageOffset), NULL, 0 );
}

void ShowKeyboard(UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,UINT type,const wchar_t* pStrTitle,const wchar_t* pStrIni,UINT iMaxInputLen,BOOL bAutoReturn)
{
	if(g_hUIWnd)
	{
		KEYBOARDMSG kbd={idCaller,pCallbackFunc,lpCallbackParam,type,pStrTitle,pStrIni,iMaxInputLen,bAutoReturn};
		::SendMessage(g_hUIWnd,WM_SHOW_KEYBOARD,TRUE,LPARAM(&kbd));
	}
}

void ShowExplorer(UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,const wchar_t *lpTitle,const wchar_t *lpPath,const wchar_t *lpFileFilter)
{
	if(g_hUIWnd)
	{
		EXPLORERMSG exp={idCaller,pCallbackFunc,lpCallbackParam,lpTitle,lpPath,lpFileFilter};
		::SendMessage(g_hUIWnd,WM_SHOW_EXPLORER,TRUE,LPARAM(&exp));
	}
}

void ShowPasswordBox(HWND hWnd,UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,UINT type,const wchar_t *pstrTitle,
						 const wchar_t *pstrNotification, UINT idIcon, BOOL bAutoHide, DWORD dwShowTime)
{
	if(g_hUIWnd)
	{
		NBMSG nb={idCaller,pCallbackFunc,lpCallbackParam,type,pstrTitle,pstrNotification,idIcon,dwShowTime,bAutoHide};
		::SendMessage(hWnd,WM_SHOW_PASSWORD_BOX_CMD,1,LPARAM(&nb));
	}
}

void ShowNotificationBox(HWND hWnd,UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,UINT type,const wchar_t *pstrTitle,
						 const wchar_t *pstrNotification, UINT idIcon, BOOL bAutoHide, DWORD dwShowTime)
{
	if(g_hUIWnd)
	{
		NBMSG nb={idCaller,pCallbackFunc,lpCallbackParam,type,pstrTitle,pstrNotification,idIcon,dwShowTime,bAutoHide};
		::SendMessage(hWnd,WM_SHOW_MSGBOX,1,LPARAM(&nb));
	}
}
void HideNotificationBox(HWND hWnd)
{
	if(g_hUIWnd)
	{
		::SendMessage(hWnd,WM_SHOW_MSGBOX,0,NULL);
	}
}
void SendMediaCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_hUIWnd)
	{
		::SendMessage(g_hUIWnd,WM_MEDIAPLAYER_CMD,wParam,lParam);
	}
}
void SendBluetoothCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_hUIWnd)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]: wParam[%X] lParam[%X]\r\n"),TEXT(__FUNCTION__),wParam, lParam));
		::SendMessage(g_hUIWnd,WM_BLUETOOTH_CMD,wParam,lParam);
	}
}
void SendRadioCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_hUIWnd)
	{
		::SendMessage(g_hUIWnd,WM_RADIO_CMD,wParam,lParam);
	}
}
void SendiPodCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_hUIWnd)
	{
		::SendMessage(g_hUIWnd,WM_IPOD_CMD,wParam,lParam);
	}
}

void SendFactoryCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_hUIWnd)
	{
		::SendMessage(g_hUIWnd,WM_FACTORY_CMD,wParam,lParam);
	}
}

UINT GetAudioSRC()
{
	UINT idRet=0;

	if(g_hUIWnd)
	{
		idRet = ::SendMessage(g_hUIWnd,WM_GET_AUDIO_SRC,0,0);
	}

	//RETAILMSG(1,(_T("MSG:[GetAudioSRC]: ####### %d #######\r\n"),idRet));

	return idRet;
}
BOOL SwitchAudioSRC(UINT idSRC,BOOL bAutoMute,UINT iMuteDelay,BOOL bUnmuteFade)
{
	BOOL bRet=FALSE;

	if(g_hUIWnd)
	{
		bRet = ::SendMessage(g_hUIWnd,WM_AUDIO_SRC_CMD,idSRC,(bUnmuteFade<<24)+(iMuteDelay<<8)+bAutoMute);
	}
	else
	{
		bRet = FALSE;
	}

//		if(bRet)
//		{
//			RETAILMSG(1,(_T("\r\n    ####### SwitchAudioSRC: 0x%x, bAutoMute,%d,  bRet:%d #######   \r\n\r\n"),idSRC,bAutoMute,bRet));
//		}

	return bRet;
}
void MuteAudio(BOOL bMute,BOOL bFade,UINT type,BOOL bAutoRelease,UINT time_out)
{
	if(time_out<100)
		time_out=100;

	if(g_hUIWnd)
	{
		::SendMessage(g_hUIWnd,WM_MUTE_AUDIO_CMD,(bMute<<24)+(bFade<<16)+type,(bAutoRelease<<16)+(time_out));
	}
}
BOOL SwitchMediaClassSRC(UINT idClass,UINT iParam)
{
	BOOL bRet=FALSE;

	if(g_hUIWnd)
	{
		bRet = ::SendMessage(g_hUIWnd,WM_MEDIA_CLASS_SRC_CMD,idClass,iParam);
	}
	else
	{
		bRet = FALSE;
	}

	if(bRet)
	{
		RETAILMSG(1,(_T("MSG:[%s]: idClass:0x%x, bRet:%d \r\n"),TEXT(__FUNCTION__),idClass,bRet));
	}

	return bRet;
}

BOOL GetCurrentSrcPlayingStatus()
{
	BOOL bRet=FALSE;

	if(g_hUIWnd)
	{
		bRet = ::SendMessage(g_hUIWnd,WM_GET_PLAYING_STATUS,0,0);
	}
	else
	{
		bRet = FALSE;
	}

	RETAILMSG(1,(_T("####### GetCurrentSrcPlayingStatus,  bRet:%d ####### \r\n"),bRet));

	return bRet;
}

BOOL GetCurrentMuteStatus()
{
	BOOL bRet=FALSE;

	if(g_hUIWnd)
	{
		bRet = ::SendMessage(g_hUIWnd,WM_GET_CUR_MUTE_STATUS,0,0);
	}
	else
	{
		bRet = FALSE;
	}

	RETAILMSG(1,(_T("####### GetCurrentMuteStatus,  bRet:%d ####### \r\n"),bRet));

	return bRet;
}

BOOL PlaySoundEffect(UINT idSnd)
{
	static DWORD s_last_beep=0;
	BOOL bRet=FALSE;

	if(g_bKeyBeep)
	{
		if(g_hUIWnd)
		{
			DWORD dwTick=GetTickCount();
			if(dwTick-s_last_beep>100)
			{
				s_last_beep=dwTick;
				bRet = ::PostMessage(g_hUIWnd,WM_CMD_PLAY_SOUND_EFFECT,idSnd,NULL);
			}
		}
		else
		{
			bRet = FALSE;
		}

		//RETAILMSG(1,(_T("\r\n    ####### PlaySoundEffect: 0x%x,  bRet:%d #######   \r\n\r\n"),idSnd,bRet));

	}

	return bRet;
}

const unsigned char MonthStr[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov","Dec"};

void GetBuildDateTime(BuildDateTime *p_BuildDateTime)
{
	unsigned char temp_str[4] = {0, 0, 0, 0}, i = 0;

	sscanf(__DATE__, "%s %d %d", temp_str, &(p_BuildDateTime->Date), &(p_BuildDateTime->Year));
	sscanf(__TIME__, "%d:%d:%d", &(p_BuildDateTime->Hours), &(p_BuildDateTime->Minutes), &(p_BuildDateTime->Seconds));
	for (i = 0; i < 12; i++)
	{
		if (temp_str[0] == MonthStr[i][0] && temp_str[1] == MonthStr[i][1] && temp_str[2] == MonthStr[i][2])
		{
			p_BuildDateTime->Month = i + 1;
			break;
		}
	}
}


BOOL IsUseDST(TIME_ZONE_INFORMATION &tziCurrent, SYSTEMTIME &stCurrent)
{
	BOOL ret = FALSE;
	SYSTEMTIME tmpstCurrent = stCurrent;
	SYSTEMTIME tmpstStandardDate = tziCurrent.StandardDate;
	SYSTEMTIME tmpstDaylightDate = tziCurrent.DaylightDate;
	__int64 ftcurrent = 0;
	__int64 ftStandardDate = 0;
	__int64 ftDaylightDate = 0;
	
	tmpstCurrent.wYear = 2000;
	tmpstStandardDate.wYear = 2000;
	tmpstDaylightDate.wYear = 2000;
	SystemTimeToFileTime(&(tmpstCurrent), (FILETIME*)&ftcurrent);
	SystemTimeToFileTime(&(tmpstStandardDate), (FILETIME*)&ftStandardDate);
	SystemTimeToFileTime(&(tmpstDaylightDate), (FILETIME*)&ftDaylightDate);
	if ( ftDaylightDate < ftStandardDate )
	{
		ret = FALSE;
		if ( (ftcurrent>ftDaylightDate) && (ftcurrent<ftStandardDate) )
		{
			ret = TRUE;
		}
	}
	else if ( ftDaylightDate > ftStandardDate )
	{
		ret = TRUE;
		if ( (ftcurrent>ftStandardDate) && (ftcurrent<ftDaylightDate) )
		{
			ret = FALSE;
		}
	}
	return ret;
}


//===========================================================================================================================
//	CRC32
//===========================================================================================================================

// Note: This uses the same polynomial as zlib: x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1.

static const UINT	kCRC32Table[ 256 ] =
{
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 
	0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 
	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 
	0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 
	0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950, 
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 
	0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 
	0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 
	0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 
	0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 
	0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242, 
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 
	0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 
	0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

UINT	CRC32( UINT inCRC, const void *inData, size_t inSize )
{
	const BYTE *		src;
	const BYTE *		end;

	inCRC ^= 0xFFFFFFFFU;
	src = (const BYTE *) inData;
	end = src + inSize;
	while( src != end )
	{
		inCRC = kCRC32Table[ ( inCRC ^ ( *src++ ) ) & 0xFF ] ^ ( inCRC >> 8 );
	}
	inCRC ^= 0xFFFFFFFFU;
	return( inCRC );
}

BOOL IsValidMcuUpdateFile(const wchar_t* fileUpdate, const wchar_t* fileChecksum)
{
	BOOL bRet = FALSE;

	FILE *fp_update=NULL;
	FILE *fp_checksum=NULL;
	int				fileSize_update = 0;
	BYTE				*localBuffer_update = NULL;
	int				fileSize_checksum = 0;
	BYTE				*localBuffer_checksum = NULL;

	if(fileUpdate && fileChecksum)
	{
		//read update file:
		if ( 0!=_wfopen_s(&fp_update, fileUpdate, L"rb") )
		{
			RETAILMSG(1,(_T("ERROR: update file not found,%s\n"), fileUpdate));
			goto exit;
		}	

		/**** Computer texture data size ****/
		fseek (fp_update , 0 , SEEK_END);
		fileSize_update = ftell(fp_update);
		fseek(fp_update, 0, SEEK_SET);

		/**** Read data from file ****/
		localBuffer_update = new BYTE[fileSize_update];
		if(!localBuffer_update)
		{
			goto exit;
		}
		fread(localBuffer_update, fileSize_update, 1, fp_update);

		//read checksum file:
		if ( 0!=_wfopen_s(&fp_checksum, fileChecksum, L"rb") )
		{
			RETAILMSG(1,(_T("ERROR: checksum file not found,%s\n"), fileChecksum));
			goto exit;
		}	

		/**** Computer texture data size ****/
		fseek (fp_checksum , 0 , SEEK_END);
		fileSize_checksum = ftell(fp_checksum);
		fseek(fp_checksum, 0, SEEK_SET);

		/**** Read data from file ****/
		localBuffer_checksum = new BYTE[fileSize_checksum];
		if(!localBuffer_checksum)
		{
			goto exit;
		}
		fread(localBuffer_checksum, fileSize_checksum, 1, fp_checksum);

		//check valid:
		UINT crc32 = CRC32(0,localBuffer_update,fileSize_update);
		char str_crc32[16];
		sprintf_s(str_crc32,16,"%08X",crc32);
		RETAILMSG(1,(_T("==>MCU CRC32 = %S\n"), str_crc32));
		char *p=strstr((char*)localBuffer_checksum,"CRC32: ");
		if(p)
		{
			p+=7;
			bRet = (memcmp(p,str_crc32,8) == 0);
		}
	}

exit:
	//clear:
	if(localBuffer_update)
	{
		delete[] localBuffer_update;
		localBuffer_update=NULL;
	}
	if(localBuffer_checksum)
	{
		delete[] localBuffer_checksum;
		localBuffer_checksum=NULL;
	}
	if(fp_update)
	{
		fclose(fp_update);
		fp_update=NULL;
	}
	if(fp_checksum)
	{
		fclose(fp_checksum);
		fp_checksum=NULL;
	}

	RETAILMSG(1,(_T("==>Check MCU update file valid, bRet=%d\n"), bRet));

	return bRet;
}

MYCODEPAGE GetLanguageCodePage(UINT idLang)
{
	switch (idLang)
	{
	case LANGUAGE_RUSSIAN:
		return CODEPAGE_RFC;
		break;
	default:
		return CODEPAGE_GBK;
		break;
	}
}