#include "AudioCodecAK7738.h"
#include "./Soc/AspAK7738_dsp2_ctrl.h"
#include "./Soc/AspAK7738_dsp1_addr.h"
#include "./Soc/AspAK7738_dsp2_addr.h"
#include "../Common/ui_common.h"
#include "math.h"


#define  BALANCE_FADER_CENTER_POINT_X   7
#define  BALANCE_FADER_CENTER_POINT_Y	7

CAudioCodecAK7738::CAudioCodecAK7738(CCommanderEx *pCommander,HWND hWnd,AspType aspType)
:CAudioCodec(pCommander,hWnd,aspType)
{
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));
	m_pAspAK7738 = NULL;
	m_iAltVol = 0;
	m_iMainVol = 0;
	m_fCurEQBass = 0.0;
	m_fCurEQMid = 0.0;
	m_fCurEQTre = 0.0;
	m_aspHFStatus = AspHFStatusIdle;
	m_bInit = FALSE;
	m_bMuteCodec = FALSE;
}

CAudioCodecAK7738::~CAudioCodecAK7738(void)
{
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));

}

BOOL CAudioCodecAK7738::codec_Ini()
{
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));
	//Init Param
	#define GPIO_PDN_ID		((0*32) + 14) 
	#define SPI_DCB_CLK		(1000000)

	m_pAspAK7738 = new CAspAK7738;
	if (m_pAspAK7738 == NULL)
	{
		return FALSE;
	}
	InitCRamBalFad(ak7738CRamParam_BalFader, _countof(ak7738CRamParam_BalFader));

	m_pAspAK7738->setSamplingFreq(48000);	//48 KHz
	m_bInit = m_pAspAK7738->Initialize(IICPort1,SPIPort1,GPIO_PDN_ID,0,0,SPI_DCB_CLK);
	return m_bInit;
}

BOOL CAudioCodecAK7738::codec_IsCodecConfigFinished()
{
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));
	return m_bInit;
}
BOOL CAudioCodecAK7738::codec_MuteAMP(BOOL bMute)
{
	if (m_pCommander == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: bMute:%d \r\n"),TEXT(__FUNCTION__), bMute));

	if (bMute)
	{
		m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+1);
		m_pCommander->SendCmd(_ARM_MUTE_AMP,0x10+1);
	}
	else
	{
		m_pCommander->SendCmd(_ARM_MUTE_ALL,0x10+0);
		m_pCommander->SendCmd(_ARM_MUTE_AMP,0x10+0);
	}

	return TRUE;
}
BOOL CAudioCodecAK7738::codec_MuteCodec(BOOL bMute)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	RETAILMSG(1, (TEXT("MSG:[%s] bMute=%d\r\n"), TEXT(__FUNCTION__),bMute));
	m_bMuteCodec = bMute;
	BYTE param[3];
	if (bMute)
	{
		param[0] = 0x10;
		param[1] = 0x00;
		param[2] = 0x00;
	}
	else
	{
		param[0] = 0x00;
		param[1] = 0x00;
		param[2] = 0x00;		
	}

	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_MUTE_MuteOut2,param, 3));
	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_MUTE_MuteOut2,NULL,0));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));

	Sleep(1);

	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_MUTE_MuteOut5,param, 3));
	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_MUTE_MuteOut5,NULL,0));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetMainVol(int vol)//0-64
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	m_iMainVol = vol;

	if (m_aspHFStatus == AspHFStatusTalking)
	{
		RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: DSP1 vol:%d\r\n"),TEXT(__FUNCTION__),vol));

		BYTE param[3];
		param[0] = 0x00;
		param[1] = vol;
		param[2] = 0x00;
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_OUTPUT_VOLUME,param, sizeof(param)));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_OUTPUT_VOLUME,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
	}
	else
	{
		RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: DSP2 vol:%d\r\n"),TEXT(__FUNCTION__),vol));

		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_Vol,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[m_iMainVol]), m_pAspAK7738->getVolumeSettingSize()));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_Vol,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
	}

	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetAltVol(int vol)//0-64
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: vol:%d \r\n"),TEXT(__FUNCTION__), vol));

	m_iAltVol = vol;
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN1_MIXER2_1,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[m_iAltVol]), m_pAspAK7738->getVolumeSettingSize()));
	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN1_MIXER2_1,NULL,0));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));

	Sleep(1);
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN1_MIXER2_2,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[m_iAltVol]), m_pAspAK7738->getVolumeSettingSize()));
	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN1_MIXER2_2,NULL,0));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));

	return TRUE;
}
BOOL CAudioCodecAK7738::codec_MuteRearSpeakers(BOOL bMute)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetAGC(BOOL bEnbale)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetSampleRate(int iSampleRate)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetMicVolume(int vol)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetEQBand(int iBand,int val)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetEQPreset(BYTE idPresetType)
{
	/*
	EQ_NONE					0
	EQ_CLASSICAL			1
	EQ_POP					2
	EQ_ROCK					3
	EQ_JAZZ					4
	EQ_USER					5
	*/
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	return TRUE;
}

