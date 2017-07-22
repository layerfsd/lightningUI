
#pragma once
#include "../../Config/customer.h"
#include <FantasyLib.h>
#include "../../Include/Platform_macro.h"
#include "controls_res.h"
#include "key.h"
#include "message_map.h"
#include "../../Res/Language.h"



//	//Enable AUX
//	#define ENABLE_AUX 1
//
//	//Enable RDS
//	#if ((BSP_CUSTOMER_ID == CUSTOMER_PROLOGY) || (BSP_CUSTOMER_ID == CUSTOMER_ALDINET) || \
//	    (BSP_CUSTOMER_ID == CUSTOMER_ICARTECH) || (BSP_CUSTOMER_ID == CUSTOMER_ERKE) || \
//	    (BSP_CUSTOMER_ID == CUSTOMER_DAIICHI) || (BSP_CUSTOMER_ID == CUSTOMER_DEFAULT) || \
//	    (BSP_CUSTOMER_ID == CUSTOMER_BOSSAUDIO) || (BSP_CUSTOMER_ID == CUSTOMER_CVTE)||\
//		(BSP_CUSTOMER_ID == CUSTOMER_V0XX) || (BSP_CUSTOMER_ID == CUSTOMER_ACTION) ||\
//		(BSP_CUSTOMER_ID == CUSTOMER_PHONOCAR) || (BSP_CUSTOMER_ID == CUSTOMER_AXION)||\
//		(BSP_CUSTOMER_ID == CUSTOMER_CALIBER) ||(BSP_CUSTOMER_ID == CUSTOMER_NEMESIS) )
//	#define ENABLE_RDS	1
//	#else
//	#define ENABLE_RDS	0
//	#endif

//	//Enable DVD
//	#if ((BSP_CUSTOMER_ID == CUSTOMER_MINSHENG) || (BSP_CUSTOMER_ID == CUSTOMER_DEFAULT) || \
//	    (BSP_CUSTOMER_ID == CUSTOMER_CVTE))
//	#define ENABLE_DVD	1
//	#else
//	#define ENABLE_DVD	0
//	#endif

//	//Enable DTV
//	#if ((BSP_CUSTOMER_ID == CUSTOMER_SOUNDEXPRESS) || (BSP_CUSTOMER_ID == CUSTOMER_MINSHENG) || \
//	     (BSP_CUSTOMER_ID == CUSTOMER_DEFAULT) || (BSP_CUSTOMER_ID == CUSTOMER_CVTE))
//	#define ENABLE_DTV	1
//	#else
//	#define ENABLE_DTV	0
//	#endif

//	//Enable DAB
//	#if (USE_CVTE_DAB == 1)
//	#define ENABLE_DAB 1
//	#else
//	#define ENABLE_DAB 0
//	#endif
//
//	#if (BSP_CUSTOMER_ID == CUSTOMER_MINSHENG)
//	#define USE_STEER_WHEEL_OLD_UI		1
//	#else
//	#define USE_STEER_WHEEL_OLD_UI		0
//	#endif

//	#if (USE_PLATFORM_HW_TYPE == HW_PLATFORM_U703PT)
//	#define ENABLE_IDLE_LOGO        1
//	#else
//	#define ENABLE_IDLE_LOGO        0
//	#endif
//
//	#if (BSP_CUSTOMER_ID == CUSTOMER_MINSHENG)
//	#define ENABLE_REAR_VOL_CONTROL 1
//	#else
//	#define ENABLE_REAR_VOL_CONTROL 0
//	#endif

//Idle id
#define IDLE_SCREEN_NONE						0
#define IDLE_SCREEN_ANALOG_CLOCK		1
#define IDLE_SCREEN_DIGITAL_CLOCK			2

