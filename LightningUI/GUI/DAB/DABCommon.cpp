
#include "DABCommon.h"

#if CVTE_EN_DAB

extern HWND g_hUIWnd;

void SendDABCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_hUIWnd)
	{
		::SendMessage(g_hUIWnd,WM_DAB_CMD,wParam,lParam);
		//::PostMessage(g_hUIWnd,WM_IMAGEPLAYER_CMD,wParam,lParam);
	}
}
#endif
