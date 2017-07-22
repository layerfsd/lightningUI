//CVideoWindow.h : interface of the CVideoWindow class
//


#pragma once

#include <dvp.h>
#include "../FantasySDK/inc/ProConfig.h"
#include <windev.h>
#include "CVTE_IoCtl.h"
#include "Decoder.h"

#define DEBUG_VIDEO_WINDOW_MSG		0
#define DEBUG_VIDEO_WINDOW_ERR      1
#define DEBUG_VIDEO_WINDOW_TEST     0

//Video mode
#define  VIP_MODE_CCIR656    1
#define  VIP_MODE_YUV422     2
#define  VIP_MODE_RGB565     3


//	#define  VIP_CHANNEL_CVBS			 0  // CVBS
//	#define  VIP_CHANNEL_DTV			 1  // DTV
//	#define  VIP_CHANNEL_CVBS_R    	     2  // Reserve
//	#define  VIP_CHANNEL_YC			     3  // DVD
//	#define  VIP_CHANNEL_MHL		     4  // MHL
//	#define  VIP_CHANNEL_ARGB		     5  // ARGB
#define  VIP_CHANNEL_CMMB            0  // CMMB
#define  VIP_CHANNEL_CVBS			 1  // CVBS
#define  VIP_CHANNEL_DVD			 2  // DVD
#define  VIP_CHANNEL_DTV             3  // DTV
#define  VIP_CHANNEL_MHL             4  // MHL

//Zoom
#define  ZOOM_25          1
#define  ZOOM_50          2
#define  ZOOM_100         3
#define  ZOOM_15          4
#define  ZOOM_FULLSCREEN  5

#define VIP_WND_FULLSCREEN					0
#define VIP_WND_PIP_NORMAL					1
#define VIP_WND_PIP_SETTING					2
#define VIP_WND_PIP_REVERSE					3
#define VIP_WND_PIP_DISK							4
#define VIP_WND_PIP_REVERSE_FULL		   5

typedef enum COLOY_SYS
{
	COLOR_INVALID = -1,
	NTSM_MJ = 0,
	NTSC_443,
	PAL_M,
	PAL_60,
	PAL_BGHID,
	SECAM,
	PAL_COM_N,
	SECAM_525,
}COLOR_SYSTEM;

typedef enum
{
	DEINTERLACING_NONE = 0,
	DEINTERLACING_INTRA_FIELD_SPATIAL,
	DEINTERLACING_WEAVE,
	DEINTERLACING_3_TAP_MEDIAN,
	DEINTERLACING_VERTICAL_MEDIAN_RANKING
}DEINTERLACING_MODE;

typedef struct _DDVIDEOPORTSETTING
{
	DEINTERLACING_MODE dmMode;
	COLOR_SYSTEM       colorSys;
}DDVIDEOPORTSETTING;

#define  MENU_HIGH        0

typedef HRESULT (* DIRECTDRAWCREATE)( GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter );

typedef void (CALLBACK* CLICKPROC)(POINT *pt,LPVOID lparam);

typedef void (CALLBACK* GESTUREPROC)(WPARAM wParam, LPARAM lParam,LPVOID lParamUser);

class CVideoWindow
{
public:
	CVideoWindow();

private:
	BOOL   m_bVideoStart;
	DWORD  m_dwVideoMode;
	DWORD  m_dwZoomCoef;
	DWORD  m_dwSaveCoef;
	DWORD  m_dwPosition;
	DWORD  m_dwVBIHeight;
	//	DWORD  m_dwBpp;
	DWORD  m_bOverlay;
	DWORD  m_bMoving;
	int    m_iScreenWidth;
	int    m_iScreenHeight;
	DWORD  m_dwVideoChannel;
    DWORD  m_dwDecoderChannel;
	DWORD		m_dwStartTick;
	DWORD  m_dwSetModeTick;

	BOOL	m_bHasSignal;
	BOOL	m_bWindowVisible;

	int    m_iDstX,m_iDstY;
	int    m_iDstWidth,m_iDstHeight;
	int    m_iSrcWidth,m_iSrcHeight;

	int    m_iFramePerSecond;
	UINT	m_idWindowMode;

	CProConfig *m_pConfig;

	DIRECTDRAWCREATE     m_pDirectDrawCreate;
	HINSTANCE            m_hModule;
	DEINTERLACING_MODE   m_dmMode;
	COLOR_SYSTEM m_iColorSystem;

 	CLICKPROC m_pfnOnTouchDown;
 	CLICKPROC m_pfnOnTouchMove;
 	CLICKPROC m_pfnOnTouchUp;
 	CLICKPROC m_pfnOnDblClick;
	LPVOID    m_lparamClick;

	GESTUREPROC m_pfnOnGesture;
	LPVOID		m_lparamGesture;

	int m_iPixelFormat;
	HWND m_hWnd;

	LPDDVIDEOPORTCONTAINER      m_pVPContainer;// Video port container object

	LPDIRECTDRAWVIDEOPORT       m_pVideoPort ;  // Video port object

	DDPIXELFORMAT               m_vpInputFormats[4];  // Input format of the video port
	DDVIDEOPORTINFO             m_vpInfo;             // Video port information
	DDCOLORCONTROL              m_vpColorControl;     // Current color data of the video port


	LPDIRECTDRAW                m_pDD;        // DirectDraw object
	LPDIRECTDRAWSURFACE         m_pDDSPrimary ; // Primary Surface.
	LPDIRECTDRAWSURFACE         m_pDDSOverlay ; // The overlay primary.
// 	LPDIRECTDRAWSURFACE         m_pDDSOverlay2 ; // The test overlay primary.

	//LPDIRECTDRAWSURFACE         m_pDDSCurrVPSurf;     // Current surface that is written by video port
	DDCAPS						m_ddcaps;

public:
	BOOL Initialize(HINSTANCE hIns,CProConfig *pConfig);
	BOOL IsRunning();
	void GetWindowPos(int *px,int *py,int *pcx,int *pcy);
	void SetWindowMode(UINT idMode);
	UINT GetWindowMode();
	void SetVideoChannel(UINT idChannel);
	void SetColorSystem(COLOR_SYSTEM idColorSys);

	static LRESULT CALLBACK	VideoWndProc(HWND, UINT, WPARAM, LPARAM);

 	void SetClickFunc(CLICKPROC pfuncDown,CLICKPROC pfuncUp,CLICKPROC pfuncMove,CLICKPROC pfuncDblClick,LPVOID lparam);

	void SetGestureFunc(GESTUREPROC pfuncGesture,LPVOID lpContext);

	DDCOLORCONTROL *GetColorControl(){return &m_vpColorControl;}
	BOOL SetColorControl(DDCOLORCONTROL *pColorCtl);

	BOOL HasVideoSignal(){return m_bHasSignal;}
	DWORD GetStartTick(){return m_dwStartTick;}
	DWORD GetLastSetModeTick(){return m_dwSetModeTick;}

	// Operations
public:
	VOID   ReleaseAllObject();
	VOID   StartVideo();
	VOID   StopVideo();

protected:
	BOOL ShowVideoWindow(BOOL bShow);
	BOOL GetVideoSignalStatus(DWORD *pdwStatus);
	void SetPos(int x,int y,int cx,int cy);

	// Overrides
public:

protected:
	void UpdateColorSystemParam();
	void SetDeinterlaceMode(DEINTERLACING_MODE dmMode);
	void OnSignalDetect();

	// Implementation
public:
	virtual ~CVideoWindow();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:
	int Zoom(int data);

};

