#pragma once
#include "ControlGL.h"

class CShapedViewGL :
	public CControlGL
{
public:
	CShapedViewGL(void);
	~CShapedViewGL(void);


public:
	BOOL Initialize(UINT id,CControlGL* pParent,UINT idShapeType,CTexture *pBaseTex,CTexture *pCoverTex);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void Render(void);

private:
	void ProcessNotify();

protected:
	CTexture			*m_ptexBase;
	CTexture			*m_ptexCover;

	UINT m_idShapeType;
	int		m_iPrecise;
	BOOL m_bPressed;
	DWORD m_dwLastPressTick;

};
