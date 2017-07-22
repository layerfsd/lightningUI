#pragma once

#include "../../stdafx.h"
#include "RdsParser.h"
#include <pkfuncs.h>
#include "../../Config/customer.h"
#include "../../Communication/CommanderEx.h"
//0:Europe:				fm 87.5-108.0MHz, am 522-1620KHz
//1:USA:					fm 87.5-107.9MHz, am 530-1710KHz
//2:EasternEurope:	fm 65.00-74.0MHz, am 522-1620KHz
//3:Japan:					fm 76.0-90.0MHz, am 522-1629KHz
//4:Korea:					fm 87.5-108.0MHz, am 531-1719KHz
//5:Row:					fm 87.5-108.0MHz, am 522-1620KHz

#define DEBUG_RADIO								1

#define ENABLE_RADIO_DEBUG_SCREEN	1

//radio commands:
#define RADIO_CMD_SET_PLAYSTATE					1
#define RADIO_CMD_SET_FM_STOP_LEVEL			2
#define RADIO_CMD_SET_AM_STOP_LEVEL			3
#define RADIO_CMD_RESET_STOP_LEVEL				4
#define RADIO_CMD_SELECT_CHANNEL				5
#define RADIO_CMD_SELECT_BAND						6
#define RADIO_CMD_AMS									7
#define RADIO_CMD_SEARCH								8
#define RADIO_CMD_SEEK									9
#define RADIO_CMD_SET_FREQ							10
#define RADIO_CMD_RDS_SWITCH						11
#define RADIO_CMD_AF_SWITCH							12
#define RADIO_CMD_TA_SWITCH							13
#define RADIO_CMD_PTY_SWITCH						14
#define RADIO_CMD_REG_SWITCH						15
#define RADIO_CMD_SELECT_PTY							16
#define RADIO_CMD_QUERY_STORAGE_CNT		17
#define RADIO_CMD_QUERY_FREQ						18
#define RADIO_CMD_SET_REGION						19
#define RADIO_CMD_SET_LOCAL_ONOFF				20
#define RADIO_CMD_ADD_FAVORITE						21
#define RADIO_CMD_DELETE_FAVORITE						22
#define RADIO_CMD_TOGGLE_FAVORITE						23
#define RADIO_CMD_QUERY_FAVORITE_STATUS			24
#define RADIO_CMD_PLAY_NEXT									25
#define RADIO_CMD_PLAY_PREVIOUS							26
#define RADIO_CMD_SWITCH_AUDIO_SRC					27
#define RADIO_CMD_ENTER_MODE								28
#define RADIO_CMD_CANCEL_TA									29
#define RADIO_CMD_SET_PI											30
#define RADIO_CMD_ENABLE_DEBUG_SCREEN				31

enum RadioFreqSetMode
{
	FREQ_MODE_NONE,
	FREQ_MODE_PRESET,
	FREQ_MODE_SEARCH,
	FREQ_MODE_AF_UPDATE,
	FREQ_MODE_AF_JUMP,
	FREQ_MODE_AF_CHECK,
	FREQ_MODE_RESERVED,
	FREQ_MODE_END,
};

typedef struct _RadioRegionParam
{
	int 			iRegionID;
	DWORD 			dwFMFrepMin;
	DWORD 			dwFMFrepMax;
	int             iFMStep;
	DWORD 			dwAMFrepMin;
	DWORD 			dwAMFrepMax;
	int             iAMStep;
}RadioRegionParam;

