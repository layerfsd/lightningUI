#pragma once
#include "api_base.h"
#include "bluetooth_notification.h"
#include "bluetooth_notification_app_ext.h"
#include "bluetooth_handsfree.h"

#include <winsock2.h>
#include "svsutil.hxx"
#include <bt_api.h>
#include <bthapi.h>
#include <bt_buffer.h>
#include <bt_ddi.h>
#include <cmthread.h>


#define MAX_NUMBER_TYPE_LEN             5

#define TIMEOUT_MS_HF_API_CALL_LIST     1000

/* Max time in millisecond to transfer HFP audio. */
#define MAX_HFP_AUDIO_TRANSFER_TIME     10000


class CALLREC
{
public:
    enum CallRecordType
    {
        CRT_INVALID = 0,

        CRT_INCOMING,   //CRT_ANSWERED & CRT_REJECTED
        CRT_OUTGOING,
        CRT_MISSED,

        CRT_ALL,    //just for mmi display and clear
    };

    CALLREC()
    {
        HEAP_STATISTICS_CLASS_ALLOC(CALLREC)

        memset(this, 0, sizeof(*this));
    }
    CALLREC(CALLREC *pcr)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CALLREC)

        *this = *pcr;
    }
    CALLREC(FILETIME ft)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CALLREC)

        memset(this, 0, sizeof(*this));
        ftTime = ft;
    }
    CALLREC(BYTE tp)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CALLREC)

        memset(this, 0, sizeof(*this));
        type = tp;
    }
    CALLREC(FILETIME ft, BYTE tp)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CALLREC)

        memset(this, 0, sizeof(*this));
        ftTime = ft;
        type = tp;
    }
    ~CALLREC()
    {
        HEAP_STATISTICS_CLASS_FREE(CALLREC)
    }

    void SetType(BYTE callRecordType)   { type = callRecordType; }
    void SetPhoneNumber(LPCTSTR number) { if(number) StringCchCopy(szPhoneNumber, _countof(szPhoneNumber), number); }
    void SetName(LPCTSTR name)          { if(name) StringCchCopy(szName, _countof(szName), name); }
    void SetTime(SYSTEMTIME& stTime)    { if (ValidSystemTime(stTime)) SystemTimeToFileTime(&stTime, &ftTime); }
    void SetTimeElapse(DWORD second)    { dwTimeElapse = second; }
    void SetNumberType(LPCTSTR numType) { if(numType) StringCchCopy(szNumberType, _countof(szNumberType), numType); }

    BYTE GetType()                      { return type; }
    LPTSTR GetPhoneNumber()             { return (LPTSTR) szPhoneNumber; }
    LPTSTR GetName()                    { return (LPTSTR) szName; }
    FILETIME* GetTime()                 { return &ftTime; }
    DWORD GetTimeElapse()               { return dwTimeElapse; }
    LPTSTR GetNumberType()              { return (LPTSTR) szNumberType; }

    void GetTime(SYSTEMTIME& stTime)    
    {
        ZeroMemory(&stTime, sizeof(SYSTEMTIME));

        if (ValidFileTime(ftTime))
        {
            FileTimeToSystemTime(&ftTime, &stTime);
        }
    }

    void GetEndTime(SYSTEMTIME& stTime)
    {
        ZeroMemory(&stTime, sizeof(SYSTEMTIME));

        if (ValidFileTime(ftTime))
        {
            FILETIME endFileTime = ftTime;

            // FIXME
            endFileTime.dwLowDateTime += dwTimeElapse * 1000;

            FileTimeToSystemTime(&endFileTime, &stTime);
        }
    }

    static BOOL ValidSystemTime(SYSTEMTIME& stTime)
    {
        SYSTEMTIME zeroSystemTime;

        ZeroMemory(&zeroSystemTime, sizeof(SYSTEMTIME));

        return memcmp(&stTime, &zeroSystemTime, sizeof(SYSTEMTIME)) ? TRUE : FALSE;
    }

    static BOOL ValidFileTime(FILETIME& ftTime)
    {
        FILETIME zeroFileTime = {0, 0};

        return memcmp(&ftTime, &zeroFileTime, sizeof(FILETIME)) ? TRUE : FALSE;
    }

    void Dump();

