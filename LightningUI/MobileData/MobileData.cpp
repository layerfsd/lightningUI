#include ".\inc\MobileData.h"
#include ".\inc\Resource.h"

CMobileData::CMobileData()
{
	m_pdata = NULL;
	m_dwRes = 0;

	m_iAreaListCnt=0;
	m_iTypeListCnt=0;

	m_iDataListCnt=0;
}

CMobileData::~CMobileData(void)
{
	if(m_pdata)
	{
		delete[] m_pdata;
	}
}

BOOL CMobileData::LoadRes()
{
	RETAILMSG(1,(_T("CMobileData::LoadRes()++%d\r\n"),GetTickCount()));

	//load data:
	if(!m_pdata)
	{
		m_pdata = new DWORD[MAX_DATA_CNT];
	}

	if(!m_pdata)
	{
		return FALSE;
	}

	FILE						*file;
	int						fileSize = 0;

	if ( 0!=_wfopen_s(&file, _T("\\windows\\mobile_data.dat"),L"rb") )
	{
		RETAILMSG(1,(_T("ERROR: mobile_data FAILED!\r\n")));
		return FALSE;
	}
	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	fread(m_pdata, min(fileSize,MAX_DATA_CNT*4), 1, file);

	fclose(file);


	//load Area table:
	LoadAreaTable();


	//load Type table:
	LoadTypeTable();



	RETAILMSG(1,(_T("CMobileData::LoadRes()--%d\r\n"),GetTickCount()));

	return TRUE;
}
BOOL CMobileData::LoadAreaTable()
{
	wchar_t *table=NULL;
	int table_len=0;

	HRSRC hrc = FindResource(
		NULL,
		MAKEINTRESOURCE(IDR_TXT_MOBILE_AREA_TABLE),
		_T("TXT"));
	if(hrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(
		NULL,
		hrc);
	if(hGlobal == NULL)
		return FALSE;

	DWORD dwSize = ::SizeofResource(
		NULL,
		hrc);

	LPVOID pData = ::LockResource(hGlobal);

	table_len=dwSize/sizeof(wchar_t);
	table = new wchar_t[table_len];
	memcpy(table,(wchar_t*)pData, dwSize);

	//process:
	m_iAreaListCnt=0;
	WCHAR str_temp[MAX_PATH];
	WCHAR *cursor=table+1;//skip 0xfeff
	WCHAR *p_start=table+1;//skip 0xfeff

	while (cursor)
	{
		p_start=cursor;
		cursor = wcsstr(cursor,_T("\r\n"));

		if(cursor!=p_start)
		{
			if(cursor)
			{
				//get string;
				memcpy(str_temp,p_start,(cursor-p_start)*2);
				str_temp[cursor-p_start]=0;

				m_strArea[m_iAreaListCnt]=str_temp;

				m_iAreaListCnt++;

				cursor+=2;

				if(cursor-table>=table_len || m_iAreaListCnt>=MAX_AREA_CNT)
				{
					break;
				}
			}
		}
		else
		{
			cursor+=2;

			if(cursor-table>=table_len || m_iAreaListCnt>=MAX_AREA_CNT)
			{
				break;
			}
		}
	}

	delete[] table;

	return TRUE;
}
BOOL CMobileData::LoadTypeTable()
{
	wchar_t *table=NULL;
	int table_len=0;

	HRSRC hrc = FindResource(
		NULL,
		MAKEINTRESOURCE(IDR_TXT_MOBILE_TYPE_TABLE),
		_T("TXT"));
	if(hrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(
		NULL,
		hrc);
	if(hGlobal == NULL)
		return FALSE;

	DWORD dwSize = ::SizeofResource(
		NULL,
		hrc);

	LPVOID pData = ::LockResource(hGlobal);

	table_len=dwSize/sizeof(wchar_t);
	table = new wchar_t[table_len];
	memcpy(table,(wchar_t*)pData, dwSize);

	//process:
	m_iTypeListCnt=0;
	WCHAR str_temp[MAX_PATH];
	WCHAR *cursor=table+1;//skip 0xfeff
	WCHAR *p_start=table+1;//skip 0xfeff

	while (cursor)
	{
		p_start=cursor;
		cursor = wcsstr(cursor,_T("\r\n"));

		if(cursor!=p_start)
		{
			if(cursor)
			{
				//get string;
				memcpy(str_temp,p_start,(cursor-p_start)*2);
				str_temp[cursor-p_start]=0;

				m_strType[m_iTypeListCnt]=str_temp;

				m_iTypeListCnt++;

				cursor+=2;

				if(cursor-table>=table_len || m_iTypeListCnt>=MAX_TYPE_CNT)
				{
					break;
				}
			}
		}
		else
		{
			cursor+=2;

			if(cursor-table>=table_len || m_iTypeListCnt>=MAX_TYPE_CNT)
			{
				break;
			}
		}
	}

	delete[] table;

	return TRUE;
}
BOOL CMobileData::Query(const wchar_t* pNumber)
{
	m_dwRes = 0;

	if(!m_pdata)
	{
		LoadRes();
	}

	if(m_pdata)
	{
		if(pNumber)
		{
			wchar_t str_temp[16];
			const wchar_t *cursor=pNumber;
			int len=0;

			if(pNumber[0]=='0')
			{
				cursor+=1;
				len=min(7,wcslen(cursor));
				memcpy(str_temp,cursor,len*2);
				str_temp[len]=0;
			}
			else if(pNumber[0]=='+' && pNumber[1]=='8' && pNumber[2]=='6')
			{
				cursor+=3;
				len=min(7,wcslen(cursor));
				memcpy(str_temp,cursor,len*2);
				str_temp[len]=0;
			}
			else
			{
				len=min(7,wcslen(cursor));
				memcpy(str_temp,cursor,len*2);
				str_temp[len]=0;
			}


			int index=_wtoi(str_temp)-1300000;

			if(index>=0 && index<MAX_DATA_CNT)
			{
				m_dwRes = m_pdata[index];
			}
		}
	}

	RETAILMSG(1,(_T("CMobileData::Query:%s, result: 0x%x\r\n"),pNumber,m_dwRes));

	return m_dwRes!=0;
}
const wchar_t *CMobileData::GetArea()
{
	int res=HIWORD(m_dwRes);

	if(res>0 && res<m_iAreaListCnt)
	{
		return m_strArea[res].String();
	}
	else
	{
		return NULL;
	}

}
const wchar_t *CMobileData::GetType()
{
	int res=LOWORD(m_dwRes);

	if(res>0 && res<m_iTypeListCnt)
	{
		return m_strType[res].String();
	}
	else
	{
		return NULL;
	}

}