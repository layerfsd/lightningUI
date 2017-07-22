#pragma once
#include "PanelBluetoothMain.h"
#include "PanelBluetoothTalking.h"
#include "PanelBluetoothA2dp.h"


class CPanelBluetoothWrapper :
	public CPanelGL
{
public:
	CPanelBluetoothWrapper(void);
	~CPanelBluetoothWrapper(void);

	void OnSystemPowerStatePrepare(UINT idState);

	void OnSRCChanged(UINT idSRC);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void LinkPages(CPanelBluetoothMain *pMain,CPanelBluetoothTalking *pTalking,CPanelBluetoothA2dp *pA2dp);

	void OnBluetoothMsg(WPARAM wParam, LPARAM lParam);

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif
	void LinkSmartBarPhone(CSmartBarPhone *pSmartBar){m_pSmartBarPhone = pSmartBar;}
	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar);

	void OnSpeechStart();
	void OnSpeechTerminate();

	void OnBluetoothCommand(WPARAM wParam, LPARAM lParam);

	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);

	void OnVolumeFadeReday(BOOL bMute);


	//BT:
	BOOL IniBTModule();
	BOOL SetBTPower(BOOL bOn);
	BOOL IsBTModuleIni();
	BOOL IsDevicePaired(LPTSTR strDevAddr);
	BOOL IsPlaying();
	void RegisterPairRequestCallback(callbackBluetoothPairReques pairCallback, LPVOID para);
	void OnPairRequestFromAndroidAuto(LPTSTR strDevAddr);
	void OnAndriodAutoConnectStatus(BOOL bConnect);

	BOOL SetDeviceName(LPTSTR lpsz);
	BOOL SetPIN(LPTSTR lpsz);

	BOOL SetA2dpVolume(DWORD dwVol,BOOL bFade=TRUE);
	void OnMonitorA2dpStatus();

	//events:
	void OnIniStatus(BOOL bSucc);
	void OnDeiniStatus(BOOL bSucc);
	void OnSignalChange(DWORD dwSignal);
	void OnBatteryChange(DWORD dwBattery);
	void OnNotifyHfpConnected(BOOL bConnected);
	void OnNotifyA2dpConnected(BOOL bConnected);
	void OnNotifyAvrcpConnected(BOOL bConnected);
	void OnNotifyDunServiceSupport(BOOL bSupport);
	void OnNotifyPanServiceSupport(BOOL bSupport);
	void OnNotifyPairResult(PAIR_RESULT result);
	void OnNotifyDeviceDelete(LPCTSTR name);
	void OnNotifyCallCount(DWORD dwCount);
	void OnNotifyCallStatus(strCallStatus* callStatus);
	void OnNotifyAudioTransferMode(AUDIO_TRANSFER_MODE mode);
	void OnNotifyPBSyncState(PB_SYNC_STATE state);
	void OnNotifyAudioSourceState(AUDIO_SOURCE src,AUDIO_STATE state);
	void OnNotifyA2dpPlayState(PLAYER_STATE state);
	void OnNotifyDeviceListChanged();
	void OnNotifyDeviceInquiryFinish();
	void OnNotifyCallLogChanged();
	void OnNotifyPhoneBookChanged();
	void OnNotifyID3(strMusicID3 *id3info);
	void OnNotifySetupInI(strSetupInI *ptIni);
	void OnNotifyPowerStatus(BOOL bOn);
	void OnNotifyModuleName(LPCTSTR name);
	void OnNotifyModuleVersion(LPCTSTR version);
	void OnNotifyMicState(MICPHONE_MUTE_STATE state);
	void OnNotifyAvPlayTime(strAvPlayStatus *info);

	//
private:
	void PlayNext();
	void PlayPrevious();
	void PlayFF();
	void PlayRW();
	void Play();
	void Pause();
	void PlayPause();
	void PreparePause();
	void PreparePlay();

	void EnableA2DP(bool bEnable, const char* iReason);
public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	static void BTTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	static void CoCallBack(UINT eventID, UINT _DataID, WCHAR* _data, UINT _dataLen);
	static DWORD AutoChangeAudioDirThreadPro(LPVOID lpParam);

