/*************************************************************

                 CSR plc 2011

                 All rights reserved

FILE:            waveout.hpp

DESCRIPTION:     Header for the utility of waveout

REVISION:        $Revision: #1 $

************************************************************/
#ifndef _WAVE_OUT_H
#define _WAVE_OUT_H

#include "a2dp_sink.h"


class WaveOut
{
private:
    DWORD       m_id;
    HWAVEOUT    m_hwo;
    WAVEHDR    *m_pwh;
    DWORD       m_count; /* WAVEHDR count */

public:
    WaveOut(DWORD id = WAVE_MAPPER)
        : m_id(id)
        , m_hwo(NULL)
        , m_pwh(NULL)
        , m_count(0)
    {
        RETAILMSG(DEBUG_A2DP_INIT, (_T("<WaveOut::WaveOut> Id: 0x%x\r\n"), m_id));
        Init();
    }

    ~WaveOut()
    {
        Fini();
    }

    BOOL Init();
    void Fini();

    BOOL Open(WAVEFORMATEX *pwfx, DWORD dwCallback, DWORD fdwOpen, WAVEHDR *pwh, DWORD count);
    void Close();

    BOOL Start();
    BOOL Stop();

    BOOL Continue(LPWAVEHDR pwh);

    BOOL Mute();
    BOOL UnMute();

    static HWAVEOUT    s_hwo;
    static BOOL PutVolume(DWORD dwVolume);
    static DWORD	s_dwA2dpVolume;
    static BOOL s_bMute;
};

#endif  /* _WAVE_OUT_H */
