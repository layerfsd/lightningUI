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
*  @file spp_device.h
*
*  Abstract
*           - Header for Bluetooth SPP(Serial Port Profile) device.
*
*/

#ifndef  _SPP_DEVICE_H_
#define _SPP_DEVICE_H_

#include <windows.h>
#include <cmthread.h>
#include "bluetooth_spp.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define DBG_SPP_INIT                    1
#define DBG_SPP_INFO                    1
#define DBG_SPP_OUTPUT                  1
#define DBG_SPP_DUMP                    1
#define DBG_SPP_WARN                    1
#define DBG_SPP_ERROR                   1
#else
#define DBG_SPP_INIT                    0
#define DBG_SPP_INFO                    0
#define DBG_SPP_OUTPUT                  0
#define DBG_SPP_DUMP                    0
#define DBG_SPP_WARN                    1
#define DBG_SPP_ERROR                   1
#endif

#undef CHK_RES
#define CHK_RES(res, msg)              { \
                                            if ((res) != ERROR_SUCCESS) \
                                            { \
                                                RETAILMSG(DBG_SPP_ERROR, (TEXT("<%s> %s fail\r\n"), _T(__FUNCTION__), (msg))); \
                                                break; \
                                            } \
                                        }

#undef IS_SUCCESS
#define IS_SUCCESS(res)                 ((res) == ERROR_SUCCESS)

#define BT_APP_REG                      TEXT("Software\\CSR\\Applications\\WPPAPP")
#define BT_APP_REG_LIB_NAME             TEXT("wpplib")

#define DEFAULT_BT_SPP_DLL              TEXT("btp_ui_ext.dll")

#define DEFAULT_SPP_DATA_BUF_SIZE       1024

#define DEFAULT_SPP_BAUD_RATE           115200
#define DEFAULT_SPP_IN_QUEUE_SIZE       0x4000          /* byte */
#define DEFAULT_SPP_OUT_QUEUE_SIZE      0x4000          /* byte */

#define DEFAULT_SPP_WAIT_TIMEOUT        10000           /* ms */


typedef struct
{
    BYTE* buffer;
    DWORD totalSize;
    DWORD length;
} SppDataBufferT;

typedef void (WINAPI *SppCbDataIndT)(PBYTE buffer, DWORD bufferSize);

class CSppDevice
{
    public:
        CSppDevice();
        virtual ~CSppDevice();

        virtual DWORD Init(DWORD sppIndex, SppCbStateT pfnStateCallback, SppCbDataIndT pfnDataIndHandler);
        virtual DWORD Deinit();

        DWORD Connect();
        DWORD Connect(BT_ADDR bluetoothAddress);
        DWORD Connect(SppConnectInfo* connectInfo);
        DWORD Disconnect();

        virtual DWORD SendData(LPCVOID lpBuffer, DWORD bufferSize, DWORD* bytesWritten);

        virtual DWORD IssueRequest(PBYTE req, DWORD reqSize, PBYTE* response, DWORD* responseSize)
        {
            return ERROR_CALL_NOT_IMPLEMENTED;
        }

        void Lock();
        void Unlock();

        static void DumpData(unsigned char* lpBuffer, unsigned int cBuffer);

    protected:
        BOOL InitAPI();
        void DeinitAPI();

        virtual HANDLE Open(LPCTSTR name, DWORD dwDesiredAccess, DWORD dwShareMode);
        virtual BOOL Open();
        virtual void Close();

        DWORD WaitOnEvent(HANDLE event, DWORD timeout = DEFAULT_SPP_WAIT_TIMEOUT);

        void UpdateState(DWORD connectionIndex, BT_ADDR bluetoothAddress, SppStateT state, DWORD result);

        void NotifyConnection();

        void NotifyRead();

        static void SppStateCallback(DWORD connectionIndex, BT_ADDR bluetoothAddress, SppStateT state, DWORD result);

        static BOOL ValidConnectionIndex(DWORD connectionIndex)
        {
            return (connectionIndex < BLUETOOTH_SPP_MAX_CONNECTION) ? TRUE :FALSE;
        }

        static DWORD StaticSppThreadStub(class CSppDevice* sppDevice);
        static DWORD StaticAutoConnectThreadStub(class CSppDevice* sppDevice);

        DWORD ThreadRun(); 
        DWORD AutoConnectThreadRun();

        void NotifyAutoConnect();

        virtual BOOL IsAutoConnectNeeded();

    protected:
        DWORD m_sppIndex;
        SppCbStateT m_pfnStateCallback;
        SppCbDataIndT m_pfnDataIndHandler;
        CRITICAL_SECTION m_lock;
        DWORD m_actionResult;

        HANDLE m_sppHandle;
        /* Handle for Bluetooth SPP R/W operation. */
        HANDLE m_file;
        HANDLE m_connectionEvent;
        BOOL m_connected;
        HANDLE m_readEvent;
        HANDLE m_actionEvent;
        SppDataBufferT m_readBuf;
        SppServiceName m_localServiceName;

        /* SPP dll. */
        HINSTANCE m_hDll;
        /* + Function pointer for Bluetooth SPP API */
        SppInitT                    m_pfnSppInit;
        SppDeinitT                  m_pfnSppDeinit;
        SppConnectT                 m_pfnSppConnect;
        SppDisconnectT              m_pfnSppDisconnect;
        SppGetLocalServiceNameT     m_pfnSppGetLocalServiceName;
        /* - Function pointer for Bluetooth SPP API */

        /* SPP read thread. */
        HANDLE m_thread;
        BOOL m_terminated;
        HANDLE m_autoConnectThread;
        HANDLE m_autoConnectEvent;

        BT_ADDR m_bluetoothAddress;
};


#ifdef __cplusplus
}
#endif

#endif  // _SPP_DEVICE_H_
