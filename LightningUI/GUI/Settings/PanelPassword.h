#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"

#define PASSWORD_MAX_LENGTH 4

class CPanelPassword :
	public CPanelGL
{
public:
	CPanelPassword(void);
	~CPanelPassword(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	BOOL IsShowing(){return m_bShowing;}
	void Hide();
    void SetNotification(NBMSG *pMsg);
    BOOL IsActivate();
	void SetCmd(WPARAM wParam, LPARAM lParam);
	
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:

private:
    void Input(wchar_t c);
    void ClearInput(int iNumChar);
    BOOL VerifyPassword(LPCTSTR lpPassword);
    void SetPassword(LPCTSTR lpPassword);
    static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	void LockPasswordBox(BOOL bLock);

private:
	UINT m_idCaller;
	NOTIFYPROC m_pCallbackFunc;
	LPVOID m_lpCallbackParam;
	UINT m_type;

	BOOL		m_bAutoHide;
	BOOL		m_bShowing;
	BOOL		m_bUnLock;
	UINT			m_idNotificationIcon;
	DWORD	m_dwNotificationShowTime;
	DWORD	m_dwLastNotificationTick;
	CM_String m_strNotification;
	CM_String m_strTitle;

    CM_String   m_strInput;
    CM_String   m_strPassword;
	CLabelGL    m_tagBkBlur;
	BOOL        m_bVerifyRight;
    BOOL        m_btagNumberInput[PASSWORD_MAX_LENGTH];
    CButtonGL   m_tagPasswordTitle;
    CButtonGL   m_tagNumber[PASSWORD_MAX_LENGTH];
    CButtonGL	m_btnNumber[10];
    CButtonGL   m_btnReturn;

	/*
	//toolbar:
	CLabelGL			m_iconHomeUp;
	CLabelGL			m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL			m_iconReturnUp;
	CLabelGL			m_iconReturnDown;
	CButtonGL		m_btnReturn;
*/
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif
};
