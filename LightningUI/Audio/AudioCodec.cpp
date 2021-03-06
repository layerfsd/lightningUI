#include "AudioCodec.h"

#include "../Common/ui_common.h"

/*
#define EQ_NONE				0
#define EQ_CLASSICAL			1
#define EQ_POP					2
#define EQ_ROCK					3
#define EQ_JAZZ					4
#define EQ_USER					5
*/

CAudioCodec::CAudioCodec(CCommanderEx *pCommander,HWND hWnd,AspType aspType)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: \r\n"),__FUNCTION__));

	m_bini = FALSE;
	m_hwndNotify = hWnd;
	m_pCommander = pCommander;
	m_bMute = FALSE;
	m_bAltMute = TRUE;

	m_idCurSRC = SRC_AUDIO_INVALID;
	m_idSRCRequestPending = SRC_AUDIO_INVALID;
	m_iSRCBeforeRequestStackDepth=0;
	for (int i=0;i<32;i++)
	{
		m_idSRCBeforeRequestStack[i]=0;
	}
	for (int i=0;i<256;i++)
	{
		m_idSRCRequestParam[i]=0;
	}

	m_iBalance = 7;
	m_iFade = 7;

	m_iVolMainSet = 0;

	m_iVolAlt = 0;
	m_iVolMain = 0;
	m_iVolBeforeDucking = 0;
	m_bDucked = FALSE;
	m_bOnDuckingProcess = FALSE;
	m_iVolMainTarget = 0;
	m_iVolAltTarget = 0;
	m_iAudioDuckLevel = 0;
	m_iVolMic = 0;

	m_iEQType = EQ_NONE;
	m_i3dEffect = 0;
	m_bDSPEffectEnabled = TRUE;

	m_dwUserEQ = 0;
	//m_idWorkMode = modeMedia;

	m_idMCUSRC = -1;

	//audio priority table:
	m_CurAudioPriority = 0;
	memset(m_AudioPriorityTable,0,256);
	m_AudioPriorityTable[SRC_AUDIO_ALERT]=255;
	m_AudioPriorityTable[SRC_AUDIO_PHONE]=254;
	m_AudioPriorityTable[SRC_AUDIO_SR]=253;
	m_AudioPriorityTable[SRC_AUDIO_SPEECH]=252;
	m_AudioPriorityTable[SRC_AUDIO_ARM]=249;
	m_AudioPriorityTable[SRC_AUDIO_RADIO]=249;
	m_AudioPriorityTable[SRC_AUDIO_AUX1]=249;
	m_AudioPriorityTable[SRC_AUDIO_AUX2]=249;
	m_AudioPriorityTable[SRC_AUDIO_TV]=249;
	m_AudioPriorityTable[SRC_AUDIO_DVD]=249;


	m_iVolumeSpeedRate = 0;
	m_iVehicleCalibrateSpeed = 0;
	m_iVehicleCurrentSpeed = 0;

	//
	m_hAsp = NULL;
	m_aspType = aspType;
	m_aspMode = AspModeMedia;

	m_bWaitingMainAudioUnmute = FALSE;
	m_iDuckSpeed  = 30;
	m_bLimitVolume = FALSE;
	m_iLimitVolume = 6;
	m_iNormalVolume = 30;
}

CAudioCodec::~CAudioCodec(void)
{
	if(m_hAsp)
	{
		AspDeinit(m_hAsp);
		m_hAsp = NULL;
	}
}

