#pragma once
#include "api_base.h"
#include <ras.h>
#include "Api_SearchPair.h"

#define DUN_STATUS_CHECK_TIMEOUT			(3*1000)

struct DUNNETWORK
{
    DUNNETWORK *pNext;

    TCHAR szName[DUNNETWORK_NAME_STRING_MAXLEN];	//unique
    TCHAR szAPN[DUNNETWORK_NAME_STRING_MAXLEN];
    TCHAR szDialNumber[DUNNETWORK_NAME_STRING_MAXLEN];
    TCHAR szUsername[DUNNETWORK_NAME_STRING_MAXLEN];
    TCHAR szPassword[DUNNETWORK_NAME_STRING_MAXLEN];

    DUNNETWORK()
    {
        HEAP_STATISTICS_CLASS_ALLOC(DUNNETWORK)

        memset(this, 0, sizeof(*this));
    }
    DUNNETWORK(DUNNETWORK *pdn)
    {
        HEAP_STATISTICS_CLASS_ALLOC(DUNNETWORK)

        *this = *pdn;
    }
    ~DUNNETWORK()
    {
        HEAP_STATISTICS_CLASS_FREE(DUNNETWORK)
    }
};

struct CDUNNetworkList
{
    DUNNETWORK *m_pdnHead;
    DWORD m_dwCntList;
    CRITICAL_SECTION m_cs;

    CDUNNetworkList(): m_pdnHead(NULL), m_dwCntList(0)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CDUNNetworkList)

        InitializeCriticalSection(&m_cs);
    }
    ~CDUNNetworkList()
    {
        HEAP_STATISTICS_CLASS_FREE(CDUNNetworkList)

        Free();
        DeleteCriticalSection(&m_cs);
    }
    void Lock() const
    {
        EnterCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }
    void Unlock() const
    {
        LeaveCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }

    //NOTE:default use the parameter pointer directly, don't free it in caller
    DUNNETWORK *Add(DUNNETWORK *pdn, BOOL bNew = FALSE);
    BOOL Remove(DUNNETWORK *pdn, BOOL bDelete = TRUE);
    void Free();
    DUNNETWORK *Find(DUNNETWORK *pdn) const;
    DUNNETWORK *Find(TCHAR *pszName) const;
};

class CApi_DUN :
    public CApi_base
{
public:
    CApi_DUN(void);
    ~CApi_DUN(void);

    virtual BOOL Initialize();
    virtual void DeInitialize();

    //dun networks
    //stored in HLM\\software\\microsoft\\bluetooth\\device\\DUNNetworks, key is szName string
    BOOL EnumDUNNetworks();
    BOOL EnumUpdate(DUNNETWORK *pdn);
    void EnumRemove(DUNNETWORK *pdn);

    DWORD m_dwThreadID;
    HANDLE m_hExit;    
    HRASCONN m_hRasConn;
	
	BTDEV *m_pbtdConnecting;
    DUNNETWORK *m_pdnConnceting;
	
	//connect and disconnect
	int DUNConnect(BTDEV *pbtd, DUNNETWORK *pdn);
	int DUNDisconnect(BTDEV *pbtd);

    void DUNConnectCancel();    
    int GetConnectedDev(BT_ADDR *b);

    BOOL IsDUNDisconnect(BTDEV *pbtd);

    //dial up thread
	HANDLE m_hThreadDialup;
	DWORD Thread_run_DUNDialup();
    static DWORD DUNDialup_thread(CApi_DUN *pApiDun)
    {
        return pApiDun->Thread_run_DUNDialup();
    }

	//status check thread
	HANDLE m_hThreadStatusCheck;
	DWORD Thread_check_DUNStatus();
    static DWORD DunStatusCheck_Thread(CApi_DUN *pApiDun)
    {
        return pApiDun->Thread_check_DUNStatus();
    }
    
	//lock
    CRITICAL_SECTION m_cs;
    void Lock() const
    {
        EnterCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }
    void Unlock() const
    {
        LeaveCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }


};
