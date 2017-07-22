#pragma once
#include "api_base.h"
#include "bluetooth_opps.h"
#include "ring_buffer.h"

/* +++ OPPS API function pointer +++ */
typedef DWORD (WINAPI *PFN_OpsmRegister)(OpsmRegisterInfoT* registerInfo);
typedef DWORD (WINAPI *PFN_OpsmUnregister)();
typedef DWORD (WINAPI *PFN_OpsmAcceptConnect)(BOOL isAcceptRequest);
typedef DWORD (WINAPI *PFN_OpsmAbort)(DWORD errorCode);
/* --- OPPS API function pointer --- */



class CApi_Opps	: public CApi_base
{
public:
    CApi_Opps(void);
    virtual ~CApi_Opps(void);
	
    virtual BOOL Initialize();
    virtual void DeInitialize();

	//Opps api
    DWORD Register(OpsmRegisterInfoT* registerInfo)
    {
		CHECK_FUNC_PTR(m_pfnOpsmRegister);
        return m_pfnOpsmRegister(registerInfo);
    }
    DWORD Unregister()
    {
		CHECK_FUNC_PTR(m_pfnOpsmUnregister);
        return m_pfnOpsmUnregister();
    }
    DWORD AcceptConnect(BOOL isAcceptRequest)
    {
		CHECK_FUNC_PTR(m_pfnOpsmAcceptConnect);
        return m_pfnOpsmAcceptConnect(isAcceptRequest);
    }
    DWORD Abort(DWORD errorCode)
    {
		CHECK_FUNC_PTR(m_pfnOpsmAbort);
        return m_pfnOpsmAbort(errorCode);
    }
	
	//Opps callback
	static void WINAPI staticBluetoothStatusChange(LPVOID userdata, ModuleApiBluetoothStatus status);
    static void WINAPI staticOpsmCbStatus(LPVOID userdata, OpsmStatusT status);
    static void WINAPI staticOpsmCbConnect(LPVOID userdata, BT_ADDR btAddr);
    static void WINAPI staticOpsmCbPush(LPVOID userdata, OpsmObjectTypeT type, LPTSTR typeString, LPTSTR objectName, DWORD objectSize);
    static void WINAPI staticOpsmCbPushPacket(LPVOID userdata, BOOL isFinalPacket, PBYTE data, DWORD dataSize);
    static void WINAPI staticOpsmCbAbort(LPVOID userdata, OpsmResultT errorCode);

	BOOL InitFuncPtr();
    void DumpFuncPtr();
	
	RingBuffer m_buffer;
	DWORD DecodeVCard();

	BOOL m_bStopDecode;
	HANDLE m_hevDecode;
	HANDLE m_hDecode;
	static DWORD Decode_vcard_thread(CApi_Opps *pApiOpps)
	{
        return pApiOpps->Thread_run_Decode_vacrd();
    }
    DWORD Thread_run_Decode_vacrd();
	
	void StartDecode();
	void StopDecode();
	
	WORD GetConnectionStatus()						{ return (WORD)m_status; }
	void SetConnectionStatus(OpsmStatusT status)	{ m_status = status; }
	
private:
    PFN_OpsmRegister					m_pfnOpsmRegister;
    PFN_OpsmUnregister					m_pfnOpsmUnregister;
    PFN_OpsmAcceptConnect				m_pfnOpsmAcceptConnect;
    PFN_OpsmAbort						m_pfnOpsmAbort;

	OpsmStatusT m_status;

};