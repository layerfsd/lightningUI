#pragma once
//#include <bt_sfwe_api.h>
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../Controls/StatusBarEx.h"
#include "../Controls/SmartBarPhone.h"
#include "../Controls/SmartBarVol.h"
#include "../Controls/SmartBarMedia.h"
#include "../Controls/SmartBarDrop.h"
#include "../Controls/SmartBarTopBanner.h"
#include "../Controls/SmartBarWarn.h"
#include "../../VIP/CVideoWindow.h"
#include "../../Backlight/Backlight.h"
#include "../../LED/LED.h"

#include "../Settings/PanelSettingWrapper.h"
#include "../Home/PanelHome.h"
#include "../Explorer/PanelExplorer.h"
#include "../Keyboard/PanelKeyboard.h"
#include "../Notification/PanelNotification.h"
#include "../DeviceMgr/PanelDevConnectMgr.h"
#include "../Idle/PanelIdle.h"
#include "../App/PanelApp.h"
#include "../RearView/PanelRearView.h"
#include "../VehicleInfo/PanelVehicleInfoWrapper.h"
#include "../PowerState/PanelPowerState.h"
#include "../PhoneLink/PanelPhoneLink.h"
#include "../EasyConnected/PanelEasyConnected.h"
#include "../EasyConnected/PanelWebLink.h"
#include "../MHL/PanelMHL.h"
#include "../Media/PanelMediaWrapper.h"
#include "../Radio/PanelRadioWrapper.h"
#include "../Bluetooth FLC/PanelBluetoothWrapper.h"
#include "../Image/PanelImageWrapper.h"
#include "../TV/PanelTV.h"
#include "../SR/PanelSRMain.h"
#include "../Update/PanelUpdate.h"
#include "../Update/PanelUpdateARM.h"
#include "../CarPlay/PanelCarPlay.h"
#include "../../AppConfigProxy/AppConfigProxy.h"
#include "../Auxx/PanelAUX.h"
#include "../ISDB/PanelISDB.h"
#include "../DVD/PanelDVD.h"
#include "../iPod/PaneliPodWrapper.h"

#include "../../CarPlay/CarplayAdapter.h"
#include "../Touch/PanelTouch.h"
#include "../../VIP/CRearOverlay.h"
#include "../Settings/PanelPassword.h"
#include "../../DAB/Adapter/DABAdapter.h"
#include "../DAB/PanelDABMain.h"
#include "../DAB/PanelDABLibrary.h"
#include "../DAB/PanelDABWrapper.h"
#include "../../Gesture/GestureProxy.h"
#include "../Key/CKeyHandler.h"
#include "../Warning/PanelWarning.h"
#include "../SetMAC/PanelSetMAC.h"
#include "../AndroidAuto/PanelAndroidAuto.h"
#include "../App/PanelNaviSimulate.h"
#include "../Media/SndPlayer.h"

//add for audio codec, by wangyiping 20160812
#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418)
#include "../Audio/AudioCodecST7418AIC3254.h"
#endif

#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_AK7738)
#include "../Audio/AK7738/AudioCodecAK7738.h"
#endif

#define DEBUG_GUI				1
//#define END_COM_PROC_THREAD TEXT("END_COM_PROC_THREAD")

#define VOLUME_DEFAULT_NORMAL_VALUE     10
#define VOLUME_DEFAULT_MINI_VALUE       5
#define VOLUME_DEFAULT_MAX_VALUE        20
#define VOLUME_DEFAULT_REVERSE_VALUE    4
#define VOLUME_MAX_VALUE                32

struct MsgData
{
	WPARAM wParam;
	LPARAM lParam;
};

