#pragma once
#include "PrimitiveTypes\intalias.h"
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

	typedef U32(*PFN_ThreadProc)();
	Thread StartThread(PFN_ThreadProc procAddress);
	void SleepThread(U64 milliseconds);
	void JoinThread(Thread& thread);
	void DestroyThread(Thread& thread);
}