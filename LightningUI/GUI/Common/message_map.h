
#pragma once

//window massage define:
#define WM_MSG_SPEECH									(WM_APP + 0x900)
#define WM_MSG_TOUCH									(WM_APP + 0x901)
#define WM_MSG_MHL										(WM_APP + 0x902)
#define WM_MSG_REARVIEW								(WM_APP + 0x903)
#define WM_MSG_GESTURE									(WM_APP + 0x904)
#define WM_MSG_CARPLAY_ARRIVE					(WM_APP + 0x905)
#define WM_MSG_CARPLAY_REMOVE					(WM_APP + 0x906)
#define WM_MSG_SR											(WM_APP + 0x907)
#define WM_MSG_REARVIEW_COLORSIGNAL							(WM_APP + 0x908)
#define WM_MSG_GESTURE_ID                              (WM_APP + 0x909)
#define WM_MSG_GESTURE_CIRCLE                              (WM_APP + 0x90A)
#define WM_MSG_USB_HUB_ATTACHED                            (WM_APP + 0x90B)


#define WM_BACKGROUND_CHANGE                  (WM_APP+0x90F)
#define WM_CAMERA_TYPE_CHANGE                  (WM_APP+0x910)
#define WM_SWITCH_PANEL								(WM_APP+0x911)
#define WM_GRAPHNOTIFY									(WM_APP+0x912)
#define WM_PANEL_KEY										(WM_APP+0x913)
#define WM_NAVI_PATH_CAHNGED						(WM_APP+0x914)
#define WM_WIFI_STATUS									(WM_APP+0x915)
#define WM_LAMP_STATUS									(WM_APP+0x916)
#define WM_BREAK_STATUS									(WM_APP+0x917)
#define WM_REVERSE_STATUS								(WM_APP+0x918)
#define WM_ON_NOTIFY										(WM_APP+0x919)
#define WM_MCU_CMD										(WM_APP+0x91A)
#define WM_SHOW_MSGBOX								(WM_APP+0x91B)
#define WM_SHOW_EXPLORER								(WM_APP+0x91C)
#define WM_SHOW_KEYBOARD							(WM_APP+0x91D)
#define WM_DISK_CMD										(WM_APP+0x91E)

#define WM_MEDIAPLAYER_MSG						(WM_APP + 0x91F)

#define WM_BLUETOOTH_MESSAGE						(WM_APP + 0x920)
#define WM_BLUETOOTH_INIT_MODULE   		        (WM_APP + 0x921)


#define WM_IPOD_MESSAGE								(WM_APP + 0x923)
#define WM_GPS_VOICE_MESSAGE						(WM_APP + 0x924)
#define WM_DEVICE_INSERT_READY					(WM_APP + 0x925)
#define WM_DEVICE_REMOVE_READY					(WM_APP + 0x926)
#define WM_MODULE_LOADED_SUCCESS			(WM_APP + 0x927)
#define WM_VOLUME_CHANGE							(WM_APP + 0x928)
#define WM_BACKLIGHT_CHANGE						(WM_APP + 0x929)
#define WM_PHONE_HF_STATE							(WM_APP+0x92A)
#define WM_PHONE_KEY										(WM_APP+0x92B)
#define WM_MEDIA_KEY										(WM_APP+0x92C)
#define WM_SYSTEM_POWER_STATE					(WM_APP+0x92D)
#define WM_VEHICLEINFO_CMD							(WM_APP+0x92E)
#define WM_PHONE_CALL_STATE							(WM_APP+0x92F)
#define WM_MUTE_AUDIO_CMD									(WM_APP+0x930)
#define WM_VCHICLE_KEY									(WM_APP+0x931)
#define WM_WIDEGT_CHANGED							(WM_APP+0x932)
#define WM_MEDIA_SOURCE_MSG							(WM_APP+0x933)

#define WM_NOTIFY_SOURCE_INFO						(WM_APP+0x934)
#define WM_NOTIFY_ICON_INFO							(WM_APP+0x935)
#define WM_NOTIFY_RADIO_INFO						(WM_APP+0x936)
#define WM_NOTIFY_MEDIA_PLAYBACK_INFO		(WM_APP+0x937)
#define WM_NOTIFY_VOLUME_INFO					(WM_APP+0x938)

#define WM_MCU_UPDATE_MSG								(WM_APP+0x939)
#define WM_MCU_UPDATE_CMD								(WM_APP+0x93A)

#define WM_NAVI_MSG											(WM_APP+0x93B)
#define WM_MANUAL_MUTE									(WM_APP+0x93C)
#define WM_SPEECH_ENABLE_MSG							(WM_APP+0x93D)

