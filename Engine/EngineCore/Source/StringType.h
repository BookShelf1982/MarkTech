#pragma once
#include <string.h>
#include <stdlib.h>

class String
{
public:
	String();
	String(const char* other);
	~String();

	const size_t length() const { return m_nLength; }

	const char* c_str() const { return (const char*)m_pStr; }

	void operator=(const char* other);
	void operator=(char* other);
	void operator=(const String& other);
private:
	char* m_pStr;
	size_t m_nLength;
};

class WideString
{
public:
	WideString();
	WideString(const wchar_t* other);
	~WideString();

	const size_t length() const { return m_nLength; }

	const wchar_t* c_str() const { return (const wchar_t*)m_lpStr; }

	void operator=(const wchar_t* other);
	void operator=(wchar_t* other);
	void operator=(const WideString& other);
private:
	wchar_t* m_lpStr;
	size_t m_nLength;
};