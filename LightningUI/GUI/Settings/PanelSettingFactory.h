#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#include "../../Include/CriticalSection.h"

class CPanelSettingFactory :
	public CPanelGL
{
public:
	CPanelSettingFactory(void);
	~CPanelSettingFactory(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

    void OnMCUCmd(WPARAM wParam, LPARAM lParam);
    void OnRearColorSignalChange(UINT uColorSignal);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
    static DWORD TPAutoRecalibrateThread(LPVOID pDevice);
    static DWORD SystemMonitorThread(LPVOID pDevice);

    void TPAutoRecalibrateProc();
    void UpdateTime();

private:
	void Lock(){m_DataCS->Enter();}
	void UnLock(){m_DataCS->Leave();}

private:

	void SetTestMode(BOOL bTestMode);

	BOOL IsFileExists(LPCTSTR   lpszFileName);

    CCriticalSection* m_DataCS;
    CRITICAL_SECTION  m_csMonitor;
    UINT      m_nCpuUtilzation;
    BOOL      m_bExplorerIsRunning;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif

    CButtonGL m_tagTitle1;
    CButtonGL m_tagTitle2;

    CLabelGL  m_labIcon;
    CLabelGL  m_labIconFocus;
    CButtonGL m_btnReturn;

    CButtonGL m_btnCpu;
    CButtonGL m_tagSystemInfo;
    CLabelGL  m_iconCpuFocus;
    CButtonGL m_btnMemory;
    CButtonGL m_tagMemory;

    CButtonGL m_btnMediaAutoTest;
    CButtonGL m_tagMediaAutoTestInfo;
    CLabelGL  m_iconMediaAutoTest;
    CLabelGL  m_iconMediaAutoTestFocus;
    CButtonGL m_tagTime;

    CButtonGL m_btnBluetoothAutoTest;
    //CButtonGL m_tagBluetoothAutoTestInfo;
    CLabelGL  m_iconBluetoothAutoTest;
    CLabelGL  m_iconBluetoothAutoTestFocus;

    CButtonGL m_btnTouchAutoTest;
    //CButtonGL m_tagTouchAutoTestInfo;
    CLabelGL  m_iconTouchAutoTest;
    CLabelGL  m_iconTouchAutoTestFocus;

    CButtonGL m_btnTPAutoReCalibrate;
    CButtonGL m_tagTPAutoReCalibrateInfo;
    CLabelGL  m_iconTPAutoReCalibrate;
    CLabelGL  m_iconTPAutoReCalibrateFocus;

    CButtonGL m_btnSnapshot;
    //CButtonGL m_tagSnapshotInfo;
    CLabelGL  m_iconSnapshot;
    CLabelGL  m_iconSnapshotFocus;

    CButtonGL m_btnExplorer;
    CLabelGL  m_iconExplorer;
    CLabelGL  m_iconExplorerFocus;

    CButtonGL m_btnLED;
    CLabelGL  m_iconLED;
    CLabelGL  m_iconLEDFocus;

	CButtonGL m_btnConnectTest;
    CLabelGL  m_iconConnectTest;

	CButtonGL m_btnDeleteMAC;
    CLabelGL  m_iconDeleteMAC;

	CButtonGL m_btnDeleteCPCA;
    CLabelGL  m_iconDeleteCPCA;

	CButtonGL m_btnCarPlayRestart;
    CLabelGL  m_iconCarPlayRestart;

	CButtonGL m_tagReleaseDay;    // release day of inner version

	//===== USB TEST DEFINE=========
	CButtonGL m_tagUSBTest;
	CButtonGL m_tagUSBTestMode;
	CButtonGL m_btnUSBTestSpread;
	CButtonGL m_btnUSBTestModeDisable;
    CButtonGL m_btnUSBTestModeJState;
    CButtonGL m_btnUSBTestModeKState;
    CButtonGL m_btnUSBTestModeSe0Nak;
    CButtonGL m_btnUSBTestModePacket;
    CButtonGL m_btnUSBTestModeForceHS;
    CButtonGL m_btnUSBTestModeForceFS;
    CButtonGL m_btnUSBTestModeForceLS;

    CButtonGL m_btniPerfServer;
	CButtonGL m_btniPerfClient;

	CLabelGL  m_iconUSBTest;
	CLabelGL  m_iconiPerfServer;
	CLabelGL  m_iconiPerfClient;
	//=====USB TEST END========

	//======H.264==============
	CButtonGL m_tagH264Test;
	CButtonGL m_btnAvccPath;
	CButtonGL m_tagValueAvccPath;
	CButtonGL m_btnH264Path;
	CButtonGL m_tagValueH264Path;
	CButtonGL m_btnH264Test;
	//=========================

    CButtonGL	m_btnRearColorSignalType;
	CButtonGL	m_tagRearColorSignalType;
    CLabelGL    m_iconRearColorSignalType;
//	    HANDLE    m_hMediaAutoTestThread;
//	    HANDLE    m_hMediaAutoTestEvent;

    BOOL      m_bMediaTesting;
    BOOL      m_bTouchTesting;
    BOOL      m_bTpAutoRecalibrating;
    BOOL      m_bCpuMonitorActivate;
	BOOL      m_bUsbTestSpread;

    DWORD     m_dwCpuMonitorTimeout;

    HANDLE   m_hActivateResetTouchEvent;
    HANDLE   m_hResetTouchFeedbackEvent;
    HANDLE   m_hResetTouchThread;
    HANDLE   m_hResetTouchStart;
    HANDLE   m_hCpuMonitorThread;
    HANDLE   m_hCpuMonitorEvent;

    SYSTEMTIME m_SystimeStart;

	CM_String		strAvccPath;
	CM_String		strH264Path;


};

