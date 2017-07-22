#ifndef	__ASPTYPE_H__
#define	__ASPTYPE_H__

typedef void* HASP;

enum IICPort
{
	IICPort1,	//"I2C1:"
	IICPort2	//"I2C2:"
};
enum SPIPort
{
	SPIPort1,	//"SPI1:"
	SPIPort2,	//"SPI2:"
	SPIPort3	//"SPI3:"
};

enum AspType
{
	AspTypeNull=0,
	AspTypeFm1388,
	AspTypeAK7738,
};

enum AspDataType
{
	//control:
	AspData_Init,
	AspData_Sleep,
	AspData_Weakup,
	AspData_Run,

	//path data:
	AspData_Path_8K,
	AspData_Path_16K,
	AspData_Path_24K,
	AspData_Path_48K,

	//communication dsp data:
	AspData_DSP_CM_8K,
	AspData_DSP_CM_16K,
	AspData_DSP_CM_24K,

	//Speech recognition dsp data:
	AspData_DSP_SR_16K,
	AspData_DSP_SR_24K,

	//media playback dsp data:
	AspData_DSP_MP_48K,

	//
	AspData_Max_Cnt,
};

enum AspFirmwareType
{
	AspFirmware_1,
	AspFirmware_2,
	AspFirmware_3,
	AspFirmware_4,
	AspFirmware_max,
};

enum AspMode
{
	AspModeNull,			//Invalid Mode
	AspModeSR,				//Speech recognition (24K)
	AspModeCM_NB,			//communication narrow band (8K)
	AspModeCM_WB,			//communication wide band (16K)
	AspModeCM_SB,			//communication sup-wide band (24K)
	AspModeMedia			//media play mode(bypass) (48K)
};

enum AspHFStatus
{
	AspHFStatusIdle,
	AspHFStatusIncomingCall,
	AspHFStatusOutgoingCall,
	AspHFStatusTalking,
	AspHFStatusTerminating,
	AspHFStatusNoHeldCall,
	AspHFStatusHeldCallNoActiveCall,
};

typedef struct _ASP_DESC_
{
	AspType		type; 
	IICPort			iicPort; 
	SPIPort			spiPort; 
	DWORD		 gpioRstID;
	DWORD		iicDevAddr;
	DWORD		iicClock;
	DWORD		spiDCB;
	PDWORD		pDataConfig[AspData_Max_Cnt];
	DWORD		dwDataConfigLen[AspData_Max_Cnt];
	DWORD		dwFirmwareAddr[AspFirmware_max];
	LPCTSTR		strFirmwareFileName[AspFirmware_max];
}ASP_DESC;

#endif