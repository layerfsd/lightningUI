#ifndef __IAP2I2C_H_
#define __IAP2I2C_H_

#include <windows.h>
#include "..\Communication\KerI2C.h"
#include "CarplayAPI.h"
#include "..\Config\customer.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                           I2C PORT/CLK DEFINE                               *
******************************************************************************/
#define IAP2_AUTHEN_IC_20C          0

#define IAP2_DEVADDR 	             0x22
#define IAP2_I2C_10K	             10000
#define IAP2_I2C_25K	             25000
#define IAP2_I2C_40K	             40000
#define IAP2_I2C_100K	             100000
#define IAP2_I2C_200K	             200000
#define IAP2_I2C_400K	             400000
#if IAP2_AUTHEN_IC_20C
#define IAP2_I2C_R_CLOCK             IAP2_I2C_400K
#define IAP2_I2C_W_CLOCK             IAP2_I2C_400K
#else
#define IAP2_I2C_R_CLOCK             IAP2_I2C_40K
#define IAP2_I2C_W_CLOCK             IAP2_I2C_40K
#endif
#define IA2P2_I2C_RETRY_TIME         10
#if (CVTE_MODEL_TYPE == CVTE_MODEL_TYPE_U705PM)
#define IAP2_I2C_SYSCALL_PORT        TEXT("I2C2:")
#define IAP2_NOSYSCALL_I2C_PORT      I2C_PORT_1
#else
#define IAP2_I2C_SYSCALL_PORT        TEXT("I2C1:")
#define IAP2_NOSYSCALL_I2C_PORT      I2C_PORT_0
#endif

#if IAP2_AUTHEN_IC_20C
#define kMFiI2cRetryDelayMics                   10
#else
#define kMFiI2cRetryDelayMics                   10
#endif

extern OSStatus IAP_I2C_Write(UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen);
extern OSStatus IAP_I2C_Read(UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen);
extern OSStatus IAP_I2C_Open();
extern OSStatus IAP_I2C_Close();

#ifdef __cplusplus
}
#endif

#endif
