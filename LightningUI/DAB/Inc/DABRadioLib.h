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


//uFgType����־�������ͣ�
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


//����ģʽ����
typedef enum _dab_search_mode
{
	AllService_Search, //������Service �в���
	PresetService_Search, //������Preset �в���
	CurrentService_Search, //�ڵ�ǰService �в���
	Manual_Search //��Station �в���
};
EXTERN_C   bool DABApi_SearchModeProc(void);
EXTERN_C   bool DABApi_ChanUpProc(byte uSearchMode);
EXTERN_C   bool DABApi_ChanDownProc(byte uSearchMode);
EXTERN_C   bool DABApi_SeekUpProc(byte uSearchMode);
EXTERN_C   bool DABApi_SeekDownProc(byte uSearchMode);
EXTERN_C   bool DABApi_ScanProc( byte uSearchMode, bool fgStart );//Ԥ����ÿ��10S



EXTERN_C   bool DABApi_ServiceScanProc( void );//���겥��һ��


typedef enum _dab_list_type
{
	ListType_AllService, //����Service �б�
	ListType_Ensembel, //����Ensemble �б�
	ListType_SubService, //ָ��Ensemble �µ�Service �б�
	ListType_PresetService, //��ǰ���ε�Preset �б�
	ListType_PresetShortService, //��ǰ���ε�Preset short label --- add 20141008
	ListType_CurrentService //��ǰEnsemble �µ�Service �б�
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
	ChnInfo_StationName, //��̨��
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
	ServiceScan, //��������
	PlayScan, //Ԥ��
	AutoStore, //�Զ��洢
	PTYSeek, //PTY ����
	ManuelSeek //�ֶ�SEEK
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
����: ������������Service Link
*/
/************************************************************************/
//EXTERN_C  bool CALLBACK CB_DABServiceLink( byte Link, WORD *pPtr );
/*�����ṩ����Ļص�������
Link = 0��DAB �źŻָ�,���֮ǰ��Ƶ�е�RADIO,��Ҫ�л�DAB
Link = 1��֪ͨMCU ����Service_Link ����
ͬʱ����PI ��Ƶ�ʸ�������
pPtr [0]~ pPtr [1] 0xNNNN PI (��λ��ǰ,��λ�ں�)
pPtr [2]~ pPtr [3] ����

pPtr [4]~ pPtr [7] 0xNN~0xNN Freq1 (��λ��ǰ,��λ�ں�)
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


//DABģ���������������غ���
EXTERN_C   bool DABApi_GetSoftWareUpgradeStatus(byte *Flag, byte *Progress);
EXTERN_C   bool DABApi_SoftWareUpgradeRequest(TCHAR *pFilePath);
EXTERN_C   bool DABApi_GetSoftWareVersion(byte *pVersion);

//SlideShow��غ���
EXTERN_C   bool DABApi_GetSlideShowFileName( byte *pFile);


//���º���Ϊ������չ��
EXTERN_C   bool DABApi_XPADSetting( bool onff);

EXTERN_C   bool DABApi_ClearListServicesPreset( void);
EXTERN_C   bool DABApi_scanband( void);

EXTERN_C   bool DABApi_ServicesListSortByLable( void);
EXTERN_C   bool DABApi_ServicesListSortByShorLable( void);
EXTERN_C   bool DABApi_ServicesListSortByEnsemble( void);
EXTERN_C   bool DABApi_GetListCountCmd( void);
EXTERN_C   UINT DABApi_GetListCountVal( void);



//���º���ΪԤ��������Э����ݣ�
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
