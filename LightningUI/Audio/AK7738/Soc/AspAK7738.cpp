#include "AspAK7738.h"
#include "CVTE_GpioProxy.h"
#include "AspAK7738_ctrl_reg.h"
#include "AspAK7738_dsp1_pram.h"
#include "AspAK7738_dsp1_cram.h"
#include "AspAK7738_dsp2_pram.h"
#include "AspAK7738_dsp2_cram.h"
#include "AspAK7738_dsp2_oram.h"
#include "AspAK7738_dsp3_pram.h"
#include "AspAK7738_dsp3_cram.h"

#include "math.h"
#include "../include/TypeDef.h"
#include "../include/CommEndian.h"

#define SPI_SPEED			1
#define MAX_LOOP_TIMES		3

#ifndef PI
#define PI 3.1415926
#endif

#ifndef Write24
#define	Write24( PTR, X ) \
	do \
{ \
	( *(PTR+0) ) = (BYTE)( ( (X) >> 16 ) & 0xFF ); \
	( *(PTR+1) ) = (BYTE)( ( (X) >>  8 ) & 0xFF ); \
	( *(PTR+2) ) = (BYTE)(   (X)         & 0xFF ); \
	PTR += 3; \
	\
}	while( 0 )
#endif

static BYTE g_ParamBuf[128] = {0};

CAspAK7738::CAspAK7738()
{
	m_iSamplingFreq = 48000;
}

CAspAK7738::~CAspAK7738(void)
{

}

