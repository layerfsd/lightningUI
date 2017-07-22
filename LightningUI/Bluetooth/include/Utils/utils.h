
#pragma once

#include "BT_Dbg.h"
#include "HeapStatistics.h"

//defautl chinese font
//#define DEFAULT_CHINESE_FONT TEXT("SimSun")	//original TEXT("MS Ming") has display problem, use TEXT("SimSun")  TEXT("NSimSun")
#define DEFAULT_CHINESE_FONT TEXT("Arial")		//use Arial to fix string contains both Chinese and English display issue

//ui version config
#define UI_VERSION_ORIGIN 0
#define UI_VERSION_UIDOG 1

#define UI_VERSION UI_VERSION_UIDOG

#if UI_VERSION == UI_VERSION_ORIGIN
#define LIST_ROW_COUNT 6//list
#define MARGIN_LEFT 1	//vertical scroll bar thumb margin
#define MARGIN_TOP 0

#elif UI_VERSION == UI_VERSION_UIDOG
#define LIST_ROW_COUNT 5
#define MARGIN_LEFT 5
#define MARGIN_TOP 7

#define FONT_HEIGHT_UNIT_PT //use point size
//#define MulDiv(a,b,c)       (((a)*(b))/(c))
#define MulDiv(a,b,c)       -(a+1)	//temp solution for the size above is too big


#endif