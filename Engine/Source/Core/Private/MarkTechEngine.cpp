#include "MarkTechEngine.h"
#include <fstream>

CEngine* CEngine::m_pIstance = nullptr;

CEngine* CEngine::CreateEngine(const MEngineData& data)
{
    MASSERT(!m_pIstance);
    m_pIstance = new CEngine(data);
    return m_pIstance;
}

void CEngine::Release()
{
    CEngine::DestroyEngine();
}

void CEngine::Update()
{
    for (uint32_t i = 0; i < m_LoadedModules.size(); i++)
    {
        m_LoadedModules[i]->pfnUpdate();
    }

    m_pfnWindowFuncs.pfnPollWindow();
    m_pRenderer->RenderFrame();
}

bool CEngine::WantToQuit()
{
    return m_bClosing;
}

CEngine::CEngine(MEngineData engData)
    :m_bClosing(false), m_pWindowModule(nullptr), m_Data(engData)
{
    // -- Load all libraries -- //
#ifdef MT_PLATFORM_WINDOWS
    CWinModuleLoader loader;
    for (uint32_t i = 0; i < engData.modules.Size(); i++)
    {
        m_LoadedModules.push_back(loader.LoadModule(engData.modules[i].c_str()));
        
        if (engData.modules[i] == "Window.dll")
        {
            m_pWindowModule = m_LoadedModules[i];
        }
        if (engData.modules[i] == "ResourceManager.dll")
        {
            m_pResourceManagerModule = m_LoadedModules[i];
        }
    }
#endif

    // -- Get ptr funcs for specific modules -- //

    // -- Window module -- //
    m_pfnWindowFuncs.pfnCreateWindow = (PFN_CreateWindow)m_pWindowModule->GetModuleProcAddress("CreateGLFWWindow");
    m_pfnWindowFuncs.pfnPollWindow = (PFN_PollWindow)m_pWindowModule->GetModuleProcAddress("PollWindow");
    m_pfnWindowFuncs.pfnDestroyWindow = (PFN_DestroyWindow)m_pWindowModule->GetModuleProcAddress("DestroyGLFWWindow");
    m_pfnWindowFuncs.pfnSetWindowCloseCallback = (PFN_SetWindowCloseCallback)m_pWindowModule->GetModuleProcAddress("SetWindowCloseCallback");
    m_pfnWindowFuncs.pfnSetWindowUserPointer = (PFN_SetWindowUserPointer)m_pWindowModule->GetModuleProcAddress("SetWindowUserPointer");
    m_pfnWindowFuncs.pfnGetWindowUserPointer = (PFN_GetWindowUserPointer)m_pWindowModule->GetModuleProcAddress("GetWindowUserPointer");
    m_pfnWindowFuncs.pfnGetWindowHWND = (PFN_GetWindowHWND)m_pWindowModule->GetModuleProcAddress("GetWindowHWND");

    // -- Resource Manager Module -- //
    m_pfnResourceManagerFuncs.pfnCreateResourceManager = (PFN_CreateResourceManager)m_pResourceManagerModule->GetModuleProcAddress("CreateResourceManager");

    Init();
}

CEngine::~CEngine()
{
    m_pfnWindowFuncs.pfnDestroyWindow(m_pWindow);
    delete m_pRenderer;
    delete m_pResourceManager;

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

void CEngine::Init()
{
    m_pWindow = m_pfnWindowFuncs.pfnCreateWindow(1280, 720, m_Data.gameName.c_str());
    m_pfnWindowFuncs.pfnSetWindowUserPointer(m_pWindow, this);

    m_pfnWindowFuncs.pfnSetWindowCloseCallback(m_pWindow, WindowCloseCallback);
#ifdef MT_PLATFORM_WINDOWS
    m_pRenderer = new C3DRenderer(m_pfnWindowFuncs.pfnGetWindowHWND(m_pWindow));
#endif

    m_pResourceManager = m_pfnResourceManagerFuncs.pfnCreateResourceManager();
}

void CEngine::Quit()
{
    m_bClosing = true;
}

IMarkTechEngine* CreateMarkTechEngine(const MEngineData& data)
{
    return CEngine::CreateEngine(data);
}

void WindowCloseCallback(GLFWwindow* pWindow)
{
    reinterpret_cast<CEngine*>(CEngine::GetWindowFuncs().pfnGetWindowUserPointer(pWindow))->Quit();
}
