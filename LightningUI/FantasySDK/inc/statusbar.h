#pragma once
#include "controlgl.h"
#include "ButtonGL.h"
#include "SliderGL.h"

class CStatusBar :
	public CControlGL
{
public:
	CStatusBar(void);
	~CStatusBar(void);


	void SetCurDisplayPanel(UINT idPanel){m_idCurDisplayPanel=idPanel;}
	UINT GetCurDisplayPanel(){return m_idCurDisplayPanel;}
	void SetCurUIClass(UINT idClass){m_idUIClass=idClass;}
	UINT GetCurUIClass(){return m_idUIClass;}

	virtual void SetTitle(const wchar_t *pTitle)=0;
	virtual void SetBTStatus(DWORD dwStatus,DWORD dwSignal,DWORD dwBattery)=0;
	virtual void SetWiFiStatus(WIFI_STATUS status)=0;
	virtual void SetWiFiPowerON(BOOL bOn)=0;
	virtual void SetAFStatus(RDS_STATUS status)=0;
	virtual void SetTAStatus(RDS_STATUS status)=0;
    virtual void SetTimeMode(UINT iTimeMode)=0;

protected:
	UINT m_idCurDisplayPanel;
	UINT	 m_idUIClass;

};
