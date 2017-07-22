#pragma once
#include "api_base.h"
#include "bluetooth_phonebook.h"
#include "util\phonebook\poom.h"
#include "util\phonebook\database.h"
#include "util\phonebook\phonebook_contact.h"
#include <cmthread.h>


/* [DebugOnly] Measure PBAP performance. */
#define DEBUG_PBAP_PERFORMANCE

/* Delay to enumerate phonebook in millisecond. */
#define ENUM_PHONEBOOK_DELAY                3000

/* Contact count to determine when to add the delay to wait for contact flushed into database. */
#define FLUSH_DB_CONTACT_COUNT              10

#define WPP_PHONEBOOK_REG_PATH              TEXT("SOFTWARE\\CSR\\Applications\\Phonebook")
#define WPP_PHONEBOOK_LIB_KEY               TEXT("pblib")
#define WPP_PHONEBOOK_DEFAULT_DLL_NAME      TEXT("btp_ui_ext_pb.dll")

/* Registry key for PhonebookInfo: PBAP or LegacyPB. */
#define PHONEBOOK_INFO_KEY                  TEXT("PhonebookInfo")
#define PHONEBOOK_SERVICE_PBAP              TEXT("PBAP")
#define PHONEBOOK_SERVICE_LEGACY_PB         TEXT("LegacyPB")

/* Registry key for phonebook entry. */
#define PB_ENTRY_KEY                        TEXT("pb_entry")
#define DEFAULT_PHONEBOOK_ENTRY_NUMBER      50
#define MAX_PHONEBOOK_ENTRY_NUMBER          0xFFFF

/* Registry key for phonebook format: vcard2.1 or vcard3.0. */
#define PB_FORMAT_KEY                       TEXT("pb_format")

/* Registry key for using local phonebook. */
#define USE_LOCAL_PHONEBOOK_KEY             TEXT("use_local_phonebook")

/* Registry key for vcard dll. */
#define VCARD_DLL_KEY                       TEXT("vcard_dll")
#define DEFAULT_VCARD_DLL                   TEXT("csr_bt_vcard.dll")

/* [DebugOnly] Registry key setting to dump phonebook. */
#define PB_DUMP_MODE_KEY                    TEXT("dump_mode")
#define PB_DUMP_PATH_KEY                    TEXT("dump_path")

/* Phonebook dump mode. */
#define PB_DUMP_MODE_NONE                   0
#define PB_DUMP_MODE_CONSOLE                1
#define PB_DUMP_MODE_FILE                   2

/* Default dump file name for phonebook. */
#define PB_DEFAULT_DUMP_PATH                TEXT("\\Temp")
#define PB_DEFAULT_DUMP_FILE                TEXT("pb.vcf")

#define MAX_NAME_LENGTH                     60
#define MAX_EMAIL_LENGTH                    250
#define MAX_ADDR_LENGTH                     250
#define MAX_LENGTH                          250

#define MAX_PHONEBOOK_NUMBER                0xFFFF

/* Macro which complies with vcard spec. */
#define VCARD_BEGIN_STRING                  "BEGIN:VCARD"
#define VCARD_END_STRING                    "END:VCARD"

/* Default flags for CSR Synergy vcard parser. */
#define CSR_BT_VCARD_FLAGS                  (CSR_BT_VCARD_FLAGS_UCS2_STRINGS | \
                                             CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ATTRIBUTE_NAMES | \
                                             CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_PARAMETER_NAMES | \
                                             CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING | \
                                             CSR_BT_VCARD_FLAGS_NO_MANDATORY_TAGS)


#define OUTPUT_STRING(str)                  (str ? str : TEXT(""))

struct CContactList
{
    Contact *m_pcrHead;
    DWORD m_dwCntList;
    CRITICAL_SECTION m_cs;

    CContactList(): m_pcrHead(NULL), m_dwCntList(0)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CContactList)

        InitializeCriticalSection(&m_cs);
    }
    ~CContactList()
    {
        HEAP_STATISTICS_CLASS_FREE(CContactList)

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
    Contact *Add(Contact *pc, BOOL bNew = FALSE);
    BOOL Remove(Contact *pc, BOOL bDelete = TRUE);
    void Free();
    Contact *Find(Contact *pc) const;
};