BOOL CAudioCodecAK7738::codec_SetBalFad(BYTE bal,BYTE fad) //0-14
{
	//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: balance[%d] fader[%d]\r\n"),TEXT(__FUNCTION__), bal,fad));
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	//printf("bal:%d fad:%d\r\n",bal,fad);

	m_iBalance = bal;
	m_iFade = fad;

	int FL;	//int(sqrt(square(m_iBalance)+square(14-m_iFade)));	
	int FR;	//int(sqrt(square(14-m_iBalance)+square(14-m_iFade)));	
	int RL;	//int(sqrt(square(m_iBalance)+square(m_iFade)));	
	int RR;	//int(sqrt(square(14-m_iBalance)+square(m_iFade)));
	
	//Origin
	if (m_iBalance == BALANCE_FADER_CENTER_POINT_X)
	{
		if (m_iFade >= BALANCE_FADER_CENTER_POINT_Y)
		{
			FL = 0;
			FR = 0;
			RL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			RR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		}
		else
		{
			FL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			FR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			RL = 0;
			RR = 0;
		}
	}
	else if (m_iFade == BALANCE_FADER_CENTER_POINT_Y)
	{
		if (m_iBalance >= BALANCE_FADER_CENTER_POINT_X)
		{
			FL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			FR = 0;
			RL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			RR = 0;
		}
		else
		{
			FL = 0;
			FR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			RL = 0;
			RR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		}
	}
	//First Quadrant
	else if (m_iBalance > BALANCE_FADER_CENTER_POINT_X && m_iFade > BALANCE_FADER_CENTER_POINT_Y)
	{
		FL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		FR = 0;
		RL = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);		
	}
	//Second Quadrant
	else if (m_iBalance < BALANCE_FADER_CENTER_POINT_X && m_iFade > BALANCE_FADER_CENTER_POINT_Y)	
	{
		FL = 0;
		FR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		RL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RR = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);		
	}
	//Third Quadrant
	else if (m_iBalance < BALANCE_FADER_CENTER_POINT_X && m_iFade < BALANCE_FADER_CENTER_POINT_Y)
	{
		FL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		FR = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RL = 0;
		RR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);		
	}
	//Fourth Quadrant
	else 
	{
		FL = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		FR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		RR = 0;	
	}

	if (FL > 7)		FL = 7;
	if (FR > 7)		FR = 7;
	if (RL > 7)		RL = 7;
	if (RR > 7)		RR = 7;

	if (FL < 0)		FL = 0;
	if (FR < 0)		FR = 0;
	if (RL < 0)		RL = 0;
	if (RR < 0)		RR = 0;

	//printf("MSG:[%s]: FL:%d, FR:%d, RL:%d, RR:%d\r\n",TEXT(__FUNCTION__),FL,FR,RL,RR);

	//FL
	CMapCRam_Vol::iterator Para;
	Para = m_MapCRamBalFad.find( FL );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_FL,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_FL,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: FL [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//FR
	Para = m_MapCRamBalFad.find( FR );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_FR,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_FR,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: FR [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//RL
	Para = m_MapCRamBalFad.find( RL );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_RL,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_RL,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: RL [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//RR
	Para = m_MapCRamBalFad.find( RR );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_RR,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_RR,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: RR [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_BALANCE,bal);
	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_FAD,fad);

	return TRUE;	
}
BOOL CAudioCodecAK7738::codec_Set3DEffect(BYTE val) //0-10
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	return TRUE;
}

