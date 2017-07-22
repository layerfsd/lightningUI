#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../Radio/RadioCommon.h"

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
typedef void (*ChangePageIconCallback_f )(UINT nPageNum, PVOID pContext);
#endif

#define DEBUG_MAINMENU						0

// #define MAIN_MENU_PAGE_COUNT						3
#define MAX_PAGE_COUNT									10

#define MAIN_MENU_MAX_APP_COUNT				32

#define ENABLE_WIDGET_TV				1

typedef struct _HomeWidgetParam
{
	int 			nWidgetID;
	int 			nWidth;
	int 			nHeight;
	int 			nPX;
	int 			nPY;
	bool  			bVisible;
}HomeWidgetParam;

class CWidgetGL :
	public CControlGL
{
public:
	CWidgetGL(void);
	~CWidgetGL(void);

	virtual BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	virtual BOOL IsReady(void);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void SetPos(GLfloat x,GLfloat y,GLfloat z=0,BOOL bAni=FALSE);

	BOOL IsOnFocus();

	virtual void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara){}

protected:
	void ProcessNotify();

protected:
	BOOL				m_bPressed;
	BOOL				m_bOnFocus;
	DWORD			m_dwLastPressTick;
	DWORD			m_dwLastGestureTick;

	CControlGL						*m_pCurActiveControl;
	CButtonGL						*m_ptagTitle;
// 	BOOL				m_bClickOnControl;
// 	UINT									m_idCurControlDown;
// 	UINT									m_idCurControlMove;
// 	UINT									m_idCurControlUp;
	BOOL								m_bEnableSendNotify;
	UINT				m_iNotifyPara;

};

