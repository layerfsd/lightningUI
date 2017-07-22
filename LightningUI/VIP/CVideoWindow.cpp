// CVideoWindow.cpp : implementation of the CVideoWindow class
//

#include "..\stdafx.h"
#include "CVideoWindow.h"
#include "../Backlight/Backlight.h"
#include "../GUI/Common/message_map.h"

extern HWND					g_hUIWnd;

//VBI height is a experience value now, if sets too small or large, video play back has corrouption(screen division) or frame packing
#define VBIHEIGHT_NTSC			15  //15	//14
#define VBIHEIGHT_PAL			1	//8
#define VBIHEIGHT_PAL_60_M		15
#define VBIHEIGHT_SECAM      8

#define VBIWIDTH   0

//VPE driver also use these names to get the setting, so if these changed, driver also need to be changed.
#define REGNAME_DEINTERLACE_MODE    L"deinterlacing_mode"
#define REGNAME_COLORSYS            L"color_system"

#define ALIGN(d, a)  (((d)+(a)-1)&~((a)-1))

#define Dump(data, mode)      printf("%s(%d): "#data" = "#mode"\r\n", __FUNCTION__, __LINE__, data)

#define CASE_RETURN_STR(x)       case x:   return _T(#x);

#define TIMER_VIDEOIN_GETSTATUS								129

/*
LPDDVIDEOPORTCONTAINER      m_pVPContainer = NULL;// Video port container object

LPDIRECTDRAWVIDEOPORT       m_pVideoPort = NULL;  // Video port object

DDPIXELFORMAT               m_vpInputFormats[4];  // Input format of the video port
DDVIDEOPORTINFO             m_vpInfo;             // Video port information
DDCOLORCONTROL              m_vpColorControl;     // Current color data of the video port

LPDIRECTDRAW                m_pDD = NULL;        // DirectDraw object
LPDIRECTDRAWSURFACE         m_pDDSPrimary = NULL; // Primary Surface.
LPDIRECTDRAWSURFACE         m_pDDSOverlay = NULL; // The overlay primary.
LPDIRECTDRAWSURFACE         m_pDDSOverlay2 = NULL; // The overlay 2.

//LPDIRECTDRAWSURFACE         m_pDDSCurrVPSurf;     // Current surface that is written by video port
DDCAPS						m_ddcaps;
*/

extern HWND					g_hMainWnd;

//define some GUIDs to suppot complie. values must keep same to the defines in dvp.h
const GUID g_HREFH_VREFH           = {0x54F39980,0xDA60,0x11CF,
0x9B,0x06,0x00,0xA0,0xC9,0x03,0xA3,0xB8};
const GUID g_HREFH_VREFL           = {0x92783220,0xDA60,0x11CF,
0x9B,0x06,0x00,0xA0,0xC9,0x03,0xA3,0xB8};
const GUID g_HREFL_VREFH           = {0xA07A02E0,0xDA60,0x11CF,
0x9B,0x06,0x00,0xA0,0xC9,0x03,0xA3,0xB8};
const GUID g_HREFL_VREFL           = {0xE09C77E0,0xDA60,0x11CF
,0x9B,0x06,0x00,0xA0,0xC9,0x03,0xA3,0xB8};
const GUID g_DDVPTYPE_CCIR656      = {0xFCA326A0,0xDA60,0x11CF,
0x9B,0x06,0x00,0xA0,0xC9,0x03,0xA3,0xB8};
const GUID g_IDDVideoPortContainer ={0x6C142760,0xA733,0x11CE,
0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60};
const GUID g_IID_IDirectDraw       ={0x9c59509a,0x39bd,0x11d1,
0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5};

enum
{
	RGB565 = 0,
	RGB888,
	ARGB8888,
	RGB655,
	RGB556
};

static DDPIXELFORMAT g_PixelFormats[] =
{
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0},        // 16-bit RGB 5:6:5
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0},        // 32-bit RGB 8:8:8
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0xff000000},        // 32-bit RGB 8:8:8
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xFc00, 0x03e0, 0x001F, 0},        // 16-bit RGB 6:5:5
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07c0, 0x003F, 0}        // 16-bit RGB 5:5:6
};

void VIPDump(const RECT* p)
{
	printf("APP:MSG:VIP: VIPDump RECT(0x%08x):(left: %d, top: %d, right: %d, bottom: %d)\r\n",
		(DWORD)p, p->left, p->top, p->right, p->bottom);
	return;
}

TCHAR* VIPdderr2str(HRESULT hr)
{
	switch(hr)
	{
		CASE_RETURN_STR(DD_OK)
			CASE_RETURN_STR(DDERR_CURRENTLYNOTAVAIL)
			CASE_RETURN_STR(DDERR_GENERIC)
			CASE_RETURN_STR(DDERR_HEIGHTALIGN)
			CASE_RETURN_STR(DDERR_INCOMPATIBLEPRIMARY)
			CASE_RETURN_STR(DDERR_INVALIDCAPS)
			CASE_RETURN_STR(DDERR_INVALIDCLIPLIST)
			CASE_RETURN_STR(DDERR_INVALIDMODE)
			CASE_RETURN_STR(DDERR_INVALIDOBJECT)
			CASE_RETURN_STR(DDERR_INVALIDPARAMS)
			CASE_RETURN_STR(DDERR_INVALIDPIXELFORMAT)
			CASE_RETURN_STR(DDERR_INVALIDRECT)
			CASE_RETURN_STR(DDERR_LOCKEDSURFACES)
			CASE_RETURN_STR(DDERR_NOCLIPLIST)
			CASE_RETURN_STR(DDERR_NOALPHAHW)
			CASE_RETURN_STR(DDERR_NOCOLORCONVHW)
			CASE_RETURN_STR(DDERR_NOCOOPERATIVELEVELSET)
			CASE_RETURN_STR(DDERR_NOCOLORKEYHW)
			CASE_RETURN_STR(DDERR_NOFLIPHW)
			CASE_RETURN_STR(DDERR_NOTFOUND)
			CASE_RETURN_STR(DDERR_NOOVERLAYHW)
			CASE_RETURN_STR(DDERR_OVERLAPPINGRECTS)
			CASE_RETURN_STR(DDERR_NORASTEROPHW)
			CASE_RETURN_STR(DDERR_NOSTRETCHHW)
			CASE_RETURN_STR(DDERR_NOVSYNCHW)
			CASE_RETURN_STR(DDERR_NOZOVERLAYHW)
			CASE_RETURN_STR(DDERR_OUTOFCAPS)
			CASE_RETURN_STR(DDERR_OUTOFMEMORY)
			CASE_RETURN_STR(DDERR_OUTOFVIDEOMEMORY)
			CASE_RETURN_STR(DDERR_PALETTEBUSY)
			CASE_RETURN_STR(DDERR_COLORKEYNOTSET)
			CASE_RETURN_STR(DDERR_SURFACEBUSY)
			CASE_RETURN_STR(DDERR_CANTLOCKSURFACE)
			CASE_RETURN_STR(DDERR_SURFACELOST)
			CASE_RETURN_STR(DDERR_SURFACENOTATTACHED)
			CASE_RETURN_STR(DDERR_TOOBIGHEIGHT)
			CASE_RETURN_STR(DDERR_TOOBIGSIZE)
			CASE_RETURN_STR(DDERR_TOOBIGWIDTH)
			CASE_RETURN_STR(DDERR_UNSUPPORTED)
			CASE_RETURN_STR(DDERR_UNSUPPORTEDFORMAT)
			CASE_RETURN_STR(DDERR_VERTICALBLANKINPROGRESS)
			CASE_RETURN_STR(DDERR_WASSTILLDRAWING)
			CASE_RETURN_STR(DDERR_INVALIDDIRECTDRAWGUID)
			CASE_RETURN_STR(DDERR_DIRECTDRAWALREADYCREATED)
			CASE_RETURN_STR(DDERR_PRIMARYSURFACEALREADYEXISTS)
			CASE_RETURN_STR(DDERR_REGIONTOOSMALL)
			CASE_RETURN_STR(DDERR_CLIPPERISUSINGHWND)
			CASE_RETURN_STR(DDERR_NOCLIPPERATTACHED)
			CASE_RETURN_STR(DDERR_NOPALETTEATTACHED)
			CASE_RETURN_STR(DDERR_NOPALETTEHW)
			CASE_RETURN_STR(DDERR_NOBLTHW)
			CASE_RETURN_STR(DDERR_OVERLAYNOTVISIBLE)
			CASE_RETURN_STR(DDERR_NOOVERLAYDEST)
			CASE_RETURN_STR(DDERR_INVALIDPOSITION)
			CASE_RETURN_STR(DDERR_NOTAOVERLAYSURFACE)
			CASE_RETURN_STR(DDERR_EXCLUSIVEMODEALREADYSET)
			CASE_RETURN_STR(DDERR_NOTFLIPPABLE)
			CASE_RETURN_STR(DDERR_NOTLOCKED)
			CASE_RETURN_STR(DDERR_CANTCREATEDC)
			CASE_RETURN_STR(DDERR_NODC)
			CASE_RETURN_STR(DDERR_WRONGMODE)
			CASE_RETURN_STR(DDERR_IMPLICITLYCREATED)
			CASE_RETURN_STR(DDERR_NOTPALETTIZED)
			CASE_RETURN_STR(DDERR_DCALREADYCREATED)
			CASE_RETURN_STR(DDERR_MOREDATA)
			CASE_RETURN_STR(DDERR_VIDEONOTACTIVE)
			CASE_RETURN_STR(DDERR_DEVICEDOESNTOWNSURFACE)
	default:
		return _T("Unknown token");
	}
}

