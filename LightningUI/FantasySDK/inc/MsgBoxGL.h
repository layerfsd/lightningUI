#pragma once
#include "controlgl.h"
#include "ButtonGL.h"
#include "LabelGL.h"

#define BOX_TYPE_DEFAULT				0
#define BOX_TYPE_ALERT					1
#define BOX_TYPE_INFO					2
#define BOX_TYPE_QUEST				3
#define BOX_TYPE_PROCESSING		4
#define BOX_TYPE_TEXT_VIEW			5

class CMsgBoxGL :
	public CControlGL
{
public:
	CMsgBoxGL(void);
	~CMsgBoxGL(void);

public:
	BOOL Initialize(UINT id,CControlGL* pParent,CSkinManagerGL *pSkinManager,UINT idType=BOX_TYPE_DEFAULT);
	void Render(void);
// 	inline void SetSize(SIZE *si){m_iWidth=si->cx;m_iHeight=si->cy;}
// 	inline void SetSize(INT x,INT y){m_iWidth=x;m_iHeight=y;}

	void LinkTextGL(CTextGL *pTextGL);
	void SetTextFontHeight(INT iHeight){m_iTextHeight=iHeight;}

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void SetMsg(const wchar_t *pstrMsg);
	BOOL IsOnOK(POINT *pt);
	BOOL IsOnCancel(POINT *pt);
	void EnableBackground(BOOL bEnable);
	const wchar_t*GetString();
private:
	CButtonGL		m_btOK;
	CButtonGL		m_btCancel;
// 	INT					m_iWidth;
// 	INT					m_iHeight;
	CM_String			m_strMsg;
	UINT					m_idBoxType;
	CLabelGL			m_icon;
	BOOL				m_hasBackground;
	INT					m_iTextHeight;
};
