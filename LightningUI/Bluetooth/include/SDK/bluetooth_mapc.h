/******************************************************************************************************

                CSR plc 2012

                All rights reserved

FILE:           bluetooth_mapc.h

DESCRIPTION:    MAPC API

REVISION:       $Revision: #1 $

*******************************************************************************************************/

#ifndef _BLUETOOTH_MAPC_H
#define _BLUETOOTH_MAPC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ws2bth.h>
#include "bluetooth_module_api.h"
#include "csr_types.h"


#ifdef BLUETOOTH_MAPC_IMPLEMENTATION
#define BLUETOOTH_MAPC_API __declspec(dllexport)
#else
#define BLUETOOTH_MAPC_API __declspec(dllimport)
#endif

typedef UINT8* MapcDataHandleT;

typedef CsrUint16  MapcmStatusT;
#define MAPCM_STATUS_IDLE                 ((MapcmStatusT) 0x0)
#define MACPM_STATUS_CONNECTING           ((MapcmStatusT) 0x1)
#define MAPCM_STATUS_CONNECTED            ((MapcmStatusT) 0x2)
#define MAPCM_STATUS_DISCONNECTING        ((MapcmStatusT) 0x3)

typedef CsrUint8 MapMesSupport;
#define MAP_NO_TYPE_SUPPORT               ((MapMesSupport) 0x00)
#define MAP_EMAIL_SUPPORT                 ((MapMesSupport) 0x01)
#define MAP_SMS_GSM_SUPPORT               ((MapMesSupport) 0x02)
#define MAP_SMS_CDMA_SUPPORT              ((MapMesSupport) 0x04)
#define MAP_MMS_SUPPORT                   ((MapMesSupport) 0x08)
#define MAP_ANY_TYPE_SUPPORT              ((MapMesSupport) 0xFF)

typedef enum 
{
    MAPCM_MESSAGE_STATUS_IND_READ    = ((CsrUint8)0x00),
    MAPCM_MESSAGE_STATUS_IND_DELETE  = ((CsrUint8)0x01),
} MAPCM_MESSAGE_STATUS_IND;

typedef enum 
{
    MAPCM_MESSAGE_STATUS_VAL_NO      = ((CsrUint8)0x00),
    MAPCM_MESSAGE_STATUS_VAL_YES     = ((CsrUint8)0x01),
} MAPCM_MESSAGE_STATUS_VAL;

typedef enum 
{
    MAPCM_MESSAGE_CHAR_SET_NATIVE    = ((CsrUint8)0x00),
    MAPCM_MESSAGE_CHAR_SET_UTF8      = ((CsrUint8)0x01),
} MAPCM_MESSAGE_CHAR_SET;


/* MAP client and server type */
typedef CsrUint32 MapMesParms;
#define MAP_PARAMETERS_ALL                                   ((MapMesParms) 0x00000000)
#define MAP_PARAMETERS_SUBJECT                               ((MapMesParms) 0x00000001)
#define MAP_PARAMETERS_DATETIME                              ((MapMesParms) 0x00000002)
#define MAP_PARAMETERS_SENDER_NAME                           ((MapMesParms) 0x00000004)
#define MAP_PARAMETERS_SENDER_ADDRESSING                     ((MapMesParms) 0x00000008)
#define MAP_PARAMETERS_RECIPIENT_NAME                        ((MapMesParms) 0x00000010)
#define MAP_PARAMETERS_RECIPIENT_ADRESSING                   ((MapMesParms) 0x00000020)
#define MAP_PARAMETERS_TYPE                                  ((MapMesParms) 0x00000040)
#define MAP_PARAMETERS_SIZE                                  ((MapMesParms) 0x00000080)
#define MAP_PARAMETERS_RECEPTION_STATUS                      ((MapMesParms) 0x00000100)
#define MAP_PARAMETERS_TEXT                                  ((MapMesParms) 0x00000200)
#define MAP_PARAMETERS_ATTACHMENT_SIZE                       ((MapMesParms) 0x00000400)
#define MAP_PARAMETERS_PRIORITY                              ((MapMesParms) 0x00000800)
#define MAP_PARAMETERS_READ                                  ((MapMesParms) 0x00001000)
#define MAP_PARAMETERS_SENT                                  ((MapMesParms) 0x00002000)
#define MAP_PARAMETERS_PROTECTED                             ((MapMesParms) 0x00004000)
#define MAP_PARAMETERS_REPLYTO_ADDRESSING                    ((MapMesParms) 0x00008000)

