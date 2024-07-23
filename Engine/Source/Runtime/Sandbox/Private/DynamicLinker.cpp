#include "DynamicLinker.h"
#include <MemoryArena.h>
#include <Log.h>
#include <CVar.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif


#ifdef MT_PLATFORM_WINDOWS
HMODULE gCoreModule = NULL;
#endif

PFN_InitMemoryArena InitMemoryArena = nullptr;
PFN_AllocFromMemoryArena AllocFromMemoryArena = nullptr;
PFN_FreeToPointer FreeToPointer = nullptr;
PFN_KillMemoryArena KillMemoryArena = nullptr;

PFN_DebugPrintF DebugPrintF = nullptr;

PFN_GetCVar GetCVar = nullptr;
PFN_SetCVar SetCVar = nullptr;
PFN_GiveCVarArray GiveCVarArray = nullptr;

bool LinkCoreLibrary(const char* pPathToDLL)
{
#ifdef MT_PLATFORM_WINDOWS
    gCoreModule = LoadLibraryA(pPathToDLL);
    
    if (gCoreModule == NULL)
            return false;
    
    InitMemoryArena = (PFN_InitMemoryArena)GetProcAddress(gCoreModule, "InitMemoryArena");
    AllocFromMemoryArena = (PFN_AllocFromMemoryArena)GetProcAddress(gCoreModule, "AllocFromMemoryArena");
    FreeToPointer = (PFN_FreeToPointer)GetProcAddress(gCoreModule, "FreeToPointer");
    KillMemoryArena = (PFN_KillMemoryArena)GetProcAddress(gCoreModule, "KillMemoryArena");
    
    DebugPrintF = (PFN_DebugPrintF)GetProcAddress(gCoreModule, "DebugPrintF");
    
    GetCVar = (PFN_GetCVar)GetProcAddress(gCoreModule, "GetCVar");
    SetCVar = (PFN_SetCVar)GetProcAddress(gCoreModule, "SetCVar");
    GiveCVarArray = (PFN_GiveCVarArray)GetProcAddress(gCoreModule, "GiveCVarArray");
    
    return true;
#else
    return false;
#endif
}

void UnlinkCoreLibrary()
{
#ifdef MT_PLATFORM_WINDOWS
    FreeLibrary(gCoreModule);
#endif
}