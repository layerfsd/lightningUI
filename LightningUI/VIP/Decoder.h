/***************************************************************************
 *                                                                         *
 *    CVTE AUTO Co. Ltd. Platform Development Kit                          *
 *                                                                         *
 *    Copyright (c) 2015 by CVTE AUTO Co. Ltd.                             *
 *    All rights reserved.                                                 *
 *                                                                         *
 *    This Software is protected by People's Republic of China copyright   *
 *    laws and international treaties.  You may not reverse engineer,      *
 *    decompile or disassemble this Software.                              *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains Shanghai CVTE AUTO . Ltd.'s                   *
 *    confidential and proprietary information. UNAUTHORIZED COPYING,      *
 *    USE, DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE *
 *    IS PROHIBITED AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not  *
 *    copy this Software without CVTE AUTO Co. Ltd.'s                      *
 *    express written permission.   Use of any portion of the contents of  *
 *    this Software is subject to and restricted by your written agreement *
 *    with CVTE AUTO Co. Ltd.                                              *
 *                                                                         *
 ***************************************************************************/
/****************************************************************************
*   Author :
*       Wu haoheng 2015
*
*	Module Name:
*
*		Decoder.h
*
*	Abstract:
*
*
*
****************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum _DECODER_INPUT_CHANNEL_
	{
		DECODER_INPUT_CHANNEL_CMMB = 0,
		DECODER_INPUT_CHANNEL_CVBS,
		DECODER_INPUT_CHANNEL_DVD,
		DECODER_INPUT_CHANNEL_NA
	}DECODERINPUTCHANNEL;

	typedef enum _DECODER_STANDARD_INPUT_
	{
		DECODER_STANDARD_INPUT_NTSC_MJ = 0,
		DECODER_STANDARD_INPUT_NTSC_443,
		DECODER_STANDARD_INPUT_PAL_M,
		DECODER_STANDARD_INPUT_PAL_60,
		DECODER_STANDARD_INPUT_PAL_BDGHI,
		DECODER_STANDARD_INPUT_SECAM,
		DECODER_STANDARD_INPUT_PAL_CN,
		DECODER_STANDARD_INPUT_SECAM_525,
		DECODER_STANDARD_INPUT_NA,
	}DECODERSTANDARDINPUT;

/*
	typedef enum _DECODER_COLOR_TYPE_
	{
		DECODER_IMAGE_CONTRAST_Y = 0,
		DECODER_IMAGE_CONTRAST_CB,
		DECODER_IMAGE_CONTRAST_CR,
		DECODER_IMAGE_BRIGHTNESS,
		DECODER_IMAGE_SHARPNESS,
		DECODER_DECODER_BRIGHTNESS,
		DECODER_DECODER_CONTRAST,
		DECODER_DECODER_SHARPNESS,
		DECODER_DECODER_SATURATION_U,
		DECODER_DECODER_SATURATION_V,
		DECODER_DECODER_HUE
	}DECODERCOLORTYPE;
*/

#ifdef __cplusplus
}
#endif