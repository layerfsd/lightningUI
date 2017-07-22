#pragma once
#include "PanelRadioMain.h"
#include "PanelRadioLibrary.h"

#define MAX_PS_FRAME		32

class CPanelRadioWrapper :
	public CPanelGL
{
public:
	CPanelRadioWrapper(void);
	~CPanelRadioWrapper(void);

	void OnSystemPowerStatePrepare(UINT idState);

	void OnSRCChanged(UINT idSRC);
	void OnUIChanged(UINT idUI);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void LinkPages(CPanelRadioMain *pMain,CPanelRadioLibrary *pLibrary);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnSRStart();
	void OnSRTerminate();

	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

	void OnRadioCommand(WPARAM wParam, LPARAM lParam);

	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar);

	void OnUpdateRadioPlayInfo();

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	void OnRadioMonitorTA();
#endif
	void OnReverseModeChange(BOOL bReverse);
	void OnDabServiceLinkMsg(WPARAM wParam, LPARAM lParam);

	void SetLcdoscRatioOffset(DWORD dwCurFreq);
	static int SetPixelClock(UINT guiPixelClock);
	static int GetPixelClock(UINT* guiPixelClock);

public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	void OnTAStatus(BOOL bSignaled);
#endif

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void PlayNext();
	void PlayPrevious();
	void Play();
	void Pause();

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	void AnalyzePS(BYTE *bufPS, INT iLen);
	void AnalyzeRT(BYTE *bufRT, INT iLen);
#endif

private:
	BOOL SetFrequency(DWORD dwFreq,BOOL bSync=TRUE);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	BOOL SetFrequencyAndGetSignal(DWORD dwFreq);
	BOOL DetectAFSignal(DWORD dwFreq);
	BOOL JumpAF(DWORD dwFreq);
	BOOL CheckAF(DWORD dwFreq);
	WORD CheckPI(DWORD dwFreq, int iRetry);
	BOOL ProcessAF(const int idxProcess, const int idxCurrent);
	BOOL GetSignalData(DWORD dwFreq);
	//RDS:
	UINT	  GetRDSData(UINT *pRdsDataType,BOOL *pbDataLoss=NULL);
	BOOL GetRDSStatus();
	BOOL GetRDSBlockData();
	BOOL GetSignalQuality(BOOL bSync);
	void UpdateRDSDatabase(RdsInfo *prdsInfo);
	BOOL LoadRDSDatabase();
	BOOL SaveRDSDatabase();
	RdsInfo* SearchDatabaseByAF(DWORD dwAFFreq);

	void EnableRDS(BOOL Enable);
	void EnableAF(BOOL Enable);
	void EnableTA(BOOL Enable);

	BOOL RdsProcess();
	static DWORD RdsThreadPro(LPVOID lpParam);

	BOOL RdsCommonProcess();
	BOOL RdsAFProcess();
	BOOL RdsTAProcess();
	void AbortCurrentRdsProcess();
	void StartPISearching(DWORD dwStartFreq,WORD piSearch);
#if CVTE_EN_DAB
	BOOL RdsDABServiceLinkProcess();
#endif
#endif

// 	void OnTAStatus(BOOL bSignaled);

	RdsData m_rdsData;
	RdsInfo m_rdsInfo;
	WORD 	m_rdsStatus;
	AFData	m_AFData;
	SIGNAL_DATA m_radioSignalData[256];
	WORD m_curPIForAF;
	WORD m_userSetPI;
	WORD m_searchingPI;
	DWORD m_userSetPIFreq;
	BOOL m_bPISearching;
	BOOL m_bAFSearching;
	BOOL m_bNeedDoPISearching;
	BOOL m_bAbortCurrentRDSProcess;
	DWORD m_dwFreqPISearchStart;
	DWORD m_dwFreqPISearching;
	DWORD m_idRadioCmdType;
	DWORD m_idRadioCmdPara;
	BOOL	m_bAutoUpdateAFinfo;
	HANDLE			m_hRdsThread;
	HANDLE			m_hEvtRdsData;
	HANDLE			m_hEvtRdsThread;
	HANDLE			m_hEvtRadioSignal;
	HANDLE			m_hEvtRdsAbort;
	DWORD			m_dwQuality;
	HANDLE			m_hEvtFreq;
	DWORD			m_dwSignalLevel;
	DWORD			m_dwQualityCntNG;
	DWORD			m_dwQualityCntGood;
	DWORD			m_dwStopLevelAF;
	BOOL				m_bNeedScanAllAFList;
	DWORD			m_dwRdsDataLossCnt;
	DWORD			m_dwRdsDataErrorCnt;
	DWORD			m_dwRdsDataRequestCnt;
	DWORD			m_dwFreqRequestSingal;

	BOOL				m_bTASearching;
	DWORD			m_dwFreqBeforeTASearching;
	DWORD			m_dwRdsStartTick;
	DWORD			m_dwFreqBeforeDABLink;

	WORD				m_radioSignalStatus;

private:

	CPanelRadioMain		*m_ppanelMain;
 	CPanelRadioLibrary	*m_ppanelLibrary;

	RadioRegionId			m_RegionId;
	RadioBandId				m_BandId;
	DWORD					m_dwCurFreq;
	DWORD					m_dwRadioCmdTick;
	DWORD					m_dwAFSearchTick;
	DWORD					m_dwSRCChangeTick;
	DWORD					m_dwAFSearchInterval;
	int							m_iAFSearchIndex;
	DWORD					m_dwFreqSet;

	BOOL				m_bQuickTAMode;

	BOOL			m_bSpeechOn;
	BOOL           m_bSROn;

	BOOL			m_bPhoneCallActive;

	BOOL			m_bNotifyingRadioInfo;
	BOOL			m_bPlaying;

	BOOL			m_bTASignaled;
	BOOL			m_bTAMonitor;
	BOOL			m_bOnTAMode;
	BOOL			m_bTADetected;
	DWORD		m_dwTADetected;
	DWORD		m_dwTACancelTick;
	DWORD		m_dwTAQuitTick;
	DWORD		m_dwTAEnterTick;

	UINT				m_idAudioSrcBeforeTA;
	UINT				m_idMediaClassBeforeTA;
	UINT				m_idUIBeforeTA;
	BOOL			m_bSrcPlayingBeforeTA;

	//BYTE		m_bufPS[MAX_PS_FRAME][8];
	BYTE		m_bufPSOut[9];
	//INT		m_iPSBufIndex;
	BOOL	m_bMCUScanning;
	UINT m_idRdsTask;
	DWORD m_dwRdsTaskStartTick;
	UINT m_idCurrentIdSrc;
	DWORD m_dwLastProcessRDSTick;
	DWORD m_dwRDSProcessTime;
	DWORD m_dwGetSignalQualityTick;
	BOOL		m_bEnableRDSProcess;
	DWORD m_dwLastPISearchTick;

	//for DAB service link:
	BOOL	m_bDabLinkProgramFound;
	BOOL	m_bDabServiceLinked;

	BOOL		m_bManualPlay;
	UINT	m_curuiPixelClock;
	CM_List<RdsInfo>	m_listRdsDatabase;
	BOOL						m_bRdsDatabaseChanged;
	BOOL				m_bUserSetRDSON;
	BOOL				m_bJumpAFFlag;
#if ENABLE_RADIO_DEBUG_SCREEN
	BOOL m_bEnbaleDebugScreen;
#endif
};
