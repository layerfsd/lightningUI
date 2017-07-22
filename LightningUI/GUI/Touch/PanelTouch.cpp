#include "PanelTouch.h"
#include "../../resource.h"
#include "afxwin.h"

extern const wchar_t* g_strDefaultBackground;
extern HWND			g_hMainWnd;
CPanelTouch * g_pCPanelTouch = NULL;

CPanelTouch::CPanelTouch(void)
{
    m_hTouchWnd = NULL;
    m_iTestState = 0;   //Idle
    m_iFactoryTestCmd = FACTORY_CMD_TEST_STOP;
    m_iTestState = touchteststateidle;
}

CPanelTouch::~CPanelTouch(void)
{
}
BOOL CPanelTouch::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelTouch::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	RETAILMSG(1,(_T("CPanelTouch::OnQuitPanel iParam:%d, idAni %d\r\n"),iParam,idAni));

	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

    m_iFactoryTestCmd = FACTORY_CMD_TEST_STOP;
    m_iTestState = touchteststateidle;

	if(IsWindow(m_hTouchWnd) && IsWindowVisible(m_hTouchWnd))
	{
	    RETAILMSG(1,(_T("CPanelTouch::OnQuitPanel Hide TouchWnd \r\n")));
        ShowTouchWindow(FALSE);
	}

}
void CPanelTouch::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(1/*DEBUG_CARPLAY*/,(_T("CPanelTouch::OnEnterPanel iParam:%d, idAni %d\r\n"),iParam,idAni));

    if(IsWindow(m_hTouchWnd) && !IsWindowVisible(m_hTouchWnd))
	{
	    RETAILMSG(1,(_T("CPanelTouch::OnEnterPanel Show TouchWnd \r\n")));
        ShowTouchWindow(TRUE);
	}
	else
	{
	    RETAILMSG(1,(_T("CPanelTouch::OnEnterPanel LaunchTouchPro \r\n")));
		LaunchTouchPro();
	}
	EndRender();
}

void CPanelTouch::Render()
{
	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_604):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

	DrawRect(pTexBack,0,0,g_iClientWidth-4,g_iClientHeight-4,2,&cr,TRUE);
#if 0
    if(m_iFactoryTestCmd == FACTORY_CMD_TEST_START)
    {
        switch(m_iTestState)
        {
            case touchteststatebutton:
                m_btnTestP1.SetAlpha(m_fCurAlpha);
                m_btnTestP1.Render();

                m_btnTestP2.SetAlpha(m_fCurAlpha);
                m_btnTestP2.Render();

                m_btnTestP3.SetAlpha(m_fCurAlpha);
                m_btnTestP3.Render();

                m_btnTestP4.SetAlpha(m_fCurAlpha);
                m_btnTestP4.Render();

                m_btnTestP5.SetAlpha(m_fCurAlpha);
                m_btnTestP5.Render();
                break;
//	            case touchteststatefreedraw:
//	        		if(!m_bAppLaunched)
//	        		{
//	        			if(!LaunchTouchPro())
//	        			{
//	                        RETAILMSG(1, (TEXT("MSG:LaunchTouchPro: failed.\r\n")));
//	        			    SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);//quit
//	        			}
//	        			else
//	        			{
//	        				m_pSkinManager->GetGUI()->EnableSwap(FALSE);
//	        			}
//
//	        			m_bAppLaunched=TRUE;
//	        		}
//	                break;
        }
    }
	else
	{
	    RETAILMSG(1,(_T("CPanelTouch::Render: m_iFactoryTestCmd:%d m_iTestState:%d\r\n"),m_iFactoryTestCmd,m_iTestState));
	}
#endif
	EndRender();
}

BOOL CPanelTouch:: Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
    g_pCPanelTouch = this;