static TCHAR* VIPcs2str( COLOR_SYSTEM       colorSys)
{
	static TCHAR buf[64];
	switch(colorSys)
	{
		CASE_RETURN_STR(NTSM_MJ)
		CASE_RETURN_STR(NTSC_443)
		CASE_RETURN_STR(PAL_M)
		CASE_RETURN_STR(PAL_60)
		CASE_RETURN_STR(PAL_BGHID)
		CASE_RETURN_STR(PAL_COM_N)
		CASE_RETURN_STR(SECAM)
		CASE_RETURN_STR(SECAM_525)
		default:
		_stprintf(buf, _T("Unknown color system(%d)"), colorSys);
		return buf;
	}
}

static TCHAR* VIPdm2str(DEINTERLACING_MODE dm)
{
	static TCHAR buf[64];
	switch(dm)
	{
		CASE_RETURN_STR(DEINTERLACING_NONE)
			CASE_RETURN_STR(DEINTERLACING_INTRA_FIELD_SPATIAL)
			CASE_RETURN_STR(DEINTERLACING_WEAVE)
			CASE_RETURN_STR(DEINTERLACING_3_TAP_MEDIAN)
			CASE_RETURN_STR(DEINTERLACING_VERTICAL_MEDIAN_RANKING)
	default:
		_stprintf(buf, _T("Unknown deinterlacing mode(%d)"), dm);
		return buf;
	}
}

void VIPDumpRect(wchar_t* prefix, const RECT* pRect)
{
	wprintf(L"%s: left = %d, top = %d, right = %d, bottom = %d\r\n", prefix,
		pRect->left, pRect->top, pRect->right, pRect->bottom);
	return;
}


static bool VIPReadRegistry(DDVIDEOPORTSETTING* pSetting)
{
	ASSERT(pSetting);

	bool bRet = false;
	HKEY hRegKey;
	static const wchar_t* szVPortPath = L"Drivers\\Display\\VPORT";
	DWORD dmMode = 0;
	DWORD ColorSys = 0;

	pSetting->dmMode = DEINTERLACING_VERTICAL_MEDIAN_RANKING;//DEINTERLACING_WEAVE;//
	pSetting->colorSys = NTSM_MJ;

	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, szVPortPath, 0, 0, &hRegKey))
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR, (_T("APP:ERROR:VIP: Registry %s is missing, use default interlacing mode %s \r\n"),
			szVPortPath, VIPdm2str(pSetting->dmMode)));
		bRet = false;
		return bRet;
	}

	{

		DWORD dwValue = 0;
		DWORD dwValLen = sizeof(DWORD);
		DWORD dwValType = 0;
		wchar_t* szRegName = REGNAME_DEINTERLACE_MODE;

		if(ERROR_SUCCESS != RegQueryValueEx(hRegKey, szRegName, NULL, &dwValType, (PBYTE)&dwValue, &dwValLen))
		{
			RETAILMSG(DEBUG_VIDEO_WINDOW_ERR, (_T("APP:ERROR:VIP: Fail to get registry %s value, use default interlacing mode %s \r\n"), szRegName, VIPdm2str(pSetting->dmMode)));
			bRet = false;
		}
		else
		{
			dmMode = dwValue;
		}

	}

	{
		DWORD dwValue = 0;
		DWORD dwValLen = sizeof(DWORD);
		DWORD dwValType = 0;
		wchar_t* szRegName = REGNAME_COLORSYS;

		if(ERROR_SUCCESS != RegQueryValueEx(hRegKey, szRegName, NULL, &dwValType, (PBYTE)&dwValue, &dwValLen))
		{
			RETAILMSG(DEBUG_VIDEO_WINDOW_ERR, (_T("APP:ERROR:VIP: Fail to get registry %s value, use default colorsystem %s \r\n"), szRegName, VIPcs2str(pSetting->colorSys)));
			RegCloseKey(hRegKey);
			bRet = false;
			return bRet;
		}
		else
		{
			ColorSys = dwValue;
		}
	}

	bRet = true;

	pSetting->dmMode    = (DEINTERLACING_MODE)dmMode;
	pSetting->colorSys  = (COLOR_SYSTEM)ColorSys;

	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG, (_T("APP:MSG:VIP: deinterlacing[%s], color system[%s] \r\n"), VIPdm2str(pSetting->dmMode),VIPcs2str(pSetting->colorSys)));

	RegCloseKey(hRegKey);

	return bRet;
}

static void VIPWriteRegistry(DDVIDEOPORTSETTING* pSetting)
{
	ASSERT(pSetting);

	static const wchar_t* szVPortPath = L"Drivers\\Display\\VPORT";
	HKEY hRegKey;

	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, szVPortPath, 0, 0, &hRegKey))
	{
		if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_LOCAL_MACHINE, szVPortPath, 0, NULL, 0, 0, NULL, &hRegKey, NULL))
		{
			wprintf(L"Registry %s is missing and create fail! \r\n", szVPortPath);
			return;
		}
	}

	{
		DWORD dwValue = (DWORD)pSetting->dmMode;
		wchar_t* szRegName = REGNAME_DEINTERLACE_MODE;

		if (ERROR_SUCCESS != RegSetValueEx(hRegKey, szRegName, 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD)))
		{
			wprintf(L"Fail to set registry %s \r\n", szRegName);
		}
		else
		{
			wprintf(L"write registry %s succeed, value = %d \r\n", szRegName, dwValue);
		}
	}

	{
		DWORD dwValue = (DWORD)pSetting->colorSys;
		wchar_t* szRegName = REGNAME_COLORSYS;

		if (ERROR_SUCCESS != RegSetValueEx(hRegKey, szRegName, 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD)))
		{
			wprintf(L"Fail to set registry %s \r\n" , szRegName);
		}
		else
		{
			wprintf(L"write registry %s succeed, value = %d \r\n", szRegName, dwValue);
		}
	}

	RegCloseKey(hRegKey);

	return;
}
float VIPVIPzoomcoef2ratio(int z)
{
	switch(z)
	{
	case ZOOM_25:
		return 0.25f;
	case ZOOM_50:
		return 0.5f;
	case ZOOM_15:
		return 0.15f;
	case ZOOM_FULLSCREEN:
	case ZOOM_100:
	default:
		return 1.0f;
	}
}

TCHAR* VIPVIPzoomcoef2str(DWORD z)
{
	switch(z)
	{
		CASE_RETURN_STR(ZOOM_15)
			CASE_RETURN_STR(ZOOM_25)
			CASE_RETURN_STR(ZOOM_50)
			CASE_RETURN_STR(ZOOM_100)
			CASE_RETURN_STR(ZOOM_FULLSCREEN)
	default:
		return L"Unknown VIPzoom coef";
	}
}

