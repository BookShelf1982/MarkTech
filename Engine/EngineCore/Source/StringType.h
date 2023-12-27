#pragma once
#include <string.h>

class String
{
public:
	String();
	String(const char* other);
	~String();

	size_t length() { return m_nLength; }

	const char* c_str() { return (const char*)m_pStr; }

	void operator=(const char* other);
	void operator=(char* other);
	void operator=(const String& other);
private:
	char* m_pStr;
	size_t m_nLength;
};