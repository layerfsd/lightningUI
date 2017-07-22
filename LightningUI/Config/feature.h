#ifndef __FEATURE_H__
#define __FEATURE_H__

//CVTE_EN_XXX
#define CVTE_DISABLE					0
#define CVTE_ENABLE						1

//CVTE_MODEL_TYPE
//平台类型定义
#define CVTE_MODEL_TYPE_MIN				1
#define CVTE_MODEL_TYPE_FVW8010			1
#define CVTE_MODEL_TYPE_U701P_C			2
#define CVTE_MODEL_TYPE_U701P_DE		3
#define CVTE_MODEL_TYPE_U621P_C			4
#define CVTE_MODEL_TYPE_U621P_DE		5
#define CVTE_MODEL_TYPE_U702P			6
#define CVTE_MODEL_TYPE_U703PT			7
#define CVTE_MODEL_TYPE_U701P_E_ASP		8
#define CVTE_MODEL_TYPE_U621P_E_ASP		9
#define CVTE_MODEL_TYPE_U702P_ASP		10
#define CVTE_MODEL_TYPE_U703PT_ASP		11
#define CVTE_MODEL_TYPE_U705PM			12
#define CVTE_MODEL_TYPE_DS702P			13
#define CVTE_MODEL_TYPE_DS702P_ASP		14
#define CVTE_MODEL_TYPE_DS701P			15
#define CVTE_MODEL_TYPE_DS701P_ASP		16
#define CVTE_MODEL_TYPE_U703PT_DSP_AK7738	17
#define CVTE_MODEL_TYPE_MAX				17

//CVTE_DEF_DISPLAY_TYPE
//显示类型定义
#define CVTE_DISPLAY_TYPE_MIN			1
#define CVTE_DISPLAY_TYPE_U701P			1
#define CVTE_DISPLAY_TYPE_U621P			2
#define CVTE_DISPLAY_TYPE_U702P			3
#define CVTE_DISPLAY_TYPE_U703PT        4
#define CVTE_DISPLAY_TYPE_U705PM		5
#define CVTE_DISPLAY_TYPE_DS702P		6
#define CVTE_DISPLAY_TYPE_DS701P		7
#define CVTE_DISPLAY_TYPE_MAX			7

#define CVTE_SCREEN_PHYSICALWIDTH_U701P     153
#define CVTE_SCREEN_PHYSICALWIDTH_U621P     137
#define CVTE_SCREEN_PHYSICALWIDTH_U702P     150
#define CVTE_SCREEN_PHYSICALWIDTH_U703PT    137
#define CVTE_SCREEN_PHYSICALWIDTH_U705PM    154
#define CVTE_SCREEN_PHYSICALWIDTH_DS702P    154
#define CVTE_SCREEN_PHYSICALWIDTH_DS701P	154

#define CVTE_SCREEN_PHYSICALHEIGHT_U701P     80
#define CVTE_SCREEN_PHYSICALHEIGHT_U621P     79
#define CVTE_SCREEN_PHYSICALHEIGHT_U702P     79
#define CVTE_SCREEN_PHYSICALHEIGHT_U703PT    79
#define CVTE_SCREEN_PHYSICALHEIGHT_U705PM    86
#define CVTE_SCREEN_PHYSICALHEIGHT_DS702P	 86
#define CVTE_SCREEN_PHYSICALHEIGHT_DS701P	 86
//CVTE_DEF_UI_STYLE
//UI风格定义
#define CVTE_UI_STYLE_MIN				1
#define CVTE_UI_STYLE_LIGHTNINGUI		1
#define CVTE_UI_STYLE_FIATUI			2
#define CVTE_UI_STYLE_BOSSAUDIOUI		3
#define CVTE_UI_STYLE_ALDINET			4
#define CVTE_UI_STYLE_MAX				4

//CVTE_DEF_LED_TYPE
//LED类型定义
#define CVTE_LED_TYPE_MIN               1
#define CVTE_LED_TYPE_U701P             1
#define CVTE_LED_TYPE_U621P             2
#define CVTE_LED_TYPE_U702P             3
#define CVTE_LED_TYPE_U703PT            4
#define CVTE_LED_TYPE_FVW8010           5
#define CVTE_LED_TYPE_U705PM            6
#define CVTE_LED_TYPE_DS702P			7
#define CVTE_LED_TYPE_DS701P			8
#define CVTE_LED_TYPE_MAX               8

