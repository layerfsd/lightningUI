#include "AspBase.h"

CAspBase::CAspBase()
{
	//default:
	m_dwDevAddr = 0x58;
	m_hASP = INVALID_HANDLE_VALUE;
	m_aspWorkMode = AspModeNull;
	m_dwRstGpioId = -1;//invalid id
}

CAspBase::~CAspBase(void)
{
	if(m_hASP != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hASP);
		m_hASP=NULL;
	}
}

BOOL CAspBase::Initialize(IICPort iicPort, SPIPort spiPort, DWORD gpioRstID,DWORD iicDevAddr, DWORD iicClock, DWORD spiDCB)
{
	if(m_hASP == INVALID_HANDLE_VALUE)
	{
		m_hASP = CreateFile(TEXT("ASP1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, 0);
		if(m_hASP != INVALID_HANDLE_VALUE)
		{
			//set port:
			DWORD port = (iicPort<<16) + (spiPort);
			DWORD dwReturn;
			if (!DeviceIoControl(m_hASP, IOCTL_ASP_CONFIG_PORT, &port, sizeof(DWORD), NULL, 0, &dwReturn, NULL))
			{
				RETAILMSG(1, (TEXT("##Failed CAspFM1388::Config ASP port error = 0x%x\r\n"), GetLastError()));
			}
			else
			{
				//RETAILMSG(1, (TEXT("##CAspFM1388::Config ASP port SUCCEED! \r\n")));
			}

			//set spi:
			ConfigSpi(spiDCB);
			//set iic:
			ConfigIIC(iicClock);
		}
		else
		{
			RETAILMSG(1, (TEXT("##Failed to open ASP driver! err = 0x%x\r\n"), GetLastError()));
		}
	}

	m_dwRstGpioId = gpioRstID;
	m_dwDevAddr = iicDevAddr;

	return (m_hASP != INVALID_HANDLE_VALUE);
}
BOOL CAspBase::Asp_ReadI2c(DWORD ulRegAddr, PBYTE pBuffer, UINT uLength)
{
	BOOL bRet=FALSE;

	if(m_hASP != INVALID_HANDLE_VALUE)
	{
		ASP_I2C_TRANS_DATA trans;
		DWORD dwReturn;

		memset(&trans,0,sizeof(ASP_I2C_TRANS_DATA));
		trans.dwDevAddr = m_dwDevAddr;
		trans.dwRegAddr = ulRegAddr;
		trans.pValueBuf = pBuffer;
		trans.uiValueLen = uLength;

		if (!DeviceIoControl(m_hASP, IOCTL_ASP_I2C_READ, NULL, 0, &trans, sizeof(ASP_I2C_TRANS_DATA), &dwReturn, NULL))
		{
			RETAILMSG(1, (TEXT("##Failed Asp_ReadI2c error = 0x%x\r\n"), GetLastError()));
			bRet=FALSE;
		}
		else
		{
			//RETAILMSG(1, (TEXT("##Asp_ReadI2c SUCCEED! 0x%x\r\n"),*trans.pValueBuf));
			bRet=TRUE;
		}
	}

	return bRet;
}
BOOL CAspBase::Asp_WriteI2c(DWORD ulRegAddr, PBYTE pBuffer, UINT uLength)
{
	BOOL bRet=FALSE;

	if(m_hASP != INVALID_HANDLE_VALUE)
	{
		ASP_I2C_TRANS_DATA trans;
		DWORD dwReturn;

		memset(&trans,0,sizeof(ASP_I2C_TRANS_DATA));
		trans.dwDevAddr = m_dwDevAddr;
		trans.dwRegAddr = ulRegAddr;
		trans.pValueBuf = pBuffer;
		trans.uiValueLen = uLength;

		if (!DeviceIoControl(m_hASP, IOCTL_ASP_I2C_WRITE, &trans, sizeof(ASP_I2C_TRANS_DATA), NULL, 0, &dwReturn, NULL))
		{
			RETAILMSG(1, (TEXT("##Failed WriteAspI2c error = 0x%x\r\n"), GetLastError()));
			bRet=FALSE;
		}
		else
		{
			//RETAILMSG(1, (TEXT("##WriteAspI2c SUCCEED!\r\n")));
			bRet=TRUE;
		}
	}

	return bRet;
}
BOOL CAspBase::Asp_ReadSpi(PBYTE pCmd, DWORD dwCmdLen, PBYTE pData, DWORD dwDataLen)
{
	BOOL bRet=FALSE;

	if(m_hASP != INVALID_HANDLE_VALUE)
	{
		SPIHDR SpiHdr;
		DWORD dwReturn;

		memset(&SpiHdr, 0, sizeof(SPIHDR));

		if (pCmd)
		{
			memcpy(SpiHdr.Cmd, pCmd, dwCmdLen);
		}

		SpiHdr.dwFlags = SPIHDR_PREPARE;

		SpiHdr.dwCmdLen = dwCmdLen;
		SpiHdr.pDataBuf = pData;
		SpiHdr.dwBufLen = dwDataLen;
		SpiHdr.dwBytesIn = 0;
		SpiHdr.bAsync = FALSE;
		SpiHdr.bWRSimult = FALSE;

		if (!DeviceIoControl(m_hASP, IOCTL_ASP_SPI_READ, NULL, 0, &SpiHdr, sizeof(SPIHDR), &dwReturn, NULL))
		{
			RETAILMSG(1, (TEXT("@@Failed Asp_ReadSpi error = 0x%x\r\n"), GetLastError()));
			bRet=FALSE;
		}
		else
		{
// 			if(pCmd)
// 				RETAILMSG(1, (TEXT("##Asp_ReadSpi SUCCEED! cmd=0x%x, dwDataLen=%d, dwBytesIn=%d, pData[0]=0x%x\r\n"),*pCmd,dwDataLen,SpiHdr.dwBytesIn,pData[0]));
// 			else
// 				RETAILMSG(1, (TEXT("##Asp_ReadSpi SUCCEED! dwDataLen=%d, dwBytesIn=%d, pData[0]=0x%x\r\n"),dwDataLen,SpiHdr.dwBytesIn,pData[0]));

			bRet=TRUE;
		}
	}

	return bRet;
}
BOOL CAspBase::Asp_WriteSpi(PBYTE pCmd, DWORD dwCmdLen, PBYTE pData, DWORD dwDataLen, BOOL bSimult)
{
	BOOL bRet=FALSE;

	if(m_hASP != INVALID_HANDLE_VALUE)
	{
		SPIHDR SpiHdr;
		DWORD dwReturn;

		memset(&SpiHdr, 0, sizeof(SPIHDR));

		if (pCmd)
		{
			memcpy(SpiHdr.Cmd, pCmd, dwCmdLen);
		}

		SpiHdr.dwFlags = SPIHDR_PREPARE;

		SpiHdr.dwCmdLen = dwCmdLen;
		SpiHdr.pDataBuf = pData;
		SpiHdr.dwBufLen = dwDataLen;
		SpiHdr.dwBytesIn = 0;
		SpiHdr.bAsync = FALSE;
		SpiHdr.bWRSimult = bSimult;

		if (!DeviceIoControl(m_hASP, IOCTL_ASP_SPI_WRITE, &SpiHdr, sizeof(SPIHDR), NULL, 0, &dwReturn, NULL))
		{
			RETAILMSG(1, (TEXT("@@Failed Asp_WriteSpi error = 0x%x\r\n"), GetLastError()));
			bRet=FALSE;
		}
		else
		{
// 			if(pCmd)
// 				RETAILMSG(1, (TEXT("##Asp_WriteSpi SUCCEED! cmd=0x%x, dwDataLen=%d, pData[0]=0x%x\r\n"),*pCmd,dwDataLen,pData[0]));
// 			else
// 				RETAILMSG(1, (TEXT("##Asp_WriteSpi SUCCEED! dwDataLen=%d, pData[0]=0x%x\r\n"),dwDataLen,pData[0]));
			bRet=TRUE;
		}
	}

	return bRet;
}

BOOL CAspBase::Asp_WriteI2cReg(BYTE ulRegAddr, DWORD dwValue)
{

	BYTE data[2];
	data[0] = BYTE((dwValue & 0x0000ff00) >> 8);
	data[1] = BYTE((dwValue & 0x000000ff));

	//RETAILMSG(1, (TEXT("##%s: reg:0x%x, val:0x%x,0x%x\r\n"),TEXT(__FUNCTION__),ulRegAddr,data[0],data[1]));

	return Asp_WriteI2c(ulRegAddr,data,sizeof(data));
}
BOOL CAspBase::Asp_ReadI2cReg(BYTE ulRegAddr, PDWORD pdwValue)
{
	if(pdwValue)
	{
		BYTE data[2]={0,0};
		if(Asp_ReadI2c(ulRegAddr,data,sizeof(data)))
		{
			//RETAILMSG(1, (TEXT("##%s: reg:0x%x, val:0x%x,0x%x\r\n"),TEXT(__FUNCTION__),ulRegAddr,data[0],data[1]));
			*pdwValue = (data[0]<<8)+(data[1]);
			return TRUE;
		}
	}
	return 0;
}
