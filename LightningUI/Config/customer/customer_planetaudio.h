/******************************************************************************

  Copyright (C) 2015-2016 CVTE. All Rights Reserved.

 ******************************************************************************
  File Name     : customer_planetaudio.h
  Version       : Initial Draft
  Author        : zhaoshunxian
  Created       : 2016/7/4
  Last Modified :
  Description   : customer_planetaudio's configs file
  Function List :
  History       :
  1.Date        : 2016/11/28
    Author      : wuhaoheng
    Modification: Created file

******************************************************************************/

#ifndef __CUSTOMER_PLANETAUDIO_H__
#define __CUSTOMER_PLANETAUDIO_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/******************************************************************************/
/*************************      ģ������ѡ��    *******************************/
/******************************************************************************/
#define CVTE_MODEL_TYPE			CVTE_MODEL_TYPE_U701P_E_ASP

/******************************************************************************/
/*************************     UI�������ѡ��   *******************************/
/******************************************************************************/
#define CVTE_DEF_UI_STYLE	CVTE_UI_STYLE_BOSSAUDIOUI

/******************************************************************************/
/*************************      ��Ҫģ��ʹ��    *******************************/
/******************************************************************************/
#define CVTE_EN_CARPLAY			CVTE_ENABLE

#define CVTE_EN_IPOD			CVTE_ENABLE

#define CVTE_EN_LED			    CVTE_ENABLE

#define CVTE_EN_RADIO			CVTE_ENABLE

#define CVTE_EN_EASYCONNECT		CVTE_DISABLE

#define CVTE_EN_ANDROID_AUTO	CVTE_DISABLE

#define CVTE_EN_BT_PHONE		CVTE_ENABLE

#define CVTE_EN_BT_A2DP			CVTE_ENABLE

#define CVTE_EN_NAVI			CVTE_DISABLE

#define CVTE_EN_USB			    CVTE_ENABLE

#define CVTE_EN_SD				CVTE_DISABLE

#define CVTE_EN_DVD		        CVTE_DISABLE

#define CVTE_EN_DTV		        CVTE_DISABLE

#define CVTE_EN_AUX		        CVTE_ENABLE

#define CVTE_EN_RDS				CVTE_ENABLE

#define CVTE_EN_RBDS			CVTE_ENABLE

#define CVTE_EN_RDS_TMC			CVTE_DISABLE

#define CVTE_EN_DAB				CVTE_DISABLE

#define CVTE_EN_CAMERA			CVTE_ENABLE

#define CVTE_EN_DSP             CVTE_ENABLE

/******************************************************************************/
/*************************       ��������       *******************************/
/******************************************************************************/
#define CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX	CVTE_DISABLE

#define CVTE_DEF_AUDIO_SUPPORT_FORMAT	( CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WAV \
										| CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP3 \
										| CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WMA \
										| CVTE_AUDIO_SUPPORT_FORMAT_TYPE_APE \
										| CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLA \
										| CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLAC \
										| CVTE_AUDIO_SUPPORT_FORMAT_TYPE_OGG )

#define CVTE_DEF_VIDEO_SUPPORT_FORMAT	( CVTE_VIDEO_SUPPORT_FORMAT_TYPE_AVI \
										| CVTE_VIDEO_SUPPORT_FORMAT_TYPE_OGM \
										| CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MKV \
										| CVTE_VIDEO_SUPPORT_FORMAT_TYPE_M4V \
										| CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MOV \
										| CVTE_VIDEO_SUPPORT_FORMAT_TYPE_WMV )

#define CVTE_DEF_LANGUAGE_SUPPORT		( CVTE_LANGUAGE_SUPPORT_ENGLISH \
										| CVTE_LANGUAGE_SUPPORT_SPANISH )

#define CVTE_DEF_RADIO_REGION_SUPPORT		( CVTE_RADIO_REGION_SUPPORT_EUROPE \
										| CVTE_RADIO_REGION_SUPPORT_USA \
										| CVTE_RADIO_REGION_SUPPORT_USA2)

/******************************************************************************/
/*************************     ��������ʹ��     *******************************/
/******************************************************************************/
//Steer wheel old ui style
#define CVTE_EN_STEER_WHEEL_OLD_UI_STYLE		CVTE_ENABLE

//Reverse volume auto adjustment
#define CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT		CVTE_DISABLE

//Navi smart drop bar auto show
#define CVTE_EN_NAVI_DROP_BAR					CVTE_DISABLE

//Dynamic logo show
#define CVTE_EN_DYNAMIC_LOGO					CVTE_DISABLE

//Waring page show after ACC ON
#define CVTE_EN_ACCON_WARNING					CVTE_ENABLE     //��������

//Home widget anim
#define CVTE_EN_HOME_WIDGET_ANIM				CVTE_DISABLE	//��ҳwidget�������

