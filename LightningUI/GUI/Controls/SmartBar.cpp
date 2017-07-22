// CSmartBar.cpp : implementation of the CSmartBar class
//
#include "SmartBar.h"

#include <pkfuncs.h>


#define TIMER_SMART_BAR_TIMEOUT					303
#define TIMER_SMART_BAR_UPDATE_FRAME		304

#define ALIGN(d, a)  (((d)+(a)-1)&~((a)-1))

DDPIXELFORMAT s_PixelFormats[] =
{
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0},        // 16-bit RGB 5:6:5
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0},        // 32-bit RGB 8:8:8
	{sizeof(DDPIXELFORMAT), DDPF_RGB|DDPF_ALPHAPIXELS|DDPF_ALPHAPREMULT, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0xFF000000},        // 32-bit ARGB 8:8:8
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xFc00, 0x03e0, 0x001F, 0},        // 16-bit RGB 6:5:5
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07c0, 0x003F, 0}        // 16-bit RGB 5:5:6
};

CM_List<CSmartBar> g_ListSmartBar;

CSmartBar::CSmartBar()
{
	// TODO: add construction code here

	m_pDD = NULL;        // DirectDraw object
	m_pDDSPrimary = NULL; // Primary Surface.
	m_pDDSOverlay = NULL; // The overlay primary.
	m_pDDSBack = NULL;

	m_bOverlayOnly = TRUE;

	m_hWnd = NULL;
	m_hMainWnd = NULL;

	 m_iOverlayWidth=g_iScreenWidth;
	 m_iOverlayHeight=100;

	 m_iOverlayX       = 0;
	 m_iOverlayY       = g_iScreenHeight- m_iOverlayHeight;

	m_bVisible = FALSE;

	m_bEnableShow = TRUE;

	m_bNeedUpdateFrame = TRUE;
	m_dwLastShowTick=0;
	m_dwShowTimeOut=5000;

	m_bClickActivate = FALSE;

	m_pSkinManager = NULL;
}

CSmartBar::~CSmartBar()
{
	ReleaseAll();

	if(m_hWnd)
	{
		DestroyWindow(m_hWnd);
	}
}

BOOL CSmartBar::OnTimeOut()
{
	RETAILMSG(DEBUG_SMARTBAR, (TEXT("---CSmartBar::OnTimeOut-- \r\n")));

	if(GetTickCount()-m_dwLastShowTick>=m_dwShowTimeOut)
		return Show(FALSE);
	else
		return FALSE;
}
BOOL CSmartBar::OnUpdateFrame()
{
	if(m_bClickActivate)
	{
		m_dwLastShowTick = GetTickCount();
	}
	if(m_bNeedUpdateFrame)
	{
		Render();
		m_bNeedUpdateFrame = FALSE;
	}

	return TRUE;
}
void CSmartBar::EnableShow(BOOL bEnable)
{
	if(!bEnable)
		Show(FALSE);
	m_bEnableShow = bEnable;
}

