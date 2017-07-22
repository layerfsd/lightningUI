#pragma once
#include "controlgl.h"
#include "ButtonGL.h"
#include "CM_List.h"
#include "IMAtom.h"

enum IMSHOWMODE
{
	IM_INPUT,
	IM_IMAGIN
};
class CIMBarGL :
	public CControlGL
{
public:
	CIMBarGL(void);
	~CIMBarGL(void);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	BOOL Initialize(UINT id,CControlGL* pParent,CSkinManagerGL *pSkinManager);

	void Render(void);

	void LinkTextGL(CTextGL *pTextGL);
	void Input(wchar_t c);
	void Imagin(wchar_t c);
	void Clear(int inum);
	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);

	wchar_t GetChar();
	const wchar_t * GetString();
	const wchar_t * GetImagin();
	const wchar_t * GetOutput();

private:
	BOOL LoadPinyinTable(void);
	BOOL LoadImaginTable(void);

private:
	CButtonGL m_btnOutputChar[8];
	CM_String         m_strInput;
	CM_String			m_strImagin;
	CM_String			m_strOutput;
	wchar_t			m_cOutput;
	CButtonGL		m_btAdd;
	CButtonGL		m_btSub;
	CM_List<CIMAtom> m_listIMPinyin;
	CM_List<CIMImagin> m_listIMImagin;

	CM_String		m_strCharList;
	int				m_iOffset;
	int				m_iImaginHeadPos;
	int				m_iImaginTailPos;

// 	int				m_iImaginLastPos;

	IMSHOWMODE m_imMode;
	CIMImagin *m_pCurImagin;
};
