#include "WinModule.h"

CWinModule::CWinModule(const char* pModName)
{
    m_Module = LoadLibraryA(pModName);
    MASSERT(m_Module);
    pfnInit = (pfn_void_func_t)GetProcAddress(m_Module, "InitModule");
    pfnUpdate = (pfn_void_func_t)GetProcAddress(m_Module, "UpdateModule");
    pfnShutdown = (pfn_void_func_t)GetProcAddress(m_Module, "ShutdownModule");
    
    pfnInit(); // Call init pfn
}

CWinModule::~CWinModule()
{
    pfnShutdown();
    FreeLibrary(m_Module);
}

void* CWinModule::GetModuleProcAddress(const char* lpName)
{
    return GetProcAddress(m_Module, lpName);
}

IModule* CWinModuleLoader::LoadModule(const char* pModName)
{
    return new CWinModule(pModName);
}
