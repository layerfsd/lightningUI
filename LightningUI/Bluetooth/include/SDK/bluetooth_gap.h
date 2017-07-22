/******************************************************************************************************

                CSR plc 2014

                All rights reserved

FILE:           bluetooth_gap.h

DESCRIPTION:    Bluetooth GAP(GenericAccessProfile) API

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_GAP_H
#define _BLUETOOTH_GAP_H


#include <ws2bth.h>


#ifdef __cplusplus
extern "C" {
#endif


#ifdef BLUETOOTH_GAP_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_GAP_API extern
#else
#define BLUETOOTH_GAP_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_GAP_API __declspec(dllimport)
#endif


#define BT_MAX_FRIENDLY_NAME_LEN            50

#define BT_LINK_KEY_SIZE                    0x10

typedef BYTE     BluetoothLinkKey[BT_LINK_KEY_SIZE];

/* Bluetooth device name in the string of UTF8. */
typedef TCHAR    BluetoothDeviceName[BT_MAX_FRIENDLY_NAME_LEN + 1];

typedef struct
{
    BT_ADDR             deviceAddr;
    BOOL                linkkeyValid;
    BYTE                linkkeyLen;
    BluetoothLinkKey    Linkkey;
    BluetoothDeviceName remoteName;
    DWORD               reserved;
} BluetoothDeviceRecord;

/* Gap callback mode. */
typedef unsigned int    GapCallbackModeT;
#define GAP_CALLBACK_MODE_DIRECT           ((GapCallbackModeT) 0x0)
#define GAP_CALLBACK_MODE_MESSAGE          ((GapCallbackModeT) 0x1)

typedef void (WINAPI* GapCbCommonCallbackT)(DWORD result);

typedef struct
{
    GapCallbackModeT               mode;
} GapRegisterInfoT;

DWORD BLUETOOTH_GAP_API GapRegister(GapRegisterInfoT* registerInfo);
DWORD BLUETOOTH_GAP_API GapUnregister();
DWORD BLUETOOTH_GAP_API GapSetLocalName(BluetoothDeviceName deviceName);
DWORD BLUETOOTH_GAP_API GapEnableUnderTestMode();
DWORD BLUETOOTH_GAP_API GapGetDeviceRecord(BT_ADDR deviceAddr, BluetoothDeviceRecord* deviceRecord);
DWORD BLUETOOTH_GAP_API GapEnumDeviceRecord(BluetoothDeviceRecord* deviceRecordList, DWORD recordCount, DWORD* actualCount);


#ifdef __cplusplus
}
#endif

#endif
