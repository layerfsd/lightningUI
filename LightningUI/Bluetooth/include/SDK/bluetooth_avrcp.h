/******************************************************************************************************

                CSR plc 2014

                All rights reserved

FILE:           bluetooth_avrcp.h

DESCRIPTION:    AVRCP(CT) API

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_AVRCP_H
#define _BLUETOOTH_AVRCP_H

#include <ws2bth.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNDER_CE
typedef unsigned __int64 BT_ADDR;
#endif

#ifdef BLUETOOTH_AVRCP_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_AVRCP_API extern
#else
#define BLUETOOTH_AVRCP_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_AVRCP_API __declspec(dllimport)
#endif

/* Invalid play time value */
#define AVCM_INVALID_TIME                       (0xffffffff)

/* Pass-through operation IDs */
typedef unsigned char AvcmPTOpIdT;
#define AVCM_PT_OP_ID_MIN                       ((AvcmPTOpIdT) 0x00)
#define AVCM_PT_OP_ID_SELECT                    ((AvcmPTOpIdT) 0x00)
#define AVCM_PT_OP_ID_OP                        ((AvcmPTOpIdT) 0x01)
#define AVCM_PT_OP_ID_DOWN                      ((AvcmPTOpIdT) 0x02)
#define AVCM_PT_OP_ID_LEFT                      ((AvcmPTOpIdT) 0x03)
#define AVCM_PT_OP_ID_RIGHT                     ((AvcmPTOpIdT) 0x04)
#define AVCM_PT_OP_ID_RIGHT_UP                  ((AvcmPTOpIdT) 0x05)
#define AVCM_PT_OP_ID_RIGHT_DOWN                ((AvcmPTOpIdT) 0x06)
#define AVCM_PT_OP_ID_LEFT_UP                   ((AvcmPTOpIdT) 0x07)
#define AVCM_PT_OP_ID_LEFT_DOWN                 ((AvcmPTOpIdT) 0x08)
#define AVCM_PT_OP_ID_ROOT_MENU                 ((AvcmPTOpIdT) 0x09)
#define AVCM_PT_OP_ID_SETUP_MENU                ((AvcmPTOpIdT) 0x0A)
#define AVCM_PT_OP_ID_CONTENTS_MENU             ((AvcmPTOpIdT) 0x0B)
#define AVCM_PT_OP_ID_FAVOURITE_MENU            ((AvcmPTOpIdT) 0x0C)
#define AVCM_PT_OP_ID_EXIT                      ((AvcmPTOpIdT) 0x0D)
#define AVCM_PT_OP_ID_0                         ((AvcmPTOpIdT) 0x20)
#define AVCM_PT_OP_ID_1                         ((AvcmPTOpIdT) 0x21)
#define AVCM_PT_OP_ID_2                         ((AvcmPTOpIdT) 0x22)
#define AVCM_PT_OP_ID_3                         ((AvcmPTOpIdT) 0x23)
#define AVCM_PT_OP_ID_4                         ((AvcmPTOpIdT) 0x24)
#define AVCM_PT_OP_ID_5                         ((AvcmPTOpIdT) 0x25)
#define AVCM_PT_OP_ID_6                         ((AvcmPTOpIdT) 0x26)
#define AVCM_PT_OP_ID_7                         ((AvcmPTOpIdT) 0x27)
#define AVCM_PT_OP_ID_8                         ((AvcmPTOpIdT) 0x28)
#define AVCM_PT_OP_ID_9                         ((AvcmPTOpIdT) 0x29)
#define AVCM_PT_OP_ID_DOT                       ((AvcmPTOpIdT) 0x2A)
#define AVCM_PT_OP_ID_ENTER                     ((AvcmPTOpIdT) 0x2B)
#define AVCM_PT_OP_ID_CLEAR                     ((AvcmPTOpIdT) 0x2C)
#define AVCM_PT_OP_ID_CHANNEL_UP                ((AvcmPTOpIdT) 0x30)
#define AVCM_PT_OP_ID_CHANNEL_DOWN              ((AvcmPTOpIdT) 0x31)
#define AVCM_PT_OP_ID_PREV_CHANNEL              ((AvcmPTOpIdT) 0x32)
#define AVCM_PT_OP_ID_SOUND_SEL                 ((AvcmPTOpIdT) 0x33)
#define AVCM_PT_OP_ID_INPUT_SEL                 ((AvcmPTOpIdT) 0x34)
#define AVCM_PT_OP_ID_DISP_INFO                 ((AvcmPTOpIdT) 0x35)
#define AVCM_PT_OP_ID_HELP                      ((AvcmPTOpIdT) 0x36)
#define AVCM_PT_OP_ID_PAGE_UP                   ((AvcmPTOpIdT) 0x37)
#define AVCM_PT_OP_ID_PAGE_DOWN                 ((AvcmPTOpIdT) 0x38)
#define AVCM_PT_OP_ID_POWER                     ((AvcmPTOpIdT) 0x40)
#define AVCM_PT_OP_ID_VOLUME_UP                 ((AvcmPTOpIdT) 0x41)
#define AVCM_PT_OP_ID_VOLUME_DOWN               ((AvcmPTOpIdT) 0x42)
#define AVCM_PT_OP_ID_MUTE                      ((AvcmPTOpIdT) 0x43)
#define AVCM_PT_OP_ID_PLAY                      ((AvcmPTOpIdT) 0x44)
#define AVCM_PT_OP_ID_STOP                      ((AvcmPTOpIdT) 0x45)
#define AVCM_PT_OP_ID_PAUSE                     ((AvcmPTOpIdT) 0x46)
#define AVCM_PT_OP_ID_RECORD                    ((AvcmPTOpIdT) 0x47)
#define AVCM_PT_OP_ID_REWIND                    ((AvcmPTOpIdT) 0x48)
#define AVCM_PT_OP_ID_FFORWARD                  ((AvcmPTOpIdT) 0x49)
#define AVCM_PT_OP_ID_EJECT                     ((AvcmPTOpIdT) 0x4A)
#define AVCM_PT_OP_ID_FORWARD                   ((AvcmPTOpIdT) 0x4B)
#define AVCM_PT_OP_ID_BACKWARD                  ((AvcmPTOpIdT) 0x4C)
#define AVCM_PT_OP_ID_ANGLE                     ((AvcmPTOpIdT) 0x50)
#define AVCM_PT_OP_ID_SUBPICTURE                ((AvcmPTOpIdT) 0x51)
#define AVCM_PT_OP_ID_F1                        ((AvcmPTOpIdT) 0x71)
#define AVCM_PT_OP_ID_F2                        ((AvcmPTOpIdT) 0x72)
#define AVCM_PT_OP_ID_F3                        ((AvcmPTOpIdT) 0x73)
#define AVCM_PT_OP_ID_F4                        ((AvcmPTOpIdT) 0x74)
#define AVCM_PT_OP_ID_F5                        ((AvcmPTOpIdT) 0x75)
#define AVCM_PT_OP_ID_VENDOR_DEP                ((AvcmPTOpIdT) 0x7E)
#define AVCM_PT_OP_ID_MAX                       ((AvcmPTOpIdT) 0x7E)
#define AVCM_PT_OP_ID_GROUP_NAV_NEXT            ((AvcmPTOpIdT) 0xFE)
#define AVCM_PT_OP_ID_GROUP_NAV_PREV            ((AvcmPTOpIdT) 0xFF)

