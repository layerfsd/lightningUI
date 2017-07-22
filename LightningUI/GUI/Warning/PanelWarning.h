#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"

class CPanelWarning :
	public CPanelGL
{
public:
	CPanelWarning(void);
	~CPanelWarning(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

	void SetShowTime(UINT nShowTime){m_nShowTime = nShowTime;}
	void OnReverseModeChange(BOOL bReverse);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:

private:

	CLabelGL    m_tagBkBlur;

	CButtonGL	m_btnAgree;

	DWORD       m_dwWarningShowTick;

	UINT		m_nShowTime;

};
