#pragma once
#include "ControlGL.h"
#include "CriticalSection.h"
#include "ButtonGL.h"
#include "CM_List.h"
#include "ButtonGL.h"
#include "SliderGL.h"
#include "LabelGL.h"

#define LIST_ALIGN_LEFT			1
#define LIST_ALIGN_CENTER		2
#define LIST_ALIGN_RIGHT		3

#define ITEM_SORT_BY_NAME		1
#define ITEM_SORT_BY_VALUE		2
#define ITEM_SORT_BY_ID			3

typedef struct tagLISTBOXSTRUCTURE
{
	UINT idTexBase;
	UINT idTexIndexBase;
	UINT idTexIndexFocus;	
	UINT idTexIndexClick;	
	UINT idTexSliBase;
	UINT idTexSliChannel;
	UINT idTexSliThumb;	
	UINT idTexBtnUp;
	UINT idTexBtnDown;	
	GLfloat pxSlider;
	GLfloat pySlider;
	GLfloat fIndexBaseOffset;
	SIZE	siClient;
	SIZE siBorder;
	INT iSpacing;
	UINT idAlignType;
	BOOL bNoSlider;
	BOOL bNoBorder;
	BOOL bNoHorSeperator;
	BOOL bGlow;
	BOOL bControlReadOnly;
	BOOL bSliderAutoHide;
	BOOL bQuickSearchBoard;

}LISTBOXSTRUCTURE,*LPLISTBOXSTRUCTURE;

