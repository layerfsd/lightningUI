#include "PanelUpdateARM.h"
#include "../../resource.h"
#include "CVTE_IoCtl.h"
#include "../../Include/CVTE_AppProxy.h"
#include "../Include/oemsector.h"
#if CVTE_EN_DAB
#include "../../DAB/Inc/DABRadioLib.h"
#include "../../DAB/Adapter/DABAdapter.h"
#endif
#include<new>
using namespace std;

#define CVTE_UPDATE_FILE_OEM_CFG_NAME		_T("oem.cfg")
#define CVTE_UPDATE_FILE_EBOOT_NAME		_T("EBOOT.nb0")
#define CVTE_UPDATE_FILE_LOGO_NAME		_T("startup.raw")
#define CVTE_UPDATE_FILE_IMAGE_NAME		_T("manifest.bin")
#define CVTE_UPDATE_FILE_RES_NAME		_T("res.bin")

#if CVTE_EN_PACKAGE_UPDATE
#define CVTE_PROGRESS_RANGE	65
#else
#define CVTE_PROGRESS_RANGE	190
#endif

typedef struct tagRESHEADER
{
	char tag[4];
	DWORD dwFileCount;
	DWORD dwTotalSize;

}RESHEADER,*LPRESHEADER;

//===========================================================================================================================
//	CRC32
//===========================================================================================================================

// Note: This uses the same polynomial as zlib: x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1.

static const DWORD	kCRC32Table[ 256 ] =
{
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4,
	0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a,
	0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f,
	0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
	0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5,
	0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6,
	0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c,
	0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31,
	0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7,
	0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8,
	0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

DWORD	CRC32( DWORD inCRC, const void *inData, unsigned int inSize )
{
	const unsigned char *		src;
	const unsigned char *		end;

	inCRC ^= 0xFFFFFFFFU;
	src = (const unsigned char *) inData;
	end = src + inSize;
	while( src != end )
	{
		inCRC = kCRC32Table[ ( inCRC ^ ( *src++ ) ) & 0xFF ] ^ ( inCRC >> 8 );
	}
	inCRC ^= 0xFFFFFFFFU;
	return( inCRC );
}

CPanelUpdateARM::CPanelUpdateARM(void)
{
	m_ptagLogo = NULL;
	m_nUpdateType = UPDATE_TYPE_NULL;
	m_nUpdateStatus = UPDATE_STATUS_IDLE;
	m_nUpdateError = UPDATE_ERROR_NOERROR;
}

CPanelUpdateARM::~CPanelUpdateARM(void)
{
}

BOOL CPanelUpdateARM::IsReady()
{
	return (CPanelGL::IsReady() && (GetUpdateStatus()==UPDATE_STATUS_START));
}

void CPanelUpdateARM::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelUpdateARM::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
	RETAILMSG(1,(_T("--->>CPanelUpdateARM OnEnterPanel :%d,%d,%d\n"),iParam,idAni,lpAniPara));

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

//	m_bEnableQuit = FALSE;
//	    if(!m_bUpdating)
//	    {
//	        StartUpdateAll();
//	    }
	//
	OnDeviceChanged();
}