void CAudioCodec::ChangeAspWorkMode(AspMode mode)
{
	switch(m_aspType)
	{
	case AspTypeFm1388:
		if(m_hAsp)
		{
			if(AspChangeMode(m_hAsp,mode))
			{
				m_aspMode = mode;
			}
		}
		break;
	case AspTypeAK7738:
		if (codec_ChangeWorkMode(mode))
			m_aspMode = mode;
		break;
	default:
		break;
	}
}
AspMode CAudioCodec::GetAspWorkMode()
{
	return m_aspMode;
}
void CAudioCodec::OnSystemPowerState(UINT idState)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodec::OnSystemPowerState :%d+++++\r\n"),idState));
	switch (idState)
	{
	case SYS_POWER_OFF:
	case SYS_POWER_IDLE:
		{
			m_idMCUSRC = -1;
			m_idCurSRC = SRC_AUDIO_INVALID;
		}
		break;
	case SYS_POWER_ON:
		{

		}
		break;
	}
}
BOOL CAudioCodec::IsCodecIni()
{
	return m_bini;
}
BOOL CAudioCodec::IniAsp()
{
	if(m_hAsp == NULL)
	{
		RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++CAudioCodec::IniAsp, AspType=%d+++++\r\n"),m_aspType));
		ASP_DESC asp_para;

		switch(m_aspType)
		{
		case AspTypeFm1388:
			{
				memset(&asp_para,0,sizeof(ASP_DESC));

				asp_para.type = m_aspType;
				asp_para.iicPort = IICPort1;
				asp_para.iicDevAddr = 0x58;
				asp_para.gpioRstID = (1*32) + 29; //group 1, index 29
				asp_para.spiPort = SPIPort1;
				asp_para.spiDCB = 20 * 1000000; //20M
				asp_para.iicClock = 400000; //400K

#if 0
				asp_para.pDataConfig[AspData_DSP_SR_24K] =(PDWORD)FM1388_DSP_VR24K;
				asp_para.dwDataConfigLen[AspData_DSP_SR_24K] = sizeof(FM1388_DSP_VR24K)/sizeof(DWORD);
				asp_para.pDataConfig[AspData_Path_24K] = (PDWORD)FM1388_path_24K;
				asp_para.dwDataConfigLen[AspData_Path_24K] = sizeof(FM1388_path_24K)/sizeof(DWORD);

				asp_para.dwFirmwareAddr[AspFirmware_1] = 0x5FFC0000;
				asp_para.strFirmwareFileName[AspFirmware_1] = _T("\\GPS CARD\\FM1388_5FFC0000.dat");
				asp_para.dwFirmwareAddr[AspFirmware_2] = 0x5FFE0000;
				asp_para.strFirmwareFileName[AspFirmware_2] = _T("\\GPS CARD\\FM1388_5FFE0000.dat");
				asp_para.dwFirmwareAddr[AspFirmware_3] = 0x50000000;
				asp_para.strFirmwareFileName[AspFirmware_3] = _T("\\GPS CARD\\FM1388_50000000.dat");
				asp_para.dwFirmwareAddr[AspFirmware_4] = 0x60000000;
				asp_para.strFirmwareFileName[AspFirmware_4] = _T("\\GPS CARD\\FM1388_60000000.dat");
#endif

				m_hAsp = AspInit(&asp_para);
			}
			break;
		default:
			break;
		}
	}

	return (m_hAsp != NULL);
}
BOOL CAudioCodec::IniCodec()
{
	RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++CAudioCodec::IniCodec+++++\r\n")));

	switch (m_aspType)
	{
	case AspTypeFm1388:
		codec_Ini();
		m_bini = IniAsp();
		break;
	case AspTypeAK7738:
		m_bini = codec_Ini();
		break;
	default:
		break;
	}

	RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++CAudioCodec::IniCodec, bRet=%d+++++\r\n"),m_bini));

	return m_bini;
}
void CAudioCodec::EnableDSPEffect(BOOL bEnable)
{
	static int s_last_eq=0;
	static int s_last_3d=0;

	if(m_bDSPEffectEnabled != bEnable)
	{
		m_bDSPEffectEnabled = bEnable;

		if(!bEnable)
		{
			s_last_3d=m_i3dEffect;
			s_last_eq=m_iEQType;

			RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++CAudioCodec::DisbleDSPEffect and mute rear speakers\r\n")));

			SetEQ(EQ_NONE);
			Set3DEffect(0);
			MuteRearSpeakers(TRUE);
		}
		else
		{
			RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++CAudioCodec::EnableDSPEffect, eq=%d, 3d=%d, unmute rear speakers \r\n"),s_last_eq,s_last_3d));

			SetEQ(s_last_eq);
			Set3DEffect(s_last_3d);
			MuteRearSpeakers(FALSE);
		}
	}
}
BOOL CAudioCodec::MuteAMP(BOOL bMute)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),bMute));

	return codec_MuteAMP(bMute);
}
BOOL CAudioCodec::GetMute()
{
	return m_bMute;
}

