#pragma once
#include "Texture.h"
#include "ProConfig.h"
#include "SkinManagerGL.h"

#define ANIM_NONE												0
#define ANIM_ROTATE_LEFT_ANTICLOCK_IN			1
#define ANIM_ROTATE_LEFT_ANTICLOCK_OUT			2
#define ANIM_ROTATE_LEFT_CLOCK_IN					3
#define ANIM_ROTATE_LEFT_CLOCK_OUT					4
#define ANIM_ROTATE_RIGHT_ANTICLOCK_IN			5
#define ANIM_ROTATE_RIGHT_ANTICLOCK_OUT		6
#define ANIM_ROTATE_RIGHT_CLOCK_IN					7
#define ANIM_ROTATE_RIGHT_CLOCK_OUT				8
#define ANIM_ROTATE_TOP_ANTICLOCK_IN				9
#define ANIM_ROTATE_TOP_ANTICLOCK_OUT			10
#define ANIM_ROTATE_TOP_CLOCK_IN					11
#define ANIM_ROTATE_TOP_CLOCK_OUT					12

#define ANIM_LINE_LEFT_IN										20
#define ANIM_LINE_TOP_IN										21
#define ANIM_LINE_RIGHT_IN									22
#define ANIM_LINE_BOTTOM_IN								23
#define ANIM_LINE_LEFT_OUT										24
#define ANIM_LINE_TOP_OUT										25
#define ANIM_LINE_RIGHT_OUT									26
#define ANIM_LINE_BOTTOM_OUT								27

#define ANIM_ZOOMIN_OUT											30
#define ANIM_ZOOMOUT_OUT										31
#define ANIM_ZOOMIN_IN												32
#define ANIM_ZOOMOUT_IN											33

typedef struct _tagViewState 
{
	GLfloat fScaleX;
	GLfloat fScaleY;
	GLfloat fScaleZ;
	GLfloat fRotateX;
	GLfloat fRotateY;
	GLfloat fRotateZ;
	GLfloat fTransX;
	GLfloat fTransY;
	GLfloat fTransZ;
}VIEW_STATE,*LPVIEW_STATE;
typedef struct _tagColorGL
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
}COLORGL,*LPCOLORGL;




// Macros for float/int from/to fixed conversions
#define _INT2FIXED(_x_)		(GLfixed)((_x_) << 16)
#define _FIXED2INT(_x_)		(int)((_x_) >> 16)
#define _FLOAT2FIXED(_x_)	(GLfixed)((_x_) *  (float)(1 << 16) + 0.5f)
#define _FIXED2FLOAT(_x_)	((float)(_x_) / (float)(1 << 16))

// extern const		int			g_iClientWidth;
// extern const		int			g_iClientHeight;


class CObjectGL
{

public:
	CObjectGL(void);
	virtual ~CObjectGL(void);

	void BeginAnimate();	
	void EndAnimate();

	void EnableAnimate(BOOL bEnable);
	BOOL IsAnimateEnabled();

	void SetAniSpeed(INT iSpeed);
	void Animate();

	void LoadAnimatePath(VIEW_STATE *pvsPath,UINT iPointCount,BOOL bClearPrevious=TRUE);
	void ResetAnimateFrameIndex(UINT iIndex);

	void SetIniViewState(VIEW_STATE * vs);
	void SetAimViewState(VIEW_STATE *vs,UINT idVS=0);
	void SetCurViewState(VIEW_STATE * vs,UINT idVS=0);
	VIEW_STATE GetCurViewState(void);
	VIEW_STATE GetAimViewState(void);

	virtual void MakeDefaultAni(UINT idAni,LPVOID lpAniPara=0);

// 	static void DrawImageRect(GLfloat x, GLfloat y,GLfloat cx,GLfloat cy,COLORGL *cr,BOOL bOnlyBorder=FALSE);
// 	static void DrawImageRect2(GLfloat x, GLfloat y,GLfloat cx,GLfloat cy,COLORGL *cr);
	static void DrawBar(CTexture *pTex,GLfloat x, GLfloat y,GLfloat cx,GLfloat cy,COLORGL *cr);
	static void DrawToolBar(CTexture *pTex,GLfloat x, GLfloat y,GLfloat cx_left,GLfloat cx_right,GLfloat height,COLORGL *cr,GLfloat color_mode=GL_ADD);
	static void DrawRect(CTexture *pTex,GLfloat x, GLfloat y,GLfloat cx,GLfloat cy,GLfloat radius,COLORGL *cr=NULL,BOOL bFill=FALSE,GLfloat color_mode=GL_MODULATE,BOOL bBottom=TRUE);
	static void DrawRect(UINT id,GLfloat x, GLfloat y,GLfloat cx,GLfloat cy,GLfloat radius,COLORGL *cr=NULL,BOOL bFill=FALSE,GLfloat color_mode=GL_MODULATE);
	static void DrawRect(GLfloat x, GLfloat y,GLfloat cx,GLfloat cy,COLORGL *cr);

