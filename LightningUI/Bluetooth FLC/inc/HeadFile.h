#pragma once
#include <Windows.h>
#include <vector>

typedef void (CALLBACK* OnBluetoothEvent)(UINT eventID, UINT _DataID, WCHAR* _data, UINT _dataLen);
// =======================================================================
// Callback event function definitions
// =======================================================================
enum BLUETOOTHEVENTID
{
	FLC_BT_EVENT_WARNING						= 0x00,	//unknown event
	FLC_BT_EVENT_INIT_INDICATOR				= 0x01,	//module can work indicator
	FLC_BT_EVENT_DEINIT_INDICATOR			= 0x02,	//module power down indicator
	FLC_BT_EVENT_SIGNAL_LEVEL				= 0x03, //network signal strength
	FLC_BT_EVENT_BATTERY_LEVEL				= 0x04, //battery level
	FLC_BT_EVENT_HFPCONNECT_STATE			= 0x05, //远端设备HFP的连接状态
	FLC_BT_EVENT_A2DPCONNECT_STATE			= 0x06, //远端设备A2DP的连接状态
	FLC_BT_EVENT_CONNECT_STATE				= 0x07, //result of the connection/disconnection
	FLC_BT_EVENT_DUNSERVICE_SUPPORT			= 0x08, //Is remote device DUN services support
	FLC_BT_EVENT_PANSERVICE_SUPPORT			= 0x09, //Is remote device PAN services support
	FLC_BT_EVENT_STANDBY_ON					= 0x0A, //Bluetooth Sleep
	FLC_BT_EVENT_STANDBY_OFF					= 0x0B, //Bluetooth Wake

	FLC_BT_EVENT_INQUIRY_DEVICE				= 0x10, //Bluetooth devices inquiry
	FLC_BT_EVENT_PAIR_REQUEST				= 0x11, //Paired Request
	FLC_BT_EVENT_PAIR_RESULT					= 0x12, //Result of a Pairing Request
	FLC_BT_EVENT_PAIRD_DEVICE_INFO			= 0x13, //Paired Bluetooth Devices
	FLC_BT_EVENT_DELETE_PAIRED				= 0x14, //Delete a Pairing
	FLC_BT_EVENT_LOCALADRESS					= 0x15, //车机本地蓝牙模块的MAC地址
	FLC_BT_EVENT_INQUIRY_DEVICE_ADDRESS		= 0x16, //Bluetooth devices inquiry address
	FLC_BT_EVENT_INQUIRY_START				= 0x17, //Bluetooth devices inquiry start

	FLC_BT_EVENT_CALL_COUNT					= 0x20, //Number of current ongoing calls. In the range [0..6]
	FLC_BT_EVENT_CALL_STATE					= 0x21, //Call status indicator
	FLC_BT_EVENT_AUDIOTRANSFER_STATE			= 0x22, //Audio transfer
	FLC_BT_EVENT_MIC_MUTE_STATE				= 0x23,	//Micphone mute staus

	FLC_BT_EVENT_PB_SYNC_INDICATOR			= 0x30,	//Phonebook sync indicator
	FLC_BT_EVENT_PB_ENTRIES					= 0x31,	//电话本的具体信息
	FLC_BT_EVENT_LETTER_POS					= 0x32, //搜索到的字母最开始的位置
	FLC_BT_EVENT_CALLLOG_ENTRIES				= 0x33, //通讯记录具体信息

	FLC_BT_EVENT_AUDIO_SOURCE_INDICATOR		= 0x40, //Audio Source Indicator如：蓝牙电话、A2DP等
	FLC_BT_EVENT_A2DP_PLAYSTATE				= 0x41, //Music Player play state
	FLC_BT_EVENT_MUSIC_ID3					= 0x42, //Music ID3 total information
	FLC_BT_EVENT_MUSIC_LENGTH_STATUS		= 0x43, //Music ID3 total information