class CGUI_Fantasy :
	public CGUI_OpenglES
{
public:
	CGUI_Fantasy(void);
	~CGUI_Fantasy(void);

public:
	BOOL Initialize(HWND window,MODULEENTRY entry,int iPriority);
	BOOL IsReady(void);
	void Render(void);
	BOOL IsIniUIReady();

	void OnSystemPowerState(UINT idState);
	void OnSystemPrePowerState(UINT idState);

	void RenderWorld(void);
	void RenderPanels(void);
	void RenderStatusBar(void);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(WPARAM wParam, LPARAM lParam);

	BOOL OnPanelChange(SWITCH_PAGE_PARAM *para);
	BOOL OnPanelChange(UINT idPanel, UINT iParam=0,UINT idSwitchAniId=PAGE_SWITCH_ANI_DEFAULT,LPVOID lpAniPara=0,BOOL bUserInteraction=FALSE);
	void OnVolumeChange(DWORD dwVolume, UINT uMainVolChannel = 1);
	void OnRasDialEvent(WPARAM wParam, LPARAM lParam);

	void OnActive(WPARAM wParam, LPARAM lParam);
	BOOL IsPanelOpened(UINT idPanel);
	void OnNaviPathChanged(WPARAM wParam, LPARAM lParam);
	void OnTouchMsg(PMULTIPOINT pCyMultiPoint);
	void OnGestureMsg(WPARAM wParam, LPARAM lParam);

    void OnVolumePlus(int nStep);
    void OnVolumeMinus(int nStep);

#if (CVTE_EN_GESTURE_AIR == 1)
    void OnGestureID(WPARAM wParam, LPARAM lParam);
    void OnGestureCircle(WPARAM wParam, LPARAM lParam);
#endif

	void OnMHLMsg(WPARAM wParam, LPARAM lParam);
	void OnSpeechMsg(WPARAM wParam, LPARAM lParam);
	void OniPodMsg(WPARAM wParam, LPARAM lParam);
	void OnSRMsg(WPARAM wParam, LPARAM lParam);

	void OnMediaPlayerCommand(WPARAM wParam, LPARAM lParam);
	void OnBluetoothCommand(WPARAM wParam, LPARAM lParam);
	void OnImagePlayerCommand(WPARAM wParam, LPARAM lParam);
	void OnDABCommand(WPARAM wParam, LPARAM lParam);
	void OnRadioCommand(WPARAM wParam, LPARAM lParam);
	void OniPodCommand(WPARAM wParam, LPARAM lParam);
    void OnFactoryCmd(WPARAM wParam, LPARAM lParam);

	void OnAudioCodecMsg(WPARAM wParam, LPARAM lParam);
	BOOL OnSwitchAudioSRC(WPARAM wParam, LPARAM lParam);
	BOOL OnVolumeConfigMsg(WPARAM wParam, LPARAM lParam);
	BOOL OnSwitchMediaClassSRC(WPARAM wParam, LPARAM lParam);
	BOOL OnPlaySoundEffect(WPARAM wParam, LPARAM lParam);
	BOOL OnSpeechEnableMsg(WPARAM wParam, LPARAM lParam);

	BOOL OnGetPlayingStatus();
	BOOL OnGetCurMuteStatus();

	UINT OnGetAudioSRC(WPARAM wParam, LPARAM lParam);

	void OnWidgetChanged(WPARAM wParam, LPARAM lParam);

	void OnMuteMsg(WPARAM wParam, LPARAM lParam);
	void OnManualMute(WPARAM wParam, LPARAM lParam);

//	    void OnSteerWheelTimerProc();
	void OnVehicleInfoTimerProc();

	void OnPhoneHFState(WPARAM wParam, LPARAM lParam);
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);
	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);
	void OnBluetoothSetupStatus(WPARAM wParam, LPARAM lParam);
	void OnBluetoothMsg(WPARAM wParam, LPARAM lParam);

	void OnRingToneCmd(WPARAM wParam, LPARAM lParam);

	void OnSmartBarControl(WPARAM wParam, LPARAM lParam);
	void OnSmartBarNotify(WPARAM wParam, LPARAM lParam);

	void OnCanbusStatus(WPARAM wParam, LPARAM lParam);
	void OnRadarStatus(WPARAM wParam, LPARAM lParam);

	void OnMcuUpdateMsg(WPARAM wParam, LPARAM lParam);

	void OnSpeechStart(DWORD dwTimeOut,DWORD dwDuckStatus=-1);
	void OnSpeechTerminate();

	void OnSRStart();
	void OnSRTerminate();

	void OnVolumeFadeReday(BOOL bMute);

	void OnLoadModules();

	void OnAutoOpenSrc();
	void OnLoadCarPlayService();
	void OnSystemReady();
	void OnBacklightChanged(INT iLevel);
    void OnCameraTypeChanged(UINT uCameraType);
    void OnBackgroundChanged(WPARAM wParam, LPARAM lParam);
    void OnPanelLightChange();
    void OnLCDBrightnessChange();
    void OnHeadLightOn(BOOL bOn, UINT nBk);
    void OnRearColorSignalChange(UINT uColorSignal);
	void OnUSBHUBAttached();

