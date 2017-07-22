#include "windows.h"
#include "AudioCodecI2C.h"

/******************************************************************************
*                   I2C INTERFACE FUNCTION DEFINE                             *
******************************************************************************/

static HANDLE  g_hI2CHandle = NULL;
static DWORD   g_dwOpenCount = 0;

static const BYTE  g_pI2CWriteSeq[] = {I2C_SEQ_SEG_DEVADDR_W | SEG_MASK_SEND_START | SEG_MASK_NEED_ACK,
											I2C_SEQ_SEG_REGADDR | SEG_MASK_NEED_ACK,
                                            I2C_SEQ_SEG_DATA | SEG_MASK_SEND_STOP | SEG_MASK_NEED_NACK
                                             };

static const BYTE  g_pI2CReadSeq[] = {I2C_SEQ_SEG_DEVADDR_W | SEG_MASK_SEND_START | SEG_MASK_NEED_ACK,
                                             I2C_SEQ_SEG_REGADDR | SEG_MASK_NEED_ACK,
											 I2C_SEQ_SEG_DEVADDR_R | SEG_MASK_SEND_START | SEG_MASK_NEED_ACK,
                                             I2C_SEQ_SEG_DATA | SEG_MASK_NEED_NACK | SEG_MASK_SEND_STOP
                                            };

DWORD AudioCodec_I2C_Open()
{
    if (g_hI2CHandle == NULL)
    {
        g_hI2CHandle = I2COpen(AUDIOCODEC_I2C_SYSCALL_PORT);
        g_dwOpenCount = 0;
    }
    if(g_hI2CHandle == NULL)
    {
        RETAILMSG(1, (TEXT("I2C_Open failed !!!\r\n")));
        return -6755;
    }
    else
    {
        RETAILMSG(1, (TEXT("I2C_Open successfully !!!\r\n")));
        g_dwOpenCount++;
    }
    return 0;
}


DWORD AudioCodec_I2C_Close()
{
    if (g_hI2CHandle && (--g_dwOpenCount == 0))
    {
        I2CClose(g_hI2CHandle);
        g_hI2CHandle = NULL;
    }
    return 0;
}

DWORD AudioCodec_I2C_Write(UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen)
{
	I2CTRANS  Trans;
    UINT      iRetry = 0;
    BOOL      bRet = FALSE;

	if(!g_hI2CHandle)
	{
		RETAILMSG(1,(TEXT("I2C_Write Error: g_hI2CHandle is NULL\r\n")));
		return -6705;
	}

	Trans.bRead = FALSE;
	Trans.dwRegAddr = ulRegAddr;
	Trans.pValueBuf = pRegValBuf;
	Trans.uiValueLen = uiValBufLen;
	Trans.dwClock = AUDIOCODEC_I2C_W_CLOCK;
	Trans.dwPort = AUDIOCODEC_NOSYSCALL_I2C_PORT;
	Trans.i2cdevinfo.pReadSeq = (PBYTE)g_pI2CReadSeq;
	Trans.i2cdevinfo.uiReadSeqLen = (UINT)sizeof(g_pI2CReadSeq);
	Trans.i2cdevinfo.pWriteSeq = (PBYTE)g_pI2CWriteSeq;
	Trans.i2cdevinfo.uiWriteSeqLen = (UINT)sizeof(g_pI2CWriteSeq);
	Trans.i2cdevinfo.ulDevAddrW = AUDIOCODEC_DEVADDR;
	Trans.i2cdevinfo.ulDevAddrR = Trans.i2cdevinfo.ulDevAddrW +1;

	do
	{
		bRet = I2CTransact(g_hI2CHandle, &Trans);
		if (!bRet)
		{
			iRetry++;
			Sleep(kI2cRetryDelayMics);
		}
	}while (iRetry<AUDIOCODEC_I2C_RETRY_TIME && !bRet);

    if(!bRet)
    {
        RETAILMSG(1,(TEXT("AudioCodec_I2C_Write: I2C write [0x%X] failed\r\n"),ulRegAddr));
        return -6747;
    }

	return 0;
}

DWORD AudioCodec_I2C_Read(UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen)
{
	I2CTRANS  Trans;
    UINT      iRetry=0;
    BOOL      bRet = FALSE;
    
	if(!g_hI2CHandle)
	{
		RETAILMSG(1,(TEXT("I2C_Read: g_hI2CHandle is NULL\r\n")));
		return -6705;
	}

	Trans.bRead = TRUE;
	Trans.dwRegAddr = ulRegAddr;
	Trans.pValueBuf = pRegValBuf;
	Trans.uiValueLen = uiValBufLen;
	Trans.dwClock = AUDIOCODEC_I2C_R_CLOCK;
	Trans.dwPort = AUDIOCODEC_NOSYSCALL_I2C_PORT;
	Trans.i2cdevinfo.pReadSeq = (PBYTE)g_pI2CReadSeq;
	Trans.i2cdevinfo.uiReadSeqLen = (UINT)sizeof(g_pI2CReadSeq);
	Trans.i2cdevinfo.pWriteSeq = (PBYTE)g_pI2CWriteSeq;
	Trans.i2cdevinfo.uiWriteSeqLen = (UINT)sizeof(g_pI2CWriteSeq);
	Trans.i2cdevinfo.ulDevAddrW = AUDIOCODEC_DEVADDR;
	Trans.i2cdevinfo.ulDevAddrR = Trans.i2cdevinfo.ulDevAddrW +1;

	do
	{
		bRet = I2CTransact(g_hI2CHandle, &Trans);
		if (!bRet)
		{
			iRetry++;
			Sleep(kI2cRetryDelayMics);
		}
	}while (iRetry<AUDIOCODEC_I2C_RETRY_TIME && !bRet);

    if(!bRet)
    {
        RETAILMSG(1,(TEXT("AudioCodec_I2C_Read: I2C read [0x%X] failed\r\n"),ulRegAddr));
        return -6746;
    }

	return 0;
}
