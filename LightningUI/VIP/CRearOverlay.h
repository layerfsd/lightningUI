//CRearOverlay.h : interface of the CSmartBarDrop class
//

#pragma once
#include "../GUI/Controls/SmartBar.h"
#include "../Config/customer.h"

#define DEBUG_REAR_OVERLAY						0

#define SB_RES_REAR_STATIC_SAFETY           70
#define SB_RES_REAR_HOME                     71
#define SB_RES_REAR_HOME_FOCUS              72
class CRearOverlay :
	public CSmartBar
{
public:
	CRearOverlay();
public:
	virtual ~CRearOverlay();

private:

	LPDIRECTDRAWSURFACE         m_pDDSRearStaticSafety; //
	LPDIRECTDRAWSURFACE         m_pDDSBtnHome; //
	LPDIRECTDRAWSURFACE         m_pDDSBtnHomeFocus; //

	SIZE	m_siRearStaticSafety;
	SIZE    m_siBtnHome;
	BOOL    m_bBtnHomeClicked;
	BOOL	m_bPressed;
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

