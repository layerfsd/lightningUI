#pragma once
#include "../stdafx.h"
#include "Platform_macro.h"
#include "CVTE_IoCtl.h"
#include "../Config/customer.h"

#define LED_DEVICE_NAME             L"LED1:"

#define DEBUG_LED					0

#define LED_NUMBER					12

#if (CVTE_DEF_TOUCH_TYPE == CVTE_TOUCH_TYPE_U621P)
#define LED_ANI_FRAME_TIME			10
#else
#define LED_ANI_FRAME_TIME			15
#endif
#define LED_BREATH_FRAME_TIME       2000
#define LED_WAIT_FRAME_TIME			3000
#define LED_SHADE_FRAME_TIME		50
#define LED_FLASH_FRAME_TIME        500
#define LED_IDLE_FRAME_TIME			INFINITE //-1

#define LED_SILDER_ENABLE_MIN_STEP	20

//
// Led RGB Value
//
typedef struct _LEDRGB_
{
    BYTE Red;
    BYTE Green;
    BYTE Blue;
}LEDRGB, *PLEDRGB;


//
// Led HSB Value
//
typedef struct _LEDHSB_
{
    float Hue;
    float Saturation;
    float Brightness;
}LEDHSB, *PLEDHSB;
//
// Led Work Mode Packet
//
typedef struct _LEDMODE_
{
    BYTE    Mode;
    BYTE    InstanceID;
}LEDMODE, *PLEDMODE;

//
// Led Individual Operation Packet
//
typedef struct _LEDNOPERATION_
{
    LEDRGB    Color;          // LED Individual Color.
    BYTE        Current;        // LED Individual Current.
    BYTE        Ledn;           // LED num;
    BYTE        InstanceID;     // LED Driver Instance num
}LEDOPERATION, *PLEDOPERATION;

//
// Led Group Operation Packet
//
typedef struct _LEDGROUPOPERATION_
{
    DWORD       dwLedControlFlag;           // LED Control Flag
    LEDRGB    Color[LED_NUMBER];          // LED Color.
    BYTE        Current[LED_NUMBER];        // LED Current. Only support 0/1/2/3
    BYTE        InstanceID;                 // LED Instance num
}LEDGROUPOPERATION, *PLEDGROUPOPERATION;

typedef enum _LED_ANIMATION_TYPE_
{
    LED_ANIMATION_IDLE = 0,
    LED_ANIMATION_POWER_BREATH,
    LED_ANIMATION_SHADE,
    LED_ANIMATION_POWER_ON,
    LED_ANIMATION_POWER_OFF,
    LED_ANIMATION_POWER_IDLE,
    LED_ANIMATION_FLASH,
};

typedef enum _LEDSTATE_
{
	LED_POWEROFF = 0,
    LED_POWERON,
    LED_POWERIDLE,
    LED_BREATH,
    LED_BRIGHTNESS_ON,
    LED_BRIGHTNESS_OFF,
    LED_MUTE,
    LED_UNMUTE,
    LED_SIRI_DISABLE,
    LED_SIRI_ENABLE,
//	    LED_ANIMATION_SHADE_STOP,
//	    LED_ANIMATION_SHADE_START
}LEDSTATE;


typedef enum _RGBCOLOR_
{
    RED = 0,
    GREEN,
    BLUE
};

typedef enum _LEDOPERATIONMODE_
{
    LED_MODE_SHUTDOWN = 0,
    LED_MODE_NORMAL_MAX_CURRENT,
    LED_MODE_NORMAL_HALF_CURRENT,
    LED_MODE_NORMAL_THIRD_CURRENT,
    LED_MODE_NORMAL_MIN_CURRENT
}LEDOPERATIONMODE;

__inline BYTE GetNearBYTE(float f)
{
	return (f-(float)(BYTE(f))>0.5f)?BYTE(f)+1:BYTE(f);
}


//
__inline void ArrayInsertSort(BYTE a[],BYTE count)
{
    int i,j,temp;
    for(i=1;i<count;i++)
    {
       temp=a[i];
       j=i-1;
       while(a[j]>temp && j>=0)
       {
         a[j+1]=a[j];
          j--;
       }
       if(j!=(i-1))
         a[j+1]=temp;
     }
}

