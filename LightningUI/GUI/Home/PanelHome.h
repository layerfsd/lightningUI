#pragma once
#include "PanelWidget.h"


class CPanelHomePage :
	public CPanelGL
{
public:
	CPanelHomePage(void);
	~CPanelHomePage(void);

	BOOL IsReady();
	BOOL IsAniReady(void);
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnHome();
	void OnBack();

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGestureMsg(UINT idGesture);

	void OnGesture(GESTUREINFO *pgi);

	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

	void OperateWidget(UINT idUI,UINT idControl,UINT idOperation,LPVOID lpara);

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif

	void OnWidgetChanged(WPARAM wParam, LPARAM lParam);
	void OnCanbusStatus(WPARAM wParam, LPARAM lParam);

	CWidgetGL*	GetWidget(UINT idWidget);

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
private:
	void InitWidgetCallback();
	static void CALLBACK CallbackProc(UINT nPageNum, PVOID pContext);
	void ChangePageSelectIcon(UINT nPageNum);
	void SetPageNum(UINT nPageNum);
	void UpdateTime();
#endif
private:

	UINT		m_idCurSubpage;

	CPanelWidget		m_panelWidget;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CButtonGL		m_tagButtonStatusBar;
	CLabelGL		m_iconPage;
	CLabelGL		m_iconPageFocus;
	CButtonGL		m_btnPageSelect1;
	CButtonGL		m_btnPageSelect2;
	UINT			m_ibtnIDPageSelect1;
	UINT			m_ibtnIDPageSelect2;
	UINT			m_ibtnIDStatusTime;
	UINT			m_iPageSelectNum;
	
	CButtonGL	m_tagAM;
	CButtonGL	m_tagTime;
#endif
	CLabelGL		m_tagBkOrigin;

	BOOL			m_bPhoneCallActive;

	BOOL			m_bHasCanbus;

};
