#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"

class CPanelSettingIperfServer :
	public CPanelGL
{
public:
	CPanelSettingIperfServer(void);
	~CPanelSettingIperfServer(void);

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

	void ServerIPAddressUpdate();

    CButtonGL m_tagTitle1;
    CButtonGL m_tagTitle2;

    CLabelGL  m_labIcon;
    CLabelGL  m_labIconFocus;
    CButtonGL m_btnReturn;

	CButtonGL m_tagIperfServerCmd;
	CButtonGL m_btnTCPCmd;
    CButtonGL m_tagTCPCmd;
    CButtonGL m_btnUDPCmd;
	CButtonGL m_tagUDPCmd;
	CButtonGL m_tagIperfServerIPAddress;
	CButtonGL m_btnServerIPAddress;
	CButtonGL m_tagAlertMessage;
	CButtonGL m_tagAlertTitleTCP;
	CButtonGL m_tagAlertCmdTCP;
	CButtonGL m_tagAlertTitleUDP;
	CButtonGL m_tagAlertCmdUDP;

	BOOL m_bServerIPAddressUpdated;
};

