#pragma once
#include "ControlGL.h"
#include "ButtonGL.h"

#define ST_HOR_NORMAL	0
#define ST_HOR_MIDDLE		1
#define ST_VER_NORMAL		2
#define ST_VER_MIDDLE		3

typedef struct tagSLIDERSTRUCTURE
{
	UINT idBackTex;
	UINT idChannelTex;
	UINT idThumbTex;
	GLfloat px;
	GLfloat py;
	SIZE	size;
	BOOL bHasBtns;
	BOOL bFilled;
	BOOL bFilledBoth;
	BOOL bNoBorder;
	DWORD dwType;
	BOOL bSwitchMode;

}SLIDERSTRUCTURE,*LPSLIDERSTRUCTURE;

class CSliderGL :
	public CControlGL
{
public:
	CSliderGL(void);
	~CSliderGL(void);

public:
	void Render(void);
	BOOL Initialize(UINT id,CControlGL* pParent,LPSLIDERSTRUCTURE pSliderStruct,CSkinManagerGL *pSkinManager);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	BOOL IsPtnOnObject(POINT *pt);
	BOOL IsReady(void);
	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);
	void SetThumbScale(GLfloat scale,BOOL bLimited=TRUE);
	void SetValue(INT iValue,BOOL bChangedFlag=FALSE);
	void SetRange(INT iLow,INT iHigh);
	void SetLimit(INT iLow,INT iHigh);
	INT GetValue(void);
	INT GetLastValue(void);
	void Step(INT iStep);
	void ShowValue(BOOL bShow);
	BOOL IsPtnOnSlideClient(POINT *pt);
	BOOL IsValueChanged();
	void SetAutoHideBtns(BOOL bAutoHide);
	void SetCaption(const wchar_t *pStr);
	void Enable(BOOL bEnable);
	BOOL IsOnAdd(POINT *pt);
	BOOL IsOnSub(POINT *pt);
	void ShowThumb(BOOL bShow);
	void SetCaptionColor(COLORGL *cr);
	void SetAutoBackFill(BOOL bAutoFill);
	void EnableMoveNotify(BOOL bEnable);
	void EnbaleClickOnlyThumb(BOOL bEnable);

protected:
	CButtonGL		m_btThumb;
	GLfloat				m_fThumbScale;
	INT					m_iValue;
	INT					m_iValueLow;
	INT					m_iValueHigh;
	INT					m_iLastValue;
	INT					m_iLimitLow;
	INT					m_iLimitHigh;
	CM_String			m_strCaption;
	DWORD			m_idType;
	BOOL				m_bHasAdjustBtns;
	BOOL				m_bFilled;
	BOOL				m_bFilledBoth;
	BOOL				m_bShowValue;
	BOOL				m_bAutoHideBtns;
	BOOL				m_bDrawBorder;
	BOOL				m_bShowThumb;
	DWORD			m_dwLastShowBtnTick;
	BOOL				m_bAutoBackFill;
	BOOL				m_bEnableThumbScale;
	BOOL				m_bSwitchMode;
private:
	RECT					m_rcClient;
	BOOL				m_bValueChanged;

	CTexture			*m_ptexBack;
	CTexture			*m_ptexChannel;

	CButtonGL		m_btAdd;
	CButtonGL		m_btSub;
	CLabelGL			m_iconAdd;
	CLabelGL			m_iconSub;

	CTexture *m_pBase;
	CTexture *m_pActive;
	CTexture *m_pNormal;

	COLORGL	m_crCaptionColor;

	BOOL	m_bEnableMoveNotify;
	BOOL	m_bEnbaleClickOnlyThumb;
};