TCHAR* VIPInputChannel(UINT z)
{
	switch(z)
	{
		CASE_RETURN_STR(VIP_CHANNEL_CMMB)
		CASE_RETURN_STR(VIP_CHANNEL_CVBS)
		CASE_RETURN_STR(VIP_CHANNEL_DVD)
		CASE_RETURN_STR(VIP_CHANNEL_DTV)
	default:
		return L"Unknown VIPInputChannel";
	}
}

TCHAR* VIPDecoderChannel(UINT z)
{
	switch(z)
	{
		CASE_RETURN_STR(DECODER_INPUT_CHANNEL_CMMB)
		CASE_RETURN_STR(DECODER_INPUT_CHANNEL_CVBS)
		CASE_RETURN_STR(DECODER_INPUT_CHANNEL_DVD)
		CASE_RETURN_STR(DECODER_INPUT_CHANNEL_NA)
	default:
		return L"Unknown VIPDecoderChannel";
	}
}

TCHAR* VIPVideoMode(UINT z)
{
	switch(z)
	{
		CASE_RETURN_STR(VIP_MODE_CCIR656)
		CASE_RETURN_STR(VIP_MODE_YUV422)
		CASE_RETURN_STR(VIP_MODE_RGB565)
	default:
		return L"Unknown VIPVideoMode";
	}
}

static int VIPzoom(int data, int z)
{
	float r = VIPVIPzoomcoef2ratio(z);
	return (int)(data * r + 0.5f);
}

static void VIPClip(int* data, int offset, int max)
{
	ASSERT(data);
	ASSERT(offset<max);

	if((*data)+offset>max)
	{
		*data = max - offset;
	}

	return;
}

HRESULT WINAPI VIPEnumVideoCallback(LPDDVIDEOPORTCAPS lpDDVideoPortCaps, LPVOID lpContext)
{
	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: Got video port[%d] max width[%d] max height[%d] CAPS[0x%x] FX[0x%x] \r\n"),
		lpDDVideoPortCaps->dwVideoPortID, lpDDVideoPortCaps->dwMaxWidth,
		lpDDVideoPortCaps->dwMaxHeight, lpDDVideoPortCaps->dwCaps,
		lpDDVideoPortCaps->dwFX));

	memcpy(lpContext, lpDDVideoPortCaps, sizeof(DDVIDEOPORTCAPS));

	return DDENUMRET_OK;
}

CVideoWindow::CVideoWindow()
{
	// TODO: add construction code here

	m_pConfig = NULL;

	m_pVPContainer = NULL;// Video port container object

	m_pVideoPort = NULL;  // Video port object

	m_hModule = NULL;
	m_pDD = NULL;        // DirectDraw object
	m_pDDSPrimary = NULL; // Primary Surface.
	m_pDDSOverlay = NULL; // The overlay primary.

	m_hWnd = NULL;
	m_idWindowMode = VIP_WND_PIP_NORMAL;
	m_dwVideoChannel = VIP_CHANNEL_CVBS;
	m_dwDecoderChannel = DECODER_INPUT_CHANNEL_CVBS;

 	m_pfnOnTouchDown = NULL;
 	m_pfnOnTouchMove = NULL;
 	m_pfnOnTouchUp = NULL;
 	m_pfnOnDblClick = NULL;
 	m_lparamClick=NULL;

	m_pfnOnGesture= NULL;
	m_lparamGesture = NULL;

	m_iColorSystem = COLOR_INVALID;

	m_bVideoStart     = FALSE;

	m_bHasSignal=FALSE;
	m_bWindowVisible=FALSE;
	m_dwSetModeTick = 0;
}

CVideoWindow::~CVideoWindow()
{
	StopVideo();

	if(m_hWnd)
	{
		DestroyWindow(m_hWnd);
	}

	if(m_hModule)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}