void CPanelUpdateARM::Render()
{
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;

	if(!BeginRender())return;

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

	//device status
	SIZE si;
	wchar_t temp[16];

	m_tagBkBlur.SetAlpha(m_fCurAlpha);
	m_tagBkBlur.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
	m_tagBkBlur.Render();

//		if(m_ptagLogo)
//		{
//			m_ptagLogo->SetAlpha(m_fCurAlpha);
//			m_ptagLogo->Render();
//		}
//		else
//		{
//			m_iconImage.SetAlpha(m_fCurAlpha);
//			m_iconImage.Render();
//		}

	if(GetTickCount() - m_dwUpdateStart < 5000)
	{
		m_tagMsg.SetAlpha(m_fCurAlpha);
		m_tagMsg.Render();
	}



	if(GetUpdateStatus() == UPDATE_STATUS_IDLE)
	{
		m_btnReturn.SetAlpha(m_fCurAlpha);
		m_btnReturn.Render();

		m_btnUpdateAll.SetAlpha(m_fCurAlpha);
		m_btnUpdateAll.Render();

		m_btnUpdateLogo.SetAlpha(m_fCurAlpha);
		m_btnUpdateLogo.Render();

#if CVTE_EN_OS_DYNAMIC_LOGO
		m_btnUpdateDynamicLogo.SetAlpha(m_fCurAlpha);
		m_btnUpdateDynamicLogo.Render();
#endif

	}
	else
	{
		//RETAILMSG(1,(_T("MSG: m_bUpdating:%d m_nUpdateType:%d\n"),m_bUpdating,m_nUpdateType));
		m_dwUpdateStart = GetTickCount();
		switch(m_nUpdateType)
		{
			case UPDATE_TYPE_UI:
				{
					VIEW_STATE vs = m_iconLoading.GetCurViewState();
					vs.fRotateZ -= 6;
					if(vs.fRotateZ < -360)
						vs.fRotateZ = 0;
					m_iconLoading.SetCurViewState(&vs);
					m_iconLoading.SetAlpha(m_fCurAlpha);
					m_iconLoading.Render();
				}
				break;
			case UPDATE_TYPE_LOGO:
			case UPDATE_TYPE_RES:
				{
					if(m_pTextGL)
					{
						m_pTextGL->SetAlpha(m_fCurAlpha);
						m_pTextGL->SetLuminance(m_fCurLuminance);

						if(m_strTitle.String())
						{
							m_pTextGL->SetColor(&cr);
							m_pTextGL->SetString(/*m_strTitle.String()*/TEXT("Update logo"));
							m_pTextGL->SetCharScale(1.2f,1.2f);
							m_pTextGL->GetExtension(&si);
							m_pTextGL->SetPos(GLfloat(-si.cx/2),160);
							m_pTextGL->Render();
						}

						if(m_strNotification.String())
						{
							m_pTextGL->SetString(m_strNotification.String());
							m_pTextGL->SetCharScale(1.0f,1.0f);
							m_pTextGL->GetExtension(&si);
							// 		m_pTextGL->SetPos(-310,m_strTitle.String()?-20:26);
							m_pTextGL->SetPos(GLfloat(-si.cx/2),80);
							// 		m_pTextGL->EnableAutoChangeLine(TRUE,620);
							m_pTextGL->Render();
						}

					}
					VIEW_STATE vs = m_iconLoading.GetCurViewState();
					vs.fRotateZ -= 6;
					if(vs.fRotateZ < -360)
						vs.fRotateZ = 0;
					m_iconLoading.SetCurViewState(&vs);
					m_iconLoading.SetAlpha(m_fCurAlpha);
					m_iconLoading.Render();

				}
				break;
			case UPDATE_TYPE_MCU:
				{

				}
				break;
			case UPDATE_TYPE_ALL:
			case UPDATE_TYPE_DAB:
				{
					if(m_pTextGL)
					{
						m_pTextGL->SetAlpha(m_fCurAlpha);
						m_pTextGL->SetLuminance(m_fCurLuminance);

						if(m_strTitle.String())
						{
							m_pTextGL->SetColor(&cr);
							m_pTextGL->SetString(m_strTitle.String());
							m_pTextGL->SetCharScale(1.2f,1.2f);
							m_pTextGL->GetExtension(&si);
							m_pTextGL->SetPos(GLfloat(-si.cx/2),160);
							m_pTextGL->Render();
						}

						if(m_strNotification.String())
						{
							m_pTextGL->SetString(m_strNotification.String());
							m_pTextGL->SetCharScale(1.0f,1.0f);
							m_pTextGL->GetExtension(&si);
					// 		m_pTextGL->SetPos(-310,m_strTitle.String()?-20:26);
							m_pTextGL->SetPos(GLfloat(-si.cx/2),80);
					// 		m_pTextGL->EnableAutoChangeLine(TRUE,620);
							m_pTextGL->Render();
						}

						//draw percent:
						swprintf_s(temp,16,_T("%d%%"),m_sliProgress.GetValue()*100/CVTE_PROGRESS_RANGE);
						//RETAILMSG(1, (TEXT("%s\r\n"),temp));
						m_pTextGL->SetString(temp);
						m_pTextGL->SetCharScale(1.0f,1.0f);
						m_pTextGL->GetExtension(&si);
						m_pTextGL->SetPos(GLfloat(-si.cx/2),-30);
						m_pTextGL->Render();
					}

					m_sliProgress.SetAlpha(m_fCurAlpha);
					m_sliProgress.Render();
				}
				break;
			default:
				break;
		}
	}

	EndRender();

}
BOOL CPanelUpdateARM::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//m_bEnableQuit = FALSE;
	VIEW_STATE vs;
	SIZE siBk;

	const int margin_x=12;
    const int box_size_x=LIST_BOX_ITEM_WIDTH;
    const int tag_h = LIST_BOX_TAG_HEIGHT;
    const int caption_h = 22;
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};

	GLfloat py=g_iClientHeight/2-120;

	SetUpdateStatus(UPDATE_STATUS_IDLE);

	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2000)));
	siBk=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/siBk.cx,GLfloat(g_iClientHeight)/siBk.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	m_btnReturn.Initialize(IDC_NB_BTN_RETURN,this,m_pSkinManager->GetTexture(TEXID_012),NULL,
		m_pSkinManager->GetTexture(TEXID_012),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnReturn.SetNotifyFunc(&CPanelUpdateARM::OnNotify,this);
	//m_btnHome.SetSize(80,80);
	//m_btnReturn.SetShowScale(0.6f);
	m_btnReturn.SetPos(g_iClientWidth/2-40,g_iClientHeight/2-40);
	m_btnReturn.EnableClickAni(TRUE);

	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(0.6f);
	m_iconLoading.SetPos(0,-180);


	WCHAR name[MAX_STRING];
	swprintf_s(name,MAX_STRING,_T("%s All"),GetResourceString(IDS_UPDATE));
	m_btnUpdateAll.Initialize(IDC_SETTING_BTN_UPDATE_ALL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnUpdateAll.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnUpdateAll.SetCaption(name);
	m_btnUpdateAll.LinkTextGL(pTextGL);
	m_btnUpdateAll.SetPos(0,py);
	m_btnUpdateAll.SetCaptionHeight(caption_h);
	m_btnUpdateAll.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnUpdateAll.SetCaptionColor(&cr_gray);
	m_btnUpdateAll.SetNotifyFunc(&CPanelUpdateARM::OnNotify,this);
	m_btnUpdateAll.Enable(IsFileExists(UPDATE_FILE_IMAGE_USBDISK) && IsFileExists(UPDATE_FILE_UI_USBDISK) && IsFileExists(UPDATE_FILE_LOGO_BMP_USBDISK));

	py-=tag_h+10;

	swprintf_s(name,MAX_STRING,_T("%s Logo"),GetResourceString(IDS_UPDATE));
	m_btnUpdateLogo.Initialize(IDC_SETTING_BTN_UPDATE_LOGO,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnUpdateLogo.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnUpdateLogo.SetCaption(name);
	m_btnUpdateLogo.LinkTextGL(pTextGL);
	m_btnUpdateLogo.SetPos(0,py);
	m_btnUpdateLogo.SetCaptionHeight(caption_h);
	m_btnUpdateLogo.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnUpdateLogo.SetCaptionColor(&cr_gray);
	m_btnUpdateLogo.SetNotifyFunc(&CPanelUpdateARM::OnNotify,this);
	m_btnUpdateLogo.Enable(FALSE);

#if CVTE_EN_OS_DYNAMIC_LOGO
	py-=tag_h+10;

	swprintf_s(name,MAX_STRING,_T("%s Boot Animation"),GetResourceString(IDS_UPDATE));
	m_btnUpdateDynamicLogo.Initialize(IDC_SETTING_BTN_UPDATE_RES,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnUpdateDynamicLogo.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnUpdateDynamicLogo.SetCaption(name);
	m_btnUpdateDynamicLogo.LinkTextGL(pTextGL);
	m_btnUpdateDynamicLogo.SetPos(0,py);
	m_btnUpdateDynamicLogo.SetCaptionHeight(caption_h);
	m_btnUpdateDynamicLogo.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnUpdateDynamicLogo.SetCaptionColor(&cr_gray);
	m_btnUpdateDynamicLogo.SetNotifyFunc(&CPanelUpdateARM::OnNotify,this);
	m_btnUpdateDynamicLogo.Enable(IsFileExists(UPDATE_FILE_RES_USBDISK));
#endif

	//slider
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=g_iClientWidth-280;//520/*776*/;
	sli.size.cy=32;
	sli.bHasBtns=FALSE;
	sli.bFilled=TRUE;
	sli.bFilledBoth=FALSE;

	m_sliProgress.Initialize(0,NULL,&sli,NULL);
	m_sliProgress.SetPos(0.0f,-130.0f);
	m_sliProgress.SetRange(0,CVTE_PROGRESS_RANGE);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoBackFill(TRUE);
	m_sliProgress.ShowThumb(FALSE);

    m_hUPD = UPDProxyOpen();
    if(m_hUPD == NULL)
    {
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateFile [UPD1] failed. err=%d\r\n"), TEXT(__FUNCTION__), GetLastError()));
		return FALSE;

    }

    m_hOPD = AppProxyOpen();
    if(m_hOPD == NULL)
    {
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateFile [OPD1] failed. err=%d\r\n"), TEXT(__FUNCTION__), GetLastError()));
		return FALSE;

    }

	if(!AppProxyGetWholeOem(m_hOPD, &m_oemsector, sizeof(m_oemsector)))
	{
		RETAILMSG(1, (TEXT("ERR[%s] Get whole oemsector failed. err=%d\r\n"), TEXT(__FUNCTION__), GetLastError()));
		return FALSE;
	}

	RETAILMSG(1, (TEXT("MSG:[%s]: [UPD1=0x%X] [OPD1=0x%X]\r\n"), TEXT(__FUNCTION__), m_hUPD, m_hOPD));

	return TRUE;
}


void CPanelUpdateARM::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelUpdateARM::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelUpdateARM::OnTouchMove(POINT *pt)
{

}

void CALLBACK CPanelUpdateARM::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("CPanelUpdateARM OnNotify id:%d,msg:%d,para:%d\n"),idControl,wMsg,wPara));

	CPanelUpdateARM *panel=(CPanelUpdateARM*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}

void CPanelUpdateARM::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("CPanelUpdateARM::HandleControlMSG id:%d,msg:%d,para:%d\n"),idControl,wMsg,wPara));
	switch (idControl)
	{
	case IDC_NB_BTN_CANCEL://cancel
		SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_NONE,PAGE_SETTINGS_ABOUT);
		break;
	case IDC_NB_BTN_OK://ok
		break;
	case IDC_NB_BTN_HOME:
		//::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,UI_MAINMENU,NULL);
		break;
	case IDC_NB_BTN_RETURN:
		OnBack();
		break;
	case IDC_SETTING_BTN_UPDATE_UI:
		if(GetUpdateStatus() == UPDATE_STATUS_IDLE)
		{
			WCHAR name[MAX_STRING];
			swprintf_s(name,MAX_STRING,_T("%s UI ?"),GetResourceString(IDS_IMAGE_UPDATE_CONFIRM));

			//show confirm box:
			ShowNotificationBox(m_hMainWnd,IDC_SETTING_BOX_UPDATE_UI_CONFIRM,
				&CPanelUpdateARM::OnNotify,
				this,
				0,
				name,
				NULL,//GetResourceString(IDS_MCU_UPDATE_PROMPT),
				0,
				FALSE);
		}
		break;
	case IDC_SETTING_BOX_UPDATE_UI_CONFIRM:
		{
			if(wMsg == NB_OK)//
			{
				StartUpdate(UPDATE_TYPE_UI);
			}
			HideNotificationBox(m_hMainWnd);
		}
		break;
	case IDC_SETTING_BTN_UPDATE_LOGO:
		if(GetUpdateStatus() == UPDATE_STATUS_IDLE)
		{
			WCHAR name[MAX_STRING];
			swprintf_s(name,MAX_STRING,_T("%s Logo ?"),GetResourceString(IDS_IMAGE_UPDATE_CONFIRM));

			//show confirm box:
			ShowNotificationBox(m_hMainWnd,IDC_SETTING_BOX_UPDATE_LOGO_CONFIRM,
				&CPanelUpdateARM::OnNotify,
				this,
				0,
				name,
				NULL,//GetResourceString(IDS_MCU_UPDATE_PROMPT),
				0,
				FALSE);
		}
		break;
	case IDC_SETTING_BOX_UPDATE_LOGO_CONFIRM:
		{
			if(wMsg == NB_OK)//
			{
				StartUpdate(UPDATE_TYPE_LOGO);
			}
			HideNotificationBox(m_hMainWnd);
		}
		break;
#if CVTE_EN_OS_DYNAMIC_LOGO
	case IDC_SETTING_BTN_UPDATE_RES:
		if(GetUpdateStatus() == UPDATE_STATUS_IDLE)
		{
			WCHAR name[MAX_STRING];
			swprintf_s(name,MAX_STRING,_T("%s Boot Animation ?"),GetResourceString(IDS_IMAGE_UPDATE_CONFIRM));

			//show confirm box:
			ShowNotificationBox(m_hMainWnd,IDC_SETTING_BTN_UPDATE_RES_CONFIRM,
				&CPanelUpdateARM::OnNotify,
				this,
				0,
				name,
				NULL,//GetResourceString(IDS_MCU_UPDATE_PROMPT),
				0,
				FALSE);
		}
		break;
	case IDC_SETTING_BTN_UPDATE_RES_CONFIRM:
		{
			if(wMsg == NB_OK)//
			{
				StartUpdate(UPDATE_TYPE_RES);
			}
			HideNotificationBox(m_hMainWnd);
		}
		break;
#endif
	case IDC_SETTING_BTN_UPDATE_ALL:
		if(GetUpdateStatus() == UPDATE_STATUS_IDLE)
		{
			WCHAR name[MAX_STRING];
			swprintf_s(name,MAX_STRING,_T("%s All ?"),GetResourceString(IDS_IMAGE_UPDATE_CONFIRM));

			//show confirm box:
			ShowNotificationBox(m_hMainWnd,IDC_SETTING_BOX_UPDATE_ALL_CONFIRM,
				&CPanelUpdateARM::OnNotify,
				this,
				0,
				name,
				NULL,//GetResourceString(IDS_MCU_UPDATE_PROMPT),
				0,
				FALSE);
		}
		break;
	case IDC_SETTING_BOX_UPDATE_ALL_CONFIRM:
		{
			if(wMsg == NB_OK)//
			{
				//bluetooth:
				//delete config file:
				RETAILMSG(1, (TEXT("Delete BT config.flc and ESIBT.ddb\r\n")));
				DeleteFile(_T("\\ResidentFlash\\dll\\config\\config.flc"));
				DeleteFile(_T("\\ResidentFlash\\dll\\ESIBT.ddb"));

				StartUpdate(UPDATE_TYPE_ALL);
			}
			HideNotificationBox(m_hMainWnd);

		}
		break;
	default:
		break;
	}
}
void CPanelUpdateARM::OnBack()
{
	SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
}

