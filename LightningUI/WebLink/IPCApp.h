#ifndef IPCApp_H
#define IPCApp_H

#include "CMsgQueue.h"
#include "cJSON.h"
#include "IIPCCommand.h"
#include "IIPCNotification.h"

class CIPCApp
{
public:
	static CIPCApp& GetInstance();
	bool Init(IIPCNotification* pNotification);
	BOOL ShowWindow(bool show);
	void OnWindowControl(bool show);
	void OnSetCurrentApp(const char * appID, const char * appParams);
	void OnBack();
	void OnHome();
protected:
	CIPCApp(void);
	~CIPCApp(void);
	static BOOL ClientMsgQCallBackProc(PVOID Param, PVOID pData, DWORD dwSize);
	BOOL ClientMsgQCallBack(PVOID pData, DWORD dwSize);
	void CMDProcessInit( cJSON * param );
	void CMDProcessConnect( cJSON * param );
	void CMDProcessWindowStatus( cJSON * param );
private:
	IIPCNotification* m_pNotification;
	CMsgQueue m_ClientMsgQ;
	CMsgQueue m_AppMsgQ;

	bool m_windowshowstatus;
};

#endif