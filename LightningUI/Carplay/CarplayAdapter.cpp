#include "CarplayAdapter.h"
#include "../../LightningUI.h"
#include "IAP2I2C.h"
#include "CVTE_IoCtl.h"

#if CVTE_EN_CARPLAY

#ifdef _DEBUG
#pragma comment(lib,"atlsd.lib")
#else
#pragma comment(lib,"atls.lib")
#endif

#define CPCA_DEVICE_NAME		  _T("CPC1:")
#define CPMAC_DEVICE_NAME		  _T("CPM1:")
#define CPD_DEVICE_NAME		      _T("CPD1:")

#define HexDigitsUppercase			"0123456789ABCDEF"

const CCarplayAdapter* CCarplayAdapter::m_pInstance = new CCarplayAdapter();

const wchar_t * CCarplayAdapter::m_DestCAPath1 = _T("\\StaticStore");
const wchar_t * CCarplayAdapter::m_DestCAPath2 = _T("\\ResidentFlash");

HANDLE CCarplayAdapter::m_hcpd = INVALID_HANDLE_VALUE;

BOOL CCarplayAdapter::m_bStoreCa = TRUE;

CCarplayAdapter::CCarplayAdapter()
:CMiniThread( 0, TRUE ),m_bInitialized(FALSE),
m_ControlStreams(FALSE),m_MainAudioStreams(FALSE),
m_AltAudioStreams(FALSE),m_MainScreenStreams(FALSE),
m_bServerInit(FALSE),
m_pSkinManager(NULL),m_hMainWnd(NULL),
m_bTestMode(FALSE),m_CarplayUSBPort(eCarplayAPIUSBPort1),
m_bUserSelectAudioSRC(FALSE),
m_bUserSelectNativeNavi(FALSE),
m_bUserSelectNativeUI(FALSE),
m_bFirstTimeConnect(FALSE),
m_tcpserver(NULL),m_udpserver(NULL),
m_bNeedShowTopBanner(FALSE),
m_isAddressValid(FALSE),m_isSessionSessionUpDate(FALSE),m_SessionAddressFamily(AF_UNSPEC),
m_bLimitUI(FALSE),
m_DeviceActivateCallback(NULL),m_SetMACCallback(NULL),m_DeviceActivateCallbackContext(NULL),
m_bOnReverseMode(FALSE),m_bSetMAC(FALSE),m_hSetMACEvent(NULL),
m_idPowerState(SYS_POWER_ON),m_ConnectTimer(0)
{
    AirPlayModeStateInit(&m_carplayModeStatus);
    memset(m_strDeviceName, 0, sizeof(m_strDeviceName));
	m_bOnNightMode = FALSE;
	m_bDealIniBluetoothPhone = FALSE;
	m_bShowAccOnWarning = FALSE;
	memset(&m_InitialModes, 0,sizeof(AirPlayModeChanges));
	m_hSetMACEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CCarplayAdapter::~CCarplayAdapter()
{
	ClearAllBluetoothID();
    Deinitialize();
}

CCarplayAdapter* CCarplayAdapter::GetInstance()
{
    return (CCarplayAdapter*)m_pInstance;
}
BOOL CCarplayAdapter::IsInitialized()
{
	return m_bInitialized;
}
void CCarplayAdapter::Initialize(CSkinManagerGL* pSkinManager, HWND window, DeviceActivateCallback_f callback, SetMACCallback_f setmaccallback, PVOID context)
{
    OSStatus err = kNoErr;
	char versionstr[MAX_PATH];
	LPCWSTR languages[] =	{ L"en" };

    m_pSkinManager = pSkinManager;
    m_hMainWnd = window;
	m_DeviceActivateCallback = callback;
	m_SetMACCallback = setmaccallback;
	m_DeviceActivateCallbackContext = context;

    while (!m_bInitialized)
    {
        CARPLAY_INIT_PARAM param;

		err = CarplayAPIGetVersion(versionstr, _countof(versionstr));
		if (err)
		{
			printf("Get Version Failed!\n");
			break;
		}

		printf("CarPlay API Version : %s\n", versionstr);

		if (_stricmp(versionstr, CarplayAPIVersionString) != 0)
		{
			printf("Version is incorrect, Please upgrade the new version of the library!\n");
			break;
		}

#if (CVTE_EN_CPCA || CVTE_EN_CPMAC)
		if ( m_hcpd == INVALID_HANDLE_VALUE )
		{
			m_hcpd = CreateFile(CPD_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
			if ( m_hcpd == INVALID_HANDLE_VALUE )
			{
				printf("CPD Open Failed! err = %d\n", GetLastError());
				break;
			}
		}
#endif
    	CarplayInitParamInit(&param);

    	param.AudioTypeDeviceIDList[eAudioType_Main_Default] = CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT;
    	param.AudioTypeDeviceIDList[eAudioType_Main_Alert] = CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT;
    	param.AudioTypeDeviceIDList[eAudioType_Main_Media] = CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA;
    	param.AudioTypeDeviceIDList[eAudioType_Main_Telephony] = CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY;
    	param.AudioTypeDeviceIDList[eAudioType_Main_SpeechRecognition] = CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION;
        param.AudioTypeDeviceIDList[eAudioType_Main_Compatibility] =  CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY;
        param.AudioTypeDeviceIDList[eAudioType_Alt_Default] =  CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT;
        param.AudioTypeDeviceIDList[eAudioType_Alt_Compatibility] = CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY;
        param.AudioTypeDeviceIDList[eAudioType_MainHigh_Media] =  CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA;
    	param.AudioTypeDeviceIDList[eAudioType_Input] =  CVTE_DEF_AUDIO_DEVICE_ID_INPUT;
    	param.ServicePath = L"\\Windows\\CarPlayService.exe";
     	param.ClientPath = L"\\Windows\\CarPlayClient.exe";
//	 		param.ServicePath = L"\\GPS Card\\CarPlayService.exe";
//     	param.ClientPath = L"\\GPS Card\\CarPlayClient.exe";
//	    	param.ClientPath = L"\\USB Disk\\CarPlayService.exe";
//	        param.ClientPath = L"\\USB Disk\\CarPlayClient.exe";
//	 		param.ClientPath = L"\\ResidentFlash\\CarPlayService.exe";
//	        param.ClientPath = L"\\ResidentFlash\\CarPlayClient.exe";
        param.ControlStreamsCallback = ControlStreamsProc;
	    param.ControlStreamsContext = this;
    	param.UpdateStreamsCallback = UpdateStreamsProc;
    	param.UpdateStreamsContext = this;
    	param.ConnectStateCallback = ConnectStateProc;
    	param.ConnectStateContext = this;
		param.IAP2ConnectStateCallback = IAP2ConnectStateProc;
		param.IAP2ConnectStateContext = this;
    	param.ModesChangedCallback = ModesChangedProc;
    	param.ModesChangedContext = this;
    	param.RequestUICallback = RequestUIProc;
    	param.RequestUIContext = this;
    	param.DuckAudioCallback = DuckAudioProc;
    	param.DuckAudioContext = this;
    	param.UnduckAudioCallback = UnduckAudioProc;
    	param.UnduckAudioContext = this;
		param.DisableBluetoothCallback = DisableBluetoothProc;
		param.DisableBluetoothContext = this;
		param.StartLocationInformationCallback = StartLocationInformationProc;
		param.StartLocationInformationContext = this;
		param.StopLocationInformationCallback = StopLocationInformationProc;
		param.StopLocationInformationContext = this;
		param.TelephonyAudioFormatCallback = TelephonyAudioFormatProc;
		param.TelephonyAudioFormatContext = this;
		param.SpeechAudioFormatCallback = SpeechAudioFormatProc;
		param.SpeechAudioFormatContext = this;
        param.GetRightHandCallback = GetRightHandProc;
		param.GetRightHandContext = this;
		param.GetInitialModesCallback = GetInitialModesProc;
		param.GetInitialModesContext = this;
		param.GetBluetoothIDsCallback = GetBluetoothIDsProc;
		param.GetBluetoothIDsContext = this;
		param.DeviceInformationUpdateCallback = DeviceInformationUpdateProc;
		param.DeviceInformationUpdateContext = this;
		param.DeviceTimeUpdateCallback = DeviceTimeUpdateProc;
		param.DeviceTimeUpdateContext = this;
		param.DeviceUUIDUpdateCallback = DeviceUUIDUpdateProc;
		param.DeviceUUIDUpdateContext = this;
		param.DeviceSetMacCallback = DeviceSetMacProc;
		param.DeviceSetMacContext = this;
 		param.I2COpenCallback = IAP_I2C_Open;
 		param.I2CCloseCallback = IAP_I2C_Close;
 		param.I2CReadCallback = IAP_I2C_Read;
 		param.I2CWriteCallback = IAP_I2C_Write;
		param.IperfReportCallback = IperfReportProc;
		param.IperfReportContext = this;
		param.IperfResultsCallback = IperfResultsProc;
		param.IperfResultsContext = this;
		param.VideoWidth = CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH;
		param.VideoHeight = CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT;
		param.PhysicalWidth = CVTE_DEF_SCREEN_PHYSICALWIDTH;
		param.PhysicalHeight = CVTE_DEF_SCREEN_PHYSICALHEIGHT;
		param.GPSUARTPort = CVTE_DEF_CARPLAY_GPS_UART_PORT;
		param.LocationInfoEnable.ucInfo = 0;
		param.LocationInfoEnable.unInfo.GPGGA = 1;
		param.LocationInfoEnable.unInfo.GPRMC = 1;
		param.LocationInfoEnable.unInfo.GPGSV = 1;
		param.LocationInfoEnable.unInfo.PASCD = 1;
		param.FirmwareRevision = CVTE_DEF_APP_VERSION_STRING;
		param.HardwareRevision = CVTE_DEF_HARDWARE_VERSION;
		param.VehicleEngineType = eAPIEngineTypes_Gasoline;
		param.VehicleDisplayName = CVTE_DEF_VEHICLE_DISPLAY_NAME;
		param.languages = languages;
		param.languagenum = _countof(languages);
		param.curlanguageindex = 0;
		param.Manufacturer = CVTE_DEF_MANUFACTURER_DISPLAY_NAME;
		param.Model = CVTE_DEF_MODEL_DISPLAY_NAME;
		param.HiFiTouch = TRUE;
		param.IconVisible = TRUE;
		param.iconlabel = CVTE_DEF_CARPLAY_ICON_LABEL;
		param.prioriconpath = "\\Windows\\oemIcon.png";
		param.iconpath[eCarplayAPIIconType_120_120] = "\\Windows\\oemIcon_120_120.png";
		param.iconpath[eCarplayAPIIconType_180_180] = "\\Windows\\oemIcon_180_180.png";
		param.iconpath[eCarplayAPIIconType_256_256] = "\\Windows\\oemIcon_256_256.png";
		param.iconprerendered[eCarplayAPIIconType_120_120] = TRUE;
		param.iconprerendered[eCarplayAPIIconType_180_180] = TRUE;
		param.iconprerendered[eCarplayAPIIconType_256_256] = TRUE;
		param.LimitedUIElement.softKeyboard = 1;
		param.LimitedUIElement.softPhoneKeypad = 1;
		param.LimitedUIElement.nonMusicLists = 1;
		param.LimitedUIElement.musicLists = 1;
		param.LimitedUIElement.japanMaps = 1;

    	if (!CarplayAPIInit(&param))
    	{
    		printf("CarplayAPIInit Failed!\n");
            break;
    	}

    	err = CarplayAPISetStoreCaCallBack(StoreCaProc);
        if (err)
    	{
            printf("Set Store Ca CallBack Failed!\n");
    		break;
    	}

#if CVTE_EN_CARPLAY_LIMIT_UI
		CarplayAPILimitedUI(m_bLimitUI);
#endif
#if CVTE_EN_CARPLAY_NIGHTMODE
		CarplayAPISetNightMode(m_bOnNightMode);
#endif

        err = CarplayAPIGetDeviceID(m_strDeviceName, MAX_PATH);
    	if (err)
    	{
            printf("Get Device ID Failed!\n");
    		break;
    	}

		//read bluetooth mac address from registry:
		CReg reg;
		reg.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Bluetooth\\hci\\"));
		LPCTSTR mac=reg.ValueSZ(_T("mac"));
		if(mac)
		{
			AddBluetoothID(mac);
		}

		CheckMAC();

		ThreadStart();

        m_bInitialized = TRUE;
    }
}

void CCarplayAdapter::Deinitialize()
{
    if (m_bInitialized)
    {
        ThreadStop();
        CarplayAPIDeinit();
        m_bInitialized = FALSE;
    }
}

void CCarplayAdapter::ControlStreamsProc(PVOID context, BOOL inControlStreams)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->ControlStreams(inControlStreams);
	}
}

