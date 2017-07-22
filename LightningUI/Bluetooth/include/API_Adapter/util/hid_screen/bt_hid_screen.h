/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bt_hid_screen.h

DESCRIPTION:    Header for Bluetooth HID screen.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BT_HID_SCREEN_H
#define _BT_HID_SCREEN_H

#include <windows.h>
#include "hash_map.hxx"
#include "bluetooth_hid_screen.h"


using ce::hash_map;


/* Additonal virtual key which is extended: VK_CLOSE, VK_DOCK. */
#define ADDITIONAL_VK_COUNT                 2

/* Default 3 virtual key: VK_MENU, VK_HOME, VK_BACK. */
#define DEFAULT_VK_COUNT                    3

/* Default 4 virtual key of media control: VK_PLAY, VK_PAUSE, VK_MEDIA_NEXT_TRACK, VK_MEDIA_PREV_TRACK. */
#define DEFAULT_VK_COUNT_OF_MEDIA_CONTROL   4

/* VK_DOCK rect */
#define DEFAULT_VK_DOCK_RECT_LEFT       ((LONG) 30)
#define DEFAULT_VK_DOCK_RECT_TOP        ((LONG) 30)
#define DEFAULT_VK_DOCK_RECT_RIGHT      ((LONG) 60)
#define DEFAULT_VK_DOCK_RECT_BOTTOM     ((LONG) 60)

/* VK_MENU rect */
#define DEFAULT_VK_MENU_RECT_LEFT       ((LONG) 60)
#define DEFAULT_VK_MENU_RECT_TOP        ((LONG) 30)
#define DEFAULT_VK_MENU_RECT_RIGHT      ((LONG) 90)
#define DEFAULT_VK_MENU_RECT_BOTTOM     ((LONG) 60)

/* VK_HOME rect */
#define DEFAULT_VK_HOME_RECT_LEFT       ((LONG) 90)
#define DEFAULT_VK_HOME_RECT_TOP        ((LONG) 30)
#define DEFAULT_VK_HOME_RECT_RIGHT      ((LONG) 120)
#define DEFAULT_VK_HOME_RECT_BOTTOM     ((LONG) 60)

/* VK_BACK rect */
#define DEFAULT_VK_BACK_RECT_LEFT       ((LONG) 120)
#define DEFAULT_VK_BACK_RECT_TOP        ((LONG) 30)
#define DEFAULT_VK_BACK_RECT_RIGHT      ((LONG) 150)
#define DEFAULT_VK_BACK_RECT_BOTTOM     ((LONG) 60)

/* VK_CLOSE rect */
#define DEFAULT_VK_CLOSE_RECT_LEFT       ((LONG) 150)
#define DEFAULT_VK_CLOSE_RECT_TOP        ((LONG) 30)
#define DEFAULT_VK_CLOSE_RECT_RIGHT      ((LONG) 180)
#define DEFAULT_VK_CLOSE_RECT_BOTTOM     ((LONG) 60)


/* Map virtual key into HID consumer control usage. */
typedef hash_map<RECT*, BYTE> VirtualKeyRectMap;


class CHidScreen
{
    public:
        CHidScreen();
        virtual ~CHidScreen();

        BOOL Init();
        void Deinit();

        BOOL SetInfo(HidScreenInfoT* phs);
        BOOL GetInfo(HidScreenInfoT* phs);
        static BOOL GetDefaultInfo(HidScreenInfoT* phs);

        BOOL IsValidPoint(LONG x, LONG y);
        BOOL IsValidPoint(POINT* localPoint);
        BOOL ConvertLocal2RemotePoint(POINT* localPoint, POINT* remotePoint);

        static BOOL IsValidScreen(HidScreenInfoT* phs);
        static BOOL VerifyScreen(HidScreenInfoT* phs);

        static BOOL IsValidResolution(ScreenResolutionT* psr);
        static BOOL IsValidOutputRect(ScreenResolutionT* psr, RECT* pOutputRect);

        static BOOL GetLocalResolution(ScreenResolutionT* localResolution);
        static BOOL GetLocalDirection(ScreenDirectionT* localDirection);

        void SetRemoteResolution(LONG screenWidth, LONG screenHeight);
        void SetRemoteDirection(ScreenDirectionT screenDirection);
        void SetOutputRect(ScreenDirectionT screenDirection, RECT* rect);

        BOOL GetRemoteResolution(ScreenResolutionT* remoteResolution);
        BOOL GetRemoteDirection(ScreenDirectionT* remoteDirection);
        BOOL GetOutputRect(ScreenDirectionT screenDirection, RECT* rect);

        static LPCTSTR GetDirectionString(ScreenDirectionT sd);

        BOOL SetVirtualKeyRect(VirtualKeyRectT* vkRect, BYTE vkCount);
        BOOL GetVirtualKeyRect(VirtualKeyRectT** vkRect, BYTE* vkCount);
        BOOL GetDefaultVirtualKeyRect(VirtualKeyRectT** vkRect, BYTE* vkCount);

        BOOL GetVirtualKey(LONG x, LONG y, BYTE& vk);
        BOOL GetVirtualKey(POINT* localPoint, BYTE& vk);

        static LPCTSTR GetVirtualKeyString(BYTE vk);

        DWORD CalculateOutputRect(ScreenDirectionT screenDir, COLORREF color, RECT* rect);

        static DWORD MapResult(HidScreenResultT hsr);

    private:
        BOOL InitLocalInfo();

        BOOL IsValidPoint(POINT* localPoint, RECT* pOutputRect);

        RECT* GetOutputRect();

        LONG GetRemotePointX(LONG outputX, RECT* pOutputRect);
        LONG GetRemotePointY(LONG outputY, RECT* pOutputRect);

        void ClearVirtualKeyRect();

        BOOL IsValidVirtualKeyRect(RECT* pOutputRect);

    private:
        HidScreenInfoT m_hidScreenInfo;
        VirtualKeyRectMap m_vkMap;
        RECT* m_pRect;
};


#endif
