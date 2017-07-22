#include "LEDBase.h"
#include "LEDCommon.h"
#include "mmsystem.h"

#if CVTE_EN_LED
CLEDBase::CLEDBase()
: m_LedState(LED_POWEROFF)
{
	//deafult:
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));

	memset(&m_LedRGB, 0, sizeof(m_LedRGB));
	memset(&m_LedHSB, 0, sizeof(m_LedHSB));
	memset(&m_LedRGBSave, 0, sizeof(m_LedRGBSave));
	memset(&m_LedHSBSave, 0, sizeof(m_LedHSBSave));
	m_bLedShadeActivate = FALSE;
	m_bLedFlashActivate = FALSE;
	m_bLedShadeEnable = FALSE;
	m_bLedBreathEnable = FALSE;
	m_hDev = LEDApi_Open();
	m_pcsLED = new CCriticalSection;
	m_dwAnimationTimeOut = LED_IDLE_FRAME_TIME;
	m_hLedAnimationEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hLEDAntimationThead = CreateThread(NULL,NULL,LedAnimationThread,this,NULL,NULL);

}

CLEDBase::~CLEDBase(void)
{
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:\r\n"),TEXT(__FUNCTION__)));
	if(m_hDev != INVALID_HANDLE_VALUE)
	{
		LEDApi_Close(m_hDev);
		m_hDev=NULL;
	}

	if(m_hLedAnimationEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hLedAnimationEvent);
		m_hLedAnimationEvent = INVALID_HANDLE_VALUE;
	}

	if(m_hLEDAntimationThead != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hLEDAntimationThead);
		m_hLEDAntimationThead = INVALID_HANDLE_VALUE;
	}

    if(m_pcsLED != NULL)
    {
        delete m_pcsLED;
    }

}

void CLEDBase::Lock()
{
	m_pcsLED->Enter();
}

void CLEDBase::UnLock()
{
	m_pcsLED->Leave();
}

PLEDRGB CLEDBase::GetLedCurrentRGB()
{
	return &m_LedRGB;
}

PLEDHSB CLEDBase::GetLedCurrentHSB()
{
	return &m_LedHSB;
}

PLEDRGB CLEDBase::GetLedSaveRGB()
{
	return &m_LedRGBSave;
}

PLEDHSB CLEDBase::GetLedSaveHSB()
{
	return &m_LedHSBSave;
}

BYTE CLEDBase::GetLedState()
{
	return m_LedState;
}

BOOL CLEDBase::GetLedShadeEnable()
{
	return m_bLedShadeEnable;
}

BOOL CLEDBase::GetLedBreathEnable()
{
	return m_bLedBreathEnable;
}

void CLEDBase::SetLedState(BYTE state)
{
	if(m_LedState == state)
	{
		RETAILMSG(1,(_T("WARN:[%s]: State[%d] Skip!!!\r\n"),TEXT(__FUNCTION__),state));
		return;
	}
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]: m_LedState[%d] \r\n"),TEXT(__FUNCTION__),state));

	switch(state)
	{
	case LED_POWEROFF:
		{
			ActionPowerOff();
		}
		break;
	case LED_POWERON:
		{
			ActionPowerOn();
		}
		break;
	case LED_POWERIDLE:
		{
			ActionPowerIdle();
		}
		break;
	case LED_MUTE:
		{
			ActionMute();
		}
		break;
	case LED_UNMUTE:
		{
			ActionUnMute();
		}
		break;
	case LED_BRIGHTNESS_ON:
		{

		}
		break;
	case LED_BRIGHTNESS_OFF:
		{

		}
		break;
	default:
		break;
	}

	m_LedState = state;
}

/**
	Action Function
*/
void CLEDBase::ActionPowerOff()
{
	//If the current state is MUTE, back to normal
	if(GetLedState() == LED_MUTE)
	{
		RestoreConfig();
		TemplateMethod_LedRefresh();
		Sleep(150);
	}

	//Stop the led shade animation
	if(m_bLedShadeActivate)
	{
		EnableAnimShade(FALSE);
		Sleep(150);
	}

	//Save config
	SaveConfig();

	//Do the power off template method func.
	EnableAnimPowerOff();
}

