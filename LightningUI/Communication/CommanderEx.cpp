#include "CommanderEx.h"
#include "../Config/customer.h"
#include <pkfuncs.h>

#define DATA_PACK_LEN		180

CCommanderEx::CCommanderEx(void)
{
	memset(m_dataMCU,256,0);
	memset(m_dataDisk,2048,0);
	memset(m_bufCmd,8192,0);
	memset(m_dataVehicleInfo,4096,0);
	memset(m_dataTpmsInfo,1024,0);

	memset(&m_rdsData,0,sizeof(RDS_DATA));
	memset(&m_radioSignalData,0,sizeof(RADIO_SIGNAL_DATA));

	m_dwProcessPos=0;
	m_dwCmdLen=0;

	m_hAudioCodecACKEvent = CreateEvent(NULL,FALSE,FALSE,_T("AUDIO_CODEC_ACK_EVENT"));

}

CCommanderEx::~CCommanderEx(void)
{
	if(m_hAudioCodecACKEvent)
		CloseHandle(m_hAudioCodecACKEvent);
}

void CCommanderEx::PreProcess(LPBYTE lpCmdData,DWORD dwCmdLen)
{
	
#if 0
	static WCHAR strPrint[4096];
	//test://///////////////////////////////////////////////////////////////////////
	WCHAR element[8];
	int pos=0;
	BOOL bflag=FALSE;
	while(pos<(INT)dwCmdLen)
	{
		swprintf_s(strPrint,4096,bflag?_T("----"):_T("<<<< REV:(%d,%d)"),dwCmdLen,GetTickCount());
		int left_len=min(128,dwCmdLen-pos);
		for (int i=0;i<left_len;i++)
		{
			swprintf_s(element,8,_T("%x "),*(lpCmdData+pos+i));
			wcscat_s(strPrint,4096,element);
		}
		wcscat_s(strPrint,4096,_T("\r\n"));
		RETAILMSG(1/*DEBUG_COMMANDER*/,(strPrint));
		pos+=left_len;
		bflag++;
	}
#endif	
	//////////////////////////////////////////////////////////////////////////////////
	


	//input command:
	memcpy(m_bufCmd+m_dwCmdLen,lpCmdData,dwCmdLen);
	m_dwCmdLen+=dwCmdLen;

	///////////////////////////////
	//process:
	if (m_dwCmdLen<5)
	{
// 		RETAILMSG(DEBUG_COMMANDER,(_T("start to process,not enough data:%d\r\n"),m_dwCmdLen));
		return;
	}

// 	RETAILMSG(DEBUG_COMMANDER,(_T("start to process,total bytes:%d\r\n"),m_dwCmdLen));

	int iCmdLen=(INT)m_dwCmdLen;
	BYTE *pCursor=m_bufCmd;

	DWORD dwType=0;
	DWORD dwID=0;
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL	
	DWORD dwLen=0;
#endif

	while((pCursor-m_bufCmd)<=iCmdLen-5)
	{
		/////////////////////////////////////initialize package////////////////////////////////////////////
		if(*pCursor== 0xF8 && *(pCursor+1)==0x71)//data package
		{
			if(iCmdLen < DATA_PACK_LEN+3)//not enough data
			{
				RETAILMSG(DEBUG_COMMANDER_EX,(_T("data packge,not enough data:%d\r\n"),m_dwCmdLen));
				break;
			}
			//check sum
			BYTE checksum=0;
			for (int i=0;i<DATA_PACK_LEN;i++)//DATA_PACK_LEN=180
			{
				checksum+=*(pCursor+2+i);
			}

			BYTE check=*(pCursor+DATA_PACK_LEN+2);
			RETAILMSG(DEBUG_COMMANDER_EX,(_T("data packge checksum %x,data %x\r\n"),checksum,check));

			if(checksum == check)//check ok
			{
				//process:
				memcpy(m_dataMCU,pCursor+2,DATA_PACK_LEN);

				//post out:
				dwType=0;//package
				dwID=0;
				DWORD dwCMD=((dwType<<16)+dwID);

				//need to use SendMessage??
// 				::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
				::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));

				//valid command,move to next:
				pCursor+=(DATA_PACK_LEN+3);
			}
			else
			{
				//invalid command, and move to next:
				pCursor+=2;
			}
		}
		/////////////////////////////////////MCU cmd////////////////////////////////////////////
		else if(*pCursor== 0xF8 && *(pCursor+1)==0x70)//single cmd
		{
			dwID=*(pCursor+2);
			dwType=1;//single command
			DWORD dwCMD=((dwType<<16)+dwID);

			if(dwID == MCU_RDS_PS)
			{
				int len=8;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}
				//process:
				memcpy(m_rdsData.bufPS,pCursor+3,len);
				m_rdsData.bufPS[8]=0;
				::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 				::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));

				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RDS_RT)
			{
				int len=64;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}
				//process:
				memcpy(m_rdsData.bufRT,pCursor+3,len);
				m_rdsData.bufRT[63]=0;
				::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 				::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));

				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RDS_TDC)
			{
				int len=64;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}
				//process:
				memcpy(m_rdsData.bufTDC,pCursor+3,len);
				m_rdsData.bufTDC[63]=0;
				::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 				::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));

				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RDS_STR)
			{
				int len=64;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}
				//process:
				memcpy(m_rdsData.bufSTR,pCursor+3,len);
				m_rdsData.bufSTR[63]=0;
				::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 				::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));

				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RDS_RP)
			{
				int len=64;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}
				//process:
				memcpy(m_rdsData.bufRP,pCursor+3,len);
				m_rdsData.bufRP[63]=0;
				::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 				::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));

				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RDS_STATUS)
			{
				int len=2;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}

				//check sum
				BYTE checksum=0;
				for (int i=0;i<len+1;i++)
				{
					checksum+=*(pCursor+2+i);
				}

				BYTE check=*(pCursor+len+3);

				if(check == checksum)
				{
					memcpy(&m_rdsData.rdsStatus,pCursor+3,len);
					m_rdsData.rdsStatus=(*(pCursor+3)<<8)+(*(pCursor+4));
					::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 					::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
				}
				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RDS_BLOCK_DATA)
			{
				int len=10;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}

				//check sum
				BYTE checksum=0;
				for (int i=0;i<len+1;i++)
				{
					checksum+=*(pCursor+2+i);
				}

				BYTE check=*(pCursor+len+3);

				if(check == checksum)
				{
					memcpy(m_rdsData.rdsBlockData,pCursor+3,len);
					::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 					::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
				}
				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RDS_STATUS_DATA)
			{
				int len=12;
				int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}

				//check sum
				BYTE checksum=0;
				for (int i=0;i<len+1;i++)
				{
					checksum+=*(pCursor+2+i);
				}

				BYTE check=*(pCursor+len+3);

				if(check == checksum)
				{
					m_rdsData.rdsStatus=(*(pCursor+3)<<8)+(*(pCursor+4));
					memcpy(m_rdsData.rdsBlockData,pCursor+5,len-2);
					::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
// 					::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
				}
				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RADIO_SIGNAL_STATUS)
			{
				const int len=2;
				const int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}

				//check sum
				BYTE checksum=0;
				for (int i=0;i<len+1;i++)
				{
					checksum+=*(pCursor+2+i);
				}

				BYTE check=*(pCursor+len+3);

				if(check == checksum)
				{
					m_radioSignalData.signalStatus=(*(pCursor+3)<<8)+(*(pCursor+4));
					::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
				}
				pCursor+=len+4;//move to next
			}
			else if(dwID == MCU_RADIO_SIGNAL_DATA)
			{
				const int len=sizeof(RADIO_SIGNAL_DATA);
				const int len_remain=(iCmdLen-(pCursor-m_bufCmd));
				if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
				{  
					break;
				}

				//check sum
				BYTE checksum=0;
				for (int i=0;i<len+1;i++)
				{
					checksum+=*(pCursor+2+i);
				}

				BYTE check=*(pCursor+len+3);

				if(check == checksum)
				{
					const BYTE *pVal=(pCursor+3);
					m_radioSignalData.signalStatus=(*(pVal+0)<<8)+(*(pVal+1));
					m_radioSignalData.level=(*(pVal+2)<<8)+(*(pVal+3));
					m_radioSignalData.usn=(*(pVal+4)<<8)+(*(pVal+5));
					m_radioSignalData.wam=(*(pVal+6)<<8)+(*(pVal+7));
					m_radioSignalData.offset=(*(pVal+8)<<8)+(*(pVal+9));
					m_radioSignalData.bandwidth=(*(pVal+10)<<8)+(*(pVal+11));
					m_radioSignalData.modulation=(*(pVal+12)<<8)+(*(pVal+13));
					m_radioSignalData.stopQuality=(*(pVal+14)<<8)+(*(pVal+15));

					::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
				}
				pCursor+=len+4;//move to next
			}
			else
			{
				BYTE checksum=0;
				checksum=*(pCursor+2)+*(pCursor+3);
				if(checksum == *(pCursor+4))
				{
					BYTE idcmd=*(pCursor+2);
					//valid command and process:
					m_dataMCU[*(pCursor+2)]=*(pCursor+3);

					//RETAILMSG(1,(_T("[MCU]:0x%x, 0x%x\r\n"),idcmd,*(pCursor+3)));

					switch (idcmd)
					{
					case MCU_AUDIO_CODEC_ACK:
						{
							//RETAILMSG(1,(_T("[CODEC ACK]:0x%x\r\n"),*(pCursor+3)));
							SetEventData(m_hAudioCodecACKEvent,*(pCursor+3));
							SetEvent(m_hAudioCodecACKEvent);
						}
						break;
					}
					//need to use SendMessage??
// 	 				::PostMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
					::SendMessage(m_hWnd,WM_MCU_CMD,dwCMD,LPARAM(this));
	// 				RETAILMSG(DEBUG_COMMANDER,(_T("Post command:%x\r\n"),dwCMD));

					pCursor+=5;//move to next
				}
				else
				{
					//invalid command, and move to next:
					pCursor+=2;
				}
			}
		}
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL		
		///////////////////////////////// Extensible CMD////////////////////////////////////////
		else if(*pCursor== 0xF8 && *(pCursor+1)==0x79 && *(pCursor+2)>=2)//single cmd
		{
			dwLen = *(pCursor+2)-2;
			dwID = *(pCursor+3);
			if(dwLen < 0)
			{
				RETAILMSG(1,(_T("ERR:[%s]: dwLen:%d is invalid \r\n"),TEXT(__FUNCTION__),dwLen));
				break;				
			}

			//Debug info
			RETAILMSG(1,(_T("MSG:[%s]: dwID:%d dwLen:%d ==========\r\n"),TEXT(__FUNCTION__),dwID,dwLen));
			for(int temp = 0;temp<dwLen+5;temp++)
			{
				RETAILMSG(1,(_T("0x%X "),*(pCursor+temp)));
			}
			RETAILMSG(1,(_T("=========\r\n")));

			MCU_EXTENSIBLE_DATA mcuextendata;
			memset(&mcuextendata, 0, sizeof(mcuextendata));
			BYTE *pData = new BYTE[dwLen];
			if (pData)
			{
				memcpy(pData, pCursor+4, dwLen);
				//check sum
				BYTE checksum=dwID;
				BYTE check =  *(pCursor+4+dwLen);
				
				for (int i=0;i<dwLen;i++)//DATA_PACK_LEN=180
				{
					checksum+=*(pData+i);
				}

				if (check == checksum)
				{
					mcuextendata.CmdID = dwID;
					mcuextendata.DataLen = dwLen;
					mcuextendata.CheckSum = check;
					mcuextendata.pData = pData;
					::SendMessage(m_hWnd,WM_EXTENSIBLE_CMD,sizeof(mcuextendata),LPARAM(&mcuextendata));
				}
				else
				{
					RETAILMSG(1,(_T("ERR:[%s]: Extensible CMD Checksum0x%X check:0x%X failed!!!\r\n"),TEXT(__FUNCTION__),checksum,check));
				}
				
				delete [] pData;
			}
			pCursor+=(5+dwLen);

		}
