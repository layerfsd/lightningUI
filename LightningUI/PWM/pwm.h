/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company PrimaII BSP/CSP

 Copyright (c) 2010 - 2011  Cambridge Silicon Radio Limited, a CSR plc group
 company.

 All rights reserved.

 This Software is protected by United Kingdom copyright laws and international
 treaties.  You may not reverse engineer, decompile or disassemble this
 Software.

 WARNING:
 This Software contains Cambridge Silicon Radio Limited's confidential and
 proprietary information. UNAUTHORIZED COPYING, USE, MODIFICATION,
 DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL, REPACKAGING, REASSEMBLING
 OR DISCLOSURE OF THE WHOLE OR ANY PART OF THE SOFTWARE IS PROHIBITED AND MAY
 RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this Software without
 Cambridge Silicon Radio Limited's express written permission.   Use of any
 portion of the contents of this Software is subject to and restricted by your
 signed written agreement with Cambridge Silicon Radio Limited.
******************************************************************************/

#ifndef __PWM_DEV_H__
#define __PWM_DEV_H__

#define DBG_PWM       0
#if DBG_PWM
#define DBG_PWM_FUNC        1
#define DBG_PWM_INFO        1
#define DBG_PWM_WARN        1
#define DBG_PWM_ERR         1
#define DBG_PWM_DUMP        0
#else
#define DBG_PWM_FUNC        0
#define DBG_PWM_INFO        0
#define DBG_PWM_WARN        0
#define DBG_PWM_ERR         1
#define DBG_PWM_DUMP        0
#endif // DBG_PWM



// define the start bit of bypass in register PWM_PRECLKSRC
#define BYPASS_START_BIT	21

#define XIN_FREQUENCY 		26000000
#define PLL1_FREQUENCY		26000000
#define PLL2_FREQUENCY		26000000
#define XINW_FREQUENCY		32768
#define PLL3_FREQUENCY		30000000

// define the operation flag
typedef enum
{
    PWM_PRIMAII_MODE = 0, PWM_PEGASUS_MODE
}PWM_FLAG;

// define the 7 ports of PWM
typedef enum
{
    PWM0 = 0, PWM1, PWM2, PWM3, CKO_0, CKO_1, I2S
}PWM_PORT;

// define the source clk of which PWM uses, the SourceUnspecified means the
// driver will automaticlly choose an appropriate source for that request
typedef enum
{
    SOURCE_UNSPECIFIED = -1, SOURCE_XIN = 0, SOURCE_PLL1, SOURCE_PLL2, SOURCE_XINW, SOURCE_PLL3
}PWM_SOURCE_CLK;

// use this type to define output frequency of PWM
typedef DWORD PWM_OUTPUT_FREQ;

// use this type to define duty cycle of PWM output waveform
typedef UCHAR PWM_DUTY_CYCLE;

// use this type to define jump step
typedef DWORD PWM_JUMP_STEP;

// use this type to define hold cycles between step jumps
typedef DWORD PWM_JUMP_HOLD;

// use this type to define QoS
typedef DWORD PWM_QOS;

// use this type to define BL_LEVEL_MAX, refer LCD register LCD_BLS_CTRL2
typedef DWORD PWM_MAX_LEVEL;

// use this type to define BL_LEVEL_MIN, refer LCD register LCD_BLS_CTRL2
typedef DWORD PWM_MIN_LEVEL;


// define the default high:low ratio to 1:1, normally we use this macro
#define DEFAULT_DUTY_CYCLE	50

// define the bypass notification, we set *pdwPrecision to 101 as bypass
#define OUTPUT_BYPASS	101

#define BLS_WIDTH		0x140	// 320
#define BLS_HIGHT		0x1E0   // 480

typedef struct
{
    // the configure flag
    PWM_FLAG PwmFlag;
    // the output port num
    PWM_PORT PwmPort;
    // the source of wave out
    PWM_SOURCE_CLK PwmSourceClk;
    // the output frequency
    PWM_OUTPUT_FREQ PwmOutputFreq;
    // the fraction of high level pulse in one period
    PWM_DUTY_CYCLE PwmDutyCycle;
}ENABLE_PORT_INFO, *PENABLE_PORT_INFO;

typedef struct
{
    // the output port num
    PWM_PORT PwmPort;
    // the fraction of high level pulse in one period
    PWM_DUTY_CYCLE PwmDutyCycle;
}DISABLE_PORT_INFO, *PDISABLE_PORT_INFO;

