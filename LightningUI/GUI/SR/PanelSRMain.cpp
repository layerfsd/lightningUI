#include "PanelSRMain.h"
#include "../../resource.h"
#include "../../Bluetooth/include/txml/txml.h"

#define ISS_SR_PARAM_SPEECH_TIMEOUTW L"speechtimeout"
#define ISS_SR_PARAM_LONGTITUDEW    L"longitude"
#define ISS_SR_PARAM_LATITUDEW      L"latitude"

class _W2CP
{
	char* m_s;
public:
	_W2CP(int CP, const wchar_t* s)
	{
		int l = ::WideCharToMultiByte(CP, 0, s, -1, NULL, 0, NULL, NULL);
		m_s = new char[l];
		::WideCharToMultiByte(CP, 0, s, -1, m_s, l, NULL, NULL);
	}
	~_W2CP() { delete [] m_s; }
	operator const char*() const { return m_s; }
};

class _CP2W
{
	wchar_t* m_s;
public:
	_CP2W(int CP, const char* s)
	{
		int l = ::MultiByteToWideChar(CP, 0, s, -1, NULL, 0);
		m_s = new wchar_t[l];
		::MultiByteToWideChar(CP, 0, s, -1, m_s, l);
	}
	~_CP2W() { delete [] m_s; }
	operator const wchar_t*() const { return m_s; }
};

template<int CP> struct _W2X : _W2CP { _W2X(const wchar_t* s) : _W2CP(CP, s) { } };
template<int CP> struct _X2W : _CP2W { _X2W(const char* s) : _CP2W(CP, s) { } };

typedef _W2X<CP_ACP> _W2A;
typedef _X2W<CP_ACP> _A2W;

typedef _X2W<CP_UTF8> _UTF82W;
typedef _W2X<CP_UTF8> _W2UTF8;

CPanelSRMain::CPanelSRMain(void)
{
	m_bSRini = FALSE;
	m_hIssSr = NULL;
	m_pWaveRecorder = NULL;
	m_bIsRecording = FALSE;
	m_iSpeechVol = 0;
	m_bGettingResult = FALSE;
}
CPanelSRMain::~CPanelSRMain(void)
{
	DeInitializeSR();
}

BOOL CPanelSRMain::IsReady()
{
	return CPanelGL::IsReady() && (!m_bGettingResult);
}

void CPanelSRMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

void CPanelSRMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}

void CPanelSRMain::Render()
{
	if(!BeginRender())return;

	m_tagSRResult.SetAlpha(m_fCurAlpha);
	m_tagSRResult.Render();

	m_btnMic.SetAlpha(m_fCurAlpha);
	m_btnMic.Render();

	m_btnStart.SetAlpha(m_fCurAlpha);
	m_btnStart.Render();

	m_btnEndAudioData.SetAlpha(m_fCurAlpha);
	m_btnEndAudioData.Render();

	if(m_bIsRecording)
	{
		for (int i=0;i<m_iSpeechVol;i++)
		{
			m_tagWave.SetShowScale(2.0f+i*0.2f);
			m_tagWave.SetAlpha(m_fCurAlpha*(1.0f-i*0.2f));
			m_tagWave.Render();
		}
	}

	if(m_bGettingResult)
	{
		VIEW_STATE vs=m_tagProcessing.GetCurViewState();
		vs.fRotateZ-=5;
		if(vs.fRotateZ<-360)
			vs.fRotateZ=0;
		m_tagProcessing.SetCurViewState(&vs);

		m_tagProcessing.SetAlpha(m_fCurAlpha);
		m_tagProcessing.Render();
	}


	if(IsAniReady() && !m_bSRini)
	{
		InitializeSR();
	}

	EndRender();
}

