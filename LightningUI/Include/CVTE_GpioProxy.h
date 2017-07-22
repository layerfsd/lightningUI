
#ifndef _CVTE_GPIO_PROXY_H_
#define _CVTE_GPIO_PROXY_H_

#include "CVTE_IoCtl.h"

#ifndef GIO_DEVICE_NAME
#define GIO_DEVICE_NAME                 TEXT("GPD1:")	//L"GPD1:"
#endif

//function to export to APP
__inline HANDLE GPIOOpen()
{
    HANDLE hDevice = INVALID_HANDLE_VALUE;
    hDevice = CreateFile(GIO_DEVICE_NAME,
                         GENERIC_READ|GENERIC_WRITE,          // desired access
                         FILE_SHARE_READ|FILE_SHARE_WRITE,    // sharing mode
                         NULL,                                // security attributes (ignored)
                         OPEN_EXISTING,                       // creation disposition
                         FILE_FLAG_RANDOM_ACCESS,             // flags/attributes
                         NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        RETAILMSG(1, (TEXT("Open %s device error!\r\n"),GIO_DEVICE_NAME));
    }
    return hDevice;
}
__inline VOID  GPIOClose(HANDLE hDevice)
{
    if (NULL != hDevice)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }
}
__inline BOOL  GPIOOutputEnable(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_OUTPUT_ENABLE, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIOOutputDisable(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_OUTPUT_DISABLE, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIOOutputOne(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_OUTPUT_ONE, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIOOutputZero(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_OUTPUT_ZERO, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIOPadEnable(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_PAD_EN_SET, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIOPadDisable(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_PAD_EN_CLR, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIORegRead(HANDLE hDevice, DWORD id, PDWORD pValue)
{
    DWORD DataOut = 0;
    return DeviceIoControl(hDevice, GPIO_IOCTL_READ_INPUT,  &id, sizeof(id), pValue, sizeof(DWORD), &DataOut, NULL);
}

__inline BOOL  GPIOPullUp(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_PULL_UP, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIOPullDown(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_PULL_DOWN, &id, sizeof(id), NULL, 0, NULL, NULL);
}

__inline BOOL  GPIOClearPull(HANDLE hDevice, DWORD id)
{
    return DeviceIoControl(hDevice, GPIO_IOCTL_CLR_PULL, &id, sizeof(id), NULL, 0, NULL, NULL);
}


#endif




