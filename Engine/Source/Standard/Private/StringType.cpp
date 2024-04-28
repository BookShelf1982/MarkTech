#include "StringType.h"

CString::CString()
{
}

CString::CString(unsigned int nSize)
	:m_Str(nSize)
{
}

CString::CString(const char* pString)
	:m_Str(pString)
{
}

CString::CString(const CString& string)
	:m_Str(string.m_Str)
{
}

CString::~CString()
{
}

CString CString::SubString(unsigned int nStartIndex, unsigned int nLength)
{
	CString str(nLength);
	memcpy((void*)str.c_str(), m_Str.c_str() + (char)nStartIndex, nLength);
	return str;
}

unsigned int CString::FindFirstOf(char character, unsigned int nOffset = 0)
{
	for (unsigned int i = nOffset; i < m_Str.size(); i++)
	{
		if (m_Str.c_str()[i] == character)
			return i;
	}
	return 0;
}

bool CString::operator==(const CString& str) const
{
	return m_Str == str.m_Str;
}

bool CString::operator==(const char* pStr) const
{
	return m_Str == pStr;
}

void CString::operator=(const char* pStr)
{
	m_Str = pStr;
}

void CString::operator=(const CString& str)
{
	m_Str = str.m_Str;
}