//CSmartBarPhone.h : interface of the CSmartBarPhone class
//

#pragma once
#include "SmartBar.h"

#define SB_RES_BACKGROUND				55

#define SB_RES_SYMBOS							60
#define SB_RES_BTN_ANSWER_UP			61
#define SB_RES_BTN_ANSWER_DOWN		62
#define SB_RES_BTN_HANG_UP				63
#define SB_RES_BTN_HANG_DOWN			64
#define SB_RES_BTN_ACH_UP					65
#define SB_RES_BTN_ACH_DOWN				66
#define SB_RES_BTN_ACH_PHONE_UP					67
#define SB_RES_BTN_ACH_PHONE_DOWN			68


enum SMARTBAR_PHONE_TYPE
{
	SB_NULL,
	SB_INCOMING,
	SB_OUTGOING,
	SB_TALKING
};

class CSmartBarPhone :
	public CSmartBar
{
public:
	CSmartBarPhone();
public:
	virtual ~CSmartBarPhone();

private:

	LPDIRECTDRAWSURFACE         m_pDDSBackground ; // 

	//incoming and talking:
	LPDIRECTDRAWSURFACE         m_pDDSSymbos ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnAnswerUp ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnAnswerDown ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnHangUp ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnHangDown ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnACHUp ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnACHDown ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnACHPhoneUp ; // 
	LPDIRECTDRAWSURFACE         m_pDDSBtnACHPhoneDown ; // 

	SIZE	m_siBtnAnswer;
	SIZE	m_siBtnHang;
	SIZE	m_siBtnACH;
	BOOL m_bBtnAnswerClicked;
	BOOL m_bBtnHangClicked;
	BOOL m_bBtnACHClicked;

	SMARTBAR_PHONE_TYPE			m_idType;
	CM_String						m_strPhoneNumber;
	UINT				m_idAudioDir;

public:
	BOOL Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns);

	void SetPhoneNumber(const wchar_t*pstrNumber);
	void SetType(SMARTBAR_PHONE_TYPE type);
	SMARTBAR_PHONE_TYPE GetType(){return m_idType;}
	void SetAudioDir(UINT idAudioDir);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

protected:
	void Render();
	BOOL LoadRes(LPCTSTR lpFileName);
	void   ReleaseRes();
	void DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si);

};

