#pragma once
#include "Util.h"


class CFileData
{
public:
	CFileData();
	CFileData(LPCTSTR lpFileName,int iPathCut=0);
	~CFileData(void);

	BOOL InstallToPath(LPCTSTR lpPath);

	BOOL PackToFile(FILE *file);

	CM_String		m_strFileName;
	DWORD	m_dwDataSize;
	unsigned char 		*m_pdataBuffer;

private:
	BOOL   IsPathExists(LPCTSTR   lpszPath) ; 
	FILE	*m_pFile;
};
