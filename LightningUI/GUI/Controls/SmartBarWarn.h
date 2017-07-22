//CSmartBarWarn.h : interface of the CSmartBarWarn class
//

#pragma once
#include "SmartBar.h"

#define SB_RES_LABEL_MAIN_PAGE			80
#define SB_RES_BTN_AGREE_UP				81
#define SB_RES_BTN_AGREE_DOWN			82



class CSmartBarWarn :
	public CSmartBar
{
public:
	CSmartBarWarn();
public:
	virtual ~CSmartBarWarn();

private:

	LPDIRECTDRAWSURFACE         m_pDDSBtnAgreeUp ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnAgreeDown ; // 
	LPDIRECTDRAWSURFACE         m_pDDSLabelMainPage ; // 

	SIZE	m_siBtnAgree;
	SIZE	m_siLabelMainPage;

	BOOL	m_bBtnDropClicked;
	UINT	m_idNotificationUI;

	UINT	m_btnAgreePx;
	UINT	m_btnAgreePy;

public:
	BOOL Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	//void SetTitle(const wchar_t *pstrTitle);
	//void SetNotificationUI(UINT idPanel);

protected:
	void Render();
	BOOL LoadRes(LPCTSTR lpFileName);
	void ReleaseRes();
	void DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si){}
};