class CListBoxItemGL :
	public CControlGL
{
public:
	CListBoxItemGL(CONST wchar_t*  name,CONST wchar_t* val=NULL,CONST wchar_t* sub_name=NULL,GLfloat px=0, GLfloat py=0);
	~CListBoxItemGL(void);

public:
	void Render(void);
	BOOL SetIcon(CLabelGL *pIcon,CLabelGL *pIconFocus=NULL,GLfloat px=0, GLfloat py=0);
	BOOL SetIconFromFile(const wchar_t *texName,int cx,int cy);
	BOOL SetIconFromImage(CImageGL *image);
	BOOL SetIcon2(CLabelGL *pIconUp,CLabelGL *pIconDown=NULL,GLfloat px=0, GLfloat py=0);
	void SetIconChecked2(BOOL bChecked);
	BOOL Initialize(UINT id,SIZE *siNormal,SIZE *siFocus,int iMargin,CControlGL *pParent,CSkinManagerGL *pSkinManager);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	BOOL IsReady(void);

	BOOL IsPtnOnItem(POINT *pt);
	void SetFocus(BOOL bFocus,BOOL bAni=FALSE);
	void SetItemName(const wchar_t* name);
	void SetItemSubName(const wchar_t* sub_name);
	void SetValue(const wchar_t* pValue);
	void SetBaseOffset(GLfloat offset);

	void SetIconScale(GLfloat fx,GLfloat fy);
	INT GetIndex(void);
	void SetIndex(INT index);
	BOOL IsOnFocus();
	wchar_t *GetItemName(void);
	wchar_t *GetItemSubName(void);
	wchar_t *GetItemValue(void);
	void SetCaptionAlignType(UINT type);

	void SetCaptioniHeight(INT iHeightName,INT iHeightValue=0,INT iHeightSubName=0);
	void SetCaptioniOffset(GLfloat iNameOffset,GLfloat iValueOffset=0,GLfloat iSubNameOffset=0);
	void AddCaptioniOffset(GLfloat iNameOffset,GLfloat iValueOffset=0,GLfloat iSubNameOffset=0);
	void SetCaptionColor(COLORGL *cr_normal,COLORGL *cr_focus=NULL,COLORGL *cr_click=NULL);

	void ShowValue(BOOL bShow);
	void SetSortValue(UINT iValue);
	void SetKeyWords(wchar_t *pStr);
	const wchar_t *GetKeyWords(void);
	UINT GetSortValue();
	CLabelGL * GetIcon();
	void SetControl(CControlGL *pControl);
	void SetHorSeperator(BOOL bVisible);
	void EnableVerSeperator(BOOL bVisible);
	void EnableGlow(BOOL bEnable);
	void EnableControl(BOOL bEnable);
	void ShowControl(BOOL bShow);
	BOOL IsPtnOnControl(POINT *pt);
	void SetUserParam(LPARAM para);
	LPARAM GetUserParam();
	void EnableClick(BOOL bEnable);
	void EnableAutoEllipsis(BOOL bEnable);

	void SetTexture(CTexture *pTexBase,CTexture *pTexFocus,CTexture *pTexClick=NULL);
	
protected:
	GLfloat			m_pxIcon;
	GLfloat			m_pyIcon;
	GLfloat			m_pxIcon2;
	GLfloat			m_pyIcon2;
	CLabelGL		*m_pbtIcon;
	CLabelGL		*m_pbtIcon2;
	CLabelGL		*m_pbtIconFocus;

	CLabelGL		*m_pbtIconUp;
	CLabelGL		*m_pbtIconDown;

	CControlGL		*m_pTheControl;
	WCHAR*			m_pstrItemName;
	WCHAR*			m_pstrItemSubName;
	INT					m_iValue;
	INT					m_iIndex;
	WCHAR*				m_pstrValue;
	BOOL				m_bHasIcon;
	BOOL				m_bOnFocus;
	BOOL				m_bHasHorSeperator;
	BOOL				m_bDrawVerSeperator;
	SIZE					m_siFocus;
	SIZE					m_siNormal;
	BOOL				m_bNeedDelete;
	UINT					m_idCaptionAlignType;
	BOOL				m_bShowValue;
	BOOL				m_bEnableGlow;
	UINT					m_iSortValue;
	CM_String			m_strKeyWords;
	CTexture			*m_pTexBase;
	CTexture			*m_pTexFocus;
	CTexture			*m_pTexClick;
	int					m_iMargin;
	GLfloat				m_fBaseOffset;

	int					m_iCaptionHeightName;
	int					m_iCaptionHeightValue;
	int					m_iCaptionHeightSubName;
	GLfloat				m_iCaptionNameOffset;
	GLfloat				m_iCaptionValueOffset;
	GLfloat				m_iCaptionSubNameOffset;
	BOOL				m_bSetCaptionColor;
	COLORGL			m_crNormal;
	COLORGL			m_crFocus;
	COLORGL			m_crClick;

	LPARAM			m_lpUserParam;

private:
	BOOL				m_bClickOnIcon1;
	BOOL				m_bClickOnIcon2;

	BOOL				m_bClickedOnItem;

	BOOL				m_bIconChecked2;
	BOOL				m_bEnableClick;
	BOOL				m_bEnableAutoEllipsis;

};
class CListBoxGL :
	public CControlGL
{
public:

	CListBoxGL(void);
	~CListBoxGL(void);

	void Lock(){m_DataCS->Enter();}
	void UnLock(){m_DataCS->Leave();}

	void ClearList(void);
	void Render(void);
	BOOL Initialize(UINT id,CControlGL* pParent,LPLISTBOXSTRUCTURE pBoxStruct,CSkinManagerGL *pSkinManager);
	BOOL IsLongTimeClick(void);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	BOOL IsPtnOnObject(POINT *pt);

	void SetSortType(UINT idType);

	void AddItem(CListBoxItemGL* pItem,UINT id=-1);
	void AddItemByOrder(CListBoxItemGL* pItem,UINT id=-1,BOOL bIgnore=FALSE,BOOL bAscending=TRUE);
	void InsertItem(CListBoxItemGL* pItem,INT iIndexBefore,UINT id=-1);
	BOOL IsReady(void);
	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);
	BOOL MoveToIndex(INT index,INT iSpeed=0,BOOL bAni=FALSE);
	INT MoveToKeyWordIndex(const wchar_t *key);
	INT GetKeyWordIndex(const wchar_t *key, int iStartIndex);

	void SetCaptioniHeight(INT iHeightName,INT iHeightValue=0,INT iHeightSubName=0);
	void SetCaptioniOffset(GLfloat iNameOffset,GLfloat iValueOffset=0,GLfloat iSubNameOffset=0);
	void SetCaptionColor(COLORGL *cr_normal,COLORGL *cr_focus=NULL,COLORGL *cr_click=NULL);

	BOOL MoveToNext(BOOL bAutoJumpHead=TRUE,BOOL bAni=FALSE);
	BOOL MoveToLast(BOOL bAutoJumpTail=TRUE,BOOL bAni=FALSE);
	BOOL MoveToWnd(INT index,INT iSpeed);
	void LinkTextGL(CTextGL *pTextGL);
	void Reset(void);
	void Enable(BOOL bEnable);
	BOOL	IsClickOnFocusIcon(POINT *pt);
	BOOL IsClickOnFocus(POINT *pt);
	BOOL IsClickOnItem(POINT *pt);
	INT GetCurIndex();
	wchar_t* GetCurFocusItemName(void);
	wchar_t* GetCurFocusItemSubName(void);
	wchar_t* GetCurFocusItemValue(void);
	LPARAM GetCurFocusItemUserParam(void);

	CListBoxItemGL* GetCurFocusItem(void);
	UINT GetCurFocusItemID(void);
	BOOL IsCurFocusItemVisible(void);
	void SetCurFocusItemName(const wchar_t *str);
	void SetCurFocusItemValue(const wchar_t *str);
	void SetCurFocusItemSubName(const wchar_t *str);

	void SetItemName(UINT id,const wchar_t *str);
	void SetItemValue(UINT id,const wchar_t *str);
	void SetItemSubName(UINT id,const wchar_t *str);
	void ShowKeyWords(BOOL bShow);

	int SearchName(const wchar_t *str);
	int SearchValue(const wchar_t *str);
	int SearchSubName(const wchar_t *str);

	int SearchID(UINT id);
	void DeleteItem(UINT id);
	void Show(BOOL bShow,BOOL bAni=FALSE,UINT iMotion=0);
	void DeleteCurFocusItem(void);
	BOOL IsOnQuickSearchMode();
	void	ShowQuickSearchMode(BOOL bShow);
	BOOL IsKeywordBtnVisible();

	GLfloat GetCurFocusItemPosX(void);
	GLfloat GetCurFocusItemPosY(void);
	INT GetCount();
	void SetAlignType(UINT idType);
	void SetMarginHor(int iSet);
	void SetBackFilled(BOOL bFilled);
	CLabelGL *GetCurFocusIcon();
	void SetBackAlpha(GLfloat fSet);

	CLabelGL *GetIndexIcon(UINT id);
	void SetIndexIcon2(UINT id,CLabelGL *pIconUp,CLabelGL *pIconDown=NULL);
	void SetIndexIcon(UINT id,CLabelGL *pIcon,CLabelGL *pIconFocus=NULL);
	void SetIndexIcon2(UINT id,CLabelGL *pIconUp,CLabelGL *pIconDown,GLfloat px, GLfloat py);
	void SetIndexIcon(UINT id,CLabelGL *pIcon,CLabelGL *pIconFocus,GLfloat px, GLfloat py);

	void SetIndexIcon2Checked(UINT id,BOOL bChecked);

	wchar_t *GetIndexValue(INT index);
	wchar_t *GetIndexName(INT index);
	wchar_t *GetIndexSubName(INT index);
	LPARAM GetIndexUserParam(INT index);

	void SetFocus(INT index);
	void KillFocus();
	void EnableMouseMove(BOOL bEnable);
	void EnableClickAni(BOOL bEnable);