//CVTE_DEF_TOUCH_TYPE
//触摸类型定义
#define CVTE_TOUCH_TYPE_MIN				1
#define CVTE_TOUCH_TYPE_U701P			1
#define CVTE_TOUCH_TYPE_U621P			2
#define CVTE_TOUCH_TYPE_U702P			3
#define CVTE_TOUCH_TYPE_U703PT          4
#define CVTE_TOUCH_TYPE_U705PM          5
#define CVTE_TOUCH_TYPE_DS702P          6
#define CVTE_TOUCH_TYPE_DS701P			7
#define CVTE_TOUCH_TYPE_MAX				7

//CVTE_DEF_AUDIO_TYPE
#define CVTE_AUDIO_TYPE_MIN									1
#define CVTE_AUDIO_TYPE_ST7418								1
#define CVTE_AUDIO_TYPE_AIC3254								2
#define CVTE_AUDIO_TYPE_FM1388_AIC3254						3
#define CVTE_AUDIO_TYPE_FM1388_AIC3254_ST7418				4
#define CVTE_AUDIO_TYPE_AK7738								5
#define CVTE_AUDIO_TYPE_MAX									5

//CVTE_DEF_AUDIO_SUPPORT_FORMAT
//音频支持格式定义
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WAV		0x00000001UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP3		0x00000002UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_M4A		0x00000004UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WMA		0x00000008UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_APE		0x00000010UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLA		0x00000020UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLAC		0x00000040UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_OGG		0x00000080UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_AMR		0x00000100UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP2		0x00000200UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_M4R		0x00000400UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_RA		0x00000800UL
#define CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MASK		0x00000FFFUL

//CVTE_DEF_VIDEO_SUPPORT_FORMAT
//视频支持格式定义
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_AVI		0x00000001UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPG		0x00000002UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPEG		0x00000004UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_VOB		0x00000008UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TS		0x00000010UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TP		0x00000020UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_OGM		0x00000040UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MKV		0x00000080UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RM		0x00000100UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RMVB		0x00000200UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MP4		0x00000400UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_M4V		0x00000800UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MOV		0x00001000UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_3GP		0x00002000UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_ASF		0x00004000UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_WMV		0x00008000UL
#define CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MASK		0x0000FFFFUL

//CVTE_DEF_RADIO_REGION
//收音区域定义
#define CVTE_DEF_RADIO_REGION_MIN                   0
#define CVTE_DEF_RADIO_REGION_EUROPE                0         // 欧洲
#define CVTE_DEF_RADIO_REGION_USA                   1         // 美洲
#define CVTE_DEF_RADIO_REGION_EAST_ERNEUROPE        2         // 东欧
#define CVTE_DEF_RADIO_REGION_JAPAN                 3         // 日本
#define CVTE_DEF_RADIO_REGION_EUROPE_EASTERNEUROPE	4        // 欧洲、东欧	OIRT
#define CVTE_DEF_RADIO_REGION_KOREAN				5        // 韩国
#define CVTE_DEF_RADIO_REGION_TAIWANG				6        // 台湾
#define CVTE_DEF_RADIO_REGION_USA2					7		// USA2
#define CVTE_DEF_RADIO_REGION_SOUTHAMERICA			8        // 南美
#define CVTE_DEF_RADIO_REGION_ARABIA_AUSTRALIA		9       // 阿拉伯、澳大利亚
#define CVTE_DEF_RADIO_REGION_LATIN_AMERICA         10       // 拉丁美洲
#define CVTE_DEF_RADIO_REGION_MIDDLE_EAST           11      // 中东
#define CVTE_DEF_RADIO_REGION_AUST                  12      // 奥匈帝国
#define CVTE_DEF_RADIO_REGION_BRAZIL                13      // 巴西
//#define CVTE_DEF_RADIO_REGION_RUSSIA                14      // 苏联
#define CVTE_DEF_RADIO_REGION_EASTERNEUROPE_EUROPE	15		// 东欧、欧洲
#define CVTE_DEF_RADIO_REGION_MAX                   15

