#pragma once
#include <windows.h>


// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DABRADIO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DABRADIODLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//	#ifdef DABRADIO_EXPORTS
//	#define DAB_API __declspec(dllexport)
//	#else
//	#define DAB_API __declspec(dllimport)
//	#endif

#define EXTERN_C    extern "C"

#define DAB_DEBUG_FUNC_ON 1

#define PRINTF_FUNC_BUILD_TIME()                    do{\
                                                        if(DAB_DEBUG_FUNC_ON)\
                                                        RETAILMSG(1, (TEXT("MSG:DAB:[%s]: Blild at %s %s\r\n"),TEXT(__FUNCTION__),TEXT(__DATE__),TEXT(__TIME__)));\
                                                    }while(0)

#define PRINTF_DEBUG_FUNC()                         do{\
                                                        if(DAB_DEBUG_FUNC_ON)\
                                                        RETAILMSG(1, (TEXT("MSG:DAB:[%s]: \r\n"),TEXT(__FUNCTION__)));\
                                                    }while(0)

#define PRINTF_DEBUG_MSG(STR)                           do{\
                                                        if(DAB_DEBUG_FUNC_ON)\
                                                        RETAILMSG(1, (TEXT("MSG:DAB:[%s]: %s\r\n"),TEXT(__FUNCTION__),TEXT(STR)));\
                                                    }while(0)


#define PRINTF_DEBUG_ERR(STR)                        do{\
                                                        if(DAB_DEBUG_FUNC_ON)\
                                                        RETAILMSG(1, (TEXT("ERR:DAB:[%s]: @Line:%d %s\r\n"),TEXT(__FUNCTION__),__LINE__,TEXT(STR)));\
                                                    }while(0)

#define PRINTF_DEBUG_FUNC_PARAM(STR,PARAM)          do{\
                                                        if(DAB_DEBUG_FUNC_ON)\
                                                        RETAILMSG(1, (STR,PARAM));\
                                                    }while(0)


typedef unsigned char byte;


//uFgType：标志定义类型：
typedef enum _dab_flag_type
{
	DABFlagType_Start = 0x00,
	DAB_ViewFlag_PlayInfoUpdate,
	DAB_ViewFlag_ScanStatus,
	DAB_ViewFlag_BandUpdate,
	DAB_ViewFlag_TAStatusUpdate,
	DAB_ViewFlag_SNRValueUpdate,
	DAB_ViewFlag_SlideShowUpdate,
	DAB_ViewFlag_Count
};
EXTERN_C   bool DABApi_GetFlag(byte uFgType);
EXTERN_C   bool DABApi_SetFlag(byte uFgType, bool bFgVal);

EXTERN_C   bool DABApi_Service( bool Start );
EXTERN_C   bool DABApi_PowerOffPro( bool bPower );

EXTERN_C   bool DABApi_ResetData( byte ResetType );

EXTERN_C   bool DABApi_BandProc(void);


//搜索模式定义
typedef enum _dab_search_mode
{
	AllService_Search, //在所有Service 中操作
	PresetService_Search, //在所有Preset 中操作
	CurrentService_Search, //在当前Service 中操作
	Manual_Search //在Station 中操作
};
EXTERN_C   bool DABApi_SearchModeProc(void);
EXTERN_C   bool DABApi_ChanUpProc(byte uSearchMode);
EXTERN_C   bool DABApi_ChanDownProc(byte uSearchMode);
EXTERN_C   bool DABApi_SeekUpProc(byte uSearchMode);
EXTERN_C   bool DABApi_SeekDownProc(byte uSearchMode);
EXTERN_C   bool DABApi_ScanProc( byte uSearchMode, bool fgStart );//预览，每播10S



EXTERN_C   bool DABApi_ServiceScanProc( void );//清完播第一个


typedef enum _dab_list_type
{
	ListType_AllService, //所有Service 列表
	ListType_Ensembel, //所有Ensemble 列表
	ListType_SubService, //指定Ensemble 下的Service 列表
	ListType_PresetService, //当前波段的Preset 列表
	ListType_PresetShortService, //当前波段的Preset short label --- add 20141008
	ListType_CurrentService //当前Ensemble 下的Service 列表
};