BOOL CAudioCodec::Mute(BOOL bMute)
{
	if(m_bMute != bMute)
	{
		RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),bMute));
		m_bMute = bMute;

		if(bMute)
		{
			//notify volume 0:
			::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_VOLUME,0);
			waveOutSetVolume(HWAVEOUT(1),0);
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
			//codec_SetMainVol(0);//DAC vol set to lowest
			return codec_MuteCodec(TRUE);
#else
			//codec_MuteCodec(TRUE);
			return codec_SetMainVol(0);//DAC vol set to lowest
#endif
		}
		else
		{
			waveOutSetVolume(HWAVEOUT(1),-1);
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
			return codec_MuteCodec(FALSE);
#else
			//codec_MuteCodec(FALSE);
			return codec_SetMainVol(0);//DAC vol set to lowest
#endif
		}
	}

	return FALSE;
}
BOOL CAudioCodec::MuteRearSpeakers(BOOL bMute)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),bMute));
	return codec_MuteRearSpeakers(bMute);
}
BOOL CAudioCodec::SetAGC(BOOL bEnbale)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),bEnbale));
	return codec_SetAGC(bEnbale);
}
BOOL CAudioCodec::SetSampleRate(int iSampleRate)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),iSampleRate));
	return codec_SetSampleRate(iSampleRate);
}
BOOL CAudioCodec::SetMicVolume(int vol)
{
	return codec_SetMicVolume(vol);
}
BOOL CAudioCodec::SetMainVolume(int vol, BOOL bSync)
{
	BOOL bRet=FALSE;

	if(bSync)
	{
		int vol_to_set;

		if(vol<0 && m_iVolMain==0)
		{
			//no need to reset volume if current volume = 0.
			return FALSE;
		}

		vol_to_set=(vol<0)?m_iVolMain:vol;

		//reserve limit volume:
		if(m_bLimitVolume)
		{
			if(!m_bOnDuckingProcess
				&& m_idCurSRC != SRC_AUDIO_PHONE 
				&& m_idCurSRC != SRC_AUDIO_SPEECH
				&& m_idCurSRC != SRC_AUDIO_ALERT
				&& m_idCurSRC != SRC_AUDIO_SR)//media mode
			{
				vol_to_set = m_iLimitVolume;
			}
		}
		else
		{
			m_iNormalVolume = vol_to_set;
		}

		if(!m_bOnDuckingProcess)
		{
			m_iVolMainSet = vol;
		}

		//auto mute if vol==0:
		Mute(vol == 0);

		//calculate speed rate:
		if(vol_to_set > 0)
		{
			int delt = 32 * (m_iVehicleCurrentSpeed*m_iVolumeSpeedRate) / 1200;
			delt = min(delt,50-vol_to_set);
			if(delt>0)
			{
				vol_to_set += delt;
				if(vol_to_set>64)
					vol_to_set=64;
			}
		}

		codec_SetMainVol(vol_to_set);

		//RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodec::SetVolume main: %d\r\n"),vol));

		//Mute(vol == 0, 0);

		if(vol >= 0 && m_idCurSRC != SRC_AUDIO_SPEECH && m_idCurSRC != SRC_AUDIO_ALERT)
		{
			::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_MAIN_VOLUME,vol);
		}
	}
	else
	{
		bRet = TRUE;
	}

	if( vol >= 0 )
	{
		m_iVolMain = vol;
	}

	return bRet;

}
BOOL CAudioCodec::SetLimitVolume(BOOL bLimit,int vol)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: bLimit=%d, vol=%d\r\n"), TEXT(__FUNCTION__),bLimit,vol));

	m_bLimitVolume = bLimit;
	m_iLimitVolume = vol;

	//reset volume:
	if(bLimit)
	{
		if(m_idCurSRC != SRC_AUDIO_PHONE 
			&& m_idCurSRC != SRC_AUDIO_SPEECH
			&& m_idCurSRC != SRC_AUDIO_ALERT
			&& m_idCurSRC != SRC_AUDIO_SR)//media mode
		{
			//SetMainVolume(m_iLimitVolume);
			DuckMainAudio(TRUE,2);
		}
	}
	else
	{
		//SetMainVolume(m_iNormalVolume);
		DuckMainAudio(FALSE);
	}

	return TRUE;
}
BOOL CAudioCodec::SetAltVolume(int vol, BOOL bSync)
{
	BOOL bRet=FALSE;

	if(bSync)
	{
		//
		int vol_to_set;

		if(vol<0 && m_iVolAlt==0)
		{
			//no need to reset volume if current volume = 0.
			return FALSE;
		}

		vol_to_set=(vol<0)?m_iVolAlt:vol;

		//calculate speed rate:
		if(vol_to_set > 0)
		{
			int delt = 32 * (m_iVehicleCurrentSpeed*m_iVolumeSpeedRate/12) / 100;
			delt = min(delt,50-vol_to_set);
			if(delt>0)
			{
				vol_to_set += delt;
				if(vol_to_set>64)
					vol_to_set=64;
			}
		}

		codec_SetAltVol(vol_to_set);

		//RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodec::SetVolume alt: %d\r\n"),vol));

		if(vol >= 0 && (m_idCurSRC == SRC_AUDIO_SPEECH || m_idCurSRC == SRC_AUDIO_ALERT))
		{
			::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_ALT_VOLUME,vol);
		}
	}
	else
	{
		bRet = TRUE;
	}

	if( vol >= 0 )
	{
		m_iVolAlt = vol;
	}

	return bRet;
}
BOOL CAudioCodec::SetMainVolumeTarget(int vol,BOOL bSync)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: vol:%d bSync:%d\r\n"), TEXT(__FUNCTION__),vol,bSync));


	m_iVolMainSet = vol;
	m_iVolBeforeDucking = vol;
	if(bSync)
	{
		m_iVolMain = m_iVolMainSet;
	}
	return TRUE;
}

