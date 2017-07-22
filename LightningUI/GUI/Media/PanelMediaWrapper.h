#pragma once
#include "../Bluetooth FLC/BluetoothCommon.h"

#include "../Controls/SmartBarVol.h"
#include <CSRMedia.h>
#include "PanelMediaMain.h"
#include "PanelMediaLibrary.h"
#include "PanelMediaDevice.h"
#include "../../Communication/CommanderEx.h"


class CPanelMediaWrapper :
	public CPanelGL
{
public:
	CPanelMediaWrapper(void);
	~CPanelMediaWrapper(void);

	void OnSystemPowerStatePrepare(UINT idState);

	void OnMediaPlayerCommand(WPARAM, LPARAM);
	void OnRingToneCommand(WPARAM, LPARAM);
    void OnFactoryCmd(WPARAM wParam, LPARAM lParam);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnSRStart();
	void OnSRTerminate();
	void OnPlaySoundEffect(BOOL bPlay);

	void OnSRCChanged(UINT idSRC);
	void OnReverseModeChange(BOOL bReverse);
	void OnUIChanged(UINT idUI);

	void OnVolumeFadeReday(BOOL bMute);
	void OnDelayOperation();

	void OnBrakeStatusChanged(BOOL bOn);

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);	
	#endif
	
	void OnMute(BOOL bMute);

	void SetMediaAudioVolume(INT iVolume,BOOL bFade);

	void OnDeviceChanged();

	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);

	void LinkSmartBarVol(CSmartBarVol *pSmartBar){m_pSmartBarVol = pSmartBar;}
	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar);

	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

	void LinkPages(CPanelMediaMain *pMain,CPanelMediaLibrary *pLibrary,CPanelMediaDevice *pDevice);
	BOOL IsMassStorageMediaType(UINT idType);

	//
	void StartRingTone(DWORD dwWavID,DWORD dwVol=0xFFFF,BOOL bLoop=TRUE);
	void EndRingTone();

	//
	void EnableVideoPlayer(BOOL bEnable);

	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);
	void OnAutoResume();

	BOOL IsPlaying();
	ICSRMedia *GetMediaCore(){return m_pCSRMedia;}

private:
	static LRESULT CALLBACK	VideoWndProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT PlayFilePro(LPCTSTR lpFileName,DWORD dwPos=0);
	void	CleanUp();
	void Play();
	void Pause();
	void Resume();
	void PreparePause(BOOL bDuck=TRUE);
	void PreparePlay();
	void Stop();
	void PrepareStop();
	void SwitchFullscreen(UINT idMode);
	void ShowVideoWindow(BOOL bShow);
	void SwitchRepeat(UINT idRepeat);
	void SwitchShuffle(UINT idShuffle);
	BOOL PlayIndex(INT index,DWORD dwPos=0);
	void PlayNext(BOOL bAni=TRUE);
	void PlayPrevious(BOOL bAni=TRUE);
	BOOL AutoPlayDevice();
	void AutoPlay();
	void SwitchPlayMode();
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	void SwitchPlayMode(UINT mode);
#endif
	void SetPlayMode(UINT idMode);
	void SetPlayPosition(DWORD dwNum,DWORD dwDen);
	void OnPlayerComplete();
	void OnPlayerLoadFileReady();
	void OnPlayerLoadFileFailed();
	void OnPlayerLoadFileAbort(const char* iReason, int line);

	void AbortLoadFile();
	BOOL WaitForLoadFileReady(INT iTimeOut=15000);
	void EnableMonitorPlayinfo(BOOL bEnable);

	HRESULT OnPlayerMsg(WPARAM wParam, LPARAM lParam);
	HRESULT OnPlayerMsgEx(WPARAM wParam, LPARAM lParam);
	HRESULT OnRingToneMsg(WPARAM wParam, LPARAM lParam);

	void OnVideoWndGesture(WPARAM wParam, LPARAM lParam);

	void OnMediaBrowsingFinish(BOOL bCancel);
	void OnMediaBrowsingBegin();

	void OnMediaVolumeFade(WPARAM wParam, LPARAM lParam);
	void OnUpdatePlaybackInfo(WPARAM wParam, LPARAM lParam);

	void SelectMediaType(UINT idType,BOOL bAutoPlay=TRUE);
	void SetFF(DWORD dwNum, BOOL bDown);
	void SetFB(DWORD dwNum, BOOL bDown);
	void SelectCategory(MEDIA_CATEGORY idCategory,BOOL bAutoPlay=TRUE);
	void AbortSelectMediaType();
	void AbortSelectCategory();

