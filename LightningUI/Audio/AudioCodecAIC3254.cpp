#include "AudioCodecAIC3254.h"

#include "../Common/ui_common.h"

#define GAIN_ADJUST_IN1	0
#define GAIN_ADJUST_IN2	0
#define GAIN_ADJUST_IN3	13

CAudioCodecAIC3254::CAudioCodecAIC3254(CCommanderEx *pCommander,HWND hWnd,AspType aspType)
:CAudioCodec(pCommander,hWnd,aspType)
{
	m_pCmdSender = NULL;

	memset(&m_inputRouting,0,sizeof(InputRouting));
	memset(&m_outputRouting,0,sizeof(OutputRouting));

	m_pBiquad[0] = new TBiquad(44,32,45,40);
	m_pBiquad[1] = new TBiquad(44,52,45,60);
	m_pBiquad[2] = new TBiquad(44,72,45,80);
	m_pBiquad[3] = new TBiquad(44,92,45,100);
	m_pBiquad[4] = new TBiquad(44,112,45,120);

}

CAudioCodecAIC3254::~CAudioCodecAIC3254(void)
{
	for (int i=0;i<5;i++)
	{
		if(m_pBiquad[i])
		{
			delete m_pBiquad[i];
			m_pBiquad[i]=NULL;
		}
	}
}

