#pragma once
#include "api_base.h"
#include "bluetooth_mapc.h"
#include "bluetooth_mapc_app_ext.h"
#include "../txml/txml.h"
#include "generic_object.h"
#include "generic_xml_error.h"
#include "generic_xml_document.h"

#include <cmthread.h>
#include <vector>
using std::vector;

#define IS_SUCCESS(result)              ((result) == ERROR_SUCCESS)

#define MAPC_DATA_BUFFER_SIZE           0x40000
#define MAPC_STORE_PATH                 TEXT("\\StaticStore")
#define MAPC_DEFAULT_STORE_FILE_NAME    TEXT("mapc.msg")

#define MAPC_TIMEOUT_IN_MS              2000
#define MAX_FOLDER_NUM					10
#define MAP_FOLDER_LENGTH				20
#define MAX_CONTENT_LENGTH				1024
#define MAX_MESSAGE_LIST_NUM			100

typedef struct
{
    BYTE       *buffer;
    DWORD       size;
    DWORD       offset;
} MapcmDataBuffer;

typedef struct
{
    HANDLE              syncEvent;
    DWORD               operationResult;
    MapcmStatusT        status;
    HANDLE              storeFileHandle;
    MapcmDataBuffer     dataBuffer;
} MapcmInfo;

static MapcmInfo mapcm;

struct Message
{
    enum	//type
    {
        MSG_INBOX,
        MSG_OUTBOX,
        MSG_SENT,
        MSG_DELETED,
        MSG_OTHER,
    };

    Message *pNext;

    DWORD id;
    DWORD dwSize;
    DWORD dwAttachmentSize;

    BOOL bDetail;	//1 means alerady get message detail
    BOOL bText;
    BOOL bPriority;
    BOOL bRead;
    BOOL bSent;
    BOOL bProtected;

    MapMesSupport type;	//SMS, EMAIL, MMS
    MapMesReceptionStatus reception_status;
    BYTE folder;

    CString szFolder;
    CString szHandle;
    CString szSubject;
    CString szDatetime;
    CString szSenderName;
    CString szSenderAddress;
    CString szReplyAddress;
    CString szRecipientName;
    CString szRecipientAddress;
    CString szContent;

    Message()
    {
        HEAP_STATISTICS_CLASS_ALLOC(Message)

        memset(this, 0, sizeof(*this));
    }
    Message(Message *pm)
    {
        HEAP_STATISTICS_CLASS_ALLOC(Message)

        *this = *pm;
    }
    Message(int index)
    {
        HEAP_STATISTICS_CLASS_ALLOC(Message)

        szFolder = _T("");
        szHandle  = _T("");
        szSubject  = _T("");
        szDatetime  = _T("");
        szSenderName  = _T("");
        szSenderAddress  = _T("");
        szReplyAddress  = _T("");
        szRecipientName  = _T("");
        szRecipientAddress  = _T("");
        szContent  = _T("");
		pNext = NULL;
    }
    Message(LPTSTR handle)
    {
        HEAP_STATISTICS_CLASS_ALLOC(Message)

        szFolder = _T("");
        szHandle.Format(_T("%s"), handle);
        szSubject  = _T("");
        szDatetime  = _T("");
        szSenderName  = _T("");
        szSenderAddress  = _T("");
        szReplyAddress  = _T("");
        szRecipientName  = _T("");
        szRecipientAddress  = _T("");
        szContent  = _T("");
		pNext = NULL;
    }
    ~Message()
    {
        HEAP_STATISTICS_CLASS_FREE(Message)
    }
};

struct CMessageList
{
    Message *m_pmHead;
	Message *m_pmTail;
    DWORD m_dwCntList;
    CRITICAL_SECTION m_cs;

    CMessageList(): m_pmHead(NULL), m_dwCntList(0),m_pmTail(NULL)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CMessageList)

        InitializeCriticalSection(&m_cs);
    }
    ~CMessageList()
    {
        HEAP_STATISTICS_CLASS_FREE(CMessageList)

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
    Message *Add(Message *pc, BOOL bNew = FALSE);
    BOOL Remove(Message *pc, BOOL bDelete = TRUE);
    void Free();
    Message *Find(Message *pc) const;
};

