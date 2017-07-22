#pragma once
#include "BluetoothCommon.h"

#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"

#include "../../MobileData/inc/MobileData.h"
#include "../Controls/SmartBarPhone.h"

class CPanelBluetoothTalking :
	public CPanelGL
{
public:
	CPanelBluetoothTalking(void);
	~CPanelBluetoothTalking(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void LinkSmartBarPhone(CSmartBarPhone *pSmartBar){m_pSmartBarPhone = pSmartBar;}

	void SetAudioDir(UINT idDir);
	void SetMicMute(BOOL bMute);

	void SelectCallIndex(DWORD dwIndex);

	//notify:
	void OnNotifyCallCount(DWORD dwCount);
	void OnNotifyCallStatus(strCallStatus* callStatus);

	void OnCarPlayConnected(BOOL bConnect){m_bCarPlayConnected=bConnect;}

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void Input(wchar_t c);
	void SetDTMFMode(BOOL bDTMF);

private:

	CMobileData	m_MobileData;
	CSmartBarPhone	*m_pSmartBarPhone;
	BOOL					m_bCarPlayConnected;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL			m_iconKeyboard;
	CLabelGL			m_iconHang;
	CLabelGL			m_iconAnswer;
	CLabelGL			m_iconACHBT;
	CLabelGL			m_iconACHPhone;
	CLabelGL			m_iconMic;
	CLabelGL			m_iconMicOff;
#else
	CLabelGL			m_iconKeyboard;
	CLabelGL			m_iconHang;
	CLabelGL			m_iconAnswer;

	CLabelGL			m_iconKeyboardFocus;

	CLabelGL			m_iconACHBT;
	CLabelGL			m_iconACHBTFocus;
	CLabelGL			m_iconACHPhone;
	CLabelGL			m_iconACHPhoneFocus;
	CLabelGL			m_iconMic;
	CLabelGL			m_iconMicFocus;
#endif

	CButtonGL	m_btnKeyboard;
	CButtonGL	m_btnACH;
	CButtonGL	m_btnMic;

	CButtonGL	m_btnAnswer1;
	CButtonGL	m_btnHang1;
	CButtonGL	m_btnAnswer2;
	CButtonGL	m_btnHang2;

	CButtonGL	m_btnCall1;
	CButtonGL	m_btnCall2;

	BOOL			m_bOnDTMF;

	//
	CButtonGL	m_btnNum0;
	CButtonGL	m_btnNum1;
	CButtonGL	m_btnNum2;
	CButtonGL	m_btnNum3;
	CButtonGL	m_btnNum4;
	CButtonGL	m_btnNum5;
	CButtonGL	m_btnNum6;
	CButtonGL	m_btnNum7;
	CButtonGL	m_btnNum8;
	CButtonGL	m_btnNum9;
	CButtonGL	m_btnStar;
	CButtonGL	m_btnWell;


	CM_String		m_strDialString;
	CButtonGL	m_tagDialString;

	DWORD		m_dwCallCount;
	DWORD		m_dwCurCallIndex;
	BOOL			m_bEnableAutoSelCall;

	DIAL_STATE	m_iCallState1;
	DIAL_STATE	m_iCallState2;
	CM_String		m_strCallStatus1;
	CM_String		m_strName1;
	CM_String		m_strNumber1;
	DWORD		m_dwCallTime1;

	CM_String		m_strCallStatus2;
	CM_String		m_strName2;
	CM_String		m_strNumber2;
	DWORD		m_dwCallTime2;

	BOOL			m_bTalking1;
	BOOL			m_bTalking2;

	CM_String		m_strMobileArea1;
	CM_String		m_strMobileArea2;
	CM_String		m_strMobileType1;
	CM_String		m_strMobileType2;

	#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CM_String	m_MuteString;
	#endif

	DWORD		m_dwEnterTick;
	BOOL			m_bNeedAutoShowSmartBar;
	BOOL        m_bAudioInHandfree;  
};

