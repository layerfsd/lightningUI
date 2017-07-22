#pragma once
#include "../Common/ui_common.h"

#include "../../Communication/CommanderEx.h"
#include "../Controls/RadarBar.h"
#include "../../VIP/CVideoWindow.h"
#include "../../Communication/CmdSender_CanBus.h"

#include "PanelVehicleInfoDrive.h"
#include "PanelVehicleInfoAC.h"
#include "PanelVehicleInfoBody.h"
#include "PanelVehicleInfoRadar.h"

#define KEY_VCHICLE_NULL					0x00
#define KEY_VCHICLE_VOL_ADD			0x01
#define KEY_VCHICLE_VOL_SUB			0x02
#define KEY_VCHICLE_UP						0x03
#define KEY_VCHICLE_DOWN				0x04
#define KEY_VCHICLE_TEL						0x05
#define KEY_VCHICLE_MUTE					0x06
#define KEY_VCHICLE_SRC					0x07

#define KEY_STATUS_UP					0x00
#define KEY_STATUS_DOWN				0x01
#define KEY_STATUS_HOLD				0x02

#define PARKING_ASSI_RADAR_AND_REARVIEW			0x01
#define PARKING_ASSI_RADAR										0x02
#define PARKING_ASSI_REARVIEW								0x03


class CPanelVehicleInfoWrapper :
	public CPanelGL
{
public:
	CPanelVehicleInfoWrapper(void);
	~CPanelVehicleInfoWrapper(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnReverseModeChange(BOOL bReverse);

	void OnSystemPowerStatePrepare(UINT idState);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void OnGestureMsg(UINT idGesture);

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnSRCChanged(UINT idSRC);

	void ProcessCanbusPack(LPBYTE pCmd,DWORD dwCmdLen);
	void ProcessTpmsPack(LPBYTE pCmd,DWORD dwCmdLen);

	void Connect(BOOL bConnect);

	void NotifySourceInfo(BYTE iSourceId,BYTE iMediaType);
	void NotifyRadioInfo(BYTE iBand,WORD iFreq,BYTE iPreset,BOOL bAlwaysSet=FALSE);
	void NotifyVolumeInfo(INT iVol, BOOL bMute);
	void NotifyMediaPlaybackInfo(DWORD dwData1, DWORD dwData2);

	void OnTimerProc();
	void OnGPSGroundSpeed(INT iKmh);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:
	void OnInfoSpeed(UINT iSpeedKmh);
	void OnInfoKey(INT idKey,INT idStatus);

	//common:
	void OnInfoCommon(BYTE iInfo);

	//main panel:
	void OnInfoBelt(BOOL bNormal);
	void OnInfoWashWater(BOOL bNormal);
	void OnInfoHandBrake(BOOL bNormal);
	void OnInfoTrunk(BOOL bNormal);
	void OnInfoDoorFL(BOOL bNormal);
	void OnInfoDoorFR(BOOL bNormal);
	void OnInfoDoorRL(BOOL bNormal);
	void OnInfoDoorRR(BOOL bNormal);

	void OnInfoEnginSpeed(INT iRPM);
	void OnInfoInstantVel(INT iKmh);
	void OnInfoBattery(INT iV);
	void OnInfoOutsideTemp(INT iC);
	void OnInfoTrip(INT iKm);
	void OnInfoOil(INT iL);

	void OnInfoAlertOil(BOOL bAlert);
	void OnInfoAlertBettery(BOOL bAlert);

	//radar:
	void OnInfoRadar(INT id,INT iSignal);
	void OnInfoParkingAssi(BYTE iStatus);

	//air:
	void OnInfoAirStatus(BYTE iStatus);
	void OnInfoAirWind(BYTE iStatus);
	void OnInfoAirTempDriver(BYTE iStatus);
	void OnInfoAirTempCoDriver(BYTE iStatus);
	void OnInfoAirSeatHeating(BYTE iStatus);

	//steer:
	void OnInfoSteerWheel(INT iAngle);

	//TPMS:
	void OnInfoTpmsPressure(UINT idTire,const wchar_t* pValue);
	void OnInfoTpmsTemper(UINT idTire,const wchar_t* pValue);
	void OnInfoTpmsAlarm(UINT idTire,UINT idAlarmType);
	void QueryTpmsSensorID(UINT idTire=-1);

	void OnTpmsSwitchTire(UINT id1,UINT id2);

private:
	void ProcessCanbusLine(LPBYTE pCmd,DWORD dwCmdLen);
	void SendCanbusCommand(BYTE *pBuff,INT  iLen,INT iSendDelay=0);

	void ProcessTpmsLine(LPBYTE pCmd,DWORD dwCmdLen);
	void SendTpmsCommand(BYTE *pBuff,INT  iLen,INT iSendDelay=0);

	void SendCanbusACK(BYTE ack);

	void RequestVchicleBodyInfo(BYTE idType);
	void RequestSpeedInfo();
	void RequestACInfo();

	void ShowPage(UINT idPage);
	void ShowQuickACPage(UINT iTimeOut=3000);
	void ShowQuickBodyPage(UINT iTimeOut=3000);
	void ShowQuickRadarPage(UINT iTimeOut=3000);

	//
private:
	UINT		m_idCurSubpage;

	//CPanelVehicleInfoSpeed  m_panelSpeed;
	CPanelVehicleInfoDrive  m_panelDrive;
	CPanelVehicleInfoAC  m_panelAC;
	CPanelVehicleInfoBody  m_panelBody;
	CPanelVehicleInfoRadar  m_panelRadar;

	CLabelGL		m_iconHomeUp;
	CLabelGL		m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL		m_iconSpeed;
	CLabelGL		m_iconSpeedFocus;
	CLabelGL		m_iconDoors;
	CLabelGL		m_iconDoorsFocus;
	CLabelGL		m_iconAC;
	CLabelGL		m_iconACFocus;
	CLabelGL		m_iconBodyInfo;
	CLabelGL		m_iconBodyInfoFocus;

	CListBoxGL		m_boxMenu;

	//
	BOOL							m_bConnecting;
	BOOL							m_bHasCanBus;

	BOOL							m_bIniQueryData;

	DWORD						m_dwLastShowACTick;
	DWORD						m_dwLastShowBodyTick;
	DWORD						m_dwLastShowRadarTick;

	DWORD						m_dwShowACTimeout;
	DWORD						m_dwShowBodyTimeout;
	DWORD						m_dwShowRadarTimeout;

// 	BOOL							m_bQuickShowACPage;
// 	BOOL							m_bQuickShowBodyPage;
// 	BOOL							m_bQuickShowRadarPage;
	UINT								m_idQuickShowPage;
	UINT								m_idLastPage;

	CmdSender_CanBus	*m_pCmdSender;
	HANDLE			m_hACKEvent;

    UINT            m_uRadarData;
    DWORD           m_dwLastTick;
    HANDLE          m_hRadarInfoEvent;

	BOOL				m_bQuickMode;

	BOOL				m_bRadarOn;
	BOOL				m_bAssiOn;
	DWORD			m_dwAssiNotifyTick;
	BOOL				m_bAutoShowRadar;

	DWORD			m_dwEnterTick;

	//
	BYTE		m_doorStatus;

	//volume info:
	INT		m_iVolSendToCanbus;

	//TPMS:
	BOOL	m_bGotTpmsSensorID;
	char m_strTpmsSensorId[5][16];
	DWORD m_dwTpmsAlarmTick;
	DWORD m_dwShowTpmsAlarmTick;

	BOOL m_bSpeechOn;
	BOOL m_bPhoneCallActive;

};
