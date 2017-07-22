#ifndef __RENESAS_COM_H__
#define __RENESAS_COM_H__

//SYS   TIME
#define MCU_CLOCK_ADJUST_HOUR			0X00
#define MCU_CLOCK_ADJUST_MIN		    	0X01
#define MCU_CLOCK_ADJUST_SEC				0X02
//SYS    INPUT
#define MCU_CURRENT_SOURCE				0X03
//	0:RADIO_INPUT	1:USB_INPUT		2:SD_INPUT		3:DVD_INPUT		4:IPOD_INPUT
//	5:TV_INPUT		6:GPS_INPUT		7:AUX1_INPUT	8:AUX2_INPUT	9:BT_INPUT
//	0XFF:NONE
#define MCU_PIP_SOURCE							0X04
//	0:USB_INPUT		1:SD_INPUT		2:DVD_INPUT		3:IPOD_INPUT	4:TV_INPUT
//	5:GPS_INPUT		6:AUX1_INPUT	7:AUX2_INPUT
//	0XFF:NONE

//SYS    STATE
#define MCU_POWER_MODE					0X05
#define MCU_BRAKE_STATE        			0X06
#define MCU_REVERSE_STATE      			0X07
#define MCU_MAIN_VERSION				0X08
//SYS    INFO
#define MCU_SYS_INFO_TITLE				0X09
#define MCU_TITLE_ASCALL_CODE		0X0A
//0X0B~0X0D		RESERVE
//SYS    SET
#define MCU_KEY_BEEP							0X0E
#define MCU_RESERVE_POLA					0X0F
#define MCU_LOCAL_AREA					0X10
#define MCU_GOTO_FREQ						0X11
#define MCU_AMS_FULLUP					0X12
#define MCU_ST									0X13
#define MCU_LOC									0X14
#define MCU_TIME_MODE						0X15
#define MCU_BREAKE_DETECT				0X16
#define MCU_LIGHT_CONTROL				0X17
#define MCU_REVERSE_POWER_ON			0X19

#define MCU_AUX_IN_DETECT				0X1A

//0X18~0X1C		RESERVE
//SYS    INPUT    ON/OFF
#define MCU_AUDIO_SUBWOOFER_FREQ        0X24
#define MCU_AUDIO_MID					0X25
#define MCU_AUDIO_INTERNAL_AMP 0X2A
#define MCU_AUDIO_VOLUME				0X2D
#define MCU_AUDIO_BALANCE				0X2E
#define MCU_AUDIO_FAD						0X2F
#define MCU_AUDIO_BASS					0X30
#define MCU_AUDIO_TREBLE					0X31
#define MCU_AUDIO_SUBWOOFER		0X32
#define MCU_AUDIO_EQ						0X33
#define MCU_AUDIO_LOUD					0X34
#define MCU_MUTE_MODE					0X35

//0X36~0X38		RESERVE
//RADIO
#define MCU_RADIO_FM_STOP_LEVEL					0X36
#define MCU_RADIO_AM_STOP_LEVEL					0X37
#define MCU_CUR_CHANNEL_LEVEL			0X38
#define MCU_FREQ_STORE_INDEX			0X39
#define MCU_FREQ_STORE_H					0X3A
#define MCU_FREQ_STORE_L					0X3B
#define MCU_RADIO_PLAY_STATE				0X3C

#define MCU_RADIO_FM_CNT					0X3D
#define MCU_RADIO_AM_CNT					0X3E
#define MCU_RADIO_FREQ_QUERY_H		0X3F
#define MCU_RADIO_FREQ_QUERY_L		0X40
#define MCU_RADIO_MIX_VOLUME        0X41

//
#define MCU_HW_MUTE                 0X42


