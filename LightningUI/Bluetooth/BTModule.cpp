// BT.cpp : Defines the class behaviors for the application.
//

#include "../stdafx.h"
#include <bt_sfwe_api.h>

HINSTANCE v_hBtpUIExtDll = NULL;
p_BluetoothNotificationResolveString		g_BluetoothNotificationResolveString = NULL;
p_BluetoothNotificationResolveValue		g_BluetoothNotificationResolveValue = NULL;

void GetBTMode(LPTSTR lpsz)
{
	HKEY hk = NULL;

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, BT_SETTING_REG_KEY, 0, KEY_READ, &hk))
	{
		DWORD dwSize = MAX_BTMODE_STRING;
		DWORD dwType = 0;

		if ((ERROR_SUCCESS != RegQueryValueEx(hk, L"BTMode", NULL, &dwType, (BYTE *)lpsz, &dwSize)) ||
			(dwType != REG_SZ) || (dwSize > MAX_BTMODE_STRING))
		{
			SetBTMode(BTMODE_DISCOVERABLE_STRING);
			_tcscpy(lpsz, BTMODE_DISCOVERABLE_STRING);
		}

		RegCloseKey(hk);
	}
	else
	{
		SetBTMode(BTMODE_DISCOVERABLE_STRING);
		_tcscpy(lpsz, BTMODE_DISCOVERABLE_STRING);
	}
}

void SetBTMode(LPTSTR lpsz)
{
	HKEY hk = NULL;
	DWORD dwDisp;

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, BT_SETTING_REG_KEY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hk, &dwDisp))
	{
		RegSetValueEx(hk, L"BTMode", 0, REG_SZ, (BYTE *)lpsz, (_tcslen(lpsz) + 1) * sizeof(TCHAR));

		RegCloseKey(hk);
		RegFlushKey(HKEY_LOCAL_MACHINE);
	}
	else
	{
		BTDBG(ZONE_ERROR, (TEXT("SetBTMode ERROR \r\n")));
	}
}


// CBTModule construction
CBTModule::CBTModule()
:hWndHidden(NULL),
pApi(NULL),
pDataPool(NULL),
m_bIni(FALSE)
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}
CBTModule::~CBTModule()
{
	if(m_bIni)
	{
		 DeInitialize();
	}
}

// The one and only CBTModule object
CBTModule g_BTModule;

// CBTModule initialization
UINT g_uiSignalNotifyMessage;

BOOL CBTModule::IsInitialized()
{
	return m_bIni;
}

BOOL CBTModule::Initialize(HWND hWnd)
{
    //DebugInitialize();
    BTDBG(ZONE_DEBUGONLY, (TEXT("CBTModule::Initialize+\r\n")));

	hWndHidden = hWnd;

    //[DebugOnly] Set start point to calculate BT APP launching time.
//     DWORD timeInMilliSec = GetTickCount();

    //register SN message
    g_uiSignalNotifyMessage = RegisterWindowMessage(BLUETOOTH_NOTIFICATION_SIGNAL_WINDOW_TEXT);

#ifdef DYNAMIC_WPP_LIB
    TCHAR 	m_szLibName[MAX_PATH] = {0};
    DWORD   dwSize = sizeof(m_szLibName);
    HKEY 	hk;
    wcscpy_s(m_szLibName, sizeof(m_szLibName) / sizeof(m_szLibName[0]), BTP_UI_EXT_DLL_NAME);

    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, WPP_LIB_PATH, 0, 0, &hk))
    {
        BTDBG(ZONE_ERROR, (TEXT("CBTModule::: Fail to open registry key \"%s\"\r\n"), hk));
    }
    else
    {
        dwSize = sizeof(m_szLibName);

        if (ERROR_SUCCESS != RegQueryValueEx(hk, WPP_LIB_NAME, NULL, NULL, (LPBYTE)m_szLibName, &dwSize))
        {
            BTDBG(ZONE_ERROR, (TEXT("CBTModule::: Read %s from registry failed. Use default value %s\r\n"), WPP_LIB_NAME, m_szLibName));
        }

        RegCloseKey(hk);
    }

    BTDBG(ZONE_THREAD, (TEXT("CBTModule:: WPP library name %s\r\n"), m_szLibName));

    if (v_hBtpUIExtDll == NULL)
        v_hBtpUIExtDll = LoadLibrary(m_szLibName);
