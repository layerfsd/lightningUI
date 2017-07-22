
/******************************************************************************************************

                CSR plc 2011
                
                All rights reserved

FILE:           Api_Avrcp.h

DESCRIPTION:    Header for WPP AVRCP(CT) application

REVISION:       $Revision: #5 $
        
*******************************************************************************************************/
#pragma once


#ifndef _AVRCP_PLAYER_H
#define _AVRCP_PLAYER_H

#include <csync.h>
#include "bluetooth_avrcp.h"

//C do not support template, so ,write it outside extern "C"{}
template<typename T>
struct CAvrcpMediaContentList
{
	T *m_pHead;
	T *m_pTail;
	DWORD m_dwCntList;

	CAvrcpMediaContentList():m_pHead(NULL),m_pTail(NULL),m_dwCntList(0)
	{
		HEAP_STATISTICS_CLASS_ALLOC(CAvrcpMediaContentList)
	}

	~CAvrcpMediaContentList()
	{
		HEAP_STATISTICS_CLASS_FREE(CAvrcpMediaContentList)
		Free();
	}
	
	//NOTE:default use the parameter pointer directly, don't free it in caller
	T *Add(T *pc);
	BOOL Remove(T *pc, BOOL bDelete = TRUE);
	void Free();
};


template<typename T> T* CAvrcpMediaContentList<T>::Add(T *pc)
{
	if(pc == NULL)
	{
		RETAILMSG(1,(_T("<%s>,Error! Can't add a NULL item \r\n"),_T(__FUNCTION__)));
		return NULL;
	}

	if(m_pHead == NULL)
	{
		m_pHead = pc;
		m_pTail = pc;
		m_pTail->pNext = NULL;
	}
	else
	{
		m_pTail->pNext = pc;
		m_pTail = pc;
		m_pTail->pNext = NULL;
	}
	
	m_dwCntList++;
	return pc;
	
}



template<typename T> BOOL CAvrcpMediaContentList<T>::Remove(T *pc, BOOL bDelete)
{
	//RETAILMSG(1,(_T("<%s>, \r\n"),_T(__FUNCTION__)));
	return FALSE;
}
template<typename T> void CAvrcpMediaContentList<T>::Free()
{
	//RETAILMSG(1,(_T("<%s>, \r\n"),_T(__FUNCTION__)));
	T *pTmp = m_pHead;
	while(pTmp != NULL)
	{
		m_pHead = m_pHead->pNext;
		delete pTmp;
		pTmp = m_pHead;
	}
	m_pHead = NULL;
	m_pTail = NULL;
	m_dwCntList = 0;
}


static BOOL StaticIsUidSame(CsrBtAvrcpUid uid1,CsrBtAvrcpUid uid2)
{
	for(CsrUint8 i=0 ;i< CSR_BT_AVRCP_UID_SIZE; i++)
	{
		if(uid1[i] != uid2[i])
			return FALSE;
	}
	return TRUE;
}



#ifdef __cplusplus
extern "C" {
#endif


#define AVRCP_API_WAIT_TIMEOUT					(2 * 1000)

#define RESET_EVETN(x) { \
	 RETAILMSG(DEBUG_AVRCP_OUTPUT, (_T("<%s>: Reset Event\r\n"), TEXT(__FUNCTION__))); \
	 ResetEvent(x);}

#define WAIT_SIGNAL(x,y)  { \
	WaitForSingleObject(x,y); \
	RETAILMSG(DEBUG_AVRCP_OUTPUT, (_T("<%s>: Recieved Event\r\n"), TEXT(__FUNCTION__)));}


// Macros:----------------------------------------------------------------------------
#undef  COUNTOF
#define COUNTOF(a) (sizeof(a) / sizeof((a)[0]))

//#define DEBUG_AVRCP

#ifdef DEBUG_AVRCP
#define DEBUG_AVRCP_INIT                        1
#define DEBUG_AVRCP_OUTPUT                      1
#define DEBUG_AVRCP_MESSAGE                     1
#define DEBUG_AVRCP_DUMP                        0
#define DEBUG_AVRCP_FUNCTION					1
#define DEBUG_AVRCP_ERROR                       1
#else
#define DEBUG_AVRCP_INIT                        0
#define DEBUG_AVRCP_OUTPUT                      0
#define DEBUG_AVRCP_MESSAGE                     0
#define DEBUG_AVRCP_DUMP                        0
#define DEBUG_AVRCP_FUNCTION					0
#define DEBUG_AVRCP_ERROR                       1
#endif