BOOL CAudioCodecAK7738::codec_SetBalance(BYTE bal) //0,14
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: bal:%d \r\n"),TEXT(__FUNCTION__), bal));
	//printf("bal:%d fad:%d\r\n",bal,fad);

	m_iBalance = bal;

	int FL;	//int(sqrt(square(m_iBalance)+square(14-m_iFade)));	
	int FR;	//int(sqrt(square(14-m_iBalance)+square(14-m_iFade)));	
	int RL;	//int(sqrt(square(m_iBalance)+square(m_iFade)));	
	int RR;	//int(sqrt(square(14-m_iBalance)+square(m_iFade)));

	//Origin
	if (m_iBalance == BALANCE_FADER_CENTER_POINT_X)
	{
		if (m_iFade >= BALANCE_FADER_CENTER_POINT_Y)
		{
			FL = 0;
			FR = 0;
			RL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			RR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		}
		else
		{
			FL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			FR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			RL = 0;
			RR = 0;
		}
	}
	else if (m_iFade == BALANCE_FADER_CENTER_POINT_Y)
	{
		if (m_iBalance >= BALANCE_FADER_CENTER_POINT_X)
		{
			FL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			FR = 0;
			RL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			RR = 0;
		}
		else
		{
			FL = 0;
			FR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			RL = 0;
			RR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		}
	}
	//First Quadrant
	else if (m_iBalance > BALANCE_FADER_CENTER_POINT_X && m_iFade > BALANCE_FADER_CENTER_POINT_Y)
	{
		FL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		FR = 0;
		RL = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);		
	}
	//Second Quadrant
	else if (m_iBalance < BALANCE_FADER_CENTER_POINT_X && m_iFade > BALANCE_FADER_CENTER_POINT_Y)	
	{
		FL = 0;
		FR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		RL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RR = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);		
	}
	//Third Quadrant
	else if (m_iBalance < BALANCE_FADER_CENTER_POINT_X && m_iFade < BALANCE_FADER_CENTER_POINT_Y)
	{
		FL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		FR = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RL = 0;
		RR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);		
	}
	//Fourth Quadrant
	else 
	{
		FL = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		FR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		RR = 0;	
	}

	if (FL > 7)		FL = 7;
	if (FR > 7)		FR = 7;
	if (RL > 7)		RL = 7;
	if (RR > 7)		RR = 7;

	if (FL < 0)		FL = 0;
	if (FR < 0)		FR = 0;
	if (RL < 0)		RL = 0;
	if (RR < 0)		RR = 0;
	
	//FL
	CMapCRam_Vol::iterator Para;
	Para = m_MapCRamBalFad.find( FL );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_FL,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_FL,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: FL [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//FR
	Para = m_MapCRamBalFad.find( FR );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_FR,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_FR,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: FR [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//RL
	Para = m_MapCRamBalFad.find( RL );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_RL,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_RL,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: RL [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//RR
	Para = m_MapCRamBalFad.find( RR );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_RR,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_RR,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: RR [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_BALANCE,bal);

	return TRUE;	

}
BOOL CAudioCodecAK7738::codec_SetFade(BYTE fad) //0-14
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	m_iFade = fad;

	int FL;	//int(sqrt(square(m_iBalance)+square(14-m_iFade)));	
	int FR;	//int(sqrt(square(14-m_iBalance)+square(14-m_iFade)));	
	int RL;	//int(sqrt(square(m_iBalance)+square(m_iFade)));	
	int RR;	//int(sqrt(square(14-m_iBalance)+square(m_iFade)));

	//Origin
	if (m_iBalance == BALANCE_FADER_CENTER_POINT_X)
	{
		if (m_iFade >= BALANCE_FADER_CENTER_POINT_Y)
		{
			FL = 0;
			FR = 0;
			RL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			RR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		}
		else
		{
			FL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			FR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
			RL = 0;
			RR = 0;
		}
	}
	else if (m_iFade == BALANCE_FADER_CENTER_POINT_Y)
	{
		if (m_iBalance >= BALANCE_FADER_CENTER_POINT_X)
		{
			FL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			FR = 0;
			RL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			RR = 0;
		}
		else
		{
			FL = 0;
			FR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
			RL = 0;
			RR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		}
	}
	//First Quadrant
	else if (m_iBalance > BALANCE_FADER_CENTER_POINT_X && m_iFade > BALANCE_FADER_CENTER_POINT_Y)
	{
		FL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		FR = 0;
		RL = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);		
	}
	//Second Quadrant
	else if (m_iBalance < BALANCE_FADER_CENTER_POINT_X && m_iFade > BALANCE_FADER_CENTER_POINT_Y)	
	{
		FL = 0;
		FR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		RL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RR = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);		
	}
	//Third Quadrant
	else if (m_iBalance < BALANCE_FADER_CENTER_POINT_X && m_iFade < BALANCE_FADER_CENTER_POINT_Y)
	{
		FL = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		FR = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RL = 0;
		RR = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);		
	}
	//Fourth Quadrant
	else 
	{
		FL = GetNearInt(sqrt(square(abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X))+square(abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y))));	//abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X) + abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		FR = abs(m_iFade - BALANCE_FADER_CENTER_POINT_Y);
		RL = abs(m_iBalance - BALANCE_FADER_CENTER_POINT_X);
		RR = 0;	
	}

	if (FL > 7)		FL = 7;
	if (FR > 7)		FR = 7;
	if (RL > 7)		RL = 7;
	if (RR > 7)		RR = 7;

	if (FL < 0)		FL = 0;
	if (FR < 0)		FR = 0;
	if (RL < 0)		RL = 0;
	if (RR < 0)		RR = 0;
	
	//FL
	CMapCRam_Vol::iterator Para;
	Para = m_MapCRamBalFad.find( FL );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_FL,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_FL,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: FL [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//FR
	Para = m_MapCRamBalFad.find( FR );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_FR,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_FR,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: FR [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//RL
	Para = m_MapCRamBalFad.find( RL );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_RL,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_RL,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: RL [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	//RR
	Para = m_MapCRamBalFad.find( RR );
	if ( Para != m_MapCRamBalFad.end() )
	{
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_FADER_RR,Para->second.param, 3));
		//FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_FADER_RR,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: RR [0x%02X 0x%02X 0x%02X]\r\n"),TEXT(__FUNCTION__), Para->second.param[0],Para->second.param[1],Para->second.param[2]));
	}
	Sleep(1);

	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_FAD,fad);
	return TRUE;	
}
BOOL CAudioCodecAK7738::codec_SelectAudioSrc(UINT idSrc)
{
	//enable MCU audio mixer if current source is speech
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	switch (idSrc)
	{
	case SRC_AUDIO_RADIO:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: Radio\r\n"), TEXT(__FUNCTION__)));
			BYTE data[3] = {0x00, 0x00, 0x20};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
		break;
	case SRC_AUDIO_ARM:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: Media\r\n"), TEXT(__FUNCTION__)));
			BYTE data[3] = {0x00, 0x00, 0x10};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
		break;
	case SRC_AUDIO_AUX1:
	case SRC_AUDIO_AUX2:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: Aux\r\n"), TEXT(__FUNCTION__)));

			BYTE param = 0x16;
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_66_AIN_FILTER,&param, sizeof(param)));

			BYTE data[3] = {0x00, 0x00, 0x40};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
		break;
	case SRC_AUDIO_PHONE:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: Phone\r\n"), TEXT(__FUNCTION__)));
			BYTE data[3] = {0x00, 0x00, 0x10};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
		break;

	case SRC_AUDIO_DAB:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: DAB\r\n"), TEXT(__FUNCTION__)));

			BYTE data[3] = {0x00, 0x00, 0x20};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
		break;
	case SRC_AUDIO_DVD:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: DVD\r\n"), TEXT(__FUNCTION__)));

			BYTE data[3] = {0x00, 0x00, 0x40};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));	
		}
		break;
	case SRC_AUDIO_TV:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: TV\r\n"), TEXT(__FUNCTION__)));
			
			BYTE param = 0x17;
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_66_AIN_FILTER,&param, sizeof(param)));

			BYTE data[3] = {0x00, 0x00, 0x40};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));	
		}	
		break;
	case SRC_AUDIO_SR:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: SR\r\n"), TEXT(__FUNCTION__)));
			
			BYTE data[3] = {0x00, 0x00, 0x10};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
			
		}
		break;
	case SRC_AUDIO_SPEECH:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: Speech\r\n"), TEXT(__FUNCTION__)));

			/*
			BYTE data[3] = {0x00, 0x00, 0x10};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
			*/
		}
		break;
	default:
		{
			RETAILMSG(1,(TEXT("MSG:[%s]: Media\r\n"), TEXT(__FUNCTION__)));

			BYTE data[3] = {0x00, 0x00, 0x10};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,data, sizeof(data)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_SWITCH82_SWITCH8in2out_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
		break;
	}

	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetLoudness(BOOL bOn)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: bOn:%d \r\n"),TEXT(__FUNCTION__), bOn));

	if (bOn)
	{
		BYTE data[3] = {0x00, 0x00, 0x00};
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LOUDNESS_ENABLE_Loudness,data, sizeof(data)));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LOUDNESS_ENABLE_Loudness,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
	}
	else
	{
		BYTE data[3] = {0x10, 0x00, 0x10};
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LOUDNESS_ENABLE_Loudness,data, sizeof(data)));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LOUDNESS_ENABLE_Loudness,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));

	}
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetSubwoofer(BYTE val) //0-14
{
	if (m_pCommander == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));
	return m_pCommander ? m_pCommander->SendCmd(_ARM_SUBWOOFER_KEY,BYTE(val)) : FALSE;
}

