#pragma once

#include "../stdafx.h"
#include <windev.h>
#include "../LEDBase.h"

#if CVTE_EN_LED

class CLEDU705PM
    : public CLEDBase
{
public:
	CLEDU705PM(BYTE Red = 255, BYTE Green = 255, BYTE Blue = 255,  BYTE Brightness = 100, BOOL bLedAnimEffect = FALSE, BOOL bActivateLed = TRUE);
	~CLEDU705PM(void);

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
	DWORD   m_dwSetTimeOut;

	int m_iCurPara;
	int m_iSetPara;

	LEDGROUPOPERATION m_LedParam;
	BYTE	m_bLEDTouchFlag[32];
	DWORD   m_dwSetTick;
	DWORD   m_dwLedFlashRunControlFlag;
};

#endif
