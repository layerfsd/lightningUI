#pragma once
#include "../Common/ui_common.h"
#include "../Controls/RadarBar.h"


class CPanelVehicleInfoRadar :
	public CPanelGL
{
public:
	CPanelVehicleInfoRadar(void);
	~CPanelVehicleInfoRadar(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void SetInfoRadar(INT id,INT iSignal);

private:
	CRadarBar m_barRadar;


};
