#ifndef __eglfslext_h_
#define __eglfslext_h_

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  * Copyright 2005-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *   */

#ifndef EGL_FSL_create_image 
#define EGL_FSL_create_image  1
#define EGL_NEW_IMAGE_FSL                               0x3190
#define EGL_IMAGE_FORMAT_FSL                            0x3191
#define EGL_FORMAT_RGBA_8888_FSL                        0x3192
#define EGL_FORMAT_RGB_565_FSL                          0x3193
#define EGL_FORMAT_BGRA_8888_FSL                        0x3194
#define EGL_FORMAT_YUV_YV12_FSL                         0x3195
#define EGL_FORMAT_YUV_NV21_FSL                         0x3196
#define EGL_FORMAT_YUV_UYVY_FSL                         0x3197
#endif //EGL_FSL_create_image

#ifndef EGL_QUERY_IMAGE_FSL
#define EGL_QUERY_IMAGE_FSL 1
#define EGL_TARGET_TYPE_FSL         0x31A0
#define EGL_CLIENTBUFFER_TYPE_FSL   0x31A1
#define EGL_YUV_INFO_FSL            0x31A2
#ifdef _WIN32_WCE
typedef struct eglYUV_INFO
{
    union
    {
        UINT32 uYUVPlane0Addr;
        UINT32 uYUVPlaneYAddr;
    };
    union
    {
        UINT32 uYUVPlane1Addr;
        UINT32 uYUVPlaneVAddr;
        UINT32 uYUVPlaneUVAddr;
    };
    union
    {
        UINT32 uYUVPlane2Addr;
        UINT32 uYUVPlaneUAddr;
    };
    union
    {
        EGLint uYUVPlane0Pitch;
        EGLint uYUVPlaneYPitch;        
    };
    union
    {    
        EGLint uYUVPlane1Pitch;
        EGLint uYUVPlaneVPitch;
        EGLint uYUVPlaneUVPitch;
    };
    union
    {
        EGLint uYUVPlane2Pitch;
        EGLint uYUVPlaneUPitch;
    };
} eglYUVInfo;
#endif

#ifdef _LINUX
struct EGLImageInfoFSL
{
    void *mem_virt[3]; //virtual addresses for Y/U/V planes
    int mem_phy[3]; //physical addresses for Y/U/V planes
    int stride; //image stride
};
#endif

EGLAPI EGLBoolean EGLAPIENTRY eglQueryImageFSL (EGLDisplay dpy, EGLImageKHR image, EGLint attribute, EGLint* value);
#endif // EGL_QUERY_IMAGE_FSL

// 0x3190 - 0x31AF is allocated to Freescale

#ifdef __cplusplus
} //extern C
#endif
#endif // __eglfslext_h_

