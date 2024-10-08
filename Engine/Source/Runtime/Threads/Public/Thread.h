#pragma once
#include <PrimitiveTypes.h>

#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	struct Thread
	{
		HANDLE hThread;
	};

	Thread MakeThread(void* pStartAddress);
#endif

	void PutThreadToSleep(U32 miliseconds);
}