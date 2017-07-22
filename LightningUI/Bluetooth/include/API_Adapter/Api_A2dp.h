/*************************************************************

                 CSR plc 2011

                 All rights reserved

FILE:            Api_A2dp.h

DESCRIPTION:     Header of A2DP API

REVISION:        $Revision: #1 $

************************************************************/
#pragma once
#ifndef A2DP_API_H
#define A2DP_API_H

#include <ws2bth.h>
#include "api_base.h"
#include "module\a2dp\a2dp_sink.h"


class CApi_A2dp : public CApi_base
{
public:
    CApi_A2dp();
    /* Define the virtual de-constructor to free SfWE SDK. Otherwise memory leak occurs. */
    virtual ~CApi_A2dp();

	virtual BOOL Initialize();
	virtual void DeInitialize();
    DWORD Connect(BT_ADDR bluetoothAddress);
    //DWORD Disconnect(BT_ADDR bluetoothAddress);
    DWORD Disconnect();

	DWORD StartStreaming();
	DWORD StopStreaming();
	BOOL  IsStreamingOn();

	DWORD MuteA2dpStream(BOOL bMute);
    /* Get Bluetooth device connected with A2DP last time. */
    DWORD GetSelectedDevice(BT_ADDR* bluetoothAddress);

    /* Override CApi_base */
    WORD GetConnectionStatus();
	void SetConnectionStatus(AvsmStatusT status);

    DWORD InitAvrcpConnect;
	
    DWORD WaitConnected(DWORD timeout = INFINITE);
    void NotifyConnected(BOOL isConnected = TRUE);

    BOOL IsConnected();

private:
    BOOL m_fInit;
	HANDLE m_hConnectionEvent;
};


#endif /* A2DP_API_H */




