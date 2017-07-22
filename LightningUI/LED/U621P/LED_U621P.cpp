#include "LED_U621P.h"
#include "..\LEDCommon.h"
#include "..\LEDEffect.h"

#if CVTE_EN_LED

static BOOL g_bActivateShade = FALSE;
#define U621P_LED_INSTANCE1_NUM		0
#define U621P_LED_INSTANCE2_NUM		1

CLEDU621P::CLEDU621P(BYTE Red, BYTE Green, BYTE Blue, BYTE Brightness, BOOL bLedAnimEffect, BOOL bActivateLed)
{
	m_hLed1SilderProcessEvent= CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hLed2SilderProcessEvent= CreateEvent(NULL,FALSE,FALSE,NULL);
	m_bProcessing1=TRUE;
	m_bProcessing2=TRUE;
    m_bSilderLed1Shutdown = FALSE;
	m_bSilderLed2Shutdown = FALSE;
	m_dwSetTimeOut1 = LED_IDLE_FRAME_TIME;
	m_dwSetTimeOut2 = LED_IDLE_FRAME_TIME;
	m_iCurPara1=7;
	m_iSetPara1=0;
	m_iCurPara2=7;
	m_iSetPara2=7;

	LEDRGB ledrgb = {Red, Green, Blue};
	SetLedRGB(&ledrgb);
	if(Brightness <= 0 || Brightness > 100)
		SetLedBrightness((float)1.0f);
	else
		SetLedBrightness((float)(Brightness/100.0f));

    m_bLed1SilderEnable = GetLedCurrentRGB()->Red>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Green>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Blue>LED_SILDER_ENABLE_MIN_STEP?TRUE:FALSE));
    m_bLed2SilderEnable = GetLedCurrentRGB()->Red>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Green>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Blue>LED_SILDER_ENABLE_MIN_STEP?TRUE:FALSE));

	m_bLED1AniReady=TRUE;
    m_bLED2AniReady=TRUE;
	m_bNeedRecover1=FALSE;
    m_bNeedRecover2=FALSE;
	m_dwSetTick1=0;
    m_dwSetTick2=0;
	memset(m_bLED1TouchFlag,0,sizeof(m_bLED1TouchFlag));
	memset(m_bLED2TouchFlag,0,sizeof(m_bLED2TouchFlag));
	memset(&m_Led1Param, 0, sizeof(LEDGROUPOPERATION));
	memset(&m_Led2Param, 0, sizeof(LEDGROUPOPERATION));
	m_Led1Param.InstanceID = U621P_LED_INSTANCE1_NUM;    //group 0
	m_Led1Param.dwLedControlFlag = 0xF80;	//0xFFF;//  0x380;//
	if(bActivateLed)
	{
		for(int i=0;i<LED_NUMBER;i++)
		{
			m_Led1Param.Color[i].Red = GetLedCurrentRGB()->Red;
			m_Led1Param.Color[i].Green= GetLedCurrentRGB()->Green;
			m_Led1Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
		}	
	}
	else
	{
		for(int i=0;i<LED_NUMBER;i++)
		{
			m_Led1Param.Color[i].Red = 0;
			m_Led1Param.Color[i].Green= 0;
			m_Led1Param.Color[i].Blue = 0;
		}	
	}

	LEDMODE LedMode;
	memset(&LedMode, 0, sizeof(LedMode));
	LedMode.InstanceID = U621P_LED_INSTANCE1_NUM;
	LedMode.Mode = LED_MODE_NORMAL_MIN_CURRENT;

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:LED1SilderProcessThread \r\n"),TEXT(__FUNCTION__)));
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:GetLedCurrentRGB()->Red[%d] GetLedCurrentRGB()->Green[%d] GetLedCurrentRGB()->Blue[%d] \r\n"),TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));

	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	if (!SetGroupLed(&m_Led1Param))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	m_Led2Param.InstanceID = U621P_LED_INSTANCE2_NUM;    
	m_Led2Param.dwLedControlFlag = 0x000;	//0xFFF;//  0x380;//

	if(bActivateLed)
	{
		for(int i=0;i<LED_NUMBER;i++)
		{
			m_Led2Param.Color[i].Red = GetLedCurrentRGB()->Red;
			m_Led2Param.Color[i].Green= GetLedCurrentRGB()->Green;
			m_Led2Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
		}	
	}
	else
	{
		for(int i=0;i<LED_NUMBER;i++)
		{
			m_Led2Param.Color[i].Red = 0;
			m_Led2Param.Color[i].Green= 0;
			m_Led2Param.Color[i].Blue = 0;
		}	
	}

    LedMode.InstanceID = U621P_LED_INSTANCE2_NUM;

	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	if (!SetGroupLed(&m_Led2Param))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	m_hLED1SilderProcessThead=CreateThread(NULL,NULL,LED1SilderProcessThread,this,NULL,NULL);
	m_hLED2SilderProcessThead=CreateThread(NULL,NULL,LED2SilderProcessThread,this,NULL,NULL);

	SetLedShadeEnable(bLedAnimEffect);
	EnableAnimShade(bLedAnimEffect);
}

