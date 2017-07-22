#pragma once

#include "Util.h"

#define MIN_FILTER			GL_LINEAR//GL_NEAREST//
#define MAG_FILTER			GL_LINEAR//GL_NEAREST//

#include "ImageGL.h"


class CTexture
{
public:
	CTexture(void);
	~CTexture(void);

	void	Init(int cx,int cy,GLint format=GL_RGB,GLint type=GL_UNSIGNED_SHORT_5_6_5);
	void	Release(void);

	unsigned int GetID(void) const;
	SIZE		GetImageSize(void) const;
	SIZE		GetTextureSize(void) const;

	GLfloat GetScaleX(void);
	GLfloat GetScaleY(void);
	GLint GetFilterParam(void);
	GLint GetImageDepth(void);

	BOOL InitializeFromFile( const wchar_t *filename,GLint param=GL_LINEAR,CImageGL *pImage=NULL);
	BOOL InitializeFromFileScaled( const wchar_t *filename,int cx,int cy,SIZE *pActualSize=NULL,GLint param=GL_LINEAR,CImageGL *pImage=NULL,BOOL bBlur=FALSE,DWORD dwBlurRadius=8);
	BOOL ReloadFromFile(const wchar_t *filename,GLint param=GL_LINEAR,CImageGL *pImage=NULL);
	BOOL ReloadFromImageBuff(const unsigned char *pBuff,DWORD dwSize,int offset=0,GLint param=GL_LINEAR);
	BOOL ReloadFromBuff(const unsigned char *pBuff,int offset,int cx,int cy,int depth=2,GLint param=GL_LINEAR,UINT iPixFormat=PIX_5_6_5,CImageGL *pImage=NULL);
	BOOL ReloadFromImage(CImageGL* pImage,GLint param=GL_LINEAR);
	BOOL ReloadFormScreen();
	BOOL ReloadFromFileScaled(const wchar_t *filename,int iMargin=0,SIZE *pActualSize=NULL,GLint param=GL_LINEAR,CImageGL *pImage=NULL,BOOL bBlur=FALSE,DWORD dwBlurRadius=8);
	BOOL InitializeFromRes(UINT idRes,LPCTSTR lpType,GLint param=GL_LINEAR,CImageGL *pImage=NULL);
	BOOL InitializeFromBuff(const unsigned char *pBuff,int cx,int cy,int depth,GLint param=GL_LINEAR,UINT iPixFormat=PIX_5_6_5,CImageGL *pImage=NULL);

private:
	BOOL LoadBmp(const wchar_t *name, unsigned char *&buffer, SIZE *size, int *depth,UINT *pix);
	BOOL LoadTiff(const wchar_t *name, unsigned char *&buffer, SIZE *size, int *depth);

	BOOL LoadBmpRes(UINT idRes, unsigned char *&buffer, SIZE *size, int *depth);
	BOOL LoadTiffRes(UINT idRes, unsigned char *&buffer, SIZE *size, int *depth);
	
private:
	unsigned int		m_ID;
	BOOL				m_bInitialized;
	SIZE					m_sizeImage;
	SIZE					m_sizeTexture;
	BOOL				m_bHasAlpha;
	GLfloat				m_fScaleX;
	GLfloat				m_fScaleY;
	GLint				m_iTexFilterPara;
	GLenum			m_iTexFormat;
	GLenum			m_iTexType;
	INT					m_iDepth;

};
