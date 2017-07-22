#include "PanelBluetoothMain.h"
#include "../Media/MediaCommon.h"
#include "../../resource.h"

CPanelBluetoothMain::CPanelBluetoothMain(void)
{
	m_dwShowContactDetailTick = 0;

	m_pCurSellectedDevice = NULL;
	m_pCurConnectedDevice = NULL;

	m_type = BT_HOMEPAGE_DEVICE;
	m_statusContact = CONTACT_NULL;

	m_dwContactDownloadedCount = 0;
	m_bDeviceSearching = FALSE;
	m_bShowSearchedDevice = FALSE;
	m_bNeedRefreshPhonebook = FALSE;
	m_pContactList = NULL;
	m_dwContactDownloadTick=0;
	m_bManualConnecting=FALSE;
	m_hfStatus = SNM_CONNECTION_STATE_IDLE;
	m_idPickDeviceFor = 0;
	m_bAutoConnecting = FALSE;
	m_bManualDisconnecting = FALSE;
}
CPanelBluetoothMain::~CPanelBluetoothMain(void)
{

}

BOOL CPanelBluetoothMain::IsReady()
{
	if(m_type == BT_HOMEPAGE_DEVICE)
		return CPanelGL::IsReady();// && (m_bShowSearchedDevice?!m_bDeviceSearching:TRUE);
	else
		return CPanelGL::IsReady();

}

void CPanelBluetoothMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

	//
	if(iParam!=0)
	{
		BT_HOMEPAGE_TYPE page=BT_HOMEPAGE_TYPE(LOWORD(iParam));
		m_idPickDeviceFor=HIWORD(iParam);

		SetShowType(page);

		if(page==BT_HOMEPAGE_DEVICE && m_boxDeviceList.GetCount()==0)
		{
			SearchDevice();
			m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));
		}
	}
	else
	{
		m_idPickDeviceFor=0;

		if(m_hfStatus == SNM_CONNECTION_STATE_CONNECTED)
		{
			SetShowType(BT_HOMEPAGE_CONTACT);
		}
		else if(m_boxDeviceList.GetCount()==0)
		{
			SearchDevice();
			m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));
		}
	}
}

void CPanelBluetoothMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	//
	if(m_type == BT_HOMEPAGE_CONTACT)
	{
		ShowContactDetail(FALSE);
		if(m_boxContact.IsOnQuickSearchMode())
		{
			m_boxContact.ShowQuickSearchMode(FALSE);
		}
		ClearInput(-1);//clear all
	}	
	
	//default
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelBluetoothMain::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
void CPanelBluetoothMain::Render()
{
	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_604):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};
	COLORGL cr_gray={0.1,0.1,0.1,m_fCurAlpha};

	if(!BeginRender())return;

	DrawRect(pTexBack,0,0,g_iClientWidth-4,g_iClientHeight-4,2,&cr,TRUE);

	//
	if(m_type == BT_HOMEPAGE_CONTACT)
	{
		m_boxLists.Enable(!m_boxContact.IsOnQuickSearchMode());
		m_btnHome.Enable(!m_boxContact.IsOnQuickSearchMode());
	}

	//box:
	m_boxLists.SetAlpha(m_fCurAlpha);
	m_boxLists.Render();


	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	if(m_type == BT_HOMEPAGE_CONTACT)
	{
		m_boxContact.Show(m_statusContact==CONTACT_DOWNLOADED);

		m_btnDownloadPhonebook.Show(m_statusContact==CONTACT_NULL || m_statusContact==CONTACT_DOWNLOADFAILED || m_statusContact==CONTACT_DOWNLOADCANCLED);
		m_btnCancelDownloadPhonebook.Show(m_statusContact==CONTACT_DOWNLOADING && GetTickCount()-m_dwContactDownloadTick>1000);

		m_btnSearchContact.Show(m_statusContact==CONTACT_DOWNLOADED && GetTickCount()-m_dwContactDownloadTick>1000);

		if(m_boxContactDetail.IsVisible() && (GetTickCount()-m_dwShowContactDetailTick>10000 || m_boxContact.IsOnQuickSearchMode()))
			m_boxContactDetail.Show(FALSE);

		BOOL bEnableKey=(!m_boxContactDetail.IsVisible() && !m_boxContact.IsOnQuickSearchMode());

		//keyboard:
		m_tagKeyboardBk.SetAlpha(m_fCurAlpha);
		m_tagKeyboardBk.Render();

		m_btnNum1.Enable(bEnableKey);
		m_btnNum2.Enable(bEnableKey);
		m_btnNum3.Enable(bEnableKey);
		m_btnNum4.Enable(bEnableKey);
		m_btnNum5.Enable(bEnableKey);
		m_btnNum6.Enable(bEnableKey);
		m_btnNum7.Enable(bEnableKey);
		m_btnNum8.Enable(bEnableKey);
		m_btnNum9.Enable(bEnableKey);
		m_btnNum0.Enable(bEnableKey);
		m_btnStar.Enable(bEnableKey);
		m_btnWell.Enable(bEnableKey);
		m_btnDial.Enable(bEnableKey);
		m_btnDelete.Enable(bEnableKey);

		m_btnNum1.SetAlpha(m_fCurAlpha);
		m_btnNum1.Render();
		m_btnNum2.SetAlpha(m_fCurAlpha);
		m_btnNum2.Render();
		m_btnNum3.SetAlpha(m_fCurAlpha);
		m_btnNum3.Render();

		m_btnNum4.SetAlpha(m_fCurAlpha);
		m_btnNum4.Render();
		m_btnNum5.SetAlpha(m_fCurAlpha);
		m_btnNum5.Render();
		m_btnNum6.SetAlpha(m_fCurAlpha);
		m_btnNum6.Render();

		m_btnNum7.SetAlpha(m_fCurAlpha);
		m_btnNum7.Render();
		m_btnNum8.SetAlpha(m_fCurAlpha);
		m_btnNum8.Render();
		m_btnNum9.SetAlpha(m_fCurAlpha);
		m_btnNum9.Render();

		m_btnStar.SetAlpha(m_fCurAlpha);
		m_btnStar.Render();
		m_btnNum0.SetAlpha(m_fCurAlpha);
		m_btnNum0.Render();
		m_btnWell.SetAlpha(m_fCurAlpha);
		m_btnWell.Render();

		m_btnDial.SetAlpha(m_fCurAlpha);
		m_btnDial.Render();
		m_btnDelete.SetAlpha(m_fCurAlpha);
		m_btnDelete.Render();

		m_btnSearchContact.SetAlpha(m_fCurAlpha);
		m_btnSearchContact.Render();

		m_tagDialString.SetAlpha(m_fCurAlpha);
		m_tagDialString.Render();

		m_boxContact.SetAlpha(m_fCurAlpha);
		m_boxContact.Render();

// 		if(m_boxContactDetail.IsVisible() && (GetTickCount()-m_dwShowContactDetailTick>10000 || m_boxContact.IsOnQuickSearchMode()))
// 			m_boxContactDetail.Show(FALSE);
		m_boxContactDetail.SetAlpha(m_fCurAlpha);
		m_boxContactDetail.Render();

		m_btnDownloadPhonebook.SetAlpha(m_fCurAlpha);
		m_btnDownloadPhonebook.Render();

		m_btnCancelDownloadPhonebook.SetAlpha(m_fCurAlpha);
		m_btnCancelDownloadPhonebook.Render();

		//
		if(m_statusContact == CONTACT_DOWNLOADING)
		{
			//count:
			SIZE si;
			wchar_t str[16];
			//COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

			swprintf_s(str,16,_T("%d"),m_dwContactDownloadedCount);

			m_pTextGL->SetAlpha(m_fCurAlpha);
			m_pTextGL->SetLuminance(m_fCurLuminance);

			m_pTextGL->SetColor(&cr_gray,FALSE);
			m_pTextGL->SetString(str);
			m_pTextGL->SetCharScale(1.4f,1.4f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(-GLfloat(si.cx/2)+m_btnDownloadPhonebook.GetPosX(),60);
			m_pTextGL->Render();

			const wchar_t *pStrDownloading=GetResourceString(IDS_DOWNLOADING);
			if(pStrDownloading)
			{
				m_pTextGL->SetColor(&cr_gray,FALSE);
				m_pTextGL->SetString(pStrDownloading);
				m_pTextGL->SetCharScale(0.8f,0.8f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx/2)+m_btnDownloadPhonebook.GetPosX(),-20);
				m_pTextGL->Render();
			}
		}

	}
	else if(m_type == BT_HOMEPAGE_DEVICE)
	{
		SetFrameTime(m_bDeviceSearching?30:500);

		m_boxDeviceList.SetAlpha(m_fCurAlpha);
		m_boxDeviceList.Render();

		m_btnSearchDevice.SetAlpha(m_fCurAlpha);
		m_btnSearchDevice.Render();


		//show status:
		if(m_bDeviceSearching)
		{
			//m_iconBT.SetAlpha(m_fCurAlpha);
			//m_iconBT.Render();

			VIEW_STATE vs=m_iconSearching.GetCurViewState();
			vs.fRotateZ-=5;
			if(vs.fRotateZ<-360)
				vs.fRotateZ=0;
			m_iconSearching.SetCurViewState(&vs);
			m_iconSearching.SetAlpha(m_fCurAlpha);
			m_iconSearching.Render();
		}
		else
		{
			if(m_boxDeviceList.GetCount()==0)
			{
				SIZE si;

				const wchar_t *pStNote=GetResourceString(IDS_NO_DEVICE_FOUND);
				if(pStNote)
				{
					m_pTextGL->SetColor(&cr_gray,FALSE);
					m_pTextGL->SetString(pStNote);
					m_pTextGL->SetCharScale(1.2f,1.2f);
					m_pTextGL->GetExtension(&si);
					m_pTextGL->SetPos(-si.cx/2+110,130);
					m_pTextGL->Render();
				}

				const wchar_t *pStNote2=GetResourceString(IDS_PHONECORE_FINDME_NOTE);
				if(pStNote2)
				{
					m_pTextGL->SetColor(&cr_gray,FALSE);
					m_pTextGL->SetString(pStNote2);
					m_pTextGL->SetCharScale(0.85f,0.85f);
					m_pTextGL->GetExtension(&si);
					m_pTextGL->SetPos(-si.cx/2+110,40);
					m_pTextGL->Render();
				}
			}
		}

// 		m_btnPairedDevice.SetAlpha(m_fCurAlpha);
// 		m_btnPairedDevice.Render();

// 		m_tagLine.SetAlpha(m_fCurAlpha);
// 		m_tagLine.Render();

	}
	else if(m_type == BT_HOMEPAGE_HISTORY)
	{
		m_boxHistory.SetAlpha(m_fCurAlpha);
		m_boxHistory.Render();
	}

	EndRender();
}

