// LightningUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <dbt.h>
#include <pkfuncs.h>
#include "LightningUI.h"
#include "./GUI/main/GUI_Fantasy.h"

#define MAX_LOADSTRING 100
#define UI_WND_CLASS_NAME  _T("MAIN_UI_WINDOW")
#define UI_WND_TITLE  _T("Main UI Window")

//events:
#define EVT_UI_LAUNCHED									_T("EVT_UI_LAUNCHED")
// #define EVT_TOUCH_NOTIFICATION						_T("EVT_TOUCH_NOTIFICATION")
#define EVT_MHL_NOTIFICATION							_T("EVT_MHL_NOTIFICATION")
#define EVT_SPEECH_NOTIFICATION					_T("EVT_SPEECH_NOTIFICATION")
#define EVT_REARVIEW_NOTIFY							_T("EVT_REARVIEW_NOTIFY")
#define EVT_REARVIEW_FEEDBACK_NOTIFY			_T("EVT_REARVIEW_FEEDBACK_NOTIFY")
#define EVT_TOUCH_NOTIFICATION						TEXT("TOUCH_EVENT_NAME")
#define EVT_GESTURE_NOTIFICATION							TEXT("GESTURE_EVENT_NAME")

#define EVT_AIRPLAY_START_EVENT					_T("AIRPLAY_START_EVENT")
#define EVT_AIRPLAY_STOP_EVENT					_T("AIRPLAY_STOP_EVENT")
#define EVT_AIRPLAY_START_FAILE_EVENT		_T("AIRPLAY_START_FAILE_EVENT")

#define TOUCH_EVENT_NAME								TEXT("TOUCH_EVENT_NAME")
#define GESTURE_EVENT_NAME							TEXT("GESTURE_EVENT_NAME")

#define END_COM_PROC_THREAD						TEXT("END_COM_PROC_THREAD")
#define EVT_REARVIEW_COLORSIANAL_CHANGE         TEXT("EVT_REARVIEW_COLORSIANAL_CHANGE")
//	#define PAC_MSG_QUEUE_NAME  					TEXT("PAC_MSG_QUEUE_NAME")

#define USB_HUB_ATTACHED_EVENT	_T("USB_HUB_ATTACHED_EVENT")

#define REARVIEW_CMD_START  0x1111
#define REARVIEW_CMD_DONE   0x2222
#define REARVIEW_CMD_FAIL   0x3333
#define REARVIEW_CMD_STOP   0x4444

HANDLE	   g_hUILaunchEvent=INVALID_HANDLE_VALUE;
HANDLE	   g_hCloseComEvent=INVALID_HANDLE_VALUE;

HANDLE	   g_hEvtSpeech=INVALID_HANDLE_VALUE;
HANDLE	   g_hEvtTouch=INVALID_HANDLE_VALUE;
HANDLE	   g_hEvtGesture=INVALID_HANDLE_VALUE;

HANDLE	   g_hEvtMHL=INVALID_HANDLE_VALUE;

HANDLE	   g_hRearViewNotifyEvent=INVALID_HANDLE_VALUE;
HANDLE	   g_hRearViewNotifyFeedbackEvent=INVALID_HANDLE_VALUE;
HANDLE     g_hRearViewColorSignalChange=INVALID_HANDLE_VALUE;
HANDLE     g_hHubAttachedEvent=NULL;

// Global Variables:
MODULEENTRY	g_entry;
CGUI_Fantasy		*g_pGUI=NULL;

HINSTANCE			g_hInst=NULL;	// current instance
HWND				g_hMainWnd=NULL;
HWND				g_hUIWnd=NULL;
HANDLE				g_hMapFile = NULL;
MULTIPOINT		g_MultiPoint;
MULTIPOINT*		g_pMultiPointShareView = NULL;
PACGestureData_t g_GestureData;


// const		int			g_iScreenWidth=800;//GetSystemMetrics(SM_CXSCREEN);
// const		int			g_iScreenHeight=480;//GetSystemMetrics(SM_CYSCREEN);
// const		int			g_iClientWidth=800;//GetSystemMetrics(SM_CXSCREEN);
// const		int			g_iClientHeight=480;//GetSystemMetrics(SM_CYSCREEN);

const		int			g_iScreenWidth=GetSystemMetrics(SM_CXSCREEN);
const		int			g_iScreenHeight=GetSystemMetrics(SM_CYSCREEN);
const		int			g_iClientWidth=GetSystemMetrics(SM_CXSCREEN);
const		int			g_iClientHeight=GetSystemMetrics(SM_CYSCREEN);

WPARAM g_wParamVolumeInfo=0;
WPARAM g_lParamVolumeInfo=0;

WPARAM g_wParamSourceInfo=0;
WPARAM g_lParamSourceInfo=0;

WPARAM g_wParamMediaPlaybackInfo=0;
WPARAM g_lParamMediaPlaybackInfo=0;

WPARAM g_wParamRadioInfo=0;
WPARAM g_lParamRadioInfo=0;

//
DWORD					g_dwNaviProcessID=0;
BOOL						g_bNaviSpeechEnabled=0;

// Forward declarations of functions included in this code module:
ATOM			RegisterMainWndClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	UIWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	MainWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM			RegisterUIWndClass(HINSTANCE, LPTSTR);

static DWORD	EventSignalThreadProc(LPVOID lparam);
static DWORD	GestureThreadProc(LPVOID lparam);