typedef unsigned char AvcmPTStateT;
#define AVCM_PT_STATE_PRESS                     ((AvcmPTStateT) 0x00)
#define AVCM_PT_STATE_RELEASE                   ((AvcmPTStateT) 0x01)
#define AVCM_PT_STATE_PRESS_RELEASE             ((AvcmPTStateT) 0x02)

/* IDs for player application settings and value */
typedef unsigned char AvcmPlayAppSettingAttIdT;
typedef unsigned char AvcmPlayAppSettingAttValIdT;

#define AVCM_PAS_EQUALIZER_ATT_ID               ((AvcmPlayAppSettingAttIdT) 0x01)
#define AVCM_PAS_EQUALIZER_VAL_OFF              ((AvcmPlayAppSettingAttValIdT) 0x01)
#define AVCM_PAS_EQUALIZER_VAL_ON               ((AvcmPlayAppSettingAttValIdT) 0x02)

#define AVCM_PAS_REPEAT_ATT_ID                  ((AvcmPlayAppSettingAttIdT) 0x02)
#define AVCM_PAS_REPEAT_VAL_OFF                 ((AvcmPlayAppSettingAttValIdT) 0x01)
#define AVCM_PAS_REPEAT_VAL_SINGLE              ((AvcmPlayAppSettingAttValIdT) 0x02)
#define AVCM_PAS_REPEAT_VAL_ALL                 ((AvcmPlayAppSettingAttValIdT) 0x03)
#define AVCM_PAS_REPEAT_VAL_GROUP               ((AvcmPlayAppSettingAttValIdT) 0x04)