void CPanelUpdateARM::StartUpdate(BYTE nUpdateType)
{
    SetUpdateType(nUpdateType);

	switch(m_nUpdateType)
	{
		case UPDATE_TYPE_LOGO:
		case UPDATE_TYPE_RES:
			{
				HANDLE hUpdateThread = CreateThread(NULL,NULL,CPanelUpdateARM::UpdateThreadPro,this,NULL,NULL);
				if(hUpdateThread)
				{
					CeSetThreadPriority(hUpdateThread, CE_THREAD_PRIO_256_ABOVE_NORMAL);
					CloseHandle(hUpdateThread);
				}
			}
			break;
		case UPDATE_TYPE_ALL:
			{
				m_btnReturn.Show(FALSE);
				m_btnUpdateUI.Show(FALSE);
				m_btnUpdateLogo.Show(FALSE);
				//m_btnUpdateMCU.Show(FALSE);
				m_btnUpdateAll.Show(FALSE);
				#if CVTE_EN_DAB
				m_btnUpdateDAB.Show(FALSE);
 				#endif
				HANDLE hUpdateThread = CreateThread(NULL,NULL,CPanelUpdateARM::UpdateThreadPro,this,NULL,NULL);
			    HANDLE hUpdateProcessThread = CreateThread(NULL,NULL,CPanelUpdateARM::ProgressThreadPro,this,NULL,NULL);

				if(hUpdateThread)
				{
					//CeSetThreadPriority(hUpdateThread, CE_THREAD_PRIO_256_ABOVE_NORMAL);
					CloseHandle(hUpdateThread);
				}

				if(hUpdateProcessThread)
				{
					CeSetThreadPriority(hUpdateThread, CE_THREAD_PRIO_256_ABOVE_NORMAL);
					CloseHandle(hUpdateProcessThread);
				}
			}
			break;
		case UPDATE_TYPE_UI:
			{
				HANDLE hUpdateThread = CreateThread(NULL,NULL,CPanelUpdateARM::UpdateThreadPro,this,NULL,NULL);
				if(hUpdateThread)
				{
					CeSetThreadPriority(hUpdateThread, CE_THREAD_PRIO_256_ABOVE_NORMAL);
					CloseHandle(hUpdateThread);
				}
			}
			break;
		default:
			break;
	}
}

DWORD CPanelUpdateARM::UpdateThreadPro(LPVOID lpParam)
{
	CPanelUpdateARM *p= (CPanelUpdateARM*)lpParam;


	if(p == NULL)
	{
        RETAILMSG(1,(_T("ERR:CPanelUpdateARM::UpdateThreadPro ERR!!!\r\n")));
        return -1;
	}

	p->m_bEnableQuit = FALSE;
	p->m_btnReturn.Enable(FALSE);
	p->m_tagMsg.Show(FALSE);

    p->SetUpdateStatus(UPDATE_STATUS_START);

	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateThreadPro] ++ m_nUpdateType[%d]\r\n"),p->m_nUpdateType));

	switch(p->m_nUpdateType)
	{
    case UPDATE_TYPE_ALL:
		p->UpdateProc();
        break;
    case UPDATE_TYPE_LOGO:
        p->UpdateLogoProc();
        break;
	case UPDATE_TYPE_RES:
		p->UpdateResProc();
    default:
        break;
	}
	p->m_bEnableQuit = TRUE;
	p->m_btnReturn.Enable(TRUE);
	p->m_btnUpdateUI.Show(TRUE);
	p->m_btnUpdateLogo.Show(TRUE);
	//m_btnUpdateMCU.Show(TRUE);
	p->m_btnUpdateAll.Show(TRUE);
	#if CVTE_EN_DAB
    p->m_btnUpdateDAB.Show(TRUE);
	#endif
	Sleep(1000);
    p->SetUpdateStatus(UPDATE_STATUS_IDLE);
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateThreadPro] --\r\n")));
	return 0;
}

DWORD CPanelUpdateARM::ProgressThreadPro(LPVOID lpParam)
{
	CPanelUpdateARM *p= (CPanelUpdateARM*)lpParam;
    int i = 0;

	Sleep(1200);

	RETAILMSG(1, (TEXT("MSG:[%s]: m_nUpdateType:%d m_nUpdateStatus:%d\r\n"),TEXT(__FUNCTION__),p->GetUpdateType(), p->GetUpdateStatus()));
    while(i++<CVTE_PROGRESS_RANGE && (p->GetUpdateError() == UPDATE_ERROR_NOERROR))
    {
        Sleep(200);

        if(p->GetUpdateStatus() == UPDATE_STATUS_START)
		{
			//RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"),TEXT(__FUNCTION__),i));
            p->m_sliProgress.SetValue(i);
		}
        else if(p->GetUpdateStatus() == UPDATE_STATUS_SUCCESS)
        {
            p->m_sliProgress.SetValue(CVTE_PROGRESS_RANGE);
            break;
        }
		else
		{
			break;
		}
    }

	return 0;
}

void CPanelUpdateARM::UpdateResProc()
{
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateResProc] ++ \r\n")));

	//
	// Check file header
	//
	if(!CheckResHeader(UPDATE_FILE_RES_USBDISK))
	{
		SetUpdateError(UPDATE_ERROR_HEADER);
		SetUpdateStatus(UPDATE_STATUS_FAIL);
		RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:[CPanelUpdateARM::UpdateResProc] -- res.bin header check failed.\r\n")));
		//Sleep(10000);
		//reset:
		//::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
		return;
	}
	
	InstallRes(UPDATE_FILE_RES_USBDISK);

	SetUpdateStatus(UPDATE_STATUS_SUCCESS);

_EXIT:
	//reset:
	::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);

	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateResProc] -- \r\n")));

	return;
}

void CPanelUpdateARM::UpdateLogoProc()
{
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateLogoProc] ++ \r\n")));

