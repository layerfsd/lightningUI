#pragma once
#include "../Common/ui_common.h"

class CPanelKeyboard :
	public CPanelGL
{
public:
	CPanelKeyboard(void);
	~CPanelKeyboard(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void SetNotification(KEYBOARDMSG *pMsg);

protected:

private:
	CKeyBoardGL	m_Keyboard;

	UINT m_idCaller;
	NOTIFYPROC m_pCallbackFunc;
	LPVOID m_lpCallbackParam;
	UINT m_type;
	BOOL m_bAutoReturn;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif
};
