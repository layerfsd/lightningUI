#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"

class CPanelScreenSaver :
	public CPanelGL
{
public:
	CPanelScreenSaver(void);
	~CPanelScreenSaver(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0);
	//void OnQuitPanel(UINT iParam=0,UINT idAni=0){}
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


};
