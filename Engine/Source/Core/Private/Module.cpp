#include "Module.h"
#include "WinModule.h"

IModule* IModuleLoader::LoadModule(const char* pModName)
{
#ifdef MT_PLATFORM_WINDOWS
    return CWinModuleLoader::LoadModule(pModName);
#else
    return nullptr;
#endif
}
