#pragma once
#include "taggl.h"

class CTextModeGL :
	public CTagGL
{
public:
	CTextModeGL(UINT iModeStyle=1);
	~CTextModeGL(void);

	void Render(void);

	void SetAdjustDim(GLfloat x,GLfloat y);
	GLfloat GetAdjustX();
	GLfloat GetAdjustY();
private:
	UINT		m_idModeStyle;
	GLfloat		m_fAdjustX;
	GLfloat		m_fAdjustY;
};