BOOL CAspAK7738::Initialize(IICPort iicPort, SPIPort spiPort, DWORD gpioRstID,DWORD iicDevAddr, DWORD iicClock, DWORD spiDCB)
{
	DWORD dwStart=GetTickCount();
	RETAILMSG(DEBUG_ASP7738,(TEXT("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));

	BOOL bRet = FALSE;
	BYTE nRetryCount=0;
	static BOOL bInit = FALSE;
	if (!bInit)
	{
		bInit = CAspBase::Initialize(IICPort1, spiPort, gpioRstID, 0, 0, spiDCB);
		if (!bInit)
		{
			RETAILMSG(1,(TEXT("ERR:[%s]: =>Initialize FAILED!!!\r\n"), TEXT(__FUNCTION__)));
			return FALSE;
		}
	}

	do 
	{
		resetDevice();
		Sleep(2);

		if (!sendSpiDummyCmd())
		{
			RETAILMSG(1,(TEXT("ERR:[%s]: =>sendSpiDummyCmd FAILED!!!\r\n"), TEXT(__FUNCTION__)));
			continue;;
		}
		Sleep(5);
		bRet = checkDeciceID();

	} while (!bRet && nRetryCount++ < 3);


	if(!bRet)
	{
		RETAILMSG(1,(TEXT("MSG:[%s]: checkDeciceID failed!!! nRetryCount=%d\r\n"),TEXT(__FUNCTION__),nRetryCount));

		return FALSE;
	}
	RETAILMSG(DEBUG_ASP7738,(TEXT("MSG:[%s]: checkDeciceID succeed, nRetryCount=%d\r\n"),TEXT(__FUNCTION__),nRetryCount));
	//TestIC();
	FUNC_CHECK(writeParamArray(ak7738InitReg, _countof(ak7738InitReg)));
	Sleep(10);
	FUNC_CHECK(writeParamArray(ak7738CtrlReg, _countof(ak7738CtrlReg)));
	Sleep(10);
	//Download DSP
	FUNC_CHECK(writeDspInitParam(ak77dsp1PRAM, sizeof(ak77dsp1PRAM)));
	FUNC_CHECK(writeDspInitParam(ak77dsp1CRAM, sizeof(ak77dsp1CRAM)));

	FUNC_CHECK(writeDspInitParam(ak77dsp2PRAM, _AK77XX_DSP_PRAM_BUF_SIZE));
	FUNC_CHECK(writeDspInitParam(ak77dsp2CRAM, _AK77XX_DSP_CRAM_BUF_SIZE));
	FUNC_CHECK(writeDspInitParam(ak77dsp2ORAM, _AK77XX_DSP_ORAM_BUF_SIZE));

	FUNC_CHECK(writeDspInitParam(ak77dsp3PRAM, sizeof(ak77dsp3PRAM)));
	FUNC_CHECK(writeDspInitParam(ak77dsp3CRAM, sizeof(ak77dsp3CRAM)));

	Sleep(10);
	//Power on DSP and Codec
	BYTE run = 0x1F;
	return write(CMD_WRITE_CONTROL_REG,0x0085,&run,1);
}


BOOL CAspAK7738::ChangeWorkMode(AspMode mode)
{
	return TRUE;
}

BOOL CAspAK7738::SetMainVolume(int vol)
{
	return TRUE;
}

BOOL CAspAK7738::SetAltVolume(int vol)
{
	return TRUE;
}
BOOL CAspAK7738::Mute(BOOL bMute)
{
	return TRUE;
}

BOOL CAspAK7738::resetDevice()
{
	return TRUE;

	BOOL bRet=FALSE;
	HANDLE hdl=GPIOOpen();

	RETAILMSG(1,(TEXT("MSG:[%s]: gpioID=%d\r\n"), TEXT(__FUNCTION__), m_dwRstGpioId));

	if(m_dwRstGpioId != -1)
	{
		//group1-index29

		if(hdl!=INVALID_HANDLE_VALUE)
		{
			DWORD id=m_dwRstGpioId;

			if(!GPIOPadEnable(hdl, id))
				goto _exit;
			if(!GPIOPullDown(hdl, id))
				goto _exit;
			if(!GPIOOutputEnable(hdl, id))
				goto _exit;
			if(!GPIOOutputZero(hdl, id))
				goto _exit;

			Sleep(80);

			if(!GPIOPadEnable(hdl, id))
				goto _exit;
			if(!GPIOPullUp(hdl,id))
				goto _exit;
			if(!GPIOOutputEnable(hdl, id))
				goto _exit;
			if(!GPIOOutputOne(hdl, id))
				goto _exit;

			bRet = TRUE;
		}
	}

_exit:

	if(hdl!=INVALID_HANDLE_VALUE)
	{
		GPIOClose(hdl);
	}

	RETAILMSG(DEBUG_ASP7738,(TEXT("MSG:[%s]: resetDevice, bRet=%d\r\n"),TEXT(__FUNCTION__), bRet));

	return bRet;
}

void CAspAK7738::setSamplingFreq(int freq)
{
	m_iSamplingFreq = freq;
}

BOOL CAspAK7738::ConfigSpi(DWORD spiDCB)
{
	DWORD dwReturn;
	SPIDCB   dcb;
	//Configure DCB
	dcb.uClkFreq = spiDCB ? spiDCB : (1000000 * SPI_SPEED); // spi clock. 1M
	dcb.uDataWidth = 8;
	dcb.uCsHoldTime = 300;//10;
	dcb.uCmdDelayTime = 0;//100;
	dcb.bClkTogInDelay = FALSE;
	dcb.bSlaveMode = FALSE;
	dcb.bClkIdleHigh = TRUE;
	dcb.bCsIdleHigh = TRUE;
	dcb.bMsb = TRUE;
	dcb.bRsingEdge = FALSE;	//FALSE;

	if (!DeviceIoControl(m_hASP, IOCTL_ASP_SET_SPI_DCB, &dcb, sizeof(SPIDCB), NULL, 0, &dwReturn, NULL))
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: error = 0x%x\r\n"),TEXT(__FUNCTION__), GetLastError()));
		return FALSE;
	}
	else
	{
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: uClkFreq=%d\r\n"),TEXT(__FUNCTION__),dcb.uClkFreq));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: uDataWidth=%d\r\n"),TEXT(__FUNCTION__),dcb.uDataWidth));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: uCsHoldTime=%d\r\n"),TEXT(__FUNCTION__),dcb.uCsHoldTime));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: uCmdDelayTime=%d\r\n"),TEXT(__FUNCTION__),dcb.uCmdDelayTime));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: bClkTogInDelay=%d\r\n"),TEXT(__FUNCTION__),dcb.bClkTogInDelay));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: bSlaveMode=%d\r\n"),TEXT(__FUNCTION__),dcb.bSlaveMode));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: bClkIdleHigh=%d\r\n"),TEXT(__FUNCTION__),dcb.bClkIdleHigh));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: bCsIdleHigh=%d\r\n"),TEXT(__FUNCTION__),dcb.bCsIdleHigh));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: bMsb=%d\r\n"),TEXT(__FUNCTION__),dcb.bMsb));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: bRsingEdge=%d\r\n"),TEXT(__FUNCTION__),dcb.bRsingEdge));
		RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: ConfigSpi SUCCEED! \r\n"),TEXT(__FUNCTION__)));
		return TRUE;
	}
}

