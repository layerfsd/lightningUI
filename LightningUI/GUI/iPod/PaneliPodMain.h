#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../Controls/SmartBarMedia.h"
#include "iPodCommon.h"


class CPaneliPodMain :
	public CPanelGL
{
public:
	CPaneliPodMain(void);
	~CPaneliPodMain(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);
	void OnGesture(GESTUREINFO *pgi);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar){m_pSmartBarMedia = pSmartBar;}


	void PlayInfoFlyOut(UINT idType);
	void PlayInfoFlyIn();

	//notify:
	void SetPlayState(BOOL bPlaying);
	void SetPlayMode(UINT idPlayMode);

	int GetProgressValue(){return m_sliProgress.GetValue();}
	void SetProgressValue(int iSet){if(!m_sliProgress.IsActived())m_sliProgress.SetValue(iSet);}
	void SetRemainTime(int iSet);
	void SetElapsedTime(int iSet);
	void SetTitle(const wchar_t *pTitle);
	void SetArtist(const wchar_t *pArtist);
	void SetAlbum(const wchar_t *pAlbum);
	void SetNowPlayingTitle(const wchar_t *pNowPlayingTitle);

	void SetArtWorkStatus(BOOL bHas){m_bHasAlbumArt = bHas;}
	void UpdateWidget();

	void SetConnectStatus(BOOL bConnect){m_bConnected = bConnect;}

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:

	CSmartBarMedia	*m_pSmartBarMedia;

	BOOL				m_bConnected;
	UINT					m_idPlayMode;
	CLabelGL			m_iconiPod;

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

	CButtonGL		m_btnPlayModeShuffle;
	CButtonGL		m_btnPlayModeRepeatOne;
	CButtonGL		m_btnPlayModeRepeatAll;


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
};