BOOL CAudioCodecAIC3254::codec_Ini()
{
	RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++%S++++\r\n"),__FUNCTION__));

	return ConfigAudioCodecAIC3254Pack((BYTE*)cmd_Adaptive_filter_ini,sizeof(cmd_Adaptive_filter_ini));
}
BOOL CAudioCodecAIC3254::codec_IsCodecConfigFinished()
{
	if(m_pCmdSender)
	{
		return m_pCmdSender->IsCmdListEmpty();
	}
	return TRUE;
}
BOOL CAudioCodecAIC3254::codec_MuteAMP(BOOL bMute)
{
	return m_pCommander ? m_pCommander->SendCmd(_ARM_MUTE_ALL,/*0x10+*/(bMute?1:0)) : FALSE;
}
BOOL CAudioCodecAIC3254::codec_MuteCodec(BOOL bMute)
{
	return TRUE;
}
BOOL CAudioCodecAIC3254::codec_SetMainVol(int vol)//-127 -> + 48 : -63.5db-+24db
{
	static int last_set_DAC=-1;
	const int tol=(127);
	int set_DAC;
	int vol_to_set=vol;
	BOOL bRet=FALSE;

	//-127 -> + 48 : -63.5db-+24db
	// 	const int tol=(127+48);
	set_DAC= -127 + tol - tol*(vol_to_set-64)*(vol_to_set-64)/64/64;
	if(set_DAC<=-127)
		set_DAC=-127;
	if(set_DAC>0)
		set_DAC=0;

	//for test: +18db if current source is phone-call taking, -12 db ring-tone
	if(m_idCurSRC == SRC_AUDIO_PHONE)
	{
		UINT idPhoneCallType = HIWORD(m_idSRCRequestParam[SRC_AUDIO_PHONE]);//0-bluetooth, 1-carplay
		UINT idCallPara=LOWORD(m_idSRCRequestParam[SRC_AUDIO_PHONE]);
		int vol_ringtone_adjust=-24; //-12db
		int vol_talking_adjust=m_hAsp?36:0; //if use hardware AEC, +18db

		if(idPhoneCallType == 1)//carplay phone-call
		{
			//ring-tone:
			if(idCallPara == 0x01/*HF_STATE_INCOMING_CALL*/)
			{
				set_DAC+=vol_ringtone_adjust;
				if(set_DAC<=-127)
					set_DAC=-127;
			}	
			else
			{
				set_DAC+=vol_talking_adjust;
			}
		}
		else //bluetooth phone-call
		{
			//ring-tone:
			if(idCallPara == 0x01/*HF_STATE_INCOMING_CALL*/)
			{
				set_DAC+=vol_ringtone_adjust;
				if(set_DAC<=-127)
					set_DAC=-127;
			}	
		}
	}

	if(last_set_DAC != set_DAC)
	{
		BYTE buff[]={0x00,0x00, 0x41, set_DAC, 0x42, set_DAC};
		bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
		last_set_DAC = set_DAC;
	}

	return bRet;
}
BOOL CAudioCodecAIC3254::codec_SetAltVol(int vol)//117->0: 117=mute,116 = -72.3db, 0=0db
{
	static int last_set_IN1=-1;
	const int tol=117;//32;//32;//
	int set_IN1;
	int vol_to_set=vol;
	BOOL bRet=FALSE;

	set_IN1 = tol*(64-vol_to_set)*(64-vol_to_set)*(64-vol_to_set)/64/64/64;

	if(set_IN1>=117)
		set_IN1=116;//117; //116 = -72.3db, 117=mute
	if(set_IN1<0)
		set_IN1=0;

	if(last_set_IN1 != set_IN1)
	{
		BYTE buff[]={0x00,0x01, 0x16, set_IN1, 0x17, set_IN1};
		 bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
		last_set_IN1 = set_IN1;
	}

	return bRet;
}
BOOL CAudioCodecAIC3254::codec_MuteRearSpeakers(BOOL bMute)
{
	if(bMute)
	{
		BYTE buff[]={0x00,0x01, 0x12, 0x40, 0x13, 0x40};
		return ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
	}
	else
	{
		return SetBalFad(m_iBalance,m_iFade);
	}
}
BOOL CAudioCodecAIC3254::codec_SetAGC(BOOL bEnbale)
{
	BOOL bRet=FALSE;
	if(bEnbale)
	{
		BYTE buff[]={0x00,0x00, 0x56, 0x80, 0x57,0x30, 0x58,0x3c, 0x59,0x18, 0x5a,0x18, 0x5b,0x03, 0x5c,0x03,
			0x5e, 0x80, 0x5f,0x30, 0x60,0x3c, 0x61,0x18, 0x62,0x18, 0x63,0x03, 0x64,0x03,};
		bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
	}
	else
	{
		BYTE buff[]={0x00,0x00, 0x56, 0x00, 0x5e, 0x00};
		bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
	}

	return bRet;
}
BOOL CAudioCodecAIC3254::codec_SetSampleRate(int iSampleRate)
{
	if(!m_bini)
		return FALSE;

	BOOL bRet=FALSE;

	switch (iSampleRate)
	{
	case 8000:
		{
			//DOSR=768
			BYTE buff[]={0x00,0x00, 0x0d, 0x03, 0x0e, 0x00};
			bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
		}
		break;
	case 16000:
		{
			//DOSR=384
			BYTE buff[]={0x00,0x00, 0x0d, 0x01, 0x0e, 0x80};
			bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
		}
		break;
	case 24000:
		{
			//DOSR=256
			BYTE buff[]={0x00,0x00, 0x0d, 0x01, 0x0e, 0x00};
			bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
		}
		break;
	default://48000
		{
			//DOSR=128
			BYTE buff[]={0x00,0x00, 0x0d, 0x00, 0x0e, 0x80};
			bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
		}
		break;
	}

	return bRet;
}
BOOL CAudioCodecAIC3254::codec_SetMicVolume(int vol)
{
	BOOL bRet=FALSE;
	if(m_iVolMic != vol)
	{
		BYTE buff[]={0x00,0x01, 0x3b, BYTE(vol), 0x3c, BYTE(vol)};
		bRet = ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
		m_iVolMic = vol;
	}

	return bRet;
}
BOOL CAudioCodecAIC3254::codec_SetEQBand(int iBand,int val)
{
	m_pBiquad[iBand]->CalcEQ(g_EQfreq[iBand],g_EQfreq[iBand]/3,44100,val,1);
	ConfigAudioCodecAIC3254Pack(m_pBiquad[iBand]->GetI2CData(),m_pBiquad[iBand]->GetI2CDataLength());
	ConfigAudioCodecAIC3254Pack((BYTE*)g_EQcmd,sizeof(g_EQcmd));
	Sleep(5);
	ConfigAudioCodecAIC3254Pack(m_pBiquad[iBand]->GetI2CData(),m_pBiquad[iBand]->GetI2CDataLength());

	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_EQ_BAND1+iBand,val);

	return TRUE;
}
BOOL CAudioCodecAIC3254::codec_SetEQPreset(BYTE idPresetType)
{
	m_iEQType = idPresetType;

	if(idPresetType<5)
	{
		for (int i=0;i<5;i++)
		{
			//SetEQBand(i,g_EQPreset[val][i]);
			m_pBiquad[i]->CalcEQ(g_EQfreq[i],g_EQfreq[i]/3,44100,g_EQPreset[idPresetType][i],1);
			ConfigAudioCodecAIC3254Pack(m_pBiquad[i]->GetI2CData(),m_pBiquad[i]->GetI2CDataLength());
		}
		ConfigAudioCodecAIC3254Pack((BYTE*)g_EQcmd,sizeof(g_EQcmd));
		Sleep(5);
		for (int i=0;i<5;i++)
		{
			ConfigAudioCodecAIC3254Pack(m_pBiquad[i]->GetI2CData(),m_pBiquad[i]->GetI2CDataLength());

			//notify:
			::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_EQ_BAND1+i,g_EQPreset[idPresetType][i]);
		}

		::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_EQ,idPresetType);
	}
	else
	{
		//set user param:
		int set[5];

		set[0]=(int)(m_dwUserEQ&0x0F);
		set[1]=(int)((m_dwUserEQ&0x0F0)>>4);
		set[2]=(int)((m_dwUserEQ&0x0F00)>>8);
		set[3]=(int)((m_dwUserEQ&0x0F000)>>12);
		set[4]=(int)((m_dwUserEQ&0x0F0000)>>16);

		for (int i=0;i<5;i++)
		{
			//SetEQBand(i,g_EQPreset[val][i]);
			m_pBiquad[i]->CalcEQ(g_EQfreq[i],g_EQfreq[i]/3,44100,(set[i]-7)*2,1);
			ConfigAudioCodecAIC3254Pack(m_pBiquad[i]->GetI2CData(),m_pBiquad[i]->GetI2CDataLength());
		}
		ConfigAudioCodecAIC3254Pack((BYTE*)g_EQcmd,sizeof(g_EQcmd));
		Sleep(5);
		for (int i=0;i<5;i++)
		{
			ConfigAudioCodecAIC3254Pack(m_pBiquad[i]->GetI2CData(),m_pBiquad[i]->GetI2CDataLength());

			//notify:
			::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_EQ_BAND1+i,(set[i]-7)*2);
		}

		::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_EQ,idPresetType);
	}

	return TRUE;
}
BOOL CAudioCodecAIC3254::codec_SetBalFad(BYTE bal,BYTE fad) //0-14
{
	//int mid=15;

	m_iBalance = bal;
	m_iFade = fad;

	const int mid = 0;

	int FL= -7 + mid + sqrt(abs((14-m_iBalance)*(m_iFade)));// * 14 / 196;
	int FR= -7 + mid + sqrt(abs((m_iBalance)*(m_iFade)));// * 14 / 196;
	int RL= -7 + mid + sqrt(abs((14-m_iBalance)*(14-m_iFade)));// * 14 / 196;
	int RR= -7 + mid + sqrt(abs((m_iBalance)*(14-m_iFade)));// * 14 / 196;

	if(FL<=-7)FL=-6;
	if(FR<=-7)FR=-6;
	if(RL<=-7)RL=-6;
	if(RR<=-7)RR=-6;

	if(FL>29)FL=29;
	if(FR>29)FR=29;
	if(RL>29)RL=29;
	if(RR>29)RR=29;

	//RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodecAIC3254::SetBalFad: FL:%d, FR:%d, RL:%d, RR:%d\r\n"),FL,FR,RL,RR));

	if(FL<=-7)	FL=64;
	else if(FL<0)FL=64+FL;

	if(FR<=-7)	FR=64;
	else if(FR<0)FR=64+FR;

	if(RL<=-7)	RL=64;
	else if(RL<0)RL=64+RL;

	if(RR<=-7)RR=64;
	else if(RR<0)RR=64+RR;

	RETAILMSG(1/*DEBUG_AUDIO_CODEC*/,(_T("++++CAudioCodecAIC3254::SetBalFad: FL:%d, FR:%d, RL:%d, RR:%d\r\n"),FL,FR,RL,RR));

	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_BALANCE,bal);
	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_FAD,fad);

	BYTE buff[]={0x00,0x01, 0x10, (BYTE)FL, 0x11, (BYTE)FR, 0x12, (BYTE)RL, 0x13, (BYTE)RR};
	return ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
}
BOOL CAudioCodecAIC3254::codec_Set3DEffect(BYTE val) //0-10
{
	m_i3dEffect = val;

	int set=val*0x7FFFFF /10;

	BYTE b1=(set>>16)&(0xFF);
	BYTE b2=(set>>8)&(0xFF);
	BYTE b3=(set)&(0xFF);

	BYTE cmd_3d_effect[]={
		0x01,0x00,
		0x00,0x2D,		
		0x10,b1,0x11,b2,0x12,b3,0x13,0x00,		
		0x00,0x2C,0x01,0x05,		
		//f,0x30,0x01,0xxxxxx1x0,		0xAA,0xAA,0xAA,0xAA,
		0xAA,0xAA,0xAA,0xAA,//end

		0x01,0x00,
		0x00,0x2D,		
		0x10,b1,0x11,b2,0x12,b3,0x13,0x00,		
		0xAA,0xAA,0xAA,0xAA,//end
	};

	ConfigAudioCodecAIC3254Pack((BYTE*)cmd_3d_effect,sizeof(cmd_3d_effect));

	::PostMessage(m_hwndNotify,WM_AUDIO_CODEC_MSG,AUDIO_SET_3D_EFFECT,val);

	return TRUE;
}
BOOL CAudioCodecAIC3254::codec_SetBalance(BYTE bal) //0-14
{
	m_iBalance = bal;

	int FL=(7-m_iBalance)+(7-m_iFade);
	int FR=-(7-m_iBalance)+(7-m_iFade);
	int RL=(7-m_iBalance)-(7-m_iFade);
	int RR=-(7-m_iBalance)-(7-m_iFade);

	if(FL<-7)FL=-7;
	if(FR<-7)FR=-7;
	if(RL<-7)RL=-7;
	if(RR<-7)RR=-7;

	if(FL>7)FL=7;
	if(FR>7)FR=7;
	if(RL>7)RL=7;
	if(RR>7)RR=7;

	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodecAIC3254::SetBalance: FL:%d, FR:%d, RL:%d, RR:%d\r\n"),FL,FR,RL,RR));

	if(FL<=-7)	FL=64;
	else if(FL<0)FL=64+FL;

	if(FR<=-7)	FR=64;
	else if(FR<0)FR=64+FR;

	if(RL<=-7)	RL=64;
	else if(RL<0)RL=64+RL;

	if(RR<=-7)RR=64;
	else if(RR<0)RR=64+RR;

	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodecAIC3254::SetBalance2: FL:%d, FR:%d, RL:%d, RR:%d\r\n"),FL,FR,RL,RR));

	BYTE buff[]={0x00,0x01, 0x10, (BYTE)FL, 0x11, (BYTE)FR, 0x12, (BYTE)RL, 0x13, (BYTE)RR};
	return ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
}
BOOL CAudioCodecAIC3254::codec_SetFade(BYTE fad) //0-14
{
	m_iFade = fad;

	int FL=(7-m_iBalance)+(7-m_iFade);
	int FR=-(7-m_iBalance)+(7-m_iFade);
	int RL=(7-m_iBalance)-(7-m_iFade);
	int RR=-(7-m_iBalance)-(7-m_iFade);

	if(FL<-7)FL=-7;
	if(FR<-7)FR=-7;
	if(RL<-7)RL=-7;
	if(RR<-7)RR=-7;

	if(FL>7)FL=7;
	if(FR>7)FR=7;
	if(RL>7)RL=7;
	if(RR>7)RR=7;

	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodecAIC3254::SetFade: FL:%d, FR:%d, RL:%d, RR:%d\r\n"),FL,FR,RL,RR));

	if(FL<=-7)	FL=64;
	else if(FL<0)FL=64+FL;

	if(FR<=-7)	FR=64;
	else if(FR<0)FR=64+FR;

	if(RL<=-7)	RL=64;
	else if(RL<0)RL=64+RL;

	if(RR<=-7)RR=64;
	else if(RR<0)RR=64+RR;

	RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++CAudioCodecAIC3254::SetFade2: FL:%d, FR:%d, RL:%d, RR:%d\r\n"),FL,FR,RL,RR));

	BYTE buff[]={0x00,0x01, 0x10, (BYTE)FL, 0x11, (BYTE)FR, 0x12, (BYTE)RL, 0x13, (BYTE)RR};
	return ConfigAudioCodecAIC3254SingleLine(1,buff,sizeof(buff));
}
BOOL CAudioCodecAIC3254::codec_SelectAudioSrc(UINT idSrc)
{
	switch (idSrc)
	{
		case SRC_AUDIO_RADIO:
			{
				m_inputRouting.ADC_IN1=FALSE;
				m_inputRouting.ADC_IN2=TRUE;
				m_inputRouting.ADC_IN3=FALSE;//mic
				m_inputRouting.DAC_IN=FALSE;
				SetMicVolume(0);//0db
				UpdateInputRouting();
				if(m_hAsp)
				{
					AspChangeMode(m_hAsp,AspModeMedia);
				}
				EnableDSPEffect(TRUE);
			}
			break;
		case SRC_AUDIO_ARM:
			{
				m_inputRouting.ADC_IN1=FALSE;
				m_inputRouting.ADC_IN2=FALSE;
				m_inputRouting.ADC_IN3=FALSE;//mic
				m_inputRouting.DAC_IN=TRUE;
				UpdateInputRouting();
				if(m_hAsp)
				{
					AspChangeMode(m_hAsp,AspModeMedia);
				}
				EnableDSPEffect(TRUE);
			}
			break;
		case SRC_AUDIO_PHONE:
			{
				m_inputRouting.ADC_IN1=FALSE;
				m_inputRouting.ADC_IN2=FALSE;
				m_inputRouting.ADC_IN3=FALSE;//mic
				m_inputRouting.DAC_IN=TRUE;
				UpdateInputRouting();
				//not switch mode at once,
				//switch mode while CARPLAY_PHONE_AUDIO_FORMAT
				//message received:
				/*
				if(m_hAsp)
				{
					AspChangeMode(m_hAsp,AspModeCM_NB);
				}
				*/
				EnableDSPEffect(FALSE);
			}
			break;
		case SRC_AUDIO_SR:
			{
				m_inputRouting.ADC_IN1=FALSE;
				m_inputRouting.ADC_IN2=FALSE;
				m_inputRouting.ADC_IN3=FALSE;//mic
				m_inputRouting.DAC_IN=TRUE;
				UpdateInputRouting();
				//not switch mode at once,
				//switch mode while CARPLAY_SIRI_AUDIO_FORMAT
				//message received:
				if(m_hAsp)
				{
					AspChangeMode(m_hAsp,AspModeSR);
				}
			}
			break;
		case SRC_AUDIO_SPEECH:
		case SRC_AUDIO_ALERT:
			{
				m_inputRouting.ADC_IN1=TRUE;
				UpdateInputRouting();
			}
			break;
		default://use IN3
			{
				m_inputRouting.ADC_IN1=FALSE;
				m_inputRouting.ADC_IN2=FALSE;
				m_inputRouting.ADC_IN3=TRUE;
				m_inputRouting.DAC_IN=FALSE;
				UpdateInputRouting();
				if(GAIN_ADJUST_IN3!=0)
				{
					SetMicVolume(GAIN_ADJUST_IN3);
				}
				if(m_hAsp)
				{
					AspChangeMode(m_hAsp,AspModeMedia);
				}
				EnableDSPEffect(TRUE);
			}
			break;
	}

	return TRUE;
}
BOOL CAudioCodecAIC3254::codec_SetLoudness(BOOL bOn)
{
	return FALSE;
}
BOOL CAudioCodecAIC3254::codec_SetSubwoofer(BYTE val) //0-14
{
	return FALSE;
}
BOOL CAudioCodecAIC3254::codec_SetBass(BYTE val) //0-14
{
	return FALSE;
}
BOOL CAudioCodecAIC3254::codec_SetMid(BYTE val) //0-14
{
	return FALSE;
}
BOOL CAudioCodecAIC3254::codec_SetTreble(BYTE val) //0-14
{
	return FALSE;
}
BOOL CAudioCodecAIC3254::codec_SetMixerOn(BOOL bOn, BOOL bMuteMain)
{
	return TRUE;
}

