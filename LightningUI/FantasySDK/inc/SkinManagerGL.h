#pragma once
#include "Texture.h"
#include "CM_List.h"
#include "GUI.h"
#include "ProConfig.h"
#include "Texture_id.h"


typedef struct tagSKINHEADER
{
	char tag[4];
	WCHAR name[32];
	int iCount;

}SKINHEADER,*LPSKINHEADER;

class CSkinManagerGL
{
public:
	CSkinManagerGL(CGUI *pGUI,CProConfig *pConfig);
	virtual ~CSkinManagerGL(void);

	BOOL LoadFromFile(LPCTSTR lpFileName);

	void Reload(WCHAR *lpSknFile);

	BOOL AddTexture(UINT ID,int cx,int cy,GLint param=GL_LINEAR,GLint type=GL_UNSIGNED_SHORT_5_6_5);
	BOOL AddTextureFromFile(UINT ID,const wchar_t *filename,int cx,int cy,BOOL bPreserve=FALSE,GLint param=GL_LINEAR,BOOL bBlur=FALSE,DWORD dwBlurRadius=8 );
	BOOL AddTextureFromRes(UINT ID,UINT idRes,LPCTSTR lpType=_T("TIFF"),BOOL bPreserve=FALSE,GLint param=GL_LINEAR);
	BOOL AddTextureFromBuff(UINT ID,const unsigned char *pBuff,int cx,int cy,int depth,BOOL bPreserve=FALSE,GLint param=GL_LINEAR,UINT iPixFormat=PIX_5_6_5);

	BOOL ReloadTextureFromFile(UINT ID,const wchar_t *filename,int iMargin=0,SIZE *pActualSize=NULL,GLint param=GL_LINEAR,CImageGL *pImage=NULL,BOOL bBlur=FALSE,DWORD dwBlurRadius=8);
	BOOL ReloadTextureFromRes(UINT ID,UINT idRes,LPCTSTR lpType=_T("TIFF"),BOOL bPreserve=FALSE,GLint param=GL_LINEAR);
	BOOL ReloadTextureFromBuff(UINT ID,const unsigned char *pBuff,int offset,int cx,int cy,int depth,BOOL bPreserve=FALSE,GLint param=GL_LINEAR,UINT iPixFormat=PIX_5_6_5);

	BOOL ReloadFromImageBuff(UINT ID,const unsigned char *pBuff,DWORD dwSize,int offset=0,GLint param=GL_LINEAR);
	BOOL ReloadTextureFromSkin(UINT ID,LPCTSTR lpStrFileName);

	CTexture * GetTexture(UINT ID);
	virtual void RestoreDefaultConfig();
	CImageGL* GetImage(UINT ID);

	BOOL GetImageFromSkin(UINT ID,LPCTSTR lpStrFileName,CImageGL*pImage);

	int GetBakImageCount();
	int GetTextureCount();
 	CProConfig * GetConfig();
	CGUI *GetGUI();
	const wchar_t *GetName();

	DWORD testGetAllTextureSize();
	BOOL IsSkinFile(LPCTSTR lpFileName);
	BOOL GetSkinName(LPCTSTR lpFileName,CM_String *pname);

protected:
	CGUI							*m_pGUI;

 	CProConfig					*m_pConfig;

	CM_String						m_strSkinName;
	CM_String						m_strSkinFileName;

	CM_List<CTexture>		m_listTexture;
	CM_List<CImageGL>	m_listBakImage;

	unsigned char				*m_pSkinData;
	SKINHEADER				m_shSkinHeader;
};