#define CSR_BLUETOOTH_MUTE_SECOND_PATH         TEXT("SOFTWARE\\CSR\\Applications\\a2dp")
#define CSR_BLUETOOTH_AVRCP_SET_PATH           TEXT("SOFTWARE\\CSR\\Applications\\avrcp")
#define CSR_BLUETOOTH_AVRCP_SET_BROWSE_MODE    TEXT("browse_mode")
#define CSR_BLUETOOTH_AVRCP_SET_BROWSE_NUM     TEXT("browse_num")
#define CSR_BLUETOOTH_AVRCP_SET_BROWSE_NPL_MAX_NUM     TEXT("browse_npl_num")


#define CSR_BLUETOOTH_MS                TEXT("MuteInterval")
#define DEFAULT_MUTE_INTERVAL           1000
#define MAX_MUTE_INTERVAL           3000

#define NOW_PLAY_LIST_MAX_NUM       (0xffff)

//#define AVRCP_PLAYER_MSG_QUEUE_NAME				_T("/btp/avrcp_player/msg")
//#define AVRCP_TG_VERSION_EVENT_NAME				_T("/btp/avrcp_player/srVersion")

/*
*	We use different strategy to get folder&media item in virtual file system. See. GetMoreItem(CsrUint32 countAtLeast)
*   STRATEGY_FIX:  We always try to get a fixed num(STRATEGY_FIX_COUNT) of item when we call GetMediaPlayerFileSys;
*   STRATEGY_AUTO: We try to get as many as possible when we call GetMediaPlayerFileSys;
*/
#define AVRCP_GET_MEDIA_FILE_SYSTEM_STRATEGY_FIX			0x2
#define AVRCP_GET_MEDIA_FILE_SYSTEM_STRATEGY_AUTO			0x1
#define AVRCP_GET_MEDIA_FILE_SYSTEM_STRATEGY_FIX_COUNT		5
#define AVRCP_GET_MEDIA_FILE_SYSTEM_STRATEGY				AVRCP_GET_MEDIA_FILE_SYSTEM_STRATEGY_FIX

/*
*	When getting now playing list, we need to call api GetNowPlayingFolderItem for several times until the callback
*	return no more item. we try to get AVRCP_GET_NOW_PLAYING_FOLDER_ITEM_FIX_COUNT items every time, which means that
*   we call function GetNowPlayingFolderItem(x,x+AVRCP_GET_NOW_PLAYING_FOLDER_ITEM_FIX_COUNT)
*/
#define AVRCP_GET_NOW_PLAYING_FOLDER_ITEM_FIX_COUNT			5

#define AVRCP_GET_SEARCH_FOLDER_ITEM_FIX_COUNT				5

#define AVRCP_ENABLE_GET_MEDIA_ITEM_ATTRIBUTE

//#define ENABLE_AVRCP_FORCE_GOTO_ROOT_DIR_FOR_THE_FIRST_TIME_BROWSING

typedef enum
{
	EnumAvrcpPlayPosition,
	EnumAvrcpSoneLength,
	EnumAvrcpPlayState,
	EnumAvrcpState,
	EnumAvrcpSongName,
	EnumAvrcpShuffle,
	EnumAvrcpRepeat,
	EnumAvrcpSrVersion
}AvrcpMsgTypeT;

typedef struct
{
	DWORD songLenInMs;
	DWORD songPosInMs;
}AvrcpPlayTimeT;

typedef struct 
{
	AvrcpMsgTypeT type;

	union
	{
		DWORD                        songPosInMs;
		DWORD                        songLenInMs;
		AvcmPlayStateT               playState;
		AvcmConnectionStatusT        status;
		AvcmPlayAppSettingAttValIdT              attrVal;
		TCHAR*                       szSongName;
//		AvcmTgVersionT               srVersion;
	}payload;	
}AvrcpMsgDataT;

typedef struct PasAttrValRef
{
	AvcmPlayAppSettingAttValPairT        attrPair;
	BOOL                      bValid;
}PasAttrValNodeT;


typedef struct
{
	AvcmEventIdT				eventId;
	AvcmNotifyValT				eventVal;
}PlayingEventT;

typedef struct MPContentDataInDataPool
{
	MPContentDataInDataPool()
	{
		RETAILMSG(DEBUG_AVRCP_FUNCTION,(TEXT("<%s>,\r\n"),_T(__FUNCTION__)));
		pNext = NULL;
		playerId = 0;
		bPlay = TRUE;
		ZeroMemory(mask,sizeof(mask));
		nameLen = 0;
		playerName = NULL;
	}
	~MPContentDataInDataPool()
	{
		RETAILMSG(DEBUG_AVRCP_FUNCTION,(TEXT("<%s>,~\r\n"),_T(__FUNCTION__)));
		if(playerName!=NULL)
			free(playerName);
	}
    struct MPContentDataInDataPool*		pNext;
    CsrUint16							playerId;
    CsrBool								bPlay;
    AvrcpMPFeatureMsk					mask;
    CsrUint16							nameLen;
	static const CsrUint16				dMaxPlayerNameLen=256;
    TCHAR*								playerName;
} AvrcpMediaPlayerContentData;


