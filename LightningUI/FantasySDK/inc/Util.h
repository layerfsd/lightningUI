#pragma once

#include "common.h"

namespace UTIL
{

#define  PI 3.1415926f

enum MYCODEPAGE
{
	CODEPAGE_GBK,
	CODEPAGE_RFC
};

enum CM_AXIS
{
	CM_X,
	CM_Y,
	CM_Z,
};


unsigned short	GetShort(unsigned char *&data, short order=0x4949, BOOL jump = true);
unsigned int	GetInt(unsigned char *&data, short order=0x4949, BOOL jump = true);
BOOL			CheckShortSize(unsigned int value);
int NextPow2(int a);
BOOL WINAPI Reboot (int nBootMode);
BOOL   HardResetDevice()  ;
INT GetNearInt(GLfloat f);

int UnicodeToMyCode(wchar_t c);
int GBKToMyCode(char *c);
wchar_t MyCodeToUnicode(int c);
void GBKTOUnicode(char *pGBKString,size_t lenGBK,wchar_t*pUniString,size_t lenUni);
unsigned short MyCodeToGBK(int c);
void UnicodeTOGBK(wchar_t*pUniString,size_t lenUni,char *pGBKString,size_t lenGBK);
void UTF8TOUnicode(char *pUTF8String,size_t lenUTF8,wchar_t*pUniString,size_t lenUni);
bool   In(int   Lp,   int   Hp,   int   Value);
char GetConvertLetter(wchar_t c);
char GetPYLetter( wchar_t c , unsigned short *pcGBK);
BOOL GetBitmapSize(const wchar_t *name,SIZE *si);
BOOL GetBitmapInfo(const wchar_t *name,BITMAPINFOHEADER *info);
int ConvertYUV422toRGB565(unsigned char *inbuf,unsigned char *outbuf,int width,int height);
BOOL GetFileNameFromPath(LPCWSTR szFilePath,LPWSTR szFileName,INT nMaxNum);
BOOL GetFileExtensionFromPath(LPCWSTR szFilePath,LPWSTR szFileExtension,INT nMaxNum);
void MultiCharTOUnicode(char *pMultiString,size_t lenMulti,wchar_t*pUniString,size_t lenUni, MYCODEPAGE codePage=CODEPAGE_GBK);

class CM_Vec3
{
	float	m_c[3];
public:
	CM_Vec3(float x=0.0, float y=0.0, float z=0.0);
	CM_Vec3(const CM_Vec3 &vec3);

	float	Normalize(float norm = -1.0);

	inline void Set(float x=0.0, float y=0.0, float z=0.0)
	{
		m_c[0] = x;
		m_c[1] = y;
		m_c[2] = z;
	}

	inline void Set(CM_AXIS axis, float value)
	{
		m_c[axis] = value;
	}

	inline float operator[] (int n) const
	{
		return m_c[n];
	}

	inline float &operator[] (int n)
	{
		return m_c[n];
	}

	inline CM_Vec3 &operator=(const CM_Vec3 &position)
	{
		m_c[0] = position.m_c[0];
		m_c[1] = position.m_c[1];
		m_c[2] = position.m_c[2];
		return *this;
	}

	inline CM_Vec3 operator*(float coeff) const
	{
		CM_Vec3 result;
		result.m_c[0] = m_c[0] * coeff;
		result.m_c[1] = m_c[1] * coeff;
		result.m_c[2] = m_c[2] * coeff;
		return result;
	}

	inline CM_Vec3 operator+(const CM_Vec3 &position) const
	{
		CM_Vec3 result;
		result.m_c[0] = m_c[0] + position.m_c[0];
		result.m_c[1] = m_c[1] + position.m_c[1];
		result.m_c[2] = m_c[2] + position.m_c[2];
		return result;
	}

	inline CM_Vec3 operator-(const CM_Vec3 &position) const
	{
		CM_Vec3 result;
		result.m_c[0] = m_c[0] - position.m_c[0];
		result.m_c[1] = m_c[1] - position.m_c[1];
		result.m_c[2] = m_c[2] - position.m_c[2];
		return result;
	}

	inline CM_Vec3 Cross(const CM_Vec3 &vec3) const
	{
		CM_Vec3 result;
		result.m_c[0] = m_c[1]*vec3.m_c[2] - m_c[2]*vec3.m_c[1];
		result.m_c[1] = m_c[2]*vec3.m_c[0] - m_c[0]*vec3.m_c[2];
		result.m_c[2] = m_c[0]*vec3.m_c[1] - m_c[1]*vec3.m_c[0];
		return result;
	}

	inline float Dot(const CM_Vec3 &vec3) const
	{
		return m_c[0]*vec3.m_c[0] + m_c[1]*vec3.m_c[1] + m_c[2]*vec3.m_c[2];
	}