BOOL CAspAK7738::ConfigIIC(DWORD iicClock)
{
	return TRUE;
}


//void CAspAK7738::TestIC()
//{
//	for (int i=0;i<0x105;i++)
//	{
//		BYTE    regvalue = 0xAA;
//		BYTE	cmd[3];// = {0x40, 0x00, 0x00};
//		cmd[2] = CMD_READ_CONTROL_REG;
//		cmd[1] = ((i>>8)&0xFF); 
//		cmd[0] = (i&0xFF);
//
//		if (!Asp_ReadSpi(cmd, sizeof(cmd), &regvalue, sizeof(regvalue)))
//		{
//			RETAILMSG(1,(TEXT("ERR:[%s]: FAILED!!!\r\n"), TEXT(__FUNCTION__)));
//			return;
//		}
//		RETAILMSG(1,(TEXT("MSG:[%s]: Reg[0x%02X%02X] regvalue[0x%X] \r\n"), TEXT(__FUNCTION__),cmd[1],cmd[0],regvalue));
//	}
//}

//ASP operation:
BOOL CAspAK7738::writeParamArray(CAspRegValue* pParam, size_t count)
{
	if (pParam == NULL)
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: pParam is NULL !!!\r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}

	for(size_t i=0;i<count;i++)
	{
		if (!write(CMD_WRITE_CONTROL_REG, pParam[i].RegAddr, &(pParam[i].RegValue),1))
		{
			RETAILMSG(1,(TEXT("ERR:[%s]: write[0x%X] Failed!!!\r\n"), TEXT(__FUNCTION__),pParam[i].RegAddr));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CAspAK7738::writeDspInitParam(unsigned char* pParam, size_t len)
{
	if (pParam == NULL)
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: pParam is NULL !!!\r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}

	BYTE cmd = pParam[0];
	UINT16 addr = (pParam[1] << 8) + pParam[2];
	RETAILMSG(DEBUG_ASP7738,(TEXT("MSG:[%s]: write cmd[0x%X] addr[0x%02X 0x%02X]\r\n"), TEXT(__FUNCTION__),cmd, pParam[1], pParam[2]));

	if (!write(cmd, addr, &pParam[3],(len-3)))
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: write[0x%X] Failed!!!\r\n"), TEXT(__FUNCTION__),addr));
		return FALSE;
	}

	return TRUE;
}


//BOOL CAspAK7738::writeDspInitParam(BYTE nCmd, UINT16 uAddr, PBYTE pData, DWORD dwDataLen)
//{
//	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
//
//	UINT16 uCRC = calculateCRC(pData, dwDataLen);
//	BYTE looptime = MAX_LOOP_TIMES;
//
//	do 
//	{
//		if (download(nCmd, uAddr, pData, dwDataLen, uCRC))
//			break;
//	} while (looptime--);
//
//	return looptime?TRUE:FALSE;
//}

BOOL CAspAK7738::writeBits(BYTE cmd, UINT16 addr, BYTE mark, BYTE data)
{
	BYTE nCmd[3];
	BYTE Value = mark & data;
	nCmd[0] = (addr & 0xFF);
	nCmd[1] = ((addr >> 8) & 0xFF);
	nCmd[2] = cmd;

	return Asp_WriteSpi(nCmd, sizeof(nCmd), &Value, 1, FALSE);
}

BOOL CAspAK7738::write(BYTE cmd, UINT16 addr, PBYTE pData, DWORD dwDataLen)
{
	BYTE nCmd[3];
	nCmd[0] = (addr & 0xFF);
	nCmd[1] = ((addr >> 8) & 0xFF);
	nCmd[2] = cmd;

	return Asp_WriteSpi(nCmd, sizeof(nCmd), pData, dwDataLen,FALSE);
}

BOOL CAspAK7738::read(BYTE cmd, UINT16 addr, PBYTE pData, DWORD dwDataLen)
{
	BYTE nCmd[3];
	nCmd[0] = (addr & 0xFF);
	nCmd[1] = ((addr >> 8) & 0xFF);
	nCmd[2] = cmd;

	return Asp_ReadSpi(nCmd, sizeof(nCmd), pData, dwDataLen);
}

