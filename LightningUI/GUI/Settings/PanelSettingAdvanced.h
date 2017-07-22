#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"

//	#define PASSWORD_MAX_LENGTH 4

typedef struct steerwheelkey
{
    BYTE keyname;
    BYTE keyvalue;
    BOOL valid;
}STEERWHEELKEY;

class CPanelSettingAdvanced :
	public CPanelGL
{
public:
	CPanelSettingAdvanced(void);
	~CPanelSettingAdvanced(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void SetCamerType(UINT idType);
	void OnCameraTypeChange(UINT uCameraType);
    void OnMCUCmd(WPARAM wParam, LPARAM lParam);
//	    void OnRearColorSignalChange(UINT uColorSignal);
    static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
private:
//	    void Input(wchar_t c);
//	    void ClearInput(int i);
//	    BOOL VerifyPassword(LPCTSTR lpPassword);

    //void RefreshPassword();
    void UpdateSteerWheelPara(BYTE item,INT iSet);
    void QuerySteerWheelValue();
    BOOL CheckPara(BYTE nSet);
    void ClearPara();
	void StartSTW();
	void ClearSTW();
    void SteerWheelShow(BOOL bShow);
	void Lock(BOOL bLock);
	void CheckCalibrate();
private:
    BOOL        m_bSteerWheelBlockShow;
    BOOL        m_bSteerWheelKeyCalibrating;
    BOOL        m_bMCUUpdating;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
    BOOL        m_bRDSEnable;
#endif
    BOOL        m_bVerify;
    BOOL        m_bPasswordHide;
    BYTE        m_nSteerWheelCurrentStatus;
    BYTE        m_nCalibCurrentKey;
    BYTE        m_nQueryKey;
    STEERWHEELKEY m_steerwheelkey[KEY_STW_LASTKEY-KEY_STW_FIRSTKEY];
    BOOL        m_bRearViewColorSignalReset;
	BOOL		m_bLock;
	BOOL		m_bCalibrated;
	CM_List<STEERWHEELKEY>	m_listSTW;

//	    CM_String   m_strPassword;
//		CLabelGL    m_tagBkBlur;
//	    BOOL        m_btagNumberInput[PASSWORD_MAX_LENGTH];
//	    CButtonGL   m_tagPasswordTitle;
//	    CButtonGL   m_tagNumber[PASSWORD_MAX_LENGTH];
//	    CButtonGL	m_btnNumber[10];
    CButtonGL   m_btnReturn;

	CLabelGL    m_iconLock;
	CLabelGL    m_iconUnLock;
	CButtonGL   m_btnLock;

	CButtonGL	m_btnCameraType;
	CButtonGL	m_tagValueCameraType;

    CButtonGL   m_tagSteerWheelPanel;

    CButtonGL   m_tagTitleStreerWheel1;
    CButtonGL   m_tagTitleStreerWheel2;

    CButtonGL   m_btnSteerWheelConfirmFocus;
    CButtonGL   m_btnSteerWheelReset;
    CButtonGL   m_btnSteerWheelConfirm;
    CButtonGL   m_btnSteerWheelCancel;

    CButtonGL   m_btnStreerWheelSpread;
    CLabelGL    m_iconSteerWheelFocus;

    CLabelGL    m_iconVolPlus;
    CLabelGL    m_iconVolPlusFocus;
    CButtonGL   m_btnVolPlus;

    CLabelGL    m_iconVolMinus;
    CLabelGL    m_iconVolMinusFocus;
    CButtonGL   m_btnVolMinus;

    CLabelGL    m_iconVolMute;
    CLabelGL    m_iconVolMuteFocus;
    CButtonGL   m_btnVolMute;

    CLabelGL    m_iconPrev;
    CLabelGL    m_iconPrevFocus;
    CButtonGL   m_btnPrev;

    CLabelGL    m_iconNext;
    CLabelGL    m_iconNextFocus;
    CButtonGL   m_btnNext;

    CLabelGL    m_iconSeek;
    CLabelGL    m_iconSeekFocus;
    CButtonGL   m_btnSeek;

    CLabelGL    m_iconCall;
    CLabelGL    m_iconCallFocus;
    CButtonGL   m_btnCall;

    CLabelGL    m_iconHangup;
    CLabelGL    m_iconHangupFocus;
    CButtonGL   m_btnHangup;

    CLabelGL    m_iconContact;
    CLabelGL    m_iconContactFocus;
    CButtonGL   m_btnContact;

    CLabelGL    m_iconNavi;
    CLabelGL    m_iconNaviFocus;
    CButtonGL   m_btnNavi;


    CLabelGL    m_iconMode;
    CLabelGL    m_iconModeFocus;
    CButtonGL   m_btnMode;

    CLabelGL    m_iconSpeech;
    CLabelGL    m_iconSpeechFocus;
    CButtonGL   m_btnSpeech;

    CLabelGL    m_iconPower;
    CLabelGL    m_iconPowerFocus;
    CButtonGL   m_btnPower;

	CLabelGL    m_iconHome;
	CLabelGL    m_iconHomeFocus;
	CButtonGL   m_btnHome;

	CButtonGL   m_tagSteerWheelWindow;

	CButtonGL	m_tagTitleBrakeTest;
	CSliderGL	m_sliBrakeTest;

	CButtonGL	m_btnDriveType;
	CButtonGL	m_tagValueDriveType;

    CButtonGL	m_tagTitleLampControl;
	CSliderGL	m_sliLampControl;


//	    CLabelGL    m_iconRearView;
//
//	    CButtonGL   m_tagTitleRearView;
//	    CButtonGL   m_tagTitleRearView2;
//
//	    CButtonGL   m_btnNTSC_MJ;
//	    CButtonGL   m_btnNTSC_443;
//	    CButtonGL   m_btnPAL_M;
//	    CButtonGL   m_btnPAL_60;
//	    CButtonGL   m_btnPAL_BGHID;
//	    CButtonGL   m_btnPAL_COM_N;
//	    CButtonGL   m_btnSECAM;
//	    CButtonGL   m_btnSECAM_525;
//
//	    CButtonGL   m_btnRearReset;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif

};

