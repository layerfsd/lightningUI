/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_monitor_util.h

DESCRIPTION:    Header for HID monitor utility.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _HID_MONITOR_UTIL_H
#define _HID_MONITOR_UTIL_H


#ifdef DEBUG
#define DBG_HIDM_INIT                1
#define DBG_HIDM_INFO                1
#define DBG_HIDM_OUTPUT              1
#define DBG_HIDM_DUMP                1
#define DBG_HIDM_WARN                1
#define DBG_HIDM_ERROR               1
#else
#define DBG_HIDM_INIT                0
#define DBG_HIDM_INFO                0
#define DBG_HIDM_OUTPUT              0
#define DBG_HIDM_DUMP                0
#define DBG_HIDM_WARN                1
#define DBG_HIDM_ERROR               1
#endif


#undef CHK
#define CHK(flag, msg)              { \
                                        if (!(flag)) \
                                        { \
                                            RETAILMSG(DBG_HIDM_ERROR, (TEXT("<%s> %s fail\r\n"), _T(__FUNCTION__), (msg))); \
                                            break; \
                                        } \
                                    }


typedef enum 
{
    HID_TIMER_HANDLE_TOUCH_MSG = 0,
    HID_TIMER_RESET_ZERO_POINT,
    HID_TIMER_CHECK_HOLD_MSG,
    HID_TIMER_MAX_COUNT
} HidTimerT;


class CHidMonitor;

class CHidMonitorThread;
class CHdmiThread;

class CHidHostInfoThread;

class CHidScheduler;

class CTouchMsgList;

class CHidTouchDialog;


#endif /* _HID_MONITOR_UTIL_H */
