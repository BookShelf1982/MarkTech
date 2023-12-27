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
