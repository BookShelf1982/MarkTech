#include "Thread.h"

#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#include <synchapi.h>
#endif

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	Thread MakeThread(void* pStartAddress)
	{
		Thread thread = {};
		thread.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pStartAddress, NULL, 0, 0);
		return thread;
	}
#endif
	void PutThreadToSleep(U32 miliseconds)
	{
		Sleep(miliseconds);
	}
}