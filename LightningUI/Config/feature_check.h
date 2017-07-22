#ifndef __FEATURE_CHECK_H__
#define __FEATURE_CHECK_H__

/****************************CVTE_MODEL_TYPE******************************
  (1) 默认值:
  (2) 范围说明:	详见Model.h
  (3) 功能描述: MODEL类型
  (4) 作者&日期: zhaoshunxian@2016/06/29
*******************************************************************************/
#ifndef CVTE_MODEL_TYPE
#error "CVTE_MODEL_TYPE is not defined."
#elif (CVTE_MODEL_TYPE<CVTE_MODEL_TYPE_MIN) || (CVTE_MODEL_TYPE>CVTE_MODEL_TYPE_MAX)
#error "CVTE_MODEL_TYPE is out of range."
#endif

/****************************CVTE_DEF_UI_STYLE******************************
  (1) 默认值:
  (2) 范围说明:	详见Feature.h
  (3) 功能描述: UI风格类型
  (4) 作者&日期: 吴昊衡@2016/08/18
*******************************************************************************/
#ifndef CVTE_DEF_UI_STYLE
#error "CVTE_DEF_UI_STYLE is not defined."
#elif (CVTE_DEF_UI_STYLE<CVTE_UI_STYLE_MIN) || (CVTE_DEF_UI_STYLE>CVTE_UI_STYLE_MAX)
#error "CVTE_DEF_UI_STYLE is out of range."
#endif

/****************************CVTE_DEF_DSIPLAY_TYPE******************************
  (1) 默认值:	CVTE_DISPLAY_TYPE_U701P
  (2) 范围说明:	CVTE_DSIPLAY_TYPE_MIN<CVTE_DEF_DSIPLAY_TYPE<CVTE_DSIPLAY_TYPE_MAX
  (3) 功能描述: 显示屏类型
  (4) 作者&日期: zhaoshunxian@2016/06/29
*******************************************************************************/
#ifndef CVTE_DEF_DISPLAY_TYPE
#error "CVTE_DEF_DISPLAY_TYPE is not defined."
#elif (CVTE_DEF_DISPLAY_TYPE<CVTE_DISPLAY_TYPE_MIN) || (CVTE_DEF_DISPLAY_TYPE>CVTE_DISPLAY_TYPE_MAX)
#error "CVTE_DEF_DISPLAY_TYPE is out of range."
#endif

/****************************CVTE_DEF_LED_TYPE******************************
  (1) 默认值:	CVTE_LED_TYPE_U701P
  (2) 范围说明:	CVTE_LED_TYPE_MIN<CVTE_DEF_LED_TYPE<CVTE_LED_TYPE_MAX
  (3) 功能描述: LED类型
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_DEF_LED_TYPE
#error "CVTE_DEF_LED_TYPE is not defined."
#elif (CVTE_DEF_LED_TYPE<CVTE_LED_TYPE_MIN) || (CVTE_DEF_LED_TYPE>CVTE_LED_TYPE_MAX)
#error "CVTE_DEF_LED_TYPE is out of range."
#endif

/****************************CVTE_DEF_TOUCH_TYPE******************************
  (1) 默认值:	CVTE_TOUCH_TYPE_U701P
  (2) 范围说明:	CVTE_TOUCH_TYPE_MIN<CVTE_DEF_TOUCH_TYPE<CVTE_TOUCH_TYPE_MAX
  (3) 功能描述: 触摸类型
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_DEF_TOUCH_TYPE
#error "CVTE_DEF_TOUCH_TYPE is not defined."
#elif (CVTE_DEF_TOUCH_TYPE<CVTE_TOUCH_TYPE_MIN) || (CVTE_DEF_TOUCH_TYPE>CVTE_TOUCH_TYPE_MAX)
#error "CVTE_DEF_TOUCH_TYPE is out of range."
#endif

/****************************CVTE_DEF_AUDIO_TYPE******************************
  (1) 默认值:	CVTE_DEF_LED_TYPE_XXX
  (2) 范围说明:	CVTE_AUDIO_TYPE_MIN<CVTE_DEF_AUDIO_TYPE<CVTE_AUDIO_TYPE_MAX
  (3) 功能描述: AUDIO类型
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_TYPE
#error "CVTE_DEF_AUDIO_TYPE is not defined."
#elif (CVTE_DEF_AUDIO_TYPE<CVTE_AUDIO_TYPE_MIN) || (CVTE_DEF_AUDIO_TYPE>CVTE_AUDIO_TYPE_MAX)
#error "CVTE_DEF_AUDIO_TYPE is out of range."
#endif

/****************************CVTE_REG_RADIO_REGION_DEFAULT******************************
  (1) 默认值:	CVTE_DEF_RADIO_REGION_EUROPE
  (2) 范围说明:	CVTE_DEF_RADIO_REGION_MIN<CVTE_REG_RADIO_REGION_DEFAULT<CVTE_DEF_RADIO_REGION_MAX
  (3) 功能描述: 收音区域默认值范围检查
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_REG_RADIO_REGION_DEFAULT
#error "CVTE_REG_RADIO_REGION_DEFAULT is not defined."
#elif (CVTE_REG_RADIO_REGION_DEFAULT<CVTE_DEF_RADIO_REGION_MIN) || (CVTE_REG_RADIO_REGION_DEFAULT>CVTE_DEF_RADIO_REGION_MAX)
#error "CVTE_REG_RADIO_REGION_DEFAULT is out of range."
#endif

/****************************CVTE_REG_RDS_AF_LEVEL_DEFAULT******************************
  (1) 默认值:	70
  (2) 范围说明:	CVTE_DEF_RADIO_REGION_MIN<CVTE_REG_RDS_AF_LEVEL_DEFAULT<CVTE_DEF_RADIO_REGION_MAX
  (3) 功能描述: RDS AF停台灵敏度默认值范围检查
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_REG_RDS_AF_LEVEL_DEFAULT
#error "CVTE_REG_RDS_AF_LEVEL_DEFAULT is not defined."
#elif (CVTE_REG_RDS_AF_LEVEL_DEFAULT<CVTE_DEF_RDS_AF_LEVEL_MIN) || (CVTE_REG_RDS_AF_LEVEL_DEFAULT>CVTE_DEF_RDS_AF_LEVEL_MAX)
#error "CVTE_REG_RDS_AF_LEVEL_DEFAULT is out of range."
#endif

/****************************CVTE_REG_BACKLIGHT_DEFAULT******************************
  (1) 默认值:	CVTE_REG_BACKLIGHT_DEFAULT
  (2) 范围说明:	CVTE_DEF_BACKLIGHT_MIN<CVTE_REG_BACKLIGHT_DEFAULT<CVTE_DEF_BACKLIGHT_MAX
  (3) 功能描述: 背光默认值检查
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_REG_BACKLIGHT_DEFAULT
#error "CVTE_REG_BACKLIGHT_DEFAULT is not defined."
#elif (CVTE_REG_BACKLIGHT_DEFAULT<CVTE_DEF_BACKLIGHT_MIN) || (CVTE_REG_BACKLIGHT_DEFAULT>CVTE_DEF_BACKLIGHT_MAX)
#error "CVTE_REG_BACKLIGHT_DEFAULT is out of range."
#endif

/****************************CVTE_REG_DRIVE_TYPE_DEFAULT******************************
  (1) 默认值:	CVTE_DEF_DRIVE_TYPE_LEFT
  (2) 范围说明:	CVTE_DEF_DRIVE_TYPE_MIN<CVTE_REG_DRIVE_TYPE_DEFAULT<CVTE_DEF_DRIVE_TYPE_MAX
  (3) 功能描述: 驾驶位置默认值检查
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_REG_DRIVE_TYPE_DEFAULT
#error "CVTE_REG_DRIVE_TYPE_DEFAULT is not defined."
#elif (CVTE_REG_DRIVE_TYPE_DEFAULT<CVTE_DEF_DRIVE_TYPE_MIN) || (CVTE_REG_DRIVE_TYPE_DEFAULT>CVTE_DEF_DRIVE_TYPE_MAX)
#error "CVTE_REG_DRIVE_TYPE_DEFAULT is out of range."
#endif

/****************************CVTE_REG_DTV_COLOR_DEFAULT******************************
  (1) 默认值:	CVTE_REG_DRIVE_TYPE_DEFAULT
  (2) 范围说明:	CVTE_DEF_VIDEO_COLOR_TYPE_MIN<CVTE_REG_DRIVE_TYPE_DEFAULT<CVTE_DEF_VIDEO_COLOR_TYPE_MAX
  (3) 功能描述: DTV视频制式默认值检查
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_REG_DTV_COLOR_DEFAULT
#error "CVTE_REG_DTV_COLOR_DEFAULT is not defined."
#elif (CVTE_REG_DTV_COLOR_DEFAULT<CVTE_DEF_VIDEO_COLOR_TYPE_MIN) || (CVTE_REG_DTV_COLOR_DEFAULT>CVTE_DEF_VIDEO_COLOR_TYPE_MAX)
#error "CVTE_REG_DTV_COLOR_DEFAULT is out of range."
#endif

/****************************CVTE_REG_DVD_COLOR_DEFAULT******************************
  (1) 默认值:	CVTE_REG_DRIVE_TYPE_DEFAULT
  (2) 范围说明:	CVTE_DEF_VIDEO_COLOR_TYPE_MIN<CVTE_REG_DVD_COLOR_DEFAULT<CVTE_DEF_VIDEO_COLOR_TYPE_MAX
  (3) 功能描述: DVD视频制式默认值检查
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_REG_DVD_COLOR_DEFAULT
#error "CVTE_REG_DVD_COLOR_DEFAULT is not defined."
#elif (CVTE_REG_DVD_COLOR_DEFAULT<CVTE_DEF_VIDEO_COLOR_TYPE_MIN) || (CVTE_REG_DVD_COLOR_DEFAULT>CVTE_DEF_VIDEO_COLOR_TYPE_MAX)
#error "CVTE_REG_DVD_COLOR_DEFAULT is out of range."
#endif

/****************************CVTE_REG_REARVIEW_COLOR_DEFAULT******************************
  (1) 默认值:	CVTE_REG_REARVIEW_COLOR_DEFAULT
  (2) 范围说明:	CVTE_DEF_VIDEO_COLOR_TYPE_MIN<CVTE_REG_REARVIEW_COLOR_DEFAULT<CVTE_DEF_VIDEO_COLOR_TYPE_MAX
  (3) 功能描述: 倒车视频制式默认值检查
  (4) 作者&日期: 吴昊衡@2016/07/09
*******************************************************************************/
#ifndef CVTE_REG_REARVIEW_COLOR_DEFAULT
#error "CVTE_REG_REARVIEW_COLOR_DEFAULT is not defined."
#elif (CVTE_REG_REARVIEW_COLOR_DEFAULT<CVTE_DEF_VIDEO_COLOR_TYPE_MIN) || (CVTE_REG_REARVIEW_COLOR_DEFAULT>CVTE_DEF_VIDEO_COLOR_TYPE_MAX)
#error "CVTE_REG_REARVIEW_COLOR_DEFAULT is out of range."
#endif

