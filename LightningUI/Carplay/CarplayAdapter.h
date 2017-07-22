#pragma once

#include <windows.h>
//#include <atlstr.h>
#include "CarplayAPI.h"
#include <CMthread.h>
#include <CSync.h>
#include <list>
#include <mmsystem.h>
#include "../GUI/Main/GUI_Fantasy.h"

#if CVTE_EN_CARPLAY

using namespace std;

typedef void
	( *DeviceActivateCallback_f )(
	PVOID context,
	BOOL IsActivated );

typedef void
	( *SetMACCallback_f )(
	PVOID context,
	BOOL IsSetMAC );

class CLocationInfo
{
public:
	CLocationInfo();
	~CLocationInfo();
	void StartLocationInformation(const LocationInfo_t* inLocationInfo);
	void StopLocationInformation();
	void OnSpeedSample(double inSpeed);
	void OnSlipDetect(BOOL inSlipDetect);
	void OnTransmissionState(char inTransmissionState);
private:
	static DWORD WINAPI ThreadProc( LPVOID dParam );
	DWORD ThreadRun();
	BOOL ForceTerminated();
	HANDLE 				m_hQuitEvent;
	HANDLE              m_hThreadHandle;
    DWORD               m_dwThreadId;
	DWORD				m_dwExitCode;
	PASCDData_t			m_PASCDdata;
	CLockObject			m_PASCDdataLock;
};

class CCarplayAdapter : public CMiniThread
{
public:
    static CCarplayAdapter* GetInstance();
    void Initialize(CSkinManagerGL* pSkinManager, HWND window, DeviceActivateCallback_f callback, SetMACCallback_f setmaccallback, PVOID context);
    void Deinitialize();
	BOOL IsInitialized();

	static BOOL IsFileExists(LPCTSTR lpszFileName);
	static char * MACAddressToCString( const unsigned char mac[ 6 ], char *outStr, BOOL bColon = TRUE );
	static OSStatus CStringToMACAddress( const void *inText, size_t inTextSize, size_t inAddrSize, void *outAddr );

    wchar_t* GetDeviceName();

    const AirPlayModeState* GetModeState();
	BOOL GetTestMode();
	void SetTestMode(BOOL inTestMode);
    OSStatus SetUSBTestMode(CarplayAPITestModeType inTestMode);

	void OnSpeedSample(double inSpeed);
	void OnSlipDetect(BOOL inSlipDetect);
	void OnTransmissionState(char inTransmissionState);
	void OnNightModeChanged(BOOL bNightMode);

	void SetUserSelectAudioSRC(BOOL bUserSelect);
	void SetUserSelectNativeNavi(BOOL bUserSelect);
	void SetUserSelectNativeUI(BOOL bUserSelect);

	void StartIperfServer();
	void UpdateSessionAddress();
	BOOL isSessionAddressValid();
	const wchar_t * GetSessionAddressStr();
	void InputSessionAddress(int c);
	void DeleteSessionAddress(int num);
	u_short GetSessionAddressFamily();
	void StartTCPClient();
	void StartUDPClient(LPCWSTR inBandwidth);
	CAtlStringW& GetIperfReport();
	void ClearIperfReport();
	CAtlStringW& GetIperfResults();
	BOOL NeedShowTopBanner();

	void StartH264Test( const wchar_t* inAVCCPath, const wchar_t* inH264Path );

	void TakeGstResource(BOOL bTake);

	void LimitedUI(BOOL inLimitUI);
	void SetReverseMode(BOOL bOnReverse);
    void SetSystemPowerState(UINT idPowerState);
	void SetBluetoothPhoneStatus(BOOL bActive);
	void SetAccOnWarning(BOOL bShow);

	void AddBluetoothID(const unsigned char mac[6]);
	void AddBluetoothID(const char * macstr);
	void AddBluetoothID(const wchar_t * macstr);
	void RemoveBluetoothID(const unsigned char mac[6]);
	void RemoveBluetoothID(const char * macstr);
	void RemoveBluetoothID(const wchar_t * macstr);
	void ClearAllBluetoothID();
	UINT GetBluetoothIDCount();


