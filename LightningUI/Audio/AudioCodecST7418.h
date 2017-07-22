#pragma once

#include "AudioCodec.h"


class CAudioCodecST7418 :
	public CAudioCodec
{
public:
	CAudioCodecST7418(CCommanderEx *pCommander,HWND hWnd,AspType aspType=AspTypeNull);
	~CAudioCodecST7418(void);

protected:
	BOOL codec_Ini();
	BOOL codec_IsCodecConfigFinished();
	BOOL codec_MuteAMP(BOOL bMute);
	BOOL codec_MuteCodec(BOOL bMute);
	BOOL codec_SetMainVol(int vol);//-127 -> + 48 : -63.5db-+24db
	BOOL codec_SetAltVol(int vol);//117->0: 117=mute,116 = -72.3db, 0=0db
	BOOL codec_MuteRearSpeakers(BOOL bMute);
	BOOL codec_SetAGC(BOOL bEnbale);
	BOOL codec_SetSampleRate(int iSampleRate);
	BOOL codec_SetMicVolume(int vol);
	BOOL codec_SetEQBand(int iBand,int val);
	BOOL codec_SetEQPreset(BYTE idPresetType);
	BOOL codec_SetBalFad(BYTE bal,BYTE fad); //0-14
	BOOL codec_Set3DEffect(BYTE val); //0-10
	BOOL codec_SetBalance(BYTE bal); //0-14
	BOOL codec_SetFade(BYTE fad); //0-14
	BOOL codec_SelectAudioSrc(UINT idSrc);
	BOOL codec_SetLoudness(BOOL bOn);
	BOOL codec_SetSubwoofer(BYTE val); //0-14
	BOOL codec_SetBass(BYTE val); //0-14
	BOOL codec_SetMid(BYTE val); //0-14
	BOOL codec_SetTreble(BYTE val); //0-14
	BOOL codec_SetMixerOn(BOOL bOn, BOOL bMuteMain);
	BOOL codec_ChangeWorkMode(AspMode mode);
	BOOL codec_SetHFStatus(AspHFStatus status);
private:
	BOOL m_bNeedAutoUnMute;
};