#else

    if (v_hBtpUIExtDll == NULL)
        v_hBtpUIExtDll = LoadLibrary(L"btp_ui_ext.dll");

#endif

    if (g_BluetoothNotificationResolveString == NULL)
        g_BluetoothNotificationResolveString = (p_BluetoothNotificationResolveString)GetProcAddress(v_hBtpUIExtDll, L"BluetoothNotificationResolveString");

    if (g_BluetoothNotificationResolveValue == NULL)
        g_BluetoothNotificationResolveValue = (p_BluetoothNotificationResolveValue)GetProcAddress(v_hBtpUIExtDll, L"BluetoothNotificationResolveValue");

    if (g_BluetoothNotificationResolveString == NULL || g_BluetoothNotificationResolveValue == NULL)
    {
        BTDBG(ZONE_ERROR, (TEXT("CBTModule::WndProc p_BluetoothNotificationResolveString||g_BluetoothNotificationResolveValue == NULL \r\n")));
		return 0;
    }
	
    if ((pDataPool = new CDataPool) == NULL || !pDataPool->Initialize())
    {
        BTDBG(ZONE_ERROR, (TEXT("CBTModule::Initialize ERROR!\r\n")));
        return FALSE;
    }

// 	BTDBG(1/*ZONE_INIT*/, (TEXT("@@@@@CBTModule::Initialize -  pDataPool initialize takes total time: %ums\r\n"), GetTickCount() - timeInMilliSec));

//     if ((pMmi = new CMMI) == NULL || !pMmi->Initialize())	//will get data when initializing
//     {
//         BTDBG(ZONE_ERROR, (TEXT("CBTModule::Initialize ERROR!\r\n")));
//         return FALSE;
//     }
// 

	//timeInMilliSec = GetTickCount();
    if ((pApi = new CAPI) == NULL || !pApi->Initialize())	//will enum device and add data which will notify mmi
    {
        BTDBG(ZONE_ERROR, (TEXT("CBTModule::Initialize ERROR!\r\n")));
        return FALSE;
    }
	
// 	BTDBG(1/*ZONE_INIT*/, (TEXT("@@@@@CBTModule::Initialize -  pApi initialize takes total time: %ums\r\n"),GetTickCount() - timeInMilliSec));

//     if ((pLogic = new CLogic) == NULL || !pLogic->Initialize())	//will call api,get/set bt state which will notify mmi
//     {
//         BTDBG(ZONE_ERROR, (TEXT("CBTModule::Initialize ERROR!\r\n")));
//         return FALSE;
//     }
// 

	m_bIni = TRUE;

    return TRUE;
}

void CBTModule::DeInitialize()
{
    if (v_hBtpUIExtDll != NULL)
    {
        FreeLibrary(v_hBtpUIExtDll);
        v_hBtpUIExtDll = NULL;
    }

    if (pApi != NULL)	//thread my call logic
    {
        pApi->DeInitialize();
        delete pApi;
        pApi = NULL;
    }

//     if (pLogic != NULL)
//     {
//         pLogic->DeInitialize();
//         delete pLogic;
//         pLogic = NULL;
//     }
// 
//     if (pMmi != NULL)
//     {
//         pMmi->DeInitialize();
//         delete pMmi;
//         pMmi = NULL;
//     }

    if (pDataPool != NULL)
    {
        pDataPool->DeInitialize();
        delete pDataPool;
        pDataPool = NULL;
    }

//     if (hMutex != NULL)
//     {
//         ReleaseMutex(hMutex);
// 		CloseHandle(hMutex);
//         hMutex = NULL;
//     }

    BTDBG(ZONE_DEINIT, (TEXT("CBTModule::DeInitialize-\r\n")));
   // DebugUninitialize();
}



