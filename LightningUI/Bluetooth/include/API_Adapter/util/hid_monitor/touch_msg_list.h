/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           touch_msg_list.h

DESCRIPTION:    Header for touch message list.

REVISION:       $Revision: #2 $

*******************************************************************************************************/
#ifndef _TOUCH_MSG_LIST_H
#define _TOUCH_MSG_LIST_H

#include <csync.h>
#include <vector>

using std::vector;


#define MAX_TOUCH_MSG_LIST_SIZE     0x2000


class CTouchMsgList
{
    public:
        CTouchMsgList(DWORD maxSize = MAX_TOUCH_MSG_LIST_SIZE);
        virtual ~CTouchMsgList();

        BOOL StoreMsg(MSG** msgList, int listLength);

        BOOL StoreMsg(MSG* msg);


       BOOL InsertMsg(MSG* msg);
    
        MSG* PeekMsg();
        MSG* PeekMsg(DWORD index);

        void PopMsg();
        void PopMsg(DWORD endIndex);

        void Clear();

        BOOL IsEmpty();

        int GetSize();

    private:
        inline void Lock()     
        {
            m_lock.Lock();
        }

        inline void Unlock()
        {
            m_lock.Unlock();
        }

        void InitStartTime()
        {
            m_startTime = GetTickCount();
        }

        DWORD GetTimeElapsed()
        {
            return GetTickCount() - m_startTime;
        }

    private:
        DWORD m_startTime;  /* [DebugOnly] */
        vector<MSG*> m_list;
        CLockObject m_lock;
};


#endif /* _TOUCH_MSG_LIST_H */
