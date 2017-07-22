/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_touch_dialog.h

DESCRIPTION:    Header for HID monitor.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _HID_TOUCH_DIALOG_H
#define _HID_TOUCH_DIALOG_H

#include <afxwin.h>
#include "bluetooth_hid_monitor.h"
#include "hid_monitor_util.h"
#include "hid_monitor_thread.h"


#define IDD_HID_TOUCH 168


class CHidTouchDialog : public CDialog
{
    // Construction
    public:
        CHidTouchDialog(CWnd* pParent = NULL);  // standard constructor

        // Dialog Data
        enum { IDD = IDD_HID_TOUCH };

        BOOL Init(CHidMonitorThread* pHidMonitorThread, BOOL enableDrawTouchLine);

        BOOL ShowDlg(BOOL fShow);

        void ClearDlg();

    private:
        static DWORD StaticShowDlgThreadStub(LPVOID lpParameter);
        DWORD ShowDlgThreadProc();

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
    protected:
        CPoint m_OriPoint;  

        HBRUSH m_hbr;
    
        // Generated message map functions
        virtual BOOL OnInitDialog();

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
        afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
        DECLARE_MESSAGE_MAP()

    public:
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    private:
        DWORD HandleTouchMessage(HWND hWnd, UINT msg, UINT flags, LONG x, LONG y);

    private:
        CHidMonitorThread* m_pHidMonitorThread;
        BOOL m_enableDrawTouchLine;
        int m_screenWidth;
        int m_screenHeight;
        BOOL m_fActivated;
        BOOL m_fTouchPressed;       /* Flag indicating wehther touch is pressed or not. */
        DWORD m_touchDownTime;      /* [DebugOnly] The time when touch is pressed. */
        DWORD m_touchStartTime;     /* [DebugOnly] Start time when touch is pressed. */
        DWORD m_touchPrevTime;      /* [DebugOnly] The time for previous touch. */
};

#endif /* _HID_TOUCH_DIALOG_H */