BOOL CPanelBluetoothMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);

	m_iconTypeHome.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_549));
	m_iconTypeMobile.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_548));
	m_iconTypeWork.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_547));

	m_iconOut.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_552));
	m_iconIn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_550));
	m_iconMissed.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_551));


	//list:
	m_iconDevice.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_504));
	m_iconHistory.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_506));
	m_iconContact.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_508));
	m_iconDelete.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_510));

	m_iconDeviceFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_505));
	m_iconHistoryFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_507));
	m_iconContactFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_509));
	m_iconDeleteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_511));


	COLORGL cr_gray={164.0f/256.0f,164.0f/256.0f,164.0f/256.0f,1.0};
	COLORGL cr_red={255.0f/256.0f,16.0f/256.0f,62.0f/256.0f,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};

	int cx=220;
	int cy=100;

	SIZE sib={cx,cy};

	//
	m_iconSearching.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconSearching.SetShowScale(1.8f);
	m_iconSearching.SetPos(cx/2,cy/2);
	m_iconBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_121));
	m_iconBT.SetShowScale(1.7f);
	m_iconBT.SetPos(cx/2,-cy/2);

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_502));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_503));

	m_btnHome.Initialize(IDC_BT_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sib);
	m_btnHome.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,88);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_b);
	m_btnHome.SetCaptionHeight(24);
	m_btnHome.SetIconPos(-cx/2+46,0);

	//
	SIZE si_btn={(g_iClientWidth-cx),cy+0};

/*
	m_iconPairedDeviceUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_504));
	m_iconPairedDeviceDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_530));

	m_btnPairedDevice.Initialize(IDC_BT_BTN_PAIRED_DEVICE,this,m_pSkinManager->GetTexture(TEXID_066),NULL,
		m_pSkinManager->GetTexture(TEXID_067),NULL,
		m_pSkinManager->GetTexture(TEXID_066),NULL,
		NULL,NULL);
	m_btnPairedDevice.SetSize(&sic);
	m_btnPairedDevice.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnPairedDevice.SetPos(-(g_iClientWidth-cx)/4+cx/2,g_iClientHeight/2-cy/2);
	m_btnPairedDevice.LinkTextGL(pTextGL);

	m_btnPairedDevice.SetIcon(&m_iconPairedDeviceUp,&m_iconPairedDeviceDown);
	m_btnPairedDevice.SetCaption(GetResourceString(IDS_PAIRED));
	m_btnPairedDevice.SetCaptionPos(BTN_CAPTION_CENTER,0,-28);
	m_btnPairedDevice.SetCaptionColor(&cr_gray,&cr_red);
	m_btnPairedDevice.SetCaptionHeight(18);
	m_btnPairedDevice.SetIconPos(0,10);
*/

	m_iconSearchDeviceUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_013));
	m_iconSearchDeviceDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_019));

	m_btnSearchDevice.Initialize(IDC_BT_BTN_SEARCHDEVICE,this,m_pSkinManager->GetTexture(TEXID_066),NULL,
		m_pSkinManager->GetTexture(TEXID_067),NULL,
		m_pSkinManager->GetTexture(TEXID_066),NULL,
		NULL,NULL);
	m_btnSearchDevice.SetSize(&si_btn);
	m_btnSearchDevice.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnSearchDevice.SetPos(/*(g_iClientWidth-cx)/4+*/cx/2,-g_iClientHeight/2+cy/2);
	m_btnSearchDevice.LinkTextGL(pTextGL);

	m_btnSearchDevice.SetIcon(&m_iconSearchDeviceUp,&m_iconSearchDeviceDown);
	m_btnSearchDevice.SetCaption(GetResourceString(IDS_SEARCH));
// 	m_btnSearchDevice.SetCaptionPos(BTN_CAPTION_CENTER,0,-28);
// 	m_btnSearchDevice.SetCaptionColor(&cr_gray,&cr_red);
// 	m_btnSearchDevice.SetCaptionHeight(18);
// 	m_btnSearchDevice.SetIconPos(0,10);
	m_btnSearchDevice.SetCaptionPos(BTN_CAPTION_LEFT,(g_iClientWidth-cx)/2-10,0);
	m_btnSearchDevice.SetCaptionColor(&cr_w,&cr_red);
	m_btnSearchDevice.SetCaptionHeight(32);
	m_btnSearchDevice.SetIconPos(-50,0);


	//
	int cx_box=340 * g_iScreenWidth / 1024;

	SIZE si0={cx_box-20,80};

	//download
	m_btnDownloadPhonebook.Initialize(IDC_BT_BTN_CONTACT_DOWNLOAD,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDownloadPhonebook.SetCaption(GetResourceString(IDS_DOWNLOAD));
	m_btnDownloadPhonebook.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDownloadPhonebook.SetCaptionHeight(32);
	m_btnDownloadPhonebook.LinkTextGL(pTextGL);
	m_btnDownloadPhonebook.SetSize(&si0);
	m_btnDownloadPhonebook.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnDownloadPhonebook.SetPos(-g_iClientWidth/2+cx+cx_box/2,g_iClientHeight/2-60);

	m_btnCancelDownloadPhonebook.Initialize(IDC_BT_BTN_CONTACT_CANCEL_DOWNLOAD,this,m_pSkinManager->GetTexture(TEXID_533),NULL,
		m_pSkinManager->GetTexture(TEXID_534),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancelDownloadPhonebook.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancelDownloadPhonebook.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancelDownloadPhonebook.SetCaptionHeight(32);
	m_btnCancelDownloadPhonebook.LinkTextGL(pTextGL);
	m_btnCancelDownloadPhonebook.SetSize(&si0);
	m_btnCancelDownloadPhonebook.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnCancelDownloadPhonebook.SetPos(-g_iClientWidth/2+cx+cx_box/2,g_iClientHeight/2-60);
	m_btnCancelDownloadPhonebook.Show(FALSE);

	m_iconSearchContactUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_013));

	m_btnSearchContact.Initialize(IDC_BT_BTN_SEARCH_CONTACT,this,m_pSkinManager->GetTexture(TEXID_512),NULL,
		m_pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
// 	m_btnSearchContact.SetCaption(GetResourceString(IDS_DOWNLOAD));
// 	m_btnSearchContact.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_btnSearchContact.SetCaptionHeight(32);
// 	m_btnSearchContact.LinkTextGL(pTextGL);
	m_btnSearchContact.SetSize(&si0);
	m_btnSearchContact.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnSearchContact.SetPos(-g_iClientWidth/2+cx+cx_box/2,g_iClientHeight/2-45);
	m_btnSearchContact.SetIcon(&m_iconSearchContactUp);
	m_btnSearchContact.Show(FALSE);

	//
	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));

	//find param:
	int count=(g_iClientHeight-cy)/100;
	if((g_iClientHeight-cy)%100 > 50)
		count+=1;

	box.iSpacing=(g_iClientHeight-cy)/count;///2*2;

	box.siClient.cx=cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=cx;
	box.siBorder.cy=g_iClientHeight-cy;

	box.idTexIndexBase=TEXID_603;
	box.idTexIndexFocus=TEXID_604;
	box.idTexBase=TEXID_603;
	box.bNoSlider=TRUE;
	box.bNoBorder=FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxLists.Initialize(IDC_BT_BOX_MENU,this,&box,pSkinManager);
	m_boxLists.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxLists.SetPos(-g_iClientWidth/2+cx/2,cy/2);
	m_boxLists.LinkTextGL(pTextGL);
	m_boxLists.SetAlpha(1.0f);
	m_boxLists.SetCaptioniHeight(24);
	m_boxLists.SetCaptionColor(&cr_gray,&cr_b);

	CListBoxItemGL *pItem;
	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_MGR));
	pItem->SetIcon(&m_iconDevice,&m_iconDeviceFocus);
	m_boxLists.AddItem(pItem,BT_HOMEPAGE_DEVICE);