////////////////////////////////// RADIO EUROPE //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EUROPE_FM_STEP
#error "CVTE_DEF_RADIO_EUROPE_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN>=CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EUROPE_AM_STEP
#error "CVTE_DEF_RADIO_EUROPE_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN>=CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO USA //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_USA_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_USA_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_USA_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_USA_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_USA_FM_STEP
#error "CVTE_DEF_RADIO_USA_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_USA_FM_FREQ_MIN>=CVTE_DEF_RADIO_USA_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_USA_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_USA_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_USA_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_USA_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_USA_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_USA_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_USA_AM_STEP
#error "CVTE_DEF_RADIO_USA_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_USA_AM_FREQ_MIN>=CVTE_DEF_RADIO_USA_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_USA_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_USA_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO EAST ERNEUROPE //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_STEP
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MIN>=CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_STEP
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MIN>=CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO JAPAN //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_JAPAN_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_JAPAN_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_JAPAN_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_JAPAN_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_JAPAN_FM_STEP
#error "CVTE_DEF_RADIO_JAPAN_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_JAPAN_FM_FREQ_MIN>=CVTE_DEF_RADIO_JAPAN_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_JAPAN_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_JAPAN_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_JAPAN_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_JAPAN_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_JAPAN_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_JAPAN_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_JAPAN_AM_STEP
#error "CVTE_DEF_RADIO_JAPAN_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_JAPAN_AM_FREQ_MIN>=CVTE_DEF_RADIO_JAPAN_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_JAPAN_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_JAPAN_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO KOREAN //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_KOREAN_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_KOREAN_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_KOREAN_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_KOREAN_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_KOREAN_FM_STEP
#error "CVTE_DEF_RADIO_KOREAN_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_KOREAN_FM_FREQ_MIN>=CVTE_DEF_RADIO_KOREAN_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_KOREAN_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_KOREAN_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_KOREAN_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_KOREAN_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_KOREAN_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_KOREAN_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_KOREAN_AM_STEP
#error "CVTE_DEF_RADIO_KOREAN_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_KOREAN_AM_FREQ_MIN>=CVTE_DEF_RADIO_KOREAN_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_KOREAN_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_KOREAN_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO SOUTHAMERICA //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_FM_STEP
#error "CVTE_DEF_RADIO_SOUTHAMERICA_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MIN>=CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_AM_STEP
#error "CVTE_DEF_RADIO_SOUTHAMERICA_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MIN>=CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO TAIWANG //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_TAIWANG_FM_STEP
#error "CVTE_DEF_RADIO_TAIWANG_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MIN>=CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_TAIWANG_AM_STEP
#error "CVTE_DEF_RADIO_TAIWANG_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MIN>=CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO ARABIA AUSTRALIA //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_STEP
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MIN>=CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_STEP
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MIN>=CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO LATIN AMERICA //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_FM_STEP
#error "CVTE_DEF_RADIO_LATIN_AMERICA_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MIN>=CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_AM_STEP
#error "CVTE_DEF_RADIO_LATIN_AMERICA_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MIN>=CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO MIDDLE EAST //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_FM_STEP
#error "CVTE_DEF_RADIO_MIDDLE_EAST_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MIN>=CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_AM_STEP
#error "CVTE_DEF_RADIO_MIDDLE_EAST_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MIN>=CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO AUST //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_AUST_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_AUST_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_AUST_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_AUST_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_AUST_FM_STEP
#error "CVTE_DEF_RADIO_AUST_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_AUST_FM_FREQ_MIN>=CVTE_DEF_RADIO_AUST_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_AUST_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_AUST_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_AUST_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_AUST_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_AUST_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_AUST_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_AUST_AM_STEP
#error "CVTE_DEF_RADIO_AUST_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_AUST_AM_FREQ_MIN>=CVTE_DEF_RADIO_AUST_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_AUST_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_AUST_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO BRAZIL //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_BRAZIL_FM_STEP
#error "CVTE_DEF_RADIO_BRAZIL_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MIN>=CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_BRAZIL_AM_STEP
#error "CVTE_DEF_RADIO_BRAZIL_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MIN>=CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MIN."
#endif