UINT16 CAspAK7738::calculateCRC(PBYTE pData, DWORD dwDataLen)
{
	UINT16 crc = 0x0000;
	DWORD i, j;

	for ( i = 0; i < dwDataLen; i++ ) 
	{
		crc ^= *pData++ << 8;
		for ( j = 0; j < 8; j++) 
		{
			if ( crc & 0x8000) 
			{
				crc <<= 1;
				crc ^= CRC16_CCITT;
			}
			else 
			{
				crc <<= 1;
			}
		}
	}

	RETAILMSG(DEBUG_ASP7738, (TEXT("MSG:[%s]: CRC = 0x%x\r\n"), TEXT(__FUNCTION__),crc));

	return crc;
}

UINT16 CAspAK7738::readCRC()
{
	BYTE rx[2];
	BOOL bRet = read(CMD_READ_CRC, 0, rx, 2);
	return bRet?((rx[0] << 8) + rx[1]):0;
}

BOOL CAspAK7738::setDspStatus(AK7738Status status)
{
	//switch(status)
	//{
	//case AK7738_STATUS_RUN1:
	//	writeBits(AK7738_01_STSCLOCK_SETTING2, 0x80, 0x80);	// CKRESETN bit = 1
	//	Sleep(10);
	//	writeBits(AK7738_85_RESET_CTRL, 0x99, 0x19);			// CRESETN bit = D1RESETN = HRESETN = 1;
	//	break;
	//case AK7738_STATUS_RUN2:
	//	writeBits(AK7738_01_STSCLOCK_SETTING2, 0x80, 0x80);	// CKRESETN bit = 1
	//	Sleep(10);
	//	writeBits(AK7738_85_RESET_CTRL, 0x95, 0x15);			// CRESETN bit = D2RESETN = HRESETN = 1;
	//	break;
	//case AK7738_STATUS_RUN3:
	//	writeBits(AK7738_01_STSCLOCK_SETTING2, 0x80, 0x80);	// CKRESETN bit = 1
	//	Sleep(10);
	//	writeBits(AK7738_85_RESET_CTRL, 0x93, 0x13);			// CRESETN bit = D3RESETN = HRESETN = 1;
	//	break;
	//case AK7738_STATUS_DOWNLOAD:
	//	writeBits(AK7738_85_RESET_CTRL, 0x80, 0x80);			// DLRDY bit = 1
	//	break;
	//case AK7738_STATUS_DOWNLOAD_FINISH:
	//	writeBits(AK7738_85_RESET_CTRL, 0x80, 0x0);			// DLRDY bit = 0
	//	break;
	//case AK7738_STATUS_STANDBY:
	//	writeBits(AK7738_01_STSCLOCK_SETTING2, 0x80, 0x80);	// CKRESETN bit = 1
	//	Sleep(10);
	//	writeBits(AK7738_85_RESET_CTRL, 0x9F, 0x00);
	//	break;
	//case AK7738_STATUS_SUSPEND:
	//case AK7738_STATUS_POWERDOWN:
	//	writeBits(AK7738_85_RESET_CTRL, 0x9F, 0x00); 
	//	writeBits(AK7738_01_STSCLOCK_SETTING2, 0x80, 0x00);
	//	break;
	//default:
	//	break;
	//}
	return TRUE;
}
//
//BOOL CAspAK7738::download(BYTE nCmd, UINT16 addr, PBYTE pData, DWORD dwDataLen, UINT16 nCRC)
//{
//	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
//
//	BYTE nDSPRun;
//	UINT16 readCRC;
//	BOOL bRet = TRUE;
//	FUNC_CHECK(read(CMD_READ_CONTROL_REG, AK7738_85_RESET_CTRL, &nDSPRun, 1));
//	FUNC_CHECK(setDspStatus(AK7738_STATUS_DOWNLOAD));
//	FUNC_CHECK(write(nCmd, addr, pData, dwDataLen));
//
//	if (nCRC)
//	{
//		readCRC = readCRC();
//		bRet = (readCRC == nCRC);
//	}
//
//	write(CMD_WRITE_CONTROL_REG, AK7738_85_RESET_CTRL, &nDSPRun, 1);
//	return bRet;
//}

