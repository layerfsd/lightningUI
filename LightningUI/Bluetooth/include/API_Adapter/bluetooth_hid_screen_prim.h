/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bluetooth_hid_screen_prim.h

DESCRIPTION:    Primitive definition for Bluetooth HID screen.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_SCREEN_PRIM_H
#define _BLUETOOTH_HID_SCREEN_PRIM_H

#include <windows.h>
#include <wingdi.h>
#include "bluetooth_hid_pointer_velocity.h"


typedef unsigned short  HidScreenPrimT;
typedef unsigned short  HidScreenResultT;

#define HidScreenDirectionT             ScreenDirectionT

#define HID_SCREEN_RESULT_SUCCESS       ((HidScreenResultT) 0x0)
#define HID_SCREEN_RESULT_ERROR         ((HidScreenResultT) 0x1)
#define HID_SCREEN_RESULT_INVALID_REQ   ((HidScreenResultT) 0x2)
#define HID_SCREEN_RESULT_TIMEOUT       ((HidScreenResultT) 0x3)

typedef unsigned short HdmiStatusT;

/* HDMI status */
#define HDMI_STATUS_IDLE                ((HdmiStatusT) 0x0)
#define HDMI_STATUS_CONNECTED           ((HdmiStatusT) 0x1)
#define HDMI_STATUS_ACTIVE              ((HdmiStatusT) 0x2)


/*******************************************************************************
 * Primitive definitions
 *******************************************************************************/

#define BT_HID_PRIM_DOWNSTREAM_LOWEST                   (0x0000)

#define BT_HID_GET_SCREEN_RESOLUTION_REQ                ((HidScreenPrimT) (0x0000 + BT_HID_PRIM_DOWNSTREAM_LOWEST))
#define BT_HID_GET_SCREEN_DIRECTION_REQ                 ((HidScreenPrimT) (0x0001 + BT_HID_PRIM_DOWNSTREAM_LOWEST))
#define BT_HID_GET_POINTER_MOVEMENT_UNIT_REQ            ((HidScreenPrimT) (0x0002 + BT_HID_PRIM_DOWNSTREAM_LOWEST))
#define BT_HID_START_SCREEN_CALIBRATION_REQ             ((HidScreenPrimT) (0x0003 + BT_HID_PRIM_DOWNSTREAM_LOWEST))
#define BT_HID_STOP_SCREEN_CALIBRATION_REQ              ((HidScreenPrimT) (0x0004 + BT_HID_PRIM_DOWNSTREAM_LOWEST))
#define BT_HID_SHOW_CALIBRATION_SCREEN_REQ              ((HidScreenPrimT) (0x0005 + BT_HID_PRIM_DOWNSTREAM_LOWEST))
#define BT_HID_GET_VELOCITY_CONTROL_PARAMETERS_REQ      ((HidScreenPrimT) (0x0006 + BT_HID_PRIM_DOWNSTREAM_LOWEST))
#define BT_HID_NOTIFY_HDMI_STATUS_IND                   ((HidScreenPrimT) (0x0007 + BT_HID_PRIM_DOWNSTREAM_LOWEST))

#define BT_HID_PRIM_DOWNSTREAM_HIGHEST                  (0x0007 + BT_HID_PRIM_DOWNSTREAM_LOWEST)


/*******************************************************************************/

#define BT_HID_PRIM_UPSTREAM_LOWEST                     (0x8000)