	BOOL DeviceSetMac(char* inMACstr);
	BOOL CheckMAC();
	BOOL CheckMACFromFile();
	OSStatus SetMACFromFile(const wchar_t * path);
	BOOL DeleteMAC();
	const wchar_t* GetMAC();

	BOOL DeleteCA();

	OSStatus ConnectTester( void *inBuf, size_t inMaxLen );

	void Restart();

protected:

    virtual DWORD ThreadRun();

private:
    static void CALLBACK ControlStreamsProc(PVOID context, BOOL inControlStreams);
    static void CALLBACK UpdateStreamsProc(PVOID context,
					   BOOL inMainAudioStreams,
					   BOOL inAltAudioStreams,
					   BOOL inMainScreenStreams );

	static void CALLBACK ConnectStateProc(PVOID context,BOOL inConnect);
	static void CALLBACK IAP2ConnectStateProc(PVOID context,BOOL inConnect);
	static void CALLBACK ModesChangedProc(PVOID context, const AirPlayModeState * inState);
	static void CALLBACK RequestUIProc(PVOID context, const char* inURL);
	static void CALLBACK DuckAudioProc(PVOID context,				   double inDurationSecs,
				   double inVolume);
	static void CALLBACK UnduckAudioProc(PVOID context, double inDurationSecs);
	static void DisableBluetoothProc(PVOID context, const unsigned char mac[ 6 ]);
	static void CALLBACK StartLocationInformationProc(PVOID context, const LocationInfo_t* inLocationInfo);
	static void CALLBACK StopLocationInformationProc(PVOID context);
	static void CALLBACK TelephonyAudioFormatProc(PVOID context, AirPlayAudioFormat inFormat);
    static void CALLBACK SpeechAudioFormatProc(PVOID context, AirPlayAudioFormat inFormat);
	static void CALLBACK GetRightHandProc(PVOID context, BOOL* outRightHand);
	static void CALLBACK GetInitialModesProc(PVOID context, AirPlayModeChanges * outInitialModes);
	static void CALLBACK GetBluetoothIDsProc(PVOID context, unsigned char ** outMAC, size_t inMaxMACCount, size_t* outMACCount);
	static OSStatus CALLBACK StoreCaProc( const void * buf, size_t bufsize );
	static OSStatus StoreCa( const wchar_t * path, const void * buf, size_t bufsize );
	static OSStatus InstallCA( const wchar_t * path, BOOL* active );
#if CVTE_EN_CPCA
	static OSStatus InstallCAToCPCA( const wchar_t * path );
	static OSStatus	DeleteCAFromCPCA();
#endif
	static void CALLBACK IperfReportProc( PVOID context, TCHAR* report );
	static void CALLBACK IperfResultsProc( PVOID context, TCHAR* results );
	static void CALLBACK DeviceInformationUpdateProc(PVOID context, LPCWSTR inDeviceName);
	static void CALLBACK DeviceTimeUpdateProc(PVOID context,
  	                                 unsigned long long inSecondsSinceReferenceDate,
  	                                 short inTimeZoneOffsetMinutes,
  	                                 char inDaylightSavingsOffsetMinutes );
	static void CALLBACK DeviceUUIDUpdateProc(PVOID context, LPCWSTR inUUID);
	static BOOL CALLBACK DeviceSetMacProc(PVOID context,unsigned char* inMacData,size_t inMacDataLen);

    CCarplayAdapter();
    CCarplayAdapter(const CCarplayAdapter &);
    CCarplayAdapter& operator = (const CCarplayAdapter &);
    ~CCarplayAdapter();

    void ControlStreams(BOOL inControlStreams);
    void UpdateStreams(BOOL inMainAudioStreams,
					   BOOL inAltAudioStreams,
					   BOOL inMainScreenStreams );

