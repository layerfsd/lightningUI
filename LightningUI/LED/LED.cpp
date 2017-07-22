//#include "Platform_macro.h"

#include "LED.h"
#if CVTE_EN_LED

CLED::CLED(BYTE Red, BYTE Green, BYTE Blue, BYTE Brightness, BOOL bLedAnimEffect, BOOL bActivateLed)
{
	RETAILMSG(1, (TEXT("MSG:[%s]:++ [%d,%d,%d] brightness[%d]\r\n"),TEXT(__FUNCTION__),Red,Green,Blue,Brightness));
	m_pLEDInstance = NULL;

#if (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U701P)
	m_pLEDInstance = new CLEDU701P(Red, Green, Blue, Brightness, bLedAnimEffect, bActivateLed);
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U621P)
	m_pLEDInstance = new CLEDU621P(Red, Green, Blue, Brightness, bLedAnimEffect, bActivateLed);
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U702P)
	m_pLEDInstance = new CLEDU702P(Red, Green, Blue, Brightness, bLedAnimEffect, bActivateLed);
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U703PT)
	m_pLEDInstance = new CLEDU703PT(Red, Green, Blue, Brightness, bLedAnimEffect, bActivateLed);
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_U705PM)
	m_pLEDInstance = new CLEDU705PM(Red, Green, Blue, Brightness, bLedAnimEffect, bActivateLed);
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_DS702P)
	m_pLEDInstance = new CLEDDS702P(Red, Green, Blue, Brightness, bLedAnimEffect, bActivateLed);
#elif (CVTE_DEF_LED_TYPE == CVTE_LED_TYPE_DS701P)
	m_pLEDInstance = new CLEDDS701P(Red, Green, Blue, Brightness, bLedAnimEffect, bActivateLed);
#else
	RETAILMSG(1, (TEXT("MSG:[%s]:++ CVTE_DEF_LED_TYPE define err\r\n"),TEXT(__FUNCTION__)));
    m_pLEDInstance = NULL;
#endif
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:-- \r\n"),TEXT(__FUNCTION__)));
}

CLED::~CLED(void)
{
    RETAILMSG(DEBUG_LED, (TEXT("ERR:[%s].\r\n"), TEXT(__FUNCTION__)));
	//quit thread:
	if(m_pLEDInstance)
	{
		delete m_pLEDInstance;
		m_pLEDInstance = NULL;
	}
}

void CLED::SetLedParam(INT iDev,INT iLedn)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->TemplateMethod_SetLedParam(iDev, iLedn);
	}
}

void CLED::EnableAnimShade(BOOL bEnable, BOOL bForce)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->SetLedShadeEnable(bEnable);
		m_pLEDInstance->EnableAnimShade(bEnable,bForce);
	}
}

void CLED::GetLedHSB(LEDHSB* pLEDHSB)
{
	if(m_pLEDInstance)
	{
		memcpy(pLEDHSB, m_pLEDInstance->GetLedCurrentHSB(), sizeof(LEDHSB));
	}
}

void CLED::GetLedRGB(LEDRGB* pLEDRGB)
{
	if(m_pLEDInstance)
	{
		memcpy(pLEDRGB, m_pLEDInstance->GetLedCurrentRGB(), sizeof(LEDRGB));
	}
}

void CLED::SetLedHSB(LEDHSB *pLedHSB)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->SetLedHSB(pLedHSB);
	}
}

void CLED::SetLedRGB(LEDRGB *pLedRGB)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->SetLedRGB(pLedRGB);
	}
}

void CLED::SetLedHue(float fHue)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->SetLedHue(fHue);
	}
}

void CLED::SetLedState(BYTE nLedState)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->SetLedState(nLedState);
	}
}

void CLED::SetLedBrightness(float fBrightness)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->SetLedBrightness(fBrightness);
	}
}

void CLED::SetLedFlash(BOOL bEnable)
{
	if(m_pLEDInstance)
	{
		m_pLEDInstance->EnableAnimFlash(bEnable);
	}
}

BOOL CLED::RefreshLED()
{
	if(m_pLEDInstance)
	{
		return m_pLEDInstance->TemplateMethod_LedRefresh();
	}
	else
	{
		return FALSE;
	}
}

#endif