typedef struct FolderItemContentDataInDataPool
{
	FolderItemContentDataInDataPool()
	{
		RETAILMSG(DEBUG_AVRCP_FUNCTION,(TEXT("<%s>,\r\n"),_T(__FUNCTION__)));
		pNext = NULL;
		ZeroMemory(folderUid,sizeof(folderUid));
		folderType = 0;
		bPlayable = FALSE;
		nameLen = 0;
		folderName = NULL;
	}
	~FolderItemContentDataInDataPool()
	{
		RETAILMSG(DEBUG_AVRCP_FUNCTION,(TEXT("<%s>,~\r\n"),_T(__FUNCTION__)));
		if(folderName!=NULL)
			free(folderName);
	}
	struct FolderItemContentDataInDataPool* pNext;
    CsrBtAvrcpUid               folderUid;
    CsrUint8                    folderType;
    CsrBool                     bPlayable;
    CsrUint16                   nameLen;
	static const CsrUint16		dMaxFolderNameLen=256;
    TCHAR*						folderName;
} AvrcpFolderItemContentData;


typedef struct MediaItemContentDataInDataPool
{
	MediaItemContentDataInDataPool()
	{
		RETAILMSG(DEBUG_AVRCP_FUNCTION,(TEXT("<%s>,\r\n"),_T(__FUNCTION__)));
		pNext = NULL;
		ZeroMemory(mediaUid,sizeof(mediaUid));;
		mediaType = 0;
		nameLen = 0;
		mediaName = NULL;
		artistName = NULL;
		albumName = NULL;
	}
	~MediaItemContentDataInDataPool()
	{
		RETAILMSG(DEBUG_AVRCP_FUNCTION,(TEXT("<%s>,~\r\n"),_T(__FUNCTION__)));
		if(mediaName != NULL)
			free(mediaName);
		if(artistName != NULL)
			free(artistName);
		if(albumName != NULL)
			free(albumName);
	}

    struct MediaItemContentDataInDataPool*  pNext;
    CsrBtAvrcpUid               mediaUid;
    CsrUint8                    mediaType;
    CsrUint16                   nameLen;
	static const CsrUint16		dMaxMediaNameLen=256;
    TCHAR*						mediaName; 
	TCHAR*						artistName;
	TCHAR*						albumName;
} AvrcpMediaItemContentData;

typedef struct
{
	DWORD index;		//The index of the item in MusicList's list view
	BOOL bMediaItem;	//Whether the member is a media item
	BOOL bFolderItem;	//Whether the member is a folder item
	void *p;
} AvrcpMediaAttrQueueMemberT;


//
//
//#define WPP_REG                                 _T("Software\\CSR\\Applications\\WPPAPP")
//#define WPP_LIB_NAME                            _T("wpplib")
//#define WPP_BTP_UI_EXT_DLL_NAME                 _T("btp_ui_ext.dll")
//
//#define DEBUG_AVRCP


#define AVRCP_EVENT                             _T("/btp/avrcp_ct/status")	
#define AVRCP_PLAY_STATE_EVENT                  _T("/btp/avrcp_ct/play_state")	

#define AVRCP_GET_MEDIA_PLAYER_LIST_CB_EVENT	_T("AvrcpGetMediaPlayerListCbEvent")
#define AVRCP_GET_MEDIA_FILE_SYS_CB_EVENT		_T("AvrcpGetMediaFileSysCbEvent")
#define AVRCP_CHANGE_PATH_CB_EVENT				_T("AvrcpChangePathCbEvent")
#define AVRCP_ADD_2_NOW_PLAYING_CB_EVENT		_T("AvrcpAdd2NowPlayingCbEvent")
#define AVRCP_PLAY_ITEM_CB_EVENT				_T("AvrcpPlayItemCbEvent")
#define AVRCP_MEDIA_ITEM_CB_EVENT				_T("AvrcpMediaItemCbEvent")

#define AVRCP_STATUS(dw)                        ((AvcmConnectionStatusT)(LOWORD(dw)))
#define AVRCP_STATUS_EVENT_DATA(s, r)           MAKELONG(s, r)

#define AVRCP_PLAY_STATE(dw)                    ((AvcmPlayStateT)(LOWORD(dw)))
#define AVRCP_PLAY_STATE_EVENT_DATA(s)          MAKELONG(s, 0)