class MapFolderListDecoder
{
    private:
        class GenericXMLDocument*   xmlDocument;
        TCHAR*                      xmlData;
        inline BOOL                 IsValidHeaderChar(CsrUint8 ch)  { return ch == '<'; }
        inline BOOL                 IsValidTailChar(CsrUint8 ch)    { return ch == '>'; }
        BOOL                        IsValidFolderListDataBuffer(LPSTR folderListString);
        BOOL                        IsValidFolderListDataBuffer(CsrUint8* buffer, DWORD size);
        BOOL                        CheckFolderListDataBuffer(CsrUint8* buffer, DWORD size);

    public:
        MapFolderListDecoder();
        ~MapFolderListDecoder();

        HRESULT     GetFolderListFromDataBuffer(CsrUint8* buffer, DWORD size, MapcFolderList* list);
};

/* +++ MAP API function pointer +++ */
typedef DWORD (WINAPI *PFN_MapcmRegister)(MapcmRegisterInfo *info);
typedef DWORD (WINAPI *PFN_MapcmUnRegister)(VOID);
typedef DWORD (WINAPI *PFN_MapcmConnect)(BT_ADDR bluetoothAddress, CsrUint8 masInstanceId);
typedef DWORD (WINAPI *PFN_MapcmDisconnect)();
typedef DWORD (WINAPI *PFN_MapcmGetFolderList)(LPCTSTR folderName, UINT16 startOffset, UINT16 maxListCount);
typedef DWORD (WINAPI *PFN_MapcmGetMessageList)(LPCTSTR folderName, UINT16 startOffset, UINT16 maxListCount, MAPCM_LIST_MESSAGE_FILTER *filter);
typedef DWORD (WINAPI *PFN_MapcmGetMessage)(LPCTSTR folderName, LPCTSTR messageHandle, MAPCM_MESSAGE_CHAR_SET charSet, BOOL attachment);
typedef DWORD (WINAPI *PFN_MapcmSetMessageStatus)(LPCTSTR folderName, LPCTSTR messageHandle, MAPCM_MESSAGE_STATUS_IND indicator,  MAPCM_MESSAGE_STATUS_VAL value);
typedef DWORD (WINAPI *PFN_MapcmPushMessage)(LPCTSTR folderName, UINT16 length, MAPCM_MESSAGE_CHAR_SET charSet, BOOL transparent);
typedef DWORD (WINAPI *PFN_MapcmUpdateInbox)();
typedef DWORD (WINAPI *PFN_MapcmCancel)();
typedef DWORD (WINAPI *PFN_MapcmRegisterNotificationService)(BOOL enableNotifications);
typedef DWORD (WINAPI *PFN_MapcmGetMasInstanceList)(BT_ADDR bluetoothAddress);
typedef DWORD (WINAPI *PFN_MapcmSelectMapcInstanceId)(CsrUint16 instanceId);
typedef DWORD (WINAPI *PFN_MapcmPushMessageContinue)(UINT8 *message, UINT16 length, BOOL finalFlag);
typedef DWORD (WINAPI *PFN_MapcmGetFolderListFromData)(MapcDataHandleT listData, CsrUint16 listDataSize, MapcFolderList* list);
typedef DWORD (WINAPI *PFN_MapcmGetFolderListSize)(MapcFolderList list, CsrUint16* size);
typedef DWORD (WINAPI *PFN_MapcmGetFolderListItemByIndex)(MapcFolderList list, CsrUint16 index, MapcFolderItem* item);
typedef DWORD (WINAPI *PFN_MapcmReleaseFolderList)(MapcFolderList* list);
typedef DWORD (WINAPI *PFN_MapcmReleaseFolderListItem)(MapcFolderItem* item);
/* --- MAP API function pointer --- */

class CApi_MAP	: public CApi_base
{
public:
    CApi_MAP(void);
    /* Define the virtual de-constructor to free SfWE SDK. Otherwise memory leak occurs. */
    virtual ~CApi_MAP(void);

