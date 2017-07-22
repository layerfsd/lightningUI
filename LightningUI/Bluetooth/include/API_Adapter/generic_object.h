/****************************************************************************

				CSR plc 2007
 	 				
				All rights reserved

FILE:			generic_object.h

DESCRIPTION:	Generic object

REVISION:		$Revision: #1 $

****************************************************************************/
#ifndef _GENERIC_OBJECT_H
#define _GENERIC_OBJECT_H

#define RELEASE_OBJ(var)\
{						\
	if (var)			\
	{					\
		var->Release();	\
		var = NULL;		\
	}					\
}

#define RELEASE_BSTR(var)	\
{							\
	if (var)				\
	{						\
		SysFreeString(var);	\
		var = NULL;			\
	}						\
}

#define RELEASE_OLESTR(var)	\
{							\
	if (var)				\
	{						\
		CoTaskMemFree(var);	\
		var = NULL;			\
	}						\
}

#define VALIDATE(func) \
{	\
	hr = func; \
	if (hr != S_OK) \
	{				\
		break;		\
	}				\
}

#define VALIDATE_MEMORY(ptr) \
{	\
	if (!ptr) \
	{				\
		hr = E_OUTOFMEMORY; \
		break;		\
	}				\
}

#define VALIDATE_WIN32(func) \
{	\
	win32Result = func; \
	hr = HRESULT_FROM_WIN32(win32Result); \
	if (hr != S_OK) \
	{				\
		break;		\
	}				\
}

#define VariantAllocString(v,str) \
{ \
	v.vt = VT_BSTR; \
	v.bstrVal = SysAllocString(str); \
} 
	
typedef enum
{
	GEN_BYTE_ORDER_LITTLE_ENDIAN,
	GEN_BYTE_ORDER_BIG_ENDIAN,
	GEN_BYTE_ORDER_HOST,
} GenericByteOrder;

typedef enum
{
	GEN_CODEPAGE_ANSI = CP_ACP,
	GEN_CODEPAGE_UTF8 = CP_UTF8,
	GEN_CODEPAGE_UCS2 = 100,
} GenericCodepage;

class GenericObject: public IUnknown
{
	private:
		LONG		referenceCount;
		static LONG genericObjectCount;
	public:
		GenericObject();
		virtual ~GenericObject();

		//IUnknown methods
		STDMETHODIMP QueryInterface(REFIID, LPVOID*);
		STDMETHODIMP_(DWORD) AddRef();
		STDMETHODIMP_(DWORD) Release();

		static LONG GetObjectCount() { return genericObjectCount; }
};


#endif