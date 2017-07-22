#pragma once
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <pwinuser.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <winioctl.h>

#include "AspType.h"

#define IOCTL_ASP_CONFIG_PORT	CTL_CODE(FILE_DEVICE_UNKNOWN, 9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ASP_SPI_WRITE			CTL_CODE(FILE_DEVICE_UNKNOWN, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ASP_SPI_READ			CTL_CODE(FILE_DEVICE_UNKNOWN, 11, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ASP_I2C_WRITE			CTL_CODE(FILE_DEVICE_UNKNOWN, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ASP_I2C_READ			CTL_CODE(FILE_DEVICE_UNKNOWN, 13, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ASP_SET_SPI_DCB			CTL_CODE(FILE_DEVICE_UNKNOWN, 14, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ASP_SET_IIC_CLOCK			CTL_CODE(FILE_DEVICE_UNKNOWN, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)

//@Flags for Spi HDR status
#define     SPIHDR_PREPARE                0 //@Indicates that the HDR is in prepare stage
#define     SPIHDR_INQUEUE                1 //@Indicates that the HDR is in task queue
#define     SPIHDR_DOING                    2 //@Indicates that the HDR is accessing by spi driver
#define     SPIHDR_DONE                     3 //@Indicates that the HDR has been filled in application can use it to get information.
#define     SPIHDR_ABORT                    4 //@Indicates that the HDR has been aborted.


typedef struct _SPI_DCB
{
	ULONG		uClkFreq;//@Specifies clock of spi bus you will work on
	ULONG		uDataWidth;//@Specifies data width of you data, such as 8bits,16bits
	ULONG		uCsHoldTime;//@Specifies time of CS hold valid before and after data transfer
	ULONG		uCmdDelayTime;//@Specifies delay time between cmd and data receive
	BOOL		bClkTogInDelay;//@Specifies action of clock in delay.TRUE means toggle, FALSE means in idle state
	BOOL		bSlaveMode;//@Specifies mode. TRUE means Slave mode, FALSE means master mode
	BOOL		bClkIdleHigh;//@Specifies status of clock in idle. TRUE means high.FALSE means low
	BOOL		bCsIdleHigh;//@Specifies status of cs in idle. TRUE means high, FALSE means low
	BOOL		bMsb;//@Specifies which data transfer mode you selected. TRUE means msb, FALSE means lsb
	//Note: For bRsingEdge variable we set it at CPU side.
	//That means set bRsingEdge=TRUE,
	//at CPU side means drive data at rising edge,but at client chip means falling edge drive.
	//Set bRsingEdge=FALSE
	//at CPU side means drive data at falling edge,but at client chip means rising edge drive.
	BOOL		bRsingEdge;
} SPIDCB, *PSPIDCB;
typedef struct __SPI_HDR
{
	BYTE        Cmd[4];
	DWORD	  dwCmdLen;//@Specifies the cmd length in bytes.
	DWORD	  dwFlags;//@ indicates the status of buffer.
	LPBYTE	  pDataBuf;//@Pointer to data buffer
	ULONG	  uReserved;//@Reserved for DMA use,store DMA physical address
	PVOID       pReserved1;//@Reserved to store asynchorous marshelled data buffer pointer
	PVOID       pReserved2;//@Reserved to store synchorous marshelled (CeOpenCallerBuffer) data buffer pointer.
	PVOID       pReserved4;//@Reserved to store synchorous marshelled (CeOpenCallerBuffer)spi hdr pointer
	DWORD	  dwBufLen;//@Specifies the data length in bytes
	DWORD	  dwBytesIn;	 //@ Idicates how many data have been received
	BOOL        bAsync;//@Async or sync
	BOOL        bWRSimult;//@TRUE Write and Read simultaneous.
	BYTE        uReserved3[20];//@reserved for driver use,to create chain list.
} SPIHDR, *PSPIHDR;


typedef struct _ASP_I2C_TRANS_DATA 
{
	DWORD           dwDevAddr;
	DWORD           dwRegAddr;
	PBYTE             pValueBuf;
	UINT               uiValueLen;
}ASP_I2C_TRANS_DATA;

class CAspBase
{
public:
	CAspBase();
	~CAspBase(void);

public:
	virtual BOOL Initialize(IICPort iicPort, SPIPort spiPort, DWORD gpioRstID,DWORD iicDevAddr, DWORD iicClock, DWORD spiDCB);
	virtual BOOL ChangeWorkMode(AspMode idMode)=0;
	virtual BOOL SetMainVolume(int vol)=0;
	virtual BOOL SetAltVolume(int vol)=0;
	virtual BOOL Mute(BOOL bMute)=0;

	virtual BOOL ConfigSpi(DWORD spiDCB)=0;
	virtual BOOL ConfigIIC(DWORD iicClock)=0;
	virtual void testFunc(){}

protected:
	BOOL Asp_WriteI2c(DWORD ulRegAddr, PBYTE pBuffer, UINT uLength);
	BOOL Asp_ReadI2c(DWORD ulRegAddr, PBYTE pBuffer, UINT uLength);

	BOOL Asp_WriteSpi(PBYTE pCmd, DWORD dwCmdLen, PBYTE pData, DWORD dwDataLen, BOOL bSimult);
	BOOL Asp_ReadSpi(PBYTE pCmd, DWORD dwCmdLen, PBYTE pData, DWORD dwDataLen);

	BOOL Asp_WriteI2cReg(BYTE ulRegAddr, DWORD dwValue);
	BOOL Asp_ReadI2cReg(BYTE ulRegAddr, PDWORD pdwValue);

protected:
	HANDLE			m_hASP;
	DWORD           m_dwDevAddr;
	AspMode			m_aspWorkMode;
	DWORD			m_dwRstGpioId;
};