#define WM_VEHICLE_CANBUS_MSG										(WM_APP+0x93F)
#define WM_VEHICLE_RADAR_MSG										(WM_APP+0x940)
#define WM_VEHICLE_SPEED_MSG										(WM_APP+0x941)

#define WM_SYSTEM_PRE_POWER_STATE					(WM_APP+0x942)
#define WM_SMARTBAR_CONTROL							(WM_APP+0x943)
#define WM_SMARTBAR_NOTIFY								(WM_APP+0x944)

#define WM_MEDIAPLAYER_MSG_EX						(WM_APP + 0x945)
#define WM_BLUETOOTH_SETUP_STATUS					(WM_APP + 0x946)
#define WM_CMD_RINGTONE									(WM_APP + 0x947)
#define WM_CMD_PLAY_SOUND_EFFECT					(WM_APP + 0x948)
#define WM_DYNAMIC_LOGO_MSG                         (WM_APP + 0X949)
#define WM_MEDIAPLAYER_CMD								(WM_APP + 0x950)
#define WM_BLUETOOTH_CMD								(WM_APP + 0x951)
#define WM_IMAGEPLAYER_CMD								(WM_APP + 0x952)
#define WM_RADIO_CMD											(WM_APP + 0x953)
#define WM_RINGTONE_MSG									(WM_APP + 0x954)
#define WM_AUDIO_SRC_CMD									(WM_APP + 0x955)
#define WM_VOLUME_CONFIG_MSG						(WM_APP + 0x956)
#define WM_MEDIA_CLASS_SRC_CMD						(WM_APP + 0x957)
#define WM_ENABLE_NAVI_SPEECH							(WM_APP + 0x958)
#define WM_IPOD_CMD											(WM_APP + 0x959)

#define WM_CARPLAY_STATUS									(WM_APP + 0x95A)
#define WM_GET_AUDIO_SRC									(WM_APP + 0x95B)
#define WM_GET_PLAYING_STATUS							(WM_APP + 0x95C)

#define WM_RDS_TA_STATUS									(WM_APP + 0x95D)
#define WM_GET_CUR_MUTE_STATUS								(WM_APP + 0x95E)

#define WM_AUDIO_CODEC_MSG							(WM_APP + 0x95F)
//SR
#define WM_SR_MSG_InitStatus							(WM_APP + 0x960)
#define WM_SR_MSG_UpLoadDictToLocalStatus    (WM_APP + 0x961)
#define WM_SR_MSG_UpLoadDictToCloudStatus    (WM_APP + 0x962)
#define WM_SR_MSG_VolumeLevel						(WM_APP + 0x963)
#define WM_SR_MSG_ResponseTimeout            (WM_APP + 0x964)
#define WM_SR_MSG_SpeechStart					 (WM_APP + 0x965)
#define WM_SR_MSG_SpeechTimeOut              (WM_APP + 0x966)
#define WM_SR_MSG_SpeechEnd						 (WM_APP + 0x967)
#define WM_SR_MSG_Error								(WM_APP + 0x968)
#define WM_SR_MSG_Result								(WM_APP + 0x969)

//Factory Settings
#define WM_RESTORE_FACTORY_SETTINGS                (WM_APP + 0x96A)
#define WM_FACTORY_CMD                             (WM_APP + 0x96B)
#define WM_SHOW_PASSWORD_BOX_CMD					(WM_APP+0x96C)
#define WM_PASSWORD_BOX_CMD							(WM_APP+0x96D)

//Rearview
#define WM_REARVIEW_KEY                             (WM_APP+0x96E)
#define WM_TPMS_CMD									(WM_APP+0x96F)

//DAB
#define WM_DAB_CMD                                  (WM_APP+0X970)
#define WM_DAB_SERVICE_LINK_MSG						(WM_APP+0x971)
#define WM_DAB_SERVICE_START_READY					(WM_APP+0x972)

//KEY PROXY
#define WM_KEY										(WM_APP+0x973)

#define WM_SHOW_WARNING_PAGE						(WM_APP+0x974)

#define WM_HIDE_WARNING_PAGE						(WM_APP+0x975)

#define WM_SMARTBARVOL_EQ							(WM_APP+0x976)

#define WM_CARPLAY_MAC_CHANGED						(WM_APP + 0x977)
#define WM_ANDROID_AUTO_MSG						(WM_APP + 0x978)

//
//Extensible CMD
//
#define WM_EXTENSIBLE_CMD						(WM_APP+0x980)

