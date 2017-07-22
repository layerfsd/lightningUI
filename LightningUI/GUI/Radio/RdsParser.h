#pragma once

#include "../../stdafx.h"
#define DEBUG_RDS									0
#define DEBUG_ERR									1

#define ERR_RDS_OK							0
#define ERR_RDS_NO_DATA					1
#define ERR_RDS_DECODE_ERR				2
#define ERR_RDS_ABORT						3
#define ERR_RDS_PI_ONLY					4

#define RDS_GROUP_TYPE_A(i) (((i)<<1))
#define RDS_GROUP_TYPE_B(i) (((i)<<1)+1)

typedef enum _RdsBlockError
{
	RBE_None = 0,
	RBE_2Bits = 1,
	RBE_5Bits = 2,
	RBE_Uncorrectable = 3,
} RdsBlockError;

typedef enum _RdsBlockType
{
	RBT_BlockA = 0,
	RBT_BlockB = 1,
	RBT_BlockC = 2,
	RBT_BlockD = 3,
	RBT_BlockCPrime = 4,
	RBT_BlockE = 5,
	RBT_InvalidRBDS = 6,
	RBT_Invalid = 7,
} RdsBlockType;

typedef struct _AFData
{
	BYTE AFTotalNum;
	BYTE AFFilledNum;
	BYTE AFFreq[25];
}AFData;

typedef struct _RdsInfo
{
	//freq:
	DWORD freq;
	DWORD userData;
	//rds:
	WORD PICode;
	WORD PIN;
	BYTE TP;
	BYTE TA;
	BYTE	 PTY;
	BYTE DI;
	BYTE MS;
	BYTE PS[8];
	BYTE RT[64];
	AFData AF;
}RdsInfo;

typedef struct _RdsData
{
	WORD blockA;
	WORD blockB;
	WORD blockC;
	WORD blockD;
	WORD decErr;
}RdsData;


BYTE RdsGetGroupType(const RdsData *pData);
BOOL RdsParse(DWORD freq,const RdsData *pData,RdsInfo *pInfo);