//list box width
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
#define LIST_BOX_ITEM_WIDTH     186
#define LIST_BOX_ITEM_HEIGHT    80
#else
#define LIST_BOX_ITEM_WIDTH     210
#define LIST_BOX_ITEM_HEIGHT    80
#endif
#define LIST_BOX_TAG_HEIGHT     72
#define SLI_BOOL_BUTTON_WIDTH   116//92
#define SLI_BOOL_BUTTON_HEIGHT  64//56
#define SLI_SETTING_THICKNESS   16
#define SLI_SETTING_WIDTH       200

//gesture ID
//	#define GEST_DUAL_TOUCH_INVALID              0xFF
//	#define GEST_TOUCH_DOWN_START                 0x2F
//	#define GEST_DUAL_TOUCH_END                     0x4F
#define GEST_SINGLE_TOUCH_END                  0x20
#define GEST_SINGLE_TOUCH_DOUBLE_CLICK     0x22
#define GEST_DUAL_TOUCH_PAN_NORTH           0x30
#define GEST_DUAL_TOUCH_PAN_NORTHEAST       0x32
#define GEST_DUAL_TOUCH_PAN_EAST             0x34
#define GEST_DUAL_TOUCH_PAN_SOUTHEAST       0x36
#define GEST_DUAL_TOUCH_PAN_SOUTH           0x38
#define GEST_DUAL_TOUCH_PAN_SOUTHWEST       0x3A
#define GEST_DUAL_TOUCH_PAN_WEST              0x3C
#define GEST_DUAL_TOUCH_PAN_NORTHWEST       0x3E
#define GEST_DUAL_TOUCH_ZOOM_IN               0x48
#define GEST_DUAL_TOUCH_ZOOM_OUT            0x49

#define TOUCH_STATUS_INVALID      0xFF
#define TOUCH_STATUS_DOWN_START   0x2F
#define TOUCH_STATUS_DOWN         0x2E
#define TOUCH_STATUS_UP           0x4F
#define TOUCH_STATUS_UNKNOW      0x00

//timer id:
#define TIMER_CARPLAY_AUTO_REQUEST_SCREEN		    117
#define TIMER_LOAD_CARPLAY_MODULE							118
#define TIMER_MEDIA_AUTO_RESUME								119
#define TIMER_UPDATE_PLAYINFO									120
#define TIMER_MEDIA_VOLUME_FADE								121
#define TIMER_MEDIA_LOADFILE_READY							122
#define TIMER_MEDIA_LOADFILE_FAILED							123
#define TIMER_MEDIA_PLAY_COMPLETE							124
#define TIMER_MEDIA_AUTO_SELECT_DEVICE					125
#define TIMER_MEDIA_DELAY_OPERATION						126

#define TIMER_CARPLAY_AUTO_SWITCH_MEDIA                 127
#define TIMER_CARPLAY_AUTO_SWITCH_SCREEN				   128
#define TIMER_CARPLAY_AUTO_SWITCH_NATIVEUI			   129

#define TIMER_SPEECH_TERMINATE						130
#define TIMER_AUTO_OPEN_SRC							131
#define TIMER_AUTO_LOAD_MODULES				132
#define TIMER_VEHICLE_INFO_PROC					133
#define TIMER_MAIN_VOLUME_FADE					134
#define TIMER_DEVICE_CHANGED						135
#define TIMER_REVERSE_SIGNAL							136
#define TIMER_AUTO_START_NAVI						137

#define TIMER_RADIO_UPDATE_PLAYINFO			138
#define TIMER_SETTING_ADVANCE_STEERWHEEL    139
#define TIMER_SETTING_ADVANCE_QUERY_STW     140