//CVTE_DEF_TIME_ZONE_TYPE
//时区定义
#define CVTE_DEF_TIME_ZONE_1200GMT_DATELINE_STANDARD_TIME         _T("Time Zones\\Dateline Standard Time")
#define CVTE_DEF_TIME_ZONE_1100GMT_SAMOA_STANDARD_TIME            _T("Time Zones\\Samoa Standard Time")
#define CVTE_DEF_TIME_ZONE_1100GMT_UTC11                           _T("Time Zones\\UTC-11")
#define CVTE_DEF_TIME_ZONE_1000GMT_HAWAIIAN_STANDARD_TIME        _T("Time Zones\\Hawaiian Standard Time")
#define CVTE_DEF_TIME_ZONE_900GMT_ALASKAN_STANDARD_TIME          _T("Time Zones\\Alaskan Standard Time")
#define CVTE_DEF_TIME_ZONE_800GMT_PACIFIC_STANDARD_TIME          _T("Time Zones\\Pacific Standard Time")
#define CVTE_DEF_TIME_ZONE_800GMT_PACIFIC_STANDARD_TIME_MEXICO  _T("Time Zones\\Pacific Standard Time (Mexico)")
#define CVTE_DEF_TIME_ZONE_700GMT_MOUNTAIN_STANDARD_TIME         _T("Time Zones\\Mountain Standard Time")
#define CVTE_DEF_TIME_ZONE_700GMT_US_MOUNTAIN_STANDARD_TIME      _T("Time Zones\\US Mountain Standard Time")
#define CVTE_DEF_TIME_ZONE_700GMT_MOUNTAIN_STANDARD_TIME_MEXICO  _T("Time Zones\\Mountain Standard Time (Mexico)")
#define CVTE_DEF_TIME_ZONE_600GMT_CENTRAL_STANDARD_TIME           _T("Time Zones\\Central Standard Time")
#define CVTE_DEF_TIME_ZONE_600GMT_CANADA_CENTRAL_STANDARD_TIME   _T("Time Zones\\Canada Central Standard Time")
#define CVTE_DEF_TIME_ZONE_600GMT_CENTRAL_AMERICA_STANDARD_TIME   _T("Time Zones\\Central America Standard Time")
#define CVTE_DEF_TIME_ZONE_600GMT_CENTRAL_STANDARD_TIME_MEXICO	_T("Time Zones\\Central Standard Time (Mexico)")
#define CVTE_DEF_TIME_ZONE_500GMT_EASTERN_STANDARD_TIME           _T("Time Zones\\Eastern Standard Time")
#define CVTE_DEF_TIME_ZONE_500GMT_US_EASTERN_STANDARD_TIME        _T("Time Zones\\US Eastern Standard Time")
#define CVTE_DEF_TIME_ZONE_500GMT_SA_PACIFIC_STANDARD_TIME        _T("Time Zones\\SA Pacific Standard Time")
#define CVTE_DEF_TIME_ZONE_430GMT_SA_PACIFIC_STANDARD_TIME        _T("Time Zones\\Venezuela Standard Time")
#define CVTE_DEF_TIME_ZONE_400GMT_ATLANTIC_STANDARD_TIME        _T("Time Zones\\Atlantic Standard Time")
#define CVTE_DEF_TIME_ZONE_400GMT_SA_WESTERN_STANDARD_TIME        _T("Time Zones\\SA Western Standard Time")
#define CVTE_DEF_TIME_ZONE_400GMT_PACIFIC_SA_STANDARD_TIME        _T("Time Zones\\Pacific SA Standard Time")
#define CVTE_DEF_TIME_ZONE_400GMT_CENTRAL_BRAZILIAN_STANDARD_TIME  _T("Time Zones\\Central Brazilian Standard Time")
#define CVTE_DEF_TIME_ZONE_400GMT_PARAGUAY_STANDARD_TIME        _T("Time Zones\\Paraguay Standard Time")
#define CVTE_DEF_TIME_ZONE_330GMT_NEWFOUNDLAND_STANDARD_TIME        _T("Time Zones\\Newfoundland Standard Time")
#define CVTE_DEF_TIME_ZONE_300GMT_ESOUTH_AMERICA_STANDARD_TIME        _T("Time Zones\\E. South America Standard Time")
#define CVTE_DEF_TIME_ZONE_300GMT_SA_EASTERN_STANDARD_TIME        _T("Time Zones\\SA Eastern Standard Time")
#define CVTE_DEF_TIME_ZONE_300GMT_GREENLAND_STANDARD_TIME        _T("Time Zones\\Greenland Standard Time")
#define CVTE_DEF_TIME_ZONE_300GMT_ARGENTINA_STANDARD_TIME        _T("Time Zones\\Argentina Standard Time")
#define CVTE_DEF_TIME_ZONE_300GMT_MONTEVIDEO_STANDARD_TIME        _T("Time Zones\\Montevideo Standard Time")
#define CVTE_DEF_TIME_ZONE_300GMT_BAHIA_STANDARD_TIME        _T("Time Zones\\Bahia Standard Time")
#define CVTE_DEF_TIME_ZONE_200GMT_MID_ATLANTIC_STANDARD_TIME        _T("Time Zones\\Mid-Atlantic Standard Time")
#define CVTE_DEF_TIME_ZONE_200GMT_UTC2                          _T("Time Zones\\UTC-2")
#define CVTE_DEF_TIME_ZONE_100GMT_AZORES_STANDARD_TIME        _T("Time Zones\\Azores Standard Time")
#define CVTE_DEF_TIME_ZONE_100GMT_CAPE_VERDE_STANDARD_TIME        _T("Time Zones\\Cape Verde Standard Time")
#define CVTE_DEF_TIME_ZONE_000GMT_GMT_STANDARD_TIME        _T("Time Zones\\GMT Standard Time")
#define CVTE_DEF_TIME_ZONE_000GMT_GREENWICH_STANDARD_TIME        _T("Time Zones\\Greenwich Standard Time")
#define CVTE_DEF_TIME_ZONE_000GMT_MOROCCO_STANDARD_TIME        _T("Time Zones\\Morocco Standard Time")
#define CVTE_DEF_TIME_ZONE_000GMT_UTC                           _T("Time Zones\\UTC")
#define CVTE_DEF_TIME_ZONE_GMT100_CENTRAL_EUROPE_STANDARD_TIME  _T("Time Zones\\Central Europe Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_CENTRAL_EUROPEAN_STANDARD_TIME  _T("Time Zones\\Central European Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_ROMANCE_STANDARD_TIME         _T("Time Zones\\Romance Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_WEUROPE_STANDARD_TIME         _T("Time Zones\\W. Europe Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_WCENTRAL_AFRICA_STANDARD_TIME         _T("Time Zones\\W. Central Africa Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_NAMIBIA_STANDARD_TIME         _T("Time Zones\\Namibia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_EEUROPE_STANDARD_TIME         _T("Time Zones\\E. Europe Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_EGYPT_STANDARD_TIME         _T("Time Zones\\Egypt Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_FLE_STANDARD_TIME         _T("Time Zones\\FLE Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_GTB_STANDARD_TIME         _T("Time Zones\\GTB Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_SOUTH_AFRICA_STANDARD_TIME         _T("Time Zones\\South Africa Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_ISRAEL_STANDARD_TIME         _T("Time Zones\\Israel Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_JORDAN_STANDARD_TIME         _T("Time Zones\\Jordan Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_MIDDLE_EAST_STANDARD_TIME         _T("Time Zones\\Middle East Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_SYRIA_STANDARD_TIME         _T("Time Zones\\Syria Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT200_TRUKEY_STANDARD_TIME         _T("Time Zones\\Turkey Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT300_RUSSIAN_STANDARD_TIME         _T("Time Zones\\Russian Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT300_ARAB_STANDARD_TIME         _T("Time Zones\\Arab Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT300_E_AFRICA_STANDARD_TIME         _T("Time Zones\\E. Africa Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT300_ARABIC_STANDARD_TIME         _T("Time Zones\\Arabic Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT300_KALININGRAD_STANDARD_TIME         _T("Time Zones\\Kaliningrad Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT330_IRAN_STANDARD_TIME         _T("Time Zones\\Iran Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT400_GEORGIAN_STANDARD_TIME         _T("Time Zones\\Georgian Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT400_ARABIAN_STANDARD_TIME         _T("Time Zones\\Arabian Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT400_CAUCASUS_STANDARD_TIME         _T("Time Zones\\Caucasus Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT400_AZERBAIJAN_STANDARD_TIME         _T("Time Zones\\Azerbaijan Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT400_MAURITIUS_STANDARD_TIME         _T("Time Zones\\Mauritius Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT430_AFGHANISTAN_STANDARD_TIME         _T("Time Zones\\Afghanistan Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT500_EKATERINBURG_STANDARD_TIME         _T("Time Zones\\Ekaterinburg Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT500_WEST_ASIA_STANDARD_TIME         _T("Time Zones\\West Asia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT500_PAKISTAN_STANDARD_TIME         _T("Time Zones\\Pakistan Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT530_INDIA_STANDARD_TIME        _T("Time Zones\\India Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT530_INDIA_STANDARD_TIME        _T("Time Zones\\India Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT530_SRI_LANKA_STANDARD_TIME        _T("Time Zones\\Sri Lanka Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT545_NEPAL_STANDARD_TIME        _T("Time Zones\\Nepal Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT600_CENTRAL_ASIA_STANDARD_TIME        _T("Time Zones\\Central Asia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT600_N_CENTRAL_ASIA_STANDARD_TIME        _T("Time Zones\\N. Central Asia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT600_BANGLADESH_STANDARD_TIME        _T("Time Zones\\Bangladesh Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT630_MYANMAR_STANDARD_TIME        _T("Time Zones\\Myanmar Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT700_SE_ASIA_STANDARD_TIME        _T("Time Zones\\SE Asia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT700_NORTH_ASIA_STANDARD_TIME        _T("Time Zones\\North Asia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT800_CHINA_STANDARD_TIME        _T("Time Zones\\China Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT800_TAIPEI_STANDARD_TIME        _T("Time Zones\\Taipei Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT800_W_AUSTRALIA_STANDARD_TIME        _T("Time Zones\\W. Australia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT800_NORTH_ASIA_EAST_STANDARD_TIME        _T("Time Zones\\North Asia East Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT800_SINGAPORE_STANDARD_TIME        _T("Time Zones\\Singapore Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT800_ULAANBAATAR_STANDARD_TIME        _T("Time Zones\\Ulaanbaatar Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT900_KOREA_STANDARD_TIME        _T("Time Zones\\Korea Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT900_TOKYO_STANDARD_TIME        _T("Time Zones\\Tokyo Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT900_YAKUTSK_STANDARD_TIME        _T("Time Zones\\Yakutsk Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT930_AUS_CENTRAL_STANDARD_TIME        _T("Time Zones\\AUS Central Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT930_CEN_AUSTRALIA_STANDARD_TIME        _T("Time Zones\\Cen. Australia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_AUS_EASTERN_STANDARD_TIME        _T("Time Zones\\AUS Eastern Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_E_AUSTRALIA_STANDARD_TIME        _T("Time Zones\\E. Australia Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_TASMANIA_STANDARD_TIME        _T("Time Zones\\Tasmania Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_VLADIVOSTOK_STANDARD_TIME        _T("Time Zones\\Vladivostok Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT100_WEST_PACIFIC_STANDARD_TIME        _T("Time Zones\\West Pacific Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT110_CENTRAL_PACIFIC_STANDARD_TIME        _T("Time Zones\\Central Pacific Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT110_MAGADAN_STANDARD_TIME        _T("Time Zones\\Magadan Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT120_FIJI_STANDARD_TIME        _T("Time Zones\\Fiji Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT120_NEW_ZEALAND_STANDARD_TIME        _T("Time Zones\\New Zealand Standard Time")
#define CVTE_DEF_TIME_ZONE_GMT120_UTC12                     _T("Time Zones\\UTC+12")
#define CVTE_DEF_TIME_ZONE_GMT130_TONGA_STANDARD_TIME                    _T("Time Zones\\Tonga Standard Time")

