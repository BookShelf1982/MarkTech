#pragma once
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	struct Mutex
	{
#ifdef MT_PLATFORM_WINDOWS
		HANDLE hMutex;
#endif
	};

	Mutex MakeMutex();
	void AcquireMutex(Mutex& mutex);
	void FreeMutex(Mutex& mutex);
	void DestoryMutex(Mutex& mutex);
}