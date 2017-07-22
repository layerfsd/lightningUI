/******************************************************************************************************

                CSR plc 2014

                All rights reserved

FILE:           bluetooth_spp.h

DESCRIPTION:    SPP(DevA & DevB) API

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_SPP_H
#define _BLUETOOTH_SPP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ws2bth.h>

#ifndef UNDER_CE
typedef unsigned __int64 BT_ADDR;
#endif

#ifdef BLUETOOTH_SPP_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_SPP_API extern
#else
#define BLUETOOTH_SPP_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_SPP_API __declspec(dllimport)
#endif

#define BLUETOOTH_SPP_MAX_CONNECTION                    3
#define BT_SPP0     0
#define BT_SPP1     1
#define BT_SPP2     2

#define BLUETOOTH_SPP_EVENT                             TEXT("Bluetooth/SPP/Event")
#define BLUETOOTH_SPP_EVENT_FORMAT                      BLUETOOTH_SPP_EVENT TEXT("%u")

#define SPP_REG_KEY                                     CSR_BLUETOOTH_DRIVER_SPP
#define SPP_REG_BLUETOOTH_ADDRESS                       CSR_BLUETOOTH_ADDRESS
#define SPP_REG_MAX_ACTIVATE_TIME                       TEXT("MaxActivateTime")
#define SPP_REG_ROLE                                    TEXT("Role")
#define SPP_REG_SERVICE_RECORD                          TEXT("ServiceRecord")
#define SPP_REG_SERVER_CHANNEL_INDEX                    TEXT("ServerChannelIndex")
#define SPP_REG_LOCAL_SERVICE_NAME                      TEXT("LocalServiceName")
#define SPP_REG_SELECTED_SERVICE_NAME                   TEXT("SelectedServiceName")
#define SPP_REG_REMOTE_SERVICE_NAME                     TEXT("RemoteServiceName")
#define SPP_REG_STATE                                   TEXT("State")
#define SPP_REG_CONNECT_MODE                            TEXT("ConnectMode")
#define SPP_REG_PROFILE_UUID16                          TEXT("ProfileUuid16")
#define SPP_REG_PROFILE_UUID128                         TEXT("ProfileUuid128")

#define SPP_STANDARD_PROFILE_UUID                       ((WORD) 0x1101)

typedef unsigned short SppConnectModeT;
#define SppConnectStandard                              ((SppConnectModeT) 0x0)
#define SppConnectUuid16                                ((SppConnectModeT) 0x1)
#define SppConnectUuid128                               ((SppConnectModeT) 0x2)

/* SPP Connection State */
typedef unsigned short  SppStateT;
#define SppStateIdle                                    ((SppStateT) 0x0)
#define SppStateConnecting                              ((SppStateT) 0x1)
#define SppStateConnected                               ((SppStateT) 0x2)
#define SppStateDisconnecting                           ((SppStateT) 0x3)
#define SppStateTransfer                                ((SppStateT) 0x4)
#define SppStateTransferCompleted                       ((SppStateT) 0x5)
#define SppStateTransferFail                            ((SppStateT) 0x6)

/* SPP Result Code */
typedef unsigned short  SppResultCode;
#define SPP_RESULT_SUCCESS                              ((SppResultCode) (0x0000))
#define SPP_RESULT_UNACCEPTABLE_PARAMETER               ((SppResultCode) (0x0001))
#define SPP_RESULT_COMMAND_DISALLOWED                   ((SppResultCode) (0x0002))
#define SPP_RESULT_MAX_NUM_OF_CONNECTIONS               ((SppResultCode) (0x0003))
#define SPP_RESULT_ALREADY_CONNECTING                   ((SppResultCode) (0x0004))
#define SPP_RESULT_SEARCH_FAILED                        ((SppResultCode) (0x0005))
#define SPP_RESULT_NOTHING_TO_CANCEL                    ((SppResultCode) (0x0006))
#define SPP_RESULT_CANCELLED_CONNECT_ATTEMPT            ((SppResultCode) (0x0007))
#define SPP_RESULT_TIMEOUT                              ((SppResultCode) (0x0008))

/* SPP Role */
typedef unsigned char  SppRoleT;
#define SPP_SERVER_ROLE                                 ((SppRoleT) (1))        /* DCE */
#define SPP_CLIENT_ROLE                                 ((SppRoleT) (2))        /* DTE */

/* SPP Service Name */
typedef TCHAR SppServiceName[50 + 1];

#define SPP_SERVICE_NAME_CHAR_COUNT                     (sizeof(SppServiceName) / sizeof(TCHAR))

#define SPP_STATE_VALUE(dw)                             ((SppStateT) dw)

typedef void (WINAPI* SppCbStateT) (DWORD connectionIndex, BT_ADDR bluetoothAddress, SppStateT state, DWORD result);

typedef struct
{
    BYTE*  serviceRecord;
    DWORD  serviceRecordSize;
    DWORD  serverChannelIndex;
} SppActivateExtInfo;

typedef struct
{
    DWORD               size;
    DWORD               connectionIndex;
    SppCbStateT         sppStateCallback;
    SppActivateExtInfo  sppActivateExtInfo;
} SppRegisterInfo;

typedef struct
{
    BT_ADDR         bluetoothAddress;
    SppServiceName  selectedServiceName;    /* Remote SPP service name selected, while empty for the 1st service. */
    SppConnectModeT connectMode;
    WORD            profileUuid16;          /* Valid for "SppConnectUuid16". */
    UUID            profileUuid128;         /* Valid for "SppConnectUuid128". */
} SppConnectInfo;

typedef DWORD (WINAPI *SppInitT)(HANDLE *sppHandle, SppRegisterInfo *registerInfo);
typedef DWORD (WINAPI *SppDeinitT)(HANDLE sppHandle);
typedef DWORD (WINAPI *SppConnectT)(HANDLE sppHandle, SppConnectInfo *connectInfo);
typedef DWORD (WINAPI *SppDisconnectT)(HANDLE sppHandle);
typedef DWORD (WINAPI *SppGetLocalServiceNameT)(HANDLE sppHandle, SppServiceName serviceName);


DWORD BLUETOOTH_SPP_API SppInit(HANDLE* sppHandle, SppRegisterInfo* registerInfo);
DWORD BLUETOOTH_SPP_API SppDeinit(HANDLE sppHandle);
DWORD BLUETOOTH_SPP_API SppConnect(HANDLE sppHandle, SppConnectInfo* connectInfo);
DWORD BLUETOOTH_SPP_API SppDisconnect(HANDLE sppHandle);
DWORD BLUETOOTH_SPP_API SppGetLocalServiceName(HANDLE sppHandle, SppServiceName serviceName);


#ifdef __cplusplus
}
#endif

#endif
