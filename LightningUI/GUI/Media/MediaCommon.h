#pragma once

#include "../../stdafx.h"
#include "../Common/ui_common.h"

#define DEBUG_MEDIA			1

#define MEDIA_MAX_VOLMUE		0xFFFF
#define MEDIA_LOW_VOLMUE		0xDDDD  //0x8888

// #define PLAY_FAILED_NO_FILE					1
// #define PLAY_FAILED_INVALID_FILE			2

//video window:
#define VIDEO_WINDOW_PX		125
#define VIDEO_WINDOW_PY		10
#define VIDEO_WINDOW_CX		550 //700
#define VIDEO_WINDOW_CY		330 //400

//media ex massage:
#define MEDIA_MSG_SCAN_STATUS							0
#define MEDIA_MSG_LOAD_STATUS							1

//media command:
#define MEDIA_CMD_INITIALIZE								0
#define MEDIA_CMD_CATEGORY								1
#define MEDIA_CMD_SELECT_DEVICE						2
#define MEDIA_CMD_LOAD_FILE								3
#define MEDIA_CMD_PLAY_CONTROL						4
#define MEDIA_CMD_VIDEO_CONTROL					5
#define MEDIA_CMD_SET_POSITION						6
#define MEDIA_CMD_PLAY_INDEX							7
#define MEDIA_CMD_MUTE										8
#define MEDIA_CMD_SWITCH_AUDIO_SRC				9
#define MEDIA_CMD_PLAY_PAUSE_SWITCH				10
#define MEDIA_CMD_ENABLE_VIDEO						11
#define MEDIA_CMD_SET_RINGTONE_FILE				12

//player control code:
#define MEDIA_CONTROL_STOP						0
#define MEDIA_CONTROL_PLAY						1
#define MEDIA_CONTROL_PAUSE						2
#define MEDIA_CONTROL_NEXT						3
#define MEDIA_CONTROL_PREVIOUS					4
#define MEDIA_CONTROL_FF						5
#define MEDIA_CONTROL_RW						6
#define MEDIA_CONTROL_PLAYMODE_REPEAT_ALL		7
#define MEDIA_CONTROL_PLAYMODE_SHUFFLE			8
#define MEDIA_CONTROL_PLAYMODE_REPEAT_ONE		9
#define MEDIA_CONTROL_PLAYMODE_TOGGLE			10
#define MEDIA_CONTROL_TERMINATE					11


//video window control:
#define MEDIA_VIDEO_HIDE										0
#define MEDIA_VIDEO_SHOW									1
#define MEDIA_VIDEO_WINDOW								2
#define MEDIA_VIDEO_FULLSCREEN							3
#define MEDIA_VIDEO_TOGGLE								4

//
#define MEDIA_SCAN_BEGIN							0
#define MEDIA_SCAN_FINISH							1
#define MEDIA_SCAN_ABORT							2
#define MEDIA_SCAN_FAILED							3

//
#define MEDIA_LOAD_BEGIN							0
#define MEDIA_LOAD_FINISH							1
#define MEDIA_LOAD_ABORT							2
#define MEDIA_LOAD_FAILED							3

//
#define DEV_NAME_MEDIACARD		_T("MEDIA CARD")
#define DEV_NAME_GPSCARD			_T("GPS CARD")
#define DEV_NAME_USB1					_T("USB Disk")
#define DEV_NAME_USB2					_T("USB Disk2")
#define DEV_NAME_NAND					_T("ResidentFlash")
#define DEV_NAME_IPOD					_T("iPod")
#define DEV_NAME_BT						_T("Bluetooth Stream")
#define DEV_NAME_LINEIN_FRONT			_T("Front Line In")
#define DEV_NAME_LINEIN_REAR				_T("Rear Line In")
#define DEV_NAME_DVD							_T("DVD")

#define CVTE_DEF_MEDIA_TEST_VIDEO	_T("\\ResidentFlash\\Media\\Test.mp4")

#define MEDIA_FILE_TYPE_FOLDER			0
#define MEDIA_FILE_TYPE_AUDIO				1
#define MEDIA_FILE_TYPE_VIDEO				2
#define MEDIA_FILE_TYPE_ALL					3
#define MEDIA_FILE_TYPE_INVALID			4

//Media slider length
#define MEDIA_SILDER_LENGTH				512

//
enum MEDIA_CATEGORY
{
	MEDIA_NULL,
	MEDIA_AUDIO,
	MEDIA_VIDEO,
	MEDIA_IMAGE,
	MEDIA_ALL,
};

enum MEDIA_LIBRARY_TYPE
{
	MEDIA_LIBRARY_TRACKS,
	MEDIA_LIBRARY_EXPLORER,
	MEDIA_LIBRARY_SEARCH_RESULT
};

enum FILE_LOADING_STATUS
{
	LOAD_STATUS_NULL,
	LOAD_STATUS_FAILED_NO_FILE,
	LOAD_STATUS_FAILED_INVALID_FILE,
	LOAD_STATUS_LOADING,
	LOAD_STATUS_SUCCEED
};

typedef enum _MediaPlayMode
{
	MEDIA_PLAY_MODE_MIN = 0,
	MEDIA_PLAY_MODE_REPEAT_ALL = 0,
	MEDIA_PLAY_MODE_REPEAT_ONE,
	MEDIA_PLAY_MODE_SHUFFLE,
	MEDIA_PLAY_MODE_MAX,
};

typedef struct _tagMEDIAITEM
{
	UINT			id;
	CM_String		strItemName;
	CM_String		strFileName;

}MEDIAITEM,*LPMEDIAITEM;

//functions:


typedef struct _ID3_V1INFO_
{
	char Title[30];
	char Artist[30];
	char Album[30];
	char Year[4];
	char Comment[28];
	char Reserve;
	unsigned char Track;
	unsigned char Genre;
}ID3V1INFO;

typedef struct _ID3_V1DATA_
{
	char Header[3];    // tag
	ID3V1INFO V1Info;  //data
}ID3V1DATA;

BOOL GetID3v1Info(const wchar_t* filename, ID3V1INFO *info);