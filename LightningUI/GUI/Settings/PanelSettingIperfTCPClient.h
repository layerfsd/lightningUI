#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#include <afxwin.h>

class CPanelSettingIperfTCPClient :
	public CPanelGL
{
public:
	CPanelSettingIperfTCPClient(void);
	~CPanelSettingIperfTCPClient(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

    void OnMCUCmd(WPARAM wParam, LPARAM lParam);

	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:

	static void CALLBACK TimerProc(UINT wTimerID,UINT msg,DWORD_PTR dwUser,DWORD dw1,DWORD dw2);
	void OnTimer(UINT wTimerID,UINT msg,DWORD dw1,DWORD dw2);

	void UpdateButtonsStatus();

    CButtonGL m_tagTitle1;
    CButtonGL m_tagTitle2;

    CLabelGL  m_labIcon;
    CLabelGL  m_labIconFocus;
    CButtonGL m_btnReturn;

	CButtonGL m_tagIperfTCPClientCmd;
	CButtonGL m_btnTCPCmd;
	CButtonGL m_tagBandwidthResults;
	CButtonGL m_btnBandwidth;
	CButtonGL m_tagBandwidth;
	CButtonGL m_tagAlertMessage;
	CButtonGL m_tagConsoleOutput;
	CButtonGL m_btnConsoleOutput;

	BOOL m_bOutput;

	CAtlStringW m_IperfReportstr;
	CAtlStringW m_IperfResultsstr;

	BOOL m_testmode;
	UINT m_uIDEvent;
	
};

