#include "RadioCommon.h"
#include "../../resource.h"
#include "../Common/ui_common.h"

WCHAR* ProgramTypeToString( ProgramType pt )
{
		/*
		PT_Undefined = 0,
		PT_News,
		PT_Current_Affairs,
		PT_Information,
		PT_Sport,
		PT_Education,//5
		PT_Drama,
		PT_Culture,
		PT_Science,
		PT_Varied,
		PT_Pop_Music,//10
		PT_Rock_Music,
		PT_Easy_Music,
		PT_Light_classical,
		PT_Serious_classical,
		PT_Other_Music,//15
		PT_Weather,
		PT_Finance,
		PT_Children,
		PT_Social_Addairs,
		PT_Religion,
		PT_Phone_In,
		PT_Travel,
		PT_Leisure,
		PT_Jazz_Music,
		PT_Country_Music,
		PT_National_Music,
		PT_Oldies_Music,
		PT_Folk_Music,
		PT_Documentary,
		PT_Alarm_Test,
		PT_Alarm,
		*/

	switch( pt )
	{
	case		PT_Undefined: return _T("None");
	case		PT_News: return _T("News");
	case		PT_Current_Affairs: return _T("Current Affairs");
	case		PT_Information: return _T("Information");
	case		PT_Sport: return _T("Sport");
	case		PT_Education: return _T("Education");
	case		PT_Drama: return _T("Drama");
	case		PT_Culture: return _T("Culture");
	case		PT_Science: return _T("Science");
	case		PT_Varied: return _T("Varied");
	case		PT_Pop_Music: return _T("Pop Music");
	case		PT_Rock_Music: return _T("Rock Music");
	case		PT_Easy_Music: return _T("Easy Music");
	case		PT_Light_classical: return _T("Light Classical");
	case		PT_Serious_classical: return _T("Serious Classical");
	case		PT_Other_Music: return _T("Other Music");
	case		PT_Weather: return _T("Weather");
	case		PT_Finance: return _T("Finance");
	case		PT_Children: return _T("Children");
	case		PT_Social_Affairs: return _T("Social Affairs");
	case		PT_Religion: return _T("Religion");
	case		PT_Phone_In: return _T("Phone In");
	case		PT_Travel: return _T("Travel");
	case		PT_Leisure: return _T("Leisure");
	case		PT_Jazz_Music: return _T("Jazz Music");
	case		PT_Country_Music: return _T("Country Music");
	case		PT_National_Music: return _T("National Music");
	case		PT_Oldies_Music: return _T("Oldies Music");
	case		PT_Folk_Music: return _T("Folk Music");
	case		PT_Documentary: return _T("Documentary");
	case		PT_Alarm_Test: return _T("Alarm Test");
	case		PT_Alarm: return _T("Alarm");
	case		PT_AllType: return _T("All Type");

	default:    return _T("Unknown PTY");
	}
}
#if 0
const wchar_t* RegionToStringEn(RadioRegionId idRegion)
{
	switch (idRegion)
	{
	case RADIO_REGION_Europe:
		return GetResourceString(IDS_SETTING_EUROPE,LANGUAGE_ENGLISH);
		break;
	case RADIO_REGION_USA:
		return GetResourceString(IDS_SETTING_USA,LANGUAGE_ENGLISH);
		break;
	case RADIO_REGION_EasternEurope:
		return GetResourceString(IDS_SETTING_EASTERN_EUROPE,LANGUAGE_ENGLISH);
		break;
	case RADIO_REGION_Japan:
		return GetResourceString(IDS_SETTING_JAPAN,LANGUAGE_ENGLISH);
		break;
    case RADIO_REGION_Korean:
		return GetResourceString(IDS_SETTING_KOREA,LANGUAGE_KOREAN);
		break;
    case RADIO_REGION_SouthAmerica:
		return GetResourceString(IDS_SETTING_SOUTHAMERICA,LANGUAGE_ENGLISH);
		break;
	default:
		return _T("UnKnown");
		break;
	}
}
const wchar_t* RegionToString(RadioRegionId idRegion)
{
	switch (idRegion)
	{
	case RADIO_REGION_Europe:
		return GetResourceString(IDS_SETTING_EUROPE);
		break;
	case RADIO_REGION_USA:
		return GetResourceString(IDS_SETTING_USA);
		break;
	case RADIO_REGION_EasternEurope:
		return GetResourceString(IDS_SETTING_EASTERN_EUROPE);
		break;
	case RADIO_REGION_Japan:
		return GetResourceString(IDS_SETTING_JAPAN);
		break;
 	case RADIO_REGION_Korean:
 		return GetResourceString(IDS_SETTING_KOREA);
 		break;
 	case RADIO_REGION_SouthAmerica:
 		return GetResourceString(IDS_SETTING_SOUTHAMERICA);
 		break;
	default:
		return _T("UnKnown");
		break;
	}
}
RadioRegionId StringToRegion(const wchar_t* strRegionName)
{
	if(!strRegionName)
		return RADIO_REGION_Invalid;
	else if (lstrcmpi(strRegionName, GetResourceString(IDS_SETTING_EUROPE)) == 0)
		return RADIO_REGION_Europe;
	else if (lstrcmpi(strRegionName, GetResourceString(IDS_SETTING_USA)) == 0)
		return RADIO_REGION_USA;
	else if (lstrcmpi(strRegionName, GetResourceString(IDS_SETTING_EASTERN_EUROPE)) == 0)
		return RADIO_REGION_EasternEurope;
	else if (lstrcmpi(strRegionName, GetResourceString(IDS_SETTING_JAPAN)) == 0)
		return RADIO_REGION_Japan;
    else if (lstrcmpi(strRegionName, GetResourceString(IDS_SETTING_KOREA)) == 0)
        return RADIO_REGION_Korean;
    else if (lstrcmpi(strRegionName, GetResourceString(IDS_SETTING_SOUTHAMERICA)) == 0)
        return RADIO_REGION_SouthAmerica;
	else
		return RADIO_REGION_Invalid;
}
#endif