#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "iPodCommon.h"

class CPaneliPodLibrary :
	public CPanelGL
{
public:
	CPaneliPodLibrary(void);
	~CPaneliPodLibrary(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void onOpenFinish(DWORD cnt,BOOL bPlayable);
	void onRecordReady(UINT32 index, std::string record);
	void onRecordReady(UINT32 index, RootRecordID recordId);
	void onRecordReady(UINT32 index, TopLevelRecordID recordId);

	void SetBrowserLevel(int iLevel);

	void ClearList(){m_boxItems.ClearList();}

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:

	UINT					m_idValidSrcList[MEDIA_TYPE_NUMBER];
	UINT					m_idCurMediaType;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL			m_iconArrow;
#endif

	CLabelGL			m_iconDevBT;
	CLabelGL			m_iconDevUSB;
	CLabelGL			m_iconDevSD;
	CLabelGL			m_iconDevCD;
	CLabelGL			m_iconDeviPod;
	CLabelGL			m_iconDevBTFocus;
	CLabelGL			m_iconDevUSBFocus;
	CLabelGL			m_iconDevSDFocus;
	CLabelGL			m_iconDevCDFocus;
	CLabelGL			m_iconDeviPodFocus;
	CM_String			m_strBrowseTitle[32];

	DWORD			m_dwListCnt;
	BOOL				m_bPlayable;
	INT					m_iBrowserLevel;

	CLabelGL			m_iconTrack;
	CLabelGL			m_iconAlbum;
	CLabelGL			m_iconArtist;
	CLabelGL			m_iconGenre;
	CLabelGL			m_iconPlaylist;

	CLabelGL			m_iconReturn;
	CLabelGL			m_iconReturnFocus;

	CLabelGL		m_iconHomeUp;
	CLabelGL		m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL		m_iconNowPlayingUp;
	CLabelGL		m_iconNowPlayingDown;
	CButtonGL		m_btnNowPlaying;

	CButtonGL		m_btnReturn;
	CButtonGL		m_btnTitle;

	CButtonGL	m_tagNote;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

 	CListBoxGL		m_boxMenu;
	CListBoxGL		m_boxItems;

};

