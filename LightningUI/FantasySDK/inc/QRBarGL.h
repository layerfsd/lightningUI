#pragma once
#include "controlgl.h"
#include "TextModeGL.h"


class CQRBarGL :
	public CControlGL
{
public:
	CQRBarGL(void);
	~CQRBarGL(void);

	BOOL Initialize(UINT id,CControlGL* pParent,CSkinManagerGL *pSkinManager,LPCSTR lpsSource);
	BOOL IsReady(void);

	void Render(void);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	SIZE GetQRSize();

private:
	CTextModeGL  m_tagQR;
	SIZE m_siQRBar;

};
