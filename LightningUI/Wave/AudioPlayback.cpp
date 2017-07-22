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
#include "stdafx.h"
#include "windev.h"
#include "wavein.hpp"
#include "waveout.hpp"

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

//****************************************************************
// Globals
//****************************************************************
BOOL        g_bStop = FALSE;
DWORD gdwMaxBuffers = 32, gdwDuration = 600;

unsigned int gau32TxRxCount[2];

void AUDIO_InitWaveFormatEx(WAVEFORMATEX *pwfx)
{
    pwfx->wFormatTag = WAVE_FORMAT_PCM;
    pwfx->nChannels  = 1;
    pwfx->nSamplesPerSec = 44100;
    pwfx->wBitsPerSample = 16;
    pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
    pwfx->nAvgBytesPerSec = pwfx->nSamplesPerSec * pwfx->nBlockAlign;
    pwfx->cbSize = 0;
}

BOOL CopyData(WaveIn& wi, WaveOut& wo, uint8 u8Dir)
{
    LPWAVEHDR whi = 0, who = 0;
	static uint32 au32Now[2], au32Prev[2];
	uint32 u32Diff;

	gau32TxRxCount[u8Dir]++;

    whi = wi.Wait(INFINITE);
    if (!whi)
    {       
        return FALSE;
    }

	who  = wo.Wait(INFINITE);
    if (!who)
    {       
        return FALSE;
    }

	memcpy(who->lpData, whi->lpData, whi->dwBytesRecorded);

	wo.Continue( who );
    wi.Continue( whi );

	if(gau32TxRxCount[u8Dir] == 250)
	{
		au32Now[u8Dir]  = GetTickCount();
		u32Diff = au32Now[u8Dir] - au32Prev[u8Dir];
		RETAILMSG(1, (TEXT("%s: TotCbTime=%d, for %d packets of buffer len:%d. 0x%x 0x%x\r\n"), 
			(u8Dir?"Tx":"Rx"), u32Diff, gau32TxRxCount[u8Dir], whi->dwBytesRecorded,
			(uint8)whi->lpData[0], (uint8)whi->lpData[1]));
		gau32TxRxCount[u8Dir] = 0;
		au32Prev[u8Dir]         = au32Now[u8Dir];
	}

    return TRUE;
}

DWORD WINAPI
AudioRxThread(
    LPVOID  pContext )
{
    int u32In;
    int iCnt = 0;

    u32In = ::waveInGetNumDevs();

    RETAILMSG( 1, (TEXT("AudioRxThread(): num input devices: %d \r\n"), u32In));

    static WaveIn  linein( 0, gdwMaxBuffers);
	static WaveOut spkr( 0, gdwMaxBuffers);
    
    WAVEFORMATEX    wfx;
    AUDIO_InitWaveFormatEx(&wfx);

	if (!spkr.Start( &wfx ) )
    {
		RETAILMSG(1, (TEXT("ERROR --- AudioRxThread(): spkr start failed \r\n")));
        return -1;
    }

    if (!linein.Start( &wfx ) )
    {
		RETAILMSG(1, (TEXT("ERROR --- AudioRxThread(): linein start failed \r\n")));
        return -1;
    }

    while( !g_bStop )
    {
		if(!CopyData(linein, spkr, 1)) 
        {
            RETAILMSG(1, (TEXT("AudioRxThread(): ReadData returned FALSE \r\n")));
            break;
        }
    }

    RETAILMSG(1, (TEXT("AudioRxThread(): stopping!!!\r\n")));

    linein.Stop();
	spkr.Stop();

    return 0;
}

DWORD WINAPI
AudioTxThread(
    LPVOID  pContext )
{
    int u32Out;
    int iCnt = 0;

    u32Out = ::waveOutGetNumDevs();

    RETAILMSG( 1, (TEXT("AudioTxThread(): num output devices: %d \r\n"), u32Out));

    static WaveOut lineout( 1, gdwMaxBuffers);
		static WaveIn  mic( 1, gdwMaxBuffers);	
    
    WAVEFORMATEX    wfx;
    AUDIO_InitWaveFormatEx(&wfx);

    if (!lineout.Start( &wfx ) )
    {
		RETAILMSG(1, (TEXT("ERROR --- AudioTxThread(): lineout start failed \r\n")));
        return -1;
    }

	if (!mic.Start( &wfx ) )
    {
		RETAILMSG(1, (TEXT("ERROR --- AudioTxThread(): mic start failed \r\n")));
        return -1;
    }

    while( !g_bStop )
    {
		if(!CopyData(mic, lineout, 0)) 
        {
            RETAILMSG(1, (TEXT("AudioTxThread(): WriteData returned FALSE \r\n")));
            break;
        }
		//Sleep(1);
    }

    RETAILMSG(1, (TEXT("AudioTxThread(): stopping!!!\r\n")));

	mic.Stop();
    lineout.Stop();

    return 0;
}

#define WAVPROXY_REG_PATH _T("\\Drivers\\WAVPROXY")

int _tmain(int argc, TCHAR* argv[])
{

	for(int i = 0; i<argc; i++)
	{
		RETAILMSG(1, (TEXT("param %d: %s\r\n"), i, argv[i]));
	}

	if(argc >= 2)
	{
		_stscanf(argv[1], _T("%d"), &gdwMaxBuffers);
	}

	if(argc >= 3)
	{
		_stscanf(argv[2], _T("%d"), &gdwDuration);
	}
	
	RETAILMSG(1, (TEXT(" Max Buffers = %d \r\n"), gdwMaxBuffers));
	RETAILMSG(1, (TEXT(" Duration = %d seconds\r\n"), gdwDuration));
#if 0
    hWav = ActivateDeviceEx(WAVPROXY_REG_PATH, NULL, 0, NULL);
    if (!hWav)
    {
        RETAILMSG(1, (TEXT("Could not load wavproxy, reg path %s, error %d\r\n"), 
                  WAVPROXY_REG_PATH, GetLastError()));
        //return -1;
    }
#endif

    Sleep(500);

	HANDLE hRx = CreateThread(NULL, 0, AudioRxThread, NULL, 0, NULL);
	HANDLE hTx = CreateThread(NULL, 0, AudioTxThread, NULL, 0, NULL);

	Sleep(gdwDuration*1000);

	g_bStop = TRUE;

	/* Sleep for 1 second, so that the threads exit */
	Sleep(1000);

	CloseHandle(hRx);
	CloseHandle(hTx);

#if 0
	if(hWav)
		DeactivateDevice(hWav);
#endif

	RETAILMSG(1, (TEXT(" [%s] Quitting Main \r\n"), argv[0]));

	return 0;
}
