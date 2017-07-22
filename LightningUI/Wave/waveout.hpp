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
#if !defined( _WAVE_OUT_H )
#define _WAVE_OUT_H

#define MAX_WAVEOUT_BUF 32

#include "wave.h"

class CWaveOut
{
    DWORD       m_id;
    HWAVEOUT    m_hwo;
    HANDLE      m_sem;
    WAVEHDR     *m_who;
    WAVEFRAME  *m_wfo;
	DWORD       m_dwMaxBuf;
    QueueEx       m_queue;
	BOOL		m_bRunning;
	DWORD    m_dwUser;

protected:
    BOOL OnDataDone( LPWAVEHDR pwh );

public:
    CWaveOut( DWORD id = 0,  DWORD dwBufferSize = 512, DWORD dwNumBuffers = 0, DWORD dwUser = 0 )
        : m_id( id )
		, m_dwMaxBuf( dwNumBuffers )
		, m_dwUser(dwUser)
    {
		m_hwo=NULL;
		m_bRunning=FALSE;
        m_sem = ::CreateSemaphore( NULL, dwNumBuffers, dwNumBuffers, NULL );
		m_who = (WAVEHDR    *) malloc(sizeof(WAVEHDR)*dwNumBuffers);
		m_wfo = (WAVEFRAME *) malloc(sizeof(WAVEFRAME)*dwNumBuffers);
		memset(m_who,0,sizeof(WAVEHDR)*dwNumBuffers);
		memset(m_wfo,0,sizeof(WAVEFRAME)*dwNumBuffers);

		for (DWORD i=0;i<m_dwMaxBuf;i++)
		{
			m_wfo[i].pData=(BYTE*)malloc(dwBufferSize);
			m_wfo[i].cbData=dwBufferSize;
		}

		m_queue.QueueInit(dwNumBuffers);
    }

    ~CWaveOut()
    {
		if(m_wfo)
		{
			for (DWORD i=0;i<m_dwMaxBuf;i++)
			{
				if(m_wfo[i].pData)
				{
					free(m_wfo[i].pData);
				}
			}
		}

        ::CloseHandle( m_sem );
		if(m_who) free(m_who);
		if(m_wfo) free(m_wfo);
    }

    BOOL Start( WAVEFORMATEX* pwfx );
    BOOL Stop();
	BOOL Pause();
	BOOL Run();
	BOOL isRunning(){return m_bRunning;}
	void SetVolume(DWORD dwVol);
	BOOL PreStop();

    BOOL Continue( LPWAVEHDR pwh );
    LPWAVEHDR Wait( DWORD dwTimeout );

    static VOID CALLBACK
    waveOutProc(
            HWAVEOUT    hwo,
            UINT        uMsg,
            DWORD       dwInstance,
            DWORD       dwParam1,
            DWORD       dwParam2 );
};

#endif  /* !defined( _WAVE_IN_H ) */
