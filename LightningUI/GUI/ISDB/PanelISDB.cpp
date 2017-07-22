#include "PanelISDB.h"
#include "../../resource.h"

#if CVTE_EN_ISDB
CPanelISDB::CPanelISDB(void)
{

}

CPanelISDB::~CPanelISDB(void)
{
}

BOOL CPanelISDB::IsReady()
{
	return CPanelGL::IsReady();
}

void CPanelISDB::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);
}

void CPanelISDB::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

    	RETAILMSG(DEBUG_ISDB,(_T("[MSG][CPanelISDB::OnEnterPanel]: \r\n")));

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

void CPanelISDB::Render()
{
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
	static CTexture *pTexBar2=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_056):NULL;
	COLORGL cr_bg={0.38f,0.36f,0.36f,1.0f};
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	wchar_t caption[20];
	if(m_BeSelect>0 && m_BeSelect<=9){
		swprintf_s(caption,20,_T("Current Station:00%01d"), m_BeSelect);
		m_pTextGL.SetString(caption);
	}else if(m_BeSelect>10 && m_BeSelect<=99){
		swprintf_s(caption,20,_T("Current Station:0%02d"), m_BeSelect);
		m_pTextGL.SetString(caption);
	}else{
		swprintf_s(caption,20,_T("Current Station:%03d"), m_BeSelect);
		m_pTextGL.SetString(caption);
	}
	
	m_sliISDBSignal.SetValue(m_isdbSignal);
	
	DrawRect(pTexBar,0,0, g_iClientWidth-4,  g_iClientHeight-4, 2, &cr, TRUE);

	//上下状态栏
	DrawRect(pTexBar,0,(-g_iClientHeight/2+40), g_iClientWidth-4, 80, 2, &cr, TRUE);
	DrawRect(pTexBar,0,(g_iClientHeight/2-40), g_iClientWidth-4, 80, 2, &cr, TRUE);
	
	if(!BeginRender())return;

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	m_tagDAB.SetLuminance(m_fCurLuminance);
	m_tagDAB.SetAlpha(m_fCurAlpha);
	m_tagDAB.Render();

	m_boxMenu.SetAlpha(m_fCurAlpha);
	m_boxMenu.Render();

	m_pTextGL.SetAlpha(m_fCurAlpha);
	m_pTextGL.SetLuminance(m_fCurLuminance);
	m_pTextGL.Render();	

	m_sliISDBSignal.SetLuminance(m_fCurLuminance);
	m_sliISDBSignal.SetAlpha(m_fCurAlpha);
	m_sliISDBSignal.Render();
		
	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}
	
	EndRender();
}

