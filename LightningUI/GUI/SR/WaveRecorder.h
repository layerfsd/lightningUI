/*
 *	模块: Pingbu's Windows Wave Recorder
 *	版本: 1.0.110613
 *	作者: pingbu@iflytek.com
 *
 *	版权: 您可以任意使用和分发本模块，但作任何修改将导致不得再冠以本模块的名称，
 *	      否则将视为对作者的侵权。
 */

#pragma once

#include <assert.h>
#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>

#ifdef _WIN32_WCE
	#pragma comment(lib, "coredll")
#else
	#pragma comment(lib, "winmm")
#endif

#define WM_MY_MESSAGE_CLOSEPCM (WM_USER + 1)

#define REC_BUF_COUNT	4
#define REC_BUF_SIZE	8192

struct IWaveRecordListener
{
	virtual void InWaveRecordData(int nID, const void* pData, int cbSize) = 0;
};

class CWaveRecorder
{
public:
    HWND m_hWnd;

private:
#ifdef _DEBUG
	DWORD m_dwThreadID;
#endif

	IWaveRecordListener* m_pListener;
	int m_nID;
	HWAVEIN m_hWaveIn;
	WAVEHDR m_tWaveHdr[REC_BUF_COUNT];
	char m_pBuffer[REC_BUF_COUNT][REC_BUF_SIZE];
	int m_nBusyBuf;

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == MM_WIM_DATA)
		{
			CWaveRecorder* pThis = (CWaveRecorder*)GetWindowLong(hWnd, GWL_USERDATA);
			if (pThis->m_nBusyBuf)
			{
				--pThis->m_nBusyBuf;
			}
			else
			{
				LPWAVEHDR lpwvhdr = (LPWAVEHDR)lParam;
				pThis->m_pListener->InWaveRecordData(pThis->m_nID, lpwvhdr->lpData, lpwvhdr->dwBytesRecorded);
				waveInAddBuffer(pThis->m_hWaveIn, lpwvhdr, sizeof(WAVEHDR));
			}
			return 0;
        } else if(uMsg == WM_MY_MESSAGE_CLOSEPCM){
            CWaveRecorder* pThis = (CWaveRecorder*)GetWindowLong(hWnd, GWL_USERDATA);
            pThis->Close();
            return 0;
        }

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

public:
	CWaveRecorder(IWaveRecordListener* pListener, HMODULE hModule = NULL, int nID = 0)
	{
#ifdef _DEBUG
		m_dwThreadID = GetCurrentThreadId();
#endif

		assert(pListener);
		m_pListener = pListener;
		m_nID = nID;

		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.lpszClassName = _T("WaveRecorder");
		wc.hInstance = hModule ? hModule : GetModuleHandle(NULL);
		wc.lpfnWndProc = __WndProc;
		RegisterClass(&wc);
		TCHAR szWndName[32] = {0};
		_stprintf(szWndName, TEXT("%d"), nID);

		m_hWnd = CreateWindow(wc.lpszClassName, szWndName, WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
		assert(m_hWnd);
		SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

		m_hWaveIn = NULL;
	}

	~CWaveRecorder()
	{
		assert(GetCurrentThreadId() == m_dwThreadID);
		assert(!m_hWaveIn);
		DestroyWindow(m_hWnd);
	}

	bool Open(LPWAVEFORMATEX pwfx, int dvc)
	{
		assert(GetCurrentThreadId() == m_dwThreadID);
		assert(!m_hWaveIn);

		waveInOpen(&m_hWaveIn, (DWORD)dvc, pwfx, (DWORD)m_hWnd, (DWORD)this, CALLBACK_WINDOW);
        // waveInOpen(&m_hWaveIn, 1, pwfx, (DWORD)m_hWnd, (DWORD)this, CALLBACK_WINDOW);
		if (!m_hWaveIn)
			return false;

		ZeroMemory(m_tWaveHdr, sizeof(m_tWaveHdr));
		for (int i = 0; i < REC_BUF_COUNT; ++i)
		{
			m_tWaveHdr[i].lpData = m_pBuffer[i];
			m_tWaveHdr[i].dwBufferLength = REC_BUF_SIZE;
			waveInPrepareHeader(m_hWaveIn, m_tWaveHdr + i, sizeof(WAVEHDR));
			waveInAddBuffer(m_hWaveIn, m_tWaveHdr + i, sizeof(WAVEHDR));
		}

		m_nBusyBuf = 0;
		waveInStart(m_hWaveIn);
		return true;
	}

	bool OpenPCM(DWORD nSamplesPerSec, int dvc, WORD nChannels = 1, WORD wBitsPerSample = 16)
	{
		WAVEFORMATEX wfx;
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		wfx.cbSize = 0;
		wfx.nSamplesPerSec = nSamplesPerSec;
		wfx.nChannels = nChannels;
		wfx.wBitsPerSample = wBitsPerSample;
		wfx.nBlockAlign = (wBitsPerSample + 7) / 8 * nChannels;
		wfx.nAvgBytesPerSec = wfx.nBlockAlign * nSamplesPerSec;
		return Open(&wfx, dvc);
	}

	void Close()
	{
		assert(GetCurrentThreadId() == m_dwThreadID);

		if (m_hWaveIn)
		{
			m_nBusyBuf = REC_BUF_COUNT;

			waveInReset(m_hWaveIn);

			while (m_nBusyBuf)
			{
				MSG msg;
				GetMessage(&msg, m_hWnd, MM_WIM_DATA, MM_WIM_DATA);
				DispatchMessage(&msg);
			}

			for (int i = 0; i < REC_BUF_COUNT; ++i)
				waveInUnprepareHeader(m_hWaveIn, m_tWaveHdr + i, sizeof(WAVEHDR));
			
			waveInClose(m_hWaveIn);
			m_hWaveIn = NULL;
		}
	}
};
