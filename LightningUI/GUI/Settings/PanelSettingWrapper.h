#pragma once
#include "PanelSettingBluetooth.h"
#include "PanelSettingAudio.h"
#include "PanelSettingGeneral.h"
#include "PanelSettingDate.h"
#include "PanelSettingTime.h"
#include "PanelSettingRadio.h"
#include "PanelSettingAbout.h"
#include "PanelSettingBackground.h"
#include "PanelSettingDisplay.h"
#include "PanelSettingLanguage.h"
#include "PanelSettingAdvanced.h"
#include "PanelSettingTimeZone.h"
#include "PanelSettingRdsPTY.h"
#include "PanelSettingFactory.h"
#include "PanelSettingGesture.h"
#include "PanelSettingDAB.h"
#include "..\Update\PanelUpdateARM.h"
#include "PanelPassword.h"
#include "PanelSettingIperfServer.h"
#include "PanelSettingIperfClient.h"
#include "PanelSettingIperfTCPClient.h"
#include "PanelSettingIperfUDPClient.h"
#include "PanelSettingConnectTest.h"
#include "PanelSettingRadioRegion.h"
#include "PanelSettingAndroidType.h"
class CPanelSettingWrapper :
	public CPanelGL
{
public:
	CPanelSettingWrapper(void);
	~CPanelSettingWrapper(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);

	void OnDeviceChanged();
#if CVTE_EN_CPMAC
	void OnMACChanged();
#endif

	//for bt:
	void OnPhoneHFState(WPARAM wParam, LPARAM lParam);
	void OnBluetoothSetupStatus(WPARAM wParam, LPARAM lParam);
    void OnCameraTypeChange(UINT uCameraType);
    void OnRearColorSignalChange(UINT uColorSignal);
    void OnBackgroundChanged(WPARAM wParam, LPARAM lParam);
    void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
    void OnDeviceRemovedReady(LPCTSTR lpDeviceName);
    void OnPanelLightChange();
    void OnLCDBrightnessChange();
    void OnHeadLightOn(BOOL bOn, UINT nBk);
    void OnSetPasswordBox(NBMSG *pMsg);
	void OnPasswordBoxCmd(WPARAM wParam, LPARAM lParam);
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);
	//
	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);
	void OnGestureCircle(BOOL bCircleRight);
    void OnGestureID(WPARAM wParam, LPARAM lParam);

	//for audio:
	void OnVolumeConfigMsg(WPARAM wParam, LPARAM lParam);

    #if CVTE_EN_LED
    //for led:
    void LinkLED(CLED *phLED){m_pLED = phLED;}
    #endif

    void LinkAppConfig(CAppConfigProxy *phAppConfig){m_pAppConfig = phAppConfig;}

	//for dab:
	void OnDabServiceStartReady(WPARAM wParam, LPARAM lParam);

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	void LinkAudioCodec(CAudioCodec *pAudioCodec);
#endif
protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);



private:
    #if CVTE_EN_LED
    CLED                *m_pLED;
    #endif

    CAppConfigProxy     *m_pAppConfig;
	UINT		m_idCurSubpage;

	CPanelSettingAudio		m_panelAudio;
	CPanelSettingGeneral		m_panelGeneral;
	CPanelSettingDisplay		m_panelDisplay;
	CPanelSettingTime		m_panelTime;
	CPanelSettingDate		m_panelDate;
	CPanelSettingRadio		m_panelRadio;
	CPanelSettingDAB		m_panelDAB;
	CPanelSettingBluetooth		m_panelBluetooth;
	CPanelSettingAbout		m_panelAbout;
    CPanelSettingBackground   m_panelSettingBackground;
    //CPanelSettingBackground   *m_ppSettingBackground;
	CPanelSettingLanguage   m_panelSettingLanguage;
	CPanelSettingAdvanced   m_panelSettingAdvanced;
	CPanelSettingTimeZone   m_panelSettingTimeZone;
	CPanelSettingRdsPTY   m_panelSettingRdsPTY;
    CPanelSettingFactory   m_panelSettingFactory;
    CPanelUpdateARM     m_panelUpdateARM;
    CPanelPassword      m_panelPassword;
#if CVTE_EN_CARPLAY
	CPanelSettingIperfServer   m_panelSettingIperfServer;
	CPanelSettingIperfClient   m_panelSettingIperfClient;
	CPanelSettingIperfTCPClient m_panelSettingIperfTCPClient;
	CPanelSettingIperfUDPClient m_panelSettingIperfUDPClient;
	CPanelSettingConnectTest m_panelSettingConnectTest;
#endif

	CPanelSettingRadioRegion	m_panelSettingRadioRegion;
#if (CVTE_EN_GESTURE_AIR == 1)
    CPanelSettingGesture m_panelGesture;
#endif
#if CVTE_EN_COMBINE_ANDROID_LINK
	CPanelSettingAndroidType   m_panelSettingAndroidType;
#endif
	CLabelGL		m_iconHomeUp;
	CLabelGL		m_iconHomeDown;
	CButtonGL		m_btnHome;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	enum SHOW_ARROW
	{
		GENERAL_SHOW_ARROW = 0,
		AUDIO_SHOW_ARROW,
		DISPLAY_SHOW_ARROW,
		BLUETOOTH_SHOW_ARROW,
		RADIO_SHOW_ARROW,
		ABOUT_SHOW_ARROW
	};

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

	CLabelGL			m_iconArrowOn;
	CLabelGL			m_iconArrowOff;

	CListBoxItemGL		*m_pGeneralItem;
	CListBoxItemGL		*m_pAudioItem;
	CListBoxItemGL		*m_pDisplayItem;
	CListBoxItemGL		*m_pBluetoothItem;
	CListBoxItemGL		*m_pRadioItem;
	CListBoxItemGL		*m_pAboutItem;
	void	ArrowChange(UINT index);
#endif

	//
	CLabelGL			m_iconGeneral;
	CLabelGL			m_iconGeneralFocus;
	CLabelGL			m_iconAudio;
	CLabelGL			m_iconAudioFocus;
	CLabelGL			m_iconDisplay;
	CLabelGL			m_iconDisplayFocus;
	CLabelGL			m_iconRadio;
	CLabelGL			m_iconRadioFocus;
	CLabelGL			m_iconBT;
	CLabelGL			m_iconBTFocus;
	CLabelGL			m_iconAbout;
	CLabelGL			m_iconAboutFocus;
    CLabelGL            m_iconAdvance;
    CLabelGL            m_iconAdvanceFocus;

	CListBoxGL		m_boxMenu;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL		m_btnMenuCover;
#endif


};

