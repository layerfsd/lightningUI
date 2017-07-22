#pragma once
#include "ControlGL.h"
#include "LabelGL.h"
#include "ButtonGL.h"
#include "SliderGL.h"

#define SET_NULL			0
#define SET_SECOND	1
#define SET_MINUTE	2
#define SET_HOUR		3

typedef struct tagCLOCKBARSTRUCTURE
{
	SIZE	siBorder;
	BOOL bDashboardOnly;
	UINT idTexBase;
	UINT idTexHour;
	UINT idTexMinute;	
	UINT idTexSecond;
	UINT idTexBtnUp;
	UINT idTexBtnDown;
	UINT idTexIconAdd;
	UINT idTexIconSub;
	COLORGL crCaptionUp;
	COLORGL crCaptionDown;
	GLfloat fDashboardScale;
}CLOCKBARSTRUCTURE,*LPCLOCKBARSTRUCTURE;

class CClockBarGL :
	public CControlGL
{
public:
	CClockBarGL(void);
	~CClockBarGL(void);

	BOOL Initialize(UINT id,CControlGL* pParent,LPCLOCKBARSTRUCTURE pStruct,CSkinManagerGL *pSkinManager,CTextGL *pTextGL);

	void Render(void);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	BOOL IsSetEnabled();

	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);
	void SetSettingType(UINT idType);
	BOOL IsReady(void);
	void ApplySettings();

private:
	CLabelGL  m_tagBack;
	CLabelGL  m_tagHourPt;
	CLabelGL  m_tagMinPt;
	CLabelGL  m_tagSecPt;
	BOOL		m_bOnFocus;
	BOOL		m_bEnableSet;
	UINT			m_idCurSet;
	COLORGL		m_crCaption;

	GLfloat		m_fDashboardScale;

	CLabelGL			m_iconUp;
	CLabelGL			m_iconDown;

	CButtonGL		m_btAdd;
	CButtonGL		m_btSub;

	CButtonGL		m_btHour;
	CButtonGL		m_btMin;
	CButtonGL		m_btSec;

// 	CButtonGL		m_btSet;

	SYSTEMTIME	m_timeToSet;
	BOOL				m_bDragSetting;
	//CSliderGL			m_sliMode;
};