typedef enum _dab_pty_list_type
{
    PtyList_AllType = 0xFF,
    PtyList_None = 0,   // 0
    PtyList_News,
    PtyList_Affairs,       // 2
    PtyList_Information,
    PtyList_Sport,         // 4
    PtyList_Education,
    PtyList_Drama,         // 6
    PtyList_Culture,
    PtyList_Science,       // 8
    PtyList_Varied,
    PtyList_PopMusic,      // 10
    PtyList_RockMusic,
    PtyList_EasyMusic,     // 12
    PtyList_LightClassical,
    PtyList_ClassicalMusic,// 14
    PtyList_OtherMusic,
    PtyList_Weather,        // 16
    PtyList_Finance,
    PtyList_Children,       // 18
    PtyList_Factual,
    PtyList_Religion,       // 20
    PtyList_PhoneIn,
    PtyList_Travel,         // 22
    PtyList_Leisure,
    PtyList_JazzAndBlues,  // 24
    PtyList_CountryMusic,
    PtyList_NationalMusic, // 26
    PtyList_OldiesMusic,
    PtyList_FolkMusic,      // 28
    PtyList_Documentary,
    PtyList_Test,
    PtyList_Alarm
};

EXTERN_C   bool DABApi_ServiceDirectEntryProc( byte uListType, byte uIndex );

EXTERN_C   bool DABApi_PresetMemeryProc(byte uBand, byte uPreset);
EXTERN_C   bool DABApi_PresetLoadProc( byte uBand, byte uPreset );
EXTERN_C   bool DABApi_PresetDeleteProc(byte uBand, int nParam );
EXTERN_C   bool DABApi_AutoStoreProc( void );




typedef enum _srv_following
{
	SrvFollowing_Off = 0x00,
	SrvFollowing_DABtoDAB,
	SrvFollowing_DABtoRDS,
	SrvFollowing_DABtoAll // include DAB to DAB, DAB to FM, Unused
};

EXTERN_C   bool DABApi_SetServiceLink( void );
EXTERN_C   bool DABApi_ServiceLinkOnOff(bool OnOff );



EXTERN_C   bool DABApi_SetTASetting( void );
EXTERN_C   bool DABApi_SetLBandSetting( void );
EXTERN_C   bool DABApi_SetLinkSNRValue( byte type, byte value );

EXTERN_C   bool DABApi_PTYSeekProc( byte uPtyValue );
EXTERN_C   bool DABApi_PTYSeekStop( void );

EXTERN_C   bool DABApi_GetCurrentBand( byte *pBand );
EXTERN_C   bool DABApi_GetCurrentPreset( byte *pPreset );
EXTERN_C   bool DABApi_GetCurrentSearchMode( byte *pMode );
EXTERN_C   bool DABApi_GetSrvLinkSetting( byte *pServiceLink );
EXTERN_C   bool DABApi_GetTASetting( byte *pTASwitch );
EXTERN_C   bool DABApi_GetLBandSetting( byte *pLBandSwitch );


typedef enum _dab_chninfo_type
{
	ChnInfo_StationName, //电台名
	ChnInfo_EnsembleLabel,
	ChnInfo_ServiceLabel,
	ChnInfo_DLS, // max is 128 bytes
	ChnInfo_Freq,
	ChnInfo_PTY
};
EXTERN_C   TCHAR* DABApi_GetCurrChanInfo( byte uInfoType );

EXTERN_C   bool DABApi_GetPowerOnStatus( void );
EXTERN_C   byte DABApi_GetLinkSNRValue();
EXTERN_C   byte DABApi_GetLinkSNRMscErr();



EXTERN_C   bool DABApi_GetTotalOfListInfo( byte uListType, byte EnsembleIdx, int *pTotal );
EXTERN_C   TCHAR* DABApi_GetNameOfListInfo( byte uListType, WORD wIndex, byte *pPty = NULL  );

//EXTERN_C   bool DABApi_GetScanPercent( byte *pPct );

typedef enum _dab_scan_type
{
	ServiceScan, //服务搜索
	PlayScan, //预览
	AutoStore, //自动存储
	PTYSeek, //PTY 搜索
	ManuelSeek //手动SEEK
};
EXTERN_C   bool DABApi_GetScanStatus( byte uType );
EXTERN_C   bool DABApi_GetTAInfoStatus( byte *pTAStatus );