CLEDU621P::~CLEDU621P(void)
{
    RETAILMSG(DEBUG_LED, (TEXT("ERR:[%s].\r\n"), TEXT(__FUNCTION__)));
	//quit thread:
	if(m_hLed1SilderProcessEvent != INVALID_HANDLE_VALUE)
	{
		m_bProcessing1=FALSE;
		SetEvent(m_hLed1SilderProcessEvent);
		CloseHandle(m_hLed1SilderProcessEvent);
		m_hLed1SilderProcessEvent = INVALID_HANDLE_VALUE;
	}

	if(m_hLed2SilderProcessEvent != INVALID_HANDLE_VALUE)
	{
		m_bProcessing2=FALSE;
		SetEvent(m_hLed2SilderProcessEvent);
		CloseHandle(m_hLed2SilderProcessEvent);
		m_hLed2SilderProcessEvent = INVALID_HANDLE_VALUE;
	}


    if(m_hLED1SilderProcessThead != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hLED1SilderProcessThead);
        m_hLED1SilderProcessThead = INVALID_HANDLE_VALUE;
    }

    if(m_hLED2SilderProcessThead != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hLED2SilderProcessThead);
        m_hLED2SilderProcessThead = INVALID_HANDLE_VALUE;
    }
}

void CLEDU621P::TemplateMethod_SetLedParam(INT iDev,INT iLedn)
{
	if(iLedn<0 || iLedn>LED_NUMBER)
	{
    	RETAILMSG(DEBUG_LED,(_T("ERR:[%s]: Param check failed.\r\n"),
            TEXT(__FUNCTION__)));
		return;
	}
	//RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:iDev[%d] iLedn[%d]\r\n"),TEXT(__FUNCTION__),iDev,iLedn));

	if(iDev==0)
	{
        if((m_iSetPara1!=7-iLedn))
		{
			if((m_dwSetTimeOut1==LED_IDLE_FRAME_TIME || m_bLED1AniReady )&& m_bLed1SilderEnable)
			{
				memset(m_bLED1TouchFlag,0,sizeof(m_bLED1TouchFlag));
				SetEvent(m_hLed1SilderProcessEvent);
			}
			m_dwSetTick1=GetTickCount();
            m_iSetPara1=7-iLedn;
			m_bNeedRecover1=TRUE;
			m_bLED1AniReady=FALSE;
            //RETAILMSG(1, (TEXT("MSG:[%s]: m_iSetPara1[%x].\r\n"), TEXT(__FUNCTION__),m_iSetPara1));
		}
	}
	if(iDev==1)
	{
        if(GetLedState() == LED_MUTE)
        {
            RestoreConfig();
        }
        if((m_iSetPara2!=7-iLedn))
		{
            if((m_dwSetTimeOut2==LED_IDLE_FRAME_TIME || m_bLED2AniReady )&& m_bLed2SilderEnable)
			{
				memset(m_bLED2TouchFlag,0,sizeof(m_bLED2TouchFlag));
				memset(m_Led2Param.Color,0,sizeof(m_Led2Param.Color));
				SetEvent(m_hLed2SilderProcessEvent);
			}

			m_dwSetTick2=GetTickCount();
            m_iSetPara2=7-iLedn;
			m_bNeedRecover2=TRUE;
			m_bLED2AniReady=FALSE;
            //RETAILMSG(1, (TEXT("MSG:[%s]: m_iSetPara2[%x].\r\n"), TEXT(__FUNCTION__),m_iSetPara2));
		}
	}
}

//void CLEDU621P::TemplateMethod_LedFlash(BOOL bEnable)
//{
//
//}

