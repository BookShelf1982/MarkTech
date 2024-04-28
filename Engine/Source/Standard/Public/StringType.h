#pragma once
#include "AllocatorType.h"
#include <string.h>

template<typename T, class A = CAllocator>
class CTBaseString
{
public:
	CTBaseString();
	CTBaseString(unsigned int nSize);
	CTBaseString(const char* pString);
	CTBaseString(const CTBaseString& string);
	~CTBaseString();

	const T* c_str() const { return m_pString; }
	unsigned int size() const { return m_nSize; }

	bool operator==(const CTBaseString& str) const;
	bool operator==(const T* pStr) const;

	void operator=(const T* pStr);
	void operator=(const CTBaseString& str);
private:
	T* m_pString;
	unsigned int m_nSize;
	A m_Allocator;
};

template<typename T, class A>
inline CTBaseString<T, A>::CTBaseString()
	:m_nSize(0), m_pString(nullptr)
{
	m_pString = (T*)m_Allocator.Allocate(m_nSize + 1);
	m_pString[m_nSize] = 0;
}

template<typename T, class A>
inline CTBaseString<T, A>::CTBaseString(unsigned int nSize)
	:m_nSize(nSize), m_pString(nullptr)
{
	m_pString = (T*)m_Allocator.Allocate(m_nSize + 1);
	memset(m_pString, 0, m_nSize);
	m_pString[m_nSize] = 0;
}

template<typename T, class A>
inline CTBaseString<T, A>::CTBaseString(const char* pString)
	:m_nSize(0), m_pString(nullptr)
{
	m_nSize = (unsigned int)strlen(pString);
	m_pString = (T*)m_Allocator.Allocate(m_nSize + 1);
	memcpy((void*)m_pString, pString, m_nSize);
	m_pString[m_nSize] = 0;
}

template<typename T, class A>
inline CTBaseString<T, A>::CTBaseString(const CTBaseString& string)
	:m_nSize(string.m_nSize), m_pString(nullptr)
{
	m_pString = (T*)m_Allocator.Allocate(m_nSize + 1);
	memcpy(m_pString, string.m_pString, m_nSize + 1);
}

template<typename T, class A>
inline CTBaseString<T, A>::~CTBaseString()
{
	if (m_pString)
		m_Allocator.Deallocate(m_pString);
}

template<typename T, class A>
inline bool CTBaseString<T, A>::operator==(const CTBaseString& str) const
{
	return (strcmp(m_pString, str.m_pString) == 0);
}

template<typename T, class A>
inline bool CTBaseString<T, A>::operator==(const T* pStr) const
{
	return (strcmp(m_pString, pStr) == 0);
}

template<typename T, class A>
inline void CTBaseString<T, A>::operator=(const T* pStr)
{
	if (m_pString)
		m_Allocator.Deallocate(m_pString);
	m_nSize = 0;

	m_nSize = (unsigned int)strlen(pStr);
	m_pString = (T*)m_Allocator.Allocate(m_nSize + 1);
	memcpy((void*)m_pString, pStr, m_nSize);
	m_pString[m_nSize] = 0;
}

template<typename T, class A>
inline void CTBaseString<T, A>::operator=(const CTBaseString& str)
{
	if (m_pString)
		m_Allocator.Deallocate(m_pString);
	m_nSize = 0;

	m_nSize = str.m_nSize;
	m_pString = (T*)m_Allocator.Allocate(m_nSize + 1);
	memcpy((void*)m_pString, str.m_pString, m_nSize);
	m_pString[m_nSize] = 0;
}

class CString
{
public:
	CString();
	CString(unsigned int nSize);
	CString(const char* pString);
	CString(const CString& string);
	~CString();

	const char* c_str() const { return m_Str.c_str(); }
	unsigned int Size() const { return m_Str.size(); }

	CString SubString(unsigned int nStartIndex, unsigned int nLength);

	unsigned int FindFirstOf(char character, unsigned int nOffset);

	bool operator==(const CString& str) const;
	bool operator==(const char* pStr) const;

	void operator=(const char* pStr);
	void operator=(const CString& str);
private:
	CTBaseString<char> m_Str;
};