	FLC_BT_EVENT_MODULE_VERSION				= 0x50, //Bluetooth module's soft Version
	FLC_BT_EVENT_MODULE_NAME					= 0x51, //Bluetooth module's Name
	FLC_BT_EVENT_SETUP_INI					= 0x52, //Setup flag:autoconnect,autoanswer,auotosyncphonebook
	FLC_BT_EVENT_PIN_CODE					= 0x53, //PinCode
	FLC_BT_EVENT_MIC_MODE					= 0x54, //Setup flag:MicPhone Selection
	FLC_BT_EVENT_MIC_GAIN					= 0x55, //Setup flag:MicGain value
	FLC_BT_EVENT_AUTO_CONNECT				= 0x56, //Setup flag:autoconnect
	FLC_BT_EVENT_AUTO_ANSWER					= 0x57, //Setup flag:autoanswer
	FLC_BT_EVENT_AUTO_SYNCPB					= 0x58, //Setup flag:auotosyncphonebook
	FLC_BT_EVENT_SPEAKER_GAIN				= 0x59, //Setup flag:SpeakerGain value
    FLC_BT_HID_CONNECTED                     =0x60,
	FLC_BT_HID_DISCONNECTED,
    FLC_BT_SPP_CONNECTED,
    FLC_BT_SPP_DISCONNECTED,
    FLC_BT_SPP_CALACK,//校屏确认
	FLC_BT_SPP_GET_RECT,
    FLC_BT_SPP_HOMEKEY,//发送home key
};

enum ACTION_TYPE
{
	ACTION_GET,
	ACTION_SET,
};

enum INQUIRY_ACTION
{
	INQUIRY_ACTION_STOP,			//取消或者停止搜索
	INQUIRY_ACTION_START,			//开始搜索
};

enum TEST_MODE_ACTION
{
	TEST_MODE_ACTION_CLOSE,		//关闭测试模式
	TEST_MODE_ACTION_OPEN,		//开启测试模式
};

enum PAIR_ACTION
{
	PAIR_ACTION_INITIATE,			//模块主动配对其他远端设备
	PAIR_ACTION_ACCEPT_REMOTE,		//远端设备请求配对时，模块允许配对
	PAIR_ACTION_REFUSE_REMOTE,		//远端设备请求配对时，模块拒绝配对
};

enum SERVICE_TYPE
{
	SERVICE_TYPE_ALL_SERVICE,							//远端蓝牙设备所支持的所有服务
	SERVICE_TYPE_HFP,				
	SERVICE_TYPE_DUN,				
	SERVICE_TYPE_A2DP,
	SERVICE_TYPE_PAN = 11,				
};

enum PAN_SERVICE_SUPPORT
{
  PAN_SERVICE_SUPPORT_NO, //远端设备不支持PAN功能或者无法获取到是否支持信息
  PAN_SERVICE_SUPPORT_YES, //远端设备支持PAN 功能
};

enum DEVICE_CONNECT_ACTION
{
	DEVICE_CONNECT_ACTION_DISCONNECT,	//断开模块与远端设备的连接
	DEVICE_CONNECT_ACTION_CONNECT,		//模块连接远端设备
};

enum DEVICE_DELETE_ACTION
{
	DEVICE_DELETE_NO,		//设备删除失败
	DEVICE_DELETE_YES,		//设备删除成功
};

enum MULTICALLS_CONTROL_TYPE
{
	MULTICALLS_CONTROL_TYPE_HOLD1STCALL,		//多方通话模式，挂起先前通话，接听来电
	MULTICALLS_CONTROL_TYPE_END1STCALL,		    //多方通话模式，挂断先前通话，接听来电
	MULTICALLS_CONTROL_TYPE_ENDINCOMINGCALL,	//多方通话模式，继续先前通话，挂断来电
	MULTICALLS_CONTROL_TYPE_SWITCHCALLS,		//多方通话模式，却换挂起和正在通话状态
};
enum DOWNLOAD_PB_ACTION
{
	DOWNLOAD_PB_ACTION_NO,
	DOWNLOAD_PB_ACTION_YES,
};
enum DIAL_STATE
{
	DIAL_STATE_TALKING,		//通话中
	DIAL_STATE_HOLDING,		//保持中
	DIAL_STATE_CALLOUT,		//拨出
	DIAL_STATE_INCOMING,	//来电
	DIAL_STATE_WAITING,		//等待
	DIAL_STATE_HANGUP,		//挂断 
};

