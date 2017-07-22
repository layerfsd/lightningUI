#pragma once

#include "LEDCommon.h"
#if (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U701P)
#include ".\U701P\LED_U701P.h"
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U621P)
#include ".\U621P\LED_U621P.h"
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U702P)
#include ".\U702P\LED_U702P.h"
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U703PT)
#include ".\U703PT\LED_U703PT.h"
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U705PM)
#include ".\U705PM\LED_U705PM.h"
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_DS702P)
#include ".\DS702P\LED_DS702P.h"
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_DS701P)
#include ".\DS701P\LED_DS701P.h"
#else
#error "CVTE_DEF_LED_TYPE is out of range."
#endif
#include "LEDEffect.h"

#if CVTE_EN_LED
class CLED
{
public:
	CLED(BYTE Red = 255, BYTE Green = 255, BYTE Blue = 255,  BYTE Brightness = 100, BOOL bLedAnimEffect = FALSE, BOOL bActivateLed = TRUE);
	~CLED(void);

public:
    void SetLedParam(INT iDev,INT iLedn);
	void SetLedState(BYTE nLedState);   //{m_nPowerState = nPowerState;};
    void GetLedHSB(LEDHSB* pLEDHSB);
    void SetLedHSB(LEDHSB *pLedHSB);
    void GetLedRGB(LEDRGB* pLEDRGB);
    void SetLedRGB(LEDRGB *pLedRGB);
    void SetLedHue(float fHue);
    void SetLedBrightness(float fBrightness);
    BOOL RefreshLED();
    void SetLedFlash(BOOL bEnable);
    void EnableAnimShade(BOOL bEnable, BOOL bForce = FALSE);

private:
    CLEDBase *m_pLEDInstance;

private:

};

#endif
