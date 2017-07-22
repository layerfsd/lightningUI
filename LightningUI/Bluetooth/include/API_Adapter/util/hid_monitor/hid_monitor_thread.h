/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_monitor_thread.h

DESCRIPTION:    Header for HID monitor thread.

REVISION:       $Revision: #5 $

*******************************************************************************************************/
#ifndef _HID_MONITOR_THREAD_H
#define _HID_MONITOR_THREAD_H

#include <windows.h>
#include <cmthread.h>
#include <csync.h>
#include "bluetooth_hid_screen.h"
#include "bluetooth_hid_monitor.h"
#include "hid_monitor_util.h"
#include "hid_monitor.h"
#include "hid_touch_dialog.h"
#include "hid_touch_gesture.h"
#include "touch_msg_list.h"


#define HID_MONITOR_MAX_EVENT               4

/* HID screen calibration timeout in ms. */
#define DEFAULT_SCREEN_CALIBRATION_TIMEOUT  7500

/* +++ Macro relative with touch message. +++ */
#define IS_RESET_ZERO_POINT(msg)        (msg && \
                                         (msg->message == WM_MOUSEMOVE) && (!(msg->wParam)) && \
                                         IS_BOUNDARY_POINT(msg->pt.x, msg->pt.y))

#define IS_TOUCH_DBLCLK(msg)            (msg && (msg->message == WM_LBUTTONDBLCLK) && (msg->wParam == MK_LBUTTON))

#define IS_TOUCH_DOWN(msg)              (msg && (msg->message == WM_LBUTTONDOWN) && (msg->wParam == MK_LBUTTON))

#define IS_TOUCH_MOVE(msg)              (msg && (msg->message == WM_MOUSEMOVE) && (msg->wParam == MK_LBUTTON))

#define IS_TOUCH_UP(msg)                (msg && (msg->message == WM_LBUTTONUP))
/* --- Macro relative with touch message. --- */


typedef struct tagGESTUREMETRICS {
    UINT   cbSize; 
    DWORD  dwID;
    DWORD  dwTimeout; 
    DWORD  dwDistanceTolerance; 
    DWORD  dwAngularTolerance;
    DWORD  dwExtraInfo;
} GESTUREMETRICS, *LPGESTUREMETRICS;

typedef struct
{
    GESTUREINFO giBegin;    /* Begin gesture information. */
    GESTUREINFO giPrev;     /* Previous gesture information. */
} HidGestureInfo;

typedef struct
{
    POINT pt;               /* Point corresponding with virtual key. */
    BYTE vk;                /* Virtual key code. */
    BOOL prevState;         /* TRUE: the key is down previoiusly, FALSE: the key is up. */
} HidVirtualKeyInfo;

typedef struct
{
    MSG touchDownMsg;       /* Touch down message. */
    MSG touchPrevMsg;       /* Previoius touch message. */
} HidTouchInfo;

typedef std::queue<MSG*> HidMsgQueue;


class CHidMonitorThread : public CMiniThread, public CLockObject
{
    public:
        CHidMonitorThread();
        virtual ~CHidMonitorThread();

        virtual BOOL Init(CHidMonitor* pHidMonitor);
        virtual void Deinit();

        virtual DWORD ThreadRun();

        void NotifyTouchMonitor(BOOL fEnableMonitor);

        void NotifyHidConnected(BOOL fConnected);

        virtual HWND GetWnd()
        {
            return NULL;
        }

        LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        DWORD HandleTouchMessage(HWND hWnd, UINT msg, UINT flags, LONG x, LONG y);
        DWORD HandleTouchMessage(UINT msg, UINT flags, LONG x, LONG y);
        DWORD HandleTouchMessage(UINT msg, UINT flags);
        DWORD HandleTouchMessageExt(UINT msg, UINT flags, LONG x, LONG y);
        DWORD HandleRemoteMessage(UINT msg, UINT flags, LONG remotePointX, LONG remotePointY);

        DWORD GetHidPacketDivision(DWORD distance);

        static void DumpMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        /* Debug only to get touch gesture string. */
        static LPCTSTR GetGestureString(DWORD gestureID);

        DWORD WaitScreenCalibrationDone(DWORD timeout = DEFAULT_SCREEN_CALIBRATION_TIMEOUT);
        void NotifyScreenCalibrationDone();

        static float GetTouchSpeed(LONG dx, LONG dy, DWORD timeInMs, LONG* dist);

        virtual DWORD NotifyHdmiStatus()
        {
            return ERROR_NOT_SUPPORTED;
        }

    protected:
        void InitPointerVelocity();

        static BOOL PtInRange(POINT& curPoint, POINT& initPoint, LONG tolerance);
        BOOL IsValidGestureTolerance(POINT& curPoint, POINT& initPoint);
        BOOL IsTouchJitter(UINT message, UINT flags, LONG x, LONG y);

