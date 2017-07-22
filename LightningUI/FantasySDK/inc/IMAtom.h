#pragma once
#include "Util.h"
#include "CM_List.h"

class CIMAtom
{
public:
	CIMAtom(void);
	~CIMAtom(void);
	CIMAtom(wchar_t *pLable,wchar_t*pCharList,UINT index);

public:
	UINT m_idIndex;
	CM_String m_strLabel;
	CM_String m_strCharList;
};
class CIMImagin
{
public:
	CIMImagin(void);
	~CIMImagin(void);
	CIMImagin(wchar_t cap,wchar_t*pCharList,UINT index);
	void ResetMenu();
	BOOL MoveToIndex(int index);
	int GetImaginCounts();
	CM_String *GetCurrent();
	CM_String *GetNext();
	CM_String *GetPrevious();

public:
	UINT m_idIndex;
	wchar_t m_cCap;
	CM_List<CM_String> m_listImagin;
};