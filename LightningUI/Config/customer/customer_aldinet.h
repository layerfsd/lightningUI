/******************************************************************************

  Copyright (C) 2015-2016 CVTE. All Rights Reserved.

 ******************************************************************************
  File Name     : customer_aldinet.h
  Version       : Initial Draft
  Author        : zhaoshunxian
  Created       : 2016/7/4
  Last Modified :
  Description   : customer_aldinet's configs file
  Function List :
  History       :
  1.Date        : 2016/7/4
    Author      : zhaoshunxian
    Modification: Created file
  2.Date		: 20160802
    Author		: wuhaoheng
	Modification: Use CVTE_MODEL_TYPE macro

******************************************************************************/

#ifndef __CUSTOMER_ALDINET_H__
#define __CUSTOMER_ALDINET_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/******************************************************************************/
/************************* ALDINET 模块类型选择 *******************************/
/******************************************************************************/
#define CVTE_MODEL_TYPE			CVTE_MODEL_TYPE_U702P_ASP

/******************************************************************************/
/*************************     UI风格类型选择   *******************************/
/******************************************************************************/
#define CVTE_DEF_UI_STYLE	CVTE_UI_STYLE_ALDINET

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

#define CVTE_EN_DAB				CVTE_ENABLE

#define CVTE_EN_CAMERA			CVTE_ENABLE

#define CVTE_EN_ANDROID_AUTO	CVTE_ENABLE

/******************************************************************************/
/*************************       功能配置       *******************************/
/******************************************************************************/

#define CVTE_DEF_LANGUAGE_SUPPORT		( CVTE_LANGUAGE_SUPPORT_ENGLISH \
	| CVTE_LANGUAGE_SUPPORT_ITALIAN )

#define CVTE_DEF_RADIO_REGION_SUPPORT		( CVTE_RADIO_REGION_SUPPORT_EUROPE \
										| CVTE_RADIO_REGION_SUPPORT_USA \
										| CVTE_RADIO_REGION_SUPPORT_USA2)

/******************************************************************************/
/*************************     其他功能使能     *******************************/
/******************************************************************************/
//Home status bar logo
#define CVTE_CUSTOMER_STATUS_BAR_LOGO			CVTE_DISABLE

//Steer wheel old ui style
#define CVTE_EN_STEER_WHEEL_OLD_UI_STYLE		CVTE_DISABLE

//Reverse volume auto adjustment
#define CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT		CVTE_DISABLE

//Navi smart drop bar auto show
#define CVTE_EN_NAVI_DROP_BAR					CVTE_DISABLE

//Dynamic logo show
#define CVTE_EN_DYNAMIC_LOGO					CVTE_DISABLE

//LightningUI High priority
#define CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY		CVTE_ENABLE

//DAB and Radio use the same widget or not, CVTE_ENABLE=do not use same widget
#define CVTE_EN_RADIO_DEPART_DAB_UI_STYLE       CVTE_ENABLE

//Enable CarPlay Night mode setting
#define CVTE_EN_CARPLAY_NIGHTMODE				CVTE_DISABLE

//蓝牙配对码弹出提示
#define CVTE_EN_BT_PAIRED_CODE_PERMIT			CVTE_ENABLE

//Carplay/iPod共用图标的切换开关
#define CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON		CVTE_DISABLE

//主页图标动画开关
#define CVTE_EN_HOME_WIDGET_ANIM				CVTE_DISABLE

