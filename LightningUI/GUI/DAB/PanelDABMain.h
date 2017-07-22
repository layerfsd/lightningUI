#pragma once

#include "../Common/ui_common.h"
#include "../../DAB/Inc/DABRadioLib.h"
#include "../../DAB/Adapter/DABAdapter.h"

#if CVTE_EN_DAB

#define DEF_DAB1  0
#define DEF_DAB2  1
#define DEF_DAB3  2

class CPanelDABMain :
	public CPanelGL
{
public:
	CPanelDABMain(void);
	~CPanelDABMain(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);
	void OnGesture(GESTUREINFO *pgi);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void NotifyDabServiceReady(BOOL bReady);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);
	void UpdateScanStatus(BOOL bScaning);
    void UpdateCurChannelInfo();
	void UpdateBandlInfo();
	void SetNowPlayingIndex(int idxCur,int total);

    void UpdateSignal();
    BOOL ScanServicePlay(BOOL bScan);
    BYTE ChangeBandMode();
    BYTE ChangeSearchType();
	void UpdatePresetInfo();


public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);


private:
    static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

    void UpdateSrvLinkMode();
    void UpdateLBandSetting();
    void UpdateTASetting();
    void UpdatePTYSetting();
    //void UpdateListInfo();
	void SelectPreset(int idx);
	void DeletePreset(int idx);
	void SavePreset(int idx);
private:
    UINT  m_idCurChannel;
    DWORD m_dwEnterTick;
	DWORD			m_dwLastSearchTick;
	DWORD           m_dwDABInfoUpdateTick;
	DWORD           m_dwDLSTick;

	BYTE    m_nSearchTypeMode;
	BYTE    m_nLinkModel;
    BYTE    m_nBandMode;
    BYTE    m_nSignalSNR;
    BYTE    m_nSignal;

	CLabelGL			m_iconLoading;

    CM_String m_strFreq;
    CM_String m_strStation;
    CM_String m_strEnsemble;
    CM_String m_strService;
    CM_String m_strDLS;
	CM_String m_strPTY;

	BOOL            m_bDabServiceReady;
	BOOL            m_bScanning;
	BOOL            m_bDLSEffect;

    wchar_t * m_pStrDLS;
    UINT      m_nDLSLen;
	wchar_t * m_tagStrService;
private:
    CListBoxGL		m_boxDABList;
	CSmartBarMedia	*m_pSmartBarMedia;

	CLabelGL			m_tagBkBlur;
 	CLabelGL			m_tagBkOrigin;

	CLabelGL			m_tagBackEq;
	CLabelGL			m_tagBackCircle;

	CButtonGL		m_btnHome;

	CButtonGL       m_btnSetting;
	CButtonGL       m_tagDABListInfoBK;
	BOOL            m_bDABListInfoSpread;

//	    CButtonGL       m_tagMenuBar1;
//	    CButtonGL       m_tagMenuBar2;
//	    CButtonGL       m_tagMenuBar3;
//	    CButtonGL       m_tagMenuBar4;
//	    CButtonGL       m_tagMenuBar5;

	CLabelGL        m_iconTA;
	CLabelGL        m_iconTAFocus;
	CButtonGL       m_btnTA;
	BOOL            m_bTAOn;

//		CLabelGL        m_iconPTY;
//		CLabelGL        m_iconPTYFocus;
//		CButtonGL       m_btnPTY;
//		BOOL            m_bPTYOn;

	CLabelGL        m_iconLBAND;
	CLabelGL        m_iconLBANDFocus;
	CButtonGL       m_btnLBand;
	BOOL            m_bLBandOn;

//		CLabelGL        m_iconAMS;
//		CLabelGL        m_iconAMSFocus;
//		CButtonGL       m_btnAMS;
//		BOOL            m_bAMSOn;
//
//		CLabelGL        m_iconSAVE;
//		CLabelGL        m_iconSAVEFocus;
//		CButtonGL       m_btnSAVE;
//		BOOL            m_bSAVEOn;

	CButtonGL	    m_btnSeekForward;
	CButtonGL	    m_btnSeekBackward;
	CButtonGL	    m_btnTuneForward;
	CButtonGL	    m_btnTuneBackward;
	CButtonGL       m_btnPTYTypeChange;
	CButtonGL       m_btnPTYSearch;

    CLabelGL        m_iconBandChange;
    CLabelGL        m_iconFavorite;
    CLabelGL        m_iconFavoriteFocus;
    CButtonGL       m_btnFavorite;

	CButtonGL       m_btnFmAm;

    CButtonGL       m_btnBandChange;
    CButtonGL       m_btnSearchTypeChange;
    CButtonGL       m_btnScanPlay;

	CButtonGL		m_btnSearch;
	CButtonGL       m_tagSearchBlock;
	CSliderGL		m_sliSearchPercent;

	CButtonGL		m_btnLibrary;

	CLabelGL        m_iconSrvLinkOff;
	CLabelGL        m_iconSrvLinkDAB;
	CLabelGL        m_iconSrvLinkRDS;
	CLabelGL        m_iconSrvLinkALL;
    CButtonGL       m_btnSrvLink;

	CButtonGL		m_tagSrvLink;
    CButtonGL       m_tagStationName;
    CButtonGL       m_tagPTYMode;
    CButtonGL       m_tagEnsemble;
    CButtonGL       m_tagService;
    CButtonGL       m_tagDLS;
    CButtonGL       m_tagFreqNum;
    CButtonGL       m_tagFreq;
    CButtonGL       m_tagBandMode;
    CButtonGL       m_tagSearchTypeMode;
    CButtonGL       m_tagTA;
    CButtonGL       m_tagSearch;

	CButtonGL	m_tagNowPlaying;

	CButtonGL	m_tagCurFrenquency;

	CButtonGL       m_btnPreset[6];

	CButtonGL	m_tagAlbumBk;

    CSliderGL	m_sliDABSignal;
    CButtonGL   m_tagDABSignalQuality;
    CLabelGL    m_iconDABSignalBad;
    CLabelGL    m_iconDABSignalPoor;
    CLabelGL    m_iconDABSignalGood;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL		m_tagSearching;
	CButtonGL		m_btnBandFM;
	CButtonGL		m_btnBandAM;
	CButtonGL		m_btnDAB;
#endif

};

#endif