//CVTE_REG_RDS_AF_LEVEL_DEFAULT
//收音停台灵敏度定义
#define CVTE_DEF_RDS_AF_LEVEL_MIN                   50
#define CVTE_DEF_RDS_AF_LEVEL_MAX                   90

//CVTE_DEF_LANGUAGE_TYPE
//语言定义
#define CVTE_DEF_LANGUAGE_MIN                       0
#define CVTE_DEF_LANGUAGE_ENGLISH                   0
#define CVTE_DEF_LANGUAGE_CHINESE_TRA               2
#define CVTE_DEF_LANGUAGE_CHINESE_SIM               3
#define CVTE_DEF_LANGUAGE_KOREAN                    4
#define CVTE_DEF_LANGUAGE_GERMAN                    5
#define CVTE_DEF_LANGUAGE_SPANISH                   6
#define CVTE_DEF_LANGUAGE_PORTUGUESE                7
#define CVTE_DEF_LANGUAGE_ITALIAN                   8
#define CVTE_DEF_LANGUAGE_FRENCH                    9
#define CVTE_DEF_LANGUAGE_JAPANESS                  10
#define CVTE_DEF_LANGUAGE_RUSSIAN                   11
#define CVTE_DEF_LANGUAGE_PERSIAN					12
#define CVTE_DEF_LANGUAGE_GREEK						13
#define CVTE_DEF_LANGUAGE_MAX                       13

