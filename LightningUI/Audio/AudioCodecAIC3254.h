#pragma once

#include "AudioCodec.h"
#include "AIC3254_cmd.h"
#include "../../Communication/CmdSender_AudioCodec.h"


#define ADC_IN1_NC		0x00
#define ADC_IN1_10K		0x40
#define ADC_IN1_20K		0x80
#define ADC_IN1_40K		0xC0
#define ADC_IN2_NC		0x00
#define ADC_IN2_10K		0x10
#define ADC_IN2_20K		0x20
#define ADC_IN2_40K		0x30
#define ADC_IN3_NC		0x00
#define ADC_IN3_10K		0x04
#define ADC_IN3_20K		0x08
#define ADC_IN3_40K		0x0C


enum TBQType {bqEQ, bqHighPass, bqLowPass, bqTrebleShelf, bqBassShelf, bqPhaseShift, bqNotch};
enum TBQSubType {bqButterworth1, bqButterworth2, bqBessel2, bqLinkwitzRiley2, bqVariableQ2};
#define n_pi 3.1415926535897932384626433832795

class TBiquad
{
public:
	TBiquad(BYTE nI2CPageLeft, BYTE nI2CRegisterLeft,BYTE nI2CPageRight, BYTE nI2CRegisterRight):
	  mnI2CPageLeft(nI2CPageLeft),
		  mnI2CRegisterLeft(nI2CRegisterLeft),
		  mnI2CPageRight(nI2CPageRight),
		  mnI2CRegisterRight(nI2CRegisterRight),
		  mnScale(1.0),
		  mnResolution(24), mbQuantize(true), mbEnabled(true),
		  mbInvertForEVM(false),
		  mBQType(bqEQ), mBQSubType(bqButterworth1),
		  mpI2CData(new unsigned char[512]),
		  mnI2CDataLen(0){};
	  virtual void Update(void){};

	  void ReCalc(void){Calc(mnFC, mnBW, mnFS, mnG/*, mnQ*/);};
	  void Calc(double FC, double BW, double FS, double G, bool bRefresh = false);
	  void CalcEQ(double FC, double BW, double FS, double G, bool bRefresh = false);
	  //void CalcBW1(double FC, double FS, double G, bool bHP, bool bRefresh);
	  void CalcVQ2(double FC, double Q, double FS, double G, bool bHP, bool bRefresh);
	  void CalcLR2(double FC, double FS, double G, bool bHP, bool bRefresh);
	  void CalcNotch(double FC, double BW, double FS, double G, bool bRefresh);
	  void CalcShelf(double FC, double FS, double G, bool bTreble, bool bRefresh);
	  void CalcPhaseShift(double FC, double BW, double FS, bool bRefresh);

	  void Enabled(bool bEnabled);
	  bool Enabled(void){return mbEnabled;};

	  double Gain(double nF);
	  double Phase(double nF);
	  void H(double nF, double &nG, double &nP);
	  double Scale(){return mnScale;};

	  void SetControlPointer(unsigned char *pControl)
	  {mpControl = pControl;};

	  void SetResolution(unsigned int nResolution);
	  void Quantize(void);
	  // 	  AnsiString Description(void);
	  // 	  AnsiString Coefficients(void);
	  unsigned char* GetI2CData(){return mpI2CData;}
	  unsigned int GetI2CDataLength(){return mnI2CDataLen;}

	  double mnFC;
	  double mnG;
	  double mnBW;
	  double mnQ;
	  double mnFS;
	  double mnDummy[16];
	  double mnBWP;
	  double ma[3];
	  double mb[3];
	  double maQ[3];
	  double mbQ[3];
	  long mN[3];
	  long mD[3];

	  unsigned int mnResolution;
	  bool mbQuantize;

	  TBQType mBQType;
	  TBQSubType mBQSubType;

	  bool mbInvertForEVM;

private:
	double mnScale;
	unsigned char *mpControl;
	bool mbEnabled;

	unsigned char *mpI2CData;
	unsigned int mnI2CDataLen; 

	BYTE mnI2CPageLeft; 
	BYTE mnI2CRegisterLeft;
	BYTE mnI2CPageRight; 
	BYTE mnI2CRegisterRight;
};



typedef struct _InputRouting
{
	BOOL	ADC_IN1; //for aux/dvd/dtv
	BOOL	ADC_IN2; //for radio
	BOOL	ADC_IN3; //for navigation
	BOOL	DAC_IN; //for ARM I2S

}InputRouting;

typedef struct _OutputRouting
{
	BOOL MA;
	BOOL DAC;
}OutputRouting;

class CAudioCodecAIC3254 :
	public CAudioCodec
{
public:
	CAudioCodecAIC3254(CCommanderEx *pCommander,HWND hWnd,AspType aspType=AspTypeNull);
	~CAudioCodecAIC3254(void);

protected:
	BOOL codec_Ini();
	BOOL codec_IsCodecConfigFinished();
	BOOL codec_MuteAMP(BOOL bMute);
	BOOL codec_MuteCodec(BOOL bMute);
	BOOL codec_SetMainVol(int vol);//-127 -> + 48 : -63.5db-+24db
	BOOL codec_SetAltVol(int vol);//117->0: 117=mute,116 = -72.3db, 0=0db
	BOOL codec_MuteRearSpeakers(BOOL bMute);
	BOOL codec_SetAGC(BOOL bEnbale);
	BOOL codec_SetSampleRate(int iSampleRate);
	BOOL codec_SetMicVolume(int vol);
	BOOL codec_SetEQBand(int iBand,int val);
	BOOL codec_SetEQPreset(BYTE idPresetType);
	BOOL codec_SetBalFad(BYTE bal,BYTE fad); //0-14
	BOOL codec_Set3DEffect(BYTE val); //0-10
	BOOL codec_SetBalance(BYTE bal); //0-14
	BOOL codec_SetFade(BYTE fad); //0-14
	BOOL codec_SelectAudioSrc(UINT idSrc);
	BOOL codec_SetLoudness(BOOL bOn);
	BOOL codec_SetSubwoofer(BYTE val); //0-14
	BOOL codec_SetBass(BYTE val); //0-14
	BOOL codec_SetMid(BYTE val); //0-14
	BOOL codec_SetTreble(BYTE val); //0-14
	BOOL codec_SetMixerOn(BOOL bOn, BOOL bMuteMain);
	BOOL codec_ChangeWorkMode(AspMode mode);
	BOOL codec_SetHFStatus(AspHFStatus status);
private:
	//for TI AIC3254
	BOOL ConfigAudioCodecAIC3254SingleLine(UINT idType,BYTE *buff, INT len,INT iDelayMs=0);
	BOOL ConfigAudioCodecAIC3254Pack(BYTE *buffScript, INT len);

	BOOL IsSrcNeedMic(UINT id);
	void UpdateInputRouting();
	void UpdateOutputRouting();

private:

	CmdSender_AudioCodec	*m_pCmdSender;

	InputRouting	m_inputRouting;
	OutputRouting		m_outputRouting;

	TBiquad *m_pBiquad[5];
};

