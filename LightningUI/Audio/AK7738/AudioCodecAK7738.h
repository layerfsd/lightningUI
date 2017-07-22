#pragma once

#include "..\AudioCodec.h"
#include ".\Soc\AspAK7738.h"

#define DEBUG_AUDIO_CODEC_AK7738	1

class CAudioCodecAK7738 :
	public CAudioCodec
{
public:
	CAudioCodecAK7738(CCommanderEx *pCommander,HWND hWnd,AspType aspType=AspTypeAK7738);
	~CAudioCodecAK7738(void);

protected:
	BOOL codec_Ini();
	BOOL codec_IsCodecConfigFinished();
	BOOL codec_MuteAMP(BOOL bMute);
	BOOL codec_MuteCodec(BOOL bMute);
	BOOL codec_SetMainVol(int vol);
	BOOL codec_SetAltVol(int vol);
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
	void AddCRamBalFad(CRamParam &pParam);
	void RemoveCRamBalFad(BYTE index);
	void InitCRamBalFad(CRamParam* pParam, size_t count);
	void DeleteCRamBalFad();
private:
	CAspAK7738*		m_pAspAK7738;
	CMapCRam_Vol	m_MapCRamBalFad;
	AspHFStatus		m_aspHFStatus;

	int				m_iAltVol;
	int				m_iMainVol;

	float			m_fCurEQBass;
	float			m_fCurEQMid;
	float			m_fCurEQTre;

	BOOL			m_bInit;
	BOOL			m_bMuteCodec;
};