    //folderList
    DWORD m_dwFolderNum;
    LPTSTR m_pfolderList[MAX_FOLDER_NUM];
    MAPCM_LIST_MESSAGE_FILTER *m_filter;

    virtual BOOL Initialize();
    virtual void DeInitialize();

    //Message Parser
    DWORD DecodeMessageListByHandle(MapcDataHandleT messageListDataHandle, DWORD dataSize, DWORD *ListEntries, MessageListData **messageListData);
	DWORD DecodeMessageListByFile(LPTSTR filename, DWORD *ListEntries, MessageListData **messageListData, TXmlDocument &doc);
    DWORD DecodeMessageListByFile(LPTSTR filename, DWORD *ListEntries, MessageListData **messageListData);
    DWORD DecodeMessageDetailByFile(LPTSTR filename, MessageDetailData *messageDetailData);
    DWORD DecodeNotificationByFile(LPTSTR filename, Notification *nt);
	DWORD DecodeMessageFileCommonProcess(LPTSTR filename,TCHAR* &pszXml2);

    //DownLoad message
    BTDEV *m_pbtdMap;

    BYTE m_bDownloadFolder;
    TCHAR m_szDownloadFolderName[MAP_FOLDER_LENGTH];
	
	DWORD m_dwDownloadResult;
	HANDLE m_syncEvent;
	HANDLE m_hThreadDownload;
    DWORD DownLoadMessage(BTDEV *pbtd);   
    static DWORD Download_thread(CApi_MAP *pApiMAP)
    {
        return pApiMAP->Thread_run_Download();
    }
    DWORD Thread_run_Download();

    //Send message
    DWORD m_dwMaxPackedSize;	//max packed size that can be pushed  
	DWORD m_pushMsgResult;		//result of push message	
	HANDLE m_pushMsgEvent;		//sync event for push msg

	HANDLE m_hThreadSendMessage;
	HANDLE m_hThreadRecoverNotification;
    DWORD SendMessage(BTDEV *pbtd);
    static DWORD SendMessage_thread(CApi_MAP *pApiMAP)
    {
        return pApiMAP->Thread_run_SendMessage();
    }


	static DWORD RecoverNotificationMessage_thread(CApi_MAP *pApiMAP)
    {
        return pApiMAP->Thread_run_Recover();
    }
    DWORD Thread_run_SendMessage();
    DWORD Thread_run_Recover();

    CString m_szSendContent;
    MapMesSupport m_SendMesType;
    MapcVCard *m_Sendoriginator;
    MapcVCard *m_SendrecipientList;
    BOOL m_bSendFinish;
    BOOL m_bReply;
	BOOL m_bForward;

    //Notification for new msg
    BOOL m_bNewMsg;
	
	//Close all send or download thread
	BOOL m_bStopDownload;
	BOOL m_bStopSend;
	BOOL m_bStopRecover;
	DWORD CancelUpdate(BOOL bCancel = TRUE);

    //clear msg file when exit;
    BOOL m_bClear;
    vector<CString> m_msgFile;

	BOOL GetMapNotification()
	{
		return m_bNotificationOn;
	}

