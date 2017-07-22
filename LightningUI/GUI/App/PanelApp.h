#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../Controls/SmartBarDrop.h"
#include "../Controls/SmartBarMedia.h"

#define DEBUG_PANELAPP					1

struct   EnumParam  
{  
	HWND   hWnd[16];
	DWORD   dwProcessID;  
	DWORD   dwThreadID;
	DWORD   dwWndNum;
};

class CPanelApp :
	public CPanelGL
{
public:
	CPanelApp(UINT idIcon=0,UINT idBackTex=0);
	~CPanelApp(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void OnGestureMsg(UINT idGesture);
	void OnSystemPowerStatePrepare(UINT idState);
	void OnSystemPowerStateDone(UINT idState);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void SetAppName(const wchar_t *pName){m_strAppName=pName;}
	void SetAppPath(const wchar_t *pPath){m_strAppPath=pPath;}

	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);
#if CVTE_EN_CARPLAY
	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);
#endif
#if CVTE_EN_ANDROID_AUTO
	void OnAndroidAutoMsg(WPARAM wParam, LPARAM lParam);
#endif
	void OnNotifyRdsTAStatus(WPARAM wParam, LPARAM lParam);
	void OnSRMsg(WPARAM wParam, LPARAM lParam);

	//
	void SetAudioSrcId(UINT idSRC){m_idAudioSRC = idSRC;}

	static DWORD WindowDetectThreadPro(LPVOID lpParam);

	void LinkSmartBarDrop(CSmartBarDrop *pSmartBar){m_pSmartBarDrop = pSmartBar;}
	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar){m_pSmartBarMedia = pSmartBar;}

private:
	BOOL LaunchApp();
	void OnAppWindowDetect();

	static DWORD LaunchThreadPro(LPVOID lpParam);
	BOOL LaunchAppProc();

private:
	CLabelGL m_bk;
	CLabelGL		m_iconApp;
	CM_String		m_strAppName;
	UINT				m_idBackTex;
	UINT				m_idIcon;

	CSmartBarDrop		*m_pSmartBarDrop;
	CSmartBarMedia	*m_pSmartBarMedia;

	UINT				m_idAudioSRC;

	BOOL			m_bHasValidBackImage;

	DWORD		m_dwEnterTick;
	BOOL				m_bAutoShowDropBar;
	BOOL				m_bEnableShowDropBar;

	BOOL				m_bAppLaunched;
	BOOL				m_bAppStartedSuceess;

	CM_String			m_strAppPath;
	HWND				m_hAppWnd;
	HANDLE			m_hAppProcess;

	HANDLE			m_hAppLaunchThread;

	HANDLE			m_hWindowDetectThread;

	BOOL				m_bEnableWindowDetect;
	HANDLE			m_hWindowDetectEvent;

	BOOL				m_bPhoneCallActive;
	BOOL			m_bCarPlayConnected;
	BOOL				m_bRdsTAon;
	BOOL				m_bSRon;

	EnumParam m_ep;

	int		m_iGPSGroundSpeed;
	BOOL	m_bNeedRenderSplash;

    UINT    m_idPowerState;
    BOOL    m_bPowerStateDone;
};
