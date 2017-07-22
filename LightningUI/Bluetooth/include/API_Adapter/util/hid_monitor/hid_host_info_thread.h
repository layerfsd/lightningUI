/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_host_info_thread.h

DESCRIPTION:    Header for HID host information thread.

REVISION:       $Revision: #2 $

*******************************************************************************************************/
#ifndef _HID_HOST_INFO_THREAD_H
#define _HID_HOST_INFO_THREAD_H

#include <windows.h>
#include <cmthread.h>
#include "hid_monitor.h"


enum
{
    HID_START_INIT_HOST_INFO = 0,
    HID_STOP_INIT_HOST_INFO,
};


class CHidHostInfoThread : public CMiniThread
{
    public:
        CHidHostInfoThread();
        virtual ~CHidHostInfoThread();

        BOOL Init(CHidMonitor* pHidMonitor, BOOL fStoreHostInfo = TRUE);
        void Deinit();

        DWORD ThreadRun();

        void NotifyHidConnected(BOOL fConnected);

        void NotifyHostInfoEvent(DWORD eventData = HID_START_INIT_HOST_INFO);

        void InitVirtualKeyRect();

        BOOL IsHidHost(BT_ADDR bluetoothAddress);

        BOOL GetHidHostInfo(BT_ADDR bluetoothAddress, HidHostInfo* pHostInfo);

        BOOL ClearHidHostInfo(BT_ADDR bluetoothAddress);

    private:
        DWORD SendReport(HiddReportLenT reportLen, HiddReportT report);

        BOOL InitHostInfo();

        BOOL InitHostInfo(BT_ADDR bluetoothAddress);

        BOOL GetHostInfo(BT_ADDR bluetoothAddress, HidHostInfo* pHostInfo);

        BOOL GetScreenInfo(LPTSTR szReg, HidScreenInfoT* phs);
        BOOL ClearScreenInfo(BT_ADDR bluetoothAddress);

        BOOL StoreHostInfo(LPTSTR szReg, HidHostInfo* pHostInfo);
        BOOL StoreHostInfo(BOOL fHidConnected);

        static void SppStateHandlerStub(SppStateT state, DWORD result);
        void SppStateHandler(SppStateT state, DWORD result);

        /* Handler for the message retrieved from remote screen through SPP. */
        static void SppMessageHandlerStub(PBYTE prim, DWORD primSize);
        void SppMessageHandler(PBYTE prim, DWORD primSize);
        void HandleScreenResolution(DWORD result, LONG screenWidth, LONG screenHeight);
        void HandleScreenDirection(DWORD result, ScreenDirectionT screenDir);
        void HandlePointerMovementUnit(DWORD result, SHORT pointerSpeed);
        void HandleCalibrationScreen(DWORD result, ScreenDirectionT screenDir, COLORREF color);
        void HandleGetVelocityControlParameters(DWORD result, VelocityControlParameters* parameters);

        DWORD WaitScreenCalibrationDone();

        void SendHIDStatus(HidStateT hidState, BOOL fScreenCalibrated = FALSE);

        void SendHIDCalibrateReq(WORD screenDir);

        void DumpReport(PCHAR reportBuf, ULONG reportSize);

        DWORD NotifyHdmiStatus();

    private:
        CHidMonitor* m_pHidMonitor;
        BOOL m_enableSppForHid;
        BOOL m_fHidConnected;
        BOOL m_fStoreHostInfo;
        BOOL m_fSppConnected;
        HANDLE m_hHostInfoEvent;
        HANDLE m_hSppConnectionEvent;

        HidHostInfo m_hidHostInfo;
};

#endif /* _HID_HOST_INFO_THREAD_H */
