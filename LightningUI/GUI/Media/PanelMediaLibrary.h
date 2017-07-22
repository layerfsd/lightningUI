#pragma once
#include "MediaCommon.h"

#include "MediaPlayingFlagBar.h"

#define MEDIA_FILE_TYPE_FOLDER			0
#define MEDIA_FILE_TYPE_AUDIO				1
#define MEDIA_FILE_TYPE_VIDEO				2
#define MEDIA_FILE_TYPE_ALL					3
#define MEDIA_FILE_TYPE_INVALID			4

class CPanelMediaLibrary :
	public CPanelGL
{
public:
	CPanelMediaLibrary(void);
	~CPanelMediaLibrary(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGestureMsg(UINT idGesture);

	void SetNotifyWnd(HWND hwnd){m_hwndNotify = hwnd;}
	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
    //void OnFactoryCmd(WPARAM wParam, LPARAM lParam);

	void RefreshLibrary();
	void EnableOperate(BOOL bEnable){m_bEnableOperate = bEnable;}
	void SetProgressValue(int iSet){if(!m_sliProgress.IsActived())m_sliProgress.SetValue(iSet);}
	void ShowProgressSlider(BOOL bShow)
	{
		if(m_bEnableShowSlider)
		{
			if(bShow && !m_sliProgress.IsVisible())//need fixed
			{
				m_bNeedUpdateSlider = TRUE;
			}
		}
	}

	void SetPlayingStatus(BOOL bPlaying){m_bPlayingFlag=bPlaying;}

	const wchar_t* GetCurValue(){CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
		return p->GetCurFocusItemValue();}
	const wchar_t* GetCurName(){CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
		return p->GetCurFocusItemName();}

	UINT GetFocusID(){CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
		return p->GetCurFocusItemID();}
	UINT GetFocusIndex(){CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
		return p->GetCurIndex();}

	const wchar_t* GetIndexValue(INT index)
	{
		CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
		p->MoveToIndex(index,0,FALSE);
		return p->GetCurFocusItemValue();
	}

	const wchar_t* GetLibraryValue(INT index)
	{
		m_boxLibrary.MoveToIndex(index,0,FALSE);
		return m_boxLibrary.GetCurFocusItemValue();
	}

	const wchar_t* GetNextValue();
	const wchar_t* GetPreviousValue();
	const wchar_t* GetRandomValue();

	void MoveToNext();
	void MoveToPrevious();

	void MoveToTrackIndex(INT index)
	{
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			m_iExplorerPlayIndex = index;
			m_boxExplorer.MoveToIndex(m_iExplorerFolderCount+index);
		}
		else
		{
			m_boxLibrary.MoveToIndex(index);
		}
	}

	INT SearchByValue(LPCTSTR lpValue){return m_boxLibrary.SearchValue(lpValue);}

	INT GetCurIndex(){
		if(m_type==MEDIA_LIBRARY_EXPLORER)
			return m_iExplorerPlayIndex;
		else
			return m_boxLibrary.GetCurIndex();}
	INT GetTrackCount(){
		if(m_type==MEDIA_LIBRARY_EXPLORER)
			return m_iExplorerFilesCount;
		else
			return m_boxLibrary.GetCount();}
	INT GetFolderCount(){
		if(m_type==MEDIA_LIBRARY_EXPLORER)
			return m_iExplorerFolderCount;
		else
			return 0;}

	INT GetTotalTrackCount(){return m_boxLibrary.GetCount();}

	void SetMediaSrcType(UINT idType);

	DWORD WaitForRefreshDone(DWORD dwWait = 10000);

	void SetMediaCategory(MEDIA_CATEGORY category);

	void ClearList();

	BOOL IsRefreshing(){return m_bRefreshingLib;}
	void AbortRefresh();

	void SetShowType(MEDIA_LIBRARY_TYPE type);

	void SetLoadingStatus(FILE_LOADING_STATUS status);

	MEDIA_LIBRARY_TYPE GetShowType(){return m_type;}

	void SetCurPath(const wchar_t* lpszPath);

	void EnterCurPath();
	void GotoParent();
	BOOL IsOnRoot();
	BOOL IsOnQuickSearchMode();
	void SetQuickSearchMode(BOOL bSearchMode);

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);

	void SetCurPlayFile(const wchar_t *filename);

	void RemoveInvalidFile(const wchar_t *filename);

	void EnableVideo(BOOL bEnable);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	BOOL IsVideoFile(LPCTSTR szFileName);
	BOOL IsAudioFile(LPCTSTR szFileName);

	BOOL ExploreMeidaFiles(LPCTSTR lpszPath);
	static DWORD ExploreThreadPro(LPVOID lpParam);