EXTERN_C   void DABApi_ParseData(byte *pData, DWORD dwLen);

EXTERN_C   bool DABApi_ServiceLinkReport( byte *pRxPtr );


#define MAX_PI_CODE_SEND 10
#define MAX_FREQS_SEND   10

typedef struct
{
	unsigned short PICode[MAX_PI_CODE_SEND];
	unsigned short Freqs[MAX_FREQS_SEND];
}link_RDS_FM_info_t;

/************************************************************************/
/*
功能: 请求主机进行Service Link
*/
/************************************************************************/
//EXTERN_C  bool CALLBACK CB_DABServiceLink( byte Link, WORD *pPtr );
/*主机提供给库的回调函数；
Link = 0：DAB 信号恢复,如果之前音频切到RADIO,需要切回DAB
Link = 1：通知MCU 进行Service_Link 动作
同时发送PI 与频率给到主机
pPtr [0]~ pPtr [1] 0xNNNN PI (高位在前,低位在后)
pPtr [2]~ pPtr [3] 保留

pPtr [4]~ pPtr [7] 0xNN~0xNN Freq1 (高位在前,低位在后)
pPtr [8]~ pPtr [11] 0xNN~0xNN Freq2
pPtr [12]~ pPtr [15] 0xNN~0xNN Freq3
*/





typedef bool ( CALLBACK * PROC_SEND_DAB_DATA )( WPARAM wParam, LPARAM
											   lParam );
typedef bool ( CALLBACK * PROC_LOAD_DAB_DATA )( int nSize, void *pPtr );
typedef bool ( CALLBACK * PROC_SAVE_DAB_DATA )( int nSize, void *pPtr );
typedef bool ( CALLBACK * PROC_DAB_SRV_LINK )( byte Link,WORD *pPtr );
struct FuncDABCallBack
{
	PROC_SEND_DAB_DATA pCBSendDABData;
	PROC_LOAD_DAB_DATA pCBLoadData;
	PROC_SAVE_DAB_DATA pCBSaveData;
	PROC_DAB_SRV_LINK pCBSrvLink;
};

EXTERN_C   void RegisterDABCBFunc(FuncDABCallBack FnCB);


//DAB模块软件升级功能相关函数
EXTERN_C   bool DABApi_GetSoftWareUpgradeStatus(byte *Flag, byte *Progress);
EXTERN_C   bool DABApi_SoftWareUpgradeRequest(TCHAR *pFilePath);
EXTERN_C   bool DABApi_GetSoftWareVersion(byte *pVersion);

//SlideShow相关函数
EXTERN_C   bool DABApi_GetSlideShowFileName( byte *pFile);


//以下函数为测试扩展：
EXTERN_C   bool DABApi_XPADSetting( bool onff);

EXTERN_C   bool DABApi_ClearListServicesPreset( void);
EXTERN_C   bool DABApi_scanband( void);

EXTERN_C   bool DABApi_ServicesListSortByLable( void);
EXTERN_C   bool DABApi_ServicesListSortByShorLable( void);
EXTERN_C   bool DABApi_ServicesListSortByEnsemble( void);
EXTERN_C   bool DABApi_GetListCountCmd( void);
EXTERN_C   UINT DABApi_GetListCountVal( void);



//以下函数为预留，用于协议兼容：
typedef struct _dab_dll_setting
{
	byte ServiceLink;
	byte TASwitch;
	byte PTYSwitch;
	byte LBandSwitch;
}DAB_DLL_SETTING;
EXTERN_C   bool DABApi_InitDllSetting( DAB_DLL_SETTING *pDllSettings );

EXTERN_C   bool DABApi_TaskExit( void );
EXTERN_C   bool DABApi_GetExistOnPresetList( byte uBand, byte uIndex );

EXTERN_C   bool DABApi_SetDeleteReadyOnPreset( byte uIndex, byte uMode );
EXTERN_C   bool DABApi_GetDeleteReadyOnPreset( byte uIndex );

EXTERN_C   TCHAR* DABApi_GetCurrPTYName( void );
EXTERN_C   bool DABApi_GetAudioMode( byte *AudioMode );
EXTERN_C   bool DABApi_GetScanPercent( byte *pPct );
EXTERN_C   bool DABApi_GetAdvisoryMsgType( byte *pMsgType );
