#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "ImageCommon.h"
#include "../Media/MediaCommon.h"


class CPanelImageLibrary :
	public CPanelGL
{
public:
	CPanelImageLibrary(void);
	~CPanelImageLibrary(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void EnableRefresh();
	void ClearList();
    int GetCurIndex();
    void MoveToIndex(INT index);
	const wchar_t* GetIndexValue(INT index, BOOL bMoveIndex = TRUE);
    const wchar_t* GetCurValue();
	const wchar_t* GetNextValue();
	const wchar_t* GetPreviousValue();

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);
	void SelectDevice(UINT idDevice);
	void SetBrowseType(MEDIA_LIBRARY_TYPE type);
	INT GetTrackCount()
	{
		if(m_type==MEDIA_LIBRARY_EXPLORER)
			return m_iExplorerFilesCount;
		else
			return m_boxLibrary.GetCount();
	}
	INT GetFolderCount()
    {
		if(m_type==MEDIA_LIBRARY_EXPLORER)
			return m_iExplorerFolderCount;
		else
			return 0;
	}
protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);


private:
	BOOL IsImageFile(LPCTSTR szFileName);
	BOOL ExploreImageFiles(LPCTSTR lpszPath);
	static DWORD ExploreThreadPro(LPVOID lpParam);

	void RefreshLibrary();
	DWORD WaitForRefreshDone(DWORD dwWait);
	void ExplorePath(const wchar_t* lpszPath);
	void SetCurPath(const wchar_t* lpszPath);
	BOOL IsOnRoot();
	void EnterCurPath();
	void GotoParent();
	int CreateSearchResultList(const wchar_t *SearchString,CListBoxGL *plistboxIn,CListBoxGL *plistboxOut);

private:
	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

	BOOL				m_bNeedRefreshLib;
	BOOL				m_bRefreshingLib;
	HANDLE			m_hExploreThread;
	HANDLE			m_hRefreshEvent;

	CLabelGL		m_iconFolders;
	CLabelGL		m_iconHomeUp;
	CLabelGL		m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL		m_iconNowPlayingUp;
	CLabelGL		m_iconNowPlayingDown;
	CButtonGL		m_btnNowPlaying;
	CButtonGL	    m_tagNote;

	CLabelGL			m_iconLoading;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL			m_iconArrow;
#endif
	CLabelGL			m_iconDevUSB;
	CLabelGL			m_iconDevSD;
	CLabelGL			m_iconDevUSBFocus;
	CLabelGL			m_iconDevSDFocus;

	CLabelGL			m_iconImage;
	CLabelGL			m_iconImageFocus;

	CLabelGL			m_iconFolder;
	CLabelGL			m_iconFolderFocus;

	CLabelGL			m_iconExplorer;
	CLabelGL			m_iconExplorerFocus;

	CListBoxGL		m_boxDevice;
	CListBoxGL		m_boxLibrary;
	CListBoxGL		m_boxExplorer;
	CListBoxGL		m_boxSearched;

	CButtonGL		m_btnCatalogAudio;
	CButtonGL		m_btnCatalogVideo;
	CButtonGL		m_btnCatalogImage;
	CButtonGL		m_btnBrowseMode;
	CButtonGL		m_btnSearch;

	CLabelGL			m_iconBrowseFolders;
	CLabelGL			m_iconBrowseFiles;

	CLabelGL			m_iconSearch;
	CLabelGL		m_iconCatalogAudio;
	CLabelGL		m_iconCatalogVideo;
	CLabelGL		m_iconCatalogImage;
	CLabelGL		m_iconCatalogImageFocus;
	

	MEDIA_LIBRARY_TYPE m_type;
	DWORD m_dwSetShowTypeTick;

	UINT							m_idValidSrcList[16];

	UINT					m_idCurDeviceID;
	CM_String			m_strCurPath;
	CM_String			m_strCurRoot;
	CM_String			m_strCurPlayFile;
	INT					m_iExplorerFilesCount;
	INT					m_iExplorerFolderCount;
	INT					m_iExplorerPlayIndex;
};

