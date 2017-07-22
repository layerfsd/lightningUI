#pragma once
#include "controlgl.h"
#include "ButtonGL.h"

#define ST_HOR_NORMAL_EX		1
#define ST_HOR_MIDDLE_EX			2

class CSliderExGL :
	public CControlGL
{
public:
	CSliderExGL(void);
	~CSliderExGL(void);

	void Render(void);
	BOOL Initialize(UINT id,CControlGL* pParent,SIZE *pSize,CSkinManagerGL *pSkinManager);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	BOOL IsReady(void);
	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);

	void SetValue(INT iValue);
	void SetRange(INT iLow,INT iHigh);
	BOOL IsValueChanged();

	int GetValue();

	void SetType(UINT idType);
	void ShowAdjustBtns(BOOL bShow);
private:
	BOOL		m_bValueChanged;
	INT			m_iValue;
	INT			m_iValueLow;
	INT			m_iValueHigh;
	INT			m_iValueMid;
	UINT			m_idType;

	CButtonGL		m_btAdd;
	CButtonGL		m_btSub;
	BOOL				m_bShowAdjustBtns;

};