BOOL CAudioCodecAIC3254::codec_ChangeWorkMode(AspMode mode)
{
	return TRUE;
}

BOOL CAudioCodecAIC3254::codec_SetHFStatus(AspHFStatus status)
{			
	return TRUE;
}

BOOL CAudioCodecAIC3254::ConfigAudioCodecAIC3254SingleLine(UINT idType,BYTE *buff, INT len,INT iDelayMs)
{
	if(!m_pCmdSender)
	{
		m_pCmdSender = new CmdSender_AudioCodec();
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



BOOL CAudioCodecAIC3254::IsSrcNeedMic(UINT id)
{
	return (id==SRC_AUDIO_SR)
		||( id==SRC_AUDIO_PHONE) ;
}
void CAudioCodecAIC3254::UpdateInputRouting()
{
	static BYTE s_val_left=-1,s_val_right=-1;
	static BYTE s_IN1=-1;

	BYTE val_left=0,val_right=0;

	BYTE cm_mode=0;

	if(m_inputRouting.ADC_IN1)
	{
// 		val_left |= (m_inputRouting.ADC_IN3?ADC_IN1_40K:ADC_IN1_10K);
// 		val_right |= (m_inputRouting.ADC_IN3?ADC_IN1_40K:ADC_IN1_10K);
// 		val_left |= ADC_IN1_10K;
// 		val_right |= ADC_IN1_10K;
	}
	else
	{
		//cm_mode |= 0xc0;
	}
	cm_mode |= 0xc0;

	if(m_inputRouting.ADC_IN2)
	{
// 		val_left |= (m_inputRouting.ADC_IN3?ADC_IN2_40K:ADC_IN2_10K);
// 		val_right |= (m_inputRouting.ADC_IN3?ADC_IN2_40K:ADC_IN2_10K);
		val_left |= ADC_IN2_10K;
		val_right |= ADC_IN2_10K;
	}
	else
	{
		cm_mode |= 0x30;
	}

	if(m_inputRouting.ADC_IN3)
	{
		val_left |= ADC_IN3_10K;
		val_right |= ADC_IN3_10K;
	}
	else
	{
		cm_mode |= 0x0c;
	}

	if(s_val_left != val_left || s_val_right != val_right)
	{
		s_val_left = val_left;
		s_val_right = val_right;

		RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++<%s>:0x%x,0x%x\r\n"),_T(__FUNCTION__),val_left,val_right));
		if(val_left || val_right)
		{
			//input:
			BYTE buf[]={
				0x00,0x01,0x34,0x0,0x37,0x0,   
				0x34,val_left,0x37,val_right,   
				//0x36,/*0x80*/0x40,0x39,/*0x80*/0x40,		
				//0x3a,cm_mode,
				0x00,0x00,	0x1d,m_inputRouting.DAC_IN?0x0:0x10,};

			ConfigAudioCodecAIC3254SingleLine(1,buf,sizeof(buf));
		}
		else
		{
			//input:
			BYTE buf[]={
				0x00,0x01,0x34,0x0,0x37,0x0,   
				//0x36,0x40,0x39,0x40,  
				//0x3a,cm_mode,
				0x00,0x00,	0x1d,0x0,};
			ConfigAudioCodecAIC3254SingleLine(1,buf,sizeof(buf));
		}
	}
}
void CAudioCodecAIC3254::UpdateOutputRouting()
{

}

BOOL CAudioCodecAIC3254::ConfigAudioCodecAIC3254Pack(BYTE *buffScript, INT len)
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


void TBiquad::Calc(double FC, double BW, double FS, double G, bool bRefresh)
{
	if (fabs(FS / 2.0 - FC) < 100.0) FC = FS / 2.0 - 100.0; // to prevent atan errors

	switch (mBQType)
	{
		case bqEQ: 	CalcEQ(FC, BW, FS, G, bRefresh); break;
		case bqHighPass:
		case bqLowPass:
			switch (mBQSubType)
			{
				case bqButterworth1:
					//CalcBW1(FC, FS, G, mBQType == bqHighPass, bRefresh); break;
				case bqButterworth2:
					CalcVQ2(FC, 0.707, FS, G, mBQType == bqHighPass, bRefresh); break;
				case bqBessel2:
					CalcVQ2(FC, 0.58, FS, G, mBQType == bqHighPass, bRefresh); break;
				case bqLinkwitzRiley2:
					CalcLR2(FC, FS, G, mBQType == bqHighPass, bRefresh); break;
				case bqVariableQ2:
					CalcVQ2(FC, mnQ, FS, G, mBQType == bqHighPass, bRefresh); break;
				default:;
			}
			break;
		case bqTrebleShelf:
			CalcShelf(FC, FS, G, true, bRefresh); break;
		case bqBassShelf:
			CalcShelf(FC, FS, G, false, bRefresh); break;
		case bqNotch:
			CalcNotch(FC, BW, FS, G, bRefresh); break;
		case bqPhaseShift:
			CalcPhaseShift(FC, BW, FS, bRefresh); break;
		default:;
	}
	Quantize();
}
void TBiquad::SetResolution(unsigned int nResolution)
{
	mnResolution = nResolution;
	Quantize();
}

UINT PackBuff(BYTE *pDest, BYTE *pSrc, UINT iLen, BYTE nI2CPage, BYTE nI2CRegister)
{
	UINT pack_len=0;

	pDest[pack_len++] = 0x01; //begin
	pDest[pack_len++] = 0x00; //delay

	pDest[pack_len++] = 0x00; //
	pDest[pack_len++] = nI2CPage; //switch page
	pDest[pack_len++] = 0xAA; //
	pDest[pack_len++] = 0xAA; //
	pDest[pack_len++] = 0xAA; //
	pDest[pack_len++] = 0xAA; //end

	pDest[pack_len++] = 0x00; //begin
	pDest[pack_len++] = 0x00; //delay

	pDest[pack_len++] = nI2CRegister; //

	for (UINT i=0;i<iLen;i++)
	{
		pDest[pack_len++] = pSrc[i];
	}

	pDest[pack_len++] = 0xAA; //
	pDest[pack_len++] = 0xAA; //
	pDest[pack_len++] = 0xAA; //
	pDest[pack_len++] = 0xAA; //end

	return pack_len;
}
void TBiquad::Quantize(void)
{
	double nMax;
	nMax = mb[0];
	double nScale = 1.0;
	for (unsigned int i = 1; i < 3; i++)
	{
		if (mb[i] > nMax)
			nMax = mb[i];
		if (nMax > 1.0)
			nScale = 1.0 / nMax;
	}

	long nRange = (2 << (mnResolution - 2)) - 1;
	mN[0] = long(nScale * mb[0] * nRange);
	mN[1] = long(nScale * (mb[1] / 2.0) * nRange);
	mN[2] = long(nScale * mb[2] * nRange);
	mD[1] = long(-(ma[1] / 2.0) * nRange);
	mD[2] = long(-ma[2] * nRange);

	mbQ[0] = mN[0] / (nScale * nRange);
	mbQ[1] = 2.0 * mN[1] / (nScale * nRange);
	mbQ[2] = mN[2] / (nScale * nRange);

	maQ[1] = -2.0 * mD[1] / nRange;
	maQ[2] = -1.0 * mD[2] / nRange;

	//RETAILMSG(DEBUG_AUDIO_CODEC,(_T("++++N[0] %X, N[1] %X, N[2] %X, D[1] %X, D[2] %X\r\n"),mN[0],mN[1],mN[2],mD[1],mD[2]));

	mnI2CDataLen=0;
	int pack_len = 0;
	unsigned char *p;
	BYTE buf[20];

	//Left Channel
	for (unsigned int i = 0; i < 3; i++)
	{
		p = (unsigned char *) &mN[i];
		buf[pack_len++] = p[2];
		buf[pack_len++] = p[1];
		buf[pack_len++] = p[0];
		buf[pack_len++] = 0;
	}

	for (unsigned int i = 1; i < 3; i++)
	{
		p = (unsigned char *) &mD[i];
		buf[pack_len++] = p[2];
		buf[pack_len++] = p[1];
		buf[pack_len++] = p[0];
		buf[pack_len++] = 0;
	}

	//check page:
	int cnt_left=0;
	if(mnI2CRegisterLeft>=112)
		cnt_left=(mnI2CRegisterLeft-108);

	if(cnt_left)
	{
		mnI2CDataLen += PackBuff(mpI2CData+mnI2CDataLen,buf,20-cnt_left,mnI2CPageLeft,mnI2CRegisterLeft);
		mnI2CDataLen += PackBuff(mpI2CData+mnI2CDataLen,buf+20-cnt_left,cnt_left,mnI2CPageLeft+1,8);
	}
	else
	{
		mnI2CDataLen += PackBuff(mpI2CData+mnI2CDataLen,buf,20,mnI2CPageLeft,mnI2CRegisterLeft);
	}


	//Right Channel
	//check page:
	if(mnI2CRegisterRight>=112)
		cnt_left=(mnI2CRegisterRight-108);

	if(cnt_left)
	{
		mnI2CDataLen += PackBuff(mpI2CData+mnI2CDataLen,buf,20-cnt_left,mnI2CPageRight,mnI2CRegisterRight);
		mnI2CDataLen += PackBuff(mpI2CData+mnI2CDataLen,buf+20-cnt_left,cnt_left,mnI2CPageRight+1,8);
	}
	else
	{
		mnI2CDataLen += PackBuff(mpI2CData+mnI2CDataLen,buf,20,mnI2CPageRight,mnI2CRegisterRight);
	}


// 	wchar_t strPrint[512];
// 	wchar_t element[8];
// 	swprintf_s(strPrint,512,_T(">>>[%d]\r\n "),mnI2CDataLen);
// 	for (int i=0;i<mnI2CDataLen;i++)
// 	{
// 		swprintf_s(element,8,_T("%x "),*(mpI2CData+i));
// 		wcscat_s(strPrint,512,element);
// 	}
// 	wcscat_s(strPrint,512,_T("\r\n"));
// 	RETAILMSG(1/*DEBUG_COMMANDER*/,(strPrint));

}


void TBiquad::CalcEQ(double FC, double BW, double FS, double G, bool bRefresh)
{
	mnFS = FS;
	mnFC = FC;
	mnG = G;
	mnQ = 1;
	mnBW = BW;
	mnBWP = BW / FC;

	double A, H;
	double a, d;
	double b0, b1, b2, a1, a2;

	if(G>15)G=15;
	if(G<-15)G=-15;

	A = pow(10, (G / 20.0));

	if (A < 1)
		a = (tan(n_pi * BW / FS) - A) / (tan(n_pi * BW / FS) + A);
	else
		a = (tan(n_pi * BW / FS) - 1) / (tan(n_pi * BW / FS) + 1);

	H = A  -  1;
	d =  - cos(2 * n_pi * FC / FS);

	b0 = 1  +  (1 + a) * H / 2;
	b1 = d * (1 - a);
	a1 = b1;
	b2 = ( - a - (1 + a) * H / 2);
	a2 =  - a;

	if (b0 > 1.0)
	{
		mnScale = 1.0 / b0;
		b1 /= b0;
		b2 /= b0;
		b0 = 1.0;
	}

	ma[0] = 1;
	ma[1] = a1;
	ma[2] = a2;
	mb[0] = b0;
	mb[1] = b1;
	mb[2] = b2;

// 	wchar_t strPrint[512];
// 	swprintf_s(strPrint,512,_T(">>>ma[0]:%f,ma[1]:%f,ma[2]:%f,mb[0]:%f,mb[1]:%f,mb[1]:%f, \r\n"),ma[0],ma[1],ma[2],mb[0],mb[1],mb[2]);
// 	RETAILMSG(1/*DEBUG_COMMANDER*/,(strPrint));

	if (bRefresh)
	{
		//RefreshData();
		Quantize();
	}
}

#if 0
void TBiquad::CalcBW1(double FC, double FS, double G, bool bHP, bool bRefresh)

{

	mnFC = FC;

	mnFS = FS;

	mnG = G;


//	LOG(AnsiString().sprintf("FC=%f, FS=%f, G=%f\n\r", mnFC, mnFS, mnG));

	volatile double wc, k;
	volatile double b0, b1, b2, a1, a2;
	volatile double B0, B1, B2, A0, A1, A2;

	volatile double A = pow(10, (G / 20.0));
//	LOG(AnsiString().sprintf("A=%f\n\r", A));
/*
1st First Order Butterworth

1.  Filter Parameters

	Fs = Sample rate in Hz
	Fc = Cut Frequency in Hz
	HL= LR Filter type (high, low )


2.  Error Checking

	0 <= Fc <= Fs/2
	0 <= Fs <= 192K
	HL= (high, low )
*/

	k = 2.0 * n_pi * FC / tan (n_pi * FC/FS);

	wc = 2 * n_pi * FC;

//	LOG(AnsiString().sprintf("k=%f, wc=%f\n\r", k, wc));

	if (!bHP)
	{

	   b0 = wc / (k + wc);
	   b1 = wc / (k + wc);
	}
	else // type == "HP"
	{
	   b0 = k / (k + wc);
	   b1 = - k / (k + wc);
	}

	b0 *= A;
	b1 *= A;

//	LOG(AnsiString().sprintf("b0=%f, b1=%f\n\r", b0, b1));

	a1 = (wc - k) / (k + wc);

//	LOG(AnsiString().sprintf("a1=%f\n\r", a1));

	if (b0 > 1.0)
	{
		mnScale = 1.0 / b0;
		b1 /= b0;
		b2 /= b0;
		b0 = 1.0;
	}

	ma[0] = 1;
	ma[1] = a1;
	ma[2] = a2;
	mb[0] = b0;
	mb[1] = b1;
	mb[2] = 0.0;

}
#endif

void TBiquad::CalcVQ2(double FC, double Q, double FS, double G, bool bHP, bool bRefresh)

{

	mnFC = FC;
	mnFS = FS;

	mnQ = Q;

	mnG = G;

	double wc, k;
	double Ba0, Ba1, Ba2;
	double Aa0, Aa1, Aa2;
	double B0, B1, B2, A0, A1, A2;

	double A = pow(10, (G / 20.0));

	wc = 2.0 * n_pi * FC;

	if (bHP)
	{
		Ba0 = 1.0;
		Ba1 = 0.0;
		Ba2 = 0.0;

		Aa0 = 1.0;
		Aa1 = wc / Q;
		Aa2 = wc * wc;
	}

	else // type == "LP"
	{
		Ba0 = 0.0;
		Ba1 = 0.0;
		Ba2 = wc * wc;

		Aa0 = 1.0;
		Aa1 = wc / Q;
		Aa2 = wc * wc;
	}

	k = 2.0 * n_pi * FC / tan (n_pi * FC/FS);

	B0 = Ba0 * k * k + Ba2 + Ba1 * k;
	B1 = -2.0 * Ba0 * k * k + 2.0 * Ba2;
	B2 = -Ba1 * k + Ba0 * k * k + Ba2;

	A0 = Aa0 * k * k + Aa2 + Aa1 * k;
	A1 = (-2.0 * Aa0 * k * k) + (2.0 * Aa2);
	A2 = (-Aa1 * k) + (Aa0 * k * k) + Aa2;

	B0 = A * B0 / A0;
	B1 = A * B1 / A0;
	B2 = A * B2 / A0;

	A1 = -A1/A0;
	A2 = -A2/A0;

	double a1, a2, b0, b1, b2;
	a1 = -A1;
	a2 = -A2;
	b0 = B0;
	b1 = B1;
	b2 = B2;

	if (b0 > 1.0)
	{
		mnScale = 1.0 / b0;
		b1 /= b0;
		b2 /= b0;
		b0 = 1.0;
	}

	ma[0] = 1;
	ma[1] = a1;
	ma[2] = a2;
	mb[0] = b0;
	mb[1] = b1;
	mb[2] = b2;

}


void TBiquad::CalcLR2(double FC, double FS, double G, bool bHP, bool bRefresh)

{

	mnFC = FC;
	mnFS = FS;
 	mnG = G;

	double wc, k;
	double Ba1, Ba2, Ba3;
	double Aa1, Aa2, Aa3;
	double B0, B1, B2, A0, A1, A2;

	double A = pow(10, (G / 20.0));

/*
	1. Filter Parameters

	   Fs = Sample rate in Hz
	   Fc = Cut Frequency in Hz
	   HL= LR Filter type (high, low )

	2.  Error Checking

	   0 <= Fc <= Fs/2
	   0 <= Fs <= 192K
	   HL= (high, low )
*/

	wc = 2 * n_pi *  FC;

	if (!bHP)
	{
	   Ba1 = 0;
	   Ba2 = 0;
	   Ba3 = wc * wc;

	   Aa1 = 1.0;
	   Aa2 = 2.0 * wc;
	   Aa3 = wc * wc;
	}

	 else // type == "HP"
	{
	   Ba1 = 1.0;
	   Ba2 = 0.0;
	   Ba3 = 0.0;

	   Aa1 = 1.0;
	   Aa2 = 2 * wc;
	   Aa3 = wc * wc;
	}

	k = 2.0 * n_pi * FC / tan (n_pi * FC/FS);

	B0 = (Ba1 * k * k) + (Ba3) + (Ba2 * k);
	B1 = (-2 * Ba1 * k * k) + (2 * Ba3);
	B2 = (-Ba2 * k) + (Ba1 *  k * k) + (Ba3);

	A0 = (Aa1 * k * k) + (Aa3) + (Aa2 * k);
	A1 = (-2 * Aa1 * k * k) + (2.0 * Aa3);
	A2 = (-Aa2 * k) + (Aa1 *  k * k) + (Aa3);

	B0 = A * B0 / A0;
	B1 = A * B1 / A0;
	B2 = A * B2 / A0;

	A1 = -A1 / A0;
	A2 = -A2 / A0;

	double a1, a2, b0, b1, b2;
	a1 = -A1;
	a2 = -A2;
	b0 = B0;
	b1 = B1;
	b2 = B2;

	if (b0 > 1.0)
	{
		mnScale = 1.0 / b0;
		b1 /= b0;
		b2 /= b0;
		b0 = 1.0;
	}

	ma[0] = 1;
	ma[1] = a1;
	ma[2] = a2;
	mb[0] = b0;
	mb[1] = b1;
	mb[2] = b2;

}


void TBiquad::CalcNotch(double FC, double BW, double FS, double G, bool bRefresh)

{

	mnFC = FC;
	mnBW = BW;
	mnBWP = BW / FC;
	mnFS = FS;
	mnG = G;

	double a, d, b0, b1, a1, b2, a2;
	double B0, B1, B2, A1, A2;

	double A = pow(10, (G / 20.0));

	a = (1  -  tan(n_pi * BW / FS)) / (1  +  tan(n_pi * BW / FS));
	d =  - cos(2 * n_pi * FC / FS);
	b0 = a;
	b1 = d * (1  +  a);
	a1 = b1;
	b2 = 1;
	a2 = a;

	A1 = -a1;
	A2 = -a2;
	B0 = A * 0.5 * (b0 + 1);
	B1 = A * 0.5 * (b1 + a1);
	B2 = A * 0.5 * (b2 + a2);

	b0 = B0;
	b1 = B1;
	b2 = B2;

	if (b0 > 1.0)
	{
		mnScale = 1.0 / b0;
		b1 /= b0;
		b2 /= b0;
		b0 = 1.0;
	}

	ma[0] = 1;
	ma[1] = a1;
	ma[2] = a2;
	mb[0] = b0;
	mb[1] = b1;
	mb[2] = b2;

}


void TBiquad::CalcShelf(double FC, double FS, double G, bool bTreble, bool bRefresh)
{
	mnFS = FS;
	mnFC = FC;
	mnG = G;

	double g, s;
	double F;
	double gd, gn, a;
	double b0, b1, b2, a1, a2;

	if (fabs(G) < 0.001) G = 0.001;

	g = pow(10, (G / 20.0));

	if ((G>-6) && (G<6))
	{
		F=sqrt(g);
	}
	else if(g > 1)
	{
		F =g/sqrt(2.0);
	}
	else
	{
		F=g*sqrt(2.0);
	}

//	F = g  /  sqrt(2.0);

	a = tan(n_pi*(FC/FS - 1.0/4.0));

	s = sqrt(2.0)/2.0;

	gd = pow((F*F  -  1.0) / (g*g  -  F*F), 0.25);

	gn = gd * sqrt(g);

	if (bTreble)
	{
		b0 = (gn*gn*a*a+2*s*gn-2*gn*gn*a+1-2*s*gn*a*a+a*a+gn*gn+2*a)/(1+gd*gd+2*s*gd-2*s*gd*a*a+gd*gd*a*a-2*gd*gd*a+a*a+2*a);
		b1 = (2-2*gn*gn*a*a+4*gn*gn*a+4*a-2*gn*gn+2*a*a)/(1+gd*gd+2*s*gd-2*s*gd*a*a+gd*gd*a*a-2*gd*gd*a+a*a+2*a);
		b2 = (1+2*s*gn*a*a-2*s*gn+2*a+a*a+gn*gn-2*gn*gn*a+gn*gn*a*a)/(1+gd*gd+2*s*gd-2*s*gd*a*a+gd*gd*a*a-2*gd*gd*a+a*a+2*a);

		a1 = (2-2*gd*gd*a*a+4*gd*gd*a+2*a*a-2*gd*gd+4*a)/(1+gd*gd+2*s*gd-2*s*gd*a*a+gd*gd*a*a-2*gd*gd*a+a*a+2*a);
		a2 = (1-2*gd*gd*a+2*a+gd*gd-2*s*gd+a*a+gd*gd*a*a+2*s*gd*a*a)/(1+gd*gd+2*s*gd-2*s*gd*a*a+gd*gd*a*a-2*gd*gd*a+a*a+2*a);
	}
	else
	{
		b0 = -(-1.0-gn*gn*a*a-a*a-2.0*gn*gn*a-gn*gn-2.0*s*gn+2.0*s*gn*a*a+2.0*a)/(2.0*s*gd+1-2.0*s*gd*a*a+gd*gd*a*a+2.0*gd*gd*a+a*a+gd*gd-2.0*a);
		b1 = -(2.0-4.0*a-4.0*gn*gn*a-2.0*gn*gn*a*a-2.0*gn*gn+2.0*a*a)/(2.0*s*gd+1.0-2.0*s*gd*a*a+gd*gd*a*a+2.0*gd*gd*a+a*a+gd*gd-2.0*a);
		b2 = (1.0+2.0*s*gn*a*a-2.0*a+gn*gn-2.0*s*gn+2.0*gn*gn*a+a*a+gn*gn*a*a)/(2.0*s*gd+1.0-2.0*s*gd*a*a+gd*gd*a*a+2.0*gd*gd*a+a*a+gd*gd-2.0*a);

		a1 = (-2.0+2.0*gd*gd*a*a+4.0*gd*gd*a-2.0*a*a+2.0*gd*gd+4.0*a)/(2.0*s*gd+1.0-2.0*s*gd*a*a+gd*gd*a*a+2.0*gd*gd*a+a*a+gd*gd-2.0*a);
		a2 = (gd*gd*a*a-2.0*a+1.0+2.0*gd*gd*a-2.0*s*gd+a*a+2.0*s*gd*a*a+gd*gd)/(2.0*s*gd+1.0-2.0*s*gd*a*a+gd*gd*a*a+2.0*gd*gd*a+a*a+gd*gd-2.0*a);
	}

	if (b0 > 1.0)
	{
		mnScale = 1.0 / b0;
		b1 /= b0;
		b2 /= b0;
		b0 = 1.0;
	}

	if (b1 > 1.0)
	{
		mnScale = 1.0 / b1;
		b0 /= b1;
		b2 /= b1;
		b1 = 1.0;
	}

	ma[0] = 1;
	ma[1] = a1;
	ma[2] = a2;
	mb[0] = b0;
	mb[1] = b1;
	mb[2] = b2;

}

void TBiquad::CalcPhaseShift(double FC, double BW, double FS, bool bRefresh)
{

	mnFC = FC;
	mnBW = BW;
	mnBWP = BW / FC;
	mnFS = FS;

	double interim = tan(n_pi * BW / FS);
	double A = (1 - interim) / (1 + interim);
	double D = -cos(2 * n_pi * FC / FS);

	mb[0] = A;
	mb[1] = D * (1 + A);
	mb[2] = 1;
	ma[0] = 1;
	ma[1] = mb[1];
	ma[2] = A;
}
