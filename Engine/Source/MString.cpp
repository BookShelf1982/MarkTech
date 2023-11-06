#include "MString.h"
//////////
//String//
//////////

CString::CString()
{
	str = "";
}

CString::CString(const char* String)
{
	str = (char*)String;
}

CString::~CString()
{
	delete [] str;
}

void CString::SetString(char* other)
{
	str = other;
}

void CString::operator=(CString other)
{
	SetString(other.c_str());
}


///////////
//WString//
///////////
CWString::CWString()
{
	wstr = L"";
}

CWString::CWString(const wchar_t* WString)
{
	wstr = (wchar_t*)WString;
}

CWString::~CWString()
{
	delete [] wstr;
}

void CWString::SetString(wchar_t* other)
{
	wstr = (wchar_t*)other;
}

void CWString::operator=(CWString other)
{
	SetString(other.c_str());
}
