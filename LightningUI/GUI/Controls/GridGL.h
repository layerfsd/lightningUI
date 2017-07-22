#pragma once
#include "controlgl.h"
#include "ButtonGL.h"
#include "LabelGL.h"

class CGridGL :
	public CControlGL
{
public:
	CGridGL(void);
	~CGridGL(void);

	BOOL Initialize(UINT id,CControlGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL);

	void Render(void);
	void Enable(BOOL bEnable);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);

	BOOL IsReady(void);
	BOOL IsPtnOnObject(POINT *pt);

	inline void SetRange(int xl,int yl,int xh,int yh){m_iLowX=xl;m_iLowY=yl;m_iHighX=xh;m_iHighY=yh;}
	void SetValueX(int x);
	void SetValueY(int y);

	void SetMargin(int iMargin);

	inline int GetValueX(){return m_iCurX;}
	inline int GetValueY(){return m_iCurY;}
	BOOL IsOnAdjustBtn(POINT *pt){return m_btDown.IsPtnOnObject(pt)||m_btUp.IsPtnOnObject(pt)||m_btRight.IsPtnOnObject(pt)||m_btLeft.IsPtnOnObject(pt);}

protected:
	void SetValue(int x,int y,BOOL bNotify=TRUE);

private:
	//CTexture  *m_ptexBase;
	CLabelGL	m_iconUp;
	CLabelGL	m_iconDown;
	CLabelGL	m_iconRight;
	CLabelGL	m_iconLeft;

	CButtonGL m_btUp;
	CButtonGL m_btDown;
	CButtonGL m_btRight;
	CButtonGL m_btLeft;
	CLabelGL		m_tagWave;

	CLabelGL		m_tagLineHor;
	CLabelGL		m_tagLineVer;

	BOOL			m_bValueChanged;
	BOOL			m_bClickOnBtns;
	BOOL			m_bEnable;

	int			m_iLowX;
	int			m_iLowY;
	int			m_iHighX;
	int			m_iHighY;
	int			m_iCurX;
	int			m_iCurY;
	int			m_iMargin;
};
