#include "LED_U701P.h"
#include "..\LEDCommon.h"
#include "..\LEDEffect.h"

#if CVTE_EN_LED

#define SILDER_LED_MAX_NUM	10
#define U701P_LED_INSTANCE_NUM		0

CLEDU701P::CLEDU701P(BYTE Red, BYTE Green, BYTE Blue, BYTE Brightness, BOOL bLedAnimEffect, BOOL bActivateLed)
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	LEDMODE LedMode;

	memset(&m_LedParam, 0, sizeof(LEDGROUPOPERATION));
	memset(&m_Led2Param, 0, sizeof(LEDGROUPOPERATION));
	memset(&LedMode, 0, sizeof(LedMode));
	LedMode.InstanceID = U701P_LED_INSTANCE_NUM;
	LedMode.Mode = LED_MODE_NORMAL_MIN_CURRENT;

	LEDRGB ledrgb = {255, 255, 255};
	SetLedRGB(&ledrgb);

	m_LedParam.InstanceID = 0;
	m_LedParam.dwLedControlFlag = 0xFFF;
	for(int i=0;i<LED_NUMBER;i++)
	{
		m_LedParam.Color[i].Red = 0;//Red;
		m_LedParam.Color[i].Green= 0;//Green;
		m_LedParam.Color[i].Blue = 0;//Blue;
	}	

	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	if (!SetGroupLed(&m_LedParam))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	m_Led2Param.InstanceID = 1;    //group 0
	m_Led2Param.dwLedControlFlag = 0xFFF;	//0xFFF;//  0x380;//
	for(int i=0;i<LED_NUMBER;i++)
	{
		m_Led2Param.Color[i].Red = 0;//Red;
		m_Led2Param.Color[i].Green= 0;//Green;
		m_Led2Param.Color[i].Blue = 0;//Blue;
	}
	LedMode.InstanceID = 1;

	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}

	if (!SetGroupLed(&m_Led2Param))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LED1SilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}
#else
	m_hLedSilderProcessEvent= CreateEvent(NULL,FALSE,FALSE,NULL);
	m_bProcessing=TRUE;
    m_bSilderLedShutdown = FALSE;
	m_bSiriEnable = FALSE;
	m_dwSetTimeOut = LED_IDLE_FRAME_TIME;

	m_iCurPara=SILDER_LED_MAX_NUM-1;
	m_iSetPara=SILDER_LED_MAX_NUM-1;

	LEDRGB ledrgb = {Red, Green, Blue};
	SetLedRGB(&ledrgb);
	if(Brightness <= 0 || Brightness > 100)
		SetLedBrightness((float)1.0f);
	else
		SetLedBrightness((float)(Brightness/100.0f));

    m_bLedSilderEnable = GetLedCurrentRGB()->Red>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Green>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Blue>LED_SILDER_ENABLE_MIN_STEP?TRUE:FALSE));

	m_bLEDAniReady=TRUE;
	m_bNeedRecover=FALSE;
	m_dwSetTick=0;
	memset(m_bLEDTouchFlag,0,sizeof(m_bLEDTouchFlag));
	memset(&m_LedParam, 0, sizeof(LEDGROUPOPERATION));
	m_LedParam.InstanceID = 0;    //group 1

	if(bActivateLed)
	{
		SetLedState(LED_POWERON);	
	}
	else
	{
		SetLedState(LED_POWEROFF);
	}

	LEDMODE LedMode;
	memset(&LedMode, 0, sizeof(LedMode));
	LedMode.InstanceID = 0;
	LedMode.Mode = LED_MODE_NORMAL_MIN_CURRENT;

	//RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:...................LedSilderProcessThread enter..........\r\n"),TEXT(__FUNCTION__)));
	if (!SetWorkMode(&LedMode))
	{
		RETAILMSG(DEBUG_LED,(_T("ERR:[%s]:...........DeviceIoControl failed. LedSilderProcessThread quit..........\r\n"),TEXT(__FUNCTION__)));
	}


	m_hLedSilderProcessThead=CreateThread(NULL,NULL,LedSilderProcessThread,this,NULL,NULL);

	TemplateMethod_SetLedParam(0,0);
	SetLedShadeEnable(bLedAnimEffect);
	EnableAnimShade(bLedAnimEffect);