class CWidgetMediaCore :
	public CWidgetGL
{
public:
	CWidgetMediaCore(void);
	~CWidgetMediaCore(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	UINT					m_idCurMediaType;

	CLabelGL		m_tagAlbumArt;
// 	CSliderGL			m_sliProgress;
	CButtonGL		m_tagTitle;
	CLabelGL		m_tagVideo;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL		m_iconMedia;
	CLabelGL		m_iconMediaSelect;
	CButtonGL		m_btnMedia;
	CButtonGL		m_tagMediaType;
#endif

	CLabelGL		m_tagAlbumArtForIpod;

	CLabelGL		m_tagUSB;
	CLabelGL		m_tagSD;
	CLabelGL		m_tagBT;
	CLabelGL		m_tagiPod;
	CLabelGL		m_tagIconBack;

	//play info
	CButtonGL		m_tagTrack;
	CButtonGL		m_tagArtist;

	CButtonGL		m_btnPlay;
	CButtonGL		m_btnPause;

	BOOL				m_bVideoMode;
	BOOL				m_bHasAlbumArt;
	CM_String		m_track;
	CM_String		m_artist;

};

class CWidgetRadioCore :
	public CWidgetGL
{
public:
	CWidgetRadioCore(void);
	~CWidgetRadioCore(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);
	UINT GetType(){return m_idType;}

public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:

	UINT			m_idType;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CLabelGL		m_tagRadio;
	CButtonGL		m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	//CLabelGL		m_tagRadio;
	CLabelGL		m_iconRadio;
	CButtonGL		m_tagFrequency;
	CButtonGL       m_btnPsInfo;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL		m_iconRadio;
	CLabelGL		m_iconRadioSelect;
	CButtonGL		m_btnRadio;

	CButtonGL		m_tagTitle;
	CButtonGL		m_tagFrequency;
	CButtonGL		m_tagFreqUnit;	

	CButtonGL		m_tagRadioStation;

	RadioBandId		m_idRadioband;
	CM_String		m_ps;
#else
	CLabelGL		m_tagRadio;

	CButtonGL		m_tagTitle;
	CButtonGL		m_tagFrequency;
	CButtonGL		m_tagBandTitle;
	CButtonGL		m_tagFreqUnit;

	CButtonGL		m_btnPowerOn;
	CButtonGL		m_btnPowerOff;
#endif
};

class CWidgetPhoneCore :
	public CWidgetGL
{
public:
	CWidgetPhoneCore(void);
	~CWidgetPhoneCore(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagPhone;
	CButtonGL		m_tagbk;
#else
	CLabelGL		m_tagPhone;
#endif	

	CButtonGL		m_tagTitle;

	BOOL				m_bPlugin;
	CLabelGL			m_tagPlugin;

};

class CWidgetA2DP :
	public CWidgetGL
{
public:
	CWidgetA2DP(void);
	~CWidgetA2DP(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CLabelGL		m_tagA2DP;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL		m_tagA2DP;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagA2DP;
	CButtonGL		m_tagbk;
#else
	CButtonGL		m_tagA2DP;
#endif
	CButtonGL		m_tagTitle;

	BOOL				m_bPlugin;
	CLabelGL			m_tagPlugin;

};

class CWidgetNaviCore :
	public CWidgetGL
{
public:
	CWidgetNaviCore(void);
	~CWidgetNaviCore(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);
	
private:

// 	CButtonGL		m_tagNavi;
	CButtonGL		m_tagTitle;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagMap;
	CButtonGL		m_tagbk;
#else
	CLabelGL		m_tagMap;
#endif

	BOOL			m_bPlugin;
};


class CWidgetVideoCore :
	public CWidgetGL
{
public:
	CWidgetVideoCore(void);
	~CWidgetVideoCore(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:
	CButtonGL		m_tagVideo;
	CButtonGL		m_tagTitle;

};

class CWidgetTVCore :
	public CWidgetGL
{
public:
	CWidgetTVCore(void);
	~CWidgetTVCore(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);
private:

	CButtonGL		m_tagTV;
	CButtonGL		m_tagTitle;
	CButtonGL		m_tagbk;

	BOOL			m_bPlugin;
};
class CWidgetDiskCore :
	public CWidgetGL
{
public:
	CWidgetDiskCore(void);
	~CWidgetDiskCore(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:

	CButtonGL		m_tagDisk;
	CButtonGL		m_tagTitle;

};


class CWidgetSettingCore :
	public CWidgetGL
{
public:
	CWidgetSettingCore(void);
	~CWidgetSettingCore(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);
	
private:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CLabelGL		m_tagSetting;
#elif(CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL		m_tagSetting;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagSetting;
	CButtonGL		m_tagbk;
#else
	CButtonGL		m_tagSetting;
#endif

	CButtonGL		m_tagTitle;

};

class CWidgetDSP :
	public CWidgetGL
{
public:
	CWidgetDSP(void);
	~CWidgetDSP(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CLabelGL		m_tagSetting;
#elif(CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL		m_tagSetting;
#else
	CButtonGL		m_tagSetting;
#endif

	CButtonGL		m_tagTitle;

};

class CWidgetAUXCore :
	public CWidgetGL
{
public:
	CWidgetAUXCore(void);
	~CWidgetAUXCore(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CLabelGL        m_tagAUX;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL        m_tagAUX;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagAUX;
	CButtonGL		m_tagbk;
#else
	CButtonGL		m_tagAUX;
#endif

	CButtonGL		m_tagTitle;
	CLabelGL		m_tagPlugin;

	BOOL			m_bPlugin;
};

class CWidgetImageCore :
	public CWidgetGL
{
public:
	CWidgetImageCore(void);
	~CWidgetImageCore(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:

	CButtonGL		m_tagImage;
	CButtonGL		m_tagTitle;
};

class CWidgetiPod :
	public CWidgetGL
{
public:
	CWidgetiPod(void);
	~CWidgetiPod(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:

	//CLabelGL			m_tagiPod;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CLabelGL			m_tagiPod;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL			m_tagiPod;	
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL			m_tagbk;
	CButtonGL			m_tagiPod;
#else
	CLabelGL			m_tagiPod;
#endif
	CButtonGL		m_tagTitle;

	BOOL				m_bPlugin;
	CLabelGL			m_tagPlugin;

};

class CWidgetCarPlay :
	public CWidgetGL
{
public:
	CWidgetCarPlay(void);
	~CWidgetCarPlay(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	CLabelGL			m_tagCarPlay;
	CLabelGL			m_tagCarPlayPlugin;
	CButtonGL		m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL			m_tagCarPlayActive;
	CLabelGL			m_tagCarPlayDisable;
	CButtonGL			m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL			m_tagCarPlay;
	CButtonGL			m_tagbk;
	CButtonGL			m_tagTitle;
#else
	CLabelGL			m_tagCarPlayActive;
	CLabelGL			m_tagCarPlayDisable;
	CButtonGL		m_tagTitle;
 	CLabelGL			m_tagPlugin;	
#endif

	BOOL				m_bPlugin;

};



class CWidgetVehicleInfo :
	public CWidgetGL
{
public:
	CWidgetVehicleInfo(void);
	~CWidgetVehicleInfo(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagbk;
#endif
	CButtonGL		m_tagVehicle;
	CButtonGL		m_tagTitle;
};

class CWidgetMHL :
	public CWidgetGL
{
public:
	CWidgetMHL(void);
	~CWidgetMHL(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:

	CButtonGL		m_tagMHL;
	CButtonGL		m_tagTitle;
};
class CWidgetPhoneLink :
	public CWidgetGL
{
public:
	CWidgetPhoneLink(void);
	~CWidgetPhoneLink(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:

	CLabelGL		m_tagPhoneLink;
	CButtonGL		m_tagTitle;

	BOOL				m_bPlugin;
	CLabelGL			m_tagPlugin;
};
class CWidgetEasyConnected :
	public CWidgetGL
{
public:
	CWidgetEasyConnected(void);
	~CWidgetEasyConnected(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
    void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);
private:

	CLabelGL		m_tagEasyConnected;
	CLabelGL        m_tagPlugin;
	CButtonGL		m_tagbk;
	CButtonGL		m_tagMirrorLink;
	CButtonGL		m_tagTitle;
	BOOL			m_bPlugin;
};

class CWidgetWeblink :
	public CWidgetGL
{
public:
	CWidgetWeblink(void);
	~CWidgetWeblink(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);
private:

	CLabelGL		m_tagWeblink;
	CLabelGL        m_tagPlugin;
	CButtonGL		m_tagTitle;
	BOOL			m_bPlugin;
};

class CWidgetAndroidAuto :
	public CWidgetGL
{
public:
	CWidgetAndroidAuto(void);
	~CWidgetAndroidAuto(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);
private:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagAndroidAuto;
	CButtonGL		m_tagbk;
#else
	CLabelGL		m_tagAndroidAuto;
	CLabelGL		m_tagAndroidAutoDisable;
#endif
	CLabelGL        m_tagPlugin;
	CButtonGL		m_tagTitle;
	BOOL			m_bPlugin;
};

class CWidgetDateTime :
	public CWidgetGL
{
public:
	CWidgetDateTime(void);
	~CWidgetDateTime(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:
	CClockBarGL		m_clock;
	CButtonGL		m_tagTitle;

	CButtonGL		m_tagDay;
	CButtonGL		m_tagMonth;
	CButtonGL		m_tagWeek;

};

class CWidgetUSB :
	public CWidgetGL
{
public:
	CWidgetUSB(void);
	~CWidgetUSB(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:
#if (CVTE_DEF_UI_STYLE ==CVTE_UI_STYLE_FIATUI)
	CLabelGL		m_tagUSB;
#elif (CVTE_DEF_UI_STYLE ==CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL		m_tagUSB;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL		m_tagUSB;
	CLabelGL		m_labUSBDisable;
	CLabelGL		m_labUSBNormal;
	CLabelGL		m_labUSBSelect;
	CButtonGL		m_tagbk;
	CLabelGL		m_labbkDown;
	CLabelGL		m_labbkNormal;
	CLabelGL		m_labbkSelect;

#else
	CButtonGL		m_tagUSB;
#endif
	CButtonGL		m_tagTitle;

	BOOL				m_bPlugin;
	CLabelGL			m_tagPlugin;
};

class CWidgetSD :
	public CWidgetGL
{
public:
	CWidgetSD(void);
	~CWidgetSD(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:
	CButtonGL		m_tagSD;
	CButtonGL		m_tagTitle;

	BOOL				m_bPlugin;
	CLabelGL			m_tagPlugin;

};

class CWidgetCamera :
	public CWidgetGL
{
public:
	CWidgetCamera(void);
	~CWidgetCamera(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

private:

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CLabelGL		m_tagCamera;
#else
	CButtonGL		m_tagCamera;
#endif
	CButtonGL		m_tagbk;
	CButtonGL		m_tagTitle;

};

class CWidgetDAB :
	public CWidgetGL
{
public:
	CWidgetDAB(void);
	~CWidgetDAB(void);

	BOOL IsReady();

	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);
	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:

	CLabelGL		m_tagDAB;
	CButtonGL		m_tagTitle;
	CButtonGL		m_tagFrequency;

};

class CWidgetISDBCore :
	public CWidgetGL
{
public:
	CWidgetISDBCore(void);
	~CWidgetISDBCore(void);

	BOOL IsReady();
	void Render();
	BOOL Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OperateControl(UINT idControl,UINT idOperation,LPVOID lpara);

private:
	CButtonGL	m_tagISDB;

	CButtonGL	m_tagTitle;
	CLabelGL		m_tagPlugin;

	BOOL		m_bPlugin;
};


class CPanelWidget :
	public CPanelGL
{
public:
	CPanelWidget(void);
	~CPanelWidget(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGestureMsg(UINT idGesture);

	void OnGesture(GESTUREINFO *pgi);

	//void SetCurrentWidgetID(UINT idClass);
	void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

	void OperateWidget(UINT idUI,UINT idControl,UINT idOperation,LPVOID lpara);

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif

	void OnWidgetChanged(WPARAM wParam, LPARAM lParam);
	void OnCanbusStatus(WPARAM wParam, LPARAM lParam);

	CWidgetGL*	GetWidget(UINT idWidget);
	void OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
public:
	void SetCallback(ChangePageIconCallback_f pFun, PVOID pContext){m_pChangeHomePageCallback = pFun;m_lparam = pContext;}
	void SetPage(UINT nPageNum);

private:
	ChangePageIconCallback_f m_pChangeHomePageCallback;
	LPVOID					m_lparam;
#endif

public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
    BOOL InitializeWidget(CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry,HomeWidgetParam *pParam, size_t count);

private:
	BOOL CreateWidget(UINT id,int cx,int cy,GLfloat px,GLfloat py,BOOL bCreateVisible=TRUE);
	BOOL CalculatePageSize(SIZE *pSize);

private:

	CM_List<CWidgetGL>	m_listWidgets;

	CWidgetGL		*m_pCurWidget;

// 	CWidgetGL		*m_pWidgetSwap;

	BOOL			m_bPhoneCallActive;

	BOOL			m_bHasCanbus;

	BOOL           m_bCarPlayConnected;
	BOOL			m_bAndroidAutoConnect;
	BOOL		   m_biPodConnected;

#if (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON == CVTE_ENABLE)
	//DWORD			m_dwBtnSwitchLastPressTick;
	UINT			m_idBtnStatusSwitch;
	CLabelGL		m_iconSwitchFocus;
	CLabelGL		m_iconSwitch;
	CButtonGL		m_btnSwitch;
	BOOL			m_bShowiPod;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	GLfloat			m_gfPageX;
#endif
};
