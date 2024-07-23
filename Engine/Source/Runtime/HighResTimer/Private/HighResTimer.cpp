#include "HighResTimer.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif


namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
    I64 GetCurrentTimestamp()
    {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);
        return time.QuadPart;
    }
#endif
}