////////////////////////////////// RADIO RUSSIA //////////////////////////////////////////////
#ifndef CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MIN
#error "CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MAX
#error "CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_RUSSIA_FM_STEP
#error "CVTE_DEF_RADIO_RUSSIA_FM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MIN>=CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MAX)
#error "CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MIN."
#endif

#ifndef CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MIN
#error "CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MIN is not defined."
#endif

#ifndef CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MAX
#error "CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MAX is not defined."
#endif

#ifndef CVTE_DEF_RADIO_RUSSIA_AM_STEP
#error "CVTE_DEF_RADIO_RUSSIA_AM_STEP is not defined."
#endif

#if (CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MIN>=CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MAX)
#error "CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MAX must be bigger than CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MIN."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RDS
#error "CVTE_EN_RDS is not defined."
#elif (CVTE_EN_RDS != CVTE_DISABLE) && (CVTE_EN_RDS != CVTE_ENABLE)
#error "CVTE_EN_RDS must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RBDS
#error "CVTE_EN_RBDS is not defined."
#elif (CVTE_EN_RBDS != CVTE_DISABLE) && (CVTE_EN_RBDS != CVTE_ENABLE)
#error "CVTE_EN_RBDS must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RDS_TMC
#error "CVTE_EN_RDS_TMC is not defined."
#elif (CVTE_EN_RDS_TMC != CVTE_DISABLE) && (CVTE_EN_RDS_TMC != CVTE_ENABLE)
#error "CVTE_EN_RDS_TMC must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_DAB
#error "CVTE_EN_DAB is not defined."
#elif (CVTE_EN_DAB != CVTE_DISABLE) && (CVTE_EN_DAB != CVTE_ENABLE)
#error "CVTE_EN_DAB must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_LED
#error "CVTE_EN_LED is not defined."
#elif (CVTE_EN_LED != CVTE_DISABLE) && (CVTE_EN_LED != CVTE_ENABLE)
#error "CVTE_EN_LED must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RADIO
#error "CVTE_EN_RADIO is not defined."
#elif (CVTE_EN_RADIO != CVTE_DISABLE) && (CVTE_EN_RADIO != CVTE_ENABLE)
#error "CVTE_EN_RADIO must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_CARPLAY
#error "CVTE_EN_CARPLAY is not defined."
#elif (CVTE_EN_CARPLAY != CVTE_DISABLE) && (CVTE_EN_CARPLAY != CVTE_ENABLE)
#error "CVTE_EN_CARPLAY must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_IPOD
#error "CVTE_EN_IPOD is not defined."
#elif (CVTE_EN_IPOD != CVTE_DISABLE) && (CVTE_EN_IPOD != CVTE_ENABLE)
#error "CVTE_EN_IPOD must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_EASYCONNECT
#error "CVTE_EN_EASYCONNECT is not defined."
#elif (CVTE_EN_EASYCONNECT != CVTE_DISABLE) && (CVTE_EN_EASYCONNECT != CVTE_ENABLE)
#error "CVTE_EN_EASYCONNECT must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_BT_PHONE
#error "CVTE_EN_BT_PHONE is not defined."
#elif (CVTE_EN_BT_PHONE != CVTE_DISABLE) && (CVTE_EN_BT_PHONE != CVTE_ENABLE)
#error "CVTE_EN_BT_PHONE must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_BT_A2DP
#error "CVTE_EN_BT_A2DP is not defined."
#elif (CVTE_EN_BT_A2DP != CVTE_DISABLE) && (CVTE_EN_BT_A2DP != CVTE_ENABLE)
#error "CVTE_EN_BT_A2DP must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_NAVI
#error "CVTE_EN_NAVI is not defined."
#elif (CVTE_EN_NAVI != CVTE_DISABLE) && (CVTE_EN_NAVI != CVTE_ENABLE)
#error "CVTE_EN_NAVI must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_USB
#error "CVTE_EN_USB is not defined."
#elif (CVTE_EN_USB != CVTE_DISABLE) && (CVTE_EN_USB != CVTE_ENABLE)
#error "CVTE_EN_USB must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_SD
#error "CVTE_EN_SD is not defined."
#elif (CVTE_EN_SD != CVTE_DISABLE) && (CVTE_EN_SD != CVTE_ENABLE)
#error "CVTE_EN_SD must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_DVD
#error "CVTE_EN_DVD is not defined."
#elif (CVTE_EN_DVD != CVTE_DISABLE) && (CVTE_EN_DVD != CVTE_ENABLE)
#error "CVTE_EN_DVD must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_DTV
#error "CVTE_EN_DTV is not defined."
#elif (CVTE_EN_DTV != CVTE_DISABLE) && (CVTE_EN_DTV != CVTE_ENABLE)
#error "CVTE_EN_DTV must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_AUX
#error "CVTE_EN_AUX is not defined."
#elif (CVTE_EN_AUX != CVTE_DISABLE) && (CVTE_EN_AUX != CVTE_ENABLE)
#error "CVTE_EN_AUX must be either DISABLE or ENABLE."
#endif

#ifndef CVTE_EN_ISDB
#error "CVTE_EN_AUX is not defined."
#elif (CVTE_EN_ISDB != CVTE_DISABLE) && (CVTE_EN_AUX != CVTE_ENABLE)
#error "CVTE_EN_AUX must be either DISABLE or ENABLE."
#endif


