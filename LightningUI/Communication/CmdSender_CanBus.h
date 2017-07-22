#pragma once

#include "CmdSender.h"

#define CAN_ACK										0xFF
#define CAN_NACK_CHECK_NG				0xF0
#define CAN_NACK_NOT_SUPPORT			0xF3
#define CAN_NACK_BUSY							0xFC

class CmdSender_CanBus :
	public CmdSender
{
public:
	CmdSender_CanBus(void);
	~CmdSender_CanBus(void);

public:
	BOOL DoSendCmd(CMD_BUFF *pbuff);

private:
	BOOL WaitACK(BYTE *ack,UINT timeout=100);

private:
	HANDLE			m_hACKEvent;

};