int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR    lpCmdLine,
				   int       nCmdShow)
{
	MSG msg;
	int iPriority = CE_THREAD_PRIO_256_ABOVE_NORMAL;
// #if (CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY == CVTE_ENABLE)
// 	iPriority = CE_THREAD_PRIO_256_ABOVE_NORMAL;
// #endif
	CeSetThreadPriority(GetCurrentThread(), iPriority);
	RETAILMSG(1, (TEXT("#########begin  to launch Fantasy UI : at %d ms, iPriority=%d, threadID=0x%x\r\n"),GetTickCount(),iPriority,GetCurrentThreadId()));
	g_hCloseComEvent = CreateEvent(NULL, FALSE, FALSE, END_COM_PROC_THREAD);
	if(g_hCloseComEvent)
	{
		RETAILMSG(1,(_T("############### SetEvent [END_COM_PROC_THREAD]#################3\r\n")));
		SetEvent(g_hCloseComEvent);
	}

	// 	//hide taskbar:
	// 	::ShowWindow(::FindWindow(_T("HHTaskBar"),NULL),SW_HIDE);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

// 	HACCEL hAccelTable;
// 	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LIGHTNINGUI));

	// 	RequestAccelebootNotification(AccelebootNofity);

	//GUI
	g_pGUI = new CGUI_Fantasy;

	if(!g_pGUI)
		return 0;

	MODULEENTRY entry={hInstance,hPrevInstance,lpCmdLine,nCmdShow};
	g_entry=entry;
	if(!g_pGUI->Initialize(g_hUIWnd,entry,iPriority))
	{
		return 0;
	}

	if(g_pGUI)
	{
		g_pGUI->OnSystemReady();
	}


	//take snapshot
	// 	TakeSnapshot(1);

	g_hEvtSpeech = CreateEvent(NULL,FALSE,FALSE,EVT_SPEECH_NOTIFICATION);
	g_hEvtTouch = CreateEvent(NULL,FALSE,FALSE,EVT_TOUCH_NOTIFICATION);
	g_hEvtGesture = CreateEvent(NULL,FALSE,FALSE,EVT_GESTURE_NOTIFICATION);
	g_hEvtMHL = CreateEvent(NULL,FALSE,FALSE,EVT_MHL_NOTIFICATION);
	g_hRearViewNotifyEvent   = CreateEvent(NULL, FALSE, FALSE, EVT_REARVIEW_NOTIFY);
	g_hRearViewNotifyFeedbackEvent   = CreateEvent(NULL, FALSE, FALSE, EVT_REARVIEW_FEEDBACK_NOTIFY);
    g_hRearViewColorSignalChange = CreateEvent(NULL, FALSE, FALSE, EVT_REARVIEW_COLORSIANAL_CHANGE);
	g_hHubAttachedEvent = CreateEvent(NULL, FALSE, FALSE, USB_HUB_ATTACHED_EVENT);
	/*
	//msg loop:
	BOOL fQuit=FALSE;

	// Main message loop:
	while (!fQuit)
	{

		dwWait = MsgWaitForMultipleObjectsEx( dwEvtCount, ahEvent, INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE );

		// if window message
		if( dwWait == WAIT_OBJECT_0 + dwEvtCount )
		{
			// Dispatch all of the messages.
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					// A WM_QUIT message, exit the loop
					fQuit = TRUE;
				}
				else
				{
					//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
					{
						// Translate and dispatch the message.
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			} // Our queue is empty.
		}
	}
	*/

	//notify UI launched:
	g_hUILaunchEvent  = CreateEvent(NULL, FALSE, FALSE, EVT_UI_LAUNCHED);
	if(g_hUILaunchEvent)
	{
		SetEvent(g_hUILaunchEvent);
	}

	CreateThread(NULL,NULL,EventSignalThreadProc,NULL,NULL,NULL);
	//CreateThread(NULL,NULL,GestureThreadProc,NULL,NULL,NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
	 	//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	 	{
	 		TranslateMessage(&msg);
	 		DispatchMessage(&msg);
	 	}
	 }


	if(g_pGUI)
	{
		//g_pGUI->OnSystemPrePowerOFF();

		delete g_pGUI;
		g_pGUI=NULL;
	}

    if(g_pMultiPointShareView)
    {
        UnmapViewOfFile(g_pMultiPointShareView);
        g_pMultiPointShareView = NULL;
    }

    if(g_hMapFile)
    {
        CloseHandle(g_hMapFile);
        g_hMapFile = NULL;
    }

	if(g_hEvtSpeech)
	{
		CloseHandle(g_hEvtSpeech);
		g_hEvtSpeech = NULL;
	}

	if(g_hEvtTouch)
	{
		CloseHandle(g_hEvtTouch);
		g_hEvtTouch = NULL;
	}

	if(g_hEvtGesture)
	{
		CloseHandle(g_hEvtGesture);
		g_hEvtGesture = NULL;
	}

	if(g_hEvtMHL)
	{
		CloseHandle(g_hEvtMHL);
		g_hEvtMHL = NULL;
	}

	if(g_hRearViewNotifyEvent)
	{
		CloseHandle(g_hRearViewNotifyEvent);
		g_hRearViewNotifyEvent = NULL;
	}

    if(g_hRearViewColorSignalChange)
    {
        CloseHandle(g_hRearViewColorSignalChange);
        g_hRearViewColorSignalChange = NULL;
    }

	if(g_hHubAttachedEvent)
	{
		CloseHandle(g_hHubAttachedEvent);
		g_hHubAttachedEvent = NULL;
	}
	// 	//hide taskbar:
	// 	::ShowWindow(::FindWindow(_T("HHTaskBar"),NULL),SW_SHOW);

	return (int) msg.wParam;
}