#define BT_HID_GET_SCREEN_RESOLUTION_CFM                ((HidScreenPrimT) (0x0000 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_GET_SCREEN_RESOLUTION_IND                ((HidScreenPrimT) (0x0001 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_GET_SCREEN_DIRECTION_CFM                 ((HidScreenPrimT) (0x0002 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_GET_SCREEN_DIRECTION_IND                 ((HidScreenPrimT) (0x0003 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_GET_POINTER_MOVEMENT_UNIT_CFM            ((HidScreenPrimT) (0x0004 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_GET_POINTER_MOVEMENT_UNIT_IND            ((HidScreenPrimT) (0x0005 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_START_SCREEN_CALIBRATION_CFM             ((HidScreenPrimT) (0x0006 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_STOP_SCREEN_CALIBRATION_CFM              ((HidScreenPrimT) (0x0007 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_SHOW_CALIBRATION_SCREEN_CFM              ((HidScreenPrimT) (0x0008 + BT_HID_PRIM_UPSTREAM_LOWEST))
#define BT_HID_GET_VELOCITY_CONTROL_PARAMETERS_CFM      ((HidScreenPrimT) (0x0009 + BT_HID_PRIM_UPSTREAM_LOWEST))

#define BT_HID_PRIM_UPSTREAM_HIGHEST                    (0x0009 + BT_HID_PRIM_UPSTREAM_LOWEST)

#define BT_HID_PRIM_DOWNSTREAM_COUNT                    (BT_HID_PRIM_DOWNSTREAM_HIGHEST + 1 - BT_HID_PRIM_DOWNSTREAM_LOWEST)
#define BT_HID_PRIM_UPSTREAM_COUNT                      (BT_HID_PRIM_UPSTREAM_HIGHEST + 1 - BT_HID_PRIM_UPSTREAM_LOWEST)

/*******************************************************************************
 * End primitive definitions
 *******************************************************************************/

#pragma pack(1)


typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
} HidCommonReqT;

typedef HidCommonReqT   HidGetScreenResolutionReqT;

typedef HidCommonReqT   HidGetScreenDirectionReqT;

typedef HidCommonReqT   HidGetPointerMovementUnitReqT;

typedef HidCommonReqT   HidStartScreenCalibrationReqT;

typedef HidCommonReqT   HidStopScreenCalibrationReqT;

typedef HidCommonReqT   HidGetVelocityControlParametersReqT;

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    WORD                    screenDir;                                  /* Screen direction                                 */
    COLORREF                color;                                      /* Color in RGB to fill with calibration screen     */
} HidShowCalibrationScreenReqT;

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    HdmiStatusT             hdmiStatus;                                 /* HDMI connection status.                          */
} HidNotifyHdmiStatusIndT;


/*******************************************************************************/

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    HidScreenResultT        result;                                     /* Result for HID request                           */
} HidCommonCfmT;

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    HidScreenResultT        result;                                     /* Result for HID request                           */
    WORD                    screenWidth;                                /* Screen width in pixel                            */
    WORD                    screenHieght;                               /* Screen height in pixel                           */
} HidGetScreenResolutionCfmT;

typedef HidGetScreenResolutionCfmT      HidGetScreenResolutionIndT;

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    HidScreenResultT        result;                                     /* Result for HID request                           */
    WORD                    screenDir;                                  /* Screen direction                                 */
} HidGetScreenDirectionCfmT;

typedef HidGetScreenDirectionCfmT       HidGetScreenDirectionIndT;

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    HidScreenResultT        result;                                     /* Result for HID request                           */
    WORD                    pixelUnit;                                  /* Pointer movement unit in pixel                   */
} HidGetPointerMovementUnitCfmT;

typedef HidGetPointerMovementUnitCfmT   HidGetPointerMovementUnitIndT;

typedef HidCommonCfmT                   HidStartScreenCalibrationCfmT;

typedef HidCommonCfmT                   HidStopScreenCalibrationCfmT;

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    HidScreenResultT        result;                                     /* Result for HID request                           */
    WORD                    screenDir;                                  /* Screen direction                                 */
    COLORREF                color;                                      /* Color in RGB to fill with calibration screen     */
} HidShowCalibrationScreenCfmT;

typedef struct
{
    HidScreenPrimT          type;                                       /* Primitive / Message identity                     */
    HidScreenResultT        result;                                     /* Result for HID request                           */
    VelocityControlParameters parameters;                               /* Pointer velocity control parameters. */    
} HidGetVelocityControlParametersCfmT;


#pragma pack()

#endif /* _BLUETOOTH_HID_SCREEN_PRIM_H */
