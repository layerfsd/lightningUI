#include "CmdSender_CanBus.h"

CmdSender_CanBus::CmdSender_CanBus(void)
{
	m_hACKEvent = CreateEvent(NULL,FALSE,FALSE,_T("CAN_BUS_ACK_EVENT"));

}

CmdSender_CanBus::~CmdSender_CanBus(void)
{
	if(m_hACKEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hACKEvent);

}

BOOL CmdSender_CanBus::DoSendCmd(CMD_BUFF *pbuff)
{

	BOOL bSucc=FALSE;

	if(m_pCommander && pbuff)
	{
// 		RETAILMSG(1,(_T("++++CmdSender_CanBus::DoSendCommand: index:%d,len:%d--,delay:%d\r\n"),
// 			pbuff->index,pbuff->iBuffLen,pbuff->iSendDelay));

		//                                                          header  data...iLen          c1                 c2
		//                                   0      1      2       3            4                   4+iLen            4+iLen+1
		static BYTE buff[512]={0xF8,0x7D,0x00,  0x2E,    0x00,0x00,          0x00,             0x00};
		BYTE checksum_can=0;
		BYTE checksum_mcu=0;

		BYTE ack=0;

		BYTE *pBuff=pbuff->BuffData;
		INT  iLen=pbuff->iBuffLen;
		INT iTryTimes=1;

		if(!pBuff || !iLen || iLen>64 ||!m_pCommander)
			return FALSE;

		buff[2]=(BYTE)iLen+2;//MCU cmd length

		memcpy(buff+4,pBuff,iLen);//data

		checksum_can=0;
		for (int i=0;i<iLen;i++)
		{
			checksum_can+=*(buff+i+4);
		}
		buff[4+iLen]=~checksum_can;//can checksum

		checksum_mcu=0;
		for (int i=0;i<iLen+3;i++)
		{
			checksum_mcu+=*(buff+i+2);
		}
		buff[5+iLen]=checksum_mcu;//MCU checksum


		do 
		{
			ResetEvent(m_hACKEvent);
			m_pCommander->WriteSerialPort(buff,iLen+6);
			// 			RETAILMSG(1,(_T("++++SendVehicleCommand WaitACK ....\r\n")));
			if(WaitACK(&ack,200))
			{
				bSucc = (ack==CAN_ACK);
			}
			else
			{
				RETAILMSG(1,(_T("++++canbus WaitACK timeout, id:0x%x----\r\n"),buff[4]));
			}

			iTryTimes--;

		} while (!bSucc && iTryTimes>0);

	}

	return bSucc;
}
BOOL CmdSender_CanBus::WaitACK(BYTE *ack,UINT timeout)
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

