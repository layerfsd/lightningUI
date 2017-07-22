#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#include <afxwin.h>

#if CVTE_EN_CARPLAY
class CPanelSettingIperfUDPClient :
	public CPanelGL
{
public:
	CPanelSettingIperfUDPClient(void);
	~CPanelSettingIperfUDPClient(void);

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

	void UpdateButtonsStatus(LPCWSTR inBandwidth);
	void StartUDPTest();

    CButtonGL m_tagTitle1;
    CButtonGL m_tagTitle2;

    CLabelGL  m_labIcon;
    CLabelGL  m_labIconFocus;
    CButtonGL m_btnReturn;

	CButtonGL m_tagIperfUDPClientCmd;
	CButtonGL m_btnUDPCmd;
	CButtonGL m_tagUDPResults;
	CButtonGL m_btnUDP50m;
	CButtonGL m_tagUDP50m;
	CButtonGL m_btnUDP100m;
	CButtonGL m_tagUDP100m;
	CButtonGL m_btnUDP150m;
	CButtonGL m_tagUDP150m;
	CButtonGL m_tagAlertMessage;
	CButtonGL m_tagConsoleOutput;
	CButtonGL m_btnConsoleOutput;

	BOOL m_bOutput;

	CAtlStringW m_IperfReportstr;
	CAtlStringW m_IperfResultsstr50m;
	CAtlStringW m_IperfResultsstr100m;
	CAtlStringW m_IperfResultsstr150m;

	UINT m_testmode;
	UINT m_testresults;
	UINT m_uIDEvent;
};

#endif