#pragma once

#include "../../Communication/CommanderEx.h"
#include <Asplib.h>
#include "../GUI/Settings/SettingCommon.h"

#define DEBUG_AUDIO_CODEC		1

#define AUDIO_DUCK_LEVEL_DEFAULT		5

#if 0
#define EQ_NONE				0
#define EQ_POP					1
#define EQ_CLASSICAL			2
#define EQ_ROCK					3
#define EQ_JAZZ					4
#define EQ_USER					5

enum AUDIO_SET_ITEM
{
	AUDIO_SET_VOLUME,
	AUDIO_SET_BASS,
	AUDIO_SET_SUBWOOFER,
	AUDIO_SET_TREBLE,
	AUDIO_SET_MID,
	AUDIO_SET_LOUDNESS,
	AUDIO_SET_MUTE,
	AUDIO_SET_BALANCE,
	AUDIO_SET_FAD,
	AUDIO_SET_EQ,
	AUDIO_SET_EQ_BAND1,
	AUDIO_SET_EQ_BAND2,
	AUDIO_SET_EQ_BAND3,
	AUDIO_SET_EQ_BAND4,
	AUDIO_SET_EQ_BAND5,
	AUDIO_SET_EQ_BAND6,	
	AUDIO_SET_EQ_BAND7,
	AUDIO_SET_EQ_BAND8,
	AUDIO_SET_EQ_BAND9,
	AUDIO_SET_EQ_BAND10,
	AUDIO_SET_SRC_VOL,
	AUDIO_SET_MIC_IN_VOL,
	AUDIO_SET_3D_EFFECT,
	AUDIO_SET_MAIN_VOLUME,
	AUDIO_SET_ALT_VOLUME,
};
#endif

enum AUDIO_REQUEST_STATUS
{
	AUDIO_REQUEST_SUCCEED,
	AUDIO_REQUEST_FAILED,
	AUDIO_REQUEST_PENDING
};

static const BYTE g_EQcmd[]={0x01, 0x00, 0x00, 0x2c, 0x01, 0x05,		0xAA,0xAA,0xAA,0xAA,};
static const double g_EQfreq[5]={63.0,250.0,1000.0,3000.0,10000.0};
// static const double g_EQfreq[5]={90.0,125.0,800.0,3200.0,12500.0};
static const int g_EQPreset[5][5]=
{
	{0,0,0,0,0},		//EQ_NONE
	{6,4,0,2,4},		//EQ_CLASSICAL
	{4,-2,0,4,-2},	//EQ_POP
	{-2,4,0,2,4},	//EQ_ROCK
	{-2,2,2,6,2},	//EQ_JAZZ
};

// enum AudioWorkMode{modeSpeech,modeSR,modePhone,modeAlert,modeMedia};
enum SpeechMode{SpeechNone,SpeechNavi,SpeechRecognition};

class CAudioCodec
{
public:
	CAudioCodec(CCommanderEx *pCommander,HWND hWnd,AspType aspType=AspTypeNull);
	~CAudioCodec(void);

	BOOL IniCodec();
	void OnSystemPowerState(UINT idState);
	BOOL IsCodecConfigFinished();
	BOOL IsCodecIni();
	void ChangeAspWorkMode(AspMode mode);
	AspMode GetAspWorkMode();
	void EnableDSPEffect(BOOL bEnable);
	BOOL Mute(BOOL bMute);
	BOOL SetVolume(int vol);
	BOOL MuteAMP(BOOL bMute);
	BOOL MuteRearSpeakers(BOOL bMute);

	BOOL MuteAltAudio(BOOL bMute);
	BOOL MuteMainAudio(BOOL bMute,BOOL bFadeOut=FALSE,BOOL bFadeIn=TRUE,DWORD iTimeOut=-1,int iSpeed=50);
	void OnMainAudioMuteTimeout();

	BOOL SetLoudness(BOOL bOn);
	BOOL SetSubwoofer(BYTE val);

	BOOL SetBass(BYTE val);
	BOOL SetMid(BYTE val);
	BOOL SetTreble(BYTE val);
	BOOL SetEQ(BYTE val);
	BOOL SetEQBand(int iBand,int val);

	BOOL SetBalance(BYTE val);
	BOOL SetFade(BYTE val);
	BOOL SetBalFad(BYTE bal,BYTE fad);

	BOOL Set3DEffect(BYTE val);

	BOOL SetMixerOn(BOOL bOn, BOOL bMuteMain=FALSE);

	//
	AUDIO_REQUEST_STATUS RequestAudioBus(UINT idSrc,UINT iParam=0);
	UINT ReleaseAudioBus(UINT idSrc);
	UINT GetCurrentAudioBus();

	//
	BOOL	GetMute();
	//	
	BOOL SetMainVolume(int vol, BOOL bSync=TRUE);
	BOOL SetAltVolume(int vol, BOOL bSync=TRUE);

