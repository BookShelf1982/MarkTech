#include "HighResTimer.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
    I64 gCPUFrequency = GetCPUFrequency();

    I64 GetCurrentTimestamp()
    {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);
        return time.QuadPart;
    }

    I64 GetCPUFrequency()
    {
        if (gCPUFrequency == 0)
        {
            LARGE_INTEGER time;
            QueryPerformanceFrequency(&time);
            return time.QuadPart;
        }
        else
        {
            return gCPUFrequency;
        }
    }

#endif
}