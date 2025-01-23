#include "Threads\Thread.h"

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
    Thread StartThread(PFN_ThreadProc procAddress)
    {
        Thread thread;

        thread.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)procAddress, NULL, 0, NULL);

        return thread;
    }

    void SleepThread(int32_t milliseconds)
    {
        Sleep(milliseconds);
    }

    void JoinThread(Thread& thread)
    {
        WaitForSingleObject(thread.hThread, INFINITE);
    }

    void DestroyThread(Thread& thread)
    {
        CloseHandle(thread.hThread);
    }
#endif
}