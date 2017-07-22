#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"


class CPanelNaviSimulate :
	public CPanelGL
{
public:
	CPanelNaviSimulate(void);
	~CPanelNaviSimulate(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void OnAndroidAutoMsg(WPARAM wParam, LPARAM lParam);
protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	static void CALLBACK QuitVoiceNotificationTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	static void CALLBACK SimGuidanceVoiceTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

private:
	CButtonGL	m_btnSimGuidanceVoice;

	CButtonGL	m_btnPlayNotificationAllowDuck;
	CButtonGL	m_btnPlayNotificationNotAllowDuck;

	BOOL m_bPlayingGuidanceVoice;
	MMRESULT m_timerVoiceNotification;
	MMRESULT m_timerSimGuidanceVoice;
};