BOOL CSmartBar::Show(BOOL bShow,UINT iTimeOut)
{
	BOOL bRet = FALSE;

	RETAILMSG(DEBUG_SMARTBAR, (TEXT("MSG:[%s]: bShow:%d m_bEnableShow:%d m_bVisible:%d m_bOverlayOnly:%d\r\n"),TEXT(__FUNCTION__),bShow,m_bEnableShow,m_bVisible,m_bOverlayOnly));

	if(bShow && !m_bEnableShow)
	{
		goto _exit;
	}

	m_bNeedUpdateFrame = TRUE;
	m_dwShowTimeOut = iTimeOut;

	if(bShow)
	{
		if(m_bVisible)
		{
			KillTimer(m_hWnd,TIMER_SMART_BAR_TIMEOUT);
		}
		if(!SetTimer(m_hWnd,TIMER_SMART_BAR_TIMEOUT,iTimeOut,NULL))
		{
			RETAILMSG(DEBUG_SMARTBAR, (TEXT("MSG:[%s]:---SetTimer FAILED! :%d--- \r\n"),TEXT(__FUNCTION__),TIMER_SMART_BAR_TIMEOUT));
			bRet = FALSE;
			goto _exit;
		}
		m_dwLastShowTick=GetTickCount();
	}

	if(bShow == m_bVisible)
	{
		bRet = TRUE;
		goto _exit;
	}

	if(bShow)
	{
		if(InitializeOverlay())
		{
			m_bVisible = TRUE;

			//RETAILMSG(DEBUG_SMARTBAR, (TEXT("MSG:[%s]:m_bOverlayOnly:%d \r\n"),TEXT(__FUNCTION__),m_bOverlayOnly));

			if(!m_bOverlayOnly)
			{
				ShowWindow(m_hWnd,SW_SHOW);
				::SetWindowPos(m_hWnd, HWND_TOPMOST ,  m_iWndX,  m_iWndY,  m_iWndWidth,  m_iWndHeight,  0 );
			}
			SetTimer(m_hWnd,TIMER_SMART_BAR_UPDATE_FRAME,50,NULL);

			bRet = TRUE;


			::PostMessage(m_hMainWnd,WM_SMARTBAR_NOTIFY,m_id,1);

			//goto _exit;
		}
	}
	else
	{
// 		::PostMessage(m_hMainWnd,WM_SMARTBAR_NOTIFY,m_id,0);

		KillTimer(m_hWnd,TIMER_SMART_BAR_UPDATE_FRAME);

		if(!m_bOverlayOnly)
		{
			ShowWindow(m_hWnd,SW_HIDE);
		}

		if(ReleaseOverlay())
		{
			m_bVisible = FALSE;
			bRet = TRUE;
			//goto _exit;
		}

		::PostMessage(m_hMainWnd,WM_SMARTBAR_NOTIFY,m_id,0);
	}

_exit:

	if(!bRet)
	{
		RETAILMSG(DEBUG_SMARTBAR, (TEXT("MSG:[%s]:---CSmartBar Show bShow:%d, m_bEnableShow:%d --- FAILED!\r\n"),TEXT(__FUNCTION__),bShow,m_bEnableShow));
	}

	return bRet;
}
BOOL CSmartBar::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns,int pxOverlay,int pyOverlay,int cxOverlay,int cyOverlay,BOOL bOverlayOnly,
								LPCTSTR lpWndName,int pxWnd,int pyWnd,int cxWnd,int cyWnd)
{
	// TODO: Add your specialized code here and/or call the base class

	m_pSkinManager = pSkinManager;
	m_id=id;
	m_bOverlayOnly = bOverlayOnly;

	 m_iOverlayWidth=cxOverlay;
	 m_iOverlayHeight=cyOverlay;

	 m_iOverlayX       = pxOverlay;//g_iScreenWidth/2- m_iOverlayWidth/2;
	 m_iOverlayY       = pyOverlay;//g_iScreenHeight/2- m_iOverlayHeight/2;

	m_iWndX=cxWnd?pxWnd:pxOverlay;
	m_iWndY=cyWnd?pyWnd:pyOverlay;
	m_iWndWidth=cxWnd?cxWnd:cxOverlay;
	m_iWndHeight=cyWnd?cyWnd:cyOverlay;

	m_hMainWnd = hWnd;

	//window:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WNDCLASS wc = {0};
	memset (&wc, 0, sizeof(wc));

	// Register  window class
	wc.style            =  0;//CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc      = SmartBarWndProc;
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = hIns;
	wc.hIcon            = NULL;
	wc.hCursor          = NULL;
	wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wc.lpszMenuName     = NULL;
	wc.lpszClassName    = lpWndName;

	RegisterClass(&wc) ;

	// Create Main Window.placeholder. Not visible by default
	m_hWnd  = ::CreateWindowEx(
		WS_EX_NOANIMATION,
		lpWndName,
		lpWndName,
		WS_POPUP,//WS_CHILD ,// WS_CLIPSIBLINGS | WS_CLIPCHILDREN| ,//,//
		m_iWndX,//CW_USEDEFAULT,
		m_iWndY,//CW_USEDEFAULT,
		m_iWndWidth,//CW_USEDEFAULT,
		m_iWndHeight,//CW_USEDEFAULT,
		NULL/*g_hMainWnd*/,
		NULL,
		hIns,
		this);

	UpdateWindow(m_hWnd);
	ShowWindow(m_hWnd,SW_HIDE);

	//ddraw:
	InitializeDDraw();

	return TRUE;
}
BOOL CSmartBar::InitializeDDraw()
{
	//initialize ddraw:
	DDSURFACEDESC       ddsd;
	HRESULT              hRet;

	//Create DDraw Object
	hRet = DirectDrawCreate(NULL, &m_pDD, NULL);
	if (hRet != DD_OK)
	{
		RETAILMSG(DEBUG_SMARTBAR,(_T("DirectDrawCreate FAILED!\r\n")));
		return FALSE;
	}

	// Get normal mode
	hRet = m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
	if (hRet != DD_OK)
	{
		ReleaseDDraw();
		RETAILMSG(DEBUG_SMARTBAR,(_T("SetCooperativeLevel FAILED!\r\n")));
		return FALSE;
	}

	// Create the primary surface
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE ;

	hRet = m_pDD->CreateSurface(&ddsd, &m_pDDSPrimary, NULL);
	if (hRet != DD_OK)
	{
		ReleaseDDraw();
		RETAILMSG(DEBUG_SMARTBAR,(_T("Create primary surface FAILED!\r\n")));
		return FALSE;
	}


	//background surface:
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY ;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH |	DDSD_PIXELFORMAT;

	ddsd.dwWidth           =  m_iOverlayWidth;
	ddsd.dwHeight          =  m_iOverlayHeight;

	ddsd.ddpfPixelFormat   = s_PixelFormats[2];

	hRet = m_pDD->CreateSurface(&ddsd, &m_pDDSBack, NULL);

	if (hRet != DD_OK)
	{
		ReleaseDDraw();
		RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar::InitializeDDraw, Create surface  FAILED! 0x%x\r\n"),hRet));
		return FALSE;
	}


	return TRUE;
}
BOOL CSmartBar::InitializeOverlay()
{
	DDSURFACEDESC       ddsd;
	HRESULT              hRet;

	if(!m_pDD)
		return FALSE;
	if(!m_pDDSPrimary)
		return FALSE;

	if(m_pDDSOverlay)
		return TRUE;

	RETAILMSG(DEBUG_SMARTBAR, (TEXT("MSG:[%s]: \r\n"),TEXT(__FUNCTION__)));


	// See if we can support overlays.
	memset(&m_ddcaps, 0, sizeof(m_ddcaps));
	m_ddcaps.dwSize = sizeof(m_ddcaps);
	hRet = m_pDD->GetCaps(&m_ddcaps,NULL);
	if (hRet != DD_OK)
	{
		ReleaseDDraw();
		RETAILMSG(DEBUG_SMARTBAR,(_T("GetCaps FAILED!\r\n")));
		return FALSE;
	}

	RETAILMSG(DEBUG_SMARTBAR,(_T("GetCaps dwMaxVisibleOverlays:%d,dwCurrVisibleOverlays:%d\r\n"),m_ddcaps.dwMaxVisibleOverlays,m_ddcaps.dwCurrVisibleOverlays));

	if (0 == (m_ddcaps.ddsCaps.dwCaps & DDSCAPS_OVERLAY))
	{
		ReleaseDDraw();
		RETAILMSG(DEBUG_SMARTBAR,(_T("Overlays are not supported in hardware!!\r\n")));
		return FALSE;
	}

	if (m_ddcaps.dwAlignSizeDest)
	{
		 m_iOverlayWidth = ALIGN( m_iOverlayWidth, m_ddcaps.dwAlignSizeDest);
		RETAILMSG(DEBUG_SMARTBAR,(_T("m_ddcaps.dwAlignSizeDest is not 0!,  m_iOverlayWidth:%d\r\n"), m_iOverlayWidth));
	}

	//test overlay://////////////////////////////////
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_VIDEOMEMORY ;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH/* | DDSD_BACKBUFFERCOUNT*/ | DDSD_PIXELFORMAT;


	ddsd.dwWidth           =  m_iOverlayWidth;
	ddsd.dwHeight          =  m_iOverlayHeight;
	ddsd.dwBackBufferCount = 0;//1;
	ddsd.ddpfPixelFormat   = s_PixelFormats[2];

	hRet = m_pDD->CreateSurface(&ddsd, &m_pDDSOverlay, NULL);
	if (hRet != DD_OK)
	{
		ReleaseDDraw();
		RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar Create Overlay surface  FAILED! 0x%x\r\n"),hRet));
		return FALSE;
	}

	/*
	hRet = m_pDDSOverlay->UpdateOverlayZOrder(DDOVERZ_SENDTOBACK,NULL);
	if (hRet != DD_OK)
	{
// 		RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar UpdateOverlayZOrder  FAILED! 0x%x\r\n"),hRet));
		if(DDERR_INVALIDOBJECT == hRet)
			RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar UpdateOverlayZOrder  FAILED! DDERR_INVALIDOBJECT\r\n")));
		else if(DDERR_INVALIDPARAMS  == hRet)
			RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar UpdateOverlayZOrder  FAILED! DDERR_INVALIDPARAMS \r\n")));
		else if(DDERR_NOTAOVERLAYSURFACE  == hRet)
			RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar UpdateOverlayZOrder  FAILED! DDERR_NOTAOVERLAYSURFACE \r\n")));
		else
			RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar UpdateOverlayZOrder  FAILED! 0x%x\r\n"),hRet));
	}
	*/

	//initialize the surface
	DDBLTFX sBltFX = {0};
	sBltFX.dwSize = sizeof(sBltFX);
	sBltFX.dwFillColor = /*0x0*/RGBA(0,0,0,0);//RGBA(118,109,98,255);

	RECT rc2 = {0, 0,  m_iOverlayWidth,  m_iOverlayHeight};
	hRet = m_pDDSOverlay->Blt(&rc2, NULL, NULL, DDBLT_COLORFILL, &sBltFX);

	if (hRet !=DD_OK)
	{
		RETAILMSG(DEBUG_SMARTBAR,(_T("Initialize Overlay surface to 0x0 FAILED!\r\n")));
		return FALSE;
	}

	m_dwUpdateFlags = DDOVER_SHOW ;
	memset(&m_ovfx, 0, sizeof(m_ovfx));
	m_ovfx.dwSize = sizeof(m_ovfx);

// 	if (m_ddcaps.dwOverlayCaps & DDOVERLAYCAPS_ALPHACONSTANT)
// 	{
// 		m_dwUpdateFlags |= DDOVER_ALPHACONSTOVERRIDE;
// 		m_ovfx.dwAlphaConst = 0xE0;
// 		m_ovfx.dwAlphaConstBitDepth = 8;
// 	}

	RETAILMSG(DEBUG_SMARTBAR,(_T("m_ddcaps.dwOverlayCaps:0x%x!\r\n"),m_ddcaps.dwOverlayCaps));


	m_dwUpdateFlags |= DDOVER_ALPHASRC;


	RECT  rs,rd;

	rs.left   = 0;
	rs.right  =  m_iOverlayWidth;
	rs.top    = 0;
	rs.bottom =  m_iOverlayHeight;

	rd.left   =  m_iOverlayX;
	rd.right  = rd.left +  m_iOverlayWidth;
	rd.top    =  m_iOverlayY;
	rd.bottom = rd.top +  m_iOverlayHeight;

	hRet = m_pDDSOverlay->UpdateOverlay(&rs, m_pDDSPrimary, &rd, m_dwUpdateFlags, &m_ovfx);

	if (hRet != DD_OK)
	{
		ReleaseOverlay();
		RETAILMSG(DEBUG_SMARTBAR,(_T("Update Overlay FAILED!\r\n")));
		return FALSE;
	}

	RETAILMSG(DEBUG_SMARTBAR,(_T("++InitializeOverlay SUCCEED!\r\n")));

	return TRUE;
}

