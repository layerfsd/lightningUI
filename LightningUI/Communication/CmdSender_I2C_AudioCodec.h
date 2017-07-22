#pragma once

#include "CmdSender.h"

#define AUDIO_CODEC_ACK_OK				0x1
#define AUDIO_CODEC_ACK_NG				0x0

class CmdSender_I2C_AudioCodec :
	public CmdSender
{
public:
	CmdSender_I2C_AudioCodec(void);
	~CmdSender_I2C_AudioCodec(void);

public:
	BOOL DoSendCmd(CMD_BUFF *pbuff);

private:
	BOOL m_bi2cinit;

};
