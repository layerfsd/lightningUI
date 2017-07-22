#ifndef _AK77XX_DSP2_ADDR_INCLUDED
#define _AK77XX_DSP2_ADDR_INCLUDED

// Component: Audio_Offset (fader_mul2_1)
#define 	CRAM_ADDR_LEVEL_Audio_Offset 	0x0000
#define 	CRAM_ADDR_ATT_TIME_Audio_Offset 	0x0001
#define 	CRAM_ADDR_REL_TIME_Audio_Offset 	0x0003

// Component: Radio_Offset (fader_mul2_2)
#define 	CRAM_ADDR_LEVEL_Radio_Offset 	0x0006
#define 	CRAM_ADDR_ATT_TIME_Radio_Offset 	0x0007
#define 	CRAM_ADDR_REL_TIME_Radio_Offset 	0x0009

// Component: SFADER_1 (fader_mul2_3)
#define 	CRAM_ADDR_LEVEL_SFADER_1 	0x000C
#define 	CRAM_ADDR_ATT_TIME_SFADER_1 	0x000D
#define 	CRAM_ADDR_REL_TIME_SFADER_1 	0x000F

// Component: SWITCH8in2out_1 (switch82_1)
#define 	CRAM_ADDR_SWITCH82_SWITCH8in2out_1 	0x0012

// Component: Headroom (fader_mul2_4)
#define 	CRAM_ADDR_LEVEL_Headroom 	0x0013
#define 	CRAM_ADDR_ATT_TIME_Headroom 	0x0014
#define 	CRAM_ADDR_REL_TIME_Headroom 	0x0016

// Component: Vol (fader_mul2_5)
#define 	CRAM_ADDR_LEVEL_Vol 	0x0019
#define 	CRAM_ADDR_ATT_TIME_Vol 	0x001A
#define 	CRAM_ADDR_REL_TIME_Vol 	0x001C

// Component: Loudness (u_Loudness_1)
#define 	CRAM_ADDR_BASS_FILTER_Loudness 	0x001F
#define 	CRAM_ADDR_BASS_WAIT_TIME_Loudness 	0x002E
#define 	CRAM_ADDR_BASS_MAX_GAIN_Loudness 	0x002F
#define 	CRAM_ADDR_BASS_OFFSET_Loudness 	0x0030
#define 	CRAM_ADDR_BASS_ATT_TIME_Loudness 	0x0031
#define 	CRAM_ADDR_BASS_REL_TIME_Loudness 	0x0033
#define 	CRAM_ADDR_TREBLE_FILTER_Loudness 	0x0029
#define 	CRAM_ADDR_TREBLE_WAIT_TIME_Loudness 	0x0035
#define 	CRAM_ADDR_TREBLE_MAX_GAIN_Loudness 	0x0036
#define 	CRAM_ADDR_TREBLE_OFFSET_Loudness 	0x0037
#define 	CRAM_ADDR_TREBLE_ATT_TIME_Loudness 	0x0038
#define 	CRAM_ADDR_TREBLE_REL_TIME_Loudness 	0x003A
#define 	CRAM_ADDR_LOUDNESS_ENABLE_Loudness 	0x0040

// Component: Bass_Mid_Tre (biquad2_1)
#define 	CRAM_ADDR_BAND1_Bass_Mid_Tre 	0x0041
#define 	CRAM_ADDR_BAND2_Bass_Mid_Tre 	0x004B
#define 	CRAM_ADDR_BAND3_Bass_Mid_Tre 	0x0055

// Component: GEQs (biquad2_2)
#define 	CRAM_ADDR_BAND1_GEQs 	0x005F
#define 	CRAM_ADDR_BAND2_GEQs 	0x0069
#define 	CRAM_ADDR_BAND3_GEQs 	0x0073
#define 	CRAM_ADDR_BAND4_GEQs 	0x007D
#define 	CRAM_ADDR_BAND5_GEQs 	0x0087
#define 	CRAM_ADDR_BAND6_GEQs 	0x0091
#define 	CRAM_ADDR_BAND7_GEQs 	0x009B
#define 	CRAM_ADDR_BAND8_GEQs 	0x00A5
#define 	CRAM_ADDR_BAND9_GEQs 	0x00AF
#define 	CRAM_ADDR_BAND10_GEQs 	0x00B9

