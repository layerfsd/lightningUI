/******************************************************************************

  Copyright (C) 2015-2016 CVTE. All Rights Reserved.

 ******************************************************************************
  File Name     : customer_ayonz.h
  Version       : Initial Draft
  Author        : wuhaoheng 
  Created       : 2017/2/22
  Last Modified :
  Description   : customer_ayonz's configs file
  Function List :
  History       :
******************************************************************************/

#ifndef __CUSTOMER_AYONZ_H__
#define __CUSTOMER_AYONZ_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/******************************************************************************/
/*************************      模块类型选择    *******************************/
/******************************************************************************/
#define CVTE_MODEL_TYPE			CVTE_MODEL_TYPE_U702P_ASP

/******************************************************************************/
/*************************      主要模块使能    *******************************/
/******************************************************************************/
#define CVTE_EN_CARPLAY			CVTE_ENABLE

#define CVTE_EN_IPOD			CVTE_ENABLE

#define CVTE_EN_LED			    CVTE_ENABLE

#define CVTE_EN_RADIO			CVTE_ENABLE

#define CVTE_EN_EASYCONNECT		CVTE_DISABLE

#define CVTE_EN_BT_PHONE		CVTE_ENABLE

#define CVTE_EN_BT_A2DP			CVTE_ENABLE

#define CVTE_EN_NAVI			CVTE_ENABLE

#define CVTE_EN_USB			    CVTE_ENABLE

#define CVTE_EN_SD				CVTE_DISABLE

#define CVTE_EN_DVD		        CVTE_DISABLE

#define CVTE_EN_DTV		        CVTE_DISABLE

#define CVTE_EN_AUX		        CVTE_ENABLE

#define CVTE_EN_RDS				CVTE_ENABLE

#define CVTE_EN_RBDS			CVTE_DISABLE

#define CVTE_EN_RDS_TMC			CVTE_DISABLE

#define CVTE_EN_DAB				CVTE_DISABLE

#define CVTE_EN_CAMERA			CVTE_DISABLE

#define CVTE_EN_ANDROID_AUTO	CVTE_ENABLE

#define CVTE_EN_WEBLINK			CVTE_DISABLE
/******************************************************************************/
/*************************       功能配置       *******************************/
/******************************************************************************/
#define CVTE_DEF_LANGUAGE_SUPPORT		( CVTE_LANGUAGE_SUPPORT_ENGLISH \
										| CVTE_LANGUAGE_SUPPORT_GERMAN )

/******************************************************************************/
/*************************     其他功能使能     *******************************/
/******************************************************************************/
//Save Sector for CA
#define CVTE_EN_CPCA							CVTE_ENABLE

//Save Sector for MAC
#define CVTE_EN_CPMAC							CVTE_ENABLE

//DCP/CDP充电控制
#define CVTE_EN_CDP_DCP_CONTROL					CVTE_ENABLE

/******************************************************************************/
/**************************    主页widget设置   *******************************/
/******************************************************************************/

#define CVTE_DEF_SCREEN_VIDEOWIDTH		800

#define CVTE_DEF_SCREEN_VIDEOHEIGHT		480

//Panel Width
#define CVTE_DEF_HOME_WIDGET_WIDTH  (CVTE_DEF_SCREEN_VIDEOWIDTH+CVTE_DEF_HOME_WIDGET_UNIT_CX)

/**
* MEDIA WIDGET
*/
#define CVTE_DEF_MEDIA_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_MEDIA_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#define CVTE_DEF_MEDIA_WIDGET_PX    CVTE_DEF_HOME_WIDGET_UINT_PX
#define CVTE_DEF_MEDIA_WIDGET_PY    CVTE_DEF_HOME_WIDGET_UINT_PY
#define CVTE_DEF_MEDIA_WIDGET_VISIBLE    CVTE_ENABLE

/**
* RADIO WIDGET
*/
#define CVTE_DEF_RADIO_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_RADIO_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_RADIO_WIDGET_PX    CVTE_DEF_HOME_WIDGET_UINT_PX
#define CVTE_DEF_RADIO_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_RADIO_WIDGET_VISIBLE    CVTE_ENABLE

/**
* CARPLAY WIDGET
*/
#define CVTE_DEF_CARPLAY_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_CARPLAY_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#define CVTE_DEF_CARPLAY_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_CARPLAY_WIDGET_PY    CVTE_DEF_HOME_WIDGET_UINT_PY
#define CVTE_DEF_CARPLAY_WIDGET_VISIBLE    CVTE_EN_CARPLAY

/**
* IPOD WIDGET
*/
#define CVTE_DEF_IPOD_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_IPOD_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#define CVTE_DEF_IPOD_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_IPOD_WIDGET_PY    CVTE_DEF_HOME_WIDGET_UINT_PY
#define CVTE_DEF_IPOD_WIDGET_VISIBLE    CVTE_DISABLE

/**
* EASYCONNECT WIDGET
*/
#define CVTE_DEF_ANDROID_LINK_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_ANDROID_LINK_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_ANDROID_LINK_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_ANDROID_LINK_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_ANDROID_LINK_WIDGET_VISIBLE    CVTE_EN_EASYCONNECT

/**
* ANDROID AUTO WIDGET
*/
#define CVTE_DEF_ANDROID_AUTO_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_ANDROID_AUTO_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_ANDROID_AUTO_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_ANDROID_AUTO_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_ANDROID_AUTO_WIDGET_VISIBLE    CVTE_EN_ANDROID_AUTO

