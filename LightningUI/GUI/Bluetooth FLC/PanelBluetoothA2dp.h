#pragma once
#include "BluetoothCommon.h"

#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../Controls/SmartBarMedia.h"


class CPanelBluetoothA2dp :
	public CPanelGL
{
public:
	CPanelBluetoothA2dp(void);
	~CPanelBluetoothA2dp(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);
	void OnGesture(GESTUREINFO *pgi);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void SetProgressValue(int iSet){if(!m_sliProgress.IsActived())m_sliProgress.SetValue(iSet);}
	void SetRemainTime(int iSet);
	void SetElapsedTime(int iSet);

	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar){m_pSmartBarMedia = pSmartBar;}

	//
	void OnA2dpConnected(BOOL bConnected);
	void OnAvrcpConnected(BOOL bConnected);


	void PlayInfoFlyOut(UINT idType);
	void PlayInfoFlyIn();

	//notify:
	void OnNotifyID3(strMusicID3 *id3info);
	void SetPlayState(PLAYER_STATE state);

	void SetShowType(BOOL bFirstTimeConnect);
	static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
	


private:

	BOOL m_bA2dpConnected;
	BOOL m_bAvrcpConnected;

	CSmartBarMedia	*m_pSmartBarMedia;


	CLabelGL			m_iconBT;

	CLabelGL			m_iconTrack;
	CLabelGL			m_iconAlbum;
	CLabelGL			m_iconArtist;
// 	CLabelGL			m_iconGenre;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

	CLabelGL			m_tagBackEq;

	CButtonGL		m_btnLibrary;

	CButtonGL		m_btnPrev;
	CButtonGL		m_btnNext;

	CButtonGL		m_btnPlay;
	CButtonGL		m_btnPause;

	CButtonGL		m_btnHome;

// 	CLabelGL			m_iconRepeat;
// 	CLabelGL			m_iconShuffle;
// 	CLabelGL			m_iconRepeat1;

// 	CButtonGL		m_btnPlayModeShuffle;
// 	CButtonGL		m_btnPlayModeRepeatOne;
// 	CButtonGL		m_btnPlayModeRepeatAll;




	CSliderGL  m_sliProgress;
// 	CM_String		m_strTitle;
// 	CM_String		m_strArtist;
// 	CM_String		m_strAlbum;

	CButtonGL	m_tagTitle;
	CButtonGL	m_tagArtist;
	CButtonGL	m_tagAlbum;
	CButtonGL	m_tagNowPlaying;

	CButtonGL	m_tagElapsedTime;
	CButtonGL	m_tagRemainTime;

	int			m_iElapsedTime;
	int			m_iRemainTime;


	CButtonGL			m_tagAlbumArt;
	BOOL			m_bPlayingFlag;
	BOOL			m_bAutoResumeFlag;
	DWORD		m_dwEnterTick;

// 	CShapedViewGL	m_tagAlbumArtRound;
	CLabelGL			m_tagAlbumBk;
	CLabelGL			m_tagVinyl;


	unsigned char *m_pBuffAlbumArt;
	DWORD m_dwAlbumArtSize;
	BOOL		m_bNeedUpdateAlbumArt;
	BOOL		m_bHasAlbumArt;

	BOOL		m_bPlayInfoFlyOut;
	UINT			m_idPlayInfoFlyOutType;
	BOOL       m_bFirstTimeConnect;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CButtonGL	m_tagVolMaxNote;
	BOOL       m_bShowVolMaxNote;
	BOOL       m_bVolMaxNoteShowed;
#endif

};

