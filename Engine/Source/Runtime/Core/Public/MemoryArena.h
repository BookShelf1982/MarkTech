#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>

namespace MarkTech
{
#define KILOBYTE 1024
#define MEGABYTE 1048576

#ifdef MT_CORELIB
	MT_DLLEXPORT bool InitMemoryArena(U64 bytesToAllocate);
	MT_DLLEXPORT void* AllocFromMemoryArena(U64 numOfBytes);
	MT_DLLEXPORT void FreeToPointer(void* ptr);
	MT_DLLEXPORT void KillMemoryArena();
}
#else
}
typedef bool	(*PFN_InitMemoryArena)(MarkTech::U64 bytesToAllocate);
typedef void*	(*PFN_AllocFromMemoryArena)(MarkTech::U64 numOfBytes);
typedef void	(*PFN_FreeToPointer)(void* ptr);
typedef void	(*PFN_KillMemoryArena)();

extern PFN_InitMemoryArena InitMemoryArena;
extern PFN_AllocFromMemoryArena AllocFromMemoryArena;
extern PFN_FreeToPointer FreeToPointer;
extern PFN_KillMemoryArena KillMemoryArena;

#endif