int CAudioCodec::GetMainVolume()
{
	return m_iVolMain;
}

int CAudioCodec::GetAltVolume()
{
	return m_iVolAlt;
}

int CAudioCodec::GetMainVolumeTarget()
{
	return m_iVolMainSet;
}
BOOL CAudioCodec::SetVolume(int vol)
{		
	BOOL bRet = FALSE;

	//vol:0-64
	if(m_idCurSRC == SRC_AUDIO_SPEECH || m_idCurSRC == SRC_AUDIO_ALERT)
	{
		bRet =SetAltVolume(vol,!m_bAltMute);
	}
	else
	{
		if(m_bWaitingMainAudioUnmute)
		{
			RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: WaitingMainAudioUnmute, cancel set volume\r\n"), TEXT(__FUNCTION__)));
			return FALSE;
		}		

		if(m_idCurSRC != SRC_AUDIO_PHONE 
			&& m_idCurSRC != SRC_AUDIO_SPEECH
			&& m_idCurSRC != SRC_AUDIO_ALERT
			&& m_idCurSRC != SRC_AUDIO_SR)//media mode
		{
			if(vol>=0)
			{
				m_iVolMainSet = vol;
				m_iVolBeforeDucking = vol;
			}
		}

		if(!m_bOnDuckingProcess)
		{
			bRet =SetMainVolume(vol);				
		}
		else if(vol>=0)
		{
			RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: onducking...update target:%d\r\n"), TEXT(__FUNCTION__),vol));

			m_iVolMainTarget = vol;
			bRet=TRUE;
		}
	}

	//RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodec::SetVolume:%d, bRet=%d\r\n"),vol,bRet));

	//::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_VOLUME,vol);

	return bRet;
}

BOOL CAudioCodec::SetLoudness(BOOL bOn)
{
	return codec_SetLoudness(bOn);
}
BOOL CAudioCodec::SetSubwoofer(BYTE val)
{
	return codec_SetSubwoofer(val);
}

