#include "PanelMediaDevice.h"
#include "../../resource.h"




//device list
CPanelMediaDevice::CPanelMediaDevice(void)
{
	m_DataCS = new CCriticalSection;

}
CPanelMediaDevice::~CPanelMediaDevice(void)
{
	if(m_DataCS)
	{
		delete m_DataCS;
	}
}

BOOL CPanelMediaDevice::IsReady()
{
	return CPanelGL::IsReady();


}
void CPanelMediaDevice::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}

void CPanelMediaDevice::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	if(idAni != ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}

}

void CPanelMediaDevice::Render()
{
// 	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_100):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,0.8};
// 	cr.a=m_pSkinManager->GetConfig()->GetCurParam()->fUITransparency;

	if(!BeginRender())return;

// 	DrawRect(pTexBack,0,0,794-32,356-32,16,&cr,TRUE);
//
// 	m_tagSeperator1.SetAlpha(m_fCurAlpha);
// 	m_tagSeperator1.Render();
//
// 	m_btnRefresh.SetAlpha(m_fCurAlpha);
// 	m_btnRefresh.Render();

// 	m_boxDevice.SetAlpha(m_fCurAlpha);
// 	m_boxDevice.Render();

	EndRender();
}
void CALLBACK CPanelMediaDevice::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: id:%d,msg:%d,para:%d\r\n"),TEXT(__FUNCTION__),idControl,wMsg,wPara));

	CPanelMediaDevice *panel=(CPanelMediaDevice*)lpUserPara;

	if(!panel)
	{
		return;
	}

	switch (idControl)
	{
	case IDC_MEDIA_BTN_REFRESH:
		break;
	}
}



BOOL CPanelMediaDevice::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);



	m_iconUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_210));
	m_iconSD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_212));
	m_iconBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_208));
	m_iconiPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_210));
	m_iconLineIn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_210));

	//device list:
	LISTBOXSTRUCTURE box;
	memset(&box,0,sizeof(box));
	box.siClient.cx=700;
	box.siClient.cy=340;
	box.siBorder.cx=700;
	box.siBorder.cy=340;
	box.iSpacing=85;
	box.idTexIndexBase=TEXID_360;
	box.idTexIndexFocus=TEXID_361;
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.pxSlider=50;

	m_boxDevice.Initialize(IDC_MEDIA_BOX_DEVICE,this,&box,pSkinManager);
	m_boxDevice.SetNotifyFunc(&CPanelMediaDevice::OnNotify,this);
	m_boxDevice.SetPos(-50,15);
	m_boxDevice.LinkTextGL(pTextGL);
	m_boxDevice.Show(TRUE,FALSE);
	m_boxDevice.SetAlpha(1.0f);

	//
	RefreshDeviceList();

	return TRUE;
}
BOOL CPanelMediaDevice::IsMediaDeviceAvailable(UINT id,UINT iRetryCnt)
{
	int index = m_boxDevice.SearchID(id);
	BOOL bRet=FALSE;

	if(index==-1)
	{
		//not exist, refresh
		//int iTryCount=0;
		//do
		//{
		//	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaDevice::IsMediaDeviceAvailable]: id:%d, refresh:%d\r\n"),id,iTryCount));

			//Sleep(1000);
			RefreshDeviceList();

			bRet = (-1!=m_boxDevice.SearchID(id));

			//iTryCount++;

		//} while (iTryCount<iRetryCnt && !bRet);

	}
	else
	{
		bRet = TRUE;
	}

	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaDevice::IsMediaDeviceAvailable]: id:%d, bRet:%d\r\n"),id,bRet));

	return bRet;
}
UINT CPanelMediaDevice::GetFirstAvailableDevice()
{
	if(m_boxDevice.GetCount()>0)
	{
		m_boxDevice.MoveToIndex(0);

		RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaDevice::GetFirstAvailableDevice]: %d\r\n"),m_boxDevice.GetCurFocusItemID()));

		return m_boxDevice.GetCurFocusItemID();
	}

	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaDevice::GetFirstAvailableDevice] no device \r\n")));

	return 0;
}
void CPanelMediaDevice::RefreshDeviceList(LPCTSTR lpszCurrentDevice)
{
	Lock();

	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaDevice::RefreshDeviceList]: current:%s\r\n"),lpszCurrentDevice));

	DWORD dwstart=GetTickCount();

	static LPCTSTR s_pszDevice[] =
	{
		//DEV_NAME_NAND,
		DEV_NAME_USB1,
		DEV_NAME_USB2,
		DEV_NAME_MEDIACARD,
#ifdef USE_GPS_CARD_MEDIA_PLAY
		DEV_NAME_GPSCARD,
#endif
	};

	TCHAR szFind[MAX_PATH];

	CListBoxItemGL* pItem;
	int index_cur=-1;

	//clear list
	m_boxDevice.ClearList();

	LPCTSTR lpszPath=_T("\\");
	wcscpy_s(szFind,MAX_PATH,lpszPath); //
	wcscat_s(szFind,MAX_PATH,_T("*.*")); //
	WIN32_FIND_DATA wfd;
	HANDLE hFind=FindFirstFile(szFind,& wfd);

	if(hFind==INVALID_HANDLE_VALUE) //
		return;
	do
	{
		if(wfd.cFileName[0]=='.')
			continue; //
		if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			!(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) &&
			!(wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			for (int i = 0; i < _countof(s_pszDevice); i++)
			{
				if (lstrcmpi(s_pszDevice[i], wfd.cFileName) == 0)
				{
// 					TCHAR szFile[MAX_PATH];
// 					swprintf_s(szFile,MAX_PATH,_T("%s%s\\"),lpszPath,wfd.cFileName);
					//添加播放设备列表
					RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: Add device:%s\r\n"),TEXT(__FUNCTION__),(wfd.cFileName)));

					if(lpszCurrentDevice)
					{
						if(lstrcmpi(lpszCurrentDevice, wfd.cFileName) == 0)
							index_cur = m_boxDevice.GetCount();
					}

					const wchar_t *pName=NULL;
					CLabelGL *pIcon=NULL;
					UINT id=-1;
					switch (i)
					{
// 					case 0:
// 						pIcon=&m_iconDevice;
// 						pName=GetResourceString(IDS_DEV_NAME_RESIDENTFLASH);
// 						break;
					case 0://usb1
						::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_USB1,1);
						pIcon=&m_iconUSB;
						pName=GetResourceString(IDS_DEVICE_USB);
						id=MEDIA_TYPE_USB1;
						break;
					case 1://usb2
						::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_USB2,1);
						pIcon=&m_iconUSB;
						pName=GetResourceString(IDS_DEVICE_USB);
						id=MEDIA_TYPE_USB2;
						break;
					case 2://sd1
						::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_SD1,1);
						pIcon=&m_iconSD;
						pName=GetResourceString(IDS_DEVICE_SD);
						id=MEDIA_TYPE_SD1;
						break;
#ifdef USE_GPS_CARD_MEDIA_PLAY
					case 3://sd2
						::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_SD2,1);
						pIcon=&m_iconSD;
						pName=GetResourceString(IDS_DEVICE_SD);
						id=MEDIA_TYPE_SD2;
						break;
#endif
#ifdef USE_RESIDENTFLASH_PLAY	
					case 4://nand flash
						::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_RESIDENTFLASH,1);
						pIcon=&m_iconSD;
						pName=GetResourceString(IDS_DEVICE_SD);
						id=MEDIA_TYPE_RESIDENTFLASH;
						break;