//Customer status bar
#define CVTE_CUSTOMER_STATUS_BAR_LOGO			CVTE_ENABLE

//Subwoofer freq adujst
#define CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST		CVTE_ENABLE

//internal AMP control
#define CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL		CVTE_ENABLE

//OS Dynamic logo
#define CVTE_EN_OS_DYNAMIC_LOGO					CVTE_ENABLE

//BT Calllog download permit
#define CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT		CVTE_ENABLE

//CarPlay LimitUI
#define CVTE_EN_CARPLAY_LIMIT_UI				CVTE_DISABLE

//BT VCard name order
#define CVTE_EN_BT_VCARD_NAME_ORDER				CVTE_ENABLE

//FULL keyboard search:
#define CVTE_EN_SEARCH_USE_FULL_KEYBOARD		CVTE_ENABLE

//Save Sector for CA
#define CVTE_EN_CPCA							CVTE_ENABLE

//Save Sector for MAC
#define CVTE_EN_CPMAC							CVTE_ENABLE

//Idle Logo
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_EN_IDLE_LOGO						CVTE_DISABLE

//��������뵯����ʾ
#define CVTE_EN_BT_PAIRED_CODE_PERMIT			CVTE_ENABLE

//slide gesture play control: NORMAL = from LEFT to right will play previous one.
#define CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL		CVTE_ENABLE

/******************************************************************************/
/**************************    ��ҳwidget����   *******************************/
/******************************************************************************/
//Panel Width
#define CVTE_DEF_HOME_WIDGET_WIDTH  (CVTE_DEF_SCREEN_VIDEOWIDTH)

/**
* MEDIA WIDGET
*/
#define CVTE_DEF_MEDIA_WIDGET_CX    252//(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_MEDIA_WIDGET_CY    252//(CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#define CVTE_DEF_MEDIA_WIDGET_PX    -8//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_MEDIA_WIDGET_PY    64//CVTE_DEF_HOME_WIDGET_UINT_PY
#define CVTE_DEF_MEDIA_WIDGET_VISIBLE    CVTE_ENABLE

/**
* RADIO WIDGET
*/
#if CVTE_EN_ANDROID_AUTO
#define CVTE_DEF_RADIO_WIDGET_CX    252
#define CVTE_DEF_RADIO_WIDGET_CY    168
#define CVTE_DEF_RADIO_WIDGET_PX    -262
#define CVTE_DEF_RADIO_WIDGET_PY    -148
#define CVTE_DEF_RADIO_WIDGET_VISIBLE    CVTE_ENABLE
#else
#define CVTE_DEF_RADIO_WIDGET_CX    384//(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_RADIO_WIDGET_CY    168//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_RADIO_WIDGET_PX    -196//CVTE_DEF_HOME_WIDGET_UINT_PX
#define CVTE_DEF_RADIO_WIDGET_PY    -148//(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_RADIO_WIDGET_VISIBLE    CVTE_ENABLE
#endif

/**
* CARPLAY WIDGET
*/
#define CVTE_DEF_CARPLAY_WIDGET_CX    252//(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_CARPLAY_WIDGET_CY    252//(CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#define CVTE_DEF_CARPLAY_WIDGET_PX    -262//CVTE_DEF_HOME_WIDGET_UINT_PX 
#define CVTE_DEF_CARPLAY_WIDGET_PY    64//CVTE_DEF_HOME_WIDGET_UINT_PY
#define CVTE_DEF_CARPLAY_WIDGET_VISIBLE    CVTE_ENABLE

/**
* IPOD WIDGET
*/
#define CVTE_DEF_IPOD_WIDGET_CX    252//(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_IPOD_WIDGET_CY    252//(CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#define CVTE_DEF_IPOD_WIDGET_PX    -262//CVTE_DEF_HOME_WIDGET_UINT_PX 
#define CVTE_DEF_IPOD_WIDGET_PY    64//CVTE_DEF_HOME_WIDGET_UINT_PY
#define CVTE_DEF_IPOD_WIDGET_VISIBLE    CVTE_DISABLE

/**
* ANDROID AUTO WIDGET
*/
#define CVTE_DEF_ANDROID_AUTO_WIDGET_CX    192
#define CVTE_DEF_ANDROID_AUTO_WIDGET_CY    168
#define CVTE_DEF_ANDROID_AUTO_WIDGET_PX    -38
#define CVTE_DEF_ANDROID_AUTO_WIDGET_PY    -148
#define CVTE_DEF_ANDROID_AUTO_WIDGET_VISIBLE    CVTE_EN_ANDROID_AUTO

/**
* USB WIDGET
*/
#define CVTE_DEF_USB_WIDGET_CX    142//(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_USB_WIDGET_CY    92//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_USB_WIDGET_PX    178//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_USB_WIDGET_PY    64//(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_USB_WIDGET_VISIBLE    CVTE_ENABLE

