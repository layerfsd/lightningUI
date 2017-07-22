#pragma once

#include "../../stdafx.h"
#include "../Common/ui_common.h"

#define DEBUG_IMAGE			1


//media command:
#define  IMAGE_CMD_INITIALIZE								0
#define  IMAGE_CMD_SELECT_DEVICE						2
#define  IMAGE_CMD_LOAD_FILE								3
#define  IMAGE_CMD_PLAY_CONTROL						4
#define  IMAGE_CMD_PLAY_INDEX							7

//player control code:
#define  IMAGE_CONTROL_STOP									0
#define  IMAGE_CONTROL_PLAY									1
#define  IMAGE_CONTROL_PAUSE								2
#define  IMAGE_CONTROL_NEXT									3
#define  IMAGE_CONTROL_PREVIOUS							4
#define  IMAGE_CONTROL_FF										5
#define  IMAGE_CONTROL_RW										6
#define  IMAGE_CONTROL_PLAYMODE_REPEAT			7
#define  IMAGE_CONTROL_PLAYMODE_SHUFFLE			8
#define  IMAGE_CONTROL_PLAYMODE_TOGGLE			9
#define  IMAGE_CONTROL_PRELOAD_NEXT             10
#define  IMAGE_CONTROL_PRELOAD_PREVIOUS         11

//functions:

void SendImageCommand(WPARAM wParam, LPARAM lParam);

