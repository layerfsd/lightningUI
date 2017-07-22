/******************************************************************************

  Copyright (C) 2015-2016 CVTE. All Rights Reserved.

 ******************************************************************************
  File Name     : customer_soundexpress.h
  Version       : Initial Draft
  Author        : zhaoshunxian
  Created       : 2016/7/4
  Last Modified :
  Description   : customer_soundexpress's configs file
  Function List :
  History       :
  1.Date        : 2016/7/4
    Author      : zhaoshunxian
    Modification: Created file
  2.Date		: 2016/7/19
  	Author		: wuhaoheng
	Modification: Added feature macro definition for customer SOUNDEXPRESS.
  3.Date		: 20160802
    Author		: wuhaoheng
	Modification: Use CVTE_MODEL_TYPE macro	
******************************************************************************/

#ifndef __CUSTOMER_SOUNDEXPRESS_H__
#define __CUSTOMER_SOUNDEXPRESS_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define CVTE_DEF_RELEASE_VERSION_NUM		1
/******************************************************************************/
/*************************      模块类型选择    *******************************/
/******************************************************************************/
//2016年6月份生产的是E版本，之前的是C版本
#define CVTE_MODEL_TYPE			CVTE_MODEL_TYPE_U621P_DE

/******************************************************************************/
/*************************      主要模块使能    *******************************/
/******************************************************************************/
#define CVTE_EN_CARPLAY			CVTE_ENABLE

#define CVTE_EN_IPOD			CVTE_ENABLE

#define CVTE_EN_LED			    CVTE_ENABLE

#define CVTE_EN_RADIO			CVTE_ENABLE

#define CVTE_EN_EASYCONNECT		CVTE_ENABLE

#define CVTE_EN_BT_PHONE		CVTE_ENABLE

#define CVTE_EN_BT_A2DP			CVTE_ENABLE

#define CVTE_EN_NAVI			CVTE_ENABLE

#define CVTE_EN_USB			    CVTE_ENABLE

#define CVTE_EN_SD				CVTE_DISABLE

#define CVTE_EN_DVD		        CVTE_DISABLE

#define CVTE_EN_DTV		        CVTE_ENABLE

#define CVTE_EN_AUX		        CVTE_ENABLE

#define CVTE_EN_RDS				CVTE_DISABLE

#define CVTE_EN_RBDS			CVTE_DISABLE

#define CVTE_EN_RDS_TMC			CVTE_DISABLE

#define CVTE_EN_DAB				CVTE_DISABLE

#define CVTE_EN_CAMERA			CVTE_DISABLE
/******************************************************************************/
/*************************       功能配置       *******************************/
/******************************************************************************/
#define CVTE_DEF_LANGUAGE_SUPPORT		(CVTE_LANGUAGE_SUPPORT_ENGLISH \
										|CVTE_LANGUAGE_SUPPORT_CHINESE_TRA \
										|CVTE_LANGUAGE_SUPPORT_CHINESE_SIM \
										|CVTE_LANGUAGE_SUPPORT_KOREAN \
										|CVTE_LANGUAGE_SUPPORT_GERMAN \
										|CVTE_LANGUAGE_SUPPORT_SPANISH \
										|CVTE_LANGUAGE_SUPPORT_ITALIAN \
										|CVTE_LANGUAGE_SUPPORT_FRENCH \
										|CVTE_LANGUAGE_SUPPORT_RUSSIAN)


/******************************************************************************/
/*************************     其他功能使能     *******************************/
/******************************************************************************/
//Steer wheel old ui style
#define CVTE_EN_STEER_WHEEL_OLD_UI_STYLE		CVTE_DISABLE

//Reverse volume auto adjustment
#define CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT		CVTE_DISABLE

//Navi smart drop bar auto show
#define CVTE_EN_NAVI_DROP_BAR					CVTE_DISABLE

//用户升级开机/待机静态logo
#define CVTE_EN_USER_UPDATE_LOGO				CVTE_ENABLE

//Save Sector for CA
#define CVTE_EN_CPCA							CVTE_ENABLE

//Save Sector for MAC
#define CVTE_EN_CPMAC							CVTE_ENABLE

