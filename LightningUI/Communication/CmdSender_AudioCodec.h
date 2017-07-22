#pragma once

#include "CmdSender.h"

#define AUDIO_CODEC_ACK_OK				0x1
#define AUDIO_CODEC_ACK_NG				0x0

class CmdSender_AudioCodec :
	public CmdSender
{
public:
	CmdSender_AudioCodec(void);
	~CmdSender_AudioCodec(void);

public:
	BOOL DoSendCmd(CMD_BUFF *pbuff);

private:
	BOOL WaitACK(BYTE *ack,UINT timeout=500);

private:
	HANDLE			m_hACKEvent;

};
