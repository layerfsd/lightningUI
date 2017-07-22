#include "../../StdAfx.h"
#include "GUI_Fantasy.h"
#include <filedata.h>
#include <DeviceActivate.h>
#include "../../resource.h"
#if CVTE_EN_ACCON_WARNING
#include "../Warning/WarningProxy.h"
#endif

#define PENDOWN_STATE_IDLE          0
#define PENDOWN_STATE_MEDIA         1
#define PENDOWN_STATE_PHONE         2
#define PENDOWN_STATE_SPEECH        3

#define INSTALL_USER_FILES			1

#define IDLE_SCREEN_BACKLIGHT		28
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
#define REARVIEW_VOLUME_DEFAULT		4
#else
#define REARVIEW_VOLUME_DEFAULT		9
#endif
//speech type: 1-carplay audio, 0-other audio, 2-audio DMA status, 3=android speech
#define SPEECH_MSG_TYPE_UNKNOWN				0
#define SPEECH_MSG_TYPE_CARPLAY				1
#define SPEECH_MSG_TYPE_DMA_STATUS			2
#define SPEECH_MSG_TYPE_ANDROID_AUTO		3

static HANDLE g_hTouchEnableGfxEvent = NULL;
#define TOUCH_EVENT_ENABLE_GFX_NAME TEXT("TOUCH_EVENT_ENABLE_GFX_NAME")

// const wchar_t* g_strInstallPath = _T("\\ResidentFlash\\Program files\\");
const wchar_t* g_strInstallPath = _T("\\ResidentFlash\\");
const wchar_t* g_strInstallFile = _T("\\USB Disk\\install.bin");
const wchar_t* g_strDefaultBackground = CVTE_REG_BACKGROUND_PATH_DEFAULT;


DWORD				g_dwTimeSyncedCnt=0;
UINT						g_idLanguageOffset=0;
BOOL					g_bKeyBeep=TRUE;
BOOL					g_bBTModuleLoaded=FALSE;
CM_String				g_strDeviceName;

BOOL					g_bEnableShootScreen=FALSE;

DWORD				g_UserClickTick=0;
DWORD				g_AccOnSRCToSwicth=-1;
DWORD				g_SRCToSwicthAfterLoadModule=-1;

extern HWND			g_hMainWnd;
extern HWND			g_hUIWnd;

// extern LPCTSTR 		g_strInstallPath;

// extern LPCTSTR			g_strInstallFile;
// extern HWND			g_hMediaCoreWnd;
// extern HWND			g_hPhoneCoreWnd;
// extern BOOL				g_bPhoneCallActive;
// extern WCHAR			g_StrMCUVersion[128];

extern WPARAM g_wParamVolumeInfo;
extern WPARAM g_lParamVolumeInfo;

extern WPARAM g_wParamSourceInfo;
extern WPARAM g_lParamSourceInfo;

extern WPARAM g_wParamMediaPlaybackInfo;
extern WPARAM g_lParamMediaPlaybackInfo;

extern WPARAM g_wParamRadioInfo;
extern WPARAM g_lParamRadioInfo;


extern BOOL				g_bNaviSpeechEnabled;

#if CVTE_EN_KEY_PROXY
static KeyHandlerParam g_keyparam[] =
{
	//Key					, key Value				,Flag,Event												            ,PressHold,LongPress,DoublePress ,FuncCallback,FuncContext
	{KEY_STW_VOL_MINUS		, KEY_VOLUMEDOWN		, 0	, EV_KEY_DOWN|EV_KEY_PRESS_HOLD													, 150,	600	,	0	, NULL, NULL},
	{KEY_STW_VOL_PLUS		, KEY_VOLUMEUP			, 0	, EV_KEY_DOWN|EV_KEY_PRESS_HOLD													, 150,	600	,	0	, NULL, NULL},
	{KEY_STW_VOL_MUTE		, KEY_MUTE				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_PREV			, KEY_PREVIOUS			, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	800	,	0	, NULL, NULL},
	{KEY_STW_NEXT			, KEY_NEXT				, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	800	,	0	, NULL, NULL},
	{KEY_STW_SEEK			, KEY_SEEK				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_PHONE_CALL		, KEY_HOOK_SWITCH		, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_PHONE_HANGUP	, KEY_DROP				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_SPEECH			, KEY_SPEECH			, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_NAVI			, KEY_NAVI				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_MODE			, KEY_MODE				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_POWER			, KEY_POWER				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_STW_HOME			, KEY_HOME				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_MEDIA_PLAY			, KEY_PLAY				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_MEDIA_RESUME		, KEY_RESUME			, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_MEDIA_STOP			, KEY_STOP				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_MEDIA_PAUSE		, KEY_PAUSE				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_MEDIA_PREVIOUS		, KEY_PREVIOUS			, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	600	,	0	, NULL, NULL},
	{KEY_MEDIA_NEXT			, KEY_NEXT				, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	600	,	0	, NULL, NULL},
	{KEY_MEDIA_MUTE			, KEY_MUTE				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_PHONE_ANSWER		, KEY_HOOK_SWITCH		, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_PHONE_HANG			, KEY_DROP				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_PHONE_ACH			, KEY_ACH				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_VCHICLE_TEL		, KEY_FLASH				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_VCHICLE_UP			, KEY_NEXT				, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	600	,	0	, NULL, NULL},
	{KEY_VCHICLE_DOWN		, KEY_PREVIOUS			, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	600	,	0	, NULL, NULL},
	{KEY_VCHICLE_VOL_SUB	, KEY_VOLUMEDOWN		, 0	, EV_KEY_DOWN|EV_KEY_PRESS_HOLD													, 150,	600	,	0	, NULL, NULL},
	{KEY_VCHICLE_VOL_ADD	, KEY_VOLUMEUP			, 0	, EV_KEY_DOWN|EV_KEY_PRESS_HOLD													, 150,	600	,	0	, NULL, NULL},
	{KEY_VCHICLE_MUTE		, KEY_MUTE_SPEECH		, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS					, 0,	600	,	0	, NULL, NULL},

	{KEY_PHY_KNOB_LEFT		, KEY_VOLUMEDOWN		, 0	, EV_KEY_DOWN																	, 150,	600	,	0	, NULL, NULL},
	{KEY_PHY_KNOB_RIGHT		, KEY_VOLUMEUP			, 0	, EV_KEY_DOWN																	, 150,	600	,	0	, NULL, NULL},
	{KEY_PHY_KNOB_BUTTON	, KEY_HOMEPAGE			, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS					, 0, 	1000,	500	, NULL, NULL},
	{KEY_PHY_FMAM			, KEY_RADIO				, 0 , EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_PHY_NAVI			, KEY_NAVI				, 0 , EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_PHY_MUTE			, KEY_MUTE				, 0 , EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_PHY_LUMINACE		, KEY_BRIGHTNESS_TOGGLE	, 0 , EV_KEY_DOWN|EV_KEY_UP|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS					, 0,	1000,	0	, NULL, NULL},
	{KEY_PHY_CYCLE_BUTTON_LEFT	, KEY_PREVIOUS		, 0 , EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	600	,	0	, NULL, NULL},
	{KEY_PHY_CYCLE_BUTTON_RIGHT	, KEY_NEXT			, 0 , EV_KEY_DOWN|EV_KEY_UP|EV_KEY_PRESS_HOLD|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS	, 200,	600	,	0	, NULL, NULL},

	{KEY_PHY_VOLUME_DOWN	, KEY_VOLUMEDOWN		, 0	, EV_KEY_DOWN|EV_KEY_PRESS_HOLD													, 150,	600	,	0	, NULL, NULL},
	{KEY_PHY_VOLUME_UP		, KEY_VOLUMEUP			, 0	, EV_KEY_DOWN|EV_KEY_PRESS_HOLD													, 150,	600	,	0	, NULL, NULL},
	{KEY_PHY_HOME			, KEY_HOMEPAGE			, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_SHORT_PRESS										, 0, 	600	,	0	, NULL, NULL},
	{KEY_PHY_POWER			, KEY_POWER				, 0	, EV_KEY_DOWN|EV_KEY_UP															, 0,	0	,	0	, NULL, NULL},
	{KEY_HARDWARE_HOME		, KEY_HOME_POWER		, 0	, EV_KEY_DOWN|EV_KEY_UP|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS|EV_KEY_DOUBLE_PRESS, 0, 	1000,	500	, NULL, NULL},
	{KEY_PHY_POWER_SCREEN	, KEY_POWER_SCREEN		, 0 , EV_KEY_DOWN|EV_KEY_UP|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS					, 0,	1000,	0	, NULL, NULL},
	{KEY_PHY_RETURN			, KEY_RETURN			, 0 , EV_KEY_DOWN|EV_KEY_UP															, 0,	0,		0	, NULL, NULL},
	{KEY_PHY_MUTE_SCREEN	, KEY_MUTE_SCREEN		, 0 , EV_KEY_DOWN|EV_KEY_UP|EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS					, 0,	1000,	0	, NULL, NULL},

	{KEY_PHY_VOL_MANUAL		, KEY_VOL_MANUAL		, 0 , EV_KEY_DOWN|EV_KEY_UP															, 0,	0,		0	, NULL, NULL},
};
#endif

CGUI_Fantasy::CGUI_Fantasy(void)
{
	m_pConfig=NULL;

	m_pSkinManager = NULL;

	m_pTextGL = NULL;

	m_pCurPanel=NULL;
	m_pIdlePanel=NULL;
	m_pSetMACPanel=NULL;

	m_hEventSD=NULL;
	m_hEventMute=NULL;
	m_hMuteThread=NULL;

	m_pCommander=NULL;
	m_pVideoWindow=NULL;
	m_pSmartBarVol=NULL;
	m_pSmartBarPhone=NULL;
	m_pSmartBarDrop=NULL;
	m_pSmartBarTopBanner=NULL;

// 	m_bSpeechIni=FALSE;
// 	m_bWidgetMode=FALSE;
	m_bSystemReady=FALSE;
	m_dwSystemReadyTick=0;
	m_bOpenSRC=FALSE;
	m_bBacklightOFF=FALSE;
	m_dwLastSpeechTick=0;
	m_bOnReverseMode=FALSE;
	m_bWaitingForPowerOFF=FALSE;
	m_bOnBrakeMode = TRUE;
	m_bReverseSignaled = FALSE;
	m_bReverseMCUCmd = TRUE;
// 	m_idLineOut=LINE_LEGACY_OUT;
// 	m_CallStatus=HFP_NO_CALL;
// 	m_bPhoneRing=FALSE;
	m_dwLastSeekTick=0;
 	m_dwLastMuteTick=0;
	m_dwOpenSRCTick=0;
	m_bModulesLoaded=FALSE;

	m_bAutoStartNavi=FALSE;

	m_idDevTypeInserted =DEV_NULL;

	m_idCurAudioSRC = SRC_AUDIO_INVALID;
	m_idCurMCUSRC = SRC_AUDIO_INVALID;
	m_idAudioSRCBeforeSpeech = SRC_AUDIO_INVALID;
	m_idCurMediaClassSRC = 0;

	m_bSpeechOn = FALSE;
    m_bSpeechVolumeChange = FALSE;
    m_bMainVolumeChange = FALSE;
	m_dwSpeechTimeOut = 0;
	m_dwSpeechStartTick=0;

	m_bOnMute = FALSE;
    m_bOnHWMute = FALSE;
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	m_bMute = TRUE;
#else
    m_bMute = FALSE;
#endif

	m_bWaitingSpeechTerminate = FALSE;

	m_bActivated = TRUE;
	m_bClickValid = TRUE;

	m_iVehicleSpeed=0;
	m_iSpeedCurVolome=0;

	m_bMCUPackageUpdated = FALSE;
	m_bAuxInDetect = FALSE;

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418 || CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	m_pAudioCodec = NULL;
#endif

	for (int i=0;i<MEDIA_TYPE_NUMBER;i++)
	{
		m_idValidSrcList[i]=0;
	}
	m_idValidSrcList[MEDIA_TYPE_RADIO]=1;

    //AUX IN
#if CVTE_EN_AUX
    m_idValidSrcList[MEDIA_TYPE_AUX]=1;
#endif
#if CVTE_EN_DVD
    m_idValidSrcList[MEDIA_TYPE_DISK]=1;
#endif
#if CVTE_EN_DTV
    m_idValidSrcList[MEDIA_TYPE_TV_DIGITAL]=1;
#endif
#if CVTE_EN_DAB
    m_idValidSrcList[MEDIA_TYPE_DAB]=1;
#endif
    m_idCurSrc = 0;

	m_idEQ=0;

	m_dwCallStatus=HF_STATE_IDLE;
	m_dwBTCallStatus=HF_STATE_IDLE;
	m_dwCarPlayCallStatus=HF_STATE_IDLE;

	m_bPhoneCallActive=FALSE;
// 	m_bPhoneTalking=FALSE;
	m_bBTPhoneCallActive=FALSE;
	m_bCarPlayPhoneCallActive=FALSE;

	m_idPowerState = SYS_POWER_ON;
    m_idScreenState = SYS_SCREEN_ON;

	m_bOnGestureProcess = FALSE;

	m_dwSetVolumeTick = 0;

	m_bAnimating=FALSE;

	#if CVTE_EN_LED
	m_pLED = NULL;
	#endif

	m_iVolume = 0;
	m_iSetVolume = 0;

	m_iVolOriginGPS=10;
	m_iVolOriginBluetooth=10;
	m_iVolOriginMedia=10;

	m_iVolumeCur=0;
	m_iVolumeFadeTo=0;
	m_bVolumeFadeReady=TRUE;
	m_bShowVolBar = TRUE;

	m_dwBklBeforeReverse = 100;

	//GPS:
	m_bEnableGPSMonitor = FALSE;
	m_hEvtGPSMonitor = INVALID_HANDLE_VALUE;
	m_hThreadGPSMonitor = INVALID_HANDLE_VALUE;
	m_bGPSPowerOn = TRUE;

	//
	m_idMuteType = 0;
	m_iMuteTimeOut = 0;
	m_bNeedUpdateVolLED=TRUE;

	m_bAutoUnMute = FALSE;
	m_bAutoMute = FALSE;

	m_idMcuUpdateStatus = 0;

	m_idNaviStatus = 0;

	//
	m_dwPowerStateTick = 0;
    m_dwActivateSiriTick = 0;

	//
	m_bPowerOperating = FALSE;
	m_bMuteOperating = FALSE;
    m_bMuteFading = FALSE;

	m_iVolumeDelt = 0;

	m_bManualPowerOn = TRUE;
	m_bManualMute = FALSE;

	m_iDeviceDetectCnt = 0;

	m_bPlayingSoundEffect = FALSE;
	m_iPlaySoundEffectCnt = 0;
	m_hPlaySoundEffect = INVALID_HANDLE_VALUE;

	//Carplay
	m_bCarPlayConnected = FALSE;
	m_bCarPlayAudioOn = FALSE;
	m_bDeviceActivated = TRUE;
	m_bIsSetMAC = TRUE;
	m_bCarPlayAltAudioOn = FALSE;
	m_bCarPlayMainAudioOn = FALSE;
	m_bCarPlayTurnbyTurnOn = FALSE;
	m_bCarPlaySiriRecognizing = FALSE;

	m_biPodConnected = FALSE;

	m_dwCarPlayArrivedTick = 0;
	m_dwCarPlayAudioOnTick = 0;
	m_dwCarPlayAudioOffTick = 0;

	m_dwFrameRenderTime = 10;
	m_bAutoReleaseNoFade = FALSE;

	m_bCommanderInitialized = FALSE;

	m_bMCUUpdating = FALSE;
	m_bOnSR = FALSE;
	m_bOnTAMode = FALSE;

    m_dwSnapshotDoneTick = 0;
    m_dwScreenStateTick = 0;

    m_iScrnBrightness = 2;
    m_bNoSendMuteCmd = FALSE;

	m_bAltAudioDataActived = FALSE;
	m_bAltAudioDmaActived = FALSE;

	m_idUIBeforeIdle = -1;

	m_idMediaClassBeforSR = 0;	//None
	m_bMediaPlayingBeforeSR=FALSE;

	m_bResumeSRCAfterRearView = FALSE;

	m_bUIWindowShow = FALSE;

#if CVTE_EN_IDLE_LOGO
	////enable dual logo here
#if 0
	m_logoAlpha = 0;
	m_logo2Alpha = 0;
	m_idLogoShowStyle = 2;
#else
	m_logoAlpha = 1.0f;
	m_logo2Alpha = 1.0f;
	m_idLogoShowStyle = 0;
#endif
	m_bLogoShade = FALSE;
	m_dwLogoShadeTick = 0;
#endif

	//android auto:
	m_bAndroidAutoConnect = FALSE;
	m_bAndroidAutoVoiceActicve = FALSE;

	m_iSpeechMsgType = 0;
	m_bSpeechSignaled = FALSE;
	m_pMediaWrapper = NULL;
	m_pSndPlayer = NULL;
}

CGUI_Fantasy::~CGUI_Fantasy(void)
{
	//test:
	PrePowerOff();

	//delete panels:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		delete m_listPanels.Get();
		m_listPanels.Next();
	}

	//common modules:
	if(m_pCommander)
	{
		delete m_pCommander;
		m_pCommander=NULL;
	}

	if(m_pVideoWindow)
	{
		delete m_pVideoWindow;
		m_pVideoWindow=NULL;
	}

	if(m_pSmartBarVol)
	{
		delete m_pSmartBarVol;
		m_pSmartBarVol=NULL;
	}
	if(m_pSmartBarPhone)
	{
		delete m_pSmartBarPhone;
		m_pSmartBarPhone=NULL;
	}

	if(m_pStatusBar)
	{
		delete m_pStatusBar;
		m_pStatusBar=NULL;
	}

	if(m_pTextGL)
	{
		delete m_pTextGL;
		m_pTextGL=NULL;
	}

	if(m_pSkinManager)
	{
		delete m_pSkinManager;
		m_pSkinManager=NULL;
	}

	if(m_pConfig)
	{
		delete m_pConfig;
		m_pConfig=NULL;
	}

	if (m_hEventSD != NULL)
	{
		CloseHandle(m_hEventSD);
		m_hEventSD = NULL;
	}

	if (m_hMuteThread != NULL)
	{
		SetEvent(m_hEventMute);
		WaitForSingleObject(m_hMuteThread, INFINITE);
		CloseHandle(m_hMuteThread);
		m_hMuteThread = NULL;
	}

	if (m_hEventMute != NULL)
	{
		CloseHandle(m_hEventMute);
		m_hEventMute = NULL;
	}

	//GPS:
	if(m_bEnableGPSMonitor)
	{
		EnableGPSMonitor(FALSE);
		Sleep(100);
	}
	if (m_hEvtGPSMonitor != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hEvtGPSMonitor);
		m_hEvtGPSMonitor = INVALID_HANDLE_VALUE;
	}

	if (m_hPlaySoundEffect != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hPlaySoundEffect);
		m_hPlaySoundEffect = INVALID_HANDLE_VALUE;
	}

	#if CVTE_EN_LED
	if(m_pLED)
	{
		delete m_pLED;
		m_pLED=NULL;
	}
	#endif

	CoUninitialize();

	RETAILMSG(DEBUG_GUI,(_T("destroy CGUI_Fantasy!..............\r\n")));

}

// #include <wavedev.h>
// #define WIDM_GETVOLUME			63
// #define WIDM_SETVOLUME			64
// #define VOLUME_ONE_STEP			(0xFFFFFFFF/10)

BOOL CGUI_Fantasy::Initialize(HWND window,MODULEENTRY entry,int iPriority)
{
#if 0
	//for test audio switch:
	Sleep(2000);
	m_pCommander = new CCommanderEx;
	if(m_pCommander)
	{
		RETAILMSG(1,(_T("_ARM_AUDIO_SW_KEY, SRC_AUDIO_ARM\r\n")));

		m_pCommander->Initialize(CommandProc,this,_T("COM3:"),CBR_115200,window);
		m_pCommander->IniMCU();
		Sleep(500);
 		m_pCommander->SendCmd(_ARM_AUDIO_SW_KEY,SRC_AUDIO_RADIO);
		Sleep(50);
		m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+0);
		Sleep(50);
		m_pCommander->SendCmd(_ARM_VOLUME_KEY,20);

		Sleep(3000);
		m_pCommander->SendCmd(_ARM_AUDIO_SW_KEY,SRC_AUDIO_ARM);
		Sleep(50);
		m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+0);
		Sleep(50);
		m_pCommander->SendCmd(_ARM_VOLUME_KEY,20);

		RETAILMSG(1,(_T("_ARM_AUDIO_SW_KEY, SRC_AUDIO_ARM finish\r\n")));
	}
	return 0;
#endif

	//create SD card event:
	m_hEventSD = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEventMute = CreateEvent(NULL,FALSE,FALSE,NULL);

	//default EGL initialize:
	CGUI_OpenglES::Initialize(window,entry,iPriority);

	if(FAILED(CoInitializeEx(NULL,COINIT_MULTITHREADED)))
	{
		RETAILMSG(DEBUG_GUI,(_T("APP:ERROR:GUI:Main: [CGUI_Fantasy::Initialize]: ERROR: CoInitialize\r\n")));
		return FALSE;
	}

	#if CVTE_EN_KEY_PROXY
	//Init KeyHandler
	CKeysHandler::GetInstance()->Initialize(window, WM_KEY);
	CKeysHandler::GetInstance()->AddKeys(g_keyparam, _countof(g_keyparam));
	#endif


	//get config:
	m_pConfig = new CAppConfigProxy;	//CProConfig;
	m_pConfig->GetOemSector();
	UINT idLang=m_pConfig->GetCurParam()->idLanguage;
 	g_idLanguageOffset=(idLang==0)?0:idLang*1000;
	CReg reg(HKEY_LOCAL_MACHINE, RK_CLOCK);
	reg.SetDW(RV_AUTODST, m_pConfig->GetCurParam()->bDSTAutoAdjust);

#if CVTE_EN_NAVI
	m_bAutoStartNavi = (m_pConfig->GetCurParam()->bAutoStartNavi);
#else
	m_bAutoStartNavi = FALSE;
#endif

	g_bKeyBeep=(m_pConfig->GetCurParam()->bKeyBeep);

	//test:
	m_pConfig->GetCurParam()->bHasCanBus=0;

	//
#if CVTE_EN_IDLE_LOGO

	const int cx=800,cy=480;
	if(m_tagLogo.Initialize(0,NULL,m_pConfig->GetCurParam()->strLogoName.String(),cx,cy))
	{
		VIEW_STATE vs=m_tagLogo.GetCurViewState();
		vs.fRotateX=-180;
		m_tagLogo.SetIniViewState(&vs);
		m_tagLogo.SetScale(GLfloat(g_iScreenWidth)/cx,GLfloat(g_iScreenHeight)/cy);
	}

	if(m_tagLogo2.Initialize(0,NULL,m_pConfig->GetCurParam()->strLogoName2.String(),cx,cy))
	{
		VIEW_STATE vs2=m_tagLogo2.GetCurViewState();
		vs2.fRotateX=-180;
		m_tagLogo2.SetIniViewState(&vs2);
		m_tagLogo2.SetScale(GLfloat(g_iScreenWidth)/cx,GLfloat(g_iScreenHeight)/cy);
	}

 	//ActivateRenderLogo();
#endif

	//skin manager:
	m_pSkinManager=new CSkinManagerGL(this,m_pConfig);

	//skins 1:

	if(!IsFileExists(m_pConfig->GetCurParam()->strSkinFileName.String()))
	{
		//m_pConfig->GetCurParam()->strSkinFileName = _T("\\Windows\\skin_lightning.skn");
		m_pConfig->GetCurParam()->strSkinFileName = _T("\\ResidentFlash\\LightningUI\\Skins\\skin_lightning.skn");
	}
  	m_pSkinManager->LoadFromFile(m_pConfig->GetCurParam()->strSkinFileName.String());

#if CVTE_EN_COMBINE_ANDROID_LINK
	CReg regandroid;
	regandroid.Open(HKEY_LOCAL_MACHINE,_T("Drivers\\AndroidAccessory"));
	m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType = regandroid.ValueDW(_T("SwitchDeviceType"));
#endif

//    m_pSkinManager->LoadFromFile(_T("\\ResidentFlash\\skin_lightning.skn"));
// 	m_pSkinManager->LoadFromFile(_T("\\USB Disk\\skin_lightning.skn"));
// 	m_pSkinManager->LoadFromFile(_T("\\SDMMC\\skin_lightning.skn"));

// 	DWORD dwt=GetTickCount();
/*
	int cx=1024;//g_iScreenWidth;//256;//512;
	int cy=512;//g_iScreenHeight;//128;//256;
	//background://////////////////////////////////
// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_000,g_strDefaultBackground,g_iScreenWidth,g_iScreenHeight))
// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_000,g_strDefaultBackground,cx,cy))
// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_000,_T("\\GPS Card\\back12.jpg"),cx,cy))
	if(!m_pSkinManager->AddTextureFromFile(TEXID_000,_T("\\ResidentFlash\\bk.jpg"),cx,cy))

// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_000,_T("\\ResidentFlash\\LIGHTNING.jpg"),g_iScreenWidth,g_iScreenHeight))
// 	if(!m_pSkinManager->AddTextureFromFile(TEXID_000,_T("\\MEDIA CARD\\background.jpg"),g_iScreenWidth,g_iScreenHeight))
	{
		RETAILMSG(DEBUG_GUI, (TEXT(": background texture FAILED!\r\n")));
	}
	m_bk.Initialize(IDC_TAG_BACKGROUND,NULL,m_pSkinManager->GetTexture(TEXID_000),NULL,TRUE);
	m_bk.SetScale(2.5f*g_iScreenWidth/cx,2.5f*g_iScreenHeight/cy);
	VIEW_STATE vs=m_bk.GetCurViewState();
	vs.fRotateX=-180;
	m_bk.SetIniViewState(&vs);
	////////////////////////////////////////////////////
// 	RETAILMSG(DEBUG_GUI,(_T("load background, takes %d ms\r\n"),GetTickCount()-dwt));
*/

// 	RenderIniBackground();

	//text:
	m_pTextGL = new CTextGL();
	//m_pTextGL->SetFontFile(_T("\\ResidentFlash\\LightningUI\\glFont\\font_xihei.bin"));


	//status bar:
	m_pStatusBar = new CStatusBarEx;
	m_pStatusBar->Initialize(0,NULL,m_pSkinManager,m_pTextGL,window,entry);


	//text:
	if(m_pConfig->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
	{
		m_pConfig->GetCurParam()->idFont = FONT_Arial;
	}
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_pTextGL->Initialize(m_pSkinManager,FontIDGL(m_pConfig->GetCurParam()->idFont)/*FONT_Helvetica*//*FONT_Segoe_UI*/,pix_ratio);
	//m_pTextGL->SetFont(m_pConfig->GetCurParam()->idFont);
	//set RIGHT_TO_LEFT reading order:
	if(m_pConfig->GetCurParam()->idLanguage == LANGUAGE_PERSIAN)
	{
		m_pTextGL->SetReadingOrder(FALSE);
	}

	//set status bar title:
	//m_pStatusBar->SetTitle((const wchar_t*)::LoadString( m_hInstance, IDS_MAIN_MENU+g_idLanguageOffset, NULL, 0 ));

// 	//keyboard:
// 	m_pKeyBoard = new CKeyBoardGL;
// 	m_pKeyBoard->Initialize(0,m_pSkinManager);
// 	m_pKeyBoard->Show(FALSE,FALSE);
// 	m_pKeyBoard->LinkTextGL(m_pTextGL);

// 	//explorer:
// 	m_pExplorer = new CExplorerGL;
// 	m_pExplorer->Initialize(0,m_pSkinManager);
// 	m_pExplorer->Show(FALSE,FALSE);
// 	m_pExplorer->LinkTextGL(m_pTextGL);

	/*
	//update software://///////////////////////////////////////////////
	if(m_pConfig->GetCurParam()->dwSoftwareUpdateFlag == 0)//nk updated,files not installed
	{
		//show windows:
		if(!m_bUIWindowShow)
		{
			ShowWindow(g_hMainWnd, SW_SHOW);
			ShowWindow(g_hUIWnd, SW_SHOW);
			m_bUIWindowShow = TRUE;
		}

// 		m_pConfig->GetCurParam()->dwSoftwareUpdateFlag = 1;
// 		m_pConfig->SaveConfig();
//
// 		//not user to remove update sd card and reset the device:
// 		RenderUpdateScreen(0);
//
// 		Sleep(INFINITE);
// 	}
// 	else if(m_pConfig->GetCurParam()->dwSoftwareUpdateFlag == 1)
// 	{
		if(InstallFiles())
		{
			m_pConfig->GetCurParam()->dwSoftwareUpdateFlag = 2;
			m_pConfig->SaveConfig();
		}
		else
		{
			RenderUpdateScreen(3);
			Sleep(INFINITE);
		}
	}
	////update software end//////////////////////////////////////////
	*/

#if 0
	{
    wchar_t m_strDeviceName[MAX_PATH];
    wchar_t strDestCA[MAX_PATH];
	wchar_t strSrcCA[MAX_PATH];
	unsigned char *ca_input;
	FILE* fp;

    wcscpy_s(m_strDeviceName, MAX_PATH, DeviceGetID());
    RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::Initialize]: ******************* %s **********************\r\n"),m_strDeviceName));
	swprintf_s(strDestCA,MAX_PATH,_T("\\StaticStore\\%s.ca"),m_strDeviceName);
    swprintf_s(strSrcCA,MAX_PATH,_T("\\USB Disk\\%s.ca"),m_strDeviceName);
    if(IsFileExists(strDestCA))
    {
		if(0 == _wfopen_s(&fp,strDestCA, L"rb"))
		{
			size_t fileSize=0;
			fseek (fp , 0 , SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			fileSize = min(8192,fileSize);
			ca_input =new unsigned char[fileSize];
			size_t readsize = fread(ca_input,fileSize,1,fp);
			fclose(fp);
			if (!DeviceInstallCA(ca_input, fileSize))
			{
				RETAILMSG(1,(_T("\n####Active#### DeviceInstallCA Failed! %s##### \n")));
                delete ca_input;
                RenderActivateScreen(m_strDeviceName);
			}
			delete ca_input;

            if(!DeviceIsActivated())
            {
				RETAILMSG(1,(_T("\n####Active#### DeviceIsActivated Failed! %s##### \n")));
                DeleteFile(strDestCA);
                RenderActivateScreen(m_strDeviceName);
            }
            else
            {
                RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::Initialize]: ******************* Activated **********************\r\n")));
            }
		}

    }
    else
    {
        RenderActivateScreen(m_strDeviceName);
    }

	}
#endif
// 	Render();

	//video window:
	m_pVideoWindow = new CVideoWindow;
	if(m_pVideoWindow)
	{
		m_pVideoWindow->Initialize(entry.hInstance,m_pConfig);
	}


	//smart bar window:
	m_pSmartBarVol = new CSmartBarVol;
	if(m_pSmartBarVol)
	{
		m_pSmartBarVol->Initialize(m_pSkinManager,SMART_BAR_VOLUME,window,entry.hInstance);
		m_listSmartBars.Add(m_pSmartBarVol,SMART_BAR_VOLUME);
	}

	m_pSmartBarPhone = new CSmartBarPhone;
	if(m_pSmartBarPhone)
	{
		m_pSmartBarPhone->Initialize(m_pSkinManager,SMART_BAR_PHONE,window,entry.hInstance);
		m_listSmartBars.Add(m_pSmartBarPhone,SMART_BAR_PHONE);
	}

	m_pSmartBarMedia = new CSmartBarMedia;
	if(m_pSmartBarMedia)
	{
		m_pSmartBarMedia->Initialize(m_pSkinManager,SMART_BAR_MEDIA,window,entry.hInstance);
		m_listSmartBars.Add(m_pSmartBarMedia,SMART_BAR_MEDIA);
	}

	m_pSmartBarDrop = new CSmartBarDrop;
	if(m_pSmartBarDrop)
	{
		m_pSmartBarDrop->Initialize(m_pSkinManager,SMART_BAR_DROP,window,entry.hInstance);
		m_listSmartBars.Add(m_pSmartBarDrop,SMART_BAR_DROP);
	}

	m_pSmartBarTopBanner = new CSmartBarTopBanner;
	if(m_pSmartBarTopBanner)
	{
		m_pSmartBarTopBanner->Initialize(m_pSkinManager,SMART_BAR_TOPBANNER,window,entry.hInstance);
		m_listSmartBars.Add(m_pSmartBarTopBanner,SMART_BAR_TOPBANNER);
	}

#if CVTE_EN_CAMERA
	m_pRearOverlay = new CRearOverlay;
	if(m_pRearOverlay)
	{
		m_pRearOverlay->Initialize(m_pSkinManager,SMART_BAR_REAR_OVERLAY,window,entry.hInstance);
		m_listSmartBars.Add(m_pRearOverlay,SMART_BAR_REAR_OVERLAY);
	}
#endif

#if CVTE_EN_ACCON_WARNING
	m_pSmartBarWarn = new CSmartBarWarn;
	if(m_pSmartBarWarn)
	{
		m_pSmartBarWarn->Initialize(m_pSkinManager,SMART_BAR_WARN,window,entry.hInstance);
		m_listSmartBars.Add(m_pSmartBarWarn,SMART_BAR_WARN);
	}
	CWarningProxy::GetInstance()->Initialize(m_pSmartBarWarn,window);
#endif

#if CVTE_EN_LED
	//led:
	#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO || CUSTOMER_ID == CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO)
	m_pLED = new CLED(255,255,255,0,FALSE,(m_pConfig->GetCurParam()->iPowerStatusBeforeACCOff==1?FALSE:TRUE));
	#else
	m_pLED = new CLED(m_pConfig->GetCurParam()->iLEDRedCurrent,
		    m_pConfig->GetCurParam()->iLEDGreenCurrent,
		    m_pConfig->GetCurParam()->iLEDBlueCurrent,
		    m_pConfig->GetCurParam()->iLEDBrightnessCurrent,
		    m_pConfig->GetCurParam()->bLedAnimEffect,
			(m_pConfig->GetCurParam()->iPowerStatusBeforeACCOff==1?FALSE:TRUE));
	#endif
#endif

	DWORD tc=GetTickCount();

	//com port:
	m_pCommander = new CCommanderEx;
	if(m_pCommander)
	{
        RETAILMSG(1, (TEXT("APP:MSG:CGUI_Fantasy: Initialize: Initialize COM3 115200.\r\n")));
		m_bCommanderInitialized = m_pCommander->Initialize(CommandProc,this,_T("COM3:"),CBR_115200,m_hMainWnd);
	}


	//audio codec:
#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
	m_pAudioCodec = new CAudioCodecST7418AIC3254(m_pCommander,g_hUIWnd,AspTypeFm1388);
#elif (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	m_pAudioCodec = new CAudioCodecAK7738(m_pCommander,g_hUIWnd,AspTypeAK7738);
#else

#endif

	//open initial panels:
	if(m_pConfig->GetCurParam()->bMCUSoftwareUpdateFlag)
	{
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_UPDATE,0));
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::Initialize]:enter update panel,take %d ms............\r\n"),GetTickCount()-tc));
	}
	else
	{
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN));
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_MAIN));
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_MAIN));
#if CVTE_EN_CARPLAY
        OpenPanels(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
#endif
#if CVTE_EN_ACCON_WARNING
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_WARNING,PAGE_ACCON_WARNING_MAIN));
#endif
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_WRAPPER));
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::Initialize]: load modules,take %d ms............\r\n"),GetTickCount()-tc));
	}

	m_hPlaySoundEffect = CreateEvent(NULL,FALSE,FALSE,NULL);

	//GPS:
	m_hEvtGPSMonitor = CreateEvent(NULL,FALSE,FALSE,NULL);
	EnableGPSMonitor(TRUE);

	//initialize MCU:
	//mcu software update:
	if(!m_pConfig->GetCurParam()->bMCUSoftwareUpdateFlag)
	{
 		if(m_pCommander && m_bCommanderInitialized)
 		{
 			m_pCommander->IniMCU();
 		}
	}

	//open canbus:
	#if USE_CVTE_CANBUS
	if(!m_pConfig->GetCurParam()->bMCUSoftwareUpdateFlag)
	{
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
	}
	#endif

	m_dwSystemReadyTick = GetTickCount();

	RETAILMSG(DEBUG_GUI,(_T("\r\nAPP:MSG:GUI:Main: [CGUI_Fantasy::Initialize] Finished %d\r\n"),GetTickCount()));


	//show windows:
	if(!m_bUIWindowShow)
	{
		ShowWindow(g_hMainWnd, SW_SHOW);
		ShowWindow(g_hUIWnd, SW_SHOW);
		m_bUIWindowShow = TRUE;
	}

	g_hTouchEnableGfxEvent = CreateEvent(NULL, FALSE, FALSE, TOUCH_EVENT_ENABLE_GFX_NAME);

#if 0
	//check font path:
	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\FontPath\\"));
	LPCTSTR path=reg.ValueSZ(_T("FontPath"));
	if(path ? (wcscmp(path,_T("\\ResidentFlash\\Font")) != 0) : TRUE)
	{
		RETAILMSG(1,(_T("==set font path: \\ResidentFlash\\Font \r\n")));
		reg.SetSZ(_T("FontPath"),_T("\\ResidentFlash\\Font"));
		RegFlushKey(HKEY_LOCAL_MACHINE);
		Sleep(1000);
		DoPowerSoftReset();
	}
#endif

	/*
	//test:
	TIME_ZONE_INFORMATION zone;
	GetTimeZoneInformation(&zone);

	RETAILMSG(1,(_T("zone.Bias: %d\r\n"),zone.Bias));
	RETAILMSG(1,(_T("zone.StandardBias: %d\r\n"),zone.StandardBias));
	RETAILMSG(1,(_T("zone.DaylightBias: %d\r\n"),zone.DaylightBias));
	RETAILMSG(1,(_T("zone.StandardName: %s\r\n"),zone.StandardName));
	RETAILMSG(1,(_T("zone.DaylightName: %s\r\n"),zone.DaylightName));
	RETAILMSG(1,(_T("zone.StandardDate: %d-%d-%d, %d:%d:%d\r\n"),
		zone.StandardDate.wYear,
		zone.StandardDate.wMonth,
		zone.StandardDate.wDay,
		zone.StandardDate.wHour,
		zone.StandardDate.wMinute,
		zone.StandardDate.wSecond,
		zone.StandardDate.wMilliseconds));
	RETAILMSG(1,(_T("zone.DaylightDate: %d-%d-%d, %d:%d:%d\r\n"),
		zone.DaylightDate.wYear,
		zone.DaylightDate.wMonth,
		zone.DaylightDate.wDay,
		zone.DaylightDate.wHour,
		zone.DaylightDate.wMinute,
		zone.DaylightDate.wSecond,
		zone.DaylightDate.wMilliseconds));

	CReg reg;
	reg.Open(HKEY_LOCAL_MACHINE,_T("Time Zones\\"));

	wchar_t temp[256];
	wchar_t val[256];
	wchar_t sub_key[256];

	struct tdata
	{
		LONG Bias;
		LONG StandardBias;
		LONG DaylightBias;
		SYSTEMTIME StandardDate;
		SYSTEMTIME DaylightDate;
	};

	while (reg.EnumKey(temp,256))
	{
		RETAILMSG(1,(_T("TimeZone: %s\r\n"),temp));

		swprintf_s(sub_key,256,_T("Time Zones\\%s\\"),temp);

		CReg regVal;
		tdata dat;
		regVal.Open(HKEY_LOCAL_MACHINE,sub_key);
		regVal.ValueSZ(_T("Display"),val,256);
		RETAILMSG(1,(_T("--Display: %s\r\n"),val));
		regVal.ValueSZ(_T("Std"),val,256);
		RETAILMSG(1,(_T("--Std: %s\r\n"),val));
		regVal.ValueSZ(_T("Dlt"),val,256);
		RETAILMSG(1,(_T("--Dlt: %s\r\n"),val));
		regVal.ValueBinary(_T("TZI"),(LPBYTE)&dat,44);
		RETAILMSG(1,(_T("--Bias: %d\r\n"),dat.Bias));
		RETAILMSG(1,(_T("--StandardBias: %d\r\n"),dat.StandardBias));
		RETAILMSG(1,(_T("--DaylightBias: %d\r\n"),dat.DaylightBias));
	}
	*/
	return TRUE;
}
void CGUI_Fantasy::OnBacklightChanged(INT iLevel)
{

// 	CPanelGL *panel=m_listPanels.GetItemByID(UI_SETTINGCORE);
// 	if(panel)
// 	{
// 		((CPanelSettings*)panel)->OnBacklightChanged(iLevel);
// 	}

	//test:
#if CVTE_EN_LED
#if (CVTE_DEF_DISPLAY_TYPE == CVTE_DISPLAY_TYPE_U621P)
	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnBacklightChanged]: iLevel:%d\r\n"),iLevel));

	if(m_pLED)
	{
		int iset;
		
		if(m_iScrnBrightness == 1)
		{
			//NightMode
			if(iLevel>=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END) //GetNearInt((iLevel-20)*7.0f/35.0f);
				iset = 7;
			else
				iset = GetNearInt((iLevel-CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START)*7.0f/(CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END-CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START));

		}
		else
		{
			//DayMode
			if(iLevel>=CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END) //GetNearInt((iLevel-20)*7.0f/35.0f);
				iset = 7;
			else
				iset = GetNearInt((iLevel-CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START)*7.0f/(CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END-CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START));
		}

		if(iset<0)
			iset=0;
		else if(iset>7)
			iset=7;
	    //RETAILMSG(DEBUG_GUI,(_T("MSG:GUI:Main: [CGUI_Fantasy::OnBacklightChanged]: SetLedParam(1,%d)\r\n"),iset));

		m_pLED->SetLedParam(1,iset);
	}
    else
    {
	    RETAILMSG(DEBUG_GUI,(_T("ERR:GUI:Main: [CGUI_Fantasy::OnBacklightChanged]: m_pLED is NULL\r\n")));
    }

#endif
#endif
}

void CGUI_Fantasy::OnCameraTypeChanged(UINT uCameraType)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnCameraTypeChanged]: uCameraType:%d\r\n"),uCameraType));
	CPanelGL *panel;

	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnCameraTypeChange(uCameraType);
	}
}

void CGUI_Fantasy::OnRearColorSignalChange(UINT uColorSignal)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnRearColorSignalChange]: uColorSignal:%d\r\n"),uColorSignal));
	CPanelGL *panel;

	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnRearColorSignalChange(uColorSignal);
	}
}

void CGUI_Fantasy::OnUSBHUBAttached()
{
	ShowNotificationBox(m_hMainWnd, 0, NULL, NULL, -1, GetResourceString(IDS_USB_HUB_UNSUPPORTED), NULL, 0, TRUE, 3000);
}

void CGUI_Fantasy::OnPanelLightChange()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnPanelLightChange]: \r\n")));
	CPanelGL *panel;

	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnPanelLightChange();
	}
}

void CGUI_Fantasy::OnLCDBrightnessChange()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnLCDBrightnessChange]: \r\n")));
	CPanelGL *panel;

	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnLCDBrightnessChange();
	}
}

void CGUI_Fantasy::OnHeadLightOn(BOOL bOn, UINT nBk)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnHeadLightOn]: bOn:%d\r\n"),bOn));
	CPanelGL *panel;

	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnHeadLightOn(bOn, nBk);
	}
}
void CGUI_Fantasy::OnLoadCarPlayService()
{
	KillTimer(g_hUIWnd,TIMER_LOAD_CARPLAY_MODULE);
#if CVTE_EN_CARPLAY
	if(!CCarplayAdapter::GetInstance()->IsInitialized())
	{
		//carplay start:
		RETAILMSG(1,(_T(".....start CarPlay service.... \r\n")));
		CCarplayAdapter::GetInstance()->SetSystemPowerState(m_idPowerState);
		CCarplayAdapter::GetInstance()->Initialize(m_pSkinManager,
			m_hMainWnd,
			DeviceActivateProc,
#if CVTE_EN_CPMAC
			DeviceSetMACProc,
#else
			NULL,
#endif
			this);
		CCarplayAdapter::GetInstance()->SetReverseMode(m_bOnReverseMode);
	}
#endif
}
void CGUI_Fantasy::OnAutoOpenSrc()
{
	static DWORD dwlast=0;
	RETAILMSG(DEBUG_GUI, (TEXT("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));

#if CVTE_EN_DYNAMIC_LOGO
	static int waitcount=0;

	if(!DynamicLogoFinish() && waitcount < 30) //Wait timeout 6s (200ms * 30)
	{
		waitcount++;
		//RETAILMSG(1,(_T("MSG:[%s]: waitcount[%d]\r\n"), TEXT(__FUNCTION__),waitcount));
		return;
	}
	RETAILMSG(1,(_T("MSG:[%s]: ++\r\n"), TEXT(__FUNCTION__)));
	m_pCurPanel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
	RenderPanels();
#endif

	DWORD tick=GetTickCount();
	if(!m_bMCUUpdating && !m_bMCUPackageUpdated && tick-m_dwSystemReadyTick>3000 && tick-dwlast>3000)
	{
		dwlast=GetTickCount();

		RETAILMSG(1,(_T("#### Connect to MCU Failed! re-connect... ###\r\n")));
		//reconnect:
		if(m_pCommander)
		{
			if(!m_bCommanderInitialized)
			{
                RETAILMSG(1, (TEXT("APP:MSG:OnAutoOpenSrc: Initialize COM3 CBR_115200.\r\n")));
				m_bCommanderInitialized = m_pCommander->Initialize(CommandProc,this,_T("COM3:"),CBR_115200,m_hMainWnd);
			}
			if(m_bCommanderInitialized)
			{
				m_pCommander->IniMCU();
			}
		}
	}

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
	//wait for audio codec initialize finish:
	if(m_pAudioCodec)
	{
		if(!m_pAudioCodec->IsCodecIni())
		{
			m_pAudioCodec->IniCodec();
			m_pAudioCodec->SetVolume(m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia);
			m_pAudioCodec->SetAltVolume(m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS);
			m_pAudioCodec->SetLimitVolume(m_bOnReverseMode);
		}
	}
#endif

	if(!m_bMCUUpdating && m_bMCUPackageUpdated && /*IsPowerOn()*/m_bManualPowerOn && !m_bOpenSRC && m_bSystemReady/* && !m_bOnReverseMode*/)
	{
		//back light on:
		if(m_bBacklightOFF)
		{
            ScreenOn();
		}

		/*
		#if CVTE_EN_CARPLAY
		//carplay start:
		RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnAutoOpenSrc]: GetInstance()->Initialize \r\n")));
		CCarplayAdapter::GetInstance()->Initialize(m_pSkinManager, m_hMainWnd, DeviceActivateProc, this);
		CCarplayAdapter::GetInstance()->SetReverseMode(m_bOnReverseMode);
		#endif
		*/

		///////////////////////////////////////////////////////
		if(m_bOnReverseMode)
		{
			m_bResumeSRCAfterRearView = TRUE;
			RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnAutoOpenSrc]: rearview on, switch home page\r\n")));
			m_pCurPanel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);

			//default audio:
			//SwitchAudioSRC(SRC_AUDIO_ARM);
		}
		else
		{

			AutoOpenSRC();
			/*
#if CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON
			//
			RETAILMSG(1,(_T("MSG[CGUI_Fantasy::OnAutoOpenSrc]: #### Auto Enter Home Page After ACC ON ####\r\n")));
#if CVTE_EN_KEY_PROXY
			CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
			CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
#else
			OnMediaKey(KEY_MEDIA_PLAY);
#endif	
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
#endif
			*/
		}

#if (CVTE_EN_GESTURE_AIR == 1)
		CGestureProxy::GetInstance()->Initialize(m_pSkinManager, m_hMainWnd);
		#if CVTE_EN_LED
		CGestureProxy::GetInstance()->LinkLED(m_pLED);
		#endif
		CGestureProxy::GetInstance()->StartGestureDetect();
		//CGestureProxy::GetInstance()->PlayGestureID();
		//CGestureProxy::GetInstance()->PlayGestureDetect();
#endif
		m_dwOpenSRCTick = GetTickCount();
		m_bOpenSRC=TRUE;

		KillTimer(g_hUIWnd,TIMER_AUTO_OPEN_SRC);
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnAutoOpenSrc]: end %d\r\n"),GetTickCount()));
	}

}

void CGUI_Fantasy::AutoOpenSRC()
{
	//auto start:
	//掉电前的源信息保存在注册表里
	UINT mode=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
	UINT para=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRCPara;

	RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::AutoOpenSRC]: Switch to open SRC: 0x%x,para:0x%x............%d\r\n"),mode,para,GetTickCount()));

	UINT idclass=GET_PANEL_CLASS_ID(mode);
	UINT idpage=GET_PANEL_PAGE_ID(mode);

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	if (m_pAudioCodec)
	{
		if (!m_pAudioCodec->IsCodecIni())
		{
			RETAILMSG(1,(_T("MSG:[%s]: => IniCodec \r\n"),TEXT(__FUNCTION__)));

			m_pAudioCodec->IniCodec();
			m_pAudioCodec->MuteAMP(TRUE);
			m_pAudioCodec->Mute(TRUE);
			m_pAudioCodec->SetMainVolume(0);
			m_pAudioCodec->SetAltVolume(m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS);
			m_pAudioCodec->SetLimitVolume(m_bOnReverseMode,REARVIEW_VOLUME_DEFAULT);
			//m_pAudioCodec->MuteAMP(FALSE);
			//m_pAudioCodec->SetMainVolumeTarget(m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia);
			//m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,500);
		}
	}
#endif

	if(idclass == UI_CLASS_BT)
	{
		if(m_idValidSrcList[MEDIA_TYPE_A2DP])
		{
			mode = MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP);
		}
		else
		{
			mode = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			g_AccOnSRCToSwicth = MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP);
		}
	}
	else if(idclass == UI_CLASS_IPOD)
	{
		if(m_idValidSrcList[MEDIA_TYPE_IPOD])
		{
			mode = MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN);
		}
		else
		{
			mode = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			g_AccOnSRCToSwicth = MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN);
		}
	}
	else if(idclass == UI_CLASS_CARPLAY)
	{
		if(m_bCarPlayConnected)
		{
			mode = MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER);
		}
		else
		{
			mode = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
	}
	else if(idclass == UI_CLASS_ANDROID_AUTO)
	{
		if(m_bAndroidAutoConnect)
		{
			mode = MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER);
		}
		else
		{
			mode = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		}
	}
	else if(idclass == UI_CLASS_MHL ||
		idclass == UI_CLASS_PHONELINK ||
		idclass == UI_CLASS_EASYCONNECTED || 
		idclass == UI_CLASS_WEBLINK)
	{
		//invalid open source, switch to home page
		RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::AutoOpenSRC]: invalid open source, switch to home page!!! idclass[%d]\r\n"),idclass));
		mode = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
	}
	else
	{
		RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::AutoOpenSRC]: open source, idclass[%d]\r\n"),idclass));
		mode = MAKE_PANEL_ID(idclass,0);
	}

	if(!OpenPanels(mode))
	{
		mode = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
// 		mode = MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_MAIN);
	}

	CPanelGL *panelTarget = NULL;
	panelTarget=m_listPanels.GetItemByID(mode);

	if(!panelTarget)
	{
		mode=MAKE_PANEL_ID(idclass,-1);
		panelTarget=m_listPanels.GetItemByID(mode);
	}

	if(panelTarget)
	{
		panelTarget->OnEnterPanel(0,ANIM_NONE);

		if(IsPanelHasValidAudioSrc(mode))
		{
			//[BUG]: <0002748> Play USB->Turn off ACC->Turn on ACC->Reverse->Quit reverse after led light on，USB display no sound
			RETAILMSG(DEBUG_GUI,(_T("MSG[CGUI_Fantasy::AutoOpenSRC]: SwitchMediaClassSRC\r\n")));
			SwitchMediaClassSRC(mode,panelTarget->GetPanelParam());
			if (IsPanelAutoUnMute(mode))
			{
				OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(400));
			}

		}
		else
		{
			//Default:
			//RETAILMSG(DEBUG_GUI,(_T("MSG[CGUI_Fantasy::AutoOpenSRC]: SwitchAudioSRC\r\n")));
			//SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
		}
		m_pConfig->GetCurParam()->idCurUI = mode;
	}

	SwitchPage(GET_PANEL_CLASS_ID(mode),GET_PANEL_PAGE_ID(mode),PAGE_SWITCH_ANI_NONE/*,para*/);

	//auto start navi:
	if(m_bAutoStartNavi)
	{
// 		SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN,PAGE_SWITCH_ANI_NONE);
		RETAILMSG(DEBUG_GUI,(_T("APP:MSG:GUI:Main: [CGUI_Fantasy::AutoOpenSRC]: Auto Start NAVI \r\n")));
		SetTimer(g_hUIWnd,TIMER_AUTO_START_NAVI,1000,NULL);
	}

// 	m_dwOpenSRCTick = GetTickCount();
// 	m_bOpenSRC=TRUE;

#if CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON
	//
	RETAILMSG(1,(_T("MSG[CGUI_Fantasy::OnAutoOpenSrc]: #### Auto Enter Home Page After ACC ON ####\r\n")));
#if CVTE_EN_KEY_PROXY
	CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
	CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
#else
	OnMediaKey(KEY_MEDIA_PLAY);
#endif	
	SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
#endif

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	if (m_pAudioCodec)
	{
		//Can send Unmute AMP cmd to MCU too early.
		if (m_pAudioCodec->IsCodecIni())
		{
			////m_pAudioCodec->MuteAMP(FALSE);
			m_pAudioCodec->SetMainVolumeTarget(m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia);
			m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,500);		
		} 
	}
#endif
	SetTimer(g_hUIWnd,TIMER_AUTO_LOAD_MODULES,2000,NULL);
}

BOOL CGUI_Fantasy::IsIniUIReady()
{
	return m_bOpenSRC;
}

void CGUI_Fantasy::OnSystemReady()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnSystemReady]\r\n")));

	m_bSystemReady=TRUE;
	m_dwSystemReadyTick = GetTickCount();

	/*
	//time stuff:
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	if(systime.wYear==2000)
	{
		systime.wYear=2015;
		systime.wMonth=10;
		systime.wDay=10;
		systime.wHour=10;
		systime.wMinute=10;
		systime.wSecond=10;
		systime.wMilliseconds=10;
		SetLocalTime(&systime);
	}

	RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::OnSystemReady]: bMCUSoftwareUpdateFlag:%d\r\n"),m_pConfig->GetCurParam()->bMCUSoftwareUpdateFlag));
*/

#if CVTE_EN_DYNAMIC_LOGO
	RenderDynamicLogoBk();

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		DWORD dwVol = 0xFFFFFFFF;
		((CPanelMediaWrapper*)panel)->PlayDynamicLogo(dwVol);
	}
#endif

	//mcu software update:
	if(m_pConfig->GetCurParam()->bMCUSoftwareUpdateFlag)
	{
		SwitchPage(UI_CLASS_UPDATE,0,PAGE_SWITCH_ANI_NONE);
	}
	else
	{
		//switch to open src:
		DWORD dwTime=200;
		if(!SetTimer(g_hUIWnd,TIMER_AUTO_OPEN_SRC,dwTime,NULL))
		{
			RETAILMSG(DEBUG_GUI, (TEXT("ERR[CGUI_Fantasy::OnSystemReady]: SetTimer FAILED! :%d--- \r\n"),TIMER_AUTO_OPEN_SRC));
		}
		else
		{
			RETAILMSG(DEBUG_GUI, (TEXT("MSG:[CGUI_Fantasy::OnSystemReady]: Auto load src after :%d--- \r\n"),dwTime));
		}

		//start a timer to load carplay service:
		SetTimer(g_hUIWnd,TIMER_LOAD_CARPLAY_MODULE,3000,NULL);
	}
}
BOOL CGUI_Fantasy::InitializeEGLWorld(void)
{
// 	glDisable(GL_ALPHA_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
// 	glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA);

// glCullFace(GL_BACK);
// glEnable(GL_CULL_FACE);

// glEnable(GL_MULTISAMPLE);
// glSampleCoverage(1,GL_FALSE);

	glClearColor(0.0f,0.0f,0.0f,0.0f);
// 	glClearColor(0.1f,0.36f,0.5f,1.0f);//
// 	glClearColor(45.0f/256.0f,62.0f/256.0f,80.0f/256.0f,1.0f);//


// glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	glClearDepthf(1.0f);

// 	glEnable(GL_TEXTURE_2D);
// 	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
//   	glFrustumf(-200.0f, 200.0f, -120.0f, 120.0f, 400.0f, 12800.0f);
//  	glFrustumf(-256.0f, 256.0f, -192.0f, 192.0f, 400.0f, 12800.0f);
//  	glFrustumf(-GLfloat(g_iClientWidth/4), GLfloat(g_iClientWidth/4), -GLfloat(g_iClientHeight/4), GLfloat(g_iClientHeight/4), 400.0f, 12800.0f);
// 	glFrustumf(-GLfloat(g_iScreenWidth/4), GLfloat(g_iScreenWidth/4), -GLfloat(g_iScreenHeight/4), GLfloat(g_iScreenHeight/4), 400.0f, 12800.0f);

	glFrustumf(-GLfloat(g_iScreenWidth/4), GLfloat(g_iScreenWidth/4), -GLfloat(g_iScreenHeight/4), GLfloat(g_iScreenHeight/4), GLfloat(g_iScreenWidth), 16000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

// 	RECT rc;
// 	::GetClientRect(m_hMainWnd,&rc);

	glViewport(0, 0,g_iScreenWidth,g_iScreenHeight);
// 	glViewport((g_iScreenWidth-g_iClientWidth)/2+rc.left, (g_iScreenHeight-g_iClientHeight)/2+rc.top,rc.right-rc.left, rc.bottom-rc.top);
// 	glViewport(0, 0,400, 300);


	//light:
// glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_color[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat lmode_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

// glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
// glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );

	GLfloat position[] = {0.0f, 0.0f,0.0f, 1.0f }; // w=1, positional light, positioned at (0, 0, 0) current coordinate system
	GLfloat direction[] = {0.0f, 0.0f, -1.0f };
	GLfloat cut_off[] = {20.0f};
	GLfloat exponent[] = {10.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, cut_off);
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);

	glLightfv(GL_LIGHT0,GL_AMBIENT,lmode_ambient);
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_color );
	glLightfv( GL_LIGHT0, GL_SPECULAR, mat_specular );
	glLightfv( GL_LIGHT0, GL_EMISSION, lmode_ambient );


	//fog
// 	GLfloat fog_color[]={0.0,0.0,0.0,0};
// 	GLfloat fog_start[]={4500};
// 	GLfloat fog_end[]={5000};
// 	GLfloat fog_density[]={0.25};
// 	GLfloat fog_mode[]={GL_LINEAR};
// 	glFogfv(GL_FOG_START,fog_start);
// 	glFogfv(GL_FOG_END,fog_end);
// 	glFogfv(GL_FOG_MODE,fog_mode);
// 	glFogfv(GL_FOG_COLOR,fog_color);
// 	glFogfv(GL_FOG_DENSITY,fog_density);

	return TRUE;
}
BOOL CGUI_Fantasy::IsReady(void)
{
	return /*(m_pStatusBar?m_pStatusBar->IsReady():TRUE) &&*/ ( m_pCurPanel?m_pCurPanel->IsReady():TRUE) ;
}
void CGUI_Fantasy::RenderWorld(void)
{
	glLoadIdentity();

	glTranslatef(0,0,-g_iScreenWidth*5.0f);

	if(m_pSkinManager->GetConfig()->GetCurParam()->bShowWallpaper)
	{
		m_bk.SetLuminance(m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
 		m_bk.Show(m_pCurPanel?m_pCurPanel->IsBackgroundVisible():TRUE);
 		m_bk.Render();
	}

/*	for (int i=0;i<8;i++)
	{
		VIEW_STATE vs=m_pop[i].GetCurViewState();
		if(vs.fTransY>200*5)
		{
			vs.fTransY = -200*5;
			vs.fRotateZ=0;
			vs.fScaleX=0.5f;
			vs.fScaleY=0.5f;

			m_pop[i].SetCurViewState(&vs);
		}
		else
		{
			vs.fTransY+=10;
			vs.fRotateZ+=1;
			vs.fScaleX+=0.01f;
			vs.fScaleY+=0.01f;

			m_pop[i].SetCurViewState(&vs);
		}
		m_pop[i].Render();
	}
	*/
}

void CGUI_Fantasy::RenderLogo(void)
{
#if CVTE_EN_IDLE_LOGO
	//RETAILMSG(1, (TEXT("RenderLogo :%d\r\n"),GetTickCount()-m_dwLogoShadeTick));
	if(m_bLogoShade)
	{
		if(m_idLogoShowStyle == 0)//logo1
		{
			if(GetTickCount()-m_dwLogoShadeTick>2000 || m_logoAlpha <= 0)
			{			
				m_bLogoShade = FALSE;
			}

			if(m_tagLogo.HasValidTeture())
			{
				m_tagLogo.SetAlpha(m_logoAlpha);
				m_tagLogo.Render();
			}
			else
			{
				m_bLogoShade = FALSE;
			}
		}
		else if(m_idLogoShowStyle == 1)//logo2
		{
			if(GetTickCount()-m_dwLogoShadeTick>1350 || m_logo2Alpha <= 0)
			{			
				m_bLogoShade = FALSE;
			}

			if(m_tagLogo2.HasValidTeture())
			{
				m_tagLogo2.SetAlpha(m_logo2Alpha);
				m_tagLogo2.Render();
			}
			else
			{
				m_bLogoShade = FALSE;
			}
		}
		else//logo1 and logo2
		{
			//static DWORD dwTickCount;
			DWORD dwTickCount = GetTickCount();
			if(dwTickCount-m_dwLogoShadeTick<=900)
			{
				m_logoAlpha+=0.040f;
				if(dwTickCount-m_dwLogoShadeTick>=400)
				{
					m_logo2Alpha+=0.040f;
				}
			}
	//			else if(dwTickCount-m_dwLogoShadeTick>800 && dwTickCount-m_dwLogoShadeTick<=1000)
	//			{
	//				m_logoAlpha-=0.05f;
	//				m_logo2Alpha+=0.040f;
	//			}
			else if(dwTickCount-m_dwLogoShadeTick>900 && dwTickCount-m_dwLogoShadeTick<=1300)
			{
				m_logoAlpha-=0.05f;
				m_logo2Alpha+=0.050f;
			}
			else if(dwTickCount-m_dwLogoShadeTick>1300)
			{
				m_logoAlpha-=0.060f;
				m_logo2Alpha-=0.040f;
			}
			else
			{
			}

			if(m_logoAlpha>1.0f)
			{
				m_logoAlpha = 1.0f;
			}
			else if(m_logoAlpha < 0)
			{
				m_logoAlpha = 0;
			}

			if(m_logo2Alpha>1.0f)
			{
				m_logo2Alpha = 1.0f;
			}
			else if(m_logo2Alpha < 0)
			{
				m_logo2Alpha = 0;
			}

			if(m_tagLogo.HasValidTeture())
			{
				m_tagLogo.SetAlpha(m_logoAlpha);
				m_tagLogo.Render();
			}

			if(m_tagLogo2.HasValidTeture())
			{
				m_tagLogo2.SetAlpha(m_logo2Alpha);
				m_tagLogo2.Render();
			}

			if(m_logoAlpha <= 0 && m_logo2Alpha <= 0)
			{
				m_bLogoShade = FALSE;
			}
		}
	}
#endif
}

void CGUI_Fantasy::RenderPanels(void)
{
	glLoadIdentity();

	glTranslatef(0,0,-g_iScreenWidth*2.0f);

	if(m_pCurPanel)
	{
		m_pCurPanel->Render();
	}

}
void CGUI_Fantasy::RenderIdleScreen()
{
	glLoadIdentity();

	glTranslatef(0,0,-g_iScreenWidth*2.0f);

	if(!m_pIdlePanel)
	{
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_IDLE,0));
		m_pIdlePanel = m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IDLE,0));
	}

	if(m_pIdlePanel)
	{
		m_pIdlePanel->Render();
	}
}
void CGUI_Fantasy::RenderStatusBar(void)
{
	/*
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-g_iScreenWidth*2.0f);

 	if(m_pCurPanel)
	{
		if(m_pStatusBar)
		{
			m_pStatusBar->Show(m_pCurPanel->IsStatusBarVisible());
			m_pStatusBar->Render();
		}
	}
	*/

}
void CGUI_Fantasy::OnLoadModules()
{
    RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: ++\r\n")));

	if(m_bOpenSRC && !m_bAnimating && !m_bOnReverseMode)
	{
		if(GetTickCount() - m_dwOpenSRCTick > 3000)
		{

// #if CVTE_EN_CARPLAY
// 			if(!CCarplayAdapter::GetInstance()->IsInitialized())
// 			{
// 				//carplay start:
// 				RETAILMSG(1,(_T(".....start CarPlay service.... \r\n")));
// 				CCarplayAdapter::GetInstance()->Initialize(m_pSkinManager,
// 														   m_hMainWnd,
// 														   DeviceActivateProc,
// 														   #if CVTE_EN_CPMAC
// 														   DeviceSetMACProc,
// 														   #else
// 														   NULL,
// 														   #endif
// 														   this);
// 				CCarplayAdapter::GetInstance()->SetReverseMode(m_bOnReverseMode);
// 			}
// #endif

			if(g_SRCToSwicthAfterLoadModule != -1)
			{
				RETAILMSG(1,(_T("==>[%s] Auto switch source to 0x%x after ACC on.\r\n"),_T(__FUNCTION__),g_SRCToSwicthAfterLoadModule));
				SwitchPage(GET_PANEL_CLASS_ID(g_SRCToSwicthAfterLoadModule),GET_PANEL_PAGE_ID(g_SRCToSwicthAfterLoadModule));
				g_SRCToSwicthAfterLoadModule = -1;
			}
			//20160428 OpenPanels Carplay in Initialize
			/*
			if(!m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER)))
			{
				RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnLoadModules]: MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER) %d\r\n"),GetTickCount()));
				OpenPanels(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
			}
			*/
			if (FALSE)
			{

			}
#if CVTE_EN_IPOD
			else if(!m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN)))
			{
				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN) %d\r\n"),GetTickCount()));
				OpenPanels(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN));
			}
#endif
// 			else if(!m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER)))
// 			{
// 				RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnLoadModules]: MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER) %d\r\n"),GetTickCount()));
// 				OpenPanels(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
// 			}
// 			else if(!m_listPanels.GetItemByID(UI_MHL))
// 			{
// 				RETAILMSG(DEBUG_GUI,(_T("****OnLoadModules...........UI_MHL %d\r\n"),GetTickCount()));
// 				OpenPanels(UI_MHL);
// 			}
// 			else if(!m_listPanels.GetItemByID(UI_PHONELINK))
// 			{
// 				RETAILMSG(DEBUG_GUI,(_T("****OnLoadModules...........UI_PHONELINK %d\r\n"),GetTickCount()));
// 				OpenPanels(UI_PHONELINK);
// 			}
#if CVTE_EN_EASYCONNECT
			else if(m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType==ANDROID_LINK_EASYCONNECTED
				&& !m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER)))
			{
				OpenPanels(MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER));
			}
#endif
#if CVTE_EN_WEBLINK
			else if(!m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER))
				#if CVTE_EN_COMBINE_ANDROID_LINK
				&& (m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType == DEVICE_TYPE_WEBLINK)
				#endif
				)
			{
				OpenPanels(MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER));
			}
#endif
#if CVTE_EN_ANDROID_AUTO
			else if(!m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER))
				#if CVTE_EN_COMBINE_ANDROID_LINK
				&& (m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType == DEVICE_TYPE_ANDROID_AUTO)
				#endif
				)
			{
				OpenPanels(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
			}
#endif
#if 0
			else if(!m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_MAIN)) && !m_bCarPlayConnected)
			{
				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: UI_CLASS_BT %d\r\n"),GetTickCount()));
				OpenPanels(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_MAIN));
			}
#endif
#if CVTE_EN_DAB
			//DAB
			else if(!m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_MAIN)))
			{
				if(GetTickCount() - m_dwOpenSRCTick > 10000)
				{
					if(!CDABAdapter::GetInstance()->GetInitialize())
					{
						RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: Initialize DABAdapter \r\n")));
						CDABAdapter::GetInstance()->Initialize(5,/*230400*/38400);
					}
                    OpenPanels(MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_MAIN));
				}
			}
#endif
			else
			{
				KillTimer(g_hUIWnd,TIMER_AUTO_LOAD_MODULES);
				RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: end %d\r\n"),GetTickCount()));
				m_bModulesLoaded = TRUE;
			}
		}
	}
	else
	{
		//RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnLoadModules]: rearview enable skip load %d\r\n"),GetTickCount()));
		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnLoadModules]: m_bOpenSRC[%d] m_bAnimating[%d] m_bOnReverseMode[%d]\r\n"),m_bOpenSRC, m_bAnimating, m_bOnReverseMode));
	}

}
void CGUI_Fantasy::Render(void)
{
	if(m_bWaitingForPowerOFF || !m_pCurPanel)
	{
		return;
	}

	static DWORD lastRenderTick=GetTickCount();
	DWORD dwStart=GetTickCount();

	glClear(GL_COLOR_BUFFER_BIT );//| GL_DEPTH_BUFFER_BIT

// 	glEnable(GL_SCISSOR_TEST);
// 	glScissor((g_iScreenWidth-g_iClientWidth)/2,(g_iScreenHeight-g_iClientHeight)/2+80,g_iClientWidth,g_iClientHeight);

	//20160428
	if(m_bDeviceActivated && m_bIsSetMAC)
	{
		if(!m_bOnReverseMode)
		{
		//	RenderWorld();
		//	RenderPanels();
		//	RenderStatusBar();

#if CVTE_EN_IDLE_LOGO
			if(m_bLogoShade)
				RenderLogo();
			else
				RenderPanels();
			m_bAnimating = ((!IsReady() || m_bLogoShade) && m_bEnableSwap);
#else
			RenderPanels();
			m_bAnimating = (!IsReady() && m_bEnableSwap);
#endif

			//m_bAnimating = (!IsReady() && m_bEnableSwap);

			//calculate frame time:
			DWORD dwFrameTime=100;
			const DWORD dwMin=20;//500;//
			const DWORD dwNorm=500;

			if(IsPowerOn() || m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen!=IDLE_SCREEN_NONE || m_bOnReverseMode)
				dwFrameTime = (m_bAnimating?dwMin:(m_pCurPanel?m_pCurPanel->GetFrameTime():dwNorm));
			else
				dwFrameTime = (m_bAnimating?dwMin:1000);

			if(m_pCurPanel && m_pStatusBar)
			{
				if(dwFrameTime>40 && m_pCurPanel->IsStatusBarVisible() && !m_pStatusBar->IsReady())
				{
					dwFrameTime = 40;
				}
			}

			//
			if(m_bEnableSwap)//&& !m_bOnReverseMode)
			{
				SwapBuffers();
			}

			//test framerate:
			DWORD thisTick = GetTickCount();
			m_dwFrameRenderTime = thisTick-dwStart;
			if(thisTick-lastRenderTick>1000 && m_bAnimating)
			{
				RETAILMSG(1,(_T("## ft:%d,cost:%d,bAnimat:%d,dwFrameTime:%d\r\n"),m_dwFrameTime,m_dwFrameRenderTime,m_bAnimating,dwFrameTime));
				lastRenderTick=thisTick;
			}

			//set new frame time:
#if CVTE_EN_IDLE_LOGO
			if(m_bLogoShade)
				SetFrameTime(10);
			else
				SetFrameTime(max(dwFrameTime,m_dwFrameRenderTime+5));
#else
			SetFrameTime(max(dwFrameTime,m_dwFrameRenderTime+5));
#endif
		}
		else
		{
			if(m_bEnableSwap)
			{
				SwapBuffers();
			}
			SetFrameTime(500);
		}
	}
	else
	{
		
		if (!m_bDeviceActivated)
		{
			#if CVTE_EN_CARPLAY
			wchar_t *strDeviceName=CCarplayAdapter::GetInstance()->GetDeviceName();
			RenderActivateScreen(strDeviceName);
			#elif CVTE_EN_ANDROID_AUTO
			//notify android auto
			CPanelGL *panel_aap=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
			if(panel_aap)
			{
				wchar_t *strDeviceName = ((CPanelAndroidAuto*)panel_aap)->OnNativeNaviMsg(iNaviCode,lParam);
				RenderActivateScreen(strDeviceName);
			}
			#endif
			SetFrameTime(500);
		}
		#if CVTE_EN_CARPLAY
		else
		{
			RenderSetMACScreen();
			SetFrameTime(100);
		}
		#endif

	}

// 	//test: auto power off
// 	static BOOL breset=FALSE;
// 	if(!breset && m_bOpenSRC && GetTickCount() - m_dwOpenSRCTick > 50000)
// 	{
// 		breset = TRUE;
// 		::PostMessage(g_hUIWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,0);
// 	}
}
BOOL CGUI_Fantasy::IsPanelOpened(UINT idPanel)
{
	return m_listPanels.GetItemByID(idPanel)!=NULL;
}
BOOL CGUI_Fantasy::OpenPanels(UINT idPanel)
{
	BOOL bRet = TRUE;
// 	CPanelGL *pPanel=NULL;

	if(m_listPanels.GetItemByID(idPanel))
	{
		//already exist
		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OpenPanel]: Panel already exists! :0x%x ###\r\n"),idPanel));
		return TRUE;
	}
	else
	{
		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OpenPanel]:0x%x.********************\r\n"),GET_PANEL_CLASS_ID(idPanel)));

		switch (GET_PANEL_CLASS_ID(idPanel))
		{
		case UI_CLASS_HOME:
			{
				CPanelHomePage *pPanel=new CPanelHomePage;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
			}
			break;

		case UI_CLASS_RADIO:
			{
				CPanelRadioLibrary *pLibrary=new CPanelRadioLibrary;
				pLibrary->Initialize(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_LIBRARY),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pLibrary->LinkStatusBar(m_pStatusBar);
				pLibrary->LinkCommander(m_pCommander);
				m_listPanels.Add(pLibrary,MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_LIBRARY));

				CPanelRadioMain *pMain=new CPanelRadioMain;
				pMain->Initialize(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pMain->LinkStatusBar(m_pStatusBar);
				pMain->LinkCommander(m_pCommander);
				m_listPanels.Add(pMain,MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_MAIN));

				CPanelRadioWrapper *pWrapper=new CPanelRadioWrapper;
				pWrapper->Initialize(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pWrapper->LinkPages(pMain,pLibrary);
				pWrapper->LinkCommander(m_pCommander);
				pWrapper->LinkSmartBarMedia(m_pSmartBarMedia);
				m_listPanels.Add(pWrapper,MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_WRAPPER));

			}
			break;
		case UI_CLASS_BT:
			{
				CPanelBluetoothWrapper *pWrapper=new CPanelBluetoothWrapper;
				m_pBluetoothWrapper = pWrapper?pWrapper:NULL;
				if(m_bCarPlayConnected)
				{
					pWrapper->OnCarPlayMsg(CARPLAY_CONNECT_STATUS,TRUE);
				}
				pWrapper->Initialize(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pWrapper->LinkStatusBar(m_pStatusBar);
				pWrapper->LinkCommander(m_pCommander);
				pWrapper->LinkSmartBarPhone(m_pSmartBarPhone);

				CPanelBluetoothMain *pMain=new CPanelBluetoothMain;
				pMain->Initialize(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pMain->LinkStatusBar(m_pStatusBar);
				pMain->LinkCommander(m_pCommander);
				m_listPanels.Add(pMain,MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_MAIN));

				CPanelBluetoothTalking *pTalking=new CPanelBluetoothTalking;
				pTalking->Initialize(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pTalking->LinkStatusBar(m_pStatusBar);
				pTalking->LinkCommander(m_pCommander);
				pTalking->LinkSmartBarPhone(m_pSmartBarPhone);
				m_listPanels.Add(pTalking,MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING));

				CPanelBluetoothA2dp *pA2dp=new CPanelBluetoothA2dp;
				pA2dp->Initialize(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pA2dp->LinkStatusBar(m_pStatusBar);
				pA2dp->LinkCommander(m_pCommander);
				m_listPanels.Add(pA2dp,MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP));

				//
				pWrapper->LinkPages(pMain,pTalking,pA2dp);
				pWrapper->LinkSmartBarMedia(m_pSmartBarMedia);
				m_listPanels.Add(pWrapper,MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
			}
			break;
			/*
		case UI_IPODCORE:
			{
				pPanel=new CPanelIPod;
				pPanel->Initialize(UI_IPODCORE,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				((CPanelIPod*)pPanel)->LinkVideoWindow(m_pVideoWindow);
				m_listPanels.Add(pPanel,UI_IPODCORE);
			}
			break;
			*/
		case UI_CLASS_MHL:
			{
				CPanelMHL *pPanel=new CPanelMHL;
				pPanel->Initialize(idPanel,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				pPanel->LinkVideoWindow(m_pVideoWindow);
				if(m_bSpeechOn)
				{
					pPanel->OnSpeechStart();
				}
				m_listPanels.Add(pPanel,idPanel);
			}
			break;
		case UI_CLASS_PHONELINK:
			{
				CPanelPhoneLink *pPanel=new CPanelPhoneLink;
				pPanel->Initialize(idPanel,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				if(m_bSpeechOn)
				{
					pPanel->OnSpeechStart();
				}
				m_listPanels.Add(pPanel,idPanel);
			}
			break;
		case UI_CLASS_EASYCONNECTED:
			{
				CPanelEasyConnected *pPanel=new CPanelEasyConnected;
				pPanel->Initialize(idPanel,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				if(m_bSpeechOn)
				{
					pPanel->OnSpeechStart();
				}
				m_listPanels.Add(pPanel,idPanel);
			}
			break;
		case UI_CLASS_WEBLINK:
			{
				CPanelWebLink *pPanel=new CPanelWebLink;
				pPanel->Initialize(idPanel,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				if(m_bSpeechOn)
				{
					pPanel->OnSpeechStart();
				}
				m_listPanels.Add(pPanel,idPanel);
			}
			break;
		case UI_CLASS_CARPLAY:
			{
				#if CVTE_EN_CARPLAY
				CPanelCarPlay *pPanel=new CPanelCarPlay;
				pPanel->Initialize(idPanel,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				pPanel->LinkSmartBarVol(m_pSmartBarVol);
				pPanel->LinkSmartBarMedia(m_pSmartBarMedia);

				if(m_bSpeechOn)
				{
					pPanel->OnSpeechStart();
				}
				m_listPanels.Add(pPanel,idPanel);

				/*
				CPanelApp *pPanel;
				pPanel=new CPanelApp(TEXID_152,TEXID_USER3);
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->SetAppPath(_T("\\Media Card\\CarPlayClient.exe"));
				pPanel->SetAudioSrcId(SRC_AUDIO_ARM);

				if(m_bSpeechOn)
				{
					pPanel->OnSpeechStart();
				}
				if(m_bPhoneCallActive)
				{
					pPanel->OnPhoneCallState(m_bPhoneCallActive,NULL);
				}

				pPanel->SetAppName(_T("CarPlay"));
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
				*/
				#endif
			}
			break;
		case UI_CLASS_ANDROID_AUTO:
			{
#if CVTE_EN_ANDROID_AUTO
				CPanelAndroidAuto *pPanel=new CPanelAndroidAuto;
				pPanel->Initialize(idPanel,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry, DeviceActivateProc, this);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				if(m_bSpeechOn)
				{
					pPanel->OnSpeechStart();
				}
				pPanel->LinkBluetoothPanel((CPanelBluetoothWrapper*)m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER)));
				m_listPanels.Add(pPanel,idPanel);
#endif
			}
			break;
		case UI_CLASS_DEVMGR:
			{
				CPanelDevConnectMgr *pPanel=new CPanelDevConnectMgr;
				pPanel->Initialize(idPanel,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				m_listPanels.Add(pPanel,idPanel);
			}
			break;
		case UI_CLASS_MEDIA:
			{
				CPanelMediaMain *pMain=new CPanelMediaMain;
				pMain->Initialize(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pMain->LinkStatusBar(m_pStatusBar);
				pMain->LinkCommander(m_pCommander);
				m_listPanels.Add(pMain,MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN));

				CPanelMediaLibrary *pLibrary=new CPanelMediaLibrary;
				pLibrary->Initialize(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pLibrary,MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY));

				CPanelMediaDevice *pDevice=new CPanelMediaDevice;
				pDevice->Initialize(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_DEVICE),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pDevice,MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_DEVICE));

				CPanelMediaWrapper *pWrapper=new CPanelMediaWrapper;
				m_pMediaWrapper = (pWrapper)?pWrapper:NULL;
				pWrapper->Initialize(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pWrapper->LinkPages(pMain,pLibrary,pDevice);
				pWrapper->LinkStatusBar(m_pStatusBar);
				pWrapper->LinkCommander(m_pCommander);
				pWrapper->OnBrakeStatusChanged(m_bOnBrakeMode);
				pWrapper->LinkSmartBarVol(m_pSmartBarVol);
				pWrapper->LinkSmartBarMedia(m_pSmartBarMedia);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_A2DP,m_idValidSrcList[MEDIA_TYPE_A2DP]);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_IPOD,m_idValidSrcList[MEDIA_TYPE_IPOD]);
				#if CVTE_EN_DVD
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_DISK,m_idValidSrcList[MEDIA_TYPE_DISK]);
				#endif
				m_listPanels.Add(pWrapper,MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));

				if(m_bCarPlayConnected)
				{
					pWrapper->OnCarPlayMsg(CARPLAY_CONNECT_STATUS,TRUE);
				}

			}
			break;

		case UI_CLASS_IMAGE:
			{
				CPanelImageLibrary *pLibrary=new CPanelImageLibrary;
				pLibrary->Initialize(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_LIBRARY),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pLibrary,MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_LIBRARY));

				CPanelImageMain *pMain=new CPanelImageMain;
				pMain->Initialize(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pMain->LinkStatusBar(m_pStatusBar);
				pMain->OnBrakeStatusChanged(m_bOnBrakeMode);
				m_listPanels.Add(pMain,MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN));

				CPanelImageWrapper *pWrapper=new CPanelImageWrapper;
				pWrapper->Initialize(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pWrapper->LinkPages(pMain,pLibrary);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_SD1,m_idValidSrcList[MEDIA_TYPE_SD1]);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_USB1,m_idValidSrcList[MEDIA_TYPE_USB1]);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_USB2,m_idValidSrcList[MEDIA_TYPE_USB2]);
				m_listPanels.Add(pWrapper,MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_WRAPPER));

			}
			break;
		case UI_CLASS_IPOD:
			{
				CPaneliPodLibrary *pLibrary=new CPaneliPodLibrary;
				pLibrary->Initialize(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_LIBRARY),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pLibrary,MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_LIBRARY));

				CPaneliPodMain *pMain=new CPaneliPodMain;
				pMain->Initialize(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pMain->LinkStatusBar(m_pStatusBar);
				pMain->LinkSmartBarMedia(m_pSmartBarMedia);
				pMain->OnBrakeStatusChanged(m_bOnBrakeMode);
				m_listPanels.Add(pMain,MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN));

				CPaneliPodWrapper *pWrapper=new CPaneliPodWrapper;
				m_piPodWrapper = (pWrapper)?pWrapper:NULL;
				pWrapper->Initialize(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pWrapper->LinkPages(pMain,pLibrary);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_A2DP,m_idValidSrcList[MEDIA_TYPE_A2DP]);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_IPOD,m_idValidSrcList[MEDIA_TYPE_IPOD]);
				#if CVTE_EN_DVD
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_DISK,m_idValidSrcList[MEDIA_TYPE_DISK]);
				#endif
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_SD1,m_idValidSrcList[MEDIA_TYPE_SD1]);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_USB1,m_idValidSrcList[MEDIA_TYPE_USB1]);
				pWrapper->OnAvailableSourceChanged(MEDIA_TYPE_USB2,m_idValidSrcList[MEDIA_TYPE_USB2]);
				m_listPanels.Add(pWrapper,MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_WRAPPER));
			}
			break;
			/*
		case UI_AUXCORE:
			{
				pPanel=new CPanelAUX;
				pPanel->Initialize(UI_AUXCORE,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				((CPanelAUX*)pPanel)->LinkVideoWindow(m_pVideoWindow);
				m_listPanels.Add(pPanel,UI_AUXCORE);
			}
			break;
		*/
		case UI_CLASS_IDLE:
			{
				CPanelIdle *pPanel=new CPanelIdle;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_IDLE,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_IDLE,0));
			}
			break;
		case UI_CLASS_NOTIFICATION:
			{
				CPanelNotification *pPanel=new CPanelNotification;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_NOTIFICATION,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_NOTIFICATION,0));
			}
			break;
			/*
		case UI_ALLAPP:
			{
				pPanel=new CPanelAllAPP;
				pPanel->Initialize(UI_ALLAPP,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				m_listPanels.Add(pPanel,UI_ALLAPP);
			}
			break;
			*/
		case UI_CLASS_EXPLORER:
			{
				CPanelExplorer *pPanel=new CPanelExplorer;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_EXPLORER,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_EXPLORER,0));
			}
			break;
		case UI_CLASS_KEYBOARD:
			{
				CPanelKeyboard *pPanel=new CPanelKeyboard;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_KEYBOARD,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_KEYBOARD,0));
			}
			break;
#if CVTE_EN_CAMERA
		case UI_CLASS_REARVIEW:
			{
				CPanelRearView *pRearView=new CPanelRearView;
				pRearView->Initialize(MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pRearView->LinkVideoWindow(m_pVideoWindow);
				pRearView->LinkCommander(m_pCommander);
				pRearView->OnReverseModeChange(m_bOnReverseMode);
				pRearView->LinkSmartBarTopBanner(m_pSmartBarTopBanner);
				pRearView->LinkSmartBarVol(m_pSmartBarVol);
				pRearView->LinkRearOverlay(m_pRearOverlay);
				m_listPanels.Add(pRearView,MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN));
			}
			break;
#endif
		#if CVTE_EN_DTV
		case UI_CLASS_TV:
			{
				CPanelTV *pPanel=new CPanelTV;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkVideoWindow(m_pVideoWindow);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnReverseModeChange(m_bOnReverseMode);
                pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER));
			}
			break;
		#endif

		#if CVTE_EN_DVD
		case UI_CLASS_DISK:
			{
				CPanelDVD *pPanel=new CPanelDVD;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_DISK,PAGE_DISK_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkVideoWindow(m_pVideoWindow);
				pPanel->LinkCommander(m_pCommander);
				pPanel->OnReverseModeChange(m_bOnReverseMode);
                pPanel->OnBrakeStatusChanged(m_bOnBrakeMode);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_DISK,PAGE_DISK_WRAPPER));
            }
			break;
		#endif

		#if CVTE_EN_AUX
		case UI_CLASS_AUX:
			{
				CPanelAUX *pPanel=new CPanelAUX;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_AUX,PAGE_AUX_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkVideoWindow(m_pVideoWindow);
				pPanel->LinkCommander(m_pCommander);
               			pPanel->LinkStatusBar(m_pStatusBar);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_AUX,PAGE_AUX_WRAPPER));
			}
			break;
		#endif

		#if CVTE_EN_ISDB
		case UI_CLASS_ISDB:
			{
				CPanelISDB *pPanel=new CPanelISDB;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_ISDB,PAGE_ISDB_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkCommander(m_pCommander);
               		 	pPanel->LinkStatusBar(m_pStatusBar);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_ISDB,PAGE_ISDB_WRAPPER));
			}
			break;
		#endif

		case UI_CLASS_SETTING:
			{
				CPanelSettingWrapper *pSetting=new CPanelSettingWrapper;
				#if CVTE_EN_LED
                pSetting->LinkLED(m_pLED);
				#endif
                pSetting->LinkAppConfig(m_pConfig);
                pSetting->Initialize(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pSetting->LinkStatusBar(m_pStatusBar);
				pSetting->LinkCommander(m_pCommander);
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
				pSetting->LinkAudioCodec(m_pAudioCodec);
#endif
				m_listPanels.Add(pSetting,MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
			}
			break;
		case UI_CLASS_SR:
			{
				/*
				CPanelSRMain *pSR=new CPanelSRMain;
				pSR->Initialize(MAKE_PANEL_ID(UI_CLASS_SR,PAGE_SR_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pSR->LinkStatusBar(m_pStatusBar);
				pSR->LinkCommander(m_pCommander);
				m_listPanels.Add(pSR,MAKE_PANEL_ID(UI_CLASS_SR,PAGE_SR_MAIN));
				*/
			}
			break;
#if CVTE_EN_NAVI
		case UI_CLASS_NAVI:
			{
				CPanelApp *pPanel;
				pPanel=new CPanelApp(TEXID_151,TEXID_USER2);
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_NAVI,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkSmartBarDrop(m_pSmartBarDrop);
				pPanel->LinkSmartBarMedia(m_pSmartBarMedia);
				pPanel->LinkCommander(m_pCommander);
				pPanel->SetAppPath(m_pSkinManager->GetConfig()->GetCurParam()->strNaviPath.String());
// 				((CPanelApp*)pPanel)->SetAppPath(_T("\\StaticStore\\1.1.13-R14\\CSRPlayer.exe"));
// 				((CPanelApp*)pPanel)->SetAppPath(_T("\\StaticStore\\BT\\BT App.exe"));
// 				((CPanelApp*)pPanel)->SetAppPath(_T("\\Windows\\ceplayer.exe"));
// 				((CPanelApp*)pPanel)->SetAppPath(_T("\\Program Files\\CSRPlayer\\CSRPlayer.exe"));
// 				pPanel->SetAppPath(_T("\\ResidentFlash\\Regedit.exe"));

//  				pPanel->SetAppPath(_T("\\GPS Card\\Navione\\Navione.exe"));
// 				pPanel->SetAppPath(_T("\\GPS Card\\MobileNavigator\\MobileNavigator.exe"));

				if(m_bPhoneCallActive)
				{
					pPanel->OnPhoneCallState(m_bPhoneCallActive,NULL);
				}

				pPanel->SetAppName(_T("Navigation"));
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_NAVI,0));
#if CVTE_EN_CARPLAY
				if(m_bCarPlayConnected)
				{
					pPanel->OnCarPlayMsg(CARPLAY_CONNECT_STATUS,TRUE);
				}
#endif
			}
			break;
#endif
			/*
		case UI_DEBUG:
			{
				pPanel=new CPanelApp;
				pPanel->Initialize(UI_DEBUG,NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				((CPanelApp*)pPanel)->SetAppPath(_T("\\Windows\\explorer.exe"));
				((CPanelApp*)pPanel)->SetAppName(_T("DEBUG"));
				m_listPanels.Add(pPanel,UI_DEBUG);
			}
			break;
			*/
		case UI_CLASS_VEHICLEINFO:
			{
				//if(m_pConfig && m_pConfig->GetCurParam()->bHasCanBus)
				{
					CPanelVehicleInfoWrapper *pWrapper=new CPanelVehicleInfoWrapper;
					pWrapper->Initialize(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
					pWrapper->LinkStatusBar(m_pStatusBar);
					pWrapper->LinkCommander(m_pCommander);
					pWrapper->Connect(TRUE);
					m_listPanels.Add(pWrapper,MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
				}
			}
			break;

		case UI_CLASS_POWERSTATE:
			{
				CPanelPowerState *pPanel=new CPanelPowerState;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_POWERSTATE,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkStatusBar(m_pStatusBar);
				pPanel->LinkCommander(m_pCommander);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_POWERSTATE,0));
			}
			break;
		case UI_CLASS_UPDATE:
			{
				CPanelUpdate *pPanel=new CPanelUpdate;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_UPDATE,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkCommander(m_pCommander);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_UPDATE,0));
			}
			break;
        case UI_CLASS_UPDATE_ARM:
            {
				CPanelUpdateARM *pPanel=new CPanelUpdateARM;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_UPDATE_ARM,PAGE_UPDATE_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkCommander(m_pCommander);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_UPDATE_ARM,PAGE_UPDATE_MAIN));
            }
            break;
        case UI_CLASS_TOUCH:
            {
				CPanelTouch *pPanel=new CPanelTouch;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_TOUCH,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkCommander(m_pCommander);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_TOUCH,PAGE_TOUCH_MAIN));
            }
            break;
		case UI_CLASS_NAVI_SIMULATE:
			{
				CPanelNaviSimulate *pPanel=new CPanelNaviSimulate;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_NAVI_SIMULATE,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_NAVI_SIMULATE,0));
			}
			break;
		#if CVTE_EN_DAB
		case UI_CLASS_DAB:
			{
				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OpenPanel]: UI_CLASS_DAB  CPanelDABMain\r\n")));
				CPanelDABMain *pDABMainPanel=new CPanelDABMain;
				pDABMainPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pDABMainPanel->LinkCommander(m_pCommander);
				pDABMainPanel->LinkStatusBar(m_pStatusBar);
				m_listPanels.Add(pDABMainPanel,MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_MAIN));

				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OpenPanel]: UI_CLASS_DAB  CPanelDABLibrary\r\n")));
				CPanelDABLibrary *pDABLibPanel=new CPanelDABLibrary;
				pDABLibPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_LIBRARY),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pDABLibPanel->LinkCommander(m_pCommander);
				m_listPanels.Add(pDABLibPanel,MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_LIBRARY));

				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OpenPanel]: UI_CLASS_DAB  CPanelDABWrapper\r\n")));
				CPanelDABWrapper *pWrapper=new CPanelDABWrapper;
				pWrapper->Initialize(MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_WRAPPER),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pWrapper->LinkPages(pDABMainPanel, pDABLibPanel);
				pWrapper->LinkCommander(m_pCommander);
				if(m_pCommander)
				{
					RETAILMSG(1,(_T("===set ini  DAB_ANTENNA_POWER=%d\r\n"),m_pConfig->GetCurParam()->bDABAntPowerOn));
					m_pCommander->SendCmd(_ARM_DAB_ANT_POWER_KEY,m_pConfig->GetCurParam()->bDABAntPowerOn?1:0);
				}
				m_listPanels.Add(pWrapper,MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_WRAPPER));
			}
			break;
		#endif

		#if CVTE_EN_ACCON_WARNING
		case UI_CLASS_WARNING:
			{
				CPanelWarning *pPanel=new CPanelWarning;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_WARNING,PAGE_ACCON_WARNING_MAIN),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				pPanel->LinkCommander(m_pCommander);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_WARNING,PAGE_ACCON_WARNING_MAIN));
			}
			break;
		#endif
		#if CVTE_EN_CPMAC
		case UI_CLASS_SETMAC:
			{
				CPanelSetMAC *pPanel=new CPanelSetMAC;
				pPanel->Initialize(MAKE_PANEL_ID(UI_CLASS_SETMAC,0),NULL,m_pSkinManager,m_pTextGL,m_hMainWnd,m_entry);
				m_listPanels.Add(pPanel,MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
			}
			break;
		#endif
		default:
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}
void CGUI_Fantasy::OnTouchDown(POINT *pt)
{
	//RETAILMSG(DEBUG_GUI, ((L"CGUI_Fantasy::OnTouchDown X:%d,Y:%d\r\n"),pt->x,pt->y));

// 	if(m_bOnReverseMode)
// 	{
// 		return ;
// 	}

	if(!m_bSystemReady /*|| !m_bOpenSRC*/ || !m_bDeviceActivated)
		return;

	if(!m_bIsSetMAC)
	{
		if(!m_pSetMACPanel)
		{
			OpenPanels(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
			m_pSetMACPanel = m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
		}

		if(m_pSetMACPanel)
		{
			m_pSetMACPanel->OnTouchDown(pt);
		}
		return;
	}

	if(!IsPowerOn() || m_bOnHWMute)
		return;

// 	m_dwLastClickTick = GetTickCount();

	m_bClickValid = TRUE;

	if(m_bBacklightOFF)
	{
		m_bClickValid = FALSE;
		return;
	}

	if(m_pSmartBarVol)
	{
		if(m_pSmartBarVol->IsVisible())
		{
			m_bClickValid = FALSE;
			return;
		}
	}

	if(m_pSmartBarMedia)
	{
		if(m_pSmartBarMedia->IsVisible())
		{
			m_bClickValid = FALSE;
			return;
		}
	}

	if(m_pStatusBar && m_pCurPanel)
	{
		if(m_pCurPanel->IsStatusBarVisible())
		{
			m_pStatusBar->OnTouchDown(pt);
		}
	}

	//default:
	CGUI::OnTouchDown(pt);

	if(m_pCurPanel && m_pCurPanel->IsAniReady())
		m_pCurPanel->OnTouchDown(pt);

}
void CGUI_Fantasy::OnTouchUp(POINT *pt)
{
 	//RETAILMSG(DEBUG_GUI, ((L"CGUI_Fantasy::OnTouchUp X:%d,Y:%d\r\n"),pt->x,pt->y));

// 	if(m_bOnReverseMode)
// 	{
// 		return ;
// 	}

	if(!m_bSystemReady /*|| !m_bOpenSRC*/ || !m_bDeviceActivated)
		return;

	if(!m_bIsSetMAC)
	{
		if(!m_pSetMACPanel)
		{
			OpenPanels(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
			m_pSetMACPanel = m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
		}

		if(m_pSetMACPanel)
		{
			m_pSetMACPanel->OnTouchUp(pt);
		}
		return;
	}

	if(!IsPowerOn())
		return;

	if(m_bBacklightOFF)
	{
#if (CVTE_DEF_DISPLAY_TYPE == CVTE_DISPLAY_TYPE_U621P)
 		BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);
 		m_bBacklightOFF=FALSE;
#endif
		return;
	}

	if(!m_bClickValid)
		return;

	if(m_bOnHWMute)
		return;

	//exit idle screen
	if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI)==UI_CLASS_IDLE)
	{
		UINT idPreUI=(m_pConfig->GetCurParam()->idPreUI);
		UINT idCurSRC=(m_pConfig->GetCurParam()->idCurSRC);
		if(!IsPanelHasValidAudioSrc(idPreUI) || GET_PANEL_CLASS_ID(idPreUI)==GET_PANEL_CLASS_ID(idCurSRC))
			ReturnToPrevPanel(PAGE_SWITCH_ANI_NONE);
		else
			SwitchPage(GET_PANEL_CLASS_ID(idCurSRC),GET_PANEL_PAGE_ID(idCurSRC),PAGE_SWITCH_ANI_NONE);
		PlaySoundEffect(0);
		return;
	}

	if(m_pStatusBar && m_pCurPanel)
	{
		if(m_pCurPanel->IsStatusBarVisible())
		{
			m_pStatusBar->OnTouchUp(pt);
		}
	}

	//default:
	CGUI::OnTouchUp(pt);

	if(m_pCurPanel && m_pCurPanel->IsAniReady())
		m_pCurPanel->OnTouchUp(pt);

}
void CGUI_Fantasy::OnTouchMove(POINT *pt)
{
 	//RETAILMSG(DEBUG_GUI, ((L"CGUI_Fantasy::OnTouchMove X:%d,Y:%d\r\n"),pt->x,pt->y));

// 	if(m_bOnReverseMode)
// 	{
// 		return ;
// 	}

	if(!m_bSystemReady /*|| !m_bOpenSRC*/ || !m_bDeviceActivated)
		return;

	if(!m_bIsSetMAC)
	{
		if(!m_pSetMACPanel)
		{
			OpenPanels(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
			m_pSetMACPanel = m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
		}

		if(m_pSetMACPanel)
		{
			m_pSetMACPanel->OnTouchMove(pt);
		}
		return;
	}

	if(!IsPowerOn())
		return;

	if(m_bBacklightOFF)
	{
		return;
	}

	if(!m_bClickValid)
		return;

	if(m_bOnHWMute)
		return;

// 	m_dwLastClickTick = GetTickCount();

	//default:
	CGUI::OnTouchMove(pt);

// 	if(m_pStatusBar)
// 	{
// 		m_pStatusBar->OnTouchMove(pt);
// 	}

	if(m_pCurPanel && m_pCurPanel->IsAniReady())
		m_pCurPanel->OnTouchMove(pt);

}
void CGUI_Fantasy::OnGesture(WPARAM wParam, LPARAM lParam)
{
	GESTUREINFO gi;
	memset(&gi, 0, sizeof(gi));
	gi.cbSize = sizeof(GESTUREINFO);
	if (GetGestureInfo((HGESTUREINFO)lParam, &gi))
	{
		//default:
		CGUI::OnGesture(wParam,lParam);

		if(m_pCurPanel)
		{
			m_pCurPanel->OnGesture(&gi);
		}
	}
	else
	{
		// Error handling
	}
}
void CGUI_Fantasy::OnLampStatusChanged(BOOL bOn)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnLampStatusChanged] %d\r\n"),bOn));


}

void CGUI_Fantasy::OnDeviceChanged()
{
	//RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnDeviceChanged] cnt:%d\r\n"),m_iDeviceDetectCnt));

	//
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		m_iDeviceDetectCnt --;
		((CPanelMediaWrapper*)panel)->OnDeviceChanged();
	}

	//
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnDeviceChanged();
	}

	if(m_iDeviceDetectCnt<=0)
	{
		KillTimer(g_hUIWnd,TIMER_DEVICE_CHANGED);
	}
}

void CGUI_Fantasy::OnDeviceArrived(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnDeviceArrived] [%s]\r\n"),lpDeviceName));

	if(!lpDeviceName)
		return;

	if(wcsstr(lpDeviceName,_T("DSK")))
	{
		m_iDeviceDetectCnt = 5;
		SetTimer(g_hUIWnd,TIMER_DEVICE_CHANGED,3000,NULL);
	}

	if (m_bPhoneCallActive)
	{//[bug] 2730 play Carplay music,make a call,insert USB,hangup call,USB sound mix Carplay sound
		return;
	}
	//pre-process
	if(wcsstr(lpDeviceName,_T("LNG")))//IAP
	{
		OnAvailableSourceChanged(MEDIA_TYPE_IPOD,1);
		BOOL bShow = TRUE;
		m_biPodConnected = TRUE;
		OperateWidget(IDC_WIDGET_IPOD,0,UIOPRT_SHOWHIDE,&bShow);
	}

#if CVTE_EN_ACCON_WARNING
	if (CWarningProxy::GetInstance()->IsShowWarningSmartBar())
	{
		return;
	}
#endif

  //  else if(wcsstr(lpDeviceName,_T("ECD1")))
  //  {
		//OnAvailableSourceChanged(MEDIA_TYPE_WEBLINK,1);
  //  }

	if(!IsPowerOn() || !m_bOpenSRC)
		return;

	if(GetTickCount()-m_dwSystemReadyTick<10000)
		return;
// 	if(GetTickCount()-m_dwOpenSRCTick<5000)
// 		return;

	if(GetTickCount()-m_dwPowerStateTick<5000)
		return;

	if(wcsstr(lpDeviceName,_T("DSK8")))//GPS card
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_SDGPSCARD;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(1<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("ECD1")))//Easyconnected Link
	{
		RETAILMSG(1,(_T("########### SwitchPage DEV_EASYCONNECTED ################\r\n")));

		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_EASYCONNECTED;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(1<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("DSK9")))//Media card
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_SDMEDIACARD;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(1<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("DSK2")))//USB disk
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_USBDISK;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(1<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("DSK3")))//USB disk2
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_USBDISK2;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(1<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("LNG")))//IAP
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_IAP;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(1<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("PLA"))/* || wcsstr(lpDeviceName,_T("LIN"))*/)//Phone Link
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_PHONELINK;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(1<<16)+m_idDevTypeInserted);
	}
	else
	{
		m_strDeviceInserted = _T("");
	}

}
void CGUI_Fantasy::OnDeviceRemoved(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnDeviceRemoved] [%s]\r\n"),lpDeviceName));

	if(!lpDeviceName)
		return;

	if(wcsstr(lpDeviceName,_T("DSK")))
	{
		m_iDeviceDetectCnt = 1;
		SetTimer(g_hUIWnd,TIMER_DEVICE_CHANGED,3000,NULL);
	}

	//pre-process
	if(wcsstr(lpDeviceName,_T("LNG")))//IAP
	{
		OnAvailableSourceChanged(MEDIA_TYPE_IPOD,0);
		BOOL bShow = FALSE;
		m_biPodConnected = FALSE;
		OperateWidget(IDC_WIDGET_IPOD,0,UIOPRT_SHOWHIDE,&bShow);
	}

  //  else if(wcsstr(lpDeviceName,_T("ECD1")))
  //  {
		//OnAvailableSourceChanged(MEDIA_TYPE_WEBLINK,0);
  //  }

	//process panels operation first:
	m_listPanels.Head();
  	//下发OnDeviceRemovedReady给有效的panel
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnDeviceRemovedReady(lpDeviceName);
		m_listPanels.Next();
	}

#if CVTE_EN_ACCON_WARNING
	if (CWarningProxy::GetInstance()->IsShowWarningSmartBar())
	{
		return;
	}
#endif

	if(!IsPowerOn() || !m_bOpenSRC)
		return;
	if(GetTickCount()-m_dwSystemReadyTick<15000)
		return;
	if(GetTickCount()-m_dwPowerStateTick<5000)
		return;

	if(wcsstr(lpDeviceName,_T("DSK8")))//GPS card
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_SDGPSCARD;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(0<<16)+m_idDevTypeInserted);
	}

	else if(wcsstr(lpDeviceName,_T("ECD1")))//Easyconnected
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_EASYCONNECTED;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(0<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("DSK9")))//Media card
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_SDMEDIACARD;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(0<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("DSK2")))//USB disk
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_USBDISK;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(0<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("DSK3")))//USB disk2
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_USBDISK2;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(0<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("LNG")))//IAP
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_IAP;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(0<<16)+m_idDevTypeInserted);
	}
	else if(wcsstr(lpDeviceName,_T("PLA"))/* || wcsstr(lpDeviceName,_T("LIN"))*/)//Phone Link
	{
		m_strDeviceInserted = lpDeviceName;
		m_idDevTypeInserted = DEV_PHONELINK;
		SwitchPage(UI_CLASS_DEVMGR,0,PAGE_SWITCH_ANI_NONE,(0<<16)+m_idDevTypeInserted);
	}
	else
	{
		m_strDeviceInserted = _T("");
	}

}
void CGUI_Fantasy::OnDeviceArrivedReady(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnDeviceArrivedReady] \r\n")));

	//切换页面
	OnPanelChange(wParam,lParam,PAGE_SWITCH_ANI_NONE);

	//查询切换的页面，并调用对应页面的OnDeviceArrivedReady函数
	//对所有有效的panel下发OnDeviceArrivedReady命令
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnDeviceArrivedReady(m_strDeviceInserted.String());
		m_listPanels.Next();
	}

}
void CGUI_Fantasy::OnDeviceRemovedReady(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnDeviceRemovedReady] \r\n")));

	OnPanelChange(wParam,lParam,PAGE_SWITCH_ANI_NONE);

// 	m_listPanels.Head();
// 	while (!m_listPanels.End())
// 	{
// 		m_listPanels.Get()->OnDeviceRemovedReady(m_strDeviceInserted.String());
// 		m_listPanels.Next();
// 	}
}

void CGUI_Fantasy::OperateWidget(UINT idUI,UINT idControl,UINT idOperation,LPVOID lpara)
{
	//RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OperateWidget] idUI[%d] idControl[%d] idOperation[%d]\r\n"),idUI,idControl,idOperation));
	CPanelGL *p = m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
	if(p)
	{
		((CPanelHomePage*)p)->OperateWidget(idUI,idControl,idOperation,lpara);
	}
}
void CGUI_Fantasy::OnBrakeStatusChanged(BOOL bOn)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnBrakeStatusChanged] %d\r\n"),bOn));

	m_bOnBrakeMode = bOn;

	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnBrakeStatusChanged(bOn);
		m_listPanels.Next();
	}
}

void CGUI_Fantasy::OnWiFiMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnWiFiMsg] %d\r\n"),wParam));

	if(m_pStatusBar)
	{
		m_pStatusBar->SetWiFiStatus(WIFI_STATUS(wParam));
	}
}
void CGUI_Fantasy::OnReverseTimerProc()
{
	RETAILMSG(DEBUG_GUI,(_T("######OnReverseTimerProc \r\n")));

	if(m_bPowerOperating)
	{
		RETAILMSG(DEBUG_GUI,(_T("######OnReverseTimerProc bPowerOperating\r\n")));
	}
	else
	{
		OnReverseModeChange(m_bReverseSignaled);
		KillTimer(g_hUIWnd,TIMER_REVERSE_SIGNAL);
	}
}

void CGUI_Fantasy::OnRearViewPreDoneTimerProc()
{
	RETAILMSG(1,(_T("######OnRearViewPreDoneTimerProc######\r\n")));

	if(m_bPowerOperating)
	{
		RETAILMSG(DEBUG_GUI,(_T("######OnRearViewPreDoneTimerProc bPowerOperating, do nothing\r\n")));
	}
	else
	{
		OnRearViewPrepare(m_bReverseSignaled);
		KillTimer(g_hUIWnd,TIMER_REARVIEW_PRE_DONE);
		if(m_bReverseMCUCmd)
		{
			SetTimer(g_hUIWnd, TIMER_REARVIEW_DONE,500,NULL);
		}
		else
		{
			RETAILMSG(1,(_T("######OnRearViewPreDoneTimerProc ### m_bReverseMCUCmd=FALSE ### do nothing\r\n")));
		}
	}
}

void CGUI_Fantasy::OnRearViewDoneTimerProc()
{
	RETAILMSG(1,(_T("######OnRearViewDoneTimerProc######\r\n")));

	if(m_bPowerOperating)
	{
		RETAILMSG(DEBUG_GUI,(_T("######OnRearViewDoneTimerProc bPowerOperating, do nothing\r\n")));
	}
	else
	{
		if(m_bReverseMCUCmd)
		{
			OnRearViewDone(m_bReverseSignaled);
		}
		else
		{
			RETAILMSG(1,(_T("######OnRearViewDoneTimerProc ### m_bReverseMCUCmd=FALSE ### do nothing\r\n")));
		}
		KillTimer(g_hUIWnd,TIMER_REARVIEW_DONE);
	}
}
void CGUI_Fantasy::OnAutoStartNaviProc()
{
	//RETAILMSG(1,(_T("######OnAutoStartNaviProc ### m_bCarPlayConnected:%d \r\n"),m_bCarPlayConnected));
	KillTimer(g_hUIWnd,TIMER_AUTO_START_NAVI);

	if (!m_bCarPlayConnected)
	{
		SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN,PAGE_SWITCH_ANI_NONE);
		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC)!=UI_CLASS_CARPLAY)
		{
			#if CVTE_EN_KEY_PROXY
			CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
			CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
			#else
			OnMediaKey(KEY_MEDIA_PLAY);
			#endif
		}
		else
		{
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
		}
	}

}
void CGUI_Fantasy::OnUpdateRadioPlayInfo()
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_WRAPPER));
	if(panel)
	{
		((CPanelRadioWrapper*)panel)->OnUpdateRadioPlayInfo();
	}
}
void CGUI_Fantasy::OnRadioMonitorTA()
{
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_WRAPPER));
	if(panel)
	{
		((CPanelRadioWrapper*)panel)->OnRadioMonitorTA();
	}
#endif
}
void CGUI_Fantasy::AutoSwitchNativeUI()
{
#if CVTE_EN_CARPLAY
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
	if(panel)
	{
		((CPanelCarPlay*)panel)->AutoSwitchNativeUI();
	}
	KillTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_NATIVEUI);
#endif
}
void CGUI_Fantasy::AutoSwitchCarPlayScreen()
{
	#if CVTE_EN_CARPLAY
	const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();

	RETAILMSG(1/*DEBUG_GUI*/, (TEXT("MSG:[CGUI_Fantasy::AutoSwitchCarPlayScreen]: mode->screen=%d\r\n"),mode->screen));

	if(mode->screen == kAirPlayEntity_Controller)
	{
		SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,(1<<16)+0);
	}
	KillTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_SWITCH_SCREEN);
	#endif
}
void CGUI_Fantasy::AutoRequestCarPlayScreen()
{
	#if CVTE_EN_CARPLAY
	const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();

	RETAILMSG(1/*DEBUG_GUI*/, (TEXT("MSG:[CGUI_Fantasy::AutoRequestCarPlayScreen]: mode->screen=%d\r\n"),mode->screen));

	if(mode->screen != kAirPlayEntity_Controller)
	{
		SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,(0<<16)+0);
	}
	KillTimer(m_hMainWnd,TIMER_CARPLAY_AUTO_REQUEST_SCREEN);
	#endif
}
void CGUI_Fantasy::AutoSwitchCarPlayMedia()
{
	#if CVTE_EN_CARPLAY
    CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
	if(panel)
	{
		((CPanelCarPlay*)panel)->AutoSwitchCarPlayMedia();
	}
	#endif
}
#if CVTE_EN_CARPLAY
void CGUI_Fantasy::OnCarPlayArrived()
{
	RETAILMSG(1/*DEBUG_GUI*/, (TEXT("MSG:[CGUI_Fantasy::OnCarPlayArrived]: ....\r\n")));

	m_dwCarPlayArrivedTick = GetTickCount();
	if(!m_bPowerOperating)
	{
		//connecting:
		OnCarPlayMsg(CARPLAY_CONNECT_STATUS,TRUE);
		OnAvailableSourceChanged(MEDIA_TYPE_CARPLAY,TRUE);
		if(!m_bPowerOperating)
		{
			//auto power on:
			if(!IsPowerOn())
			{
				if(m_bMCUPackageUpdated)
				{
					//do not power on while carplay arrived at power idle status.
					//DoPowerOn();
					//PreparePowerOn();

	// 				OnSystemPrePowerState(SYS_POWER_ON);
	// 				OnSystemPowerState(SYS_POWER_ON);
	// 				SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
				}
			}
			else
			{
				if( (m_pCurPanel?GET_PANEL_CLASS_ID(m_pCurPanel->GetID())!=UI_CLASS_CARPLAY:TRUE) && m_bOpenSRC)
				{
					//SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
				}
			}
		}
	}
}
void CGUI_Fantasy::OnCarPlayRemove()
{
	RETAILMSG(1/*DEBUG_GUI*/, (TEXT("MSG:[CGUI_Fantasy::OnCarPlayRemove]: ....\r\n")));

	//disconnected:
	OnCarPlayMsg(CARPLAY_CONNECT_STATUS,FALSE);
	OnAvailableSourceChanged(MEDIA_TYPE_CARPLAY,FALSE);
	/*
	if((m_pCurPanel?GET_PANEL_CLASS_ID(m_pCurPanel->GetID())==UI_CLASS_CARPLAY:TRUE) && m_bOpenSRC)
	{
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		//OnSwitchMediaClassSRC(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreSRC),GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idPreSRC));
		//OnMediaKey(KEY_MEDIA_PLAY,NULL);
		//ReturnToPrevPanel(PAGE_SWITCH_ANI_ZOOMIN);
	}
	*/

}
#endif
void CGUI_Fantasy::OnRearViewPrepare(BOOL bOn)
{
	if(m_bReverseSignaled == bOn)
	{
		RETAILMSG(1/*DEBUG_GUI*/,(_T("######OnRearViewPrepare bOn[%d] Skip!!!  Do nothing...\r\n"),bOn));
		return;
	}

	RETAILMSG(1/*DEBUG_GUI*/,(_T("######OnRearViewPrepare %d\r\n"),bOn));

	m_bReverseSignaled = bOn;

	BKL_SetBackLight(0,TRUE);

	if((bOn && m_bPowerOperating) || !bOn)
	{
        RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:OnRearViewPrepare: SetTimer TIMER_REVERSE_SIGNAL\r\n")));
		SetTimer(g_hUIWnd,TIMER_REVERSE_SIGNAL,1,NULL);
	}
	else
	{
		OnReverseModeChange(bOn);
	}
}

void CGUI_Fantasy::OnRearViewDone(BOOL bOn, DWORD dwTimeout)
{
	RETAILMSG(1,(_T("######OnRearViewDone: bOn:%d, timeout:%d m_iScrnBrightness:%d\r\n"),bOn,dwTimeout,m_iScrnBrightness));
#if 0
	if(bOn)
	{
//			if(m_pLED)
//			{
//				m_pLED->SetLedState(LED_NORMAL);
//			}

		//backlight:
		if(m_bBacklightOFF)
		{
			m_dwBklBeforeReverse = 0;
		}
		else
		{
			m_dwBklBeforeReverse = m_pConfig->GetCurParam()->dwBacklight;
		}

        if(dwTimeout)
        {
            Sleep(dwTimeout);
	        RETAILMSG(1,(_T("######OnRearViewDone: Sleep:%d\r\n"),dwTimeout));
        }
		int bkl=max(m_pConfig->GetCurParam()->dwBacklight,30);
		BKL_SetBackLight(bkl,TRUE);
		m_bBacklightOFF=FALSE;
	}
	else
	{
		//backlight:
		if(m_dwBklBeforeReverse == 0 && !m_bPhoneCallActive)
		{
			BKL_SetBackLight(0,TRUE);
			m_bBacklightOFF=TRUE;
		}
		else if(IsPowerOn())
		{
			int bkl=max(m_pConfig->GetCurParam()->dwBacklight,30);
			BKL_SetBackLight(bkl,TRUE);
			m_bBacklightOFF=FALSE;
		}
	}
#else
	if(bOn)
	{
        if(dwTimeout)
        {
            Sleep(dwTimeout);
	        RETAILMSG(1,(_T("######OnRearViewDone: true Sleep:%d\r\n"),dwTimeout));
        }

        if(m_iScrnBrightness == 1)  //Scrn on
        {
    		//backlight:
    		if(m_bBacklightOFF)
    		{
    			m_dwBklBeforeReverse = 0;
    		}
    		else
    		{
    			m_dwBklBeforeReverse = min(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight);
    		}

			//Make sure the reverse backlight is enough
    		BKL_SetBackLight(max(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight),TRUE);

        }
        else
        {
    		//backlight:
    		if(m_bBacklightOFF)
    		{
    			m_dwBklBeforeReverse = 0;
    		}
    		else
    		{
    			m_dwBklBeforeReverse = m_pConfig->GetCurParam()->dwBacklight;
    		}

    		int bkl=max(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight);
    		BKL_SetBackLight(bkl,TRUE);
        }

		m_bBacklightOFF=FALSE;
	}
	else
	{
        if(dwTimeout)
        {
            Sleep(dwTimeout);
	        RETAILMSG(1,(_T("######OnRearViewDone: false Sleep:%d\r\n"),dwTimeout));
        }

        if(m_iScrnBrightness == 1)  //Scrn on
        {
            //backlight:
    		if(m_dwBklBeforeReverse == 0 && !m_bPhoneCallActive)
    		{
    			BKL_SetBackLight(0,TRUE);
    			m_bBacklightOFF=TRUE;
    		}
    		else if(IsPowerOn())
    		{
                BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
    			m_bBacklightOFF=FALSE;
    		}
        }
        else
        {
            //backlight:
    		if(m_dwBklBeforeReverse == 0 && !m_bPhoneCallActive)
    		{
    			BKL_SetBackLight(0,TRUE);
    			m_bBacklightOFF=TRUE;
    		}
    		else if(IsPowerOn())
    		{
                BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
    			m_bBacklightOFF=FALSE;
    		}
        }

    }
#endif
}

void CGUI_Fantasy::OnReverseModeChange(BOOL bReverse)
{
	//static BOOL bflag=FALSE;
	//static int iVolume=0;

	RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::OnReverseModeChange] %d\r\n"),bReverse));

//		if(m_bOnReverseMode == bReverse)
//		{
//			RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnReverseModeChange] m_bOnReverseMode == bReverse  do nothing. skip!!!\r\n")));
//			return;
//		}

	//
	m_bOnReverseMode=bReverse;

// 	if(!m_bUILaunchNotify && bReverse)
// 	{
// 		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::m_bUILaunchNotify] %d \r\n"),m_bUILaunchNotify));
//
// 		return;
// 	}

	//panels:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnReverseModeChange(bReverse);
		m_listPanels.Next();
	}

	//
// 	if(IsPowerOn())
	{
		//smartbar
		if(bReverse)
		{
			if(m_pSmartBarVol)
			{
				if(m_pSmartBarVol->IsVisible())
				{
					m_pSmartBarVol->Show(FALSE);
				}
			}

			if(m_pSmartBarMedia)
			{
				if(m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->Show(FALSE);
				}
			}
#if CVTE_EN_ACCON_WARNING
			CWarningProxy::GetInstance()->PauseShowWarningBar();
#endif
		}
		else
		{
			if (m_pSmartBarVol && m_bManualMute)
			{
				m_pSmartBarVol->Show(TRUE);
			}
#if CVTE_EN_ACCON_WARNING
			CWarningProxy::GetInstance()->ResumeShowWarningBar();
#endif
		}

		//volume:
		if(m_bManualPowerOn && m_bOpenSRC)
		{
			if(!m_bManualMute)
			{
				if(bReverse)
				{
					DWORD dwVol = m_pConfig->GetCurParam()->dwVolMedia;

					#if CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
					if(m_pConfig->GetCurParam()->bRearVolControl)
					{
						dwVol=min(REARVIEW_VOLUME_DEFAULT,dwVol);
					}
					#else
					dwVol=min(REARVIEW_VOLUME_DEFAULT,dwVol);
					#endif

					if(!m_bPhoneCallActive && !m_bSpeechOn)
					{
						//SetMainVolume(m_pConfig->GetCurParam()->dwVolMedia,TRUE,FALSE,FALSE);
                        SetVolume(dwVol,TRUE,FALSE,FALSE);
                    }
				}
				else
				{
					if(!m_bPhoneCallActive && !m_bSpeechOn)
					{
						//SetMainVolume(m_pConfig->GetCurParam()->dwVolMedia,TRUE,FALSE,FALSE);
						SetVolume(m_pConfig->GetCurParam()->dwVolMedia,TRUE,FALSE,FALSE);
					}
				}
			}
			else
			{
				if(bReverse)
				{

				}
				else
				{
					if(!m_bPhoneCallActive && !m_bSpeechOn)
					{
						//SetMainVolume(0,TRUE,TRUE,FALSE);
						SetVolume(0,TRUE,TRUE,FALSE);
					}
				}
			}
		}


		//
		if(bReverse)
		{
			//power:
// 			if(!IsPowerOn())
// 			{
// 				PowerOn();
// 				SetMainVolume(0,0);
// 			}

// 			//enter rear view:
// 			if(m_bUILaunchNotify)
// 			{
// 				if(/*m_bOpenSRC && */m_pConfig->GetCurParam()->idCurUI!=MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN))
// 				{
// 					SwitchPage(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN,PAGE_SWITCH_ANI_NONE,0);
// 				}
// 			}
// 			else
// 			{
// 				BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);
// 				m_bBacklightOFF=FALSE;
// 			}
			//BUG 0002881: "自动调整倒车音量"关闭下->任意源静音->取消静音->进入倒车->音量区的LED灯不亮
            if(!IsPowerOn())
            {
				#if CVTE_EN_LED
                if(m_pLED)
                {
                    m_pLED->SetLedState(LED_POWERON);
                }
				#endif
            }
		}
		else
		{
			//quit rear view:
			if(m_bManualPowerOn)
			{
// 				if(m_bUILaunchNotify)
// 				{
// 					if(m_bPhoneCallActive)
// 					{
// 						SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING,PAGE_SWITCH_ANI_NONE,0);
// 					}
// 					else if(/*m_bOpenSRC && */m_pConfig->GetCurParam()->idCurUI==MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN))
// 					{
// 						ReturnToPrevPanel(PAGE_SWITCH_ANI_NONE);
// 					}
// 				}
			}
			else
			{
				if(m_bPhoneCallActive)
				{
					//SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING,PAGE_SWITCH_ANI_NONE,0);
				}
				else
				{
					RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnReverseModeChange]: =>PowerIdle\r\n")));

					PowerIdle();
				}
			}
		}
	}

	//
	if(!bReverse && m_bResumeSRCAfterRearView)
	{
		m_bResumeSRCAfterRearView = FALSE;
		if(!m_bPhoneCallActive)
		{
			AutoOpenSRC();
		}
	}

	#if CVTE_EN_CARPLAY
	//carplay:
	CCarplayAdapter::GetInstance()->SetReverseMode(m_bOnReverseMode);
	#endif

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	if(m_pAudioCodec)
		m_pAudioCodec->SetLimitVolume(m_bOnReverseMode);
#endif
}

void CGUI_Fantasy::ReturnToPrevPanel(PAGE_ANI_ID idSwitchAniId)
{
    RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::ReturnToPrevPanel]: \r\n")));
	if(m_pConfig)
	{
		UINT idUI=(m_pConfig->GetCurParam()->idPreUI);
		UINT idClass=GET_PANEL_CLASS_ID(idUI);
		UINT idPage=GET_PANEL_PAGE_ID(idUI);

		if( (idUI!=MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING) || m_bPhoneCallActive)
			&& idClass!=UI_CLASS_SR
			&& idClass>UI_CLASS_NULL&&idClass<UI_CLASS_MAX_VALID)//invalid previous panels
		{
			RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::ReturnToPrevPanel]: SwithPage #1# idClass[%X]\r\n"),idClass));
			SwitchPage(idClass,idPage,idSwitchAniId,0);
		}
		else
		{
			RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::ReturnToPrevPanel]: SwithPage #2# idClass[%X]\r\n"),GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC)));
			//Mantis 0003528当前不播放任何源，车机显示idle界面时，蓝牙来电，挂断电话，点击屏幕显示区域无法取消idle界面
			if ((GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC) == UI_CLASS_CARPLAY && !m_bCarPlayConnected) || 
				GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC) == UI_CLASS_NULL )
			{
				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
			}
			else
			{
				SwitchPage(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC),idSwitchAniId,0);
			}
		}
	}
}

void CGUI_Fantasy::OnWallpaperChanged(LPCTSTR pStrFilePath)
{
	if(!pStrFilePath)
		return;
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnWallpaperChanged]:%s.\r\n"),pStrFilePath));


}
void CGUI_Fantasy::OnShowKeyboard(WPARAM wParam, LPARAM lParam)
{
	KEYBOARDMSG *pkbd=(KEYBOARDMSG*)lParam;

	if(!pkbd)
		return;

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnShowKeyboard]:wParam:%d,lParam:%x\r\n"),wParam,lParam));

// 	OnPanelChange(MAKE_PANEL_ID(UI_CLASS_KEYBOARD,0),0,wParam);
	OpenPanels(MAKE_PANEL_ID(UI_CLASS_KEYBOARD,0));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_KEYBOARD,0));
	if(panel)
	{
		((CPanelKeyboard*)panel)->SetNotification(pkbd);
	}

	//show
	SwitchPage(UI_CLASS_KEYBOARD,0,PAGE_SWITCH_ANI_NONE);
}
void CGUI_Fantasy::OnShowExplorer(WPARAM wParam, LPARAM lParam)
{
	EXPLORERMSG *pexp=(EXPLORERMSG*)lParam;

	if(!pexp)
		return;

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnShowExplorer]:wParam:%d,lParam:%x\r\n"),wParam,lParam));

// 	OnPanelChange(MAKE_PANEL_ID(UI_CLASS_EXPLORER,0),0,wParam);

	OpenPanels(MAKE_PANEL_ID(UI_CLASS_EXPLORER,0));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_EXPLORER,0));
	if(panel)
	{
		((CPanelExplorer*)panel)->SetNotification(pexp);
	}

	//show
	SwitchPage(UI_CLASS_EXPLORER,0);
}
void CGUI_Fantasy::OnShowMsgBox(WPARAM wParam, LPARAM lParam)
{
	NBMSG *pnb=(NBMSG*)lParam;

	if(wParam)
	{
		if(!pnb)
			return;

		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnShowMsgBox:wParam]:%d,lParam:%x\r\n"),wParam,lParam));

		OnPanelChange(MAKE_PANEL_ID(UI_CLASS_NOTIFICATION,0),0,PAGE_SWITCH_ANI_NONE);

		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NOTIFICATION,0));
		if(panel)
		{
			((CPanelNotification*)panel)->SetNotification(pnb);
		}
	}
	else
	{
		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NOTIFICATION,0));
		if(panel)
		{
			((CPanelNotification*)panel)->HideNotification();
		}
	}
}

void CGUI_Fantasy::OnShowPasswordBox(WPARAM wParam, LPARAM lParam)
{
	NBMSG *pnb=(NBMSG*)lParam;

	if(wParam)
	{
		if(!pnb)
			return;

		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnShowPasswordBox:wParam]:%d,lParam:%x\r\n"),wParam,lParam));

		//OnPanelChange(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER),0,PAGE_SWITCH_ANI_NONE,PAGE_SETTINGS_PASSWORD);
		SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_PASSWORD);

		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
		if(panel)
		{
			((CPanelSettingWrapper*)panel)->OnSetPasswordBox(pnb);
		}
		else
		{
			RETAILMSG(1,(_T("ERR:[CGUI_Fantasy::OnShowPasswordBox]: Panel is null!!!\r\n")));
		}
	}
}

void CGUI_Fantasy::OnPasswordBoxCmd(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnPasswordBoxCmd(wParam,lParam);
	}
}

void CGUI_Fantasy::OnVehicleSpeed(WPARAM wParam, LPARAM lParam)
{
	static int last=-1;

// 	//test:
// 	static DWORD s_cnt=0;
// 	static DWORD last_time=GetTickCount();
// 	static int step=10;
// 	s_cnt++;
// 	if(s_cnt%20==0)
// 	{
// 		step=-step;
// 	}
// 	m_iVehicleSpeed += step;

	if(wParam == last)
	{
		return;
	}
	last=wParam;
	m_iVehicleSpeed = wParam;

	if(!m_bManualMute && m_bManualPowerOn && m_bOpenSRC)
	{
		if(m_pConfig && m_pCommander/*&&m_iVehicleSpeed>m_iSpeedCurVolome*/ && !m_bOnReverseMode)
		{
			if(m_pConfig->GetCurParam()->dwVolumeAdjustRate>0)
			{
				//need fixed:
				int delt=int(32.0f*(m_pConfig->GetCurParam()->dwVolumeAdjustRate*0.1*(m_iVehicleSpeed-m_iSpeedCurVolome)/120.0));

				int vol_cur=0;
				int vol_max=max(32,m_iSetVolume);

				m_iVolumeDelt = delt;

				if(m_bPhoneCallActive)
					vol_cur=m_iVolOriginBluetooth;
				else if(m_bSpeechOn)
					vol_cur=m_iVolOriginGPS;
				else
					vol_cur=m_iVolOriginMedia;

				//limit:
// 				if( delt<0 && vol_cur+delt<10 )
// 					delt=(vol_cur>10)?(10-vol_cur):0;
				if(delt<0)
					delt=0;

				int vol_set=/*m_iSetVolume*/vol_cur+delt;
				if(vol_set<0)vol_set=0;if(vol_set>vol_max)vol_set=vol_max;

				RETAILMSG(DEBUG_GUI,(_T("++OnVehicleSpeed:%d, vol: %d, delt: %d, set: %d, speed_set:%d\r\n"),m_iVehicleSpeed,vol_set,delt,vol_cur/*m_iSetVolume*/,m_iSpeedCurVolome));

				int vol_t=0;
				vol_t=m_iVolOriginBluetooth+delt;
				if(vol_t<0)vol_t=0;if(vol_t>vol_max)vol_t=vol_max;
				m_pConfig->GetCurParam()->dwVolBluetooth=vol_t;

				vol_t=m_iVolOriginGPS+delt;
				if(vol_t<0)vol_t=0;if(vol_t>vol_max)vol_t=vol_max;
				m_pConfig->GetCurParam()->dwVolGPS=vol_t;

				vol_t=m_iVolOriginMedia+delt;
				if(vol_t<0)vol_t=0;if(vol_t>vol_max)vol_t=vol_max;
				m_pConfig->GetCurParam()->dwVolMedia=vol_t;

				if(m_iVolumeFadeTo!=vol_set)
				{
					//SetMixVolume(BYTE(vol_set),TRUE,/*TRUE*/FALSE,FALSE,TRUE);
					SetVolume(BYTE(vol_set),FALSE,/*TRUE*/FALSE,FALSE,TRUE,FALSE);
				}

// 				RETAILMSG(DEBUG_GUI,(_T("#########Auto Adjust, OriginGPS %d,  dwVolGPS:%d\r\n"),m_iVolOriginGPS,m_pConfig->GetCurParam()->dwVolGPS));
// 				RETAILMSG(DEBUG_GUI,(_T("#########Auto Adjust, OriginBluetooth %d,  dwVolBluetooth:%d\r\n"),m_iVolOriginBluetooth,m_pConfig->GetCurParam()->dwVolBluetooth));
// 				RETAILMSG(DEBUG_GUI,(_T("#########Auto Adjust, OriginMedia %d, dwVolMedia:%d\r\n"),m_iVolOriginMedia,m_pConfig->GetCurParam()->dwVolMedia));
			}
		}
	}

	//
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
	if(panel)
	{
		((CPanelVehicleInfoWrapper*)panel)->OnGPSGroundSpeed(m_iVehicleSpeed);
	}
}

//	void CGUI_Fantasy::OnSteerWheelTimerProc()
//	{
//		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_ADVANCED));
//		if(panel)
//		{
//			((CPanelSettingAdvanced*)panel)->OnSteerWheelTimerProc();
//		}
//	}

void CGUI_Fantasy::OnVehicleInfoTimerProc()
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
	if(panel)
	{
		((CPanelVehicleInfoWrapper*)panel)->OnTimerProc();
	}
}

void CGUI_Fantasy::OnVehicleInfoCmd(WPARAM wParam, LPARAM lParam)
{
// 	RETAILMSG(DEBUG_GUI,(_T("CGUI_Fantasy::OnVehicleInfoCmd wParam:%d \r\n"),wParam));

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander || wParam==0)
	{
		return;
	}

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
	if(panel)
	{
		((CPanelVehicleInfoWrapper*)panel)->ProcessCanbusPack((LPBYTE)pCommander->GetVchicleInfoData(),wParam);
	}
}

void CGUI_Fantasy::OnDiskCmd(WPARAM wParam, LPARAM lParam)
{
	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander || wParam==0)
	{
		return;
	}

// 	CPanelGL *panel=m_listPanels.GetItemByID(UI_DISKCORE);
// 	if(panel)
// 	{
// 		((CPanelDisk*)panel)->ProcessCommandPack(pCommander->GetDiskCmdData(),wParam);
// 	}
}
void CGUI_Fantasy::OnNaviMsg(UINT iNaviCode, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnNaviMsg]: iNaviCode:%d, lParam=0x%x\r\n"),iNaviCode,lParam));

	if(iNaviCode == NAVI_START_MSG)
		m_idNaviStatus = lParam;

#if CVTE_EN_ANDROID_AUTO
	//notify android auto
	CPanelGL *panel_aap=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
	if(panel_aap)
	{
		((CPanelAndroidAuto*)panel_aap)->OnNativeNaviMsg(iNaviCode,lParam);
	}
#endif

}
void CGUI_Fantasy::OnMcuUpdateCmd(WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(1,(_T("[CGUI_Fantasy::OnMcuUpdateCmd]: %d \r\n"),wParam));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_UPDATE,0));
	if(panel)
	{
		((CPanelUpdate*)panel)->OnMcuUpdateACK(wParam);
	}
}
void CGUI_Fantasy::NotifyUILaunched()
{

}
void CGUI_Fantasy::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
// 	m_dwLastClickTick = GetTickCount();
	static BOOL bOnMCUPackageDone = FALSE;

	if(m_bMCUUpdating)
	{
		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_UPDATE,0));
		if(panel)
		{
			((CPanelUpdate*)panel)->OnMCUCmd(wParam,lParam);
		}
        return;
	}

	if(m_bWaitingForPowerOFF)
	{
		return;
	}

	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

 	RETAILMSG(DEBUG_GUI,(_T("======>ARM: type:%d,ID:0x%x,commander:0x%x,val:%d\r\n"),dwType,dwID,lParam,pCommander->GetItemData(dwID)));

	//pre-process:
	if(dwType == 0)//package
	{
		bOnMCUPackageDone = TRUE;
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: receive mcu package\r\n")));

		m_idPowerState = pCommander->GetItemData(MCU_POWER_MODE);

		m_bOnReverseMode = pCommander->GetItemData(MCU_REVERSE_STATE);
		m_bReverseSignaled = m_bOnReverseMode;

		m_bOnBrakeMode = pCommander->GetItemData(MCU_BRAKE_STATE);
		m_iVolume = pCommander->GetItemData(MCU_AUDIO_VOLUME);
		m_bOnMute = pCommander->GetItemData(MCU_MUTE_MODE);
		m_idEQ = pCommander->GetItemData(MCU_AUDIO_EQ);
        m_bOnHWMute = (pCommander->GetItemData(MCU_HW_MUTE) == 1);
        m_iScrnBrightness = pCommander->GetItemData(MCU_SCRN_BRIGHTNESS);
		m_bAuxInDetect = pCommander->GetItemData(MCU_AUX_IN_DETECT);

		if(m_pSmartBarVol)
		{
			m_pSmartBarVol->SetEQType(m_idEQ);
		}

#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
		m_iTreble = pCommander->GetItemData(MCU_AUDIO_TREBLE);
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::package] m_iTreble[%d]\r\n"),m_iTreble));
#endif
		RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::package] m_idPowerState[%d] m_bOnReverseMode[%d]\r\n"),m_idPowerState, m_bOnReverseMode));
		RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::package] m_iScrnBrightness[%d] m_bOnHWMute[%d]\r\n"),m_iScrnBrightness, m_bOnHWMute));

		//notify brake status:
		::PostMessage(g_hUIWnd,WM_BREAK_STATUS,m_bOnBrakeMode,NULL);

		//volume:
		if(m_iVolume>32)
			m_iVolume=32;

// 		m_iSetVolume = m_iVolume;
// 		m_iVolumeCur=m_iVolume;
// 		m_iVolumeFadeTo=m_iVolume;

		//get default volume:
		int vol_t=0;
		//bluetooth volume:
//			vol_t = m_pConfig->GetCurParam()->dwVolBluetooth;
//			if(vol_t == 0)
//			{
//				m_pConfig->GetCurParam()->dwVolBluetooth = m_iVolume;
//			}
//			else
//			{
//				if(vol_t>20)vol_t=20;
//				m_pConfig->GetCurParam()->dwVolBluetooth = vol_t;
//			}
		//media volume:
		vol_t = m_pConfig->GetCurParam()->dwVolMedia;
		if(vol_t == 0)
		{
			m_pConfig->GetCurParam()->dwVolMedia = m_iVolume;
		}
		else
		{
			if(vol_t>20)vol_t=20;
			m_pConfig->GetCurParam()->dwVolMedia = vol_t;
		}
		//gps volume:
//			vol_t = m_pConfig->GetCurParam()->dwVolGPS;
//			if(vol_t == 0)
//			{
//				m_pConfig->GetCurParam()->dwVolGPS = m_iVolume;
//			}
//			else
//			{
//				if(vol_t>20)vol_t=20;
//				m_pConfig->GetCurParam()->dwVolGPS = vol_t;
//			}

		m_iVolOriginGPS=m_pConfig->GetCurParam()->dwVolGPS;
		m_iVolOriginBluetooth=m_pConfig->GetCurParam()->dwVolBluetooth;
		m_iVolOriginMedia=m_pConfig->GetCurParam()->dwVolMedia;

		if(m_bPhoneCallActive)
		{
			m_iVolume=m_iVolOriginBluetooth;
			m_iSetVolume=m_iVolOriginBluetooth;
			m_iVolumeCur=m_iVolOriginBluetooth;
			m_iVolumeFadeTo=m_iVolOriginBluetooth;
		}
		else if(m_bSpeechOn)
		{
			m_iVolume=m_iVolOriginGPS;
			m_iSetVolume=m_iVolOriginGPS;
			m_iVolumeCur=m_iVolOriginGPS;
			m_iVolumeFadeTo=m_iVolOriginGPS;
		}
		else
		{
			m_iVolume=m_iVolOriginMedia;
			m_iSetVolume=m_iVolOriginMedia;
			m_iVolumeCur=m_iVolOriginMedia;
			m_iVolumeFadeTo=m_iVolOriginMedia;
		}

		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: dwVolMedia:%d\r\n"),m_pConfig->GetCurParam()->dwVolMedia));
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: dwVolGPS:%d\r\n"),m_pConfig->GetCurParam()->dwVolGPS));
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: dwVolBluetooth:%d\r\n"),m_pConfig->GetCurParam()->dwVolBluetooth));

		//status bar:
		if(m_pStatusBar)
		{
			m_pStatusBar->SetVolume(m_iVolume);
			m_pStatusBar->SetEQ(m_idEQ);
			m_pStatusBar->SetMute(m_bOnMute);
		}

		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: Package Command: MCU_POWER_MODE:%d\r\n"),m_idPowerState));

#if (CVTE_EN_LED && (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO || CUSTOMER_ID == CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
		if (m_iScrnBrightness == 1)
		{
			m_pLED->SetLedBrightness(1.0f);
		}
		else
		{
			m_pLED->SetLedBrightness(0.0f);
		}
		m_pLED->RefreshLED();
#endif
		//power state:
		m_bManualPowerOn = (m_idPowerState == SYS_POWER_ON);
		if(m_idPowerState == SYS_POWER_OFF || m_idPowerState == SYS_POWER_IDLE)//power off or idle
		{
			if(!m_bOnReverseMode)
			{
				PowerIdle();
			}
		}
		else
		{
 			PowerOn();
            if(m_bOnHWMute)
            {
                OnMuteKey(TRUE);
            }

			//RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: m_iScrnBrightness:%d dwBacklight:%d iLCDNightModeBrightness:%d iLCDDayModeBrightness:%d\r\n"),
			//	m_iScrnBrightness,m_pConfig->GetCurParam()->dwBacklight,m_pConfig->GetCurParam()->iLCDNightModeBrightness,m_pConfig->GetCurParam()->iLCDDayModeBrightness));
			switch (m_iScrnBrightness)
			{
			case 1:
				m_bOnNightMode = TRUE;
				m_pConfig->GetCurParam()->dwBacklight = m_pConfig->GetCurParam()->iLCDNightModeBrightness;//min(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->iLCDNightModeBrightness);
				BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);	//BKL_SetBackLight(min(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight));
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pConfig->GetCurParam()->iLEDNightModeBrightness;	//min(30,m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent);
				m_bBacklightOFF=FALSE;
				OnHeadLightOn(TRUE, m_pConfig->GetCurParam()->dwBacklight);
				OnLCDBrightnessChange();
				break;
			case 2:
			default:
				m_bOnNightMode = FALSE;
				m_pConfig->GetCurParam()->dwBacklight = m_pConfig->GetCurParam()->iLCDDayModeBrightness;//max(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight);
				BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
				m_bBacklightOFF=FALSE;
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness;
				OnHeadLightOn(FALSE,m_pConfig->GetCurParam()->dwBacklight);
				OnLCDBrightnessChange();
				break;
			}

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
			//LED
			if(m_pLED && !m_bOnReverseMode)
			{
				//m_pLED->SetLedState(LED_POWERON);

				//volume:
				int iset=GetNearInt((m_iVolume)*7.0f/32.0f);
				if(iset<0)
					iset=0;
				else if(iset>7)
					iset=7;
                RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(%d) 1\r\n"), TEXT(__FUNCTION__), iset));

				m_pLED->SetLedParam(0,iset);

                //Get led current brightness
				LEDHSB ledhsb;
				memset(&ledhsb, 0, sizeof(ledhsb));
				m_pLED->GetLedHSB(&ledhsb);
				//printf("MSG:[OnMCUCmd]: ledhsb.brightness(%f)  ledbrightness(%f)\r\n", ledhsb.Brightness, ledbrightness/100.0f);

				if(m_iScrnBrightness == 1)
				{
					if(ledhsb.Brightness > m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f)
					{
						//printf("MSG:[OnMCUCmd]: NightMode SetLedBrightness(%f)\r\n",ledbrightness/100.0f);
						m_pLED->SetLedBrightness(m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f);
					}
				}
				else
				{
					//printf("MSG:[OnMCUCmd]: NormalMode SetLedBrightness(%f)\r\n",ledbrightness/100.0f);
					m_pLED->SetLedBrightness(m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f);
				}
				#if (CVTE_DEF_LED_TYPE != CVTE_LED_TYPE_U703PT && CVTE_DEF_LED_TYPE != CVTE_LED_TYPE_U705PM)
			    m_pLED->RefreshLED();
				#endif
                OnPanelLightChange();
			}
#endif

			#if CVTE_EN_CARPLAY
			{
			//Carplay:
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
			if(panel)
			{
				((CPanelCarPlay*)panel)->OnNightModeChanged(m_bOnNightMode);
			}
			}
			#endif
			#if CVTE_EN_ANDROID_AUTO
			{
			//AndroidAuto:
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
			if(panel)
			{
				((CPanelAndroidAuto*)panel)->OnNightModeChanged(m_bOnNightMode);
			}
			}
			#endif
		}
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
		case MCU_AUDIO_TREBLE:
			{
				m_iTreble = pCommander->GetItemData(MCU_AUDIO_TREBLE);
				RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: Single Command m_iTreble[%d]\r\n"),m_iTreble));
			}
			break;
#endif
		case MCU_POWER_MODE:
			{
				static BYTE s_state=-1;
				BYTE state=pCommander->GetItemData(MCU_POWER_MODE);

				if(s_state != state)
				{
					s_state = state;
					RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: Single Command MCU_POWER_MODE:%d\r\n"),state));

					if(state == SYS_POWER_OFF)//power off
					{
						if(IsPowerOn())
						{
							//store carplay last modes:
							if(m_bCarPlayConnected)
							{
								m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;
								m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI=m_pSkinManager->GetConfig()->GetCurParam()->idCurUI;

								RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: idCarPlayLastSRC=0x%x, idCarPlayLastUI=0x%x\r\n"),
									m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC,m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI));
							}
							else
							{
								m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC = 0;
								m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI = 0;

								RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: idCarPlayLastSRC=0x%x, idCarPlayLastUI=0x%x\r\n"),
									m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastSRC,m_pSkinManager->GetConfig()->GetCurParam()->idCarPlayLastUI));
							}

							//Mantis 0003458 将主机静音，关ACC，静音图标叠在关机logo上
							if(m_bManualMute)
							{
								OnMuteKey(FALSE);
							}

							#if CVTE_EN_LED
							//led down:
							if(m_pLED)
							{
                                if(m_bManualMute)
                                {
                                    m_pLED->SetLedState(LED_UNMUTE);
                                }
                                RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(0) 2\r\n"), TEXT(__FUNCTION__)));
								m_pLED->SetLedParam(0,0);
								#if (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U621P)
								m_pLED->SetLedParam(1,0);
								#endif
							}
							#endif

							//
//							m_pLED->SetLedState(LED_POWERIDLE);
//							OnPanelLightChange();
							m_bManualPowerOn = FALSE;
							ActivateRenderLogo();
							SwitchPage(UI_CLASS_POWERSTATE,0,PAGE_SWITCH_ANI_NONE,SYS_POWER_OFF);
						}
						else if(IsPowerIdle())
						{
							OnSystemPrePowerState(SYS_POWER_OFF);
							OnSystemPowerState(SYS_POWER_OFF);
						}
					}
					else if(state == SYS_POWER_ON)//power on
					{
						OnSystemPowerState(SYS_POWER_ON);
					}
					else if(state == SYS_POWER_SOFT_RESET)//reset
					{
						OnSystemPowerState(SYS_POWER_SOFT_RESET);
					}
					else if(state == SYS_POWER_IDLE)//idle
					{
						OnSystemPowerState(SYS_POWER_IDLE);
					}
				}
			}
			break;
		case MCU_BRAKE_STATE:
			{
				::PostMessage(g_hUIWnd,WM_BREAK_STATUS,pCommander->GetItemData(MCU_BRAKE_STATE),NULL);
			}
			break;
		case MCU_REVERSE_STATE:
			{
				//::PostMessage(g_hUIWnd,WM_REVERSE_STATUS,pCommander->GetItemData(MCU_REVERSE_STATE),NULL);
				//::SendMessage(g_hUIWnd,WM_REVERSE_STATUS,pCommander->GetItemData(MCU_REVERSE_STATE),NULL);
				BOOL bSingal=pCommander->GetItemData(MCU_REVERSE_STATE);
				//If we receive the single cmd first, we should ignore this cmd.
				if(m_bReverseMCUCmd && bOnMCUPackageDone && m_bReverseSignaled != bSingal)
				{
					RETAILMSG(1,(_T(">>MCU_REVERSE_STATE: ++\r\n")));
					OnRearViewPrepare(bSingal);
					SetTimer(g_hUIWnd, TIMER_REARVIEW_DONE,800,NULL);
					RETAILMSG(1,(_T(">>MCU_REVERSE_STATE: --\r\n")));
				}
				else
				{
					RETAILMSG(1,(_T(">>MCU_REVERSE_STATE:%d Skip!!! m_bReverseMCUCmd:%d bOnMCUPackageDone:%d m_bReverseSignaled:%d\r\n"),bSingal,m_bReverseMCUCmd,bOnMCUPackageDone,m_bReverseSignaled));
				}
			}
			break;
		case MCU_SCRN_BRIGHTNESS:
			{
                static BYTE scrnbefore = m_pConfig->GetCurParam()->dwBacklight;
                m_iScrnBrightness = pCommander->GetItemData(MCU_SCRN_BRIGHTNESS);

				//RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: m_iScrnBrightness:%d dwBacklight:%d iLCDNightModeBrightness:%d ：%d\r\n"),
				//	m_iScrnBrightness,m_pConfig->GetCurParam()->dwBacklight,m_pConfig->GetCurParam()->iLCDNightModeBrightness,m_pConfig->GetCurParam()->iLCDDayModeBrightness));
#if (CVTE_EN_LED && (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO || CUSTOMER_ID == CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
				if (m_iScrnBrightness == 1)
				{
					m_pLED->SetLedBrightness(1.0f);
				}
				else
				{
					m_pLED->SetLedBrightness(0.0f);
				}
				m_pLED->RefreshLED();
#endif

				if(IsPowerOn()/* && !m_bBacklightOFF*/)
				{
					switch (m_iScrnBrightness)
					{
					case 0:
						m_bOnNightMode = FALSE;
						BKL_SetBackLight(BACKLIGHT_STATE_OFF);
						m_bBacklightOFF=TRUE;
                        m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = min(10,m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent);
						OnLCDBrightnessChange();
						break;
					case 1:
						m_bOnNightMode = TRUE;
						if(!m_bBacklightOFF)
						{
							m_pConfig->GetCurParam()->dwBacklight = m_pConfig->GetCurParam()->iLCDNightModeBrightness;//min(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->iLCDNightModeBrightness);
							BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);	//BKL_SetBackLight(min(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight));
							//m_bBacklightOFF=FALSE;
						}
						m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pConfig->GetCurParam()->iLEDNightModeBrightness;	//min(30,m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent);
						OnHeadLightOn(TRUE, m_pConfig->GetCurParam()->dwBacklight);
						OnLCDBrightnessChange();
						break;
					case 2:
						m_bOnNightMode = FALSE;
						if(!m_bBacklightOFF)
						{
							m_pConfig->GetCurParam()->dwBacklight = m_pConfig->GetCurParam()->iLCDDayModeBrightness;//max(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight);
							BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
							//m_bBacklightOFF=FALSE;
						}
						m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness;
						OnHeadLightOn(FALSE,m_pConfig->GetCurParam()->dwBacklight);
						OnLCDBrightnessChange();
						break;
					}

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
					//LED
                    if(m_pLED)
                    {
                        LEDHSB ledhsb;
                        memset(&ledhsb, 0, sizeof(ledhsb));
                        m_pLED->GetLedHSB(&ledhsb);
                        //printf("MSG:[OnMCUCmd]: ledhsb.brightness(%f)  ledbrightness(%f)\r\n", ledhsb.Brightness, ledbrightness/100.0f);

                        if(m_iScrnBrightness == 1)
                        {
                            if(ledhsb.Brightness > m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f)
                            {
                                //printf("MSG:[OnMCUCmd]: NightMode SetLedBrightness(%f)\r\n",ledbrightness/100.0f);
    	                        m_pLED->SetLedBrightness(m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f);
                            }
                        }
                        else
                        {
                            //printf("MSG:[OnMCUCmd]: NormalMode SetLedBrightness(%f)\r\n",ledbrightness/100.0f);
	                        m_pLED->SetLedBrightness(m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f);
                        }

						if(m_bManualMute)
						{
							m_pLED->RefreshLED();
	                    	//m_pLED->SetLedState(LED_MUTE);
						}
						else
						{
	                        //m_pLED->SetLedBrightness(ledbrightness/100.0f);

	                        //m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = brightness;
	        				DWORD dwVol=0;
	        				if(m_bPhoneCallActive)
	        				{
	        					dwVol=m_pConfig->GetCurParam()->dwVolBluetooth;
	        				}
	        				else if(m_bSpeechOn)
	        				{
	        					dwVol=m_pConfig->GetCurParam()->dwVolGPS;
	        				}
	        				else
	        				{
	        					dwVol=m_pConfig->GetCurParam()->dwVolMedia;
	        				}

	        				int iset=GetNearInt((dwVol)*7.0f/32.0f);
	        				if(iset<0)
	        					iset=0;
	        				else if(iset>7)
	        					iset=7;
	                        //RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(%d) 7\r\n"), TEXT(__FUNCTION__), iset));
	        				m_pLED->SetLedParam(0,iset);
                        	m_pLED->RefreshLED();
						}
						OnPanelLightChange();
                    }
#endif

					#if CVTE_EN_CARPLAY
					{
						//Carplay:
						CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
						if(panel)
						{
							((CPanelCarPlay*)panel)->OnNightModeChanged(m_bOnNightMode);
						}
					}
					#endif
					#if CVTE_EN_ANDROID_AUTO
					{
						//AndroidAuto:
						CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
						if(panel)
						{
							((CPanelAndroidAuto*)panel)->OnNightModeChanged(m_bOnNightMode);
						}
					}
					#endif
				}
				#if CVTE_EN_CARPLAY
				CCarplayAdapter::GetInstance()->OnNightModeChanged(m_bOnNightMode);
				#endif
			}
			break;
		case MCU_KEY:
			{
// 				OnPanelKey(pCommander->GetItemData(MCU_KEY));
				::PostMessage(m_hMainWnd,WM_PANEL_KEY,pCommander->GetItemData(MCU_KEY),NULL);
			}
			break;
		case MCU_AUDIO_EQ:
			{
				m_idEQ = pCommander->GetItemData(MCU_AUDIO_EQ);
				if(m_pStatusBar)
				{
					m_pStatusBar->SetEQ(m_idEQ);
				}
				if(m_pSmartBarVol)
				{
					m_pSmartBarVol->SetEQType(m_idEQ);
				}
			}
			break;
		case MCU_MUTE_MODE:
			{
				m_bOnMute = pCommander->GetItemData(MCU_MUTE_MODE);
				if(m_pStatusBar)
				{
					m_pStatusBar->SetMute(m_bOnMute);
				}
                RETAILMSG(1, (TEXT("###################### MCU_MUTE_MODE %d####################\r\n"),m_bOnMute));

				//
				/*
				if(IsPowerOn() && m_pSmartBarVol && m_pCurPanel && m_dwOpenSRCTick
					&& GetTickCount() - m_dwOpenSRCTick>2000)
				{
					if(!m_bOnReverseMode && m_pCurPanel->IsEnableSmartBar() && m_pCurPanel->IsAniReady())
					{
						m_pSmartBarVol->Show(TRUE,1000);
						m_pSmartBarVol->SetMute(pCommander->GetItemData(MCU_MUTE_MODE));
					}
				}
				*/

				//
				//::PostMessage(g_hUIWnd,WM_NOTIFY_VOLUME_INFO,m_iVolume,m_bOnMute);
			}
			break;
        case MCU_HW_MUTE:
            {
                //RETAILMSG(1, (TEXT("MSG:OnMCUCmd:MCU_HW_MUTE :%d\r\n"),pCommander->GetItemData(MCU_HW_MUTE)));
                if(m_idPowerState != SYS_POWER_ON || m_bPhoneCallActive)
                {
                    RETAILMSG(1, (TEXT("MSG:OnMCUCmd:MCU_HW_MUTE: Do noting.\r\n")));
                    break;
                }

				m_bOnHWMute =  (pCommander->GetItemData(MCU_HW_MUTE) == 1);
                if(m_bOnHWMute)
                {
                    RETAILMSG(1, (TEXT("MSG:OnMCUCmd:MCU_HW_MUTE Mute\r\n")));
					OnMuteKey(TRUE);
                }
                else
                {
					RETAILMSG(1, (TEXT("MSG:OnMCUCmd:MCU_HW_MUTE UnMute\r\n")));
					if(m_bManualMute && GetTickCount()-m_dwOpenSRCTick > 3000)
					{
						OnMuteKey(m_bOnHWMute);
					}
					else
					{
						RETAILMSG(1, (TEXT("MSG:OnMCUCmd:MCU_HW_MUTE: umute too early or the curren is unmute mode. Do noting.\r\n")));
					}
                }
            }
            break;
		case MCU_CURRENT_SOURCE:
// 			if(IsPowerOn())
// 			{
				m_idCurMCUSRC = pCommander->GetItemData(MCU_CURRENT_SOURCE);
// 				RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMCUCmd]: MCU_CURRENT_SOURCE:%d\r\n"),m_idCurMCUSRC));
// 			}
			break;
		case MCU_AUDIO_VOLUME:
 			//RETAILMSG(DEBUG_GUI,(_T("CGUI_Fantasy::MCU_AUDIO_VOLUME:%d\r\n"),pCommander->GetItemData(MCU_AUDIO_VOLUME)));

            if(m_pSmartBarVol && m_bOpenSRC)
			{
				DWORD dwTick = GetTickCount();
				//RETAILMSG(1,(_T("MSG:[%s]: MCU_AUDIO_VOLUME m_bManualMute:%d m_bShowVolBar:%d dwTick:%d m_dwOpenSRCTick:%d m_dwSetVolumeTick:%d\r\n"),
				//	TEXT(__FUNCTION__),m_bManualMute,m_bShowVolBar,dwTick,m_dwOpenSRCTick,m_dwSetVolumeTick));

				//show volume bar:
				if(m_bManualMute ||
					(IsPowerOn() && m_pCurPanel && m_bShowVolBar && m_dwOpenSRCTick
						&& dwTick-m_dwOpenSRCTick>2000 && dwTick-m_dwSetVolumeTick<1000)
					)
				{
 					//RETAILMSG(DEBUG_GUI,(_T("CGUI_Fantasy::MCU_AUDIO_VOLUME:%d\r\n"),pCommander->GetItemData(MCU_AUDIO_VOLUME)));

					if(!m_bOnReverseMode && m_pCurPanel->IsEnableSmartBar())
					{
						m_pSmartBarVol->Show(TRUE,4000);
	// 					m_pSmartBarVol->SetVolume(pCommander->GetItemData(MCU_AUDIO_VOLUME));
	//					m_pSmartBarVol->SetMute(pCommander->GetItemData(MCU_MUTE_MODE));

						if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
						{
							m_pSmartBarMedia->Show(FALSE);
						}
					}
				}
			}

			if(m_pStatusBar)
			{
				m_pStatusBar->SetVolume(pCommander->GetItemData(MCU_AUDIO_VOLUME));
			}
			m_iVolume = pCommander->GetItemData(MCU_AUDIO_VOLUME);

			//::PostMessage(g_hUIWnd,WM_NOTIFY_VOLUME_INFO,m_iVolume,m_bOnMute);
			::PostMessage(g_hUIWnd,WM_VOLUME_CHANGE,m_iVolume,1);

// 			RETAILMSG(DEBUG_GUI,(_T("#########MCU_AUDIO_VOLUME:%d\r\n"),m_iVolume));

			break;

		//CAN BUS:notify head unit:
// 		case MCU_CUR_FREQU_STORE_H:
		case MCU_CUR_FREQU_STORE_L:
		case MCU_CUR_BAND_STORE:
		case MCU_CUR_CHANNEL_STORE:
			{
				if(!m_bAnimating && m_pCurPanel && GET_PANEL_CLASS_ID(m_pCurPanel->GetID())==UI_CLASS_RADIO)
				{
					UpdateFrame();
				}
				//::PostMessage(g_hUIWnd,WM_NOTIFY_RADIO_INFO,0,0);
			}
			break;
		case MCU_UPDATE_STATUS:
			{
				if(!m_bMCUUpdating)
				{
					RETAILMSG(1,(_T(">>mcu updating: %d\r\n"),pCommander->GetItemData(MCU_UPDATE_STATUS)));
					m_bMCUUpdating = (pCommander->GetItemData(MCU_UPDATE_STATUS) == 2) ;
				}
			}
			break;
        case MCU_RADIO_MIX_VOLUME:
            {
                //RETAILMSG(DEBUG_GUI,(_T("CGUI_Fantasy::MCU_RADIO_MIX_VOLUME:%d\r\n"),pCommander->GetItemData(MCU_RADIO_MIX_VOLUME)));

                if(m_pSmartBarVol && m_bOpenSRC)
    			{
    				//m_pSmartBarVol->SetVolume(m_iVolumeFadeTo,SB_VOLUME_CHANNEL_SPEECH);

    				//show volume bar:
    				if(m_bManualMute ||
    					(IsPowerOn() && m_pCurPanel && m_bShowVolBar && m_dwOpenSRCTick
    						&& GetTickCount()-m_dwOpenSRCTick>2000 && GetTickCount()-m_dwSetVolumeTick<1000)
    					)
    				{
    					if(!m_bOnReverseMode && m_pCurPanel->IsEnableSmartBar() && (m_bActivated?m_pCurPanel->IsAniReady():TRUE))
    					{
    						m_pSmartBarVol->Show(TRUE,4000);
    	// 					m_pSmartBarVol->SetVolume(pCommander->GetItemData(MCU_AUDIO_VOLUME));
    	//					m_pSmartBarVol->SetMute(pCommander->GetItemData(MCU_MUTE_MODE));

    						if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
    						{
    							m_pSmartBarMedia->Show(FALSE);
    						}
    					}
    				}
    			}

    			if(m_pStatusBar)
    			{
    				m_pStatusBar->SetVolume(pCommander->GetItemData(MCU_RADIO_MIX_VOLUME));
    			}
    			m_iVolume = pCommander->GetItemData(MCU_RADIO_MIX_VOLUME);

    			//::PostMessage(g_hUIWnd,WM_NOTIFY_VOLUME_INFO,m_iVolume,m_bOnMute);
    			::PostMessage(g_hUIWnd,WM_VOLUME_CHANGE,m_iVolume,0);

            }
            break;
		case MCU_AUX_IN_DETECT:
			{			
				m_bAuxInDetect = pCommander->GetItemData(MCU_AUX_IN_DETECT);
				RETAILMSG(1,(_T("CGUI_Fantasy::MCU_AUX_IN_DETECT:%d\r\n"),m_bAuxInDetect));
				if (m_bAuxInDetect)
				{
					m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_AUX,0,UIOPRT_SHOWHIDE,&m_bAuxInDetect);
					SwitchPage(UI_CLASS_AUX,PAGE_AUX_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0);
				}
				else
				{
					m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_AUX,0,UIOPRT_SHOWHIDE,&m_bAuxInDetect);
					ReturnToPrevPanel(PAGE_SWITCH_ANI_NONE);
				}
			}
			break;
		}
	}


	//panels process:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnMCUCmd(wParam,lParam);
		m_listPanels.Next();
	}

	//
	if(dwType == 0)//package
	{
		m_bMCUPackageUpdated = TRUE;

		RETAILMSG(1,(_T("==>connect MCU succeed!\r\n")));

		if(m_bCarPlayConnected && m_idPowerState!=SYS_POWER_ON)
		{
			RETAILMSG(1,(_T("==>MCU package received, carplay connect, power on\r\n")));
			PreparePowerOn();
			ActivateRenderLogo();
		}

		//set rearview delay:
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_SET_REARVIEW_DELAY,BYTE(m_pConfig->GetCurParam()->dwRearViewDelay));
			RETAILMSG(1,(_T("APP:MSG:GUI:initialize: _ARM_SET_REARVIEW_DELAY: %d \r\n"),m_pConfig->GetCurParam()->dwRearViewDelay));
		}
		//set power off delay:
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_SET_POWEROFF_DELAY,BYTE(m_pConfig->GetCurParam()->dwPowerOffDelay));
			RETAILMSG(1,(_T("APP:MSG:GUI:initialize: _ARM_SET_POWEROFF_DELAY: %d \r\n"),m_pConfig->GetCurParam()->dwPowerOffDelay));
		}
	}
}

#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
void CGUI_Fantasy::OnExtensibleCmd(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1,(TEXT("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));
	MCU_EXTENSIBLE_DATA *pExtendata = (MCU_EXTENSIBLE_DATA *)lParam;
	BYTE *pData = NULL;
	if (pExtendata == NULL || wParam != sizeof(MCU_EXTENSIBLE_DATA))
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: Param check failed!!!\r\n"),TEXT(__FUNCTION__)));
		return;
	}

	pData = new BYTE[pExtendata->DataLen];
	if (pData == NULL)
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: new data buffer failed!!!\r\n"),TEXT(__FUNCTION__)));
		return;
	}
	memcpy(pData, pExtendata->pData, pExtendata->DataLen);
	BYTE cmd = pExtendata->CmdID;
	switch(cmd)
	{
	case MCU_EXTEN_TIME_DATA:
		{
			RETAILMSG(1, (TEXT("MSG:[%s]: TIME_DATA minute:%d hour:%d day:%d month:%d year:%d\r\n"),
				TEXT(__FUNCTION__),
				*pData,*(pData+1),*(pData+2),*(pData+3),*(pData+4)*256+*(pData+5)));
		}
		break;
	case MCU_EXTEN_LANGUAGE:
		{
			RETAILMSG(1, (TEXT("MSG:[%s]: LANGUAGE 0x%X\r\n"),
				TEXT(__FUNCTION__),*pData));			
		}
		break;
	case MCU_EXTEN_SWCOMMAND:
		{
			RETAILMSG(1, (TEXT("MSG:[%s]: SWCOMMAND 0x%X\r\n"),
				TEXT(__FUNCTION__),*pData));	
		}
		break;
	}
}
#endif

#if !CVTE_EN_KEY_PROXY
void CGUI_Fantasy::OnPhoneKey(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnPhoneKey]: 0x%x\r\n"),wParam));
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
	if(panel)
	{
		((CPanelBluetoothWrapper*)panel)->OnKey(wParam);
	}
}
#endif

#if !CVTE_EN_KEY_PROXY
void CGUI_Fantasy::OnMediaKey(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMediaKey]:wParam[0x%x] lParam[0x%x] idCurSRC[%d]\r\n"),wParam,lParam,m_pConfig->GetCurParam()->idCurSRC));
	UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
	if(panel)
	{
		panel->OnKey(wParam);
	}
}
#endif

void CGUI_Fantasy::OnRearViewKey(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnRearViewKey]: 0x%x\r\n"),wParam));
#if CVTE_EN_CAMERA
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN));
	if(panel)
	{
		((CPanelRearView*)panel)->OnRearViewKey(wParam,lParam);
		PlaySoundEffect(0);
	}
#endif
}

#if CVTE_EN_KEY_PROXY
void CGUI_Fantasy::OnVehicleKey(WPARAM wParam, LPARAM lParam)
{
	static UINT idKey=wParam;

	RETAILMSG(DEBUG_GUI,(_T("++VehicleKey: 0x%x,status:0x%x\r\n"),wParam,lParam));

	if(lParam==KEY_STATUS_DOWN)
	{
		CKeysHandler::GetInstance()->KeyDown(idKey);
	}
	else if(lParam==KEY_STATUS_UP)
	{
		CKeysHandler::GetInstance()->KeyUp(idKey);
	}
	else if(lParam==KEY_STATUS_HOLD)
	{

	}

}
#else
void CGUI_Fantasy::OnVehicleKey(WPARAM wParam, LPARAM lParam)
{

	static UINT idKey=KEY_VCHICLE_NULL;
	static BOOL bHold=FALSE;
	static BOOL bPressed=FALSE;
	static BOOL bPrewarm=FALSE;
	static int last_vol_id=-1;

	static DWORD last_down_tick=0;
	static DWORD last_vol_add_tick=0;
	static DWORD last_vol_sub_tick=0;
	static DWORD last_src_tick=0;

	RETAILMSG(1/*DEBUG_GUI*/,(_T("++VehicleKey: 0x%x,status:0x%x\r\n"),wParam,lParam));

	if(lParam==KEY_STATUS_DOWN)
	{
		if(GetTickCount()-last_down_tick>100)
		{
			if(wParam != KEY_VCHICLE_NULL)
	 		{
	 			idKey = wParam;
	 		}
			last_down_tick = GetTickCount();

			//key process:
			switch (wParam)
			{
			case KEY_VCHICLE_NULL:
				break;
			case KEY_VCHICLE_MUTE:
				{
					if (!m_bPhoneCallActive)
					{
						if (m_bCarPlaySiriRecognizing)
						{
							CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonDown);
						}
						else
						{
							if (CarplayAPIRequestSiri(kAirPlaySiriAction_Prewarm) == kNoErr)
							{
								bPrewarm = TRUE;
							}
						}
					}
					else
					{
						PhoneKeyHIDReport_t key;
						key.uiReport=0;
						key.unReprot.PhoneMute=1;
						CarplayAPIPhoneKey(key);
					}
				}
				break;
			case KEY_VCHICLE_SRC:
				if(!bHold)
				{
					if(m_bOpenSRC)
					{
						if(GetTickCount()-last_src_tick>500)
						{
// 							if(m_pCurPanel && IsPanelHasValidAudioSrc(m_pCurPanel->GetID()))
// 								SwitchSource();
// 							else
								OnPanelChange(m_pConfig->GetCurParam()->idCurSRC,0);

							last_src_tick = GetTickCount();

							PlaySoundEffect(0);
						}
					}
				}
				break;
			case KEY_VCHICLE_VOL_ADD:
				if(!bHold)
				{
					//auto unmute:
					if(m_bManualMute && !m_bSpeechOn && !m_bPhoneCallActive)
					{
						OnMuteKey(FALSE);
						PlaySoundEffect(0);
					}
					else if(!m_bMuteOperating)
					{
// 						int vol=min(64,m_iVolumeFadeTo+1);
// 						SetVolume(vol,FALSE,TRUE);
//
// 						m_iSetVolume = vol;
// 						m_iSpeedCurVolome=m_iVehicleSpeed;
//
// 						if(m_bPhoneCallActive)
// 						{
// 							last_vol_id=1;
// 							m_iVolOriginBluetooth =vol;
// 						}
// 						else if(m_bSpeechOn)
// 						{
// 							last_vol_id=2;
// 							m_iVolOriginGPS =vol;
// 						}
// 						else
// 						{
// 							last_vol_id=3;
// 							m_iVolOriginMedia =vol;
// 						}

						if(m_bPhoneCallActive)
						{
							last_vol_id=1;
						}
						else if(m_bSpeechOn)
						{
							last_vol_id=2;
						}
						else
						{
							last_vol_id=3;
						}
						AdjustVolume(1);

						PlaySoundEffect(0);
					}
				}
				break;
			case KEY_VCHICLE_VOL_SUB:
				if(!bHold)
				{
					//auto unmute:
					if(m_bManualMute && !m_bSpeechOn && !m_bPhoneCallActive)
					{
						OnMuteKey(FALSE);
						PlaySoundEffect(0);
					}
					else if(!m_bMuteOperating)
					{
// 						int vol=max(0,m_iVolumeFadeTo-1);
// 						SetVolume(vol,FALSE,TRUE);
//
// 						m_iSetVolume = vol;
// 						m_iSpeedCurVolome=m_iVehicleSpeed;
//
// 						if(m_bPhoneCallActive)
// 						{
// 							last_vol_id=1;
// 							m_iVolOriginBluetooth =vol;
// 						}
// 						else if(m_bSpeechOn)
// 						{
// 							last_vol_id=2;
// 							m_iVolOriginGPS =vol;
// 						}
// 						else
// 						{
// 							last_vol_id=3;
// 							m_iVolOriginMedia =vol;
// 						}

						if(m_bPhoneCallActive)
						{
							last_vol_id=1;
						}
						else if(m_bSpeechOn)
						{
							last_vol_id=2;
						}
						else
						{
							last_vol_id=3;
						}
						AdjustVolume(-1);

						PlaySoundEffect(0);
					}
				}
				break;
			case KEY_VCHICLE_TEL:
				if(!bHold)
				{
					if(m_bOpenSRC)
					{
						if(m_bCarPlayConnected)
						{
							PhoneKeyHIDReport_t key;
					        key.uiReport=0;
					    	key.unReprot.Flash=1;
					        CarplayAPIPhoneKey(key);
							/*
							CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
							if(panel)
							{
								if(m_bPhoneCallActive)
								{
									((CPanelCarPlay*)panel)->OnKey(KEY_PHONE_ANSWER);
								}
								else
								{
									if(m_pConfig->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER))
										SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,2);
									else
										((CPanelCarPlay*)panel)->OnRequestUI("mobilephone:");
								}
							}
							*/
						}
						else
						{
							if(m_dwCallStatus==HF_STATE_INCOMING_CALL)
							{
								::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_ANSWER,NULL);
							}
							else if(m_dwCallStatus==HF_STATE_TALKING)
							{
								::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
							}
							else if(m_dwCallStatus==HF_STATE_OUTGOING_CALL)
							{
								::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
							}
							else if(GET_PANEL_CLASS_ID(m_pCurPanel->GetID())!=UI_CLASS_BT)
							{
								//OnPanelChange(UI_BTCORE,(PAGE_BTCORE_KEYBOARD<<16)+0);
								if(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
								{
									SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN);
								}
								else
								{
									SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_BLUETOOTH);
								}
							}
						}

						PlaySoundEffect(0);
					}
				}
				break;
			case KEY_VCHICLE_UP:
				if(!bHold)
				{
					UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
					if(m_bOpenSRC)
					{
						if(idClass == UI_CLASS_CARPLAY)
						{
							if(m_bCarPlayConnected)
							{
								ButtonHIDReport_t key;
				    			key.ucReport=0;
				    			key.unReprot.ScanNextTrack=1;
				    			CarplayAPIButtonKey(key);
							}
						}
						else
						{
							UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
							CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
							if(panel)
							{
								panel->OnKey(KEY_MEDIA_NEXT);
							}
						}

						PlaySoundEffect(0);
					}
				}
				break;
			case KEY_VCHICLE_DOWN:
				if(!bHold)
				{
					UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
					if(m_bOpenSRC)
					{
						if(idClass == UI_CLASS_CARPLAY)
						{
							if(m_bCarPlayConnected)
							{
								ButtonHIDReport_t key;
								key.ucReport=0;
								key.unReprot.ScanPreviousTrack=1;
								CarplayAPIButtonKey(key);
							}
						}
						else
						{
							CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
							if(panel)
							{
								panel->OnKey(KEY_MEDIA_PREVIOUS);
							}
						}

						PlaySoundEffect(0);
					}
				}
				break;
			}
		}

// 		bHold = FALSE;
// 		bPressed = FALSE;
	}
	else if(lParam==KEY_STATUS_UP)
	{
		last_vol_id = -1;
		bHold = FALSE;
		bPressed = FALSE;

// 		if(wParam != KEY_VCHICLE_NULL)
// 		{
// 			idKey = wParam;
// 			bPressed = TRUE;
// 		}
		switch (idKey)
		{
		case KEY_VCHICLE_MUTE:
			{
				//BOOL bsn=TRUE;
				if (!m_bPhoneCallActive)
				{

					if (m_bCarPlaySiriRecognizing || (bPrewarm && ((GetTickCount() - last_down_tick) > 600)))
					{
						CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonUp);
					}
					else if (!m_bSpeechOn)
					{
						//mute:
						if(!m_bManualMute)
						{
							OnMuteKey(TRUE);
						}
						else
						{
							OnMuteKey(FALSE);
						}
					}
				}
				else
				{
					PhoneKeyHIDReport_t key;
					key.uiReport=0;
					CarplayAPIPhoneKey(key);
				}

				PlaySoundEffect(0);
				bPrewarm = FALSE;
			}
			break;
		case KEY_VCHICLE_TEL :
			{
				if (m_bCarPlayConnected)
				{
					PhoneKeyHIDReport_t key;
			        key.uiReport=0;
			        CarplayAPIPhoneKey(key);
				}
			}
			break;
		case KEY_VCHICLE_UP :
			{
				UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
				if(m_bOpenSRC)
				{
					if(idClass == UI_CLASS_CARPLAY)
					{
						if (m_bCarPlayConnected)
						{
							ButtonHIDReport_t key;
			    			key.ucReport=0;
			    			CarplayAPIButtonKey(key);
						}
					}
				}
			}
			break;
		case KEY_VCHICLE_DOWN :
			{
				UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
				if(m_bOpenSRC)
				{
					if(idClass == UI_CLASS_CARPLAY)
					{
						if (m_bCarPlayConnected)
						{
							ButtonHIDReport_t key;
			    			key.ucReport=0;
			    			CarplayAPIButtonKey(key);
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else if(lParam==KEY_STATUS_HOLD)
	{
		bHold = TRUE;

		//key process:
		switch (wParam)
		{
		case KEY_VCHICLE_NULL:
			break;
		case KEY_VCHICLE_MUTE:
			{
				//start speech:
				if (bPrewarm && ((GetTickCount() - last_down_tick) > 600))
				{
					if (CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonDown) == kNoErr)
					{
						bPrewarm = FALSE;
					}
				}
			}
			break;
		case KEY_VCHICLE_SRC:
			break;
		case KEY_VCHICLE_VOL_ADD:
			if(!m_bMuteOperating && m_iVolumeFadeTo)
			{
				int cur_vol_id=0;
				if(m_bPhoneCallActive)
					cur_vol_id =1;
				else if(m_bSpeechOn)
					cur_vol_id =2;
				else
					cur_vol_id =3;

				if(/*GetTickCount()-last_vol_add_tick>100 &&*/ cur_vol_id==last_vol_id)
				{
					AdjustVolume(2);
// 					int vol=m_iVolumeFadeTo+2;
// 					m_iSetVolume = vol;
// 					m_iSpeedCurVolome=m_iVehicleSpeed;
//
// 					if(m_bPhoneCallActive)
// 						m_iVolOriginBluetooth =vol;
// 					else if(m_bSpeechOn)
// 						m_iVolOriginGPS =vol;
// 					else
// 						m_iVolOriginMedia =vol;
//
// 					last_vol_add_tick=GetTickCount();
// 					SetVolume(min(64,vol),FALSE,TRUE);
				}
				else
				{
					last_vol_id = -1;
				}
			}
			break;
		case KEY_VCHICLE_VOL_SUB:
			if(!m_bMuteOperating && m_iVolumeFadeTo)
			{
				static DWORD last_tick=0;

				int cur_vol_id=0;
				if(m_bPhoneCallActive)
					cur_vol_id =1;
				else if(m_bSpeechOn)
					cur_vol_id =2;
				else
					cur_vol_id =3;

				if(/*GetTickCount()-last_vol_sub_tick>100 &&*/ cur_vol_id==last_vol_id)
				{
					AdjustVolume(-2);
// 					int vol=m_iVolumeFadeTo-2;
// 					m_iSetVolume = vol;
// 					m_iSpeedCurVolome=m_iVehicleSpeed;
//
// 					if(m_bPhoneCallActive)
// 						m_iVolOriginBluetooth =vol;
// 					else if(m_bSpeechOn)
// 						m_iVolOriginGPS =vol;
// 					else
// 						m_iVolOriginMedia =vol;
//
// 					last_vol_sub_tick=GetTickCount();
// 					SetVolume(max(0,vol),FALSE,TRUE);
				}
				else
				{
					last_vol_id = -1;
				}
			}
			break;
		case KEY_VCHICLE_TEL:
			break;
		case KEY_VCHICLE_UP:
			break;
		case KEY_VCHICLE_DOWN:
			break;
		}
	}

}
#endif

void CGUI_Fantasy::AdjustVolume(INT iVolOffset)
{
    if(m_bManualMute)
    {
        OnMuteKey(FALSE);
    }
    else
    {
        if(!m_bMuteOperating && !m_bOnReverseMode && IsPowerOn())
		{
            int vol;
			m_iSpeedCurVolome=m_iVehicleSpeed;

			if(m_bPhoneCallActive)
			{
                vol=m_pConfig->GetCurParam()->dwVolBluetooth+iVolOffset;
                if(vol<0)
                    vol=0;
                if(vol>32)
                    vol=32;
                m_iSetVolume = vol;
				m_iVolOriginBluetooth =vol;
                m_pConfig->GetCurParam()->dwVolBluetooth = vol;
				SetVolume(vol,FALSE,TRUE);
			}
			else if(m_bSpeechOn /*&& !m_bOnSR */&& (IsNaviSpeechEnable() || m_bOnSR))
			{
                vol=m_pConfig->GetCurParam()->dwVolGPS+iVolOffset;
                if(vol<0)
                    vol=0;
                if(vol>32)
                    vol=32;
                m_pConfig->GetCurParam()->dwVolGPS = vol;
				m_iVolOriginGPS =vol;
                SetVolume(vol,FALSE,TRUE,TRUE,TRUE,m_bOnSR);
			}
			else
			{
                if(m_bSpeechOn)
                {
                    //SetVolume(VOLUME_DEFAULT_MINI_VALUE,TRUE,TRUE,FALSE);
                }
                else
                {
                    vol=m_pConfig->GetCurParam()->dwVolMedia+iVolOffset;
                    if(vol<0)
                        vol=0;
                    if(vol>32)
                        vol=32;
                    m_pConfig->GetCurParam()->dwVolMedia = vol;
				    m_iSetVolume = vol;
					m_iVolOriginMedia =vol;
                    SetVolume(vol,FALSE,TRUE);    //SetVolume(vol,TRUE,TRUE,FALSE);
                }
			}
			PlaySoundEffect(0);
		}
    }


}
void CGUI_Fantasy::SwitchSRC()
{
	if(!m_pConfig || !m_bManualPowerOn || m_bPowerOperating)
		return;

	BOOL bGetNextValidSrc=FALSE;
	UINT idNextSRC=-1;

    RETAILMSG(DEBUG_GUI,(_T("MSG:CGUI_Fantasy::SwitchSRC: m_idCurSrc[%d]\r\n"),m_idCurSrc));

// 	for (int i=0;i<16;i++)
// 	{
// 		RETAILMSG(DEBUG_GUI,(_T("---valid src: :%d---%d\r\n"),i,m_idValidSrcList[i]));
// 	}

	if(m_idCurSrc<MEDIA_TYPE_NUMBER)
	{
		for (UINT i=m_idCurSrc+1;i<MEDIA_TYPE_NUMBER;i++)
		{
			if(m_idValidSrcList[i])
			{
				idNextSRC=i;
				bGetNextValidSrc=TRUE;
                m_idCurSrc = idNextSRC;
				break;
			}
		}

		if(!bGetNextValidSrc)
		{
			for (UINT i=0;i<m_idCurSrc;i++)
			{
				if(m_idValidSrcList[i])
				{
					idNextSRC=i;
					bGetNextValidSrc=TRUE;
                    m_idCurSrc = idNextSRC;
					break;
				}
			}
		}
	}

	RETAILMSG(DEBUG_GUI,(_T("MSG:CGUI_Fantasy::SwitchSRC: SwitchSrc[%d], bGetNextValidSrc[%d]\r\n"),idNextSRC,bGetNextValidSrc));

	if(bGetNextValidSrc)
	{
		switch (idNextSRC)
		{
		case MEDIA_TYPE_RADIO:
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_DEFAULT);
			break;
		case MEDIA_TYPE_SD1:
			SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_DEFAULT,MEDIA_TYPE_SD1);
			break;
		case MEDIA_TYPE_USB1:
			SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_DEFAULT,MEDIA_TYPE_USB1);
			break;
        case MEDIA_TYPE_DISK:
			SwitchPage(UI_CLASS_DISK,PAGE_MEDIA_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,MEDIA_TYPE_DISK);
            break;
		case MEDIA_TYPE_A2DP:
			{
				SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_DEFAULT);
				SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_A2DP));
				SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,MEDIA_TYPE_A2DP);
			}
			break;
		case MEDIA_TYPE_IPOD:
            SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_DEFAULT);
			break;
		case MEDIA_TYPE_AUX:
            SwitchPage(UI_CLASS_AUX,PAGE_AUX_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
			break;
        case MEDIA_TYPE_CARPLAY:
			//20160428 new carplay lparam must be 0
            SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,0);
			//We don't need to auto play.
			//SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
            //OnMediaKey(KEY_MEDIA_PLAY);
            break;
        case MEDIA_TYPE_WEBLINK:
			SwitchPage(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
            break;
		case MEDIA_TYPE_EASYCONNECT:
			SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
			break;
        case MEDIA_TYPE_TV_DIGITAL:
            SwitchPage(UI_CLASS_TV,PAGE_TV_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,0);
            break;
		case MEDIA_TYPE_DAB:
			RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::SwitchSRC]: MEDIA_TYPE_DAB\r\n")));
            SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_DEFAULT);
            break;
		default:
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_DEFAULT);
			break;
		}
	}
}

#if CVTE_EN_KEY_PROXY
void CGUI_Fantasy::OnKey(UINT idKey,UINT idStatus)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnKey]: key=%d(0x%x), Status=0x%x\r\n") ,idKey, idKey, idStatus));

	switch ( idKey )
	{
	case KEY_MODE :
		if ( idStatus == EV_KEY_UP )
		{
			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
			}
			else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
			{
				if(m_bSpeechSignaled && m_bSpeechOn)
				{
					UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
				}
				else
				{
					UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
				}
			}
			else
			{
				static DWORD dwlast=0;
				if(GetTickCount()-dwlast>1000)
				{
					dwlast=GetTickCount();
					SwitchSRC();
					//PlaySoundEffect(0);
				}
			}
		}
		break;
	case KEY_FLASH :
	case KEY_HOOK_SWITCH :
	case KEY_DROP :
	case KEY_ACH :
		if (0);
#if CVTE_EN_CARPLAY
		else if(m_bCarPlayConnected)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
			if(panel)
			{
				((CPanelCarPlay*)panel)->OnKey(idKey, idStatus);
			}
		}
#endif
#if CVTE_EN_ANDROID_AUTO
		else if(m_bAndroidAutoConnect && (GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_ANDROID_AUTO))
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
			if(panel)
			{
				((CPanelAndroidAuto*)panel)->OnKey(idKey, idStatus);
			}
		}
#endif
		else
		{
			if( (m_dwCallStatus==HF_STATE_INCOMING_CALL)
				||	(m_dwCallStatus==HF_STATE_TALKING)
				||	(m_dwCallStatus==HF_STATE_OUTGOING_CALL) )
			{
				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
				if(panel)
				{
					((CPanelBluetoothWrapper*)panel)->OnKey(idKey,idStatus);
				}
			}
			else if(GET_PANEL_CLASS_ID(m_pCurPanel->GetID())!=UI_CLASS_BT)
			{
				if ( idStatus == EV_KEY_UP )
				{
					//OnPanelChange(UI_BTCORE,(PAGE_BTCORE_KEYBOARD<<16)+0);
					if(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
					{
						SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN);
					}
					else
					{
						SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_BLUETOOTH);
					}
					PlaySoundEffect(0);
				}
			}
		}
		break;
	case KEY_MUTE :
		{
			if(IsPowerOn() && !m_bPhoneCallActive)
			{
				if ( idStatus == EV_KEY_UP )
				{
					if(m_bManualMute)
					{
						OnMuteKey(FALSE);
					}
					else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
					{
						if(m_bSpeechSignaled && m_bSpeechOn)
						{
							UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
						}
						else
						{
							UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
						}
					}
					else
					{
						OnMuteKey(TRUE);
					}
					PlaySoundEffect(0);
				}
			}
			else
			{
#if CVTE_EN_CARPLAY
				if ( m_bCarPlayConnected )
				{
					CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
					if(panel)
					{
						((CPanelCarPlay*)panel)->OnKey(idKey, idStatus);
					}
				}
				else
#endif
				{
					CPanelGL * panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
					if(panel)
					{
						((CPanelBluetoothWrapper*)panel)->OnKey(idKey, idStatus);
					}
				}
			}
		}
		break;
	case KEY_PLAY :
	case KEY_PAUSE :
	case KEY_STOP :
	case KEY_RESUME:
		{
			UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
			if(panel)
			{
				panel->OnKey(idKey,idStatus);
			}
		}
		break;
	case KEY_NEXT :
	case KEY_PREVIOUS :
		if(m_bOpenSRC)
		{
			if(m_bManualMute)
			{
				if (idStatus & EV_KEY_UP)
					OnMuteKey(FALSE);
			}
			else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
			{
				if(m_bSpeechSignaled && m_bSpeechOn)
				{
					UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
				}
				else
				{
					UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
				}
			}
			else
			{
				UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));

				if(panel)
				{
					panel->OnKey(idKey,idStatus);
				}
			}
			if ( idStatus & (EV_KEY_PRESS_HOLD|EV_KEY_UP))
			{
				PlaySoundEffect(0);
			}
		}
		break;
	case KEY_VOLUMEUP :
		if ( idStatus & (EV_KEY_PRESS_HOLD|EV_KEY_DOWN) )
		{
			AdjustVolume(1);
		}
		break;
	case KEY_VOLUMEDOWN :
		if ( idStatus & (EV_KEY_PRESS_HOLD|EV_KEY_DOWN) )
		{
			AdjustVolume(-1);
		}
		break;
	case KEY_VOL_MANUAL:
		if (idStatus & EV_KEY_UP)
		{
			if(IsPowerOn() && !m_bMuteOperating)
			{
				if(m_bManualMute && !m_bPhoneCallActive)
				{
					if(GetTickCount()-m_dwLastMuteTick>500)
					{
						OnMuteKey(FALSE);
					}
				}
				else
				{
					int vol = 0;
					if(m_bPhoneCallActive)
					{
						vol = CVTE_REG_AUDIO_VOLUME_PHONE_DEFAULT;
						RETAILMSG(DEBUG_GUI,(_T("MSG:[%s]: Set bluetooth default volume %d\r\n"),TEXT(__FUNCTION__),vol));
						m_pConfig->GetCurParam()->dwVolBluetooth = vol;
						m_iVolOriginBluetooth =vol;
						SetVolume(vol,TRUE,TRUE,FALSE);
					}
					else if(m_bSpeechOn && (m_bOnSR || IsNaviSpeechEnable()))
					{
						vol = CVTE_REG_AUDIO_VOLUME_SPEECH_DEFAULT;
						RETAILMSG(DEBUG_GUI,(_T("MSG:[%s]: Set GPS default volume %d\r\n"),TEXT(__FUNCTION__),vol));
						m_pConfig->GetCurParam()->dwVolGPS = vol;
						m_iVolOriginGPS =vol;
						SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
					}
					else
					{
						if(!m_bSpeechOn)
						{
							vol = CVTE_REG_AUDIO_VOLUME_MEDIA_DEFAULT;
							RETAILMSG(DEBUG_GUI,(_T("MSG:[%s]: Set Media default volum %d\r\n"),TEXT(__FUNCTION__),vol));
							m_pConfig->GetCurParam()->dwVolMedia = vol;
							m_iVolOriginMedia =vol;
							SetVolume(vol,TRUE,TRUE,FALSE);
						}
					}

					if (vol)
					{
						m_iSetVolume = vol;
						m_iSpeedCurVolome=m_iVehicleSpeed;
					}
					PlaySoundEffect(0);
				}
			}
		}
		break;
	case KEY_POWER :
		if ( idStatus == EV_KEY_UP )
		{
			if (m_pSkinManager->GetConfig()->GetCurParam()->bSnapshotEnable)
			{
				if(IsPowerOn())
				{
					if(GetTickCount()-m_dwSnapshotDoneTick>1000)
					{
						m_dwSnapshotDoneTick = GetTickCount();
						PlaySoundEffect(0);
						static int i=0;
						WCHAR str[MAX_PATH];
						swprintf_s(str,MAX_PATH,_T("\\USB Disk\\shot%04d.bmp"),i);
						RETAILMSG(1, (TEXT("\r\n## Shot Screen succeed, %s ##\r\n"),str));
						ScreenOff();
						ShotScreen(str);
						ScreenOn();
						i++;
					}
				}	
				return;
			}

			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
			}
			else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
			{
				if(m_bSpeechSignaled && m_bSpeechOn)
				{
					UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
				}
				else
				{
					UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
				}
			}
			else if(!m_bOnTAMode && !m_bOnReverseMode && !m_bPhoneCallActive && m_idMcuUpdateStatus!=UPDATE_MCU_UPDATING)
			{		
				DWORD dwTick = GetTickCount();
				if(IsPowerOn())
				{
					if(dwTick-m_dwPowerStateTick>3000 && !m_bPowerOperating)
					{
						RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Power Off ##\r\n")));

						UINT idui=m_pConfig->GetCurParam()->idCurUI;
#if CVTE_EN_NAVI
						m_bAutoStartNavi = (idui==MAKE_PANEL_ID(UI_CLASS_NAVI,0));
#else
						m_bAutoStartNavi = FALSE;
#endif
						PreparePowerIdle();
						ActivateRenderLogo();
					}
				}
				else
				{
					if(dwTick-m_dwPowerStateTick>3000 && !m_bPowerOperating)
					{
						RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Power On ##\r\n")));
						//KillTimer(g_hUIWnd,TIMER_LED_POWER_BREATH);
						DoPowerOn();
						PreparePowerOn();
						ActivateRenderLogo();
					}
				}
			}
			PlaySoundEffect(0);
		}
		break;
	case KEY_HOME_POWER:
		{
			if (m_pSkinManager->GetConfig()->GetCurParam()->bSnapshotEnable && (idStatus == EV_KEY_UP || idStatus == EV_KEY_SHORT_PRESS))
			{
				if(IsPowerOn())
				{
					if(GetTickCount()-m_dwSnapshotDoneTick>1000)
					{
						m_dwSnapshotDoneTick = GetTickCount();
						PlaySoundEffect(0);
						static int i=0;
						WCHAR str[MAX_PATH];
						swprintf_s(str,MAX_PATH,_T("\\USB Disk\\shot%04d.bmp"),i);
						RETAILMSG(1, (TEXT("\r\n## Shot Screen succeed, %s ##\r\n"),str));
						ScreenOff();
						ShotScreen(str);
						ScreenOn();
						i++;
					}
				}	
				return;
			}

			BOOL bKeyBeep = FALSE;
			if ( (idStatus == EV_KEY_SHORT_PRESS) || (idStatus == EV_KEY_DOUBLE_PRESS) )
			{
				if(IsPowerOn() && !m_bOnReverseMode && !m_bPowerOperating)//enable switch power while not on reverse mode,phone active,or mcu updating
				{
					static DWORD last_home_key=0;
					bKeyBeep=TRUE;
					//
					if(m_pCurPanel && m_bManualPowerOn/* && GetTickCount()-last_home_key>1000*/)
					{
						last_home_key = GetTickCount();
#if CVTE_EN_WEBLINK
						if (GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_WEBLINK)
						{
							CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER));
							if(panel)
							{
								panel->OnKey(idKey, idStatus);
							}
						}
						else
#endif
						{
							if(m_bBacklightOFF)
							{
								ScreenOn();
							}
							else if(m_bManualMute)
							{
								OnMuteKey(FALSE);
							}
							else if(m_pCurPanel->GetID()==MAKE_PANEL_ID(UI_CLASS_EXPLORER,0))
							{
								RETAILMSG(1, (TEXT("=>ReturnToPrevPanel\r\n")));
								ReturnToPrevPanel(PAGE_SWITCH_ANI_RIGHT);
							}
							else if(m_pCurPanel->GetID()!=MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER))
							{
								RETAILMSG(1, (TEXT("=>SwitchPage HOME\r\n")));
								SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
							}
#if CVTE_EN_NAVI
							else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_NAVI && m_idNaviStatus != 0)
							{
								RETAILMSG(1, (TEXT("=>SwitchPage NAVI\r\n")));
								SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN);
							}
#endif
							else if(m_bCarPlayConnected && GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_CARPLAY)
							{
								RETAILMSG(1, (TEXT("=>SwitchPage CarPlay\r\n")));
								SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0);
							}
#if CVTE_EN_EASYCONNECT
							else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_EASYCONNECTED)
							{
								RETAILMSG(1, (TEXT("=>SwitchPage Easyconnect\r\n")));
								SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER);
							}
#endif
#if CVTE_EN_WEBLINK
							else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_WEBLINK)
							{
								RETAILMSG(1, (TEXT("=>SwitchPage WebLink\r\n")));
								SwitchPage(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER);
							}
#endif
							else if(m_bPhoneCallActive)
							{
								if(m_bCarPlayConnected)
								{
									RETAILMSG(1, (TEXT("=>SwitchPage Phyonecall CarPlay\r\n")));
									SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0);
								}
								else if(m_bAndroidAutoConnect)
								{
									RETAILMSG(1, (TEXT("=>SwitchPage Phonecall AndroidAuto\r\n")));
									SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0);
								}
								else
								{
									RETAILMSG(1, (TEXT("=>SwitchPage Phyonecall BT\r\n")));
									SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING);
								}
							}
							else
							{
								RETAILMSG(1, (TEXT("=>SwitchPage CurClass:%d CurPage:%d\r\n"),GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC)));
								SwitchPage(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),
									GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC));
							}

							//refresh UI at once:
							UpdateFrame();
						}
					}
					else
					{
						RETAILMSG(1, (TEXT("\r\n## On HOME_POWER Key ## INVALID!!! m_pCurPanel:0x%X m_bManualPowerOn:%d\r\n"),m_pCurPanel,m_bManualPowerOn));
					}
				}
	            else if(!m_bOnTAMode && !IsPowerOn() && !m_bOnReverseMode && !m_bPhoneCallActive && m_idMcuUpdateStatus!=UPDATE_MCU_UPDATING)
	    	    {
					if( GetTickCount() - m_dwPowerStateTick > 3000 )
					{
						RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Power On ##\r\n")));
						bKeyBeep=TRUE;
						DoPowerOn();
						PreparePowerOn();
						ActivateRenderLogo();
					}
	            }
			}
			else if ( idStatus == EV_KEY_LONG_PRESS )
			{
				if(!m_bPowerOperating && !m_bOnTAMode && !m_bOnReverseMode && !m_bPhoneCallActive && m_idMcuUpdateStatus!=UPDATE_MCU_UPDATING)
	    	    {
					if(GetTickCount() - m_dwPowerStateTick > 3000)
					{
	    				if(m_idPowerState==SYS_POWER_ON)
	    				{
							RETAILMSG(1, (TEXT("\r\n## On HOME_POWER Key ## =>Power Idle \r\n")));
							//SetTimer(g_hUIWnd,TIMER_LED_POWER_BREATH,6000,NULL);

    						UINT idui=m_pConfig->GetCurParam()->idCurUI;
#if CVTE_EN_NAVI
							m_bAutoStartNavi = (idui==MAKE_PANEL_ID(UI_CLASS_NAVI,0));
#else
							m_bAutoStartNavi = FALSE;
#endif
    						PreparePowerIdle();
							ActivateRenderLogo();
							bKeyBeep=TRUE;
						}
						else
						{
							RETAILMSG(1, (TEXT("\r\n## On HOME_POWER Key ## => Power On \r\n")));
							//KillTimer(g_hUIWnd,TIMER_LED_POWER_BREATH);
							bKeyBeep=TRUE;
        					DoPowerOn();
							PreparePowerOn();
							ActivateRenderLogo();
						}
					}
	    		}
			}
			if(bKeyBeep)
			{
				PlaySoundEffect(0);
			}
		}
		break;
	case KEY_SPEECH :
		if(IsPowerOn() && !m_bPhoneCallActive && m_bManualPowerOn && !m_bPowerOperating)
		{
			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
				break;
			}

#if CVTE_EN_CARPLAY
			if(m_bCarPlayConnected)
			{
				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
				if(panel)
				{
					((CPanelCarPlay*)panel)->OnKey(idKey,  idStatus == EV_KEY_DOWN ? EV_KEY_LONG_PRESS : idStatus);
				}
			}
#endif
#if CVTE_EN_ANDROID_AUTO
			if(m_bAndroidAutoConnect)
			{
				CPanelGL *panel_aap=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
				if(panel_aap)
				{
					((CPanelAndroidAuto*)panel_aap)->OnKey(idKey, idStatus);
				}
			}
#endif
		}
		break;
	case KEY_SEEK :
		if ( idStatus == EV_KEY_UP )
		{
			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
			}
			else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
			{
				if(m_bSpeechSignaled && m_bSpeechOn)
				{
					UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
				}
				else
				{
					UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
				}
			}
			else
			{
				SendRadioCommand(RADIO_CMD_AMS,1);
			}
			PlaySoundEffect(0);
		}
		break;
	case KEY_MUTE_SPEECH :
		if ( idStatus == EV_KEY_DOWN )
		{
			if (!m_bPhoneCallActive)
			{
#if CVTE_EN_CARPLAY
				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
				if(panel)
				{
					((CPanelCarPlay*)panel)->OnKey(idKey, idStatus);
				}
#endif
			}
			else
			{
				OnKey(KEY_MUTE, idStatus);
			}
		}
		else if ( idStatus == EV_KEY_UP )
		{
			BOOL bsn=TRUE;
			if (!m_bPhoneCallActive)
			{
				if (m_bCarPlaySiriRecognizing)
				{
#if CVTE_EN_CARPLAY
					CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
					if(panel)
					{
						((CPanelCarPlay*)panel)->OnKey(idKey, idStatus);
					}
#endif
				}
				else if (!m_bSpeechOn)
				{
					OnKey(KEY_MUTE, idStatus);
				}
			}
			else
			{
				OnKey(KEY_MUTE, idStatus);
			}
		}
		else if ( idStatus == EV_KEY_LONG_PRESS )
		{

		}
		break;
	case KEY_HOME:
		if( idStatus == EV_KEY_UP)
		{
#if 0
			if(IsPowerOn() && !m_bOnReverseMode && !m_bPowerOperating)//enable switch power while not on reverse mode,phone active,or mcu updating
			{
				static DWORD dwlast=0;					
				//
				if(m_pCurPanel && m_bManualPowerOn && GetTickCount()-dwlast>500)
				{
					dwlast=GetTickCount();
#if CVTE_EN_WEBLINK
					if (GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_WEBLINK)
					{
						CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER));
						if(panel)
						{
							panel->OnKey(idKey, idStatus);
						}
					}
					else
#endif
					{
						if(m_bBacklightOFF)
						{
							ScreenOn();
						}
						else if(m_bManualMute)
						{
							OnMuteKey(FALSE);
						}
						else if(m_pCurPanel->GetID()==MAKE_PANEL_ID(UI_CLASS_EXPLORER,0))
						{
							RETAILMSG(1, (TEXT("=>ReturnToPrevPanel\r\n")));
							ReturnToPrevPanel(PAGE_SWITCH_ANI_RIGHT);
						}
						else if(m_pCurPanel->GetID()!=MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER))
						{
							RETAILMSG(1, (TEXT("=>SwitchPage HOME\r\n")));
							SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
						}
						else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_NAVI && m_idNaviStatus != 0)
						{
							RETAILMSG(1, (TEXT("=>SwitchPage NAVI\r\n")));
							SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN);
						}
						else if(m_bCarPlayConnected && GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_CARPLAY)
						{
							RETAILMSG(1, (TEXT("=>SwitchPage CarPlay\r\n")));
							SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0);
						}
#if CVTE_EN_EASYCONNECT
						else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_EASYCONNECTED)
						{
							RETAILMSG(1, (TEXT("=>SwitchPage Easyconnect\r\n")));
							SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER);
						}
#endif
#if CVTE_EN_WEBLINK
						else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI) == UI_CLASS_WEBLINK)
						{
							RETAILMSG(1, (TEXT("=>SwitchPage WebLink\r\n")));
							SwitchPage(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER);
						}
#endif
						else if(m_bPhoneCallActive)
						{
							if(m_bCarPlayConnected)
							{
								RETAILMSG(1, (TEXT("=>SwitchPage Phyonecall CarPlay\r\n")));
								SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0);
							}
							else if(m_bAndroidAutoConnect)
							{
								RETAILMSG(1, (TEXT("=>SwitchPage Phonecall AndroidAuto\r\n")));
								SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0);
							}
							else
							{
								RETAILMSG(1, (TEXT("=>SwitchPage Phyonecall BT\r\n")));
								SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING);
							}
						}
						else
						{
							RETAILMSG(1, (TEXT("=>SwitchPage CurClass:%d CurPage:%d\r\n"),GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC)));
							SwitchPage(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),
								GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC));
						}

						//refresh UI at once:
						UpdateFrame();
					}
				}
				else
				{
					RETAILMSG(1, (TEXT("\r\n## On HOME_POWER Key ## INVALID!!! m_pCurPanel:%X m_bManualPowerOn:%d\r\n"),m_pCurPanel,m_bManualPowerOn));
				}
				PlaySoundEffect(0);		
			}
#endif
			if(IsPowerOn())
			{
				if(m_bManualMute)
				{
					OnMuteKey(FALSE);
				}
#if 0
				else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
				{
					if(m_bSpeechSignaled && m_bSpeechOn)
					{
						UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
					}
					else
					{
						UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
					}
				}
#endif
				else
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
				}
				PlaySoundEffect(0);			
			}
		}
		break;
	case KEY_HOMEPAGE:
		if(IsPowerOn())
		{
			if( idStatus == EV_KEY_SHORT_PRESS)
			{
				if(m_bManualMute)
				{
					OnMuteKey(FALSE);
				}
				else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
				{
					if(m_bSpeechSignaled && m_bSpeechOn)
					{
						UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
					}
					else
					{
						UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
					}
				}
				else
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
				}
				PlaySoundEffect(0);
			}
			else if( idStatus == EV_KEY_LONG_PRESS)
			{
				DWORD dwTick = GetTickCount();
				if(m_idPowerState==SYS_POWER_ON)
				{
					if(!m_bOnTAMode && !m_bOnReverseMode && !m_bPhoneCallActive && dwTick-m_dwPowerStateTick>3000 && !m_bPowerOperating)
					{
						RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Power Off ##\r\n")));

						UINT idui=m_pConfig->GetCurParam()->idCurUI;
#if CVTE_EN_NAVI
						m_bAutoStartNavi = (idui==MAKE_PANEL_ID(UI_CLASS_NAVI,0));
#else
						m_bAutoStartNavi = FALSE;
#endif
						PreparePowerIdle();
						ActivateRenderLogo();
					}
				}
				PlaySoundEffect(0);
			}
			else if( idStatus == EV_KEY_DOUBLE_PRESS)
			{
				static DWORD dwlast=0;
				if(GetTickCount()-dwlast>1000)
				{
					dwlast=GetTickCount();
					SwitchSRC();
					//PlaySoundEffect(0);
				}
			}
		}
		else
		{
			if( idStatus & (EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS))
			{
				DWORD dwTick = GetTickCount();
				if(dwTick-m_dwPowerStateTick>3000 && !m_bPowerOperating)
				{
					RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Power On ##\r\n")));
					//KillTimer(g_hUIWnd,TIMER_LED_POWER_BREATH);
					DoPowerOn();
					PreparePowerOn();
					ActivateRenderLogo();
				}
				PlaySoundEffect(0);
			}
		}
		break;
	case KEY_BRIGHTNESS_TOGGLE:
		{
			if (m_bBacklightOFF)
			{
				if ( idStatus & (EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS))
				{
					ScreenOn();
				}
			}
			else
			{
				if ( idStatus == EV_KEY_SHORT_PRESS)
				{
					if(m_bManualMute)
					{
						OnMuteKey(FALSE);
						break;
					}
					
					if (m_bOnNightMode)
					{
						if(m_pConfig->GetCurParam()->dwBacklight > CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START)
							m_pConfig->GetCurParam()->dwBacklight = CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START;
						else
							m_pConfig->GetCurParam()->dwBacklight = CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END;
						m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness = m_pConfig->GetCurParam()->dwBacklight;
					}
					else
					{
						if(m_pConfig->GetCurParam()->dwBacklight > CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START)
							m_pConfig->GetCurParam()->dwBacklight = CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START;
						else
							m_pConfig->GetCurParam()->dwBacklight = 100;
						m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness = m_pConfig->GetCurParam()->dwBacklight;
					}

					BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);
					OnLCDBrightnessChange();
					PlaySoundEffect(0);
				}
				else if ( idStatus == EV_KEY_LONG_PRESS)
				{
					ScreenOff();
					PlaySoundEffect(0);
				}
			}
		}
		break;
	case KEY_RADIO:
		if ( idStatus == EV_KEY_UP)
		{
			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
			}
			else
			{
				if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) != UI_CLASS_RADIO)
				{
					SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_DEFAULT);
				}
				else
				{
					static BYTE nBand = 0;
					if(nBand)
					{
						nBand = 0;
					}
					else
					{
						nBand = 1;
					}
					SendRadioCommand(RADIO_CMD_SELECT_BAND,nBand);
				}
			}

			PlaySoundEffect(0);
		}
		break;
	case KEY_NAVI:
		if ( idStatus == EV_KEY_UP)
		{
			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
			}
			else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
			{
				if(m_bSpeechSignaled && m_bSpeechOn)
				{
					UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
				}
				else
				{
					UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
				}
			}
			else if(m_bOpenSRC)
			{
				static DWORD lasttick=0;
				if(GetTickCount() - lasttick>500/*2000*/)
				{
					lasttick = GetTickCount();

					if(m_pCurPanel?m_pCurPanel->GetID()==MAKE_PANEL_ID(UI_CLASS_NAVI,0):FALSE)
					{
						//test:
						// 				ShotScreen(_T("\\ResidentFlash\\shot.bmp"));
						OnPanelChange(m_pConfig->GetCurParam()->idCurSRC==MAKE_PANEL_ID(UI_CLASS_NAVI,0)?MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER):m_pConfig->GetCurParam()->idCurSRC,NULL);
					}
					else
					{
						OnPanelChange(MAKE_PANEL_ID(UI_CLASS_NAVI,0),0);
					}
				}
				PlaySoundEffect(0);
			}
		}
		break;
	case KEY_POWER_SCREEN:
		if(IsPowerOn())
		{
			if( idStatus == EV_KEY_SHORT_PRESS)
			{
				if (m_bBacklightOFF)
				{
					ScreenOn();
				}
				else
				{
					if(m_bManualMute)
					{
						OnMuteKey(FALSE);
					}
					else
					{
						ScreenOff();
					}
				}
				PlaySoundEffect(0);
			}
			else if( idStatus == EV_KEY_LONG_PRESS)
			{
				if(!m_bOnTAMode && !m_bOnReverseMode && !m_bPhoneCallActive && GetTickCount()-m_dwPowerStateTick>3000 && !m_bPowerOperating)
				{
					RETAILMSG(1, (TEXT("\r\n##KEY_POWER_SCREEN## =>Power Off\r\n")));

					UINT idui=m_pConfig->GetCurParam()->idCurUI;
#if CVTE_EN_NAVI
					m_bAutoStartNavi = (idui==MAKE_PANEL_ID(UI_CLASS_NAVI,0));
#else
					m_bAutoStartNavi = FALSE;
#endif
					PreparePowerIdle();
					ActivateRenderLogo();
				}
				PlaySoundEffect(0);
			}
		}
		else
		{
			if( idStatus & (EV_KEY_SHORT_PRESS|EV_KEY_LONG_PRESS))
			{
				if(GetTickCount()-m_dwPowerStateTick>3000 && !m_bPowerOperating)
				{
					RETAILMSG(DEBUG_GUI, (TEXT("\r\n##KEY_POWER_SCREEN## =>Power On\r\n")));
					//KillTimer(g_hUIWnd,TIMER_LED_POWER_BREATH);
					DoPowerOn();
					PreparePowerOn();
					ActivateRenderLogo();
					PlaySoundEffect(0);
				}
			}
		}
		break;
	case KEY_RETURN:
		if ( idStatus == EV_KEY_UP)
		{
			if(IsPowerOn())
			{
				if(m_bManualMute)
				{
					OnMuteKey(FALSE);
				}
				else
				{
					ReturnToPrevPanel(PAGE_SWITCH_ANI_RIGHT);
				}
			}
		}
		break;
	case KEY_MUTE_SCREEN:
		{
			if (IsPowerOn() && !m_bPhoneCallActive)
			{
				if (idStatus == EV_KEY_SHORT_PRESS)
				{
					if(m_bManualMute)
					{
						RETAILMSG(DEBUG_GUI, (TEXT("\r\n## OnMuteKey ## FALSE\r\n")));
						OnMuteKey(FALSE);
					}
					else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
					{
						if(m_bSpeechSignaled && m_bSpeechOn)
						{
							UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
						}
						else
						{
							UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
						}
					}
					else
					{
						RETAILMSG(DEBUG_GUI, (TEXT("\r\n## OnMuteKey ## TRUE\r\n")));
						OnMuteKey(TRUE);
					}
					m_dwLastMuteTick = GetTickCount();
					PlaySoundEffect(0);
				}
				else if (idStatus == EV_KEY_LONG_PRESS)
				{
					if(m_idMcuUpdateStatus!=UPDATE_MCU_UPDATING && !m_bOnReverseMode)
					{
						if(m_bBacklightOFF)
						{
							RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Screen On ##\r\n")));
							ScreenOn();
						}
						else
						{
							RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Screen Off ##\r\n")));
							//SwitchPage(UI_CLASS_IDLE,0,PAGE_SWITCH_ANI_NONE,m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen);
							ScreenOff();
						}
					}
					PlaySoundEffect(0);
				}
			}
		}
		break;
	default:
		break;
	}
}


void CGUI_Fantasy::OnPanelKey(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnPanelKey]: 0x%x\r\n"),wParam));
	static UINT lastkey = 0;
	if(wParam == KEY_STW_UP)
	{
		CKeysHandler::GetInstance()->KeyUp(lastkey);
	}
	else
	{
		lastkey = (UINT)wParam;
		CKeysHandler::GetInstance()->KeyDown(lastkey);
	}
}
#else
void CGUI_Fantasy::OnPanelKey(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnPanelKey]: 0x%x\r\n"),wParam));

#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
	switch (wParam)
	{
	case KEY_PHY_KNOB_LEFT:
	case KEY_PHY_KNOB_RIGHT:
	case KEY_STW_UP:
		break;
	default:
		if(GetTickCount()-m_dwLastClickTick<200 || ((m_idPowerState == SYS_POWER_IDLE) && wParam!=KEY_STW_POWER && wParam!=KEY_PHY_KNOB_BUTTON))
			return;
		break;
	}
#else
    if(GetTickCount()-m_dwLastClickTick<200 || ((m_idPowerState == SYS_POWER_IDLE) && wParam!=KEY_STW_POWER))
        return;
#endif
	/*
	//auto turn on backlight:
	if(m_bBacklightOFF)
	{
		BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);
		m_bBacklightOFF=FALSE;
		return;
	}
	*/

	switch (wParam)
	{
		case KEY_MCU_SRC:
		case KEY_STW_MODE://STW KEY
			{
                RETAILMSG(1, (TEXT("MSG:[OnPanelKey]: m_bManualMute:%d, m_iVolumeCur:%d\r\n"),m_bManualMute,m_iVolumeCur));
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else
                {
    				static DWORD dwlast=0;
    				if(GetTickCount()-dwlast>1000)
    				{
    					dwlast=GetTickCount();
    					SwitchSRC();
                        //PlaySoundEffect(0);
    				}
                }
				PlaySoundEffect(0);
			}
			break;
		case KEY_MCU_MENU://
			if(m_bOpenSRC)
			{
				static DWORD lasttick=0;
				if(GetTickCount() - lasttick>500/*2000*/)
				{
					lasttick = GetTickCount();

					if(m_pCurPanel?m_pCurPanel->GetID()==MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER):FALSE)
						OnPanelChange(/*m_pConfig->GetCurParam()->idCurSRC*/m_pConfig->GetCurParam()->idPreUI,0);
					else
						OnPanelChange(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER),0);
				}
			}
			break;
		case KEY_MCU_RADIO://radio
			if(m_bOpenSRC)
			{
				if(m_pCurPanel?m_pCurPanel->GetID()==MAKE_PANEL_ID(UI_CLASS_RADIO,0):FALSE)
				{
					//switch band:
					static BYTE band=0;
					if(m_pCommander)
					{
						m_pCommander->SendCmd(_ARM_RADIO_BAND_SEL_KEY,band);
					}
					band=(band==0)?1:0;
				}
				else
				{
					OnPanelChange(MAKE_PANEL_ID(UI_CLASS_RADIO,0),0);
				}
			}
			break;
		case KEY_MCU_AUX://aux
			if(m_bOpenSRC)
			{
				//OnPanelChange(UI_AUXCORE,AUX_INPUT_FRONT);
				//::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,MAKE_PANEL_ID(UI_CLASS_MEDIA,0),MEDIA_AUDIO);
				//OnPanelChange(MAKE_PANEL_ID(UI_CLASS_MEDIA,0),MEDIA_AUDIO);
			}
			break;
		case KEY_MCU_DVD_SRC://dvd
			if(m_bOpenSRC)
			{
				//OnPanelChange(UI_DISKCORE,0);
			}
			break;
		case KEY_MCU_DVD_EJECT:
			{
// 				CPanelGL *panel=m_listPanels.GetItemByID(UI_DISKCORE);
// 				if(panel)
// 				{
// 					((CPanelDisk*)panel)->OnEjectKey();
// 				}
			}
			break;
		case KEY_MCU_PHONE://
		case KEY_STW_PHONE_CALL:
			if(m_bOpenSRC)
			{
                RETAILMSG(1, (TEXT("OnPanelKey KEY_STW_PHONE_CALL m_pSmartBarPhone->GetType()=[%d] m_dwCallStatus[%d] m_bCarPlayConnected[%d]\r\n"),m_pSmartBarPhone->GetType(),m_dwCallStatus,m_bCarPlayConnected));

                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }

//					if(m_pSmartBarPhone->IsVisible() && m_pSmartBarPhone->GetType()==SB_INCOMING)
//					{
//						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_ANSWER,NULL);
//					}
//					else if(m_pSmartBarPhone->IsVisible() && m_pSmartBarPhone->GetType()==SB_TALKING)
//					{
//						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
//					}
//					else if(m_pSmartBarPhone->IsVisible() && m_pSmartBarPhone->GetType()==SB_OUTGOING)
//					{
//						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
//					}
// 					else if(m_pCurPanel?m_pCurPanel->GetID()==UI_BTCORE:FALSE)
// 					{
// 						OnPanelChange(m_pConfig->GetCurParam()->idPreUI/*idPreSRC*/,0);
// 					}
				#if CVTE_EN_CARPLAY
				if(m_bCarPlayConnected)
				{
					CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
					if(panel)
					{
						if(m_bPhoneCallActive)
						{
							((CPanelCarPlay*)panel)->OnKey(KEY_PHONE_ANSWER);
						}
						//20160429 New carplay do not change to carplay panel.
						/*
						else
						{
							if(m_pConfig->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER))
								SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
							else
								((CPanelCarPlay*)panel)->OnRequestUI("mobilephone:");
						}
						*/
					}
				}
				else
				#endif
				{
					if(m_dwCallStatus==HF_STATE_INCOMING_CALL)
					{
						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_ANSWER,NULL);
					}
//						else if(m_dwCallStatus==HF_STATE_TALKING)
//						{
//							::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
//						}
//						else if(m_dwCallStatus==HF_STATE_OUTGOING_CALL)
//						{
//							::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
//						}
					else if(GET_PANEL_CLASS_ID(m_pCurPanel->GetID())!=UI_CLASS_BT)
					{
						//OnPanelChange(UI_BTCORE,(PAGE_BTCORE_KEYBOARD<<16)+0);
						if(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
						{
							SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN);
						}
						else
						{
							SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_BLUETOOTH);
						}
					}
				}
				PlaySoundEffect(0);
			}
			break;
		case KEY_STW_PHONE_HANGUP:
			if(m_bOpenSRC)
			{
                RETAILMSG(1, (TEXT("OnPanelKey KEY_STW_PHONE_HANGUP m_pSmartBarPhone->GetType()=[%d] m_dwCallStatus[%d] m_bCarPlayConnected[%d]\r\n"),m_pSmartBarPhone->GetType(),m_dwCallStatus,m_bCarPlayConnected));

                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }

//	                if(m_pSmartBarPhone->IsVisible() && m_pSmartBarPhone->GetType()==SB_TALKING)
//					{
//						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
//					}
//					else if(m_pSmartBarPhone->IsVisible() && m_pSmartBarPhone->GetType()==SB_OUTGOING)
//					{
//						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
//					}
//	// 				else if(m_pCurPanel?m_pCurPanel->GetID()==UI_BTCORE:FALSE)
//	// 				{
//	// 					OnPanelChange(m_pConfig->GetCurParam()->idPreUI/*idPreSRC*/,0);
//	// 				}
				#if CVTE_EN_CARPLAY
				if(m_bCarPlayConnected)
				{
                    RETAILMSG(1, (TEXT("OnPanelKey KEY_STW_PHONE_HANGUP m_bPhoneCallActive[%d]\r\n"),m_bPhoneCallActive));
					CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
					if(panel)
					{
						if(m_bPhoneCallActive)
						{
							((CPanelCarPlay*)panel)->OnKey(KEY_PHONE_HANG);
						}
						//20160429 New carplay do not change to carplay panel.
						/*
						else
						{
							if(m_pConfig->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER))
								SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
							else
								((CPanelCarPlay*)panel)->OnRequestUI("mobilephone:");
						}
						*/
					}
				}
				else
				#endif
				{
					if(m_dwCallStatus==HF_STATE_TALKING)
					{
						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
					}
					else if(m_dwCallStatus==HF_STATE_OUTGOING_CALL)
					{
						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
					}
					if(m_dwCallStatus==HF_STATE_INCOMING_CALL)
					{
						::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
					}
                    else if(GET_PANEL_CLASS_ID(m_pCurPanel->GetID())!=UI_CLASS_BT)
					{
						//OnPanelChange(UI_BTCORE,(PAGE_BTCORE_KEYBOARD<<16)+0);
						if(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
						{
							SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN);
						}
						else
						{
							SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_BLUETOOTH);
						}
					}
				}
				PlaySoundEffect(0);
			}
			break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_NAVI:
#endif
        case KEY_STW_NAVI:
		case KEY_MCU_NAVI://
			if(m_bOpenSRC)
			{
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }

				static DWORD lasttick=0;
				if(GetTickCount() - lasttick>500/*2000*/)
				{
					lasttick = GetTickCount();

					if(m_pCurPanel?m_pCurPanel->GetID()==MAKE_PANEL_ID(UI_CLASS_NAVI,0):FALSE)
					{
						//test:
		// 				ShotScreen(_T("\\ResidentFlash\\shot.bmp"));
						OnPanelChange(m_pConfig->GetCurParam()->idCurSRC==MAKE_PANEL_ID(UI_CLASS_NAVI,0)?MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER):m_pConfig->GetCurParam()->idCurSRC,NULL);
					}
					else
					{
						OnPanelChange(MAKE_PANEL_ID(UI_CLASS_NAVI,0),0);
					}
				}
				PlaySoundEffect(0);
			}
			break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_MUTE:
#endif
		case KEY_MCU_MUTE://
		case KEY_STW_VOL_MUTE://STW KEY
			{
            	if(IsPowerOn() /*&& !m_bSpeechOn */&& !m_bPhoneCallActive)
            	{
					if(m_bManualMute)
	                {
	                    OnMuteKey(FALSE);
	                }
	                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
	                {
	                    if(m_bSpeechSignaled && m_bSpeechOn)
	                    {
	                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
	                    }
	                    else
	                    {
	                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
	                    }
	                }
	                else
	                {
	                    OnMuteKey(TRUE);
	                }
					PlaySoundEffect(0);
            	}
			}
			break;

// 		case KEY_MCU_VOLUME_ENTER:
// 			{
// 				::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_SETTINGCORE,(1<<16)+PAGE_SETTINGS_AUDIO);
// 			}
// 			break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_CYCLE_BUTTON_RIGHT:
#endif

		case KEY_STW_NEXT://STW KEY
			if(m_bOpenSRC)
			{
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else
                {
                    UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
    				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));

    				if(panel)
    				{
    					panel->OnKey(KEY_MEDIA_NEXT);
    				}
                }
				PlaySoundEffect(0);
			}
			break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_CYCLE_BUTTON_LEFT:
#endif
		case KEY_STW_PREV://STW KEY
			if(m_bOpenSRC)
			{
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else
                {
    				UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
    				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
    				if(panel)
    				{
    					panel->OnKey(KEY_MEDIA_PREVIOUS);
    				}
                }
				PlaySoundEffect(0);
			}
			break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_KNOB_RIGHT:
#endif
		case KEY_STW_VOL_PLUS://STW KEY
			{
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else
                {
                    if(!m_bMuteOperating)
    				{
    					m_iSpeedCurVolome=m_iVehicleSpeed;

						if(m_bPhoneCallActive)
						{
                            int vol=min(32,m_pConfig->GetCurParam()->dwVolBluetooth+1);
                            m_iSetVolume = vol;
    						m_iVolOriginBluetooth =vol;
                            m_pConfig->GetCurParam()->dwVolBluetooth = vol;
							SetVolume(vol,FALSE,TRUE);
						}
						else if(m_bSpeechOn /*&& !m_bOnSR */&& (IsNaviSpeechEnable() || m_bOnSR))
						{
                            int vol=min(32,m_pConfig->GetCurParam()->dwVolGPS+1);
                            m_pConfig->GetCurParam()->dwVolGPS = vol;
    						m_iVolOriginGPS =vol;
                            SetVolume(vol,FALSE,TRUE,TRUE,TRUE,m_bOnSR);
						}
						else
						{
                            if(m_bSpeechOn)
                            {
                                //SetVolume(VOLUME_DEFAULT_MINI_VALUE,TRUE,TRUE,FALSE);
                            }
                            else
                            {
	                            int vol=min(32,m_pConfig->GetCurParam()->dwVolMedia+1);
	                            m_pConfig->GetCurParam()->dwVolMedia = vol;
	    					    m_iSetVolume = vol;
	    						m_iVolOriginMedia =vol;
                                SetVolume(vol,FALSE,TRUE);    //SetVolume(vol,TRUE,TRUE,FALSE);
                            }
						}
    				}
                }
				PlaySoundEffect(0);
			}
			break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_KNOB_LEFT:
#endif
		case KEY_STW_VOL_MINUS://STW KEY
			{
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else
                {
    				if(!m_bMuteOperating)
    				{
    					m_iSpeedCurVolome=m_iVehicleSpeed;

    					if(m_bPhoneCallActive)
    					{
                            int vol;
                            if(m_pConfig->GetCurParam()->dwVolBluetooth>=1)
                            {
    					        vol=m_pConfig->GetCurParam()->dwVolBluetooth-1;
                            }
                            else
                            {
                                vol=0;
                            }
    					    m_iSetVolume = vol;
    						m_iVolOriginBluetooth =vol;
                            m_pConfig->GetCurParam()->dwVolBluetooth = vol;
                            //SetMainVolume(vol,FALSE,TRUE);
                            SetVolume(vol,FALSE,TRUE);
    					}
    					else if(m_bSpeechOn /*&& !m_bOnSR */&& (IsNaviSpeechEnable() || m_bOnSR))
    					{
                            int vol;
                            if(m_pConfig->GetCurParam()->dwVolGPS>=1)
                            {
    					        vol=m_pConfig->GetCurParam()->dwVolGPS-1;
                            }
                            else
                            {
                                vol=0;
                            }
                            m_pConfig->GetCurParam()->dwVolGPS = vol;
    						m_iVolOriginGPS =vol;
                            //SetMixVolume(vol,FALSE,TRUE);
                            SetVolume(vol,FALSE,TRUE,TRUE,TRUE,m_bOnSR);
    					}
    					else
    					{
                            if(!m_bSpeechOn)
                            {
	                            int vol;
	                            if(m_pConfig->GetCurParam()->dwVolMedia>=1)
	                            {
	    					        vol=m_pConfig->GetCurParam()->dwVolMedia-1;
	                            }
	                            else
	                            {
	                                vol=0;
	                            }
	                            m_pConfig->GetCurParam()->dwVolMedia = vol;
	    					    m_iSetVolume = vol;
	    						m_iVolOriginMedia =vol;
	                            //SetMainVolume(vol,FALSE,TRUE);
	                            SetVolume(vol,FALSE,TRUE);
                           	}
    					}
				    }
				}
				PlaySoundEffect(0);
			}
			break;

			//new keys:
		case KEY_MCU_EQ:
			{
// 				INT idEQ=m_idEQ+1;
// 				if(idEQ>EQ_JAZZ)
// 					idEQ=EQ_NONE;
//
// 				if(m_pCommander)
// 					m_pCommander->SendCmd(_ARM_EQ_KEY,BYTE(idEQ));
			}
			break;
		case KEY_MCU_VOLUME_PREV:
		case KEY_MCU_VOLUME_NEXT:
			{

			}
			break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_KNOB_BUTTON:
#endif
        case KEY_STW_POWER:
            {
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else if(!m_bOnTAMode && !m_bOnReverseMode && !m_bPhoneCallActive && m_idMcuUpdateStatus!=UPDATE_MCU_UPDATING)
                {
                    DWORD dwTick = GetTickCount();
        			if(m_idPowerState==SYS_POWER_ON)
        			{
                        if(dwTick-m_dwPowerStateTick>3000 && !m_bPowerOperating)
                        {
                            RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Power Off ##\r\n")));

        					UINT idui=m_pConfig->GetCurParam()->idCurUI;
        					m_bAutoStartNavi=(idui==MAKE_PANEL_ID(UI_CLASS_NAVI,0));
        					PreparePowerIdle();
                        }
                    }
                    else
                    {
                        if(dwTick-m_dwPowerStateTick>3000 && !m_bPowerOperating)
                        {
                            RETAILMSG(DEBUG_GUI, (TEXT("\r\n## Power On ##\r\n")));
    			            //KillTimer(g_hUIWnd,TIMER_LED_POWER_BREATH);
            				DoPowerOn();
                            PreparePowerOn();
                        }
                    }
                }
				PlaySoundEffect(0);
            }
            break;
        case KEY_STW_SPEECH:
            {
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else
                {
					#if CVTE_EN_CARPLAY
                    if(m_bCarPlayConnected/*  && !m_bSpeechOn && GetTickCount()-m_dwLastSpeechTick>4000*/)
    				{
    					CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
    					if(panel)
    					{
                            RETAILMSG(DEBUG_GUI, (TEXT("MSG:OnRequestSiri \r\n")));
							if(!m_bCarPlaySiriRecognizing && GetTickCount()-m_dwLastSpeechTick>4000)
							{
                            	m_dwActivateSiriTick = GetTickCount();
    							((CPanelCarPlay*)panel)->OnRequestSiri();
							}
							else
							{
								RETAILMSG(DEBUG_GUI, (TEXT("MSG:kAirPlaySiriAction_ButtonUp\r\n")));
								CarplayAPIRequestSiri(kAirPlaySiriAction_ButtonUp);
							}
    					}
    				}
					#endif
                }
				PlaySoundEffect(0);
            }
            break;
        case KEY_STW_SEEK:
            {
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else
                {
                    SendRadioCommand(RADIO_CMD_AMS,1);
                }
				PlaySoundEffect(0);
	        }
            break;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DUCATO)
		case KEY_PHY_FMAM:
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_DEFAULT);
			PlaySoundEffect(0);
			break;
		case KEY_PHY_LUMINACE:
			if(m_pConfig->GetCurParam()->dwBacklight > 50)
				m_pConfig->GetCurParam()->dwBacklight = 30;
			else
				m_pConfig->GetCurParam()->dwBacklight = 100;
			BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);
			OnLCDBrightnessChange();
			PlaySoundEffect(0);
			break;
#endif
		default:
			break;
	}

 	m_dwLastClickTick = GetTickCount();

	//panels process:
// 	if(m_pCurPanel)
// 	{
// 		if(!m_pCurPanel->OnKey(wParam))
// 		{
// 			CPanelGL *panel=m_listPanels.GetItemByID(m_pConfig->GetCurParam()->idCurSRC);
// 			if(panel)
// 				panel->OnKey(wParam);
// 		}
// 	}
}
#endif

BOOL CGUI_Fantasy::IsInstallFile(LPCTSTR lpFileName)
{
	FILE *file;
	INSTALLHEADER				sh;
	memset(&sh,0,sizeof(sh));

	if ( 0!=_wfopen_s(&file, lpFileName,_T("rb")) )
	{
		RETAILMSG(DEBUG_GUI,(_T("file not found.\r\n")));
		return false;
	}

	// read file header
	fread(&sh, 1, sizeof(sh), file);

	fclose(file);

	return sh.tag[0]=='I'	&& sh.tag[1]=='N' && sh.tag[2]=='S' /*&& sh.iCount==88*/;
}
BOOL CGUI_Fantasy::LaunchInstaller(LPCTSTR   lpszFileName,LPCTSTR   lpszInstallPath,CLabelGL *pIcon,CSliderGL *pSlider)
{
	if(!IsInstallFile(lpszFileName))
		return FALSE;

	FILE								*file = NULL;
	// 	int								fileSize = 0;
	// 	unsigned char				*localBuffer;
	// 	unsigned char				*cursor;
	INSTALLHEADER				sh;

	if ( 0!=_wfopen_s(&file, lpszFileName,_T("rb")) )
	{
		RETAILMSG(DEBUG_GUI,(_T("file not found.\r\n")));
		return false;
	}

	// read file header
	fread(&sh, 1, sizeof(sh), file);

	// 	/**** Computer texture data size ****/
	// 	fseek (file , 0 , SEEK_END);
	// 	fileSize = ftell(file);
	// 	fseek(file, 0, SEEK_SET);
	//
	// 	/**** Read data from file ****/
	// 	localBuffer = new unsigned char[fileSize+1];
	// 	fread(localBuffer, fileSize, 1, file);
	// 	localBuffer[fileSize]='\0';//terminate.
	//
	// 	cursor = localBuffer+sizeof(sh);
	//para data to texture:
	int i=sh.dwFileCount;
	int iFileCount=i;
	int iFileNameLen=0;
	WCHAR strFileName[256];
	int iFileSize=0;
	CFileData *pFileData=NULL;
	if(pSlider)
		pSlider->SetRange(0,i);
	while(i>0)
	{
		pFileData = new CFileData;

		fread(&iFileNameLen, 4, 1, file);
		fread(strFileName, iFileNameLen, 1, file);
		strFileName[iFileNameLen/2]='\0';
		pFileData->m_strFileName=strFileName;

		fread(&iFileSize, 4, 1, file);
		pFileData->m_dwDataSize=iFileSize;
		pFileData->m_pdataBuffer = new unsigned char[iFileSize];
		fread(pFileData->m_pdataBuffer, iFileSize, 1, file);


		pFileData->InstallToPath(lpszInstallPath);

		RETAILMSG(DEBUG_GUI,(_T("Install file: %s\r\n"),pFileData->m_strFileName.String()));

		RenderInstallProgress(pIcon,pSlider,iFileCount-i,GetResourceString(IDS_UPDATE_PROMPT_2));
		delete pFileData;
		i--;

		// 		iFileNameLen=GetInt(cursor);
		// 		memcpy(strFileName,cursor,iFileNameLen);
		// 		strFileName[iFileNameLen/2]='\0';
		// 		pFileData->m_strFileName=strFileName;
		// 		cursor+=iFileNameLen;
		// 		iFileSize=GetInt(cursor);
		// 		pFileData->m_dwDataSize=iFileSize;
		// 		pFileData->m_pdataBuffer = new unsigned char[iFileSize];
		// 		memcpy(pFileData->m_pdataBuffer,cursor,iFileSize);
		// 		cursor+=iFileSize;
		// 		pFileData->InstallToPath(lpszInstallPath);
		// 		RenderInstallProgress(pSlider,iFileCount-i,pLable);
		// 		delete pFileData;
		// 		i--;
		Sleep(0);
	}

	// 	delete [] localBuffer;

	fclose(file);

	return TRUE;
}
void CGUI_Fantasy::RenderPoweroffScreen(void)
{
	CLabelGL tagLogo;

	if(!m_pConfig)
		return;

	tagLogo.Initialize(0,NULL,m_pConfig->GetCurParam()->strLogoName2.String());

	glClearColor(0.0f,0.0f,0.0f,0.0f);//

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-g_iScreenWidth*2.0f);

	//tagLogo.SetScale(1.2f,1.2f,1.0f,FALSE);
	tagLogo.Render();

	glPopMatrix();

	SwapBuffers();
}
void CGUI_Fantasy::RenderUpdateScreen(UINT idFlag)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-g_iScreenWidth*2.0f);

	//begin:

	CLabelGL icon_chip;
	icon_chip.Initialize(0,NULL,IDR_TIFF_ICON_CHIP,_T("TIFF"));
	icon_chip.SetShowScale(1.3);
	icon_chip.SetPos(0,100);

	icon_chip.Render();

	COLORGL cr_w={1,1,1,1};
	SIZE si;
	//
	if(m_pTextGL)
	{
		m_pTextGL->SetColor(&cr_w,FALSE);
		m_pTextGL->SetString(GetResourceString(IDS_UPDATE_PROMPT_0+idFlag));
		m_pTextGL->SetCharScale(1.0f,1.0f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(-GLfloat(si.cx/2),-40);
		m_pTextGL->SetAlpha(1.0f);
		m_pTextGL->SetLuminance(1.0f);
		m_pTextGL->Render();
	}

	//end:
	glPopMatrix();
	SwapBuffers();
}

void CGUI_Fantasy::DeviceActivateProc(PVOID context, BOOL IsActivated)
{
	CGUI_Fantasy* pFantasy = (CGUI_Fantasy*)context;
	if (pFantasy != NULL)
	{
		pFantasy->m_bDeviceActivated = IsActivated;
	}
}

void CGUI_Fantasy::DeviceSetMACProc(PVOID context, BOOL IsSetMAC)
{
	CGUI_Fantasy* pFantasy = (CGUI_Fantasy*)context;
	if (pFantasy != NULL)
	{
		pFantasy->m_bIsSetMAC = IsSetMAC;
	}
}

void CGUI_Fantasy::RenderActivateScreen(const wchar_t *name)
{
    RETAILMSG(1,(_T("RenderActivateScreen: name = %s\r\n"),name));
    if(!m_bUIWindowShow)
    {
		ShowWindow(g_hMainWnd, SW_SHOW);
		ShowWindow(g_hUIWnd, SW_SHOW);
		m_bUIWindowShow = TRUE;
    }

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-g_iScreenWidth*2.0f);

	//begin:

	COLORGL cr_w={1,1,1,1};
	SIZE si;
	//
	if(m_pTextGL)
	{
		m_pTextGL->SetColor(&cr_w,FALSE);
		m_pTextGL->SetAlpha(1.0f);
		m_pTextGL->SetLuminance(1.0f);

		m_pTextGL->SetString(name);
		m_pTextGL->SetCharScale(0.8f,0.8f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(-GLfloat(si.cx/2),50);
		m_pTextGL->Render();

		m_pTextGL->SetString(GetResourceString(IDS_ACTIVATE_PROMPT_0));
		m_pTextGL->SetCharScale(1.0f,1.0f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(-GLfloat(si.cx/2),-50);
		m_pTextGL->Render();
	}

	//end:
	glPopMatrix();
	SwapBuffers();
}

void CGUI_Fantasy::RenderSetMACScreen()
{
	glPushMatrix();
    glLoadIdentity();
	glTranslatef(0,0,-g_iScreenWidth*2.0f);

	if(!m_pSetMACPanel)
	{
		OpenPanels(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
		m_pSetMACPanel = m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETMAC,0));
	}

	if(m_pSetMACPanel)
	{
		m_pSetMACPanel->Render();
	}
	
	//end:
	glPopMatrix();
	SwapBuffers();
}

void CGUI_Fantasy::ActivateRenderLogo()
{
#if CVTE_EN_IDLE_LOGO
// 	if(!m_pConfig)
// 		return;

	if(m_bLogoShade)
		return;

	//enable dual logo here
#if 0
	m_idLogoShowStyle = 2;
#else
	if(m_bManualPowerOn)
		m_idLogoShowStyle = 0;
	else
		m_idLogoShowStyle = 1;
#endif
	RETAILMSG(1,(_T("CGUI_Fantasy::ActivateRenderLogo: #@# m_idLogoShowStyle:%d\r\n"),m_idLogoShowStyle));

	if(m_idLogoShowStyle == 0 || m_idLogoShowStyle == 1)//only logo1 or logo2
	{
		m_logoAlpha = 1.0f;
		m_logo2Alpha = 1.0f;
	}
	else//both logo
	{
		m_logoAlpha = 0.01f;
		m_logo2Alpha = 0.0f;
	}
	m_bLogoShade = TRUE;
	m_dwLogoShadeTick = GetTickCount();
// 	tagLogo.Initialize(0,NULL,_T("\\Media Card\\bootlogo_ICARTECH.jpg"),g_iScreenWidth,g_iScreenHeight);
// 	VIEW_STATE vs=tagLogo.GetCurViewState();
// 	vs.fRotateX=-180;
// 	tagLogo.SetIniViewState(&vs);
	//tagLogo.SetScale(1.172f,1.172f,1.0f,FALSE);

	/*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-g_iScreenWidth*2.0f);

	//m_tagLogo.Render();

	glPopMatrix();

	SwapBuffers();
	*/

	SetFrameTime(10);
#endif

}

void CGUI_Fantasy::RenderIniBackground(void)
{
	RETAILMSG(DEBUG_GUI, (_T("MSG: [CGUI_Fantasy::RenderIniBackground] \r\n")));
	RenderWorld();

	if(m_pSkinManager->GetConfig()->GetCurParam()->bShowWallpaper)
	{
		SwapBuffers();
	}
}



void CGUI_Fantasy::RenderInstallProgress(CLabelGL *pIcon,CSliderGL *pSlider,INT iProgress,const wchar_t *pStrTitle)
{
	RETAILMSG(DEBUG_GUI, (_T("MSG: [CGUI_Fantasy::RenderInstallProgress] \r\n")));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-g_iScreenWidth*2.0f);

	//tagLogo.SetScale(1.5f,1.6f,1.6f,FALSE);

	if(pIcon)
	{
		pIcon->Render();
	}
	//draw slider:
	if(pSlider)
	{
		pSlider->SetValue(iProgress);
		pSlider->Render();
	}
	//draw note:
	COLORGL cr_w={1,1,1,1};
	SIZE si;
	if(m_pTextGL && pStrTitle)
	{
		m_pTextGL->SetColor(&cr_w,FALSE);
		m_pTextGL->SetString(pStrTitle);
		m_pTextGL->SetCharScale(1.0f,1.0f);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(-GLfloat(si.cx/2),-40);
		m_pTextGL->SetAlpha(1.0f);
		m_pTextGL->SetLuminance(1.0f);
		m_pTextGL->Render();
	}

	glPopMatrix();

	SwapBuffers();

}



BOOL CGUI_Fantasy::EnterInstallProgress(CLabelGL *pIcon,CSliderGL *pSlider)
{
	while(!IsFileExists(g_strInstallFile))
	{
		/*
		//refresh device:
		TCHAR szFind[MAX_PATH];
		LPCTSTR lpszPath=_T("\\");
		wcscpy_s(szFind,MAX_PATH,lpszPath); //
		wcscat_s(szFind,MAX_PATH,_T("*.*")); //
		WIN32_FIND_DATA wfd;
		HANDLE hFind=FindFirstFile(szFind,& wfd);

		if(hFind!=INVALID_HANDLE_VALUE) //
		{
			do
			{
				if(wfd.cFileName[0]=='.')
					continue; //
				if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
					!(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
					!(wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
				{
					RETAILMSG(DEBUG_GUI,(_T(">>>>Get device:%s\r\n"),(wfd.cFileName)));
				}
			}
			while(FindNextFile(hFind,&wfd));
			FindClose(hFind);
		}
		*/

		//render:
		RenderInstallProgress(pIcon,NULL,0,GetResourceString(IDS_UPDATE_PROMPT_1));

		RETAILMSG(DEBUG_GUI,(_T("....wait for USB Disk ready....\r\n")));

		WaitForSingleObject(m_hEventSD,2000);
	}

	RenderInstallProgress(pIcon,pSlider,0,GetResourceString(IDS_UPDATE_PROMPT_2));

	return LaunchInstaller(g_strInstallFile,g_strInstallPath,pIcon,pSlider);

}
void CGUI_Fantasy::PrepareForFirstTime()
{

}
BOOL CGUI_Fantasy::InstallFiles()
{
	//install user files:
	if(!IsPathExists(g_strInstallPath) ||  IsFileExists(g_strInstallFile) )
	{
		DWORD tick=GetTickCount();

		glClearColor(0.0f,0.0f,0.0f,1.0f);//

		CSliderGL sliInstallProgress;

		SLIDERSTRUCTURE sli;
		memset(&sli,0,sizeof(sli));
		sli.size.cx=g_iClientWidth-280;//520/*776*/;
		sli.size.cy=32;
		sli.bHasBtns=FALSE;
		sli.bFilled=TRUE;
		sli.bFilledBoth=FALSE;
// 		sli.idBackTex=TEXID_052;
// 		sli.idChannelTex=TEXID_052;
// 		sli.idThumbTex=TEXID_050;

		sliInstallProgress.Initialize(0,NULL,&sli,NULL);
		sliInstallProgress.SetPos(0.0f,-130.0f);
		sliInstallProgress.SetRange(0,500);
		sliInstallProgress.SetValue(0);
		sliInstallProgress.SetAutoBackFill(TRUE);
		sliInstallProgress.ShowThumb(FALSE);

		CLabelGL icon_chip;
		icon_chip.Initialize(0,NULL,IDR_TIFF_ICON_CHIP,_T("TIFF"));
		icon_chip.SetShowScale(1.3);
		icon_chip.SetPos(0,100);

		//first time to run,install files:
		BOOL bRet = EnterInstallProgress(&icon_chip,&sliInstallProgress);

		//show install success massage box:

		//glClearColor(45.0f/256.0f,62.0f/256.0f,80.0f/256.0f,1.0f);//

		RETAILMSG(DEBUG_GUI,(_T("install files, takes %d ms\r\n"),GetTickCount()-tick));

		return bRet;
	}
	else
	{
		return TRUE;
	}
}
// void CGUI_Fantasy::CopyFiles(LPCTSTR lpszDesPath,LPCTSTR lpszSrcPath,BOOL bShowProgress,CLabelGL *pIcon,CSliderGL *pSlider,CLabelGL *pLable)
// {
// 	TCHAR szFind[MAX_PATH];
// 	wcscpy_s(szFind,MAX_PATH,lpszSrcPath); //
// 	wcscat_s(szFind,MAX_PATH,_T("*.*")); //
//
//
// 	WIN32_FIND_DATA wfd;
// 	HANDLE hFind;
//
// 	TCHAR szwFile[MAX_PATH];
// 	TCHAR szCurSrcPath[MAX_PATH];
// 	TCHAR szCurDesPath[MAX_PATH];
//
// 	if (!IsPathExists(lpszDesPath))        //
// 	{
// 		if(!CreateDirectory(lpszDesPath,NULL))
// 			return ;
// 	}
//
// 	hFind=FindFirstFile(szFind,& wfd);
//
//
// 	wcscpy_s(szCurSrcPath,MAX_PATH,lpszSrcPath);
// 	wcscpy_s(szCurDesPath,MAX_PATH,lpszDesPath);
//
// 	if(hFind==INVALID_HANDLE_VALUE) //
// 		return;
//
// 	do
// 	{
// 		if(wfd.cFileName[0]=='.')
// 			continue; //
// 		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 		{
// 			//swprintf_s(szwFile,MAX_PATH,_T("%s"),wfd.cFileName);
//
// 			TCHAR szwInstallFile[MAX_PATH];
//
// 			wcscpy_s(szwFile,MAX_PATH,szCurSrcPath);
// 			wcscat_s(szwFile,MAX_PATH,wfd.cFileName);
// 			wcscat_s(szwFile,MAX_PATH,_T("\\"));
//
// 			wcscpy_s(szwInstallFile,MAX_PATH,szCurDesPath);
// 			wcscat_s(szwInstallFile,MAX_PATH,wfd.cFileName);
// 			wcscat_s(szwInstallFile,MAX_PATH,_T("\\"));
//
// 			CopyFiles(szwInstallFile,szwFile,bShowProgress,pIcon,pSlider,pLable); //
//
// 		}
// 		else
// 		{
// 			static int i=0;
// 			swprintf_s(szwFile,MAX_PATH,_T("%s"),wfd.cFileName);
//
// 			TCHAR szwTemp[MAX_PATH];
// 			TCHAR szwFileName[MAX_PATH];
// 			wcscpy_s(szwFileName,MAX_PATH,szCurSrcPath);
// 			wcscat_s(szwFileName,MAX_PATH,szwFile);
//
// 			wcscpy_s(szwTemp,MAX_PATH,szCurDesPath);
// 			wcscat_s(szwTemp,MAX_PATH,szwFile);
//
// 			if(bShowProgress)
// 				RenderInstallProgress(pIcon,pSlider,i++,pLable);
//
// 			CopyFile(szwFileName,szwTemp,  0);
// 		}
// 	}
// 	while(FindNextFile(hFind,&wfd));
// 	FindClose(hFind); //
//
// }
//
BOOL   CGUI_Fantasy::IsPathExists(LPCTSTR   lpszPath)
{
	TCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA   wfd;
	BOOL   bRet;
	HANDLE   hFind;

	wcscpy_s(szFind,MAX_PATH,lpszPath); //windows API 用lstrcpy，不是strcpy
	wcscat_s(szFind,MAX_PATH,_T("*.*")); //找所有文件

	hFind   =   FindFirstFile(szFind,   &wfd);
	bRet   =   hFind   !=   INVALID_HANDLE_VALUE;
	FindClose(hFind);
	return   bRet;
}
BOOL   CGUI_Fantasy::IsFileExists(LPCTSTR   lpszFileName)
{
	//
	WIN32_FIND_DATA   wfd;
	HANDLE   hFind;
	BOOL   bRet;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   (hFind   !=   INVALID_HANDLE_VALUE);
	FindClose(hFind);
	return   bRet;
}
void CALLBACK CGUI_Fantasy::CommandProc(LPBYTE pCmd,DWORD dwCmdLen,LPVOID lparam)
{
	CGUI_Fantasy *pGUI=(CGUI_Fantasy*)lparam;
}

void CGUI_Fantasy::OnRasDialEvent(WPARAM wParam, LPARAM lParam)
{
	/*
	RASCONNSTATE RasState = (RASCONNSTATE)wParam;
	switch(RasState)
	{
	case RASCS_OpenPort:
		if(m_pStatusBar)
			m_pStatusBar->SetDialupStatus(DIALUP_CONNECTING);
		break;
	case  RASCS_Connected:
		if(m_pStatusBar)
			m_pStatusBar->SetDialupStatus(DIALUP_CONNECTED);
		break;
	case  RASCS_Disconnected:
		if(m_pStatusBar)
			m_pStatusBar->SetDialupStatus(DIALUP_DISCONNECTED);
		break;
	}

	//pass to settings panel:
	CPanelGL *p = m_listPanels.GetItemByID(UI_SETTINGCORE);
	if(p)
	{
		 ((CPanelSettings*)p)->OnRasDialEvent(wParam,lParam);
	}
*/
}
void CGUI_Fantasy::PrePowerOff()
{
	if (!m_bSystemReady && m_bWaitingForPowerOFF)
		return;
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::PrePowerOff] ...\r\n")));


	if(m_pVideoWindow)
	{
		m_pVideoWindow->StopVideo();
	}
	if(m_pSmartBarVol)
	{
		m_pSmartBarVol->Show(FALSE);
	}


	m_pSkinManager->GetGUI()->EnableSwap(TRUE);
	SetForegroundWindow(g_hMainWnd);

	//::SetWindowPos(g_hUIWnd, HWND_TOPMOST , 0, 0, g_iScreenWidth, g_iScreenHeight,  0 );

/*
	//send command to mcu to save time:
	if(m_pCommander)
	{
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		BYTE bYear=sys_time.wYear-2000;
		BYTE bMonth=sys_time.wMonth;
		BYTE bDay=sys_time.wDay;
		BYTE bHour=sys_time.wHour;
		BYTE bMinute=sys_time.wMinute;
		BYTE bSecond=sys_time.wSecond;

		RETAILMSG(DEBUG_GUI,(_T(".......set MCU time %d-%02d-%02d, %02d:%02d:%02d\r\n"),sys_time.wYear,sys_time.wMonth,sys_time.wDay,
			sys_time.wHour,sys_time.wMinute,sys_time.wSecond));

		m_pCommander->SendCmd(_ARM_TIME_YEAR,bYear);
		m_pCommander->SendCmd(_ARM_TIME_MONTH,bMonth);
		m_pCommander->SendCmd(_ARM_TIME_DAY,bDay);

		m_pCommander->SendCmd(_ARM_TIME_HOUR,bHour);
		m_pCommander->SendCmd(_ARM_TIME_MINUS,bMinute);
		m_pCommander->SendCmd(_ARM_TIME_SECOND,bSecond);

	}
*/

	m_bSystemReady =FALSE;
	m_bWaitingForPowerOFF=TRUE;


	//RenderPoweroffScreen();

	//save config:

	if(m_pConfig)
	{
		RETAILMSG(DEBUG_GUI,(_T("\r\n...SaveConfig...\r\n")));
	
		//volumes:
		int vol_t;
//			int vol_t=m_pConfig->GetCurParam()->dwVolBluetooth;
//			if(vol_t<5)vol_t=5;if(vol_t>20)vol_t=20;
//			m_pConfig->GetCurParam()->dwVolBluetooth=vol_t;
//
//			vol_t=m_pConfig->GetCurParam()->dwVolGPS;
//			if(vol_t<5)vol_t=5;if(vol_t>20)vol_t=20;
//			m_pConfig->GetCurParam()->dwVolGPS=vol_t;

		vol_t=m_pConfig->GetCurParam()->dwVolMedia;
		if(vol_t<5)vol_t=5;if(vol_t>20)vol_t=20;
		m_pConfig->GetCurParam()->dwVolMedia=vol_t;

		//
		UINT idui=m_pConfig->GetCurParam()->idCurUI;
		UINT idpreui=m_pConfig->GetCurParam()->idPreUI;
		m_pConfig->GetCurParam()->bAutoStartNavi=(idpreui==MAKE_PANEL_ID(UI_CLASS_NAVI,0));

#if 0
		//save current source:
		UINT idclass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)		
		if(/*idclass == UI_CLASS_BT || */idclass == UI_CLASS_MHL ||
			/*idclass == UI_CLASS_CARPLAY || idclass == UI_CLASS_IPOD ||*/ //BUG 0002765 iPod play->Turn off ACC->Turn on ACC->Do not switch to iPod panel.
			idclass == UI_CLASS_PHONELINK ||
			idclass == UI_CLASS_EASYCONNECTED)
		{
			m_pConfig->GetCurParam()->idCurSRC = MAKE_PANEL_ID(UI_CLASS_RADIO,0);
		}
#else
		if(idclass == UI_CLASS_BT || idclass == UI_CLASS_MHL ||
			/*idclass == UI_CLASS_CARPLAY || idclass == UI_CLASS_IPOD ||*/ //BUG 0002765 iPod play->Turn off ACC->Turn on ACC->Do not switch to iPod panel.
			idclass == UI_CLASS_PHONELINK ||
			idclass == UI_CLASS_EASYCONNECTED)
		{
			m_pConfig->GetCurParam()->idCurSRC = MAKE_PANEL_ID(UI_CLASS_RADIO,0);
		}
#endif
#endif

		m_pConfig->GetCurParam()->iPowerStatusBeforeACCOff = (m_idPowerState == SYS_POWER_IDLE)?1:0;

		if(m_pConfig->GetCurParam()->dwBacklight <= 22)
			m_pConfig->GetCurParam()->dwBacklight = 22;
		m_pConfig->SaveAppConfig();
		Sleep(500);
	}

	m_pCurPanel = NULL;

}
void CGUI_Fantasy::OnSystemPrePowerState(UINT idState)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnSystemPrePowerState]: 0x%x\r\n"),idState));

	if(m_idPowerState != idState && idState!=SYS_POWER_NULL)
	{
		m_listPanels.Head();
		while (!m_listPanels.End())
		{
			m_listPanels.Get()->OnSystemPowerStatePrepare(idState);
			m_listPanels.Next();
		}

		switch (idState)
		{
		case SYS_POWER_OFF:
			{
#if CVTE_EN_LED
				//led down:
				if(m_pLED)
				{
					RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(0) 3\r\n"), TEXT(__FUNCTION__)));

					m_pLED->SetLedState(LED_POWEROFF);
					OnPanelLightChange();
				}
#endif

				PrePowerOff();
				//mute:
				Mute(TRUE,FALSE);    //Mute(TRUE,TRUE);

			}
			break;
		case SYS_POWER_ON:
			{
				//turn on back light:
				ScreenOn();
//					m_pConfig->GetCurParam()->dwBacklight=max(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight);
//					BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
//					m_bBacklightOFF=FALSE;
			}
			break;
		case SYS_POWER_SOFT_RESET:
		case SYS_POWER_HARD_RESET:
			{
				//mute:
				Mute(TRUE,FALSE);//Mute(TRUE,TRUE);
			}
			break;
		case SYS_POWER_IDLE:
			{
				//mute:
				Mute(TRUE,FALSE);//Mute(TRUE,TRUE);
			}
			break;
		}

	}
}
void CGUI_Fantasy::OnSystemPowerState(UINT idState)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnSystemPowerState]: 0x%x\r\n"),idState));

	if(m_idPowerState != idState && idState!=SYS_POWER_NULL)
	{
		m_dwPowerStateTick = GetTickCount();

		if( idState == SYS_POWER_ON)
		{
			PowerOn();

			m_listPanels.Head();
			while (!m_listPanels.End())
			{
				m_listPanels.Get()->OnSystemPowerStateDone(idState);
				m_listPanels.Next();
			}

			#if CVTE_EN_CARPLAY
			//notify carplay
			CCarplayAdapter::GetInstance()->SetSystemPowerState(idState);
			#endif
		}
		else
		{
			m_listPanels.Head();
			while (!m_listPanels.End())
			{
				m_listPanels.Get()->OnSystemPowerStateDone(idState);
				m_listPanels.Next();
			}

			#if CVTE_EN_CARPLAY
			//notify carplay
			CCarplayAdapter::GetInstance()->SetSystemPowerState(idState);
			#endif

			switch (idState)
			{
			case SYS_POWER_OFF:
				PowerOff();
				break;
			case SYS_POWER_SOFT_RESET:
				PowerSoftReset();
				break;
			case SYS_POWER_HARD_RESET:
				PowerHardReset();
				break;
			case SYS_POWER_IDLE:
				if(!m_bReverseSignaled)
				{
					PowerIdle();
				}
				break;
			}
		}
	}

#if CVTE_EN_IDLE_LOGO
	m_bLogoShade = FALSE;
#endif
	m_bPowerOperating = FALSE;
	RETAILMSG(1,(_T("MSG: Power operating Done.\r\n")));
	SetTimer(g_hUIWnd,TIMER_SYSTEM_POWER_OPERATION_READY,1500,NULL);
}
void CGUI_Fantasy::OnSystemPowerOperationReady()
{
	RETAILMSG(1,(_T("MSG: OnSystemPowerOperationReady\r\n")));
	KillTimer(g_hUIWnd, TIMER_SYSTEM_POWER_OPERATION_READY);
	//check phone call command list:
	if(m_listPhoneCallCmd.size())
	{
		for (int i=0; i<m_listPhoneCallCmd.size(); i++)
		{
			ProcessPhoneCallState(m_listPhoneCallCmd.at(i).wParam, m_listPhoneCallCmd.at(i).lParam);
		}
		m_listPhoneCallCmd.clear();
	}
}
BOOL  CGUI_Fantasy::IsPowerOn()
{
	return 	m_idPowerState == SYS_POWER_ON;
}
BOOL  CGUI_Fantasy::IsPowerIdle()
{
	return 	m_idPowerState == SYS_POWER_IDLE;
}
void CGUI_Fantasy::DoPowerIdle()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::DoPowerIdle]\r\n")));

	m_idCurAudioSRC=SRC_AUDIO_INVALID;
	m_idCurMediaClassSRC = 0;

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_POWER_KEY,SYS_POWER_IDLE);
	}
}
void CGUI_Fantasy::DoPowerOff()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::DoPowerOff]\r\n")));

	m_idCurAudioSRC=SRC_AUDIO_INVALID;
	m_idCurMediaClassSRC = 0;

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_POWER_KEY,SYS_POWER_OFF);
	}
}
void CGUI_Fantasy::DoPowerOn()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::DoPowerOn]\r\n")));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_POWER_KEY,SYS_POWER_ON);
	}
}
void CGUI_Fantasy::DoPowerHardReset()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::DoPowerHardReset]\r\n")));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_POWER_KEY,SYS_POWER_HARD_RESET);
	}
}
void CGUI_Fantasy::DoPowerSoftReset()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::DoPowerSoftReset]\r\n")));

	m_idCurAudioSRC=SRC_AUDIO_INVALID;
	m_idCurMediaClassSRC = 0;

	BKL_SetBackLight(BACKLIGHT_STATE_OFF,TRUE);
	m_bBacklightOFF=TRUE;

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_POWER_KEY,SYS_POWER_SOFT_RESET);
	}
}
void CGUI_Fantasy::PreparePowerIdle()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::PreparePowerIdle] m_bOpenSRC[%d]\r\n"),m_bOpenSRC));

	if(GetTickCount()-m_dwCarPlayArrivedTick<4000)
		return;

	m_bManualPowerOn = FALSE;

	if(m_bManualMute)
	{
		OnMuteKey(FALSE);
	}

	if (m_bOpenSRC)
	{
		m_bPowerOperating = TRUE;
		RETAILMSG(1,(_T("MSG: Power IDLE operating Start.\r\n")));

        m_idUIBeforeIdle = m_pConfig->GetCurParam()->idCurUI;

		SwitchPage(UI_CLASS_POWERSTATE,0,PAGE_SWITCH_ANI_NONE,SYS_POWER_IDLE);
		//OnSystemPrePowerState(SYS_POWER_IDLE);
		//Sleep(1000);
		//OnSystemPowerState(SYS_POWER_IDLE);

	}
	else
	{
		PowerIdle();
	}
}
void CGUI_Fantasy::PreparePowerOn()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::PreparePowerOn] m_bOpenSRC[%d]\r\n"),m_bOpenSRC));

	m_bManualPowerOn = TRUE;

	#if CVTE_EN_LED
	//on:
	if(m_pLED)
	{
		m_pLED->SetLedState(LED_POWERON);
	}
	#endif

	if(m_bOpenSRC)
	{
		m_bPowerOperating = TRUE;
		RETAILMSG(1,(_T("MSG: Power ON operating Start.\r\n")));

		//screen on:
		if(m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen == IDLE_SCREEN_NONE)
		{
			ScreenPowerOn();
		}
		//RETAILMSG(1, (TEXT("MSG:%s: SwitchPage [UI_CLASS_POWERSTATE] SYS_POWER_ON \r\n"), TEXT(__FUNCTION__)));
		SwitchPage(UI_CLASS_POWERSTATE,0,PAGE_SWITCH_ANI_NONE,SYS_POWER_ON);
		//OnSystemPrePowerState(SYS_POWER_ON);
		//Sleep(1000);
		//OnSystemPowerState(SYS_POWER_ON);
	}
	else
	{
		Sleep(500);
		//PowerOn();
		OnSystemPrePowerState(SYS_POWER_ON);
		OnSystemPowerState(SYS_POWER_ON);
        Sleep(100);
	}

// 	//turn on back light:
// 	m_pConfig->GetCurParam()->dwBacklight=max(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight);
// 	BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
// 	m_bBacklightOFF=FALSE;

}

void CGUI_Fantasy::ScreenOff()
{
    m_dwScreenStateTick = GetTickCount();
    m_idScreenState = SYS_SCREEN_OFF;
	BKL_SetBackLight(0,TRUE);
	m_bBacklightOFF=TRUE;
}

void CGUI_Fantasy::ScreenOn()
{
    m_dwScreenStateTick = GetTickCount();
    m_idScreenState = SYS_SCREEN_ON;
    if(m_bBacklightOFF)
    {
		//RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::ScreenOn]: m_iScrnBrightness:%d dwBacklight:%d\r\n"),m_iScrnBrightness,m_pConfig->GetCurParam()->dwBacklight));

		switch (m_iScrnBrightness)
		{
		case 1:
			m_pConfig->GetCurParam()->dwBacklight = min(CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END,m_pConfig->GetCurParam()->iLCDNightModeBrightness);//min(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->iLCDNightModeBrightness);
			BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight);	//BKL_SetBackLight(min(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight));
			m_bBacklightOFF=FALSE;
			break;
		case 2:
		default:
			m_pConfig->GetCurParam()->dwBacklight = max(CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START,m_pConfig->GetCurParam()->iLCDDayModeBrightness);//max(BACKLIGHT_STATE_HALFON,m_pConfig->GetCurParam()->dwBacklight);
			BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
			m_bBacklightOFF=FALSE;
			break;
		}
    }
}

void CGUI_Fantasy::ScreenIdle()
{
    m_dwScreenStateTick = GetTickCount();
    m_idScreenState = SYS_SCREEN_IDLE;
    BKL_SetBackLight(10);
    m_bBacklightOFF = FALSE;
}


void CGUI_Fantasy::PowerIdle()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::PowerIdle]\r\n")));

	m_idPowerState = SYS_POWER_IDLE;

	m_dwPowerStateTick = GetTickCount();

	#if CVTE_EN_LED
	if(m_pLED)
	{
        RETAILMSG(DEBUG_GUI, (TEXT("MSG:[%s] SetLedParam(0) 3\r\n"), TEXT(__FUNCTION__)));

		m_pLED->SetLedParam(0,0);
		m_pLED->SetLedState(LED_POWERIDLE);
		#if (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U621P)
        m_pLED->SetLedParam(1,0);
		#endif
		OnPanelLightChange();
	}
	#endif

	//navi speech control:
	if(m_bSpeechOn)
	{
		OnSpeechTerminate();
	}

	//turn off back light://///////////////////////////////////////////////
	if(!m_bOnReverseMode)
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen == IDLE_SCREEN_NONE)//no idle screen
		{
			BKL_SetBackLight(BACKLIGHT_STATE_OFF,TRUE);
			m_bBacklightOFF=TRUE;

			Sleep(50);

			ScreenPowerOff();
			//
			//SwitchPage(UI_CLASS_IDLE,0,PAGE_SWITCH_ANI_NONE,m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen);
		}
		else
		{
			//turn on back light to half:
			BKL_SetBackLight(min(IDLE_SCREEN_BACKLIGHT,m_pConfig->GetCurParam()->dwBacklight),TRUE);
			m_bBacklightOFF=FALSE;

			SwitchPage(UI_CLASS_IDLE,0,PAGE_SWITCH_ANI_NONE,m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen);

		}
	}
	else
	{
		//turn on back light:
		BKL_SetBackLight(m_pConfig->GetCurParam()->dwBacklight,TRUE);
		m_bBacklightOFF=FALSE;
	}
	//////////////////////////////////////////////////////////

 	//mute:
	//Mute(TRUE,TRUE);


	//GPS:
	//GPSPowerOff();


// 	//screen:
// 	if(m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen == IDLE_SCREEN_NONE)
// 	{
// 		ScreenPowerOff();
// 	}

	//
	DoPowerIdle();

// 	//refresh UI:
// 	UpdateFrame();

// 	DWORD dwErr;
// 	dwErr = SetSystemPowerState(NULL, POWER_STATE_IDLE, POWER_FORCE);
// 	if ( dwErr )
// 	{
// 		RETAILMSG(DEBUG_GUI,( (L"SetSystemPowerState POWER_STATE_IDLE Error: %d\r\n"), dwErr ));
// 	}
}
void CGUI_Fantasy::PowerOn()
{
	RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::PowerOn] m_bOpenSRC:%d m_bManualPowerOn:%d\r\n"),m_bOpenSRC,m_bManualPowerOn));

	m_idPowerState = SYS_POWER_ON;

	m_dwPowerStateTick = GetTickCount();

#if CVTE_EN_ACCON_WARNING
	if(m_bManualPowerOn)
	{
		CWarningProxy::GetInstance()->StartShowWarningBar(5000);
	}
#endif

	//DoPowerOn();
	//Sleep(800);

    //RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::PowerOn]: Sleep 3s \r\n")));

	//RETAILMSG(1, (TEXT("MSG:[%s]: iLCDDayModeBrightness[%d] \r\n"), TEXT(__FUNCTION__),m_pConfig->GetCurParam()->iLCDDayModeBrightness));
	//RETAILMSG(1, (TEXT("MSG:[%s]: iLCDNightModeBrightness[%d] \r\n"), TEXT(__FUNCTION__),m_pConfig->GetCurParam()->iLCDNightModeBrightness));
	//RETAILMSG(1, (TEXT("MSG:[%s]: iLEDDayModeBrightness[%d] \r\n"), TEXT(__FUNCTION__),m_pConfig->GetCurParam()->iLEDDayModeBrightness));
	//RETAILMSG(1, (TEXT("MSG:[%s]: iLEDNightModeBrightness[%d] \r\n"), TEXT(__FUNCTION__),m_pConfig->GetCurParam()->iLEDNightModeBrightness));
	//RETAILMSG(1, (TEXT("MSG:[%s]: dwBacklight[%d] \r\n"), TEXT(__FUNCTION__),m_pConfig->GetCurParam()->dwBacklight));

	//DayMode & NightMode param
	if(m_pConfig->GetCurParam()->iLCDDayModeBrightness == 0)
		m_pConfig->GetCurParam()->iLCDDayModeBrightness = CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_DEFAULT;
	else if(m_pConfig->GetCurParam()->iLCDDayModeBrightness < CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START)
		m_pConfig->GetCurParam()->iLCDDayModeBrightness = CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START;
	else if(m_pConfig->GetCurParam()->iLCDDayModeBrightness > CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END)
		m_pConfig->GetCurParam()->iLCDDayModeBrightness = CVTE_DEF_LCD_BRIGHTNESS_MAX;

	if(m_pConfig->GetCurParam()->iLCDNightModeBrightness  == 0)
		m_pConfig->GetCurParam()->iLCDNightModeBrightness = CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_DEFAULT;
	else if(m_pConfig->GetCurParam()->iLCDNightModeBrightness < CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START)
		m_pConfig->GetCurParam()->iLCDNightModeBrightness = CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START;
	else if(m_pConfig->GetCurParam()->iLCDNightModeBrightness > CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END)
		m_pConfig->GetCurParam()->iLCDNightModeBrightness = CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END;

	if(m_pConfig->GetCurParam()->iLEDDayModeBrightness == 0)
		m_pConfig->GetCurParam()->iLEDDayModeBrightness = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_DEFAULT;
	else if(m_pConfig->GetCurParam()->iLEDDayModeBrightness < CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START)
		m_pConfig->GetCurParam()->iLEDDayModeBrightness = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START;
	else if(m_pConfig->GetCurParam()->iLEDDayModeBrightness > CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END)
		m_pConfig->GetCurParam()->iLEDDayModeBrightness = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END;

	if(m_pConfig->GetCurParam()->iLEDNightModeBrightness == 0)
		m_pConfig->GetCurParam()->iLEDNightModeBrightness = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_DEFAULT;
	else if(m_pConfig->GetCurParam()->iLEDNightModeBrightness < CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START)
		m_pConfig->GetCurParam()->iLEDNightModeBrightness = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START;
	else if(m_pConfig->GetCurParam()->iLEDNightModeBrightness > CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END)
		m_pConfig->GetCurParam()->iLEDNightModeBrightness = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END;

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)	
	if (m_pAudioCodec)
	{
		if (!m_pAudioCodec->IsCodecIni())
		{
			RETAILMSG(1,(_T("MSG:[%s]: => IniCodec \r\n"),TEXT(__FUNCTION__)));
			m_pAudioCodec->IniCodec();
			m_pAudioCodec->MuteAMP(TRUE);
			m_pAudioCodec->Mute(TRUE);
			m_pAudioCodec->SetMainVolume(0);
			m_pAudioCodec->SetAltVolume(m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS);
			m_pAudioCodec->SetLimitVolume(m_bOnReverseMode,REARVIEW_VOLUME_DEFAULT);
			Sleep(10);
		}
		if (m_bSpeechOn)
		{
			m_pAudioCodec->SetAltVolume(m_pConfig->GetCurParam()->dwVolGPS);
			m_pAudioCodec->SetMixerOn(TRUE);
		}
		else if (m_bPhoneCallActive)
		{
			m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolBluetooth);
			m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,1000);
		}
		else
		{
			m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolMedia);
			m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,1000);
		}
	}
#endif

 	if(m_bOpenSRC && m_bManualPowerOn/*&& m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen != 0*/)//with idle screen
	{
		/*
		SwitchPage(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),
			GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC),
			PAGE_SWITCH_ANI_NONE,0);

		//
		if(m_bAutoStartNavi)
		{
			SetTimer(g_hUIWnd,TIMER_AUTO_START_NAVI,2000,NULL);
		}
		*/
		if(m_bCarPlayConnected && GetTickCount()-m_dwCarPlayArrivedTick<3000)
		{
			RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::PowerOn]: Auto Start carplay \r\n")));
			SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);		//20160428
			SwitchMediaClassSRC(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
			SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
		}
		else if(m_bAutoStartNavi)
		{
			RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::PowerOn]: Auto Start Navi \r\n")));
#if 0
			SetTimer(g_hUIWnd,TIMER_AUTO_START_NAVI,4000,NULL);
			//SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN,PAGE_SWITCH_ANI_NONE,0);
			//media auto resume:
			//OnMediaKey(KEY_MEDIA_PLAY);
#else
#if CVTE_EN_NAVI
			//Bug: <0002860> 导航播报提示音时，切源，导航提示音停止后，该源的声音很小，基本听不见
			SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN,PAGE_SWITCH_ANI_NONE,0);
#endif
			//resume playing:
			if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC)!=UI_CLASS_CARPLAY)
			{
				#if CVTE_EN_KEY_PROXY
				CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
				CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
				#else
				OnMediaKey(KEY_MEDIA_PLAY);
				#endif
			}
			else
			{
				SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
			}
#endif
		}
		else
		{
			UINT idCurSRC=m_pConfig->GetCurParam()->idCurSRC;
			RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::PowerOn]: idCurSRC=0x%x \r\n"),idCurSRC));

// 			if(GET_PANEL_CLASS_ID(idCurSRC)==UI_CLASS_CARPLAY || GET_PANEL_CLASS_ID(idCurSRC)==UI_CLASS_IPOD)
// 			{
// 				SwitchPage(UI_CLASS_RADIO,
// 					PAGE_RADIO_MAIN,
// 					PAGE_SWITCH_ANI_NONE,0);
// 			}
			if(GET_PANEL_CLASS_ID(idCurSRC)==UI_CLASS_IPOD)
			{//  [BUG] 0002560 playing ipod ,enter to poweroff status,and cancel poweroff,machine play Radio  fix by Franco20160520
				if (m_biPodConnected)
				{
					SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_NONE,0);
					SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
					#if CVTE_EN_KEY_PROXY
					CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
					CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
					#else
					OnMediaKey(KEY_MEDIA_PLAY);
					#endif
				}
				else
				{
					SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE,0);
				}


			}
			else if(GET_PANEL_CLASS_ID(idCurSRC)==UI_CLASS_CARPLAY)
			{
				if(m_bCarPlayConnected)
				{
					SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
					SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				}
				else
				{
					SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_NONE);
				}
			}
			else if(m_bCarPlayConnected && GET_PANEL_CLASS_ID(m_idUIBeforeIdle)==UI_CLASS_CARPLAY)
			{
				SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_NONE,0);

				//media auto resume:
				#if CVTE_EN_KEY_PROXY
				CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
				CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
				#else
				OnMediaKey(KEY_MEDIA_PLAY);
				#endif
			}
			else if(GET_PANEL_CLASS_ID(idCurSRC)==UI_CLASS_ANDROID_AUTO)
			{
				if(m_bAndroidAutoConnect)
				{
					SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_NONE,0);
					//SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
				}
				else
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
				}
			}
			else if(m_bAndroidAutoConnect && GET_PANEL_CLASS_ID(m_idUIBeforeIdle)==UI_CLASS_ANDROID_AUTO)
			{
				SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_NONE,0);

				//media auto resume:
#if CVTE_EN_KEY_PROXY
				CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
				CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
#else
				OnMediaKey(KEY_MEDIA_PLAY);
#endif
			}
			else
			{
				if (GET_PANEL_CLASS_ID(idCurSRC)> UI_CLASS_NULL && GET_PANEL_CLASS_ID(idCurSRC) < UI_CLASS_MAX_VALID)
				{
					SwitchPage(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),
						GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC),
						PAGE_SWITCH_ANI_NONE,0);
				}
				else 
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
				}
			}
		}

		//reset volumes:
		int vol_t;
		//We just readjust the media volume

		//			//bluetooth volume:
		//			vol_t = m_pConfig->GetCurParam()->dwVolBluetooth;
		//			if(vol_t<5)vol_t=5;if(vol_t>20)vol_t=20;
		//			m_pConfig->GetCurParam()->dwVolBluetooth = vol_t;

		//media volume:
		vol_t = m_pConfig->GetCurParam()->dwVolMedia;
		if(vol_t<5)vol_t=5;if(vol_t>20)vol_t=20;
		m_pConfig->GetCurParam()->dwVolMedia = vol_t;

		//
		//			//gps volume:
		//			vol_t = m_pConfig->GetCurParam()->dwVolGPS;
		//			/*if(vol_t<5)vol_t=5;*/if(vol_t>20)vol_t=20;
		//			m_pConfig->GetCurParam()->dwVolGPS = vol_t;

		//un mute:
		m_bManualMute = FALSE;

        RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::PowerOn]: m_bPhoneCallActive:%d m_bSpeechOn:%d\r\n"),m_bPhoneCallActive,m_bSpeechOn));

#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)	
        if(m_bSpeechOn)
        {
#if USE_WAV_ONCHIP
            if(m_pCommander)
            {
                RETAILMSG(1, (TEXT("MSG:[PowerOn]: ReSend MIX cmd.\r\n")));
                m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,m_pConfig->GetCurParam()->dwVolGPS);
                m_pCommander->SendCmd(_ARM_MIX_WAV_KEY,1);
            }
#endif
        }
		else
		{
			SetVolume(BYTE(m_pConfig->GetCurParam()->dwVolGPS),FALSE,TRUE,TRUE,TRUE,FALSE);
		}

		if(m_bPhoneCallActive)
		{
			//SetMainVolume(BYTE(m_pConfig->GetCurParam()->dwVolBluetooth),TRUE);
            SetVolume(BYTE(m_pConfig->GetCurParam()->dwVolBluetooth),TRUE,TRUE);
		}
		else
		{
			//SetMainVolume(BYTE(m_pConfig->GetCurParam()->dwVolMedia),TRUE);
            SetVolume(BYTE(m_pConfig->GetCurParam()->dwVolMedia),TRUE,FALSE,TRUE,TRUE/*FALSE*/);
		}
#endif
	}

	//GPS:
	//GPSPowerOn();
	EnableGPSMonitor(TRUE);

//		//screen:
//		if(m_pSkinManager->GetConfig()->GetCurParam()->dwIdleScreen == IDLE_SCREEN_NONE)
//		{
//			ScreenPowerOn();
//		}

	//refresh UI:
	UpdateFrame();

	#if CVTE_EN_LED
	//on:
	if(m_pLED)
	{
        RETAILMSG(1, (TEXT("MSG:[PowerOn]: SetLedState LED_POWERON.\r\n")));
		m_pLED->SetLedState(LED_POWERON);
	}
	#endif

	//turn on back light:
	if(!m_bOnReverseMode)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]: =>ScreenOn \r\n"),TEXT(__FUNCTION__)));

		if(m_bBacklightOFF)
		{
			ScreenOn();
		}
	}

   OnSwitchAudioSRC(GetAudioSRC(), TRUE);
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	if (m_pAudioCodec)
	{
		m_pAudioCodec->MuteAMP(FALSE);
	}
#endif

	if(m_pConfig->GetCurParam()->dwVolGPS == 0)
	{
		EnableNaviSpeech(FALSE);
	}
	else
	{
		if(!g_bNaviSpeechEnabled && m_pConfig->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)//
		{
			EnableNaviSpeech(TRUE);
		}
	}
}
void  CGUI_Fantasy::PowerHardReset()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::PowerHardReset]\r\n")));

	m_idPowerState = SYS_POWER_HARD_RESET;

	m_dwPowerStateTick = GetTickCount();

	PrePowerOff();

	RenderPoweroffScreen();

	#if CVTE_EN_LED
	//led down:
	if(m_pLED)
	{
        RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(0) 4\r\n"), TEXT(__FUNCTION__)));
		m_pLED->SetLedParam(0,0);
	}
	#endif

	//mute:
	//Mute(TRUE,TRUE);

	//
	DoPowerHardReset();
}
void CGUI_Fantasy::PowerSoftReset()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::PowerSoftReset]\r\n")));

	m_idPowerState = SYS_POWER_SOFT_RESET;

	m_dwPowerStateTick = GetTickCount();

	PrePowerOff();

	RenderPoweroffScreen();

	//
// 	BKL_SetBackLight(BACKLIGHT_STATE_OFF,TRUE);
// 	m_bBacklightOFF=TRUE;

	#if CVTE_EN_LED
	//led down:
	if(m_pLED)
	{
        RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(0) 5\r\n"), TEXT(__FUNCTION__)));
		m_pLED->SetLedParam(0,0);
	}
	#endif

	//mute:
	//Mute(TRUE,TRUE);

	//
	DoPowerSoftReset();

	/*
	//reset:
	DWORD dwErr;
	dwErr = SetSystemPowerState(NULL, POWER_STATE_SUSPEND, POWER_FORCE);
	if ( dwErr )
	{
		RETAILMSG(DEBUG_GUI,( (L"SetSystemPowerState Error: %d\r\n"), dwErr ));
	}
	*/
}
void CGUI_Fantasy::PowerOff()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::PowerOff]\r\n")));

	m_dwPowerStateTick = GetTickCount();

	PrePowerOff();

	//navi speech control:
	if(m_bSpeechOn)
	{
		OnSpeechTerminate();
	}

	m_idPowerState = SYS_POWER_OFF;

	//
	BKL_SetBackLight(BACKLIGHT_STATE_OFF,TRUE);
	m_bBacklightOFF=TRUE;


	//mute:
	//Mute(TRUE,TRUE);
	//
	DoPowerOff();

	RETAILMSG(1,(_T("MSG: [CGUI_Fantasy::PowerOff] finish\r\n")));

	Sleep(3000);

	/*
 	DWORD dwErr;
 	dwErr = SetSystemPowerState(NULL, POWER_STATE_SUSPEND, POWER_FORCE);
 	if ( dwErr )
 	{
 		RETAILMSG(DEBUG_GUI,( (L"SetSystemPowerState Error: %d\r\n"), dwErr ));
 	}
	*/

}
void CGUI_Fantasy::OnNaviPathChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnNaviPathChanged]: %s\r\n"),(const wchar_t*)lParam));

	CPanelGL *p = m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NAVI,0));
	if(p)
	{
		((CPanelApp*)p)->SetAppPath((const wchar_t*)lParam);
	}
}

void CGUI_Fantasy::OnVolumePlus(int nStep)
{
	int vol;
	DWORD dwTick = GetTickCount();
	if(IsPowerOn() && !m_bMuteOperating)
	{
		if(m_bManualMute && !m_bPhoneCallActive)
		{
			if(dwTick-m_dwLastMuteTick>500)
			{
				OnMuteKey(FALSE);
			}
		}
		else
		{
			if(m_bPhoneCallActive)
			{
				vol = m_pConfig->GetCurParam()->dwVolBluetooth+nStep;
                if(vol>32)
                    vol=32;

				RETAILMSG(1, (TEXT("MSG:OnVolumePlus: Bluetooth:%d \r\n"),vol));

                m_pConfig->GetCurParam()->dwVolBluetooth = vol;
				m_iVolOriginBluetooth =vol;
                SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
			}
			else if(m_bSpeechOn && (m_bOnSR || IsNaviSpeechEnable()))
			{
				vol=m_pConfig->GetCurParam()->dwVolGPS+nStep;

                if(vol>32)
                    vol=32;

				RETAILMSG(1, (TEXT("MSG:OnVolumePlus: GPS:%d \r\n"),vol));

                m_pConfig->GetCurParam()->dwVolGPS = vol;
				m_iVolOriginGPS =vol;
                SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
			}
			else
			{
				if(!m_bSpeechOn)
            	{
					vol=m_pConfig->GetCurParam()->dwVolMedia+nStep;

                    if(vol>32)
                        vol=32;

					RETAILMSG(1, (TEXT("MSG:OnVolumePlus: Media:%d \r\n"),vol));
                    m_pConfig->GetCurParam()->dwVolMedia = vol;
					m_iVolOriginMedia =vol;
                    SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
                }
			}

			m_iSetVolume = vol;
			m_iSpeedCurVolome=m_iVehicleSpeed;

		}

		PlaySoundEffect(0);
	}
}

void CGUI_Fantasy::OnVolumeMinus(int nStep)
{
	int vol;
	DWORD dwTick = GetTickCount();
	if(IsPowerOn() && !m_bMuteOperating)
	{
		if(m_bManualMute && !m_bPhoneCallActive)
		{
			if(dwTick-m_dwLastMuteTick>500)
			{
				OnMuteKey(FALSE);
			}
		}
		else
		{
			if(m_bPhoneCallActive)
			{
				vol = m_pConfig->GetCurParam()->dwVolBluetooth-nStep;
                if(vol<0)
                    vol=0;

				RETAILMSG(1, (TEXT("MSG:OnVolumeMinus: Bluetooth:%d \r\n"),vol));

                m_pConfig->GetCurParam()->dwVolBluetooth = vol;
				m_iVolOriginBluetooth =vol;
                SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
			}
			else if(m_bSpeechOn && (m_bOnSR || IsNaviSpeechEnable()))
			{
				vol=m_pConfig->GetCurParam()->dwVolGPS-nStep;

                if(vol<0)
                    vol=0;

				RETAILMSG(1, (TEXT("MSG:OnVolumeMinus: GPS:%d \r\n"),vol));

                m_pConfig->GetCurParam()->dwVolGPS = vol;
				m_iVolOriginGPS =vol;
                SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
			}
			else
			{
				if(!m_bSpeechOn)
            	{
					vol=m_pConfig->GetCurParam()->dwVolMedia-nStep;

	                if(vol<0)
	                    vol=0;

					RETAILMSG(1, (TEXT("MSG:OnVolumeMinus: Media:%d \r\n"),vol));
                    m_pConfig->GetCurParam()->dwVolMedia = vol;
					m_iVolOriginMedia =vol;
                    SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
                }
			}

			m_iSetVolume = vol;
			m_iSpeedCurVolome=m_iVehicleSpeed;

		}

		if(vol != 0)
		{
			PlaySoundEffect(0);
		}
	}
}

#if (CVTE_EN_GESTURE_AIR == 1)
void CGUI_Fantasy::OnGestureCircle(WPARAM wParam, LPARAM lParam)
{
	BOOL bCircleRight = (BOOL)wParam;
	//if(m_pConfig->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER))
	if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_SETTING)
	{
		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));

		if(panel)
		{
			((CPanelSettingWrapper*)panel)->OnGestureCircle(bCircleRight);
		}
	}
	else
	{
		if(bCircleRight)
		{
			//
	        RETAILMSG(1, (TEXT("MSG:OnGestureCircle: right \r\n")));
			OnVolumePlus(1);
		}
		else
		{
			//
	        RETAILMSG(1, (TEXT("MSG:OnGestureCircle: left \r\n")));
			OnVolumeMinus(1);
		}
	}
}

void CGUI_Fantasy::OnGestureID(WPARAM wParam, LPARAM lParam)
{
	PACGestureData_t * pGestureData = (PACGestureData_t *)lParam;
	if(pGestureData == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: pGestureData is NULL!!!\r\n"), TEXT(__FUNCTION__)));
		return;
	}

	if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_SETTING)
	//if(m_pConfig->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_GESTURE))
	{
		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));

		if(panel)
		{
			((CPanelSettingWrapper*)panel)->OnGestureID(wParam, lParam);
		}
	}

 	switch(pGestureData->nGesture)
	{
		case GES_RIGHT_FLAG:
			RETAILMSG(1, (TEXT("MSG:OnGestureID: Right move\r\n")));
			if(m_bOpenSRC)
			{
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else
				{
#if CVTE_EN_KEY_PROXY
					CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_NEXT);
					CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_NEXT);
#else
                    UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
    				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));

    				if(panel)
    				{
    					panel->OnKey(KEY_MEDIA_NEXT);
    				}
#endif
                }
			}
			break;
		case GES_LEFT_FLAG:
			RETAILMSG(1, (TEXT("MSG:OnGestureID: Left move\r\n")));
			if(m_bOpenSRC)
			{
                if(m_bManualMute)
                {
                    OnMuteKey(FALSE);
                }
                else if(m_pConfig->GetCurParam()->dwVolMedia == 0)
                {
                    if(m_bSpeechSignaled && m_bSpeechOn)
                    {
                        UpdateVolume(VOLUME_DEFAULT_MINI_VALUE);
                    }
                    else
                    {
                        UpdateVolume(VOLUME_DEFAULT_NORMAL_VALUE);
                    }
                }
                else
                {
#if CVTE_EN_KEY_PROXY
					CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PREVIOUS);
					CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PREVIOUS);
#else
    				UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
    				CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
    				if(panel)
    				{
    					panel->OnKey(KEY_MEDIA_PREVIOUS);
    				}
#endif
                }

			}
			break;
		case GES_UP_FLAG:
			RETAILMSG(1, (TEXT("MSG:OnGestureID: Up move\r\n")));
			break;
		case GES_DOWN_FLAG:
			RETAILMSG(1, (TEXT("MSG:OnGestureID: Down move\r\n")));
			break;
        case GES_FORWARD_FLAG:
            RETAILMSG(1, (TEXT("MSG:OnGestureID: Forward move\r\n")));
            break;
        case GES_BACKWARD_FLAG:
            RETAILMSG(1, (TEXT("MSG:OnGestureID: Backward move\r\n")));
//				SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
            break;
        case GES_CLOCKWISE_FLAG:
        case GES_COUNT_CLOCKWISE_FLAG:
            break;
		default:
            RETAILMSG(1, (TEXT("MSG:OnGestureID:nGesture[%X]\r\n"), pGestureData->nGesture));
			break;
	}

	if(pGestureData->nWave)
	{
		if(/*m_pLED && */m_idPowerState == SYS_POWER_ON)
		{
	   		RETAILMSG(1, (TEXT("MSG:OnGestureID:nWave[%X]\r\n"), pGestureData->nWave));

			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
			}
			PlaySoundEffect(0);
			//m_pLED->SetLedFlash(TRUE);
		}
	}
}
#endif

void CGUI_Fantasy::OnGestureMsg(WPARAM wParam, LPARAM lParam)
{
	static BOOL bEnableGes=FALSE;

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnGestureMsg]: 0x%x\r\n"),wParam));


	switch (wParam)
	{
	case TOUCH_STATUS_DOWN_START:
		m_bOnGestureProcess=TRUE;
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnGestureMsg]: start.....0x%x\r\n"),wParam));
		bEnableGes=TRUE;
		break;
	case TOUCH_STATUS_UP:
	case GEST_SINGLE_TOUCH_END:
		m_bOnGestureProcess=FALSE;
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnGestureMsg]: end.....0x%x\r\n"),wParam));
		bEnableGes = FALSE;

		break;
	default:
		{
			if(bEnableGes)
			{
// 				if(m_pCurPanel->GetID()==MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER))
// 					SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_RIGHT);
// 				else
// 					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);

				//enter sr:
// 				if(wParam==GEST_DUAL_TOUCH_PAN_SOUTH)
// 				{
// 					SwitchPage(UI_CLASS_SR,PAGE_SR_MAIN,PAGE_SWITCH_ANI_DOWN);
// 				}

				if(wParam!=0 && wParam!=TOUCH_STATUS_INVALID)
				{
					if(m_pCurPanel)
					{
						m_pCurPanel->OnGestureMsg(wParam);
					}
					if(wParam!=GEST_DUAL_TOUCH_ZOOM_IN && wParam!=GEST_DUAL_TOUCH_ZOOM_OUT)
					{
						bEnableGes = FALSE;
					}
				}
			}
		}
		break;
	}

}

#if (CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_U701P)
void CGUI_Fantasy::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	#define CVTE_DEF_VOL_SILDER_LEFT			340
	#define CVTE_DEF_VOL_SILDER_RIGHT			800

	#define CVTE_DEF_VOL_VALUE_MAX				32

	static const RECT	rcPowerHome = {
		0, 0, 100, 50
	};

	static const RECT	rcSiri	= {
		130, 0, 230, 50
	};

	static const RECT	rcMute	= {
		250, 0, 350, 50
	};

	static const RECT	rcVolSilder	= {
		340, 0, 800, 50
	};

	static DWORD dwPenDownTick=0;

#if  CVTE_EN_KEY_PROXY
	static BOOL bPowerHomePenDown = FALSE;
	static BOOL bSiriPenDown = FALSE;
	static BOOL bMutePenDown = FALSE;
#endif

	static BOOL bEnabled=FALSE;
	static BOOL bMoving=FALSE;
	static BOOL bVolSliding=FALSE;

	static POINT ptPenDown;
	static POINT spt;
	static POINT pt;

    static UINT PenDownState;

    static BOOL bTouchStart=FALSE;

	static int 	iPenDownCurVol;
	static int 	iPenDownSetVol;
	static int	step = (CVTE_DEF_VOL_SILDER_RIGHT-CVTE_DEF_VOL_SILDER_LEFT/CVTE_DEF_VOL_VALUE_MAX);


	if(!m_bSystemReady || !m_bDeviceActivated)
	{
		RETAILMSG(1, (TEXT("System is not ready.\r\n")));
		return;
	}

    if(m_bOnHWMute)
        return;

	pt.x = pCyMultiPoint->sPointCoordinate[0].PointX;
	pt.y = pCyMultiPoint->sPointCoordinate[0].PointY;
	BYTE area = pCyMultiPoint->sPointStatus.toucharea;
	int status = pCyMultiPoint->sPointStatus.pointstatus;
	BOOL bKeyBeep=FALSE;

	//static int pointnum = pCyMultiPoint->sPointStatus.pointnum;

    if(!bTouchStart)
    {
        if((status!=TOUCH_STATUS_DOWN_START && status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
        {
            status = TOUCH_STATUS_DOWN_START;
        }
        bTouchStart = TRUE;
    }

	//reset gesture:
	if((status==TOUCH_STATUS_DOWN_START) || (!bEnabled && status==TOUCH_STATUS_DOWN))//first pen down
	{
        //RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: DOWN Start\r\n")));
        //RETAILMSG(1,(_T("@OnTouchMsg: Down\r\n")));
        bTouchStart = TRUE;
		ptPenDown = pt;
		bEnabled = TRUE;
		dwPenDownTick=GetTickCount();
		iPenDownSetVol = (ptPenDown.x-CVTE_DEF_VOL_SILDER_LEFT)*CVTE_DEF_VOL_VALUE_MAX/(CVTE_DEF_VOL_SILDER_RIGHT-CVTE_DEF_VOL_SILDER_LEFT);
		if(iPenDownSetVol<1)
			iPenDownSetVol=1;
		if(iPenDownSetVol>=CVTE_DEF_VOL_VALUE_MAX)
			iPenDownSetVol=CVTE_DEF_VOL_VALUE_MAX;

        if(m_bPhoneCallActive)
        {
            PenDownState = PENDOWN_STATE_PHONE;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolBluetooth;
        }
        else if(m_bSpeechOn  && (m_bOnSR || IsNaviSpeechEnable()))
        {
            PenDownState = PENDOWN_STATE_SPEECH;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolGPS;
        }
        else
        {
            PenDownState = PENDOWN_STATE_MEDIA;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolMedia;
        }

		spt = pt;
	}
	else if((status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
	{
		//save point and status:
		spt = pt;
	}

    //RETAILMSG(1,(_T("@@OnTouchMsg: x:%d,y:%d, sx:%d sy:%d status:%x,bgfx:%d\r\n"),pt.x,pt.y,spt.x,spt.y,status,bGfx));

	//GWES window
	if(area == TOUCH_AREA_GFX_WINDOW)
	{
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Down\r\n")));

			//Touch down or move
			if(GetForegroundWindow() != g_hMainWnd && !m_bManualMute && status==TOUCH_STATUS_DOWN_START)
			{
				bKeyBeep=TRUE;
			}

			if(status==TOUCH_STATUS_DOWN_START)
			{
				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchDown(&ptPenDown);
				}

#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchDown(&ptPenDown);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchDown(&ptPenDown);
				}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				if(status==TOUCH_STATUS_DOWN_START && m_pSmartBarVol && m_pSmartBarVol->IsVisible())
				{
					m_pSmartBarVol->OnTouchDown(&ptPenDown);
				}
#endif
			}
		}
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//Touch up
        	//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Up\r\n")));

			if(IsPowerOn())
			{
				//hide smart bar:
				if(m_bBacklightOFF)
				{
		            ScreenOn();
					bKeyBeep=TRUE;
				}
				else
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
					if(m_pSmartBarVol && m_pSmartBarVol->IsVisible())
					{
						m_pSmartBarVol->OnTouchUp(&spt);
						if(!m_pSmartBarVol->IsTouchOnObject(&spt) && !m_bManualMute)
						{
							//RETAILMSG(1, (TEXT("MSG:[%s]: Hide SmartBarVol\r\n"), TEXT(__FUNCTION__)));
							m_pSmartBarVol->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}
#else
					if(!m_bManualMute)
					{
						if(m_pSmartBarVol)
						{
							if(m_pSmartBarVol->IsVisible())
							{
								m_pSmartBarVol->Show(FALSE);
								bKeyBeep=TRUE;
								goto EXIT;
							}
						}
					}
#endif

					if(m_pSmartBarMedia)
					{
						if(m_pSmartBarMedia->IsVisible() && GetTickCount()-m_pSmartBarMedia->GetLastShowTick()>1000)
						{
							RETAILMSG(1, (TEXT("##### HIDE SMART BAR MEDIA ######\r\n")));
							m_pSmartBarMedia->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}

#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
					//auto unmute:
					if(m_bManualMute && !m_bPhoneCallActive && GetTickCount()-m_dwLastMuteTick>500)
					{
						OnMuteKey(FALSE);
						bKeyBeep=TRUE;
						goto EXIT;
					}
#endif
				}

				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchUp(&spt);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchUp(&spt);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchUp(&spt);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchUp(&spt);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchUp(&spt);
				}
			}
		}

		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_IMAGE)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN));

			if(panel)
			{
				((CPanelImageMain*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
		else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_TV)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER));

			if(panel)
			{
				((CPanelTV*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#endif
	}
	//Menu window
	else
	{
#if (CVTE_EN_BORDER_TOUCH == CVTE_DISABLE)
		goto EXIT;
#endif

		//Touch down or move
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{

			if(!bMoving)
			{
				bMoving = (abs(ptPenDown.x-spt.x)>10) && (GetTickCount()-dwPenDownTick>100);
			}

			//led slider
			if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcVolSilder,ptPenDown) && PtInRect(&rcVolSilder,spt) && bEnabled)
			{
				if(IsPowerOn() && !m_bManualMute/*!m_bMuteOperating*/)
				{
#if (USE_VOLUME_ADJUST_STYLE == 1)
					int vol=(spt.x-CVTE_DEF_VOL_SILDER_LEFT)*CVTE_DEF_VOL_VALUE_MAX/(CVTE_DEF_VOL_SILDER_RIGHT-CVTE_DEF_VOL_SILDER_LEFT);

					if(vol<1)
						vol=1;
					if(vol>=CVTE_DEF_VOL_VALUE_MAX)
						vol=CVTE_DEF_VOL_VALUE_MAX;

					if(!bVolSliding)
					{
                        //RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: iPenDownSetVol[%d] m_iVolumeFadeTo[%d] vol[%d]\r\n"),iPenDownSetVol,m_iVolumeFadeTo,vol));
                        if(iPenDownSetVol>m_iVolumeFadeTo && (abs(iPenDownSetVol-m_iVolumeFadeTo)>4))
                        {
                            bVolSliding = FALSE;
                        }
                        else
						{
                            bVolSliding = TRUE;
//								if(m_bSpeechOn && (m_bOnSR || IsNaviSpeechEnable()))
//									bVolSliding = ((vol<=m_pConfig->GetCurParam()->dwVolGPS) || vol-m_pConfig->GetCurParam()->dwVolGPS>10);      // 10
//								else
//									bVolSliding = ((vol<=m_iVolumeFadeTo) || vol-m_iVolumeFadeTo>10);      // 10
						}
        				//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: bVolSliding:%d m_iVolumeFadeTo:%d dwVolGPS:%d\r\n"),bVolSliding,m_iVolumeFadeTo,m_pConfig->GetCurParam()->dwVolGPS));
 					}

        			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: Move vol:%d\r\n"),vol));
					if(m_iVolumeFadeTo!=vol)
					{
						if(bVolSliding && bMoving)
						{
							m_iSetVolume = vol;
							m_iSpeedCurVolome=m_iVehicleSpeed=vol;

						    if(m_bPhoneCallActive && PenDownState == PENDOWN_STATE_PHONE)
							{
								m_iVolOriginBluetooth =vol;
                                m_pConfig->GetCurParam()->dwVolBluetooth = vol;
                                SetVolume(vol,FALSE,TRUE,FALSE);    //SetVolume(vol,TRUE,TRUE,FALSE);
							}
							else if(m_bSpeechOn /*&& !m_bOnSR */&& (m_bOnSR || IsNaviSpeechEnable()) && PenDownState == PENDOWN_STATE_SPEECH)
							{
								if(vol>0)
								{
	                                m_pConfig->GetCurParam()->dwVolGPS = vol;
									m_iVolOriginGPS =vol;
	                                SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
								}
							}
							else if(PenDownState == PENDOWN_STATE_MEDIA)
							{
                                if(m_bSpeechOn)
                                {
                                    //SetVolume(VOLUME_DEFAULT_MINI_VALUE,TRUE,TRUE,FALSE);
                                }
                                else
                                {
                                    m_pConfig->GetCurParam()->dwVolMedia = vol;
    								m_iVolOriginMedia =vol;
                                    SetVolume(vol,FALSE,TRUE,FALSE);  //SetVolume(vol,TRUE,TRUE,FALSE);
                                }
							}
                            else
                            {
								RETAILMSG(1, (TEXT("@@OnTouchMsg: Move Skip!!! PenDownState[%d]\r\n"),PenDownState));
                            }
						}
					}
#endif

#if (USE_VOLUME_ADJUST_STYLE == 2)

					int vol=(spt.x-CVTE_DEF_VOL_SILDER_LEFT)*CVTE_DEF_VOL_VALUE_MAX/(CVTE_DEF_VOL_SILDER_RIGHT-CVTE_DEF_VOL_SILDER_LEFT);
					//int range = (abs(x-ptPenDown.x))>step?(abs(x-ptPenDown.x)*32/ledsilder_w):1;
					//int volrange;
					int vol;
					//BOOL bRightmove;

					//Touch in the led area
					if(iPenDownSetVol <= iPenDownCurVol)
					{
						//right move
						if(voltouch>=m_iVolumeFadeTo)
						{
							vol = max(voltouch, m_iVolumeFadeTo);
						}
						//left move
						else
						{
							vol = voltouch;
						}
					}
					else
					{
						goto EXIT;
					}
//						//other
//						else
//						{
//							//right move
//							if(x>prevPointX)
//							{
//								//vol = volrange;
//								goto EXIT;
//							}
//							else
//							{
//								vol = min(voltouch, m_iVolumeFadeTo);
//							}
//						}

					if(vol>CVTE_DEF_VOL_VALUE_MAX)
						vol = CVTE_DEF_VOL_VALUE_MAX;
					else if(vol<1)
						vol = 1;

					if(m_iVolumeFadeTo!=vol)
					{
					    if(m_bPhoneCallActive && PenDownState == PENDOWN_STATE_PHONE)
						{
							m_iVolOriginBluetooth =vol;
	                        m_pConfig->GetCurParam()->dwVolBluetooth = vol;
							SetVolume(vol,FALSE,TRUE,FALSE);
						}
						else if(m_bSpeechOn && (m_bOnSR || IsNaviSpeechEnable()) && PenDownState == PENDOWN_STATE_SPEECH)
						{
	                        m_pConfig->GetCurParam()->dwVolGPS = vol;
							m_iVolOriginGPS =vol;
	                        SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
						}
						else if(PenDownState == PENDOWN_STATE_MEDIA && !m_bSpeechOn)
						{
	                        m_pConfig->GetCurParam()->dwVolMedia = vol;
							m_iVolOriginMedia =vol;
	                        SetVolume(vol,FALSE,TRUE,FALSE);  //SetVolume(vol,TRUE,TRUE,FALSE);
						}
						else
						{
						}

						m_iSetVolume = vol;
						m_iSpeedCurVolome=m_iVehicleSpeed=vol;
					}
#endif
				}
			}
	        //Power Home Key
	        else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcPowerHome,ptPenDown) && PtInRect(&rcPowerHome,spt) && bEnabled)
	        {
#if CVTE_EN_KEY_PROXY
				if(!bPowerHomePenDown)
				{
					bPowerHomePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_HARDWARE_HOME);
				}
#endif
	        }
	        //Siri
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcSiri,ptPenDown) && PtInRect(&rcSiri,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bSiriPenDown)
				{
					bSiriPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_SPEECH);
				}
#endif
			}
	        //Mute = Screen
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcMute,ptPenDown) && PtInRect(&rcMute,spt) && bEnabled && !bMoving)
	        {
#if CVTE_EN_KEY_PROXY
				if(!bMutePenDown)
				{
					bMutePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_MUTE_SCREEN);
				}
#endif
	        }
		}
	    //TOUCH_STATUS_UP: Lift off
	    //GEST_SINGLE_TOUCH_END: Single-Touch Single Click
	    //GEST_SINGLE_TOUCH_DOUBLE_CLICK: Single-Touch Double Click
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: Menu Up\r\n")));

	 	    //RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: UP PenDown.x[%d] sx[%d] sy[%d]\r\n"),ptPenDown.x,sx,sy));
			//LED SILDER
			if ( area == TOUCH_AREA_MENU_D_WINDOW && bEnabled && PtInRect(&rcVolSilder,ptPenDown) && PtInRect(&rcVolSilder,spt) )
			{
				DWORD dwTick = GetTickCount();
				if(IsPowerOn()  && !m_bMuteOperating && !bMoving && dwTick-dwPenDownTick<600)
				{
					if(m_bManualMute /*&& !m_bSpeechOn */&& !m_bPhoneCallActive)
					{
						if(dwTick-m_dwLastMuteTick>500)
						{
							OnMuteKey(FALSE);
							bKeyBeep=TRUE;
						}
					}
					else
					{
						int vol=(spt.x-CVTE_DEF_VOL_SILDER_LEFT)*CVTE_DEF_VOL_VALUE_MAX/(CVTE_DEF_VOL_SILDER_RIGHT-CVTE_DEF_VOL_SILDER_LEFT);
						if(vol<1)
							vol=1;
						if(vol>=CVTE_DEF_VOL_VALUE_MAX)
							vol=CVTE_DEF_VOL_VALUE_MAX;

						if(m_bPhoneCallActive && PenDownState == PENDOWN_STATE_PHONE)
						{
							if(vol>=10 && vol>m_pConfig->GetCurParam()->dwVolBluetooth)
							{
								if(m_pConfig->GetCurParam()->dwVolBluetooth>=10)
									vol=m_pConfig->GetCurParam()->dwVolBluetooth+1;
								else
									vol=10;
	                            if(vol>CVTE_DEF_VOL_VALUE_MAX)
	                                vol=CVTE_DEF_VOL_VALUE_MAX;
							}
						    //RETAILMSG(DEBUG_GUI,(_T("@@short click on bluetooth volume ,vol:%d\r\n"),vol));
	                        m_pConfig->GetCurParam()->dwVolBluetooth = vol;
							m_iVolOriginBluetooth =vol;
	                        SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
						}
						else if(m_bSpeechOn /*&& !m_bOnSR */&& (m_bOnSR ||IsNaviSpeechEnable()) && PenDownState == PENDOWN_STATE_SPEECH)
						{
							if(vol>=10 && vol>m_pConfig->GetCurParam()->dwVolGPS)
							{
								if(m_pConfig->GetCurParam()->dwVolGPS>=10)
									vol=m_pConfig->GetCurParam()->dwVolGPS+1;
								else
									vol=10;
	                            if(vol>CVTE_DEF_VOL_VALUE_MAX)
	                                vol=CVTE_DEF_VOL_VALUE_MAX;
							}
						    //RETAILMSG(DEBUG_GUI,(_T("@@short click on GPS volume ,vol:%d\r\n"),vol));
	                        m_pConfig->GetCurParam()->dwVolGPS = vol;
							m_iVolOriginGPS =vol;
	                        SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
						}
						else if(PenDownState == PENDOWN_STATE_MEDIA)
						{
	                        if(m_bSpeechOn)
	                        {
	                            //SetVolume(VOLUME_DEFAULT_MINI_VALUE,TRUE/*FALSE*/,TRUE,FALSE);
	                        }
	                        else
	                        {
	    						if(vol>=10 && vol>m_pConfig->GetCurParam()->dwVolMedia)
	    						{
	    							if(m_pConfig->GetCurParam()->dwVolMedia>=10)
	    								vol=m_pConfig->GetCurParam()->dwVolMedia+1;
	    							else
	    								vol=10;
	                                if(vol>CVTE_DEF_VOL_VALUE_MAX)
	                                    vol=CVTE_DEF_VOL_VALUE_MAX;
							    }
							    //RETAILMSG(DEBUG_GUI,(_T("@@short click on Media volume ,vol:%d\r\n"),vol));
	                            m_pConfig->GetCurParam()->dwVolMedia = vol;
								m_iVolOriginMedia =vol;
	                            SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
	                        }
						}

						m_iSetVolume = vol;
						m_iSpeedCurVolome=m_iVehicleSpeed;

					}
				}
			}

			//HOME KEY
			else if ( area == TOUCH_AREA_MENU_D_WINDOW && bEnabled && PtInRect(&rcPowerHome,ptPenDown) && PtInRect(&rcPowerHome,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bPowerHomePenDown)
				{
					bPowerHomePenDown=FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
				}
#endif			
			}
	        //SIRI KEY
			else if ( area == TOUCH_AREA_MENU_D_WINDOW && bEnabled && PtInRect(&rcSiri,ptPenDown) && PtInRect(&rcSiri,spt) )
			{
#if CVTE_EN_CARPLAY
				if(bSiriPenDown)
				{
					bSiriPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
				}
#endif
			}
	        //MUTE KEY
			else if ( area == TOUCH_AREA_MENU_D_WINDOW && bEnabled && PtInRect(&rcMute,ptPenDown) && PtInRect(&rcMute,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bMutePenDown)
				{
					bMutePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
				}
#endif
	        }
		}
	}

EXIT:
	if(bKeyBeep)
	{
		PlaySoundEffect(0);
	}

#if CVTE_EN_KEY_PROXY
	//This part use to check the touch area.
	if(bPowerHomePenDown)
	{
		if(!PtInRect(&rcPowerHome,spt))
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME,TRUE);
		}
	}
	else if(bSiriPenDown)
	{
		if(!PtInRect(&rcSiri,spt))
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH,TRUE);
		}
	}
	else if(bMutePenDown)
	{
		if(!PtInRect(&rcMute,spt))
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN,TRUE);
		}
	}
#endif

	if(status==TOUCH_STATUS_UP || status==GEST_SINGLE_TOUCH_END || status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//pen up
	{
#if CVTE_EN_KEY_PROXY
		if(bPowerHomePenDown)
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
		}
		else if(bSiriPenDown)
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
		}
		else if(bMutePenDown)
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
		}
#endif

		//reset:
		bEnabled = FALSE;
		bMoving = FALSE;
		bVolSliding = FALSE;
        bTouchStart = FALSE;
		spt.x = -1;
		spt.y = -1;
        PenDownState = PENDOWN_STATE_IDLE;
        //RETAILMSG(1,(_T("@OnTouchMsg: Up\r\n")));
	}
}
#elif (CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_U621P)
void CGUI_Fantasy::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
#define CVTE_DEF_VOL_VALUE_MAX				32


	static const RECT rcVolSilder = {
		0, 0, 82, 480
	};

	static const RECT	rcMute = {
		0, 475,	82, 548
	};

	static const RECT	rcPower = {
		165, 0,	235, 68
	};

	static const RECT	rcHome = {
		365, 0, 435, 68
	};

	static const RECT	rcSiri = {
		565, 0,	635, 68
	};

	static const RECT	rcBrightness = {
		0, 475,	82, 548
	};

	static const RECT rcBrightnessSilder = {
		0, 0, 82, 480
	};
	static DWORD dwPenDownTick=0;

#if  CVTE_EN_KEY_PROXY
	static BOOL bHomePenDown = FALSE;
	static BOOL bSiriPenDown = FALSE;
	static BOOL bMutePenDown = FALSE;
	static BOOL bPowerPenDown = FALSE;
	static BOOL bBrightnessPenDown = FALSE;
#endif

	static BOOL bEnabled=FALSE;
	static BOOL bMoving=FALSE;
	static BOOL bVolSliding=FALSE;

	static POINT ptPenDown;
	static POINT spt;
	static POINT pt;

	static UINT PenDownState;
	static BOOL bTouchStart=FALSE;

	static int 	iPenDownCurVol;
	static int 	iPenDownSetVol;
	static int step = (480/CVTE_DEF_VOL_VALUE_MAX);

	if(!m_bSystemReady || !m_bDeviceActivated)
	{
		RETAILMSG(1, (TEXT("System is not ready.\r\n")));
		return;
	}

	if(m_bOnHWMute)
		return;

	pt.x = pCyMultiPoint->sPointCoordinate[0].PointX;
	pt.y = pCyMultiPoint->sPointCoordinate[0].PointY;
	BYTE area = pCyMultiPoint->sPointStatus.toucharea;
	int status = pCyMultiPoint->sPointStatus.pointstatus;
	BOOL bKeyBeep=FALSE;

	//static int pointnum = pCyMultiPoint->sPointStatus.pointnum;

	if(!bTouchStart)
	{
		if((status!=TOUCH_STATUS_DOWN_START && status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
		{
			status = TOUCH_STATUS_DOWN_START;
		}
		bTouchStart = TRUE;
	}

	//reset gesture:
	if((status==TOUCH_STATUS_DOWN_START) || (!bEnabled && status==TOUCH_STATUS_DOWN))//first pen down
	{
		//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: DOWN Start\r\n")));
		bTouchStart = TRUE;
		ptPenDown = pt;
		bEnabled = TRUE;
		dwPenDownTick=GetTickCount();
		iPenDownSetVol = (480-ptPenDown.y)*32/480;

		if(m_bPhoneCallActive)
		{
			PenDownState = PENDOWN_STATE_PHONE;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolBluetooth;
		}
		else if(m_bSpeechOn  && (m_bOnSR || IsNaviSpeechEnable()))
		{
			PenDownState = PENDOWN_STATE_SPEECH;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolGPS;
		}
		else
		{
			PenDownState = PENDOWN_STATE_MEDIA;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolMedia;
		}
		spt = pt;
	}
	else if((status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
	{
		//save point and status:
		spt = pt;
	}

	//RETAILMSG(1,(_T("@@OnTouchMsg: x:%d,y:%d, sx:%d sy:%d status:%x,bgfx:%d\r\n"),pt.x,pt.y,spt.x,spt.y,status,bGfx));

	//GWES window
	if(area == TOUCH_AREA_GFX_WINDOW)
	{
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Down\r\n")));

			//Touch down or move
			if(GetForegroundWindow() != g_hMainWnd && !m_bManualMute && status==TOUCH_STATUS_DOWN_START)
			{
				bKeyBeep=TRUE;
			}

			if(status==TOUCH_STATUS_DOWN_START)
			{
				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchDown(&ptPenDown);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchDown(&ptPenDown);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchDown(&ptPenDown);
				}
			}
		}
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//Touch up
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Up\r\n")));

			if(IsPowerOn())
			{
				//hide smart bar:
				if(m_bBacklightOFF)
				{
					ScreenOn();
					bKeyBeep=TRUE;
				}
				else
				{
					if(!m_bManualMute)
					{
						if(m_pSmartBarVol)
						{
							if(m_pSmartBarVol->IsVisible())
							{
								m_pSmartBarVol->Show(FALSE);
								bKeyBeep=TRUE;
								goto EXIT;
							}
						}
					}

					if(m_pSmartBarMedia)
					{
						if(m_pSmartBarMedia->IsVisible() && GetTickCount()-m_pSmartBarMedia->GetLastShowTick()>1000)
						{
							RETAILMSG(1, (TEXT("##### HIDE SMART BAR MEDIA ######\r\n")));
							m_pSmartBarMedia->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}

#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
					//auto unmute:
					if(m_bManualMute && !m_bPhoneCallActive && GetTickCount()-m_dwLastMuteTick>500)
					{
						OnMuteKey(FALSE);
						bKeyBeep=TRUE;
						goto EXIT;
					}
#endif
				}

				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchUp(&spt);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchUp(&spt);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchUp(&spt);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchUp(&spt);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchUp(&spt);
				}
			}
		}

		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_IMAGE)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN));

			if(panel)
			{
				((CPanelImageMain*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
		else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_TV)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER));

			if(panel)
			{
				((CPanelTV*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#endif
	}
	//Menu window
	else
	{
#if (CVTE_EN_BORDER_TOUCH == CVTE_DISABLE)
		goto EXIT;
#endif
		//Touch down or move
		if(status==TOUCH_STATUS_DOWN_START || (status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK && status!=GEST_SINGLE_TOUCH_END))//down or move
		{

			if(!bMoving)
			{
				bMoving = (abs(ptPenDown.y-spt.y)>50) && (GetTickCount()-dwPenDownTick>100);
			}

			//RETAILMSG(1,(_T("@@OnTouchMsg: Menu:%d,%d PenDown:%d,%d bMoving:%d\r\n"),x,y, ptPenDown.x,ptPenDown.y,bMoving));

			//right led slider
			if( area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcBrightnessSilder, ptPenDown) && PtInRect(&rcBrightnessSilder, spt) && bEnabled )
			{
				if(bMoving && (IsPowerOn() || m_bOnReverseMode))
				{
					int bkl;

					if(m_iScrnBrightness == 1)
					{
						//Night mode
						bkl = CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START+GetNearInt((480-spt.y)*(CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END-CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START)/480);

						if(bkl<=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START)
							bkl=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START;
						else if(bkl>=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END)
							bkl=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END;	

						m_pConfig->GetCurParam()->iLCDNightModeBrightness = bkl;
					}
					else
					{
						//Day mode
						bkl = CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START+GetNearInt((480-spt.y)*(CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END-CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START)/480);

						if(bkl<=CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START)
							bkl=CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START;
						else if(bkl>=CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END)
							bkl=BACKLIGHT_STATE_FULLON;

						m_pConfig->GetCurParam()->iLCDDayModeBrightness = bkl;
					}

					BKL_SetBackLight(bkl,FALSE);

					m_bBacklightOFF=(bkl==BACKLIGHT_STATE_OFF);

					//save backlight:
					m_pConfig->GetCurParam()->dwBacklight=bkl;
					OnLCDBrightnessChange();
				}
			}
			//left led slider
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolSilder, ptPenDown) && PtInRect(&rcVolSilder, spt) && bEnabled )
			{
				if(bMoving && IsPowerOn() && !m_bManualMute)
				{
					int vol=1+GetNearInt((480-spt.y)*32/480);
					if(vol<1)
						vol=1;
					if(vol>=CVTE_DEF_VOL_VALUE_MAX)
						vol=CVTE_DEF_VOL_VALUE_MAX;

					if(!bVolSliding)
					{
						if(iPenDownSetVol>m_iVolumeFadeTo && (abs(iPenDownSetVol-m_iVolumeFadeTo)>4))
						{
							bVolSliding = FALSE;
						}
						else
						{
							if(m_bSpeechOn && (m_bOnSR || IsNaviSpeechEnable()))
								bVolSliding = ((vol<=m_pConfig->GetCurParam()->dwVolGPS) || vol-m_pConfig->GetCurParam()->dwVolGPS>10);      // 10
							else
								bVolSliding = ((vol<=m_iVolumeFadeTo) || vol-m_iVolumeFadeTo>10);      // 10
						}
						//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: bVolSliding:%d m_iVolumeFadeTo:%d\r\n"),bVolSliding,m_iVolumeFadeTo));
					}

					//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: Move vol:%d\r\n"),vol));
					if(m_iVolumeFadeTo!=vol)
					{
						if(bVolSliding && bMoving)
						{
							m_iSetVolume = vol;
							m_iSpeedCurVolome=m_iVehicleSpeed=vol;

							if(m_bPhoneCallActive && PenDownState == PENDOWN_STATE_PHONE)
							{
								m_iVolOriginBluetooth =vol;
								m_pConfig->GetCurParam()->dwVolBluetooth = vol;
								SetVolume(vol,FALSE,TRUE,FALSE);    //SetVolume(vol,TRUE,TRUE,FALSE);
							}
							else if(m_bSpeechOn /*&& !m_bOnSR */&& (m_bOnSR || IsNaviSpeechEnable()) && PenDownState == PENDOWN_STATE_SPEECH)
							{
								if(vol>0)
								{
									m_pConfig->GetCurParam()->dwVolGPS = vol;
									m_iVolOriginGPS =vol;
									SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
								}
							}
							else if(PenDownState == PENDOWN_STATE_MEDIA)
							{
								if(m_bSpeechOn)
								{
									//SetVolume(VOLUME_DEFAULT_MINI_VALUE,TRUE,TRUE,FALSE);
								}
								else
								{
									m_pConfig->GetCurParam()->dwVolMedia = vol;
									m_iVolOriginMedia =vol;
									SetVolume(vol,FALSE,TRUE,FALSE);    //SetVolume(vol,TRUE,TRUE,FALSE);
								}
							}
							else
							{
								RETAILMSG(1, (TEXT("@@OnTouchMsg: Move Skip!!! PenDownState[%d]\r\n"),PenDownState));
							}
						}
					}
				}

			}
			//Power Key
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcPower, ptPenDown) && PtInRect(&rcPower, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bPowerPenDown)
				{
					bPowerPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_POWER);
				}
#endif
			}
			//Siri
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcSiri, ptPenDown) && PtInRect(&rcSiri, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bSiriPenDown)
				{
					bSiriPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_SPEECH);
				}
#endif
			}
			//Home Key
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcHome, ptPenDown) && PtInRect(&rcHome, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bHomePenDown)
				{
					bHomePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_HOME);
				}
#endif
			}
			//Mute Key
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcMute, ptPenDown) && PtInRect(&rcMute, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bMutePenDown)
				{
					bMutePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_MUTE);
				}
#endif
			}
			//Brightness Key
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcBrightness, ptPenDown) && PtInRect(&rcBrightness, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bBrightnessPenDown)
				{
					bBrightnessPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_LUMINACE);
				}
#endif
			}
		}
		//TOUCH_STATUS_UP: Lift off
		//GEST_SINGLE_TOUCH_END: Single-Touch Single Click
		//GEST_SINGLE_TOUCH_DOUBLE_CLICK: Single-Touch Double Click
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: UP\r\n")));
			//Left Led silder
			if( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcVolSilder,ptPenDown) && PtInRect(&rcVolSilder,spt))
			{
				DWORD dwTick = GetTickCount();
				if(IsPowerOn() && !m_bMuteOperating && !bMoving && dwTick-dwPenDownTick<600)
				{
					if(m_bManualMute && !m_bPhoneCallActive)
					{
						if(dwTick-m_dwLastMuteTick>500)
						{
							OnMuteKey(FALSE);
							bKeyBeep=TRUE;
						}
					}
					else
					{
						int vol=1+GetNearInt((480-spt.y)*31/480);
						if(vol<1)
							vol=1;
						if(vol>=CVTE_DEF_VOL_VALUE_MAX)
							vol=CVTE_DEF_VOL_VALUE_MAX;

						if(m_bPhoneCallActive && PenDownState == PENDOWN_STATE_PHONE)
						{
							if(vol>=10 && vol>m_pConfig->GetCurParam()->dwVolBluetooth)
							{
								if(m_pConfig->GetCurParam()->dwVolBluetooth>=10)
									vol=m_pConfig->GetCurParam()->dwVolBluetooth+1;
								else
									vol=10;
								if(vol>CVTE_DEF_VOL_VALUE_MAX)
									vol=CVTE_DEF_VOL_VALUE_MAX;
							}
							RETAILMSG(DEBUG_GUI,(_T("@@short click on bluetooth volume ,vol:%d\r\n"),vol));
							m_pConfig->GetCurParam()->dwVolBluetooth = vol;
							m_iVolOriginBluetooth =vol;
							SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
						}
						else if(m_bSpeechOn && (m_bOnSR || IsNaviSpeechEnable()) && PenDownState == PENDOWN_STATE_SPEECH)
						{
							if(vol>=10 && vol>m_pConfig->GetCurParam()->dwVolGPS)
							{
								if(m_pConfig->GetCurParam()->dwVolGPS>=10)
									vol=m_pConfig->GetCurParam()->dwVolGPS+1;
								else
									vol=10;
								if(vol>CVTE_DEF_VOL_VALUE_MAX)
									vol=CVTE_DEF_VOL_VALUE_MAX;
							}
							RETAILMSG(DEBUG_GUI,(_T("@@short click on GPS volume ,vol:%d\r\n"),vol));
							m_pConfig->GetCurParam()->dwVolGPS = vol;
							m_iVolOriginGPS =vol;
							SetVolume(vol,FALSE,TRUE,FALSE,TRUE,m_bOnSR);
						}
						else if(PenDownState == PENDOWN_STATE_MEDIA)
						{
							if(m_bSpeechOn)
							{
								//SetVolume(VOLUME_DEFAULT_MINI_VALUE,TRUE/*FALSE*/,TRUE,FALSE);
							}
							else
							{
								if(vol>=10 && vol>m_pConfig->GetCurParam()->dwVolMedia)
								{
									if(m_pConfig->GetCurParam()->dwVolMedia>=10)
										vol=m_pConfig->GetCurParam()->dwVolMedia+1;
									else
										vol=10;

									if(vol>CVTE_DEF_VOL_VALUE_MAX)
										vol=CVTE_DEF_VOL_VALUE_MAX;
								}
								RETAILMSG(DEBUG_GUI,(_T("@@short click on Media volume ,vol:%d\r\n"),vol));
								m_pConfig->GetCurParam()->dwVolMedia = vol;
								m_iVolOriginMedia =vol;
								SetVolume(vol,TRUE/*FALSE*/,TRUE,FALSE);
							}
						}

						m_iSetVolume = vol;
						m_iSpeedCurVolome=m_iVehicleSpeed;

					}
				}
			}
			//right led slider
			else if(area == TOUCH_AREA_MENU_R_WINDOW && bEnabled && PtInRect(&rcBrightnessSilder,ptPenDown) && PtInRect(&rcBrightnessSilder,spt))
			{
				if(!bMoving && (IsPowerOn() || m_bOnReverseMode))
				{
					int bkl;

					if(m_iScrnBrightness == 1)
					{
						//Night mode
						bkl = CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START+GetNearInt((480-spt.y)*(CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END-CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START)/480);

						if(bkl<=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START)
							bkl=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START;
						else if(bkl>=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END)
							bkl=CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END;	

						m_pConfig->GetCurParam()->iLCDNightModeBrightness = bkl;
					}
					else
					{
						//Day mode
						bkl = CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START+GetNearInt((480-spt.y)*(CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END-CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START)/480);

						if(bkl<=CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START)
							bkl=CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START;
						else if(bkl>=CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END)
							bkl=BACKLIGHT_STATE_FULLON;

						m_pConfig->GetCurParam()->iLCDDayModeBrightness = bkl;
					}

					BKL_SetBackLight(bkl,FALSE);

					m_bBacklightOFF=(bkl==BACKLIGHT_STATE_OFF);

					//save backlight:
					m_pConfig->GetCurParam()->dwBacklight=bkl;
					OnLCDBrightnessChange();

					bKeyBeep=TRUE;
				}
			}
			//Power Key
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcPower, ptPenDown) && PtInRect(&rcPower, spt) && bEnabled)
			{

#if CVTE_EN_KEY_PROXY
				if(bPowerPenDown)
				{
					bPowerPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_POWER);
				}
#endif
			}
			//Siri
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcSiri, ptPenDown) && PtInRect(&rcSiri, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(bSiriPenDown)
				{
					bSiriPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
				}
#endif
			}
			//Home Key
			else if(area == TOUCH_AREA_MENU_D_WINDOW && PtInRect(&rcHome, ptPenDown) && PtInRect(&rcHome, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(bHomePenDown)
				{
					bHomePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_HOME);
				}
#endif
			}
			//Mute Key
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcMute, ptPenDown) && PtInRect(&rcMute, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(bMutePenDown)
				{
					bMutePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MUTE);
				}
#endif
			}
			//Brightness Key
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcBrightness, ptPenDown) && PtInRect(&rcBrightness, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(bBrightnessPenDown)
				{
					bBrightnessPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE);
				}
#endif
			}
		}
	}

EXIT:
	if(bKeyBeep)
	{
		PlaySoundEffect(0);
	}

#if CVTE_EN_KEY_PROXY
	//This part use to check the touch area.
	if(bHomePenDown)
	{
		if(!PtInRect(&rcHome,spt))
		{
			bHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_HOME,TRUE);
		}
	}
	else if(bSiriPenDown)
	{
		if(!PtInRect(&rcSiri,spt))
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH,TRUE);
		}
	}
	else if(bMutePenDown)
	{
		if(!PtInRect(&rcMute,spt))
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MUTE,TRUE);
		}
	}
	else if(bPowerPenDown)
	{
		if(!PtInRect(&rcPower,spt))
		{
			bPowerPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_POWER,TRUE);
		}
	}
	else if(bBrightnessPenDown)
	{
		if(!PtInRect(&rcBrightness,spt))
		{
			bBrightnessPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE,TRUE);
		}
	}	
#endif

	if(status==TOUCH_STATUS_UP || status==GEST_SINGLE_TOUCH_END || status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//pen up
	{
#if CVTE_EN_KEY_PROXY
		if(bHomePenDown)
		{
			bHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_HOME);
		}
		else if(bSiriPenDown)
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
		}
		else if(bMutePenDown)
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MUTE);
		}
		else if(bPowerPenDown)
		{
			bPowerPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_POWER);
		}
		else if(bBrightnessPenDown)
		{
			bBrightnessPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE);
		}				
#endif
		//reset:
		bEnabled = FALSE;
		bMoving = FALSE;
		bVolSliding = FALSE;
		spt.x = -1;
		spt.y = -1;

		PenDownState = PENDOWN_STATE_IDLE;
	}
}
#elif (CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_U702P)
void CGUI_Fantasy::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	#define CVTE_DEF_VOL_VALUE_MAX				32

	static const RECT	rcPowerHome = {	
		0, 0, 45, 100 
	};
	static const RECT	rcSiri	= {	
		0, 120,	45,	200	
	};
	static const RECT	rcVolPlus	= {	
		0, 220,	45, 300	
	};
	static const RECT	rcVolMinus	= {
		0, 320,	45, 400	
	};
	static const RECT	rcMute	= {	
		0, 410,	45, 480	
	};
	static DWORD dwPenDownTick=0;

#if  CVTE_EN_KEY_PROXY
	static BOOL bPowerHomePenDown = FALSE;
	static BOOL bSiriPenDown = FALSE;
	static BOOL bVolPlusPenDown = FALSE;
	static BOOL bVolMinusPenDown = FALSE;
	static BOOL bMutePenDown = FALSE;
#endif

	static BOOL bEnabled=FALSE;

	static POINT ptPenDown;
	static POINT spt;
	static POINT pt;

	static UINT PenDownState;

	static BOOL bTouchStart=FALSE;

	static int 	iPenDownCurVol;
	static int 	iPenDownSetVol;

	if(!m_bSystemReady || !m_bDeviceActivated)
	{
		RETAILMSG(1, (TEXT("System is not ready.\r\n")));
		return;
	}

	if(m_bOnHWMute)
		return;

	pt.x = pCyMultiPoint->sPointCoordinate[0].PointX;
	pt.y = pCyMultiPoint->sPointCoordinate[0].PointY;
	BYTE area = pCyMultiPoint->sPointStatus.toucharea;
	int status = pCyMultiPoint->sPointStatus.pointstatus;
	BOOL bKeyBeep=FALSE;

	//static int pointnum = pCyMultiPoint->sPointStatus.pointnum;

	if(!bTouchStart)
	{
		if((status!=TOUCH_STATUS_DOWN_START && status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
		{
			status = TOUCH_STATUS_DOWN_START;
		}
		bTouchStart = TRUE;
	}

	//reset gesture:
	if((status==TOUCH_STATUS_DOWN_START) || (!bEnabled && status==TOUCH_STATUS_DOWN))//first pen down
	{
		//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: DOWN Start\r\n")));
		//RETAILMSG(1,(_T("@OnTouchMsg: Down\r\n")));
		bTouchStart = TRUE;
		ptPenDown = pt;
		bEnabled = TRUE;
		dwPenDownTick=GetTickCount();

		if(m_bPhoneCallActive)
		{
			PenDownState = PENDOWN_STATE_PHONE;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolBluetooth;
		}
		else if(m_bSpeechOn  && (m_bOnSR || IsNaviSpeechEnable()))
		{
			PenDownState = PENDOWN_STATE_SPEECH;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolGPS;
		}
		else
		{
			PenDownState = PENDOWN_STATE_MEDIA;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolMedia;
		}

		spt = pt;
	}
	else if((status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
	{
		//save point and status:
		spt = pt;
	}

	//RETAILMSG(1,(_T("@@OnTouchMsg: x:%d,y:%d, sx:%d sy:%d status:%x area:%d\r\n"),pt.x,pt.y,spt.x,spt.y,status,area));

	//GWES window
	if(area == TOUCH_AREA_GFX_WINDOW)
	{
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Down\r\n")));

			//Touch down or move
			if(GetForegroundWindow() != g_hMainWnd && !m_bManualMute && status==TOUCH_STATUS_DOWN_START)
			{
				bKeyBeep=TRUE;
			}

			if(status==TOUCH_STATUS_DOWN_START)
			{
				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchDown(&ptPenDown);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchDown(&ptPenDown);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchDown(&ptPenDown);
				}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				if(status==TOUCH_STATUS_DOWN_START && m_pSmartBarVol && m_pSmartBarVol->IsVisible())
				{
					m_pSmartBarVol->OnTouchDown(&ptPenDown);
				}
#endif
#if CVTE_EN_ACCON_WARNING
				CWarningProxy::GetInstance()->OnTouchDown(&ptPenDown);
#endif
			}
		}
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//Touch up
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Up\r\n")));
			if(IsPowerOn())
			{
				//hide smart bar:
				if(m_bBacklightOFF)
				{
					ScreenOn();
					bKeyBeep=TRUE;
				}
				else
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
					if(m_pSmartBarVol && m_pSmartBarVol->IsVisible())
					{
						POINT RealPt = {spt.x-55,spt.y};
						m_pSmartBarVol->OnTouchUp(&spt);
						if(!m_pSmartBarVol->IsTouchOnObject(&RealPt) && !m_bManualMute)
						{
							//RETAILMSG(1, (TEXT("MSG:[%s]: Hide SmartBarVol\r\n"), TEXT(__FUNCTION__)));
							m_pSmartBarVol->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}
#else
					if(!m_bManualMute)
					{
						if(m_pSmartBarVol)
						{
							if(m_pSmartBarVol->IsVisible())
							{
								m_pSmartBarVol->Show(FALSE);
								bKeyBeep=TRUE;
								goto EXIT;
							}
						}
					}
#endif

					if(m_pSmartBarMedia)
					{
						if(m_pSmartBarMedia->IsVisible() && GetTickCount()-m_pSmartBarMedia->GetLastShowTick()>1000)
						{
							RETAILMSG(1, (TEXT("##### HIDE SMART BAR MEDIA ######\r\n")));
							m_pSmartBarMedia->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}
										
#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
					//auto unmute:
					if(m_bManualMute && !m_bPhoneCallActive && GetTickCount()-m_dwLastMuteTick>500)
					{
						OnMuteKey(FALSE);
						bKeyBeep=TRUE;
						goto EXIT;
					}
#endif					
				}

				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchUp(&spt);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchUp(&spt);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchUp(&spt);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchUp(&spt);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchUp(&spt);
				}

#if CVTE_EN_ACCON_WARNING
				CWarningProxy::GetInstance()->OnTouchUp(&spt);
#endif
			}
		}

		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_IMAGE)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN));

			if(panel)
			{
				((CPanelImageMain*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
		else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_TV)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER));

			if(panel)
			{
				((CPanelTV*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#endif
	}
	//Menu window
	else
	{
#if (CVTE_EN_BORDER_TOUCH == CVTE_DISABLE)
		goto EXIT;
#endif

		//Touch down or move
		if(area == TOUCH_AREA_MENU_L_WINDOW && status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//Power Home Key
			if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcPowerHome,ptPenDown) && PtInRect(&rcPowerHome,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bPowerHomePenDown)
				{
					bPowerHomePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_HARDWARE_HOME);
				}
#endif
			}
			//Siri
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcSiri,ptPenDown) && PtInRect(&rcSiri,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bSiriPenDown)
				{
					bSiriPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_SPEECH);
				}
#endif
			}
			//Vol Plus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolPlus,ptPenDown) && PtInRect(&rcVolPlus,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bVolPlusPenDown)
				{
					bVolPlusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_PLUS);
				}
#endif
			}
			//Vol Minus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolMinus,ptPenDown) && PtInRect(&rcVolMinus,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bVolMinusPenDown)
				{
					bVolMinusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_MINUS);
				}
#endif
			}
			//Mute = Screen
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcMute,ptPenDown) && PtInRect(&rcMute,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bMutePenDown)
				{
					bMutePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_MUTE_SCREEN);
				}
#endif
			}
		}
		//TOUCH_STATUS_UP: Lift off
		//GEST_SINGLE_TOUCH_END: Single-Touch Single Click
		//GEST_SINGLE_TOUCH_DOUBLE_CLICK: Single-Touch Double Click
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: Menu Up\r\n")));

			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: UP PenDown.x[%d] sx[%d] sy[%d]\r\n"),ptPenDown.x,sx,sy));
			//HOME KEY
			if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcPowerHome,ptPenDown) && PtInRect(&rcPowerHome,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bPowerHomePenDown)
				{
					bPowerHomePenDown=FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
				}
#endif			
			}
			//SIRI KEY
			else if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcSiri,ptPenDown) && PtInRect(&rcSiri,spt) )
			{
#if CVTE_EN_CARPLAY
				if(bSiriPenDown)
				{
					bSiriPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
				}
#endif
			}
			//Vol Plus KEY
			else if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcVolPlus,ptPenDown) && PtInRect(&rcVolPlus,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bVolPlusPenDown)
				{
					bVolPlusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS);
				}
#endif
			}
			//Vol Minus KEY
			else if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcVolMinus,ptPenDown) && PtInRect(&rcVolMinus,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bVolMinusPenDown)
				{
					bVolMinusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
				}
#endif
			}
			//MUTE KEY
			else if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcMute,ptPenDown) && PtInRect(&rcMute,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bMutePenDown)
				{
					bMutePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
				}
#endif
			}
		}
	}

EXIT:
	if(bKeyBeep)
	{
		PlaySoundEffect(0);
	}

#if CVTE_EN_KEY_PROXY
	//This part use to check the touch area.
	if(bPowerHomePenDown)
	{
		if(!PtInRect(&rcPowerHome,spt))
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME,TRUE);
		}
	}
	else if(bSiriPenDown)
	{
		if(!PtInRect(&rcSiri,spt))
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH,TRUE);
		}
	}
	else if(bVolPlusPenDown)
	{
		if(!PtInRect(&rcVolPlus,spt))
		{
			bVolPlusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS,TRUE);
		}
	}
	else if(bVolMinusPenDown)
	{
		if(!PtInRect(&rcVolMinus,spt))
		{
			bVolMinusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS,TRUE);
		}
	}
	else if(bMutePenDown)
	{
		if(!PtInRect(&rcMute,spt))
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN,TRUE);
		}
	}
#endif

	if(status==TOUCH_STATUS_UP || status==GEST_SINGLE_TOUCH_END || status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//pen up
	{
#if CVTE_EN_KEY_PROXY
		if(bPowerHomePenDown)
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
		}
		else if(bSiriPenDown)
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
		}
		else if(bVolMinusPenDown)
		{
			bVolMinusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
		}
		else if(bMutePenDown)
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
		}
		else if(bMutePenDown)
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
		}
#endif

		//reset:
		bEnabled = FALSE;
		bTouchStart = FALSE;
		spt.x = -1;
		spt.y = -1;
		PenDownState = PENDOWN_STATE_IDLE;
		//RETAILMSG(1,(_T("@OnTouchMsg: Up\r\n")));
	}
}

#elif (CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_U703PT || CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_U705PM)	//U703PT U705PM
#if (CVTE_MODEL_TYPE == CVTE_MODEL_TYPE_U703PT_DSP_AK7738)
#define USE_NEW_U703PT_TP	1
#else
#define USE_NEW_U703PT_TP	0
#endif
void CGUI_Fantasy::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
#if USE_NEW_U703PT_TP
	static const RECT	rcVolMute = {
		0, 0, 90, 295
	};

	static const RECT	rcPower = {
		0, 70, 90, 145
	};

	static const RECT	rcVolPlus = {
		0, 220, 90, 295
	};

	static const RECT	rcVolMinus = {
		0, 370, 90, 480
	};

	static const RECT	rcHome = {
		0, 70, 90, 145
	};

	static const RECT	rcSiri = {
		0, 220, 90, 295
	};

	static const RECT	rcPhone = {
		0, 370, 90, 480
	};

	static BOOL bHomePenDown = FALSE;
	static BOOL bPhonePenDown = FALSE;
	static BOOL bPowerPenDown = FALSE;
	static BOOL bVolMinusPenDown = FALSE;
	static BOOL bVolPlusPenDown = FALSE;
	static BOOL bSiriPenDown = FALSE;
#else
	static const RECT	rcVolPlus = {
		0, 70, 90, 145
	};

	static const RECT	rcVolManual = {
		0, 220, 90, 445
	};

	static const RECT	rcVolMinus = {
		0, 370, 90, 480
	};

	static const RECT	rcBrightness = {
		0, 70, 90, 145
	};

	static const RECT	rcSiri = {
		0, 220, 90, 295
	};

	static const RECT	rcPowerHome = {
		0, 370, 90, 480
	};
	static BOOL bPowerHomePenDown = FALSE;
	static BOOL bSiriPenDown = FALSE;
	static BOOL bBrightnessPenDown = FALSE;
	static BOOL bVolManualPenDown = FALSE;
	static BOOL bVolMinusPenDown = FALSE;
	static BOOL bVolPlusPenDown = FALSE;
#endif


	static DWORD dwPenDownTick=0;
	
	static BOOL bEnabled=FALSE;
	static BOOL bMoving=FALSE;
	static BOOL bVolSliding=FALSE;

	static POINT ptPenDown;
	static POINT spt;
	static POINT pt;

	static int 	iPenDownCurVol;
	static UINT PenDownState;
	static BOOL bTouchStart=FALSE;

	if(!m_bSystemReady || !m_bDeviceActivated)
	{
		RETAILMSG(1, (TEXT("System is not ready.\r\n")));
		return;
	}

	if(m_bOnHWMute)
		return;

	pt.x = pCyMultiPoint->sPointCoordinate[0].PointX;
	pt.y = pCyMultiPoint->sPointCoordinate[0].PointY;
	BYTE area = pCyMultiPoint->sPointStatus.toucharea;
	int status = pCyMultiPoint->sPointStatus.pointstatus;
	BOOL bKeyBeep=FALSE;

	//static int pointnum = pCyMultiPoint->sPointStatus.pointnum;

	if(!bTouchStart)
	{
		if((status!=TOUCH_STATUS_DOWN_START && status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
		{
			status = TOUCH_STATUS_DOWN_START;
		}
		bTouchStart = TRUE;
	}

	//reset gesture:
	if((status==TOUCH_STATUS_DOWN_START) || (!bEnabled && status==TOUCH_STATUS_DOWN))//first pen down
	{
		bTouchStart = TRUE;
		ptPenDown = pt;
		bEnabled = TRUE;
		dwPenDownTick=GetTickCount();

		if(m_bPhoneCallActive)
		{
			PenDownState = PENDOWN_STATE_PHONE;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolBluetooth;
		}
		else if(m_bSpeechOn  && (m_bOnSR || IsNaviSpeechEnable()))
		{
			PenDownState = PENDOWN_STATE_SPEECH;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolGPS;
		}
		else
		{
			PenDownState = PENDOWN_STATE_MEDIA;
			iPenDownCurVol = m_pConfig->GetCurParam()->dwVolMedia;
		}

		//save point and status:
		spt = pt;
	}
	else if((status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
	{
		//save point and status:
		spt = pt;
	}

	//RETAILMSG(1,(_T("@@OnTouchMsg: x:%d,y:%d, sx:%d sy:%d status:%x,area:%d\r\n"),pt.x,pt.y,spt.x,spt.y,status,area));

	//GWES window
	if(area == TOUCH_AREA_GFX_WINDOW)
	{
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Down\r\n")));

			//Touch down or move
			if(GetForegroundWindow() != g_hMainWnd && !m_bManualMute && status==TOUCH_STATUS_DOWN_START)
			{
				bKeyBeep=TRUE;
			}

			if(status==TOUCH_STATUS_DOWN_START)
			{
				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchDown(&ptPenDown);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchDown(&ptPenDown);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchDown(&ptPenDown);
				}

			}
		}
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//Touch up
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Up\r\n")));

			if(IsPowerOn())
			{
				//hide smart bar:
				if(m_bBacklightOFF)
				{
					ScreenOn();
					bKeyBeep=TRUE;
				}
				else
				{
					if(!m_bManualMute)
					{
						if(m_pSmartBarVol)
						{
							if(m_pSmartBarVol->IsVisible())
							{
								m_pSmartBarVol->Show(FALSE);
								bKeyBeep=TRUE;
								goto EXIT;
							}
						}
					}

					if(m_pSmartBarMedia)
					{
						if(m_pSmartBarMedia->IsVisible() && GetTickCount()-m_pSmartBarMedia->GetLastShowTick()>1000)
						{
							RETAILMSG(1, (TEXT("##### HIDE SMART BAR MEDIA ######\r\n")));
							m_pSmartBarMedia->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}
					
#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
					//auto unmute:
					if(m_bManualMute && !m_bPhoneCallActive && GetTickCount()-m_dwLastMuteTick>500)
					{
						OnMuteKey(FALSE);
						bKeyBeep=TRUE;
						goto EXIT;
					}
#endif
				}

				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchUp(&spt);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchUp(&spt);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchUp(&spt);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchUp(&spt);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchUp(&spt);
				}
			}
		}

		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_IMAGE)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN));

			if(panel)
			{
				((CPanelImageMain*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
#if CVTE_EN_DTV
		else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_TV)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER));

			if(panel)
			{
				((CPanelTV*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
#endif
#if (CVTE_EN_CAMERA && CVTE_DEF_CAMERA_WIDGET_TYPE == CVTE_DEF_CAMERA_WIDGET_TYPE_360)
		else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_REARVIEW)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN));

			if(panel)
			{
				((CPanelRearView*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
#endif
	}
	//Menu window
	else
	{
#if (CVTE_EN_BORDER_TOUCH == CVTE_DISABLE)
		goto EXIT;
#endif
		//Touch down or move
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: Touch down or move\r\n")));

			if(!bMoving)
			{
				bMoving = (abs(ptPenDown.y-spt.y)>50) && (GetTickCount()-dwPenDownTick>100);
			}

#if USE_NEW_U703PT_TP
			//Silder mute
			if( area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolMute, ptPenDown) && PtInRect(&rcVolMinus, spt) && bEnabled )
			{
				if(IsPowerOn() /*&& !m_bSpeechOn */&& !m_bPhoneCallActive && !m_bManualMute)
				{
					RETAILMSG(DEBUG_GUI, (TEXT("## MUTE KEY ##\r\n")));
					OnMuteKey(TRUE);
				}
			}
			//Power Key
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcPower, ptPenDown) && PtInRect(&rcPower, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("Power Key\r\n")));
				if(!bPowerPenDown && !m_bOnReverseMode)
				{
					bPowerPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_POWER_SCREEN);
				}
			}
			//Phone Key
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcPhone, ptPenDown) && PtInRect(&rcPhone, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("Phone Key\r\n")));
				if(!bPhonePenDown)
				{
					bPhonePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_PHONE_CALL);
				}
			}
			//Home Key
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcHome, ptPenDown) && PtInRect(&rcHome, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("Phone Key\r\n")));
				if(!bHomePenDown)
				{
					bHomePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_HOME);
				}
			}
			//Siri
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcSiri, ptPenDown) && PtInRect(&rcSiri, spt) && bEnabled)
			{
				if(!bSiriPenDown)
				{
					bSiriPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_SPEECH);
				}
			}
			//Vol Plus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolPlus, ptPenDown) && PtInRect(&rcVolPlus, spt) && bEnabled)
			{
				if(!bVolPlusPenDown)
				{
					bVolPlusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_PLUS);
				}
			}
			//Vol minus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolMinus, ptPenDown) && PtInRect(&rcVolMinus, spt) && bEnabled)
			{
				if(!bVolMinusPenDown)
				{
					bVolMinusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_MINUS);
				}
			}
#else
			//Silder mute
			if( area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolPlus, ptPenDown) && PtInRect(&rcVolMinus, spt) && bEnabled )
			{
				RETAILMSG(DEBUG_GUI, (TEXT("## MUTE KEY ##\r\n")));
				if(IsPowerOn() /*&& !m_bSpeechOn */&& !m_bPhoneCallActive && !m_bManualMute)
				{
					OnMuteKey(TRUE);
				}
			}
			//HomePower Key
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcPowerHome, ptPenDown) && PtInRect(&rcPowerHome, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("HomePower Key\r\n")));
				if(!bPowerHomePenDown)
				{
					bPowerHomePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_HARDWARE_HOME);
				}
			}
			//Siri
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcSiri, ptPenDown) && PtInRect(&rcSiri, spt) && bEnabled)
			{
				if(!bSiriPenDown)
				{
					bSiriPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_SPEECH);
				}
			}
			//Vol Plus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolPlus, ptPenDown) && PtInRect(&rcVolPlus, spt) && bEnabled)
			{
				if(!bVolPlusPenDown)
				{
					bVolPlusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_PLUS);
				}
			}
			//Vol minus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolMinus, ptPenDown) && PtInRect(&rcVolMinus, spt) && bEnabled)
			{
				if(!bVolMinusPenDown)
				{
					bVolMinusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_MINUS);
				}
			}
			//Vol manual
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolManual, ptPenDown) && PtInRect(&rcVolManual, spt) && bEnabled)
			{
				if(!bVolManualPenDown)
				{
					bVolManualPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_VOL_MANUAL);
				}
			}
			//Brightness
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcBrightness, ptPenDown) && PtInRect(&rcBrightness, spt) && bEnabled)
			{
				if(!bBrightnessPenDown)
				{
					bBrightnessPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_LUMINACE);
				}
			}
#endif
		}
		//TOUCH_STATUS_UP: Lift off
		//GEST_SINGLE_TOUCH_END: Single-Touch Single Click
		//GEST_SINGLE_TOUCH_DOUBLE_CLICK: Single-Touch Double Click
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
#if USE_NEW_U703PT_TP
			//Power Key
			 if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcPower, ptPenDown) && PtInRect(&rcPower, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("Power Key\r\n")));
				if(bPowerPenDown)
				{
					bPowerPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_POWER_SCREEN);
				}
			}
			//Phone Key
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcPhone, ptPenDown) && PtInRect(&rcPhone, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("Phone Key\r\n")));
				if(bPhonePenDown)
				{
					bPhonePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_PHONE_CALL);
				}
			}
			//Home Key
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcHome, ptPenDown) && PtInRect(&rcHome, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("Phone Key\r\n")));
				if(bHomePenDown)
				{
					bHomePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_HOME);
				}
			}
			//Vol plus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolPlus, ptPenDown) && PtInRect(&rcVolPlus, spt) && bEnabled)
			{
				if(bVolPlusPenDown)
				{
					bVolPlusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS);
				}
			}
			//Vol minus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolMinus, ptPenDown) && PtInRect(&rcVolMinus, spt) && bEnabled)
			{
				if(bVolMinusPenDown)
				{
					bVolMinusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
				}
			}
			//SIRI KEY
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcSiri, ptPenDown) && PtInRect(&rcSiri, spt) && bEnabled)
			{
				if(bSiriPenDown)
				{
					bSiriPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
				}
			}
#else
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: UP\r\n")));
			//Vol plus
			if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolPlus, ptPenDown) && PtInRect(&rcVolPlus, spt) && bEnabled)
			{
				if(bVolPlusPenDown)
				{
					bVolPlusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS);
				}
			}
			//Vol minus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolMinus, ptPenDown) && PtInRect(&rcVolMinus, spt) && bEnabled)
			{
				if(bVolMinusPenDown)
				{
					bVolMinusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
				}
			}
			//Vol manual
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolManual, ptPenDown) && PtInRect(&rcVolManual, spt) && bEnabled)
			{
				if(bVolManualPenDown)
				{
					bVolManualPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_VOL_MANUAL);
				}
			}
			//HOME_POWER KEY
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcPowerHome, ptPenDown) && PtInRect(&rcPowerHome, spt) && bEnabled)
			{
				//RETAILMSG(DEBUG_GUI, (TEXT("## HOME KEY ## IsPowerOn[%d] m_bOnReverseMode[%d] dwPenDownTick[%d] dwTick[%d]\r\n"),IsPowerOn(),m_bOnReverseMode,dwPenDownTick,GetTickCount()));
				if(bPowerHomePenDown)
				{
					bPowerHomePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
				}
			}

			//SIRI KEY
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcSiri, ptPenDown) && PtInRect(&rcSiri, spt) && bEnabled)
			{
				if(bSiriPenDown)
				{
					bSiriPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
				}
			}
			//Brightness
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcBrightness, ptPenDown) && PtInRect(&rcBrightness, spt) && bEnabled)
			{
				if(bBrightnessPenDown)
				{
					bBrightnessPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE);
				}
			}
#endif
		}
	}

EXIT:
	if(bKeyBeep)
	{
		PlaySoundEffect(0);
	}

	//This part use to check the touch area.
#if USE_NEW_U703PT_TP
	if(bPowerPenDown)
	{
		if(!PtInRect(&rcPower,spt))
		{
			bPowerPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_POWER_SCREEN,TRUE);
		}
	}
	else if(bHomePenDown)
	{
		if(!PtInRect(&rcHome,spt))
		{
			bHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_HOME,TRUE);
		}
	}
	else if(bPhonePenDown)
	{
		if(!PtInRect(&rcPhone,spt))
		{
			bPhonePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_PHONE_CALL,TRUE);
		}
	}
	else if(bSiriPenDown)
	{
		if(!PtInRect(&rcSiri,spt))
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH,TRUE);
		}
	}
	else if(bVolPlusPenDown)
	{
		if(!PtInRect(&rcVolPlus,spt))
		{
			bVolPlusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS,TRUE);
		}
	}
	else if(bVolMinusPenDown)
	{
		if(!PtInRect(&rcVolMinus,spt))
		{
			bVolMinusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS,TRUE);
		}
	}
#else
	if(bPowerHomePenDown)
	{
		if(!PtInRect(&rcPowerHome,spt))
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME,TRUE);
		}
	}
	else if(bSiriPenDown)
	{
		if(!PtInRect(&rcSiri,spt))
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH,TRUE);
		}
	}
	else if(bVolPlusPenDown)
	{
		if(!PtInRect(&rcVolPlus,spt))
		{
			bVolPlusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS,TRUE);
		}
	}
	else if(bVolMinusPenDown)
	{
		if(!PtInRect(&rcVolMinus,spt))
		{
			bVolMinusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS,TRUE);
		}
	}
	else if(bVolManualPenDown)
	{
		if(!PtInRect(&rcVolManual,spt))
		{
			bVolManualPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_VOL_MANUAL,TRUE);
		}
	}
	else if(bBrightnessPenDown)
	{
		if(!PtInRect(&rcBrightness,spt))
		{
			bBrightnessPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE,TRUE);
		}
	}
#endif

	if(status==TOUCH_STATUS_UP || status==GEST_SINGLE_TOUCH_END || status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//pen up
	{
#if USE_NEW_U703PT_TP
	if(bPowerPenDown)
	{
		bPowerPenDown = FALSE;
		CKeysHandler::GetInstance()->KeyUp(KEY_PHY_POWER_SCREEN);
	}
	else if(bHomePenDown)
	{
		bHomePenDown = FALSE;
		CKeysHandler::GetInstance()->KeyUp(KEY_STW_HOME);
	}
	else if(bPhonePenDown)
	{
		bPhonePenDown = FALSE;
		CKeysHandler::GetInstance()->KeyUp(KEY_STW_PHONE_CALL);
	}
	else if(bSiriPenDown)
	{
		bSiriPenDown = FALSE;
		CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
	}
	else if(bVolPlusPenDown)
	{
		bVolPlusPenDown = FALSE;
		CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS);
	}
	else if(bVolMinusPenDown)
	{
		bVolMinusPenDown = FALSE;
		CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
	}

#else
		if(bPowerHomePenDown)
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
		}
		else if(bSiriPenDown)
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
		}
		else if(bVolPlusPenDown)
		{
			bVolPlusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS);
		}
		else if(bVolMinusPenDown)
		{
			bVolMinusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
		}
		else if(bVolManualPenDown)
		{
			bVolManualPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_VOL_MANUAL);
		}
		else if(bBrightnessPenDown)
		{
			bBrightnessPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE);
		}
#endif
		//reset:
		bEnabled = FALSE;
		bTouchStart = FALSE;
		spt.x = -1;
		spt.y = -1;
		PenDownState = PENDOWN_STATE_IDLE;
		//RETAILMSG(1,(_T("@OnTouchMsg: Up\r\n")));
	}
}
#elif (CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_DS702P)
void CGUI_Fantasy::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	#define CVTE_DEF_VOL_VALUE_MAX				32

	static const RECT	rcBrightness	= {
		0,60,892,140
	};

	static const RECT	rcSiri	= {
		0,200,892,280
	};

	static const RECT	rcPowerHome = {
		0,350,892,430
	};

	static const RECT	rcVolPlus	= {
		0,60,45,140
	};

	static const RECT	rcVolMinus	= {
		0,200,45,280
	};

	static const RECT	rcMute	= {
		0,350,45,430
	};

	static DWORD dwPenDownTick=0;

#if  CVTE_EN_KEY_PROXY
	static BOOL bPowerHomePenDown = FALSE;
	static BOOL bSiriPenDown = FALSE;
	static BOOL bVolPlusPenDown = FALSE;
	static BOOL bVolMinusPenDown = FALSE;
	static BOOL bMutePenDown = FALSE;
	static BOOL bBrightnessPenDown = FALSE;
#endif

	static BOOL bEnabled=FALSE;

	static POINT ptPenDown;
	static POINT spt;
	static POINT pt;

	static BOOL bTouchStart=FALSE;

	if(!m_bSystemReady || !m_bDeviceActivated)
	{
		RETAILMSG(1, (TEXT("System is not ready.\r\n")));
		return;
	}

	if(m_bOnHWMute)
		return;

	pt.x = pCyMultiPoint->sPointCoordinate[0].PointX;
	pt.y = pCyMultiPoint->sPointCoordinate[0].PointY;
	BYTE area = pCyMultiPoint->sPointStatus.toucharea;
	int status = pCyMultiPoint->sPointStatus.pointstatus;
	BOOL bKeyBeep=FALSE;

	//static int pointnum = pCyMultiPoint->sPointStatus.pointnum;

	if(!bTouchStart)
	{
		if((status!=TOUCH_STATUS_DOWN_START && status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
		{
			status = TOUCH_STATUS_DOWN_START;
		}
		bTouchStart = TRUE;
	}

	//reset gesture:
	if((status==TOUCH_STATUS_DOWN_START) || (!bEnabled && status==TOUCH_STATUS_DOWN))//first pen down
	{
		//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: DOWN Start\r\n")));
		//RETAILMSG(1,(_T("@OnTouchMsg: Down\r\n")));
		bTouchStart = TRUE;
		ptPenDown = pt;
		bEnabled = TRUE;
		dwPenDownTick=GetTickCount();

		spt = pt;
	}
	else if((status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
	{
		//save point and status:
		spt = pt;
	}

	//RETAILMSG(1,(_T("@@OnTouchMsg: x:%d,y:%d, sx:%d sy:%d status:%x,bgfx:%d\r\n"),pt.x,pt.y,spt.x,spt.y,status,bGfx));

	//GWES window
	if(area == TOUCH_AREA_GFX_WINDOW)
	{
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Down\r\n")));

			//Touch down or move
			if(GetForegroundWindow() != g_hMainWnd && !m_bManualMute && status==TOUCH_STATUS_DOWN_START)
			{
				bKeyBeep=TRUE;
			}

			if(status==TOUCH_STATUS_DOWN_START)
			{
				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchDown(&ptPenDown);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchDown(&ptPenDown);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchDown(&ptPenDown);
				}
			}
		}
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//Touch up
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Up\r\n")));
			if(IsPowerOn())
			{
				//hide smart bar:
				if(m_bBacklightOFF)
				{
					ScreenOn();
					bKeyBeep=TRUE;
				}
				else
				{
					if(!m_bManualMute)
					{
						if(m_pSmartBarVol)
						{
							if(m_pSmartBarVol->IsVisible())
							{
								m_pSmartBarVol->Show(FALSE);
								bKeyBeep=TRUE;
								goto EXIT;
							}
						}
					}
					if(m_pSmartBarMedia)
					{
						if(m_pSmartBarMedia->IsVisible() && GetTickCount()-m_pSmartBarMedia->GetLastShowTick()>1000)
						{
							RETAILMSG(1, (TEXT("##### HIDE SMART BAR MEDIA ######\r\n")));
							m_pSmartBarMedia->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}			

#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
					//auto unmute:
					if(m_bManualMute && !m_bPhoneCallActive && GetTickCount()-m_dwLastMuteTick>500)
					{
						OnMuteKey(FALSE);
						bKeyBeep=TRUE;
						goto EXIT;
					}
#endif					
				}

				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchUp(&spt);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchUp(&spt);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchUp(&spt);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchUp(&spt);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchUp(&spt);
				}
			}
		}

		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_IMAGE)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN));

			if(panel)
			{
				((CPanelImageMain*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
		else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_TV)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER));

			if(panel)
			{
				((CPanelTV*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#endif
	}
	//Menu window
	else
	{
#if (CVTE_EN_BORDER_TOUCH == CVTE_DISABLE)
		goto EXIT;
#endif
		//Touch down or move
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//Power Home Key
			if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcPowerHome,ptPenDown) && PtInRect(&rcPowerHome,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bPowerHomePenDown)
				{
					bPowerHomePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_HARDWARE_HOME);
				}
#endif
			}
			//Siri
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcSiri,ptPenDown) && PtInRect(&rcSiri,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bSiriPenDown)
				{
					bSiriPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_SPEECH);
				}
#endif
			}
			//Vol Plus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolPlus,ptPenDown) && PtInRect(&rcVolPlus,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bVolPlusPenDown)
				{
					bVolPlusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_PLUS);
				}
#endif
			}
			//Vol Minus
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcVolMinus,ptPenDown) && PtInRect(&rcVolMinus,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bVolMinusPenDown)
				{
					bVolMinusPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_STW_VOL_MINUS);
				}
#endif
			}
			//Mute = Screen
			else if(area == TOUCH_AREA_MENU_L_WINDOW && PtInRect(&rcMute,ptPenDown) && PtInRect(&rcMute,spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bMutePenDown)
				{
					bMutePenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_MUTE_SCREEN);
				}
#endif
			}
			else if(area == TOUCH_AREA_MENU_R_WINDOW && PtInRect(&rcBrightness,ptPenDown) && PtInRect(&rcBrightness, spt) && bEnabled)
			{
#if CVTE_EN_KEY_PROXY
				if(!bBrightnessPenDown)
				{
					bBrightnessPenDown = TRUE;
					CKeysHandler::GetInstance()->KeyDown(KEY_PHY_LUMINACE);
				}
#endif
			}
		}
		//TOUCH_STATUS_UP: Lift off
		//GEST_SINGLE_TOUCH_END: Single-Touch Single Click
		//GEST_SINGLE_TOUCH_DOUBLE_CLICK: Single-Touch Double Click
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: Menu Up\r\n")));

			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: UP PenDown.x[%d] sx[%d] sy[%d]\r\n"),ptPenDown.x,sx,sy));
			//HOME KEY
			if ( area == TOUCH_AREA_MENU_R_WINDOW && bEnabled && PtInRect(&rcPowerHome,ptPenDown) && PtInRect(&rcPowerHome,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bPowerHomePenDown)
				{
					bPowerHomePenDown=FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
				}
#endif			
			}
			//SIRI KEY
			else if ( area == TOUCH_AREA_MENU_R_WINDOW && bEnabled && PtInRect(&rcSiri,ptPenDown) && PtInRect(&rcSiri,spt) )
			{
#if CVTE_EN_CARPLAY
				if(bSiriPenDown)
				{
					bSiriPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
				}
#endif
			}
			//Vol Plus KEY
			else if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcVolPlus,ptPenDown) && PtInRect(&rcVolPlus,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bVolPlusPenDown)
				{
					bVolPlusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS);
				}
#endif
			}
			//Vol Minus KEY
			else if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcVolMinus,ptPenDown) && PtInRect(&rcVolMinus,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bVolMinusPenDown)
				{
					bVolMinusPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
				}
#endif
			}
			//MUTE KEY
			else if ( area == TOUCH_AREA_MENU_L_WINDOW && bEnabled && PtInRect(&rcMute,ptPenDown) && PtInRect(&rcMute,spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bMutePenDown)
				{
					bMutePenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
				}
#endif
			}
			else if( area == TOUCH_AREA_MENU_R_WINDOW && bEnabled && PtInRect(&rcBrightness,ptPenDown) && PtInRect(&rcBrightness, spt) )
			{
#if CVTE_EN_KEY_PROXY
				if(bBrightnessPenDown)
				{
					bBrightnessPenDown = FALSE;
					CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE);
				}
#endif
			}
		}
	}

EXIT:
	if(bKeyBeep)
	{
		PlaySoundEffect(0);
	}

#if CVTE_EN_KEY_PROXY
	//This part use to check the touch area.
	if(bPowerHomePenDown)
	{
		if(!PtInRect(&rcPowerHome,spt))
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME,TRUE);
		}
	}
	else if(bSiriPenDown)
	{
		if(!PtInRect(&rcSiri,spt))
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH,TRUE);
		}
	}
	else if(bVolPlusPenDown)
	{
		if(!PtInRect(&rcVolPlus,spt))
		{
			bVolPlusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_PLUS,TRUE);
		}
	}
	else if(bVolMinusPenDown)
	{
		if(!PtInRect(&rcVolMinus,spt))
		{
			bVolMinusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS,TRUE);
		}
	}
	else if(bMutePenDown)
	{
		if(!PtInRect(&rcMute,spt))
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN,TRUE);
		}
	}
	else if(bBrightnessPenDown)
	{
		if(!PtInRect(&rcBrightness,spt))
		{
			bBrightnessPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE,TRUE);
		}
	}
#endif

	if(status==TOUCH_STATUS_UP || status==GEST_SINGLE_TOUCH_END || status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//pen up
	{
#if CVTE_EN_KEY_PROXY
		if(bPowerHomePenDown)
		{
			bPowerHomePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_HARDWARE_HOME);
		}
		else if(bSiriPenDown)
		{
			bSiriPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_SPEECH);
		}
		else if(bVolMinusPenDown)
		{
			bVolMinusPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_STW_VOL_MINUS);
		}
		else if(bMutePenDown)
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
		}
		else if(bMutePenDown)
		{
			bMutePenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_MUTE_SCREEN);
		}
		else if(bBrightnessPenDown)
		{
			bBrightnessPenDown = FALSE;
			CKeysHandler::GetInstance()->KeyUp(KEY_PHY_LUMINACE);
		}
#endif

		//reset:
		bEnabled = FALSE;
		bTouchStart = FALSE;
		spt.x = -1;
		spt.y = -1;
		//RETAILMSG(1,(_T("@OnTouchMsg: Up\r\n")));
	}
}
#elif (CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_DS701P)
void CGUI_Fantasy::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{
	static DWORD dwPenDownTick=0;
	static BOOL bEnabled=FALSE;

	static POINT ptPenDown;
	static POINT spt;
	static POINT pt;

	static BOOL bTouchStart=FALSE;

	if(!m_bSystemReady || !m_bDeviceActivated)
	{
		RETAILMSG(1, (TEXT("System is not ready.\r\n")));
		return;
	}

	if(m_bOnHWMute)
		return;

	pt.x = pCyMultiPoint->sPointCoordinate[0].PointX;
	pt.y = pCyMultiPoint->sPointCoordinate[0].PointY;
	BOOL bGfx = (pCyMultiPoint->sPointStatus.toucharea == TOUCH_AREA_GFX_WINDOW);
	int status = pCyMultiPoint->sPointStatus.pointstatus;
	BOOL bKeyBeep=FALSE;

	//static int pointnum = pCyMultiPoint->sPointStatus.pointnum;

	if(!bTouchStart)
	{
		if((status!=TOUCH_STATUS_DOWN_START && status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
		{
			status = TOUCH_STATUS_DOWN_START;
		}
		bTouchStart = TRUE;
	}

	//reset gesture:
	if((status==TOUCH_STATUS_DOWN_START) || (!bEnabled && status==TOUCH_STATUS_DOWN))//first pen down
	{
		//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: DOWN Start\r\n")));
		//RETAILMSG(1,(_T("@OnTouchMsg: Down\r\n")));
		bTouchStart = TRUE;
		ptPenDown = pt;
		bEnabled = TRUE;
		dwPenDownTick=GetTickCount();

		spt = pt;
	}
	else if((status!=TOUCH_STATUS_UP && status!=TOUCH_STATUS_INVALID && status!=GEST_SINGLE_TOUCH_END) && (pt.x != 0 || pt.y != 0))
	{
		//save point and status:
		spt = pt;
	}

	//RETAILMSG(1,(_T("@@OnTouchMsg: x:%d,y:%d, sx:%d sy:%d status:%x,bgfx:%d\r\n"),pt.x,pt.y,spt.x,spt.y,status,bGfx));

	//GWES window
	if(bGfx)
	{
		if(status==TOUCH_STATUS_DOWN_START||(status!=TOUCH_STATUS_INVALID && status!=TOUCH_STATUS_UP && status!=GEST_SINGLE_TOUCH_DOUBLE_CLICK  && status!=GEST_SINGLE_TOUCH_END))//down or move
		{
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Down\r\n")));

			//Touch down or move
			if(GetForegroundWindow() != g_hMainWnd && !m_bManualMute && status==TOUCH_STATUS_DOWN_START)
			{
				bKeyBeep=TRUE;
			}

			if(status==TOUCH_STATUS_DOWN_START)
			{
				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchDown(&ptPenDown);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchDown(&ptPenDown);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchDown(&ptPenDown);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchDown(&ptPenDown);
				}
			}
		}
		else if(status==TOUCH_STATUS_UP||status==GEST_SINGLE_TOUCH_END||status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//up
		{
			//Touch up
			//RETAILMSG(DEBUG_GUI,(_T("@@OnTouchMsg: GFX Up\r\n")));
			if(IsPowerOn())
			{
				//hide smart bar:
				if(m_bBacklightOFF)
				{
					ScreenOn();
					bKeyBeep=TRUE;
				}
				else
				{
					if(!m_bManualMute)
					{
						if(m_pSmartBarVol)
						{
							if(m_pSmartBarVol->IsVisible())
							{
								m_pSmartBarVol->Show(FALSE);
								bKeyBeep=TRUE;
								goto EXIT;
							}
						}
					}
					if(m_pSmartBarMedia)
					{
						if(m_pSmartBarMedia->IsVisible() && GetTickCount()-m_pSmartBarMedia->GetLastShowTick()>1000)
						{
							RETAILMSG(1, (TEXT("##### HIDE SMART BAR MEDIA ######\r\n")));
							m_pSmartBarMedia->Show(FALSE);
							bKeyBeep=TRUE;
							goto EXIT;
						}
					}			

#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
					//auto unmute:
					if(m_bManualMute && !m_bPhoneCallActive && GetTickCount()-m_dwLastMuteTick>500)
					{
						OnMuteKey(FALSE);
						bKeyBeep=TRUE;
						goto EXIT;
					}
#endif					
				}

				if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
				{
					m_pSmartBarMedia->OnTouchUp(&spt);
				}

				if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
				{
					m_pSmartBarPhone->OnTouchUp(&spt);
				}

				if(m_pSmartBarDrop && m_pSmartBarDrop->IsVisible())
				{
					m_pSmartBarDrop->OnTouchUp(&spt);
				}
#if CVTE_EN_CAMERA
				if(m_pRearOverlay && m_pRearOverlay->IsVisible())
				{
					m_pRearOverlay->OnTouchUp(&spt);
				}
#endif
				if(m_pSmartBarTopBanner && m_pSmartBarTopBanner->IsVisible())
				{
					m_pSmartBarTopBanner->OnTouchUp(&spt);
				}
			}
		}

		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_IMAGE)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN));

			if(panel)
			{
				((CPanelImageMain*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
		else if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_TV)
		{
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_TV,PAGE_TV_WRAPPER));

			if(panel)
			{
				((CPanelTV*)panel)->OnTouchMsg(pCyMultiPoint);
			}
		}
		#endif
	}

EXIT:
	if(bKeyBeep)
	{
		PlaySoundEffect(0);
	}


	if(status==TOUCH_STATUS_UP || status==GEST_SINGLE_TOUCH_END || status==GEST_SINGLE_TOUCH_DOUBLE_CLICK)//pen up
	{
		//reset:
		bEnabled = FALSE;
		bTouchStart = FALSE;
		spt.x = -1;
		spt.y = -1;
		//RETAILMSG(1,(_T("@OnTouchMsg: Up\r\n")));
	}
}
#else
void CGUI_Fantasy::OnTouchMsg(PMULTIPOINT pCyMultiPoint)
{

}
#endif
void CGUI_Fantasy::OnMuteKey(BOOL bMute)
{
	static int iSaveMainVolBeforeMute=0;
    static int iSaveMixVolBeforeMute=0;
	static BOOL bIsPlayingBeforeMute = FALSE;

    if (m_bOnHWMute && m_bManualMute)
    {
        RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: Current state is HW Mute. skip!!!\r\n")));
        return;
    }

	if (m_bOnTAMode && bMute)
	{
		RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: Current state is on TA mode. skip!!!\r\n")));
		return;
	}

	m_bManualMute = bMute;
    RETAILMSG(DEBUG_GUI, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: bMute:%d\r\n"),bMute));

//	    RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnMuteKey]: bMute:%d m_iVolumeFadeTo:%d dwVolMedia:%d dwVolGPS:%d iSaveMainVolBeforeMute:%d iSaveMixVolBeforeMute:%d\r\n"),
//	    bMute,
//	    m_iVolumeFadeTo,
//	    m_pConfig->GetCurParam()->dwVolMedia,
//	    m_pConfig->GetCurParam()->dwVolGPS,
//	    iSaveMainVolBeforeMute,
//	    iSaveMixVolBeforeMute));
	//RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: Mute:%d m_bSpeechOn:%d m_bOnSR:%d dwVolGPS:%d dwVolMedia:%d\r\n"),bMute,m_bSpeechOn,m_bOnSR,m_pConfig->GetCurParam()->dwVolGPS,m_pConfig->GetCurParam()->dwVolMedia));

	if(bMute)
	{
		if(m_pConfig->GetCurParam()->dwVolGPS>0)
		{
			iSaveMixVolBeforeMute=m_pConfig->GetCurParam()->dwVolGPS;
		}
		else
		{
			iSaveMixVolBeforeMute=VOLUME_DEFAULT_NORMAL_VALUE;//default recover volume
		}

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec && (m_bSpeechOn || m_bOnSR))
		{
			m_pAudioCodec->SetMixerOn(FALSE);
		}
#else
        //Add by wuhaoheng
        //m_pConfig->GetCurParam()->dwVolGPS = 0;
        if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,0/*m_pConfig->GetCurParam()->dwVolGPS*/);
		}
#endif
		//if(m_iVolumeFadeTo != 0)
		{
			if(m_pConfig->GetCurParam()->dwVolMedia>0)
			{
				iSaveMainVolBeforeMute=m_pConfig->GetCurParam()->dwVolMedia;
			}
			else
			{
				iSaveMainVolBeforeMute=VOLUME_DEFAULT_NORMAL_VALUE;//default recover volume
			}

            //Add by wuhaoheng
            //m_pConfig->GetCurParam()->dwVolMedia = 0;
            //m_bAutoMute = TRUE;

			//SetMainVolume(m_pConfig->GetCurParam()->dwVolMedia,FALSE,TRUE,FALSE);
            SetVolume(0/*m_pConfig->GetCurParam()->dwVolMedia*/,FALSE,TRUE,FALSE);
		}
	}
	else
	{
        if(m_bSpeechOn && !m_bOnSR)
        {
            if(iSaveMainVolBeforeMute)
            {
        		int iVolume=0;
        		if(m_bPhoneCallActive)
        		{
        			iVolume = m_pConfig->GetCurParam()->dwVolBluetooth;
			        RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: m_bPhoneCallActive :%d\r\n"),iVolume));
        		}
        		else if(m_bSpeechOn/* && (IsNaviSpeechEnable())*/)
        		{
        			iVolume = min(5,iSaveMainVolBeforeMute);
			        RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: m_bSpeechOn :%d\r\n"),iVolume));
        		}
        		else
        		{
        			if(m_bOnReverseMode)
        			{
                        if(m_pConfig->GetCurParam()->dwVolMedia>0)
                        {
							#if CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
							if(m_pConfig->GetCurParam()->bRearVolControl)
							{
								iVolume = min(REARVIEW_VOLUME_DEFAULT,iSaveMainVolBeforeMute);
							}
							else
							{
								iVolume = iSaveMainVolBeforeMute;
							}
							#else
        				    iVolume = min(REARVIEW_VOLUME_DEFAULT,iSaveMainVolBeforeMute);
							#endif
                        }
                        else
                        {
                            iVolume = 1;
                        }
        			}
        			else
        			{
        				iVolume = iSaveMainVolBeforeMute;
			            RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: m_bSpeechOn :%d\r\n"),iVolume));
        			}

        		}
			    RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: SetVolume :%d\r\n"),iVolume));

    			//m_pConfig->GetCurParam()->dwVolMedia= iVolume;
                SetVolume(iVolume/*m_pConfig->GetCurParam()->dwVolMedia*/,FALSE,TRUE,FALSE);
            }
            else
            {
			    RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: iSaveMainVolBeforeMute :%d\r\n"),iSaveMainVolBeforeMute));
            }

            if(iSaveMixVolBeforeMute)
            {
    			//m_pConfig->GetCurParam()->dwVolGPS = iSaveMixVolBeforeMute;
                SetVolume(m_pConfig->GetCurParam()->dwVolGPS,FALSE,TRUE,FALSE,TRUE,FALSE);
            }

#if USE_WAV_ONCHIP
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)	
			if (m_pAudioCodec)
			{
				m_pAudioCodec->SetMixerOn(TRUE);
			}
#else
            if(m_pCommander)
            {
                //m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,m_pConfig->GetCurParam()->dwVolGPS);
                m_pCommander->SendCmd(_ARM_MIX_WAV_KEY,1);
            }
#endif
#endif

        }
        else
        {
            if(iSaveMixVolBeforeMute)
            {
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)	
				if (m_pAudioCodec)
				{
					m_pAudioCodec->SetAltVolume(m_pConfig->GetCurParam()->dwVolGPS);
				}
#else
    			//m_pConfig->GetCurParam()->dwVolGPS = iSaveMixVolBeforeMute;
    			//SetMixVolume(m_pConfig->GetCurParam()->dwVolGPS,FALSE,TRUE,FALSE);
                //SetVolume(m_pConfig->GetCurParam()->dwVolGPS,FALSE,TRUE,FALSE,TRUE,FALSE);
                if(m_pCommander)
    			{
    				m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,m_pConfig->GetCurParam()->dwVolGPS);
    			}
#endif
            }

    		if(iSaveMainVolBeforeMute)
    		{
                m_bAutoUnMute = TRUE;

				if(m_bOnSR)
				{
					SetVolume(m_pConfig->GetCurParam()->dwVolGPS,TRUE,TRUE,FALSE);
				}
				else
				{
					if(m_bOnReverseMode)
					{
						int iVolume=0;
						if(m_pConfig->GetCurParam()->dwVolMedia>0)
						{
#if CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
							if(m_pConfig->GetCurParam()->bRearVolControl)
							{
								iVolume = min(REARVIEW_VOLUME_DEFAULT,iSaveMainVolBeforeMute);
							}
							else
							{
								iVolume = iSaveMainVolBeforeMute;
							}
#else
							iVolume = min(REARVIEW_VOLUME_DEFAULT,iSaveMainVolBeforeMute);
#endif
						}
						else
						{
							iVolume = 1;
						}

						//m_pConfig->GetCurParam()->dwVolMedia= iVolume;
						RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnMuteKey]: SetVolume :%d\r\n"),iVolume));
						SetVolume(iVolume/*m_pConfig->GetCurParam()->dwVolMedia*/,FALSE,TRUE,FALSE);
					}
					else
					{
						if(m_pConfig->GetCurParam()->dwVolMedia<=1)
							SetVolume(m_pConfig->GetCurParam()->dwVolMedia,FALSE,TRUE,FALSE);
						else
							SetVolume(m_pConfig->GetCurParam()->dwVolMedia,TRUE,TRUE,FALSE);
					}

				}
            }
        }
	}

	//Mark 20160629 Under the mode of siri not any operation.
	if(!m_bOnSR)
	{
		//pause/play media:
		UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
		if(panel)
		{
			if(idClass != UI_CLASS_RADIO)
			{
#if CVTE_EN_KEY_PROXY
				if (bMute)
				{
					if (idClass == UI_CLASS_MEDIA)
					{
						bIsPlayingBeforeMute = m_pMediaWrapper?m_pMediaWrapper->IsPlaying():TRUE;
					}
					else if (idClass == UI_CLASS_IPOD)
					{
						bIsPlayingBeforeMute = m_piPodWrapper?m_piPodWrapper->IsPlaying():TRUE;
					}
					else if(idClass == UI_CLASS_BT)
					{
						bIsPlayingBeforeMute = m_pBluetoothWrapper?m_pBluetoothWrapper->IsPlaying():TRUE;
					}
#if CVTE_EN_ANDROID_AUTO
					else if (idClass == UI_CLASS_ANDROID_AUTO)
					{
						CPanelGL *panel_aap=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
						if(panel_aap)
						{
							bIsPlayingBeforeMute = ((CPanelAndroidAuto*)panel_aap)->IsPlaying();
						}
					}
#endif
					else
					{
						bIsPlayingBeforeMute = TRUE;
					}
					RETAILMSG(DEBUG_GUI, (TEXT("MSG:[%s]: #Mute# bIsPlayingBeforeMute:%d \r\n"), TEXT(__FUNCTION__),bIsPlayingBeforeMute));

					CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PAUSE);
					CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PAUSE);
				}
				else
				{
					RETAILMSG(DEBUG_GUI, (TEXT("MSG:[%s]: #UnMute# bIsPlayingBeforeMute:%d \r\n"), TEXT(__FUNCTION__),bIsPlayingBeforeMute));

					if (bIsPlayingBeforeMute)
					{
						CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PLAY);
						CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PLAY);
					}
				}
#else
				panel->OnKey(/*KEY_MEDIA_PLAY_PAUSE*/bMute?KEY_MEDIA_PAUSE:KEY_MEDIA_PLAY);
#endif
			}
		}
	}

	#if CVTE_EN_LED
    if(m_pLED)
    {
        m_pLED->SetLedState(bMute?LED_MUTE:LED_UNMUTE);
    }
	#endif

	//We show mute icon if we call OnMuteKey
	if(m_pSmartBarVol)
	{
		m_pSmartBarVol->SetMute(bMute);
	}
}
void CGUI_Fantasy::OnSpeechStart(DWORD dwTimeOut,DWORD dwDuckStatus)
{
	static DWORD idUnderSpeechSRC=m_pConfig->GetCurParam()->idCurSRC;
	static BOOL bFirstSwitch=TRUE;
	DWORD dwElapsed = 0;
	DWORD dwTick = 0;
	//RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart]: dwDuckStatus=%d, tick=%d\r\n"),dwDuckStatus,GetTickCount()));

	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSpeechStart] :%d --%d,idUnderSpeechSRC:%d m_bPhoneCallActive:%d\r\n"),dwTimeOut,GetTickCount(),idUnderSpeechSRC,m_bPhoneCallActive));
#if !USE_WAV_ONCHIP
	if(dwTimeOut>0)
	{
		if(m_bSpeechOn)
		{
			m_dwSpeechTimeOut +=( dwTimeOut);
			dwTick = GetTickCount();
			dwElapsed = dwTick-m_dwSpeechStartTick;
			m_dwSpeechStartTick = dwTick;
		}
		else
		{
			m_dwSpeechTimeOut = 500 + (dwTimeOut);
		}

		if(m_dwSpeechTimeOut>dwElapsed)
			m_dwSpeechTimeOut -= dwElapsed;

		if(m_bWaitingSpeechTerminate)
		{
			KillTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE);
			m_bWaitingSpeechTerminate = FALSE;
		}
		if(!SetTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE,m_dwSpeechTimeOut+500,NULL))
		{
			RETAILMSG(DEBUG_GUI, (TEXT("MSG: [CGUI_Fantasy::OnSpeechStart]: SetTimer FAILED! :%d--- \r\n"),TIMER_SPEECH_TERMINATE));
		}
		else
		{
			m_bWaitingSpeechTerminate = TRUE;
			//RETAILMSG(DEBUG_GUI, (TEXT("MSG: [CGUI_Fantasy::OnSpeechStart]: Speech will be end after :%d--- \r\n"),m_dwSpeechTimeOut));
		}
	}
	else
	{
		m_dwSpeechTimeOut = 500 + (dwTimeOut);
	}
#endif

	//
	if(m_bSpeechOn && idUnderSpeechSRC==m_pConfig->GetCurParam()->idCurSRC)
	{
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
#else
		//only update background media volume:
		if(dwDuckStatus !=-1 /*&& !m_bPhoneCallActive*/ && !m_bManualMute)
		{
#if USE_WAV_ONCHIP
			if(m_pCommander)
			{
				BYTE duckVol = m_pConfig->GetCurParam()->dwVolMedia;
				if(m_bPhoneCallActive)
					duckVol = m_pConfig->GetCurParam()->dwVolBluetooth;
				else if(dwDuckStatus == 0) //no media volume:
					duckVol = 0;
				else if(dwDuckStatus == 2) // no duck media:
					duckVol = m_pConfig->GetCurParam()->dwVolMedia;
				else
					duckVol = min(8,min(m_pConfig->GetCurParam()->dwVolMedia, m_pConfig->GetCurParam()->dwVolGPS*6/10)); //default, duck to dwVolGPS 60%
				m_pCommander->SendCmd(_ARM_VOLUME_KEY,duckVol);
				RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart]: update main volume= %d\r\n"),duckVol));
			}
#endif
		}
#endif
		return;
	}

	m_bSpeechOn = TRUE;
	//
	idUnderSpeechSRC=m_pConfig->GetCurParam()->idCurSRC;
	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart]: m_bSpeechOn[%d]: dwTimeOut[%d] ,idUnderSpeechSRC[0x%x] m_dwCallStatus:%d\r\n"),m_bSpeechOn,dwTimeOut,idUnderSpeechSRC,m_dwCallStatus));

#if !USE_WAV_ONCHIP
	m_dwSpeechStartTick = GetTickCount();
#endif

	//process panels operation:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnSpeechStart();
		m_listPanels.Next();
	}

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	//switch to speech:
	m_idAudioSRCBeforeSpeech = m_idCurAudioSRC;

	if(m_pAudioCodec && !m_bManualMute)
	{
		m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolMedia);
		m_pAudioCodec->RequestAudioBus(SRC_AUDIO_SPEECH);
		m_pAudioCodec->SetAltVolume(m_pConfig->GetCurParam()->dwVolGPS);
		m_iVolumeFadeTo = m_pAudioCodec->GetAltVolume();
		m_pAudioCodec->MuteAltAudio(FALSE);
		m_pAudioCodec->SetMixerOn(TRUE,(dwDuckStatus==0)?TRUE:FALSE);
	}
#else
    if(/*!m_bPhoneCallActive && */!m_bManualMute)
    {
#if USE_WAV_ONCHIP
        if(m_pCommander)
        {
//			if(m_idMuteType == 0)
// 			{
// 				m_idMuteType = _ARM_MUTE_ALL;
// 				m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+0);
// 				RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart]: unmute _ARM_MUTE_ALL\r\n")));
// 			}
			DoMute(FALSE);
	        RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSpeechStart]: dwVolGPS[%d]: \r\n"),m_pConfig->GetCurParam()->dwVolGPS));
            m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,m_pConfig->GetCurParam()->dwVolGPS);
            m_pCommander->SendCmd(_ARM_MIX_WAV_KEY,1);
			BYTE duckVol = m_pConfig->GetCurParam()->dwVolMedia;
			if(m_bPhoneCallActive)
				duckVol = m_pConfig->GetCurParam()->dwVolBluetooth;
			else if(dwDuckStatus == 0) //no media volume:
				duckVol = 0;
			else if(dwDuckStatus == 2) // no duck media:
				duckVol = m_pConfig->GetCurParam()->dwVolMedia;
			else
				duckVol = min(8,min(m_pConfig->GetCurParam()->dwVolMedia, m_pConfig->GetCurParam()->dwVolGPS*6/10)); //default, duck to dwVolGPS 60%
			m_pCommander->SendCmd(_ARM_VOLUME_KEY,duckVol);
			RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart]: set main volume= %d\r\n"),duckVol));
        }
#else
		//switch to speech:
		m_idAudioSRCBeforeSpeech = m_idCurAudioSRC;

		UINT audio_src=SRC_AUDIO_ARM;
		if(SwitchAudioSRC(audio_src))
		{
	        RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart] :1\r\n")));

// 			Sleep(20);
// 			Mute(FALSE,bFirstSwitch?TRUE:FALSE);
// 			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,((bFirstSwitch?0:1)<<24)+(1<<16)+(200));
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,((1)<<24)+(1<<16)+(300));
		}
		else
		{
	        RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart] :2\r\n")));
			if(IsMediaSRCSupportDucking(m_pConfig->GetCurParam()->idCurSRC))
				SetVolume(BYTE(m_pConfig->GetCurParam()->dwVolGPS),TRUE);//SetMainVolume(BYTE(m_pConfig->GetCurParam()->dwVolGPS),TRUE);
			else if(m_bCarPlayConnected)
				OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,((1)<<24)+(1<<16)+(100));
			else
				OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,((1)<<24)+(1<<16)+(300));
		}

		bFirstSwitch = FALSE;
#endif
// 		//change volume:
// 		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC) != UI_CLASS_MEDIA
// 			&& GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC) != UI_CLASS_BT)
// 		{
// 			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(100));
// 		}
// 		else
// 		{
// 			SetMainVolume(BYTE(m_pConfig->GetCurParam()->dwVolGPS),/*FALSE*/TRUE);
// 		}
	}
// 	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechStart]: \r\n")));
#endif
}
void CGUI_Fantasy::OnSpeechTerminate()
{
	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSpeechTerminate]: %d\r\n"),GetTickCount()));

	if(!m_bSpeechOn)
		return;

	m_bSpeechOn = FALSE;

	m_dwSpeechTimeOut = 0;
    m_dwLastSpeechTick = GetTickCount();

#if USE_WAV_ONCHIP
	if(m_bWaitingSpeechTerminate)
	{
		KillTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE);
		m_bWaitingSpeechTerminate = FALSE;
	}
#endif
	//process panels operation:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnSpeechTerminate();
		m_listPanels.Next();
	}

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)	
	if (m_bManualMute)
	{
		SetTimer(g_hUIWnd, TIMER_AUTO_RELEASE_SPEECH, 500, NULL);
	}
	else
	{
		if(m_pAudioCodec)
		{
			m_pAudioCodec->SetMixerOn(FALSE);
			m_pAudioCodec->ReleaseAudioBus(SRC_AUDIO_SPEECH);
		}

		if(m_bPhoneCallActive)
		{
			SetVolume(m_pConfig->GetCurParam()->dwVolBluetooth,FALSE);
		}
		else if(!m_bOnSR && !m_bSpeechOn)
		{
			SetVolume(m_pConfig->GetCurParam()->dwVolMedia,FALSE);
		}
	}


	//waveOutSetVolume(0,0);
	if(m_pAudioCodec && !(m_bAltAudioDataActived && m_bAltAudioDmaActived))
		m_pAudioCodec->MuteAltAudio(TRUE);
#else
	/*
	//change volume:
	if(!m_bPhoneCallActive)
	{
// 		SetMainVolume(BYTE(m_pConfig->GetCurParam()->dwVolMedia),TRUE);

		if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC) != UI_CLASS_MEDIA
			&& GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC) != UI_CLASS_BT)
		{
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(200));
		}
		else
		{
			SetMainVolume(BYTE(m_pConfig->GetCurParam()->dwVolMedia),TRUE);
		}
	}
	*/

	//if(!m_bPhoneCallActive)
	{
#if USE_WAV_ONCHIP
    	if(m_pCommander)
    	{
    		m_pCommander->SendCmd(_ARM_MIX_WAV_KEY,0);
    	}
#endif

		//switch back to current media src:
		UINT idSRC=SRC_AUDIO_ARM;
		switch (GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC))
		{
		case UI_CLASS_RADIO:
			idSRC=SRC_AUDIO_RADIO;
			break;
		case UI_CLASS_DAB:
			idSRC=SRC_AUDIO_DAB;
			break;
		case UI_CLASS_TV:
			idSRC=SRC_AUDIO_TV;
			break;
		case UI_CLASS_AUX:
			idSRC=SRC_AUDIO_AUX1;
			break;
        case UI_CLASS_DISK:
			idSRC=SRC_AUDIO_DVD;
			break;
        default:
            idSRC=SRC_AUDIO_ARM;
            break;
		}

		//if phone-call or siri is active, switch to SRC_AUDIO_ARM
		if(m_bPhoneCallActive || m_bOnSR)
		{
			idSRC = SRC_AUDIO_ARM;
		}

        if(GetAudioSRC() == idSRC)
        {
            if(!IsNaviSpeechEnable())
            {
				//Mark: This part is nessary whh
				SwitchAudioSRC(idSRC,TRUE);
                RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: CurPanelID:%d return\r\n"),GET_PANEL_CLASS_ID(m_pCurPanel->GetID())));
                return;
            }
        }

		//Mark: Here do not use SwitchAudioSRC(idSRC,TRUE)
		//BUG <0002867> 任意源和导航同时运行，导航播报完提示音后，主音源声音卡顿一下
		if(SwitchAudioSRC(idSRC))
		{
			if(!m_bManualMute)
			{
            	RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: 1->OnMuteMsg \r\n")));
				OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(600));
			}
			else
			{
                SetVolume(0,FALSE,TRUE,FALSE);
                RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: 1->SetMainVolume 0\r\n")));
			}
		}
		else
		{
            //RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: 2->m_bManualMute:%d \r\n"),m_bManualMute));

			if(!m_bManualMute)
			{
				if(m_bPhoneCallActive)
				{
					RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: 2->SetMainVolume %d\r\n"),m_pConfig->GetCurParam()->dwVolBluetooth));
					SetVolume(BYTE(m_pConfig->GetCurParam()->dwVolBluetooth),FALSE/*TRUE*/);
				}
				else if(IsMediaSRCSupportDucking(m_pConfig->GetCurParam()->idCurSRC))
				{
                    RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: 2->SetMainVolume %d\r\n"),m_pConfig->GetCurParam()->dwVolMedia));
                    SetVolume(BYTE(m_pConfig->GetCurParam()->dwVolMedia),FALSE/*TRUE*/);
				}
                else
                {
                    RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: 2->SetVolume %d \r\n"),m_pConfig->GetCurParam()->dwVolMedia));
                    SetVolume(BYTE(m_pConfig->GetCurParam()->dwVolMedia),FALSE/*TRUE*/);
                }
			}
			else
			{
                SetVolume(0,FALSE,TRUE,FALSE);
                RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechTerminate]: 2->SetMainVolume 0\r\n")));
			}
		}
	}
#endif
}

void CGUI_Fantasy::OnTimerReleaseSpeech()
{
	if(!m_bManualMute)
	{
		RETAILMSG(1, (TEXT("MSG:[%s]: Auto releasa audio bus.\r\n"),TEXT(__FUNCTION__)));
		KillTimer(g_hUIWnd,TIMER_AUTO_RELEASE_SPEECH);
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			m_pAudioCodec->ReleaseAudioBus(SRC_AUDIO_SPEECH);
		}
		if(m_bPhoneCallActive)
		{
			SetVolume(m_pConfig->GetCurParam()->dwVolBluetooth,FALSE);
		}
		else if(!m_bOnSR && !m_bSpeechOn)
		{
			SetVolume(m_pConfig->GetCurParam()->dwVolMedia,FALSE);
		}
#endif
	}
}

BOOL CGUI_Fantasy::IsMediaSRCSupportDucking(UINT idSRC)
{
	switch (GET_PANEL_CLASS_ID(idSRC))
	{
	case UI_CLASS_MEDIA:
    case UI_CLASS_CARPLAY:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

void CGUI_Fantasy::OnSRMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSRMsg]: ++ wParam:%d,lParam:%d\r\n"),wParam,lParam));

	m_bOnSR = wParam;

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NAVI,0));

	if(wParam)
	{
		if(panel)
		{
			((CPanelApp*)panel)->OnSRMsg(wParam,lParam);
		}

		if(!IsPowerOn())
		{
			OnSystemPrePowerState(SYS_POWER_ON);
			OnSystemPowerState(SYS_POWER_ON);
			m_bManualPowerOn = TRUE;
		}

 		//turn on backlight
 		if(m_bBacklightOFF)
 		{
            ScreenOn();
 		}

		OnSRStart();
	}
	else
	{
		OnSRTerminate();

		if(panel)
		{
			((CPanelApp*)panel)->OnSRMsg(wParam,lParam);
		}
	}
}


void CGUI_Fantasy::OnSRStart()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG:GUI:Main: [CGUI_Fantasy::OnSRStart] ++\r\n")));

	UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);

	m_bSpeechOn = TRUE;

	//Mute(TRUE,FALSE);

    //m_bSpeechOn = TRUE;
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnSRStart();
		m_listPanels.Next();
	}

	if(idClass == UI_CLASS_MEDIA || idClass == UI_CLASS_BT/* || idClass == UI_CLASS_IPOD*/)
	{
		m_bMediaPlayingBeforeSR = OnGetPlayingStatus();
		m_idMediaClassBeforSR = idClass;
		if(m_bMediaPlayingBeforeSR)
		{
			#if CVTE_EN_KEY_PROXY
			CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_STOP);
			CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_STOP);
			#else
			OnMediaKey(KEY_MEDIA_STOP);			
			#endif
		}
	}

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	if (m_pAudioCodec)
	{
		//OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(600));
		m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolGPS,TRUE);
		m_pAudioCodec->MuteMainAudio(TRUE);
		m_pAudioCodec->RequestAudioBus(SRC_AUDIO_SR);
		m_iVolumeFadeTo = m_pAudioCodec->GetMainVolumeTarget();
		m_pAudioCodec->ChangeAspWorkMode(AspModeSR);
	}
#else
	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSRStart]: -- SwitchAudioSRC SRC_AUDIO_ARM\r\n")));
    //if(GetAudioSRC() != SRC_AUDIO_ARM)
    {
        //SetMainVolume(0,FALSE,FALSE);
        SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
        //SetMainVolume(m_pConfig->GetCurParam()->dwVolGPS,FALSE,FALSE);
        //Whh 20160523 Don't need mute operation here
        //OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,((1)<<24)+(1<<16)+(50));
    }
#endif

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
	if(m_pAudioCodec)
	{
		m_pAudioCodec->ChangeAspWorkMode(AspModeSR);
	}
#endif


	//mute audio, and not release mute until siri audio is connected
	//Mute(TRUE,FALSE);

}
void CGUI_Fantasy::OnSRTerminate()
{
	RETAILMSG(DEBUG_GUI,(_T("MSG:GUI:Main: [CGUI_Fantasy::OnSRTerminate] ++\r\n")));

	UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
    m_bSpeechOn = FALSE;

	//process panels operation:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnSRTerminate();
		m_listPanels.Next();
	}

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();

	if(m_pAudioCodec)
	{		
		if(m_bManualMute)
		{
			m_pAudioCodec->SetMainVolume(0);
		}
		else
		{
			if (m_bPhoneCallActive)
			{
				m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolBluetooth);
			}
			else
			{
				m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolMedia);		
			}
		}

		m_iVolumeFadeTo = m_pAudioCodec->GetMainVolumeTarget();
		if(mode->mainAudio != kAirPlayEntity_Controller)
		{
			m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,100);
			m_pAudioCodec->ReleaseAudioBus(SRC_AUDIO_SR);
		}
		else
		{
			m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,100);
			m_pAudioCodec->ReleaseAudioBus(SRC_AUDIO_SR);
		}
		m_pAudioCodec->ChangeAspWorkMode(AspModeMedia);
	}

	if(!m_bPhoneCallActive)
	{
		if(m_bManualMute)
		{
			SetVolume(0,TRUE,TRUE,FALSE);
		}
		else
		{
			//pop noise is ordinarily caused by hardware MUTE/UNMUTE
			//operation, if want to maximumly reduce pop noise,
			//you need to use this HARDWARE mute/un-mute operation
			//very carefully!
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(600));
		}

		if((idClass == UI_CLASS_MEDIA || idClass == UI_CLASS_BT || idClass == UI_CLASS_IPOD) &&
			idClass == m_idMediaClassBeforSR && m_bMediaPlayingBeforeSR && !m_bPhoneCallActive)
		{
			CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_RESUME);
			CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_RESUME);
		}
	}
#else
    if(!m_bPhoneCallActive)
    {
		//switch back to current media src:
		UINT idSRC=SRC_AUDIO_ARM;
		switch (GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC))
		{
		case UI_CLASS_RADIO:
			idSRC=SRC_AUDIO_RADIO;
			break;
		case UI_CLASS_DAB:
			idSRC=SRC_AUDIO_DAB;
			break;
		case UI_CLASS_TV:
			idSRC=SRC_AUDIO_TV;
			break;
		case UI_CLASS_AUX:
			idSRC=SRC_AUDIO_AUX1;
			break;
        case UI_CLASS_DISK:
			idSRC=SRC_AUDIO_DVD;
			break;
        default:
            idSRC=SRC_AUDIO_ARM;
            break;
		}

        //if(GetAudioSRC() != idSRC)
		SwitchAudioSRC(idSRC, TRUE); //20160523 Auto mute
		if(!m_bManualMute)
		{
			//pop noise is ordinarily caused by hardware MUTE/UNMUTE
			//operation, if want to maximumly reduce pop noise,
			//you need to use this HARDWARE mute/un-mute operation
			//very carefully!
			//OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(100));
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(600));
		}
		else
		{
            SetVolume(0,TRUE,TRUE,FALSE);
		}

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
		if(m_pAudioCodec)
		{
			m_pAudioCodec->ChangeAspWorkMode(AspModeMedia);
		}
#endif

		if((idClass == UI_CLASS_MEDIA || idClass == UI_CLASS_BT || idClass == UI_CLASS_IPOD) &&
			idClass == m_idMediaClassBeforSR && m_bMediaPlayingBeforeSR && !m_bPhoneCallActive)
		{
			#if CVTE_EN_KEY_PROXY
			CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_RESUME);
			CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_RESUME);
			#else
			OnMediaKey(KEY_MEDIA_PLAY);
			#endif
		}
    }
#endif
	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSRTerminate]: --\r\n")));
}

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418 || CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_AK7738)
void CGUI_Fantasy::OnSpeechMsg(WPARAM wParam, LPARAM lParam)
{
	int type=(wParam>>30);
	int para=(wParam&0x3FFFFFFF);

	if(!IsPowerOn())
		return;

	RETAILMSG(1,(_T("MSG:[%s]: OnSpeechMsg  type=%d, para=%d, lParam=0x%x\r\n\r\n"),TEXT(__FUNCTION__),type,para,lParam));

	//type: 1-carplay audio, 0-other audio, 2-audio DMA status, 3=android speech
	if(type==SPEECH_MSG_TYPE_ANDROID_AUTO)
	{
		if(LOWORD(lParam))
		{
			if(m_bWaitingSpeechTerminate)
			{
				KillTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE);
				m_bWaitingSpeechTerminate = FALSE;
			}
			OnSpeechStart(0,HIWORD(lParam));
		}
		else
		{
			OnSpeechTerminate();
		}
	}
	else if(type==SPEECH_MSG_TYPE_DMA_STATUS)
	{
		if(m_bAndroidAutoConnect && m_bAndroidAutoVoiceActicve)
			return;

		m_bAltAudioDmaActived = para;

		m_bSpeechSignaled = para;

		if(!para)
		{
			if(m_bSpeechOn && !m_bOnSR)
			{
				//OnSpeechTerminate();
				//terminate after 500ms
				m_bWaitingSpeechTerminate = TRUE;
				SetTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE,500,NULL);
			}
		}
	}
	else if(type==SPEECH_MSG_TYPE_CARPLAY)//carplay alt audio
	{
		if(m_bAndroidAutoConnect && m_bAndroidAutoVoiceActicve)
			return;

		if(para)
		{
			if(IsNaviSpeechEnable())
			{
				OnSpeechStart(0);
			}
		}
		else
		{
			if(m_bSpeechOn && !m_bOnSR)
			{
				//OnSpeechTerminate();
				//terminate after 500ms
				m_bWaitingSpeechTerminate = TRUE;
				SetTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE,500,NULL);
			}
		}
	}
	else if(!(m_bAndroidAutoConnect && m_bAndroidAutoVoiceActicve))//other native alt audio
	{
		if(para)
		{
			DWORD dwElapsed = 0;
			DWORD dwTick = 0;

			m_bSpeechSignaled = TRUE;

			if(!IsNaviSpeechEnable())
				return;

			if(m_bSpeechOn)
			{
				m_dwSpeechTimeOut +=( para);
				dwTick = GetTickCount();
				dwElapsed = GetTickCount()-m_dwSpeechStartTick;
				m_dwSpeechStartTick = dwTick;
			}
			else
			{
				m_dwSpeechTimeOut = 500 + (para);
			}

			if(m_dwSpeechTimeOut>dwElapsed)
				m_dwSpeechTimeOut -= dwElapsed;

			m_bWaitingSpeechTerminate = TRUE;

			SetTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE,m_dwSpeechTimeOut+500,NULL);

			OnSpeechStart(0);
		}
	}
}
#else
void CGUI_Fantasy::OnSpeechMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnSpeechMsg]: wParam:0x%x,lParam:0x%x, phone:%d, aap=%d, aap_voice=%d\r\n"),
		wParam,lParam,m_bPhoneCallActive,m_bAndroidAutoConnect,m_bAndroidAutoVoiceActicve));

	int type=(wParam>>30);
	int para=(wParam&0x3FFFFFFF);

	if(type != SPEECH_MSG_TYPE_ANDROID_AUTO)
		m_bSpeechSignaled = para;

	/*
	if(!IsNaviSpeechEnable())
	{
		return;
	}
	*/

	if(IsPowerOn())
	{
		//type: 0-other audio,  3=android speech
		if(type == SPEECH_MSG_TYPE_ANDROID_AUTO)
		{
			if(LOWORD(lParam))
			{
				m_iSpeechMsgType = type;
				OnSpeechStart(0,HIWORD(lParam));
			}
			else
			{
				//reset type:
				m_iSpeechMsgType = SPEECH_MSG_TYPE_UNKNOWN;
				if(!m_bSpeechSignaled || !IsNaviSpeechEnable())
					OnSpeechTerminate();
			}
		}
		//if andriod auto voice is active, do not use following process,
		//speech message will be handled by above case of (type == SPEECH_MSG_TYPE_ANDROID_AUTO)
		else if(!(m_bAndroidAutoConnect && m_bAndroidAutoVoiceActicve))
		{

		if(para)
		{
            if(!m_bPhoneCallActive && !m_bManualMute && !m_bOnHWMute && IsNaviSpeechEnable())
            {
    			//need fixed:
    			//if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC) != UI_CLASS_CARPLAY)
    			#if CVTE_EN_CARPLAY
    			const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
    			if(!(m_bCarPlayConnected && (m_bCarPlayAudioOn || (mode->phoneCall==kAirPlayEntity_Controller) || (mode->speech.entity==kAirPlayEntity_Controller))))
    			{
                    OnSpeechStart(para);
    			}
				#endif
            }
		}
		else
		{
			if(lParam)
			{
                if(!m_bPhoneCallActive && !m_bManualMute && !m_bOnHWMute && IsNaviSpeechEnable())
				{
                    //RETAILMSG(DEBUG_GUI,(_T("APP:MSG:CGUI_Fantasy::OnSpeechMsg: OnSpeechStart(0)\r\n")));
					OnSpeechStart(0);

					//kill wait speech terminating...
					if(m_bWaitingSpeechTerminate)
					{
						KillTimer(g_hUIWnd,TIMER_SPEECH_TERMINATE);
						m_bWaitingSpeechTerminate = FALSE;
					}
				}
			}
			else
			{
                //RETAILMSG(DEBUG_GUI,(_T("APP:MSG:OnSpeechMsg: OnSpeechTerminate\r\n")));
                if(!m_bPhoneCallActive && (m_bSpeechOn || IsNaviSpeechEnable()) && !m_bOnSR)
                {
					if(m_iSpeechMsgType == type)
						OnSpeechTerminate();
                }
			}
		}
		}
	}

}
#endif

void CGUI_Fantasy::OnMHLMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMHLMsg]: wParam:%d,lParam:%d\r\n"),wParam,lParam));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MHL,PAGE_MHL_MAIN));
	if(panel)
	{
		((CPanelMHL*)panel)->OnMHLMsg(wParam,lParam);
	}
}


void CGUI_Fantasy::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnAvailableSourceChanged]: %d,%d\r\n"),wParam,lParam));

	if(wParam<MEDIA_TYPE_NUMBER)
	{
		m_idValidSrcList[wParam]=lParam;
	}

	if(wParam == MEDIA_TYPE_ANDROID_AUTO)
	{
		m_bAndroidAutoConnect = lParam;
	}

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		((CPanelMediaWrapper*)panel)->OnAvailableSourceChanged(wParam,lParam);
	}
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_WRAPPER));
	if(panel)
	{
		((CPaneliPodWrapper*)panel)->OnAvailableSourceChanged(wParam,lParam);
	}
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
	if(panel)
	{
		((CPanelHomePage*)panel)->OnAvailableSourceChanged(wParam,lParam);
	}
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_WRAPPER));
	if(panel)
	{
		((CPanelImageWrapper*)panel)->OnAvailableSourceChanged(wParam,lParam);
	}

	//auto switch source after ACC on:
	if(lParam && g_AccOnSRCToSwicth != -1)
	{
		if((wParam == MEDIA_TYPE_A2DP && GET_PANEL_CLASS_ID(g_AccOnSRCToSwicth) == UI_CLASS_BT) ||
		   (wParam == MEDIA_TYPE_IPOD && GET_PANEL_CLASS_ID(g_AccOnSRCToSwicth) == UI_CLASS_IPOD))
		{
			if(m_bModulesLoaded)
			{
				RETAILMSG(1,(_T("==>[%s] Auto switch source to 0x%x after ACC on.\r\n"),_T(__FUNCTION__),g_AccOnSRCToSwicth));
				SwitchPage(GET_PANEL_CLASS_ID(g_AccOnSRCToSwicth),GET_PANEL_PAGE_ID(g_AccOnSRCToSwicth));
			}
			else
			{
				g_SRCToSwicthAfterLoadModule = g_AccOnSRCToSwicth;
			}
			//reset:
			g_AccOnSRCToSwicth = -1;
		}
	}
}
void CGUI_Fantasy::OnMcuUpdateMsg(WPARAM wParam, LPARAM lParam)
{
	m_idMcuUpdateStatus = wParam;

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMcuUpdateMsg]: wParam:%d lParam:%d\r\n"),wParam,lParam));

	switch (wParam)
	{
	case UPDATE_MCU_FAILED://update failed
		{
			m_pConfig->GetCurParam()->bMCUSoftwareUpdateFlag = FALSE;
			m_pConfig->SaveConfig();
			//reset:
			::PostMessage(g_hUIWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
		}
		break;
	case UPDATE_MCU_SUCCEED://update successful
		{
			m_pConfig->GetCurParam()->bMCUSoftwareUpdateFlag = FALSE;
			m_pConfig->SaveConfig();
			//reset:
			::PostMessage(g_hUIWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
		}
		break;
	}

	//
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_UPDATE,0));
	if(panel)
	{
		((CPanelUpdate*)panel)->OnMcuUpdateMsg(wParam,lParam);
	}
}
void CGUI_Fantasy::OnRadarStatus(WPARAM wParam, LPARAM lParam)
{
#if CVTE_EN_CAMERA
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN));
	if(panel)
	{
		((CPanelRearView*)panel)->OnRadarStatus(wParam,lParam);
	}
#endif
}
void CGUI_Fantasy::OnCanbusStatus(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI, ((L"MSG: [CGUI_Fantasy::OnCanbusStatus]: %d\r\n"),wParam));


	if(m_pConfig)
	{
		m_pConfig->GetCurParam()->bHasCanBus=wParam;

		CPanelGL *panel;

		panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
		if(panel)
		{
			((CPanelHomePage*)panel)->OnCanbusStatus(wParam,lParam);
		}

// 		panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NAVI,0));
// 		if(panel)
// 		{
// 			((CPanelRearView*)panel)->OnCanbusStatus(wParam,lParam);
// 		}
	}

}

void CGUI_Fantasy::OnDabServiceStartReady(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnDabServiceStartReady(wParam,lParam);
	}
}
void CGUI_Fantasy::OnDabServiceLinkMsg(WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: Link=%d\n"), TEXT(__FUNCTION__),wParam));
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_WRAPPER));
	if(panel)
	{
		((CPanelRadioWrapper*)panel)->OnDabServiceLinkMsg(wParam,lParam);
	}
}
#if CVTE_EN_ANDROID_AUTO
void CGUI_Fantasy::OnAndroidAutoMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: wParam=%d, lParam=%d\n"), TEXT(__FUNCTION__),wParam,lParam));
	switch (wParam)
	{
	case ANDROIDAUTO_CONNECT_STATUS:
		{
			BOOL bConnected = LOWORD(lParam);
			BOOL bShowTopBanner = HIWORD(lParam);
			m_bAndroidAutoConnect = bConnected;
			if(!bConnected)
				m_bAndroidAutoVoiceActicve = FALSE;
			//top banner notification:
			if(m_pSmartBarTopBanner)
			{
				if(bConnected && bShowTopBanner)
				{
					if(m_pConfig->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER))
					{
						m_pSmartBarTopBanner->SetTitle(_T("Android Auto Connected"));
						m_pSmartBarTopBanner->SetNotificationUI(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
						m_pSmartBarTopBanner->Show(TRUE,10000);
					}
				}
				else
				{
					m_pSmartBarTopBanner->Show(FALSE);
				}
			}
		}
		break;
	case ANDROIDAUTO_VOICE_STATUS:
		m_bAndroidAutoVoiceActicve = lParam;
		break;
	}

	//notify navigation simulator:
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NAVI_SIMULATE,0));
	if(panel)
	{
		((CPanelNaviSimulate*)panel)->OnAndroidAutoMsg(wParam,lParam);
	}
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NAVI,0));
	if(panel)
	{
		((CPanelApp*)panel)->OnAndroidAutoMsg(wParam,lParam);
	}
}
#endif
#if CVTE_EN_CARPLAY
void CGUI_Fantasy::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
    //Carplay:
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
	if(panel)
	{
		((CPanelCarPlay*)panel)->OnCarPlayMsg(wParam,lParam);
	}
	//settings:
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnCarPlayMsg(wParam,lParam);
	}
	//navigation:
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NAVI,0));
	if(panel)
	{
		((CPanelApp*)panel)->OnCarPlayMsg(wParam,lParam);
	}
	//media:
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		((CPanelMediaWrapper*)panel)->OnCarPlayMsg(wParam,lParam);
	}
	//bluetooth:
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
	if(panel)
	{
		((CPanelBluetoothWrapper*)panel)->OnCarPlayMsg(wParam,lParam);
	}
	else
	{//[BUG] 0002753 connected carplay before acc off,then acc on,remove carplay device,bt can`t auto connect
		if ((CARPLAY_CONNECT_STATUS == wParam) && (FALSE == (BOOL)lParam))
		{
			OpenPanels(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_MAIN));
			panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
			if(panel)
			{
				((CPanelBluetoothWrapper*)panel)->OnCarPlayMsg(CARPLAY_CONNECT_STATUS,FALSE);
			}
		}
	}

	//notify widget:
	switch (wParam)
	{
    case CARPLAY_CONNECT_STATUS:
        {
			/*
			BOOL bshow = (BOOL)lParam;
     		OperateWidget(IDC_WIDGET_CARPLAY,0,UIOPRT_SHOWHIDE,&bshow);
			m_idValidSrcList[MEDIA_TYPE_CARPLAY] = bshow;
    		m_bCarPlayConnected = bshow;
			#if CVTE_EN_LED
			m_pLED->SetLedState(bshow?LED_SIRI_ENABLE:LED_SIRI_DISABLE);
			#endif
			*/
		}
		break;
    case CARPLAY_CONTROL_STATUS:
        {
			BOOL bshow = (BOOL)lParam;
			OperateWidget(IDC_WIDGET_CARPLAY,0,UIOPRT_SHOWHIDE,&bshow);
			m_idValidSrcList[MEDIA_TYPE_CARPLAY] = bshow;
			m_bCarPlayConnected = bshow;
#if (CVTE_EN_LED && CVTE_EN_LED_SIRI_ANIM)
			m_pLED->SetLedState(bshow?LED_SIRI_ENABLE:LED_SIRI_DISABLE);
#endif

			if(m_pSmartBarTopBanner)
			{
				if(lParam)
				{
					if(CCarplayAdapter::GetInstance()->NeedShowTopBanner()
						&& m_pConfig->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER))
					{
						m_pSmartBarTopBanner->SetTitle(_T("Apple CarPlay Connected"));
						m_pSmartBarTopBanner->SetNotificationUI(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
						m_pSmartBarTopBanner->Show(TRUE,10000);
					}
				}
				else
				{
					m_pSmartBarTopBanner->Show(FALSE);
				}
			}
        }
        break;
    case CARPLAY_MAIN_AUDIO_STATUS:
        {
			RETAILMSG(DEBUG_GUI, ((L"MSG:[%s]: CARPLAY_MAIN_AUDIO_STATUS %d m_bOnSR:%d\r\n"), TEXT(__FUNCTION__),lParam, m_bOnSR));
			m_bCarPlayMainAudioOn = (BOOL)lParam;
#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
			if(m_bOnSR && !lParam)
			{
				Mute(TRUE,FALSE);
			}
#endif
        }
        break;
    case CARPLAY_ALT_AUDIO_STATUS:
        {
			RETAILMSG(DEBUG_GUI, ((L"MSG:[%s]: CARPLAY_ALT_AUDIO_STATUS %d\r\n"), TEXT(__FUNCTION__),lParam));
    		m_bCarPlayAudioOn = (BOOL)lParam;
    		if(m_bCarPlayAudioOn)
    		{
    			m_dwCarPlayAudioOnTick = GetTickCount();
    		}
    		else
    		{
    			m_dwCarPlayAudioOffTick = GetTickCount();
    		}
        }
        break;
    case CARPLAY_SCREEN_STATUS:
        {

        }
        break;
    case CARPLAY_MODESCHANGED_STATUS:
        {
			#if CVTE_EN_CARPLAY
			const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();

			//turnbyturn:
			m_bCarPlaySiriRecognizing = ((mode->speech.entity == kAirPlayEntity_Controller) /*&& (mode->speech.mode == kAirPlaySpeechMode_Recognizing)*/);
			m_bCarPlayTurnbyTurnOn = (mode->turnByTurn == kAirPlayEntity_Controller);
			#endif
        }
        break;
    case CARPLAY_REQUESTUI_STATUS:
        {
        }
        break;
    case CARPLAY_DUCK_STATUS:
        {
			RETAILMSG(1, (TEXT("MSG:[%s]: CARPLAY_DUCK_STATUS %d\r\n"), TEXT(__FUNCTION__),lParam));
            int vol=lParam;
            if(vol<100 && m_bCarPlayTurnbyTurnOn && IsNaviSpeechEnable()&& !m_bPhoneCallActive)
            {
                //RETAILMSG(1, (TEXT("OnSpeechStart m_pConfig->GetCurParam()->dwVolGPS:%d\r\n"),m_pConfig->GetCurParam()->dwVolGPS));
                OnSpeechStart(0);
            }
        }
        break;
    case CARPLAY_UNDUCK_STATUS:
        {
            RETAILMSG(1, (TEXT("MSG:[%s]: CARPLAY_UNDUCK_STATUS \r\n"), TEXT(__FUNCTION__)));
			//Bug <0002872> 播放carplay音乐->点击启用Siri->点击取消Siri->听到carplay音乐后再启用Siri，反复操作5次左右
			//车机处于语音状态，carplay音乐声音被压低，调节音量，为语音音量图标
            //if(!m_bOnSR && !m_bPhoneCallActive)
            {
                OnSpeechTerminate();
            }
        }
        break;
	case CARPLAY_PHONE_AUDIO_FORMAT:
		{
			RETAILMSG(1, ((L"---CARPLAY_PHONE_AUDIO_FORMAT=%d---\r\n"),lParam));

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
			//switch asp work mode to phone-call mode if has any asp module
			if(m_pAudioCodec)
			{
				//if(m_pAudioCodec->GetCurrentAudioBus() == SRC_AUDIO_PHONE)
				{
					//m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,1000);

					//mute audio for a while to avoid pop-noise
					OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(600));

					if(lParam == kAirPlayAudioFormat_PCM_8KHz_16Bit_Mono)
						m_pAudioCodec->ChangeAspWorkMode(AspModeCM_NB);
					else if(lParam == kAirPlayAudioFormat_PCM_16KHz_16Bit_Mono)
						m_pAudioCodec->ChangeAspWorkMode(AspModeCM_WB);
					else if(lParam == kAirPlayAudioFormat_PCM_24KHz_16Bit_Mono)
						m_pAudioCodec->ChangeAspWorkMode(AspModeCM_SB);
					else //default
						m_pAudioCodec->ChangeAspWorkMode(AspModeCM_NB);
				}
			}
#elif (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
			//switch asp work mode to phone-call mode if has any asp module
			if(m_pAudioCodec)
			{
				//mute audio for a while to avoid pop-noise
				m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,1000);

				if(lParam == kAirPlayAudioFormat_PCM_8KHz_16Bit_Mono)
					m_pAudioCodec->ChangeAspWorkMode(AspModeCM_NB);
				else if(lParam == kAirPlayAudioFormat_PCM_16KHz_16Bit_Mono)
					m_pAudioCodec->ChangeAspWorkMode(AspModeCM_WB);
				else if(lParam == kAirPlayAudioFormat_PCM_24KHz_16Bit_Mono)
					m_pAudioCodec->ChangeAspWorkMode(AspModeCM_SB);
				else //default
					m_pAudioCodec->ChangeAspWorkMode(AspModeCM_NB);
			}
#endif

			//carplay phone-call connected:
			::PostMessage(m_hMainWnd,WM_PHONE_CALL_STATE,TRUE,(1<<16)+HF_STATE_TALKING);
		}
		break;
	case CARPLAY_SIRI_AUDIO_FORMAT:
		{
			RETAILMSG(1, ((L"---CARPLAY_SIRI_AUDIO_FORMAT=%d---\r\n"),lParam));

			//release mute while siri audio is connected,
			//switch asp work mode to speech mode if has any asp module
			if(m_bOnSR)
			{
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)	
				if(m_pAudioCodec)
				{
					m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolGPS,TRUE);
					m_pAudioCodec->MuteMainAudio(FALSE);
					m_iVolumeFadeTo = m_pAudioCodec->GetMainVolumeTarget();
				}
#else
				OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,((1)<<24)+(1<<16)+(100));
#endif
			}
		}
		break;
    default:
        break;
	}
}
#endif
#if CVTE_EN_CPMAC
void CGUI_Fantasy::OnCarPlayMACChanged(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if (panel)
	{
		((CPanelSettingWrapper*)panel)->OnMACChanged();
	}
}
#endif

void CGUI_Fantasy::OnPhoneHFState(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnPhoneHFState(wParam,lParam);
	}
}
void CGUI_Fantasy::OnBluetoothSetupStatus(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnBluetoothSetupStatus(wParam,lParam);
	}
}
void CGUI_Fantasy::OnBluetoothMsg(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel = NULL;
#if CVTE_EN_EASYCONNECT
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER));
	if(panel)
	{
		((CPanelEasyConnected*)panel)->OnBluetoothMsg(wParam,lParam);
	}
#endif
#if CVTE_EN_WEBLINK
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER));
	if(panel)
	{
		((CPanelWebLink*)panel)->OnBluetoothMsg(wParam,lParam);
	}
#endif

	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
	if(panel)
	{
		((CPanelBluetoothWrapper*)panel)->OnBluetoothMsg(wParam,lParam);
	}
}
void CGUI_Fantasy::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	if(m_bPowerOperating)
	{
		//if power is on operating, just put the command into a list,
		//and process it after power status is ready.
		MsgData data={wParam, lParam};
		m_listPhoneCallCmd.push_back(data);
	}
	else
	{
		//empty command list first:
		if(m_listPhoneCallCmd.size())
		{
			for (int i=0; i<m_listPhoneCallCmd.size(); i++)
			{
				ProcessPhoneCallState(m_listPhoneCallCmd.at(i).wParam, m_listPhoneCallCmd.at(i).lParam);
			}
			m_listPhoneCallCmd.clear();
		}
		ProcessPhoneCallState(wParam, lParam);
	}
}
void CGUI_Fantasy::ProcessPhoneCallState(WPARAM wParam, LPARAM lParam)
{

	RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: wParam[%d] lParam[0x%x] m_bSpeechOn[%d] m_bPhoneCallActive[%d] m_dwCallStatus[%d]\r\n"),wParam,lParam,m_bSpeechOn,m_bPhoneCallActive,m_dwCallStatus));

	const UINT idPhoneCallType = HIWORD(lParam);//0-bluetooth, 1-carplay
	const UINT idCallPara=LOWORD(lParam);

	if(idPhoneCallType==0)
	{
		m_bBTPhoneCallActive=wParam;
		m_dwBTCallStatus=idCallPara;
#if CVTE_EN_CARPLAY
		CCarplayAdapter::GetInstance()->SetBluetoothPhoneStatus(wParam);
#endif
	}
	else if(idPhoneCallType==1)
	{
		m_bCarPlayPhoneCallActive=wParam;
		m_dwCarPlayCallStatus=idCallPara;
	}

	BOOL bActive = (m_bCarPlayPhoneCallActive||m_bBTPhoneCallActive);

	//
	if(m_bPhoneCallActive!=bActive)
	{
		//m_bPhoneCallActive=(bActive>0?TRUE:FALSE);

		if(bActive)
		{
			RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: process phone-call bActive=1\r\n")));

			if(!IsPowerOn())
			{
                DoPowerOn();
#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO) || (CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
				//customer require device remain power on after phone-call end.
				if(!m_bOnReverseMode)
				{
					OnSystemPrePowerState(SYS_POWER_ON);
					OnSystemPowerState(SYS_POWER_ON);
					m_bManualPowerOn = TRUE;
				}
				else
				{
					PowerOn();
				}
#else
				PowerOn();
#endif
			}

			//turn on backlight
			if(m_bBacklightOFF)
			{
	            RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: BKL_SetBackLight\r\n")));
                ScreenOn();
			}

			if(m_bManualMute)
			{
				OnMuteKey(FALSE);
			}

            if(m_bSpeechOn)
            {
	            RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: OnSpeechTerminate\r\n")));
#if USE_WAV_ONCHIP
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)	
				if (m_pAudioCodec)
				{
					m_pAudioCodec->SetMixerOn(FALSE);
				}
#else
            	if(m_pCommander)
            	{
            		m_pCommander->SendCmd(_ARM_MIX_WAV_KEY,0);
            	}
#endif
#endif
            }

		}
#if 0
		else
		{
			//
// 			if(!m_bOnReverseMode)
// 			{
// 				if(!m_bManualPowerOn)
// 					PowerIdle();
// 				else if(m_pConfig->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
// 					ReturnToPrevPanel(PAGE_SWITCH_ANI_NONE);
// 			}

			if(!m_bManualPowerOn)
			{
				RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: Hangup #1#\r\n")));
				if(!m_bOnReverseMode)
				{
					PowerIdle();
				}
			}
			else
			{
				RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: Hangup #2#\r\n")));
				if(m_pConfig->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
				{
					if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI)!=UI_CLASS_IDLE)
					{
					    ReturnToPrevPanel(PAGE_SWITCH_ANI_NONE);
					}
				}
				else
				{
					RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: m_pConfig->GetCurParam()->idCurUI[%X] != MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING)\r\n"),m_pConfig->GetCurParam()->idCurUI));
				}
			}

			//
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(0<<16)+(2000));
		}
#endif

#if 0
		//
		if(m_bManualPowerOn)
		{
			m_listPanels.Head();
			while (!m_listPanels.End())
			{
				m_listPanels.Get()->OnPhoneCallState(bActive,lParam);
				m_listPanels.Next();
			}
		}
#endif

		//
		if(bActive)
		{
			//hide smart bar:
			if(m_pSmartBarVol && m_pSmartBarVol->IsVisible())
			{
				m_pSmartBarVol->Show(FALSE);
			}
			if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
			{
				m_pSmartBarMedia->Show(FALSE);
			}
#if CVTE_EN_ACCON_WARNING
			CWarningProxy::GetInstance()->PauseShowWarningBar();
#endif			
		}
		else
		{
#if CVTE_EN_ACCON_WARNING
			CWarningProxy::GetInstance()->ResumeShowWarningBar();
#endif	
		}
	}


	//if(m_dwCallStatus!=idCallPara)
	{
		m_dwCallStatus=idCallPara;

		//asp control:
#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
		//if bluetooth call, change asp mode directly here,
		//if carplay call, change asp mode while CARPLAY_PHONE_AUDIO_FORMAT message received.
		if(idPhoneCallType==0)
		{
			if(m_pAudioCodec)
			{
				if(bActive)
				{
					if(m_pAudioCodec->GetAspWorkMode()!=AspModeCM_WB)
						m_pAudioCodec->ChangeAspWorkMode(AspModeCM_WB);
				}
			}
		}
#endif

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_AK7738)
		//change audio src:
		if(m_pAudioCodec)
		{
			switch(idCallPara)
			{
			case HF_STATE_IDLE:
				{
					RETAILMSG(1,(_T("MSG:[%s]: # HF_STATE_IDLE #\r\n"),TEXT(__FUNCTION__)));
					m_pAudioCodec->SetHFStatus(AspHFStatus(idCallPara));
					if(m_bSpeechOn)
						m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolGPS);
					else
						m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolMedia);
					m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,1000,5);
					m_pAudioCodec->ReleaseAudioBus(SRC_AUDIO_PHONE);
					m_pAudioCodec->ChangeAspWorkMode(AspModeMedia);
	
					if(m_bManualPowerOn)
					{
						RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: Hangup #2#\r\n")));
						if(m_pConfig->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
						{
							if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI)!=UI_CLASS_IDLE)
							{
								ReturnToPrevPanel(PAGE_SWITCH_ANI_NONE);
							}
						}
						else
						{
							RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: m_pConfig->GetCurParam()->idCurUI[%X] != MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING)\r\n"),m_pConfig->GetCurParam()->idCurUI));
						}
					}
					else
					{
						RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: Hangup #1#\r\n")));
						if(!m_bOnReverseMode)
						{
							SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
							PowerIdle();
						}
					}
				}
				break;
			case HF_STATE_INCOMING_CALL:
			case HF_STATE_OUTGOING_CALL:
				{
					RETAILMSG(1,(_T("MSG:[%s]: # HF_STATE_INCOMING_CALL/HF_STATE_OUTGOING_CALL #\r\n"),TEXT(__FUNCTION__)));
					m_pAudioCodec->SetHFStatus(AspHFStatus(idCallPara));	
					m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolBluetooth);
					m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,400,5);
					m_pAudioCodec->RequestAudioBus(SRC_AUDIO_PHONE,lParam);
					m_pAudioCodec->ChangeAspWorkMode(AspModeMedia);
				}
				break;
			case HF_STATE_TERMINATING:
				{
					RETAILMSG(1,(_T("MSG:[%s]: # HF_STATE_TERMINATING #\r\n"),TEXT(__FUNCTION__)));
					m_pAudioCodec->SetHFStatus(AspHFStatus(idCallPara));	
				}
				break;
			case HF_STATE_TALKING:
				{
					RETAILMSG(1,(_T("MSG:[%s]: # HF_STATE_TALKING #\r\n"),TEXT(__FUNCTION__)));
					m_pAudioCodec->SetHFStatus(AspHFStatus(idCallPara));	
					m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolBluetooth);
					m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,400,5);
					//m_pAudioCodec->RequestAudioBus(SRC_AUDIO_PHONE,lParam);

					//if bluetooth call, change asp mode directly here,
					//if carplay call, change asp mode while CARPLAY_PHONE_AUDIO_FORMAT message received.
					if(idPhoneCallType==0)
					{
						m_pAudioCodec->ChangeAspWorkMode(AspModeCM_WB);
					}
				}
				break;
			case HF_STATE_WAITING_CALL:
			case HF_STATE_NO_HELD_CALL:
			case HF_STATE_HELD_CALL_NO_ACTIVE_CALL:
				{
					RETAILMSG(1,(_T("MSG:[%s]: # HF_STATE_WAITING_CALL / HF_STATE_NO_HELD_CALL / HF_STATE_HELD_CALL_NO_ACTIVE_CALL #\r\n"),TEXT(__FUNCTION__)));
					m_pAudioCodec->SetHFStatus(AspHFStatus(idCallPara));	
					m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolBluetooth);
					m_pAudioCodec->MuteMainAudio(TRUE,FALSE,TRUE,400);
					m_pAudioCodec->RequestAudioBus(SRC_AUDIO_PHONE,lParam);
				}
				break;
			default:
				break;
			}
			

		}
#else
		//change audio src:
		if(idCallPara==HF_STATE_OUTGOING_CALL)
		{
            SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(1500));

			//adjust bluetooth volume when phone-call is active:
			//0=-10db, 10=0db, 20=10db
			BYTE offset = (idPhoneCallType==0) ? 20 : 10; //bluetooth +10db, carplay +5db
			RETAILMSG(1,(_T("MSG:[%s]: Volume offset=%ddb\r\n"),_T(__FUNCTION__),offset/2));
			m_pCommander->SendCmd(_ARM_TFT_VOL_GAIN_OFFSET,offset);
		}
		else if(idCallPara==HF_STATE_TALKING)
		{
            SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
     		OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(600));

			//adjust bluetooth volume when phone-call is active:
			//0=-10db, 10=0db, 20=10db
			BYTE offset = (idPhoneCallType==0) ? 20 : 10; //bluetooth +10db, carplay +5db
			RETAILMSG(1,(_T("MSG:[%s]: Volume offset=%ddb\r\n"),_T(__FUNCTION__),offset/2));
			m_pCommander->SendCmd(_ARM_TFT_VOL_GAIN_OFFSET,offset);

// #if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
// 			//if bluetooth call, change asp mode directly here,
// 			//if carplay call, change asp mode while CARPLAY_PHONE_AUDIO_FORMAT message received.
// 			if(idPhoneCallType==0)
// 			{
// 				if(m_pAudioCodec && m_pAudioCodec->GetAspWorkMode()!=AspModeCM_NB)
// 				{
// 					m_pAudioCodec->ChangeAspWorkMode(AspModeCM_NB);
// 				}
// 			}
// #endif

		}
		else if(idCallPara==HF_STATE_INCOMING_CALL)
		{
#if USE_WAV_ONCHIP
            SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
     		OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(2300));
#else
            SwitchAudioSRC(SRC_AUDIO_ARM,TRUE);
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(1000));
#endif
        }
		else if(m_dwBTCallStatus==HF_STATE_IDLE && m_dwCarPlayCallStatus==HF_STATE_IDLE)//no call
		{
#if USE_WAV_ONCHIP
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(2000));
#else
			OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(500));
#endif

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
			if(m_pAudioCodec)
			{
				m_pAudioCodec->ChangeAspWorkMode(AspModeMedia);
			}
#endif

			//reset volume:
			//0=-10db, 10=0db, 20=10db
			RETAILMSG(1,(_T("MSG:[%s]: Volume offset=0db\r\n"),_T(__FUNCTION__)));
			m_pCommander->SendCmd(_ARM_TFT_VOL_GAIN_OFFSET,10);
		}
        else
        {
            OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(2000));
        }
#endif

#if CVTE_EN_BT_PHONECALL_MEDIA_RING
		if(idPhoneCallType == 0)
		{
			//ring-tone control:
			if(idCallPara==HF_STATE_INCOMING_CALL)
			{
				//SendMediaCommand(MEDIA_CMD_SET_RINGTONE_FILE,(LPARAM)_T("\\ResidentFlash\\Media\\ringtone.wav"));
				//OnRingToneCmd(MAKELONG(1, 1),MAKELONG(0xFFFF, 0));//ring, loop, max volume
				//sndPlaySound(_T("\\ResidentFlash\\Media\\ringtone.wav"),SND_ASYNC|SND_LOOP);
				if(!m_pSndPlayer)
				{
					if(m_pMediaWrapper)
					{
						m_pSndPlayer = new CSndPlayer(m_entry.hInstance, m_pMediaWrapper->GetMediaCore(),
							_T("\\ResidentFlash\\Media\\ringtone.wav"), //filename
							TRUE, //loop
							1,	//wave id
							-1 //volume, -1 = max volume
							);
					}
				}
				if(m_pSndPlayer)
				{
					m_pSndPlayer->start();
				}
			}
			else
			{
				//OnRingToneCmd(0,0);//stop ring
				//sndPlaySound(NULL,0);
				if(m_pSndPlayer)
				{
					m_pSndPlayer->stop();
				}
			}
		}
#endif
	}

#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
	//power stuff:
	if(m_bPhoneCallActive != bActive)
	{
		if(!bActive)
		{
			RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: process phone-call bActive=0\r\n")));

			if(!m_bManualPowerOn)
			{
				RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: Hangup #1#\r\n")));
				if(!m_bOnReverseMode)
				{
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
					PowerIdle();
				}
			}
			else
			{
				RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: Hangup #2#\r\n")));
				if(m_pConfig->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
				{
					if(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idPreUI)!=UI_CLASS_IDLE)
					{
						ReturnToPrevPanel(PAGE_SWITCH_ANI_NONE);
					}
				}
				else
				{
					RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnPhoneCallState]: m_pConfig->GetCurParam()->idCurUI[%X] != MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING)\r\n"),m_pConfig->GetCurParam()->idCurUI));
				}
				OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(0<<16)+(2000));
			}
		}
	}
#endif

	//panel notify:
	BOOL btmp=m_bPhoneCallActive;
	m_bPhoneCallActive=(bActive>0?TRUE:FALSE);

	if(btmp != bActive)
	{
		m_listPanels.Head();
		while (!m_listPanels.End())
		{
			m_listPanels.Get()->OnPhoneCallState(bActive,lParam);
			m_listPanels.Next();
		}

	}

}
void CGUI_Fantasy::MuteReleaseProc()
{
	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::MuteReleaseProc]: begin idType:%x m_bAutoReleaseNoFade:%d\r\n"),m_idMuteType,m_bAutoReleaseNoFade));

	//Sleep(m_iMuteTimeOut);
	if(WaitForSingleObject(m_hEventMute,m_iMuteTimeOut)== WAIT_OBJECT_0)
	{
		//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::MuteReleaseProc]: break... idType:%x\r\n"),m_idMuteType));
		return;
	}

	//un mute:
	Mute(FALSE,!m_bAutoReleaseNoFade,m_bNoSendMuteCmd);

	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::MuteReleaseProc]: end  idType:%x\r\n"),m_idMuteType));

}
DWORD CGUI_Fantasy::MuteReleaseThreadPro(LPVOID lpParam)
{
	CGUI_Fantasy *p_GUI = (CGUI_Fantasy*)lpParam;

	if(p_GUI)
	{
		p_GUI->MuteReleaseProc();
	}

	return 0;
}
void CGUI_Fantasy::OnManualMute(WPARAM wParam, LPARAM lParam)
{
	m_bManualMute = wParam;
}
void CGUI_Fantasy::OnMuteMsg(WPARAM wParam, LPARAM lParam)
{
    if(m_bManualMute || m_bOnHWMute)
    {
		RETAILMSG(1, (TEXT("WAR:[%s]: ManualMute or HWMute, skip!!!\r\n"), TEXT(__FUNCTION__)));
		return;
	}

	BOOL bMute=((wParam>>24)&0xF);//HIWORD(wParam);
	UINT idType=LOWORD(wParam);
	BOOL bAutoRelease=((lParam>>16)&0xFF);//HIWORD(lParam);
	BOOL bAutoReleaseNoFade=(lParam>>24);
	UINT iTimeOut=LOWORD(lParam);
	BOOL fade=((wParam>>16)&0xFF);
    BOOL bNoSendMuteCmd = ((wParam>>28)&0xF);

	m_bAutoReleaseNoFade=bAutoReleaseNoFade;
    m_bNoSendMuteCmd=bNoSendMuteCmd;

	RETAILMSG(1/*DEBUG_GUI*/,(_T("MSG:[CGUI_Fantasy::OnMuteMsg]: bMute:%d,fade:%d,idType:%d,bAutoRelease:%d,bAutoReleaseNoFade:%d,iTimeOut:%d,bNoSendMuteCmd:%d\r\n"),
        bMute,
        fade,
        idType,
        bAutoRelease,
        bAutoReleaseNoFade,
        iTimeOut,
        bNoSendMuteCmd));
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	if (m_pAudioCodec)
	{
		if(m_bPhoneCallActive)
			m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolBluetooth);
		else
			m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolMedia);

		m_iVolumeFadeTo = m_pAudioCodec->GetMainVolumeTarget();

		m_pAudioCodec->MuteMainAudio(bMute, bAutoRelease, fade, iTimeOut,25);
	}
#else
	{
		m_idMuteType = idType;
		m_iMuteTimeOut = iTimeOut;

		Mute(bMute,fade,m_bNoSendMuteCmd);

		//cancel manual mute:
		if((bMute && bAutoRelease) || !bMute)
		{
//			    if(m_bManualMute && m_pLED)
//			    {
//			        m_pLED->SetLedState(LED_UNMUTE);
//			    }
			m_bManualMute = FALSE;
		}

		if(bMute && bAutoRelease)
		{
			if(m_hMuteThread != NULL)
			{
				RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMuteMsg]: on muting,quit...\r\n")));
				SetEvent(m_hEventMute);
				WaitForSingleObject(m_hMuteThread, INFINITE);
				CloseHandle(m_hMuteThread);
				m_hMuteThread = NULL;
			}
			RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnMuteMsg]: Reset Mute Event...\r\n")));
			ResetEvent(m_hEventMute);
			m_hMuteThread = CreateThread(NULL,NULL,CGUI_Fantasy::MuteReleaseThreadPro,this,NULL,NULL);
		}
	}
#endif
}

void CGUI_Fantasy::OnWidgetChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnWidgetChanged]: wParam:%d,lParam:%d\r\n"),wParam,lParam));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
	if(panel)
	{
		((CPanelHomePage*)panel)->OnWidgetChanged(wParam,lParam);
	}

// 	panel=m_listPanels.GetItemByID(UI_ALLAPP);
// 	if(panel)
// 	{
// 		((CPanelAllAPP*)panel)->OnWidgetChanged(wParam,lParam);
// 	}
}
void CGUI_Fantasy::OnBluetoothCommand(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
	if(panel)
	{
		((CPanelBluetoothWrapper*)panel)->OnBluetoothCommand(wParam,lParam);
	}
	else
	{
		if(wParam==BT_CMD_POWER)//bt power on
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn = (lParam?TRUE:FALSE);
			if(lParam)
			{
				OpenPanels(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_MAIN));
			}
		}
	}
}
void CGUI_Fantasy::OnSmartBarControl(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnSmartBarControl]: wParam:%d,lParam:%d\r\n"),wParam,lParam));

	CSmartBar *pSmartBar=m_listSmartBars.GetItemByID(wParam);

	if(pSmartBar)
	{
		pSmartBar->Show(lParam,pSmartBar->GetShowTime());
	}
}
void CGUI_Fantasy::OnSmartBarNotify(WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnSmartBarNotify]: wParam:%d,lParam:%d\r\n"),wParam,lParam));

	//re-calculate gfx touch area:

	UINT ptLeft=0;
	UINT ptRight=0;
	UINT ptTop=0;
	UINT ptBottom=0;

	UINT px=0;
	UINT py=0;
	UINT cx=0;
	UINT cy=0;

	m_listSmartBars.Head();
	while (!m_listSmartBars.End())
	{
		if(m_listSmartBars.Get()->IsVisible())
		{
			px=m_listSmartBars.Get()->m_iOverlayX;
			py=m_listSmartBars.Get()->m_iOverlayY;
			cx=m_listSmartBars.Get()->m_iOverlayX+m_listSmartBars.Get()->m_iOverlayWidth;
			cy=m_listSmartBars.Get()->m_iOverlayY+m_listSmartBars.Get()->m_iOverlayHeight;

			if(px<ptLeft || ptLeft==0)
				ptLeft=px;
			if(py<ptTop || ptTop==0)
				ptTop=py;
			if(cx>ptRight)
				ptRight=cx;
			if(cy>ptBottom)
				ptBottom=cy;
		}

		m_listSmartBars.Next();
	}
	//RETAILMSG(1, (TEXT("MSG:[%s]: smart bar touch dwArea :left:%d, top:%d, right:%d, bottom:%d \r\n"),TEXT(__FUNCTION__),ptLeft,ptTop,ptRight,ptBottom));

	ptLeft=(ptLeft)*255/(g_iScreenWidth);
	ptRight=(ptRight)*255/(g_iScreenWidth);
	ptTop=(ptTop)*255/(g_iScreenHeight);
	ptBottom=(ptBottom)*255/(g_iScreenHeight);

	if(ptLeft>255)ptLeft=255;
	if(ptRight>255)ptRight=255;
	if(ptTop>255)ptTop=255;
	if(ptBottom>255)ptBottom=255;

	DWORD dwArea=(ptLeft<<24)+(ptTop<<16)+(ptRight<<8)+(ptBottom);

	SetEventData(g_hTouchEnableGfxEvent,dwArea);
    SetEvent(g_hTouchEnableGfxEvent);
	//RETAILMSG(1, (TEXT("MSG:[%s]: Disable smart bar touch dwArea :left:%d, top:%d, right:%d, bottom:%d\r\n"),TEXT(__FUNCTION__), ptLeft,ptTop,ptRight,ptBottom));

	//auto show/hide drop bar:
	if(m_pCurPanel)
	{
		if(GET_PANEL_CLASS_ID(m_pCurPanel->GetID())==UI_CLASS_NAVI && wParam==SMART_BAR_MEDIA && !lParam)
		{
			::PostMessage(g_hUIWnd,WM_SMARTBAR_CONTROL,SMART_BAR_DROP,1);
		}
		else if(wParam==SMART_BAR_MEDIA && lParam)
		{
			::PostMessage(g_hUIWnd,WM_SMARTBAR_CONTROL,SMART_BAR_DROP,0);
		}
	}
}
void CGUI_Fantasy::OnRingToneCmd(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		((CPanelMediaWrapper*)panel)->OnRingToneCommand(wParam,lParam);
	}
}

#if CVTE_EN_DYNAMIC_LOGO
void CGUI_Fantasy::OnPlayDynamicLogo()
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		DWORD dwVol = 0xFFFFFFFF;
		((CPanelMediaWrapper*)panel)->PlayDynamicLogo(dwVol);
	}
}

BOOL CGUI_Fantasy::DynamicLogoFinish()
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		return ((CPanelMediaWrapper*)panel)->DynamicLogoFinish();
	}
	return FALSE;
}
#endif

void CGUI_Fantasy::OnMediaPlayerCommand(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		((CPanelMediaWrapper*)panel)->OnMediaPlayerCommand(wParam,lParam);
	}
}
void CGUI_Fantasy::OnImagePlayerCommand(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnImagePlayerCommand]: wParam:%d,lParam:%d\r\n"),wParam,lParam));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IMAGE,PAGE_IMAGE_WRAPPER));
	if(panel)
	{
		((CPanelImageWrapper*)panel)->OnImagePlayerCommand(wParam,lParam);
	}
	else
	{
		RETAILMSG(1,(_T("ERR: [CGUI_Fantasy::OnImagePlayerCommand]: (CPanelImageWrapper*)panel is NULL !!! \r\n")));
	}
}

void CGUI_Fantasy::OnDABCommand(WPARAM wParam, LPARAM lParam)
{
#if CVTE_EN_DAB
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnDABCommand]: wParam:%d,lParam:%d\r\n"),wParam,lParam));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_DAB,PAGE_DAB_WRAPPER));
	if(panel)
	{
		((CPanelDABWrapper*)panel)->OnDABCommand(wParam,lParam);
	}
#endif
}

BOOL CGUI_Fantasy::OnVolumeConfigMsg(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet=FALSE;

	switch (wParam)
	{
	case 0://media
		{
			if(!m_bPhoneCallActive && !m_bSpeechOn)
			{
	            RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeConfigMsg]: Media SetMainVolume:%d\r\n"),lParam));
				//m_bManualMute = (lParam==0);
				m_iSetVolume=lParam;
				m_iVolOriginMedia=lParam;
				m_iSpeedCurVolome=m_iVehicleSpeed;
                m_pConfig->GetCurParam()->dwVolMedia = lParam;
				//SetMainVolume(lParam,FALSE,FALSE,FALSE);
                SetVolume(lParam,FALSE,FALSE,FALSE);

				bRet = TRUE;
			}
		}
		break;
	case 1://phone
		{
	        //RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeConfigMsg]: phone\r\n")));
			if(m_bPhoneCallActive)
			{
	            RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeConfigMsg]: Bluetooth SetMainVolume:%d\r\n"),lParam));
				m_iSetVolume=lParam;
				m_iVolOriginBluetooth=lParam;
				m_iSpeedCurVolome=m_iVehicleSpeed;
                m_pConfig->GetCurParam()->dwVolBluetooth = lParam;
				//SetMainVolume(lParam,FALSE,FALSE,FALSE);
                SetVolume(lParam,FALSE,FALSE,FALSE);
			}
			bRet = TRUE;
		}
		break;
	case 2://navi
		{
			//if(m_bSpeechOn)
			{
	            RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeConfigMsg]: Speech SetMixVolume:%d\r\n"),lParam));
				m_iSetVolume=lParam;
				m_iVolOriginGPS=lParam;
				m_iSpeedCurVolome=m_iVehicleSpeed;
                m_pConfig->GetCurParam()->dwVolGPS = lParam;
				//SetMixVolume(lParam,FALSE,FALSE,FALSE);
                SetVolume(lParam,FALSE,FALSE,FALSE,TRUE,FALSE);

				//
				if(lParam == 0)
				{
					EnableNaviSpeech(FALSE);
				}
				else
				{
					if(!g_bNaviSpeechEnabled && m_pConfig->GetCurParam()->idNaviSpeechConfig == NAVI_SPEECH_ALWAYS)//
					{
						EnableNaviSpeech(TRUE);
					}
				}

				bRet = TRUE;
			}
		}
		break;
	}

	return bRet;
}
DWORD CGUI_Fantasy::PlaySoundEffectThreadPro(LPVOID lpParam)
{
	CGUI_Fantasy *p_GUI = (CGUI_Fantasy*)lpParam;

	if(p_GUI)
	{
		RETAILMSG(DEBUG_GUI,(_T("------ PlaySoundEffectThreadPro begin-------\r\n")));
		p_GUI->PlaySoundEffectProc();
		RETAILMSG(DEBUG_GUI,(_T("------ PlaySoundEffectThreadPro end-------\r\n")));
	}

	return 0;
}
BOOL CGUI_Fantasy::OnGetPlayingStatus()
{
	UINT idCurSRCClass = GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
	if(idCurSRCClass==UI_CLASS_MEDIA ||
		idCurSRCClass==UI_CLASS_BT)
	{
		return m_pSmartBarMedia?m_pSmartBarMedia->GetPlayingFlag():FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CGUI_Fantasy::OnGetCurMuteStatus()
{
	return (m_bOnHWMute || m_bManualMute || m_bMute);
}

BOOL CGUI_Fantasy::OnPlaySoundEffect(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet=TRUE;

	//RETAILMSG(DEBUG_GUI,(_T("+++ OnPlaySoundEffect +++\r\n")));
	//BUG 0003421 beep音在播放任意声音后才出来
	if (IsPowerOn())
	{
		if(m_idMuteType == 0)
		{
			m_idMuteType = _ARM_MUTE_ALL;
			if(m_pCommander)
			{
				m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+0);
				RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnPlaySoundEffect]: unmute _ARM_MUTE_ALL\r\n")));
			}
		}

		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_KEY_BEEP_KEY,0);
		}
	}

#if 0
	if(!m_bPlayingSoundEffect)
	{
		ResetEvent(m_hPlaySoundEffect);
		m_bPlayingSoundEffect = TRUE;
		m_iPlaySoundEffectCnt = 1;
		HANDLE h = CreateThread(NULL,NULL,CGUI_Fantasy::PlaySoundEffectThreadPro,this,NULL,NULL);
		CloseHandle(h);
		bRet = TRUE;
	}
	else
	{
		SetEvent(m_hPlaySoundEffect);
		m_iPlaySoundEffectCnt ++;
	}
#endif

	return bRet;
}
void CGUI_Fantasy::PlaySoundEffectProc()
{
	static UINT s_src_before;
	BOOL bRet=FALSE;

	s_src_before = m_idCurAudioSRC;

	//process panels operation:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnPlaySoundEffect(TRUE);
		m_listPanels.Next();
	}

	//volume:
	SwitchAudioSRC(SRC_AUDIO_ARM);
// 	SwitchAudioSRC(SRC_AUDIO_SPEECH);
	OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(100));
	Sleep(120);
	SetVolume(10,FALSE,FALSE,TRUE,FALSE);//SetMainVolume(10,FALSE,FALSE,TRUE,FALSE);
	Sleep(10);

	do
	{
		m_iPlaySoundEffectCnt--;
		bRet =::PlaySound(_T("\\Media Card\\Click.wav"),NULL,SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_LOOP);
		Sleep(150);
		::PlaySound(NULL,NULL,0);
		Sleep(50);
	} while (WaitForSingleObject(m_hPlaySoundEffect,1500)==WAIT_OBJECT_0);

	//Sleep(1000);

	SwitchAudioSRC(s_src_before);
	OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(100));

	//process panels operation:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnPlaySoundEffect(FALSE);
		m_listPanels.Next();
	}

	m_bPlayingSoundEffect = FALSE;

	//return bRet;
}
BOOL CGUI_Fantasy::OnSwitchMediaClassSRC(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet=FALSE;

	if(IsPanelHasValidAudioSrc(wParam) && (m_idCurMediaClassSRC != wParam  || m_pConfig->GetCurParam()->idCurSRC != wParam))
	{
		RETAILMSG(1,( (L"MSG:[%s]: wParam:%X lParam:%X, idCurSRC:%X idPreSRC:%X\r\n"), TEXT(__FUNCTION__),wParam,lParam,m_pConfig->GetCurParam()->idCurSRC,m_pConfig->GetCurParam()->idPreSRC ));
		bRet = TRUE;

		//reset ACC source:
		g_AccOnSRCToSwicth = -1;

		//Update Param:
		//更新当前的多媒体源，写入到注册表，下次开机会继续使用上一个源
		m_pConfig->GetCurParam()->idPreSRC = m_pConfig->GetCurParam()->idCurSRC;
		m_idCurMediaClassSRC = wParam;
		m_pConfig->GetCurParam()->idCurSRC = wParam;//id_cur;
		m_pConfig->GetCurParam()->idCurSRCPara = lParam;//m_pCurPanel->GetPanelParam();

		/////////////////////////////////////
		m_listPanels.Head();
		//通知每个panel 源改变
		while (!m_listPanels.End())
		{
			RETAILMSG(DEBUG_GUI,( (L">>Panel ID: %d\r\n"), m_listPanels.Get()->GetID() ));
			m_listPanels.Get()->OnSRCChanged(wParam);
			m_listPanels.Next();
		}
		////////////////////////////////////////

// 		//
// 		if(!m_bSpeechOn && !m_bPhoneCallActive)
// 		{
// 			if(GET_PANEL_CLASS_ID(wParam) != UI_CLASS_MEDIA && GET_PANEL_CLASS_ID(wParam) != UI_CLASS_BT/* && GET_PANEL_CLASS_ID(wParam) != UI_CLASS_CARPLAY*/)
// 			{
// 				OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(600));
// 			}
// 		}

		//update smart bar media:
		if(m_pSmartBarMedia)
		{
			switch (GET_PANEL_CLASS_ID(wParam))
			{
			case UI_CLASS_CARPLAY:
				m_pSmartBarMedia->SetType(SB_MEDIATYPE_CARPLAY);
				m_pSmartBarMedia->SetNowPlayingTitle(_T(""));
				m_pSmartBarMedia->SetTrackName(_T("CarPlay"));
				m_pSmartBarMedia->SetPlayingFlag(TRUE);
				break;
// 			case UI_CLASS_RADIO:
// 				m_pSmartBarMedia->SetType(SB_MEDIATYPE_RADIO);
// 				m_pSmartBarMedia->SetNowPlayingTitle(_T(""));
// 				m_pSmartBarMedia->SetTrackName(_T("Radio"));
// 				m_pSmartBarMedia->SetPlayingFlag(TRUE);
// 				break;
			}
		}
	}

	return bRet;
}
BOOL CGUI_Fantasy::OnSpeechEnableMsg(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnSpeechEnableMsg]: %d, m_bSpeechSignaled:%d\r\n"),wParam,m_bSpeechSignaled));

	if(wParam)
	{
		//不作时间限制，否则取消待机时导航音无法第一时间出来
		if(m_bSpeechSignaled/* && GetTickCount()-m_dwPowerStateTick>5000*/)
		{
			OnSpeechStart(0);
		}
	}
	else
	{
		//if(m_bSpeechOn && !m_bOnSR)
		if(m_bSpeechOn)
		{
			OnSpeechTerminate();
		}
	}


	return TRUE;
}
UINT CGUI_Fantasy::OnGetAudioSRC(WPARAM wParam, LPARAM lParam)
{
	return m_idCurAudioSRC;
}

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418 || CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_AK7738)
void CGUI_Fantasy::OnAudioCodecMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case AUDIO_SET_ASP_WORKMODE:
		if(m_pAudioCodec)
		{
			m_pAudioCodec->ChangeAspWorkMode(AspMode(lParam));
		}
		break;
	case AUDIO_SET_MAIN_VOLUME:
		{
			//RETAILMSG(1, (TEXT("MSG:[%s]: lParam:%d m_bShowVolBar:%d \r\n"),TEXT(__FUNCTION__), lParam, m_bShowVolBar));
			m_iVolume = (int)lParam;
			if(m_pSmartBarVol && m_bOpenSRC)
			{
				DWORD dwTick = GetTickCount();

				//show volume bar:
				if(m_bManualMute ||
					(IsPowerOn() && m_pCurPanel && m_bShowVolBar && m_dwOpenSRCTick
					&& dwTick-m_dwOpenSRCTick>2000 && dwTick-m_dwSetVolumeTick<1000)
					)
				{
					if(!m_bOnReverseMode && m_pCurPanel->IsEnableSmartBar())
					{
						m_pSmartBarVol->Show(TRUE,4000);
						if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
						{
							m_pSmartBarMedia->Show(FALSE);
						}
					}
				}
			}

			if(m_pStatusBar)
			{
				m_pStatusBar->SetVolume(m_iVolume);
			}

			::PostMessage(g_hUIWnd,WM_VOLUME_CHANGE,m_iVolume,1);
		}
		break;
	case AUDIO_SET_ALT_VOLUME:
		{
			//RETAILMSG(1, (TEXT("MSG:[%s]: lParam:%d m_bShowVolBar:%d \r\n"),TEXT(__FUNCTION__), lParam, m_bShowVolBar));
			m_iVolume = (int)lParam;
			if(m_pSmartBarVol && m_bOpenSRC)
			{
				DWORD dwTick = GetTickCount();

				//show volume bar:
				if(m_bManualMute ||
					(IsPowerOn() && m_pCurPanel && m_bShowVolBar && m_dwOpenSRCTick
					&& dwTick-m_dwOpenSRCTick>2000 && dwTick-m_dwSetVolumeTick<1000)
					)
				{
					if(!m_bOnReverseMode && m_pCurPanel->IsEnableSmartBar())
					{
						m_pSmartBarVol->Show(TRUE,4000);
						if(m_pSmartBarMedia && m_pSmartBarMedia->IsVisible())
						{
							m_pSmartBarMedia->Show(FALSE);
						}
					}
				}
			}

			if(m_pStatusBar)
			{
				m_pStatusBar->SetVolume(m_iVolume);
			}

			::PostMessage(g_hUIWnd,WM_VOLUME_CHANGE,m_iVolume,0);
		}
		break;
	default:
		break;
	}
}
#endif

BOOL CGUI_Fantasy::OnSwitchAudioSRC(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet=FALSE;

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	RETAILMSG(DEBUG_GUI,(_T("MSG:[%s]: wParam:0x%x,lParam:0x%x, m_idCurAudioSRC:0x%x\r\n"),TEXT(__FUNCTION__),wParam,lParam,m_idCurAudioSRC));

	if(IsPowerOn() && m_pAudioCodec && wParam!=SRC_AUDIO_INVALID && (lParam || m_idCurAudioSRC != wParam))
	{
		//Notify MCU
		BOOL bSpecialMode = (m_idCurAudioSRC == SRC_AUDIO_INVALID)?TRUE:FALSE;
		if (m_pCommander)
		{
			//Mute AMP
			if (bSpecialMode)
			{
				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: =>SendCmd _ARM_DISABLE_INTERNAL_AMP 1 \r\n")));
				m_pCommander->SendCmd(_ARM_DISABLE_INTERNAL_AMP, 1);
			}
			m_pCommander->SendCmd(_ARM_AUDIO_SW_KEY,BYTE(wParam),TRUE);
		}

		m_pAudioCodec->SetMainVolume(0);
		UINT iMuteDelay = ((lParam&0xFFFF00)>>8);
		if(m_bPhoneCallActive)
			m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolBluetooth);
		else
			m_pAudioCodec->SetMainVolumeTarget(m_pConfig->GetCurParam()->dwVolMedia);

		m_iVolumeFadeTo = m_pAudioCodec->GetMainVolumeTarget();

		m_pAudioCodec->MuteMainAudio(TRUE, FALSE, TRUE, iMuteDelay+400,5);
		m_pAudioCodec->RequestAudioBus(BYTE(wParam));
		m_idCurAudioSRC = wParam;
		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: %d Done!\r\n"),wParam));

		if (m_pCommander && bSpecialMode)
		{
			//Unmute AMP
			RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: =>SendCmd _ARM_DISABLE_INTERNAL_AMP 0 \r\n")));
			m_pCommander->SendCmd(_ARM_DISABLE_INTERNAL_AMP, 0);
		}

		bRet = TRUE;
	}
#else
	static INT iTrebleValue;
	BOOL bAutoMute = (lParam&0xFF);
	UINT iMuteDelay = ((lParam&0xFFFF00)>>8);
	BOOL bUnmuteNoFade = !((lParam&0xFF000000)>>24);

	if(iMuteDelay == 0)
	{
		iMuteDelay = 400;
	}

	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: wParam:%d,lParam:0x%x\r\n"),wParam,lParam));
	if(IsPowerOn() && wParam!=SRC_AUDIO_INVALID && (bAutoMute || m_idCurAudioSRC != wParam))
	{

//	        if(m_bSpeechOn)
//	        {
//	            OnSpeechTerminate();
//	        }

		//auto mute:
		if(bAutoMute && /*(!m_bSpeechOn || m_bOnSR) &&*/ !m_bPhoneCallActive)
		{
			Mute(TRUE,FALSE);
		}

		if(m_idCurAudioSRC != wParam || m_idCurMCUSRC != wParam)
		{
			if(m_pCommander)
			{
				if(m_pCommander->SendCmd(_ARM_AUDIO_SW_KEY,BYTE(wParam),TRUE))
				{
					m_idCurAudioSRC = wParam;
					RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: %d Done!\r\n"),wParam));
				}
			}
		}

		bRet=TRUE;

		//auto unmute:
		if(bAutoMute && /*(!m_bSpeechOn || m_bOnSR) && */!m_bPhoneCallActive)
		{
            if(m_bOnSR)
            {
				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: m_bOnSR do not auto release mute.\r\n")));

				//do not auto release mute.
				//just wait for CARPLAY_SIRI_AUDIO_FORMAT message
                //	                RETAILMSG(1, (TEXT("MSG:OnMuteMsg: auto unmute\r\n")));
                //	                //Mute(FALSE,FALSE);
                //				    OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<24)+(1<<16)+(150));
            }
            else 
			{
				//both check ui and src:
				UINT idUI=m_pCurPanel?m_pCurPanel->GetID():0;
				UINT idSRC=m_pConfig->GetCurParam()->idCurSRC;
				BOOL bAutoUnmute =  FALSE;
				/*if(idUI==0)
				{
					bAutoUnmute = TRUE;
				}
				else*/ if(IsPanelHasValidAudioSrc(idUI))
				{
					bAutoUnmute =  (m_bOnTAMode) || IsPanelAutoUnMute(idUI);
				}
				else
				{
					bAutoUnmute =  (m_bOnTAMode) || IsPanelAutoUnMute(idSRC);
				}
				RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: bAutoUnmute=%d,Fade=%d, iMuteDelay=%d, idUI=0x%x,idSRC=0x%x\r\n"),
					bAutoUnmute,!bUnmuteNoFade,iMuteDelay,idUI,idSRC));
				if(bAutoUnmute)
				{
					OnMuteMsg((1<<24)+(0<<16)+_ARM_MUTE_ALL,(bUnmuteNoFade<<24)+(1<<16)+(/*400*/iMuteDelay));
				}
			}
		}
	}
    else
    {
	    RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnSwitchAudioSRC]: skip!!!  wParam:%d,lParam:0x%x,m_idCurAudioSRC:%d\r\n"),wParam,lParam,m_idCurAudioSRC));
    }
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	BOOL bPlugin = (m_idCurAudioSRC == SRC_AUDIO_TV);
	m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_TV,0,UIOPRT_SETSTATUS,&bPlugin);
#endif
	return bRet;
}
void CGUI_Fantasy::OnRadioCommand(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_RADIO,PAGE_RADIO_WRAPPER));
	if(panel)
	{
		((CPanelRadioWrapper*)panel)->OnRadioCommand(wParam,lParam);
	}
}
void CGUI_Fantasy::OniPodCommand(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_WRAPPER));
	if(panel)
	{
		((CPaneliPodWrapper*)panel)->OniPodCommand(wParam,lParam);
	}
}

void CGUI_Fantasy::OnFactoryCmd(WPARAM wParam, LPARAM lParam)
{
	//panels process:
	m_listPanels.Head();
	while (!m_listPanels.End())
	{
		m_listPanels.Get()->OnFactoryCmd(wParam,lParam);
		m_listPanels.Next();
	}
}

void CGUI_Fantasy::OniPodMsg(WPARAM wParam, LPARAM lParam)
{
	//RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OniPodMsg]: wParam:%d,lParam:%d\r\n"),wParam,lParam));

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_WRAPPER));
	if(panel)
	{
		((CPaneliPodWrapper*)panel)->OniPodMsg(wParam,lParam);
	}
}
void CGUI_Fantasy::OnActive(WPARAM wParam, LPARAM lParam)
{
	BOOL fActive = LOWORD(wParam);
	BOOL fMinimized = (BOOL)HIWORD(wParam);
	HWND hwndPrevious = (HWND) lParam;
	//WA_INACTIVE
	//WA_ACTIVE
	//WA_CLICKACTIVE

	RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::OnActive]: fActive:%d,fMinimized:%d\r\n"),fActive,fMinimized));

	m_bActivated = fActive;

	if(fActive)
	{
		//CeSetThreadPriority(GetCurrentThread(), CE_THREAD_PRIO_256_ABOVE_NORMAL);
	}
	else
	{
		//CeSetThreadPriority(GetCurrentThread(), CE_THREAD_PRIO_256_NORMAL);
	}

}


void CGUI_Fantasy::OnVolumeChange(DWORD dwVolume, UINT uMainVolChannel)
{
 	//RETAILMSG(DEBUG_GUI,(_T("MSG:OnVolumeChange: uMainVolChannel[%d] dwVolume[%d] m_iVolumeFadeTo[%d] m_bMuteOperating[%d] m_bNeedUpdateVolLED[%d] IsPowerOn[%d] Tick()[%d]\r\n"),uMainVolChannel,dwVolume,m_iVolumeFadeTo,m_bMuteOperating,m_bNeedUpdateVolLED,IsPowerOn(),GetTickCount()-m_dwSetVolumeTick));
 	//RETAILMSG(DEBUG_GUI,(_T("MSG:OnVolumeChange: m_bMuteOperating[%d] m_bNeedUpdateVolLED[%d] m_bMuteOperating[%d] m_dwSetVolumeTick[%d] IsPowerOn[%d] Tick()[%d]\r\n"),m_bMuteOperating,m_bNeedUpdateVolLED,m_bMuteOperating,m_dwSetVolumeTick,IsPowerOn(),GetTickCount()-m_dwSetVolumeTick));
	//save volumes:OnVolumeChange
	if(IsPowerOn() && !m_bMuteOperating && m_bNeedUpdateVolLED && GetTickCount()-m_dwSetVolumeTick<1000)
	{
		//update setting UI:
		CPanelGL *panel;
		panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));

        //RETAILMSG(DEBUG_GUI,(_T("MSG:OnVolumeChange: m_bPhoneCallActive:%d m_bSpeechOn:%d m_bSpeechVolumeChange:%d\r\n"),m_bPhoneCallActive,m_bSpeechOn,m_bSpeechVolumeChange));

		if(m_bPhoneCallActive && uMainVolChannel == 1)
		{
 			//RETAILMSG(DEBUG_GUI,(_T("MSG:OnVolumeChange: update dwVolBluetooth :%d\r\n"),m_iVolumeFadeTo));
            //if(m_pConfig->GetCurParam()->dwVolBluetooth != m_iVolumeFadeTo)
            {
    		    //m_pConfig->GetCurParam()->dwVolBluetooth = m_iVolumeFadeTo; //dwVolume;

    			if(panel)
    			{
    				((CPanelSettingWrapper*)panel)->OnVolumeConfigMsg(1,m_pConfig->GetCurParam()->dwVolBluetooth);
                    UpdateFrame();
    			}
            }
            m_bMainVolumeChange = FALSE;
		}
		else if(/*(m_bSpeechOn && IsNaviSpeechEnable())|| */(uMainVolChannel == 0))
		{
 			//RETAILMSG(DEBUG_GUI,(_T("MSG:OnVolumeChange: update dwVolGPS :%d\r\n"),m_pConfig->GetCurParam()->dwVolGPS));
            //if(m_pConfig->GetCurParam()->dwVolGPS != m_iVolumeFadeTo)   //dwVolume;
            {
                //m_pConfig->GetCurParam()->dwVolGPS = m_iVolumeFadeTo;   //dwVolume;
    			if(panel)
    			{
    				((CPanelSettingWrapper*)panel)->OnVolumeConfigMsg(2,m_pConfig->GetCurParam()->dwVolGPS);
                    UpdateFrame();
    			}
            }
		}
		else if(uMainVolChannel == 1)
		{
			//RETAILMSG(DEBUG_GUI,(_T("MSG:OnVolumeChange: update dwVolMedia :%d, cur: %d\r\n"),m_iVolumeFadeTo,dwVolume));
            //if(m_pConfig->GetCurParam()->dwVolMedia != m_iVolumeFadeTo) //dwVolume;
            {
                //m_pConfig->GetCurParam()->dwVolMedia = m_iVolumeFadeTo; //dwVolume;
    			if(panel)
    			{
                    if(m_bOnSR && (GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurUI) == UI_CLASS_CARPLAY))
                    {
    				    ((CPanelSettingWrapper*)panel)->OnVolumeConfigMsg(2,m_pConfig->GetCurParam()->dwVolGPS);
                    }
                    else
                    {
    				    ((CPanelSettingWrapper*)panel)->OnVolumeConfigMsg(0,m_pConfig->GetCurParam()->dwVolMedia);
                    }
                    UpdateFrame();
    			}
            }
		}
	}

	//led
 	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeChange]: m_bManualMute[%d] m_bNeedUpdateVolLED[%d] m_iVolumeFadeTo[%d] \r\n"),m_bManualMute,m_bNeedUpdateVolLED,m_iVolumeFadeTo));
	if((m_bManualMute||m_bNeedUpdateVolLED) && (IsPowerOn()?TRUE:m_iVolumeFadeTo==0))
	{
        //RETAILMSG(1, (TEXT("MSG:[%s] =================1\r\n"), TEXT(__FUNCTION__)));
//			if(m_iVolumeFadeTo == 0)
//			{
//				if(m_pLED)
//				{
//	                //RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(0) 6\r\n"), TEXT(__FUNCTION__)));
//					m_pLED->SetLedParam(0,0);
//				}
//			}
//			else
		{
            //RETAILMSG(1, (TEXT("MSG:[%s] =================2 m_bSpeechOn[%d] dwVolGPS[%d]\r\n"), TEXT(__FUNCTION__),m_bSpeechOn,m_pConfig->GetCurParam()->dwVolGPS));
			#if CVTE_EN_LED
			if(m_pLED)
			{
				DWORD dwVol=0;
				if(m_bPhoneCallActive)
				{
					dwVol=m_pConfig->GetCurParam()->dwVolBluetooth;
				}
				else if(m_bSpeechOn)
				{
					dwVol=m_pConfig->GetCurParam()->dwVolGPS;
				}
				else
				{
					dwVol=m_pConfig->GetCurParam()->dwVolMedia;
				}

				int iset=GetNearInt((dwVol)*7.0f/32.0f);
				if(iset<0)
					iset=0;
				else if(iset>7)
					iset=7;
                //RETAILMSG(1, (TEXT("MSG:[%s] SetLedParam(%d) 7\r\n"), TEXT(__FUNCTION__), iset));
				m_pLED->SetLedParam(0,iset);
			}
			#endif
		}
	}

}
BOOL CGUI_Fantasy::OnPanelChange(SWITCH_PAGE_PARAM *para)
{
	if(para)
	{
		UINT idPanel = MAKE_PANEL_ID(para->idClass,para->idPage);
		UINT iParam = para->pageParam;
		UINT idSwitchAniId = para->idSwitchAniId;
		LPVOID lpAniPara = para->lpAniPara;
		BOOL bUserInteraction = para->bUserInteraction;

		//release buff:
		delete para;
		para=NULL;

		return OnPanelChange(idPanel,iParam,idSwitchAniId,lpAniPara,bUserInteraction);
	}
	else
	{
		return FALSE;
	}
}
BOOL CGUI_Fantasy::OnPanelChange(UINT idPanel, UINT iParam,UINT idSwitchAniId,LPVOID lpAniPara,BOOL bUserInteraction)
{
	UINT idClass=GET_PANEL_CLASS_ID(idPanel);

	//RETAILMSG(1,( (L"MSG:[%s]: idPanel[%X] iParam[%X]\r\n"), idPanel,iParam));

	//disable page switch while manual power off
	if(!m_bManualPowerOn
		&& idClass!=UI_CLASS_POWERSTATE
		&& idClass!=UI_CLASS_IDLE
		&& idClass!=UI_CLASS_SETMAC
		&& idClass!=UI_CLASS_REARVIEW
		&& idClass!=UI_CLASS_CARPLAY
		&& idClass!=UI_CLASS_ANDROID_AUTO
		&& idClass!=UI_CLASS_UPDATE
		&& idPanel!=MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING)
		)
	{
		return FALSE;
	}

	if(m_bOnReverseMode && !m_bOpenSRC)
	{
		RETAILMSG(1,( (L"####ReverseMode ON, cancel panel change: 0x%x \r\n"), idPanel));

		return FALSE;
	}

	CPanelGL *pPanel;

	OpenPanels(idPanel);

	pPanel= m_listPanels.GetItemByID(idPanel);

	if(!pPanel)
	{
		return FALSE;
	}

	if(m_pCurPanel && m_pCurPanel!=pPanel)
	{
		RETAILMSG(1,( (L"current panel enable quit: 0x%x, %d\r\n"), m_pCurPanel->GetID() , m_pCurPanel->IsEnableQuit()));

		if(idClass!=UI_CLASS_POWERSTATE
			&& idClass!=UI_CLASS_REARVIEW
			&& idPanel!=MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING)
			&& idClass!=UI_CLASS_CARPLAY
#if CVTE_EN_EASYCONNECT
			&& idClass!=UI_CLASS_EASYCONNECTED
#endif
#if CVTE_EN_WEBLINK
			&& idClass!=UI_CLASS_WEBLINK
#endif
			)//always change when panel is UI_CLASS_POWEROFF & UI_CLASS_REARVIEW
		{
			if(!m_pCurPanel->IsEnableQuit())
			{
				RETAILMSG(DEBUG_GUI,( (L"current panel can't quit now: 0x%x\r\n"), m_pCurPanel->GetID() ));
				return FALSE;
			}
// 			else if(m_bOnReverseMode && idPanel!=MAKE_PANEL_ID(UI_CLASS_NAVI,0))
// 			{
// 				RETAILMSG(DEBUG_GUI,( (L"OnReverseMode panel can't quit now: %d\r\n"), m_pCurPanel->GetID() ));
// 				return FALSE;
// 			}
// 			else if(!m_pCurPanel->IsAniReady())
// 			{
// 				RETAILMSG(DEBUG_GUI,( (L"current panel not ready, can't quit now: %d\r\n"), m_pCurPanel->GetID() ));
// 				return FALSE;
// 			}
		}
	}

	RETAILMSG(1/*DEBUG_GUI*/,( (L"MSG:[%s]:0x%x, para:%d, ani:%d, cur:0x%x, user=%d##\r\n"),TEXT(__FUNCTION__),
		idPanel, iParam, idSwitchAniId,m_pCurPanel?m_pCurPanel->GetID():0, bUserInteraction ));

	if(pPanel)
	{
// 		m_dwLastClickTick = GetTickCount();
		if(m_pSmartBarVol)
		{
			if(m_pSmartBarVol->IsVisible())
			{
				m_pSmartBarVol->Show(FALSE);
			}
		}
		if(m_pSmartBarMedia)
		{
			if(m_pSmartBarMedia->IsVisible())
			{
				m_pSmartBarMedia->Show(FALSE);
			}
		}
		if(m_pSmartBarTopBanner)
		{
			if(m_pSmartBarTopBanner->IsVisible())
			{
				m_pSmartBarTopBanner->Show(FALSE);
			}
		}

		if(GET_PANEL_CLASS_ID(idPanel) == UI_CLASS_UPDATE_ARM)
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: Stop all media.\r\n"),TEXT(__FUNCTION__)));
			//pause/play media:
			UINT idClass=GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC);
			CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(idClass,-1));
			if(panel)
			{
				if(idClass != UI_CLASS_BT && idClass != UI_CLASS_RADIO)//not operate play status while current is A2DP or RADIO
				{
					#if CVTE_EN_KEY_PROXY
					CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PAUSE);
					CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PAUSE);
					#else
					panel->OnKey(KEY_MEDIA_PAUSE);
					#endif
				}
			}
		}

		//
		UpdateFrame();

#if CVTE_EN_CARPLAY
		//user click to select audio source or native ui, update carplay ini-modes:
		if(bUserInteraction)
		{
			CCarplayAdapter::GetInstance()->SetUserSelectNativeUI(TRUE);
			if(IsPanelHasValidAudioSrc(idPanel))
			{
				CCarplayAdapter::GetInstance()->SetUserSelectAudioSRC(TRUE);
			}
		}
#endif

		//如果当期的panel和要切换的不相同
		if(m_pCurPanel!=pPanel)
		{
			//if(m_pCurPanel)
			{
				//get previous src:
				UINT id_cur=m_pCurPanel?m_pCurPanel->GetID():0;

				UINT ani_out = ANIM_NONE;
				UINT ani_in = ANIM_NONE;

				switch (idSwitchAniId)
				{
				case PAGE_SWITCH_ANI_DEFAULT:
					{
						if(m_pCurPanel)
						{
							if(m_pCurPanel->GetAniMoveOut()!=ANIM_NONE)
							{
								ani_out = m_pCurPanel->GetAniMoveOut();
								ani_in = m_pCurPanel->GetAniNextPanelMoveIn();
							}
							else
							{
								ani_out = pPanel->GetAniPrePanelMoveOut();
								ani_in = pPanel->GetAniMoveIn();
							}
						}

						//set default para://
						if(!lpAniPara && (ani_in==ANIM_ZOOMIN_IN||ani_in==ANIM_ZOOMOUT_IN))
						{
							CPanelHomePage *home=(CPanelHomePage *)m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER));
							if(home)
							{
								UINT idp=(idPanel==MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER))?(m_pCurPanel?m_pCurPanel->GetID():0):(idPanel);
								switch (GET_PANEL_CLASS_ID(idp))
								{
								case UI_CLASS_MEDIA:
									#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
									lpAniPara = home->GetWidget(IDC_WIDGET_USB);
									#else
									lpAniPara = home->GetWidget(IDC_WIDGET_MEDIA);
									#endif
									break;
								case UI_CLASS_RADIO:
									lpAniPara = home->GetWidget(IDC_WIDGET_RADIO);
									break;
								case UI_CLASS_NAVI:
								case UI_CLASS_NAVI_SIMULATE:
									lpAniPara = home->GetWidget(IDC_WIDGET_NAVI);
									break;
								case UI_CLASS_CARPLAY:
									lpAniPara = home->GetWidget(IDC_WIDGET_CARPLAY);
									break;
								case UI_CLASS_IPOD:
									lpAniPara = home->GetWidget(IDC_WIDGET_IPOD);
									break;
								case UI_CLASS_PHONELINK:
									lpAniPara = home->GetWidget(IDC_WIDGET_PHONELINK);
									break;
								case UI_CLASS_TV:
									lpAniPara = home->GetWidget(IDC_WIDGET_TV);
									break;
								case UI_CLASS_DISK:
									lpAniPara = home->GetWidget(IDC_WIDGET_DISK);
									break;
								case UI_CLASS_AUX:
									lpAniPara = home->GetWidget(IDC_WIDGET_AUX);
									break;
								case UI_CLASS_ISDB:
									lpAniPara = home->GetWidget(IDC_WIDGET_ISDB);
									break;
								case UI_CLASS_EASYCONNECTED:
									lpAniPara = home->GetWidget(IDC_WIDGET_EASYCONNECTED);
									break;
								case UI_CLASS_WEBLINK:
									lpAniPara = home->GetWidget(IDC_WIDGET_WEBLINK);
									break;
								case UI_CLASS_ANDROID_AUTO:
									lpAniPara = home->GetWidget(IDC_WIDGET_ANDROID_AUTO);
									break;
								case UI_CLASS_SETTING:
									lpAniPara = home->GetWidget(IDC_WIDGET_SETTING);
									break;

								case UI_CLASS_BT:
									if(GET_PANEL_PAGE_ID(idp) == PAGE_BT_A2DP)
									{
										#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
										lpAniPara = home->GetWidget(IDC_WIDGET_A2DP);
										#else
										lpAniPara = home->GetWidget(IDC_WIDGET_MEDIA);
										#endif
									}
									else
									{
										lpAniPara = home->GetWidget(IDC_WIDGET_PHONE);
									}
									break;

								case UI_CLASS_VEHICLEINFO:
									lpAniPara = home->GetWidget(IDC_WIDGET_VEHICLEINFO);
									break;
								case UI_CLASS_DAB:
#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
									lpAniPara = home->GetWidget(IDC_WIDGET_DAB);
#else
									lpAniPara = home->GetWidget(IDC_WIDGET_RADIO);
#endif
									break;
								default:
									RETAILMSG(1, (TEXT("WAR:[%s]: GET_PANEL_CLASS_ID(idp) has not case\r\n"),TEXT(__FUNCTION__)));
									break;
								}
							}
						}
						//end of set default para//
					}
					break;
				case PAGE_SWITCH_ANI_LEFT:
					{
						ani_out = ANIM_LINE_LEFT_OUT;
						ani_in = ANIM_LINE_RIGHT_IN;
					}
					break;
				case PAGE_SWITCH_ANI_RIGHT:
					{
						ani_out = ANIM_LINE_RIGHT_OUT;
						ani_in = ANIM_LINE_LEFT_IN;
					}
					break;
				case PAGE_SWITCH_ANI_DOWN:
					{
						ani_out = ANIM_LINE_BOTTOM_OUT;
						ani_in = ANIM_LINE_TOP_IN;
					}
					break;
				case PAGE_SWITCH_ANI_UP:
					{
						ani_out = ANIM_LINE_TOP_OUT;
						ani_in = ANIM_LINE_BOTTOM_IN;
					}
					break;
				case PAGE_SWITCH_ANI_ZOOMIN:
					{
						ani_out = ANIM_ZOOMIN_OUT;
						ani_in = ANIM_ZOOMIN_IN;
					}
					break;
				case PAGE_SWITCH_ANI_ZOOMOUT:
					{
						ani_out = ANIM_ZOOMOUT_OUT;
						ani_in = ANIM_ZOOMOUT_IN;
					}
					break;
				}

// 				RETAILMSG(1,( (L"MSG: [CGUI_Fantasy::OnPanelChange]: ani_out:%d ,ani_in:%d, lpAniPara:0x%x##\r\n"), ani_out, ani_in,lpAniPara ));
// 				if(lpAniPara)
// 				{
// 					CControlGL* obj=(CControlGL*)lpAniPara;
// 					RECT rc=obj->GetScreenRect();
// 					RETAILMSG(1,( (L"id:%d, left: %d, top: %d, right: %d, bottom: %d\r\n"), obj->GetID(),rc.left,rc.top,rc.right,rc.bottom));
// 				}

				//store current:
				m_pConfig->GetCurParam()->idPreUI=id_cur;

				//
				if(m_pCurPanel)
				{
					m_pCurPanel->OnQuitPanel(iParam,ani_out,lpAniPara);
				}

				//set last panel:
				if(m_pCurPanel && GET_PANEL_CLASS_ID(id_cur)>UI_CLASS_NULL && GET_PANEL_CLASS_ID(id_cur)<UI_CLASS_MAX_VALID)
				{
					pPanel->SetPreviousPanel(m_pCurPanel->GetID(),m_pCurPanel->GetPanelParam());
				}
				else
				{
					pPanel->SetPreviousPanel(m_pConfig->GetCurParam()->idCurSRC,0);
				}

				//
				pPanel->SetSwitchInControl((ani_out!=ANIM_NONE||ani_in!=ANIM_NONE)?m_pCurPanel:NULL);

				//hide smartbar phone if page is android auto.
				if(m_pSmartBarPhone)
				{
					if(GET_PANEL_CLASS_ID(idPanel) == UI_CLASS_ANDROID_AUTO && m_pSmartBarPhone->IsVisible())
					{
						m_pSmartBarPhone->Show(FALSE);
					}
				}

				//get new panel:
				m_pCurPanel = pPanel;


				//action:
				m_pSkinManager->GetGUI()->EnableSwap(TRUE);
				//OnEnterPanel调用相应页面的OnEnterPanel
				m_pCurPanel->OnEnterPanel(iParam,ani_in,lpAniPara);

				//
				if(!IsWindowVisible(g_hMainWnd))
				{
					RETAILMSG(DEBUG_GUI, (TEXT("MSG:[%s]: show main window\r\n"), TEXT(__FUNCTION__)));
					ShowWindow(g_hMainWnd,SW_SHOW);
				}
				SetForegroundWindow(g_hMainWnd);

				//get current src:////////////////////////////////////
				id_cur=m_pCurPanel->GetID();
				if(IsPanelAutoSwitchMediaSRC(id_cur))
				{
					//RETAILMSG(1, (TEXT("MSG:[%s]: Auto Switch Media SRC \r\n"), TEXT(__FUNCTION__)));
					SwitchMediaClassSRC(id_cur,m_pCurPanel->GetPanelParam());
				}
				m_pConfig->GetCurParam()->idCurUI = id_cur;
				RETAILMSG(1, (TEXT("MSG:[%s]: idCurUI=[%X] \r\n"), TEXT(__FUNCTION__),m_pConfig->GetCurParam()->idCurUI));

				/////////////////////////////////////
				//notify UI changed:
				m_listPanels.Head();
				while (!m_listPanels.End())
				{
					m_listPanels.Get()->OnUIChanged(id_cur);
					m_listPanels.Next();
				}
				////////////////////////////////////////////////////
			}
		}
		else
		{
			if(m_pCurPanel)
			{
				m_pConfig->GetCurParam()->idCurUI = m_pCurPanel->GetID();
				if(iParam != m_pCurPanel->GetPanelParam())
				{
					m_pCurPanel->OnEnterPanel(iParam);
				}
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void  CGUI_Fantasy::OnNotifyVolumeInfo(WPARAM wParam, LPARAM lParam)
{
	g_wParamVolumeInfo=wParam;
	g_lParamVolumeInfo=lParam;

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
	if(panel)
	{
		((CPanelVehicleInfoWrapper*)panel)->NotifyVolumeInfo(wParam,lParam);
	}
}
void CGUI_Fantasy::OnNotifySourceInfo(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnNotifySourceInfo]: %d ++\r\n"),wParam));

	g_wParamSourceInfo=wParam;
	g_lParamSourceInfo=lParam;

	if(wParam<MEDIA_TYPE_NUMBER)
	{
		m_idCurSrc = wParam;
	}

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
	if(panel)
	{
		((CPanelVehicleInfoWrapper*)panel)->NotifySourceInfo((BYTE)wParam,(BYTE)lParam);
	}
}
void CGUI_Fantasy::OnNotifyMediaPlaybackInfo(WPARAM wParam, LPARAM lParam)
{
	g_wParamMediaPlaybackInfo=wParam;
	g_lParamMediaPlaybackInfo=lParam;

	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
	if(panel)
	{
		((CPanelVehicleInfoWrapper*)panel)->NotifyMediaPlaybackInfo(wParam,lParam);
	}
}
void CGUI_Fantasy::OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam)
{
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_NAVI,0));
	if(panel)
	{
		((CPanelApp*)panel)->OnNotifyRdsTAStatus(wParam,lParam);
	}
	#if CVTE_EN_CARPLAY
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER));
	if(panel)
	{
		((CPanelCarPlay*)panel)->OnNotifyRdsTAStatus(wParam,lParam);
	}
	#endif
	#if CVTE_EN_ANDROID_AUTO
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER));
	if(panel)
	{
		((CPanelAndroidAuto*)panel)->OnNotifyRdsTAStatus(wParam,lParam);
	}
	#endif
	//auto un-mute:
	if(wParam && m_bManualMute)
	{
		OnMuteKey(FALSE);
	}

	m_bOnTAMode = wParam;
}
void CGUI_Fantasy::OnNotifyRadioInfo(WPARAM wParam, LPARAM lParam)
{
	g_wParamRadioInfo=wParam;
	g_lParamRadioInfo=lParam;


	if(m_pCommander)
	{
		CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER));
		if(panel)
		{
			BYTE preset=m_pCommander->GetItemData(MCU_CUR_CHANNEL_STORE);
			if(preset>5)
				preset=0;
			else
				preset+=1;

			BYTE band=m_pCommander->GetItemData(MCU_CUR_BAND_STORE);
			WORD freq;

			if(band<=3)//FM
			{
				if(band==3)
					band=0;
				else
					band+=1;
				freq=m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_H)*100+m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
			}
			else
			{
				band+=13;
				freq=m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_H)*100+m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
			}

			((CPanelVehicleInfoWrapper*)panel)->NotifyRadioInfo(band,freq,	preset,wParam);
		}
	}

}
void CGUI_Fantasy::OnHandleSRMsg(UINT msg,WPARAM wParam, LPARAM lParam)
{
	/*
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SR,PAGE_SR_MAIN));
	if(panel)
	{
		((CPanelSRMain*)panel)->HandleSRMsg(msg,wParam,lParam);
	}
	*/
}

BOOL CGUI_Fantasy::GPSPowerOn()
{
	if(m_bGPSPowerOn)
	{
		return FALSE;
	}

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::GPSPowerOn] ++\r\n")));

	GPSColdStart();
	//GPSWarmStart();

	m_bGPSPowerOn = TRUE;

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::GPSPowerOn] --\r\n")));

	return TRUE;
}
void CGUI_Fantasy::ScreenPowerOn()
{
/*
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::ScreenPowerOn] ++\r\n")));

		//
		HANDLE hDevTW8836 = CreateFile(TEXT("TWD1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);

		if(hDevTW8836 != INVALID_HANDLE_VALUE)
		{
			TW8836POWERCONTROL input=TW8836_POWER_ON;

			if (!DeviceIoControl(hDevTW8836, TW8836_IOCTL_POWER_CONTROL, &input, sizeof(input),	NULL, 0, NULL, NULL))
			{
				RETAILMSG(DEBUG_GUI, (TEXT("Failed DeviceIoControl TW8836_IOCTL_POWER_CONTROL -- error = 0x%x\r\n"), GetLastError()));
			}
			else
			{
				Sleep(100);
			}

			CloseHandle(hDevTW8836);
		}
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::ScreenPowerOn] --\r\n")));
*/

}
void CGUI_Fantasy::ScreenPowerOff()
{
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::ScreenPowerOff] ++\r\n")));
/*
		//
		HANDLE hDevTW8836 = CreateFile(TEXT("TWD1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);

		if(hDevTW8836 != INVALID_HANDLE_VALUE)
		{
			TW8836POWERCONTROL input=TW8836_POWER_OFF;

			if (!DeviceIoControl(hDevTW8836, TW8836_IOCTL_POWER_CONTROL, &input, sizeof(input),	NULL, 0, NULL, NULL))
			{
				RETAILMSG(DEBUG_GUI, (TEXT("Failed DeviceIoControl TW8836_IOCTL_POWER_CONTROL -- error = 0x%x\r\n"), GetLastError()));
			}
			else
			{

			}

			CloseHandle(hDevTW8836);
		}
*/
		RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::ScreenPowerOff] --\r\n")));
}
BOOL CGUI_Fantasy::GPSPowerOff()
{
	if(!m_bGPSPowerOn)
	{
		return FALSE;
	}

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::GPSPowerOff] ++\r\n")));

	GPSRealShutdown();
	//GPSShutdown();

	m_bGPSPowerOn = FALSE;

	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::GPSPowerOff] --\r\n")));

	return TRUE;
}
void CGUI_Fantasy::EnableGPSMonitor(BOOL bEnable)
{
#if CVTE_EN_NAVI
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::EnableGPSMonitor]: ++ %d\r\n"),bEnable));

	if ((m_hEvtGPSMonitor != NULL) && (m_hEvtGPSMonitor != INVALID_HANDLE_VALUE))
	{
		if(bEnable)
		{
			if(m_hThreadGPSMonitor==INVALID_HANDLE_VALUE || m_hThreadGPSMonitor == NULL)
			{
				m_hThreadGPSMonitor = CreateThread(NULL,NULL,GPSMonitorThreadPro,this,NULL,NULL);
				if ((m_hThreadGPSMonitor != NULL) && (m_hThreadGPSMonitor != INVALID_HANDLE_VALUE))
				{
					CeSetThreadPriority(m_hThreadGPSMonitor, CE_THREAD_PRIO_256_LOWEST);
					m_bEnableGPSMonitor = TRUE;
				}
				else
				{
					RETAILMSG(DEBUG_GUI,(_T("Create TimeAutoAdjust Thread Faile!\r\n")));
				}
			}
			else
			{
				SetEvent(m_hEvtGPSMonitor);
				RETAILMSG(DEBUG_GUI,(_T("++EnableGPSMonitor 1: aready enabled!\r\n")));
			}
		}
		else
		{
			if ((m_hThreadGPSMonitor != NULL) && (m_hThreadGPSMonitor != INVALID_HANDLE_VALUE))
			{
				m_bEnableGPSMonitor = FALSE;
				SetEvent(m_hEvtGPSMonitor);

				DWORD dwExitCode;
				do
				{
					Sleep(10);
					if (!GetExitCodeThread(m_hThreadGPSMonitor, &dwExitCode))
					{
						break;
					}
				} while (dwExitCode == STILL_ACTIVE);

				//
				CloseHandle(m_hThreadGPSMonitor);
				m_hThreadGPSMonitor = INVALID_HANDLE_VALUE;

				//reset speed:
				::PostMessage(g_hUIWnd,WM_VEHICLE_SPEED_MSG,0,NULL);
			}
			else
			{
				RETAILMSG(DEBUG_GUI,(_T("++EnableGPSMonitor 0: already disable!\r\n")));
			}
		}
	}
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::EnableGPSMonitor]: -- %d\r\n"),bEnable));
#endif
}

DWORD CGUI_Fantasy::GPSMonitorThreadPro(LPVOID lpParam)
{
	static DWORD s_dwLoopCnt=0;

	CGUI_Fantasy *pgui = (CGUI_Fantasy*)lpParam;

	DWORD dwRefreshTime = 1000; //2000;

	Sleep(8000);

	RETAILMSG(DEBUG_GUI,(_T("++GPSMonitorThreadPro enter....\r\n")));

	if(!StartGpsApi())
	{
		return 0;
	}

	while( pgui->m_bEnableGPSMonitor )
	{
		//RETAILMSG(DEBUG_GUI,(_T("## GPSMonitorThreadPro wait :%d##\r\n"),dwRefreshTime));

		WaitForSingleObject(pgui->m_hEvtGPSMonitor, dwRefreshTime);

		if(!pgui->m_bEnableGPSMonitor)
			break;

		//power on/off:
		if(pgui->IsPowerOn())
		{
			//pgui->GPSPowerOn();
			dwRefreshTime = 1000;   //2000;
		}
		else
		{
			//pgui->GPSPowerOff();
			dwRefreshTime = INFINITE;
			continue;
		}

		//sync time, if synced, 30min, or not, 30second
		BOOL bNeedSync=(g_dwTimeSyncedCnt==0) && (s_dwLoopCnt%15==0);
// 		if(g_dwTimeSyncedCnt==0)
// 			bNeedSync=TRUE;
// 		else
// 			bNeedSync=(g_dwTimeSyncedCnt>10)?(s_dwLoopCnt%900==0):(s_dwLoopCnt%15==0);

		if(pgui->m_pConfig->GetCurParam()->bTimeAutoAdjust && bNeedSync)
		{
			DWORD starttick = GetTickCount();

			UTCTIME utctime;
			if (GetCurrentUTC(&utctime))
			{
				if (utctime.available)
				{
					SYSTEMTIME systemtime;
					//FILETIME systemfiletime;
					//GetLocalTime(&systemtime);
					systemtime.wYear = utctime.utcYY;
					systemtime.wMonth = utctime.utcMon;
					systemtime.wDay = utctime.utcDD;
					systemtime.wHour = utctime.utcHH;
					systemtime.wMinute = utctime.utcMM;
					double second;
					systemtime.wMilliseconds = (short)(modf(utctime.utcSS, &second)*1000);
					systemtime.wSecond = (short)second;

					SetSystemTime(&systemtime);

					g_dwTimeSyncedCnt ++;

					RETAILMSG(1,(_T(" #### TimeSynced SetSystemTime: %u-%u-%u %u:%u:%u\r\n"),
						systemtime.wYear, systemtime.wMonth,
						systemtime.wDay, systemtime.wHour,
						systemtime.wMinute, systemtime.wSecond));

					/*
					if (SystemTimeToFileTime(&systemtime, &systemfiletime))
					{
						LARGE_INTEGER longsystemfiletime;
						longsystemfiletime.LowPart = systemfiletime.dwLowDateTime;
						longsystemfiletime.HighPart = systemfiletime.dwHighDateTime;
						// 0x430E234000 = 8 hours = 8*60*60*1000*1000*10 ns;
						longsystemfiletime.QuadPart += pgui->m_pConfig->GetCurParam()->iTimeZone*60*60*1000*1000*10;//0x430E234000LL;
						systemfiletime.dwLowDateTime = longsystemfiletime.LowPart;
						systemfiletime.dwHighDateTime = longsystemfiletime.HighPart;

						if (FileTimeToSystemTime(&systemfiletime, &systemtime))
						{
							if (SetLocalTime(&systemtime))
							{
								RETAILMSG(DEBUG_GUI,(_T(" #### TimeSynced SetLocalTime: %u-%u-%u %u:%u:%u\r\n"),
									systemtime.wYear, systemtime.wMonth,
									systemtime.wDay, systemtime.wHour,
									systemtime.wMinute, systemtime.wSecond));

								g_dwTimeSyncedCnt ++;
							}
						}
					}
					*/
				}
			}
// 			dwRefreshTime = bSynced?30000:2000;
			//RETAILMSG(DEBUG_GUI,(_T("APP:MSG:GUI:MAIN: [GPSMonitorThreadPro] #### Sync Time:%d, take %u ms\r\n"), g_dwTimeSyncedCnt,GetTickCount()-starttick));
		}

// 		DWORD dw_t=GetTickCount();

		//get speed:
		LLAPOSITION pos;
		if(GetPosition(&pos))
		{
			if(pos.available)
			{
				double speed = GetGroundSpeed() * 1.852 * 2; //4;
				::PostMessage(g_hUIWnd,WM_VEHICLE_SPEED_MSG,int(speed),NULL);
			}
		}

#if 0
		wchar_t str[16];
		swprintf_s(str,16,_T("%.2f"),speed);
		RETAILMSG(DEBUG_GUI,(_T(" #### GetGroundSpeed %s km/h\r\n"),str));

		//get position:
		LLAPOSITION pos;
		if(GetPosition(&pos))
		{
			if(pos.available)
			{
				wchar_t str[16];
				swprintf_s(str,16,_T("%.2f"),pos.lat);
				RETAILMSG(DEBUG_GUI,(_T(" #### GetPosition lat: %s \r\n"),str));
				swprintf_s(str,16,_T("%.2f"),pos.lon);
				RETAILMSG(DEBUG_GUI,(_T(" #### GetPosition lon: %s \r\n"),str));
				swprintf_s(str,16,_T("%.2f"),pos.alt);
				RETAILMSG(DEBUG_GUI,(_T(" #### GetPosition alt: %s \r\n"),str));
			}
		}
		RETAILMSG(DEBUG_GUI,(_T(" #### take: %d ms \r\n"),GetTickCount()-dw_t));
#endif

		s_dwLoopCnt++;
	}

	StopGpsApi();

	RETAILMSG(DEBUG_GUI,(_T("++GPSMonitorThreadPro quit....\r\n")));

	return 0;
}
void CGUI_Fantasy::Mute(BOOL bMute, BOOL bFade, BOOL bNoSendCmd)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::Mute]: bMute%d,fade:%d,bNoSendCmd:%d\r\n"),bMute,bFade,bNoSendCmd));

	//
	m_bMuteOperating = TRUE;

	RETAILMSG(DEBUG_GUI,(_T("@@@MUTE start: %d,fade:%d\r\n"),bMute,bFade));

	if(bMute)
	{
		m_iVolumeFadeTo = 0;

		m_bShowVolBar=FALSE;
		m_bNeedUpdateVolLED=FALSE;

		//
		if(m_pSmartBarVol)
		{
			if(m_pSmartBarVol->IsVisible())
			{
				m_pSmartBarVol->Show(FALSE);
			}
		}
	}
	else
	{
		m_bShowVolBar=FALSE;//TRUE;//
		m_bNeedUpdateVolLED=TRUE;

//	        RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::Mute]: m_bPhoneCallActive%d,m_bSpeechOn:%d,m_bOnReverseMode:%d\r\n"),
//	            m_bPhoneCallActive,
//	            m_bSpeechOn,
//	            m_bOnReverseMode));
//
//	        RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::Mute]: dwVolBluetooth%d,dwVolGPS:%d,dwVolMedia:%d\r\n"),
//	            m_pConfig->GetCurParam()->dwVolBluetooth,
//	            m_pConfig->GetCurParam()->dwVolGPS,
//	            m_pConfig->GetCurParam()->dwVolMedia));

		int iVolume=0;
		if(m_bPhoneCallActive)
		{
			iVolume = m_pConfig->GetCurParam()->dwVolBluetooth;
		}
        else if(m_bOnSR)
        {
			iVolume = m_pConfig->GetCurParam()->dwVolGPS;
        }
		else if(m_bSpeechOn /*&& !m_bOnSR */&& (IsNaviSpeechEnable()))
		{
			iVolume = min(5,m_pConfig->GetCurParam()->dwVolMedia);
		}
		else
		{
			if(m_bOnReverseMode)
			{
				#if CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
				if(m_pConfig->GetCurParam()->bRearVolControl)
				{
					iVolume = min(REARVIEW_VOLUME_DEFAULT,m_pConfig->GetCurParam()->dwVolMedia);
				}
				else
				{
					iVolume = m_pConfig->GetCurParam()->dwVolMedia;
				}
				#else
				iVolume = min(REARVIEW_VOLUME_DEFAULT,m_pConfig->GetCurParam()->dwVolMedia);
				#endif
			}
			else
			{
				iVolume = m_pConfig->GetCurParam()->dwVolMedia;
			}
		}

		m_iVolumeFadeTo = iVolume;
		//Do not change value 'm_bManualMute' here, because the vol mute icon of SmartBarVol will show
		//When the current volume is zero.
		//m_bManualMute=(m_iVolumeFadeTo==0);

//	        RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::Mute]: m_iVolumeFadeTo:%d m_bManualMute:%d\r\n"),m_iVolumeFadeTo,m_bManualMute));

	}


	if(!bFade)
	{
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		m_iVolumeCur=m_iVolumeFadeTo;
		if (m_pAudioCodec)
		{
			m_pAudioCodec->SetMainVolume(m_iVolumeCur);
		}
		m_bVolumeFadeReady=TRUE;
		DoMute(bMute);
#else
		if(bNoSendCmd)
        {
            int volumecur;
    		m_iVolumeCur=m_iVolumeFadeTo;
            if(m_iVolumeFadeTo == 0)
                volumecur = 1;
            else
                volumecur = m_iVolumeCur;

    		if(m_pCommander)
    		{
    			m_pCommander->SendCmd(_ARM_VOLUME_KEY,volumecur,TRUE);
    		}

    		m_bVolumeFadeReady=TRUE;
            RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::Mute]: Do not send mute cmd.\r\n")));
		}
		else
        {
    		m_iVolumeCur=m_iVolumeFadeTo;

    		if(m_pCommander)
    		{
    			m_pCommander->SendCmd(_ARM_VOLUME_KEY,m_iVolumeCur,TRUE);
    		}

    		m_bVolumeFadeReady=TRUE;
    		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::Mute]: m_iVolumeCur:%d\r\n"),m_iVolumeCur));

    		//Sleep(10);
    		//

            DoMute(bMute);
        }
#endif
		//
		m_bMuteOperating = FALSE;

		//
		OnVolumeFadeReday(bMute);
	}
	else
	{
		if(!bMute)
		{
			m_bAutoUnMute = TRUE;
		}
		else
		{
			m_bAutoMute = TRUE;
		}

		if(m_bVolumeFadeReady)
		{
			//RETAILMSG(DEBUG_GUI,(_T("++SetMainVolume mute :%d,fade:%d, begin timer\r\n"),m_iVolumeFadeTo,bFade));
			m_bVolumeFadeReady=FALSE;
			SetTimer(g_hUIWnd,TIMER_MAIN_VOLUME_FADE,VOLUME_FADE_STEP,NULL);
		}
	}

	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::Mute] end.\r\n")));

}

void CGUI_Fantasy::DoMute(BOOL bMute)
{
    if(m_bMute == bMute)
    {
        //RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::DoMute]: skip!!! m_bMute == bMute == %d\r\n"),m_bMute));
        return;
    }

	if(!bMute && m_idPowerState!=SYS_POWER_ON)
	{
		RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::DoMute]: unmute fail, because power off \r\n")));
		return;
	}
	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::DoMute]: bMute:%d \r\n"),bMute));

    if(bMute)
    {
		//mcu mute:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		//Cut off the onchip output
		waveOutSetVolume(0,0);

		if (m_pAudioCodec)
		{
			m_pAudioCodec->Mute(TRUE);
			m_pAudioCodec->MuteAMP(TRUE);
		}
#else
		if(m_pCommander)
		{
			if(m_idMuteType == _ARM_MUTE_ALL)
			{
				m_pCommander->SendCmd(_ARM_VOLUME_KEY,0,TRUE);
				Sleep(20);
				m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+1);
                RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::DoMute]: mute _ARM_MUTE_ALL\r\n")));
			}
			else if(m_idMuteType == _ARM_MUTE_AMP)
			{
				m_pCommander->SendCmd(_ARM_VOLUME_KEY,0,TRUE);
				Sleep(20);
				m_pCommander->SendCmd(_ARM_MUTE_AMP,0x10+1);
                RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::DoMute]: mute _ARM_MUTE_AMP\r\n")));
			}
            else
            {
                RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::DoMute]: mute fail m_idMuteType:%d\r\n"),m_idMuteType));
            }
		}
		//Cut off the onchip output
		waveOutSetVolume(0,0);
#endif
    }
    else
    {
        //mcu unmute:
//	        RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::DoMute]: m_iVolumeFadeTo:%d m_iVolumeCur:%d dwVolMedia:%d\r\n"),
//	            m_iVolumeFadeTo,
//	            m_iVolumeCur,
//	            m_pConfig->GetCurParam()->dwVolMedia));

		//Open the onchip output
		waveOutSetVolume(0,-1);
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			m_pAudioCodec->Mute(FALSE);
			m_pAudioCodec->MuteAMP(FALSE);
		}
#else
    	if(m_pCommander)
    	{
    		if(m_idMuteType == _ARM_MUTE_ALL)
    		{
    			m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+0);
                RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::DoMute]: unmute _ARM_MUTE_ALL\r\n")));
    		}
    		else if(m_idMuteType == _ARM_MUTE_AMP)
    		{
    			m_pCommander->SendCmd(_ARM_MUTE_AMP,0x10+0);
                RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::DoMute]: unmute _ARM_MUTE_AMP\r\n")));
    		}
            else
            {
                RETAILMSG(1,(_T("MSG:[CGUI_Fantasy::DoMute]: unmute fail m_idMuteType:%d\r\n"),m_idMuteType));
            }
    	}
#endif
    }
    m_bMute = bMute;

	//OnVolumeFadeReday(bMute);

}

#if 0
void CGUI_Fantasy::SetMainVolume(INT iVolume,BOOL bFade,BOOL bShowVolBar,BOOL bLowerVolNoFade,BOOL bUpdateLED)
{
//		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::SetMainVolume]: iVolume[%d] bFade[%d] bShowVolBar[%d] bLowerVolNoFade[%d] bUpdateLED[%d]\r\n"),
//	        iVolume,
//	        bFade,
//	        bShowVolBar,
//	        bLowerVolNoFade,
//	        bUpdateLED));

	if(m_bMuteOperating)
	{
		//quit mute fade:
		m_bMuteOperating = FALSE;
		OnVolumeFadeReday(TRUE);

		m_bAutoMute=FALSE;
		m_bAutoUnMute=FALSE;
	}

	{
		if(iVolume<0)
			iVolume=0;
		if(iVolume>32)
			iVolume=32;

	//
		if(iVolume<m_iVolumeFadeTo && bLowerVolNoFade)
		{
			bFade=FALSE;
		}

		m_iVolumeFadeTo = iVolume;
		m_bShowVolBar=bShowVolBar;
		m_bNeedUpdateVolLED=bUpdateLED;
        m_bMainVolumeChange=TRUE;

//			RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::SetMainVolume]: m_iVolumeFadeTo[%d] m_bShowVolBar[%d] m_bNeedUpdateVolLED[%d] bFade[%d]\r\n"),
//	            m_iVolumeFadeTo,
//	            m_bShowVolBar,
//	            m_bNeedUpdateVolLED,
//	            bFade));

		if(bShowVolBar) //if(bShowVolBar)
		{
			m_dwSetVolumeTick = GetTickCount();
		}

		if(!bFade)
		{

			m_iVolumeCur=m_iVolumeFadeTo;
			if(m_pCommander)
			{
				m_pCommander->SendCmd(_ARM_VOLUME_KEY,m_iVolumeCur,TRUE);
                DoMute(!m_iVolumeCur);
			}
			m_bVolumeFadeReady=TRUE;
		}
		else
		{
            //Add by whh
            if(bFade && iVolume)
            {
                m_bAutoUnMute = TRUE;
            }
            else if(bFade && iVolume == 0)
            {
                m_bAutoMute = TRUE;
            }

			if(m_bVolumeFadeReady)
			{
				//RETAILMSG(DEBUG_GUI,(_T("++++ SetMainVolume :%d,fade:%d, begin timer\r\n"),iVolume,bFade));
				m_bVolumeFadeReady=FALSE;
				SetTimer(g_hUIWnd,TIMER_MAIN_VOLUME_FADE,VOLUME_FADE_STEP,NULL);
			}
			else
			{
				//RETAILMSG(DEBUG_GUI,(_T("++++ SetMainVolume :%d,fade:%d, no need begin timer\r\n"),iVolume,bFade));
			}
		}
	}

}

void CGUI_Fantasy::SetMixVolume(INT iVolume,BOOL bFade,BOOL bShowVolBar,BOOL bLowerVolNoFade,BOOL bUpdateLED)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::SetMixVolume]: iVolume[%d] fade[%d] m_iVolumeCur[%d] m_bNeedUpdateVolLED[%d] bShowVolBar[%d]\r\n"),iVolume,bFade,m_iVolumeCur,m_bNeedUpdateVolLED,bShowVolBar));

//		if(m_bMuteOperating)
//		{
//			//quit mute fade:
//			m_bMuteOperating = FALSE;
//			//OnVolumeFadeReday(TRUE);
//
//			m_bAutoMute=FALSE;
//			m_bAutoUnMute=FALSE;
//		}

	{
		if(iVolume<0)
			iVolume=0;
		if(iVolume>32)
			iVolume=32;

//			//
//			if(iVolume<m_iVolumeFadeTo && bLowerVolNoFade)
//			{
//				bFade=FALSE;
//			}
		m_iVolumeFadeTo = iVolume;
		m_bShowVolBar=bShowVolBar;
		m_bNeedUpdateVolLED=bUpdateLED;
        m_bSpeechVolumeChange = TRUE;
//
//
		//if(bShowVolBar)
		{
			m_dwSetVolumeTick = GetTickCount();
		}

//			if(!bFade)
		{

			//m_iVolumeCur=m_iVolumeFadeTo;
			if(m_pCommander)
			{
				m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,iVolume);
			}
			//m_bVolumeFadeReady=TRUE;
		}
//			else
//			{
//				if(m_bVolumeFadeReady)
//				{
//					//RETAILMSG(DEBUG_GUI,(_T("++++ SetMainVolume :%d,fade:%d, begin timer\r\n"),iVolume,bFade));
//					m_bVolumeFadeReady=FALSE;
//					SetTimer(g_hUIWnd,TIMER_MAIN_VOLUME_FADE,VOLUME_FADE_STEP,NULL);
//				}
//				else
//				{
//					//RETAILMSG(DEBUG_GUI,(_T("++++ SetMainVolume :%d,fade:%d, no need begin timer\r\n"),iVolume,bFade));
//				}
//			}
	}

}
#else

void CGUI_Fantasy::SetVolume(INT iVolume,BOOL bFade,BOOL bShowVolBar,BOOL bLowerVolNoFade,BOOL bUpdateLED, BOOL bMainVolChannel)
{
    //SetVolume(INT iVolume,BOOL bFade,BOOL bShowVolBar=FALSE,BOOL bLowerVolNoFade=TRUE,BOOL bUpdateLED=TRUE, BOOL bMainVolChannel = TRUE);

//		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::SetVolume]: iVolume[%d] bFade[%d] bShowVolBar[%d] bLowerVolNoFade[%d] bUpdateLED[%d] bMainVolChannel[%d] m_iVolumeFadeTo[%d]\r\n"),
//	        iVolume,
//	        bFade,
//	        bShowVolBar,
//	        bLowerVolNoFade,
//	        bUpdateLED,
//	        bMainVolChannel,
//	        m_iVolumeFadeTo));

	if(m_bMuteOperating)
	{
		//quit mute fade:
		m_bMuteOperating = FALSE;
		OnVolumeFadeReday(TRUE);

		m_bAutoMute=FALSE;
		m_bAutoUnMute=FALSE;
	}

	do
	{
		if(iVolume<0)
			iVolume=0;
		if(iVolume>32)
			iVolume=32;

        if(bMainVolChannel)
        {
    		//
    		if(iVolume<m_iVolumeFadeTo && bLowerVolNoFade)
    		{
    			bFade=FALSE;
    		}
        }

        m_iVolumeFadeTo = iVolume;
		m_bShowVolBar=bShowVolBar;
		m_bNeedUpdateVolLED=bUpdateLED;

		//if(bShowVolBar)
		{
			m_dwSetVolumeTick = GetTickCount();
		}

		//Here we must set smartbarvol value.
        if(m_pSmartBarVol && m_bOpenSRC)
		{
			if(bMainVolChannel)
			{
				if(m_bPhoneCallActive)
					m_pSmartBarVol->SetVolume(iVolume,SB_VOLUME_CHANNEL_PHONE);
				else if(m_bSpeechOn || m_bOnSR)
					m_pSmartBarVol->SetVolume(iVolume,SB_VOLUME_CHANNEL_SPEECH);
				else
					m_pSmartBarVol->SetVolume(iVolume,SB_VOLUME_CHANNEL_MEDIA);
			}
			else
			{
				m_pSmartBarVol->SetVolume(iVolume,SB_VOLUME_CHANNEL_SPEECH);
			}
		}

		if(!bFade)
		{
            if(bMainVolChannel)
            {
			    m_iVolumeCur=m_iVolumeFadeTo;
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
				if (m_pAudioCodec)
				{
					m_pAudioCodec->SetMainVolume(m_iVolumeCur);
					DoMute(!m_iVolumeCur);
				}
#else
                //Main volume
    			if(m_pCommander)
    			{
    				m_pCommander->SendCmd(_ARM_VOLUME_KEY,m_iVolumeCur,TRUE);
    			}   
				DoMute(!m_iVolumeCur);
#endif
            }
            else
            {
                m_iVolumeCur=m_iVolumeFadeTo;
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
				if (m_pAudioCodec)
				{
					m_pAudioCodec->SetAltVolume(m_iVolumeCur);
				}
#else
                //Mix volume
                if(m_pCommander)
    			{
    				m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,m_iVolumeCur);
    			}
#endif
				//
				if(m_iVolumeCur && m_pConfig->GetCurParam()->dwVolMedia != 0)
				{
					DoMute(FALSE);
				}
            }
			m_bVolumeFadeReady=TRUE;
		}
		else
		{
            if(bMainVolChannel)
            {
                //Add by whh
                if(bFade && iVolume)
                {
                    m_bAutoUnMute = TRUE;
                }
                else if(bFade && iVolume == 0)
                {
                    m_bAutoMute = TRUE;
                }

    			if(m_bVolumeFadeReady)
    			{
    				//RETAILMSG(DEBUG_GUI,(_T("++++ SetMainVolume :%d,fade:%d, begin timer\r\n"),iVolume,bFade));
    				m_bVolumeFadeReady=FALSE;

                    SetTimer(g_hUIWnd,TIMER_MAIN_VOLUME_FADE,VOLUME_FADE_STEP,NULL);
    			}
    			else
    			{
    				//RETAILMSG(DEBUG_GUI,(_T("++++ SetMainVolume :%d,fade:%d, no need begin timer\r\n"),iVolume,bFade));
    			}
            }
		}
	}while(0);


}

#endif



void CGUI_Fantasy::OnVolumeFadeReday(BOOL bMute)
{
    //RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnVolumeFadeReday]: bMute%d\r\n"),bMute));
	CPanelGL *panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_WRAPPER));
	if(panel)
	{
		((CPanelMediaWrapper*)panel)->OnVolumeFadeReday(bMute);
	}
	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_WRAPPER));
	if(panel)
	{
		((CPanelBluetoothWrapper*)panel)->OnVolumeFadeReday(bMute);
	}
}

DWORD CGUI_Fantasy::OnVolumeFadeProc()
{
	BOOL bRet=FALSE;

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	//adjust volume:
	if(!m_bVolumeFadeReady)
	{
		int min_step=1;
		if(abs(m_iVolumeCur-m_iVolumeFadeTo)>min_step)
		{
			int step=(m_iVolumeFadeTo-m_iVolumeCur)/10;
			if(abs(step)<1)
				step=(m_iVolumeFadeTo-m_iVolumeCur)>0?1:-1;
			m_iVolumeCur += step;//(m_iVolumeFadeTo-m_iVolumeCur)>0?min_step:-min_step;//
			if (m_pAudioCodec)
			{
				m_pAudioCodec->SetMainVolume(m_iVolumeCur);
			}
			bRet=FALSE;
		}
		else
		{
			m_iVolumeCur = m_iVolumeFadeTo;
			//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: SendMuteCmd->reach aim volume:%d\r\n"),m_iVolumeCur));
			if (m_pAudioCodec)
			{
				m_pAudioCodec->SetMainVolume(m_iVolumeCur);
			}
			m_bVolumeFadeReady = TRUE;
			bRet=TRUE;
		}

	}
	else
	{
		bRet=TRUE;
	}

	if(bRet)
	{
		//RETAILMSG(DEBUG_GUI,(_T("## OnVolumeFadeProc end timer##\r\n")));
		KillTimer(g_hUIWnd,TIMER_MAIN_VOLUME_FADE);

		if(m_bMuteOperating)
		{
			m_bMuteOperating = FALSE;
			//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: MUTE end.\r\n")));
		}
	}

	if(m_bAutoMute)
	{
		m_bAutoMute = FALSE;
		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: ## bAutoMute ## :0x%x\r\n"),m_idMuteType));
		DoMute(TRUE);
		OnVolumeFadeReday(TRUE);
	}

	if(m_bAutoUnMute)
	{
		m_bAutoUnMute = FALSE;
		DoMute(FALSE);
		OnVolumeFadeReday(FALSE);
	}

#else
	//adjust volume:
	if(!m_bVolumeFadeReady)
	{
		int min_step=1;
        if(m_bNoSendMuteCmd)
        {
            int volumefadeto;
            if(m_iVolumeFadeTo == 0)
                volumefadeto = 1;
            else
                volumefadeto = m_iVolumeFadeTo;

    		if(abs(m_iVolumeCur-volumefadeto)>min_step)
    		{
    			int step=(volumefadeto-m_iVolumeCur)/10;
    			if(abs(step)<1)
    				step=(volumefadeto-m_iVolumeCur)>0?1:-1;
    			m_iVolumeCur += step;//(m_iVolumeFadeTo-m_iVolumeCur)>0?min_step:-min_step;//

				//Main
				if(m_pCommander)
				{
					//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: SendCmd[2F][%d]\r\n"),m_iVolumeCur));
					m_pCommander->SendCmd(_ARM_VOLUME_KEY,m_iVolumeCur,TRUE);
				}
    			bRet=FALSE;
    		}
    		else
    		{
    			m_iVolumeCur = volumefadeto;
    			//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: NoSendMuteCmd->reach aim volume:%d\r\n"),m_iVolumeCur));
                //Main
                if(m_pCommander)
    			{
    				m_pCommander->SendCmd(_ARM_VOLUME_KEY,m_iVolumeCur,TRUE);
    			}
    			m_bVolumeFadeReady = TRUE;
    			bRet=TRUE;
    		}

        }
        else
        {
    		if(abs(m_iVolumeCur-m_iVolumeFadeTo)>min_step)
    		{
    			int step=(m_iVolumeFadeTo-m_iVolumeCur)/10;
    			if(abs(step)<1)
    				step=(m_iVolumeFadeTo-m_iVolumeCur)>0?1:-1;
    			m_iVolumeCur += step;//(m_iVolumeFadeTo-m_iVolumeCur)>0?min_step:-min_step;//
                //Main
                if(m_pCommander)
    			{
    		        //RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: SendCmd[2F][%d]\r\n"),m_iVolumeCur));
    				m_pCommander->SendCmd(_ARM_VOLUME_KEY,m_iVolumeCur,TRUE);
    			}
    			bRet=FALSE;
    		}
    		else
    		{
    			m_iVolumeCur = m_iVolumeFadeTo;
    			//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: SendMuteCmd->reach aim volume:%d\r\n"),m_iVolumeCur));
                //Main
                if(m_pCommander)
    			{
    				m_pCommander->SendCmd(_ARM_VOLUME_KEY,m_iVolumeCur,TRUE);
    			}
    			m_bVolumeFadeReady = TRUE;
    			bRet=TRUE;
    		}
        }

	}
	else
	{
		bRet=TRUE;
	}

	if(bRet)
	{
		//RETAILMSG(DEBUG_GUI,(_T("## OnVolumeFadeProc end timer##\r\n")));

        //if(bMainVolChannel)
            KillTimer(g_hUIWnd,TIMER_MAIN_VOLUME_FADE);
        //else
        //    KillTimer(g_hUIWnd,TIMER_MIX_VOLUME_FADE);

		if(m_bMuteOperating)
		{
			m_bMuteOperating = FALSE;
			//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: MUTE end.\r\n")));
		}
	}

	//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: m_bAutoUnMute[%d] m_bAutoMute[%d] bRet[%d]\r\n"),m_bAutoUnMute,m_bAutoMute,bRet));
	if(m_bNoSendMuteCmd)
	{
		if(m_bAutoUnMute && (m_iVolumeCur>1/* || bRet*/))
		{
			m_bAutoUnMute = FALSE;

			//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: ## bAutoUnMute ## :0x%x\r\n"),m_idMuteType));

			//mcu unmute
			//if(!m_bNoSendMuteCmd)
		        DoMute(FALSE);

	// 		//
	// 		m_bMuteOperating = FALSE;
	//
			//
			OnVolumeFadeReday(FALSE);
		}
	}
	else
	{
		if(m_bAutoUnMute && (m_iVolumeCur>=1/* || bRet*/))
		{
			m_bAutoUnMute = FALSE;

			//RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: ## bAutoUnMute ## :0x%x\r\n"),m_idMuteType));

			//mcu unmute
			//if(!m_bNoSendMuteCmd)
		        DoMute(FALSE);

	// 		//
	// 		m_bMuteOperating = FALSE;
	//
			//
			OnVolumeFadeReday(FALSE);
		}
	}


	if(m_bAutoMute && (m_iVolumeCur<=1 || bRet))
	{
		m_bAutoMute = FALSE;

		RETAILMSG(DEBUG_GUI,(_T("MSG:[CGUI_Fantasy::OnVolumeFadeProc]: ## bAutoMute ## :0x%x\r\n"),m_idMuteType));

		//mcu mute:

		if(!m_bNoSendMuteCmd)
            DoMute(TRUE);

// 		//
// 		m_bMuteOperating = FALSE;
//
        OnVolumeFadeReday(TRUE);
		//
	}
#endif
	return bRet;
}

void CGUI_Fantasy::OnBackgroundChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_GUI,(_T("MSG: [CGUI_Fantasy::OnBackgroundChanged]: wParam:0x%x\r\n"),wParam));
	CPanelGL *panel;

	panel=m_listPanels.GetItemByID(MAKE_PANEL_ID(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER));
	if(panel)
	{
		((CPanelSettingWrapper*)panel)->OnBackgroundChanged(wParam, lParam);
	}
}


BOOL CGUI_Fantasy::IsNaviSpeechEnable()
{
    BOOL bRet = (g_bNaviSpeechEnabled && m_pConfig->GetCurParam()->dwVolGPS!=0);
    //RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::IsNaviSpeechEnable] bRet:%d g_bNaviSpeechEnabled:%d dwVolGPS:%d\r\n"),bRet,g_bNaviSpeechEnabled,m_pConfig->GetCurParam()->dwVolGPS));
    return bRet;
}

void CGUI_Fantasy::UpdateVolume(UINT uVolume)
{
	if(m_bPhoneCallActive)
	{
        m_iSetVolume = uVolume;
		m_iVolOriginBluetooth =uVolume;
        m_pConfig->GetCurParam()->dwVolBluetooth = uVolume;
        SetVolume(uVolume,FALSE,TRUE);
	}
//		else if(m_bSpeechOn /*&& !m_bOnSR */&& (IsNaviSpeechEnable()))
//		{
//	        //m_iSetVolume = uVolume;
//	        m_pConfig->GetCurParam()->dwVolGPS = uVolume;
//			m_iVolOriginGPS =uVolume;
//	        SetVolume(uVolume,FALSE,TRUE,TRUE,TRUE,FALSE);
//		}
	else
	{
        m_pConfig->GetCurParam()->dwVolMedia = uVolume;
	    m_iSetVolume = uVolume;
		m_iVolOriginMedia =uVolume;
        SetVolume(uVolume,TRUE,TRUE);
	}
}

#if CVTE_EN_ACCON_WARNING
void CGUI_Fantasy::OnEnterWarningPanel(WPARAM wParam, LPARAM lParam)
{
	UINT nShowTime = (UINT)wParam;
	RETAILMSG(1, (TEXT("MSG:[%s]: nShowTime:%d \r\n"), TEXT(__FUNCTION__),nShowTime));

	if(m_pConfig->GetCurParam()->idCurUI != MAKE_PANEL_ID(UI_CLASS_WARNING,PAGE_ACCON_WARNING_MAIN))
	{
		RETAILMSG(1, (TEXT("MSG:[%s]: ==>EnterWarningPage\r\n"), TEXT(__FUNCTION__)));
		SwitchPage(UI_CLASS_WARNING,PAGE_ACCON_WARNING_MAIN,PAGE_SWITCH_ANI_NONE,nShowTime);
	}
}

void CGUI_Fantasy::OnQuitWarningPanel(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnQuitWarningPanel]: \r\n")));


	if(m_pConfig)
	{
		UINT idUI=(m_pConfig->GetCurParam()->idPreUI);
		UINT idClass=GET_PANEL_CLASS_ID(idUI);
		UINT idPage=GET_PANEL_PAGE_ID(idUI);

		if(!(idUI==MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING)&&!m_bPhoneCallActive)
			&& idClass!=UI_CLASS_SR
			&& idClass>UI_CLASS_NULL&&idClass<UI_CLASS_MAX_VALID)//invalid previous panels
		{
			RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnQuitWarningPanel]: SwithPage #1# idClass[%X] idPage[%X]\r\n"),idClass,idPage));
			SwitchPage(idClass,idPage,PAGE_SWITCH_ANI_NONE,0);
		}
		else
		{
			RETAILMSG(1, (TEXT("MSG:[CGUI_Fantasy::OnQuitWarningPanel]: SwithPage #2# idClass[%X] idPage[%d]\r\n"),GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC)));
			SwitchPage(GET_PANEL_CLASS_ID(m_pConfig->GetCurParam()->idCurSRC),
				GET_PANEL_PAGE_ID(m_pConfig->GetCurParam()->idCurSRC),
				PAGE_SWITCH_ANI_NONE,0);
		}
	}
}
#endif


#if CVTE_EN_DYNAMIC_LOGO
void CGUI_Fantasy::RenderDynamicLogoBk()
{
	CLabelGL tagLogo;
	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_065):NULL;
	tagLogo.Initialize(0,NULL,pTexVideoBack);
	tagLogo.SetScale(810.0f/64.0f,490.0f/64.0f,1.0f,FALSE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-g_iScreenWidth*2.0f);

	tagLogo.Render();

	glPopMatrix();

	SwapBuffers();

}
#endif

void CGUI_Fantasy::QuickEnterEQSetting()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: Switch to EQ Setting \r\n"),TEXT(__FUNCTION__)));
	//SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_AUDIO,PAGE_SWITCH_ANI_NONE,0);
	SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_AUDIO);
	PlaySoundEffect(0);
}

void CGUI_Fantasy::OnAudioMuteTimeout()
{
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	if(m_pAudioCodec)
	{
		m_pAudioCodec->OnMainAudioMuteTimeout();
	}
#endif
}