        void StoreTouchInfo(MSG* dstMsg, UINT message, UINT flags, LONG x, LONG y);
        void StoreTouchInfo(MSG* dstMsg, MSG* srcMsg);

        DWORD HandleTouchMsgProc();

        DWORD HandleResetZeroPointProc();
        void SentResetZeroPointMsg();
        void InitResetZeroPointMsg(MSG* msg);

        DWORD HandleHoldMsgProc();
        void SendHoldMsg();

        static void CALLBACK StaticTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
        void TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

        BOOL StartTimer(HidTimerT timerType, DWORD timeout);
        void StopTimer(HidTimerT timerType);
        void CleanTimer();

        BOOL StartHandleTouchMsgTimer(DWORD timeout);
        BOOL StartHandleTouchMsgTimer();
        void StopHandleTouchMsgTimer();
        BOOL RestartHandleTouchMsgTimer();

        BOOL StartResetZeroPointTimer();
        void StopResetZeroPointTimer();
        BOOL RestartResetZeroPointTimer();

        BOOL StartCheckHoldMsgTimer();
        void StopCheckHoldMsgTimer();
        BOOL RestartCheckHoldMsgTimer();

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        virtual BOOL IsMonitorNeeded();

        virtual BOOL StartMonitor();
        virtual void StopMonitor();

        BOOL CreateMonitorWindow();
        void DestroyMonitorWindow();

        BOOL StoreMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        BOOL StoreGestureMessage(UINT gestureID, HGESTUREINFO hgi);
        BOOL StoreTouchMessage(UINT message, UINT flags);
        BOOL StoreTouchMessage(UINT message, LONG x, LONG y);
        BOOL StoreTouchMessage(UINT message, UINT flags, LONG x, LONG y);
        BOOL StoreWindowMessage(HWND hWnd, UINT message, UINT flags, LONG x, LONG y);
        BOOL StoreWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        BOOL StoreWindowMessage(MSG* msg);

        void ClearMessage();

        void HandleMessage();

        BOOL HandleGestureMessage(UINT gestureID);

        BOOL PreHandleTouchMessage(MSG* msg);
        BOOL HandleTouchMessage(MSG* msg);
        BOOL PostHandleTouchMessage(MSG* msg);

        BOOL HandleExtendedVirtualKey(BYTE vk, UINT message, WPARAM wParam, LPARAM lParam);

        BOOL HandleGesture(UINT gestureID, HGESTUREINFO hgi);

        BOOL HandleVirtualKey(BYTE vk, DWORD vkFlags);

        BOOL HandleVirtualKeyOfMedialControl(BYTE vk, DWORD vkFlags);

        BOOL GetVirtualKey(LONG x, LONG y, BYTE& vk);

        BOOL SendTouchMessage(UINT message, WPARAM wParam, LPARAM lParam);

        BOOL ConvertLocal2RemotePoint(POINT* localPoint, POINT* remotePoint);

        DWORD InitializeInputReport(UINT msg, UINT flags, LONG x, LONG y, PCHAR* reportBuf, ULONG* reportSize);

        void StoreGestureInfo(GESTUREINFO& giDst, GESTUREINFO& giSrc);

        void InitGestureBeginTime()
        {
            /* [Debug Only] */
            m_dwGestureBeginTime = GetTickCount();
        }

        DWORD GetGestureElapsedTime(DWORD dwSequenceID)
        {
            /* [Debug Only][FIXME] It's found out that 'dwSequenceID' is always less than gesture begin time. Puzzled? */
            //return abs(dwSequenceID - m_dwGestureBeginTime);
            return (GetTickCount() - m_dwGestureBeginTime);
        }

        void StoreVKInfo(HidVirtualKeyInfo& vkInfo, UINT msg, LONG x, LONG y, BYTE vk);
        void ClearVKInfo();
        BOOL IsValidVKInfo(HidVirtualKeyInfo& vkInfo)
        {
            return VALID_VK(vkInfo.vk) ? TRUE : FALSE;
        }

        BOOL ShowTouchDialog(BOOL fShow = TRUE);

        void StartHandleTouchMsg();
        static DWORD StaticTouchMsgThreadStub(LPVOID lpParameter);
        DWORD TouchMsgThreadProc();

        void InitSendPacketTime()
        {
            m_dwSendPacketTime = GetTickCount();
        }

        DWORD GetSendPacketDiffTime()
        {
            return GetTickCount() - m_dwSendPacketTime;
        }

        void UpdateSendPacketDiffTime()
        {
            m_dwSendPacketDiffTime = GetSendPacketDiffTime();
        }

        inline void InitPoint(POINT& dstPoint, POINT& srcPoint)
        {
            dstPoint.x = srcPoint.x;
            dstPoint.y = srcPoint.y;
        }

