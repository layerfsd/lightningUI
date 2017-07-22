#pragma once

#include <windows.h>
#include "DABRadioLib.h"
#include "CnComm.h"
//#include <CMthread.h>
//#include <CSync.h>
#include "../../GUI/Main/GUI_Fantasy.h"
#include "../../Config/customer.h"

#if CVTE_EN_DAB
//DEBUG
#define DABADAPTER_DEBUG_MSG    1
#define DABADAPTER_DEBUG_ERR    1
#define DABADAPTER_DEBUG_TEST   1

#define DEBUG_COMMANDER		(0)
#define MAX_DATA_LENGTH       (255)
#define MAX_RECORD_NODE       (255)
#define MAX_RECORD_FIELDS     (12)
#define MAX_MESSAGE_LENGTH   (256)//64
#define MAX_PACKET_SIZE       (256)
#define MAX_MSG_DATA_LENGTH   (4)
#define MAX_APP_MSG_LENGTH    (128)
#define CYCLIC_BUF_SIZE       (5*1024)//512
#define STRING_LEN             (4096)
#define DISABLE_CHECKSUM_VERIFICATION 0xEF

typedef enum
{
	UPDATE_NOT_START   = 0,
	UPDATE_IN_PROGRESS = 1,
	UPDATE_IN_VERIFY   = 2,
	UPDATE_IN_FINISH   = 3,
	UPDATE_BAUD_38400  = 0xFE,
	UPDATE_IN_FAIL     = 0xFF
} UPDATE_FLAG_t;

typedef enum
{
	CMD_GET					= 0x00,
	CMD_GET_RESPONSE		= 0x80,

	CMD_GET_LIST			= 0x01,
	CMD_GET_NEXT_LIST		= 0x02,
	CMD_GET_PREV_LIST		= 0x03,
	CMD_GET_LIST_RESPONSE	= 0x81,

	CMD_SET					= 0x04,
	CMD_SET_RESPONSE		= 0x84,

	CMD_SET_LIST			= 0x05,
	CMD_SET_LIST_RESPONSE	= 0x85,

	CMD_SET_NOTIFICATION	= 0x06,
	CMD_SETNOTIFY_RESPONSE= 0x86,
	CMD_NOTIFICATION		= 0xc6,

	CMD_RESPONSE            = 0x87,

	CMD_LIST_COUNT          = 0x08,
	CMD_LIST_COUNT_RESPONSE = 0x88,

	//CMD_ITEM_DATA           = 0xC7
};

//Status Codes Define
typedef enum _FS_Status_Codes_
{
    FS_OK = 0x00,
    FS_FAIL = 0x85,
    FS_PACKET_BAD = 0x80,
    FS_NODE_DOES_NOT_EXIST = 0x81,
    FS_ITEM_DOES_NOT_EXIST = 0x82,
    FS_NODE_BLOCKED = 0x83
}FSStatus_t;

//Frame ID Define
#define MSG_BYTE_END                (0xC0)
#define MSG_BYTE_ESC                (0xDB)
#define MSG_BYTE_ESC_END            (0xDC)
#define MSG_BYTE_ESC_ESC            (0xDD)
typedef enum _SLIP_STEP
{
    SLIP_LOOK_FOR_START = 0,
    SLIP_ACTUAL_START,
    SLIP_LOOK_FOR_END
};

typedef enum _SLIP_CHAR
{
    SLIP_NORMAL_CHAR = 0,
    SLIP_ESCAPE_CHAR
};

class CDABAdapter
{
public:
    static CDABAdapter* GetInstance();
    void Initialize(DWORD PortNum, DWORD dwBaudRate);
    void Deinitialize();
    BOOL GetInitialize();
	BOOL HasData();
	bool SaveData();
	DWORD GetStartTick(){return m_dwStartTick;}
	link_RDS_FM_info_t *GetServiceLinkInfo(){return m_pServiceLinkInfo;}
	void EnableServiceLink(BOOL bEnable){m_bNeedServiceLink = bEnable;}

private:
	static bool OnComSend(WPARAM wParam, LPARAM lParam);
    static bool OnSrvLink(byte Link, WORD *pPtr );
    static bool OnPrintPic(unsigned char * filename);
	static bool OnSaveData( int nSize, void *pPtr );
	static bool OnLoadData( int nSize, void *pPtr );


private:

    CDABAdapter();
    CDABAdapter(const CDABAdapter &);
    ~CDABAdapter();

    VOID RegisterDABCallback();
    static DWORD WINAPI ThreadProc( LPVOID dParam );
    DWORD COMThreadRun();
    VOID OnComRecv(WPARAM wParam, LPARAM lParam);
    void StartCOM(DWORD dwBaudRate);
    void StopCOM();

public:
    //COM
   static CnComm* m_pCnComm;
   static byte *m_pdata;
   static int m_iDataLen;
   static BOOL m_bHasData;

private:
    //COM
    DWORD       m_PortNum;
    DWORD       m_dwThreadId;
    DWORD       m_dwStartTick;
    DWORD	    m_dwExitCode;
    HANDLE      m_hThreadHandle;
    HANDLE      m_hStartEvent;

    //DAB Param
//	    BOOL        m_bLBandOn;
//	    BOOL        m_bTAOn;
//	    BYTE        m_nSearchTypeMode;



	//
    static const CDABAdapter* m_pInstance;
    BOOL m_bInitialized;
    CSkinManagerGL *m_pSkinManager;
    HWND m_hMainWnd;
	static link_RDS_FM_info_t *m_pServiceLinkInfo;
	static BOOL	m_bNeedServiceLink;
};

#endif