/* +++ Phonebook API function pointer +++ */
typedef DWORD (WINAPI *PFN_PacmConnect)(BT_ADDR bluetoothAddress);
typedef DWORD (WINAPI *PFN_PacmDisconnect)();
typedef DWORD (WINAPI *PFN_PacmCancel)();
typedef DWORD (WINAPI *PFN_PacmRegister)(PacmRegisterInfo* pacmRegisterInfo);
typedef DWORD (WINAPI *PFN_PacmPullPhonebook)(BT_ADDR bluetoothAddress, TCHAR* filename, PacmSource source, PacmFormat format, PacmFilter filter, unsigned short maximumListCount, unsigned short listStartOffset);
typedef DWORD (WINAPI *PFN_PacmGetVcardList)(BT_ADDR bluetoothAddress, TCHAR* filename, PacmOrder order, TCHAR* searchFilter, PacmSearchAttribute searchAttribute, unsigned short maximumListCount, unsigned short listStartOffset);
typedef DWORD (WINAPI *PFN_PacmGetVcardEntry)(BT_ADDR bluetoothAddress, TCHAR* filename, PacmFormat format, PacmFilter filter);
typedef DWORD (WINAPI *PFN_PacmUnregister)();

typedef DWORD (WINAPI *PFN_PacmDecodeVcardByHandle)(PhonebookDataHandleT phonebookDataHandle, CsrBtVcardData** vcardData);
typedef DWORD (WINAPI *PFN_PacmDecodeVcardByBuffer)(LPVOID buffer, DWORD bufferSize, PhonebookDataHandleT* phonebookDataHandle, CsrBtVcardData** vcardData);
typedef DWORD (WINAPI *PFN_PacmDecodeVcardList)(PhonebookDataHandleT phonebookDataHandle, DWORD* vcardListEntries, PacmVcardListEntry** list);
typedef DWORD (WINAPI *PFN_PacmGetDataPointer)(PhonebookDataHandleT phonebookDataHandle, LPVOID* dataPointer);
typedef DWORD (WINAPI *PFN_PacmGetDataSize)(PhonebookDataHandleT phonebookDataHandle, DWORD* size);
typedef DWORD (WINAPI *PFN_PacmGetDataType)(PhonebookDataHandleT phonebookDataHandle, PacmDatatype* dataType);
typedef void  (WINAPI *PFN_PacmCloseHandle)(PhonebookDataHandleT phonebookDataHandle);
/* --- Phonebook API function pointer --- */

/* +++ Vcard parser API function pointer +++ */
typedef void      (WINAPI *PFN_VcardFree)(CsrBtVcardData* csrBtVcardData);
typedef void      (WINAPI *PFN_VcardParseInitialize)(CsrBtVcardParserControl* control, CsrUint32 flags);
typedef CsrUint32 (WINAPI *PFN_VcardParseRead)(CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control);
typedef CsrUint32 (WINAPI *PFN_VcardParseReadVersionOnly)(CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control);
typedef CsrUint32 (WINAPI *PFN_VcardParseComplete)(CsrUint32* csrBtVcardDataEntries, CsrBtVcardData** vcardData, CsrUint8* version, CsrBtVcardParserControl* control);
typedef CsrUint32 (WINAPI *PFN_VcardParse)(CsrUint8** buffer, CsrUint32 bufferSize, CsrUint32* csrBtVcardDataEntriesFound, CsrUint32* csrBtVcardDataEntriesParsed, CsrBtVcardData** csrBtVcardData, CsrUint32 flags);
typedef void      (WINAPI *PFN_VcardGeneratorInitialize)(CsrBtVcardGeneratorControl* control, CsrUint32 flags);
typedef CsrUint32 (WINAPI *PFN_VcardGeneratorWrite)(CsrBtVcardData* csrBtVcardData, CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl* control);
typedef CsrBool   (WINAPI *PFN_VcardIsDateValid)(CsrBtIso8601Date* iso8601Date);
typedef CsrBool   (WINAPI *PFN_VcardIsTimeValid)(CsrBtIso8601Time* iso8601Time);
/* --- Vcard parser API function pointer --- */