	SIZE GetScaledSize(void);
	GLfloat GetScaledSizeX();
	GLfloat GetScaledSizeY();
	void SetSize(SIZE *size,BOOL bAni=FALSE);
	void SetSize(int cx,int cy,BOOL bAni=FALSE);
	virtual void SetAlpha(GLfloat fAlpha,BOOL bAni=FALSE);
	void EnableRender(BOOL bEnable);

	virtual BOOL IsReady(void);
	virtual BOOL IsAniReady(void);
	UINT GetVsID(void);
	virtual void SyncParentAlpha(void);
	GLfloat GetAlpha(void);
	SIZE GetSize(void);
	GLfloat GetPosX(void);
	GLfloat GetPosY(void);
	GLfloat GetPosxIni(void);
	GLfloat GetPosyIni(void);
	GLfloat GetPoszIni(void);

	RECT GetRect(void);
	RECT GetScreenRect(void);
	BOOL IsVisible(void);
	BOOL IsRenderEnabled(void);
	BOOL IsOnScreen();
	BOOL IsWholeBodyOnScreen();
	CObjectGL* GetParent(void);
	virtual BOOL IsPtnOnObject(POINT *pt);
	void AddRef(INT iRef);
	void SubRef(INT iRef);
	void SetLuminance(GLfloat fSet);
	virtual void Enable(BOOL bEnable);
	void EnableLight(BOOL bEnable);
	BOOL IsEnabled(void);
	virtual void SetParent(CObjectGL *pParent);
	void SetShowScale(GLfloat fScale);
	void SetScale(GLfloat x,GLfloat y,GLfloat z=1.0f,BOOL bAni=FALSE);
	virtual void LinkSkinManager(CSkinManagerGL *pSkinManager);
	void OffsetScreenRect(int x,int y);

	virtual void Show(BOOL bShow,BOOL bAni=FALSE);
	virtual void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);
	void SetPosIni(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);
	BOOL IsNeedRender(void);
	virtual void SetColor(COLORGL *cr,BOOL bAni=FALSE ) ;
	virtual void SetLightColor(COLORGL *cr,BOOL bAni=FALSE ) ;
	void Rotate(UINT iStyle);

protected:


protected:
	void UpdateTexCoord(CTexture * pTex,GLfloat xScale=1.0f,GLfloat yScale=1.0f);

public:

	virtual void Render(void)=0;

protected:

protected:
	CSkinManagerGL	*m_pSkinManager;

	VIEW_STATE		*m_pAniPath;
	UINT					m_iAniPointCount;
	UINT					m_iCurAniPointIndex;

	VIEW_STATE		m_vsCurrent;
	VIEW_STATE		m_vsOld;
	VIEW_STATE		m_vsSet;
	VIEW_STATE		m_vsIni;

	GLfloat				*m_pVertex;
	GLfloat				*m_pTexCoord;
	GLfloat				*m_pColor;
	BOOL				m_bAnimating;
	BOOL				m_bAniReady;
	BOOL				m_bReady;
	BOOL				m_bEnableRender;
	UINT					m_idVS;
	INT					m_iRenderRef;
	BOOL				m_bEnableLight;
	BOOL				m_bSetIniPos;
	BOOL				m_bEnableScale;

	GLfloat				m_fCurAlpha;
	GLfloat				m_fSetAlpha;
	GLfloat				m_fOldAlpha;
	GLfloat				m_fShowAlpha;
	GLfloat				m_fShowScaleX;
	GLfloat				m_fShowScaleY;
	GLfloat				m_fShowScaleZ;
	GLfloat				m_fCurLuminance;

	GLfloat				m_posX;
	GLfloat				m_posY;
	GLfloat				m_posZ;
	GLfloat				m_posxIni;
	GLfloat				m_posyIni;
	GLfloat				m_poszIni;

	RECT					m_rcCurRect;
	RECT					m_rcScreenRect;
	SIZE					m_siCurSize;
	SIZE					m_siOldSize;
	SIZE					m_siSetSize;
	BOOL				m_bVisible;


	BOOL				m_bEnabled;
	BOOL				m_bEnableAnimate;

	CObjectGL		*m_pParent;
	BOOL				m_bSizeChanged;

	POINT				m_ptLastPress;

	COLORGL			m_crCurColor;
	COLORGL			m_crSetColor;
	COLORGL			m_crOldColor;

	COLORGL			m_crLightColor;

	GLfloat				m_fParentAlpha;

	INT					m_iAniSpeed;
	INT					m_iSrnRectOffsetX;
	INT					m_iSrnRectOffsetY;

	UINT					m_idAni;
	LPVOID				m_lpAniPara;
};
