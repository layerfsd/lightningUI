#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"

#include "../Controls/GridGL.h"

typedef enum _SUBWOOFERFREQID
{
	SUBWOOFER_FREQ_80HZ,
	SUBWOOFER_FREQ_120HZ,
	SUBWOOFER_FREQ_160HZ
};

class CPanelSettingAudio :
	public CPanelGL
{
public:
	CPanelSettingAudio(void);
	~CPanelSettingAudio(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);

	void OnVolumeConfigMsg(WPARAM wParam, LPARAM lParam);
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	void LinkAudioCodec(CAudioCodec *pAudioCodec);
#endif

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void UpdateAudioPara(AUDIO_SET_ITEM item,INT iSet);
	void ConfigKeyBeep(BOOL bOn);
    void ConfigRearVolControl(BOOL bOn);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	void SetEQSwitch(BOOL bOn);
	void SetEQLocked(BOOL bOn);
#endif

#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
	void UpdateSubWooferFreq(UINT iSet);
#endif
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	BOOL RestoreEQSetting();
	static void OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
#endif

private:
    BOOL        m_bUpdateTreble;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	BOOL		m_bEQLock;
#endif
	//
// 	CButtonGL	m_tagTitleMute;
// 	CSliderGL		m_sliMute;
	CButtonGL	m_tagTitleKeyBeep;
	CSliderGL		m_sliKeyBeep;

	//
	CButtonGL	m_tagTitleLoudness;
	CSliderGL		m_sliLoudness;

    //
	CButtonGL	m_tagTitleRearVolControl;
	CSliderGL		m_sliRearVolControl;

	//
	CButtonGL	m_tagTitleSubwoofer;
	CSliderGL		m_sliSubwoofer;

	CButtonGL	m_btnSubwooferFreq;
	CButtonGL	m_tagValueSubwooferFreq;

	//
	CButtonGL	m_tagTitleVolumeAdaption;
	CSliderGL		m_sliVolumeAdaption;

	//volume:
	CButtonGL	m_tagTitleVolume;
	CButtonGL	m_tagTitleVolumeMultiMedia;
	CSliderGL		m_sliVolumeMultiMedia;
	CButtonGL	m_tagTitleVolumeNavi;
	CSliderGL		m_sliVolumeNavi;
	CButtonGL	m_tagTitleVolumePhone;
	CSliderGL		m_sliVolumePhone;

	//
	CButtonGL	m_tagTitleAudioEffect;
	CButtonGL	m_tagAudioEffectBack;

	CButtonGL	m_tagTitleEqualizer;
	CButtonGL	m_tagTitleBalance;

	CButtonGL	m_tagEqualizerBack;
	CButtonGL	m_tagBalanceBack;

	CButtonGL	m_tagValueBalance;
	CGridGL		m_barBalance;


	//eq
	CButtonGL	m_tagTitleHigh;
	CButtonGL	m_tagTitleMid;
	CButtonGL	m_tagTitleLow;
 	CButtonGL	m_tagValueHigh;
 	CButtonGL	m_tagValueMid;
 	CButtonGL	m_tagValueLow;

	CButtonGL	m_btnEQUser;
	CButtonGL	m_btnEQClassic;
	CButtonGL	m_btnEQJazz;
	CButtonGL	m_btnEQPop;
	CButtonGL	m_btnEQRock;

	CSliderGL		m_sliHigh;
	CSliderGL		m_sliMid;
	CSliderGL		m_sliLow;	
	
#if (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL)
	CButtonGL	m_tagTitleInternalAMP;
	CSliderGL		m_sliInternalAMP;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	CButtonGL   m_tagControlEQ;
	CSliderGL   m_sliEQBalFadConctrol;
#if 0
	CButtonGL   m_tagEQLock;
	CLabelGL    m_iconLock;
	CLabelGL    m_iconUnLock;
	CButtonGL   m_btnLock;
#endif
#endif

	int	m_iCreatePageSizeY;
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	CAudioCodec *m_pAudioCodec;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif
};

