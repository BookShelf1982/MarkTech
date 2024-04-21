#include "StringType.h"

CString::CString()
	:m_nSize(0), m_pString(nullptr)
{
}

CString::CString(const char* pString)
	:m_nSize(0), m_pString(nullptr)
{
	m_nSize = strlen(pString);
	m_pString = new char[m_nSize + 1];
	memcpy((void*)m_pString, pString, m_nSize);
	m_pString[m_nSize] = 0;
}

CString::CString(const CString& string)
	:m_nSize(string.m_nSize), m_pString(nullptr)
{
	m_pString = new char[m_nSize + 1];
	memcpy(m_pString, string.m_pString, m_nSize+1);
}

CString::~CString()
{
	if (m_pString)
		delete[] m_pString;
}