////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RDS
#error "CVTE_EN_RDS is not defined."
#elif (CVTE_EN_RDS != CVTE_DISABLE) && (CVTE_EN_RDS != CVTE_ENABLE)
#error "CVTE_EN_RDS must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RBDS
#error "CVTE_EN_RBDS is not defined."
#elif (CVTE_EN_RBDS != CVTE_DISABLE) && (CVTE_EN_RBDS != CVTE_ENABLE)
#error "CVTE_EN_RBDS must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RDS_TMC
#error "CVTE_EN_RDS_TMC is not defined."
#elif (CVTE_EN_RDS_TMC != CVTE_DISABLE) && (CVTE_EN_RDS_TMC != CVTE_ENABLE)
#error "CVTE_EN_RDS_TMC must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_DAB
#error "CVTE_EN_DAB is not defined."
#elif (CVTE_EN_DAB != CVTE_DISABLE) && (CVTE_EN_DAB != CVTE_ENABLE)
#error "CVTE_EN_DAB must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_CAMERA
#error "CVTE_EN_CAMERA is not defined."
#elif (CVTE_EN_CAMERA != CVTE_DISABLE) && (CVTE_EN_CAMERA != CVTE_ENABLE)
#error "CVTE_EN_CAMERA must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_DSP
#error "CVTE_EN_DSP is not defined."
#elif (CVTE_EN_DSP != CVTE_DISABLE) && (CVTE_EN_DSP != CVTE_ENABLE)
#error "CVTE_EN_DSP must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_ANDROID_AUTO
#error "CVTE_EN_ANDROID_AUTO is not defined."
#elif (CVTE_EN_ANDROID_AUTO != CVTE_DISABLE) && (CVTE_EN_ANDROID_AUTO != CVTE_ENABLE)
#error "CVTE_EN_ANDROID_AUTO must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_WEBLINK
#error "CVTE_EN_WEBLINK is not defined."
#elif (CVTE_EN_WEBLINK != CVTE_DISABLE) && (CVTE_EN_WEBLINK != CVTE_ENABLE)
#error "CVTE_EN_WEBLINK must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_PHONELINK
#error "CVTE_EN_PHONELINK is not defined."
#elif (CVTE_EN_PHONELINK != CVTE_DISABLE) && (CVTE_EN_PHONELINK != CVTE_ENABLE)
#error "CVTE_EN_PHONELINK must be either DISABLE or ENABLE."
#endif
/*******************************************************************************
***********************FUNCTION ENABLE/DISABLE CHECK***************************
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_IDLE_LOGO
#error "CVTE_EN_IDLE_LOGO is not defined."
#elif (CVTE_EN_IDLE_LOGO != CVTE_DISABLE) && (CVTE_EN_IDLE_LOGO != CVTE_ENABLE)
#error "CVTE_EN_IDLE_LOGO must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
#error "CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT is not defined."
#elif (CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT != CVTE_DISABLE) && (CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT != CVTE_ENABLE)
#error "CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
#error "CVTE_EN_STEER_WHEEL_OLD_UI_STYLE is not defined."
#elif (CVTE_EN_STEER_WHEEL_OLD_UI_STYLE != CVTE_DISABLE) && (CVTE_EN_STEER_WHEEL_OLD_UI_STYLE != CVTE_ENABLE)
#error "CVTE_EN_STEER_WHEEL_OLD_UI_STYLE must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_NAVI_DROP_BAR
#error "CVTE_EN_NAVI_DROP_BAR is not defined."
#elif (CVTE_EN_NAVI_DROP_BAR != CVTE_DISABLE) && (CVTE_EN_NAVI_DROP_BAR != CVTE_ENABLE)
#error "CVTE_EN_NAVI_DROP_BAR must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_DYNAMIC_LOGO
#error "CVTE_EN_DYNAMIC_LOGO is not defined."
#elif (CVTE_EN_DYNAMIC_LOGO != CVTE_DISABLE) && (CVTE_EN_DYNAMIC_LOGO != CVTE_ENABLE)
#error "CVTE_EN_DYNAMIC_LOGO must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_KEY_PROXY
#error "CVTE_EN_KEY_PROXY is not defined."
#elif (CVTE_EN_KEY_PROXY != CVTE_DISABLE) && (CVTE_EN_KEY_PROXY != CVTE_ENABLE)
#error "CVTE_EN_KEY_PROXY must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_PACKAGE_UPDATE
#error "CVTE_EN_PACKAGE_UPDATE is not defined."
#elif (CVTE_EN_PACKAGE_UPDATE != CVTE_DISABLE) && (CVTE_EN_PACKAGE_UPDATE != CVTE_ENABLE)
#error "CVTE_EN_PACKAGE_UPDATE must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON
#error "CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON is not defined."
#elif (CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON != CVTE_DISABLE) && (CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON != CVTE_ENABLE)
#error "CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_AUDIO_SUBWOOFER
#error "CVTE_EN_AUDIO_SUBWOOFER is not defined."
#elif (CVTE_EN_AUDIO_SUBWOOFER != CVTE_DISABLE) && (CVTE_EN_AUDIO_SUBWOOFER != CVTE_ENABLE)
#error "CVTE_EN_AUDIO_SUBWOOFER must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
#error "CVTE_EN_RADIO_SUBWOOFER_FREQ_ADJUST is not defined."
#elif (CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST != CVTE_DISABLE) && (CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST != CVTE_ENABLE)
#error "CVTE_EN_RADIO_SUBWOOFER_FREQ_ADJUST must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL
#error "CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL is not defined."
#elif (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL != CVTE_DISABLE) && (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL != CVTE_ENABLE)
#error "CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_BORDER_TOUCH
#error "CVTE_EN_BORDER_TOUCH is not defined."
#elif (CVTE_EN_BORDER_TOUCH != CVTE_DISABLE) && (CVTE_EN_BORDER_TOUCH != CVTE_ENABLE)
#error "CVTE_EN_BORDER_TOUCH must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_ACCON_WARNING
#error "CVTE_EN_ACCON_WARNING is not defined."
#elif (CVTE_EN_ACCON_WARNING != CVTE_DISABLE) && (CVTE_EN_ACCON_WARNING != CVTE_ENABLE)
#error "CVTE_EN_ACCON_WARNING must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_HOME_WIDGET_ANIM
#error "CVTE_EN_HOME_WIDGET_ANIM is not defined."
#elif (CVTE_EN_HOME_WIDGET_ANIM != CVTE_DISABLE) && (CVTE_EN_HOME_WIDGET_ANIM != CVTE_ENABLE)
#error "CVTE_EN_HOME_WIDGET_ANIM must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON
#error "CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON is not defined."
#elif (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON != CVTE_DISABLE) && (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON != CVTE_ENABLE)
#error "CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY
#error "CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY is not defined."
#elif (CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY != CVTE_DISABLE) && (CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY != CVTE_ENABLE)
#error "CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
#error "CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE is not defined."
#elif (CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE != CVTE_DISABLE) && (CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE != CVTE_ENABLE)
#error "CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_OS_DYNAMIC_LOGO
#error "CVTE_EN_OS_DYNAMIC_LOGO_CONSOLE is not defined."
#elif (CVTE_EN_OS_DYNAMIC_LOGO != CVTE_DISABLE) && (CVTE_EN_OS_DYNAMIC_LOGO != CVTE_ENABLE)
#error "CVTE_EN_OS_DYNAMIC_LOGO must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_CUSTOMER_STATUS_BAR_LOGO
#error "CVTE_CUSTOMER_STATUS_BAR_LOGO is not defined."
#elif (CVTE_CUSTOMER_STATUS_BAR_LOGO != CVTE_DISABLE) && (CVTE_CUSTOMER_STATUS_BAR_LOGO != CVTE_ENABLE)
#error "CVTE_CUSTOMER_STATUS_BAR_LOGO must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_GESTURE_AIR
#error "CVTE_EN_GESTURE_AIR is not defined."
#elif (CVTE_EN_GESTURE_AIR != CVTE_DISABLE) && (CVTE_EN_GESTURE_AIR != CVTE_ENABLE)
#error "CVTE_EN_GESTURE_AIR must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
#error "CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT is not defined."
#elif (CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT != CVTE_DISABLE) && (CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT != CVTE_ENABLE)
#error "CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_CARPLAY_LIMIT_UI
#error "CVTE_EN_CARPLAY_LIMIT_UI is not defined."
#elif (CVTE_EN_CARPLAY_LIMIT_UI != CVTE_DISABLE) && (CVTE_EN_CARPLAY_LIMIT_UI != CVTE_ENABLE)
#error "CVTE_EN_CARPLAY_LIMIT_UI must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_CARPLAY_NIGHTMODE
#error "CVTE_EN_CARPLAY_NIGHTMODE is not defined."
#elif (CVTE_EN_CARPLAY_NIGHTMODE != CVTE_DISABLE) && (CVTE_EN_CARPLAY_NIGHTMODE != CVTE_ENABLE)
#error "CVTE_EN_CARPLAY_NIGHTMODE must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_BT_VCARD_NAME_ORDER
#error "CVTE_EN_BT_VCARD_NAME_ORDER is not defined."
#elif (CVTE_EN_BT_VCARD_NAME_ORDER != CVTE_DISABLE) && (CVTE_EN_BT_VCARD_NAME_ORDER != CVTE_ENABLE)
#error "CVTE_EN_BT_VCARD_NAME_ORDER must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_SEARCH_USE_FULL_KEYBOARD
#error "CVTE_EN_SEARCH_USE_FULL_KEYBOARD is not defined."
#elif (CVTE_EN_SEARCH_USE_FULL_KEYBOARD != CVTE_DISABLE) && (CVTE_EN_SEARCH_USE_FULL_KEYBOARD != CVTE_ENABLE)
#error "CVTE_EN_SEARCH_USE_FULL_KEYBOARD must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
#error "CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL is not defined."
#elif (CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL != CVTE_DISABLE) && (CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL != CVTE_ENABLE)
#error "CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_RDS_AF_STOP_LEVEL
#error "CVTE_EN_RDS_AF_STOP_LEVEL is not defined."
#elif (CVTE_EN_RDS_AF_STOP_LEVEL != CVTE_DISABLE) && (CVTE_EN_RDS_AF_STOP_LEVEL != CVTE_ENABLE)
#error "CVTE_EN_RDS_AF_STOP_LEVEL must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_BT_PHONECALL_MEDIA_RING
#error "CVTE_EN_BT_PHONECALL_MEDIA_RING is not defined."
#elif (CVTE_EN_BT_PHONECALL_MEDIA_RING != CVTE_DISABLE) && (CVTE_EN_BT_PHONECALL_MEDIA_RING != CVTE_ENABLE)
#error "CVTE_EN_BT_PHONECALL_MEDIA_RING must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME
#error "CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME is not defined."
#elif (CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME != CVTE_DISABLE) && (CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME != CVTE_ENABLE)
#error "CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_USER_UPDATE_LOGO
#error "CVTE_EN_USER_UPDATE_LOGO is not defined."
#elif (CVTE_EN_USER_UPDATE_LOGO != CVTE_DISABLE) && (CVTE_EN_USER_UPDATE_LOGO != CVTE_ENABLE)
#error "CVTE_EN_USER_UPDATE_LOGO must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_LED_SIRI_ANIM
#error "CVTE_EN_LED_SIRI_ANIM is not defined."
#elif (CVTE_EN_LED_SIRI_ANIM != CVTE_DISABLE) && (CVTE_EN_LED_SIRI_ANIM != CVTE_ENABLE)
#error "CVTE_EN_LED_SIRI_ANIM must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_SHOW_STW_VOLTAGE_VALUE
#error "CVTE_EN_SHOW_STW_VOLTAGE_VALUE is not defined."
#elif (CVTE_EN_SHOW_STW_VOLTAGE_VALUE != CVTE_DISABLE) && (CVTE_EN_SHOW_STW_VOLTAGE_VALUE != CVTE_ENABLE)
#error "CVTE_EN_SHOW_STW_VOLTAGE_VALUE must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_CDP_DCP_CONTROL
#error "CVTE_EN_CDP_DCP_CONTROL is not defined."
#elif (CVTE_EN_CDP_DCP_CONTROL != CVTE_DISABLE) && (CVTE_EN_CDP_DCP_CONTROL != CVTE_ENABLE)
#error "CVTE_EN_CDP_DCP_CONTROL must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
#error "CVTE_EN_EXTENSIBLE_MCU_PROTOCOL is not defined."
#elif (CVTE_EN_EXTENSIBLE_MCU_PROTOCOL != CVTE_DISABLE) && (CVTE_EN_EXTENSIBLE_MCU_PROTOCOL != CVTE_ENABLE)
#error "CVTE_EN_EXTENSIBLE_MCU_PROTOCOL must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX
#error "CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX is not defined."
#elif (CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX != CVTE_DISABLE) && (CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX != CVTE_ENABLE)
#error "CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_VIDEO_DECODE_SUPPORT_XVID
#error "CVTE_EN_VIDEO_DECODE_SUPPORT_XVID is not defined."
#elif (CVTE_EN_VIDEO_DECODE_SUPPORT_XVID != CVTE_DISABLE) && (CVTE_EN_VIDEO_DECODE_SUPPORT_XVID != CVTE_ENABLE)
#error "CVTE_EN_VIDEO_DECODE_SUPPORT_XVID must be either DISABLE or ENABLE."
#endif

////////////////////////////////////////////////////////////////////////////////
#ifndef CVTE_EN_VIDEO_DECODE_SUPPORT_H264
#error "CVTE_EN_VIDEO_DECODE_SUPPORT_H264 is not defined."
#elif (CVTE_EN_VIDEO_DECODE_SUPPORT_H264 != CVTE_DISABLE) && (CVTE_EN_VIDEO_DECODE_SUPPORT_H264 != CVTE_ENABLE)
#error "CVTE_EN_VIDEO_DECODE_SUPPORT_H264 must be either DISABLE or ENABLE."
#endif
/***********************CVTE_DEF_AUDIO_SUPPORT_FORMAT***************************
  (1) 默认值:	(CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WAV \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP3 \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_M4A \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WMA \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_APE \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLA \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLAC \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_OGG \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_AMR \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP2 \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_M4R \
				|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_RA )
  (2) 范围说明:	(CVTE_DEF_AUDIO_SUPPORT_FORMAT&CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MASK) != 0
  (3) 功能描述: 音频文件支持格式
  (4) 作者&日期: zhaoshunxian@2016/06/29
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_SUPPORT_FORMAT
#error "CVTE_DEF_AUDIO_SUPPORT_FORMAT is not defined."
#elif (CVTE_DEF_AUDIO_SUPPORT_FORMAT&CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MASK) == 0
#error "CVTE_DEF_AUDIO_SUPPORT_FORMAT must be defined an available value."
#endif


/***********************CVTE_DEF_VIDEO_SUPPORT_FORMAT***************************
  (1) 默认值:	(CVTE_VIDEO_SUPPORT_FORMAT_TYPE_AVI \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPG \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPEG \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_VOB \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TS \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TP \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_OGM \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MKV \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RM \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RMVB \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MP4 \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_M4V \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MOV \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_3GP \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_ASF \
				|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_WMV )
  (2) 范围说明:	(CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MASK) != 0
  (3) 功能描述: 视频文件支持格式
  (4) 作者&日期: wuhaoheng@2016/10/19
*******************************************************************************/
#ifndef CVTE_DEF_VIDEO_SUPPORT_FORMAT
#error "CVTE_DEF_VIDEO_SUPPORT_FORMAT is not defined."
#elif (CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MASK) == 0
#error "CVTE_DEF_VIDEO_SUPPORT_FORMAT must be defined an available value."
#endif

