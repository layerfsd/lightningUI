#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"

#define DEBUG_RADARBAR		0

#define RADAR_COUNT			8

class CRadarBar :
	public CControlGL
{
public:
	CRadarBar(void);
	~CRadarBar(void);

	BOOL IsReady();

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void SetRadarSingal(INT index,INT iSingal);


private:


	CLabelGL			m_tagBarBack;
	CLabelGL			m_tagBarBack2;

	CLabelGL			m_tagRadarBack;
	CLabelGL			m_tagRadarBar[RADAR_COUNT];

	int					m_iRadarSingal[RADAR_COUNT];


};