BOOL CPanelISDB::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bShowStatusBar = TRUE;
	m_bEnableSmartBar = TRUE;
	m_BeSelect = ISDB_LIST_001;
	m_isdbSignal = 1;
	
	//home
	m_btnHome.Initialize(IDC_ISDB_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelISDB::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+40);
	m_btnHome.SetTouchRate(1.6f,1.6f);
	m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//DAB图片
	GLfloat texCoord_s01000[]={
		4.0/512.0, 172/512.0,
		4.0/512.0, 340.0/512.0,
		328.0/512.0, 172.0/512.0,
		328.0/512.0, 340.0/512.0};

	m_tagDAB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
	m_tagDAB.SetPos(-130, 30);

	const int cx=ISDB_LIST_BOX_ITEM_WIDTH;
	const int cy=ISDB_LIST_BOX_ITEM_HEIGHT;
	COLORGL cr_gray={180.0f/256.0f,180.0f/256.0f,180.0f/256.0f,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};

	//菜单列表
	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));
	box.siClient.cx=cx;
	box.siClient.cy=g_iClientHeight-2*cy;
	box.siBorder.cx=cx;
	box.siBorder.cy=g_iClientHeight-2*cy;
	box.iSpacing=(g_iClientHeight-2*cy)/8;

	box.idTexIndexBase=TEXID_603;
	box.idTexIndexFocus=TEXID_068;//TEXID_604;
	box.idTexBase=TEXID_603;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;//FALSE;
	box.bGlow=TRUE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	
	m_boxMenu.Initialize(IDC_ISDB_BOX_MAIN,this,&box,pSkinManager);
	m_boxMenu.SetNotifyFunc(&CPanelISDB::OnNotify,this);
	m_boxMenu.SetPos(g_iClientWidth/2-cx/2, 0);
	m_boxMenu.LinkTextGL(pTextGL);
	m_boxMenu.Show(TRUE,FALSE);
	m_boxMenu.SetAlpha(1.0f);
	m_boxMenu.SetCaptioniHeight(18);
	m_boxMenu.SetCaptionColor(&cr_gray,&cr_w);

	CListBoxItemGL *pItem;
	pItem=new CListBoxItemGL(_T(" Station Lists"));
	pItem->EnableClick(FALSE);
	m_boxMenu.AddItem(pItem, ISDB_LIST_NULL);

	pItem=new CListBoxItemGL(_T(" "));
	pItem->EnableClick(FALSE);
	m_boxMenu.AddItem(pItem, ISDB_LIST_NULL);

	pItem=new CListBoxItemGL(_T(" 001.GNTV_SD"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_001);

	pItem=new CListBoxItemGL(_T(" 002.GMA 1Seg"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_002);

	pItem=new CListBoxItemGL(_T(" 003.BEAM_SD1"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_003);

	pItem=new CListBoxItemGL(_T(" 004.CCTV-5"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_004);

	pItem=new CListBoxItemGL(_T(" 005.UNTV-5"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_005);

	pItem=new CListBoxItemGL(_T(" 006.SPORTS"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_006);

	pItem=new CListBoxItemGL(_T(" 007.ABS-CBN"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_007);

	pItem=new CListBoxItemGL(_T(" 008.CINEMOI"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_008);

	pItem=new CListBoxItemGL(_T(" 009.CCTV-1"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_009);

	pItem=new CListBoxItemGL(_T(" 010.CCTV-2"));
	m_boxMenu.AddItem(pItem, ISDB_LIST_010);

	m_boxMenu.MoveToIndex(2);

	//标题	
	COLORGL cr={1,1,1,1};
	m_pTextGL.SetColor(&cr,FALSE);
	m_pTextGL.SetPos(-g_iClientWidth/2+115,-55);
	m_pTextGL.SetCharScale(0.8f,0.8f);	
	
	//信号强度
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=32;
	sli.size.cy=32;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idThumbTex=0;
	sli.idBackTex=TEXID_024;
	sli.idChannelTex=TEXID_029;
	
	m_sliISDBSignal.Initialize(IDC_STATUSBAR_SLI_ISDBSIGNAL,this,&sli,pSkinManager);
	m_sliISDBSignal.ShowThumb(FALSE);
	m_sliISDBSignal.SetRange(0,5);
	m_sliISDBSignal.SetPos(-g_iClientWidth/2+50,  g_iClientHeight/2-50);
	m_sliISDBSignal.Show(TRUE);
	
	return TRUE;
}

void CALLBACK CPanelISDB::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_ISDB,(_T("[MSG]CPanelISDB::OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelISDB *panel=(CPanelISDB*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}

void CPanelISDB::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(DEBUG_ISDB,(_T("[MSG]CPanelISDB::HandleControlMSG id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));
	switch (idControl)
	{
		case IDC_ISDB_BTN_HOME:
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
			break;

		case IDC_ISDB_BOX_MAIN:
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg){
				case NOTIFY_DOUBLE_CLICK:{

				}
				break;
				
				case NOTIFY_SHORT_CLICK:
				{
					switch(id_item){
						case ISDB_LIST_001:
							m_BeSelect = ISDB_LIST_001;
							m_isdbSignal = 1;
							break;

						case ISDB_LIST_002:
							m_BeSelect = ISDB_LIST_002;
							m_isdbSignal = 2;
							break;

						case ISDB_LIST_003:
							m_BeSelect = ISDB_LIST_003;
							m_isdbSignal = 3;
							break;

						case ISDB_LIST_004:
							m_BeSelect = ISDB_LIST_004;
							m_isdbSignal = 4;
							break;

						case ISDB_LIST_005:
							m_BeSelect = ISDB_LIST_005;
							m_isdbSignal = 5;
							break;

						case ISDB_LIST_006:
							m_BeSelect = ISDB_LIST_006;
							m_isdbSignal = 1;
							break;

						case ISDB_LIST_007:
							m_BeSelect = ISDB_LIST_007;
							m_isdbSignal = 2;
							break;

						case ISDB_LIST_008:
							m_BeSelect = ISDB_LIST_008;
							m_isdbSignal = 3;
							break;

						case ISDB_LIST_009:
							m_BeSelect = ISDB_LIST_009;
							m_isdbSignal = 4;
							break;

						case ISDB_LIST_010:
							m_BeSelect = ISDB_LIST_010;
							m_isdbSignal = 5;
							break;

						default:
							break;
					}
					
					PlaySoundEffect(0);
				}
				break;

				default:
					break;
			}	
	}
}

#endif