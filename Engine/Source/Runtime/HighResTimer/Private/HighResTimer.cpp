#include "HighResTimer.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif


namespace MarkTech
{
    I64 HighResTimer::GetCurrentTimestamp()
    {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);
        return time.QuadPart;
    }
}