// 	const wchar_t*GetDeviceFriendlyName(LPCTSTR lpDeviceName);
	void PrepareSelectMediaType(UINT idType);
	void PrepareSelectCategory(MEDIA_CATEGORY idCategory);

	void SaveCurrentMediaInfo(DWORD dwPos=0);
	void UpdatePlayPostion();

	void StartPlayback();
	void PlayFailed(LPCTSTR lpFileName=NULL,UINT idErrCode = LOAD_STATUS_FAILED_NO_FILE);

// 	void Mute(BOOL bMute,UINT idType=_TFT_MUTE_ALL,BOOL bAutoRelease=TRUE);
	BOOL IsMediaFileExists(LPCTSTR   lpszFileName);

	BOOL PlayFile(LPCTSTR lpFileName,DWORD dwPos=0);
	BOOL PreparePlayFile(LPCTSTR lpFileName,DWORD dwPos=0);

	static DWORD OpenFileThreadPro(LPVOID lpParam);

	void MuteAudio(BOOL bMute,BOOL bFade,UINT type,BOOL bAutoRelease,UINT time_out,BOOL bSync=FALSE,BOOL bNoSendMuteCmd=TRUE);

	//
	void EnableSrcWidget(BOOL bEnable);

	//
	void EnablePlayUI(BOOL bEnable,const char* iReason);

	void LoadMediaModule();
	static DWORD LoadMediaModuleThreadPro(LPVOID lpParam);
	BOOL LoadMediaModuleProc();
	BOOL WaitForMediaModuleReady();

	static DWORD RingtoneThreadPro(LPVOID lpParam);
	void TerminateRingtonePlayer();

	void SetVideoColorKey(DWORD dwColorKey);

	void TerminateVideoPlayback();
	void TakeGstResource(BOOL bTake);
#if CVTE_EN_DYNAMIC_LOGO
public:
    void PlayDynamicLogo(DWORD dwVol);
    void StopDynamicLogo();
    BOOL DynamicLogoFinish();

private:
    static DWORD DynamicLogoThreadPro(LPVOID lpParam);
    void ClearUpDynamicLogo();
    HRESULT DynamicLogoRun();
    HRESULT OnDynamicLogoMsg(WPARAM wParam, LPARAM lParam);
#endif

private:
	HANDLE	m_hLoadMediaModuleEvent;

private:
// 	static DWORD AutoVolumeThreadPro(LPVOID lpParam);
	BOOL AutoVolumeStepProc();
	BOOL AutoSelectDevice();
	void Lock();
	void UnLock();

private:
// 	PAGE_MEDIACORE				m_page;
	CPanelMediaMain		*m_ppanelMain;
	CPanelMediaLibrary		*m_ppanelLibrary;
	CPanelMediaDevice		*m_ppanelDevice;

	CCriticalSection* m_DataCS;

	UINT					m_idCurMediaType;
	UINT					m_idSelectMediaType;

	UINT					m_iAutoDetectDeviceRetryCnt;

	BOOL				m_bAutoPlayAfterBrowsingFinish;

	BOOL				m_bMediaModuleLoaded;
	BOOL				m_bBrowsing;

	BOOL				m_bEnableVideoPlayer;

	BOOL				m_bLoadFileReady;
	BOOL				m_bPlayFileReady;

	INT				m_iAutoSelectCategory;

	MEDIA_CATEGORY	m_category;
	MEDIA_CATEGORY	m_categoryToSet;

	CSmartBarVol	*m_pSmartBarVol;
	CSmartBarMedia	*m_pSmartBarMedia;

//	BOOL		m_bBeginToPlay;
//	BOOL		m_bContinueToPlay;
	BOOL		m_bAutoPlay;
	BOOL		m_bAutoResume;

	HWND		m_hVideoWnd;

	CM_String		m_strCurrentPlayFile;
