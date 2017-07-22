#include "PanelSettingBackground.h"
#include "../../resource.h"

extern const wchar_t* g_strInstallPath;
extern const wchar_t* g_strDefaultBackground;

CPanelSettingBackground::CPanelSettingBackground(void)
{
	m_hExploreThread = INVALID_HANDLE_VALUE;
	m_hRefreshEvent = INVALID_HANDLE_VALUE;
	m_bRefreshingLib = FALSE;
	m_bNeedRefreshLib = TRUE;
	m_bNeedUpdateUIList = FALSE;
}
CPanelSettingBackground::~CPanelSettingBackground(void)
{

}

BOOL CPanelSettingBackground::IsReady()
{
	return CPanelGL::IsReady();
}

void CPanelSettingBackground::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

	/*
	int allindex = m_boxLibrary.GetCount();
	for(int index=0;index<allindex;index++)
	{
		m_boxLibrary.SetIndexIcon(index, &m_iconImage);
	}
	int index =	m_boxLibrary.SearchValue(m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName.String());

	if(index != -1)
	{
		m_boxLibrary.MoveToIndex(index);
		m_boxLibrary.SetIndexIcon(m_boxLibrary.GetCurFocusItemID(), &m_iconImageCurrent);
	}
	*/

// 	MakeDefaultAni(ANIM_ROTATE_TOP_CLOCK_IN);

// 	m_bNeedRefreshLib = TRUE;
}

void CPanelSettingBackground::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	if(m_boxLibrary.IsOnQuickSearchMode())
	{
		m_boxLibrary.ShowQuickSearchMode(FALSE);
	}
}

void CPanelSettingBackground::Render()
{
	if(!BeginRender())
		return;

	//m_tagBk.SetAlpha(m_fCurAlpha);
	//m_tagBk.Render();

	//m_boxDevice.Enable(!m_bRefreshingLib && !m_boxLibrary.IsOnQuickSearchMode());

	if(IsAniReady() && m_bNeedRefreshLib)
	{
		RefreshLibrary();
	}

	//update list:
	if(m_bNeedUpdateUIList)
	{
		m_bNeedUpdateUIList = FALSE;
		UpdateUIList();
	}

	SetFrameTime(m_bRefreshingLib?30:500);

	m_boxLibrary.Enable(!m_bRefreshingLib);

	//m_boxDevice.SetAlpha(m_fCurAlpha);
	//m_boxDevice.Render();

	if(!m_bRefreshingLib)
	{
		m_boxLibrary.SetAlpha(m_fCurAlpha);
		m_boxLibrary.Render();
	}




	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	m_btnCancel.SetAlpha(m_fCurAlpha);
	m_btnCancel.Render();

	SetFrameTime(m_bRefreshingLib?30:500);

	if(m_bRefreshingLib)
	{
		VIEW_STATE vs=m_iconLoading.GetCurViewState();
		vs.fRotateZ-=5;
		if(vs.fRotateZ<-360)
			vs.fRotateZ=0;
		m_iconLoading.SetCurViewState(&vs);
		m_iconLoading.SetAlpha(m_fCurAlpha);
		m_iconLoading.Render();
	}


	EndRender();
}

BOOL CPanelSettingBackground::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x = 12;
	int margin_y = 12;
	int caption_h = 24;
	int caption2_h = 24;
	int caption3_h = 20;
	int gridHeight = 80;
	int cx = g_iClientWidth - LIST_BOX_ITEM_WIDTH;//g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x*2;
	int cy = LIST_BOX_ITEM_HEIGHT;

	m_iconImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_631));
	m_iconLoading.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713),NULL);
	m_iconLoading.SetShowScale(1.44f);
	m_iconLoading.SetPos(0,cy/2);

	GLfloat py=g_iClientHeight/2-40;

	LISTBOXSTRUCTURE box;
	memset(&box,0,sizeof(box));

	box.iSpacing = (g_iClientHeight - cy)/5;
	box.siClient.cx = g_iClientWidth-cx;
	box.siClient.cy = g_iClientHeight - cy;	
	box.siBorder.cx = g_iClientWidth-cx;	
	box.siBorder.cy = g_iClientHeight - cy;
	box.idTexIndexBase=TEXID_066;
	box.idTexIndexFocus=TEXID_242;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_065;
	box.idTexBtnUp=TEXID_066;
	box.idTexBtnDown=TEXID_241;
	box.idTexSliBase=TEXID_060;
	box.idTexSliChannel=TEXID_060;
	box.idTexSliThumb=TEXID_276;
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;	
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=TRUE;
	box.pxSlider=-8;
	box.fIndexBaseOffset = 0;

	m_boxLibrary.Initialize(IDC_SETTING_BOX_BACKGROUND,this,&box,pSkinManager);
	m_boxLibrary.SetNotifyFunc(&CPanelSettingBackground::OnNotify,this);
	m_boxLibrary.SetPos(0, cy/2);
	m_boxLibrary.LinkTextGL(pTextGL);
	m_boxLibrary.Show(TRUE,FALSE);
	m_boxLibrary.SetAlpha(0.8f);
	m_boxLibrary.SetCaptionColor(&cr_gray,&cr_w);
	m_boxLibrary.SetCaptioniHeight(18);

	int gap = 5;
	//Set
	m_btnOK.Initialize(IDC_SETTING_BTN_SET_BACKGROUND,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnOK.SetSize((cx-margin_x + gap)/2, gridHeight);
	m_btnOK.SetPos(-(cx-margin_x)/4 - gap,-g_iClientHeight/2+cy/2);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_w);
	m_btnOK.SetCaptionHeight(24);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingBackground::OnNotify,this);

	//Cancel
	m_btnCancel.Initialize(IDC_SETTING_BTN_CANCEL_BACKGROUND,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnCancel.SetSize((cx-margin_x + gap)/2, gridHeight);
	m_btnCancel.SetPos((cx-margin_x)/4 + gap,-g_iClientHeight/2+cy/2);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_w);
	m_btnCancel.SetCaptionHeight(24);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingBackground::OnNotify,this);

