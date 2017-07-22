#pragma once

#include <windows.h>

//comment to disable heap statistics
//#define ENABLE_HEAP_STATISTICS

#ifdef ENABLE_HEAP_STATISTICS
//class statistics
#define HEAP_STATISTICS_CLASS_ALLOC(theClass) g_heapStatistics.HeapStatisticClassAlloc(L#theClass, sizeof(theClass));
#define HEAP_STATISTICS_CLASS_FREE(theClass) g_heapStatistics.HeapStatisticClassFree(L#theClass, sizeof(theClass));
//general varibales statistics
#define HEAP_STATISTICS_VAR_ALLOC(varName, cbSize) g_heapStatistics.HeapStatisticVarAlloc(L#varName, cbSize);
#define HEAP_STATISTICS_VAR_FREE(varName, cbSize) g_heapStatistics.HeapStatisticVarFree(L#varName, cbSize);
//print all statistics
#define HEAP_STATISTICS_PRINT() g_heapStatistics.HeapStatisticPrint(NULL);
#else
#define HEAP_STATISTICS_CLASS_ALLOC(theClass)
#define HEAP_STATISTICS_CLASS_FREE(theClass)
#define HEAP_STATISTICS_VAR_ALLOC(varName, cbSize)
#define HEAP_STATISTICS_VAR_FREE(varName, cbSize)
#define HEAP_STATISTICS_PRINT()
#endif

#define MAX_CLASS_NAME 30
struct CHeapOfClass
{
    CHeapOfClass *m_pNext;

    TCHAR m_szClassName[MAX_CLASS_NAME];
    DWORD m_cbSize;
    DWORD m_dwCntAlloc;
    DWORD m_dwCntFree;
    DWORD m_dwCntExist;
    DWORD m_cbTotalHeap;

    CHeapOfClass(BOOL bHeapSta = TRUE);
    CHeapOfClass(LPCTSTR szClass, DWORD cbSize, BOOL bHeapSta = TRUE);
    ~CHeapOfClass();
};

class CHeapStatistics
{
public:

    CHeapOfClass *m_pHead;
    DWORD m_dwCntList;
    CRITICAL_SECTION m_cs;

    CHeapStatistics(): m_pHead(NULL), m_dwCntList(0)
    {
        InitializeCriticalSection(&m_cs);
        InitializeCHeapOfClass();
    }
    ~CHeapStatistics()
    {
        Clear();
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

    //class statistics
    void HeapStatisticClassAlloc(LPCTSTR szClass, DWORD cbSize);
    void HeapStatisticClassFree(LPCTSTR szClass, DWORD cbSize);
    //general varibales statistics
    void HeapStatisticVarAlloc(LPCTSTR szClass, DWORD cbSize);
    void HeapStatisticVarFree(LPCTSTR szClass, DWORD cbSize);
    //print all statistics
    void HeapStatisticPrint(LPCTSTR szClass);

protected:
    BOOL InitializeCHeapOfClass();
    void Clear()
    {
        CHeapOfClass *pCur = NULL;

        Lock();

        for (pCur = m_pHead; pCur != NULL; pCur = m_pHead)
        {
            m_pHead = m_pHead->m_pNext;
            delete pCur;
            m_dwCntList--;
        }

        Unlock();
    }
    CHeapOfClass *Find(LPCTSTR szClass);
    CHeapOfClass *Add(LPCTSTR szClass, DWORD cbSize);
};

extern CHeapStatistics g_heapStatistics;
