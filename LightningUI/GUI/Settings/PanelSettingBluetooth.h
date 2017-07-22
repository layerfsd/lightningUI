#pragma once
#include "../Bluetooth FLC/BluetoothCommon.h"

#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"

class CPanelSettingBluetooth :
	public CPanelGL
{
public:
	CPanelSettingBluetooth(void);
	~CPanelSettingBluetooth(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnPhoneHFState(WPARAM wParam, LPARAM lParam);
	void OnBluetoothSetupStatus(WPARAM wParam, LPARAM lParam);
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);
	//
	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	//
	void RefreshSettingItem(BOOL bShow);

private:

	CButtonGL	m_tagTitlePower;
	CSliderGL		m_sliPower;

// 	CButtonGL	m_btnConnect;
// 	CButtonGL	m_tagValueConnect;

	CButtonGL	m_btnDeviceName;
	CButtonGL	m_tagValueDeviceName;

// 	CButtonGL	m_btnPairPin;
// 	CButtonGL	m_tagValuePairPin;

	CButtonGL	m_tagTitleAutoConnect;
	CSliderGL		m_sliAutoConnect;

	CButtonGL	m_tagTitleAutoAnswer;
	CSliderGL		m_sliAutoAnswer;

	CButtonGL	m_tagTitleAutoDownloadPB;
	CSliderGL		m_sliAutoDownloadPB;

#if CVTE_EN_BT_VCARD_NAME_ORDER
	CButtonGL	m_tagTitleVcardNameOrder;
	CSliderGL	m_sliVcardNameOrder;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CButtonGL	m_tagBTNote;
#endif

// 	CButtonGL	m_tagTitleRingtone;
// 	CSliderGL		m_sliRingtone;

	CButtonGL	m_btnBluetoothVersion;
	CButtonGL	m_tagValueBluetoothVersion;

	UINT				m_iBTPowerState;

	ConnectionStatus m_hfConnectStatus;
	BOOL					m_bPhoneCallActive;

	CLabelGL		m_iconSearching;

	BOOL			m_bCarPlayConnected;
	BOOL			m_bShowDetails;
	BOOL			m_bOperatePowerOn;
	BOOL			m_bAutoSwitchDeviceMgr;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif

};

