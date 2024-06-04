#include "String\String.h"
#include <string.h>

namespace MarkTech
{
	String::String()
		:m_pString(nullptr), m_Length(0)
	{
	}

	String::~String()
	{

	}

	void String::operator=(const char* string)
	{
		U64 size = strlen(string);
		m_pString = (char*)DebugStringManager::AllocStringDbg(size);
		m_Length = size;
		memcpy(m_pString, string, m_Length + 1);
		m_pString[m_Length] = 0;
	}
}