//支持的语言定义
#define CVTE_LANGUAGE_SUPPORT_ENGLISH			0x00000001UL
#define CVTE_LANGUAGE_SUPPORT_CHINESE_TRA		0x00000002UL
#define CVTE_LANGUAGE_SUPPORT_CHINESE_SIM		0x00000004UL
#define CVTE_LANGUAGE_SUPPORT_KOREAN			0x00000008UL
#define CVTE_LANGUAGE_SUPPORT_GERMAN			0x00000010UL
#define CVTE_LANGUAGE_SUPPORT_SPANISH			0x00000020UL
#define CVTE_LANGUAGE_SUPPORT_PORTUGESE			0x00000040UL
#define CVTE_LANGUAGE_SUPPORT_ITALIAN			0x00000080UL
#define CVTE_LANGUAGE_SUPPORT_FRENCH			0x00000100UL
#define CVTE_LANGUAGE_SUPPORT_JAPANESS			0x00000200UL
#define CVTE_LANGUAGE_SUPPORT_RUSSIAN			0x00000400UL
#define CVTE_LANGUAGE_SUPPORT_PERSIAN			0x00000800UL
#define CVTE_LANGUAGE_SUPPORT_GREEK				0x00001000UL
#define CVTE_LANGUAGE_SUPPORT_MASK				0x00000FFFUL