BOOL CLEDU621P::TemplateMethod_LedRefresh()
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

    m_bLed1SilderEnable = GetLedCurrentRGB()->Red>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Green>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Blue>LED_SILDER_ENABLE_MIN_STEP?TRUE:FALSE));
    m_Led1Param.dwLedControlFlag = 0xFFF; //(0xFFF & (0xFFF << m_iSetPara));
	m_Led1Param.InstanceID = U621P_LED_INSTANCE1_NUM;
    if(GetLedState() == LED_MUTE)
    {
        //Change the led
		LEDHSB hsb;
		LEDRGB rgb;
		memset(&rgb,0,sizeof(rgb));
		memcpy(&hsb, GetLedCurrentHSB(), sizeof(LEDHSB));
		hsb.Hue = 0;
		hsb.Saturation = 1.0f;
		HSB2RGB(&hsb,&rgb);

        for(int i=0;i<8;i++)    //Left silder has 8 leds
        {
        	if(i>=m_iSetPara1)
        	{
            	m_Led1Param.Color[i].Red = rgb.Red;
                m_Led1Param.Color[i].Green= rgb.Green;
                m_Led1Param.Color[i].Blue = rgb.Blue;
        	}
            else
            {
            	m_Led1Param.Color[i].Red = 0;
                m_Led1Param.Color[i].Green= 0;
                m_Led1Param.Color[i].Blue = 0;
                //RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));
            }
        }
        RestoreConfig();
        for(int i=8;i<LED_NUMBER;i++)
        {
        	m_Led1Param.Color[i].Red = GetLedCurrentRGB()->Red;
            m_Led1Param.Color[i].Green= GetLedCurrentRGB()->Green;
            m_Led1Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
        }
    }
    else
    {
        for(int i=0;i<LED_NUMBER;i++)
        {
        	if(i>=m_iSetPara1)
        	{
            	m_Led1Param.Color[i].Red = GetLedCurrentRGB()->Red;
                m_Led1Param.Color[i].Green= GetLedCurrentRGB()->Green;
                m_Led1Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
        	}
            else
            {
            	m_Led1Param.Color[i].Red = 0;
                m_Led1Param.Color[i].Green= 0;
                m_Led1Param.Color[i].Blue = 0;
                //RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));
            }
        }
    }


	if (!SetGroupLed(&m_Led1Param))
	{
		RETAILMSG(DEBUG_LED,
			(TEXT("ERR:[%s]: DeviceIoControl[LED_IOCTL_SET_ALLLED_PWM] Instance[%d]failed. error = 0x%x\r\n"),
			TEXT(__FUNCTION__),
			m_Led1Param.InstanceID,
			GetLastError()));
        return FALSE;

	}

    m_bLed2SilderEnable = GetLedCurrentRGB()->Red>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Green>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Blue>LED_SILDER_ENABLE_MIN_STEP?TRUE:FALSE));
	//RETAILMSG(1, (TEXT("MSG:[%s]: m_bLed2SilderEnable[%x] RGB[%d,%d,%d].\r\n"), TEXT(__FUNCTION__),m_bLed2SilderEnable,GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue));

    m_Led2Param.dwLedControlFlag = 0xFFF;
	m_Led2Param.InstanceID = U621P_LED_INSTANCE2_NUM;
    for(int i=0;i<LED_NUMBER;i++)
    {
    	if(i>=m_iSetPara2)
    	{
        	m_Led2Param.Color[i].Red = GetLedCurrentRGB()->Red;
            m_Led2Param.Color[i].Green= GetLedCurrentRGB()->Green;
            m_Led2Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
    	}
        else
        {
        	m_Led2Param.Color[i].Red = 0;
            m_Led2Param.Color[i].Green= 0;
            m_Led2Param.Color[i].Blue = 0;
            //RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));
        }
    }

	if (!SetGroupLed(&m_Led2Param))
	{
		RETAILMSG(DEBUG_LED,
			(TEXT("ERR:[%s]: DeviceIoControl[LED_IOCTL_SET_ALLLED_PWM] Instance[%d]failed. error = 0x%x\r\n"),
			TEXT(__FUNCTION__),
			m_Led2Param.InstanceID,
			GetLastError()));
        return FALSE;
	}

    return TRUE;
}
void CLEDU621P::TemplateMethod_LedAnimPowerOn()
{
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	//SetLedShadeOn(0xF80,U621P_LED_INSTANCE1_NUM,2);
	OnLedPowerOn();
	Sleep(150);
	SetEvent(m_hLed1SilderProcessEvent);
	SetEvent(m_hLed2SilderProcessEvent);
	Sleep(1000);

	//Start the led shade animation
	if(GetLedShadeEnable())
	{
		EnableAnimShade(TRUE,TRUE);
	}
}