#endif
}

CLEDU701P::~CLEDU701P(void)
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
#else
    RETAILMSG(DEBUG_LED, (TEXT("ERR:[%s].\r\n"), TEXT(__FUNCTION__)));
	//quit thread:
	if(m_hLedSilderProcessEvent != INVALID_HANDLE_VALUE)
	{
		m_bProcessing=FALSE;
		SetEvent(m_hLedSilderProcessEvent);
		CloseHandle(m_hLedSilderProcessEvent);
		m_hLedSilderProcessEvent = INVALID_HANDLE_VALUE;
	}

    if(m_hLedSilderProcessThead != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hLedSilderProcessThead);
        m_hLedSilderProcessThead = INVALID_HANDLE_VALUE;
    }

#endif
}

void CLEDU701P::TemplateMethod_SetLedParam(INT iDev,INT iLedn)
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	return;
#endif

	if(iLedn<0 || iLedn>LED_NUMBER)
	{
    	RETAILMSG(DEBUG_LED,(_T("ERR:[%s]: Param check failed.\r\n"),
            TEXT(__FUNCTION__)));
		return;
	}
//		RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:iLedn[%d]\r\n"),
//	        TEXT(__FUNCTION__),iLedn));

	if(iDev==0)
	{
        if((m_iSetPara!=9-iLedn))
		{
			if((m_dwSetTimeOut==LED_IDLE_FRAME_TIME || m_bLEDAniReady )&& m_bLedSilderEnable)
			{
				memset(m_bLEDTouchFlag,0,sizeof(m_bLEDTouchFlag));
				SetEvent(m_hLedSilderProcessEvent);
			}

			m_dwSetTick=GetTickCount();
            m_iSetPara=9-iLedn;
			m_bNeedRecover=TRUE;
			m_bLEDAniReady=FALSE;
		}
	}
}

//void CLEDU701P::TemplateMethod_LedFlash(BOOL bEnable)
//{
//
//}

BOOL CLEDU701P::TemplateMethod_LedRefresh()
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	m_LedParam.dwLedControlFlag = 0xFFF; //(0xFFF & (0xFFF << m_iSetPara));
	m_LedParam.InstanceID = 0;

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

	m_Led2Param.dwLedControlFlag = 0xFFF;
	m_Led2Param.InstanceID = 1;

	for(int i=0;i<LED_NUMBER;i++)
	{
		m_Led2Param.Color[i].Red = GetLedCurrentRGB()->Red;
		m_Led2Param.Color[i].Green= GetLedCurrentRGB()->Green;
		m_Led2Param.Color[i].Blue = GetLedCurrentRGB()->Blue;
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
#else
    m_bLedSilderEnable = GetLedCurrentRGB()->Red>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Green>LED_SILDER_ENABLE_MIN_STEP?TRUE:(GetLedCurrentRGB()->Blue>LED_SILDER_ENABLE_MIN_STEP?TRUE:FALSE));
    m_LedParam.dwLedControlFlag = 0xFFC; //(0xFFF & (0xFFF << m_iSetPara));

	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: RGB[%d,%d,%d]. GetLedState()[%d] dwLedControlFlag[%X]\r\n"), TEXT(__FUNCTION__),GetLedCurrentRGB()->Red,GetLedCurrentRGB()->Green,GetLedCurrentRGB()->Blue,GetLedState(),m_LedParam.dwLedControlFlag));
	m_LedParam.InstanceID = 0;
    if(GetLedState() == LED_MUTE)
    {
        //RETAILMSG(1, (TEXT("MSG:[%s]: GetLedState() == LED_MUTE\r\n"), TEXT(__FUNCTION__)));
        //Change the led
		LEDHSB hsb;
		LEDRGB rgb;
		memset(&rgb,0,sizeof(rgb));
		memcpy(&hsb, GetLedCurrentHSB(), sizeof(LEDHSB));
		hsb.Hue = 0;
		hsb.Saturation = 1.0f;
		HSB2RGB(&hsb,&rgb);

        for(int i=2;i<10;i++)    //Sound silder led
        {
        	if(i>=m_iSetPara)
        	{
            	m_LedParam.Color[i].Red = rgb.Red;
                m_LedParam.Color[i].Green= rgb.Green;
                m_LedParam.Color[i].Blue = rgb.Blue;
        	}
            else
            {
            	m_LedParam.Color[i].Red = 0;
                m_LedParam.Color[i].Green= 0;
                m_LedParam.Color[i].Blue = 0;
                //RETAILMSG(1, (TEXT("MSG:[%s]: led[%d] = 0.\r\n"), TEXT(__FUNCTION__),i));

            }
        }
        RestoreConfig();
        for(int i=10;i<LED_NUMBER;i++)   //Menu led
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
#endif
}

