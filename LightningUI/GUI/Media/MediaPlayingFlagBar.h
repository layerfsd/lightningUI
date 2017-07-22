#pragma once
#include "controlgl.h"
#include "ButtonGL.h"
#include "LabelGL.h"

class CMediaPlayingFlagBar :
	public CControlGL
{
public:
	CMediaPlayingFlagBar(void);
	~CMediaPlayingFlagBar(void);

	BOOL Initialize(UINT id,CControlGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL);

	void Render(void);

	BOOL IsReady(void);

	BOOL IsPtnOnObject(POINT *pt);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi){}

private:
	int			m_iBarCount;

	int			m_iValueCur[5];
	int			m_iValueSet[5];
	BOOL		m_bValueChangeReady;
	BOOL		m_bClicked;
};