#if BT_PHONEBOOK_AUTOSTORE
private:
	void SaveContactToFile(ContactInfo *contact, FILE *fp);
	int LoadStoredPhonebook(const wchar_t *strDevAddr);
#endif

private:

	CPanelBluetoothMain	*m_ppMain;
	CPanelBluetoothTalking	*m_ppTalking;
	CPanelBluetoothA2dp	*m_ppA2dp;

	CSmartBarPhone	*m_pSmartBarPhone;

	BOOL					m_bInitializing;
	DWORD				m_dwInitializeTick;
	DWORD				m_dwConnectTick;

	BOOL					m_bSpeechOn;
	BOOL					m_bCarPlayConnected;

	BOOL					m_bBTManualPowerOn;
	BOOL					m_bBTPowerOn;
	BOOL					m_bBTPowerOnBeforeCarplay;
	BOOL					m_bBTPowerOnTarget;

	BOOL					m_bCurSrcEnableA2dp;
	CM_String				m_strBTName;
	CM_String				m_strBTVersion;
	BOOL		m_bAutoDownloadPhonebookFlag;
	BOOL		m_bRefrehPhonebookFlag;
	BOOL		m_bPhonebookDownloading;

	BOOL	m_bAutoUnmuteMic;

	BOOL		m_bA2dpVolumeOn;
	DWORD	m_dwA2dpVolumeOnTick;
	UINT				m_idSRCBeforeA2dpOn;

	BOOL					m_bMediaOnMute;

	BOOL		m_bA2dpMonitoring;

	BOOL		m_bPreparePlay;
	BOOL		m_bPreparePause;


	BOOL		m_bHfpConnected;
	BOOL		m_bA2dpConnected;
	DWORD		m_dwA2dpAutoConnectCnt;
	WCHAR    m_strCurDeviceID[8];
	BOOL		m_bSetModuleName;
	BOOL		m_bGetModuleInfo;
	BOOL		m_bGetIniSettingValue;
	BOOL		m_bGetLocalAdress;

	//bt parameter:
	int         m_ServerNum;
	bool	    bisInit;
	bool		pbstate;
	bool        bisneedplay;
	bool        bMute;
	LONG		m_phonebook;
	TCHAR		m_tempbook[8][20];
	INT         m_enumOppAcceptMethod;
	char*       ServerResult;
	HfPBInfo	m_pbinfo;
	HfPBEntry	m_pbentry;
	AvStatus a2dpStatus;
	PLAYER_STATE playStatus;
	HfAudioStatus AudioStatus;
	HfPhoneBookStorage m_storage;
	HfCallSetupStatus CallStatus[2];
	HfConnectStatus ConnectStatus;
	DWORD m_dwSignal;
	DWORD m_dwBattery;
	MICPHONE_MUTE_STATE m_micState;
	std::vector<strDeviceInfo>* m_pvInquire;
	std::vector<strDeviceInfo>* m_pvPaired;
	std::vector<ContactInfo>* m_pvContacts;
	std::vector<strCallLog>*  m_pvAllCalls;
	std::vector<strCallLog>*  m_pvDialCalls;
	std::vector<strCallLog>*  m_pvMissCalls;
	std::vector<strCallLog>*  m_pvReceivCalls;
	strRequestDeviceInfo	m_strRequestDeviceInfo;
	DWORD		m_dwCallStatus;
	BOOL		m_bManualPlay;
	WCHAR m_strConnectedDevAddr[20];
	WCHAR m_strConnectedDevID[8];
	WCHAR m_strConnectedDevName[34];
#if BT_PHONEBOOK_AUTOSTORE
	CM_List<ContactInfo>	m_listStoredContacts;
	BOOL		m_bNeedLoadStoredContact;
#endif

	callbackBluetoothPairReques m_pairRequestCallback;
	LPVOID m_paraPairRequestCallback;
	BOOL m_bAndroidAutoConnect;
	WCHAR m_strAndroidAutoDevAddr[20];
};