void CCarplayAdapter::UpdateStreamsProc(PVOID context,
										   BOOL inMainAudioStreams,
										   BOOL inAltAudioStreams,
										   BOOL inMainScreenStreams )
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->UpdateStreams(inMainAudioStreams, inAltAudioStreams, inMainScreenStreams);
	}
}

void CCarplayAdapter::ConnectStateProc(PVOID context,BOOL inConnect)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->ConnectState(inConnect);
	}
}

void CCarplayAdapter::IAP2ConnectStateProc(PVOID context,BOOL inConnect)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->IAP2ConnectState(inConnect);
	}
}

void CCarplayAdapter::ModesChangedProc(PVOID context, const AirPlayModeState * inState)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->ModesChanged(inState);
	}
}

void CCarplayAdapter::RequestUIProc(PVOID context, const char* inURL)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->RequestUI(inURL);
	}
}

void CCarplayAdapter::DuckAudioProc(PVOID context,
									   double inDurationSecs,
									   double inVolume)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->DuckAudio(inDurationSecs, inVolume);
	}
}

void CCarplayAdapter::UnduckAudioProc(PVOID context, double inDurationSecs)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->UnduckAudio(inDurationSecs);
	}
}

void CCarplayAdapter::DisableBluetoothProc(PVOID context, const unsigned char mac[ 6 ])
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->DisableBluetooth(mac);
	}
}

void CCarplayAdapter::StartLocationInformationProc(PVOID context, const LocationInfo_t* inLocationInfo)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->StartLocationInformation(inLocationInfo);
	}
}

void CCarplayAdapter::StopLocationInformationProc(PVOID context)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->StopLocationInformation();
	}
}

void CCarplayAdapter::TelephonyAudioFormatProc(PVOID context, AirPlayAudioFormat inFormat)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->TelephonyAudioFormat(inFormat);
	}
}
void CCarplayAdapter::SpeechAudioFormatProc(PVOID context, AirPlayAudioFormat inFormat)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->SpeechAudioFormat(inFormat);
	}
}
void CCarplayAdapter::GetRightHandProc(PVOID context, BOOL* outRightHand)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->GetRightHand(outRightHand);
	}
}

void CCarplayAdapter::GetInitialModesProc(PVOID context, AirPlayModeChanges * outInitialModes)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->GetInitialModes(outInitialModes);
	}
}

void CCarplayAdapter::GetBluetoothIDsProc(PVOID context, unsigned char ** outMAC, size_t inMaxMACCount, size_t* outMACCount)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->GetBluetoothIDs(outMAC, inMaxMACCount, outMACCount);
	}
}

BOOL CCarplayAdapter::IsFileExists(LPCTSTR   lpszFileName)
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

OSStatus CCarplayAdapter::StoreCa( const wchar_t * path, const void * buf, size_t bufsize )
{
	OSStatus err = kNoErr;
	FILE* fp_pub_key = NULL;
	BOOL bRet;
	int iRet;

	err = _wfopen_s(&fp_pub_key, path, L"wb");
	if(0 != err)
	{
		goto exit;
	}
	iRet = fwrite(buf,bufsize,1,fp_pub_key);
	if (iRet != 1)
	{
		err = GetLastError();
		goto exit;
	}
	bRet = SetFileAttributes(path,FILE_ATTRIBUTE_HIDDEN);
	if (!bRet)
	{
		err = GetLastError();
		goto exit;
	}
	
exit:
	if (fp_pub_key != NULL)
	{
		fclose(fp_pub_key);
	}
	return err;
}

OSStatus CCarplayAdapter::StoreCaProc( const void * buf, size_t bufsize )
{
	OSStatus err = kNoErr;
	int iRet;
	if (!m_bStoreCa)
	{
		RETAILMSG(1,(_T("####m_bStoreCa = FALSE#####\n")));
		m_bStoreCa = TRUE;
		goto exit;
	}
#if CVTE_EN_CPCA
	if (!DeviceIoControl(m_hcpd, IOCTL_CSECURE_SET_CA, (LPVOID)buf, bufsize, NULL, 0, NULL, NULL))
	{
		err = GetLastError();
		RETAILMSG(1,(_T("\n####Set CA Failed! err = %d.##### \n"), err));
		goto exit;
	}
#endif
	wchar_t strDestCA[MAX_PATH];
	wchar_t strDestCA2[MAX_PATH];

	iRet = swprintf_s(strDestCA,MAX_PATH,_T("%s\\%s.ca"), m_DestCAPath1, m_pInstance->m_strDeviceName);
	if (iRet == -1)
	{
		err = GetLastError();
		goto exit;
	}

	iRet = swprintf_s(strDestCA2,MAX_PATH,_T("%s\\%s.ca"), m_DestCAPath2, m_pInstance->m_strDeviceName);
	if (iRet == -1)
	{
		err = GetLastError();
		goto exit;
	}

	err = StoreCa( strDestCA, buf, bufsize );
	if (err)
	{
		goto exit;
	}

	err = StoreCa( strDestCA2, buf, bufsize );
	if (err)
	{
		goto exit;
	}
	
exit:
	return err;
}

void CCarplayAdapter::IperfReportProc( PVOID context, TCHAR* report )
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->IperfReport(report);
	}
}

void CCarplayAdapter::IperfResultsProc( PVOID context, TCHAR* results )
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->IperfResults(results);
	}
}

void CCarplayAdapter::DeviceInformationUpdateProc(PVOID context, LPCWSTR inDeviceName)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->DeviceInformationUpdate(inDeviceName);
	}
}

void CCarplayAdapter::DeviceTimeUpdateProc(PVOID context,
  	                                    unsigned long long inSecondsSinceReferenceDate,
  	                                    short inTimeZoneOffsetMinutes,
  	                                    char inDaylightSavingsOffsetMinutes )
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->DeviceTimeUpdate(inSecondsSinceReferenceDate,
			                   inTimeZoneOffsetMinutes,
			                   inDaylightSavingsOffsetMinutes);
	}
}

void CCarplayAdapter::DeviceUUIDUpdateProc(PVOID context, LPCWSTR inUUID)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		pAdapter->DeviceUUIDUpdate(inUUID);
	}
}

BOOL CCarplayAdapter::DeviceSetMacProc(PVOID context,unsigned char* inMacData,size_t inMacDataLen)
{
	BOOL bRet = FALSE;
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)context;
	if (pAdapter != NULL)
	{
		bRet = pAdapter->DeviceSetMac(inMacData, inMacDataLen);
	}
	return bRet;
}