enum CALLLOG_TYPE
{
	CALLLOG_TYPE_ALL =0,			//全部记录
	CALLLOG_TYPE_LASTDIAL = 1,		//已拨电话	
	CALLLOG_TYPE_RECEIVEDCALL,		//已接电话
	CALLLOG_TYPE_MISSEDCALL,		//未接电话
};

enum PLAY_CTL_TYPE
{
	PLAY_CTL_TYPE_STOP,					//Stop playing
	PLAY_CTL_TYPE_PAUSE,				//Pause playing. (if player is in pause/mute or stop state, an error is returned).
	PLAY_CTL_TYPE_PLAY,					//Resume playing.
	PLAY_CTL_TYPE_NEXT,					//Play next song.
	PLAY_CTL_TYPE_PREVIOUS,				//Play previous song
	PLAY_CTL_TYPE_FAST_F,				//Fast forward playing.
	PLAY_CTL_TYPE_FAST_R,				//Fast rewind playing.
	PLAY_CTL_TYPE_PAUSE_PLAY = 8,		//Toogle pause/resume.(if player is in play state do a pause, otherwise do a resume)
};

enum SETUP_MENU_TYPE
{
	SETUP_MENU_TYPE_AUTOCONNECT,
	SETUP_MENU_TYPE_AUTOANSWER,
	SETUP_MENU_TYPE_AUTOSYNCPB,
	SETUP_MENU_TYPE_MICSELECT,
	SETUP_MENU_TYPE_MICGAIN,
	SETUP_MENU_TYPE_SPEAKERGAIN,
	SETUP_MENU_TYPE_CALLLOG,
	SETUP_MENU_TYPE_VOICEBROADCAST,
	SETUP_MENU_TYPE_VCARDNAMEORDER,
};

enum VCARD_NAME_ORDER
{
	VCARD_NAME_ORDER_FAMILLY_LAST,		//sort by family name last 
	VCARD_NAME_ORDER_FAMILLY_FIRST,		//sort by family name first 
};

enum CALLLOG_SYNC_MODE
{
	CALLLOG_SYNC_ALL, //通讯录下载的时候不分类，全部下载
	CALLLOG_SYNC_DETAIL, //通讯录下载的时候分类（按照已接、已拨、未接类型）
};

enum VOICE_BROADCAST_MODE
{
	VOICE_BROADCAST_MODE_NO,         //来电信息不语音播报
	VOICE_BROADCAST_MODE_YES,        //来电信息语音播报
};

enum WARNING_EVENT
{
	WARNINGEVENT_FLASH_FULL= 1,				//模块的flash已满，无法再保存数据了
	WARNINGEVENT_BRUTALD_IS_CONNECT,		//蓝牙连接断开，可能由于远离了蓝牙设备通讯范围		
	WARNINGEVENT_RAM_FULL,					//RAM空间已满
	WARNINGEVENT_CHIP_ERROR,				//芯片程序出错
	WARNINGEVENT_DIAL_ERROR=11,				//拨号错误
	WARNINGEVENT_HANGUP_ERROR,				//挂断错误
	WARNINGEVENT_ANSWER_ERROR,				//接听错误
	WARNINGEVENT_TWO_CALLS_ERROR,			//2路电话错误
	WARNINGEVENT_DTMF_ERROR,				//DTMF拨号错误
	WARNINGEVENT_BTRH_ERROR,				//日本模式的挂断和相应错误				
	WARNINGEVENT_SIVR_ERROR=18,				//语音识别错误
	WARNINGEVENT_AVRCP_ERROR=21,			//AVRCP错误
	WARNINGEVENT_A2DP_ERROR=31,				//A2DP错误
	WARNINGEVENT_USB_ERROR=41,				//USB错误
	WARNINGEVENT_PHONEBOOK_FULL=61,			//手机存储的电话本超出了模块能处理的大小范围
};

enum INIT_STATE
{
	INIT_STATE_ERROR,			//初始化错误
	INIT_STATE_OK,				//初始化OK
};

enum STANDBY_ON_RESULT
{
	STANDBY_ON_RESULT_ERROR =0x0, //Standby on函数执行过程中出错未成功
	STANDBY_ON_RESULT_OK, //Standby on函数顺利执行完所有功能。
};