#define AVCM_PAS_SHUFFLE_ATT_ID                 ((AvcmPlayAppSettingAttIdT) 0x03)
#define AVCM_PAS_SHUFFLE_VAL_OFF                ((AvcmPlayAppSettingAttValIdT) 0x01)
#define AVCM_PAS_SHUFFLE_VAL_ALL                ((AvcmPlayAppSettingAttValIdT) 0x02)
#define AVCM_PAS_SHUFFLE_VAL_GROUP              ((AvcmPlayAppSettingAttValIdT) 0x03)

#define AVCM_PAS_SCAN_ATT_ID                    ((AvcmPlayAppSettingAttIdT) 0x04)
#define AVCM_PAS_SCAN_VAL_OFF                   ((AvcmPlayAppSettingAttValIdT) 0x01)
#define AVCM_PAS_SCAN_VAL_ALL                   ((AvcmPlayAppSettingAttValIdT) 0x02)
#define AVCM_PAS_SCAN_VAL_GROUP                 ((AvcmPlayAppSettingAttValIdT) 0x03)

#define AVCM_PAS_SPECIFIED_MAX_COUNT            (4)

#define AVCM_PAS_EXT_ATT_ID_BEGIN               ((AvcmPlayAppSettingAttIdT) 0x80)
#define AVCM_PAS_EXT_ATT_ID_END                 ((AvcmPlayAppSettingAttIdT) 0xFF)

#define AVCM_PAS_ATT_ID_INVALID                 ((AvcmPlayAppSettingAttIdT) 0x00)
#define AVCM_PAS_VALUE_ID_INVALID               ((AvcmPlayAppSettingAttValIdT) 0x00)

/* AvcmEventIdT*/
typedef unsigned char AvcmEventIdT;
#define apPlayStatusChanged                     ((AvcmEventIdT) 0x0)
#define apTrackChanged                          ((AvcmEventIdT) 0x1)
#define apTrackReachedEnd                       ((AvcmEventIdT) 0x2)
#define apTrackReachedStart                     ((AvcmEventIdT) 0x3)
#define apPlayPosChanged                        ((AvcmEventIdT) 0x4)
#define apBattStatusChanged                     ((AvcmEventIdT) 0x5)
#define apSystemStatusChanged                   ((AvcmEventIdT) 0x6)
#define apPlayerSettingChanged                  ((AvcmEventIdT) 0x7)
#define nowPlayingContentChanged                ((AvcmEventIdT) 0x8)
#define availablePlayerChanged                  ((AvcmEventIdT) 0x9)
#define addressedPlayerChanged                  ((AvcmEventIdT) 0xa)
#define uidChanged                              ((AvcmEventIdT) 0xb)
#define apSRVersion                             ((AvcmEventIdT) 0xc) /* Not used */
#define apVolumeChanged                         ((AvcmEventIdT) 0xd)

