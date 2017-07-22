#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"

#define DEBUG_STATUSBAR						0

#define BT_STATUS_OFF							0
#define BT_STATUS_CONNECTING			1
#define BT_STATUS_CONNECTED				2
#define BT_STATUS_READY						3

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
#define CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR	CVTE_DISABLE//CVTE_ENABLE
#else
#define CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR	CVTE_DISABLE
#endif

class CStatusBarEx :
	public CStatusBar
{
public:
	CStatusBarEx(void);
	~CStatusBarEx(void);

	void Render(void);
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void Show(BOOL bShow,BOOL bAni=FALSE);
	BOOL IsReady();

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

// 	BOOL IsOnBack(POINT *pt){return m_bEnableButtons&&IsVisible()&&m_btBack.IsPtnOnObject(pt)&&m_btBack.IsPtnOnObject(&m_ptLastPress);}
// 	BOOL IsOnHome(POINT *pt){return m_bEnableButtons&&IsVisible()&&m_btHome.IsPtnOnObject(pt)&&m_btHome.IsPtnOnObject(&m_ptLastPress);}

	void SetTitle(const wchar_t *pTitle){m_tagTitle.SetCaption(pTitle,TRUE);}
// 	void SetSettingTitle(const wchar_t *pTitle){m_tagTitle.SetCaption(pTitle);}
// 	void SetPhoneName(const wchar_t *pTitle);

// 	void SetPhoneNumber(const wchar_t *pTitle);

	void SetCurDisplayPanel(UINT idPanel){m_idCurDisplayPanel=idPanel;}
	UINT GetCurDisplayPanel(){return m_idCurDisplayPanel;}
	void SetCurUIClass(UINT idClass){m_idUIClass=idClass;}
	UINT GetCurUIClass(){return m_idUIClass;}

	void SetMute(BOOL bMute){m_bOnMute=bMute;}
	void SetVolume(DWORD dwVol);
	void SetEQ(UINT idEQ);

// 	void SetSimpleMode(BOOL bOn){if(m_idShowType==SB_NORMAL)m_bSimpleMode=bOn;}
// 	UINT GetShowType(){return m_idShowType;}
// 	void SetShowType(UINT idType);
// 	void SetSliderValue(INT iSet){m_sliSettings.SetValue(iSet,FALSE);}

// 	BOOL IsSettingSliderValueChanged(){return m_sliSettings.IsValueChanged();}
// 	int GetSettingSliderValue(){return m_sliSettings.GetValue();}
// 	void EnableButtons(BOOL bEnable);
#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
	void SetNaviStatus(BOOL bEnable);
#endif
	void SetWiFiStatus(WIFI_STATUS status);
	void SetWiFiPowerON(BOOL bOn){if(!bOn)SetWiFiStatus(WIFI_OFF);m_bWifiPowerON=bOn;}

	void SetDialupStatus(DIALUP_STATUS status);

	void SetAFStatus(RDS_STATUS status);
	void SetTAStatus(RDS_STATUS status);

	void SetBTStatus(DWORD dwStatus,DWORD dwSignal,DWORD dwBattery);
    void SetTimeMode(UINT iTimeMode);
#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
#endif

private:
	void UpdateTime();

private:
// 	UINT m_idCurDisplayPanel;
// 	UINT	 m_idUIClass;
// 	CLabelGL m_tagBKBottom;
// 	CLabelGL m_tagBKTop;
// 	CLabelGL m_tagLight;

	CButtonGL m_btnPopup;

	CLabelGL	m_iconBT;
	CLabelGL	m_iconVolume;
	CLabelGL	m_iconMute;
// 	CLabelGL	m_iconCall;
// 	CLabelGL	m_iconSMS;
// 	CLabelGL	m_iconPower;
	CLabelGL	m_icon3G;
	CLabelGL	m_iconWifi;
	CLabelGL	m_iconRDS;
	CLabelGL	m_iconCustomerLogo;

	CLabelGL	m_iconPhonePower;

	CSliderGL	m_sliPhonePower;
	CSliderGL	m_sliPhoneSignal;

// 	CLabelGL		m_iconAdd;
// 	CLabelGL		m_iconSub;
// 	CLabelGL		m_iconDial;
// 	CLabelGL		m_iconHang;

// 	CLabelGL	m_iconHome;
// 	CButtonGL	m_btHome;
// 	CLabelGL	m_iconBack;
// 	CButtonGL	m_btBack;
	CButtonGL	m_tagTitle;
// 	CButtonGL	m_tagPhoneName;
// 	CButtonGL	m_tagPhoneNumber;
// 	CButtonGL	m_tagSettingItemName;

// 	CButtonGL	m_tagHour1;
// 	CButtonGL	m_tagHour2;
// 	CButtonGL	m_tagMin1;
// 	CButtonGL	m_tagMin2;
// 	CButtonGL	m_tagTimeSep;

	CButtonGL	m_tagAM;
	CButtonGL	m_tagTime;
	CButtonGL	m_tagVolume;
	CButtonGL	m_tagEQ;

	CButtonGL	m_tagAC;

// 	CButtonGL	m_tagAF;
// 	CButtonGL	m_tagTA;

// 	CSliderGL		m_sliSettings;

	BOOL		m_bBTConnecting;
// 	BOOL		m_bEnableButtons;

	UINT			m_idBTStatus;
	UINT			m_idAFStatus;
	UINT			m_idTAStatus;
	UINT			m_idEQStatus;
	UINT			m_idLoudStatus;
	UINT			m_iVolume;
    UINT            m_iTimeMode;

// 	INT			m_iTreble;
// 	INT			m_iBass;
// 	INT			m_iSubwoofer;
// 	INT			m_iBalance;
// 	INT			m_iFAD;
//
// 	BOOL		m_bMute;
// 	DWORD	m_dwShowSetTick;
// 	UINT			m_idAudioSetType;
	BOOL		m_bOnMute;
// 	BOOL		m_bSimpleMode;
	DWORD	m_dwVolume;

// 	UINT			m_idShowType;
// 	UINT			m_dwLastShowSettingTick;
// 	UINT			m_idShowTypeBeforeSetting;

	WIFI_STATUS	m_wifiStatus;
	BOOL				m_bWifiPowerON;
	DIALUP_STATUS	m_dialupStatus;
	RDS_STATUS			m_statusAF;
	RDS_STATUS			m_statusTA;

	BOOL		m_bBtnTimeClicked;
#if CVTE_EN_UI_ALDINET_STYLE_STATUS_BAR
	CLabelGL	m_iconNavi;
	CLabelGL	m_iconSwitch;
	CLabelGL	m_iconSwitchSelect;
	CButtonGL	m_btnPage1;
	CButtonGL	m_btnPage2;
	CButtonGL	m_btnPage3;
	CButtonGL	m_btnArrowLeft;
	CButtonGL	m_btnArrowRight;
	CLabelGL	m_iconDropMenu;
	CLabelGL	m_iconDropMenuSelect;
	CButtonGL	m_btnTopDropMenu;
	BOOL		m_bBtnTopDropMenuClicked;
#endif
};
