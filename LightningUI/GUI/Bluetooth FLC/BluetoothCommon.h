#pragma once

#include "../../stdafx.h"
#include "../Common/ui_common.h"

#define BT_STACK_FLC	1
#define BT_STACK_IVT	2

#define BT_STACK_TYPE	BT_STACK_FLC

#if (BT_STACK_TYPE == BT_STACK_IVT)
#include "../../Bluetooth IVT/inc/BtAdapter.h"
#pragma  comment(lib, "BtAdapter.lib")
#elif(BT_STACK_TYPE == BT_STACK_FLC)
#include "FLC_BLUETOOTH_SDK.h"
#include "HeadFile.h"
#pragma  comment(lib, "FLC_BLUETOOTH_SDK.lib")
#pragma  comment(lib, "FlcBT.lib")
#else
#error "BT_STACK_TYPE is not defined."
#endif

#define DEBUG_BT								1

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
#define BT_PHONEBOOK_AUTOSTORE		1
#else
#define BT_PHONEBOOK_AUTOSTORE		1
#endif

#define DEFAULT_NAME TEXT("CSR PrimaII BT Device")
#define DEFAULT_PIN TEXT("8888")

#define A2DP_MUTE_VOL					0
#define A2DP_LOW_VOL					60
#define A2DP_MAX_VOL					100

//BT Power State:
#define BT_STATUS_POWER				1
#define BT_STATUS_SETUP_INI			2
#define BT_STATUS_VERSION			3
#define BT_STATUS_NAME				4

#define BT_POWER_OFF											0
#define BT_POWER_ON											1

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
#define BT_CMD_SET_AUTO_ANSWER				10
#define BT_CMD_SET_AUTO_SYNCPB				11
#define BT_CMD_SET_AUTO_CONNECT			12
#define BT_CMD_SET_VCARD_NAME_ORDER	13
#define BT_CMD_DOWNLOAD_PHONEBOOK	14
#define BT_CMD_DOWNLOAD_CALLLOG			15
#define BT_CMD_SET_24H_TIME_MODE		16

//HFP control code:
#define HFP_CONTROL_ANSWER1							0
#define HFP_CONTROL_ANSWER2							1
#define HFP_CONTROL_HANGUP1							2
#define HFP_CONTROL_HANGUP2							3
#define HFP_CONTROL_ACH										4
#define HFP_CONTROL_HOLD1STCALL						5
#define HFP_CONTROL_END1STCALL						6
#define HFP_CONTROL_ENDINCOMINGCALL			7
#define HFP_CONTROL_SWITCHCALLS						8
#define HFP_CONTROL_MIC_MUTE							9

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
#define A2DP_CONTROL_ENABLE								15
#define A2DP_CONTROL_DISABLE							16

#define BT_PICK_DEVICE_FOR_HF					1
#define BT_PICK_DEVICE_FOR_A2DP				2

typedef enum
{
	CONNECTION_STATUS_IDLE,
	CONNECTION_STATUS_CONNECTING,
	CONNECTION_STATUS_CONNECTED,
	CONNECTION_STATUS_DISCONNECTING,
} ConnectionStatus;

typedef struct  _BT_MSG
{
	UINT eventID;
	UINT _DataID;
	WCHAR* _data;
	UINT _dataLen;
	BOOL dataCopy;
}BT_MSG;

/* Hfm states */
typedef UINT HfmStatesType;
#define HF_STATE_IDLE													(HfmStatesType)0x00
#define HF_STATE_INCOMING_CALL                             (HfmStatesType)0x01
#define HF_STATE_OUTGOING_CALL                            (HfmStatesType)0x02
#define HF_STATE_TALKING											(HfmStatesType)0x03
#define HF_STATE_TERMINATING									(HfmStatesType)0x04
#define HF_STATE_WAITING_CALL									(HfmStatesType)0x05
#define HF_STATE_NO_HELD_CALL                               (HfmStatesType)0x06
#define HF_STATE_HELD_CALL_NO_ACTIVE_CALL          (HfmStatesType)0x07

enum BT_HOMEPAGE_TYPE
{
	BT_HOMEPAGE_NULL,
	BT_HOMEPAGE_DEVICE,
	BT_HOMEPAGE_HISTORY,
	BT_HOMEPAGE_CONTACT,
	BT_HOMEPAGE_DIAL
};

enum CONTACT_STATUS
{
	CONTACT_NULL,
	CONTACT_DOWNLOADING,
	CONTACT_DOWNLOADED,
	CONTACT_DOWNLOADFAILED,
	CONTACT_DOWNLOADCANCLED
};

const wchar_t* DialStateToString(DIAL_STATE state);
typedef void (CALLBACK* callbackBluetoothPairReques)(LPCTSTR,LPCTSTR,LPVOID);