//		if(m_bFileExitLogoBmp && !m_bFileExitLogoRaw)
	{
		FILE	*file = NULL;
		CM_String strPath=CM_String(_T("\\USB Disk\\Temp\\"));
		CreateDirectory(strPath.String(),NULL);
		CM_String strFile=CM_String(_T("\\USB Disk\\Temp\\startup.raw"));

		m_Dib.Load(UPDATE_FILE_LOGO_BMP_USBDISK);
		m_dwDibWidth = m_Dib.GetWidth();
		m_dwDibHight = m_Dib.GetHeight();
		m_dwDibAbsHight = labs(m_dwDibHight);

		if(m_Dib.GetHeight()==-m_dwDibAbsHight)
		{
			if(m_Dib.GetBitCount()==16)
			{
				printf("暂时不支持 16 Bits-〉32 bits");
			}
			if(m_Dib.GetBitCount()==24)
			{
				//要修改
				m_Dib.Convert24To32();
				m_Dib.Save_RawFile(strFile.String(),m_Dib.GetNewImgData(),m_Dib.GetNewImgDataSize());
			}
			if(m_Dib.GetBitCount()==32)
			{
				m_Dib.Save_RawFile(strFile.String(),m_Dib.GetImgData(),m_Dib.GetImgDataSize());
			}
			//MessageBox(_T(""));
		}
		else
		{
			//要换方向
			if(m_Dib.GetBitCount()==16)
			{
				printf("暂时不支持 16 Bits-〉32 bits");
			}
			if(m_Dib.GetBitCount()==24)
			{
				//要修改
				m_Dib.Convert24InverseTo32();
				m_Dib.Save_RawFile(strFile.String(),m_Dib.GetNewImgData(),m_Dib.GetNewImgDataSize());
			}
			if(m_Dib.GetBitCount()==32)
			{
				m_Dib.ConvertDirection32();
				m_Dib.Save_RawFile(strFile.String(),m_Dib.GetNewImgData(),m_Dib.GetNewImgDataSize());
			}
		}

		if(m_Dib.GetWidth() != g_iClientWidth || m_Dib.GetHeight() != g_iClientHeight)
		{
			RETAILMSG(1, (TEXT("ERR:[%s]: Logo size is incorrect.\r\n"), TEXT(__FUNCTION__)));
			SetUpdateStatus(UPDATE_STATUS_FAIL);
			WCHAR msg[MAX_STRING];
			swprintf_s(msg,MAX_STRING,_T("Update failed. The logo[%d x %d] size is incorrect."),m_Dib.GetWidth(),m_Dib.GetHeight());
			m_tagMsg.SetCaption(msg);
			m_tagMsg.Show(TRUE);
			goto _EXIT;
		}

	    if(!DownloadFile(_T("\\USB Disk\\Temp\\startup.raw"),IOCTL_UPD_LOGO))
	    {
	        RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:CPanelUpdateARM::UpdateProc DownloadFile %s failed.\r\n"),UPDATE_FILE_LOGO_BMP_USBDISK));
	        SetUpdateStatus(UPDATE_STATUS_FAIL);
	        goto _EXIT;
	    }
	}

	if(m_bFileExitLogoBmp)
	{
		if(CopyFile(UPDATE_FILE_LOGO_JPG_USBDISK,_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo.jpg"),FALSE))
		{
	        RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:CPanelUpdateARM::UpdateLogoProc Update Car logo [logo.jpg]\r\n")));
			m_pSkinManager->GetConfig()->GetCurParam()->strLogoName = _T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo.jpg");
		}
		if(IsFileExists(UPDATE_FILE_LOGO2_JPG_USBDISK))
		{
			if(CopyFile(UPDATE_FILE_LOGO2_JPG_USBDISK,_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo2.jpg"),FALSE))
			{
		        RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:CPanelUpdateARM::UpdateLogoProc Update Car logo [logo2.jpg]\r\n")));
				m_pSkinManager->GetConfig()->GetCurParam()->strLogoName2 = _T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo2.jpg");
			}
		}
		else
		{
			if(CopyFile(UPDATE_FILE_LOGO_JPG_USBDISK,_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo2.jpg"),FALSE))
			{
		        RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:CPanelUpdateARM::UpdateLogoProc Update Car logo [logo2.jpg]\r\n")));
				m_pSkinManager->GetConfig()->GetCurParam()->strLogoName2 = _T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo2.jpg");
			}
		}
		m_pSkinManager->GetConfig()->SaveConfig();
	}
	else
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Logo bmp do not exist.\r\n"), TEXT(__FUNCTION__)));
		SetUpdateStatus(UPDATE_STATUS_FAIL);
		WCHAR msg[MAX_STRING];
		swprintf_s(msg,MAX_STRING,_T("Update failed.  Logo.bmp do not exist.."));
		m_tagMsg.SetCaption(msg);
		m_tagMsg.Show(TRUE);
		goto _EXIT;
	}
//		else if(m_bFileExitLogoRaw)
//		{
//		    if(!DownloadFile(UPDATE_FILE_LOGO_RAW_USBDISK,IOCTL_UPD_LOGO))
//		    {
//		        RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:CPanelUpdateARM::UpdateProc DownloadFile %s failed.\r\n"),UPDATE_FILE_LOGO_BMP_USBDISK));
//		        m_bUpdating = FALSE;
//
//		        m_strTitle = GetResourceString(IDS_MCU_UPDATE_FAILED);
//		        goto _EXIT;
//		    }
//		}

//	m_bUpdating = FALSE;
	SetUpdateStatus(UPDATE_STATUS_SUCCESS);

_EXIT:
	m_Dib.Empty();
	if(IsFileExists(_T("\\USB Disk\\Temp\\startup.raw")))
	{
		DeleteFile(_T("\\USB Disk\\Temp\\startup.raw"));
	}
	if(IsFileExists(_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo.tif")))
	{
	    RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:CPanelUpdateARM::UpdateProc Delete logo.tif\r\n")));
		DeleteFile(_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo.tif"));
	}
	if(IsFileExists(_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo2.tif")))
	{
	    RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:CPanelUpdateARM::UpdateProc Delete logo2.tif\r\n")));
		DeleteFile(_T("\\ResidentFlash\\LightningUI\\Car logo\\IMAGES\\logo2.tif"));
	}
//	    Sleep(3000);

	//reset:
	::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);

	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateLogoProc] -- \r\n")));

	return;
}

void CPanelUpdateARM::UpdateUIProc()
{
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateUIProc] -- \r\n")));

	if(m_hOPD)
	{
		BOOL bRet;
		m_oemsector.UpdateType = OEMCONFIG_UPD_TYPE_STORE;
		m_oemsector.UpdateUI = OEMCONFIG_FLAG_ENABLE;
		bRet = DeviceIoControl(m_hOPD, OEMCONFIG_IOCTL_SET_WHOLE_OEMSECTOR, &m_oemsector, sizeof(m_oemsector), NULL, 0, NULL, NULL);
		//m_bUpdating = FALSE;

		if(bRet)
		{
		    Sleep(1000);
		    //reset:
			::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
		}
	}

	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateUIProc] -- \r\n")));

}

#if CVTE_EN_PACKAGE_UPDATE
void CPanelUpdateARM::UpdateProc()
{
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateProc] ++ \r\n")));

	//
	// Check file header
	//
	if(!CheckPackageHeader(UPDATE_FILE_PACKAGE_USBDISK))
	{
		SetUpdateError(UPDATE_ERROR_HEADER);
		SetUpdateStatus(UPDATE_STATUS_FAIL);
		RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:[CPanelUpdateARM::UpdateProc] -- update package header check failed.\r\n")));
		Sleep(10000);
		//reset:
		::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
		return;
	}

	Sleep(1000);

	if(m_bFileExitRes)
	{
		if(!CheckResHeader(UPDATE_FILE_RES_USBDISK))
		{
			SetUpdateError(UPDATE_ERROR_HEADER);
			SetUpdateStatus(UPDATE_STATUS_FAIL);
			RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:[CPanelUpdateARM::UpdateResProc] -- res.bin header check failed.\r\n")));
			//Sleep(10000);
			//reset:
			//::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
			return;
		}

#if CVTE_EN_OS_DYNAMIC_LOGO
		if(!InstallRes(UPDATE_FILE_RES_USBDISK))
		{
			SetUpdateStatus(UPDATE_STATUS_FAIL);
			Sleep(10000);
			//reset:
			::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
			return;
		}
#endif
	}

	if(InstallPackage(UPDATE_FILE_PACKAGE_USBDISK))
	{
	    SetUpdateStatus(UPDATE_STATUS_SUCCESS);
	    Sleep(3000);
	    //reset:
		::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
	}
	else
	{
		SetUpdateStatus(UPDATE_STATUS_FAIL);
		Sleep(10000);
	    //reset:
		::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
	}

}

BOOL CPanelUpdateARM::CheckPackageHeader(LPCTSTR lpFileName)
{
	FILE *file;
	INSTALLHEADER				sh;
	memset(&sh,0,sizeof(sh));

	if ( 0!=_wfopen_s(&file, lpFileName,_T("rb")) )
	{
		RETAILMSG(1,(_T("file not found.\r\n")));
		return false;
	}

	// read file header
	fread(&sh, 1, sizeof(sh), file);
	fclose(file);
	return sh.tag[0]=='I'	&& sh.tag[1]=='N' && sh.tag[2]=='S' /*&& sh.iCount==88*/;
}

BOOL CPanelUpdateARM::CheckResHeader(LPCTSTR lpFileName)
{
	FILE *file;
	INSTALLHEADER				sh;
	memset(&sh,0,sizeof(sh));

	if ( 0!=_wfopen_s(&file, lpFileName,_T("rb")) )
	{
		RETAILMSG(1,(_T("file not found.\r\n")));
		return false;
	}

	// read file header
	fread(&sh, 1, sizeof(sh), file);
	fclose(file);
	return sh.tag[0]=='R'	&& sh.tag[1]=='E' && sh.tag[2]=='S' /*&& sh.iCount==88*/;
}

BOOL CPanelUpdateARM::IsUpdateFile(LPCTSTR lpFileName)
{
	if(lstrcmpi(lpFileName,CVTE_UPDATE_FILE_OEM_CFG_NAME) == 0)
	{
		return TRUE;
	}
	else if(lstrcmpi(lpFileName,CVTE_UPDATE_FILE_EBOOT_NAME) == 0)
	{
		return TRUE;
	}
	else if(lstrcmpi(lpFileName,CVTE_UPDATE_FILE_LOGO_NAME) == 0)
	{
		return TRUE;
	}
	else if(lstrcmpi(lpFileName,CVTE_UPDATE_FILE_IMAGE_NAME) == 0)
	{
		return TRUE;
	}
	else if(lstrcmpi(lpFileName,CVTE_UPDATE_FILE_RES_NAME) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

BOOL CPanelUpdateARM::InstallPackage(LPCTSTR lpszFileName)
{
	BOOL bRet = FALSE;
	BOOL bResult = FALSE;
	FILE *file;
	DWORD DataReadLen = 0;
	size_t sizeRead = 0;
	INSTALLHEADER sh;
	int iFileCount;
	int iFileNameLen;
	int iFileDataLen;
	WCHAR strFileName[256];
	unsigned char * m_pdataBuffer = NULL;

	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::InstallPackage]: ++\r\n")));
	do
	{
		//
		// Open file
		//
		if ( 0!=_wfopen_s(&file, lpszFileName,_T("rb")) )
		{
			RETAILMSG(1,(_T("ERR:[%s]: open:%s failed, err:%d \r\n"),TEXT(__FUNCTION__), lpszFileName, GetLastError()));
			SetUpdateError(UPDATE_ERROR_OPEN_FILE);
			break;
		}

		//
		// Read file header
		//
		fseek(file, 0, SEEK_SET);	//Point to Header
		sizeRead = fread(&sh, 1, sizeof(sh), file);
		if(sizeRead != sizeof(sh))
		{
			RETAILMSG(1,(_T("ERR:[%s]: Read: %s header failed, err:%d readsize:%d\r\n"),TEXT(__FUNCTION__),lpszFileName, GetLastError(),sizeRead));
			SetUpdateError(UPDATE_ERROR_READ_FILE);
			break;
		}
		DataReadLen = sizeof(sh);
		fseek(file, DataReadLen, SEEK_SET);	//Point to FileNameLen

		iFileCount=sh.dwFileCount;

		RETAILMSG(1, (TEXT("MSG:[%s]: Update.bin include %d files. \r\n"),TEXT(__FUNCTION__), iFileCount));

		DWORD dwChecksum=0;
		LPBYTE pFileBuf = NULL;

		RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::InstallPackage]: iFileCount:%d\r\n"),iFileCount));
		while(iFileCount>0 && GetUpdateStatus() == UPDATE_STATUS_START && GetUpdateError() == UPDATE_ERROR_NOERROR)
		{
			//
			// 1.	Get FileNameLen
			//
			sizeRead = fread(&iFileNameLen, sizeof(iFileNameLen), 1, file);
			if(sizeRead != 1)
			{
				RETAILMSG(1, (TEXT("MSG:[%s]: fread iFileNameLen sizeRead:%d ferror:%d \r\n"),TEXT(__FUNCTION__),sizeRead,ferror(file)));
				SetUpdateError(UPDATE_ERROR_READ_FILE);
				break;
			}

			// Data check
			if (iFileNameLen == 0)
			{
				RETAILMSG(1, (TEXT("MSG:[%s]: FileNameLen is 0, break! \r\n"),TEXT(__FUNCTION__)));
				SetUpdateError(UPDATE_ERROR_DATA);
				break;
			}

			// Pointer update
			DataReadLen += sizeof(iFileNameLen);
			fseek(file, DataReadLen, SEEK_SET);	//Point to FileName

			// Calculate FileNameLen checksum
			dwChecksum = CRC32(0, &iFileNameLen, sizeof(iFileNameLen));

			// Update statue check
			if(GetUpdateStatus() != UPDATE_STATUS_START)
			{
				break;
			}

			//
			// 2.	Get FileName
			//
			memset(strFileName, 0, sizeof(strFileName));
			sizeRead = fread(strFileName, iFileNameLen, 1, file);
			if(sizeRead != 1)
			{
				RETAILMSG(1, (TEXT("MSG:[%s]: fread strFileName sizeRead:%d ferror:%d \r\n"),TEXT(__FUNCTION__),sizeRead,ferror(file)));
				SetUpdateError(UPDATE_ERROR_READ_FILE);
				break;
			}

			// Update statue check
			if(GetUpdateStatus() != UPDATE_STATUS_START)
			{
				break;
			}

			// Pointer update
			DataReadLen += iFileNameLen;
			fseek(file, DataReadLen, SEEK_SET);	//Point to FileDataLen
			strFileName[iFileNameLen/2]='\0';
			RETAILMSG(1, (TEXT("MSG:[%s]: strFileName:%s\r\n"),TEXT(__FUNCTION__), strFileName));

			// Calculate FileName checksum
			dwChecksum += CRC32(0, strFileName, iFileNameLen);

			// Update statue check
			if(GetUpdateStatus() != UPDATE_STATUS_START)
			{
				break;
			}

			//
			// 3.	Get FileDataLen
			//
			sizeRead = fread(&iFileDataLen, sizeof(iFileDataLen), 1, file);
			if(sizeRead != 1)
			{
				RETAILMSG(1, (TEXT("MSG:[%s]: fread iFileDataLen sizeRead:%d ferror:%d \r\n"),TEXT(__FUNCTION__),sizeRead,ferror(file)));
				SetUpdateError(UPDATE_ERROR_READ_FILE);
				break;
			}

			// Data check
			if (iFileDataLen == 0)
			{
				RETAILMSG(1, (TEXT("MSG:[%s]: iFileDataLen:%d is invalid\r\n"),TEXT(__FUNCTION__), iFileDataLen));
				SetUpdateError(UPDATE_ERROR_DATA);
				break;
			}

			//Pointer update
			DataReadLen += sizeof(iFileDataLen);
			fseek(file, DataReadLen, SEEK_SET);	//Point to FileData

			// Calculate FileDataLen  checksum
			dwChecksum += CRC32(0, &iFileDataLen, sizeof(iFileDataLen));
			//RETAILMSG(1,(_T("dwChecksum:%d sizeof(iFileDataLen):%d\r\n"),dwChecksum,sizeof(iFileDataLen)));


			// Update statue check
			if(GetUpdateStatus() != UPDATE_STATUS_START)
			{
				break;
			}

			//
			// 4.	Compare file name
			//
			if(IsUpdateFile(strFileName))
			{
				//
				// Allocate memory
				//
				try
				{
					m_pdataBuffer = new unsigned char[iFileDataLen];
				}
				catch (const std::bad_alloc &)
				{
					m_pdataBuffer = NULL;
					RETAILMSG(1, (TEXT("ERR:[%s]: No memory for new m_pdataBuffer!\r\n"),TEXT(__FUNCTION__)));
					SetUpdateError(UPDATE_ERROR_ALLOC_MEMORY);
					break;
				}
				catch (...)
				{
					m_pdataBuffer = NULL;
					RETAILMSG(1, (TEXT("ERR:[%s]: new m_pdataBuffer Unknown err!\r\n"),TEXT(__FUNCTION__)));
					SetUpdateError(UPDATE_ERROR_ALLOC_MEMORY);
					break;
				}

				//
				// Get FileData
				//
				int iRemainSize=0;
				int iReadSize=0;
				unsigned char * FileDataBuf = NULL;
				iRemainSize = iFileDataLen;
				FileDataBuf = m_pdataBuffer;	//pFileData->GetFileDataBuf();
				do
				{
					iReadSize = min(iRemainSize, 1*1024*1024);

					sizeRead = fread(FileDataBuf, iReadSize, 1, file);
					if(sizeRead != 1)
					{
						RETAILMSG(1, (TEXT("MSG:[%s]: fread FileDataBuf sizeRead:%d ferror:%d \r\n"),TEXT(__FUNCTION__),sizeRead,ferror(file)));
						SetUpdateError(UPDATE_ERROR_READ_FILE);
						break;
					}

					DataReadLen += iReadSize;
					fseek(file, DataReadLen, SEEK_SET);	//Update pointer

					iRemainSize -= iReadSize;
					FileDataBuf += iReadSize;
				}while(iRemainSize && GetUpdateStatus()  == UPDATE_STATUS_START);

				// Calculate FileData  checksum
				dwChecksum += CRC32(0, m_pdataBuffer, iFileDataLen);
				//RETAILMSG(1,(_T("dwChecksum:%d iFileDataLen:%d\r\n"),dwChecksum,iFileDataLen));


				// Update statue check
				if(GetUpdateStatus() != UPDATE_STATUS_START)
				{
					break;
				}

				//
				// 5.	Get Checksum
				//
				DWORD dwCRC;
				sizeRead = fread(&dwCRC, sizeof(dwCRC), 1, file);
				if(sizeRead != 1)
				{
					RETAILMSG(1, (TEXT("MSG:[%s]: fread dwCRC sizeRead:%d ferror:%d \r\n"),TEXT(__FUNCTION__),sizeRead,ferror(file)));
					SetUpdateError(UPDATE_ERROR_READ_FILE);
					break;
				}

				DataReadLen += sizeof(dwCRC);
				fseek(file, DataReadLen, SEEK_SET);	//Update pointer

				// Compare checksum
				if (dwCRC == 0 || dwCRC != dwChecksum)
				{
					RETAILMSG(1,(_T("ERR:[%s]: Checksum err: CRC:%d Calculate:%d\r\n"),TEXT(__FUNCTION__),dwCRC,dwChecksum));
					SetUpdateError(UPDATE_ERROR_CRC);
					break;
				}

				dwChecksum=0;
			}
			else
			{
				//
				// Skip read
				//
				RETAILMSG(1, (TEXT("ERR:[%s]: %s is invalid, skip\r\n"),TEXT(__FUNCTION__), strFileName));

				DataReadLen += iFileDataLen;
				DataReadLen += sizeof(DWORD);
				fseek(file, DataReadLen, SEEK_SET);	//Point to FileData
			}

			//
			// Compare file name
			//
			if(lstrcmpi(strFileName,_T("oem.cfg")) == 0)
			{
				//Download cfg
				RETAILMSG(1,(_T("MSG:[CPanelUpdateARM::InstallPackage]: UpdateCfg:%X, %d\r\n"),m_pdataBuffer,iFileDataLen));
				bResult = UpdateCfg(m_pdataBuffer,iFileDataLen);
				if(!bResult)
				{
					RETAILMSG(1,(_T("ERR:[CPanelUpdateARM::InstallPackage]: UpdateCfg failed\r\n")));
					break;
				}
			}
			else if(lstrcmpi(strFileName,_T("Eboot.nb0")) == 0)
			{
				//Download EBOOT
				RETAILMSG(1,(_T("MSG:[CPanelUpdateARM::InstallPackage]: UPDProxyDownloadEboot:%X, %d\r\n"),m_pdataBuffer,iFileDataLen));
				bResult = UPDProxyDownloadEboot(m_hUPD, m_pdataBuffer, iFileDataLen);
				if(!bResult)
				{
					RETAILMSG(1,(_T("ERR:[CPanelUpdateARM::InstallPackage]: UPDProxyDownloadLogo failed\r\n")));
					break;
				}
			}
#if (CVTE_EN_OS_DYNAMIC_LOGO == CVTE_DISABLE)
			else if(lstrcmpi(strFileName,_T("startup.raw")) == 0)
			{
				//Download logo
				RETAILMSG(1,(_T("MSG:[CPanelUpdateARM::InstallPackage]: UPDProxyDownloadLogo:%X, %d\r\n"),m_pdataBuffer,iFileDataLen));
				bResult = UPDProxyDownloadLogo(m_hUPD, m_pdataBuffer, iFileDataLen);
				if(!bResult)
				{
					RETAILMSG(1,(_T("ERR:[CPanelUpdateARM::InstallPackage]: UPDProxyDownloadLogo failed\r\n")));
					break;
				}
			}
#endif
			else if(lstrcmpi(strFileName,_T("manifest.bin")) == 0)
			{
				//Download manifest
				RETAILMSG(1,(_T("MSG:[CPanelUpdateARM::InstallPackage]: UPDProxyDownloadImage:%X, %d\r\n"),m_pdataBuffer,iFileDataLen));
				bResult = UPDProxyDownloadImage(m_hUPD, m_pdataBuffer, iFileDataLen);
				if(!bResult)
				{
					RETAILMSG(1,(_T("ERR:[CPanelUpdateARM::InstallPackage]: UPDProxyDownloadImage failed\r\n")));
					break;
				}
			}
			else
			{
				//Skip
				RETAILMSG(1,(_T("MSG:[CPanelUpdateARM::InstallPackage]: Skip\r\n")));
			}

			if(m_pdataBuffer != NULL)
			{
				delete m_pdataBuffer;
				m_pdataBuffer = NULL;
			}

			iFileCount--;
		}
		bRet = (iFileCount==0 && GetUpdateError() == UPDATE_ERROR_NOERROR && bResult);
	}while(0);

	if(bRet)
	{
		Sleep(300);
		bRet = DownloadCfg();
	}
	else
	{
		RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::InstallPackage]: failed!!!!  Had downloaded %d files, GetUpdateError[%d], bResult\r\n"),iFileCount,GetUpdateError(),bResult));
	}

	if(m_pdataBuffer != NULL)
	{
		delete m_pdataBuffer;
		m_pdataBuffer = NULL;
	}
	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::InstallPackage]: --  %s!\r\n"), bRet?TEXT("Success"):TEXT("Failed")));

	return bRet;
}

#else
void CPanelUpdateARM::UpdateProc()
{
	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateProc] ++ \r\n")));

	m_strTitle = GetResourceString(IDS_IMAGE_UPDATING);

	if(!DownloadFile(UPDATE_FILE_EBOOT_USBDISK,IOCTL_UPD_STORE_EBOOT))
	{
		RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:CPanelUpdateARM::UpdateProc DownloadFile %s failed.\r\n"),UPDATE_FILE_EBOOT_USBDISK));
		SetUpdateStatus(UPDATE_STATUS_FAIL);
		SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_ABOUT);
		return;
	}
	//m_pAppConfig->GetOemSectorParam()->SGEbootInfo.Infor.cookie = 0;
	m_oemsector.SGEbootInfo.Infor.datavalid = 1;

	//m_pAppConfig->SetOemSector();

	if(!DownloadFile(UPDATE_FILE_LOGO_RAW_USBDISK,IOCTL_UPD_STORE_LOGO))
	{
		RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:CPanelUpdateARM::UpdateProc DownloadFile %s failed.\r\n"),UPDATE_FILE_LOGO_BMP_USBDISK));
		SetUpdateStatus(UPDATE_STATUS_FAIL);

		m_strTitle = GetResourceString(IDS_MCU_UPDATE_FAILED);
		goto _EXIT;
	}
	//m_pAppConfig->GetOemSectorParam()->SGLogoInfo.Infor.cookie = 0;
	m_oemsector.SGLogoInfo.Infor.datavalid = 1;

	//m_pAppConfig->SetOemSector();

	if(!DownloadFile(UPDATE_FILE_IMAGE_USBDISK,IOCTL_UPD_STORE_IMAGE))
	{
		RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:CPanelUpdateARM::UpdateProc DownloadFile %s failed.\r\n"),UPDATE_FILE_IMAGE_USBDISK));
		SetUpdateStatus(UPDATE_STATUS_FAIL);

		m_strTitle = GetResourceString(IDS_MCU_UPDATE_FAILED);
		goto _EXIT;
	}

	//m_pAppConfig->GetOemSectorParam()->SGImageInfo.Infor.cookie = 0;
	m_oemsector.SGImageInfo.Infor.datavalid = 1;

	//m_pAppConfig->SetOemSector();

	if(!DownloadCfg(UPDATE_FILE_OEMCFG_USBDISK))
	{
		RETAILMSG(DEBUG_UPDATE_ARM,(_T("ERR:CPanelUpdateARM::UpdateProc DownloadCfg failed.\r\n")));
		SetUpdateStatus(UPDATE_STATUS_FAIL);

		m_strTitle = GetResourceString(IDS_MCU_UPDATE_FAILED);
		goto _EXIT;
	}

	SetUpdateStatus(UPDATE_STATUS_SUCCESS);

_EXIT:

	m_strNotification = GetResourceString(IDS_RESTART_SYSTEM);
	m_pSkinManager->GetConfig()->GetCurParam()->dwSoftwareUpdateFlag = 0;
	Sleep(3000);

	RETAILMSG(DEBUG_UPDATE_ARM,(_T("MSG:[CPanelUpdateARM::UpdateProc] -- \r\n")));

	//reset:
	::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
}
#endif

