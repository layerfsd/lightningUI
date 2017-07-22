//CSmartBar.h : interface of the CSmartBar class
//

#pragma once
#include <FantasyLib.h>
#include <ddraw.h>
#include "../Common/ui_common.h"

#define DEBUG_SMARTBAR						0

class CSmartBar
{
public:
	CSmartBar();

public:
	int    m_iOverlayX,m_iOverlayY;
	int     m_iOverlayWidth, m_iOverlayHeight;

protected:

	UINT m_id;
	HWND m_hWnd;
	HWND m_hMainWnd;


	int    m_iWndX, m_iWndY;
	int     m_iWndWidth, m_iWndHeight;

	LPDIRECTDRAW						m_pDD;        // DirectDraw object
	LPDIRECTDRAWSURFACE         m_pDDSPrimary ; // Primary Surface.
	LPDIRECTDRAWSURFACE         m_pDDSOverlay ; // The overlay primary.
	LPDIRECTDRAWSURFACE         m_pDDSBack ; // The overlay primary.


	//LPDIRECTDRAWSURFACE         m_pDDSCurrVPSurf;     // Current surface that is written by video port
	DDCAPS						m_ddcaps;
	DDOVERLAYFX     m_ovfx;
	DWORD m_dwUpdateFlags;

	BOOL		m_bVisible;

	BOOL			m_bEnableShow;
	BOOL			m_bNeedUpdateFrame;
	DWORD		m_dwLastShowTick;
	DWORD		m_dwShowTimeOut;

	POINT			m_ptLastPress;

	//
	BOOL			m_bOverlayOnly;

	BOOL			m_bClickActivate;

public:
	virtual BOOL Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns,int pxOverlay,int pyOverlay,int cxOverlay,int cyOverlay,BOOL bOverlayOnly,
		LPCTSTR lpWndName,int pxWnd=0,int pyWnd=0,int cxWnd=0,int cyWnd=0);
	virtual BOOL Show(BOOL bShow,UINT iTimeOut=5000);
	BOOL IsVisible(){return m_bVisible;}
	void EnableShow(BOOL bEnable);
	DWORD GetShowTime(){return m_dwShowTimeOut;}
	DWORD GetLastShowTick(){return m_dwLastShowTick;}

protected:
	virtual void Render()=0;
	virtual BOOL LoadRes(LPCTSTR lpFileName)=0;
	virtual void   ReleaseRes()=0;
	virtual void DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si)=0;
	virtual void OnTouchDown(POINT *pt)=0;
	virtual void OnTouchUp(POINT *pt)=0;
	virtual void OnTouchMove(POINT *pt)=0;

protected:
	BOOL OnTimeOut();

	static LRESULT CALLBACK	SmartBarWndProc(HWND, UINT, WPARAM, LPARAM);
	BOOL OnUpdateFrame();

	BOOL InitializeOverlay();
	BOOL InitializeDDraw();

	VOID   ReleaseAll();
	VOID   ReleaseDDraw();
	BOOL   ReleaseOverlay();

	// Overrides
	BOOL FillSurface(LPDIRECTDRAWSURFACE *pSurface,const unsigned char *pBuff,int cx,int cy,int depth);
	void ReleaseSurface(LPDIRECTDRAWSURFACE pSurface);

	BOOL GetStringExtensionArt(const wchar_t*pString,SIZE *si);
	BOOL GetStringExtensionGDI(const wchar_t*pString,SIZE *si);
	void DrawStringArt(LPDIRECTDRAWSURFACE pSurface,const wchar_t*pString,int x,int y);
	void DrawStringGDI(LPDIRECTDRAWSURFACE pSurface, const wchar_t*pString, RECT *lpRect,UINT uFormat, INT iHeight=36,COLORREF cr=RGB(255,255,255));


protected:
	CSkinManagerGL *m_pSkinManager;

	// Implementation
public:
	virtual ~CSmartBar();

};

