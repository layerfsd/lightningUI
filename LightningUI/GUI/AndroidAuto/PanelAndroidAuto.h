#pragma once
#include "../Bluetooth FLC/PanelBluetoothWrapper.h"
#include <windev.h>
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "AndroidAutoAPI.h"
#include <CMthread.h>
#include <CSync.h>

#if CVTE_EN_ANDROID_AUTO

#define DEBUG_ANDROIDAUTO		1

typedef void
	( *DeviceActivateCallback_f )(
	PVOID context,
	BOOL IsActivated );

using namespace AndroidAutoAPI;

class CPanelAndroidAuto :
	public CPanelGL, public IAndroidAutoAPICallback, public CMiniThread
{
public:
	CPanelAndroidAuto(void);
	~CPanelAndroidAuto(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();
	BOOL IsEnableQuit();
	void OnSystemPowerStatePrepare(UINT idState);

	void OnGestureMsg(UINT idGesture);
	void OnNativeNaviMsg(UINT iNaviCode, LPARAM lParam);
	void OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry,DeviceActivateCallback_f callback, PVOID context);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnSRCChanged(UINT idSRC);

	BOOL OnKey(UINT idKey,UINT idStatus);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

	void OnSpeechStart();
	void OnSpeechTerminate();

	void LinkBluetoothPanel(CPanelBluetoothWrapper *pBTPanel);
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

	void OnBrakeStatusChanged(BOOL bOn);
	void OnNightModeChanged(BOOL bNightMode);

	static BOOL IsFileExists(LPCTSTR lpszFileName);

	wchar_t* GetDeviceName();

	BOOL IsPlaying();

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

	virtual DWORD ThreadRun();

private:
	virtual void connectCallback(bool connect);
	virtual void audioFocusRequestCallback(AudioFocusRequestType request);
	virtual void videoFocusCallback(VideoFocusMode focus, int reason);
	virtual void onPairingRequest(const string& phoneAddr, BluetoothPairingMethod pairingMethod);
	virtual void microphoneRequestCallback(bool open, bool ancEnabled, bool ecEnabled, int maxUnacked);
	virtual void naviFocusCallback(NavFocusType type);
	virtual void voiceSessionNotificationCallback(VoiceSessionStatus status);
	virtual void audioPlaybackStartCallback(AudioStreamType type);
	virtual void audioPlaybackStopCallback(AudioStreamType type);
	virtual void vedioPlaybackStartCallback();
	virtual void vedioPlaybackStopCallback();

	static void onBluetoothPairRequestCallback(LPCTSTR devAddr,LPCTSTR passkey,LPVOID para);
	static void CALLBACK QuitVoiceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK EnterVoiceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK QuitAASourceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK EnterAASourceModeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK KillAAPVideoFocusTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK FirstConnectionTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK AutoShowAAPTelTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

	string getId();
	bool GenerateId();

	BOOL EnterVoiceMode();
	BOOL QuitVoiceMode();
	void PrepareEnterVoiceMode();
	void PrepareQuitVoiceMode();

	BOOL EnterAASourceMode();
	BOOL QuitAASourceMode();
	void PrepareEnterAASourceMode();
	void PrepareQuitAASourceMode();

	BOOL ActiveOnce(LPCTSTR strDestCA);
	BOOL StartClient();

private:
	DWORD		m_dwEnterTick;
	BOOL			m_bSpeechOn;
	CLabelGL		m_iconLink;

	AndroidAutoAPI::CAndroidAutoAPI *m_pAndroidAuto;
	CPanelBluetoothWrapper *m_pBTPanel;
	BOOL m_bAndroidAutoConnected;
	UINT m_idMediaClassBeforeAA;
	BOOL m_bSrcPlayingBeforeAA;
	BOOL m_bAABorrowAudio;
	AudioFocusRequestType m_audioRequest;
	AudioFocusRequestType m_audioRequestBeforeIdle;
	AudioFocusRequestType m_audioRequestBeforePhonecall;
	bool	m_bVideoRequested;
	MMRESULT m_timerEnterVoiceMode;
	MMRESULT m_timerQuitVoiceMode;
	MMRESULT m_timerEnterAASourceMode;
	MMRESULT m_timerQuitAASourceMode;
	MMRESULT m_timerKillAAPVideoFocus;
	MMRESULT m_timerFirstConnection;
	MMRESULT m_timerAutoShowAAPTel;
	BOOL			m_bRdsTAon;
	BOOL			m_bAndroidAutoVoiceOn;
	wchar_t  m_strPhoneBluetoothAddr[20];
	BOOL	m_bAudioStreamGuidance;
	BOOL	m_bAudioStreamMedia;
	BOOL  m_bVoiceOn;
	BOOL  m_bPhoneCallActive;
	BOOL	m_bScreenVideoOn;
	BOOL   m_bNeedShowPrompt;

	BOOL m_bOnNightMode;

	wchar_t m_strDeviceName[MAX_PATH];

	DeviceActivateCallback_f m_DeviceActivateCallback;
	PVOID m_DeviceActivateCallbackContext;

	static const wchar_t * m_DestCAPath1;
	static const wchar_t * m_DestCAPath2;

};
#endif