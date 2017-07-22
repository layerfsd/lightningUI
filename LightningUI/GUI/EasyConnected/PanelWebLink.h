#pragma once
#include "../Bluetooth FLC/BluetoothCommon.h"
#include <windev.h>
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../../Weblink/IIPCNotification.h"

#define DEBUG_WEBLINK			1
typedef BOOL (WINAPI *PFN_SENDNOTIFYMESSAGE)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

class CPanelWebLink :
	public CPanelGL,
	protected IIPCNotification
{
public:
	CPanelWebLink(void);
	~CPanelWebLink(void);

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

    void OnDeviceChange(LPTSTR lpDevName, BOOL bNew);
	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);
    void OnBluetoothMsg(WPARAM wParam, LPARAM lParam);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnPlaySoundEffect(BOOL bPlay);

	void OnInit( bool init );
	void OnConnect( bool connect );
	void OnWindowStatus( bool enable );

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
	BOOL TransDataProcess();

private:
	BOOL WeblinkRunApp(BOOL bShow);
    void WeblinkShowWindow(BOOL bShow);
	void SetMsg(const wchar_t *pStrMsg){m_tagTitle.SetCaption(pStrMsg);}
	static void CALLBACK AutoResumeTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	void AutoResumeTimer();
	BOOL AutoResume();
private:
	BOOL		    m_bWeblinkVisible;
    BOOL            m_bExitWeblink;
	//toolbar:
	BOOL			m_bSpeechOn;
	BOOL			m_bRunApp;
	BOOL			m_bConnect;
	BOOL			m_bWindowEnable;

	DWORD	        m_dwEnterTick;

	//CM_String			m_strMsg;
	CButtonGL		m_btnCancel;
	CLabelGL		m_iconLink;
	CButtonGL		m_tagTitle;
	CLabelGL		m_iconLoading;

	BOOL			m_bAutoResumeTimer;
	BOOL			m_bAutoResumeFlag;
	MMRESULT 		m_AutoResumeTimer;

	BOOL            m_bPlayingFlag;
	BOOL            m_bA2dpConnected;

};
