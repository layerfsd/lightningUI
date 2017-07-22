/******************************************************************************************************

                CSR plc 2012

                All rights reserved

FILE:           bluetooth_notification_app_ext.h

DESCRIPTION:    Extended Notification API for Application

*******************************************************************************************************/
#ifndef _BLUETOOTH_NOTIFICATION_APP_EXT_H
#define _BLUETOOTH_NOTIFICATION_APP_EXT_H

#ifndef __cplusplus
extern "C" {
#endif


/* +++ Bluetooth Handsfree App Ext +++ */
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_IDLE                            TEXT("Idle")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_INCOMINGCALL                    TEXT("Incoming call")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_OUTGOINGCALL                    TEXT("Outgoing call")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_TALKING                         TEXT("Talking")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_TERMINATING                     TEXT("Terminating")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_WAITING                              TEXT("Waiting call")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_NO_HELD                              TEXT("No held call")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_ACTIVE_AND_HELD                      TEXT("Active call and held call")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_VALUE_HELD_CALL_NO_ACTIVE                  TEXT("No active only held call")

#define BLUETOOTH_NOTIFY_HANDSFREE_CONNECTION_TYPE_VALUE_HEADSET                    TEXT("Headset")
#define BLUETOOTH_NOTIFY_HANDSFREE_CONNECTION_TYPE_VALUE_HANDSFREE                  TEXT("Handsfree")

#define BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS_VALUE_NOHELDCALL                     TEXT("No held call")
#define BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS_VALUE_ACTIVEANDHELDCALL              TEXT("Active call and held call")
#define BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS_VALUE_NOACTIVECALLONLYHELDCALL       TEXT("No active only held call")
/* --- Bluetooth Handsfree App Ext --- */

// General state notify ID                                               
#define BLUETOOTH_NOTIFY_GENERAL_STATE_IDLE                                         0x00000001
#define BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTING                                   0x00000002
#define BLUETOOTH_NOTIFY_GENERAL_STATE_CONNECTED                                    0x00000004
#define BLUETOOTH_NOTIFY_GENERAL_STATE_DISCONNECTING                                0x00000008

// TPMS notify ID                                               
#define BLUETOOTH_NOTIFY_TPMS_SYSTEM_INFO_RECEIVED_SUCCESS							0x00000011
#define BLUETOOTH_NOTIFY_TPMS_SYSTEM_INFO_RECEIVED_FAILED							0x00000012
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_VERSION_RECEIVED_SUCCESS						0x00000013
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_VERSION_RECEIVED_FAILED						0x00000014
#define BLUETOOTH_NOTIFY_TPMS_SMARTPHONE_BOUND_SUCCESS								0x00000015
#define BLUETOOTH_NOTIFY_TPMS_SMARTPHONE_BOUND_FAILED								0x00000016
#define BLUETOOTH_NOTIFY_TPMS_TRANSMITTER_POSITION_SWAPPED_SUCCESS					0x00000017
#define BLUETOOTH_NOTIFY_TPMS_TRANSMITTER_POSITION_SWAPPED_FAILED					0x00000018
#define BLUETOOTH_NOTIFY_TPMS_TRANSMITTER_FOUND										0x00000019
#define	BLUETOOTH_NOTIFY_TPMS_START_BINDING_TRANSMITTER_SUCCESS						0x00000001
#define	BLUETOOTH_NOTIFY_TPMS_START_BINDING_TRANSMITTER_FAILED						0x00000002
#define BLUETOOTH_NOTIFY_TPMS_STOP_BINDING_TRANSMITTER_SUCCESS						0x0000000F
#define BLUETOOTH_NOTIFY_TPMS_STOP_BINDING_TRANSMITTER_FAILED						0x00000010
#define BLUETOOTH_NOTIFY_TPMS_TRANSMITTER_BOUND_SUCCESS								0x00000003
#define BLUETOOTH_NOTIFY_TPMS_TRANSMITTER_BOUND_FAILED								0x00000004
#define BLUETOOTH_NOTIFY_TPMS_TRANSMITTER_DATA_RECEIVED_SUCCESS						0x00000005
#define BLUETOOTH_NOTIFY_TPMS_TRANSMITTER_DATA_RECEIVED_FAILED						0x00000006
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_STATUS_RECEIVED_SUCCESS						0x00000007
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_STATUS_RECEIVED_FAILED						0x00000008
#define BLUETOOTH_NOTIFY_TPMS_SMARTPHONE_CONNECT_STATUS_RECEIVED_SUCCESS			0x00000009
#define BLUETOOTH_NOTIFY_TPMS_SMARTPHONE_CONNECT_STATUS_RECEIVED_FAILED				0x0000000A
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_SETTINGS_RECEIVED_SUCCESS					0x0000000B
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_SETTINGS_RECEIVED_FAILED						0x0000000C
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_SETTINGS_APPLIED_SUCCESS						0x0000000D
#define BLUETOOTH_NOTIFY_TPMS_RECEIVER_SETTINGS_APPLIED_FAILED						0x0000000E
#define BLUETOOTH_NOTIFY_TPMS_ALERT_TEMP                                            0x00000100
#define BLUETOOTH_NOTIFY_TPMS_ALERT_PRESSUREHIGH                                    0x00000020
#define BLUETOOTH_NOTIFY_TPMS_ALERT_PRESSURELOW                                     0x00000040
#define BLUETOOTH_NOTIFY_TPMS_ALERT_RAPID_LEAK                                      0x00000080

// MAP API notify ID                                               
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_GETFOLDERLIST                        0x00000010
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_GETMESSAGELIST                       0x00000020
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_GETMESSAGEDETAIL                     0x00000040
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_PUSHMESSAGE                          0x00000080
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_FAIL_CONNECT                              0x00000100
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_GETFOLDERLIST                     0x00000200
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_GETMESSAGELIST                    0x00000400
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_GETMESSAGE                        0x00000800
#define BLUETOOTH_NOTIFY_MAP_API_CALLBACK_SUCCESS_PUSHMESSAGE                       0x00001000
#define BLUETOOTH_NOTIFY_MAP_DEVICE_EVT_NEW_MESSAGE                                 0x00002000

// SPP API notify ID                                               
#define BLUETOOTH_NOTIFY_SPP_API_CALL_FAIL_CONNECT                                  0x00000010

// HID API notify ID
#define BLUETOOTH_NOTIFY_HID_API_CALL_FAIL_CONNECT                                  0x00000010

// PBAP API notify ID                                               
#define BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_DOWNLOAD_FINISH                            0x00000010
#define BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_SEARCH_FINISH                              0x00000020
#define BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_GETENTRY_FINISH                            0x00000040
#define BLUETOOTH_NOTIFY_PBAP_API_CALL_FAIL_DOWNLOAD                                0x00000080
#define BLUETOOTH_NOTIFY_PBAP_DEVICE_EVT_DOWNLOADED_COUNT					0x00000100

// A2DP API notify ID                                               
#define BLUETOOTH_NOTIFY_A2DP_API_CALL_FAIL_CONNECT                                 0x00000010

// AVRCP API notify ID
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_PLAYER                              0x00000010
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_FOLDER_ITEM                         0x00000011
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_MEDIA_ITEM                          0x00000012
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_MEDIA_ATTR                          0x00000013
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_ADD_MEDIA_ATTR_WRONG                    0x00000014
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_NOW_PLAYING_CONTENT_CHANGED      0x00000015
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_UID_CHANGED                      0x00000016
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_PLAYSTATE_CHANGED			0x00000017
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_METADATA							0x00000018
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_PLAYTIME								0x00000019
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_REPEATMODE								0x0000001A
#define BLUETOOTH_NOTIFY_AVRCP_API_CALLBACK_NOTIFY_SHUFFLEMODE								0x0000001B

// HF API notify ID                                               
#define BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_OUTGOING                           0x00000010
#define BLUETOOTH_NOTIFY_HF_API_CALLBACK_FAIL_OUTGOING                              0x00000020
#define BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_INCOMINGCALL                       0x00000040
#define BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_ANSWERCALL                         0x00000080
#define BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_REJECTCALL                         0x00000100
#define BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_TERMINATE                          0x00000200
#define BLUETOOTH_NOTIFY_HF_DEVICE_EVT_CALLNUM_ZERO                                 0x00000400
#define BLUETOOTH_NOTIFY_HF_API_CALLBACK_SUCCESS_WAITINGCALL                        0x00000800
#define BLUETOOTH_NOTIFY_HF_DEVICE_EVT_CALLNUM_VALID							0x00001000
#define BLUETOOTH_NOTIFY_HF_AUDIO_DIRECTION											0x00002000

#ifndef __cplusplus
}
#endif

#endif