enum STANDBY_OFF_RESULT
{
	STANDBY_OFF_RESULT_ERROR =0x0, //Standby off函数执行过程中出错未成功
	STANDBY_OFF_RESULT_OK, //Standby off函数顺利执行完所有功能。
};

enum DEINIT_STATE
{
  DEINIT_STATE_ERROR,           //退出错误
  DEINIT_STATE_OK,              //退出成功
};
enum HFP_CONNECT_STATE
{
	HFP_CONNECT_STATE_DISCONNECT,	//远端设备的HFP未连接
	HFP_CONNECT_STATE_CONNECT,		//远端设备的HFP已连接
};
enum A2DP_CONNECT_STATE
{
	A2DP_CONNECT_STATE_DISCONNECT,	//远端设备的A2DP未连接
	A2DP_CONNECT_STATE_CONNECT,		//远端设备的A2DP已连接
};
enum DUN_SERVICE_SUPPORT
{
	DUN_SERVICE_SUPPORT_NO,			//远端设备不支持DUN功能
	DUN_SERVICE_SUPPORT_YES,		//远端设备支持DUN功能
};

enum PAIR_RESULT
{
	PAIR_RESULT_FAILED,				//failed :Extented pairing error code disabled, unknown reason
	PAIR_RESULT_OK,					//succeeded
	PAIR_RESULT_KO,					//failed :no PIN code or bad PIN code given, timeout reached on target or phone
	PAIR_RESULT_NOT_FOUND,			//Device not found
	PAIR_RESULT_MAX_DEV_REACHED,	//Maximum number of paired devices has been reached
};
enum  PB_SOURCE
{
	PB_SOURCE_PHONE,				//电话本来自于手机
	PB_SOURCE_SIM					//电话本来自于SIM卡
};

enum PB_SYNC_STATE
{
	PB_SYNC_STATE_FAIL = 0, //从手机端同步电话本失败（如手机解决、手机确认超时等）
	PB_SYNC_STATE_START, //电话本下载正常开始
	PB_SYNC_STATE_NONSUPPORT,//手机不支持电话本下载（针对PBAP协议）
};

enum PB_NUMER_TYPE
{
	PB_NUMER_TYPE_PREF,				//
	PB_NUMER_TYPE_OTHER,			//
	PB_NUMER_TYPE_HOME,				//
	PB_NUMER_TYPE_WORK,				//
	PB_NUMER_TYPE_CELL,				//
	PB_NUMER_TYPE_VOICE,			//
	PB_NUMER_TYPE_FAX,				//
	PB_NUMER_TYPE_PAGER,			//
	PB_NUMER_TYPE_MODEM,			//
	PB_NUMER_TYPE_CAR,				//
	PB_NUMER_TYPE_ISDN,				//
};

enum CONNECT_RESULT
{
	CONNECT_RESULT_CONNECT_OK,
	CONNECT_RESULT_CONNECT_FAILED,
	CONNECT_RESULT_CONNECT_NOT_FOUND,
	CONNECT_RESULT_DISCONNECT_OK,
	CONNECT_RESULT_DISCONNECT_KO,
	CONNECT_RESULT_AUTOCONNECT,
	CONNECT_RESULT_CONNECT_ABORT,
	DEFAULT_SERVICES_CONNECTED,
};

enum AUDIO_TRANSFER_MODE
{
	AUDIO_TRANSFER_MODE_PHONE,		//音频转移到手机上
	AUDIO_TRANSFER_MODE_HANDFREE,	//音频转移到模块上
};

enum MICPHONE_MUTE_STATE
{
	MICPHONE_MUTE_STATE_MUTED,		//麦克风静音状态
	MICPHONE_MUTE_STATE_UNMUTED,	//麦克风非静音状态
};

enum RING_ACTION
{
	RING_ACTION_STOP,		//Stop ringtone or jingle
	RING_ACTION_PLAY,		//Play a ringtone (continuous mode)
};

enum AUDIO_SOURCE
{
	AUDIO_SOURCE_NOACTIVE,
	AUDIO_SOURCE_COMM = 2,	//Communication; phone call
	AUDIO_SOURCE_STREAMING,	//Streaming in progress (A2DP )
};
enum AUDIO_STATE
{
	AUDIO_STATE_INACTIVE,	//source is inactive
	AUDIO_STATE_ACTIVE,		//source is active
};
enum PLAYER_STATE
{
	PLAYER_STATE_STOP,
	PLAYER_STATE_PAUSE,
    PLAYER_STATE_PLAY,
    PLAYER_STATE_FF,
    PLAYER_STATE_FR,
};

