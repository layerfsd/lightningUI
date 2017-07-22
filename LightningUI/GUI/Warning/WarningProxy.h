#pragma once
#include <windows.h>
#include "../Controls/SmartBarWarn.h"

class CWarningProxy
{
public:
	static CWarningProxy* GetInstance();
	BOOL Initialize(CSmartBarWarn* pSmartBarWarn, HWND hWnd);

	void StartShowWarningBar(UINT nShowTime);
	void StopAndHideWarningBar();
	void PauseShowWarningBar();
	void ResumeShowWarningBar();

	void EnterWarningPage(UINT nShowTime);
	void LeaveWarningPage();

	void  OnTouchDown(POINT *pt);
	void  OnTouchUp(POINT *pt);
	void  OnTouchMove(POINT *pt);

	BOOL IsShowWarningSmartBar();
	BOOL IsShowWarningPage();

protected:

private:
	static void WarningBarTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	void DoWarningBarTimerEnd();

	CWarningProxy(void);
	~CWarningProxy(void);

private:
	static const	CWarningProxy* m_pInstance;

	HWND			m_hWnd;
	CSmartBarWarn	*m_pSmartBarWarn;
	MMRESULT		m_SmartBarWarnTimer;

	DWORD			m_SmartBarWarnShowTick;

	UINT			m_nShowPageCount;
	UINT			m_nShowBarTime;

	BOOL			m_bShowWarningSmartBar;
	BOOL			m_bShowWarningPage;

#if CVTE_EN_CARPLAY
	BOOL			m_bCarPlaySetAccOnWarning;
#endif
};
