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
#if !defined( _WAVE_IN_H )
#define _WAVE_IN_H

// #define MAX_WAVEIN_BUF 32

#include "wave.h"

#include <wavedev.h>
#define WIDM_GETVOLUME			63
#define WIDM_SETVOLUME			64
#define VOLUME_ONE_STEP			(0xFFFFFFFF/15)
#define MAX_GAIN						(0xFFFFFFFF)

class CWaveIn
{
    DWORD       m_id;
    HWAVEIN     m_hwi;
    HANDLE      m_sem;
    WAVEHDR     *m_whi;
    WAVEFRAME  *m_wfi;
	DWORD       dwMaxBuf;
    QueueEx       m_queue;

	BOOL			m_bRunning;

public:

protected:
    BOOL OnDataReady( LPWAVEHDR pwh );

public:
    CWaveIn( DWORD id = 0, DWORD dwBufferSize = 512, DWORD dwNumBuffers = 0)
        : m_id( id )
		, dwMaxBuf( dwNumBuffers )
    {
		m_hwi=NULL;
        m_sem = ::CreateSemaphore( NULL, 0, dwNumBuffers, NULL );
		m_whi = (WAVEHDR *)malloc(sizeof(WAVEHDR)*dwNumBuffers);
		m_wfi = (WAVEFRAME *)malloc(sizeof(WAVEFRAME)*dwNumBuffers);
		memset(m_whi,0,sizeof(WAVEHDR)*dwNumBuffers);
		memset(m_wfi,0,sizeof(WAVEFRAME)*dwNumBuffers);

		for (DWORD i=0;i<dwMaxBuf;i++)
		{
			m_wfi[i].pData=(BYTE*)malloc(dwBufferSize);
			m_wfi[i].cbData=dwBufferSize;
		}

		m_queue.QueueInit(dwNumBuffers);
		m_bRunning = FALSE;
    }

    ~CWaveIn()
    {
		if(m_wfi)
		{
			for (DWORD i=0;i<dwMaxBuf;i++)
			{
				if(m_wfi[i].pData)
				{
					free(m_wfi[i].pData);
				}
			}
		}

        ::CloseHandle( m_sem );
		if(m_whi) free(m_whi);
		if(m_wfi) free(m_wfi);
    }

    BOOL Start( WAVEFORMATEX* pwfx );
    BOOL Stop();
	BOOL PreStop();
	void SetGain(DWORD dwGain);
	DWORD GetGain();

    BOOL Continue( LPWAVEHDR pwh );
    LPWAVEHDR Wait( DWORD dwTimeout );

    static VOID CALLBACK
    waveInProc(
            HWAVEIN     hwi,
            UINT        uMsg,
            DWORD       dwInstance,
            DWORD       dwParam1,
            DWORD       dwParam2 );
};

#endif  /* !defined( _WAVE_IN_H ) */
