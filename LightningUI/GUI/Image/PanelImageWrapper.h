#pragma once
#include "PanelImageLibrary.h"
#include "PanelImageMain.h"


class CPanelImageWrapper :
	public CPanelGL
{
public:
	CPanelImageWrapper(void);
	~CPanelImageWrapper(void);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void LinkPages(CPanelImageMain *pMain,CPanelImageLibrary *pLibrary);

	BOOL OnImagePlayerCommand(WPARAM wParam, LPARAM lParam);

	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
 	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);

	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	BOOL PlayIndex(INT index);
	BOOL PlayFile(const wchar_t* pFileName);
	BOOL PlayNext();
	BOOL PlayPrevious();
//	    void PlayZoomMinus();
//	    void PlayZoomPlus();

private:
	CPanelImageLibrary	*m_ppLibrary;
	CPanelImageMain		*m_ppMain;

	UINT				m_idCurDeviceID;
};

