#include "CmdSender_McuUpdate.h"

CmdSender_McuUpdate::CmdSender_McuUpdate(void)
{
	m_hACKEvent = CreateEvent(NULL,FALSE,FALSE,_T("MCU_UPDATE_ACK_EVENT"));

}

CmdSender_McuUpdate::~CmdSender_McuUpdate(void)
{
	if(m_hACKEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hACKEvent);
		m_hACKEvent = INVALID_HANDLE_VALUE;
	}

}

BOOL CmdSender_McuUpdate::DoSendCmd(CMD_BUFF *pbuff)
{

	BOOL bSucc=FALSE;

	if(m_pCommander && pbuff)
	{
// 		RETAILMSG(1,(_T("++++CmdSender_McuUpdate::DoSendCommand: index:%d,len:%d--,delay:%d\r\n"),
// 			pbuff->index,pbuff->iBuffLen,pbuff->iSendDelay));

		//                                   header   iLen   serial     data...iLen      checksum
		//                                   0      1      2       3            4                   4+iLen            
		static BYTE buff[512]={0xF8,0x5F,0x00,  0x00,    0x00,0x00,          0x00};
		BYTE checksum=0;

		BYTE ack=0;

		BYTE *pDataBuff=pbuff->BuffData;
		INT  iLen=pbuff->iBuffLen;
		INT iTryTimes=1;

		if(!pDataBuff || !iLen || iLen>320 ||!m_pCommander)
			return FALSE;

		buff[2]=(BYTE)iLen+2;//MCU cmd length

		BYTE iser=pbuff->index%255;//serial number, 0-254
		buff[3]=iser;

		memcpy(buff+4,pDataBuff,iLen);//data

		checksum=0;
		for (int i=0;i<iLen+2;i++)
		{
			checksum+=*(buff+i+2);
		}
		buff[4+iLen]=checksum;//checksum

		//
		do 
		{
			ResetEvent(m_hACKEvent);
			m_pCommander->WriteSerialPort(buff,iLen+5);

			if(WaitACK(&ack,2000))
			{
				bSucc = (ack==iser);
				if(!bSucc)
				{
					RETAILMSG(1,(_T("++++MCU push pack FAILED!----index: %d,size:%d,ack:%d\r\n"),pbuff->index,pbuff->iBuffLen,ack));
				}
			}
			else
			{
				RETAILMSG(1,(_T("++++MCU push pack WaitACK timeout----index: %d,size:%d\r\n"),pbuff->index,pbuff->iBuffLen));
			}

			iTryTimes--;

		} while (!bSucc && iTryTimes>0);

	}

	return bSucc;
}
BOOL CmdSender_McuUpdate::WaitACK(BYTE *ack,UINT timeout)
{
	BOOL bRet = FALSE;

	/*ResetEvent(m_hACKEvent);*/
	if(WAIT_OBJECT_0 == WaitForSingleObject(m_hACKEvent,timeout))
	{
		BYTE back=BYTE(GetEventData(m_hACKEvent));
		bRet = (back!=0xFF);
		if(ack)
		{
			*ack=back;
		}
	}

	return bRet;

}