void CLEDBase::ActionPowerOn()
{
	//Disable Breath
	EnableAnimBreath(FALSE);
	Sleep(150);

	//Restore config
	RestoreConfig();

	//Do the power on template method func.
	EnableAnimPowerOn();
}

void CLEDBase::ActionPowerIdle()
{
	if(GetLedState() == LED_MUTE)
	{
		RestoreConfig();
		TemplateMethod_LedRefresh();
		Sleep(150);
	}

	//Stop the led shade animation
	if(m_bLedShadeActivate)
	{
		EnableAnimShade(FALSE);
		Sleep(150);
	}

	//Save config
	SaveConfig();

	//Do the power idle template method func.
	EnableAnimPowerIdle();
}

void CLEDBase::ActionMute()
{
	//Stop the led shade animation
	if(m_bLedShadeActivate)
	{
		EnableAnimShade(FALSE);
	}

	SaveConfig();
	TemplateMethod_LedMute();
}

void CLEDBase::ActionUnMute()
{
	RestoreConfig();
	TemplateMethod_LedUnmute();

	//Start the led shade animation
	if(m_bLedShadeEnable)
	{
		EnableAnimShade(TRUE);
		//////m_bLedShadeEnable = FALSE;
	}
}


/////////////////////////////////////////////////

void CLEDBase::SetLedShadeEnable(BOOL bEnable)
{
	m_bLedShadeEnable = bEnable;
}

void CLEDBase::SetLedBreathEnable(BOOL bEnable)
{
	m_bLedBreathEnable = bEnable;
}