#define AVRCP_PLAYER_MIN_VOLUME_RANGE           0
#define AVRCP_PLAYER_MAX_VOLUME_RANGE           100

#define MIN_WAVEOUT_VOLUME                      ((WORD)0x0)
#define MAX_WAVEOUT_VOLUME                      ((WORD)0xFFFF)
#define WAVEOUT_LEFT_CHANNEL_VOLUME(dw)         (WORD)(LOWORD(dw))
#define WAVEOUT_RIGHT_CHANNEL_VOLUME(dw)        (WORD)(HIWORD(dw))
#define WAVEOUT_VOLUME(l, r)                    (DWORD)(MAKELONG(l, r))

#define AVRCP_INVALID_SONG_POS                  ((DWORD) -1)
#define AVRCP_INVALID_SONG_LEN                  ((DWORD) -1)


/* Function pointer type for AVRCP */
typedef DWORD (WINAPI* AvrcpRegisterT) (AvcmRegisterInfoT* registerInfo);
typedef DWORD (WINAPI* AvrcpUnregisterT) ();
typedef DWORD (WINAPI* AvrcpGetPlayStateT) ();
typedef DWORD (WINAPI* AvcmPlayTimeT) ();
typedef DWORD (WINAPI* AvrcpConnectT) (BT_ADDR bluetoothAddress);
typedef DWORD (WINAPI* AvrcpDisconnectT) ();

/* Issue PASS THROUGH/UNIT INFO/SUBUNIT INFO command*/
typedef DWORD (WINAPI* AvrcpIssuePTCommandT) (AvcmPTOpIdT opId, AvcmPTStateT state);
typedef DWORD (WINAPI* AvrcpGetUnitInfoT) (unsigned char *data, unsigned short dataLen);
typedef DWORD (WINAPI* AvrcpGetSubUnitInfoT) (unsigned char *data, unsigned short dataLen);

/* Common operations relied on PASS-THROUGH command */
typedef DWORD (WINAPI* AvrcpPlayT) (BOOL fKeyPressed);
typedef DWORD (WINAPI* AvrcpPauseT) (BOOL fKeyPressed);
typedef DWORD (WINAPI* AvrcpStopT) (BOOL fKeyPressed);
typedef DWORD (WINAPI* AvrcpNextT) (BOOL fKeyPressed);
typedef DWORD (WINAPI* AvrcpPreviousT) (BOOL fKeyPressed);

/* Common operations based on VENDOR-DEPENDENT command */
typedef DWORD (WINAPI* AvrcpGetPASCapsT) ();
typedef DWORD (WINAPI* AvrcpGetCurrentPAST) (unsigned char num, AvcmPlayAppSettingAttIdT *attrIds);
typedef DWORD (WINAPI* AvrcpSetCurrentPAST) (unsigned char num, AvcmPlayAppSettingAttValPairT* attrs);
typedef DWORD (WINAPI* AvrcpGetMediaInfoT) (AvcmInfoMaskT mask);

/* Interface for avrcp1.4 */
typedef DWORD (WINAPI* AvrcpGetMediaPlayerListT) (CsrUint16 startOffset, CsrUint16 endOffset);
typedef DWORD (WINAPI* AvrcpGetMediaPlayerFileSysT) (CsrUint16 startOffset, CsrUint16 endOfset);
typedef DWORD (WINAPI* AvrcpGetSearchFolderItemT) (CsrUint16 startOffset, CsrUint16 endOffset);
typedef DWORD (WINAPI* AvrcpGetNowPlayingFolderItemT) (CsrUint16 startOffset, CsrUint16 endOffset);
typedef DWORD (WINAPI* AvrcpChangePathT) (CsrBool bDown, CsrBtAvrcpUid uid);
typedef DWORD (WINAPI* AvrcpSearchT) (CsrUint16 stringLen, CsrUint8* searchString);
typedef DWORD (WINAPI* AvrcpSetAddressedPlayerT) (CsrUint16 playerId);
typedef DWORD (WINAPI* AvrcpSetBrowsedPlayerT) (CsrUint16 playerId);
typedef DWORD (WINAPI* AvrcpAdd2NowPlayingT) (CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);
typedef DWORD (WINAPI* AvrcpPlayItemT) (CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);
typedef DWORD (WINAPI* AvrcpGetItemAttributeT)(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid, AvcmInfoMaskT mask);


/* AVRCP(CT) */
class CAvrcpPlayer
{
public:
    CAvrcpPlayer();
    /* Define the virtual de-constructor to free SfWE SDK. Otherwise memory leak occurs. */
    virtual ~CAvrcpPlayer();

