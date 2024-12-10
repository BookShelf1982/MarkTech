#include "Mutex.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
    Mutex MakeMutex()
    {
        Mutex mutex;
#ifdef MT_PLATFORM_WINDOWS
        mutex.hMutex = CreateMutexW(NULL, FALSE, NULL);
#endif
        return mutex;
    }

    void AcquireMutex(Mutex& mutex)
    {
#ifdef MT_PLATFORM_WINDOWS
        WaitForSingleObject(mutex.hMutex, INFINITE);
#endif
    }

    void FreeMutex(Mutex& mutex)
    {
#ifdef MT_PLATFORM_WINDOWS
        ReleaseMutex(mutex.hMutex);
#endif
    }

    void DestoryMutex(Mutex& mutex)
    {
#ifdef MT_PLATFORM_WINDOWS
        CloseHandle(mutex.hMutex);
#endif
    }

}