	static inline CM_Vec3 Quadratic(float time, const CM_Vec3 &P0, const CM_Vec3 &P1, const CM_Vec3 &P2)
	{
		float oneMinusTime = 1.0f-time;
		return P0*oneMinusTime*oneMinusTime + P1*2.0f*oneMinusTime*time + P2*time*time;
	}

	static	inline CM_Vec3 Cubic(float time, const CM_Vec3 &P0, const CM_Vec3 &P1, const CM_Vec3 &P2, const CM_Vec3 &P3)
	{
		float oneMinusTime = 1.0f-time;
		return P0*oneMinusTime*oneMinusTime*oneMinusTime + P1*3.0f*oneMinusTime*oneMinusTime*time 
				+ P2*3.0f*oneMinusTime*time*time + P3*time*time*time;
	}
};


class CM_Vec4
{
	float	m_c[4];
public:
	CM_Vec4(float x=0.0, float y=0.0, float z=0.0, float w=1.0);
	CM_Vec4(const CM_Vec4 &vec4);

	float	Normalize(float norm = -1.0);

	inline void Divide(void)
	{
		if (m_c[3] != 0.0)
		{
			m_c[0] /= m_c[3];
			m_c[1] /= m_c[3];
			m_c[2] /= m_c[3];
		}
	}

	inline void Set(float x=0.0, float y=0.0, float z=0.0, float w=1.0)
	{
		m_c[0] = x;
		m_c[1] = y;
		m_c[2] = z;
		m_c[3] = w;
	}

	inline float operator[] (int n) const
	{
		return m_c[n];
	}

	inline float &operator[] (int n)
	{
		return m_c[n];
	}

	inline CM_Vec4 &operator=(const CM_Vec4 &position)
	{
		m_c[0] = position.m_c[0];
		m_c[1] = position.m_c[1];
		m_c[2] = position.m_c[2];
		m_c[3] = position.m_c[3];
		return *this;
	}

	inline CM_Vec4 operator*(float coeff) const
	{
		CM_Vec4 result;
		result.m_c[0] = m_c[0] * coeff;
		result.m_c[1] = m_c[1] * coeff;
		result.m_c[2] = m_c[2] * coeff;
		result.m_c[3] = m_c[3] * coeff;
		return result;
	}

	inline CM_Vec4 operator+(const CM_Vec4 &position) const
	{
		CM_Vec4 result;
		result.m_c[0] = m_c[0] + position.m_c[0];
		result.m_c[1] = m_c[1] + position.m_c[1];
		result.m_c[2] = m_c[2] + position.m_c[2];
		result.m_c[3] = m_c[3] + position.m_c[3];
		return result;
	}

	inline CM_Vec4 operator-(const CM_Vec4 &position) const
	{
		CM_Vec4 result;
		result.m_c[0] = m_c[0] - position.m_c[0];
		result.m_c[1] = m_c[1] - position.m_c[1];
		result.m_c[2] = m_c[2] - position.m_c[2];
		result.m_c[3] = m_c[3] - position.m_c[3];
		return result;
	}

	inline CM_Vec4 Cross(const CM_Vec4 &vec4) const
	{
		CM_Vec4 result;
		result.m_c[0] = m_c[1]*vec4.m_c[2] - m_c[2]*vec4.m_c[1];
		result.m_c[1] = m_c[2]*vec4.m_c[0] - m_c[0]*vec4.m_c[2];
		result.m_c[2] = m_c[0]*vec4.m_c[1] - m_c[1]*vec4.m_c[0];
		return result;
	}

	inline float Dot(const CM_Vec4 &vec4) const
	{
		return m_c[0]*vec4.m_c[0] + m_c[1]*vec4.m_c[1] + m_c[2]*vec4.m_c[2] + m_c[3]*vec4.m_c[3];
	}
};


class CM_String
{
	wchar_t			*m_string;
	unsigned int	m_size;
public:
	CM_String(void);
	CM_String(const wchar_t *string);
	CM_String(const CM_String &string);
	~CM_String(void);

	void Set(const wchar_t *string);
	void  MakeUpper(void) ;
	void  MakeLower(void) ;
	int LoadFromResource(HINSTANCE hIns,UINT uID);

	inline const wchar_t* String(void) const
	{
		return m_string;
	}

	inline unsigned int Size(void) const
	{
		return m_size;
	}


	inline CM_String &operator=(const CM_String &string)
	{
		if (m_string != NULL)	delete[] m_string;

		if(string.m_string)
		{
			m_size		= string.m_size;
			m_string	= new wchar_t[m_size+1];
			wcscpy_s(m_string, m_size+1, string.m_string);
		}
		else
		{
			m_string=NULL;
			m_size = 0;
		}

		return *this;
	}