OSStatus CCarplayAdapter::InstallCA( const wchar_t * path, BOOL* active )
{
#if CVTE_EN_CPCA
	OSStatus err = kNoErr;
	unsigned char buf[2560] = {0};
	size_t bufsize = sizeof(buf);

	if (!DeviceIoControl(m_hcpd, IOCTL_CSECURE_GET_CA, NULL, 0, buf, bufsize, NULL, NULL))
	{
		err = GetLastError();
		RETAILMSG(1,(_T("\n####Get CA Failed! err = %d.##### \n"), err));
		goto exit;
	}
	m_bStoreCa = FALSE;
	err = CarplayAPIInstallCA(buf, bufsize);
	if (err)
	{
		RETAILMSG(1,(_T("\n####Install CA file Failed!##### \n")));
		goto exit;
	}
	m_bStoreCa = TRUE;
	err = CarplayAPIIsActivated(active);
	if (err)
	{
		goto exit;
	}
#else
	FILE* fp;
	unsigned char *ca_input;
	OSStatus err = kNoErr;
	if ( path == NULL )
	{
		goto exit;
	}
	if ( active == NULL )
	{
		goto exit;
	}
	if (IsFileExists(path))
	{
		if(0 == _wfopen_s(&fp,path, L"rb"))
		{
			size_t fileSize=0;
			fseek (fp , 0 , SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			fileSize = min(2560,fileSize);
			ca_input =new unsigned char[fileSize];
			size_t readsize = fread(ca_input,fileSize,1,fp);
			fclose(fp);
			m_bStoreCa = FALSE;
			err = CarplayAPIInstallCA(ca_input, fileSize);
			if (err)
			{
				RETAILMSG(1,(_T("\n####Install CA file Failed! %s##### \n"),path));
			}
			delete ca_input;
		}
		else
		{
			RETAILMSG(1,(_T("\n####File open failed %s##### \n"),path));
		}
	}
	err = CarplayAPIIsActivated(active);
	RETAILMSG(1,(_T("\n####InstallCA %s CarplayAPIIsActivated err =%d,bActivated=%d##### \n"),path,err,*active));
#endif
exit:
#if CVTE_EN_CPCA
	if (err != kNoErr)
	{
		m_bStoreCa = TRUE;
	}
#endif
	return err;
}

#if CVTE_EN_CPCA
OSStatus CCarplayAdapter::InstallCAToCPCA( const wchar_t * path )
{
	FILE* fp;
	unsigned char *ca_input;
	OSStatus err = kNoErr;

	if ( path == NULL )
	{
		err = kParamErr;
		goto exit;
	}
	if (IsFileExists(path))
	{
		err = _wfopen_s(&fp,path, L"rb");
		if( kNoErr == err )
		{
			size_t fileSize=0;
			fseek (fp , 0 , SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			fileSize = min(2560,fileSize);
			ca_input =new unsigned char[fileSize];
			size_t readsize = fread(ca_input,fileSize,1,fp);
			fclose(fp);
			if (!DeviceIoControl(m_hcpd, IOCTL_CSECURE_SET_CA, ca_input, fileSize, NULL, 0, NULL, NULL))
			{
				err = GetLastError();
				RETAILMSG(1,(_T("\n####Set CA Failed! err = %d.##### \n"), err));
				goto exit;
			}
			delete ca_input;
		}
		else
		{
			RETAILMSG(1,(_T("\n####File open failed %s##### \n"),path));
		}
	}
	else
	{
		err = kPathErr;
	}
exit:
	return err;
}

OSStatus CCarplayAdapter::DeleteCAFromCPCA()
{
	OSStatus err = kNoErr;

	if (!DeviceIoControl(m_hcpd, IOCTL_CSECURE_DELETE_CA, NULL, 0, NULL, 0, NULL, NULL))
	{
		err = GetLastError();
		RETAILMSG(1,(_T("\n####Delete CA Failed! err = %d##### \n"), err));
	}
	return err;
}
#endif

DWORD CCarplayAdapter::ThreadRun( )
{
#if !CVTE_EN_CPCA
	wchar_t strDestCA[MAX_PATH];
	wchar_t strDestCA2[MAX_PATH];
#endif
	wchar_t strSrcCA[MAX_PATH];
	wchar_t strSrcCA2[MAX_PATH];
	wchar_t strDeviceNameFile[MAX_PATH];
	OSStatus err = kNoErr;
    DWORD dwRetry = 10;
	int iRet;
	BOOL bActivated = FALSE;
	BOOL bUSBDevice = FALSE;

	if (!m_bServerInit)
	{
		err = CarplayAPIWaitServerInit(INFINITE, &m_bServerInit);
		if (err || !m_bServerInit)
		{
			printf("Wait Server Init failed! err = %d\n", err);
			return -1;
		}
	}
#if !CVTE_EN_CPCA
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
#endif
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

#if CVTE_EN_CPCA
	InstallCA( NULL, &bActivated );
#else
	if(IsFileExists(strDestCA) || IsFileExists(strDestCA2))
	{
		err = InstallCA( strDestCA, &bActivated );
		if (err)
		{
			goto exit;
		}
		if (!bActivated)
		{
			err = InstallCA( strDestCA2, &bActivated );
			if (err)
			{
				goto exit;
			}
			if (bActivated)
			{
				CopyFile(strDestCA2, strDestCA, FALSE);
				SetFileAttributes(strDestCA,FILE_ATTRIBUTE_HIDDEN);
			}
		}
		else if (!IsFileExists(strDestCA2))
		{
			CopyFile(strDestCA, strDestCA2, FALSE);
			SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_HIDDEN);
		}
	}
	else
	{
		RETAILMSG(1,(_T("\n####File NOT found %s and %s##### \n"),strDestCA, strDestCA2));
	}
#endif

	if (!bActivated)
	{
		//output devicename.txt
		swprintf_s(strDeviceNameFile,MAX_PATH,_T("\\USB Disk\\%s.txt"),m_strDeviceName);
		if(!IsFileExists(strDeviceNameFile))
		{
			FILE	*file = NULL;
			printf("###########output device name file#########\r\n");
			if ( 0==_wfopen_s(&file, strDeviceNameFile, L"wb") )
			{
				fwrite(m_strDeviceName,wcslen(m_strDeviceName)*sizeof(wchar_t),1,file);
				fclose(file);
			}
		}
		/////////////////////////////////

		if (!bUSBDevice)
		{
		    dwRetry = 10;
		    do
		    {
			    err = CarplayAPISwitchDevice(m_CarplayUSBPort);
		        if (err != kNoErr)
		        {
		            Sleep(100);
		        }
		        else
		        {
					bUSBDevice = TRUE;
		            break;
		        }
		    }while(--dwRetry);
		}

		if (m_DeviceActivateCallback)
		{
			m_DeviceActivateCallback(m_DeviceActivateCallbackContext, FALSE);
		}
		while(((err = CarplayAPIIsActivated(&bActivated)) == 0) && (!bActivated))
		{
			#if CVTE_EN_CPCA
			if(IsFileExists(strSrcCA) || IsFileExists(strSrcCA2))
			#else
			if(IsFileExists(strDestCA) || IsFileExists(strDestCA2) || IsFileExists(strSrcCA) || IsFileExists(strSrcCA2))
			#endif
			{
				if (IsFileExists(strSrcCA))
				{
					#if CVTE_EN_CPCA
					InstallCAToCPCA(strSrcCA);
					#else
					if (IsFileExists(strDestCA))
					{
						SetFileAttributes(strDestCA,FILE_ATTRIBUTE_NORMAL);
					}
					CopyFile(strSrcCA, strDestCA, FALSE);
					SetFileAttributes(strDestCA,FILE_ATTRIBUTE_HIDDEN);
					if (IsFileExists(strDestCA2))
					{
						SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_NORMAL);
					}
					CopyFile(strSrcCA, strDestCA2, FALSE);
					SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_HIDDEN);
					#endif
				}
				if (IsFileExists(strSrcCA2))
				{
					#if CVTE_EN_CPCA
					InstallCAToCPCA(strSrcCA2);
					#else
					if (IsFileExists(strDestCA))
					{
						SetFileAttributes(strDestCA,FILE_ATTRIBUTE_NORMAL);
					}
					CopyFile(strSrcCA2, strDestCA, FALSE);
					SetFileAttributes(strDestCA,FILE_ATTRIBUTE_HIDDEN);
					if (IsFileExists(strDestCA2))
					{
						SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_NORMAL);
					}
					CopyFile(strSrcCA2, strDestCA2, FALSE);
					SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_HIDDEN);
					#endif
				}
				#if CVTE_EN_CPCA
				InstallCA( NULL, &bActivated );
				#else
				err = InstallCA( strDestCA, &bActivated );
				if (err)
				{
					goto exit;
				}
				if (!bActivated)
				{
					err = InstallCA( strDestCA2, &bActivated );
					if (err)
					{
						goto exit;
					}
					if (bActivated)
					{
						CopyFile(strDestCA2, strDestCA, FALSE);
						SetFileAttributes(strDestCA,FILE_ATTRIBUTE_HIDDEN);
					}
				}
				else if (!IsFileExists(strDestCA2))
				{
					CopyFile(strDestCA, strDestCA2, FALSE);
					SetFileAttributes(strDestCA2,FILE_ATTRIBUTE_HIDDEN);
				}
				#endif
				if (!bActivated)
				{
					Sleep(3000);
				}
				else
				{
					break;
				}
			}
			else
			{
				RETAILMSG(1,(_T("\n####CA file NOT FOUND! %s or %s##### \nPlease insert correct SD card!\n"),strSrcCA, strSrcCA2));
				Sleep(3000);
			}
		}

		if (m_DeviceActivateCallback)
		{
			m_DeviceActivateCallback(m_DeviceActivateCallbackContext, TRUE);
		}
	}
#if CVTE_EN_CPMAC
	if (m_SetMACCallback && !m_bSetMAC)
	{
		if (CheckMACFromFile())
		{
			CheckMAC();
		}
	}

	if (m_SetMACCallback && !m_bSetMAC)
	{
		m_SetMACCallback(m_DeviceActivateCallbackContext, FALSE);
		
		while (!m_bSetMAC)
		{
			if (!bUSBDevice)
			{
			    dwRetry = 10;
			    do
			    {
				    err = CarplayAPISwitchDevice(m_CarplayUSBPort);
			        if (err != kNoErr)
			        {
			            Sleep(100);
			        }
			        else
			        {
						bUSBDevice = TRUE;
			            break;
			        }
			    }while(--dwRetry);
			}
			DWORD waitobj;
			do
			{
				waitobj = WaitForSingleObject(m_hSetMACEvent,1000);
				if (waitobj == WAIT_OBJECT_0)
				{
					break;
				}
			}while(!CheckMACFromFile());
			CheckMAC();
		}

		m_SetMACCallback(m_DeviceActivateCallbackContext, TRUE);
	}
#endif
exit:
	if (bUSBDevice)
	{
		dwRetry = 10;
	    do
	    {
		    err = CarplayAPISwitchHost(m_CarplayUSBPort);
	        if (err != kNoErr)
	        {
	            Sleep(100);
	        }
	        else
	        {
	            break;
	        }
	    }while(--dwRetry);
	}
	if (err)
	{
		RETAILMSG(1,(_T("\n####Active err=%d, Ret=%d##### \n"),err,bActivated));
	}
	else
	{
		RETAILMSG(1,(_T("\n####Active err=%d, Ret=%d##### \n"),err,bActivated));
	}
	ExitThread(1);
	return 1;
}

void CCarplayAdapter::ControlStreams(BOOL inControlStreams)
{
	//printf("[%s] : inControlStreams = %s\n", __FUNCTION__, inControlStreams ? "TRUE": "FALSE");
    if (m_ControlStreams != inControlStreams)
    {
        m_ControlStreams = inControlStreams;
		if (m_ControlStreams)
		{
			if ( m_ConnectTimer != 0 )
			{
				timeKillEvent(m_ConnectTimer);
				m_ConnectTimer = 0;
			}
		}
		//UpdateSessionAddress();
        ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_CONTROL_STATUS,m_ControlStreams);
    }
}