BOOL CSmartBar::GetStringExtensionArt(const wchar_t*pString,SIZE *si)
{
	if(!si || !pString)
		return FALSE;


	SIZE si_c;

	const wchar_t *pCursor=pString;

	si->cx=0;
	si->cy=0;

	while (*pCursor)
	{
		DrawChar(NULL,*pCursor,0,0,&si_c);
		si->cx+=si_c.cx;
		si->cy=si_c.cy;
		pCursor++;
	}

	return TRUE;
}

BOOL CSmartBar::GetStringExtensionGDI(const wchar_t*pString,SIZE *si)
{
	if(!si || !pString)
		return FALSE;

	return TRUE;
}

void CSmartBar::DrawStringGDI(LPDIRECTDRAWSURFACE pSurface, const wchar_t*pString, RECT *lpRect,UINT uFormat, INT iHeight,COLORREF cr)
{
	if(!pString || !pSurface || !lpRect)
		return;

	int cbCount = wcslen(pString);

	if(cbCount==0)
		return;

	HDC hDC = (HDC)INVALID_HANDLE_VALUE;

	if (pSurface->GetDC(&hDC) == DD_OK)
	{
		HFONT hFontNew;
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));

		lf.lfHeight = 0;//
		lf.lfItalic =FALSE;
		lf.lfOrientation = 0;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;//OUT_RASTER_PRECIS;//
		lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		lf.lfQuality = ANTIALIASED_QUALITY;//CLEARTYPE_QUALITY;//DEFAULT_QUALITY;//
		lf.lfStrikeOut = FALSE;
		lf.lfUnderline =FALSE;
		lf.lfWidth = 0;
		lf.lfWeight =FW_NORMAL;//FW_THIN;//
		lf.lfHeight = -((iHeight) * GetDeviceCaps(hDC, LOGPIXELSY) / 72);

