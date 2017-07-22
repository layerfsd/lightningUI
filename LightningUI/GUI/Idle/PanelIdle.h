#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"

#define IDLE_SCREEN_NONE						0
#define IDLE_SCREEN_ANALOG_CLOCK		1
#define IDLE_SCREEN_DIGITAL_CLOCK			2

class CPanelIdle :
	public CPanelGL
{
public:
	CPanelIdle(void);
	~CPanelIdle(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:

private:
	CClockBarGL		m_barClock;

	UINT		m_idType;

};