// 	const wchar_t* GetDeviceFriendlyName(LPCTSTR lpDeviceName);

	void ExplorePath(const wchar_t* lpszPath);
	BOOL IsMassStorageMediaType(UINT idType);
    BOOL IsFileExists(LPCTSTR   lpszFileName);
	int CreateSearchResultList(const wchar_t *SearchString,CListBoxGL *plistboxIn,CListBoxGL *plistboxOut);

private:
    BOOL                m_bUsb1Available;
    BOOL                m_bUsb2Available;
	HWND						m_hwndNotify;

	UINT							m_idValidSrcList[MEDIA_TYPE_NUMBER];
	UINT							m_idCurMediaType;
    UINT                m_iFactoryTestType;
    UINT                m_iFactoryTestCmd;

	MEDIA_CATEGORY	m_mediaCategory;

	BOOL		m_bEnableVideo;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	DWORD		m_dwNoValidFileTick;
	FILE_LOADING_STATUS	m_idLoadingStatus;
#endif
	CLabelGL		m_iconHomeUp;
	CLabelGL		m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL		m_iconNowPlayingUp;
	CLabelGL		m_iconNowPlayingDown;
	CButtonGL		m_btnNowPlaying;

	CSliderGL  m_sliProgress;
	BOOL		m_bEnableShowSlider;
	BOOL		m_bNeedUpdateSlider;

	CLabelGL		m_iconCatalogAudio;
	CLabelGL		m_iconCatalogVideo;
	CLabelGL		m_iconCatalogImage;

	CMediaPlayingFlagBar m_barPlayingFlag;

	CButtonGL		m_btnCatalogAudio;
	CButtonGL		m_btnCatalogVideo;
	CButtonGL		m_btnCatalogImage;

	CButtonGL		m_btnBrowseMode;
	CLabelGL			m_iconLoading;
	BOOL			m_bPlayingFlag;

// 	CLabelGL		m_iconCatalogArtist;
// 	CButtonGL		m_btnCatalogArtist;

	CButtonGL	m_tagNote;

	MEDIA_LIBRARY_TYPE m_type;
	DWORD m_dwSetShowTypeTick;

	HANDLE			m_hRefreshEvent;

	BOOL				m_bCancelRefresh;

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

	CLabelGL			m_iconFolder;
	CLabelGL			m_iconAudio;
	CLabelGL			m_iconVideo;
	CLabelGL			m_iconImage;
	CLabelGL			m_iconSearch;

	CLabelGL			m_iconAudioFocus;
	CLabelGL			m_iconVideoFocus;


	CButtonGL		m_btnSearch;

	CListBoxGL		m_boxDevice;
	CListBoxGL		m_boxLibrary;
	CListBoxGL		m_boxExplorer;
	CListBoxGL		m_boxSearched;

	CLabelGL			m_iconExplorer;

	CM_String			m_strCurPath;
	CM_String			m_strCurRoot;

	CM_String			m_strCurPlayFile;

	INT					m_iExplorerFilesCount;
	INT					m_iExplorerFolderCount;
	INT					m_iExplorerPlayIndex;

// 	CButtonGL		m_barTitle;

	HANDLE			m_hExploreThread;
	//CM_String			m_strDeviceName;
	//CM_String			m_strTitle;

	BOOL				m_bRefreshingLib;
	INT					m_iTotalFileScanned;

	BOOL				m_bEnableOperate;

	BOOL				m_bAutoReturn;
	DWORD			m_dwRefreshTick;

// 	CLabelGL			m_tagSeperator1;

// 	CButtonGL		m_btnRefresh;
// 	CMsgBoxGL		m_boxNote;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	enum SELECED_ITEM
	{
		SELECED_ITEM0 = 0,
		SELECED_ITEM1,
		SELECED_ITEM2,
		SELECED_ITEM3,
	};

	CLabelGL			m_iconArrowOn;
	CLabelGL			m_iconArrowOff;

	CLabelGL			m_iconCatalogAudioFocus;
	CLabelGL			m_iconCatalogVideoFocus;
	CLabelGL			m_iconCatalogImageFocus;
	CLabelGL			m_iconFolderFocus;
	CLabelGL			m_iconExplorerFocus;

	void ArrowChange();
#endif
};

