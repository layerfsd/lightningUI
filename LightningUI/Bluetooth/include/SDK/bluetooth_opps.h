#ifndef _BLUETOOTH_PUSHSERVER_H
#define _BLUETOOTH_PUSHSERVER_H

#include <ws2bth.h>
#include "bluetooth_module_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNDER_CE
typedef unsigned __int64 BT_ADDR;
#endif

#ifdef BLUETOOTH_OPPS_IMPLEMENTATION
#define BLUETOOTH_OPPS_API __declspec(dllexport)
#else
#define BLUETOOTH_OPPS_API __declspec(dllimport)
#endif

typedef enum
{
    OpsmIdle,
    OpsmConnected,
    OpsmObjectPushing,
    OpsmObjectPulling,
} OpsmStatusT;

/* OPP object type */
typedef BYTE OpsmObjectTypeT;
#define OPSM_UNKNOWN_TYPE                           (OpsmObjectTypeT) 0x00
#define OPSM_VCARD_TYPE                             (OpsmObjectTypeT) 0x01
#define OPSM_VCAL_TYPE                              (OpsmObjectTypeT) 0x03
#define OPSM_VNOTE_TYPE                             (OpsmObjectTypeT) 0x05
#define OPSM_VMESSAGE_TYPE                          (OpsmObjectTypeT) 0x06

/* Opsm Error Status */
typedef enum
{
    OpsmErrorSuccess,              /* Normal Disconnection    */
    OpsmErrorNotEnoughMemory,
    OpsmErrorInvalidData,
    OpsmErrorUnsupportMediaType,
    OpsmErrorRequestReject,
    OpsmErrorAbortByPeerDevice,
    OpsmErrorAbortByOtherFailure,  /* Other Failures          */
} OpsmResultT;

/*
indicating OPP Server connection status
*/
typedef void (WINAPI* OpsmCbStatusIndT)(LPVOID userdata, OpsmStatusT status);

/*
indicating OPP connect request from peer device
*/
typedef void (WINAPI* OpsmCbConnectIndT)(LPVOID userdata, BT_ADDR btAddr);

/*
{push}
This interface function indicates received object information.
*/
typedef void (WINAPI* OpsmCbPushIndT)(LPVOID userdata, OpsmObjectTypeT type, LPTSTR typeString, LPTSTR objectName, DWORD objectSize);

/*
{push}
This interface function indicates received packet data.
To discard the subsequence push request and object data, OpsmAbort() could be adopted.
*/
typedef void (WINAPI* OpsmCbPushPacketIndT)(LPVOID userdata, BOOL isFinalPacket, PBYTE data, DWORD dataSize);

/* {push | pull} This interface function is used for indicating the end of object transfer on the OPP server */
typedef void (WINAPI* OpsmCbAbortIndT)(LPVOID userdata, OpsmResultT errorCode);

#ifdef OPP_PULL_ENABLE
/*
{pull}
This interface function is called whenever a pull request is initiated by peer device.
*/
typedef void (WINAPI* OpsmCbPullIndT)(LPVOID userdata, OpsmObjectTypeT type, LPTSTR typeString);

/*
{pull}
This interface function is called whenever object packet data is necessary for a pull request.
To reject the present and subsequence pull request, OpsmAbort() could be adopted.
To respond object packet, OpsmPullPacket() could be adopted.
*/
typedef void (WINAPI* OpsmCbPullPacketIndT)(LPVOID userdata);
#endif

typedef struct
{
    DWORD                           size;
    LPVOID                          userdata;
    BluetoothStatusChange           opsmCbBTStatusChangeInd;
    OpsmCbConnectIndT               opsmCbConnectInd;
    OpsmCbStatusIndT                opsmCbStatusInd;
    OpsmCbPushIndT                  opsmCbPushInd;
    OpsmCbPushPacketIndT            opsmCbPushPacketInd;
    OpsmCbAbortIndT                 opsmCbAbortInd;
#ifdef OPP_PULL_ENABLE
    OpsmCbPullIndT                  opsmCbPullInd;
    OpsmCbPullPacketIndT            opsmCbPullObjectInd;
#endif
} OpsmRegisterInfoT;


DWORD BLUETOOTH_OPPS_API OpsmRegister(OpsmRegisterInfoT* registerInfo);
DWORD BLUETOOTH_OPPS_API OpsmUnregister();
DWORD BLUETOOTH_OPPS_API OpsmAcceptConnect(BOOL isAcceptRequest);
#ifdef OPP_PULL_ENABLE
DWORD BLUETOOTH_OPPS_API OpsmAcceptPull(LPTSTR objectName, DWORD objectSize);
DWORD BLUETOOTH_OPPS_API OpsmPullPacket(BOOL isFinalPacket, PBYTE data, DWORD dataSize);
#endif
DWORD BLUETOOTH_OPPS_API OpsmAbort(DWORD errorCode);
#ifdef __cplusplus
}
#endif

#endif
