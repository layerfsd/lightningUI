/******************************************************************************
 CVTE Limited.

 Copyright (c) 2014 CVTE company.

 Author Wu haoheng 2015

 All rights reserved.

******************************************************************************/

/******************************************************************************
*
*	Module Name:
*
*		CVTE_AppProxy.h
*
*	Abstract:
*
*		Implement bsp oem sector operation functions
*
*------------------------------------------------------------------------------
*/

#ifndef _CVTE_GESTURE_PROXY_H_
#define _CVTE_GESTURE_PROXY_H_

#include "CVTE_IoCtl.h"
#define GESTURE_PROXY_DEVICE_NAME TEXT("PAC1:")

typedef struct _GES_ADJUST_DIRECTION_
{
    BYTE nDataX;
    BYTE nDataY;
    BYTE nDataZ;
}GesDirection_t;

typedef struct _GES_ADJUST_WAVE_
{
    BYTE nWaveCount;
    BYTE nWaveAngle;
}GesWave_t;

typedef struct _GES_OBJECT_INFO_
{
    DWORD dwPositionX;
    DWORD dwPositionY;
    UINT16 nSize;
    BYTE  nAvgY;
}GesObjectInfo_t;



__inline HANDLE GestureProxyOpen()
{
    HANDLE hDevice = CreateFile(GESTURE_PROXY_DEVICE_NAME,
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                FILE_FLAG_RANDOM_ACCESS,
                                NULL);

    return hDevice;
}

__inline VOID GestureProxyClose(HANDLE hDevice)
{
    if(hDevice)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }
}

__inline BOOL GestureProxySetOperationDistance(HANDLE hDevice, UINT16 nDistance)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_SET_OPERATION_DISTANCE, &nDistance, sizeof(nDistance), NULL, 0, NULL, NULL);
}

__inline BOOL GestureProxySetOperationSensitivity(HANDLE hDevice, GesDirection_t *pGesDirection)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_SET_OPERATION_SENSITIVITY, pGesDirection, sizeof(GesDirection_t), NULL, 0, NULL, NULL);
}

__inline BOOL GestureProxySetOperationWave(HANDLE hDevice, GesWave_t *pGesWave)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_SET_OPERATION_WAVE, pGesWave, sizeof(GesWave_t), NULL, 0, NULL, NULL);
}

__inline BOOL GestureProxySetOperationRotate(HANDLE hDevice, UINT16 nRotate)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_SET_OPERATION_ROTATE, &nRotate, sizeof(nRotate), NULL, 0, NULL, NULL);
}

__inline BOOL GestureProxySetOperationNoMotion(HANDLE hDevice, BYTE nNoMotion)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_SET_OPERATION_NOMOTION, &nNoMotion, sizeof(nNoMotion), NULL, 0, NULL, NULL);
}

__inline BOOL GestureProxySetOperationNoObject(HANDLE hDevice, BYTE nNoObject)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_SET_OPERATION_NOOBJECT, &nNoObject, sizeof(nNoObject), NULL, 0, NULL, NULL);
}

__inline BOOL GestureProxyGetOperationDistance(HANDLE hDevice, UINT16 *pDistance)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_GET_OPERATION_DISTANCE, NULL, 0, pDistance, sizeof(UINT16), NULL, NULL);
}

__inline BOOL GestureProxyGetOperationSensitivity(HANDLE hDevice, GesDirection_t *pGesDirection)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_GET_OPERATION_SENSITIVITY, NULL, 0, pGesDirection, sizeof(GesDirection_t), NULL, NULL);
}

__inline BOOL GestureProxyGetOperationWave(HANDLE hDevice, GesWave_t *pGesWave)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_GET_OPERATION_WAVE, NULL, 0, pGesWave, sizeof(GesWave_t), NULL, NULL);
}

__inline BOOL GestureProxyGetOperationRotate(HANDLE hDevice, UINT16 *pRotate)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_GET_OPERATION_ROTATE, NULL, 0, pRotate, sizeof(UINT16), NULL, NULL);
}

__inline BOOL GestureProxyGetOperationNoMotion(HANDLE hDevice, BYTE *pNoMotion)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_GET_OPERATION_NOMOTION, NULL, 0, pNoMotion, sizeof(BYTE), NULL, NULL);
}

__inline BOOL GestureProxyGetOperationNoObject(HANDLE hDevice, BYTE *pNoObject)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_GET_OPERATION_NOOBJECT, NULL, 0, pNoObject, sizeof(BYTE), NULL, NULL);
}

__inline BOOL GestureProxyGetObjectInfo(HANDLE hDevice, GesObjectInfo_t *pGesObjectInfo)
{
    return DeviceIoControl(hDevice, PAC_IOCTL_GET_OBJECT_INFO, NULL, 0, pGesObjectInfo, sizeof(GesObjectInfo_t), NULL, NULL);
}

#endif
