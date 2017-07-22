#pragma once
#include "../../Communication/CommanderEx.h"
#include "MediaCommon.h"

#include "../Controls/SmartBarMedia.h"

class CPanelMediaMain :
	public CPanelGL
{
public:
	CPanelMediaMain(void);
	~CPanelMediaMain(void);

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
	void SetTitle(const wchar_t *pTitle);

	void SetArtist(const wchar_t *pArtist);

	void SetAlbum(const wchar_t *pAlbum);
	
	void SetNowPlayingTitle(const wchar_t *pNowPlayingTitle);

// 	void UpdatePlayStatus(BOOL bPlaying);
	void ShowAlert(BOOL bShow){m_bShowAlert = bShow;}
	void SetVideoMode(BOOL bVideo);

	void OnPrepareUpdateAlbumArt(const unsigned char *pBuff,DWORD dwSize);

	void SetRepeatCheck(BOOL bRepeat);
	void SetShuffleCheck(BOOL bShuffle);
	void SetPlayMode(UINT idPlayMode);

	void SetMediaSrcType(UINT idType);

	void SetPlayingStatus(BOOL bPlaying);

	void SetBrowsingStatus(BOOL bBrowsing);
	void UpdateAlbumArt();
	void SetLoadingStatus(FILE_LOADING_STATUS status);

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);
	void SetVideoFullScreen(BOOL bFullScreen);

	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar){m_pSmartBarMedia = pSmartBar;}

	//
	void ShowLibraryBtn(BOOL bShow);

	void EnableOperate(BOOL bEnable);

	void PlayInfoFlyOut(UINT idType);
	void PlayInfoFlyIn();

	void SetFileLoadFlag(BOOL bLoaded);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:

private:
	FILE_LOADING_STATUS		m_idLoadingStatus;
	DWORD			m_dwEnterTick;

	BOOL				m_bBrowsing;
	UINT					m_idCurMediaType;

	CSmartBarMedia	*m_pSmartBarMedia;

	CLabelGL			m_iconUSB;
	CLabelGL			m_iconSD;
// 	CLabelGL			m_iconBT;

// 	CLabelGL			m_iconVideo;
// 	CLabelGL			m_iconAudio;

	CLabelGL			m_iconTrack;
	CLabelGL			m_iconAlbum;
	CLabelGL			m_iconArtist;
// 	CLabelGL			m_iconGenre;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

	CLabelGL			m_tagBackEq;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL				m_btnLibrary;
#else
	CButtonGL				m_btnLibraryVideo;
	CButtonGL				m_btnLibraryMusic;
#endif

	CButtonGL		m_btnPrev;
	CButtonGL		m_btnNext;

	CButtonGL		m_btnPlay;
	CButtonGL		m_btnPause;

	CButtonGL		m_btnHome;

// 	CLabelGL			m_iconRepeat;
// 	CLabelGL			m_iconShuffle;
// 	CLabelGL			m_iconRepeat1;

	CButtonGL		m_btnPlayModeShuffle;
	CButtonGL		m_btnPlayModeRepeatOne;
	CButtonGL		m_btnPlayModeRepeatAll;

	CLabelGL			m_iconLoading;

	CLabelGL		m_iconAlert;
	BOOL				m_bShowAlert;

// 	CButtonGL		m_btnUSB1;
// 	CButtonGL		m_btnUSB2;
// 	CButtonGL		m_btnSD;
// 	CButtonGL		m_btniPod;
// 	CButtonGL		m_btnBT;
// 	CButtonGL		m_btnDisk;

// 	GLfloat				m_fSrcButtonPosY;
// 	GLfloat				m_fSrcButtonPosX;

// 	BOOL				m_bSrcValid[16];
// 	INT					m_iSrcValidCount;

// 	CButtonGL		m_tagVideoWindow;

	CSliderGL  m_sliProgress;
// 	CM_String		m_strTitle;
// 	CM_String		m_strArtist;
// 	CM_String		m_strAlbum;

	CButtonGL	m_tagMediaStatus;

	CButtonGL	m_tagTitle;
	CButtonGL	m_tagArtist;
	CButtonGL	m_tagAlbum;
	CButtonGL	m_tagNowPlaying;

	CButtonGL	m_tagElapsedTime;
	CButtonGL	m_tagRemainTime;

	int			m_iElapsedTime;
	int			m_iRemainTime;

// 	CLabelGL			m_tagSeperator1;
// 	CLabelGL			m_tagSeperator2;

	CButtonGL			m_tagAlbumArt;
	BOOL				m_bVideoMode;
	BOOL			m_bPlayingFlag;
	BOOL			m_bAutoResumeFlag;

	BOOL			m_bUpdateParamFlag;

	BOOL				m_bVideoFullScreen;

// 	CShapedViewGL	m_tagAlbumArtRound;
	CLabelGL			m_tagAlbumBk;
	CLabelGL			m_tagVinyl;

// 	CButtonGL			m_tagVideoBk;

	unsigned char *m_pBuffAlbumArt;
	DWORD m_dwAlbumArtSize;
	BOOL		m_bNeedUpdateAlbumArt;
	BOOL		m_bHasAlbumArt;

	//
	BOOL		m_bEnableOperate;

	BOOL		m_bPlayInfoFlyOut;
	UINT			m_idPlayInfoFlyOutType;

	BOOL		m_bFileLoaded;

	CM_String		m_title;
	CM_String		m_artist;
	CM_String		m_album;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	BOOL		m_bWarningPageShowed;
	DWORD		m_dwNoValidFileTick;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	BOOL					m_bBtnVinylClicked;
	POINT					m_ptDownVinyl;
	POINT					m_ptLastVinyl;
#endif
};