    BOOL Initialize(
		AvcmCbConnectionStatusT avcmStatusCb, 
		AvcmCbPlayStateT avcmPlayStateCb,
		AvcmCbNotifyT avcmNotifyCb,
		AvcmCbMediaInfoT avcmMediaInfoCb,
		AvcmCbPlayTimeT avcmPlayTimeCb,
		AvcmCbPassThroughCmdResponseT avcmPTCmdResCb,
		AvcmCbSubUnitCmdResponseT avcmSubUnitCmdResCb,
		AvcmCbUnitCmdResponseT avcmUnitCmdResCb,
		AvcmCbGetCurPlayAppSettingT avcmGetCurPasCb,
		AvcmCbSetCurPlayAppSettingT avcmSetCurPasCb,
		AvcmCbGetPlayAppSettingCapsT avcmGetPasCapsCb,
//		AvcmCbGetPlayAppSettingCapsT avcmInformCharSetCb,
		DWORD dwMinVolumeRange, 
		DWORD dwMaxVolumeRange);
	BOOL Initialize(AvcmTypeT avcmType, DWORD dwMinVolumeRange, DWORD dwMaxVolumeRange);
    
    void DeInitialize();

	/* Pseudo function for AVRCP API */
	DWORD Register(AvcmRegisterInfoT* registerInfo);
	DWORD Unregister();
	DWORD GetPlayState();
	DWORD GetPlayTime();
	DWORD Connect(BT_ADDR bluetoothAddress);
	DWORD Disconnect();

	/* Issue PASS THROUGH/UNIT INFO/SUBUNIT INFO command*/
	DWORD IssuePTCommand(AvcmPTOpIdT opId, AvcmPTStateT state);
	DWORD GetUnitInfo(unsigned char *data, unsigned short dataLen);
	DWORD GetSubUnitInfo(unsigned char *data, unsigned short dataLen);
    
    /* Common operations relied on PASS-THROUGH command */
	DWORD Play(BOOL fKeyPressed = TRUE);
	DWORD Pause(BOOL fKeyPressed = TRUE);
	DWORD Stop(BOOL fKeyPressed = TRUE);
	DWORD Next(BOOL fKeyPressed = TRUE);
	DWORD Previous(BOOL fKeyPressed = TRUE);

	virtual HRESULT put_Mute(BOOL fMute);
    virtual HRESULT get_Mute(BOOL *pfMute);
    virtual HRESULT put_Volume(long lVolumeRange);
    virtual HRESULT get_Volume(long *plVolumeRange);

    //static long GetMPPlayState(AvcmPlayStateT playState);

	DWORD   GetShuffle(AvcmPlayAppSettingAttValIdT &shuffleAttrVal);
	DWORD   SetRepeat(AvcmPlayAppSettingAttValIdT valId);
	DWORD   GetRepeat(AvcmPlayAppSettingAttValIdT &repeatAttrVal);
	DWORD   SetShuffle(AvcmPlayAppSettingAttValIdT valId);
	DWORD   SetAttribute(AvcmPlayAppSettingAttIdT attId, AvcmPlayAppSettingAttValIdT valId);
	DWORD   GetAttributeVal(AvcmPlayAppSettingAttIdT attId, AvcmPlayAppSettingAttValIdT &valId);

	DWORD GetPASCaps();
	DWORD GetCurrentPAS(unsigned char num = 0, AvcmPlayAppSettingAttIdT *attrIds = NULL);
	DWORD SetCurrentPAS(unsigned char num, AvcmPlayAppSettingAttValPairT* attrs);
	DWORD GetMediaInfo(AvcmInfoMaskT mask = AVCM_ITEM_ATT_MASK_ALL);

	/*Avrcp 1.4*/
	DWORD GetMediaPlayerList(CsrUint16 startOffset=0, CsrUint16 endOfset=100);//[fixme_uu] Did I set the default value right?
	DWORD GetMediaPlayerFileSys(CsrUint16 startOffset=0, CsrUint16 endOfset=100);//[fixme_uu] Did I set the default value right?
	DWORD GetSearchFolderItem(CsrUint16 startOffset=0, CsrUint16 endOffset=100);
	DWORD GetNowPlayingFolderItem(CsrUint16 startOffset=0, CsrUint16 endOffset=100);
	DWORD ChangePath(CsrBool bDown, CsrBtAvrcpUid uid);
	DWORD Search(CsrUint16 stringLen, CsrUint8* searchString);
	DWORD SetAddressedPlayer(CsrUint16 playerId);
	DWORD SetBrowsedPlayer(CsrUint16 playerId);
	DWORD Add2NowPlaying(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);
	DWORD PlayItem(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);
	DWORD GetMediaItemAttribute(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid, AvcmInfoMaskT mask);

