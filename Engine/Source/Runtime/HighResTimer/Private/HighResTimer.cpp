#include "HighResTimer.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	I64 gCPUFrequency = GetCPUFrequency();
	U64 gStartTimestamp = GetCurrentTimestamp();
#ifdef MT_PLATFORM_WINDOWS

	U64 GetCurrentTimestamp()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return (U64)time.QuadPart;
	}

	U64 GetCPUFrequency()
	{
		if (gCPUFrequency == 0)
		{
			LARGE_INTEGER time;
			QueryPerformanceFrequency(&time);
			return (U64)time.QuadPart;
		}
		else
		{
			return gCPUFrequency;
		}
	}
#endif
}