void CLEDU621P::TemplateMethod_LedAnimPowerOff()
{
	m_bNeedRecover1 = FALSE;
	m_bNeedRecover2 = FALSE;
	SetEvent(m_hLed1SilderProcessEvent);
	SetEvent(m_hLed2SilderProcessEvent);

	DWORD dwCount = 0;
	do
	{
		Sleep(50);
		dwCount++;
		if(dwCount>40)
		{
			RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: Led wait for ready timeout\r\n"), TEXT(__FUNCTION__)));
			break;
		}
	}while(!m_bLED1AniReady || m_bNeedRecover1 || !m_bLED2AniReady || m_bNeedRecover2);
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: Led is ready to power idle.\r\n"), TEXT(__FUNCTION__)));
}

void CLEDU621P::TemplateMethod_LedAnimPowerIdle()
{
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	m_bNeedRecover1 = FALSE;
	m_bNeedRecover2 = FALSE;
	SetEvent(m_hLed1SilderProcessEvent);
	SetEvent(m_hLed2SilderProcessEvent);


	DWORD dwCount = 0;
	do
	{
		Sleep(100);
		dwCount++;
		if(dwCount>17)
		{
			RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: Led wait for ready timeout\r\n"), TEXT(__FUNCTION__)));
			break;
		}
	}while(!m_bLED1AniReady || m_bNeedRecover1 || !m_bLED2AniReady || m_bNeedRecover2);
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: Led is ready to power idle.\r\n"), TEXT(__FUNCTION__)));

	SetLedShadeOff(0xF80,U621P_LED_INSTANCE1_NUM,2);
}

void CLEDU621P::TemplateMethod_LedAnimBreath()
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
	LedParam.Ledn = 8;//m_nBreathLedNumber;
	LedParam.InstanceID = U621P_LED_INSTANCE1_NUM;
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

void CLEDU621P::TemplateMethod_LedAnimShade()
{
	if(!m_bLED1AniReady || m_bNeedRecover1 || !m_bLED2AniReady || m_bNeedRecover2)
	{
		return;
	}

	LedShadeRun();
}


void CLEDU621P::TemplateMethod_LedMute()
{
	//led silder effect
	m_bLED1AniReady = FALSE;	//TRUE;
	m_bNeedRecover1 = FALSE;	//TRUE;
	SetEvent(m_hLed1SilderProcessEvent);

	LEDHSB hsb;
	LEDRGB rgb;
	memcpy(&hsb, GetLedCurrentHSB(), sizeof(LEDHSB));
	hsb.Hue = 0;
	hsb.Saturation = 1.0f;
	HSB2RGB(&hsb, &rgb);
	//SetLedHSB(&hsb,TRUE);

	m_Led1Param.dwLedControlFlag = 0xFF; //(0xFFF & (0xFFF << m_iSetPara));
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_Led1Param.dwLedControlFlag));
	m_Led1Param.InstanceID = U621P_LED_INSTANCE1_NUM;
	for(int i=0;i<8;i++)    //Sound silder led
	{
		if(i>=m_iSetPara1)
		{
			m_Led1Param.Color[i].Red = rgb.Red;		//GetLedCurrentRGB()->Red;
			m_Led1Param.Color[i].Green= rgb.Green;	//GetLedCurrentRGB()->Green;
			m_Led1Param.Color[i].Blue = rgb.Blue;	//GetLedCurrentRGB()->Blue;
		}
		else
		{
			m_Led1Param.Color[i].Red = 0;
			m_Led1Param.Color[i].Green= 0;
			m_Led1Param.Color[i].Blue = 0;
			//RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));
		}
	}

	if (!SetGroupLed(&m_Led1Param))
	{
		RETAILMSG(DEBUG_LED,
			(TEXT("ERR:[%s]: DeviceIoControl[LED_IOCTL_SET_ALLLED_PWM] Instance[%d]failed. error = 0x%x\r\n"),
			TEXT(__FUNCTION__),
			m_Led1Param.InstanceID,
			GetLastError()));
	}
}

void CLEDU621P::TemplateMethod_LedAnimFlash()
{

}

