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

	void OnSystemPowerState(UINT idState);

	void OnSRCChanged(UINT idSRC);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void LinkPages(CPanelBluetoothMain *pMain,CPanelBluetoothTalking *pTalking,CPanelBluetoothA2dp *pA2dp);

	BOOL OnKey(UINT idKey);
	void LinkSmartBarPhone(CSmartBarPhone *pSmartBar){m_pSmartBarPhone = pSmartBar;}
	void LinkSmartBarMedia(CSmartBarMedia *pSmartBar);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnPlaySoundEffect(BOOL bPlay);

	void OnBluetoothCommand(WPARAM wParam, LPARAM lParam);

	void OnVolumeFadeReday(BOOL bMute);
	void OnDelayOperation();

	//
	void OnPinRequest(BTDEV * pbtd);

	BOOL OnActiveBTService(DWORD idServiceActive);
	BOOL OnDeactiveBTService(DWORD idServiceDeactive);

	void OnSearchFinished();
	void OnPairedDeviceListChanged();

	//
	void OnPairFailed();

	void OnHFStateChange(SnmConnectionState state);

	void OnCallStateChange(DWORD state);
	void OnCallInfoChange();

	void OnAudioDirChange(DWORD idDir);

	void OnSignalChange(DWORD dwSignal);
	void OnBatteryChange(DWORD dwBattery);

	void OnPhoneBookDownloadFinish();
	void OnPhoneBookDownloadFailed();

	void OnDeviceConnectFinish(WPARAM wParam);

	void OnNotifyA2dpConnected(BOOL bConnected);
	void OnNotifyAvrcpConnected(BOOL bConnected);
	void OnAvrcpPlayStateChange(AvcmPlayStateT state);
	void OnAvrcpMetaDataChange();
	void OnAvrcpPlayTimeChange();

	void OnAvrcpRepeatModeChange(AvcmPlayAppSettingAttValIdT repeatMode);
	void OnAvrcpShuffleModeChange(AvcmPlayAppSettingAttValIdT shuffleMode);

	void OnA2dpVolumeFade(WPARAM wParam, LPARAM lParam);

	void MuteAudio(BOOL bMute,BOOL bFade,UINT type,BOOL bAutoRelease,UINT time_out);

	void SwitchPower();

	void OnCarPlayMsg(WPARAM wParam, LPARAM lParam);
	void OnA2dpStreamingStatus(BOOL bOn);

public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

	static LRESULT CALLBACK BTWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void AutoDownloadPhonebook();
	void AutoAnswer();
	void UpdatePhonebook();
	void AutoSwitchToA2DP();

	BOOL IsValidCallInfo(const CallInfoT *piCallInfo);

private:
	BOOL IniBTModule();
	BOOL SetBTPower(BOOL bOn);

	void PhoneAnswer1();
	void PhoneAnswer2();
	void PhoneHangup1();
	void PhoneHangup2();
	void PhoneTransferAudio(UINT idDirection=2/*HF_AUDIO_TOGGLE*/);

	//
	BOOL AutoVolumeStepProc();
	void SetA2dpVolume(INT iVolume,BOOL bFade=TRUE);

	//via registry
	void GetDeviceName(LPTSTR lpsz);
	BOOL SetDeviceName(LPTSTR lpsz);
	void GetPIN(LPTSTR lpsz);
	BOOL SetPIN(LPTSTR lpsz);

private:
	void Play();
	void Pause();
	void PlayNext();
	void PlayPrevious();
	void SwitchPlayMode();

	void PreparePause();

	void RepeatOne();
	void RepeatAll();
	void RepeatOff();

	void ShuffleOn();
	void ShuffleOff();

	//power state:
	static DWORD PowerOnThreadPro(LPVOID lpParam);
	static DWORD PowerOffThreadPro(LPVOID lpParam);
	void PowerOn();
	void PowerOff();

	//
	void OnMonitorPlayInfo();

	BOOL OnAutoPowerOn();
	BOOL OnAutoPowerOff();

