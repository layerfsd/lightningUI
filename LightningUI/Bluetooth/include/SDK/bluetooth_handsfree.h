/******************************************************************************************************

                CSR plc 2014

                All rights reserved

FILE:           bluetooth_handsfree.h

DESCRIPTION:    Handsfree API

REVISION:       $Revision: #2 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HANDSFREE_H
#define _BLUETOOTH_HANDSFREE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNDER_CE
typedef unsigned __int64 BT_ADDR;
#endif

#ifdef BLUETOOTH_HANDSFREE_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_HANDSFREE_API extern
#else
#define BLUETOOTH_HANDSFREE_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_HANDSFREE_API __declspec(dllimport)
#endif

#define HFM_BLUETOOTH_STATUS_ON     0x0001
#define HFM_BLUETOOTH_STATUS_OFF    0x0002

typedef unsigned int    HfmCallbackModeT;
typedef unsigned int    HfmBluetoothStatusT;
typedef unsigned int    HfmAudioDirectionT;
typedef unsigned char   HfmGainT;
typedef unsigned int    HfmThreewayHandlerT;
typedef unsigned int    HfmCallIndexT;

#define MAX_PHONE_NUMBER    50
#define MAX_PHONE_NAME      128
#define MAX_OPERATOR_LEN    17
#define MAX_APLX_NAME_LEN   16

#define CALL_INFO_DIR_MOBILE_ORIGINATED_CALL    0
#define CALL_INFO_DIR_MOBILE_TERMINATED_CALL    1

typedef struct
{
    BYTE  index;        /* call identification number */
    BYTE  direction;    /* 0 : mobile originated (MO) call, 1 : mobile terminated (MT) call */
    BYTE  status;       /* state of the call */
    BYTE  mode;         /* bearer/teleservice */
    BYTE  mpty;         /* 1: call is one of multiparty (conference) call parties, 0 : not */
    DWORD type;         /* TOA: type of phone number/address */
    TCHAR phoneNumbers[MAX_PHONE_NUMBER];
    TCHAR name[MAX_PHONE_NAME];    /* Alphanumeric representation of <number> corresponding to the entry found in phonebook. */
} CallInfoT;

typedef enum
{
    HfmModeNormal,
    HfmModeATCommand
} HfmMode;

typedef struct
{ 
    USHORT vendorID; 
    USHORT productID;
    USHORT version;
    USHORT features;
} Aplx;

#define APL_XAPL_ACCESSORY_FEATURES_BIT0_RESERVED                        ((USHORT)0x0001)
#define APL_XAPL_ACCESSORY_FEATURES_BIT1_SUPPORT_BATTERY_REPORT          ((USHORT)0x0002)
#define APL_XAPL_ACCESSORY_FEATURES_BIT2_SUPPORT_DOCKED_POWERED          ((USHORT)0x0004)
#define APL_XAPL_ACCESSORY_FEATURES_BIT3_SUPPORT_SIRI_REPORT             ((USHORT)0x0008)
#define APL_XAPL_ACCESSORY_FEATURES_BIT4_SUPPORT_NOISE_REDUCTION_REPORT  ((USHORT)0x0010)

#define APL_SIRI_NOT_AVAILABLE_ON_THIS_PLATFORM                          ((USHORT)0x0000)   
#define APL_SIRI_AVAILABLE_AND_ENABLED                                   ((USHORT)0x0001) 
#define APL_SIRI_AVAILABLE_BUT_DISABLE                                   ((USHORT)0x0002)

typedef void (WINAPI* HfmCbT)(void);
typedef void (WINAPI* HfmCbIncomingCallT)(BOOL generateRingtone);
typedef void (WINAPI* HfmCbMakeOutgoingCallT)(DWORD result);
typedef void (WINAPI* HfmCbTransferAudioT)(HfmAudioDirectionT direction);
typedef void (WINAPI* HfmCbBluetoothStatusT)(HfmBluetoothStatusT status);
typedef void (WINAPI* HfmCbCallInfoT)(CallInfoT* callInfo , DWORD numberOfCalls, DWORD result);
typedef void (WINAPI* HfmCbThreewayCallStatusT)(CallInfoT* callInfo, DWORD numberOfCalls, DWORD result);
typedef void (WINAPI* HfmDirectATResponseT)(LPSTR response);
typedef void (WINAPI* HfmCbQueryOperatorT)(DWORD result, LPCTSTR operatorName);
typedef void (WINAPI* HfmCbVoiceRecognitionT)(DWORD result, DWORD voiceRecognitionStatus);
typedef void (WINAPI* HfmCbAplSetEyesFreeModeT)(DWORD result);
typedef void (WINAPI* HfmCbAplEnableCustATCmdT)(DWORD result, LPCTSTR string, DWORD features);
typedef void (WINAPI* HfmCbAplQuerySiriStatusT)(DWORD result, DWORD status);

// Used in HfmRegisterReq requests
#define HFM_CM_DIRECT                       (0x0001)
#define HFM_CM_MESSAGE                      (0x0002)

// Used in HfmTransferAudio requests
#define HFM_AD_TRANSFER_TO_GATEWAY          (0x0001)
#define HFM_AD_TRANSFER_TO_DEVICE           (0x0002)

// Used in HfmCbTransferAudioT callbacks
#define HFM_AD_IN_GATEWAY                   (0x0003)
#define HFM_AD_IN_DEVICE                    (0x0004)

// Used in HfmAnswerCall and HfmThreewayCall
#define HFM_NO_THREEWAY                     (0x0800)
#define HFM_NO_INDEX                        (0x0002)

