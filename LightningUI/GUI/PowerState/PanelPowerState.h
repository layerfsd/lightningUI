#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"


class CPanelPowerState :
	public CPanelGL
{
public:
	CPanelPowerState(void);
	~CPanelPowerState(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
    
private:
    static DWORD PowerManagerThreadPro(LPVOID lpParam);
    BOOL StartPowerManager();
    void StopPowerManager();
    BOOL PowerManagerRun();

protected:

private:
	CLabelGL		m_iconLogo;
    CLabelGL        m_iconLogo2;
	DWORD		    m_dwEnterTick;
    
	BOOL			m_bNotifyPrePowerMsg;
	BOOL			m_bNotifyPowerMsg;

    HANDLE          m_hPowerManagerEndEvt;
    HANDLE          m_hPowerManagerThread;
    BOOL            m_bEndPowerManager;
};