//	static DWORD GestureThreadProc(LPVOID lparam)
//	{
//		HANDLE hMsgQueue;
//		DWORD   dwBytesRead, dwFlags;
//		MSGQUEUEOPTIONS MsgQueueOpt;
//		MsgQueueOpt.dwSize = sizeof(MsgQueueOpt);
//		MsgQueueOpt.dwFlags = MSGQUEUE_NOPRECOMMIT | MSGQUEUE_ALLOW_BROKEN;
//		MsgQueueOpt.dwMaxMessages = 0;
//		MsgQueueOpt.cbMaxMessage = sizeof(PACGestureData_t);
//		MsgQueueOpt.bReadAccess = TRUE;
//
//		hMsgQueue = CreateMsgQueue(PAC_MSG_QUEUE_NAME, &MsgQueueOpt);
//		if(hMsgQueue == NULL)
//		{
//			return -1;
//		}
//
//		while(1)
//		{
//			if(!ReadMsgQueue(hMsgQueue, &g_GestureData, sizeof(g_GestureData), &dwBytesRead, INFINITE, &dwFlags))
//			{
//				RETAILMSG(1, (TEXT("ERR:[%s]: ReadMsgQueue failed!!!\r\n"), TEXT(__FUNCTION__)));
//				continue;
//			}
//
//			if(g_hUIWnd)
//			{
//				::SendMessage(g_hUIWnd,WM_MSG_GESTURE_ID,0,0);
//			}
//		}
//	}

DWORD	EventSignalThreadProc(LPVOID lparam)
{
	DWORD dwWait = 0;
	DWORD dwEvtCount = 0;
	HANDLE ahEvent[9] = {NULL};
	DWORD dwSpeechEvent = -1;
	DWORD dwTouchEvent = -1;
	DWORD dwGestureEvent = -1;
	DWORD dwMHLEvent = -1;
	DWORD dwRearviewEvent = -1;
    DWORD dwRearviewColorSignalEvent = -1;
	DWORD dwHubAttachedEvent = -1;

	CeSetThreadPriority(GetCurrentThread(), CE_THREAD_PRIO_256_ABOVE_NORMAL);

	if( g_hEvtSpeech!=INVALID_HANDLE_VALUE)
	{
		ahEvent[dwEvtCount] = g_hEvtSpeech;
		dwSpeechEvent = dwEvtCount++;
	}
	if( g_hEvtTouch!=INVALID_HANDLE_VALUE)
	{
		ahEvent[dwEvtCount] = g_hEvtTouch;
		dwTouchEvent = dwEvtCount++;
	}
	if( g_hEvtGesture!=INVALID_HANDLE_VALUE)
	{
		ahEvent[dwEvtCount] = g_hEvtGesture;
		dwGestureEvent = dwEvtCount++;
	}
	if( g_hEvtMHL!=INVALID_HANDLE_VALUE)
	{
		ahEvent[dwEvtCount] = g_hEvtMHL;
		dwMHLEvent = dwEvtCount++;
	}
	if( g_hRearViewNotifyEvent!=INVALID_HANDLE_VALUE)
	{
		ahEvent[dwEvtCount] = g_hRearViewNotifyEvent;
		dwRearviewEvent = dwEvtCount++;
	}
	if( g_hRearViewColorSignalChange!=INVALID_HANDLE_VALUE)
	{
		ahEvent[dwEvtCount] = g_hRearViewColorSignalChange;
		dwRearviewColorSignalEvent = dwEvtCount++;
	}
	if( g_hHubAttachedEvent != NULL)
	{
		ahEvent[dwEvtCount] = g_hHubAttachedEvent;
		dwHubAttachedEvent = dwEvtCount++;
	}

	while (TRUE)
	{
		dwWait = WaitForMultipleObjects(dwEvtCount, ahEvent, FALSE, INFINITE);

		if( dwWait == dwSpeechEvent )
		{
			if(g_hUIWnd)
			{
				::SendMessage(g_hUIWnd,WM_MSG_SPEECH,GetEventData(g_hEvtSpeech),0);
			}
		}
		else if( dwWait == dwTouchEvent )
		{
			if(g_hUIWnd)
			{
				::SendMessage(g_hUIWnd,WM_MSG_TOUCH,0,0);
			}
		}
		else if( dwWait == dwGestureEvent )
		{
			if(g_hUIWnd)
			{
				::SendMessage(g_hUIWnd,WM_MSG_GESTURE,GetEventData(g_hEvtGesture),0);
			}
		}
		else if( dwWait == dwMHLEvent )
		{
			if(g_hUIWnd)
			{
				::SendMessage(g_hUIWnd,WM_MSG_MHL,GetEventData(g_hEvtMHL),0);
			}
		}
		else if( dwWait == dwRearviewEvent )
		{
			if(g_hUIWnd)
			{
				::SendMessage(g_hUIWnd,WM_MSG_REARVIEW,GetEventData(g_hRearViewNotifyEvent),0);
			}
		}
		else if( dwWait == dwRearviewColorSignalEvent)
		{
			if(g_hUIWnd)
			{
				::SendMessage(g_hUIWnd,WM_MSG_REARVIEW_COLORSIGNAL,GetEventData(g_hRearViewColorSignalChange),0);
			}
		}
		else if( dwWait == dwHubAttachedEvent)
		{
			if(g_hUIWnd)
			{
				::SendMessage(g_hUIWnd,WM_MSG_USB_HUB_ATTACHED,0,0);
			}
		}
	}

	return 0;
}

ATOM			RegisterUIWndClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc={0};

	memset (&wc, 0, sizeof(wc));

	wc.style         =  CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = UIWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = 0;//LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