typedef enum _RadioRegionId
{
	RADIO_REGION_Europe             = CVTE_DEF_RADIO_REGION_EUROPE,						// 0
	RADIO_REGION_USA                = CVTE_DEF_RADIO_REGION_USA,						// 1
	RADIO_REGION_EasternEurope     = CVTE_DEF_RADIO_REGION_EAST_ERNEUROPE,				// 2
	RADIO_REGION_Japan              = CVTE_DEF_RADIO_REGION_JAPAN,						// 3
	RADIO_REGION_Euro_EastEuro     = CVTE_DEF_RADIO_REGION_EUROPE_EASTERNEUROPE,		// 4
	RADIO_REGION_Korean             = CVTE_DEF_RADIO_REGION_KOREAN,						// 5
	RADIO_REGION_TaiWang            = CVTE_DEF_RADIO_REGION_TAIWANG,					// 6
	RADIO_REGION_USA2				= CVTE_DEF_RADIO_REGION_USA2,						// 7
	RADIO_REGION_SouthAmerica      = CVTE_DEF_RADIO_REGION_SOUTHAMERICA,				// 8
	RADIO_REGION_ArabiaAustralia    = CVTE_DEF_RADIO_REGION_ARABIA_AUSTRALIA,			// 9
	RADIO_REGION_LatinAmerica       = CVTE_DEF_RADIO_REGION_LATIN_AMERICA,				// 10
	RADIO_REGION_MiddleEast         = CVTE_DEF_RADIO_REGION_MIDDLE_EAST,				// 11
	RADIO_REGION_Aust               = CVTE_DEF_RADIO_REGION_AUST,						// 12
	RADIO_REGION_Brazil             = CVTE_DEF_RADIO_REGION_BRAZIL,						// 13
	//RADIO_REGION_Russia             = CVTE_DEF_RADIO_REGION_RUSSIA,						// 14
	RADIO_REGION_EastEuro_Euro		= CVTE_DEF_RADIO_REGION_EASTERNEUROPE_EUROPE,		//15
	RADIO_REGION_Max                = CVTE_DEF_RADIO_REGION_MAX,
	RADIO_REGION_Invalid            = -1
}RadioRegionId;

//
#define  MAX_RADIO_STA_NAME		32
// #define  RADIO_FAV_FILENAME		_T("\\ResidentFlash\\RadioFav.dat")

// Radio band indices
typedef enum _RadioBandId
{
	RADIO_BAND_ID_FM,
	RADIO_BAND_ID_AM,
	RADIO_BAND_ID_WX,
	RADIO_BAND_ID_INVALID = 0xffffffff
}
RadioBandId;

typedef struct _RadioStationInfo
{
	DWORD dwSize;
	DWORD dwBand;
	DWORD dwRegion;
	WCHAR name[MAX_RADIO_STA_NAME];
	DWORD dwFreq;
	DWORD dwPICode;
}RadioStationInfo;

typedef enum _ProgramType
{
	PT_Undefined = 0,
	PT_News,
	PT_Current_Affairs,
	PT_Information,
	PT_Sport,
	PT_Education,//5
	PT_Drama,
	PT_Culture,
	PT_Science,
	PT_Varied,
	PT_Pop_Music,//10
	PT_Rock_Music,
	PT_Easy_Music,
	PT_Light_classical,
	PT_Serious_classical,
	PT_Other_Music,//15
	PT_Weather,
	PT_Finance,
	PT_Children,
	PT_Social_Affairs,
	PT_Religion,
	PT_Phone_In,
	PT_Travel,
	PT_Leisure,
	PT_Jazz_Music,
	PT_Country_Music,
	PT_National_Music,
	PT_Oldies_Music,
	PT_Folk_Music,
	PT_Documentary,
	PT_Alarm_Test,
	PT_Alarm,

	PT_Count,           // 32

	PT_AllType = 0xFF,

	PT_Invalid = 0xFFFFFFFF
}
ProgramType;

enum RADIO_LIBRARY_TYPE
{
	RADIO_LIBRARY_NULL,
	RADIO_LIBRARY_FM,
	RADIO_LIBRARY_AM,
	RADIO_LIBRARY_FAVORITE
};

struct SIGNAL_DATA
{
	WORD PICode;
	RADIO_SIGNAL_DATA signalData;
};

WCHAR* ProgramTypeToString( ProgramType pt );
#if 0
const wchar_t* RegionToString(RadioRegionId idRegion);
const wchar_t* RegionToStringEn(RadioRegionId idRegion);
RadioRegionId StringToRegion(const wchar_t* strRegionName);
#endif
