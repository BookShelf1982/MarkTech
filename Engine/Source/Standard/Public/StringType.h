#pragma once
#include <string.h>

class CString
{
public:
	CString();
	CString(const char* pString);
	CString(const CString& string);
	~CString();

	const char* c_str() { return m_pString; }
	unsigned int size() { return m_nSize; }
private:
	char* m_pString;
	unsigned int m_nSize;
};