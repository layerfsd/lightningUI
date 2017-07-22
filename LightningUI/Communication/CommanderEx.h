#pragma once

#include "Util.h"
#include "keydef.h"
#include "Renesas_com.h"
#include "Commander.h"
#include "../GUI/Common/message_map.h"

#define DEBUG_COMMANDER_EX		0

typedef struct _RDS_DATA
{
	BYTE		bufPS[9];
	BYTE		bufRT[64];
	BYTE		bufTDC[64];
	BYTE		bufSTR[64];
	BYTE		bufRP[64];
	BYTE		rdsBlockData[10];
	WORD  rdsStatus;
}RDS_DATA,*LPRDS_DATA;

typedef struct _RADIO_SIGNAL_DATA
{
	WORD		signalStatus; // quality detector status
										//	[15] =AF_update flag
										//	0 = continuous quality data with time stamp
										//	1 = AF_Update sampled data
										//	[14:10] = reserved
										//	[9:0] = quality time stamp
										//	0 = tuning is in progress, no quality data available
										//	1 бн 320 (* 0.1 ms) = 0.1 бн 32 ms after tuning,
										//	quality data available, reliability depending on time stamp
										//	1000 = > 32 ms after tuning
										//	quality data continuously updated

	SHORT		level;		//level detector result
								//-200 бн 1200 (0.1 * dBuV) = -20 бн 120 dBuV RF input level
								//actual range and accuracy is limited by noise and agc

	WORD		usn;		//noise detector
								//FM 	ultrasonic noise detector
								//	0 бн 1000 (*0.1 %) = 0 бн 100% relative usn detector result

	WORD		wam;	//FM multipath detector
								//FM	 'wideband-AM' multipath detector
								//	0 бн 1000 (*0.1 %) = 0 бн 100% relative wam detector result

	WORD		offset;	//radio frequency offset
								//-1200 бн 1200 (*0.1 kHz) = -120 kHz бн 120 kHz radio frequency error
								//	actual range and accuracy is limited by noise and bandwidth

	WORD		bandwidth;	//IF bandwidth
										// FM	970 бн 3110 [*0.1 kHz] = IF bandwidth 97 бн 311 kHz; narrow бн wide
										//	AM	30 бн 80 [*0.1 kHz] = IF bandwidth 3 бн 8 kHz; narrow бн wide

	WORD		modulation;	//modulation detector
										// FM		0 бн 1000 [*0.1 %] = 0 бн 100% modulation = 0 бн 75 kHz FM dev.
										// 1000 бн 2000 [*0.1 %] = 100% бн 200% over-modulation range
										// (modulation results are an approximate indication of actual FM dev.)
										// AM		0 бн 1000 [*0.1 %] = 0 бн 100% AM modulation index
										// 1000 бн 2000 [*0.1 %] = 100% бн 200% peak modulation range
										// (peak modulation results vary depending on the modulation setup)
	WORD	    stopQuality;  //identify can stop or not.

}RADIO_SIGNAL_DATA,*LPRADIO_SIGNAL_DATA;

typedef struct _EXTENSIBLE_DATA
{
	BYTE	CmdID;
	BYTE	DataLen;
	BYTE	*pData;
	BYTE	CheckSum;
}MCU_EXTENSIBLE_DATA;

class CCommanderEx : 
	public CCommander
{
public:
	CCommanderEx(void);
	~CCommanderEx(void);

public:
	BYTE GetItemData(INT ID);
	void SetItemData(INT ID,BYTE val);
	void IniMCU();
	const BYTE *GetDiskCmdData()const{return m_dataDisk;}
	const RDS_DATA *GetRDSData()const{return &m_rdsData;}
	const RADIO_SIGNAL_DATA *GetRadioSignalData()const{return &m_radioSignalData;}
	const BYTE *GetVchicleInfoData()const{return m_dataVehicleInfo;}
	const BYTE *GetTpmsInfoData()const{return m_dataTpmsInfo;}

private:
	void PreProcess(LPBYTE lpCmdData,DWORD dwCmdLen);

private:
	BYTE		m_dataMCU[256];
	BYTE		m_bufCmd[8192];
	BYTE		m_dataDisk[2048];
	BYTE		m_dataVehicleInfo[4096];
	BYTE		m_dataTpmsInfo[1024];
	RDS_DATA	m_rdsData;
	RADIO_SIGNAL_DATA m_radioSignalData;

	DWORD	m_dwProcessPos;
	DWORD   m_dwCmdLen;

	//
	HANDLE	m_hAudioCodecACKEvent;

};
