#pragma once
#include "controlgl.h"
#include "LabelGL.h"

typedef struct tagDASHBOARDSTRUCTURE
{
	UINT idBack;
	UINT idPointer;
	GLfloat fStartAngle;
	GLfloat fEndAngle;
}DASHBOARDSTRUCTURE,*LPDASHBOARDSTRUCTURE;

class CDashboardGL :
	public CControlGL
{
public:
	CDashboardGL(void);
	~CDashboardGL(void);

	BOOL Initialize(UINT id,CControlGL* pParent,LPDASHBOARDSTRUCTURE pStruct,CSkinManagerGL *pSkinManager);
	BOOL IsReady(void);

	void Render(void);
	INT GetValue(void);
	void SetValue(INT iValue,BOOL bAni=TRUE);
	void SetRange(INT iLow,INT iHigh);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void SetResponseSpeed(INT iSpeed);

private:
	CLabelGL  m_tagBack;
	CLabelGL  m_tagPointer;

	int m_iResponseSpeed;
	GLfloat		m_fBeginAngle;
	GLfloat		m_fEndAngle;

	INT			m_iValueLow;
	INT			m_iValueHigh;
	INT			m_iValue;

	GLfloat		m_fRotateAngleCur;
	GLfloat		m_fRotateAngleSet;
	BOOL		m_bMovingPointer;

};
