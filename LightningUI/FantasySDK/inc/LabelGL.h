#pragma once
// #include "ObjectGL.h"
#include "ControlGL.h"

class CLabelGL :
	public CControlGL
{
public:
	CLabelGL(void);
	~CLabelGL(void);


public:
	BOOL Initialize(UINT id,CControlGL* pParent,const wchar_t *texName,SIZE *psi=NULL);
	BOOL Initialize(UINT id,CControlGL* pParent,const wchar_t *texName,int cx,int cy,SIZE *pActualSize=NULL);
	BOOL Initialize(UINT id,CControlGL* pParent,UINT idRes,LPCTSTR lpType);
	BOOL Initialize(UINT id,CControlGL* pParent,CTexture *pBaseTex,GLfloat *pTexCoor=NULL,BOOL bAutoTexAlign=FALSE);
	BOOL Initialize(UINT id,CControlGL* pParent,unsigned char*pBuff,int depth,int offset,int cx,int cy,GLint format=GL_RGB,GLint type=GL_UNSIGNED_SHORT_5_6_5);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);
	void OnGesture(GESTUREINFO *pgi);

	void Render(void);
	void SetOffset(GLfloat x,GLfloat y);
	GLfloat GetOffsetX(void);
	GLfloat GetOffsetY(void);
	void SetLeanAngle(GLfloat x,GLfloat y);
	void EnableShadow(BOOL bEnable);
	CTexture *GetTexture(void);
	BOOL HasValidTeture();

protected:
	UINT					m_ID;
	CTexture			*m_ptexBase;
	GLfloat				m_fOffsetX;
	GLfloat				m_fOffsetY;
	GLfloat				m_fLeanAngleX;
	GLfloat				m_fLeanAngleY;
	BOOL				m_bNeedDelete;
	BOOL				m_bAutoTexAlign;
	BOOL				m_bDrawShadow;
};