BOOL CPanelUpdateARM::IsUpdateFileExists(LPCTSTR   lpszFileName)
{
	WIN32_FIND_DATA   wfd;
	BOOL   bRet;
	HANDLE   hFind;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   hFind   !=   INVALID_HANDLE_VALUE;
	FindClose(hFind);
	return   bRet;
}

BOOL CPanelUpdateARM::DownloadFile(LPCTSTR lpszFileName, UINT IoCtl)
{
	HANDLE hFile = NULL;
	HANDLE hFileMapping = NULL;

    DWORD  dwFileSize,dwFileSizeHigh;
    LPBYTE pFileBuf = NULL;

	BOOL bRet = FALSE;

    RETAILMSG(1, (_T("MSG:[CPanelUpdateARM::DownloadFile]: [%s] , IoCtl[%X] \r\n"),lpszFileName, IoCtl));

	hFile = CreateFile(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_RANDOM_ACCESS,NULL);
    if (hFile  == NULL )
	{
	    RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadFile CreateFile[%s] failed.\r\n"),lpszFileName));
		goto _exit;
	}

	dwFileSize = GetFileSize(hFile,&dwFileSizeHigh);
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadFile dwFileSize[%d]\r\n"),dwFileSize));

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hFileMapping == NULL)
    {
        RETAILMSG(1, (_T("ERR:CPanelUpdateARM::DownloadFile CreateFileMapping Fail! \r\n")));
        goto _exit;
    }
    //4.Mapping Memory Address
    pFileBuf = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (pFileBuf == NULL)
    {
        RETAILMSG(1, (_T("ERR:CPanelUpdateARM::DownloadFile MapViewOfFile Fail! \r\n")));
        goto _exit;
    }

    if(m_hUPD == NULL)
    {
        RETAILMSG(1, (_T("ERR:CPanelUpdateARM::DownloadFile m_hUPD is NULL! \r\n")));
        goto _exit;
    }

	if(!DeviceIoControl(m_hUPD, IoCtl, pFileBuf, dwFileSize, NULL, 0, NULL, NULL))
	{
        RETAILMSG(1, (_T("ERR:CPanelUpdateARM::DownloadFile DeviceIoControl fail m_hUPD[%X] IoCtl[%X], pFileBuf[%x] dwFileSize[%d] , err=%d! \r\n"),m_hUPD,IoCtl,pFileBuf,dwFileSize,GetLastError()));
	}
    else
    {
        bRet = TRUE;
    }

