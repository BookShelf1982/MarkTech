#include "ThreadSync.h"
namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	Mutex MakeMutex()
	{
		Mutex mutex = {};
		mutex.hMutex = CreateMutexW(NULL, FALSE, NULL);
		return mutex;
	}

	void LockMutex(Mutex* pMutex)
	{
		WaitForSingleObject(pMutex->hMutex, INFINITE);
	}

	void UnlockMutex(Mutex* pMutex)
	{
		ReleaseMutex(pMutex->hMutex);
	}

	void DestroyMutex(Mutex* pMutex)
	{
		CloseHandle(pMutex->hMutex);
	}
#endif
}