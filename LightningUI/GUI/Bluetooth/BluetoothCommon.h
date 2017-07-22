#pragma once

#include "../../stdafx.h"

#include <bt_sfwe_api.h>
#include "../Common/ui_common.h"

#define DEBUG_BT								1

#define DEFAULT_NAME TEXT("CSR PrimaII BT Device")
#define DEFAULT_PIN TEXT("8888")

#define A2DP_MUTE_VOL					0
#define A2DP_LOW_VOL					60
#define A2DP_MAX_VOL					100

//BT Power State:
#define BT_POWER_OFF											0
#define BT_POWER_ON											1
#define BT_POWER_OPERATING_ON							2
#define BT_POWER_OPERATING_OFF						3

//bluetooth command:
#define BT_CMD_POWER									0
#define BT_CMD_CONNECT_DEVICE					1
#define BT_CMD_DISCONNECT_DEVICE			2
#define BT_CMD_A2DP_CONTROL					3
#define BT_CMD_HFP_CONTROL						4
#define BT_CMD_SET_DEVICE_NAME				5
#define BT_CMD_SET_PIN									6
#define BT_CMD_MUTE									7
#define BT_CMD_A2DP_VOLUME						8
#define BT_CMD_GET_A2DP_STREAM				9

//HFP control code:
#define HFP_CONTROL_ANSWER1							0
#define HFP_CONTROL_ANSWER2							1
#define HFP_CONTROL_HANGUP1							2
#define HFP_CONTROL_HANGUP2							3
#define HFP_CONTROL_ACH										4

//player control code:
#define A2DP_CONTROL_STOP									0
#define A2DP_CONTROL_PLAY									1
#define A2DP_CONTROL_PAUSE								2
#define A2DP_CONTROL_NEXT									3
#define A2DP_CONTROL_PREVIOUS							4
#define A2DP_CONTROL_FF										5
#define A2DP_CONTROL_RW										6
#define A2DP_CONTROL_PLAYMODE_REPEATALL			7
#define A2DP_CONTROL_PLAYMODE_REPEATONE		8
#define A2DP_CONTROL_PLAYMODE_SHUFFLE			9
#define A2DP_CONTROL_PLAYMODE_TOGGLE			10

#define BT_PICK_DEVICE_FOR_HF					1
#define BT_PICK_DEVICE_FOR_A2DP				2

enum BT_HOMEPAGE_TYPE
{
	BT_HOMEPAGE_NULL,
	BT_HOMEPAGE_DEVICE,
	BT_HOMEPAGE_HISTORY,
	BT_HOMEPAGE_CONTACT
};

enum CONTACT_STATUS
{
	CONTACT_NULL,
	CONTACT_DOWNLOADING,
	CONTACT_DOWNLOADED,
	CONTACT_DOWNLOADFAILED,
	CONTACT_DOWNLOADCANCLED
};
