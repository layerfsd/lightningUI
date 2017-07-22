#pragma once
#include "MediaCommon.h"


class CPanelMediaDevice :
	public CPanelGL
{
public:
	CPanelMediaDevice(void);
	~CPanelMediaDevice(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void RefreshDeviceList(LPCTSTR lpszCurrentDevice=NULL);
	const wchar_t* GetCurValue(){return m_boxDevice.GetCurFocusItemValue();}
	const wchar_t* GetIndexValue(INT index){m_boxDevice.MoveToIndex(index,0,FALSE);return m_boxDevice.GetCurFocusItemValue();}
	INT GetIndexID(INT index){m_boxDevice.MoveToIndex(index,0,FALSE);return m_boxDevice.GetCurFocusItemID();}
	UINT GetFirstAvailableDevice();

// 	INT SearchByValue(LPCTSTR lpValue){return m_boxDevice.SearchValue(lpValue);}
// 	INT SearchByID(UINT id){return m_boxDevice.SearchID(id);}

	BOOL IsMediaDeviceAvailable(UINT id,UINT iRetryCnt=3);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

	void MoveToNext(){m_boxDevice.MoveToNext(TRUE,FALSE);}
	void MoveToPrevious(){m_boxDevice.MoveToLast(TRUE,FALSE);}

protected:
	void Lock();
	void UnLock();

private:
	CListBoxGL		m_boxDevice;
	CLabelGL			m_iconUSB;
	CLabelGL			m_iconSD;
	CLabelGL			m_iconBT;
	CLabelGL			m_iconiPod;
	CLabelGL			m_iconLineIn;

	CCriticalSection* m_DataCS;

};
