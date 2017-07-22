#pragma once
#include "../../stdafx.h"
#include "../Common/ui_common.h"
#include <CSRMedia.h>


class CSndPlayer
{
public:
	CSndPlayer(HINSTANCE hIns, ICSRMedia* media, const wchar_t *filename, BOOL bLoop, DWORD dwWaveId, DWORD dwVol);
	~CSndPlayer(void);

	void start();
	void stop();

private:
	static DWORD playThreadPro(LPVOID lpParam);
	static LRESULT CALLBACK	notifyWndProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT OnPlayerMsg(WPARAM wParam, LPARAM lParam);

private:
	HWND		m_hNotifyWnd;
	ICSRMedia* m_pCSRMedia ;
	ICSRPlayer* m_pPlayer ;
	ICSREventSource* m_pEvent ;
	ICSRWaveoutAudio* m_pAudio;

	BOOL m_bPlaying;
	BOOL m_bLoop;
	DWORD m_dwVol;
	DWORD m_dwWavID;
	CM_String m_strFile;

};