        inline void InitPoint(POINT& pt, LONG x, LONG y)
        {
            pt.x = x;
            pt.y = y;
        }

        inline BOOL IsSamePoint(POINT& pt1, POINT& pt2)
        {
            return ((pt1.x == pt2.x) && (pt1.y == pt2.y)) ? TRUE : FALSE;
        }

        inline BOOL IsSamePoint(POINT& pt, LONG x, LONG y)
        {
            return ((pt.x == x) && (pt.y == y)) ? TRUE : FALSE;
        }

        static BOOL StaticHandleTouchGestureCompletion(MSG** msgList, int listLength);
        BOOL HandleTouchGestureCompletion(MSG** msgList, int listLength);

        static void StaticFreeTouchMsg(MSG* msg);

        static MSG* DupMsg(MSG* msg);

        BOOL IsValidPoint(LONG x, LONG y);

        float GetTouchSpeed(LONG newX, LONG newY, LONG* dist);

    protected:
        virtual void InitHandles();

        virtual void HandleEvent() {}

        virtual DWORD GetHidPacketDelay(UINT msg, UINT flags, LONG x, LONG y);

        void NotifyHostInfoEvent(DWORD eventData);

        DWORD SendReport(HiddReportLenT reportLen, HiddReportT report);
        DWORD EnableLowPowerMode(BOOL enable);

        void SendCallStatus();

        DWORD A2dpConnect(BT_ADDR bluetoothAddress);
        DWORD A2dpConnect();
        DWORD A2dpDisconnect();
        BOOL A2dpIsConnected();

        DWORD AvrcpConnect(BT_ADDR bluetoothAddress);
        DWORD AvrcpConnect();
        DWORD AvrcpDisconnect();
        BOOL AvrcpIsConnected();
        HRESULT AvrcpPlay();
        HRESULT AvrcpPause();
        HRESULT AvrcpNext();
        HRESULT AvrcpPrevious();

        virtual void VerifyA2dpConnection()
        {
        }

        virtual void VerifyAvrcpConnection()
        {
        }

    protected:
        DWORD m_hidFeature;
        HidMsgQueue m_msgQueue;
        HANDLE m_hEvents[HID_MONITOR_MAX_EVENT];
        DWORD m_dwEventCount;
        ScreenResolutionT m_localResolution;
        HWND m_hWnd;
        HANDLE m_hExitEvent;
        HANDLE m_hTouchMonitorEvent;
        HANDLE m_hHidConnectEvent;
        BOOL m_fHidConnected;
        BOOL m_fEnableMonitor;
        BOOL m_fMonitorStarted;
        GESTUREMETRICS m_gestureMetrics;
        HidVirtualKeyInfo m_hidVKInfo;
        HidGestureInfo m_hidGestureInfo;
        DWORD m_resetZeroPointTimeout;
        HANDLE m_hCalibrationEvent;
        DWORD m_pointInterval;          /* Delay in milli-second between 2 HID pointer packet. */
        DWORD m_checkHoldMsgTimeout;
        DWORD m_dwGestureBeginTime;     /* Debug only. */

        CTouchMsgList m_touchMsgList;
        CHidTouchDialog* m_pTouchDialog;
        HANDLE m_touchMsgSemaphore;
        HANDLE m_touchMsgThread;

        DWORD m_dwSendPacketTime;
        DWORD m_dwSendPacketDiffTime;

        CHidMonitor* m_pHidMonitor;

        CHidScheduler* m_pHidScheduler;

        UINT m_timers[HID_TIMER_MAX_COUNT];

        DWORD m_touchSelectTimeout;             /* Touch select timeout in ms. This is valid if MFC CDialog is used. */

        DWORD m_touchSelectDistanceTolerance;   /* Touch select distance tolerance in pixel. This is valid if MFC CDialog is used. */

        DWORD m_pointerVelocityLowThreshold;    /* Remote pointer velocity lowthreshold in pixel/sec.*/

        DWORD m_touchSleepTime;

        DWORD m_hidPacketSendIntervalTime;

        HidTouchInfo m_touchInfo;

        CHidTouchGesture m_touchGesture;        /* Utilty class which is mainly used to handle touch jitter. */

        DWORD m_touchMoveMsgIndex;
        DWORD m_hidNopCount;
        DWORD m_touchPointResampleFreq;

        DWORD m_touchScrollDistTolerance;       /* Touch scroll distance tolerance in pixel. This is only fitted with checking horizontal/vertical scroll in local screen . */
        DWORD m_touchScrollTimeTolerance;       /* Touch scroll time tolerance in ms. This is only fitted with checking horizontal/vertical scroll in local screen. */