#else

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	SIZE si={64,64};


	int cx=LIST_BOX_ITEM_WIDTH;
	int cy=LIST_BOX_ITEM_HEIGHT;
	int margin_x = 12;
	int margin_y = 12;

	m_iconImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_631));
	m_iconLoading.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713),NULL);
	m_iconLoading.SetShowScale(1.44f);
	m_iconLoading.SetPos(0,cy/2);

	LISTBOXSTRUCTURE box;


	memset(&box,0,sizeof(box));

	box.iSpacing= 78; //(g_iClientHeight)/count;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=box.iSpacing*5; //(box.iSpacing * count)-100;
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=390;  //g_iClientHeight-100;	//(box.iSpacing * count);

	box.idTexIndexBase=TEXID_606;
	box.idTexIndexFocus=TEXID_607;			//TEXID_361;
	box.idTexBase=TEXID_066;
	box.idTexSliThumb=TEXID_661;
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;//FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=FALSE;	//TRUE;
	box.pxSlider=-15;	//-30;
	box.fIndexBaseOffset=8; //74;

	m_boxLibrary.Initialize(IDC_SETTING_BOX_BACKGROUND,this,&box,pSkinManager);
	m_boxLibrary.SetNotifyFunc(&CPanelSettingBackground::OnNotify,this);
	m_boxLibrary.SetMarginHor(18);  //38
	m_boxLibrary.SetPos(0,40);
	m_boxLibrary.LinkTextGL(pTextGL);
	m_boxLibrary.Show(TRUE,FALSE);
	m_boxLibrary.SetAlpha(0.8f);
	m_boxLibrary.SetCaptionColor(&cr_gray,&cr_w);
	m_boxLibrary.SetCaptioniHeight(18);

	//Set
	m_btnOK.Initialize(IDC_SETTING_BTN_SET_BACKGROUND,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnOK.SetSize((g_iClientWidth-cx-margin_x)/2,78);
	m_btnOK.SetPos(-(g_iClientWidth-cx-margin_x)/4,-g_iClientHeight/2+40);
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_gray);
	m_btnOK.SetCaptionHeight(24);
	m_btnOK.SetCaption(GetResourceString(IDS_OK));
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(CPanelSettingBackground::OnNotify,this);

	//Cancel
	m_btnCancel.Initialize(IDC_SETTING_BTN_CANCEL_BACKGROUND,this,pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancel.SetSize((g_iClientWidth-cx-margin_x)/2,78);
	m_btnCancel.SetPos((g_iClientWidth-cx-margin_x)/4,-g_iClientHeight/2+40);
	m_btnCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancel.SetCaptionColor(&cr_gray);
	m_btnCancel.SetCaptionHeight(24);
	m_btnCancel.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancel.LinkTextGL(pTextGL);
	m_btnCancel.SetNotifyFunc(CPanelSettingBackground::OnNotify,this);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET


	return TRUE;
}


