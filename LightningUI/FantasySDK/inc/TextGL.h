#pragma once
#include "ObjectGL.h"
#include "TextModeGL.h"
#include "CM_List.h"
#include "SkinManagerGL.h"
#include <map>

#define DEBUG_TEXT_ARBIC	0

#define STYLE_NORMAL	1
#define STYLE_ART			2

#define CHAR_OK						1
#define CHAR_CANCEL				2
#define CHAR_SWITCH				3
#define CHAR_DELETE				4
#define CHAR_DIAL					5
#define CHAR_HANG					6
#define CHAR_SAVE					7
#define CHAR_CAPS					11
#define CHAR_CHN_YUAN		14
// #define CHAR_UP						15
// #define CHAR_DOWN				16
// #define CHAR_LEFT						17
// #define CHAR_RIGHT					18


enum FontIDGL
{
	FONT_Tahoma,
	FONT_Courier_New,
	FONT_Arial,
	FONT_Times_New_Roman,
	FONT_Symbol,
	FONT_Myriad_Pro,
	FONT_Segoe_UI,
	FONT_Helvetica,
};

using namespace std;

typedef struct _ArbicParam
{
	wchar_t cBaseWord;
	wchar_t cLast;
	wchar_t cFirst;
	wchar_t cMiddle;
	wchar_t cAlone;
}ArbicParam,*PArbicParam;

typedef map  < wchar_t, ArbicParam > CTextArbicMap;
typedef pair < wchar_t, ArbicParam > CTextArbicPair;

class CTextArbicGL
{
public:
	CTextArbicGL();
	~CTextArbicGL();
	void InitTextArbicParam(ArbicParam* param, size_t count);
	void DeinitTextArbicParam();
	void SwapString(wchar_t *pStr, int len);
	void ArabicStringReshape(wchar_t *pStr);
	void TransformString(wchar_t *pStr);
	BOOL IsArabicValidChar(wchar_t c);
	BOOL IsArabicChar(wchar_t c);

private:

	BOOL IsAggregationA(wchar_t *pStr);
	BOOL IsAggregationB(wchar_t *pStr);
	UINT IsHyphen(wchar_t *pStr);

	void SetStringStart(wchar_t *pStr){m_pStringStart = pStr;}
	wchar_t TransformChar(wchar_t *pStr);

	void AddArbicParam(ArbicParam &param);
	void RemoveArbicParam(wchar_t cBaseWord);
private:
	CTextArbicMap	m_TextArbicMap;
	wchar_t			*m_pStringStart;
};

///////////////////

class CTextGL :
	public CObjectGL
{
public:
	CTextGL(void);
	~CTextGL(void);

	void Render();
	BOOL Initialize(CSkinManagerGL *pSkinManager,FontIDGL idFont,float fPixRatio=1.0f);

// 	void OnTouchDown(POINT *pt);
// 	void OnTouchUp(POINT *pt);
// 	void OnTouchMove(POINT *pt);
	void SetString(const wchar_t *pStr);
	BOOL IsReady(void);
	//void SetCharHeight(DWORD dwHeight){m_dwCharHeight=dwHeight;m_fCharScale=GLfloat(dwHeight)/GLfloat(32);}
	void GetExtension(SIZE *si,UINT idStyle=STYLE_NORMAL);
	void GetExtension(const wchar_t*pString,SIZE *si,UINT idStyle=STYLE_NORMAL);
	INT GetCharWidth(wchar_t c,UINT idStyle=STYLE_NORMAL);
	BOOL IsPtnOnText(POINT *pt);
	void OnFontChanged(FontIDGL idFont);

	void SetCharHeight(UINT iHeight);
	void SetCharScale(GLfloat scalex,GLfloat scaley);
	void SetOrientation(GLfloat fSet);
	INT GetLength();
	INT GetCharModeCount();
	void SetFontFile(const wchar_t* strFontFile);
	void EnableAutoChangeLine(BOOL bEnable,INT iMaxWidth=100);
	void EnableAutoEllipsis(BOOL bEnable,INT iMaxWidth=100);
	void SetUnderline(int iStart,int iEnd);
	void EnableShadow(BOOL bEnable);
	void SetStyle(UINT idStyle);
	BOOL IsInitialized();

	void DrawTextGL2(RECT *pRect, const WCHAR *string);
	void SetReadingOrder(BOOL bLeftToRight=TRUE);

private:
	void DrawChar(wchar_t c,GLfloat x,GLfloat y);
	void CreateTextModesBasicLatin_TimesNewRoman();
	void CreateTextModesSupplementLatin_TimesNewRoman();
	void CreateTextModesUser();

	void CreateTextModesArt();
	void CreateTextModesBasicLatin_MSYH();
	void CreateTextModesSupplementLatin_MSYH();
	void CreateTextModesRomaNum_MSYH();

	void CreateTextModesBasicLatin_Segoeui();
	void CreateTextModesSupplementLatin_Segoeui();

	void CreateTextModesArt_Helvetica();
	void CreateTextModesBasicLatin_Helvetica();
	void CreateTextModesSupplementLatin_Helvetica();
	void CreateTextModesRomaNum_Helvetica();

	BOOL GenerateCharMode(wchar_t c);
	BOOL IsDrawrableSymbo(wchar_t c);


private:
	wchar_t*	       m_pString;	
	BOOL				m_bInitialized;
	//CTextModeGL		*m_pChar[128];
	CTextModeGL		*m_pArtChar[128];
	DWORD		m_dwCharHeight;
	GLfloat			m_fCharScaleX;
	GLfloat			m_fCharScaleY;
	GLfloat			m_fOrientation;
	CTexture		*m_ptexCharBasicLatin;
	CTexture		*m_ptexCharSupplementLatin;

	BOOL			m_bAutoChangeLine;
	INT				m_iAutoChangeLineWidth;

	BOOL			m_bAutoEllipsis;
	INT				m_iAutoEllipsisWidth;

	CTexture		*m_ptexCharArt;
	CM_List<CTextModeGL> m_listCharPreset;
	CM_List<CTextModeGL> m_listCharMode;
	CTextModeGL *m_pNullMode;
	UINT					m_idFont;
	int					m_iUnderlineStart;
	int					m_iUnderlineEnd;
	BOOL				m_bDrawShadow;
	BOOL				m_idStyle;
	CM_String			m_strFont;
	int					m_cxDraw;
	int					m_cyDraw;
	int					m_cxActual;
	int					m_cyActual;
	BOOL				m_bLeftToRight;

	CTextArbicGL		*m_pTextArbicGL;
};
