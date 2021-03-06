#include "AudioCodecST7418.h"

#include "../Common/ui_common.h"


CAudioCodecST7418::CAudioCodecST7418(CCommanderEx *pCommander,HWND hWnd,AspType aspType)
:CAudioCodec(pCommander,hWnd,aspType)
{
	m_bNeedAutoUnMute = TRUE;
}

CAudioCodecST7418::~CAudioCodecST7418(void)
{

}

BOOL CAudioCodecST7418::codec_Ini()
{
	return TRUE;
}
BOOL CAudioCodecST7418::codec_IsCodecConfigFinished()
{
	return TRUE;
}
BOOL CAudioCodecST7418::codec_MuteAMP(BOOL bMute)
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MUTE_ALL,/*0x10+*/(bMute?1:0)) : FALSE;
}
BOOL CAudioCodecST7418::codec_MuteCodec(BOOL bMute)
{
	RETAILMSG(1, (TEXT("========MSG:[%s] bMute=%d\r\n"), TEXT(__FUNCTION__),bMute));
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MUTE_ALL,/*0x10+*/(bMute?1:0)) : FALSE;
}
BOOL CAudioCodecST7418::codec_SetMainVol(int vol)//0-64
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_VOLUME_KEY,BYTE(vol)) : FALSE;
}
BOOL CAudioCodecST7418::codec_SetAltVol(int vol)//0-64
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MIX_VOLUME_KEY,vol) : FALSE;
}
BOOL CAudioCodecST7418::codec_MuteRearSpeakers(BOOL bMute)
{
	return FALSE;
}
BOOL CAudioCodecST7418::codec_SetAGC(BOOL bEnbale)
{
	return FALSE;
}
BOOL CAudioCodecST7418::codec_SetSampleRate(int iSampleRate)
{
	return FALSE;
}
BOOL CAudioCodecST7418::codec_SetMicVolume(int vol)
{
	return FALSE;
}
BOOL CAudioCodecST7418::codec_SetEQBand(int iBand,int val)
{
	return FALSE;
}
BOOL CAudioCodecST7418::codec_SetEQPreset(BYTE idPresetType)
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
BOOL CAudioCodecST7418::codec_SetBalFad(BYTE bal,BYTE fad) //0-14
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
BOOL CAudioCodecST7418::codec_Set3DEffect(BYTE val) //0-10
{
	return FALSE;
}
BOOL CAudioCodecST7418::codec_SetBalance(BYTE bal) //0-14
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
BOOL CAudioCodecST7418::codec_SetFade(BYTE fad) //0-14
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
BOOL CAudioCodecST7418::codec_SelectAudioSrc(UINT idSrc)
{
	//enable MCU audio mixer if current source is speech
	if(idSrc == SRC_AUDIO_SPEECH)
	{
		//enable mixer:

	}
	else
	{
		//disable mixer:
	}

	//if(m_bNeedAutoUnMute)
	{
		//m_bNeedAutoUnMute = FALSE;
		codec_MuteCodec(FALSE);
	}

	return TRUE;
}
BOOL CAudioCodecST7418::codec_SetLoudness(BOOL bOn)
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_LOUDNESS_KEY,BYTE(bOn)) : FALSE;
}
BOOL CAudioCodecST7418::codec_SetSubwoofer(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_SUBWOOFER_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418::codec_SetBass(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_BASS_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418::codec_SetMid(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MID_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418::codec_SetTreble(BYTE val) //0-14
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_TREBLE_KEY,BYTE(val)) : FALSE;
}
BOOL CAudioCodecST7418::codec_SetMixerOn(BOOL bOn, BOOL bMuteMain)
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MIX_WAV_KEY,bOn) : FALSE;
}

BOOL CAudioCodecST7418::codec_ChangeWorkMode(AspMode mode)
{
	return TRUE;
}

BOOL CAudioCodecST7418::codec_SetHFStatus(AspHFStatus status)
{			
	return TRUE;
}