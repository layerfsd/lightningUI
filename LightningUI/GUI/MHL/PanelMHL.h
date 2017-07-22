#pragma once
#include "../Bluetooth FLC/BluetoothCommon.h"
#include <windev.h>

#include <FantasyLib.h>
#include "../Common/ui_common.h"

#include "../../Communication/CommanderEx.h"
#include "../../VIP/CVideoWindow.h"

#define DEBUG_MHL			0

#define IOCTL_MHL_INIT			CTL_CODE(FILE_DEVICE_UNKNOWN, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MHL_START		CTL_CODE(FILE_DEVICE_UNKNOWN, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MHL_STOP		CTL_CODE(FILE_DEVICE_UNKNOWN, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MHL_RCP			CTL_CODE(FILE_DEVICE_UNKNOWN, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MHL_RESET		CTL_CODE(FILE_DEVICE_UNKNOWN, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)


typedef enum _Video_State_Type {
	VSTATE_Off=0,
	VSTATE_TerminationOff,
	VSTATE_TerminationOn,
	VSTATE_5VOff,
	VSTATE_SyncWait,  
	VSTATE_SWReset,
	VSTATE_SyncChecking,
	VSTATE_HDCPSet,
	VSTATE_HDCP_Reset,
	VSTATE_ModeDetecting,
	VSTATE_VideoOn,
	VSTATE_ColorDetectReset,
	VSTATE_HDMI_OFF,
	VSTATE_Reserved
} Video_State_Type;

enum
{
	RCP_SELECT          = 0x00,
	RCP_UP              = 0x01,
	RCP_DOWN            = 0x02,
	RCP_LEFT            = 0x03,
	RCP_RIGHT           = 0x04,
	RCP_RIGHT_UP        = 0x05,
	RCP_RIGHT_DOWN      = 0x06,
	RCP_LEFT_UP         = 0x07,
	RCP_LEFT_DOWN       = 0x08,
	RCP_ROOT_MENU       = 0x09,
	RCP_SETUP_MENU      = 0x0A,
	RCP_CONTENTS_MENU   = 0x0B,
	RCP_FAVORITE_MENU   = 0x0C,
	RCP_EXIT            = 0x0D,

	//0x0E - 0x1F are reserved

	RCP_NUM_0           = 0x20,
	RCP_NUM_1           = 0x21,
	RCP_NUM_2           = 0x22,
	RCP_NUM_3           = 0x23,
	RCP_NUM_4           = 0x24,
	RCP_NUM_5           = 0x25,
	RCP_NUM_6           = 0x26,
	RCP_NUM_7           = 0x27,
	RCP_NUM_8           = 0x28,
	RCP_NUM_9           = 0x29,

	RCP_DOT             = 0x2A,
	RCP_ENTER           = 0x2B,
	RCP_CLEAR           = 0x2C,

	//0x2D - 0x2F are reserved

	RCP_CH_UP           = 0x30,
	RCP_CH_DOWN         = 0x31,
	RCP_PRE_CH          = 0x32,
	RCP_SOUND_SELECT    = 0x33,
	RCP_INPUT_SELECT    = 0x34,
	RCP_SHOW_INFO       = 0x35,
	RCP_HELP            = 0x36,
	RCP_PAGE_UP         = 0x37,
	RCP_PAGE_DOWN       = 0x38,

	//0x39 - 0x40 are reserved

	RCP_VOL_UP	        = 0x41,
	RCP_VOL_DOWN        = 0x42,
	RCP_MUTE            = 0x43,
	RCP_PLAY            = 0x44,
	RCP_STOP            = 0x45,
	RCP_PAUSE           = 0x46,
	RCP_RECORD          = 0x47,
	RCP_REWIND          = 0x48,
	RCP_FAST_FWD        = 0x49,
	RCP_EJECT           = 0x4A,
	RCP_FWD             = 0x4B,
	RCP_BKWD            = 0x4C,

	//0x4D - 0x4F are reserved

	RCP_ANGLE            = 0x50,
	RCP_SUBPICTURE       = 0x51,

	//0x52 - 0x5F are reserved

	RCP_PLAY_FUNC       = 0x60,
	RCP_PAUSE_PLAY_FUNC = 0x61,
	RCP_RECORD_FUNC     = 0x62,
	RCP_PAUSE_REC_FUNC  = 0x63,
	RCP_STOP_FUNC       = 0x64,
	RCP_MUTE_FUNC       = 0x65,
	RCP_UN_MUTE_FUNC    = 0x66,
	RCP_TUNE_FUNC       = 0x67,
	RCP_MEDIA_FUNC      = 0x68,

	//0x69 - 0x70 are reserved

	RCP_F1              = 0x71,
	RCP_F2              = 0x72,
	RCP_F3              = 0x73,
	RCP_F4              = 0x74,
	RCP_F5              = 0x75,

	//0x76 - 0x7D are reserved

	RCP_VS              = 0x7E,
	RCP_RSVD            = 0x7F

};

class CPanelMHL :
	public CPanelGL
{
public:
	CPanelMHL(void);
	~CPanelMHL(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnSRCChanged(UINT idSRC);

	BOOL OnKey(UINT idKey);

	void LinkVideoWindow(CVideoWindow *pVideoWindow);
	void OnMHLMsg(WPARAM wParam, LPARAM lParam);
	void OnRCPKey(BYTE ucKey);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

// 	static void CALLBACK OnVideoWndDown(POINT *pt,LPVOID lparam);
// 	static void CALLBACK OnVideoWndUp(POINT *pt,LPVOID lparam);
// 	static void CALLBACK OnVideoWndMove(POINT *pt,LPVOID lparam);
// 	static void CALLBACK OnVideoWndDblClick(POINT *pt,LPVOID lparam);

	static void CALLBACK OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lparam);

	void OnSpeechStart();
	void OnSpeechTerminate();



protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	BOOL IniMHL();
	BOOL DeiniMHL();

	void ShowAlert(BOOL bShow){m_bShowAlert = bShow;}
	void SetVideoState(Video_State_Type state);

	void StartMHL();
	void StopMHL();

	HANDLE m_hDevMHL;

private:

	CVideoWindow				*m_pVideoWindow;

	BOOL				m_bIniMHL;
	//toolbar:
// 	CLabelGL			m_iconHomeUp;
// 	CLabelGL			m_iconHomeDown;
// 
// 	CButtonGL		m_btnHome;

	DWORD	m_dwLastFullScreenTick;

	BOOL m_bFullScreenMode;

	BOOL			m_bSpeechOn;

	CLabelGL		m_iconMHL;
	CLabelGL		m_iconAlert;
	BOOL				m_bShowAlert;

	Video_State_Type	m_videoState;

	CButtonGL		m_btnCancel;

	CButtonGL		m_tagTitle;

	DWORD			m_dwEnterTick;
};