#define TIMER_BT_PBDOWNLOAD_NOTIFY						141
#define TIMER_BT_A2DP_NOTIFY										142
#define TIMER_BT_INCOMINGCALL_NOTIFY						143
#define TIMER_BT_AUTO_ANSWER									144
#define TIMER_BT_UPDATE_PHONEBOOK							145
#define TIMER_BT_BEGIN_CONNECT								146
#define TIMER_BT_BEGIN_PAIR											147
#define TIMER_BT_CHANGE_AUDIO_DIR							148
#define TIMER_BT_START_RINGTONE								149
#define TIMER_BT_AUTO_CONNECT_AFTER_PAIRED			150
#define TIMER_BT_AUTO_CHANGE_AUDIO_DIR					151
#define TIMER_BT_DELAY_OPERATION								152
#define TIMER_BT_AUTO_CANCLE_DOWNLOAD_PB						153
#define TIMER_BT_AUTO_CANCLE_DOWNLOAD_CALLLOG			154
#define TIMER_BT_MONITOR_PLAYINFO							155
#define TIMER_BT_DISCONNECT_ALL_FINISH					156
#define TIMER_BT_AUTO_POWER_ON								157
#define TIMER_BT_AUTO_POWER_OFF								158
#define TIMER_BT_AUTO_LOAD_MODULE							159
#define TIMER_BT_MONITOR_A2DP_STATUS						160
#define TIMER_BT_AUTO_DOWNLOAD_CALLLOG						161
#define TIMER_BT_AUTO_GET_MODULE_INFO					162
#define TIMER_BT_NOTIFY_POWER_OFF							163
#define TIMER_BT_NOTIFY_POWER_ON							164
#define TIMER_BT_MONITOR_INI_STATUS						165

#define TIMER_RADIO_MONITOR_TA					162
#define TIMER_MIX_VOLUME_FADE                   163
#define TIMER_REARVIEW_PRE_DONE                 164
#define TIMER_REARVIEW_DONE                     165

#define TIMER_IMAGE_LOAD_PREV                   166
#define TIMER_IMAGE_LOAD_NEXT                   167
#define TIMER_IMAGE_LOAD_CUR_HD                 168
#define TIMER_IMAGE_HD_SHOW                     169

//Advance
#define TIMER_SETTING_ADVANCE_PASSWORD_CLEAR     170
#define TIMER_SETTING_ADVANCE_PASSWORD_VERIFY     171

//Gesture
#define TIMER_GESTURE_ID_ENABLE				172
#define TIMER_GESTURE_CIRCLE_ENABLE         173
#define TIMER_GESTURE_DETECT_CHECK          174

#define TIMER_SETTING_GESTURE_POLL          180
#define TIMER_SETTING_GESTURE_LEFT          181
#define TIMER_SETTING_GESTURE_RIGHT         182
#define TIMER_SETTING_GESTURE_UP            183
#define TIMER_SETTING_GESTURE_DOWN          184
#define TIMER_SETTING_GESTURE_FORWARD       185
#define TIMER_SETTING_GESTURE_BACK          186

//DAB
#define TIMER_DAB_MODULE_INIT               187
#define TIMER_DAB_DLS_EFFECT                188

#define TIMER_IMAME_AUTO_PLAY               189

#define TIMER_BT_MONITOR_A2DP_CONNECTION	190

#define TIMER_AUDIO_MUTE_TIMEOUT			191
#define TIMER_AUDIO_FADE_PROC				192
#define TIMER_AUDIO_AUTO_UNDUCK				193

#define TIMER_BLUETOOTH_SWITCH				194

#define TIMER_BLUETOOTH_A2DP_WARNING		195

#define TIMER_DAB_ELLIPSIS                  196
#define TIMER_SYSTEM_POWER_OPERATION_READY	197

#define TIMER_RESTORE_AUDIO_EQ_SETTING		198
#define TIMER_AUTO_RELEASE_SPEECH			199
//for massage box:
#define NB_OK				0x01
#define NB_CANCEL		0x02
#define NB_ABORT		0x04

// for volume
#define VOLUME_FADE_STEP				10

//page ani
enum PAGE_ANI_ID
{
	PAGE_SWITCH_ANI_NONE,
	PAGE_SWITCH_ANI_DEFAULT,
	PAGE_SWITCH_ANI_LEFT,
	PAGE_SWITCH_ANI_RIGHT,
	PAGE_SWITCH_ANI_UP,
	PAGE_SWITCH_ANI_DOWN,
	PAGE_SWITCH_ANI_ZOOMIN,
	PAGE_SWITCH_ANI_ZOOMOUT
};


