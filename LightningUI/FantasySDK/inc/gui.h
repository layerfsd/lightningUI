#pragma once
//#include "Resource_Fantasy.h"
#include "Common.h"
#include "COM.h"

class CGUI
{
public:
	CGUI(void);
	virtual ~CGUI(void);

public:
	virtual BOOL Initialize(HWND window,MODULEENTRY entry,int iPriority);
	BOOL isActivated();
	HWND GetMainWnd();
	void UpdateFrame();
	void Run();
	void Stop();

	virtual BOOL IsIniUIReady(void)=0;
	virtual void Render(void)=0;
	virtual void testFunc();

	//massage:
	virtual void OnTouchDown(POINT *pt);
	virtual void OnTouchUp(POINT *pt);
	virtual void OnTouchMove(POINT *pt);
	virtual void OnGesture(WPARAM wParam, LPARAM lParam);

	virtual void OnStorageDiskArrived(LPCTSTR lpDeviceName);
	virtual void OnStorageDiskRemoved(LPCTSTR lpDeviceName);
	virtual HRESULT OnHandleGraphEvent(void);
	virtual void OnLanguageChanged(UINT idLanguage);
	virtual void OnFontChanged(UINT idFont);
	virtual void OnVolumeChanged(INT iVolume);
	virtual void OnProcessMCUCmd(BYTE byCmd,LPBYTE lpData,DWORD dwDataLen);
	virtual void OnProcessMCUPackage();
	virtual void OnNaviMsg(UINT iNaviCode);
	virtual void OnBTMsg(UINT iBTMsg);
	virtual void OnQuickSet(UINT iQuickSetCode);
	virtual void OnPanelKey(WPARAM wParam, LPARAM lParam=NULL)=0;
	virtual void OnSystemPowerOFF();
	virtual void OnWallpaperChanged(LPCTSTR pStrFilePath);
	virtual void OperateWidget(UINT idUI,UINT idControl,UINT idOperation,LPVOID lpara);

	void EnableSwap(BOOL bEnable);
	BOOL IsAnimating();

protected:
	DWORD GetFrameTime();
	void SetFrameTime(DWORD dwSet);
	static DWORD GUIRenderThreadProc(LPVOID lparam);
	virtual BOOL IsReady(void)=0;



protected:

	HWND							m_hMainWnd;
	MODULEENTRY			m_entry;
	HINSTANCE					m_hInstance;
	HANDLE						m_hRenderThread;
	HANDLE						m_hRenderEvent;
	DWORD						m_dwGUIRenderThreadID;
	DWORD						m_dwFrameTime;
	BOOL							m_bRunning;
	BOOL							m_bEnableSwap;
	BOOL							m_bAlive;
	BOOL							m_bAnimating;
	BOOL							m_bTouched;
};
