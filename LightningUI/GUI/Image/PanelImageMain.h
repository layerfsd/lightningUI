#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "ImageCommon.h"
#include "CriticalSection.h"

#define IMAGE_PRLOAD_NUM 3

typedef enum _IMAGE_ACTION_
{
    IMAGE_ACTION_ALL = 0,
    IMAGE_ACTION_LEFT_MOVE,
    IMAGE_ACTION_RIGHT_MOVE
};

 typedef struct _IMAGE_NODE
{
    CLabelGL * ptagImage;
    wchar_t    strFileName[MAX_PATH];
    SIZE       si;
//    wchar_t    strFilePath[MAX_PATH];
//    BOOL       bImageValid;
} IMAGE_NODE, *PIMAGE_NODE;

class CPanelImageMain :
	public CPanelGL
{
public:
	CPanelImageMain(void);
	~CPanelImageMain(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);
    void OnGesture(GESTUREINFO *pgi);

	void OnGestureMsg(UINT idGesture);
    void OnTouchMsg(PMULTIPOINT pCyMultiPoint);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

    BOOL LoadImageFile(const wchar_t *pFileName);
//    BOOL LoadPrevImage(const wchar_t *pFileName);
//    BOOL LoadNextImage(const wchar_t *pFileName);
    void LoadImageReady(BOOL bReday){m_bImageLoadReady=bReday;}
    static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
//    BOOL LoadImageHD(const wchar_t *pFileName);
	void ImagePlayAbort();

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
	static void ImageAutoPlayTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);


private:
	void ZoomIn();
	void ZoomOut();
    BOOL LoadIFile(const wchar_t *pFileName, CLabelGL * ptagImage);
    void Lock(){m_pCS->Enter();m_bLock=TRUE;}
    void Unlock(){m_pCS->Leave();m_bLock=FALSE;}
    BOOL IsLock(){return m_bLock;}
    BOOL LoadFile(const wchar_t *pFileName, CLabelGL * ptagImage, int cx, int cy);
    void PointUpdate(UINT uAction);
    void InforUpdate();
    void RightMove(int n);
    void LeftMove(int n);
    void RightMoveRelease(int n);
    void LeftMoveRelease(int n);
    BOOL ReleaseImageHD();

private:
	CCriticalSection* m_pCS;

	CButtonGL		m_tagTitle;
	CButtonGL       m_tagButtonBG;
	CM_String		m_strCurPlayFile;

	CLabelGL		m_iconLibraryImage;
	CLabelGL		m_iconPrev;
	CLabelGL		m_iconNext;
	CLabelGL		m_iconWallPaper;
	CLabelGL        m_iconRotate;
    //CLabelGL        m_iconBrowse;
    CLabelGL        m_iconZoom;
    CLabelGL        m_iconLoading;

	CLabelGL        m_iconPlay;
	CLabelGL        m_iconPause;

	CButtonGL		m_btnLibraryImage;
    CButtonGL       m_btnZoomIn;
	CButtonGL		m_btnPrev;
	CButtonGL		m_btnNext;
    CButtonGL       m_btnZoomOut;
 	CButtonGL       m_btnRotate;
 	CButtonGL       m_btnBrowse;
 	CButtonGL       m_btnZoom;
	CButtonGL		m_btnSetWallPaper;
	CButtonGL       m_btnPlayPause;

   	CSliderGL		m_sliRotate;
	CLabelGL		m_iconAlert;

    CLabelGL        m_tagImage1;
    CLabelGL        m_tagImage2;
    CLabelGL        m_tagImage3;

    CLabelGL *      m_pImage1;  //Image memory point
    CLabelGL *      m_pImage2;
    CLabelGL *      m_pImage3;

    IMAGE_NODE      m_pImageNode[IMAGE_PRLOAD_NUM];

    UINT            m_nRotateSliValue;

    BOOL            m_bRotateSliShow;
    BOOL            m_bImage1Valid;
    BOOL            m_bImage2Valid;
    BOOL            m_bImageLoadReady;
    BOOL            m_bImage3Valid;

//    PLIST_NODE      m_pListNode;
//    LIST_ENTRY      m_ListHead;

	CLabelGL *  m_ptagImageDisplay;
	CLabelGL *  m_ptagImageDisplayHD;
	CLabelGL *  m_ptagImageDisplayPrev;
	CLabelGL *  m_ptagImageDisplayNext;

    wchar_t     m_strImageDisplayPath[MAX_PATH];

	BOOL        m_bImagePrevIsEmpty;
	BOOL        m_bImageNextIsEmpty;
	BOOL        m_bImageDisplayHDReady;
	BOOL        m_bImageDisplayHDShow;
	DWORD       m_dwImageDisplayHDLoadTick;

	GLfloat		m_fZoomRate;

	DWORD	    m_dwShowControlTick;
	DWORD       m_dwTouchDownTick;
	BOOL		m_bShowControl;
	BOOL		m_bEnableFreeDrag;
	BOOL        m_bEnableGestureSwitch;
    //BOOL        m_bGetFirstDown;
    BOOL        m_bLock;
    BOOL        m_bFirstMove;
    BOOL        m_bDoubleClick;

	BOOL   m_bImagePlaying;  

	GLfloat     m_px;
	GLfloat     m_py;
	GLfloat     m_Prevpx;
	GLfloat     m_Nextpx;
	INT         m_iMoveRange;
    POINT       m_ptPenDown;
    POINT       m_ptPoint1;
    POINT       m_ptPoint2;
};