/* apSRVersion */
typedef unsigned short AvcmTgVersionT;
#define AVCM_SR_VERSION_INVALID                 ((AvcmTgVersionT) (0x0000))
#define AVCM_SR_VERSION_10                      ((AvcmTgVersionT) (0x0100))
#define AVCM_SR_VERSION_13                      ((AvcmTgVersionT) (0x0103))
#define AVCM_SR_VERSION_14                      ((AvcmTgVersionT) (0x0104))

/* AvcmPlayStateT */
typedef unsigned char AvcmEventIntT;

/* apPlayStatusChanged */
typedef unsigned char AvcmPlayStateT;
#define apStopped                               ((AvcmPlayStateT) 0x0)
#define apPlaying                               ((AvcmPlayStateT) 0x1)
#define apPaused                                ((AvcmPlayStateT) 0x2)
#define apScanForward                           ((AvcmPlayStateT) 0x3)
#define apScanReverse                           ((AvcmPlayStateT) 0x4)
#define apWaiting                               ((AvcmPlayStateT) 0x5)    /* Not used */
#define apClosed                                ((AvcmPlayStateT) 0x6)    /* Not used */

/* apBattStatusChanged */
typedef unsigned char AvcmBattStateT;
#define apBattNormal                            ((AvcmBattStateT) 0x0)
#define apBattWarning                           ((AvcmBattStateT) 0x1)
#define apBattCritical                          ((AvcmBattStateT) 0x2)
#define apBattExternal                          ((AvcmBattStateT) 0x3)
#define apBattFullCharge                        ((AvcmBattStateT) 0x4)

/* apSystemStatusChanged */
typedef unsigned char AvcmSysStateT;
#define apPowerOn                               ((AvcmSysStateT) 0x0)
#define apPowerOff                              ((AvcmSysStateT) 0x1)
#define apPowerUnplugged                        ((AvcmSysStateT) 0x2)

#define CSR_BT_AVRCP_UID_SIZE          (8)
typedef CsrUint8                    CsrBtAvrcpUid[CSR_BT_AVRCP_UID_SIZE];


/* apTrackChanged */
typedef struct
{
    CsrBtAvrcpUid uid;
}
AvcmTrackUid;

/* apPlayPosChanged */
typedef DWORD AvcmPlayPosT;

/*player Id in target device*/
typedef struct
{
    CsrUint8  playerId;
    CsrUint16 uidCounter;
} AvcmPlayerUidT;

/* apVolumeChanged */
typedef unsigned char AvcmVolumeT;

typedef union AvcmNotifyValRef
{
    AvcmBattStateT batteryStatus;
    AvcmPlayPosT   playbackPos;
    AvcmSysStateT  systemStatus;
    AvcmTgVersionT srVersion;
    AvcmPlayerUidT playerUid;
    AvcmVolumeT    volume;
    AvcmTrackUid   trackUid;
} AvcmNotifyValT;

/* AvcmConnectionStatusT */
typedef unsigned short AvcmConnectionStatusT;
#define AvrcpSuccess                            ((AvcmConnectionStatusT) 0x0)
#define AvrcpNull                               ((AvcmConnectionStatusT) 0x1)
#define AvrcpIdle                               ((AvcmConnectionStatusT) 0x2)
#define AvrcpConnecting                         ((AvcmConnectionStatusT) 0x3)
#define AvrcpConnected                          ((AvcmConnectionStatusT) 0x4)
#define AvrcpDisconnecting                      ((AvcmConnectionStatusT) 0x5)

#define AvrcpBluetoothStatusOn                  ((AvcmConnectionStatusT) 0x1000)
#define AvrcpBluetoothStatusOff                 ((AvcmConnectionStatusT) 0x2000)


/* AvcmCallbackModeT: direct/message */
typedef unsigned short AvcmCallbackModeT;
/* Used in AvcmRegisterReq request. Currently only support AVCM_CM_DIRECT */
#define AVCM_CM_DIRECT                          ((AvcmCallbackModeT) 0x0)
#define AVCM_CM_MESSAGE                         ((AvcmCallbackModeT) 0x1)


