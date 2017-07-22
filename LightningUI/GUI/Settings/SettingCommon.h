#pragma once

#include "../../stdafx.h"
#include "../../Audio/AudioCodec.h"

#define DEBUG_SETTING		1

#define UPDATE_FILE_MCU_GPSCARD		_T("\\GPS CARD\\MCU.bin")
#define UPDATE_FILE_OEMCFG_GPSCARD  _T("\\GPS CARD\\oem.cfg")
#define UPDATE_FILE_EBOOT_GPSCARD   _T("\\GPS CARD\\Eboot.nb0")
#define UPDATE_FILE_LOGO_GPSCARD    _T("\\GPS CARD\\Startup.raw")
#define UPDATE_FILE_IMAGE_GPSCARD	_T("\\GPS CARD\\manifest.bin")

#define UPDATE_FILE_UPDATE_TEXT      _T("\\USB Disk\\update.txt")
#define UPDATE_FILE_MCU_USBDISK		_T("\\USB Disk\\MCU.bin")
#define UPDATE_FILE_MCU_CKECKSUM_USBDISK				_T("\\USB Disk\\Checksum_MCU.txt")
#define UPDATE_FILE_OEMCFG_USBDISK  _T("\\USB Disk\\oem.cfg")
#define UPDATE_FILE_EBOOT_USBDISK   _T("\\USB Disk\\Eboot.nb0")
#define UPDATE_FILE_LOGO_BMP_USBDISK    _T("\\USB Disk\\logo.bmp")
#define UPDATE_FILE_LOGO2_BMP_USBDISK    _T("\\USB Disk\\logo2.bmp")
#define UPDATE_FILE_LOGO_JPG_USBDISK    _T("\\USB Disk\\logo.jpg")
#define UPDATE_FILE_LOGO2_JPG_USBDISK    _T("\\USB Disk\\logo2.jpg")
#define UPDATE_FILE_LOGO_RAW_USBDISK    _T("\\USB Disk\\startup.raw")
#define UPDATE_FILE_IMAGE_USBDISK	_T("\\USB Disk\\manifest.bin")
#define UPDATE_FILE_UI_USBDISK	_T("\\USB Disk\\install.bin")
#define UPDATE_FILE_PACKAGE_USBDISK  _T("\\USB Disk\\update.bin")
#define UPDATE_FILE_DAB_USBDISK	_T("\\USB Disk\\DAB_Module.fsapi")
#define UPDATE_FILE_RES_USBDISK	_T("\\USB Disk\\res.bin")

#define EQ_NONE				0
#define EQ_POP					1
#define EQ_CLASSICAL			2
#define EQ_ROCK					3
#define EQ_JAZZ					4
//#define EQ_BASS				5
#define EQ_USER					5

//
//MCU update msg:
#define UPDATE_MCU_FAILED               0
#define UPDATE_MCU_SUCCEED              1
#define UPDATE_MCU_UPDATING             2
#define UPDATE_MCU_REQUST_SUCCEED       3
#define UPDATE_MCU_REQUST_FAILED        4
#define SET_UPDATE_OBJECT				5

//
//Steer wheel
#define STEER_WHEEL_RNONE   0
#define STEER_WHEEL_R1      1
#define STEER_WHEEL_R2      2
#define STEER_WHEEL_R3      3
#define STEER_WHEEL_R4      4

//factory command:
#define FACTORY_CMD_INVALID             0
#define FACTORY_CMD_TEST_STOP           1
#define FACTORY_CMD_TEST_START          2

#define FACTORY_CMD_MEDIA_TEST_INVALID        0
#define FACTORY_CMD_MEDIA_TEST_VIDEO          1
#define FACTORY_CMD_MEDIA_TEST_AUDIO		  2
#define FACTORY_CMD_MEDIA_TEST_IMAGE 		  3
#define FACTORY_CMD_MEDIA_TEST_IPOD 		  4
#define FACTORY_CMD_MEDIA_TEST_A2DP 		  5
#define FACTORY_CMD_MEDIA_TEST_ALL 		      6

#define FACTORY_CMD_TOUCH_TEST                7

#define RK_CLOCK                TEXT("Software\\Microsoft\\Clock")
#define RV_INDST                TEXT("HomeDST")                        // are we currently in DST
#define RV_AUTODST              TEXT("AutoDST")
#define RV_DSTUI                TEXT("ShowDSTUI")
#define RV_TIMEZONES            TEXT("Time Zones")
#define RV_DISPLAY              TEXT("Display")
#define RV_TZI                  TEXT("TZI")
#define RV_DLT                  TEXT("Dlt")
#define RV_STD                  TEXT("Std")

#define ZoneHasDaylightTime(tzi)    (tzi.DaylightDate.wMonth && tzi.DaylightBias)

#define ARRAYSIZEOF(x)	(sizeof(x) / sizeof((x)[0]))

#define FILETIME_TO_MINUTES ((__int64)600000000L)

enum AUDIO_SET_ITEM
{
	AUDIO_SET_VOLUME,
	AUDIO_SET_BASS,
	AUDIO_SET_SUBWOOFER,
	AUDIO_SET_TREBLE,
	AUDIO_SET_MID,
	AUDIO_SET_LOUDNESS,
	AUDIO_SET_MUTE,
	AUDIO_SET_BALANCE,
	AUDIO_SET_FAD,
	AUDIO_SET_EQ,
	AUDIO_SET_EQ_BAND1,
	AUDIO_SET_EQ_BAND2,
	AUDIO_SET_EQ_BAND3,
	AUDIO_SET_EQ_BAND4,
	AUDIO_SET_EQ_BAND5,
	AUDIO_SET_EQ_BAND6,
	AUDIO_SET_EQ_BAND7,
	AUDIO_SET_EQ_BAND8,
	AUDIO_SET_EQ_BAND9,
	AUDIO_SET_EQ_BAND10,
	AUDIO_SET_SRC_VOL,
	AUDIO_SET_MIC_IN_VOL,
	AUDIO_SET_3D_EFFECT,
	AUDIO_SET_MAIN_VOLUME,
	AUDIO_SET_ALT_VOLUME,
	AUDIO_SET_SUBWOOFER_FREQ,
	AUDIO_SET_INTERNAL_AMP,
	AUDIO_SET_ASP_WORKMODE
};

//	enum STEER_WHEEL_ITEM
//	{
//	//    STEER_WHEEL_RESISTANCE,
//	    STEER_WHEEL_VOL_PLUS,
//	    STEER_WHEEL_VOL_MINUS,
//	    STEER_WHEEL_VOL_MUTE,
//	    STEER_WHEEL_PREV,
//	    STEER_WHEEL_NEXT,
//	    STEER_WHEEL_SEEK,
//	    STEER_WHEEL_PHONE,
//	    STEER_WHEEL_MODE,
//	    STEER_WHEEL_POWER
//	};


enum NAVI_SPEECH_CONFIG
{
	NAVI_SPEECH_OFF,
	NAVI_SPEECH_ALWAYS,
	NAVI_SPEECH_WHILEONTOP
};

enum ANDROID_LINK_DEVICE_TYPE
{
	DEVICE_TYPE_ANDROID_AUTO	=	1,
	DEVICE_TYPE_WEBLINK		=	2,
};