int CVideoWindow::Zoom(int data)
{
	return VIPzoom(data, m_dwZoomCoef);
}
BOOL CVideoWindow::IsRunning()
{
	return m_bVideoStart;
}
BOOL CVideoWindow::ShowVideoWindow(BOOL bShow)
{
	if(m_hWnd)
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_TEST,(_T("APP:TEST:VIP: CVideoWindow::ShowVideoWindow %d\r\n"),bShow));

		m_bWindowVisible = bShow;

		if(bShow)
		{
			::SetWindowPos(m_hWnd, HWND_TOPMOST , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_SHOWWINDOW) );

			if(m_pDDSOverlay && m_pDDSPrimary)
			{
				RECT  rs,rd;

				rs.left   = 0;
				rs.right  = m_iDstWidth;
				rs.top    = 0;
				rs.bottom = m_iDstHeight;

				VIPDump(&rs);

				rd.left   = m_iDstX;
				rd.right  = rd.left + m_iDstWidth;
				rd.top    = m_iDstY;
				rd.bottom = rd.top + m_iDstHeight;

				VIPDump(&rd);

				Sleep(200);

				 m_pDDSOverlay->UpdateOverlay(&rs, m_pDDSPrimary, &rd, DDOVER_SHOW, NULL);
			}

		}
		else
		{
			if(m_pDDSOverlay)
			{
				m_pDDSOverlay->UpdateOverlay(NULL, m_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
			}

			::SetWindowPos(m_hWnd, HWND_BOTTOM , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | (SWP_HIDEWINDOW) );
		}
	}
	return TRUE;
}
void CVideoWindow::OnSignalDetect()
{
	if(m_dwVideoChannel == VIP_CHANNEL_CMMB  ||
		m_dwVideoChannel == VIP_CHANNEL_CVBS ||
		m_dwVideoChannel == VIP_CHANNEL_DVD ||
		m_dwVideoChannel == VIP_CHANNEL_DTV ||
		m_dwVideoChannel == VIP_CHANNEL_MHL)
	{
		DWORD dwStatus=0;
		if(GetVideoSignalStatus(&dwStatus))
		{
			BOOL bHasSignal=FALSE;
			BYTE 	uCurrentDecoderChannel = DECODER_INPUT_CHANNEL_NA;
			COLOR_SYSTEM col;
			bHasSignal = ((dwStatus&0x0000FFFF) == DDVPSQ_SIGNALOK)?TRUE:FALSE;
			uCurrentDecoderChannel = (DECODERINPUTCHANNEL)((dwStatus&0xFFFF0000) >> 16);

			switch(uCurrentDecoderChannel)
			{
				case DECODER_STANDARD_INPUT_NTSC_MJ:
					col = NTSM_MJ;
					break;
				case DECODER_STANDARD_INPUT_NTSC_443:
					col = NTSC_443;
					break;
				case DECODER_STANDARD_INPUT_PAL_M:
					col = PAL_M;
					break;
				case DECODER_STANDARD_INPUT_PAL_60:
					col = PAL_60;
					break;
				case DECODER_STANDARD_INPUT_PAL_BDGHI:
					col = PAL_BGHID;
					break;
				case DECODER_STANDARD_INPUT_SECAM:
					col = SECAM;
					break;
				case DECODER_STANDARD_INPUT_PAL_CN:
					col = PAL_COM_N;
					break;
				case DECODER_STANDARD_INPUT_SECAM_525:
					col = SECAM_525;
					break;
                default:
					col = COLOR_INVALID;
					break;
			}
	 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("MSG:OnSignalDetect: HasSignal[%d], NewColor[%s], OldColor[%s]\r\n"),bHasSignal,VIPcs2str(col),VIPcs2str(m_iColorSystem)));

			if((m_iColorSystem != col /*|| m_bHasSignal != bHasSignal*/) && bHasSignal)
			{
				RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: OnSignalDetect: Reset color system:%s\r\n"),VIPcs2str(col)));
				SetColorSystem(col);
				m_iColorSystem = col;
				RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: PostMessage WM_CAMERA_TYPE_CHANGE [%s]\r\n"),VIPDecoderChannel(uCurrentDecoderChannel)));
				switch(m_dwVideoChannel)
				{
                    case VIP_CHANNEL_CVBS:
					//PostMessage(g_hUIWnd,WM_CAMERA_TYPE_CHANGE,uCurrentDecoderChannel,NULL);
                        break;
					case VIP_CHANNEL_DTV:
    					m_pConfig->GetCurParam()->iDTVStandard = (BYTE)col;
    					RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: iDTVStandard[%d]\r\n"),m_pConfig->GetCurParam()->iDTVStandard));
                        break;
                    case VIP_CHANNEL_DVD:
    					m_pConfig->GetCurParam()->iDVDStandard = (BYTE)col;
    					RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: iDVDStandard[%d]\r\n"),m_pConfig->GetCurParam()->iDVDStandard));
                        break;
                }
			}

			m_bHasSignal = bHasSignal;

			//show/hide video window
			if(m_bHasSignal)
			{
				if(!m_bWindowVisible)
				{
 					RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: OnSignalDetect: ShowVideoWindow(TRUE)\r\n")));

					ShowVideoWindow(TRUE);
				}
			}
			else
			{
				if(m_bWindowVisible)
				{
 					RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: ShowVideoWindow(FALSE)\r\n")));

					ShowVideoWindow(FALSE);
				}
			}
		}
	}
}
LRESULT CALLBACK CVideoWindow::VideoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CVideoWindow *pVideoWindow=NULL;
	static UINT idLastMode=-1;
	static DWORD dwLastPressTick=0;

	PAINTSTRUCT ps;
	HDC hdc;
 	POINT pt;

	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		pVideoWindow =(CVideoWindow *)(((CREATESTRUCT*)lParam)->lpCreateParams);
		break;
	case WM_SETCURSOR:
		SetCursor(NULL);
		ShowCursor(FALSE);
		break;
	case WM_ERASEBKGND:
		{
			RECT rc;
			static HBRUSH br=CreateSolidBrush(RGB(8,8,8));
			GetClientRect(hWnd,&rc);
			::FillRect(GetDC(hWnd), &rc, br);
			return 0;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		{
			if(pVideoWindow)
			{
				if(wParam == TIMER_VIDEOIN_GETSTATUS)
				{
					pVideoWindow->OnSignalDetect();
				}
			}
		}
		break;
	case WM_DESTROY:
		break;
	case WM_GESTURE:
		if(pVideoWindow->m_pfnOnGesture)
		{
			pVideoWindow->m_pfnOnGesture(wParam,lParam,pVideoWindow->m_lparamGesture);
		}
		else
		{

			GESTUREINFO gi;
			memset(&gi, 0, sizeof(gi));
			gi.cbSize = sizeof(GESTUREINFO);
			if (GetGestureInfo((HGESTUREINFO)lParam, &gi))
			{
				RETAILMSG(DEBUG_VIDEO_WINDOW_MSG, (_T("APP:MSG:VIP: video window WM_GESTURE  dwID: %d, x:%d,y:%d \r\n"),gi.dwID,gi.ptsLocation.x,gi.ptsLocation.y));

				// Handle gesture indicated by wParam or gi.dwID
				switch (gi.dwID)
				{
				case GID_BEGIN:
					break;
				case GID_END:
					break;
				case GID_PAN:
					break;
				case GID_ROTATE:
					break;
				case GID_SCROLL:
					break;
				case GID_HOLD:
					break;
				case GID_DOUBLESELECT:
					break;
				case GID_SELECT:
					{
						/*
						static DWORD lastTick=0;
						if(GetTickCount() - lastTick >500)
						{
							lastTick = GetTickCount();

							if(pVideoWindow->GetWindowMode() == VIP_WND_FULLSCREEN)
							{
								pVideoWindow->SetWindowMode(idLastMode);
							}
							else
							{
								idLastMode=pVideoWindow->GetWindowMode();
								pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
							}
						}
						*/
					}
					break;
				}
			}

		}
		break;
		
	case WM_LBUTTONDOWN:
		{
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("video windows Clicked on WM_LBUTTONDOWN,x:%d,y:%d\r\n"),pt.x,pt.y));
			dwLastPressTick = GetTickCount();

			if(pVideoWindow->m_pfnOnTouchDown)
			{
				pVideoWindow->m_pfnOnTouchDown(&pt,pVideoWindow->m_lparamClick);
			}
			else
			{
			}
		}
		break;
	case WM_LBUTTONUP:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("video windows Clicked on WM_LBUTTONUP,x:%d,y:%d\r\n"),pt.x,pt.y));

		if(pVideoWindow->m_pfnOnTouchUp)
		{
			pVideoWindow->m_pfnOnTouchUp(&pt,pVideoWindow->m_lparamClick);
		}
		else
		{
			if(GetTickCount() - dwLastPressTick >30)
			{
				if(pVideoWindow->GetWindowMode() == VIP_WND_FULLSCREEN)
				{
					pVideoWindow->SetWindowMode(idLastMode);
				}
				else
				{
					idLastMode=pVideoWindow->GetWindowMode();
					pVideoWindow->SetWindowMode(VIP_WND_FULLSCREEN);
				}
			}
		}

		break;
	case WM_MOUSEMOVE:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		if(pVideoWindow->m_pfnOnTouchMove)
		{
			pVideoWindow->m_pfnOnTouchMove(&pt,pVideoWindow->m_lparamClick);
		}
		else
		{
		}
		break;
	case WM_LBUTTONDBLCLK:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		if(pVideoWindow->m_pfnOnDblClick)
		{
			pVideoWindow->m_pfnOnDblClick(&pt,pVideoWindow->m_lparamClick);
		}
		else
		{
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
 void CVideoWindow::SetClickFunc(CLICKPROC pfuncDown,CLICKPROC pfuncUp,CLICKPROC pfuncMove,CLICKPROC pfuncDblClick,LPVOID lparam)
 {
	 //RETAILMSG(1/*DEBUG_VIDEO_WINDOW_MSG*/, (_T("MSG:[%s] FuncDown[0x%X] FuncUp[0x%X] FuncMove[0x%X] FuncDBClick[0x%X] lparam[0x%X]\r\n"),
		// TEXT(__FUNCTION__), pfuncDown, pfuncUp, pfuncMove, pfuncDblClick, lparam));

 	m_pfnOnTouchDown = pfuncDown;
 	m_pfnOnTouchMove = pfuncMove;
 	m_pfnOnTouchUp = pfuncUp;
 	m_pfnOnDblClick = pfuncDblClick;
 	m_lparamClick = lparam;
 }

void CVideoWindow::SetGestureFunc(GESTUREPROC pfuncGesture,LPVOID lpContext)
{
	m_pfnOnGesture= pfuncGesture;
	m_lparamGesture = lpContext;

}

void CVideoWindow::SetColorSystem(COLOR_SYSTEM idColorSys)
{
	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG, (_T("APP:MSG:VIP: SetColorSystem [%s].\r\n"),VIPcs2str(idColorSys)));

	switch(idColorSys)
	{
	case NTSM_MJ:
		m_iSrcHeight  = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_NTSC;
		m_iColorSystem = NTSM_MJ;
		break;
	case NTSC_443:
		m_iSrcHeight  = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_NTSC;
		m_iColorSystem = NTSC_443;
		break;
	case PAL_M:
		m_iSrcHeight = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_PAL_60_M;
		m_iColorSystem = PAL_M;
		break;
	case PAL_60:
		m_iSrcHeight = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_PAL_60_M;
		m_iColorSystem = PAL_60;
		break;
	case PAL_BGHID:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_PAL;
		m_iColorSystem = PAL_BGHID;
		break;
	case PAL_COM_N:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_PAL;
		m_iColorSystem = PAL_COM_N;
		break;
	case SECAM:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_SECAM;
		m_iColorSystem = SECAM;
		break;
	case SECAM_525:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_SECAM;
		m_iColorSystem = SECAM_525;
		break;
	default:
		ASSERT(0);
		break;
	}

	if(m_bVideoStart)
	{
		StopVideo();

		Sleep(500);

		StartVideo();
	}
}
void CVideoWindow::SetVideoChannel(UINT idChannel)
{
	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG, (_T("APP:MSG:VIP: SetVideoChannel [%s].\r\n"),VIPInputChannel(idChannel)));
	switch (idChannel)
	{
	case VIP_CHANNEL_CMMB:
    case VIP_CHANNEL_MHL:
	case VIP_CHANNEL_DTV:
		m_dwVideoChannel = idChannel;
		m_dwVideoMode = VIP_MODE_CCIR656;
		m_iSrcWidth = 720;
		m_iSrcHeight = 480;
		SetDeinterlaceMode(DEINTERLACING_WEAVE);
		break;
	case VIP_CHANNEL_CVBS:	// Default : NTSC
		m_dwVideoChannel = idChannel;
		m_dwVideoMode = VIP_MODE_CCIR656;
		m_iSrcWidth = 720;
		m_iSrcHeight = 480;
		SetDeinterlaceMode(DEINTERLACING_WEAVE);
// 		SetDeinterlaceMode(DEINTERLACING_3_TAP_MEDIAN);
		break;
	case VIP_CHANNEL_DVD:
		m_dwVideoChannel = idChannel;
		m_dwVideoMode = VIP_MODE_CCIR656;
		m_iSrcWidth = 720;//1280;//720;
		m_iSrcHeight = 576;//720;//720;//540;//480;
		//m_dwVBIHeight = 0;
		SetDeinterlaceMode(DEINTERLACING_WEAVE);
		break;
    default:
		break;
	}

	if(m_bVideoStart)
	{
		StopVideo();

		Sleep(500);

		StartVideo();
	}

}
UINT CVideoWindow::GetWindowMode()
{
	return m_idWindowMode;
}
void CVideoWindow::SetWindowMode(UINT idMode)
{
	if(m_idWindowMode != idMode)
	{
		m_dwSetModeTick = GetTickCount();

		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: CVideoWindow::SetWindowMode %d\r\n"),idMode));

		switch (idMode)
		{
		case VIP_WND_FULLSCREEN:
			SetPos(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
			m_idWindowMode = VIP_WND_FULLSCREEN;
			break;
		case VIP_WND_PIP_NORMAL:
			SetPos(144,80,512,288);
			m_idWindowMode = VIP_WND_PIP_NORMAL;
			break;
		case VIP_WND_PIP_SETTING:
			SetPos(300,72,384,224);
			m_idWindowMode = VIP_WND_PIP_SETTING;
			break;
		case VIP_WND_PIP_REVERSE:
			SetPos(10,10,800,480);
			m_idWindowMode = VIP_WND_PIP_REVERSE;
			break;
		case VIP_WND_PIP_DISK:
			SetPos(168,72,464,256);
			m_idWindowMode = VIP_WND_PIP_DISK;
			break;
		case VIP_WND_PIP_REVERSE_FULL:
			SetPos(10,10,780,380);
			m_idWindowMode = VIP_WND_PIP_REVERSE_FULL;
			break;
		}
	}
}
void CVideoWindow::SetPos(int x,int y,int cx,int cy)
{
	BOOL bRunning = m_bVideoStart;

	if(bRunning)
	{
		StopVideo();
	}

	m_iDstX = x;
	m_iDstY = y;
	m_iDstWidth = cx;
	m_iDstHeight = cy;

	::SetWindowPos(m_hWnd, HWND_TOPMOST , m_iDstX, m_iDstY, m_iDstWidth, m_iDstHeight,  0 );

	if(bRunning)
	{
		StartVideo();
	}
}

void CVideoWindow::GetWindowPos(int *px,int *py,int *pcx,int *pcy)
{
	if(px)
	{
		*px=m_iDstX;
	}
	if(py)
	{
		*py=m_iDstY;
	}
	if(pcx)
	{
		*pcx=m_iDstWidth;
	}
	if(pcy)
	{
		*pcy=m_iDstHeight;
	}
}


BOOL CVideoWindow::Initialize(HINSTANCE hIns,CProConfig *pConfig)
{
	DDVIDEOPORTSETTING vpsSetting;

	m_pConfig = pConfig;

	// TODO: Add your specialized code here and/or call the base class
	m_iScreenWidth    = GetSystemMetrics(SM_CXSCREEN) ;
	m_iScreenHeight   = GetSystemMetrics(SM_CYSCREEN) ;

	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: CVideoWindow::Initialize  ScreenWidth[%d]  ScreenHeight[%d]\r\n"), m_iScreenWidth, m_iScreenHeight));

	m_bVideoStart     = FALSE;
	m_dwVideoMode     = VIP_MODE_CCIR656;//
	m_dwZoomCoef      = ZOOM_50;
	m_bOverlay        = TRUE;
	m_bMoving         = FALSE;
	m_dwPosition      = 0;
	m_iPixelFormat = RGB888;//0;
	m_dwVBIHeight  = 0;

	m_iDstX       = 144;
	m_iDstY       = 80;

	m_iDstWidth=512;
	m_iDstHeight=288;

	m_iSrcWidth = 720;
	m_iSrcHeight = 480;

	m_iFramePerSecond = 30;
	m_dmMode = DEINTERLACING_VERTICAL_MEDIAN_RANKING;//DEINTERLACING_WEAVE;//

	vpsSetting.dmMode=DEINTERLACING_VERTICAL_MEDIAN_RANKING;//DEINTERLACING_WEAVE;//
	vpsSetting.colorSys=NTSM_MJ;

	if(!VIPReadRegistry(&vpsSetting))
	{
		VIPWriteRegistry(&vpsSetting);
	}

	m_dmMode = vpsSetting.dmMode;
	m_iColorSystem = vpsSetting.colorSys;


	UpdateColorSystemParam();

	m_pDirectDrawCreate = NULL;
	m_hModule=::LoadLibrary(_T("ddraw.dll"));

	if(m_hModule==NULL)
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Don't support DDRAW!\r\n")));
		return FALSE;
	}

	m_pDirectDrawCreate = (DIRECTDRAWCREATE)::GetProcAddress(m_hModule,L"DirectDrawCreate");

	if(m_pDirectDrawCreate==NULL)
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Could not Get DirectDrawCreate function\r\n")));
		return FALSE;
	}

	//window:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WNDCLASS wc = {0};
	memset (&wc, 0, sizeof(wc));

	// Register  window class
	wc.style            =  0;//CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc      = VideoWndProc;
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = hIns;
	wc.hIcon            = NULL;
	wc.hCursor          = NULL;
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName     = NULL;
	wc.lpszClassName    = _T("CAM_VIDEO_WND");

	RegisterClass(&wc) ;

	// Create Main Window.placeholder. Not visible by default
	m_hWnd  = ::CreateWindowEx(
		WS_EX_NOANIMATION,
		_T("CAM_VIDEO_WND"),
		_T("Camera video window"),
		WS_CHILD ,// WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//,//| WS_VISIBLE
		m_iDstX,//CW_USEDEFAULT,
		m_iDstY,//CW_USEDEFAULT,
		m_iDstWidth,//CW_USEDEFAULT,
		m_iDstHeight,//CW_USEDEFAULT,
		g_hMainWnd,//NULL,
		NULL,
		hIns,
		this);

	ShowWindow(m_hWnd,SW_HIDE);

	return TRUE;
}
void CVideoWindow::UpdateColorSystemParam()
{
	switch(m_iColorSystem)
	{
	case NTSM_MJ:
		m_iSrcHeight  = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_NTSC;
	case NTSC_443:
		m_iSrcHeight  = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_NTSC;
		break;
	case PAL_M:
		m_iSrcHeight = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_PAL_60_M;
		break;
	case PAL_60:
		m_iSrcHeight = 480;
		m_iFramePerSecond = 30;
		m_dwVBIHeight = VBIHEIGHT_PAL_60_M;
		break;
	case PAL_BGHID:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_PAL;
		break;
	case PAL_COM_N:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_PAL;
		break;
	case SECAM:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_SECAM;
		break;
	case SECAM_525:
		m_iSrcHeight = 576;
		m_iFramePerSecond = 25;
		m_dwVBIHeight = VBIHEIGHT_SECAM;
		break;
	default:
		ASSERT(0);
		break;
	}

	return;
}



