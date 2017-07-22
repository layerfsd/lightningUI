#ifndef __AUDIOCODECI2C_H_
#define __AUDIOCODECI2C_H_

#include "KerI2C.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                           I2C PORT/CLK DEFINE                               *
******************************************************************************/

#define AUDIOCODEC_DEVADDR 	             0x30
#define AUDIOCODEC_I2C_10K	             10000
#define AUDIOCODEC_I2C_25K	             25000
#define AUDIOCODEC_I2C_40K	             40000
#define AUDIOCODEC_I2C_100K	             100000
#define AUDIOCODEC_I2C_200K	             200000
#define AUDIOCODEC_I2C_400K	             400000
#define AUDIOCODEC_I2C_R_CLOCK           AUDIOCODEC_I2C_400K
#define AUDIOCODEC_I2C_W_CLOCK           AUDIOCODEC_I2C_400K
#define AUDIOCODEC_I2C_RETRY_TIME        10
#define AUDIOCODEC_I2C_SYSCALL_PORT      TEXT("I2C1:")
#define AUDIOCODEC_NOSYSCALL_I2C_PORT    I2C_PORT_0

#define kI2cRetryDelayMics               10

extern DWORD AudioCodec_I2C_Write(UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen);
extern DWORD AudioCodec_I2C_Read(UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen);
extern DWORD AudioCodec_I2C_Open();
extern DWORD AudioCodec_I2C_Close();

#ifdef __cplusplus
}
#endif

#endif
