#pragma once
#include <FantasyLib.h>

#define MAX_DATA_CNT		600000
#define MAX_AREA_CNT		1000
#define MAX_TYPE_CNT		1000

class CMobileData 
{
public:
	CMobileData();
	~CMobileData(void);

public:
	BOOL Query(const wchar_t* pNumber);
	const wchar_t *GetArea();
	const wchar_t *GetType();

private:
	BOOL LoadRes();

	BOOL LoadAreaTable();
	BOOL LoadTypeTable();

private:
	DWORD	*m_pdata;
	DWORD	m_dwRes;
	CM_String m_strArea[MAX_AREA_CNT];
	CM_String m_strType[MAX_TYPE_CNT];
	INT			m_iAreaListCnt;
	INT			m_iTypeListCnt;
	INT			m_iDataListCnt;
};