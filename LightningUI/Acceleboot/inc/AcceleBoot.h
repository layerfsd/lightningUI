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

#ifndef _ACCELE_BOOT_H_
#define _ACCELE_BOOT_H_

#include "windows.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum _ACCELEBOOT_EVENT
    {
        ACCELEBOOT_EVENT_SHUTDOWN,          //device will create one acceleboot snapshot
        ACCELEBOOT_EVENT_BOOT,              //device has booted up from one snapshot
        ACCELEBOOT_EVENT_NONE               //no acceleboot event happened
    } ACCELEBOOT_EVENT, *PACCELEBOOT_EVENT;

    typedef enum _POWEROFF_MODE
    {
        POWEROFFMODE_SUSPEND,            //will enter susmpend mode
        POWEROFFMODE_HIBERNATE,          //will enter hibernation mode
        POWEROFFMODE_CREATE_ACCELEBOOT_SNAPSHOT, //is creating the acceleboot snapshot
        POWEROFFMODE_INVALID                   //invalid mode
    } POWEROFF_MODE, *PPOWEROFF_MODE;

    typedef enum
    {
        BOOTIMG_CE_IMG = 0,           //boot from ce img (cold boot or warm reset)
        BOOTIMG_ACCELEBOOT_SNAPSHOT,    //boot from acceleboot snapshot
        BOOTIMG_HIBERNATION,          //boot from hibernation snapshot
        BOOTIMG_SLEEP,                //boot from suspend
        BOOTIMG_INVALID
    }BOOT_IMG;

//the snapshot index which is reserved for hibernation
// any other index can be used as acceleboot snapshot idx
#define SNAPSHOT_IDX_HIBERNATE         0

    typedef void (*PFN_ACCELEBOOT_NOTIFICATION)(ACCELEBOOT_EVENT AccelebootEvent);

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Function:       RequestAccelebootNotification()
    Description:    The exported api for applications/drivers to register the envent handle call back
                    function.
    Notes:
    Returns:        TRUE: success; FALSE: failed
    -------------------------------------------------------------------------------*/
    BOOL RequestAccelebootNotification(PFN_ACCELEBOOT_NOTIFICATION pfnFBNotification);

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Function:       StopAccelebootNotification()
    Description:    The exported api for applications/drivers to deregister the envent handle call back
                    function.
    Notes:
    Returns:        TRUE: success; FALSE: failed
    -------------------------------------------------------------------------------*/
    BOOL StopAccelebootNotification(PFN_ACCELEBOOT_NOTIFICATION pfnFBNotification);


    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Function:       RequestCreateAccelebootSnapshot()
    Description:    OEM applications can use this API to request create the acceleboot
                    snapshot.
    -------------------------------------------------------------------------------*/
    BOOL RequestCreateAccelebootSnapshot(DWORD dwSnapshotIdx);

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Function:       GetBootImg()
    Description:    Get the img from which current boot session is.
    -------------------------------------------------------------------------------*/
    BOOT_IMG GetBootImg();

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Function:       GetPowerOffMode()
    Description:    Get the system mode the next power off will enter
    -------------------------------------------------------------------------------*/
    POWEROFF_MODE GetPowerOffMode();

#ifdef __cplusplus
}

#endif

#endif//#ifndef _ACCELE_BOOT_H_