    AvcmConnectionStatusT GetConnectionStatus();
	void SetConnectionStatus(AvcmConnectionStatusT status);
	HRESULT SetMuteInterval();

	DWORD Browse(CsrBool bDown,CsrBtAvrcpUid uid);
	DWORD GetMPFileSysFirstFewItems(CsrUint32 countFirstFew);
	DWORD GetMPFileSysMoreItems(CsrUint32 countAtLeast);

	DWORD GetNowPlayingListFirstFewItems(CsrUint32 uCountFirstFew);
	DWORD GetNowPlayingListMoreItems(CsrUint32 uCountMore);

	DWORD GetSearchFirstFewItems(CsrUint32 uCount);
	DWORD GetSearchMoreItems(CsrUint32 uCountMore);

	void   SetPlayTimeFlag(BOOL playTimeFlag);
	BOOL   GetPlayTimeFlag(void);

    DWORD browseMode;
    CsrUint16 browseModeFixedNum;
    CsrUint16 browseNowPlayingMaxNum;

	/*Callback return some usefull values.*/

	//StaticGetMediaPlayerListCb callback values
	CsrUint16 iGotMediaPlayerCount;
	CsrUint16 iGotFirstMediaPlayerId;	//If there is only one media player, this value is used to set addressedPlayer/browsedPlayer automatically in function IsMediaPlayerSelected()
	
	//StaticChangePathCb callback values
	DWORD result_StaticChangePathCb;

	//StaticChangePathCb & StaticSetBrowsedPlayerCb will change this value
	CsrUint32 numOfItemUnderCurPath;

	//StaticGetMediaFileSysCb  callback values
	CsrUint16 numOfFolderItem_StaticGetMediaFileSysCb;
	CsrUint16 numOfMediaItem_StaticGetMediaFileSysCb;
	DWORD result_StaticGetMediaFileSysCb;

	//StaticAdd2NowPlayingCb callback values
	DWORD result_StaticAdd2NowPlayingCb;

	//StaticPlayItemCb callback values
	DWORD result_StaticPlayItemCb;

	//StaticSearchCb callback values
	CsrUint32 numOfItem_StaticSearchCb;

	//StaticMediaItemCb callback values
	CsrUint16 numOfMediaItem_StaticMediaItemCb;
	DWORD result_StaticMediaItemCb;

	//the total numOfItemGetCurrently
	CsrUint32 numOfFolderMediaItemTotalGotCurrently;

	//the total numOfNowPlayingListItemGotCurrently
	CsrUint32 numOfNowPlayingListItemGotCurrently;

	//the total numOfSearchItemGotCurrently;
	CsrUint32 numOfSearchItemGotCurrently;
	

	//HANDLES
	HANDLE hGetMediaPlayerListCbEvent;
	HANDLE hGetMediaFileSysCbEvent;
	HANDLE hChangePathCbEvent;
	HANDLE hAdd2NowPlayingCbEvent;
	HANDLE hPlayItemCbEvent;
	HANDLE hMediaItemCbEvent;


private:


	CRITICAL_SECTION        m_actionLock;
	CRITICAL_SECTION        m_lock;

	// call back function data
	PasAttrValNodeT         m_attrValPairList[AVCM_PAS_EXT_ATT_ID_END]; 
	BOOL                    m_requestAttrVal;

	DWORD                   m_muteMillisecond; //The millisecond of mute to avoid the unexpectable noise
	struct
	{
		unsigned char  *data;
		unsigned short dataLen;
	}UnitInfo, SubUnitInfo;

	BOOL                    m_getPlayTimeFlag;

	void    ActionLock();
    void    ActionUnlock();
	void    Lock();
	void    Unlock();


	/* Convert functions */
    static LPCTSTR AvrcpStatus2String(AvcmConnectionStatusT status);

    static LPCTSTR AvrcpPlayState2String(AvcmPlayStateT playState);

	/*some function*/
	static BOOL StaticIsUidInDataPool(CString szDataName,CsrBtAvrcpUid uid);

	

	/* Call back function */
    static void StaticAvcmStatusCb(BT_ADDR btAddr, AvcmConnectionStatusT status, DWORD result);

    static void StaticAvcmPlayStateCb(AvcmPlayStateT playState);
	
    static void StaticNotifyCb(AvcmEventIdT eventId, AvcmNotifyValT eventVal);

    static void StaticMediaInfoCb(AvcmMetaDataInfoT* info, DWORD result);

    static void StaticPlayTimeCb(DWORD songLenInMs, DWORD songPosInMs, DWORD result);