/**
* BT PHONE WIDGET
*/
#if CVTE_EN_ANDROID_AUTO
#define CVTE_DEF_BT_PHONE_WIDGET_CX    192
#define CVTE_DEF_BT_PHONE_WIDGET_CY    168
#define CVTE_DEF_BT_PHONE_WIDGET_PX    153
#define CVTE_DEF_BT_PHONE_WIDGET_PY    -148
#define CVTE_DEF_BT_PHONE_WIDGET_VISIBLE    CVTE_ENABLE
#else
#define CVTE_DEF_BT_PHONE_WIDGET_CX    252//(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_BT_PHONE_WIDGET_CY    168//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_BT_PHONE_WIDGET_PX    123//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4)
#define CVTE_DEF_BT_PHONE_WIDGET_PY    -148//(CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_BT_PHONE_WIDGET_VISIBLE    CVTE_ENABLE
#endif

/**
* NAVI WIDGET
*/
#define CVTE_DEF_NAVI_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_NAVI_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_NAVI_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4-CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_NAVI_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_NAVI_WIDGET_VISIBLE    CVTE_DISABLE

/**
* BT A2DP WIDGET
*/
#define CVTE_DEF_BT_A2DP_WIDGET_CX    142//(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_BT_A2DP_WIDGET_CY    90//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_BT_A2DP_WIDGET_PX    178//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4-CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_BT_A2DP_WIDGET_PY    145//(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_BT_A2DP_WIDGET_VISIBLE    CVTE_ENABLE

/**
* CAMERA WIDGET
*/
#define CVTE_DEF_CAMERA_WIDGET_CX    138//(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_CAMERA_WIDGET_CY    252//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_CAMERA_WIDGET_PX    319//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_CAMERA_WIDGET_PY    64//(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_CAMERA_WIDGET_VISIBLE    CVTE_ENABLE


/**
* AUX WIDGET
*/
#define CVTE_DEF_AUX_WIDGET_CX    142//(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_AUX_WIDGET_CY    90//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_AUX_WIDGET_PX    178//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4-CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_AUX_WIDGET_PY    -17//(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_AUX_WIDGET_VISIBLE    CVTE_ENABLE

/**
* DAB WIDGET
*/
#define CVTE_DEF_DAB_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DAB_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DAB_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6+CVTE_DEF_HOME_WIDGET_UNIT_CX*2+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DAB_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_DAB_WIDGET_VISIBLE    CVTE_DISABLE

/**
* SETTING WIDGET
*/
#define CVTE_DEF_SETTING_WIDGET_CX    138//(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_SETTING_WIDGET_CY    90//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_SETTING_WIDGET_PX    319//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_SETTING_WIDGET_PY    -187//(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_SETTING_WIDGET_VISIBLE    CVTE_ENABLE

/**
* DSP WIDGET
*/
#define CVTE_DEF_DSP_WIDGET_CX    138//(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DSP_WIDGET_CY    90//(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DSP_WIDGET_PX    319//(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_DSP_WIDGET_PY    -109//(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_DSP_WIDGET_VISIBLE    CVTE_ENABLE

/**
* LED
*/
#if (CVTE_MODEL_TYPE == CVTE_MODEL_TYPE_U702P_ASP)

#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START		20
#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END		50
#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_DEFAULT			40
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START	5
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END		20
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_DEFAULT		15

#endif

/******************************************************************************/
/************************** CVTE ע���Ĭ������ *******************************/
/******************************************************************************/
//Font Ĭ������
#define CVTE_REG_FONT_DEFAULT				CVTE_DEF_FONT_TYPE_MYRIAD_PRO	

//TimeZone Ĭ��ʱ��
#define CVTE_REG_TIMEZONE_DEFAULT	    CVTE_DEF_TIME_ZONE_800GMT_PACIFIC_STANDARD_TIME

//Background Path
#define CVTE_REG_BACKGROUND_PATH_DEFAULT	_T("\\ResidentFlash\\LightningUI\\background\\IMAGES\\PLANET AUDIO.jpg")

//Navi Path
#define CVTE_REG_NAVI_PATH_DEFAULT	    _T("\\GPS CARD\\")

//Bluetooth Device Name
#define CVTE_REG_BT_DEVICE_NAME_DEFAULT	    _T("PLANET AUDIO PCP9800")

//Radio Region
//CVTE_DEF_RADIO_REGION_MIN~CVTE_DEF_RADIO_REGION_MAX
#define CVTE_REG_RADIO_REGION_DEFAULT	    CVTE_DEF_RADIO_REGION_USA2

//RDS AF̨ͣ������ (Լ��ʵֵdb��2��)
//CVTE_DEF_RDS_AF_LEVEL_MIN~CVTE_DEF_RDS_AF_LEVEL_MAX
#define CVTE_REG_RDS_AF_LEVEL_DEFAULT    70

