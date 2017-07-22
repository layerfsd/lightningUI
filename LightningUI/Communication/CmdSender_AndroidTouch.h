#pragma once

#include "CmdSender.h"

class CmdSender_AndroidTouch :
	public CmdSender
{
public:
	CmdSender_AndroidTouch(void);
	~CmdSender_AndroidTouch(void);

public:
	BOOL DoSendCmd(CMD_BUFF *pbuff);

};