void CCarplayAdapter::UpdateStreams(BOOL inMainAudioStreams,
									   BOOL inAltAudioStreams,
									   BOOL inMainScreenStreams )
{
// 	printf("[%s] : inMainAudioStreams = %s, inAltAudioStreams = %s, inMainScreenStreams = %s\n",
// 		__FUNCTION__,
// 		inMainAudioStreams ? "TRUE": "FALSE",
// 		inAltAudioStreams ? "TRUE": "FALSE",
// 		inMainScreenStreams ? "TRUE": "FALSE");
    if (!m_bTestMode)
    {
        if (m_MainAudioStreams != inMainAudioStreams)
        {
            m_MainAudioStreams = inMainAudioStreams;
            ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_MAIN_AUDIO_STATUS,m_MainAudioStreams);
        }
        if (m_AltAudioStreams != inAltAudioStreams)
        {
            m_AltAudioStreams = inAltAudioStreams;
            ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_ALT_AUDIO_STATUS,m_AltAudioStreams);
        }
        if (m_MainScreenStreams != inMainScreenStreams)
        {
            m_MainScreenStreams = inMainScreenStreams;
            ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_SCREEN_STATUS,m_MainScreenStreams);
        }
    }
}

void CCarplayAdapter::ConnectState(BOOL inConnect)
{
	printf("[%s] : inConnect = %s\n", __FUNCTION__, inConnect ? "TRUE": "FALSE");
    if (!m_bTestMode)
    {
        if (inConnect)
        {
            ::SendMessage(m_hMainWnd,WM_MSG_CARPLAY_ARRIVE,0,0);
			if ( (CVTE_DEF_CARPLAY_RESTART_TIME_MS > 0) && m_ConnectTimer == 0)
			{
				m_ConnectTimer = timeSetEvent( CVTE_DEF_CARPLAY_RESTART_TIME_MS, 1, ConnectTimerProc, (DWORD)this, /*TIME_ONESHOT*/TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
			}
        }
        else
        {
			if(m_ControlStreams)
			{
				m_bUserSelectAudioSRC = FALSE;
				m_bUserSelectNativeUI = FALSE;

				//store carplay last modes:
				m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
				m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI=m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;

				RETAILMSG(1,(_T("##CCarplayAdapter::ConnectState: idCarPlayLastSRC=0x%x, idCarPlayLastUI=0x%x\r\n"),
					m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC,m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI));
			}

			//reset status:
			AirPlayModeState state;
			AirPlayModeStateInit(&state);
			state.screen = kAirPlayEntity_Accessory;
			state.mainAudio = kAirPlayEntity_Accessory;
			state.phoneCall = kAirPlayEntity_NotApplicable;
			state.speech.entity = kAirPlayEntity_NotApplicable;
			state.speech.mode = kAirPlaySpeechMode_NotApplicable;
			state.turnByTurn = kAirPlayEntity_NotApplicable;
			::SendMessage(m_hMainWnd,WM_MSG_CARPLAY_REMOVE,0,0);
			ModesChanged(&state);
			UnduckAudio(0);
			//release timer:
			if ( m_ConnectTimer != 0 )
			{
				timeKillEvent(m_ConnectTimer);
				m_ConnectTimer = 0;
			}
        }
    }
}

void CCarplayAdapter::IAP2ConnectState(BOOL inConnect)
{
	printf("[%s] : inConnect = %s\n",
		__FUNCTION__, inConnect ? "TRUE": "FALSE");
	::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_IAP2_CONNECT_STATUS,inConnect);
}

void CCarplayAdapter::ModesChanged(const AirPlayModeState * inState)
{
	/*
	printf("[%s] : a mode has been changed by the Apple device, please updates the accessory's current mode\n",
		__FUNCTION__);
	printf("[%s] : MainScreen's entity is %s \n",
		__FUNCTION__, (inState->screen == kAirPlayEntity_Controller) ? "Controller" : (inState->screen == kAirPlayEntity_Accessory) ? "Accessory": "NotApplicable");
	printf("[%s] : MainAudio's entity is %s \n",
		__FUNCTION__, (inState->mainAudio == kAirPlayEntity_Controller) ? "Controller" : (inState->mainAudio == kAirPlayEntity_Accessory) ? "Accessory": "NotApplicable");
	printf("[%s] : PhoneCall's entity is %s \n",
		__FUNCTION__, (inState->phoneCall == kAirPlayEntity_Controller) ? "Controller" : (inState->phoneCall == kAirPlayEntity_Accessory) ? "Accessory": "NotApplicable");
	printf("[%s] : Speech's entity is %s \n",
		__FUNCTION__, (inState->speech.entity == kAirPlayEntity_Controller) ? "Controller" : (inState->speech.entity == kAirPlayEntity_Accessory) ? "Accessory": "NotApplicable");
	printf("[%s] : Speech's mode is %s \n",
		__FUNCTION__, (inState->speech.mode == kAirPlaySpeechMode_None) ? "None" : (inState->speech.mode == kAirPlaySpeechMode_Speaking) ? "Speaking": (inState->speech.mode == kAirPlaySpeechMode_Recognizing) ? "Recognizing": "NotApplicable");
	printf("[%s] : TurnByTurn's entity is %s \n",
		__FUNCTION__, (inState->turnByTurn == kAirPlayEntity_Controller) ? "Controller" : (inState->turnByTurn == kAirPlayEntity_Accessory) ? "Accessory": "NotApplicable");
	*/
    if (!m_bTestMode)
    {
        memcpy(&m_carplayModeStatus, inState, sizeof(m_carplayModeStatus));
        ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_MODESCHANGED_STATUS,0);
    }
}
void CCarplayAdapter::RequestUI(const char* inURL)
{
// 	printf("[%s] : the Apple device to ask for the accessory UI to be shown, URL : %s\n",
// 		__FUNCTION__, inURL ? inURL : "null");
    if (!m_bTestMode)
    {
	    ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_REQUESTUI_STATUS,0);
    }
}

void CCarplayAdapter::DuckAudio(double inDurationSecs, double inVolume)
{
// 	printf("[%s] : Ramps volume down to %f in %f Sec%s\n",
// 		__FUNCTION__, inVolume, inDurationSecs, (inDurationSecs>1.0) ? "s" : "");
	DWORD vol=((DWORD(inVolume*100.0)));
	DWORD time_out=(DWORD(inDurationSecs*1000.0));
    ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_DUCK_STATUS,vol);
	//RETAILMSG(1, ((L"+++DuckAudio: %x %x\r\n"),vol,time_out));
}

void CCarplayAdapter::UnduckAudio(double inDurationSecs)
{
// 	printf("[%s] : Ramps back to the pre-duck level in %f Sec%s\n",
// 		__FUNCTION__, inDurationSecs, (inDurationSecs>1.0)?"s":"");
    ::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_UNDUCK_STATUS,0);
}

wchar_t* CCarplayAdapter::GetDeviceName()
{
    return m_strDeviceName;
}

void CCarplayAdapter::SetTestMode(BOOL inTestMode)
{
	OSStatus err = kNoErr;
	if (m_bTestMode != inTestMode)
	{
		err = CarplayAPITestMode(inTestMode);
		if (err == kNoErr)
		{
			m_bTestMode = inTestMode;
		}
	}
}

BOOL CCarplayAdapter::GetTestMode()
{
	return m_bTestMode;
}

const AirPlayModeState* CCarplayAdapter::GetModeState()
{
    return &m_carplayModeStatus;
}

OSStatus CCarplayAdapter::SetUSBTestMode(CarplayAPITestModeType inTestMode)
{
    OSStatus err;
    err = CarplayAPISetTestMode(m_CarplayUSBPort, inTestMode);
    return err;
}

char * CCarplayAdapter::MACAddressToCString( const unsigned char mac[ 6 ], char *outStr, BOOL bColon )
{
	const unsigned char *		src;
	const unsigned char *		end;
	char *						dst;
	unsigned char				b;

	src = (const unsigned char *) mac;
	end = src + 6;
	dst = outStr;
	while( src < end )
	{
		if( bColon && dst != outStr ) *dst++ = ':';
		b = *src++;
		*dst++ = HexDigitsUppercase[ b >> 4 ];
		*dst++ = HexDigitsUppercase[ b & 0xF ];
	}
	*dst = '\0';
	return( outStr );
}

#define isdigit_safe( X )		isdigit(  ( (unsigned char)( (X) & 0xFF ) ) )
#define isxdigit_safe( X )		isxdigit( ( (unsigned char)( (X) & 0xFF ) ) )
#define tolower_safe( X )		tolower(  ( (unsigned char)( (X) & 0xFF ) ) )

OSStatus CCarplayAdapter::CStringToMACAddress( const void *inText, size_t inTextSize, size_t inAddrSize, void *outAddr )
{
	OSStatus			err;
	const char *		src;
	const char *		end;
	int					i;
	int					x;
	char				c;
	unsigned char *		dst;

	if( inTextSize == -1 ) inTextSize = strlen( (const char *) inText );
	src = (const char *) inText;
	end = src + inTextSize;
	dst = (unsigned char *) outAddr;

	while( inAddrSize-- > 0 )
	{
		x = 0;
		i = 0;
		while( ( i < 2 ) && ( src < end ) )
		{
			c = *src++;
			if(      isdigit_safe(  c ) ) { x = ( x * 16 )      +               ( c   - '0' ); ++i; }
			else if( isxdigit_safe( c ) ) { x = ( x * 16 ) + 10 + ( tolower_safe( c ) - 'a' ); ++i; }
			else if( ( i != 0 ) || ( ( c != ':' ) && ( c != '-' ) && ( c != ' ' ) ) ) break;
		}
		if( i == 0 )
		{
			err = kMalformedErr;
			goto exit;
		}
		if( dst ) *dst++ = (unsigned char) x;
	}
	err = kNoErr;

exit:
	return( err );
}

void CCarplayAdapter::DisableBluetooth(const unsigned char mac[ 6 ])
{
	char cstr[ 32 ];
	MACAddressToCString( mac, cstr );
	printf("[%s] : Disable Bluetooth MAC : %s\n", __FUNCTION__, cstr);
	::PostMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_DISABLE_BLUETOOTH,NULL);

	//if bluetooth phone call is active, un-borrow screen and audio
	if(m_bDealIniBluetoothPhone)
	{
		m_bDealIniBluetoothPhone = FALSE;

		if(!m_bOnReverseMode && !m_bShowAccOnWarning)
		{
			CarplayAPIUnborrowScreen(NULL);
		}
		CarplayAPIUnborrowMainAudio(NULL);
		CarplayAPIChangeAppState(kAirPlaySpeechMode_NotApplicable,kAirPlayTriState_False,kAirPlayTriState_NotApplicable,"DisableBluetooth");
		if(!m_bOnReverseMode && !m_bShowAccOnWarning)
		{
			CarplayAPIRequestUI("");
		}
	}
}