ATOM RegisterMainWndClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc={0};
	memset (&wc, 0, sizeof(wc));

	wc.style         =  CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIGHTNINGUI));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	TCHAR szTitle[MAX_LOADSTRING];		// title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

	SetLastError(0);
	g_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READONLY,0,sizeof(MULTIPOINT),TOUCH_SHARE_MEM_NAME);
	if(NULL == g_hMapFile)
	{
		RETAILMSG(1, (TEXT("CreateFileMapping[TOUCH_SHARE_MEM_NAME] failed Err:%d.\r\n"),GetLastError()));
		return FALSE;
	}

	SetLastError(0);
	if(g_hMapFile)
	{
		g_pMultiPointShareView = (MULTIPOINT*)MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MULTIPOINT));
		if(NULL == g_pMultiPointShareView)
		{
			RETAILMSG(1, (TEXT("MapViewOfFile failed Err:%d.\r\n"),GetLastError()));
			return FALSE;
		}
	}

	g_hInst = hInstance; // Store instance handle in our global variable

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LIGHTNINGUI, szWindowClass, MAX_LOADSTRING);


	if (!RegisterMainWndClass(hInstance, szWindowClass))
	{
		return FALSE;
	}
	if (!RegisterUIWndClass(hInstance, UI_WND_CLASS_NAME))
	{
		return FALSE;
	}
	//     hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
	//         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	g_hMainWnd = CreateWindowEx(
		WS_EX_NOANIMATION ,// | WS_EX_NOACTIVATE,
		szWindowClass,
		szTitle,
		WS_VISIBLE ,//| WS_DISABLED,//WS_POPUP,
		CW_USEDEFAULT, //
		CW_USEDEFAULT, //
		CW_USEDEFAULT, //
		CW_USEDEFAULT, //
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!g_hMainWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hMainWnd, /*nCmdShow*/SW_HIDE);
	UpdateWindow(g_hMainWnd);


	g_hUIWnd = CreateWindowEx(
		WS_EX_NOANIMATION ,
		UI_WND_CLASS_NAME,
		UI_WND_TITLE,
		WS_CHILD|WS_VISIBLE,//
		(g_iScreenWidth-g_iClientWidth)/2,//+50,//CW_USEDEFAULT,
		(g_iScreenHeight-g_iClientHeight)/2,//CW_USEDEFAULT,
		g_iClientWidth-1,//CW_USEDEFAULT,
		g_iClientHeight,//CW_USEDEFAULT,
		// 		CW_USEDEFAULT, //
		// 		CW_USEDEFAULT, //
		// 		CW_USEDEFAULT, //
		// 		CW_USEDEFAULT, //
		g_hMainWnd,//NULL, //
		NULL,
		hInstance,
		NULL);

	ShowWindow(g_hUIWnd, /*SW_SHOW*/SW_HIDE);
	UpdateWindow(g_hUIWnd);

	return TRUE;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		break;
	case WM_SETCURSOR:
		break;
	case WM_ACTIVATE:
		if(g_pGUI)
		{
			g_pGUI->OnActive(wParam, lParam);
		}
		break;
	case WM_COPYDATA:
		{
			PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
			RETAILMSG(1, (_T("**MainWndProc -WM_COPYDATA dwData:0x%x\r\n"),pcds->dwData));
		}
		break;
	case WM_BACKLIGHT_CHANGE:
		if(g_pGUI)
		{
			g_pGUI->OnBacklightChanged(wParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);

			if(g_pGUI)
			{
				g_pGUI->OnTouchUp(&pt);
			}
		}
		break;
	case WM_DEVICECHANGE:

		switch (wParam)
		{
		case DBT_DEVICEARRIVAL: //
			{
				DEV_BROADCAST_HDR* dhr = (DEV_BROADCAST_HDR *)lParam;
				PDEV_BROADCAST_PORT lpdbp = (PDEV_BROADCAST_PORT)dhr;

				UINT                 idDeviceType;
				LPCTSTR              lpstrDeviceName;

				idDeviceType=lpdbp->dbcp_devicetype;
				lpstrDeviceName = lpdbp->dbcp_name;
				RETAILMSG(1,(_T("+++++DBT_DEVICEARRIVAL.size:%d,type%d,%s\r\n"),lpdbp->dbcp_size,lpdbp->dbcp_devicetype,lpdbp->dbcp_name));

				if( lpdbp->dbcp_devicetype == DBT_DEVTYP_PORT )
				{
					if(g_pGUI)
					{
						g_pGUI->OnDeviceArrived(lpdbp->dbcp_name);
					}
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			{
				DEV_BROADCAST_HDR* dhr = (DEV_BROADCAST_HDR *)lParam;
				PDEV_BROADCAST_PORT lpdbp = (PDEV_BROADCAST_PORT)dhr;
				UINT                 idDeviceType;
				LPCTSTR              lpstrDeviceName;

				idDeviceType=lpdbp->dbcp_devicetype;
				lpstrDeviceName = lpdbp->dbcp_name;
				RETAILMSG(1,(_T("+++++DBT_DEVICEREMOVECOMPLETE.type:%d,%s\r\n"),lpdbp->dbcp_devicetype,lpdbp->dbcp_name));

				if( lpdbp->dbcp_devicetype == DBT_DEVTYP_PORT )
				{
					if(g_pGUI)
					{
						g_pGUI->OnDeviceRemoved(lpdbp->dbcp_name);
					}
				}
			}
			break;
		default:
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK UIWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//     int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	POINT pt;
	static BOOL bTouchDown = FALSE;

	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		break;
	case WM_GESTURE:
		{
			//RETAILMSG(1, (TEXT("WM_GESTURE wParam: %d,lParam:%x \r\n"),wParam, lParam));
			if(g_pGUI)
			{
				g_pGUI->OnGesture(wParam,lParam);
			}
		}
		break;
	case WM_SETCURSOR:
		break;
	case WM_COPYDATA:
		{
			PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
			RETAILMSG(1, (TEXT("UIWndProc WM_COPYDATA - : %d \r\n"), lParam));
		}
		break;
	case WM_TIMER:
		{
			switch (wParam)
			{
			case TIMER_SPEECH_TERMINATE:
				if(g_pGUI)
				{
			        //RETAILMSG(1, (TEXT("APP:MSG:g_pGUI->OnSpeechTerminate() \r\n")));
					g_pGUI->OnSpeechTerminate();
				}
				break;
			case TIMER_AUTO_LOAD_MODULES:
				if(g_pGUI)
				{
					g_pGUI->OnLoadModules();
				}
				break;
			case TIMER_AUTO_OPEN_SRC:
				if(g_pGUI)
				{
					g_pGUI->OnAutoOpenSrc();
				}
				break;
			case TIMER_LOAD_CARPLAY_MODULE:
				if(g_pGUI)
				{
					g_pGUI->OnLoadCarPlayService();
				}
				break;
			case TIMER_VEHICLE_INFO_PROC:
				if(g_pGUI)
				{
					g_pGUI->OnVehicleInfoTimerProc();
				}
				break;
			case TIMER_MAIN_VOLUME_FADE:
				if(g_pGUI)
				{
					g_pGUI->OnVolumeFadeProc();
				}
				break;
			case TIMER_MIX_VOLUME_FADE:
				if(g_pGUI)
				{
					//g_pGUI->OnVolumeFadeProc();
				}
				break;
			case TIMER_DEVICE_CHANGED:
				if(g_pGUI)
				{
					g_pGUI->OnDeviceChanged();
				}
				break;
			case TIMER_AUTO_START_NAVI:
				if(g_pGUI)
				{
					g_pGUI->OnAutoStartNaviProc();
				}
				break;
			case TIMER_SYSTEM_POWER_OPERATION_READY:
				if(g_pGUI)
				{
					g_pGUI->OnSystemPowerOperationReady();
				}
				break;
			case TIMER_REVERSE_SIGNAL:
				if(g_pGUI)
				{
					g_pGUI->OnReverseTimerProc();
				}
				break;
			case TIMER_REARVIEW_PRE_DONE:
				if(g_pGUI)
				{
					g_pGUI->OnRearViewPreDoneTimerProc();
				}
				break;
			case TIMER_REARVIEW_DONE:
				if(g_pGUI)
				{
					g_pGUI->OnRearViewDoneTimerProc();
				}
				break;
			case TIMER_RADIO_UPDATE_PLAYINFO:
				if(g_pGUI)
				{
					g_pGUI->OnUpdateRadioPlayInfo();
				}
				break;
			case TIMER_RADIO_MONITOR_TA:
				if(g_pGUI)
				{
					g_pGUI->OnRadioMonitorTA();
				}
				break;
			case TIMER_CARPLAY_AUTO_SWITCH_MEDIA:
                if(g_pGUI)
				{
					g_pGUI->AutoSwitchCarPlayMedia();
				}
				break;
			case TIMER_CARPLAY_AUTO_SWITCH_SCREEN:
				if(g_pGUI)
				{
					g_pGUI->AutoSwitchCarPlayScreen();
				}
				break;
			case TIMER_CARPLAY_AUTO_REQUEST_SCREEN:
				if(g_pGUI)
				{
					g_pGUI->AutoRequestCarPlayScreen();
				}
				break;
			case TIMER_CARPLAY_AUTO_SWITCH_NATIVEUI:
				if(g_pGUI)
				{
					g_pGUI->AutoSwitchNativeUI();
				}
				break;
    //        case TIMER_SETTING_ADVANCE_STEERWHEEL:
				//if(g_pGUI)
    //            {
				//	g_pGUI->OnSteerWheelTimerProc();
    //            }
    //            break;
			case TIMER_AUDIO_MUTE_TIMEOUT:
				if(g_pGUI)
				{
					g_pGUI->OnAudioMuteTimeout();
				}
				break;
			case TIMER_AUTO_RELEASE_SPEECH:
				if(g_pGUI)
				{
					g_pGUI->OnTimerReleaseSpeech();
				}
				break;
			}
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			if(g_pGUI)
			{
				g_pGUI->Render();
			}
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	case WM_SYSTEM_POWER_STATE:
		if(g_pGUI)
		{
			g_pGUI->OnSystemPowerState(wParam);
		}
		break;
	case WM_SYSTEM_PRE_POWER_STATE:
		if(g_pGUI)
		{
			g_pGUI->OnSystemPrePowerState(wParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);

			if(g_pGUI)
			{
				bTouchDown = TRUE;
				g_pGUI->OnTouchDown(&pt);
			}
// 			if(pt.x>950&&pt.y<50)
// 			{
// 				 PostMessage(hWnd,WM_QUIT,0,0);
// 			}
		}
		break;
	case WM_LBUTTONUP:
		{
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			if(g_pGUI)
			{
				bTouchDown = FALSE;
				g_pGUI->OnTouchUp(&pt);
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			if(g_pGUI)
			{
				if (bTouchDown)
				{
					g_pGUI->OnTouchMove(&pt);
				}
				/*
				else
				{
					bTouchDown = TRUE;
					g_pGUI->OnTouchDown(&pt);
				}
				*/
			}
		}
		break;
	case WM_MUTE_AUDIO_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnMuteMsg(wParam,lParam);
		}
		break;
	case WM_MANUAL_MUTE:
		if(g_pGUI)
		{
			g_pGUI->OnMuteMsg(wParam,lParam);
		}
		break;
	case WM_PHONE_CALL_STATE:
		if(g_pGUI)
		{
			g_pGUI->OnPhoneCallState(wParam,lParam);
		}
		break;
	case WM_PHONE_HF_STATE:
		if(g_pGUI)
		{
			g_pGUI->OnPhoneHFState(wParam,lParam);
		}
		break;
	case WM_CMD_RINGTONE:
		if(g_pGUI)
		{
			g_pGUI->OnRingToneCmd(wParam,lParam);
		}
		break;
	case WM_SMARTBAR_NOTIFY:
		if(g_pGUI)
		{
			g_pGUI->OnSmartBarNotify(wParam,lParam);
		}
		break;
	case WM_SMARTBAR_CONTROL:
		if(g_pGUI)
		{
			g_pGUI->OnSmartBarControl(wParam,lParam);
		}
		break;
	case WM_BLUETOOTH_SETUP_STATUS:
		if(g_pGUI)
		{
			g_pGUI->OnBluetoothSetupStatus(wParam,lParam);
		}
		break;
	case WM_BLUETOOTH_MESSAGE:
		if(g_pGUI)
		{
			g_pGUI->OnBluetoothMsg(wParam,lParam);
		}
		break;
	case WM_CAMERA_TYPE_CHANGE:
		if(g_pGUI)
		{
			g_pGUI->OnCameraTypeChanged(wParam);
		}
		break;
	case WM_BACKGROUND_CHANGE:
		if(g_pGUI)
		{
			g_pGUI->OnBackgroundChanged(wParam,lParam);
		}
		break;
	case WM_VEHICLE_CANBUS_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnCanbusStatus(wParam,lParam);
		}
		break;
	case WM_VEHICLE_SPEED_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnVehicleSpeed(wParam,lParam);
		}
		break;

	case WM_MSG_SPEECH:
		if(g_pGUI)
		{
            //RETAILMSG(1, (TEXT("MSG:g_pGUI->OnSpeechMsg(%d, %d).\r\n"),wParam,lParam));
			g_pGUI->OnSpeechMsg(wParam,lParam);
		}
		break;
	case WM_MSG_SR:
		if(g_pGUI)
		{
			g_pGUI->OnSRMsg(wParam,lParam);
		}
		break;
	case WM_MSG_TOUCH:
		if(g_pGUI)
		{
			memcpy(&g_MultiPoint, g_pMultiPointShareView, sizeof(MULTIPOINT));
			g_pGUI->OnTouchMsg(&g_MultiPoint);
		}
		break;
	case WM_MSG_GESTURE:
		if(g_pGUI)
		{
#if (USE_TOUCH_CYPRESS_GESTURE == 1)
			g_pGUI->OnGestureMsg(wParam,lParam);
#endif
		}
		break;
	case WM_AUDIO_CODEC_MSG:
#if (CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418 || CVTE_DEF_AUDIO_TYPE==CVTE_AUDIO_TYPE_AK7738)
		if(g_pGUI)
		{
			g_pGUI->OnAudioCodecMsg(wParam,lParam);
		}
#endif
		break;
	case WM_MSG_MHL:
		if(g_pGUI)
		{
			g_pGUI->OnMHLMsg(wParam,lParam);
		}
		break;
	case WM_MSG_REARVIEW:
		if(g_pGUI)
		{
			//int para=LOWORD(wParam);
			//int pre=HIWORD(wParam);
//				RETAILMSG(1, (TEXT("# WM_MSG_REARVIEW #  para:%d pre:%d\r\n"),para,pre));
//				if(para)
//				{
//					if(pre)
//					{
//						g_pGUI->OnRearViewPrepare(TRUE);
//						RETAILMSG(1, (TEXT("..send feedback...\r\n")));
//						SetEvent(g_hRearViewNotifyFeedbackEvent);
//					}
//					else
//					{
//						g_pGUI->OnRearViewDone(TRUE);
//					}
//				}
//				else
//				{
//					g_pGUI->OnRearViewPrepare(FALSE);
//					g_pGUI->OnRearViewDone(FALSE);
//				}
            RETAILMSG(1, (TEXT("WM_MSG_REARVIEW: wParam:%x\r\n"),wParam));

			switch(wParam)
			{
                case REARVIEW_CMD_START:
					//Recevice the kernel event, and disable the MCU rear cmd.
					g_pGUI->OnRearViewMCUCmdDone(FALSE);
                    g_pGUI->OnRearViewPrepare(TRUE);
                    RETAILMSG(1, (TEXT("..send feedback...\r\n")));
                    SetEvent(g_hRearViewNotifyFeedbackEvent);
                    break;
                case REARVIEW_CMD_DONE:
					//The current status is rear, disable the MCU rear cmd.
					g_pGUI->OnRearViewMCUCmdDone(FALSE);
					g_pGUI->OnRearViewPrepare(TRUE);
                    g_pGUI->OnRearViewDone(TRUE);
                    break;
                case REARVIEW_CMD_FAIL:
                case REARVIEW_CMD_STOP:
					//The reverse has been finished. enable the MCU rear cmd.
                    g_pGUI->OnRearViewPrepare(FALSE);
                    g_pGUI->OnRearViewDone(FALSE);
					g_pGUI->OnRearViewMCUCmdDone(TRUE);
                    break;
            }
		}
		break;
    case WM_MSG_REARVIEW_COLORSIGNAL:
		if(g_pGUI)
        {
            UINT colorsignal = wParam;
			RETAILMSG(1, (TEXT("The Current colorsignal is :%d\r\n"),colorsignal));
			g_pGUI->OnRearColorSignalChange(colorsignal);
        }
        break;
	case WM_MSG_USB_HUB_ATTACHED:
		if(g_pGUI)
        {
			g_pGUI->OnUSBHUBAttached();
		}
		break;
#if (CVTE_EN_GESTURE_AIR == 1)
	case WM_MSG_GESTURE_ID:
		if(g_pGUI)
		{
			g_pGUI->OnGestureID(wParam,lParam);
		}
		break;
	case WM_MSG_GESTURE_CIRCLE:
		if(g_pGUI)
		{
			g_pGUI->OnGestureCircle(wParam,lParam);
		}
		break;
#endif
#if CVTE_EN_CARPLAY
	case WM_MSG_CARPLAY_ARRIVE:
		if(g_pGUI)
		{
			g_pGUI->OnCarPlayArrived();
		}
		break;
	case WM_MSG_CARPLAY_REMOVE:
		if(g_pGUI)
		{
			g_pGUI->OnCarPlayRemove();
		}
		break;
#endif
	case WM_RDS_TA_STATUS:
		if(g_pGUI)
		{
			g_pGUI->OnNotifyRdsTAStatus(wParam,lParam);
		}
		break;
	case WM_VOLUME_CHANGE:
		if(g_pGUI)
		{
			g_pGUI->OnVolumeChange(wParam,lParam);
		}
		break;
	case WM_VEHICLE_RADAR_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnRadarStatus(wParam,lParam);
		}
		break;
	case WM_SWITCH_PANEL:
		if(g_pGUI)
		{
			return g_pGUI->OnPanelChange((SWITCH_PAGE_PARAM*)lParam);
		}
		break;
	case WM_NAVI_PATH_CAHNGED:
		if(g_pGUI)
		{
			g_pGUI->OnNaviPathChanged(wParam,lParam);
		}
		break;
// 	case WM_REVERSE_STATUS:
// 		if(g_pGUI)
// 		{
// 			g_pGUI->OnReverseSignal(wParam,lParam);
//
// 			if(wParam)
// 			{
// 				//feed back:
// 				RETAILMSG(1, (TEXT("..send feedback...\r\n")));
// 				SetEvent(g_hRearViewNotifyFeedbackEvent);
// 			}
// 		}
// 		break;
	case WM_GRAPHNOTIFY:
		if(g_pGUI)
		{
			g_pGUI->OnHandleGraphEvent();
		}
		break;
	case WM_SHOW_MSGBOX:
		if(g_pGUI)
		{
			g_pGUI->OnShowMsgBox(wParam,lParam);
		}
		break;
	case WM_SHOW_EXPLORER:
		if(g_pGUI)
		{
			g_pGUI->OnShowExplorer(wParam,lParam);
		}
		break;
	case WM_SHOW_KEYBOARD:
		if(g_pGUI)
		{
			g_pGUI->OnShowKeyboard(wParam,lParam);
		}
		break;
	case WM_SHOW_PASSWORD_BOX_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnShowPasswordBox(wParam,lParam);
		}
		break;
	case WM_PASSWORD_BOX_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnPasswordBoxCmd(wParam,lParam);
		}
		break;
	case WM_MCU_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnMCUCmd(wParam,lParam);
		}
		break;
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
	case WM_EXTENSIBLE_CMD:
		if (g_pGUI)
		{
			g_pGUI->OnExtensibleCmd(wParam,lParam);
		}
		break;
