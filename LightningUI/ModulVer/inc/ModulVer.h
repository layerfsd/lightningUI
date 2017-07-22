#ifndef __MODULEVER_H
#define __MODULEVER_H

#include <windows.h>
#include <xstring>

using namespace std;

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

//////////////////
// CModuleVersion version info about a module.
// To use:
//
// CModuleVersion ver
// if (ver.GetFileVersionInfo("_T("mymodule))) {
//		// info is in ver, you can call GetValue to get variable info like
//		CString s = ver.GetValue(_T("CompanyName"));
// }
//
// You can also call the static fn DllGetVersion to get DLLVERSIONINFO.
//
class CModuleVersion : public VS_FIXEDFILEINFO {
protected:
	BYTE* m_pVersionInfo;	// all version info

	struct TRANSLATION {
		WORD langID;			// language ID
		WORD charset;			// character set (code page)
	} m_translation;

public:
	CModuleVersion();
	virtual ~CModuleVersion();

	BOOL		GetFileVersionInfo(LPCTSTR modulename);
	tstring		GetValue(LPCTSTR lpKeyName);
};

#endif