_exit:

    if (pFileBuf)
    {
        UnmapViewOfFile(pFileBuf);
        pFileBuf = NULL;
    }

    if (hFileMapping)
    {
        CloseHandle(hFileMapping);
        hFileMapping = NULL;
    }

    if (hFile)
    {
        CloseHandle(hFile);
        hFile = NULL;
    }

    return bRet;
}

BOOL CPanelUpdateARM::UpdateCfg(LPBYTE lpBuf, DWORD dwLen)
{

	if(lpBuf == NULL || dwLen != sizeof(OEMSECTOR))
	{
        RETAILMSG(1, (_T("ERR:[CPanelUpdateARM::UpdateCfg]:  param chcck failed! dwLen:%d \r\n"),dwLen));
		return FALSE;
	}

	memcpy(&m_oemsector, lpBuf, dwLen);

	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: oemsector:\r\n")));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: VersionID:       %S\r\n"),m_oemsector.VersionID));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: UpdateOEMSector: %d\r\n"),m_oemsector.UpdateOEMSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: UpdateEboot:     %d\r\n"),m_oemsector.UpdateEboot));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: UpdateNK:        %d\r\n"),m_oemsector.UpdateNK));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: UpdateLogo:      %d\r\n"),m_oemsector.UpdateLogo));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: UpdateUI:        %d\r\n"),m_oemsector.UpdateUI));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: EraseFlsah:      %d\r\n"),m_oemsector.EraseFlsah));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: UpdateType:      %d\r\n"),m_oemsector.UpdateType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: VersionType:     %d\r\n"),m_oemsector.VersionType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: TraceType:       %d\r\n"),m_oemsector.TraceType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: TouchType:       %d\r\n"),m_oemsector.TouchType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: GPSType:         %d\r\n"),m_oemsector.GPSType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: PowerStatus:     %d\r\n"),m_oemsector.PowerStatus));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: RVChannel:       %d\r\n"),m_oemsector.RVChannel));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: LCDGamma:        %d\r\n"),m_oemsector.LCDGamma));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: ColorSignal:     %d\r\n"),m_oemsector.ColorSignal));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGEbootInfo.Infor:     %d\r\n"),m_oemsector.SGEbootInfo.Infor));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGEbootInfo.dwSector:  %d\r\n"),m_oemsector.SGEbootInfo.dwSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGEbootInfo.dwLength:  %d\r\n"),m_oemsector.SGEbootInfo.dwLength));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGLogoInfo.Infor:      %d\r\n"),m_oemsector.SGLogoInfo.Infor));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGLogoInfo.dwSector:   %d\r\n"),m_oemsector.SGLogoInfo.dwSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGLogoInfo.dwLength:   %d\r\n"),m_oemsector.SGLogoInfo.dwLength));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGImageInfo.Infor:     %d\r\n"),m_oemsector.SGImageInfo.Infor));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGImageInfo.dwSector:  %d\r\n"),m_oemsector.SGImageInfo.dwSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::UpdateCfg]: SGImageInfo.dwLength:  %d\r\n"),m_oemsector.SGImageInfo.dwLength));
	
	return TRUE;
}