void CVideoWindow::StopVideo()
{
	if(m_bVideoStart)
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: StopVideo++\r\n")));

		KillTimer(m_hWnd,TIMER_VIDEOIN_GETSTATUS);

		ShowVideoWindow(FALSE);

		m_bVideoStart = FALSE;
		//m_bHasSignal = FALSE;

		ASSERT(m_pVideoPort);

		HRESULT hRet = m_pVideoPort->StopVideo();

		if (hRet != DD_OK)
		{
			ReleaseAllObject();
			RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Stop video FAILED %s!:%d\r\n"),VIPdderr2str(hRet)));
			return;
		}

		ReleaseAllObject();

		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: StopVideo--\r\n")));

	}
}

VOID CVideoWindow::ReleaseAllObject()
{
	if( m_pVideoPort != NULL )
	{
		m_pVideoPort->Release();
		m_pVideoPort = NULL;
	}
	if( m_pVPContainer != NULL )
	{
		m_pVPContainer->Release();
		m_pVPContainer = NULL;
	}

	if (m_pDDSOverlay != NULL)
	{
		// Use UpdateOverlay() with the DDOVER_HIDE flag to remove an overlay
		// from the display.
		m_pDDSOverlay->UpdateOverlay(NULL, m_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
		m_pDDSOverlay->Release();
		m_pDDSOverlay = NULL;
	}


	if (m_pDDSPrimary != NULL)
	{
		m_pDDSPrimary->Release();
		m_pDDSPrimary = NULL;
	}

	if (m_pDD != NULL)
	{
		m_pDD->Release();
		m_pDD = NULL;
	}
}

VOID  CVideoWindow::StartVideo()
{

	DDSURFACEDESC       ddsd;
	HRESULT              hRet;
	int   iCurrentConnectIndex,iInputIndex,iOutputIndex;

	if(m_pDirectDrawCreate == NULL)
		return ;

	if(m_bVideoStart)
	{
		return ;
	}

	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: StartVideo++\r\n")));

// 	//Show window:
// 	Show(TRUE);


	//Create DDraw Object
	hRet = m_pDirectDrawCreate(NULL, &m_pDD, NULL);
	if (hRet != DD_OK)
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: DirectDrawCreate FAILED!\r\n")));
		return ;
	}

	// Fetch VideoPortContainer interface
	hRet = m_pDD->QueryInterface(g_IDDVideoPortContainer, (LPVOID *)&m_pVPContainer);
	if (hRet != DD_OK)
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: QueryInterface for IID_IDDVideoPortContainer FAILED!\r\n")));
		return ;
	}

	// Get normal mode
	hRet = m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: SetCooperativeLevel FAILED!\r\n")));
		return ;
	}

	// Create the primary surface
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOPORT;

	hRet = m_pDD->CreateSurface(&ddsd, &m_pDDSPrimary, NULL);
	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Create primary surface FAILED!\r\n")));
		return ;
	}

	// Enum video ports
	DDVIDEOPORTCAPS     vpCaps;
	DDVIDEOPORTCONNECT  vpConnectInfo[4];
	DWORD               dwInfoArrayLen = 4;

	memset(&vpCaps, 0, sizeof(DDVIDEOPORTCAPS));
	hRet = m_pVPContainer->EnumVideoPorts(0, NULL, &vpCaps, VIPEnumVideoCallback);
	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Enumerate video ports FAILED!\r\n")));
		return ;
	}

	// Get connect information
	hRet = m_pVPContainer->GetVideoPortConnectInfo(vpCaps.dwVideoPortID,
		&dwInfoArrayLen, vpConnectInfo);
	if ((hRet!=DD_OK) && (hRet!=DDERR_MOREDATA))
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Get connect information of video port FAILED.!\r\n")));
		return ;
	}

	for(iCurrentConnectIndex = 0; iCurrentConnectIndex < (int)dwInfoArrayLen; iCurrentConnectIndex++)
	{
		GUID tmpGuid = vpConnectInfo[iCurrentConnectIndex].guidTypeID;

// 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("Get connect Interface info, index=%d,GUID:%x\r\n"),iCurrentConnectIndex,
// 			tmpGuid.Data1));

		if((m_dwVideoMode == VIP_MODE_YUV422 || m_dwVideoMode == VIP_MODE_RGB565) &&
			((0 == memcmp(&tmpGuid, &g_HREFH_VREFH, sizeof(GUID)) ) ||
			(0 == memcmp(&tmpGuid, &g_HREFH_VREFL, sizeof(GUID)) ) ||
			(0 == memcmp(&tmpGuid, &g_HREFL_VREFH, sizeof(GUID)) ) ||
			(0 == memcmp(&tmpGuid, &g_HREFL_VREFL, sizeof(GUID)) ))   )
		{
			break;
		}

		if((m_dwVideoMode == VIP_MODE_CCIR656) &&
			(0 == memcmp(&tmpGuid, &g_DDVPTYPE_CCIR656, sizeof(GUID)) ) )
		{
			break;
		}
	}

	if(iCurrentConnectIndex == 4)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: Get connect Interface mode!\r\n")));
		return ;
	}
	else
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_TEST,(_T("APP:TEST:VIP: Get connect Interface info, index=%d\r\n"),iCurrentConnectIndex));
	}

	m_vpInfo.dwSize       = sizeof(DDVIDEOPORTINFO);
	m_vpInfo.dwVBIHeight  = m_dwVBIHeight;

	// Create video port
	DDVIDEOPORTDESC     vpDesc;
	vpDesc.dwSize = sizeof(DDVIDEOPORTDESC);
	vpDesc.dwVBIWidth    = VBIWIDTH;

	vpDesc.dwFieldWidth  = m_iSrcWidth;
	vpDesc.dwFieldHeight = m_iSrcHeight+m_vpInfo.dwVBIHeight;

	vpDesc.dwMicrosecondsPerField = 1000/(2*m_iFramePerSecond);
	vpDesc.dwMaxPixelsPerSecond = m_iSrcWidth*m_iSrcHeight*m_iFramePerSecond;
	vpDesc.dwVideoPortID = vpCaps.dwVideoPortID;
	memcpy(&vpDesc.VideoPortType, &vpConnectInfo[iCurrentConnectIndex], sizeof(DDVIDEOPORTCONNECT));

