#include "PanelImageWrapper.h"
#include "../../resource.h"

#if CVTE_EN_ACCON_WARNING
#include "../Warning/WarningProxy.h"
#endif

CPanelImageWrapper::CPanelImageWrapper(void)
{
	m_ppLibrary = NULL;
	m_ppMain = NULL;
	m_idCurDeviceID = MEDIA_TYPE_INVALID;
}
CPanelImageWrapper::~CPanelImageWrapper(void)
{

}



void CPanelImageWrapper::Render()
{
	if(!BeginRender())return;


	EndRender();
}

BOOL CPanelImageWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);


	return TRUE;
}
// void CPanelImageWrapper::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
// {
// 	if(m_ppLibrary)
// 	{
// 		m_ppLibrary->EnableRefresh();
// 	}
// }
// void CPanelImageWrapper::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
// {
// 	if(m_ppLibrary)
// 	{
// 		m_ppLibrary->EnableRefresh();
// 	}
// }
void CPanelImageWrapper::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
// 	RETAILMSG(DEBUG_IMAGE,(_T("APP:MSG:GUI:Image: [CPanelImageWrapper::OnAvailableSourceChanged]: %d,%d\r\n"),wParam,lParam));

	if(m_ppLibrary)
	{
		m_ppLibrary->OnAvailableSourceChanged(wParam,lParam);
	}
}
BOOL CPanelImageWrapper::OnImagePlayerCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;
	RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageWrapper::OnImagePlayerCommand]: ++ %d,%d\r\n"),wParam,lParam));

	switch (wParam)
	{
	case IMAGE_CMD_PLAY_INDEX:
		{
			bRet = PlayIndex(lParam);
		}
		break;
	case IMAGE_CMD_LOAD_FILE:
		{
			bRet = PlayFile((const wchar_t*)lParam);
		}
		break;
	case IMAGE_CMD_PLAY_CONTROL:
		{
			switch (lParam)
			{
			case IMAGE_CONTROL_STOP:
				//Stop();
				break;
			case IMAGE_CONTROL_PAUSE:
				//Pause();
				break;
			case IMAGE_CONTROL_PLAY:
				//Play();
				break;
			case IMAGE_CONTROL_NEXT:
				bRet = PlayNext();
				break;
			case IMAGE_CONTROL_PREVIOUS:
				bRet = PlayPrevious();
				break;
			case IMAGE_CONTROL_PLAYMODE_TOGGLE:
				break;
			}
		}
		break;
	case IMAGE_CMD_SELECT_DEVICE:
		{
			m_idCurDeviceID = (UINT)lParam;
			RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageWrapper::OnImagePlayerCommand]: m_idCurDeviceID:%d \r\n"),m_idCurDeviceID));
			bRet = TRUE;
		}
		break;

	}
	RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageWrapper::OnImagePlayerCommand]: -- bRet:%d\r\n"),bRet));

	return bRet;
}

void CPanelImageWrapper:: LinkPages(CPanelImageMain *pMain,CPanelImageLibrary *pLibrary)
{
	m_ppMain = pMain;
	m_ppLibrary = pLibrary;
}

void CALLBACK CPanelImageWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelImageWrapper *panel=(CPanelImageWrapper*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelImageWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BTN_HOME:
		break;
	}
}

BOOL CPanelImageWrapper::PlayIndex(INT index)
{
	if(m_ppLibrary && m_ppMain)
	{
		return m_ppMain->LoadImageFile(m_ppLibrary->GetIndexValue(index));
	}
	return FALSE;
}
BOOL CPanelImageWrapper::PlayFile(const wchar_t* pFileName)
{
	if(m_ppMain && pFileName)
	{
		return m_ppMain->LoadImageFile(pFileName);
	}
	return FALSE;
}
BOOL CPanelImageWrapper::PlayNext()
{
	if(m_ppLibrary && m_ppMain)
	{
		return m_ppMain->LoadImageFile(m_ppLibrary->GetNextValue());
	}
	return FALSE;
}
BOOL CPanelImageWrapper::PlayPrevious()
{
	if(m_ppLibrary && m_ppMain)
	{
		return m_ppMain->LoadImageFile(m_ppLibrary->GetPreviousValue());
	}
	return FALSE;
}

void CPanelImageWrapper::OnDeviceArrivedReady(LPCTSTR lpDeviceName)
{
	if (!lpDeviceName)
		return;

#if CVTE_EN_ACCON_WARNING
	if (CWarningProxy::GetInstance()->IsShowWarningSmartBar() || CWarningProxy::GetInstance()->IsShowWarningPage())
	{
		return;
	}
#endif

	RETAILMSG(DEBUG_IMAGE,(_T("MSG: [CPanelImageWrapper::OnDeviceArrivedReady]: %s\r\n"),lpDeviceName));

}

void CPanelImageWrapper::OnDeviceRemovedReady(LPCTSTR lpDeviceName)
{
	RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageWrapper::OnDeviceRemovedReady]: %s\r\n"),lpDeviceName));

	if (!lpDeviceName)
		return;

	//notify:
	if (lstrcmpi(_T("DSK2:"), lpDeviceName) == 0)
	{
		//m_bUsb1Inserted = FALSE;
		::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_USB1,0);
	}
	else if (lstrcmpi(_T("DSK3:"), lpDeviceName) == 0)
	{
		::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_USB2,0);
	}
	else if (lstrcmpi(_T("DSK9:"), lpDeviceName) == 0)
	{
		::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_SD1,0);
	}
#ifdef USE_GPS_CARD_MEDIA_PLAY
	else if (lstrcmpi(_T("DSK8:"), lpDeviceName) == 0)
	{
		::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_SD2,0);
	}
#endif

	RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageWrapper::OnDeviceRemovedReady]: m_idCurDeviceID:%d lpDeviceName:%s\r\n"),m_idCurDeviceID,lpDeviceName));
	if (( m_idCurDeviceID == MEDIA_TYPE_USB1 && lstrcmpi(_T("DSK2:"), lpDeviceName) == 0 ) ||
		( m_idCurDeviceID == MEDIA_TYPE_USB2 && lstrcmpi(_T("DSK3:"), lpDeviceName) == 0 ) ||
#ifdef USE_GPS_CARD_MEDIA_PLAY
		( m_idCurDeviceID == MEDIA_TYPE_SD2 && lstrcmpi(_T("DSK8:"), lpDeviceName) == 0 ) ||
#endif
		( m_idCurDeviceID == MEDIA_TYPE_SD1 && lstrcmpi(_T("DSK9:"), lpDeviceName) == 0 ))
	{
		RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageWrapper::OnDeviceRemovedReady]: reset current device\r\n")));
		if (m_ppMain)
		{
			m_ppMain->ImagePlayAbort();
		}

		if (m_ppLibrary)
		{
			m_ppLibrary->ClearList();
		}
	}
}
