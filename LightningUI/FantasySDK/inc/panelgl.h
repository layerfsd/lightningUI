#pragma once
#include "controlgl.h"
#include "CM_List.h"
#include "StatusBar.h"
#include "Commander.h"
#include "../../Config/customer.h"

class CPanelGL :
	public CControlGL
{
public:
	CPanelGL(void);
	virtual ~CPanelGL(void);

	virtual BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	virtual BOOL IsAniReady(void);
	virtual BOOL IsReady(void);

	virtual void Render()=0;
	virtual void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);//=0;
	virtual void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);//=0;
	virtual void OnBack(){}
	virtual void OnHome(){}
	virtual void SwitchToMainPanel(){}
	virtual void OnSystemPowerStatePrepare(UINT idState);
	virtual void OnSystemPowerStateDone(UINT idState);
	virtual void OnSRCChanged(UINT idSRC){}
	virtual void OnUIChanged(UINT idSRC){}
	virtual void OnReverseModeChange(BOOL bReverse);
	virtual void OnSpeechStart(){}
	virtual void OnSpeechTerminate(){}
	virtual void OnSRStart(){}
	virtual void OnSRTerminate(){}
	virtual void OnPlaySoundEffect(BOOL bPlay){}
	virtual void OnPhoneCallState(WPARAM wParam, LPARAM lParam){}
	virtual void OnBrakeStatusChanged(BOOL bBrake);
	virtual void OnMute(BOOL bMute){}

	virtual void OnTouchDown(POINT *pt);
	virtual void OnTouchUp(POINT *pt);
	virtual void OnTouchMove(POINT *pt);

	virtual void OnGestureMsg(UINT idGesture);

	virtual void OnGesture(GESTUREINFO *pgi);

	virtual BOOL OnKey(UINT idKey,UINT idStatus);

	virtual BOOL IsEnableQuit();
	virtual BOOL IsEnableSmartBar();

	virtual void OnDeviceArrivedReady(LPCTSTR lpDeviceName){}
	virtual void OnDeviceRemovedReady(LPCTSTR lpDeviceName){}
	virtual void OnMCUCmd(WPARAM wParam, LPARAM lParam){}
    virtual void OnFactoryCmd(WPARAM wParam, LPARAM lParam){}

	virtual void UpdateControl(UINT idControl,UINT idOperation,LPVOID lpara){}
	virtual void HandleControlMSG(DWORD id,DWORD wMsg,DWORD wPara,LPVOID lpPara){}

	virtual void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);

	void SlideToPosX(GLfloat x);
	void SlideToPosY(GLfloat y);

// 	void AttachControl(CControlGL *pCtr);
	CControlGL *GetControl(UINT id);
	void LinkStatusBar(CStatusBar *pStatusBar);
	void LinkCommander(CCommander *pCommander);
	CPanelGL* GetChildPanel(UINT id);

// 	UINT	GetActiveDownID();
// 	UINT	GetActiveUpID();
// 	UINT	GetActiveMoveID();
	CPanelGL* GetParentPanel();
	void SetPreviousPanel(UINT idPanel,UINT iParam);
	void ShowStatusBar(BOOL bShow);
	void ShowBackground(BOOL bShow);
	BOOL IsStatusBarVisible();
	BOOL IsBackgroundVisible();
	BOOL IsEnableScreenSaver();
	void	SetFrameTime(DWORD dwSet);
	DWORD GetFrameTime();
	UINT GetPanelParam();
	void SetPanelParam(UINT iParam);

	UINT GetAniMoveIn();
	UINT GetAniPrePanelMoveOut();
	void SetAniMoveIn(UINT idAni);
	void SetAniPrePanelMoveOut(UINT idAni);

	UINT GetAniMoveOut();
	UINT GetAniNextPanelMoveIn();
	void SetAniMoveOut(UINT idAni);
	void SetAniNextPanelMoveIn(UINT idAni);

	void EnableQuit(BOOL bEnable);
	void EnableScrollingX(BOOL bEnable);
	void EnableScrollingY(BOOL bEnable);

protected:
	BOOL BeginRender();

private:
	void AddChildPanel(CPanelGL *pChild);
	void SetParentPanel(CPanelGL *pParent);

protected:
	CPanelGL						*m_pParentPanel;
	UINT								m_idPreviousPanel;
	UINT								m_iPreviousPanelParam;
	UINT								m_iParam;
	CM_List<CPanelGL>	m_listChildrenPanels;

	UINT								m_idAniMoveIn;
	UINT								m_idAniPrePanelMoveOut;
	UINT								m_idAniMoveOut;
	UINT								m_idAniNextPanelMoveIn;

	CControlGL						*m_pCurActiveControl;
	CControlGL						*m_pPrevActiveControl;
// 	UINT									m_idCurControlDown;
// 	UINT									m_idCurControlMove;
// 	UINT									m_idCurControlUp;
	BOOL								m_bSystemReady;
	DWORD							m_dwSystemReadyTick;
	DWORD							m_dwLastPressTick;

// 	UINT									m_idControlActive;

	CStatusBar					*m_pStatusBar;

	CCommander					*m_pCommander;
	BOOL								m_bShowStatusBar;
	BOOL								m_bShowBackground;
	BOOL								m_bEnableQuit;
	DWORD							m_dwFrameTime;
	BOOL								m_bOnBrake;
	BOOL								m_bOnReverseMode;
	UINT									m_idSystemPowerState;
	BOOL								m_bEnableScreenSaver;
	BOOL								m_bPanelEntered;
	BOOL								m_bEnableSmartBar;

	GLfloat								m_posXBeforeScolling;
	GLfloat								m_posYBeforeScolling;

	BOOL								m_bEnableScrollingX;
	BOOL								m_bEnableScrollingY;

	BOOL								m_bBeginScrolling;

	BOOL								m_bScrollingX;
	BOOL								m_bScrollingY;

private:
	BOOL								m_bAutoScrollingX;
	BOOL								m_bAutoScrollingY;
	GLfloat								m_posXScollingCurrent;
	GLfloat								m_posYScollingCurrent;
	GLfloat								m_posXScollingTarget;
	GLfloat								m_posYScollingTarget;
};