#define TOUCH_POINT_NUM	 2
#define TOUCH_SHARE_MEM_NAME TEXT("TOUCH_SHARE_MEM_NAME")
#define CARPLAY_TOUCH_EVENT_NAME TEXT("CARPLAY_TOUCH_EVENT_NAME")

#define INVAILD_DISPLAY_WIDTH           50          //48

typedef enum _TOUCH_AREA_
{
    TOUCH_AREA_INVALID = 0x00,
    TOUCH_AREA_GFX_WINDOW,
    TOUCH_AREA_MENU_L_WINDOW,
    TOUCH_AREA_MENU_R_WINDOW,
    TOUCH_AREA_MENU_D_WINDOW,
    TOUCH_AREA_MENU_U_WINDOW
}TOUCHAREA;

typedef struct _TOUCH_STATUS_
{
    BYTE    pointnum;       // 1 | 2 | 3
    BYTE    pointstatus;    // PenDown | PenUp
    BYTE    toucharea;      // Left Invalid Area | Valid Area | Right Invalid Area
    //BYTE    gestureid;      // Touch Start | Touch End | Touch
}TOUCHSTATUS;

typedef struct _SINGLE_POINT_COORDINATE_
{
    BOOL Valid;
    INT  PointX;
    INT  PointY;
    BYTE id;
}SINGLEPOINTCOORDINATE;

typedef struct _MULTI_POINT_COORDINATE_
{
    TOUCHSTATUS           sPointStatus;
    SINGLEPOINTCOORDINATE sPointCoordinate[TOUCH_POINT_NUM];
}MULTIPOINT, *PMULTIPOINT;

typedef enum _GES_FLAG_E
{
	// REGISTER Bank 0 Addr 0x43
	GES_UP_FLAG				 = 0x01,    //BIT(0),
	GES_DOWN_FLAG			 = 0x02,    //BIT(1),
	GES_LEFT_FLAG			 = 0x04,    //BIT(2),
	GES_RIGHT_FLAG			 = 0x08,    //BIT(3),
	GES_FORWARD_FLAG		 = 0x10,    //BIT(4),
	GES_BACKWARD_FLAG		 = 0x20,    //BIT(5),
	GES_CLOCKWISE_FLAG		 = 0x40,    //BIT(6),
	GES_COUNT_CLOCKWISE_FLAG = 0x80,    //BIT(7),
	//REGISTER Bank 0 Addr 0x44
	GES_WAVE_FLAG		     = 0x01    //BIT(0),
};

typedef struct _PAC_GESTURE_DATE_
{
    BYTE nGesture;
    BYTE nWave;
}PACGestureData_t, *PPACGestureData_t;


typedef struct _SWITCH_PAGE_PARAM_
{
	UINT idClass;
	UINT idPage;
	PAGE_ANI_ID idSwitchAniId;
	UINT pageParam;
	LPVOID lpAniPara;
	BOOL	bUserInteraction;
}SWITCH_PAGE_PARAM, *PSWITCH_PAGE_PARAM;

enum DUCK_STATUS
{
	DUCK_NO_VOLUME,
	DUCK_LOW_VOLUME,
	DUCK_NORMAL_VOLUME,
};

BOOL SwitchPage(UINT idClass,UINT idPage,PAGE_ANI_ID idSwitchAniId=PAGE_SWITCH_ANI_DEFAULT,UINT pageParam=0,LPVOID lpAniPara=0,BOOL	bUserInteraction=FALSE);
BOOL EnableAudio(BOOL bEnable);
BOOL EnableNaviSpeech(BOOL bEnable);
BOOL SetNativeNaviSpeechVolume(DWORD dwVolume);
BOOL DuckNativeNaviSpeech(DUCK_STATUS duckStatus);
const wchar_t* GetResourceString(UINT id,UINT idLang=-1);
MYCODEPAGE GetLanguageCodePage(UINT idLang);