enum AV_MEDIA_STATUS
{
    AV_MEDIA_STATUS_STOPPED   =    0x00,
    AV_MEDIA_STATUS_PAUSED    =    0x01,
    AV_MEDIA_STATUS_PLAYING   =    0x02,
    AV_MEDIA_STATUS_FWD_SEEK  =    0x03,
    AV_MEDIA_STATUS_REV_SEEK  =    0x04,
   // AV_MEDIA_STATUS_ERROR     =    0xFF,
};


enum MICROPHONE_SELECT_MODE
{
	MICROPHONE_SELECT_MODE_MIC1,	//Microphone 1.内置MIC
	MICROPHONE_SELECT_MODE_MIC2,	//Microphone 2.外接MIC
};
enum AUTO_CONNECT_MODE
{
	AUTO_CONNECT_MODE_DISABLED,		// 不自动连接
	AUTO_CONNECT_MODE_ENABLED,		// 自动连接
};
enum AUTO_ANSWER_MODE
{
	AUTO_ANSWER_MODE_DISABLED,		//不自动接听
	AUTO_ANSWER_MODE_ENABLED,		// 自动接听
};
enum AUTO_SYNC_PB_MODE
{
	AUTO_SYNC_PB_MODE_DISABLED,		//不自动同步电话本
	AUTO_SYNC_PB_MODE_ENABLED,		//自动同步电话本
};
enum Mouse_Control{
    MOVE,
    DOWN,
    UP,
};



enum KeyEvent{
    HOME,
    MENU,
    BACK,
};
typedef void (*SppDataProc)(char *data, int dataLen);
typedef void (*MouseDataProc)(int x, int y, Mouse_Control ctr, bool bIs);
struct strDeviceInfo
{
	WCHAR ID[5];			//	BT Device ID
	WCHAR DeviceName[32];	//	BT Device name	//32
	WCHAR DevAdd[18];		//	BT Device address 
	UINT  Connected;		//	connecte state 1:connected 0:no connected
	HFP_CONNECT_STATE	HFPState;		//远端设备HFP连接状态
	A2DP_CONNECT_STATE	A2DPState;		//远端设备A2DP连接状态

	strDeviceInfo()
	{
		ZeroMemory(ID,sizeof(ID));
		ZeroMemory(DeviceName,sizeof(DeviceName));
		ZeroMemory(DevAdd,sizeof(DevAdd));
		Connected	= 0;
		HFPState	= HFP_CONNECT_STATE_DISCONNECT;
		A2DPState	= A2DP_CONNECT_STATE_DISCONNECT;
	}
};

struct strRequestDeviceInfo
{
	WCHAR ID[5];			//	BT Device ID
	WCHAR DeviceName[32];	//	BT Device name	//32
	WCHAR DevAdd[18];		//	BT Device address
	WCHAR PassKey[10];		//  Passkey

	strRequestDeviceInfo()
	{
		ZeroMemory(ID,sizeof(ID));
		ZeroMemory(DeviceName,sizeof(DeviceName));
		ZeroMemory(DevAdd,sizeof(DevAdd));
		ZeroMemory(PassKey,sizeof(PassKey));
	}
};

struct strCallStatus 
{
	int iCallIndex;			//Value which identifies the call.In the range [0..6]
	DIAL_STATE iCallState;	//state of the call.
	WCHAR wcCallerName[32];/*eshin 20160830*/	//String identifying the remote party,UTF8 format, 50 bytes max //56
	WCHAR wcCallerNum[20];	//String number //36
};

struct strCallLog
{
	int index;					//Index of the contact in the active phone book
	CALLLOG_TYPE pbtype;		//Current phone book type
	WCHAR callnum[20];			//phone number of the entry In ASCIIformat, 32 characters max//34   
	WCHAR calldate[14];			//Date of the call in the active phone book, UTF8 format,32 characters max.//34
	WCHAR calltime[14];			//Time of the call in the active phone book, UTF8 format,32 characters max.//34 
	WCHAR callname[32];	/*eshin 20160830*///Name of the contact in the active phone book,50 bytes max// 52