    static void StaticPTCmdResCb(AvcmPTOpIdT opId, AvcmPTStateT state, DWORD result);

    static void StaticSubUnitInfoCb(unsigned char *data, unsigned short dataLen, DWORD result);

    static void StaticUnitInfoCb(unsigned char *data, unsigned short dataLen, DWORD result);

    static void StaticGetCurPasCb(unsigned char attValPairCount, AvcmPlayAppSettingAttValPairT *attValPair, DWORD result);

    static void StaticSetCurPasCb(DWORD result);

    static void StaticGetPasCapsCb(AvcmPlayAppSettingCapsT *info, DWORD result);

    static void StaticInformCharSetCb(DWORD result);

	static void StaticGetMediaPlayerListCb(CsrUint16 numOfPlayer, AvcmMediaPlayerContent *pMPList, DWORD result);

	static void StaticGetMediaFileSysCb(CsrUint16 numOfFolderItem, AvcmFolderItemContent *pFIList, CsrUint16 numOfMediaItem, AvcmMediaItemContent *pMIList, DWORD result);

	static void StaticMediaItemCb(CsrUint16 numOfMediaItem, AvcmMediaItemContent *pMIList, DWORD result);

	static void StaticChangePathCb(CsrUint32 numofItem , DWORD result);

	static void StaticSetBrowsedPlayerCb(CsrUint32 numOfItem, CsrUint16 nameLen, CsrUint8* name, DWORD result);

	static void StaticSearchCb(CsrUint32 numOfItem, CsrUint16 uidCounter, DWORD result);

	static void StaticAdd2NowPlayingCb(DWORD result);

	static void StaticPlayItemCb(DWORD result);

	static void StaticSetAddressedPlayerCb(DWORD result);

	static void StaticMediaItemAttributeCb(AvcmMetaDataInfoT* info, DWORD result);// callback of function GetMediaItemAttribute

	static void StaticAddFIMIDataInDataPool(CsrUint16 numOfFolderItem, AvcmFolderItemContent *pFIList,CsrUint16 numOfMediaItem, AvcmMediaItemContent *pMIList);//add FIdata/MIdata in dataPool

    static DWORD WINAPI InformPlayingEvent(LPVOID lpParam);//AvcmNotifyCb trigger a new thread to run this function. e.g. apTrackChanged;apPlayPosChanged

	static DWORD WINAPI InformGetPlayAppSettings(LPVOID lpParam);

	static DWORD WINAPI InformGetSearchFolderItem(LPVOID lpParam);//StaticSearchCb trigger a new thread to run this function. TODO: call GetSearchFolderItem(*)

	static DWORD WINAPI InformGotoRootDir(LPVOID lpParam);

    virtual void AvcmStatusCb(BT_ADDR btAddr, AvcmConnectionStatusT status, BYTE result);

    virtual void AvcmPlayStateCb(AvcmPlayStateT playState);

	void AvcmNotifyCb(AvcmEventIdT eventId, AvcmNotifyValT eventVal);

	void AvcmPlayTimeCb(DWORD songLenInMs, DWORD songPosInMs, DWORD result);
	void AvcmMediaInfoCb(AvcmMetaDataInfoT* info, DWORD result);
    
    virtual void UpdateAvrcpStatus(BT_ADDR btAddr,AvcmConnectionStatusT status, BYTE result);
    
    virtual void NotifyAvcmPlayState(AvcmPlayStateT playState);

    void NotifyAvcmPlayStateEvent(AvcmPlayStateT playState);

	void UpdateAvrcpPlayState(AvcmPlayStateT playState);

	void put_Volume(DWORD dwVolume);
    DWORD get_Volume();

    WORD Range2Volume(long lVolumeRange);
    long Volume2Range(WORD wVolume);

    DWORD                       m_dwVolume;
    BOOL                        m_fMute;

    /* WPP Dll handle */
    HINSTANCE                   m_hDll;


    /* Function pointer for AVRCP API */
	AvrcpRegisterT                      m_pfnAvrcpRegister;
    AvrcpUnregisterT                    m_pfnAvrcpUnregister;
    AvrcpGetPlayStateT                  m_pfnAvrcpGetPlayState;
    AvcmPlayTimeT                       m_pfnAvrcpPlayTime;
    AvrcpConnectT                       m_pfnAvrcpConnect;
    AvrcpDisconnectT                    m_pfnAvrcpDisconnect;

    AvrcpIssuePTCommandT                m_pfnAvrcpIssuePTCommand;  
    AvrcpGetUnitInfoT                   m_pfnAvrcpGetUnitInfo;  
    AvrcpGetSubUnitInfoT                m_pfnAvrcpGetSubUnitInfo;  