//支持的收音区域选择
#define CVTE_RADIO_REGION_SUPPORT_EUROPE						0x00000001UL         // 欧洲
#define CVTE_RADIO_REGION_SUPPORT_USA								0x00000002UL         // 美洲
#define CVTE_RADIO_REGION_SUPPORT_EAST_ERNEUROPE        0x00000004UL         // 东欧
#define CVTE_RADIO_REGION_SUPPORT_JAPAN							0x00000008UL         // 日本
#define CVTE_RADIO_REGION_SUPPORT_EUROPE_EASTERNEUROPE	0x00000010UL        // 欧洲、东欧	OIRT
#define CVTE_RADIO_REGION_SUPPORT_KOREAN					0x00000020UL        // 韩国
#define CVTE_RADIO_REGION_SUPPORT_TAIWANG					0x00000040UL        // 台湾
#define CVTE_RADIO_REGION_SUPPORT_USA2							0x00000080UL		// USA2
#define CVTE_RADIO_REGION_SUPPORT_SOUTHAMERICA			0x00000100UL        // 南美
#define CVTE_RADIO_REGION_SUPPORT_ARABIA_AUSTRALIA		0x00000200UL       // 阿拉伯、澳大利亚
#define CVTE_RADIO_REGION_SUPPORT_LATIN_AMERICA         0x00000400UL       // 拉丁美洲
#define CVTE_RADIO_REGION_SUPPORT_MIDDLE_EAST           0x00000800UL      // 中东
#define CVTE_RADIO_REGION_SUPPORT_AUST							0x00001000UL      // 奥匈帝国
#define CVTE_RADIO_REGION_SUPPORT_BRAZIL									0x00002000UL      // 巴西
#define CVTE_RADIO_REGION_SUPPORT_EASTERNEUROPE_EUROPE	0x00004000UL		// 东欧、欧洲
#define CVTE_RADIO_REGION_SUPPORT_MASK	0x00007FFFUL		

