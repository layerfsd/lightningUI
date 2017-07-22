/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company PrimaII BSP/CSP

 Copyright (c) 2010 - 2011  Cambridge Silicon Radio Limited, a CSR plc group
 company.

 All rights reserved.

 This Software is protected by United Kingdom copyright laws and international
 treaties.  You may not reverse engineer, decompile or disassemble this
 Software.

 WARNING:
 This Software contains Cambridge Silicon Radio Limited's confidential and
 proprietary information. UNAUTHORIZED COPYING, USE, MODIFICATION,
 DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL, REPACKAGING, REASSEMBLING
 OR DISCLOSURE OF THE WHOLE OR ANY PART OF THE SOFTWARE IS PROHIBITED AND MAY
 RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this Software without
 Cambridge Silicon Radio Limited's express written permission.   Use of any
 portion of the contents of this Software is subject to and restricted by your
 signed written agreement with Cambridge Silicon Radio Limited.
******************************************************************************/

#ifndef __CSPI2C_H_INCLUDED__
#define __CSPI2C_H_INCLUDED__

//I2C port number
typedef enum
{
    I2C_PORT_0 = 0,
    I2C_PORT_1,
    I2C_PORT_2,
    I2C_PORT_3,
    I2C_SOFT_PORT_0,
    I2C_SOFT_PORT_1,
    I2C_SOFT_PORT_2,
    I2C_PORT_MAX
} I2C_PORT_INDEX;
// Definitions for read/write sequence
#define     I2C_SEQ_SEG_DEVADDR_W      0x01 // Segment should be replaced
// by device address W
#define     I2C_SEQ_SEG_DEVADDR_R      0x02 // Segment should be replaced
// by device address R
#define     I2C_SEQ_SEG_REGADDR        0x03 // Segment should be replaced
// by register address
#define     I2C_SEQ_SEG_DATA           0x04 // Segment should be replaced
// by register data

// Definitions for segment protocol mask
#define     SEG_MASK_ALL               0xF0
#define     SEG_MASK_SEND_START        0x80 // Send start bit before send segment
#define     SEG_MASK_SEND_STOP         0x40 // Send stop bit after send segment
#define     SEG_MASK_NEED_ACK          0x20 // Need to wait ack after write
#define     SEG_MASK_NEED_NACK	       0x10 // Send NACK when reading segment finishes

#define		MAX_I2C_DEVICE_IN_SYS		6

#define		I2C_CLOCK_100K				100000
#define		I2C_CLOCK_200K				200000
#define		I2C_DEFAULT_CLOCK			I2C_CLOCK_100K


static const BYTE  g_pI2CDefaultWriteSeq[] = {I2C_SEQ_SEG_DEVADDR_W | SEG_MASK_SEND_START | SEG_MASK_NEED_ACK,
                                              I2C_SEQ_SEG_REGADDR | SEG_MASK_NEED_ACK,
                                              I2C_SEQ_SEG_DATA | SEG_MASK_SEND_STOP | SEG_MASK_NEED_ACK
                                             };

static const BYTE  g_pI2CDefaultReadSeq[] = {I2C_SEQ_SEG_DEVADDR_W | SEG_MASK_SEND_START | SEG_MASK_NEED_ACK,
                                             I2C_SEQ_SEG_REGADDR | SEG_MASK_NEED_ACK,
                                             I2C_SEQ_SEG_DEVADDR_R | SEG_MASK_SEND_START | SEG_MASK_NEED_ACK,
                                             I2C_SEQ_SEG_DATA | SEG_MASK_NEED_NACK | SEG_MASK_SEND_STOP
                                            };

typedef struct __I2C_DEVINFO
{
    ULONG  ulDevAddrW;   // Device address W
    ULONG  ulDevAddrR;   // Device address R
    PBYTE   pReadSeq;     // Buffer stored with read sequence, set it to NULL means
    // use the default read sequence
    UINT     uiReadSeqLen; // Length of the read sequence buffer in bytes, ignored if
    // pReadSeq is NULL.
    PBYTE   pWriteSeq;    // Buffer stored with write sequence, set it to NULL means
    // use the default write sequence
    UINT     uiWriteSeqLen;// Length of the write sequence buffer in bytes, ignored if
    // pWriteSeq is NULL.
    PVOID  pi2cbase;
    DWORD  dwReserve;
} I2C_DEVINFO, *PI2C_DEVINFO;

typedef struct _i2cTransaction
{
    I2C_DEVINFO  i2cdevinfo;
    //dwPort must be specified for kernel call.Drivers use I2C driver calls not need to set this.
    DWORD           dwPort;
    DWORD           dwClock;
    DWORD           dwRegAddr;
    BOOL              bRead;
    PBYTE             pValueBuf;
    UINT               uiValueLen;
} I2CTRANS;