/* AvcmType: ctrl/data path */
/* TODO: what the difference between AVCM_TYPE_CTRL and AVCM_TYPE_DATA */
typedef unsigned short AvcmTypeT;
#define AVCM_TYPE_CTRL                          ((AvcmTypeT) 0x0)
#define AVCM_TYPE_DATA                          ((AvcmTypeT) 0x1)

/* Media attribute IDs */
typedef DWORD AvcmMediaAttrIdT;
#define AvrcpMediaTitle                         ((AvcmMediaAttrIdT) 0x1)
#define AvrcpArtistName                         ((AvcmMediaAttrIdT) 0x2)
#define AvrcpAlbumName                          ((AvcmMediaAttrIdT) 0x3)
#define AvrcpMediaNum                           ((AvcmMediaAttrIdT) 0x4)
#define AvrcpTotalMediaNum                      ((AvcmMediaAttrIdT) 0x5)
#define AvrcpGenre                              ((AvcmMediaAttrIdT) 0x6)
#define AvrcpPlayingInMs                        ((AvcmMediaAttrIdT) 0x7)


/* Media attribute Masks */
typedef unsigned char AvcmInfoMaskT;
#define AVCM_ITEM_ATT_MASK_TITLE                ((AvcmInfoMaskT) 0x00000001) /* Mandatory */
#define AVCM_ITEM_ATT_MASK_ARTIST               ((AvcmInfoMaskT) 0x00000002)
#define AVCM_ITEM_ATT_MASK_ALBUM                ((AvcmInfoMaskT) 0x00000004)
#define AVCM_ITEM_ATT_MASK_TRACK_NUMBER         ((AvcmInfoMaskT) 0x00000008)
#define AVCM_ITEM_ATT_MASK_TOTAL_TRACK_NUMBER   ((AvcmInfoMaskT) 0x00000010)
#define AVCM_ITEM_ATT_MASK_GENRE                ((AvcmInfoMaskT) 0x00000020)
#define AVCM_ITEM_ATT_MASK_TIME                 ((AvcmInfoMaskT) 0x00000040)
#define AVCM_ITEM_ATT_MASK_ALL                  ((AvcmInfoMaskT) 0x0000007F)
#define AVCM_ITEM_ATT_MASK_NONE                 ((AvcmInfoMaskT) 0xFFFFFFFF)

/* Media data information*/
typedef struct
{
    AvcmInfoMaskT   metaDataBitMask;
    DWORD           trackNumber;
    DWORD           totalTrackNumber;
    DWORD           playingTime;
    TCHAR*          title;  /* null-terminated string */
    TCHAR*          artist; /* null-terminated string */
    TCHAR*          album;  /* null-terminated string */
    TCHAR*          genre;  /* null-terminated string */
} AvcmMetaDataInfoT;

/* Player application setting capablity*/
typedef struct AvcmPlayAppSettingCapsTypeRef
{
    AvcmPlayAppSettingAttIdT        attrId;
    unsigned char                   valIdCount;
    AvcmPlayAppSettingAttValIdT*    valList;
} AvcmPlayAppSettingCapsTypeT;

typedef struct
{
    AvcmPlayAppSettingCapsTypeT*    list;
    unsigned char                   attIdCount;
} AvcmPlayAppSettingCapsT;

/* player application setting */
typedef struct AvcmPlayAppSettingAttValPairRef
{
    AvcmPlayAppSettingAttIdT             attrId;
    AvcmPlayAppSettingAttValIdT          valueId;
} AvcmPlayAppSettingAttValPairT;

/* Response to AVRCP PASS THROUGH command */
typedef struct
{
    unsigned char state;
    DWORD         result;
} AvcmPTCmdResT;

#define MP_FEATURE_MASK_SIZE    16
typedef CsrUint8            AvrcpMPFeatureMsk[MP_FEATURE_MASK_SIZE];

