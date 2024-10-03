#pragma once
#include <PrimitiveTypes.h>

#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	struct Mutex
	{
		HANDLE hMutex;
	};

	Mutex MakeMutex();
	void LockMutex(Mutex* pMutex);
	void UnlockMutex(Mutex* pMutex);
	void DestroyMutex(Mutex* pMutex);
#endif
}