// 	HRESULT OnHandleSpeechMessage(UINT idClass,UINT message, WPARAM wParam, LPARAM lParam);

// 	void EnterWidgetMode(BOOL bEnter);

// 	BOOL OnSpeechPTT();

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
	void OnExtensibleCmd(WPARAM wParam, LPARAM lParam);
#endif
	void OnDiskCmd(WPARAM wParam, LPARAM lParam);
	void OnVehicleInfoCmd(WPARAM wParam, LPARAM lParam);
	void OnMcuUpdateCmd(WPARAM wParam, LPARAM lParam);

	void OnNaviMsg(UINT iNaviCode, LPARAM lParam);

	void OnVehicleSpeed(WPARAM wParam, LPARAM lParam);

#if CVTE_EN_KEY_PROXY
	void OnKey(UINT idKey,UINT idStatus);
#endif	
 	void OnPanelKey(WPARAM wParam, LPARAM lParam=NULL);
	void OnVehicleKey(WPARAM wParam, LPARAM lParam=NULL);

#if !CVTE_EN_KEY_PROXY
	void OnPhoneKey(WPARAM wParam, LPARAM lParam=NULL);
	void OnMediaKey(WPARAM wParam, LPARAM lParam=NULL);
#endif	
	void OnRearViewKey(WPARAM wParam, LPARAM lParam=NULL);

	void OnShowMsgBox(WPARAM wParam, LPARAM lParam);
	void OnShowExplorer(WPARAM wParam, LPARAM lParam);
	void OnShowKeyboard(WPARAM wParam, LPARAM lParam);
	void OnShowPasswordBox(WPARAM wParam, LPARAM lParam);
	void OnPasswordBoxCmd(WPARAM wParam, LPARAM lParam);

	void OnSystemPowerOperationReady();
	void OnAutoStartNaviProc();
	void OnUpdateRadioPlayInfo();
	void OnRadioMonitorTA();
    void AutoSwitchCarPlayMedia();
	void AutoSwitchCarPlayScreen();
	void AutoSwitchNativeUI();
	void AutoRequestCarPlayScreen();

	void OnReverseTimerProc();
	void OnRearViewPreDoneTimerProc();
	void OnRearViewDoneTimerProc();
	void OnRearViewMCUCmdDone(BOOL bOn){m_bReverseMCUCmd = bOn;}
// 	void OnReverseSignal(WPARAM wParam, LPARAM lParam);
// 	void OnVolumeKey(BOOL bPositive);
// 	void OnPhoneCallStatusChange(CALLSTATUS cs,BOOL bRing);
// 	void OnReverseModeChange(BOOL bReverse);
// 	void OnMediaStorageInserted(WPARAM wParam, LPARAM lParam);
// 	void OnMediaStorageRemoved(WPARAM wParam, LPARAM lParam);
	void OnWallpaperChanged(LPCTSTR pStrFilePath);