BOOL CAspAK7738::sendSpiDummyCmd()
{
	RETAILMSG(DEBUG_ASP7738,(TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));

	BYTE	cmd[4] = {0xDE, 0xAD, 0xDA, 0x7A};
	return Asp_WriteSpi(NULL, 0, cmd, sizeof(cmd), FALSE);
}

BOOL CAspAK7738::checkDeciceID()
{
	RETAILMSG(DEBUG_ASP7738,(TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	BYTE	deviceid;
	BYTE cmd[3];
	cmd[2] = (BYTE)(CMD_READ_CONTROL_REG);
	cmd[1] = (BYTE)(0xFF & (AK7738_100_DEVICE_ID>>8));
	cmd[0] = (BYTE)(0xFF & AK7738_100_DEVICE_ID); 

	if (!CAspBase::Asp_ReadSpi(cmd, 3, &deviceid, 1))
	{
		RETAILMSG(1,(TEXT("ERR:[%s]: FAILED!!!\r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}

	RETAILMSG(1,(TEXT("MSG:[%s]: deviceid[0x%X]\r\n"), TEXT(__FUNCTION__),deviceid));
	
	return (deviceid == 0x38);
}

//x,	 Float Point data input
//shift, Shift amount
int CAspAK7738::fix24(double x, int shift)
{
	double temp;
	temp = x * pow(2,(23 - shift));
	temp = min(8388607, temp);
	temp = max(-8388608, temp);
	if ( temp < 0 ) 
		temp += 16777216;
	return int( sat(temp + 0.5, 0, 16777215) );
}

// MSB 16bit when Double Precision
int CAspAK7738::fix24H(double x, int shift)
{
	double temp;
	temp = x * pow(2,(15 - shift));
	temp = min(32767.99998474, temp);
	temp = max(-32768, temp);

	if (temp < 0)
		temp += 65536;

	temp = int( sat(temp, 0, 65535) );
	return int(temp * 256);	// Extend to 24bit
}

// LSB 23bit when Double Precision
int CAspAK7738::fix24L(double x, int shift)
{
	double temp;
	temp = x * pow(2,(15 - shift));
	temp = min(32767.99998474, temp);
	temp = max(-32768, temp);

	if (temp < 0)
		temp += 65536;

	temp = (temp - int(temp)) * 8388608;
	return int( sat(temp + 0.5, 0, 8388607));
}

PBYTE CAspAK7738::getEQSettingBuff(float freq, float val, BOOL bDoublePrecision)
{
#if DEBUG_ASP7738
	printf("getEQSettingBuff: freq:%g val:%g bDbPrecision:%d\r\n",freq,val,bDoublePrecision);
#endif
	double w = 2 * PI * freq / m_iSamplingFreq;		//120 Hz
	double sn = sin(w);
	double cs = cos(w);
	double alpha = sn / (2 * 1.4142);
	double A = pow(10, (val/40.0));
	double p0 = 1 + alpha / A;
	double a2 = (1 - alpha * A) / p0;
	double a1 = -(2 * cs) / p0;
	double b2 = -(1 - alpha / A)/p0;
	double b1 = 2 * cs / p0;
	double a0 = (1 + alpha * A) / p0;
	int    shift = 1;

	if (bDoublePrecision)
	{
		int C0 = fix24H(a2, shift);
		int C1 = fix24L(a2, shift);
		int C2 = fix24H(a1, shift);
		int C3 = fix24L(a1, shift);
		int C4 = fix24H(b2, shift);
		int C5 = fix24L(b2, shift);
		int C6 = fix24H(b1, shift);
		int C7 = fix24L(b1, shift);
		int C8 = fix24H(a0, shift);
		int C9 = fix24L(a0, shift);	

		//RETAILMSG(DEBUG_ASP7738,(_T("C0[0x%06X]\r\n"), C0));
		//RETAILMSG(DEBUG_ASP7738,(_T("C1[0x%06X]\r\n"), C1));
		//RETAILMSG(DEBUG_ASP7738,(_T("C2[0x%06X]\r\n"), C2));
		//RETAILMSG(DEBUG_ASP7738,(_T("C3[0x%06X]\r\n"), C3));
		//RETAILMSG(DEBUG_ASP7738,(_T("C4[0x%06X]\r\n"), C4));
		//RETAILMSG(DEBUG_ASP7738,(_T("C5[0x%06X]\r\n"), C5));
		//RETAILMSG(DEBUG_ASP7738,(_T("C6[0x%06X]\r\n"), C6));
		//RETAILMSG(DEBUG_ASP7738,(_T("C7[0x%06X]\r\n"), C7));
		//RETAILMSG(DEBUG_ASP7738,(_T("C8[0x%06X]\r\n"), C8));
		//RETAILMSG(DEBUG_ASP7738,(_T("C9[0x%06X]\r\n"), C9));

		PBYTE	pBuf = g_ParamBuf;
		memset(g_ParamBuf, 0, sizeof(g_ParamBuf));
		Write24(pBuf, C0);
		Write24(pBuf, C1);
		Write24(pBuf, C2);
		Write24(pBuf, C3);
		Write24(pBuf, C4);
		Write24(pBuf, C5);
		Write24(pBuf, C6);
		Write24(pBuf, C7);
		Write24(pBuf, C8);
		Write24(pBuf, C9);

		return g_ParamBuf;
	}
	else
	{
		int C0 = fix24(a2, shift);
		int C1 = fix24(a1, shift);
		int C2 = fix24(b2, shift);
		int C3 = fix24(b1, shift);
		int C4 = fix24(a0, shift);

		//RETAILMSG(DEBUG_ASP7738,(_T("MSG:[%s]: [0x%06X]\r\n"),TEXT(__FUNCTION__), C0));
		//RETAILMSG(DEBUG_ASP7738,(_T("MSG:[%s]: [0x%06X]\r\n"),TEXT(__FUNCTION__), C1));
		//RETAILMSG(DEBUG_ASP7738,(_T("MSG:[%s]: [0x%06X]\r\n"),TEXT(__FUNCTION__), C2));
		//RETAILMSG(DEBUG_ASP7738,(_T("MSG:[%s]: [0x%06X]\r\n"),TEXT(__FUNCTION__), C3));
		//RETAILMSG(DEBUG_ASP7738,(_T("MSG:[%s]: [0x%06X]\r\n"),TEXT(__FUNCTION__), C4));

		PBYTE	pBuf = g_ParamBuf;
		memset(g_ParamBuf, 0, sizeof(g_ParamBuf));
		ChangePointWrite24(pBuf, C0);
		ChangePointWrite24(pBuf, C1);
		ChangePointWrite24(pBuf, C2);
		ChangePointWrite24(pBuf, C3);
		ChangePointWrite24(pBuf, C4);

		return g_ParamBuf;
	}
}

int CAspAK7738::getEQSettingSize(BOOL bDoublePrecision)
{
	return bDoublePrecision?30:15;
}

PBYTE CAspAK7738::getFaderSettingBuff(float db, int attacktime, int releasetime)
{
#if DEBUG_ASP7738
	printf("getEQSettingBuff: db:%g attacktime:%d releasetime:%d\r\n",db,attacktime,releasetime);
#endif

	double a = (db <= -144)?0:(pow(10,(db/20.0)));
	double ta = (1.0 - 1.0 / (1 + attacktime * 0.001 * m_iSamplingFreq));
	double tr = (1.0 - 1.0 / (1 + releasetime * 0.001 * m_iSamplingFreq));

	int	C0 = fix24(a, 2);
	int C1 = fix24H(ta, 0);
	int C2 = fix24L(ta, 0);
	int C3 = fix24H(tr, 0);
	int C4 = fix24L(tr, 0);

	PBYTE	pBuf = g_ParamBuf;
	memset(g_ParamBuf, 0, sizeof(g_ParamBuf));
	Write24(pBuf, C0);
	Write24(pBuf, C1);
	Write24(pBuf, C2);
	Write24(pBuf, C3);
	Write24(pBuf, C4);

	return g_ParamBuf;
}

int CAspAK7738::getFaderSettingSize()
{
	return 15;
}

PBYTE CAspAK7738::getVolumeSettingBuff(float db)
{
	double a = (db <= -144)?0:(pow(10,(db/20.0)));

	int	C0 = fix24(a, 2);
	//printf("MSG: db[%g] val[0x%06X]\r\n",db, C0);
	PBYTE	pBuf = g_ParamBuf;
	memset(g_ParamBuf, 0, sizeof(g_ParamBuf));
	Write24(pBuf, C0);

	return g_ParamBuf;
}

int CAspAK7738::getVolumeSettingSize()
{
	return 3;
}