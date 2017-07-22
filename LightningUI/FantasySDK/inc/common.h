#pragma once

// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
 #include <pwinuser.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//OpenGL 
#include <GLES/gl.h>
#include <EGL/eglplatform.h>
#include <EGL/egl.h>


// TODO: reference additional headers your program requires here
#include "resource.h"

#define MAX_STRING			256

extern const int			g_iClientWidth;
extern const int			g_iClientHeight;
extern const int			g_iScreenWidth;
extern const int			g_iScreenHeight;

//UI Operation Code:
#define UIOPRT_SETSTRING									1
#define UIOPRT_GETSTRING									2
#define UIOPRT_SETVALUE									3
#define UIOPRT_GETVALUE									4
#define UIOPRT_ADDITEM									5
#define UIOPRT_DELETEITEM								6
#define UIOPRT_UPDATEITEM								7
#define UIOPRT_CLEARALL									8
#define UIOPRT_SWITCHPAGE								9
#define UIOPRT_SETIMAGEDATA							10
#define UIOPRT_RELOADIMAGEDATA					11
#define UIOPRT_SETSTATUS									12
#define UIOPRT_GETSTATUS									13
#define UIOPRT_GETCURSEL_INDEX						14
#define UIOPRT_GETCURSEL_ID							15
#define UIOPRT_SHOWHIDE									16
#define UIOPRT_ENABLE										17
#define UIOPRT_QUITPANEL									18
#define UIOPRT_MOVETOINDEX							19
#define UIOPRT_GET_COUNTS								20
#define UIOPRT_SHOW_NOTIFICATION				21

typedef struct _TMODULEENTRY
{
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPTSTR    lpCmdLine;
	int       nCmdShow;
}MODULEENTRY,*LPMODULEENTRY;

typedef struct _TUIOPERATIONCODE
{
	DWORD dwCode;
	DWORD dwControlID;
	LPVOID lpParam;
	DWORD dwParam;

}UIOPERATIONCODE,*LPUIOPERATIONCODE;

typedef struct _TMODULEOPERATIONCODE
{
	DWORD dwCode;
	DWORD dwParam;
	DWORD dwParam2;
	LPVOID lpParam;

}MODULEOPERATIONCODE,*LPMODULEOPERATIONCODE;

enum BUTTON_STATUS
{
	BTS_NORMAL,
	BTS_DOWN,
	BTS_FOCUS,
	BTS_DISABLED
};

enum WIFI_STATUS
{
	WIFI_OFF,
	WIFI_CONNECTING,
	WIFI_CONNECTED,
	WIFI_DISCONNECTING,
	WIFI_DISCONNECTED
};
enum DIALUP_STATUS
{
	DIALUP_OFF,
	DIALUP_CONNECTING,
	DIALUP_CONNECTED,
	DIALUP_DISCONNECTING,
	DIALUP_DISCONNECTED
};
enum RDS_STATUS
{
	RDS_OFF,
	RDS_ON,
	RDS_SEARCHING
};

enum _ANDROID_LINK_TYPE
{
    ANDROID_LINK_PHONELINK,
    ANDROID_LINK_EASYCONNECTED
};