BOOL CAudioCodec::SetBass(BYTE val)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: val:%d \r\n"), TEXT(__FUNCTION__),val));

	return codec_SetBass(val);
}
BOOL CAudioCodec::SetMid(BYTE val)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: val:%d \r\n"), TEXT(__FUNCTION__),val));
	return codec_SetMid(val);
}
BOOL CAudioCodec::SetTreble(BYTE val)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: val:%d \r\n"), TEXT(__FUNCTION__),val));
	return codec_SetTreble(val);
}
BOOL CAudioCodec::SetEQBand(int iBand,int val)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: iBand:%d val:%d \r\n"), TEXT(__FUNCTION__),iBand, val));
	return codec_SetEQBand(iBand,val);
}
BOOL CAudioCodec::SetEQ(BYTE val)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: val:%d \r\n"), TEXT(__FUNCTION__),val));
	return codec_SetEQPreset(val);
}
BOOL CAudioCodec::SetBalFad(BYTE bal,BYTE fad)
{
	return codec_SetBalFad(bal,fad);
}
BOOL CAudioCodec::Set3DEffect(BYTE val)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: val:%d \r\n"), TEXT(__FUNCTION__),val));
	return codec_Set3DEffect(val);
}
BOOL CAudioCodec::SetBalance(BYTE val)
{
	return codec_SetBalance(val);
}
BOOL CAudioCodec::SetFade(BYTE val)
{
	return codec_SetFade(val);
}
BOOL CAudioCodec::SetMixerOn(BOOL bOn, BOOL bMuteMain)
{
	return codec_SetMixerOn(bOn, bMuteMain);
}
BOOL CAudioCodec::IsValidMcuSrc(UINT id)
{
	return (id==SRC_AUDIO_ARM)
		|| (id==SRC_AUDIO_RADIO)
		|| (id==SRC_AUDIO_DVD)
		|| (id==SRC_AUDIO_TV) 
		|| (id==SRC_AUDIO_AUX1)
		|| (id==SRC_AUDIO_AUX2)
		|| (id==SRC_AUDIO_DAB)
		||( id==SRC_AUDIO_MHL) 	;
}

BOOL CAudioCodec::SelectAudioSrc(UINT idSrc)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: idSrc:%d \r\n"), TEXT(__FUNCTION__),idSrc));

	m_idCurSRC = idSrc;

#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
	//mcu source:
	UINT idtemp=SRC_AUDIO_INVALID;
	if(IsValidMcuSrc(idSrc))
	{
		idtemp = idSrc;
	}
	else if(idSrc==SRC_AUDIO_PHONE||idSrc==SRC_AUDIO_SR)
	{
		idtemp=SRC_AUDIO_ARM;
	}

	//WHH 20170522
	//这里写法不合适，具体动作应移动到具体SOC类中进行处理
	if(m_pCommander && idtemp!=SRC_AUDIO_INVALID && m_idMCUSRC!=idtemp)
	{
		m_idMCUSRC=idtemp;
		RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("MSG:[%s]: send MCU SRC:%d\r\n"),_T(__FUNCTION__),idtemp));
		m_pCommander->SendCmd(_ARM_AUDIO_SW_KEY,idtemp);
	}
#endif
	//codec source:
	codec_SelectAudioSrc(idSrc);

	return TRUE;
}

BOOL CAudioCodec::MuteAltAudio(BOOL bMute)
{
	if(m_bAltMute != bMute)
	{
		RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: bMute:%d \r\n"), TEXT(__FUNCTION__),bMute));

		m_bAltMute = bMute;

		if(bMute)
		{
			/*
			int alt_vol=m_iVolAlt;
			SetAltVolume(0);
			m_iVolAlt=alt_vol;
			Sleep(20);
			*/
			waveOutSetVolume(HWAVEOUT(0),0);
		}
		else
		{
			/*
			int alt_vol=m_iVolAlt;
			SetAltVolume(alt_vol);
			m_iVolAlt=alt_vol;
			Sleep(20);
			*/
			waveOutSetVolume(HWAVEOUT(0),-1);
		}
	}

	return TRUE;
}