typedef struct MPContent
{
    struct MPContent*   pNext;
    CsrUint16           playerId;
    CsrBool             bPlay;
    AvrcpMPFeatureMsk   mask;
    CsrUint16           nameLen;
    CsrUint8*           playerName;
} AvcmMediaPlayerContent;



typedef enum
{
    MIXED,
    TITLE,
    ALBUM,
    ARTIST,
    GENRES,
    PLAYLIST,
    YEARS
} FolderType;

typedef enum
{
    AUDIO,
    VEDIO
} MediaType;

typedef enum
{
    MEDIA_PLAYER_ITEM = 1,
    FOLDER_ITEM,
    MEDIA_ELEMENT_ITEM
} BrowseableItem;

typedef struct FOLDER_ITEM_CONTENT
{
    struct FOLDER_ITEM_CONTENT* pNext;
    CsrBtAvrcpUid               folderUid;
    CsrUint8                    folderType;
    CsrBool                     bPlayable;
    CsrUint16                   nameLen;
    CsrUint8*                   folderName;
} AvcmFolderItemContent;

typedef struct MEDIA_ITEM_CONTENT
{
    struct MEDIA_ITEM_CONTENT*  pNext;
    CsrBtAvrcpUid               mediaUid;
    CsrUint8                    mediaType;
    CsrUint16                   nameLen;
    CsrUint8*                   mediaName;
} AvcmMediaItemContent;

typedef CsrUint8 CsrBtAvrcpScope;

/* Call back function definition*/
typedef void (WINAPI* AvcmCbConnectionStatusT)(BT_ADDR btAddr, AvcmConnectionStatusT status, DWORD result);
typedef void (WINAPI* AvcmCbPlayStateT)(AvcmPlayStateT state);
typedef void (WINAPI* AvcmCbNotifyT)(AvcmEventIdT eventId, AvcmNotifyValT eventVal);
typedef void (WINAPI* AvcmCbMediaInfoT)(AvcmMetaDataInfoT* info, DWORD result);
typedef void (WINAPI* AvcmCbPlayTimeT)(DWORD songLenInMs, DWORD songPosInMs, DWORD result);
typedef void (WINAPI* AvcmCbPassThroughCmdResponseT)(AvcmPTOpIdT opId, AvcmPTStateT state, DWORD result);
typedef void (WINAPI* AvcmCbSubUnitCmdResponseT)(unsigned char* data, unsigned short dataLen, DWORD result);
typedef void (WINAPI* AvcmCbUnitCmdResponseT)(unsigned char* data, unsigned short dataLen, DWORD result);
typedef void (WINAPI* AvcmCbGetCurPlayAppSettingT)(unsigned char attValPairCount, AvcmPlayAppSettingAttValPairT* attValPair, DWORD result);
typedef void (WINAPI* AvcmCbSetCurPlayAppSettingT)(DWORD result);
typedef void (WINAPI* AvcmCbGetPlayAppSettingCapsT)(AvcmPlayAppSettingCapsT* info, DWORD result);

/* Call back function defintion for avrcp1.4 */
typedef void (WINAPI* AvcmCbGetMediaPlayerListT)(CsrUint16 numOfPlayer, AvcmMediaPlayerContent* pMPList, DWORD result);
typedef void (WINAPI* AvcmCbGetMediaPlayerFileSysT)(CsrUint16 numOfFolderItem, AvcmFolderItemContent* pFIList, CsrUint16 numOfMediaItem, AvcmMediaItemContent* pMIList, DWORD result);
typedef void (WINAPI* AvcmCbMediaItemT)(CsrUint16 numOfMediaItem, AvcmMediaItemContent* pMIList, DWORD result);
typedef void (WINAPI* AvcmCbChangePathT)(CsrUint32 numofItem , DWORD result);
typedef void (WINAPI* AvcmCbSetBrowsedPlayerResT)(CsrUint32 numOfItem, CsrUint16 nameLen, CsrUint8* name, DWORD result);
typedef void (WINAPI* AvcmCbSearchResT)(CsrUint32 numOfItem, CsrUint16 uidCounter, DWORD result);
typedef void (WINAPI* AvcmCbAdd2NowPlayingT)(DWORD result);
typedef void (WINAPI* AvcmCbPlayItemT)(DWORD result);
typedef void (WINAPI* AvcmCbSetAddressedPlayerT)(DWORD result);
typedef void (WINAPI* AvcmCbMediaItemInfoT)(AvcmMetaDataInfoT* info, DWORD result);


