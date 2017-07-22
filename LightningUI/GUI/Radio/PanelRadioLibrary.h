#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "RadioCommon.h"


class CPanelRadioLibrary :
	public CPanelGL
{
public:
	CPanelRadioLibrary(void);
	~CPanelRadioLibrary(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);

	void OnSystemPowerStatePrepare(UINT idState);

	//
	void AddFavorite(DWORD dwFreq,RadioBandId idBand,RadioRegionId idRegion,const wchar_t *pStrName=NULL);
	void DeleteFavorite(DWORD dwFreq);

	BOOL IsFavorite(DWORD dwFreq);

	int GetFMStorageIndex(DWORD dwFreq);
	int GetAMStorageIndex(DWORD dwFreq);

	int GetFMListCnt();
	int GetAMListCnt();
	int GetFavListCnt();

	DWORD GetNextFM();
	DWORD GetPrevFM();
	DWORD GetNextAM();
	DWORD GetPrevAM();

	//
	void ClearList(RadioBandId band);

	//
	void AddFM(DWORD dwFreq);
	void AddAM(DWORD dwFreq);

	BOOL OnUpdateRadioPlayInfo();
	void SetRadioRegion(RadioRegionId idRegion,BOOL bResetFreq);

	void SetPS(const wchar_t* ps);
	void SetAFSearching(BOOL bSearching){m_bAFSearching=bSearching;}

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void SetShowType(RADIO_LIBRARY_TYPE type);
// 	void RefreshLibrary();
	BOOL SaveFavData();
	BOOL GetFavData();

	BOOL GetRegionPresetData(RadioRegionId idRegion);
	BOOL SaveRegionPresetData(RadioRegionId idRegion);

	//
	void ReloadList(RadioRegionId idRegion,BOOL bResetFreq);

	void UpdateListPS();
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
typedef	enum _Item_Selected
	{
		FM_SELECTED = 0,
		AM_SELECTED,
		FAV_SELECTED
	}Item_Selected;
	void ArrowChange(Item_Selected index);
	GLfloat m_iconoffset;
#endif

private:

	RADIO_LIBRARY_TYPE m_type;

	RadioRegionId			m_RegionId;
	RadioBandId				m_BandId;
	RadioRegionId			m_RegionIdtoSet;

	CM_String		m_strPS;

	BOOL						m_bResetFreq;

	CM_List<RadioStationInfo>	m_listFM[RADIO_REGION_Max];
	CM_List<RadioStationInfo>	m_listAM[RADIO_REGION_Max];
	BOOL			m_bFMListChanged[RADIO_REGION_Max];
	BOOL			m_bAMListChanged[RADIO_REGION_Max];

	RadioStationInfo	m_infoFM[RADIO_REGION_Max];
	RadioStationInfo	m_infoAM[RADIO_REGION_Max];

	DWORD					m_dwCurFreq;

	int						m_iCurStoreIndex;
	UINT			m_idCurBOXMENU;

	BOOL			m_bFavListChanged;

	CLabelGL		m_iconHomeUp;
	CLabelGL		m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL		m_iconNowPlayingUp;
	CLabelGL		m_iconNowPlayingDown;
	CButtonGL		m_btnNowPlaying;

	CButtonGL	m_tagNote;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;


	CLabelGL			m_iconFM;
	CLabelGL			m_iconAM;
	CLabelGL			m_iconFavorite;
	CLabelGL			m_iconFreq;
	CLabelGL			m_iconDelete;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL			m_iconArrowOn;
	CLabelGL			m_iconArrowOff;
	CLabelGL			m_iconFavoriteList;
	CLabelGL			m_iconFavoriteListFocus;
#endif

	CLabelGL			m_iconFMFocus;
	CLabelGL			m_iconAMFocus;
	CLabelGL			m_iconFavoriteFocus;
	CLabelGL			m_iconFreqFocus;
	CLabelGL			m_iconDeleteFocus;

	CListBoxGL			m_boxLists;

	CListBoxGL			m_boxFM;
	CListBoxGL			m_boxAM;
	CListBoxGL			m_boxFavorite;

// 	BOOL				m_bRDSOn;
// 	BOOL				m_bTAOn;
// 	BOOL				m_bAFOn;
	BOOL				m_bSearching;
	BOOL				m_bAFSearching;

};

