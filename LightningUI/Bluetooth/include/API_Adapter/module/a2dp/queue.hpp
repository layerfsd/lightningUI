/*************************************************************

                 CSR plc 2011

                 All rights reserved

FILE:            queue.hpp

DESCRIPTION:     Header for the utility of queue

REVISION:        $Revision: #1 $

************************************************************/
#ifndef _QUEUE_H
#define _QUEUE_H

#include <windows.h>
#include "a2dp_sink.h"

class Queue
{
    CRITICAL_SECTION    m_csLock;
    LPVOID              *m_lpBuff;
    DWORD               m_dwHead;
    DWORD               m_dwTail;
    DWORD               m_dwMaxBuf;

protected:
    VOID Lock()     { ::EnterCriticalSection( &m_csLock ); }
    VOID Unlock()   { ::LeaveCriticalSection( &m_csLock ); }

public:
    Queue()
    : m_dwHead(0)
    , m_dwTail(0)
    , m_dwMaxBuf(0)
    , m_lpBuff(NULL)
    {
        ::InitializeCriticalSection(&m_csLock);
    }

    ~Queue()
    {
        ::DeleteCriticalSection(&m_csLock);
        if (m_lpBuff) ::LocalFree(m_lpBuff);
    }

    BOOL Init(DWORD dwMaxBuf)
    {
        m_dwMaxBuf = dwMaxBuf;
        
        m_lpBuff = (LPVOID *)::LocalAlloc(LPTR, sizeof(LPVOID) * m_dwMaxBuf);
        if (!m_lpBuff)
        {
            return FALSE;
        }

        return TRUE;
    }

    void Deinit()
    {
        m_dwHead = 0;

        m_dwTail = 0;

        m_dwMaxBuf = 0;

        if (m_lpBuff != NULL) 
        {
            ::LocalFree(m_lpBuff);
            m_lpBuff = NULL;
        }
    }

    BOOL IsFull()   { return m_lpBuff[m_dwTail] != NULL; }
    BOOL IsEmpty()  { return m_lpBuff[m_dwHead] == NULL; }
    DWORD GetNumItems()
    {
        if (IsFull()) 
            return m_dwMaxBuf;

        if (m_dwTail >= m_dwHead) 
            return m_dwTail - m_dwHead;

        return m_dwTail - m_dwHead + m_dwMaxBuf;
    }

    LPVOID GetBuf()
    {
        LPVOID  lpItem = NULL;

        if (!m_lpBuff)
        {
            return NULL;
        }

        Lock();

        if (IsEmpty())
        {
            RETAILMSG(DEBUG_A2DP_WARN, (_T("<Queue::GetBuf> Fail due to queue empty\r\n")));
        }
        else
        {
            lpItem = m_lpBuff[m_dwHead];
            m_lpBuff[m_dwHead] = NULL;
            m_dwHead = (m_dwMaxBuf == m_dwHead + 1 ? 0 : m_dwHead + 1);
        }

        Unlock();

        return lpItem;
    }

    void PutBuf(LPVOID lpItem)
    {
        if (NULL == lpItem || !m_lpBuff)
        {
            RETAILMSG(DEBUG_A2DP_WARN, (_T("<Queue::PutBuf> Invalid data item\r\n")));
            return;
        }

        Lock();

        if (IsFull())
        {
            RETAILMSG(DEBUG_A2DP_WARN, (_T("<Queue::PutBuf> Fail due to queue full\r\n")));
        }
        else
        {
            m_lpBuff[m_dwTail] = lpItem;
            m_dwTail = (m_dwMaxBuf == m_dwTail + 1 ? 0 : m_dwTail + 1);
        }

        Unlock();
    }
};

#endif  /* _QUEUE_H */
