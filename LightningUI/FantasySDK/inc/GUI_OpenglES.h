#pragma once
#include "gui.h"
#include "LabelGL.h"
#include "SliderGL.h"

typedef struct tagINSTALLHEADER
{
	char tag[4];
	DWORD dwFileCount;
	DWORD dwTotalSize;

}INSTALLHEADER,*LPINSTALLHEADER;

class CGUI_OpenglES :
	public CGUI
{
public:
	CGUI_OpenglES(void);
	virtual ~CGUI_OpenglES(void);
public:
	virtual 	BOOL Initialize(HWND window,MODULEENTRY entry,int iPriority);

protected:
	BOOL InitializeEGL(EGLNativeWindowType window);
	virtual BOOL InitializeEGLWorld(void)=0;
	void SwapBuffers(void);

	void ShotScreen(LPCTSTR lpFileName);

	virtual void PrepareForFirstTime()=0;


private:
	EGLSurface		m_surface;
	EGLContext		m_context;
	EGLDisplay		m_display;


};
