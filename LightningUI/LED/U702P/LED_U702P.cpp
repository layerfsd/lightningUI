#include "LED_U702P.h"
#include "..\LEDCommon.h"
#include "..\LEDEffect.h"

#if CVTE_EN_LED


CLEDU702P::CLEDU702P(BYTE Red, BYTE Green, BYTE Blue, BYTE Brightness, BOOL bLedAnimEffect, BOOL bActivateLed)
{
	LEDRGB ledrgb = {Red, Green, Blue};
	SetLedRGB(&ledrgb);

	//U702P reduce the brightness
	if(Brightness > 30)
		Brightness = 30;
	else if (Brightness < 5)
		Brightness = 5;
	SetLedBrightness((float)(Brightness/100.0f));

	memset(&m_LedParam, 0, sizeof(LEDGROUPOPERATION));
	m_LedParam.InstanceID = 0;    //group 1
	m_LedParam.dwLedControlFlag = 0x01F;	//0xE00;    //0xFFF;//  0x380;//
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

	memset(m_bLEDTouchFlag,0,sizeof(m_bLEDTouchFlag));
	m_dwSetTick=0;

	LEDMODE LedMode;
	memset(&LedMode, 0, sizeof(LedMode));
	LedMode.InstanceID = 0;
	LedMode.Mode = LED_MODE_NORMAL_MIN_CURRENT;

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:GetLedCurrentRGB()->Red[%d] GetLedCurrentRGB()->Green[%d] GetLedCurrentRGB()->Blue[%d] \r\n"),TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LEDSilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	if (!SetGroupLed(&m_LedParam))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LEDSilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	SetLedShadeEnable(bLedAnimEffect);
	EnableAnimShade(bLedAnimEffect);
}

CLEDU702P::~CLEDU702P(void)
{
    RETAILMSG(DEBUG_LED, (TEXT("ERR:[%s].\r\n"), TEXT(__FUNCTION__)));
	//quit thread:
}

void CLEDU702P::TemplateMethod_SetLedParam(INT iDev,INT iLedn)
{

}

//void CLEDU702P::TemplateMethod_LedFlash(BOOL bEnable)
//{
//
//}

BOOL CLEDU702P::TemplateMethod_LedRefresh()
{
    m_LedParam.dwLedControlFlag = 0x01F; //(0xFFF & (0xFFF << m_iSetPara));
    //RETAILMSG(1, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_LedParam.dwLedControlFlag));
	//RETAILMSG(1, (TEXT("MSG:[%s]: RGB[%d,%d,%d].\r\n"), TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	m_LedParam.InstanceID = 0;
    for(int i=0;i<LED_NUMBER;i++)
    {
    	if(i>=m_iSetPara)
    	{
        	m_LedParam.Color[i].Red = GetLedCurrentRGB()->Red;
            m_LedParam.Color[i].Green= GetLedCurrentRGB()->Green;
            m_LedParam.Color[i].Blue = GetLedCurrentRGB()->Blue;
    	}
        else
        {
        	m_LedParam.Color[i].Red = 0;
            m_LedParam.Color[i].Green= 0;
            m_LedParam.Color[i].Blue = 0;
            //RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));

        }
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

void CLEDU702P::TemplateMethod_LedAnimPowerOn()
{
	OnLedPowerOn();
	Sleep(1000);
	//Start the led shade animation
	if(GetLedShadeEnable())
	{
		EnableAnimShade(TRUE,TRUE);
	}
}

void CLEDU702P::TemplateMethod_LedAnimPowerOff()
{
	Sleep(800);
	OnLedPowerIdle();
}

void CLEDU702P::TemplateMethod_LedAnimPowerIdle()
{
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	OnLedPowerIdle();
}

void CLEDU702P::TemplateMethod_LedAnimBreath()
{
	BYTE uCount = sizeof(GammaBreath);
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: uCount:%d\r\n"), TEXT(__FUNCTION__),uCount));

	//Fix Bug<0002624> IDLE¡úRearviewStart¡úRearviewStop¡úCancel IDLE¡úLED all off
	LEDOPERATION LedParam;
	LEDHSB       ledhsb;
	LEDRGB       ledrgb;
	memcpy(&ledhsb, GetLedSaveHSB(), sizeof(ledhsb));
	memset(&ledrgb,0,sizeof(ledrgb));
	memset(&LedParam, 0, sizeof(LedParam));
	LedParam.Ledn = 4;//m_nBreathLedNumber;
	LedParam.InstanceID = 0;
	//printf("HSB[%.2f,%.2f,%.2f]\r\n", ledhsb.Hue,ledhsb.Saturation,ledhsb.Brightness);
	for(int i=0;i<uCount;i++)
	{
		ledhsb.Brightness = (GammaBreath[i]/255.0f)*(GetLedSaveHSB()->Brightness/1.0f);
		HSB2RGB(&ledhsb, &ledrgb);
		LedParam.Color.Red = ledrgb.Red;      //GammaBreath[i];
		LedParam.Color.Green= ledrgb.Green;   //GammaBreath[i];
		LedParam.Color.Blue = ledrgb.Blue;    //GammaBreath[i];
		if(!SetLed(&LedParam))
		{
			RETAILMSG(DEBUG_LED,
				(TEXT("ERR:%s: Instance[%d] error = 0x%x\r\n"),
				LedParam.InstanceID,
				GetLastError()));
		}

		if(!GetLedBreathEnable())
			break;
		Sleep(20);
	}
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
}

void CLEDU702P::TemplateMethod_LedAnimShade()
{
	LedShadeRun();
}

void CLEDU702P::TemplateMethod_LedAnimFlash()
{

}

void CLEDU702P::TemplateMethod_LedMute()
{

}

void CLEDU702P::TemplateMethod_LedUnmute()
{

}

void CLEDU702P::OnLedPowerOn()
{
    m_LedParam.dwLedControlFlag=0x01F; //0x0E00;

    for(int i=0;i<LED_NUMBER;i++)
    {
    	m_LedParam.Color[i].Red = GetLedCurrentRGB()->Red;
        m_LedParam.Color[i].Green= GetLedCurrentRGB()->Green;
        m_LedParam.Color[i].Blue = GetLedCurrentRGB()->Blue;
    }

	if(!SetGroupLed(&m_LedParam))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]: LED_POWERON failed.\r\n"),TEXT(__FUNCTION__)));
    }
}

void CLEDU702P::OnLedPowerIdle()
{
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));

    m_LedParam.dwLedControlFlag=0x01F; //0x0E00;

	for(int i=0;i<LED_NUMBER;i++)
    {
    	m_LedParam.Color[i].Red = 0;
        m_LedParam.Color[i].Green= 0;
        m_LedParam.Color[i].Blue = 0;
    }

	if (SetGroupLed(&m_LedParam))
	{
        //m_nLedState = LED_NORMAL;
	}
    else
    {
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]: LED_POWERON failed.\r\n"),TEXT(__FUNCTION__)));
    }
}

#endif
