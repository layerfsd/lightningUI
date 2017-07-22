//CSmartBarVol.h : interface of the CSmartBarVol class
//

#pragma once
#include "SmartBar.h"
#include "../Settings/SettingCommon.h"

#define SB_RES_SLI_VOL_BASE				52
#define SB_RES_SLI_VOL_ACTIVE			53
#define SB_RES_ICON_MUTE				54
#define SB_RES_BACKGROUND				55
#define SB_RES_SPEECH                   56
#define SB_RES_MEDIA                    57
#define SB_RES_PHONE                    58
#define SB_RES_SYMBOS					60
#define SB_RES_EQ_CLASSIC				61
#define SB_RES_EQ_POP					62
#define SB_RES_EQ_ROCK					63
#define SB_RES_EQ_JAZZ					64
#define SB_RES_EQ_NONE					65
#define SB_RES_EQ_USER					66
#define SB_RES_EQ_CLASSIC_FOCUS			67
#define SB_RES_EQ_POP_FOCUS				68
#define SB_RES_EQ_ROCK_FOCUS			69
#define SB_RES_EQ_JAZZ_FOCUS			70
#define SB_RES_EQ_NONE_FOCUS			71
#define SB_RES_EQ_USER_FOCUS			72


typedef enum
{
    SB_VOLUME_CHANNEL_MEDIA = 0,
    SB_VOLUME_CHANNEL_SPEECH,
    SB_VOLUME_CHANNEL_PHONE
}SB_VOLUME_CHANNEL;

class CSmartBarVol :
	public CSmartBar
{
public:
	CSmartBarVol();
public:
	virtual ~CSmartBarVol();

private:

	LPDIRECTDRAWSURFACE         m_pDDSBackground ; //
	LPDIRECTDRAWSURFACE         m_pDDSSymbos ; //

	//volume:
	LPDIRECTDRAWSURFACE         m_pDDSIconMute ; //
	LPDIRECTDRAWSURFACE         m_pDDSSliBase ; //
	LPDIRECTDRAWSURFACE         m_pDDSSliActive ; //
	LPDIRECTDRAWSURFACE         m_pDDSIconMedia; //
	LPDIRECTDRAWSURFACE         m_pDDSIconSpeech ; //
	LPDIRECTDRAWSURFACE         m_pDDSIconPhone ; //

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	LPDIRECTDRAWSURFACE         m_pDDSIconEQClassic;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQPop;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQRock;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQJazz;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQUser;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQNone;

	LPDIRECTDRAWSURFACE         m_pDDSIconEQClassicFocus;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQPopFocus;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQRockFocus;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQJazzFocus;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQUserFocus;
	LPDIRECTDRAWSURFACE         m_pDDSIconEQNoneFocus;

	SIZE	m_siEQType;
	int		m_iconEQPx;
	int		m_iconEQPy;
	BOOL	m_bBtnEQClicked;
#endif

	SIZE	m_siSliThumb;
	SIZE	m_siSliBase;
	SIZE	m_siIconVolume;
    SIZE    m_siIconVolumeChannel;
	SIZE    m_siSliBackground;


	DWORD	m_dwVolumeCur;
	DWORD	m_dwVolumeMax;
	BOOL	m_bMute;
    UINT    m_iVolumeChannel;
	UINT    m_idEQType;

	int		m_iOverlayPx;
	int		m_iOverlayPy;

public:
	BOOL Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns);

	BOOL Show(BOOL bShow,UINT iTimeOut=5000);

	void SetMute(BOOL bMute);
    void SetVolume(DWORD dwVolume,UINT iVolumeChannel=SB_VOLUME_CHANNEL_MEDIA);
	BOOL IsOnMute(){return m_bMute;}
	void SetEQType(UINT idType);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	BOOL IsTouchOnObject(POINT *pt);
protected:
	void Render();
	BOOL LoadRes(LPCTSTR lpFileName);
	void   ReleaseRes();
	void DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si);
};

