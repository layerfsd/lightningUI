//CSmartBarTopBanner.h : interface of the CSmartBarTopBanner class
//

#pragma once
#include "SmartBar.h"

#define SB_RES_BTN_GREEN_UP				1
#define SB_RES_BTN_GREEN_DOWN			2
#define SB_RES_BTN_TITLE_CARPLAY		3


class CSmartBarTopBanner :
	public CSmartBar
{
public:
	CSmartBarTopBanner();
public:
	virtual ~CSmartBarTopBanner();

private:

	LPDIRECTDRAWSURFACE         m_pDDSBtnGreenUp ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnGreenDown ; // 
	LPDIRECTDRAWSURFACE         m_pDDSTitleCarPlay ; // 

	SIZE	m_siBtnDrop;
	SIZE	m_siBtnTitle;

	BOOL	m_bBtnDropClicked;
	UINT		m_idNotificationUI;
	CM_String m_strTitle;

public:
	BOOL Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void SetTitle(const wchar_t *pstrTitle);
	void SetNotificationUI(UINT idPanel);

protected:
	void Render();
	BOOL LoadRes(LPCTSTR lpFileName);
	void   ReleaseRes();
	void DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si){}
};