//重低音频率调整选项
#define CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST		CVTE_ENABLE
/******************************************************************************/
/**************************    主页widget设置   *******************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//             HOME WIDGET OPTIONS 主页Widget基本属性                        //
////////////////////////////////////////////////////////////////////////////////

#ifndef CVTE_DEF_HOME_WIDGET_MARGIN_WIDTH
#define CVTE_DEF_HOME_WIDGET_MARGIN_WIDTH		0
#endif

#ifndef CVTE_DEF_HOME_WIDGET_MARGIN_HEIGHT
#define CVTE_DEF_HOME_WIDGET_MARGIN_HEIGHT		70
#endif

#ifndef CVTE_DEF_HOME_WIDGET_STATUS_BAR_HEIGHT
#define CVTE_DEF_HOME_WIDGET_STATUS_BAR_HEIGHT	42
#endif

#ifndef CVTE_DEF_HOME_WIDGET_UNIT_CX		//130
#define CVTE_DEF_HOME_WIDGET_UNIT_CX		130
#endif

#ifndef CVTE_DEF_HOME_WIDGET_UNIT_CY		//140
#define CVTE_DEF_HOME_WIDGET_UNIT_CY		134
#endif
//Panel Width
#define CVTE_DEF_HOME_WIDGET_WIDTH  (CVTE_DEF_SCREEN_VIDEOWIDTH)

#define CVTE_DEF_HOME_WIDGET_UINT_PX		(-(CVTE_DEF_SCREEN_VIDEOWIDTH/2)+CVTE_DEF_HOME_WIDGET_MARGIN_WIDTH/2+CVTE_DEF_HOME_WIDGET_UNIT_CX)
/**
* MEDIA WIDGET
*/
#define CVTE_DEF_MEDIA_WIDGET_CX    (396)
#define CVTE_DEF_MEDIA_WIDGET_CY    (202)
#define CVTE_DEF_MEDIA_WIDGET_PX    (4+CVTE_DEF_MEDIA_WIDGET_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_MEDIA_WIDGET_PY    -(70+CVTE_DEF_MEDIA_WIDGET_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_MEDIA_WIDGET_VISIBLE    CVTE_ENABLE

/**
* RADIO WIDGET
*/
#define CVTE_DEF_RADIO_WIDGET_CX    (396)
#define CVTE_DEF_RADIO_WIDGET_CY    (202)
#define CVTE_DEF_RADIO_WIDGET_PX    (4+CVTE_DEF_RADIO_WIDGET_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_RADIO_WIDGET_PY    -(274+CVTE_DEF_RADIO_WIDGET_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_RADIO_WIDGET_VISIBLE    CVTE_EN_RADIO

/**
* CARPLAY WIDGET
*/
#define CVTE_DEF_CARPLAY_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_CARPLAY_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_CARPLAY_WIDGET_PX    (402+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_CARPLAY_WIDGET_PY    -(342+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_CARPLAY_WIDGET_VISIBLE    CVTE_EN_CARPLAY

/**
* IPOD WIDGET
*/
#define CVTE_DEF_IPOD_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_IPOD_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_IPOD_WIDGET_PX    (534+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_IPOD_WIDGET_PY    -(342+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_IPOD_WIDGET_VISIBLE    CVTE_EN_IPOD

/**
* EASYCONNECT WIDGET
*/
#define CVTE_DEF_ANDROID_LINK_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_ANDROID_LINK_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_ANDROID_LINK_WIDGET_PX    (402+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_ANDROID_LINK_WIDGET_PY    -(206+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_ANDROID_LINK_WIDGET_VISIBLE    CVTE_EN_EASYCONNECT

/**
* WEBLINK WIDGET
*/
#define CVTE_DEF_WEBLINK_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_WEBLINK_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_WEBLINK_WIDGET_PX			(402+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_WEBLINK_WIDGET_PY			-(206+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_WEBLINK_WIDGET_VISIBLE			CVTE_EN_WEBLINK


/**
* ANDROID AUTO WIDGET
*/
#define CVTE_DEF_ANDROID_AUTO_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_ANDROID_AUTO_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_ANDROID_AUTO_WIDGET_PX    (402+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_ANDROID_AUTO_WIDGET_PY    -(206+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_ANDROID_AUTO_WIDGET_VISIBLE    CVTE_EN_ANDROID_AUTO

/**
* BT PHONE WIDGET
*/
#define CVTE_DEF_BT_PHONE_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_BT_PHONE_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_BT_PHONE_WIDGET_PX    (402+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_BT_PHONE_WIDGET_PY    -(70+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_BT_PHONE_WIDGET_VISIBLE    CVTE_EN_BT_PHONE

/**
* NAVI WIDGET
*/
#define CVTE_DEF_NAVI_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_NAVI_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_NAVI_WIDGET_PX    (666+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_NAVI_WIDGET_PY    -(70+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_NAVI_WIDGET_VISIBLE    CVTE_EN_NAVI

/**
* BT A2DP WIDGET
*/
#define CVTE_DEF_BT_A2DP_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_BT_A2DP_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_BT_A2DP_WIDGET_PX    (534+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_BT_A2DP_WIDGET_PY    -(70+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_BT_A2DP_WIDGET_VISIBLE    CVTE_EN_BT_A2DP

/**
* USB WIDGET
*/
#define CVTE_DEF_USB_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_USB_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_USB_WIDGET_PX    (534+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_USB_WIDGET_PY    -(206+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_USB_WIDGET_VISIBLE    CVTE_EN_USB