BOOL CAudioCodecAK7738::codec_SetBass(BYTE val) //0-14
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: val:%d\r\n"),TEXT(__FUNCTION__),val));

	m_fCurEQBass = ak7738EQParam[val];	//val - 7.0;

	//float damp = max(max(m_fCurEQBass,m_fCurEQMid),m_fCurEQTre);
	//if(damp > 0)
	//{
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN, CRAM_ADDR_LEVEL_Headroom, m_pAspAK7738->getVolumeSettingBuff(0.0-damp), m_pAspAK7738->getVolumeSettingSize()));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_LEVEL_Headroom, NULL, 0));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	//}
	//else
	//{
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN, CRAM_ADDR_LEVEL_Headroom, m_pAspAK7738->getVolumeSettingBuff(0), m_pAspAK7738->getVolumeSettingSize()));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_LEVEL_Headroom, NULL, 0));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	//}

	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN+9,	CRAM_ADDR_BAND1_Bass_Mid_Tre, m_pAspAK7738->getEQSettingBuff(100, m_fCurEQBass, TRUE), m_pAspAK7738->getEQSettingSize(TRUE)));
	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_BAND1_Bass_Mid_Tre, NULL, 0));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,	0, NULL, 0));

	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetMid(BYTE val) //0-14
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: val:%d\r\n"),TEXT(__FUNCTION__),val));

	m_fCurEQMid = ak7738EQParam[val];	//val - 7.0;

	//float damp = max(max(m_fCurEQBass,m_fCurEQMid),m_fCurEQTre);
	//if(damp > 0)
	//{
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN, CRAM_ADDR_LEVEL_Headroom, m_pAspAK7738->getVolumeSettingBuff(0.0-damp), m_pAspAK7738->getVolumeSettingSize()));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_LEVEL_Headroom, NULL, 0));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	//}
	//else
	//{
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN, CRAM_ADDR_LEVEL_Headroom, m_pAspAK7738->getVolumeSettingBuff(0), m_pAspAK7738->getVolumeSettingSize()));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_LEVEL_Headroom, NULL, 0));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	//}

	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN+9, CRAM_ADDR_BAND2_Bass_Mid_Tre, m_pAspAK7738->getEQSettingBuff(1000, m_fCurEQMid, TRUE), m_pAspAK7738->getEQSettingSize(TRUE)));
	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_BAND2_Bass_Mid_Tre, NULL, 0));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	return TRUE;
}

