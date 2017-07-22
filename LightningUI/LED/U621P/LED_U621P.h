#pragma once

#include "../stdafx.h"
#include <windev.h>
#include "../LEDBase.h"

#if CVTE_EN_LED

class CLEDU621P
    : public CLEDBase
{
public:
	CLEDU621P(BYTE Red = 255, BYTE Green = 255, BYTE Blue = 255,  BYTE Brightness = 100, BOOL bLedAnimEffect = FALSE, BOOL bActivateLed = TRUE);
	~CLEDU621P(void);

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
	void OnLedPowerOn();
    static DWORD LED1SilderProcessThread(LPVOID lpParam);
    static DWORD LED2SilderProcessThread(LPVOID lpParam);
    void Led1SilderProcessRun();
    void Led2SilderProcessRun();

private:
	HANDLE	m_hLED1SilderProcessThead;
    HANDLE	m_hLED2SilderProcessThead;
	HANDLE	m_hLed1SilderProcessEvent;
    HANDLE  m_hLed2SilderProcessEvent;

	BOOL	m_bProcessing1;
    BOOL	m_bProcessing2;
    BOOL    m_bLed1SilderEnable;
    BOOL    m_bLed2SilderEnable;
    BOOL    m_bSilderLed1Shutdown;
    BOOL    m_bSilderLed2Shutdown;

	DWORD   m_dwSetTimeOut1;
	DWORD   m_dwSetTimeOut2;

	int m_iCurPara1;
	int m_iSetPara1;
	int m_iCurPara2;
	int m_iSetPara2;

	LEDGROUPOPERATION m_Led1Param;
    LEDGROUPOPERATION m_Led2Param;

	BYTE	m_bLED1TouchFlag[32];
	BYTE	m_bLED2TouchFlag[32];

	BOOL	m_bLED1AniReady;
    BOOL	m_bLED2AniReady;

	BOOL	m_bNeedRecover1;
    BOOL	m_bNeedRecover2;

	DWORD   m_dwSetTick1;
    DWORD   m_dwSetTick2;
};

#endif