/******************************************************************************/
/**************************    主页widget设置   *******************************/
/******************************************************************************/
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
#define CVTE_DEF_CARPLAY_WIDGET_VISIBLE    CVTE_ENABLE

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
#define CVTE_DEF_ANDROID_LINK_WIDGET_VISIBLE    CVTE_ENABLE

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
#define CVTE_DEF_NAVI_WIDGET_VISIBLE    CVTE_ENABLE

/**
* BT A2DP WIDGET
*/
#define CVTE_DEF_BT_A2DP_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_BT_A2DP_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_BT_A2DP_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4-CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_BT_A2DP_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_BT_A2DP_WIDGET_VISIBLE    CVTE_ENABLE

/**
* USB WIDGET
*/
#define CVTE_DEF_USB_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_USB_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_USB_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_USB_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_USB_WIDGET_VISIBLE    CVTE_ENABLE

/**
* DVD WIDGET
*/
#define CVTE_DEF_DVD_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DVD_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DVD_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX*2+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#define CVTE_DEF_DVD_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2+CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_DVD_WIDGET_VISIBLE    CVTE_DISABLE

/**
* DTV WIDGET
*/
#define CVTE_DEF_DTV_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_DTV_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_DTV_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_DTV_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_DTV_WIDGET_VISIBLE    CVTE_ENABLE

/**
* AUX WIDGET
*/
#define CVTE_DEF_AUX_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_AUX_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_AUX_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_AUX_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
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
* CAMERA WIDGET
*/
#define CVTE_DEF_CAMERA_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_CAMERA_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_CAMERA_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_CAMERA_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#define CVTE_DEF_CAMERA_WIDGET_VISIBLE    CVTE_DISABLE

/**
* SETTING WIDGET
*/
#define CVTE_DEF_SETTING_WIDGET_CX    (CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#define CVTE_DEF_SETTING_WIDGET_CY    (CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#define CVTE_DEF_SETTING_WIDGET_PX    (CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#define CVTE_DEF_SETTING_WIDGET_PY    (CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#define CVTE_DEF_SETTING_WIDGET_VISIBLE    CVTE_ENABLE

/******************************************************************************/
/************************** CVTE 注册表默认设置 *******************************/
/******************************************************************************/
//Font 默认字体
#define CVTE_REG_FONT_DEFAULT				CVTE_DEF_FONT_TYPE_SEGOE_UI	

//TimeZone 默认时区
#define CVTE_REG_TIMEZONE_DEFAULT	    CVTE_DEF_TIME_ZONE_500GMT_SA_PACIFIC_STANDARD_TIME

//Navi Path
#define CVTE_REG_NAVI_PATH_DEFAULT	    _T("\\GPS CARD\\MobileNavigator\\MobileNavigator.exe")

//Bluetooth Device Name
#define CVTE_REG_BT_DEVICE_NAME_DEFAULT	    _T("SOUNDEXPRESS AUTO")

//Radio Region
//CVTE_DEF_RADIO_REGION_MIN~CVTE_DEF_RADIO_REGION_MAX
#define CVTE_REG_RADIO_REGION_DEFAULT	    CVTE_DEF_RADIO_REGION_USA

//RDS AF停台灵敏度 (约真实值db的2倍)
//CVTE_DEF_RDS_AF_LEVEL_MIN~CVTE_DEF_RDS_AF_LEVEL_MAX
#define CVTE_REG_RDS_AF_LEVEL_DEFAULT    70

//Language
//CVTE_DEF_LANGUAGE_MIN~CVTE_DEF_LANGUAGE_MAX
#define CVTE_REG_LANGUAGE_DEFAULT	    CVTE_DEF_LANGUAGE_SPANISH

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
#define CVTE_DEF_CARPLAY_ICON_LABEL	"Main menu"

/******************************************************************************/
/***************************   CVTE TV Widget 设置    *************************/
/******************************************************************************/
#define CVTE_DEF_TV_WIDGET_TYPE				CVTE_DEF_TV_WIDGET_TYPE_AUX_VIDEO

#define CVTE_TV_WIDGET_NAME_STRING			TEXT("AUX Video")

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CUSTOMER_SOUNDEXPRESS_H__ */
