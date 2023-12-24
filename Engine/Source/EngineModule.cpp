#include "EngineModule.h"

CEngineModule* CEngineModule::g_pEngineModule = nullptr;

void StartModule()
{
	CEngineModule::g_pEngineModule = new CEngineModule();
}

CEngineModule::CEngineModule()
{
	m_pEngine = new MarkTech::CEngine();
}

CEngineModule::~CEngineModule()
{
	delete m_pEngine;
}

void CEngineModule::PreInit(HINSTANCE phInstance)
{
	hInstance = phInstance;
}

void CEngineModule::Init()
{
}

void CEngineModule::Update()
{
}

void CEngineModule::Shutdown()
{
}
