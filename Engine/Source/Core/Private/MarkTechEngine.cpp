#include "MarkTechEngine.h"

CEngine* CEngine::m_pIstance = nullptr;


CEngine* CEngine::CreateEngine(const char** ppRequiredModules, uint32_t nModulesCount)
{
    MASSERT(!m_pIstance);
    m_pIstance = new CEngine(ppRequiredModules, nModulesCount);
    return m_pIstance;
}

void CEngine::Release()
{
    CEngine::DestroyEngine();
}

CEngine::CEngine(const char** ppStrings, uint32_t nStringCount)
{
    // -- Load all libraries -- //
#ifdef MT_PLATFORM_WINDOWS
    CWinModuleLoader loader;
    for (uint32_t i = 0; i < nStringCount; i++)
    {
        m_LoadedModules.push_back(loader.LoadModule(ppStrings[i]));
    }
#endif
}

CEngine::~CEngine()
{
    for (size_t i = 0; i < m_LoadedModules.size(); i++)
    {
        delete m_LoadedModules[i];
    }
}

void CEngine::DestroyEngine()
{
    MASSERT(m_pIstance);
    delete m_pIstance;   
}

IMarkTechEngine* CreateMarkTechEngine(const char** ppStrings, uint32_t nStringCount)
{
    return CEngine::CreateEngine(ppStrings, nStringCount);
}
