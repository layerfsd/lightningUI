#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#if CVTE_EN_LED
#include "../../LED/LED.h"
#endif



class CPanelSettingDisplay :
	public CPanelGL
{
public:
	CPanelSettingDisplay(void);
	~CPanelSettingDisplay(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
    void LinkLED(CLED *phLED){m_pLED = phLED;}
    void FreshLedSilder();
    void EnableLedAnimEffect(BOOL bEnable);
#endif
    void FreshLCDBrightnessSilder();
    void OnHeadlightOn(BOOL bOn, UINT nBk);
	void OnMCUCmd(WPARAM wParam, LPARAM lParam);


protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void SetGamma(DOUBLE iSet);
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
    void SetLedRGB(LEDRGB * pLedColor);
    void SetLedHue(float fHue);
    void SetLedDayModeBrightness(UINT nSet);
	void SetLedNightModeBrightness(UINT nSet);
#endif
    void SetLCDDayModeBrightness(UINT nBrightness);
	void SetLCDNightModeBrightness(UINT nBrightness);

private:
    BOOL        m_bColorPaletteShown;
	BOOL		m_bNightMode;

	CButtonGL   m_tagTitleScreen;
	CButtonGL   m_tagPanelScreen;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CButtonGL	m_tagTitleLampControl;
	CSliderGL	m_sliLampControl;
#endif

	CButtonGL	m_tagTitleBlurEffect;
	CSliderGL	m_sliBlurEffect;
	CLabelGL	m_iconLCDGamma;
	CButtonGL	m_tagTitleGamma;
	CSliderGL	m_sliGamma;
	CButtonGL	m_tagTitleLCDBrightness;
	CSliderGL	m_sliLCDBrightness;
	CLabelGL	m_iconLCDDaymode;

	CButtonGL	m_tagTitleLCDBrightnessnight;
	CSliderGL	m_sliLCDBrightnessnight;
	CLabelGL	m_iconLCDNightmode;

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
	CLED        *m_pLED;

	CButtonGL   m_tagTitleLED;
    CButtonGL   m_tagPanelLight;
    CButtonGL   m_tagTitlePanelLight;
    //CButtonGL   m_tagTitlePanelLight2;
    CButtonGL   m_btnLEDColor1;
    CButtonGL   m_btnLEDColor2;
    CButtonGL   m_btnLEDColor3;
    CButtonGL   m_btnLEDColor4;
    CButtonGL   m_btnLEDColor5;
    CButtonGL   m_btnLEDColorUser;
    CButtonGL   m_btnLEDColorCustom;

    CLabelGL    m_labLEDCurrentColor;
    CButtonGL   m_tagTitleLEDColor;
    CLabelGL    m_tagLEDColor;
    CSliderGL   m_sliLEDColor;
	CLabelGL	m_iconLEDColor;

    CButtonGL   m_tagTitleLEDBrightness;
    CLabelGL	m_tagLEDBrightness;
    CSliderGL   m_sliLEDBrightness;
	CLabelGL	m_iconLEDDaymode;

	CButtonGL   m_tagTitleLEDBrightnessnight;
	CLabelGL	m_tagLEDBrightnessnight;
	CSliderGL   m_sliLEDBrightnessnight;
	CLabelGL	m_iconLEDNightmode;

	CButtonGL	m_tagTitleLedAnimationEffect;
	CSliderGL	m_sliLedAnimationEffect;
#endif

	CButtonGL   m_tagTitleReset;
	CButtonGL	m_btnReset;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif

};