#endif		
		/////////////////////////////////////DVD cmd////////////////////////////////////////////
		else if(*pCursor== 0xF8 && *(pCursor+1)==0x7C)//DVD cmd
		{
			int len=*(pCursor+2);

			int len_remain=(iCmdLen-(pCursor-m_bufCmd));

			if(len_remain < len+4)//include 0xF8,0x7C,length,CRC
			{  
				RETAILMSG(DEBUG_COMMANDER_EX,(_T("dick cmd not enough data: len:%d,need:%d\r\n"),len_remain,len+4));
				break;
			}

			//process:
			memcpy(m_dataDisk,pCursor+3,*(pCursor+2));

			::SendMessage(m_hWnd,WM_DISK_CMD,*(pCursor+2),LPARAM(this));
// 			::PostMessage(m_hWnd,WM_DISK_CMD,*(pCursor+2),LPARAM(this));

			pCursor+=len+4;//move to next
		}
		/////////////////////////////////////Vehicle info cmd////////////////////////////////////////////
		else if(*pCursor== 0xF8 && *(pCursor+1)==0x7D)//Vehicle info cmd
		{
			//process:
			int len=*(pCursor+2);

			int len_remain=(iCmdLen-(pCursor-m_bufCmd));

			//check sum:

			memcpy(m_dataVehicleInfo,pCursor+3,*(pCursor+2));

			::SendMessage(m_hWnd,WM_VEHICLEINFO_CMD,*(pCursor+2),LPARAM(this));
// 			::PostMessage(m_hWnd,WM_VEHICLEINFO_CMD,*(pCursor+2),LPARAM(this));

			pCursor+=len+4;//move to next
		}
		/////////////////////////////////////TPMS info cmd////////////////////////////////////////////
		else if(*pCursor== 0xF8 && *(pCursor+1)==0x7E)//TPMS info cmd
		{
			//process:
			int len=*(pCursor+2);

			int len_remain=(iCmdLen-(pCursor-m_bufCmd));

			//check sum:

			memcpy(m_dataTpmsInfo,pCursor+3,*(pCursor+2));
			m_dataTpmsInfo[len]=0;

			::SendMessage(m_hWnd,WM_TPMS_CMD,*(pCursor+2),LPARAM(this));
// 			::PostMessage(m_hWnd,WM_TPMS_CMD,*(pCursor+2),LPARAM(this));

			pCursor+=len+4;//move to next
		}
		/////////////////////////////////////MCU update cmd////////////////////////////////////////////
		else if(*pCursor== 0xF8 && *(pCursor+1)==0x7F)
		{
			//process:MCU update ACK message
			
			::SendMessage(m_hWnd,WM_MCU_UPDATE_CMD,*(pCursor+2),LPARAM(this));
			pCursor+=4;//move to next
		}
		/////////////////////////////////////invalid cmd////////////////////////////////////////////
		else//invalid :
		{
			pCursor++;
		}

		Sleep(0);
	}

	//calculate left:
	int iLeft=iCmdLen-(pCursor-m_bufCmd);
	if(iLeft>0)
	{
		RETAILMSG(DEBUG_COMMANDER_EX,(_T("--------Left %d\r\n"),iLeft));
		m_dwCmdLen = iLeft;

		if(m_bufCmd != pCursor)
		{
			memmove(m_bufCmd,pCursor,iLeft);
		}
	}
	else
	{
		m_dwCmdLen = 0;
	}

	//
// 	if(m_hWnd)
// 	{
// 		InvalidateRect(m_hWnd,NULL,FALSE);
// 	}
}
void CCommanderEx::IniMCU()
{
	RETAILMSG(1,(_T("-----CCommanderEx::IniMCU------\r\n")));

	//mcu:
	BYTE buff[4]={0xf8,0x51,0x01,0x52};
	WriteSerialPort(buff,4);
}
BYTE CCommanderEx::GetItemData(INT ID)
{
	BYTE res=0;

	if(ID>=0 && ID<256)
	{
		res = m_dataMCU[ID];
	}

	return res;
}
void CCommanderEx::SetItemData(INT ID,BYTE val)
{
	if(ID>=0 && ID<256)
	{
		m_dataMCU[ID] = val;
	}
}