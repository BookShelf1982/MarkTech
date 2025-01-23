#pragma once
#include <stdint.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	struct Thread
	{
#ifdef MT_PLATFORM_WINDOWS
		HANDLE hThread;
#endif
	};

	typedef uint32_t(*PFN_ThreadProc)();
	Thread StartThread(PFN_ThreadProc procAddress);
	void SleepThread(int32_t milliseconds);
	void JoinThread(Thread& thread);
	void DestroyThread(Thread& thread);
}