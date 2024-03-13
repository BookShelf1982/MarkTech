#include "WinModule.h"

CWinModule::CWinModule(const char* pModName)
{
    m_Module = LoadLibraryA(pModName);
}

CWinModule::~CWinModule()
{
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