// 	CM_String		m_strCurrentDevice;

	DWORD		m_dwLoadFileErrorCode;
	BOOL			m_bNeedProcessCompleteNotify;
	BOOL			m_bAbortLoadFile;

	HANDLE		m_hOpenFileEvent;
	HANDLE		m_hOpenFileThread;
	CM_String		m_strFileToPlay;
	DWORD		m_dwPosToPlay;
	HANDLE		m_hOpenFileReadyEvent;
	DWORD		m_dwOpenFileDelay;

	BOOL			m_bMonitorPlayinfo;
// 	HANDLE m_hArbitratorThread;
	DWORD		m_dwLastSeekTick;

	DWORD	m_dwLastFullScreenTick;
	BOOL		m_bPlaying;
// 	BOOL		m_bAudioBusConnected;
	UINT			m_iVideoStartPos;
	BOOL		m_bPlayingBeforeReverse;
	BOOL		m_bPlayingBeforeBreak;

	BOOL		m_bManualPause;
	BOOL		m_bManualSetPosition;
	DWORD		m_dwManualSetPosition;
	BOOL		m_bManualPlayFile;
	BOOL		m_bManualChangeSrcType;
	BOOL		m_bManualChangeCategory;
	BOOL		m_bManualStop;
    BOOL        m_bManualMute;
	BOOL		m_bPreparePlay;

	BOOL m_bFullScreenMode;
	ICSRMedia* m_pCSRMedia ;
	ICSRPlayer* m_pPlayer ;
	ICSREventSource* m_pEvent ;
	ICSRVideo* m_pVideo ;
	ICSRWaveoutAudio* m_pAudio;
	ICSRStreamInfo* 	m_pStreamInfo;
	ICSRInspector* m_pInspector ;
	ICSRMediaInfo* m_pMediaInfo ;

	CM_STATE	m_nLastState;
	UINT				m_iAudioStreamCount;
	UINT				m_iVideoStreamCount;

	//for phone ringtone:
	ICSRPlayer* m_pRingTonePlayer ;
	ICSREventSource* m_pRingToneEvent ;
	ICSRWaveoutAudio* m_pRingToneAudio;
	BOOL m_bRingTonePlaying;
	BOOL m_bRingToneLoop;
	DWORD m_dwRingToneVol;
	DWORD m_dwRingToneWavID;
	CM_String m_strRingtoneFile;

	BOOL m_bVideoWndVisible;
	BOOL m_bCurPlayingAudioOnly;
	BOOL m_bHasMediaStream;
	INT m_iVolumeCur;
	INT m_iVolumeSet;
	BOOL m_bVolumeFadeReady;
	BOOL m_bRepeat;
	BOOL  m_bShuffle;
	DWORD	m_dwPlayPos;
	BOOL m_bNeedSetIniPos;
	DWORD m_dwIniPos;
	DWORD	m_dwPlayTick;
// 	BOOL		m_bBeginPlayIndex;
// 	BOOL		m_bBeginPlayFile;
// 	CM_String		m_strBeginPlayFile;
// 	DWORD		m_dwBeginPlayIndex;
// 	DWORD		m_dwBeginPlayPos;
 	BOOL			m_bPlayingBeforeSpeech;
	BOOL			m_bSpeechOn;
    BOOL            m_bSROn;
	BOOL			m_bPhoneCallActive;

	UINT				m_idPlayMode;

	BOOL			m_bA2DPAvailable;

	DWORD		m_dwEnterTick;
	DWORD		m_dwDeviceRemoveTick;
	BOOL			m_bAutoSelectDevice;
	BOOL			m_bOpeningFile;
	BOOL			m_bAbortOpeningFile;
	BOOL			m_bPlayCompleteDetection;

	//
	BOOL			m_bCarPlayConnected;

    UINT           m_iFactoryTestType;
    UINT           m_iFactoryTestCmd;
	
	BOOL			m_bEnableAutoPlay;
	BOOL			m_bResumeVideoPlayAfterCarplay;

	HANDLE		m_hGstReadyEvt;
	BOOL			m_bMediaPlayerTakeGst;

	//BOOL        m_bUsb1Inserted;

	#if CVTE_EN_DYNAMIC_LOGO
    DWORD       m_dwDynamicLogoVol;
    BOOL        m_bDynamicLogoPlaying;
	ICSRPlayer* m_pDynamicLogoPlayer;
	ICSREventSource* m_pDynamicLogoSourceEvent;
	ICSRVideo* m_pDynamicLogoVideo;
	ICSRWaveoutAudio* m_pDynamicLogoAudio;
	#endif
};