//  	pItem=new CListBoxItemGL(GetResourceString(IDS_CALL_HISTORY));
//  	pItem->SetIcon(&m_iconHistory,&m_iconHistoryFocus);
//  	m_boxLists.AddItem(pItem,BT_HOMEPAGE_HISTORY);
//  	pItem=new CListBoxItemGL(GetResourceString(IDS_CONTACT));
//  	pItem->SetIcon(&m_iconContact,&m_iconContactFocus);
//  	m_boxLists.AddItem(pItem,BT_HOMEPAGE_CONTACT);


	//list:
	memset(&box,0,sizeof(box));

	//find param:
	count=(g_iClientHeight-cy-20)/100;
	if((g_iClientHeight-cy-20)%100 > 50)
		count+=1;

	box.iSpacing=(g_iClientHeight-cy-20)/count/2*2;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=g_iClientHeight-cy;//(box.iSpacing * count);


	box.idTexIndexBase=TEXID_077;//TEXID_606;//
	box.idTexIndexFocus=TEXID_077;//TEXID_607;//
	box.idTexBase=TEXID_066;//TEXID_301;//
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=FALSE;
	box.pxSlider=-20;
	box.fIndexBaseOffset=74;

	m_boxDeviceList.Initialize(IDC_BT_BOX_DEVICE_LIST,this,&box,pSkinManager);
	m_boxDeviceList.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxDeviceList.SetMarginHor(40);
	m_boxDeviceList.SetPos(cx/2,cy/2);
	m_boxDeviceList.LinkTextGL(pTextGL);
	m_boxDeviceList.SetAlpha(1.0f);
	m_boxDeviceList.SetCaptioniHeight(32,24,18);
	m_boxDeviceList.SetCaptionColor(&cr_b,&cr_b);

	//paired device:
// 	m_boxDevicePaired.Initialize(IDC_BT_BOX_DEVICE_PAIRED,this,&box,pSkinManager);
// 	m_boxDevicePaired.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
// 	m_boxDevicePaired.SetMarginHor(40);
// 	m_boxDevicePaired.SetPos(cx/2,-cy/2);
// 	m_boxDevicePaired.LinkTextGL(pTextGL);
// 	m_boxDevicePaired.SetAlpha(1.0f);
// 	m_boxDevicePaired.SetCaptioniHeight(32,24,18);
//  	m_boxDevicePaired.SetCaptionColor(&cr_b,&cr_b);

	//test:
//  	pItem=new CListBoxItemGL(_T("wangyiping's Window Phone "),NULL,_T("Paired"));
//  	pItem->SetIcon(&m_iconDeviceFocus);
//  	pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
//  	m_boxDevicePaired.AddItem(pItem);
// 	pItem=new CListBoxItemGL(_T("wangyiping's Window Phone "),NULL,_T("Paired"));
// 	pItem->SetIcon(&m_iconDeviceFocus);
// 	pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
// 	m_boxDevicePaired.AddItem(pItem);


	//paired device:
// 	m_boxDeviceSearched.Initialize(IDC_BT_BOX_DEVICE_SEARCHED,this,&box,pSkinManager);
// 	m_boxDeviceSearched.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
// 	m_boxDeviceSearched.SetMarginHor(40);
// 	m_boxDeviceSearched.SetPos(cx/2,-cy/2);
// 	m_boxDeviceSearched.LinkTextGL(pTextGL);
// 	m_boxDeviceSearched.SetAlpha(1.0f);
// 	m_boxDeviceSearched.SetCaptioniHeight(32,24,18);
// 	m_boxDeviceSearched.SetCaptionColor(&cr_b,&cr_b);

 	//test:
//  	pItem=new CListBoxItemGL(_T("Window Phone "),NULL,_T("unPaired"));
//  	pItem->SetIcon(&m_iconDeviceFocus);
//  	m_boxDeviceSearched.AddItem(pItem);
// 	pItem=new CListBoxItemGL(_T("Window Phone "),NULL,_T("unPaired"));
// 	pItem->SetIcon(&m_iconDeviceFocus);
// 	m_boxDeviceSearched.AddItem(pItem);

	//
	//find param:
	count=(g_iClientHeight-20)/100;
	if((g_iClientHeight-20)%100 > 50)
		count+=1;

	box.iSpacing=(g_iClientHeight-20)/count/2*2;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=g_iClientHeight;//(box.iSpacing * count);
	box.fIndexBaseOffset=74;

	//History
	m_boxHistory.Initialize(IDC_BT_BOX_HISTORY,this,&box,pSkinManager);
	m_boxHistory.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxHistory.SetMarginHor(40);
	m_boxHistory.SetPos(cx/2,0);
	m_boxHistory.LinkTextGL(pTextGL);
	m_boxHistory.SetAlpha(1.0f);
 	m_boxHistory.SetCaptionColor(&cr_b,&cr_b);

	//test:
//  		for (int i=0;i<32;i++)
//  		{
//  			pItem=new CListBoxItemGL(_T("wangyiping "),_T("13888888888"),_T("2012-09-03 12:22:23"));
//  			pItem->SetIcon(&m_iconIn);
//  			m_boxHistory.AddItem(pItem);
//  
//  		}



// 	int cx_box=340;
// 	box.siClient.cx=cx_box;
// 	box.siClient.cy=(box.iSpacing * count);
// 	box.siBorder.cx=cx_box;
// 	box.siBorder.cy=g_iClientHeight;//(box.iSpacing * count);

	//find param:
	count=(g_iClientHeight-100)/84;
	if((g_iClientHeight-100)%84 > 42)
		count+=1;

	box.iSpacing=(g_iClientHeight-100)/count/2*2;

	box.siClient.cx=cx_box;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=cx_box;
	box.siBorder.cy=g_iClientHeight-90;//(box.iSpacing * count);
	box.bQuickSearchBoard=TRUE;
	box.fIndexBaseOffset=15;

	//Contact:
	m_boxContact.Initialize(IDC_BT_BOX_CONTACT,this,&box,pSkinManager);
	m_boxContact.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContact.SetMarginHor(40);
	m_boxContact.SetPos(-g_iClientWidth/2+cx+cx_box/2,-42);
	m_boxContact.LinkTextGL(pTextGL);
	m_boxContact.SetAlpha(1.0f);
	m_boxContact.SetCaptionColor(&cr_b,&cr_b);

//  	for (int i=0;i<32;i++)
//  	{
//  		wchar_t str[32];
//  		swprintf_s(str,32,_T("%cYiping Wang"),'z'-i);
//  		pItem=new CListBoxItemGL(str,NULL,NULL);
//  		pItem->SetIcon(&m_iconContactFocus);
//  		m_boxContact.AddItemByOrder(pItem);
//  	}

	//contact detail:
	//box.iSpacing=(g_iClientHeight-20)/count;

	box.idTexBase=TEXID_605;
	box.siClient.cx=g_iClientWidth-cx-cx_box;
	box.siClient.cy=(box.iSpacing * 3);
	box.siBorder.cx=g_iClientWidth-cx-cx_box;
	box.siBorder.cy=(box.iSpacing * 3)+20;
	box.bQuickSearchBoard=FALSE;
	box.bNoBorder=FALSE;
	box.pxSlider=-30;
	box.fIndexBaseOffset=74;


	m_boxContactDetail.Initialize(IDC_BT_BOX_CONTACT_DETAIL,this,&box,pSkinManager);
	m_boxContactDetail.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContactDetail.SetMarginHor(60);
	m_boxContactDetail.SetPos((cx+cx_box)/2,g_iClientHeight/2-box.iSpacing * 3/2-10);
	m_boxContactDetail.LinkTextGL(pTextGL);
	m_boxContactDetail.Show(FALSE,FALSE);
	m_boxContactDetail.EnableMouseMove(FALSE);
 	m_boxContactDetail.SetCaptionColor(&cr_b,&cr_b);

	//
	COLORGL cr_black={0.0,0.0,0.0,1.0};

	CTexture *pUp=pSkinManager->GetTexture(TEXID_514);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_515);

	int key_margin_x=10;
	int key_margin_y=4;

	int cx_key=(g_iClientWidth-cx-cx_box-key_margin_x*2)/3;
	int cy_key=(g_iClientHeight-key_margin_y*2)/6;

	SIZE si_key={cx_key-4,cy_key-4};
	GLfloat px=g_iClientWidth/2-key_margin_x-2.5f*cx_key;
	GLfloat py=g_iClientHeight/2-key_margin_y-1.5f*cy_key;

	SIZE si_dial={cx_key*3-4,cy_key-4+8};

	SIZE siInfo2={cx_key*5/2,cy_key-4};

	m_tagDialString.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDialString.SetSize(&siInfo2);
	//m_tagDialString.SetCaption(_T("13870204516888888"));
	m_tagDialString.LinkTextGL(pTextGL);
	m_tagDialString.SetPos(g_iClientWidth/2-key_margin_x-62-cx_key*5/4,py+cy_key);
	m_tagDialString.SetCaptionHeight(32);
	m_tagDialString.EnableDrawBack(FALSE);
	m_tagDialString.SetCaptionPos(BTN_CAPTION_RIGHT);
