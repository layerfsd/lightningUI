
#pragma once

template <class T>
class CM_ListCell
{
public:
	T			*m_body;
	CM_ListCell<T>	*m_next;
	CM_ListCell<T>	*m_prev;
	INT		m_iIndex;
	INT		m_iID;
	DWORD m_dwAccessRef;

	CM_ListCell()
	{
		m_body = NULL;
		m_next = m_prev = NULL;
		m_iIndex=0;
		m_iID=0;
		m_dwAccessRef=0;
	}
	~CM_ListCell()
	{
	}
};

template <class T>
class CM_List
{
public:

	CM_List();
	~CM_List();

	void	Head(void);
	void	Tail(void);
	void	Clear(void);
	BOOL	IsHead(void);
	BOOL	IsTail(void);

	BOOL	Next(void);
	BOOL	Previous(void);
	
	void	Add(T*,INT id=0);
	void	Insert(T*,INT iIndexBefore,INT id=0);
	T*		Get(void);
	T*		GetHead(void);
	T*		GetTail(void);
	int GetCount(void);
	int GetIndex(void);
	int GetID(void);
	int SetID(INT id);
	DWORD GetAccessRef();
	BOOL	End(void);
	BOOL	Last(void);
	BOOL Search(T* element);
	BOOL SearchID(UINT id);
	T* GetItemByID(UINT id);
	int GetIndexByID(UINT id);
	T* GetItemByIndex(INT index);
	INT GetItemIndex(T* element);
	BOOL MoveToIndex(INT index);
	BOOL Swap(INT index1,INT index2);
	BOOL Delete(INT index);
	BOOL DeleteTail();
	BOOL SearchLeastAccessItem();
	void	AddAccessRef();

	CM_ListCell<T> *GetHeadCell();
	CM_ListCell<T> *GetTailCell();

	void	RestoreCurrent(CM_ListCell<T>*);
	CM_ListCell<T> *BackupCurrent(void);

private:
	int				m_cellCount;
	DWORD		m_dwAccessRef;
	CM_ListCell<T>	*m_head;
	CM_ListCell<T>	*m_tail;
	CM_ListCell<T>	*m_current;


};


template <class T>
CM_List<T>::CM_List()
{
	m_head = m_tail = m_current = NULL;
	m_cellCount = 0;
	m_dwAccessRef=0;
}

template <class T>
CM_List<T>::~CM_List()
{
	CM_ListCell<T>	*tmp;
	if (m_cellCount)
	{
		m_current = m_head;
		while (m_current)
		{
			tmp = m_current;
			m_current = m_current->m_next;
			delete tmp;
		}
	}
	m_cellCount = 0;
}
template <class T>
void CM_List<T>::Clear()
{
	CM_ListCell<T>	*tmp;
	if (m_cellCount)
	{
		m_current = m_head;
		while (m_current)
		{
			tmp = m_current;
			m_current = m_current->m_next;
			delete tmp;
		}
	}
	m_head = m_tail = m_current = NULL;
	m_cellCount = 0;
}