/**
* WEBLINK WIDGET
*/
#define CVTE_DEF_WEBLINK_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_WEBLINK_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_WEBLINK_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_WEBLINK_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_WEBLINK_WIDGET_VISIBLE    CVTE_EN_WEBLINK

/**
* BT PHONE WIDGET
*/
#define CVTE_DEF_BT_PHONE_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_BT_PHONE_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_BT_PHONE_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4)
#define CVTE_DEF_BT_PHONE_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_BT_PHONE_WIDGET_VISIBLE    CVTE_ENABLE

/**
* NAVI WIDGET
*/
#define CVTE_DEF_NAVI_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#define CVTE_DEF_NAVI_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_NAVI_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4)
#define CVTE_DEF_NAVI_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_NAVI_WIDGET_VISIBLE    CVTE_EN_NAVI

/**
* BT A2DP WIDGET
*/
#define CVTE_DEF_BT_A2DP_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_BT_A2DP_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_BT_A2DP_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4-CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_BT_A2DP_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_BT_A2DP_WIDGET_VISIBLE    CVTE_ENABLE

/**
* USB WIDGET
*/
#define CVTE_DEF_USB_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_USB_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_USB_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_USB_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_USB_WIDGET_VISIBLE    CVTE_ENABLE

/**
* DVD WIDGET
*/
#define CVTE_DEF_DVD_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DVD_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DVD_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DVD_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2+CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_DVD_WIDGET_VISIBLE    CVTE_EN_DVD

/**
* DTV WIDGET
*/
#define CVTE_DEF_DTV_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DTV_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DTV_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX*2+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DTV_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2+CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_DTV_WIDGET_VISIBLE    CVTE_EN_DTV

/**
* AUX WIDGET
*/
#define CVTE_DEF_AUX_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_AUX_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_AUX_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_AUX_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_AUX_WIDGET_VISIBLE    CVTE_EN_AUX

/**
* DAB WIDGET
*/
#define CVTE_DEF_DAB_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DAB_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DAB_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*5+CVTE_DEF_HOME_WIDGET_UNIT_CX*2+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DAB_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_DAB_WIDGET_VISIBLE    CVTE_EN_DAB

/**
* CAMERA WIDGET
*/
#define CVTE_DEF_CAMERA_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_CAMERA_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_CAMERA_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*5+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_CAMERA_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_CAMERA_WIDGET_VISIBLE    CVTE_EN_CAMERA

/**
* SETTING WIDGET
*/
#define CVTE_DEF_SETTING_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_SETTING_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_SETTING_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_SETTING_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_SETTING_WIDGET_VISIBLE    CVTE_ENABLE

/******************************************************************************/
/************************** CVTE 注册表默认设置 *******************************/
/******************************************************************************/

//TimeZone 默认时区
#define CVTE_REG_TIMEZONE_DEFAULT	    CVTE_DEF_TIME_ZONE_GMT100_WEUROPE_STANDARD_TIME

//Navi Path
#define CVTE_REG_NAVI_PATH_DEFAULT	    _T("\\GPS CARD\\")

//Bluetooth Device Name
#define CVTE_REG_BT_DEVICE_NAME_DEFAULT	    _T("Automotive BT Device")

//Radio Region
//CVTE_DEF_RADIO_REGION_MIN~CVTE_DEF_RADIO_REGION_MAX
#define CVTE_REG_RADIO_REGION_DEFAULT	    CVTE_DEF_RADIO_REGION_EUROPE

//RDS AF停台灵敏度 (约真实值db的2倍)
//CVTE_DEF_RDS_AF_LEVEL_MIN~CVTE_DEF_RDS_AF_LEVEL_MAX
#define CVTE_REG_RDS_AF_LEVEL_DEFAULT    70

//Language
//CVTE_DEF_LANGUAGE_MIN~CVTE_DEF_LANGUAGE_MAX
#define CVTE_REG_LANGUAGE_DEFAULT	    CVTE_DEF_LANGUAGE_ENGLISH

//Time Mode
//CVTE_DEF_TIME_MODE_12H/CVTE_DEF_TIME_MODE_24H
#define CVTE_REG_TIME_MODE_DEFAULT  CVTE_DEF_TIME_MODE_24H

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
#define CVTE_REG_KEEP_BEEP_DEFAULT  CVTE_ENABLE

//Reverse Volume Auto Adjustment
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_REAR_VOL_CONTROL_DEFAULT  CVTE_DISABLE

//Display Blur Effect
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_BLUR_EFFECT_DEFAULT  CVTE_DISABLE

//Led Anim Effect
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_LED_ANIM_EFFECT_DEFAULT  CVTE_DISABLE

//Activate Factory Mode
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_FACTORY_MODE_DEFAULT  CVTE_DISABLE

//Snapshot Enable
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_SNAPSHOT_ENABLE_DEFAULT  CVTE_DISABLE

//DAB Enable
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_DAB_ENABLE_DEFAULT  CVTE_DISABLE

/******************************************************************************/
/************************** CVTE Carplay相关设置 *******************************/
/******************************************************************************/
//Carplay icon label
#define CVTE_DEF_CARPLAY_ICON_LABEL	"Main menu"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CUSTOMER_DAIICHI_H__ */