/***********************CVTE_DEF_LANGUAGE_SUPPORT***************************
 (1) 默认值:	(CVTE_LANGUAGE_SUPPORT_ENGLISH \
				|CVTE_LANGUAGE_SUPPORT_CHINESE_TRA \
				|CVTE_LANGUAGE_SUPPORT_CHINESE_SIM \
				|CVTE_LANGUAGE_SUPPORT_KOREAN \
				|CVTE_LANGUAGE_SUPPORT_GERMAN \
				|CVTE_LANGUAGE_SUPPORT_SPANISH \
				|CVTE_LANGUAGE_SUPPORT_PORTUGESE \
				|CVTE_LANGUAGE_SUPPORT_ITALIAN \
				|CVTE_LANGUAGE_SUPPORT_FRENCH \
				|CVTE_LANGUAGE_SUPPORT_JAPANESS \
				|CVTE_LANGUAGE_SUPPORT_RUSSIAN )
  (2) 范围说明:	(CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_MASK) != 0
  (3) 功能描述: 语言支持
  (4) 作者&日期: 吴昊衡@2016/10/19
*******************************************************************************/
#ifndef CVTE_DEF_LANGUAGE_SUPPORT
#error "CVTE_DEF_LANGUAGE_SUPPORT is not defined."
#elif (CVTE_DEF_LANGUAGE_SUPPORT & CVTE_LANGUAGE_SUPPORT_MASK) == 0
#error "CVTE_DEF_LANGUAGE_SUPPORT must be defined an available value."
#endif

