#pragma once
#include "../Bluetooth FLC/BluetoothCommon.h"
#include <windev.h>
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"

#define DEBUG_PHONELINK			0

#define IOCTL_PLN_WRITE			CTL_CODE(FILE_DEVICE_UNKNOWN, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PLN_READ				CTL_CODE(FILE_DEVICE_UNKNOWN, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

//TRANS type
#define TRANS_READ  0
#define TRANS_WRITE  1
#define TRANS_READ_FEEDBACK  2
#define TRANS_WRITE_FEEDBACK  3

//TRANS state
#define TRANS_STATE_SUCCESS  0
#define TRANS_STATE_FAIL  1



typedef struct TRANS_DATA_STRU 
{
	int type;
	int state;
	int count;          // nBytes to read/write
	UCHAR  buf[16];           // Buffer
	UCHAR  device_addr; 
	UCHAR  sub_addr;
}TRANS_DATA;


class CPanelPhoneLink :
	public CPanelGL
{
public:
	CPanelPhoneLink(void);
	~CPanelPhoneLink(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void OnSystemPowerStatePrepare(UINT idState);

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnSRCChanged(UINT idSRC);

	BOOL OnKey(UINT idKey);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

	static LRESULT CALLBACK PhoneLinkWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnPhoneLinkMsg(UINT message, WPARAM wParam, LPARAM lParam);

	void OnDeviceArrivedReady(LPCTSTR lpDeviceName);
	void OnDeviceRemovedReady(LPCTSTR lpDeviceName);

	void NotifyConnected(BOOL bConnected);

	void OnSpeechStart();
	void OnSpeechTerminate();
	void OnPlaySoundEffect(BOOL bPlay);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

	void PostMsgToClient(UINT message, WPARAM wParam, LPARAM lParam);

	void SendMsg();
	BOOL TransDataProcess();
	BOOL PLDeviceRead(TRANS_DATA *data);
	BOOL PLDeviceWrite(TRANS_DATA *data);

private:
	BOOL IniPhoneLinkSrv();
	void DeiniPhoneLinkSrv();

	void SetMsg(const wchar_t *pStrMsg){m_tagTitle.SetCaption(pStrMsg);}

private:

	HWND			m_hwndPhoneLinkNotify;
	HWND			m_hwndClient;
	TRANS_DATA m_data_out;
	TRANS_DATA m_data_in;
	HANDLE	m_hPLDevice;
	BOOL		m_bConnected;
	BOOL		m_bPhoneLinkVisible;
	BOOL		m_bPhoneLinkSrvIni;

	DWORD		m_dwEnterTick;

	//toolbar:
	BOOL			m_bSpeechOn;

	BOOL m_bA2dpOn;

	//CM_String			m_strMsg;
	CButtonGL		m_btnCancel;
	CLabelGL		m_iconLink;
	CButtonGL		m_tagTitle;

	BOOL			m_bAutoSwitchToPhoelink;

	BOOL			m_bDeviceInserted;
};