BOOL CLEDBase::SetWorkMode(LEDMODE * pLedMode)
{
	if(m_hDev == NULL || m_hDev == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL bRet = FALSE;
	Lock();
	bRet = LEDApi_ChangeWorkMode(m_hDev, pLedMode);
	UnLock();

	return bRet;
}

BOOL CLEDBase::SetGroupLed(LEDGROUPOPERATION * pLedGroupOperation)
{
	if(m_hDev == NULL || m_hDev == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL bRet = FALSE;
	Lock();
	bRet = LEDApi_SetGroupPWM(m_hDev, pLedGroupOperation);
	UnLock();
	return bRet;
}

BOOL CLEDBase::SetLed(LEDOPERATION * pLedOperation)
{
	if(m_hDev == NULL || m_hDev == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL bRet = FALSE;
	Lock();
	bRet = LEDApi_SetPWM(m_hDev, pLedOperation);
	UnLock();
	return bRet;
}

void CLEDBase::SetLedHSB(LEDHSB *pLedHSB, BOOL bTemp)
{
	if(pLedHSB)
	{
		//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: bTemp:%d.\r\n"), TEXT(__FUNCTION__),bTemp));
		memcpy(&m_LedHSB, pLedHSB, sizeof(LEDHSB));
		HSB2RGB(&m_LedHSB, &m_LedRGB);
		if (!bTemp)
		{
			SaveConfig();
		}
	}
}

void CLEDBase::SetLedRGB(LEDRGB *pLedRGB, BOOL bTemp)
{
	if(pLedRGB)
	{
		//RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: bTemp:%d.\r\n"), TEXT(__FUNCTION__),bTemp));
		memcpy(&m_LedRGB, pLedRGB, sizeof(LEDRGB));
		RGB2HSB(&m_LedRGB,&m_LedHSB);
		if (!bTemp)
		{
			SaveConfig();
		}
	}
}

void CLEDBase::SetLedHue(float fHue, BOOL bTemp)
{
	if(m_LedHSB.Hue == fHue)
	{
		return ;
	}
	if(fHue<0)
	{
		fHue = 0.0f;
	}
	if(fHue>360.0f)
	{
		fHue = 360.0f;
	}
	m_LedHSB.Hue = fHue;

	HSB2RGB(&m_LedHSB, &m_LedRGB);
	if (!bTemp)
	{
		SaveConfig();
	}
}

void CLEDBase::SetLedBrightness(float fBrightness, BOOL bTemp)
{
	if(m_LedHSB.Brightness == fBrightness || fBrightness <= 0 || fBrightness > 100)
	{
		return;
	}

	if (m_LedState == LED_MUTE)
	{
		RestoreConfig();
	}
	m_LedHSB.Brightness = fBrightness;
	HSB2RGB(&m_LedHSB, &m_LedRGB);
	if (!bTemp)
	{
		SaveConfig();
	}
}


void CLEDBase::SetLedShadeOn(DWORD dwControlFlag, BYTE nInstanceID,DWORD dwTime)
{
	BYTE uCount = sizeof(GammaBrightnessIncrease);
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: ++ dwControlFlag[%X] dwTime[%d]\r\n"), TEXT(__FUNCTION__),dwControlFlag,dwTime));

	DWORD	dwAnimTime = dwTime;
	LEDGROUPOPERATION ledParam;
	LEDHSB ledHsb;
	LEDRGB ledRgb;
	memset(&ledParam, 0, sizeof(ledParam));
	memcpy(&ledHsb, GetLedCurrentHSB(), sizeof(LEDHSB));

	ledParam.dwLedControlFlag = dwControlFlag;
	ledParam.InstanceID = nInstanceID;
	for(int i=0;i<uCount;i++)
	{
		ledHsb.Brightness = (GammaBrightnessIncrease[i]/255.0f)*(GetLedCurrentHSB()->Brightness/1.0f);
		HSB2RGB(&ledHsb, &ledRgb);

		for(int i=0;i<LED_NUMBER;i++)
		{
			ledParam.Color[i].Red = ledRgb.Red;
			ledParam.Color[i].Green= ledRgb.Green;
			ledParam.Color[i].Blue = ledRgb.Blue;
		}

		if(!SetGroupLed(&ledParam))
		{
			RETAILMSG(1,
				(TEXT("ERR:[%s]: SetGroupLed Instance[%d] failed. error = 0x%x\r\n"),
				TEXT(__FUNCTION__),
				ledParam.InstanceID,
				GetLastError()));
			return;
		}

		Sleep(dwAnimTime);
	}

	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
}

void CLEDBase::SetLedShadeOff(DWORD dwControlFlag, BYTE nInstanceID,DWORD dwTime)
{
	BYTE uCount = sizeof(GammaBrightnessReduce);
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: ++ dwControlFlag[%X]\r\n"), TEXT(__FUNCTION__),dwControlFlag));

	DWORD	dwAnimTime = dwTime;
	LEDGROUPOPERATION ledParam;
	LEDHSB ledHsb;
	LEDRGB ledRgb;
	memset(&ledParam, 0, sizeof(ledParam));
	memcpy(&ledHsb, GetLedCurrentHSB(), sizeof(LEDHSB));

	ledParam.dwLedControlFlag = dwControlFlag;
	ledParam.InstanceID = nInstanceID;
	for(int i=0;i<uCount;i++)
	{
		ledHsb.Brightness = (GammaBrightnessReduce[i]/255.0f)*(GetLedCurrentHSB()->Brightness/1.0f);
		HSB2RGB(&ledHsb, &ledRgb);

		for(int i=0;i<LED_NUMBER;i++)
		{
			ledParam.Color[i].Red = ledRgb.Red;
			ledParam.Color[i].Green= ledRgb.Green;
			ledParam.Color[i].Blue = ledRgb.Blue;
		}

		if (!SetGroupLed(&ledParam))
		{
			RETAILMSG(1,
				(TEXT("ERR:[%s]: SetGroupLed Instance[%d] failed. error = 0x%x\r\n"),
				TEXT(__FUNCTION__),
				ledParam.InstanceID,
				GetLastError()));
			return;
		}

		Sleep(dwAnimTime);
	}
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));

}

void CLEDBase::SetLedOn(DWORD dwControlFlag, BYTE nInstanceID)
{
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: ++ dwControlFlag[%X] nInstanceID[%d]\r\n"), TEXT(__FUNCTION__),dwControlFlag,nInstanceID));

	LEDGROUPOPERATION ledParam;
	ledParam.dwLedControlFlag = dwControlFlag;
	ledParam.InstanceID = nInstanceID;

	for(int i=0;i<LED_NUMBER;i++)
	{
		ledParam.Color[i].Red = GetLedCurrentRGB()->Red;
		ledParam.Color[i].Green= GetLedCurrentRGB()->Green;
		ledParam.Color[i].Blue = GetLedCurrentRGB()->Blue;
	}

	if(!SetGroupLed(&ledParam))
	{
		RETAILMSG(1,
			(TEXT("ERR:[%s]: SetGroupLed Instance[%d] failed. error = 0x%x\r\n"),
			TEXT(__FUNCTION__),
			ledParam.InstanceID,
			GetLastError()));
		return;
	}

	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
}

void CLEDBase::SetLedOff(DWORD dwControlFlag, BYTE nInstanceID)
{
	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: ++ dwControlFlag[%X] nInstanceID[%d]\r\n"), TEXT(__FUNCTION__),dwControlFlag,nInstanceID));

	LEDGROUPOPERATION ledParam;
	memset(&ledParam, 0, sizeof(ledParam));
	ledParam.dwLedControlFlag = dwControlFlag;
	ledParam.InstanceID = nInstanceID;

	if(!SetGroupLed(&ledParam))
	{
		RETAILMSG(1,
			(TEXT("ERR:[%s]: SetGroupLed Instance[%d] failed. error = 0x%x\r\n"),
			TEXT(__FUNCTION__),
			ledParam.InstanceID,
			GetLastError()));
		return;
	}

	RETAILMSG(DEBUG_LED, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));
}