/***********************CVTE_DEF_RADIO_REGION_SUPPORT***************************
 (1) 默认值:
				(CVTE_RADIO_REGION_SUPPORT_EUROPE \
				|CVTE_RADIO_REGION_SUPPORT_USA \
				|CVTE_RADIO_REGION_SUPPORT_EAST_ERNEUROPE \
				|CVTE_RADIO_REGION_SUPPORT_JAPAN \
				|CVTE_RADIO_REGION_SUPPORT_EUROPE_EASTERNEUROPE \
				|CVTE_RADIO_REGION_SUPPORT_KOREAN \
				|CVTE_RADIO_REGION_SUPPORT_TAIWANG \
				|CVTE_RADIO_REGION_SUPPORT_USA2	 \
				|CVTE_RADIO_REGION_SUPPORT_SOUTHAMERICA \
				|CVTE_RADIO_REGION_SUPPORT_ARABIA_AUSTRALIA \
				|CVTE_RADIO_REGION_SUPPORT_LATIN_AMERICA \
				|CVTE_RADIO_REGION_SUPPORT_MIDDLE_EAST \
				|CVTE_RADIO_REGION_SUPPORT_AUST	 \
				|CVTE_RADIO_REGION_SUPPORT_BRAZIL \
				|CVTE_RADIO_REGION_SUPPORT_EASTERNEUROPE_EUROPE)
  (2) 范围说明:	(CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_MASK) != 0
  (3) 功能描述: 语言支持
  (4) 作者&日期: 吴昊衡@2016/10/19
*******************************************************************************/
#ifndef CVTE_DEF_RADIO_REGION_SUPPORT
#error "CVTE_DEF_RADIO_REGION_SUPPORT is not defined."
#elif (CVTE_DEF_RADIO_REGION_SUPPORT & CVTE_RADIO_REGION_SUPPORT_MASK) == 0
#error "CVTE_DEF_RADIO_REGION_SUPPORT must be defined an available value."
#endif

/******************************CVTE_EN_CARPLAY**********************************
  (1) 默认值: CVTE_ENABLE
  (2) 范围说明:	CVTE_ENABLE or CVTE_DISABLE
  (3) 功能描述: CarPlay 功能开关
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_EN_CARPLAY
#error "CVTE_EN_CARPLAY is not defined."
#elif (CVTE_EN_CARPLAY != CVTE_DISABLE) && (CVTE_EN_CARPLAY != CVTE_ENABLE)
#error "CVTE_EN_CARPLAY must be either DISABLE or ENABLE."
#endif

/**************************CVTE_DEF_CARPLAY_ICON_LABEL**************************
  (1) 默认值: "Vehicle"
  (2) 范围说明:	任意UTF8字符串
  (3) 功能描述: CarPlay Icon下的Lable字符串
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_CARPLAY_ICON_LABEL
#error "CVTE_DEF_CARPLAY_ICON_LABEL is not defined."
#endif

/**************************CVTE_DEF_SCREEN_VIDEOWIDTH**************************
  (1) 默认值: 800
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: 屏幕显示宽度,单位是像素
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_SCREEN_VIDEOWIDTH
#error "CVTE_DEF_SCREEN_VIDEOWIDTH is not defined."
#elif (CVTE_DEF_SCREEN_VIDEOWIDTH <= 0)
#error "CVTE_DEF_SCREEN_VIDEOWIDTH must be greater than 0."
#endif

/**************************CVTE_DEF_SCREEN_VIDEOHEIGHT**************************
  (1) 默认值: 480
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: 屏幕显示高度,单位是像素
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_SCREEN_VIDEOHEIGHT
#error "CVTE_DEF_SCREEN_VIDEOHEIGHT is not defined."
#elif (CVTE_DEF_SCREEN_VIDEOHEIGHT <= 0)
#error "CVTE_DEF_SCREEN_VIDEOHEIGHT must be greater than 0."
#endif

/**********************CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH***********************
  (1) 默认值: CVTE_DEF_SCREEN_VIDEOHEIGHT
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: 屏幕显示宽度,单位是像素
  (4) 作者&日期: zhaoshunxian@2016/11/22
*******************************************************************************/
#ifndef CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH
#error "CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH is not defined."
#elif (CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH <= 0)
#error "CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH must be greater than 0."
#endif