void CCarplayAdapter::StartLocationInformation(const LocationInfo_t* inLocationInfo)
{
	m_locationinfo.StartLocationInformation(inLocationInfo);
}

void CCarplayAdapter::StopLocationInformation()
{
	m_locationinfo.StopLocationInformation();
}

void CCarplayAdapter::TelephonyAudioFormat(AirPlayAudioFormat inFormat)
{
	//printf("[%s] : Format = %llu\n", __FUNCTION__, inFormat);
	::PostMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_PHONE_AUDIO_FORMAT,(LPARAM)inFormat);
}

void CCarplayAdapter::SpeechAudioFormat(AirPlayAudioFormat inFormat)
{
	//printf("[%s] : Format = %llu\n", __FUNCTION__, inFormat);
	::SendMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_SIRI_AUDIO_FORMAT,(LPARAM)inFormat);
}

void CCarplayAdapter::GetRightHand(BOOL* outRightHand)
{
	if (outRightHand)
	{
		*outRightHand = !!m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType;
	}
}

void CCarplayAdapter::GetInitialModes(AirPlayModeChanges * outInitialModes)
{
	//printf("[%s] \n", __FUNCTION__);

	if (outInitialModes)
	{
		//screen:
		if(m_idPowerState != SYS_POWER_ON)
		{
			printf("[%s] screen 1 \n", __FUNCTION__);
			outInitialModes->screen.type = kAirPlayTransferType_Borrow;
			outInitialModes->screen.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->screen.takeConstraint = kAirPlayConstraint_Never;
			outInitialModes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_Never;
			m_bNeedShowTopBanner = FALSE;
		}
		else if(m_bOnReverseMode || m_bShowAccOnWarning)
		{
			printf("[%s] screen 2 \n", __FUNCTION__);
			outInitialModes->screen.type = kAirPlayTransferType_Borrow;
			outInitialModes->screen.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->screen.takeConstraint = kAirPlayConstraint_Never;
			outInitialModes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_Never;
			m_bNeedShowTopBanner = FALSE;
		}
		else if(m_bBluetoothPhoneActive)
		{
			printf("[%s] screen 3 \n", __FUNCTION__);
			outInitialModes->screen.type = kAirPlayTransferType_Borrow;
			outInitialModes->screen.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->screen.takeConstraint = kAirPlayConstraint_Never;
			outInitialModes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_Never;
			m_bNeedShowTopBanner = FALSE;
		}
		else if(m_bFirstTimeConnect)
		{
			printf("[%s] screen 4 \n", __FUNCTION__);
			outInitialModes->screen.type = kAirPlayTransferType_Take;
			outInitialModes->screen.priority = kAirPlayTransferPriority_NiceToHave;
			outInitialModes->screen.takeConstraint = kAirPlayConstraint_Anytime;
			outInitialModes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
			m_bNeedShowTopBanner = FALSE;
		}
		else if(m_bUserSelectNativeUI)
		{
			printf("[%s] screen 5 \n", __FUNCTION__);
			outInitialModes->screen.type = kAirPlayTransferType_Take;
			outInitialModes->screen.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->screen.takeConstraint = kAirPlayConstraint_UserInitiated;
			outInitialModes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
			m_bNeedShowTopBanner = TRUE;
		}
		else if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI) == UI_CLASS_CARPLAY)
		{
			printf("[%s] screen 6 \n", __FUNCTION__);
			outInitialModes->screen.type = kAirPlayTransferType_Take;
			outInitialModes->screen.priority = kAirPlayTransferPriority_NiceToHave;
			outInitialModes->screen.takeConstraint = kAirPlayConstraint_Anytime;
			outInitialModes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
			m_bNeedShowTopBanner = FALSE;
		}
		else
		{
			printf("[%s] screen 7 \n", __FUNCTION__);
			outInitialModes->screen.type = kAirPlayTransferType_Take;
			outInitialModes->screen.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->screen.takeConstraint = kAirPlayConstraint_UserInitiated;
			outInitialModes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
			m_bNeedShowTopBanner = TRUE;
		}

		//audio:
		if(m_idPowerState != SYS_POWER_ON)
		{
			printf("[%s] audio 1 \n", __FUNCTION__);
			outInitialModes->mainAudio.type = kAirPlayTransferType_Borrow;
			outInitialModes->mainAudio.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->mainAudio.takeConstraint = kAirPlayConstraint_Never;
			outInitialModes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_Never;
		}
		else if(m_bBluetoothPhoneActive /*|| m_bShowAccOnWarning*/)
		{
			printf("[%s] audio 2 \n", __FUNCTION__);
			outInitialModes->mainAudio.type = kAirPlayTransferType_Borrow;
			outInitialModes->mainAudio.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->mainAudio.takeConstraint = kAirPlayConstraint_Never;
			outInitialModes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_Never;
		}
		else if(m_bFirstTimeConnect)
		{
			printf("[%s] audio 3 \n", __FUNCTION__);
			outInitialModes->mainAudio.type = kAirPlayTransferType_Take;
			outInitialModes->mainAudio.priority = kAirPlayTransferPriority_NiceToHave;
			outInitialModes->mainAudio.takeConstraint = kAirPlayConstraint_Anytime;
			outInitialModes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
		}
		else if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_BT)
		{
			printf("[%s] audio 4 \n", __FUNCTION__);
			outInitialModes->mainAudio.type = kAirPlayTransferType_Take;
			outInitialModes->mainAudio.priority = kAirPlayTransferPriority_NiceToHave;
			outInitialModes->mainAudio.takeConstraint = kAirPlayConstraint_Anytime;
			outInitialModes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
		}
		else if(m_bUserSelectAudioSRC)
		{
			printf("[%s] audio 5 \n", __FUNCTION__);
			outInitialModes->mainAudio.type = kAirPlayTransferType_Take;
			outInitialModes->mainAudio.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->mainAudio.takeConstraint = kAirPlayConstraint_UserInitiated;
			outInitialModes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
		}
		else if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC) == UI_CLASS_CARPLAY)
		{
			printf("[%s] audio 6 \n", __FUNCTION__);
			outInitialModes->mainAudio.type = kAirPlayTransferType_Take;
			outInitialModes->mainAudio.priority = kAirPlayTransferPriority_NiceToHave;
			outInitialModes->mainAudio.takeConstraint = kAirPlayConstraint_Anytime;
			outInitialModes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
		}
		else
		{
			printf("[%s] audio 7 \n", __FUNCTION__);
			outInitialModes->mainAudio.type = kAirPlayTransferType_Take;
			outInitialModes->mainAudio.priority = kAirPlayTransferPriority_UserInitiated;
			outInitialModes->mainAudio.takeConstraint = kAirPlayConstraint_UserInitiated;
			outInitialModes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_Anytime;
		}

		outInitialModes->speech = kAirPlaySpeechMode_None;
		outInitialModes->phoneCall = m_bBluetoothPhoneActive?kAirPlayTriState_True:kAirPlayTriState_False;
		outInitialModes->turnByTurn = m_bUserSelectNativeNavi?kAirPlayTriState_True:kAirPlayTriState_False;

		memcpy(&m_InitialModes, outInitialModes,sizeof(AirPlayModeChanges));
		m_bDealIniBluetoothPhone = (outInitialModes->phoneCall == kAirPlayTriState_True);

		/*
		printf("outInitialModes->idCurSRC = 0x%x \n", m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);

		printf("outInitialModes->screen.type = %d \n", outInitialModes->screen.type);
		printf("outInitialModes->screen.priority = %d \n", outInitialModes->screen.priority);
		printf("outInitialModes->screen.takeConstraint = %d \n", outInitialModes->screen.takeConstraint);
		printf("outInitialModes->screen.borrowOrUnborrowConstraint = %d \n", outInitialModes->screen.borrowOrUnborrowConstraint);
		printf("outInitialModes->mainAudio.type = %d \n", outInitialModes->mainAudio.type);
		printf("outInitialModes->mainAudio.priority = %d \n", outInitialModes->mainAudio.priority);
		printf("outInitialModes->mainAudio.takeConstraint = %d \n", outInitialModes->mainAudio.takeConstraint);
		printf("outInitialModes->mainAudio.borrowOrUnborrowConstraint = %d \n", outInitialModes->mainAudio.borrowOrUnborrowConstraint);
		printf("outInitialModes->turnByTurn = %d \n", outInitialModes->turnByTurn);
		printf("outInitialModes->phoneCall = %d \n", outInitialModes->phoneCall);
		*/
	}
}
void CCarplayAdapter::AddBluetoothID(const unsigned char mac[6])
{
	unsigned char * newmac;
	if ( mac == NULL )
	{
		return;
	}
	m_maclock.Lock();
	newmac = new unsigned char[6];
	memcpy(newmac, mac, 6);
	m_maclist.push_back(newmac);
	m_maclock.Unlock();
	printf("AddBluetoothID: %X %X %X %X %X %X \n", newmac[0],newmac[1],newmac[2],newmac[3],newmac[4],newmac[5]);
}