//////////////////////////////////////////////////////////////////////////////////////
//	VOID I2cSocInitializeHW(PI2C_DEVINFO pDevHdl, DWORD dwClkRate, DWORD dwPort);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			dwClkRate -  The I2C clock rate in Hz
//			dwPort - The index of the I2C port
//	Return:	None
//	Remark:	This function initializes the specified i2c controller
//////////////////////////////////////////////////////////////////////////////////////
VOID	I2cSocInitializeHW(PI2C_DEVINFO pDevHdl, DWORD dwClkRate, DWORD dwPort);

//////////////////////////////////////////////////////////////////////////////////////
//	VOID I2cSocDeinitializeHW(PI2C_DEVINFO pDevHdl,  DWORD dwPort);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			dwPort - The index of the I2C port
//	Return:	None
//	Remark:	This function de-initializes the specified I2C port.
//////////////////////////////////////////////////////////////////////////////////////
VOID	I2cSocDeinitializeHW(PI2C_DEVINFO pDevHdl,  DWORD dwPort);

//////////////////////////////////////////////////////////////////////////////////////
//	BOOL I2cSocRead(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			ulRegAddr - The device address to read
//			pRegValBuf - The pointer to the data buffer which stores the data read back
//						 from the device
//			uiValBufLen - The number of bytes to read
//	Return:	TRUE - Success
//			FALSE - Failure
//	Remark:	This function reads the specified number of bytes from the specified address
//			of the I2C device
//////////////////////////////////////////////////////////////////////////////////////
BOOL	I2cSocRead(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);

//////////////////////////////////////////////////////////////////////////////////////
//	BOOL I2cSocWrite(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			ulRegAddr - The device address to read
//			pRegValBuf - The pointer to the data buffer which stores the data to be
//						 written to the device
//			uiValBufLen - The number of bytes to write
//	Return:	TRUE - Success
//			FALSE - Failure
//	Remark:	This function writes the specified number of bytes to the specified address
//			of the I2C device
//////////////////////////////////////////////////////////////////////////////////////
BOOL	I2cSocWrite(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);

//////////////////////////////////////////////////////////////////////////////////////
//	VOID I2cSoftInitializeHW(PI2C_DEVINFO pDevHdl, DWORD dwClkRate, DWORD dwPort);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			dwClkRate -  The I2C clock rate in Hz
//			dwPort - The index of the soft I2C port (start from 4)
//	Return:	None
//	Remark:	This function initializes the specified i2c port
//////////////////////////////////////////////////////////////////////////////////////
VOID	I2cSoftInitializeHW(PI2C_DEVINFO pDevHdl, DWORD dwClkRate, DWORD dwPort);

//////////////////////////////////////////////////////////////////////////////////////
//	VOID I2cSoftDeinitializeHW(PI2C_DEVINFO pDevHdl,  DWORD dwPort);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			dwPort - The index of the sotf I2C port
//	Return:	None
//	Remark:	This function de-initializes the specified I2C port.
//////////////////////////////////////////////////////////////////////////////////////
VOID	I2cSoftDeinitializeHW(PI2C_DEVINFO pDevHdl,  DWORD dwPort);

//////////////////////////////////////////////////////////////////////////////////////
//	BOOL I2cSoftRead(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			ulRegAddr - The device address to read
//			pRegValBuf - The pointer to the data buffer which stores the data read back
//						 from the device
//			uiValBufLen - The number of bytes to read
//	Return:	TRUE - Success
//			FALSE - Failure
//	Remark:	This function reads the specified number of bytes from the specified address
//			of the I2C device
//////////////////////////////////////////////////////////////////////////////////////
BOOL	I2cSoftRead(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);

//////////////////////////////////////////////////////////////////////////////////////
//	BOOL I2cSoftWrite(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);
//	Param:	pDevHdl - A pointer to structure of I2C_DEVINFO
//			ulRegAddr - The device address to read
//			pRegValBuf - The pointer to the data buffer which stores the data to be
//						 written to the device
//			uiValBufLen - The number of bytes to write
//	Return:	TRUE - Success
//			FALSE - Failure
//	Remark:	This function writes the specified number of bytes to the specified address
//			of the I2C device
//////////////////////////////////////////////////////////////////////////////////////
BOOL	I2cSoftWrite(PI2C_DEVINFO pDevHdl, ULONG ulRegAddr, PBYTE pRegValBuf, UINT uiValBufLen);
#endif  // __CSPI2C_H_INCLUDED__

