#include "MemoryArena.h"
#include <Assert.h>
#include <stdlib.h>
#include <memory.h>

namespace MarkTech
{
    char* gpMemoryArena = nullptr;
    char* gpMemoryCursor = nullptr;

    bool InitMemoryArena(U64 bytesToAllocate)
    {
        MT_ASSERT(gpMemoryArena == nullptr);
        gpMemoryArena = (char*)malloc(bytesToAllocate);
        memset(gpMemoryArena, 0, bytesToAllocate);
        gpMemoryCursor = gpMemoryArena;
        return true;
    }

    void* AllocFromMemoryArena(U64 numOfBytes)
    {
        char* ptr = gpMemoryCursor;
        gpMemoryCursor += numOfBytes;
        return ptr;
    }

    void FreeToPointer(void* ptr)
    {
        MT_ASSERT((char*)ptr < gpMemoryCursor);
        MT_ASSERT((char*)ptr >= gpMemoryArena);
        memset(ptr, 0, gpMemoryCursor - (char*)ptr);
        gpMemoryCursor = (char*)ptr;
    }

    void KillMemoryArena()
    {
        free(gpMemoryArena);
    }
}