#include "CmdSender_Disk.h"

CmdSender_Disk::CmdSender_Disk(void)
{
	m_hACKEvent = CreateEvent(NULL,FALSE,FALSE,_T("DISK_ACK_EVENT"));

}

CmdSender_Disk::~CmdSender_Disk(void)
{
	if(m_hACKEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hACKEvent);

}

BOOL CmdSender_Disk::DoSendCmd(CMD_BUFF *pbuff)
{

	BOOL bSucc=FALSE;

	if(m_pCommander && pbuff)
	{
		RETAILMSG(1,(_T("++++CmdSender_Disk::DoSendCommand: index:%d,len:%d--,delay:%d\r\n"),
			pbuff->index,pbuff->iBuffLen,pbuff->iSendDelay));

		static BYTE buff[512]={0xF8,0x7C,0x00,0xFF,0x55,0x00,0x00};
		BYTE checksum_dvd=0;
		BYTE checksum_mcu=0;
		BYTE ack=0;

		BYTE *pBuff=pbuff->BuffData;
		INT  iLen=pbuff->iBuffLen;

		if(!pBuff || !iLen || iLen>64 ||!m_pCommander)
			return FALSE;

		buff[2]=(BYTE)iLen+4;//MCU cmd length

		buff[5]=(BYTE)iLen;//DVD length
		memcpy(buff+6,pBuff,iLen);//DVD data

		checksum_dvd=iLen;
		for (int i=0;i<iLen;i++)
		{
			checksum_dvd+=*(buff+i+6);
		}
		buff[6+iLen]=~checksum_dvd;//DVD checksum

		checksum_mcu=0;
		for (int i=0;i<iLen+5;i++)
		{
			checksum_mcu+=*(buff+i+2);
		}
		buff[7+iLen]=checksum_mcu;//MCU checksum

		ResetEvent(m_hACKEvent);
		m_pCommander->WriteSerialPort(buff,iLen+8);

		if(WaitACK(&ack,300))
		{
			bSucc = (ack==0);

			RETAILMSG(1,(_T("++++CmdSender_Disk::DoSendCommand WaitACK OK ack:%d----\r\n"),ack));
		}
		else
		{
			RETAILMSG(1,(_T("++++CmdSender_Disk::DoSendCommand WaitACK timeout----\r\n")));
		}
	}

	return bSucc;
}
BOOL CmdSender_Disk::WaitACK(BYTE *ack,UINT timeout)
{
	BOOL bRet = FALSE;

	/*ResetEvent(m_hACKEvent);*/
	if(WAIT_OBJECT_0 == WaitForSingleObject(m_hACKEvent,timeout))
	{
		bRet = TRUE;
		if(ack)
		{
			*ack=BYTE(GetEventData(m_hACKEvent));
		}
	}

	return bRet;

}

