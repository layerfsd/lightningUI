#pragma once
#include "../Bluetooth FLC/BluetoothCommon.h"
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../Controls/SmartBarMedia.h"
#include "../Controls/SmartBarVol.h"
#if CVTE_EN_KEY_PROXY
#include "../../CarPlay/CarplayAdapter.h"
#endif

#if CVTE_EN_CARPLAY

#define DEBUG_CARPLAY			0

typedef struct _CARPLAY_MODE_STATUS
{
	int mainScreen ;
	int mainAudio ;
	int phoneCall ;
	int speechState ;
	int speechMode ;
	int turnByTurn ;
	int altAudio;
}CARPLAY_MODE_STATUS;

class CPanelCarPlay :
	public CPanelGL
{
public:
	CPanelCarPlay(void);
	~CPanelCarPlay(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();
	BOOL IsEnableQuit();
	void OnSystemPowerStatePrepare(UINT idState);
	void OnSystemPowerStateDone(UINT idState);

	void OnBrakeStatusChanged(BOOL bOn);
	void OnReverseModeChange(BOOL bReverse);
	void OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam);
	void OnNightModeChanged(BOOL bNightMode);

	void OnGestureMsg(UINT idGesture);
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);
    void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);
    void AutoSwitchCarPlayMedia();
	void AutoSwitchNativeUI();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnSRCChanged(UINT idSRC);

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif
	
	void RequestNativeUI(SWITCH_PAGE_PARAM *para);
	void ReleaseNativeUI();

	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnPlaySoundEffect(BOOL bPlay);

	void OnRequestSiri();
	void OnRequestUI(const char* inURL);

	void LinkSmartBarVol(CSmartBarVol *pSmartBar){m_pSmartBarVol = pSmartBar;}
	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar){m_pSmartBarMedia = pSmartBar;}

	void PlayNext();
	void PlayPrevious();
	void Play();
	void Pause();
	void PlayPause();

    void AnswerOrHang();
    void Answer();
    void Hang();

protected:
	void StartCarPlay();
	void StopCarPlay();

private:
	void SetMsg(const wchar_t *pStrMsg){m_tagTitle.SetCaption(pStrMsg);}

private:

	CSmartBarVol	*m_pSmartBarVol;
	CSmartBarMedia	*m_pSmartBarMedia;

	BOOL			m_bConnected;
    BOOL            m_bControlConnected;
	BOOL			m_bVideoConnected;
	BOOL			m_bOnNightMode;

	BOOL			m_bCarPlaySpeechOn;
	BOOL			m_bCarPlayPhoneCallOn;
	BOOL			m_bCarPlayAltAudioOn;
	BOOL			m_bCarPlayMainAudioOn;
	BOOL			m_bCarPlayTurnbyTurnOn;

	DWORD			m_idUIBeforePhoneCallOn;
	DWORD			m_idUIBeforeSiri;
	DWORD			m_idUIBeforeReverse;


	BOOL			m_bCarPlayTakeMainAudio;
	BOOL			m_bCarPlayAudioOnDucking;

	//
	UINT			m_idSRCBeforeCarplayTakeAudio;

	DWORD			m_dwDisableReloadScreenTick;

	//
	CLabelGL m_bk;
	BOOL			m_bHasValidBackImage;

	CLabelGL		m_iconLoading;

	BOOL			m_bSpeechOn;
	BOOL			m_bPhoneCallActive;

	BOOL			m_bCarPlayRunning;
	DWORD			m_dwEnterTick;

	CLabelGL		m_iconLink;
	CButtonGL		m_tagTitle;

	BOOL			m_bNeedReturnHome;

	UINT			m_idRequestURL;
	BOOL			m_bManualEnter;

	BOOL			m_bPlaying;
	BOOL			m_bPlayingBeforeIdle;

	BOOL			m_bNeedTakeScreen;
	BOOL			m_bNeedUnTakeScreen;
	BOOL			m_bRdsTAon;

	DWORD		m_dwAutoSwitchCarplayRef;
	BOOL			m_bRefreshScreen;
	SWITCH_PAGE_PARAM		m_NativeRequestUIPara;

	BOOL	m_bIdleTakeResource;
	BOOL	m_bRequestingCarPlayUI;
	DWORD m_dwVideoConnectTick;

#if CVTE_EN_KEY_PROXY
	ButtonHIDReport_t m_buttonkey;
	PhoneKeyHIDReport_t m_phonekey;
#endif	
};

#endif