    //MAP apis
    DWORD Register(MapcmRegisterInfo *info)
    {
		CHECK_FUNC_PTR(m_pfnMapcmRegister);
        return m_pfnMapcmRegister(info);
    }
    DWORD UnRegister()
    {
		CHECK_FUNC_PTR(m_pfnMapcmUnRegister);
        return m_pfnMapcmUnRegister();
    }
    DWORD Connect(BT_ADDR bluetoothAddress, CsrUint8 masInstanceId)
    {
		CHECK_FUNC_PTR(m_pfnMapcmConnect);
		//disconnect any phone which have MAP connected.
		if(!DisconnectExistedMAPConnection())
		{
			return ERROR_NOT_READY;
		}
        return m_pfnMapcmConnect(bluetoothAddress, masInstanceId);
    }
    DWORD Disconnect()
    {
		CHECK_FUNC_PTR(m_pfnMapcmDisconnect);
        return m_pfnMapcmDisconnect();
    }
    DWORD GetFolderList(LPCTSTR folderName, UINT16 startOffset, UINT16 maxListCount)
    {
		CHECK_FUNC_PTR(m_pfnMapcmGetFolderList);
        return m_pfnMapcmGetFolderList(folderName, startOffset, maxListCount);
    }
    DWORD GetMessageList(LPCTSTR folderName, UINT16 startOffset, UINT16 maxListCount, MAPCM_LIST_MESSAGE_FILTER *filter)
    {
		CHECK_FUNC_PTR(m_pfnMapcmGetMessageList);
        return m_pfnMapcmGetMessageList(folderName, startOffset, maxListCount, filter);
    }
    DWORD GetMessageDetail(LPCTSTR folderName, LPCTSTR messageHandle, MAPCM_MESSAGE_CHAR_SET charSet, BOOL attachment)
    {
		CHECK_FUNC_PTR(m_pfnMapcmGetMessage);
        return m_pfnMapcmGetMessage(folderName, messageHandle, charSet, attachment);
    }
    DWORD SetMessageStatus(LPCTSTR folderName, LPCTSTR messageHandle, MAPCM_MESSAGE_STATUS_IND indicator,  MAPCM_MESSAGE_STATUS_VAL value)
    {
		CHECK_FUNC_PTR(m_pfnMapcmSetMessageStatus);
        return m_pfnMapcmSetMessageStatus(folderName, messageHandle, indicator, value);
    }
    DWORD PushMessage(LPCTSTR folderName, UINT16 length, MAPCM_MESSAGE_CHAR_SET charSet, BOOL transparent)
    {
		CHECK_FUNC_PTR(m_pfnMapcmPushMessage);
        return m_pfnMapcmPushMessage(folderName, length, charSet, transparent);
    }
    DWORD UpdateInbox()
    {
		CHECK_FUNC_PTR(m_pfnMapcmUpdateInbox);
        return m_pfnMapcmUpdateInbox();
    }
    DWORD Cancel()
    {
		CHECK_FUNC_PTR(m_pfnMapcmCancel);
        return m_pfnMapcmCancel();
    }
    DWORD RegisterNotificationService(BOOL enableNotifications)
    {
		CHECK_FUNC_PTR(m_pfnMapcmRegisterNotificationService);
        return m_pfnMapcmRegisterNotificationService(enableNotifications);
    }
    DWORD GetMasInstanceList(BT_ADDR bluetoothAddress)
    {
		CHECK_FUNC_PTR(m_pfnMapcmGetMasInstanceList);
        return m_pfnMapcmGetMasInstanceList(bluetoothAddress);
    }
    DWORD SelectMapcInstanceId(CsrUint16 instanceId)
    {
		CHECK_FUNC_PTR(m_pfnMapcmSelectMapcInstanceId);
        return m_pfnMapcmSelectMapcInstanceId(instanceId);
    }
    DWORD PushMessageContinue(UINT8 *message, UINT16 length, BOOL finalFlag)
    {
		CHECK_FUNC_PTR(m_pfnMapcmPushMessageContinue);
        return m_pfnMapcmPushMessageContinue(message, length, finalFlag);
    }
    /* +++ Parse folder list +++ */
    DWORD GetFolderListFromData(MapcDataHandleT listData, CsrUint16 listDataSize, MapcFolderList *list);
    DWORD GetFolderListSize(MapcFolderList list, CsrUint16 *size);
    DWORD GetFolderListItemByIndex(MapcFolderList list, CsrUint16 index, MapcFolderItem *item);
    DWORD ReleaseFolderList(MapcFolderList *list);
    DWORD ReleaseFolderListItem(MapcFolderItem *item);
    /* --- Praser folder list --- */