	inline CM_String &operator=(const wchar_t* pstr)
	{
		if (m_string != NULL)	
		{
			delete[] m_string;
			m_string=NULL;
		}

		if(pstr)
		{
			m_size		= wcslen(pstr);
			m_string	= new wchar_t[m_size+1];
			wcscpy_s(m_string, m_size+1, pstr);
		}
		else
		{
			m_size = 0;
		}

		return *this;
	}
	inline CM_String operator+(const CM_String &string) const
	{
		CM_String	 result;

		if(string.String())
		{
			result.m_size = m_size + string.m_size;
			result.m_string = new wchar_t[result.m_size+1];
			if(m_string)
				swprintf_s(result.m_string, result.m_size+1, L"%s%s", m_string, string.m_string);
			else
				swprintf_s(result.m_string, result.m_size+1, L"%s", string.m_string);

			return result;
		}
		else
			return CM_String(m_string);

	}
	inline void operator+=(const CM_String &string) 
	{
		if(!string.String())
			return;

		wchar_t *temp=NULL;
		if(m_string)
		{
			temp=new wchar_t[m_size+1];
			wcscpy_s(temp, m_size+1, m_string);
		}

		m_size = m_size + string.m_size;

		if (m_string != NULL)	delete[] m_string;
		m_string = new wchar_t[m_size+1];

		if(temp)
			swprintf_s(m_string, m_size+1, L"%s%s", temp, string.String());
		else
			swprintf_s(m_string, m_size+1, L"%s", string.String());

		if(temp)
			delete[]temp;

	}
	inline void operator-=(int i) 
	{
		if(!m_string)
			return;
		if(i>(INT)m_size)
			i=m_size;

		wchar_t *temp=NULL;
		temp=new wchar_t[m_size+1];
		wcscpy_s(temp, m_size+1, m_string);

		m_size = m_size -i;

		delete[] m_string;
		m_string=NULL;

		if(m_size>0)
		{
			m_string = new wchar_t[m_size+1];

			memcpy(m_string,temp,m_size*2);
			m_string[m_size]='\0';
		}


		delete[]temp;

	}
	inline void operator+=(const wchar_t *pstr) 
	{
		if(!pstr)
			return;
		wchar_t *temp=NULL;
		if(m_string)
		{
			temp=new wchar_t[m_size+1];
			wcscpy_s(temp, m_size+1, m_string);
		}

		m_size = m_size + (pstr?wcslen(pstr):0);

		if (m_string != NULL)	delete[] m_string;
		m_string = new wchar_t[m_size+1];

		if(temp)
			swprintf_s(m_string, m_size+1, L"%s%s", temp, pstr);
		else
			swprintf_s(m_string, m_size+1, L"%s", pstr);

		if(temp)
			delete[]temp;
	}
	inline BOOL operator==(const CM_String &string) 
	{
		if(!string.String())
		{
			if(m_string)
				return FALSE;
			else
				return TRUE;
		}

		return m_string?(0 == wcscmp(string.String(),m_string)):FALSE;
	}
	inline BOOL operator!=(const CM_String &string) 
	{
		if(!string.String())
		{
			if(m_string)
				return TRUE;
			else
				return FALSE;
		}

		return m_string?(0 != wcscmp(string.String(),m_string)):TRUE;
	}
};


class CM_Matrix3x3;

class CM_Matrix4x4
{
	friend class CM_Matrix3x3;
	float	m_c[16];
public:
	CM_Matrix4x4();

	void			Identity(void);
	void			Empty(void);
	CM_Matrix4x4	&operator=(const CM_Matrix4x4 &mat);
	CM_Matrix4x4	operator*(const CM_Matrix4x4 &mat);
	CM_Vec4		operator*(const CM_Vec4 &vec4) const;
	CM_Matrix4x4	operator/(float coef);

	inline float operator[] (int n) const
	{
		return m_c[n];
	}

	inline float &operator[] (int n)
	{
		return m_c[n];
	}

	void			Transpose(void);
	void			Inverse3x3(float *inv3x3);
	CM_Matrix3x3	ExtractMatrix3x3(void);
	float			Determinant(void);
	CM_Matrix4x4	Inverse(void);

	inline float	*Data(void) { return m_c; }
};

//******************************************************************************************************//
//																										//
//******************************************************************************************************//


class CM_Matrix3x3
{
	friend class CM_Matrix4x4;
	float	m_c[9];
public:
	CM_Matrix3x3();

	void			Identity(void);
	void			Empty(void);
	CM_Matrix3x3	&operator=(const CM_Matrix3x3 &mat);
	CM_Matrix3x3	operator*(const CM_Matrix3x3 &mat);

	inline float operator[] (int n) const
	{
		return m_c[n];
	}

	inline float &operator[] (int n)
	{
		return m_c[n];
	}

	void			Transpose(void);
	void			Extract(const CM_Matrix4x4 &matrix4x4);
	void			Inverse(void);
	void			InverseTranspose(void);
	float			Determinant(void);

	inline float	*Data(void) { return m_c; }
};

class CFileLog
{
public:
	CFileLog(const wchar_t* fileName);
	~CFileLog();

	BOOL OutPut(const wchar_t * _Format, ...);

private:
	FILE *m_fp;
	CM_String m_strFileName;
};
}

using namespace UTIL;