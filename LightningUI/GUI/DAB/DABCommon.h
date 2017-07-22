#pragma once

#include "../../stdafx.h"
#include "../Common/ui_common.h"
#include "../Radio/RadioCommon.h"
#include "../../Config/customer.h"

#if CVTE_EN_DAB
#define DEBUG_DAB			1

//media command:
#define  DAB_CMD_INITIALIZE                     0
#define  DAB_CMD_SWITCH                         1
#define  DAB_CMD_ANTENNA_POWER         2
#define  DAB_CMD_PLAY_CONTROL                   4
#define  DAB_CMD_REFRESH_LIBRARY                5
#define  DAB_CMD_PLAY_INDEX                     7

//player control code:
#define  DAB_CONTROL_SCAN_AUTO_STORE           0
#define  DAB_CONTROL_SCAN_PLAY_START           1
#define  DAB_CONTROL_SCAN_PLAY_STOP            2
#define  DAB_CONTROL_NEXT                       3
#define  DAB_CONTROL_PREVIOUS                   4
#define  DAB_CONTROL_SEEK_NEXT                  5
#define  DAB_CONTROL_SEEK_PREVIOUS              6
#define  DAB_CONTROL_CHANGE_BAND                7
#define  DAB_CONTROL_CHANGE_SEARCH_TYPE         8
#define  DAB_CONTROL_SCAN_SERVICE			    9

#define DISP_CHAR_MAX   16

//functions:
#define FUNC_CHECK_BOOL(X) if(!X) {return FALSE;}

//
typedef enum _DAB_LIBRARY_TYPE_
{
	DAB_LIBRARY_NULL = 0,
	DAB_LIBRARY_SERVICE,
	DAB_LIBRARY_PTY_LIST,
	DAB_LIBRARY_ENSEMBLE_LIST,
	DAB_LIBRARY_FAVORITE
}DAB_LIBRARY_TYPE;


//typedef struct _DABServiceInfo
//{
//    WCHAR ensemblelabel[LONG_LABEL_LENGTH + 1];
//    WCHAR servicelabel[LONG_LABEL_LENGTH + 1];
////    WCHAR stationname[MAX_FREQUENCY_NUM];
//    DWORD dwFreq;
//    DWORD dwSize;
//}DABServiceInfo;

void SendDABCommand(WPARAM wParam, LPARAM lParam);

#endif
