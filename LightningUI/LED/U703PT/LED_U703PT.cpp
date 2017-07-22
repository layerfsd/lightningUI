#include "LED_U703PT.h"
#include "..\LEDCommon.h"
#include "..\LEDEffect.h"

#if CVTE_EN_LED

#define U703PT_LED_INSTANCE_NUM		0

CLEDU703PT::CLEDU703PT(BYTE Red, BYTE Green, BYTE Blue, BYTE Brightness, BOOL bLedAnimEffect, BOOL bActivateLed)
{
    LEDRGB ledrgb = {Red, Green, Blue};
	SetLedRGB(&ledrgb);

	if(Brightness <= 0 || Brightness > 100)
		SetLedBrightness((float)1.0f);
	else
		SetLedBrightness((float)(Brightness/100.0f));

	memset(&m_LedParam, 0, sizeof(LEDGROUPOPERATION));
	m_LedParam.InstanceID = U703PT_LED_INSTANCE_NUM;    //group 1
	m_LedParam.dwLedControlFlag = 0x3FF;	//0xE00;    //0xFFF;//  0x380;//

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
	LedMode.InstanceID = U703PT_LED_INSTANCE_NUM;
	LedMode.Mode = LED_MODE_NORMAL_MIN_CURRENT;

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:GetLedCurrentRGB()->Red[%d] GetLedCurrentRGB()->Green[%d] GetLedCurrentRGB()->Blue[%d] \r\n"),TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LEDSilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}
	if (!SetGroupLed(&m_LedParam))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED_IOCTL_SET_LEDGROUP_PWM quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	SetLedShadeEnable(bLedAnimEffect);
	EnableAnimShade(bLedAnimEffect);

}

CLEDU703PT::~CLEDU703PT(void)
{
    RETAILMSG(DEBUG_LED, (TEXT("ERR:[%s].\r\n"), TEXT(__FUNCTION__)));
	//quit thread:
}

void CLEDU703PT::TemplateMethod_SetLedParam(INT iDev,INT iLedn)
{

}


// void CLEDU705PM::TemplateMethod_LedFlash(BOOL bEnable)
// {
// 
// }

BOOL CLEDU703PT::TemplateMethod_LedRefresh()
{
	m_LedParam.dwLedControlFlag = (GetLedState()==LED_MUTE)?0x2B5:0x3FF;	//0xE00;    //0xFFF;//  0x380;//

    //RETAILMSG(1, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_LedParam.dwLedControlFlag));
	//RETAILMSG(1, (TEXT("MSG:[%s]: RGB[%d,%d,%d].\r\n"), TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	m_LedParam.InstanceID = U703PT_LED_INSTANCE_NUM;
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

void CLEDU703PT::TemplateMethod_LedAnimPowerOn()
{
	SetLedOn(0x3FF,U703PT_LED_INSTANCE_NUM);
	Sleep(1000);
	//Start the led shade animation
	if(GetLedShadeEnable())
	{
		EnableAnimShade(TRUE,TRUE);
	}
}

void CLEDU703PT::TemplateMethod_LedAnimPowerOff()
{
	SetLedShadeOff(0x2B5,U703PT_LED_INSTANCE_NUM,2);
	Sleep(800);
	SetLedShadeOff(0x14A,U703PT_LED_INSTANCE_NUM,2);
}

void CLEDU703PT::TemplateMethod_LedAnimPowerIdle()
{
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	SetLedShadeOff(0x2B5,U703PT_LED_INSTANCE_NUM,2);
	Sleep(500);
	SetLedShadeOff(0x14A,U703PT_LED_INSTANCE_NUM,2);
}

void CLEDU703PT::TemplateMethod_LedAnimBreath()
{
	BYTE uCount = sizeof(GammaBreath);
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: uCount:%d\r\n"), TEXT(__FUNCTION__),uCount));

	//Fix Bug<0002624> IDLE?¨²RearviewStart?¨²RearviewStop?¨²Cancel IDLE?¨²LED all off
	LEDOPERATION LedParam;
	LEDHSB       ledhsb;
	LEDRGB       ledrgb;
	memcpy(&ledhsb, GetLedSaveHSB(), sizeof(ledhsb));
	memset(&ledrgb,0,sizeof(ledrgb));
	memset(&LedParam, 0, sizeof(LedParam));
#if (CVTE_MODEL_TYPE == CVTE_MODEL_TYPE_U703PT_DSP_AK7738)
    LedParam.Ledn = 5;//m_nBreathLedNumber;
#else
	LedParam.Ledn = 4;//m_nBreathLedNumber;
#endif
	LedParam.InstanceID = U703PT_LED_INSTANCE_NUM;
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
		{
			LedParam.Color.Red = 0;      //GammaBreath[i];
			LedParam.Color.Green= 0;   //GammaBreath[i];
			LedParam.Color.Blue = 0;    //GammaBreath[i];
			if(!SetLed(&LedParam))
			{
				RETAILMSG(DEBUG_LED,
					(TEXT("ERR:%s: Instance[%d] error = 0x%x\r\n"),
					LedParam.InstanceID,
					GetLastError()));
			}
			break;	
		}
		Sleep(20);
	}
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
}

void CLEDU703PT::TemplateMethod_LedAnimShade()
{
	LedShadeRun();
}

void CLEDU703PT::TemplateMethod_LedAnimFlash()
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
		ledParam.InstanceID = U703PT_LED_INSTANCE_NUM;

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

void CLEDU703PT::TemplateMethod_LedMute()
{
	SetLedShadeOff(0x14A,U703PT_LED_INSTANCE_NUM,1);
}

void CLEDU703PT::TemplateMethod_LedUnmute()
{
	SetLedShadeOn(0x14A,U703PT_LED_INSTANCE_NUM,1);
}
#endif