/*
	RETAILMSG(1,(_T("APP:MSG:VIP:vpDesc.dwSize           [%d]\r\n"),vpDesc.dwSize ));
	RETAILMSG(1,(_T("APP:MSG:VIP:vpDesc.dwFieldWidth     [%d]\r\n"),vpDesc.dwFieldWidth  ));
	RETAILMSG(1,(_T("APP:MSG:VIP:vpDesc.dwVBIWidth       [%d]\r\n"),vpDesc.dwVBIWidth ));
	RETAILMSG(1,(_T("APP:MSG:VIP:vpDesc.dwFieldHeight    [%d]\r\n"),vpDesc.dwFieldHeight ));
	RETAILMSG(1,(_T("APP:MSG:VIP:vpDesc.dwMicrosecondsPerField [%d]\r\n"),vpDesc.dwMicrosecondsPerField ));
	RETAILMSG(1,(_T("APP:MSG:VIP:vpDesc.dwVideoPortID       [%d]\r\n"),vpDesc.dwVideoPortID ));
*/


	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: Create video port dwFieldWidth: %d, dwFieldHeight: %d\r\n"), vpDesc.dwFieldWidth,vpDesc.dwFieldHeight));

	hRet = m_pVPContainer->CreateVideoPort(DDVPCREATE_VIDEOONLY, &vpDesc, &m_pVideoPort, NULL);

	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Create video port FAILED %s!\r\n"), VIPdderr2str(hRet)));
		return ;
	}
	//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:Create video port Successfully %s!\r\n"), VIPdderr2str(hRet)));

	// Get input formats
	DWORD dwNumFormats = 4;

	hRet = m_pVideoPort->GetInputFormats(&dwNumFormats, m_vpInputFormats, DDVPFORMAT_VIDEO);
	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Get input formats FAILED!\r\n")));
		return ;
	}
	//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:Get input formats Successfully!\r\n")));

	for(iInputIndex = 0;iInputIndex < 4;iInputIndex++)
	{
		if((m_dwVideoMode == VIP_MODE_YUV422 ) &&
			(m_vpInputFormats[iInputIndex].dwFourCC  == MAKEFOURCC('Y', 'U', 'Y', 'V') ))
		{
			break;
		}

		if((m_dwVideoMode == VIP_MODE_CCIR656) &&
			(m_vpInputFormats[iInputIndex].dwFourCC == MAKEFOURCC('U', 'Y', 'V', 'Y') ))
		{
			break;
		}

		if((m_dwVideoMode == VIP_MODE_RGB565) &&
			(DDPF_RGB & m_vpInputFormats[iInputIndex].dwFlags) &&
			(m_vpInputFormats[iInputIndex].dwRBitMask == 0xF800) &&
			(m_vpInputFormats[iInputIndex].dwGBitMask == 0x7E0) &&
			(m_vpInputFormats[iInputIndex].dwBBitMask == 0x1F)     )
		{
			break;
		}
	}

	if(iInputIndex == 4)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Could not Get appropriate input pixelformat!\r\n")));
		return ;
	}

	// Get input formats
	dwNumFormats = 6;
	DDPIXELFORMAT     vpOutPixelFormats[6];

	hRet = m_pVideoPort->GetOutputFormats(&m_vpInputFormats[iInputIndex],
		&dwNumFormats,
		vpOutPixelFormats,
		DDVPFORMAT_VIDEO);
	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Get input formats FAILED!\r\n")));
		return ;
	}
	//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:Get input formats Successfully!\r\n")));

	//for future to do, add RGB556,RGB655,RGB8888
	for(iOutputIndex = 0;iOutputIndex < 6;iOutputIndex++)
	{
		if((DDPF_RGB & vpOutPixelFormats[iOutputIndex].dwFlags) &&
			(vpOutPixelFormats[iOutputIndex].dwRBitMask == 0xF800) &&
			(vpOutPixelFormats[iOutputIndex].dwGBitMask == 0x7E0) &&
			(vpOutPixelFormats[iOutputIndex].dwBBitMask == 0x1F)     )
		{
			break;
		}
	}

	if(iOutputIndex == 6)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Could not Get appropriate Output pixelformat!\r\n")));
		return ;
	}

	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG, (_T("APP:MSG:VIP: VIPzoom coef [%s] \r\n"), VIPVIPzoomcoef2str(m_dwZoomCoef)));

	if(m_dwZoomCoef	== ZOOM_FULLSCREEN)
	{
		m_iDstX = 0;
		m_iDstY = 0;
		m_iDstWidth = m_iScreenWidth;
		m_iDstHeight = m_iScreenHeight;
	}
	else
	{

		VIPClip(&m_iDstWidth, m_iDstX, m_iScreenWidth);
		VIPClip(&m_iDstHeight, m_iDstY, m_iScreenHeight);
	}

	if(m_bOverlay)
	{
		// See if we can support overlays.
		memset(&m_ddcaps, 0, sizeof(m_ddcaps));
		m_ddcaps.dwSize = sizeof(m_ddcaps);
		hRet = m_pDD->GetCaps(&m_ddcaps,NULL);
		if (hRet != DD_OK)
		{
			ReleaseAllObject();
			RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: GetCaps FAILED!\r\n")));
			return ;
		}

		if (0 == (m_ddcaps.ddsCaps.dwCaps & DDSCAPS_OVERLAY))
		{
			ReleaseAllObject();
			RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Overlays are not supported in hardware!!\r\n")));
			return ;
		}

		if (m_ddcaps.dwAlignSizeDest)
		{
			RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: m_ddcaps.dwAlignSizeDest is not 0!\r\n")));
			m_iDstWidth = ALIGN(m_iDstWidth, m_ddcaps.dwAlignSizeDest);
		}

		// Create the overlay surface
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);

		ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP |
			DDSCAPS_VIDEOMEMORY | DDSCAPS_VIDEOPORT;
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT |
			DDSD_PIXELFORMAT;

		ddsd.dwWidth           = m_iDstWidth;
		ddsd.dwHeight          = m_iDstHeight;
		ddsd.dwBackBufferCount = 2;
		ddsd.ddpfPixelFormat   = g_PixelFormats[m_iPixelFormat];

