#pragma once

#include "../../stdafx.h"
#include "../Common/ui_common.h"

#include "../iAP/IpodControl.h"
#include "../iAP/IapAuthenticationListener.h"
#include "../iAP/IapBrowserListener.h"
#include "../iAP/IapPlayerListener.h"
#include "../iAP/IapIOSCommunicationListener.h"
#include "../iAP/IapRemoteControl.h"

#define DEBUG_IPOD	0


//ipod command:
#define IPOD_CMD_SELECT_INDEX						1
#define IPOD_CMD_PLAY_CONTROL						2
#define IPOD_CMD_SET_POSITION						3
#define IPOD_CMD_BROWSEUP							4
#define IPOD_CMD_SET_PLAYMODE						5

//player control code:
#define IPOD_CONTROL_STOP									0
#define IPOD_CONTROL_PLAY									1
#define IPOD_CONTROL_PAUSE								2
#define IPOD_CONTROL_NEXT									3
#define IPOD_CONTROL_PREVIOUS							4
#define IPOD_CONTROL_FF										5
#define IPOD_CONTROL_RW									6

//play mode:
#define IPOD_PLAYMODE_OFF									0
#define IPOD_PLAYMODE_REPEAT_ALL						1
#define IPOD_PLAYMODE_REPEAT_ONE					2
#define IPOD_PLAYMODE_SHUFFLE							3