public:
	CM_List<CListBoxItemGL> m_listItems;

private:
	void StopSlide(BOOL bAni=TRUE);
private:
	void ProcessNotify();
	void UpdateSearchWord();

private:
	CCriticalSection* m_DataCS;

	CTexture			*m_pTexBack;
	CTexture			*m_pTexIndexBase;
	CTexture			*m_pTexIndexFocus;
	CTexture			*m_pTexIndexClick;
	DWORD			m_dwHoldTick;
	BOOL				m_bPressed;
	BOOL		m_bHasSlider;
	BOOL		m_bBackFilled;
	BOOL		m_bWithSlider;
	BOOL		m_bHasHorSeperator;
	BOOL		m_bHasGlow;
	INT			m_iItemCounts;
	INT			m_iSpacing;
	INT			m_iIndexOffsetVer;			
	INT			m_iIndexOffsetVerOld;			
	INT			m_iIndexOffsetVerSet;			
	INT			m_iIndexOffsetVerLastPress;			
	INT			m_iMarginX;
	INT			m_iMarginY;
	INT			m_iMarginHor;
	INT			m_iVisibleNum;
	INT			m_iCurIndex;
	INT			m_iSlideSpeed;
	SIZE			m_siBorder;
	BOOL		m_bEnableDragging;
	BOOL		m_bClickOnIndex;
	BOOL		m_bDraging;
	BOOL		m_bEnableMouseMove;
	BOOL		m_bNeedRefreshPos;
	CListBoxItemGL *m_pVisibleItemLow;
	CListBoxItemGL *m_pVisibleItemHigh;
	CListBoxItemGL *m_pCurFocusItem;
	CListBoxItemGL *m_pOldFocusItem;
	CListBoxItemGL *m_pCurClickedItem;

	BOOL		m_bActiveSlide;
	DWORD	m_dwLastPressTick;
	DWORD	m_dwLastShowSliderTick;
	DWORD	m_dwLastKeyWordTick;

	BOOL		m_bShowKeyWords;
	BOOL		m_bControlReadOnly;
	UINT			m_idSortType;
	GLfloat				m_pxSlider;
	GLfloat				m_pySlider;
	GLfloat				m_fIndexBaseOffset;
	BOOL				m_bAutoHideSlider;
	BOOL				m_bEnableDoubleClick;
	BOOL				m_bClickAni;

	int					m_iCaptionHeightName;
	int					m_iCaptionHeightValue;
	int					m_iCaptionHeightSubName;
	GLfloat				m_iCaptionNameOffset;
	GLfloat				m_iCaptionValueOffset;
	GLfloat				m_iCaptionSubNameOffset;
	COLORGL			m_crNormal;
	COLORGL			m_crFocus;
	COLORGL			m_crClick;

	CSliderGL  m_sliControl;
	CButtonGL	m_tagKeyWord;

	BOOL			m_bShowQuickSearchBoard;
	CButtonGL	m_btnQuickSearch[27];
	CButtonGL	m_btnSearchEnter;
	CLabelGL		m_iconSearch;
	CButtonGL	m_btnDelete;
	CLabelGL		m_iconDelete;

	CM_String		m_strSearch;

	BOOL			m_bHasQuickSearchBoard;
	INT			m_iQuickSearchIndexCount[6][27];

	BOOL			m_bDrawBorder;
	UINT				m_idAlignType;
	GLfloat			m_fBackAlpha;

	BOOL			m_bValidClick;
	BOOL			m_bRendered;
	BOOL			m_bEnableScissor;
	BOOL			m_bEnableAutoEllipsis;
};