BOOL CAudioCodec::IsCodecConfigFinished()
{
	return codec_IsCodecConfigFinished();
}
DWORD CAudioCodec::MainVolumeThreadPro(LPVOID lpParam)
{

	CAudioCodec *pCodec = (CAudioCodec*)lpParam;

	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: start,target:%d, current:%d\r\n"),
		_T(__FUNCTION__),pCodec->m_iVolMainTarget,pCodec->m_iVolMain));

	while (pCodec->m_iVolMain != pCodec->m_iVolMainTarget)
	{
		BOOL bp = pCodec->m_iVolMainTarget>pCodec->m_iVolMain;
		int diff=abs(pCodec->m_iVolMainTarget-pCodec->m_iVolMain);
// 		int st=bp?1:2;//abs(pCodec->m_iVolMain - pCodec->m_iVolMainTarget)/10;
		int st=1;

		if(diff<=st)
		{
			pCodec->m_iVolMain = pCodec->m_iVolMainTarget;
		}
		else
		{
			int step=min(diff,st);
			pCodec->m_iVolMain += bp ? step : -step;
		}
		pCodec->SetMainVolume(pCodec->m_iVolMain);
// 		Sleep(bp?40:20);
		Sleep(/*30*/pCodec->m_iDuckSpeed);
	}

	pCodec->m_bOnDuckingProcess = FALSE;

	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: end\r\n"),_T(__FUNCTION__)));

	return 0;
}
void CAudioCodec::SetUserEQ(DWORD dwUserEQ,BOOL bRefresh)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: 0x%X\r\n"),_T(__FUNCTION__),dwUserEQ));

	m_dwUserEQ=dwUserEQ;
	if(bRefresh)
	{
		SetEQ(BYTE(dwUserEQ>>24));
	}
}
BOOL CAudioCodec::MuteMainAudio(BOOL bMute,BOOL bFadeOut,BOOL bFadeIn,DWORD iTimeOut,int iSpeed)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: bMute=%d, bFadeOut=%d, bFadeIn=%d, iTimeOut=%d,speed=%d\r\n"),
		_T(__FUNCTION__),bMute,bFadeOut,bFadeIn,iTimeOut,iSpeed));

	if(bMute)
	{
		DuckMainAudio(TRUE,0,bFadeOut,iSpeed);

		if(m_bWaitingMainAudioUnmute)
		{
			m_bWaitingMainAudioUnmute = FALSE;
			KillTimer(m_hwndNotify,TIMER_AUDIO_MUTE_TIMEOUT);
		}
		if(iTimeOut != -1)
		{
			m_bFadeIn = bFadeIn;
			m_bWaitingMainAudioUnmute = TRUE;
			SetTimer(m_hwndNotify,TIMER_AUDIO_MUTE_TIMEOUT,iTimeOut,NULL);
		}
	}
	else
	{
		DuckMainAudio(FALSE,0,bFadeOut,iSpeed);
	}

	return TRUE;
}
void CAudioCodec::OnMainAudioMuteTimeout()
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: \r\n"),_T(__FUNCTION__)));

	KillTimer(m_hwndNotify,TIMER_AUDIO_MUTE_TIMEOUT);

	m_bWaitingMainAudioUnmute = FALSE;
	DuckMainAudio(FALSE,0,m_bFadeIn);
}
BOOL CAudioCodec::IsAudioVolumeProcessReady()
{
	return !m_bWaitingMainAudioUnmute && !m_bOnDuckingProcess;
}
void CAudioCodec::DuckMainAudio(BOOL bDuck, int iLevel,BOOL bFade,int iSpeed)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s] m_bDucked:%d bDuck:%d, level:%d, bFade:%d m_idCurSRC:%d m_iVolMain:%d m_iVolMainTarget:%d\r\n"),
		_T(__FUNCTION__),m_bDucked, bDuck,iLevel,bFade,m_idCurSRC,m_iVolMain,m_iVolMainTarget));

	if(iSpeed<5)
		iSpeed=5;

	m_iDuckSpeed = iSpeed;

	if(bDuck)
	{
		m_iAudioDuckLevel = iLevel;

		if(!m_bDucked)
		{
			m_bDucked = TRUE;
			m_iVolBeforeDucking = m_iVolMainSet;
			m_iVolMainTarget = m_iVolBeforeDucking*iLevel/10;
		}
		else
		{
			m_iVolMainTarget = m_iVolBeforeDucking*iLevel/10;
		}

		//invalid main volume:
		if(m_iVolMain>m_iVolMainSet)
		{
			RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("MSG:[%s]: current main volume is invalid: %d, direct set to :%d\r\n"),_T(__FUNCTION__),m_iVolMain,m_iVolMainTarget));
			SetMainVolume(m_iVolMainTarget);
		}

		if(m_iVolMainTarget<m_iVolMain)
		{
			if(bFade)
			{
				if(!m_bOnDuckingProcess)
				{
					m_bOnDuckingProcess = TRUE;
					CreateThread(NULL,NULL,MainVolumeThreadPro,this,NULL,NULL);
				}
			}
			else
			{
				SetMainVolume(m_iVolMainTarget);
			}
		}
	}
	else
	{
		if(m_bDucked)
		{
			if(m_idCurSRC == SRC_AUDIO_SPEECH)//navi speech is still on, fade on default value 6
			{
				m_iAudioDuckLevel = AUDIO_DUCK_LEVEL_DEFAULT;

				m_iVolMainTarget = m_iVolMainSet*m_iAudioDuckLevel/10;

				if(m_iVolMainTarget>m_iVolMain)
				{
					if(bFade)
					{
						if(!m_bOnDuckingProcess)
						{
							m_bOnDuckingProcess = TRUE;
							CreateThread(NULL,NULL,MainVolumeThreadPro,this,NULL,NULL);
						}
					}
					else
					{
						SetMainVolume(m_iVolMainTarget);
					}
				}
				else
				{
					SetMainVolume(m_iVolMain);
				}
			}
			else
			{
				m_bDucked = FALSE;
				m_iVolMainTarget = m_iVolBeforeDucking;

				if(m_iVolMainTarget>m_iVolMain)
				{
					if(bFade)
					{
						if(!m_bOnDuckingProcess)
						{
							m_bOnDuckingProcess = TRUE;
							//SetMainVolume(m_iVolMain*6/10);
							CreateThread(NULL,NULL,MainVolumeThreadPro,this,NULL,NULL);
						}
					}
					else
					{
						SetMainVolume(m_iVolMainTarget);
					}
				}
				else
				{
					SetMainVolume(m_iVolMain);
				}
			}
		}
	}