// 		Dump(m_iDstWidth, %d);
// 		Dump(m_iDstHeight, %d);

		hRet = m_pDD->CreateSurface(&ddsd, &m_pDDSOverlay, NULL);

		if (hRet != DD_OK)
		{
			ReleaseAllObject();
			RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: CVideoWindow Create Overlay surface FAILED!\r\n")));
			return ;
		}
		//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:CVideoWindow Create Overlay surface Successfully!\r\n")));
		//initialize the surface
		DDBLTFX sBltFX = {0};
		sBltFX.dwSize = sizeof(sBltFX);
		sBltFX.dwFillColor = 0x0;

		RECT rc = {0, 0, m_iDstWidth, m_iDstHeight};
		hRet = m_pDDSOverlay->Blt(&rc, NULL, NULL, DDBLT_COLORFILL, &sBltFX);

		if (hRet !=DD_OK)
		{
			RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Initialize Overlay surface to 0x0 FAILED!\r\n")));
			return;
		}
		//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:Initialize Overlay surface to 0x0 Successfully!\r\n")));

		m_pDDSOverlay->UpdateOverlay(NULL, m_pDDSPrimary, NULL, DDOVER_HIDE, NULL);

	}

	// Set target surface
	if(m_bOverlay)
	{
		hRet = m_pVideoPort->SetTargetSurface((LPDIRECTDRAWSURFACE)m_pDDSOverlay, DDVPTARGET_VIDEO);
	}
	else
	{
		hRet = m_pVideoPort->SetTargetSurface((LPDIRECTDRAWSURFACE)m_pDDSPrimary, DDVPTARGET_VIDEO);
	}

	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Set target surface FAILED!\r\n")));
		return ;
	}

	// Get color control
	m_vpColorControl.dwSize = sizeof(DDCOLORCONTROL);
	hRet = m_pVideoPort->GetColorControls(&m_vpColorControl);
	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Get color controls FAILED!\r\n")));
		return ;
	}
	else
	{
		//RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:Get color controls Successfully!\r\n")));
		//dump:
// 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("+++Get color controls lBrightness:%d!\r\n"),m_vpColorControl.lBrightness));
// 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("+++Get color controls lContrast:%d!\r\n"),m_vpColorControl.lContrast));
// 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("+++Get color controls lHue:%d!\r\n"),m_vpColorControl.lHue));
// 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("+++Get color controls lSaturation:%d!\r\n"),m_vpColorControl.lSaturation));
//
// 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("+++Get color controls dwFlags:%x!\r\n"),m_vpColorControl.dwFlags));
// 		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("+++Get color controls lColorEnable:%d!\r\n"),m_vpColorControl.lColorEnable));

		//set default color control:
		if(m_pConfig)
		{
			m_vpColorControl.lColorEnable=1;
			m_vpColorControl.lContrast=(m_pConfig->GetCurParam()->iVideoContrast)*20000/64;
			m_vpColorControl.lBrightness=(m_pConfig->GetCurParam()->iVideoBrightness)*10000/64;
			m_vpColorControl.lHue=(m_pConfig->GetCurParam()->iVideoHUE-32)*180/32;
			m_vpColorControl.lSaturation=(m_pConfig->GetCurParam()->iVideoSaturation)*20000/64;

			m_pVideoPort->SetColorControls(&m_vpColorControl);
		}

	}

	// Start video
	if(m_bOverlay)
	{
		m_vpInfo.dwOriginX = 0;  // do not change
		m_vpInfo.dwOriginY = 0;	 // do not change
	}
	else
	{
		m_vpInfo.dwOriginX = m_iDstX;
		m_vpInfo.dwOriginY = m_iDstY;
	}