	strCallLog()
	{
		index = 0;
		pbtype	 = CALLLOG_TYPE_LASTDIAL; 

		ZeroMemory(callnum,sizeof(callnum));
		ZeroMemory(calldate,sizeof(calldate));
		ZeroMemory(calltime,sizeof(calltime));
		ZeroMemory(callname,sizeof(callname));
	}
};

struct strPhoneNum
{
	PB_NUMER_TYPE nNumerType;	//电话号码的性质 如work、home、other、cell、fax等
	WCHAR number[20];
};

struct ContactInfo
{
	UINT  Index;					//当前联系人在电话本中的索引值
	WCHAR Name[32];/*Eshin 20160830 change*/					//联系人姓名
	WCHAR FirstLetter[32];/*eshin 20160830*/			//联系人名字的汉语拼音首字母数组，如王小二 为 WXE
	UINT  PhoneCount;				//联系人的总共号码数
	PB_SOURCE  ntype;				//电话本来源：手机或SIM卡
	std ::vector<strPhoneNum > PhoneNumber; //电话号码的相关属性

	ContactInfo()
	{
		Index = 0;
		PhoneCount = 0;
		ntype = PB_SOURCE_PHONE;
		ZeroMemory(Name,sizeof(Name));
		ZeroMemory(FirstLetter,sizeof(FirstLetter));
	};
};

struct strMusicID3
{
	WCHAR Title[30];		
	WCHAR Artist[30];
	WCHAR Album[30];
	UINT  TrackNum;	

	strMusicID3()
	{
		TrackNum = 0;
		ZeroMemory(Title,sizeof(Title));
		ZeroMemory(Artist,sizeof(Artist));
		ZeroMemory(Album,sizeof(Album));
	}
};


struct strAvPlayStatus
{
    unsigned long   length;      //total lenth
    unsigned long   position;    //current position
    AV_MEDIA_STATUS    status;       //play status

	strAvPlayStatus()
	{
		length = 0;
            position=0;
            status=AV_MEDIA_STATUS_STOPPED;
	}
};

struct strSetupInI
{
	AUTO_CONNECT_MODE	bAutoConnect;
	AUTO_ANSWER_MODE	bAutoAnswer;
	AUTO_SYNC_PB_MODE	bAutoSyncPB;
	MICROPHONE_SELECT_MODE MicMode;
	int Micgain;
	int Speakergain;
	CALLLOG_SYNC_MODE CallLogType;
	VOICE_BROADCAST_MODE VoiceBroadCast;
      VCARD_NAME_ORDER bVcardFamilyNameFirst;

	strSetupInI()
	{
		bAutoConnect = AUTO_CONNECT_MODE_DISABLED;
		bAutoAnswer	= AUTO_ANSWER_MODE_DISABLED;
		bAutoSyncPB	= AUTO_SYNC_PB_MODE_ENABLED;
		MicMode		= MICROPHONE_SELECT_MODE_MIC1;
		Micgain		= 0;
		Speakergain = 0;
		CallLogType = CALLLOG_SYNC_DETAIL;
		VoiceBroadCast = VOICE_BROADCAST_MODE_NO;
            bVcardFamilyNameFirst = VCARD_NAME_ORDER_FAMILLY_LAST;
	}
};


#define FLC_BT_DLL __declspec(dllexport)

