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
#ifndef ESCAPE_H
#define ESCAPE_H

#if __cplusplus
extern "C" {
#endif

#define	ESC_SUCCESS			0x00000001
#define	ESC_FAILED			0xFFFFFFFF
#define	ESC_NOT_SUPPORTED		0x00000000
#define ESC_SGX_POWERED_DOWN	        -2

#define OEMESC_BASEVALUE                    100000

/*****************************************************************************
    Should be defined in system files but isn't
*****************************************************************************/
#define DRVESC_COMPOSITORINFO			     6601

/* OEM Private Escape Call */
#define OEMESC_GET_PHYS_ROTATION		(OEMESC_BASEVALUE + 1)
#define OEMESC_GET_USER_VIRTBASE   		(OEMESC_BASEVALUE + 2)

#define OEMESC_REG_MEMORY_MAP   		(OEMESC_BASEVALUE + 3)
#define OEMESC_RESET_LCD        		(OEMESC_BASEVALUE + 4)

#define OEMESC_TURNON_LCD       		(OEMESC_BASEVALUE + 6)
#define OEMESC_TURNOFF_LCD      		(OEMESC_BASEVALUE + 7)

#define OEMESC_SET_TOP_LAYER    		(OEMESC_BASEVALUE + 9)
#define OEMESC_RESET_VPP        		(OEMESC_BASEVALUE + 0xa)
#define OEMESC_ENABLE_CURSOR    		(OEMESC_BASEVALUE + 0xb)
#define OEMESC_DISABLE_CURSOR   		(OEMESC_BASEVALUE + 0xc)
#define OEMESC_SET_PIXEL_CLOCK  		(OEMESC_BASEVALUE + 0xd)
#define OEMESC_GET_PIXEL_CLOCK  		(OEMESC_BASEVALUE + 0xe)

#define OEMESC_GET_LAYER_PARAMS			(OEMESC_BASEVALUE + 0xf)
#define OEMESC_SET_LAYER_PARAMS			(OEMESC_BASEVALUE + 0x10)
#define OEMESC_PRINT_REGISTER			(OEMESC_BASEVALUE + 0x11)
#define OEMESC_GET_TOP_LAYER    		(OEMESC_BASEVALUE + 0x12)

#define OEMESC_REL_USER_VIRTBASE   		(OEMESC_BASEVALUE + 0x13)
#define OEMESC_SET_LAYER_ORDER   		(OEMESC_BASEVALUE + 0x14)
#define OEMESC_GET_LAYER_ORDER   		(OEMESC_BASEVALUE + 0x15)

#if defined(SUPPORT_OEM_SURFACEBLT)
#define OEMESC_GET_DDRAW_SURFACE_INFO           (OEMESC_BASEVALUE + 0x16)
#define OEMESC_RELEASE_DDRAW_SURFACE_INFO       (OEMESC_BASEVALUE + 0x17)

#define CMD_RECORD_CALLER_PROCESS_ID            0x100
#define CMD_GET_DDRAW_SURFACE_INFO              0x101
#endif

#define DRVESC_GET_PIXMAPDETAILS		(OEMESC_BASEVALUE + 0x18)
#define DRVESC_GET_BITMAPDETAILS		(OEMESC_BASEVALUE + 0x19)
#define DRVESC_WIN_FRONT_BUFF_BLT		(OEMESC_BASEVALUE + 0x20)
#define DRVESC_UNMAP_DDGPE_MEMINFO		(OEMESC_BASEVALUE + 0x21)
#define DRVESC_MAP_DDGPE_MEMINFO		(OEMESC_BASEVALUE + 0x22)
#define DRVESC_GET_COMPOSITORINFO		(OEMESC_BASEVALUE + 0x23)


#define OEMESC_GET_VPP_PARAMS			(OEMESC_BASEVALUE + 0x24)
#define OEMESC_CAPTURE_STREAM_START		(OEMESC_BASEVALUE + 0x25)
#define OEMESC_CAPTURE_STREAM_STOP		(OEMESC_BASEVALUE + 0x26)
#define OEMESC_CAPTURE_STREAM_QUERY		(OEMESC_BASEVALUE + 0x27)
#define OEMESC_SET_GAMMA_RAMP           (OEMESC_BASEVALUE + 0x28)


#define OEMESC_CHARGER_ESCAPE           (OEMESC_BASEVALUE+0x1001)
#define OEMESC_PLAYER_ON_FPGA_ESCAPE    (OEMESC_BASEVALUE+0x1002)

typedef enum
{
    REG_DOMAIN_LCD_REG      = 0x10001,
    REG_DOMAIN_LCD_MEM      = 0x10002,
    REG_DOMAIN_VPP_REG      = 0x10003,
    REG_DOMAIN_PERFM_REG    = 0x10004
}ENUM_REG_DOMAIN;

typedef enum
{
    CHIP_INFO_ATLAS5        = 0x10001,
    CHIP_INFO_PRIMA2        = 0x10002,
    CHIP_INFO_ATLAS7        = 0x10003,
    CHIP_INFO_ATLAS6        = 0x10004
}ENUM_CHIP_INFO;

typedef enum
{
    REG_OP_MAP            = 0x00001,
    REG_OP_UNMAP          = 0x00002,
}ENUM_QUERY_OP;

typedef struct tag_MEM_MAP_INFO
{
    ENUM_REG_DOMAIN eRegDomain;
    DWORD dwVirtAddr;
    DWORD dwPhyAddr;
    DWORD dwBufLen;
}MEM_MAP_INFO, *PMEM_MAP_INFO;

#define MEM_MAP_LCD_REG_INDEX   0x0
#define MEM_MAP_LCD_MEM_INDEX   0x1
#define MEM_MAP_VPP_REG_INDEX   0x2
#define MEM_MAP_PERFM_REG_INDEX   0x3


#define MEM_MAP_LCD_REG_MASK   (0x1<<MEM_MAP_LCD_REG_INDEX)
#define MEM_MAP_LCD_MEM_MASK   (0x1<<MEM_MAP_LCD_MEM_INDEX)
#define MEM_MAP_VPP_REG_MASK   (0x1<<MEM_MAP_VPP_REG_INDEX)
#define MEM_MAP_PERFM_REG_MASK   (0x1<<MEM_MAP_PERFM_REG_INDEX)


typedef struct tag_LCD_MEM_MAP
{
    ENUM_CHIP_INFO eChipInfo;
    ENUM_QUERY_OP eOpCode;
    HANDLE hClientProcess;
    DWORD dwMapFlag;
    MEM_MAP_INFO asMapInfo[1]; /*User mode can extend this array*/
}LCD_MEM_MAP, *PLCD_MEM_MAP;

typedef struct _GET_USER_VIRT_DATA_
{
	UINT32 ui32PhysAddr;
	UINT32 ui32Size;    
} GET_USER_VIRT_DATA;

#if __cplusplus
}
#endif

#endif
