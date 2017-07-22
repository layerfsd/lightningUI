#include "CmdSender_AndroidTouch.h"

CmdSender_AndroidTouch::CmdSender_AndroidTouch(void)
{
}

CmdSender_AndroidTouch::~CmdSender_AndroidTouch(void)
{
}

BOOL CmdSender_AndroidTouch::DoSendCmd(CMD_BUFF *pbuff)
{

	BOOL bSucc=FALSE;

	if(m_pCommander && pbuff)
	{        
		static BYTE buff[512]={0x01,0x01,0x00,0x00,0x02,0x00,0x00};
		BYTE checksum=0;
		BYTE *pDataBuff=pbuff->BuffData;
		INT  iLen=pbuff->iBuffLen;

		//RETAILMSG(1,(_T("++++CmdSender_AndroidTouch::DoSendCmd: len:%d--\r\n"),pbuff->iBuffLen));

		if(!pDataBuff || !iLen || iLen>320 ||!m_pCommander)
			return FALSE;

		buff[2]=(BYTE)iLen+2;//MCU cmd length

		memcpy(buff+5,pDataBuff,iLen);//data

		checksum=0;
		for (int i=0;i<iLen+4;i++)
		{
			checksum ^= *(buff+i+2);
		}
		buff[5+iLen]=checksum;//checksum

		bSucc = m_pCommander->WriteSerialPort(buff,iLen+6);

	}

	return bSucc;
}