void CLEDBase::EnableAnimPowerOn()
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	return;
#endif

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));
	if(m_bLedBreathEnable || m_nLedAnimationType == LED_ANIMATION_POWER_BREATH)
	{
		SetLedBreathEnable(FALSE);
	}
	SetLedAnimationTimeout(LED_IDLE_FRAME_TIME);
	SetLedAnimationType(LED_ANIMATION_POWER_ON);
	SetEvent(m_hLedAnimationEvent);
}

void CLEDBase::EnableAnimPowerOff()
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	return;
#endif

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));
	SetLedAnimationTimeout(LED_IDLE_FRAME_TIME);
	SetLedAnimationType(LED_ANIMATION_POWER_OFF);
	SetEvent(m_hLedAnimationEvent);
}

void CLEDBase::EnableAnimPowerIdle()
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	return;
#endif

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));
	SetLedAnimationTimeout(LED_IDLE_FRAME_TIME);
	SetLedAnimationType(LED_ANIMATION_POWER_IDLE);
	SetEvent(m_hLedAnimationEvent);
}


void CLEDBase::EnableAnimFlash(BOOL bEnable)
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	return;
#endif

	if(m_bLedFlashActivate == bEnable)
	{
		RETAILMSG(1,(_T("WARN:[%s]: m_bLedFlashActivate=bEnable=[%d] skip!!!\r\n"), TEXT(__FUNCTION__), bEnable));
		return ;
	}
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]: bEnable[%d]\r\n"), TEXT(__FUNCTION__), bEnable));
	if(bEnable)
	{
		SetLedAnimationTimeout(LED_FLASH_FRAME_TIME);
		SetLedAnimationType(LED_ANIMATION_FLASH);
	}
	else
	{
		SetLedAnimationTimeout(LED_IDLE_FRAME_TIME);
		SetLedAnimationType(LED_ANIMATION_IDLE);
	}
	m_bLedFlashActivate = bEnable;
	SetEvent(m_hLedAnimationEvent);
}

void CLEDBase::EnableAnimShade(BOOL bEnable, BOOL bForce)
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	return;
#endif

	if(m_bLedShadeActivate == bEnable && !bForce)
	{
		RETAILMSG(1,(_T("WARN:[%s]: m_bLedShadeActivate=bEnable=[%d] skip!!!\r\n"), TEXT(__FUNCTION__), bEnable));
		return ;
	}
	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]: bEnable[%d]\r\n"), TEXT(__FUNCTION__), bEnable));
	if(bEnable)
	{
		SetLedAnimationTimeout(LED_SHADE_FRAME_TIME);
		SetLedAnimationType(LED_ANIMATION_SHADE);
	}
	else
	{
		SetLedAnimationTimeout(LED_IDLE_FRAME_TIME);
		SetLedAnimationType(LED_ANIMATION_IDLE);
	}
	m_bLedShadeActivate = bEnable;
	SetEvent(m_hLedAnimationEvent);
}

