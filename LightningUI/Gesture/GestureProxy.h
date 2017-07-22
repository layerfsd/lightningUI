#pragma once

#include "../GUI/Common/ui_common.h"
#include "CVTE_GestureProxy.h"
#if CVTE_EN_LED
#include "../LED/led.h"
#endif

#if (CVTE_EN_GESTURE_AIR == 1)

class CGestureProxy
{
public:
    static CGestureProxy* GetInstance();
    void Initialize(CSkinManagerGL* pSkinManager, HWND hWnd);
    #if CVTE_EN_LED
    void LinkLED(CLED *phLED);
    #endif
    void ChangeRotateThd(UINT16 nRotate);

    void StopGestureDetect();
    BOOL StartGestureDetect();
    void PauseGestureDetect();
    void PlayGestureDetect();

    void GestureIDEnable(BOOL bEnable);
    void GestureCircleEnable(BOOL bEnable);


private:
	CGestureProxy(void);
	~CGestureProxy(void);
    static DWORD WINAPI GestureDetectThread(LPVOID lpvParam);
    static DWORD WINAPI GestureIDThread(LPVOID lpvParam);
    static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
    void GestureDetect();
    void GestureIDProcess();

private:
    HWND m_hMainWnd;
    static const CGestureProxy* m_pInstance;
    PACGestureData_t m_sGesData;
    #if CVTE_EN_LED
    CLED                *m_pLED;
    #endif
    //HANDLE m_hGestureIDThread;
    BOOL    m_bGesWaveFirstDetect;
    BOOL    m_bGestureIDThreadEnd;
    BOOL    m_bGestureIDEnable;
    BOOL    m_bGestureCircleEnable;

    HANDLE m_hGestureDetectEvent;
    HANDLE m_hGestureDetectThread;
    DWORD  m_dwGestureDetectTimeout;
    BOOL   m_bGestureDetectThreadEnd;
    BOOL   m_bGestureCircleDetect;
    BOOL   m_bGestureDetectIsRunning;
    BOOL   m_bGestureIDIsRunning;

    HANDLE     m_hGestProxy;
    UINT16      m_nGesDistanceThd;
    UINT16      m_nGesRotateThd;
    UINT16      m_nGesRotate;
    UINT16      m_nGesRotatePrev;
	BOOL        m_bGesCircleStart;
    BOOL        m_bGesClockCircle;
    BYTE        m_nGesContinueCircleCount;

    BYTE        m_nGestureIDPrev;
    BOOL        m_bGestureIDDoubleCheck;

    DWORD       m_dwGesDetectTick;
    DWORD       m_dwGesIDTick;
    DWORD       m_dwGesInitTick;

    GesObjectInfo_t m_GesObject;
    GesObjectInfo_t m_GesObjectPrev;

};
#endif