typedef CsrUint8 MapMesTypes;
#define MAP_TYPE_ALL                                         ((MapMesTypes) 0x00)
#define MAP_TYPE_NO_SMS_GSM                                  ((MapMesTypes) 0x01)
#define MAP_TYPE_NO_SMS_CDMA                                 ((MapMesTypes) 0x02)
#define MAP_TYPE_NO_EMAIL                                    ((MapMesTypes) 0x04)
#define MAP_TYPE_NO_MSS                                      ((MapMesTypes) 0x08)
#define MAP_TYPE_NO_MMS                                      ((MapMesTypes) 0x08) /* MAP_TYPE_NO_MSS should have been called MAP_TYPE_NO_MMS */

typedef CsrUint8 MapReadStatus;
#define MAP_STATUS_NO_FILTERING                              ((MapReadStatus) 0x00)
#define MAP_STATUS_UNREAD                                    ((MapReadStatus) 0x01)
#define MAP_STATUS_READ                                      ((MapReadStatus) 0x02)

typedef CsrUint8 MapPriority;
#define MAP_PRIORITY_NO_FILTERING                            ((MapPriority) 0x00)
#define MAP_PRIORITY_HIGH                                    ((MapPriority) 0x01)
#define MAP_PRIORITY_NON_HIGH                                ((MapPriority) 0x02)

typedef struct
{
    LPCTSTR         serviceName;                    /* ServiceName from SDP record */
    CsrUint8        masInstanceId;                  /* MASInstanceID from SDP record */
    MapMesSupport   supportedMessages;              /* bit pattern of supported message types in this MAS instance */
} MapcmMasInstance;

typedef struct
{
    MapMesParms     parameterMask;                  /* bitmask of relevant parameters for the message listing. NB: a bit value of 1 means that the parameter should be present and a value of 0 means it should be filtered out */
    MapMesTypes     filterMessageType;              /* bitmask specifying which message types to include/exclude in the listing. NB: a bit value of 1 means that the message type should be filtered and a value of 0 means that it should be present */
    LPCTSTR         filterPeriodBegin;              /* null terminated time string */
    LPCTSTR         filterPeriodEnd;                /* null terminated time string */
    MapReadStatus   filterReadStatus;               /* bitmask specifying if filtering should be done on behalf of the read status */
    LPCTSTR         filterRecipient;                /* null terminated recipient string */
    LPCTSTR         filterOriginator;               /* null terminated originator string */
    MapPriority     filterPriority;                 /* bitmask specifying which priority type to include in the listing */
} MAPCM_LIST_MESSAGE_FILTER;

/*
This API is both for cancel connect and abort previous request
userdata here is the Application handle
result is the result code of the operation
*/
//typedef void (WINAPI* MapcmOperationIndT) (LPVOID userdata, MapcDataHandleT dataHandle, DWORD dataSize);
typedef void (WINAPI* MapcmOperationT) (LPVOID userdata, DWORD result, MapcDataHandleT dataHandle, DWORD dataSize, BOOL finalFlag);

typedef void (WINAPI* MapcmCbConnectCompleteT) (LPVOID userdata, DWORD result, LPCTSTR serviceName, CsrUint8 masInstanceId, MapMesSupport supportedMessages, CsrUint16 obexPeerMaxPacketSize);
typedef void (WINAPI* MapcmCbCancelCompleteT) (LPVOID userdata, DWORD result);
typedef MapcmOperationT MapcmGetFolderListT;
typedef MapcmOperationT MapcmGetMessageListT;
typedef MapcmOperationT MapcmGetMessageT;
typedef void (WINAPI* MapcmPushMessageCompleteT) (LPVOID userdata, DWORD result, LPCTSTR messageHandle);
typedef void (WINAPI* MapcmUpdateInboxCompleteT) (LPVOID userdata, DWORD result);
typedef void (WINAPI* MapcmNotificationRegisterStatusT) (LPVOID userdata, DWORD result, BOOL enableNotifications);
typedef void (WINAPI* MapcmCbNotificationHandlerT) (LPVOID userdata, BOOL isAbort, BOOL finalFlag, DWORD dataSize, MapcDataHandleT dataHandle);
typedef void (WINAPI* MapcmStatusIndT) (LPVOID userdata, MapcmStatusT status);
typedef void (WINAPI* MapcmCbGetMasInstanceListT) (LPVOID userdata, DWORD result, DWORD numOfMasInstances, MapcmMasInstance* list);
typedef void (WINAPI* MapcmSelectMapcInstanceIdCompleteT) (LPVOID userdata, DWORD result);
typedef void (WINAPI* MapcmCbPushMessageContinueIndT) (LPVOID userdata, DWORD result, CsrUint16 maxAllowedPayloadSize);
typedef void (WINAPI* MapcmNotificationRegisterOffIndT) (LPVOID userdata);

