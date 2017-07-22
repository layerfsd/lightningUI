/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           bluetooth_hid_monitor.h

DESCRIPTION:    Header for Bluetooth HID monitor API

REVISION:       $Revision: #4 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_MONITOR_H
#define _BLUETOOTH_HID_MONITOR_H

#include "bluetooth_hid_device.h"
#include "bluetooth_hid_screen.h"


#ifdef __cplusplus
extern "C" {
#endif


/* +++ Definition for Bluetooth HID registry key. */
#define BT_HID_REG                              TEXT("Software\\CSR\\Applications\\HID")
#define BT_HID_ADDR_FORMAT                      TEXT("Addr_%012I64x")
#define BT_HID_OUTPUT_RECT_FORMAT               TEXT("%d, %d, %d, %d")  /* left, top, right, bottom */
#define BT_HID_REMOTE_WIDTH_KEY                 TEXT("RemoteWidth")
#define BT_HID_REMOTE_HEIGHT_KEY                TEXT("RemoteHeight")
#define BT_HID_REMOTE_DIRECTION_KEY             TEXT("RemoteDirection")
#define BT_HID_LOCAL_WIDTH_KEY                  TEXT("LocalWidth")
#define BT_HID_LOCAL_HEIGHT_KEY                 TEXT("LocalHeight")
#define BT_HID_LOCAL_DIRECTION_KEY              TEXT("LocalDirection")
#define BT_HID_OUTPUT_LANDSCAPE_RECT            TEXT("OutputLandscapeRect")
#define BT_HID_OUTPUT_PORTRAIT_RECT             TEXT("OutputPortraitRect")
#define BT_HID_POINT_INTERVAL                   TEXT("PointInterval")
#define BT_HID_RESET_ZERO_POINT_TIMEOUT         TEXT("ResetZeroPointTimeout")
#define BT_HID_CHECK_HOLD_MSG_TIMEOUT           TEXT("CheckHoldMsgTimeout")
#define BT_HID_TOUCH_SELECT_TIMEOUT             TEXT("TouchSelectTimeout")
#define BT_HID_TOUCH_SELECT_DISTANCE_TOLERANCE  TEXT("TouchSelectDistanceTolerance")
#define BT_HID_POINTER_VELOCITY_LOWTHRESHOLD    TEXT("PointerVelocityLowThreshold")
#define BT_HID_NOP_COUNT                        TEXT("HidNopCount")
#define BT_HID_TOUCH_POINT_RESAMPLE_FREQ        TEXT("TouchPointResampleFreq")
#define BT_HID_TOUCH_SCROLL_DIST_TOLERANCE      TEXT("TouchScrollDistTolerance")
#define BT_HID_TOUCH_SCROLL_TIME_TOLERANCE      TEXT("TouchScrollTimeTolerance")
#define BT_HID_TOUCH_VELOCITY_SPEED             TEXT("TouchVelocitySpeed")
#define BT_HID_TOUCH_VELOCITY_DISTANCE          TEXT("TouchVelocityDistance")
#define BT_HID_DISTANCE_BASE                    TEXT("HidDistanceBase")
#define BT_HID_PACKET_SEND_INTERVAL_TIME        TEXT("PacketSendIntervalTime")
#define BT_HID_TOUCHU_SLEEP_TIME                TEXT("TouchSleepTime")


/* --- Definition for Bluetooth HID registry key. */


typedef struct
{
    BT_ADDR bluetoothAddress;           /* Remote Bluetooth address. */
    HidScreenInfoT hidScreen;           /* HID screen(local/remote/output) information. */
} HidHostInfo;

/* Function pointer to send HID report. */
typedef DWORD (WINAPI* HiddSendReportFuncPtrT)(HiddReportLenT reportLen, HiddReportT report);

/* Function pointer to enable low power mode. */
typedef DWORD (WINAPI* HiddEnableLowPowerModeFuncPtrT)(BOOL enable);

/* Function pointer to get HFP call status. */
typedef BOOL (WINAPI* HfpIsCallActiveFuncPtrT)();

/* Function pointer to connect/disconnect A2DP. */
typedef DWORD   (WINAPI* A2dpConnectFuncPtrT)(BT_ADDR bluetoothAddress);
typedef DWORD   (WINAPI* A2dpDisconnectFuncPtrT)();
typedef BOOL    (WINAPI* A2dpIsConnectedFuncPtrT)();

/* Function pointer to control media player through AVRCP. */
typedef DWORD   (WINAPI* AvrcpConnectFuncPtrT)(BT_ADDR bluetoothAddress);
typedef DWORD   (WINAPI* AvrcpDisconnectFuncPtrT)();
typedef BOOL    (WINAPI* AvrcpIsConnectedFuncPtrT)();
typedef HRESULT (WINAPI* AvrcpPlayFuncPtrT)();
typedef HRESULT (WINAPI* AvrcpPauseFuncPtrT)();
typedef HRESULT (WINAPI* AvrcpNextFuncPtrT)();
typedef HRESULT (WINAPI* AvrcpPreviousFuncPtrT)();
    
typedef struct
{
    DWORD   size;                       /* Total size in byte for HID moniter register information. */
    BOOL    enableHidForHdmi;           /* Flag indicating whether to enable HID for HDMI. TRUE : enable(default), FALSE : disable. */
    BOOL    enableSppForHid;            /* Flag indicating whether to enable SPP for HID.  TRUE : enable(default), FALSE : disable. */
    BOOL    enableAvrcpForHdmi;         /* Flag indicating whether to enable AVRCP(CT) to control medial player for HDMI.  TRUE : enable(default), FALSE : disable. */
    BOOL    enableA2dpForHdmi;          /* Flag indicating whether to enable A2DP(SNK) to receive audio for HDMI.  TRUE : enable, FALSE : disable(default). */

    HiddSendReportFuncPtrT      hiddSendReport;
    HiddEnableLowPowerModeFuncPtrT  hiddEnableLowPowerMode;

    HfpIsCallActiveFuncPtrT     hfpIsCallActive;

    A2dpConnectFuncPtrT         a2dpConnect;
    A2dpDisconnectFuncPtrT      a2dpDisconnect;
    A2dpIsConnectedFuncPtrT     a2dpIsConnected;

    AvrcpConnectFuncPtrT        avrcpConnect;
    AvrcpDisconnectFuncPtrT     avrcpDisconnect;
    AvrcpIsConnectedFuncPtrT    avrcpIsConnected;
    AvrcpPlayFuncPtrT           avrcpPlay;
    AvrcpPauseFuncPtrT          avrcpPause;
    AvrcpNextFuncPtrT           avrcpNext;
    AvrcpPreviousFuncPtrT       avrcpPrevious;
} HidMonitorRegisterInfoT;


/* HID monitor API. */
BOOL WINAPI HidMonitorInit(HidMonitorRegisterInfoT* registerInfo);
void WINAPI HidMonitorDeinit();

void WINAPI HidMonitorSetConnectionStatus(HidStateT hidState, BT_ADDR bluetoothAddress);

BOOL WINAPI HidMonitorIsHidHost(BT_ADDR bluetoothAddress);

BOOL WINAPI HidMonitorGetHidHostInfo(BT_ADDR bluetoothAddress, HidHostInfo* pHostInfo);

BOOL WINAPI HidMonitorClearHidHostInfo(BT_ADDR bluetoothAddress);

void WINAPI HidMonitorNotifyCallActive(BOOL fCallActive);

DWORD WINAPI HidMonitorHandleTouchMessage(UINT msg, UINT flags, LONG x, LONG y);


#ifdef __cplusplus
}
#endif

#endif
