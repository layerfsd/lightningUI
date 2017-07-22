#include "MediaCommon.h"


BOOL GetID3v1Info(const wchar_t* filename, ID3V1INFO *info)
{
	BOOL bRet = FALSE;

	FILE *file=NULL;

	if(!filename || !info)
		return FALSE;

	memset(info, 0, sizeof(ID3V1INFO));

	if ( 0!=_wfopen_s(&file, filename,_T("rb")) )
	{
		return FALSE;
	}

	//seek
	if (fseek(file, -128, SEEK_END) != 0)  
	{  
		fclose(file);  
		return FALSE;  
	} 

	//read 128 bytes
	ID3V1DATA id3V1Data;  
	if (fread(&id3V1Data, sizeof(id3V1Data), 1, file) != 1)  
	{  
		fclose(file);  
		return FALSE;  
	}  
	
	fclose(file);  
	
	bRet = (id3V1Data.Header[0]=='T' && id3V1Data.Header[1]=='A' && id3V1Data.Header[2]=='G');  
	if (bRet)  
	{  
		*info = id3V1Data.V1Info;
	}  

	return bRet;
}