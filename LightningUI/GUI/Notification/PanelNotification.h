#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"

class CPanelNotification :
	public CPanelGL
{
public:
	CPanelNotification(void);
	~CPanelNotification(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	BOOL IsShowingNotification(){return m_bShowingNotification;}
	void HideNotification();

	void SetNotification(NBMSG *pMsg);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:

private:
	UINT m_idCaller;
	NOTIFYPROC m_pCallbackFunc;
	LPVOID m_lpCallbackParam;
	UINT m_type;

	BOOL		m_bAutoHide;
	BOOL		m_bShowingNotification;
	UINT			m_idNotificationIcon;
	DWORD	m_dwNotificationShowTime;
	DWORD	m_dwLastNotificationTick;
	CM_String m_strNotification;
	CM_String m_strTitle;

// 	CLabelGL	m_iconInfo;
// 	CLabelGL	m_iconError;
// 	CLabelGL	m_iconSpeech;
// 	CLabelGL	m_iconQuest;

// 	INT		m_idLastUIClass;
	CLabelGL    m_tagBkBlur;

	CButtonGL	m_btOK;
	CButtonGL	m_btCancel;
	CButtonGL	m_btAbort;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif
	/*
	//toolbar:
	CLabelGL			m_iconHomeUp;
	CLabelGL			m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL			m_iconReturnUp;
	CLabelGL			m_iconReturnDown;
	CButtonGL		m_btnReturn;
*/

};