    AvrcpPlayT                          m_pfnAvrcpPlay;
    AvrcpPauseT                         m_pfnAvrcpPause;
    AvrcpStopT                          m_pfnAvrcpStop;   
    AvrcpNextT                          m_pfnAvrcpNext;
    AvrcpPreviousT                      m_pfnAvrcpPrevious;

    AvrcpGetPASCapsT                    m_pfnAvrcpGetPASCaps;
    AvrcpGetCurrentPAST                 m_pfnAvrcpGetCurrentPAS;
    AvrcpSetCurrentPAST                 m_pfnAvrcpSetCurrentPAS;
    AvrcpGetMediaInfoT                  m_pfnAvrcpGetMediaInfo;

	AvrcpGetMediaPlayerListT			m_pfnAvrcpGetMediaPlayerList;
	AvrcpGetMediaPlayerFileSysT			m_pfnAvrcpGetMediaPlayerFileSys;
	AvrcpGetSearchFolderItemT			m_pfnAvrcpGetSearchFolderItem;
	AvrcpGetNowPlayingFolderItemT		m_pfnAvrcpGetNowPlayingFolderItem;
	AvrcpChangePathT					m_pfnAvrcpChangePath;
	AvrcpSearchT						m_pfnAvrcpSearch;
	AvrcpSetAddressedPlayerT			m_pfnAvrcpSetAddressedPlayer;
	AvrcpSetBrowsedPlayerT				m_pfnAvrcpSetBrowsedPlayer;
	AvrcpAdd2NowPlayingT				m_pfnAvrcpAdd2NowPlaying;
	AvrcpPlayItemT						m_pfnAvrcpPlayItem;
	AvrcpGetItemAttributeT				m_pfnAvcrpGetItemAttribute;
	
	/* Upper layer call back functions */
	/* Callback for AVRCP status */
    AvcmCbConnectionStatusT     m_avcmStatusCb;
	
	/* Callback for AVRCP event (battery, system, play position) */
	AvcmCbNotifyT               m_avcmNotifyCb;

	/* Callback for AVRCP meta data information */
    AvcmCbMediaInfoT            m_avcmMediaInfoCb;

	/* Callback for AVRCP play time */
    AvcmCbPlayTimeT             m_avcmPlayTimeCb;

	/* Callback for AVRCP pass through command response */
    AvcmCbPassThroughCmdResponseT             m_avcmPTCmdCb;
	
	/* Callback for AVRCP sub unit command response */
    AvcmCbSubUnitCmdResponseT        m_avcmSubUnitCmdCb;
	
	/* Callback for AVRCP unit command response */
    AvcmCbUnitCmdResponseT           m_avcmUnitCmdCb;
	
	/* Callback for AVRCP player application setting (PAS) attribute value */
    AvcmCbGetCurPlayAppSettingT            m_avcmGetCurPasCmdCb;
	
	/* Callback for AVRCP set the player application setting list of player application setting values on the target */
    AvcmCbSetCurPlayAppSettingT            m_avcmSetCurPasCmdCb;

	/* Callback for AVRCP informs the list of character set supported by CT to TG */
//    AvcmCbGetPlayAppSettingCapsT        m_avcmInformCharSetCb;

	/* Callback for AVRCP player application setting capability */
    AvcmCbGetPlayAppSettingCapsT           m_avcmGetPasCapsCb;

    /* Callback for AVRCP play state */
    AvcmCbPlayStateT            m_avcmPlayStateCb;

	DWORD                       m_dwMinVolumeRange;
    DWORD                       m_dwMaxVolumeRange;

	HANDLE                      m_actionEvent;
	DWORD                       m_actionResult;

    /* Event for AVRCP status synchonization */
 //   HANDLE                      m_hAvrcpEvent;
 //   HANDLE                      m_hAvrcpPlayStateEvent;
    
    /* AVRCP connection status */
    AvcmConnectionStatusT       m_status;
    
    /* AVRCP play state */
    AvcmPlayStateT              m_playState;

	//AvcmEventIdT				m_eventId;
	//AvcmNotifyValT				m_eventVal;

	DWORD						m_dwSongLenInMs;
	DWORD						m_dwSongPosInMs;

	AvrcpPlayTimeT				m_avrcpPlayTime;

	PlayingEventT				m_playingEvent;


	DWORD GetMediaPlayerFileSysAndWait();
	DWORD GetSearchFolderItemAndWait();
	DWORD GetNowPlayingFolderItemAndWait();

    /* Flag to indicate whther AVRCP module is initialized */
    BOOL                        m_fInit;
};


#ifdef __cplusplus
}
#endif

#endif

