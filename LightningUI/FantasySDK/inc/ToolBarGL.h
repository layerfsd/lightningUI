#pragma once
#include "controlgl.h"
#include "CM_List.h"
#include "ButtonGL.h"

#define BAR_BACK_STYLE_DEFAULT			0
#define BAR_BACK_STYLE_IMAGE_1			1

class CToolBarGL :
	public CControlGL
{
public:
	CToolBarGL(void);
	~CToolBarGL(void);

	void Render(void);
	BOOL Initialize(CSkinManagerGL *pSkinManager,CTextGL *pTextGL=NULL);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	BOOL IsReady(void);

	void AddIcon(CButtonGL*pIcon,UINT dwID);
	void Show(BOOL bShow,BOOL bAni=FALSE);
	void EnableIcon(UINT dwID,BOOL bEnable);

	CButtonGL* GetIcon(UINT id);
	UINT GetClickedButtonID(void);
	void SetBackStyle(UINT iStyle);
	BOOL IsOnQuit(POINT *pt);
private:
	CM_List<CButtonGL> m_listIcons;
	CButtonGL		m_btnQuit;
	UINT			m_idClickedButton;
	UINT			m_iBackStyle;
};
