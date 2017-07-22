#pragma once
#include "controlgl.h"
#include "ListBoxGL.h"
#include "ButtonGL.h"

#define FILE_TYPE_FOLDER			0
#define FILE_TYPE_UNKNOWN		1

class CExplorerGL :
	public CControlGL
{
public:
	CExplorerGL(void);
public:
	~CExplorerGL(void);

public:
	BOOL Initialize(UINT id,CControlGL* pParent,CSkinManagerGL *pSkinManager);
	void Render(void);
	BOOL IsReady(void);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnGesture(GESTUREINFO *pgi);

	void MoveToNext();
	void MoveToPrevious();
	void OnEnter();

	void LinkTextGL(CTextGL *pTextGL);
	void GotoParent(void);
	BOOL IsRoot(void);

	const wchar_t *GetPath(void);
	const wchar_t *GetFileName(void);
	BOOL IsClickOnItem(POINT *pt);
	BOOL IsCurFocusFile();
	void SetFilter(const wchar_t *pFilter);
	void SetCurPath(const wchar_t* lpszPath);
	void Refresh();
	UINT GetTask();
	void SetTask(UINT idTask);
private:
	void ExplorePath(const wchar_t* lpszPath,const wchar_t* lpszFilter=NULL);
	BOOL IsAudioFile(LPCTSTR szFileName);
	BOOL IsVideoFile(LPCTSTR szFileName);
	BOOL IsImageFile(LPCTSTR szFileName);

private:
	CListBoxGL		m_listboxExplorer;

	CLabelGL		m_iconBlank;
	CLabelGL		m_iconFolder;
	CLabelGL		m_iconStore;
	CLabelGL		m_iconAudio;
	CLabelGL		m_iconVideo;
	CLabelGL		m_iconImage;

	CLabelGL		m_iconBlankFocus;
	CLabelGL		m_iconFolderFocus;
	CLabelGL		m_iconStoreFocus;
	CLabelGL		m_iconAudioFocus;
	CLabelGL		m_iconVideoFocus;
	CLabelGL		m_iconImageFocus;

	CM_String			m_strCurPath;
	CM_String			m_strCurFile;
	BOOL				m_bPathChanged;
	CM_String			m_strFilter;
	UINT					m_idTask;
};
