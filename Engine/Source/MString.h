#pragma once
#include "DllExport.h"

class MARKTECH_API CString
{
public:
	CString() 
	{
		str = "";
	}
	CString(const char* string)
	{
		str = (char*)string;
	}

	~CString() { delete str; }

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
	void operator=(wchar_t* other);

private:
	wchar_t* wstr;
};