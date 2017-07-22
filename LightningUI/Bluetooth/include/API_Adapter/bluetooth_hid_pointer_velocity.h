/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           bluetooth_hid_pointer_velocity.h

DESCRIPTION:    Header for Bluetooth HID pointer velocity.

REVISION:       $Revision: #2 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_POINTER_VELOCITY_H
#define _BLUETOOTH_HID_POINTER_VELOCITY_H


#ifdef __cplusplus
extern "C" {
#endif


/* The control parameter that govern pointer acceleration.
   Specific with Android OS. 
   Refer to 
   - "/android/4.4/frameworks/native/include/input/VelocityControl.h"
   - "/android/4.4/frameworks/native/libs/input/VelocityControl.cpp"
   - "/android/4.4/frameworks/base/services/input/InputReader.h"
   Note: 
   (1) BT APP needs to take into consideration such acceleration so as
       to avoid the pointer floating unexpectedly. 
   (2) BT APP is recommended not to reach the 'lowThreshold' when sending HID report.
       Otherwise, BT APP may fail to know the exact movement of the pointer in the
       phone due to the acceleration. */
typedef struct
{
    /* A scale factor that is multiplied with the raw velocity deltas.
       Must be a positive value.
       Default is 1.0 (no scaling). */
    float scale;

    /* The scaled speed at which acceleration begins to be applied. 
       Must be a non-negative value.
       Default is 0.0 (no low threshold). */
    float lowThreshold;

    /* The scaled speed at which maximum acceleration is applied. 
       Must be a non-negative value greater than or equal to lowThreshold.
       Default is 0.0 (no high threshold). */
    float highThreshold;

    /* The acceleration factor.
       When the speed is above the low speed threshold, the velocity will scaled
       by an interpolated value between 1.0 and this amount. 
       Must be a positive greater than or equal to 1.0.
       Default is 1.0 (no acceleration). */
    float acceleration;
} VelocityControlParameters;

/* Pointer velocity information specific with remote phone. */
typedef struct
{
    /* Previous x-axis in remote phone. */
    LONG prevX;

    /* Previous y-axis in remote phone. */
    LONG prevY;

    /* Delta of x-axis in remote phone. */
    LONG deltaX;

    /* Delta of y-axis in remote phone. */
    LONG deltaY;

    /* Distance in pixel between current remote point and previous remote phone. */
    LONG distance;
} PointerVelocity;


#ifdef __cplusplus
}
#endif

#endif