// 	void OnAudioSourceChanged(ARB_WAVE_DEVICE_APPLICATION_LINE_OUT idOut);
// 	void OnAudioSourcePrepareChange(ARB_WAVE_DEVICE_APPLICATION_LINE_OUT idOut);

	void OnWiFiMsg(WPARAM wParam, LPARAM lParam);

	void OnMuteKey(BOOL bMute);

	void OnLampStatusChanged(BOOL bOn);
	void OnBrakeStatusChanged(BOOL bOn);

	void OnDeviceChanged();

	void OnDeviceArrived(LPCTSTR lpDeviceName);
	void OnDeviceRemoved(LPCTSTR lpDeviceName);

	void OnDeviceArrivedReady(WPARAM wParam, LPARAM lParam);
	void OnDeviceRemovedReady(WPARAM wParam, LPARAM lParam);

	void OperateWidget(UINT idUI,UINT idControl,UINT idOperation,LPVOID lpara);

	BOOL IsActivated(){return m_bActivated;}

	//
	void OnNotifyVolumeInfo(WPARAM wParam, LPARAM lParam);
	void OnNotifySourceInfo(WPARAM wParam, LPARAM lParam);
	void OnNotifyMediaPlaybackInfo(WPARAM wParam, LPARAM lParam);
	void OnNotifyRadioInfo(WPARAM wParam, LPARAM lParam);
	void OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam);
#if CVTE_EN_ANDROID_AUTO
	void OnAndroidAutoMsg(WPARAM wParam, LPARAM lParam);
#endif
    #if CVTE_EN_CARPLAY
	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);
	#endif
	#if CVTE_EN_CPMAC
	void OnCarPlayMACChanged(WPARAM wParam, LPARAM lParam);
	#endif
	void OnDabServiceLinkMsg(WPARAM wParam, LPARAM lParam);
	void OnDabServiceStartReady(WPARAM wParam, LPARAM lParam);

	void OnHandleSRMsg(UINT msg,WPARAM wParam, LPARAM lParam);

	DWORD OnVolumeFadeProc();
	void DoPowerIdle();
	void DoPowerOff();
	void DoPowerOn();
	void DoPowerSoftReset();
	void DoPowerHardReset();

	//
	void OnRearViewPrepare(BOOL bOn);
	void OnRearViewDone(BOOL bOn, DWORD dwTimeout = 0);

	#if CVTE_EN_CARPLAY
	void OnCarPlayArrived();
	void OnCarPlayRemove();
	#endif

	BOOL	IsMediaSRCSupportDucking(UINT idSRC);
#if CVTE_EN_ACCON_WARNING
	void OnEnterWarningPanel(WPARAM wParam, LPARAM lParam);
	void OnQuitWarningPanel(WPARAM wParam, LPARAM lParam);
#endif

    #if CVTE_EN_DYNAMIC_LOGO
    void OnPlayDynamicLogo();
    BOOL DynamicLogoFinish();
	void RenderDynamicLogoBk();
    #endif

	void QuickEnterEQSetting();

	//BOOL DoSwitchAudioSRC();
	void OnAudioMuteTimeout();
	void OnTimerReleaseSpeech();

protected:
	//GPS:
	void EnableGPSMonitor(BOOL bEnable);
	static DWORD GPSMonitorThreadPro(LPVOID lpParam);
	BOOL GPSPowerOn();
	BOOL GPSPowerOff();
	//screen power:
	void ScreenPowerOn();
	void ScreenPowerOff();

protected:
	//volume:
//		void SetMainVolume(INT iVolume,BOOL bFade,BOOL bShowVolBar=FALSE,BOOL bLowerVolNoFade=TRUE,BOOL bUpdateLED=TRUE);
//	    void SetMixVolume(INT iVolume,BOOL bFade,BOOL bShowVolBar=FALSE,BOOL bLowerVolNoFade=TRUE,BOOL bUpdateLED=TRUE);
        void SetVolume(INT iVolume,BOOL bFade,BOOL bShowVolBar=FALSE,BOOL bLowerVolNoFade=TRUE,BOOL bUpdateLED=TRUE, BOOL bMainVolChannel = TRUE);

	//

