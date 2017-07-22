#pragma once
#include "../Common/ui_common.h"

#include "../../Communication/CommanderEx.h"
#include "../Controls/SmartBarMedia.h"

#include "RadioCommon.h"

// #include "PanelRadioLibrary.h"

#define MAX_QUICK_FAV_CNT			10

#define ENABLE_DAB_UI					1

class CPanelRadioMain :
	public CPanelGL
{
public:
	CPanelRadioMain(void);
	~CPanelRadioMain(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGestureMsg(UINT idGesture);
	void OnGesture(GESTUREINFO *pgi);

	void OnSystemPowerStatePrepare(UINT idState);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);

	void SetBand(RadioBandId id);
	void SetFreq(DWORD freq, BOOL bReset=FALSE);

	void SetFavStatus(BOOL bFav);
// 	void LinkLibrary(CPanelRadioLibrary *pLibrary){m_pLibrary = pLibrary;}
	void SetNowPlayingTitle(const wchar_t *pNowPlayingTitle);

	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar){m_pSmartBarMedia = pSmartBar;}
	BOOL OnUpdateRadioPlayInfo();
	void AutoUpdateQuickFav(DWORD dwFreq,int index=-1);
// 	void AddQuickFav(DWORD dwFreq);
	void DeleteQuickFav(DWORD dwFreq);

	void SetPS(const wchar_t* ps);
	void SetRT(const wchar_t* rt);
	void SetPTY(ProgramType idPTY);
	void ShowPSForCurrentFreq(BOOL bShow,BOOL bPowerOn = FALSE);
	void SetPI(DWORD dwFreq, DWORD dwPICode);

	void SetOnTAMode(BOOL bTA){m_bOnTAMode = bTA;  m_btnCancelTA.Show(m_bOnTAMode);  EnableQuit(!bTA);}
	void EnableUpdateWidget(BOOL bEnable){m_bEnableUpdateWidget = bEnable;}
	void SetAFSearching(BOOL bSearching){m_bAFSearching=bSearching;m_dwAFSearchingTick=GetTickCount();}

public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);


private:
	void SetSignalStrength(UINT idb);
	void SetScaleOffset(GLfloat offset,BOOL bAni=TRUE);
	void UpdateScaleForBand(RadioBandId id);
	void ApplyCurrentScaleFreq();
	void StopScaleAni();
	void UpdateRegionDisplay(RadioRegionId id);
	void ShowQuickFav(BOOL bShow,BOOL bAni=TRUE);
	void AddQuickFav(RadioBandId id,RadioStationInfo* info,int index);
	void UpdateQuickFavList();
	BOOL SaveQuickFavData();
	BOOL GetQuickFavData();
	BOOL SaveRdsListData();
	BOOL GetRdsListData();

	void UpdateRdsList(DWORD dwFreq,const wchar_t* name,DWORD dwPICode);
	const wchar_t* GetFreqPS(DWORD dwFreq);
public:
	const DWORD GetFreqPI(DWORD dwFreq);
#if ENABLE_RADIO_DEBUG_SCREEN
	void SetDebugRadioSignalData(UINT idx, const SIGNAL_DATA* pData);
	void SetDebugAFData(const AFData* pData);
	void SetDebugPIData(WORD piCode, WORD piCodeUser);
	void ClearDebugData();
#endif
private:
	RadioRegionId			m_RegionId;
	RadioBandId				m_BandId;
	DWORD					m_dwCurFreq;
	DWORD					m_dwCurPICode;
	DWORD					m_dwCurPICodeFreq;

	CSmartBarMedia	*m_pSmartBarMedia;

	DWORD					m_dwFreqChangeTick;
	BOOL						m_bCheckFavorite;
	BOOL			m_bOnTAMode;

	DWORD					m_dwFreqFM;
	DWORD					m_dwFreqAM;

	DWORD					m_dwFreqMinFM;
	DWORD					m_dwFreqMaxFM;
	DWORD					m_dwFreqMinAM;
	DWORD					m_dwFreqMaxAM;
	int							m_iStepFM;
	int							m_iStepAM;

	CLabelGL			m_tagBkBlur;
 	CLabelGL			m_tagBkOrigin;

	CLabelGL			m_tagBackEq;
	CLabelGL			m_tagBackCircle;

	BOOL				m_bSearching;
	BOOL				m_bAFSearching;
	DWORD			m_dwAFSearchingTick;

	CButtonGL		m_btnHome;
	CButtonGL		m_btnSearch;

	CButtonGL		m_btnLibrary;
	CButtonGL		m_btnBandFM;
	CButtonGL		m_btnBandAM;
    #if CVTE_EN_DAB
	CButtonGL		m_btnDAB;
    #endif

	CButtonGL		m_btnRadioRegion;

	CButtonGL		m_btnCurPS;

	CButtonGL		m_btnQuickFav[MAX_QUICK_FAV_CNT];