void CLEDBase::EnableAnimBreath(BOOL bEnable)
{
#if (CUSTOMER_ID == CUSTOMER_DAIICHI_FIAT_DOBLO)
	return;
#endif

	RETAILMSG(DEBUG_LED,(_T("MSG:[%s]: bEnable:%d\r\n"), TEXT(__FUNCTION__),bEnable));
	if (bEnable)
	{
		SetLedBreathEnable(TRUE);
		SetLedAnimationTimeout(LED_BREATH_FRAME_TIME);
		SetLedAnimationType(LED_ANIMATION_POWER_BREATH);
	}
	else
	{		
		SetLedAnimationTimeout(LED_IDLE_FRAME_TIME);
		SetLedAnimationType(LED_ANIMATION_IDLE);
		SetLedBreathEnable(FALSE);
	}
	SetEvent(m_hLedAnimationEvent);
}

void CLEDBase::LedAnimationRun()
{
	switch(m_nLedAnimationType)
	{
	case LED_ANIMATION_POWER_BREATH:
		if(m_bLedBreathEnable)
		{
			TemplateMethod_LedAnimBreath();
		}
		break;
	case LED_ANIMATION_SHADE:
		if(m_bLedShadeActivate)
		{
			TemplateMethod_LedAnimShade();
		}
		break;
	case LED_ANIMATION_POWER_ON:
		{
			TemplateMethod_LedAnimPowerOn();
		}
		break;
	case LED_ANIMATION_POWER_OFF:
		{
			TemplateMethod_LedAnimPowerOff();
		}
		break;
	case LED_ANIMATION_POWER_IDLE:
		{
			TemplateMethod_LedAnimPowerIdle();
			EnableAnimBreath(TRUE);
		}
		break;
	case LED_ANIMATION_FLASH:
		{
			TemplateMethod_LedAnimFlash();
		}
	default:
		break;
	}
}

DWORD CLEDBase::LedAnimationThread(LPVOID lpParam)
{
	BOOL bStart = FALSE;
	CLEDBase *pLED = (CLEDBase *)lpParam;

	//RETAILMSG(DEBUG_LED,(_T("\r\n...................LedAnimationThread enter..........\r\n")));
	if (pLED == NULL)
	{
		RETAILMSG(1,(_T("ERR:[%s]: pLED is NULL\r\n"), TEXT(__FUNCTION__)));
		return -1;
	}

	while(1)
	{
		WaitForSingleObject(pLED->m_hLedAnimationEvent, pLED->m_dwAnimationTimeOut);
		if (pLED)
		{
			pLED->LedAnimationRun();	
		}
	}

	RETAILMSG(DEBUG_LED,(_T("\r\n...................LedAnimationThread quit..........\r\n")));

	return 0;
}

void CLEDBase::LedShadeRun()
{
	m_LedHSB.Hue +=2;
	m_LedHSB.Saturation = 1.0f;

	if(m_LedHSB.Hue>=360)
		m_LedHSB.Hue = 0;

	HSB2RGB(&m_LedHSB, &m_LedRGB);
	TemplateMethod_LedRefresh();
}


void CLEDBase::SaveConfig()
{
	memcpy(&m_LedRGBSave, &m_LedRGB, sizeof(LEDRGB));
	memcpy(&m_LedHSBSave, &m_LedHSB, sizeof(LEDHSB));
	//printf("MSG:SaveConfig: HSB[%.2f,%.2f,%.2f] \r\n",m_LedHSBSave.Hue, m_LedHSBSave.Saturation, m_LedHSBSave.Brightness);
	//printf("MSG:SaveConfig: RGB[%d,%d,%d] \r\n",m_LedRGBSave.Red, m_LedRGBSave.Green, m_LedRGBSave.Blue);
}

void CLEDBase::RestoreConfig()
{
	memcpy(&m_LedRGB, &m_LedRGBSave, sizeof(LEDRGB));
	memcpy(&m_LedHSB, &m_LedHSBSave, sizeof(LEDHSB));
	//printf("MSG:RestoreConfig: HSB[%.2f,%.2f,%.2f] \r\n",m_LedHSB.Hue, m_LedHSB.Saturation, m_LedHSB.Brightness);
	//printf("MSG:RestoreConfig: RGB[%d,%d,%d] \r\n",m_LedRGB.Red, m_LedRGB.Green, m_LedRGB.Blue);
}


#endif