public:
    CALLREC *pNext;

private:
    FILETIME ftTime;    //ULARGE_INTEGER
    DWORD dwTimeElapse; //in second
    TCHAR szPhoneNumber[MAX_PHONE_NUMBER];
    TCHAR szName[MAX_PHONE_NAME];
    TCHAR szNumberType[MAX_NUMBER_TYPE_LEN];
    BYTE type;
};

struct CCallRecList
{
    CALLREC *m_pcrHead;
    DWORD m_dwCntList;
    CRITICAL_SECTION m_cs;

    CCallRecList(): m_pcrHead(NULL), m_dwCntList(0)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CCallRecList)

        InitializeCriticalSection(&m_cs);
    }
    ~CCallRecList()
    {
        HEAP_STATISTICS_CLASS_FREE(CCallRecList)

        Free();
        DeleteCriticalSection(&m_cs);
    }
    void Lock() const
    {
        EnterCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }
    void Unlock() const
    {
        LeaveCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }

    //NOTE:default use the parameter pointer directly, don't free it in caller
    CALLREC *Add(CALLREC *pcr, BOOL bNew = FALSE);
    BOOL Remove(CALLREC *pcr, BOOL bDelete = TRUE);
    void Free();
    CALLREC *Find(CALLREC *pcr) const;
};

/* +++ HF API function pointer +++ */
typedef DWORD (WINAPI *PFN_HfmRegister)(HfmRegisterInfoT* registerInfo);
typedef DWORD (WINAPI *PFN_HfmUnregister)();
typedef DWORD (WINAPI *PFN_HfmMakeOutgoingCallPhoneNumber)(WCHAR* phoneNumber);
typedef DWORD (WINAPI *PFN_HfmMakeOutgoingCallLastNumber)();
typedef DWORD (WINAPI *PFN_HfmMakeOutgoingCallMemoryDial)(DWORD memoryNumber);
typedef DWORD (WINAPI *PFN_HfmAnswerCall)();
typedef DWORD (WINAPI *PFN_HfmRejectCall)();
typedef DWORD (WINAPI *PFN_HfmSendDTMF)(WCHAR character);
typedef DWORD (WINAPI *PFN_HfmTerminateCall)();
typedef DWORD (WINAPI *PFN_HfmTransferAudio)(HfmAudioDirectionT direction);
typedef DWORD (WINAPI *PFN_HfmSetSpeakerGain)(HfmGainT gain);
typedef DWORD (WINAPI *PFN_HfmSetMicrophoneGain)(HfmGainT gain);
typedef DWORD (WINAPI *PFN_HfmConnect)();
typedef DWORD (WINAPI *PFN_HfmDisconnect)();
typedef DWORD (WINAPI *PFN_HfmSetSelectedDevice)(BT_ADDR bluetoothAddress);
typedef DWORD (WINAPI *PFN_HfmGetSelectedDevice)(BT_ADDR* bluetoothAddress);
typedef DWORD (WINAPI *PFN_HfmThreewayCall)(HfmThreewayHandlerT threewayHandler, HfmCallIndexT callIndex);
typedef DWORD (WINAPI *PFN_HfmCallInfo)();
typedef DWORD (WINAPI *PFN_HfmDirectATCommand)(LPSTR command, BOOL standardResponse);
typedef DWORD (WINAPI *PFN_HfmQueryOperator)();
typedef DWORD (WINAPI *PFN_HfmActivateVoiceRecognition)(BOOL activate);
#ifdef ENABLE_API_IPHONE_SIRI_EFM
typedef DWORD (WINAPI *PFN_HfmAplEnableCustATcommand)(Aplx settings);
typedef DWORD (WINAPI *PFN_HfmAplSetEyesFreeMode)(BOOL enable);
typedef DWORD (WINAPI *PFN_HfmAplQuerySiriStatus)();
#endif
/* --- HF API function pointer --- */


