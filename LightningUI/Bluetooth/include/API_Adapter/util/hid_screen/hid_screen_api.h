/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           hid_screen_api.h

DESCRIPTION:    Header for HID screen API.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _HID_SCREEN_API_H
#define _HID_SCREEN_API_H

#include "bluetooth_hid_screen.h"
#include "bt_hid_screen.h"


#ifdef DEBUG
#define DBG_HIDS_INIT                    1
#define DBG_HIDS_INFO                    1
#define DBG_HIDS_OUTPUT                  1
#define DBG_HIDS_DUMP                    1
#define DBG_HIDS_WARN                    1
#define DBG_HIDS_ERROR                   1
#else
#define DBG_HIDS_INIT                    0
#define DBG_HIDS_INFO                    0
#define DBG_HIDS_OUTPUT                  0
#define DBG_HIDS_DUMP                    0
#define DBG_HIDS_WARN                    0
#define DBG_HIDS_ERROR                   1
#endif


class CHidScreenAPI
{
    public:
        CHidScreenAPI();
        virtual ~CHidScreenAPI();

        BOOL Init();
        void Deinit();

        BOOL CheckValid(HidScreenInfoT* phs);
        BOOL Verify(HidScreenInfoT* phs);

        BOOL SetInfo(HidScreenInfoT* phs);
        BOOL GetInfo(HidScreenInfoT* phs);
        BOOL GetDefaultInfo(HidScreenInfoT* phs);

        BOOL GetLocalResolution(ScreenResolutionT* psr);
        BOOL GetLocalDirection(ScreenDirectionT* psd);

        BOOL SetRemoteResolution(ScreenResolutionT* psr);
        BOOL SetRemoteDirection(ScreenDirectionT sd);

        BOOL SetOutputRect(ScreenDirectionT sd, RECT* rect);

        BOOL IsValidPoint(POINT* localPoint);
        BOOL ConvertLocal2RemotePoint(POINT* localPoint, POINT* remotePoint);

        BOOL SetVirtualKeyRect(VirtualKeyRectT* vkRect, BYTE vkCount);
        BOOL GetVirtualKeyRect(VirtualKeyRectT** vkRect, BYTE* vkCount);
        BOOL GetDefaultVirtualKeyRect(VirtualKeyRectT** vkRect, BYTE* vkCount);

        BOOL GetVirtualKey(LONG x, LONG y, BYTE& vk);

        static LPCTSTR GetVirtualKeyString(BYTE vk);

        static LPCTSTR GetDirectionString(ScreenDirectionT sd);

        static DWORD MapResult(HidScreenResultT hsr);

    private:
        CHidScreen* m_pHidScreen;
};

#endif /* _HID_SCREEN_API_H */