void CCarplayAdapter::AddBluetoothID(const char * macstr)
{
	if(!macstr)
		return;

	unsigned char mac[6];
	CStringToMACAddress( macstr, -1, sizeof(mac), mac );
	AddBluetoothID(mac);
}
void CCarplayAdapter::AddBluetoothID(const wchar_t * macstr)
{
	if(!macstr)
		return;

	unsigned char mac[6];
	char macstr_multi[32];
	int ret = WideCharToMultiByte( CP_ACP, 0, macstr, -1, macstr_multi, _countof(macstr_multi), NULL, NULL );
	if ( ret > 0 )
	{
		CStringToMACAddress( macstr_multi, -1, sizeof(mac), mac );
		AddBluetoothID(mac);
	}
}
void CCarplayAdapter::RemoveBluetoothID(const unsigned char mac[6])
{
	if ( mac == NULL )
	{
		return;
	}
	m_maclock.Lock();
	list <unsigned char *>::iterator c1_Iter;
	for ( c1_Iter = m_maclist.begin( ); c1_Iter != m_maclist.end( ); c1_Iter++ )
	{
		if (memcmp(*c1_Iter, mac, 6) == 0)
		{
			delete *c1_Iter;
			m_maclist.erase(c1_Iter);
			break;
		}
	}
	m_maclock.Unlock();

	printf("RemoveBluetoothID: %X %X %X %X %X %X \n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

}

void CCarplayAdapter::RemoveBluetoothID(const char * macstr)
{
	if(!macstr)
		return;

	unsigned char mac[6];
	CStringToMACAddress( macstr, -1, sizeof(mac), mac );
	RemoveBluetoothID(mac);
}
void CCarplayAdapter::RemoveBluetoothID(const wchar_t * macstr)
{
	if(!macstr)
		return;

	unsigned char mac[6];
	char macstr_multi[32];
	int ret = WideCharToMultiByte( CP_ACP, 0, macstr, -1, macstr_multi, _countof(macstr_multi), NULL, NULL );
	if ( ret > 0 )
	{
		CStringToMACAddress( macstr_multi, -1, sizeof(mac), mac );
		RemoveBluetoothID(mac);
	}
}

void CCarplayAdapter::ClearAllBluetoothID()
{
	printf("ClearAllBluetoothID \n");

	m_maclock.Lock();
	list <unsigned char *>::iterator c1_Iter;
	for ( c1_Iter = m_maclist.begin( ); c1_Iter != m_maclist.end( ); c1_Iter++ )
	{
		delete *c1_Iter;
	}
	m_maclist.clear();
	m_maclock.Unlock();
}
UINT CCarplayAdapter::GetBluetoothIDCount()
{
	UINT cnt=0;
	m_maclock.Lock();
	cnt = m_maclist.size();
	m_maclock.Unlock();
	return cnt;
}
void CCarplayAdapter::GetBluetoothIDs(unsigned char ** outMAC, size_t inMaxMACCount, size_t* outMACCount)
{
	m_maclock.Lock();
	if ( (outMAC == NULL) || (inMaxMACCount < m_maclist.size()) )
	{
		if (outMACCount)
		{
			*outMACCount = m_maclist.size();
		}
	}
	else
	{
		size_t i;
		list <unsigned char *>::iterator c1_Iter;
		for ( i = 0, c1_Iter = m_maclist.begin( ); (c1_Iter != m_maclist.end( )) && (i<inMaxMACCount); c1_Iter++, i++)
		{
			const unsigned char * mac = *c1_Iter;
			if ( mac != NULL )
			{
				memcpy(outMAC[i], mac, 6);
			}
		}
		if (outMACCount)
		{
			*outMACCount = i;
		}
	}
	m_maclock.Unlock();
}

void CCarplayAdapter::IperfReport( TCHAR* report )
{
	if ( report )
	{
		//RETAILMSG(1, (report));
		m_strIperfReport += report;
		::PostMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_IPERF_REPORT,0);
	}
}

void CCarplayAdapter::IperfResults( TCHAR* results )
{
	if ( results )
	{
		m_strIperfResults = results;
	}
	else
	{
		m_strIperfResults = _T("");
	}
	::PostMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_IPERF_RESULTS,0);
}

void CCarplayAdapter::DeviceInformationUpdate(LPCWSTR inDeviceName)
{
	if ( inDeviceName )
	{
		//RETAILMSG(1, (_T("Device Name : %s\n"), inDeviceName));

		wchar_t file[MAX_PATH];
		swprintf_s(file,MAX_PATH,_T("\\ResidentFlash\\CarPlay Device\\%s.dat"),inDeviceName);

		if(IsFileExists(file))
		{
			RETAILMSG(1, (_T("<%s>, known device, %s\n"),_T(__FUNCTION__),inDeviceName));
			m_bFirstTimeConnect = FALSE;
		}
		else
		{
			RETAILMSG(1, (_T("<%s>, first connect device, %s\n"),_T(__FUNCTION__),inDeviceName));
			m_bFirstTimeConnect = TRUE;

			//store uuid file:
			CreateDirectory(_T("\\ResidentFlash\\CarPlay Device"),NULL);

			FILE* fp = NULL;
			_wfopen_s(&fp, file, L"wb");
			fclose(fp);
		}
	}
}

void CCarplayAdapter::DeviceTimeUpdate(unsigned long long inSecondsSinceReferenceDate,
	                                   short inTimeZoneOffsetMinutes,
	                                   char inDaylightSavingsOffsetMinutes )
{
	FILETIME				ft;
	SYSTEMTIME				st;
	unsigned long long		usec;
	BOOL					good;

	usec  = inSecondsSinceReferenceDate  * 1000000ULL;
	usec *= 10ULL;					// Convert microseconds to 100ns units.
	usec += 0x019db1ded53e8000ULL;	// Convert from Unix 1970 time: add 100ns units from 1/1/1601 to 1/1/1970.

	ft.dwHighDateTime	= (DWORD)( ( usec >> 32 ) & 0xFFFFFFFFULL );
	ft.dwLowDateTime	= (DWORD)(   usec         & 0xFFFFFFFFULL );

	good = FileTimeToSystemTime( &ft, &st );
	if ( good )
	{
		SetSystemTime( &st );
	}
}

void CCarplayAdapter::DeviceUUIDUpdate(LPCWSTR inUUID)
{
	if ( inUUID )
	{
		RETAILMSG(1, (_T("##DeviceUUIDUpdate : %s\n"), inUUID));

#if 0
		wchar_t file[MAX_PATH];
		swprintf_s(file,MAX_PATH,_T("\\ResidentFlash\\CarPlay Device\\%s.dat"),inUUID);

		if(IsFileExists(file))
		{
			RETAILMSG(1, (_T("<%s>, known device, %s\n"),_T(__FUNCTION__),inUUID));
			m_bFirstTimeConnect = FALSE;
		}
		else
		{
			RETAILMSG(1, (_T("<%s>, first connect device, %s\n"),_T(__FUNCTION__),inUUID));
			m_bFirstTimeConnect = TRUE;

			//store uuid file:
			CreateDirectory(_T("\\ResidentFlash\\CarPlay Device"),NULL);

			FILE* fp = NULL;
			_wfopen_s(&fp, file, L"wb");
			fclose(fp);
		}
#endif

	}
}

BOOL CCarplayAdapter::DeviceSetMac(unsigned char* inMacData,size_t inMacDataLen)
{
	BOOL bRet = FALSE;
	char cstr[ 32 ] = {0};
	if (inMacData != NULL && (inMacDataLen == 6) )
	{
		MACAddressToCString( inMacData, cstr, FALSE );
		bRet = SetMacToCPMAC(inMacData, inMacDataLen) && SetMacToReg(cstr);
		::PostMessage(m_hMainWnd,WM_CARPLAY_MAC_CHANGED,0,0);
	}
	return bRet;
}

BOOL CCarplayAdapter::DeviceSetMac(char* inMACstr)
{
	BOOL bRet = FALSE;
	unsigned char mac[6];
	
	if (inMACstr != NULL)
	{
		if (CStringToMACAddress( inMACstr, -1, sizeof(mac), mac ) == kNoErr)
		{
			bRet = SetMacToCPMAC(mac, sizeof(mac)) && SetMacToReg(inMACstr);
			::PostMessage(m_hMainWnd,WM_CARPLAY_MAC_CHANGED,0,0);
		}
	}
	return bRet;
}

