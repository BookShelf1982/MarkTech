#include "String\String.h"
#include "Memory\AlignedAllocator.h"
#include <stdio.h>
#include <string.h>
#include <cstdarg>

namespace MarkTech
{
	String::String()
		:m_pString(nullptr), m_Length(0)
	{
	}

	String::String(const char* str)
		:m_pString(nullptr), m_Length(0)
	{
		U64 size = strlen(str);
		if (size > 32)
			return;

		m_pString = (char*)AllocAligned(size, 1);
		m_Length = size;

		strcpy(m_pString, str);
	}

	String::~String()
	{
		if (m_pString)
			FreeAligned(m_pString);
	}

	void String::Format(const char* format, ...)
	{
	}

	void String::operator=(const char* str)
	{
		U64 size = strlen(str);
		m_Length = size;
		if (!m_pString)
			m_pString = (char*)AllocAligned(size, 1);

		strcpy(m_pString, str);
	}
}