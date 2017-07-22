#include "ImageCommon.h"

extern HWND g_hUIWnd;

void SendImageCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_hUIWnd)
	{
		::SendMessage(g_hUIWnd,WM_IMAGEPLAYER_CMD,wParam,lParam);
		//::PostMessage(g_hUIWnd,WM_IMAGEPLAYER_CMD,wParam,lParam);
	}
}