#endif
	case WM_DISK_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnDiskCmd(wParam,lParam);
		}
		break;
	case WM_VEHICLEINFO_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnVehicleInfoCmd(wParam,lParam);
		}
		break;
	case WM_MCU_UPDATE_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnMcuUpdateCmd(wParam,lParam);
		}
		break;
	case WM_NAVI_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnNaviMsg(wParam,lParam);
		}
		break;
	case WM_PANEL_KEY:
		if(g_pGUI)
		{
			g_pGUI->OnPanelKey(wParam,lParam);
		}
		break;
#if !CVTE_EN_KEY_PROXY
	case WM_PHONE_KEY:
		if(g_pGUI)
		{
			g_pGUI->OnPhoneKey(wParam,lParam);
		}
		break;
#endif
	case WM_VCHICLE_KEY:
		if(g_pGUI)
		{
			g_pGUI->OnVehicleKey(wParam,lParam);
		}
		break;
#if !CVTE_EN_KEY_PROXY	
	case WM_MEDIA_KEY:
		if(g_pGUI)
		{
			g_pGUI->OnMediaKey(wParam,lParam);
		}
		break;
#endif	
#if CVTE_EN_KEY_PROXY		
	case WM_KEY:
		if(g_pGUI)
		{
			g_pGUI->OnKey(wParam,lParam);
		}
		break;
