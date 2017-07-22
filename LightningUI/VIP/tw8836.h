/******************************************************************************
 CVTE Limited

 Copyright (c) 2014 CVTE company.

 Author Wu haoheng 2014

 All rights reserved.

******************************************************************************/

#ifndef _TW8836_PDD_H_
#define _TW8836_PDD_H_

#define DEBUG_TW8836_MSG    0
#define DEBUG_TW8836_ERR    1
#define DEBUG_TW8836_TEST   0

#define NEW_HW_VERSION      1


#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum _TW8836_INPUT_CHANNEL_
	{
		TW8836_INPUT_CHANNEL_CVBS_NTSC = 0,
		TW8836_INPUT_CHANNEL_CVBS_PAL,
		TW8836_INPUT_CHANNEL_ARGB,
		TW8836_INPUT_CHANNEL_DTV_NTSC,
		TW8836_INPUT_CHANNEL_DTV_PAL,
		TW8836_INPUT_CHANNEL_DVD,
		TW8836_INPUT_CHANNEL_RGB888,
		TW8836_INPUT_CHANNEL_NA
	}TW8836INPUTCHANNEL;

	typedef enum _TW8836_STANDARD_INPUT_
	{
		TW8836_STANDARD_INPUT_NTSC_M = 0,
		TW8836_STANDARD_INPUT_PAL_BDGHI,
		TW8836_STANDARD_INPUT_SECAM,
		TW8836_STANDARD_INPUT_NTSC_443,
		TW8836_STANDARD_INPUT_PAL_M,
		TW8836_STANDARD_INPUT_PAL_CN,
		TW8836_STANDARD_INPUT_PAL_60,
		TW8836_STANDARD_INPUT_NA,
	}TW8836STANDARDINPUT;

	typedef enum _TW8836_COLOR_TYPE_
	{
		TW8836_IMAGE_CONTRAST_Y = 0,
		TW8836_IMAGE_CONTRAST_CB,
		TW8836_IMAGE_CONTRAST_CR,
		TW8836_IMAGE_BRIGHTNESS,
		TW8836_IMAGE_SHARPNESS,
		TW8836_DECODER_BRIGHTNESS,
		TW8836_DECODER_CONTRAST,
		TW8836_DECODER_SHARPNESS,
		TW8836_DECODER_SATURATION_U,
		TW8836_DECODER_SATURATION_V,
		TW8836_DECODER_HUE
	}TW8836COLORTYPE;

	typedef struct _TW8836_COLOR_EFFECT_
	{
		BYTE uColorType;
		BYTE uColorValue;
	}TW8836COLOREFFECT, *PTW8836COLOREFFECT;

	typedef enum _TW8836_BT656_ENCODE_SOURCE_
	{
		TW8836_BT656_ENCODE_DECODER = 0,    // 0
		TW8836_BT656_ENCODE_ARGB,           // 1
		TW8836_BT656_ENCODE_DTV,            // 2
		TW8836_BT656_ENCODE_LVDSRX,         // 3
		TW8836_BT656_ENCODE_PANEL_OUTPUT    // 4
	}TW8836BT656ENCODESOURCE;

	typedef enum _TW8836_MODE_
	{
		TW8836_MODE_LVDS_CVBS_PAL = 0,
		TW8836_MODE_LVDS_CVBS_NTSC,
		TW8836_MODE_LVDS_DVD,
		TW8836_MODE_BT656_CVBS_PAL,
		TW8836_MODE_BT656_CVBS_NTSC,
		TW8836_MODE_NULL
	}TW8836MODE;

	typedef enum _TW8836_POWER_CONTROL_
	{
		TW8836_POWER_ON = 0,
		TW8836_POWER_OFF
	}TW8836POWERCONTROL;

#ifdef __cplusplus
}
#endif

#endif