template <class T>
BOOL CM_List<T>::Next(void)
{
	if(m_current)
		m_current = m_current->m_next;
	return (m_current != NULL);
}
template <class T>
BOOL CM_List<T>::Search(T* element)
{
	Head();
	while (!End())
	{
		if(Get() == element)
			return TRUE;
		Next();
	}
	return FALSE;
}
template <class T>
INT CM_List<T>::GetItemIndex(T* element)
{
	CM_ListCell<T>* pcur=m_head;
	while (pcur)
	{
		if(pcur->m_body == element)
		{
			return pcur->m_iIndex;
		}
		pcur=pcur->m_next;
	}
	//search nothing,restore:
	return -1;
}
template <class T>
int CM_List<T>::GetIndexByID(UINT id)
{
	int res=-1;
	CM_ListCell<T>* pcur=m_head;
	while (pcur)
	{
		if(pcur->m_iID == id)
		{
			return pcur->m_iIndex;
		}
		pcur=pcur->m_next;
	}
	//search nothing,restore:
	return res;
}
template <class T>
T* CM_List<T>::GetItemByID(UINT id)
{
	CM_ListCell<T>* pcur=m_head;
	while (pcur)
	{
		if(pcur->m_iID == id)
		{
			return pcur->m_body;
		}
		pcur=pcur->m_next;
	}
	//search nothing,restore:
	return NULL;
}
template <class T>
T* CM_List<T>::GetItemByIndex(INT index)
{
	CM_ListCell<T>* pcur=m_head;
	while (pcur)
	{
		if(pcur->m_iIndex == index)
		{
			return pcur->m_body;
		}
		pcur=pcur->m_next;
	}
	//search nothing,restore:
	return NULL;
}
template <class T>
BOOL CM_List<T>::SearchID(UINT id)
{
	CM_ListCell<T>* pcur=m_current;
	Head();
	while (!End())
	{
		if(GetID() == id)
		{
			return TRUE;
		}
		Next();
	}
	//search nothing,restore:
	m_current=pcur;
	return FALSE;
}
template <class T>
void CM_List<T>::AddAccessRef()
{
	// 	if(m_current)
	// 		m_current->m_dwAccessRef++;
	// 	m_dwAccessRef++;
	m_dwAccessRef=GetTickCount();
}
template <class T>
BOOL CM_List<T>::SearchLeastAccessItem()
{
	CM_ListCell<T>* pSearch=NULL;
	DWORD ref=0;
	Head();
	ref=m_current->m_dwAccessRef;
	pSearch=m_current;
	while (!End())
	{
		if(m_current->m_dwAccessRef<ref)
		{
			pSearch=m_current;
			ref=m_current->m_dwAccessRef;
		}
		Next();
	}
	if(pSearch)
		m_current=pSearch;//item found;

	return pSearch!=NULL;
}
template <class T>
BOOL CM_List<T>::MoveToIndex(INT index)
{
	if(index<0)
		return FALSE;
	Head();
	while (!End())
	{
		if(m_current->m_iIndex== index)
			return TRUE;
		Next();
	}
	return FALSE;
}
template <class T>
BOOL CM_List<T>::Swap(INT index1,INT index2)
{
	CM_ListCell<T> *cell1;
	CM_ListCell<T> *cell2;
	CM_ListCell<T> celltemp;
	INT id;

	Head();
	if(!MoveToIndex(index1))
		return FALSE;
	cell1=BackupCurrent();

	if(!MoveToIndex(index2))
		return FALSE;
	cell2=BackupCurrent();

	celltemp.m_body=cell2->m_body;
	cell2->m_body=cell1->m_body;
	cell1->m_body=celltemp.m_body;

	id=cell2->m_iID;
	cell2->m_iID=cell1->m_iID;
	cell1->m_iID=id;

	return TRUE;
}
template <class T>
BOOL CM_List<T>::Previous(void)
{
	if(m_current)
		m_current = m_current->m_prev;
	return (m_current != NULL);
}

template <class T>
void CM_List<T>::Head(void)
{
	m_current = m_head;
}

template <class T>
void CM_List<T>::Tail(void)
{
	m_current = m_tail;
}
template <class T>
BOOL CM_List<T>::IsHead(void)
{
	return m_current == m_head;
}

template <class T>
BOOL CM_List<T>::IsTail(void)
{
	return m_current == m_tail;
}
template <class T>
void	CM_List<T>::Insert(T *element,INT iIndexBefore,INT id=0)
{
	CM_ListCell<T> *pCursor=m_head;
	while(pCursor)
	{
		if (pCursor->m_iIndex==iIndexBefore)
		{
			break;
		}
		pCursor=pCursor->m_next;
	}
	if(!pCursor)//index not found
		return;

	//add item here:
	CM_ListCell<T> *pNew= new CM_ListCell<T>;
	if(m_head!=pCursor)
	{
		pNew->m_prev=pCursor->m_prev;
		pNew->m_next=pCursor;
		pCursor->m_prev->m_next=pNew;
		pCursor->m_prev=pNew;
	}
	else
	{
		m_head=pNew;
		pNew->m_prev=NULL;
		pNew->m_next=pCursor;
		pCursor->m_prev=pNew;
	}

	//update index and id:
	pNew->m_iID=id;
	pNew->m_iIndex=pCursor->m_iIndex;
	pNew->m_body=element;

	while(pCursor)
	{
		pCursor->m_iIndex++;

		pCursor=pCursor->m_next;
	}

	m_cellCount++;
}
template <class T>
void CM_List<T>::Add(T *element,INT id)
{
	if (!m_cellCount)
	{
		m_current = new CM_ListCell<T>;
		m_head = m_tail = m_current;

		m_current->m_body = element;
		m_current->m_next = m_current->m_prev = NULL;
	}
	else
	{
		m_tail->m_next = new CM_ListCell<T>;
		m_tail->m_next->m_prev = m_tail;
		m_tail = m_tail->m_next;

		m_tail->m_body = element;
		m_current = m_tail;
	}

	m_current->m_iIndex=m_cellCount;
	m_current->m_iID=id;
	m_cellCount++;
}


