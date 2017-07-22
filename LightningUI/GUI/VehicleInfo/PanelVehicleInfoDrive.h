#pragma once
#include "../Common/ui_common.h"

#define MAX_SPEED_SAMPLE		600

class CPanelVehicleInfoDrive :
	public CPanelGL
{
public:
	CPanelVehicleInfoDrive(void);
	~CPanelVehicleInfoDrive(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	//msg
	void SetInfoBelt(BOOL bNormal);
	void SetInfoWashWater(BOOL bNormal);
	void SetInfoHandBrake(BOOL bNormal);

	void SetInfoEnginSpeed(INT iRPM);
	void SetInfoInstantVel(INT iKmh);
	void SetInfoBattery(INT iV);
	void SetInfoOutsideTemp(INT iC);
	void SetInfoTrip(INT iKm);
	void SetInfoOil(INT iL);

	void SetInfoAlertOil(BOOL bAlert);
	void SetInfoAlertBettery(BOOL bAlert);

	//
	void testFun();
	void OnGPSGroundSpeed(INT iKmh);


private:
	BOOL SampleSpeed(INT iSpeed);
	void ClearSpeedData();

private:

	GLfloat				*m_pCurveVertex;
	int					m_iSpeedSempleCount;
	DWORD			m_dwSpeedSampleTick;

	CDashboardGL m_barSpeed;
	CDashboardGL m_barEngin;

	CLabelGL m_tagGridBack;

	INT m_iEnginSpeed;
	INT m_iInstantVel;
	BOOL m_bCheckDashboard;

	//
	CLabelGL m_iconHandbrakeAlert;
	CLabelGL m_iconWashWaterAlert;
	CLabelGL m_iconBeltAlert;

	CLabelGL m_iconBattery;
	CLabelGL m_iconBatteryAlert;

	CLabelGL m_iconOil;
	CLabelGL m_iconOilAlert;

	CLabelGL m_iconTrip;

// 	CListBoxGL  m_box1;
// 	CListBoxGL  m_box2;

	//
	CButtonGL m_btnBattery;
	CButtonGL m_btnTrip;
	CButtonGL m_btnOil;
//
	BOOL		m_bSamplingSpeed;
	BOOL		m_bGetSpeedAccData;

	CButtonGL m_btnSpeedData;

};
