#pragma once
#include "ControlGL.h"
#include "LabelGL.h"
#include "CriticalSection.h"

#define BS_UP				0
#define BS_DOWN		1
#define BS_FOCUS		2
#define BS_DISABLED	3

#define BTN_CAPTION_BOTTOM					1
#define BTN_CAPTION_LEFT							2
#define BTN_CAPTION_TOP							3
#define BTN_CAPTION_RIGHT						4
#define BTN_CAPTION_CENTER					5
#define BTN_CAPTION_BOTTOM_RIGHT		6

class CButtonGL :
	public CControlGL
{
public:
	CButtonGL(void);
	~CButtonGL(void);

	void Lock(){m_DataCS->Enter();}
	void UnLock(){m_DataCS->Leave();}

	 void Render(void);
	 BOOL CButtonGL::Initialize(UINT id,CControlGL* pParent,
		 CTexture *pTexUp,GLfloat *pTexCoorUp,
		 CTexture *pTexDown,GLfloat *pTexCoorDown,
		 CTexture *pTexFocus,GLfloat *pTexCoorFocus,
		 CTexture *pTexDisable,GLfloat *pTexCoorDisable);

	 void SetCaptionBackTex(CTexture *pTex);
	 void OnTouchDown(POINT *pt);
	 void OnTouchUp(POINT *pt);
	 void OnTouchMove(POINT *pt);

	 void OnGesture(GESTUREINFO *pgi);
	 BOOL IsReady(void);

	 void Enable(BOOL bEnable);
	 BOOL IsPtnOnObject(POINT *pt);
	 void SetStatus(UINT iStatus,BOOL bAni=FALSE);
	 BOOL IsClickValid(void);
	 BOOL IsLongTimeClick(void);
	 void ResetHoldTick();
	 void SetCaption(const wchar_t* pName,BOOL bAutoSize=FALSE,BOOL *pbEllipsis=NULL);
	 int  GetCaption(wchar_t* pName,INT iLen);
	 void SetSubCaption(const wchar_t* pSubName);
	 wchar_t *GetCaption();
	 void ShowCaption(BOOL bShow);
	 void SetCaptionHeight(INT iHeight,INT iWidth=0);
	 void SetSubCaptionHeight(INT iHeight,INT iWidth=0);
	 void SetCaptionPos(UINT idType,int offset_x=0,int offset_y=0);
	 void SetSubCaptionPos(UINT idType,int offset_x=0,int offset_y=0);
	 void EnableMove(BOOL bEnable);
	 void SetAnglePos(GLfloat fSet);
	 GLfloat GetAnglePos(void);
	 BOOL IsOnFocus(void);
	 UINT GetStatus(void);
	 void SetCaptionColor(COLORGL *cr,COLORGL *cr_down=NULL);
	 void SetButtonColor(COLORGL *cr);
	 CLabelGL*GetIcon();
	 void SetIcon(CLabelGL *pIconUp,CLabelGL *pIconDown=NULL,GLfloat px=0,GLfloat py=0);
	 void SetIconPos(GLfloat px,GLfloat py);
	 void EnableClickAni(BOOL bEnable);
	 void EnableDrawBack(BOOL bEnable);
	 void EnableCaptionShadow(BOOL bEnable);
	 void SetCaptionCharStyle(UINT idStyle);
	 BOOL HasTexture();
	 void SetIconScale(GLfloat fx,GLfloat fy);
	 void SetTouchRate(GLfloat fx,GLfloat fy);
	 void SetTouchRect(RECT *prc);

	 CLabelGL *GetLabel(UINT idStatus);
	 void EnableHoldNotify(BOOL bEnable);
	 void EnableLongClickNotify(BOOL bEnable);
     void EnableTouchDownNotify(BOOL bEnable);
	 void EnableTouchUpNotify(BOOL bEnable);
	 void EnableIconClickAni(BOOL bEnable);
	 void EnableTouchGlow(BOOL bEnable,CTexture *pTexGlow=NULL,int cx=0,int cy=0);

	 void SetUserParam(LPARAM para);
	 LPARAM GetUserParam();

	 void Show(BOOL bShow,BOOL bAni=FALSE);

protected:

private:
	void ProcessNotify();

private:
	CCriticalSection* m_DataCS;

protected:
// 	CTexture			*m_ptexUp;
// 	CTexture			*m_ptexDown;
// 	CTexture			*m_ptexFocus;
 	CTexture			*m_ptexCaptionBack;

	CLabelGL			m_tagUp;
	CLabelGL			m_tagDown;
	CLabelGL			m_tagFocus;
	CLabelGL			m_tagDisabled;
	CLabelGL			m_tagGlow;

	UINT					m_idButtonStatus;
	DWORD			m_dwHoldTick;
	DWORD			m_dwLastPressTick;
	wchar_t*				m_pstrName;
	wchar_t*				m_pstrSubName;
	BOOL				m_bShowCaption;
	UINT				m_idCaptionPos;
    UINT                m_idSubCaptionPos;
	GLfloat				m_fLastPosX;
	GLfloat				m_fLastPosY;
	GLfloat				m_fAnglePos;
	BOOL				m_bEnableMove;
	POINT				m_ptLastPress;
	INT					m_iCaptionHeight;
	INT					m_iCaptionWidth;
	INT					m_iSubCaptionHeight;
	INT					m_iSubCaptionWidth;
	COLORGL         m_crButtonColor;
	COLORGL         m_crCaptionColor;
	COLORGL         m_crCaptionColorDown;
	CLabelGL			*m_ptagIconUp;
	CLabelGL			*m_ptagIconDown;
	BOOL				m_bClickAni;
	BOOL				m_bClickAniFlag;
	BOOL				m_bDrawBack;
	BOOL				m_bDrawCaptionShadow;
	BOOL				m_bOnFocus;
	BOOL				m_bLongTimeClick;
	BOOL				m_bPressFlag;
	BOOL				m_bIconClickAni;


	UINT					m_idCaptionCharStyle;

	SIZE					m_siImageSize;
	GLfloat				m_fIconScaleX;
	GLfloat				m_fIconScaleY;

	INT					m_iCaptionOffsetX;
	INT					m_iCaptionOffsetY;
	INT					m_iSubCaptionOffsetX;
	INT					m_iSubCaptionOffsetY;

	GLfloat				m_fTouchRateX;
	GLfloat				m_fTouchRateY;
	GLfloat				m_fIconPosX;
	GLfloat				m_fIconPosY;
	BOOL				m_bUseDefaultTouchRate;
	BOOL				m_bUseDefaultTouchRect;
	RECT					m_rcUserTouchRect;
	BOOL				m_bEnableHoldNotify;
	BOOL				m_bEnableLongClickNotify;
    BOOL                m_bEnableTouchDownNotify;
	BOOL                m_bEnableTouchUpNotify;
	BOOL				m_bEnableTouchGlow;

	LPARAM			m_lpUserParam;

};
