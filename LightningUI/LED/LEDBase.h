#pragma once

#include "LEDCommon.h"
#include "LEDEffect.h"
#include "CriticalSection.h"

#if CVTE_EN_LED

class CLEDBase
{
public:
	CLEDBase();
	~CLEDBase(void);

public:
	void SaveConfig();
	void RestoreConfig();

	void SetLedHSB(LEDHSB *pLedHSB, BOOL bTemp = FALSE);
	void SetLedRGB(LEDRGB *pLedRGB, BOOL bTemp = FALSE);
	void SetLedHue(float fHue, BOOL bTemp = FALSE);
	void SetLedBrightness(float fBrightness, BOOL bTemp = FALSE);
	void SetLedState(BYTE state);
	void SetLedShadeEnable(BOOL bEnable);
	void SetLedBreathEnable(BOOL bEnable);
	void SetLedShadeOn(DWORD dwControlFlag, BYTE nInstanceID,DWORD dwTime);
	void SetLedShadeOff(DWORD dwControlFlag, BYTE nInstanceID,DWORD dwTime);
	void SetLedOn(DWORD dwControlFlag, BYTE nInstanceID);
	void SetLedOff(DWORD dwControlFlag, BYTE nInstanceID);
	BOOL SetWorkMode(LEDMODE * pLedMode);
	BOOL SetGroupLed(LEDGROUPOPERATION * pLedGroupOperation);
	BOOL SetLed(LEDOPERATION * pLedOperation);

	BYTE GetLedState();
	BOOL GetLedShadeEnable();
	BOOL GetLedBreathEnable();
	BOOL GetLedShadeActivate() { return m_bLedShadeActivate;}
	PLEDRGB GetLedCurrentRGB();
	PLEDHSB GetLedCurrentHSB();	
	PLEDRGB GetLedSaveRGB();
	PLEDHSB GetLedSaveHSB();

	void LedShadeRun();

    virtual void TemplateMethod_SetLedParam(INT iDev,INT iLedn) = 0;
    virtual BOOL TemplateMethod_LedRefresh() = 0;
	virtual void TemplateMethod_LedMute() = 0;
	virtual void TemplateMethod_LedUnmute() = 0;

	virtual void TemplateMethod_LedAnimBreath() = 0;
	virtual void TemplateMethod_LedAnimShade() = 0;
	virtual void TemplateMethod_LedAnimFlash() = 0;
	virtual void TemplateMethod_LedAnimPowerOn() = 0;
	virtual void TemplateMethod_LedAnimPowerOff() = 0;
	virtual void TemplateMethod_LedAnimPowerIdle() = 0;
	
	void EnableAnimShade(BOOL bEnable, BOOL bForce = FALSE);
	void EnableAnimFlash(BOOL bEnable);
	void EnableAnimBreath(BOOL bEnable);
	void EnableAnimPowerOn();
	void EnableAnimPowerOff();
	void EnableAnimPowerIdle();

private:
	void ActionPowerOff();
	void ActionPowerOn();
	void ActionPowerIdle();
	void ActionMute();
	void ActionUnMute();

	void LedAnimationRun();
	static DWORD LedAnimationThread(LPVOID lpParam);

	void Lock();
	void UnLock();
	void SetLedAnimationType(BYTE animation) {m_nLedAnimationType = animation;}
	void SetLedAnimationTimeout(DWORD timeout) {m_dwAnimationTimeOut = timeout;}

private:
    CCriticalSection *m_pcsLED;
	BYTE		m_LedState;
	HANDLE		m_hDev;

	BOOL		m_bLedBreathEnable;
	BOOL		m_bLedShadeEnable;
	BOOL		m_bLedShadeActivate;
	BOOL		m_bLedFlashActivate;

	HANDLE		m_hLEDAntimationThead;
	HANDLE		m_hLedAnimationEvent;
	DWORD		m_dwAnimationTimeOut;
	BYTE		m_nLedAnimationType;

	LEDRGB		m_LedRGB;
	LEDHSB		m_LedHSB;
	LEDRGB		m_LedRGBSave;
	LEDHSB		m_LedHSBSave;
};

#endif
