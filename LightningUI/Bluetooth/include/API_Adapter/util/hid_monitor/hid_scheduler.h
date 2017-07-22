/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_scheduler.h

DESCRIPTION:    Header for HID scheduler to handle touch message.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _HID_SCHEDULER_H
#define _HID_SCHEDULER_H

#include <windows.h>
#include <svsutil.hxx>


typedef enum 
{
    HSE_HANDLE_TOUCH_MSG = 0,
    HSE_RESET_ZERO_POINT,
    HSE_CHECK_HOLD_MSG,
    HSE_MAX_EVENT_COUNT
} HidSchedulerEventT;


class CHidScheduler : public SVSThreadPool
{
    public:
        CHidScheduler(DWORD maxEventCount = HSE_MAX_EVENT_COUNT);
        virtual ~CHidScheduler();

        BOOL Init();
        void Deinit();

        BOOL StartScheduleTouchMsg(LPTHREAD_START_ROUTINE pfn, LPVOID lpParameter, unsigned long ulDelayTime = 0);
        BOOL StopScheduleTouchMsg();

        BOOL ScheduleHandleTouchMsg(LPTHREAD_START_ROUTINE pfn, LPVOID lpParameter, unsigned long ulDelayTime = 0);   
        BOOL UnScheduleHandleTouchMsg();

        BOOL ScheduleResetZeroPoint(LPTHREAD_START_ROUTINE pfn, LPVOID lpParameter, unsigned long ulDelayTime = 0);
        BOOL UnScheduleResetZeroPoint();

        BOOL ScheduleCheckHoldMsg(LPTHREAD_START_ROUTINE pfn, LPVOID lpParameter, unsigned long ulDelayTime = 0);
        BOOL UnScheduleCheckHoldMsg();

    private:
        BOOL ScheduleEvent(HidSchedulerEventT evt, LPTHREAD_START_ROUTINE pfn, LPVOID lpParameter, unsigned long ulDelayTime);
        BOOL UnScheduleEvent(HidSchedulerEventT evt);


    private:
        DWORD m_maxEventCount;
        SVSCookie* m_cookie;
        BOOL m_fHandlingTouchMsg;
};

#endif /* _HID_SCHEDULER_H */