// 	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_VOLUME,m_iVolMainTarget);

	//RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("MSG:[%s]:bDuck:%d, level:%d,bFade:%d, VolMainTarget:%d, speed:%d\r\n"),_T(__FUNCTION__),bDuck,iLevel,bFade,m_iVolMainTarget,m_iDuckSpeed));

}

AUDIO_REQUEST_STATUS CAudioCodec::RequestAudioBus(UINT idSrc,UINT iParam)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: ++ RequestAudioBus=%d, iParam=%d, current=%d\r\n"),TEXT(__FUNCTION__),idSrc,iParam,m_idCurSRC));

	if(idSrc>=SRC_AUDIO_INVALID)
		return AUDIO_REQUEST_FAILED;

	m_idSRCRequestParam[idSrc] = iParam;

	if(m_idCurSRC==idSrc)
	{
		RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: m_idCurSRC=idSrc=%d return \r\n"),TEXT(__FUNCTION__),idSrc));
		return AUDIO_REQUEST_SUCCEED;
	}

	AUDIO_REQUEST_STATUS st=AUDIO_REQUEST_FAILED;
	//RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: m_AudioPriorityTable[%d]=%d, m_AudioPriorityTable[m_idCurSRC]=%d, m_CurAudioPriority:%d\r\n"),TEXT(__FUNCTION__),idSrc,m_AudioPriorityTable[idSrc],m_AudioPriorityTable[m_idCurSRC],m_CurAudioPriority));

	if(m_AudioPriorityTable[idSrc] >= m_CurAudioPriority)
	{
		m_CurAudioPriority = m_AudioPriorityTable[idSrc];
		m_idSRCRequestPending = SRC_AUDIO_INVALID;
		//m_idSRCBeforeRequest = m_idCurSRC;

		//reset request stack depth:
		if(IsValidMcuSrc(idSrc))
			m_iSRCBeforeRequestStackDepth = 0;

		//set auto recover sources:
		if(m_idCurSRC != SRC_AUDIO_INVALID 
			&& m_idCurSRC != SRC_AUDIO_SR 
			&& m_idCurSRC != SRC_AUDIO_SPEECH
			&& m_idCurSRC != SRC_AUDIO_ALERT)
		{
			m_idSRCBeforeRequestStack[m_iSRCBeforeRequestStackDepth] = m_idCurSRC;
			m_iSRCBeforeRequestStackDepth++;
		}

		SelectAudioSrc(idSrc);

		//addition operation:
		if(idSrc==SRC_AUDIO_SPEECH)
		{
			if(m_iVolAlt)
			{
				DuckMainAudio(TRUE,6);
			}
		}

		st = AUDIO_REQUEST_SUCCEED;
	}
	else
	{
		m_idSRCRequestPending = idSrc;
		//m_idSRCBeforeRequest = m_idCurSRC;
		st = AUDIO_REQUEST_PENDING;
	}


	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: -- RequestAudioBus: %d, status: %d, depth:%d\r\n"),TEXT(__FUNCTION__),idSrc,st,m_iSRCBeforeRequestStackDepth));

	return st;
}
UINT CAudioCodec::ReleaseAudioBus(UINT idSrc)
{
	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: ++ ReleaseAudioBus=%d, current=%d\r\n"),TEXT(__FUNCTION__),idSrc,m_idCurSRC));

	if(m_idCurSRC == idSrc)
	{
		UINT idCur=m_idCurSRC;
		BOOL bs=FALSE;

		//
		if(m_idSRCRequestPending != SRC_AUDIO_INVALID && m_idSRCRequestPending != idSrc)
		{
			SelectAudioSrc(m_idSRCRequestPending);
			m_idSRCRequestPending = SRC_AUDIO_INVALID;
			bs=TRUE;
		}
		else if(m_iSRCBeforeRequestStackDepth > 0)
		{
			//recover a valid source:
			while(m_iSRCBeforeRequestStackDepth>0)
			{
				m_iSRCBeforeRequestStackDepth--;
				if(m_idSRCBeforeRequestStack[m_iSRCBeforeRequestStackDepth] != SRC_AUDIO_INVALID
					&& m_idSRCBeforeRequestStack[m_iSRCBeforeRequestStackDepth] != idSrc)
				{
					SelectAudioSrc(m_idSRCBeforeRequestStack[m_iSRCBeforeRequestStackDepth]);
					bs=TRUE;
					break;
				}
			}
		}

		m_CurAudioPriority = 0;

		//addition operation:
		if(idCur==SRC_AUDIO_SPEECH && bs)
		{
			if(m_bDucked && m_iAudioDuckLevel != 0)
			{
				DuckMainAudio(FALSE);
			}
		}
		RETAILMSG(DEBUG_AUDIO_CODEC,(_T("MSG:[%s]: ReleaseAudioBus SUCCEED. idSrc:%d, to SRC: %d,\r\n"),TEXT(__FUNCTION__),idSrc,m_idCurSRC));
	}
	else
	{
		RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("MSG:[%s]: ReleaseAudioBus FAILED! idSrc:%d, current src is %d\r\n"),TEXT(__FUNCTION__),idSrc,m_idCurSRC));

		//clear pending source:
		if(m_idSRCRequestPending == idSrc)
		{
			m_idSRCRequestPending = SRC_AUDIO_INVALID;
		}

		//clear source in request statck;
		 if(m_iSRCBeforeRequestStackDepth > 0)
		 {
			 for (int i=0;i<m_iSRCBeforeRequestStackDepth;i++)
			 {
				 if(m_idSRCBeforeRequestStack[i] == idSrc)
				 {
					 m_idSRCBeforeRequestStack[i] = SRC_AUDIO_INVALID;
				 }
			 }
		 }
	}

	return m_idCurSRC;
}
UINT CAudioCodec::GetCurrentAudioBus()
{
	return m_idCurSRC;
}
void CAudioCodec::SetVolumeSpeedRate(int iRate)
{
	//iRate: 0 - 10
	if(iRate<0)
		iRate=0;
	if(iRate>10)
		iRate=10;
	m_iVolumeSpeedRate = iRate;
}
void CAudioCodec::SetVehicleCurrentSpeed(int iKmh)
{
	if(iKmh<0)
		iKmh=0;

	if(m_iVehicleCurrentSpeed != iKmh)
	{
// 		m_iVehicleCurrentSpeed = iKmh;
		m_iVehicleCurrentSpeed += (iKmh-m_iVehicleCurrentSpeed)/3;
		//set -1 to reset current volume:
		SetVolume(-1);
	}
}
void CAudioCodec::SetVehicleCalibrateSpeed(int iKmh)
{
	if(iKmh<0)
		iKmh=0;
	m_iVehicleCalibrateSpeed = iKmh;
}

BOOL CAudioCodec::SetHFStatus(AspHFStatus status)
{
	return codec_SetHFStatus(status);
}