BOOL CAudioCodecAK7738::codec_SetTreble(BYTE val) //0-14
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: val:%d\r\n"),TEXT(__FUNCTION__),val));

	m_fCurEQTre = ak7738EQParam[val];	//val - 7.0;

	//float damp = max(max(m_fCurEQBass,m_fCurEQMid),m_fCurEQTre);
	//if(damp > 0)
	//{
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN, CRAM_ADDR_LEVEL_Headroom, m_pAspAK7738->getVolumeSettingBuff(0.0-damp), m_pAspAK7738->getVolumeSettingSize()));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_LEVEL_Headroom, NULL, 0));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	//}
	//else
	//{
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN, CRAM_ADDR_LEVEL_Headroom, m_pAspAK7738->getVolumeSettingBuff(0), m_pAspAK7738->getVolumeSettingSize()));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_LEVEL_Headroom, NULL, 0));
	//	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	//}

	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN+9, CRAM_ADDR_BAND3_Bass_Mid_Tre, m_pAspAK7738->getEQSettingBuff(10000, m_fCurEQTre, TRUE), m_pAspAK7738->getEQSettingSize(TRUE)));
	FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA, CRAM_ADDR_BAND3_Bass_Mid_Tre, NULL, 0));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2, 0, NULL, 0));
	return TRUE;
}
BOOL CAudioCodecAK7738::codec_SetMixerOn(BOOL bOn, BOOL bMuteMain)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}

	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: bOn:%d bMuteMain:%d\r\n"),TEXT(__FUNCTION__),bOn, bMuteMain));

	if (bOn)
	{
		if(bMuteMain)
		{
			//Mute Mixter1 Ch2
			BYTE param[3] = {0x00,0x00,0x00};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN2_MIXER2_1,param, 3));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN2_MIXER2_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
			Sleep(5);
				
			//Mute Mixter2 Ch2
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN2_MIXER2_2,param, 3));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN2_MIXER2_2,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));		
			Sleep(1);

			//Mute MuteOut5
			BYTE paramMute[3] = {0x10,0x00,0x00};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_MUTE_MuteOut5,paramMute, 3));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_MUTE_MuteOut5,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
		else
		{
			//Set Mixter1 Ch2 to speech vol by default.
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN2_MIXER2_1,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[7]), 3));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN2_MIXER2_1,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
			Sleep(1);

			//Set Mixter2 Ch2 to speech vol by default.
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN2_MIXER2_2,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[7]), 3));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN2_MIXER2_2,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
			Sleep(1);

			//Unmute MuteOut5
			if (!m_bMuteCodec)
			{
				BYTE paramMute[3] = {0x00,0x00,0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_MUTE_MuteOut5,paramMute, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_MUTE_MuteOut5,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
			}
		}

		//Set Mixter1 ch1 to alt vol
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN1_MIXER2_1,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[m_iAltVol]), m_pAspAK7738->getVolumeSettingSize()));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN1_MIXER2_1,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		Sleep(1);

		//Set Mixter2 ch1 to alt vol
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN1_MIXER2_2,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[m_iAltVol]), m_pAspAK7738->getVolumeSettingSize()));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN1_MIXER2_2,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		Sleep(1);

		//Set Vol2  to speech vol by default.
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_Vol2,m_pAspAK7738->getVolumeSettingBuff(ak7738VolStepToDB[9]), 3));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_Vol2,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
	}
	else
	{
		//Close Mixter1 Ch1
		BYTE param[3] = {0x00,0x00,0x00};
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN1_MIXER2_1,param, 3));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN1_MIXER2_1,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		Sleep(1);

		//Close Mixter2 Ch1
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN1_MIXER2_2,param, 3));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN1_MIXER2_2,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		Sleep(1);

		//Reset Mixter1 Ch2 to default vol.
		BYTE param2[3] = {0x20,0x00,0x00};
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN2_MIXER2_1,param2, 3));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN2_MIXER2_1,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		Sleep(1);

		//Reset Mixter2 Ch2 to default vol.
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_VOL_IN2_MIXER2_2,param2, 3));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_VOL_IN2_MIXER2_2,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		Sleep(1);

		//Reset Vol2 to default vol. 
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_LEVEL_Vol2,m_pAspAK7738->getVolumeSettingBuff(0.0), 3));
		FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_LEVEL_Vol2,NULL,0));
		FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		Sleep(1);

		//Unmute MuteOut5
		if (!m_bMuteCodec)
		{
			BYTE paramMute[3] = {0x00,0x00,0x00};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_MUTE_MuteOut5,paramMute, 3));
			FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_MUTE_MuteOut5,NULL,0));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP2,0,NULL,0));
		}
	}

	return TRUE;
}

