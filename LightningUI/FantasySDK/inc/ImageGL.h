#pragma once
#include "Util.h"

#define PIX_5_6_5			1
#define PIX_5_5_5_1		2
#define PIX_8_8_8			3
#define PIX_8_8_8_8		4

class CImageGL
{
public:
	CImageGL(int cx,int cy,int depth=2);
	CImageGL();
public:
	~CImageGL(void);
	BOOL LoadFromFile(const wchar_t *filename);
	BOOL LoadFromFileScaled(const wchar_t *filename,int sx,int sy,int max_cx=-1,int max_cy=-1);
	void DeleteImage(void);
	void CopyImage(CImageGL *pImage);
	BOOL SaveAsBMP(const wchar_t *filename);

	BOOL SaveAsAlphaFile(const wchar_t *filename);
private:
	BOOL LoadBmp(const wchar_t *name, unsigned char *&buffer, SIZE *size, int *depth);

public:
	unsigned char *m_pImageData;
	CM_String m_strFilePath;
	int m_depth;
	int m_cx;
	int m_cy;
	UINT		m_iPixFormat;
};