void CALLBACK CPanelSettingBackground::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingBackground *panel=(CPanelSettingBackground*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingBackground::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_BTN_SET_BACKGROUND:
		::SendMessage(m_hMainWnd,WM_BACKGROUND_CHANGE,wPara, (LPARAM)m_boxLibrary.GetIndexValue(m_boxLibrary.GetCurIndex()));
		//SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BTN_CANCEL_BACKGROUND:
		SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BOX_BACKGROUND:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
				{
					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_DOUBLE_CLICK:
				{
					//begin to play
					//SendImageCommand(IMAGE_CMD_PLAY_INDEX,index);
					//SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN);
					::SendMessage(m_hMainWnd,WM_BACKGROUND_CHANGE,wPara, (LPARAM)m_boxLibrary.GetIndexValue(index));
					//SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_GENERAL);
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_DEFAULT);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	}
}

BOOL CPanelSettingBackground::IsImageFile(LPCTSTR szFileName)
{
	WCHAR strExt[16]={0};
	INT iLen=wcslen(szFileName);

	if(!GetFileExtensionFromPath(szFileName,strExt,16))
		return FALSE;

	if(wcslen(strExt)<1)
		return FALSE;

	static LPCTSTR s_pszExts[] =
	{
		_T("jpg")
	};
	// 	_T("bmp"),
	// 		_T("tif")

	for (int i = 0; i < _countof(s_pszExts); i++)
	{
		if (lstrcmpi(s_pszExts[i], strExt) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CPanelSettingBackground::ExploreImageFiles(LPCTSTR lpszPath,BOOL bThumbed)
{

	TCHAR szFind[MAX_PATH];
	wcscpy_s(szFind,MAX_PATH,lpszPath); //
	wcscat_s(szFind,MAX_PATH,_T("*.*")); //
	WIN32_FIND_DATA wfd;
	HANDLE hFind=FindFirstFile(szFind,& wfd);

	TCHAR szFile[1024];

	//CListBoxItemGL* pItem;

	if(hFind==INVALID_HANDLE_VALUE) //
		return FALSE;
	do
	{
		if(wfd.cFileName[0]=='.')
			continue; //


		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			TCHAR szFile[MAX_PATH];

			swprintf_s(szFile,MAX_PATH,_T("%s%s\\"),lpszPath,wfd.cFileName);
			ExploreImageFiles(szFile,bThumbed); //
		}
		else if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			//cut max path:
			int len=swprintf_s(szFile,1024,_T("%s%s"),lpszPath,wfd.cFileName);
			if(len>MAX_PATH)
			{
				continue;
			}


			if(IsImageFile(wfd.cFileName))
			{
				/*
				pItem=new CListBoxItemGL(wfd.cFileName,szFile);
				pItem->ShowValue(FALSE);
				pItem->SetIcon(&m_iconImage);
				//pItem->SetIconFromFile(szFile,128,64);
				m_boxLibrary.AddItemByOrder(pItem,m_boxLibrary.GetCount(),TRUE);
				//m_boxLibrary.AddItem(pItem);

				//int num=m_boxLibrary.GetCount();
				//m_iTotalPage=num/IMAGE_THUMB_COUNT_PERPAGE+((num%IMAGE_THUMB_COUNT_PERPAGE)?1:0);
				*/
				IMAGE_THUMB *img = new IMAGE_THUMB;
				if(img)
				{
					const int max_x=1920;
					const int max_y=1080;

					//get max 200 thumbs:
					if(bThumbed && m_listImageThums.GetCount()<200)
					{
						img->bHasimageData=TRUE;
						if(img->image.LoadFromFileScaled(szFile,128,64,max_x,max_y))
						{
							img->strFileName=szFile;
							img->strTitle=wfd.cFileName;
							m_listImageThums.Add(img);
						}
						else
						{
							delete img;
						}
					}
					else
					{
						img->bHasimageData=FALSE;

						img->strFileName=szFile;
						img->strTitle=wfd.cFileName;
						m_listImageThums.Add(img);
					}
				}
			}
		}
		Sleep(0);
	}
	while(FindNextFile(hFind,&wfd));
	FindClose(hFind);

	return TRUE;
}

DWORD CPanelSettingBackground::ExploreThreadPro(LPVOID lpParam)
{
	CPanelSettingBackground *panel=(CPanelSettingBackground*)lpParam;

	DWORD dwTick=GetTickCount();

	// 	//storage only

	// 	panel->ExploreImageFiles(_T("\\ResidentFlash\\"));
	// 	panel->ExploreImageFiles(_T("\\GPS CARD\\"));
	//panel->ExploreImageFiles(_T("\\Windows\\"));

	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("background\\images\\"));
	panel->ExploreImageFiles(strPath.String(),TRUE);
	panel->ExploreImageFiles(_T("\\MEDIA CARD\\"));
	panel->ExploreImageFiles(_T("\\USB Disk\\"));
	panel->RefreshDefaultBKIndex();

	//Sleep(500);

	panel->m_bRefreshingLib=FALSE;

	SetEvent(panel->m_hRefreshEvent);

	panel->m_bNeedUpdateUIList = TRUE;

	return 0;
}