#endif		
	case WM_REARVIEW_KEY:
		if(g_pGUI)
		{
			g_pGUI->OnRearViewKey(wParam,lParam);
		}
		break;
	case WM_BREAK_STATUS:
		if(g_pGUI)
		{
			g_pGUI->OnBrakeStatusChanged(wParam);
		}
		break;
	case WM_MEDIAPLAYER_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnMediaPlayerCommand(wParam,lParam);
		}
		break;
	case WM_IMAGEPLAYER_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnImagePlayerCommand(wParam,lParam);
		}
		break;
	case WM_DAB_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnDABCommand(wParam,lParam);
		}
		break;
	case WM_BLUETOOTH_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnBluetoothCommand(wParam,lParam);
		}
		break;
	case WM_RADIO_CMD:
		if(g_pGUI)
		{
			g_pGUI->OnRadioCommand(wParam,lParam);
		}
		break;
	case WM_IPOD_CMD:
		if(g_pGUI)
		{
			g_pGUI->OniPodCommand(wParam,lParam);
		}
		break;
    case WM_FACTORY_CMD:
        if(g_pGUI)
        {
            g_pGUI->OnFactoryCmd(wParam,lParam);
        }
        break;
	case WM_AUDIO_SRC_CMD:
		if(g_pGUI)
		{
			return g_pGUI->OnSwitchAudioSRC(wParam,lParam);
		}
		break;
	case WM_GET_AUDIO_SRC:
		if(g_pGUI)
		{
			return g_pGUI->OnGetAudioSRC(wParam,lParam);
		}
		break;
	case WM_MEDIA_CLASS_SRC_CMD:
		if(g_pGUI)
		{
			return g_pGUI->OnSwitchMediaClassSRC(wParam,lParam);
		}
		break;
	case WM_GET_PLAYING_STATUS:
		if(g_pGUI)
		{
			return g_pGUI->OnGetPlayingStatus();
		}
		break;
	case WM_GET_CUR_MUTE_STATUS:
		if(g_pGUI)
		{
			return g_pGUI->OnGetCurMuteStatus();
		}
	case WM_CMD_PLAY_SOUND_EFFECT:
		if(g_pGUI)
		{
			return g_pGUI->OnPlaySoundEffect(wParam,lParam);
		}
		break;
	case WM_VOLUME_CONFIG_MSG:
		if(g_pGUI)
		{
			return g_pGUI->OnVolumeConfigMsg(wParam,lParam);
		}
		break;
	case WM_SPEECH_ENABLE_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnSpeechEnableMsg(wParam,lParam);
		}
		break;
	case WM_IPOD_MESSAGE:
		if(g_pGUI)
		{
			g_pGUI->OniPodMsg(wParam, lParam);
		}
		break;
	case WM_WIDEGT_CHANGED:
		if(g_pGUI)
		{
			g_pGUI->OnWidgetChanged(wParam, lParam);
		}
		break;
	case WM_MEDIA_SOURCE_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnAvailableSourceChanged(wParam,lParam);
		}
		break;
	case WM_MCU_UPDATE_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnMcuUpdateMsg(wParam,lParam);
		}
		break;
	case WM_DEVICE_INSERT_READY:
		if(g_pGUI)
		{
			g_pGUI->OnDeviceArrivedReady(wParam, lParam);
		}
		break;
	case WM_DEVICE_REMOVE_READY:
		if(g_pGUI)
		{
			g_pGUI->OnDeviceRemovedReady(wParam, lParam);
		}
		break;
	case WM_NOTIFY_VOLUME_INFO:
		if(g_pGUI)
		{
			g_pGUI->OnNotifyVolumeInfo(wParam,lParam);
		}
		break;
	case WM_NOTIFY_SOURCE_INFO:
		if(g_pGUI)
		{
			g_pGUI->OnNotifySourceInfo(wParam,lParam);
		}
		break;
	case WM_NOTIFY_MEDIA_PLAYBACK_INFO:
		if(g_pGUI)
		{
			g_pGUI->OnNotifyMediaPlaybackInfo(wParam,lParam);
		}
		break;
	case WM_NOTIFY_RADIO_INFO:
		if(g_pGUI)
		{
			g_pGUI->OnNotifyRadioInfo(wParam,lParam);
		}
		break;
