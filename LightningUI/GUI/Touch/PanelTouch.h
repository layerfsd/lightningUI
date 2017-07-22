#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../Settings/SettingCommon.h"


#define DEBUG_TOUCH			1

typedef  UINT TOUCHTESTSTATE;
#define touchteststateidle      0
#define touchteststatebutton    1
#define touchteststatefreedraw  2

class CPanelTouch :
	public CPanelGL
{
public:
	CPanelTouch(void);
	~CPanelTouch(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
//		void OnTouchDown(POINT *pt);
//		void OnTouchUp(POINT *pt);
//		void OnTouchMove(POINT *pt);
    void OnFactoryCmd(WPARAM wParam, LPARAM lParam);
    BOOL LaunchTouchPro();

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
    BOOL CreateTouchWindow(LPTSTR szWindowClass);
    static LRESULT CALLBACK TouchWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    void ShowTouchWindow(BOOL bShow);


private:
    CButtonGL m_btnQuit;
    CButtonGL m_btnTestP1;
    CButtonGL m_btnTestP2;
    CButtonGL m_btnTestP3;
    CButtonGL m_btnTestP4;
    CButtonGL m_btnTestP5;

    POINT     m_Point;
    UINT      m_iFactoryTestType;
    UINT      m_iFactoryTestCmd;
    UINT      m_iTestState;

	HWND      m_hTouchWnd;
    POINT     m_ptOrigin;
    BOOL      m_bDraw;
    BOOL      m_bAppLaunched;
    BOOL      m_bTouchWindowVisible;

};