/**********************CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT**********************
  (1) 默认值: CVTE_DEF_SCREEN_VIDEOHEIGHT
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: 屏幕显示高度,单位是像素
  (4) 作者&日期: zhaoshunxian@2016/11/22
*******************************************************************************/
#ifndef CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT
#error "CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT is not defined."
#elif (CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT <= 0)
#error "CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT must be greater than 0."
#endif

/********************CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH*********************
  (1) 默认值: CVTE_DEF_SCREEN_VIDEOHEIGHT
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: AndroidAuto屏幕显示宽度,单位是像素
  (4) 作者&日期: zhaoshunxian@2016/11/26
*******************************************************************************/
#ifndef CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH
#error "CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH is not defined."
#elif (CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH <= 0)
#error "CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH must be greater than 0."
#endif

/********************CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT********************
  (1) 默认值: CVTE_DEF_SCREEN_VIDEOHEIGHT
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: AndroidAuto屏幕显示高度,单位是像素
  (4) 作者&日期: zhaoshunxian@2016/11/26
*******************************************************************************/
#ifndef CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT
#error "CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT is not defined."
#elif (CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT <= 0)
#error "CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT must be greater than 0."
#endif

/**************************CVTE_DEF_SCREEN_PHYSICALWIDTH************************
  (1) 默认值: 137
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: 屏幕物理尺寸宽度,单位是毫米
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_SCREEN_PHYSICALWIDTH
#error "CVTE_DEF_SCREEN_PHYSICALWIDTH is not defined."
#elif (CVTE_DEF_SCREEN_PHYSICALWIDTH <= 0)
#error "CVTE_DEF_SCREEN_PHYSICALWIDTH must be greater than 0."
#endif

/**************************CVTE_DEF_SCREEN_PHYSICALHEIGHT***********************
  (1) 默认值: 79
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: 屏幕物理尺寸高度,单位是毫米
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_SCREEN_PHYSICALHEIGHT
#error "CVTE_DEF_SCREEN_PHYSICALHEIGHT is not defined."
#elif (CVTE_DEF_SCREEN_PHYSICALHEIGHT <= 0)
#error "CVTE_DEF_SCREEN_PHYSICALHEIGHT must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_XXX************************/

/**************************CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT****************
  (1) 默认值: 1
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Main Default类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT******************
  (1) 默认值: 1
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Main Alert类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA******************
  (1) 默认值: 1
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Main Media类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY**************
  (1) 默认值: 1
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Main Telephone类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION******
  (1) 默认值: 1
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Main SpeechRecognition类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY**********
  (1) 默认值: 1
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Main Compatibility类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT*****************
  (1) 默认值: 0
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Alt Default类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT
#error "CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY***********
  (1) 默认值: 0
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Alt Compatibility类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY
#error "CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA**************
  (1) 默认值: 1
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay MainHigh Media类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA must be greater than 0."
#endif

/**************************CVTE_DEF_AUDIO_DEVICE_ID_INPUT***********************
  (1) 默认值: 0
  (2) 范围说明:	大于等于0的任意整数
  (3) 功能描述: CarPlay Input类型音频默认Device ID
  (4) 作者&日期: zhaoshunxian@2016/07/04
*******************************************************************************/
#ifndef CVTE_DEF_AUDIO_DEVICE_ID_INPUT
#error "CVTE_DEF_AUDIO_DEVICE_ID_INPUT is not defined."
#elif (CVTE_DEF_AUDIO_DEVICE_ID_INPUT < 0)
#error "CVTE_DEF_AUDIO_DEVICE_ID_INPUT must be greater than 0."
#endif

/******************************************************************************/

/***************************CVTE_DEF_XXX_VERSION*******************************/

/****************************CVTE_DEF_APP_VERSION*******************************
  (1) 默认值:
  (2) 范围说明:	任意字符串
  (3) 功能描述: APP版本号
  (4) 作者&日期: zhaoshunxian@2016/07/05
*******************************************************************************/
#ifndef CVTE_DEF_APP_VERSION
#error "CVTE_DEF_APP_VERSION is not defined."
#endif

/****************************CVTE_DEF_HARDWARE_VERSION**************************
  (1) 默认值:
  (2) 范围说明:	任意字符串
  (3) 功能描述: 硬件版本号
  (4) 作者&日期: zhaoshunxian@2016/07/05
*******************************************************************************/
#ifndef CVTE_DEF_HARDWARE_VERSION
#error "CVTE_DEF_HARDWARE_VERSION is not defined."
#endif

/******************************************************************************/

/****************************CVTE_DEF_CARPLAY_GPS_UART_PORT**************************
  (1) 默认值: 6
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: CarPlay读取GPS报文的串口号
  (4) 作者&日期: zhaoshunxian@2016/07/05
*******************************************************************************/
#ifndef CVTE_DEF_CARPLAY_GPS_UART_PORT
#error "CVTE_DEF_CARPLAY_GPS_UART_PORT is not defined."
#elif (CVTE_DEF_CARPLAY_GPS_UART_PORT <= 0)
#error "CVTE_DEF_CARPLAY_GPS_UART_PORT must be greater than 0."
#endif

/***********************CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT**********************
  (1) 默认值: 6
  (2) 范围说明:	大于0的任意整数
  (3) 功能描述: AndroidAuto读取GPS报文的串口号
  (4) 作者&日期: zhaoshunxian@2016/11/26
*******************************************************************************/
#ifndef CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT
#error "CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT is not defined."
#elif (CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT <= 0)
#error "CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT must be greater than 0."
#endif

