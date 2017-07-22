//CSmartBarDrop.h : interface of the CSmartBarDrop class
//

#pragma once
#include "SmartBar.h"

#define SB_RES_BTN_DROP_UP			1
#define SB_RES_BTN_DROP_DOWN		2


class CSmartBarDrop :
	public CSmartBar
{
public:
	CSmartBarDrop();
public:
	virtual ~CSmartBarDrop();

private:

	LPDIRECTDRAWSURFACE         m_pDDSBtnDropUp ; //
	LPDIRECTDRAWSURFACE         m_pDDSBtnDropDown ; //

	SIZE	m_siBtnDrop;

	BOOL	m_bBtnDropClicked;
//	    POINT   m_LastPoint;
    DWORD   m_LastTouchDownTick;

public:
	BOOL Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

protected:
	void Render();
	BOOL LoadRes(LPCTSTR lpFileName);
	void   ReleaseRes();
	void DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si){}
};

