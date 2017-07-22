/****************************************************************************

                CSR plc 2012
                    
                All rights reserved

FILE:           Api_Avrcp.h

DESCRIPTION:    Header for Bluetooth AVRCP(CT) API

REVISION:       $Revision: #1 $

****************************************************************************/
#ifndef AVRCP_API_H
#define AVRCP_API_H


#include "api_base.h"
#include "avrcp_player.h"


class CApi_Avrcp : public CApi_base
{
public:
    CApi_Avrcp();
    virtual ~CApi_Avrcp();

    ULONG AddRef();
    ULONG Release();

    BOOL Initialize(DWORD dwMinVolumeRange = AVRCP_PLAYER_MIN_VOLUME_RANGE, DWORD dwMaxVolumeRange = AVRCP_PLAYER_MAX_VOLUME_RANGE);
    void DeInitialize();


	DWORD Connect(BT_ADDR bluetoothAddress);
	DWORD Disconnect();

    HRESULT GetCurrentAvrcpPlayState();

    HRESULT Play();
    HRESULT Play(BOOL fKeyPressed);

    HRESULT Stop();
    HRESULT Stop(BOOL fKeyPressed);

    HRESULT Pause();
    HRESULT Pause(BOOL fKeyPressed);

    HRESULT Next();
    HRESULT Next(BOOL fKeyPressed);

    HRESULT Previous();
    HRESULT Previous(BOOL fKeyPressed);

    HRESULT Mute();
    HRESULT put_Mute(VARIANT_BOOL fMute = VARIANT_TRUE);
    HRESULT get_Mute(VARIANT_BOOL *pMute);

    HRESULT put_Volume(long lVolumeRange);
    HRESULT get_Volume(long *plVolumeRange);

    HRESULT get_HasMultipleItems(VARIANT_BOOL *pHasMultipleItems);
    HRESULT get_CanScan(VARIANT_BOOL *pCanScan);

    HRESULT OpenFile(LPCTSTR szFilename);
    HRESULT Close();
	
	DWORD   GetShuffle(AvcmPlayAppSettingAttValIdT &shuffleAttrVal);
	DWORD   GetRepeat(AvcmPlayAppSettingAttValIdT &repeatAttrVal);

    HRESULT Repeat(AvcmPlayAppSettingAttValIdT attrVal);
	
   	HRESULT Shuffle(AvcmPlayAppSettingAttValIdT &attrVal);
	//HRESULT GetShuffleState(AvcmPlayAppSettingAttValIdT &shuffleAttrVal);

	HRESULT GetPlayTime();

	HRESULT GetMediaInfo();
	
	HRESULT GetPAS();

	HRESULT GetPASCaps();

	HRESULT GetMediaPlayerList(CsrUint16 startOffset=0, CsrUint16 endOffset=100);

	HRESULT ChangePath(CsrBool bDown, CsrBtAvrcpUid uid);

	HRESULT Search(CsrUint16 stringLen, CsrUint8* searchString);

	HRESULT SetAddressedPlayer(CsrUint16 playerId);

	HRESULT SetBrowsedPlayer(CsrUint16 playerId);

	HRESULT Add2NowPlaying(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);

	HRESULT PlayItem(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);

	HRESULT GetMediaItemAttribute(CsrBtAvrcpScope scope, CsrBtAvrcpUid uid,AvcmInfoMaskT mask = AVCM_ITEM_ATT_MASK_ALL);
	
	HRESULT BrowseDown(CsrBtAvrcpUid uid);
	HRESULT BrowseUp(CsrBtAvrcpUid uid);
	HRESULT GetFirstFewItemsFromCurPath(CsrUint32 countFirstFew);
	HRESULT GetMoreMPFileSysItemFromCurPath(CsrUint32 countAtLeast);

	HRESULT GetNowPlayingListFirstFewItems(CsrUint32 uCountFirstFew);
	HRESULT GetNowPlayingListMoreItems(CsrUint32 uCountMore);

	HRESULT GetSearchMoreItems(CsrUint32 uCountMore);
	
    bool IsRepeating();

    /* Override CApi_base */
    WORD GetConnectionStatus();
	void SetConnectionStatus(AvcmConnectionStatusT status);
	
    DWORD WaitConnected(DWORD timeout = INFINITE);
    void NotifyConnected(BOOL isConnected = TRUE);

    BOOL IsConnected();

private:
    ULONG m_ulRef;
    BOOL m_clearA2dpStreaming;
	HANDLE m_hConnectionEvent;
};


#endif