/*
#define MCU_FM1_FREQU_STORE_1_H		0X39
#define MCU_FM1_FREQU_STORE_1_L		0X3A
#define MCU_FM1_FREQU_STORE_2_H		0X3B
#define MCU_FM1_FREQU_STORE_2_L		0X3C
#define MCU_FM1_FREQU_STORE_3_H		0X3D
#define MCU_FM1_FREQU_STORE_3_L		0X3E
#define MCU_FM1_FREQU_STORE_4_H		0X3F
#define MCU_FM1_FREQU_STORE_4_L		0X40
#define MCU_FM1_FREQU_STORE_5_H		0X41
#define MCU_FM1_FREQU_STORE_5_L		0X42
#define MCU_FM1_FREQU_STORE_6_H		0X43
#define MCU_FM1_FREQU_STORE_6_L		0X44
#define MCU_FM2_FREQU_STORE_1_H		0X45
#define MCU_FM2_FREQU_STORE_1_L		0X46
#define MCU_FM2_FREQU_STORE_2_H		0X47
#define MCU_FM2_FREQU_STORE_2_L		0X48
#define MCU_FM2_FREQU_STORE_3_H		0X49
#define MCU_FM2_FREQU_STORE_3_L		0X4A
#define MCU_FM2_FREQU_STORE_4_H		0X4B
#define MCU_FM2_FREQU_STORE_4_L		0X4C
#define MCU_FM2_FREQU_STORE_5_H		0X4D
#define MCU_FM2_FREQU_STORE_5_L		0X4E
#define MCU_FM2_FREQU_STORE_6_H		0X4F
#define MCU_FM2_FREQU_STORE_6_L		0X50
#define MCU_FM3_FREQU_STORE_1_H		0X51
#define MCU_FM3_FREQU_STORE_1_L		0X52
#define MCU_FM3_FREQU_STORE_2_H		0X53
#define MCU_FM3_FREQU_STORE_2_L		0X54
#define MCU_FM3_FREQU_STORE_3_H		0X55
#define MCU_FM3_FREQU_STORE_3_L		0X56
#define MCU_FM3_FREQU_STORE_4_H		0X57
#define MCU_FM3_FREQU_STORE_4_L		0X58
#define MCU_FM3_FREQU_STORE_5_H		0X59
#define MCU_FM3_FREQU_STORE_5_L		0X5A
#define MCU_FM3_FREQU_STORE_6_H		0X5B
#define MCU_FM3_FREQU_STORE_6_L		0X5C
#define MCU_FM4_FREQU_STORE_1_H		0X5D
#define MCU_FM4_FREQU_STORE_1_L		0X5E
#define MCU_FM4_FREQU_STORE_2_H		0X5F
#define MCU_FM4_FREQU_STORE_2_L		0X60
#define MCU_FM4_FREQU_STORE_3_H		0X61
#define MCU_FM4_FREQU_STORE_3_L		0X62
#define MCU_FM4_FREQU_STORE_4_H		0X63
#define MCU_FM4_FREQU_STORE_4_L		0X64
#define MCU_FM4_FREQU_STORE_5_H		0X65
#define MCU_FM4_FREQU_STORE_5_L		0X66
#define MCU_FM4_FREQU_STORE_6_H		0X67
#define MCU_FM4_FREQU_STORE_6_L		0X68
#define MCU_AM1_FREQU_STORE_1_H		0X69
#define MCU_AM1_FREQU_STORE_1_L		0X6A
#define MCU_AM1_FREQU_STORE_2_H		0X6B
#define MCU_AM1_FREQU_STORE_2_L		0X6C
#define MCU_AM1_FREQU_STORE_3_H		0X6D
#define MCU_AM1_FREQU_STORE_3_L		0X6E
#define MCU_AM1_FREQU_STORE_4_H		0X6F
#define MCU_AM1_FREQU_STORE_4_L		0X70
#define MCU_AM1_FREQU_STORE_5_H		0X71
#define MCU_AM1_FREQU_STORE_5_L		0X72
#define MCU_AM1_FREQU_STORE_6_H		0X73
#define MCU_AM1_FREQU_STORE_6_L		0X74
#define MCU_AM2_FREQU_STORE_1_H		0X75
#define MCU_AM2_FREQU_STORE_1_L		0X76
#define MCU_AM2_FREQU_STORE_2_H		0X77
#define MCU_AM2_FREQU_STORE_2_L		0X78
#define MCU_AM2_FREQU_STORE_3_H		0X79
#define MCU_AM2_FREQU_STORE_3_L		0X7A
#define MCU_AM2_FREQU_STORE_4_H		0X7B
#define MCU_AM2_FREQU_STORE_4_L		0X7C
#define MCU_AM2_FREQU_STORE_5_H		0X7D
#define MCU_AM2_FREQU_STORE_5_L		0X7E
#define MCU_AM2_FREQU_STORE_6_H		0X7F
#define MCU_AM2_FREQU_STORE_6_L		0X80
*/
#define MCU_CUR_BAND_STORE				0X81
#define MCU_CUR_CHANNEL_STORE			0X82
#define MCU_CUR_FREQU_STORE_H			0X83
#define MCU_CUR_FREQU_STORE_L			0X84
#define MCU_CUR_FREQU_LIST				0X85