typedef struct
{
    AvcmTypeT                               type;
    AvcmCallbackModeT                       mode;
    AvcmCbConnectionStatusT                 avcmConnectStatusCb;                   /* Call back function of AvcmConnect()/AvcmDisconnect()                                                        */
    AvcmCbPlayStateT                        avcmPlayStateCb;                       /* Call back function of AvcmGetPlayState()                                                                    */
    AvcmCbNotifyT                           avcmNotifyCb;                          /* Call back function of event notificaiton                                                                    */
    AvcmCbMediaInfoT                        avcmMediaInfoCb;                       /* Call back function of AvcmGetMediaAttribute()                                                               */
    AvcmCbPlayTimeT                         avcmPlayTimeCb;                        /* Call back function of AvcmGetPlayState()                                                                    */
    AvcmCbPassThroughCmdResponseT           avcmPassThroughCmdResponseCb;          /* Call back function of AvcmIssuePassThroughCmd()/AvcmPlay()/AvcmPause()/AvcmStop()/AvcmNext()/AvcmPrevious() */
    AvcmCbSubUnitCmdResponseT               avcmSubUnitInfoResponseCb;             /* Call back function of AvcmGetSubUnitInfo()                                                                  */
    AvcmCbUnitCmdResponseT                  avcmUnitInfoResponseCb;                /* Call back function of AvcmGetUnitInfo()                                                                     */
    AvcmCbGetCurPlayAppSettingT             avcmGetCurPlayAppSettingCb;            /* Call back function of AvcmGetCurPlayAppSetting() or target player applicaiton setting changed               */
    AvcmCbSetCurPlayAppSettingT             avcmSetCurPlayAppSettingCb;            /* Call back function of AvcmSetCurPlayAppSetting()                                                            */
    AvcmCbGetPlayAppSettingCapsT            avcmGetPlayAppSettingCapsCb;           /* Call back function of AvcmGetPlayAppSettingCaps()                                                           */
    AvcmCbGetMediaPlayerListT               avcmGetMediaPlayerListCb;              /* Call back function of AvcmGetMediaPlayerList()                                                              */
    AvcmCbGetMediaPlayerFileSysT            avcmGetMediaPlayerFileSysCb;           /* Call back function of AvcmGetMediaPlayerFileSys()                                                           */
    AvcmCbMediaItemT                        avcmMediaItemCb;                       /* Call back function of AvcmGetSearchFolderItem() and AvcmGetNowPlayingFolderItem()                           */
    AvcmCbChangePathT                       avcmChangePathCb;                      /* Call back function of AvcmAvcmChangePath()                                                                  */
    AvcmCbSetBrowsedPlayerResT              avcmSetBrowsedPlayerCb;                /* Call back function of AvcmSetBrowsedPlayer()                                                                */
    AvcmCbSearchResT                        avcmSearchCb;                          /* Call back function of AvcmSearch()                                                                          */
    AvcmCbAdd2NowPlayingT                   avcmAdd2NowPlayingCb;                  /* Call back function of AvcmAdd2NowPlaying()                                                                  */
    AvcmCbPlayItemT                         avcmPlayItemCb;                        /* Call back function of AvcmPlayItem()                                                                        */
    AvcmCbSetAddressedPlayerT               avcmSetAddressedPlayerCb;              /* Call back function of AvcmSetAddressedPlayer()                                                              */
    AvcmCbMediaItemInfoT                    avcmMediaItemInfoCb;                   /* Call back function of AvcmGetItemAttribut()                                                                 */
} AvcmRegisterInfoT;

