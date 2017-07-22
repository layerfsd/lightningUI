#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../VIP/CVideoWindow.h"
#include "../../Communication/CommanderEx.h"
#include "../../Communication/CmdSender_AndroidTouch.h"

#define DEBUG_TV		0


class CPanelTV :
	public CPanelGL
{
public:
	CPanelTV(void);
	~CPanelTV(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void LinkVideoWindow(CVideoWindow *pVideoWindow){m_pVideoWindow = pVideoWindow;}

	void OnReverseModeChange(BOOL bReverse);
    void OnSpeechStart();
    void OnSpeechTerminate();
    void OnPhoneCallState(WPARAM wParam, LPARAM lParam);
	void OnTouchMsg(PMULTIPOINT pCyMultiPoint);


	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif
	
    void ShowAlert(BOOL bShow){m_bShowAlert = bShow;}
    void OnBrakeStatusChanged(BOOL bOn);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
	static void CALLBACK OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lpContext);

	static void CALLBACK OnVideoWndTouchDown(POINT *pt,LPVOID lparam);
	static void CALLBACK OnVideoWndTouchUp(POINT *pt,LPVOID lparam);
	static void CALLBACK OnVideoWndTouchMove(POINT *pt,LPVOID lparam);
	static void CALLBACK OnVideoWndTouchDBClick(POINT *pt,LPVOID lparam);
#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	void SendPointCmd(POINT *pt, BYTE bDown);
#else
	void SendPointCmd(POINT *pt, BOOL bDown);
#endif

#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	static void CALLBACK CommandProc(LPBYTE pCmd,DWORD dwCmdLen,LPVOID lparam);
#endif

private:

	CVideoWindow				*m_pVideoWindow;

	CLabelGL			m_iconTV;
    CLabelGL        m_iconAlert;
	BOOL			m_bSpeechOn;
	BOOL			m_bPhoneCallActive;

    BOOL            m_bShowAlert;

    BOOL            m_bVideoWindowShow;
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

	CButtonGL		m_tagDTVTitle;

#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	CCommander		*m_pTouchCmd;
	CmdSender_AndroidTouch *m_pTouchSender;
#endif
	POINT       	m_ptPoint1;
    POINT       	m_ptPoint2;

	DWORD			m_dwEnterTick;
	DWORD			m_dwReverseTick;
	DWORD			m_dwVideoStartTick;
};