BOOL CAudioCodecAK7738::codec_ChangeWorkMode(AspMode mode)
{
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: mode:%d\r\n"),TEXT(__FUNCTION__),mode));

	//Set DSP1 CRAM to Hold Mode
	BYTE holdmode[] = {0x00, 0x01, 0x00};
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN, CRAM_ADDR_DSP1_HOLD_MODE, holdmode, sizeof(holdmode)));
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));

	Sleep(1);

	//Reset DSP1 and SubDSP
	BYTE reset = 0x15;
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG, AK7738_85_RESET_CTRL, &reset, sizeof(reset)));

	//Set HF Voice Band-Wide (RCV)-1 
	//Set HF Voice Band-Wide (RCV)-2
	//Set Parameter Bank
	switch(mode)
	{
	case AspModeCM_WB: 
		{
			//fs=16k
			BYTE paramRCV1[] = {0xA0, 0x02, 0x00};
			BYTE paramRCV2[] = {0x08, 0xAF, 0X00};
			BYTE paramBank[] = {0x00, 0x01, 0x00};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV1,paramRCV1, sizeof(paramRCV1)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV2,paramRCV2, sizeof(paramRCV2)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_PBANK,paramBank, sizeof(paramBank)));
		}
		break;
	case AspModeCM_NB:
		{
			//fs=8k
			BYTE paramRCV1[] = {0xA0, 0x02, 0x00};
			BYTE paramRCV2[] = {0x08, 0xDF, 0X00};
			BYTE paramBank[] = {0x00, 0x02, 0x00};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV1,paramRCV1, sizeof(paramRCV1)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV2,paramRCV2, sizeof(paramRCV2)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_PBANK,paramBank, sizeof(paramBank)));
		}
		break;
	case AspModeCM_SB:
		{
			//fs=24k
			BYTE paramRCV1[] = {0xA0, 0x00, 0x00};
			BYTE paramRCV2[] = {0x08, 0xAF, 0X00};
			BYTE paramBank[] = {0x00, 0x01, 0x00};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV1,paramRCV1, sizeof(paramRCV1)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV2,paramRCV2, sizeof(paramRCV2)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_PBANK,paramBank, sizeof(paramBank)));
		}
		break;
	case AspModeMedia:
	default:
		{
			//fs=48k
			BYTE paramRCV1[] = {0xA0, 0x00, 0x00};
			BYTE paramRCV2[] = {0x08, 0xAF, 0X00};
			BYTE paramBank[] = {0x00, 0x03, 0x00};
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV1,paramRCV1, sizeof(paramRCV1)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_RCV2,paramRCV2, sizeof(paramRCV2)));
			FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_OPERATION_TO_CRAM_OF_DSP1, CRAM_ADDR_DSP1_PBANK,paramBank, sizeof(paramBank)));
		}
		break;
	}

	Sleep(1);

	//Power On DSP1 and SubDSP
	BYTE poweron = 0x1F;
	FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG, AK7738_85_RESET_CTRL, &poweron, sizeof(poweron)));

	return TRUE;
}

