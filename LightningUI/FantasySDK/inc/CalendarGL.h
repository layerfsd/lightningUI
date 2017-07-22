#pragma once
#include "controlgl.h"
#include "ButtonGL.h"

typedef struct tagCALENDARBARSTRUCTURE
{
	SIZE	siBorder;
	UINT idTexBtnUp;
	UINT idTexBtnDown;
	UINT idTexIconAdd;
	UINT idTexIconSub;
	COLORGL crCaptionUp;
	COLORGL crCaptionDown;
}CALENDARBARSTRUCTURE,*LPCALENDARBARSTRUCTURE;

class CCalendarGL :
	public CControlGL
{
public:
	CCalendarGL(void);
	~CCalendarGL(void);

	BOOL Initialize(UINT id,CControlGL* pParent,LPCALENDARBARSTRUCTURE pStruct,CSkinManagerGL *pSkinManager,CTextGL *pTextGL);

	void Render(void);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);

	void OnGesture(GESTUREINFO *pgi);

	void SetTime(SYSTEMTIME *time);
	void ResetTime();

	void LinkTextGL(CTextGL *pTextGL);
	WORD GetYear();
	WORD GetMonth();
	WORD GetDay();
	void ApplySettings();

private:
	INT GetDayOfWeek(INT iYear,INT iMonth,INT iDay);
	INT DaysInMonth(INT iYear,INT iMonth);

private:
	SYSTEMTIME m_time;
	SYSTEMTIME m_LocalTime;
	CButtonGL    m_btDay[31];
	INT m_iWeekOffset;

	COLORGL		m_crCaption;
// 	CLabelGL			m_tagSeperator1;
// 	CLabelGL			m_tagSeperator2;

	CLabelGL			m_iconUp;
	CLabelGL			m_iconDown;

	CButtonGL		m_btAdd;
	CButtonGL		m_btSub;
	UINT					m_idType;
	CButtonGL		m_btYear;
	CButtonGL		m_btMonth;

// 	CButtonGL		m_btApply;
};
