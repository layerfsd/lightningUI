#include "CmdSender_I2C_AudioCodec.h"
#include "AudioCodecI2C.h"

CmdSender_I2C_AudioCodec::CmdSender_I2C_AudioCodec(void)
:m_bi2cinit(FALSE)
{
	m_bi2cinit = (AudioCodec_I2C_Open() == 0);
}

CmdSender_I2C_AudioCodec::~CmdSender_I2C_AudioCodec(void)
{
	AudioCodec_I2C_Close();
}

BOOL CmdSender_I2C_AudioCodec::DoSendCmd(CMD_BUFF *pbuff)
{

	BOOL bSucc=FALSE;

	if(m_bi2cinit && pbuff)
	{
		if ( pbuff->dwUserData == 1 )
		{
			for ( INT i = 0; i < pbuff->iBuffLen/2; i++ )
			{
				AudioCodec_I2C_Write(pbuff->BuffData[i*2], &pbuff->BuffData[i*2+1], 1);
			}
		}
	}

	return bSucc;
}

