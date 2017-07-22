#pragma once
#include <iss_sr.h>
#include "WaveRecorder.h"

#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"

#define		DEBUG_SR		1


class CPanelSRMain :
	public CPanelGL, IWaveRecordListener
{
public:
	CPanelSRMain(void);
	~CPanelSRMain(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	//
	void PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam);

	void InWaveRecordData(int nID, const void* pData, int cbSize);

	void HandleSRMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SRStart();
	void SRStop();
	void SREndAudioData();

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	BOOL InitializeSR();
	BOOL DeInitializeSR();

	static void ProcOnMsgProc(const void* pUsrArg,unsigned int uMsg, unsigned int wParam, const void* lParam);

	LRESULT HandleOnMsgProc_InitStatus(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnUpLoadDictToLocalStatus(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnUpLoadDictToCloudStatus(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnVolumeLevel(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnResponseTimeout(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnSpeechStart(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnSpeechTimeOut(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnSpeechEnd(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnError(WPARAM wParam, LPARAM lParam);
	LRESULT HandleOnMsgProc_OnResult(WPARAM wParam, LPARAM lParam);

	//
	void OnEndRecord();
	void OnSrEnd();
	void OnSrGettingResult();

	//
	void ParaResult();

private:

	CButtonGL	m_btnMic;
	CLabelGL		m_tagWave;
	CLabelGL		m_tagProcessing;

	CButtonGL	m_btnStart;
	CButtonGL	m_btnEndAudioData;
	CButtonGL	m_tagSRResult;

	//sr:
	BOOL	m_bSRini;
	HISSSR m_hIssSr;
	CWaveRecorder* m_pWaveRecorder;
	volatile bool m_bIsRecording;

	int m_iSpeechVol;
	BOOL	m_bGettingResult;

	char m_szDictName[1024];
	char m_szErrMsg[1024];
	char m_szRlt[10240];
};

