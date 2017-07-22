/******************************************************************************************************

                CSR plc 2011
                
                All rights reserved

FILE:           a2dp_sink.h

DESCRIPTION:    Header for WPP A2DP sink application

REVISION:       $Revision: #1 $
        
*******************************************************************************************************/
#ifndef _A2DP_SINK_H
#define _A2DP_SINK_H

#include "bluetooth_notification.h"
#include "bluetooth_notification_app_ext.h"
#include "bluetooth_a2dp.h"
#include "a2dp_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define DEBUG_A2DP

#ifdef DEBUG_A2DP
#define DEBUG_A2DP_INIT                 1
#define DEBUG_A2DP_OUTPUT               1
#define DEBUG_A2DP_MESSAGE              1
#define DEBUG_A2DP_DUMP                 1
#define DEBUG_A2DP_WARN                 1
#define DEBUG_A2DP_ERROR                1
#else
#define DEBUG_A2DP_INIT                 0
#define DEBUG_A2DP_OUTPUT               0
#define DEBUG_A2DP_MESSAGE              0
#define DEBUG_A2DP_DUMP                 0
#define DEBUG_A2DP_WARN                 0
#define DEBUG_A2DP_ERROR                1
#endif

#ifndef WPP_REG
#define WPP_REG                         _T("Software\\CSR\\Applications\\WPPAPP")
#endif

#ifndef WPP_LIB_NAME
#define WPP_LIB_NAME                    _T("wpplib")
#endif

#ifndef WPP_BTP_UI_EXT_DLL_NAME
#define WPP_BTP_UI_EXT_DLL_NAME         _T("btp_ui_ext.dll")
#endif

#define A2DP_EVENT                      _T("/btp/a2dp/status")

//#define A2DP_STATUS(dw)                 ((AvsmStatusT)(LOWORD(dw)))

//#define A2DP_EVENT_DATA(s)              MAKELONG(s, 0)

/* General settings */
#define MAX_NUM_FILTERS                  (10)

/* Default PCM sampling settings */
#define DEFAULT_BPS                      (16)    /* Bits per sample */
#define DEFAULT_FREQ                     (48000) /* Sampling frequency */
#define DEFAULT_CHANNELS                 (2)     /* Number of channels, 1:mono, 2:stereo */

/* AVSM status structure */
typedef struct
{
    AvsmStatusT status;
    BT_ADDR     btAddr;
} AvsmStatusEventData;

#define A2DP_EVENT_STATUS(s)             MAKELONG(s, 0)

#define A2DP_EVENT_DATA(buf, x, y)      \
	(buf)->status = (x); \
	(buf)->btAddr = (y);

#define A2DP_STATUS(dw)                  ((AvsmStatusT)(dw->status))
#define A2DP_ADDRESS(dw)                 ((BT_ADDR)(dw->btAddr))

#define INVALID_BT_ADDRESS               (0xffffffffffffffff)
#define VALID_BT_ADDRESS(x)              ((x) != INVALID_BT_ADDRESS)


/* Application instance data */
typedef struct _a2dp_instance_t
{
    AvsmCbStatusT           pfnStateCallback;                   /* AVSM state callback from UI. */
    CRITICAL_SECTION        avsm_cs;                            /* AVSM critical section */
    AvsmStatusT             status;                             /* AVSM status */
    BOOL                    activated;                          /* Flag indicating whether A2DP is activated */

    /* Filter instances and timers */
    struct a2dp_filter_t   *filters[MAX_NUM_FILTERS];           /* Actual filter chain being used */
    void                   *filter_instance[MAX_NUM_FILTERS];   /* Instance data pointers */
    BYTE                    filter_count;                       /* Number of filters */
    struct a2dp_filter_t   *filters_snk[MAX_NUM_FILTERS];       /* Default sink filter (static) */

    /* Global filter/PCM configuration */
    BYTE                    pcm_channels;
    WORD                    pcm_freq;
    BYTE                    pcm_bits;
} a2dp_instance_t;

typedef DWORD (WINAPI *A2dpRegisterT)(AvsmRegisterInfoT *registerInfo);
typedef DWORD (WINAPI *A2dpUnregisterT)();
typedef DWORD (WINAPI *A2dpConnectT)(BT_ADDR bluetoothAddress);
typedef DWORD (WINAPI *A2dpDisconnectT)();

BOOL A2dpInit(LPVOID pfnStateCallback);
void A2dpDeinit();

DWORD A2dpRegister(AvsmRegisterInfoT *registerInfo);
DWORD A2dpUnregister();
DWORD A2dpConnect(BT_ADDR bluetoothAddress);
DWORD A2dpDisconnect();
DWORD A2dpStartStreaming();
DWORD A2dpStopStreaming();
BOOL A2dpIsStreamingOn();
DWORD A2dpMuteStreaming(BOOL bMute);
DWORD A2dpGetSelectedDevice(BT_ADDR* bluetoothAddress);
AvsmStatusT A2dpGetConnectionStatus();
void A2dpSetConnectionStatus(AvsmStatusT status);

LPCTSTR A2dpStatus2String(AvsmStatusT status);
BOOL ConvertBluetoothAddressString(LPTSTR str, BT_ADDR *addr);
void DumpBuffer(BYTE *buf, DWORD size);

/* Filter functions */
void filtersSetup(a2dp_instance_t *instData);
void filtersInstall(a2dp_instance_t *instData);
void filtersKill(a2dp_instance_t *instData);
BOOL filtersRun(a2dp_instance_t *instData, BYTE op);
BOOL filtersCheck(a2dp_instance_t *instData);
void filtersQosUpdate(a2dp_instance_t *instData, WORD qosLevel);
BYTE *filtersGetConfig(a2dp_instance_t *instData, BYTE *length);
BOOL filtersSetConfig(a2dp_instance_t *instData, BYTE *conf, BYTE conf_len);
BYTE *filtersGetCaps(a2dp_instance_t *instData, BYTE *length);
BYTE *filtersRemoteCaps(a2dp_instance_t *instData, BYTE *caps, BYTE caps_len, BYTE *conf_len);

/* Utility functions used by filters to get/set globals */
void getPcmSettings(BYTE *channels, BYTE *bits, WORD *freq);
void setPcmSettings(BYTE channels, BYTE bits, WORD freq);


#ifdef __cplusplus
}
#endif

#endif
