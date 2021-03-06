#include "LED_DS701P.h"
#include "..\LEDCommon.h"
#include "..\LEDEffect.h"

#if CVTE_EN_LED
#define DS701P_LED_INSTANCE_NUM	0

CLEDDS701P::CLEDDS701P(BYTE Red, BYTE Green, BYTE Blue, BYTE Brightness, BOOL bLedAnimEffect, BOOL bActivateLed)
{
    LEDRGB ledrgb = {Red, Green, Blue};
	SetLedRGB(&ledrgb);

	if(Brightness <= 0 || Brightness > 100)
		SetLedBrightness((float)1.0f);
	else
		SetLedBrightness((float)(Brightness/100.0f));

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:GetLedCurrentRGB()->Red[%d] GetLedCurrentRGB()->Green[%d] GetLedCurrentRGB()->Blue[%d] \r\n"),TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));

	memset(m_bLEDTouchFlag,0,sizeof(m_bLEDTouchFlag));
	m_dwSetTick=0;

	LEDMODE LedMode;
	memset(&LedMode, 0, sizeof(LedMode));
	LedMode.InstanceID = DS701P_LED_INSTANCE_NUM;
	LedMode.Mode = LED_MODE_NORMAL_MIN_CURRENT;

	//RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:GetLedCurrentRGB()->Red[%d] GetLedCurrentRGB()->Green[%d] GetLedCurrentRGB()->Blue[%d] \r\n"),TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LEDSilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	memset(&m_LedParam, 0, sizeof(LEDGROUPOPERATION));
	m_LedParam.InstanceID = DS701P_LED_INSTANCE_NUM;    //group 1
	m_LedParam.dwLedControlFlag = 0xFFF;	//0xE00;    //0x3F;//  0x380;//

	if(bActivateLed)
	{
		for(int i=0;i<LED_NUMBER;i++)
		{
			m_LedParam.Color[i].Red = GetLedCurrentRGB()->Red;
			m_LedParam.Color[i].Green= GetLedCurrentRGB()->Green;
			m_LedParam.Color[i].Blue = GetLedCurrentRGB()->Blue;
		}	
	}
	else
	{
		for(int i=0;i<LED_NUMBER;i++)
		{
			m_LedParam.Color[i].Red = 0;
			m_LedParam.Color[i].Green= 0;
			m_LedParam.Color[i].Blue = 0;
		}	
	}

	if (!SetGroupLed(&m_LedParam))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED_IOCTL_SET_LEDGROUP_PWM quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	SetLedShadeEnable(bLedAnimEffect);
	EnableAnimShade(bLedAnimEffect);

}

CLEDDS701P::~CLEDDS701P(void)
{
    RETAILMSG(DEBUG_LED, (TEXT("ERR:[%s].\r\n"), TEXT(__FUNCTION__)));
}

void CLEDDS701P::TemplateMethod_SetLedParam(INT iDev,INT iLedn)
{

}

BOOL CLEDDS701P::TemplateMethod_LedRefresh()
{
	m_LedParam.dwLedControlFlag = 0xFFF;	//0xE00;    //0x3F;//  0x380;//

    //RETAILMSG(1, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_LedParam.dwLedControlFlag));
	//RETAILMSG(1, (TEXT("MSG:[%s]: RGB[%d,%d,%d].\r\n"), TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	m_LedParam.InstanceID = DS701P_LED_INSTANCE_NUM;
    for(int i=0;i<LED_NUMBER;i++)
    {
    	m_LedParam.Color[i].Red = GetLedCurrentRGB()->Red;
        m_LedParam.Color[i].Green= GetLedCurrentRGB()->Green;
        m_LedParam.Color[i].Blue = GetLedCurrentRGB()->Blue;
    }


	if (!SetGroupLed(&m_LedParam))
	{
		RETAILMSG(DEBUG_LED,
			(TEXT("ERR:[%s]: DeviceIoControl[LED_IOCTL_SET_ALLLED_PWM] Instance[%d]failed. error = 0x%x\r\n"),
			TEXT(__FUNCTION__),
			m_LedParam.InstanceID,
			GetLastError()));
        return FALSE;

	}
    return TRUE;
}

void CLEDDS701P::TemplateMethod_LedAnimPowerOn()
{

}

void CLEDDS701P::TemplateMethod_LedAnimPowerOff()
{

}

void CLEDDS701P::TemplateMethod_LedAnimPowerIdle()
{

}

void CLEDDS701P::TemplateMethod_LedAnimBreath()
{

}

void CLEDDS701P::TemplateMethod_LedAnimShade()
{
	LedShadeRun();
}

void CLEDDS701P::TemplateMethod_LedAnimFlash()
{
	BYTE uCount = sizeof(GammaFlash);
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: ++ dwControlFlag[%X]\r\n"), TEXT(__FUNCTION__),m_dwLedFlashRunControlFlag));

	LEDGROUPOPERATION ledParam;
	LEDHSB ledHsb;
	LEDRGB ledRgb;
	memcpy(&ledParam, &m_LedParam, sizeof(m_LedParam));
	memcpy(&ledHsb, GetLedCurrentHSB(), sizeof(LEDHSB));
	memset(&ledRgb, 0, sizeof(LEDRGB));

	ledParam.dwLedControlFlag = m_dwLedFlashRunControlFlag;

	for(int i=0;i<uCount;i++)
	{
		ledHsb.Brightness = GammaFlash[i]/255.0f;
		HSB2RGB(&ledHsb, &ledRgb);
		ledParam.InstanceID = DS701P_LED_INSTANCE_NUM;

		for(int i=0;i<LED_NUMBER;i++)
		{
			ledParam.Color[i].Red = ledRgb.Red;
			ledParam.Color[i].Green= ledRgb.Green;
			ledParam.Color[i].Blue = ledRgb.Blue;
		}

		if (!SetGroupLed(&ledParam))
		{
			RETAILMSG(DEBUG_LED,(TEXT("ERR:[%s]: SetGroupLed failed. error = 0x%x\r\n"),TEXT(__FUNCTION__),GetLastError()));
			return;
		}
		Sleep(5);
	}
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
}

void CLEDDS701P::TemplateMethod_LedMute()
{

}

void CLEDDS701P::TemplateMethod_LedUnmute()
{

}


#endif