typedef struct  
{
    DWORD                                   size;
    LPVOID                                  userdata;
    BluetoothStatusChange                   bluetoothStatusChangeCb;
    MapcmGetFolderListT                     mapcmCbGetFolderList;
    MapcmGetMessageListT                    mapcmCbGetMessageList;
    MapcmGetMessageT                        mapcmCbGetMessage;
    MapcmCbCancelCompleteT                  mapcmCbCancelComplete;
    MapcmCbConnectCompleteT                 mapcmCbConnectComplete;
    MapcmCbNotificationHandlerT             mapcmCbNotificationHandler;
    MapcmPushMessageCompleteT               mapcmCbPushMessageComplete;
    MapcmUpdateInboxCompleteT               mapcmCbUpdateInboxComplete;
    MapcmNotificationRegisterStatusT        mapcmCbNotificationRegisterStatus;
    MapcmNotificationRegisterOffIndT        mapcmCbNotificationRegisterOffInd;
    MapcmStatusIndT                         mapcmCbStatusInd;
    MapcmCbGetMasInstanceListT              mapcmCbGetMasInstanceList;
    MapcmSelectMapcInstanceIdCompleteT      mapcmCbSelectMapcInstanceIdComplete;
    MapcmCbPushMessageContinueIndT          mapcmCbPushMessageContinueInd;
} MapcmRegisterInfo;


DWORD BLUETOOTH_MAPC_API MapcmRegister(MapcmRegisterInfo* info);
DWORD BLUETOOTH_MAPC_API MapcmUnRegister(VOID);
DWORD BLUETOOTH_MAPC_API MapcmConnect(BT_ADDR bluetoothAddress, CsrUint8 masInstanceId);
DWORD BLUETOOTH_MAPC_API MapcmDisconnect();
DWORD BLUETOOTH_MAPC_API MapcmGetFolderList(LPCTSTR folderName, UINT16 startOffset, UINT16 maxListCount);
DWORD BLUETOOTH_MAPC_API MapcmGetMessageList(LPCTSTR folderName, UINT16 startOffset, UINT16 maxListCount, MAPCM_LIST_MESSAGE_FILTER* filter);
DWORD BLUETOOTH_MAPC_API MapcmGetMessage(LPCTSTR folderName, LPCTSTR messageHandle, MAPCM_MESSAGE_CHAR_SET charSet, BOOL attachment);
DWORD BLUETOOTH_MAPC_API MapcmSetMessageStatus(LPCTSTR folderName, LPCTSTR messageHandle, MAPCM_MESSAGE_STATUS_IND indicator,  MAPCM_MESSAGE_STATUS_VAL value);
DWORD BLUETOOTH_MAPC_API MapcmPushMessage(LPCTSTR folderName, UINT16 length, MAPCM_MESSAGE_CHAR_SET charSet, BOOL transparent);
DWORD BLUETOOTH_MAPC_API MapcmUpdateInbox();
DWORD BLUETOOTH_MAPC_API MapcmCancel();
DWORD BLUETOOTH_MAPC_API MapcmRegisterNotificationService(BOOL enableNotifications);
DWORD BLUETOOTH_MAPC_API MapcmGetMasInstanceList(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_MAPC_API MapcmSelectMapcInstanceId(CsrUint16 instanceId);
DWORD BLUETOOTH_MAPC_API MapcmPushMessageContinue(UINT8* message, UINT16 length, BOOL finalFlag);

//folderlist APIs
typedef struct
{
    TCHAR* folderName;
    TCHAR* folderModified;
} MapcFolderItem;

typedef struct
{
    CsrUint16 numOfItems;
    MapcFolderItem* list;
} MapcFolderList;


#ifdef __cplusplus
}
#endif

#endif