protected:
// 	void ProcessMCUCmdLine(LPBYTE pCmd,DWORD dwCmdLen);

	BOOL InitializeEGLWorld(void);
	void ActivateRenderLogo();
	void RenderLogo(void);
	void RenderIniBackground(void);
	void RenderPoweroffScreen(void);
	void RenderIdleScreen();
	void RenderUpdateScreen(UINT idFlag);
	static void CALLBACK DeviceActivateProc(PVOID context, BOOL IsActivated);
	static void CALLBACK DeviceSetMACProc(PVOID context, BOOL IsSetMAC);
	void RenderActivateScreen(const wchar_t *name);
	void RenderSetMACScreen();

private:
	void RenderInstallProgress(CLabelGL *pIcon,CSliderGL *pSlider,INT iProgress,const wchar_t *pStrTitle=NULL);
	//void CopyFiles(LPCTSTR lpszDesPath,LPCTSTR lpszSrcPath,BOOL bShowProgress=FALSE,CLabelGL *pIcon=NULL,CSliderGL *pSlider=NULL,CLabelGL *pLable=NULL);
	BOOL EnterInstallProgress(CLabelGL *pIcon,CSliderGL *pSlider);
	BOOL IsPathExists(LPCTSTR   lpszPath);
	BOOL IsFileExists(LPCTSTR   lpszFileName)  ;
	BOOL IsInstallFile(LPCTSTR lpFileName);
	BOOL InstallFiles();
    BOOL IsNaviSpeechEnable();

	void PrepareForFirstTime();
	BOOL LaunchInstaller(LPCTSTR   lpszFileName,LPCTSTR   lpszInstallPath,CLabelGL *pIcon,CSliderGL *pSlider);
	BOOL OpenPanels(UINT idPanel);

// 	void StopAutoReversePoweroff();
	void ReturnToPrevPanel(PAGE_ANI_ID idSwitchAniId=PAGE_SWITCH_ANI_DEFAULT );

	//
	void SwitchSRC();

	//power:
	void PrePowerOff();
	void PowerOn();
	void PowerOff();
	void PowerIdle();
	void PowerSoftReset();
	void PowerHardReset();
	BOOL IsPowerOn();
	BOOL IsPowerIdle();
