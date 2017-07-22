/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           bluetooth_hid_device.h

DESCRIPTION:    HID Device API for OEM's

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_DEVICE_H
#define _BLUETOOTH_HID_DEVICE_H

#include <ws2bth.h>
#include "bluetooth_module_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BLUETOOTH_HID_DEVICE_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_HID_DEVICE_API extern
#else
#define BLUETOOTH_HID_DEVICE_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_HID_DEVICE_API __declspec(dllimport)
#endif

/* ---------- Generel typedefs: ---------- */
typedef unsigned char   HiddResultT;
typedef unsigned char   HiddControlTypeT;
typedef unsigned short  HiddReportLenT;
typedef unsigned char   HiddReportIdT;
typedef unsigned char * HiddReportT;
typedef unsigned char   HiddReportTypeT;
typedef unsigned char   HiddProtocolTypeT;
typedef unsigned char   HiddIdleRateT;
typedef unsigned char   HiddDeviceTypeT;
typedef unsigned short  HiddServiceRecordLenT;
typedef unsigned char * HiddServiceRecordT;
typedef unsigned short  HidStateT;
/* ---------- Defines: ---------- */
#define HIDD_DEVICE_MOUSE           ((HiddDeviceTypeT) 0x01)
#define HIDD_DEVICE_KEYBOARD        ((HiddDeviceTypeT) 0x02)
#define HIDD_DEVICE_COMBO           ((HiddDeviceTypeT) 0x03)

#define HIDD_REPORT_TYPE_INPUT      ((HiddReportTypeT) 0x01)
#define HIDD_REPORT_TYPE_OUTPUT     ((HiddReportTypeT) 0x02)
#define HIDD_REPORT_TYPE_FEATURE    ((HiddReportTypeT) 0x03)

#define HIDD_PROTOCOL_TYPE_REPORT   ((HiddProtocolTypeT) 0x01)
#define HIDD_PROTOCOL_TYPE_BOOT     ((HiddProtocolTypeT) 0x02)
 
#define HIDD_CONTROL_HARD_RESET     ((HiddControlTypeT) 0x01)
#define HIDD_CONTROL_SOFT_RESET     ((HiddControlTypeT) 0x02)
#define HIDD_CONTROL_SUSPEND        ((HiddControlTypeT) 0x03)
#define HIDD_CONTROL_EXIT_SUSPEND   ((HiddControlTypeT) 0x04)

#define HIDD_SUCCESSFULL            ((HiddResultT) 0x00)
#define HIDD_NOTREADY               ((HiddResultT) 0x01)
#define HIDD_ERRINVALIDREPORTID     ((HiddResultT) 0x02)
#define HIDD_ERRUNSUPPORTEDREQ      ((HiddResultT) 0x03)
#define HIDD_ERRINVALIDPARAM        ((HiddResultT) 0x04)
#define HIDD_ERRUNKNOWN             ((HiddResultT) 0x0e)
#define HIDD_ERRFATAL               ((HiddResultT) 0x0f)


#define HID_STATE_IDLE                ((HidStateT) 0x0)
#define HID_STATE_CONNECTED           ((HidStateT) 0x1)
#define HID_STATE_RECONNECTING        ((HidStateT) 0x2)
 
#define HID_MAXSIZE_REPORT           0x1E

/* ---------- Simple API: ---------- */
typedef void (WINAPI* HiddT) (void);
/*typedef void (WINAPI* BluetoothStatusChange) (LPVOID userdata, ModuleApiBluetoothStatus status);*/
typedef void (WINAPI* HiddHostControlT)     (HiddControlTypeT status);
typedef void (WINAPI* HiddOutputReportT)    (HiddReportLenT reportLen, HiddReportT report);
typedef void (WINAPI* HiddGetReportT)       (HiddReportTypeT reportType, HiddReportIdT reportId, HiddReportLenT* reportLen, HiddReportT report, HiddResultT* result);
typedef void (WINAPI* HiddSetReportT)       (HiddReportTypeT reportType, HiddReportLenT reportLen, HiddReportT report, HiddResultT* result);
typedef void (WINAPI* HIDStatusChangeT)     (HidStateT hidState,BT_ADDR btAddress);

typedef struct 
{
    HiddT                   hiddReadyCb;
    HiddT                   hiddUnregisterCb;
	BluetoothStatusChange   hiddBluetoothStatusCb;
    HIDStatusChangeT        hiddStatusCb;
    HiddHostControlT        hiddHostControlCb;
    HiddOutputReportT       hiddOutputReportCb;
    HiddGetReportT          hiddGetReportCb;
    HiddSetReportT          hiddSetReportCb;
} HiddRegisterT;


/* ---------- For Extended API also: ---------- */
typedef void (WINAPI* HiddGetProtocolT) (HiddProtocolTypeT* protocolType, HiddResultT* result);
typedef void (WINAPI* HiddSetProtocolT) (HiddProtocolTypeT protocolType, HiddResultT* result);
typedef void (WINAPI* HiddGetIdleRateT) (HiddIdleRateT* idleRate, HiddResultT* result);
typedef void (WINAPI* HiddSetIdleRateT) (HiddIdleRateT idleRate, HiddResultT* result);


typedef struct
{
    HiddT                   hiddReadyCb;
    HiddT                   hiddUnregisterCb;
	BluetoothStatusChange   hiddBluetoothStatusCb;
    HIDStatusChangeT        hiddStatusCb;
    HiddHostControlT        hiddHostControlCb;
    HiddOutputReportT       hiddOutputReportCb;
    HiddGetReportT          hiddGetReportCb;
    HiddSetReportT          hiddSetReportCb;
    /* Extended API */
    HiddGetProtocolT        hiddGetProtocolCb;
    HiddSetProtocolT        hiddSetProtocolCb;
    HiddGetIdleRateT        hiddGetIdleRateCb;
    HiddSetIdleRateT        hiddSetIdleRateCb;
} HiddRegisterExT;

DWORD BLUETOOTH_HID_DEVICE_API HiddRegisterEx(HiddServiceRecordLenT serviceRecordLen, 
                                              HiddServiceRecordT serviceRecord,
                                              HiddRegisterT* registerCallbacks);


DWORD BLUETOOTH_HID_DEVICE_API HiddRegister(HiddDeviceTypeT deviceType, HiddRegisterT*  registerCallbacks);
DWORD BLUETOOTH_HID_DEVICE_API HiddUnregister();
DWORD BLUETOOTH_HID_DEVICE_API HiddSendReport(HiddReportLenT reportLen, HiddReportT report);
DWORD BLUETOOTH_HID_DEVICE_API HiddPostReport(HiddReportLenT reportLen, HiddReportT report);
DWORD BLUETOOTH_HID_DEVICE_API HiddSetHost(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_HID_DEVICE_API HiddGetHost(BT_ADDR* bluetoothAddress);
DWORD BLUETOOTH_HID_DEVICE_API HiddUnPlug();
DWORD BLUETOOTH_HID_DEVICE_API HiddGetMouseCoo(CsrUint8* X,CsrUint8* Y,CsrUint8* scroll);
#ifdef __cplusplus
}
#endif

#endif