void CPanelSettingBackground::RefreshDefaultBKIndex()
{
	/*
	int index =	m_boxLibrary.SearchValue(m_pSkinManager->GetConfig()->GetCurParam()->strTexBackgroundName.String());
	if(index != -1)
	{
		m_boxLibrary.MoveToIndex(index);
		m_boxLibrary.SetIndexIcon(m_boxLibrary.GetCurFocusItemID(), &m_iconImageCurrent);
	}
	*/
}
void CPanelSettingBackground::UpdateUIList()
{
	CListBoxItemGL *pItem;
	if(m_listImageThums.GetCount()>0)
	{
		DWORD tick=GetTickCount();

		m_listImageThums.Head();
		while (!m_listImageThums.End())
		{
			pItem=new CListBoxItemGL(m_listImageThums.Get()->strTitle.String(),m_listImageThums.Get()->strFileName.String());
			pItem->ShowValue(FALSE);
			if(m_listImageThums.Get()->bHasimageData)
			{
				pItem->SetIconFromImage(&m_listImageThums.Get()->image);
                pItem->SetIconScale(0.8f, 1.0f);
			}
			else
			{
				pItem->SetIcon(&m_iconImage);
                pItem->SetIconScale(0.8f, 1.0f);
			}
// 			m_boxLibrary.AddItemByOrder(pItem,m_boxLibrary.GetCount(),TRUE);
			m_boxLibrary.AddItem(pItem,m_boxLibrary.GetCount());
			m_listImageThums.Next();
		}

		//
		ClearThumbList();
	}

}
void CPanelSettingBackground::ClearThumbList()
{
	if(m_listImageThums.GetCount()>0)
	{

		m_listImageThums.Head();
		while (!m_listImageThums.End())
		{
			delete m_listImageThums.Get();
			m_listImageThums.Next();
		}
		m_listImageThums.Clear();
	}
}
void CPanelSettingBackground::RefreshLibrary()
{
	CListBoxItemGL* pItem;

	if(m_bRefreshingLib)
	{

		return;
	}

	m_bRefreshingLib = TRUE;

	ResetEvent(m_hRefreshEvent);

	//clear thumb list:
	ClearThumbList();
	//clear list:
	m_boxLibrary.ClearList();
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	#if (CVTE_MODEL_TYPE == CVTE_MODEL_TYPE_U702P_ASP)
	pItem=new CListBoxItemGL(_T("BOSS AUDIO.jpg"),g_strDefaultBackground);
	#else
	pItem=new CListBoxItemGL(_T("PLANET AUDIO.jpg"),g_strDefaultBackground);
	#endif	
#else
	pItem=new CListBoxItemGL(_T("LIGHTNING.jpg"),g_strDefaultBackground);
#endif
	pItem->ShowValue(FALSE);
// 	pItem->SetIcon(&m_iconImage);
	if(!pItem->SetIconFromFile(g_strDefaultBackground,128,64))
	{
		pItem->SetIcon(&m_iconImage);
	}
    pItem->SetIconScale(0.8f, 1.0f);
// 	m_boxLibrary.AddItemByOrder(pItem,m_boxLibrary.GetCount());
	m_boxLibrary.AddItem(pItem,m_boxLibrary.GetCount());


	if(m_hExploreThread!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hExploreThread);

	//begin thread to explore for video files:
	m_hExploreThread=CreateThread(NULL,NULL,ExploreThreadPro,this,NULL,NULL);

	m_bNeedRefreshLib = FALSE;
}

/*
DWORD CPanelSettingBackground::WaitForRefreshDone(DWORD dwWait)
{
	return WaitForSingleObject(m_hRefreshEvent,dwWait);
}
*/
const wchar_t* CPanelSettingBackground::GetIndexValue(INT index)
{
	if(m_boxLibrary.MoveToIndex(index,0,FALSE))
		return m_boxLibrary.GetCurFocusItemValue();
	else
		return NULL;
}
const wchar_t* CPanelSettingBackground::GetNextValue()
{
	m_boxLibrary.MoveToNext(TRUE,FALSE);
	return m_boxLibrary.GetCurFocusItemValue();
}
const wchar_t* CPanelSettingBackground::GetPreviousValue()
{
	m_boxLibrary.MoveToLast(TRUE,FALSE);
	return m_boxLibrary.GetCurFocusItemValue();
}
void CPanelSettingBackground::EnableRefresh()
{
	m_bNeedRefreshLib = TRUE;
}