// 	Dump(m_vpInfo.dwOriginX, %d);
// 	Dump(m_vpInfo.dwOriginY, %d);

	m_vpInfo.dwVPFlags    = DDVP_CROP | DDVP_PRESCALE ;

    switch(m_dwVideoChannel)
    {
        case VIP_CHANNEL_DTV:
        case VIP_CHANNEL_CMMB:
        {
            m_dwDecoderChannel = DECODER_INPUT_CHANNEL_CMMB;
    		if (m_dwVideoMode == VIP_MODE_CCIR656)
    		{
    // 			m_vpInfo.dwVBIHeight  = 4;

    			m_vpInfo.rCrop.left   = 0;
    			m_vpInfo.rCrop.right  = m_vpInfo.rCrop.left + m_iSrcWidth;	//Do not change
    			m_vpInfo.rCrop.top    = -5;		//m_vpInfo.dwVBIHeight;
    			m_vpInfo.rCrop.bottom = m_vpInfo.rCrop.top + m_iSrcHeight / 2 + m_vpInfo.dwVBIHeight;
    		}
    		else
    		{
     			m_vpInfo.dwVBIHeight  = 4;

    			m_vpInfo.rCrop.left   = 0;
    			m_vpInfo.rCrop.right  = m_iSrcWidth;
    			m_vpInfo.rCrop.top    = 0;//m_vpInfo.dwVBIHeight;
    			m_vpInfo.rCrop.bottom = m_iSrcHeight/2 - m_vpInfo.dwVBIHeight;
    		}
        }
        break;
        case VIP_CHANNEL_CVBS:
        {
            m_dwDecoderChannel = DECODER_INPUT_CHANNEL_CVBS;
    		if (m_dwVideoMode == VIP_MODE_CCIR656)
    		{
    // 			m_vpInfo.dwVBIHeight  = 4;

    			m_vpInfo.rCrop.left   = 0;
    			m_vpInfo.rCrop.right  = m_vpInfo.rCrop.left + m_iSrcWidth;	//Do not change
    			m_vpInfo.rCrop.top    = -5;		//m_vpInfo.dwVBIHeight;
    			m_vpInfo.rCrop.bottom = m_vpInfo.rCrop.top + m_iSrcHeight / 2 + m_vpInfo.dwVBIHeight;
    		}
    		else
    		{
     			m_vpInfo.dwVBIHeight  = 4;

    			m_vpInfo.rCrop.left   = 0;
    			m_vpInfo.rCrop.right  = m_iSrcWidth;
    			m_vpInfo.rCrop.top    = 0;//m_vpInfo.dwVBIHeight;
    			m_vpInfo.rCrop.bottom = m_iSrcHeight/2 - m_vpInfo.dwVBIHeight;
    		}

        }
        break;
        case VIP_CHANNEL_DVD:
        {
            m_dwDecoderChannel = DECODER_INPUT_CHANNEL_DVD;
    		if (m_dwVideoMode == VIP_MODE_CCIR656)
    		{
    // 			m_vpInfo.dwVBIHeight  = 4;

    			m_vpInfo.rCrop.left   = 0;
    			m_vpInfo.rCrop.right  = m_vpInfo.rCrop.left + m_iSrcWidth;	//Do not change
    			m_vpInfo.rCrop.top    = -5;		//m_vpInfo.dwVBIHeight;
    			m_vpInfo.rCrop.bottom = m_vpInfo.rCrop.top + m_iSrcHeight / 2 + m_vpInfo.dwVBIHeight;
    		}
    		else
    		{
     			m_vpInfo.dwVBIHeight  = 4;

    			m_vpInfo.rCrop.left   = 0;
    			m_vpInfo.rCrop.right  = m_iSrcWidth;
    			m_vpInfo.rCrop.top    = 0;//m_vpInfo.dwVBIHeight;
    			m_vpInfo.rCrop.bottom = m_iSrcHeight/2 - m_vpInfo.dwVBIHeight;
    		}

        }
        break;
        default:
		    RETAILMSG(1,(_T("APP:ERROR:VIP: Format invalid.\r\n")));
		    return ;

    }

	//select video channel:
	m_vpInfo.dwVPFlags += (m_dwDecoderChannel << 24);

	//vpinfo:
	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG, (_T("APP:MSG:VIP: StartVideo[%s] VideoMode[%s] \r\n"),VIPInputChannel(m_dwVideoChannel), VIPVideoMode(m_dwVideoMode)));
	RETAILMSG(DEBUG_VIDEO_WINDOW_TEST, (_T("APP:TEST:VIP: StartVideo m_vpInfo.dwVPFlags 0x%x\r\n"),m_vpInfo.dwVPFlags));

	m_vpInfo.dwPrescaleWidth  = m_iDstWidth;
	m_vpInfo.dwPrescaleHeight = m_iDstHeight;

	m_vpInfo.lpddpfInputFormat = &m_vpInputFormats[iInputIndex];
	m_vpInfo.lpddpfVBIInputFormat = NULL;
	m_vpInfo.lpddpfVBIOutputFormat = NULL;

	//VIPDumpRect(L"crop rect in application", &m_vpInfo.rCrop);

/*
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.dwOriginX        [%d]\r\n"),m_vpInfo.dwOriginX));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.dwOriginY        [%d]\r\n"),m_vpInfo.dwOriginY ));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.dwVPFlags        [%d]\r\n"),m_vpInfo.dwVPFlags));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.rCrop.left       [%d]\r\n"),m_vpInfo.rCrop.left));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.rCrop.top        [%d]\r\n"),m_vpInfo.rCrop.top));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.rCrop.right      [%d]\r\n"),m_vpInfo.rCrop.right));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.rCrop.bottom     [%d]\r\n"),m_vpInfo.rCrop.bottom));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.dwPrescaleWidth  [%d]\r\n"),m_vpInfo.dwPrescaleWidth));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.dwPrescaleHeight [%d]\r\n"),m_vpInfo.dwPrescaleHeight));
	RETAILMSG(1,(_T("APP:MSG:VIP:m_vpInfo.dwVBIHeight      [%d]\r\n"),m_vpInfo.dwVBIHeight ));
*/

	hRet = m_pVideoPort->StartVideo(&m_vpInfo);

	if (hRet != DD_OK)
	{
		ReleaseAllObject();
		RETAILMSG(DEBUG_VIDEO_WINDOW_ERR,(_T("APP:ERROR:VIP: Start video FAILED %s!\r\n"), VIPdderr2str(hRet)));
		return ;
	}

	if(m_bOverlay)
	{
// 		RECT  rs,rd;
//
// 		rs.left   = 0;
// 		rs.right  = m_iDstWidth;
// 		rs.top    = 0;
// 		rs.bottom = m_iDstHeight;
//
// 		VIPDump(&rs);
//
// 		rd.left   = m_iDstX;
// 		rd.right  = rd.left + m_iDstWidth;
// 		rd.top    = m_iDstY;
// 		rd.bottom = rd.top + m_iDstHeight;
//
// 		VIPDump(&rd);
//
// 		Sleep(100);
// 		hRet = m_pDDSOverlay->UpdateOverlay(&rs, m_pDDSPrimary, &rd, DDOVER_SHOW, NULL);
//
// 		if (hRet != DD_OK)
// 		{
// 			ReleaseAllObject();
// 			RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("Update Overlay FAILED!\r\n")));
// 			return ;
// 		}
	}

	RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: Start video SUCCEED!!!!! \r\n")));

	//if(m_dwVideoChannel == VIP_CHANNEL_MHL)
	//{
		ShowVideoWindow(TRUE);
	//}
	//else
	//{
		//OnSignalDetect();
	//}

	//
	SetTimer(m_hWnd,TIMER_VIDEOIN_GETSTATUS,2000,NULL);

	m_bVideoStart = TRUE;

	m_dwStartTick = GetTickCount();
}
BOOL CVideoWindow::SetColorControl(DDCOLORCONTROL *pColorCtl)
{
	HRESULT hRet;
	if(!m_pVideoPort || !pColorCtl)
		return FALSE;

	hRet = m_pVideoPort->SetColorControls(pColorCtl);

	return (hRet == DD_OK);

}
BOOL CVideoWindow::GetVideoSignalStatus(DWORD *pdwStatus)
{
	HRESULT hRet;
	if(!m_pVideoPort || !pdwStatus)
		return FALSE;

	hRet = m_pVideoPort->GetVideoSignalStatus(pdwStatus);

	return (hRet == DD_OK);
}
void CVideoWindow::SetDeinterlaceMode(DEINTERLACING_MODE dmMode)
{
	DDVIDEOPORTSETTING vpsSetting;

	if(m_dmMode != dmMode)
	{
		RETAILMSG(DEBUG_VIDEO_WINDOW_MSG,(_T("APP:MSG:VIP: CVideoWindow::SetDeinterlaceMode: [%s]\r\n"),VIPdm2str(dmMode)));

		m_dmMode = dmMode;
		vpsSetting.dmMode = m_dmMode;
		vpsSetting.colorSys = m_iColorSystem;
		VIPWriteRegistry(&vpsSetting);
	}

	return;
}

