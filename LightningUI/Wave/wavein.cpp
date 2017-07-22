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
#include "wavein.hpp"

VOID CALLBACK
CWaveIn::waveInProc(
    HWAVEIN     hwi,
    UINT        uMsg,
    DWORD       dwInstance,
    DWORD       dwParam1,
    DWORD       dwParam2 )
{
    LPWAVEHDR   pwh = ( LPWAVEHDR )dwParam1;
    CWaveIn*     pwi = ( CWaveIn * )dwInstance;

    switch( uMsg )
    {
    case WIM_DATA:
        if( !pwh )
        {
            
        }
        else if( !pwi->OnDataReady( pwh ) )
        {
            
		}
        break;

    case WIM_OPEN:
		break;
    case WIM_CLOSE:
		{

		}
		break;
    default:
        break;
    }
}

BOOL CWaveIn::Start( WAVEFORMATEX* pwfx )
{
    MMRESULT mr;
    DWORD dwIndex;
    const DWORD cbwh = sizeof( WAVEHDR );

    do
    {

        mr = ::waveInOpen( &m_hwi, m_id, pwfx, ( DWORD )waveInProc, ( DWORD )this, CALLBACK_FUNCTION );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInOpen" ), mr );
            break;
        }

        for( dwIndex = 0; dwIndex < dwMaxBuf; ++dwIndex )
        {
            memset( &m_whi[ dwIndex ], 0, cbwh );
            INIT_WAVEHDR( &m_whi[ dwIndex ], m_wfi[ dwIndex ].pData, m_wfi[ dwIndex ].cbData );

            mr = ::waveInPrepareHeader( m_hwi, &m_whi[ dwIndex ], cbwh );
            if( MMSYSERR_NOERROR != mr )
            {
                ReportError( TEXT( "waveInPrepareHeader" ), mr );
                break;
            }

            mr = ::waveInAddBuffer( m_hwi, &m_whi[ dwIndex ], cbwh );
            if( MMSYSERR_NOERROR != mr )
            {
                ReportError( TEXT( "waveInAddBuffer" ), mr );
                break;
            }

        }

        if( dwIndex < dwMaxBuf )
        {
            break;
        }

		//select input:
// 		waveInMessage(m_hwi,/*WIDM_OPEN_AUDIO_PHYSICAL_INPUT*/(WM_USER + 7),0,0);

        mr = ::waveInStart( m_hwi );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInStart" ), mr );
            break;
        }

		m_bRunning = TRUE;

        return TRUE;
    } while( 0 );

    return FALSE;
}
BOOL CWaveIn::PreStop()
{
	if(m_bRunning)
	{
		LONG    semCount;

		m_bRunning = FALSE;

		if( !::ReleaseSemaphore( m_sem, 1, &semCount ) )
		{
			RETAILMSG( 1, ( TEXT( "CWaveIn::PreStop ReleaseSemaphore() FAILED, Line %d, semCount: %d\r\n" ), __LINE__ ,semCount) );
			return FALSE;
		}
	}

	return TRUE;
}
BOOL CWaveIn::OnDataReady( LPWAVEHDR pwh )
{
	
	if(m_bRunning)
	{
		LONG    semCount;

		m_queue.PutBuf( pwh );
		if( !::ReleaseSemaphore( m_sem, 1, &semCount ) )
		{
			RETAILMSG( 1, ( TEXT( "ReleaseSemaphore() FAILED, errno %d,semCount:%d\r\n" ), GetLastError() , semCount) );
			return FALSE;
		}
	}
	
    return TRUE;
}

LPWAVEHDR CWaveIn::Wait( DWORD dwTimeout )
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
            //RETAILMSG( 1, ( TEXT( "CWaveIn::GetBuf failed 0, Line = %d\r\n" ), __LINE__ ) );
        }
        else if( ( pwh->dwFlags & WHDR_INQUEUE ) )
        {
            RETAILMSG( 1, ( TEXT( "CWaveIn::Wait, pwh->dwFlags = 0x%X\r\n" ), pwh->dwFlags ) );
		}
    }
	else
	{
		RETAILMSG( 1, (TEXT("%d buffers in wavein queue\r\n" ), m_queue.GetNumItems()));
	}

    return pwh;
}

