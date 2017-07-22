#pragma once
#include "../Common/ui_common.h"

#define DEBUG_VEHICLEINFO				0

class CPanelVehicleInfoAC :
	public CPanelGL
{
public:
	CPanelVehicleInfoAC(void);
	~CPanelVehicleInfoAC(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void SetInfoAirStatus(BYTE iStatus);
	void SetInfoAirWind(BYTE iStatus);
	void SetInfoAirTempDriver(BYTE iStatus);
	void SetInfoAirTempCoDriver(BYTE iStatus);
	void SetInfoAirSeatHeating(BYTE iStatus);

	void SetInfoOutsideTemp(INT iC);

	void SetECONLevel(int iLevel);

	int GetWindLevel(){return m_iWindLevel;}

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
	static void EconTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	void OnEconProc();

private:

	CLabelGL m_tagACBack;

	//
	//CLabelGL m_tagFan;
	CButtonGL m_tagFanBarLeft[7];
	CButtonGL m_tagFanBarRight[7];

	//CLabelGL	m_tagSeatLeft;
	//CLabelGL	m_tagSeatRight;

	CLabelGL m_iconMaxFront;

	CLabelGL m_iconBlowHorizontalLeft;
	CLabelGL m_iconBlowDownLeft;
	CLabelGL m_iconBlowHorizontalRight;
	CLabelGL m_iconBlowDownRight;

	CLabelGL m_iconAQS;
	CLabelGL m_iconBlowUp;
	CLabelGL m_iconBlowRear;
	CLabelGL m_iconSeatHeat0;
	CLabelGL m_iconSeatHeat1;
	CLabelGL m_iconSeatHeat2;
	CLabelGL m_iconSeatHeat3;

// 	CLabelGL m_iconAutoSmall;
// 	CLabelGL m_iconAuotoBig;
	CLabelGL m_iconLoopOut;
	CLabelGL m_iconLoopInner;

// 	CLabelGL m_iconDegree1;
// 	CLabelGL m_iconDegree2;
	CLabelGL m_iconDegree3;

	CLabelGL m_iconTempOutside;
	CButtonGL	m_btnTempOutside;

	CButtonGL	m_btnMaxFront;
	CButtonGL	m_btnRear;
	CButtonGL	m_btnBlowUp;
	CButtonGL	m_btnInnerLoop;

	CButtonGL	m_btnAutoSmall;
	CButtonGL	m_btnAutoBig;
// 	CButtonGL	m_btnACStatus;

	CButtonGL	m_btnSeatDriver;
	CButtonGL	m_btnSeatCoDriver;

	CButtonGL	m_btnPowerStatus;
	//CButtonGL	m_btnDual;
	CButtonGL	m_btnAQS;

	CButtonGL	m_btnTempDriver;
	CButtonGL	m_btnTempCoDriver;

#if ENABLE_ECON_UI
	CButtonGL	m_btnECON;
	CSliderGL		m_sliECON;
#endif

	int				m_iTempDriver;
	int				m_iTempCoDriver;
	int				m_iTempOutside;

	int				m_iWindLevel;
	BOOL			m_bOnDual;
	BOOL			m_bAutoBig;
	BOOL			m_bAutoSmall;

};