BOOL CAudioCodecAK7738::codec_SetHFStatus(AspHFStatus status)
{	
	if (m_pAspAK7738 == NULL || !m_bInit)
	{
		RETAILMSG(1,(_T("ERR:[%s]: Point is NULL or m_bInit:%d is FALSE!!!\r\n"),TEXT(__FUNCTION__), m_bInit));
		return FALSE;
	}
	//RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: status:%d\r\n"),TEXT(__FUNCTION__),status));

	if (m_aspHFStatus != status)
	{
		m_aspHFStatus = status;
		switch(status)
		{
		case AspHFStatusIdle:
			{
				RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: # AspHFStatusIdle #\r\n"),TEXT(__FUNCTION__)));
				//Fade Out
				BYTE paramOut[] = {0x00, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramOut, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
				Sleep(1);

				//Switch Media mode
				BYTE param1 = 0x13;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_29_DAC1_DATA_SELECT,&param1, sizeof(param1)));
				param1 = 0x14;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_2A_DAC2_DATA_SELECT,&param1, sizeof(param1)));
				Sleep(1);

				//Fade In
				BYTE paramIn[] = {0x08, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramIn, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
			}
			break;
		case AspHFStatusIncomingCall:
			{
				RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: # AspHFStatusIncomingCall #\r\n"),TEXT(__FUNCTION__)));
				//Fade Out
				BYTE paramOut[] = {0x00, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramOut, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));

				Sleep(1);

				//Switch Media mode
				BYTE param1 = 0x13;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_29_DAC1_DATA_SELECT,&param1, sizeof(param1)));
				param1 = 0x14;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_2A_DAC2_DATA_SELECT,&param1, sizeof(param1)));
				Sleep(1);

				//Fade In
				BYTE paramIn[] = {0x08, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramIn, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));

			}
			break;
		case AspHFStatusOutgoingCall:
			{
				RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: # AspHFStatusOutgoingCall #\r\n"),TEXT(__FUNCTION__)));
				//Fade Out
				BYTE paramOut[] = {0x00, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramOut, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
				Sleep(1);

				//Switch Media mode
				BYTE param1 = 0x13;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_29_DAC1_DATA_SELECT,&param1, sizeof(param1)));
				param1 = 0x14;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_2A_DAC2_DATA_SELECT,&param1, sizeof(param1)));
				Sleep(1);

				//Fade In
				BYTE paramIn[] = {0x08, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramIn, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
			}
			break;
		case AspHFStatusTalking:
			{
				RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: # AspHFStatusTalking #\r\n"),TEXT(__FUNCTION__)));
				//Fade Out
				BYTE paramOut[] = {0x00, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramOut, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));		
				Sleep(1);

				//Switch Phone mode
				BYTE param1 = 0x1F;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_29_DAC1_DATA_SELECT,&param1, sizeof(param1)));
				param1 = 0x00;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_2A_DAC2_DATA_SELECT,&param1, sizeof(param1)));

				BYTE init[] = {0x00, 0x01, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_HF_INIT,init, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_HF_INIT,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
				Sleep(1);
				
				//Fade In
				BYTE paramIn[] = {0x08, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramIn, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
			}
			break;
		case AspHFStatusTerminating:
			{
				RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: # AspHFStatusTerminating #\r\n"),TEXT(__FUNCTION__)));
				//Fade Out
				BYTE paramOut[] = {0x00, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramOut, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
				Sleep(1);

				//Switch Media mode
				BYTE param1 = 0x13;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_29_DAC1_DATA_SELECT,&param1, sizeof(param1)));
				param1 = 0x14;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_2A_DAC2_DATA_SELECT,&param1, sizeof(param1)));
				Sleep(1);

				//Fade In
				BYTE paramIn[] = {0x08, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramIn, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
			}
			break;
		case AspHFStatusNoHeldCall:
			{
				RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: # AspHFStatusNoHeldCall #\r\n"),TEXT(__FUNCTION__)));
				//Fade Out
				BYTE paramOut[] = {0x00, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramOut, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
				Sleep(1);

				//Switch Media mode
				BYTE param1 = 0x13;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_29_DAC1_DATA_SELECT,&param1, sizeof(param1)));
				param1 = 0x14;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_2A_DAC2_DATA_SELECT,&param1, sizeof(param1)));
				Sleep(1);

				//Fade In
				BYTE paramIn[] = {0x08, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramIn, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
			}
			break;
		case AspHFStatusHeldCallNoActiveCall:
			{
				RETAILMSG(DEBUG_AUDIO_CODEC_AK7738,(_T("MSG:[%s]: # AspHFStatusHeldCallNoActiveCall #\r\n"),TEXT(__FUNCTION__)));
				//Fade Out
				BYTE paramOut[] = {0x00, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramOut, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
				Sleep(1);

				//Switch Media mode
				BYTE param1 = 0x13;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_29_DAC1_DATA_SELECT,&param1, sizeof(param1)));
				param1 = 0x14;
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CONTROL_REG,AK7738_2A_DAC2_DATA_SELECT,&param1, sizeof(param1)));
				Sleep(1);

				//Fade In
				BYTE paramIn[] = {0x08, 0x00, 0x00};
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_CRAM_DURING_RUN,CRAM_ADDR_DSP1_FADE,paramIn, 3));
				FUNC_CHECK(m_pAspAK7738->write(CMD_READ_CRAM_WRITE_PRE_DATA,CRAM_ADDR_DSP1_FADE,NULL,0));
				FUNC_CHECK(m_pAspAK7738->write(CMD_WRITE_EXECUTION_TO_CRAM_OF_DSP1,0,NULL,0));
			}
			break;
		default:
			break;	 
		}
	}

	return TRUE;
}

void CAudioCodecAK7738::InitCRamBalFad(CRamParam* pParam, size_t count)
{
	for( size_t i = 0; i < count; i++ )
	{
		AddCRamBalFad(pParam[i]);
	}	
}

void CAudioCodecAK7738::AddCRamBalFad(CRamParam &pParam)
{
	RemoveCRamBalFad( pParam.index );
	m_MapCRamBalFad.insert( CPairCRam_Vol( pParam.index, pParam));
}

void CAudioCodecAK7738::RemoveCRamBalFad(BYTE index)
{
	CMapCRam_Vol::iterator Para;
	Para = m_MapCRamBalFad.find( index );
	if ( Para != m_MapCRamBalFad.end() )
	{
		m_MapCRamBalFad.erase(Para);
	}
}

void CAudioCodecAK7738::DeleteCRamBalFad()
{
	for (CMapCRam_Vol::iterator Para = m_MapCRamBalFad.begin(); Para != m_MapCRamBalFad.end(); Para++)
	{
		m_MapCRamBalFad.erase(Para);
	}
}