extern "C" 
{
	//=============================Initialize Function=======================//
	FLC_BT_DLL void BT_Init(OnBluetoothEvent _event
#if 0
, const TCHAR* cBtPortName ,DWORD uBtBaud2
#endif
);
	FLC_BT_DLL void BT_DeInit();

	FLC_BT_DLL void BT_Standby_On();

	FLC_BT_DLL void BT_Standby_Off();

    FLC_BT_DLL bool BT_SoftwareReset();

	FLC_BT_DLL void BT_GetModuleInfo();

	//============================Device Manage Function======================//
	FLC_BT_DLL void BT_InquiryDevice(INQUIRY_ACTION vAction);
	FLC_BT_DLL void BT_PairDevice(LPCWSTR DeviceID, PAIR_ACTION vAction);
	FLC_BT_DLL void BT_GetPairedDevices();
	FLC_BT_DLL void BT_DeletePairedDevice(LPCWSTR DeviceID);
	FLC_BT_DLL void BT_ConnectPairedDevice(LPCWSTR DeviceID, DEVICE_CONNECT_ACTION vAction, SERVICE_TYPE nType);
    FLC_BT_DLL void BT_SearchServices(LPCWSTR DeviceID);
	FLC_BT_DLL void BT_GetLocalAdress();

	FLC_BT_DLL void BT_PBAPConnect(LPCWSTR DeviceID,DEVICE_CONNECT_ACTION vAction);
	FLC_BT_DLL void BT_HFPConnect(LPCWSTR DeviceID,DEVICE_CONNECT_ACTION vAction);
	FLC_BT_DLL void BT_A2DPConnect(LPCWSTR DeviceID,DEVICE_CONNECT_ACTION vAction);

	//=============================Hand_Free Function==========================//
	FLC_BT_DLL void BT_Dial(LPCWSTR number);
	FLC_BT_DLL void BT_DTMF(WCHAR nKey);
	FLC_BT_DLL void BT_Answer();
	FLC_BT_DLL void BT_Hangup(UINT pIndex);
	FLC_BT_DLL void BT_DailLastCall();
	FLC_BT_DLL void BT_MultiCallsControl(MULTICALLS_CONTROL_TYPE nType);
	FLC_BT_DLL int BT_A2DPSoundControl (bool bState, int iVolume);

	FLC_BT_DLL void BT_AudioTransfer(AUDIO_TRANSFER_MODE nMode);
	FLC_BT_DLL void BT_MicControl(MICPHONE_MUTE_STATE nState);
	FLC_BT_DLL void BT_RingControl(RING_ACTION vAction);

	//=============================Phone book Function=========================//
	FLC_BT_DLL void BT_DownloadPB(DOWNLOAD_PB_ACTION vAction);	//保留
	FLC_BT_DLL void BT_GetCallLog(CALLLOG_TYPE nMode);		//保留

	FLC_BT_DLL void BT_PBSearch(WCHAR letter);

	//=============================Music Function==============================//
	FLC_BT_DLL void BT_MusicControl(PLAY_CTL_TYPE ePlayCtlType);

	//=============================Setup Function==============================//
	FLC_BT_DLL void BT_GetIniSettingValue();
	FLC_BT_DLL void BT_SetIniSettingValue(SETUP_MENU_TYPE index, int nvalue);
	FLC_BT_DLL void BT_SetModuleName(LPCWSTR moduleName);
	FLC_BT_DLL void BT_LoadFactory();	

    //=============================HID SPP Function============================//

    FLC_BT_DLL bool BT_HID_Init(DWORD x, DWORD y, OnBluetoothEvent _hid_event
#if 0
    , const TCHAR* cBtPortName ,DWORD uBtBaud2
#endif
    );
    FLC_BT_DLL void BT_HID_DeInit();

    FLC_BT_DLL void BT_HID_MouseCtrl(int x, int y, Mouse_Control);
    FLC_BT_DLL void BT_HID_SendABSMC(int x, int y, Mouse_Control);
    FLC_BT_DLL void BT_SendKeyEvent(KeyEvent key);
	FLC_BT_DLL void BT_SetValidRect (RECT &rect);

    FLC_BT_DLL void BT_SPP_Init(OnBluetoothEvent _spp_event
#if 0
, const TCHAR* cBtPortName ,DWORD uBtBaud2
#endif
	);
    FLC_BT_DLL void BT_SPP_DeInit();
#if 0
    FLC_BT_DLL void FLC_SppDataHandler(char *data, int Receivelength);

    FLC_BT_DLL void FLC_Spp_Open(SppDataProc SppDataCb);
    FLC_BT_DLL void FLC_Spp_Close();
    FLC_BT_DLL void FLC_Spp_Write(char *data,int length);
#endif
    
    FLC_BT_DLL void BT_InstallMouseHook(MouseDataProc MouseDataCb);
    FLC_BT_DLL void BT_RemoveMouseHook();
    FLC_BT_DLL void BT_SetPhoneData(int type, int para1, int para2);
};