BOOL CWaveIn::Continue( LPWAVEHDR pwh )
{
    const DWORD cbwh = sizeof( WAVEHDR );

    do
    {
        MMRESULT mr;

        mr = ::waveInUnprepareHeader( m_hwi, pwh, cbwh );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInUnprepareHeader" ), mr );
            break;
        }

        mr = ::waveInPrepareHeader( m_hwi, pwh, cbwh );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInPrepareHeader" ), mr );
            break;
        }

        mr = ::waveInAddBuffer( m_hwi, pwh, cbwh );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInAddBuffer" ), mr );
            break;
        }

        return TRUE;
    } while( 0 );

    return FALSE;
}

BOOL CWaveIn::Stop()
{
    do
    {
		m_bRunning = FALSE;

        MMRESULT mr = ::waveInStop( m_hwi );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInStop" ), mr );
            break;
        }

        mr = ::waveInReset( m_hwi );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInReset" ), mr );
            break;
        }

		m_queue.Clear();

//         if( !m_queue.IsFull() )
//         {
//             Sleep( 2000 );
//         }

        mr = ::waveInClose( m_hwi );
        if( MMSYSERR_NOERROR != mr )
        {
            ReportError( TEXT( "waveInClose" ), mr );
            break;
        }

		m_hwi = NULL;

        return TRUE;
    } while( 0 );

    return FALSE;
}
DWORD CWaveIn::GetGain()
{
	DWORD dwVolume = 0;

	MMDRV_MESSAGE_PARAMS Params;
	BOOL bRet;
	Params.uMsg = WIDM_GETVOLUME;
	Params.dwParam1 = (DWORD) & dwVolume;
	Params.dwUser = 0; // Select Device rather than stream
	Params.uDeviceId = 0; // Select Device 0

	HANDLE hWAV=NULL;

	hWAV = CreateFile(TEXT("WAV1:"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (INVALID_HANDLE_VALUE == hWAV)
	{
		RETAILMSG(1, (TEXT("Fail to open wav driver\r\n")));
		return 0;
	}

	bRet = DeviceIoControl(hWAV, IOCTL_WAV_MESSAGE, &Params, sizeof(MMDRV_MESSAGE_PARAMS), 0, 0, NULL, NULL);

	if (FALSE == bRet)
	{
		RETAILMSG(1, (TEXT("GetAudioMicVolume Failed\r\n")));
		return 0;
	}

// 	RETAILMSG(1, (TEXT("--->  GetAudioMicVolume volume=(0x%x)\r\n"), dwVolume));

	return dwVolume;
}
void CWaveIn::SetGain(DWORD dwGain)
{
	MMDRV_MESSAGE_PARAMS Params;
	BOOL bRet;
	Params.uMsg = WIDM_SETVOLUME;
	Params.dwParam1 = dwGain;//15* VOLUME_ONE_STEP;
	Params.dwUser = 0; // Select Device rather than stream
	Params.uDeviceId = 0; // Select Device 0

// 	RETAILMSG(1, (TEXT("--->  SetAudioMicVolume  volume=(0x%x)\r\n"), Params.dwParam1));

	HANDLE hWAV=NULL;

	hWAV = CreateFile(TEXT("WAV1:"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (INVALID_HANDLE_VALUE == hWAV)
	{
		RETAILMSG(1, (TEXT("Fail to open wav driver\r\n")));
		//return ;
	}

	bRet = DeviceIoControl(hWAV, IOCTL_WAV_MESSAGE, &Params, sizeof(MMDRV_MESSAGE_PARAMS), 0, 0, NULL, NULL);

	if (FALSE == bRet)
	{
		RETAILMSG(1, (TEXT("SetAudioMicVolume Failed\r\n")));
	}

	CloseHandle(hWAV);
}