//	    BOOL IsScreenOn();
//	    BOOL IsScreenOff();
//	    BOOL IsScreenIdle();


    //Screen
    void ScreenOff();
    void ScreenOn();
    void ScreenIdle();

	void AutoOpenSRC();
	//
	void PreparePowerIdle();
	void PreparePowerOn();

	//
	void NotifyUILaunched();

	//
	static DWORD MuteReleaseThreadPro(LPVOID lpParam);
	void MuteReleaseProc();
	void Mute(BOOL bMute, BOOL bFade=TRUE, BOOL bNoSendCmd=FALSE);
    void DoMute(BOOL bMute);

	//
	void OnReverseModeChange(BOOL bReverse);
	//
	static DWORD PlaySoundEffectThreadPro(LPVOID lpParam);
	void PlaySoundEffectProc();

	void AdjustVolume(INT iVolOffset);

    void UpdateVolume(UINT uVolume);
	void ProcessPhoneCallState(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK CommandProc(LPBYTE pCmd,DWORD dwCmdLen,LPVOID lparam);

private:
	UINT m_idMuteType;
	UINT m_iMuteTimeOut;
	HANDLE	m_hMuteThread;
	HANDLE	m_hEventMute;
	BOOL	m_bNeedUpdateVolLED;
	BOOL	m_bMuteOperating;
    BOOL	m_bMuteFading;
    BOOL    m_bMute;

	BOOL	m_bMCUPackageUpdated;
	BOOL	m_bMCUUpdating;

	BOOL	m_bPlayingSoundEffect;
	INT		m_iPlaySoundEffectCnt;
	HANDLE	m_hPlaySoundEffect;

    //Carplay
    BOOL        m_bDeviceActivated;
	BOOL		m_bIsSetMAC;
	BOOL	    m_bCarPlayConnected;
	BOOL	    m_bCarPlayAltAudioOn;
	BOOL	    m_bCarPlayMainAudioOn;
	BOOL	    m_bCarPlayTurnbyTurnOn;
	BOOL	    m_bCarPlaySiriRecognizing;
	BOOL	    m_bCarPlayAudioOn;
	BOOL        m_bOnNightMode;
	DWORD       m_dwCarPlayAudioOnTick;
	DWORD       m_dwCarPlayAudioOffTick;
	DWORD       m_dwCarPlayArrivedTick;

	//android auto
	BOOL			m_bAndroidAutoConnect;
	BOOL			m_bAndroidAutoVoiceActicve;

	DWORD	    m_dwFrameRenderTime;
	BOOL		m_bAutoReleaseNoFade;
    BOOL        m_bNoSendMuteCmd;

	//iPod
	BOOL       m_biPodConnected;

public:
	CCommanderEx					*m_pCommander;
	BOOL									m_bCommanderInitialized;

private:
// 	BOOL					m_bWidgetMode;
// 	BOOL						m_bSpeechIni;
	BOOL						m_bSystemReady;
	DWORD					m_dwSystemReadyTick;
	BOOL						m_bOpenSRC;
	BOOL						m_bBacklightOFF;
	CM_List<CPanelGL>	m_listPanels;
	CM_List<CSmartBar>	m_listSmartBars;

	CPanelGL					*m_pCurPanel;
	DWORD					m_dwOpenSRCTick;
	BOOL						m_bModulesLoaded;
	CPanelGL					*m_pIdlePanel;
	CPanelGL					*m_pSetMACPanel;

	DWORD					m_dwSetVolumeTick;


	BOOL						m_bOnMute;
    BOOL                    m_bOnHWMute;
	UINT							m_idEQ;
	UINT							m_idCurAudioSRC;
	UINT							m_idCurMediaClassSRC;
	UINT							m_idAudioSRCBeforeSpeech;
	UINT							m_idCurMCUSRC;

	UINT							m_idValidSrcList[MEDIA_TYPE_NUMBER];
	UINT							m_idCurSrc;

	BOOL						m_bWaitingSpeechTerminate;

	BOOL						m_bAutoStartNavi;

	BOOL						m_bClickValid;
	BOOL			m_bOnTAMode;

	//AUX IN DETECT
	BOOL				m_bAuxInDetect;

	HANDLE		m_hEventSD;
	CAppConfigProxy *m_pConfig;
	CSkinManagerGL	*m_pSkinManager;
	CTextGL						*m_pTextGL;
// 	CKeyBoardGL				*m_pKeyBoard;
	CStatusBarEx				*m_pStatusBar;
// 	CExplorerGL					*m_pExplorer;
	CVideoWindow				*m_pVideoWindow;
	CSmartBarVol		*m_pSmartBarVol;
	CSmartBarPhone		*m_pSmartBarPhone;
	CSmartBarMedia		*m_pSmartBarMedia;
	CSmartBarDrop		*m_pSmartBarDrop;
    CRearOverlay        *m_pRearOverlay;
	CSmartBarTopBanner		*m_pSmartBarTopBanner;
#if CVTE_EN_ACCON_WARNING
	CSmartBarWarn		*m_pSmartBarWarn;
#endif

    #if CVTE_EN_LED
	CLED		        *m_pLED;
    #endif

	CLabelGL				m_bk;
// 	CLabelGL				m_pop[8];
	DWORD				m_dwLastSpeechTick;

	//BOOL					m_bReversePowerOn;
	UINT						m_idPowerState;
    UINT                m_idScreenState;
	DWORD				m_dwPowerStateTick;
    DWORD               m_dwScreenStateTick;
	BOOL					m_bManualPowerOn;
	BOOL					m_bManualMute;
    DWORD               m_dwActivateSiriTick;
	BOOL					m_bOnSR;
	UINT					m_idMediaClassBeforSR;
	BOOL					m_bMediaPlayingBeforeSR;

	BOOL					m_bOnReverseMode;
	BOOL					m_bWaitingForPowerOFF;
	BOOL					m_bOnBrakeMode;
	BOOL					m_bReverseSignaled;
	BOOL                    m_bReverseMCUCmd;
// 	ARB_WAVE_DEVICE_APPLICATION_LINE_OUT m_idLineOut;
// 	CALLSTATUS			m_CallStatus;
// 	BOOL					m_bPhoneRing;
	CM_String			m_strDeviceInserted;
	DEV_TYPE			m_idDevTypeInserted;

	DWORD		m_dwLastSeekTick;
 	DWORD		m_dwLastClickTick;
 	DWORD       m_dwLastMuteTick;

	BOOL		m_bSpeechOn;
    BOOL        m_bSpeechVolumeChange;
    BOOL        m_bMainVolumeChange;
	BOOL			m_bAltAudioDataActived;
	BOOL			m_bAltAudioDmaActived;

	DWORD		m_dwSpeechTimeOut;
	DWORD		m_dwSpeechStartTick;

	BOOL			m_bActivated;
	INT				m_iVolume;
	INT				m_iSetVolume;
	INT				m_iVolOriginGPS;
	INT				m_iVolOriginBluetooth;
	INT				m_iVolOriginMedia;

	BOOL			m_bVolumeFadeReady;
	INT				m_iVolumeCur;
	INT				m_iVolumeFadeTo;
	BOOL			m_bShowVolBar;
	BOOL			m_bAutoUnMute;
	BOOL			m_bAutoMute;

	DWORD		m_dwBklBeforeReverse;

	INT				m_iVehicleSpeed;
	INT				m_iSpeedCurVolome;
	INT				m_iVolumeDelt;

	//
#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
    INT         m_iTreble;
#endif

	//phone status:
	DWORD		m_dwCallStatus;
	DWORD		m_dwBTCallStatus;
	DWORD		m_dwCarPlayCallStatus;
	BOOL			m_bPhoneCallActive;
// 	BOOL			m_bPhoneTalking;
	BOOL			m_bBTPhoneCallActive;
	BOOL			m_bCarPlayPhoneCallActive;

	BOOL			m_bOnGestureProcess;

	//GPS:
	BOOL			m_bEnableGPSMonitor;
	HANDLE		m_hEvtGPSMonitor;
	HANDLE		m_hThreadGPSMonitor;

	BOOL			m_bGPSPowerOn;

	//
	UINT				m_idMcuUpdateStatus;

	//
	UINT				m_idNaviStatus;

	//
	BOOL			m_bPowerOperating;

	INT				m_iDeviceDetectCnt;

    //Snapshot
    DWORD           m_dwSnapshotDoneTick;

    BOOL            m_bUIWindowShow;
    UINT            m_iScrnBrightness;

	//
	BOOL			m_bSpeechSignaled;
	UINT            m_iSpeechMsgType;

#if CVTE_EN_IDLE_LOGO
    CLabelGL        m_tagLogo;
    CLabelGL        m_tagLogo2;
    GLfloat         m_logoAlpha;
    GLfloat         m_logo2Alpha;
    BOOL            m_bLogoShade;
    DWORD           m_dwLogoShadeTick;
	UINT				m_idLogoShowStyle;
#endif

#if ((CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418) || (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738))
	//audio codec:
	CAudioCodec	*m_pAudioCodec;
#endif

    UINT			m_idUIBeforeIdle;

    BOOL            m_bResumeSRCAfterRearView;

	CPanelMediaWrapper*	m_pMediaWrapper;
	CPaneliPodWrapper* m_piPodWrapper;
	CPanelBluetoothWrapper* m_pBluetoothWrapper;
	CSndPlayer *m_pSndPlayer;

	std::vector<MsgData> m_listPhoneCallCmd;

	//UINT			m_idSwitchAudioSRC;
};