        DWORD m_touchVelocitySpeed;             /* Pixel per second in local touch screen. The parameter is used to reduce local touch point if needed. */
        DWORD m_touchVelocityDistance;          /* Pixel in local touch screen. The parameter is used to reduce local touch point if needed.*/

        DWORD m_hidDistanceBase;                /* HID pointer distance in remote phone, measured in pixel. This value is used to divide HID packet.*/

        BOOL m_fNeedRecoverA2dpConnection;      /* Flag indicating whether to recover A2DP connection after HDMI isn't active. This is only valid with 'm_enableA2dpForHdmi(FALSE)'. */

        /* Generic configuration in HID monitor thread. */
        static BOOL m_enableHidForHdmi;             /* TRUE: Enable HID for HDMI. FALSE: Disable. */
        static BOOL m_enableAvrcpForHdmi;           /* TRUE: Enable to control media player in remote phone through AVRCP. FALSE: Use Bluetooth HID consumer control to control media player. */
        static BOOL m_enableA2dpForHdmi;            /* TRUE: Enable to make remote phone transfer audio through A2DP for HDMI. FALSE: Use HDMI audio. */
        static BOOL m_enableTouchGesture;           /* TRUE: Enable to utilize HID touch gesture instead of normal touch message. FALSE: Use MFC/ATL CDialog to capture touch message. */
        static BOOL m_enableTouchMsgThread;         /* TRUE: Enable to handle touch msg in seperate thread. FALSE: Disable. */
        static BOOL m_enableTouchTimer;             /* TRUE: Enable to start HID touch timer after touch is release. FALSE: Disable. */
        static BOOL m_enableDelay;                  /* TRUE: Enable to add delay when sending HID packet. FALSE: Disable. */
        static BOOL m_enableConstInterval;          /* TRUE: Enable to add const HID packet interval before sending HID packet. FALSE: Use dynamic delay according to pointer velocity in Remote phone. */
        static BOOL m_enableResetZeroPoint;         /* TRUE: Enable to move HID mouse to reset zero point right after touch is released. FALSE: Disable. */
        static BOOL m_enableHandleTouchJitter;      /* TRUE: Enable to handle touch jitter in touch message captured in CDialog. FALSE: Disable. */
        static BOOL m_enableHandleTouchJitterforVK; /* TRUE: Enable to handle touch jitter in touch message captured in CDialog specific with virtual key or extended virtual key. FALSE: Disable. */
        static BOOL m_enableSyncPointer;            /* TRUE: Enable to sync pointer cursor after some time when touch is released. FALSE: Disable. */
        static BOOL m_enableSendHidNopPacket;       /* TRUE: Enable to send HID NOP packet during touch is moving, which is for the work-around of touch floating. FALSE: Disable. */
        static BOOL m_enableDirectHandleMessage;    /* TRUE: Enable to handle touch message directly. FALSE: Disable. */
        static BOOL m_enableReduceMovePoint;        /* TRUE: Enable to reduce touch move point. FALSE: Disable. */
        static BOOL m_enableCheckHoldMsg;           /* TRUE: Enable to check hold message. FALSE: Disable. */
        static BOOL m_enableCheckHorizontalScroll;  /* TRUE: Enable to check horizontal scroll. FALSE: Disable. */
        static BOOL m_enableCheckVerticalScroll;    /* TRUE: Enable to check vertical scroll. FALSE: Disable. */
        static BOOL m_enableLinearInterpolation;    /* TRUE: Enable to add the linear interpolation point. FALSE: Disable. */
        static BOOL m_enableDrawTouchLine;          /* [TestOnly] TRUE: Enable to draw line in local screen when touch is pressed, moved. This is mainly for the comparing touch between local screen and remote screen. */
};

class CHdmiThread : public CHidMonitorThread
{
    public:
        CHdmiThread();
        virtual ~CHdmiThread();

        BOOL Init(CHidMonitor* pHidMonitor);
        void Deinit();

        HWND GetWnd();

        static HWND GetWnd(LPTSTR appName);

        void VerifyA2dpConnection();

        void VerifyAvrcpConnection();

        DWORD NotifyHdmiStatus();

    private:
        void InitHandles();

        void HandleEvent();

        BOOL IsMonitorNeeded();

        BOOL StartMonitor();

        BOOL InitHdmiWnd();

        HWND GetHdmiWnd();
    
        HWND GetHdmiWnd(LPCTSTR hdmiAppName);

        void HandleHdmiStatusInd(WORD hdmiStatus);
        void HandleHdmiCalibrateInd(BYTE result, BYTE dir);
        void HandleHdmiControlBarStatusInd(WORD controlBarStatus);

    private:
        HWND m_hHdmiWnd;
        HANDLE m_hHdmiEvent;
        BOOL m_fHdmiActive;
};

#endif /* _HID_MONITOR_THREAD_H */