BOOL CCarplayAdapter::SetMacToReg(char* inMACstr)
{
	BOOL bRet = FALSE;
	wchar_t wmacstr[32] = {0};
	if (inMACstr != NULL)
	{
		printf("[CCarplayAdapter::DeviceSetMac] mac = %s\n", inMACstr);
		int ret;
		ret = MultiByteToWideChar( CP_ACP, 0, inMACstr, -1, wmacstr, _countof(wmacstr));
		if ( ret > 0 )
		{
			CReg reg;
			reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\USB\\FunctionDrivers\\ncmfn\\"));
			if (reg.SetSZ(_T("MacAddress"),wmacstr))
			{
				SetEvent(m_hSetMACEvent);
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

BOOL CCarplayAdapter::SetMacToCPMAC(unsigned char* inMacData,size_t inMacDataLen)
{
	BOOL bRet = FALSE;
	if (m_SetMACCallback != NULL)
	{
		bRet = DeviceIoControl(m_hcpd, IOCTL_CSECURE_SET_MAC, inMacData, inMacDataLen, NULL, 0, NULL, NULL);
		if (!bRet)
		{
			RETAILMSG(1,(_T("\n####Set MAC Failed! err = %d##### \n"), GetLastError()));
		}
	}
	return bRet;
}

BOOL CCarplayAdapter::CheckMAC()
{
	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\USB\\FunctionDrivers\\ncmfn\\"));
	LPCTSTR mac=reg.ValueSZ(_T("MacAddress"));
	if (mac != NULL)
	{
		m_bSetMAC = TRUE;
	}
	else if (m_SetMACCallback != NULL)
	{
		unsigned char buf[6] = {0};
		size_t bufsize = sizeof(buf);

		if (!DeviceIoControl(m_hcpd, IOCTL_CSECURE_GET_MAC, NULL, 0, buf, bufsize, NULL, NULL))
		{
			RETAILMSG(1,(_T("\n####Get MAC Failed! err = %d##### \n"), GetLastError()));
		}
		else
		{
			DeviceSetMac(buf, bufsize);
		}
	}
	return m_bSetMAC;
}

BOOL CCarplayAdapter::CheckMACFromFile()
{
	const wchar_t strSrcMAC[] = _T("\\USB Disk\\MAC");
	const wchar_t strSrcMAC2[] = _T("\\GPS CARD\\MAC");
	
	if (SetMACFromFile(strSrcMAC) == kNoErr)
	{
		RETAILMSG(1,(_T("\n####Set MAC from %s success!##### \n"),strSrcMAC));
		return TRUE;
	}
	if (SetMACFromFile(strSrcMAC2) == kNoErr)
	{
		RETAILMSG(1,(_T("\n####Set MAC from %s success!##### \n"),strSrcMAC2));
		return TRUE;
	}
	return FALSE;
}

OSStatus CCarplayAdapter::SetMACFromFile(const wchar_t * path)
{
	FILE* fp;
	char *ca_input;
	OSStatus err = kNoErr;

	if ( path == NULL )
	{
		err = kParamErr;
		goto exit;
	}
	if (IsFileExists(path))
	{
		err = _wfopen_s(&fp,path, L"rb");
		if( kNoErr == err )
		{
			size_t fileSize=0;
			fseek (fp , 0 , SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			fileSize = min(256,fileSize);
			ca_input =new char[fileSize+1];
			size_t readsize = fread(ca_input,fileSize,1,fp);
			ca_input[fileSize] = 0;
			fclose(fp);
			if (!DeviceSetMac(ca_input))
			{
				err = kUnknownErr;
			}
			delete ca_input;
		}
		else
		{
			RETAILMSG(1,(_T("\n####File open failed %s##### \n"),path));
		}
	}
	else
	{
		err = kPathErr;
	}
exit:
	return err;
}

BOOL CCarplayAdapter::DeleteMAC()
{
	OSStatus err = kNoErr;
	CReg reg;

	if (m_SetMACCallback != NULL)
	{
		if (!DeviceIoControl(m_hcpd, IOCTL_CSECURE_DELETE_MAC, NULL, 0, NULL, 0, NULL, NULL))
		{
			err = GetLastError();
			RETAILMSG(1,(_T("\n[CCarplayAdapter::DeleteMAC]####Delete MAC Failed! err = %d##### \n"), err));
			goto exit;
		}
	}
	
	if (!reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\USB\\FunctionDrivers\\ncmfn\\")))
	{
		err = GetLastError();
		RETAILMSG(1,(_T("####[CCarplayAdapter::DeleteMAC]Open reg Failed! err = %d##### \n"), err));
		goto exit;
	}
	if (!reg.DeleteValue(_T("MacAddress")))
	{
		err = GetLastError();
		RETAILMSG(1,(_T("####[CCarplayAdapter::DeleteMAC]Open Delete reg Value Failed! err = %d##### \n"), err));
		goto exit;
	}
	
exit:
	return err == kNoErr;
}

const wchar_t* CCarplayAdapter::GetMAC()
{
	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\USB\\FunctionDrivers\\ncmfn\\"));
	LPCTSTR macstr=reg.ValueSZ(_T("MacAddress"));
	if (macstr != NULL)
	{
		m_strMAC = macstr;
		m_strMAC.Insert(2, _T('-'));
		m_strMAC.Insert(5, _T('-'));
		m_strMAC.Insert(8, _T('-'));
		m_strMAC.Insert(11, _T('-'));
		m_strMAC.Insert(14, _T('-'));
		RETAILMSG(1, (_T("[CCarplayAdapter::GetMAC] mac = %s\n"), m_strMAC));
	}
	else
	{
		m_strMAC.Empty();
	}
	return m_strMAC;
}


BOOL CCarplayAdapter::DeleteCA()
{
	OSStatus err = kNoErr;
#if CVTE_EN_CPCA
	err = DeleteCAFromCPCA();
#else
	wchar_t strDestCA[MAX_PATH];
	wchar_t strDestCA2[MAX_PATH];
	int iRet;
	
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
	if(IsFileExists(strDestCA))
	{
		if (!DeleteFile(strDestCA))
		{
			printf("Delete CA File failed!\n");
			err = GetLastError();
			goto exit;
		}
	}
	if(IsFileExists(strDestCA2))
	{
		if (!DeleteFile(strDestCA2))
		{
			printf("Delete CA File failed!\n");
			err = GetLastError();
			goto exit;
		}
	}
exit:
#endif
	return (err == kNoErr);
}

void CCarplayAdapter::OnSpeedSample(double inSpeed)
{
	m_locationinfo.OnSpeedSample(inSpeed);
}

void CCarplayAdapter::OnSlipDetect(BOOL inSlipDetect)
{
	m_locationinfo.OnSlipDetect(inSlipDetect);
}

void CCarplayAdapter::OnTransmissionState(char inTransmissionState)
{
	m_locationinfo.OnTransmissionState(inTransmissionState);
}
void CCarplayAdapter::OnNightModeChanged(BOOL bNightMode)
{
	m_bOnNightMode = bNightMode;
}
void CCarplayAdapter::SetUserSelectAudioSRC(BOOL bUserSelect)
{
	RETAILMSG(1,(_T("#### CCarplayAdapter::SetUserSelectAudioSRC=%d###\r\n"),bUserSelect));
	m_bUserSelectAudioSRC = bUserSelect;
}
void CCarplayAdapter::SetUserSelectNativeUI(BOOL bUserSelect)
{
	RETAILMSG(1,(_T("#### CCarplayAdapter::SetUserSelectNativeUI=%d###\r\n"),bUserSelect));
	m_bUserSelectNativeUI = bUserSelect;
}
void CCarplayAdapter::SetUserSelectNativeNavi(BOOL bUserSelect)
{
	RETAILMSG(1,(_T("#### CCarplayAdapter::SetUserSelectNativeNavi=%d###\r\n"),bUserSelect));
	m_bUserSelectNativeNavi = bUserSelect;
}
void CCarplayAdapter::StartIperfServer()
{
	wchar_t pBuf[MAX_PATH];
	LPCWSTR tcparglist[] =
	{
		_T(""),
		_T("-V"),
		_T("-s"),
		_T("-l"),
		_T("128k"),
		_T("-w"),
		_T("128k"),
		_T("-i"),
		_T("1")
	};
	LPCWSTR udparglist[] =
	{
		_T(""),
		_T("-V"),
		_T("-s"),
		_T("-u"),
		_T("-l"),
		_T("1448"),
		_T("-w"),
		_T("256k"),
		_T("-i"),
		_T("1")
	};
	GetModuleFileName(NULL,pBuf,sizeof(pBuf)/sizeof(wchar_t));
	if ( m_tcpserver == NULL )
	{
		tcparglist[0] = pBuf;
		CarplayAPIIperfStart( _countof(tcparglist), tcparglist, &m_tcpserver );
	}
	if ( m_udpserver == NULL )
	{
		udparglist[0] = pBuf;
		CarplayAPIIperfStart( _countof(udparglist), udparglist, &m_udpserver );
	}
}

void CCarplayAdapter::UpdateSessionAddress()
{
	OSStatus err;
	wchar_t pwcaddress[MAX_PATH];
	char paddress[MAX_PATH];

	err = CarplayAPIGetSessionAddress( paddress, _countof(paddress) );
	if ( err == kNoErr )
	{
		int ret;
		ret = MultiByteToWideChar( CP_ACP, 0, paddress, -1, pwcaddress, _countof(pwcaddress));
		if ( ret > 0 )
		{
			m_SessionAddress = pwcaddress;
		}
		else
		{
			m_SessionAddress = NULL;
		}
	}
	else
	{
		m_SessionAddress = NULL;
	}
	m_isSessionSessionUpDate = TRUE;
}

BOOL CCarplayAdapter::isSessionAddressValid()
{
	OSStatus err;
	char paddress[MAX_PATH];

	if (m_isSessionSessionUpDate)
	{
		m_isSessionSessionUpDate = FALSE;
		m_isAddressValid = FALSE;
		m_SessionAddressFamily = AF_UNSPEC;
		if ( m_SessionAddress.Size() > 0 )
		{
			int ret;
			ret = WideCharToMultiByte( CP_ACP, 0, m_SessionAddress.String(), -1, paddress, _countof(paddress), NULL, NULL );
			if ( ret > 0 )
			{
				sockaddr_ip		sip;
				size_t			sipLen;
				err = CarplayAPIStringToSockAddr( paddress, &sip, sizeof(sip), &sipLen );
				if ( err == kNoErr )
				{
					m_SessionAddressFamily = sip.sa.sa_family;
					m_isAddressValid = TRUE;
				}
			}
		}
	}
	return m_isAddressValid;
}

u_short CCarplayAdapter::GetSessionAddressFamily()
{
	return m_SessionAddressFamily;
}

const wchar_t * CCarplayAdapter::GetSessionAddressStr()
{
	return m_SessionAddress.String();
}

void CCarplayAdapter::InputSessionAddress(int c)
{
	wchar_t str[8];
	swprintf_s(str,8,_T("%c"),c);

	m_SessionAddress += str;
	m_isSessionSessionUpDate = TRUE;
}

void CCarplayAdapter::DeleteSessionAddress(int num)
{
	if(m_SessionAddress.Size()>0)
	{
		if(num<=0)
		{
			m_SessionAddress = NULL;
		}
		else
		{
			m_SessionAddress -= num;
		}
	}
	m_isSessionSessionUpDate = TRUE;
}

void CCarplayAdapter::StartTCPClient()
{
	void * context = NULL;
	wchar_t pBuf[MAX_PATH];
	LPCWSTR arglistv4[] =
	{
		_T(""),
		_T("-p"),
		_T("5001"),
		_T("-l"),
		_T("128k"),
		_T("-w"),
		_T("128k"),
		_T("-i"),
		_T("1.0"),
		_T("-c"),
		_T("127.0.0.1"),
		_T("-t"),
		_T("10.0"),
	};
	LPCWSTR arglistv6[] =
	{
		_T(""),
		_T("-p"),
		_T("5001"),
		_T("-l"),
		_T("128k"),
		_T("-w"),
		_T("128k"),
		_T("-i"),
		_T("1.0"),
		_T("-V"),
		_T("-c"),
		_T("FE80::10BD:ED2F:7337:A30C%5"),
		_T("-t"),
		_T("10.0"),
	};
	GetModuleFileName(NULL,pBuf,sizeof(pBuf)/sizeof(wchar_t));
	if (m_SessionAddressFamily == AF_INET)
	{
		arglistv4[0] = pBuf;
		arglistv4[10] = m_SessionAddress.String();
//			for (int i=0; i<_countof(arglistv4); i++)
//			{
//				RETAILMSG(1, (_T("%s "), arglistv4[i]));
//			}
//			RETAILMSG(1, (_T("\n")));
		CarplayAPIIperfStart( _countof(arglistv4), arglistv4, &context );
	}
	else if( m_SessionAddressFamily == AF_INET6 )
	{
		arglistv6[0] = pBuf;
		arglistv6[11] = m_SessionAddress.String();
//			for (int i=0; i<_countof(arglistv6); i++)
//			{
//				RETAILMSG(1, (arglistv6[i]));
//			}
//			RETAILMSG(1, (_T("\n")));
		CarplayAPIIperfStart( _countof(arglistv6), arglistv6, &context );
	}
}

void CCarplayAdapter::StartUDPClient(LPCWSTR inBandwidth)
{
	void * context = NULL;
	wchar_t pBuf[MAX_PATH];
	LPCWSTR arglistv4[] =
	{
		_T(""),
		_T("-p"),
		_T("5001"),
		_T("-l"),
		_T("1448"),
		_T("-w"),
		_T("128k"),
		_T("-i"),
		_T("1.0"),
		_T("-u"),
		_T("-c"),
		_T("127.0.0.1"),
		_T("-t"),
		_T("10.0"),
		_T("-b"),
		_T("100m"),
	};
	LPCWSTR arglistv6[] =
	{
		_T(""),
		_T("-p"),
		_T("5001"),
		_T("-l"),
		_T("1448"),
		_T("-w"),
		_T("128k"),
		_T("-i"),
		_T("1.0"),
		_T("-u"),
		_T("-V"),
		_T("-c"),
		_T("FE80::10BD:ED2F:7337:A30C%5"),
		_T("-t"),
		_T("10.0"),
		_T("-b"),
		_T("100m"),
	};
	GetModuleFileName(NULL,pBuf,sizeof(pBuf)/sizeof(wchar_t));
	if (m_SessionAddressFamily == AF_INET)
	{
		arglistv4[0] = pBuf;
		arglistv4[11] = m_SessionAddress.String();
		arglistv4[15] = inBandwidth;
//			for (int i=0; i<_countof(arglistv4); i++)
//			{
//				RETAILMSG(1, (_T("%s "), arglistv4[i]));
//			}
//			RETAILMSG(1, (_T("\n")));
		CarplayAPIIperfStart( _countof(arglistv4), arglistv4, &context );
	}
	else if( m_SessionAddressFamily == AF_INET6 )
	{
		arglistv6[0] = pBuf;
		arglistv6[12] = m_SessionAddress.String();
		arglistv6[16] = inBandwidth;
//			for (int i=0; i<_countof(arglistv6); i++)
//			{
//				RETAILMSG(1, (_T("%s "), arglistv6[i]));
//			}
//			RETAILMSG(1, (_T("\n")));
		CarplayAPIIperfStart( _countof(arglistv6), arglistv6, &context );
	}
}

CAtlStringW& CCarplayAdapter::GetIperfReport()
{
	return m_strIperfReport;
}

void CCarplayAdapter::ClearIperfReport()
{
	m_strIperfReport = _T("");
}

CAtlStringW& CCarplayAdapter::GetIperfResults()
{
	return m_strIperfResults;
}
BOOL CCarplayAdapter::NeedShowTopBanner()
{
	return m_bNeedShowTopBanner;
}

void CCarplayAdapter::StartH264Test( const wchar_t* inAVCCPath, const wchar_t* inH264Path )
{
	char avccpath[MAX_PATH];
	char h264path[MAX_PATH];
	int ret;

	ret = WideCharToMultiByte( CP_ACP, 0, inAVCCPath, -1, avccpath, _countof(avccpath), NULL, NULL);
	if ( ret == 0 )
	{
		return;
	}
	ret = WideCharToMultiByte( CP_ACP, 0, inH264Path, -1, h264path, _countof(h264path), NULL, NULL);
	if ( ret == 0 )
	{
		return;
	}
	printf("APP:MSG:GUI:CarPlay: [CCarplayAdapter::StartH264Test]: avccpath=%s, h264path=%s\r\n",avccpath, h264path);
	CarplayAPIStartH264Test( avccpath, h264path );
}

void CCarplayAdapter::TakeGstResource(BOOL bTake)
{
	CarplayAPITakeGstResource(bTake);
}

void CCarplayAdapter::LimitedUI(BOOL inLimitUI)
{
	m_bLimitUI = inLimitUI;
	CarplayAPILimitedUI(inLimitUI);
}

void CCarplayAdapter::SetReverseMode(BOOL bOnReverse)
{
	m_bOnReverseMode = bOnReverse;
}
void CCarplayAdapter::SetAccOnWarning(BOOL bShow)
{
	m_bShowAccOnWarning = bShow;
}
void CCarplayAdapter::SetSystemPowerState(UINT idPowerState)
{
	m_idPowerState = idPowerState;
}
void CCarplayAdapter::SetBluetoothPhoneStatus(BOOL bActive)
{
	printf("[%s] : %d\r\n",__FUNCTION__,bActive);
	m_bBluetoothPhoneActive = bActive;

	//if bluetooth phone call is active, un-borrow screen and audio
	if(!bActive && m_ControlStreams)
	{
		printf("[%s] : Disable Bluetooth \n", __FUNCTION__);
		::PostMessage(m_hMainWnd,WM_CARPLAY_STATUS,CARPLAY_DISABLE_BLUETOOTH,NULL);

		if(m_bDealIniBluetoothPhone)
		{
			m_bDealIniBluetoothPhone = FALSE;
			if(!m_bOnReverseMode && !m_bShowAccOnWarning)
			{
				CarplayAPIUnborrowScreen(NULL);
			}
			CarplayAPIUnborrowMainAudio(NULL);
			CarplayAPIChangeAppState(kAirPlaySpeechMode_NotApplicable,kAirPlayTriState_False,kAirPlayTriState_NotApplicable,"DisableBluetooth");
		}
	}
}

OSStatus CCarplayAdapter::ConnectTester( void *inBuf, size_t inMaxLen )
{
	return CarplayAPIConnectTester( inBuf, inMaxLen );
}

void CCarplayAdapter::ConnectTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CCarplayAdapter* pAdapter = (CCarplayAdapter*)dwUser;
	if ( pAdapter != NULL )
	{
		pAdapter->ConnectTimer();
	}
}

void CCarplayAdapter::ConnectTimer()
{
// 	if ( m_ConnectTimer != 0 )
// 	{
// 		timeKillEvent(m_ConnectTimer);
// 		m_ConnectTimer = 0;
// 	}
	printf("\r\n####### CarplayAPIRestart #######\r\n\r\n",__FUNCTION__);
	CarplayAPIRestart();
}

void CCarplayAdapter::Restart()
{
	CarplayAPIRestart();
}

CLocationInfo::CLocationInfo()
:m_hQuitEvent(NULL),m_hThreadHandle(NULL),
m_dwThreadId(-1),m_dwExitCode(-1)
{
	m_hQuitEvent = ::CreateEvent(0,FALSE,FALSE,0);
	m_PASCDdata.timestamp = (double)GetTickCount() / 1000.0;
	m_PASCDdata.transmissionState = kNMEATransmissionState_Drive;
	m_PASCDdata.slipDetect = FALSE;
	m_PASCDdata.sampleCount = 0;
	for (int i = 0; i < _countof(m_PASCDdata.samples); i++)
	{
		m_PASCDdata.samples[i].timeOffset = 0.0;
		m_PASCDdata.samples[i].speed = 0.0;
	}
}

CLocationInfo::~CLocationInfo()
{
	ForceTerminated();
	if (m_hQuitEvent)
	{
		CloseHandle(m_hQuitEvent);
	}
}

void CLocationInfo::StartLocationInformation(const LocationInfo_t* inLocationInfo)
{
	if (inLocationInfo && inLocationInfo->unInfo.PASCD)
	{
		m_hThreadHandle = ::CreateThread( NULL,
	                                      0,
	                                      CLocationInfo::ThreadProc,
	                                      ( LPVOID )this,
	                                      0,
	                                      &m_dwThreadId );
		if (m_hThreadHandle != NULL)
		{
			CeSetThreadPriority(m_hThreadHandle, CE_THREAD_PRIO_256_HIGHEST);
		}
	}
}

void CLocationInfo::StopLocationInformation()
{
	if ( m_hThreadHandle )
	{
		SetEvent(m_hQuitEvent);
		WaitForSingleObject( m_hThreadHandle, INFINITE);
		::CloseHandle( m_hThreadHandle );
		m_hThreadHandle = NULL;
		m_dwThreadId = 0;
	}
}

BOOL CLocationInfo::ForceTerminated()
{
    if( m_hThreadHandle )
    {
        BOOL bReturn = ::TerminateThread( m_hThreadHandle, ( DWORD ) - 1 ); // terminate abnormal
        ::CloseHandle( m_hThreadHandle );
		m_dwExitCode = -1;
        m_hThreadHandle = NULL;
		m_dwThreadId = 0;
        return bReturn;
    };
    return TRUE;
}

DWORD CLocationInfo::ThreadProc( LPVOID dParam )
{
    CLocationInfo* pLocationInfo = ( CLocationInfo* ) dParam;
    pLocationInfo->m_dwExitCode = pLocationInfo->ThreadRun();
    ::ExitThread( pLocationInfo->m_dwExitCode );
    return pLocationInfo->m_dwExitCode;
};

DWORD CLocationInfo::ThreadRun()
{
	m_PASCDdataLock.Lock();
	m_PASCDdata.sampleCount = 0;
	m_PASCDdataLock.Unlock();
	while(1)
	{
	    DWORD dwRet = WaitForSingleObject(m_hQuitEvent, 1000);
		if (dwRet == WAIT_TIMEOUT)
		{
			m_PASCDdataLock.Lock();
			if (m_PASCDdata.sampleCount == 0)
			{
				m_PASCDdata.samples[m_PASCDdata.sampleCount].timeOffset = 0.0;
				m_PASCDdata.samples[m_PASCDdata.sampleCount].speed = 0.0;
				m_PASCDdata.sampleCount++;
			}
			CarplayAPISendPASCD(&m_PASCDdata);
			m_PASCDdata.timestamp = (double)GetTickCount() / 1000.0;
			m_PASCDdata.sampleCount = 0;
			m_PASCDdataLock.Unlock();
		}
		else
		{
			break;
		}
	}
    return 0;
}

void CLocationInfo::OnSpeedSample(double inSpeed)
{
	m_PASCDdataLock.Lock();
	if (m_hThreadHandle && (m_PASCDdata.sampleCount < 50))
	{
		if (m_PASCDdata.sampleCount == 0)
		{
			m_PASCDdata.timestamp = (double)GetTickCount() / 1000.0;
		}
		double tmptime = (double)GetTickCount() / 1000.0;
		m_PASCDdata.samples[m_PASCDdata.sampleCount].timeOffset = tmptime - m_PASCDdata.timestamp;
		m_PASCDdata.samples[m_PASCDdata.sampleCount].speed = inSpeed;
		m_PASCDdata.sampleCount++;
	}
	m_PASCDdataLock.Unlock();
}

void CLocationInfo::OnSlipDetect(BOOL inSlipDetect)
{
	m_PASCDdataLock.Lock();
	m_PASCDdata.slipDetect = inSlipDetect;
	m_PASCDdataLock.Unlock();
}

void CLocationInfo::OnTransmissionState(char inTransmissionState)
{
	if (	(inTransmissionState == kNMEATransmissionState_Drive)
		||	(inTransmissionState == kNMEATransmissionState_Neutral)
		||	(inTransmissionState == kNMEATransmissionState_Park)
		||	(inTransmissionState == kNMEATransmissionState_Reverse)
		||	(inTransmissionState == kNMEATransmissionState_Unknown)
		)
	{
		m_PASCDdataLock.Lock();
		m_PASCDdata.transmissionState = inTransmissionState;
		m_PASCDdataLock.Unlock();
	}
}

#endif