void CLEDU701P::TemplateMethod_LedAnimPowerOn()
{
    //RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
    SetLedOn(0xE00,U701P_LED_INSTANCE_NUM);
    Sleep(150);
    SetEvent(m_hLedSilderProcessEvent);
    Sleep(1000);

	//Start the led shade animation
	if(GetLedShadeEnable())
	{
		EnableAnimShade(TRUE,TRUE);
	}
}

void CLEDU701P::TemplateMethod_LedAnimPowerOff()
{
    m_iSetPara = 9;
    SetEvent(m_hLedSilderProcessEvent);
    DWORD dwCount = 0;
    do
    {
        Sleep(100);
        dwCount++;
        if(dwCount>20)
        {
            RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: Led wait for ready timeout\r\n"), TEXT(__FUNCTION__)));
            break;
        }
    }while(!m_bLEDAniReady || m_bNeedRecover);
    //RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: Led is ready to power idle.\r\n"), TEXT(__FUNCTION__)));

    Sleep(100);
    SetLedOff(0xFFC,U701P_LED_INSTANCE_NUM);
}

void CLEDU701P::TemplateMethod_LedAnimPowerIdle()
{
    //RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
    m_iSetPara = 9;
    SetEvent(m_hLedSilderProcessEvent);

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
    }while(!m_bLEDAniReady || m_bNeedRecover);
    //RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: Led is ready to power idle.\r\n"), TEXT(__FUNCTION__)));

	SetLedShadeOff(0xE00,U701P_LED_INSTANCE_NUM,2);
}

void CLEDU701P::TemplateMethod_LedAnimBreath()
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
    LedParam.Ledn = 11;//m_nBreathLedNumber;
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

void CLEDU701P::TemplateMethod_LedAnimShade()
{
	if(!m_bLEDAniReady || m_bNeedRecover)
	{
		return;
	}

	LedShadeRun();
}

void CLEDU701P::TemplateMethod_LedAnimFlash()
{

}


void CLEDU701P::TemplateMethod_LedMute()
{
	//led silder effect
	m_bLEDAniReady = FALSE;	//TRUE;
	m_bNeedRecover = FALSE;	//TRUE;
	SetEvent(m_hLedSilderProcessEvent);

	LEDHSB hsb;
	memcpy(&hsb, GetLedCurrentHSB(), sizeof(LEDHSB));
	hsb.Hue = 0;
	hsb.Saturation = 1.0f;
	SetLedHSB(&hsb,TRUE);

	m_LedParam.dwLedControlFlag = 0x3FC; //(0xFFF & (0xFFF << m_iSetPara));
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_LedParam.dwLedControlFlag));
	m_LedParam.InstanceID = 0;
	for(int i=0;i<LED_NUMBER;i++)    //Sound silder led
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
	}
}

void CLEDU701P::TemplateMethod_LedUnmute()
{
	//led silder effect
	m_bLEDAniReady = TRUE;
	m_bNeedRecover = TRUE;
	SetEvent(m_hLedSilderProcessEvent);

	m_LedParam.dwLedControlFlag = 0x3FC; //(0xFFF & (0xFFF << m_iSetPara));
	//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_LedParam.dwLedControlFlag));
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
	}
}

