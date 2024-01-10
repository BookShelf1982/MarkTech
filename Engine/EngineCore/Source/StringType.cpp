#include "StringType.h"

String::String()
	:m_pStr(""), m_nLength(0)
{
}

String::String(const char* other)
	:m_pStr((char*)other), m_nLength(0)
{
	m_nLength = strlen(other);
}

String::~String()
{
}

void String::operator=(const char* other)
{
	m_pStr = (char*)other;
	m_nLength = strlen(other);
}

void String::operator=(char* other)
{
	m_pStr = other;
	m_nLength = strlen(other);
}

void String::operator=(const String& other)
{
	m_pStr = other.m_pStr;
	m_nLength = other.m_nLength;
}



WideString::WideString()
	:m_lpStr(L""), m_nLength(0)
{
}

WideString::WideString(const wchar_t* other)
	:m_lpStr((wchar_t*)other)
{
	m_nLength = wcslen(other);
}

WideString::~WideString()
{
}

void WideString::operator=(const wchar_t* other)
{
	m_lpStr = (wchar_t*)other;
	m_nLength = wcslen(other);
}

void WideString::operator=(wchar_t* other)
{
	m_lpStr = other;
	m_nLength = wcslen(other);
}

void WideString::operator=(const WideString& other)
{
	m_lpStr = other.m_lpStr;
	m_nLength = other.m_nLength;
}

WideString StringToWideString(String str)
{
	wchar_t wstr[128];
	size_t size = 128;
	mbstowcs_s(&size, wstr, str.c_str(), 128);

	return WideString(wstr);
}
