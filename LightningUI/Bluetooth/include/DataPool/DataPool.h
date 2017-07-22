#pragma once

#include "../../StdAfx.h"
#include <queue>

typedef void (*pfnObserverNotify)(DWORD dataName, void *pBuf, DWORD dwSize);

#define STATE_STRING_MAXLEN 40	//Change For 3way talking. 20 -> 40
#define MAX_CALLINFO 200
#define MAX_CONNECTED_DEVICE 7
#define MAX_PIN_LEN 16
#define ERROR_STATUS 10

//data change mode when notifing observers
enum
{
    CM_SET,
    CM_ADD,		//following modes are just for DT_LIST, add to tail
    CM_REMOVE,	//remove a node
    CM_CLEAR,	//clear a list
    CM_UPDATE,	//update a list node
};

//
//DataPool layer
//DataPool is used to split data-view, and keep confromance of the data in all views
//When data is changed, it will notify all data hosts which are also called observers
//Some global variables would  be placed here
//

class CDataPool
{
public:
    enum  	//dataType
    {
        DT_VALUE = 0,
        DT_ARRAY,
        DT_LIST,		//m_pBuf would point to a list class
    };

    struct CData
    {
        DWORD m_dataName;
        DWORD m_dataType;	//
        void *m_pBuf;
        DWORD m_cbSize;
        DWORD *m_pObserversName;	//observers, now just mmi.dlgs
        DWORD m_dwCntObservers;
        CRITICAL_SECTION m_cs;

        CData(DWORD dataName, DWORD dataType, DWORD cbSize, DWORD *pObserversName, DWORD dwCntObervers): m_pBuf(NULL),
            m_dataName(dataName), m_dataType(dataType), m_cbSize(cbSize), m_pObserversName(pObserversName), m_dwCntObservers(dwCntObervers)
        {
            HEAP_STATISTICS_CLASS_ALLOC(CData)

            InitializeCriticalSection(&m_cs);
        }
        ~CData()
        {
            HEAP_STATISTICS_CLASS_FREE(CData)

            DeleteCriticalSection(&m_cs);
        }
        void Lock()
        {
            EnterCriticalSection(&m_cs);
        }
        void Unlock()
        {
            LeaveCriticalSection(&m_cs);
        }

    };

    enum
    {
#define INSERT_DATA(name, dataClass, dataType, cntArray)  name,
#include "Data_table.h"
        CNT_DATA,
    };

    CData **m_ppData;
    DWORD m_dwCntData;

    CDataPool(void);
    ~CDataPool(void);

    CData *Data(DWORD dataName)
    {
        ASSERT(dataName < m_dwCntData);
        return m_ppData[dataName];
    }

    //offer proctection after data got
    void LockData(DWORD dataName)
    {
        Data(dataName)->Lock();
    }

    void UnlockData(DWORD dataName)
    {
        Data(dataName)->Unlock();
    }

    //set the value of *m_pBuf
    BOOL SetData(DWORD dataName, void *pBuf, DWORD dwSize);
    //get the pointer:m_pBuf, don't modify its contents!!
    void GetData(DWORD dataName, const void **ppBuf);

    //functions below are just for DT_LIST
    //add to list, now just add without new a node
    BOOL AddData(DWORD dataName, void *pBuf, DWORD dwSize);
    //remove a node
    BOOL RemoveData(DWORD dataName, void *pBuf, DWORD dwSize, BOOL bDelete = TRUE);
    //update a node, now just send notifications
    BOOL UpdateData(DWORD dataName, void *pBuf, DWORD dwSize);
    //clear a list
    void ClearData(DWORD dataName);

    BOOL Initialize();
    void DeInitialize();

protected:
    //free m_pBuf
    void FreeData(DWORD dataName);
};

//global variables that don't need notifications
extern BYTE g_byaDefaultPIN[MAX_PIN_LEN + 1];	//default pin, write by UI, read by UI and API
extern BTDEV *g_pbtdConnected[MAX_CONNECTED_DEVICE];		//connected devices when BTState==Connected, write by FSM, read by FSM and API
extern DWORD g_dwCntConnectedDev;
extern DWORD g_dwTalkingTime;		//talking time, write by UI, read by UI
extern BTDEV *g_pbtdSel;			//selected dun dev and network, write by UI, read by UI and FSM
extern DUNNETWORK *g_pdnSel;
extern DUNNETWORK *g_pdnEdit;		//dun network which would be edit or new(NULL), wirte by UI ,read by UI an FSM
extern DWORD g_dwTalkingTime1;
extern DWORD g_dwTalkingTime2;
extern DWORD g_dwHangupCall;
extern BTDEV *g_pbtdL1;
extern BTDEV *g_pbtdL2;
extern BTDEV *g_pbtdR1;
extern BTDEV *g_pbtdR2;
extern DWORD g_dwBtdSeleted1;
extern DWORD g_dwBtdSeleted2;
extern BTDEV *g_pbtdCurrent;
extern DWORD g_dwSelectDeviceType;
extern Message *g_pmsgCurrent;
extern Message* g_pmsgNew;
extern DWORD g_dwFSMCurrentStateName;
extern BYTE g_bFolder;
extern Message *g_pmsgDel;
extern int g_iTpmsPair;
extern BT_ADDR g_bHFConnecting;
extern BT_ADDR g_bA2DPConnecting;
extern BT_ADDR g_bAVRCPConnecting;
extern BT_ADDR *g_bTPMSConnecting;
extern BT_ADDR g_bSPPConnecting;
extern BT_ADDR g_bMAPConnecting;
extern BT_ADDR g_bPBAPConnecting;
extern BTDEV *g_pbtdConnectedOne;
extern BTDEV *g_pbtdSelPBAP;
extern std::queue<AvrcpMediaAttrQueueMemberT *> g_AVRCPMusicListSelectedItemAttrQueue;
