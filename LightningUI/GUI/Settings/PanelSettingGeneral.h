#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#include <GpsAPI.h>
#include "../../VIP/CVideoWindow.h"
#include "PanelSettingBackground.h"

class CPanelSettingGeneral :
	public CPanelGL
{
public:
	CPanelSettingGeneral(void);
	~CPanelSettingGeneral(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);
    void OnBackgroundChanged(WPARAM wParam, LPARAM lParam);
    void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
    void OnDeviceRemovedReady(LPCTSTR lpDeviceName);
    void OnRearColorSignalChange(UINT uColorSignal);
	void OnDSTChanged();
    void LinkPages(void *pLibrary);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	BOOL ConfigNaviSpeech(NAVI_SPEECH_CONFIG config);

	void RestoreFactorySetting();
	BOOL RestoreTimeZone(void);
	BOOL RestoreBTReg(void);
	BOOL RestoreRegSetting();

	//
	void ConfigBrakeTest(BOOL bOn);
	void ConfigReversePolar(BOOL bHigh);
	void ConfigLampControl(BOOL bOn);
//		void ConfigKeyBeep(BOOL bOn);
    void ConfigBacklightControl(BOOL bOn);
#if CVTE_EN_CDP_DCP_CONTROL
	void SwitchDCP(BOOL bEnable);
#endif
private:
	void ClearFilesInFolder(LPCTSTR foder);

private:

    CPanelSettingBackground   *m_ppSettingBackground;

	CButtonGL	m_tagTitleBrakeTest;
	CButtonGL	m_tagTitleReversePolar;
	CButtonGL	m_tagTitleLampControl;
//		CButtonGL	m_tagTitleKeyBeep;

	CButtonGL	m_btnDriveType;
	CButtonGL	m_tagValueDriveType;

    CButtonGL	m_btnRearColorSignalType;
	CButtonGL	m_tagRearColorSignalType;

	CButtonGL	m_btnIdleScreen;
	CButtonGL	m_tagValueIdleScreen;

// 	CButtonGL	m_btnCameraType;
// 	CButtonGL	m_tagValueCameraType;

	CSliderGL		m_sliBrakeTest;
	CSliderGL		m_sliReversePolar;
	CSliderGL		m_sliLampControl;
//		CSliderGL		m_sliKeyBeep;

	CButtonGL	m_tagTitleTimeAndDate;

	CButtonGL	m_btnDate;
	CButtonGL	m_btnTime;
	CButtonGL	m_btnTimeZone;

	CButtonGL	m_tagTitleTime24HMode;
	CButtonGL	m_tagTitleTimeAutoAdjustDLST;
	CSliderGL	m_sliTime24HMode;
	CSliderGL	m_sliTimeAutoAdjustDLST;
	CButtonGL	m_tagValueTime;
	CButtonGL	m_tagValueDate;
#if CVTE_EN_NAVI
	CButtonGL	m_tagTitleTimeAutoAdjust;
	CSliderGL	m_sliTimeAutoAdjust;
#endif

	//
	CButtonGL	m_tagTitleNavi;
	CButtonGL	m_btnNaviPath;
	CButtonGL	m_btnNaviSpeech;

	CButtonGL	m_tagValueNaviPath;
	CButtonGL	m_tagValueNaviSpeech;

	//language
	CButtonGL	m_tagTitleLanguage;
	CButtonGL	m_btnLanguageSet;

#if CVTE_EN_COMBINE_ANDROID_LINK
	//Android Type
	CButtonGL	m_tagTitleAndroidType;
	CButtonGL	m_btnAndroidTypeSet;
#endif

#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	//skin
	CButtonGL	m_tagTitleSkin;
	CButtonGL	m_btnSkinPath;
	CButtonGL	m_tagValueSkinPath;
	CButtonGL	m_btnBackground;
	CButtonGL	m_tagValueBackground;
	CButtonGL	m_tagTitleBkLumin;
	CSliderGL		m_sliBkLumin;
	CButtonGL	m_tagTitleBlurEffect;
	CSliderGL		m_sliBlurEffect;
#endif

    //Advance settings
    CButtonGL   m_tagTitleAdvance;
    CButtonGL   m_btnAdvanceSettings;

    //Advance settings
    CButtonGL   m_tagTitleGesture;
    CButtonGL   m_btnGestureSettings;
	
#if (CUSTOMER_ID == CUSTOMER_BOSSAUDIO) || (CUSTOMER_ID == CUSTOMER_PLANETAUDIO)
	//rear view delay settings
	CButtonGL   m_tagTitleRearViewDelay;
	CButtonGL   m_btnRearViewDelay;
	CButtonGL	m_tagValueRearViewDelay;
	//power off delay settings
	CButtonGL   m_tagTitlePowerOffDelay;
	//CButtonGL   m_btnPowerOffDelay;
	//CButtonGL	m_tagValuePowerOffDelay;
	CButtonGL   m_tagPowerOffDelay;
	CSliderGL	m_sliPowerOffDelay;
#endif

    //factory settings
    CButtonGL   m_tagTitleFactory;
    CButtonGL   m_btnRestoreFactorySettings;
	//CButtonGL   m_btnAdvancedSettings;

#if CVTE_EN_ANDROID_AUTO
	CButtonGL   m_tagTitleAAP;
	CButtonGL   m_tagAAPAutoLaunch;
	CSliderGL		m_sliAAPAutoLaunch;
#endif

#if CVTE_EN_CDP_DCP_CONTROL
	CButtonGL	m_tagTitleDCPControl;
	CSliderGL	m_sliDCPControl;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif

};

