#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../VIP/CVideoWindow.h"
#include "../../Communication/CommanderEx.h"
#include "../Controls/RadarBar.h"
#include "../../VIP/CRearOverlay.h"
#include "../Controls/SmartBarVol.h"
#include "../Controls/SmartBarTopBanner.h"

#define DEBUG_REARVIEW		1

class CPanelRearView :
	public CPanelGL
{
public:
	CPanelRearView(void);
	~CPanelRearView(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnReverseModeChange(BOOL bReverse);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);
	void OnRearViewKey(WPARAM wParam, LPARAM lParam);


	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void LinkVideoWindow(CVideoWindow *pVideoWindow){m_pVideoWindow = pVideoWindow;}
	void LinkSmartBarVol(CSmartBarVol *pSmartBar){m_pSmartBarVol = pSmartBar;}
	void LinkSmartBarTopBanner(CSmartBarTopBanner *pSmartBar){m_pSmartBarTopBanner = pSmartBar;}
    void LinkRearOverlay(CRearOverlay *pRearOverlay){m_pRearOverlay = pRearOverlay;}

	void OnCanbusStatus(WPARAM wParam, LPARAM lParam);
	void OnRadarStatus(WPARAM wParam, LPARAM lParam);
	void OnTouchMsg(PMULTIPOINT pCyMultiPoint);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void StartCAMVideo();
	void StopCAMVideo();
    void SwitchTW8836Channel(BYTE uChannel);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
private:
	static DWORD RearMonitorThreadPro(LPVOID lpParam);
	BOOL InitRearMonitor();
	void StartRearMonitor();
	void StopRearMonitor();
	void RearMonitorRun();
#endif
private:
	void SendPointCmd(POINT *pt, BOOL bDown);

private:
	BOOL				m_bCAMVideoStarted;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	HANDLE			m_hRearMonitorStartEvt;
	HANDLE          m_hRearMonitorEndEvt;
	HANDLE          m_hRearMonitorThread;
	BOOL            m_bRearMonitorWorking;
	BOOL			m_bHasVideoSignal;
#endif

private:

	CVideoWindow				*m_pVideoWindow;
	CSmartBarVol                *m_pSmartBarVol;
	CSmartBarTopBanner			*m_pSmartBarTopBanner;
    CRearOverlay                *m_pRearOverlay;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL			m_tagCamera;
	CLabelGL			m_tagCameraBox;
	CLabelGL			m_tagCameraTitle;	
	CButtonGL			m_btnHome;
#else
	CLabelGL			m_iconCamera;
#endif
	/*
	//toolbar:
	CLabelGL			m_iconHomeUp;
	CLabelGL			m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL			m_iconSettingUp;
	CLabelGL			m_iconSettingDown;
	CButtonGL		m_btnSetting;
*/

	CRadarBar		m_barRadar;

	DWORD			m_dwEnterTick;
    DWORD           m_dwVideoStartTick;

	BOOL			m_bHasRadar;

	POINT       	m_ptPoint1;


};