__inline void RGB2HSB(PLEDRGB pledrgb, PLEDHSB pledhsb)
{
	BYTE r,g,b;
    LEDHSB ledhsb;

    memset(&ledhsb, 0, sizeof(ledhsb));
	r = pledrgb->Red;
	g = pledrgb->Green;
	b = pledrgb->Blue;

    ASSERT( 0 <= r && r <= 255);
    ASSERT( 0 <= g && g <= 255);
    ASSERT( 0 <= b && b <= 255);

	//RETAILMSG(1, (TEXT("APP:MSG:RGB2HSB: RGB[%d,%d,%d] \r\n"),r, g, b));

    BYTE rgb[]  = { r, g, b };
    ArrayInsertSort(rgb,3);

    BYTE min = rgb[0];
	BYTE mid = rgb[1];
    BYTE max = rgb[2];

	//RETAILMSG(1, (TEXT("APP:MSG:RGB2HSB: min[%d] mid[%d] max[%d] \r\n"),min, mid, max));

    ledhsb.Brightness = (max / 255.0f);
    if(ledhsb.Brightness<0)
        ledhsb.Brightness = 0.0f;
    else if(ledhsb.Brightness>1.0f)
        ledhsb.Brightness = 1.0f;

    ledhsb.Saturation = (max == 0 ? 0 :(1.0f - ((float)min / (float)max)));
    if(ledhsb.Saturation<0)
        ledhsb.Saturation = 0.0f;
    else if(ledhsb.Saturation>1.0f)
        ledhsb.Saturation = 1.0f;

    ledhsb.Hue = 0;

    if (max == r && g >= b)
	{
        if(max == min)
            ledhsb.Hue = 0;
        else
            ledhsb.Hue = ((g - b) * 60.0f / (max - min) + 0);
    }
	else if (max == r && g < b)
	{
        ledhsb.Hue = ((g - b) * 60.0f / (max - min) + 360);
    }
	else if (max == g)
	{
        ledhsb.Hue = ((b - r) * 60.0f / (max - min) + 120);
    }
	else if (max == b)
	{
        ledhsb.Hue = ((r - g) * 60.0f / (max - min) + 240);
    }

    if(ledhsb.Hue<0)
        ledhsb.Hue = 0.0f;
    else if(ledhsb.Hue>360.0f)
        ledhsb.Hue = 360.0f;

    memcpy(pledhsb, &ledhsb, sizeof(ledhsb));
	//printf("APP:MSG:RGB2HSB: HSB[%.2f,%.2f,%.2f] \r\n",pledhsb->Hue, pledhsb->Saturation, pledhsb->Brightness);

}

__inline void HSB2RGB(PLEDHSB pledhsb, PLEDRGB pledrgb)
{
	float h,s,v;
	h = pledhsb->Hue;
	s = pledhsb->Saturation;
	v = pledhsb->Brightness;

    ASSERT( (h>=0.0f) && (h<=360.0f));
    ASSERT( (s>=0.0f) && (s<=1.0f));
    ASSERT( (v>=0.0f) && (v<=1.0f));

	//printf("APP:MSG:HSB2RGB: h[%g] s[%g] b[%g] \r\n",h, s, v);

    float r = 0, g = 0, b = 0;
    int i = (int) ((int)(h/60.0f)%6);
    float f = (h / 60) - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    //printf("APP:MSG:HSB2RGB: i[%g] f[%g] p[%g] q[%g] t[%g]\r\n",i, f, p, g, t);

    switch (i)
    {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    case 5:
        r = v;
        g = p;
        b = q;
        break;
    default:
        break;
    }

	//printf("MSG:HSB2RGB: rgb[%.2f,%.2f,%.2f] \r\n",r, g, b);

	pledrgb->Red = (BYTE) (GetNearBYTE(r * 255.0f));
	pledrgb->Green = (BYTE) (GetNearBYTE(g * 255.0f));
	pledrgb->Blue = (BYTE) (GetNearBYTE(b * 255.0f));

	//printf("MSG:HSB2RGB: RGB[%d,%d,%d] \r\n",pledrgb->Red,pledrgb->Green,pledrgb->Blue);
}



//function to export to APP
__inline HANDLE LEDApi_Open()
{
	HANDLE hDevice;
    hDevice = CreateFile(LED_DEVICE_NAME,
                         GENERIC_READ|GENERIC_WRITE,          // desired access
                         FILE_SHARE_READ|FILE_SHARE_WRITE,    // sharing mode
                         NULL,                                // security attributes (ignored)
                         OPEN_EXISTING,                       // creation disposition
                         FILE_FLAG_RANDOM_ACCESS,             // flags/attributes
                         NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        RETAILMSG(1, (TEXT("ERR: Open LED device error!\r\n")));
    }
    return hDevice;
}

__inline void  LEDApi_Close(HANDLE hDevice)
{
    if (NULL != hDevice)
    {
        CloseHandle(hDevice);
        hDevice = NULL;
    }
}

__inline BOOL  LEDApi_ChangeWorkMode(HANDLE hDevice, LEDMODE * pLedMode)
{
    return DeviceIoControl(hDevice, LED_IOCTL_CHANGE_WORK_MODE, pLedMode, sizeof(LEDMODE), NULL, 0, NULL, NULL);
}

__inline BOOL  LEDApi_SetGroupPWM(HANDLE hDevice, LEDGROUPOPERATION * pLedGroupOperation)
{
    return DeviceIoControl(hDevice, LED_IOCTL_SET_LEDGROUP_PWM, pLedGroupOperation, sizeof(LEDGROUPOPERATION), NULL, 0, NULL, NULL);
}

__inline BOOL  LEDApi_SetPWM(HANDLE hDevice, LEDOPERATION * pLedOperation)
{
    return DeviceIoControl(hDevice, LED_IOCTL_SET_LED_PWM, pLedOperation, sizeof(LEDOPERATION), NULL, 0, NULL, NULL);
}
