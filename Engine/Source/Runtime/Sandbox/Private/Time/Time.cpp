#include "Time.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	double g_clockFrequency = 0;

	void StartTime()
	{
		LARGE_INTEGER integer;
		QueryPerformanceFrequency(&integer);
		g_clockFrequency = (double)integer.QuadPart;
	}

	double GetTime()
	{
		LARGE_INTEGER integer;
		QueryPerformanceCounter(&integer);
		return (double)integer.QuadPart / g_clockFrequency;
	}
}