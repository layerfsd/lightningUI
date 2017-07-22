/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/
#if !defined( _QUEUE_H )
#define _QUEUE_H

class QueueEx
{
    CRITICAL_SECTION    m_csLock;
    LPVOID              *m_lpBuff;
    DWORD               m_dwHead;
    DWORD               m_dwTail;
	DWORD               m_dwMaxBuf;

protected:
    VOID Lock() { ::EnterCriticalSection( &m_csLock ); }
    VOID Unlock() { ::LeaveCriticalSection( &m_csLock ); }

public:
    QueueEx()
        : m_dwHead( 0 )
        , m_dwTail( 0 )
    {
        //memset( m_lpBuff, 0, sizeof m_lpBuff );
		m_lpBuff = NULL;
        ::InitializeCriticalSection( &m_csLock );
    }

    ~QueueEx()
    {
        ::DeleteCriticalSection( &m_csLock );
		if(m_lpBuff) free(m_lpBuff);
    }

	void QueueInit(DWORD dwMaxBuf)
	{
		m_dwMaxBuf = dwMaxBuf;
		m_lpBuff = (LPVOID *) malloc(sizeof(LPVOID)*dwMaxBuf);
		memset(m_lpBuff,0,sizeof(LPVOID)*dwMaxBuf);
	}

	void Clear(){if(m_lpBuff)memset(m_lpBuff,0,sizeof(LPVOID)*m_dwMaxBuf);}
    BOOL IsFull() { return m_lpBuff[ m_dwTail ] != NULL; }
    BOOL IsEmpty() { return m_lpBuff[ m_dwHead ] == NULL; }
    DWORD GetNumItems()
    {
        if( IsFull() ) return m_dwMaxBuf;
        if( m_dwTail >= m_dwHead ) return m_dwTail - m_dwHead;
        return m_dwTail - m_dwHead + m_dwMaxBuf;
    }

    LPVOID GetBuf()
    {
        LPVOID  lpItem = NULL;

        Lock();

        if( IsEmpty() )
        {
            
        }
        else
        {
			//RETAILMSG( 1, ( TEXT( "GetBuf head:%d,tail:%d,size:%d\r\n" ),m_dwHead, m_dwTail,(LPWAVEHDR(m_lpBuff[ m_dwHead ])->dwBytesRecorded)) );

			lpItem = m_lpBuff[ m_dwHead ];
            m_lpBuff[ m_dwHead ] = NULL;
            m_dwHead = ( m_dwMaxBuf == m_dwHead + 1 ? 0 : m_dwHead + 1 );
        }

        Unlock();

        return lpItem;
    }

    VOID PutBuf( LPVOID lpItem )
    {
        if( NULL == lpItem )
        {
            RETAILMSG( 1, ( TEXT( "Invalid data, PutBuf() FAILED\r\n" ) ) );
            return;
        }

        Lock();

        if( IsFull() )
        {
            RETAILMSG( 1, ( TEXT( "Queue full, PutBuf() FAILED\r\n" ) ) );
        }
        else
        {
            m_lpBuff[ m_dwTail ] = lpItem;
            m_dwTail = ( m_dwMaxBuf == m_dwTail + 1 ? 0 : m_dwTail + 1 );

			//RETAILMSG( 1, ( TEXT( "PutBuf head:%d,tail:%d\r\n" ),m_dwHead, m_dwTail) );
        }

        Unlock();
    }
};

#endif  /* !defined( _QUEUE_H ) */
