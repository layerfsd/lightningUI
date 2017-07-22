#pragma once
#include "controlgl.h"
#include "ButtonGL.h"
#include "CM_List.h"
#include "LabelGL.h"
#include "ListBoxGL.h"
#include "IMBarGL.h"

#define INPUT_LETTERS_CAP		1
#define INPUT_LETTERS_SMALL	2
#define INPUT_SYMBOLS				4
#define INPUT_PINYIN					5

typedef struct tagKEYBOARDSTRUCTURE
{
	SIZE	siBorder;
	UINT idTexBtnUp;
	UINT idTexBtnDown;
	COLORGL crCaptionUp;
	COLORGL crCaptionDown;
}KEYBOARDSTRUCTURE,*LPKEYBOARDSTRUCTURE;


class CKeyBoardGL :
	public CControlGL
{
public:
	CKeyBoardGL(void);
	~CKeyBoardGL(void);

	BOOL Initialize(UINT id,CControlGL* pParent,LPKEYBOARDSTRUCTURE pStruct,CSkinManagerGL *pSkinManager,CTextGL *pTextGL);

	void Render(void);
	BOOL IsReady(void);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void SetInputType(UINT idType);
	BOOL IsOnOK(POINT *pt);
	BOOL IsOnSave(POINT *pt);
	BOOL IsOnCancel(POINT *pt);
	BOOL IsOnDial(POINT *pt);
	BOOL IsOnHang(POINT *pt);

	void SetInputString(const wchar_t *str);
	void SetCaption(const wchar_t *str);
	const wchar_t *GetInputString(void);
	UINT GetInputType(void);
	void Show(BOOL bShow,BOOL bAni=FALSE);
	void Input(wchar_t *pInput);
	void SetInputMaxLen(UINT iLen);
	void SetInputMinLen(UINT iLen);

	void SetTask(UINT idTask);
	UINT GetTask();
	INT GetInputLength();
private:
	CButtonGL *m_pbtKey[128];

// 	CListBoxGL m_listboxInputMethod;
	CIMBarGL	 m_barIM;

// 	CLabelGL m_tagUp;
// 	CLabelGL m_tagDown;

// 	CButtonGL m_tagFocus;

	CM_List<CButtonGL> m_listLetters;
	CM_List<CButtonGL> m_listNumbers;
	CM_List<CButtonGL> m_listSymbols;
// 	CM_List<CButtonGL> m_listSymbols2;

	UINT			m_idCurInputType;
	UINT			m_idLastInputType;
	CM_String         m_strInput;
	CM_String			m_strCaption;
	BOOL			m_bCaps;
	UINT				m_iInputMaxLen;
	UINT				m_iInputMinLen;
	UINT				m_iAutoBreakLen;
	UINT				m_iLines;
	UINT				m_idTask;

};
