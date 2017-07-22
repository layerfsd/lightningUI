//

#pragma once

#include "../Utils/utils.h"
#include "API.h"
#include "../DataPool/DataPool.h"	//depends on api.h(BTDEV*)
// #include "MMI.h"		//depends on datapool.h(STATE_STRING_MAXLEN)
// #include "Logic.h"		//depends on api.h(FSM_User:BTDEV*)

#define HIDDEN_WND_CLASSNAME TEXT("CBTMODULE:SNM_ACCEPT_WND")

#define BT_APP_REG_KEY      TEXT("software\\microsoft\\bluetooth\\device\\Settings")
#define MMI_REG_KEY         BT_APP_REG_KEY
#define BT_SETTING_REG_KEY      BT_APP_REG_KEY

#define MAX_BTMODE_STRING 20
#define BTMODE_DISCOVERABLE_STRING TEXT("Discoverable")
#define BTMODE_UNDISCOVERABLE_STRING TEXT("Undiscoverable")

//BT_STATE
#define BT_STATE_OFF												0
#define BT_STATE_UNCONNECTED							1
#define BT_STATE_SEARCHING									2
#define BT_STATE_PARING										3
#define BT_STATE_CONNECTING								4
#define BT_STATE_CONNECTED								5
#define BT_STATE_DISCONNECTING							6

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


//user-defined messages
enum
{
    UM_SNTHREAD_EXIT = WM_USER + 1,
    UM_PIN_REQUEST,
    UM_SEARCH_SUCCESS,
    UM_SEARCH_FAIL,
    UM_PAIR_SUCCESS,
    UM_PAIR_FAIL,
    UM_CHANGE_SKIN,
    UM_CHANGE_LANGUAGE,
    UM_SYS_PN_SLEEP,
    UM_SYS_PN_WAKEUP,
	UM_SYS_HARDWARE_ERR,
    UM_SHOW_BTAPP,
    UM_DUN_CONNECT_SUCCESS,
    UM_DUN_CONNECT_FAIL,
    UM_ANSWERPAIR_SUCCESS,
    UM_ANSWERPAIR_FAIL,
    UM_ANSWERPAIR_TIMEOUT,
	UM_SINGAL_MSG,
	WM_SHOWTASK,
	UM_CONNECT_ALL_FINISH,
	UM_OPP_REQUEST,
	UM_AUTO_CONNECT_STATUS,
	UM_A2DP_STREAM_STATUS,
};

enum	//logic's errors
{
	//ERROR_SUCCESS
	ERROR_GENERAL = 1,

	//FSM error	,
	ERROR_NOT_HANDLED,
	ERROR_INCORRECT_TRIGGER,
	ERROR_API_CALL_FAIL,

	//apiSearchPair error
	ERROR_INVALID_PARAM,
	ERROR_NEW,
	ERROR_SDP_OR_NOSERVICE,
	ERROR_ALREADY_CONNECTED,

};

enum
{
	MB_BN_CLICKED_CANCEL = 0,
	MB_BN_CLICKED_OK,
	MB_BN_CLICKED_IGNORE,
};

typedef enum 
{
	SNM_CONNECTION_STATE_IDLE,
	SNM_CONNECTION_STATE_CONNECTING,
	SNM_CONNECTION_STATE_CONNECTED,
	SNM_CONNECTION_STATE_DISCONNECTING,
} SnmConnectionState;

//via registry
void GetBTMode(LPTSTR lpsz);
void SetBTMode(LPTSTR lpsz);

#define DEFAULT_RING_TONE TEXT("DefaultRingTone.wav")
//via registry
void GetRingTone(LPTSTR lpsz);
void SetRingTone(LPTSTR lpsz);

#define MIN_RING_VOLUME 0
#define MAX_RING_VOLUME 0xFFFF
#define DEFAULT_RING_VOLUME 0x8000
//via registry
DWORD GetRingVolume();
void SetRingVolume(DWORD dwRingVolume);

// /*static*/ HINSTANCE v_hBtpUIExtDll = NULL;
typedef DWORD (WINAPI *p_BluetoothNotificationResolveString)(BluetoothNotifyIdentifierT identifier, TCHAR **str);
typedef DWORD (WINAPI *p_BluetoothNotificationResolveValue)(BluetoothNotifyIdentifierT identifier, DWORD *value);
// /*static*/ p_BluetoothNotificationResolveString		g_BluetoothNotificationResolveString = NULL;
// /*static*/ p_BluetoothNotificationResolveValue		g_BluetoothNotificationResolveValue = NULL;


//COPYDATASTRUCT dwData when WM_COPYDATA
#define IPC_CALL_DIALER_DATA 1

// CBTApp:
// See BT.cpp for the implementation of this class
//

class CBTModule
{
public:
    CBTModule();
	~CBTModule();

// Overrides
public:
    BOOL Initialize(HWND hWnd);
    void DeInitialize();
	BOOL IsInitialized();

// Implementation
//     CMMI *pMmi;
//     CLogic *pLogic;
    CDataPool *pDataPool;
    CAPI *pApi;

	BOOL	m_bIni;
    HWND hWndHidden;
};

extern CBTModule g_BTModule;
extern  UINT g_uiSignalNotifyMessage;
extern p_BluetoothNotificationResolveString		g_BluetoothNotificationResolveString;
extern p_BluetoothNotificationResolveValue		g_BluetoothNotificationResolveValue;