//0X85~0X87		RESERVE
//RDS
#define MCU_RDS_STATE					0X88
#define MCU_RDS_AF						0X89
#define MCU_RDS_TA						0X8A
#define MCU_RDS_TP						0X8B
#define MCU_RDS_EON						0X8C
#define MCU_RDS_PTY						0X8D
#define MCU_RDS_PTY_CODE_CUR			0X8E
#define MCU_RDS_PS						0X8F
#define MCU_RDS_RT						0X90
#define MCU_RDS_TDC					0X91
#define MCU_RDS_STR					0X92
#define MCU_RDS_RP					0X93
#define MCU_RADIO_SIGNAL_STATUS			0X94
#define MCU_RADIO_SIGNAL_DATA				0X95
#define MCU_RDS_BLOCK_DATA			0X96
#define MCU_RADIO_QUALITY				0X97
#define MCU_RDS_STATUS					0X98
#define MCU_RDS_STATUS_DATA					0X99

//
#define MCU_AUDIO_CODEC_MUTE							0X9D
#define MCU_AUDIO_CODEC_MUTE_FADE				0X9E
#define MCU_AUDIO_CODEC_ACK							0X9F

//0XA0~0XA2		RESERVE
//BLUETOOTH
#define MCU_BT_STATE						0XA3
#define MCU_BT_DIAL_NUMBER		0XA4
#define MCU_BT_RING_NUMBER		0XA5
#define MCU_BT_MENU						0XA6
//0XA7~0XA9		RESERVE
//VIDEO
#define MCU_VIDEO_BRIGHT				0XAA
#define MCU_VIDEO_SHARP				0XAB
#define MCU_VIDEO_CONTRAST		0XAC
#define MCU_VIDEO_SAT_U				0XAD
#define MCU_VIDEO_SAT_V				0XAE
#define MCU_VIDEO_HUE					0XAF
#define MCU_VIDEO_SAVE				0XB0

#define MCU_KEY_STUDY_RESISTANCE	0XB4
#define MCU_SCRN_BRIGHTNESS		0XB5
#define MCU_KEY						0XB6

#define MCU_CHANGE_STW_KEY			0XB7
#define MCU_STW_VALUE_KEY			0XB8
#define MCU_BACKLIGHT_CONTROL_KEY   0XB9
#define MCU_STW_CMD_KEY             0XBA

#define MCU_UPDATE_STATUS			0XFF

//0XB1~0XB2		RESERVE

//Audio SRC
#define SRC_AUDIO_INVALID			0xFF
#define SRC_AUDIO_RADIO			0
#define SRC_AUDIO_ARM				1
#define SRC_AUDIO_BT_HF			7/*2*/
#define SRC_AUDIO_SPEECH		3
#define SRC_AUDIO_TV				4
#define SRC_AUDIO_AUX1			5
#define SRC_AUDIO_AUX2			6
#define SRC_AUDIO_DAB          7
#define SRC_AUDIO_MHL			7   //SRC_AUDIO_DAB
#define SRC_AUDIO_DVD			2
#define SRC_AUDIO_KEY_BEEP		9
#define SRC_AUDIO_ALERT			10
#define SRC_AUDIO_PHONE			11
#define SRC_AUDIO_SR					12

//Video SRC
#define SRC_VIDEO_RADIO			0
#define SRC_VIDEO_ARM				1
#define SRC_VIDEO_DVD				2
#define SRC_VIDEO_GPS				3
#define SRC_VIDEO_TV					4
#define SRC_VIDEO_AUX1				5
#define SRC_VIDEO_AUX2				6
#define SRC_VIDEO_MHL				7
#define SRC_VIDEO_CAMERA		8

#define SRC_MENU			0xFE
#define SRC_NONE			0xFF

//Extensible CMD
#define MCU_EXTEN_TIME_DATA		0x10
#define MCU_EXTEN_LANGUAGE		0x11
#define MCU_EXTEN_SWCOMMAND		0x12

//Camera
#define CAMERA_POWER_ON     1
#define CAMERA_POWER_OFF    0

#endif