BOOL CPanelSRMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	SIZE siInfo={800,40};
	COLORGL cr_w={1,1,1,1};

	m_tagSRResult.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagSRResult.SetSize(&siInfo);
	m_tagSRResult.SetCaption(_T("正在初始化..."));
	m_tagSRResult.LinkTextGL(pTextGL);
	m_tagSRResult.SetPos(0,180);
	m_tagSRResult.SetCaptionHeight(40);
	m_tagSRResult.EnableDrawBack(FALSE);
	m_tagSRResult.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagSRResult.SetCaptionColor(&cr_w);

	m_tagWave.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_710));

	m_tagProcessing.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713));
	m_tagProcessing.SetShowScale(2.0f);

	m_btnMic.Initialize(IDC_SR_BTN_MIC,this,m_pSkinManager->GetTexture(TEXID_711),NULL,
		m_pSkinManager->GetTexture(TEXID_712),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnMic.SetNotifyFunc(&CPanelSRMain::OnNotify,this);
	m_btnMic.SetShowScale(2.0f);
	m_btnMic.SetPos(0,0);


	SIZE si0={400,100};
	m_btnStart.Initialize(IDC_SR_BTN_START,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnStart.SetCaption(_T("开始"));
	m_btnStart.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnStart.SetCaptionHeight(32);
	m_btnStart.LinkTextGL(pTextGL);
	m_btnStart.SetSize(&si0);
	m_btnStart.SetNotifyFunc(& CPanelSRMain::OnNotify,this);
	m_btnStart.SetPos(0,-g_iClientHeight/2+50);
	m_btnStart.Show(FALSE);

	m_btnEndAudioData.Initialize(IDC_SR_BTN_ENDAUDIODATA,this,m_pSkinManager->GetTexture(TEXID_533),NULL,
		m_pSkinManager->GetTexture(TEXID_534),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEndAudioData.SetCaption(_T("我说完了"));
	m_btnEndAudioData.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnEndAudioData.SetCaptionHeight(32);
	m_btnEndAudioData.LinkTextGL(pTextGL);
	m_btnEndAudioData.SetSize(&si0);
	m_btnEndAudioData.SetNotifyFunc(& CPanelSRMain::OnNotify,this);
	m_btnEndAudioData.SetPos(0,-g_iClientHeight/2+50);
	m_btnEndAudioData.Show(FALSE);

	return TRUE;
}


void CALLBACK CPanelSRMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSRMain *panel=(CPanelSRMain*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSRMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SR_BTN_MIC:
		SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
		break;
	case IDC_SR_BTN_START:
		SRStart();
		break;
	case IDC_SR_BTN_ENDAUDIODATA:
		SREndAudioData();
		break;
	}
}
void CPanelSRMain::PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	if(m_hMainWnd)
	{
		::PostMessage(m_hMainWnd,Msg,wParam,lParam);
	}
}
void CPanelSRMain::HandleSRMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SR_MSG_InitStatus:
		HandleOnMsgProc_InitStatus(wParam,lParam);
		break;
	case WM_SR_MSG_UpLoadDictToLocalStatus:
		HandleOnMsgProc_OnUpLoadDictToLocalStatus(wParam,lParam);
		break;
	case WM_SR_MSG_UpLoadDictToCloudStatus:
		HandleOnMsgProc_OnUpLoadDictToCloudStatus(wParam,lParam);
		break;
	case WM_SR_MSG_VolumeLevel:
		HandleOnMsgProc_OnVolumeLevel(wParam,lParam);
		break;
	case WM_SR_MSG_ResponseTimeout:
		HandleOnMsgProc_OnResponseTimeout(wParam,lParam);
		break;
	case WM_SR_MSG_SpeechStart:
		HandleOnMsgProc_OnSpeechStart(wParam,lParam);
		break;
	case WM_SR_MSG_SpeechTimeOut:
		HandleOnMsgProc_OnSpeechTimeOut(wParam,lParam);
		break;
	case WM_SR_MSG_SpeechEnd:
		HandleOnMsgProc_OnSpeechEnd(wParam,lParam);
		break;
	case WM_SR_MSG_Error:
		HandleOnMsgProc_OnError(wParam,lParam);
		break;
	case WM_SR_MSG_Result:
		HandleOnMsgProc_OnResult(wParam,lParam);
		break;
	}
}
BOOL CPanelSRMain::InitializeSR()
{
	ISSErrID err;
	wchar_t *wstr_path = _T("\\GPS CARD\\bin\\SRRes");
// 	wchar_t *wstr_path = _T("\\USB Disk\\bin\\SRRes");

	char fake_machine_id_buf[1024]="ThisIsAFakeMachineIdWhichNotLongerThan1024";
	err = ISSSetMachineCode(fake_machine_id_buf);

	RETAILMSG(DEBUG_SR,(_T("##[SR]## ISSSetMachineCode:%d\r\n"),err));

	err = ISSSRCreateW(&m_hIssSr, wstr_path, CPanelSRMain::ProcOnMsgProc, this, 0);

	RETAILMSG(DEBUG_SR,(_T("##[SR]## ISSSRCreateW:%d\r\n"),err));

	m_pWaveRecorder = new CWaveRecorder(this);

	memset(m_szErrMsg, 0, sizeof(m_szErrMsg));
	memset(m_szRlt, 0, sizeof(m_szRlt));

	ISSSRSetParam(m_hIssSr, ISS_SR_PARAM_TRACE_LEVEL, ISS_SR_PARAM_TRACE_LEVEL_VALUE_DEBUG);

	RETAILMSG(DEBUG_SR,(_T("##[SR]## ISSSRSetParam ISS_SR_PARAM_TRACE_LEVEL:%d\r\n"),err));

	//location
	err = ISSSRSetParam(m_hIssSr, ISS_SR_PARAM_LONGTITUDE, "117.17");
	RETAILMSG(DEBUG_SR,(_T("##[SR]## ISSSRSetParam ISS_SR_PARAM_LONGTITUDE:%d\r\n"),err));

	err = ISSSRSetParam(m_hIssSr, ISS_SR_PARAM_LATITUDE, "31.837463");
	RETAILMSG(DEBUG_SR,(_T("##[SR]## ISSSRSetParam ISS_SR_PARAM_LATITUDE:%d\r\n"),err));


	m_bSRini = TRUE;

	return err==ISS_SUCCESS;
}
BOOL CPanelSRMain::DeInitializeSR()
{
	if(m_bSRini)
	{
		OnEndRecord();

		if(m_hIssSr)
		{
			ISSErrID err = ISSSRDestroy(m_hIssSr);

			m_hIssSr = NULL;
		}

		if(m_pWaveRecorder)
		{
			delete m_pWaveRecorder;
			m_pWaveRecorder=NULL;
		}
	}

	return 1;
}
void CPanelSRMain::ProcOnMsgProc(const void* pUsrArg,unsigned int uMsg, unsigned int wParam, const void* lParam)
{
	CPanelSRMain* pThis = (CPanelSRMain*)pUsrArg;
	switch(uMsg) 
	{
	case ISS_SR_MSG_InitStatus:
		pThis->PostMessage(WM_SR_MSG_InitStatus, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_UpLoadDictToLocalStatus:
		if(lParam) 
		{
			strcpy(pThis->m_szDictName, (const char*)lParam);
		}
		else 
		{
			strcpy(pThis->m_szDictName, "");
		}
		pThis->PostMessage(WM_SR_MSG_UpLoadDictToLocalStatus, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_UpLoadDictToCloudStatus:
		if(lParam) 
		{
			strcpy(pThis->m_szDictName, (const char*)lParam);
		} 
		else
		{
			strcpy(pThis->m_szDictName, "");
		}
		pThis->PostMessage(WM_SR_MSG_UpLoadDictToCloudStatus, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_VolumeLevel:
		pThis->PostMessage(WM_SR_MSG_VolumeLevel, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_ResponseTimeout:
		pThis->PostMessage(WM_SR_MSG_ResponseTimeout, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_SpeechStart:
		pThis->PostMessage(WM_SR_MSG_SpeechStart, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_SpeechTimeOut:
		pThis->PostMessage(WM_SR_MSG_SpeechTimeOut, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_SpeechEnd:
		pThis->PostMessage(WM_SR_MSG_SpeechEnd, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_Error:
		strncpy(pThis->m_szErrMsg, (char*)lParam, sizeof(pThis->m_szErrMsg)-1);
		pThis->PostMessage(WM_SR_MSG_Error, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_Result:
		{
			// ???????,???MFC????????
			// \n -> \r\n
			int i_in=0;
			int j_out=0;
			const char* str_in = (const char*)lParam;
			char* str_out = pThis->m_szRlt;
			memset(pThis->m_szRlt, 0, sizeof(pThis->m_szRlt));
			while(j_out<(sizeof(pThis->m_szRlt)-1) && str_in[i_in]!=0) 
			{
				if(str_in[i_in]!='\n')
				{
					str_out[j_out++] = str_in[i_in];
				} 
				else
				{
					str_out[j_out++] = '\r';
					str_out[j_out++] = '\n';
				}
				i_in++;
			}
		}
		pThis->PostMessage(WM_SR_MSG_Result, (WPARAM)wParam, (LPARAM)lParam);
		break;
	case ISS_SR_MSG_LoadBigSrResStatus:
		//DEBUG_TRACE(L"ISS_SR_MSG_LoadBigSrResStatus wParam: %d.\r\n", wParam);
		break;
	default:
		//DEBUG_TRACE(L"Unhandled Msg: %d\r\n", uMsg);
		break;
	}
}
void CPanelSRMain::InWaveRecordData(int nID, const void* pData, int cbSize) 
{
	unsigned int NumWritten=0;
	ISSErrID err = ISSSRAppendAudioData(m_hIssSr, (short *)pData, cbSize>>1, &NumWritten);
	//CM_String str_log = L"";
	if(err) 
	{
		RETAILMSG(DEBUG_SR,(_T("##[SR]## ISSSRAppendAudioData error:%d\r\n"),err));
	} 
	else 
	{
		if(err == ISS_SUCCESS && NumWritten!=cbSize>>1) 
		{
			RETAILMSG(DEBUG_SR,(_T("##[SR]## ISSSRAppendAudioData Buffer Full\r\n")));
		}
	}
}

void CPanelSRMain::OnEndRecord()
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnEndRecord\r\n")));
	if(m_bIsRecording) 
	{        
		::SendMessage(this->m_pWaveRecorder->m_hWnd, WM_MY_MESSAGE_CLOSEPCM, 0, NULL);
		m_bIsRecording = false;
	}
}
void CPanelSRMain::OnSrEnd()
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnSrEnd\r\n")));
	m_btnStart.Show(TRUE);
	m_btnEndAudioData.Show(FALSE);

	m_bGettingResult = FALSE;
}
void CPanelSRMain::OnSrGettingResult()
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnSrGettingResult...\r\n")));
	m_btnStart.Show(FALSE);
	m_btnEndAudioData.Show(FALSE);

	m_bGettingResult = TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_InitStatus(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## InitStatus...:%d\r\n"),(int)wParam));

// 	CString str_log;
// 	str_log.Format(L"OnInitStatus: %d\r\n", (int)wParam);
// 	AppendStrToEdit(&edit_show_result, str_log);
	if(wParam == ISS_SUCCESS) 
	{
		m_btnStart.Show(TRUE);
		m_btnEndAudioData.Show(FALSE);

		m_tagSRResult.SetCaption(_T(""));
	}
	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnUpLoadDictToLocalStatus(WPARAM wParam, LPARAM lParam)
{
	return TRUE;

}
LRESULT CPanelSRMain::HandleOnMsgProc_OnUpLoadDictToCloudStatus(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnVolumeLevel(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnVolumeLevel:%d\r\n"),wParam));

	m_iSpeechVol = (wParam/50);

	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnResponseTimeout(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnResponseTimeout\r\n")));

	OnSrEnd();     // ????,????
	OnEndRecord(); // ????,????

	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnSpeechStart(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnSpeechStart\r\n")));

	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnSpeechTimeOut(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnSpeechTimeOut\r\n")));

	OnSrGettingResult();
	OnEndRecord();     // ????,????

	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnSpeechEnd(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnSpeechEnd\r\n")));

	OnSrGettingResult();
	OnEndRecord();     // ????????,????

	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnError(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnError ID=%d Msg=%ls\r\n"),(int)wParam, (const wchar_t*)_UTF82W(this->m_szErrMsg)));

	OnEndRecord();
	OnSrEnd();   // ????,????
	return TRUE;
}
LRESULT CPanelSRMain::HandleOnMsgProc_OnResult(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnResult\r\n")));

// 	CString str_log;
// 	if(strlen(this->m_szRlt)<=0) {
// 		str_log = L"No Result.\r\n";
// 	} else {
// 		str_log = (const wchar_t*)_UTF82W(this->m_szRlt);
// 		str_log += L"\r\n";
// 	}
// 	AppendStrToEdit(&edit_show_result, str_log);

	RETAILMSG(DEBUG_SR,(_T("##[SR]## OnResult:%s\r\n"),(const wchar_t*)_UTF82W(this->m_szRlt)));

	OnEndRecord();
	OnSrEnd();   // ??????,????

	//
	ParaResult();

	return TRUE;
}
void CPanelSRMain::ParaResult()
{
	RETAILMSG(DEBUG_SR,(_T("####### [SR ParaResult] ####### \r\n")));

	TXmlDocument doc;
	doc.Parse((const wchar_t*)_UTF82W(this->m_szRlt));

	TXmlElement *root = doc.RootElement();

	if (root == NULL)
	{
		return;
	}

	LPCTSTR pszName = root->Value();

	RETAILMSG(DEBUG_SR,(_T("##name: %s \r\n"),pszName));

	if (pszName == NULL)
	{
		return;
	}

	TXmlElement *element = root->ToElement();

	if(element == NULL)
	{
		return;
	}

	element = element->FirstChildElement(_T("rawtext"));

	if(element)
	{
		RETAILMSG(DEBUG_SR,(_T("##FirstChildElement: name %s \r\n"),element->Value()));

		TXmlNode *node = element->FirstChild();
		if(node)
		{
			RETAILMSG(DEBUG_SR,(_T("##%s: text %s \r\n"),element->Value(),node->Value()));
			m_tagSRResult.SetCaption(node->Value());		
		}
		else
		{
			RETAILMSG(DEBUG_SR,(_T("##%s: no text \r\n"),element->Value()));
		}

	}
}
void CPanelSRMain::SRStart()
{
	ISSErrID err;
	int iMode=ISS_SR_MODE_LOCAL_REC;
	const char* sz_scene=ISS_SR_SCENE_ALL;

	m_tagSRResult.SetCaption(_T(""));

// 	DEBUG_TRACE(L"Before ISSSRStart. iMode = %d", iMode);
// 	if(iMode==ISS_SR_MODE_LOCAL_CMDLIST) 
// 	{
// 		err = ISSSRStart(m_hIssSr, sz_scene, iMode, (const char*)_W2UTF8(g_str_cmd));
// 	} 
// 	else 
	{
		err = ISSSRStart(m_hIssSr, sz_scene, iMode, NULL);
	}
// 	DEBUG_TRACE(L"After ISSSRStart. iMode = %d", iMode);
// 	m_iMode = iMode;
// 	DEBUG_TRACE(L"ISSSRStart ret: %d", err);
// 	str_log.Format(L"ISSSRStart ret: %d\r\n", err);
// 	AppendStrToEdit(&edit_show_result, str_log);
	if(err==ISS_SUCCESS) 
	{
// 		this->button_start.SetWindowText(L"????");
// 		m_hButtonType = BUTTON_ENDAUDIO;
		m_btnEndAudioData.Show(TRUE);
		m_btnStart.Show(FALSE);

		if(!m_bIsRecording) 
		{
			this->m_pWaveRecorder->OpenPCM(16000, 0/*m_nDvcNum*/);  // ??ISSSRStart???,????
			m_bIsRecording = true;
		}
	}
}
void CPanelSRMain::SRStop()
{

}
void CPanelSRMain::SREndAudioData()
{
	ISSErrID err;

// 	DEBUG_TRACE(L"Before ISSSREndAudioData.");
	err = ISSSREndAudioData(m_hIssSr);
// 	DEBUG_TRACE(L"After ISSSREndAudioData.");
// 	DEBUG_TRACE(L"ISSSREndAudioData ret: %d", err);
// 	str_log.Format(L"ISSSREndAudioData ret: %d\r\n", err);
// 	AppendStrToEdit(&edit_show_result, str_log);
	if(err==ISS_SUCCESS)
	{
		this->OnSrGettingResult();
	} 
	else if(err==ISS_ERROR_NO_SPEECH) 
	{
		this->OnSrEnd();                    // ???????,????
// 		str_log = L"NoSpeech\r\n";
// 		AppendStrToEdit(&edit_show_result, str_log);
	}
	this->OnEndRecord();         // ????????
}