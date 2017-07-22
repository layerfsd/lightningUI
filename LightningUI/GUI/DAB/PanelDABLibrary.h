#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../../DAB/Inc/DABRadioLib.h"
#include "../../DAB/Adapter/DABAdapter.h"
#include "DABCommon.h"

#if CVTE_EN_DAB
class CPanelDABLibrary :
	public CPanelGL
{
public:
	CPanelDABLibrary(void);
	~CPanelDABLibrary(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void NotifyDabServiceReady(BOOL bReady);

	void ClearEnsembleBoxList();
	void ClearServiceBoxList();
    void UpdateQueryMode(BYTE nQueryMode);
    BOOL PlayItemAllService(UINT idItem);
	BOOL PlayItemSubService(UINT idItem);

    BOOL PlayPrevious();
    BOOL PlayNext();

    int GetCurIndex();
	const wchar_t* GetIndexValue(INT index, BOOL bMoveIndex = TRUE);
    const wchar_t* GetCurValue();
	const wchar_t* GetNextValue();
	const wchar_t* GetPreviousValue();

	    void UpdateEnsembleList();
    void UpdateAllServiceList();
//     void UpdatePTYList();
	void UpdateSubServiceList(int idxEnsemble);
	void UpdatePtyServiceList(int iPty);

	void UpdateScanStatus(BOOL bScaning);
	bool UpdateCurChannelInfo();

	int GetTotalServiceCnt();

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
    void EnterNextClassService();
    void ReturnPrevClassService();
    void CycleClassService();
    void UpdateQueryTitle();
    void UpdatePTYListTitle();
    void SetShowType(DAB_LIBRARY_TYPE type);
//	    void SavePresetData();

private:
	void ShowPTYServiceDetail(BOOL bShow,int iPty=0);
	void ShowEnsembleServiceDetail(BOOL bShow,int idxEnsemble=0);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	void ArrowChange(UINT index);
#endif

private:
    BYTE    m_nQueryMode;
    BYTE    m_nSearchMode;
    BYTE    m_nDABLibBoxType;
    BYTE    m_nCurPTYType;
    UINT    m_idCurDeviceID;
    //DABServiceInfo m_DABServiceInfo;

    BOOL    m_bPresetListChanged;
private:

	UINT					m_idValidSrcList[MEDIA_TYPE_NUMBER];
	UINT					m_idCurMediaType;

	CM_String			m_strBrowseTitle[32];

	DWORD			    m_dwListCnt;
	BOOL				m_bPlayable;
	INT					m_iBrowserLevel;

	CLabelGL			m_iconPlaylist;

    CLabelGL            m_iconService;
    CLabelGL            m_iconServiceFocus;
    CLabelGL            m_iconEnsemble;
    CLabelGL            m_iconEnsembleFocus;
	CLabelGL            m_iconPTY;
	CLabelGL            m_iconPTYFocus;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CListBoxItemGL		*m_pServiceItem;
	CLabelGL			m_ServiceArrowOn;
	CLabelGL			m_ServiceArrowOff;
	CListBoxItemGL		*m_pEnsembleItem;
	CLabelGL			m_EnsembleArrowOn;
	CLabelGL			m_EnsembleArrowOff;
	CListBoxItemGL		*m_pPtyItem;
	CLabelGL			m_PtyArrowOn;
	CLabelGL			m_PtyArrowOff;
#endif


	CLabelGL			m_iconReturn;

	CLabelGL		    m_iconHomeUp;
	CLabelGL		    m_iconHomeDown;
	CButtonGL		    m_btnHome;

	CLabelGL		    m_iconNowPlayingUp;
	CLabelGL		    m_iconNowPlayingDown;
	CButtonGL		    m_btnNowPlaying;


	CButtonGL		m_btnReturn;
	CButtonGL		m_btnTitle;

	CLabelGL        m_iconChangeQueryMode;
	CButtonGL       m_btnChangeQueryMode;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

 	CListBoxGL		m_DABboxList;
 	CListBoxGL      m_boxEnsembleList;
	CListBoxGL		m_boxServiceList;
	CListBoxGL		m_boxPTYList;
	CListBoxGL		m_boxPTYServiceList;
	CListBoxGL		m_boxEnsembleServiceList;

	CListBoxGL		*m_pCurPlayingList;

	CM_String m_strCurrentService;

};

#endif