	BOOL SetLimitVolume(BOOL bLimit,int vol=6);

	BOOL SetMainVolumeTarget(int vol,BOOL bSync=FALSE);
	int GetMainVolumeTarget();

	int GetMainVolume();
	int GetAltVolume();

	BOOL SetMicVolume(int vol);
	BOOL SetAGC(BOOL bEnbale);

	BOOL SetSampleRate(int iSampleRate);

	void SetUserEQ(DWORD dwUserEQ,BOOL bRefresh=TRUE);

	void SetVolumeSpeedRate(int iRate);
	void SetVehicleCurrentSpeed(int iKmh);
	void SetVehicleCalibrateSpeed(int iKmh);

	BOOL IsAudioVolumeProcessReady();

	BOOL SetHFStatus(AspHFStatus status);
protected:
	//interface:
	virtual BOOL codec_Ini()=0;
	virtual BOOL codec_IsCodecConfigFinished()=0;
	virtual BOOL codec_MuteAMP(BOOL bMute)=0;
	virtual BOOL codec_MuteCodec(BOOL bMute)=0;
	virtual BOOL codec_SetMainVol(int vol)=0;//0-64
	virtual BOOL codec_SetAltVol(int vol)=0;//0-64
	virtual BOOL codec_MuteRearSpeakers(BOOL bMute)=0;
	virtual BOOL codec_SetAGC(BOOL bEnbale)=0;
	virtual BOOL codec_SetSampleRate(int iSampleRate)=0;
	virtual BOOL codec_SetMicVolume(int vol)=0;
	virtual BOOL codec_SetEQBand(int iBand,int val)=0;
	virtual BOOL codec_SetEQPreset(BYTE idPresetType)=0;
	virtual BOOL codec_SetBalFad(BYTE bal,BYTE fad)=0; //0-14
	virtual BOOL codec_Set3DEffect(BYTE val)=0; //0-10
	virtual BOOL codec_SetBalance(BYTE bal)=0; //0-14
	virtual BOOL codec_SetFade(BYTE fad)=0; //0-14
	virtual BOOL codec_SelectAudioSrc(UINT idSrc)=0;
	virtual BOOL codec_SetLoudness(BOOL bOn)=0;
	virtual BOOL codec_SetSubwoofer(BYTE val)=0; //0-14
	virtual BOOL codec_SetBass(BYTE val)=0; //0-14
	virtual BOOL codec_SetMid(BYTE val)=0; //0-14
	virtual BOOL codec_SetTreble(BYTE val)=0; //0-14
	virtual BOOL codec_SetMixerOn(BOOL bOn, BOOL bMuteMain)=0;
	virtual BOOL codec_ChangeWorkMode(AspMode mode)=0;
	virtual BOOL codec_SetHFStatus(AspHFStatus status)=0;

private:
	void DuckMainAudio(BOOL bDuck, int iLevel=AUDIO_DUCK_LEVEL_DEFAULT,BOOL bFade=TRUE,int iSpeed=50);

private:
	static DWORD MainVolumeThreadPro(LPVOID lpParam);
	BOOL IsValidMcuSrc(UINT id);
	BOOL IniAsp();

	//
	BOOL	SelectAudioSrc(UINT idSrc);

protected:
	BOOL		m_bini;
	HWND		m_hwndNotify;
	CCommanderEx *m_pCommander;

	HASP	m_hAsp;
	AspType	m_aspType;
	AspMode m_aspMode;

	UINT	m_idCurSRC;
	UINT	m_idSRCBeforeRequestStack[32];
	UINT	m_idSRCRequestPending;
	UINT	m_idSRCRequestParam[256];
	INT		m_iSRCBeforeRequestStackDepth;

	BOOL	m_bDSPEffectEnabled;
	int		m_iEQType;
	int		m_i3dEffect;
	//
	DWORD	m_dwUserEQ;
	//
	BOOL	m_bMute;
	BOOL	m_bAltMute;
	int		m_iBalance;
	int		m_iFade;
	int		m_iVolMain;
	int		m_iVolAlt;
	BOOL	m_bDucked;
	int		m_iVolBeforeDucking;
	BOOL	m_bOnDuckingProcess;
	int		m_iDuckSpeed;

	int		m_iVolMic;

	int		m_iVolMainSet;

	int		m_iVolMainTarget;
	int		m_iVolAltTarget;
	int		m_iAudioDuckLevel;

	UINT	m_idMCUSRC;
	BYTE	m_AudioPriorityTable[256];
	BYTE	m_CurAudioPriority;

	int		m_iVolumeSpeedRate;
	int		m_iVehicleCurrentSpeed;
	int		m_iVehicleCalibrateSpeed;

	BOOL	m_bWaitingMainAudioUnmute;
	BOOL	m_bLimitVolume;
	int		m_iLimitVolume;
	int		m_iNormalVolume;
	BOOL	m_bFadeIn;
};