// 		lf.lfHeight = iHeight;
// 		lf.lfQuality = CLEARTYPE_QUALITY;
// 		lf.lfWeight = FW_THIN;
// 		lf.lfCharSet  = GB2312_CHARSET;
// 		wcscpy_s(lf.lfFaceName,32,_T("Arial"));

		if(m_pSkinManager)
		{
			switch (m_pSkinManager->GetConfig()->GetCurParam()->idFont)
			{
			case FONT_Tahoma:
				wcscpy_s(lf.lfFaceName,32, _T("Tahoma"));
				break;
			case FONT_Arial:
				wcscpy_s(lf.lfFaceName,32, _T("Arial"));
				break;
			case FONT_Myriad_Pro:
				wcscpy_s(lf.lfFaceName,32, _T("Myriad Pro"));
				break;
			case FONT_Times_New_Roman:
				wcscpy_s(lf.lfFaceName,32, _T("Times New Roman"));
				break;
			case FONT_Segoe_UI:
				wcscpy_s(lf.lfFaceName,32, _T("Segoe UI"));
				break;
			case FONT_Courier_New:
				wcscpy_s(lf.lfFaceName,32, _T("Courier New"));
				break;
			case FONT_Helvetica:
				wcscpy_s(lf.lfFaceName,32, _T("HelveticaNeueLT Pro 45 Lt"));
				break;
			default:
				wcscpy_s(lf.lfFaceName,32, _T("Tahoma"));
				break;
			}
		}
		else
		{
			wcscpy_s(lf.lfFaceName,32,_T("Tahoma"));
		}

		SetBkColor(hDC, 0);
		SetBkMode(hDC, TRANSPARENT);
		hFontNew = CreateFontIndirect(&lf);
		HGDIOBJ hFontOld = SelectObject(hDC, hFontNew);
		SetTextColor(hDC, cr);

		DrawText(hDC,pString, cbCount, lpRect,uFormat);

		SelectObject(hDC, hFontOld);
		DeleteObject(hFontNew);

		pSurface->ReleaseDC(hDC);
	}
}

