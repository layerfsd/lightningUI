#pragma once

#include "../stdafx.h"
#include <windev.h>
#include "../LEDBase.h"

#if CVTE_EN_LED

class CLEDU701P
    : public CLEDBase
{
public:
	CLEDU701P(BYTE Red = 255, BYTE Green = 255, BYTE Blue = 255,  BYTE Brightness = 100, BOOL bLedAnimEffect = FALSE, BOOL bActivateLed = TRUE);
	~CLEDU701P(void);

public:
	void TemplateMethod_SetLedParam(INT iDev,INT iLedn);
	BOOL TemplateMethod_LedRefresh();
	void TemplateMethod_LedAnimBreath();
	void TemplateMethod_LedAnimShade();
	void TemplateMethod_LedAnimFlash();
	void TemplateMethod_LedAnimPowerOn();
	void TemplateMethod_LedAnimPowerOff();
	void TemplateMethod_LedAnimPowerIdle();
	void TemplateMethod_LedMute();
	void TemplateMethod_LedUnmute();
private:
    static DWORD LedSilderProcessThread(LPVOID lpParam);
    void LedSilderProcessRun();

private:
	HANDLE	m_hLedSilderProcessThead;
	HANDLE	m_hLedSilderProcessEvent;

	BOOL	m_bProcessing;
    BOOL    m_bLedSilderEnable;
    BOOL    m_bSilderLedShutdown;

	DWORD   m_dwSetTimeOut;

	int m_iCurPara;
	int m_iSetPara;

	LEDGROUPOPERATION m_LedParam;
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	LEDGROUPOPERATION m_Led2Param;
#endif
	BYTE	m_bLEDTouchFlag[32];
	BOOL	m_bLEDAniReady;
	BOOL	m_bNeedRecover;
	BOOL    m_bSiriEnable;

	DWORD   m_dwSetTick;
};

#endif
