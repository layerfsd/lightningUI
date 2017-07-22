//CSmartBarMedia.h : interface of the CSmartBarMedia class
//

#pragma once
#include "SmartBar.h"

#define SB_RES_BACKGROUND					55
#define SB_RES_BAR									56
#define SB_RES_SYMBOS							60

#define SB_RES_ICON_BT									121
#define SB_RES_ICON_RADIO							123
#define SB_RES_ICON_USB								125
#define SB_RES_ICON_SD									124
#define SB_RES_ICON_IPOD								126
#define SB_RES_ICON_CARPLAY						127

#define SB_RES_BTN_PLAY_UP							113
#define SB_RES_BTN_PLAY_DOWN					118
#define SB_RES_BTN_PAUSE_UP						112
#define SB_RES_BTN_PAUSE_DOWN					117
#define SB_RES_BTN_PRE_UP							115
#define SB_RES_BTN_PRE_DOWN						120
#define SB_RES_BTN_NEXT_UP							114
#define SB_RES_BTN_NEXT_DOWN					119


//
#define SB_MEDIATYPE_NULL					0
#define SB_MEDIATYPE_USB						1
#define SB_MEDIATYPE_SD						2
#define SB_MEDIATYPE_A2DP					3
#define SB_MEDIATYPE_RADIO					4
#define SB_MEDIATYPE_IPOD					5
#define SB_MEDIATYPE_CARPLAY				6

class CSmartBarMedia :
	public CSmartBar
{
public:
	CSmartBarMedia();
public:
	virtual ~CSmartBarMedia();

private:

	LPDIRECTDRAWSURFACE         m_pDDSBackground; //
	LPDIRECTDRAWSURFACE         m_pDDSBar; //

	LPDIRECTDRAWSURFACE         m_pDDSSymbos; //

	LPDIRECTDRAWSURFACE         m_pDDSIconUSB; //
	LPDIRECTDRAWSURFACE         m_pDDSIconSD; //
	LPDIRECTDRAWSURFACE         m_pDDSIconBT;//
	LPDIRECTDRAWSURFACE         m_pDDSIconRadio;//
	LPDIRECTDRAWSURFACE         m_pDDSIconiPod;//
	LPDIRECTDRAWSURFACE         m_pDDSIconCarPlay;//

	LPDIRECTDRAWSURFACE         m_pDDSBtnPlayUp;//
	LPDIRECTDRAWSURFACE         m_pDDSBtnPlayDown;//
	LPDIRECTDRAWSURFACE         m_pDDSBtnPauseUp;//
	LPDIRECTDRAWSURFACE         m_pDDSBtnPauseDown;//
	LPDIRECTDRAWSURFACE         m_pDDSBtnPreUp;//
	LPDIRECTDRAWSURFACE         m_pDDSBtnPreDown;//
	LPDIRECTDRAWSURFACE         m_pDDSBtnNextUp;//
	LPDIRECTDRAWSURFACE         m_pDDSBtnNextDown;//

	SIZE	m_siIcon;
	SIZE	m_siButton;

	BOOL m_bBtnPlayClicked;
	BOOL m_bBtnPreClicked;
	BOOL m_bBtnNextClicked;

private:
	UINT		m_idType;
	CM_String		m_strNowPlayingTitle;
	CM_String		m_strTrackName;
	BOOL			m_bPlayingFlag;

public:
	BOOL Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns);
	void SetType(UINT idType){m_idType=idType;m_bNeedUpdateFrame=TRUE;}
	void SetNowPlayingTitle(const wchar_t*pTitle){m_strNowPlayingTitle=pTitle;m_bNeedUpdateFrame=TRUE;}
	void SetTrackName(const wchar_t*pTitle){m_strTrackName=pTitle;m_bNeedUpdateFrame=TRUE;m_dwLastShowTick=GetTickCount();}
	void SetPlayingFlag(BOOL bPlaying){m_bPlayingFlag = bPlaying;m_bNeedUpdateFrame=TRUE;}
	UINT GetType(){return m_idType;}
	BOOL GetPlayingFlag(){return m_bPlayingFlag;}

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

protected:
	void Render();
	BOOL LoadRes(LPCTSTR lpFileName);
	void   ReleaseRes();
	void DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si);

private:
	BOOL	m_bPressed;

};

