/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_touch_gesture.h

DESCRIPTION:    Header for the utility of HID touch gesture which is parsed by touch message captured.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _HID_TOUCH_GESTURE_H
#define _HID_TOUCH_GESTURE_H


#include <winuser.h>
#include <csync.h>
#include <vector>


using std::vector;


/* Default touch select timeout in ms. This is valid if MFC CDialog is used to capture touch message. */
#define DEFAULT_TOUCH_SELECT_TIMEOUT                900

/* Default touch slect distance tolerance in pixel. The default value 26 corresponds with the distance of 0.197 inch (0.5cm), 
   if the touch screen is 7 inch with the resolution (800 * 480). This is valid if MFC CDialog is used to capture touch message. */
#define DEFAULT_TOUCH_SELECT_DISTANCE_TOLERANCE     8       // 26

#define MAX_TOUCH_MSG_LIST_LENGTH                   0x1000


enum TouchGestureType
{
    TG_TYPE_BEGIN = 0,
    TG_TYPE_END,
    TG_TYPE_SELECT,
    TG_TYPE_PAN,
    TG_TYPE_HOLD,

    TG_MAX_COUNT,
    TG_INVALID_TYPE = 0xFF
};

enum TouchGestureState
{
    TG_STATE_IDLE = 0,
    TG_STATE_HANDLING,
    TG_STATE_FINAL
};


/* Function pointer for the callback when checking touch select is completed. */
typedef BOOL (WINAPI* HandleTouchGestureCompletionFuncPtrT)(MSG** msgList, int listLength);

/* Function pointer to free touch message. */
typedef void (WINAPI* FreeTouchMsgFuncPtrT)(MSG* msg);


typedef struct
{
    HandleTouchGestureCompletionFuncPtrT  handleTouchGestureCompletionCb;
    FreeTouchMsgFuncPtrT                  freeTouchMsgCb;
} HidTouchGestureRegisterInfoT;


class CHidTouchGesture
{
    public:
        CHidTouchGesture(DWORD maxTouchMsgListLength = MAX_TOUCH_MSG_LIST_LENGTH);
        virtual ~CHidTouchGesture();

        BOOL Init(HidTouchGestureRegisterInfoT* registerInfo,
                  DWORD touchSlectTimeout = DEFAULT_TOUCH_SELECT_TIMEOUT, 
                  DWORD touchSelectDistanceTolerance = DEFAULT_TOUCH_SELECT_DISTANCE_TOLERANCE);
        void Deinit();

        BOOL ProcessTouchMsg(MSG* msg);        

    private:
        void Clear();

        MSG* CopyTouchMsg(MSG* dstMsg, MSG* srcMsg);

        void UpdateTouchMsg(MSG* msg);
        void UpdateTouchMsg(MSG* msg, LONG newX, LONG newY);
        void UpdateTouchMsg(MSG* msg, UINT newMessage, UINT newFlag, LONG newX, LONG newY);

        BOOL IsTouchSelectMsg(MSG* msg);

        static DWORD GetPointDistance(POINT& pt1, POINT& pt2);

        BOOL StoreTouchMsg(MSG* msg);
        void ClearTouchMsg();

        BOOL HandleTouchSelect();

        BOOL HandleTouchGestureCompletion();

        BOOL HandleTouchGestureCompletion(MSG* msg)
        {
            return HandleTouchGestureCompletion(&msg, 1);
        }

        BOOL HandleTouchGestureCompletion(MSG** msgList, int listLength)
        {
            if (m_registerInfo.handleTouchGestureCompletionCb)
            {
                return m_registerInfo.handleTouchGestureCompletionCb(msgList, listLength);
            }
            else
            {
                return FALSE;
            }
        }

        void FreeTouchMsg(MSG* msg)
        {
            if (m_registerInfo.freeTouchMsgCb)
            {
                m_registerInfo.freeTouchMsgCb(msg);
            }
        }

        inline void Lock()
        {
            m_lock.Lock();
        }

        inline void Unlock()
        {
            m_lock.Unlock();
        }

        static LPCTSTR GetTouchGestureType(DWORD type);
        static LPCTSTR GetTouchGestureState(DWORD state);

    private:
        HidTouchGestureRegisterInfoT m_registerInfo;

        DWORD m_touchSelectTimeout;
        DWORD m_touchSelectDistanceTolerance;
        DWORD m_maxTouchMsgListLength;

        DWORD m_state;
        UINT m_timerID;

        MSG m_touchDownMsg;

        DWORD m_touchGestureType;

        vector<MSG*> m_pendingMsgList;

        CLockObject m_lock;
};

#endif /* _HID_TOUCH_GESTURE_H */