typedef struct _KEYBOARDMSG
{
	UINT idCaller;
	NOTIFYPROC pCallbackFunc;
	LPVOID lpCallbackParam;
	UINT type;
	const wchar_t* pStrTitle;
	const wchar_t* pStrIni;
	UINT iMaxInputLen;
	BOOL bAutoReturn;
}KEYBOARDMSG,*LPKEYBOARDMSG;

void ShowKeyboard(UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,UINT type,const wchar_t* pStrTitle,const wchar_t* pStrIni,UINT iMaxInputLen=16,BOOL bAutoReturn=TRUE);


typedef struct _EXPLORERMSG
{
	UINT idCaller;
	NOTIFYPROC pCallbackFunc;
	LPVOID lpCallbackParam;
	const wchar_t *lpTitle;
	const wchar_t *lpPath;
	const wchar_t *lpFileFilter;
}EXPLORERMSG,*LPEXPLORERMSG;

typedef struct _NBMSG
{
	UINT idCaller;
	NOTIFYPROC pCallbackFunc;
	LPVOID lpCallbackParam;
	UINT type;
	const wchar_t *pstrTitle;
	const wchar_t *pstrNotification;
	UINT idIcon;
	DWORD dwShowTime;
	BOOL bAutoHide;
}NBMSG,*LPNBMSG;

typedef struct
{
	unsigned int Year;
	unsigned int Month;
	unsigned int Date;
	unsigned int Hours;
	unsigned int Minutes;
	unsigned int Seconds;
}BuildDateTime;

void ShowPasswordBox(HWND hWnd,UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,UINT type,const wchar_t *pstrTitle,
						 const wchar_t *pstrNotification, UINT idIcon, BOOL bAutoHide=FALSE, DWORD dwShowTime=INFINITE);

void ShowExplorer(UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,const wchar_t *lpTitle,const wchar_t *lpPath,const wchar_t *lpFileFilter);

void ShowNotificationBox(HWND hWnd,UINT idCaller,NOTIFYPROC pCallbackFunc,LPVOID lpCallbackParam,UINT type,const wchar_t *pstrTitle,
						 const wchar_t *pstrNotification, UINT idIcon, BOOL bAutoHide=TRUE, DWORD dwShowTime=INFINITE);
void HideNotificationBox(HWND hWnd);

void SendMediaCommand(WPARAM wParam, LPARAM lParam);

void SendBluetoothCommand(WPARAM wParam, LPARAM lParam);

void SendRadioCommand(WPARAM wParam, LPARAM lParam);

void SendiPodCommand(WPARAM wParam, LPARAM lParam);

void SendFactoryCommand(WPARAM wParam, LPARAM lParam);

UINT GetAudioSRC();
BOOL SwitchAudioSRC(UINT idSRC,BOOL bAutoMute=FALSE,UINT iMuteDelay=400,BOOL bUnmuteFade=TRUE);

BOOL SwitchMediaClassSRC(UINT idClass,UINT iParam=0);

BOOL PlaySoundEffect(UINT idSnd);
BOOL GetCurrentSrcPlayingStatus();
BOOL GetCurrentMuteStatus();
void MuteAudio(BOOL bMute,BOOL bFade,UINT type,BOOL bAutoRelease,UINT time_out);
void GetBuildDateTime(BuildDateTime *p_BuildDateTime);
BOOL IsUseDST(TIME_ZONE_INFORMATION &tziCurrent, SYSTEMTIME &stCurrent);
BOOL IsValidMcuUpdateFile(const wchar_t* fileUpdate, const wchar_t* fileChecksum);
BOOL IsPanelAutoUnMute(UINT idPanel);
BOOL IsPanelHasValidAudioSrc(UINT idPanel);
BOOL IsPanelAutoSwitchMediaSRC(UINT idPanel);
UINT GetPanelAudioSrc(UINT idPanel);
