#pragma once
#include "PaneliPodMain.h"
#include "PaneliPodLibrary.h"
#include "../GUI/Common/message_map.h"

class CPaneliPodWrapper :
	public CPanelGL
{
public:
	CPaneliPodWrapper(void);
	~CPaneliPodWrapper(void);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);

	void OnSystemPowerStatePrepare(UINT idState);
	void OnSystemPowerStateDone(UINT idState);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnSRStart();
	void OnSRTerminate();
	void OnSRCChanged(UINT idSRC);
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

	void OniPodCommand(WPARAM wParam, LPARAM lParam);
	
	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif

	void OniPodMsg(WPARAM wParam, LPARAM lParam);
	void OnSelectItem(int index);
	void OnBrowserUp();

	void LinkPages(CPaneliPodMain *pMain,CPaneliPodLibrary *pLibrary);

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);

public:
	void Play();
	void Pause();
	void Stop();
	void PlayNext();
	void PlayPrevious();
	void PlayFF();
	void PlayRW();
	void EndFFRPlay();
	void	Repeat();
	void Shuffle();
	void SetPlayMode(UINT idMode);
	BOOL IsPlaying();
protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void IniAPPlayer();
	void DeiniAPPlayer();
	void plugInDetection();
	void plugOutDetection();

private:
	CPaneliPodMain		*m_ppMain;
	CPaneliPodLibrary		*m_ppLibrary;


	BOOL m_bSpeechOn;
	BOOL m_bPhoneCallActive;
	bool				m_bDeviceDonnected;
	PLAY_STATE m_playState;
	PLAY_STATE m_playStateBeforeSpeech;
	PLAY_STATE m_playStateBeforePhonecall;

	//
	IpodControl * mIpodControl;
	AuthenticationListener * mAuthenticationListener;
	BrowserListener * mBrowserListener;
	PlayerListener * mPlayerListener;
	iOSCommunicationListener * miOSCommunicationListener;
	RemoteControlListener * mRemoteControlListener;

	repeat_mode m_repeatMode;
	shuffle_mode m_shuffleMode;

	DWORD m_dwCurTrackLength;
	DWORD m_dwCurTrackTime;

	DWORD m_dwCurTrackIndex;
	DWORD m_dwTotalTracks;

	int		m_browser_level;
	BOOL	m_bHasAlbumArt;
	DWORD m_dwRefreshPlayinfoRef;
	DWORD m_dwRefreshPlayinfoTick;
	BOOL	m_bRefreshPlayinfoAfterConnect;

	char m_strTrackName[MAX_STRING];
	char m_strArtist[MAX_STRING];
	char m_strAlbum[MAX_STRING];
	BOOL m_bTrackNameValid;
	BOOL m_bArtistValid;
	BOOL m_bAlbumValid;

	BOOL m_bPlayingBeforeSpeech;
};