template <class T>
T *CM_List<T>::Get(void)
{
	if (m_current)
	{
		m_current->m_dwAccessRef=m_dwAccessRef;
		return m_current->m_body;
	}
	else
		return NULL;
}
template <class T>
CM_ListCell<T> *CM_List<T>::GetHeadCell()
{
	return m_head;
}
template <class T>
CM_ListCell<T> *CM_List<T>::GetTailCell()
{
	return m_tail;
}

template <class T>
T *CM_List<T>::GetHead(void)
{
	if (m_head)
		return m_head->m_body;
	else
		return NULL;
}
template <class T>
T *CM_List<T>::GetTail(void)
{
	if (m_tail)
		return m_tail->m_body;
	else
		return NULL;
}

template <class T>
int CM_List<T>::GetCount(void)
{
	return m_cellCount;
}
template <class T>
int CM_List<T>::GetIndex(void)
{
	if (m_current)
		return (m_current->m_iIndex);
	else
		return 0;
}
template <class T>
int CM_List<T>::GetID(void)
{
	if (m_current)
		return (m_current->m_iID);
	else
		return 0;
}
template <class T>
int CM_List<T>::SetID(INT id)
{
	if (m_current)
	{
		m_current->m_iID=id;
		return TRUE;
	}
	else
		return 0;
}
template <class T>
DWORD CM_List<T>::GetAccessRef()
{
	return m_dwAccessRef;
}
template <class T>
BOOL CM_List<T>::End(void)
{
	return (m_current == NULL);
}

template <class T>
BOOL CM_List<T>::Last(void)
{
	if (m_current)
		return (m_current->m_next == NULL);
	else
		return true;
}


template <class T>
void CM_List<T>::RestoreCurrent(CM_ListCell<T> *cell)
{
	m_current = cell;
}

template <class T>
CM_ListCell<T> *CM_List<T>::BackupCurrent(void)
{
	return m_current;
}
template <class T>
BOOL CM_List<T>::DeleteTail()
{
	if(!m_tail)
		return FALSE;

	CM_ListCell<T> *cell=m_tail;
	m_tail=m_tail->m_prev;
	m_tail->m_next=NULL;

	delete cell;

	m_cellCount--;

	return TRUE;
}
template <class T>
BOOL CM_List<T>::Delete(INT index)
{
	CM_ListCell<T> *cell;
	CM_ListCell<T> *cell_pre;

	if(m_cellCount<=0)
		return FALSE;
	if(!MoveToIndex(index))
		return FALSE;
	cell=BackupCurrent();

	if(cell==m_head)
	{
		if(cell==m_tail)
		{
			m_head = m_tail = m_current = NULL;
		}
		else
		{	
			m_head=cell->m_next;
			cell->m_next->m_prev=NULL;

			m_current=cell->m_next;
			while (!End())
			{
				m_current->m_iIndex-=1;
				Next();
			}
			m_current=cell->m_next;
		}
	}
	else if (cell==m_tail)
	{
		if(cell==m_head)
		{
			m_head = m_tail = m_current = NULL;
		}
		else
		{	
			m_tail=cell->m_prev;
			cell->m_prev->m_next=NULL;
			m_current=m_tail;//NULL;
		}
	}
	else
	{	
		cell_pre=cell->m_prev;
		cell_pre->m_next=cell->m_next;
		cell->m_next->m_prev=cell_pre;

		m_current=cell->m_next;
		while (!End())
		{
			m_current->m_iIndex-=1;
			Next();
		}
		m_current=cell->m_next;

	}

	delete cell;

	m_cellCount--;

	return TRUE;
}
