#pragma once
#include "DABCommon.h"
#include "PanelDABLibrary.h"
#include "PanelDABMain.h"

#if CVTE_EN_DAB

class CPanelDABLibrary;
class CPanelDABMain;

class CPanelDABWrapper :
	public CPanelGL
{
public:
	CPanelDABWrapper(void);
	~CPanelDABWrapper(void);

	void OnSystemPowerStatePrepare(UINT idState);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	BOOL OnDABCommand(WPARAM wParam, LPARAM lParam);

    void LinkPages(CPanelDABMain *pMain, CPanelDABLibrary *pLibrary);
#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
#else
	BOOL OnKey(UINT idKey);
#endif
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

// 	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
// 	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);

//		void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	BOOL PlayIndex(INT index);
	BOOL PlayNext();
	BOOL PlayPrevious();
	BOOL SeekNext();
    BOOL SeekPrevious();

    void RefreshLibrary(DAB_LIBRARY_TYPE nLibType);
	BOOL DABProcess();
	static DWORD DABThreadPro(LPVOID lpParam);
    void EnableDAB(BOOL Enable);
    BOOL ScanServicePlay(BOOL bScan);
    BOOL AutoScanStore();
    BOOL ChangeBand();
    BOOL ChangeSearchType();
    BOOL UpdateScanStatus(BOOL bScanning);
	BOOL ScanService();

	void SetTASetting(BOOL bOn);
	void SetLBandSetting(BOOL bOn);
	BOOL IsPlayInfoValid();

private:
	CPanelDABMain	*m_ppMain;
	CPanelDABLibrary *m_ppLibrary;
	HANDLE			m_hDABThread;
    HANDLE          m_hEvtDABThread;
    DWORD   	    m_dwDABCmdTick;
    DWORD           m_dwDABLibUpdateTick;

	DWORD           m_dwDABScaningTick;
	BOOL            m_bDABServiceStart;
    BOOL            m_bDABScaning;
	BOOL            m_bNeedUpdateAfterScaning;
    BYTE            m_nBandMode;
    BYTE            m_nSearchType;
	BOOL			m_bIniStatus;
	BOOL			m_bPhoneCallActive;

};

#endif