/*******************************************************************************
***********************REG ENABLE/DISABLE CHECK***************************
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//Auto Start Navi
#ifndef CVTE_REG_AUTO_START_NAVI_DEFAULT
#error "CVTE_REG_AUTO_START_NAVI_DEFAULT is not defined."
#elif (CVTE_REG_AUTO_START_NAVI_DEFAULT != CVTE_DISABLE) && (CVTE_REG_AUTO_START_NAVI_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_AUTO_START_NAVI_DEFAULT must be either DISABLE or ENABLE."
#endif

//Key Beep
#ifndef CVTE_REG_KEEP_BEEP_DEFAULT
#error "CVTE_REG_KEEP_BEEP_DEFAULT is not defined."
#elif (CVTE_REG_KEEP_BEEP_DEFAULT != CVTE_DISABLE) && (CVTE_REG_KEEP_BEEP_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_KEEP_BEEP_DEFAULT must be either DISABLE or ENABLE."
#endif

//Reverse Volume Auto Adjustment
#ifndef CVTE_REG_REAR_VOL_CONTROL_DEFAULT
#error "CVTE_REG_REAR_VOL_CONTROL_DEFAULT is not defined."
#elif (CVTE_REG_REAR_VOL_CONTROL_DEFAULT != CVTE_DISABLE) && (CVTE_REG_REAR_VOL_CONTROL_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_REAR_VOL_CONTROL_DEFAULT must be either DISABLE or ENABLE."
#endif

//Display Blur Effect
#ifndef CVTE_REG_BLUR_EFFECT_DEFAULT
#error "CVTE_REG_BLUR_EFFECT_DEFAULT is not defined."
#elif (CVTE_REG_BLUR_EFFECT_DEFAULT != CVTE_DISABLE) && (CVTE_REG_BLUR_EFFECT_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_BLUR_EFFECT_DEFAULT must be either DISABLE or ENABLE."
#endif

//Led Anim Effect
#ifndef CVTE_REG_LED_ANIM_EFFECT_DEFAULT
#error "CVTE_REG_LED_ANIM_EFFECT_DEFAULT is not defined."
#elif (CVTE_REG_LED_ANIM_EFFECT_DEFAULT != CVTE_DISABLE) && (CVTE_REG_LED_ANIM_EFFECT_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_LED_ANIM_EFFECT_DEFAULT must be either DISABLE or ENABLE."
#endif

//Activate Factory Mode
#ifndef CVTE_REG_FACTORY_MODE_DEFAULT
#error "CVTE_REG_FACTORY_MODE_DEFAULT is not defined."
#elif (CVTE_REG_FACTORY_MODE_DEFAULT != CVTE_DISABLE) && (CVTE_REG_FACTORY_MODE_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_FACTORY_MODE_DEFAULT must be either DISABLE or ENABLE."
#endif

//Snapshot Enable
#ifndef CVTE_REG_SNAPSHOT_ENABLE_DEFAULT
#error "CVTE_REG_SNAPSHOT_ENABLE_DEFAULT is not defined."
#elif (CVTE_REG_SNAPSHOT_ENABLE_DEFAULT != CVTE_DISABLE) && (CVTE_REG_SNAPSHOT_ENABLE_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_SNAPSHOT_ENABLE_DEFAULT must be either DISABLE or ENABLE."
#endif

//DAB Enable
#ifndef CVTE_REG_DAB_ENABLE_DEFAULT
#error "CVTE_REG_DAB_ENABLE_DEFAULT is not defined."
#elif (CVTE_REG_DAB_ENABLE_DEFAULT != CVTE_DISABLE) && (CVTE_REG_DAB_ENABLE_DEFAULT != CVTE_ENABLE)
#error "CVTE_REG_DAB_ENABLE_DEFAULT must be either DISABLE or ENABLE."
#endif

//CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
#ifndef CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
#error "CVTE_EN_RADIO_DEPART_DAB_UI_STYLE is not defined."
#elif (CVTE_EN_RADIO_DEPART_DAB_UI_STYLE != CVTE_DISABLE) && (CVTE_EN_RADIO_DEPART_DAB_UI_STYLE != CVTE_ENABLE)
#error "CVTE_EN_RADIO_DEPART_DAB_UI_STYLE must be either DISABLE or ENABLE."
#endif

//PHONE BOOK SAVE TO LOCAL DAT
#ifndef SAVE_PHONEBOOK_TO_LOCAL
#error "SAVE_PHONEBOOK_TO_LOCAL is not defined."
#elif (SAVE_PHONEBOOK_TO_LOCAL != CVTE_DISABLE) && (SAVE_PHONEBOOK_TO_LOCAL != CVTE_ENABLE)
#error "SAVE_PHONEBOOK_TO_LOCAL must be either DISABLE or ENABLE."
#endif

//蓝牙配对码弹出提示
#ifndef CVTE_EN_BT_PAIRED_CODE_PERMIT
#error "CVTE_EN_BT_PAIRED_CODE_PERMIT is not defined."
#elif (CVTE_EN_BT_PAIRED_CODE_PERMIT != CVTE_DISABLE) && (CVTE_EN_BT_PAIRED_CODE_PERMIT != CVTE_ENABLE)
#error "CVTE_EN_BT_PAIRED_CODE_PERMIT must be either DISABLE or ENABLE."
#endif

//CPCA Enable
#ifndef CVTE_EN_CPCA
#error "CVTE_EN_CPCA is not defined."
#elif (CVTE_EN_CPCA != CVTE_DISABLE) && (CVTE_EN_CPCA != CVTE_ENABLE)
#error "CVTE_EN_CPCA must be either DISABLE or ENABLE."
#endif

//CPMAC Enable

#ifndef CVTE_EN_CPMAC
#error "CVTE_EN_CPMAC is not defined."
#elif (CVTE_EN_CPMAC != CVTE_DISABLE) && (CVTE_EN_CPMAC != CVTE_ENABLE)
#error "CVTE_EN_CPMAC must be either DISABLE or ENABLE."
#elif (CVTE_EN_CPMAC == CVTE_ENABLE) && (CVTE_EN_CARPLAY != CVTE_ENABLE)
#error "CVTE_EN_CARPLAY must be ENABLE when CVTE_EN_CPMAC is ENABLE."
#endif

/****************************CVTE_DEF_CARPLAY_RESTART_TIME_MS*******************
  (1) 默认值: 10000(10s)
  (2) 范围说明:	大于等于0的任意整数,为0时关闭此功能。
  (3) 功能描述: CarPlay插入后无法连接的超时时间,超时后自动重启Client,单位毫秒ms。
  (4) 作者&日期: zhaoshunxian@2016/10/18
*******************************************************************************/
#ifndef CVTE_DEF_CARPLAY_RESTART_TIME_MS
#error "CVTE_DEF_CARPLAY_RESTART_TIME_MS is not defined."
#elif (CVTE_DEF_CARPLAY_RESTART_TIME_MS < 0)
#error "CVTE_DEF_CARPLAY_RESTART_TIME_MS must be greater than or equal to 0."
#endif

/****************************CVTE_EN_COMBINE_ANDROID_LINK*******************
  (1) 默认值: CVTE_DISABLE
  (2) 范围说明:	
  (3) 功能描述: 同时打开AndroidAtuo功能和WebLink功能，他们之间可以通过设置菜单相互切换。
  (4) 作者&日期: zhaoshunxian@2017/01/24
*******************************************************************************/

#ifndef CVTE_EN_COMBINE_ANDROID_LINK
#error "CVTE_EN_COMBINE_ANDROID_LINK is not defined."
#elif (CVTE_EN_COMBINE_ANDROID_LINK != CVTE_DISABLE) && (CVTE_EN_COMBINE_ANDROID_LINK != CVTE_ENABLE)
#error "CVTE_EN_COMBINE_ANDROID_LINK must be either DISABLE or ENABLE."
#elif (CVTE_EN_COMBINE_ANDROID_LINK == CVTE_ENABLE) && ((CVTE_EN_ANDROID_AUTO != CVTE_ENABLE) || (CVTE_EN_WEBLINK != CVTE_ENABLE))
#error "CVTE_EN_ANDROID_AUTO and CVTE_EN_WEBLINK must be ENABLE when CVTE_EN_COMBINE_ANDROID_LINK is ENABLE."
#endif

//audio codec samplerate:
#ifndef CVTE_DEF_AUDIO_CODEC_SAMPLERATE_44100
#error "CVTE_DEF_AUDIO_CODEC_SAMPLERATE_44100 is not defined."
#endif

#endif