DWORD BLUETOOTH_AVRCP_API AvcmRegister(AvcmRegisterInfoT* registerInfo);
DWORD BLUETOOTH_AVRCP_API AvcmUnregister();
DWORD BLUETOOTH_AVRCP_API AvcmGetPlayState();
DWORD BLUETOOTH_AVRCP_API AvcmGetPlayTime();
DWORD BLUETOOTH_AVRCP_API AvcmConnect(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_AVRCP_API AvcmDisconnect();

/* Issue PASS THROUGH/UNIT INFO/SUBUNIT INFO command*/
DWORD BLUETOOTH_AVRCP_API AvcmIssuePassThroughCmd(AvcmPTOpIdT opId, AvcmPTStateT state);
DWORD BLUETOOTH_AVRCP_API AvcmGetUnitInfo(unsigned char* data, unsigned short dataLen);
DWORD BLUETOOTH_AVRCP_API AvcmGetSubUnitInfo(unsigned char* data, unsigned short dataLen);

/* Common operations relied on PASS-THROUGH command */
DWORD BLUETOOTH_AVRCP_API AvcmPlay(BOOL keyPressed);
DWORD BLUETOOTH_AVRCP_API AvcmPause(BOOL keyPressed);
DWORD BLUETOOTH_AVRCP_API AvcmStop(BOOL keyPressed);
DWORD BLUETOOTH_AVRCP_API AvcmNext(BOOL keyPressed);
DWORD BLUETOOTH_AVRCP_API AvcmPrevious(BOOL keyPressed);

/* Common operations based on VENDOR-DEPENDENT command */
DWORD BLUETOOTH_AVRCP_API AvcmGetPlayAppSettingCaps();
DWORD BLUETOOTH_AVRCP_API AvcmGetCurPlayAppSetting(unsigned char num, AvcmPlayAppSettingAttIdT* attrIds);
DWORD BLUETOOTH_AVRCP_API AvcmSetCurPlayAppSetting(unsigned char num, AvcmPlayAppSettingAttValPairT* attrs);
DWORD BLUETOOTH_AVRCP_API AvcmGetMediaInfo(AvcmInfoMaskT mask);



/* Interface for avrcp1.4 */
DWORD BLUETOOTH_AVRCP_API AvcmGetMediaPlayerList(CsrUint16 startOffset, CsrUint16 endOffset);
DWORD BLUETOOTH_AVRCP_API AvcmGetMediaPlayerFileSys(CsrUint16 startOffset, CsrUint16 endOfset);
DWORD BLUETOOTH_AVRCP_API AvcmGetSearchFolderItem(CsrUint16 startOffset, CsrUint16 endOffset);
DWORD BLUETOOTH_AVRCP_API AvcmGetNowPlayingFolderItem(CsrUint16 startOffset, CsrUint16 endOffset);
DWORD BLUETOOTH_AVRCP_API AvcmChangePath(CsrBool bDown, CsrBtAvrcpUid uid);
DWORD BLUETOOTH_AVRCP_API AvcmSearch(CsrUint16 stringLen, CsrUint8* searchString);
DWORD BLUETOOTH_AVRCP_API AvcmSetAddressedPlayer(CsrUint16 playerId);
DWORD BLUETOOTH_AVRCP_API AvcmSetBrowsedPlayer(CsrUint16 playerId);
DWORD BLUETOOTH_AVRCP_API AvcmAdd2NowPlaying(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);
DWORD BLUETOOTH_AVRCP_API AvcmPlayItem(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);
DWORD BLUETOOTH_AVRCP_API AvcmGetItemAttribut(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid, AvcmInfoMaskT mask);
DWORD BLUETOOTH_AVRCP_API AvcmSetAbsoluteVol(CsrUint8 vol);

#ifdef __cplusplus
}
#endif

#endif