// Used in HfmThreewayCall
#define HFM_RELEASE_ALL_HELD_CALL           (0x0001)
#define HFM_RELEASE_ACTIVE_ACCEPT           (0x0002)
#define HFM_RELEASE_SPECIFIED_CALL          (0x0004)
#define HFM_HOLD_ACTIVE_ACCEPT              (0x0008)
#define HFM_REQUEST_PRIVATE_WITH_SPECIFIED  (0x0010)
#define HFM_ADD_CALL                        (0x0020)
#define HFM_CONNECT_TWO_CALLS               (0x0040)

#define HFM_BTRH_PUT_ON_HOLD                (0x0080)
#define HFM_BTRH_ACCEPT_INCOMING            (0x0100)
#define HFM_BTRH_REJECT_INCOMING            (0x0200)
#define HFM_BTRH_READ_STATUS                (0x0400)

// Threeway calling status
#define HFM_CALL_ACTIVE                     0
#define HFM_CALL_HELD                       1
#define HFM_OUTGOING_CALL                   2
#define HFM_OUTGOING_CALL_ALERT             3
#define HFM_INCOMING_CALL                   4
#define HFM_CALL_WAITING                    5

// Used in HfmRemoteVolumeControlReq requests
#define HFM_INCREASE_SPEAKER                (0xFF)
#define HFM_DECREASE_SPEAKER                (0xFE)

#define HFM_INCREASE_MIC                    (0xFF)
#define HFM_DECREASE_MIC                    (0xFE)

// Voice recognition status
#define HFM_VOICE_RECOGNITION_OFF           0
#define HFM_VOICE_RECOGNITION_ON            1

typedef struct
{
    HfmCallbackModeT            mode;
    HfmCbT                      hfmAnswerCallCb;
    HfmCbT                      hfmRejectCallCb;
    HfmCbT                      hfmTalkingCb;
    HfmCbT                      hfmTerminateCallCb;
    HfmCbBluetoothStatusT       hfmBluetoothStatusCb;
    HfmCbIncomingCallT          hfmIncomingCallCb;
    HfmCbMakeOutgoingCallT      hfmMakeOutgoingCallCb;
    HfmCbTransferAudioT         hfmTransferAudioCb;
    HfmCbT                      hfmCallWaitingCb;
    HfmCbThreewayCallStatusT    hfmThreewayCallStatusCb;
    HfmCbCallInfoT              hfmCallInfoCb;
    HfmDirectATResponseT        hfmDirectATResponseCb;
    HfmCbQueryOperatorT         hfmQueryOperatorCb;
    HfmCbVoiceRecognitionT      hfmVoiceRecognitionCb;
    HfmCbAplEnableCustATCmdT    hfmAplEnableCustATCmdCb;
    HfmCbAplSetEyesFreeModeT    hfmAplSetEyesFreeModeCb;
    HfmCbAplQuerySiriStatusT    hfmAplQuerySiriStatusCb;
    /* Extended flag indicating whether to disable SCO ring tone or not. */
    BOOL                        disableScoRingTone;
} HfmRegisterInfoT;

DWORD BLUETOOTH_HANDSFREE_API HfmRegister(HfmRegisterInfoT* registerInfo);
DWORD BLUETOOTH_HANDSFREE_API HfmUnregister();
DWORD BLUETOOTH_HANDSFREE_API HfmMakeOutgoingCallPhoneNumber(WCHAR* phoneNumber);
DWORD BLUETOOTH_HANDSFREE_API HfmMakeOutgoingCallLastNumber();
DWORD BLUETOOTH_HANDSFREE_API HfmMakeOutgoingCallMemoryDial(DWORD memoryNumber);
DWORD BLUETOOTH_HANDSFREE_API HfmAnswerCall();
DWORD BLUETOOTH_HANDSFREE_API HfmRejectCall();
DWORD BLUETOOTH_HANDSFREE_API HfmSendDTMF(WCHAR character);
DWORD BLUETOOTH_HANDSFREE_API HfmTerminateCall();
DWORD BLUETOOTH_HANDSFREE_API HfmTransferAudio(HfmAudioDirectionT direction);
DWORD BLUETOOTH_HANDSFREE_API HfmSetSpeakerGain(HfmGainT gain);
DWORD BLUETOOTH_HANDSFREE_API HfmSetMicrophoneGain(HfmGainT gain);
DWORD BLUETOOTH_HANDSFREE_API HfmConnect();
DWORD BLUETOOTH_HANDSFREE_API HfmDisconnect();
DWORD BLUETOOTH_HANDSFREE_API HfmSetSelectedDevice(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_HANDSFREE_API HfmGetSelectedDevice(BT_ADDR* bluetoothAddress);
DWORD BLUETOOTH_HANDSFREE_API HfmThreewayCall(HfmThreewayHandlerT threewayHandler, HfmCallIndexT callIndex);
DWORD BLUETOOTH_HANDSFREE_API HfmCallInfo();
DWORD BLUETOOTH_HANDSFREE_API HfmDirectATCommand(LPSTR command, BOOL standardResponse);
DWORD BLUETOOTH_HANDSFREE_API HfmQueryOperator();
DWORD BLUETOOTH_HANDSFREE_API HfmActivateVoiceRecognition(BOOL activate);

/* For Apple Device */
DWORD BLUETOOTH_HANDSFREE_API HfmAplEnableCustATcommand(Aplx settings);
DWORD BLUETOOTH_HANDSFREE_API HfmAplSetEyesFreeMode(BOOL enable);
DWORD BLUETOOTH_HANDSFREE_API HfmAplQuerySiriStatus();
#ifdef __cplusplus
}
#endif

#endif