// 	m_tagDialString.SetCaptionColor(&crb);

	m_tagKeyboardBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagKeyboardBk.SetSize(g_iClientWidth-cx-cx_box,g_iClientHeight);
	m_tagKeyboardBk.SetPos(px+cx_key,0);

	m_btnDelete.Initialize(IDC_BT_BTN_KEY_DEL,this,m_pSkinManager->GetTexture(TEXID_519),NULL,
		m_pSkinManager->GetTexture(TEXID_520),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDelete.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnDelete.SetPos(g_iClientWidth/2-key_margin_x-28,py+cy_key);
	m_btnDelete.SetTouchRate(2.0f,1.0f);
	m_btnDelete.EnableHoldNotify(TRUE);

	m_iconDial.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_518));
	m_btnDial.Initialize(IDC_BT_BTN_KEY_DIAL,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDial.SetSize(&si_dial);
	m_btnDial.SetIcon(&m_iconDial);
	m_btnDial.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnDial.SetPos(px+cx_key,py-cy_key*4-4);

	//1-3
	m_btnNum1.Initialize(IDC_BT_BTN_KEY_1,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum1.SetCaptionColor(&cr_black);
	m_btnNum1.SetCaptionHeight(40);
	m_btnNum1.LinkTextGL(pTextGL);
	m_btnNum1.SetSize(&si_key);
	m_btnNum1.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum1.SetPos(px,py);

	m_btnNum2.Initialize(IDC_BT_BTN_KEY_2,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum2.SetCaptionColor(&cr_black);
	m_btnNum2.SetCaptionHeight(40);
	m_btnNum2.LinkTextGL(pTextGL);
	m_btnNum2.SetSize(&si_key);
	m_btnNum2.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum2.SetPos(px+cx_key,py);

	m_btnNum3.Initialize(IDC_BT_BTN_KEY_3,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum3.SetCaptionColor(&cr_black);
	m_btnNum3.SetCaptionHeight(40);
	m_btnNum3.LinkTextGL(pTextGL);
	m_btnNum3.SetSize(&si_key);
	m_btnNum3.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum3.SetPos(px+cx_key*2,py);

	//4-6:
	m_btnNum4.Initialize(IDC_BT_BTN_KEY_4,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum4.SetCaptionColor(&cr_black);
	m_btnNum4.SetCaptionHeight(40);
	m_btnNum4.LinkTextGL(pTextGL);
	m_btnNum4.SetSize(&si_key);
	m_btnNum4.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum4.SetPos(px,py-cy_key);

	m_btnNum5.Initialize(IDC_BT_BTN_KEY_5,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum5.SetCaptionColor(&cr_black);
	m_btnNum5.SetCaptionHeight(40);
	m_btnNum5.LinkTextGL(pTextGL);
	m_btnNum5.SetSize(&si_key);
	m_btnNum5.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum5.SetPos(px+cx_key,py-cy_key);

	m_btnNum6.Initialize(IDC_BT_BTN_KEY_6,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum6.SetCaptionColor(&cr_black);
	m_btnNum6.SetCaptionHeight(40);
	m_btnNum6.LinkTextGL(pTextGL);
	m_btnNum6.SetSize(&si_key);
	m_btnNum6.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum6.SetPos(px+cx_key*2,py-cy_key);


	//7-9:
	m_btnNum7.Initialize(IDC_BT_BTN_KEY_7,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum7.SetCaptionColor(&cr_black);
	m_btnNum7.SetCaptionHeight(40);
	m_btnNum7.LinkTextGL(pTextGL);
	m_btnNum7.SetSize(&si_key);
	m_btnNum7.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum7.SetPos(px,py-cy_key*2);

	m_btnNum8.Initialize(IDC_BT_BTN_KEY_8,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum8.SetCaptionColor(&cr_black);
	m_btnNum8.SetCaptionHeight(40);
	m_btnNum8.LinkTextGL(pTextGL);
	m_btnNum8.SetSize(&si_key);
	m_btnNum8.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum8.SetPos(px+cx_key,py-cy_key*2);

	m_btnNum9.Initialize(IDC_BT_BTN_KEY_9,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum9.SetCaptionColor(&cr_black);
	m_btnNum9.SetCaptionHeight(40);
	m_btnNum9.LinkTextGL(pTextGL);
	m_btnNum9.SetSize(&si_key);
	m_btnNum9.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum9.SetPos(px+cx_key*2,py-cy_key*2);

	//*0#:
	m_btnStar.Initialize(IDC_BT_BTN_KEY_STAR,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnStar.SetCaption(L"*");
	m_btnStar.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnStar.SetCaptionColor(&cr_black);
	m_btnStar.SetCaptionHeight(40);
	m_btnStar.LinkTextGL(pTextGL);
	m_btnStar.SetSize(&si_key);
	m_btnStar.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnStar.SetPos(px,py-cy_key*3);

	m_btnNum0.Initialize(IDC_BT_BTN_KEY_0,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum0.SetCaptionColor(&cr_black);
	m_btnNum0.SetCaptionHeight(40);
	m_btnNum0.LinkTextGL(pTextGL);
	m_btnNum0.SetSize(&si_key);
	m_btnNum0.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum0.SetPos(px+cx_key,py-cy_key*3);

	m_btnWell.Initialize(IDC_BT_BTN_KEY_WELL,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnWell.SetCaption(L"#");
	m_btnWell.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnWell.SetCaptionColor(&cr_black);
	m_btnWell.SetCaptionHeight(40);
	m_btnWell.LinkTextGL(pTextGL);
	m_btnWell.SetSize(&si_key);
	m_btnWell.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnWell.SetPos(px+cx_key*2,py-cy_key*3);


	//
	SetShowType(BT_HOMEPAGE_DEVICE);

	//
	UpdateDeviceList();
	QueryCallHistory();

	return TRUE;
}
void CPanelBluetoothMain::OnPairedDeviceListChanged()
{
	UpdateDeviceList();
}

void CPanelBluetoothMain::UpdateDeviceList()
{
	m_boxDeviceList.ClearList();

	QueryPairedDevice();
	QuerySearchedDevice();
}
void CPanelBluetoothMain::QueryPairedDevice()
{

	m_pCurConnectedDevice = NULL;

	CBTDevList *pbtdPairedDeviceList;
	g_BTModule.pDataPool->GetData(CDataPool::PairedDeviceList, (const void **)&pbtdPairedDeviceList);

	if (pbtdPairedDeviceList == NULL)
	{
		BTDBG(ZONE_ERROR, (TEXT("<%s> GetData  ERROR \r\n"), _T(__FUNCTION__)));
		return;
	}

	BTDEV *pCursor = pbtdPairedDeviceList->m_pbtdList;
	DWORD dwCount = pbtdPairedDeviceList->m_dwCntList;

	RETAILMSG(DEBUG_BT,(_T("##CPanelBTDevice QueryPairedDevice,count:%d\r\n"),dwCount));

	if(!pCursor || dwCount==0)
	{
		return;
	}

	CListBoxItemGL *pItem = NULL;
	BTSERVICE *pbts=NULL;
	wchar_t strSub[MAX_PATH];

	//IDS_PHONESTATUS_CONNECTED

// 	::LoadString( m_hInstance, IDS_PHONESTATUS_CONNECTED+g_idLanguageOffset, strSub, MAX_PATH );

	while (pCursor)
	{
		swprintf_s(strSub,MAX_PATH,GetResourceString(IDS_CONNECTED));
		BOOL bConnected=FALSE;

		pItem = NULL;

		//update service:
		if(pCursor->fActive)
		{
			if ((pbts = pCursor->FindService(BTENUM_DEVICE_HANDSFREE_AG)) != NULL && pbts->fActive == TRUE)
			{
				bConnected=TRUE;
				wcscat_s(strSub,MAX_PATH,_T(" HFP"));
			}
			if ((pbts = pCursor->FindService(BTENUM_DEVICE_A2DP)) != NULL && pbts->fActive == TRUE)
			{
				bConnected=TRUE;
				wcscat_s(strSub,MAX_PATH,_T(" A2DP"));
			}
			if ((pbts = pCursor->FindService(BTENUM_DEVICE_AVRCP)) != NULL && pbts->fActive == TRUE)
			{
				bConnected=TRUE;
				wcscat_s(strSub,MAX_PATH,_T(" AVRCP"));
			}
			if ((pbts = pCursor->FindService(BTENUM_DEVICE_HID)) != NULL && pbts->fActive == TRUE)
			{
				bConnected=TRUE;
				wcscat_s(strSub,MAX_PATH,_T(" HID"));
			}
			if ((pbts = pCursor->FindService(BTENUM_DEVICE_MAP_MSE)) != NULL && pbts->fActive == TRUE)
			{
				bConnected=TRUE;
				wcscat_s(strSub,MAX_PATH,_T(" MAP"));
			}
			if ((pbts = pCursor->FindService(BTENUM_DEVICE_TPMS)) != NULL && pbts->fActive == TRUE)
			{
				bConnected=TRUE;
				wcscat_s(strSub,MAX_PATH,_T(" TPMS"));
			}
			if ((pbts = pCursor->FindService(BTENUM_DEVICE_SPP)) != NULL && pbts->fActive == TRUE)
			{
				bConnected=TRUE;
				wcscat_s(strSub,MAX_PATH,_T(" SPP"));
			}

			if(!bConnected)
			{
				pCursor->fActive = FALSE;
			}
			else
			{
				m_pCurConnectedDevice = pCursor;
			}
		}

		if(m_bManualConnecting)
		{
			if(m_pCurSellectedDevice == pCursor)
			{
				pItem = new CListBoxItemGL(pCursor->szDeviceName,NULL,GetResourceString(IDS_CONNECTING));
			}
			else
			{
				pItem = new CListBoxItemGL(pCursor->szDeviceName,NULL,NULL/*GetResourceString(IDS_DISCONNECTED)*/);
			}
		}
		else if(m_bManualDisconnecting)
		{
			if(m_pCurSellectedDevice == pCursor)
			{
				pItem = new CListBoxItemGL(pCursor->szDeviceName,NULL,GetResourceString(IDS_DISCONNECTING));
			}
			else
			{
				pItem = new CListBoxItemGL(pCursor->szDeviceName,NULL,NULL/*GetResourceString(IDS_DISCONNECTED)*/);
			}
		}
		else
		{
			pItem = new CListBoxItemGL(pCursor->szDeviceName,NULL,bConnected?strSub:GetResourceString(IDS_BT_DEVICE_PAIRED));
		}

		//
		if(pItem)
		{
			COLORGL cr_blue={0.0f/256.0f,30.0f/256.0f,180.0f/256.0f,1.0f};

	// 		pItem->SetIcon(pCursor->fActive?&m_iconPairedDeviceDown:&m_iconDeviceFocus);
			pItem->SetIcon(&m_iconDeviceFocus);
			pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
			pItem->SetUserParam((LPARAM)pCursor);
			if(bConnected)
			{
				pItem->SetCaptionColor(&cr_blue,&cr_blue);
			}
			m_boxDeviceList.AddItem(pItem,1);
		}

		pCursor = pCursor->pNext;
	}

}

void CALLBACK CPanelBluetoothMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_MEDIA,(_T("CPanelBluetoothMain OnNotify id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelBluetoothMain *panel=(CPanelBluetoothMain*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelBluetoothMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_BT_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_0:
		{
			Input('0');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_1:
		{
			Input('1');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_2:
		{
			Input('2');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_3:
		{
			Input('3');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_4:
		{
			Input('4');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_5:
		{
			Input('5');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_6:
		{
			Input('6');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_7:
		{
			Input('7');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_8:
		{
			Input('8');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_9:
		{
			Input('9');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_STAR:
		{
			Input('*');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_WELL:
		{
			Input('#');
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_DEL:
		{
			ClearInput(1);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BOX_MENU:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
				{

				}
				break;
			case NOTIFY_SHORT_CLICK:
				{
					//switch type
					if(id_item==BT_HOMEPAGE_DEVICE)
					{
						m_bShowSearchedDevice = FALSE;
						UpdateDeviceList();
					}
					else if(id_item==BT_HOMEPAGE_HISTORY)
					{
						QueryCallHistory();
					}
					SetShowType(BT_HOMEPAGE_TYPE(id_item));
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_BT_BOX_CONTACT:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
				{
					ShowContactDetail(TRUE,m_boxContact.GetCurFocusItemName());
					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_DOUBLE_CLICK:
				{
					ShowContactDetail(TRUE,m_boxContact.GetCurFocusItemName());
					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_VALUE_CHANGE:
				{
					ShowContactDetail(TRUE,m_boxContact.GetCurFocusItemName());
				}
				break;
			}
		}
		break;
	case IDC_BT_BOX_CONTACT_DETAIL:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					ShowContactDetail(FALSE);
					SetDialString(m_boxContactDetail.GetCurFocusItemSubName());
					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_VALUE_CHANGE:
				{
					m_dwShowContactDetailTick = GetTickCount();
				}
				break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_DIAL:
		{
			if(m_strDialString.String())
			{
				m_strRedialString = m_strDialString;
				g_BTModule.pApi->apiHF.MakeOutgoingCallPhoneNumber((wchar_t*)m_strDialString.String());
				ClearInput(0);
			}
			else if(m_strRedialString.String())
			{
				SetDialString(m_strRedialString.String());
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_CONTACT_DOWNLOAD:
		{
			DownloadPhonebook();
			//m_btnDownloadPhonebook.SetCaption(_T("Downloading..."));
			//m_btnDownloadPhonebook.Enable(FALSE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_CONTACT_CANCEL_DOWNLOAD:
		{
			CancelDownloadPhonebook();
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_SEARCH_CONTACT:
		{
			m_boxContact.ShowQuickSearchMode(!m_boxContact.IsOnQuickSearchMode());
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BOX_HISTORY:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					//g_BTModule.pApi->apiHF.MakeOutgoingCallPhoneNumber(m_boxHistory.GetCurFocusItemValue());

					CM_String str(GetResourceString(IDS_CALL));
					str+=m_boxHistory.GetCurFocusItemName();

					ShowNotificationBox(m_hMainWnd,IDC_BT_BOX_HISTORY_DIAL_CONFIRM,
						&CPanelBluetoothMain::OnNotify,
						this,
						0,
						str.String(),
						m_boxHistory.GetCurFocusItemValue(),
						0);

					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
		
	case IDC_BT_BOX_DEVICE_LIST:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				if(id_item == 1)//paired device:
				{
					BTDEV *pbtd=(BTDEV *)m_boxDeviceList.GetCurFocusItemUserParam();
					m_pCurSellectedDevice = pbtd;
					if(pbtd)
					{
						if(pbtd->fActive)
						{
							//DisconnectDevice(pbtd);
							ShowNotificationBox(m_hMainWnd,IDC_BT_DISCONNECT_CONFIRM,
								&CPanelBluetoothMain::OnNotify,
								this,
								0,
								GetResourceString(IDS_DISCONNECT_DEVICE),
								m_boxDeviceList.GetCurFocusItemName(),
								0);
						}
						else
						{
							m_boxDeviceList.SetCurFocusItemSubName(GetResourceString(IDS_CONNECTING));

							m_boxDeviceList.Enable(FALSE);
							g_BTModule.pApi->apiSearchPair.StopAutoConnect();
							
							SetTimer(g_BTModule.hWndHidden,TIMER_BT_BEGIN_CONNECT,200,NULL);
						}
					}
				}
				else if(id_item == 0)//not paired
				{
					BTDEV *pbtd=(BTDEV *)m_boxDeviceList.GetCurFocusItemUserParam();
					m_pCurSellectedDevice = pbtd;

					if(pbtd && !m_boxDeviceList.GetCurFocusItemSubName())
					{
						m_boxDeviceList.SetCurFocusItemSubName(GetResourceString(IDS_BT_PAIRING));
						m_boxDeviceList.Enable(FALSE);

						g_BTModule.pApi->apiSearchPair.StopAutoConnect();

						//
						SetTimer(g_BTModule.hWndHidden,TIMER_BT_BEGIN_PAIR,200,NULL);
					}
				}

				PlaySoundEffect(0);

				break;
			case NOTIFY_CONTROL://delete current:
				if(id_item == 1)//paired
				{
					m_boxDeviceList.MoveToIndex(index);
					BTDEV *pbtd=(BTDEV *)m_boxDeviceList.GetCurFocusItemUserParam();
					if(pbtd)
					{
						UnpairDevice(pbtd);
						Sleep(100);
						UpdateDeviceList();
					}
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
		
	case IDC_BT_BOX_HISTORY_DIAL_CONFIRM:
		{
			if(wMsg == NB_OK)//confirm to dial:
			{
				RETAILMSG(DEBUG_BT,(_T("----confirm to dial-----\r\n")));
				//to do:
				PhoneDial(m_boxHistory.GetCurFocusItemValue());
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_DISCONNECT_CONFIRM:
		{
			if(wMsg == NB_OK)
			{
				if(m_pCurSellectedDevice)
				{
					DisconnectDevice(m_pCurSellectedDevice);
// 					m_pCurSellectedDevice=NULL;

					//
					m_bManualDisconnecting = TRUE;
					m_boxDeviceList.Enable(FALSE);
					SetTimer(g_BTModule.hWndHidden,TIMER_BT_DISCONNECT_ALL_FINISH,8000,NULL);
				}
			}
			PlaySoundEffect(0);
		}
		break;
		
	case IDC_BT_BTN_SEARCHDEVICE:
		{
			if(m_bDeviceSearching)
			{
				CancelSearchDevice();
			}
			else
			{
				SearchDevice();
			}

			m_boxDeviceList.Show(m_type == BT_HOMEPAGE_DEVICE && !m_bDeviceSearching);
			m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));

			PlaySoundEffect(0);
		}
		break;
		
		/*
	case IDC_BT_BTN_PAIRED_DEVICE:
		{
			if(!m_btnPairedDevice.IsOnFocus())
			{
				QueryPairedDevice();
			}

			m_bShowSearchedDevice = FALSE;

			m_boxDeviceList.Show(m_type == BT_HOMEPAGE_DEVICE && !m_bDeviceSearching);

// 			m_boxDevicePaired.Show(m_type == BT_HOMEPAGE_DEVICE && !m_bShowSearchedDevice);
// 			m_boxDeviceSearched.Show(m_type == BT_HOMEPAGE_DEVICE && m_bShowSearchedDevice);

			m_btnPairedDevice.SetStatus(m_bShowSearchedDevice?BS_UP:BS_FOCUS);
			m_btnSearchDevice.SetStatus(m_bShowSearchedDevice?BS_FOCUS:BS_UP);

		}
		break;
	case IDC_BT_BOX_DEVICE_SEARCHED:
		if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
		{
			BTDEV *pbtd=(BTDEV *)m_boxDeviceSearched.GetCurFocusItemUserParam();
			m_pCurSellectedDevice = pbtd;

			if(pbtd && !m_boxDeviceSearched.GetCurFocusItemSubName())
			{
// 				Disconnect();
// 				//m_pCurDevice = pbtd;
// 				PairDevice(pbtd);

				//
				m_boxDeviceSearched.SetCurFocusItemSubName(GetResourceString(IDS_BT_PAIRING));

				m_boxDevicePaired.Enable(FALSE);
				m_boxDeviceSearched.Enable(FALSE);
				g_BTModule.pApi->apiSearchPair.StopAutoConnect();

				//
				SetTimer(g_BTModule.hWndHidden,TIMER_BT_BEGIN_PAIR,200,NULL);

				//
				//thread to pair and connect:

			}
		}
		break;
		*/
	}
}
void CPanelBluetoothMain::OnDisconnectAllFinish()
{
	RETAILMSG(1,(_T("##CPanelBluetoothMain::OnDisconnectAllFinish.... \r\n")));

	m_bManualDisconnecting = FALSE;
	m_boxDeviceList.Enable(!m_bAutoConnecting && !m_bManualConnecting);

	UpdateDeviceList();

	m_pCurSellectedDevice = NULL;
}
DWORD CPanelBluetoothMain::ConnectThreadPro(LPVOID lpParam)
{
	RETAILMSG(DEBUG_BT,(_T("##CPanelBluetoothMain::ConnectThreadPro begin.... \r\n")));

	CPanelBluetoothMain *p= (CPanelBluetoothMain*)lpParam;

	if(p)
	{
		if(p->m_pCurSellectedDevice)
		{
			if(p->m_pCurConnectedDevice)//already connect one
			{
				p->Disconnect();
				Sleep(4000);
			}
			p->ConnectDevice(p->m_pCurSellectedDevice);
		}
		else
		{
			p->m_boxDeviceList.Enable(!p->m_bAutoConnecting && !p->m_bManualConnecting);
		}
	}

	RETAILMSG(DEBUG_BT,(_T("##CPanelBluetoothMain::ConnectThreadPro end.... \r\n")));

	return 0;
}
DWORD CPanelBluetoothMain::PairThreadPro(LPVOID lpParam)
{
	RETAILMSG(DEBUG_BT,(_T("##CPanelBluetoothMain::PairThreadPro begin.... \r\n")));

	CPanelBluetoothMain *p= (CPanelBluetoothMain*)lpParam;

	if(p)
	{
		if(p->m_pCurSellectedDevice)
		{
			if(p->m_pCurConnectedDevice)//already connect one
			{
				p->Disconnect();
				Sleep(4000);
			}
			p->PairDevice(p->m_pCurSellectedDevice);
		}
		else
		{
			p->m_boxDeviceList.Enable(!p->m_bAutoConnecting && !p->m_bManualConnecting);
		}
	}

	RETAILMSG(DEBUG_BT,(_T("##CPanelBluetoothMain::PairThreadPro end.... \r\n")));

	return 0;
}
void CPanelBluetoothMain::OnBeginConnect()
{
	if(!m_bManualConnecting)
	{
		m_bManualConnecting = TRUE;

		HANDLE h = CreateThread(NULL,NULL,CPanelBluetoothMain::ConnectThreadPro,this,NULL,NULL);
		//CeSetThreadPriority(h, CE_THREAD_PRIO_256_BELOW_NORMAL); 
		CloseHandle(h);
	}
}
void CPanelBluetoothMain::OnBeginPair()
{
	if(!m_bManualConnecting)
	{
		m_bManualConnecting = TRUE;

		HANDLE h = CreateThread(NULL,NULL,CPanelBluetoothMain::PairThreadPro,this,NULL,NULL);
		//CeSetThreadPriority(h, CE_THREAD_PRIO_256_BELOW_NORMAL); 
		CloseHandle(h);
	}
}
void CPanelBluetoothMain::Disconnect()
{
	if(m_pCurConnectedDevice)//already connect one
	{
		DisconnectDevice(m_pCurConnectedDevice);
		m_pCurConnectedDevice = NULL;
	}
}
CONTACT_STATUS CPanelBluetoothMain::GetPhonebookStatus()
{
	return m_statusContact;
}
void CPanelBluetoothMain::CancelDownloadPhonebook()
{

	DWORD dwRet = g_BTModule.pApi->apiPB.Cancel();
	if (dwRet != ERROR_SUCCESS)
	{

	}

	m_statusContact = CONTACT_DOWNLOADCANCLED;
	m_dwContactDownloadedCount = 0;

}

void CPanelBluetoothMain::DownloadPhonebook()
{
	RETAILMSG(DEBUG_BT,(_T("##CPanelBTContact DownloadPhonebook \r\n")));

	if(	m_statusContact != CONTACT_DOWNLOADING)
	{
		m_dwContactDownloadTick = GetTickCount();

		//clear box first:
		//m_boxContact.ClearList();
// 		m_sliDownloadProgress.SetValue(0);

// 		m_dwDownloadedCount = 0;

		DWORD dwRet = ERROR_SUCCESS;

		//the first time or g_pbtdSelPBAP has been deleted, set a default device
		// 		if (g_pbtdSelPBAP == NULL)
		{
			//set the connected device
			if (GetConnectedDeviceOne(BTENUM_DEVICE_PHONEBOOK_PSE) || GetConnectedDeviceOne(BTENUM_DEVICE_HANDSFREE_AG))
			{
				g_pbtdSelPBAP = g_pbtdConnectedOne;
			}
			//set the paired device
			else if (g_BTModule.pApi->apiSearchPair.m_pbtdPairedList != NULL)
			{
				BTDEV *pbtd = g_BTModule.pApi->apiSearchPair.m_pbtdPairedList->m_pbtdList;

				while (pbtd != NULL)
				{
					if ((pbtd->FindService(BTENUM_DEVICE_PHONEBOOK_PSE) != NULL || pbtd->FindService(BTENUM_DEVICE_HANDSFREE_AG) != NULL))
					{
						g_pbtdSelPBAP = pbtd;
						break;
					}
				}
			}
		}

		if (!g_pbtdSelPBAP)
		{        
			BTDBG(ZONE_ERROR, (TEXT("<%s> Download contacts, return due to invalid g_pbtdSelPBAP\r\n"), _T(__FUNCTION__)));
			return;
		}

#ifdef ENABLE_HFP_CHECK_CALL_IDLE
		//limit not to download phonebook when call is busy.
		if (!g_BTModule.pApi->apiHF.IsCallIdle())
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s> apiPB.PullPhonebook fail due to call busy\r\n"), _T(__FUNCTION__))); 

			dwRet = ERROR_BUSY;

			// 			g_BTModule.pLogic->FSM(CLogic::User)->OfferTriggerToFSM(PBAP_API_CALL_FAIL_DOWNLOAD, (void *)dwRet);
			return;
		}
#endif

		g_BTModule.pApi->apiPB.m_btaddr = g_pbtdSelPBAP->b;
		g_BTModule.pApi->apiPB.m_listStartOffset = 0;
		g_BTModule.pApi->apiPB.m_maximumListCount = g_BTModule.pApi->apiPB.m_phonebookEntry;

		if (!_tcsicmp(g_BTModule.pApi->apiPB.m_filename, FILENAME_PHONEBOOK))
		{
			//filter
			g_BTModule.pApi->apiPB.m_specified_filter.fields.formattedName = 1;
			g_BTModule.pApi->apiPB.m_specified_filter.fields.telephoneNumber = 1;
			g_BTModule.pApi->apiPB.m_specified_filter.fields.electornicMailAddress = 1;

			//clear contacts
			g_BTModule.pDataPool->ClearData(CDataPool::ContactsList);

#ifdef STORE_PHONEBOOK_IN_DB
			//avoid to store the same contacts.
			g_BTModule.pApi->apiPB.RemoveAllContacts();
#endif
		}
		else
		{
			//clear callrecords
			g_BTModule.pApi->apiPB.m_specified_filter.fields.formattedName = 1;
			g_BTModule.pApi->apiPB.m_specified_filter.fields.telephoneNumber = 1;
			g_BTModule.pApi->apiPB.m_specified_filter.fields.timeStamp = 1;

			g_BTModule.pDataPool->ClearData(CDataPool::CallRecordsList);
		}

		dwRet = g_BTModule.pApi->apiPB.PullPhonebook(
			g_BTModule.pApi->apiPB.m_btaddr,
			g_BTModule.pApi->apiPB.m_filename,
			g_BTModule.pApi->apiPB.m_source,
			g_BTModule.pApi->apiPB.m_format,
			g_BTModule.pApi->apiPB.m_specified_filter,
			g_BTModule.pApi->apiPB.m_maximumListCount,
			g_BTModule.pApi->apiPB.m_listStartOffset);

		if (dwRet != ERROR_SUCCESS)
		{
			BTDBG(ZONE_ERROR, (TEXT("<%s>apiPB.PullPhonebook fail, error = %d\r\n"), _T(__FUNCTION__), dwRet)); 

			return;
		}

		m_statusContact = CONTACT_DOWNLOADING;
	}
}
INT CPanelBluetoothMain::GetContactDetail(const wchar_t* szName)
{
	RETAILMSG(DEBUG_BT,(_T("##GetContactDetail:%s\r\n"),szName));
	INT res=0;

#ifdef STORE_PHONEBOOK_IN_DB
	BOOL searchLocalContact = g_BTModule.pApi->apiPB.m_useLocalPhonebook /* || (pc->GetFileName() == NULL)*/;

	BTDBG(1/*ZONE_MMI*/, (TEXT("CDlgContact::OnBnClickedButtonDetail  searchLocalContact: %x\r\n"), searchLocalContact));

	if (searchLocalContact)
	{
		Contact *tempContact = g_BTModule.pApi->apiPB.GetContact((LPTSTR)szName);

		if (tempContact)
		{
			// 			#ifdef ZONE_DUMP
			// 			g_BTModule.pApi->apiPB.DumpContact(tempContact);
			// 			#endif

			g_BTModule.pApi->apiPB.SetCurrentContact(tempContact);

			//update details:
			// 			m_panelContactDetail.UpdateDetails(tempContact);
			res = UpdateDetails(tempContact);
		}
	}

#endif

	return res;
}



void CPanelBluetoothMain::ShowContactDetail(BOOL bShow,const wchar_t* szName)
{

	if(bShow)
	{
		if(szName)
		{
			int count=GetContactDetail(szName);

			if(count>1)
			{
				m_dwShowContactDetailTick = GetTickCount();
				m_boxContactDetail.Show(bShow);
			}
			else if(count==1)
			{
				m_boxContactDetail.Show(FALSE);
				SetDialString(m_boxContactDetail.GetCurFocusItemSubName());
			}
		}
	}
	else
	{
		m_dwShowContactDetailTick = GetTickCount();
		m_boxContactDetail.Show(bShow);
	}

}
void CPanelBluetoothMain::OnDeviceConnectFinish(WPARAM wParam)
{
	m_bManualConnecting=FALSE;

	m_boxDeviceList.Enable(!m_bAutoConnecting && !m_bManualConnecting);

	UpdateDeviceList();

	m_pCurSellectedDevice = NULL;
}
void CPanelBluetoothMain::OnHFStateChange(SnmConnectionState status)
{
	m_hfStatus = status;

	if(SNM_CONNECTION_STATE_CONNECTED==status)
	{
		CListBoxItemGL *pItem;

		pItem=new CListBoxItemGL(GetResourceString(IDS_CALL_HISTORY));
		pItem->SetIcon(&m_iconHistory,&m_iconHistoryFocus);
		m_boxLists.AddItem(pItem,BT_HOMEPAGE_HISTORY);
		pItem=new CListBoxItemGL(GetResourceString(IDS_CONTACT));
		pItem->SetIcon(&m_iconContact,&m_iconContactFocus);
		m_boxLists.AddItem(pItem,BT_HOMEPAGE_CONTACT);

		m_bShowSearchedDevice = FALSE;

		if(m_idPickDeviceFor == BT_PICK_DEVICE_FOR_HF || m_idPickDeviceFor == 0)
		{
			SetShowType(BT_HOMEPAGE_CONTACT);
		}
	}
	else
	{
		SetShowType(BT_HOMEPAGE_DEVICE);

		m_boxLists.DeleteItem(BT_HOMEPAGE_HISTORY);
		m_boxLists.DeleteItem(BT_HOMEPAGE_CONTACT);

		m_boxContact.ClearList();
		m_boxHistory.ClearList();

		m_statusContact = CONTACT_NULL;
		m_dwContactDownloadedCount = 0;

	}
}
void CPanelBluetoothMain::UpdateCallHistory()
{
	QueryCallHistory();
}
void CPanelBluetoothMain::OnA2dpConnected(BOOL bConnected)
{
	if(bConnected && m_idPickDeviceFor==BT_PICK_DEVICE_FOR_A2DP)
	{
		SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,MEDIA_TYPE_A2DP);
		SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP);
		//reset:
		m_idPickDeviceFor = 0;
	}
}
void CPanelBluetoothMain::OnPhoneBookDownloadFailed()
{
	m_statusContact = CONTACT_DOWNLOADFAILED;
}
void CPanelBluetoothMain::RefreshContactList()
{
	if(!m_pContactList)
		return;

	RETAILMSG(DEBUG_BT,(_T("##CPanelBTContact UpdatePhoneBook count:%d\r\n"),m_pContactList->m_dwCntList));

	// 	m_btnDownloadPhonebook.Enable(TRUE);
	// 	m_btnDownloadPhonebook.SetCaption(_T("Download"));

	Contact *pCursor = m_pContactList->m_pcrHead;

	CListBoxItemGL* pItem;
	int iCount=0;

	m_boxContact.ClearList();

	while(pCursor)
	{
		pItem = new CListBoxItemGL(pCursor->GetKey());
		pItem->SetIcon(&m_iconContactFocus);
		pItem->SetUserParam((LPARAM)pCursor);
		m_boxContact.AddItemByOrder(pItem);

		iCount++;
		pCursor = pCursor->m_pNext;
	}

	// 	m_bHasPhoneBook = (iCount>0);

	//
	GetContactDetail(m_boxContact.GetCurFocusItemName());

	RETAILMSG(DEBUG_BT,(_T("##UpdatePhoneBook end,  iCount:%d\r\n"),iCount));
}
void CPanelBluetoothMain::OnPhoneBookDownloadFinish(const CContactList *pclist)
{
	if(	m_statusContact == CONTACT_DOWNLOADCANCLED)
	{
		return;
	}

	if(!pclist)
	{
		return;
	}
	if(pclist->m_dwCntList <= 0)
	{
		return;
	}

	m_pContactList = pclist;
	m_statusContact = CONTACT_DOWNLOADED;
	m_bNeedRefreshPhonebook = TRUE;

	RefreshContactList();

}
INT CPanelBluetoothMain::UpdateDetails(Contact *pContact)
{
	//clear first:
	m_boxContactDetail.ClearList();

	if(pContact)
	{
		CListBoxItemGL *pItem=NULL;

		ContactInfo *pContactInfo = pContact->GetContactInfo();

		if(pContactInfo)
		{
			// 			pItem = new CListBoxItemGL((const wchar_t*)::LoadString( m_hInstance, IDS_BTCORE_SET_NAME),NULL,_T("10086"));
			// 			pItem->SetIcon(&m_iconMobile);
			// 			m_boxContactDetail.AddItem(pItem,0);

			//mobile tel
			if (Contact::ValidStr(pContactInfo->mobileTel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_MOBILE),NULL,pContactInfo->mobileTel);
				pItem->SetIcon(&m_iconTypeMobile);
				m_boxContactDetail.AddItem(pItem);
			}
			//mobile tel extension: +++ 
			//consider "radioTel" as mobile tel.
			if (Contact::ValidStr(pContactInfo->radioTel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_MOBILE),NULL,pContactInfo->radioTel);
				pItem->SetIcon(&m_iconTypeMobile);
				m_boxContactDetail.AddItem(pItem);
			}
			//consider "carTel" as mobile tel.
			if (Contact::ValidStr(pContactInfo->carTel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_MOBILE),NULL,pContactInfo->carTel);
				pItem->SetIcon(&m_iconTypeMobile);
				m_boxContactDetail.AddItem(pItem);
			}
			//consider "pager" as mobile tel.
			if (Contact::ValidStr(pContactInfo->pager))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_MOBILE),NULL,pContactInfo->pager);
				pItem->SetIcon(&m_iconTypeMobile);
				m_boxContactDetail.AddItem(pItem);
			}
			//consider "assistantTel" as mobile tel.
			if (Contact::ValidStr(pContactInfo->assistantTel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_MOBILE),NULL,pContactInfo->assistantTel);
				pItem->SetIcon(&m_iconTypeMobile);
				m_boxContactDetail.AddItem(pItem);

			}
			//mobile tel extension: ---

			//home tel
			if (Contact::ValidStr(pContactInfo->homeTel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_HOME),NULL,pContactInfo->homeTel);
				pItem->SetIcon(&m_iconTypeHome);
				m_boxContactDetail.AddItem(pItem);
			}
			//home tel2
			if (Contact::ValidStr(pContactInfo->home2Tel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_HOME),NULL,pContactInfo->home2Tel);
				pItem->SetIcon(&m_iconTypeHome);
				m_boxContactDetail.AddItem(pItem);
			}

			//work tel
			if (Contact::ValidStr(pContactInfo->workTel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_WORK),NULL,pContactInfo->workTel);
				pItem->SetIcon(&m_iconTypeWork);
				m_boxContactDetail.AddItem(pItem);
			}
			//work tel2
			if (Contact::ValidStr(pContactInfo->work2Tel))
			{
				pItem = new CListBoxItemGL(GetResourceString(IDS_LOCATION_WORK),NULL,pContactInfo->work2Tel);
				pItem->SetIcon(&m_iconTypeWork);
				m_boxContactDetail.AddItem(pItem);
			}

		}
	}

	return m_boxContactDetail.GetCount();
}

void CPanelBluetoothMain::SetShowType(BT_HOMEPAGE_TYPE type)
{
	if(type!=BT_HOMEPAGE_DEVICE && type!=BT_HOMEPAGE_CONTACT && type!=BT_HOMEPAGE_HISTORY )
		return;

	m_type = type;

	m_iParam = type;

	//device
// 	m_btnPairedDevice.Show(type == BT_HOMEPAGE_DEVICE);
	m_btnSearchDevice.Show(type == BT_HOMEPAGE_DEVICE);

	m_boxDeviceList.Show(type == BT_HOMEPAGE_DEVICE && !m_bDeviceSearching);
// 	m_boxDevicePaired.Show(type == BT_HOMEPAGE_DEVICE && !m_bShowSearchedDevice);
// 	m_boxDeviceSearched.Show(type == BT_HOMEPAGE_DEVICE && m_bShowSearchedDevice);

// 	m_btnPairedDevice.SetStatus(m_bShowSearchedDevice?BS_UP:BS_FOCUS);
// 	m_btnSearchDevice.SetStatus(m_bShowSearchedDevice?BS_FOCUS:BS_UP);

	//history
	m_boxHistory.Show(type == BT_HOMEPAGE_HISTORY);

	//contact
	m_tagKeyboardBk.Show(type == BT_HOMEPAGE_CONTACT);
	//m_boxContactDetail.Show(type == BT_HOMEPAGE_CONTACT);
	m_tagDialString.Show(type == BT_HOMEPAGE_CONTACT);

	m_boxContact.Show(type == BT_HOMEPAGE_CONTACT && m_statusContact==CONTACT_DOWNLOADED);

	m_btnDownloadPhonebook.Show(type == BT_HOMEPAGE_CONTACT  && m_statusContact==CONTACT_NULL || m_statusContact==CONTACT_DOWNLOADFAILED || m_statusContact==CONTACT_DOWNLOADCANCLED);
	m_btnCancelDownloadPhonebook.Show(type == BT_HOMEPAGE_CONTACT  && m_statusContact==CONTACT_DOWNLOADING && GetTickCount()-m_dwContactDownloadTick>1000);
	m_btnSearchContact.Show(type == BT_HOMEPAGE_CONTACT  && m_statusContact==CONTACT_DOWNLOADED );

	m_btnDial.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnDelete.Show(type == BT_HOMEPAGE_CONTACT);

	m_btnNum0.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum1.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum2.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum3.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum4.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum5.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum6.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum7.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum8.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnNum9.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnStar.Show(type == BT_HOMEPAGE_CONTACT);
	m_btnWell.Show(type == BT_HOMEPAGE_CONTACT);

	//move to current device:
	m_boxLists.MoveToIndex(m_boxLists.SearchID(type));

}

void CPanelBluetoothMain::SetDialString(const wchar_t* pString)
{
	m_strDialString = pString;

	if(m_strDialString.Size())
	{
		int maxlen=g_iClientWidth*11/800;
		if(m_strDialString.Size()<=maxlen)
		{
			m_tagDialString.SetCaption(m_strDialString.String());
		}
		else
		{
			m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
		}
	}
	else
	{
		m_tagDialString.SetCaption(NULL);
	}
}
void CPanelBluetoothMain::Input(wchar_t c)
{
	wchar_t str[8];
	swprintf_s(str,8,_T("%c"),c);

	m_strDialString += str;
	if(m_strDialString.Size())
	{
		int maxlen=g_iClientWidth*11/800;
		if(m_strDialString.Size()<=maxlen)
		{
			m_tagDialString.SetCaption(m_strDialString.String());
		}
		else
		{
			m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
		}
	}

	//
	if(m_boxContactDetail.IsVisible())
	{
		ShowContactDetail(FALSE);
	}
}
void CPanelBluetoothMain::ClearInput(int i)
{
	if(m_strDialString.Size()>0)
	{
		if(i<=0)
			m_strDialString = NULL;
		else
			m_strDialString -= i;

		if(m_strDialString.Size())
		{
			int maxlen=g_iClientWidth*11/800;
			if(m_strDialString.Size()<=maxlen)
			{
				m_tagDialString.SetCaption(m_strDialString.String());
			}
			else
			{
				m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
			}
		}
		else
		{
			m_tagDialString.SetCaption(NULL);
		}
	}
}
void CPanelBluetoothMain::ConnectDevice(BTDEV *pbtd)
{
	DWORD dwRet = g_BTModule.pApi->apiSearchPair.ConnectDeviceAll(pbtd);

	if (dwRet != ERROR_SUCCESS)
	{
		return;
	}
}
void CPanelBluetoothMain::DisconnectDevice(BTDEV *pbtd)
{
	DWORD dwRet = g_BTModule.pApi->apiSearchPair.DisconnectDeviceAll(pbtd);

	if (dwRet != ERROR_SUCCESS)
	{
		return;
	}
}
void CPanelBluetoothMain::UnpairDevice(BTDEV *pbtd)
{

	if(!pbtd)
		return;

	DWORD dwRet =0;

	if(pbtd->fActive)
	{
		dwRet = g_BTModule.pApi->apiSearchPair.DisconnectDeviceAll(pbtd);
		if (dwRet != ERROR_SUCCESS)
		{
			return;
		}	
	}


	dwRet = g_BTModule.pApi->apiSearchPair.DelPairedDevice(pbtd);

	if (dwRet != ERROR_SUCCESS)
	{
		return;
	}
}

void CPanelBluetoothMain::QueryCallHistory()
{
// 	return;

	m_boxHistory.ClearList();

	CCallRecList *pCallRecList;
	g_BTModule.pDataPool->GetData(CDataPool::CallRecordsList, (const void **)&pCallRecList);

	if (pCallRecList == NULL)
	{
		BTDBG(ZONE_ERROR, (TEXT("<%s> GetData  ERROR \r\n"), _T(__FUNCTION__)));
		return;
	}

	CALLREC *pCursor = pCallRecList->m_pcrHead;
	DWORD dwCount = pCallRecList->m_dwCntList;

	RETAILMSG(DEBUG_BT,(_T("##CPanelBTHistory QueryCallHistory,count:%d\r\n"),dwCount));

	if(!pCursor || dwCount==0)
	{
		return;
	}


	while (pCursor)
	{
		AddRecord(pCursor);

		pCursor = pCursor->pNext;
	}
}

void CPanelBluetoothMain::AddRecord(CALLREC *pcr)
{
	if(!pcr)
		return;

	if(pcr->GetType()!=CALLREC::CRT_OUTGOING && pcr->GetType()!=CALLREC::CRT_INCOMING && pcr->GetType()!=CALLREC::CRT_MISSED)
		return;

	CListBoxItemGL* pItem;
	SYSTEMTIME t;
	wchar_t strTime[64];

	RETAILMSG(DEBUG_BT,(_T("####CPanelBTHistory AddRecord,type:%d,num:%s\r\n"),pcr->GetType(),pcr->GetPhoneNumber()));

	pcr->GetEndTime(t);
	swprintf_s(strTime,64,_T("%02d:%02d:%02d %d/%d/%d"),t.wHour,t.wMinute,t.wSecond,t.wDay,t.wMonth,t.wYear);

	pItem = new CListBoxItemGL(pcr->GetName(),pcr->GetPhoneNumber(),strTime);
	if(pcr->GetType()==CALLREC::CRT_OUTGOING)
		pItem->SetIcon(&m_iconOut);
	else if(pcr->GetType()==CALLREC::CRT_INCOMING)
		pItem->SetIcon(&m_iconIn);
	else if(pcr->GetType()==CALLREC::CRT_MISSED)
		pItem->SetIcon(&m_iconMissed);

	pItem->SetUserParam((LPARAM)pcr);

	m_boxHistory.AddItem(pItem);

}


void CPanelBluetoothMain::PhoneDial(const wchar_t *pstrDialNumber)
{
	if(!pstrDialNumber)
		return;

	RETAILMSG(DEBUG_BT,(_T("##MakeOutgoingCallPhoneNumber:%s\r\n"),pstrDialNumber));

	DWORD dwRet = g_BTModule.pApi->apiHF.MakeOutgoingCallPhoneNumber((wchar_t*)pstrDialNumber);

	if (dwRet != ERROR_SUCCESS)
	{
		RETAILMSG(DEBUG_BT,(_T("##MakeOutgoingCallPhoneNumber:%s FAILED!!\r\n"),pstrDialNumber));

	}

}
void CPanelBluetoothMain::OnDeviceSearchFinished()
{
	m_bDeviceSearching = FALSE;

	UpdateDeviceList();

	m_boxDeviceList.Show(m_type == BT_HOMEPAGE_DEVICE && !m_bDeviceSearching);
	m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));

}

void CPanelBluetoothMain::OnPairFailed()
{
// 	if(m_type == BT_HOMEPAGE_DEVICE && m_bShowSearchedDevice)
// 	{
// 		//
// 		m_boxDeviceSearched.SetCurFocusItemSubName(NULL);
// 	}

	m_bManualConnecting=FALSE;

	m_boxDeviceList.Enable(!m_bAutoConnecting && !m_bManualConnecting);

	//QueryCallHistory();
	UpdateDeviceList();

	m_pCurSellectedDevice = NULL;
}

void CPanelBluetoothMain::SearchDevice()
{
	if(!m_bDeviceSearching)
	{
		g_BTModule.pApi->apiSearchPair.SearchDevice();
		m_bDeviceSearching = TRUE;
	}
}
void CPanelBluetoothMain::CancelSearchDevice()
{
	if(m_bDeviceSearching)
	{
		g_BTModule.pApi->apiSearchPair.SearchDeviceCancel();
		m_bDeviceSearching = FALSE;
	}
}
void CPanelBluetoothMain::QuerySearchedDevice()
{

	CBTDevList *pbtdSearchedDeviceList;
	g_BTModule.pDataPool->GetData(CDataPool::SearchDeviceList, (const void **)&pbtdSearchedDeviceList);

	if (pbtdSearchedDeviceList == NULL)
	{
		BTDBG(ZONE_ERROR, (TEXT("<%s> GetData  ERROR \r\n"), _T(__FUNCTION__)));
		return;
	}

	BTDEV *pCursor = pbtdSearchedDeviceList->m_pbtdList;
	DWORD dwCount = pbtdSearchedDeviceList->m_dwCntList;

	RETAILMSG(DEBUG_BT,(_T("##CPanelBTSearch QuerySearchedDevice,count:%d\r\n"),dwCount));

	if(!pCursor || dwCount==0)
	{
		return;
	}

	CListBoxItemGL *pItem = NULL;
	BTSERVICE *pbts=NULL;

	while (pCursor)
	{
		RETAILMSG(DEBUG_BT,(_T("##CPanelBTSearch QuerySearchedDevice,%s\r\n"),pCursor->szDeviceName));

		pItem = new CListBoxItemGL(pCursor->szDeviceName);
		pItem->SetIcon(&m_iconDevice);
		pItem->SetUserParam((LPARAM)pCursor);
		m_boxDeviceList.AddItem(pItem,0);

		pCursor = pCursor->pNext;
	}

}
void CPanelBluetoothMain::PairDevice(BTDEV *pbtd)
{
	RETAILMSG(DEBUG_BT/*DEBUG_BT*/,(_T("CPanelBTSearch PairDevice:%x\r\n"),pbtd));

	if(!pbtd)
		return;

	DWORD dwRet =0;

	if(!pbtd->fActive)
	{
		RETAILMSG(DEBUG_BT/*DEBUG_BT*/,(_T("PairDevice Addr: %04x%08x \r\n"), GET_NAP(pbtd->b), GET_SAP(pbtd->b)));

		dwRet = g_BTModule.pApi->apiSearchPair.Pair(pbtd);
		if (dwRet != ERROR_SUCCESS)
		{
			return;
		}	
	}

}