class CApi_PB :
    public CApi_base
{
public:
    CApi_PB(void);
    /* Define the virtual de-constructor to free SfWE SDK. Otherwise memory leak occurs. */
    virtual ~CApi_PB(void);

    //for download
    BT_ADDR m_btaddr;
    TCHAR m_filename[MAX_PATH];
    PacmSource m_source;
    PacmFormat m_format;
    PacmFilter m_specified_filter;

    //for sarch
    PacmOrder m_order;
    PacmSearchAttribute m_searchAttribute;
    TCHAR m_currentPhonebookPath[MAX_PATH];
    TCHAR m_searchVal[MAX_PATH];

    unsigned short m_maximumListCount;
    unsigned short m_listStartOffset;
    //Phonebook entry number per transfer
    unsigned short m_phonebookEntry;
    //Flag indicating whether to use local phonebook when to retrieve contact info.
    BOOL m_useLocalPhonebook;

    virtual BOOL Initialize();
    virtual void DeInitialize();

    //SN thread
    static DWORD SignalNotify_thread(CApi_PB *pApi_PB)
    {
        return pApi_PB->Thread_run_SignalNotify();
    }
    DWORD Thread_run_SignalNotify();

    UINT m_uiSignalNotifyMessage;
    DWORD m_dwThreadID;

    const TCHAR *m_szPBState;                       //PB state

    //PBAP apis
    DWORD Connect(BT_ADDR bluetoothAddress)
    {
        CHECK_FUNC_PTR(m_pfnPacmConnect);
        return m_pfnPacmConnect(bluetoothAddress);
    }
    DWORD Disconnect()
    {
        CHECK_FUNC_PTR(m_pfnPacmDisconnect);
        return m_pfnPacmDisconnect();
    }
    DWORD Cancel()
    {
        CHECK_FUNC_PTR(m_pfnPacmCancel);
        return m_pfnPacmCancel();
    }
    DWORD Register(PacmRegisterInfo *pacmRegisterInfo)
    {
        CHECK_FUNC_PTR(m_pfnPacmRegister);
        return m_pfnPacmRegister(pacmRegisterInfo);
    }
    DWORD PullPhonebook(BT_ADDR bluetoothAddress, TCHAR *filename, PacmSource source, PacmFormat format, PacmFilter filter, unsigned short maximumListCount, unsigned short listStartOffset)
    {
        CHECK_FUNC_PTR(m_pfnPacmPullPhonebook);

#ifdef DEBUG_PBAP_PERFORMANCE
        if (!GetTimeInMilliSec())
        {
            SetTimeInMilliSec();
        }
#endif

        return m_pfnPacmPullPhonebook(bluetoothAddress, filename, source, format, filter, maximumListCount, listStartOffset);
    }
    DWORD GetVcardList(BT_ADDR bluetoothAddress, TCHAR *filename, PacmOrder order, TCHAR *searchFilter, PacmSearchAttribute searchAttribute, unsigned short maximumListCount, unsigned short listStartOffset)
    {
        CHECK_FUNC_PTR(m_pfnPacmGetVcardList);
        return m_pfnPacmGetVcardList(bluetoothAddress, filename, order, searchFilter, searchAttribute, maximumListCount, listStartOffset);
    }
    DWORD GetVcardEntry(BT_ADDR bluetoothAddress, TCHAR *filename, PacmFormat format, PacmFilter filter)
    {
        CHECK_FUNC_PTR(m_pfnPacmGetVcardEntry);
        return m_pfnPacmGetVcardEntry(bluetoothAddress, filename, format, filter);
    }
    DWORD Unregister()
    {
        CHECK_FUNC_PTR(m_pfnPacmUnregister);
        return m_pfnPacmUnregister();
    }
    DWORD DecodeVcardByHandle(PhonebookDataHandleT phonebookDataHandle, CsrBtVcardData **vcardData)
    {
        CHECK_FUNC_PTR(m_pfnPacmDecodeVcardByHandle);
        return m_pfnPacmDecodeVcardByHandle(phonebookDataHandle, vcardData);
    }
    DWORD DecodeVcardByBuffer(LPVOID buffer, DWORD bufferSize, PhonebookDataHandleT *phonebookDataHandle, CsrBtVcardData **vcardData)
    {
        CHECK_FUNC_PTR(m_pfnPacmDecodeVcardByBuffer);
        return m_pfnPacmDecodeVcardByBuffer(buffer, bufferSize, phonebookDataHandle, vcardData);
    }
    DWORD DecodeVcardList(PhonebookDataHandleT phonebookDataHandle, DWORD *vcardListEntries, PacmVcardListEntry **list)
    {
        CHECK_FUNC_PTR(m_pfnPacmDecodeVcardList);
        return m_pfnPacmDecodeVcardList(phonebookDataHandle, vcardListEntries, list);
    }
    DWORD GetDataPointer(PhonebookDataHandleT phonebookDataHandle, LPVOID *dataPointer)
    {
        CHECK_FUNC_PTR(m_pfnPacmGetDataPointer);
        return m_pfnPacmGetDataPointer(phonebookDataHandle, dataPointer);
    }
    DWORD GetDataSize(PhonebookDataHandleT phonebookDataHandle, DWORD *size)
    {
        CHECK_FUNC_PTR(m_pfnPacmGetDataSize);
        return m_pfnPacmGetDataSize(phonebookDataHandle, size);
    }
    DWORD GetDataType(PhonebookDataHandleT phonebookDataHandle, PacmDatatype *dataType)
    {
        CHECK_FUNC_PTR(m_pfnPacmGetDataType);
        return m_pfnPacmGetDataType(phonebookDataHandle, dataType);
    }
    void PBCloseHandle(PhonebookDataHandleT phonebookDataHandle)
    {
        if (m_pfnPacmCloseHandle)
        {
            m_pfnPacmCloseHandle(phonebookDataHandle);
        }
    }

    //Vcard API
    void VcardFree(CsrBtVcardData* csrBtVcardData)
    {
        if (m_pfnVcardFree)
        {
            m_pfnVcardFree(csrBtVcardData);
        }
    }
    void VcardParseInitialize(CsrBtVcardParserControl* control, CsrUint32 flags)
    {
        if (m_pfnVcardParseInitialize)
        {
            m_pfnVcardParseInitialize(control, flags);
        }
    }
    CsrUint32 VcardParseRead(CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control)
    {
        return m_pfnVcardParseRead ? m_pfnVcardParseRead(buffer, bufferSize, control) : CSR_BT_VCARD_ERROR_INVALID_PARAMETER;
    }
    CsrUint32 VcardParseReadVersionOnly(CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control)
    {
        return m_pfnVcardParseReadVersionOnly ? m_pfnVcardParseReadVersionOnly(buffer, bufferSize, control) : CSR_BT_VCARD_ERROR_INVALID_PARAMETER;
    }
    CsrUint32 VcardParseComplete(CsrUint32* csrBtVcardDataEntries, CsrBtVcardData** vcardData, CsrUint8* version, CsrBtVcardParserControl* control)
    {
        return m_pfnVcardParseComplete ? m_pfnVcardParseComplete(csrBtVcardDataEntries, vcardData, version, control) : CSR_BT_VCARD_ERROR_INVALID_PARAMETER;
    }
    CsrUint32 VcardParse(CsrUint8** buffer, CsrUint32 bufferSize, CsrUint32* csrBtVcardDataEntriesFound, CsrUint32* csrBtVcardDataEntriesParsed, CsrBtVcardData** csrBtVcardData, CsrUint32 flags)
    {
    	return m_pfnVcardParse ? m_pfnVcardParse(buffer, bufferSize, csrBtVcardDataEntriesFound, csrBtVcardDataEntriesParsed, csrBtVcardData, flags) : CSR_BT_VCARD_ERROR_INVALID_PARAMETER;
    }
    void VcardGeneratorInitialize(CsrBtVcardGeneratorControl* control, CsrUint32 flags)
    {
        if (m_pfnVcardGeneratorInitialize)
        {
            m_pfnVcardGeneratorInitialize(control, flags); 
        }
    }
    CsrUint32 VcardGeneratorWrite(CsrBtVcardData* csrBtVcardData, CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl* control)
    {
        return m_pfnVcardGeneratorWrite ? m_pfnVcardGeneratorWrite(csrBtVcardData, buffer, bufferSize, control) : 0;
    }
    CsrBool VcardIsDateValid(CsrBtIso8601Date* iso8601Date)
    {
        return m_pfnVcardIsDateValid ? m_pfnVcardIsDateValid(iso8601Date) : FALSE;
    }
    CsrBool VcardIsTimeValid(CsrBtIso8601Time* iso8601Time)
    {
        return m_pfnVcardIsTimeValid ? m_pfnVcardIsTimeValid(iso8601Time) : FALSE;
    }

    //PBAP callbacks
    static void WINAPI StaticPacmCbPullPhonebookComplete(LPVOID userdata, DWORD result, PhonebookDataHandleT dataHandle, DWORD phonebookSize, DWORD newMissedCall);
    static void WINAPI StaticPacmCbGetVcardListComplete(LPVOID userdata, DWORD result, PhonebookDataHandleT dataHandle, DWORD phonebookSize, DWORD newMissedCall);
    static void WINAPI StaticPacmCbGetVcardEntryComplete(LPVOID userdata, DWORD result, PhonebookDataHandleT dataHandle);
    static void WINAPI StaticPacmCbCancelComplete(LPVOID userdata, DWORD result);
    static void WINAPI StaticBluetoothStatusChange(LPVOID userdata, ModuleApiBluetoothStatus result);

    BOOL IsConnected();

    inline BOOL IsVcardDecoded(PhonebookDataHandleT phonebookDataHandle, PBYTE buffer, DWORD bufferSize)
    {
        if (!phonebookDataHandle || !buffer || !bufferSize)
        {
            return FALSE;
        }

        return ((CsrBtVcardData*) phonebookDataHandle == (CsrBtVcardData*) buffer) && (bufferSize == sizeof(CsrBtVcardData)) ? TRUE : FALSE;
    }

    BOOL IsVcardDecoded(PhonebookDataHandleT phonebookDataHandle);

    //Calculate the total number of vcard entry in vcard data.
    DWORD GetVcardEntry(CsrBtVcardData* vcardData);

    //Decode vcard according to phonebook data handle.
    DWORD DecodeVcard(PhonebookDataHandleT phonebookDataHandle, CsrBtVcardData** vcardData, DWORD& vcardEntries, DWORD& vcardParsed, BOOL fullParse = FALSE);

    //Decode vcard, using CSR Synergy vcard parser exposed.
    //NOTE: 
    //(1) SfWE export the API in PBAP to decode vcard, including "PacmDecodeVcardByHandle".
    //However, it's originally based on the vcard parser in legacy BCHS. And most of importance,
    //we'd better expose vcard parser so as to facilitate debug IOP issue relative with PBAP.
    //(2) Customer can also utilize their own vcard parser, after retrieving vcard data. 
    CsrUint32 DecodeVcard(PBYTE buffer, DWORD bufferSize, CsrBtVcardData** vcardData, DWORD& entries, DWORD& parsed, CsrUint32 flags = CSR_BT_VCARD_FLAGS);

    //Decode single vcard data fully into 1 contact.
    BOOL DecodeVcard(CsrBtVcardData* vcardData, Contact* pContact);

    void FreeVcardData(PhonebookDataHandleT phonebookDataHandle, CsrBtVcardData* vcardData);

    static void DumpVcard(CsrBtVcardData* vcardData);
    static void DumeVcard(LPTSTR field, CsrBtVcardDataType* vcardDataType);
    static void DumpVcard(PBYTE buf, DWORD size);

    static void DumpVcardListEntry(PacmVcardListEntry* vcardListEntry);

    //Dump phonebook into "pb.vcf" for debug.
    void DumpPhonebook(PhonebookDataHandleT phonebookDataHandle);
    static void DumpPhonebook(PBYTE buf, DWORD size, PacmDatatype type = PACM_DATATYPE_PHONEBOOK);

    BOOL InitFuncPtr();
    void DumpFuncPtr();

    PacmFormat GetPhonebookFormat();

    BOOL InitContact(Contact* pContact, CsrBtVcardData* vcardData, BOOL fullInit = TRUE);
    void AddContact(Contact* pContact, BOOL& isNewContact, BOOL alwaysAdd = FALSE, BOOL freeContactInfo = TRUE);
    void UpdateContact(Contact* pContact, BOOL freeContactInfo = TRUE);
    Contact* GetContact(LPTSTR szKey);
    void RemoveContact(LPTSTR szKey);
    void RemoveAllContacts();
    void EnumContact();
    void FlushContact();
    DWORD SearchContact(PacmSearchAttribute searchAttr, LPTSTR key, Contact** &contactList, DWORD &contactCount);    
    DWORD SearchContact();

    void InitCallRecord(CALLREC* pCallRecord, CsrBtVcardData* vcardData, BYTE callRecordType);
    void AddCallRecord(CALLREC* pCallRecord);
    void RemoveCallRecord(CALLREC* pCallRecord);
    void RemoveAllCallRecords();
    void EnumCallRecord();
    void FlushCallRecord();

    void SetCurrentContact(Contact* pc, BOOL freeContact = FALSE);
    Contact* GetCurrentContact()    { return m_pCurContact; }

    inline BOOL ValidContact(Contact* pc)   { return pc && (pc->GetKey() != NULL) ? TRUE : FALSE; }

    static void DumpContact(Contact* pContact);
    static void DumpCallRecord(CALLREC* pCallRecord);

    static BOOL ValidSearchKey(LPTSTR searchKey);

#ifdef DEBUG_PBAP_PERFORMANCE
    DWORD GetTimeInMilliSec()           { return m_timeInMilliSec; }
    void SetTimeInMilliSec()            { m_timeInMilliSec = GetTickCount(); }
    void ClearTimeInMilliSec()          { m_timeInMilliSec = 0; }
#endif

private:
    HINSTANCE m_hPhonebookDll;

    PFN_PacmConnect                    m_pfnPacmConnect;
    PFN_PacmDisconnect                 m_pfnPacmDisconnect;
    PFN_PacmCancel                     m_pfnPacmCancel;
    PFN_PacmRegister                   m_pfnPacmRegister;
    PFN_PacmPullPhonebook              m_pfnPacmPullPhonebook;
    PFN_PacmGetVcardList               m_pfnPacmGetVcardList;
    PFN_PacmGetVcardEntry              m_pfnPacmGetVcardEntry;
    PFN_PacmUnregister                 m_pfnPacmUnregister;
    PFN_PacmDecodeVcardByHandle        m_pfnPacmDecodeVcardByHandle;
    PFN_PacmDecodeVcardByBuffer        m_pfnPacmDecodeVcardByBuffer;
    PFN_PacmDecodeVcardList            m_pfnPacmDecodeVcardList;
    PFN_PacmGetDataPointer             m_pfnPacmGetDataPointer;
    PFN_PacmGetDataSize                m_pfnPacmGetDataSize;
    PFN_PacmGetDataType                m_pfnPacmGetDataType;
    PFN_PacmCloseHandle                m_pfnPacmCloseHandle;

    HINSTANCE m_hVcardDll;

    PFN_VcardFree                      m_pfnVcardFree;
    PFN_VcardParseInitialize           m_pfnVcardParseInitialize;
    PFN_VcardParseRead                 m_pfnVcardParseRead;
    PFN_VcardParseReadVersionOnly      m_pfnVcardParseReadVersionOnly;
    PFN_VcardParseComplete             m_pfnVcardParseComplete;
    PFN_VcardParse                     m_pfnVcardParse;
    PFN_VcardGeneratorInitialize       m_pfnVcardGeneratorInitialize;
    PFN_VcardGeneratorWrite            m_pfnVcardGeneratorWrite;
    PFN_VcardIsDateValid               m_pfnVcardIsDateValid;
    PFN_VcardIsTimeValid               m_pfnVcardIsTimeValid;

    // Current contact selected.
    Contact* m_pCurContact;

#ifdef STORE_PHONEBOOK_IN_DB
    // POOM for the access of WinCE default 'Contact Database'.
    Poom_t* m_poom;
#endif

#ifdef STORE_CALL_RECORD_IN_DB
    // Database to store call record.
    Database_t* m_database;
#endif

#ifdef DEBUG_PBAP_PERFORMANCE
    // Debug only to store current tick count in milli-second so as to measure PBAP performance.
    DWORD m_timeInMilliSec;
#endif

    class CEnumPhonebookThread;
    friend CEnumPhonebookThread;
    class CEnumPhonebookThread : public CMiniThread
    {
        public:
            CEnumPhonebookThread(CApi_PB* apiPB);
            virtual ~CEnumPhonebookThread();
    
            virtual DWORD ThreadRun();

        private:
            CApi_PB* m_apiPB;
            HANDLE m_hWaitEvent;
    };

    CEnumPhonebookThread* m_pEnumPhonebookThread;

    BOOL StartEnumPhonebook();
    void StopEnumPhonebook();
};