class CApi_HF :
    public CApi_base
{
public:
    CApi_HF(void);
    /* Define the virtual de-constructor to free SfWE SDK. Otherwise memory leak occurs. */
    virtual ~CApi_HF(void);

    virtual BOOL Initialize();
    virtual void DeInitialize();
    virtual void PowerDown();
    virtual void PowerUp();
 
	/* Manage Bluetooth audio between HFP and A2DP/AVRCP. */
    void ManageBluetoothAudio(BOOL calling = TRUE);
    void NotifyAudioDirection(HfmAudioDirectionT audioDirection);
    BOOL TransferHFPAudioNeeded(DWORD transferDirection, HfmAudioDirectionT audioDirection);
    void TransferHFPAudio(DWORD transferDirection);
    void TransferHFPAudio();
    static HfmAudioDirectionT GetAudioDirection();
    static LPTSTR GetAudioDirectionString(DWORD direction);

    //HFM apis
    DWORD Register(HfmRegisterInfoT *registerInfo)
    {
        CHECK_FUNC_PTR(m_pfnHfmRegister);
        return m_pfnHfmRegister(registerInfo);
    }
    DWORD Unregister()
    {
        CHECK_FUNC_PTR(m_pfnHfmUnregister);
        return m_pfnHfmUnregister();
    }
    DWORD MakeOutgoingCallPhoneNumber(WCHAR *phoneNumber)
    {
        CHECK_FUNC_PTR(m_pfnHfmMakeOutgoingCallPhoneNumber);
        return m_pfnHfmMakeOutgoingCallPhoneNumber(phoneNumber);
    }
    DWORD MakeOutgoingCallLastNumber()
    {
        CHECK_FUNC_PTR(m_pfnHfmMakeOutgoingCallLastNumber);
        return m_pfnHfmMakeOutgoingCallLastNumber();
    }
    DWORD MakeOutgoingCallMemoryDial(DWORD memoryNumber)
    {
        CHECK_FUNC_PTR(m_pfnHfmMakeOutgoingCallMemoryDial);
        return m_pfnHfmMakeOutgoingCallMemoryDial(memoryNumber);
    }
    DWORD AnswerCall()
    {
        CHECK_FUNC_PTR(m_pfnHfmAnswerCall);
        return m_pfnHfmAnswerCall();
    }
    DWORD RejectCall()
    {
        CHECK_FUNC_PTR(m_pfnHfmRejectCall);
        return m_pfnHfmRejectCall();
    }
    DWORD SendDTMF(WCHAR character)
    {
        CHECK_FUNC_PTR(m_pfnHfmSendDTMF);
        return m_pfnHfmSendDTMF(character);
    }
    DWORD TerminateCall()
    {
        CHECK_FUNC_PTR(m_pfnHfmTerminateCall);
        return m_pfnHfmTerminateCall();
    }
    DWORD TransferAudio(HfmAudioDirectionT direction)
    {
        CHECK_FUNC_PTR(m_pfnHfmTransferAudio);
        return m_pfnHfmTransferAudio(direction);
    }
    DWORD SetSpeakerGain(HfmGainT gain)
    {
        CHECK_FUNC_PTR(m_pfnHfmSetSpeakerGain);
        return m_pfnHfmSetSpeakerGain(gain);
    }
    DWORD SetMicrophoneGain(HfmGainT gain)
    {
        CHECK_FUNC_PTR(m_pfnHfmSetMicrophoneGain);
        return m_pfnHfmSetMicrophoneGain(gain);
    }
    DWORD Connect()
    {
        CHECK_FUNC_PTR(m_pfnHfmConnect);
        return m_pfnHfmConnect();
    }
    DWORD Disconnect()
    {
        CHECK_FUNC_PTR(m_pfnHfmDisconnect);
        return m_pfnHfmDisconnect();
    }
    DWORD SetSelectedDevice(BT_ADDR bluetoothAddress)
    {
        CHECK_FUNC_PTR(m_pfnHfmSetSelectedDevice);
        return m_pfnHfmSetSelectedDevice(bluetoothAddress);
    }
    DWORD GetSelectedDevice(BT_ADDR *bluetoothAddress)
    {
        CHECK_FUNC_PTR(m_pfnHfmGetSelectedDevice);
        return m_pfnHfmGetSelectedDevice(bluetoothAddress);
    }
    DWORD ThreewayCall(HfmThreewayHandlerT threewayHandler, HfmCallIndexT callIndex)
    {
        CHECK_FUNC_PTR(m_pfnHfmThreewayCall);
        return m_pfnHfmThreewayCall(threewayHandler, callIndex);
    }
    DWORD CallInfo()
    {
        CHECK_FUNC_PTR(m_pfnHfmCallInfo);
        return m_pfnHfmCallInfo();
    }
    DWORD DirectATCommand(LPSTR command, BOOL standardResponse)
    {
        CHECK_FUNC_PTR(m_pfnHfmDirectATCommand);
        return m_pfnHfmDirectATCommand(command, standardResponse);
    }
    DWORD QueryOperator()
    {
        CHECK_FUNC_PTR(m_pfnHfmQueryOperator);
        return m_pfnHfmQueryOperator();
    }
    DWORD ActivateVoiceRecognition(BOOL activate)
    {
        CHECK_FUNC_PTR(m_pfnHfmActivateVoiceRecognition);
        return m_pfnHfmActivateVoiceRecognition(activate);
    }
#ifdef ENABLE_API_IPHONE_SIRI_EFM
    DWORD AppleEnableCustATCmd(Aplx settings)
    {
        CHECK_FUNC_PTR(m_pfnHfmAplEnableCustATcommand);
        return m_pfnHfmAplEnableCustATcommand(settings);
    }
    DWORD AppleSetEyesFreeMode(BOOL enable)
    {
        CHECK_FUNC_PTR(m_pfnHfmAplSetEyesFreeMode);
        return m_pfnHfmAplSetEyesFreeMode(enable);
    }
    DWORD AppleQuerySiriStatus()
    {
        CHECK_FUNC_PTR(m_pfnHfmAplQuerySiriStatus);
        return m_pfnHfmAplQuerySiriStatus();
    }
#endif

    // HFM callbacks
    static void WINAPI StaticHandsfreeAnswerCall();
    static void WINAPI StaticHandsfreeRejectCall();
    static void WINAPI StaticHandsfreeTalking();
    static void WINAPI StaticHandsfreeTerminateCall();
    static void WINAPI StaticHandsfreeBluetoothStatus(HfmBluetoothStatusT status);
    static void WINAPI StaticHandsfreeIncomingCall(BOOL generateRingtone);
    static void WINAPI StaticHandsfreeMakeOutgoingCall(DWORD result);
    static void WINAPI StaticHandsfreeTransferAudio(HfmAudioDirectionT direction);
    static void WINAPI StaticHandsfreeWaitingCall();
    static void WINAPI StaticHandsfreeThreewayCall(CallInfoT *callInfo , DWORD numberOfCalls, DWORD result);
    static void WINAPI StaticHandsfreeCallInfo(CallInfoT *callInfo , DWORD numberOfCalls, DWORD result);
    static void WINAPI StaticHandsfreeDirectATResponse(LPSTR response);
    static void WINAPI StaticHandsfreeQueryOperator(DWORD result, LPCTSTR operatorName);
    static void WINAPI StaticHandsfreeActivateVoiceRecognition(DWORD result, DWORD voiceRecognitionStatus);
#ifdef ENABLE_API_IPHONE_SIRI_EFM
    static void WINAPI StaticHandsfreeAppleEnableCustATCmd(DWORD result, LPCTSTR string, DWORD features);
    static void WINAPI StaticHandsfreeAppleEyesFreeMode(DWORD result);
    static void WINAPI StaticHandsfreeApplelQuerySiriStatus(DWORD result, DWORD status);
#endif

    //call records, in HLM\\software\\microsoft\\bluetooth\\device\\CallRecords, key is ftTime string(hex)
    BOOL EnumRecords();
    BOOL EnumUpdate(CALLREC *pcr);
    void EnumRemove(CALLREC *pcr);
    void EnumRemoveAll();

    BOOL InitFuncPtr();
    void DumpFuncPtr();

    //timer for getting current call list.
    static void UpdateCallListTimer(CallInfoT *callInfo, DWORD numberOfCalls, DWORD result);

    static void CALLBACK CallListTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

    static BOOL IsCallIdle();

    DWORD WaitConnected(DWORD timeout = INFINITE);
    void NotifyConnected(BOOL isConnected = TRUE);

    static BOOL IsValidPhoneNumber(LPTSTR phoneNumber)
    {
        size_t cch = 0;

        return phoneNumber && (StringCchLength(phoneNumber, MAX_PHONE_NUMBER, &cch) == S_OK) && cch ? TRUE : FALSE;
    }

    void SetCallEvent(BOOL fCallActive);
    BOOL IsCallActive()
    {
        return m_fCallActive;
    }

    static void DumpATCmd(char* atCmd);

    /* +++ Apple iPhone specific +++ */
    static BOOL IsAppleiPhoneATCmd(const char* atCmd);

    DWORD EnableBluetoothAccessoryIdentification();

    DWORD RetrieveSiriStatusInfomation();

    DWORD EnableSiriEyesFreeMode(BOOL enable = TRUE);
    /* --- Apple iPhone specific --- */

private:
    HANDLE m_hConnectionEvent;
    BOOL m_fCallActive;

    PFN_HfmRegister                       m_pfnHfmRegister;
    PFN_HfmUnregister                     m_pfnHfmUnregister;
    PFN_HfmMakeOutgoingCallPhoneNumber    m_pfnHfmMakeOutgoingCallPhoneNumber;
    PFN_HfmMakeOutgoingCallLastNumber     m_pfnHfmMakeOutgoingCallLastNumber;
    PFN_HfmMakeOutgoingCallMemoryDial     m_pfnHfmMakeOutgoingCallMemoryDial;
    PFN_HfmAnswerCall                     m_pfnHfmAnswerCall;
    PFN_HfmRejectCall                     m_pfnHfmRejectCall;
    PFN_HfmSendDTMF                       m_pfnHfmSendDTMF;
    PFN_HfmTerminateCall                  m_pfnHfmTerminateCall;
    PFN_HfmTransferAudio                  m_pfnHfmTransferAudio;
    PFN_HfmSetSpeakerGain                 m_pfnHfmSetSpeakerGain;
    PFN_HfmSetMicrophoneGain              m_pfnHfmSetMicrophoneGain;
    PFN_HfmConnect                        m_pfnHfmConnect;
    PFN_HfmDisconnect                     m_pfnHfmDisconnect;
    PFN_HfmSetSelectedDevice              m_pfnHfmSetSelectedDevice;
    PFN_HfmGetSelectedDevice              m_pfnHfmGetSelectedDevice;
    PFN_HfmThreewayCall                   m_pfnHfmThreewayCall;
    PFN_HfmCallInfo                       m_pfnHfmCallInfo;
    PFN_HfmDirectATCommand                m_pfnHfmDirectATCommand;
    PFN_HfmQueryOperator                  m_pfnHfmQueryOperator;
    PFN_HfmActivateVoiceRecognition       m_pfnHfmActivateVoiceRecognition;
#ifdef ENABLE_API_IPHONE_SIRI_EFM
    PFN_HfmAplEnableCustATcommand         m_pfnHfmAplEnableCustATcommand;
    PFN_HfmAplSetEyesFreeMode             m_pfnHfmAplSetEyesFreeMode;
    PFN_HfmAplQuerySiriStatus             m_pfnHfmAplQuerySiriStatus;
#endif

    //Thread to transfer HFP audio.
    class CHFPAudioThread;
    friend CHFPAudioThread;
    class CHFPAudioThread : public CMiniThread
    {
        public:
            CHFPAudioThread(CApi_HF* apiHF, HfmAudioDirectionT transferDirection);
            virtual ~CHFPAudioThread();

            virtual DWORD ThreadRun();

            void NotifyAudioDirection();

        private:
            CApi_HF* m_apiHF;
            HfmAudioDirectionT m_transferDirection;
            HANDLE m_hAudioDirectionEvent;
    };

    CHFPAudioThread* m_pHFPAudioThread;
};