void CSmartBar::DrawStringArt(LPDIRECTDRAWSURFACE pSurface,const wchar_t*pString,int x,int y)
{
	if(!pString || !pSurface)
		return;

	int x_offset =x;

	SIZE si;
	const wchar_t *pCursor=pString;

	while (*pCursor)
	{
		DrawChar(pSurface,*pCursor,x_offset,y,&si);
		x_offset+=si.cx;
		pCursor++;
	}
}
BOOL CSmartBar::FillSurface(LPDIRECTDRAWSURFACE *pSurface,const unsigned char *pBuff,int cx,int cy,int depth)
{

	DDSURFACEDESC       ddsd;
	HRESULT              hRet;

	if(!pSurface)
		return FALSE;
	if(!pBuff)
		return FALSE;

	LPDIRECTDRAWSURFACE pSurOld = *pSurface;

	LPDIRECTDRAWSURFACE pSurCreate =NULL;


	if(!m_pDD)
		return FALSE;
	if(!m_pDDSPrimary)
		return FALSE;

	if(pSurOld != NULL)
	{
		pSurOld->Release();
		pSurOld=NULL;
	}

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.ddsCaps.dwCaps =DDSCAPS_VIDEOMEMORY ;
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH |	DDSD_PIXELFORMAT;

	ddsd.dwWidth           =cx;
	ddsd.dwHeight          =cy;
	ddsd.ddpfPixelFormat   = s_PixelFormats[2];

	hRet = m_pDD->CreateSurface(&ddsd, &pSurCreate, NULL);

	if (hRet != DD_OK)
	{
		ReleaseDDraw();
		RETAILMSG(DEBUG_SMARTBAR,(_T("CSmartBar::FillSurface, Create surface  FAILED!\r\n")));
		return FALSE;
	}

	//initialize the surface
// 	DDBLTFX sBltFX = {0};
// 	sBltFX.dwSize = sizeof(sBltFX);
// 	sBltFX.dwFillColor = /*0x0*/RGBA(0xff,0x00,0x00,0xa0);
//
// 	RECT rc1 = {0, 0, image.m_cx, image.m_cy};
// 	hRet =pSurCreate->Blt(&rc1, NULL, NULL, DDBLT_COLORFILL, &sBltFX);

	hRet = pSurCreate->Lock( NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);
	if (DD_OK != hRet)
	{
		pSurCreate->Release();
		pSurface=NULL;
		return FALSE;
	}

	UINT8   *pbySrcBuffer=NULL,*pbyDstBuffer = NULL;
	pbyDstBuffer = reinterpret_cast<PUINT8>(ddsd.lpSurface);

	int iLoopCount = cy;
	int iCopySize=cx*depth;
	pbySrcBuffer=(UINT8*)pBuff;

	for (; iLoopCount > 0; iLoopCount--)
	{
		UINT8 *cursor=pbySrcBuffer;
		UINT8 t0;
		for (int i=0;i<cx;i++)
		{
			t0=cursor[0];
			cursor[0]=cursor[2];
			cursor[2]=t0;
			cursor+=depth;
		}
		memcpy(pbyDstBuffer,pbySrcBuffer,iCopySize);
		pbySrcBuffer += iCopySize;
		pbyDstBuffer += ddsd.lPitch;
	}

	hRet = pSurCreate->Unlock(NULL);
	if (hRet !=DD_OK)
	{
		pSurCreate->Release();
		pSurface=NULL;
		return FALSE;
	}

	*pSurface = pSurCreate;

	return TRUE;
}