#if CVTE_EN_ANDROID_AUTO
	case WM_ANDROID_AUTO_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnAndroidAutoMsg(wParam,lParam);
		}
		break;
#endif
#if CVTE_EN_CARPLAY
	case WM_CARPLAY_STATUS:
		if(g_pGUI)
		{
			g_pGUI->OnCarPlayMsg(wParam,lParam);
		}
		break;
#endif
#if CVTE_EN_CPMAC
	case WM_CARPLAY_MAC_CHANGED:
		if(g_pGUI)
		{
			g_pGUI->OnCarPlayMACChanged(wParam,lParam);
		}
		break;
#endif
	case WM_SR_MSG_InitStatus:
	case WM_SR_MSG_UpLoadDictToLocalStatus:
	case WM_SR_MSG_UpLoadDictToCloudStatus:
	case WM_SR_MSG_VolumeLevel:
	case WM_SR_MSG_ResponseTimeout:
	case WM_SR_MSG_SpeechStart:
	case WM_SR_MSG_SpeechTimeOut:
	case WM_SR_MSG_SpeechEnd:
	case WM_SR_MSG_Error:
	case WM_SR_MSG_Result:
		if(g_pGUI)
		{
			g_pGUI->OnHandleSRMsg(message,wParam,lParam);
		}
		break;
	case WM_DAB_SERVICE_LINK_MSG:
		if(g_pGUI)
		{
			g_pGUI->OnDabServiceLinkMsg(wParam,lParam);
		}
		break;
	case WM_DAB_SERVICE_START_READY:
		if(g_pGUI)
		{
			g_pGUI->OnDabServiceStartReady(wParam,lParam);
		}
		break;	
#if CVTE_EN_ACCON_WARNING
	case WM_SHOW_WARNING_PAGE:
		if(g_pGUI)
		{
			g_pGUI->OnEnterWarningPanel(wParam,lParam);
		}
		break;
	case WM_HIDE_WARNING_PAGE:
		if(g_pGUI)
		{
			g_pGUI->OnQuitWarningPanel(wParam,lParam);
		}
		break;
#endif
	case WM_SMARTBARVOL_EQ:
		if(g_pGUI)
		{
			g_pGUI->QuickEnterEQSetting();
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

