#pragma once
#include "ObjectGL.h"

class CTagGL :
	public CObjectGL
{
public:
	CTagGL(void);
	~CTagGL(void);


public:
	BOOL Initialize(UINT id,CObjectGL* pParent,const wchar_t *texName,INT iPrecise=0,SIZE *psi=NULL);
	BOOL Initialize(UINT id,CObjectGL* pParent,const wchar_t *texName,int cx,int cy,SIZE *pActualSize=NULL);
	BOOL Initialize(UINT id,CObjectGL* pParent,UINT idRes,LPCTSTR lpType);
	BOOL Initialize(UINT id,CObjectGL* pParent,CTexture *pBaseTex,GLfloat *pTexCoor=NULL,INT iPrecise=0,BOOL bAutoTexAlign=FALSE);
	BOOL Initialize(UINT id,CObjectGL* pParent,unsigned char*pBuff,int depth,int offset,int cx,int cy,GLint format=GL_RGB,GLint type=GL_UNSIGNED_SHORT_5_6_5);

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
	void EnableFlash(BOOL bEnable);

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
	INT					m_iModePrecise;
	BOOL				m_bFlash;
};
