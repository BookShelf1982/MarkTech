#include "Debug\Log.h"
#include <stdio.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	Log* Log::m_gpLog = nullptr;

	Log::Log()
	{
		if (!m_gpLog)
			m_gpLog = this;
	}

	Log::~Log()
	{
	}

	void Log::PrintF(const char* msg, ...)
	{
		va_list argList;
		va_start(argList, msg);

		const U32 MAX_CHARS = 1024;
		char buffer[MAX_CHARS];
		vsnprintf(buffer, MAX_CHARS, msg, argList);

#ifdef MT_PLATFORM_WINDOWS
		OutputDebugStringA(buffer);
#endif
		va_end(argList);
	}
}