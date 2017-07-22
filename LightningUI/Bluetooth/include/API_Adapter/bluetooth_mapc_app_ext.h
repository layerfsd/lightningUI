/******************************************************************************************************

                CSR plc 2014

                All rights reserved

FILE:           bluetooth_mapc_app_ext.h

DESCRIPTION:    Header for the extended definition of Bluetooth MAPC

REVISION:       $Revision: #1 $

*******************************************************************************************************/

#ifndef _BLUETOOTH_MAPC_APP_EXT_H
#define _BLUETOOTH_MAPC_APP_EXT_H

#ifdef __cplusplus
extern "C" {
#endif


/* +++ Bluetooth MAPC App Ext +++ */
//messagelist APIs
typedef enum
{
    COMPLETE    = 0x00,
    FRACTIONED  = 0x01,
    NOTIFATION  = 0x02,
} MapMesReceptionStatus;

typedef enum
{
    YES = 0x00,
    NO  = 0x01,
} MapMesStatus;

typedef struct MessageListDataRef
{
    struct MessageListDataRef                *next;

    /* Standard message list parameters */
    LPCTSTR                 handle;     
    LPCTSTR                 subject;
    LPCTSTR                 datetime;
    LPCTSTR                 sender_name;
    LPCTSTR                 sender_addressing;
    LPCTSTR                 replyto_addressing;
    LPCTSTR                 recipient_name;
    LPCTSTR                 recipient_addressing;

    DWORD                   size;
    DWORD                   attachment_size;
    bool                    text;
    MapMesTypes             type;
    MapMesReceptionStatus   reception_status;
    bool                    priority;
    bool                    read;
    bool                    sent;
    bool                    is_protected;
} MessageListData;

//message detail APIs

typedef enum
{
    VCARD_2_1       = 0x00,
    VCARD_3_0       = 0x01,
} MpacVCardVersion;

typedef struct MapcVCardRef
{
    struct MapcVCardRef* next;

    MpacVCardVersion version;       //2.1 or 3.0
    TCHAR name[MAX_PATH];
    TCHAR fName[MAX_PATH];
    TCHAR email[MAX_PATH];
    TCHAR tel[MAX_PATH];

    MapcVCardRef()
    {
        version = VCARD_2_1;
        memset(name,0,MAX_PATH);
        memset(fName,0,MAX_PATH);
        memset(email,0,MAX_PATH);
        memset(tel,0,MAX_PATH);
        next = NULL;
    };
} MapcVCard;
  
typedef struct
{
    int                     version;
    bool                    status;     
    MapMesTypes             type;
    TCHAR                   folderName[30];
    MapcVCard*              originator;
    MapcVCard*              recipient;
    int                     partID;
    int                     length;
    TCHAR                   encoding[20];
    TCHAR                   charset[20];
    TCHAR                   language[20];
    CString                 content;
} MessageDetailData;
    
typedef struct
{
    TCHAR type[80];
    TCHAR handle[80];
    TCHAR folder[80];
    MapMesTypes msg_type;
} Notification;
/* --- Bluetooth MAPC App Ext --- */

#ifdef __cplusplus
}
#endif

#endif
