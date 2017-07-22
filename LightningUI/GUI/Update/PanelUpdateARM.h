#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
//	#include "../../Communication/CmdSender_McuUpdate.h"
#include "../Settings/SettingCommon.h"
#include "../../AppConfigProxy/AppConfigProxy.h"
#include "../../DIB/Dib.h"

#define DEBUG_UPDATE_ARM		1

typedef enum _UPDATE_TYPE
{
	UPDATE_TYPE_NULL = 0,
    UPDATE_TYPE_UI,
    UPDATE_TYPE_LOGO,
    UPDATE_TYPE_MCU,
    UPDATE_TYPE_ALL,
    UPDATE_TYPE_DAB,
	UPDATE_TYPE_RES,
};

typedef enum _UPDATE_STATUS
{
	UPDATE_STATUS_IDLE = 0,
	UPDATE_STATUS_START,
	UPDATE_STATUS_SUCCESS,
	UPDATE_STATUS_FAIL,
	UPDATE_STATUS_END,
	UPDATE_STATUS_STOP
};

typedef enum _UPDATE_ERROR
{
	UPDATE_ERROR_NOERROR = 0,
	UPDATE_ERROR_OPEN_FILE,
	UPDATE_ERROR_READ_FILE,
	UPDATE_ERROR_WRITE_FILE,
	UPDATE_ERROR_ALLOC_MEMORY,
	UPDATE_ERROR_HEADER,
	UPDATE_ERROR_DATA,
	UPDATE_ERROR_CRC,
	UPDATE_ERROR_DEVICE
};

class CPanelUpdateARM :
	public CPanelGL
{
public:
	CPanelUpdateARM(void);
	~CPanelUpdateARM(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
    void OnDeviceChanged();

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
    BOOL DownloadFile(LPCTSTR lpszFileName, UINT IoCtl);
#if CVTE_EN_PACKAGE_UPDATE
    BOOL DownloadCfg();
#else
	BOOL DownloadCfg(LPCTSTR lpszFileName);
#endif
    BOOL UpdateCfg(LPBYTE lpBuf, DWORD dwLen);

	BOOL IsUpdateFileExists(LPCTSTR   lpszFileName) ;
	void StartUpdate(BYTE nUpdateType);
	static DWORD UpdateThreadPro(LPVOID lpParam);
    static DWORD ProgressThreadPro(LPVOID lpParam);
    void UpdateUIProc();
    void UpdateLogoProc();
	void UpdateProc();
	void UpdateResProc();

    BOOL IsFileExists(LPCTSTR   lpszFileName);
    BOOL LoadImageFile(const wchar_t *pFileName);
	void RequestUpdateMCU(BOOL bUpdate);
	void OnRequestUpdateMCUSucceed();
	void OnRequestUpdateMCUFailed();

#if CVTE_EN_PACKAGE_UPDATE
    BOOL CheckPackageHeader(LPCTSTR lpFileName);
    BOOL IsUpdateFile(LPCTSTR lpFileName);
    BOOL InstallPackage(LPCTSTR lpszFileName);
#endif

	BOOL CheckResHeader(LPCTSTR lpFileName);
	void SetUpdateStatus(BYTE nUpdateStatus);
	BYTE GetUpdateStatus();
	void SetUpdateType(BYTE nUpdateType);
	BYTE GetUpdateType();
	void SetUpdateError(BYTE nUpdateError);
	BYTE GetUpdateError();
	BOOL InstallRes(LPCTSTR lpszFileName);
private:
    HANDLE      m_hUPD;
    HANDLE      m_hOPD;

    HWND        m_hwnd;
    OEMSECTOR   m_oemsector;

	CM_String   m_strNotification;
	CM_String   m_strTitle;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;


    CButtonGL   m_btnReturn;
    CButtonGL   m_btnUpdateUI;
    CButtonGL   m_btnUpdateLogo;
    CButtonGL   m_btnUpdateAll;
    CButtonGL   m_btnUpdateMCU;
    CButtonGL   m_btnUpdateDAB;
	CButtonGL	m_btnUpdateDynamicLogo;

	CSliderGL   m_sliProgress;
	CLabelGL    m_iconLoading;
    CLabelGL    m_iconImage;
    //CButtonGL   m_btnLogo1;
	CLabelGL    *m_ptagLogo;
    CButtonGL    m_tagMsg;

	CDib        m_Dib;
	DWORD       m_dwDibWidth;
	DWORD       m_dwDibHight;
	DWORD       m_dwDibAbsHight;
	DWORD       m_dwUpdateMcuRequestFlag;
	DWORD       m_bOnRequestMcuUpdate;
    DWORD       m_dwUpdateStart;

    BYTE        m_nUpdateType;
	BYTE		m_nUpdateStatus;
	BYTE		m_nUpdateError;
	BOOL        m_bFileExitUI;
	BOOL        m_bFileExitLogoBmp;
	BOOL        m_bFileExitLogoRaw;
	BOOL        m_bFileExitImage;
	BOOL		m_bFileExitRes;

};