DWORD CLEDU701P::LedSilderProcessThread(LPVOID lpParam)
{
	DWORD dwResult;
	BOOL bStart = FALSE;
	CLEDU701P *pLED = (CLEDU701P *)lpParam;

	while(pLED->m_bProcessing)
	{
		dwResult = WaitForSingleObject(pLED->m_hLedSilderProcessEvent, pLED->m_dwSetTimeOut);
		if(dwResult == WAIT_FAILED)
		{
			break;
		}

		if(!pLED->m_bProcessing)
		{
			break;
		}

		if(pLED!=NULL && !pLED->m_bSilderLedShutdown)
		{
			pLED->LedSilderProcessRun();
		}
	}

	RETAILMSG(DEBUG_LED,(_T("\r\n...................LEDSilderProcessThread quit..........\r\n")));

	return 0;
}


void CLEDU701P::LedSilderProcessRun()
{
    static const BYTE step=6;
    static const BYTE light_next=LED_SILDER_ENABLE_MIN_STEP;
	static DWORD dwLastReady=0;

	BOOL bReady=TRUE;
    BOOL bRecovering=FALSE;

	if(GetTickCount()-m_dwSetTick<=700 || !m_bLEDAniReady)
	{
		for (int i=0;i<SILDER_LED_MAX_NUM;i++)
		{
			if(i!=m_iCurPara)
			{
				if(m_bLEDTouchFlag[i])
				{
					if(m_LedParam.Color[i].Red <= GetLedCurrentRGB()->Red-step ||
                        m_LedParam.Color[i].Green <= GetLedCurrentRGB()->Green-step ||
                        m_LedParam.Color[i].Blue <= GetLedCurrentRGB()->Blue-step)
					{
                        if(m_LedParam.Color[i].Red <= GetLedCurrentRGB()->Red-step)
                            m_LedParam.Color[i].Red+=step;
                        if(m_LedParam.Color[i].Green <= GetLedCurrentRGB()->Green-step)
                            m_LedParam.Color[i].Green+=step;
                        if(m_LedParam.Color[i].Blue <= GetLedCurrentRGB()->Blue-step)
                            m_LedParam.Color[i].Blue+=step;
						if(i!=SILDER_LED_MAX_NUM-1)
						{
							bReady=FALSE;
						}
					}
					else
					{
						m_bLEDTouchFlag[i]=0;
						m_LedParam.Color[i].Red=GetLedCurrentRGB()->Red;
                        m_LedParam.Color[i].Green=GetLedCurrentRGB()->Green;
                        m_LedParam.Color[i].Blue=GetLedCurrentRGB()->Blue;
					}
				}
				else
				{
					if(m_LedParam.Color[i].Red>=step ||
                        m_LedParam.Color[i].Green>=step ||
                        m_LedParam.Color[i].Blue>=step)
					{
                        if(m_LedParam.Color[i].Red>=step)
						    m_LedParam.Color[i].Red-=step;
                        if(m_LedParam.Color[i].Green>=step)
                            m_LedParam.Color[i].Green-=step;
                        if(m_LedParam.Color[i].Blue>=step)
                            m_LedParam.Color[i].Blue-=step;
						if(i!=SILDER_LED_MAX_NUM-1)
						{
							bReady=FALSE;
						}
					}
					else
					{
						m_LedParam.Color[i].Red=0;
                        m_LedParam.Color[i].Green=0;
                        m_LedParam.Color[i].Blue=0;
					}
				}

			}
			else
			{
				if(m_LedParam.Color[i].Red<=GetLedCurrentRGB()->Red-step ||
                    m_LedParam.Color[i].Green<=GetLedCurrentRGB()->Green-step ||
                    m_LedParam.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
				{
                    if(m_LedParam.Color[i].Red<=GetLedCurrentRGB()->Red-step)
					    m_LedParam.Color[i].Red+=step;
                    if(m_LedParam.Color[i].Green<=GetLedCurrentRGB()->Green-step)
                        m_LedParam.Color[i].Green+=step;
                    if(m_LedParam.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
                        m_LedParam.Color[i].Blue+=step;

					m_bLEDTouchFlag[i]=1;
					if(i!=SILDER_LED_MAX_NUM-1)
					{
						bReady=FALSE;
					}
				}
				else
				{
					m_bLEDTouchFlag[i]=1;
					m_LedParam.Color[i].Red=GetLedCurrentRGB()->Red;
                    m_LedParam.Color[i].Green=GetLedCurrentRGB()->Green;
                    m_LedParam.Color[i].Blue=GetLedCurrentRGB()->Blue;
				}

				if(m_LedParam.Color[i].Red>=light_next ||
                    m_LedParam.Color[i].Green>=light_next ||
                    m_LedParam.Color[i].Blue>=light_next)
				{
					if(m_iSetPara != m_iCurPara || !m_bLedSilderEnable)
					{
						m_iCurPara+=(m_iSetPara>m_iCurPara)?1:-1;
						if (m_iCurPara > (SILDER_LED_MAX_NUM-1))
							m_iCurPara = SILDER_LED_MAX_NUM-1;
					}
				}
			}
		}
	}

	if(!m_bLEDAniReady && bReady)
	{
		dwLastReady=GetTickCount();
	}

	m_bLEDAniReady = (GetLedState()==LED_MUTE)?FALSE:(bReady && (m_iSetPara == m_iCurPara));

    //RETAILMSG(1, (TEXT("bReady:%d m_bNeedRecover:%d GetLedState():%d\r\n"),bReady,m_bNeedRecover,GetLedState()));
	if(bReady && GetTickCount()-dwLastReady>700 && GetTickCount()-m_dwSetTick>700 && m_bNeedRecover)
	{
		for (int i=SILDER_LED_MAX_NUM-1;i>m_iCurPara;i--)
		{
			//m_LedParam.nLedControl[i]=50;//light_max;
			BOOL bJudge = (i<(SILDER_LED_MAX_NUM-1))?(
			(m_LedParam.Color[i+1].Red>=light_next) ||
			(m_LedParam.Color[i+1].Green>=light_next) ||
			(m_LedParam.Color[i+1].Blue>=light_next)
			):TRUE;

			if(bJudge)
			{
				if(m_LedParam.Color[i].Red<=GetLedCurrentRGB()->Red-step||
                    m_LedParam.Color[i].Green<=GetLedCurrentRGB()->Green-step||
                    m_LedParam.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
				{
                    if(m_LedParam.Color[i].Red<=GetLedCurrentRGB()->Red-step)
					    m_LedParam.Color[i].Red+=step;
                    if(m_LedParam.Color[i].Green<=GetLedCurrentRGB()->Green-step)
                        m_LedParam.Color[i].Green+=step;
                    if(m_LedParam.Color[i].Blue<=GetLedCurrentRGB()->Blue-step)
                        m_LedParam.Color[i].Blue+=step;
					bRecovering=TRUE;
				}
				else
				{
					m_LedParam.Color[i].Red=GetLedCurrentRGB()->Red;
                    m_LedParam.Color[i].Green=GetLedCurrentRGB()->Green;
                    m_LedParam.Color[i].Blue=GetLedCurrentRGB()->Blue;
				}
			}
		}

		if(!bRecovering && m_bNeedRecover)
		{
			m_bNeedRecover = FALSE;
		}
	}

    //RETAILMSG(1, (TEXT("MSG:[%s]: dwLedControlFlag[%x].\r\n"), TEXT(__FUNCTION__),m_LedParam.dwLedControlFlag));
	m_LedParam.dwLedControlFlag=0x01FC;
	if (!SetGroupLed(&m_LedParam))
	{
        RETAILMSG(1, (TEXT("ERR:[%s]: set led pwm failed.\r\n"), TEXT(__FUNCTION__)));
	}

	if (GetLedState() == LED_MUTE)
	{
		m_dwSetTimeOut = LED_IDLE_FRAME_TIME;
	}
	else
	{
		if(!m_bLEDAniReady || m_bNeedRecover)
			m_dwSetTimeOut=10;
		else
			m_dwSetTimeOut=LED_IDLE_FRAME_TIME;
	}
}

#endif