void CSmartBar::ReleaseSurface(LPDIRECTDRAWSURFACE pSurface)
{
	if (pSurface != NULL)
	{
		pSurface->Release();
		pSurface = NULL;
	}
}
VOID CSmartBar::ReleaseDDraw()
{
	if (m_pDDSPrimary != NULL)
	{
		m_pDDSPrimary->Release();
		m_pDDSPrimary = NULL;
	}

	if (m_pDDSBack != NULL)
	{
		m_pDDSBack->Release();
		m_pDDSBack = NULL;
	}

	if (m_pDD != NULL)
	{
		m_pDD->Release();
		m_pDD = NULL;
	}
}
BOOL CSmartBar::ReleaseOverlay()
{
	HRESULT              hRet;

	if (m_pDDSOverlay != NULL)
	{
		// Use UpdateOverlay() with the DDOVER_HIDE flag to remove an overlay
		// from the display.
		hRet = m_pDDSOverlay->UpdateOverlay(NULL, m_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
		if (hRet != DD_OK)
		{
			return FALSE;
		}
		hRet = m_pDDSOverlay->Release();
		if (hRet != DD_OK)
		{
			return FALSE;
		}
		m_pDDSOverlay = NULL;
	}

	return TRUE;
}
VOID CSmartBar::ReleaseAll()
{
	ReleaseOverlay();
	ReleaseRes();
	ReleaseDDraw();
}
LRESULT CALLBACK CSmartBar::SmartBarWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	POINT pt;

	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		{
			CSmartBar *pNewBar =(CSmartBar *)(((CREATESTRUCT*)lParam)->lpCreateParams);
			g_ListSmartBar.Add(pNewBar,(INT)hWnd);
		}
		break;
	case WM_SETCURSOR:
		{

		}
		break;
	case WM_TIMER:
		{
			CSmartBar *pCurrentBar = g_ListSmartBar.GetItemByID((INT)hWnd);

			switch (wParam)
			{
			case TIMER_SMART_BAR_TIMEOUT:
				{
					if(pCurrentBar)
					{
						if(pCurrentBar->OnTimeOut())
						{
							KillTimer(hWnd,TIMER_SMART_BAR_TIMEOUT);
						}
					}
				}
				break;
			case TIMER_SMART_BAR_UPDATE_FRAME:
				{
					if(pCurrentBar)
					{
						pCurrentBar->OnUpdateFrame();
					}
				}
				break;
			}
		}
		break;
	case 	WM_ERASEBKGND:
		{
			/*
			RETAILMSG(1,(_T("+++++CSmartBar::SmartBarWndProc WM_ERASEBKGND!!!!....\r\n")));

			RECT rc;
			static HBRUSH br=CreateSolidBrush(RGB(8,8,8));
			GetClientRect(hWnd,&rc);
			::FillRect(GetDC(hWnd), &rc, br);

			CSmartBar *pCurrentBar = g_ListSmartBar.GetItemByID((INT)hWnd);

			if(pCurrentBar)
			{
				pCurrentBar->Render();
			}
			*/
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
// 		if(pBar)
// 		{
// 			pBar->Render();
// 		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		break;
	case WM_COPYDATA:
		{
			PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
// 			RETAILMSG(DEBUG_SMARTBAR, (_T("**SmartBarWndProc -WM_COPYDATA dwData:0x%x\r\n"),pcds->dwData));
		}
		break;
	case WM_LBUTTONDOWN:
		{
			CSmartBar *pCurrentBar = g_ListSmartBar.GetItemByID((INT)hWnd);
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			if(pCurrentBar)
			{
				pCurrentBar->OnTouchDown(&pt);
				pCurrentBar->m_bNeedUpdateFrame = TRUE;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CSmartBar *pCurrentBar = g_ListSmartBar.GetItemByID((INT)hWnd);
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			if(pCurrentBar)
			{
				pCurrentBar->OnTouchUp(&pt);
				pCurrentBar->m_bNeedUpdateFrame = TRUE;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			CSmartBar *pCurrentBar = g_ListSmartBar.GetItemByID((INT)hWnd);
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			if(pCurrentBar)
			{
				pCurrentBar->OnTouchMove(&pt);
				pCurrentBar->m_bNeedUpdateFrame = TRUE;
			}
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