/**
* DVD WIDGET
*/
#define CVTE_DEF_DVD_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_DVD_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_DVD_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*5+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DVD_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_DVD_WIDGET_VISIBLE    CVTE_ENABLE

/**
* DTV WIDGET
*/
#define CVTE_DEF_DTV_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_DTV_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_DTV_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DTV_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_DTV_WIDGET_VISIBLE    CVTE_ENABLE

/**
* AUX WIDGET
*/
#define CVTE_DEF_AUX_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_AUX_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_AUX_WIDGET_PX    (666+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_AUX_WIDGET_PY    -(206+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_AUX_WIDGET_VISIBLE    CVTE_EN_AUX

/**
* DAB WIDGET
*/
#define CVTE_DEF_DAB_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DAB_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DAB_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*5+CVTE_DEF_HOME_WIDGET_UNIT_CX*2+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DAB_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_DAB_WIDGET_VISIBLE    CVTE_DISABLE

/**
* CAMERA WIDGET
*/
#define CVTE_DEF_CAMERA_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_CAMERA_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_CAMERA_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*5+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_CAMERA_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_CAMERA_WIDGET_VISIBLE    CVTE_ENABLE

/**
* SETTING WIDGET
*/
#define CVTE_DEF_SETTING_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_SETTING_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_SETTING_WIDGET_PX    (666+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_SETTING_WIDGET_PY    -(342+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_SETTING_WIDGET_VISIBLE    CVTE_ENABLE

/**
* VEHICLE WIDGET
*/
#define CVTE_DEF_VEHICLE_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX)
#define CVTE_DEF_VEHICLE_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_VEHICLE_WIDGET_PX    (666+CVTE_DEF_HOME_WIDGET_UNIT_CX/2-(CVTE_DEF_SCREEN_VIDEOWIDTH/2))
#define CVTE_DEF_VEHICLE_WIDGET_PY    -(342+CVTE_DEF_HOME_WIDGET_UNIT_CY/2-(CVTE_DEF_SCREEN_VIDEOHEIGHT/2))
#define CVTE_DEF_VEHICLE_WIDGET_VISIBLE    CVTE_ENABLE
/******************************************************************************/
/************************** CVTE 注册表默认设置 *******************************/
/******************************************************************************/
//Font 字体
#define CVTE_REG_FONT_DEFAULT			CVTE_DEF_FONT_TYPE_HELVETICA	

//TimeZone 默认时区
#define CVTE_REG_TIMEZONE_DEFAULT	    CVTE_DEF_TIME_ZONE_GMT100_WEUROPE_STANDARD_TIME

//Navi Path
#define CVTE_REG_NAVI_PATH_DEFAULT	    _T("\\GPS CARD\\MobileNavigator\\MobileNavigator.exe")

//Bluetooth Device Name
#define CVTE_REG_BT_DEVICE_NAME_DEFAULT	    _T("M-DL9000")

//Radio Region
//CVTE_DEF_RADIO_REGION_MIN~CVTE_DEF_RADIO_REGION_MAX
#define CVTE_REG_RADIO_REGION_DEFAULT	    CVTE_DEF_RADIO_REGION_EUROPE

//RDS AF停台灵敏度 (约真实值db的2倍)
//CVTE_DEF_RDS_AF_LEVEL_MIN~CVTE_DEF_RDS_AF_LEVEL_MAX
#define CVTE_REG_RDS_AF_LEVEL_DEFAULT    70

//Language
//CVTE_DEF_LANGUAGE_MIN~CVTE_DEF_LANGUAGE_MAX
#define CVTE_REG_LANGUAGE_DEFAULT	    CVTE_DEF_LANGUAGE_ITALIAN

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
#define CVTE_REG_LED_G_DEFAULT  255
#define CVTE_REG_LED_B_DEFAULT  255

//Auto Start Navi
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_AUTO_START_NAVI_DEFAULT  CVTE_DISABLE

//Key Beep
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_KEEP_BEEP_DEFAULT  CVTE_ENABLE

//Reverse Volume Auto Adjustment
//CVTE_DISABLE/CVTE_ENABLE
#define CVTE_REG_REAR_VOL_CONTROL_DEFAULT  CVTE_ENABLE

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
#define CVTE_DEF_CARPLAY_ICON_LABEL	"M-DL9000"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CUSTOMER_ALDINET_H__ */