#if CVTE_EN_PACKAGE_UPDATE
BOOL CPanelUpdateARM::DownloadCfg()
{
	BOOL bRet = FALSE;
	OEMSECTOR oemsector;
	memset(&oemsector, 0, sizeof(oemsector));

	if(!AppProxyGetWholeOem(m_hOPD, &oemsector, sizeof(oemsector)))
	{
		RETAILMSG(1, (TEXT("ERR[%s] Get whole oemsector failed. err=%d\r\n"), TEXT(__FUNCTION__), GetLastError()));
		return FALSE;
	}

	memcpy(&oemsector.VersionID[0],&m_oemsector.VersionID[0],sizeof(BYTE)*VERSIONIDLENGTH);

    oemsector.UpdateOEMSector = OEMCONFIG_FLAG_DISABLE;    //oemsector.UpdateOEMSector;
    oemsector.UpdateEboot = OEMCONFIG_FLAG_ENABLE;    //oemsector.UpdateEboot;
    oemsector.UpdateNK = OEMCONFIG_FLAG_ENABLE;   //oemsector.UpdateNK;
    oemsector.UpdateLogo = OEMCONFIG_FLAG_ENABLE; //oemsector.UpdateLogo;
    oemsector.UpdateUI = OEMCONFIG_FLAG_DISABLE;
    oemsector.UpdateType = OEMCONFIG_UPD_TYPE_STORE;

    oemsector.EraseFlsah = m_oemsector.EraseFlsah;
    oemsector.VersionType = OEMCONFIG_VERSION_TYPE_RELEASE;//oemsector.VersionType;
    //oemsector.TraceType = oemsector.TraceType;
    //oemsector.TouchType = oemsector.TouchType;
    //oemsector.GPSType = oemsector.GPSType;
    //oemsector.PowerStatus = oemsector.PowerStatus;
    //oemsector.RVChannel = oemsector.RVChannel;
    //oemsector.LCDGamma = oemsector.LCDGamma;
    //oemsector.ColorSignal = oemsector.ColorSignal;

    bRet = AppProxySetWholeOem(m_hOPD, &oemsector, sizeof(oemsector));
    if(bRet)
    {
        RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: Save oemsector successfully\r\n")));
    }
    else
    {
        RETAILMSG(1, (TEXT("ERR:[CPanelUpdateARM::DownloadCfg]: Save oemsector failed\r\n")));
    }

	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: oemsector:\r\n")));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: VersionID:       %S\r\n"),oemsector.VersionID));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: UpdateOEMSector: %d\r\n"),oemsector.UpdateOEMSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: UpdateEboot:     %d\r\n"),oemsector.UpdateEboot));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: UpdateNK:        %d\r\n"),oemsector.UpdateNK));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: UpdateLogo:      %d\r\n"),oemsector.UpdateLogo));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: UpdateUI:        %d\r\n"),oemsector.UpdateUI));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: EraseFlsah:      %d\r\n"),oemsector.EraseFlsah));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: UpdateType:      %d\r\n"),oemsector.UpdateType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: VersionType:     %d\r\n"),oemsector.VersionType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: TraceType:       %d\r\n"),oemsector.TraceType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: TouchType:       %d\r\n"),oemsector.TouchType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: GPSType:         %d\r\n"),oemsector.GPSType));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: PowerStatus:     %d\r\n"),oemsector.PowerStatus));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: RVChannel:       %d\r\n"),oemsector.RVChannel));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: LCDGamma:        %d\r\n"),oemsector.LCDGamma));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: ColorSignal:     %d\r\n"),oemsector.ColorSignal));
	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: Backlight:       %d\r\n"),oemsector.Backlight));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGEbootInfo.Infor:     %d\r\n"),oemsector.SGEbootInfo.Infor));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGEbootInfo.dwSector:  %d\r\n"),oemsector.SGEbootInfo.dwSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGEbootInfo.dwLength:  %d\r\n"),oemsector.SGEbootInfo.dwLength));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGLogoInfo.Infor:      %d\r\n"),oemsector.SGLogoInfo.Infor));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGLogoInfo.dwSector:   %d\r\n"),oemsector.SGLogoInfo.dwSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGLogoInfo.dwLength:   %d\r\n"),oemsector.SGLogoInfo.dwLength));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGImageInfo.Infor:     %d\r\n"),oemsector.SGImageInfo.Infor));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGImageInfo.dwSector:  %d\r\n"),oemsector.SGImageInfo.dwSector));
    RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: SGImageInfo.dwLength:  %d\r\n"),oemsector.SGImageInfo.dwLength));
	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: sFunctionOption_t.MouseCursor:  %d\r\n"),oemsector.FunctionOption_t.sFunctionOption_t.MouseCursor));
	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: sFunctionOption_t.TouchAdapter: %d\r\n"),oemsector.FunctionOption_t.sFunctionOption_t.TouchAdapter));
	RETAILMSG(1, (TEXT("MSG:[CPanelUpdateARM::DownloadCfg]: sFunctionOption_t.DynamicLogo:  %d\r\n"),oemsector.FunctionOption_t.sFunctionOption_t.DynamicLogo));
	
    return bRet;
}
#else
BOOL CPanelUpdateARM::DownloadCfg(LPCTSTR lpszFileName)
{
	FILE *hFile = NULL;
	UINT FileLen;
	PBYTE pFileBuf = NULL;
	BOOL bRet = FALSE;
	OEMSECTOR oemsector;
	HANDLE hOPD = NULL;


	if ( (hFile = _tfopen(lpszFileName, TEXT("rb"))) == NULL )
	{
		RETAILMSG(1, (_T("ERR:CPanelUpdateARM::DownloadCfg Open %s fail, err=%d! \r\n"),lpszFileName, GetLastError()));
		goto _exit;
	}

	fseek(hFile, 0, SEEK_END);

	FileLen = ftell(hFile);
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg FileLen[%d]\r\n"),FileLen));

	if(FileLen != sizeof(OEMSECTOR))
	{
		RETAILMSG(1, (TEXT("ERR:CPanelUpdateARM::DownloadCfg FileLen[%d] != OEMSECTOR[%d]\r\n"),FileLen, sizeof(OEMSECTOR)));
		goto _exit;
	}

	fseek(hFile, 0, SEEK_SET);

	pFileBuf = (PBYTE)malloc(FileLen);

	if(FileLen != fread(pFileBuf, sizeof(BYTE), FileLen, hFile))
	{
		RETAILMSG(1, (TEXT("ERR:CPanelUpdateARM::DownloadCfg Read [%s] fail.\r\n"),lpszFileName));
		goto _exit;
	}
	memset(&oemsector, 0, sizeof(oemsector));
	memcpy(&oemsector, pFileBuf, sizeof(pFileBuf));

	memcpy(m_oemsector.VersionID,pFileBuf,VERSIONIDLENGTH*sizeof(BYTE));

	//	    memcpy(&m_oemsector, pFileBuf, 46*sizeof(BYTE));
	m_oemsector.UpdateOEMSector = OEMCONFIG_FLAG_DISABLE;    //oemsector.UpdateOEMSector;
	m_oemsector.UpdateEboot = OEMCONFIG_FLAG_ENABLE;    //oemsector.UpdateEboot;
	m_oemsector.UpdateNK = OEMCONFIG_FLAG_ENABLE;   //oemsector.UpdateNK;
	m_oemsector.UpdateLogo = OEMCONFIG_FLAG_ENABLE; //oemsector.UpdateLogo;
	m_oemsector.UpdateUI = OEMCONFIG_FLAG_DISABLE;
	m_oemsector.UpdateType = OEMCONFIG_UPD_TYPE_STORE;

	m_oemsector.EraseFlsah = OEMCONFIG_FLAG_DISABLE;
	m_oemsector.VersionType = OEMCONFIG_VERSION_TYPE_RELEASE;//oemsector.VersionType;
	m_oemsector.TraceType = oemsector.TraceType;
	//m_oemsector.TouchType = oemsector.TouchType;
	//m_oemsector.GPSType = oemsector.GPSType;
	//m_oemsector.PowerStatus = oemsector.PowerStatus;
	//m_oemsector.RVChannel = oemsector.RVChannel;
	//m_oemsector.LCDGamma = oemsector.LCDGamma;
	//m_oemsector.ColorSignal = oemsector.ColorSignal;

	bRet = DeviceIoControl(m_hOPD, OEMCONFIG_IOCTL_SET_WHOLE_OEMSECTOR, &m_oemsector, sizeof(m_oemsector), NULL, 0, NULL, NULL);
	if(bRet)
	{
		RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg Save oemsector successfully\r\n")));
	}
	else
	{
		RETAILMSG(1, (TEXT("ERR:CPanelUpdateARM::DownloadCfg Save oemsector failed\r\n")));
	}

	Sleep(250);

	memset(&oemsector, 0, sizeof(oemsector));
	bRet = DeviceIoControl(m_hOPD, OEMCONFIG_IOCTL_GET_WHOLE_OEMSECTOR, NULL, 0,	&oemsector, sizeof(oemsector), NULL, NULL);
	if(bRet)
	{
		RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg Save oemsector successfully\r\n")));
	}
	else
	{
		RETAILMSG(1, (TEXT("ERR:CPanelUpdateARM::DownloadCfg Save oemsector failed\r\n")));
	}

	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg oemsector:\r\n")));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg VersionID:       %S\r\n"),oemsector.VersionID));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg UpdateOEMSector: %d\r\n"),oemsector.UpdateOEMSector));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg UpdateEboot:     %d\r\n"),oemsector.UpdateEboot));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg UpdateNK:        %d\r\n"),oemsector.UpdateNK));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg UpdateLogo:      %d\r\n"),oemsector.UpdateLogo));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg UpdateUI:        %d\r\n"),oemsector.UpdateUI));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg EraseFlsah:      %d\r\n"),oemsector.EraseFlsah));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg UpdateType:      %d\r\n"),oemsector.UpdateType));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg VersionType:     %d\r\n"),oemsector.VersionType));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg TraceType:       %d\r\n"),oemsector.TraceType));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg TouchType:       %d\r\n"),oemsector.TouchType));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg GPSType:         %d\r\n"),oemsector.GPSType));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg PowerStatus:     %d\r\n"),oemsector.PowerStatus));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg RVChannel:       %d\r\n"),oemsector.RVChannel));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg LCDGamma:        %d\r\n"),oemsector.LCDGamma));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg ColorSignal:     %d\r\n"),oemsector.ColorSignal));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGEbootInfo.Infor:     %d\r\n"),oemsector.SGEbootInfo.Infor));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGEbootInfo.dwSector:  %d\r\n"),oemsector.SGEbootInfo.dwSector));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGEbootInfo.dwLength:  %d\r\n"),oemsector.SGEbootInfo.dwLength));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGLogoInfo.Infor:      %d\r\n"),oemsector.SGLogoInfo.Infor));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGLogoInfo.dwSector:   %d\r\n"),oemsector.SGLogoInfo.dwSector));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGLogoInfo.dwLength:   %d\r\n"),oemsector.SGLogoInfo.dwLength));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGImageInfo.Infor:     %d\r\n"),oemsector.SGImageInfo.Infor));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGImageInfo.dwSector:  %d\r\n"),oemsector.SGImageInfo.dwSector));
	RETAILMSG(1, (TEXT("MSG:CPanelUpdateARM::DownloadCfg SGImageInfo.dwLength:  %d\r\n"),oemsector.SGImageInfo.dwLength));

