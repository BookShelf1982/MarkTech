#pragma once
#include "DllExport.h"

class MARKTECH_API CString
{
public:
	CString();
	CString(const char* str);
	~CString();

	void SetString(char* other);

	char* c_str() { return str; }

	void operator=(CString other);

private:
	char* str;
};

class CWString
{
public:
	CWString();
	CWString(const wchar_t* str);
	~CWString();

	void SetString(wchar_t* other);

	wchar_t* c_str() { return wstr; }

	void operator=(CWString other);

private:
	wchar_t* wstr;
};