void CLEDU621P::TemplateMethod_LedUnmute()
{
	//led silder effect
	m_bLED1AniReady = TRUE;
	m_bNeedRecover1 = TRUE;
	SetEvent(m_hLed1SilderProcessEvent);

	m_Led1Param.dwLedControlFlag = 0xFF; //(0xFFF & (0xFFF << m_iSetPara));
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_Led1Param.dwLedControlFlag));
	m_Led1Param.InstanceID = U621P_LED_INSTANCE1_NUM;
	for(int i=0;i<8;i++)
	{
		if(i>=m_iSetPara1)
		{
			m_Led1Param.Color[i].Red = GetLedCurrentRGB()->Red;
			m_Led1Param.Color[i].Green= GetLedCurrentRGB()->Green;
			m_Led1Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
		}
		else
		{
			m_Led1Param.Color[i].Red = 0;
			m_Led1Param.Color[i].Green= 0;
			m_Led1Param.Color[i].Blue = 0;
			//RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));
		}
	}

	if (!SetGroupLed(&m_Led1Param))
	{
		RETAILMSG(DEBUG_LED,
			(TEXT("ERR:[%s]: DeviceIoControl[LED_IOCTL_SET_ALLLED_PWM] Instance[%d]failed. error = 0x%x\r\n"),
			TEXT(__FUNCTION__),
			m_Led1Param.InstanceID,
			GetLastError()));
	}
}

void CLEDU621P::OnLedPowerOn()
{
    m_Led1Param.dwLedControlFlag=0xF80;

    for(int i=0;i<LED_NUMBER;i++)
    {
    	if(i>=m_iSetPara1)
    	{
        	m_Led1Param.Color[i].Red = GetLedCurrentRGB()->Red;
            m_Led1Param.Color[i].Green= GetLedCurrentRGB()->Green;
            m_Led1Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
    	}
        else
        {
        	m_Led1Param.Color[i].Red = 0;
            m_Led1Param.Color[i].Green= 0;
            m_Led1Param.Color[i].Blue = 0;
            //RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));
        }
    }

	if(!SetGroupLed(&m_Led1Param))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]: LED_POWERON failed.\r\n"),TEXT(__FUNCTION__)));
    }

}
DWORD CLEDU621P::LED1SilderProcessThread(LPVOID lpParam)
{
    DWORD dwResult;
    BOOL bStart = FALSE;
    CLEDU621P *pLED = (CLEDU621P *)lpParam;

    while(pLED->m_bProcessing1)
    {
        dwResult = WaitForSingleObject(pLED->m_hLed1SilderProcessEvent, pLED->m_dwSetTimeOut1);
    	if(dwResult == WAIT_FAILED)
    	{
            break;
        }

        if(!pLED->m_bProcessing1)
        {
            break;
        }

        if(pLED!=NULL && !pLED->m_bSilderLed1Shutdown)
        {
            pLED->Led1SilderProcessRun();
        }
    }

	RETAILMSG(DEBUG_LED,(_T("\r\n...................LED1SilderProcessThread quit..........\r\n")));

	return 0;
}

DWORD CLEDU621P::LED2SilderProcessThread(LPVOID lpParam)
{
    DWORD dwResult;
    BOOL bStart = FALSE;
    CLEDU621P *pLED = (CLEDU621P *)lpParam;

    while(pLED->m_bProcessing2)
    {
        dwResult = WaitForSingleObject(pLED->m_hLed2SilderProcessEvent, pLED->m_dwSetTimeOut2);
    	if(dwResult == WAIT_FAILED)
    	{
            break;
        }

        if(!pLED->m_bProcessing2)
        {
            break;
        }

        if(pLED!=NULL && !pLED->m_bSilderLed2Shutdown)
        {
            pLED->Led2SilderProcessRun();
        }
    }

	RETAILMSG(DEBUG_LED,(_T("\r\n...................LED1SilderProcessThread quit..........\r\n")));

	return 0;
}