//RDS �ܿ���
#define CVTE_REG_RDS_ENABLE_DEFAULT			CVTE_ENABLE

//RDS AF ���ܿ���
#define CVTE_REG_RDS_AF_ENABLE_DEFAULT		CVTE_DISABLE

//RDS TA ���ܿ���
#define CVTE_REG_RDS_TA_ENABLE_DEFAULT		CVTE_DISABLE

//Language
//CVTE_DEF_LANGUAGE_MIN~CVTE_DEF_LANGUAGE_MAX
#define CVTE_REG_LANGUAGE_DEFAULT	    CVTE_DEF_LANGUAGE_ENGLISH

//Time Mode
//CVTE_DEF_TIME_MODE_12H/CVTE_DEF_TIME_MODE_24H
#define CVTE_REG_TIME_MODE_DEFAULT  CVTE_DEF_TIME_MODE_12H

//Drive Type
//CVTE_DEF_DRIVE_TYPE_LEFT~CVTE_DEF_DRIVE_TYPE_RIGHT
#define CVTE_REG_DRIVE_TYPE_DEFAULT CVTE_DEF_DRIVE_TYPE_LEFT

//Backlight
//CVTE_DEF_BACKLIGHT_MIN~CVTE_DEF_BACKLIGHT_MAX
#define CVTE_REG_BACKLIGHT_DEFAULT  CVTE_DEF_BACKLIGHT_MAX

//DTV Video Color
//CVTE_DEF_VIDEO_COLOR_TYPE_MIN~CVTE_DEF_VIDEO_COLOR_TYPE_MAX
#define CVTE_REG_DTV_COLOR_DEFAULT  CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_MJ

//DVD Video Color
//CVTE_DEF_VIDEO_COLOR_TYPE_MIN~CVTE_DEF_VIDEO_COLOR_TYPE_MAX
#define CVTE_REG_DVD_COLOR_DEFAULT  CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_MJ

//Reverse Video Color
//CVTE_DEF_VIDEO_COLOR_TYPE_MIN~CVTE_DEF_VIDEO_COLOR_TYPE_MAX
#define CVTE_REG_REARVIEW_COLOR_DEFAULT  CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_MJ

//LCD GAMMA
#define CVTE_REG_GAMMA_DEFAULT				150

//LED
//0~255
//Red Green Blue
#define CVTE_REG_LED_R_DEFAULT  255
#define CVTE_REG_LED_G_DEFAULT  0
#define CVTE_REG_LED_B_DEFAULT  0

//Auto Start Navi
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_AUTO_START_NAVI_DEFAULT  CVTE_DISABLE

//Key Beep
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_KEEP_BEEP_DEFAULT  CVTE_DISABLE

//Reverse Volume Auto Adjustment
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_REAR_VOL_CONTROL_DEFAULT  CVTE_ENABLE

//Display Blur Effect
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_BLUR_EFFECT_DEFAULT  CVTE_DISABLE

//Led Anim Effect
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_LED_ANIM_EFFECT_DEFAULT  CVTE_ENABLE

//Activate Factory Mode
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_FACTORY_MODE_DEFAULT  CVTE_DISABLE

//Snapshot Enable
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_SNAPSHOT_ENABLE_DEFAULT  CVTE_DISABLE

//DAB Enable
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_DAB_ENABLE_DEFAULT  CVTE_DISABLE

//DayMode & NightMode
#define CVTE_REG_LCD_DAYMODE_BRIGHTNESS_DEFAULT		CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END												//CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_DEFAULT
#define CVTE_REG_LCD_NIGHTMODE_BRIGHTNESS_DEFAULT	CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START + 8	//CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_DEFAULT

#define CVTE_REG_LED_DAYMODE_BRIGHTNESS_DEFAULT		CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END												//CVTE_DEF_DAYMODE_LED_BRIGHTNESS_DEFAULT
#define CVTE_REG_LED_NIGHTMODE_BRIGHTNESS_DEFAULT	CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START + 15	//CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_DEFAULT

/******************************************************************************/
/************************** CVTE Carplay������� *******************************/
/******************************************************************************/
//Carplay icon label
#define CVTE_DEF_CARPLAY_ICON_LABEL	"Radio"

//Carplay Vehicle Display name
#define CVTE_DEF_VEHICLE_DISPLAY_NAME		TEXT("PLANET AUDIO PCP9800")

//Carplay manufacturer display name
#define CVTE_DEF_MANUFACTURER_DISPLAY_NAME	"PLANET AUDIO"

//Carplay model display name
#define CVTE_DEF_MODEL_DISPLAY_NAME			"PCP9800"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CUSTOMER_PLANETAUDIO_H__ */