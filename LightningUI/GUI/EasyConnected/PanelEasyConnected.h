#pragma once
#include "../Bluetooth FLC/BluetoothCommon.h"
#include <windev.h>
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"

#define DEBUG_EASYCONNECTED			1
typedef BOOL (WINAPI *PFN_SENDNOTIFYMESSAGE)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

class CPanelEasyConnected :
	public CPanelGL
{
public:
	CPanelEasyConnected(void);
	~CPanelEasyConnected(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void OnSystemPowerStatePrepare(UINT idState);

	void OnGestureMsg(UINT idGesture);

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

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

	static LRESULT CALLBACK EasyConnectedWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnEasyConnectedMsg(UINT message, WPARAM wParam, LPARAM lParam);
    void OnDeviceChange(LPTSTR lpDevName, BOOL bNew);
	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);
    void OnBluetoothMsg(WPARAM wParam, LPARAM lParam);

	//void NotifyConnected(BYTE nConnectedStatus);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnPlaySoundEffect(BOOL bPlay);

	//void OnA2dpConnected(BOOL bConnected);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

	BOOL PostMsgToClient(UINT message, WPARAM wParam, LPARAM lParam);
	void SendMsgToClient(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL TransDataProcess();

private:
	BOOL EasyConnectedInitSrv();
	BOOL EasyConnectedRunApp(BOOL bShow);
	void EasyConnectedDeinitSrv();
    void EasyConnectedShowWindow(BOOL bShow);
    void EasyConnectedSetLanguage();
    UINT EasyConnectedDeviceStatusCheck();
    BOOL AutoResume();
	void SetMsg(const wchar_t *pStrMsg){m_tagTitle.SetCaption(pStrMsg);}
    PFN_SENDNOTIFYMESSAGE m_fnSendNotifyMessage;
private:

	HWND			m_hwndEasyConnectedNotify;
	//HWND			m_hwndClient;
    HMODULE         m_hmCoreDll;


	BYTE		    m_nConnectedStatus;
	BOOL		    m_bEasyConnectedVisible;
    BOOL            m_bExitEasyConnected;
    BOOL            m_bIsReadyToScreen;
    BOOL            m_bInteractive;
    BOOL            m_bLanguageSwitched;
    BOOL            m_bFocusGet;

    DWORD           m_dwSetLanguageTick;

    UINT            m_iPowerStatus;
	//toolbar:
	BOOL			m_bSpeechOn;

	BOOL            m_bA2dpOn;
	BOOL            m_bA2dpConnected;
	BOOL            m_bAutoResumeFlag;
	DWORD	        m_dwEnterTick;

	//CM_String			m_strMsg;
	CButtonGL		m_btnCancel;
	CLabelGL		m_iconLink;
	CButtonGL		m_tagTitle;
	CLabelGL		m_iconLoading;

	BOOL			m_bAutoSwitchToEasyConnected;

    BOOL            m_bAutoResumeTimer;
	BOOL            m_bPlayingFlag;

};