typedef struct
{
    // the configure flag
    PWM_FLAG PwmFlag;
    // the output port num
    PWM_PORT PwmPort;
    // the source of wave out
    PWM_SOURCE_CLK PwmSourceClk;
    // the output frequency
    PWM_OUTPUT_FREQ PwmOutputFreq;
    // the fraction of high level pulse in one period
    PWM_DUTY_CYCLE PwmDutyCycle;
    // jump times
    PWM_JUMP_STEP PwmJumpStep;
    // step hold mili-seconds
    PWM_JUMP_HOLD PwmJumpHold;
}STEPJUMP_INFO, *PSTEPJUMP_INFO;


typedef struct
{
    // the source of wave out
    PWM_SOURCE_CLK PwmSourceClk;
    // the output frequency
    PWM_OUTPUT_FREQ PwmOutputFreq;
    // the max BLS level
    PWM_MAX_LEVEL PwmMaxLevel;
    // the min BLS level
    PWM_MIN_LEVEL PwmMinLevel;
    // the fraction of high level pulse in one period, to fill the look table, it needs 16 duty cycle
    PWM_DUTY_CYCLE PwmDutyCycle[16];
    // QoS
    PWM_QOS PwmQoS;
}BACKLIGHT_SCALING_INFO, *PBACKLIGHT_SCALING_INFO;

typedef struct
{
    // the max BLS level
    PWM_MAX_LEVEL PwmMaxLevel;
    // the min BLS level
    PWM_MIN_LEVEL PwmMinLevel;
    // QoS
    PWM_QOS PwmQoS;
}TEST_INFO, *PTEST_INFO;


// define the IOCTRL code passed to PWM_IOControl
#define IOCTL_PWM_ENABLE_PORT \
	CTL_CODE(FILE_DEVICE_CONTROLLER, 1000, METHOD_BUFFERED, FILE_ANY_ACCESS)

// define the IOCTRL code passed to PWM_IOControl
#define IOCTL_PWM_DISABLE_PORT \
	CTL_CODE(FILE_DEVICE_CONTROLLER, 1001, METHOD_BUFFERED, FILE_ANY_ACCESS)

// define the IOCTRL code passed to PWM_IOControl
#define IOCTL_PWM_STEPJUMP \
	CTL_CODE(FILE_DEVICE_CONTROLLER, 1002, METHOD_BUFFERED, FILE_ANY_ACCESS)

// define the IOCTRL code passed to PWM_IOControl
#define IOCTL_PWM_ENABLE_BKLSCALING \
	CTL_CODE(FILE_DEVICE_CONTROLLER, 1003, METHOD_BUFFERED, FILE_ANY_ACCESS)

// define the IOCTRL code passed to PWM_IOControl
#define IOCTL_PWM_DISABLE_BKLSCALING \
	CTL_CODE(FILE_DEVICE_CONTROLLER, 1004, METHOD_BUFFERED, FILE_ANY_ACCESS)

// define the IOCTRL code passed to PWM_IOControl
#define IOCTL_PWM_RESET \
	CTL_CODE(FILE_DEVICE_CONTROLLER, 1005, METHOD_BUFFERED, FILE_ANY_ACCESS)

// define the IOCTRL code passed to PWM_IOControl
#define IOCTL_PWM_TEST \
	CTL_CODE(FILE_DEVICE_CONTROLLER, 1006, METHOD_BUFFERED, FILE_ANY_ACCESS)







// -----------------------------------------------------------------------------
//
//  @func   BOOL | PWM_Configure | Interface for other drivers' access
//
//  @parm   PPwmInfo | Info | PwmInfo structure passed to customize the driver
//
//  @parm   PDWORD | pdwPrecision | return the precision of output clock
//
//  @rdesc  Returns TRUE for success, FALSE for failure.
//
// -----------------------------------------------------------------------------
static __inline BOOL PWM_Configure(DWORD dwIoctlCode, LPVOID pInBuf, DWORD BufLen, PDWORD pdwPrecision)
{
    HANDLE hPwmObject;
    BOOL bRet = TRUE;

    hPwmObject = CreateFile(TEXT("PWM1:"), GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL, OPEN_EXISTING, 0, 0);

    if (INVALID_HANDLE_VALUE == hPwmObject)
    {
        RETAILMSG(1, (TEXT("Failed CreateFile for PWM -- error = 0x%x\r\n"), GetLastError()));
        return FALSE;
    }

    if (!DeviceIoControl(hPwmObject, dwIoctlCode, (LPVOID)pInBuf, BufLen, pdwPrecision,
                         sizeof(DWORD), NULL, NULL))
    {
        RETAILMSG(1, (TEXT("Failed DeviceIoControl for PWM -- error = 0x%x\r\n"), GetLastError()));
        bRet = FALSE;
    }

    CloseHandle(hPwmObject);

    return bRet;
}


#endif //__PWM_DEV_H__
