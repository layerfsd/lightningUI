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
#include "wave.h"
#include "waveout.hpp"

VOID CALLBACK
CWaveOut::waveOutProc(
    HWAVEOUT    hwo,
    UINT        uMsg,
    DWORD       dwInstance,
    DWORD       dwParam1,
    DWORD       dwParam2 )
{
    LPWAVEHDR   pwh = ( LPWAVEHDR )dwParam1;
    CWaveOut*    pwo = ( CWaveOut * )dwInstance;

    switch( uMsg )
    {
    case WOM_DONE:
		RETAILMSG(0, (TEXT("waveOut: Got wom_done msg with pwh = 0x%x\r\n"), pwh));
        if( !pwh )
        {
           
        }
        else if( !pwo->OnDataDone( pwh ) )
        {
            
		}
        break;
    case WOM_OPEN:
    case WOM_CLOSE:
    default:
        break;
    }
}

BOOL CWaveOut::Start( WAVEFORMATEX* pwfx )
{
    MMRESULT mr;
    const DWORD cbwh = sizeof( WAVEHDR );

    do
    {
        mr = ::waveOutOpen( &m_hwo, m_id, pwfx, ( DWORD )waveOutProc, ( DWORD )this, CALLBACK_FUNCTION );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveOutOpen" ), mr );
            break;
        }

        for( DWORD dwIndex = 0; dwIndex < m_dwMaxBuf; ++dwIndex )
        {
            memset( &m_who[ dwIndex ], 0, cbwh );
            INIT_WAVEHDR( &m_who[ dwIndex ], m_wfo[ dwIndex ].pData, m_wfo[ dwIndex ].cbData );

			//set user data:
			m_who[ dwIndex ].dwUser = m_dwUser;

            mr = ::waveOutPrepareHeader( m_hwo, &m_who[ dwIndex ], cbwh );
            if( MMSYSERR_NOERROR != mr )
            {
                ReportError( TEXT( "waveOutPrepareHeader" ), mr );
                break;
            }
            m_queue.PutBuf( &m_who[ dwIndex ] );
        }

		m_bRunning=TRUE;

        return TRUE;
    } while( 0 );

    return FALSE;
}
BOOL CWaveOut::PreStop()
{
	if(m_bRunning)
	{
		LONG    semCount;

		m_bRunning = FALSE;

		if( !::ReleaseSemaphore( m_sem, 1, &semCount ) )
		{
			RETAILMSG( 1, ( TEXT( "CWaveOut::PreStop ReleaseSemaphore() FAILED, Line %d, semCount: %d\r\n" ), __LINE__ ,semCount) );
			return FALSE;
		}
	}

	return TRUE;
}

void CWaveOut::SetVolume(DWORD dwVol)
{
	if(m_hwo)
	{
		::waveOutSetVolume(m_hwo,dwVol);
	}
}
BOOL CWaveOut::OnDataDone( LPWAVEHDR pwh )
{

	if(m_bRunning)
	{
		LONG    semCount;
		m_queue.PutBuf( pwh );
		if( !::ReleaseSemaphore( m_sem, 1, &semCount ) )
		{
			RETAILMSG( 1, ( TEXT( "CWaveOut::OnDataDone ReleaseSemaphore() FAILED, Line %d, semCount: %d\r\n" ), __LINE__ ,semCount) );
			return FALSE;
		}
	}

    return TRUE;
}

LPWAVEHDR CWaveOut::Wait( DWORD dwTimeout )
{
    LPWAVEHDR   pwh = NULL;

    if( WAIT_OBJECT_0 == ::WaitForSingleObject( m_sem, dwTimeout ) )
    {
		if(!m_bRunning)
		{
			return NULL;
		}

        pwh = ( LPWAVEHDR )m_queue.GetBuf();

        if( !pwh )
        {
            RETAILMSG( 1, ( TEXT( "GetBuf failed Line = %d\r\n" ), __LINE__ ) );
        }
        else if( ( pwh->dwFlags & WHDR_INQUEUE ) )
        {
            RETAILMSG( 1, ( TEXT( "pwh->dwFlags = 0x%X\r\n" ), pwh->dwFlags ) );
		}
    }
	else
	{
		RETAILMSG( 1, (TEXT("%d buffers in waveout queue\r\n" ), m_queue.GetNumItems()));
	}

    return pwh;
}

BOOL CWaveOut::Continue( LPWAVEHDR pwh )
{
    const DWORD cbwh = sizeof( WAVEHDR );

    do
    {
        MMRESULT mr;

        mr = ::waveOutUnprepareHeader( m_hwo, pwh, cbwh );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveOutUnprepareHeader" ), mr );
            break;
        }

        mr = ::waveOutPrepareHeader( m_hwo, pwh, cbwh );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveOutPrepareHeader" ), mr );
            break;
        }

        mr = ::waveOutWrite( m_hwo, pwh, cbwh );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveOutWrite" ), mr );
            break;
        }

        return TRUE;
    } while( 0 );

    return FALSE;
}
BOOL CWaveOut::Pause()
{
	do
	{
		if(m_bRunning)
		{
			MMRESULT mr = ::waveOutPause( m_hwo );
			if( MMSYSERR_NOERROR != mr )
			{
				ReportError( TEXT( "waveOutPause" ), mr );
				break;
			}
			m_bRunning=FALSE;
			return TRUE;
		}
	} while( 0 );

	return FALSE;
}
BOOL CWaveOut::Run()
{
	do
	{
		if(!m_bRunning)
		{
			MMRESULT mr = ::waveOutRestart( m_hwo );
			if( MMSYSERR_NOERROR != mr )
			{
				ReportError( TEXT( "waveOutRestart" ), mr );
				break;
			}
			m_bRunning=TRUE;
			return TRUE;
		}
	} while( 0 );

	return FALSE;
}
BOOL CWaveOut::Stop()
{
    do
    {
		m_bRunning = FALSE;

        MMRESULT mr = ::waveOutPause( m_hwo );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveOutPause" ), mr );
            break;
        }

        mr = ::waveOutReset( m_hwo );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveOutReset" ), mr );
            break;
        }

		m_queue.Clear();

        mr = ::waveOutClose( m_hwo );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveOutClose" ), mr );
            break;
        }

		m_hwo = NULL;

        return TRUE;
    } while( 0 );

    return FALSE;
}