private:

	CPanelBluetoothMain	*m_ppMain;
	CPanelBluetoothTalking	*m_ppTalking;
	CPanelBluetoothA2dp	*m_ppA2dp;

	HWND			m_hBTWndHidden;

	CSmartBarPhone	*m_pSmartBarPhone;

	//
	const DWORD *m_pdwBTState;	//bt status
	const TCHAR *m_szHFState;						//HF state
	const TCHAR *m_szDUNState;						//DUN state
	const TCHAR *m_szCallState;						//Call state
	const TCHAR *m_szCallNumber;					//calling number
	const CCallRecList *m_pcrlist;					//call records list
	const CDUNNetworkList *m_pdnlist;	//dun network list
	const CallInfoT *m_ciCallInfo1;			//call info 1, For 3way talking
	const CallInfoT *m_ciCallInfo2;			//call info 2, For 3way talking
	const TCHAR *m_szHeldState;				//held state
	const TCHAR *m_szWaitingNumber;			//Waiting Number
	const DWORD *m_pdwTotalNumberOfCalls;	//NumberOfCalls
	/*
	const TpmsTireDataT *m_tdTireL1;		//Tpms TireL1
	const TpmsTireDataT *m_tdTireL2;		//Tpms TireL1
	const TpmsTireDataT *m_tdTireR1;		//Tpms TireL1
	const TpmsTireDataT *m_tdTireR2;		//Tpms TireL1
	*/
	//pbap
	const TCHAR *m_szPBAPState;
	const CContactList *m_pclist;
	//tpms
	const TCHAR *m_szTPMSState;
	//map
	const TCHAR *m_szMAPState;
	const CMessageList *m_pmlist;
	//spp
	const TCHAR *m_szSPPState;
	//a2dp
	const TCHAR *m_szA2DPState;
	//avrcp
	const TCHAR *m_szAVRCPState;
	//avrcp play state
	const TCHAR *m_tcAVRCPPlayState;//Not Implement


	//
	//BT_ADDR	m_addrCurrent;
	//BTDEV * m_pbtdPairing;
	BTDEV * m_pbtdPairRequest;
	BTDEV * m_pbtdAutoConnect;

	AvcmPlayStateT m_PlaySate;

	CALLREC * m_pcr1;
	CALLREC * m_pcr2;

	DWORD	m_dwEnterTick;
	BOOL			m_bSpeechOn;
	BOOL			m_bPlayingBeforeSpeech ;
	BOOL			m_bManualPause;
	//
	BOOL			m_bNeedPause;
	BOOL			m_bNeedUnMute;
	DWORD			m_dwLastUnMuteTick;

	BOOL			m_bBTPowerOn;
	BOOL			m_bBTPowerOnBeforeCarplay;

	SnmConnectionState	m_hfpStatus;
	DWORD		m_dwCurDeviceID;
	DWORD		m_dwCallStatus;
	BOOL			m_bTalking;
	DWORD		m_dwRingTick;
	DWORD		m_dwTalkingTick;
	BOOL			m_bAutoAnswerFlag;
	DWORD		m_dwConnectedTick;
	BOOL			m_bAutoDownloadPhonebookFlag;
	BOOL			m_bCheckA2dpFlag;
	BOOL			m_bPhoneCallActive;

	BOOL			m_bOperatingPower;
	BOOL			m_bChangingAudioDir;
	UINT				m_idAudioDir;
	UINT				m_idAudioDirManualSet;

	//
	INT m_iVolumeCur;
	INT m_iVolumeSet;
	BOOL m_bVolumeFadeReady;

	BOOL	m_bA2dpStreamingOn;
	UINT		m_idSRCBeforeA2dpStreamingOn;
	BOOL	m_bMonitorA2dpStatus;
	BOOL	m_bCarPlayConnected;

};