// 	CButtonGL		m_btnPower;

	CButtonGL	m_tagNowPlaying;

// 	CLabelGL			m_tagFreBack;
// 	CLabelGL			m_tagSignal;
	CLabelGL			m_tagSearching;
	CSliderGL			m_sliRadioSignal;

	CButtonGL		m_tagBandTitle;
	CButtonGL		m_tagChannelTitle;

	CButtonGL		m_tagFreqUnit;
// 	CButtonGL		m_tagSingalStrength;
	CButtonGL		m_tagSTMono;
	CButtonGL		m_tagLOC;
// 	CButtonGL		m_tagTitle;
	CButtonGL		m_tagTA;
	CButtonGL		m_tagAF;

	CButtonGL		m_tagPS;
	CButtonGL		m_tagRT;
	CButtonGL		m_tagPTY;
	CButtonGL		m_tagPISearching;

	CButtonGL	m_tagCurFrenquency;

	CButtonGL	m_btnSeekForward;
	CButtonGL	m_btnSeekBackward;
	CButtonGL	m_btnTuneForward;
	CButtonGL	m_btnTuneBackward;

	CButtonGL		m_btnFavorite;
	CButtonGL		m_btnCancelTA;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CButtonGL		m_btnAudioSetting;
#endif

	CM_String			m_strRadioText;
	DWORD			m_dwRadioTextUpdateTick;
	int					m_iRadioTextShowOffset;
	int					m_iRadioTextUpdateInterval;
	BOOL				m_bShowingLongRadioText;

	int					m_iScaleCount;
	int					m_iScaleMargin;
	CButtonGL		m_tagScale[60];
	CLabelGL			m_tagFrePointer;

	GLfloat				m_fScaleOffsetLimitHigh;
	GLfloat				m_fScaleOffsetLimitLow;

	BOOL				m_bScaleMoving;
	GLfloat				m_fScaleOffsetCur;
	GLfloat				m_fScaleOffsetSet;
	CM_String			m_strPS;

	GLfloat				m_hidearea;

	BOOL				m_bEnableScaleDraging;
	BOOL				m_bScaleDraging;
	GLfloat				m_fScaleOffsetBeforeDraging;
	DWORD			m_dwLastPressTick;
	DWORD			m_dwLastScaleDragTick;

	BOOL				m_bShowQuickFav;
	DWORD				m_dwShowQuickFavTick;

	BOOL			m_bFavListChanged;
	RadioStationInfo m_FMFavList[MAX_QUICK_FAV_CNT];
	RadioStationInfo m_AMFavList[MAX_QUICK_FAV_CNT];

// 	CM_String		m_strFMFavListPS[MAX_QUICK_FAV_CNT];
// 	CM_String		m_strAMFavListPS[MAX_QUICK_FAV_CNT];
// 	DWORD		m_dwFMFavList[MAX_QUICK_FAV_CNT];
// 	DWORD		m_dwAMFavList[MAX_QUICK_FAV_CNT];
// 	RadioRegionId		m_regionFMFavList[MAX_QUICK_FAV_CNT];
// 	RadioRegionId		m_regionAMFavList[MAX_QUICK_FAV_CNT];

	BOOL				m_bAutoShowList;
	DWORD			m_dwLastSearchTick;

	//BOOL				m_bRDSOn;

	CM_List<RadioStationInfo>	m_listRdsStations;
	BOOL	m_bEnableUpdateWidget;

	BOOL	m_bShowPSForCurrentFreq;
	BOOL			m_bRdsListChanged;

	ProgramType m_idPTY;

#if ENABLE_RADIO_DEBUG_SCREEN
	UINT	 m_iDebugClickRef;
	BOOL m_bEnbaleDebugScreen;
	SIGNAL_DATA m_DebugRadioSignalData[256];
	AFData	m_DebugAFData;
	WORD m_DebugPIData;
	CButtonGL		m_tagDebugPI;
	CButtonGL		m_tagDebugAFlist;
#endif
};
