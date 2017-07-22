#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"

class CPanelSettingIperfClient :
	public CPanelGL
{
public:
	CPanelSettingIperfClient(void);
	~CPanelSettingIperfClient(void);

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

	void UpdateSwitchStatus();
	void UpdateRunTestStatus();

    CButtonGL m_tagTitle1;
    CButtonGL m_tagTitle2;

    CLabelGL  m_labIcon;
    CLabelGL  m_labIconFocus;
    CButtonGL m_btnReturn;
	CButtonGL m_btnTCPSwitch;
	CButtonGL m_btnUDPSwitch;
	CButtonGL m_btnAddress;
	CButtonGL m_btnTagAddress;
	CButtonGL m_btnTagAlertAddress;
	CButtonGL m_btnDelete;
	CButtonGL m_btnPort;
	CButtonGL m_btnTagPort;
	CButtonGL m_btnRunTestBack;
	CButtonGL m_btnRunTest;

	CButtonGL	m_btnNum0;
	CButtonGL	m_btnNum1;
	CButtonGL	m_btnNum2;
	CButtonGL	m_btnNum3;
	CButtonGL	m_btnNum4;
	CButtonGL	m_btnNum5;
	CButtonGL	m_btnNum6;
	CButtonGL	m_btnNum7;
	CButtonGL	m_btnNum8;
	CButtonGL	m_btnNum9;
	CButtonGL	m_btnNumA;
	CButtonGL	m_btnNumB;
	CButtonGL	m_btnNumC;
	CButtonGL	m_btnNumD;
	CButtonGL	m_btnNumE;
	CButtonGL	m_btnNumF;
	CButtonGL	m_btnDot;
	CButtonGL	m_btnColon;
	CButtonGL	m_btnSign;

	BOOL m_bIsUDP;
};

