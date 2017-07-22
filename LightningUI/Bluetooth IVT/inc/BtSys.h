//
// Copyright (c) 2010 FLC Corporation.  All rights reserved.
//
/***********************************************************
Module Name:  
FLC_BLUETOOTH_SDK.h

Abstract:
API for Bluetooth Development.
************************************************************/


#ifndef _FLC_SYSTEMCTL_API_H
#define _FLC_SYSTEMCTL_API_H
#include <windows.h>
#ifdef __cplusplus
extern "C" {
#endif

#define FLC_BLUETOOTH_API __declspec(dllexport)

#define MAX_ADDRESS_LEN		18
#define MAX_NUM_LEN			30
#define	MAX_STRING_LEN		256
#define MAX_SERVICE_NUM		20
#define MAX_DATETIME_LEN	25/*added by hz*/
#define MAX_PINCODE_LEN		16


typedef	int FlcBtStatus;
	#define		FLC_BT_STATUS_SUCCESS		0
	#define 	FLC_BT_STATUS_FAILED		1
	#define 	FLC_BT_STATUS_PENDING		2
	#define 	FLC_BT_STATUS_DISCONNECT	3
	#define 	FLC_BT_STATUS_NO_LINK		4
	#define 	FLC_BT_STATUS_IN_USE		5
	#define 	FLC_BT_STATUS_BUSY			11
	#define 	FLC_BT_STATUS_NO_RESOURCES	12
	#define 	FLC_BT_STATUS_NOT_FOUND		13
	#define 	FLC_BT_STATUS_DEVICE_NOT_FOUND	14
	#define 	FLC_BT_STATUS_CONNECTION_FAILED	15
	#define 	FLC_BT_STATUS_TIMEOUT			16
	#define 	FLC_BT_STATUS_NO_CONNECTION		17
	#define 	FLC_BT_STATUS_INVALID_PARM		18
	#define 	FLC_BT_STATUS_IN_PROGRESS		19
	#define 	FLC_BT_STATUS_RESTRICTED		20
	#define 	FLC_BT_STATUS_INVALID_TYPE		21
	#define 	FLC_BT_STATUS_HCI_INIT_ERR		22
	#define 	FLC_BT_STATUS_NOT_SUPPORTED		23
	#define 	FLC_BT_STATUS_SDP_CONT_STATE	24
	#define 	FLC_BT_STATUS_CANCELLED			25
	#define 	FLC_BT_STATUS_ABORT				128
	#define 	FLC_BT_STATUS_ALREADY_EXISTS	129
	#define 	FLC_BT_STATUS_SDP_QUERY_FAILED	130


typedef enum {
	BT_EVENT_START,//0

	BT_EVENT_POWERON_FINISH,
	BT_EVENT_POWEROFF_FINISH,
	BT_EVENT_INIT_FINISH,
	BT_EVENT_DEINIT_FINISH,
	BT_EVENT_RESET_FINISH,//5

	BT_EVENT_STANDBY_ON,
	BT_EVENT_STANDBY_OFF,
	//connection event   
	BT_CONN_PIN_REQ,
	BT_CONN_PAIR_IND,
	BT_CONN_PAIR_COMPLETE,//10
	
    BT_CONN_INQUIRE_RESULT_ADDRESS,
	BT_CONN_INQUIRE_RESULT,
	BT_CONN_INQUIRE_COMPLETE,
	BT_CONN_PAIRED_LIST_RESULT,
	BT_CONN_PAIRED_LIST_COMPLETE,//15
	BT_CONN_PAIRED_DELETE_DEVICE,
	
	BT_CONN_CURRENT_DEVICE_INFO,
	BT_CONN_SERVICE_RESULT,
	BT_CONN_EVENT_END,
	//Hands free event
	BT_HF_CALL_ID,//20
	BT_HF_CALL_SETUP_STATUS,
	
	BT_HF_CONNECT_STATUS,
	BT_HF_AUDIO_STATUS,
	BT_HF_SUBSCRIBER_NUMBER,
	BT_HF_NETWORK_OPERATOR,//25
	BT_HF_BATTERY_IND,
	
	BT_HF_SIGNAL_IND,
	BT_HF_EVENT_END,
	BT_HFG_CONNECT_STATUS,
	BT_HFG_AUDIO_STATUS,//30
	BT_HFG_ANSWER_CALL,
	
	BT_HFG_DIAL_NUMBER,
	BT_HFG_MEMORY_DIAL,
	BT_HFG_REDIAL,
	BT_HFG_CALL_HOLD,//35
	BT_HFG_HANGUP,
	
	BT_HFG_SEND_DTMF,
	BT_HFG_REPORT_HFMIC_VOLUME,
	BT_HFG_REPORT_HFSPK_VOLUME,
	BT_HFG_EVENT_END,//40
    BT_HF_PB_SUPPORT,
	
	BT_HF_PB_SUPPORTED_LIST,
	BT_HF_PB_RANGE,
	BT_HF_PB_ENTRY,
	BT_HF_PB_ENTRY_FINISH,//45
	BT_HF_PB_EVENT_END,
	
	//Av event
	BT_AV_STATUS,

    BT_AV_PLAY_STATUS,
    BT_AV_TRACK_CHANGED,
    BT_AV_MEDIA_INFO,//50
    BT_AV_AVRCP_VERSION,
	
	BT_AV_EVENT_END,

	//OPP EVENT
	BT_OPP_CONNECTED,
	BT_OPP_DISCONNECTED,
	BT_OPP_RECEIVE_REQUEST,//55
	BT_OPP_RECEIVE_START,
	
	BT_OPP_RECEIVE_PROCESS,
	BT_OPP_RECEIVE_COMPLETE,
	BT_OPP_EVENT_END,
	BT_PBAP_CONNECT_STATUS,//60
    BT_PBAP_PB_EVENT_ENTRY,
	
	BT_PBAP_PB_ENTRY_FINISH,
	BT_PBAP_OP_ABORT,
	BT_PBAP_SUPPORT_INFO,
    BT_PBAP_EVENT_END,//65
	BT_MAP_CONNECT_STATUS,
	
	BT_MAP_DATA_IND,
	BT_MAP_GET_MESSAGE_LIST_FINISH,
	BT_MAP_GET_MESSAGE_FINISH,
	BT_MAP_EVENT_END,//70
    //PAN EVENT  
	BT_PAN_USER_CONNECTED,
	
	BT_PAN_USER_DISCONNECTED,
	BT_PAN_UESR_REQUEST,
    BT_PAN_GROUP_CONNECTED,
	BT_PAN_GROUP_DISCONNECTED,//75
	BT_PAN_GROUP_REQUEST,
	
	BT_PAN_UNSUPPORT,
	//DUN EVENT
	BT_DUN_CONNECTED,
	BT_DUN_DISCONNECTED,
	BT_DUN_ERROR,//80
	BT_HID_CONNECTED,
	
	BT_HID_DISCONNECTED,
    BT_SPP_CONNECTED,
    BT_SPP_DISCONNECTED,
    BT_SPP_CALACK,//85
	BT_SPP_GET_RECT,
	BT_EVENT_DRIVELOAD_ABORT,
	
	BT_EVENT_END//87
}BTEvent;

typedef enum 
{
	BT_SERVICE_SERIAL_PORT,
	BT_SERVICE_DIALUP_NETWORKING,
	BT_SERVICE_HANDSFREE,
	BT_SERVICE_HANDSFREE_AUDIO_GATEWAY,
	BT_SERVICE_PBAP_SERVER,
	BT_SERVICE_PBAP_PROFILE,
	BT_SERVICE_MAP_SERVER,
	BT_SERVICE_MAP_NOTIFY_SERVER,
	BT_SERVICE_MAP_PROFILE,
	BT_SERVICE_AUDIO_DISTRIBUTION,
	BT_SERVICE_AV_REMOTE_CONTROL,
	BT_SERVICE_OBEX_OBJECT_PUSH,
	BT_SERVICE_AUDIO_SOURCE,
	BT_SERVICE_AUDIO_SINK,
    BT_SERVICE_PAN_SERVER,
	BT_SERVICE_IAP

}DeviceService;

enum MICROPHONE_MODE
{
	MICROPHONE_MIC1,	//Microphone 1.ƒ⁄÷√MIC
	MICROPHONE_MIC2,	//Microphone 2.Õ‚Ω”MIC
};

typedef struct _DeviceServiceInfo {
	char			szAddr[MAX_ADDRESS_LEN];
	unsigned long	ServiceNum;
	unsigned short	ServiceEntry[MAX_SERVICE_NUM];
}DeviceServiceInfo;
/******************  Data structures for Connection: Start   ***********************/ 
typedef enum {
  BT_COMMON_PAIR,	                  
  BT_SIMPLE_PAIR   
}PairTypeList;

typedef struct _DeviceInfoEntry {
	char			szAddr[MAX_ADDRESS_LEN];
	char			szName[MAX_STRING_LEN];
	unsigned long	classOfDevice;
}DeviceInfoEntry;

typedef struct _PairCompleteInfo {
	char			szAddr[MAX_ADDRESS_LEN];
	bool			bPairResult;
}PairCompleteInfo;

typedef struct _PairIndication {
    char			szAddr[MAX_ADDRESS_LEN];
    char			szName[MAX_STRING_LEN];
    char            szNumeric[MAX_PINCODE_LEN];
    PairTypeList    pairType;
}PairIndication;
/******************  Data structures for Connection: End   ***********************/ 


/******************  Data structures for Hands free: Start  ***********************/ 
typedef enum {
	TO_PHONE,	                  
	TO_HF
}TransferDir;

typedef enum {
	HF_STATUS_CALL_IDLE,            
	HF_STATUS_CALL_INCOMMING,	   
	HF_STATUS_CALL_DIALING,
	HF_STATUS_CALL_ALERTING,
	HF_STATUS_CALL_ACTIVE,
	HF_STATUS_CALL_HELD,
	HF_STATUS_CALL_WAITING
}CallSetupStatus;

typedef enum {
	HF_STATUS_DISCONNECTED,	                  
	HF_STATUS_CONNECTING,	   
	HF_STATUS_CONNECTED
}HfConnectStatus;

typedef struct _HfConnectInfo
{
  HfConnectStatus status;
  char            szAddr[MAX_ADDRESS_LEN];
}HfConnectInfo;

typedef enum {
	HF_AUDIO_NONE,	                  
	HF_AUDIO_CONNECTED	   
}HfAudioStatus;


typedef	struct _HfCallId {
	int			intline;					/* The call index (used by Hold commands). */ 
	bool		bMultiParty;				/* Identifies a multiparty call */ 
	char		szNumber[MAX_NUM_LEN];	/* The calling number on incoming calls, the called number on outgoing */ 
}HfCallId;

typedef	struct _HfCallSetupStatus{
	CallSetupStatus		callSetup;
	int					intline;              //the index of call. 
	bool				bMultiParty;        //if the call is multiParty              	   
}HfCallSetupStatus;

/******************  Data structures for Hands free: End   ***********************/ 




/******************  Data structures for AV: Start   *****************************/ 

typedef enum {
    AV_MEDIA_STOPPED   =    0x00,
    AV_MEDIA_PLAYING   =    0x01,
    AV_MEDIA_PAUSED    =    0x02,
    AV_MEDIA_FWD_SEEK  =    0x03,
    AV_MEDIA_REV_SEEK  =    0x04,
    AV_MEDIA_ERROR     =    0xFF
}MediaStatus;


typedef struct _AvPlayStatus {
    unsigned long   length;      //total lenth
    unsigned long   position;    //current position
    MediaStatus    status;       //play status
} AvPlayStatus;

typedef unsigned long MediaAttrId;
#define MEDIA_ATTR_TITLE       0x00000001
#define MEDIA_ATTR_ARTIST      0x00000002
#define MEDIA_ATTR_ALBUM       0x00000003
#define MEDIA_ATTR_TRACK       0x00000004
#define MEDIA_ATTR_NUM_TRACKS  0x00000005
#define MEDIA_ATTR_GENRE       0x00000006
#define MEDIA_ATTR_DURATION    0x00000007

#define NUM_MEDIA_ATTRIBUTES        7

typedef struct _MediaAttrElement {
    unsigned long    attrId;     //attr ID, refer to MediaAttrId
    unsigned short  charSet;     //charset, reserved, no use here.  default UTF-8
    unsigned short   length;     //strlen
    char string[MAX_STRING_LEN]; //data
} MediaAttrElement;

/*one struct will return all the media attributes*/
typedef struct _MediaAttr{

    /* The number of elements returned */ 
    int numIds;
    /* An array of element value information */ 
    MediaAttrElement element[NUM_MEDIA_ATTRIBUTES];

} MediaAttr;

typedef enum {
	BT_AV_DISCONNECTED,
	BT_AV_CONNECTED,                  	   
	BT_AV_STREAMING
}AvStatus;

typedef enum {
	BT_AUDIO_A2DP,
	BT_AUDIO_AVRCP
}AvSource;

typedef struct _AvConnInfo
{
  AvStatus status;
  AvSource source;
  char     szAddr[MAX_ADDRESS_LEN];
}AvConnInfo;

typedef unsigned short AvrcpVersion;
#define AVRCP_VERSION_UNKNOWN 0x0000  /* Unable to determine the Hands Free Profile version that is supported */
#define AVRCP_VERSION_1_0     0x0100  /* Supports Version 1.0 of the AVRCP Profile */
#define AVRCP_VERSION_1_3     0x0103  /* Supports Version 1.3 of the AVRCP Profile */  
#define AVRCP_VERSION_1_4     0x0104  /* Supports Version 1.4 of the AVRCP Profile */


/******************  Data structures for AV: End   *******************************/ 

/******************  Data structures for HFG: Start   ****************************/ 

typedef enum {
	HFG_STATUS_DISCONNECTED,	                  
	HFG_STATUS_CONNECTING,	   
	HFG_STATUS_CONNECTED
}HfgConnectStatus;



typedef enum {
	HFG_AUDIO_NONE,	                  
	HFG_AUDIO_CONNECTED	   
}HfgAudioStatus;


typedef enum 
{
	BT_HFG_CALL_ACTIVE,
	BT_HFG_CALL_HELD,
	BT_HFG_CALL_DIALING,
	BT_HFG_CALL_ALERTING,
	BT_HFG_CALL_INCOMING,
	BT_HFG_CALL_WAITING,
	BT_HFG_CALL_NONE
}HfgCallState;

/******************  Data structures for HFG: End   ***********************/ 




/******************  Data structures for PB: Start   ***********************/ 
#define DIALED_CALLS_MASK	0x0001
#define FIXED_DIAL_MASK		0x0002
#define LAST_DIAL_MASK		0x0004
#define MISSED_CALLS_MASK	0x0008
#define PB_PHONE_MASK		0x0010
#define PB_COMBINED_MASK	0x0020
#define RECEIVED_CALLS_MASK	0x0040
#define PB_SIM_MASK			0x0080


/******************  Data structures for PBAP: Start *************************/
typedef enum {
	BT_PBAP_DISCONNECTED,
	BT_PBAP_CONNECTING,
	BT_PBAP_CONNECTED,
}PbapStatus;

typedef enum{
	BT_PBAP_FAIL,
	BT_PBAP_START,
	BT_PBAP_NONSUPPORT,
}Reason;

typedef enum {
	BT_PBAP_PHONE,
	BT_PBAP_SIM,
}PbapType;

typedef enum {
	PBAP_DIALED_CALLS,
	PBAP_MISSED_CALLS,
	PBAP_PB_PHONE,
	PBAP_PB_COMBINED,
	PBAP_RECEIVED_CALLS, 
	PBAP_PB_SIM,
	PBAP_PB_NONE,
}PbapStorage;

typedef enum _FLC_BTS_PB_AUTO_DOWNLOAD_STATE_E
{
  FLC_BTS_PB_AUTO_DL_START,
  FLC_BTS_PB_AUTO_DL_DOWNLOADIND,
  FLC_BTS_PB_AUTO_DL_END
}FlcBtsPBAutoDownload_e;
/******************  Data structures for PBAP: End   *************************/


typedef enum {
	DIALED_CALLS,
	FIXED_DIAL,
	LAST_DIAL,
	MISSED_CALLS,
	PB_PHONE,
	PB_COMBINED,
	RECEIVED_CALLS, 
	PB_SIM,
}HfPhoneBookStorage;

typedef struct _PBAPConnectStatus{
	PbapStatus status;
	Reason reason;
}PbapConnectStatus;


typedef	struct _HfPBInfo {
	unsigned short		ucStart;		/* First Entry           */
	unsigned short		ucEnd;			/* Last Entry            */
	unsigned short		ucNumberLen;	/* Maximum number length */
	unsigned short		ucTextLen;		/* Maximum text length   */
}HfPBInfo;

typedef	struct _HfPBEntry {
	unsigned short  ucIndex;					/* Location in the phone book     */
	char			szNumber[MAX_NUM_LEN];   /* The phone number               */
	char			szText[MAX_STRING_LEN];     /* The text part                  */
}HfPBEntry;

typedef enum{
	PBAP_CALL_MISSED,
	PBAP_CALL_RECEIVED,
	PBAP_CALL_DIALED,
	PBAP_CALL_UNKNOWN,
}PbapCallType;

typedef	struct _PbapPBEntry {
    char			szCellNumber[MAX_NUM_LEN];   /* The phone number               */
    char			szWorkNumber[MAX_NUM_LEN];   /* The phone number               */
    char			szHomeNumber[MAX_NUM_LEN];   /* The phone number               */
    char			szVoiceNumber[MAX_NUM_LEN];   /* The phone number               */
	char			szPrefNumber[MAX_NUM_LEN];   /* The phone number               */
	char			szOtherNumber[MAX_NUM_LEN];   /* The phone number               */
    char			szText[MAX_STRING_LEN];     /* The text part                  */
	char			szCallDatetime[MAX_DATETIME_LEN];/*tel call datetime Format:20130519T191219   added by hz*/
	PbapCallType   ucCallType;//0==missed 1=received 2=dialed 3=unknown
}PbapPBEntry;

typedef	struct _PhonebookEntry {
    char			szCellNumber[MAX_NUM_LEN];   /* The phone number               */
    char			szText[128];     /* The text part                  */
	char			szCallDatetime[MAX_DATETIME_LEN];/*tel call datetime 20130519T191219  added by hz*/
	PbapCallType	ucCallType;//0==missed 1=received 2=dialed 3=unknown
}PhoneBookEntry;


typedef	struct _Phonebook {
	PhoneBookEntry			*phonebookentry;
	unsigned int			phonebook_lenth;
}PhoneBook;



/******************  Data structures for PB: End   *************************/

/******************  Data structures for OPPS: Start *************************/
typedef	struct _OppTranProgress {
		unsigned long ulTotalSize;
		unsigned long ulTransferred;
}OppTranProgress;	
/******************  Data structures for OPPS: End   *************************/

/******************  Data structures for MAP: Start *************************/
typedef enum {
	BT_MAP_DISCONNECTED,
	BT_MAP_CONNECTING,
	BT_MAP_CONNECTED,
}MapConnectStatus;

typedef enum {
	BT_MAP_NONE,
	BT_MAP_UNREAD,
	BT_MAP_READ,
	BT_MAP_MASK
}MapGetMessageStatus;


typedef enum {
	BT_MAP_NATIVE,
	BT_MAP_UTF8
}MapCharset;

typedef enum {
	BT_MAP_FRACTION_FIRST,
	BT_MAP_FRACTION_NEXT,
	BT_MAP_FRACTION_NONE
}MapFractionFlags;

typedef enum {
	BT_MAP_OP_NONE,
	BT_MAP_OP_SET_PATH,
	BT_MAP_OP_GET_PATH_LISTING,
	BT_MAP_OP_GET_MESSAGE,
	BT_MAP_OP_GET_MESSAGE_LISTING,
	BT_MAP_OP_SET_EVENT
}MapOperator;

typedef enum {
	BT_MAP_PATH_NONE,
	BT_MAP_PATH_INBOX,
	BT_MAP_PATH_OUTBOX,
	BT_MAP_PATH_SENT,
	BT_MAP_PATH_DELETED,
	BT_MAP_PATH_DRAFT
}MapMessageListPath;

typedef struct _MapDataInd
{
	MapOperator		mapOp;
    unsigned char	*buffer;
    unsigned short	len;
}MapDataInd;

typedef struct _MapMsgListingParms
{
    const char				*fullPathName;
    MapGetMessageStatus		messageStatus;
}MapMsgListingParms;

typedef struct _MapMsgPushParms
{
    const char      *folderName;
    bool            transparent;
    bool            retry;
    MapCharset		charset;
    unsigned long   msgLen;
	char*			msgBuffer;
}MapMsgPushParms;

/******************  Data structures for MAP: End   *************************/

 /******************  Data structures for PAN:Start	 *************************/
 /*---------------------------------------------------------------------------
 * 
 * XCBPAN_ServiceClassUUID type
 *
 *   The following service class UUIDs are used when requesting connections 
 *   to PAN services.
 */
typedef unsigned short XCBPAN_ServiceClassUUID;

/** Invalid service class UUID.  Use when calling XCBPAN_UserConnect() if the
 *  target profile is unknown at the time of the call.  A request will be sent
 *  to the Chooser API to acquire a valid UUID.
 */
#define BT_XCBPAN_UUID_INVALID     0x0000

/** Personal Area Networking User (PANU) service class UUID 
*/
#define BT_XCBPAN_UUID_PANU        0x1115

/** Personal Area Networking Access Point (NAP) service class UUID
*/
#define BT_XCBPAN_UUID_NAP         0x1116

/* Personal Area Networking Group Network (GN) service class UUID
*/
#define BT_XCBPAN_UUID_GN          0x1117

/* End of XCBPAN_ServiceClassUUID */
/******************  Data structures for PAN:End	*************************/


#ifdef __cplusplus
};
#endif

#endif//_FLC_SYSTEMCTL_API_
