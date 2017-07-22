/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company PrimaII BSP/CSP

 Copyright (c) 2010 - 2013  Cambridge Silicon Radio Limited, a CSR plc group
 company.

 All rights reserved.

 This Software is protected by United Kingdom copyright laws and international
 treaties.  You may not reverse engineer, decompile or disassemble this
 Software.

 WARNING:
 This Software contains Cambridge Silicon Radio Limited's confidential and
 proprietary information. UNAUTHORIZED COPYING, USE, MODIFICATION,
 DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL, REPACKAGING, REASSEMBLING
 OR DISCLOSURE OF THE WHOLE OR ANY PART OF THE SOFTWARE IS PROHIBITED AND MAY
 RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this Software without
 Cambridge Silicon Radio Limited's express written permission.   Use of any
 portion of the contents of this Software is subject to and restricted by your
 signed written agreement with Cambridge Silicon Radio Limited.
******************************************************************************/

/**
*
*  @file spp_screen.h
*
*  Abstract
*           - Header for the service to retrieve remote screen information through Bluetooth SPP.
*
*/

#ifndef  _SPP_SCREEN_H_
#define _SPP_SCREEN_H_

#include "spp_device.h"
#include "bluetooth_hid_screen.h"
#include "bluetooth_spp_screen.h"


#ifdef __cplusplus
extern "C" {
#endif


class CSppScreen : public CSppDevice
{
    public:
        CSppScreen();
        virtual ~CSppScreen();

        DWORD Init(DWORD sppIndex, SppScreenRegisterInfoT* registerInfo);
        DWORD Deinit();
        DWORD Connect(BT_ADDR bluetoothAddress, DWORD timeout);
        DWORD Disconnect();

        /* Init remote screen information fully. */
        DWORD InitInfo();

        DWORD SendData(LPCVOID lpBuffer, DWORD bufferSize);

        DWORD GetResolution(ScreenResolutionT* screenResolution);
        DWORD GetDirection(ScreenDirectionT* screenDirection);
        DWORD GetPointerMovementUnit(SHORT* pixelUnit);

        DWORD StartCalibration();
        DWORD StopCalibration();
        DWORD ShowCalibration(HidScreenDirectionT screenDir, COLORREF color);

        DWORD GetVelocityControlParameters(VelocityControlParameters* parameters);

        DWORD NotifyHdmiStatus(HdmiStatusT hdmiStatus);

        DWORD MessageHandler(PBYTE prim, DWORD primSize);

        void MessageHandlerCallback(PBYTE prim, DWORD primSize);

		DWORD StateHandler(DWORD connectionIndex, BT_ADDR bluetoothAddress, SppStateT state, DWORD result);

        void StateHandlerCallback(SppStateT state, DWORD result);

        static LPCTSTR GetScreenDirString(ScreenDirectionT screenDirection);

    private:
        DWORD ShowCalibration(HidScreenDirectionT screenDir);

        BOOL IsAutoConnectNeeded();

    private:
        BOOL m_fInit;
        SppScreenRegisterInfoT m_registerInfo;

        ScreenResolutionT m_screenResolution;
        ScreenDirectionT  m_screenDirection;
        SHORT m_pixelUnit;

        VelocityControlParameters m_velocityControlParameters;
};

#ifdef __cplusplus
}
#endif

#endif  // _SPP_SCREEN_H_
