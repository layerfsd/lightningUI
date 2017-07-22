#pragma once
#include "../Common/ui_common.h"

class CPanelVehicleInfoBody :
	public CPanelGL
{
public:
	CPanelVehicleInfoBody(void);
	~CPanelVehicleInfoBody(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);


	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	//msg
	void SetInfoTrunk(BOOL bNormal);
	void SetInfoDoorFL(BOOL bNormal);
	void SetInfoDoorFR(BOOL bNormal);
	void SetInfoDoorRL(BOOL bNormal);
	void SetInfoDoorRR(BOOL bNormal);

	//TPMS:
	void SetTpmsPressure(UINT idTire,const wchar_t* pValue);
	void SetTpmsTemper(UINT idTire,const wchar_t* pValue);
	void SetTpmsAlarm(UINT idTire,UINT idAlarmType);
	void SetTpmsSensorID(UINT idTire,DWORD idSensorID);
	void CancelTpmsAlarm(UINT idTire);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	CLabelGL m_tagCar;

	CLabelGL m_tagDoorFL;

	CLabelGL m_tagDoorFR;

	CLabelGL m_tagDoorRL;

	CLabelGL m_tagDoorRR;

	CLabelGL m_tagDoorTrunk;

#if ENABLE_TPMS_UI
	CButtonGL	m_tagPressure[5];
	CButtonGL	m_tagTemper[5];
	CButtonGL m_btnTire[5];
	CLabelGL m_tagTireBack[5];
	UINT		m_idAlarmType[5];
	DWORD m_dwAlarmTick[5];
	DWORD m_dwSensorID[5];
	UINT m_idTireSwitch0;
	UINT m_idTireSwitch1;
	BOOL	m_EnableTireSwitch;
#endif

};
