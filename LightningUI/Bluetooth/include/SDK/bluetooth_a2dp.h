/******************************************************************************************************

                CSR plc 2014

                All rights reserved

FILE:           bluetooth_a2dp.h

DESCRIPTION:    A2DP(SNK) API

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_A2DP_H
#define _BLUETOOTH_A2DP_H

#include <ws2bth.h>
#include "sbc_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNDER_CE
typedef unsigned __int64 BT_ADDR;
#endif

#ifdef BLUETOOTH_A2DP_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_A2DP_API extern
#else
#define BLUETOOTH_A2DP_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_A2DP_API __declspec(dllimport)
#endif

#define MAX_A2DP_CONFIG_SIZE                    64

/* A2DP media types */
typedef BYTE a2dp_media_t;
#define A2DP_AUDIO                              ((a2dp_media_t) 0x00)
#define A2DP_VIDEO                              ((a2dp_media_t) 0x01)
#define A2DP_MULTIMEDIA                         ((a2dp_media_t) 0x02)

/* A2DP media codecs */
typedef BYTE a2dp_codec_t;
#define A2DP_SBC                                ((a2dp_codec_t) 0x00)
#define A2DP_MPEG12_AUDIO                       ((a2dp_codec_t) 0x01)
#define A2DP_MPEG24_AAC                         ((a2dp_codec_t) 0x02)
#define A2DP_ATRAC                              ((a2dp_codec_t) 0x04)
#define A2DP_NON_A2DP_CODEC                     ((a2dp_codec_t) 0xFF)

typedef unsigned short  AvsmStatusT;
#define AvsmSuccess                             ((AvsmStatusT) 0x0)
#define AvsmNull                                ((AvsmStatusT) 0x1)
#define AvsmIdle                                ((AvsmStatusT) 0x2)
#define AvsmConnecting                          ((AvsmStatusT) 0x3)
#define AvsmConnected                           ((AvsmStatusT) 0x4)
#define AvsmDisconnecting                       ((AvsmStatusT) 0x5)
#define AvsmStreamingOn                         ((AvsmStatusT) 0x6)
#define AvsmStreamingOff                        ((AvsmStatusT) 0x7)
#define AvsmConfigured                          ((AvsmStatusT) 0x8)

// Avsm Error Status
#define AvsmConnectFail                         ((AvsmStatusT) 0xF001)
#define AvsmNoStreamEndPoint                    ((AvsmStatusT) 0xF002)
#define AvsmDiscoverFail                        ((AvsmStatusT) 0xF003)
#define AvsmGetCapabilitiesFail                 ((AvsmStatusT) 0xF004)
#define AvsmGetCapabilitiesCfmNoCapSupported    ((AvsmStatusT) 0xF005)
#define AvsmSetConfigurationSbcFail             ((AvsmStatusT) 0xF006)
#define AvsmSetConfigurationFail                ((AvsmStatusT) 0xF007)
#define AvsmGetConfigurationFail                ((AvsmStatusT) 0xF008)
#define AvsmOpenFail                            ((AvsmStatusT) 0xF009)
#define AvsmStartFail                           ((AvsmStatusT) 0xF00A)
#define AvsmWrongState                          ((AvsmStatusT) 0xF00B)

#define AvsmBluetoothStatusOn                   ((AvsmStatusT) 0x1000)
#define AvsmBluetoothStatusOff                  ((AvsmStatusT) 0x2000)

typedef unsigned int    AvsmCallbackModeT;
// Used in AvsmRegisterReq request. Currently only support AVSM_CM_DIRECT
#define AVSM_CM_DIRECT                          ((AvsmCallbackModeT) 0x0001)
#define AVSM_CM_MESSAGE                         ((AvsmCallbackModeT) 0x0002)

typedef void (WINAPI* AvsmCbT)(void);
typedef void (WINAPI* AvsmCbStatusT)(BT_ADDR btAddr, AvsmStatusT status, DWORD result);
typedef void (WINAPI* AvsmCbConfigT)(BYTE* conf, BYTE conf_len);
typedef void (WINAPI* AvsmCbStreamDataT)(BYTE* data, DWORD size);

typedef struct
{
    AvsmCallbackModeT           mode;
    AvsmCbStatusT               avsmStatusCb;
    AvsmCbConfigT               avsmConfigCb;
    AvsmCbStreamDataT           avsmStreamDataCb;
} AvsmRegisterInfoT;

typedef struct
{
    WORD                sample_freq;                    /* Sampling frequency */
    CsrSbcChannelMode   channel_mode;                   /* Channel mode */
    BYTE                blocks;                         /* Block length */
    BYTE                subbands;                       /* Subbands */
    CsrSbcAllocMethod   alloc_method;                   /* Allocation method */
    BYTE                minBitpool;                     /* Remote minimum bitpool value */
    BYTE                maxBitpool;                     /* Remote maximum bitpool value */
} a2dp_sbc_config_t;

typedef struct
{
    a2dp_media_t        media;                          /* media types */
    a2dp_codec_t        codec;                          /* media codecs */
    BYTE                config[MAX_A2DP_CONFIG_SIZE];   /* specific A2DP configuration */
} a2dp_config_t;

DWORD BLUETOOTH_A2DP_API AvsmRegister(AvsmRegisterInfoT* registerInfo);
DWORD BLUETOOTH_A2DP_API AvsmUnregister();
DWORD BLUETOOTH_A2DP_API AvsmConnect(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_A2DP_API AvsmDisconnect();

#ifdef __cplusplus
}
#endif

#endif