#include "AudioCodecST7418AIC3254.h"

#include "../Common/ui_common.h"


CAudioCodecST7418AIC3254::CAudioCodecST7418AIC3254(CCommanderEx *pCommander,HWND hWnd,AspType aspType)
:CAudioCodec(pCommander,hWnd,aspType)
{
	m_bNeedAutoUnMute = TRUE;
	
	m_pCmdSender = NULL;
	
}

CAudioCodecST7418AIC3254::~CAudioCodecST7418AIC3254(void)
{

}

BOOL CAudioCodecST7418AIC3254::codec_Ini()
{
	RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++%S++++\r\n"),__FUNCTION__));

	ConfigAudioCodecAIC3254Pack((BYTE*)cmd_Adaptive_filter_ini_n4p5db,sizeof(cmd_Adaptive_filter_ini_n4p5db));

	return m_pCmdSender->WaitSendFinish();
}
BOOL CAudioCodecST7418AIC3254::codec_IsCodecConfigFinished()
{
	if(m_pCmdSender)
	{
		return m_pCmdSender->IsCmdListEmpty();
	}
	return TRUE;
}
BOOL CAudioCodecST7418AIC3254::codec_MuteAMP(BOOL bMute)
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MUTE_ALL,/*0x10+*/(bMute?1:0)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_MuteCodec(BOOL bMute)
{
	RETAILMSG(1, (TEXT("========MSG:[%s] bMute=%d\r\n"), TEXT(__FUNCTION__),bMute));
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MUTE_ALL,/*0x10+*/(bMute?1:0)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetMainVol(int vol)//0-64
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_VOLUME_KEY,BYTE(vol)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetAltVol(int vol)//0-64
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,vol) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_MuteRearSpeakers(BOOL bMute)
{
	return FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetAGC(BOOL bEnbale)
{
	return FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetSampleRate(int iSampleRate)
{
	return FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetMicVolume(int vol)
{
	return FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetEQBand(int iBand,int val)
{
	return FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetEQPreset(BYTE idPresetType)
{
	/*
	EQ_NONE					0
	EQ_CLASSICAL			1
	EQ_POP					2
	EQ_ROCK					3
	EQ_JAZZ					4
	EQ_USER					5
	*/

	return FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetBalFad(BYTE bal,BYTE fad) //0-14
{
	if(m_pCommander)
	{
		m_iBalance = bal;
		m_iFade = fad;

		m_pCommander->SendCmd(_ARM_BALANCE_KEY,BYTE(bal));
		m_pCommander->SendCmd(_ARM_FAD_KEY,BYTE(fad));

		::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_BALANCE,bal);
		::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_FAD,fad);

		return TRUE;	
	}
	else
	{
		return FALSE;
	}
}
BOOL CAudioCodecST7418AIC3254::codec_Set3DEffect(BYTE val) //0-10
{
	return FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetBalance(BYTE bal) //0-14
{
	if(m_pCommander)
	{
		m_iBalance = bal;
		m_pCommander->SendCmd(_ARM_BALANCE_KEY,BYTE(bal));
		::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_BALANCE,bal);
		return TRUE;	
	}
	else
	{
		return FALSE;
	}
}
BOOL CAudioCodecST7418AIC3254::codec_SetFade(BYTE fad) //0-14
{
	if(m_pCommander)
	{
		m_iFade = fad;
		m_pCommander->SendCmd(_ARM_FAD_KEY,BYTE(fad));
		::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_FAD,fad);
		return TRUE;	
	}
	else
	{
		return FALSE;
	}
}
BOOL CAudioCodecST7418AIC3254::codec_SelectAudioSrc(UINT idSrc)
{
	//enable MCU audio mixer if current source is speech
	if (idSrc == SRC_AUDIO_SR)
	{
		if(m_hAsp)
		{
			AspChangeMode(m_hAsp,AspModeSR);
		}
	}
	else if (idSrc == SRC_AUDIO_PHONE)
	{
		
	}
	else
	{
		//disable mixer:
		if(m_hAsp)
		{
			AspChangeMode(m_hAsp,AspModeMedia);
		}
	}

	//if(m_bNeedAutoUnMute)
	{
		//m_bNeedAutoUnMute = FALSE;
		codec_MuteCodec(FALSE);
	}

	return TRUE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetLoudness(BOOL bOn)
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_LOUDNESS_KEY,BYTE(bOn)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetSubwoofer(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_SUBWOOFER_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetBass(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_BASS_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetMid(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MID_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetTreble(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_TREBLE_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418AIC3254::codec_SetMixerOn(BOOL bOn, BOOL bMuteMain)
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MIX_WAV_KEY,bOn) : FALSE;
}

BOOL CAudioCodecST7418AIC3254::ConfigAudioCodecAIC3254Pack(BYTE *buffScript, INT len)
{
	if(!buffScript || len<4)
		return FALSE;

	//RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++<%s>:len:%d\r\n"),_T(__FUNCTION__),len));

	//parse command buff:
	BYTE *ps=buffScript;
	BYTE *pe=buffScript;

	while(pe-buffScript<len)
	{
		if(pe[0]==0xAA && pe[1]==0xAA && pe[2]==0xAA && pe[3]==0xAA) //detect item end
		{
			ConfigAudioCodecAIC3254SingleLine(ps[0],ps+2,pe-ps-2,ps[1]*10);
			pe+=4;
			ps=pe;
		}
		else
		{
			pe++;
		}
	}

	return TRUE;
}

BOOL CAudioCodecST7418AIC3254::ConfigAudioCodecAIC3254SingleLine(UINT idType,BYTE *buff, INT len,INT iDelayMs)
{
	if(!m_pCmdSender)
	{
		m_pCmdSender = new CmdSender_I2C_AudioCodec();
		if(m_pCmdSender)
		{
			m_pCmdSender->SetPriority(CE_THREAD_PRIO_256_ABOVE_NORMAL);
			m_pCmdSender->LinkCommander(m_pCommander);
		}
	}

	if(m_pCmdSender)
	{
		return m_pCmdSender->QueueCmdBuff(idType,buff,len,iDelayMs);
	}

	return FALSE;
}

BOOL CAudioCodecST7418AIC3254::codec_ChangeWorkMode(AspMode mode)
{
	return TRUE;
}

BOOL CAudioCodecST7418AIC3254::codec_SetHFStatus(AspHFStatus status)
{			
	return TRUE;
}