    //MAP callbacks
    static void WINAPI staticBluetoothStatusChange(LPVOID userdata, ModuleApiBluetoothStatus status);
    static void WINAPI staticMapcmCbConnectComplete(LPVOID userdata, DWORD result, LPCTSTR serviceName, CsrUint8 masInstanceId, MapMesSupport supportedMessages, CsrUint16 obexPeerMaxPacketSize);
    static void WINAPI staticMapcmCbCancelComplete(LPVOID userdata, DWORD result);
    static void WINAPI staticMapcmGetFolderList(LPVOID userdata, DWORD result, MapcDataHandleT dataHandle, DWORD dataSize, BOOL finalFlag);
    static void WINAPI staticMapcmGetMessageList(LPVOID userdata, DWORD result, MapcDataHandleT dataHandle, DWORD dataSize, BOOL finalFlag);
    static void WINAPI staticMapcmGetMessage(LPVOID userdata, DWORD result, MapcDataHandleT dataHandle, DWORD dataSize, BOOL finalFlag);
    static void WINAPI staticMapcmPushMessageComplete(LPVOID userdata, DWORD result, LPCTSTR messageHandle);
    static void WINAPI staticMapcmUpdateInboxComplete(LPVOID userdata, DWORD result);
    static void WINAPI staticMapcmNotificationRegistrationStatus(LPVOID userdata, DWORD result, BOOL enableNotifications);
    static void WINAPI staticMapcmNotificationRegistrationOffInd(LPVOID userdata);
    static void WINAPI staticMapcmCbNotificationHandler(LPVOID userdata, BOOL isAbort, BOOL finalFlag, DWORD dataSize, MapcDataHandleT dataHandle);
    static void WINAPI staticMapcmStatusInd(LPVOID userdata, MapcmStatusT status);
    static void WINAPI staticMapcmCbGetMasInstanceList(LPVOID userdata, DWORD result, DWORD numOfMasInstances, MapcmMasInstance *list);
    static void WINAPI staticMapcmSelectMapcInstanceIdComplete(LPVOID userdata, DWORD result);
    static void WINAPI staticMapcmCbPushMessageContinueInd(LPVOID userdata, DWORD result, CsrUint16 maxAllowedPayloadSize);

    BOOL InitFuncPtr();
    void DumpFuncPtr();

    /* Override CApi_base. */
    WORD GetConnectionStatus();
	void SetConnectionStatus(MapcmStatusT status);
    DWORD WaitConnected(DWORD timeout = INFINITE);
    void NotifyConnected(BOOL isConnected = TRUE);

private:
    PFN_MapcmRegister							m_pfnMapcmRegister;
    PFN_MapcmUnRegister							m_pfnMapcmUnRegister;
    PFN_MapcmConnect							m_pfnMapcmConnect;
    PFN_MapcmDisconnect							m_pfnMapcmDisconnect;
    PFN_MapcmGetFolderList						m_pfnMapcmGetFolderList;
    PFN_MapcmGetMessageList						m_pfnMapcmGetMessageList;
    PFN_MapcmGetMessage							m_pfnMapcmGetMessage;
    PFN_MapcmSetMessageStatus					m_pfnMapcmSetMessageStatus;
    PFN_MapcmPushMessage						m_pfnMapcmPushMessage;
    PFN_MapcmUpdateInbox						m_pfnMapcmUpdateInbox;
    PFN_MapcmCancel								m_pfnMapcmCancel;
    PFN_MapcmRegisterNotificationService		m_pfnMapcmRegisterNotificationService;
    PFN_MapcmGetMasInstanceList					m_pfnMapcmGetMasInstanceList;
    PFN_MapcmSelectMapcInstanceId				m_pfnMapcmSelectMapcInstanceId;
    PFN_MapcmPushMessageContinue				m_pfnMapcmPushMessageContinue;
    HANDLE m_hConnectionEvent;

    //MNS(MAP Notification Service) registration status.
    BOOL m_bNotificationOn;            /* Flag indicating whether MNS(Message Notification Service) is on. */
    BOOL m_bRecoverNotification;         /* Flag indicating whether recover the notification flag*/
	HANDLE m_hRecoverEvent;

    //Update MAP connection status
    MapcmStatusT m_status;
    HANDLE m_hStatusEvent;
    void UpdateConnectionStatus(MapcmStatusT status);
	BOOL DisconnectExistedMAPConnection();
	
};