	void ConnectState(BOOL inConnect);
	void IAP2ConnectState(BOOL inConnect);
	void ModesChanged(const AirPlayModeState * inState);
	void RequestUI(const char* inURL);
	void DuckAudio(double inDurationSecs,
				   double inVolume);
	void UnduckAudio(double inDurationSecs);
	void DisableBluetooth(const unsigned char mac[ 6 ]);
	void StartLocationInformation(const LocationInfo_t* inLocationInfo);
	void StopLocationInformation();
	void TelephonyAudioFormat(AirPlayAudioFormat inFormat);
    void SpeechAudioFormat(AirPlayAudioFormat inFormat);
	void GetRightHand(BOOL* outRightHand);
	void GetInitialModes(AirPlayModeChanges * outInitialModes);
	void GetBluetoothIDs(unsigned char ** outMAC, size_t inMaxMACCount, size_t* outMACCount);
	void IperfReport( TCHAR* report );
	void IperfResults( TCHAR* results );
	void DeviceInformationUpdate(LPCWSTR inDeviceName);
	void DeviceTimeUpdate(unsigned long long inSecondsSinceReferenceDate,
  	                      short inTimeZoneOffsetMinutes,
  	                      char inDaylightSavingsOffsetMinutes );
	void DeviceUUIDUpdate(LPCWSTR inUUID);
	BOOL DeviceSetMac(unsigned char* inMacData,size_t inMacDataLen);
	BOOL SetMacToReg(char* inMACstr);
	BOOL SetMacToCPMAC(unsigned char* inMacData,size_t inMacDataLen);

	static void CALLBACK ConnectTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	void ConnectTimer();

    static const CCarplayAdapter* m_pInstance;
    BOOL m_bInitialized;

    wchar_t m_strDeviceName[MAX_PATH];
    BOOL m_bActivated;
	static BOOL m_bStoreCa;
	BOOL m_bServerInit;
    BOOL m_bTestMode;
    CSkinManagerGL *m_pSkinManager;
    HWND m_hMainWnd;
	BOOL m_bOnNightMode;

    BOOL m_ControlStreams;
    BOOL m_MainAudioStreams;
    BOOL m_AltAudioStreams;
    BOOL m_MainScreenStreams;

	list <unsigned char *> m_maclist;
	CLockObject	m_maclock;

    AirPlayModeState m_carplayModeStatus;
    CarplayAPIUSBPort m_CarplayUSBPort;
	DeviceActivateCallback_f m_DeviceActivateCallback;
	SetMACCallback_f m_SetMACCallback;
	PVOID m_DeviceActivateCallbackContext;

	CLocationInfo m_locationinfo;

	BOOL m_bUserSelectAudioSRC;
	BOOL m_bUserSelectNativeNavi;
	BOOL m_bUserSelectNativeUI;
	BOOL m_bFirstTimeConnect;
	BOOL m_bNeedShowTopBanner;

	void * m_tcpserver;
	void * m_udpserver;
	CAtlStringW	m_strIperfReport;
	CAtlStringW	m_strIperfResults;

	CM_String m_SessionAddress;
	BOOL m_isAddressValid;
	BOOL m_isSessionSessionUpDate;
	u_short m_SessionAddressFamily;

	BOOL m_bLimitUI;
	BOOL m_bOnReverseMode;
	BOOL m_bBluetoothPhoneActive;
	UINT m_idPowerState;
	AirPlayModeChanges m_InitialModes;
	BOOL m_bDealIniBluetoothPhone;
	BOOL	m_bShowAccOnWarning;

	BOOL m_bSetMAC;
	HANDLE m_hSetMACEvent;
	CAtlStringW	m_strMAC;

	static const wchar_t * m_DestCAPath1;
	static const wchar_t * m_DestCAPath2;

	static HANDLE m_hcpd;

	MMRESULT m_ConnectTimer;
};

#endif

