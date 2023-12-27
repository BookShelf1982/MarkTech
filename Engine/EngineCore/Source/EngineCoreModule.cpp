#include "EngineCoreModule.h"
#include <Windows.h>

CEngineCoreModule* CEngineCoreModule::g_pEngineCoreModule = new CEngineCoreModule();

CEngineCoreModule::CEngineCoreModule()
	: m_pEngine(nullptr)
{
	m_pEngine = new CEngine();
}

CEngineCoreModule::~CEngineCoreModule()
{
	m_pEngine->DestroyEngine();
	delete m_pEngine;
}

void CEngineCoreModule::PreInit( HINSTANCE hInstance, pfnStopGameLoop pfn )
{
	m_hInstance = hInstance;
	m_pfnStopGameLoop = pfn;
}

void CEngineCoreModule::Init()
{
	m_pEngine->InitEngine(m_pfnStopGameLoop);
}

void CEngineCoreModule::Update()
{
	m_pEngine->UpdateEngine();
}

void CEngineCoreModule::Shutdown()
{
	delete g_pEngineCoreModule;
}

MTRESULT GetInterface(IModuleInterface** pInterface)
{
	*pInterface = CEngineCoreModule::g_pEngineCoreModule;
	return MT_OK;
}