//param:

//MHL:
#define MHL_VIDEO_MSG									1
#define MHL_AUDIO_MSG									2

//IPOD:
#define IPOD_AUTHENTICATION							1
#define IPOD_PLAY_STATE									3
#define IPOD_TRACK_ALBUM								4
#define IPOD_TRACK_ARTIST								5
#define IPOD_TRACK_ARTWORK							6
#define IPOD_TRACK_NAME									7
#define IPOD_ARTWORK_STATUS							8
#define IPOD_TRACK_INDEX									9
#define IPOD_TRACK_LENGTH								10
#define IPOD_REPEAT											11
#define IPOD_SHUFFLE											12
#define IPOD_TRACK_TIME									13
#define IPOD_TRACK_ARTWORK_STATUS				14
#define IPOD_COMMUNICATION							16
#define IPOD_EXTENDEDUIMODEREADY					20
#define IPOD_OPENFINISH										21
#define IPOD_RECORDREADY_ROOT							22
#define IPOD_RECORDREADY_TOPLEVEL					23
#define IPOD_RECORDREADY_RECORD						24
#define IPOD_READFINISH										25
#define IPOD_TRACK_NUM										26


//Media source type
#define MEDIA_TYPE_INVALID									0
#define MEDIA_TYPE_RADIO									1
#define MEDIA_TYPE_DISK										2
#define MEDIA_TYPE_TV_ANALOG							    3
#define MEDIA_TYPE_NAVI										4
#define MEDIA_TYPE_PHONE									5
#define MEDIA_TYPE_IPOD										6
#define MEDIA_TYPE_AUX										7
#define MEDIA_TYPE_USB1										8
#define MEDIA_TYPE_SD1										9
#define MEDIA_TYPE_DVB_T									10
#define MEDIA_TYPE_A2DP										11
#define MEDIA_TYPE_OTHER									12
#define MEDIA_TYPE_CDC										13
#define MEDIA_TYPE_USB2										14
#define MEDIA_TYPE_SD2										15
#define MEDIA_TYPE_CARPLAY									16
#define MEDIA_TYPE_WEBLINK                                  17
#define MEDIA_TYPE_TV_DIGITAL                               18
#define MEDIA_TYPE_DAB                                      19
#define MEDIA_TYPE_FLASH									20
#define MEDIA_TYPE_ANDROID_AUTO						21
#define MEDIA_TYPE_EASYCONNECT						22
#define MEDIA_TYPE_NUMBER                                   23

//power state:
#define SYS_POWER_NULL								-1
#define SYS_POWER_OFF									0
#define SYS_POWER_ON									1
#define SYS_POWER_SOFT_RESET						2
#define SYS_POWER_IDLE									3
#define SYS_POWER_HARD_RESET					4

#define SYS_SCREEN_NULL                     -1
#define SYS_SCREEN_OFF                      0
#define SYS_SCREEN_ON                       1
#define SYS_SCREEN_IDLE                     2

//for carplay
#define CARPLAY_CONNECT_STATUS				1
#define CARPLAY_CONTROL_STATUS			    2
#define CARPLAY_MAIN_AUDIO_STATUS			3
#define CARPLAY_ALT_AUDIO_STATUS			4
#define CARPLAY_SCREEN_STATUS				5
#define CARPLAY_MODESCHANGED_STATUS			6
#define CARPLAY_REQUESTUI_STATUS			7
#define CARPLAY_DUCK_STATUS			        8
#define CARPLAY_UNDUCK_STATUS			    9
#define CARPLAY_IAP2_CONNECT_STATUS			10
#define CARPLAY_PHONE_AUDIO_FORMAT			11
#define CARPLAY_IPERF_REPORT				12
#define CARPLAY_IPERF_RESULTS				13
#define CARPLAY_SIRI_AUDIO_FORMAT		    14
#define CARPLAY_DISABLE_BLUETOOTH			15

//for smart bar:
#define SMART_BAR_VOLUME					0
#define SMART_BAR_PHONE						1
#define SMART_BAR_MEDIA						2
#define SMART_BAR_DROP						3
#define SMART_BAR_REAR_OVERLAY				4
#define SMART_BAR_TOPBANNER					5
#define SMART_BAR_WARN						6

//for android auto
#define ANDROIDAUTO_CONNECT_STATUS				1
#define ANDROIDAUTO_VOICE_STATUS					2
#define ANDROIDAUTO_NAVI_STATUS						3


//for navigation
#define NAVI_START_MSG				1
#define NAVI_GUIDANCE_MSG			2