#if 0
	SIZE sizebtn={64,64};
    COLORGL cr_dark={0.0,0.0,0.0,1.0};

    m_btnTestP1.Initialize(IDC_TOUCH_TEST_POINT1,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTestP1.SetSize(&sizebtn);
	m_btnTestP1.SetPos(400-64,240-64);
    m_btnTestP1.SetCaption(_T("1"));
    m_btnTestP1.LinkTextGL(pTextGL);
    m_btnTestP1.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnTestP1.SetCaptionHeight(16);
	m_btnTestP1.SetNotifyFunc(&CPanelTouch::OnNotify,this);
    m_btnTestP1.SetCaptionPos(BTN_CAPTION_CENTER);
    m_btnTestP1.Show(FALSE);

    m_btnTestP2.Initialize(IDC_TOUCH_TEST_POINT2,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTestP2.SetSize(&sizebtn);
	m_btnTestP2.SetPos(-400+64,240-64);
    m_btnTestP2.SetCaption(_T("2"));
    m_btnTestP2.LinkTextGL(pTextGL);
    m_btnTestP2.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnTestP2.SetCaptionHeight(16);
	m_btnTestP2.SetNotifyFunc(&CPanelTouch::OnNotify,this);
    m_btnTestP2.SetCaptionPos(BTN_CAPTION_CENTER);
    m_btnTestP2.Show(FALSE);

    m_btnTestP3.Initialize(IDC_TOUCH_TEST_POINT3,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTestP3.SetSize(&sizebtn);
	m_btnTestP3.SetPos(-400+64,-240+64);
    m_btnTestP3.SetCaption(_T("3"));
    m_btnTestP3.LinkTextGL(pTextGL);
    m_btnTestP3.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnTestP3.SetCaptionHeight(16);
	m_btnTestP3.SetNotifyFunc(&CPanelTouch::OnNotify,this);
    m_btnTestP3.SetCaptionPos(BTN_CAPTION_CENTER);
    m_btnTestP3.Show(FALSE);

    m_btnTestP4.Initialize(IDC_TOUCH_TEST_POINT4,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTestP4.SetSize(&sizebtn);
	m_btnTestP4.SetPos(400-64,-240+64);
    m_btnTestP4.SetCaption(_T("4"));
    m_btnTestP4.LinkTextGL(pTextGL);
    m_btnTestP4.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnTestP4.SetCaptionHeight(16);
	m_btnTestP4.SetNotifyFunc(&CPanelTouch::OnNotify,this);
    m_btnTestP4.SetCaptionPos(BTN_CAPTION_CENTER);
    m_btnTestP4.Show(FALSE);

    m_btnTestP5.Initialize(IDC_TOUCH_TEST_POINT5,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTestP5.SetSize(&sizebtn);
	m_btnTestP5.SetPos(0,0);
    m_btnTestP5.SetCaption(_T("5"));
    m_btnTestP5.LinkTextGL(pTextGL);
    m_btnTestP5.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnTestP5.SetCaptionHeight(16);
	m_btnTestP5.SetNotifyFunc(&CPanelTouch::OnNotify,this);
    m_btnTestP5.SetCaptionPos(BTN_CAPTION_CENTER);
    m_btnTestP5.Show(FALSE);
#endif
	return TRUE;
}

void CALLBACK CPanelTouch::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelTouch *panel=(CPanelTouch*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelTouch::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelTouch::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));

	switch (idControl)
	{
        case IDC_TOUCH_TEST_POINT1:
            m_btnTestP1.SetStatus(BS_FOCUS);
            m_btnTestP1.Enable(FALSE);
            m_btnTestP2.Show(TRUE);
            break;

        case IDC_TOUCH_TEST_POINT2:
            m_btnTestP2.SetStatus(BS_FOCUS);
            m_btnTestP2.Enable(FALSE);
            m_btnTestP3.Show(TRUE);
            break;

        case IDC_TOUCH_TEST_POINT3:
            m_btnTestP3.SetStatus(BS_FOCUS);
            m_btnTestP3.Enable(FALSE);
            m_btnTestP4.Show(TRUE);
            break;

        case IDC_TOUCH_TEST_POINT4:
            m_btnTestP4.SetStatus(BS_FOCUS);
            m_btnTestP4.Enable(FALSE);
            m_btnTestP5.Show(TRUE);
            break;

        case IDC_TOUCH_TEST_POINT5:
            m_btnTestP5.SetStatus(BS_FOCUS);
            m_btnTestP5.Enable(FALSE);
            m_iTestState = touchteststatefreedraw;
            Sleep(500);
            m_btnTestP1.Show(FALSE);
            m_btnTestP2.Show(FALSE);
            m_btnTestP3.Show(FALSE);
            m_btnTestP4.Show(FALSE);
            m_btnTestP5.Show(FALSE);
            Sleep(500);
            LaunchTouchPro();
            //ShowWindow(m_hTouchWnd,SW_SHOW);
            //UpdateWindow(m_hTouchWnd);
            break;

    }
}

//	void CPanelTouch::OnTouchDown(POINT *pt)
//	{
//		CPanelGL::OnTouchDown(pt);
//
//	    m_Point.x = pt->x;
//	    m_Point.y = pt->y;
//	}
//
//	void CPanelTouch::OnTouchUp(POINT *pt)
//	{
//		CPanelGL::OnTouchUp(pt);
//
//	    m_Point.x = 0;
//	    m_Point.y = 0;
//	}
//
//	void CPanelTouch::OnTouchMove(POINT *pt)
//	{
//		CPanelGL::OnTouchMove(pt);
//
//	    m_Point.x = pt->x;
//	    m_Point.y = pt->y;
//
//		if(!BeginRender())return;
//
//		COLORGL cr_gray={0.1,0.1,0.1,m_fCurAlpha};
//	    DrawRect(m_Point.x,m_Point.y,3,3,&cr_gray);
//
//		EndRender();
//	    RETAILMSG(1, (TEXT("MSG:[CPanelTouch::OnTouchMove]: x:%d y:%d\r\n"),m_Point.x,m_Point.y));
//
//	}

void CPanelTouch::OnFactoryCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType = DWORD(wParam);
	DWORD dwCmd = DWORD(lParam);

    RETAILMSG(1, (TEXT("MSG:[CPanelTouch::OnFactoryCmd]: dwType:%d dwCmd:%d\r\n"),dwType,dwCmd));

    switch(dwType)
    {
        case FACTORY_CMD_TOUCH_TEST:
            m_iFactoryTestType = dwType;
            m_iFactoryTestCmd = dwCmd;
            if(m_iFactoryTestCmd == FACTORY_CMD_TEST_START && m_iTestState == touchteststateidle)
            {
                RETAILMSG(1, (TEXT("MSG:[CPanelTouch::OnFactoryCmd]: change m_iTestState to touchteststatebutton\r\n")));
                m_iTestState = touchteststatebutton;
                m_btnTestP1.Show(TRUE);
                m_btnTestP1.Enable(TRUE);
            }
            break;
        default:
            break;
    }

}

BOOL CPanelTouch::CreateTouchWindow(LPTSTR szWindowClass)
{
	WNDCLASS wc = {0};
	memset (&wc, 0, sizeof(wc));

	// Register  window class
	wc.style            =  0;//CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc      = TouchWndProc;
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = m_moduleEntry.hInstance;
	wc.hIcon            = NULL;
	wc.hCursor          = NULL;
	wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wc.lpszMenuName     = NULL;
	wc.lpszClassName    = szWindowClass;

	RegisterClass(&wc) ;

	// Create Main Window.placeholder. Not visible by default
    m_hTouchWnd = CreateWindow(
        szWindowClass,           //类名，要和刚才注册的一致
        L"Touch Wnd",  //窗口标题文字
        WS_VISIBLE | WS_VISIBLE, //窗口外观样式
        0,                 //窗口相对于父级的X坐标
        0,                 //窗口相对于父级的Y坐标
        800,                //窗口的宽度
        480,                //窗口的高度
        NULL,               //没有父窗口，为NULL
        NULL,	//LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MAIN)),               //菜单
        m_moduleEntry.hInstance,          //当前应用程序的实例句柄
        NULL);

    if(m_hTouchWnd == NULL)
    {
        RETAILMSG(1, (TEXT("MSG:[CPanelTouch::CreateTouchWindow]: CreateWindow[%s] fialed.\r\n"),szWindowClass));
        return FALSE;
    }

	ShowTouchWindow(TRUE);

    UpdateWindow(m_hTouchWnd);

//	    MSG msg;
//	    while(GetMessage(&msg, NULL, 0, 0))
//	    {
//	        TranslateMessage(&msg);
//	        DispatchMessage(&msg);
//	    }
    return TRUE;
}

LRESULT CALLBACK CPanelTouch::TouchWndProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    //RETAILMSG(1, (TEXT("MSG:[CPanelTouch:TouchWndProc]: uMsg:%d\r\n"),uMsg));
	POINT pt;
    switch(uMsg)
    {
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			{
                RETAILMSG(1, (TEXT("MSG:[CPanelTouch:TouchWndProc]: Paint\r\n")));
                static HBRUSH brushbk;
                RECT rc;
                brushbk = CreateSolidBrush(RGB(255,255,255));
                GetClientRect(hwnd, &rc);
                ::FillRect(GetDC(hwnd),&rc, brushbk);
            }
			EndPaint(hwnd, &ps);
			//UpdateWindow(hwnd);
		}
		return 0;
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
	case WM_MOUSEMOVE:
		{
			if(g_pCPanelTouch->m_bDraw)
			{
				//HDC         hDC;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
//					//g_bDown = TRUE;
//					//pix(hwnd,hDC,NewBrush,Ps);
//					hDC=GetDC(hwnd);
//					SetPixel(hDC,pt.x,pt.y,RGB(255,0,0));
//					ReleaseDC(hwnd,hDC);

				CWnd *pcwnd = CWnd::FromHandle(hwnd);
                CClientDC dc(pcwnd);
                dc.MoveTo(g_pCPanelTouch->m_ptOrigin);
                dc.LineTo(pt);
                g_pCPanelTouch->m_ptOrigin = pt;
                //RETAILMSG(1, (TEXT("MSG:CPanelTouch:Move:%d,%d\r\n"),pt.x,pt.y));
			}
			return 0;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			//HDC         hDC;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			//g_bDown = TRUE;
			//pix(hwnd,hDC,NewBrush,Ps);
//				hDC=GetDC(hwnd);
//				SetPixel(hDC,pt.x,pt.y,RGB(255,0,0));
//				ReleaseDC(hwnd,hDC);

			CWnd *pcwnd = CWnd::FromHandle(hwnd);
            CClientDC dc(pcwnd);

            g_pCPanelTouch->m_bDraw = TRUE;
            g_pCPanelTouch->m_ptOrigin = pt;
            //RETAILMSG(1, (TEXT("MSG:CPanelTouch:Down:%d,%d\r\n"),pt.x,pt.y));
			return 0;
		}
		break;
	case WM_LBUTTONUP:
		{
			g_pCPanelTouch->m_bDraw = FALSE;
            //RETAILMSG(1, (TEXT("MSG:CPanelTouch:Up\r\n")));
			return 0;
		}
		break;
	//case WM_ERASEBK:
	//	{
	//		RECT rc;
	//		static HBRUSH br=CreateSolidBrush(RGB(255,255,255));
	//		GetClientRect(hwnd,&rc);
	//		::FillRect(GetDC(hwnd), &rc, br);
	//		return 0;
	//	}
	//	break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CPanelTouch::LaunchTouchPro()
{
	if(IsWindow(m_hTouchWnd))
	{
		BOOL bRet=FALSE;
		RETAILMSG(1/*DEBUG_PANELAPP*/, (TEXT("MSG:[CPanelTouch::LaunchTouchPro]: Window exist,%x,show window\r\n"),m_hTouchWnd));

		//
		ShowWindow(g_hMainWnd,SW_HIDE);

        ShowTouchWindow(TRUE);

		Sleep(10);

		return TRUE;
	}
	else
	{
#if 1
        return CreateTouchWindow(TEXT("TouchWindow"));
#else

		PROCESS_INFORMATION pi;
		if(CreateProcess(_T("\\USB Disk\\TPTest.exe"),NULL,NULL,NULL,0,0,NULL,NULL,NULL,&pi))//set correct location:
		{
//				EnumParam ep;
//				ep.hMainWnd=NULL;
//				ep.dwProcessID   =   pi.dwProcessId;
//
//				int count=0;

			//m_hAppProcess=pi.hProcess;

			//g_dwNaviProcessID = pi.dwProcessId;

			RETAILMSG(1/*DEBUG_PANELAPP*/,(_T("MSG:[CPanelTouch::LaunchTouchPro]: CreateProcess succeed:%s.hProcess:%x,dwProcessId:%x\r\n"),_T("\\USB Disk\\TPTest.exe"),pi.hProcess, pi.dwProcessId));

			Sleep(500);

//				while(ep.hMainWnd==NULL && count<10)
//				{
//					EnumWindows((WNDENUMPROC)EnumWinProc,   (long)&ep);
//					count++;
//					if(ep.hMainWnd==NULL)
//					{
//						DWORD code=WaitForSingleObject(m_hAppProcess,500);
//
//						RETAILMSG(DEBUG_PANELAPP,(_T("MSG:[CPanelTouch::LaunchTouchPro]: WaitForSingleObject m_hAppProcess:%d\r\n"),code));
//
//						Sleep(500);
//
//						RETAILMSG(DEBUG_PANELAPP,(_T("ERR:[CPanelTouch::LaunchTouchPro]: EnumWindows failed,next.\r\n")));
//					}
//					else
//					{
//						RETAILMSG(DEBUG_PANELAPP,(_T("MSG:[CPanelTouch::LaunchTouchPro]: EnumWindows SUCCEED!. 0x%x\r\n"),ep.hMainWnd));
//						break;
//					}
//				}

//				m_hTouchWnd=ep.hMainWnd;

			Sleep(100);
			ShowWindow(g_hMainWnd,SW_HIDE);
			Sleep(100);
			ShowWindow(m_hTouchWnd,SW_SHOW);
			SetForegroundWindow((HWND)(((ULONG) m_hTouchWnd) | 0x01) );
			SetForegroundWindow(m_hTouchWnd);

			//			return TRUE;
		}
		else
		{
			RETAILMSG(1,(_T("ERR:[CPanelTouch::TouchWndProc]: CreateProcess failed:%s,next.\r\n"),_T("\\USB Disk\\TPTest.exe")));

			m_hTouchWnd=NULL;
			//m_hAppProcess=NULL;
			return FALSE;
		}
#endif
	}
	//Sleep(1000);

}


void CPanelTouch::ShowTouchWindow(BOOL bShow)
{
	BOOL bRet = FALSE;

	if(IsWindow(m_hTouchWnd))
	{
		if(bShow)
		{
			bRet = ::SetWindowPos(m_hTouchWnd, HWND_TOPMOST , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
			RETAILMSG(1, (TEXT("MSG:[CPanelTouch::ShowWindow]: SetWindowPos:%d Return[%d]\r\n"),bShow, bRet));
			if(bRet)
			{
				m_bTouchWindowVisible = TRUE;
			}
			else
			{
				m_bTouchWindowVisible = FALSE;
			}
			::ShowWindow(m_hTouchWnd,SW_SHOW);
			SetForegroundWindow(m_hTouchWnd);
		}
		else
		{
	        ::ShowWindow(m_hTouchWnd,SW_HIDE);
			bRet = ::SetWindowPos(m_hTouchWnd, HWND_BOTTOM , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW );
			RETAILMSG(1, (TEXT("MSG:[CPanelTouch::ShowWindow]: SetWindowPos:%d Return[%d]\r\n"),bShow, bRet));

			if(bRet)
			{
				m_bTouchWindowVisible = FALSE;
			}
			else
			{
				m_bTouchWindowVisible = TRUE;
			}

		}
	}
}