_exit:

	if(hFile)
	{
		fclose(hFile);
	}

	if(pFileBuf)
	{
		free(pFileBuf);
	}

	return bRet;
}

#endif

BOOL CPanelUpdateARM::IsFileExists(LPCTSTR   lpszFileName)
{
	//
	WIN32_FIND_DATA   wfd;
	HANDLE   hFind;
	BOOL   bRet;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   (hFind   !=   INVALID_HANDLE_VALUE);
	FindClose(hFind);
	return   bRet;
}

void CPanelUpdateARM::OnDeviceChanged()
{	
#if CVTE_EN_PACKAGE_UPDATE
	m_bFileExitImage = IsFileExists(UPDATE_FILE_PACKAGE_USBDISK);
#else
	m_bFileExitImage = IsFileExists(UPDATE_FILE_IMAGE_USBDISK);
	m_bFileExitUI = IsFileExists(UPDATE_FILE_UI_USBDISK);
	m_bFileExitLogoRaw = IsFileExists(UPDATE_FILE_LOGO_RAW_USBDISK);
#endif
#if (CVTE_EN_USER_UPDATE_LOGO && !CVTE_EN_OS_DYNAMIC_LOGO)
	m_bFileExitLogoBmp = IsFileExists(UPDATE_FILE_LOGO_BMP_USBDISK) && IsFileExists(UPDATE_FILE_LOGO_JPG_USBDISK);
#endif
#if CVTE_EN_OS_DYNAMIC_LOGO
	m_bFileExitRes = IsFileExists(UPDATE_FILE_RES_USBDISK);
#endif

	RETAILMSG(1, (TEXT("MSG:[%s] \r\n"), TEXT(__FUNCTION__)));
	if(GetUpdateStatus() == UPDATE_STATUS_START)
	{
		switch(GetUpdateType())
		{
		case UPDATE_TYPE_LOGO:
			if(!m_bFileExitLogoBmp)
			{
				SetUpdateError(UPDATE_ERROR_DEVICE);
				SetUpdateStatus(UPDATE_STATUS_FAIL);
			}
			break;
		case UPDATE_TYPE_ALL:
			if(!m_bFileExitImage)
			{
				SetUpdateError(UPDATE_ERROR_DEVICE);
				SetUpdateStatus(UPDATE_STATUS_FAIL);
			}
			break;
		case UPDATE_TYPE_RES:
			if(!m_bFileExitRes)
			{
				SetUpdateError(UPDATE_ERROR_DEVICE);
				SetUpdateStatus(UPDATE_STATUS_FAIL);
			}
			break;
		default:
			break;
		}
	}
	else
	{
#if CVTE_EN_PACKAGE_UPDATE
		m_btnUpdateAll.Enable(m_bFileExitImage);
#else
		m_btnUpdateAll.Enable(m_bFileExitUI && m_bFileExitLogoRaw && m_bFileExitImage);
#endif
#if (CVTE_EN_USER_UPDATE_LOGO && !CVTE_EN_OS_DYNAMIC_LOGO)
		m_btnUpdateLogo.Enable(m_bFileExitLogoBmp);
#endif
#if CVTE_EN_OS_DYNAMIC_LOGO
		m_btnUpdateDynamicLogo.Enable(m_bFileExitRes);
#endif
	}
}

void CPanelUpdateARM::RequestUpdateMCU(BOOL bUpdate)
{
	m_dwUpdateMcuRequestFlag = bUpdate;

	m_bOnRequestMcuUpdate = TRUE;

	RETAILMSG(DEBUG_SETTING,(_T("###### RequestUpdateMCU : %d\r\n"),bUpdate));

	if(m_pCommander)
	{
		m_pCommander->SendCmd(_ARM_UPDATE_STATUS,bUpdate);
	}
}

void CPanelUpdateARM::SetUpdateStatus(BYTE nUpdateStatus)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__), nUpdateStatus));
	m_nUpdateStatus = nUpdateStatus;
	switch(m_nUpdateStatus)
	{
	case UPDATE_STATUS_IDLE:
		m_strTitle = GetResourceString(IDS_UPDATE);
		m_strNotification = GetResourceString(IDS_MCU_UPDATING_ALERT);
		break;
	case UPDATE_STATUS_START:
		m_strTitle = GetResourceString(IDS_IMAGE_UPDATING);
		break;
	case UPDATE_STATUS_SUCCESS:
		m_strNotification = GetResourceString(IDS_RESTART_SYSTEM);
		m_pSkinManager->GetConfig()->GetCurParam()->dwSoftwareUpdateFlag = 0;
		break;
	case UPDATE_STATUS_STOP:
	case UPDATE_STATUS_FAIL:
		m_strTitle = GetResourceString(IDS_MCU_UPDATE_FAILED);
		m_strNotification = GetResourceString(IDS_RESTART_SYSTEM);
		break;
	case UPDATE_STATUS_END:
		break;
	default:
		break;
	}
}

BYTE CPanelUpdateARM::GetUpdateStatus()
{
	return m_nUpdateStatus;
}

void CPanelUpdateARM::SetUpdateType(BYTE nUpdateType)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__), nUpdateType));
	m_nUpdateType = nUpdateType;
}

BYTE CPanelUpdateARM::GetUpdateType()
{
	return m_nUpdateType;
}

void CPanelUpdateARM::SetUpdateError(BYTE nUpdateError)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__), nUpdateError));
	m_nUpdateError = nUpdateError;
}

BYTE CPanelUpdateARM::GetUpdateError()
{
	return m_nUpdateError;
}

BOOL CPanelUpdateARM::InstallRes(LPCTSTR lpszFileName)
{
	HANDLE hDevice = NULL;
	FILE *hFile = NULL;
	UINT FileLen;
	BYTE	* pFileBuf = NULL;
	BOOL Ret = FALSE;
	int readSize = 0;
	int leftSize = 0;
	int secRead = 0;
	const int sector_size = 1024*1024;

	RETAILMSG(1, (TEXT("[%s]:[%s]\r\n"), TEXT(__FUNCTION__),lpszFileName));

	if ( 0!=_wfopen_s(&hFile, lpszFileName,_T("rb")) )
	{
		RETAILMSG(1,(_T("file not found. %s\r\n"),lpszFileName));
		return false;
	}

	fseek(hFile, 0, SEEK_END);

	FileLen = ftell(hFile);

	fseek(hFile, 0, SEEK_SET);

	RETAILMSG(1, (TEXT("[%s]: FileLen=%d\r\n"), TEXT(__FUNCTION__),FileLen));

	pFileBuf = new BYTE[FileLen+1];
	if(!pFileBuf)
	{
		goto _exit;
	}

	leftSize = FileLen;
	int size_to_read = 0;
	do 
	{
		size_to_read = min(sector_size, leftSize);
		secRead = fread(pFileBuf+readSize, size_to_read, 1, hFile);
		if(secRead)
		{
			readSize += size_to_read;
			leftSize -= size_to_read;
		}
	} while (secRead == 1 && sector_size==size_to_read);

	RETAILMSG(1, (TEXT("[%s]: actual read FileLen=%d\r\n"), TEXT(__FUNCTION__),readSize));
	if(readSize != FileLen)
	{
		goto _exit;
	}

	if(!UPDProxyDownloadRes(m_hUPD, pFileBuf, FileLen))
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: UPDProxyDownloadRes failed\r\n"), TEXT(__FUNCTION__)));
		Ret = FALSE;
	}
	else
	{
		Ret = TRUE;
	}

_exit:

	if(hFile)
	{
		fclose(hFile);
	}

	if(pFileBuf)
	{
		delete [] pFileBuf;
	}

	return Ret;
}