#pragma once
#include <CSRMedia.h>
#include "../GUI/Common/message_map.h"

class CMediaPlayer :
	public CPanelGL
{
public:
	CMediaPlayer(void);
	~CMediaPlayer(void);

	BOOL Initialize(HWND hWnd);

	void Play();
	void Pause();
	void Stop();
	void SwitchFullscreen(UINT idMode);
	void ShowVideoWindow(BOOL bShow);
	void SwitchRepeat(UINT idRepeat);
	void SwitchShuffle(UINT idShuffle);
	void PlayIndex(INT index,DWORD dwPos=0);
	void PlayNext();
	void PlayPrevious();
	void AutoPlay();
	void SwitchPlayMode();

	void SetVolume(INT iVolume,BOOL bFade);

	void PlayFile(LPCTSTR lpFileName,DWORD dwPos=0);

private:	
	BOOL LoadMediaModule();
	HRESULT PlayFilePro(LPCTSTR lpFileName,DWORD dwPos=0);
	void	CleanUp();
	static DWORD OpenFileThreadPro(LPVOID lpParam);
	BOOL IsMediaFileExists(LPCTSTR   lpszFileName);


private:

	BOOL				m_bMediaModuleLoaded;
	BOOL				m_bScanning;
	BOOL				m_bScanningUpdate;

	BOOL				m_bLoadFileReady;
	BOOL				m_bPlayFileReady;


	MEDIA_CATEGORY	m_category;

	CSmartBarWindow	*m_pSmartBarWindow;

//	BOOL		m_bBeginToPlay;
//	BOOL		m_bContinueToPlay;
	BOOL		m_bAutoPlay;
	BOOL		m_bAutoResume;

	HWND		m_hVideoWnd;

	CM_String		m_strCurrentPlayFile;
	CM_String		m_strCurrentDevice;

	HANDLE		m_hOpenFileEvent;
	HANDLE		m_hOpenFileThread;
	CM_String		m_strFileToPlay;
	DWORD		m_dwPosToPlay;

// 	HANDLE m_hArbitratorThread;
	DWORD		m_dwLastSeekTick;

	DWORD	m_dwLastFullScreenTick;
	BOOL		m_bPlaying;
// 	BOOL		m_bAudioBusConnected;
	UINT			m_iVideoStartPos;
	BOOL		m_bPlayingBeforeReverse;
	BOOL		m_bPlayingBeforeBreak;

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
	BOOL		m_bBeginPlayIndex;
	BOOL		m_bBeginPlayFile;
	CM_String		m_strBeginPlayFile;
	DWORD		m_dwBeginPlayIndex;
	DWORD		m_dwBeginPlayPos;
	BOOL			m_bPlayingBeforeSpeech;
	BOOL			m_bSpeechOn;
	BOOL			m_bPhoneCallActive;

	UINT				m_idPlayMode;

	BOOL			m_bA2DPAvailable;

	DWORD		m_dwEnterTick;
	DWORD		m_dwDeviceRemoveTick;
	BOOL			m_bAutoSelectDevice;
	BOOL			m_bOpeningFile;
	BOOL			m_bAbortOpeningFile;
	BOOL			m_bPlayCompleteDetection;

};
