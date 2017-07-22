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

#ifndef _CVTE_APPPROXY_H_
#define _CVTE_APPPROXY_H_

#include "CVTE_IoCtl.h"

#define OEMPROXY_DEVICE_NAME TEXT("OPD1:")
#define UPDPROXY_DEVICE_NAME TEXT("UPD1:")

__inline HANDLE UPDProxyOpen()
{
    HANDLE hDevice = CreateFile(UPDPROXY_DEVICE_NAME,
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                FILE_FLAG_RANDOM_ACCESS,
                                NULL);

    return hDevice;
}

__inline VOID UPDProxyClose(HANDLE hDevice)
{
    if(hDevice)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }
}

__inline BOOL UPDProxyDownloadEboot(HANDLE hDevice, LPVOID lpVID, DWORD dwLen)
{
	return DeviceIoControl(hDevice, IOCTL_UPD_STORE_EBOOT, lpVID, dwLen, NULL, 0, NULL, NULL);
}

__inline BOOL UPDProxyDownloadLogo(HANDLE hDevice, LPVOID lpVID, DWORD dwLen)
{
	return DeviceIoControl(hDevice, IOCTL_UPD_STORE_LOGO, lpVID, dwLen, NULL, 0, NULL, NULL);
}

__inline BOOL UPDProxyDownloadImage(HANDLE hDevice, LPVOID lpVID, DWORD dwLen)
{
	return DeviceIoControl(hDevice, IOCTL_UPD_STORE_IMAGE, lpVID, dwLen, NULL, 0, NULL, NULL);
}

__inline BOOL UPDProxyDownloadRes(HANDLE hDevice, LPVOID lpVID, DWORD dwLen)
{
	return DeviceIoControl(hDevice, IOCTL_UPD_RES, lpVID, dwLen, NULL, 0, NULL, NULL);
}



__inline HANDLE AppProxyOpen()
{
    HANDLE hDevice = CreateFile(OEMPROXY_DEVICE_NAME,
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                FILE_FLAG_RANDOM_ACCESS,
                                NULL);

    return hDevice;
}

__inline VOID AppProxyClose(HANDLE hDevice)
{
    if(hDevice)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }
}

__inline BOOL AppProxyGetSWID(HANDLE hDevice, LPVOID lpVID, DWORD dwLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_VERSION_ID, NULL, 0, lpVID, dwLen, NULL,NULL);
}

__inline BOOL AppProxyGetUUID(HANDLE hDevice, PDWORD pdwUUID, DWORD dwLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_HW_UUID, NULL, 0, pdwUUID, dwLen, NULL,NULL);
}

__inline BOOL AppProxyGetWholeOem(HANDLE hDevice, LPVOID pOemsector, DWORD dwLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_WHOLE_OEMSECTOR, NULL, 0,	pOemsector, dwLen, NULL, NULL);
}

__inline BOOL AppProxyGetUpdateType(HANDLE hDevice, LPVOID lpVID, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_UPDATE_TYPE, NULL, 0, lpVID, nLen, NULL,NULL);
}

__inline BOOL AppProxyGetTraceType(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_TRACE_TYPE, NULL, 0, pValue, nLen, NULL,NULL);
}

__inline BOOL AppProxyGetPowerStatus(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_POWER_STATUS, NULL, 0, pValue, nLen, NULL,NULL);
}

__inline BOOL AppProxyGetRVChannel(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_RV_CHANNEL, NULL, 0, pValue, nLen, NULL,NULL);
}

__inline BOOL AppProxyGetLCDGamma(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_LCD_GAMMA, NULL, 0, pValue, nLen, NULL,NULL);
}

__inline BOOL AppProxyGetColorSignal(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_COLOR_SIGNAL, NULL, 0, pValue, nLen, NULL,NULL);
}

__inline BOOL AppProxyGetMouseCursor(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_FUNCTION_MOUSE_CURSOR, NULL, 0, pValue, nLen, NULL,NULL);
}

__inline BOOL AppProxyGetDynamicLogo(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_GET_FUNCTION_DYNAMIC_LOGO, NULL, 0, pValue, nLen, NULL,NULL);
}

__inline BOOL AppProxySetWholeOem(HANDLE hDevice, LPVOID pOemsector, DWORD dwLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_SET_WHOLE_OEMSECTOR, pOemsector, dwLen, NULL, 0, NULL, NULL);
}

__inline BOOL AppProxySetUpdateFlag(HANDLE hDevice)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_SET_UPDATE_FLAG, NULL, 0, NULL, 0, NULL, NULL);
}

__inline BOOL AppProxySetUpdateFont(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_SET_UPDATE_FONT_FLAG, pValue, nLen, NULL, 0, NULL, NULL);
}

__inline BOOL AppProxySetPowerStatus(HANDLE hDevice, PBYTE pValue, BYTE nLen)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_SET_POWER_STATUS, pValue, nLen, NULL, 0, NULL, NULL);
}

__inline BOOL AppProxyClearUpdateFlag(HANDLE hDevice)
{
    return DeviceIoControl(hDevice, OEMCONFIG_IOCTL_CLEAR_UPDATE_FLAG, NULL, 0, NULL, 0, NULL, NULL);
}

#endif
