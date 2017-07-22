#pragma once

#include <csync.h>


#define WPP_DEFAULT_DLL_NAME    TEXT("btp_ui_ext.dll")

#define CHECK_FUNC_PTR(pfn)     { if (!(pfn)) return ERROR_INVALID_FUNCTION; }


typedef unsigned short BluetoothStatusT;
#define BLUETOOTH_STATUS_IDLE           ((BluetoothStatusT) 0x0)
#define BLUETOOTH_STATUS_CONNECTING     ((BluetoothStatusT) 0x1)
#define BLUETOOTH_STATUS_CONNECTED      ((BluetoothStatusT) 0x2)
#define BLUETOOTH_STATUS_DISCONNECTING  ((BluetoothStatusT) 0x3)


class CApi_base: public CLockObject
{
public:
    CApi_base(void): m_name(0), m_hThread(NULL), m_bExitThread(FALSE), m_connectionStatus(BLUETOOTH_STATUS_IDLE)
    {
        m_hDll = LoadLibrary(WPP_DEFAULT_DLL_NAME);
    }
    /* Define the virtual de-constructor to free SfWE SDK("btp_ui_ext.dll") commonly. Otherwise, memory leak occurs. */
    virtual ~CApi_base(void)
    {
        if (m_hDll)
        {
            FreeLibrary(m_hDll);
        }
    }

    virtual BOOL Initialize();
    virtual void DeInitialize();
    virtual void PowerDown() {}
    virtual void PowerUp() {}

    virtual BOOL InitFuncPtr() { return FALSE; }
    virtual void DumpFuncPtr() {}

    virtual WORD GetConnectionStatus();
    virtual void SetConnectionStatus(LPTSTR status);
    virtual BOOL IsSameConnectionStatus(LPTSTR status);

    DWORD m_name;
    HANDLE m_hThread;
    BOOL m_bExitThread;

    HINSTANCE m_hDll;

private:
    BluetoothStatusT m_connectionStatus;
};
