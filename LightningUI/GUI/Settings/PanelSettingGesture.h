#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#include "CVTE_GestureProxy.h"

#if (CVTE_EN_GESTURE_AIR == 1)

class CPanelSettingGesture :
	public CPanelGL
{
public:
	CPanelSettingGesture(void);
	~CPanelSettingGesture(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
    void OnGestureCircle(BOOL bCircleRight);
	void OnGestureID(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
    BOOL UpdateDistance(UINT16 nDistance);
    BOOL UpdateSensitivity(GesDirection_t* pGesDirection);
    BOOL UpdateWave(GesWave_t* pGesWave);
    BOOL UpdateRotate(UINT16 nRotate);
    BOOL UpdateNoMotion(BYTE nNoMotion);
    BOOL UpdateNoObject(BYTE nNoObject);
    void DeinitGestureProcThread();
    BOOL InitGestureProcThread();
    void GestureThreadSwitch(BOOL bOn);
    void GestureDetect();
    static DWORD WINAPI GestureProcThread(LPVOID lpvParam);
    static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);


private:
    HANDLE m_hGestureEvent;
    HANDLE m_hGestureProcThread;
    DWORD  m_dwGestureDetectTimeout;
    BOOL   m_bGestureThreadEnd;
    BOOL   m_bGestureThreadIsRunning;

    HANDLE     m_hGestProxy;
    UINT16      m_nGesDistanceThd;
    UINT16      m_nGesRotateThd;
    UINT16      m_nGesRotate;
    UINT16      m_nGesRotatePrev;
//	    BOOL        m_bGesCircleStart;
    BOOL        m_bGesClockCircle;
    BYTE        m_nGesContinueCircleCount;
    BYTE        m_nNoMotionThd;
    BYTE        m_nNoObjectThd;
    BYTE        m_bCursorTracekEnable;
    GesObjectInfo_t m_GesObject;
    GesObjectInfo_t m_GesObjectPrev;
    GesDirection_t  m_GesDirectionThd;
    GesWave_t       m_GesWaveThd;

private:
	CButtonGL	m_tagTitleOperationDistance;
	CSliderGL	m_sliOperationDistance;

	CButtonGL	m_tagTitleSensitivityX;
	CSliderGL	m_sliSensitivityX;

	CButtonGL	m_tagTitleSensitivityY;
	CSliderGL	m_sliSensitivityY;

	CButtonGL	m_tagTitleSensitivityZ;
	CSliderGL	m_sliSensitivityZ;

	CButtonGL	m_tagTitleRotate;
	CSliderGL	m_sliRotate;

	CButtonGL	m_tagTitleWaveCountThd;
	CSliderGL	m_sliWaveCountThd;

	CButtonGL	m_tagTitleWaveAngleThd;
	CSliderGL	m_sliWaveAngleThd;

	CButtonGL	m_tagTitleNoMotion;
	CSliderGL	m_sliNoMotionThd;

	CButtonGL	m_tagTitleNoObject;
	CSliderGL	m_sliNoObjectThd;

    CButtonGL   m_tagCircle;

    CButtonGL   m_tagBlock;
    CButtonGL   m_tagUp;
    CButtonGL   m_tagDown;
    CButtonGL   m_tagLeft;
    CButtonGL   m_tagRight;
    CButtonGL   m_tagForward;
    CButtonGL   m_tagBack;

    CButtonGL   m_tagTitleGestureCursorTrack;
    CSliderGL   m_sliGestureCursorTrack;
    CButtonGL   m_tagObject;
};


#endif