//CVTE_DEF_TIME_MODE_TYPE
//时间模式定义
#define CVTE_DEF_TIME_MODE_MIN                      1
#define CVTE_DEF_TIME_MODE_12H                      1
#define CVTE_DEF_TIME_MODE_24H                      2
#define CVTE_DEF_TIME_MODE_MAX                      2

//CVTE_DEF_BACKLIGHT
//背光定义
#define CVTE_DEF_BACKLIGHT_MIN                      0
#define CVTE_DEF_BACKLIGHT_MAX                      100

//CVTE_DEF_DRIVE_TYPE
//左右驾驶定义
#define CVTE_DEF_DRIVE_TYPE_MIN                     0
#define CVTE_DEF_DRIVE_TYPE_LEFT                     0
#define CVTE_DEF_DRIVE_TYPE_RIGHT                    1
#define CVTE_DEF_DRIVE_TYPE_MAX                      1

//CVTE_DEF_VIDEO_COLOR_TYPE
//CVBS视频制式定义
#define CVTE_DEF_VIDEO_COLOR_TYPE_MIN                     0
#define CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_MJ                 0
#define CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_443                1
#define CVTE_DEF_VIDEO_COLOR_TYPE_PAL_M                   2
#define CVTE_DEF_VIDEO_COLOR_TYPE_PAL_60                  3
#define CVTE_DEF_VIDEO_COLOR_TYPE_PAL_BGHID               4
#define CVTE_DEF_VIDEO_COLOR_TYPE_SECAM                   5
#define CVTE_DEF_VIDEO_COLOR_TYPE_PAL_COM_N               6
#define CVTE_DEF_VIDEO_COLOR_TYPE_SECAM_525               7
#define CVTE_DEF_VIDEO_COLOR_TYPE_MAX                      7

//字体定义
#define CVTE_DEF_FONT_TYPE_TAHOMA						0	//	FONT_Tahoma
#define CVTE_DEF_FONT_TYPE_COURIER_NEW					1	//	FONT_Courier_New
#define CVTE_DEF_FONT_TYPE_ARIAL						2	//	FONT_Arial
#define CVTE_DEF_FONT_TYPE_TIMES_NEW_ROMAN				3	//	FONT_Times_New_Roman
#define CVTE_DEF_FONT_TYPE_SYMBOL						4	//	FONT_Symbol
#define CVTE_DEF_FONT_TYPE_MYRIAD_PRO					5	//	FONT_Myriad_Pro
#define CVTE_DEF_FONT_TYPE_SEGOE_UI						6	//	FONT_Segoe_UI
#define CVTE_DEF_FONT_TYPE_HELVETICA					7	//	FONT_Helvetica

//TV Widget类型定义
#define CVTE_DEF_TV_WIDGET_TYPE_DTV						0	//	DTV
#define CVTE_DEF_TV_WIDGET_TYPE_DVB						1	//	DVB
#define CVTE_DEF_TV_WIDGET_TYPE_ISDB					2	//	ISDB
#define CVTE_DEF_TV_WIDGET_TYPE_AUX_VIDEO 				3	//	AUX VIDEO
#define CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE			4	//	ANDROID WAZE

//Camera Widget类型定义
#define CVTE_DEF_CAMERA_WIDGET_TYPE_DEFAULT				0	//	CVBS camera default
#define CVTE_DEF_CAMERA_WIDGET_TYPE_360					1	//	360 camera
#endif