// Component: FADER_RL (fader_mul_1)
#define 	CRAM_ADDR_LEVEL_FADER_RL 	0x00C3
#define 	CRAM_ADDR_ATT_TIME_FADER_RL 	0x00C4
#define 	CRAM_ADDR_REL_TIME_FADER_RL 	0x00C6

// Component: Delay_RL (delay.1_1)
#define 	ORAM_ADDR_WRITE_Delay_RL 	0x0000
#define 	ORAM_ADDR_READ1_Delay_RL 	0x0001

// Component: FADER_RR (fader_mul_2)
#define 	CRAM_ADDR_LEVEL_FADER_RR 	0x00C9
#define 	CRAM_ADDR_ATT_TIME_FADER_RR 	0x00CA
#define 	CRAM_ADDR_REL_TIME_FADER_RR 	0x00CC

// Component: Delay_RR (delay.1_2)
#define 	ORAM_ADDR_WRITE_Delay_RR 	0x0002
#define 	ORAM_ADDR_READ1_Delay_RR 	0x0003

// Component: Vol2 (fader_mul2_6)
#define 	CRAM_ADDR_LEVEL_Vol2 	0x00CF
#define 	CRAM_ADDR_ATT_TIME_Vol2 	0x00D0
#define 	CRAM_ADDR_REL_TIME_Vol2 	0x00D2

// Component: MuteOut5 (u_SoftMute_1)
#define 	CRAM_ADDR_MUTE_MuteOut5 	0x00D5
#define 	CRAM_ADDR_Ta_MuteOut5 	0x00D6
#define 	CRAM_ADDR_Tr_MuteOut5 	0x00D7

// Component: NoiseGate2_1 (noisegate2_1)
#define 	CRAM_ADDR_THRESHOLD_NoiseGate2_1 	0x00DB
#define 	CRAM_ADDR_ATT_TIME_NoiseGate2_1 	0x00DC
#define 	CRAM_ADDR_REL_TIME_NoiseGate2_1 	0x00DE

// Component: MIXER2_3 (mixerl2_1)
#define 	CRAM_ADDR_VOL_IN1_MIXER2_3 	0x00E0
#define 	CRAM_ADDR_VOL_IN2_MIXER2_3 	0x00E1

// Component: FADER_1 (fader_mul_3)
#define 	CRAM_ADDR_LEVEL_FADER_1 	0x00E2
#define 	CRAM_ADDR_ATT_TIME_FADER_1 	0x00E3
#define 	CRAM_ADDR_REL_TIME_FADER_1 	0x00E5

// Component: FADER_FL (fader_mul_4)
#define 	CRAM_ADDR_LEVEL_FADER_FL 	0x00E8
#define 	CRAM_ADDR_ATT_TIME_FADER_FL 	0x00E9
#define 	CRAM_ADDR_REL_TIME_FADER_FL 	0x00EB

// Component: Delay_FL (delay.1_3)
#define 	ORAM_ADDR_WRITE_Delay_FL 	0x0004
#define 	ORAM_ADDR_READ1_Delay_FL 	0x0005

// Component: MIXER2_1 (mixerl2_2)
#define 	CRAM_ADDR_VOL_IN1_MIXER2_1 	0x00EE
#define 	CRAM_ADDR_VOL_IN2_MIXER2_1 	0x00EF

// Component: FADER_FR (fader_mul_5)
#define 	CRAM_ADDR_LEVEL_FADER_FR 	0x00F0
#define 	CRAM_ADDR_ATT_TIME_FADER_FR 	0x00F1
#define 	CRAM_ADDR_REL_TIME_FADER_FR 	0x00F3

// Component: Delay_FR (delay.1_4)
#define 	ORAM_ADDR_WRITE_Delay_FR 	0x0006
#define 	ORAM_ADDR_READ1_Delay_FR 	0x0007

// Component: MIXER2_2 (mixerl2_3)
#define 	CRAM_ADDR_VOL_IN1_MIXER2_2 	0x00F6
#define 	CRAM_ADDR_VOL_IN2_MIXER2_2 	0x00F7

// Component: MuteOut2 (u_SoftMute_2)
#define 	CRAM_ADDR_MUTE_MuteOut2 	0x00F8
#define 	CRAM_ADDR_Ta_MuteOut2 	0x00F9
#define 	CRAM_ADDR_Tr_MuteOut2 	0x00FA

#endif //#end of _AK77XX_DSP_ADDR_INCLUDED
