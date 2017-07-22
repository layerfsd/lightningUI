#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
//#include "ImageCommon.h"


typedef struct _IMAGE_THUMB
{
	BOOL	bHasimageData;
	CImageGL image;
	CM_String strTitle;
	CM_String strFileName;
}IMAGE_THUMB,*LPIMAGE_THUMB;

class CPanelSettingBackground :
	public CPanelGL
{
public:
	CPanelSettingBackground(void);
	~CPanelSettingBackground(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void EnableRefresh();
	const wchar_t* GetIndexValue(INT index);
	const wchar_t* GetNextValue();
	const wchar_t* GetPreviousValue();

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);


private:
	BOOL IsImageFile(LPCTSTR szFileName);
	BOOL ExploreImageFiles(LPCTSTR lpszPath,BOOL bThumbed=FALSE);
	static DWORD ExploreThreadPro(LPVOID lpParam);
	void RefreshLibrary();
    void RefreshDefaultBKIndex();
	//DWORD WaitForRefreshDone(DWORD dwWait);
	void ClearThumbList();
	void UpdateUIList();

private:
	CLabelGL        m_tagBk;

	CM_List<IMAGE_THUMB>	m_listImageThums;

	BOOL            m_bNeedRefreshLib;
	BOOL		    m_bRefreshingLib;
	HANDLE			m_hExploreThread;
	HANDLE			m_hRefreshEvent;
	BOOL            m_bNeedUpdateUIList;

	//CLabelGL		m_iconHomeUp;
	//CLabelGL		m_iconHomeDown;
	CButtonGL       m_btnOK;
	CButtonGL       m_btnCancel;

	//CLabelGL		m_iconNowPlayingUp;
	//CLabelGL		m_iconNowPlayingDown;
	//CButtonGL		m_btnNowPlaying;

	CLabelGL	    m_iconLoading;

	//CLabelGL	    m_iconDevUSB;
	//CLabelGL	    m_iconDevSD;
	//CLabelGL	    m_iconDevUSBFocus;
	//CLabelGL	    m_iconDevSDFocus;

	CLabelGL	    m_iconImage;
   // CLabelGL        m_iconImageCurrent;

	CListBoxGL		m_boxDevice;
	CListBoxGL		m_boxLibrary;

};


