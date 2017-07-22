#pragma once
#include "objectgl.h"
#include "TextGL.h"

#define ROTATE_ANTICLOCK	0
#define ROTATE_CLOCK			1


#define RENDER_ALL				0
#define RENDER_SIMPLE			1
#define RENDER_ONLY_BACK	2
#define RENDER_NONE			3

#define NOTIFY_NULL							0
#define NOTIFY_SHORT_CLICK				1
#define NOTIFY_LONG_CLICK				2
#define NOTIFY_HOLD_CLICK				3
#define NOTIFY_DOUBLE_CLICK			4
#define NOTIFY_VALUE_CHANGE			5
#define NOTIFY_CONTROL					6
#define NOTIFY_TOUCH_DOWN           7
#define NOTIFY_TOUCH_UP					8

typedef void (CALLBACK* NOTIFYPROC)(DWORD,DWORD,DWORD,LPVOID,LPVOID);


class CControlGL :
	public CObjectGL
{
public:
	CControlGL(void);
	~CControlGL(void);

	virtual BOOL Initialize(UINT id,CControlGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	virtual void OnTouchDown(POINT *pt)=0;
	virtual void OnTouchUp(POINT *pt)=0;
	virtual void OnTouchMove(POINT *pt)=0;

	virtual void OnGesture(GESTUREINFO *pgi)=0;

	virtual BOOL IsPtnOnObject(POINT *pt);

	virtual void Render()=0;

	virtual BOOL IsReady(void);
	virtual BOOL IsAniReady(void);

	virtual void Enable(BOOL bEnable);

	virtual void LinkTextGL(CTextGL *pTextGL);
	UINT GetID(void);
	void UpdateID(UINT id);

	BOOL IsActived(void);
	HWND GetMainWnd();

	virtual BOOL BeginRender();
	BOOL EndRender();

	void SetRenderMode(UINT iMode);
	void SetNotifyFunc(NOTIFYPROC pFunc,LPVOID lparam);
	BOOL IsNotifyEnabled();
	void EnableNotify(BOOL bEnable);

	BOOL HasMoveNotify();

	void SetParent(CControlGL *pParent);
	BOOL SetSwitchInControl(CControlGL *pCtrlIn);

	void MakeDefaultAni(UINT idAni,LPVOID lpAniPara=0);

protected:
	BOOL RenderSwitchInCtl();

	void AttachChildControl(CControlGL *pCtr);
	void DettachChildControl(CControlGL *pCtr);

protected:
	CM_List<CControlGL>	m_listChildControls;

	CTextGL	*m_pTextGL;
	NOTIFYPROC	m_lpNotifyFunc;
	LPVOID					m_lparam;

	CControlGL		*m_pCtlSwitchIn;
	BOOL				m_bSwitchFlag;

	BOOL					m_bActived;
	BOOL					m_bEnableChildAni;
	BOOL					m_bOnlyChildAni;

	UINT					m_ID;

	HWND					m_hMainWnd;
	HINSTANCE			m_hInstance;
	MODULEENTRY	m_moduleEntry;

	UINT		m_idRenderMode;

	BOOL	m_bEnableNotify;

	BOOL				m_bNotifyFlag;
	UINT					m_idNotifyID;

	BOOL				m_bHasMoveNotify;
};
