#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../VIP/CVideoWindow.h"
#include "../../Communication/CommanderEx.h"

#if CVTE_EN_DVD
#define DEBUG_DVD		1


class CPanelDVD :
	public CPanelGL
{
public:
	CPanelDVD(void);
	~CPanelDVD(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void LinkVideoWindow(CVideoWindow *pVideoWindow){m_pVideoWindow = pVideoWindow;}

	void OnReverseModeChange(BOOL bReverse);
    void ShowAlert(BOOL bShow){m_bShowAlert = bShow;}
    void OnBrakeStatusChanged(BOOL bOn);
    void OnSpeechStart();
    void OnSpeechTerminate();
    void OnPhoneCallState(WPARAM wParam, LPARAM lParam);
	
	#if CVTE_EN_KEY_PROXY
    BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
	static void CALLBACK OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lpContext);
	void SendPointCmd(POINT *pt);

private:

	CVideoWindow				*m_pVideoWindow;
    CLabelGL		    m_iconAlert;
	CLabelGL			m_iconDVD;
    BOOL                m_bShowAlert;


	/*
	//toolbar:
	CLabelGL			m_iconHomeUp;
	CLabelGL			m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL			m_iconSettingUp;
	CLabelGL			m_iconSettingDown;
	CButtonGL		m_btnSetting;
*/
	CButtonGL		m_tagTitle;

	CButtonGL		m_tagDVDTitle;

	DWORD			m_dwEnterTick;
    DWORD           m_dwVideoStartTick;
    BOOL            m_bSpeechOn;
    BOOL            m_bPhoneCallActive;
};
#endif