void CLEDU621P::Led1SilderProcessRun()
{
    static const BYTE step=6;
    static const BYTE light_next=LED_SILDER_ENABLE_MIN_STEP;
	static DWORD dwLastReady=0;

	BOOL bReady=TRUE;
    BOOL bRecovering=FALSE;

    m_Led1Param.dwLedControlFlag=0x7F;
	//1:
	if(GetTickCount()-m_dwSetTick1<=800 || !m_bLED1AniReady)
	{
		for (int i=0;i<LED_NUMBER;i++)
		{
			if(i!=m_iCurPara1)
			{
				if(m_bLED1TouchFlag[i])
				{
					if(m_Led1Param.Color[i].Red <= GetLedCurrentRGB()->Red-step ||
                        m_Led1Param.Color[i].Green <= GetLedCurrentRGB()->Green-step ||
                        m_Led1Param.Color[i].Blue <= GetLedCurrentRGB()->Blue-step)
					{
                        if(m_Led1Param.Color[i].Red <= GetLedCurrentRGB()->Red-step)
                            m_Led1Param.Color[i].Red+=step;
                        if(m_Led1Param.Color[i].Green <= GetLedCurrentRGB()->Green-step)
                            m_Led1Param.Color[i].Green+=step;
                        if(m_Led1Param.Color[i].Blue <= GetLedCurrentRGB()->Blue-step)
                            m_Led1Param.Color[i].Blue+=step;
						if(i!=LED_NUMBER-1)
						{
							bReady=FALSE;
						}
					}
					else
					{
						m_bLED1TouchFlag[i]=0;
						m_Led1Param.Color[i].Red=GetLedCurrentRGB()->Red;
                        m_Led1Param.Color[i].Green=GetLedCurrentRGB()->Green;
                        m_Led1Param.Color[i].Blue=GetLedCurrentRGB()->Blue;
					}
				}
				else
				{
					if(m_Led1Param.Color[i].Red>=step ||
                        m_Led1Param.Color[i].Green>=step ||
                        m_Led1Param.Color[i].Blue>=step)
					{
                        if(m_Led1Param.Color[i].Red>=step)
						    m_Led1Param.Color[i].Red-=step;
                        if(m_Led1Param.Color[i].Green>=step)
                            m_Led1Param.Color[i].Green-=step;
                        if(m_Led1Param.Color[i].Blue>=step)
                            m_Led1Param.Color[i].Blue-=step;
						if(i!=LED_NUMBER-1)
						{
							bReady=FALSE;
						}
					}
					else
					{
						m_Led1Param.Color[i].Red=0;
                        m_Led1Param.Color[i].Green=0;
                        m_Led1Param.Color[i].Blue=0;
					}
				}

			}
			else
			{
				if(m_Led1Param.Color[i].Red<=GetLedCurrentRGB()->Red-step ||
                    m_Led1Param.Color[i].Green<=GetLedCurrentRGB()->Green-step ||
                    m_Led1Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
				{
                    if(m_Led1Param.Color[i].Red<=GetLedCurrentRGB()->Red-step)
					    m_Led1Param.Color[i].Red+=step;
                    if(m_Led1Param.Color[i].Green<=GetLedCurrentRGB()->Green-step)
                        m_Led1Param.Color[i].Green+=step;
                    if(m_Led1Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
                        m_Led1Param.Color[i].Blue+=step;

					m_bLED1TouchFlag[i]=1;
					if(i!=LED_NUMBER-1)
					{
						bReady=FALSE;
					}
				}
				else
				{
					m_bLED1TouchFlag[i]=1;
					m_Led1Param.Color[i].Red=GetLedCurrentRGB()->Red;
                    m_Led1Param.Color[i].Green=GetLedCurrentRGB()->Green;
                    m_Led1Param.Color[i].Blue=GetLedCurrentRGB()->Blue;
				}

				if(m_Led1Param.Color[i].Red>=light_next ||
                    m_Led1Param.Color[i].Green>=light_next ||
                    m_Led1Param.Color[i].Blue>=light_next)
				{
					if(m_iSetPara1 != m_iCurPara1 || !m_bLed1SilderEnable)
					{
						m_iCurPara1+=(m_iSetPara1>m_iCurPara1)?1:-1;
					}
				}
			}
		}
	}
	if(!m_bLED1AniReady && bReady)
	{
		dwLastReady=GetTickCount();
	}

	m_bLED1AniReady = (GetLedState()==LED_MUTE)?FALSE:(bReady && (m_iSetPara1 == m_iCurPara1));

//	    for(int i=0;i<LED_NUMBER;i++)
//	    {
//	        RETAILMSG(1, (TEXT("LED%d  RGB[%d,%d,%d]\r\n"),i,m_Led1Param.Color[i].Red,m_Led1Param.Color[i].Green,m_Led1Param.Color[i].Blue));
//	    }


	if(bReady && GetTickCount()-dwLastReady>800 && GetTickCount()-m_dwSetTick1>800 && m_bNeedRecover1 /*&& GetLedState() != LED_POWERIDLE*/)
	{
		for (int i=LED_NUMBER-1;i>m_iCurPara1;i--)
		{
			//m_Led1Param.nLedControl[i]=50;//light_max;
			BOOL bJudge = (i<(LED_NUMBER-1))?(
			(m_Led1Param.Color[i+1].Red>=light_next) ||
			(m_Led1Param.Color[i+1].Green>=light_next) ||
			(m_Led1Param.Color[i+1].Blue>=light_next)
			):TRUE;

			if(bJudge)
			{
				if(m_Led1Param.Color[i].Red<=GetLedCurrentRGB()->Red-step||
                    m_Led1Param.Color[i].Green<=GetLedCurrentRGB()->Green-step||
                    m_Led1Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
				{
                    if(m_Led1Param.Color[i].Red<=GetLedCurrentRGB()->Red-step)
					    m_Led1Param.Color[i].Red+=step;
                    if(m_Led1Param.Color[i].Green<=GetLedCurrentRGB()->Green-step)
                        m_Led1Param.Color[i].Green+=step;
                    if(m_Led1Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
                        m_Led1Param.Color[i].Blue+=step;
					bRecovering=TRUE;
				}
				else
				{
					m_Led1Param.Color[i].Red=GetLedCurrentRGB()->Red;
                    m_Led1Param.Color[i].Green=GetLedCurrentRGB()->Green;
                    m_Led1Param.Color[i].Blue=GetLedCurrentRGB()->Blue;
				}
			}
		}

		if(!bRecovering && m_bNeedRecover1)
		{
            //RETAILMSG(1, (TEXT("#LED# m_bNeedRecover1 = FALSE \r\n")));
			m_bNeedRecover1 = FALSE;
		}
	}

	if (!SetGroupLed(&m_Led1Param))
	{
        RETAILMSG(1, (TEXT("ERR:[%s]: set led pwm failed.\r\n"), TEXT(__FUNCTION__)));
	}

	if (GetLedState() == LED_MUTE)
	{
		m_dwSetTimeOut1 = LED_IDLE_FRAME_TIME;
	}
	else
	{
		if(!m_bLED1AniReady || m_bNeedRecover1)
			m_dwSetTimeOut1=LED_ANI_FRAME_TIME;
		else
			m_dwSetTimeOut1=LED_IDLE_FRAME_TIME;
	}
}

void CLEDU621P::Led2SilderProcessRun()
{
    static const BYTE step=6;
    static const BYTE light_next=LED_SILDER_ENABLE_MIN_STEP;
	static DWORD dwLastReady=0;

	BOOL bReady=TRUE;
    BOOL bRecovering=FALSE;


    m_Led2Param.dwLedControlFlag=0x7F;
	//1:
	if(GetTickCount()-m_dwSetTick2<=800 || !m_bLED2AniReady)
	{
		for (int i=0;i<LED_NUMBER;i++)
		{
			if(i!=m_iCurPara2)
			{
				if(m_bLED2TouchFlag[i])
				{
					if(m_Led2Param.Color[i].Red <= GetLedCurrentRGB()->Red-step ||
                        m_Led2Param.Color[i].Green <= GetLedCurrentRGB()->Green-step ||
                        m_Led2Param.Color[i].Blue <= GetLedCurrentRGB()->Blue-step)
					{
                        if(m_Led2Param.Color[i].Red <= GetLedCurrentRGB()->Red-step)
                            m_Led2Param.Color[i].Red+=step;
                        if(m_Led2Param.Color[i].Green <= GetLedCurrentRGB()->Green-step)
                            m_Led2Param.Color[i].Green+=step;
                        if(m_Led2Param.Color[i].Blue <= GetLedCurrentRGB()->Blue-step)
                            m_Led2Param.Color[i].Blue+=step;
						if(i!=LED_NUMBER-1)
						{
							bReady=FALSE;
						}
					}
					else
					{
						m_bLED2TouchFlag[i]=0;
						m_Led2Param.Color[i].Red=GetLedCurrentRGB()->Red;
                        m_Led2Param.Color[i].Green=GetLedCurrentRGB()->Green;
                        m_Led2Param.Color[i].Blue=GetLedCurrentRGB()->Blue;
					}
				}
				else
				{
					if(m_Led2Param.Color[i].Red>=step ||
                        m_Led2Param.Color[i].Green>=step ||
                        m_Led2Param.Color[i].Blue>=step)
					{
                        if(m_Led2Param.Color[i].Red>=step)
						    m_Led2Param.Color[i].Red-=step;
                        if(m_Led2Param.Color[i].Green>=step)
                            m_Led2Param.Color[i].Green-=step;
                        if(m_Led2Param.Color[i].Blue>=step)
                            m_Led2Param.Color[i].Blue-=step;
						if(i!=LED_NUMBER-1)
						{
							bReady=FALSE;
						}
					}
					else
					{
						m_Led2Param.Color[i].Red=0;
                        m_Led2Param.Color[i].Green=0;
                        m_Led2Param.Color[i].Blue=0;
					}
				}

			}
			else
			{
				if(m_Led2Param.Color[i].Red<=GetLedCurrentRGB()->Red-step ||
                    m_Led2Param.Color[i].Green<=GetLedCurrentRGB()->Green-step ||
                    m_Led2Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
				{
                    if(m_Led2Param.Color[i].Red<=GetLedCurrentRGB()->Red-step)
					    m_Led2Param.Color[i].Red+=step;
                    if(m_Led2Param.Color[i].Green<=GetLedCurrentRGB()->Green-step)
                        m_Led2Param.Color[i].Green+=step;
                    if(m_Led2Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
                        m_Led2Param.Color[i].Blue+=step;

					m_bLED2TouchFlag[i]=1;
					if(i!=LED_NUMBER-1)
					{
						bReady=FALSE;
					}
				}
				else
				{
					m_bLED2TouchFlag[i]=1;
					m_Led2Param.Color[i].Red=GetLedCurrentRGB()->Red;
                    m_Led2Param.Color[i].Green=GetLedCurrentRGB()->Green;
                    m_Led2Param.Color[i].Blue=GetLedCurrentRGB()->Blue;
				}

				if(m_Led2Param.Color[i].Red>=light_next ||
                    m_Led2Param.Color[i].Green>=light_next ||
                    m_Led2Param.Color[i].Blue>=light_next)
				{
					if(m_iSetPara2 != m_iCurPara2 || !m_bLed2SilderEnable)
					{
						m_iCurPara2+=(m_iSetPara2>m_iCurPara2)?1:-1;
					}
				}
			}
		}
	}

	if(!m_bLED2AniReady && bReady)
	{
		dwLastReady=GetTickCount();
	}
	m_bLED2AniReady=bReady && (m_iSetPara2 == m_iCurPara2);

    //for(int i=0;i<LED_NUMBER;i++)
    //{
    //    RETAILMSG(1, (TEXT("LED%d  RGB[%d,%d,%d]\r\n"),i,m_Led2Param.Color[i].Red,m_Led2Param.Color[i].Green,m_Led2Param.Color[i].Blue));
    //}


	if(bReady && GetTickCount()-dwLastReady>800 && GetTickCount()-m_dwSetTick2>800 && m_bNeedRecover2/* && GetLedState() != LED_POWERIDLE*/)
	{
		for (int i=LED_NUMBER-1;i>m_iCurPara2;i--)
		{
			//m_Led1Param.nLedControl[i]=50;//light_max;
			BOOL bJudge = (i<(LED_NUMBER-1))?(
			(m_Led2Param.Color[i+1].Red>=light_next) ||
			(m_Led2Param.Color[i+1].Green>=light_next) ||
			(m_Led2Param.Color[i+1].Blue>=light_next)
			):TRUE;

			if(bJudge)
			{
				if(m_Led2Param.Color[i].Red<=GetLedCurrentRGB()->Red-step||
                    m_Led2Param.Color[i].Green<=GetLedCurrentRGB()->Green-step||
                    m_Led2Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
				{
                    if(m_Led2Param.Color[i].Red<=GetLedCurrentRGB()->Red-step)
					    m_Led2Param.Color[i].Red+=step;
                    if(m_Led2Param.Color[i].Green<=GetLedCurrentRGB()->Green-step)
                        m_Led2Param.Color[i].Green+=step;
                    if(m_Led2Param.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
                        m_Led2Param.Color[i].Blue+=step;
					bRecovering=TRUE;
				}
				else
				{
					m_Led2Param.Color[i].Red=GetLedCurrentRGB()->Red;
                    m_Led2Param.Color[i].Green=GetLedCurrentRGB()->Green;
                    m_Led2Param.Color[i].Blue=GetLedCurrentRGB()->Blue;
				}
			}
		}

		if(!bRecovering && m_bNeedRecover2)
		{
            //RETAILMSG(1, (TEXT("#LED# m_bNeedRecover2 = FALSE \r\n")));
			m_bNeedRecover2 = FALSE;
		}
	}

	if (!SetGroupLed(&m_Led2Param))
	{
        RETAILMSG(1, (TEXT("ERR:[%s]: set led pwm failed.\r\n"), TEXT(__FUNCTION__)));
	}

	if(!m_bLED2AniReady || m_bNeedRecover2)
		m_dwSetTimeOut2=LED_ANI_FRAME_TIME;
	else
		m_dwSetTimeOut2=LED_IDLE_FRAME_TIME;

}

#endif
