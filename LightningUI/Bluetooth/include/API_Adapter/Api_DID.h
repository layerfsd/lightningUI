#pragma once
#include "api_base.h"
#include "bluetooth_did.h"
// #include "afxwin.h"

#define BLUETOOTH_DID_REQUEST_INFO_EVENT	TEXT("/bluetooth/did/request/info/event")
#define BLUETOOTH_DID_RECEIVED_INFO_EVENT	TEXT("/bluetooth/did/received/info/event")
#define BT_DID_REG                              TEXT("Software\\CSR\\Bluetooth\\Device\\Did")

/* +++ OPPS API function pointer +++ */
typedef DWORD (WINAPI *PFN_DidRegister)(DidmRegisterInfoT* registerInfo);
typedef DWORD (WINAPI *PFN_DidUnregister)();
typedef DWORD (WINAPI *PFN_DidReadDiServiceRecord)(BT_ADDR bluetoothAddress);
typedef DWORD (WINAPI *PFN_DidCancelReadDiServiceRecord)();
/* --- OPPS API function pointer --- */



class CApi_Did	: public CApi_base
{
public:
    CApi_Did(void);
    virtual ~CApi_Did(void);
	
    virtual BOOL Initialize();
    virtual void DeInitialize();

	BOOL InitFuncPtr();
    void DumpFuncPtr();

	//Did api
    DWORD Register(DidmRegisterInfoT* registerInfo)
    {
		CHECK_FUNC_PTR(m_pfnDidRegister);
        return m_pfnDidRegister(registerInfo);
    }
    DWORD Unregister()
    {
		CHECK_FUNC_PTR(m_pfnDidUnregister);
        return m_pfnDidUnregister();
    }
    DWORD DidReadDiServiceRecord(BT_ADDR bluetoothAddress)
    {
		CHECK_FUNC_PTR(m_pfnDidReadDiServiceRecord);
        return m_pfnDidReadDiServiceRecord(bluetoothAddress);
    }
    DWORD DidCancelReadDiServiceRecord()
    {
		CHECK_FUNC_PTR(m_pfnDidCancelReadDiServiceRecord);
        return m_pfnDidCancelReadDiServiceRecord();
    } 


	//Did callbacks
	static void WINAPI staticDidmCbReadServiceRecordInd(DWORD result, DiServiceRecordV13* diServiceRecord);
    static void WINAPI staticDidmCbReadServiceRecordCfm(DWORD result);

private:
    PFN_DidRegister 					m_pfnDidRegister;
    PFN_DidUnregister					m_pfnDidUnregister;
    PFN_DidReadDiServiceRecord			m_pfnDidReadDiServiceRecord;
    PFN_DidCancelReadDiServiceRecord	m_pfnDidCancelReadDiServiceRecord;

	HANDLE m_hAppExitEvent;

	HANDLE m_hThread;//Thread for listening IPC request bluetooth did info
	static BT_ADDR m_addrOfConnectedSppDevice;
	static HANDLE m_hReceivedDidInfoEvent;//Event for telling the IPC that we have already prepared the did info.
	static bool m_fRequestDidInfoFromIPC;//if IPC want the Bluetooth Did info;
	static DWORD staticIPCRequestDidInfoEventHandlerThread(CApi_Did *instance);

	static bool staticCopyRegValToRegDid(HKEY hk);

};