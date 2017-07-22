//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company PrimaII BSP/CSP

 Copyright (c) 2010 - 2011  Cambridge Silicon Radio Limited, a CSR plc group
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
//
//------------------------------------------------------------------------------
//
//  File:  KerI2C.h
//
/*
* In this file, we encapsulate a thin layer to easy other driver to use I2C driver.
*/


#ifndef __KERI2C_H
#define __KERI2C_H

#include <WinIoCtl.h>
#include "cspi2c.h"

//typedef unsigned long ULONG;
//typedef unsigned char BYTE;
//typedef void*         HANDLE;
//typedef bool          BOOL;

#ifdef __cplusplus
extern "C"
{
#endif

    //------------------------------------------------------------------------------
    //
    //  GUID:  DEVICE_IFC_I2C_GUID
    //
    DEFINE_GUID(
        DEVICE_IFC_I2C_GUID, 0xecdf6dd3, 0x4405, 0x4ebc,
        0x87, 0x23, 0xf4, 0x48, 0x1a, 0x7, 0x2c, 0x4c
    );
    //------------------------------------------------------------------------------
    //
    //  Define:  IOCTL_I2C_GET_DRIVER_IFC
    //
    //  This IOCTL code is used to obtain device driver interface pointers used for
    //  in-process calls between drivers. The input driver can contain interface
    //  GUID. The driver should fill output buffer with interface structure based
    //  on interface GUID.
    //
#define IOCTL_I2C_GET_DRIVER_IFC        \
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0000, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C_TRANSACT     \
            CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0001, METHOD_BUFFERED, FILE_ANY_ACCESS)

    //------------------------------------------------------------------------------
    //
    //  Type:  DEVICE_IFC_I2C
    //
    //  This structure is used to obtain I2C interface funtion pointers used for
    //  in-process calls via IOCTL_DDK_GET_DRIVER_IFC.
    //

    typedef struct
    {
        DWORD context;
        BOOL (*pfnTransact)(DWORD context, I2CTRANS *pTrans);
    } DEVICE_IFC_I2C;

    //------------------------------------------------------------------------------
    //
    //  Type:  DEVICE_CONTEXT_I2C
    //
    //  This structure is used to store I2C device context.
    //

    typedef struct
    {
        DEVICE_IFC_I2C ifc;
        HANDLE hDevice;
    } DEVICE_CONTEXT_I2C;

    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    //
    //  Functions: I2Cxxx
    //
    // example devicename L"I2C1:"
    __inline HANDLE I2COpen(LPCWSTR devicename)
    {
        HANDLE hDevice;
        DEVICE_CONTEXT_I2C *pContext = NULL;

        hDevice = CreateFile(devicename, 0, 0, NULL, 0, 0, NULL);

        if (hDevice == INVALID_HANDLE_VALUE) goto clean;

        // Allocate memory for our handler...
        if ((pContext = (DEVICE_CONTEXT_I2C *)LocalAlloc(
            LPTR, sizeof(DEVICE_CONTEXT_I2C)
        )) == NULL)
        {
            CloseHandle(hDevice);
            goto clean;
        }

        // Get function pointers, fail when IOCTL isn't supported...
        //if (!DeviceIoControl(
        //        hDevice, IOCTL_I2C_GET_DRIVER_IFC, (VOID *)&DEVICE_IFC_I2C_GUID,
        //        sizeof(DEVICE_IFC_I2C_GUID), &pContext->ifc, sizeof(DEVICE_IFC_I2C),
        //        NULL, NULL
        //    ))
        //{
        //    CloseHandle(hDevice);
        //    LocalFree(pContext);
        //    pContext = NULL;
        //    goto clean;
        //}

        // Save device handle
        pContext->hDevice = hDevice;

clean:
        return pContext;
    }

    __inline VOID I2CClose(HANDLE hContext)
    {
        DEVICE_CONTEXT_I2C *pContext = (DEVICE_CONTEXT_I2C *)hContext;

        if ((pContext != NULL) && (pContext->hDevice != INVALID_HANDLE_VALUE))
            CloseHandle(pContext->hDevice);

        if (pContext != NULL)
            LocalFree(pContext);
    }

    __inline BOOL I2CTransact(HANDLE hContext, I2CTRANS *pTrans)
    {
        DEVICE_CONTEXT_I2C *pContext = (DEVICE_CONTEXT_I2C *)hContext;
        DWORD dwRelSize = 0;

        return DeviceIoControl(pContext->hDevice, IOCTL_I2C_TRANSACT, NULL, 0, pTrans, sizeof(I2CTRANS), &dwRelSize, NULL);
    }

    //------------------------------------------------------------------------------

#ifdef __cplusplus
}

#endif

#endif

