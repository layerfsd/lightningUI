#include "CmdSender_AudioCodec.h"

CmdSender_AudioCodec::CmdSender_AudioCodec(void)
{
	m_hACKEvent = CreateEvent(NULL,FALSE,FALSE,_T("AUDIO_CODEC_ACK_EVENT"));

}

CmdSender_AudioCodec::~CmdSender_AudioCodec(void)
{
	if(m_hACKEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hACKEvent);

}

BOOL CmdSender_AudioCodec::DoSendCmd(CMD_BUFF *pbuff)
{

	BOOL bSucc=FALSE;

	if(m_pCommander && pbuff)
	{
		//									header		length		addr  value.....
		static BYTE cmd[512]={0xF8,0x5D,	0x00,			0x00,0x00,        0x00,0x00};
		BYTE checksum=0;
		int iTryTimes=5;

		if(!m_pCommander || !pbuff)
		{
			return FALSE;
		}

		if (pbuff->iBuffLen>64)
		{
			pbuff->iBuffLen=64;
		}

		cmd[1]=pbuff->dwUserData==0?0x5D:0x5E;
		cmd[2]=pbuff->iBuffLen;

		memcpy(cmd+3,pbuff->BuffData,pbuff->iBuffLen);

		for(int i=0;i<=pbuff->iBuffLen;i++)
		{
			checksum+=cmd[i+2];
		}

		cmd[pbuff->iBuffLen+3]=checksum;

// 		RETAILMSG(1,(_T("++++AudioCodec send data: user: %d index:%d, len:%d, checksum: %d----\r\n"),pbuff->dwUserData,pbuff->index,pbuff->iBuffLen,checksum));

#if 0
		wchar_t strPrint[512];
		wchar_t element[8];
		swprintf_s(strPrint,512,_T(">>>SEND[%d][%d] "),pbuff->dwUserData,pbuff->index);
		for (int i=0;i<pbuff->iBuffLen;i++)
		{
			swprintf_s(element,8,_T("%x "),*(pbuff->BuffData+i));
			wcscat_s(strPrint,512,element);
		}
		swprintf_s(element,8,_T(" [%x] "),checksum);
		wcscat_s(strPrint,512,element);
		wcscat_s(strPrint,512,_T("\r\n"));
#endif

		do 
		{
#if 0
			RETAILMSG(1/*DEBUG_COMMANDER*/,(strPrint));
#endif

			BYTE ack=0;
			ResetEvent(m_hACKEvent);
			BOOL bw=m_pCommander->WriteSerialPort(cmd,pbuff->iBuffLen+4);
			if(bw)
			{
				if(WaitACK(&ack,1000))
				{
					bSucc = (ack==AUDIO_CODEC_ACK_OK);
					//RETAILMSG(1,(_T("###: %d\r\n"),bSucc));
					if(!bSucc)
					{
						RETAILMSG(1,(_T("\r\n##[ERR] AudioCodec send failed, idx: %d----\r\n\r\n"),pbuff->index));
						Sleep(10);
					}
				}
				else
				{
					RETAILMSG(1,(_T("\r\n##[ERR] AudioCodec WaitACK timeout :idx:%d----\r\n\r\n"),pbuff->index));
				}
			}
			else
			{
				RETAILMSG(1,(_T("\r\n##[ERR] AudioCodec WriteSerialPort failed :idx:%d----\r\n\r\n"),pbuff->index));
			}

			iTryTimes--;

		} while (!bSucc /*&& iTryTimes>0*/);

	}

	return bSucc;
}
BOOL CmdSender_AudioCodec::WaitACK(BYTE *ack,UINT timeout)
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