#endif						
					}

					if(pIcon && pName)
					{
						pItem=new CListBoxItemGL(pName,wfd.cFileName);
						pItem->SetIcon(pIcon);
	 					pItem->ShowValue(FALSE);
// 						m_boxDevice.AddItemByOrder(pItem);
						m_boxDevice.AddItem(pItem,id);
					}
				}
			}
		}
	}
	while(FindNextFile(hFind,&wfd));
	FindClose(hFind);

	/*
	//test: Add ipod and bluetooth device:
	pItem=new CListBoxItemGL(DEV_NAME_IPOD,DEV_NAME_IPOD);
	pItem->ShowValue(FALSE);
	pItem->SetIcon(&m_iconiPod);
	m_boxDevice.AddItem(pItem);

	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_BT),DEV_NAME_BT);
	pItem->ShowValue(FALSE);
	pItem->SetIcon(&m_iconBT);
	m_boxDevice.AddItem(pItem);

	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_LINEIN_FRONT),DEV_NAME_LINEIN_FRONT);
	pItem->ShowValue(FALSE);
	pItem->SetIcon(&m_iconLineIn);
	m_boxDevice.AddItem(pItem);

	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_LINEIN_REAR),DEV_NAME_LINEIN_REAR);
	pItem->ShowValue(FALSE);
	pItem->SetIcon(&m_iconLineIn);
	m_boxDevice.AddItem(pItem);

	//move to current index:
	if(index_cur != -1)
	{
		m_boxDevice.MoveToIndex(index_cur,0,FALSE);
	}
*/

	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaDevice::RefreshDeviceList]: take:%d ms\r\n"),GetTickCount()-dwstart));

	UnLock();
}
void CPanelMediaDevice::Lock()
{
	if(m_DataCS)
	{
		m_DataCS->Enter();
	}
}
void CPanelMediaDevice::UnLock()
{
	if(m_DataCS)
	{
		m_DataCS->Leave();
	}
}
