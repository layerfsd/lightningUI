// Dib.h : 头文件

#pragma once
#include <afx.h>
#include <afxwin.h>
      typedef struct	_RGB24
      {
          BYTE    b;
          BYTE    g;
          BYTE    r;
      }RGB24;
      #define RGB16   WORD

      typedef struct	_RGB32
      {
          BYTE    b;
          BYTE    g;
          BYTE    r;
          BYTE    a;
      }RGB32;


class CDib
{

public:
	CDib(void);
	~CDib(void);
	LONG GetWidth();
	LONG GetHeight();
	LONG GetNewHeight();
	DWORD GetImgDataSize();
	DWORD GetNewImgDataSize();
	unsigned char *GetNewImgData(){return m_p_newDibBits;}
	unsigned char *GetImgData(){return m_pDibBits;}

	WORD GetBitCount();//
	WORD GetNewBitCount();//


	BOOL Load( LPCTSTR pszFilename);
	BOOL Read(CFile* pFile);
	BOOL Save( LPCTSTR pszFilename,int hightsign=1);
	BOOL Save_New( LPCTSTR pszFilename);
	BOOL Save_RawFile( LPCTSTR  pszFilename,unsigned char *pDibBits,	DWORD dwDibBitsSize );
	BOOL Save_HeaderFile( LPCTSTR  pszFilename,unsigned char *pDibBits,	DWORD dwDibBitsSize );
	BOOL Save_BMP24HeaderFile32( LPCTSTR  pszFilename,unsigned char *pDibBits,	DWORD dHight,	DWORD dWidth);
	BOOL Save_BMP32HeaderFile32( LPCTSTR  pszFilename,unsigned char *pDibBits,	DWORD dHight,	DWORD dWidth);
	BOOL Draw( CDC *pDC, int nX = 0, int nY = 0, int nWidth = -1, int nHeight = -1 );
	BOOL Draw( CDC *pDC,LPBITMAPINFOHEADER lpBMIH,unsigned char * pDibBits, int nX, int nY, int nWidth, int nHeight );
	BOOL SetPalette( CDC * pDC);
	void Empty();
	BOOL  IsEmpty();
	BOOL  IsNewEmpty();
	BOOL Convert24To16();	BOOL Convert24InverseTo16();
	BOOL Convert32To24();
	BOOL Convert24To32();   BOOL Convert24InverseTo32();
	BOOL Convert24To32Header( LPCTSTR  pszFilename);
	BOOL Convert32To16();	BOOL Convert32InverseTo16();

	BOOL ConvertDirection16();
	BOOL ConvertDirection32();
	BOOL ConvertDirection24();

private:
	CPalette m_Palette;
	unsigned char *m_pDib;//+ InfoHeader
	unsigned char *m_p_newDib;//+ InfoHeader  转换后的缓存
	unsigned char *m_pDibBits;//starting address of DIB bits
	unsigned char *m_p_newDibBits;//starting address of new DIB bits 转换后的缓存

	DWORD m_dwDibSize;//m_pDib 的大小
	DWORD m_dw_newDibSize;//m_p_newDib 的大小

	DWORD m_dwDibBitsSize;//m_pDibBits 的大小
	DWORD m_dw_newDibBitsSize;//m_p_newDibBits 的大小
	//RGBQUAD *m_pPalette;

	DWORD m_dwSizeImage; // of bits -- not BITMAPINFOHEADER or BITMAPFILEHEADER
	//int m_nColorTableEntries;//m_nPaletteEntries
	void ComputeMetrics();
	void ComputePaletteSize(int nBitCount);
public:
	//LPVOID m_lpvColorTable;
	HBITMAP m_hBitmap;
	LPBITMAPINFOHEADER m_lpBMIH; //  buffer containing the BITMAPINFOHEADER
	LPBITMAPINFOHEADER m_lp_new_BMIH; // 转换后的 buffer  BITMAPINFOHEADER
	BITMAPFILEHEADER m_BFH;
	BITMAPFILEHEADER m_new_BFH;
};
