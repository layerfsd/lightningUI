#include "../StdAfx.h"
#include "Platform_macro.h"
#include <windev.h>
#include "Backlight.h"
#include "../GUI/Common/message_map.h"

extern HWND g_hMainWnd;
HANDLE g_hDevBKL=INVALID_HANDLE_VALUE;

void BKL_InitBackLight(int iBKL)
{
	int iNewPos;
	iNewPos = iBKL;

	if(g_hDevBKL==INVALID_HANDLE_VALUE)
	{
		g_hDevBKL = CreateFile(TEXT("bkl1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);
	}

	if(g_hDevBKL!=INVALID_HANDLE_VALUE)
	{
		if (!DeviceIoControl(g_hDevBKL, IOCTL_BACKLIGHT_INIT, &iNewPos, sizeof(iNewPos),
			NULL, 0, NULL, NULL))
		{
			RETAILMSG(DEBUG_BKL, (TEXT("Failed DeviceIoControl IOCTL_BACKLIGHT_INIT -- error = 0x%x\r\n"), GetLastError()));
		}
	}


}

void BKL_SetBackLight(int iBKL,BOOL bJump,BOOL bLedAnti)
{
	static int pos=-1;
	int iNewPos = iBKL;

//	    RETAILMSG(1, (TEXT("MSG:BKL_SetBackLight iBKL:%d bJump:%d bLedAnti:%d\r\n"),iBKL,bJump,bLedAnti));

#if (CVTE_DEF_DISPLAY_TYPE == CVTE_DISPLAY_TYPE_U621P)
	if(pos!=iNewPos || bJump)
#endif
	{
		if(g_hDevBKL==INVALID_HANDLE_VALUE)
		{
			g_hDevBKL = CreateFile(TEXT("bkl1:"), GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_ALWAYS, 0, 0);
		}

		if(g_hDevBKL!=INVALID_HANDLE_VALUE)
		{
			if (!DeviceIoControl(g_hDevBKL, IOCTL_BACKLIGHT_SET, &iNewPos, sizeof(iNewPos),
				NULL, 0, NULL, NULL))
			{

				RETAILMSG(1, (TEXT("Failed DeviceIoControl IOCTL_BACKLIGHT_SET -- error = 0x%x\r\n"), GetLastError()));
			}
			else
			{
                //RETAILMSG(1, (TEXT("MSG:BKL_SetBackLight:%d\r\n"),iBKL));
#if (CVTE_DEF_DISPLAY_TYPE == CVTE_DISPLAY_TYPE_U621P)
				if(bLedAnti)
				{
					pos = iNewPos;
	// 				::PostMessage(g_hMainWnd,WM_BACKLIGHT_CHANGE,iBKL,NULL);
					::SendMessage(g_hMainWnd,WM_BACKLIGHT_CHANGE,iBKL,NULL);
				}
#endif
			}
		}
	}

}
int BKL_GetBackLight()
{
	int iNewPos = -1;

	if(g_hDevBKL==INVALID_HANDLE_VALUE)
	{
		g_hDevBKL = CreateFile(TEXT("bkl1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);
	}

	if(g_hDevBKL!=INVALID_HANDLE_VALUE)
	{
		if (!DeviceIoControl(g_hDevBKL, IOCTL_BACKLIGHT_GET, NULL, 0,
			&iNewPos, sizeof(iNewPos), NULL, NULL))
		{

			RETAILMSG(DEBUG_BKL, (TEXT("Failed DeviceIoControl IOCTL_BACKLIGHT_GET -- error = 0x%x\r\n"), GetLastError()));
		}
	}

	return iNewPos;
}
void BKL_TurnBKLSOn()
{
	if(g_hDevBKL==INVALID_HANDLE_VALUE)
	{
		g_hDevBKL = CreateFile(TEXT("bkl1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);
	}

	if(g_hDevBKL!=INVALID_HANDLE_VALUE)
	{
		if (!DeviceIoControl(g_hDevBKL, IOCTL_BACKLIGHTSCALING_ON, NULL, 0,
			NULL, 0, NULL, NULL))
		{

			RETAILMSG(DEBUG_BKL, (TEXT("Failed DeviceIoControl IOCTL_BACKLIGHTSCALING_ON -- error = 0x%x\r\n"), GetLastError()));
		}
	}

}
void BKL_TurnBKLSOff()
{
	if(g_hDevBKL==INVALID_HANDLE_VALUE)
	{
		g_hDevBKL = CreateFile(TEXT("bkl1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);
	}

	if(g_hDevBKL!=INVALID_HANDLE_VALUE)
	{
		if (!DeviceIoControl(g_hDevBKL, IOCTL_BACKLIGHTSCALING_OFF, NULL, 0,
			NULL, 0, NULL, NULL))
		{

			RETAILMSG(DEBUG_BKL, (TEXT("Failed DeviceIoControl IOCTL_BACKLIGHTSCALING_OFF -- error = 0x%x\r\n"), GetLastError()));
		}
	}
}
