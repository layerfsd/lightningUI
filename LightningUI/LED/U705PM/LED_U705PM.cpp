#include "LED_U705PM.h"
#include "..\LEDCommon.h"
#include "..\LEDEffect.h"

#if CVTE_EN_LED

#define U705PM_LED_INSTANCE_NUM	0

CLEDU705PM::CLEDU705PM(BYTE Red, BYTE Green, BYTE Blue, BYTE Brightness, BOOL bLedAnimEffect, BOOL bActivateLed)
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
	LedMode.InstanceID = U705PM_LED_INSTANCE_NUM;
	LedMode.Mode = LED_MODE_NORMAL_MIN_CURRENT;

	//RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:GetLedCurrentRGB()->Red[%d] GetLedCurrentRGB()->Green[%d] GetLedCurrentRGB()->Blue[%d] \r\n"),TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LEDSilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	memset(&m_LedParam, 0, sizeof(LEDGROUPOPERATION));
	m_LedParam.InstanceID = U705PM_LED_INSTANCE_NUM;    //group 1
	m_LedParam.dwLedControlFlag = 0x3FF;	//0xE00;    //0xFFF;//  0x380;//

	if(bActivateLed)
	{
		for(int i=0;i<LED_NUMBER;i++)
		{
			m_LedParam.Color[i].Red = 0;	//GetLedCurrentRGB()->Red;
			m_LedParam.Color[i].Green= 0;	//GetLedCurrentRGB()->Green;
			m_LedParam.Color[i].Blue = 0;	//GetLedCurrentRGB()->Blue;
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

CLEDU705PM::~CLEDU705PM(void)
{
    RETAILMSG(DEBUG_LED, (TEXT("ERR:[%s].\r\n"), TEXT(__FUNCTION__)));
	//quit thread:
}

void CLEDU705PM::TemplateMethod_SetLedParam(INT iDev,INT iLedn)
{

}


// void CLEDU705PM::TemplateMethod_LedFlash(BOOL bEnable)
// {
// 
// }

BOOL CLEDU705PM::TemplateMethod_LedRefresh()
{
	m_LedParam.dwLedControlFlag = (GetLedState()==LED_MUTE)?0x2B5:0x3FF;	//0xE00;    //0xFFF;//  0x380;//

    //RETAILMSG(1, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_LedParam.dwLedControlFlag));
	//RETAILMSG(1, (TEXT("MSG:[%s]: RGB[%d,%d,%d].\r\n"), TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));
	m_LedParam.InstanceID = U705PM_LED_INSTANCE_NUM;
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

void CLEDU705PM::TemplateMethod_LedAnimPowerOn()
{
	SetLedShadeOn(0x14A,U705PM_LED_INSTANCE_NUM,2);
	Sleep(500);
	SetLedShadeOn(0x2B5,U705PM_LED_INSTANCE_NUM,2);
	//Start the led shade animation
	if(GetLedShadeEnable())
	{
		EnableAnimShade(TRUE,TRUE);
	}
}

void CLEDU705PM::TemplateMethod_LedAnimPowerOff()
{
	SetLedShadeOff(0x2B5,U705PM_LED_INSTANCE_NUM,2);
	Sleep(800);
	SetLedShadeOff(0x14A,U705PM_LED_INSTANCE_NUM,2);
}

void CLEDU705PM::TemplateMethod_LedAnimPowerIdle()
{
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	SetLedShadeOff(0x2B5,U705PM_LED_INSTANCE_NUM,2);
	Sleep(500);
	SetLedShadeOff(0x14A,U705PM_LED_INSTANCE_NUM,2);
}

void CLEDU705PM::TemplateMethod_LedAnimBreath()
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
	LedParam.Ledn = 0;//m_nBreathLedNumber;
	LedParam.InstanceID = U705PM_LED_INSTANCE_NUM;
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

void CLEDU705PM::TemplateMethod_LedAnimShade()
{
	LedShadeRun();
}

void CLEDU705PM::TemplateMethod_LedAnimFlash()
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
		ledParam.InstanceID = U705PM_LED_INSTANCE_NUM;

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

void CLEDU705PM::TemplateMethod_LedMute()
{
	SetLedShadeOff(0x14A,U705PM_LED_INSTANCE_NUM,1);
}

void CLEDU705PM::TemplateMethod_LedUnmute()
{
	SetLedShadeOn(0x14A,U705PM_LED_INSTANCE_NUM,1);
}

#endif
