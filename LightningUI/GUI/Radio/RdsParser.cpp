#include "RdsParser.h"
#include "../../resource.h"
#include "../Common/ui_common.h"

BYTE RdsGetGroupType(const RdsData *pData)
{
	return pData ? ((pData->blockB >> 11) & 0x1F) : 0;
}

BOOL RdsParse(DWORD freq,const RdsData *pData,RdsInfo *pInfo)
{
	if(!pData || !pInfo)
		return FALSE;

	pInfo->freq = freq;

	//check error code:
	BYTE  err_A = ((pData->decErr >> 14) & 0x03);
	BYTE  err_B = ((pData->decErr >> 12) & 0x03);
	BYTE  err_C = ((pData->decErr >> 10) & 0x03);
	BYTE  err_D = ((pData->decErr >> 8) & 0x03);

	//RETAILMSG(DEBUG_RDS,(_T("####<%s>:: RdsGetGroupType: 0x%x\r\n"),_T(__FUNCTION__),RdsGetGroupType(pData)));

	RETAILMSG(DEBUG_RDS,(_T("<%s>::[%X] decErr: 0x%x, A: 0x%x, B: 0x%x, C: 0x%x, D: 0x%x, \r\n"),_T(__FUNCTION__),
		RdsGetGroupType(pData),pData->decErr,
		pData->blockA,pData->blockB,pData->blockC,pData->blockD));

#if 0
	if((err_A == RBE_Uncorrectable) ||
		(err_B == RBE_Uncorrectable) ||
		(err_C == RBE_Uncorrectable) ||
		(err_D == RBE_Uncorrectable))
	{
		RETAILMSG(DEBUG_ERR,(_T("<%s>[ERR]:: err_A: %d, err_B: %d, err_C: %d, err_D: %d\r\n"),_T(__FUNCTION__),err_A,err_B,err_C,err_D));
		return FALSE;
	}

	if((err_A > RBE_2Bits) ||
		(err_B > RBE_2Bits) ||
		(err_C > RBE_2Bits) ||
		(err_D > RBE_2Bits))
	{
		RETAILMSG(DEBUG_ERR,(_T("<%s>[ERR]:: err_A: %d, err_B: %d, err_C: %d, err_D: %d\r\n"),_T(__FUNCTION__),err_A,err_B,err_C,err_D));
		return FALSE;
	}
#endif

	//get PI:
	if(err_A == RBE_None)
	{
		pInfo->PICode = pData->blockA;
	}
	else
	{
		//invalid PI, skip this frame:
		return FALSE;
	}

	//no valid group type available:
	if(err_B != RBE_None)
	{
		return FALSE;
	}

	switch (RdsGetGroupType(pData))
	{
	case RDS_GROUP_TYPE_A(0): //Group 0A
		{
			if(err_B == RBE_None)
			{
				pInfo->TP = ((pData->blockB >> 10) & 0x01);
				pInfo->PTY = ((pData->blockB >> 5) & 0x1F);
				pInfo->TA = ((pData->blockB >> 4) & 0x01);
			}
			if(err_B == RBE_None && err_D == RBE_None)
			{
				//PS:
				pInfo->PS[2 * (pData->blockB & 0x03)] = (pData->blockD >> 8);
				pInfo->PS[2 * (pData->blockB & 0x03) + 1] = (pData->blockD & 0x0FF);
			}

			if(err_C == RBE_None)
			{
				//AF:
				BYTE AF1 = BYTE(pData->blockC >> 8);
				BYTE AF2 = BYTE(pData->blockC);
				//AF number:
				if(AF1>=224 && AF1<=249)
					pInfo->AF.AFTotalNum=AF1-224;
				if(AF2>=224 && AF2<=249)
					pInfo->AF.AFTotalNum=AF2-224;

				RETAILMSG(DEBUG_RDS,(_T("<%s>:: AF1 %d, AF2: %d \r\n"),_T(__FUNCTION__),AF1,AF2));

	// 			if(pInfo->AFNum == 0 || pInfo->AFFilled > pInfo->AFNum)
	// 			{
	// 				pInfo->AFFilled = 0;
	// 			}

				if(AF1>0&&AF1<=204 && (pInfo->AF.AFTotalNum?pInfo->AF.AFFilledNum<pInfo->AF.AFTotalNum:TRUE))
				{
					//if(pInfo->AFFilledNum<pInfo->AFTotalNum)
					{
						//check if already in list:
						BOOL bexist=FALSE;
						for (int i=0;i<25;i++)
						{
							if(pInfo->AF.AFFreq[i] == AF1)
							{
								bexist = TRUE;
								break;
							}
						}
						if(!bexist && pInfo->AF.AFFilledNum<24)
						{
							pInfo->AF.AFFreq[pInfo->AF.AFFilledNum]=AF1;
							pInfo->AF.AFFilledNum++;
							RETAILMSG(1/*DEBUG_RDS*/,(_T("<%s>:: GET AF %d, total: %d, filed: %d \r\n"),_T(__FUNCTION__),AF1,pInfo->AF.AFTotalNum,pInfo->AF.AFFilledNum));
						}
					}
				}
				if(AF2>0&&AF2<=204 && (pInfo->AF.AFTotalNum?pInfo->AF.AFFilledNum<pInfo->AF.AFTotalNum:TRUE))
				{
					//if(pInfo->AFFilledNum<pInfo->AFTotalNum)
					{
						//check if already in list:
						BOOL bexist=FALSE;
						for (int i=0;i<25;i++)
						{
							if(pInfo->AF.AFFreq[i] == AF2)
							{
								bexist = TRUE;
								break;
							}
						}
						if(!bexist && pInfo->AF.AFFilledNum<24)
						{
							pInfo->AF.AFFreq[pInfo->AF.AFFilledNum]=AF2;
							pInfo->AF.AFFilledNum++;
							RETAILMSG(1/*DEBUG_RDS*/,(_T("<%s>:: GET AF %d, total: %d, filed: %d \r\n"),_T(__FUNCTION__),AF2,pInfo->AF.AFTotalNum,pInfo->AF.AFFilledNum));
						}
					}
				}
			}
		}
		break;
	case RDS_GROUP_TYPE_B(0): //Group 0B
		{
			if(err_B == RBE_None)
			{
				pInfo->TP = ((pData->blockB >> 10) & 0x01);
				pInfo->PTY = ((pData->blockB >> 5) & 0x1F);
				pInfo->TA = ((pData->blockB >> 4) & 0x01);
			}

			if(err_B == RBE_None && err_D == RBE_None)
			{
				//PS:
				pInfo->PS[2 * (pData->blockB & 0x03)] = (pData->blockD >> 8);
				pInfo->PS[2 * (pData->blockB & 0x03) + 1] = (pData->blockD & 0x0FF);
			}
		}
		break;

	case RDS_GROUP_TYPE_A(1): //Group 1A
		break;
	case RDS_GROUP_TYPE_B(1): //Group 1B
		break;

	case RDS_GROUP_TYPE_A(2): //Group 2A
		{
			if(err_B == RBE_None && err_C == RBE_None && err_D == RBE_None)
			{
				//RT:
				pInfo->RT[4 * (pData->blockB & 0x0F) + 0] = (pData->blockC >> 8);
				pInfo->RT[4 * (pData->blockB & 0x0F) + 1] = (pData->blockC & 0xFF);
				pInfo->RT[4 * (pData->blockB & 0x0F) + 2] = (pData->blockD >> 8);
				pInfo->RT[4 * (pData->blockB & 0x0F) + 3] = (pData->blockD & 0xFF);
			}
		}
		break;
	case RDS_GROUP_TYPE_B(2): //Group 2B
		{
			if(err_B == RBE_None && err_D == RBE_None)
			{
				//RT:
				pInfo->RT[2 * (pData->blockB & 0x0F) + 0] = (pData->blockD >> 8);
				pInfo->RT[2 * (pData->blockB & 0x0F) + 1] = (pData->blockD & 0xFF);
			}
		}
		break;
	}

	return TRUE;
}