#pragma once
#include "api_base.h"

class CApi_SystemEvent :
    public CApi_base
{
public:
    CApi_SystemEvent(void);
    ~CApi_SystemEvent(void);

    virtual BOOL Initialize();
    virtual void DeInitialize();

    HANDLE m_hMsgQ;
    HANDLE m_hMsgQWrite;	//for notifing thread exit
    HANDLE m_hRequestPN;
	HANDLE m_hAppExitEvent;	// for notifing the threads(m_hThreadGeneralSystemEventHandle) to stop when BTApp exit.
    //PN thread
    static DWORD PowerNotify_thread(CApi_SystemEvent *pApi_SystemEvent)
    {
        return pApi_SystemEvent->Thread_run_PowerNotify();
    }
    DWORD Thread_run_PowerNotify();

    //memory monitor thread
    HANDLE m_hThreadMemoryMonitor;
    static DWORD MemoryMonitor_thread(CApi_SystemEvent *pApi_SystemEvent)
    {
        return pApi_SystemEvent->Thread_run_MemoryMonitor();
    }
    DWORD Thread_run_MemoryMonitor();

	HANDLE m_hThreadGeneralSystemEventHandle;
	static DWORD SystemEventHandle_thread(CApi_SystemEvent *pApi_SystemEvent)
	{
		return pApi_SystemEvent->Thread_run_GeneralSystemEventHandle();
	}
	DWORD Thread_run_GeneralSystemEventHandle();

    //Power management
    /* [SfWE] NO HCI BCSP driver in SfWE. */

    bool SendIOControl(DWORD dwIOControl, WCHAR *szServiceName, void *pInput, DWORD dwInputSize, void *pOutput, DWORD dwOutputSize, DWORD *pdwActualOutputSize, HANDLE hDev = INVALID_HANDLE_VALUE) ;
    bool RefreshBTSatus();
};
