#pragma once

#include "CmdSender.h"

class CmdSender_McuUpdate :
	public CmdSender
{
public:
	CmdSender_McuUpdate(void);
	~CmdSender_McuUpdate(void);

public:
	BOOL DoSendCmd(CMD_BUFF *pbuff);

private:
	BOOL WaitACK(BYTE *ack,UINT timeout=500);

private:
	HANDLE			m_hACKEvent;

};
